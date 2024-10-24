#ifndef smallTree_h
#define smallTree_h

#include "TTree.h"
#include "TString.h"

#include <vector>

// create the small tree with the branches I am interested in
struct smallTree
{
  smallTree (TString treeName) :
  m_smallT (new TTree (treeName, "small tree for HH analysis"))
  {
    init () ;
  }

  int Fill ()
  {
    return m_smallT->Fill () ;
  }

  int clearVars ()
    {
      m_MC_weight = -1. ;
	  m_MC_weight_fixed = -1. ;
	  m_aMCatNLOweight = -1. ;
	  m_MC_QCDscale0 = -1. ;
	  m_MC_QCDscale1 = -1. ;
	  m_MC_QCDscale2 = -1. ;
	  m_MC_QCDscale3 = -1. ;
	  m_MC_QCDscale4 = -1. ;
	  m_MC_QCDscale5 = -1. ;
	  m_MC_QCDscale6 = -1. ;
	  m_MC_pdf0 = -1. ;
	  m_MC_pdf1 = -1. ;
	  m_MC_pdf2 = -1. ;
	  m_MC_pdf3 = -1. ;
	  m_MC_pdf4 = -1. ;
	  m_MC_pdf5 = -1. ;
	  m_MC_pdf6 = -1. ;
	  m_MC_pdf7 = -1. ;
	  m_MC_pdf8 = -1. ;
	  m_MC_pdf9 = -1. ;
	  m_MC_pdf10 = -1. ;
	  m_MC_pdf11 = -1. ;
	  m_MC_pdf12 = -1. ;
	  m_MC_pdf13 = -1. ;
	  m_MC_pdf14 = -1. ;
	  m_MC_pdf15 = -1. ;
	  m_MC_pdf16 = -1. ;
	  m_MC_pdf17 = -1. ;
	  m_MC_pdf18 = -1. ;
	  m_MC_pdf19 = -1. ;
	  m_MC_pdf20 = -1. ;
	  m_MC_pdf21 = -1. ;
	  m_MC_pdf22 = -1. ;
	  m_MC_pdf23 = -1. ;
	  m_MC_pdf24 = -1. ;
	  m_MC_pdf25 = -1. ;
	  m_MC_pdf26 = -1. ;
	  m_MC_pdf27 = -1. ;
	  m_MC_pdf28 = -1. ;
	  m_MC_pdf29 = -1. ;
	  m_MC_pdf30 = -1. ;
	  m_MC_pdf31 = -1. ;
	  m_MC_pdf32 = -1. ;
	  m_MC_pdf33 = -1. ;
	  m_MC_pdf34 = -1. ;
	  m_MC_pdf35 = -1. ;
	  m_MC_pdf36 = -1. ;
	  m_MC_pdf37 = -1. ;
	  m_MC_pdf38 = -1. ;
	  m_MC_pdf39 = -1. ;
	  m_MC_pdf40 = -1. ;
	  m_MC_pdf41 = -1. ;
	  m_MC_pdf42 = -1. ;
	  m_MC_pdf43 = -1. ;
	  m_MC_pdf44 = -1. ;
	  m_MC_pdf45 = -1. ;
	  m_MC_pdf46 = -1. ;
	  m_MC_pdf47 = -1. ;
	  m_MC_pdf48 = -1. ;
	  m_MC_pdf49 = -1. ;
	  m_MC_pdf50 = -1. ;
	  m_MC_pdf51 = -1. ;
	  m_MC_pdf52 = -1. ;
	  m_MC_pdf53 = -1. ;
	  m_MC_pdf54 = -1. ;
	  m_MC_pdf55 = -1. ;
	  m_MC_pdf56 = -1. ;
	  m_MC_pdf57 = -1. ;
	  m_MC_pdf58 = -1. ;
	  m_MC_pdf59 = -1. ;
	  m_MC_pdf60 = -1. ;
	  m_MC_pdf61 = -1. ;
	  m_MC_pdf62 = -1. ;
	  m_MC_pdf63 = -1. ;
	  m_MC_pdf64 = -1. ;
	  m_MC_pdf65 = -1. ;
	  m_MC_pdf66 = -1. ;
	  m_MC_pdf67 = -1. ;
	  m_MC_pdf68 = -1. ;
	  m_MC_pdf69 = -1. ;
	  m_MC_pdf70 = -1. ;
	  m_MC_pdf71 = -1. ;
	  m_MC_pdf72 = -1. ;
	  m_MC_pdf73 = -1. ;
	  m_MC_pdf74 = -1. ;
	  m_MC_pdf75 = -1. ;
	  m_MC_pdf76 = -1. ;
	  m_MC_pdf77 = -1. ;
	  m_MC_pdf78 = -1. ;
	  m_MC_pdf79 = -1. ;
	  m_MC_pdf80 = -1. ;
	  m_MC_pdf81 = -1. ;
	  m_MC_pdf82 = -1. ;
	  m_MC_pdf83 = -1. ;
	  m_MC_pdf84 = -1. ;
	  m_MC_pdf85 = -1. ;
	  m_MC_pdf86 = -1. ;
	  m_MC_pdf87 = -1. ;
	  m_MC_pdf88 = -1. ;
	  m_MC_pdf89 = -1. ;
	  m_MC_pdf90 = -1. ;
	  m_MC_pdf91 = -1. ;
	  m_MC_pdf92 = -1. ;
	  m_MC_pdf93 = -1. ;
	  m_MC_pdf94 = -1. ;
	  m_MC_pdf95 = -1. ;
	  m_MC_pdf96 = -1. ;
	  m_MC_pdf97 = -1. ;
	  m_MC_pdf98 = -1. ;
	  m_MC_pdf99 = -1. ;
	  m_MC_pdf100 = -1. ;
	  m_MC_astrong0 = -1. ;
	  m_MC_astrong1 = -1. ;
      m_totalWeight = -1. ;
      m_prescaleWeight = 1. ;
      m_L1pref_weight = 1.;
	  m_L1pref_weight_up = 1.;
	  m_L1pref_weight_down = 1.;
      m_PUjetID_SF = 1.;
      m_PUjetID_SF_up = 1.;
      m_PUjetID_SF_down = 1.;
      m_PUReweight = -1. ;
      m_PUReweight_up = -1. ;
      m_PUReweight_down = -1. ;
      m_bTagweightL = 1. ;
      m_bTagweightL_up = 1. ;
      m_bTagweightL_down = 1. ;
      m_bTagweightM = 1. ;
      m_bTagweightM_up = 1. ;
      m_bTagweightM_down = 1. ;
      m_bTagweightT = 1. ;
      m_bTagweightT_up = 1. ;
      m_bTagweightT_down = 1. ;
      m_bTagweightReshape = 1.;
      m_bTagweightReshape_jes_up = 1.;
      m_bTagweightReshape_lf_up = 1.;
      m_bTagweightReshape_hf_up = 1.;
      m_bTagweightReshape_hfstats1_up = 1.;
      m_bTagweightReshape_hfstats2_up = 1.;
      m_bTagweightReshape_lfstats1_up = 1.;
      m_bTagweightReshape_lfstats2_up = 1.;
      m_bTagweightReshape_cferr1_up = 1.;
      m_bTagweightReshape_cferr2_up = 1.;
      m_bTagweightReshape_jes_down = 1.;
      m_bTagweightReshape_lf_down = 1.;
      m_bTagweightReshape_hf_down = 1.;
      m_bTagweightReshape_hfstats1_down = 1.;
      m_bTagweightReshape_hfstats2_down = 1.;
      m_bTagweightReshape_lfstats1_down = 1.;
      m_bTagweightReshape_lfstats2_down = 1.;
      m_bTagweightReshape_cferr1_down = 1.;
      m_bTagweightReshape_cferr2_down = 1.;
      m_bTagweightReshape_jetup1    = 1.;
      m_bTagweightReshape_jetup2    = 1.;
      m_bTagweightReshape_jetup3    = 1.;
      m_bTagweightReshape_jetup4    = 1.;
      m_bTagweightReshape_jetup5    = 1.;
      m_bTagweightReshape_jetup6    = 1.;
      m_bTagweightReshape_jetup7    = 1.;
      m_bTagweightReshape_jetup8    = 1.;
      m_bTagweightReshape_jetup9    = 1.;
      m_bTagweightReshape_jetup10   = 1.;
      m_bTagweightReshape_jetup11   = 1.;
      m_bTagweightReshape_jetdown1  = 1.;
      m_bTagweightReshape_jetdown2  = 1.;
      m_bTagweightReshape_jetdown3  = 1.;
      m_bTagweightReshape_jetdown4  = 1.;
      m_bTagweightReshape_jetdown5  = 1.;
      m_bTagweightReshape_jetdown6  = 1.;
      m_bTagweightReshape_jetdown7  = 1.;
      m_bTagweightReshape_jetdown8  = 1.;
      m_bTagweightReshape_jetdown9  = 1.;
      m_bTagweightReshape_jetdown10 = 1.;
      m_bTagweightReshape_jetdown11 = 1.;
      m_TTtopPtreweight = 1. ;
      m_TTtopPtreweight_up   = 1. ;
      m_TTtopPtreweight_down = 1. ;
      m_EventNumber = 0 ;
      m_trigSF = -1.;
      m_trigSFnoMET = -1.;
      m_trigSFnoTau = -1.;
      m_trigSF_ele_up = -1.;
      m_trigSF_SL_ele_up = -1.;
      m_trigSF_cross_ele_up = -1.;
      m_trigSF_mu_up = -1.;
      m_trigSF_SL_mu_up = -1.;
      m_trigSF_cross_mu_up = -1.;
      m_trigSF_tau_DM0_up = -1.;
      m_trigSF_tau_DM1_up = -1.;
      m_trigSF_tau_DM10_up = -1.;
      m_trigSF_tau_DM11_up = -1.;
      m_trigSF_ele_down = -1.;
      m_trigSF_SL_ele_down = -1.;
      m_trigSF_cross_ele_down = -1.;
      m_trigSF_mu_down = -1.;
      m_trigSF_SL_mu_down = -1.;
      m_trigSF_cross_mu_down = -1.;
      m_trigSF_tau_DM0_down = -1.;
      m_trigSF_tau_DM1_down = -1.;
      m_trigSF_tau_DM10_down = -1.;
      m_trigSF_tau_DM11_down = -1.;
      m_trigSF_met_up = -1.;
      m_trigSF_met_down = -1.;
      m_trigSF_stau_up = -1.;
      m_trigSF_stau_down = -1.;

      m_IdSF_deep_2d = -99.;
      m_IdSF_leg1_deep_vsJet_2d = -99.;
      m_IdSF_leg2_deep_vsJet_2d = -99.;
      m_dauSFs = -99.;
      m_FakeRateSF_deep = -99.;

      m_dauSFs_tauid_2d_stat0_DM0_up = -99.;
      m_dauSFs_tauid_2d_stat0_DM0_down = -99.;
      m_dauSFs_tauid_2d_stat1_DM0_up = -99.;
      m_dauSFs_tauid_2d_stat1_DM0_down = -99.;
      m_dauSFs_tauid_2d_systuncorrdmeras_DM0_up = -99.;
      m_dauSFs_tauid_2d_systuncorrdmeras_DM0_down = -99.;

      m_dauSFs_tauid_2d_stat0_DM1_up = -99.;
      m_dauSFs_tauid_2d_stat0_DM1_down = -99.;
      m_dauSFs_tauid_2d_stat1_DM1_up = -99.;
      m_dauSFs_tauid_2d_stat1_DM1_down = -99.;
      m_dauSFs_tauid_2d_systuncorrdmeras_DM1_up = -99.;
      m_dauSFs_tauid_2d_systuncorrdmeras_DM1_down = -99.;

      m_dauSFs_tauid_2d_stat0_DM10_up = -99.;
      m_dauSFs_tauid_2d_stat0_DM10_down = -99.;
      m_dauSFs_tauid_2d_stat1_DM10_up = -99.;
      m_dauSFs_tauid_2d_stat1_DM10_down = -99.;
      m_dauSFs_tauid_2d_systuncorrdmeras_DM10_up = -99.;
      m_dauSFs_tauid_2d_systuncorrdmeras_DM10_down = -99.;

      m_dauSFs_tauid_2d_stat0_DM11_up = -99.;
      m_dauSFs_tauid_2d_stat0_DM11_down = -99.;
      m_dauSFs_tauid_2d_stat1_DM11_up = -99.;
      m_dauSFs_tauid_2d_stat1_DM11_down = -99.;
      m_dauSFs_tauid_2d_systuncorrdmeras_DM11_up = -99.;
      m_dauSFs_tauid_2d_systuncorrdmeras_DM11_down = -99.;

      m_dauSFs_tauid_2d_systcorrdmeras_up = -99.;
      m_dauSFs_tauid_2d_systcorrdmeras_down = -99.;
      m_dauSFs_tauid_2d_systcorrdmuncorreras_up = -99.;
      m_dauSFs_tauid_2d_systcorrdmuncorreras_down = -99.;
      m_dauSFs_tauid_2d_systcorrerasgt140_up = -99.;
      m_dauSFs_tauid_2d_systcorrerasgt140_down = -99.;
      m_dauSFs_tauid_2d_stat0gt140_up = -99.;
      m_dauSFs_tauid_2d_stat0gt140_down = -99.;
      m_dauSFs_tauid_2d_stat1gt140_up = -99.;
      m_dauSFs_tauid_2d_stat1gt140_down = -99.;
      m_dauSFs_tauid_2d_extrapgt140_up = -99.;
      m_dauSFs_tauid_2d_extrapgt140_down = -99.;

      m_dauSFs_mutauFR_etaLt0p4_up = -99.;
      m_dauSFs_mutauFR_eta0p4to0p8_up = -99.;
      m_dauSFs_mutauFR_eta0p8to1p2_up = -99.;
      m_dauSFs_mutauFR_eta1p2to1p7_up = -99.;
      m_dauSFs_mutauFR_etaGt1p7_up = -99.;
      m_dauSFs_mutauFR_etaLt0p4_down = -99.;
      m_dauSFs_mutauFR_eta0p4to0p8_down = -99.;
      m_dauSFs_mutauFR_eta0p8to1p2_down = -99.;
      m_dauSFs_mutauFR_eta1p2to1p7_down = -99.;
      m_dauSFs_mutauFR_etaGt1p7_down = -99.;

      m_dauSFs_etauFR_barrel_up = -99.;
      m_dauSFs_etauFR_endcap_up = -99.;
      m_dauSFs_etauFR_barrel_down = -99.;
      m_dauSFs_etauFR_endcap_down = -99.;

      m_dauSFs_muID_up = -99.;
      m_dauSFs_muID_down = -99.;
      m_dauSFs_muIso_up = -99.;
      m_dauSFs_muIso_down = -99.;

      m_dauSFs_eleID_up = -99.;
      m_dauSFs_eleID_down = -99.;
      m_dauSFs_eleReco_up = -99.;
      m_dauSFs_eleReco_down = -99.;

      m_jetFakeSF = 1.;
      m_lheNOutPartons = -1;
      m_lheNOutB = -1;
      m_RunNumber = -1. ;
      m_isBoosted = -1 ;

      m_isLeptrigger = 0;
      m_isMETtrigger = 0;
      m_isSingleTautrigger = 0;

      m_genDecMode1 = -1;
      m_genDecMode2 = -1;

      m_genMHH   = -1;
      m_genCosth = -999;

      m_npv = -1. ;
      m_npu = -1. ;
      m_lumi = -1. ;
      m_triggerbit = -1. ;
      m_pass_triggerbit = -1. ;
	  m_legacyAccept = -1;
	  m_metAccept = -1;
	  m_singletauAccept = -1;
      m_cross_monitoring_trig = false ;
      m_rho = -1. ;

      m_pairType = -1. ;
      m_isMC = -1. ;
      m_isOS = -1. ;

      m_met_phi = -1. ;
      m_met_et = -1. ;
      m_METx = 0.  ;
      m_METy = 0.  ;

      m_metnomu_phi   = -1.;
      m_metnomu_et    = -1.;
      m_METnoMux      = 0.;
      m_METnoMuy      = 0.;
      m_mht_phi   = -1.;
      m_mht_et    = -1.;
      m_MHTx      = 0.;
      m_MHTy      = 0.;
      m_mhtnomu_phi   = -1.;
      m_mhtnomu_et    = -1.;
      m_MHTnoMux      = 0.;
      m_MHTnoMuy      = 0.;

	  m_METx_jetup1    = 0.;
	  m_METx_jetup2	   = 0.;
	  m_METx_jetup3	   = 0.;
	  m_METx_jetup4	   = 0.;
	  m_METx_jetup5	   = 0.;
	  m_METx_jetup6	   = 0.;
	  m_METx_jetup7	   = 0.;
	  m_METx_jetup8	   = 0.;
	  m_METx_jetup9	   = 0.;
	  m_METx_jetup10   = 0.;
	  m_METx_jetup11   = 0.;

	  m_METy_jetup1    = 0.;
	  m_METy_jetup2	   = 0.;
	  m_METy_jetup3	   = 0.;
	  m_METy_jetup4	   = 0.;
	  m_METy_jetup5	   = 0.;
	  m_METy_jetup6	   = 0.;
	  m_METy_jetup7	   = 0.;
	  m_METy_jetup8	   = 0.;
	  m_METy_jetup9	   = 0.;
	  m_METy_jetup10   = 0.;
	  m_METy_jetup11   = 0.;

	  m_METx_jetdown1  = 0.;
	  m_METx_jetdown2  = 0.;
	  m_METx_jetdown3  = 0.;
	  m_METx_jetdown4  = 0.;
	  m_METx_jetdown5  = 0.;
	  m_METx_jetdown6  = 0.;
	  m_METx_jetdown7  = 0.;
	  m_METx_jetdown8  = 0.;
	  m_METx_jetdown9  = 0.;
	  m_METx_jetdown10 = 0.;
	  m_METx_jetdown11 = 0.;

	  m_METy_jetdown1  = 0.;
	  m_METy_jetdown2  = 0.;
	  m_METy_jetdown3  = 0.;
	  m_METy_jetdown4  = 0.;
	  m_METy_jetdown5  = 0.;
	  m_METy_jetdown6  = 0.;
	  m_METy_jetdown7  = 0.;
	  m_METy_jetdown8  = 0.;
	  m_METy_jetdown9  = 0.;
	  m_METy_jetdown10 = 0.;
	  m_METy_jetdown11 = 0.;

	  m_METx_tauup_DM0    = 0.;
	  m_METx_tauup_DM1    = 0.;
	  m_METx_tauup_DM10   = 0.;
	  m_METx_tauup_DM11   = 0.;
	  m_METy_tauup_DM0    = 0.;
	  m_METy_tauup_DM1    = 0.;
	  m_METy_tauup_DM10   = 0.;
	  m_METy_tauup_DM11   = 0.;
	  m_METx_taudown_DM0  = 0.;
	  m_METx_taudown_DM1  = 0.;
	  m_METx_taudown_DM10 = 0.;
	  m_METx_taudown_DM11 = 0.;
	  m_METy_taudown_DM0  = 0.;
	  m_METy_taudown_DM1  = 0.;
	  m_METy_taudown_DM10 = 0.;
	  m_METy_taudown_DM11 = 0.;

	  m_METx_eleup_DM0   = 0.;
	  m_METx_eleup_DM1   = 0.;
	  m_METy_eleup_DM0   = 0.;
	  m_METy_eleup_DM1   = 0.;
	  m_METx_eledown_DM0 = 0.;
	  m_METx_eledown_DM1 = 0.;
	  m_METy_eledown_DM0 = 0.;
	  m_METy_eledown_DM1 = 0.;

	  m_METx_eesUp = 0., m_METx_eesDown = 0.;
	  m_METx_eerUp = 0., m_METx_eerDown = 0.;
	  m_METy_eesUp = 0., m_METy_eesDown = 0.;
	  m_METy_eerUp = 0., m_METy_eerDown = 0.;

      m_METx_muup       = 0.;
	  m_METy_muup		= 0.;
	  m_METx_mudown		= 0.;
	  m_METy_mudown		= 0.;

	  m_METx_jetupTot	= 0.;
	  m_METy_jetupTot	= 0.;
	  m_METx_jetdownTot = 0.;
	  m_METy_jetdownTot = 0.;

      m_met_et_corr =-1.;
      m_met_cov00 = -1.;
      m_met_cov01 = -1.;
      m_met_cov10 = -1.;
      m_met_cov11 = -1.;
	  
      m_DeepMET_ResponseTune_px = -999;
      m_DeepMET_ResponseTune_py = -999;
      m_DeepMET_ResolutionTune_px = -999;
      m_DeepMET_ResolutionTune_py = -999;

      m_dau1_iso  = -1. ;
      m_dau1_eleMVAiso = -1;
      m_dau1_deepTauVsJet = -1;
      m_dau1_deepTauVsEle = -1;
      m_dau1_deepTauVsMu = -1;

      m_dau1_pt  = -1. ;

      m_dau1_pt_muup   = -1.;
      m_dau1_pt_mudown = -1. ;
      m_dau1_e_muup    = -1.;
      m_dau1_e_mudown  = -1.;

      m_dau1_pt_tauup_DM0  = -1. ;
      m_dau1_pt_tauup_DM1  = -1. ;
      m_dau1_pt_tauup_DM10  = -1. ;
      m_dau1_pt_tauup_DM11  = -1. ;
      m_dau1_e_tauup_DM0  = -1. ;
      m_dau1_e_tauup_DM1  = -1. ;
      m_dau1_e_tauup_DM10  = -1. ;
      m_dau1_e_tauup_DM11  = -1. ;

      m_dau1_pt_taudown_DM0  = -1. ;
      m_dau1_pt_taudown_DM1  = -1. ;
      m_dau1_pt_taudown_DM10  = -1. ;
      m_dau1_pt_taudown_DM11  = -1. ;
      m_dau1_e_taudown_DM0  = -1. ;
      m_dau1_e_taudown_DM1  = -1. ;
      m_dau1_e_taudown_DM10  = -1. ;
      m_dau1_e_taudown_DM11  = -1. ;

      m_dau1_pt_eleup_DM0  = -1. ;
      m_dau1_pt_eleup_DM1  = -1. ;
      m_dau1_e_eleup_DM0  = -1. ;
      m_dau1_e_eleup_DM1  = -1. ;

      m_dau1_pt_eledown_DM0  = -1. ;
      m_dau1_pt_eledown_DM1  = -1. ;
      m_dau1_e_eledown_DM0  = -1. ;
      m_dau1_e_eledown_DM1  = -1. ;

	  m_dau1_pt_eesUp   = -1.;
	  m_dau1_pt_eesDown = -1.;
	  m_dau1_pt_eerUp   = -1.;
	  m_dau1_pt_eerDown = -1.;
	  m_dau1_e_eesUp    = -1.;
	  m_dau1_e_eesDown  = -1.;
	  m_dau1_e_eerUp    = -1.;
	  m_dau1_e_eerDown  = -1.;

      m_dau1_eta  = -1. ;
      m_dau1_phi  = -1. ;
      m_dau1_e  = -1. ;
      m_dau1_flav  = -1. ;
      m_dau1_dxy  = -1. ;
      m_dau1_dz  = -1. ;
      m_dau1_decayMode  = -1 ;

      m_genmatched1_pt  = -1.;
      m_genmatched1_eta = -1.;
      m_genmatched1_phi = -1.;
      m_genmatched1_e   = -1.;
      m_genmatched2_pt  = -1.;
      m_genmatched2_eta = -1.;
      m_genmatched2_phi = -1.;
      m_genmatched2_e   = -1.;

      m_hasgenmatch1 = false;
      m_hasgenmatch2 = false;

      m_dau2_iso  = -1. ;
      m_dau2_deepTauVsJet = -1;
      m_dau2_deepTauVsEle = -1;
      m_dau2_deepTauVsMu = -1;

      m_genH1_pt = -1;
      m_genH1_eta = -1;
      m_genH1_phi = -1;
      m_genH1_e = -1;
      m_genH2_pt = -1;
      m_genH2_eta = -1;
      m_genH2_phi = -1;
      m_genH2_e = -1;

      m_genB1_pt = -1;
      m_genB1_eta = -1;
      m_genB1_phi = -1;
      m_genB1_e = -1;
      m_genB2_pt = -1;
      m_genB2_eta = -1;
      m_genB2_phi = -1;
      m_genB2_e = -1;

      m_genBQuark1_pt = -1;
      m_genBQuark1_eta = -1;
      m_genBQuark1_phi = -1;
      m_genBQuark1_e = -1;
      m_genBQuark1_motherPdgId = -1;
      m_genBQuark2_pt = -1;
      m_genBQuark2_eta = -1;
      m_genBQuark2_phi = -1;
      m_genBQuark2_e = -1;
      m_genBQuark2_motherPdgId = -1;
      m_genBQuark3_pt = -1;
      m_genBQuark3_eta = -1;
      m_genBQuark3_phi = -1;
      m_genBQuark3_e = -1;
      m_genBQuark3_motherPdgId = -1;
      m_genBQuark4_pt = -1;
      m_genBQuark4_eta = -1;
      m_genBQuark4_phi = -1;
      m_genBQuark4_e = -1;
      m_genBQuark4_motherPdgId = -1;

      m_genLepton1_pt = -1;
      m_genLepton1_eta = -1;
      m_genLepton1_phi = -1;
      m_genLepton1_e = -1;
      m_genLepton1_pdgId = -1;
      m_genLepton1_motherPdgId = -1;
      m_genLepton2_pt = -1;
      m_genLepton2_eta = -1;
      m_genLepton2_phi = -1;
      m_genLepton2_e = -1;
      m_genLepton2_pdgId = -1;
      m_genLepton2_motherPdgId = -1;
      m_genLepton3_pt = -1;
      m_genLepton3_eta = -1;
      m_genLepton3_phi = -1;
      m_genLepton3_e = -1;
      m_genLepton3_pdgId = -1;
      m_genLepton3_motherPdgId = -1;
      m_genLepton4_pt = -1;
      m_genLepton4_eta = -1;
      m_genLepton4_phi = -1;
      m_genLepton4_e = -1;
      m_genLepton4_pdgId = -1;
      m_genLepton4_motherPdgId = -1;

      m_genNu1_pt = -1;
      m_genNu1_eta = -1;
      m_genNu1_phi = -1;
      m_genNu1_e = -1;
      m_genNu2_pt = -1;
      m_genNu2_eta = -1;
      m_genNu2_phi = -1;
      m_genNu2_e = -1;
      m_genNuNoMatch_pt = -1;
      m_genNuNoMatch_eta = -1;
      m_genNuNoMatch_phi = -1;
      m_genNuNoMatch_e = -1;
      m_genNuTot_pt = -1;
      m_genNuTot_eta = -1;
      m_genNuTot_phi = -1;
      m_genNuTot_e = -1;
      m_genNuTotWithNoMatch_pt = -1;
      m_genNuTotWithNoMatch_eta = -1;
      m_genNuTotWithNoMatch_phi = -1;
      m_genNuTotWithNoMatch_e = -1;
      m_recoGenTauH_pt   = -1;
      m_recoGenTauH_eta  = -1;
      m_recoGenTauH_phi  = -1;
      m_recoGenTauH_e    = -1;
      m_recoGenTauH_mass = -1;
      m_recoGenWithNoMatchTauH_pt   = -1;
      m_recoGenWithNoMatchTauH_eta  = -1;
      m_recoGenWithNoMatchTauH_phi  = -1;
      m_recoGenWithNoMatchTauH_e    = -1;
      m_recoGenWithNoMatchTauH_mass = -1;

      m_dau2_pt  = -1.;

      m_dau2_pt_muup    = -1.;
      m_dau2_pt_mudown  = -1.;
      m_dau2_e_muup   = -1.;
      m_dau2_e_mudown = -1.;

      m_dau2_pt_tauup_DM0  = -1.;
      m_dau2_pt_tauup_DM1  = -1.;
      m_dau2_pt_tauup_DM10  = -1.;
      m_dau2_pt_tauup_DM11  = -1.;
      m_dau2_e_tauup_DM0  = -1.;
      m_dau2_e_tauup_DM1  = -1.;
      m_dau2_e_tauup_DM10  = -1.;
      m_dau2_e_tauup_DM11  = -1.;

      m_dau2_pt_taudown_DM0  = -1.;
      m_dau2_pt_taudown_DM1  = -1.;
      m_dau2_pt_taudown_DM10  = -1.;
      m_dau2_pt_taudown_DM11  = -1.;
      m_dau2_e_taudown_DM0  = -1.;
      m_dau2_e_taudown_DM1  = -1.;
      m_dau2_e_taudown_DM10  = -1.;
      m_dau2_e_taudown_DM11  = -1.;

      m_dau2_pt_eleup_DM0  = -1.;
      m_dau2_pt_eleup_DM1  = -1.;
      m_dau2_e_eleup_DM0  = -1.;
      m_dau2_e_eleup_DM1  = -1.;

      m_dau2_pt_eledown_DM0  = -1.;
      m_dau2_pt_eledown_DM1  = -1.;
      m_dau2_e_eledown_DM0  = -1.;
      m_dau2_e_eledown_DM1  = -1.;

	  m_dau2_pt_eesUp  = -1.;
	  m_dau2_pt_eesDown  = -1.;
	  m_dau2_pt_eerUp  = -1.;
	  m_dau2_pt_eerDown  = -1.;

	  m_dau2_e_eesUp  = -1.;
	  m_dau2_e_eesDown  = -1.;
	  m_dau2_e_eerUp  = -1.;
	  m_dau2_e_eerDown  = -1.;
	  
      m_dau2_eta  = -1.;
      m_dau2_phi  = -1.;
      m_dau2_e  = -1.;
      m_dau2_flav  = -1.;
      m_dau2_dxy  = -1.;
      m_dau2_dz  = -1.;
      m_dau2_decayMode  = -1;

      m_bjet1_pt			 = -1.;
	  m_bjet1_eta			 = -99.;
	  m_bjet1_phi			 = -99.;
	  m_bjet1_e				 = -1.;
	  m_bjet1_bID_deepFlavor = -1.;
	  m_bjet1_cID_deepFlavor = -1.;
	  m_bjet1_CvsL			 = -1.;
	  m_bjet1_CvsB			 = -1.;
	  m_bjet1_HHbtag		 = -1.;
	  m_bjet1_pnet_bb		 = -1.;
	  m_bjet1_pnet_cc		 = -1.;
	  m_bjet1_pnet_b		 = -1.;
	  m_bjet1_pnet_c		 = -1.;
	  m_bjet1_pnet_g		 = -1.;
	  m_bjet1_pnet_uds		 = -1.;
	  m_bjet1_pnet_pu		 = -1.;
	  m_bjet1_pnet_undef	 = -1.;
	  m_bjet1_smearFactor	 = -1.;
	  m_bjet1_bMVAID		 = -1. ;
	  m_bjet1_PUjetIDupdated = -99.;
	  m_bjet1_flav			 = -1 ;

      m_bjet1_hasgenjet = false;
      m_bjet1_JER = -1.;
      m_bjet1_JER_jetup.clear();
      m_bjet1_JER_jetdown.clear();
      m_bjet1_JER_jetupTot   = -1.;
      m_bjet1_JER_jetdownTot = -1.;

      m_bjet1_gen_matched = false;

      m_bjet2_pt			 = -1.;
	  m_bjet2_eta			 = -99.;
	  m_bjet2_phi			 = -99.;
	  m_bjet2_e				 = -1.;
	  m_bjet2_bID_deepFlavor = -1.;
	  m_bjet2_cID_deepFlavor = -1.;
	  m_bjet2_CvsL			 = -1.;
	  m_bjet2_CvsB			 = -1.;
	  m_bjet2_HHbtag		 = -1.;
	  m_bjet2_pnet_bb		 = -1.;
	  m_bjet2_pnet_cc		 = -1.;
	  m_bjet2_pnet_b		 = -1.;
	  m_bjet2_pnet_c		 = -1.;
	  m_bjet2_pnet_g		 = -1.;
	  m_bjet2_pnet_uds		 = -1.;
	  m_bjet2_pnet_pu		 = -1.;
	  m_bjet2_pnet_undef	 = -1.;
	  m_bjet2_smearFactor	 = -1.;
	  m_bjet2_bMVAID		 = -1.;
	  m_bjet2_PUjetIDupdated = -99.;
	  m_bjet2_flav			 = -1 ;

      m_bjet2_hasgenjet = false;
      m_bjet2_JER = -1.;
      m_bjet2_JER_jetup.clear();
      m_bjet2_JER_jetdown.clear();
      m_bjet2_JER_jetupTot   = -1.;
      m_bjet2_JER_jetdownTot = -1.;

      m_bjet2_gen_matched = false;

      m_bjets_bID_deepFlavor = -1.;
      m_bjets_cID_deepFlavor = -1.;
      m_bjets_gen_matched = false;
      m_nfatjets = -1;
      m_fatjet_highest_pT_bTag = -1;
      m_fatjet_pt  = -1.;
      m_fatjet_eta = -9.;
      m_fatjet_phi = -9.;
	  m_fatjet_e   = -1.;
      m_fatjet_bID_deepFlavor = -1.;
      m_fatjet_massIndependentDeepDoubleBvLJetTags_probHbb = -1.;
      m_fatjet_deepDoubleBvLJetTags_probHbb = -1.;
      m_fatjet_deepBoostedJetTags_probHbb = -1.;
      m_fatjet_particleNetJetTags_probHbb = -1.;
      m_fatjet_particleNetDiscriminatorsJetTags_HbbvsQCD = -1.;
      m_fatjet_particleNetMDJetTags_probXbb = -1.;
      m_fatjet_particleNetMDJetTags_probXcc = -1.;
      m_fatjet_particleNetMDJetTags_probXqq = -1.;
      m_fatjet_particleNetMDJetTags_probQCD = -1.;
      m_fatjet_particleNetMDJetTags_mass = -1.;  
      m_fatjet_particleNetMDJetTags_score = -1.;  
      m_fatjet_filteredMass = -1.;
      m_fatjet_prunedMass = -1.;
      m_fatjet_trimmedMass = -1.;
      m_fatjet_softdropMass = -1.;
      m_fatjet_tau1 = -1.;
      m_fatjet_tau2 = -1.;
      m_fatjet_tau3 = -1.;
      m_fatjet_nsubjets = -1;
      m_fatjet_hasMatchedSj = -1;
      m_dR_subj1_subj2 = -1.;

      m_HHbregrsvfit_pt  = -1.;
      m_HHbregrsvfit_eta = -1.;
      m_HHbregrsvfit_phi = -1.;
      m_HHbregrsvfit_m   = -1.;
      m_fatjet_particleNetMDJetTags_HP_SF = 1.;
      m_fatjet_particleNetMDJetTags_HP_SF_up = 1.;
      m_fatjet_particleNetMDJetTags_HP_SF_down = 1.;
      m_fatjet_particleNetMDJetTags_MP_SF = 1.;
      m_fatjet_particleNetMDJetTags_MP_SF_up = 1.;
      m_fatjet_particleNetMDJetTags_MP_SF_down = 1.;
      m_fatjet_particleNetMDJetTags_LP_SF = 1.;
      m_fatjet_particleNetMDJetTags_LP_SF_up = 1.;
      m_fatjet_particleNetMDJetTags_LP_SF_down = 1.;

      m_subjetjet1_pt = -1. ;
      m_subjetjet1_eta  = -1. ;
      m_subjetjet1_phi  = -1. ;
      m_subjetjet1_e  = -1. ;
      m_subjetjet1_bID_deepFlavor = -1. ;

      m_subjetjet2_pt = -1. ;
      m_subjetjet2_eta  = -1. ;
      m_subjetjet2_phi  = -1. ;
      m_subjetjet2_e  = -1. ;
      m_subjetjet2_bID_deepFlavor = -1. ;

      m_genjet1_pt  = -1. ;
      m_genjet1_eta  = -1. ;
      m_genjet1_phi  = -1. ;
      m_genjet1_e  = -1. ;

      m_genjet2_pt  = -1. ;
      m_genjet2_eta  = -1. ;
      m_genjet2_phi  = -1. ;
      m_genjet2_e  = -1. ;

      m_tauH_pt = -1. ;
      m_tauH_eta = -1. ;
      m_tauH_phi = -1. ;
      m_tauH_e = -1. ;
      m_tauH_mass = -1. ;
      m_tauH_SVFIT_mass = -1. ;
      m_tauH_SVFIT_pt = -1. ;
      m_tauH_SVFIT_eta = -1. ;
      m_tauH_SVFIT_phi = -1. ;
      m_tauH_SVFIT_METphi = -1. ;
      m_tauH_SVFIT_METrho = -1. ;

      m_bH_pt = -1. ;
      m_bH_eta = -1. ;
      m_bH_phi = -1. ;
      m_bH_e = -1. ;
      m_bH_mass = -1. ;

      m_HHsvfit_pt = -1. ;
      m_HHsvfit_eta = -1. ;
      m_HHsvfit_phi = -1. ;
      m_HHsvfit_e = -1. ;
      m_HHsvfit_mass = -1. ;

      m_HH_pt = -1. ;
      m_HH_eta = -1. ;
      m_HH_phi = -1. ;
      m_HH_e = -1. ;
      m_HH_mass = -1. ;

      m_HHKin_mass = -1. ;
      m_HHKin_chi2 = -1. ;

      m_HH_deltaPhi = -1. ;
      m_HH_deltaEta = -1. ;
      m_HH_deltaR = -1. ;
      m_HHsvfit_deltaPhi = -1. ;
      m_tauHMet_deltaPhi = -1. ;
      m_tauHsvfitMet_deltaPhi = -1. ;
      m_bHMet_deltaPhi = -1. ;
      m_ditau_deltaPhi = -1. ;
      m_ditau_deltaEta = -1. ;
      m_dib_deltaPhi = -1. ;
      m_dib_deltaEta = -1. ;
      m_ditau_deltaR = -1. ;
      m_dib_deltaR = -1. ;
      m_dib_deltaR_per_bHpt = -1. ;
      m_btau_deltaRmin = -1. ;
      m_btau_deltaRmax = -1. ;
      m_dau1MET_deltaphi = -1. ;
      m_dau2MET_deltaphi = -1. ;
      m_HT20     = 0;
      m_HT50     = 0;
      m_HT20Full = 0;
      m_jet20centrality = 0;

      m_HHkinsvfit_bHmass =-1;
      m_HHkinsvfit_pt =-1;
      m_HHkinsvfit_eta = -1;
      m_HHkinsvfit_phi = -1;
      m_HHkinsvfit_e =-1;
      m_HHkinsvfit_m =-1;
      
      m_HEM1516veto = -1;
      
      m_jets_pt.clear () ;
      m_jets_eta.clear () ;
      m_jets_phi.clear () ;
      m_jets_e.clear () ;
      m_jets_btag_deepFlavor.clear () ;
      m_jets_ctag_deepFlavor.clear () ;
      m_jets_CvsL.clear () ;
      m_jets_CvsB.clear () ;
      m_jets_HHbtag.clear () ;
      m_jets_pnet_bb.clear();
      m_jets_pnet_cc.clear();
      m_jets_pnet_b.clear();
      m_jets_pnet_c.clear();
      m_jets_pnet_g.clear();
      m_jets_pnet_uds.clear();
      m_jets_pnet_pu.clear();
      m_jets_pnet_undef.clear();
      m_smearFactor_AK4.clear();
      m_smearFactor_AK8.clear();
      m_jets_flav.clear () ;
      m_jets_isH.clear () ;
      m_jets_hasgenjet.clear () ;
      m_njets = 0 ;
      m_addjets = 0 ;
      m_njets20 = 0 ;
      m_njets50 = 0 ;
      m_nbjetscand = 0 ;
      m_nbjets20 = 0 ;
      m_nbjets50 = 0 ;
      m_njetsBHadFlav = 0 ;
      m_njetsCHadFlav = 0 ;
      m_dau1_jecUnc= -1;
      m_dau2_jecUnc= -1;
      m_bjet1_jecUnc = -1;
      m_bjet2_jecUnc = -1;
      m_jets_jecUnc.clear() ;
      m_PUjetID.clear() ;

      m_leps_pt.clear () ;
      m_leps_eta.clear () ;
      m_leps_phi.clear () ;
      m_leps_e.clear () ;
      m_leps_flav.clear () ;
      m_nleps = 0 ;

      m_bH_pt_jetup.clear() ;
      m_bH_pt_jetdown.clear() ;
      m_bH_mass_jetup.clear() ;
      m_bH_mass_jetdown.clear() ;

	  m_bjet1_pt_jetup1  = -1.;
	  m_bjet1_pt_jetup2	 = -1.;
	  m_bjet1_pt_jetup3	 = -1.;
	  m_bjet1_pt_jetup4	 = -1.;
	  m_bjet1_pt_jetup5	 = -1.;
	  m_bjet1_pt_jetup6	 = -1.;
	  m_bjet1_pt_jetup7	 = -1.;
	  m_bjet1_pt_jetup8	 = -1.;
	  m_bjet1_pt_jetup9	 = -1.;
	  m_bjet1_pt_jetup10 = -1.;
	  m_bjet1_pt_jetup11 = -1.;
	  m_bjet1_e_jetup1	 = -1.;
	  m_bjet1_e_jetup2	 = -1.;
	  m_bjet1_e_jetup3	 = -1.;
	  m_bjet1_e_jetup4	 = -1.;
	  m_bjet1_e_jetup5	 = -1.;
	  m_bjet1_e_jetup6	 = -1.;
	  m_bjet1_e_jetup7	 = -1.;
	  m_bjet1_e_jetup8	 = -1.;
	  m_bjet1_e_jetup9	 = -1.;
	  m_bjet1_e_jetup10	 = -1.;
	  m_bjet1_e_jetup11	 = -1.;

      m_bjet1_pt_jetdown1  = -1.;
	  m_bjet1_pt_jetdown2  = -1.;
	  m_bjet1_pt_jetdown3  = -1.;
	  m_bjet1_pt_jetdown4  = -1.;
	  m_bjet1_pt_jetdown5  = -1.;
	  m_bjet1_pt_jetdown6  = -1.;
	  m_bjet1_pt_jetdown7  = -1.;
	  m_bjet1_pt_jetdown8  = -1.;
	  m_bjet1_pt_jetdown9  = -1.;
	  m_bjet1_pt_jetdown10 = -1.;
	  m_bjet1_pt_jetdown11 = -1.;
	  m_bjet1_e_jetdown1   = -1.;
	  m_bjet1_e_jetdown2   = -1.;
	  m_bjet1_e_jetdown3   = -1.;
	  m_bjet1_e_jetdown4   = -1.;
	  m_bjet1_e_jetdown5   = -1.;
	  m_bjet1_e_jetdown6   = -1.;
	  m_bjet1_e_jetdown7   = -1.;
	  m_bjet1_e_jetdown8   = -1.;
	  m_bjet1_e_jetdown9   = -1.;
	  m_bjet1_e_jetdown10  = -1.;
	  m_bjet1_e_jetdown11  = -1.;

      m_bjet2_pt_jetup1  = -1.;
	  m_bjet2_pt_jetup2	 = -1.;
	  m_bjet2_pt_jetup3	 = -1.;
	  m_bjet2_pt_jetup4	 = -1.;
	  m_bjet2_pt_jetup5	 = -1.;
	  m_bjet2_pt_jetup6	 = -1.;
	  m_bjet2_pt_jetup7	 = -1.;
	  m_bjet2_pt_jetup8	 = -1.;
	  m_bjet2_pt_jetup9	 = -1.;
	  m_bjet2_pt_jetup10 = -1.;
	  m_bjet2_pt_jetup11 = -1.;
	  m_bjet2_e_jetup1	 = -1.;
	  m_bjet2_e_jetup2	 = -1.;
	  m_bjet2_e_jetup3	 = -1.;
	  m_bjet2_e_jetup4	 = -1.;
	  m_bjet2_e_jetup5	 = -1.;
	  m_bjet2_e_jetup6	 = -1.;
	  m_bjet2_e_jetup7	 = -1.;
	  m_bjet2_e_jetup8	 = -1.;
	  m_bjet2_e_jetup9	 = -1.;
	  m_bjet2_e_jetup10	 = -1.;
	  m_bjet2_e_jetup11	 = -1.;

	  m_bjet2_pt_jetdown1  = -1.;
	  m_bjet2_pt_jetdown2  = -1.;
	  m_bjet2_pt_jetdown3  = -1.;
	  m_bjet2_pt_jetdown4  = -1.;
	  m_bjet2_pt_jetdown5  = -1.;
	  m_bjet2_pt_jetdown6  = -1.;
	  m_bjet2_pt_jetdown7  = -1.;
	  m_bjet2_pt_jetdown8  = -1.;
	  m_bjet2_pt_jetdown9  = -1.;
	  m_bjet2_pt_jetdown10 = -1.;
	  m_bjet2_pt_jetdown11 = -1.;
	  m_bjet2_e_jetdown1   = -1.;
	  m_bjet2_e_jetdown2   = -1.;
	  m_bjet2_e_jetdown3   = -1.;
	  m_bjet2_e_jetdown4   = -1.;
	  m_bjet2_e_jetdown5   = -1.;
	  m_bjet2_e_jetdown6   = -1.;
	  m_bjet2_e_jetdown7   = -1.;
	  m_bjet2_e_jetdown8   = -1.;
	  m_bjet2_e_jetdown9   = -1.;
	  m_bjet2_e_jetdown10  = -1.;
	  m_bjet2_e_jetdown11  = -1.;

      m_fatjet_pt_jetup1  = -1.;
	  m_fatjet_pt_jetup2  = -1.;
	  m_fatjet_pt_jetup3  = -1.;
	  m_fatjet_pt_jetup4  = -1.;
	  m_fatjet_pt_jetup5  = -1.;
	  m_fatjet_pt_jetup6  = -1.;
	  m_fatjet_pt_jetup7  = -1.;
	  m_fatjet_pt_jetup8  = -1.;
	  m_fatjet_pt_jetup9  = -1.;
	  m_fatjet_pt_jetup10 = -1.;
	  m_fatjet_pt_jetup11 = -1.;
	  m_fatjet_e_jetup1	  = -1.;
	  m_fatjet_e_jetup2	  = -1.;
	  m_fatjet_e_jetup3	  = -1.;
	  m_fatjet_e_jetup4	  = -1.;
	  m_fatjet_e_jetup5	  = -1.;
	  m_fatjet_e_jetup6	  = -1.;
	  m_fatjet_e_jetup7	  = -1.;
	  m_fatjet_e_jetup8	  = -1.;
	  m_fatjet_e_jetup9	  = -1.;
	  m_fatjet_e_jetup10  = -1.;
	  m_fatjet_e_jetup11  = -1.;

	  m_fatjet_pt_jetdown1  = -1.;
	  m_fatjet_pt_jetdown2	= -1.;
	  m_fatjet_pt_jetdown3	= -1.;
	  m_fatjet_pt_jetdown4	= -1.;
	  m_fatjet_pt_jetdown5	= -1.;
	  m_fatjet_pt_jetdown6	= -1.;
	  m_fatjet_pt_jetdown7	= -1.;
	  m_fatjet_pt_jetdown8	= -1.;
	  m_fatjet_pt_jetdown9	= -1.;
	  m_fatjet_pt_jetdown10 = -1.;
	  m_fatjet_pt_jetdown11 = -1.;
	  m_fatjet_e_jetdown1	= -1.;
	  m_fatjet_e_jetdown2	= -1.;
	  m_fatjet_e_jetdown3	= -1.;
	  m_fatjet_e_jetdown4	= -1.;
	  m_fatjet_e_jetdown5	= -1.;
	  m_fatjet_e_jetdown6	= -1.;
	  m_fatjet_e_jetdown7	= -1.;
	  m_fatjet_e_jetdown8	= -1.;
	  m_fatjet_e_jetdown9	= -1.;
	  m_fatjet_e_jetdown10	= -1.;
	  m_fatjet_e_jetdown11	= -1.;

      m_bH_mass_jetup1  = -1.;
	  m_bH_mass_jetup2	= -1.;
	  m_bH_mass_jetup3	= -1.;
	  m_bH_mass_jetup4	= -1.;
	  m_bH_mass_jetup5	= -1.;
	  m_bH_mass_jetup6	= -1.;
	  m_bH_mass_jetup7	= -1.;
	  m_bH_mass_jetup8	= -1.;
	  m_bH_mass_jetup9	= -1.;
	  m_bH_mass_jetup10 = -1.;
	  m_bH_mass_jetup11 = -1.;

      m_bH_mass_jetdown1  = -1.;
	  m_bH_mass_jetdown2  = -1.;
	  m_bH_mass_jetdown3  = -1.;
	  m_bH_mass_jetdown4  = -1.;
	  m_bH_mass_jetdown5  = -1.;
	  m_bH_mass_jetdown6  = -1.;
	  m_bH_mass_jetdown7  = -1.;
	  m_bH_mass_jetdown8  = -1.;
	  m_bH_mass_jetdown9  = -1.;
	  m_bH_mass_jetdown10 = -1.;
	  m_bH_mass_jetdown11 = -1.;
	  
      m_bjet1_pt_jetupTot    = -1.;
	  m_bjet1_pt_jetdownTot	 = -1.;
	  m_bjet2_pt_jetupTot	 = -1.;
	  m_bjet2_pt_jetdownTot	 = -1.;
	  m_bjet1_e_jetupTot	 = -1.;
	  m_bjet1_e_jetdownTot	 = -1.;
	  m_bjet2_e_jetupTot	 = -1.;
	  m_bjet2_e_jetdownTot	 = -1.;
	  m_bH_mass_jetupTot	 = -1.;
	  m_bH_mass_jetdownTot	 = -1.;
	  m_fatjet_pt_jetupTot	 = -1.;
	  m_fatjet_pt_jetdownTot = -1.;
	  m_fatjet_e_jetupTot	 = -1.;
	  m_fatjet_e_jetdownTot	 = -1.;

      m_lheht = 0;

      m_topReweight=-1;

      // Additional variables
      m_dau1_z  = -999.;
      m_dau2_z = -999.;
      m_bjet1_z = -999.;
      m_bjet2_z = -999.;
      m_tauH_z = -999.;
      m_bH_z = -999.;
      m_HH_z = -999.;

      m_HH_zV  = -999.;
      m_HH_A = -999.;

      m_tauH_MET_pt                   = -9. ;
      m_dau2_MET_deltaEta             = -9. ;
      m_bH_MET_deltaEta               = -9. ;
      m_bH_MET_deltaR                 = -9. ;
      m_bH_tauH_MET_deltaR            = -9. ;
      m_ditau_deltaR_per_tauH_MET_pt  = -9. ;

      // fake taus in MC
      m_isTau1real = -1; // -1 if data
      m_isTau2real = -1; // -1 if data
      m_nRealTaus = -1; // -1 if data

      return 0 ;
    }

