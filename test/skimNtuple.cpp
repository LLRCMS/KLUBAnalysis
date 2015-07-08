// c++ -o skimNtuple `root-config --cflags --glibs --libs` -lm skimNtuple.cpp

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

using namespace std ;

int main (int argc, char** argv)
{
  if (argc < 3) 
    {
      cerr << "missing input parameters" << endl ;
      exit (1) ;
    }
  TString inputFile = argv[1] ;
  TString outputFile = argv[2] ;

  TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;
  bigChain->Add (inputFile) ;
  bigTree theBigTree (bigChain) ;

  //Create a new file + a clone of old tree header. Do not copy events
  TFile * smallFile = new TFile (outputFile, "recreate") ;
  smallTree theSmallTree ("HTauTauTree") ;

  int eventsNumber = theBigTree.fChain->GetEntries () ;
  eventsNumber = 10 ; //DEBUG
  float selectedEvents = 0 ;

  // loop over events
  for (Long64_t iEvent = 0 ; iEvent < eventsNumber ; ++iEvent) 
    {
      if (iEvent % 10000 == 0) cout << "reading event " << iEvent << endl ;

      theSmallTree.clearVars () ;
      theBigTree.GetEntry (iEvent) ;
 
      cout << theBigTree.indexDau1->size () << endl ;
      
      //PG the H > tautau candidate
      //PG by now take the OS pair with largest pT
      int firstDaughterIndex = theBigTree.indexDau1->at (0) ;
      
      cout << firstDaughterIndex << endl ;
      cout << theBigTree.daughters_px->size () << endl ;
      TLorentzVector firstLepton
        (
          theBigTree.daughters_px->at (firstDaughterIndex),
          theBigTree.daughters_py->at (firstDaughterIndex),
          theBigTree.daughters_pz->at (firstDaughterIndex),
          theBigTree.daughters_e->at (firstDaughterIndex)
        ) ;
      int secondDaughterIndex = theBigTree.indexDau2->at (0) ;
      TLorentzVector secondLepton
        (
          theBigTree.daughters_px->at (secondDaughterIndex),
          theBigTree.daughters_py->at (secondDaughterIndex),
          theBigTree.daughters_pz->at (secondDaughterIndex),
          theBigTree.daughters_e->at (secondDaughterIndex)
        ) ;


      //PG apply some selections here
      //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

      if (false) continue ;

      //PG fill the variables of interest
      //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

      for (int iJet = 0 ; 
           (iJet < theBigTree.JetsNumber) && (theSmallTree.m_njets < 5) ; 
           ++iJet)
        {
          // PG filter jets at will
          theSmallTree.m_jets_px.push_back (theBigTree.jets_px->at (iJet)) ;
          theSmallTree.m_jets_py.push_back (theBigTree.jets_py->at (iJet)) ;
          theSmallTree.m_jets_pz.push_back (theBigTree.jets_pz->at (iJet)) ;
          theSmallTree.m_jets_e.push_back (theBigTree.jets_e->at (iJet)) ;
// whether it's a b-tagged jet or not
//           theSmallTree.m_jets_btag.push_back (theBigTree. ->at (iJet) ;
// whether it's one of the jets produced by the H decay
//           theSmallTree.m_jets_isH.push_back (theBigTree. ->at (iJet) ;
          ++theSmallTree.m_njets ;
        }

      theSmallTree.m_PUReweight = theBigTree.PUReweight ;
      theSmallTree.m_MC_weight = theBigTree.MC_weight ;
      theSmallTree.m_EventNumber = theBigTree.EventNumber ;
      theSmallTree.m_RunNumber = theBigTree.RunNumber ;
      
      ++selectedEvents ; 
      theSmallTree.Fill () ;
    } // loop over events

  TH1F h_eff ("h_eff", "h_eff", 1, 0, 1) ;
  h_eff.Fill (0.5, selectedEvents / eventsNumber) ;
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
