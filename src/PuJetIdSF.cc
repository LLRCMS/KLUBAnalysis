#include "PuJetIdSF.h"

// Constructor
PuJetIdSF::PuJetIdSF (std::string directory, std::string year)
{
  // Build file names
  std::string nf_eff    = directory + "/h2_eff_mc_" + year + "_L.root";
  std::string nf_eff_sf = directory + "/h2_eff_sf_" + year + "_L.root";
  std::string nf_mistag    = directory + "/h2_mistag_mc_" + year + "_L.root";
  std::string nf_mistag_sf = directory + "/h2_mistag_sf_" + year + "_L.root";

  // Open files
  TFile* f_eff    = new TFile(nf_eff.c_str(),"READ");
  TFile* f_eff_sf = new TFile(nf_eff_sf.c_str(),"READ");
  TFile* f_mistag    = new TFile(nf_mistag.c_str(),"READ");
  TFile* f_mistag_sf = new TFile(nf_mistag_sf.c_str(),"READ");

  // Build TH2F names
  std::string nh_eff    = "h2_eff_mc"+year+"_L";
  std::string nh_eff_sf = "h2_eff_sf"+year+"_L";
  std::string nh_mistag    = "h2_mistag_mc"+year+"_L";
  std::string nh_mistag_sf = "h2_mistag_sf"+year+"_L";

  // Get the TH2F histograms
  h_eff_    = (TH2F*) f_eff->Get(nh_eff.c_str());
  h_eff_SF_ = (TH2F*) f_eff_sf->Get(nh_eff_sf.c_str());
  h_mistag_    = (TH2F*) f_mistag->Get(nh_mistag.c_str());
  h_mistag_SF_ = (TH2F*) f_mistag_sf->Get(nh_mistag_sf.c_str());
}


// Destructor
PuJetIdSF::~PuJetIdSF() 
{
  if (h_eff_) delete h_eff_;
  if (h_eff_SF_) delete h_eff_SF_;
  if (h_mistag_) delete h_mistag_;
  if (h_mistag_SF_) delete h_mistag_SF_;
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


//getEvtWeight
float PuJetIdSF::getEvtWeight(bigTree &theBigTree, TLorentzVector tau1, TLorentzVector tau2)
{
  // Weight for each event
  float eventWeight;
  float P_MC   = 1.;
  float P_DATA = 1.;

  //std::cout << "----- PuJetIdSF -----" << std::endl;

  // Loop on jets
  for (unsigned int iJet = 0; iJet < theBigTree.jets_px->size(); ++iJet)
  {
    // JET PF ID cut
    if (theBigTree.PFjetID->at (iJet) < 1) continue; // 0 ; don't pass PF Jet ID; 1: tight, 2: tightLepVeto

    // Build jet
    TLorentzVector tlv_jet(theBigTree.jets_px->at (iJet), theBigTree.jets_py->at (iJet), theBigTree.jets_pz->at (iJet), theBigTree.jets_e->at (iJet));

    //std::cout << " iJet: " << iJet << "  pt: " << tlv_jet.Pt() << " eta: " << tlv_jet.Eta() << " phi: " << tlv_jet.Phi() << std::endl;

    // Kinematic selections
    if (tlv_jet.Pt() < 20 || tlv_jet.Pt() > 50) continue;
    if (TMath::Abs(tlv_jet.Eta()) > 4.7) continue;
    if (tlv_jet.DeltaR(tau1) < 0.5) continue;
    if (tlv_jet.DeltaR(tau2) < 0.5) continue;

    // Build genJet and check if it is matched
    bool isRealJet = false;
    for (unsigned int igj = 0; igj < theBigTree.genjet_px->size(); igj++)
    {
      TLorentzVector tlv_genJet(theBigTree.genjet_px->at(igj),theBigTree.genjet_py->at(igj),theBigTree.genjet_pz->at(igj),theBigTree.genjet_e->at(igj));
      if (tlv_jet.DeltaR(tlv_genJet)<0.4)
      {
        isRealJet = true;
        //std::cout << "  matched genJet pt: " << tlv_genJet.Pt() << " eta: " << tlv_genJet.Eta() << " phi: " << tlv_genJet.Phi() << std::endl;
        break;
      }
    }

    // Pass PU jet ID Loose WP
    bool passPUjetIDLoose = CheckBit(theBigTree.jets_PUJetIDupdated_WP->at(iJet), 2);

    //std::cout << "   isRealJet: " << isRealJet << " passPUjetIDLoose: " << passPUjetIDLoose << std::endl;

    // Get Efficiency and SF
    float eff = getEff(isRealJet, tlv_jet.Pt(), tlv_jet.Eta());
    float sf  = getSF (isRealJet, tlv_jet.Pt(), tlv_jet.Eta());

    //std::cout << "   eff: " << eff << " sf: " << sf << std::endl;

    // compute eventWeight
    if (passPUjetIDLoose)
    {
      P_MC *= eff;
      P_DATA *= sf*eff;
    }
    else
    {
      P_MC *= (1. - eff);
      P_DATA *= (1. - sf*eff);
    }
    //std::cout << "   P_MC: " << P_MC << " P_DATA: " << P_DATA << " weight: " << (P_DATA/P_MC) << std::endl;

  } // end loop on jets

  eventWeight = P_DATA/P_MC;
  return eventWeight;
}
