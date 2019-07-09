#include "../interface/TauTriggerSFs2017.h"

#include <TString.h> // Form

#include <iostream> // std::cerr, std::endl
#include <iomanip> 
#include <assert.h> // assert
#include <cmath> // std::sqrt

const TH1* loadTH1(const TFile* inputFile, const std::string& histogramName)
{
  const TH1* histogram = dynamic_cast<TH1*>((const_cast<TFile*>(inputFile))->Get(histogramName.data()));
  if ( !histogram ) {
    std::cerr << "Failed to load histogram = '" << histogramName << "' from input file !!" << std::endl;
    assert(0);
  }
  return histogram;
}


const TH2* loadTH2(const TFile* inputFile, const std::string& histogramName)
{
  const TH2* histogram = dynamic_cast<TH2*>((const_cast<TFile*>(inputFile))->Get(histogramName.data()));
  if ( !histogram ) {
    std::cerr << "Failed to load histogram = '" << histogramName << "' from input file !!" << std::endl;
    assert(0);
  }
  return histogram;
}


const TF1* loadTF1(const TFile* inputFile, const std::string& functionName)
{
  const TF1* function = dynamic_cast<TF1*>((const_cast<TFile*>(inputFile))->Get(functionName.data()));
  if ( !function ) {
    std::cerr << "Failed to load function = '" << functionName << "' from input file !!" << std::endl;
    assert(0);
  }
  return function;
}


// Make sure we stay on our histograms
double ptCheck(double pt)
{
  if      ( pt > 450 ) pt = 450;
  else if ( pt < 20 )  pt = 20;
  return pt;
}


// Make sure to have only old DMs, DM0, DM1, DM10
int dmCheck(int dm )
{
  if (dm == 2) dm = 1; // Originally, DM=2 was included in oldDM, but with the dynamic strip clustering the second strip was reconstructed together with the first one. So it ends up to DM=1. But,there are still some cases where DM=2 survives.
  return dm;
}


