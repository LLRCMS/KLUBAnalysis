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
      m_leps_isH.clear () ;
      m_nleps = 0 ;
      
      m_met_px.clear () ;
      m_met_py.clear () ;
      m_met_et.clear () ;
    
      m_tauH_px = -1. ;
      m_tauH_py = -1. ;
      m_tauH_pz = -1. ;
      m_tauH_e = -1. ;
    
      m_bH_px = -1. ;
      m_bH_py = -1. ;
      m_bH_pz = -1. ;
      m_bH_e = -1. ;
      
      m_HH_px = -1. ;
      m_HH_py = -1. ;
      m_HH_pz = -1. ;
      m_HH_e = -1. ;

      return 0 ;    
    }

  int init () 
    {
      m_smallT->Branch ("jets_px", &m_jets_px) ;
      m_smallT->Branch ("jets_py", &m_jets_py) ;
      m_smallT->Branch ("jets_pz", &m_jets_pz) ;
      m_smallT->Branch ("jets_e", &m_jets_e) ;
      m_smallT->Branch ("jets_btag", &m_jets_btag) ;
      m_smallT->Branch ("jets_isH", &m_jets_isH) ;
      m_smallT->Branch ("njets", &m_njets, "njets/I") ;
      
      // first four leptons
      m_smallT->Branch ("leps_px", &m_leps_px) ;
      m_smallT->Branch ("leps_py", &m_leps_py) ;
      m_smallT->Branch ("leps_pz", &m_leps_pz) ;
      m_smallT->Branch ("leps_e", &m_leps_e) ;
      m_smallT->Branch ("leps_flav", &m_leps_flav) ;
      m_smallT->Branch ("leps_isH", &m_leps_isH) ;
      m_smallT->Branch ("nleps", &m_nleps, "nleps/I") ;
      
      // MET
      m_smallT->Branch ("met_px", &m_met_px) ;
      m_smallT->Branch ("met_py", &m_met_py) ;
      m_smallT->Branch ("met_et", &m_met_et) ;
    
      // the tautau-H candidate
      m_smallT->Branch ("tauH_px", &m_tauH_px, "tauH_px/F") ;
      m_smallT->Branch ("tauH_py", &m_tauH_py, "tauH_py/F") ;
      m_smallT->Branch ("tauH_pz", &m_tauH_pz, "tauH_pz/F") ;
      m_smallT->Branch ("tauH_e", &m_tauH_e, "tauH_e/F") ;
    
      // the bb-H candidate
      m_smallT->Branch ("bH_px", &m_bH_px, "bH_px/F") ;
      m_smallT->Branch ("bH_py", &m_bH_py, "bH_py/F") ;
      m_smallT->Branch ("bH_pz", &m_bH_pz, "bH_pz/F") ;
      m_smallT->Branch ("bH_e", &m_bH_e, "bH_e/F") ;
      
      // the di-higgs candidate
      m_smallT->Branch ("HH_px", &m_HH_px, "HH_px/F") ;
      m_smallT->Branch ("HH_py", &m_HH_py, "HH_py/F") ;
      m_smallT->Branch ("HH_pz", &m_HH_pz, "HH_pz/F") ;
      m_smallT->Branch ("HH_e", &m_HH_e, "HH_e/F") ;
      m_smallT->Branch ("MC_weight", &m_MC_weight, "MC_weight/F") ;
      m_smallT->Branch ("PUReweight", &m_PUReweight, "PUReweight/F") ;

      return 0 ;
    }

  // the tree itself
  TTree * m_smallT ;
  
  // first five jets
  std::vector<Float_t> m_jets_px ;
  std::vector<Float_t> m_jets_py ;
  std::vector<Float_t> m_jets_pz ;
  std::vector<Float_t> m_jets_e ;
  std::vector<Int_t> m_jets_btag ;
  std::vector<Int_t> m_jets_isH ;
  Int_t m_njets ;
  
  // first four leptons
  std::vector<Float_t> m_leps_px ;
  std::vector<Float_t> m_leps_py ;
  std::vector<Float_t> m_leps_pz ;
  std::vector<Float_t> m_leps_e ;
  std::vector<Int_t> m_leps_flav ;
  std::vector<Int_t> m_leps_isH ;
  Int_t m_nleps ;
  
  // MET
  std::vector<Float_t> m_met_px ;
  std::vector<Float_t> m_met_py ;
  std::vector<Float_t> m_met_et ;

  // the tautau-H candidate
  Float_t m_tauH_px ;
  Float_t m_tauH_py ;
  Float_t m_tauH_pz ;
  Float_t m_tauH_e ;

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

  // general variables
  Float_t m_MC_weight ;
  Float_t m_PUReweight ;
  Int_t m_EventNumber ;
  Int_t m_RunNumber ;
} ;

#endif
