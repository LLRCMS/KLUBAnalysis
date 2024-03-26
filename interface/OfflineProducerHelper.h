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
#include <tuple>

// using namespace std;

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

  enum eleMVAIDWP {
    EMVATight  = 0, // 80% eff
    EMVAMedium = 1, // 90% eff
    EMVALoose  = 2  // Loose WP
  };

  enum muIDWP {
    MuLoose  = 0,
    MuSoft   = 1,
    MuMedium = 2,
    MuTight  = 3,
    MuHighPt = 4
  };

// deepTauVsJet: 0 = VVVLoose, 1 = VVLoose, 2 = VLoose, 3 = Loose, 4 = Medium, 5 = Tight, 6 = VTight, 7 = VVTight
// deepTauVsEle: 0 = VVVLoose, 1 = VVLoose, 2 = VLoose, 3 = Loose, 4 = Medium, 5 = Tight, 6 = VTight, 7 = VVTight
// deepTauVsMu: 0 = VLoose, 1 = Loose, 2 = Medium, 3 = Tight

  enum aeleWP {
    aeleVVVLoose = 0,
    aeleVVLoose  = 1,
    aeleVLoose   = 2,
    aeleLoose    = 3,
    aeleMedium   = 4,
    aeleTight    = 5,
    aeleVTight   = 6,
    aeleVVTight  = 7
  };

  enum amuWP {
    amuVLoose = 0,
    amuLoose  = 1,
    amuMedium = 2,
    amuTight  = 3
  };

/*   enum aeleWP {
     aeleVLoose = 0,
     aeleLoose  = 1,
     aeleMedium = 2,
     aeleTight  = 3,
     aeleVTight = 4
     };

     enum amuWP {
     amuLoose = 0,
     amuTight = 1
     };
*/
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
  bool checkBit (long word, long bitpos); // check bit "bitpos" in a word
  int getTAUidNumber(TString tauIDname);

  // whatApply: use "OSCharge" (appplies on pairs only)
  // whatApply: use "All", "Iso", "pTMin", "etaMax", "againstEle", "againstMu", "Vertex"; separate various arguments with a semicolon
  // is contains "All" it will override all the other settings; additional parameters are not considered (have no effect)
  // a selection is applied by default if no parameter is specified
  bool checkPassBaseline (bigTree* tree, int iPair, TString whatApply = "All"){return pairPassBaseline(tree,iPair,whatApply,true);}
  bool pairPassBaseline (bigTree* tree, int iPair, TString whatApply = "All", bool debug=false);
  bool eleBaseline (bigTree* tree, int iDau, float ptMin, float relIso,  int MVAIDflag = 0, TString whatApply = "All", bool debug=false); // return true if leptons passes the baseline selections
  bool eleBaseline (bigTree* tree, int iDau, float ptMin, float etaMax, float relIso,  int MVAIDflag = 0, TString whatApply = "All", bool debug=false); // version with eta cut
  bool muBaseline (bigTree* tree, int iDau, float ptMin, float etaMax, float relIsopf, int muIDWPpf, float relIsotk, int muIDWPtk, TString whatApply = "All", bool debug=false);
  bool tauBaseline (bigTree* tree, int iDau, float ptMin, float etaMax, int againstEleWP, int againstMuWP, float isoRaw3Hits, TString whatApply = "All",bool debug=false);
  bool tightEleMVAID (float BDT, float fSCeta); // compute tight ele MVA id WP, but isBDT in ntuples has been fixed --> this will be soon deprecated

  int getMothPairType (bigTree* tree, int iMoth); // return the pair type of a given pair in the tree

  // approx!! I call it using lepton eta and not superCluster eta
  bool EleMVAID (float BDT, float eta, float pT, int strength) ;
  // bool jetPassPuID (bigTree* tree, int ijet);

  TLorentzVector buildDauP4 (bigTree* tree, int iDau); // build daughter 4 vector
  TLorentzVector buildMothP4 (bigTree* tree, int iMoth); // build pair 4 vector
  TLorentzVector buildGenP4 (bigTree* tree, int iGen); // build pair 4 vector
  bool getBestJets (bigTree* tree, int& jet1, int& jet2, int strategy); // select jets, possibly two b jets, returns true if found, else false
  int getBestPair (bigTree* tree, std::vector<int>& pairIdxs, TString strategy = "OSMaxPt"); // from a vector of indexes to pairs in the evemt retutn index to the one chose by a stategy
  int getBestPair (bigTree* tree, TString strategy = "OSMaxPt"); // calls the previous on the whole pair collection of the event
  int getPairByIndexes (bigTree* tree, int dau1, int dau2); // knowing the sons, get the pair formed

  typedef std::tuple <float, float, int, float, float, int, int> tauPair_t; // pt1 - iso1 - idx1 - pt2 - iso2 - idx2 - idxoriginalPair
  int getBestPairHTauTau (bigTree* tree, TString whatApply = "All", bool debug = false); // returns best pair formed by idx1, idx2, using HTauTau strategy - for studies
  int getBestPairPtAndRawIsoOrd (bigTree* tree, TString whatApply = "All", bool debug = false); // returns best pair formed by idx1, idx2, sorting them by pt in each pair, then by raw iso
  static bool pairSort (const tauPair_t& pA, const tauPair_t& pB);
  static bool pairSortRawIso (const tauPair_t& pA, const tauPair_t& pB);
  float DeltaRDau(bigTree* tree, int dau1idx, int dau2idx);
  const float getEtaCut(std::string);

  // --------------------------------
  // gen related functions
  // --------------------------------
  int MCHiggsTauTauDecayMode (bigTree* tree); // find the MC decay of the Higgs to tau in the event
  bool getHardTauFinalVisGenProducts (bigTree* tree, int& ind1, int& ind2); // find hard scatter tau decay products and store their indices; return false if problems, else true
  bool drMatchGenReco (bigTree* tree, int iGen, int iReco, float dRcone = 0.5);
  int getRecoMatchedToGen (bigTree* tree, int iGen, bool checkId = true, bool checkCharge = false, float dRcone = 0.5);

  ~OfflineProducerHelper(){}

private:
  std::vector<TString> triggerlist;
  std::vector<TString> tauidlist;
  const float eleEtaMax=2.5, muEtaMax=2.4, tauEtaMax=2.3;
};

#endif
