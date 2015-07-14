#include <iostream>
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "TBranch.h"
#include "TString.h"
#include "TLorentzVector.h"

// bigTree is produced on an existing ntuple as follows (see at the end of the file) 
#include "bigTree.h" 
#include "smallTree.h"
#include "OfflineProducerHelper.h"
#include "../../HHKinFit/interface/HHKinFitMaster.h"

using namespace std ;

/*  the modern way of making lorenzvectors (no warnings)
    here is an example of usage:

https://github.com/govoni/LHEAnalysis/blob/master/epem_01.cpp

    here are the headers needed:

#include "Math/Vector3D.h"
#include "Math/Vector4D.h"

using namespace ROOT::Math;

typedef LorentzVector<ROOT::Math::PxPyPzE4D<double> > lorentzVector ;

    I am missing about how to set one of those things up with Pt, eta, phi coordinates

*/


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


struct leptSort: public std::binary_function<pair<TLorentzVector, float> &, pair<TLorentzVector, float> &, bool>
{
  bool operator() (pair<TLorentzVector, float> & x, pair<TLorentzVector, float> & y)
    {
      return x.first.Pt () < y.first.Pt () ;
    }
} ;


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


template <class T>
struct scoreSortSingle: public std::binary_function<pair <T, float> &, pair <T, float> &, bool>
{
  bool operator() (pair <T, float> & x, pair <T, float> & y)
    {
      return x.second < y.second ;
    }
} ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char** argv)
{
  if (argc < 4) 
    {
      cerr << "missing input parameters" << endl ;
      exit (1) ;
    }
  TString inputFile = argv[1] ;
  TString outputFile = argv[2] ;
  float XS = atof (argv[3]) ;

  OfflineProducerHelper oph ;

  TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;
  bigChain->Add (inputFile) ;
  bigTree theBigTree (bigChain) ;

  //Create a new file + a clone of old tree header. Do not copy events
  TFile * smallFile = new TFile (outputFile, "recreate") ;
  smallTree theSmallTree ("HTauTauTree") ;

  // initial parameters, might be read from a cfg file in the future
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  float PUjetID_minCut = -0.5 ;
  bool  isMC           = true ;
  bool  saveOS         = true ; // save same-sign candidates

  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<Int_t> hypo_mh1 ; //FIXME why is this an integer?!
  hypo_mh1.push_back (125) ;
  vector<Int_t> hypo_mh2 ;
  hypo_mh2.push_back (125) ;

  int eventsNumber = theBigTree.fChain->GetEntries () ;
//  eventsNumber = 100 ; //DEBUG
  float totalEvents = 0. ;
  float selectedEvents = 0. ;

  int totalMCEventsNum = 0 ;
  int selectedMCEventsNum = 0 ;

  // loop over events
  for (Long64_t iEvent = 0 ; iEvent < eventsNumber ; ++iEvent) 
    {
      if (iEvent % 10000 == 0)  cout << "reading event " << iEvent << endl ;

      theSmallTree.clearVars () ;
      theBigTree.GetEntry (iEvent) ;
      
      totalEvents += theBigTree.MC_weight * XS ;
      ++totalMCEventsNum ;
      
      if (theBigTree.indexDau1->size () == 0) continue ;
      if (theBigTree.jets_px->size () < 2)    continue ;
 
      // the H > tautau candidate
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
      
      // by now take the OS pair with largest pT
      int chosenTauPair = -1 ;
      
      for (int iPair = 0 ; iPair < theBigTree.indexDau1->size () ; ++iPair)
        {
          if (theBigTree.isOSCand->at (iPair) != saveOS) continue ;
          if (oph.pairPassBaseline (&theBigTree, iPair, "All"))
            {
              chosenTauPair = iPair ;
              break ; 
            }
          
        }  
      if (chosenTauPair < 0) continue ;

      int firstDaughterIndex = theBigTree.indexDau1->at (chosenTauPair) ;  
      TLorentzVector tlv_firstLepton
        (
          theBigTree.daughters_px->at (firstDaughterIndex),
          theBigTree.daughters_py->at (firstDaughterIndex),
          theBigTree.daughters_pz->at (firstDaughterIndex),
          theBigTree.daughters_e->at (firstDaughterIndex)
        ) ;
      int secondDaughterIndex = theBigTree.indexDau2->at (chosenTauPair) ;
      TLorentzVector tlv_secondLepton
        (
          theBigTree.daughters_px->at (secondDaughterIndex),
          theBigTree.daughters_py->at (secondDaughterIndex),
          theBigTree.daughters_pz->at (secondDaughterIndex),
          theBigTree.daughters_e->at (secondDaughterIndex)
        ) ;

      TLorentzVector tlv_tauH = tlv_firstLepton + tlv_secondLepton ;

      // the H > bb candidate
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

      vector <pair <int, float> > jets_and_btag ;
      vector <pair <pair <int, int>, float> > pairs_and_btag ;      
      // loop over jets
      for (int iJet = 0 ; iJet < theBigTree.jets_px->size () ; ++iJet)
        {
          // PG filter jets at will
          if (theBigTree.jets_PUJetID->at (iJet) < PUjetID_minCut) continue ;
          
          jets_and_btag.push_back (std::pair <int, float> (
              iJet, theBigTree.bCSVscore->at (iJet)
            )) ;          
        } // loop over jets

      if (jets_and_btag.size () < 2) continue ;
      sort (jets_and_btag.rbegin (), jets_and_btag.rend (), scoreSortSingle<int> ()) ;
      
      int firstBjetIndex = jets_and_btag.at (0).first ;
      int secondBjetIndex = jets_and_btag.at (1).first ;

      // apply some selections here
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

      if (false) continue ;

      // fill the variables of interest
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

      theSmallTree.m_PUReweight = theBigTree.PUReweight ;
      theSmallTree.m_MC_weight = theBigTree.MC_weight * XS ;
      theSmallTree.m_EventNumber = theBigTree.EventNumber ;
      theSmallTree.m_RunNumber = theBigTree.RunNumber ;
      theSmallTree.m_npv = theBigTree.npv ;
      theSmallTree.m_npu = theBigTree.npu ;
      theSmallTree.m_lumi = theBigTree.lumi ;
      theSmallTree.m_triggerbit = theBigTree.triggerbit ;
      theSmallTree.m_rho = theBigTree.rho ;
      theSmallTree.m_isMC = isMC ;
      theSmallTree.m_isOS = theBigTree.isOSCand->at (chosenTauPair) ;

      int type1 = theBigTree.particleType->at (firstDaughterIndex) ;
      int type2 = theBigTree.particleType->at (secondDaughterIndex) ;
      theSmallTree.m_pairType = oph.getPairType (type1, type2) ;
      
      theSmallTree.m_tauH_pt = tlv_tauH.Pt () ;
      theSmallTree.m_tauH_eta = tlv_tauH.Eta () ;
      theSmallTree.m_tauH_phi = tlv_tauH.Phi () ;
      theSmallTree.m_tauH_e = tlv_tauH.E () ;

      theSmallTree.m_dau1_pt = tlv_firstLepton.Pt () ;
      theSmallTree.m_dau1_eta = tlv_firstLepton.Eta () ;
      theSmallTree.m_dau1_phi = tlv_firstLepton.Phi () ;
      theSmallTree.m_dau1_e = theBigTree.daughters_e->at (firstDaughterIndex) ;
      theSmallTree.m_dau1_flav = theBigTree.daughters_charge->at (firstDaughterIndex) * 
                                 (theBigTree.particleType->at (firstDaughterIndex) + 1) ;
                                 // 1 = from muons collection
                                 // 2 = from electrons collection
                                 // 3 = from tauH collection
                                 
      theSmallTree.m_dau2_pt = tlv_secondLepton.Pt () ;
      theSmallTree.m_dau2_eta = tlv_secondLepton.Eta () ;
      theSmallTree.m_dau2_phi = tlv_secondLepton.Phi () ;
      theSmallTree.m_dau2_e = theBigTree.daughters_e->at (secondDaughterIndex) ;
      theSmallTree.m_dau2_flav = theBigTree.daughters_charge->at (secondDaughterIndex) * 
                                 (theBigTree.particleType->at (secondDaughterIndex) + 1) ;

      TLorentzVector tlv_firstBjet 
        (
          theBigTree.jets_px->at (firstBjetIndex),
          theBigTree.jets_py->at (firstBjetIndex),
          theBigTree.jets_pz->at (firstBjetIndex),
          theBigTree.jets_e->at (firstBjetIndex)
        ) ;
      TLorentzVector tlv_secondBjet 
        (
          theBigTree.jets_px->at (secondBjetIndex),
          theBigTree.jets_py->at (secondBjetIndex),
          theBigTree.jets_pz->at (secondBjetIndex),
          theBigTree.jets_e->at (secondBjetIndex)
        ) ;

      theSmallTree.m_bjet1_pt  = tlv_firstBjet.Pt () ;
      theSmallTree.m_bjet1_eta = tlv_firstBjet.Eta () ;
      theSmallTree.m_bjet1_phi = tlv_firstBjet.Phi () ;
      theSmallTree.m_bjet1_e = theBigTree.jets_e->at (firstBjetIndex) ;
      theSmallTree.m_bjet1_bID = theBigTree.bCSVscore->at (firstBjetIndex) ;

      theSmallTree.m_bjet2_pt  = tlv_secondBjet.Pt () ;
      theSmallTree.m_bjet2_eta = tlv_secondBjet.Eta () ;
      theSmallTree.m_bjet2_phi = tlv_secondBjet.Phi () ;
      theSmallTree.m_bjet2_e = theBigTree.jets_e->at (secondBjetIndex) ;
      theSmallTree.m_bjet2_bID = theBigTree.bCSVscore->at (secondBjetIndex) ;

      float METx = theBigTree.METx->at (chosenTauPair) ;
      float METy = theBigTree.METy->at (chosenTauPair) ;
      float METpt = TMath::Sqrt (METx*METx + METy*METy) ;
    
      TLorentzVector ptmiss = TLorentzVector (METx, METy, 0., METpt) ;
      TMatrixD metcov (2, 2) ;
      metcov (0,0) = theBigTree.MET_cov00->at (chosenTauPair) ;
      metcov (1,0) = theBigTree.MET_cov10->at (chosenTauPair) ;
      metcov (0,1) = theBigTree.MET_cov01->at (chosenTauPair) ;    
      metcov (1,1) = theBigTree.MET_cov11->at (chosenTauPair) ;
    
      TLorentzVector tlv_bH = tlv_firstBjet + tlv_secondBjet ;
      theSmallTree.m_bH_pt = tlv_bH.Pt () ;
      theSmallTree.m_bH_eta = tlv_bH.Eta () ;
      theSmallTree.m_bH_phi = tlv_bH.Phi () ;
      theSmallTree.m_bH_e = tlv_bH.E () ;

      TLorentzVector tlv_HH = tlv_bH + tlv_tauH ;
      theSmallTree.m_HH_pt = tlv_HH.Pt () ;
      theSmallTree.m_HH_eta = tlv_HH.Eta () ;
      theSmallTree.m_HH_phi = tlv_HH.Phi () ;
      theSmallTree.m_HH_e = tlv_HH.E () ;

      // in case the SVFIT mass is calculated
      if (theBigTree.SVfitMass->at (chosenTauPair) > -900.)
        {
          theSmallTree.m_tauH_SVFIT_mass = theBigTree.SVfitMass->at (chosenTauPair) ;


          theSmallTree.m_tauH_SVFIT_pt = theBigTree.SVfit_pt->at (chosenTauPair) ;
          theSmallTree.m_tauH_SVFIT_eta = theBigTree.SVfit_eta->at (chosenTauPair) ;
          theSmallTree.m_tauH_SVFIT_phi = theBigTree.SVfit_phi->at (chosenTauPair) ;
          theSmallTree.m_tauH_SVFIT_METphi = theBigTree.SVfit_fitMETPhi->at (chosenTauPair) ;
          theSmallTree.m_tauH_SVFIT_METrho = theBigTree.SVfit_fitMETRho->at (chosenTauPair) ;

          TLorentzVector tlv_tauH_SVFIT ;
          tlv_tauH_SVFIT.SetPtEtaPhiM (
              theBigTree.SVfit_pt->at (chosenTauPair),
              theBigTree.SVfit_eta->at (chosenTauPair),
              theBigTree.SVfit_phi->at (chosenTauPair),
              theBigTree.SVfitMass->at (chosenTauPair)
            ) ;

          TLorentzVector tlv_HHsvfit = tlv_bH + tlv_tauH_SVFIT ;
          theSmallTree.m_HHsvfit_pt = tlv_HHsvfit.Pt () ;
          theSmallTree.m_HHsvfit_eta = tlv_HHsvfit.Eta () ;
          theSmallTree.m_HHsvfit_phi = tlv_HHsvfit.Phi () ;
          theSmallTree.m_HHsvfit_e = tlv_HHsvfit.E () ;
        } // in case the SVFIT mass is calculated
        
      //intance of fitter master class
      HHKinFitMaster kinFits = HHKinFitMaster (&tlv_firstBjet, &tlv_secondBjet, 
                                               &tlv_firstLepton, &tlv_secondLepton) ;
      kinFits.setAdvancedBalance (&ptmiss, metcov) ;
      //kinFits.setSimpleBalance (ptmiss.Pt (),10) ; //alternative which uses only the absolute value of ptmiss in the fit

      kinFits.addMh1Hypothesis (hypo_mh1) ;
      kinFits.addMh2Hypothesis (hypo_mh2) ;
      kinFits.doFullFit () ;

      theSmallTree.m_HHKin_mass = kinFits.getBestMHFullFit () ;
      theSmallTree.m_HHKin_chi2 = kinFits.getBestChi2FullFit () ;

      // loop over leptons
      vector<pair<TLorentzVector, float> > dummyLeptCollection ;
      for (int iLep = 0 ; 
           (iLep < theBigTree.daughters_px->size ()) ;
           ++iLep)
        {
          // skip the H decay candiates
          if (iLep == firstDaughterIndex || iLep == secondDaughterIndex) continue ;

          // remove taus
          if (theBigTree.particleType->at (iLep) == 2)
            {
              continue ;
            }  
          else if (theBigTree.particleType->at (iLep) == 0) // muons
            {
              if (!oph.muBaseline (&theBigTree, iLep, 10., 2.4, 0.3)) continue ;
            }
          else if (theBigTree.particleType->at (iLep) == 1) // electrons
            {
              if (!oph.eleBaseline (&theBigTree, iLep, 10., 2.5, 0.3, 1)) continue ;
            }
          TLorentzVector tlv_dummyLepton
            (
              theBigTree.daughters_px->at (iLep),
              theBigTree.daughters_py->at (iLep),
              theBigTree.daughters_pz->at (iLep),
              theBigTree.daughters_e->at (iLep)
            ) ;
          dummyLeptCollection.push_back (pair<TLorentzVector, float> (
              tlv_dummyLepton, 
              theBigTree.daughters_charge->at (iLep) * 
              (theBigTree.particleType->at (iLep) + 1)              
            )) ;
          
        } // loop over leptons

      sort (dummyLeptCollection.rbegin (), dummyLeptCollection.rend (), leptSort ()) ;
      for (int iLep = 0 ; 
           (iLep < dummyLeptCollection.size ()) && (theSmallTree.m_nleps < 2) ;
           ++iLep)
        {
          theSmallTree.m_leps_pt.push_back (dummyLeptCollection.at (iLep).first.Pt ()) ;
          theSmallTree.m_leps_eta.push_back (dummyLeptCollection.at (iLep).first.Eta ()) ;
          theSmallTree.m_leps_phi.push_back (dummyLeptCollection.at (iLep).first.Phi ()) ;
          theSmallTree.m_leps_e.push_back (dummyLeptCollection.at (iLep).first.E ()) ;
          theSmallTree.m_leps_flav.push_back (dummyLeptCollection.at (iLep).second) ;
          ++theSmallTree.m_nleps ;
        } 

      // loop over jets
      for (int iJet = 0 ; 
           (iJet < theBigTree.jets_px->size ()) && (theSmallTree.m_njets < 3) ;
           ++iJet)
        {
          // PG filter jets at will
          if (theBigTree.jets_PUJetID->at (iJet) < PUjetID_minCut) continue ;
          
          // skip the H decay candiates
          if (iJet == firstBjetIndex || iJet == secondBjetIndex) continue ;

          TLorentzVector tlv_dummyJet (
              theBigTree.jets_px->at (iJet),
              theBigTree.jets_py->at (iJet),
              theBigTree.jets_pz->at (iJet),
              theBigTree.jets_e->at (iJet)
            ) ;
      
          theSmallTree.m_jets_pt.push_back (tlv_dummyJet.Pt ()) ;
          theSmallTree.m_jets_eta.push_back (tlv_dummyJet.Eta ()) ;
          theSmallTree.m_jets_phi.push_back (tlv_dummyJet.Phi ()) ;
          theSmallTree.m_jets_e.push_back (theBigTree.jets_e->at (iJet)) ;
          theSmallTree.m_jets_btag.push_back (theBigTree.bCSVscore->at (iJet)) ;
          ++theSmallTree.m_njets ;
        } // loop over jets

      selectedEvents += theBigTree.MC_weight * XS ; 
      ++selectedMCEventsNum ;
      theSmallTree.Fill () ;
    } // loop over events

  cout << "efficiency = " << selectedEvents / totalEvents << endl ;
  TH1F h_eff ("h_eff", "h_eff", 2, 0, 2) ;
  h_eff.Fill (0.5, totalEvents) ;
  h_eff.Fill (1.5, selectedEvents) ;
  h_eff.Fill (2.5, totalMCEventsNum) ;
  h_eff.Fill (3.5, selectedMCEventsNum) ;
  h_eff.Write () ;
  smallFile->Write () ;
  delete smallFile ;
  return 0 ;
}



