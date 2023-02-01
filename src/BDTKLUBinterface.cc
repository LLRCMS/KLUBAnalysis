#include "BDTKLUBinterface.h"

// bjet2_pt:pt_b2, bH_pt:pt_hbb, dau1_pt:pt_l1, dau2_pt:pt_l2, tauH_SVFIT_pt:pt_htautau_sv,
// BDT_HT20:HT_otherjets, p_zeta:p_zeta, p_zeta_visible:p_zetavisible, BDT_ditau_deltaPhi:dphi_l1l2,
// BDT_tauHsvfitMet_deltaPhi:dphi_METhtautau_sv, mT_tauH_MET:MT_htautau, mT_total:MT_tot, MT2:MT2,
// BDT_MX:mass_X, BDT_bH_tauH_MET_InvMass:mass_H, BDT_bH_tauH_SVFIT_InvMass:mass_H_sv, BDT_bH_tauH_InvMass:mass_H_vis,
// HHKin_mass_raw:mass_H_kinfit, HHKin_mass_raw_chi2:mass_H_kinfit_chi2, BDT_MET_bH_cosTheta:costheta_METhbb
// channel, kl

// Constructor
BDTKLUBinterface::BDTKLUBinterface (std::string weights, std::vector<float> target_kls)
{
  // Store target lambdas
  target_kls_ = target_kls;

  // Store weights
  weights_ = weights;

  // Assign variables to reader
  readerSM_.AddVariable("pt_b2"        , &bjet2_pt_);
  readerSM_.AddVariable("pt_hbb"       , &bH_pt_);
  readerSM_.AddVariable("pt_l1"        , &dau1_pt_);
  readerSM_.AddVariable("pt_l2"        , &dau2_pt_);
  readerSM_.AddVariable("pt_htautau_sv", &tauH_SVFIT_pt_);

  readerSM_.AddVariable("HT_otherjets" , &BDT_HT20_);
  readerSM_.AddVariable("p_zeta"       , &p_zeta_);
  readerSM_.AddVariable("p_zetavisible", &p_zeta_visible_);
  readerSM_.AddVariable("dphi_l1l2"    , &BDT_ditau_deltaPhi_);

  readerSM_.AddVariable("dphi_METhtautau_sv", &BDT_tauHsvfitMet_deltaPhi_);
  readerSM_.AddVariable("MT_htautau"        , &mT_tauH_MET_);
  readerSM_.AddVariable("MT_tot"            , &mT_total_);
  readerSM_.AddVariable("MT2"               , &MT2_);

  readerSM_.AddVariable("mass_X"    , &BDT_MX_);
  readerSM_.AddVariable("mass_H"    , &BDT_bH_tauH_MET_InvMass_);
  readerSM_.AddVariable("mass_H_sv" , &BDT_bH_tauH_SVFIT_InvMass_);
  readerSM_.AddVariable("mass_H_vis", &BDT_bH_tauH_InvMass_);

  readerSM_.AddVariable("mass_H_kinfit"     , &HHKin_mass_raw_);
  readerSM_.AddVariable("mass_H_kinfit_chi2", &HHKin_mass_raw_chi2_);
  readerSM_.AddVariable("costheta_METhbb"   , &BDT_MET_bH_cosTheta_);

  readerSM_.AddVariable("channel", &BDT_channel_);
  readerSM_.AddVariable("kl"     , &BDT_kl_);

  // Book reader method
  readerSM_.BookMVA("Grad_1", weights_.c_str() );

}


// Destructor
BDTKLUBinterface::~BDTKLUBinterface() {}


