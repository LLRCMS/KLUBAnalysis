#ifndef DNNKLUBinterface_h
#define DNNKLUBinterface_h

// -------------------------------------------------------------------------------------------------------------- //
//                                                                                                                //
//   class DNNKLUBinterface                                                                                       //
//                                                                                                                //
//   Class to compute DNN output.                                                                                 //
//   Original DNN packages from:                                                                                  //
//    - https://github.com/GilesStrong/cms_hh_proc_interface                                                      //
//    - https://github.com/GilesStrong/cms_hh_tf_inference                                                        //
//    - https://github.com/GilesStrong/cms_runII_dnn_models                                                       //
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

// Math libraries
#include <Math/VectorUtil.h>
#include <Math/LorentzVector.h>
#include <Math/PxPyPzM4D.h>

// ROOT libraries
#include "TLorentzVector.h"

// DNN
#include "../../cms_hh_tf_inference/inference/interface/inf_wrapper.hh"
#include "../../cms_hh_proc_interface/processing/interface/feat_comp.hh"
#include "../../cms_hh_proc_interface/processing/interface/evt_proc.hh"

// Using names
using DNNVector = ROOT::Math::LorentzVector<ROOT::Math::PxPyPzM4D<float>>;

// DNN Constants
const double E_MASS  = 0.0005109989; //GeV
const double MU_MASS = 0.1056583715; //GeV

// DNNKLUBinterface class
class DNNKLUBinterface {

  public:
    DNNKLUBinterface (std::string model_dir, std::vector<std::string> requested, std::vector<float> target_kls);
    ~DNNKLUBinterface ();
    
    void SetGlobalInputs(Year year, Spin spin);

    void SetEventInputs(Channel channel, int is_boosted, float deepFlav1, float deepFlav2, 
          float deepCSV1, float deepCSV2, int nvbf, unsigned long long int eventn);

    void SetShiftedInputs(TLorentzVector b1, TLorentzVector b2, TLorentzVector l1, TLorentzVector l2, 
          TLorentzVector vbf1, TLorentzVector vbf2, TLorentzVector met, TLorentzVector svfit, 
          float KinFitMass, float KinFitChi2, bool KinFitConv, bool SVfitConv, float MT2, 
          float mTtot, float pzeta_vis, float pzeta, float top1m, float top2m, float mt1, float mt2);

    std::vector<float> GetPredictions();


  private:
    InfWrapper wrapper_;
    EvtProc    evt_proc_;
    std::vector<float> target_kls_;

    // Declare input variables to DNN
    DNNVector DNN_b_1_, DNN_b_2_, DNN_l_1_, DNN_l_2_, DNN_met_, DNN_svfit_, DNN_vbf_1_, DNN_vbf_2_;
    float DNN_kinfit_mass_, DNN_kinfit_chi2_, DNN_mt2_, DNN_mt_tot_, DNN_top_1_mass_, DNN_top_2_mass_, DNN_p_zetavisible_, DNN_p_zeta_;
    float DNN_b_1_deepflav_, DNN_b_2_deepflav_, DNN_b_1_deepcsv_, DNN_b_2_deepcsv_;
    int DNN_is_boosted_, DNN_n_vbf_;
    float DNN_l_1_mt_, DNN_l_2_mt_;
    unsigned long long int DNN_evt_;
    bool DNN_svfit_conv_, DNN_hh_kinfit_conv_;
    Channel DNN_e_channel_;
    Year DNN_e_year_;
    Spin DNN_spin_;
    float DNN_klambda_;
    float DNN_res_mass_;
};

#endif // DNNKLUBinterface_h