  int init ()
    {
      m_smallT->Branch ("MC_weight", &m_MC_weight, "MC_weight/F") ;
	  m_smallT->Branch ("MC_weight_fixed", &m_MC_weight_fixed, "MC_weight_fixed/F") ;
	  m_smallT->Branch ("aMCatNLOweight", &m_aMCatNLOweight, "aMCatNLOweight/F") ;
	  m_smallT->Branch ("MC_QCDscale0", &m_MC_QCDscale0, "MC_QCDscale0/F") ;
	  m_smallT->Branch ("MC_QCDscale1", &m_MC_QCDscale1, "MC_QCDscale1/F") ;
	  m_smallT->Branch ("MC_QCDscale2", &m_MC_QCDscale2, "MC_QCDscale2/F") ;
	  m_smallT->Branch ("MC_QCDscale3", &m_MC_QCDscale3, "MC_QCDscale3/F") ;
	  m_smallT->Branch ("MC_QCDscale4", &m_MC_QCDscale4, "MC_QCDscale4/F") ;
	  m_smallT->Branch ("MC_QCDscale5", &m_MC_QCDscale5, "MC_QCDscale5/F") ;
	  m_smallT->Branch ("MC_QCDscale6", &m_MC_QCDscale6, "MC_QCDscale6/F") ;
	  m_smallT->Branch ("MC_pdf0", &m_MC_pdf0, "MC_pdf0/F") ;
	  m_smallT->Branch ("MC_pdf1", &m_MC_pdf1, "MC_pdf1/F") ;
	  m_smallT->Branch ("MC_pdf2", &m_MC_pdf2, "MC_pdf2/F") ;
	  m_smallT->Branch ("MC_pdf3", &m_MC_pdf3, "MC_pdf3/F") ;
	  m_smallT->Branch ("MC_pdf4", &m_MC_pdf4, "MC_pdf4/F") ;
	  m_smallT->Branch ("MC_pdf5", &m_MC_pdf5, "MC_pdf5/F") ;
	  m_smallT->Branch ("MC_pdf6", &m_MC_pdf6, "MC_pdf6/F") ;
	  m_smallT->Branch ("MC_pdf7", &m_MC_pdf7, "MC_pdf7/F") ;
	  m_smallT->Branch ("MC_pdf8", &m_MC_pdf8, "MC_pdf8/F") ;
	  m_smallT->Branch ("MC_pdf9", &m_MC_pdf9, "MC_pdf9/F") ;
	  m_smallT->Branch ("MC_pdf10", &m_MC_pdf10, "MC_pdf10/F") ;
	  m_smallT->Branch ("MC_pdf11", &m_MC_pdf11, "MC_pdf11/F") ;
	  m_smallT->Branch ("MC_pdf12", &m_MC_pdf12, "MC_pdf12/F") ;
	  m_smallT->Branch ("MC_pdf13", &m_MC_pdf13, "MC_pdf13/F") ;
	  m_smallT->Branch ("MC_pdf14", &m_MC_pdf14, "MC_pdf14/F") ;
	  m_smallT->Branch ("MC_pdf15", &m_MC_pdf15, "MC_pdf15/F") ;
	  m_smallT->Branch ("MC_pdf16", &m_MC_pdf16, "MC_pdf16/F") ;
	  m_smallT->Branch ("MC_pdf17", &m_MC_pdf17, "MC_pdf17/F") ;
	  m_smallT->Branch ("MC_pdf18", &m_MC_pdf18, "MC_pdf18/F") ;
	  m_smallT->Branch ("MC_pdf19", &m_MC_pdf19, "MC_pdf19/F") ;
	  m_smallT->Branch ("MC_pdf20", &m_MC_pdf20, "MC_pdf20/F") ;
	  m_smallT->Branch ("MC_pdf21", &m_MC_pdf21, "MC_pdf21/F") ;
	  m_smallT->Branch ("MC_pdf22", &m_MC_pdf22, "MC_pdf22/F") ;
	  m_smallT->Branch ("MC_pdf23", &m_MC_pdf23, "MC_pdf23/F") ;
	  m_smallT->Branch ("MC_pdf24", &m_MC_pdf24, "MC_pdf24/F") ;
	  m_smallT->Branch ("MC_pdf25", &m_MC_pdf25, "MC_pdf25/F") ;
	  m_smallT->Branch ("MC_pdf26", &m_MC_pdf26, "MC_pdf26/F") ;
	  m_smallT->Branch ("MC_pdf27", &m_MC_pdf27, "MC_pdf27/F") ;
	  m_smallT->Branch ("MC_pdf28", &m_MC_pdf28, "MC_pdf28/F") ;
	  m_smallT->Branch ("MC_pdf29", &m_MC_pdf29, "MC_pdf29/F") ;
	  m_smallT->Branch ("MC_pdf30", &m_MC_pdf30, "MC_pdf30/F") ;
	  m_smallT->Branch ("MC_pdf31", &m_MC_pdf31, "MC_pdf31/F") ;
	  m_smallT->Branch ("MC_pdf32", &m_MC_pdf32, "MC_pdf32/F") ;
	  m_smallT->Branch ("MC_pdf33", &m_MC_pdf33, "MC_pdf33/F") ;
	  m_smallT->Branch ("MC_pdf34", &m_MC_pdf34, "MC_pdf34/F") ;
	  m_smallT->Branch ("MC_pdf35", &m_MC_pdf35, "MC_pdf35/F") ;
	  m_smallT->Branch ("MC_pdf36", &m_MC_pdf36, "MC_pdf36/F") ;
	  m_smallT->Branch ("MC_pdf37", &m_MC_pdf37, "MC_pdf37/F") ;
	  m_smallT->Branch ("MC_pdf38", &m_MC_pdf38, "MC_pdf38/F") ;
	  m_smallT->Branch ("MC_pdf39", &m_MC_pdf39, "MC_pdf39/F") ;
	  m_smallT->Branch ("MC_pdf40", &m_MC_pdf40, "MC_pdf40/F") ;
	  m_smallT->Branch ("MC_pdf41", &m_MC_pdf41, "MC_pdf41/F") ;
	  m_smallT->Branch ("MC_pdf42", &m_MC_pdf42, "MC_pdf42/F") ;
	  m_smallT->Branch ("MC_pdf43", &m_MC_pdf43, "MC_pdf43/F") ;
	  m_smallT->Branch ("MC_pdf44", &m_MC_pdf44, "MC_pdf44/F") ;
	  m_smallT->Branch ("MC_pdf45", &m_MC_pdf45, "MC_pdf45/F") ;
	  m_smallT->Branch ("MC_pdf46", &m_MC_pdf46, "MC_pdf46/F") ;
	  m_smallT->Branch ("MC_pdf47", &m_MC_pdf47, "MC_pdf47/F") ;
	  m_smallT->Branch ("MC_pdf48", &m_MC_pdf48, "MC_pdf48/F") ;
	  m_smallT->Branch ("MC_pdf49", &m_MC_pdf49, "MC_pdf49/F") ;
	  m_smallT->Branch ("MC_pdf50", &m_MC_pdf50, "MC_pdf50/F") ;
	  m_smallT->Branch ("MC_pdf51", &m_MC_pdf51, "MC_pdf51/F") ;
	  m_smallT->Branch ("MC_pdf52", &m_MC_pdf52, "MC_pdf52/F") ;
	  m_smallT->Branch ("MC_pdf53", &m_MC_pdf53, "MC_pdf53/F") ;
	  m_smallT->Branch ("MC_pdf54", &m_MC_pdf54, "MC_pdf54/F") ;
	  m_smallT->Branch ("MC_pdf55", &m_MC_pdf55, "MC_pdf55/F") ;
	  m_smallT->Branch ("MC_pdf56", &m_MC_pdf56, "MC_pdf56/F") ;
	  m_smallT->Branch ("MC_pdf57", &m_MC_pdf57, "MC_pdf57/F") ;
	  m_smallT->Branch ("MC_pdf58", &m_MC_pdf58, "MC_pdf58/F") ;
	  m_smallT->Branch ("MC_pdf59", &m_MC_pdf59, "MC_pdf59/F") ;
	  m_smallT->Branch ("MC_pdf60", &m_MC_pdf60, "MC_pdf60/F") ;
	  m_smallT->Branch ("MC_pdf61", &m_MC_pdf61, "MC_pdf61/F") ;
	  m_smallT->Branch ("MC_pdf62", &m_MC_pdf62, "MC_pdf62/F") ;
	  m_smallT->Branch ("MC_pdf63", &m_MC_pdf63, "MC_pdf63/F") ;
	  m_smallT->Branch ("MC_pdf64", &m_MC_pdf64, "MC_pdf64/F") ;
	  m_smallT->Branch ("MC_pdf65", &m_MC_pdf65, "MC_pdf65/F") ;
	  m_smallT->Branch ("MC_pdf66", &m_MC_pdf66, "MC_pdf66/F") ;
	  m_smallT->Branch ("MC_pdf67", &m_MC_pdf67, "MC_pdf67/F") ;
	  m_smallT->Branch ("MC_pdf68", &m_MC_pdf68, "MC_pdf68/F") ;
	  m_smallT->Branch ("MC_pdf69", &m_MC_pdf69, "MC_pdf69/F") ;
	  m_smallT->Branch ("MC_pdf70", &m_MC_pdf70, "MC_pdf70/F") ;
	  m_smallT->Branch ("MC_pdf71", &m_MC_pdf71, "MC_pdf71/F") ;
	  m_smallT->Branch ("MC_pdf72", &m_MC_pdf72, "MC_pdf72/F") ;
	  m_smallT->Branch ("MC_pdf73", &m_MC_pdf73, "MC_pdf73/F") ;
	  m_smallT->Branch ("MC_pdf74", &m_MC_pdf74, "MC_pdf74/F") ;
	  m_smallT->Branch ("MC_pdf75", &m_MC_pdf75, "MC_pdf75/F") ;
	  m_smallT->Branch ("MC_pdf76", &m_MC_pdf76, "MC_pdf76/F") ;
	  m_smallT->Branch ("MC_pdf77", &m_MC_pdf77, "MC_pdf77/F") ;
	  m_smallT->Branch ("MC_pdf78", &m_MC_pdf78, "MC_pdf78/F") ;
	  m_smallT->Branch ("MC_pdf79", &m_MC_pdf79, "MC_pdf79/F") ;
	  m_smallT->Branch ("MC_pdf80", &m_MC_pdf80, "MC_pdf80/F") ;
	  m_smallT->Branch ("MC_pdf81", &m_MC_pdf81, "MC_pdf81/F") ;
	  m_smallT->Branch ("MC_pdf82", &m_MC_pdf82, "MC_pdf82/F") ;
	  m_smallT->Branch ("MC_pdf83", &m_MC_pdf83, "MC_pdf83/F") ;
	  m_smallT->Branch ("MC_pdf84", &m_MC_pdf84, "MC_pdf84/F") ;
	  m_smallT->Branch ("MC_pdf85", &m_MC_pdf85, "MC_pdf85/F") ;
	  m_smallT->Branch ("MC_pdf86", &m_MC_pdf86, "MC_pdf86/F") ;
	  m_smallT->Branch ("MC_pdf87", &m_MC_pdf87, "MC_pdf87/F") ;
	  m_smallT->Branch ("MC_pdf88", &m_MC_pdf88, "MC_pdf88/F") ;
	  m_smallT->Branch ("MC_pdf89", &m_MC_pdf89, "MC_pdf89/F") ;
	  m_smallT->Branch ("MC_pdf90", &m_MC_pdf90, "MC_pdf90/F") ;
	  m_smallT->Branch ("MC_pdf91", &m_MC_pdf91, "MC_pdf91/F") ;
	  m_smallT->Branch ("MC_pdf92", &m_MC_pdf92, "MC_pdf92/F") ;
	  m_smallT->Branch ("MC_pdf93", &m_MC_pdf93, "MC_pdf93/F") ;
	  m_smallT->Branch ("MC_pdf94", &m_MC_pdf94, "MC_pdf94/F") ;
	  m_smallT->Branch ("MC_pdf95", &m_MC_pdf95, "MC_pdf95/F") ;
	  m_smallT->Branch ("MC_pdf96", &m_MC_pdf96, "MC_pdf96/F") ;
	  m_smallT->Branch ("MC_pdf97", &m_MC_pdf97, "MC_pdf97/F") ;
	  m_smallT->Branch ("MC_pdf98", &m_MC_pdf98, "MC_pdf98/F") ;
	  m_smallT->Branch ("MC_pdf99", &m_MC_pdf99, "MC_pdf99/F") ;
	  m_smallT->Branch ("MC_pdf100", &m_MC_pdf100, "MC_pdf100/F") ;
	  m_smallT->Branch ("MC_astrong0", &m_MC_astrong0, "MC_astrong0/F") ;
	  m_smallT->Branch ("MC_astrong1", &m_MC_astrong1, "MC_astrong1/F") ;
      m_smallT->Branch ("totalWeight", &m_totalWeight, "totalWeight/F") ;
      m_smallT->Branch ("prescaleWeight", &m_prescaleWeight, "prescaleWeight/F");
      m_smallT->Branch ("L1pref_weight", &m_L1pref_weight, "L1pref_weight/F");
      m_smallT->Branch ("L1pref_weight_up", &m_L1pref_weight_up, "L1pref_weight_up/F");
      m_smallT->Branch ("L1pref_weight_down", &m_L1pref_weight_down, "L1pref_weight_down/F");
      m_smallT->Branch ("PUjetID_SF", &m_PUjetID_SF, "PUjetID_SF/F") ;
      m_smallT->Branch ("PUjetID_SF_up", &m_PUjetID_SF_up, "PUjetID_SF_up/F") ;
      m_smallT->Branch ("PUjetID_SF_down", &m_PUjetID_SF_down, "PUjetID_SF_down/F") ;
      m_smallT->Branch ("PUReweight", &m_PUReweight, "PUReweight/F") ;
      m_smallT->Branch ("PUReweight_up", &m_PUReweight_up, "PUReweight_up/F") ;
      m_smallT->Branch ("PUReweight_down", &m_PUReweight_down, "PUReweight_down/F") ;
      m_smallT->Branch ("bTagweightL", &m_bTagweightL, "bTagweightL/F") ;
      m_smallT->Branch ("bTagweightL_up", &m_bTagweightL_up, "bTagweightL_up/F") ;
      m_smallT->Branch ("bTagweightL_down", &m_bTagweightL_down, "bTagweightL_down/F") ;
      m_smallT->Branch ("bTagweightM", &m_bTagweightM, "bTagweightM/F") ;
      m_smallT->Branch ("bTagweightM_up", &m_bTagweightM_up, "bTagweightM_up/F") ;
      m_smallT->Branch ("bTagweightM_down", &m_bTagweightM_down, "bTagweightM_down/F") ;
      m_smallT->Branch ("bTagweightT", &m_bTagweightT, "bTagweightT/F") ;
      m_smallT->Branch ("bTagweightT_up", &m_bTagweightT_up, "bTagweightT_up/F") ;
      m_smallT->Branch ("bTagweightT_down", &m_bTagweightT_down, "bTagweightT_down/F") ;
      m_smallT->Branch ("bTagweightReshape", &m_bTagweightReshape, "bTagweightReshape/F") ;
      m_smallT->Branch ("bTagweightReshape_jes_up", &m_bTagweightReshape_jes_up, "bTagweightReshape_jes_up/F") ;
      m_smallT->Branch ("bTagweightReshape_lf_up", &m_bTagweightReshape_lf_up, "bTagweightReshape_lf_up/F") ;
      m_smallT->Branch ("bTagweightReshape_hf_up", &m_bTagweightReshape_hf_up, "bTagweightReshape_hf_up/F") ;
      m_smallT->Branch ("bTagweightReshape_hfstats1_up", &m_bTagweightReshape_hfstats1_up, "bTagweightReshape_hfstats1_up/F") ;
      m_smallT->Branch ("bTagweightReshape_hfstats2_up", &m_bTagweightReshape_hfstats2_up, "bTagweightReshape_hfstats2_up/F") ;
      m_smallT->Branch ("bTagweightReshape_lfstats1_up", &m_bTagweightReshape_lfstats1_up, "bTagweightReshape_lfstats1_up/F") ;
      m_smallT->Branch ("bTagweightReshape_lfstats2_up", &m_bTagweightReshape_lfstats2_up, "bTagweightReshape_lfstats2_up/F") ;
      m_smallT->Branch ("bTagweightReshape_cferr1_up", &m_bTagweightReshape_cferr1_up, "bTagweightReshape_cferr1_up/F") ;
      m_smallT->Branch ("bTagweightReshape_cferr2_up", &m_bTagweightReshape_cferr2_up, "bTagweightReshape_cferr2_up/F") ;
      m_smallT->Branch ("bTagweightReshape_jes_down", &m_bTagweightReshape_jes_down, "bTagweightReshape_jes_down/F") ;
      m_smallT->Branch ("bTagweightReshape_lf_down", &m_bTagweightReshape_lf_down, "bTagweightReshape_lf_down/F") ;
      m_smallT->Branch ("bTagweightReshape_hf_down", &m_bTagweightReshape_hf_down, "bTagweightReshape_hf_down/F") ;
      m_smallT->Branch ("bTagweightReshape_hfstats1_down", &m_bTagweightReshape_hfstats1_down, "bTagweightReshape_hfstats1_down/F") ;
      m_smallT->Branch ("bTagweightReshape_hfstats2_down", &m_bTagweightReshape_hfstats2_down, "bTagweightReshape_hfstats2_down/F") ;
      m_smallT->Branch ("bTagweightReshape_lfstats1_down", &m_bTagweightReshape_lfstats1_down, "bTagweightReshape_lfstats1_down/F") ;
      m_smallT->Branch ("bTagweightReshape_lfstats2_down", &m_bTagweightReshape_lfstats2_down, "bTagweightReshape_lfstats2_down/F") ;
      m_smallT->Branch ("bTagweightReshape_cferr1_down", &m_bTagweightReshape_cferr1_down, "bTagweightReshape_cferr1_down/F") ;
      m_smallT->Branch ("bTagweightReshape_cferr2_down", &m_bTagweightReshape_cferr2_down, "bTagweightReshape_cferr2_down/F") ;
      m_smallT->Branch ("bTagweightReshape_jetup1" , &m_bTagweightReshape_jetup1 , "bTagweightReshape_jetup1/F") ;
      m_smallT->Branch ("bTagweightReshape_jetup2" , &m_bTagweightReshape_jetup2 , "bTagweightReshape_jetup2/F") ;
      m_smallT->Branch ("bTagweightReshape_jetup3" , &m_bTagweightReshape_jetup3 , "bTagweightReshape_jetup3/F") ;
      m_smallT->Branch ("bTagweightReshape_jetup4" , &m_bTagweightReshape_jetup4 , "bTagweightReshape_jetup4/F") ;
      m_smallT->Branch ("bTagweightReshape_jetup5" , &m_bTagweightReshape_jetup5 , "bTagweightReshape_jetup5/F") ;
      m_smallT->Branch ("bTagweightReshape_jetup6" , &m_bTagweightReshape_jetup6 , "bTagweightReshape_jetup6/F") ;
      m_smallT->Branch ("bTagweightReshape_jetup7" , &m_bTagweightReshape_jetup7 , "bTagweightReshape_jetup7/F") ;
      m_smallT->Branch ("bTagweightReshape_jetup8" , &m_bTagweightReshape_jetup8 , "bTagweightReshape_jetup8/F") ;
      m_smallT->Branch ("bTagweightReshape_jetup9" , &m_bTagweightReshape_jetup9 , "bTagweightReshape_jetup9/F") ;
      m_smallT->Branch ("bTagweightReshape_jetup10", &m_bTagweightReshape_jetup10, "bTagweightReshape_jetup10/F") ;
      m_smallT->Branch ("bTagweightReshape_jetup11", &m_bTagweightReshape_jetup11, "bTagweightReshape_jetup11/F") ;
      m_smallT->Branch ("bTagweightReshape_jetdown1" , &m_bTagweightReshape_jetdown1 , "bTagweightReshape_jetdown1/F") ;
      m_smallT->Branch ("bTagweightReshape_jetdown2" , &m_bTagweightReshape_jetdown2 , "bTagweightReshape_jetdown2/F") ;
      m_smallT->Branch ("bTagweightReshape_jetdown3" , &m_bTagweightReshape_jetdown3 , "bTagweightReshape_jetdown3/F") ;
      m_smallT->Branch ("bTagweightReshape_jetdown4" , &m_bTagweightReshape_jetdown4 , "bTagweightReshape_jetdown4/F") ;
      m_smallT->Branch ("bTagweightReshape_jetdown5" , &m_bTagweightReshape_jetdown5 , "bTagweightReshape_jetdown5/F") ;
      m_smallT->Branch ("bTagweightReshape_jetdown6" , &m_bTagweightReshape_jetdown6 , "bTagweightReshape_jetdown6/F") ;
      m_smallT->Branch ("bTagweightReshape_jetdown7" , &m_bTagweightReshape_jetdown7 , "bTagweightReshape_jetdown7/F") ;
      m_smallT->Branch ("bTagweightReshape_jetdown8" , &m_bTagweightReshape_jetdown8 , "bTagweightReshape_jetdown8/F") ;
      m_smallT->Branch ("bTagweightReshape_jetdown9" , &m_bTagweightReshape_jetdown9 , "bTagweightReshape_jetdown9/F") ;
      m_smallT->Branch ("bTagweightReshape_jetdown10", &m_bTagweightReshape_jetdown10, "bTagweightReshape_jetdown10/F") ;
      m_smallT->Branch ("bTagweightReshape_jetdown11", &m_bTagweightReshape_jetdown11, "bTagweightReshape_jetdown11/F") ;
      m_smallT->Branch ("TTtopPtreweight", &m_TTtopPtreweight, "TTtopPtreweight/F") ;
      m_smallT->Branch ("TTtopPtreweight_up", &m_TTtopPtreweight_up, "TTtopPtreweight_up/F") ;
      m_smallT->Branch ("TTtopPtreweight_down", &m_TTtopPtreweight_down, "TTtopPtreweight_down/F") ;
	  
      m_smallT->Branch ("trigSF",               &m_trigSF,           "trigSF/F") ;
	  m_smallT->Branch ("trigSFnoMET",          &m_trigSFnoMET,      "trigSFnoMET/F") ;
	  m_smallT->Branch ("trigSFnoTau",          &m_trigSFnoTau,      "trigSFnoTau/F") ;
      m_smallT->Branch ("trigSF_ele_up",        &m_trigSF_ele_up,    "trigSF_ele_up/F") ;
      m_smallT->Branch ("trigSF_SL_ele_up",     &m_trigSF_SL_ele_up, "trigSF_SL_ele_up/F") ;
      m_smallT->Branch ("trigSF_cross_ele_up",  &m_trigSF_cross_ele_up, "trigSF_cross_ele_up/F") ;
      m_smallT->Branch ("trigSF_mu_up",         &m_trigSF_mu_up,     "trigSF_mu_up/F") ;
      m_smallT->Branch ("trigSF_SL_mu_up",      &m_trigSF_SL_mu_up,  "trigSF_SL_mu_up/F") ;
      m_smallT->Branch ("trigSF_cross_mu_up",   &m_trigSF_cross_mu_up,"trigSF_cross_mu_up/F") ;
      m_smallT->Branch ("trigSF_tau_DM0_up",    &m_trigSF_tau_DM0_up,    "trigSF_tau_DM0_up/F") ;
      m_smallT->Branch ("trigSF_tau_DM1_up",    &m_trigSF_tau_DM1_up,    "trigSF_tau_DM1_up/F") ;
      m_smallT->Branch ("trigSF_tau_DM10_up",   &m_trigSF_tau_DM10_up,   "trigSF_tau_DM10_up/F") ;
      m_smallT->Branch ("trigSF_tau_DM11_up",   &m_trigSF_tau_DM11_up,   "trigSF_tau_DM11_up/F") ;
      m_smallT->Branch ("trigSF_ele_down",      &m_trigSF_ele_down, "trigSF_ele_down/F") ;
      m_smallT->Branch ("trigSF_SL_ele_down",   &m_trigSF_SL_ele_down, "trigSF_SL_ele_down/F") ;
      m_smallT->Branch ("trigSF_cross_ele_down",&m_trigSF_cross_ele_down, "trigSF_cross_ele_down/F") ;
      m_smallT->Branch ("trigSF_mu_down",       &m_trigSF_mu_down,   "trigSF_mu_down/F") ;
      m_smallT->Branch ("trigSF_SL_mu_down",    &m_trigSF_SL_mu_down,    "trigSF_SL_mu_down/F") ;
      m_smallT->Branch ("trigSF_cross_mu_down", &m_trigSF_cross_mu_down, "trigSF_cross_mu_down/F") ;
      m_smallT->Branch ("trigSF_tau_DM0_down",  &m_trigSF_tau_DM0_down,  "trigSF_tau_DM0_down/F") ;
      m_smallT->Branch ("trigSF_tau_DM1_down",  &m_trigSF_tau_DM1_down,  "trigSF_tau_DM1_down/F") ;
      m_smallT->Branch ("trigSF_tau_DM10_down", &m_trigSF_tau_DM10_down, "trigSF_tau_DM10_down/F") ;
      m_smallT->Branch ("trigSF_tau_DM11_down", &m_trigSF_tau_DM11_down, "trigSF_tau_DM11_down/F") ;
	  m_smallT->Branch ("trigSF_met_up",        &m_trigSF_met_up,    "trigSF_met_up/F") ;
	  m_smallT->Branch ("trigSF_met_down",      &m_trigSF_met_down,  "trigSF_met_down/F") ;
	  m_smallT->Branch ("trigSF_stau_up",       &m_trigSF_stau_up,   "trigSF_stau_up/F") ;
	  m_smallT->Branch ("trigSF_stau_down",     &m_trigSF_stau_down, "trigSF_stau_down/F") ;

      m_smallT->Branch ("jetFakeSF", &m_jetFakeSF, "jetFakeSF/F") ;
	  m_smallT->Branch ("IdSF_deep_2d", &m_IdSF_deep_2d, "IdSF_deep_2d/F") ;
	  m_smallT->Branch("IdSF_leg1_deep_vsJet_2d", &m_IdSF_leg1_deep_vsJet_2d,
					   "IdSF_leg1_deep_vsJet_2d/F") ;
	  m_smallT->Branch("IdSF_leg2_deep_vsJet_2d", &m_IdSF_leg2_deep_vsJet_2d,
					   "IdSF_leg2_deep_vsJet_2d/F") ;
      m_smallT->Branch ("dauSFs", &m_dauSFs, "dauSFs/F") ;
      m_smallT->Branch ("FakeRateSF_deep", &m_FakeRateSF_deep, "FakeRateSF_deep/F") ;

      m_smallT->Branch("dauSFs_tauid_2d_stat0_DM0_up", &m_dauSFs_tauid_2d_stat0_DM0_up,
        "dauSFs_tauid_2d_stat0_DM0_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_stat0_DM0_down", &m_dauSFs_tauid_2d_stat0_DM0_down,
        "dauSFs_tauid_2d_stat0_DM0_down/F");
      m_smallT->Branch("dauSFs_tauid_2d_stat1_DM0_up", &m_dauSFs_tauid_2d_stat1_DM0_up,
        "dauSFs_tauid_2d_stat1_DM0_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_stat1_DM0_down", &m_dauSFs_tauid_2d_stat1_DM0_down,
        "dauSFs_tauid_2d_stat1_DM0_down/F");
      m_smallT->Branch("dauSFs_tauid_2d_systuncorrdmeras_DM0_up", &m_dauSFs_tauid_2d_systuncorrdmeras_DM0_up,
        "dauSFs_tauid_2d_systuncorrdmeras_DM0_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_systuncorrdmeras_DM0_down", &m_dauSFs_tauid_2d_systuncorrdmeras_DM0_down,
        "dauSFs_tauid_2d_systuncorrdmeras_DM0_down/F");

      m_smallT->Branch("dauSFs_tauid_2d_stat0_DM1_up", &m_dauSFs_tauid_2d_stat0_DM1_up,
        "dauSFs_tauid_2d_stat0_DM1_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_stat0_DM1_down", &m_dauSFs_tauid_2d_stat0_DM1_down,
        "dauSFs_tauid_2d_stat0_DM1_down/F");
      m_smallT->Branch("dauSFs_tauid_2d_stat1_DM1_up", &m_dauSFs_tauid_2d_stat1_DM1_up,
        "dauSFs_tauid_2d_stat1_DM1_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_stat1_DM1_down", &m_dauSFs_tauid_2d_stat1_DM1_down,
        "dauSFs_tauid_2d_stat1_DM1_down/F");
      m_smallT->Branch("dauSFs_tauid_2d_systuncorrdmeras_DM1_up", &m_dauSFs_tauid_2d_systuncorrdmeras_DM1_up,
        "dauSFs_tauid_2d_systuncorrdmeras_DM1_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_systuncorrdmeras_DM1_down", &m_dauSFs_tauid_2d_systuncorrdmeras_DM1_down,
        "dauSFs_tauid_2d_systuncorrdmeras_DM1_down/F");

      m_smallT->Branch("dauSFs_tauid_2d_stat0_DM10_up", &m_dauSFs_tauid_2d_stat0_DM10_up,
        "dauSFs_tauid_2d_stat0_DM10_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_stat0_DM10_down", &m_dauSFs_tauid_2d_stat0_DM10_down,
        "dauSFs_tauid_2d_stat0_DM10_down/F");
      m_smallT->Branch("dauSFs_tauid_2d_stat1_DM10_up", &m_dauSFs_tauid_2d_stat1_DM10_up,
        "dauSFs_tauid_2d_stat1_DM10_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_stat1_DM10_down", &m_dauSFs_tauid_2d_stat1_DM10_down,
        "dauSFs_tauid_2d_stat1_DM10_down/F");
      m_smallT->Branch("dauSFs_tauid_2d_systuncorrdmeras_DM10_up", &m_dauSFs_tauid_2d_systuncorrdmeras_DM10_up,
        "dauSFs_tauid_2d_systuncorrdmeras_DM10_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_systuncorrdmeras_DM10_down", &m_dauSFs_tauid_2d_systuncorrdmeras_DM10_down,
        "dauSFs_tauid_2d_systuncorrdmeras_DM10_down/F");

      m_smallT->Branch("dauSFs_tauid_2d_stat0_DM11_up", &m_dauSFs_tauid_2d_stat0_DM11_up,
        "dauSFs_tauid_2d_stat0_DM11_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_stat0_DM11_down", &m_dauSFs_tauid_2d_stat0_DM11_down,
        "dauSFs_tauid_2d_stat0_DM11_down/F");
      m_smallT->Branch("dauSFs_tauid_2d_stat1_DM11_up", &m_dauSFs_tauid_2d_stat1_DM11_up,
        "dauSFs_tauid_2d_stat1_DM11_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_stat1_DM11_down", &m_dauSFs_tauid_2d_stat1_DM11_down,
        "dauSFs_tauid_2d_stat1_DM11_down/F");
      m_smallT->Branch("dauSFs_tauid_2d_systuncorrdmeras_DM11_up", &m_dauSFs_tauid_2d_systuncorrdmeras_DM11_up,
        "dauSFs_tauid_2d_systuncorrdmeras_DM11_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_systuncorrdmeras_DM11_down", &m_dauSFs_tauid_2d_systuncorrdmeras_DM11_down,
        "dauSFs_tauid_2d_systuncorrdmeras_DM11_down/F");

      m_smallT->Branch("dauSFs_tauid_2d_stat0gt140_up", &m_dauSFs_tauid_2d_stat0gt140_up,
        "dauSFs_tauid_2d_stat0gt140_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_stat0gt140_down", &m_dauSFs_tauid_2d_stat0gt140_down,
        "dauSFs_tauid_2d_stat0gt140_down/F");
      m_smallT->Branch("dauSFs_tauid_2d_stat1gt140_up", &m_dauSFs_tauid_2d_stat1gt140_up,
        "dauSFs_tauid_2d_stat1gt140_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_stat1gt140_down", &m_dauSFs_tauid_2d_stat1gt140_down,
        "dauSFs_tauid_2d_stat1gt140_down/F");

      m_smallT->Branch("dauSFs_tauid_2d_systcorrdmeras_up", &m_dauSFs_tauid_2d_systcorrdmeras_up,
        "dauSFs_tauid_2d_systcorrdmeras_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_systcorrdmeras_down", &m_dauSFs_tauid_2d_systcorrdmeras_down,
        "dauSFs_tauid_2d_systcorrdmeras_down/F");
      m_smallT->Branch("dauSFs_tauid_2d_systcorrdmuncorreras_up", &m_dauSFs_tauid_2d_systcorrdmuncorreras_up,
        "dauSFs_tauid_2d_systcorrdmuncorreras_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_systcorrdmuncorreras_down", &m_dauSFs_tauid_2d_systcorrdmuncorreras_down,
        "dauSFs_tauid_2d_systcorrdmuncorreras_down/F");
      m_smallT->Branch("dauSFs_tauid_2d_systcorrerasgt140_up", &m_dauSFs_tauid_2d_systcorrerasgt140_up,
        "dauSFs_tauid_2d_systcorrerasgt140_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_systcorrerasgt140_down", &m_dauSFs_tauid_2d_systcorrerasgt140_down,
        "dauSFs_tauid_2d_systcorrerasgt140_down/F");
      m_smallT->Branch("dauSFs_tauid_2d_extrapgt140_up", &m_dauSFs_tauid_2d_extrapgt140_up,
        "dauSFs_tauid_2d_extrapgt140_up/F");
      m_smallT->Branch("dauSFs_tauid_2d_extrapgt140_down", &m_dauSFs_tauid_2d_extrapgt140_down,
        "dauSFs_tauid_2d_extrapgt140_down/F");
  
      m_smallT->Branch ("dauSFs_mutauFR_etaLt0p4_up", &m_dauSFs_mutauFR_etaLt0p4_up, "dauSFs_mutauFR_etaLt0p4_up/F");
      m_smallT->Branch ("dauSFs_mutauFR_eta0p4to0p8_up", &m_dauSFs_mutauFR_eta0p4to0p8_up, "dauSFs_mutauFR_eta0p4to0p8_up/F");
      m_smallT->Branch ("dauSFs_mutauFR_eta0p8to1p2_up", &m_dauSFs_mutauFR_eta0p8to1p2_up, "dauSFs_mutauFR_eta0p8to1p2_up/F");
      m_smallT->Branch ("dauSFs_mutauFR_eta1p2to1p7_up", &m_dauSFs_mutauFR_eta1p2to1p7_up, "dauSFs_mutauFR_eta1p2to1p7_up/F");
      m_smallT->Branch ("dauSFs_mutauFR_etaGt1p7_up", &m_dauSFs_mutauFR_etaGt1p7_up, "dauSFs_mutauFR_etaGt1p7_up/F");
      m_smallT->Branch ("dauSFs_mutauFR_etaLt0p4_down", &m_dauSFs_mutauFR_etaLt0p4_down, "dauSFs_mutauFR_etaLt0p4_down/F");
      m_smallT->Branch ("dauSFs_mutauFR_eta0p4to0p8_down", &m_dauSFs_mutauFR_eta0p4to0p8_down, "dauSFs_mutauFR_eta0p4to0p8_down/F");
      m_smallT->Branch ("dauSFs_mutauFR_eta0p8to1p2_down", &m_dauSFs_mutauFR_eta0p8to1p2_down, "dauSFs_mutauFR_eta0p8to1p2_down/F");
      m_smallT->Branch ("dauSFs_mutauFR_eta1p2to1p7_down", &m_dauSFs_mutauFR_eta1p2to1p7_down, "dauSFs_mutauFR_eta1p2to1p7_down/F");
      m_smallT->Branch ("dauSFs_mutauFR_etaGt1p7_down", &m_dauSFs_mutauFR_etaGt1p7_down, "dauSFs_mutauFR_etaGt1p7_down/F");

      m_smallT->Branch ("dauSFs_etauFR_barrel_up", &m_dauSFs_etauFR_barrel_up, "dauSFs_etauFR_barrel_up/F");
      m_smallT->Branch ("dauSFs_etauFR_endcap_up", &m_dauSFs_etauFR_endcap_up, "dauSFs_etauFR_endcap_up/F");
      m_smallT->Branch ("dauSFs_etauFR_barrel_down", &m_dauSFs_etauFR_barrel_down, "dauSFs_etauFR_barrel_down/F");
      m_smallT->Branch ("dauSFs_etauFR_endcap_down", &m_dauSFs_etauFR_endcap_down, "dauSFs_etauFR_endcap_down/F");

      m_smallT->Branch ("dauSFs_muID_up", &m_dauSFs_muID_up   , "dauSFs_muID_up/F");
      m_smallT->Branch ("dauSFs_muID_down", &m_dauSFs_muID_down , "dauSFs_muID_down/F");
      m_smallT->Branch ("dauSFs_muIso_up", &m_dauSFs_muIso_up  , "dauSFs_muIso_up/F");
      m_smallT->Branch ("dauSFs_muIso_down", &m_dauSFs_muIso_down, "dauSFs_muIso_down/F");
      m_smallT->Branch ("dauSFs_eleID_up", &m_dauSFs_eleID_up  , "dauSFs_eleID_up/F");
      m_smallT->Branch ("dauSFs_eleID_down", &m_dauSFs_eleID_down, "dauSFs_eleID_down/F");
      m_smallT->Branch ("dauSFs_eleReco_up", &m_dauSFs_eleReco_up, "dauSFs_eleReco_up/F");
      m_smallT->Branch ("dauSFs_eleReco_down", &m_dauSFs_eleReco_down, "dauSFs_eleReco_down/F");

      m_smallT->Branch ("lheNOutPartons", &m_lheNOutPartons, "lheNOutPartons/I");
      m_smallT->Branch ("lheNOutB", &m_lheNOutB, "lheNOutB/I");
      m_smallT->Branch ("EventNumber", &m_EventNumber, "EventNumber/l") ;
      m_smallT->Branch ("RunNumber", &m_RunNumber, "RunNumber/I") ;
      m_smallT->Branch ("isBoosted", &m_isBoosted, "isBoosted/I") ;

      m_smallT->Branch ("isLeptrigger", &m_isLeptrigger, "isLeptrigger/I") ;
      m_smallT->Branch ("isMETtrigger", &m_isMETtrigger, "isMETtrigger/I") ;
      m_smallT->Branch ("isSingleTautrigger", &m_isSingleTautrigger, "isSingleTautrigger/I") ;

      m_smallT->Branch("genDecMode1", &m_genDecMode1, "genDecMode1/I");
      m_smallT->Branch("genDecMode2", &m_genDecMode2, "genDecMode2/I");

      m_smallT->Branch("genMHH", &m_genMHH, "m_genMHH/F");
      m_smallT->Branch("genCosth", &m_genCosth, "m_genCosth/F");

      m_smallT->Branch ("npv", &m_npv, "npv/I") ;
      m_smallT->Branch ("npu", &m_npu, "npu/F") ;
      m_smallT->Branch ("lumi", &m_lumi, "lumi/I") ;

      m_smallT->Branch ("triggerbit", &m_triggerbit, "triggerbit/L") ;
      m_smallT->Branch ("pass_triggerbit", &m_pass_triggerbit, "pass_triggerbit/I") ;
	  m_smallT->Branch ("legacyAccept", &m_legacyAccept, "legacyAccept/I") ;
	  m_smallT->Branch ("metAccept", &m_metAccept, "metAccept/I") ;
	  m_smallT->Branch ("singletauAccept", &m_singletauAccept, "singletauAccept/I") ;
      m_smallT->Branch ("cross_monitoring_trig", &m_cross_monitoring_trig, "cross_monitoring_trig/O") ;
      m_smallT->Branch ("rho", &m_rho, "rho/F") ;

      m_smallT->Branch ("pairType", &m_pairType, "pairType/I") ;
      m_smallT->Branch ("isMC", &m_isMC, "isMC/I") ;
      m_smallT->Branch ("isOS", &m_isOS, "isOS/I") ;

      m_smallT->Branch ("met_phi", &m_met_phi, "met_phi/F") ;
      m_smallT->Branch ("met_et", &m_met_et, "met_et/F") ;
      m_smallT->Branch ("METx", &m_METx, "METx/F") ;
      m_smallT->Branch ("METy", &m_METy, "METy/F") ;

      m_smallT->Branch ("metnomu_phi", &m_metnomu_phi,  "metnomu_phi/F");
      m_smallT->Branch ("metnomu_et",  &m_metnomu_et,   "metnomu_et/F" );
      m_smallT->Branch ("METnoMux",    &m_METnoMux,     "METnoMux/F"   );
      m_smallT->Branch ("METnoMuy",    &m_METnoMuy,     "METnoMuy/F"   );
      m_smallT->Branch ("mht_phi",     &m_mht_phi,      "mht_phi/F"    );
      m_smallT->Branch ("mht_et",      &m_mht_et,       "mht_et/F"     );
      m_smallT->Branch ("MHTx",        &m_MHTx,         "MHTx/F"       );
      m_smallT->Branch ("MHTy",        &m_MHTy,         "MHTy/F"       );
      m_smallT->Branch ("mhtnomu_phi", &m_mhtnomu_phi,  "mhtnomu_phi/F");
      m_smallT->Branch ("mhtnomu_et",  &m_mhtnomu_et,   "mhtnomu_et/F" );
      m_smallT->Branch ("MHTnoMux",    &m_MHTnoMux,     "MHTnoMux/F"   );
      m_smallT->Branch ("MHTnoMuy",    &m_MHTnoMuy,     "MHTnoMuy/F"   );

	  m_smallT->Branch ("METx_jetup1",  &m_METx_jetup1,  "METx_jetup1/F");
	  m_smallT->Branch ("METx_jetup2",  &m_METx_jetup2,  "METx_jetup2/F");
	  m_smallT->Branch ("METx_jetup3",  &m_METx_jetup3,  "METx_jetup3/F");
	  m_smallT->Branch ("METx_jetup4",  &m_METx_jetup4,  "METx_jetup4/F");
	  m_smallT->Branch ("METx_jetup5",  &m_METx_jetup5,  "METx_jetup5/F");
	  m_smallT->Branch ("METx_jetup6",  &m_METx_jetup6,  "METx_jetup6/F");
	  m_smallT->Branch ("METx_jetup7",  &m_METx_jetup7,  "METx_jetup7/F");
	  m_smallT->Branch ("METx_jetup8",  &m_METx_jetup8,  "METx_jetup8/F");
	  m_smallT->Branch ("METx_jetup9",  &m_METx_jetup9,  "METx_jetup9/F");
	  m_smallT->Branch ("METx_jetup10", &m_METx_jetup10, "METx_jetup10/F");
	  m_smallT->Branch ("METx_jetup11", &m_METx_jetup11, "METx_jetup11/F");

	  m_smallT->Branch ("METy_jetup1",  &m_METy_jetup1,  "METy_jetup1/F");
	  m_smallT->Branch ("METy_jetup2",  &m_METy_jetup2,  "METy_jetup2/F");
	  m_smallT->Branch ("METy_jetup3",  &m_METy_jetup3,  "METy_jetup3/F");
	  m_smallT->Branch ("METy_jetup4",  &m_METy_jetup4,  "METy_jetup4/F");
	  m_smallT->Branch ("METy_jetup5",  &m_METy_jetup5,  "METy_jetup5/F");
	  m_smallT->Branch ("METy_jetup6",  &m_METy_jetup6,  "METy_jetup6/F");
	  m_smallT->Branch ("METy_jetup7",  &m_METy_jetup7,  "METy_jetup7/F");
	  m_smallT->Branch ("METy_jetup8",  &m_METy_jetup8,  "METy_jetup8/F");
	  m_smallT->Branch ("METy_jetup9",  &m_METy_jetup9,  "METy_jetup9/F");
	  m_smallT->Branch ("METy_jetup10", &m_METy_jetup10, "METy_jetup10/F");
	  m_smallT->Branch ("METy_jetup11", &m_METy_jetup11, "METy_jetup11/F");

	  m_smallT->Branch ("METx_jetdown1",  &m_METx_jetdown1,  "METx_jetdown1/F");
	  m_smallT->Branch ("METx_jetdown2",  &m_METx_jetdown2,  "METx_jetdown2/F");
	  m_smallT->Branch ("METx_jetdown3",  &m_METx_jetdown3,  "METx_jetdown3/F");
	  m_smallT->Branch ("METx_jetdown4",  &m_METx_jetdown4,  "METx_jetdown4/F");
	  m_smallT->Branch ("METx_jetdown5",  &m_METx_jetdown5,  "METx_jetdown5/F");
	  m_smallT->Branch ("METx_jetdown6",  &m_METx_jetdown6,  "METx_jetdown6/F");
	  m_smallT->Branch ("METx_jetdown7",  &m_METx_jetdown7,  "METx_jetdown7/F");
	  m_smallT->Branch ("METx_jetdown8",  &m_METx_jetdown8,  "METx_jetdown8/F");
	  m_smallT->Branch ("METx_jetdown9",  &m_METx_jetdown9,  "METx_jetdown9/F");
	  m_smallT->Branch ("METx_jetdown10", &m_METx_jetdown10, "METx_jetdown10/F");
	  m_smallT->Branch ("METx_jetdown11", &m_METx_jetdown11, "METx_jetdown11/F");

	  m_smallT->Branch ("METy_jetdown1",  &m_METy_jetdown1,  "METy_jetdown1/F");
	  m_smallT->Branch ("METy_jetdown2",  &m_METy_jetdown2,  "METy_jetdown2/F");
	  m_smallT->Branch ("METy_jetdown3",  &m_METy_jetdown3,  "METy_jetdown3/F");
	  m_smallT->Branch ("METy_jetdown4",  &m_METy_jetdown4,  "METy_jetdown4/F");
	  m_smallT->Branch ("METy_jetdown5",  &m_METy_jetdown5,  "METy_jetdown5/F");
	  m_smallT->Branch ("METy_jetdown6",  &m_METy_jetdown6,  "METy_jetdown6/F");
	  m_smallT->Branch ("METy_jetdown7",  &m_METy_jetdown7,  "METy_jetdown7/F");
	  m_smallT->Branch ("METy_jetdown8",  &m_METy_jetdown8,  "METy_jetdown8/F");
	  m_smallT->Branch ("METy_jetdown9",  &m_METy_jetdown9,  "METy_jetdown9/F");
	  m_smallT->Branch ("METy_jetdown10", &m_METy_jetdown10, "METy_jetdown10/F");
	  m_smallT->Branch ("METy_jetdown11", &m_METy_jetdown11, "METy_jetdown11/F");

	  m_smallT->Branch ("METx_tauup_DM0",    &m_METx_tauup_DM0,	   "METx_tauup_DM0/F");
	  m_smallT->Branch ("METx_tauup_DM1",	 &m_METx_tauup_DM1,	   "METx_tauup_DM1/F");
	  m_smallT->Branch ("METx_tauup_DM10",	 &m_METx_tauup_DM10,   "METx_tauup_DM10/F");
	  m_smallT->Branch ("METx_tauup_DM11",	 &m_METx_tauup_DM11,   "METx_tauup_DM11/F");
	  m_smallT->Branch ("METy_tauup_DM0",	 &m_METy_tauup_DM0,	   "METy_tauup_DM0/F");
	  m_smallT->Branch ("METy_tauup_DM1",	 &m_METy_tauup_DM1,	   "METy_tauup_DM1/F");
	  m_smallT->Branch ("METy_tauup_DM10",	 &m_METy_tauup_DM10,   "METy_tauup_DM10/F");
	  m_smallT->Branch ("METy_tauup_DM11",	 &m_METy_tauup_DM11,   "METy_tauup_DM11/F");
	  m_smallT->Branch ("METx_taudown_DM0",	 &m_METx_taudown_DM0,  "METx_taudown_DM0/F");
	  m_smallT->Branch ("METx_taudown_DM1",	 &m_METx_taudown_DM1,  "METx_taudown_DM1/F");
	  m_smallT->Branch ("METx_taudown_DM10", &m_METx_taudown_DM10, "METx_taudown_DM10/F");
	  m_smallT->Branch ("METx_taudown_DM11", &m_METx_taudown_DM11, "METx_taudown_DM11/F");
	  m_smallT->Branch ("METy_taudown_DM0",	 &m_METy_taudown_DM0,  "METy_taudown_DM0/F");
	  m_smallT->Branch ("METy_taudown_DM1",	 &m_METy_taudown_DM1,  "METy_taudown_DM1/F");
	  m_smallT->Branch ("METy_taudown_DM10", &m_METy_taudown_DM10, "METy_taudown_DM10/F");
	  m_smallT->Branch ("METy_taudown_DM11", &m_METy_taudown_DM11, "METy_taudown_DM11/F");

	  m_smallT->Branch ("METx_eleup_DM0",   &m_METx_eleup_DM0,   "METx_eleup_DM0/F");
	  m_smallT->Branch ("METx_eleup_DM1",   &m_METx_eleup_DM1,   "METx_eleup_DM1/F");
	  m_smallT->Branch ("METy_eleup_DM0",   &m_METy_eleup_DM0,   "METy_eleup_DM0/F");
	  m_smallT->Branch ("METy_eleup_DM1",   &m_METy_eleup_DM1,   "METy_eleup_DM1/F");
	  m_smallT->Branch ("METx_eledown_DM0", &m_METx_eledown_DM0, "METx_eledown_DM0/F");
	  m_smallT->Branch ("METx_eledown_DM1", &m_METx_eledown_DM1, "METx_eledown_DM1/F");
	  m_smallT->Branch ("METy_eledown_DM0", &m_METy_eledown_DM0, "METy_eledown_DM0/F");
	  m_smallT->Branch ("METy_eledown_DM1", &m_METy_eledown_DM1, "METy_eledown_DM1/F");

	  m_smallT->Branch ("METx_eesUp",   &m_METx_eesUp,   "METx_eesUp/F");
	  m_smallT->Branch ("METx_eesDown", &m_METx_eesDown, "METx_eesDown/F");
	  m_smallT->Branch ("METy_eesUp",   &m_METy_eesUp,   "METy_eesUp/F");
	  m_smallT->Branch ("METy_eesDown", &m_METy_eesDown, "METy_eesDown/F");
	  m_smallT->Branch ("METx_eerUp",   &m_METx_eerUp,   "METx_eerUp/F");
	  m_smallT->Branch ("METx_eerDown", &m_METx_eerDown, "METx_eerDown/F");
	  m_smallT->Branch ("METy_eerUp",   &m_METy_eerUp,   "METy_eerUp/F");
	  m_smallT->Branch ("METy_eerDown", &m_METy_eerDown, "METy_eerDown/F");

      m_smallT->Branch ("METx_muup",   &m_METx_muup,   "METx_muup/F");
      m_smallT->Branch ("METy_muup",   &m_METy_muup,   "METy_muup/F");
      m_smallT->Branch ("METx_mudown", &m_METx_mudown, "METx_mudown/F");
	  m_smallT->Branch ("METy_mudown", &m_METy_mudown, "METy_mudown/F");
			
	  m_smallT->Branch ("METx_jetupTot", &m_METx_jetupTot, "METx_jetupTot/F");
      m_smallT->Branch ("METy_jetupTot", &m_METy_jetupTot, "METy_jetupTot/F");
	  m_smallT->Branch ("METx_jetdownTot", &m_METx_jetdownTot, "METx_jetdownTot/F") ;
      m_smallT->Branch ("METy_jetdownTot", &m_METy_jetdownTot, "METy_jetdownTot/F") ;

      m_smallT->Branch ("met_et_corr", &m_met_et_corr, "met_et_corr/F");
      m_smallT->Branch ("met_cov00",   &m_met_cov00,   "met_cov00/F");
      m_smallT->Branch ("met_cov01",   &m_met_cov01,   "met_cov01/F");
      m_smallT->Branch ("met_cov10",   &m_met_cov10,   "met_cov10/F");
      m_smallT->Branch ("met_cov11",   &m_met_cov11,   "met_cov11/F");

      m_smallT->Branch("DeepMET_ResponseTune_px",   &m_DeepMET_ResponseTune_px,   "DeepMET_ResponseTune_px/F");
      m_smallT->Branch("DeepMET_ResponseTune_py",   &m_DeepMET_ResponseTune_py,   "DeepMET_ResponseTune_py/F");
      m_smallT->Branch("DeepMET_ResolutionTune_px", &m_DeepMET_ResolutionTune_px, "DeepMET_ResolutionTune_px");
      m_smallT->Branch("DeepMET_ResolutionTune_py", &m_DeepMET_ResolutionTune_py, "DeepMET_ResolutionTune_py");

      m_smallT->Branch ("dau1_iso", &m_dau1_iso, "dau1_iso/F") ;
      m_smallT->Branch ("dau1_eleMVAiso", &m_dau1_eleMVAiso, "dau1_eleMVAiso/I") ;
      m_smallT->Branch ("dau1_deepTauVsJet",&m_dau1_deepTauVsJet,"dau1_deepTauVsJet/I");
      m_smallT->Branch ("dau1_deepTauVsEle",&m_dau1_deepTauVsEle,"dau1_deepTauVsEle/I");
      m_smallT->Branch ("dau1_deepTauVsMu",&m_dau1_deepTauVsMu,"dau1_deepTauVsMu/I");

      m_smallT->Branch ("dau1_pt", &m_dau1_pt, "dau1_pt/F") ;
	  m_smallT->Branch ("dau1_pt_muup",    &m_dau1_pt_muup,   "dau1_pt_muup/F");
      m_smallT->Branch ("dau1_pt_mudown",  &m_dau1_pt_mudown, "dau1_pt_mudown/F");
      m_smallT->Branch ("dau1_e_muup",     &m_dau1_e_muup,    "dau1_e_muup/F");
      m_smallT->Branch ("dau1_e_mudown",   &m_dau1_e_mudown,  "dau1_pt_mudown/F");

      m_smallT->Branch ("dau1_pt_tauup_DM0" , &m_dau1_pt_tauup_DM0,  "dau1_pt_tauup_DM0/F");
      m_smallT->Branch ("dau1_pt_tauup_DM1" , &m_dau1_pt_tauup_DM1,  "dau1_pt_tauup_DM1/F");
      m_smallT->Branch ("dau1_pt_tauup_DM10", &m_dau1_pt_tauup_DM10, "dau1_pt_tauup_DM10/F");
      m_smallT->Branch ("dau1_pt_tauup_DM11", &m_dau1_pt_tauup_DM11, "dau1_pt_tauup_DM11/F");
      m_smallT->Branch ("dau1_e_tauup_DM0" ,  &m_dau1_e_tauup_DM0,   "dau1_e_tauup_DM0/F");
      m_smallT->Branch ("dau1_e_tauup_DM1" ,  &m_dau1_e_tauup_DM1 ,  "dau1_e_tauup_DM1/F");
      m_smallT->Branch ("dau1_e_tauup_DM10",  &m_dau1_e_tauup_DM10,  "dau1_e_tauup_DM10/F");
      m_smallT->Branch ("dau1_e_tauup_DM11",  &m_dau1_e_tauup_DM11,  "dau1_e_tauup_DM11/F");

      m_smallT->Branch ("dau1_pt_taudown_DM0" , &m_dau1_pt_taudown_DM0,  "dau1_pt_taudown_DM0/F");
      m_smallT->Branch ("dau1_pt_taudown_DM1" , &m_dau1_pt_taudown_DM1,  "dau1_pt_taudown_DM1/F");
      m_smallT->Branch ("dau1_pt_taudown_DM10", &m_dau1_pt_taudown_DM10, "dau1_pt_taudown_DM10/F");
      m_smallT->Branch ("dau1_pt_taudown_DM11", &m_dau1_pt_taudown_DM11, "dau1_pt_taudown_DM11/F");
      m_smallT->Branch ("dau1_e_taudown_DM0" ,  &m_dau1_e_taudown_DM0,   "dau1_e_taudown_DM0/F");
      m_smallT->Branch ("dau1_e_taudown_DM1" ,  &m_dau1_e_taudown_DM1,   "dau1_e_taudown_DM1/F");
      m_smallT->Branch ("dau1_e_taudown_DM10",  &m_dau1_e_taudown_DM10,  "dau1_e_taudown_DM10/F");
      m_smallT->Branch ("dau1_e_taudown_DM11",  &m_dau1_e_taudown_DM11,  "dau1_e_taudown_DM11/F");

      m_smallT->Branch ("dau1_pt_eleup_DM0", &m_dau1_pt_eleup_DM0, "dau1_pt_eleup_DM0/F");
      m_smallT->Branch ("dau1_pt_eleup_DM1", &m_dau1_pt_eleup_DM1, "dau1_pt_eleup_DM1/F");
      m_smallT->Branch ("dau1_e_eleup_DM0",  &m_dau1_e_eleup_DM0,  "dau1_e_eleup_DM0/F");
      m_smallT->Branch ("dau1_e_eleup_DM1",  &m_dau1_e_eleup_DM1,  "dau1_e_eleup_DM1/F");

      m_smallT->Branch ("dau1_pt_eledown_DM0", &m_dau1_pt_eledown_DM0, "dau1_pt_eledown_DM0/F");
      m_smallT->Branch ("dau1_pt_eledown_DM1", &m_dau1_pt_eledown_DM1, "dau1_pt_eledown_DM1/F");
      m_smallT->Branch ("dau1_e_eledown_DM0",  &m_dau1_e_eledown_DM0,  "dau1_e_eledown_DM0/F");
      m_smallT->Branch ("dau1_e_eledown_DM1",  &m_dau1_e_eledown_DM1,  "dau1_e_eledown_DM1/F");

	  m_smallT->Branch ("dau1_pt_eesUp",   &m_dau1_pt_eesUp,   "dau1_pt_eesUp/F");
	  m_smallT->Branch ("dau1_pt_eesDown", &m_dau1_pt_eesDown, "dau1_pt_eesDown/F");
	  m_smallT->Branch ("dau1_pt_eerUp",   &m_dau1_pt_eerUp,   "dau1_pt_eerUp/F");
	  m_smallT->Branch ("dau1_pt_eerDown", &m_dau1_pt_eerDown, "dau1_pt_eerDown/F");
	  m_smallT->Branch ("dau1_e_eesUp",   &m_dau1_e_eesUp,   "dau1_e_eesUp/F");
	  m_smallT->Branch ("dau1_e_eesDown", &m_dau1_e_eesDown, "dau1_e_eesDown/F");
	  m_smallT->Branch ("dau1_e_eerUp",   &m_dau1_e_eerUp,   "dau1_e_eerUp/F");
	  m_smallT->Branch ("dau1_e_eerDown", &m_dau1_e_eerDown, "dau1_e_eerDown/F");

      m_smallT->Branch ("dau1_eta",       &m_dau1_eta,       "dau1_eta/F");
      m_smallT->Branch ("dau1_phi",       &m_dau1_phi,       "dau1_phi/F");
      m_smallT->Branch ("dau1_e",         &m_dau1_e,         "dau1_e/F");
      m_smallT->Branch ("dau1_flav",      &m_dau1_flav,      "dau1_flav/F");
      m_smallT->Branch ("dau1_dxy",       &m_dau1_dxy,       "dau1_dxy/F");
      m_smallT->Branch ("dau1_dz",        &m_dau1_dz,        "dau1_dz/F");
      m_smallT->Branch ("dau1_decayMode", &m_dau1_decayMode, "dau1_decayMode/I");

      m_smallT->Branch("genmatched1_pt", &m_genmatched1_pt, "genmatched1_pt/F");
      m_smallT->Branch("genmatched1_eta", &m_genmatched1_eta, "genmatched1_eta/F");
      m_smallT->Branch("genmatched1_phi", &m_genmatched1_phi, "genmatched1_phi/F");
      m_smallT->Branch("genmatched1_e", &m_genmatched1_e, "genmatched1_e/F");
      m_smallT->Branch("genmatched2_pt", &m_genmatched2_pt, "genmatched2_pt/F");
      m_smallT->Branch("genmatched2_eta", &m_genmatched2_eta, "genmatched2_eta/F");
      m_smallT->Branch("genmatched2_phi", &m_genmatched2_phi, "genmatched2_phi/F");
      m_smallT->Branch("genmatched2_e", &m_genmatched2_e, "genmatched2_e/F");

      m_smallT->Branch("hasgenmatch1", &m_hasgenmatch1, "hasgenmatch1/O");
      m_smallT->Branch("hasgenmatch2", &m_hasgenmatch2, "hasgenmatch2/O");

      m_smallT->Branch ("dau2_iso", &m_dau2_iso, "dau2_iso/F") ;
      m_smallT->Branch ("dau2_deepTauVsJet",&m_dau2_deepTauVsJet,"dau2_deepTauVsJet/I");
      m_smallT->Branch ("dau2_deepTauVsEle",&m_dau2_deepTauVsEle,"dau2_deepTauVsEle/I");
      m_smallT->Branch ("dau2_deepTauVsMu",&m_dau2_deepTauVsMu,"dau2_deepTauVsMu/I");

      m_smallT->Branch("genH1_pt", &m_genH1_pt  , "genH1_pt/F");
      m_smallT->Branch("genH1_eta", &m_genH1_eta  , "genH1_eta/F");
      m_smallT->Branch("genH1_phi", &m_genH1_phi  , "genH1_phi/F");
      m_smallT->Branch("genH1_e", &m_genH1_e  , "genH1_e/F");
      m_smallT->Branch("genH2_pt", &m_genH2_pt  , "genH2_pt/F");
      m_smallT->Branch("genH2_eta", &m_genH2_eta  , "genH2_eta/F");
      m_smallT->Branch("genH2_phi", &m_genH2_phi  , "genH2_phi/F");
      m_smallT->Branch("genH2_e", &m_genH2_e  , "genH2_e/F");

      m_smallT->Branch("genB1_pt", &m_genB1_pt  , "genB1_pt/F");
      m_smallT->Branch("genB1_eta", &m_genB1_eta  , "genB1_eta/F");
      m_smallT->Branch("genB1_phi", &m_genB1_phi  , "genB1_phi/F");
      m_smallT->Branch("genB1_e", &m_genB1_e  , "genB1_e/F");
      m_smallT->Branch("genB2_pt", &m_genB2_pt  , "genB2_pt/F");
      m_smallT->Branch("genB2_eta", &m_genB2_eta  , "genB2_eta/F");
      m_smallT->Branch("genB2_phi", &m_genB2_phi  , "genB2_phi/F");
      m_smallT->Branch("genB2_e", &m_genB2_e  , "genB2_e/F");

      m_smallT->Branch("genBQuark1_pt", &m_genBQuark1_pt  , "genBQuark1_pt/F");
      m_smallT->Branch("genBQuark1_eta", &m_genBQuark1_eta  , "genBQuark1_eta/F");
      m_smallT->Branch("genBQuark1_phi", &m_genBQuark1_phi  , "genBQuark1_phi/F");
      m_smallT->Branch("genBQuark1_e", &m_genBQuark1_e  , "genBQuark1_e/F");
      m_smallT->Branch("genBQuark1_motherPdgId", &m_genBQuark1_motherPdgId  , "genBQuark1_motherPdgId/I");
      m_smallT->Branch("genBQuark2_pt", &m_genBQuark2_pt  , "genBQuark2_pt/F");
      m_smallT->Branch("genBQuark2_eta", &m_genBQuark2_eta  , "genBQuark2_eta/F");
      m_smallT->Branch("genBQuark2_phi", &m_genBQuark2_phi  , "genBQuark2_phi/F");
      m_smallT->Branch("genBQuark2_e", &m_genBQuark2_e  , "genBQuark2_e/F");
      m_smallT->Branch("genBQuark2_motherPdgId", &m_genBQuark2_motherPdgId  , "genBQuark2_motherPdgId/I");
      m_smallT->Branch("genBQuark3_pt", &m_genBQuark3_pt  , "genBQuark3_pt/F");
      m_smallT->Branch("genBQuark3_eta", &m_genBQuark3_eta  , "genBQuark3_eta/F");
      m_smallT->Branch("genBQuark3_phi", &m_genBQuark3_phi  , "genBQuark3_phi/F");
      m_smallT->Branch("genBQuark3_e", &m_genBQuark3_e  , "genBQuark3_e/F");
      m_smallT->Branch("genBQuark3_motherPdgId", &m_genBQuark3_motherPdgId  , "genBQuark3_motherPdgId/I");
      m_smallT->Branch("genBQuark4_pt", &m_genBQuark4_pt  , "genBQuark4_pt/F");
      m_smallT->Branch("genBQuark4_eta", &m_genBQuark4_eta  , "genBQuark4_eta/F");
      m_smallT->Branch("genBQuark4_phi", &m_genBQuark4_phi  , "genBQuark4_phi/F");
      m_smallT->Branch("genBQuark4_e", &m_genBQuark4_e  , "genBQuark4_e/F");
      m_smallT->Branch("genBQuark4_motherPdgId", &m_genBQuark4_motherPdgId  , "genBQuark4_motherPdgId/I");

      m_smallT->Branch("genLepton1_pt", &m_genLepton1_pt  , "genLepton1_pt/F");
      m_smallT->Branch("genLepton1_eta", &m_genLepton1_eta  , "genLepton1_eta/F");
      m_smallT->Branch("genLepton1_phi", &m_genLepton1_phi  , "genLepton1_phi/F");
      m_smallT->Branch("genLepton1_e", &m_genLepton1_e  , "genLepton1_e/F");
      m_smallT->Branch("genLepton1_pdgId", &m_genLepton1_pdgId  , "genLepton1_pdgId/I");
      m_smallT->Branch("genLepton1_motherPdgId", &m_genLepton1_motherPdgId  , "genLepton1_motherPdgId/I");
      m_smallT->Branch("genLepton2_pt", &m_genLepton2_pt  , "genLepton2_pt/F");
      m_smallT->Branch("genLepton2_eta", &m_genLepton2_eta  , "genLepton2_eta/F");
      m_smallT->Branch("genLepton2_phi", &m_genLepton2_phi  , "genLepton2_phi/F");
      m_smallT->Branch("genLepton2_e", &m_genLepton2_e  , "genLepton2_e/F");
      m_smallT->Branch("genLepton2_pdgId", &m_genLepton2_pdgId  , "genLepton2_pdgId/I");
      m_smallT->Branch("genLepton2_motherPdgId", &m_genLepton2_motherPdgId  , "genLepton2_motherPdgId/I");
      m_smallT->Branch("genLepton3_pt", &m_genLepton3_pt  , "genLepton3_pt/F");
      m_smallT->Branch("genLepton3_eta", &m_genLepton3_eta  , "genLepton3_eta/F");
      m_smallT->Branch("genLepton3_phi", &m_genLepton3_phi  , "genLepton3_phi/F");
      m_smallT->Branch("genLepton3_e", &m_genLepton3_e  , "genLepton3_e/F");
      m_smallT->Branch("genLepton3_pdgId", &m_genLepton3_pdgId  , "genLepton3_pdgId/I");
      m_smallT->Branch("genLepton3_motherPdgId", &m_genLepton3_motherPdgId  , "genLepton3_motherPdgId/I");
      m_smallT->Branch("genLepton4_pt", &m_genLepton4_pt  , "genLepton4_pt/F");
      m_smallT->Branch("genLepton4_eta", &m_genLepton4_eta  , "genLepton4_eta/F");
      m_smallT->Branch("genLepton4_phi", &m_genLepton4_phi  , "genLepton4_phi/F");
      m_smallT->Branch("genLepton4_e", &m_genLepton4_e  , "genLepton4_e/F");
      m_smallT->Branch("genLepton4_pdgId", &m_genLepton4_pdgId  , "genLepton4_pdgId/I");
      m_smallT->Branch("genLepton4_motherPdgId", &m_genLepton4_motherPdgId  , "genLepton4_motherPdgId/I");

      m_smallT->Branch("genNu1_pt", &m_genNu1_pt  , "genNu1_pt/F");
      m_smallT->Branch("genNu1_eta", &m_genNu1_eta , "genNu1_eta/F");
      m_smallT->Branch("genNu1_phi", &m_genNu1_phi , "genNu1_phi/F");
      m_smallT->Branch("genNu1_e", &m_genNu1_e , "genNu1_e/F");
      m_smallT->Branch("genNu2_pt", &m_genNu2_pt  , "genNu2_pt/F");
      m_smallT->Branch("genNu2_eta", &m_genNu2_eta , "genNu2_eta/F");
      m_smallT->Branch("genNu2_phi", &m_genNu2_phi , "genNu2_phi/F");
      m_smallT->Branch("genNu2_e", &m_genNu2_e , "genNu2_e/F");
      m_smallT->Branch("genNuNoMatch_pt", &m_genNuNoMatch_pt  , "genNuNoMatch_pt/F");
      m_smallT->Branch("genNuNoMatch_eta", &m_genNuNoMatch_eta , "genNuNoMatch_eta/F");
      m_smallT->Branch("genNuNoMatch_phi", &m_genNuNoMatch_phi , "genNuNoMatch_phi/F");
      m_smallT->Branch("genNuNoMatch_e", &m_genNuNoMatch_e , "genNuNoMatch_e/F");
      m_smallT->Branch("genNuTot_pt", &m_genNuTot_pt  , "genNuTot_pt/F");
      m_smallT->Branch("genNuTot_eta", &m_genNuTot_eta , "genNuTot_eta/F");
      m_smallT->Branch("genNuTot_phi", &m_genNuTot_phi , "genNuTot_phi/F");
      m_smallT->Branch("genNuTot_e", &m_genNuTot_e , "genNuTot_e/F");
      m_smallT->Branch("genNuTotWithNoMatch_pt", &m_genNuTotWithNoMatch_pt  , "genNuTotWithNoMatch_pt/F");
      m_smallT->Branch("genNuTotWithNoMatch_eta", &m_genNuTotWithNoMatch_eta , "genNuTotWithNoMatch_eta/F");
      m_smallT->Branch("genNuTotWithNoMatch_phi", &m_genNuTotWithNoMatch_phi , "genNuTotWithNoMatch_phi/F");
      m_smallT->Branch("genNuTotWithNoMatch_e", &m_genNuTotWithNoMatch_e , "genNuTotWithNoMatch_e/F");
      m_smallT->Branch("recoGenTauH_pt", &m_recoGenTauH_pt , "recoGenTauH_pt/F");
      m_smallT->Branch("recoGenTauH_eta", &m_recoGenTauH_eta , "recoGenTauH_eta/F");
      m_smallT->Branch("recoGenTauH_phi",&m_recoGenTauH_phi , "recoGenTauH_phi/F");
      m_smallT->Branch("recoGenTauH_e", &m_recoGenTauH_e , "recoGenTauH_e/F");
      m_smallT->Branch("recoGenTauH_mass", &m_recoGenTauH_mass , "recoGenTauH_mass/F");
      m_smallT->Branch("recoGenWithNoMatchTauH_pt", &m_recoGenWithNoMatchTauH_pt , "recoGenWithNoMatchTauH_pt/F");
      m_smallT->Branch("recoGenWithNoMatchTauH_eta", &m_recoGenWithNoMatchTauH_eta , "recoGenWithNoMatchTauH_eta/F");
      m_smallT->Branch("recoGenWithNoMatchTauH_phi",&m_recoGenWithNoMatchTauH_phi , "recoGenWithNoMatchTauH_phi/F");
      m_smallT->Branch("recoGenWithNoMatchTauH_e", &m_recoGenWithNoMatchTauH_e , "recoGenWithNoMatchTauH_e/F");
      m_smallT->Branch("recoGenWithNoMatchTauH_mass", &m_recoGenWithNoMatchTauH_mass , "recoGenWithNoMatchTauH_mass/F");

      m_smallT->Branch ("dau2_pt", &m_dau2_pt, "dau2_pt/F") ;

      m_smallT->Branch ("dau2_pt_muup",   &m_dau2_pt_muup,   "dau2_pt_muup/F");
      m_smallT->Branch ("dau2_pt_mudown", &m_dau2_pt_mudown, "dau2_pt_mudown/F");
      m_smallT->Branch ("dau2_e_muup",    &m_dau2_e_muup,    "dau2_e_muup/F");
      m_smallT->Branch ("dau2_e_mudown",  &m_dau2_e_mudown,  "dau2_pt_mudown/F");

      m_smallT->Branch ("dau2_pt_tauup_DM0" , &m_dau2_pt_tauup_DM0,  "dau2_pt_tauup_DM0/F");
      m_smallT->Branch ("dau2_pt_tauup_DM1" , &m_dau2_pt_tauup_DM1,  "dau2_pt_tauup_DM1/F");
      m_smallT->Branch ("dau2_pt_tauup_DM10", &m_dau2_pt_tauup_DM10, "dau2_pt_tauup_DM10/F");
      m_smallT->Branch ("dau2_pt_tauup_DM11", &m_dau2_pt_tauup_DM11, "dau2_pt_tauup_DM11/F");
      m_smallT->Branch ("dau2_e_tauup_DM0" ,  &m_dau2_e_tauup_DM0,   "dau2_e_tauup_DM0/F");
      m_smallT->Branch ("dau2_e_tauup_DM1" ,  &m_dau2_e_tauup_DM1,   "dau2_e_tauup_DM1/F");
      m_smallT->Branch ("dau2_e_tauup_DM10",  &m_dau2_e_tauup_DM10,  "dau2_e_tauup_DM10/F");
      m_smallT->Branch ("dau2_e_tauup_DM11",  &m_dau2_e_tauup_DM11,  "dau2_e_tauup_DM11/F");

      m_smallT->Branch ("dau2_pt_taudown_DM0" , &m_dau2_pt_taudown_DM0,  "dau2_pt_taudown_DM0/F");
      m_smallT->Branch ("dau2_pt_taudown_DM1" , &m_dau2_pt_taudown_DM1,  "dau2_pt_taudown_DM1/F");
      m_smallT->Branch ("dau2_pt_taudown_DM10", &m_dau2_pt_taudown_DM10, "dau2_pt_taudown_DM10/F");
      m_smallT->Branch ("dau2_pt_taudown_DM11", &m_dau2_pt_taudown_DM11, "dau2_pt_taudown_DM11/F");
      m_smallT->Branch ("dau2_e_taudown_DM0" ,  &m_dau2_e_taudown_DM0,   "dau2_e_taudown_DM0/F");
      m_smallT->Branch ("dau2_e_taudown_DM1" ,  &m_dau2_e_taudown_DM1,   "dau2_e_taudown_DM1/F");
      m_smallT->Branch ("dau2_e_taudown_DM10",  &m_dau2_e_taudown_DM10,  "dau2_e_taudown_DM10/F");
      m_smallT->Branch ("dau2_e_taudown_DM11",  &m_dau2_e_taudown_DM11,  "dau2_e_taudown_DM11/F");

      m_smallT->Branch ("dau2_pt_eleup_DM0", &m_dau2_pt_eleup_DM0, "dau2_pt_eleup_DM0/F");
      m_smallT->Branch ("dau2_pt_eleup_DM1", &m_dau2_pt_eleup_DM1, "dau2_pt_eleup_DM1/F");
      m_smallT->Branch ("dau2_e_eleup_DM0",  &m_dau2_e_eleup_DM0,  "dau2_e_eleup_DM0/F");
      m_smallT->Branch ("dau2_e_eleup_DM1",  &m_dau2_e_eleup_DM1,  "dau2_e_eleup_DM1/F");

      m_smallT->Branch ("dau2_pt_eledown_DM0", &m_dau2_pt_eledown_DM0, "dau2_pt_eledown_DM0/F");
      m_smallT->Branch ("dau2_pt_eledown_DM1", &m_dau2_pt_eledown_DM1, "dau2_pt_eledown_DM1/F");
      m_smallT->Branch ("dau2_e_eledown_DM0",  &m_dau2_e_eledown_DM0,  "dau2_e_eledown_DM0/F");
      m_smallT->Branch ("dau2_e_eledown_DM1",  &m_dau2_e_eledown_DM1,  "dau2_e_eledown_DM1/F");

	  m_smallT->Branch ("dau2_pt_eesUp",   &m_dau2_pt_eesUp,   "dau2_pt_eesUp/F");
	  m_smallT->Branch ("dau2_pt_eesDown", &m_dau2_pt_eesDown, "dau2_pt_eesDown/F");
	  m_smallT->Branch ("dau2_pt_eerUp",   &m_dau2_pt_eerUp,   "dau2_pt_eerUp/F");
	  m_smallT->Branch ("dau2_pt_eerDown", &m_dau2_pt_eerDown, "dau2_pt_eerDown/F");
	  m_smallT->Branch ("dau2_e_eesUp",   &m_dau2_e_eesUp,   "dau2_e_eesUp/F");
	  m_smallT->Branch ("dau2_e_eesDown", &m_dau2_e_eesDown, "dau2_e_eesDown/F");
	  m_smallT->Branch ("dau2_e_eerUp",   &m_dau2_e_eerUp,   "dau2_e_eerUp/F");
	  m_smallT->Branch ("dau2_e_eerDown", &m_dau2_e_eerDown, "dau2_e_eerDown/F");

      m_smallT->Branch ("dau2_eta", &m_dau2_eta, "dau2_eta/F") ;
      m_smallT->Branch ("dau2_phi", &m_dau2_phi, "dau2_phi/F") ;
      m_smallT->Branch ("dau2_e", &m_dau2_e, "dau2_e/F") ;
      m_smallT->Branch ("dau2_flav", &m_dau2_flav, "dau2_flav/F") ;
      m_smallT->Branch ("dau2_dxy", &m_dau2_dxy, "dau2_dxy/F") ;
      m_smallT->Branch ("dau2_dz", &m_dau2_dz, "dau2_dz/F") ;
      m_smallT->Branch ("dau2_decayMode", &m_dau2_decayMode, "dau2_decayMode/I") ;

      m_smallT->Branch ("bjet1_pt", &m_bjet1_pt, "bjet1_pt/F") ;
      m_smallT->Branch ("bjet1_eta", &m_bjet1_eta, "bjet1_eta/F") ;
      m_smallT->Branch ("bjet1_phi", &m_bjet1_phi, "bjet1_phi/F") ;
      m_smallT->Branch ("bjet1_e", &m_bjet1_e, "bjet1_e/F") ;
      m_smallT->Branch ("bjet1_bID_deepFlavor", &m_bjet1_bID_deepFlavor, "bjet1_bID_deepFlavor/F") ;
      m_smallT->Branch ("bjet1_cID_deepFlavor", &m_bjet1_cID_deepFlavor, "bjet1_cID_deepFlavor/F") ;
      m_smallT->Branch ("bjet1_CvsL", &m_bjet1_CvsL, "bjet1_CvsL/F") ;
      m_smallT->Branch ("bjet1_CvsB", &m_bjet1_CvsB, "bjet1_CvsB/F") ;
      m_smallT->Branch ("bjet1_HHbtag", &m_bjet1_HHbtag, "bjet1_HHbtag/F") ;
      m_smallT->Branch ("bjet1_pnet_bb", &m_bjet1_pnet_bb, "bjet1_pnet_bb/F") ;
      m_smallT->Branch ("bjet1_pnet_cc", &m_bjet1_pnet_cc, "bjet1_pnet_cc/F") ;
      m_smallT->Branch ("bjet1_pnet_b", &m_bjet1_pnet_b, "bjet1_pnet_b/F") ;
      m_smallT->Branch ("bjet1_pnet_c", &m_bjet1_pnet_c, "bjet1_pnet_c/F") ;
      m_smallT->Branch ("bjet1_pnet_g", &m_bjet1_pnet_g, "bjet1_pnet_g/F") ;
      m_smallT->Branch ("bjet1_pnet_uds", &m_bjet1_pnet_uds, "bjet1_pnet_uds/F") ;
      m_smallT->Branch ("bjet1_pnet_pu", &m_bjet1_pnet_pu, "bjet1_pnet_pu/F") ;
      m_smallT->Branch ("bjet1_pnet_undef", &m_bjet1_pnet_undef, "bjet1_pnet_undef/F") ;
      m_smallT->Branch ("bjet1_smearFactor", &m_bjet1_smearFactor, "bjet1_smearFactor/F") ;
      m_smallT->Branch ("bjet1_bMVAID", &m_bjet1_bMVAID, "bjet1_bMVAID/F") ;
      m_smallT->Branch ("bjet1_PUjetIDupdated", &m_bjet1_PUjetIDupdated, "bjet1_PUjetIDupdated/F") ;
      m_smallT->Branch ("bjet1_flav", &m_bjet1_flav, "bjet1_flav/I") ;

      m_smallT->Branch ("bjet1_hasgenjet", &m_bjet1_hasgenjet, "bjet1_hasgenjet/O") ;
      m_smallT->Branch ("bjet1_JER",        &m_bjet1_JER, "bjet1_JER/F") ;
      m_smallT->Branch ("bjet1_JER_jetup",  &m_bjet1_JER_jetup) ;
      m_smallT->Branch ("bjet1_JER_jetdown",&m_bjet1_JER_jetdown) ;
      m_smallT->Branch ("bjet1_JER_jetupTot"  , &m_bjet1_JER_jetupTot  , "bjet1_JER_jetupTot/F") ;
      m_smallT->Branch ("bjet1_JER_jetdownTot", &m_bjet1_JER_jetdownTot, "bjet1_JER_jetdownTot/F") ;

      m_smallT->Branch ("bjet1_gen_matched", &m_bjet1_gen_matched, "bjet1_gen_matched/O") ;

      m_smallT->Branch ("bjet2_pt", &m_bjet2_pt, "bjet2_pt/F") ;
      m_smallT->Branch ("bjet2_eta", &m_bjet2_eta, "bjet2_eta/F") ;
      m_smallT->Branch ("bjet2_phi", &m_bjet2_phi, "bjet2_phi/F") ;
      m_smallT->Branch ("bjet2_e", &m_bjet2_e, "bjet2_e/F") ;
      m_smallT->Branch ("bjet2_bID_deepFlavor", &m_bjet2_bID_deepFlavor, "bjet2_bID_deepFlavor/F") ;
      m_smallT->Branch ("bjet2_cID_deepFlavor", &m_bjet2_cID_deepFlavor, "bjet2_cID_deepFlavor/F") ;
      m_smallT->Branch ("bjet2_CvsL", &m_bjet2_CvsL, "bjet2_CvsL/F") ;
      m_smallT->Branch ("bjet2_CvsB", &m_bjet2_CvsB, "bjet2_CvsB/F") ;
      m_smallT->Branch ("bjet2_HHbtag", &m_bjet2_HHbtag, "bjet2_HHbtag/F") ;
      m_smallT->Branch ("bjet2_pnet_bb", &m_bjet2_pnet_bb, "bjet2_pnet_bb/F") ;
      m_smallT->Branch ("bjet2_pnet_cc", &m_bjet2_pnet_cc, "bjet2_pnet_cc/F") ;
      m_smallT->Branch ("bjet2_pnet_b", &m_bjet2_pnet_b, "bjet2_pnet_b/F") ;
      m_smallT->Branch ("bjet2_pnet_c", &m_bjet2_pnet_c, "bjet2_pnet_c/F") ;
      m_smallT->Branch ("bjet2_pnet_g", &m_bjet2_pnet_g, "bjet2_pnet_g/F") ;
      m_smallT->Branch ("bjet2_pnet_uds", &m_bjet2_pnet_uds, "bjet2_pnet_uds/F") ;
      m_smallT->Branch ("bjet2_pnet_pu", &m_bjet2_pnet_pu, "bjet2_pnet_pu/F") ;
      m_smallT->Branch ("bjet2_pnet_undef", &m_bjet2_pnet_undef, "bjet2_pnet_undef/F") ;
      m_smallT->Branch ("bjet2_smearFactor", &m_bjet2_smearFactor, "bjet2_smearFactor/F") ;
      m_smallT->Branch ("bjet2_bMVAID", &m_bjet2_bMVAID, "bjet2_bMVAID/F") ;
      m_smallT->Branch ("bjet2_PUjetIDupdated", &m_bjet2_PUjetIDupdated, "bjet2_PUjetIDupdated/F") ;
      m_smallT->Branch ("bjet2_flav", &m_bjet2_flav, "bjet2_flav/I") ;

      m_smallT->Branch ("bjet2_hasgenjet", &m_bjet2_hasgenjet, "bjet2_hasgenjet/O") ;
      m_smallT->Branch ("bjet2_JER",        &m_bjet2_JER, "bjet2_JER/F") ;
      m_smallT->Branch ("bjet2_JER_jetup",  &m_bjet2_JER_jetup) ;
      m_smallT->Branch ("bjet2_JER_jetdown",&m_bjet2_JER_jetdown) ;
      m_smallT->Branch ("bjet2_JER_jetupTot"  , &m_bjet2_JER_jetupTot  , "bjet2_JER_jetupTot/F") ;
      m_smallT->Branch ("bjet2_JER_jetdownTot", &m_bjet2_JER_jetdownTot, "bjet2_JER_jetdownTot/F") ;

      m_smallT->Branch ("bjet2_gen_matched", &m_bjet2_gen_matched, "bjet2_gen_matched/O") ;
      m_smallT->Branch ("bjets_bID_deepFlavor",&m_bjets_bID_deepFlavor,"bjets_bID_deepFlavor/F");
      m_smallT->Branch ("bjets_cID_deepFlavor",&m_bjets_cID_deepFlavor,"bjets_cID_deepFlavor/F");

      m_smallT->Branch ("bjets_gen_matched", &m_bjets_gen_matched, "bjets_gen_matched/O") ;

      m_smallT->Branch ("nfatjets", &m_nfatjets, "nfatjets/I");
      m_smallT->Branch ("fatjet_highest_pT_bTag", &m_fatjet_highest_pT_bTag, "fatjet_highest_pT_bTag/I");
      m_smallT->Branch ("fatjet_pt", &m_fatjet_pt, "fatjet_pt/F");
      m_smallT->Branch ("fatjet_eta", &m_fatjet_eta, "fatjet_eta/F");
      m_smallT->Branch ("fatjet_phi", &m_fatjet_phi, "fatjet_phi/F");
      m_smallT->Branch ("fatjet_e", &m_fatjet_e, "fatjet_e/F");
      m_smallT->Branch ("fatjet_bID_deepFlavor", &m_fatjet_bID_deepFlavor, "fatjet_bID_deepFlavor/F");
      m_smallT->Branch ("fatjet_massIndependentDeepDoubleBvLJetTags_probHbb", &m_fatjet_massIndependentDeepDoubleBvLJetTags_probHbb, "fatjet_massIndependentDeepDoubleBvLJetTags_probHbb/F");
      m_smallT->Branch ("fatjet_deepDoubleBvLJetTags_probHbb", &m_fatjet_deepDoubleBvLJetTags_probHbb, "fatjet_deepDoubleBvLJetTags_probHbb/F");
      m_smallT->Branch ("fatjet_deepBoostedJetTags_probHbb", &m_fatjet_deepBoostedJetTags_probHbb, "fatjet_deepBoostedJetTags_probHbb/F");
      m_smallT->Branch ("fatjet_particleNetJetTags_probHbb", &m_fatjet_particleNetJetTags_probHbb, "fatjet_particleNetJetTags_probHbb/F");
      m_smallT->Branch ("fatjet_particleNetDiscriminatorsJetTags_HbbvsQCD", &m_fatjet_particleNetDiscriminatorsJetTags_HbbvsQCD, "fatjet_particleNetDiscriminatorsJetTags_HbbvsQCD/F");
      m_smallT->Branch ("fatjet_particleNetMDJetTags_probXbb", &m_fatjet_particleNetMDJetTags_probXbb, "fatjet_particleNetMDJetTags_probXbb/F");
      m_smallT->Branch ("fatjet_particleNetMDJetTags_probXcc", &m_fatjet_particleNetMDJetTags_probXcc, "fatjet_particleNetMDJetTags_probXcc/F");
      m_smallT->Branch ("fatjet_particleNetMDJetTags_probXqq", &m_fatjet_particleNetMDJetTags_probXqq, "fatjet_particleNetMDJetTags_probXqq/F");
      m_smallT->Branch ("fatjet_particleNetMDJetTags_probQCD", &m_fatjet_particleNetMDJetTags_probQCD, "fatjet_particleNetMDJetTags_probQCD/F");
      m_smallT->Branch ("fatjet_particleNetMDJetTags_mass", &m_fatjet_particleNetMDJetTags_mass, "fatjet_particleNetMDJetTags_mass/F");
      m_smallT->Branch ("fatjet_particleNetMDJetTags_score", &m_fatjet_particleNetMDJetTags_score, "fatjet_particleNetMDJetTags_score/F");
      m_smallT->Branch ("fatjet_filteredMass", &m_fatjet_filteredMass, "fatjet_filteredMass/F");
      m_smallT->Branch ("fatjet_prunedMass", &m_fatjet_prunedMass, "fatjet_prunedMass/F");
      m_smallT->Branch ("fatjet_trimmedMass", &m_fatjet_trimmedMass, "fatjet_trimmedMass/F");
      m_smallT->Branch ("fatjet_softdropMass", &m_fatjet_softdropMass, "fatjet_softdropMass/F");
      m_smallT->Branch ("fatjet_tau1", &m_fatjet_tau1, "fatjet_tau1/F");
      m_smallT->Branch ("fatjet_tau2", &m_fatjet_tau2, "fatjet_tau2/F");
      m_smallT->Branch ("fatjet_tau3", &m_fatjet_tau3, "fatjet_tau3/F");
      m_smallT->Branch ("fatjet_nsubjets", &m_fatjet_nsubjets, "fatjet_nsubjets/I");
      m_smallT->Branch ("fatjet_hasMatchedSj", &m_fatjet_hasMatchedSj, "fatjet_hasMatchedSj/I");
      m_smallT->Branch ("dR_subj1_subj2", &m_dR_subj1_subj2, "dR_subj1_subj2/F");

      m_smallT->Branch ("HHbregrsvfit_pt", &m_HHbregrsvfit_pt, "HHbregrsvfit_pt/F");
      m_smallT->Branch ("HHbregrsvfit_eta", &m_HHbregrsvfit_eta, "HHbregrsvfit_eta/F");
      m_smallT->Branch ("HHbregrsvfit_phi", &m_HHbregrsvfit_phi, "HHbregrsvfit_phi/F");
      m_smallT->Branch ("HHbregrsvfit_m", &m_HHbregrsvfit_m, "HHbregrsvfit_m/F");
      m_smallT->Branch ("fatjet_particleNetMDJetTags_HP_SF",      &m_fatjet_particleNetMDJetTags_HP_SF,      "fatjet_particleNetMDJetTags_HP_SF/F");
      m_smallT->Branch ("fatjet_particleNetMDJetTags_HP_SF_up",   &m_fatjet_particleNetMDJetTags_HP_SF_up,   "fatjet_particleNetMDJetTags_HP_SF_up/F");
      m_smallT->Branch ("fatjet_particleNetMDJetTags_HP_SF_down", &m_fatjet_particleNetMDJetTags_HP_SF_down, "fatjet_particleNetMDJetTags_HP_SF_down/F");
      m_smallT->Branch ("fatjet_particleNetMDJetTags_MP_SF",      &m_fatjet_particleNetMDJetTags_MP_SF,      "fatjet_particleNetMDJetTags_MP_SF/F");
      m_smallT->Branch ("fatjet_particleNetMDJetTags_MP_SF_up",   &m_fatjet_particleNetMDJetTags_MP_SF_up,   "fatjet_particleNetMDJetTags_MP_SF_up/F");
      m_smallT->Branch ("fatjet_particleNetMDJetTags_MP_SF_down", &m_fatjet_particleNetMDJetTags_MP_SF_down, "fatjet_particleNetMDJetTags_MP_SF_down/F");
      m_smallT->Branch ("fatjet_particleNetMDJetTags_LP_SF",      &m_fatjet_particleNetMDJetTags_LP_SF,      "fatjet_particleNetMDJetTags_LP_SF/F");
      m_smallT->Branch ("fatjet_particleNetMDJetTags_LP_SF_up",   &m_fatjet_particleNetMDJetTags_LP_SF_up,   "fatjet_particleNetMDJetTags_LP_SF_up/F");
      m_smallT->Branch ("fatjet_particleNetMDJetTags_LP_SF_down", &m_fatjet_particleNetMDJetTags_LP_SF_down, "fatjet_particleNetMDJetTags_LP_SF_down/F");

      m_smallT->Branch ("subjetjet1_pt", &m_subjetjet1_pt, "subjetjet1_pt/F");
      m_smallT->Branch ("subjetjet1_eta", &m_subjetjet1_eta, "subjetjet1_eta/F");
      m_smallT->Branch ("subjetjet1_phi", &m_subjetjet1_phi, "subjetjet1_phi/F");
      m_smallT->Branch ("subjetjet1_e", &m_subjetjet1_e, "subjetjet1_e/F");
      m_smallT->Branch ("subjetjet1_bID_deepFlavor", &m_subjetjet1_bID_deepFlavor, "subjetjet1_bID_deepFlavor/F");

      m_smallT->Branch ("subjetjet2_pt", &m_subjetjet2_pt, "subjetjet2_pt/F");
      m_smallT->Branch ("subjetjet2_eta", &m_subjetjet2_eta, "subjetjet2_eta/F");
      m_smallT->Branch ("subjetjet2_phi", &m_subjetjet2_phi, "subjetjet2_phi/F");
      m_smallT->Branch ("subjetjet2_e", &m_subjetjet2_e, "subjetjet2_e/F");
      m_smallT->Branch ("subjetjet2_bID_deepFlavor", &m_subjetjet2_bID_deepFlavor, "subjetjet2_bID_deepFlavor/F");

      m_smallT->Branch ("genjet1_pt",  &m_genjet1_pt,  "genjet1_pt/F") ;
      m_smallT->Branch ("genjet1_eta", &m_genjet1_eta, "genjet1_eta/F") ;
      m_smallT->Branch ("genjet1_phi", &m_genjet1_phi, "genjet1_phi/F") ;
      m_smallT->Branch ("genjet1_e",   &m_genjet1_e,   "genjet1_e/F") ;

      m_smallT->Branch ("genjet2_pt",  &m_genjet2_pt,  "genjet2_pt/F") ;
      m_smallT->Branch ("genjet2_eta", &m_genjet2_eta, "genjet2_eta/F") ;
      m_smallT->Branch ("genjet2_phi", &m_genjet2_phi, "genjet2_phi/F") ;
      m_smallT->Branch ("genjet2_e",   &m_genjet2_e,   "genjet2_e/F") ;

      m_smallT->Branch ("tauH_pt", &m_tauH_pt, "tauH_pt/F") ;
      m_smallT->Branch ("tauH_eta", &m_tauH_eta, "tauH_eta/F") ;
      m_smallT->Branch ("tauH_phi", &m_tauH_phi, "tauH_phi/F") ;
      m_smallT->Branch ("tauH_e", &m_tauH_e, "tauH_e/F") ;
      m_smallT->Branch ("tauH_mass", &m_tauH_mass, "tauH_mass/F") ;
      m_smallT->Branch ("tauH_SVFIT_mass", &m_tauH_SVFIT_mass, "tauH_SVFIT_mass/F") ;
      m_smallT->Branch ("tauH_SVFIT_pt", &m_tauH_SVFIT_pt, "tauH_SVFIT_pt/F") ;
      m_smallT->Branch ("tauH_SVFIT_eta", &m_tauH_SVFIT_eta, "tauH_SVFIT_eta/F") ;
      m_smallT->Branch ("tauH_SVFIT_phi", &m_tauH_SVFIT_phi, "tauH_SVFIT_phi/F") ;
      m_smallT->Branch ("tauH_SVFIT_METphi", &m_tauH_SVFIT_METphi, "tauH_SVFIT_METphi/F") ;
      m_smallT->Branch ("tauH_SVFIT_METrho", &m_tauH_SVFIT_METrho, "tauH_SVFIT_METrho/F") ;

      m_smallT->Branch ("bH_pt", &m_bH_pt, "bH_pt/F") ;
      m_smallT->Branch ("bH_eta", &m_bH_eta, "bH_eta/F") ;
      m_smallT->Branch ("bH_phi", &m_bH_phi, "bH_phi/F") ;
      m_smallT->Branch ("bH_e", &m_bH_e, "bH_e/F") ;
      m_smallT->Branch ("bH_mass", &m_bH_mass, "bH_mass/F") ;

      m_smallT->Branch ("HHsvfit_pt", &m_HHsvfit_pt, "HHsvfit_pt/F") ;
      m_smallT->Branch ("HHsvfit_eta", &m_HHsvfit_eta, "HHsvfit_eta/F") ;
      m_smallT->Branch ("HHsvfit_phi", &m_HHsvfit_phi, "HHsvfit_phi/F") ;
      m_smallT->Branch ("HHsvfit_e", &m_HHsvfit_e, "HHsvfit_e/F") ;
      m_smallT->Branch ("HHsvfit_mass", &m_HHsvfit_mass, "HHsvfit_mass/F") ;
      m_smallT->Branch ("HH_pt", &m_HH_pt, "HH_pt/F") ;
      m_smallT->Branch ("HH_eta", &m_HH_eta, "HH_eta/F") ;
      m_smallT->Branch ("HH_phi", &m_HH_phi, "HH_phi/F") ;
      m_smallT->Branch ("HH_e", &m_HH_e, "HH_e/F") ;
      m_smallT->Branch ("HH_mass", &m_HH_mass, "HH_mass/F") ;

      m_smallT->Branch ("HHKin_mass", &m_HHKin_mass, "HHKin_mass/F") ;
      m_smallT->Branch ("HHKin_chi2", &m_HHKin_chi2, "HHKin_chi2/F") ;
      m_smallT->Branch ("HH_deltaPhi", &m_HH_deltaPhi, "HH_deltaPhi/F") ;
      m_smallT->Branch ("HH_deltaR", &m_HH_deltaR, "HH_deltaR/F") ;
      m_smallT->Branch ("HH_deltaEta", &m_HH_deltaEta, "HH_deltaEta/F") ;
      m_smallT->Branch ("HHsvfit_deltaPhi", &m_HHsvfit_deltaPhi, "HHsvfit_deltaPhi/F") ;
      m_smallT->Branch ("tauHMet_deltaPhi", &m_tauHMet_deltaPhi, "tauHMet_deltaPhi/F") ;
      m_smallT->Branch ("tauHsvfitMet_deltaPhi", &m_tauHsvfitMet_deltaPhi, "tauHsvfitMet_deltaPhi/F") ;
      m_smallT->Branch ("bHMet_deltaPhi", &m_bHMet_deltaPhi, "bHMet_deltaPhi/F") ;

      m_smallT->Branch ("ditau_deltaPhi", &m_ditau_deltaPhi, "ditau_deltaPhi/F") ;
      m_smallT->Branch ("ditau_deltaEta", &m_ditau_deltaEta, "ditau_deltaEta/F") ;
      m_smallT->Branch ("dib_deltaPhi", &m_dib_deltaPhi, "dib_deltaPhi/F") ;
      m_smallT->Branch ("dib_deltaEta", &m_dib_deltaEta, "dib_deltaEta/F") ;
      m_smallT->Branch ("ditau_deltaR",   &m_ditau_deltaR,    "ditau_deltaR/F") ;
      m_smallT->Branch ("dib_deltaR",     &m_dib_deltaR,      "dib_deltaR/F") ;
      m_smallT->Branch ("dib_deltaR_per_bHpt", &m_dib_deltaR_per_bHpt, "dib_deltaR_per_bHpt/F") ;

      m_smallT->Branch ("btau_deltaRmin", &m_btau_deltaRmin,  "btau_deltaRmin/F") ;
      m_smallT->Branch ("btau_deltaRmax", &m_btau_deltaRmax,  "btau_deltaRmax/F") ;
      m_smallT->Branch ("dau1MET_deltaphi", &m_dau1MET_deltaphi,  "dau1MET_deltaphi/F") ;
      m_smallT->Branch ("dau2MET_deltaphi", &m_dau2MET_deltaphi,  "dau2MET_deltaphi/F") ;
      m_smallT->Branch ("HT20", &m_HT20, "HT20/F");
      m_smallT->Branch ("HT50", &m_HT50, "HT50/F");
      m_smallT->Branch ("HT20Full", &m_HT20Full, "HT20Full/F");
      m_smallT->Branch ("jet20centrality", &m_jet20centrality, "jet20centrality/F");

      m_smallT->Branch ("HEM1516veto", &m_HEM1516veto, "HEM1516veto/I");

      m_smallT->Branch ("jets_pt", &m_jets_pt) ;
      m_smallT->Branch ("jets_eta", &m_jets_eta) ;
      m_smallT->Branch ("jets_phi", &m_jets_phi) ;
      m_smallT->Branch ("jets_e", &m_jets_e) ;
      m_smallT->Branch ("jets_btag_deepFlavor", &m_jets_btag_deepFlavor) ;
      m_smallT->Branch ("jets_ctag_deepFlavor", &m_jets_ctag_deepFlavor) ;
      m_smallT->Branch ("jets_CvsL", &m_jets_CvsL) ;
      m_smallT->Branch ("jets_CvsB", &m_jets_CvsB) ;
      m_smallT->Branch ("jets_HHbtag", &m_jets_HHbtag) ;
      m_smallT->Branch ("jets_pnet_bb", &m_jets_pnet_bb) ;
      m_smallT->Branch ("jets_pnet_cc", &m_jets_pnet_cc) ;
      m_smallT->Branch ("jets_pnet_b", &m_jets_pnet_b) ;
      m_smallT->Branch ("jets_pnet_c", &m_jets_pnet_c) ;
      m_smallT->Branch ("jets_pnet_g", &m_jets_pnet_g) ;
      m_smallT->Branch ("jets_pnet_uds", &m_jets_pnet_uds) ;
      m_smallT->Branch ("jets_pnet_pu", &m_jets_pnet_pu) ;
      m_smallT->Branch ("jets_pnet_undef", &m_jets_pnet_undef) ;
      m_smallT->Branch ("smearFactor_AK4", &m_smearFactor_AK4) ;
	  m_smallT->Branch ("smearFactor_AK8", &m_smearFactor_AK8) ;
      m_smallT->Branch ("jets_flav", &m_jets_flav) ;
      m_smallT->Branch ("jets_isH", &m_jets_isH) ;
      m_smallT->Branch ("jets_hasgenjet", &m_jets_hasgenjet) ;
      m_smallT->Branch ("njets", &m_njets, "njets/I") ;
      m_smallT->Branch ("addjets", &m_addjets, "addjets/I") ;
      m_smallT->Branch ("njets20", &m_njets20, "njets20/I") ;
      m_smallT->Branch ("njets50", &m_njets50, "njets50/I") ;
      m_smallT->Branch ("nbjets20", &m_nbjets20, "nbjets20/I") ;
      m_smallT->Branch ("nbjets50", &m_nbjets50, "nbjets50/I") ;
      m_smallT->Branch ("nbjetscand", &m_nbjetscand, "nbjetscand/I") ;
      m_smallT->Branch ("njetsBHadFlav", &m_njetsBHadFlav, "njetsBHadFlav/I") ;
      m_smallT->Branch ("njetsCHadFlav", &m_njetsCHadFlav, "njetsCHadFlav/I") ;
      m_smallT->Branch ("jets_jecUnc", &m_jets_jecUnc);
      m_smallT->Branch ("dau1_jecUnc", &m_dau1_jecUnc,"dau1_jecUnc/F");
      m_smallT->Branch ("dau2_jecUnc", &m_dau2_jecUnc,"dau2_jecUnc/F");
      m_smallT->Branch ("bjet1_jecUnc", &m_bjet1_jecUnc,"bjet1_jecUnc/F");
      m_smallT->Branch ("bjet2_jecUnc", &m_bjet2_jecUnc,"bjet2_jecUnc/F");

      m_smallT->Branch ("PUjetIDupdated", &m_PUjetID);

      m_smallT->Branch ("leps_pt", &m_leps_pt) ;
      m_smallT->Branch ("leps_eta", &m_leps_eta) ;
      m_smallT->Branch ("leps_phi", &m_leps_phi) ;
      m_smallT->Branch ("leps_e", &m_leps_e) ;
      m_smallT->Branch ("leps_flav", &m_leps_flav) ;
      m_smallT->Branch ("nleps", &m_nleps, "nleps/I") ;

      m_smallT->Branch("HHkinsvfit_bHmass", &m_HHkinsvfit_bHmass,"HHkinsvfit_bHmass/F");
      m_smallT->Branch("HHkinsvfit_pt", &m_HHkinsvfit_pt,"HHkinsvfit_pt/F");
      m_smallT->Branch("HHkinsvfit_eta", &m_HHkinsvfit_eta,"HHkinsvfit_eta/F");
      m_smallT->Branch("HHkinsvfit_phi", &m_HHkinsvfit_phi,"HHkinsvfit_phi/F");
      m_smallT->Branch("HHkinsvfit_e", &m_HHkinsvfit_e,"HHkinsvfit_e/F");
      m_smallT->Branch("HHkinsvfit_m", &m_HHkinsvfit_m,"HHkinsvfit_m/F");
	  
	  m_smallT->Branch ("bjet1_pt_jetup1",  &m_bjet1_pt_jetup1,  "bjet1_pt_jetup1/F");
      m_smallT->Branch ("bjet1_pt_jetup2",  &m_bjet1_pt_jetup2,  "bjet1_pt_jetup2/F");
      m_smallT->Branch ("bjet1_pt_jetup3",  &m_bjet1_pt_jetup3,  "bjet1_pt_jetup3/F");
      m_smallT->Branch ("bjet1_pt_jetup4",  &m_bjet1_pt_jetup4,  "bjet1_pt_jetup4/F");
      m_smallT->Branch ("bjet1_pt_jetup5",  &m_bjet1_pt_jetup5,  "bjet1_pt_jetup5/F");
      m_smallT->Branch ("bjet1_pt_jetup6",  &m_bjet1_pt_jetup6,  "bjet1_pt_jetup6/F");
      m_smallT->Branch ("bjet1_pt_jetup7",  &m_bjet1_pt_jetup7,  "bjet1_pt_jetup7/F");
      m_smallT->Branch ("bjet1_pt_jetup8",  &m_bjet1_pt_jetup8,  "bjet1_pt_jetup8/F");
      m_smallT->Branch ("bjet1_pt_jetup9",  &m_bjet1_pt_jetup9,  "bjet1_pt_jetup9/F");
      m_smallT->Branch ("bjet1_pt_jetup10", &m_bjet1_pt_jetup10, "bjet1_pt_jetup10/F");
      m_smallT->Branch ("bjet1_pt_jetup11", &m_bjet1_pt_jetup11, "bjet1_pt_jetup11/F");
	  m_smallT->Branch ("bjet1_e_jetup1",   &m_bjet1_e_jetup1,   "bjet1_e_jetup1/F");
      m_smallT->Branch ("bjet1_e_jetup2",   &m_bjet1_e_jetup2,   "bjet1_e_jetup2/F");
      m_smallT->Branch ("bjet1_e_jetup3",   &m_bjet1_e_jetup3,   "bjet1_e_jetup3/F");
      m_smallT->Branch ("bjet1_e_jetup4",   &m_bjet1_e_jetup4,   "bjet1_e_jetup4/F");
      m_smallT->Branch ("bjet1_e_jetup5",   &m_bjet1_e_jetup5,   "bjet1_e_jetup5/F");
      m_smallT->Branch ("bjet1_e_jetup6",   &m_bjet1_e_jetup6,   "bjet1_e_jetup6/F");
      m_smallT->Branch ("bjet1_e_jetup7",   &m_bjet1_e_jetup7,   "bjet1_e_jetup7/F");
      m_smallT->Branch ("bjet1_e_jetup8",   &m_bjet1_e_jetup8,   "bjet1_e_jetup8/F");
      m_smallT->Branch ("bjet1_e_jetup9",   &m_bjet1_e_jetup9,   "bjet1_e_jetup9/F");
      m_smallT->Branch ("bjet1_e_jetup10",  &m_bjet1_e_jetup10,  "bjet1_e_jetup10/F");
      m_smallT->Branch ("bjet1_e_jetup11",  &m_bjet1_e_jetup11,  "bjet1_e_jetup11/F");

	  m_smallT->Branch ("bjet1_pt_jetdown1",  &m_bjet1_pt_jetdown1,  "bjet1_pt_jetdown1/F");
      m_smallT->Branch ("bjet1_pt_jetdown2",  &m_bjet1_pt_jetdown2,  "bjet1_pt_jetdown2/F");
      m_smallT->Branch ("bjet1_pt_jetdown3",  &m_bjet1_pt_jetdown3,  "bjet1_pt_jetdown3/F");
      m_smallT->Branch ("bjet1_pt_jetdown4",  &m_bjet1_pt_jetdown4,  "bjet1_pt_jetdown4/F");
      m_smallT->Branch ("bjet1_pt_jetdown5",  &m_bjet1_pt_jetdown5,  "bjet1_pt_jetdown5/F");
      m_smallT->Branch ("bjet1_pt_jetdown6",  &m_bjet1_pt_jetdown6,  "bjet1_pt_jetdown6/F");
      m_smallT->Branch ("bjet1_pt_jetdown7",  &m_bjet1_pt_jetdown7,  "bjet1_pt_jetdown7/F");
      m_smallT->Branch ("bjet1_pt_jetdown8",  &m_bjet1_pt_jetdown8,  "bjet1_pt_jetdown8/F");
      m_smallT->Branch ("bjet1_pt_jetdown9",  &m_bjet1_pt_jetdown9,  "bjet1_pt_jetdown9/F");
      m_smallT->Branch ("bjet1_pt_jetdown10", &m_bjet1_pt_jetdown10, "bjet1_pt_jetdown10/F");
      m_smallT->Branch ("bjet1_pt_jetdown11", &m_bjet1_pt_jetdown11, "bjet1_pt_jetdown11/F");
	  m_smallT->Branch ("bjet1_e_jetdown1",   &m_bjet1_e_jetdown1,   "bjet1_e_jetdown1/F");
      m_smallT->Branch ("bjet1_e_jetdown2",   &m_bjet1_e_jetdown2,   "bjet1_e_jetdown2/F");
      m_smallT->Branch ("bjet1_e_jetdown3",   &m_bjet1_e_jetdown3,   "bjet1_e_jetdown3/F");
      m_smallT->Branch ("bjet1_e_jetdown4",   &m_bjet1_e_jetdown4,   "bjet1_e_jetdown4/F");
      m_smallT->Branch ("bjet1_e_jetdown5",   &m_bjet1_e_jetdown5,   "bjet1_e_jetdown5/F");
      m_smallT->Branch ("bjet1_e_jetdown6",   &m_bjet1_e_jetdown6,   "bjet1_e_jetdown6/F");
      m_smallT->Branch ("bjet1_e_jetdown7",   &m_bjet1_e_jetdown7,   "bjet1_e_jetdown7/F");
      m_smallT->Branch ("bjet1_e_jetdown8",   &m_bjet1_e_jetdown8,   "bjet1_e_jetdown8/F");
      m_smallT->Branch ("bjet1_e_jetdown9",   &m_bjet1_e_jetdown9,   "bjet1_e_jetdown9/F");
      m_smallT->Branch ("bjet1_e_jetdown10",  &m_bjet1_e_jetdown10,  "bjet1_e_jetdown10/F");
      m_smallT->Branch ("bjet1_e_jetdown11",  &m_bjet1_e_jetdown11,  "bjet1_e_jetdown11/F");

	  m_smallT->Branch ("bjet2_pt_jetup1",  &m_bjet2_pt_jetup1,  "bjet2_pt_jetup1/F");
      m_smallT->Branch ("bjet2_pt_jetup2",  &m_bjet2_pt_jetup2,  "bjet2_pt_jetup2/F");
      m_smallT->Branch ("bjet2_pt_jetup3",  &m_bjet2_pt_jetup3,  "bjet2_pt_jetup3/F");
      m_smallT->Branch ("bjet2_pt_jetup4",  &m_bjet2_pt_jetup4,  "bjet2_pt_jetup4/F");
      m_smallT->Branch ("bjet2_pt_jetup5",  &m_bjet2_pt_jetup5,  "bjet2_pt_jetup5/F");
      m_smallT->Branch ("bjet2_pt_jetup6",  &m_bjet2_pt_jetup6,  "bjet2_pt_jetup6/F");
      m_smallT->Branch ("bjet2_pt_jetup7",  &m_bjet2_pt_jetup7,  "bjet2_pt_jetup7/F");
      m_smallT->Branch ("bjet2_pt_jetup8",  &m_bjet2_pt_jetup8,  "bjet2_pt_jetup8/F");
      m_smallT->Branch ("bjet2_pt_jetup9",  &m_bjet2_pt_jetup9,  "bjet2_pt_jetup9/F");
      m_smallT->Branch ("bjet2_pt_jetup10", &m_bjet2_pt_jetup10, "bjet2_pt_jetup10/F");
      m_smallT->Branch ("bjet2_pt_jetup11", &m_bjet2_pt_jetup11, "bjet2_pt_jetup11/F");
	  m_smallT->Branch ("bjet2_e_jetup1",   &m_bjet2_e_jetup1,   "bjet2_e_jetup1/F");
      m_smallT->Branch ("bjet2_e_jetup2",   &m_bjet2_e_jetup2,   "bjet2_e_jetup2/F");
      m_smallT->Branch ("bjet2_e_jetup3",   &m_bjet2_e_jetup3,   "bjet2_e_jetup3/F");
      m_smallT->Branch ("bjet2_e_jetup4",   &m_bjet2_e_jetup4,   "bjet2_e_jetup4/F");
      m_smallT->Branch ("bjet2_e_jetup5",   &m_bjet2_e_jetup5,   "bjet2_e_jetup5/F");
      m_smallT->Branch ("bjet2_e_jetup6",   &m_bjet2_e_jetup6,   "bjet2_e_jetup6/F");
      m_smallT->Branch ("bjet2_e_jetup7",   &m_bjet2_e_jetup7,   "bjet2_e_jetup7/F");
      m_smallT->Branch ("bjet2_e_jetup8",   &m_bjet2_e_jetup8,   "bjet2_e_jetup8/F");
      m_smallT->Branch ("bjet2_e_jetup9",   &m_bjet2_e_jetup9,   "bjet2_e_jetup9/F");
      m_smallT->Branch ("bjet2_e_jetup10",  &m_bjet2_e_jetup10,  "bjet2_e_jetup10/F");
      m_smallT->Branch ("bjet2_e_jetup11",  &m_bjet2_e_jetup11,  "bjet2_e_jetup11/F");

	  m_smallT->Branch ("bjet2_pt_jetdown1",  &m_bjet2_pt_jetdown1,  "bjet2_pt_jetdown1/F");
      m_smallT->Branch ("bjet2_pt_jetdown2",  &m_bjet2_pt_jetdown2,  "bjet2_pt_jetdown2/F");
      m_smallT->Branch ("bjet2_pt_jetdown3",  &m_bjet2_pt_jetdown3,  "bjet2_pt_jetdown3/F");
      m_smallT->Branch ("bjet2_pt_jetdown4",  &m_bjet2_pt_jetdown4,  "bjet2_pt_jetdown4/F");
      m_smallT->Branch ("bjet2_pt_jetdown5",  &m_bjet2_pt_jetdown5,  "bjet2_pt_jetdown5/F");
      m_smallT->Branch ("bjet2_pt_jetdown6",  &m_bjet2_pt_jetdown6,  "bjet2_pt_jetdown6/F");
      m_smallT->Branch ("bjet2_pt_jetdown7",  &m_bjet2_pt_jetdown7,  "bjet2_pt_jetdown7/F");
      m_smallT->Branch ("bjet2_pt_jetdown8",  &m_bjet2_pt_jetdown8,  "bjet2_pt_jetdown8/F");
      m_smallT->Branch ("bjet2_pt_jetdown9",  &m_bjet2_pt_jetdown9,  "bjet2_pt_jetdown9/F");
      m_smallT->Branch ("bjet2_pt_jetdown10", &m_bjet2_pt_jetdown10, "bjet2_pt_jetdown10/F");
      m_smallT->Branch ("bjet2_pt_jetdown11", &m_bjet2_pt_jetdown11, "bjet2_pt_jetdown11/F");
	  m_smallT->Branch ("bjet2_e_jetdown1",   &m_bjet2_e_jetdown1,   "bjet2_e_jetdown1/F");
      m_smallT->Branch ("bjet2_e_jetdown2",   &m_bjet2_e_jetdown2,   "bjet2_e_jetdown2/F");
      m_smallT->Branch ("bjet2_e_jetdown3",   &m_bjet2_e_jetdown3,   "bjet2_e_jetdown3/F");
      m_smallT->Branch ("bjet2_e_jetdown4",   &m_bjet2_e_jetdown4,   "bjet2_e_jetdown4/F");
      m_smallT->Branch ("bjet2_e_jetdown5",   &m_bjet2_e_jetdown5,   "bjet2_e_jetdown5/F");
      m_smallT->Branch ("bjet2_e_jetdown6",   &m_bjet2_e_jetdown6,   "bjet2_e_jetdown6/F");
      m_smallT->Branch ("bjet2_e_jetdown7",   &m_bjet2_e_jetdown7,   "bjet2_e_jetdown7/F");
      m_smallT->Branch ("bjet2_e_jetdown8",   &m_bjet2_e_jetdown8,   "bjet2_e_jetdown8/F");
      m_smallT->Branch ("bjet2_e_jetdown9",   &m_bjet2_e_jetdown9,   "bjet2_e_jetdown9/F");
      m_smallT->Branch ("bjet2_e_jetdown10",  &m_bjet2_e_jetdown10,  "bjet2_e_jetdown10/F");
      m_smallT->Branch ("bjet2_e_jetdown11",  &m_bjet2_e_jetdown11,  "bjet2_e_jetdown11/F");

	  m_smallT->Branch ("fatjet_pt_jetup1",  &m_fatjet_pt_jetup1,  "fatjet_pt_jetup1/F");
      m_smallT->Branch ("fatjet_pt_jetup2",  &m_fatjet_pt_jetup2,  "fatjet_pt_jetup2/F");
      m_smallT->Branch ("fatjet_pt_jetup3",  &m_fatjet_pt_jetup3,  "fatjet_pt_jetup3/F");
      m_smallT->Branch ("fatjet_pt_jetup4",  &m_fatjet_pt_jetup4,  "fatjet_pt_jetup4/F");
      m_smallT->Branch ("fatjet_pt_jetup5",  &m_fatjet_pt_jetup5,  "fatjet_pt_jetup5/F");
      m_smallT->Branch ("fatjet_pt_jetup6",  &m_fatjet_pt_jetup6,  "fatjet_pt_jetup6/F");
      m_smallT->Branch ("fatjet_pt_jetup7",  &m_fatjet_pt_jetup7,  "fatjet_pt_jetup7/F");
      m_smallT->Branch ("fatjet_pt_jetup8",  &m_fatjet_pt_jetup8,  "fatjet_pt_jetup8/F");
      m_smallT->Branch ("fatjet_pt_jetup9",  &m_fatjet_pt_jetup9,  "fatjet_pt_jetup9/F");
      m_smallT->Branch ("fatjet_pt_jetup10", &m_fatjet_pt_jetup10, "fatjet_pt_jetup10/F");
      m_smallT->Branch ("fatjet_pt_jetup11", &m_fatjet_pt_jetup11, "fatjet_pt_jetup11/F");
	  m_smallT->Branch ("fatjet_e_jetup1",   &m_fatjet_e_jetup1,   "fatjet_e_jetup1/F");
      m_smallT->Branch ("fatjet_e_jetup2",   &m_fatjet_e_jetup2,   "fatjet_e_jetup2/F");
      m_smallT->Branch ("fatjet_e_jetup3",   &m_fatjet_e_jetup3,   "fatjet_e_jetup3/F");
      m_smallT->Branch ("fatjet_e_jetup4",   &m_fatjet_e_jetup4,   "fatjet_e_jetup4/F");
      m_smallT->Branch ("fatjet_e_jetup5",   &m_fatjet_e_jetup5,   "fatjet_e_jetup5/F");
      m_smallT->Branch ("fatjet_e_jetup6",   &m_fatjet_e_jetup6,   "fatjet_e_jetup6/F");
      m_smallT->Branch ("fatjet_e_jetup7",   &m_fatjet_e_jetup7,   "fatjet_e_jetup7/F");
      m_smallT->Branch ("fatjet_e_jetup8",   &m_fatjet_e_jetup8,   "fatjet_e_jetup8/F");
      m_smallT->Branch ("fatjet_e_jetup9",   &m_fatjet_e_jetup9,   "fatjet_e_jetup9/F");
      m_smallT->Branch ("fatjet_e_jetup10",  &m_fatjet_e_jetup10,  "fatjet_e_jetup10/F");
      m_smallT->Branch ("fatjet_e_jetup11",  &m_fatjet_e_jetup11,  "fatjet_e_jetup11/F");

	  m_smallT->Branch ("fatjet_pt_jetdown1",  &m_fatjet_pt_jetdown1,  "fatjet_pt_jetdown1/F");
      m_smallT->Branch ("fatjet_pt_jetdown2",  &m_fatjet_pt_jetdown2,  "fatjet_pt_jetdown2/F");
      m_smallT->Branch ("fatjet_pt_jetdown3",  &m_fatjet_pt_jetdown3,  "fatjet_pt_jetdown3/F");
      m_smallT->Branch ("fatjet_pt_jetdown4",  &m_fatjet_pt_jetdown4,  "fatjet_pt_jetdown4/F");
      m_smallT->Branch ("fatjet_pt_jetdown5",  &m_fatjet_pt_jetdown5,  "fatjet_pt_jetdown5/F");
      m_smallT->Branch ("fatjet_pt_jetdown6",  &m_fatjet_pt_jetdown6,  "fatjet_pt_jetdown6/F");
      m_smallT->Branch ("fatjet_pt_jetdown7",  &m_fatjet_pt_jetdown7,  "fatjet_pt_jetdown7/F");
      m_smallT->Branch ("fatjet_pt_jetdown8",  &m_fatjet_pt_jetdown8,  "fatjet_pt_jetdown8/F");
      m_smallT->Branch ("fatjet_pt_jetdown9",  &m_fatjet_pt_jetdown9,  "fatjet_pt_jetdown9/F");
      m_smallT->Branch ("fatjet_pt_jetdown10", &m_fatjet_pt_jetdown10, "fatjet_pt_jetdown10/F");
      m_smallT->Branch ("fatjet_pt_jetdown11", &m_fatjet_pt_jetdown11, "fatjet_pt_jetdown11/F");
	  m_smallT->Branch ("fatjet_e_jetdown1",  &m_fatjet_e_jetdown1,  "fatjet_e_jetdown1/F");
      m_smallT->Branch ("fatjet_e_jetdown2",  &m_fatjet_e_jetdown2,  "fatjet_e_jetdown2/F");
      m_smallT->Branch ("fatjet_e_jetdown3",  &m_fatjet_e_jetdown3,  "fatjet_e_jetdown3/F");
      m_smallT->Branch ("fatjet_e_jetdown4",  &m_fatjet_e_jetdown4,  "fatjet_e_jetdown4/F");
      m_smallT->Branch ("fatjet_e_jetdown5",  &m_fatjet_e_jetdown5,  "fatjet_e_jetdown5/F");
      m_smallT->Branch ("fatjet_e_jetdown6",  &m_fatjet_e_jetdown6,  "fatjet_e_jetdown6/F");
      m_smallT->Branch ("fatjet_e_jetdown7",  &m_fatjet_e_jetdown7,  "fatjet_e_jetdown7/F");
      m_smallT->Branch ("fatjet_e_jetdown8",  &m_fatjet_e_jetdown8,  "fatjet_e_jetdown8/F");
      m_smallT->Branch ("fatjet_e_jetdown9",  &m_fatjet_e_jetdown9,  "fatjet_e_jetdown9/F");
      m_smallT->Branch ("fatjet_e_jetdown10", &m_fatjet_e_jetdown10, "fatjet_e_jetdown10/F");
      m_smallT->Branch ("fatjet_e_jetdown11", &m_fatjet_e_jetdown11, "fatjet_e_jetdown11/F");

      m_smallT->Branch ("bH_mass_jetup1",   &m_bH_mass_jetup1,  "bH_mass_jetup1/F");
      m_smallT->Branch ("bH_mass_jetup2",   &m_bH_mass_jetup2,  "bH_mass_jetup2/F");
      m_smallT->Branch ("bH_mass_jetup3",   &m_bH_mass_jetup3,  "bH_mass_jetup3/F");
      m_smallT->Branch ("bH_mass_jetup4",   &m_bH_mass_jetup4,  "bH_mass_jetup4/F");
      m_smallT->Branch ("bH_mass_jetup5",   &m_bH_mass_jetup5,  "bH_mass_jetup5/F");
      m_smallT->Branch ("bH_mass_jetup6",   &m_bH_mass_jetup6,  "bH_mass_jetup6/F");
      m_smallT->Branch ("bH_mass_jetup7",   &m_bH_mass_jetup7,  "bH_mass_jetup7/F");
      m_smallT->Branch ("bH_mass_jetup8",   &m_bH_mass_jetup8,  "bH_mass_jetup8/F");
      m_smallT->Branch ("bH_mass_jetup9",   &m_bH_mass_jetup9,  "bH_mass_jetup9/F");
      m_smallT->Branch ("bH_mass_jetup10",  &m_bH_mass_jetup10, "bH_mass_jetup10/F");
      m_smallT->Branch ("bH_mass_jetup11",  &m_bH_mass_jetup11, "bH_mass_jetup11/F");

      m_smallT->Branch ("bH_mass_jetdown1",  &m_bH_mass_jetdown1,  "bH_mass_jetdown1/F");
      m_smallT->Branch ("bH_mass_jetdown2",  &m_bH_mass_jetdown2,  "bH_mass_jetdown2/F");
      m_smallT->Branch ("bH_mass_jetdown3",  &m_bH_mass_jetdown3,  "bH_mass_jetdown3/F");
      m_smallT->Branch ("bH_mass_jetdown4",  &m_bH_mass_jetdown4,  "bH_mass_jetdown4/F");
      m_smallT->Branch ("bH_mass_jetdown5",  &m_bH_mass_jetdown5,  "bH_mass_jetdown5/F");
      m_smallT->Branch ("bH_mass_jetdown6",  &m_bH_mass_jetdown6,  "bH_mass_jetdown6/F");
      m_smallT->Branch ("bH_mass_jetdown7",  &m_bH_mass_jetdown7,  "bH_mass_jetdown7/F");
      m_smallT->Branch ("bH_mass_jetdown8",  &m_bH_mass_jetdown8,  "bH_mass_jetdown8/F");
      m_smallT->Branch ("bH_mass_jetdown9",  &m_bH_mass_jetdown9,  "bH_mass_jetdown9/F");
      m_smallT->Branch ("bH_mass_jetdown10", &m_bH_mass_jetdown10, "bH_mass_jetdown10/F");
      m_smallT->Branch ("bH_mass_jetdown11", &m_bH_mass_jetdown11, "bH_mass_jetdown11/F");
	  
      m_smallT->Branch ("bjet1_pt_jetupTot",   &m_bjet1_pt_jetupTot,   "bjet1_pt_jetupTot/F");
      m_smallT->Branch ("bjet1_pt_jetdownTot", &m_bjet1_pt_jetdownTot, "bjet1_pt_jetdownTot/F");
      m_smallT->Branch ("bjet2_pt_jetupTot",   &m_bjet2_pt_jetupTot,   "bjet2_pt_jetupTot/F");
      m_smallT->Branch ("bjet2_pt_jetdownTot", &m_bjet2_pt_jetdownTot, "bjet2_pt_jetdownTot/F");
      m_smallT->Branch ("bjet1_e_jetupTot",    &m_bjet1_e_jetupTot,    "bjet1_e_jetupTot/F");
      m_smallT->Branch ("bjet1_e_jetdownTot",  &m_bjet1_e_jetdownTot,  "bjet1_e_jetdownTot/F");
      m_smallT->Branch ("bjet2_e_jetupTot"  ,  &m_bjet2_e_jetupTot,    "bjet2_e_jetupTot/F");
      m_smallT->Branch ("bjet2_e_jetdownTot",  &m_bjet2_e_jetdownTot,  "bjet2_e_jetdownTot/F");
      m_smallT->Branch ("bH_mass_jetupTot",    &m_bH_mass_jetupTot,    "bH_mass_jetupTot/F");
      m_smallT->Branch ("bH_mass_jetdownTot",  &m_bH_mass_jetdownTot,  "bH_mass_jetdownTot/F");

      m_smallT->Branch ("fatjet_pt_jetupTot",   &m_fatjet_pt_jetupTot,   "fatjet_pt_jetupTot/F");
	  m_smallT->Branch ("fatjet_pt_jetdownTot", &m_fatjet_pt_jetdownTot, "fatjet_pt_jetdownTot/F");
	  m_smallT->Branch ("fatjet_e_jetupTot",    &m_fatjet_e_jetupTot,    "fatjet_e_jetupTot/F");
	  m_smallT->Branch ("fatjet_e_jetdownTot",  &m_fatjet_e_jetdownTot,  "fatjet_e_jetdownTot/F");

      m_smallT->Branch ("lheht", &m_lheht, "lheht/F");
      m_smallT->Branch("topReweight",&m_topReweight,"topReweight/F");

      // Additional variables
      m_smallT->Branch ("dau1_z",&m_dau1_z, "dau1_z/F") ;
      m_smallT->Branch ("dau2_z",&m_dau2_z , "dau2_z/F") ;
      m_smallT->Branch ("bjet1_z",&m_bjet1_z, "bjet1_z/F") ;
      m_smallT->Branch ("bjet2_z",&m_bjet2_z, "bjet2_z/F") ;

      m_smallT->Branch ("tauH_z",&m_tauH_z , "tauH_z/F") ;
      m_smallT->Branch ("bH_z",&m_bH_z, "bH_z/F") ;

      m_smallT->Branch ("HH_z",&m_HH_z, "HH_z/F") ;
      m_smallT->Branch ("HH_zV",&m_HH_zV, "HH_zV/F") ;
      m_smallT->Branch ("HH_A",&m_HH_A, "HH_A/F") ;

      // For Angela's BDT
      m_smallT->Branch ("tauH_MET_pt",                    &m_tauH_MET_pt                   , "tauH_MET_pt/F");
      m_smallT->Branch ("dau2_MET_deltaEta",              &m_dau2_MET_deltaEta             , "dau2_MET_deltaEta/F");
      m_smallT->Branch ("bH_MET_deltaEta",                &m_bH_MET_deltaEta               , "bH_MET_deltaEta/F");
      m_smallT->Branch ("bH_MET_deltaR",                  &m_bH_MET_deltaR                 , "bH_MET_deltaR/F");
      m_smallT->Branch ("bH_tauH_MET_deltaR",             &m_bH_tauH_MET_deltaR            , "bH_tauH_MET_deltaR/F");
      m_smallT->Branch ("ditau_deltaR_per_tauH_MET_pt",   &m_ditau_deltaR_per_tauH_MET_pt  , "ditau_deltaR_per_tauH_MET_pt/F");

      m_smallT->Branch ("isTau1real",     &m_isTau1real     , "isTau1real/I");
      m_smallT->Branch ("isTau2real",     &m_isTau2real     , "isTau2real/I");
      m_smallT->Branch ("nRealTaus",      &m_nRealTaus     , "nRealTaus/I");

      return 0 ;
    }

