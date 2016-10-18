#ifndef HHKINFIT2
#define HHKINFIT2
#endif

#ifndef OfflineHelper_h
#define OfflineHelper_h
/** \class OfflineProducerHelper
 *
 *  Collection of functions to analyze HTauTau primary trees, without CMSSW dependencies.
 *
 *  $Date: 2012/06/06 00:27:43 $
 *  $Revision: 1.3 $
 *  \author G. Ortona - LLR
 */
 
#include <TString.h>
#include "TMath.h"
#include "bigTree.h"
#include "TLorentzVector.h"
#include <iostream>
#include <vector>
#include <utility>
#include <TH1F.h>

using namespace std;
 
class OfflineProducerHelper {
 public:
  enum particleType {
  MUON = 0,
  ELECTRON = 1,
  TAU =2
  };
  float m_MVAEleIDCuts[2][2][3] ;

  enum pairType {
    MuHad  = 0,
    EHad   = 1,
    HadHad = 2,
    MuMu   = 3,
    EE     = 4,
    EMu    = 5,
    EEPrompt = 6, // prompt Z->ee/mumu decays
    MuMuPrompt = 7,
    Other  = 8 // for e.g. h->bb
  };

  OfflineProducerHelper();
  OfflineProducerHelper(TH1F* hCounter, TH1F *htauids);
  OfflineProducerHelper(TH1F* hCounter);
  void SetEleMVAIDCuts();

  int FindTriggerNumber(TString triggername);
  bool IsTriggerFired(int triggerbit, int triggerNumber);
  bool IsTriggerFired(int triggerbit, TString triggerName){return IsTriggerFired(triggerbit, FindTriggerNumber(triggerName));}
  int printFiredPaths(int);
  bool isMuon(int type){if(type == MUON)return true; else return false;}
  bool isElectron(int type){if(type == ELECTRON)return true; else return false;}
  bool isTau(int type){if(type == TAU)return true; else return false;}
  int getPairType (int type1, int type2); // return pair type giving as input the particle types of the two composants
  bool checkBit (int word, int bitpos); // check bit "bitpos" in a word
  int getTAUidNumber(TString tauIDname);
  
  // whatApply: use "OSCharge" (appplies on pairs only)
  // whatApply: use "All", "Iso", "pTMin", "etaMax", "againstEle", "againstMu", "Vertex"; separate various arguments with a semicolon
  // is contains "All" it will override all the other settings; additional parameters are not considered (have no effect) 
  // a selection is applied by default if no parameter is specified
  bool checkPassBaseline (bigTree* tree, int iPair, TString whatApply = "All"){return pairPassBaseline(tree,iPair,whatApply,true);}
  bool pairPassBaseline (bigTree* tree, int iPair, TString whatApply = "All", bool debug=false);
  bool eleBaseline (bigTree* tree, int iDau, float ptMin, float relIso,  int MVAIDflag = 0, TString whatApply = "All", bool debug=false); // return true if leptons passes the baseline selections
  bool eleBaseline (bigTree* tree, int iDau, float ptMin, float etaMax, float relIso,  int MVAIDflag = 0, TString whatApply = "All", bool debug=false); // version with eta cut
  bool muBaseline (bigTree* tree, int iDau, float ptMin, float etaMax, float relIso, TString whatApply = "All", bool debug=false);
  bool tauBaseline (bigTree* tree, int iDau, float ptMin, float etaMax, int againstEleWP, int againstMuWP, float isoRaw3Hits, TString whatApply = "All",bool debug=false);
  bool tightEleMVAID (float BDT, float fSCeta); // compute tight ele MVA id WP, but isBDT in ntuples has been fixed --> this will be soon deprecated

  int getMothPairType (bigTree* tree, int iMoth); // return the pair type of a given pair in the tree
                                                
                                                // approx!! I call it using lepton eta and not superCluster eta
  bool EleMVAID (float BDT, float eta, float pT, int strength) ;
  // bool jetPassPuID (bigTree* tree, int ijet);

  /*
  // separate check of various requirements applied in baseline
  bool combRelIso (bigTree* tree, int iDau, float iso); // for mu, ele
  bool combIsodBetaRaw3Hits (bigTree* tree, int iDau, float iso); // for tau
  bool passEleID (bigTree* tree, int iDau); // tight ele ID
  bool passMuID (bigTree* tree, int iDau); // medium mu ID
  bool passTauID (bigTree* tree, int iDau); // tau pog ID (decay Mode Finding OLD || NEW)
  bool passTauAntiEle (bigTree* tree, int iDau, int againstEleWP);
  bool passTauAntiMu  (bigTree* tree, int iDau, int againstMuWP); 
  */

  TLorentzVector buildDauP4 (bigTree* tree, int iDau); // build daughter 4 vector
  TLorentzVector buildMothP4 (bigTree* tree, int iMoth); // build pair 4 vector
  TLorentzVector buildGenP4 (bigTree* tree, int iGen); // build pair 4 vector
  bool getBestJets (bigTree* tree, int& jet1, int& jet2, int strategy); // select jets, possibly two b jets, returns true if found, else false
  int getBestPair (bigTree* tree, std::vector<int>& pairIdxs, TString strategy = "OSMaxPt"); // from a vector of indexes to pairs in the evemt retutn index to the one chose by a stategy
  int getBestPair (bigTree* tree, TString strategy = "OSMaxPt"); // calls the previous on the whole pair collection of the event
  int getPairByIndexes (bigTree* tree, int dau1, int dau2); // knowing the sons, get the pair formed  
  float DeltaRDau(bigTree* tree, int dau1idx, int dau2idx);
  

  // --------------------------------
  // gen related functions
  // --------------------------------
  int MCHiggsTauTauDecayMode (bigTree* tree); // find the MC decay of the Higgs to tau in the event
  bool getHardTauFinalVisGenProducts (bigTree* tree, int& ind1, int& ind2); // find hard scatter tau decay products and store their indices; return false if problems, else true
  bool drMatchGenReco (bigTree* tree, int iGen, int iReco, float dRcone = 0.5);
  int getRecoMatchedToGen (bigTree* tree, int iGen, bool checkId = true, bool checkCharge = false, float dRcone = 0.5);

  ~OfflineProducerHelper(){}

 private:
  vector<TString> triggerlist;
  vector<TString> tauidlist;
  
};

#endif
