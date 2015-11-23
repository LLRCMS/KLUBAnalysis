//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jul  8 11:54:34 2015 by ROOT version 6.02/05
// from TTree HTauTauTree/HTauTauTree
// found on file: /data_CMS/cms/govoni/test_submit_to_tier3/HiggsTauTauOutput_DY_-1Events_0Skipped_1436202480.82/output_9.root
//////////////////////////////////////////////////////////

#ifndef bigTree_h
#define bigTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>

class bigTree {
public :
   TChain          * fChain ;   //!pointer to the analyzed TTree or TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           EventNumber;
   Int_t           RunNumber;
   Int_t           lumi;
   Long64_t        triggerbit;
   Int_t           metfilterbit;
   Float_t         met;
   Float_t         metphi;
   Int_t           npv;
   Int_t           npu;
   Float_t         PUReweight;
   Float_t         rho;
   std::vector<float>   *mothers_px;
   std::vector<float>   *mothers_py;
   std::vector<float>   *mothers_pz;
   std::vector<float>   *mothers_e;
   std::vector<float>   *daughters_px;
   std::vector<float>   *daughters_py;
   std::vector<float>   *daughters_pz;
   std::vector<float>   *daughters_e;
   std::vector<int>     *daughters_charge;
   std::vector<int>     *daughters_genindex;
   Float_t         MC_weight;
   Float_t         aMCatNLOweight;
   std::vector<float>   *genpart_px;
   std::vector<float>   *genpart_py;
   std::vector<float>   *genpart_pz;
   std::vector<float>   *genpart_e;
   std::vector<int>     *genpart_pdg;
   std::vector<int>     *genpart_status;
   std::vector<int>     *genpart_HMothInd;
   std::vector<int>     *genpart_TopMothInd;
   std::vector<int>     *genpart_TauMothInd;
   std::vector<int>     *genpart_ZMothInd;
   std::vector<int>     *genpart_HZDecayMode;
   std::vector<int>     *genpart_TauGenDecayMode;
   std::vector<int>     *genpart_flags;
   std::vector<float>   *genjet_px;
   std::vector<float>   *genjet_py;
   std::vector<float>   *genjet_pz;
   std::vector<float>   *genjet_e;
   std::vector<int>     *jets_genjetIndex;
   Int_t           NUP;
   std::vector<float>   *SVfitMass;
   std::vector<float>   *SVfit_pt;
   std::vector<float>   *SVfit_ptUnc;
   std::vector<float>   *SVfit_eta;
   std::vector<float>   *SVfit_etaUnc;
   std::vector<float>   *SVfit_phi;
   std::vector<float>   *SVfit_phiUnc;
   std::vector<float>   *SVfit_fitMETRho;
   std::vector<float>   *SVfit_fitMETPhi;
   std::vector<bool>    *isOSCand;
   std::vector<float>   *METx;
   std::vector<float>   *METy;
   std::vector<float>   *MET_cov00;
   std::vector<float>   *MET_cov01;
   std::vector<float>   *MET_cov10;
   std::vector<float>   *MET_cov11;
   std::vector<float>   *MET_significance;
   std::vector<float>   *mT_Dau1;
   std::vector<float>   *mT_Dau2;
   std::vector<int>     *PDGIdDaughters;
   std::vector<int>     *indexDau1;
   std::vector<int>     *indexDau2;
   std::vector<int>     *particleType;
   std::vector<float>   *discriminator;
   std::vector<int>     *daughters_muonID;
   std::vector<int>     *daughters_typeOfMuon;
   std::vector<float>   *dxy;
   std::vector<float>   *dz;
   std::vector<float>   *SIP;
   std::vector<bool>    *daughters_iseleBDT;
   std::vector<int>     *daughters_eleCUTID;
   std::vector<int>     *decayMode;
   std::vector<float>   *combreliso;
   std::vector<float>   *daughters_IetaIeta;
   std::vector<float>   *daughters_deltaPhiSuperClusterTrackAtVtx;
   std::vector<float>   *daughters_SCeta;
   std::vector<float>   *daughters_depositR03_tracker;
   std::vector<float>   *daughters_depositR03_ecal;
   std::vector<float>   *daughters_depositR03_hcal;
   std::vector<int>     *daughters_decayModeFindingOldDMs;
   std::vector<int>     *daughters_decayModeFindingNewDMs;
   std::vector<Long64_t> *tauID ;
//   std::vector<int>     *daughters_byLooseCombinedIsolationDeltaBetaCorr3Hits;
//   std::vector<int>     *daughters_byMediumCombinedIsolationDeltaBetaCorr3Hits;
//   std::vector<int>     *daughters_byTightCombinedIsolationDeltaBetaCorr3Hits;
   std::vector<float>   *daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits;
   std::vector<float>   *daughters_byIsolationMVA3oldDMwoLTraw;
   std::vector<float>   *daughters_byIsolationMVA3oldDMwLTraw;
   std::vector<float>   *daughters_byIsolationMVA3newDMwoLTraw;
   std::vector<float>   *daughters_byIsolationMVA3newDMwLTraw;
   std::vector<float>   *daughters_chargedIsoPtSum;
   std::vector<float>   *daughters_neutralIsoPtSum;
   std::vector<float>   *daughters_puCorrPtSum;
   std::vector<float>   *daughters_photonPtSumOutsideSignalCone;
//   std::vector<int>     *daughters_againstMuonLoose3;
//   std::vector<int>     *daughters_againstMuonTight3;
//   std::vector<int>     *daughters_againstElectronVLooseMVA5;
//   std::vector<int>     *daughters_againstElectronLooseMVA5;
//   std::vector<int>     *daughters_againstElectronMediumMVA5;
//   std::vector<int>     *daughters_againstElectronTightMVA5;
//   std::vector<int>     *daughters_againstElectronVTightMVA5;
   std::vector<int>     *daughters_isLastTriggerObjectforPath;
   std::vector<int>     *daughters_isTriggerObjectforPath;
   std::vector<int>     *daughters_FilterFired;
   std::vector<bool>    *daughters_isGoodTriggerType;
   std::vector<int>     *daughters_L3FilterFired;
   std::vector<int>     *daughters_L3FilterFiredLast;
   Int_t           JetsNumber;
   std::vector<float>   *jets_px;
   std::vector<float>   *jets_py;
   std::vector<float>   *jets_pz;
   std::vector<float>   *jets_e;
   std::vector<int>     *jets_Flavour;
   std::vector<int>     *jets_HadronFlavour;
   std::vector<float>   *jets_PUJetID;
   std::vector<float>   *bDiscriminator;
   std::vector<float>   *bCSVscore;
   std::vector<int>     *PFjetID;
   std::vector<float>   *jetRawf;

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
   TBranch        *b_PUReweight;   //!
   TBranch        *b_rho;   //!
   TBranch        *b_mothers_px;   //!
   TBranch        *b_mothers_py;   //!
   TBranch        *b_mothers_pz;   //!
   TBranch        *b_mothers_e;   //!
   TBranch        *b_daughters_px;   //!
   TBranch        *b_daughters_py;   //!
   TBranch        *b_daughters_pz;   //!
   TBranch        *b_daughters_e;   //!
   TBranch        *b_daughters_charge;   //!
   TBranch        *b_daughters_genindex;   //!
   TBranch        *b_MC_weight;   //!
   TBranch        *b_aMCatNLOweight;   //!
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
   TBranch        *b_genjet_px;   //!
   TBranch        *b_genjet_py;   //!
   TBranch        *b_genjet_pz;   //!
   TBranch        *b_genjet_e;   //!
   TBranch        *b_jets_genjetIndex; //!
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
   TBranch        *b_daughters_typeOfMuon;   //!
   TBranch        *b_dxy;   //!
   TBranch        *b_dz;   //!
   TBranch        *b_SIP;   //!
   TBranch        *b_daughters_iseleBDT;   //!
   TBranch        *b_daughters_eleCUTID;   //!
   TBranch        *b_decayMode;   //!
   TBranch        *b_combreliso;   //!
   TBranch        *b_daughters_IetaIeta;   //!
   TBranch        *b_daughters_deltaPhiSuperClusterTrackAtVtx;   //!
   TBranch        *b_daughters_SCeta;   //!
   TBranch        *b_daughters_depositR03_tracker;   //!
   TBranch        *b_daughters_depositR03_ecal;   //!
   TBranch        *b_daughters_depositR03_hcal;   //!
   TBranch        *b_daughters_decayModeFindingOldDMs;   //!
   TBranch        *b_daughters_decayModeFindingNewDMs;   //!
   TBranch        *b_tauID; //!
   TBranch        *b_daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits;   //!
   TBranch        *b_daughters_byIsolationMVA3oldDMwoLTraw;   //!
   TBranch        *b_daughters_byIsolationMVA3oldDMwLTraw;   //!
   TBranch        *b_daughters_byIsolationMVA3newDMwoLTraw;   //!
   TBranch        *b_daughters_byIsolationMVA3newDMwLTraw;   //!
   TBranch        *b_daughters_chargedIsoPtSum;   //!
   TBranch        *b_daughters_neutralIsoPtSum;   //!
   TBranch        *b_daughters_puCorrPtSum;   //!
   TBranch        *b_daughters_photonPtSumOutsideSignalCone; //!
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
   TBranch        *b_jets_HadronFlavour;   //!
   TBranch        *b_jets_PUJetID;   //!
   TBranch        *b_bDiscriminator;   //!
   TBranch        *b_bCSVscore;   //!
   TBranch        *b_PFjetID;   //!
   TBranch        *b_jetRawf;   //!

