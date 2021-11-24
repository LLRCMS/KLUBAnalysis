// Author: L. Cadamuro (LLR)
// Date:   09 March 2015
//
// Wrapper for Higgs Tau Tau tree output tree
// Can be either included in a interpreted macro or compiled in c++
// (use `root-config --glibs --cflags`)
//
// Create the HTauTauTree object from the pointer to the tree, then access the stored objects from it
// Common TTree functions GetEntry (entry), GetEntries() are implemented

#ifndef HTAUTAUTREE_H
#define HTAUTAUTREE_H

#include <TROOT.h>
#include <TChain.h>
#include <TTree.h>
// #include <Rtypes.h>
//#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.

using namespace std;

class HTauTauTree {
public :
  TChain          *_tree;   //!pointer to the analyzed TTree or TChain
  /* TTree          *_tree;   //!pointer to the analyzed TTree or TChain */

  // Declaration of leaf types
  Int_t           EventNumber;
  Int_t           RunNumber;
  Int_t           lumi;
  Int_t           triggerbit;
  Int_t           metfilterbit;
  Float_t         met;
  Float_t         lheHt;
  Int_t           lheNOutPartons;
  Float_t         metphi;
  Int_t           npv;
  Float_t           npu;
  Float_t         PUReweight;
  Float_t         aMCatNLOweight;
  Float_t         rho;
  vector<float>   *daughters_HLTpt;
  vector<float>   *mothers_px;
  vector<float>   *mothers_py;
  vector<float>   *mothers_pz;
  vector<float>   *mothers_e;
  vector<string>  *trigger_name;
  vector<int>     *trigger_accept;
  //vector<float>   *trigger_accept;
  //vector<string>   *trigger_name;
  vector<float>   *daughters_px;
  vector<float>   *daughters_py;
  vector<float>   *daughters_pz;
  vector<float>   *daughters_e;
  vector<int>     *daughters_charge;
  vector<int>     *daughters_genindex;
  Float_t         MC_weight;
  vector<float>   *genpart_px;
  vector<float>   *genpart_py;
  vector<float>   *genpart_pz;
  vector<float>   *genpart_e;
  vector<int>     *genpart_pdg;
  vector<int>     *genpart_status;
  vector<int>     *genpart_HMothInd;
  vector<int>     *genpart_TopMothInd;
  vector<int>     *genpart_TauMothInd;
  vector<int>     *genpart_ZMothInd;
  vector<int>     *genpart_HZDecayMode;
  vector<int>     *genpart_TauGenDecayMode;
  vector<int>     *genpart_flags;
  Int_t           NUP;
  vector<float>   *SVfitMass;
  vector<float>   *SVfit_pt;
  vector<float>   *SVfit_ptUnc;
  vector<float>   *SVfit_eta;
  vector<float>   *SVfit_etaUnc;
  vector<float>   *SVfit_phi;
  vector<float>   *SVfit_phiUnc;
  vector<float>   *SVfit_fitMETRho;
  vector<float>   *SVfit_fitMETPhi;
  vector<bool>    *isOSCand;
  vector<float>   *METx;
  vector<float>   *METy;
  vector<float>   *MET_cov00;
  vector<float>   *MET_cov01;
  vector<float>   *MET_cov10;
  vector<float>   *MET_cov11;
  vector<float>   *MET_significance;
  vector<float>   *mT_Dau1;
  vector<float>   *mT_Dau2;
  vector<int>     *PDGIdDaughters;
  vector<int>     *indexDau1;
  vector<int>     *indexDau2;
  vector<int>     *particleType;
  vector<float>   *discriminator;
  vector<int>     *daughters_muonID;
  vector<Long64_t>     *tauID;
  vector<int>     *daughters_typeOfMuon;
  vector<float>   *dxy;
  vector<float>   *dz;
  vector<bool>    *daughters_iseleBDT;
  vector<bool>    *daughters_iseleWP80;
  vector<bool>    *daughters_iseleWP90;
  vector<float>    *daughters_eleMVAnt;
  vector<bool>    *daughters_passConversionVeto;
  vector<int>     *daughters_eleCUTID;
  vector<int>     *decayMode;
  vector<float>   *combreliso;
  vector<float>   *daughters_IetaIeta;
  vector<int>     *daughters_TauUpExists;
  vector<float>   *daughters_deltaPhiSuperClusterTrackAtVtx;
  vector<float>   *daughters_SCeta;
  vector<float>   *daughters_depositR03_tracker;
  vector<float>   *daughters_depositR03_ecal;
  vector<float>   *daughters_depositR03_hcal;
  vector<int>     *daughters_decayModeFindingOldDMs;
  vector<int>     *daughters_decayModeFindingNewDMs;
  vector<float>   *daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits;
  vector<float>   *daughters_byIsolationMVA3oldDMwoLTraw;
  vector<float>   *daughters_byIsolationMVA3newDMwoLTraw;
  vector<float>   *daughters_byIsolationMVA3oldDMwLTraw;
  vector<float>   *daughters_byIsolationMVA3newDMwLTraw;
  vector<float>   *daughters_photonPtSumOutsideSignalCone;
  vector<float>   *daughters_chargedIsoPtSum;
  vector<float>   *daughters_neutralIsoPtSum;
  vector<float>   *daughters_trackRefPt;
  vector<float>   *daughters_leadChargedParticlePt;
  vector<int>   *daughters_numParticlesIsoCone;
  vector<int>   *daughters_numPhotonsIsoCone;
  vector<int>   *daughters_numNeutralHadronsIsoCone;
  vector<int>   *daughters_numChargedParticlesIsoCone;
  vector<int>   *daughters_numParticlesSignalCone;
  vector<int>   *daughters_numPhotonsSignalCone;
  vector<int>   *daughters_numNeutralHadronsSignalCone;
  vector<int>   *daughters_numChargedParticlesSignalCone;
  vector<float>   *daughters_puCorrPtSum;
  vector<int>     *daughters_isLastTriggerObjectforPath;
  vector<int>     *daughters_isTriggerObjectforPath;
  vector<int>     *daughters_FilterFired;
  vector<bool>    *daughters_isGoodTriggerType;
  vector<int>     *daughters_L3FilterFired;
  vector<int>     *daughters_L3FilterFiredLast;
  Int_t           JetsNumber;
  vector<float>   *jets_px;
  vector<float>   *jets_py;
  vector<float>   *jets_pz;
  vector<float>   *jets_e;
  vector<int>     *jets_Flavour;
  vector<float>   *jets_PUJetID;
  vector<float>   *bDiscriminator;
  vector<float>   *bCSVscore;
  vector<int>     *PFjetID;
  vector<float>   *jetRawf;

