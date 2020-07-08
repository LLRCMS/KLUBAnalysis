#include "DNNKLUBinterface.h"

// Constructor
DNNKLUBinterface::DNNKLUBinterface (std::string model_dir, std::vector<std::string> requested, std::vector<float> target_kls)
 : wrapper_(model_dir, 1, false),
   evt_proc_(false, requested, true)
{
  // Store target lambdas
  target_kls_ = target_kls;
}


// Destructor
DNNKLUBinterface::~DNNKLUBinterface() {}


// SetGlobalInputs: set inputs that never change with the events
void DNNKLUBinterface::SetGlobalInputs(Year year, Spin spin)
{
  DNN_e_year_   = year;
  DNN_spin_     = spin;
  DNN_res_mass_ = 125.; // FIXME later
}


// SetEventInputs: set inputs that change every event
void DNNKLUBinterface::SetEventInputs(Channel channel, int is_boosted, int nvbf, unsigned long long int eventn,
  float deepFlav1, float deepFlav2, float CvsL_b1, float CvsL_b2, float CvsL_vbf1, float CvsL_vbf2,
  float CvsB_b1, float CvsB_b2, float CvsB_vbf1, float CvsB_vbf2,
  float HHbtag_b1, float HHbtag_b2, float HHbtag_vbf1, float HHbtag_vbf2)
{
  DNN_e_channel_    = channel;
  DNN_is_boosted_   = is_boosted;
  DNN_n_vbf_        = nvbf;
  DNN_evt_          = eventn;

  DNN_b_1_deepflav_ = deepFlav1;
  DNN_b_2_deepflav_ = deepFlav2;

  DNN_b_1_cvsl_     = CvsL_b1;
  DNN_b_2_cvsl_     = CvsL_b2;
  DNN_vbf_1_cvsl_   = CvsL_vbf1;
  DNN_vbf_2_cvsl_   = CvsL_vbf2;

  DNN_b_1_cvsb_     = CvsB_b1;
  DNN_b_2_cvsb_     = CvsB_b2;
  DNN_vbf_1_cvsb_   = CvsB_vbf1;
  DNN_vbf_2_cvsb_   = CvsB_vbf2;

  DNN_b_1_hhbtag_   = HHbtag_b1;
  DNN_b_2_hhbtag_   = HHbtag_b2;
  DNN_vbf_1_hhbtag_ = HHbtag_vbf1;
  DNN_vbf_2_hhbtag_ = HHbtag_vbf2;
}


// SetShiftedInputs: set inputs that change for the different shifts
// due to TES, EES, MES, JES
void DNNKLUBinterface::SetShiftedInputs(TLorentzVector b1, TLorentzVector b2, TLorentzVector l1, TLorentzVector l2, 
    TLorentzVector vbf1, TLorentzVector vbf2, TLorentzVector met, TLorentzVector svfit, 
    float KinFitMass, float KinFitChi2, bool KinFitConv, bool SVfitConv, float MT2)
{
  // Taus
  DNN_l_1_.SetCoordinates(l1.Px(), l1.Py(), l1.Pz(), l1.M());
  if      (DNN_e_channel_ == muTau)
  { 
    DNN_l_1_.SetM(MU_MASS);
  }
  else if (DNN_e_channel_ == eTau)
  {
    DNN_l_1_.SetM(E_MASS);
  }
  DNN_l_2_.SetCoordinates(l2.Px(), l2.Py(), l2.Pz(), l2.M());

  // b-jets
  DNN_b_1_.SetCoordinates(b1.Px(), b1.Py(), b1.Pz(), b1.M());
  DNN_b_2_.SetCoordinates(b2.Px(), b2.Py(), b2.Pz(), b2.M());

  // VBF jets
  DNN_vbf_1_.SetCoordinates(vbf1.Px(), vbf1.Py(), vbf1.Pz(), vbf1.M());
  DNN_vbf_2_.SetCoordinates(vbf2.Px(), vbf2.Py(), vbf2.Pz(), vbf2.M());

  // MET and SVfit
  DNN_met_  .SetCoordinates(met.Px(), met.Py(), 0, 0);
  DNN_svfit_.SetCoordinates(svfit.Px(), svfit.Py(), svfit.Pz(), svfit.M());

  // KinFit, MT2 and SVfit variables
  DNN_kinfit_mass_    = KinFitMass;
  DNN_kinfit_chi2_    = KinFitChi2;
  DNN_hh_kinfit_conv_ = KinFitConv;
  DNN_svfit_conv_     = SVfitConv;
  DNN_mt2_            = MT2;

  // Pass ellyptic mass cut
  // ((tauH_SVFIT_mass-116.)*(tauH_SVFIT_mass-116.))/(35.*35.) + ((bH_mass_raw-111.)*(bH_mass_raw-111.))/(45.*45.) <  1.0
  DNN_pass_massCut_ = ( ((svfit.M()-116.)*(svfit.M()-116.))/(35.*35.) + (((b1+b2).M()-111.)*((b1+b2).M()-111.))/(45.*45.) <  1.0 );

}


// GetPredictions
std::vector<float> DNNKLUBinterface::GetPredictions()
{
  // Store results with predictions for each kl value
  std::vector<float> outDNN;

  for (unsigned int ikl = 0; ikl < target_kls_.size(); ikl++)
  {
    // Assign external values
    DNN_klambda_ = target_kls_.at(ikl);

    // Compute fatures
    std::vector<float> feat_vals = evt_proc_.process_as_vec(
        DNN_b_1_, DNN_b_2_, DNN_l_1_, DNN_l_2_, DNN_met_, DNN_svfit_, DNN_vbf_1_, DNN_vbf_2_,
        DNN_kinfit_mass_, DNN_kinfit_chi2_, DNN_mt2_, DNN_is_boosted_, DNN_b_1_deepflav_, DNN_b_2_deepflav_,
        DNN_e_channel_, DNN_e_year_, DNN_res_mass_, DNN_spin_, DNN_klambda_,
        DNN_n_vbf_, DNN_svfit_conv_, DNN_hh_kinfit_conv_,
        DNN_b_1_hhbtag_, DNN_b_2_hhbtag_, DNN_vbf_1_hhbtag_, DNN_vbf_2_hhbtag_,
        DNN_b_1_cvsl_, DNN_b_2_cvsl_, DNN_vbf_1_cvsl_, DNN_vbf_2_cvsl_,
        DNN_b_1_cvsb_, DNN_b_2_cvsb_, DNN_vbf_1_cvsb_, DNN_vbf_2_cvsb_,
        0, 0, 0, // cv, c2v, c3
        DNN_pass_massCut_);

    std::vector<std::string> feats_names = evt_proc_.get_feats();

    //std::cout << " Feats size : " << feat_vals.size() << std::endl;
    //for (uint j=0; j<feat_vals.size(); j++)
    //{
    //  std::cout << " - " << feats_names.at(j) << " : " << feat_vals.at(j) << std::endl;
    //}

    // Get model prediction
    float DNN_pred = wrapper_.predict(feat_vals, DNN_evt_);

    // Store prediction
    outDNN.push_back(DNN_pred);
  }

  return outDNN;
}
