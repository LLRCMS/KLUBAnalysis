#include "PuJetIdSF.h"

#define DEBUG false

// Constructor
PuJetIdSF::PuJetIdSF (std::string directory, std::string year)
{
  // Build file names
  std::string nf_eff       = directory + "/h2_eff_mc_" + year + "_L.root";
  std::string nf_eff_sf    = directory + "/h2_eff_sf_" + year + "_L.root";
  std::string nf_mistag    = directory + "/h2_mistag_mc_" + year + "_L.root";
  std::string nf_mistag_sf = directory + "/h2_mistag_sf_" + year + "_L.root";
  std::string nf_sf_err    = directory + "/scalefactorsPUID_81Xtraining.root";

  // Open files
  TFile* f_eff       = new TFile(nf_eff.c_str(),"READ");
  TFile* f_eff_sf    = new TFile(nf_eff_sf.c_str(),"READ");
  TFile* f_mistag    = new TFile(nf_mistag.c_str(),"READ");
  TFile* f_mistag_sf = new TFile(nf_mistag_sf.c_str(),"READ");
  TFile* f_sf_err    = new TFile(nf_sf_err.c_str(),"READ");

  // Build TH2F names
  std::string nh_eff           = "h2_eff_mc"+year+"_L";
  std::string nh_eff_sf        = "h2_eff_sf"+year+"_L";
  std::string nh_eff_sf_err    = "h2_eff_sf"+year+"_L_Systuncty";
  std::string nh_mistag        = "h2_mistag_mc"+year+"_L";
  std::string nh_mistag_sf     = "h2_mistag_sf"+year+"_L";
  std::string nh_mistag_sf_err = "h2_mistag_sf"+year+"_L_Systuncty";

  // Get the TH2F histograms
  h_eff_           = (TH2F*) f_eff->Get(nh_eff.c_str());
  h_eff_SF_        = (TH2F*) f_eff_sf->Get(nh_eff_sf.c_str());
  h_eff_SF_err_    = (TH2F*) f_sf_err->Get(nh_eff_sf_err.c_str());
  h_mistag_        = (TH2F*) f_mistag->Get(nh_mistag.c_str());
  h_mistag_SF_     = (TH2F*) f_mistag_sf->Get(nh_mistag_sf.c_str());
  h_mistag_SF_err_ = (TH2F*) f_sf_err->Get(nh_mistag_sf_err.c_str());
}


// Destructor
PuJetIdSF::~PuJetIdSF() 
{
  if (h_eff_) delete h_eff_;
  if (h_eff_SF_) delete h_eff_SF_;
  if (h_eff_SF_err_) delete h_eff_SF_err_;
  if (h_mistag_) delete h_mistag_;
  if (h_mistag_SF_) delete h_mistag_SF_;
  if (h_mistag_SF_err_) delete h_mistag_SF_err_;
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
  float eff;

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
  else
  {
    int binglobal = h_mistag_->FindBin (pt, eta);
    int binx, biny, binz;
    h_mistag_->GetBinXYZ (binglobal, binx, biny, binz); // converts to x, y bins
    int nx = h_mistag_->GetNbinsX();
    int ny = h_mistag_->GetNbinsY();
    
    // under-overflows
    if (binx < 1) binx = 1;
    if (biny < 1) biny = 1;
    if (binx > nx) binx = nx;
    if (biny > ny) biny = ny;

    eff = h_mistag_->GetBinContent (binx, biny);
  }

  return eff;
}


// getSF
float PuJetIdSF::getSF(bool isReal, float pt, float eta)
{
  float sf;

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
  else
  {
    int binglobal = h_mistag_SF_->FindBin (pt, eta);
    int binx, biny, binz;
    h_mistag_SF_->GetBinXYZ (binglobal, binx, biny, binz); // converts to x, y bins
    int nx = h_mistag_SF_->GetNbinsX();
    int ny = h_mistag_SF_->GetNbinsY();
    
    // under-overflows
    if (binx < 1) binx = 1;
    if (biny < 1) biny = 1;
    if (binx > nx) binx = nx;
    if (biny > ny) biny = ny;

    sf = h_mistag_SF_->GetBinContent (binx, biny);
  }

  return sf;
}