// SetInputValues: set all input values
void BDTKLUBinterface::SetInputValues(float bjet2_pt, float bH_pt, float dau1_pt, float dau2_pt, float tauH_SVFIT_pt, float BDT_channel,
				      float BDT_HT20, float p_zeta, float p_zeta_visible, float BDT_ditau_deltaPhi,
				      float BDT_tauHsvfitMet_deltaPhi, float mT_tauH_MET, float mT_total, float MT2,
				      float BDT_MX, float BDT_bH_tauH_MET_InvMass, float BDT_bH_tauH_SVFIT_InvMass,
				      float BDT_bH_tauH_InvMass, float HHKin_mass_raw, float HHKin_mass_raw_chi2, float BDT_MET_bH_cosTheta)
{
  bjet2_pt_ = bjet2_pt;
  bH_pt_ = bH_pt;
  dau1_pt_ = dau1_pt;
  dau2_pt_ = dau2_pt;
  tauH_SVFIT_pt_ = tauH_SVFIT_pt;
  BDT_channel_ = BDT_channel;
  BDT_HT20_ = BDT_HT20;
  p_zeta_ = p_zeta;
  p_zeta_visible_ = p_zeta_visible;
  BDT_ditau_deltaPhi_ = BDT_ditau_deltaPhi;
  BDT_tauHsvfitMet_deltaPhi_ = BDT_tauHsvfitMet_deltaPhi;
  mT_tauH_MET_ = mT_tauH_MET;
  mT_total_ = mT_total;
  MT2_ = MT2;
  BDT_MX_ = BDT_MX;
  BDT_bH_tauH_MET_InvMass_ = BDT_bH_tauH_MET_InvMass;
  BDT_bH_tauH_SVFIT_InvMass_ = BDT_bH_tauH_SVFIT_InvMass;
  BDT_bH_tauH_InvMass_ = BDT_bH_tauH_InvMass;
  HHKin_mass_raw_ = HHKin_mass_raw;
  HHKin_mass_raw_chi2_ = HHKin_mass_raw_chi2;
  BDT_MET_bH_cosTheta_ = BDT_MET_bH_cosTheta;
}


// GetPredictions
std::vector<float> BDTKLUBinterface::GetPredictions()
{
  // Store results with predictions for each kl value
  std::vector<float> outBDT;

  for (unsigned int ikl = 0; ikl < target_kls_.size(); ikl++)
  {
    // Assign external values
    BDT_kl_ = target_kls_.at(ikl);

    //std::cout << "------------ BDT inputs ------------" << std::endl;
    //std::cout << " - BDT_HT20_                  : " << BDT_HT20_ << std::endl;
    //std::cout << " - BDT_MET_bH_cosTheta_       : " << BDT_MET_bH_cosTheta_ << std::endl;
    //std::cout << " - BDT_MX_                    : " << BDT_MX_ << std::endl;
    //std::cout << " - BDT_bH_tauH_InvMass_       : " << BDT_bH_tauH_InvMass_ << std::endl;
    //std::cout << " - BDT_bH_tauH_MET_InvMass_   : " << BDT_bH_tauH_MET_InvMass_ << std::endl;
    //std::cout << " - BDT_bH_tauH_SVFIT_InvMass_ : " << BDT_bH_tauH_SVFIT_InvMass_ << std::endl;
    //std::cout << " - BDT_ditau_deltaPhi_        : " << BDT_ditau_deltaPhi_ << std::endl;
    //std::cout << " - BDT_tauHsvfitMet_deltaPhi_ : " << BDT_tauHsvfitMet_deltaPhi_ << std::endl;
    //std::cout << " - HHKin_mass_raw_            : " << HHKin_mass_raw_ << std::endl;
    //std::cout << " - HHKin_mass_raw_chi2_       : " << HHKin_mass_raw_chi2_ << std::endl;
    //std::cout << " - MT2_                       : " << MT2_ << std::endl;
    //std::cout << " - bH_pt_                     : " << bH_pt_ << std::endl;
    //std::cout << " - bjet2_pt_                  : " << bjet2_pt_ << std::endl;
    //std::cout << " - dau1_pt_                   : " << dau1_pt_ << std::endl;
    //std::cout << " - dau2_pt_                   : " << dau2_pt_ << std::endl;
    //std::cout << " - mT_tauH_MET_               : " << mT_tauH_MET_ << std::endl;
    //std::cout << " - mT_total_                  : " << mT_total_ << std::endl;
    //std::cout << " - p_zeta_                    : " << p_zeta_ << std::endl;
    //std::cout << " - p_zeta_visible_            : " << p_zeta_visible_ << std::endl;
    //std::cout << " - tauH_SVFIT_pt_             : " << tauH_SVFIT_pt_ << std::endl;
    //std::cout << " - BDT_channel_               : " << BDT_channel_ << std::endl;
    //std::cout << " - BDT_kl_                    : " << BDT_kl_ << std::endl;

    // Get model prediction
    float BDT_pred = readerSM_.EvaluateMVA("Grad_1");

    // Store prediction
    outBDT.push_back(BDT_pred);
  }

  return outBDT;
}
