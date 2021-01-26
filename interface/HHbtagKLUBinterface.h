#ifndef HHbtagKLUBinterface_h
#define HHbtagKLUBinterface_h

// -------------------------------------------------------------------------------------------------------------- //
//                                                                                                                //
//   class HHbtagKLUBinterface                                                                                    //
//                                                                                                                //
//   Class to compute HHbtag output.                                                                              //
//                                                                                                                //
//   Author: Francesco Brivio (Milano-Bicocca)                                                                    //
//   Date  : May 2020                                                                                             //
//                                                                                                                //
// -------------------------------------------------------------------------------------------------------------- //

// Standard libraries
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>

// KLUB libraries
#include "bigTree.h"
#include "SmearedJetProducer.h"

// ROOT libraries
#include "TLorentzVector.h"
#include <Math/VectorUtil.h>

// HHbtag libraries
#include "../../HHTools/HHbtag/interface/HH_BTag.h"


// HHbtagKLUBinterface class
class HHbtagKLUBinterface {

  public:
    HHbtagKLUBinterface (std::array<std::string, 2> models, int year);
    ~HHbtagKLUBinterface ();

    void SetInputValues(bigTree &theBigTree, std::vector<std::pair<float,int>> jets_and_sortPar, int channel,
          TLorentzVector tlv_tau1, TLorentzVector tlv_tau2, TLorentzVector tlv_tauH, TLorentzVector tlv_MET,
          unsigned long long event, SmearedJetProducer &smearer);

    std::map<int,float> GetScore();


  private:
    hh_btag::HH_BTag HHbtagger_;
    std::array<std::string, 2> models_;

    // Util variables
    std::vector<std::pair<float,int>> HHbtag_jets_and_sortPar_;

    // Declare input variables to BDT
    int HHbtag_year_, HHbtag_channel_;
    unsigned long long int HHbtag_evt_;
    std::vector<float> HHbtag_jet_pt_, HHbtag_jet_eta_, HHbtag_rel_jet_M_pt_, HHbtag_rel_jet_E_pt_;
    std::vector<float> HHbtag_jet_htt_deta_, HHbtag_jet_deepFlavour_, HHbtag_jet_htt_dphi_;
    float HHbtag_htt_met_dphi_, HHbtag_rel_met_pt_htt_pt_, HHbtag_htt_scalar_pt_;
    float HHbtag_tauH_pt_, HHbtag_tauH_eta_;
};

#endif // HHbtagKLUBinterface