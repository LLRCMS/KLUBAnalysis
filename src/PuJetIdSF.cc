#include "PuJetIdSF.h"

#define DEBUG false

// Constructor
PuJetIdSF::PuJetIdSF (std::string directory, std::string year)
{

  // UL: single file for all periods, and for efficiencies, SFs, and unc. 
  std::string nf = directory + "/PUID_106XTraining_ULRun2_EffSFandUncties_v1.root";
  TFile *f = new TFile(nf.c_str(),"READ");


  // UL: no mistag eff/sf
  std::string nh_eff_sf        = "h2_eff_sfUL"+year+"_L";
  std::string nh_eff_sf_err    = "h2_eff_sfUL"+year+"_L_Systuncty";

  // Get the TH2F histograms
  h_eff_SF_        = (TH2F*) f->Get(nh_eff_sf.c_str());
  h_eff_SF_err_    = (TH2F*) f->Get(nh_eff_sf_err.c_str());
  
  year_ = year;
}


// Destructor
PuJetIdSF::~PuJetIdSF()
{
  if (h_eff_SF_) delete h_eff_SF_;
  if (h_eff_SF_err_) delete h_eff_SF_err_;
}

// check bit
bool PuJetIdSF::CheckBit (int number, int bitpos)
{
  bool res = number & (1 << bitpos);
  return res;
}

// getSF
float PuJetIdSF::getSF(float pt, float eta)
{
  float sf = 1.;

  if (pt<20.) pt=20.;
  if (pt>50.) pt=50.;

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
  

  return sf;
}

// getSFError
float PuJetIdSF::getSFError(float pt, float eta)
{
  float sferr = 0.;

  if (pt<20.) pt=20.;
  if (pt>50.) pt=50.;

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

  return sferr;
}

//getEvtWeight - Returns a vector with {central SF, SF_up, SF_down}
std::vector<float> PuJetIdSF::getEvtWeight(bigTree &theBigTree, TLorentzVector tau1, TLorentzVector tau2, std::map<int,double> jets_and_smearFactor)
{
  // Weight for each event
  std::vector<float> eventWeight(3); // keeping 3 values so that not to change too much skimmers; can be updated
  float SF = 1.;
  float SF_up   = 1.;
  float SF_down = 1.;

  if (DEBUG) std::cout << "----- PuJetIdSF -----" << std::endl;

  if (theBigTree.jets_px->size() != jets_and_smearFactor.size()) {
	std::cout << "[DEBUG] " << theBigTree.jets_px->size() << " " << jets_and_smearFactor.size() << std::endl;
	for (unsigned i=0; i<theBigTree.jets_px->size(); ++i) {
	  std::cout << i << " val: " << theBigTree.jets_px->at(i) << " " << jets_and_smearFactor.at(i) << std::endl;
	}
	throw std::runtime_error("Different length jet quantities!");
  }
	
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
    bool passPUjetIDLoose;
    if (year_ == "2017") passPUjetIDLoose = CheckBit(theBigTree.jets_PUJetID_WP->at(iJet), 2);
    else passPUjetIDLoose = CheckBit(theBigTree.jets_PUJetIDupdated_WP->at(iJet), 2);

    if (DEBUG) std::cout << "   isRealJet: " << isRealJet << " passPUjetIDLoose: " << passPUjetIDLoose << std::endl;

    if (passPUjetIDLoose && isRealJet)
    {
      // Get SF
      float sf  = getSF(tlv_jet.Pt(), tlv_jet.Eta());
      float sfErr   = getSFError(tlv_jet.Pt(), tlv_jet.Eta());
      float sf_up   = std::clamp(sf + sfErr, (float)0, (float)5);
      float sf_down = std::clamp(sf - sfErr, (float)0, (float)5);

      if (DEBUG) std::cout << " sf: " << sf << " sfErr: " << sfErr << " sf_up: " << sf_up << " sf_down: " <<sf_down << std::endl;

      // compute eventWeight
      SF      *= sf;
      SF_up   *= sf_up;
      SF_down *= sf_down;
    }

  } // end loop on jets

  eventWeight.at(0)  = SF;
  eventWeight.at(1)  = SF_up;
  eventWeight.at(2)  = SF_down;

  return eventWeight;
}