/* how to prepare bigTree:

 prepare the base class with ROOT
 ------------------------------------------------

   root /data_CMS/cms/govoni/test_submit_to_tier3/HiggsTauTauOutput_DY_-1Events_0Skipped_1436202480.82/output_9.root 
   TTree * t = (TTree *) _file0->Get ("HTauTauTree/HTauTauTree")
   t->MakeSelector ("bigTree")

 remove the following functions from the .h
 ------------------------------------------------
 
   virtual void    Init(TTree *tree);
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

 move the Init implementation in the class definition
 ------------------------------------------------
 
 change the following line
 ------------------------------------------------
 
    bigTree(TTree ETC....  --->
    bigTree (TChain * inputChain) : fChain (inputChain) { Init(fChain) ; }

 remove all the rest of the implementations
 ------------------------------------------------

 delete bigTree.C
 ------------------------------------------------

 remove the following lines to simplify things
 ------------------------------------------------

#endif

#ifdef bigTree_cxx

 remove the inheritance:
 ------------------------------------------------

class bigTree : public TSelector { --->
class bigTree { --->

 remove the following lines
 ------------------------------------------------

 #include <TSelector.h>

 remove the following lines from Init
 ------------------------------------------------

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

 remove the ClassDef call
 ------------------------------------------------

*/