  // the tree itself
  TTree * m_smallT ;

  // general variables
  Float_t m_MC_weight ;
  Float_t m_MC_weight_fixed ;
  Float_t m_aMCatNLOweight ;
  Float_t m_MC_QCDscale0 ;
  Float_t m_MC_QCDscale1 ;
  Float_t m_MC_QCDscale2 ;
  Float_t m_MC_QCDscale3 ;
  Float_t m_MC_QCDscale4 ;
  Float_t m_MC_QCDscale5 ;
  Float_t m_MC_QCDscale6 ;
  Float_t m_MC_pdf0 ;
  Float_t m_MC_pdf1 ;
  Float_t m_MC_pdf2 ;
  Float_t m_MC_pdf3 ;
  Float_t m_MC_pdf4 ;
  Float_t m_MC_pdf5 ;
  Float_t m_MC_pdf6 ;
  Float_t m_MC_pdf7 ;
  Float_t m_MC_pdf8 ;
  Float_t m_MC_pdf9 ;
  Float_t m_MC_pdf10 ;
  Float_t m_MC_pdf11 ;
  Float_t m_MC_pdf12 ;
  Float_t m_MC_pdf13 ;
  Float_t m_MC_pdf14 ;
  Float_t m_MC_pdf15 ;
  Float_t m_MC_pdf16 ;
  Float_t m_MC_pdf17 ;
  Float_t m_MC_pdf18 ;
  Float_t m_MC_pdf19 ;
  Float_t m_MC_pdf20 ;
  Float_t m_MC_pdf21 ;
  Float_t m_MC_pdf22 ;
  Float_t m_MC_pdf23 ;
  Float_t m_MC_pdf24 ;
  Float_t m_MC_pdf25 ;
  Float_t m_MC_pdf26 ;
  Float_t m_MC_pdf27 ;
  Float_t m_MC_pdf28 ;
  Float_t m_MC_pdf29 ;
  Float_t m_MC_pdf30 ;
  Float_t m_MC_pdf31 ;
  Float_t m_MC_pdf32 ;
  Float_t m_MC_pdf33 ;
  Float_t m_MC_pdf34 ;
  Float_t m_MC_pdf35 ;
  Float_t m_MC_pdf36 ;
  Float_t m_MC_pdf37 ;
  Float_t m_MC_pdf38 ;
  Float_t m_MC_pdf39 ;
  Float_t m_MC_pdf40 ;
  Float_t m_MC_pdf41 ;
  Float_t m_MC_pdf42 ;
  Float_t m_MC_pdf43 ;
  Float_t m_MC_pdf44 ;
  Float_t m_MC_pdf45 ;
  Float_t m_MC_pdf46 ;
  Float_t m_MC_pdf47 ;
  Float_t m_MC_pdf48 ;
  Float_t m_MC_pdf49 ;
  Float_t m_MC_pdf50 ;
  Float_t m_MC_pdf51 ;
  Float_t m_MC_pdf52 ;
  Float_t m_MC_pdf53 ;
  Float_t m_MC_pdf54 ;
  Float_t m_MC_pdf55 ;
  Float_t m_MC_pdf56 ;
  Float_t m_MC_pdf57 ;
  Float_t m_MC_pdf58 ;
  Float_t m_MC_pdf59 ;
  Float_t m_MC_pdf60 ;
  Float_t m_MC_pdf61 ;
  Float_t m_MC_pdf62 ;
  Float_t m_MC_pdf63 ;
  Float_t m_MC_pdf64 ;
  Float_t m_MC_pdf65 ;
  Float_t m_MC_pdf66 ;
  Float_t m_MC_pdf67 ;
  Float_t m_MC_pdf68 ;
  Float_t m_MC_pdf69 ;
  Float_t m_MC_pdf70 ;
  Float_t m_MC_pdf71 ;
  Float_t m_MC_pdf72 ;
  Float_t m_MC_pdf73 ;
  Float_t m_MC_pdf74 ;
  Float_t m_MC_pdf75 ;
  Float_t m_MC_pdf76 ;
  Float_t m_MC_pdf77 ;
  Float_t m_MC_pdf78 ;
  Float_t m_MC_pdf79 ;
  Float_t m_MC_pdf80 ;
  Float_t m_MC_pdf81 ;
  Float_t m_MC_pdf82 ;
  Float_t m_MC_pdf83 ;
  Float_t m_MC_pdf84 ;
  Float_t m_MC_pdf85 ;
  Float_t m_MC_pdf86 ;
  Float_t m_MC_pdf87 ;
  Float_t m_MC_pdf88 ;
  Float_t m_MC_pdf89 ;
  Float_t m_MC_pdf90 ;
  Float_t m_MC_pdf91 ;
  Float_t m_MC_pdf92 ;
  Float_t m_MC_pdf93 ;
  Float_t m_MC_pdf94 ;
  Float_t m_MC_pdf95 ;
  Float_t m_MC_pdf96 ;
  Float_t m_MC_pdf97 ;
  Float_t m_MC_pdf98 ;
  Float_t m_MC_pdf99 ;
  Float_t m_MC_pdf100 ;
  Float_t m_MC_astrong0 ;
  Float_t m_MC_astrong1 ;
  Float_t m_totalWeight ;
  Float_t m_prescaleWeight ;
  Float_t m_L1pref_weight ;
  Float_t m_L1pref_weight_up ;
  Float_t m_L1pref_weight_down ;
  Float_t m_PUjetID_SF ;
  Float_t m_PUjetID_SF_up ;
  Float_t m_PUjetID_SF_down ;
  Float_t m_PUReweight ;
  Float_t m_PUReweight_up ;
  Float_t m_PUReweight_down ;
  Float_t m_bTagweightL ;
  Float_t m_bTagweightL_up ;
  Float_t m_bTagweightL_down ;
  Float_t m_bTagweightM ;
  Float_t m_bTagweightM_up ;
  Float_t m_bTagweightM_down ;
  Float_t m_bTagweightT ;
  Float_t m_bTagweightT_up ;
  Float_t m_bTagweightT_down ;
  Float_t m_bTagweightReshape ;
  Float_t m_bTagweightReshape_jes_up ;
  Float_t m_bTagweightReshape_lf_up ;
  Float_t m_bTagweightReshape_hf_up ;
  Float_t m_bTagweightReshape_hfstats1_up ;
  Float_t m_bTagweightReshape_hfstats2_up ;
  Float_t m_bTagweightReshape_lfstats1_up ;
  Float_t m_bTagweightReshape_lfstats2_up ;
  Float_t m_bTagweightReshape_cferr1_up ;
  Float_t m_bTagweightReshape_cferr2_up ;
  Float_t m_bTagweightReshape_jes_down ;
  Float_t m_bTagweightReshape_lf_down ;
  Float_t m_bTagweightReshape_hf_down ;
  Float_t m_bTagweightReshape_hfstats1_down ;
  Float_t m_bTagweightReshape_hfstats2_down ;
  Float_t m_bTagweightReshape_lfstats1_down ;
  Float_t m_bTagweightReshape_lfstats2_down ;
  Float_t m_bTagweightReshape_cferr1_down ;
  Float_t m_bTagweightReshape_cferr2_down ;
  Float_t m_bTagweightReshape_jetup1 ;
  Float_t m_bTagweightReshape_jetup2 ;
  Float_t m_bTagweightReshape_jetup3 ;
  Float_t m_bTagweightReshape_jetup4 ;
  Float_t m_bTagweightReshape_jetup5 ;
  Float_t m_bTagweightReshape_jetup6 ;
  Float_t m_bTagweightReshape_jetup7 ;
  Float_t m_bTagweightReshape_jetup8 ;
  Float_t m_bTagweightReshape_jetup9 ;
  Float_t m_bTagweightReshape_jetup10 ;
  Float_t m_bTagweightReshape_jetup11 ;
  Float_t m_bTagweightReshape_jetdown1 ;
  Float_t m_bTagweightReshape_jetdown2 ;
  Float_t m_bTagweightReshape_jetdown3 ;
  Float_t m_bTagweightReshape_jetdown4 ;
  Float_t m_bTagweightReshape_jetdown5 ;
  Float_t m_bTagweightReshape_jetdown6 ;
  Float_t m_bTagweightReshape_jetdown7 ;
  Float_t m_bTagweightReshape_jetdown8 ;
  Float_t m_bTagweightReshape_jetdown9 ;
  Float_t m_bTagweightReshape_jetdown10 ;
  Float_t m_bTagweightReshape_jetdown11 ;
  Float_t m_TTtopPtreweight ;
  Float_t m_TTtopPtreweight_up ;
  Float_t m_TTtopPtreweight_down ;
  Float_t m_trigSF ;
  Float_t m_trigSFnoMET ;
  Float_t m_trigSFnoTau ;
  Float_t m_trigSF_ele_up ;
  Float_t m_trigSF_SL_ele_up ;
  Float_t m_trigSF_cross_ele_up ;
  Float_t m_trigSF_mu_up ;
  Float_t m_trigSF_SL_mu_up ;
  Float_t m_trigSF_cross_mu_up ;
  Float_t m_trigSF_tau_DM0_up ;
  Float_t m_trigSF_tau_DM1_up ;
  Float_t m_trigSF_tau_DM10_up ;
  Float_t m_trigSF_tau_DM11_up ;
  Float_t m_trigSF_ele_down ;
  Float_t m_trigSF_SL_ele_down ;
  Float_t m_trigSF_cross_ele_down ;
  Float_t m_trigSF_mu_down ;
  Float_t m_trigSF_SL_mu_down ;
  Float_t m_trigSF_cross_mu_down ;
  Float_t m_trigSF_tau_DM0_down ;
  Float_t m_trigSF_tau_DM1_down ;
  Float_t m_trigSF_tau_DM10_down ;
  Float_t m_trigSF_tau_DM11_down ;
  Float_t m_trigSF_met_up ;
  Float_t m_trigSF_met_down ;
  Float_t m_trigSF_stau_up ;
  Float_t m_trigSF_stau_down ;
  Float_t m_jetFakeSF ;
  Float_t m_IdSF_deep_2d ;
  Float_t m_IdSF_leg1_deep_vsJet_2d;
  Float_t m_IdSF_leg2_deep_vsJet_2d;
  Float_t m_dauSFs ;
  Float_t m_FakeRateSF_deep;