TauTriggerSFs2017::TauTriggerSFs2017(const std::string& inputFileName, const std::string& trigger, const std::string& year, const std::string& tauWP, const std::string& wpType)
  : inputFileName_(inputFileName),
    trigger_(trigger),
    year_(year),
    tauWP_(tauWP),
    wpType_(wpType)
{
  inputFile_ = new TFile(inputFileName_.data());
  if ( !inputFile_ ) {
    std::cerr << "Failed to open input file = '" << inputFileName_ << "' !!" << std::endl;
    assert(0);
  }

  // Load the TF1s containing the analytic best-fit results
  // This is done per decay mode: 0, 1, 10.
  fitDataMap_ [ 0] = loadTF1(inputFile_, Form("%s_%s%s_dm0_DATA_fit", trigger_.data(), tauWP_.data(), wpType_.data()));
  fitDataMap_ [ 1] = loadTF1(inputFile_, Form("%s_%s%s_dm1_DATA_fit", trigger_.data(), tauWP_.data(), wpType_.data()));
  fitDataMap_ [10] = loadTF1(inputFile_, Form("%s_%s%s_dm10_DATA_fit", trigger_.data(), tauWP_.data(), wpType_.data()));
  fitMCMap_ [ 0] = loadTF1(inputFile_, Form("%s_%s%s_dm0_MC_fit", trigger_.data(), tauWP_.data(), wpType_.data()));
  fitMCMap_ [ 1] = loadTF1(inputFile_, Form("%s_%s%s_dm1_MC_fit", trigger_.data(), tauWP_.data(), wpType_.data()));
  fitMCMap_ [10] = loadTF1(inputFile_, Form("%s_%s%s_dm10_MC_fit", trigger_.data(), tauWP_.data(), wpType_.data()));
  

  // Load the TH1s containing the analytic best-fit result in 1 GeV incriments and the associated uncertainty.
  // This is done per decay mode: 0, 1, 10.
  fitUncDataMap_ [ 0] = loadTH1(inputFile_, Form("%s_%s%s_dm0_DATA_errorBand", trigger_.data(), tauWP_.data(), wpType_.data()));
  fitUncDataMap_ [ 1] = loadTH1(inputFile_, Form("%s_%s%s_dm1_DATA_errorBand", trigger_.data(), tauWP_.data(), wpType_.data()));
  fitUncDataMap_ [10] = loadTH1(inputFile_, Form("%s_%s%s_dm10_DATA_errorBand", trigger_.data(), tauWP_.data(), wpType_.data()));
  fitUncMCMap_ [ 0] = loadTH1(inputFile_, Form("%s_%s%s_dm0_MC_errorBand", trigger_.data(), tauWP_.data(), wpType_.data()));
  fitUncMCMap_ [ 1] = loadTH1(inputFile_, Form("%s_%s%s_dm1_MC_errorBand", trigger_.data(), tauWP_.data(), wpType_.data()));
  fitUncMCMap_ [10] = loadTH1(inputFile_, Form("%s_%s%s_dm10_MC_errorBand", trigger_.data(), tauWP_.data(), wpType_.data()));



  // Because of low statistics in the problem region of the barrel, we apply the Eta-Phi corrections
  // based on taus firing mutau trigger and passing the vloose MVA WP. This provides the most statistically
  // robust measurement for the correction. Considering the three Eta-Phi regions should not have significantly
  // different SF adjustments for different MVA WPs, this should also be a safe choice.
  std::string etaPhiWP = "vloose";
  std::string etaPhiTrigger = "mutau";

  // Load the TH2s containing the eta phi efficiency corrections
  // This is done per decay mode: 0, 1, 10.
  effEtaPhiDataMap_ [ 0] = loadTH2(inputFile_, Form("%s_%s%s_dm0_DATA", etaPhiTrigger.data(), etaPhiWP.data(), wpType_.data()));
  effEtaPhiDataMap_ [ 1] = loadTH2(inputFile_, Form("%s_%s%s_dm1_DATA", etaPhiTrigger.data(), etaPhiWP.data(), wpType_.data()));
  effEtaPhiDataMap_ [10] = loadTH2(inputFile_, Form("%s_%s%s_dm10_DATA", etaPhiTrigger.data(), etaPhiWP.data(), wpType_.data()));
  effEtaPhiMCMap_ [ 0] = loadTH2(inputFile_, Form("%s_%s%s_dm0_MC", etaPhiTrigger.data(), etaPhiWP.data(), wpType_.data()));
  effEtaPhiMCMap_ [ 1] = loadTH2(inputFile_, Form("%s_%s%s_dm1_MC", etaPhiTrigger.data(), etaPhiWP.data(), wpType_.data()));
  effEtaPhiMCMap_ [10] = loadTH2(inputFile_, Form("%s_%s%s_dm10_MC", etaPhiTrigger.data(), etaPhiWP.data(), wpType_.data()));


  // Eta Phi Averages
  // This is done per decay mode: 0, 1, 10.
  effEtaPhiAvgDataMap_ [ 0] = loadTH2(inputFile_, Form("%s_%s%s_dm0_DATA_AVG", etaPhiTrigger.data(), etaPhiWP.data(), wpType_.data()));
  effEtaPhiAvgDataMap_ [ 1] = loadTH2(inputFile_, Form("%s_%s%s_dm1_DATA_AVG", etaPhiTrigger.data(), etaPhiWP.data(), wpType_.data()));
  effEtaPhiAvgDataMap_ [10] = loadTH2(inputFile_, Form("%s_%s%s_dm10_DATA_AVG", etaPhiTrigger.data(), etaPhiWP.data(), wpType_.data()));
  effEtaPhiAvgMCMap_ [ 0] = loadTH2(inputFile_, Form("%s_%s%s_dm0_MC_AVG", etaPhiTrigger.data(), etaPhiWP.data(), wpType_.data()));
  effEtaPhiAvgMCMap_ [ 1] = loadTH2(inputFile_, Form("%s_%s%s_dm1_MC_AVG", etaPhiTrigger.data(), etaPhiWP.data(), wpType_.data()));
  effEtaPhiAvgMCMap_ [10] = loadTH2(inputFile_, Form("%s_%s%s_dm10_MC_AVG", etaPhiTrigger.data(), etaPhiWP.data(), wpType_.data()));
}


TauTriggerSFs2017::~TauTriggerSFs2017()
{
  delete inputFile_;
}


