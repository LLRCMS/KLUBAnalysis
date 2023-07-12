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
      m_totalWeight = -1. ;
      m_prescaleWeight = 1. ;
      m_L1pref_weight = 1.;
      m_PUjetID_SF = 1.;
      m_PUjetID_SF_up = 1.;
      m_PUjetID_SF_down = 1.;
      m_PUjetID_SF_eff_up = 1.;
      m_PUjetID_SF_eff_down = 1.;
      m_PUjetID_SF_mistag_up = 1.;
      m_PUjetID_SF_mistag_down = 1.;
      m_PUjetID_SF_eff_eta_s2p5_up = 1.;
      m_PUjetID_SF_eff_eta_s2p5_down = 1.;
      m_PUjetID_SF_mistag_eta_s2p5_up = 1.;
      m_PUjetID_SF_mistag_eta_s2p5_down = 1.;
      m_PUjetID_SF_eff_eta_l2p5_up = 1.;
      m_PUjetID_SF_eff_eta_l2p5_down = 1.;
      m_PUjetID_SF_mistag_eta_l2p5_up = 1.;
      m_PUjetID_SF_mistag_eta_l2p5_down = 1.;
      m_PUReweight = -1. ;
      m_bTagweightL = -1. ;
      m_bTagweightL_up = -1. ;
      m_bTagweightL_down = -1. ;
      m_bTagweightM = -1. ;
      m_bTagweightM_up = -1. ;
      m_bTagweightM_down = -1. ;
      m_bTagweightT = -1. ;
      m_bTagweightT_up = -1. ;
      m_bTagweightT_down = -1. ;
      m_bTagweightReshape = -1.;
      m_bTagweightReshape_jes_up = -1.;
      m_bTagweightReshape_lf_up = -1.;
      m_bTagweightReshape_hf_up = -1.;
      m_bTagweightReshape_hfstats1_up = -1.;
      m_bTagweightReshape_hfstats2_up = -1.;
      m_bTagweightReshape_lfstats1_up = -1.;
      m_bTagweightReshape_lfstats2_up = -1.;
      m_bTagweightReshape_cferr1_up = -1.;
      m_bTagweightReshape_cferr2_up = -1.;
      m_bTagweightReshape_jes_down = -1.;
      m_bTagweightReshape_lf_down = -1.;
      m_bTagweightReshape_hf_down = -1.;
      m_bTagweightReshape_hfstats1_down = -1.;
      m_bTagweightReshape_hfstats2_down = -1.;
      m_bTagweightReshape_lfstats1_down = -1.;
      m_bTagweightReshape_lfstats2_down = -1.;
      m_bTagweightReshape_cferr1_down = -1.;
      m_bTagweightReshape_cferr2_down = -1.;
      m_bTagweightReshape_jetup1    = -1.;
      m_bTagweightReshape_jetup2    = -1.;
      m_bTagweightReshape_jetup3    = -1.;
      m_bTagweightReshape_jetup4    = -1.;
      m_bTagweightReshape_jetup5    = -1.;
      m_bTagweightReshape_jetup6    = -1.;
      m_bTagweightReshape_jetup7    = -1.;
      m_bTagweightReshape_jetup8    = -1.;
      m_bTagweightReshape_jetup9    = -1.;
      m_bTagweightReshape_jetup10   = -1.;
      m_bTagweightReshape_jetup11   = -1.;
      m_bTagweightReshape_jetdown1  = -1.;
      m_bTagweightReshape_jetdown2  = -1.;
      m_bTagweightReshape_jetdown3  = -1.;
      m_bTagweightReshape_jetdown4  = -1.;
      m_bTagweightReshape_jetdown5  = -1.;
      m_bTagweightReshape_jetdown6  = -1.;
      m_bTagweightReshape_jetdown7  = -1.;
      m_bTagweightReshape_jetdown8  = -1.;
      m_bTagweightReshape_jetdown9  = -1.;
      m_bTagweightReshape_jetdown10 = -1.;
      m_bTagweightReshape_jetdown11 = -1.;
      m_TTtopPtreweight = -1. ;
      m_TTtopPtreweight_up   = -1. ;
      m_TTtopPtreweight_down = -1. ;
      m_EventNumber = 0 ;
      m_DYscale_LL = -1. ;
      m_DYscale_MM = -1. ;
      m_DYscale_MH = -1. ;
      m_DYscale_MTT = -1. ;
      m_DYscale_MTT_up   = -1. ;
      m_DYscale_MTT_down = -1. ;
      m_genZ_pt = -1.;
      m_trigSF = -1.;
      m_trigSF_ele_up = -1.;
      m_trigSF_mu_up = -1.;
      m_trigSF_DM0_up = -1.;
      m_trigSF_DM1_up = -1.;
      m_trigSF_DM10_up = -1.;
      m_trigSF_DM11_up = -1.;
      m_trigSF_ele_down = -1.;
      m_trigSF_mu_down = -1.;
      m_trigSF_DM0_down = -1.;
      m_trigSF_DM1_down = -1.;
      m_trigSF_DM10_down = -1.;
      m_trigSF_DM11_down = -1.;
      m_trigSF_single = -1.;
      m_trigSF_cross = -1.;
      m_VBFtrigSF = 1.;
      m_trigSF_vbfjet_up = 1.;
      m_trigSF_vbfjet_down = 1.;
      m_FakeRateSF = -1.;
      m_IdSF_deep_dm = -1.;
      m_IdSF_deep_pt = -1.;
	  m_IdSF_deep_2d = -1.;
	  m_IdSF_leg1_deep_vsJet_2d = -1;
	  m_IdSF_leg2_deep_vsJet_2d = -1;
      m_IdFakeSF_deep_dm = -1.;
      m_IdFakeSF_deep_pt = -1.;
	  m_IdFakeSF_deep_2d = -1.;
      m_FakeRateSF_deep = -1.;
      m_customTauIdSF = 1.;
      m_customTauIdSF_DM0_up    = 1.;
      m_customTauIdSF_DM0_down  = 1.;
      m_customTauIdSF_DM1_up    = 1.;
      m_customTauIdSF_DM1_down  = 1.;
      m_customTauIdSF_DM10_up   = 1.;
      m_customTauIdSF_DM10_down = 1.;
      m_customTauIdSF_DM11_up   = 1.;
      m_customTauIdSF_DM11_down = 1.;

	  m_idFakeSF_tauid_2d_stat0_up                  = 1.;
	  m_idFakeSF_tauid_2d_stat0_down                = 1.;
	  m_idFakeSF_tauid_2d_stat1_up                  = 1.;
	  m_idFakeSF_tauid_2d_stat1_down                = 1.;
	  m_idFakeSF_tauid_2d_systcorrdmeras_up         = 1.;
	  m_idFakeSF_tauid_2d_systcorrdmeras_down       = 1.;
	  m_idFakeSF_tauid_2d_systcorrdmuncorreras_up   = 1.;
	  m_idFakeSF_tauid_2d_systcorrdmuncorreras_down = 1.;
	  m_idFakeSF_tauid_2d_systuncorrdmeras_up       = 1.;
	  m_idFakeSF_tauid_2d_systuncorrdmeras_down     = 1.;
	  m_idFakeSF_tauid_2d_systcorrerasgt140_up		= 1.;
	  m_idFakeSF_tauid_2d_systcorrerasgt140_down	= 1.;
	  m_idFakeSF_tauid_2d_statgt140_up				= 1.;
	  m_idFakeSF_tauid_2d_statgt140_down			= 1.;
	  m_idFakeSF_tauid_2d_extrapgt140				= 1.;
	  
      m_idFakeSF_tauid_pt20to25_up        = -1.;
      m_idFakeSF_tauid_pt25to30_up        = -1.;
      m_idFakeSF_tauid_pt30to35_up        = -1.;
      m_idFakeSF_tauid_pt35to40_up        = -1.;
      m_idFakeSF_tauid_pt40toInf_up       = -1.;
      m_idFakeSF_tauid_pt20to25_down      = -1.;
      m_idFakeSF_tauid_pt25to30_down      = -1.;
      m_idFakeSF_tauid_pt30to35_down      = -1.;
      m_idFakeSF_tauid_pt35to40_down      = -1.;
      m_idFakeSF_tauid_pt40toInf_down     = -1.;
      m_idFakeSF_mutauFR_etaLt0p4_up      = -1.;
      m_idFakeSF_mutauFR_eta0p4to0p8_up   = -1.;
      m_idFakeSF_mutauFR_eta0p8to1p2_up   = -1.;
      m_idFakeSF_mutauFR_eta1p2to1p7_up   = -1.;
      m_idFakeSF_mutauFR_etaGt1p7_up      = -1.;
      m_idFakeSF_mutauFR_etaLt0p4_down    = -1.;
      m_idFakeSF_mutauFR_eta0p4to0p8_down = -1.;
      m_idFakeSF_mutauFR_eta0p8to1p2_down = -1.;
      m_idFakeSF_mutauFR_eta1p2to1p7_down = -1.;
      m_idFakeSF_mutauFR_etaGt1p7_down    = -1.;
      m_idFakeSF_etauFR_barrel_up         = -1.;
      m_idFakeSF_etauFR_endcap_up         = -1.;
      m_idFakeSF_etauFR_barrel_down       = -1.;
      m_idFakeSF_etauFR_endcap_down       = -1.;
      m_jetFakeSF = -1.;
      m_nBhadrons = -1;
      m_lheNOutPartons = -1;
      m_lheNOutB = -1;
      m_RunNumber = -1. ;
      m_isBoosted = -1 ;
      m_isVBF = 0 ;
      m_isVBFtrigger = 0;

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

      m_METx_jetup.clear()  ;
      m_METx_tauup.clear()  ;
      m_METx_eleup.clear()  ;
      m_METx_muup = 0. ;
      m_METx_jetupTot = 0. ;
      m_METy_jetup.clear()  ;
      m_METy_tauup.clear()  ;
      m_METy_eleup.clear()  ;
      m_METy_muup = 0. ;
      m_METy_jetupTot = 0. ;
      m_METx_jetdown.clear()  ;
      m_METx_taudown.clear()  ;
      m_METx_eledown.clear()  ;
      m_METx_mudown = 0. ;
      m_METx_jetdownTot = 0. ;
      m_METy_jetdown.clear()  ;
      m_METy_taudown.clear()  ;
      m_METy_eledown.clear()  ;
      m_METy_mudown = 0. ;
      m_METy_jetdownTot = 0. ;

      m_met_et_corr =-1.;
      m_met_cov00 = -1.;
      m_met_cov01 = -1.;
      m_met_cov10 = -1.;
      m_met_cov11 = -1.;

      m_mT1 = -1. ;
      m_mT2 = -1. ;

      m_DeepMET_ResponseTune_px = -999;
      m_DeepMET_ResponseTune_px_tauup.clear();
      m_DeepMET_ResponseTune_px_taudown.clear();
      m_DeepMET_ResponseTune_px_eleup.clear();
      m_DeepMET_ResponseTune_px_eledown.clear();
      m_DeepMET_ResponseTune_px_muup = -999;
      m_DeepMET_ResponseTune_px_mudown = -999;

      m_DeepMET_ResponseTune_py = -999;
      m_DeepMET_ResponseTune_py_tauup.clear();
      m_DeepMET_ResponseTune_py_taudown.clear();
      m_DeepMET_ResponseTune_py_eleup.clear();
      m_DeepMET_ResponseTune_py_eledown.clear();
      m_DeepMET_ResponseTune_py_muup = -999;
      m_DeepMET_ResponseTune_py_mudown = -999;

      m_DeepMET_ResolutionTune_px = -999;
      m_DeepMET_ResolutionTune_px_tauup.clear();
      m_DeepMET_ResolutionTune_px_taudown.clear();
      m_DeepMET_ResolutionTune_px_eleup.clear();
      m_DeepMET_ResolutionTune_px_eledown.clear();
      m_DeepMET_ResolutionTune_px_muup = -999;
      m_DeepMET_ResolutionTune_px_mudown = -999;

      m_DeepMET_ResolutionTune_py = -999;
      m_DeepMET_ResolutionTune_py_tauup.clear();
      m_DeepMET_ResolutionTune_py_taudown.clear();
      m_DeepMET_ResolutionTune_py_eleup.clear();
      m_DeepMET_ResolutionTune_py_eledown.clear();
      m_DeepMET_ResolutionTune_py_muup = -999;
      m_DeepMET_ResolutionTune_py_mudown = -999;

      m_dau1_iso  = -1. ;
      m_dau1_eleMVAiso = -1;
      m_dau1_CUTiso = -1 ;
      m_dau1_deepTauVsJet = -1;
      m_dau1_deepTauVsEle = -1;
      m_dau1_deepTauVsMu = -1;

      m_dau1_byLooseCombinedIsolationDeltaBetaCorr3Hits= false;
      m_dau1_byMediumCombinedIsolationDeltaBetaCorr3Hits= false;
      m_dau1_byTightCombinedIsolationDeltaBetaCorr3Hits= false;
      m_dau1_pt  = -1. ;
      m_dau1_pt_tauup.clear() ;
      m_dau1_pt_taudown.clear() ;
      m_dau1_mass_tauup.clear() ;
      m_dau1_mass_taudown.clear() ;
      m_dau1_pt_eleup.clear() ;
      m_dau1_pt_eledown.clear() ;
      m_dau1_mass_eleup.clear() ;
      m_dau1_mass_eledown.clear() ;

      m_dau1_pt_muup    = -1.;
      m_dau1_pt_mudown  = -1. ;
      m_dau1_mass_muup   = -1.;
      m_dau1_mass_mudown = -1.;

      m_dau1_pt_tauup_DM0  = -1. ;
      m_dau1_pt_tauup_DM1  = -1. ;
      m_dau1_pt_tauup_DM10  = -1. ;
      m_dau1_pt_tauup_DM11  = -1. ;

      m_dau1_pt_taudown_DM0  = -1. ;
      m_dau1_pt_taudown_DM1  = -1. ;
      m_dau1_pt_taudown_DM10  = -1. ;
      m_dau1_pt_taudown_DM11  = -1. ;

      m_dau1_pt_eleup_DM0  = -1. ;
      m_dau1_pt_eleup_DM1  = -1. ;

      m_dau1_pt_eledown_DM0  = -1. ;
      m_dau1_pt_eledown_DM1  = -1. ;

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
      m_dau2_CUTiso = -1 ;
      m_dau2_deepTauVsJet = -1;
      m_dau2_deepTauVsEle = -1;
      m_dau2_deepTauVsMu = -1;
      m_dau2_byLooseCombinedIsolationDeltaBetaCorr3Hits= false;
      m_dau2_byMediumCombinedIsolationDeltaBetaCorr3Hits= false;
      m_dau2_byTightCombinedIsolationDeltaBetaCorr3Hits= false;

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

      m_dau2_pt  = -1. ;

      m_dau2_pt_tauup.clear() ;
      m_dau2_pt_taudown.clear() ;
      m_dau2_mass_tauup.clear() ;
      m_dau2_mass_taudown.clear() ;
      m_dau2_pt_eleup.clear() ;
      m_dau2_pt_eledown.clear() ;
      m_dau2_mass_eleup.clear() ;
      m_dau2_mass_eledown.clear() ;

      m_dau2_pt_muup    = -1.;
      m_dau2_pt_mudown  = -1. ;
      m_dau2_mass_muup   = -1.;
      m_dau2_mass_mudown = -1.;

      m_dau2_pt_tauup_DM0  = -1. ;
      m_dau2_pt_tauup_DM1  = -1. ;
      m_dau2_pt_tauup_DM10  = -1. ;
      m_dau2_pt_tauup_DM11  = -1. ;

      m_dau2_pt_taudown_DM0  = -1. ;
      m_dau2_pt_taudown_DM1  = -1. ;
      m_dau2_pt_taudown_DM10  = -1. ;
      m_dau2_pt_taudown_DM11  = -1. ;

      m_dau2_pt_eleup_DM0  = -1. ;
      m_dau2_pt_eleup_DM1  = -1. ;

      m_dau2_pt_eledown_DM0  = -1. ;
      m_dau2_pt_eledown_DM1  = -1. ;

      m_dau2_eta  = -1. ;
      m_dau2_phi  = -1. ;
      m_dau2_e  = -1. ;
      m_dau2_flav  = -1. ;
      m_dau2_dxy  = -1. ;
      m_dau2_dz  = -1. ;
      m_dau2_decayMode  = -1 ;

      m_bjet1_pt  = -1. ;
      m_bjet1_eta  = -1. ;
      m_bjet1_phi  = -1. ;
      m_bjet1_e  = -1. ;
      m_bjet1_bID = -1. ;
      m_bjet1_bID_deepCSV = -1.;
      m_bjet1_bID_deepFlavor = -1.;
      m_bjet1_cID_deepFlavor = -1.;
      m_bjet1_CvsL = -1.;
      m_bjet1_CvsB = -1.;
      m_bjet1_HHbtag = -1.;
      m_bjet1_pnet_bb = -1.;
      m_bjet1_pnet_cc = -1.;
      m_bjet1_pnet_b = -1.;
      m_bjet1_pnet_c = -1.;
      m_bjet1_pnet_g = -1.;
      m_bjet1_pnet_uds = -1.;
      m_bjet1_pnet_pu = -1.;
      m_bjet1_pnet_undef = -1.;
      m_bjet1_smearFactor = -1.;
      m_bjet1_bMVAID = -1. ;
      m_bjet1_PUjetIDupdated = -99. ;
      m_bjet1_flav = -1 ;
      m_bjet1_pt_raw  = -1. ;
      m_bjet1_pt_raw_jetup.clear() ;
      m_bjet1_pt_raw_jetdown.clear() ;
      m_bjet1_mass_raw_jetup.clear() ;
      m_bjet1_mass_raw_jetdown.clear() ;

      m_bjet1_hasgenjet = false ;
      m_bjet1_JER = -1.;
      m_bjet1_JER_jetup.clear() ;
      m_bjet1_JER_jetdown.clear() ;
      m_bjet1_JER_jetupTot = -1.;
      m_bjet1_JER_jetdownTot = -1.;

      m_bjet1_gen_matched = false;

      m_bjet2_pt  = -1. ;
      m_bjet2_eta  = -1. ;
      m_bjet2_phi  = -1. ;
      m_bjet2_e  = -1. ;
      m_bjet2_bID = -1. ;
      m_bjet2_bID_deepCSV = -1.;
      m_bjet2_bID_deepFlavor = -1.;
      m_bjet2_cID_deepFlavor = -1.;
      m_bjet2_CvsL = -1.;
      m_bjet2_CvsB = -1.;
      m_bjet2_HHbtag = -1.;
      m_bjet2_pnet_bb = -1.;
      m_bjet2_pnet_cc = -1.;
      m_bjet2_pnet_b = -1.;
      m_bjet2_pnet_c = -1.;
      m_bjet2_pnet_g = -1.;
      m_bjet2_pnet_uds = -1.;
      m_bjet2_pnet_pu = -1.;
      m_bjet2_pnet_undef = -1.;
      m_bjet2_smearFactor = -1.;
      m_bjet2_bMVAID = -1. ;
      m_bjet2_PUjetIDupdated = -99. ;
      m_bjet2_flav = -1 ;
      m_bjet2_pt_raw  = -1. ;
      m_bjet2_pt_raw_jetup.clear() ;
      m_bjet2_pt_raw_jetdown.clear() ;
      m_bjet2_mass_raw_jetup.clear() ;
      m_bjet2_mass_raw_jetdown.clear() ;

      m_bjet2_hasgenjet = false ;
      m_bjet2_JER = -1.;
      m_bjet2_JER_jetup.clear() ;
      m_bjet2_JER_jetdown.clear() ;
      m_bjet2_JER_jetupTot = -1.;
      m_bjet2_JER_jetdownTot = -1.;

      m_bjet2_gen_matched = false;

      m_bjets_bID = -1.;
      m_bjets_bID_deepCSV = -1.;
      m_bjets_bID_deepFlavor = -1.;
      m_bjets_cID_deepFlavor = -1.;
      m_bjets_gen_matched = false;
      m_nfatjets = -1;
      m_fatjet_pt = -1. ;
      m_fatjet_eta  = -1. ;
      m_fatjet_phi  = -1. ;
      m_fatjet_e  = -1. ;
      m_fatjet_bID = -1. ;
      m_fatjet_bID_deepCSV = -1. ;
      m_fatjet_bID_deepFlavor = -1. ;
      m_fatjet_massIndependentDeepDoubleBvLJetTags_probHbb = -1.;
      m_fatjet_deepDoubleBvLJetTags_probHbb = -1.;
      m_fatjet_deepBoostedJetTags_probHbb = -1.;
      m_fatjet_particleNetJetTags_probHbb = -1.;
      m_fatjet_particleNetDiscriminatorsJetTags_HbbvsQCD = -1.;
      m_fatjet_filteredMass = -1. ;
      m_fatjet_prunedMass = -1. ;
      m_fatjet_trimmedMass = -1. ;
      m_fatjet_softdropMass = -1. ;
      m_fatjet_tau1 = -1. ;
      m_fatjet_tau2 = -1. ;
      m_fatjet_tau3 = -1. ;
      m_fatjet_nsubjets = -1;
      m_dR_subj1_subj2 = -1.;

      m_subjetjet1_pt = -1. ;
      m_subjetjet1_eta  = -1. ;
      m_subjetjet1_phi  = -1. ;
      m_subjetjet1_e  = -1. ;
      m_subjetjet1_bID = -1. ;
      m_subjetjet1_bID_deepCSV = -1. ;
      m_subjetjet1_bID_deepFlavor = -1. ;

      m_subjetjet2_pt = -1. ;
      m_subjetjet2_eta  = -1. ;
      m_subjetjet2_phi  = -1. ;
      m_subjetjet2_e  = -1. ;
      m_subjetjet2_bID = -1. ;
      m_subjetjet2_bID_deepCSV = -1. ;
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


      m_tauH_SVFIT_mass_up   = -1. ;
      m_tauH_SVFIT_mass_down = -1. ;
      m_tauH_SVFIT_mass_METup   = -1. ;
      m_tauH_SVFIT_mass_METdown = -1. ;

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
      m_HH_mass_raw = -1. ;

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
      m_ditau_deltaR_per_tauHsvfitpt = -1. ;
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

      m_MT2 = -1.;

      // m_MT2_covMtrxStatus = -1;
      // m_MT2_EDM = -1.;
      // m_MT2_hasConverged = -1;
      // m_MT2_status = -1;
      // m_MT2_ncalls = -1;
      // m_MT2_l = -1.;


      m_jets_pt.clear () ;
      m_jets_eta.clear () ;
      m_jets_phi.clear () ;
      m_jets_e.clear () ;
      m_jets_btag.clear () ;
      m_jets_btag_deepCSV.clear () ;
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
      m_jets_smearFactor.clear () ;
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

      //m_mvaValueMuTau = 0;
      //m_mvaValueTauTau = 0;
      //m_mvaValueETau = 0;

      m_bH_mass_raw = 0;
      m_bH_pt_raw = 0;

      m_bH_pt_raw_jetup.clear() ;
      m_bH_pt_raw_jetdown.clear() ;
      m_bH_mass_raw_jetup.clear() ;
      m_bH_mass_raw_jetdown.clear() ;
      m_bjet1_pt_raw_jetup1 = - 1.;
      m_bjet1_pt_raw_jetup2 = - 1.;
      m_bjet1_pt_raw_jetup3 = - 1.;
      m_bjet1_pt_raw_jetup4 = - 1.;
      m_bjet1_pt_raw_jetup5 = - 1.;
      m_bjet1_pt_raw_jetup6 = - 1.;
      m_bjet1_pt_raw_jetup7 = - 1.;
      m_bjet1_pt_raw_jetup8 = - 1.;
      m_bjet1_pt_raw_jetup9 = - 1.;
      m_bjet1_pt_raw_jetup10 = - 1.;
      m_bjet1_pt_raw_jetup11 = - 1.;

      m_bjet1_pt_raw_jetdown1 = - 1.;
      m_bjet1_pt_raw_jetdown2 = - 1.;
      m_bjet1_pt_raw_jetdown3 = - 1.;
      m_bjet1_pt_raw_jetdown4 = - 1.;
      m_bjet1_pt_raw_jetdown5 = - 1.;
      m_bjet1_pt_raw_jetdown6 = - 1.;
      m_bjet1_pt_raw_jetdown7 = - 1.;
      m_bjet1_pt_raw_jetdown8 = - 1.;
      m_bjet1_pt_raw_jetdown9 = - 1.;
      m_bjet1_pt_raw_jetdown10 = - 1.;
      m_bjet1_pt_raw_jetdown11 = - 1.;

      m_bjet2_pt_raw_jetup1 = - 1.;
      m_bjet2_pt_raw_jetup2 = - 1.;
      m_bjet2_pt_raw_jetup3 = - 1.;
      m_bjet2_pt_raw_jetup4 = - 1.;
      m_bjet2_pt_raw_jetup5 = - 1.;
      m_bjet2_pt_raw_jetup6 = - 1.;
      m_bjet2_pt_raw_jetup7 = - 1.;
      m_bjet2_pt_raw_jetup8 = - 1.;
      m_bjet2_pt_raw_jetup9 = - 1.;
      m_bjet2_pt_raw_jetup10 = - 1.;
      m_bjet2_pt_raw_jetup11 = - 1.;

      m_bjet2_pt_raw_jetdown1 = - 1.;
      m_bjet2_pt_raw_jetdown2 = - 1.;
      m_bjet2_pt_raw_jetdown3 = - 1.;
      m_bjet2_pt_raw_jetdown4 = - 1.;
      m_bjet2_pt_raw_jetdown5 = - 1.;
      m_bjet2_pt_raw_jetdown6 = - 1.;
      m_bjet2_pt_raw_jetdown7 = - 1.;
      m_bjet2_pt_raw_jetdown8 = - 1.;
      m_bjet2_pt_raw_jetdown9 = - 1.;
      m_bjet2_pt_raw_jetdown10 = - 1.;
      m_bjet2_pt_raw_jetdown11 = - 1.;


      m_bH_mass_raw_jetup1 = - 1.;
      m_bH_mass_raw_jetup2 = - 1.;
      m_bH_mass_raw_jetup3 = - 1.;
      m_bH_mass_raw_jetup4 = - 1.;
      m_bH_mass_raw_jetup5 = - 1.;
      m_bH_mass_raw_jetup6 = - 1.;
      m_bH_mass_raw_jetup7 = - 1.;
      m_bH_mass_raw_jetup8 = - 1.;
      m_bH_mass_raw_jetup9 = - 1.;
      m_bH_mass_raw_jetup10 = - 1.;
      m_bH_mass_raw_jetup11 = - 1.;

      m_bH_mass_raw_jetdown1 = - 1.;
      m_bH_mass_raw_jetdown2 = - 1.;
      m_bH_mass_raw_jetdown3 = - 1.;
      m_bH_mass_raw_jetdown4 = - 1.;
      m_bH_mass_raw_jetdown5 = - 1.;
      m_bH_mass_raw_jetdown6 = - 1.;
      m_bH_mass_raw_jetdown7 = - 1.;
      m_bH_mass_raw_jetdown8 = - 1.;
      m_bH_mass_raw_jetdown9 = - 1.;
      m_bH_mass_raw_jetdown10 = - 1.;
      m_bH_mass_raw_jetdown11 = - 1.;

      m_bjet1_pt_raw_jetupTot     = - 1.;
      m_bjet1_pt_raw_jetdownTot   = - 1.;
      m_bjet2_pt_raw_jetupTot     = - 1.;
      m_bjet2_pt_raw_jetdownTot   = - 1.;
      m_bjet1_mass_raw_jetupTot   = - 1.;
      m_bjet1_mass_raw_jetdownTot = - 1.;
      m_bjet2_mass_raw_jetupTot   = - 1.;
      m_bjet2_mass_raw_jetdownTot = - 1.;
      m_bH_mass_raw_jetupTot      = - 1.;
      m_bH_mass_raw_jetdownTot    = - 1.;

      m_HHKin_mass_raw = 0;
      m_HHKin_mass_raw_chi2 = 0;
      m_HHKin_mass_raw_convergence = 0;
      m_HHKin_mass_raw_prob = 0;
      m_HHKin_mass_raw_copy = 0;

      m_lheht = 0;

      m_topReweight=-1;


      //VBF
      m_VBFjet1_pt= -999. ;
      m_VBFjet1_pt_jetup.clear();
      m_VBFjet1_pt_jetdown.clear();
      m_VBFjet1_mass_jetup.clear();
      m_VBFjet1_mass_jetdown.clear();
      m_VBFjet1_pt_jetup1= -999. ;
      m_VBFjet1_pt_jetup2= -999. ;
      m_VBFjet1_pt_jetup3= -999. ;
      m_VBFjet1_pt_jetup4= -999. ;
      m_VBFjet1_pt_jetup5= -999. ;
      m_VBFjet1_pt_jetup6= -999. ;
      m_VBFjet1_pt_jetup7= -999. ;
      m_VBFjet1_pt_jetup8= -999. ;
      m_VBFjet1_pt_jetup9= -999. ;
      m_VBFjet1_pt_jetup10= -999. ;
      m_VBFjet1_pt_jetup11= -999. ;

      m_VBFjet1_pt_jetdown1= -999. ;
      m_VBFjet1_pt_jetdown2= -999. ;
      m_VBFjet1_pt_jetdown3= -999. ;
      m_VBFjet1_pt_jetdown4= -999. ;
      m_VBFjet1_pt_jetdown5= -999. ;
      m_VBFjet1_pt_jetdown6= -999. ;
      m_VBFjet1_pt_jetdown7= -999. ;
      m_VBFjet1_pt_jetdown8= -999. ;
      m_VBFjet1_pt_jetdown9= -999. ;
      m_VBFjet1_pt_jetdown10= -999. ;
      m_VBFjet1_pt_jetdown11= -999. ;

      m_VBFjet1_eta= -999. ;
      m_VBFjet1_phi= -999. ;
      m_VBFjet1_e= -999. ;

      m_VBFjet1_btag= -999. ;
      m_VBFjet1_btag_deepCSV= -999. ;
      m_VBFjet1_btag_deepFlavor= -999. ;
      m_VBFjet1_ctag_deepFlavor= -999. ;
      m_VBFjet1_CvsL = -999. ;
      m_VBFjet1_CvsB = -999. ;
      m_VBFjet1_HHbtag = -999.;
      m_VBFjet1_pnet_bb = -999.;
      m_VBFjet1_pnet_cc = -999.;
      m_VBFjet1_pnet_b = -999.;
      m_VBFjet1_pnet_c = -999.;
      m_VBFjet1_pnet_g = -999.;
      m_VBFjet1_pnet_uds = -999.;
      m_VBFjet1_pnet_pu = -999.;
      m_VBFjet1_pnet_undef = -999.;
      m_VBFjet1_smearFactor = -999.;
      m_VBFjet1_PUjetIDupdated= -99. ;
      m_VBFjet1_flav= 0 ;
      m_VBFjet1_hasgenjet= false ;

      m_VBFgenjet1_pt= -999. ;
      m_VBFgenjet1_eta= -999. ;
      m_VBFgenjet1_phi= -999. ;
      m_VBFgenjet1_e= -999. ;

      m_VBFjet2_pt= -999. ;
      m_VBFjet2_pt_jetup.clear();
      m_VBFjet2_pt_jetdown.clear();
      m_VBFjet2_mass_jetup.clear();
      m_VBFjet2_mass_jetdown.clear();
      m_VBFjet2_pt_jetup1= -999. ;
      m_VBFjet2_pt_jetup2= -999. ;
      m_VBFjet2_pt_jetup3= -999. ;
      m_VBFjet2_pt_jetup4= -999. ;
      m_VBFjet2_pt_jetup5= -999. ;
      m_VBFjet2_pt_jetup6= -999. ;
      m_VBFjet2_pt_jetup7= -999. ;
      m_VBFjet2_pt_jetup8= -999. ;
      m_VBFjet2_pt_jetup9= -999. ;
      m_VBFjet2_pt_jetup10= -999. ;
      m_VBFjet2_pt_jetup11= -999. ;

      m_VBFjet2_pt_jetdown1= -999. ;
      m_VBFjet2_pt_jetdown2= -999. ;
      m_VBFjet2_pt_jetdown3= -999. ;
      m_VBFjet2_pt_jetdown4= -999. ;
      m_VBFjet2_pt_jetdown5= -999. ;
      m_VBFjet2_pt_jetdown6= -999. ;
      m_VBFjet2_pt_jetdown7= -999. ;
      m_VBFjet2_pt_jetdown8= -999. ;
      m_VBFjet2_pt_jetdown9= -999. ;
      m_VBFjet2_pt_jetdown10= -999. ;
      m_VBFjet2_pt_jetdown11= -999. ;
      m_VBFjet2_eta= -999. ;
      m_VBFjet2_phi= -999. ;
      m_VBFjet2_e= -999. ;

      m_VBFjet2_btag= -999. ;
      m_VBFjet2_btag_deepCSV= -999. ;
      m_VBFjet2_btag_deepFlavor= -999. ;
      m_VBFjet2_ctag_deepFlavor= -999. ;
      m_VBFjet2_CvsL = -999.;
      m_VBFjet2_CvsB = -999.;
      m_VBFjet2_HHbtag = -999.;
      m_VBFjet2_pnet_bb = -999.;
      m_VBFjet2_pnet_cc = -999.;
      m_VBFjet2_pnet_b = -999.;
      m_VBFjet2_pnet_c = -999.;
      m_VBFjet2_pnet_g = -999.;
      m_VBFjet2_pnet_uds = -999.;
      m_VBFjet2_pnet_pu = -999.;
      m_VBFjet2_pnet_undef = -999.;
      m_VBFjet2_smearFactor = -999.;
      m_VBFjet2_PUjetIDupdated= -99. ;
      m_VBFjet2_flav= 0 ;
      m_VBFjet2_hasgenjet= false ;


      m_VBFgenjet2_pt= -999. ;
      m_VBFgenjet2_eta= -999. ;
      m_VBFgenjet2_phi= -999. ;
      m_VBFgenjet2_e= -999. ;
      m_VBFjj_deltaEta = -999.;

      m_VBFjj_mass = -999.;
      m_VBFjj_mass_jetup.clear();
      m_VBFjj_mass_jetdown.clear();

      m_VBFjj_mass_jetup1= -999. ;
      m_VBFjj_mass_jetup2= -999. ;
      m_VBFjj_mass_jetup3= -999. ;
      m_VBFjj_mass_jetup4= -999. ;
      m_VBFjj_mass_jetup5= -999. ;
      m_VBFjj_mass_jetup6= -999. ;
      m_VBFjj_mass_jetup7= -999. ;
      m_VBFjj_mass_jetup8= -999. ;
      m_VBFjj_mass_jetup9= -999. ;
      m_VBFjj_mass_jetup10= -999. ;
      m_VBFjj_mass_jetup11= -999. ;
      m_VBFjj_mass_jetdown1= -999. ;
      m_VBFjj_mass_jetdown2= -999. ;
      m_VBFjj_mass_jetdown3= -999. ;
      m_VBFjj_mass_jetdown4= -999. ;
      m_VBFjj_mass_jetdown5= -999. ;
      m_VBFjj_mass_jetdown6= -999. ;
      m_VBFjj_mass_jetdown7= -999. ;
      m_VBFjj_mass_jetdown8= -999. ;
      m_VBFjj_mass_jetdown9= -999. ;
      m_VBFjj_mass_jetdown10= -999. ;
      m_VBFjj_mass_jetdown11= -999. ;
      m_VBFjj_deltaEta = -999.;
      m_VBFjj_deltaPhi = -999.;

      m_VBFjj_HT  = -999.;

      m_VBFjet1_pt_jetupTot       = -999. ;
      m_VBFjet1_pt_jetdownTot     = -999. ;
      m_VBFjet2_pt_jetupTot       = -999. ;
      m_VBFjet2_pt_jetdownTot     = -999. ;
      m_VBFjet1_mass_jetupTot     = -999. ;
      m_VBFjet1_mass_jetdownTot   = -999. ;
      m_VBFjet2_mass_jetupTot     = -999. ;
      m_VBFjet2_mass_jetdownTot   = -999. ;
      m_VBFjj_mass_jetupTot       = -999. ;
      m_VBFjj_mass_jetdownTot     = -999. ;

      // Additional central (|eta| <= 2.4) jets that pass selection for b-candidates
      m_addJetCentr1_pt  = -999.;
      m_addJetCentr2_pt  = -999.;
      m_addJetCentr3_pt  = -999.;
      m_addJetCentr4_pt  = -999.;
      m_addJetCentr5_pt  = -999.;
      m_addJetCentr1_eta = -999.;
      m_addJetCentr2_eta = -999.;
      m_addJetCentr3_eta = -999.;
      m_addJetCentr4_eta = -999.;
      m_addJetCentr5_eta = -999.;
      m_addJetCentr1_phi = -999.;
      m_addJetCentr2_phi = -999.;
      m_addJetCentr3_phi = -999.;
      m_addJetCentr4_phi = -999.;
      m_addJetCentr5_phi = -999.;
      m_addJetCentr1_e   = -999.;
      m_addJetCentr2_e   = -999.;
      m_addJetCentr3_e   = -999.;
      m_addJetCentr4_e   = -999.;
      m_addJetCentr5_e   = -999.;
      m_addJetCentr1_btag_deepFlavor = -999.;
      m_addJetCentr2_btag_deepFlavor = -999.;
      m_addJetCentr3_btag_deepFlavor = -999.;
      m_addJetCentr4_btag_deepFlavor = -999.;
      m_addJetCentr5_btag_deepFlavor = -999.;
      m_addJetCentr1_CvsL            = -999.;
      m_addJetCentr2_CvsL            = -999.;
      m_addJetCentr3_CvsL            = -999.;
      m_addJetCentr4_CvsL            = -999.;
      m_addJetCentr5_CvsL            = -999.;
      m_addJetCentr1_CvsB            = -999.;
      m_addJetCentr2_CvsB            = -999.;
      m_addJetCentr3_CvsB            = -999.;
      m_addJetCentr4_CvsB            = -999.;
      m_addJetCentr5_CvsB            = -999.;
      m_addJetCentr1_HHbtag          = -999.;
      m_addJetCentr2_HHbtag          = -999.;
      m_addJetCentr3_HHbtag          = -999.;
      m_addJetCentr4_HHbtag          = -999.;
      m_addJetCentr5_HHbtag          = -999.;
      m_addJetCentr1_smearFactor     = -999.;
      m_addJetCentr2_smearFactor     = -999.;
      m_addJetCentr3_smearFactor     = -999.;
      m_addJetCentr4_smearFactor     = -999.;
      m_addJetCentr5_smearFactor     = -999.;
      m_addJetCentr1_pnet_bb = -999.;
      m_addJetCentr1_pnet_cc = -999.;
      m_addJetCentr1_pnet_b = -999.;
      m_addJetCentr1_pnet_c = -999.;
      m_addJetCentr1_pnet_g = -999.;
      m_addJetCentr1_pnet_uds = -999.;
      m_addJetCentr1_pnet_pu = -999.;
      m_addJetCentr1_pnet_undef = -999.;
      m_addJetCentr2_pnet_bb = -999.;
      m_addJetCentr2_pnet_cc = -999.;
      m_addJetCentr2_pnet_b = -999.;
      m_addJetCentr2_pnet_c = -999.;
      m_addJetCentr2_pnet_g = -999.;
      m_addJetCentr2_pnet_uds = -999.;
      m_addJetCentr2_pnet_pu = -999.;
      m_addJetCentr2_pnet_undef = -999.;
      m_addJetCentr3_pnet_bb = -999.;
      m_addJetCentr3_pnet_cc = -999.;
      m_addJetCentr3_pnet_b = -999.;
      m_addJetCentr3_pnet_c = -999.;
      m_addJetCentr3_pnet_g = -999.;
      m_addJetCentr3_pnet_uds = -999.;
      m_addJetCentr3_pnet_pu = -999.;
      m_addJetCentr3_pnet_undef = -999.;
      m_addJetCentr4_pnet_bb = -999.;
      m_addJetCentr4_pnet_cc = -999.;
      m_addJetCentr4_pnet_b = -999.;
      m_addJetCentr4_pnet_c = -999.;
      m_addJetCentr4_pnet_g = -999.;
      m_addJetCentr4_pnet_uds = -999.;
      m_addJetCentr4_pnet_pu = -999.;
      m_addJetCentr4_pnet_undef = -999.;
      m_addJetCentr5_pnet_bb = -999.;
      m_addJetCentr5_pnet_cc = -999.;
      m_addJetCentr5_pnet_b = -999.;
      m_addJetCentr5_pnet_c = -999.;
      m_addJetCentr5_pnet_g = -999.;
      m_addJetCentr5_pnet_uds = -999.;
      m_addJetCentr5_pnet_pu = -999.;
      m_addJetCentr5_pnet_undef = -999.;

      m_addJetCentr1_pt_jetup.clear();
      m_addJetCentr1_pt_jetdown.clear();
      m_addJetCentr1_mass_jetup.clear();
      m_addJetCentr1_mass_jetdown.clear();
      m_addJetCentr2_pt_jetup.clear();
      m_addJetCentr2_pt_jetdown.clear();
      m_addJetCentr2_mass_jetup.clear();
      m_addJetCentr2_mass_jetdown.clear();
      m_addJetCentr3_pt_jetup.clear();
      m_addJetCentr3_pt_jetdown.clear();
      m_addJetCentr3_mass_jetup.clear();
      m_addJetCentr3_mass_jetdown.clear();
      m_addJetCentr4_pt_jetup.clear();
      m_addJetCentr4_pt_jetdown.clear();
      m_addJetCentr4_mass_jetup.clear();
      m_addJetCentr4_mass_jetdown.clear();
      m_addJetCentr5_pt_jetup.clear();
      m_addJetCentr5_pt_jetdown.clear();
      m_addJetCentr5_mass_jetup.clear();
      m_addJetCentr5_mass_jetdown.clear();

      m_addJetCentr1_pt_jetupTot     = -999.;
      m_addJetCentr1_pt_jetdownTot   = -999.;
      m_addJetCentr1_mass_jetupTot   = -999.;
      m_addJetCentr1_mass_jetdownTot = -999.;
      m_addJetCentr2_pt_jetupTot     = -999.;
      m_addJetCentr2_pt_jetdownTot   = -999.;
      m_addJetCentr2_mass_jetupTot   = -999.;
      m_addJetCentr2_mass_jetdownTot = -999.;
      m_addJetCentr3_pt_jetupTot     = -999.;
      m_addJetCentr3_pt_jetdownTot   = -999.;
      m_addJetCentr3_mass_jetupTot   = -999.;
      m_addJetCentr3_mass_jetdownTot = -999.;
      m_addJetCentr4_pt_jetupTot     = -999.;
      m_addJetCentr4_pt_jetdownTot   = -999.;
      m_addJetCentr4_mass_jetupTot   = -999.;
      m_addJetCentr4_mass_jetdownTot = -999.;
      m_addJetCentr5_pt_jetupTot     = -999.;
      m_addJetCentr5_pt_jetdownTot   = -999.;
      m_addJetCentr5_mass_jetupTot   = -999.;
      m_addJetCentr5_mass_jetdownTot = -999.;

      // Additional forward (|eta| > 2.4) jets that pass selection for VBF-candidates
      m_addJetForw1_pt  = -999.;
      m_addJetForw2_pt  = -999.;
      m_addJetForw3_pt  = -999.;
      m_addJetForw4_pt  = -999.;
      m_addJetForw5_pt  = -999.;
      m_addJetForw1_eta = -999.;
      m_addJetForw2_eta = -999.;
      m_addJetForw3_eta = -999.;
      m_addJetForw4_eta = -999.;
      m_addJetForw5_eta = -999.;
      m_addJetForw1_phi = -999.;
      m_addJetForw2_phi = -999.;
      m_addJetForw3_phi = -999.;
      m_addJetForw4_phi = -999.;
      m_addJetForw5_phi = -999.;
      m_addJetForw1_e   = -999.;
      m_addJetForw2_e   = -999.;
      m_addJetForw3_e   = -999.;
      m_addJetForw4_e   = -999.;
      m_addJetForw5_e   = -999.;
      m_addJetForw1_smearFactor = -999.;
      m_addJetForw2_smearFactor = -999.;
      m_addJetForw3_smearFactor = -999.;
      m_addJetForw4_smearFactor = -999.;
      m_addJetForw5_smearFactor = -999.;

      m_addJetForw1_pt_jetup.clear();
      m_addJetForw1_pt_jetdown.clear();
      m_addJetForw1_mass_jetup.clear();
      m_addJetForw1_mass_jetdown.clear();
      m_addJetForw2_pt_jetup.clear();
      m_addJetForw2_pt_jetdown.clear();
      m_addJetForw2_mass_jetup.clear();
      m_addJetForw2_mass_jetdown.clear();
      m_addJetForw3_pt_jetup.clear();
      m_addJetForw3_pt_jetdown.clear();
      m_addJetForw3_mass_jetup.clear();
      m_addJetForw3_mass_jetdown.clear();
      m_addJetForw4_pt_jetup.clear();
      m_addJetForw4_pt_jetdown.clear();
      m_addJetForw4_mass_jetup.clear();
      m_addJetForw4_mass_jetdown.clear();
      m_addJetForw5_pt_jetup.clear();
      m_addJetForw5_pt_jetdown.clear();
      m_addJetForw5_mass_jetup.clear();
      m_addJetForw5_mass_jetdown.clear();

      m_addJetForw1_pt_jetupTot     = -999.;
      m_addJetForw1_pt_jetdownTot   = -999.;
      m_addJetForw1_mass_jetupTot   = -999.;
      m_addJetForw1_mass_jetdownTot = -999.;
      m_addJetForw2_pt_jetupTot     = -999.;
      m_addJetForw2_pt_jetdownTot   = -999.;
      m_addJetForw2_mass_jetupTot   = -999.;
      m_addJetForw2_mass_jetdownTot = -999.;
      m_addJetForw3_pt_jetupTot     = -999.;
      m_addJetForw3_pt_jetdownTot   = -999.;
      m_addJetForw3_mass_jetupTot   = -999.;
      m_addJetForw3_mass_jetdownTot = -999.;
      m_addJetForw4_pt_jetupTot     = -999.;
      m_addJetForw4_pt_jetdownTot   = -999.;
      m_addJetForw4_mass_jetupTot   = -999.;
      m_addJetForw4_mass_jetdownTot = -999.;
      m_addJetForw5_pt_jetupTot     = -999.;
      m_addJetForw5_pt_jetdownTot   = -999.;
      m_addJetForw5_mass_jetupTot   = -999.;
      m_addJetForw5_mass_jetdownTot = -999.;

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

      m_BDT_channel                   = -9. ;
      m_tauH_MET_pt                   = -9. ;
      m_dau2_MET_deltaEta             = -9. ;
      m_bH_MET_deltaEta               = -9. ;
      m_bH_MET_deltaR                 = -9. ;
      m_bH_tauH_MET_deltaR            = -9. ;
      m_ditau_deltaR_per_tauH_MET_pt  = -9. ;
      m_p_zeta                        = -9. ;
      m_p_zeta_visible                = -9. ;
      m_mT_tauH_MET                   = -9. ;
      m_mT_tauH_SVFIT_MET             = -9. ;
      m_mT_total                      = -9. ;
      m_BDT_ditau_deltaPhi            = -9. ;
      m_BDT_dib_abs_deltaPhi          = -9. ;
      m_BDT_dib_deltaPhi              = -9. ;
      m_BDT_dau1MET_deltaPhi          = -9. ;
      m_BDT_tauHsvfitMet_abs_deltaPhi = -9. ;
      m_BDT_tauHsvfitMet_deltaPhi     = -9. ;
      m_BDT_bHMet_deltaPhi            = -9. ;
      m_BDT_HHsvfit_abs_deltaPhi      = -9. ;
      m_BDT_HT20                      = 0. ;
      m_BDT_HT20_jetdown.clear();
      m_BDT_HT20_jetup.clear();
      m_BDT_HT20_jetdownTot           = 0. ;
      m_BDT_HT20_jetupTot             = 0. ;
      m_BDT_topPairMasses             = -999. ;
      m_BDT_topPairMasses2            = -999. ;
      m_BDT_MX                        = -999. ;
      m_BDT_bH_tauH_MET_InvMass       = -999. ;
      m_BDT_bH_tauH_SVFIT_InvMass     = -999. ;
      m_BDT_bH_tauH_InvMass           = -999. ;
      m_BDT_total_CalcPhi             = -9. ;
      m_BDT_ditau_CalcPhi             = -9. ;
      m_BDT_dib_CalcPhi               = -9. ;
      m_BDT_MET_tauH_SVFIT_cosTheta   = -9. ;
      m_BDT_MET_bH_cosTheta           = -9. ;
      m_BDT_b1_bH_cosTheta            = -9. ;
      m_BDT_tauH_SVFIT_reson_cosTheta = -9. ;

      // 4 variables for VBF BDT
      m_VBFjj_dEtaSign   = -99.;
      m_VBFjet2_PUjetID  = -99.;
      m_bH_VBF1_deltaEta = -99.;
      m_dib_dEtaSign     = -99.;


      // fake taus in MC
      m_isTau1real = -1; // -1 if data
      m_isTau2real = -1; // -1 if data
      m_nRealTaus = -1; // -1 if data

      return 0 ;
    }

  int init ()
    {
      m_smallT->Branch ("MC_weight", &m_MC_weight, "MC_weight/F") ;
      m_smallT->Branch ("totalWeight", &m_totalWeight, "totalWeight/F") ;
      m_smallT->Branch ("prescaleWeight", &m_prescaleWeight, "prescaleWeight/F") ;
      m_smallT->Branch ("L1pref_weight", &m_L1pref_weight, "L1pref_weight/F") ;
      m_smallT->Branch ("PUjetID_SF", &m_PUjetID_SF, "PUjetID_SF/F") ;
      m_smallT->Branch ("PUjetID_SF_up", &m_PUjetID_SF_up, "PUjetID_SF_up/F") ;
      m_smallT->Branch ("PUjetID_SF_down", &m_PUjetID_SF_down, "PUjetID_SF_down/F") ;
      m_smallT->Branch ("PUjetID_SF_eff_up", &m_PUjetID_SF_eff_up, "PUjetID_SF_eff_up/F") ;
      m_smallT->Branch ("PUjetID_SF_eff_down", &m_PUjetID_SF_eff_down, "PUjetID_SF_eff_down/F") ;
      m_smallT->Branch ("PUjetID_SF_mistag_up", &m_PUjetID_SF_mistag_up, "PUjetID_SF_mistag_up/F") ;
      m_smallT->Branch ("PUjetID_SF_mistag_down", &m_PUjetID_SF_mistag_down, "PUjetID_SF_mistag_down/F") ;
      m_smallT->Branch ("PUjetID_SF_eff_eta_s2p5_up", &m_PUjetID_SF_eff_eta_s2p5_up, "PUjetID_SF_eff_eta_s2p5_up/F") ;
      m_smallT->Branch ("PUjetID_SF_eff_eta_s2p5_down", &m_PUjetID_SF_eff_eta_s2p5_down, "PUjetID_SF_eff_eta_s2p5_down/F") ;
      m_smallT->Branch ("PUjetID_SF_mistag_eta_s2p5_up", &m_PUjetID_SF_mistag_eta_s2p5_up, "PUjetID_SF_mistag_eta_s2p5_up/F") ;
      m_smallT->Branch ("PUjetID_SF_mistag_eta_s2p5_down", &m_PUjetID_SF_mistag_eta_s2p5_down, "PUjetID_SF_mistag_eta_s2p5_down/F") ;
      m_smallT->Branch ("PUjetID_SF_eff_eta_l2p5_up", &m_PUjetID_SF_eff_eta_l2p5_up, "PUjetID_SF_eff_eta_l2p5_up/F") ;
      m_smallT->Branch ("PUjetID_SF_eff_eta_l2p5_down", &m_PUjetID_SF_eff_eta_l2p5_down, "PUjetID_SF_eff_eta_l2p5_down/F") ;
      m_smallT->Branch ("PUjetID_SF_mistag_eta_l2p5_up", &m_PUjetID_SF_mistag_eta_l2p5_up, "PUjetID_SF_mistag_eta_l2p5_up/F") ;
      m_smallT->Branch ("PUjetID_SF_mistag_eta_l2p5_down", &m_PUjetID_SF_mistag_eta_l2p5_down, "PUjetID_SF_mistag_eta_l2p5_down/F") ;
      m_smallT->Branch ("PUReweight", &m_PUReweight, "PUReweight/F") ;
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
      m_smallT->Branch ("trigSF", &m_trigSF, "trigSF/F") ;
      m_smallT->Branch ("trigSF_ele_up",  &m_trigSF_ele_up,  "trigSF_ele_up/F") ;
      m_smallT->Branch ("trigSF_mu_up",  &m_trigSF_mu_up,  "trigSF_mu_up/F") ;
      m_smallT->Branch ("trigSF_DM0_up",  &m_trigSF_DM0_up,  "trigSF_DM0_up/F") ;
      m_smallT->Branch ("trigSF_DM1_up",  &m_trigSF_DM1_up,  "trigSF_DM1_up/F") ;
      m_smallT->Branch ("trigSF_DM10_up", &m_trigSF_DM10_up, "trigSF_DM10_up/F") ;
      m_smallT->Branch ("trigSF_DM11_up", &m_trigSF_DM11_up, "trigSF_DM11_up/F") ;
      m_smallT->Branch ("trigSF_ele_down",  &m_trigSF_ele_down,  "trigSF_ele_down/F") ;
      m_smallT->Branch ("trigSF_mu_down",  &m_trigSF_mu_down,  "trigSF_mu_down/F") ;
      m_smallT->Branch ("trigSF_DM0_down",  &m_trigSF_DM0_down,  "trigSF_DM0_down/F") ;
      m_smallT->Branch ("trigSF_DM1_down",  &m_trigSF_DM1_down,  "trigSF_DM1_down/F") ;
      m_smallT->Branch ("trigSF_DM10_down", &m_trigSF_DM10_down, "trigSF_DM10_down/F") ;
      m_smallT->Branch ("trigSF_DM11_down", &m_trigSF_DM11_down, "trigSF_DM11_down/F") ;
      m_smallT->Branch ("trigSF_single", &m_trigSF_single, "trigSF_single/F") ;
      m_smallT->Branch ("trigSF_cross", &m_trigSF_cross, "trigSF_cross/F") ;
      m_smallT->Branch ("VBFtrigSF", &m_VBFtrigSF, "VBFtrigSF/F") ;
      m_smallT->Branch ("trigSF_vbfjet_up", &m_trigSF_vbfjet_up, "trigSF_vbfjet_up/F") ;
      m_smallT->Branch ("trigSF_vbfjet_down", &m_trigSF_vbfjet_down, "trigSF_vbfjet_down/F") ;
      m_smallT->Branch ("FakeRateSF", &m_FakeRateSF, "FakeRateSF/F") ;
      m_smallT->Branch ("jetFakeSF", &m_jetFakeSF, "jetFakeSF/F") ;
      m_smallT->Branch ("IdSF_deep_dm", &m_IdSF_deep_dm, "IdSF_deep_dm/F") ;
      m_smallT->Branch ("IdSF_deep_pt", &m_IdSF_deep_pt, "IdSF_deep_pt/F") ;
	  m_smallT->Branch ("IdSF_deep_2d", &m_IdSF_deep_2d, "IdSF_deep_2d/F") ;
	  m_smallT->Branch("IdSF_leg1_deep_vsJet_2d", &m_IdSF_leg1_deep_vsJet_2d,
					   "IdSF_leg1_deep_vsJet_2d/F") ;
	  m_smallT->Branch("IdSF_leg2_deep_vsJet_2d", &m_IdSF_leg2_deep_vsJet_2d,
					   "IdSF_leg2_deep_vsJet_2d/F") ;
      m_smallT->Branch ("IdFakeSF_deep_dm", &m_IdFakeSF_deep_dm, "IdFakeSF_deep_dm/F") ;
      m_smallT->Branch ("IdFakeSF_deep_pt", &m_IdFakeSF_deep_pt, "IdFakeSF_deep_pt/F") ;
	  m_smallT->Branch ("IdFakeSF_deep_2d", &m_IdFakeSF_deep_2d, "IdFakeSF_deep_2d/F") ;
      m_smallT->Branch ("FakeRateSF_deep", &m_FakeRateSF_deep, "FakeRateSF_deep/F") ;
      m_smallT->Branch ("customTauIdSF", &m_customTauIdSF, "customTauIdSF/F") ;
      m_smallT->Branch ("customTauIdSF_DM0_up"   , &m_customTauIdSF_DM0_up   , "customTauIdSF_DM0_up/F") ;
      m_smallT->Branch ("customTauIdSF_DM0_down" , &m_customTauIdSF_DM0_down , "customTauIdSF_DM0_down/F") ;
      m_smallT->Branch ("customTauIdSF_DM1_up"   , &m_customTauIdSF_DM1_up   , "customTauIdSF_DM1_up/F") ;
      m_smallT->Branch ("customTauIdSF_DM1_down" , &m_customTauIdSF_DM1_down , "customTauIdSF_DM1_down/F") ;
      m_smallT->Branch ("customTauIdSF_DM10_up"  , &m_customTauIdSF_DM10_up  , "customTauIdSF_DM10_up/F") ;
      m_smallT->Branch ("customTauIdSF_DM10_down", &m_customTauIdSF_DM10_down, "customTauIdSF_DM10_down/F") ;
      m_smallT->Branch ("customTauIdSF_DM11_up"  , &m_customTauIdSF_DM11_up  , "customTauIdSF_DM11_up/F") ;
      m_smallT->Branch ("customTauIdSF_DM11_down", &m_customTauIdSF_DM11_down, "customTauIdSF_DM11_down/F") ;

	  m_smallT->Branch("idFakeSF_tauid_2d_stat0_up", &m_idFakeSF_tauid_2d_stat0_up,
					   "idFakeSF_tauid_2d_stat0_up/F");
	  m_smallT->Branch("idFakeSF_tauid_2d_stat0_down", &m_idFakeSF_tauid_2d_stat0_down,
					   "idFakeSF_tauid_2d_stat0_down/F");
	  m_smallT->Branch("idFakeSF_tauid_2d_stat1_up", &m_idFakeSF_tauid_2d_stat1_up,
					   "idFakeSF_tauid_2d_stat1_up/F");
	  m_smallT->Branch("idFakeSF_tauid_2d_stat1_down", &m_idFakeSF_tauid_2d_stat1_down,
					   "idFakeSF_tauid_2d_stat1_down/F");
	  m_smallT->Branch("idFakeSF_tauid_2d_systcorrdmeras_up", &m_idFakeSF_tauid_2d_systcorrdmeras_up,
					   "idFakeSF_tauid_2d_systcorrdmeras_up/F");
	  m_smallT->Branch("idFakeSF_tauid_2d_systcorrdmeras_down", &m_idFakeSF_tauid_2d_systcorrdmeras_down,
					   "idFakeSF_tauid_2d_systcorrdmeras_down/F");
	  m_smallT->Branch("idFakeSF_tauid_2d_systcorrdmuncorreras_up", &m_idFakeSF_tauid_2d_systcorrdmuncorreras_up,
					   "idFakeSF_tauid_2d_systcorrdmuncorreras_up/F");
	  m_smallT->Branch("idFakeSF_tauid_2d_systcorrdmuncorreras_down", &m_idFakeSF_tauid_2d_systcorrdmuncorreras_down,
					   "idFakeSF_tauid_2d_systcorrdmuncorreras_down/F");
	  m_smallT->Branch("idFakeSF_tauid_2d_systuncorrdmeras_up", &m_idFakeSF_tauid_2d_systuncorrdmeras_up,
					   "idFakeSF_tauid_2d_systuncorrdmeras_up/F");
	  m_smallT->Branch("idFakeSF_tauid_2d_systuncorrdmeras_down", &m_idFakeSF_tauid_2d_systuncorrdmeras_down,
					   "idFakeSF_tauid_2d_systuncorrdmeras_down/F");
	  m_smallT->Branch("idFakeSF_tauid_2d_systcorrerasgt140_up", &m_idFakeSF_tauid_2d_systcorrerasgt140_up,
					   "idFakeSF_tauid_2d_systcorrerasgt140_up/F");
	  m_smallT->Branch("idFakeSF_tauid_2d_systcorrerasgt140_down", &m_idFakeSF_tauid_2d_systcorrerasgt140_down,
					   "idFakeSF_tauid_2d_systcorrerasgt140_down/F");
	  m_smallT->Branch("idFakeSF_tauid_2d_statgt140_up", &m_idFakeSF_tauid_2d_statgt140_up,
					   "idFakeSF_tauid_2d_statgt140_up/F");
	  m_smallT->Branch("idFakeSF_tauid_2d_statgt140_down", &m_idFakeSF_tauid_2d_statgt140_down,
					   "idFakeSF_tauid_2d_statgt140_down/F");
	  m_smallT->Branch("idFakeSF_tauid_2d_extrapgt140", &m_idFakeSF_tauid_2d_extrapgt140,
					   "idFakeSF_tauid_2d_extrapgt140/F");
		  
      m_smallT->Branch ("idFakeSF_tauid_pt20to25_up"       , &m_idFakeSF_tauid_pt20to25_up       , "idFakeSF_tauid_pt20to25_up/F");
      m_smallT->Branch ("idFakeSF_tauid_pt25to30_up"       , &m_idFakeSF_tauid_pt25to30_up       , "idFakeSF_tauid_pt25to30_up/F");
      m_smallT->Branch ("idFakeSF_tauid_pt30to35_up"       , &m_idFakeSF_tauid_pt30to35_up       , "idFakeSF_tauid_pt30to35_up/F");
      m_smallT->Branch ("idFakeSF_tauid_pt35to40_up"       , &m_idFakeSF_tauid_pt35to40_up       , "idFakeSF_tauid_pt35to40_up/F");
      m_smallT->Branch ("idFakeSF_tauid_pt40toInf_up"      , &m_idFakeSF_tauid_pt40toInf_up      , "idFakeSF_tauid_pt40toInf_up/F");
      m_smallT->Branch ("idFakeSF_tauid_pt20to25_down"     , &m_idFakeSF_tauid_pt20to25_down     , "idFakeSF_tauid_pt20to25_down/F");
      m_smallT->Branch ("idFakeSF_tauid_pt25to30_down"     , &m_idFakeSF_tauid_pt25to30_down     , "idFakeSF_tauid_pt25to30_down/F");
      m_smallT->Branch ("idFakeSF_tauid_pt30to35_down"     , &m_idFakeSF_tauid_pt30to35_down     , "idFakeSF_tauid_pt30to35_down/F");
      m_smallT->Branch ("idFakeSF_tauid_pt35to40_down"     , &m_idFakeSF_tauid_pt35to40_down     , "idFakeSF_tauid_pt35to40_down/F");
      m_smallT->Branch ("idFakeSF_tauid_pt40toInf_down"    , &m_idFakeSF_tauid_pt40toInf_down    , "idFakeSF_tauid_pt40toInf_down/F");
      m_smallT->Branch ("idFakeSF_mutauFR_etaLt0p4_up"     , &m_idFakeSF_mutauFR_etaLt0p4_up     , "idFakeSF_mutauFR_etaLt0p4_up/F");
      m_smallT->Branch ("idFakeSF_mutauFR_eta0p4to0p8_up"  , &m_idFakeSF_mutauFR_eta0p4to0p8_up  , "idFakeSF_mutauFR_eta0p4to0p8_up/F");
      m_smallT->Branch ("idFakeSF_mutauFR_eta0p8to1p2_up"  , &m_idFakeSF_mutauFR_eta0p8to1p2_up  , "idFakeSF_mutauFR_eta0p8to1p2_up/F");
      m_smallT->Branch ("idFakeSF_mutauFR_eta1p2to1p7_up"  , &m_idFakeSF_mutauFR_eta1p2to1p7_up  , "idFakeSF_mutauFR_eta1p2to1p7_up/F");
      m_smallT->Branch ("idFakeSF_mutauFR_etaGt1p7_up"     , &m_idFakeSF_mutauFR_etaGt1p7_up     , "idFakeSF_mutauFR_etaGt1p7_up/F");
      m_smallT->Branch ("idFakeSF_mutauFR_etaLt0p4_down"   , &m_idFakeSF_mutauFR_etaLt0p4_down   , "idFakeSF_mutauFR_etaLt0p4_down/F");
      m_smallT->Branch ("idFakeSF_mutauFR_eta0p4to0p8_down", &m_idFakeSF_mutauFR_eta0p4to0p8_down, "idFakeSF_mutauFR_eta0p4to0p8_down/F");
      m_smallT->Branch ("idFakeSF_mutauFR_eta0p8to1p2_down", &m_idFakeSF_mutauFR_eta0p8to1p2_down, "idFakeSF_mutauFR_eta0p8to1p2_down/F");
      m_smallT->Branch ("idFakeSF_mutauFR_eta1p2to1p7_down", &m_idFakeSF_mutauFR_eta1p2to1p7_down, "idFakeSF_mutauFR_eta1p2to1p7_down/F");
      m_smallT->Branch ("idFakeSF_mutauFR_etaGt1p7_down"   , &m_idFakeSF_mutauFR_etaGt1p7_down   , "idFakeSF_mutauFR_etaGt1p7_down/F");
      m_smallT->Branch ("idFakeSF_etauFR_barrel_up"        , &m_idFakeSF_etauFR_barrel_up        , "idFakeSF_etauFR_barrel_up/F");
      m_smallT->Branch ("idFakeSF_etauFR_endcap_up"        , &m_idFakeSF_etauFR_endcap_up        , "idFakeSF_etauFR_endcap_up/F");
      m_smallT->Branch ("idFakeSF_etauFR_barrel_down"      , &m_idFakeSF_etauFR_barrel_down      , "idFakeSF_etauFR_barrel_down/F");
      m_smallT->Branch ("idFakeSF_etauFR_endcap_down"      , &m_idFakeSF_etauFR_endcap_down      , "idFakeSF_etauFR_endcap_down/F");
      m_smallT->Branch ("DYscale_LL", &m_DYscale_LL, "DYscale_LL/F") ;
      m_smallT->Branch ("DYscale_MM", &m_DYscale_MM, "DYscale_MM/F") ;
      m_smallT->Branch ("DYscale_MH", &m_DYscale_MH, "DYscale_MH/F") ;
      m_smallT->Branch ("DYscale_MTT", &m_DYscale_MTT, "DYscale_MTT/F") ;
      m_smallT->Branch ("DYscale_MTT_up", &m_DYscale_MTT_up, "DYscale_MTT_up/F") ;
      m_smallT->Branch ("DYscale_MTT_down", &m_DYscale_MTT_down, "DYscale_MTT_down/F") ;
      m_smallT->Branch ("genZ_pt", &m_genZ_pt, "genZ_pt/F") ;
      m_smallT->Branch ("nBhadrons", &m_nBhadrons, "nBhadrons/I") ;
      m_smallT->Branch ("lheNOutPartons", &m_lheNOutPartons, "lheNOutPartons/I");
      m_smallT->Branch ("lheNOutB", &m_lheNOutB, "lheNOutB/I");
      m_smallT->Branch ("EventNumber", &m_EventNumber, "EventNumber/l") ;
      m_smallT->Branch ("RunNumber", &m_RunNumber, "RunNumber/I") ;
      m_smallT->Branch ("isBoosted", &m_isBoosted, "isBoosted/I") ;
      m_smallT->Branch ("isVBF", &m_isVBF, "isVBF/I") ;
      m_smallT->Branch ("isVBFtrigger", &m_isVBFtrigger, "isVBFtrigger/I") ;

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


      m_smallT->Branch ("METx_jetup",  &m_METx_jetup) ;
      m_smallT->Branch ("METx_tauup",  &m_METx_tauup) ;
      m_smallT->Branch ("METx_eleup",  &m_METx_eleup) ;
      m_smallT->Branch ("METx_muup",   &m_METx_muup, "METx_muup/F") ;
      m_smallT->Branch ("METx_jetupTot",   &m_METx_jetupTot, "METx_jetupTot/F") ;
      m_smallT->Branch ("METy_jetup",  &m_METy_jetup) ;
      m_smallT->Branch ("METy_tauup",  &m_METy_tauup) ;
      m_smallT->Branch ("METy_eleup",  &m_METy_eleup) ;
      m_smallT->Branch ("METy_muup",   &m_METy_muup, "METy_muup/F") ;
      m_smallT->Branch ("METy_jetupTot",   &m_METy_jetupTot, "METy_jetupTot/F") ;

      m_smallT->Branch ("METx_jetdown",  &m_METx_jetdown) ;
      m_smallT->Branch ("METx_taudown",  &m_METx_taudown) ;
      m_smallT->Branch ("METx_eledown",  &m_METx_eledown) ;
      m_smallT->Branch ("METx_mudown",  &m_METx_mudown, "METx_mudown/F") ;
      m_smallT->Branch ("METx_jetdownTot",   &m_METx_jetdownTot, "METx_jetdownTot/F") ;
      m_smallT->Branch ("METy_jetdown",  &m_METy_jetdown) ;
      m_smallT->Branch ("METy_taudown",  &m_METy_taudown) ;
      m_smallT->Branch ("METy_eledown",  &m_METy_eledown) ;
      m_smallT->Branch ("METy_mudown",  &m_METy_mudown, "METy_mudown/F") ;
      m_smallT->Branch ("METy_jetdownTot",   &m_METy_jetdownTot, "METy_jetdownTot/F") ;

      m_smallT->Branch ("met_et_corr", &m_met_et_corr, "met_et_corr/F") ;
      m_smallT->Branch ("met_cov00", &m_met_cov00, "met_cov00/F") ;
      m_smallT->Branch ("met_cov01", &m_met_cov01, "met_cov01/F") ;
      m_smallT->Branch ("met_cov10", &m_met_cov10, "met_cov10/F") ;
      m_smallT->Branch ("met_cov11", &m_met_cov11, "met_cov11/F") ;
      m_smallT->Branch ("mT1", &m_mT1, "mT1/F") ;
      m_smallT->Branch ("mT2", &m_mT2, "mT2/F") ;

      m_smallT->Branch("DeepMET_ResponseTune_px", &m_DeepMET_ResponseTune_px, "DeepMET_ResponseTune_px/F");
      m_smallT->Branch("DeepMET_ResponseTune_px_tauup", &m_DeepMET_ResponseTune_px_tauup);
      m_smallT->Branch("DeepMET_ResponseTune_px_taudown", &m_DeepMET_ResponseTune_px_taudown);
      m_smallT->Branch("DeepMET_ResponseTune_px_eleup", &m_DeepMET_ResponseTune_px_eleup);
      m_smallT->Branch("DeepMET_ResponseTune_px_eledown", &m_DeepMET_ResponseTune_px_eledown);
      m_smallT->Branch("DeepMET_ResponseTune_px_muup", &m_DeepMET_ResponseTune_px_muup, "DeepMET_ResponseTune_px_muup/F");
      m_smallT->Branch("DeepMET_ResponseTune_px_mudown", &m_DeepMET_ResponseTune_px_mudown, "DeepMET_ResponseTune_px_mudown/F");

      m_smallT->Branch("DeepMET_ResponseTune_py", &m_DeepMET_ResponseTune_py, "DeepMET_ResponseTune_py/F");
      m_smallT->Branch("DeepMET_ResponseTune_py_tauup", &m_DeepMET_ResponseTune_py_tauup);
      m_smallT->Branch("DeepMET_ResponseTune_py_taudown", &m_DeepMET_ResponseTune_py_taudown);
      m_smallT->Branch("DeepMET_ResponseTune_py_eleup", &m_DeepMET_ResponseTune_py_eleup);
      m_smallT->Branch("DeepMET_ResponseTune_py_eledown", &m_DeepMET_ResponseTune_py_eledown);
      m_smallT->Branch("DeepMET_ResponseTune_py_muup", &m_DeepMET_ResponseTune_py_muup, "DeepMET_ResponseTune_py_muup/F");
      m_smallT->Branch("DeepMET_ResponseTune_py_mudown", &m_DeepMET_ResponseTune_py_mudown, "DeepMET_ResponseTune_py_mudown/F");

      m_smallT->Branch("DeepMET_ResolutionTune_px", &m_DeepMET_ResolutionTune_px, "DeepMET_ResolutionTune_px");
      m_smallT->Branch("DeepMET_ResolutionTune_px_tauup", &m_DeepMET_ResolutionTune_px_tauup);
      m_smallT->Branch("DeepMET_ResolutionTune_px_taudown", &m_DeepMET_ResolutionTune_px_taudown);
      m_smallT->Branch("DeepMET_ResolutionTune_px_eleup", &m_DeepMET_ResolutionTune_px_eleup);
      m_smallT->Branch("DeepMET_ResolutionTune_px_eledown", &m_DeepMET_ResolutionTune_px_eledown);
      m_smallT->Branch("DeepMET_ResolutionTune_px_muup", &m_DeepMET_ResolutionTune_px_muup, "DeepMET_ResolutionTune_px_muup/F");
      m_smallT->Branch("DeepMET_ResolutionTune_px_mudown", &m_DeepMET_ResolutionTune_px_mudown, "DeepMET_ResolutionTune_px_mudown/F");

      m_smallT->Branch("DeepMET_ResolutionTune_py", &m_DeepMET_ResolutionTune_py, "DeepMET_ResolutionTune_py");
      m_smallT->Branch("DeepMET_ResolutionTune_py_tauup", &m_DeepMET_ResolutionTune_py_tauup);
      m_smallT->Branch("DeepMET_ResolutionTune_py_taudown", &m_DeepMET_ResolutionTune_py_taudown);
      m_smallT->Branch("DeepMET_ResolutionTune_py_eleup", &m_DeepMET_ResolutionTune_py_eleup);
      m_smallT->Branch("DeepMET_ResolutionTune_py_eledown", &m_DeepMET_ResolutionTune_py_eledown);
      m_smallT->Branch("DeepMET_ResolutionTune_py_muup", &m_DeepMET_ResolutionTune_py_muup, "DeepMET_ResolutionTune_py_muup/F");
      m_smallT->Branch("DeepMET_ResolutionTune_py_mudown", &m_DeepMET_ResolutionTune_py_mudown, "DeepMET_ResolutionTune_py_mudown/F");

      m_smallT->Branch ("dau1_iso", &m_dau1_iso, "dau1_iso/F") ;
      m_smallT->Branch ("dau1_eleMVAiso", &m_dau1_eleMVAiso, "dau1_eleMVAiso/I") ;
      m_smallT->Branch ("dau1_CUTiso", &m_dau1_CUTiso, "dau1_CUTiso/I") ;
      m_smallT->Branch ("dau1_deepTauVsJet",&m_dau1_deepTauVsJet,"dau1_deepTauVsJet/I");
      m_smallT->Branch ("dau1_deepTauVsEle",&m_dau1_deepTauVsEle,"dau1_deepTauVsEle/I");
      m_smallT->Branch ("dau1_deepTauVsMu",&m_dau1_deepTauVsMu,"dau1_deepTauVsMu/I");

      m_smallT->Branch ("dau1_byLooseCombinedIsolationDeltaBetaCorr3Hits", &m_dau1_byLooseCombinedIsolationDeltaBetaCorr3Hits, "dau1_byLooseCombinedIsolationDeltaBetaCorr3Hits/O") ;
      m_smallT->Branch ("dau1_byMediumCombinedIsolationDeltaBetaCorr3Hits", &m_dau1_byMediumCombinedIsolationDeltaBetaCorr3Hits, "dau1_byMediumCombinedIsolationDeltaBetaCorr3Hits/O") ;
      m_smallT->Branch ("dau1_byTightCombinedIsolationDeltaBetaCorr3Hits", &m_dau1_byTightCombinedIsolationDeltaBetaCorr3Hits, "dau1_byTightCombinedIsolationDeltaBetaCorr3Hits/O") ;
      m_smallT->Branch ("dau1_pt", &m_dau1_pt, "dau1_pt/F") ;
      m_smallT->Branch ("dau1_pt_tauup",    &m_dau1_pt_tauup);
      m_smallT->Branch ("dau1_pt_taudown",  &m_dau1_pt_taudown);
      m_smallT->Branch ("dau1_mass_tauup",  &m_dau1_mass_tauup);
      m_smallT->Branch ("dau1_mass_taudown",&m_dau1_mass_taudown);
      m_smallT->Branch ("dau1_pt_eleup",    &m_dau1_pt_eleup);
      m_smallT->Branch ("dau1_pt_eledown",  &m_dau1_pt_eledown);
      m_smallT->Branch ("dau1_mass_eleup",  &m_dau1_mass_eleup);
      m_smallT->Branch ("dau1_mass_eledown",&m_dau1_mass_eledown);
      m_smallT->Branch ("dau1_pt_muup",    &m_dau1_pt_muup, "dau1_pt_muup/F");
      m_smallT->Branch ("dau1_pt_mudown",  &m_dau1_pt_mudown, "dau1_pt_mudown/F");
      m_smallT->Branch ("dau1_mass_muup",  &m_dau1_mass_muup, "dau1_mass_muup/F");
      m_smallT->Branch ("dau1_mass_mudown",&m_dau1_mass_mudown, "dau1_pt_mudown/F");

      m_smallT->Branch ("dau1_pt_tauup_DM0" , &m_dau1_pt_tauup_DM0 , "dau1_pt_tauup_DM0/F") ;
      m_smallT->Branch ("dau1_pt_tauup_DM1" , &m_dau1_pt_tauup_DM1 , "dau1_pt_tauup_DM1/F") ;
      m_smallT->Branch ("dau1_pt_tauup_DM10", &m_dau1_pt_tauup_DM10, "dau1_pt_tauup_DM10/F") ;
      m_smallT->Branch ("dau1_pt_tauup_DM11", &m_dau1_pt_tauup_DM11, "dau1_pt_tauup_DM11/F") ;

      m_smallT->Branch ("dau1_pt_taudown_DM0" , &m_dau1_pt_taudown_DM0 , "dau1_pt_taudown_DM0/F") ;
      m_smallT->Branch ("dau1_pt_taudown_DM1" , &m_dau1_pt_taudown_DM1 , "dau1_pt_taudown_DM1/F") ;
      m_smallT->Branch ("dau1_pt_taudown_DM10", &m_dau1_pt_taudown_DM10, "dau1_pt_taudown_DM10/F") ;
      m_smallT->Branch ("dau1_pt_taudown_DM11", &m_dau1_pt_taudown_DM11, "dau1_pt_taudown_DM11/F") ;

      m_smallT->Branch ("dau1_pt_eleup_DM0", &m_dau1_pt_eleup_DM0 , "dau1_pt_eleup_DM0/F") ;
      m_smallT->Branch ("dau1_pt_eleup_DM1", &m_dau1_pt_eleup_DM1 , "dau1_pt_eleup_DM1/F") ;

      m_smallT->Branch ("dau1_pt_eledown_DM0", &m_dau1_pt_eledown_DM0 , "dau1_pt_eledown_DM0/F") ;
      m_smallT->Branch ("dau1_pt_eledown_DM1", &m_dau1_pt_eledown_DM1 , "dau1_pt_eledown_DM1/F") ;

      m_smallT->Branch ("dau1_eta", &m_dau1_eta, "dau1_eta/F") ;
      m_smallT->Branch ("dau1_phi", &m_dau1_phi, "dau1_phi/F") ;
      m_smallT->Branch ("dau1_e", &m_dau1_e, "dau1_e/F") ;
      m_smallT->Branch ("dau1_flav", &m_dau1_flav, "dau1_flav/F") ;
      m_smallT->Branch ("dau1_dxy", &m_dau1_dxy, "dau1_dxy/F") ;
      m_smallT->Branch ("dau1_dz", &m_dau1_dz, "dau1_dz/F") ;
      m_smallT->Branch ("dau1_decayMode", &m_dau1_decayMode, "dau1_decayMode/I") ;

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
      m_smallT->Branch ("dau2_CUTiso", &m_dau2_CUTiso, "dau2_CUTiso/I") ;
      m_smallT->Branch ("dau2_deepTauVsJet",&m_dau2_deepTauVsJet,"dau2_deepTauVsJet/I");
      m_smallT->Branch ("dau2_deepTauVsEle",&m_dau2_deepTauVsEle,"dau2_deepTauVsEle/I");
      m_smallT->Branch ("dau2_deepTauVsMu",&m_dau2_deepTauVsMu,"dau2_deepTauVsMu/I");
      m_smallT->Branch ("dau2_byLooseCombinedIsolationDeltaBetaCorr3Hits", &m_dau2_byLooseCombinedIsolationDeltaBetaCorr3Hits, "dau2_byLooseCombinedIsolationDeltaBetaCorr3Hits/O") ;
      m_smallT->Branch ("dau2_byMediumCombinedIsolationDeltaBetaCorr3Hits", &m_dau2_byMediumCombinedIsolationDeltaBetaCorr3Hits, "dau2_byMediumCombinedIsolationDeltaBetaCorr3Hits/O") ;
      m_smallT->Branch ("dau2_byTightCombinedIsolationDeltaBetaCorr3Hits", &m_dau2_byTightCombinedIsolationDeltaBetaCorr3Hits, "dau2_byTightCombinedIsolationDeltaBetaCorr3Hits/O") ;

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
      m_smallT->Branch ("dau2_pt_tauup",    &m_dau2_pt_tauup);
      m_smallT->Branch ("dau2_pt_taudown",  &m_dau2_pt_taudown);
      m_smallT->Branch ("dau2_mass_tauup",  &m_dau2_mass_tauup);
      m_smallT->Branch ("dau2_mass_taudown",&m_dau2_mass_taudown);
      m_smallT->Branch ("dau2_pt_eleup",    &m_dau2_pt_eleup);
      m_smallT->Branch ("dau2_pt_eledown",  &m_dau2_pt_eledown);
      m_smallT->Branch ("dau2_mass_eleup",  &m_dau2_mass_eleup);
      m_smallT->Branch ("dau2_mass_eledown",&m_dau2_mass_eledown);
      m_smallT->Branch ("dau2_pt_muup",    &m_dau2_pt_muup, "dau2_pt_muup/F");
      m_smallT->Branch ("dau2_pt_mudown",  &m_dau2_pt_mudown, "dau2_pt_mudown/F");
      m_smallT->Branch ("dau2_mass_muup",  &m_dau2_mass_muup, "dau2_mass_muup/F");
      m_smallT->Branch ("dau2_mass_mudown",&m_dau2_mass_mudown, "dau2_pt_mudown/F");

      m_smallT->Branch ("dau2_pt_tauup_DM0" , &m_dau2_pt_tauup_DM0 , "dau2_pt_tauup_DM0/F") ;
      m_smallT->Branch ("dau2_pt_tauup_DM1" , &m_dau2_pt_tauup_DM1 , "dau2_pt_tauup_DM1/F") ;
      m_smallT->Branch ("dau2_pt_tauup_DM10", &m_dau2_pt_tauup_DM10, "dau2_pt_tauup_DM10/F") ;
      m_smallT->Branch ("dau2_pt_tauup_DM11", &m_dau2_pt_tauup_DM11, "dau2_pt_tauup_DM11/F") ;

      m_smallT->Branch ("dau2_pt_taudown_DM0" , &m_dau2_pt_taudown_DM0 , "dau2_pt_taudown_DM0/F") ;
      m_smallT->Branch ("dau2_pt_taudown_DM1" , &m_dau2_pt_taudown_DM1 , "dau2_pt_taudown_DM1/F") ;
      m_smallT->Branch ("dau2_pt_taudown_DM10", &m_dau2_pt_taudown_DM10, "dau2_pt_taudown_DM10/F") ;
      m_smallT->Branch ("dau2_pt_taudown_DM11", &m_dau2_pt_taudown_DM11, "dau2_pt_taudown_DM11/F") ;

      m_smallT->Branch ("dau2_pt_eleup_DM0", &m_dau2_pt_eleup_DM0 , "dau2_pt_eleup_DM0/F") ;
      m_smallT->Branch ("dau2_pt_eleup_DM1", &m_dau2_pt_eleup_DM1 , "dau2_pt_eleup_DM1/F") ;

      m_smallT->Branch ("dau2_pt_eledown_DM0", &m_dau2_pt_eledown_DM0 , "dau2_pt_eledown_DM0/F") ;
      m_smallT->Branch ("dau2_pt_eledown_DM1", &m_dau2_pt_eledown_DM1 , "dau2_pt_eledown_DM1/F") ;

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
      m_smallT->Branch ("bjet1_bID", &m_bjet1_bID, "bjet1_bID/F") ;
      m_smallT->Branch ("bjet1_bID_deepCSV", &m_bjet1_bID_deepCSV, "bjet1_bID_deepCSV/F") ;
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
      m_smallT->Branch ("bjet1_pt_raw", &m_bjet1_pt_raw, "bjet1_pt_raw/F") ;

      m_smallT->Branch ("bjet1_pt_raw_jetup",     &m_bjet1_pt_raw_jetup) ;
      m_smallT->Branch ("bjet1_pt_raw_jetdown",   &m_bjet1_pt_raw_jetdown) ;
      m_smallT->Branch ("bjet1_mass_raw_jetup",   &m_bjet1_mass_raw_jetup) ;
      m_smallT->Branch ("bjet1_mass_raw_jetdown", &m_bjet1_mass_raw_jetdown) ;

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
      m_smallT->Branch ("bjet2_bID", &m_bjet2_bID, "bjet2m_bID/F") ;
      m_smallT->Branch ("bjet2_bID_deepCSV", &m_bjet2_bID_deepCSV, "bjet2_bID_deepCSV/F") ;
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
      m_smallT->Branch ("bjet2_pt_raw", &m_bjet2_pt_raw, "bjet2_pt_raw/F") ;

      m_smallT->Branch ("bjet2_pt_raw_jetup",     &m_bjet2_pt_raw_jetup) ;
      m_smallT->Branch ("bjet2_pt_raw_jetdown",   &m_bjet2_pt_raw_jetdown) ;
      m_smallT->Branch ("bjet2_mass_raw_jetup",   &m_bjet2_mass_raw_jetup) ;
      m_smallT->Branch ("bjet2_mass_raw_jetdown", &m_bjet2_mass_raw_jetdown) ;

      m_smallT->Branch ("bjet2_hasgenjet", &m_bjet2_hasgenjet, "bjet2_hasgenjet/O") ;
      m_smallT->Branch ("bjet2_JER",        &m_bjet2_JER, "bjet2_JER/F") ;
      m_smallT->Branch ("bjet2_JER_jetup",  &m_bjet2_JER_jetup) ;
      m_smallT->Branch ("bjet2_JER_jetdown",&m_bjet2_JER_jetdown) ;
      m_smallT->Branch ("bjet2_JER_jetupTot"  , &m_bjet2_JER_jetupTot  , "bjet2_JER_jetupTot/F") ;
      m_smallT->Branch ("bjet2_JER_jetdownTot", &m_bjet2_JER_jetdownTot, "bjet2_JER_jetdownTot/F") ;

      m_smallT->Branch ("bjet2_gen_matched", &m_bjet2_gen_matched, "bjet2_gen_matched/O") ;
      m_smallT->Branch ("bjets_bID",&m_bjets_bID,"bjets_bID/F");
      m_smallT->Branch ("bjets_bID_deepCSV",&m_bjets_bID_deepCSV,"bjets_bID_deepCSV/F");
      m_smallT->Branch ("bjets_bID_deepFlavor",&m_bjets_bID_deepFlavor,"bjets_bID_deepFlavor/F");
      m_smallT->Branch ("bjets_cID_deepFlavor",&m_bjets_cID_deepFlavor,"bjets_cID_deepFlavor/F");

      m_smallT->Branch ("bjets_gen_matched", &m_bjets_gen_matched, "bjets_gen_matched/O") ;

      m_smallT->Branch ("nfatjets", &m_nfatjets, "nfatjets/I");
      m_smallT->Branch ("fatjet_pt", &m_fatjet_pt, "fatjet_pt/F");
      m_smallT->Branch ("fatjet_eta", &m_fatjet_eta, "fatjet_eta/F");
      m_smallT->Branch ("fatjet_phi", &m_fatjet_phi, "fatjet_phi/F");
      m_smallT->Branch ("fatjet_e", &m_fatjet_e, "fatjet_e/F");
      m_smallT->Branch ("fatjet_bID", &m_fatjet_bID, "fatjet_bID/F");
      m_smallT->Branch ("fatjet_bID_deepCSV", &m_fatjet_bID_deepCSV, "fatjet_bID_deepCSV/F");
      m_smallT->Branch ("fatjet_bID_deepFlavor", &m_fatjet_bID_deepFlavor, "fatjet_bID_deepFlavor/F");
      m_smallT->Branch ("fatjet_massIndependentDeepDoubleBvLJetTags_probHbb", &m_fatjet_massIndependentDeepDoubleBvLJetTags_probHbb, "fatjet_massIndependentDeepDoubleBvLJetTags_probHbb/F");
      m_smallT->Branch ("fatjet_deepDoubleBvLJetTags_probHbb", &m_fatjet_deepDoubleBvLJetTags_probHbb, "fatjet_deepDoubleBvLJetTags_probHbb/F");
      m_smallT->Branch ("fatjet_deepBoostedJetTags_probHbb", &m_fatjet_deepBoostedJetTags_probHbb, "fatjet_deepBoostedJetTags_probHbb/F");
      m_smallT->Branch ("fatjet_particleNetJetTags_probHbb", &m_fatjet_particleNetJetTags_probHbb, "fatjet_particleNetJetTags_probHbb/F");
      m_smallT->Branch ("fatjet_particleNetDiscriminatorsJetTags_HbbvsQCD", &m_fatjet_particleNetDiscriminatorsJetTags_HbbvsQCD, "fatjet_particleNetDiscriminatorsJetTags_HbbvsQCD/F");
      m_smallT->Branch ("fatjet_filteredMass", &m_fatjet_filteredMass, "fatjet_filteredMass/F");
      m_smallT->Branch ("fatjet_prunedMass", &m_fatjet_prunedMass, "fatjet_prunedMass/F");
      m_smallT->Branch ("fatjet_trimmedMass", &m_fatjet_trimmedMass, "fatjet_trimmedMass/F");
      m_smallT->Branch ("fatjet_softdropMass", &m_fatjet_softdropMass, "fatjet_softdropMass/F");
      m_smallT->Branch ("fatjet_tau1", &m_fatjet_tau1, "fatjet_tau1/F");
      m_smallT->Branch ("fatjet_tau2", &m_fatjet_tau2, "fatjet_tau2/F");
      m_smallT->Branch ("fatjet_tau3", &m_fatjet_tau3, "fatjet_tau3/F");
      m_smallT->Branch ("fatjet_nsubjets", &m_fatjet_nsubjets, "fatjet_nsubjets/I");
      m_smallT->Branch ("dR_subj1_subj2", &m_dR_subj1_subj2, "dR_subj1_subj2/F");

      m_smallT->Branch ("subjetjet1_pt", &m_subjetjet1_pt, "subjetjet1_pt/F");
      m_smallT->Branch ("subjetjet1_eta", &m_subjetjet1_eta, "subjetjet1_eta/F");
      m_smallT->Branch ("subjetjet1_phi", &m_subjetjet1_phi, "subjetjet1_phi/F");
      m_smallT->Branch ("subjetjet1_e", &m_subjetjet1_e, "subjetjet1_e/F");
      m_smallT->Branch ("subjetjet1_bID", &m_subjetjet1_bID, "subjetjet1_bID/F");
      m_smallT->Branch ("subjetjet1_bID_deepCSV", &m_subjetjet1_bID_deepCSV, "subjetjet1_bID_deepCSV/F");
      m_smallT->Branch ("subjetjet1_bID_deepFlavor", &m_subjetjet1_bID_deepFlavor, "subjetjet1_bID_deepFlavor/F");

      m_smallT->Branch ("subjetjet2_pt", &m_subjetjet2_pt, "subjetjet2_pt/F");
      m_smallT->Branch ("subjetjet2_eta", &m_subjetjet2_eta, "subjetjet2_eta/F");
      m_smallT->Branch ("subjetjet2_phi", &m_subjetjet2_phi, "subjetjet2_phi/F");
      m_smallT->Branch ("subjetjet2_e", &m_subjetjet2_e, "subjetjet2_e/F");
      m_smallT->Branch ("subjetjet2_bID", &m_subjetjet2_bID, "subjetjet2_bID/F");
      m_smallT->Branch ("subjetjet2_bID_deepCSV", &m_subjetjet2_bID_deepCSV, "subjetjet2_bID_deepCSV/F");
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
      m_smallT->Branch ("tauH_SVFIT_mass_up", &m_tauH_SVFIT_mass_up, "tauH_SVFIT_mass_up/F") ;
      m_smallT->Branch ("tauH_SVFIT_mass_down", &m_tauH_SVFIT_mass_down, "tauH_SVFIT_mass_down/F") ;
      m_smallT->Branch ("tauH_SVFIT_mass_METup", &m_tauH_SVFIT_mass_METup, "tauH_SVFIT_mass_METup/F") ;
      m_smallT->Branch ("tauH_SVFIT_mass_METdown", &m_tauH_SVFIT_mass_METdown, "tauH_SVFIT_mass_METdown/F") ;

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
      m_smallT->Branch ("HH_mass_raw", &m_HH_mass_raw, "HH_mass_raw/F") ;


      m_smallT->Branch ("HHKin_mass", &m_HHKin_mass, "HHKin_mass/F") ;
      m_smallT->Branch ("HHKin_chi2", &m_HHKin_chi2, "HHKin_chi2/F") ;
      m_smallT->Branch ("HH_deltaPhi", &m_HH_deltaPhi, "HH_deltaPhi/F") ;
      m_smallT->Branch ("HH_deltaR", &m_HH_deltaR, "HH_deltaR/F") ;
      m_smallT->Branch ("HH_deltaEta", &m_HH_deltaEta, "HH_deltaEta/F") ;
      m_smallT->Branch ("HHsvfit_deltaPhi", &m_HHsvfit_deltaPhi, "HHsvfit_deltaPhi/F") ;
      m_smallT->Branch ("tauHMet_deltaPhi", &m_tauHMet_deltaPhi, "tauHMet_deltaPhi/F") ;
      m_smallT->Branch ("tauHsvfitMet_deltaPhi", &m_tauHsvfitMet_deltaPhi, "tauHsvfitMet_deltaPhi/F") ;
      m_smallT->Branch ("bHMet_deltaPhi", &m_bHMet_deltaPhi, "bHMet_deltaPhi/F") ;
//      m_smallT->Branch ("", &m_, "/F") ;
      m_smallT->Branch ("ditau_deltaPhi", &m_ditau_deltaPhi, "ditau_deltaPhi/F") ;
      m_smallT->Branch ("ditau_deltaEta", &m_ditau_deltaEta, "ditau_deltaEta/F") ;
      m_smallT->Branch ("dib_deltaPhi", &m_dib_deltaPhi, "dib_deltaPhi/F") ;
      m_smallT->Branch ("dib_deltaEta", &m_dib_deltaEta, "dib_deltaEta/F") ;
      m_smallT->Branch ("ditau_deltaR",   &m_ditau_deltaR,    "ditau_deltaR/F") ;
      m_smallT->Branch ("dib_deltaR",     &m_dib_deltaR,      "dib_deltaR/F") ;
      m_smallT->Branch ("ditau_deltaR_per_tauHsvfitpt", &m_ditau_deltaR_per_tauHsvfitpt, "ditau_deltaR_per_tauHsvfitpt/F") ;
      m_smallT->Branch ("dib_deltaR_per_bHpt", &m_dib_deltaR_per_bHpt, "dib_deltaR_per_bHpt/F") ;

      m_smallT->Branch ("btau_deltaRmin", &m_btau_deltaRmin,  "btau_deltaRmin/F") ;
      m_smallT->Branch ("btau_deltaRmax", &m_btau_deltaRmax,  "btau_deltaRmax/F") ;
      m_smallT->Branch ("dau1MET_deltaphi", &m_dau1MET_deltaphi,  "dau1MET_deltaphi/F") ;
      m_smallT->Branch ("dau2MET_deltaphi", &m_dau2MET_deltaphi,  "dau2MET_deltaphi/F") ;
      m_smallT->Branch ("HT20", &m_HT20, "HT20/F");
      m_smallT->Branch ("HT50", &m_HT50, "HT50/F");
      m_smallT->Branch ("HT20Full", &m_HT20Full, "HT20Full/F");
      m_smallT->Branch ("jet20centrality", &m_jet20centrality, "jet20centrality/F");

      m_smallT->Branch ("jets_pt", &m_jets_pt) ;
      m_smallT->Branch ("jets_eta", &m_jets_eta) ;
      m_smallT->Branch ("jets_phi", &m_jets_phi) ;
      m_smallT->Branch ("jets_e", &m_jets_e) ;
      m_smallT->Branch ("jets_btag", &m_jets_btag) ;
      m_smallT->Branch ("jets_btag_deepCSV", &m_jets_btag_deepCSV) ;
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
      m_smallT->Branch ("jets_smearFactor", &m_jets_smearFactor) ;
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

      m_smallT->Branch("MT2",              &m_MT2,               "MT2/F");

      // m_smallT->Branch("MT2_covMtrxStatus", &m_MT2_covMtrxStatus, "MT2_covMtrxStatus/I");
      // m_smallT->Branch("MT2_EDM",           &m_MT2_EDM,           "MT2_EDM/F");
      // m_smallT->Branch("MT2_hasConverged",  &m_MT2_hasConverged,  "MT2_hasConverged/I");
      // m_smallT->Branch("MT2_status",        &m_MT2_status,        "MT2_status/I");
      // m_smallT->Branch("MT2_ncalls",        &m_MT2_ncalls,        "MT2_ncalls/I");
      // m_smallT->Branch("MT2_l",               &m_MT2_l,               "MT2_l/F");

      m_smallT->Branch ("bH_mass_raw",        &m_bH_mass_raw,"bH_mass_raw/F");
      m_smallT->Branch ("bH_pt_raw",          &m_bH_pt_raw,"bH_pt_raw/F");
      m_smallT->Branch ("bH_pt_raw_jetup",    &m_bH_pt_raw_jetup);
      m_smallT->Branch ("bH_mass_raw_jetup",  &m_bH_mass_raw_jetup);
      m_smallT->Branch ("bH_pt_raw_jetdown",  &m_bH_pt_raw_jetdown);
      m_smallT->Branch ("bH_mass_raw_jetdown",&m_bH_mass_raw_jetdown);

      m_smallT->Branch ("bjet1_pt_raw_jetup1",   &m_bjet1_pt_raw_jetup1,"bjet1_pt_raw_jetup1/F");
      m_smallT->Branch ("bjet1_pt_raw_jetup2",   &m_bjet1_pt_raw_jetup2,"bjet1_pt_raw_jetup2/F");
      m_smallT->Branch ("bjet1_pt_raw_jetup3",   &m_bjet1_pt_raw_jetup3,"bjet1_pt_raw_jetup3/F");
      m_smallT->Branch ("bjet1_pt_raw_jetup4",   &m_bjet1_pt_raw_jetup4,"bjet1_pt_raw_jetup4/F");
      m_smallT->Branch ("bjet1_pt_raw_jetup5",   &m_bjet1_pt_raw_jetup5,"bjet1_pt_raw_jetup5/F");
      m_smallT->Branch ("bjet1_pt_raw_jetup6",   &m_bjet1_pt_raw_jetup6,"bjet1_pt_raw_jetup6/F");
      m_smallT->Branch ("bjet1_pt_raw_jetup7",   &m_bjet1_pt_raw_jetup7,"bjet1_pt_raw_jetup7/F");
      m_smallT->Branch ("bjet1_pt_raw_jetup8",   &m_bjet1_pt_raw_jetup8,"bjet1_pt_raw_jetup8/F");
      m_smallT->Branch ("bjet1_pt_raw_jetup9",   &m_bjet1_pt_raw_jetup9,"bjet1_pt_raw_jetup9/F");
      m_smallT->Branch ("bjet1_pt_raw_jetup10",  &m_bjet1_pt_raw_jetup10,"bjet1_pt_raw_jetup10/F");
      m_smallT->Branch ("bjet1_pt_raw_jetup11",  &m_bjet1_pt_raw_jetup11,"bjet1_pt_raw_jetup11/F");

      m_smallT->Branch ("bjet1_pt_raw_jetdown1", &m_bjet1_pt_raw_jetdown1,"bjet1_pt_raw_jetdown1/F");
      m_smallT->Branch ("bjet1_pt_raw_jetdown2", &m_bjet1_pt_raw_jetdown2,"bjet1_pt_raw_jetdown2/F");
      m_smallT->Branch ("bjet1_pt_raw_jetdown3", &m_bjet1_pt_raw_jetdown3,"bjet1_pt_raw_jetdown3/F");
      m_smallT->Branch ("bjet1_pt_raw_jetdown4", &m_bjet1_pt_raw_jetdown4,"bjet1_pt_raw_jetdown4/F");
      m_smallT->Branch ("bjet1_pt_raw_jetdown5", &m_bjet1_pt_raw_jetdown5,"bjet1_pt_raw_jetdown5/F");
      m_smallT->Branch ("bjet1_pt_raw_jetdown6", &m_bjet1_pt_raw_jetdown6,"bjet1_pt_raw_jetdown6/F");
      m_smallT->Branch ("bjet1_pt_raw_jetdown7", &m_bjet1_pt_raw_jetdown7,"bjet1_pt_raw_jetdown7/F");
      m_smallT->Branch ("bjet1_pt_raw_jetdown8", &m_bjet1_pt_raw_jetdown8,"bjet1_pt_raw_jetdown8/F");
      m_smallT->Branch ("bjet1_pt_raw_jetdown9", &m_bjet1_pt_raw_jetdown9,"bjet1_pt_raw_jetdown9/F");
      m_smallT->Branch ("bjet1_pt_raw_jetdown10",&m_bjet1_pt_raw_jetdown10,"bjet1_pt_raw_jetdown10/F");
      m_smallT->Branch ("bjet1_pt_raw_jetdown11",&m_bjet1_pt_raw_jetdown11,"bjet1_pt_raw_jetdown11/F");

      m_smallT->Branch ("bjet2_pt_raw_jetup1",   &m_bjet2_pt_raw_jetup1,"bjet2_pt_raw_jetup1/F");
      m_smallT->Branch ("bjet2_pt_raw_jetup2",   &m_bjet2_pt_raw_jetup2,"bjet2_pt_raw_jetup2/F");
      m_smallT->Branch ("bjet2_pt_raw_jetup3",   &m_bjet2_pt_raw_jetup3,"bjet2_pt_raw_jetup3/F");
      m_smallT->Branch ("bjet2_pt_raw_jetup4",   &m_bjet2_pt_raw_jetup4,"bjet2_pt_raw_jetup4/F");
      m_smallT->Branch ("bjet2_pt_raw_jetup5",   &m_bjet2_pt_raw_jetup5,"bjet2_pt_raw_jetup5/F");
      m_smallT->Branch ("bjet2_pt_raw_jetup6",   &m_bjet2_pt_raw_jetup6,"bjet2_pt_raw_jetup6/F");
      m_smallT->Branch ("bjet2_pt_raw_jetup7",   &m_bjet2_pt_raw_jetup7,"bjet2_pt_raw_jetup7/F");
      m_smallT->Branch ("bjet2_pt_raw_jetup8",   &m_bjet2_pt_raw_jetup8,"bjet2_pt_raw_jetup8/F");
      m_smallT->Branch ("bjet2_pt_raw_jetup9",   &m_bjet2_pt_raw_jetup9,"bjet2_pt_raw_jetup9/F");
      m_smallT->Branch ("bjet2_pt_raw_jetup10",  &m_bjet2_pt_raw_jetup10,"bjet2_pt_raw_jetup10/F");
      m_smallT->Branch ("bjet2_pt_raw_jetup11",  &m_bjet2_pt_raw_jetup11,"bjet2_pt_raw_jetup11/F");

      m_smallT->Branch ("bjet2_pt_raw_jetdown1", &m_bjet2_pt_raw_jetdown1,"bjet2_pt_raw_jetdown1/F");
      m_smallT->Branch ("bjet2_pt_raw_jetdown2", &m_bjet2_pt_raw_jetdown2,"bjet2_pt_raw_jetdown2/F");
      m_smallT->Branch ("bjet2_pt_raw_jetdown3", &m_bjet2_pt_raw_jetdown3,"bjet2_pt_raw_jetdown3/F");
      m_smallT->Branch ("bjet2_pt_raw_jetdown4", &m_bjet2_pt_raw_jetdown4,"bjet2_pt_raw_jetdown4/F");
      m_smallT->Branch ("bjet2_pt_raw_jetdown5", &m_bjet2_pt_raw_jetdown5,"bjet2_pt_raw_jetdown5/F");
      m_smallT->Branch ("bjet2_pt_raw_jetdown6", &m_bjet2_pt_raw_jetdown6,"bjet2_pt_raw_jetdown6/F");
      m_smallT->Branch ("bjet2_pt_raw_jetdown7", &m_bjet2_pt_raw_jetdown7,"bjet2_pt_raw_jetdown7/F");
      m_smallT->Branch ("bjet2_pt_raw_jetdown8", &m_bjet2_pt_raw_jetdown8,"bjet2_pt_raw_jetdown8/F");
      m_smallT->Branch ("bjet2_pt_raw_jetdown9", &m_bjet2_pt_raw_jetdown9,"bjet2_pt_raw_jetdown9/F");
      m_smallT->Branch ("bjet2_pt_raw_jetdown10",&m_bjet2_pt_raw_jetdown10,"bjet2_pt_raw_jetdown10/F");
      m_smallT->Branch ("bjet2_pt_raw_jetdown11",&m_bjet2_pt_raw_jetdown11,"bjet2_pt_raw_jetdown11/F");


      m_smallT->Branch ("bH_mass_raw_jetup1",   &m_bH_mass_raw_jetup1,"bH_mass_raw_jetup1/F");
      m_smallT->Branch ("bH_mass_raw_jetup2",   &m_bH_mass_raw_jetup2,"bH_mass_raw_jetup2/F");
      m_smallT->Branch ("bH_mass_raw_jetup3",   &m_bH_mass_raw_jetup3,"bH_mass_raw_jetup3/F");
      m_smallT->Branch ("bH_mass_raw_jetup4",   &m_bH_mass_raw_jetup4,"bH_mass_raw_jetup4/F");
      m_smallT->Branch ("bH_mass_raw_jetup5",   &m_bH_mass_raw_jetup5,"bH_mass_raw_jetup5/F");
      m_smallT->Branch ("bH_mass_raw_jetup6",   &m_bH_mass_raw_jetup6,"bH_mass_raw_jetup6/F");
      m_smallT->Branch ("bH_mass_raw_jetup7",   &m_bH_mass_raw_jetup7,"bH_mass_raw_jetup7/F");
      m_smallT->Branch ("bH_mass_raw_jetup8",   &m_bH_mass_raw_jetup8,"bH_mass_raw_jetup8/F");
      m_smallT->Branch ("bH_mass_raw_jetup9",   &m_bH_mass_raw_jetup9,"bH_mass_raw_jetup9/F");
      m_smallT->Branch ("bH_mass_raw_jetup10",  &m_bH_mass_raw_jetup10,"bH_mass_raw_jetup10/F");
      m_smallT->Branch ("bH_mass_raw_jetup11",  &m_bH_mass_raw_jetup11,"bH_mass_raw_jetup11/F");

      m_smallT->Branch ("bH_mass_raw_jetdown1", &m_bH_mass_raw_jetdown1,"bH_mass_raw_jetdown1/F");
      m_smallT->Branch ("bH_mass_raw_jetdown2", &m_bH_mass_raw_jetdown2,"bH_mass_raw_jetdown2/F");
      m_smallT->Branch ("bH_mass_raw_jetdown3", &m_bH_mass_raw_jetdown3,"bH_mass_raw_jetdown3/F");
      m_smallT->Branch ("bH_mass_raw_jetdown4", &m_bH_mass_raw_jetdown4,"bH_mass_raw_jetdown4/F");
      m_smallT->Branch ("bH_mass_raw_jetdown5", &m_bH_mass_raw_jetdown5,"bH_mass_raw_jetdown5/F");
      m_smallT->Branch ("bH_mass_raw_jetdown6", &m_bH_mass_raw_jetdown6,"bH_mass_raw_jetdown6/F");
      m_smallT->Branch ("bH_mass_raw_jetdown7", &m_bH_mass_raw_jetdown7,"bH_mass_raw_jetdown7/F");
      m_smallT->Branch ("bH_mass_raw_jetdown8", &m_bH_mass_raw_jetdown8,"bH_mass_raw_jetdown8/F");
      m_smallT->Branch ("bH_mass_raw_jetdown9", &m_bH_mass_raw_jetdown9,"bH_mass_raw_jetdown9/F");
      m_smallT->Branch ("bH_mass_raw_jetdown10",&m_bH_mass_raw_jetdown10,"bH_mass_raw_jetdown10/F");
      m_smallT->Branch ("bH_mass_raw_jetdown11",&m_bH_mass_raw_jetdown11,"bH_mass_raw_jetdown11/F");


      m_smallT->Branch ("bjet1_pt_raw_jetupTot"    , &m_bjet1_pt_raw_jetupTot    , "bjet1_pt_raw_jetupTot/F");
      m_smallT->Branch ("bjet1_pt_raw_jetdownTot"  , &m_bjet1_pt_raw_jetdownTot  , "bjet1_pt_raw_jetdownTot/F");
      m_smallT->Branch ("bjet2_pt_raw_jetupTot"    , &m_bjet2_pt_raw_jetupTot    , "bjet2_pt_raw_jetupTot/F");
      m_smallT->Branch ("bjet2_pt_raw_jetdownTot"  , &m_bjet2_pt_raw_jetdownTot  , "bjet2_pt_raw_jetdownTot/F");
      m_smallT->Branch ("bjet1_mass_raw_jetupTot"  , &m_bjet1_mass_raw_jetupTot  , "bjet1_mass_raw_jetupTot/F");
      m_smallT->Branch ("bjet1_mass_raw_jetdownTot", &m_bjet1_mass_raw_jetdownTot, "bjet1_mass_raw_jetdownTot/F");
      m_smallT->Branch ("bjet2_mass_raw_jetupTot"  , &m_bjet2_mass_raw_jetupTot  , "bjet2_mass_raw_jetupTot/F");
      m_smallT->Branch ("bjet2_mass_raw_jetdownTot", &m_bjet2_mass_raw_jetdownTot, "bjet2_mass_raw_jetdownTot/F");
      m_smallT->Branch ("bH_mass_raw_jetupTot"     , &m_bH_mass_raw_jetupTot     , "bH_mass_raw_jetupTot/F");
      m_smallT->Branch ("bH_mass_raw_jetdownTot"   , &m_bH_mass_raw_jetdownTot   , "bH_mass_raw_jetdownTot/F");

      m_smallT->Branch ("HHKin_mass_raw",&m_HHKin_mass_raw ,"HHKin_mass_raw/F");
      m_smallT->Branch ("HHKin_mass_raw_chi2", &m_HHKin_mass_raw_chi2, "HHKin_mass_raw_chi2/F") ;
      m_smallT->Branch ("HHKin_mass_raw_convergence", &m_HHKin_mass_raw_convergence, "HHKin_mass_raw_convergence/I") ;
      m_smallT->Branch ("HHKin_mass_raw_prob", &m_HHKin_mass_raw_prob, "HHKin_mass_raw_prob/F") ;

      m_smallT->Branch("HHKin_mass_raw_copy",&m_HHKin_mass_raw_copy ,"HHKin_mass_raw_copy/F");

      //m_smallT->Branch ("MuTauKine", &m_mvaValueMuTau, "MuTauKine/F");
      //m_smallT->Branch ("TauTauKine", &m_mvaValueTauTau, "TauTauKine/F");

      m_smallT->Branch ("lheht", &m_lheht, "lheht/F");
      m_smallT->Branch("topReweight",&m_topReweight,"topReweight/F");
      m_smallT->Branch ("VBFjet1_pt",&m_VBFjet1_pt,"VBFjet1_pt/F");
      m_smallT->Branch ("VBFjet1_pt_jetup",  &m_VBFjet1_pt_jetup);
      m_smallT->Branch ("VBFjet1_pt_jetdown",&m_VBFjet1_pt_jetdown);
      m_smallT->Branch ("VBFjet1_mass_jetup",  &m_VBFjet1_mass_jetup);
      m_smallT->Branch ("VBFjet1_mass_jetdown",&m_VBFjet1_mass_jetdown);

      m_smallT->Branch ("VBFjet1_pt_jetup1",&m_VBFjet1_pt_jetup1, "VBFjet1_pt_jetup1/F");
      m_smallT->Branch ("VBFjet1_pt_jetup2",&m_VBFjet1_pt_jetup2, "VBFjet1_pt_jetup2/F");
      m_smallT->Branch ("VBFjet1_pt_jetup3",&m_VBFjet1_pt_jetup3, "VBFjet1_pt_jetup3/F");
      m_smallT->Branch ("VBFjet1_pt_jetup4",&m_VBFjet1_pt_jetup4, "VBFjet1_pt_jetup4/F");
      m_smallT->Branch ("VBFjet1_pt_jetup5",&m_VBFjet1_pt_jetup5, "VBFjet1_pt_jetup5/F");
      m_smallT->Branch ("VBFjet1_pt_jetup6",&m_VBFjet1_pt_jetup6, "VBFjet1_pt_jetup6/F");
      m_smallT->Branch ("VBFjet1_pt_jetup7",&m_VBFjet1_pt_jetup7, "VBFjet1_pt_jetup7/F");
      m_smallT->Branch ("VBFjet1_pt_jetup8",&m_VBFjet1_pt_jetup8, "VBFjet1_pt_jetup8/F");
      m_smallT->Branch ("VBFjet1_pt_jetup9",&m_VBFjet1_pt_jetup9, "VBFjet1_pt_jetup9/F");
      m_smallT->Branch ("VBFjet1_pt_jetup10",&m_VBFjet1_pt_jetup10, "VBFjet1_pt_jetup10/F");
      m_smallT->Branch ("VBFjet1_pt_jetup11",&m_VBFjet1_pt_jetup11, "VBFjet1_pt_jetup11/F");

      m_smallT->Branch ("VBFjet1_pt_jetdown1",&m_VBFjet1_pt_jetdown1, "VBFjet1_pt_jetdown1/F");
      m_smallT->Branch ("VBFjet1_pt_jetdown2",&m_VBFjet1_pt_jetdown2, "VBFjet1_pt_jetdown2/F");
      m_smallT->Branch ("VBFjet1_pt_jetdown3",&m_VBFjet1_pt_jetdown3, "VBFjet1_pt_jetdown3/F");
      m_smallT->Branch ("VBFjet1_pt_jetdown4",&m_VBFjet1_pt_jetdown4, "VBFjet1_pt_jetdown4/F");
      m_smallT->Branch ("VBFjet1_pt_jetdown5",&m_VBFjet1_pt_jetdown5, "VBFjet1_pt_jetdown5/F");
      m_smallT->Branch ("VBFjet1_pt_jetdown6",&m_VBFjet1_pt_jetdown6, "VBFjet1_pt_jetdown6/F");
      m_smallT->Branch ("VBFjet1_pt_jetdown7",&m_VBFjet1_pt_jetdown7, "VBFjet1_pt_jetdown7/F");
      m_smallT->Branch ("VBFjet1_pt_jetdown8",&m_VBFjet1_pt_jetdown8, "VBFjet1_pt_jetdown8/F");
      m_smallT->Branch ("VBFjet1_pt_jetdown9",&m_VBFjet1_pt_jetdown9, "VBFjet1_pt_jetdown9/F");
      m_smallT->Branch ("VBFjet1_pt_jetdown10",&m_VBFjet1_pt_jetdown10, "VBFjet1_pt_jetdown10/F");
      m_smallT->Branch ("VBFjet1_pt_jetdown11",&m_VBFjet1_pt_jetdown11, "VBFjet1_pt_jetdown11/F");

      m_smallT->Branch ("VBFjet1_eta",&m_VBFjet1_eta,"VBFjet1_eta/F");
      m_smallT->Branch ("VBFjet1_phi",&m_VBFjet1_phi,"VBFjet1_phi/F");
      m_smallT->Branch ("VBFjet1_e",&m_VBFjet1_e,"VBFjet1_e/F");

      m_smallT->Branch ("VBFjet1_btag",&m_VBFjet1_btag,"VBFjet1_btag/F");
      m_smallT->Branch ("VBFjet1_btag_deepCSV",&m_VBFjet1_btag_deepCSV,"VBFjet1_btag_deepCSV/F");
      m_smallT->Branch ("VBFjet1_btag_deepFlavor",&m_VBFjet1_btag_deepFlavor,"VBFjet1_btag_deepFlavor/F");
      m_smallT->Branch ("VBFjet1_ctag_deepFlavor",&m_VBFjet1_ctag_deepFlavor,"VBFjet1_ctag_deepFlavor/F");
      m_smallT->Branch ("VBFjet1_CvsL",&m_VBFjet1_CvsL,"VBFjet1_CvsL/F");
      m_smallT->Branch ("VBFjet1_CvsB",&m_VBFjet1_CvsB,"VBFjet1_CvsB/F");
      m_smallT->Branch ("VBFjet1_HHbtag",&m_VBFjet1_HHbtag,"VBFjet1_HHbtag/F");
      m_smallT->Branch ("VBFjet1_pnet_bb", &m_VBFjet1_pnet_bb, "VBFjet1_pnet_bb/F");
      m_smallT->Branch ("VBFjet1_pnet_cc", &m_VBFjet1_pnet_cc, "VBFjet1_pnet_cc/F");
      m_smallT->Branch ("VBFjet1_pnet_b", &m_VBFjet1_pnet_b, "VBFjet1_pnet_b/F");
      m_smallT->Branch ("VBFjet1_pnet_c", &m_VBFjet1_pnet_c, "VBFjet1_pnet_c/F");
      m_smallT->Branch ("VBFjet1_pnet_g", &m_VBFjet1_pnet_g, "VBFjet1_pnet_g/F");
      m_smallT->Branch ("VBFjet1_pnet_uds", &m_VBFjet1_pnet_uds, "VBFjet1_pnet_uds/F");
      m_smallT->Branch ("VBFjet1_pnet_pu", &m_VBFjet1_pnet_pu, "VBFjet1_pnet_pu/F");
      m_smallT->Branch ("VBFjet1_pnet_undef", &m_VBFjet1_pnet_undef, "VBFjet1_pnet_undef/F");
      m_smallT->Branch ("VBFjet1_smearFactor",&m_VBFjet1_smearFactor,"VBFjet1_smearFactor/F");
      m_smallT->Branch ("VBFjet1_PUjetIDupdated",&m_VBFjet1_PUjetIDupdated,"VBFjet1_PUjetIDupdated/F");
      m_smallT->Branch ("VBFjet1_flav",&m_VBFjet1_flav,"VBFjet1_flav/I");
      m_smallT->Branch ("VBFjet1_hasgenjet",&m_VBFjet1_hasgenjet,"VBFjet1_hasgenjet/O");

      m_smallT->Branch ("VBFgenjet1_pt",&m_VBFgenjet1_pt,"VBFgenjet1_pt/F");
      m_smallT->Branch ("VBFgenjet1_eta",&m_VBFgenjet1_eta,"VBFgenjet1_eta/F");
      m_smallT->Branch ("VBFgenjet1_phi",&m_VBFgenjet1_phi,"VBFgenjet1_phi/F");
      m_smallT->Branch ("VBFgenjet1_e",&m_VBFgenjet1_e,"VBFgenjet1_e/F");


      m_smallT->Branch ("VBFjet2_pt",&m_VBFjet2_pt,"VBFjet2_pt/F");
      m_smallT->Branch ("VBFjet2_pt_jetup",  &m_VBFjet2_pt_jetup);
      m_smallT->Branch ("VBFjet2_pt_jetdown",&m_VBFjet2_pt_jetdown);
      m_smallT->Branch ("VBFjet2_mass_jetup",  &m_VBFjet2_mass_jetup);
      m_smallT->Branch ("VBFjet2_mass_jetdown",&m_VBFjet2_mass_jetdown);

      m_smallT->Branch ("VBFjet2_pt_jetup1",&m_VBFjet2_pt_jetup1, "VBFjet2_pt_jetup1/F");
      m_smallT->Branch ("VBFjet2_pt_jetup2",&m_VBFjet2_pt_jetup2, "VBFjet2_pt_jetup2/F");
      m_smallT->Branch ("VBFjet2_pt_jetup3",&m_VBFjet2_pt_jetup3, "VBFjet2_pt_jetup3/F");
      m_smallT->Branch ("VBFjet2_pt_jetup4",&m_VBFjet2_pt_jetup4, "VBFjet2_pt_jetup4/F");
      m_smallT->Branch ("VBFjet2_pt_jetup5",&m_VBFjet2_pt_jetup5, "VBFjet2_pt_jetup5/F");
      m_smallT->Branch ("VBFjet2_pt_jetup6",&m_VBFjet2_pt_jetup6, "VBFjet2_pt_jetup6/F");
      m_smallT->Branch ("VBFjet2_pt_jetup7",&m_VBFjet2_pt_jetup7, "VBFjet2_pt_jetup7/F");
      m_smallT->Branch ("VBFjet2_pt_jetup8",&m_VBFjet2_pt_jetup8, "VBFjet2_pt_jetup8/F");
      m_smallT->Branch ("VBFjet2_pt_jetup9",&m_VBFjet2_pt_jetup9, "VBFjet2_pt_jetup9/F");
      m_smallT->Branch ("VBFjet2_pt_jetup10",&m_VBFjet2_pt_jetup10, "VBFjet2_pt_jetup10/F");
      m_smallT->Branch ("VBFjet2_pt_jetup11",&m_VBFjet2_pt_jetup11, "VBFjet2_pt_jetup11/F");

      m_smallT->Branch ("VBFjet2_pt_jetdown1",&m_VBFjet2_pt_jetdown1, "VBFjet2_pt_jetdown1/F");
      m_smallT->Branch ("VBFjet2_pt_jetdown2",&m_VBFjet2_pt_jetdown2, "VBFjet2_pt_jetdown2/F");
      m_smallT->Branch ("VBFjet2_pt_jetdown3",&m_VBFjet2_pt_jetdown3, "VBFjet2_pt_jetdown3/F");
      m_smallT->Branch ("VBFjet2_pt_jetdown4",&m_VBFjet2_pt_jetdown4, "VBFjet2_pt_jetdown4/F");
      m_smallT->Branch ("VBFjet2_pt_jetdown5",&m_VBFjet2_pt_jetdown5, "VBFjet2_pt_jetdown5/F");
      m_smallT->Branch ("VBFjet2_pt_jetdown6",&m_VBFjet2_pt_jetdown6, "VBFjet2_pt_jetdown6/F");
      m_smallT->Branch ("VBFjet2_pt_jetdown7",&m_VBFjet2_pt_jetdown7, "VBFjet2_pt_jetdown7/F");
      m_smallT->Branch ("VBFjet2_pt_jetdown8",&m_VBFjet2_pt_jetdown8, "VBFjet2_pt_jetdown8/F");
      m_smallT->Branch ("VBFjet2_pt_jetdown9",&m_VBFjet2_pt_jetdown9, "VBFjet2_pt_jetdown9/F");
      m_smallT->Branch ("VBFjet2_pt_jetdown10",&m_VBFjet2_pt_jetdown10, "VBFjet2_pt_jetdown10/F");
      m_smallT->Branch ("VBFjet2_pt_jetdown11",&m_VBFjet2_pt_jetdown11, "VBFjet2_pt_jetdown11/F");

      m_smallT->Branch ("VBFjet2_eta",&m_VBFjet2_eta,"VBFjet2_eta/F");
      m_smallT->Branch ("VBFjet2_phi",&m_VBFjet2_phi,"VBFjet2_phi/F");
      m_smallT->Branch ("VBFjet2_e",&m_VBFjet2_e,"VBFjet2_e/F");

      m_smallT->Branch ("VBFjet2_btag",&m_VBFjet2_btag,"VBFjet2_btag/F");
      m_smallT->Branch ("VBFjet2_btag_deepCSV",&m_VBFjet2_btag_deepCSV,"VBFjet2_btag_deepCSV/F");
      m_smallT->Branch ("VBFjet2_btag_deepFlavor",&m_VBFjet2_btag_deepFlavor,"VBFjet2_btag_deepFlavor/F");
      m_smallT->Branch ("VBFjet2_ctag_deepFlavor",&m_VBFjet2_ctag_deepFlavor,"VBFjet2_ctag_deepFlavor/F");
      m_smallT->Branch ("VBFjet2_CvsL",&m_VBFjet2_CvsL,"VBFjet2_CvsL/F");
      m_smallT->Branch ("VBFjet2_CvsB",&m_VBFjet2_CvsB,"VBFjet2_CvsB/F");
      m_smallT->Branch ("VBFjet2_HHbtag",&m_VBFjet2_HHbtag,"VBFjet2_HHbtag/F");
      m_smallT->Branch ("VBFjet2_pnet_bb", &m_VBFjet2_pnet_bb, "VBFjet2_pnet_bb/F");
      m_smallT->Branch ("VBFjet2_pnet_cc", &m_VBFjet2_pnet_cc, "VBFjet2_pnet_cc/F");
      m_smallT->Branch ("VBFjet2_pnet_b", &m_VBFjet2_pnet_b, "VBFjet2_pnet_b/F");
      m_smallT->Branch ("VBFjet2_pnet_c", &m_VBFjet2_pnet_c, "VBFjet2_pnet_c/F");
      m_smallT->Branch ("VBFjet2_pnet_g", &m_VBFjet2_pnet_g, "VBFjet2_pnet_g/F");
      m_smallT->Branch ("VBFjet2_pnet_uds", &m_VBFjet2_pnet_uds, "VBFjet2_pnet_uds/F");
      m_smallT->Branch ("VBFjet2_pnet_pu", &m_VBFjet2_pnet_pu, "VBFjet2_pnet_pu/F");
      m_smallT->Branch ("VBFjet2_pnet_undef", &m_VBFjet2_pnet_undef, "VBFjet2_pnet_undef/F");
      m_smallT->Branch ("VBFjet2_smearFactor",&m_VBFjet2_smearFactor,"VBFjet2_smearFactor/F");
      m_smallT->Branch ("VBFjet2_PUjetIDupdated",&m_VBFjet2_PUjetIDupdated,"VBFjet2_PUjetIDupdated/F");
      m_smallT->Branch ("VBFjet2_flav",&m_VBFjet2_flav,"VBFjet2_flav/I");
      m_smallT->Branch ("VBFjet2_hasgenjet",&m_VBFjet2_hasgenjet,"VBFjet2_hasgenjet/O");

      m_smallT->Branch ("VBFgenjet2_pt",&m_VBFgenjet2_pt,"VBFgenjet2_pt/F");
      m_smallT->Branch ("VBFgenjet2_eta",&m_VBFgenjet2_eta,"VBFgenjet2_eta/F");
      m_smallT->Branch ("VBFgenjet2_phi",&m_VBFgenjet2_phi,"VBFgenjet2_phi/F");
      m_smallT->Branch ("VBFgenjet2_e",&m_VBFgenjet2_e,"VBFgenjet2_e/F");

      m_smallT->Branch ("VBFjj_mass", &m_VBFjj_mass ,"VBFjj_mass/F");
      m_smallT->Branch ("VBFjj_mass_jetup",    &m_VBFjj_mass_jetup );
      m_smallT->Branch ("VBFjj_mass_jetdown",  &m_VBFjj_mass_jetdown );

      m_smallT->Branch ("VBFjj_mass_jetup1",   &m_VBFjj_mass_jetup1, "VBFjj_mass_jetup1/F");
      m_smallT->Branch ("VBFjj_mass_jetup2",   &m_VBFjj_mass_jetup2, "VBFjj_mass_jetup2/F");
      m_smallT->Branch ("VBFjj_mass_jetup3",   &m_VBFjj_mass_jetup3, "VBFjj_mass_jetup3/F");
      m_smallT->Branch ("VBFjj_mass_jetup4",   &m_VBFjj_mass_jetup4, "VBFjj_mass_jetup4/F");
      m_smallT->Branch ("VBFjj_mass_jetup5",   &m_VBFjj_mass_jetup5, "VBFjj_mass_jetup5/F");
      m_smallT->Branch ("VBFjj_mass_jetup6",   &m_VBFjj_mass_jetup6, "VBFjj_mass_jetup6/F");
      m_smallT->Branch ("VBFjj_mass_jetup7",   &m_VBFjj_mass_jetup7, "VBFjj_mass_jetup7/F");
      m_smallT->Branch ("VBFjj_mass_jetup8",   &m_VBFjj_mass_jetup8, "VBFjj_mass_jetup8/F");
      m_smallT->Branch ("VBFjj_mass_jetup9",   &m_VBFjj_mass_jetup9, "VBFjj_mass_jetup9/F");
      m_smallT->Branch ("VBFjj_mass_jetup10",  &m_VBFjj_mass_jetup10, "VBFjj_mass_jetup10/F");
      m_smallT->Branch ("VBFjj_mass_jetup11",  &m_VBFjj_mass_jetup11, "VBFjj_mass_jetup11/F");

      m_smallT->Branch ("VBFjj_mass_jetdown1", &m_VBFjj_mass_jetdown1, "VBFjj_mass_jetdown1/F");
      m_smallT->Branch ("VBFjj_mass_jetdown2", &m_VBFjj_mass_jetdown2, "VBFjj_mass_jetdown2/F");
      m_smallT->Branch ("VBFjj_mass_jetdown3", &m_VBFjj_mass_jetdown3, "VBFjj_mass_jetdown3/F");
      m_smallT->Branch ("VBFjj_mass_jetdown4", &m_VBFjj_mass_jetdown4, "VBFjj_mass_jetdown4/F");
      m_smallT->Branch ("VBFjj_mass_jetdown5", &m_VBFjj_mass_jetdown5, "VBFjj_mass_jetdown5/F");
      m_smallT->Branch ("VBFjj_mass_jetdown6", &m_VBFjj_mass_jetdown6, "VBFjj_mass_jetdown6/F");
      m_smallT->Branch ("VBFjj_mass_jetdown7", &m_VBFjj_mass_jetdown7, "VBFjj_mass_jetdown7/F");
      m_smallT->Branch ("VBFjj_mass_jetdown8", &m_VBFjj_mass_jetdown8, "VBFjj_mass_jetdown8/F");
      m_smallT->Branch ("VBFjj_mass_jetdown9", &m_VBFjj_mass_jetdown9, "VBFjj_mass_jetdown9/F");
      m_smallT->Branch ("VBFjj_mass_jetdown10",&m_VBFjj_mass_jetdown10, "VBFjj_mass_jetdown10/F");
      m_smallT->Branch ("VBFjj_mass_jetdown11",&m_VBFjj_mass_jetdown11, "VBFjj_mass_jetdown11/F");

      m_smallT->Branch ("VBFjj_deltaEta" , &m_VBFjj_deltaEta ,"VBFjj_deltaEta/F");
      m_smallT->Branch ("VBFjj_deltaPhi" , &m_VBFjj_deltaPhi ,"VBFjj_deltaPhi/F");
      m_smallT->Branch ("VBFjj_HT" , &m_VBFjj_HT ,"VBFjj_HT/F");

      m_smallT->Branch ("VBFjet1_pt_jetupTot"    , &m_VBFjet1_pt_jetupTot    , "VBFjet1_pt_jetupTot/F");
      m_smallT->Branch ("VBFjet1_pt_jetdownTot"  , &m_VBFjet1_pt_jetdownTot  , "VBFjet1_pt_jetdownTot/F");
      m_smallT->Branch ("VBFjet2_pt_jetupTot"    , &m_VBFjet2_pt_jetupTot    , "VBFjet2_pt_jetupTot/F");
      m_smallT->Branch ("VBFjet2_pt_jetdownTot"  , &m_VBFjet2_pt_jetdownTot  , "VBFjet2_pt_jetdownTot/F");
      m_smallT->Branch ("VBFjet1_mass_jetupTot"  , &m_VBFjet1_mass_jetupTot  , "VBFjet1_mass_jetupTot/F");
      m_smallT->Branch ("VBFjet1_mass_jetdownTot", &m_VBFjet1_mass_jetdownTot, "VBFjet1_mass_jetdownTot/F");
      m_smallT->Branch ("VBFjet2_mass_jetupTot"  , &m_VBFjet2_mass_jetupTot  , "VBFjet2_mass_jetupTot/F");
      m_smallT->Branch ("VBFjet2_mass_jetdownTot", &m_VBFjet2_mass_jetdownTot, "VBFjet2_mass_jetdownTot/F");
      m_smallT->Branch ("VBFjj_mass_jetupTot"    , &m_VBFjj_mass_jetupTot    , "VBFjj_mass_jetupTot/F");
      m_smallT->Branch ("VBFjj_mass_jetdownTot"  , &m_VBFjj_mass_jetdownTot  , "VBFjj_mass_jetdownTot/F");

      // Additional central jets
      m_smallT->Branch ("addJetCentr1_pt"  , &m_addJetCentr1_pt  , "addJetCentr1_pt/F");
      m_smallT->Branch ("addJetCentr2_pt"  , &m_addJetCentr2_pt  , "addJetCentr2_pt/F");
      m_smallT->Branch ("addJetCentr3_pt"  , &m_addJetCentr3_pt  , "addJetCentr3_pt/F");
      m_smallT->Branch ("addJetCentr4_pt"  , &m_addJetCentr4_pt  , "addJetCentr4_pt/F");
      m_smallT->Branch ("addJetCentr5_pt"  , &m_addJetCentr5_pt  , "addJetCentr5_pt/F");
      m_smallT->Branch ("addJetCentr1_eta" , &m_addJetCentr1_eta , "addJetCentr1_eta/F");
      m_smallT->Branch ("addJetCentr2_eta" , &m_addJetCentr2_eta , "addJetCentr2_eta/F");
      m_smallT->Branch ("addJetCentr3_eta" , &m_addJetCentr3_eta , "addJetCentr3_eta/F");
      m_smallT->Branch ("addJetCentr4_eta" , &m_addJetCentr4_eta , "addJetCentr4_eta/F");
      m_smallT->Branch ("addJetCentr5_eta" , &m_addJetCentr5_eta , "addJetCentr5_eta/F");
      m_smallT->Branch ("addJetCentr1_phi" , &m_addJetCentr1_phi , "addJetCentr1_phi/F");
      m_smallT->Branch ("addJetCentr2_phi" , &m_addJetCentr2_phi , "addJetCentr2_phi/F");
      m_smallT->Branch ("addJetCentr3_phi" , &m_addJetCentr3_phi , "addJetCentr3_phi/F");
      m_smallT->Branch ("addJetCentr4_phi" , &m_addJetCentr4_phi , "addJetCentr4_phi/F");
      m_smallT->Branch ("addJetCentr5_phi" , &m_addJetCentr5_phi , "addJetCentr5_phi/F");
      m_smallT->Branch ("addJetCentr1_e"   , &m_addJetCentr1_e   , "addJetCentr1_e/F");
      m_smallT->Branch ("addJetCentr2_e"   , &m_addJetCentr2_e   , "addJetCentr2_e/F");
      m_smallT->Branch ("addJetCentr3_e"   , &m_addJetCentr3_e   , "addJetCentr3_e/F");
      m_smallT->Branch ("addJetCentr4_e"   , &m_addJetCentr4_e   , "addJetCentr4_e/F");
      m_smallT->Branch ("addJetCentr5_e"   , &m_addJetCentr5_e   , "addJetCentr5_e/F");
      m_smallT->Branch ("addJetCentr1_btag_deepFlavor" , &m_addJetCentr1_btag_deepFlavor , "addJetCentr1_btag_deepFlavor/F");
      m_smallT->Branch ("addJetCentr2_btag_deepFlavor" , &m_addJetCentr2_btag_deepFlavor , "addJetCentr2_btag_deepFlavor/F");
      m_smallT->Branch ("addJetCentr3_btag_deepFlavor" , &m_addJetCentr3_btag_deepFlavor , "addJetCentr3_btag_deepFlavor/F");
      m_smallT->Branch ("addJetCentr4_btag_deepFlavor" , &m_addJetCentr4_btag_deepFlavor , "addJetCentr4_btag_deepFlavor/F");
      m_smallT->Branch ("addJetCentr5_btag_deepFlavor" , &m_addJetCentr5_btag_deepFlavor , "addJetCentr5_btag_deepFlavor/F");
      m_smallT->Branch ("addJetCentr1_CvsL"            , &m_addJetCentr1_CvsL            , "addJetCentr1_CvsL/F");
      m_smallT->Branch ("addJetCentr2_CvsL"            , &m_addJetCentr2_CvsL            , "addJetCentr2_CvsL/F");
      m_smallT->Branch ("addJetCentr3_CvsL"            , &m_addJetCentr3_CvsL            , "addJetCentr3_CvsL/F");
      m_smallT->Branch ("addJetCentr4_CvsL"            , &m_addJetCentr4_CvsL            , "addJetCentr4_CvsL/F");
      m_smallT->Branch ("addJetCentr5_CvsL"            , &m_addJetCentr5_CvsL            , "addJetCentr5_CvsL/F");
      m_smallT->Branch ("addJetCentr1_CvsB"            , &m_addJetCentr1_CvsB            , "addJetCentr1_CvsB/F");
      m_smallT->Branch ("addJetCentr2_CvsB"            , &m_addJetCentr2_CvsB            , "addJetCentr2_CvsB/F");
      m_smallT->Branch ("addJetCentr3_CvsB"            , &m_addJetCentr3_CvsB            , "addJetCentr3_CvsB/F");
      m_smallT->Branch ("addJetCentr4_CvsB"            , &m_addJetCentr4_CvsB            , "addJetCentr4_CvsB/F");
      m_smallT->Branch ("addJetCentr5_CvsB"            , &m_addJetCentr5_CvsB            , "addJetCentr5_CvsB/F");
      m_smallT->Branch ("addJetCentr1_HHbtag"          , &m_addJetCentr1_HHbtag          , "addJetCentr1_HHbtag/F");
      m_smallT->Branch ("addJetCentr2_HHbtag"          , &m_addJetCentr2_HHbtag          , "addJetCentr2_HHbtag/F");
      m_smallT->Branch ("addJetCentr3_HHbtag"          , &m_addJetCentr3_HHbtag          , "addJetCentr3_HHbtag/F");
      m_smallT->Branch ("addJetCentr4_HHbtag"          , &m_addJetCentr4_HHbtag          , "addJetCentr4_HHbtag/F");
      m_smallT->Branch ("addJetCentr5_HHbtag"          , &m_addJetCentr5_HHbtag          , "addJetCentr5_HHbtag/F");
      m_smallT->Branch ("addJetCentr1_smearFactor"     , &m_addJetCentr1_smearFactor     , "addJetCentr1_smearFactor/F");
      m_smallT->Branch ("addJetCentr2_smearFactor"     , &m_addJetCentr2_smearFactor     , "addJetCentr2_smearFactor/F");
      m_smallT->Branch ("addJetCentr3_smearFactor"     , &m_addJetCentr3_smearFactor     , "addJetCentr3_smearFactor/F");
      m_smallT->Branch ("addJetCentr4_smearFactor"     , &m_addJetCentr4_smearFactor     , "addJetCentr4_smearFactor/F");
      m_smallT->Branch ("addJetCentr5_smearFactor"     , &m_addJetCentr5_smearFactor     , "addJetCentr5_smearFactor/F");
      m_smallT->Branch ("addJetCentr1_pnet_bb", &m_addJetCentr1_pnet_bb, "addJetCentr1_pnet_bb/F");
      m_smallT->Branch ("addJetCentr1_pnet_cc", &m_addJetCentr1_pnet_cc, "addJetCentr1_pnet_cc/F");
      m_smallT->Branch ("addJetCentr1_pnet_b", &m_addJetCentr1_pnet_b, "addJetCentr1_pnet_b/F");
      m_smallT->Branch ("addJetCentr1_pnet_c", &m_addJetCentr1_pnet_c, "addJetCentr1_pnet_c/F");
      m_smallT->Branch ("addJetCentr1_pnet_g", &m_addJetCentr1_pnet_g, "addJetCentr1_pnet_g/F");
      m_smallT->Branch ("addJetCentr1_pnet_uds", &m_addJetCentr1_pnet_uds, "addJetCentr1_pnet_uds/F");
      m_smallT->Branch ("addJetCentr1_pnet_pu", &m_addJetCentr1_pnet_pu, "addJetCentr1_pnet_pu/F");
      m_smallT->Branch ("addJetCentr1_pnet_undef", &m_addJetCentr1_pnet_undef, "addJetCentr1_pnet_undef/F");
      m_smallT->Branch ("addJetCentr2_pnet_bb", &m_addJetCentr2_pnet_bb, "addJetCentr2_pnet_bb/F");
      m_smallT->Branch ("addJetCentr2_pnet_cc", &m_addJetCentr2_pnet_cc, "addJetCentr2_pnet_cc/F");
      m_smallT->Branch ("addJetCentr2_pnet_b", &m_addJetCentr2_pnet_b, "addJetCentr2_pnet_b/F");
      m_smallT->Branch ("addJetCentr2_pnet_c", &m_addJetCentr2_pnet_c, "addJetCentr2_pnet_c/F");
      m_smallT->Branch ("addJetCentr2_pnet_g", &m_addJetCentr2_pnet_g, "addJetCentr2_pnet_g/F");
      m_smallT->Branch ("addJetCentr2_pnet_uds", &m_addJetCentr2_pnet_uds, "addJetCentr2_pnet_uds/F");
      m_smallT->Branch ("addJetCentr2_pnet_pu", &m_addJetCentr2_pnet_pu, "addJetCentr2_pnet_pu/F");
      m_smallT->Branch ("addJetCentr2_pnet_undef", &m_addJetCentr2_pnet_undef, "addJetCentr2_pnet_undef/F");
      m_smallT->Branch ("addJetCentr3_pnet_bb", &m_addJetCentr3_pnet_bb, "addJetCentr3_pnet_bb/F");
      m_smallT->Branch ("addJetCentr3_pnet_cc", &m_addJetCentr3_pnet_cc, "addJetCentr3_pnet_cc/F");
      m_smallT->Branch ("addJetCentr3_pnet_b", &m_addJetCentr3_pnet_b, "addJetCentr3_pnet_b/F");
      m_smallT->Branch ("addJetCentr3_pnet_c", &m_addJetCentr3_pnet_c, "addJetCentr3_pnet_c/F");
      m_smallT->Branch ("addJetCentr3_pnet_g", &m_addJetCentr3_pnet_g, "addJetCentr3_pnet_g/F");
      m_smallT->Branch ("addJetCentr3_pnet_uds", &m_addJetCentr3_pnet_uds, "addJetCentr3_pnet_uds/F");
      m_smallT->Branch ("addJetCentr3_pnet_pu", &m_addJetCentr3_pnet_pu, "addJetCentr3_pnet_pu/F");
      m_smallT->Branch ("addJetCentr3_pnet_undef", &m_addJetCentr3_pnet_undef, "addJetCentr3_pnet_undef/F");
      m_smallT->Branch ("addJetCentr4_pnet_bb", &m_addJetCentr4_pnet_bb, "addJetCentr4_pnet_bb/F");
      m_smallT->Branch ("addJetCentr4_pnet_cc", &m_addJetCentr4_pnet_cc, "addJetCentr4_pnet_cc/F");
      m_smallT->Branch ("addJetCentr4_pnet_b", &m_addJetCentr4_pnet_b, "addJetCentr4_pnet_b/F");
      m_smallT->Branch ("addJetCentr4_pnet_c", &m_addJetCentr4_pnet_c, "addJetCentr4_pnet_c/F");
      m_smallT->Branch ("addJetCentr4_pnet_g", &m_addJetCentr4_pnet_g, "addJetCentr4_pnet_g/F");
      m_smallT->Branch ("addJetCentr4_pnet_uds", &m_addJetCentr4_pnet_uds, "addJetCentr4_pnet_uds/F");
      m_smallT->Branch ("addJetCentr4_pnet_pu", &m_addJetCentr4_pnet_pu, "addJetCentr4_pnet_pu/F");
      m_smallT->Branch ("addJetCentr4_pnet_undef", &m_addJetCentr4_pnet_undef, "addJetCentr4_pnet_undef/F");
      m_smallT->Branch ("addJetCentr5_pnet_bb", &m_addJetCentr5_pnet_bb, "addJetCentr5_pnet_bb/F");
      m_smallT->Branch ("addJetCentr5_pnet_cc", &m_addJetCentr5_pnet_cc, "addJetCentr5_pnet_cc/F");
      m_smallT->Branch ("addJetCentr5_pnet_b", &m_addJetCentr5_pnet_b, "addJetCentr5_pnet_b/F");
      m_smallT->Branch ("addJetCentr5_pnet_c", &m_addJetCentr5_pnet_c, "addJetCentr5_pnet_c/F");
      m_smallT->Branch ("addJetCentr5_pnet_g", &m_addJetCentr5_pnet_g, "addJetCentr5_pnet_g/F");
      m_smallT->Branch ("addJetCentr5_pnet_uds", &m_addJetCentr5_pnet_uds, "addJetCentr5_pnet_uds/F");
      m_smallT->Branch ("addJetCentr5_pnet_pu", &m_addJetCentr5_pnet_pu, "addJetCentr5_pnet_pu/F");
      m_smallT->Branch ("addJetCentr5_pnet_undef", &m_addJetCentr5_pnet_undef, "addJetCentr5_pnet_undef/F");

      m_smallT->Branch ("addJetCentr1_pt_jetup"    , &m_addJetCentr1_pt_jetup);
      m_smallT->Branch ("addJetCentr1_pt_jetdown"  , &m_addJetCentr1_pt_jetdown);
      m_smallT->Branch ("addJetCentr1_mass_jetup"  , &m_addJetCentr1_mass_jetup);
      m_smallT->Branch ("addJetCentr1_mass_jetdown", &m_addJetCentr1_mass_jetdown);
      m_smallT->Branch ("addJetCentr2_pt_jetup"    , &m_addJetCentr2_pt_jetup);
      m_smallT->Branch ("addJetCentr2_pt_jetdown"  , &m_addJetCentr2_pt_jetdown);
      m_smallT->Branch ("addJetCentr2_mass_jetup"  , &m_addJetCentr2_mass_jetup);
      m_smallT->Branch ("addJetCentr2_mass_jetdown", &m_addJetCentr2_mass_jetdown);
      m_smallT->Branch ("addJetCentr3_pt_jetup"    , &m_addJetCentr3_pt_jetup);
      m_smallT->Branch ("addJetCentr3_pt_jetdown"  , &m_addJetCentr3_pt_jetdown);
      m_smallT->Branch ("addJetCentr3_mass_jetup"  , &m_addJetCentr3_mass_jetup);
      m_smallT->Branch ("addJetCentr3_mass_jetdown", &m_addJetCentr3_mass_jetdown);
      m_smallT->Branch ("addJetCentr4_pt_jetup"    , &m_addJetCentr4_pt_jetup);
      m_smallT->Branch ("addJetCentr4_pt_jetdown"  , &m_addJetCentr4_pt_jetdown);
      m_smallT->Branch ("addJetCentr4_mass_jetup"  , &m_addJetCentr4_mass_jetup);
      m_smallT->Branch ("addJetCentr4_mass_jetdown", &m_addJetCentr4_mass_jetdown);
      m_smallT->Branch ("addJetCentr5_pt_jetup"    , &m_addJetCentr5_pt_jetup);
      m_smallT->Branch ("addJetCentr5_pt_jetdown"  , &m_addJetCentr5_pt_jetdown);
      m_smallT->Branch ("addJetCentr5_mass_jetup"  , &m_addJetCentr5_mass_jetup);
      m_smallT->Branch ("addJetCentr5_mass_jetdown", &m_addJetCentr5_mass_jetdown);

      m_smallT->Branch ("addJetCentr1_pt_jetupTot"    , &m_addJetCentr1_pt_jetupTot    , "addJetCentr1_pt_jetupTot/F");
      m_smallT->Branch ("addJetCentr1_pt_jetdownTot"  , &m_addJetCentr1_pt_jetdownTot  , "addJetCentr1_pt_jetdownTot/F");
      m_smallT->Branch ("addJetCentr1_mass_jetupTot"  , &m_addJetCentr1_mass_jetupTot  , "addJetCentr1_mass_jetupTot/F");
      m_smallT->Branch ("addJetCentr1_mass_jetdownTot", &m_addJetCentr1_mass_jetdownTot, "addJetCentr1_mass_jetdownTot/F");
      m_smallT->Branch ("addJetCentr2_pt_jetupTot"    , &m_addJetCentr2_pt_jetupTot    , "addJetCentr2_pt_jetupTot/F");
      m_smallT->Branch ("addJetCentr2_pt_jetdownTot"  , &m_addJetCentr2_pt_jetdownTot  , "addJetCentr2_pt_jetdownTot/F");
      m_smallT->Branch ("addJetCentr2_mass_jetupTot"  , &m_addJetCentr2_mass_jetupTot  , "addJetCentr2_mass_jetupTot/F");
      m_smallT->Branch ("addJetCentr2_mass_jetdownTot", &m_addJetCentr2_mass_jetdownTot, "addJetCentr2_mass_jetdownTot/F");
      m_smallT->Branch ("addJetCentr3_pt_jetupTot"    , &m_addJetCentr3_pt_jetupTot    , "addJetCentr3_pt_jetupTot/F");
      m_smallT->Branch ("addJetCentr3_pt_jetdownTot"  , &m_addJetCentr3_pt_jetdownTot  , "addJetCentr3_pt_jetdownTot/F");
      m_smallT->Branch ("addJetCentr3_mass_jetupTot"  , &m_addJetCentr3_mass_jetupTot  , "addJetCentr3_mass_jetupTot/F");
      m_smallT->Branch ("addJetCentr3_mass_jetdownTot", &m_addJetCentr3_mass_jetdownTot, "addJetCentr3_mass_jetdownTot/F");
      m_smallT->Branch ("addJetCentr4_pt_jetupTot"    , &m_addJetCentr4_pt_jetupTot    , "addJetCentr4_pt_jetupTot/F");
      m_smallT->Branch ("addJetCentr4_pt_jetdownTot"  , &m_addJetCentr4_pt_jetdownTot  , "addJetCentr4_pt_jetdownTot/F");
      m_smallT->Branch ("addJetCentr4_mass_jetupTot"  , &m_addJetCentr4_mass_jetupTot  , "addJetCentr4_mass_jetupTot/F");
      m_smallT->Branch ("addJetCentr4_mass_jetdownTot", &m_addJetCentr4_mass_jetdownTot, "addJetCentr4_mass_jetdownTot/F");
      m_smallT->Branch ("addJetCentr5_pt_jetupTot"    , &m_addJetCentr5_pt_jetupTot    , "addJetCentr5_pt_jetupTot/F");
      m_smallT->Branch ("addJetCentr5_pt_jetdownTot"  , &m_addJetCentr5_pt_jetdownTot  , "addJetCentr5_pt_jetdownTot/F");
      m_smallT->Branch ("addJetCentr5_mass_jetupTot"  , &m_addJetCentr5_mass_jetupTot  , "addJetCentr5_mass_jetupTot/F");
      m_smallT->Branch ("addJetCentr5_mass_jetdownTot", &m_addJetCentr5_mass_jetdownTot, "addJetCentr5_mass_jetdownTot/F");

      // Additional forward jets
      m_smallT->Branch ("addJetForw1_pt"  , &m_addJetForw1_pt  , "addJetForw1_pt/F");
      m_smallT->Branch ("addJetForw2_pt"  , &m_addJetForw2_pt  , "addJetForw2_pt/F");
      m_smallT->Branch ("addJetForw3_pt"  , &m_addJetForw3_pt  , "addJetForw3_pt/F");
      m_smallT->Branch ("addJetForw4_pt"  , &m_addJetForw4_pt  , "addJetForw4_pt/F");
      m_smallT->Branch ("addJetForw5_pt"  , &m_addJetForw5_pt  , "addJetForw5_pt/F");
      m_smallT->Branch ("addJetForw1_eta" , &m_addJetForw1_eta , "addJetForw1_eta/F");
      m_smallT->Branch ("addJetForw2_eta" , &m_addJetForw2_eta , "addJetForw2_eta/F");
      m_smallT->Branch ("addJetForw3_eta" , &m_addJetForw3_eta , "addJetForw3_eta/F");
      m_smallT->Branch ("addJetForw4_eta" , &m_addJetForw4_eta , "addJetForw4_eta/F");
      m_smallT->Branch ("addJetForw5_eta" , &m_addJetForw5_eta , "addJetForw5_eta/F");
      m_smallT->Branch ("addJetForw1_phi" , &m_addJetForw1_phi , "addJetForw1_phi/F");
      m_smallT->Branch ("addJetForw2_phi" , &m_addJetForw2_phi , "addJetForw2_phi/F");
      m_smallT->Branch ("addJetForw3_phi" , &m_addJetForw3_phi , "addJetForw3_phi/F");
      m_smallT->Branch ("addJetForw4_phi" , &m_addJetForw4_phi , "addJetForw4_phi/F");
      m_smallT->Branch ("addJetForw5_phi" , &m_addJetForw5_phi , "addJetForw5_phi/F");
      m_smallT->Branch ("addJetForw1_e"   , &m_addJetForw1_e   , "addJetForw1_e/F");
      m_smallT->Branch ("addJetForw2_e"   , &m_addJetForw2_e   , "addJetForw2_e/F");
      m_smallT->Branch ("addJetForw3_e"   , &m_addJetForw3_e   , "addJetForw3_e/F");
      m_smallT->Branch ("addJetForw4_e"   , &m_addJetForw4_e   , "addJetForw4_e/F");
      m_smallT->Branch ("addJetForw5_e"   , &m_addJetForw5_e   , "addJetForw5_e/F");
      m_smallT->Branch ("addJetForw1_smearFactor" , &m_addJetForw1_smearFactor , "addJetForw1_smearFactor/F");
      m_smallT->Branch ("addJetForw2_smearFactor" , &m_addJetForw2_smearFactor , "addJetForw2_smearFactor/F");
      m_smallT->Branch ("addJetForw3_smearFactor" , &m_addJetForw3_smearFactor , "addJetForw3_smearFactor/F");
      m_smallT->Branch ("addJetForw4_smearFactor" , &m_addJetForw4_smearFactor , "addJetForw4_smearFactor/F");
      m_smallT->Branch ("addJetForw5_smearFactor" , &m_addJetForw5_smearFactor , "addJetForw5_smearFactor/F");

      m_smallT->Branch ("addJetForw1_pt_jetup"    , &m_addJetForw1_pt_jetup);
      m_smallT->Branch ("addJetForw1_pt_jetdown"  , &m_addJetForw1_pt_jetdown);
      m_smallT->Branch ("addJetForw1_mass_jetup"  , &m_addJetForw1_mass_jetup);
      m_smallT->Branch ("addJetForw1_mass_jetdown", &m_addJetForw1_mass_jetdown);
      m_smallT->Branch ("addJetForw2_pt_jetup"    , &m_addJetForw2_pt_jetup);
      m_smallT->Branch ("addJetForw2_pt_jetdown"  , &m_addJetForw2_pt_jetdown);
      m_smallT->Branch ("addJetForw2_mass_jetup"  , &m_addJetForw2_mass_jetup);
      m_smallT->Branch ("addJetForw2_mass_jetdown", &m_addJetForw2_mass_jetdown);
      m_smallT->Branch ("addJetForw3_pt_jetup"    , &m_addJetForw3_pt_jetup);
      m_smallT->Branch ("addJetForw3_pt_jetdown"  , &m_addJetForw3_pt_jetdown);
      m_smallT->Branch ("addJetForw3_mass_jetup"  , &m_addJetForw3_mass_jetup);
      m_smallT->Branch ("addJetForw3_mass_jetdown", &m_addJetForw3_mass_jetdown);
      m_smallT->Branch ("addJetForw4_pt_jetup"    , &m_addJetForw4_pt_jetup);
      m_smallT->Branch ("addJetForw4_pt_jetdown"  , &m_addJetForw4_pt_jetdown);
      m_smallT->Branch ("addJetForw4_mass_jetup"  , &m_addJetForw4_mass_jetup);
      m_smallT->Branch ("addJetForw4_mass_jetdown", &m_addJetForw4_mass_jetdown);
      m_smallT->Branch ("addJetForw5_pt_jetup"    , &m_addJetForw5_pt_jetup);
      m_smallT->Branch ("addJetForw5_pt_jetdown"  , &m_addJetForw5_pt_jetdown);
      m_smallT->Branch ("addJetForw5_mass_jetup"  , &m_addJetForw5_mass_jetup);
      m_smallT->Branch ("addJetForw5_mass_jetdown", &m_addJetForw5_mass_jetdown);

      m_smallT->Branch ("addJetForw1_pt_jetupTot"    , &m_addJetForw1_pt_jetupTot    , "addJetForw1_pt_jetupTot/F");
      m_smallT->Branch ("addJetForw1_pt_jetdownTot"  , &m_addJetForw1_pt_jetdownTot  , "addJetForw1_pt_jetdownTot/F");
      m_smallT->Branch ("addJetForw1_mass_jetupTot"  , &m_addJetForw1_mass_jetupTot  , "addJetForw1_mass_jetupTot/F");
      m_smallT->Branch ("addJetForw1_mass_jetdownTot", &m_addJetForw1_mass_jetdownTot, "addJetForw1_mass_jetdownTot/F");
      m_smallT->Branch ("addJetForw2_pt_jetupTot"    , &m_addJetForw2_pt_jetupTot    , "addJetForw2_pt_jetupTot/F");
      m_smallT->Branch ("addJetForw2_pt_jetdownTot"  , &m_addJetForw2_pt_jetdownTot  , "addJetForw2_pt_jetdownTot/F");
      m_smallT->Branch ("addJetForw2_mass_jetupTot"  , &m_addJetForw2_mass_jetupTot  , "addJetForw2_mass_jetupTot/F");
      m_smallT->Branch ("addJetForw2_mass_jetdownTot", &m_addJetForw2_mass_jetdownTot, "addJetForw2_mass_jetdownTot/F");
      m_smallT->Branch ("addJetForw3_pt_jetupTot"    , &m_addJetForw3_pt_jetupTot    , "addJetForw3_pt_jetupTot/F");
      m_smallT->Branch ("addJetForw3_pt_jetdownTot"  , &m_addJetForw3_pt_jetdownTot  , "addJetForw3_pt_jetdownTot/F");
      m_smallT->Branch ("addJetForw3_mass_jetupTot"  , &m_addJetForw3_mass_jetupTot  , "addJetForw3_mass_jetupTot/F");
      m_smallT->Branch ("addJetForw3_mass_jetdownTot", &m_addJetForw3_mass_jetdownTot, "addJetForw3_mass_jetdownTot/F");
      m_smallT->Branch ("addJetForw4_pt_jetupTot"    , &m_addJetForw4_pt_jetupTot    , "addJetForw4_pt_jetupTot/F");
      m_smallT->Branch ("addJetForw4_pt_jetdownTot"  , &m_addJetForw4_pt_jetdownTot  , "addJetForw4_pt_jetdownTot/F");
      m_smallT->Branch ("addJetForw4_mass_jetupTot"  , &m_addJetForw4_mass_jetupTot  , "addJetForw4_mass_jetupTot/F");
      m_smallT->Branch ("addJetForw4_mass_jetdownTot", &m_addJetForw4_mass_jetdownTot, "addJetForw4_mass_jetdownTot/F");
      m_smallT->Branch ("addJetForw5_pt_jetupTot"    , &m_addJetForw5_pt_jetupTot    , "addJetForw5_pt_jetupTot/F");
      m_smallT->Branch ("addJetForw5_pt_jetdownTot"  , &m_addJetForw5_pt_jetdownTot  , "addJetForw5_pt_jetdownTot/F");
      m_smallT->Branch ("addJetForw5_mass_jetupTot"  , &m_addJetForw5_mass_jetupTot  , "addJetForw5_mass_jetupTot/F");
      m_smallT->Branch ("addJetForw5_mass_jetdownTot", &m_addJetForw5_mass_jetdownTot, "addJetForw5_mass_jetdownTot/F");

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
      m_smallT->Branch ("BDT_channel",                    &m_BDT_channel                   , "BDT_channel/F");
      m_smallT->Branch ("tauH_MET_pt",                    &m_tauH_MET_pt                   , "tauH_MET_pt/F");
      m_smallT->Branch ("dau2_MET_deltaEta",              &m_dau2_MET_deltaEta             , "dau2_MET_deltaEta/F");
      m_smallT->Branch ("bH_MET_deltaEta",                &m_bH_MET_deltaEta               , "bH_MET_deltaEta/F");
      m_smallT->Branch ("bH_MET_deltaR",                  &m_bH_MET_deltaR                 , "bH_MET_deltaR/F");
      m_smallT->Branch ("bH_tauH_MET_deltaR",             &m_bH_tauH_MET_deltaR            , "bH_tauH_MET_deltaR/F");
      m_smallT->Branch ("ditau_deltaR_per_tauH_MET_pt",   &m_ditau_deltaR_per_tauH_MET_pt  , "ditau_deltaR_per_tauH_MET_pt/F");
      m_smallT->Branch ("p_zeta",                         &m_p_zeta                        , "p_zeta/F");
      m_smallT->Branch ("p_zeta_visible",                 &m_p_zeta_visible                , "p_zeta_visible/F");
      m_smallT->Branch ("mT_tauH_MET",                    &m_mT_tauH_MET                   , "mT_tauH_MET/F");
      m_smallT->Branch ("mT_tauH_SVFIT_MET",              &m_mT_tauH_SVFIT_MET             , "mT_tauH_SVFIT_MET/F");
      m_smallT->Branch ("mT_total",                       &m_mT_total                      , "mT_total/F");
      m_smallT->Branch ("BDT_ditau_deltaPhi",             &m_BDT_ditau_deltaPhi            , "m_BDT_ditau_deltaPhi/F");
      m_smallT->Branch ("BDT_dib_abs_deltaPhi",           &m_BDT_dib_abs_deltaPhi          , "BDT_dib_abs_deltaPhi/F");
      m_smallT->Branch ("BDT_dib_deltaPhi",               &m_BDT_dib_deltaPhi              , "BDT_dib_deltaPhi/F");
      m_smallT->Branch ("BDT_dau1MET_deltaPhi",           &m_BDT_dau1MET_deltaPhi          , "BDT_dau1MET_deltaPhi/F");
      m_smallT->Branch ("BDT_tauHsvfitMet_abs_deltaPhi",  &m_BDT_tauHsvfitMet_abs_deltaPhi , "BDT_tauHsvfitMet_abs_deltaPhi/F");
      m_smallT->Branch ("BDT_tauHsvfitMet_deltaPhi",      &m_BDT_tauHsvfitMet_deltaPhi     , "BDT_tauHsvfitMet_deltaPhi/F");
      m_smallT->Branch ("BDT_bHMet_deltaPhi",             &m_BDT_bHMet_deltaPhi            , "BDT_bHMet_deltaPhi/F");
      m_smallT->Branch ("BDT_HHsvfit_abs_deltaPhi",       &m_BDT_HHsvfit_abs_deltaPhi      , "BDT_HHsvfit_abs_deltaPhi/F");
      m_smallT->Branch ("BDT_HT20",                       &m_BDT_HT20                      , "BDT_HT20/F");
      m_smallT->Branch ("BDT_HT20_jetup",                 &m_BDT_HT20_jetup);
      m_smallT->Branch ("BDT_HT20_jetdown",               &m_BDT_HT20_jetdown);
      m_smallT->Branch ("BDT_HT20_jetupTot",              &m_BDT_HT20_jetupTot             , "BDT_HT20_jetupTot/F");
      m_smallT->Branch ("BDT_HT20_jetdownTot",            &m_BDT_HT20_jetdownTot           , "BDT_HT20_jetdownTot/F");
      m_smallT->Branch ("BDT_topPairMasses",              &m_BDT_topPairMasses             , "BDT_topPairMasses/F");
      m_smallT->Branch ("BDT_topPairMasses2",             &m_BDT_topPairMasses2            , "BDT_topPairMasses2/F");
      m_smallT->Branch ("BDT_MX",                         &m_BDT_MX                        , "BDT_MX/F");
      m_smallT->Branch ("BDT_bH_tauH_MET_InvMass",        &m_BDT_bH_tauH_MET_InvMass       , "BDT_bH_tauH_MET_InvMass/F");
      m_smallT->Branch ("BDT_bH_tauH_SVFIT_InvMass",      &m_BDT_bH_tauH_SVFIT_InvMass     , "BDT_bH_tauH_SVFIT_InvMass/F");
      m_smallT->Branch ("BDT_bH_tauH_InvMass",            &m_BDT_bH_tauH_InvMass           , "BDT_bH_tauH_InvMass/F");
      m_smallT->Branch ("BDT_total_CalcPhi",              &m_BDT_total_CalcPhi             , "BDT_total_CalcPhi/F");
      m_smallT->Branch ("BDT_ditau_CalcPhi",              &m_BDT_ditau_CalcPhi             , "BDT_ditau_CalcPhi/F");
      m_smallT->Branch ("BDT_dib_CalcPhi",                &m_BDT_dib_CalcPhi               , "BDT_dib_CalcPhi/F");
      m_smallT->Branch ("BDT_MET_tauH_SVFIT_cosTheta",    &m_BDT_MET_tauH_SVFIT_cosTheta   , "BDT_MET_tauH_SVFIT_cosTheta/F");
      m_smallT->Branch ("BDT_MET_bH_cosTheta",            &m_BDT_MET_bH_cosTheta           , "BDT_MET_bH_cosTheta/F");
      m_smallT->Branch ("BDT_b1_bH_cosTheta",             &m_BDT_b1_bH_cosTheta            , "BDT_b1_bH_cosTheta/F");
      m_smallT->Branch ("BDT_tauH_SVFIT_reson_cosTheta",  &m_BDT_tauH_SVFIT_reson_cosTheta , "BDT_tauH_SVFIT_reson_cosTheta/F");

      m_smallT->Branch ("VBFjj_dEtaSign",   &m_VBFjj_dEtaSign   , "VBFjj_dEtaSign/F");
      m_smallT->Branch ("VBFjet2_PUjetID",  &m_VBFjet2_PUjetID  , "VBFjet2_PUjetID/F");
      m_smallT->Branch ("bH_VBF1_deltaEta", &m_bH_VBF1_deltaEta , "bH_VBF1_deltaEta/F");
      m_smallT->Branch ("dib_dEtaSign",     &m_dib_dEtaSign     , "dib_dEtaSign/F");

      m_smallT->Branch ("isTau1real",     &m_isTau1real     , "isTau1real/I");
      m_smallT->Branch ("isTau2real",     &m_isTau2real     , "isTau2real/I");
      m_smallT->Branch ("nRealTaus",      &m_nRealTaus     , "nRealTaus/I");

      return 0 ;
    }

  // the tree itself
  TTree * m_smallT ;

  // general variables
  Float_t m_MC_weight ;
  Float_t m_totalWeight ;
  Float_t m_prescaleWeight ;
  Float_t m_L1pref_weight ;
  Float_t m_PUjetID_SF ;
  Float_t m_PUjetID_SF_up ;
  Float_t m_PUjetID_SF_down ;
  Float_t m_PUjetID_SF_eff_up ;
  Float_t m_PUjetID_SF_eff_down ;
  Float_t m_PUjetID_SF_mistag_up ;
  Float_t m_PUjetID_SF_mistag_down ;
  Float_t m_PUjetID_SF_eff_eta_s2p5_up ;
  Float_t m_PUjetID_SF_eff_eta_s2p5_down ;
  Float_t m_PUjetID_SF_mistag_eta_s2p5_up ;
  Float_t m_PUjetID_SF_mistag_eta_s2p5_down ;
  Float_t m_PUjetID_SF_eff_eta_l2p5_up ;
  Float_t m_PUjetID_SF_eff_eta_l2p5_down ;
  Float_t m_PUjetID_SF_mistag_eta_l2p5_up ;
  Float_t m_PUjetID_SF_mistag_eta_l2p5_down ;
  Float_t m_PUReweight ;
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
  Float_t m_trigSF_ele_up ;
  Float_t m_trigSF_mu_up ;
  Float_t m_trigSF_DM0_up ;
  Float_t m_trigSF_DM1_up ;
  Float_t m_trigSF_DM10_up ;
  Float_t m_trigSF_DM11_up ;
  Float_t m_trigSF_ele_down ;
  Float_t m_trigSF_mu_down ;
  Float_t m_trigSF_DM0_down ;
  Float_t m_trigSF_DM1_down ;
  Float_t m_trigSF_DM10_down ;
  Float_t m_trigSF_DM11_down ;
  Float_t m_trigSF_cross ;
  Float_t m_trigSF_single ;
  Float_t m_VBFtrigSF;
  Float_t m_trigSF_vbfjet_up;
  Float_t m_trigSF_vbfjet_down;
  Float_t m_FakeRateSF ;
  Float_t m_jetFakeSF ;
  Float_t m_IdSF_deep_dm ;
  Float_t m_IdSF_deep_pt ;
  Float_t m_IdSF_deep_2d ;
  Float_t m_IdSF_leg1_deep_vsJet_2d;
  Float_t m_IdSF_leg2_deep_vsJet_2d;
  Float_t m_IdFakeSF_deep_dm ;
  Float_t m_IdFakeSF_deep_pt ;
  Float_t m_IdFakeSF_deep_2d ;
  Float_t m_FakeRateSF_deep;
  Float_t m_customTauIdSF;
  Float_t m_customTauIdSF_DM0_up;
  Float_t m_customTauIdSF_DM0_down;
  Float_t m_customTauIdSF_DM1_up;
  Float_t m_customTauIdSF_DM1_down;
  Float_t m_customTauIdSF_DM10_up;
  Float_t m_customTauIdSF_DM10_down;
  Float_t m_customTauIdSF_DM11_up;
  Float_t m_customTauIdSF_DM11_down;

  Float_t m_idFakeSF_tauid_2d_stat0_up;
  Float_t m_idFakeSF_tauid_2d_stat0_down;
  Float_t m_idFakeSF_tauid_2d_stat1_up;
  Float_t m_idFakeSF_tauid_2d_stat1_down;
  Float_t m_idFakeSF_tauid_2d_systcorrdmeras_up;
  Float_t m_idFakeSF_tauid_2d_systcorrdmeras_down;
  Float_t m_idFakeSF_tauid_2d_systcorrdmuncorreras_up;
  Float_t m_idFakeSF_tauid_2d_systcorrdmuncorreras_down;
  Float_t m_idFakeSF_tauid_2d_systuncorrdmeras_up;
  Float_t m_idFakeSF_tauid_2d_systuncorrdmeras_down;
  Float_t m_idFakeSF_tauid_2d_systcorrerasgt140_up;
  Float_t m_idFakeSF_tauid_2d_systcorrerasgt140_down;
  Float_t m_idFakeSF_tauid_2d_statgt140_up;
  Float_t m_idFakeSF_tauid_2d_statgt140_down;
  Float_t m_idFakeSF_tauid_2d_extrapgt140;

  Float_t m_idFakeSF_tauid_pt20to25_up       ;
  Float_t m_idFakeSF_tauid_pt25to30_up       ;
  Float_t m_idFakeSF_tauid_pt30to35_up       ;
  Float_t m_idFakeSF_tauid_pt35to40_up       ;
  Float_t m_idFakeSF_tauid_pt40toInf_up      ;
  Float_t m_idFakeSF_tauid_pt20to25_down     ;
  Float_t m_idFakeSF_tauid_pt25to30_down     ;
  Float_t m_idFakeSF_tauid_pt30to35_down     ;
  Float_t m_idFakeSF_tauid_pt35to40_down     ;
  Float_t m_idFakeSF_tauid_pt40toInf_down    ;
  Float_t m_idFakeSF_mutauFR_etaLt0p4_up     ;
  Float_t m_idFakeSF_mutauFR_eta0p4to0p8_up  ;
  Float_t m_idFakeSF_mutauFR_eta0p8to1p2_up  ;
  Float_t m_idFakeSF_mutauFR_eta1p2to1p7_up  ;
  Float_t m_idFakeSF_mutauFR_etaGt1p7_up     ;
  Float_t m_idFakeSF_mutauFR_etaLt0p4_down   ;
  Float_t m_idFakeSF_mutauFR_eta0p4to0p8_down;
  Float_t m_idFakeSF_mutauFR_eta0p8to1p2_down;
  Float_t m_idFakeSF_mutauFR_eta1p2to1p7_down;
  Float_t m_idFakeSF_mutauFR_etaGt1p7_down   ;
  Float_t m_idFakeSF_etauFR_barrel_up        ;
  Float_t m_idFakeSF_etauFR_endcap_up        ;
  Float_t m_idFakeSF_etauFR_barrel_down      ;
  Float_t m_idFakeSF_etauFR_endcap_down      ;
  Float_t m_DYscale_LL ;
  Float_t m_DYscale_MM ;
  Float_t m_DYscale_MH ;
  Float_t m_DYscale_MTT ;
  Float_t m_DYscale_MTT_up ;
  Float_t m_DYscale_MTT_down ;
  Float_t m_genZ_pt ;
  Int_t m_nBhadrons ;
  Int_t m_lheNOutPartons ;
  Int_t m_lheNOutB ;
  ULong64_t m_EventNumber ;
  Int_t m_RunNumber ;
  Int_t m_isBoosted ;
  Int_t m_isVBF;
  Int_t m_isVBFtrigger;

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
  Bool_t m_cross_monitoring_trig ;
  Float_t m_rho ;

  Int_t m_pairType ;
  Int_t m_isMC ;
  Int_t m_isOS ;