  Float_t m_dauSFs_tauid_2d_stat0_DM0_up;
  Float_t m_dauSFs_tauid_2d_stat0_DM0_down;
  Float_t m_dauSFs_tauid_2d_stat1_DM0_up;
  Float_t m_dauSFs_tauid_2d_stat1_DM0_down;
  Float_t m_dauSFs_tauid_2d_systuncorrdmeras_DM0_up;
  Float_t m_dauSFs_tauid_2d_systuncorrdmeras_DM0_down;

  Float_t m_dauSFs_tauid_2d_stat0_DM1_up;
  Float_t m_dauSFs_tauid_2d_stat0_DM1_down;
  Float_t m_dauSFs_tauid_2d_stat1_DM1_up;
  Float_t m_dauSFs_tauid_2d_stat1_DM1_down;
  Float_t m_dauSFs_tauid_2d_systuncorrdmeras_DM1_up;
  Float_t m_dauSFs_tauid_2d_systuncorrdmeras_DM1_down;

  Float_t m_dauSFs_tauid_2d_stat0_DM10_up;
  Float_t m_dauSFs_tauid_2d_stat0_DM10_down;
  Float_t m_dauSFs_tauid_2d_stat1_DM10_up;
  Float_t m_dauSFs_tauid_2d_stat1_DM10_down;
  Float_t m_dauSFs_tauid_2d_systuncorrdmeras_DM10_up;
  Float_t m_dauSFs_tauid_2d_systuncorrdmeras_DM10_down;

