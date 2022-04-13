#include "PuJetIdSF.h"

#define DEBUG false

// Constructor
PuJetIdSF::PuJetIdSF (std::string directory, std::string year)
{

  // UL: single file for all periods, and for efficiencies, SFs, and unc. 
  std::string nf = directory + "/PUID_106XTraining_ULRun2_EffSFandUncties_v1.root";
  TFile *f = new TFile(nf.c_str(),"READ");


  // UL: no mistag eff/sf
  std::string nh_eff           = "h2_eff_mcUL"+year+"_L";
  std::string nh_eff_sf        = "h2_eff_sfUL"+year+"_L";
  std::string nh_eff_sf_err    = "h2_eff_sfUL"+year+"_L_Systuncty";

  // Get the TH2F histograms
  h_eff_           = (TH2F*) f->Get(nh_eff.c_str());
  h_eff_SF_        = (TH2F*) f->Get(nh_eff_sf.c_str());
  h_eff_SF_err_    = (TH2F*) f->Get(nh_eff_sf_err.c_str());
}


// Destructor
PuJetIdSF::~PuJetIdSF()
{
  if (h_eff_) delete h_eff_;
  if (h_eff_SF_) delete h_eff_SF_;
  if (h_eff_SF_err_) delete h_eff_SF_err_;
}

// check bit
bool PuJetIdSF::CheckBit (int number, int bitpos)
{
  bool res = number & (1 << bitpos);
  return res;
}

// getEff
float PuJetIdSF::getEff(bool isReal, float pt, float eta)
{
  float eff = 1.;

  if (pt<20.) pt=20.;
  if (pt>50.) pt=50.;

  if (isReal)
  {
    int binglobal = h_eff_->FindBin (pt, eta);
    int binx, biny, binz;
    h_eff_->GetBinXYZ (binglobal, binx, biny, binz); // converts to x, y bins
    int nx = h_eff_->GetNbinsX();
    int ny = h_eff_->GetNbinsY();

    // under-overflows
    if (binx < 1) binx = 1;
    if (biny < 1) biny = 1;
    if (binx > nx) binx = nx;
    if (biny > ny) biny = ny;

    eff = h_eff_->GetBinContent (binx, biny);
  }
  return eff;
}


// getSF
float PuJetIdSF::getSF(bool isReal, float pt, float eta)
{
  float sf = 1.;

  if (pt<20.) pt=20.;
  if (pt>50.) pt=50.;

  if (isReal)
  {
    int binglobal = h_eff_SF_->FindBin (pt, eta);
    int binx, biny, binz;
    h_eff_SF_->GetBinXYZ (binglobal, binx, biny, binz); // converts to x, y bins
    int nx = h_eff_SF_->GetNbinsX();
    int ny = h_eff_SF_->GetNbinsY();

    // under-overflows
    if (binx < 1) binx = 1;
    if (biny < 1) biny = 1;
    if (binx > nx) binx = nx;
    if (biny > ny) biny = ny;

    sf = h_eff_SF_->GetBinContent (binx, biny);
  }

  return sf;
}

// getSFError
float PuJetIdSF::getSFError(bool isReal, float pt, float eta)
{
  float sferr = 0.;

  if (pt<20.) pt=20.;
  if (pt>50.) pt=50.;

  if (isReal)
  {
    int binglobal = h_eff_SF_err_->FindBin (pt, eta);
    int binx, biny, binz;
    h_eff_SF_err_->GetBinXYZ (binglobal, binx, biny, binz); // converts to x, y bins
    int nx = h_eff_SF_err_->GetNbinsX();
    int ny = h_eff_SF_err_->GetNbinsY();

    // under-overflows
    if (binx < 1) binx = 1;
    if (biny < 1) biny = 1;
    if (binx > nx) binx = nx;
    if (biny > ny) biny = ny;

    sferr = h_eff_SF_err_->GetBinContent (binx, biny);
  }
  // no mistag in UL
//  else
//  {
//    if (fabs(eta) > 2.5)
//    {
//      int binglobal = h_mistag_SF_err_->FindBin (pt, eta);
//      int binx, biny, binz;
//      h_mistag_SF_err_->GetBinXYZ (binglobal, binx, biny, binz); // converts to x, y bins
//      int nx = h_mistag_SF_err_->GetNbinsX();
//      int ny = h_mistag_SF_err_->GetNbinsY();
//
//      // under-overflows
//      if (binx < 1) binx = 1;
//      if (biny < 1) biny = 1;
//      if (binx > nx) binx = nx;
//      if (biny > ny) biny = ny;
//
//      sferr = h_mistag_SF_err_->GetBinContent (binx, biny);
//    }
//    else /* fabs(eta) < 2.5 */
//    {
//      // inside tracker acceptance use the SF itself as error on the mistag
//      sferr = getSF(isReal, pt, eta);
//    }
//  }

  return sferr;
}