double getEfficiency(double pt, double eta, double phi, const TF1* fit, const TH1* uncHist, const TH2* etaPhiHist, const TH2* etaPhiAvgHist, const std::string uncert="Nominal")
{
  double pt_checked = ptCheck( pt );
  double eff = fit->Eval( pt_checked );

  // Shift the pt dependent efficiency by the fit uncertainty if requested
  if (uncert != "Nominal")
  {
    if ( (uncert!="Up") && (uncert!="Down") )
    {
      std::cerr << "Uncertainties are provided using 'Up'/'Down'. You provided uncert = '" << uncert << "'"<< std::endl;
      assert(0);
    }
    if (uncert == "Up") eff += uncHist->GetBinError( (const_cast<TH1*>(uncHist))->FindBin( pt_checked ) );
    else                eff -= uncHist->GetBinError( (const_cast<TH1*>(uncHist))->FindBin( pt_checked ) );
  }

  // Adjust SF based on (eta, phi) location
  // keep eta barrel boundaries within SF region
  // but, for taus outside eta limits or with unralistic
  // phi values, return zero SF
  if      (eta ==  2.1) eta =  2.09;
  else if (eta == -2.1) eta = -2.09;

  double etaPhiVal = etaPhiHist->GetBinContent( (const_cast<TH2*>(etaPhiHist))->FindBin( eta, phi ) );
  double etaPhiAvg = etaPhiAvgHist->GetBinContent( (const_cast<TH2*>(etaPhiAvgHist))->FindBin( eta, phi ) );
  if (etaPhiAvg <= 0.)
  {
    std::cout << Form("One of the provided tau (eta, phi) values (%3.3f, %3.3f) is outside the boundary of triggering taus", eta, phi) << std::endl;
    std::cout << "Returning efficiency = 0.0" << std::endl;
    return 0.;
  }
  eff *= etaPhiVal/etaPhiAvg;
  if ( eff > 1. ) eff = 1.;
  if ( eff < 0. ) eff = 0.; // Some efficiency fits go negative at very low tau pT, prevent that.
  return eff;
}


// Return the data efficiency or the +/- 1 sigma uncertainty shifted efficiency
double TauTriggerSFs2017::getTriggerEfficiencyData(double pt, double eta, double phi, int dm) const
{
  int dm_checked = dmCheck( dm );
  if ( (dm_checked!=0) && (dm_checked!=1) && (dm_checked!=10) )
  {
    std::cerr << Form("Efficiencies only provided for DMs 0, 1, 10.  You provided DM %i", dm_checked) << std::endl;
    assert(0);
  }
  return getEfficiency(pt, eta, phi, fitDataMap_.at(dm_checked), fitUncDataMap_.at(dm_checked), effEtaPhiDataMap_.at(dm_checked), effEtaPhiAvgDataMap_.at(dm_checked));
}

double TauTriggerSFs2017::getTriggerEfficiencyDataUncertUp(double pt, double eta, double phi, int dm) const
{
  int dm_checked = dmCheck( dm );
  if ( (dm_checked!=0) && (dm_checked!=1) && (dm_checked!=10) )
  {
    std::cerr << Form("Efficiencies only provided for DMs 0, 1, 10.  You provided DM %i", dm_checked) << std::endl;
    assert(0);
  }
  return getEfficiency(pt, eta, phi, fitDataMap_.at(dm_checked), fitUncDataMap_.at(dm_checked), effEtaPhiDataMap_.at(dm_checked), effEtaPhiAvgDataMap_.at(dm_checked), "Up");
}

double TauTriggerSFs2017::getTriggerEfficiencyDataUncertDown(double pt, double eta, double phi, int dm) const
{
  int dm_checked = dmCheck( dm );
  if ( (dm_checked!=0) && (dm_checked!=1) && (dm_checked!=10) )
  {
    std::cerr << Form("Efficiencies only provided for DMs 0, 1, 10.  You provided DM %i", dm_checked) << std::endl;
    assert(0);
  }
  return getEfficiency(pt, eta, phi, fitDataMap_.at(dm_checked), fitUncDataMap_.at(dm_checked), effEtaPhiDataMap_.at(dm_checked), effEtaPhiAvgDataMap_.at(dm_checked), "Down");
}


// Return the MC efficiency or the +/- 1 sigma uncertainty shifted efficiency
double TauTriggerSFs2017::getTriggerEfficiencyMC(double pt, double eta, double phi, int dm) const
{
  int dm_checked = dmCheck( dm );
  if ( (dm_checked!=0) && (dm_checked!=1) && (dm_checked!=10) )
  {
    std::cerr << Form("Efficiencies only provided for DMs 0, 1, 10.  You provided DM %i", dm_checked) << std::endl;
    assert(0);
  }
  return getEfficiency(pt, eta, phi, fitMCMap_.at(dm_checked), fitUncMCMap_.at(dm_checked), effEtaPhiMCMap_.at(dm_checked), effEtaPhiAvgMCMap_.at(dm_checked));
}

