#include "HHbtagKLUBinterface.h"

// Constructor
HHbtagKLUBinterface::HHbtagKLUBinterface (std::array<std::string, 2> models, int year)
 : HHbtagger_(models)
{
  // Set the year just once
  HHbtag_year_ = year;
}


// Destructor
HHbtagKLUBinterface::~HHbtagKLUBinterface() {}


// SetEventInputs: set inputs that change for every event
void HHbtagKLUBinterface::SetInputValues(bigTree &theBigTree, std::vector<std::pair<float,int>> jets_and_sortPar, int channel,
  TLorentzVector tlv_tau1, TLorentzVector tlv_tau2, TLorentzVector tlv_tauH, TLorentzVector tlv_MET, unsigned long long event)
{
  // Clear the vectors
  HHbtag_jet_pt_.clear();
  HHbtag_jet_eta_.clear();
  HHbtag_rel_jet_M_pt_.clear();
  HHbtag_rel_jet_E_pt_.clear();
  HHbtag_jet_htt_deta_.clear();
  HHbtag_jet_deepFlavour_.clear();
  HHbtag_jet_htt_dphi_.clear();

  // Set generic variables
  HHbtag_channel_  = channel;
  HHbtag_evt_      = event;

  // Revers jets_and_sortPar: first highest deepFlav score, last lowest deepFlav score
  std::reverse(jets_and_sortPar.begin(), jets_and_sortPar.end());

  // Loop on jets_and_sortPar to save quantities needed for HHbtag
  for (unsigned int i = 0; i < jets_and_sortPar.size(); i++)
  {
    unsigned int idx = jets_and_sortPar.at(i).second;
    TLorentzVector tlv_jet;
    tlv_jet.SetPxPyPzE(theBigTree.jets_px->at(idx),theBigTree.jets_py->at(idx),theBigTree.jets_pz->at(idx),theBigTree.jets_e->at(idx));

    HHbtag_jet_pt_ .push_back(tlv_jet.Pt());
    HHbtag_jet_eta_.push_back(tlv_jet.Eta());

    HHbtag_rel_jet_M_pt_.push_back( tlv_jet.M() / tlv_jet.Pt() );
    HHbtag_rel_jet_E_pt_.push_back( tlv_jet.E() / tlv_jet.Pt() );

    HHbtag_jet_htt_deta_.push_back( tlv_tauH.Eta() - tlv_jet.Eta() );
    HHbtag_jet_htt_dphi_.push_back( ROOT::Math::VectorUtil::DeltaPhi(tlv_tauH, tlv_jet) );

    HHbtag_jet_deepFlavour_.push_back( jets_and_sortPar.at(i).first );
  }

  // Set other input variables
  HHbtag_htt_met_dphi_      = ROOT::Math::VectorUtil::DeltaPhi(tlv_tauH, tlv_MET);
  HHbtag_htt_scalar_pt_     = tlv_tau1.Pt() + tlv_tau2.Pt();
  HHbtag_rel_met_pt_htt_pt_ = tlv_MET.Pt() / HHbtag_htt_scalar_pt_;
  HHbtag_tauH_pt_           = tlv_tauH.Pt();
  HHbtag_tauH_eta_          = tlv_tauH.Eta();

  // Finally store the jet indexes, will be used in HHbtagKLUBinterface::GetScore()
  HHbtag_jets_and_sortPar_ = jets_and_sortPar;
}


// GetScore
std::map<int,float> HHbtagKLUBinterface::GetScore()
{
  // Get HHbtag score
  auto HHbtag_scores = HHbtagger_.HH_BTag::GetScore(HHbtag_jet_pt_, HHbtag_jet_eta_, HHbtag_rel_jet_M_pt_,
      HHbtag_rel_jet_E_pt_, HHbtag_jet_htt_deta_, HHbtag_jet_deepFlavour_, HHbtag_jet_htt_dphi_,
      HHbtag_year_, HHbtag_channel_, HHbtag_tauH_pt_, HHbtag_tauH_eta_, HHbtag_htt_met_dphi_,
      HHbtag_rel_met_pt_htt_pt_, HHbtag_htt_scalar_pt_, HHbtag_evt_);

  // Store HHbtag scores in a map<jet_idx,HHbtag_score>
  std::map<int,float> jets_and_HHbtag;
  for (unsigned int i=0; i<HHbtag_jets_and_sortPar_.size(); i++)
  {
    jets_and_HHbtag[HHbtag_jets_and_sortPar_.at(i).second] = HHbtag_scores.at(i);
  }

  return jets_and_HHbtag;
}
