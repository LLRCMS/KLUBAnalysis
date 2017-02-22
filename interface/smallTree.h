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
      m_PUReweight = -1. ;
      m_bTagweightL = -1. ;
      m_bTagweightM = -1. ;
      m_bTagweightT = -1. ;
      m_TTtopPtreweight = -1. ;
      m_TTtopPtreweight_up   = -1. ;
      m_TTtopPtreweight_down = -1. ;
      m_turnOnreweight = -1. ;
      m_turnOnreweight_tauup = -1. ;
      m_turnOnreweight_taudown = -1. ;
      m_EventNumber = -1. ;
      m_DYscale_LL = -1. ;
      m_DYscale_MM = -1. ;
      m_trigSF = -1.;
      m_IdAndIsoSF = -1.;
      m_nBhadrons = -1;
      m_lheNOutPartons = -1;
      m_lheNOutB = -1;
      m_RunNumber = -1. ;
      m_isBoosted = -1 ;
      
      m_genDecMode1 = -1;
      m_genDecMode2 = -1;

      m_genMHH   = -1;
      m_genCosth = -999;

      m_npv = -1. ;
      m_npu = -1. ;
      m_lumi = -1. ;
      m_triggerbit = -1. ;
      m_L3filterlast1 = -1;
      m_L3filter1 = -1;
      m_L3filterlast2 = -1;
      m_L3filter2 = -1;
      m_rho = -1. ;

      m_pairType = -1. ;
      m_isMC = -1. ;
      m_isOS = -1. ;
      
      m_met_phi = -1. ;
      m_met_et = -1. ;
      m_met_et_corr =-1.;
      m_met_cov00 = -1.;
      m_met_cov01 = -1.;
      m_met_cov10 = -1.;
      m_met_cov11 = -1.;
      m_mT1 = -1. ;
      m_mT2 = -1. ;
    
      m_dau1_iso  = -1. ;
      m_dau1_MVAiso = -1 ;
      m_dau1_CUTiso = -1 ;
      m_dau1_antiele = -1 ;
      m_dau1_antimu = -1 ;
      m_dau1_photonPtSumOutsideSignalCone = -1;
      m_dau1_byLooseCombinedIsolationDeltaBetaCorr3Hits= false;
      m_dau1_byMediumCombinedIsolationDeltaBetaCorr3Hits= false;
      m_dau1_byTightCombinedIsolationDeltaBetaCorr3Hits= false;
      m_dau1_pt  = -1. ;
      m_dau1_pt_tauup  = -1. ;
      m_dau1_pt_taudown  = -1. ;
      m_dau1_eta  = -1. ;
      m_dau1_phi  = -1. ;
      m_dau1_e  = -1. ;
      m_dau1_flav  = -1. ;

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
      m_dau2_MVAiso = -1 ;
      m_dau2_CUTiso = -1 ;
      m_dau2_antiele = -1 ;
      m_dau2_antimu = -1 ;
      m_dau2_photonPtSumOutsideSignalCone = -1;
      m_dau2_byLooseCombinedIsolationDeltaBetaCorr3Hits= false;
      m_dau2_byMediumCombinedIsolationDeltaBetaCorr3Hits= false;
      m_dau2_byTightCombinedIsolationDeltaBetaCorr3Hits= false;
      m_dau2_pt  = -1. ;
      m_dau2_pt_tauup  = -1. ;
      m_dau2_pt_taudown  = -1. ;
      m_dau2_eta  = -1. ;
      m_dau2_phi  = -1. ;
      m_dau2_e  = -1. ;
      m_dau2_flav  = -1. ;

      m_bjet1_pt  = -1. ;
      m_bjet1_eta  = -1. ;
      m_bjet1_phi  = -1. ;
      m_bjet1_e  = -1. ;
      m_bjet1_bID = -1. ;
      m_bjet1_flav = -1 ;
      m_bjet1_pt_raw  = -1. ;
      m_bjet1_pt_raw_jetup  = -1. ;
      m_bjet1_pt_raw_jetdown  = -1. ;
      m_bjet1_hasgenjet = false ;
      
      m_bjet2_pt  = -1. ;
      m_bjet2_eta  = -1. ;
      m_bjet2_phi  = -1. ;
      m_bjet2_e  = -1. ;
      m_bjet2_bID = -1. ;
      m_bjet2_flav = -1 ;
      m_bjet2_pt_raw  = -1. ;
      m_bjet2_pt_raw_jetup  = -1. ;
      m_bjet2_pt_raw_jetdown  = -1. ;
      m_bjet2_hasgenjet = false ;

      m_nfatjets = -1;
      m_fatjet_pt = -1. ;
      m_fatjet_eta  = -1. ;
      m_fatjet_phi  = -1. ;
      m_fatjet_e  = -1. ;
      m_fatjet_bID = -1. ;
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

      m_subjetjet2_pt = -1. ;
      m_subjetjet2_eta  = -1. ;
      m_subjetjet2_phi  = -1. ;
      m_subjetjet2_e  = -1. ;
      m_subjetjet2_bID = -1. ;


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
      m_HH_mass_raw_tauup   = -1. ;
      m_HH_mass_raw_taudown = -1. ;
      m_HHKin_mass = -1. ;
      m_HHKin_chi2 = -1. ;

      m_HH_deltaPhi = -1. ;
      m_HHsvfit_deltaPhi = -1. ;
      m_tauHMet_deltaPhi = -1. ;
      m_tauHsvfitMet_deltaPhi = -1. ;
      m_bHMet_deltaPhi = -1. ;
      m_ditau_deltaPhi = -1. ;
      m_dib_deltaPhi = -1. ;
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
      m_MT2_tauup = -1.;
      m_MT2_taudown = -1.;
      m_MT2_jetup = -1.;
      m_MT2_jetdown = -1.;
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
      m_jets_flav.clear () ;
      m_jets_isH.clear () ;
      m_jets_hasgenjet.clear () ;
      m_njets = 0 ;
      m_njets20 = 0 ;
      m_njets50 = 0 ;
      m_nbjetscand = 0 ;
      m_njetsBHadFlav = 0 ;
      m_njetsCHadFlav = 0 ;
      m_dau1_jecUnc= -1;
      m_dau2_jecUnc= -1;
      m_bjet1_jecUnc = -1;
      m_bjet2_jecUnc = -1;
      m_jets_jecUnc.clear() ;

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
      m_bH_mass_raw_jetup = 0;
      m_bH_mass_raw_jetdown = 0;
      
      m_HHKin_mass_raw = 0;
      m_HHKin_mass_raw_tauup = 0;
      m_HHKin_mass_raw_taudown = 0;
      m_HHKin_mass_raw_jetup = 0;
      m_HHKin_mass_raw_jetdown = 0;
      m_HHKin_mass_raw_chi2 = 0;
      m_HHKin_mass_raw_convergence = 0;
      m_HHKin_mass_raw_prob = 0;
      m_HHKin_mass_raw_copy = 0;

      m_lheht = 0;

      m_topReweight=-1;

      return 0 ;    
    }

  int init () 
    {      
      m_smallT->Branch ("MC_weight", &m_MC_weight, "MC_weight/F") ;
      m_smallT->Branch ("PUReweight", &m_PUReweight, "PUReweight/F") ;
      m_smallT->Branch ("bTagweightL", &m_bTagweightL, "bTagweightL/F") ;
      m_smallT->Branch ("bTagweightM", &m_bTagweightM, "bTagweightM/F") ;
      m_smallT->Branch ("bTagweightT", &m_bTagweightT, "bTagweightT/F") ;
      m_smallT->Branch ("TTtopPtreweight", &m_TTtopPtreweight, "TTtopPtreweight/F") ;
      m_smallT->Branch ("TTtopPtreweight_up", &m_TTtopPtreweight_up, "TTtopPtreweight_up/F") ;
      m_smallT->Branch ("TTtopPtreweight_down", &m_TTtopPtreweight_down, "TTtopPtreweight_down/F") ;
      m_smallT->Branch ("turnOnreweight", &m_turnOnreweight, "turnOnreweight/F") ;
      m_smallT->Branch ("turnOnreweight_tauup", &m_turnOnreweight_tauup, "turnOnreweight_tauup/F") ;
      m_smallT->Branch ("turnOnreweight_taudown", &m_turnOnreweight_taudown, "turnOnreweight_taudown/F") ;
      m_smallT->Branch ("trigSF", &m_trigSF, "trigSF/F") ;
      m_smallT->Branch ("IdAndIsoSF", &m_IdAndIsoSF, "IdAndIsoSF/F") ;
      m_smallT->Branch ("DYscale_LL", &m_DYscale_LL, "DYscale_LL/F") ;
      m_smallT->Branch ("DYscale_MM", &m_DYscale_MM, "DYscale_MM/F") ;
      m_smallT->Branch ("nBhadrons", &m_nBhadrons, "nBhadrons/I") ;
      m_smallT->Branch ("lheNOutPartons", &m_lheNOutPartons, "lheNOutPartons/I");
      m_smallT->Branch ("lheNOutB", &m_lheNOutB, "lheNOutB/I");
      m_smallT->Branch ("EventNumber", &m_EventNumber, "EventNumber/I") ;
      m_smallT->Branch ("RunNumber", &m_RunNumber, "RunNumber/I") ;
      m_smallT->Branch ("isBoosted", &m_isBoosted, "isBoosted/I") ;

      m_smallT->Branch("genDecMode1", &m_genDecMode1, "genDecMode1/I");
      m_smallT->Branch("genDecMode2", &m_genDecMode2, "genDecMode2/I");

      m_smallT->Branch("genMHH", &m_genMHH, "m_genMHH/F");
      m_smallT->Branch("genCosth", &m_genCosth, "m_genCosth/F");

      m_smallT->Branch ("npv", &m_npv, "npv/I") ;
      m_smallT->Branch ("npu", &m_npu, "npu/F") ;
      m_smallT->Branch ("lumi", &m_lumi, "lumi/I") ;
      m_smallT->Branch ("triggerbit", &m_triggerbit, "triggerbit/L") ;
      m_smallT->Branch ("L3filter1", &m_L3filter1, "L3filter1/I") ;
      m_smallT->Branch ("L3filterlast1", &m_L3filterlast1, "L3filterlast1/I") ;
      m_smallT->Branch ("L3filter2", &m_L3filter2, "L3filter2/I") ;
      m_smallT->Branch ("L3filterlast2", &m_L3filterlast2, "L3filterlast2/I") ;
      m_smallT->Branch ("rho", &m_rho, "rho/F") ;

      m_smallT->Branch ("pairType", &m_pairType, "pairType/I") ;
      m_smallT->Branch ("isMC", &m_isMC, "isMC/I") ;
      m_smallT->Branch ("isOS", &m_isOS, "isOS/I") ;

      m_smallT->Branch ("met_phi", &m_met_phi, "met_phi/F") ;
      m_smallT->Branch ("met_et", &m_met_et, "met_et/F") ;
      m_smallT->Branch ("met_et_corr", &m_met_et_corr, "met_et_corr/F") ;
      m_smallT->Branch ("met_cov00", &m_met_cov00, "met_cov00/F") ;
      m_smallT->Branch ("met_cov01", &m_met_cov01, "met_cov01/F") ;
      m_smallT->Branch ("met_cov10", &m_met_cov10, "met_cov10/F") ;
      m_smallT->Branch ("met_cov11", &m_met_cov11, "met_cov11/F") ;
      m_smallT->Branch ("mT1", &m_mT1, "mT1/F") ;
      m_smallT->Branch ("mT2", &m_mT2, "mT2/F") ;

      m_smallT->Branch ("dau1_iso", &m_dau1_iso, "dau1_iso/F") ;
      m_smallT->Branch ("dau1_MVAiso", &m_dau1_MVAiso, "dau1_MVAiso/I") ;
      m_smallT->Branch ("dau1_CUTiso", &m_dau1_CUTiso, "dau1_CUTiso/I") ;
      m_smallT->Branch ("dau1_antiele", &m_dau1_antiele, "dau1_antiele/I") ;
      m_smallT->Branch ("dau1_antimu", &m_dau1_antimu, "dau1_antimu/I") ;
      m_smallT->Branch ("dau1_photonPtSumOutsideSignalCone", &m_dau1_photonPtSumOutsideSignalCone,"dau1_photonPtSumOutsideSignalCone/F");
      m_smallT->Branch ("dau1_byLooseCombinedIsolationDeltaBetaCorr3Hits", &m_dau1_byLooseCombinedIsolationDeltaBetaCorr3Hits, "dau1_byLooseCombinedIsolationDeltaBetaCorr3Hits/O") ;
      m_smallT->Branch ("dau1_byMediumCombinedIsolationDeltaBetaCorr3Hits", &m_dau1_byMediumCombinedIsolationDeltaBetaCorr3Hits, "dau1_byMediumCombinedIsolationDeltaBetaCorr3Hits/O") ;
      m_smallT->Branch ("dau1_byTightCombinedIsolationDeltaBetaCorr3Hits", &m_dau1_byTightCombinedIsolationDeltaBetaCorr3Hits, "dau1_byTightCombinedIsolationDeltaBetaCorr3Hits/O") ;
      m_smallT->Branch ("dau1_pt", &m_dau1_pt, "dau1_pt/F") ;
      m_smallT->Branch ("dau1_pt_tauup", &m_dau1_pt_tauup, "dau1_pt_tauup/F") ;
      m_smallT->Branch ("dau1_pt_taudown", &m_dau1_pt_taudown, "dau1_pt_taudown/F") ;
      m_smallT->Branch ("dau1_eta", &m_dau1_eta, "dau1_eta/F") ;
      m_smallT->Branch ("dau1_phi", &m_dau1_phi, "dau1_phi/F") ;
      m_smallT->Branch ("dau1_e", &m_dau1_e, "dau1_e/F") ;
      m_smallT->Branch ("dau1_flav", &m_dau1_flav, "dau1_flav/F") ;

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
      m_smallT->Branch ("dau2_MVAiso", &m_dau2_MVAiso, "dau2_MVAiso/I") ;
      m_smallT->Branch ("dau2_CUTiso", &m_dau2_CUTiso, "dau2_CUTiso/I") ;
      m_smallT->Branch ("dau2_antiele", &m_dau2_antiele, "dau2_antiele/I") ;
      m_smallT->Branch ("dau2_antimu", &m_dau2_antimu, "dau2_antimu/I") ;
      m_smallT->Branch ("dau2_photonPtSumOutsideSignalCone", &m_dau2_photonPtSumOutsideSignalCone, "dau2_photonPtSumOutsideSignalCone/F");
      m_smallT->Branch ("dau2_byLooseCombinedIsolationDeltaBetaCorr3Hits", &m_dau2_byLooseCombinedIsolationDeltaBetaCorr3Hits, "dau2_byLooseCombinedIsolationDeltaBetaCorr3Hits/O") ;
      m_smallT->Branch ("dau2_byMediumCombinedIsolationDeltaBetaCorr3Hits", &m_dau2_byMediumCombinedIsolationDeltaBetaCorr3Hits, "dau2_byMediumCombinedIsolationDeltaBetaCorr3Hits/O") ;
      m_smallT->Branch ("dau2_byTightCombinedIsolationDeltaBetaCorr3Hits", &m_dau2_byTightCombinedIsolationDeltaBetaCorr3Hits, "dau2_byTightCombinedIsolationDeltaBetaCorr3Hits/O") ;
      m_smallT->Branch ("dau2_pt", &m_dau2_pt, "dau2_pt/F") ;
      m_smallT->Branch ("dau2_pt_tauup", &m_dau2_pt_tauup, "dau2_pt_tauup/F") ;
      m_smallT->Branch ("dau2_pt_taudown", &m_dau2_pt_taudown, "dau2_pt_taudown/F") ;
      m_smallT->Branch ("dau2_eta", &m_dau2_eta, "dau2_eta/F") ;
      m_smallT->Branch ("dau2_phi", &m_dau2_phi, "dau2_phi/F") ;
      m_smallT->Branch ("dau2_e", &m_dau2_e, "dau2_e/F") ;
      m_smallT->Branch ("dau2_flav", &m_dau2_flav, "dau2_flav/F") ;

      m_smallT->Branch ("bjet1_pt", &m_bjet1_pt, "bjet1_pt/F") ;
      m_smallT->Branch ("bjet1_eta", &m_bjet1_eta, "bjet1_eta/F") ;
      m_smallT->Branch ("bjet1_phi", &m_bjet1_phi, "bjet1_phi/F") ;
      m_smallT->Branch ("bjet1_e", &m_bjet1_e, "bjet1_e/F") ;
      m_smallT->Branch ("bjet1_bID", &m_bjet1_bID, "bjet1_bID/F") ;
      m_smallT->Branch ("bjet1_flav", &m_bjet1_flav, "bjet1_flav/I") ;
      m_smallT->Branch ("bjet1_pt_raw", &m_bjet1_pt_raw, "bjet1_pt_raw/F") ;
      m_smallT->Branch ("bjet1_pt_raw_jetup", &m_bjet1_pt_raw_jetup, "bjet1_pt_raw_jetup/F") ;
      m_smallT->Branch ("bjet1_pt_raw_jetdown", &m_bjet1_pt_raw_jetdown, "bjet1_pt_raw_jetdown/F") ;
      m_smallT->Branch ("bjet1_hasgenjet", &m_bjet1_hasgenjet, "bjet1_hasgenjet/O") ;
     
      m_smallT->Branch ("bjet2_pt", &m_bjet2_pt, "bjet2_pt/F") ;
      m_smallT->Branch ("bjet2_eta", &m_bjet2_eta, "bjet2_eta/F") ;
      m_smallT->Branch ("bjet2_phi", &m_bjet2_phi, "bjet2_phi/F") ;
      m_smallT->Branch ("bjet2_e", &m_bjet2_e, "bjet2_e/F") ;
      m_smallT->Branch ("bjet2_bID", &m_bjet2_bID, "bjet2m_bID/F") ;
      m_smallT->Branch ("bjet2_flav", &m_bjet2_flav, "bjet2_flav/I") ;
      m_smallT->Branch ("bjet2_pt_raw", &m_bjet2_pt_raw, "bjet2_pt_raw/F") ;
      m_smallT->Branch ("bjet2_pt_raw_jetup", &m_bjet2_pt_raw_jetup, "bjet2_pt_raw_jetup/F") ;
      m_smallT->Branch ("bjet2_pt_raw_jetdown", &m_bjet2_pt_raw_jetdown, "bjet2_pt_raw_jetdown/F") ;
      m_smallT->Branch ("bjet2_hasgenjet", &m_bjet2_hasgenjet, "bjet2_hasgenjet/O") ;

      m_smallT->Branch ("nfatjets", &m_nfatjets, "nfatjets/I");
      m_smallT->Branch ("fatjet_pt", &m_fatjet_pt, "fatjet_pt/F");
      m_smallT->Branch ("fatjet_eta", &m_fatjet_eta, "fatjet_eta/F");
      m_smallT->Branch ("fatjet_phi", &m_fatjet_phi, "fatjet_phi/F");
      m_smallT->Branch ("fatjet_e", &m_fatjet_e, "fatjet_e/F");
      m_smallT->Branch ("fatjet_bID", &m_fatjet_bID, "fatjet_bID/F");
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

      m_smallT->Branch ("subjetjet2_pt", &m_subjetjet2_pt, "subjetjet2_pt/F");
      m_smallT->Branch ("subjetjet2_eta", &m_subjetjet2_eta, "subjetjet2_eta/F");
      m_smallT->Branch ("subjetjet2_phi", &m_subjetjet2_phi, "subjetjet2_phi/F");
      m_smallT->Branch ("subjetjet2_e", &m_subjetjet2_e, "subjetjet2_e/F");
      m_smallT->Branch ("subjetjet2_bID", &m_subjetjet2_bID, "subjetjet2_bID/F");

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
      m_smallT->Branch ("HH_mass_raw_tauup", &m_HH_mass_raw_tauup, "HH_mass_raw_tauup/F") ;
      m_smallT->Branch ("HH_mass_raw_taudown", &m_HH_mass_raw_taudown, "HH_mass_raw_taudown/F") ;

      m_smallT->Branch ("HHKin_mass", &m_HHKin_mass, "HHKin_mass/F") ;
      m_smallT->Branch ("HHKin_chi2", &m_HHKin_chi2, "HHKin_chi2/F") ;
      m_smallT->Branch ("HH_deltaPhi", &m_HH_deltaPhi, "HH_deltaPhi/F") ;
      m_smallT->Branch ("HHsvfit_deltaPhi", &m_HHsvfit_deltaPhi, "HHsvfit_deltaPhi/F") ;
      m_smallT->Branch ("tauHMet_deltaPhi", &m_tauHMet_deltaPhi, "tauHMet_deltaPhi/F") ;
      m_smallT->Branch ("tauHsvfitMet_deltaPhi", &m_tauHsvfitMet_deltaPhi, "tauHsvfitMet_deltaPhi/F") ;
      m_smallT->Branch ("bHMet_deltaPhi", &m_bHMet_deltaPhi, "bHMet_deltaPhi/F") ;
//      m_smallT->Branch ("", &m_, "/F") ;
      m_smallT->Branch ("ditau_deltaPhi", &m_ditau_deltaPhi, "ditau_deltaPhi/F") ;
      m_smallT->Branch ("dib_deltaPhi", &m_dib_deltaPhi, "dib_deltaPhi/F") ;
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
      m_smallT->Branch ("jets_flav", &m_jets_flav) ;
      m_smallT->Branch ("jets_isH", &m_jets_isH) ;
      m_smallT->Branch ("jets_hasgenjet", &m_jets_hasgenjet) ;
      m_smallT->Branch ("njets", &m_njets, "njets/I") ;
      m_smallT->Branch ("njets20", &m_njets20, "njets20/I") ;
      m_smallT->Branch ("njets50", &m_njets50, "njets50/I") ;
      m_smallT->Branch ("nbjetscand", &m_nbjetscand, "nbjetscand/I") ;
      m_smallT->Branch ("njetsBHadFlav", &m_njetsBHadFlav, "njetsBHadFlav/I") ;
      m_smallT->Branch ("njetsCHadFlav", &m_njetsCHadFlav, "njetsCHadFlav/I") ;
      m_smallT->Branch ("jets_jecUnc", &m_jets_jecUnc);
      m_smallT->Branch ("dau1_jecUnc", &m_dau1_jecUnc,"dau1_jecUnc/F");
      m_smallT->Branch ("dau2_jecUnc", &m_dau2_jecUnc,"dau2_jecUnc/F");
      m_smallT->Branch ("bjet1_jecUnc", &m_bjet1_jecUnc,"bjet1_jecUnc/F");
      m_smallT->Branch ("bjet2_jecUnc", &m_bjet2_jecUnc,"bjet2_jecUnc/F");


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
      m_smallT->Branch("MT2_tauup",        &m_MT2_tauup,         "MT2_tauup/F");
      m_smallT->Branch("MT2_taudown",      &m_MT2_taudown,       "MT2_taudown/F");
      m_smallT->Branch("MT2_jetup",        &m_MT2_jetup,         "MT2_jetup/F");
      m_smallT->Branch("MT2_jetdown",      &m_MT2_jetdown,       "MT2_jetdown/F");
      // m_smallT->Branch("MT2_covMtrxStatus", &m_MT2_covMtrxStatus, "MT2_covMtrxStatus/I");
      // m_smallT->Branch("MT2_EDM",           &m_MT2_EDM,           "MT2_EDM/F");
      // m_smallT->Branch("MT2_hasConverged",  &m_MT2_hasConverged,  "MT2_hasConverged/I");
      // m_smallT->Branch("MT2_status",        &m_MT2_status,        "MT2_status/I");
      // m_smallT->Branch("MT2_ncalls",        &m_MT2_ncalls,        "MT2_ncalls/I");
      // m_smallT->Branch("MT2_l",               &m_MT2_l,               "MT2_l/F");

      m_smallT->Branch ("bH_mass_raw",&m_bH_mass_raw,"bH_mass_raw/F");
      m_smallT->Branch ("bH_mass_raw_jetup",&m_bH_mass_raw_jetup,"bH_mass_raw_jetup/F");
      m_smallT->Branch ("bH_mass_raw_jetdown",&m_bH_mass_raw_jetdown,"bH_mass_raw_jetdown/F");
      m_smallT->Branch ("HHKin_mass_raw",&m_HHKin_mass_raw ,"HHKin_mass_raw/F");
      m_smallT->Branch ("HHKin_mass_raw_tauup",&m_HHKin_mass_raw_tauup ,"HHKin_mass_raw_tauup/F");
      m_smallT->Branch ("HHKin_mass_raw_taudown",&m_HHKin_mass_raw_taudown ,"HHKin_mass_raw_taudown/F");
      m_smallT->Branch ("HHKin_mass_raw_jetup",&m_HHKin_mass_raw_jetup ,"HHKin_mass_raw_jetup/F");
      m_smallT->Branch ("HHKin_mass_raw_jetdown",&m_HHKin_mass_raw_jetdown ,"HHKin_mass_raw_jetdown/F");
      m_smallT->Branch ("HHKin_mass_raw_chi2", &m_HHKin_mass_raw_chi2, "HHKin_mass_raw_chi2/F") ;
      m_smallT->Branch ("HHKin_mass_raw_convergence", &m_HHKin_mass_raw_convergence, "HHKin_mass_raw_convergence/I") ;
      m_smallT->Branch ("HHKin_mass_raw_prob", &m_HHKin_mass_raw_prob, "HHKin_mass_raw_prob/F") ;

      m_smallT->Branch("HHKin_mass_raw_copy",&m_HHKin_mass_raw_copy ,"HHKin_mass_raw_copy/F");

      //m_smallT->Branch ("MuTauKine", &m_mvaValueMuTau, "MuTauKine/F");
      //m_smallT->Branch ("TauTauKine", &m_mvaValueTauTau, "TauTauKine/F");

      m_smallT->Branch ("lheht", &m_lheht, "lheht/F");
      m_smallT->Branch("topReweight",&m_topReweight,"topReweight/F");

      return 0 ;
    }

  // the tree itself
  TTree * m_smallT ;
  
  // general variables
  Float_t m_MC_weight ;
  Float_t m_PUReweight ;
  Float_t m_bTagweightL ;
  Float_t m_bTagweightM ;
  Float_t m_bTagweightT ;
  Float_t m_TTtopPtreweight ;
  Float_t m_TTtopPtreweight_up ;
  Float_t m_TTtopPtreweight_down ;
  Float_t m_turnOnreweight ;
  Float_t m_turnOnreweight_tauup ;
  Float_t m_turnOnreweight_taudown ;
  Float_t m_trigSF ;
  Float_t m_IdAndIsoSF ;
  Float_t m_DYscale_LL ;
  Float_t m_DYscale_MM ;
  Int_t m_nBhadrons ;
  Int_t m_lheNOutPartons ;
  Int_t m_lheNOutB ;
  Int_t m_EventNumber ;
  Int_t m_RunNumber ;
  Int_t m_isBoosted ;

  Int_t m_genDecMode1 ;
  Int_t m_genDecMode2 ;

  Float_t m_genMHH ;
  Float_t m_genCosth ;

  Int_t   m_npv ;
  Float_t m_npu ;
  Int_t m_lumi ;
  Long64_t m_triggerbit ;
  Int_t m_L3filter1 ;
  Int_t m_L3filterlast1 ;
  Int_t m_L3filter2 ;
  Int_t m_L3filterlast2 ;
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
  Float_t m_met_et_corr;
  Float_t m_met_cov00;
  Float_t m_met_cov01;
  Float_t m_met_cov10;
  Float_t m_met_cov11;
  // mt
  Float_t m_mT1 ;
  Float_t m_mT2 ;

  // the largest pT daughter visible candidate
  Float_t m_dau1_iso ;
  Int_t   m_dau1_MVAiso; // for taus only
  Int_t   m_dau1_CUTiso; // for taus only
  Int_t   m_dau1_antiele; // for taus only
  Int_t   m_dau1_antimu; // for taus only
  Float_t m_dau1_photonPtSumOutsideSignalCone;
  Bool_t m_dau1_byLooseCombinedIsolationDeltaBetaCorr3Hits;
  Bool_t m_dau1_byMediumCombinedIsolationDeltaBetaCorr3Hits;
  Bool_t m_dau1_byTightCombinedIsolationDeltaBetaCorr3Hits;  
  Float_t m_dau1_pt ;
  Float_t m_dau1_pt_tauup ;
  Float_t m_dau1_pt_taudown ;
  Float_t m_dau1_eta ;
  Float_t m_dau1_phi ;
  Float_t m_dau1_e ;
  Float_t m_dau1_flav ; // let this contain also whether it's a hadronic dau

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
  Int_t   m_dau2_MVAiso; // for taus only
  Int_t   m_dau2_CUTiso; // for taus only
  Int_t   m_dau2_antiele; // for taus only
  Int_t   m_dau2_antimu; // for taus only
  Float_t m_dau2_photonPtSumOutsideSignalCone;
  Bool_t m_dau2_byLooseCombinedIsolationDeltaBetaCorr3Hits;
  Bool_t m_dau2_byMediumCombinedIsolationDeltaBetaCorr3Hits;
  Bool_t m_dau2_byTightCombinedIsolationDeltaBetaCorr3Hits;  
  Float_t m_dau2_pt ;
  Float_t m_dau2_pt_tauup ;
  Float_t m_dau2_pt_taudown ;
  Float_t m_dau2_eta ;
  Float_t m_dau2_phi ;
  Float_t m_dau2_e ;
  Float_t m_dau2_flav ; // let this contain also whether it's a hadronic dau

  // the largest pT b visible candidate /  highest CSV score
  Float_t m_bjet1_pt ;
  Float_t m_bjet1_eta ;
  Float_t m_bjet1_phi ;
  Float_t m_bjet1_e ;
  Float_t m_bjet1_bID ;
  Int_t   m_bjet1_flav ;
  Float_t m_bjet1_pt_raw ;
  Float_t m_bjet1_pt_raw_jetup ;
  Float_t m_bjet1_pt_raw_jetdown ;
  Bool_t  m_bjet1_hasgenjet ;

  // the trailing pT b visible candidate  /  highest CSV score
  Float_t m_bjet2_pt ;
  Float_t m_bjet2_eta ;
  Float_t m_bjet2_phi ;
  Float_t m_bjet2_e ;
  Float_t m_bjet2_bID ;
  Int_t   m_bjet2_flav ;
  Float_t m_bjet2_pt_raw ;
  Float_t m_bjet2_pt_raw_jetup ;
  Float_t m_bjet2_pt_raw_jetdown ;
  Bool_t  m_bjet2_hasgenjet ;

  // fat jets and subjet info
  Int_t   m_nfatjets ;
  Float_t m_fatjet_pt ;
  Float_t m_fatjet_eta  ;
  Float_t m_fatjet_phi  ;
  Float_t m_fatjet_e  ;
  Float_t m_fatjet_bID ;
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

  Float_t m_subjetjet2_pt ;
  Float_t m_subjetjet2_eta  ;
  Float_t m_subjetjet2_phi  ;
  Float_t m_subjetjet2_e  ;
  Float_t m_subjetjet2_bID ;

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
  Float_t m_HH_mass_raw_tauup ;
  Float_t m_HH_mass_raw_taudown ;

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
  Float_t m_HHsvfit_deltaPhi ;
  Float_t m_tauHMet_deltaPhi ;
  Float_t m_tauHsvfitMet_deltaPhi ;
  Float_t m_bHMet_deltaPhi ;
  Float_t m_ditau_deltaPhi ;
  Float_t m_dib_deltaPhi ;
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
  std::vector<Int_t> m_jets_flav ;
  std::vector<Int_t> m_jets_isH ;
  std::vector<Bool_t> m_jets_hasgenjet ;
  Int_t m_njets ;
  Int_t m_njets20 ;
  Int_t m_njets50 ;
  Int_t m_nbjetscand ;
  Int_t m_njetsBHadFlav ;
  Int_t m_njetsCHadFlav ;

  std::vector<Float_t> m_jets_jecUnc ;
  Float_t m_dau1_jecUnc;
  Float_t m_dau2_jecUnc;
  Float_t m_bjet1_jecUnc;
  Float_t m_bjet2_jecUnc;

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
  Float_t m_HHKin_mass_raw_tauup;
  Float_t m_HHKin_mass_raw_taudown;
  Float_t m_HHKin_mass_raw_jetup;
  Float_t m_HHKin_mass_raw_jetdown;
  Float_t m_HHKin_mass_raw_chi2;
  Int_t   m_HHKin_mass_raw_convergence;
  Float_t m_HHKin_mass_raw_prob;

  Float_t m_HHKin_mass_raw_copy;
  Float_t m_bH_mass_raw;
  Float_t m_bH_mass_raw_jetup;
  Float_t m_bH_mass_raw_jetdown;

  Float_t m_HHkinsvfit_bHmass;
  Float_t m_HHkinsvfit_pt;
  Float_t m_HHkinsvfit_eta ;
  Float_t m_HHkinsvfit_phi ;
  Float_t m_HHkinsvfit_e;
  Float_t m_HHkinsvfit_m;

  Float_t m_MT2;
  Float_t m_MT2_tauup;
  Float_t m_MT2_taudown;
  Float_t m_MT2_jetup;
  Float_t m_MT2_jetdown;
  // Int_t   m_MT2_covMtrxStatus;
  // Float_t m_MT2_EDM;
  // Int_t   m_MT2_hasConverged;
  // Int_t   m_MT2_status;
  // Int_t   m_MT2_ncalls;
  // Float_t m_MT2_l;

  Float_t m_lheht ;
  Float_t m_topReweight;
  
} ;

#endif