  Float_t m_dauSFs_tauid_2d_stat0_DM11_up;
  Float_t m_dauSFs_tauid_2d_stat0_DM11_down;
  Float_t m_dauSFs_tauid_2d_stat1_DM11_up;
  Float_t m_dauSFs_tauid_2d_stat1_DM11_down;
  Float_t m_dauSFs_tauid_2d_systuncorrdmeras_DM11_up;
  Float_t m_dauSFs_tauid_2d_systuncorrdmeras_DM11_down;

  Float_t m_dauSFs_tauid_2d_systcorrdmeras_up;
  Float_t m_dauSFs_tauid_2d_systcorrdmeras_down;
  Float_t m_dauSFs_tauid_2d_systcorrdmuncorreras_up;
  Float_t m_dauSFs_tauid_2d_systcorrdmuncorreras_down;
  Float_t m_dauSFs_tauid_2d_systcorrerasgt140_up;
  Float_t m_dauSFs_tauid_2d_systcorrerasgt140_down;
  Float_t m_dauSFs_tauid_2d_stat0gt140_up;
  Float_t m_dauSFs_tauid_2d_stat0gt140_down;
  Float_t m_dauSFs_tauid_2d_stat1gt140_up;
  Float_t m_dauSFs_tauid_2d_stat1gt140_down;
  Float_t m_dauSFs_tauid_2d_extrapgt140_up;
  Float_t m_dauSFs_tauid_2d_extrapgt140_down;