   bigTree (TChain * inputChain) : fChain (inputChain) { Init(fChain) ; fChain->GetEntries() ; }
   virtual ~bigTree() { }
//   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual Int_t   GetEntry(Long64_t entry) { return fChain->GetEntry(entry) ; }

    void Init(TChain *tree)
    {
       // The Init() function is called when the selector needs to initialize
       // a new tree or chain. Typically here the branch addresses and branch
       // pointers of the tree will be set.
       // It is normally not necessary to make changes to the generated
       // code, but the routine can be extended by the user if needed.
       // Init() will be called many times when running on PROOF
       // (once per file to be processed).

       // Set object pointer
       mothers_px = 0;
       mothers_py = 0;
       mothers_pz = 0;
       mothers_e = 0;
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
       genjet_px = 0;
       genjet_py = 0;
       genjet_pz = 0;
       genjet_e = 0;
       jets_genjetIndex = 0;
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
       daughters_typeOfMuon = 0;
       dxy = 0;
       dz = 0;
       SIP = 0;
       daughters_iseleBDT = 0;
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
       tauID = 0;
       daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits = 0;
       daughters_byIsolationMVA3oldDMwoLTraw = 0;
       daughters_byIsolationMVA3oldDMwLTraw = 0;
       daughters_byIsolationMVA3newDMwoLTraw = 0;
       daughters_byIsolationMVA3newDMwLTraw = 0;
       daughters_chargedIsoPtSum = 0;
       daughters_neutralIsoPtSum = 0;
       daughters_puCorrPtSum = 0;
       daughters_photonPtSumOutsideSignalCone = 0;
       daughters_isLastTriggerObjectforPath = 0;
       daughters_isTriggerObjectforPath = 0;
       daughters_FilterFired = 0;
       daughters_isGoodTriggerType = 0;
       daughters_L3FilterFired = 0;
       daughters_L3FilterFiredLast = 0;
       jets_px = 0;
       jets_py = 0;
       jets_pz = 0;
       jets_e = 0;
       jets_Flavour = 0;
       jets_HadronFlavour = 0;
       jets_PUJetID = 0;
       bDiscriminator = 0;
       bCSVscore = 0;
       PFjetID = 0;
       jetRawf = 0;

       fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
       fChain->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber);
       fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
       fChain->SetBranchAddress("triggerbit", &triggerbit, &b_triggerbit);
       fChain->SetBranchAddress("metfilterbit", &metfilterbit, &b_metfilterbit);
       fChain->SetBranchAddress("met", &met, &b_met);
       fChain->SetBranchAddress("metphi", &metphi, &b_metphi);
       fChain->SetBranchAddress("npv", &npv, &b_npv);
       fChain->SetBranchAddress("npu", &npu, &b_npu);
       fChain->SetBranchAddress("PUReweight", &PUReweight, &b_PUReweight);
       fChain->SetBranchAddress("rho", &rho, &b_rho);
       fChain->SetBranchAddress("mothers_px", &mothers_px, &b_mothers_px);
       fChain->SetBranchAddress("mothers_py", &mothers_py, &b_mothers_py);
       fChain->SetBranchAddress("mothers_pz", &mothers_pz, &b_mothers_pz);
       fChain->SetBranchAddress("mothers_e", &mothers_e, &b_mothers_e);
       fChain->SetBranchAddress("daughters_px", &daughters_px, &b_daughters_px);
       fChain->SetBranchAddress("daughters_py", &daughters_py, &b_daughters_py);
       fChain->SetBranchAddress("daughters_pz", &daughters_pz, &b_daughters_pz);
       fChain->SetBranchAddress("daughters_e", &daughters_e, &b_daughters_e);
       fChain->SetBranchAddress("daughters_charge", &daughters_charge, &b_daughters_charge);
       fChain->SetBranchAddress("SVfitMass", &SVfitMass, &b_SVfitMass);
       fChain->SetBranchAddress("SVfit_pt", &SVfit_pt, &b_SVfit_pt);
       fChain->SetBranchAddress("SVfit_ptUnc", &SVfit_ptUnc, &b_SVfit_ptUnc);
       fChain->SetBranchAddress("SVfit_eta", &SVfit_eta, &b_SVfit_eta);
       fChain->SetBranchAddress("SVfit_etaUnc", &SVfit_etaUnc, &b_SVfit_etaUnc);
       fChain->SetBranchAddress("SVfit_phi", &SVfit_phi, &b_SVfit_phi);
       fChain->SetBranchAddress("SVfit_phiUnc", &SVfit_phiUnc, &b_SVfit_phiUnc);
       fChain->SetBranchAddress("SVfit_fitMETRho", &SVfit_fitMETRho, &b_SVfit_fitMETRho);
       fChain->SetBranchAddress("SVfit_fitMETPhi", &SVfit_fitMETPhi, &b_SVfit_fitMETPhi);
       fChain->SetBranchAddress("isOSCand", &isOSCand, &b_isOSCand);
       fChain->SetBranchAddress("METx", &METx, &b_METx);
       fChain->SetBranchAddress("METy", &METy, &b_METy);
       fChain->SetBranchAddress("MET_cov00", &MET_cov00, &b_MET_cov00);
       fChain->SetBranchAddress("MET_cov01", &MET_cov01, &b_MET_cov01);
       fChain->SetBranchAddress("MET_cov10", &MET_cov10, &b_MET_cov10);
       fChain->SetBranchAddress("MET_cov11", &MET_cov11, &b_MET_cov11);
       fChain->SetBranchAddress("MET_significance", &MET_significance, &b_MET_significance);
       fChain->SetBranchAddress("mT_Dau1", &mT_Dau1, &b_mT_Dau1);
       fChain->SetBranchAddress("mT_Dau2", &mT_Dau2, &b_mT_Dau2);
       fChain->SetBranchAddress("PDGIdDaughters", &PDGIdDaughters, &b_PDGIdDaughters);
       fChain->SetBranchAddress("indexDau1", &indexDau1, &b_indexDau1);
       fChain->SetBranchAddress("indexDau2", &indexDau2, &b_indexDau2);
       fChain->SetBranchAddress("particleType", &particleType, &b_particleType);
       fChain->SetBranchAddress("discriminator", &discriminator, &b_discriminator);
       fChain->SetBranchAddress("daughters_muonID", &daughters_muonID, &b_daughters_muonID);
       fChain->SetBranchAddress("daughters_typeOfMuon", &daughters_typeOfMuon, &b_daughters_typeOfMuon);
       fChain->SetBranchAddress("dxy", &dxy, &b_dxy);
       fChain->SetBranchAddress("dz", &dz, &b_dz);
       fChain->SetBranchAddress("SIP", &SIP, &b_SIP);
       fChain->SetBranchAddress("daughters_iseleBDT", &daughters_iseleBDT, &b_daughters_iseleBDT);
       fChain->SetBranchAddress("daughters_eleCUTID", &daughters_eleCUTID, &b_daughters_eleCUTID);
       fChain->SetBranchAddress("decayMode", &decayMode, &b_decayMode);
       fChain->SetBranchAddress("combreliso", &combreliso, &b_combreliso);
       fChain->SetBranchAddress("daughters_IetaIeta", &daughters_IetaIeta, &b_daughters_IetaIeta);
       fChain->SetBranchAddress("daughters_deltaPhiSuperClusterTrackAtVtx", &daughters_deltaPhiSuperClusterTrackAtVtx, &b_daughters_deltaPhiSuperClusterTrackAtVtx);
       fChain->SetBranchAddress("daughters_SCeta", &daughters_SCeta, &b_daughters_SCeta);
       fChain->SetBranchAddress("daughters_depositR03_tracker", &daughters_depositR03_tracker, &b_daughters_depositR03_tracker);
       fChain->SetBranchAddress("daughters_depositR03_ecal", &daughters_depositR03_ecal, &b_daughters_depositR03_ecal);
       fChain->SetBranchAddress("daughters_depositR03_hcal", &daughters_depositR03_hcal, &b_daughters_depositR03_hcal);
       fChain->SetBranchAddress("daughters_decayModeFindingOldDMs", &daughters_decayModeFindingOldDMs, &b_daughters_decayModeFindingOldDMs);
       fChain->SetBranchAddress("daughters_decayModeFindingNewDMs", &daughters_decayModeFindingNewDMs, &b_daughters_decayModeFindingNewDMs);
       fChain->SetBranchAddress("tauID", &tauID, &b_tauID);
       fChain->SetBranchAddress("daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits", &daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits, &b_daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits);
       fChain->SetBranchAddress("daughters_byIsolationMVA3oldDMwoLTraw", &daughters_byIsolationMVA3oldDMwoLTraw, &b_daughters_byIsolationMVA3oldDMwoLTraw);
       fChain->SetBranchAddress("daughters_byIsolationMVA3oldDMwLTraw", &daughters_byIsolationMVA3oldDMwLTraw, &b_daughters_byIsolationMVA3oldDMwLTraw);
       fChain->SetBranchAddress("daughters_byIsolationMVA3newDMwoLTraw", &daughters_byIsolationMVA3newDMwoLTraw, &b_daughters_byIsolationMVA3newDMwoLTraw);
       fChain->SetBranchAddress("daughters_byIsolationMVA3newDMwLTraw", &daughters_byIsolationMVA3newDMwLTraw, &b_daughters_byIsolationMVA3newDMwLTraw);
       fChain->SetBranchAddress("daughters_chargedIsoPtSum", &daughters_chargedIsoPtSum, &b_daughters_chargedIsoPtSum);
       fChain->SetBranchAddress("daughters_neutralIsoPtSum", &daughters_neutralIsoPtSum, &b_daughters_neutralIsoPtSum);
       fChain->SetBranchAddress("daughters_puCorrPtSum", &daughters_puCorrPtSum, &b_daughters_puCorrPtSum);
       fChain->SetBranchAddress("photonPtSumOutsideSignalCone",&daughters_photonPtSumOutsideSignalCone, &b_daughters_photonPtSumOutsideSignalCone);
       fChain->SetBranchAddress("daughters_isLastTriggerObjectforPath", &daughters_isLastTriggerObjectforPath, &b_daughters_isLastTriggerObjectforPath);
       fChain->SetBranchAddress("daughters_isTriggerObjectforPath", &daughters_isTriggerObjectforPath, &b_daughters_isTriggerObjectforPath);
       fChain->SetBranchAddress("daughters_FilterFired", &daughters_FilterFired, &b_daughters_FilterFired);
       fChain->SetBranchAddress("daughters_isGoodTriggerType", &daughters_isGoodTriggerType, &b_daughters_isGoodTriggerType);
       fChain->SetBranchAddress("daughters_L3FilterFired", &daughters_L3FilterFired, &b_daughters_L3FilterFired);
       fChain->SetBranchAddress("daughters_L3FilterFiredLast", &daughters_L3FilterFiredLast, &b_daughters_L3FilterFiredLast);
       fChain->SetBranchAddress("JetsNumber", &JetsNumber, &b_JetsNumber);
       fChain->SetBranchAddress("jets_px", &jets_px, &b_jets_px);
       fChain->SetBranchAddress("jets_py", &jets_py, &b_jets_py);
       fChain->SetBranchAddress("jets_pz", &jets_pz, &b_jets_pz);
       fChain->SetBranchAddress("jets_e", &jets_e, &b_jets_e);
       fChain->SetBranchAddress("jets_Flavour", &jets_Flavour, &b_jets_Flavour);
       fChain->SetBranchAddress("jets_HadronFlavour", &jets_HadronFlavour, &b_jets_HadronFlavour);
       fChain->SetBranchAddress("jets_PUJetID", &jets_PUJetID, &b_jets_PUJetID);
       fChain->SetBranchAddress("bDiscriminator", &bDiscriminator, &b_bDiscriminator);
       fChain->SetBranchAddress("bCSVscore", &bCSVscore, &b_bCSVscore);
       fChain->SetBranchAddress("PFjetID", &PFjetID, &b_PFjetID);
       fChain->SetBranchAddress("jetRawf", &jetRawf, &b_jetRawf);
       
