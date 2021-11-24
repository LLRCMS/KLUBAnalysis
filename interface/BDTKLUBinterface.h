#ifndef BDTKLUBinterface_h
#define BDTKLUBinterface_h

// -------------------------------------------------------------------------------------------------------------- //
//                                                                                                                //
//   class BDTKLUBinterface                                                                                       //
//                                                                                                                //
//   Class to compute BDT output.                                                                                 //
//                                                                                                                //
//   Author: Francesco Brivio (Milano-Bicocca)                                                                    //
//   Date  : April 2020                                                                                           //
//                                                                                                                //
// -------------------------------------------------------------------------------------------------------------- //

// Standard libraries
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>

// ROOT libraries
#include "TLorentzVector.h"

// TMVA
#include "TMVA/MsgLogger.h"
#include "TMVA/Config.h"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"

// BDTKLUBinterface class
class BDTKLUBinterface {

public:
  BDTKLUBinterface (std::string weights, std::vector<float> target_kls);
  ~BDTKLUBinterface ();

  void SetInputValues(float bjet2_pt, float bH_pt, float dau1_pt, float dau2_pt, float tauH_SVFIT_pt, float BDT_channel,
		      float BDT_HT20, float p_zeta, float p_zeta_visible, float BDT_ditau_deltaPhi,
		      float BDT_tauHsvfitMet_deltaPhi, float mT_tauH_MET, float mT_total, float MT2,
		      float BDT_MX, float BDT_bH_tauH_MET_InvMass, float BDT_bH_tauH_SVFIT_InvMass,
		      float BDT_bH_tauH_InvMass, float HHKin_mass_raw, float HHKin_mass_raw_chi2, float BDT_MET_bH_cosTheta);

  std::vector<float> GetPredictions();


private:
  TMVA::Reader readerSM_;
  std::string weights_;
  std::vector<float> target_kls_;

  // Declare input variables to BDT
  float bjet2_pt_, bH_pt_, dau1_pt_, dau2_pt_, tauH_SVFIT_pt_;                             // 5
  float BDT_HT20_, p_zeta_, p_zeta_visible_, BDT_ditau_deltaPhi_;                          // 4
  float BDT_tauHsvfitMet_deltaPhi_, mT_tauH_MET_, mT_total_, MT2_;                         // 4
  float BDT_MX_, BDT_bH_tauH_MET_InvMass_, BDT_bH_tauH_SVFIT_InvMass_;                     // 3
  float BDT_bH_tauH_InvMass_, HHKin_mass_raw_, HHKin_mass_raw_chi2_, BDT_MET_bH_cosTheta_; // 4
  float BDT_channel_, BDT_kl_;                                                             // 2

};

#endif // BDTKLUBinterface_h