  Float_t m_dauSFs_mutauFR_etaLt0p4_up;
  Float_t m_dauSFs_mutauFR_eta0p4to0p8_up;
  Float_t m_dauSFs_mutauFR_eta0p8to1p2_up;
  Float_t m_dauSFs_mutauFR_eta1p2to1p7_up;
  Float_t m_dauSFs_mutauFR_etaGt1p7_up;
  Float_t m_dauSFs_mutauFR_etaLt0p4_down;
  Float_t m_dauSFs_mutauFR_eta0p4to0p8_down;
  Float_t m_dauSFs_mutauFR_eta0p8to1p2_down;
  Float_t m_dauSFs_mutauFR_eta1p2to1p7_down;
  Float_t m_dauSFs_mutauFR_etaGt1p7_down;

  Float_t m_dauSFs_etauFR_barrel_up;
  Float_t m_dauSFs_etauFR_endcap_up;
  Float_t m_dauSFs_etauFR_barrel_down;
  Float_t m_dauSFs_etauFR_endcap_down;

  Float_t m_dauSFs_muID_up;
  Float_t m_dauSFs_muID_down;
  Float_t m_dauSFs_muIso_up;
  Float_t m_dauSFs_muIso_down;

  Float_t m_dauSFs_eleID_up;
  Float_t m_dauSFs_eleID_down;
  Float_t m_dauSFs_eleReco_up;
  Float_t m_dauSFs_eleReco_down;