//FIXME what about these?
//    Int_t           metfilterbit;
//    Float_t         met;
//    Float_t         metphi;

  // MET
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

  std::vector<Float_t> m_METx_jetup ;
  std::vector<Float_t> m_METx_tauup ;
  std::vector<Float_t> m_METx_eleup ;
  Float_t m_METx_muup;
  Float_t m_METx_jetupTot;
  std::vector<Float_t> m_METx_jetdown ;
  std::vector<Float_t> m_METx_taudown ;
  std::vector<Float_t> m_METx_eledown ;
  Float_t m_METx_mudown;
  Float_t m_METx_jetdownTot;
  std::vector<Float_t> m_METy_jetup ;
  std::vector<Float_t> m_METy_tauup ;
  std::vector<Float_t> m_METy_eleup ;
  Float_t m_METy_muup;
  Float_t m_METy_jetupTot;
  std::vector<Float_t> m_METy_jetdown ;
  std::vector<Float_t> m_METy_taudown ;
  std::vector<Float_t> m_METy_eledown ;
  Float_t m_METy_mudown;
  Float_t m_METy_jetdownTot;

  Float_t m_met_et_corr;
  Float_t m_met_cov00;
  Float_t m_met_cov01;
  Float_t m_met_cov10;
  Float_t m_met_cov11;
  // mt
  Float_t m_mT1 ;
  Float_t m_mT2 ;

  Float_t m_DeepMET_ResponseTune_px;
  std::vector<Float_t> m_DeepMET_ResponseTune_px_tauup;
  std::vector<Float_t> m_DeepMET_ResponseTune_px_taudown;
  std::vector<Float_t> m_DeepMET_ResponseTune_px_eleup;
  std::vector<Float_t> m_DeepMET_ResponseTune_px_eledown;
  Float_t m_DeepMET_ResponseTune_px_muup;
  Float_t m_DeepMET_ResponseTune_px_mudown;

  Float_t m_DeepMET_ResponseTune_py;
  std::vector<Float_t> m_DeepMET_ResponseTune_py_tauup;
  std::vector<Float_t> m_DeepMET_ResponseTune_py_taudown;
  std::vector<Float_t> m_DeepMET_ResponseTune_py_eleup;
  std::vector<Float_t> m_DeepMET_ResponseTune_py_eledown;
  Float_t m_DeepMET_ResponseTune_py_muup;
  Float_t m_DeepMET_ResponseTune_py_mudown;

  Float_t m_DeepMET_ResolutionTune_px;
  std::vector<Float_t> m_DeepMET_ResolutionTune_px_tauup;
  std::vector<Float_t> m_DeepMET_ResolutionTune_px_taudown;
  std::vector<Float_t> m_DeepMET_ResolutionTune_px_eleup;
  std::vector<Float_t> m_DeepMET_ResolutionTune_px_eledown;
  Float_t m_DeepMET_ResolutionTune_px_muup;
  Float_t m_DeepMET_ResolutionTune_px_mudown;

  Float_t m_DeepMET_ResolutionTune_py;
  std::vector<Float_t> m_DeepMET_ResolutionTune_py_tauup;
  std::vector<Float_t> m_DeepMET_ResolutionTune_py_taudown;
  std::vector<Float_t> m_DeepMET_ResolutionTune_py_eleup;
  std::vector<Float_t> m_DeepMET_ResolutionTune_py_eledown;
  Float_t m_DeepMET_ResolutionTune_py_muup;
  Float_t m_DeepMET_ResolutionTune_py_mudown;

  // the largest pT daughter visible candidate
  Float_t m_dau1_iso ;
  Int_t   m_dau1_eleMVAiso;
  Int_t   m_dau1_CUTiso; // for taus only
  Int_t   m_dau1_deepTauVsJet;
  Int_t   m_dau1_deepTauVsEle;
  Int_t   m_dau1_deepTauVsMu;

  Bool_t m_dau1_byLooseCombinedIsolationDeltaBetaCorr3Hits;
  Bool_t m_dau1_byMediumCombinedIsolationDeltaBetaCorr3Hits;
  Bool_t m_dau1_byTightCombinedIsolationDeltaBetaCorr3Hits;
  Float_t m_dau1_pt ;
  std::vector <Float_t> m_dau1_pt_tauup ;
  std::vector <Float_t> m_dau1_pt_taudown ;
  std::vector <Float_t> m_dau1_mass_tauup ;
  std::vector <Float_t> m_dau1_mass_taudown ;
  std::vector <Float_t> m_dau1_pt_eleup ;
  std::vector <Float_t> m_dau1_pt_eledown ;
  std::vector <Float_t> m_dau1_mass_eleup ;
  std::vector <Float_t> m_dau1_mass_eledown ;

  Float_t m_dau1_pt_muup ;
  Float_t m_dau1_pt_mudown ;
  Float_t m_dau1_mass_muup ;
  Float_t m_dau1_mass_mudown ;

  Float_t m_dau1_pt_tauup_DM0 ;
  Float_t m_dau1_pt_tauup_DM1 ;
  Float_t m_dau1_pt_tauup_DM10 ;
  Float_t m_dau1_pt_tauup_DM11 ;
  Float_t m_dau1_pt_taudown_DM0 ;
  Float_t m_dau1_pt_taudown_DM1 ;
  Float_t m_dau1_pt_taudown_DM10 ;
  Float_t m_dau1_pt_taudown_DM11 ;
  Float_t m_dau1_pt_eleup_DM0 ;
  Float_t m_dau1_pt_eleup_DM1 ;
  Float_t m_dau1_pt_eledown_DM0 ;
  Float_t m_dau1_pt_eledown_DM1 ;

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
  Int_t   m_dau2_CUTiso; // for taus only
  Int_t   m_dau2_deepTauVsJet;
  Int_t   m_dau2_deepTauVsEle;
  Int_t   m_dau2_deepTauVsMu;
  Bool_t m_dau2_byLooseCombinedIsolationDeltaBetaCorr3Hits;
  Bool_t m_dau2_byMediumCombinedIsolationDeltaBetaCorr3Hits;
  Bool_t m_dau2_byTightCombinedIsolationDeltaBetaCorr3Hits;

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
  std::vector <Float_t> m_dau2_pt_tauup ;
  std::vector <Float_t> m_dau2_pt_taudown ;
  std::vector <Float_t> m_dau2_mass_tauup ;
  std::vector <Float_t> m_dau2_mass_taudown ;
  std::vector <Float_t> m_dau2_pt_eleup ;
  std::vector <Float_t> m_dau2_pt_eledown ;
  std::vector <Float_t> m_dau2_mass_eleup ;
  std::vector <Float_t> m_dau2_mass_eledown ;

  Float_t m_dau2_pt_muup ;
  Float_t m_dau2_pt_mudown ;
  Float_t m_dau2_mass_muup ;
  Float_t m_dau2_mass_mudown ;

  Float_t m_dau2_pt_tauup_DM0 ;
  Float_t m_dau2_pt_tauup_DM1 ;
  Float_t m_dau2_pt_tauup_DM10 ;
  Float_t m_dau2_pt_tauup_DM11 ;
  Float_t m_dau2_pt_taudown_DM0 ;
  Float_t m_dau2_pt_taudown_DM1 ;
  Float_t m_dau2_pt_taudown_DM10 ;
  Float_t m_dau2_pt_taudown_DM11 ;
  Float_t m_dau2_pt_eleup_DM0 ;
  Float_t m_dau2_pt_eleup_DM1 ;
  Float_t m_dau2_pt_eledown_DM0 ;
  Float_t m_dau2_pt_eledown_DM1 ;
  Float_t m_dau2_eta ;
  Float_t m_dau2_phi ;
  Float_t m_dau2_e ;
  Float_t m_dau2_flav ; // let this contain also whether it's a hadronic dau
  Float_t m_dau2_dxy ;
  Float_t m_dau2_dz ;
  Int_t m_dau2_decayMode ;

  // the largest pT b visible candidate /  highest CSV score
  Float_t m_bjet1_pt ;
  Float_t m_bjet1_eta ;
  Float_t m_bjet1_phi ;
  Float_t m_bjet1_e ;
  Float_t m_bjet1_bID ;
  Float_t m_bjet1_bID_deepCSV ;
  Float_t m_bjet1_bID_deepFlavor ;
  Float_t m_bjet1_cID_deepFlavor ;
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
  Float_t m_bjet1_bMVAID ;
  Float_t m_bjet1_PUjetIDupdated;
  Int_t   m_bjet1_flav ;
  Float_t m_bjet1_pt_raw ;

  std::vector <Float_t> m_bjet1_pt_raw_jetup ;
  std::vector <Float_t> m_bjet1_pt_raw_jetdown ;
  std::vector <Float_t> m_bjet1_mass_raw_jetup ;
  std::vector <Float_t> m_bjet1_mass_raw_jetdown ;

  Bool_t  m_bjet1_hasgenjet ;
  Float_t m_bjet1_JER ;
  std::vector <Float_t> m_bjet1_JER_jetup;
  std::vector <Float_t> m_bjet1_JER_jetdown;
  Float_t m_bjet1_JER_jetupTot ;
  Float_t m_bjet1_JER_jetdownTot ;

  Bool_t  m_bjet1_gen_matched;

  // the trailing pT b visible candidate  /  highest CSV score
  Float_t m_bjet2_pt ;
  Float_t m_bjet2_eta ;
  Float_t m_bjet2_phi ;
  Float_t m_bjet2_e ;
  Float_t m_bjet2_bID ;
  Float_t m_bjet2_bID_deepCSV ;
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
  Float_t m_bjet2_pt_raw ;

  std::vector <Float_t> m_bjet2_pt_raw_jetup ;
  std::vector <Float_t> m_bjet2_pt_raw_jetdown ;
  std::vector <Float_t> m_bjet2_mass_raw_jetup ;
  std::vector <Float_t> m_bjet2_mass_raw_jetdown ;

  Bool_t  m_bjet2_hasgenjet ;
  Float_t m_bjet2_JER ;
  std::vector <Float_t> m_bjet2_JER_jetup;
  std::vector <Float_t> m_bjet2_JER_jetdown;
  Float_t m_bjet2_JER_jetupTot ;
  Float_t m_bjet2_JER_jetdownTot ;

  Bool_t  m_bjet2_gen_matched;

  Float_t m_bjets_bID;
  Float_t m_bjets_bID_deepCSV;
  Float_t m_bjets_bID_deepFlavor;
  Float_t m_bjets_cID_deepFlavor;
  Bool_t  m_bjets_gen_matched;
  // fat jets and subjet info
  Int_t   m_nfatjets ;
  Float_t m_fatjet_pt ;
  Float_t m_fatjet_eta  ;
  Float_t m_fatjet_phi  ;
  Float_t m_fatjet_e  ;
  Float_t m_fatjet_bID ;
  Float_t m_fatjet_bID_deepCSV ;
  Float_t m_fatjet_bID_deepFlavor ;
  Float_t m_fatjet_massIndependentDeepDoubleBvLJetTags_probHbb;
  Float_t m_fatjet_deepDoubleBvLJetTags_probHbb;
  Float_t m_fatjet_deepBoostedJetTags_probHbb;
  Float_t m_fatjet_particleNetJetTags_probHbb;
  Float_t m_fatjet_particleNetDiscriminatorsJetTags_HbbvsQCD;
  Float_t m_fatjet_filteredMass ;
  Float_t m_fatjet_prunedMass ;
  Float_t m_fatjet_trimmedMass ;
  Float_t m_fatjet_softdropMass ;
  Float_t m_fatjet_tau1 ;
  Float_t m_fatjet_tau2 ;
  Float_t m_fatjet_tau3 ;
  Int_t   m_fatjet_nsubjets ;
  Float_t m_dR_subj1_subj2 ;

  Float_t m_subjetjet1_pt ;
  Float_t m_subjetjet1_eta  ;
  Float_t m_subjetjet1_phi  ;
  Float_t m_subjetjet1_e  ;
  Float_t m_subjetjet1_bID ;
  Float_t m_subjetjet1_bID_deepCSV ;
  Float_t m_subjetjet1_bID_deepFlavor ;

  Float_t m_subjetjet2_pt ;
  Float_t m_subjetjet2_eta  ;
  Float_t m_subjetjet2_phi  ;
  Float_t m_subjetjet2_e  ;
  Float_t m_subjetjet2_bID ;
  Float_t m_subjetjet2_bID_deepCSV ;
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
  Float_t m_tauH_SVFIT_mass_up ;
  Float_t m_tauH_SVFIT_mass_down ;
  Float_t m_tauH_SVFIT_mass_METup ;
  Float_t m_tauH_SVFIT_mass_METdown ;

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
  Float_t m_HH_mass_raw ;

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
  Float_t m_ditau_deltaR_per_tauHsvfitpt ;
  Float_t m_dib_deltaR_per_bHpt ;
  Float_t m_btau_deltaRmin ;
  Float_t m_btau_deltaRmax ;
  Float_t m_dau1MET_deltaphi ;
  Float_t m_dau2MET_deltaphi ;

  Float_t m_HT20 ;
  Float_t m_HT50 ;
  Float_t m_HT20Full ;
  Float_t m_jet20centrality ;


  // additional jets
  std::vector<Float_t> m_jets_pt ;
  std::vector<Float_t> m_jets_eta ;
  std::vector<Float_t> m_jets_phi ;
  std::vector<Float_t> m_jets_e ;
  std::vector<Float_t> m_jets_btag ;
  std::vector<Float_t> m_jets_btag_deepCSV ;
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
  std::vector<Float_t> m_jets_smearFactor ;
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

  // TMVAs
  //Float_t m_mvaValueMuTau ;
  //Float_t m_mvaValueTauTau ;
  //Float_t m_mvaValueETau ;

  Float_t m_HHKin_mass_raw;
  Float_t m_HHKin_mass_raw_chi2;
  Int_t   m_HHKin_mass_raw_convergence;
  Float_t m_HHKin_mass_raw_prob;

  Float_t m_HHKin_mass_raw_copy;
  Float_t m_bH_mass_raw;
  Float_t m_bH_pt_raw;
  std::vector <Float_t> m_bH_pt_raw_jetup;
  std::vector <Float_t> m_bH_pt_raw_jetdown;
  std::vector <Float_t> m_bH_mass_raw_jetup;
  std::vector <Float_t> m_bH_mass_raw_jetdown;


  Float_t m_bjet1_pt_raw_jetup1 ;
  Float_t m_bjet1_pt_raw_jetup2 ;
  Float_t m_bjet1_pt_raw_jetup3 ;
  Float_t m_bjet1_pt_raw_jetup4 ;
  Float_t m_bjet1_pt_raw_jetup5 ;
  Float_t m_bjet1_pt_raw_jetup6 ;
  Float_t m_bjet1_pt_raw_jetup7 ;
  Float_t m_bjet1_pt_raw_jetup8 ;
  Float_t m_bjet1_pt_raw_jetup9 ;
  Float_t m_bjet1_pt_raw_jetup10;
  Float_t m_bjet1_pt_raw_jetup11;

  Float_t m_bjet1_pt_raw_jetdown1 ;
  Float_t m_bjet1_pt_raw_jetdown2 ;
  Float_t m_bjet1_pt_raw_jetdown3 ;
  Float_t m_bjet1_pt_raw_jetdown4 ;
  Float_t m_bjet1_pt_raw_jetdown5 ;
  Float_t m_bjet1_pt_raw_jetdown6 ;
  Float_t m_bjet1_pt_raw_jetdown7 ;
  Float_t m_bjet1_pt_raw_jetdown8 ;
  Float_t m_bjet1_pt_raw_jetdown9 ;
  Float_t m_bjet1_pt_raw_jetdown10;
  Float_t m_bjet1_pt_raw_jetdown11;

  Float_t m_bjet2_pt_raw_jetup1 ;
  Float_t m_bjet2_pt_raw_jetup2 ;
  Float_t m_bjet2_pt_raw_jetup3 ;
  Float_t m_bjet2_pt_raw_jetup4 ;
  Float_t m_bjet2_pt_raw_jetup5 ;
  Float_t m_bjet2_pt_raw_jetup6 ;
  Float_t m_bjet2_pt_raw_jetup7 ;
  Float_t m_bjet2_pt_raw_jetup8 ;
  Float_t m_bjet2_pt_raw_jetup9 ;
  Float_t m_bjet2_pt_raw_jetup10;
  Float_t m_bjet2_pt_raw_jetup11;

  Float_t m_bjet2_pt_raw_jetdown1 ;
  Float_t m_bjet2_pt_raw_jetdown2 ;
  Float_t m_bjet2_pt_raw_jetdown3 ;
  Float_t m_bjet2_pt_raw_jetdown4 ;
  Float_t m_bjet2_pt_raw_jetdown5 ;
  Float_t m_bjet2_pt_raw_jetdown6 ;
  Float_t m_bjet2_pt_raw_jetdown7 ;
  Float_t m_bjet2_pt_raw_jetdown8 ;
  Float_t m_bjet2_pt_raw_jetdown9 ;
  Float_t m_bjet2_pt_raw_jetdown10;
  Float_t m_bjet2_pt_raw_jetdown11;

  Float_t m_bH_mass_raw_jetup1 ;
  Float_t m_bH_mass_raw_jetup2 ;
  Float_t m_bH_mass_raw_jetup3 ;
  Float_t m_bH_mass_raw_jetup4 ;
  Float_t m_bH_mass_raw_jetup5 ;
  Float_t m_bH_mass_raw_jetup6 ;
  Float_t m_bH_mass_raw_jetup7 ;
  Float_t m_bH_mass_raw_jetup8 ;
  Float_t m_bH_mass_raw_jetup9 ;
  Float_t m_bH_mass_raw_jetup10;
  Float_t m_bH_mass_raw_jetup11;

  Float_t m_bH_mass_raw_jetdown1 ;
  Float_t m_bH_mass_raw_jetdown2 ;
  Float_t m_bH_mass_raw_jetdown3 ;
  Float_t m_bH_mass_raw_jetdown4 ;
  Float_t m_bH_mass_raw_jetdown5 ;
  Float_t m_bH_mass_raw_jetdown6 ;
  Float_t m_bH_mass_raw_jetdown7 ;
  Float_t m_bH_mass_raw_jetdown8 ;
  Float_t m_bH_mass_raw_jetdown9 ;
  Float_t m_bH_mass_raw_jetdown10;
  Float_t m_bH_mass_raw_jetdown11;

  Float_t m_bjet1_pt_raw_jetupTot;
  Float_t m_bjet1_pt_raw_jetdownTot;
  Float_t m_bjet2_pt_raw_jetupTot;
  Float_t m_bjet2_pt_raw_jetdownTot;
  Float_t m_bjet1_mass_raw_jetupTot;
  Float_t m_bjet1_mass_raw_jetdownTot;
  Float_t m_bjet2_mass_raw_jetupTot;
  Float_t m_bjet2_mass_raw_jetdownTot;
  Float_t m_bH_mass_raw_jetupTot;
  Float_t m_bH_mass_raw_jetdownTot;

  Float_t m_HHkinsvfit_bHmass;
  Float_t m_HHkinsvfit_pt;
  Float_t m_HHkinsvfit_eta ;
  Float_t m_HHkinsvfit_phi ;
  Float_t m_HHkinsvfit_e;
  Float_t m_HHkinsvfit_m;

  Float_t m_MT2;

  // Int_t   m_MT2_covMtrxStatus;
  // Float_t m_MT2_EDM;
  // Int_t   m_MT2_hasConverged;
  // Int_t   m_MT2_status;
  // Int_t   m_MT2_ncalls;
  // Float_t m_MT2_l;

  Float_t m_lheht ;
  Float_t m_topReweight;

  Float_t m_VBFjet1_pt;

  std::vector<Float_t> m_VBFjet1_pt_jetup;
  std::vector<Float_t> m_VBFjet1_pt_jetdown;
  std::vector<Float_t> m_VBFjet1_mass_jetup;
  std::vector<Float_t> m_VBFjet1_mass_jetdown;

  Float_t m_VBFjet1_pt_jetup1;
  Float_t m_VBFjet1_pt_jetup2;
  Float_t m_VBFjet1_pt_jetup3;
  Float_t m_VBFjet1_pt_jetup4;
  Float_t m_VBFjet1_pt_jetup5;
  Float_t m_VBFjet1_pt_jetup6;
  Float_t m_VBFjet1_pt_jetup7;
  Float_t m_VBFjet1_pt_jetup8;
  Float_t m_VBFjet1_pt_jetup9;
  Float_t m_VBFjet1_pt_jetup10;
  Float_t m_VBFjet1_pt_jetup11;

  Float_t m_VBFjet1_pt_jetdown1;
  Float_t m_VBFjet1_pt_jetdown2;
  Float_t m_VBFjet1_pt_jetdown3;
  Float_t m_VBFjet1_pt_jetdown4;
  Float_t m_VBFjet1_pt_jetdown5;
  Float_t m_VBFjet1_pt_jetdown6;
  Float_t m_VBFjet1_pt_jetdown7;
  Float_t m_VBFjet1_pt_jetdown8;
  Float_t m_VBFjet1_pt_jetdown9;
  Float_t m_VBFjet1_pt_jetdown10;
  Float_t m_VBFjet1_pt_jetdown11;

  Float_t m_VBFjet1_eta;
  Float_t m_VBFjet1_phi;
  Float_t m_VBFjet1_e;
  Float_t m_VBFjet1_btag;
  Float_t m_VBFjet1_btag_deepCSV;
  Float_t m_VBFjet1_btag_deepFlavor;
  Float_t m_VBFjet1_ctag_deepFlavor;
  Float_t m_VBFjet1_CvsL;
  Float_t m_VBFjet1_CvsB;
  Float_t m_VBFjet1_HHbtag;
  Float_t m_VBFjet1_pnet_bb;
  Float_t m_VBFjet1_pnet_cc;
  Float_t m_VBFjet1_pnet_b;
  Float_t m_VBFjet1_pnet_c;
  Float_t m_VBFjet1_pnet_g;
  Float_t m_VBFjet1_pnet_uds;
  Float_t m_VBFjet1_pnet_pu;
  Float_t m_VBFjet1_pnet_undef;
  Float_t m_VBFjet1_smearFactor;
  Float_t m_VBFjet1_PUjetIDupdated;
  Int_t m_VBFjet1_flav;
  Bool_t m_VBFjet1_hasgenjet;

  Float_t m_VBFgenjet1_pt;
  Float_t m_VBFgenjet1_eta;
  Float_t m_VBFgenjet1_phi;
  Float_t m_VBFgenjet1_e;

  Float_t m_VBFjet2_pt;
  std::vector<Float_t> m_VBFjet2_pt_jetup;
  std::vector<Float_t> m_VBFjet2_pt_jetdown;
  std::vector<Float_t> m_VBFjet2_mass_jetup;
  std::vector<Float_t> m_VBFjet2_mass_jetdown;
  Float_t m_VBFjet2_pt_jetup1;
  Float_t m_VBFjet2_pt_jetup2;
  Float_t m_VBFjet2_pt_jetup3;
  Float_t m_VBFjet2_pt_jetup4;
  Float_t m_VBFjet2_pt_jetup5;
  Float_t m_VBFjet2_pt_jetup6;
  Float_t m_VBFjet2_pt_jetup7;
  Float_t m_VBFjet2_pt_jetup8;
  Float_t m_VBFjet2_pt_jetup9;
  Float_t m_VBFjet2_pt_jetup10;
  Float_t m_VBFjet2_pt_jetup11;

  Float_t m_VBFjet2_pt_jetdown1;
  Float_t m_VBFjet2_pt_jetdown2;
  Float_t m_VBFjet2_pt_jetdown3;
  Float_t m_VBFjet2_pt_jetdown4;
  Float_t m_VBFjet2_pt_jetdown5;
  Float_t m_VBFjet2_pt_jetdown6;
  Float_t m_VBFjet2_pt_jetdown7;
  Float_t m_VBFjet2_pt_jetdown8;
  Float_t m_VBFjet2_pt_jetdown9;
  Float_t m_VBFjet2_pt_jetdown10;
  Float_t m_VBFjet2_pt_jetdown11;

  Float_t m_VBFjet2_eta;
  Float_t m_VBFjet2_phi;
  Float_t m_VBFjet2_e;
  Float_t m_VBFjet2_btag;
  Float_t m_VBFjet2_btag_deepCSV;
  Float_t m_VBFjet2_btag_deepFlavor;
  Float_t m_VBFjet2_ctag_deepFlavor;
  Float_t m_VBFjet2_CvsL;
  Float_t m_VBFjet2_CvsB;
  Float_t m_VBFjet2_HHbtag;
  Float_t m_VBFjet2_pnet_bb;
  Float_t m_VBFjet2_pnet_cc;
  Float_t m_VBFjet2_pnet_b;
  Float_t m_VBFjet2_pnet_c;
  Float_t m_VBFjet2_pnet_g;
  Float_t m_VBFjet2_pnet_uds;
  Float_t m_VBFjet2_pnet_pu;
  Float_t m_VBFjet2_pnet_undef;
  Float_t m_VBFjet2_smearFactor;
  Float_t m_VBFjet2_PUjetIDupdated;
  Int_t m_VBFjet2_flav;
  Bool_t m_VBFjet2_hasgenjet;

  Float_t m_VBFgenjet2_pt;
  Float_t m_VBFgenjet2_eta;
  Float_t m_VBFgenjet2_phi;
  Float_t m_VBFgenjet2_e;

  Float_t m_VBFjj_mass ;
  std::vector <Float_t> m_VBFjj_mass_jetup ;
  std::vector <Float_t> m_VBFjj_mass_jetdown ;

  Float_t m_VBFjj_mass_jetup1;
  Float_t m_VBFjj_mass_jetup2;
  Float_t m_VBFjj_mass_jetup3;
  Float_t m_VBFjj_mass_jetup4;
  Float_t m_VBFjj_mass_jetup5;
  Float_t m_VBFjj_mass_jetup6;
  Float_t m_VBFjj_mass_jetup7;
  Float_t m_VBFjj_mass_jetup8;
  Float_t m_VBFjj_mass_jetup9;
  Float_t m_VBFjj_mass_jetup10;
  Float_t m_VBFjj_mass_jetup11;

  Float_t m_VBFjj_mass_jetdown1;
  Float_t m_VBFjj_mass_jetdown2;
  Float_t m_VBFjj_mass_jetdown3;
  Float_t m_VBFjj_mass_jetdown4;
  Float_t m_VBFjj_mass_jetdown5;
  Float_t m_VBFjj_mass_jetdown6;
  Float_t m_VBFjj_mass_jetdown7;
  Float_t m_VBFjj_mass_jetdown8;
  Float_t m_VBFjj_mass_jetdown9;
  Float_t m_VBFjj_mass_jetdown10;
  Float_t m_VBFjj_mass_jetdown11;

  Float_t m_VBFjj_deltaEta ;
  Float_t m_VBFjj_deltaPhi ;

  Float_t m_VBFjj_HT ;

  Float_t m_VBFjet1_pt_jetupTot;
  Float_t m_VBFjet1_pt_jetdownTot;
  Float_t m_VBFjet2_pt_jetupTot;
  Float_t m_VBFjet2_pt_jetdownTot;
  Float_t m_VBFjet1_mass_jetupTot;
  Float_t m_VBFjet1_mass_jetdownTot;
  Float_t m_VBFjet2_mass_jetupTot;
  Float_t m_VBFjet2_mass_jetdownTot;
  Float_t m_VBFjj_mass_jetupTot;
  Float_t m_VBFjj_mass_jetdownTot;

  // additional central jets
  Float_t m_addJetCentr1_pt;
  Float_t m_addJetCentr2_pt;
  Float_t m_addJetCentr3_pt;
  Float_t m_addJetCentr4_pt;
  Float_t m_addJetCentr5_pt;
  Float_t m_addJetCentr1_eta;
  Float_t m_addJetCentr2_eta;
  Float_t m_addJetCentr3_eta;
  Float_t m_addJetCentr4_eta;
  Float_t m_addJetCentr5_eta;
  Float_t m_addJetCentr1_phi;
  Float_t m_addJetCentr2_phi;
  Float_t m_addJetCentr3_phi;
  Float_t m_addJetCentr4_phi;
  Float_t m_addJetCentr5_phi;
  Float_t m_addJetCentr1_e;
  Float_t m_addJetCentr2_e;
  Float_t m_addJetCentr3_e;
  Float_t m_addJetCentr4_e;
  Float_t m_addJetCentr5_e;
  Float_t m_addJetCentr1_btag_deepFlavor;
  Float_t m_addJetCentr2_btag_deepFlavor;
  Float_t m_addJetCentr3_btag_deepFlavor;
  Float_t m_addJetCentr4_btag_deepFlavor;
  Float_t m_addJetCentr5_btag_deepFlavor;
  Float_t m_addJetCentr1_CvsL;
  Float_t m_addJetCentr2_CvsL;
  Float_t m_addJetCentr3_CvsL;
  Float_t m_addJetCentr4_CvsL;
  Float_t m_addJetCentr5_CvsL;
  Float_t m_addJetCentr1_CvsB;
  Float_t m_addJetCentr2_CvsB;
  Float_t m_addJetCentr3_CvsB;
  Float_t m_addJetCentr4_CvsB;
  Float_t m_addJetCentr5_CvsB;
  Float_t m_addJetCentr1_HHbtag;
  Float_t m_addJetCentr2_HHbtag;
  Float_t m_addJetCentr3_HHbtag;
  Float_t m_addJetCentr4_HHbtag;
  Float_t m_addJetCentr5_HHbtag;
  Float_t m_addJetCentr1_smearFactor;
  Float_t m_addJetCentr2_smearFactor;
  Float_t m_addJetCentr3_smearFactor;
  Float_t m_addJetCentr4_smearFactor;
  Float_t m_addJetCentr5_smearFactor;
  Float_t m_addJetCentr1_pnet_bb;
  Float_t m_addJetCentr1_pnet_cc;
  Float_t m_addJetCentr1_pnet_b;
  Float_t m_addJetCentr1_pnet_c;
  Float_t m_addJetCentr1_pnet_g;
  Float_t m_addJetCentr1_pnet_uds;
  Float_t m_addJetCentr1_pnet_pu;
  Float_t m_addJetCentr1_pnet_undef;
  Float_t m_addJetCentr2_pnet_bb;
  Float_t m_addJetCentr2_pnet_cc;
  Float_t m_addJetCentr2_pnet_b;
  Float_t m_addJetCentr2_pnet_c;
  Float_t m_addJetCentr2_pnet_g;
  Float_t m_addJetCentr2_pnet_uds;
  Float_t m_addJetCentr2_pnet_pu;
  Float_t m_addJetCentr2_pnet_undef;
  Float_t m_addJetCentr3_pnet_bb;
  Float_t m_addJetCentr3_pnet_cc;
  Float_t m_addJetCentr3_pnet_b;
  Float_t m_addJetCentr3_pnet_c;
  Float_t m_addJetCentr3_pnet_g;
  Float_t m_addJetCentr3_pnet_uds;
  Float_t m_addJetCentr3_pnet_pu;
  Float_t m_addJetCentr3_pnet_undef;
  Float_t m_addJetCentr4_pnet_bb;
  Float_t m_addJetCentr4_pnet_cc;
  Float_t m_addJetCentr4_pnet_b;
  Float_t m_addJetCentr4_pnet_c;
  Float_t m_addJetCentr4_pnet_g;
  Float_t m_addJetCentr4_pnet_uds;
  Float_t m_addJetCentr4_pnet_pu;
  Float_t m_addJetCentr4_pnet_undef;
  Float_t m_addJetCentr5_pnet_bb;
  Float_t m_addJetCentr5_pnet_cc;
  Float_t m_addJetCentr5_pnet_b;
  Float_t m_addJetCentr5_pnet_c;
  Float_t m_addJetCentr5_pnet_g;
  Float_t m_addJetCentr5_pnet_uds;
  Float_t m_addJetCentr5_pnet_pu;
  Float_t m_addJetCentr5_pnet_undef;

  std::vector<Float_t> m_addJetCentr1_pt_jetup;
  std::vector<Float_t> m_addJetCentr1_pt_jetdown;
  std::vector<Float_t> m_addJetCentr1_mass_jetup;
  std::vector<Float_t> m_addJetCentr1_mass_jetdown;
  std::vector<Float_t> m_addJetCentr2_pt_jetup;
  std::vector<Float_t> m_addJetCentr2_pt_jetdown;
  std::vector<Float_t> m_addJetCentr2_mass_jetup;
  std::vector<Float_t> m_addJetCentr2_mass_jetdown;
  std::vector<Float_t> m_addJetCentr3_pt_jetup;
  std::vector<Float_t> m_addJetCentr3_pt_jetdown;
  std::vector<Float_t> m_addJetCentr3_mass_jetup;
  std::vector<Float_t> m_addJetCentr3_mass_jetdown;
  std::vector<Float_t> m_addJetCentr4_pt_jetup;
  std::vector<Float_t> m_addJetCentr4_pt_jetdown;
  std::vector<Float_t> m_addJetCentr4_mass_jetup;
  std::vector<Float_t> m_addJetCentr4_mass_jetdown;
  std::vector<Float_t> m_addJetCentr5_pt_jetup;
  std::vector<Float_t> m_addJetCentr5_pt_jetdown;
  std::vector<Float_t> m_addJetCentr5_mass_jetup;
  std::vector<Float_t> m_addJetCentr5_mass_jetdown;

  Float_t m_addJetCentr1_pt_jetupTot;
  Float_t m_addJetCentr1_pt_jetdownTot;
  Float_t m_addJetCentr1_mass_jetupTot;
  Float_t m_addJetCentr1_mass_jetdownTot;
  Float_t m_addJetCentr2_pt_jetupTot;
  Float_t m_addJetCentr2_pt_jetdownTot;
  Float_t m_addJetCentr2_mass_jetupTot;
  Float_t m_addJetCentr2_mass_jetdownTot;
  Float_t m_addJetCentr3_pt_jetupTot;
  Float_t m_addJetCentr3_pt_jetdownTot;
  Float_t m_addJetCentr3_mass_jetupTot;
  Float_t m_addJetCentr3_mass_jetdownTot;
  Float_t m_addJetCentr4_pt_jetupTot;
  Float_t m_addJetCentr4_pt_jetdownTot;
  Float_t m_addJetCentr4_mass_jetupTot;
  Float_t m_addJetCentr4_mass_jetdownTot;
  Float_t m_addJetCentr5_pt_jetupTot;
  Float_t m_addJetCentr5_pt_jetdownTot;
  Float_t m_addJetCentr5_mass_jetupTot;
  Float_t m_addJetCentr5_mass_jetdownTot;

  // Additional forward jets
  Float_t m_addJetForw1_pt;
  Float_t m_addJetForw2_pt;
  Float_t m_addJetForw3_pt;
  Float_t m_addJetForw4_pt;
  Float_t m_addJetForw5_pt;
  Float_t m_addJetForw1_eta;
  Float_t m_addJetForw2_eta;
  Float_t m_addJetForw3_eta;
  Float_t m_addJetForw4_eta;
  Float_t m_addJetForw5_eta;
  Float_t m_addJetForw1_phi;
  Float_t m_addJetForw2_phi;
  Float_t m_addJetForw3_phi;
  Float_t m_addJetForw4_phi;
  Float_t m_addJetForw5_phi;
  Float_t m_addJetForw1_e;
  Float_t m_addJetForw2_e;
  Float_t m_addJetForw3_e;
  Float_t m_addJetForw4_e;
  Float_t m_addJetForw5_e;
  Float_t m_addJetForw1_smearFactor;
  Float_t m_addJetForw2_smearFactor;
  Float_t m_addJetForw3_smearFactor;
  Float_t m_addJetForw4_smearFactor;
  Float_t m_addJetForw5_smearFactor;

  std::vector<Float_t> m_addJetForw1_pt_jetup;
  std::vector<Float_t> m_addJetForw1_pt_jetdown;
  std::vector<Float_t> m_addJetForw1_mass_jetup;
  std::vector<Float_t> m_addJetForw1_mass_jetdown;
  std::vector<Float_t> m_addJetForw2_pt_jetup;
  std::vector<Float_t> m_addJetForw2_pt_jetdown;
  std::vector<Float_t> m_addJetForw2_mass_jetup;
  std::vector<Float_t> m_addJetForw2_mass_jetdown;
  std::vector<Float_t> m_addJetForw3_pt_jetup;
  std::vector<Float_t> m_addJetForw3_pt_jetdown;
  std::vector<Float_t> m_addJetForw3_mass_jetup;
  std::vector<Float_t> m_addJetForw3_mass_jetdown;
  std::vector<Float_t> m_addJetForw4_pt_jetup;
  std::vector<Float_t> m_addJetForw4_pt_jetdown;
  std::vector<Float_t> m_addJetForw4_mass_jetup;
  std::vector<Float_t> m_addJetForw4_mass_jetdown;
  std::vector<Float_t> m_addJetForw5_pt_jetup;
  std::vector<Float_t> m_addJetForw5_pt_jetdown;
  std::vector<Float_t> m_addJetForw5_mass_jetup;
  std::vector<Float_t> m_addJetForw5_mass_jetdown;

  Float_t m_addJetForw1_pt_jetupTot;
  Float_t m_addJetForw1_pt_jetdownTot;
  Float_t m_addJetForw1_mass_jetupTot;
  Float_t m_addJetForw1_mass_jetdownTot;
  Float_t m_addJetForw2_pt_jetupTot;
  Float_t m_addJetForw2_pt_jetdownTot;
  Float_t m_addJetForw2_mass_jetupTot;
  Float_t m_addJetForw2_mass_jetdownTot;
  Float_t m_addJetForw3_pt_jetupTot;
  Float_t m_addJetForw3_pt_jetdownTot;
  Float_t m_addJetForw3_mass_jetupTot;
  Float_t m_addJetForw3_mass_jetdownTot;
  Float_t m_addJetForw4_pt_jetupTot;
  Float_t m_addJetForw4_pt_jetdownTot;
  Float_t m_addJetForw4_mass_jetupTot;
  Float_t m_addJetForw4_mass_jetdownTot;
  Float_t m_addJetForw5_pt_jetupTot;
  Float_t m_addJetForw5_pt_jetdownTot;
  Float_t m_addJetForw5_mass_jetupTot;
  Float_t m_addJetForw5_mass_jetdownTot;

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
  Float_t m_BDT_channel; //
  Float_t m_tauH_MET_pt; //
  Float_t m_dau2_MET_deltaEta; //
  Float_t m_bH_MET_deltaEta; //
  Float_t m_bH_MET_deltaR; //
  Float_t m_bH_tauH_MET_deltaR; //
  Float_t m_ditau_deltaR_per_tauH_MET_pt; //
  Float_t m_p_zeta; //
  Float_t m_p_zeta_visible; //
  Float_t m_mT_tauH_MET; //
  Float_t m_mT_tauH_SVFIT_MET; //
  Float_t m_mT_total; //
  Float_t m_BDT_ditau_deltaPhi; //
  Float_t m_BDT_dib_abs_deltaPhi; //
  Float_t m_BDT_dib_deltaPhi; //
  Float_t m_BDT_dau1MET_deltaPhi; //
  Float_t m_BDT_tauHsvfitMet_abs_deltaPhi; //
  Float_t m_BDT_tauHsvfitMet_deltaPhi; //
  Float_t m_BDT_bHMet_deltaPhi; //
  Float_t m_BDT_HHsvfit_abs_deltaPhi; //
  Float_t m_BDT_HT20; //
  std::vector <Float_t> m_BDT_HT20_jetup; //
  std::vector <Float_t> m_BDT_HT20_jetdown; //
  Float_t m_BDT_HT20_jetupTot; //
  Float_t m_BDT_HT20_jetdownTot; //
  Float_t m_BDT_topPairMasses; //
  Float_t m_BDT_topPairMasses2; //
  Float_t m_BDT_MX; //
  Float_t m_BDT_bH_tauH_MET_InvMass; //
  Float_t m_BDT_bH_tauH_SVFIT_InvMass; //
  Float_t m_BDT_bH_tauH_InvMass; //
  Float_t m_BDT_total_CalcPhi; //
  Float_t m_BDT_ditau_CalcPhi; //
  Float_t m_BDT_dib_CalcPhi; //
  Float_t m_BDT_MET_tauH_SVFIT_cosTheta; //
  Float_t m_BDT_MET_bH_cosTheta; //
  Float_t m_BDT_b1_bH_cosTheta; //
  Float_t m_BDT_tauH_SVFIT_reson_cosTheta; //

  // 4 variables for VBF BDT
  Float_t m_VBFjj_dEtaSign   ;
  Float_t m_VBFjet2_PUjetID  ;
  Float_t m_bH_VBF1_deltaEta ;
  Float_t m_dib_dEtaSign     ;

  // fake taus
  Int_t m_isTau1real;
  Int_t m_isTau2real;
  Int_t m_nRealTaus;

} ;

#endif