  // List of branches
  TBranch        *b_EventNumber;   //!
  TBranch        *b_RunNumber;   //!
  TBranch        *b_lumi;   //!
  TBranch        *b_triggerbit;   //!
  TBranch        *b_metfilterbit;   //!
  TBranch        *b_met;   //!
  TBranch        *b_metphi;   //!
  TBranch        *b_npv;   //!
  TBranch        *b_npu;   //!
  TBranch        *b_lheHt;
  TBranch        *b_lheNOutPartons;
  TBranch        *b_PUReweight;   //!
  TBranch        *b_aMCatNLOweight;   //!
  TBranch        *b_rho;   //!
  TBranch        *b_daughters_HLTpt;   //!
  TBranch        *b_mothers_px;   //!
  TBranch        *b_mothers_py;   //!
  TBranch        *b_mothers_pz;   //!
  TBranch        *b_mothers_e;   //!
  TBranch        *b_trigger_name;   //!
  TBranch        *b_trigger_accept;   //!
  //TBranch        *b_trigger_accept;   //!
  //TBranch        *b_trigger_name;   //!
  TBranch        *b_daughters_px;   //!
  TBranch        *b_daughters_py;   //!
  TBranch        *b_daughters_pz;   //!
  TBranch        *b_daughters_e;   //!
  TBranch        *b_daughters_charge;   //!
  TBranch        *b_daughters_genindex;   //!
  TBranch        *b_MC_weight;   //!
  TBranch        *b_genpart_px;   //!
  TBranch        *b_genpart_py;   //!
  TBranch        *b_genpart_pz;   //!
  TBranch        *b_genpart_e;   //!
  TBranch        *b_genpart_pdg;   //!
  TBranch        *b_genpart_status;   //!
  TBranch        *b_genpart_HMothInd;   //!
  TBranch        *b_genpart_TopMothInd;   //!
  TBranch        *b_genpart_TauMothInd;   //!
  TBranch        *b_genpart_ZMothInd;   //!
  TBranch        *b_genpart_HZDecayMode;   //!
  TBranch        *b_genpart_TauGenDecayMode;   //!
  TBranch        *b_genpart_flags;   //!
  TBranch        *b_NUP;   //!
  TBranch        *b_SVfitMass;   //!
  TBranch        *b_SVfit_pt;   //!
  TBranch        *b_SVfit_ptUnc;   //!
  TBranch        *b_SVfit_eta;   //!
  TBranch        *b_SVfit_etaUnc;   //!
  TBranch        *b_SVfit_phi;   //!
  TBranch        *b_SVfit_phiUnc;   //!
  TBranch        *b_SVfit_fitMETRho;   //!
  TBranch        *b_SVfit_fitMETPhi;   //!
  TBranch        *b_isOSCand;   //!
  TBranch        *b_METx;   //!
  TBranch        *b_METy;   //!
  TBranch        *b_MET_cov00;   //!
  TBranch        *b_MET_cov01;   //!
  TBranch        *b_MET_cov10;   //!
  TBranch        *b_MET_cov11;   //!
  TBranch        *b_MET_significance;   //!
  TBranch        *b_mT_Dau1;   //!
  TBranch        *b_mT_Dau2;   //!
  TBranch        *b_PDGIdDaughters;   //!
  TBranch        *b_indexDau1;   //!
  TBranch        *b_indexDau2;   //!
  TBranch        *b_particleType;   //!
  TBranch        *b_discriminator;   //!
  TBranch        *b_daughters_muonID;   //!
  TBranch        *b_tauID;
  TBranch        *b_daughters_typeOfMuon;   //!
  TBranch        *b_dxy;   //!
  TBranch        *b_dz;   //!
  TBranch        *b_daughters_iseleBDT;   //!
  TBranch        *b_daughters_iseleWP80;   //!
  TBranch        *b_daughters_iseleWP90;   //!
  TBranch        *b_daughters_eleMVAnt;   //!
  TBranch        *b_daughters_passConversionVeto;   //!
  TBranch        *b_daughters_eleCUTID;   //!
  TBranch        *b_decayMode;   //!
  TBranch        *b_combreliso;   //!
  TBranch        *b_daughters_IetaIeta;   //!
  TBranch        *b_daughters_TauUpExists;   //!
  TBranch        *b_daughters_deltaPhiSuperClusterTrackAtVtx;   //!
  TBranch        *b_daughters_SCeta;   //!
  TBranch        *b_daughters_depositR03_tracker;   //!
  TBranch        *b_daughters_depositR03_ecal;   //!
  TBranch        *b_daughters_depositR03_hcal;   //!
  TBranch        *b_daughters_decayModeFindingOldDMs;   //!
  TBranch        *b_daughters_decayModeFindingNewDMs;   //!
  TBranch        *b_daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits;   //!
  TBranch        *b_daughters_byIsolationMVA3oldDMwLTraw;   //!
  TBranch        *b_daughters_byIsolationMVA3newDMwLTraw;   //!
  TBranch        *b_daughters_byIsolationMVA3oldDMwoLTraw;   //!
  TBranch        *b_daughters_byIsolationMVA3newDMwoLTraw;   //!
  TBranch        *b_daughters_photonPtSumOutsideSignalCone;
  TBranch        *b_daughters_chargedIsoPtSum;   //!
  TBranch        *b_daughters_neutralIsoPtSum;   //!
  TBranch        *b_daughters_trackRefPt;
  TBranch        *b_daughters_numPhotonsSignalCone;
  TBranch        *b_daughters_leadChargedParticlePt;
  TBranch        *b_daughters_numParticlesSignalCone;
  TBranch        *b_daughters_numNeutralHadronsSignalCone;
  TBranch        *b_daughters_numChargedParticlesSignalCone;
  TBranch        *b_daughters_numParticlesIsoCone;
  TBranch        *b_daughters_numPhotonsIsoCone;
  TBranch        *b_daughters_numNeutralHadronsIsoCone;
  TBranch        *b_daughters_numChargedParticlesIsoCone;
  TBranch        *b_daughters_puCorrPtSum;   //!
  TBranch        *b_daughters_isLastTriggerObjectforPath;   //!
  TBranch        *b_daughters_isTriggerObjectforPath;   //!
  TBranch        *b_daughters_FilterFired;   //!
  TBranch        *b_daughters_isGoodTriggerType;   //!
  TBranch        *b_daughters_L3FilterFired;   //!
  TBranch        *b_daughters_L3FilterFiredLast;   //!
  TBranch        *b_JetsNumber;   //!
  TBranch        *b_jets_px;   //!
  TBranch        *b_jets_py;   //!
  TBranch        *b_jets_pz;   //!
  TBranch        *b_jets_e;   //!
  TBranch        *b_jets_Flavour;   //!
  TBranch        *b_jets_PUJetID;   //!
  TBranch        *b_bDiscriminator;   //!
  TBranch        *b_bCSVscore;   //!
  TBranch        *b_PFjetID;   //!
  TBranch        *b_jetRawf;   //!