  Int_t m_lheNOutPartons ;
  Int_t m_lheNOutB ;
  ULong64_t m_EventNumber ;
  Int_t m_RunNumber ;
  Int_t m_isBoosted ;

  Int_t m_isLeptrigger;
  Int_t m_isMETtrigger;
  Int_t m_isSingleTautrigger;

  Int_t m_genDecMode1 ;
  Int_t m_genDecMode2 ;
  Float_t m_genMHH ;
  Float_t m_genCosth ;

  Int_t   m_npv ;
  Float_t m_npu ;
  Int_t m_lumi ;
  Long64_t m_triggerbit ;
  int m_pass_triggerbit ;
  int m_legacyAccept ;
  int m_metAccept ;
  int m_singletauAccept ;
  Bool_t m_cross_monitoring_trig ;
  Float_t m_rho ;

  Int_t m_pairType ;
  Int_t m_isMC ;
  Int_t m_isOS ;

  Float_t m_met_phi ;
  Float_t m_met_et ;
  Float_t m_METx ;
  Float_t m_METy ;

  Float_t m_metnomu_phi;
  Float_t m_metnomu_et;
  Float_t m_METnoMux;
  Float_t m_METnoMuy;
  Float_t m_mht_phi;
  Float_t m_mht_et;
  Float_t m_MHTx;
  Float_t m_MHTy;
  Float_t m_mhtnomu_phi;
  Float_t m_mhtnomu_et;
  Float_t m_MHTnoMux;
  Float_t m_MHTnoMuy;

  Float_t m_METx_jetup1, m_METx_jetup2, m_METx_jetup3, m_METx_jetup4, m_METx_jetup5, m_METx_jetup6, m_METx_jetup7, m_METx_jetup8, m_METx_jetup9, m_METx_jetup10, m_METx_jetup11;
  Float_t m_METy_jetup1, m_METy_jetup2, m_METy_jetup3, m_METy_jetup4, m_METy_jetup5, m_METy_jetup6, m_METy_jetup7, m_METy_jetup8, m_METy_jetup9, m_METy_jetup10, m_METy_jetup11;
  Float_t m_METx_jetdown1, m_METx_jetdown2, m_METx_jetdown3, m_METx_jetdown4, m_METx_jetdown5, m_METx_jetdown6, m_METx_jetdown7, m_METx_jetdown8, m_METx_jetdown9, m_METx_jetdown10, m_METx_jetdown11;
  Float_t m_METy_jetdown1, m_METy_jetdown2, m_METy_jetdown3, m_METy_jetdown4, m_METy_jetdown5, m_METy_jetdown6, m_METy_jetdown7, m_METy_jetdown8, m_METy_jetdown9, m_METy_jetdown10, m_METy_jetdown11;

  Float_t m_METx_tauup_DM0, m_METx_tauup_DM1, m_METx_tauup_DM10, m_METx_tauup_DM11;
  Float_t m_METy_tauup_DM0, m_METy_tauup_DM1, m_METy_tauup_DM10, m_METy_tauup_DM11;
  Float_t m_METx_taudown_DM0, m_METx_taudown_DM1, m_METx_taudown_DM10, m_METx_taudown_DM11;
  Float_t m_METy_taudown_DM0, m_METy_taudown_DM1, m_METy_taudown_DM10, m_METy_taudown_DM11;

  Float_t m_METx_eleup_DM0, m_METx_eleup_DM1;
  Float_t m_METy_eleup_DM0, m_METy_eleup_DM1;
  Float_t m_METx_eledown_DM0, m_METx_eledown_DM1;
  Float_t m_METy_eledown_DM0, m_METy_eledown_DM1;

  Float_t m_METx_eesUp, m_METx_eesDown;
  Float_t m_METx_eerUp, m_METx_eerDown;
  Float_t m_METy_eesUp, m_METy_eesDown;
  Float_t m_METy_eerUp, m_METy_eerDown;

  Float_t m_METx_muup;
  Float_t m_METx_mudown;
  Float_t m_METy_muup;
  Float_t m_METy_mudown;
  
  Float_t m_METx_jetupTot;
  Float_t m_METx_jetdownTot;
  Float_t m_METy_jetupTot;
  Float_t m_METy_jetdownTot;

  Float_t m_met_et_corr;
  Float_t m_met_cov00;
  Float_t m_met_cov01;
  Float_t m_met_cov10;
  Float_t m_met_cov11;

  Float_t m_DeepMET_ResponseTune_px;
  Float_t m_DeepMET_ResponseTune_py;
  Float_t m_DeepMET_ResolutionTune_px;
  Float_t m_DeepMET_ResolutionTune_py;

  // the largest pT daughter visible candidate
  Float_t m_dau1_iso ;
  Int_t   m_dau1_eleMVAiso;
  Int_t   m_dau1_deepTauVsJet;
  Int_t   m_dau1_deepTauVsEle;
  Int_t   m_dau1_deepTauVsMu;

  Float_t m_dau1_pt ;
  Float_t m_dau1_pt_muup;
  Float_t m_dau1_pt_mudown;
  Float_t m_dau1_e_muup;
  Float_t m_dau1_e_mudown;