//getEvtWeight - Returns a vector with {central SF, SF_up, SF_down}
std::vector<float> PuJetIdSF::getEvtWeight(bigTree &theBigTree, TLorentzVector tau1, TLorentzVector tau2, std::map<int,double> jets_and_smearFactor, bool cleanJets)
{
  // Weight for each event
  std::vector<float> eventWeight(15); // keeping 15 values so that not to change too much skimmers; can be updated
  float SF = 1.;
  float SF_up   = 1.;
  float SF_down = 1.;

  if (DEBUG) std::cout << "----- PuJetIdSF -----" << std::endl;

  // Loop on jets
  for (unsigned int iJet = 0; iJet < theBigTree.jets_px->size(); ++iJet)
  {
    // JET PF ID cut
    if (theBigTree.PFjetID->at (iJet) < 1) continue; // 0 ; don't pass PF Jet ID; 1: tight, 2: tightLepVeto

    // Build jet
    TLorentzVector tlv_jet(theBigTree.jets_px->at (iJet), theBigTree.jets_py->at (iJet), theBigTree.jets_pz->at (iJet), theBigTree.jets_e->at (iJet));

    // Apply the smearing (it's already set to 1 if !isMC or !doSmearing)
    tlv_jet = tlv_jet * jets_and_smearFactor[iJet];

    if (DEBUG) std::cout << " iJet: " << iJet << "  pt: " << tlv_jet.Pt() << " eta: " << tlv_jet.Eta() << " phi: " << tlv_jet.Phi() << std::endl;

    // Kinematic selections
    if (tlv_jet.Pt() < 20 || tlv_jet.Pt() > 50) continue;
    if (TMath::Abs(tlv_jet.Eta()) > 4.7) continue;
    if (tlv_jet.DeltaR(tau1) < 0.5) continue;
    if (tlv_jet.DeltaR(tau2) < 0.5) continue;

    // Apply further cleaning for 2017 noisy jets, as suggested by HTT group: https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorkingLegacyRun2#Jets
    // The noisy jets to be removed are defined as: 20 < pt < 50 && abs(eta) > 2.65 && abs(eta) < 3.139
    if (cleanJets && TMath::Abs(tlv_jet.Eta()) > 2.65 && TMath::Abs(tlv_jet.Eta()) < 3.139) continue;

    // Build genJet and check if it is matched
    bool isRealJet = false;
    for (unsigned int igj = 0; igj < theBigTree.genjet_px->size(); igj++)
    {
      TLorentzVector tlv_genJet(theBigTree.genjet_px->at(igj),theBigTree.genjet_py->at(igj),theBigTree.genjet_pz->at(igj),theBigTree.genjet_e->at(igj));
      if (tlv_jet.DeltaR(tlv_genJet)<0.4)
      {
        isRealJet = true;
        if (DEBUG) std::cout << "  matched genJet pt: " << tlv_genJet.Pt() << " eta: " << tlv_genJet.Eta() << " phi: " << tlv_genJet.Phi() << std::endl;
        break;
      }
    }

    // Pass PU jet ID Loose WP
    bool passPUjetIDLoose = CheckBit(theBigTree.jets_PUJetID_WP->at(iJet), 2);

    if (DEBUG) std::cout << "   isRealJet: " << isRealJet << " passPUjetIDLoose: " << passPUjetIDLoose << std::endl;

    // Get SF
    float sf  = getSF (isRealJet, tlv_jet.Pt(), tlv_jet.Eta());

    float sfErr   = getSFError(isRealJet, tlv_jet.Pt(), tlv_jet.Eta());
    float sf_up   = std::clamp(sf + sfErr, (float)0, (float)5);
    float sf_down = std::clamp(sf - sfErr, (float)0, (float)5);

    if (DEBUG) std::cout << "   eff: " << eff << " sf: " << sf << " sfErr: " << sfErr << " sf_up: " << sf_up << " sf_down: " <<sf_down << std::endl;

    // compute eventWeight
    if (passPUjetIDLoose)
    {
      SF      *= eff;
      SF_up   *= sf_up;
      SF_down *= sf_down;
    }
    if (DEBUG) std::cout << "   P_MC: " << P_MC << " P_DATA: " << P_DATA << " weight: " << (P_DATA/P_MC) << " weight_up: " << (P_DATA_up/P_MC) << " weight_down: " << (P_DATA_down/P_MC) << std::endl;

  } // end loop on jets

  eventWeight.at(0) = SF;
  eventWeight.at(1) = SF_up;
  eventWeight.at(2) = SF_down;
  eventWeight.at(3)  = 1.;
  eventWeight.at(4)  = 1.;
  eventWeight.at(5)  = 1.;
  eventWeight.at(6)  = 1.;
  eventWeight.at(7)  = 1.;
  eventWeight.at(8)  = 1.;
  eventWeight.at(9)  = 1.;
  eventWeight.at(10) = 1.;
  eventWeight.at(11) = 1.;
  eventWeight.at(12) = 1.;
  eventWeight.at(13) = 1.;
  eventWeight.at(14) = 1.;

  return eventWeight;
}
