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
      m_EventNumber = -1. ;
      m_RunNumber = -1. ;

      m_met_px = -1. ;
      m_met_py = -1. ;
      m_met_et = -1. ;
    
      m_dau1_px  = -1. ;
      m_dau1_py  = -1. ;
      m_dau1_pz  = -1. ;
      m_dau1_e  = -1. ;
      m_dau1_flav  = -1. ;
      
      m_dau2_px  = -1. ;
      m_dau2_py  = -1. ;
      m_dau2_pz  = -1. ;
      m_dau2_e  = -1. ;
      m_dau2_flav  = -1. ;

      m_bjet1_px  = -1. ;
      m_bjet1_py  = -1. ;
      m_bjet1_pz  = -1. ;
      m_bjet1_e  = -1. ;
      m_bjet1_bID = -1. ;
      
      m_bjet2_px  = -1. ;
      m_bjet2_py  = -1. ;
      m_bjet2_pz  = -1. ;
      m_bjet2_e  = -1. ;
      m_bjet2_bID = -1. ;

      m_tauH_px = -1. ;
      m_tauH_py = -1. ;
      m_tauH_pz = -1. ;
      m_tauH_e = -1. ;
      m_tauH_mass = -1. ;
    
      m_bH_px = -1. ;
      m_bH_py = -1. ;
      m_bH_pz = -1. ;
      m_bH_e = -1. ;
      
      m_HH_px = -1. ;
      m_HH_py = -1. ;
      m_HH_pz = -1. ;
      m_HH_e = -1. ;
      m_HH_mass = -1. ;

      m_jets_px.clear () ;
      m_jets_py.clear () ;
      m_jets_pz.clear () ;
      m_jets_e.clear () ;
      m_jets_btag.clear () ;
      m_jets_isH.clear () ;
      m_njets = 0 ;
      
      m_leps_px.clear () ;
      m_leps_py.clear () ;
      m_leps_pz.clear () ;
      m_leps_e.clear () ;
      m_leps_flav.clear () ;
      m_nleps = 0 ;
      
      return 0 ;    
    }

  int init () 
    {      
      m_smallT->Branch ("MC_weight", &m_MC_weight, "MC_weight/F") ;
      m_smallT->Branch ("PUReweight", &m_PUReweight, "PUReweight/F") ;
      m_smallT->Branch ("EventNumber", &m_EventNumber, "EventNumber/F") ;
      m_smallT->Branch ("RunNumber", &m_RunNumber, "RunNumber/F") ;

      m_smallT->Branch ("met_px", &m_met_px, "met_px/F") ;
      m_smallT->Branch ("met_py", &m_met_py, "met_py/F") ;
      m_smallT->Branch ("met_et", &m_met_et, "met_et/F") ;

      m_smallT->Branch ("dau1_px", &m_dau1_px, "dau1_px/F") ;
      m_smallT->Branch ("dau1_py", &m_dau1_py, "dau1_py/F") ;
      m_smallT->Branch ("dau1_pz", &m_dau1_pz, "dau1_pz/F") ;
      m_smallT->Branch ("dau1_e", &m_dau1_e, "dau1_e/F") ;
      m_smallT->Branch ("dau1_flav", &m_dau1_flav, "dau1_flav/F") ;

      m_smallT->Branch ("dau2_px", &m_dau2_px, "dau2_px/F") ;
      m_smallT->Branch ("dau2_py", &m_dau2_py, "dau2_py/F") ;
      m_smallT->Branch ("dau2_pz", &m_dau2_pz, "dau2_pz/F") ;
      m_smallT->Branch ("dau2_e", &m_dau2_e, "dau2_e/F") ;
      m_smallT->Branch ("dau2_flav", &m_dau2_flav, "dau2_flav/F") ;

      m_smallT->Branch ("bjet1_px", &m_bjet1_px, "bjet1_px/F") ;
      m_smallT->Branch ("bjet1_py", &m_bjet1_py, "bjet1_py/F") ;
      m_smallT->Branch ("bjet1_pz", &m_bjet1_pz, "bjet1_pz/F") ;
      m_smallT->Branch ("bjet1_e", &m_bjet1_e, "bjet1_e/F") ;
      m_smallT->Branch ("bjet1_bID", &m_bjet1_bID, "bjet1_bID/F") ;

      m_smallT->Branch ("bjet2_px", &m_bjet2_px, "bjet2_px/F") ;
      m_smallT->Branch ("bjet2_py", &m_bjet2_py, "bjet2_py/F") ;
      m_smallT->Branch ("bjet2_pz", &m_bjet2_pz, "bjet2_pz/F") ;
      m_smallT->Branch ("bjet2_e", &m_bjet2_e, "bjet2_e/F") ;
      m_smallT->Branch ("bjet2_bID", &m_bjet2_bID, "bjet2m_bID/F") ;

      m_smallT->Branch ("tauH_px", &m_tauH_px, "tauH_px/F") ;
      m_smallT->Branch ("tauH_py", &m_tauH_py, "tauH_py/F") ;
      m_smallT->Branch ("tauH_pz", &m_tauH_pz, "tauH_pz/F") ;
      m_smallT->Branch ("tauH_e", &m_tauH_e, "tauH_e/F") ;
      m_smallT->Branch ("tauH_mass", &m_tauH_mass, "tauH_mass/F") ;
    
      m_smallT->Branch ("bH_px", &m_bH_px, "bH_px/F") ;
      m_smallT->Branch ("bH_py", &m_bH_py, "bH_py/F") ;
      m_smallT->Branch ("bH_pz", &m_bH_pz, "bH_pz/F") ;
      m_smallT->Branch ("bH_e", &m_bH_e, "bH_e/F") ;
      
      m_smallT->Branch ("HH_px", &m_HH_px, "HH_px/F") ;
      m_smallT->Branch ("HH_py", &m_HH_py, "HH_py/F") ;
      m_smallT->Branch ("HH_pz", &m_HH_pz, "HH_pz/F") ;
      m_smallT->Branch ("HH_e", &m_HH_e, "HH_e/F") ;
      m_smallT->Branch ("HH_mass", &m_HH_mass, "HH_mass/F") ;
      
      m_smallT->Branch ("jets_px", &m_jets_px) ;
      m_smallT->Branch ("jets_py", &m_jets_py) ;
      m_smallT->Branch ("jets_pz", &m_jets_pz) ;
      m_smallT->Branch ("jets_e", &m_jets_e) ;
      m_smallT->Branch ("jets_btag", &m_jets_btag) ;
      m_smallT->Branch ("jets_isH", &m_jets_isH) ;
      m_smallT->Branch ("njets", &m_njets, "njets/I") ;
      
      m_smallT->Branch ("leps_px", &m_leps_px) ;
      m_smallT->Branch ("leps_py", &m_leps_py) ;
      m_smallT->Branch ("leps_pz", &m_leps_pz) ;
      m_smallT->Branch ("leps_e", &m_leps_e) ;
      m_smallT->Branch ("leps_flav", &m_leps_flav) ;
      m_smallT->Branch ("nleps", &m_nleps, "nleps/I") ;

      return 0 ;
    }

  // the tree itself
  TTree * m_smallT ;
  
  // general variables
  Float_t m_MC_weight ;
  Float_t m_PUReweight ;
  Int_t m_EventNumber ;
  Int_t m_RunNumber ;

  // MET
  Float_t m_met_px ;
  Float_t m_met_py ;
  Float_t m_met_et ;

  // the largest pT daughter visible candidate
  Float_t m_dau1_px ;
  Float_t m_dau1_py ;
  Float_t m_dau1_pz ;
  Float_t m_dau1_e ;
  Float_t m_dau1_flav ; // let this contain also whether it's a hadronic dau

  // the smallest pT daughter visible candidate
  Float_t m_dau2_px ;
  Float_t m_dau2_py ;
  Float_t m_dau2_pz ;
  Float_t m_dau2_e ;
  Float_t m_dau2_flav ; // let this contain also whether it's a hadronic dau

  // the largest pT b visible candidate
  Float_t m_bjet1_px ;
  Float_t m_bjet1_py ;
  Float_t m_bjet1_pz ;
  Float_t m_bjet1_e ;
  Float_t m_bjet1_bID ;

  // the smallest pT b visible candidate
  Float_t m_bjet2_px ;
  Float_t m_bjet2_py ;
  Float_t m_bjet2_pz ;
  Float_t m_bjet2_e ;
  Float_t m_bjet2_bID ;

  // the tautau-H candidate
  Float_t m_tauH_px ;
  Float_t m_tauH_py ;
  Float_t m_tauH_pz ;
  Float_t m_tauH_e ;
  Float_t m_tauH_mass ;

  // the bb-H candidate
  Float_t m_bH_px ;
  Float_t m_bH_py ;
  Float_t m_bH_pz ;
  Float_t m_bH_e ;
  
  // the di-higgs candidate
  Float_t m_HH_px ;
  Float_t m_HH_py ;
  Float_t m_HH_pz ;
  Float_t m_HH_e ;
  Float_t m_HH_mass ;

  // additional jets
  std::vector<Float_t> m_jets_px ;
  std::vector<Float_t> m_jets_py ;
  std::vector<Float_t> m_jets_pz ;
  std::vector<Float_t> m_jets_e ;
  std::vector<Int_t> m_jets_btag ;
  std::vector<Int_t> m_jets_isH ;
  Int_t m_njets ;
  
  // additional leptons
  std::vector<Float_t> m_leps_px ;
  std::vector<Float_t> m_leps_py ;
  std::vector<Float_t> m_leps_pz ;
  std::vector<Float_t> m_leps_e ;
  std::vector<Int_t> m_leps_flav ;
  Int_t m_nleps ;
  
} ;

#endif