  Float_t m_dau1_pt_tauup_DM0, m_dau1_pt_tauup_DM1, m_dau1_pt_tauup_DM10, m_dau1_pt_tauup_DM11;
  Float_t m_dau1_pt_taudown_DM0, m_dau1_pt_taudown_DM1, m_dau1_pt_taudown_DM10, m_dau1_pt_taudown_DM11;
  Float_t m_dau1_pt_eleup_DM0, m_dau1_pt_eleup_DM1;
  Float_t m_dau1_pt_eledown_DM0, m_dau1_pt_eledown_DM1;
  Float_t m_dau1_pt_eesUp, m_dau1_pt_eesDown, m_dau1_pt_eerUp, m_dau1_pt_eerDown;
  Float_t m_dau1_e_tauup_DM0, m_dau1_e_tauup_DM1, m_dau1_e_tauup_DM10, m_dau1_e_tauup_DM11;
  Float_t m_dau1_e_taudown_DM0, m_dau1_e_taudown_DM1, m_dau1_e_taudown_DM10, m_dau1_e_taudown_DM11;
  Float_t m_dau1_e_eleup_DM0, m_dau1_e_eleup_DM1;
  Float_t m_dau1_e_eledown_DM0, m_dau1_e_eledown_DM1;
  Float_t m_dau1_e_eesUp, m_dau1_e_eesDown, m_dau1_e_eerUp, m_dau1_e_eerDown;
  
  Float_t m_dau1_eta ;
  Float_t m_dau1_phi ;
  Float_t m_dau1_e ;
  Float_t m_dau1_flav ; // let this contain also whether it's a hadronic dau
  Float_t m_dau1_dxy ;
  Float_t m_dau1_dz ;
  Int_t   m_dau1_decayMode ;

  Float_t m_genmatched1_pt ;
  Float_t m_genmatched1_eta ;
  Float_t m_genmatched1_phi ;
  Float_t m_genmatched1_e ;
  Float_t m_genmatched2_pt ;
  Float_t m_genmatched2_eta ;
  Float_t m_genmatched2_phi ;
  Float_t m_genmatched2_e ;

  Bool_t m_hasgenmatch1 ;
  Bool_t m_hasgenmatch2 ;

  // the smallest pT daughter visible candidate
  Float_t m_dau2_iso ;
  Int_t   m_dau2_deepTauVsJet;
  Int_t   m_dau2_deepTauVsEle;
  Int_t   m_dau2_deepTauVsMu;

  Float_t m_genH1_pt;
  Float_t m_genH1_eta;
  Float_t m_genH1_phi;
  Float_t m_genH1_e;
  Float_t m_genH2_pt;
  Float_t m_genH2_eta;
  Float_t m_genH2_phi;
  Float_t m_genH2_e;

  Float_t m_genB1_pt;
  Float_t m_genB1_eta;
  Float_t m_genB1_phi;
  Float_t m_genB1_e;
  Float_t m_genB2_pt;
  Float_t m_genB2_eta;
  Float_t m_genB2_phi;
  Float_t m_genB2_e;

  Float_t m_genBQuark1_pt;
  Float_t m_genBQuark1_eta;
  Float_t m_genBQuark1_phi;
  Float_t m_genBQuark1_e;
  Int_t m_genBQuark1_motherPdgId;
  Float_t m_genBQuark2_pt;
  Float_t m_genBQuark2_eta;
  Float_t m_genBQuark2_phi;
  Float_t m_genBQuark2_e;
  Int_t m_genBQuark2_motherPdgId;
  Float_t m_genBQuark3_pt;
  Float_t m_genBQuark3_eta;
  Float_t m_genBQuark3_phi;
  Float_t m_genBQuark3_e;
  Int_t m_genBQuark3_motherPdgId;
  Float_t m_genBQuark4_pt;
  Float_t m_genBQuark4_eta;
  Float_t m_genBQuark4_phi;
  Float_t m_genBQuark4_e;
  Int_t m_genBQuark4_motherPdgId;

  Float_t m_genLepton1_pt;
  Float_t m_genLepton1_eta;
  Float_t m_genLepton1_phi;
  Float_t m_genLepton1_e;
  Int_t m_genLepton1_pdgId;
  Int_t m_genLepton1_motherPdgId;
  Float_t m_genLepton2_pt;
  Float_t m_genLepton2_eta;
  Float_t m_genLepton2_phi;
  Float_t m_genLepton2_e;
  Int_t m_genLepton2_pdgId;
  Int_t m_genLepton2_motherPdgId;
  Float_t m_genLepton3_pt;
  Float_t m_genLepton3_eta;
  Float_t m_genLepton3_phi;
  Float_t m_genLepton3_e;
  Int_t m_genLepton3_pdgId;
  Int_t m_genLepton3_motherPdgId;
  Float_t m_genLepton4_pt;
  Float_t m_genLepton4_eta;
  Float_t m_genLepton4_phi;
  Float_t m_genLepton4_e;
  Int_t m_genLepton4_pdgId;
  Int_t m_genLepton4_motherPdgId;

  Float_t m_genNu1_pt;
  Float_t m_genNu1_eta;
  Float_t m_genNu1_phi;
  Float_t m_genNu1_e;
  Float_t m_genNu2_pt;
  Float_t m_genNu2_eta;
  Float_t m_genNu2_phi;
  Float_t m_genNu2_e;
  Float_t m_genNuNoMatch_pt;
  Float_t m_genNuNoMatch_eta;
  Float_t m_genNuNoMatch_phi;
  Float_t m_genNuNoMatch_e;
  Float_t m_genNuTot_pt;
  Float_t m_genNuTot_eta;
  Float_t m_genNuTot_phi;
  Float_t m_genNuTot_e;
  Float_t m_genNuTotWithNoMatch_pt;
  Float_t m_genNuTotWithNoMatch_eta;
  Float_t m_genNuTotWithNoMatch_phi;
  Float_t m_genNuTotWithNoMatch_e;
  Float_t m_recoGenTauH_pt;
  Float_t m_recoGenTauH_eta;
  Float_t m_recoGenTauH_phi;
  Float_t m_recoGenTauH_e;
  Float_t m_recoGenTauH_mass;
  Float_t m_recoGenWithNoMatchTauH_pt;
  Float_t m_recoGenWithNoMatchTauH_eta;
  Float_t m_recoGenWithNoMatchTauH_phi;
  Float_t m_recoGenWithNoMatchTauH_e;
  Float_t m_recoGenWithNoMatchTauH_mass;

  Float_t m_dau2_pt ;

  Float_t m_dau2_pt_muup;
  Float_t m_dau2_pt_mudown;
  Float_t m_dau2_e_muup;
  Float_t m_dau2_e_mudown;

  Float_t m_dau2_pt_tauup_DM0;
  Float_t m_dau2_pt_tauup_DM1;
  Float_t m_dau2_pt_tauup_DM10;
  Float_t m_dau2_pt_tauup_DM11;
  Float_t m_dau2_e_tauup_DM0;
  Float_t m_dau2_e_tauup_DM1;
  Float_t m_dau2_e_tauup_DM10;
  Float_t m_dau2_e_tauup_DM11;

  Float_t m_dau2_pt_taudown_DM0;
  Float_t m_dau2_pt_taudown_DM1;
  Float_t m_dau2_pt_taudown_DM10;
  Float_t m_dau2_pt_taudown_DM11;
  Float_t m_dau2_e_taudown_DM0;
  Float_t m_dau2_e_taudown_DM1;
  Float_t m_dau2_e_taudown_DM10;
  Float_t m_dau2_e_taudown_DM11;

  Float_t m_dau2_pt_eleup_DM0;
  Float_t m_dau2_pt_eleup_DM1;
  Float_t m_dau2_e_eleup_DM0;
  Float_t m_dau2_e_eleup_DM1;

  Float_t m_dau2_pt_eledown_DM0;
  Float_t m_dau2_pt_eledown_DM1;
  Float_t m_dau2_e_eledown_DM0;
  Float_t m_dau2_e_eledown_DM1;

  Float_t m_dau2_pt_eesUp, m_dau2_pt_eesDown, m_dau2_pt_eerUp, m_dau2_pt_eerDown;
  Float_t m_dau2_e_eesUp,  m_dau2_e_eesDown,  m_dau2_e_eerUp,  m_dau2_e_eerDown;
  
  Float_t m_dau2_eta;
  Float_t m_dau2_phi;
  Float_t m_dau2_e;
  Float_t m_dau2_flav; // let this contain also whether it's a hadronic dau
  Float_t m_dau2_dxy;
  Float_t m_dau2_dz;
  Int_t m_dau2_decayMode;

  // the largest pT b visible candidate /  highest deepJet score
  Float_t m_bjet1_pt;
  Float_t m_bjet1_eta;
  Float_t m_bjet1_phi;
  Float_t m_bjet1_e;
  Float_t m_bjet1_bID_deepFlavor;
  Float_t m_bjet1_cID_deepFlavor;
  Float_t m_bjet1_CvsL;
  Float_t m_bjet1_CvsB;
  Float_t m_bjet1_HHbtag;
  Float_t m_bjet1_pnet_bb;
  Float_t m_bjet1_pnet_cc;
  Float_t m_bjet1_pnet_b;
  Float_t m_bjet1_pnet_c;
  Float_t m_bjet1_pnet_g;
  Float_t m_bjet1_pnet_uds;
  Float_t m_bjet1_pnet_pu;
  Float_t m_bjet1_pnet_undef;
  Float_t m_bjet1_smearFactor;
  Float_t m_bjet1_bMVAID;
  Float_t m_bjet1_PUjetIDupdated;
  Int_t   m_bjet1_flav;

  Bool_t  m_bjet1_hasgenjet ;
  Float_t m_bjet1_JER ;
  std::vector <Float_t> m_bjet1_JER_jetup;
  std::vector <Float_t> m_bjet1_JER_jetdown;
  Float_t m_bjet1_JER_jetupTot ;
  Float_t m_bjet1_JER_jetdownTot ;

  Bool_t  m_bjet1_gen_matched;

  // the trailing pT b visible candidate  /  highest deepJet score
  Float_t m_bjet2_pt ;
  Float_t m_bjet2_eta ;
  Float_t m_bjet2_phi ;
  Float_t m_bjet2_e ;
  Float_t m_bjet2_bID_deepFlavor ;
  Float_t m_bjet2_cID_deepFlavor ;
  Float_t m_bjet2_CvsL;
  Float_t m_bjet2_CvsB;
  Float_t m_bjet2_HHbtag;
  Float_t m_bjet2_pnet_bb;
  Float_t m_bjet2_pnet_cc;
  Float_t m_bjet2_pnet_b;
  Float_t m_bjet2_pnet_c;
  Float_t m_bjet2_pnet_g;
  Float_t m_bjet2_pnet_uds;
  Float_t m_bjet2_pnet_pu;
  Float_t m_bjet2_pnet_undef;
  Float_t m_bjet2_smearFactor;
  Float_t m_bjet2_bMVAID ;
  Float_t m_bjet2_PUjetIDupdated;
  Int_t   m_bjet2_flav ;

  Bool_t  m_bjet2_hasgenjet ;
  Float_t m_bjet2_JER ;
  std::vector <Float_t> m_bjet2_JER_jetup;
  std::vector <Float_t> m_bjet2_JER_jetdown;
  Float_t m_bjet2_JER_jetupTot ;
  Float_t m_bjet2_JER_jetdownTot ;

  Bool_t  m_bjet2_gen_matched;

  Float_t m_bjets_bID_deepFlavor;
  Float_t m_bjets_cID_deepFlavor;
  Bool_t  m_bjets_gen_matched;
  // fat jets and subjet info
  Int_t   m_nfatjets ;
  Int_t   m_fatjet_highest_pT_bTag ;
  Float_t m_fatjet_pt ;
  Float_t m_fatjet_eta;
  Float_t m_fatjet_phi;
  Float_t m_fatjet_e  ;
  Float_t m_fatjet_bID_deepFlavor ;
  Float_t m_fatjet_massIndependentDeepDoubleBvLJetTags_probHbb;
  Float_t m_fatjet_deepDoubleBvLJetTags_probHbb;
  Float_t m_fatjet_deepBoostedJetTags_probHbb;
  Float_t m_fatjet_particleNetJetTags_probHbb;
  Float_t m_fatjet_particleNetDiscriminatorsJetTags_HbbvsQCD;
  Float_t m_fatjet_particleNetMDJetTags_probXbb;
  Float_t m_fatjet_particleNetMDJetTags_probXcc;
  Float_t m_fatjet_particleNetMDJetTags_probXqq;
  Float_t m_fatjet_particleNetMDJetTags_probQCD;
  Float_t m_fatjet_particleNetMDJetTags_mass;  
  Float_t m_fatjet_particleNetMDJetTags_score;  
  Float_t m_fatjet_filteredMass ;
  Float_t m_fatjet_prunedMass ;
  Float_t m_fatjet_trimmedMass ;
  Float_t m_fatjet_softdropMass ;
  Float_t m_fatjet_tau1 ;
  Float_t m_fatjet_tau2 ;
  Float_t m_fatjet_tau3 ;
  Int_t   m_fatjet_nsubjets ;
  Int_t   m_fatjet_hasMatchedSj ;
  Float_t m_dR_subj1_subj2 ;

  Float_t m_HHbregrsvfit_pt ;
  Float_t m_HHbregrsvfit_eta ;
  Float_t m_HHbregrsvfit_phi ;
  Float_t m_HHbregrsvfit_m ;
  Float_t m_fatjet_particleNetMDJetTags_HP_SF ;
  Float_t m_fatjet_particleNetMDJetTags_HP_SF_up ;
  Float_t m_fatjet_particleNetMDJetTags_HP_SF_down ;
  Float_t m_fatjet_particleNetMDJetTags_MP_SF ;
  Float_t m_fatjet_particleNetMDJetTags_MP_SF_up ;
  Float_t m_fatjet_particleNetMDJetTags_MP_SF_down ;
  Float_t m_fatjet_particleNetMDJetTags_LP_SF ;
  Float_t m_fatjet_particleNetMDJetTags_LP_SF_up ;
  Float_t m_fatjet_particleNetMDJetTags_LP_SF_down ;

  Float_t m_subjetjet1_pt ;
  Float_t m_subjetjet1_eta  ;
  Float_t m_subjetjet1_phi  ;
  Float_t m_subjetjet1_e  ;
  Float_t m_subjetjet1_bID_deepFlavor ;

  Float_t m_subjetjet2_pt ;
  Float_t m_subjetjet2_eta  ;
  Float_t m_subjetjet2_phi  ;
  Float_t m_subjetjet2_e  ;
  Float_t m_subjetjet2_bID_deepFlavor ;

  // the gen jet associated to the first b jet
  Float_t m_genjet1_pt ;
  Float_t m_genjet1_eta ;
  Float_t m_genjet1_phi ;
  Float_t m_genjet1_e ;

  // the gen jet associated to the second b jet
  Float_t m_genjet2_pt ;
  Float_t m_genjet2_eta ;
  Float_t m_genjet2_phi ;
  Float_t m_genjet2_e ;

  // the tautau-H candidate
  Float_t m_tauH_pt ;
  Float_t m_tauH_eta ;
  Float_t m_tauH_phi ;
  Float_t m_tauH_e ;
  Float_t m_tauH_mass ;

  Float_t m_tauH_SVFIT_mass ;
  Float_t m_tauH_SVFIT_pt ;
  Float_t m_tauH_SVFIT_eta ;
  Float_t m_tauH_SVFIT_phi ;
  Float_t m_tauH_SVFIT_METphi ;
  Float_t m_tauH_SVFIT_METrho ;

  // the bb-H candidate
  Float_t m_bH_pt ;
  Float_t m_bH_eta ;
  Float_t m_bH_phi ;
  Float_t m_bH_e ;
  Float_t m_bH_mass ;


  // the di-higgs candidate
  Float_t m_HH_pt ;
  Float_t m_HH_eta ;
  Float_t m_HH_phi ;
  Float_t m_HH_e ;
  Float_t m_HH_mass ;

  Float_t m_HHsvfit_pt ;
  Float_t m_HHsvfit_eta ;
  Float_t m_HHsvfit_phi ;
  Float_t m_HHsvfit_e ;
  Float_t m_HHsvfit_mass ;

  // the di-higgs candidate
  Float_t m_HHKin_mass ;
  Float_t m_HHKin_chi2 ;

  // angular variables
  Float_t m_HH_deltaPhi ;
  Float_t m_HH_deltaEta ;
  Float_t m_HH_deltaR ;
  Float_t m_HHsvfit_deltaPhi ;
  Float_t m_tauHMet_deltaPhi ;
  Float_t m_tauHsvfitMet_deltaPhi ;
  Float_t m_bHMet_deltaPhi ;
  Float_t m_ditau_deltaPhi ;
  Float_t m_ditau_deltaEta ;
  Float_t m_dib_deltaPhi ;
  Float_t m_dib_deltaEta ;
  Float_t m_ditau_deltaR   ;
  Float_t m_dib_deltaR     ;
  Float_t m_dib_deltaR_per_bHpt ;
  Float_t m_btau_deltaRmin ;
  Float_t m_btau_deltaRmax ;
  Float_t m_dau1MET_deltaphi ;
  Float_t m_dau2MET_deltaphi ;

  Float_t m_HT20 ;
  Float_t m_HT50 ;
  Float_t m_HT20Full ;
  Float_t m_jet20centrality ;

  Int_t m_HEM1516veto;

  // additional jets
  std::vector<Float_t> m_jets_pt ;
  std::vector<Float_t> m_jets_eta ;
  std::vector<Float_t> m_jets_phi ;
  std::vector<Float_t> m_jets_e ;
  std::vector<Float_t> m_jets_btag_deepFlavor ;
  std::vector<Float_t> m_jets_ctag_deepFlavor ;
  std::vector<Float_t> m_jets_CvsL ;
  std::vector<Float_t> m_jets_CvsB ;
  std::vector<Float_t> m_jets_HHbtag ;
  std::vector<Float_t> m_jets_pnet_bb;
  std::vector<Float_t> m_jets_pnet_cc;
  std::vector<Float_t> m_jets_pnet_b;
  std::vector<Float_t> m_jets_pnet_c;
  std::vector<Float_t> m_jets_pnet_g;
  std::vector<Float_t> m_jets_pnet_uds;
  std::vector<Float_t> m_jets_pnet_pu;
  std::vector<Float_t> m_jets_pnet_undef;
  std::vector<Float_t> m_smearFactor_AK4;
  std::vector<Float_t> m_smearFactor_AK8;
  std::vector<Int_t> m_jets_flav ;
  std::vector<Int_t> m_jets_isH ;
  std::vector<Bool_t> m_jets_hasgenjet ;
  Int_t m_njets ;
  Int_t m_addjets ;
  Int_t m_njets20 ;
  Int_t m_njets50 ;
  Int_t m_nbjetscand ;
  Int_t m_nbjets20 ;
  Int_t m_nbjets50 ;
  Int_t m_njetsBHadFlav ;
  Int_t m_njetsCHadFlav ;

  std::vector<Float_t> m_jets_jecUnc ;
  Float_t m_dau1_jecUnc;
  Float_t m_dau2_jecUnc;
  Float_t m_bjet1_jecUnc;
  Float_t m_bjet2_jecUnc;

  std::vector<Float_t> m_PUjetID;

  // additional leptons
  std::vector<Float_t> m_leps_pt ;
  std::vector<Float_t> m_leps_eta ;
  std::vector<Float_t> m_leps_phi ;
  std::vector<Float_t> m_leps_e ;
  std::vector<Int_t> m_leps_flav ;
  Int_t m_nleps ;

  std::vector <Float_t> m_bH_pt_jetup;
  std::vector <Float_t> m_bH_pt_jetdown;
  std::vector <Float_t> m_bH_mass_jetup;
  std::vector <Float_t> m_bH_mass_jetdown;

  Float_t m_bjet1_pt_jetup1;
  Float_t m_bjet1_pt_jetup2;
  Float_t m_bjet1_pt_jetup3;
  Float_t m_bjet1_pt_jetup4;
  Float_t m_bjet1_pt_jetup5;
  Float_t m_bjet1_pt_jetup6;
  Float_t m_bjet1_pt_jetup7;
  Float_t m_bjet1_pt_jetup8;
  Float_t m_bjet1_pt_jetup9;
  Float_t m_bjet1_pt_jetup10;
  Float_t m_bjet1_pt_jetup11;
  Float_t m_bjet1_e_jetup1;
  Float_t m_bjet1_e_jetup2;
  Float_t m_bjet1_e_jetup3;
  Float_t m_bjet1_e_jetup4;
  Float_t m_bjet1_e_jetup5;
  Float_t m_bjet1_e_jetup6;
  Float_t m_bjet1_e_jetup7;
  Float_t m_bjet1_e_jetup8;
  Float_t m_bjet1_e_jetup9;
  Float_t m_bjet1_e_jetup10;
  Float_t m_bjet1_e_jetup11;

  Float_t m_bjet1_pt_jetdown1;
  Float_t m_bjet1_pt_jetdown2;
  Float_t m_bjet1_pt_jetdown3;
  Float_t m_bjet1_pt_jetdown4;
  Float_t m_bjet1_pt_jetdown5;
  Float_t m_bjet1_pt_jetdown6;
  Float_t m_bjet1_pt_jetdown7;
  Float_t m_bjet1_pt_jetdown8;
  Float_t m_bjet1_pt_jetdown9;
  Float_t m_bjet1_pt_jetdown10;
  Float_t m_bjet1_pt_jetdown11;
  Float_t m_bjet1_e_jetdown1;
  Float_t m_bjet1_e_jetdown2;
  Float_t m_bjet1_e_jetdown3;
  Float_t m_bjet1_e_jetdown4;
  Float_t m_bjet1_e_jetdown5;
  Float_t m_bjet1_e_jetdown6;
  Float_t m_bjet1_e_jetdown7;
  Float_t m_bjet1_e_jetdown8;
  Float_t m_bjet1_e_jetdown9;
  Float_t m_bjet1_e_jetdown10;
  Float_t m_bjet1_e_jetdown11;

  Float_t m_bjet2_pt_jetup1;
  Float_t m_bjet2_pt_jetup2;
  Float_t m_bjet2_pt_jetup3;
  Float_t m_bjet2_pt_jetup4;
  Float_t m_bjet2_pt_jetup5;
  Float_t m_bjet2_pt_jetup6;
  Float_t m_bjet2_pt_jetup7;
  Float_t m_bjet2_pt_jetup8;
  Float_t m_bjet2_pt_jetup9;
  Float_t m_bjet2_pt_jetup10;
  Float_t m_bjet2_pt_jetup11;
  Float_t m_bjet2_e_jetup1;
  Float_t m_bjet2_e_jetup2;
  Float_t m_bjet2_e_jetup3;
  Float_t m_bjet2_e_jetup4;
  Float_t m_bjet2_e_jetup5;
  Float_t m_bjet2_e_jetup6;
  Float_t m_bjet2_e_jetup7;
  Float_t m_bjet2_e_jetup8;
  Float_t m_bjet2_e_jetup9;
  Float_t m_bjet2_e_jetup10;
  Float_t m_bjet2_e_jetup11;

  Float_t m_bjet2_pt_jetdown1;
  Float_t m_bjet2_pt_jetdown2;
  Float_t m_bjet2_pt_jetdown3;
  Float_t m_bjet2_pt_jetdown4;
  Float_t m_bjet2_pt_jetdown5;
  Float_t m_bjet2_pt_jetdown6;
  Float_t m_bjet2_pt_jetdown7;
  Float_t m_bjet2_pt_jetdown8;
  Float_t m_bjet2_pt_jetdown9;
  Float_t m_bjet2_pt_jetdown10;
  Float_t m_bjet2_pt_jetdown11;
  Float_t m_bjet2_e_jetdown1;
  Float_t m_bjet2_e_jetdown2;
  Float_t m_bjet2_e_jetdown3;
  Float_t m_bjet2_e_jetdown4;
  Float_t m_bjet2_e_jetdown5;
  Float_t m_bjet2_e_jetdown6;
  Float_t m_bjet2_e_jetdown7;
  Float_t m_bjet2_e_jetdown8;
  Float_t m_bjet2_e_jetdown9;
  Float_t m_bjet2_e_jetdown10;
  Float_t m_bjet2_e_jetdown11;

  Float_t m_fatjet_pt_jetup1;
  Float_t m_fatjet_pt_jetup2;
  Float_t m_fatjet_pt_jetup3;
  Float_t m_fatjet_pt_jetup4;
  Float_t m_fatjet_pt_jetup5;
  Float_t m_fatjet_pt_jetup6;
  Float_t m_fatjet_pt_jetup7;
  Float_t m_fatjet_pt_jetup8;
  Float_t m_fatjet_pt_jetup9;
  Float_t m_fatjet_pt_jetup10;
  Float_t m_fatjet_pt_jetup11;
  Float_t m_fatjet_e_jetup1;
  Float_t m_fatjet_e_jetup2;
  Float_t m_fatjet_e_jetup3;
  Float_t m_fatjet_e_jetup4;
  Float_t m_fatjet_e_jetup5;
  Float_t m_fatjet_e_jetup6;
  Float_t m_fatjet_e_jetup7;
  Float_t m_fatjet_e_jetup8;
  Float_t m_fatjet_e_jetup9;
  Float_t m_fatjet_e_jetup10;
  Float_t m_fatjet_e_jetup11;

  Float_t m_fatjet_pt_jetdown1;
  Float_t m_fatjet_pt_jetdown2;
  Float_t m_fatjet_pt_jetdown3;
  Float_t m_fatjet_pt_jetdown4;
  Float_t m_fatjet_pt_jetdown5;
  Float_t m_fatjet_pt_jetdown6;
  Float_t m_fatjet_pt_jetdown7;
  Float_t m_fatjet_pt_jetdown8;
  Float_t m_fatjet_pt_jetdown9;
  Float_t m_fatjet_pt_jetdown10;
  Float_t m_fatjet_pt_jetdown11;
  Float_t m_fatjet_e_jetdown1;
  Float_t m_fatjet_e_jetdown2;
  Float_t m_fatjet_e_jetdown3;
  Float_t m_fatjet_e_jetdown4;
  Float_t m_fatjet_e_jetdown5;
  Float_t m_fatjet_e_jetdown6;
  Float_t m_fatjet_e_jetdown7;
  Float_t m_fatjet_e_jetdown8;
  Float_t m_fatjet_e_jetdown9;
  Float_t m_fatjet_e_jetdown10;
  Float_t m_fatjet_e_jetdown11;

  Float_t m_bH_mass_jetup1;
  Float_t m_bH_mass_jetup2;
  Float_t m_bH_mass_jetup3;
  Float_t m_bH_mass_jetup4;
  Float_t m_bH_mass_jetup5;
  Float_t m_bH_mass_jetup6;
  Float_t m_bH_mass_jetup7;
  Float_t m_bH_mass_jetup8;
  Float_t m_bH_mass_jetup9;
  Float_t m_bH_mass_jetup10;
  Float_t m_bH_mass_jetup11;

  Float_t m_bH_mass_jetdown1;
  Float_t m_bH_mass_jetdown2;
  Float_t m_bH_mass_jetdown3;
  Float_t m_bH_mass_jetdown4;
  Float_t m_bH_mass_jetdown5;
  Float_t m_bH_mass_jetdown6;
  Float_t m_bH_mass_jetdown7;
  Float_t m_bH_mass_jetdown8;
  Float_t m_bH_mass_jetdown9;
  Float_t m_bH_mass_jetdown10;
  Float_t m_bH_mass_jetdown11;
  
  Float_t m_bjet1_pt_jetupTot;
  Float_t m_bjet1_pt_jetdownTot;
  Float_t m_bjet2_pt_jetupTot;
  Float_t m_bjet2_pt_jetdownTot;
  Float_t m_bjet1_e_jetupTot;
  Float_t m_bjet1_e_jetdownTot;
  Float_t m_bjet2_e_jetupTot;
  Float_t m_bjet2_e_jetdownTot;
  Float_t m_bH_mass_jetupTot;
  Float_t m_bH_mass_jetdownTot;
  Float_t m_fatjet_pt_jetupTot;
  Float_t m_fatjet_pt_jetdownTot;
  Float_t m_fatjet_e_jetupTot;
  Float_t m_fatjet_e_jetdownTot;

  Float_t m_HHkinsvfit_bHmass;
  Float_t m_HHkinsvfit_pt;
  Float_t m_HHkinsvfit_eta ;
  Float_t m_HHkinsvfit_phi ;
  Float_t m_HHkinsvfit_e;
  Float_t m_HHkinsvfit_m;

  Float_t m_lheht ;
  Float_t m_topReweight;

  // additional variables
  Float_t m_dau1_z;
  Float_t m_dau2_z;
  Float_t m_bjet1_z;
  Float_t m_bjet2_z;
  Float_t m_tauH_z;
  Float_t m_bH_z;
  Float_t m_HH_z;
  Float_t m_HH_zV;
  Float_t m_HH_A;

  // Angela's BDT missing variables
  Float_t m_tauH_MET_pt; //
  Float_t m_dau2_MET_deltaEta; //
  Float_t m_bH_MET_deltaEta; //
  Float_t m_bH_MET_deltaR; //
  Float_t m_bH_tauH_MET_deltaR; //
  Float_t m_ditau_deltaR_per_tauH_MET_pt; //

  // fake taus
  Int_t m_isTau1real;
  Int_t m_isTau2real;
  Int_t m_nRealTaus;

} ;

#endif