       // MC only
       if(fChain->GetListOfBranches()->FindObject("MC_weight"))
       {
            fChain->SetBranchAddress("aMCatNLOweight", &aMCatNLOweight, &b_aMCatNLOweight);
            fChain->SetBranchAddress("MC_weight", &MC_weight, &b_MC_weight);
            fChain->SetBranchAddress("daughters_genindex", &daughters_genindex, &b_daughters_genindex);
            fChain->SetBranchAddress("genpart_px", &genpart_px, &b_genpart_px);
            fChain->SetBranchAddress("genpart_py", &genpart_py, &b_genpart_py);
            fChain->SetBranchAddress("genpart_pz", &genpart_pz, &b_genpart_pz);
            fChain->SetBranchAddress("genpart_e", &genpart_e, &b_genpart_e);
            fChain->SetBranchAddress("genpart_pdg", &genpart_pdg, &b_genpart_pdg);
            fChain->SetBranchAddress("genpart_status", &genpart_status, &b_genpart_status);
            fChain->SetBranchAddress("genpart_HMothInd", &genpart_HMothInd, &b_genpart_HMothInd);
            fChain->SetBranchAddress("genpart_TopMothInd", &genpart_TopMothInd, &b_genpart_TopMothInd);
            fChain->SetBranchAddress("genpart_TauMothInd", &genpart_TauMothInd, &b_genpart_TauMothInd);
            fChain->SetBranchAddress("genpart_ZMothInd", &genpart_ZMothInd, &b_genpart_ZMothInd);
            fChain->SetBranchAddress("genpart_HZDecayMode", &genpart_HZDecayMode, &b_genpart_HZDecayMode);
            fChain->SetBranchAddress("genpart_TauGenDecayMode", &genpart_TauGenDecayMode, &b_genpart_TauGenDecayMode);
            fChain->SetBranchAddress("genpart_flags", &genpart_flags, &b_genpart_flags);
            fChain->SetBranchAddress("genjet_px", &genjet_px, &b_genjet_px);
            fChain->SetBranchAddress("genjet_py", &genjet_py, &b_genjet_py);
            fChain->SetBranchAddress("genjet_pz", &genjet_pz, &b_genjet_pz);
            fChain->SetBranchAddress("genjet_e", &genjet_e, &b_genjet_e);
            fChain->SetBranchAddress("jets_genjetIndex", &jets_genjetIndex, &b_jets_genjetIndex);
            fChain->SetBranchAddress("NUP", &NUP, &b_NUP);
       }
       
    }
};

#endif