// getSFError
float PuJetIdSF::getSFError(bool isReal, float pt, float eta)
{
  float sferr;

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
  else
  {
    if (fabs(eta) > 2.5)
    {
      int binglobal = h_mistag_SF_err_->FindBin (pt, eta);
      int binx, biny, binz;
      h_mistag_SF_err_->GetBinXYZ (binglobal, binx, biny, binz); // converts to x, y bins
      int nx = h_mistag_SF_err_->GetNbinsX();
      int ny = h_mistag_SF_err_->GetNbinsY();

      // under-overflows
      if (binx < 1) binx = 1;
      if (biny < 1) biny = 1;
      if (binx > nx) binx = nx;
      if (biny > ny) biny = ny;

      sferr = h_mistag_SF_err_->GetBinContent (binx, biny);
    }
    else /* fabs(eta) < 2.5 */
    {
      // inside tracker acceptance use the SF itself as error on the mistag
      sferr = getSF(isReal, pt, eta);
    }
  }

  return sferr;
}

//getEvtWeight - Returns a vector with {central SF, SF_up, SF_down}
std::vector<float> PuJetIdSF::getEvtWeight(bigTree &theBigTree, TLorentzVector tau1, TLorentzVector tau2, std::map<int,double> jets_and_smearFactor, bool cleanJets)
{
  // Weight for each event
  std::vector<float> eventWeight(15);
  float P_MC   = 1.;
  float P_DATA = 1.;
  float P_DATA_up   = 1.;
  float P_DATA_down = 1.;
  float P_DATA_effic_up    = 1.;
  float P_DATA_effic_down  = 1.;
  float P_DATA_mistag_up   = 1.;
  float P_DATA_mistag_down = 1.;
  float P_DATA_effic_eta_s2p5_up    = 1.;
  float P_DATA_effic_eta_s2p5_down  = 1.;
  float P_DATA_effic_eta_l2p5_up    = 1.;
  float P_DATA_effic_eta_l2p5_down  = 1.;
  float P_DATA_mistag_eta_s2p5_up   = 1.;
  float P_DATA_mistag_eta_s2p5_down = 1.;
  float P_DATA_mistag_eta_l2p5_up   = 1.;
  float P_DATA_mistag_eta_l2p5_down = 1.;

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
    bool passPUjetIDLoose = CheckBit(theBigTree.jets_PUJetIDupdated_WP->at(iJet), 2);

    if (DEBUG) std::cout << "   isRealJet: " << isRealJet << " passPUjetIDLoose: " << passPUjetIDLoose << std::endl;

    // Get Efficiency and SF
    float eff = getEff(isRealJet, tlv_jet.Pt(), tlv_jet.Eta());
    float sf  = getSF (isRealJet, tlv_jet.Pt(), tlv_jet.Eta());

    float sfErr   = getSFError(isRealJet, tlv_jet.Pt(), tlv_jet.Eta());
    float sf_up   = std::clamp(sf + sfErr, (float)0, (float)5);
    float sf_down = std::clamp(sf - sfErr, (float)0, (float)5);

    if (DEBUG) std::cout << "   eff: " << eff << " sf: " << sf << " sfErr: " << sfErr << " sf_up: " << sf_up << " sf_down: " <<sf_down << std::endl;

    // compute eventWeight
    if (passPUjetIDLoose)
    {
      P_MC *= eff;
      P_DATA *= sf*eff;
      P_DATA_up *= sf_up*eff;
      P_DATA_down *= sf_down*eff;
      P_DATA_effic_up *= sf_up*eff;
      P_DATA_effic_down *= sf_down*eff;
      P_DATA_mistag_up *= sf*eff;    // true jet --> use nominal SF for mistag
      P_DATA_mistag_down *= sf*eff;  // true jet --> use nominal SF for mistag

      if (fabs(tlv_jet.Eta()) <= 2.5)
      {
        P_DATA_effic_eta_s2p5_up    *= sf_up*eff;
        P_DATA_effic_eta_s2p5_down  *= sf_down*eff;
        P_DATA_effic_eta_l2p5_up    *= sf*eff;
        P_DATA_effic_eta_l2p5_down  *= sf*eff;
        P_DATA_mistag_eta_s2p5_up   *= sf*eff;
        P_DATA_mistag_eta_s2p5_down *= sf*eff;
        P_DATA_mistag_eta_l2p5_up   *= sf*eff;
        P_DATA_mistag_eta_l2p5_down *= sf*eff;
      }
      else /*fabs(tlv_jet.Eta()) > 2.5*/
      {
        P_DATA_effic_eta_s2p5_up    *= sf*eff;
        P_DATA_effic_eta_s2p5_down  *= sf*eff;
        P_DATA_effic_eta_l2p5_up    *= sf_up*eff;
        P_DATA_effic_eta_l2p5_down  *= sf_down*eff;
        P_DATA_mistag_eta_s2p5_up   *= sf*eff;
        P_DATA_mistag_eta_s2p5_down *= sf*eff;
        P_DATA_mistag_eta_l2p5_up   *= sf*eff;
        P_DATA_mistag_eta_l2p5_down *= sf*eff;
      }
    }
    else /*don't pass PUjetIDLoose*/
    {
      P_MC *= (1. - eff);
      P_DATA *= (1. - sf*eff);
      P_DATA_up *= (1. - sf_up*eff);
      P_DATA_down *= (1. - sf_down*eff);
      P_DATA_effic_up *= (1. - sf*eff);    // fake jet --> use nominal SF for effic
      P_DATA_effic_down *= (1. - sf*eff);  // fake jet --> use nominal SF for effic
      P_DATA_mistag_up *= (1. - sf_up*eff);
      P_DATA_mistag_down *= (1. - sf_down*eff);

      if (fabs(tlv_jet.Eta()) <= 2.5)
      {
        P_DATA_effic_eta_s2p5_up    *= (1. - sf*eff);
        P_DATA_effic_eta_s2p5_down  *= (1. - sf*eff);
        P_DATA_effic_eta_l2p5_up    *= (1. - sf*eff);
        P_DATA_effic_eta_l2p5_down  *= (1. - sf*eff);
        P_DATA_mistag_eta_s2p5_up   *= (1. - sf_up*eff);
        P_DATA_mistag_eta_s2p5_down *= (1. - sf_down*eff);
        P_DATA_mistag_eta_l2p5_up   *= (1. - sf*eff);
        P_DATA_mistag_eta_l2p5_down *= (1. - sf*eff);
      }
      else /*fabs(tlv_jet.Eta()) > 2.5*/
      {
        P_DATA_effic_eta_s2p5_up    *= (1. - sf*eff);
        P_DATA_effic_eta_s2p5_down  *= (1. - sf*eff);
        P_DATA_effic_eta_l2p5_up    *= (1. - sf*eff);
        P_DATA_effic_eta_l2p5_down  *= (1. - sf*eff);
        P_DATA_mistag_eta_s2p5_up   *= (1. - sf*eff);
        P_DATA_mistag_eta_s2p5_down *= (1. - sf*eff);
        P_DATA_mistag_eta_l2p5_up   *= (1. - sf_up*eff);
        P_DATA_mistag_eta_l2p5_down *= (1. - sf_down*eff);
      }
    }
    if (DEBUG) std::cout << "   P_MC: " << P_MC << " P_DATA: " << P_DATA << " weight: " << (P_DATA/P_MC) << " weight_up: " << (P_DATA_up/P_MC) << " weight_down: " << (P_DATA_down/P_MC) << std::endl;

  } // end loop on jets

  eventWeight.at(0) = P_DATA      / P_MC;
  eventWeight.at(1) = P_DATA_up   / P_MC;
  eventWeight.at(2) = P_DATA_down / P_MC;
  eventWeight.at(3) = P_DATA_effic_up    / P_MC;
  eventWeight.at(4) = P_DATA_effic_down  / P_MC;
  eventWeight.at(5) = P_DATA_mistag_up   / P_MC;
  eventWeight.at(6) = P_DATA_mistag_down / P_MC;
  eventWeight.at(7)  = P_DATA_effic_eta_s2p5_up    / P_MC;
  eventWeight.at(8)  = P_DATA_effic_eta_s2p5_down  / P_MC;
  eventWeight.at(9)  = P_DATA_effic_eta_l2p5_up    / P_MC;
  eventWeight.at(10) = P_DATA_effic_eta_l2p5_down  / P_MC;
  eventWeight.at(11) = P_DATA_mistag_eta_s2p5_up   / P_MC;
  eventWeight.at(12) = P_DATA_mistag_eta_s2p5_down / P_MC;
  eventWeight.at(13) = P_DATA_mistag_eta_l2p5_up   / P_MC;
  eventWeight.at(14) = P_DATA_mistag_eta_l2p5_down / P_MC;

  return eventWeight;
}