double TauTriggerSFs2017::getTriggerEfficiencyMCUncertUp(double pt, double eta, double phi, int dm) const
{
  int dm_checked = dmCheck( dm );
  if ( (dm_checked!=0) && (dm_checked!=1) && (dm_checked!=10) )
  {
    std::cerr << Form("Efficiencies only provided for DMs 0, 1, 10.  You provided DM %i", dm_checked) << std::endl;
    assert(0);
  }
  return getEfficiency(pt, eta, phi, fitMCMap_.at(dm_checked), fitUncMCMap_.at(dm_checked), effEtaPhiMCMap_.at(dm_checked), effEtaPhiAvgMCMap_.at(dm_checked), "Up");
}

double TauTriggerSFs2017::getTriggerEfficiencyMCUncertDown(double pt, double eta, double phi, int dm) const
{
  int dm_checked = dmCheck( dm );
  if ( (dm_checked!=0) && (dm_checked!=1) && (dm_checked!=10) )
  {
    std::cerr << Form("Efficiencies only provided for DMs 0, 1, 10.  You provided DM %i", dm_checked) << std::endl;
    assert(0);
  }
  return getEfficiency(pt, eta, phi, fitMCMap_.at(dm_checked), fitUncMCMap_.at(dm_checked), effEtaPhiMCMap_.at(dm_checked), effEtaPhiAvgMCMap_.at(dm_checked), "Down");
}


// Return the data/MC scale factor
double TauTriggerSFs2017::getTriggerScaleFactor(double pt, double eta, double phi, int dm) const
{
  double pt_checked = ptCheck( pt );
  int dm_checked = dmCheck( dm );
  double effData = getTriggerEfficiencyData( pt_checked, eta, phi, dm_checked );
  double effMC = getTriggerEfficiencyMC( pt_checked, eta, phi, dm_checked );
  if ( effMC < 1e-5 )
  {
    std::cerr << "Eff MC is suspiciously low. Please contact Tau POG." << std::endl;
    std::cerr << Form(" - %s Trigger SF for Tau ID: %s   WP: %s   pT: %f   eta: %f   phi: %f", trigger_.data(), wpType_.data(), tauWP_.data(), pt, eta, phi) << std::endl;
    std::cerr << Form(" - MC Efficiency = %f", effMC) << std::endl;
    return 0.;
  }
  double sf = effData / effMC;
  return sf;
}


// return the data/MC scale factor with +1/-1 sigma uncertainty.
// Data and MC fit uncertainties are treated as uncorrelated.
// The calculated uncertainties are symmetric. Do error propagation
// for simple division. Using getTriggerEfficiencyXXXUncertDown instead
// of Up ensures we have the full uncertainty reported. Up sometimes
// is clipped by efficiency max of 1.0.
double TauTriggerSFs2017::getTriggerScaleFactorUncert(double pt, double eta, double phi, int dm, const std::string& uncert) const
{
  if ( (uncert!="Up") && (uncert!="Down") )
  {
    std::cerr << "Uncertainties are provided using 'Up'/'Down'. You provided uncert = '" << uncert << "'"<< std::endl;
    assert(0);
  }

  double pt_checked = ptCheck( pt );
  int dm_checked = dmCheck( dm );

  double effData = getTriggerEfficiencyData( pt_checked, eta, phi, dm_checked );
  double effDataDown = getTriggerEfficiencyDataUncertDown( pt_checked, eta, phi, dm_checked );
  double relDataDiff = (effData - effDataDown) / effData;

  double effMC = getTriggerEfficiencyMC( pt_checked, eta, phi, dm_checked );
  double effMCDown = getTriggerEfficiencyMCUncertDown( pt_checked, eta, phi, dm_checked );
  if (effMC < 1e-5)
  {
    // Already printed an error for the nominal case...
    return 0.0;
  }
  double relMCDiff = (effMC - effMCDown) / effMC;

  double deltaSF = std::sqrt( (relDataDiff*relDataDiff) + (relMCDiff*relMCDiff) );
  double sf = (effData / effMC);
  if (uncert == "Up") return sf * (1. + deltaSF);
  else                return sf * (1. - deltaSF);
}