  // methods
  HTauTauTree (TChain* tree); //ctor
  /* HTauTauTree (TTree* tree); //ctor */
  ~HTauTauTree();
  void Init(TChain* tree);
  /* void Init(TTree* tree); */
  Int_t GetEntry(int entry);
  Long64_t GetEntries();
  TChain* GetTree();
  /* TTree* GetTree(); */
};

HTauTauTree::HTauTauTree (TChain* tree)
/* HTauTauTree::HTauTauTree (TTree* tree) */
{
  Init(tree);
}

HTauTauTree::~HTauTauTree() {}

void HTauTauTree::Init(TChain* tree)
/* void HTauTauTree::Init(TTree* tree) */
{

  // Set object pointer
  mothers_px = 0;
  mothers_py = 0;
  mothers_pz = 0;
  mothers_e = 0;
  trigger_name = 0;
  trigger_accept = 0;
  //trigger_accept = 0;
  daughters_px = 0;
  daughters_py = 0;
  daughters_pz = 0;
  daughters_e = 0;
  daughters_charge = 0;
  daughters_genindex = 0;
  genpart_px = 0;
  genpart_py = 0;
  genpart_pz = 0;
  genpart_e = 0;
  genpart_pdg = 0;
  genpart_status = 0;
  genpart_HMothInd = 0;
  genpart_TopMothInd = 0;
  genpart_TauMothInd = 0;
  genpart_ZMothInd = 0;
  genpart_HZDecayMode = 0;
  genpart_TauGenDecayMode = 0;
  genpart_flags = 0;
  SVfitMass = 0;
  SVfit_pt = 0;
  SVfit_ptUnc = 0;
  SVfit_eta = 0;
  SVfit_etaUnc = 0;
  SVfit_phi = 0;
  SVfit_phiUnc = 0;
  SVfit_fitMETRho = 0;
  SVfit_fitMETPhi = 0;
  isOSCand = 0;
  METx = 0;
  METy = 0;
  MET_cov00 = 0;
  MET_cov01 = 0;
  MET_cov10 = 0;
  MET_cov11 = 0;
  MET_significance = 0;
  mT_Dau1 = 0;
  mT_Dau2 = 0;
  PDGIdDaughters = 0;
  indexDau1 = 0;
  indexDau2 = 0;
  particleType = 0;
  discriminator = 0;
  daughters_muonID = 0;
  tauID=0;
  daughters_typeOfMuon = 0;
  dxy = 0;
  dz = 0;
  daughters_iseleBDT = 0;
  daughters_iseleWP80 = 0;
  daughters_iseleWP90 = 0;
  daughters_eleMVAnt = 0;
  daughters_passConversionVeto = 0;
  daughters_eleCUTID = 0;
  decayMode = 0;
  combreliso = 0;
  daughters_IetaIeta = 0;
  daughters_deltaPhiSuperClusterTrackAtVtx = 0;
  daughters_SCeta = 0;
  daughters_depositR03_tracker = 0;
  daughters_depositR03_ecal = 0;
  daughters_depositR03_hcal = 0;
  daughters_decayModeFindingOldDMs = 0;
  daughters_decayModeFindingNewDMs = 0;
  daughters_TauUpExists = 0;
  daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits = 0;
  daughters_byIsolationMVA3newDMwLTraw=0;
  daughters_byIsolationMVA3oldDMwLTraw=0;
  daughters_byIsolationMVA3newDMwoLTraw=0;
  daughters_byIsolationMVA3oldDMwoLTraw=0;
  daughters_photonPtSumOutsideSignalCone=0;
  daughters_chargedIsoPtSum = 0;
  daughters_neutralIsoPtSum = 0;
  daughters_trackRefPt = 0;
  daughters_leadChargedParticlePt = 0;
  daughters_numChargedParticlesSignalCone = 0;
  daughters_numNeutralHadronsSignalCone = 0;
  daughters_numParticlesSignalCone = 0;
  daughters_numPhotonsSignalCone = 0;
  daughters_numChargedParticlesIsoCone = 0;
  daughters_numNeutralHadronsIsoCone = 0;
  daughters_numParticlesIsoCone = 0;
  daughters_numPhotonsIsoCone = 0;
  daughters_puCorrPtSum = 0;
  daughters_isLastTriggerObjectforPath = 0;
  daughters_isTriggerObjectforPath = 0;
  daughters_FilterFired = 0;
  daughters_isGoodTriggerType = 0;
  daughters_L3FilterFired = 0;
  daughters_L3FilterFiredLast = 0;
  daughters_HLTpt=0;
  jets_px = 0;
  jets_py = 0;
  jets_pz = 0;
  jets_e = 0;
  jets_Flavour = 0;
  jets_PUJetID = 0;
  bDiscriminator = 0;
  bCSVscore = 0;
  PFjetID = 0;
  jetRawf = 0;
  lheHt = 0;
  lheNOutPartons = 0;

  cout<<"hello there0"<<endl;

  // Set branch addresses and branch pointers
  if (!tree) return;
  _tree = tree;
  _tree->SetMakeClass(1); // needed especially when compiling

  cout<<"hello there0.5"<<endl;

  _tree->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
  _tree->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber);
  _tree->SetBranchAddress("lumi", &lumi, &b_lumi);
  _tree->SetBranchAddress("triggerbit", &triggerbit, &b_triggerbit);
  _tree->SetBranchAddress("metfilterbit", &metfilterbit, &b_metfilterbit);
  _tree->SetBranchAddress("met", &met, &b_met);
  _tree->SetBranchAddress("metphi", &metphi, &b_metphi);
  _tree->SetBranchAddress("npv", &npv, &b_npv);
  _tree->SetBranchAddress("npu", &npu, &b_npu);
  _tree->SetBranchAddress("lheHt", &lheHt, &b_lheHt);
  _tree->SetBranchAddress("lheNOutPartons", &lheNOutPartons, &b_lheNOutPartons);
  _tree->SetBranchAddress("PUReweight", &PUReweight, &b_PUReweight);
  _tree->SetBranchAddress("aMCatNLOweight", &aMCatNLOweight, &b_aMCatNLOweight);
  _tree->SetBranchAddress("rho", &rho, &b_rho);
  _tree->SetBranchAddress("daughters_HLTpt", &daughters_HLTpt, &b_daughters_HLTpt);
  _tree->SetBranchAddress("mothers_px", &mothers_px, &b_mothers_px);
  _tree->SetBranchAddress("mothers_py", &mothers_py, &b_mothers_py);
  _tree->SetBranchAddress("mothers_pz", &mothers_pz, &b_mothers_pz);
  _tree->SetBranchAddress("mothers_e", &mothers_e, &b_mothers_e);
  _tree->SetBranchAddress("trigger_name", &trigger_name, &b_trigger_name);
  _tree->SetBranchAddress("trigger_accept", &trigger_accept, &b_trigger_accept);
  _tree->SetBranchAddress("daughters_px", &daughters_px, &b_daughters_px);
  _tree->SetBranchAddress("daughters_py", &daughters_py, &b_daughters_py);
  _tree->SetBranchAddress("daughters_pz", &daughters_pz, &b_daughters_pz);
  _tree->SetBranchAddress("daughters_e", &daughters_e, &b_daughters_e);
  _tree->SetBranchAddress("daughters_charge", &daughters_charge, &b_daughters_charge);

  _tree->SetBranchAddress("SVfitMass", &SVfitMass, &b_SVfitMass);
  _tree->SetBranchAddress("SVfit_pt", &SVfit_pt, &b_SVfit_pt);
  _tree->SetBranchAddress("SVfit_ptUnc", &SVfit_ptUnc, &b_SVfit_ptUnc);
  _tree->SetBranchAddress("SVfit_eta", &SVfit_eta, &b_SVfit_eta);
  _tree->SetBranchAddress("SVfit_etaUnc", &SVfit_etaUnc, &b_SVfit_etaUnc);
  _tree->SetBranchAddress("SVfit_phi", &SVfit_phi, &b_SVfit_phi);
  _tree->SetBranchAddress("SVfit_phiUnc", &SVfit_phiUnc, &b_SVfit_phiUnc);
  _tree->SetBranchAddress("SVfit_fitMETRho", &SVfit_fitMETRho, &b_SVfit_fitMETRho);
  _tree->SetBranchAddress("SVfit_fitMETPhi", &SVfit_fitMETPhi, &b_SVfit_fitMETPhi);
  _tree->SetBranchAddress("isOSCand", &isOSCand, &b_isOSCand);
  _tree->SetBranchAddress("METx", &METx, &b_METx);
  _tree->SetBranchAddress("METy", &METy, &b_METy);
  _tree->SetBranchAddress("MET_cov00", &MET_cov00, &b_MET_cov00);
  _tree->SetBranchAddress("MET_cov01", &MET_cov01, &b_MET_cov01);
  _tree->SetBranchAddress("MET_cov10", &MET_cov10, &b_MET_cov10);
  _tree->SetBranchAddress("MET_cov11", &MET_cov11, &b_MET_cov11);
  _tree->SetBranchAddress("MET_significance", &MET_significance, &b_MET_significance);
  _tree->SetBranchAddress("mT_Dau1", &mT_Dau1, &b_mT_Dau1);
  _tree->SetBranchAddress("mT_Dau2", &mT_Dau2, &b_mT_Dau2);
  _tree->SetBranchAddress("PDGIdDaughters", &PDGIdDaughters, &b_PDGIdDaughters);
  _tree->SetBranchAddress("indexDau1", &indexDau1, &b_indexDau1);
  _tree->SetBranchAddress("indexDau2", &indexDau2, &b_indexDau2);
  _tree->SetBranchAddress("particleType", &particleType, &b_particleType);
  _tree->SetBranchAddress("discriminator", &discriminator, &b_discriminator);
  _tree->SetBranchAddress("daughters_muonID", &daughters_muonID, &b_daughters_muonID);
  _tree->SetBranchAddress("tauID", &tauID, &b_tauID);
  _tree->SetBranchAddress("daughters_typeOfMuon", &daughters_typeOfMuon, &b_daughters_typeOfMuon);
  _tree->SetBranchAddress("dxy", &dxy, &b_dxy);
  _tree->SetBranchAddress("dz", &dz, &b_dz);
  _tree->SetBranchAddress("daughters_iseleBDT", &daughters_iseleBDT, &b_daughters_iseleBDT);
  _tree->SetBranchAddress("daughters_iseleWP80", &daughters_iseleWP80, &b_daughters_iseleWP80);
  _tree->SetBranchAddress("daughters_iseleWP90", &daughters_iseleWP90, &b_daughters_iseleWP90);
  _tree->SetBranchAddress("daughters_eleMVAnt", &daughters_eleMVAnt, &b_daughters_eleMVAnt);
  _tree->SetBranchAddress("daughters_passConversionVeto", &daughters_passConversionVeto, &b_daughters_passConversionVeto);
  _tree->SetBranchAddress("daughters_eleCUTID", &daughters_eleCUTID, &b_daughters_eleCUTID);
  _tree->SetBranchAddress("decayMode", &decayMode, &b_decayMode);
  _tree->SetBranchAddress("combreliso", &combreliso, &b_combreliso);
  _tree->SetBranchAddress("daughters_IetaIeta", &daughters_IetaIeta, &b_daughters_IetaIeta);
  _tree->SetBranchAddress("daughters_TauUpExists", &daughters_TauUpExists, &b_daughters_TauUpExists);
  _tree->SetBranchAddress("daughters_deltaPhiSuperClusterTrackAtVtx", &daughters_deltaPhiSuperClusterTrackAtVtx, &b_daughters_deltaPhiSuperClusterTrackAtVtx);
  _tree->SetBranchAddress("daughters_SCeta", &daughters_SCeta, &b_daughters_SCeta);
  _tree->SetBranchAddress("daughters_depositR03_tracker", &daughters_depositR03_tracker, &b_daughters_depositR03_tracker);
  _tree->SetBranchAddress("daughters_depositR03_ecal", &daughters_depositR03_ecal, &b_daughters_depositR03_ecal);
  _tree->SetBranchAddress("daughters_depositR03_hcal", &daughters_depositR03_hcal, &b_daughters_depositR03_hcal);
  _tree->SetBranchAddress("daughters_decayModeFindingOldDMs", &daughters_decayModeFindingOldDMs, &b_daughters_decayModeFindingOldDMs);
  _tree->SetBranchAddress("daughters_decayModeFindingNewDMs", &daughters_decayModeFindingNewDMs, &b_daughters_decayModeFindingNewDMs);
  _tree->SetBranchAddress("daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits", &daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits, &b_daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits);
  _tree->SetBranchAddress("daughters_photonPtSumOutsideSignalCone", &daughters_photonPtSumOutsideSignalCone, &b_daughters_photonPtSumOutsideSignalCone);
  _tree->SetBranchAddress("daughters_byIsolationMVA3oldDMwoLTraw", &daughters_byIsolationMVA3oldDMwoLTraw, &b_daughters_byIsolationMVA3oldDMwoLTraw);
  _tree->SetBranchAddress("daughters_byIsolationMVA3newDMwoLTraw", &daughters_byIsolationMVA3newDMwoLTraw, &b_daughters_byIsolationMVA3newDMwoLTraw);
  _tree->SetBranchAddress("daughters_byIsolationMVA3oldDMwLTraw", &daughters_byIsolationMVA3oldDMwLTraw, &b_daughters_byIsolationMVA3oldDMwLTraw);
  _tree->SetBranchAddress("daughters_byIsolationMVA3newDMwLTraw", &daughters_byIsolationMVA3newDMwLTraw, &b_daughters_byIsolationMVA3newDMwLTraw);
  _tree->SetBranchAddress("daughters_chargedIsoPtSum", &daughters_chargedIsoPtSum, &b_daughters_chargedIsoPtSum);
  _tree->SetBranchAddress("daughters_neutralIsoPtSum", &daughters_neutralIsoPtSum, &b_daughters_neutralIsoPtSum);
  _tree->SetBranchAddress("daughters_trackRefPt", &daughters_trackRefPt, &b_daughters_trackRefPt);
  _tree->SetBranchAddress("daughters_leadChargedParticlePt", &daughters_leadChargedParticlePt, &b_daughters_leadChargedParticlePt);
  _tree->SetBranchAddress("daughters_numParticlesSignalCone", &daughters_numParticlesSignalCone, &b_daughters_numParticlesSignalCone);
  _tree->SetBranchAddress("daughters_numParticlesIsoCone", &daughters_numParticlesIsoCone, &b_daughters_numParticlesIsoCone);
  _tree->SetBranchAddress("daughters_numNeutralHadronsSignalCone", &daughters_numNeutralHadronsSignalCone, &b_daughters_numNeutralHadronsSignalCone);
  _tree->SetBranchAddress("daughters_numNeutralHadronsIsoCone", &daughters_numNeutralHadronsIsoCone, &b_daughters_numNeutralHadronsIsoCone);
  _tree->SetBranchAddress("daughters_numPhotonsSignalCone", &daughters_numPhotonsSignalCone, &b_daughters_numPhotonsSignalCone);
  _tree->SetBranchAddress("daughters_numPhotonsIsoCone", &daughters_numPhotonsIsoCone, &b_daughters_numPhotonsIsoCone);
  _tree->SetBranchAddress("daughters_numChargedParticlesIsoCone", &daughters_numChargedParticlesIsoCone, &b_daughters_numChargedParticlesIsoCone);
  _tree->SetBranchAddress("daughters_numChargedParticlesSignalCone", &daughters_numChargedParticlesSignalCone, &b_daughters_numChargedParticlesSignalCone);
  _tree->SetBranchAddress("daughters_puCorrPtSum", &daughters_puCorrPtSum, &b_daughters_puCorrPtSum);
  _tree->SetBranchAddress("daughters_isLastTriggerObjectforPath", &daughters_isLastTriggerObjectforPath, &b_daughters_isLastTriggerObjectforPath);
  _tree->SetBranchAddress("daughters_isTriggerObjectforPath", &daughters_isTriggerObjectforPath, &b_daughters_isTriggerObjectforPath);
  _tree->SetBranchAddress("daughters_FilterFired", &daughters_FilterFired, &b_daughters_FilterFired);
  _tree->SetBranchAddress("daughters_isGoodTriggerType", &daughters_isGoodTriggerType, &b_daughters_isGoodTriggerType);
  _tree->SetBranchAddress("daughters_L3FilterFired", &daughters_L3FilterFired, &b_daughters_L3FilterFired);
  _tree->SetBranchAddress("daughters_L3FilterFiredLast", &daughters_L3FilterFiredLast, &b_daughters_L3FilterFiredLast);
  _tree->SetBranchAddress("JetsNumber", &JetsNumber, &b_JetsNumber);
  _tree->SetBranchAddress("jets_px", &jets_px, &b_jets_px);
  _tree->SetBranchAddress("jets_py", &jets_py, &b_jets_py);
  _tree->SetBranchAddress("jets_pz", &jets_pz, &b_jets_pz);
  _tree->SetBranchAddress("jets_e", &jets_e, &b_jets_e);
  _tree->SetBranchAddress("jets_Flavour", &jets_Flavour, &b_jets_Flavour);
  _tree->SetBranchAddress("jets_PUJetID", &jets_PUJetID, &b_jets_PUJetID);
  _tree->SetBranchAddress("bDiscriminator", &bDiscriminator, &b_bDiscriminator);
  _tree->SetBranchAddress("bCSVscore", &bCSVscore, &b_bCSVscore);
  _tree->SetBranchAddress("PFjetID", &PFjetID, &b_PFjetID);
  _tree->SetBranchAddress("jetRawf", &jetRawf, &b_jetRawf);

  cout<<"hello there0.6"<<endl;


  // MC only
  if(1)
    /* if(_tree->GetListOfBranches()->FindObject("genpart_px")) */
  {
    cout<<"hello there0.7"<<endl;

    _tree->SetBranchAddress("daughters_genindex", &daughters_genindex, &b_daughters_genindex);
    _tree->SetBranchAddress("MC_weight", &MC_weight, &b_MC_weight);
    _tree->SetBranchAddress("genpart_px", &genpart_px, &b_genpart_px);
    _tree->SetBranchAddress("genpart_py", &genpart_py, &b_genpart_py);
    _tree->SetBranchAddress("genpart_pz", &genpart_pz, &b_genpart_pz);
    _tree->SetBranchAddress("genpart_e", &genpart_e, &b_genpart_e);
    _tree->SetBranchAddress("genpart_pdg", &genpart_pdg, &b_genpart_pdg);
    _tree->SetBranchAddress("genpart_status", &genpart_status, &b_genpart_status);
    _tree->SetBranchAddress("genpart_HMothInd", &genpart_HMothInd, &b_genpart_HMothInd);
    _tree->SetBranchAddress("genpart_TopMothInd", &genpart_TopMothInd, &b_genpart_TopMothInd);
    _tree->SetBranchAddress("genpart_TauMothInd", &genpart_TauMothInd, &b_genpart_TauMothInd);
    _tree->SetBranchAddress("genpart_ZMothInd", &genpart_ZMothInd, &b_genpart_ZMothInd);
    _tree->SetBranchAddress("genpart_HZDecayMode", &genpart_HZDecayMode, &b_genpart_HZDecayMode);
    _tree->SetBranchAddress("genpart_TauGenDecayMode", &genpart_TauGenDecayMode, &b_genpart_TauGenDecayMode);
    _tree->SetBranchAddress("genpart_flags", &genpart_flags, &b_genpart_flags);
    _tree->SetBranchAddress("NUP", &NUP, &b_NUP);
  }


  cout<<"hello there"<<endl;

}

Int_t HTauTauTree::GetEntry(int entry)
{
  return _tree->GetEntry(entry);
}

Long64_t HTauTauTree::GetEntries()
{
  return _tree->GetEntries();
}

TChain* HTauTauTree::GetTree()
/* TTree* HTauTauTree::GetTree() */
{
  return _tree;
}

#endif
