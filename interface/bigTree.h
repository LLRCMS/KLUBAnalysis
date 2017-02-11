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
   ULong64_t       EventNumber;
   Int_t           RunNumber;
   Int_t           lumi;
   Int_t           NBadMu;
   Long64_t        triggerbit;
   Int_t           metfilterbit;
   Float_t         met;
   Float_t         metphi;
   Int_t           npv;
   Float_t         npu;
   Float_t         PUNumInteractions;
   Float_t         PUReweight;
   Float_t         rho;
   std::vector<float>   *mothers_px;
   std::vector<float>   *mothers_py;
   std::vector<float>   *mothers_pz;
   std::vector<float>   *mothers_e;
   std::vector<Long64_t> *mothers_trgSeparateMatch;
   std::vector<float>   *daughters_px;
   std::vector<float>   *daughters_py;
   std::vector<float>   *daughters_pz;
   std::vector<float>   *daughters_e;
   std::vector<float>   *daughters_px_TauUp;
   std::vector<float>   *daughters_py_TauUp;
   std::vector<float>   *daughters_pz_TauUp;
   std::vector<float>   *daughters_e_TauUp;
   std::vector<float>   *daughters_px_TauDown;
   std::vector<float>   *daughters_py_TauDown;
   std::vector<float>   *daughters_pz_TauDown;
   std::vector<float>   *daughters_e_TauDown;
   std::vector<int>     *daughters_TauUpExists;
   std::vector<int>     *daughters_charge;
   std::vector<int>     *daughters_genindex;
   Float_t         MC_weight;
   Float_t         lheHt;
   Int_t           lheNOutPartons;
   Int_t           lheNOutB;
   Float_t         aMCatNLOweight;
   std::vector<float>   *genpart_px;
   std::vector<float>   *genpart_py;
   std::vector<float>   *genpart_pz;
   std::vector<float>   *genpart_e;
   std::vector<int>     *genpart_pdg;
   std::vector<int>     *genpart_status;
   std::vector<int>     *genpart_HMothInd;
   std::vector<int>     *genpart_MSSMHMothInd;
   std::vector<int>     *genpart_TopMothInd;
   std::vector<int>     *genpart_TauMothInd;
   std::vector<int>     *genpart_ZMothInd;
   std::vector<int>     *genpart_WMothInd;
   std::vector<int>     *genpart_bMothInd;
   std::vector<int>     *genpart_HZDecayMode;
   std::vector<int>     *genpart_TopDecayMode;
   std::vector<int>     *genpart_WDecayMode;
   std::vector<int>     *genpart_TauGenDecayMode;
   std::vector<int>     *genpart_flags;
   std::vector<float>   *genjet_px;
   std::vector<float>   *genjet_py;
   std::vector<float>   *genjet_pz;
   std::vector<float>   *genjet_e;
   std::vector<int>     *genjet_partonFlavour;
   std::vector<int>     *genjet_hadronFlavour;
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

   std::vector<float>   *SVfitMassTauUp;
   std::vector<float>   *SVfit_ptTauUp;
   std::vector<float>   *SVfit_ptUncTauUp;
   std::vector<float>   *SVfit_etaTauUp;
   std::vector<float>   *SVfit_etaUncTauUp;
   std::vector<float>   *SVfit_phiTauUp;
   std::vector<float>   *SVfit_phiUncTauUp;
   std::vector<float>   *SVfit_fitMETRhoTauUp;
   std::vector<float>   *SVfit_fitMETPhiTauUp;

   std::vector<float>   *SVfitMassTauDown;
   std::vector<float>   *SVfit_ptTauDown;
   std::vector<float>   *SVfit_ptUncTauDown;
   std::vector<float>   *SVfit_etaTauDown;
   std::vector<float>   *SVfit_etaUncTauDown;
   std::vector<float>   *SVfit_phiTauDown;
   std::vector<float>   *SVfit_phiUncTauDown;
   std::vector<float>   *SVfit_fitMETRhoTauDown;
   std::vector<float>   *SVfit_fitMETPhiTauDown;


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
   std::vector<bool>    *daughters_iseleWP80;
   std::vector<bool>    *daughters_iseleWP90;
   std::vector<float>   *daughters_eleMVAnt;
   std::vector<bool>    *daughters_passConversionVeto;
   std::vector<int>     *daughters_eleMissingHits;
   std::vector<int>     *daughters_eleCUTID;
   std::vector<int>     *decayMode;
   std::vector<Long64_t> *tauID;
   std::vector<float>   *combreliso;
   // std::vector<float>   *daughters_IetaIeta;
   // std::vector<float>   *daughters_deltaPhiSuperClusterTrackAtVtx;
   std::vector<float>   *daughters_SCeta;
   std::vector<float>   *daughters_depositR03_tracker;
   std::vector<float>   *daughters_depositR03_ecal;
   std::vector<float>   *daughters_depositR03_hcal;
   std::vector<int>     *daughters_decayModeFindingOldDMs;
   std::vector<float>   *againstElectronMVA5category;
   std::vector<float>   *againstElectronMVA5raw;
   std::vector<float>   *byPileupWeightedIsolationRaw3Hits;
   std::vector<float>   *footprintCorrection;
   std::vector<float>   *neutralIsoPtSumWeight;
   std::vector<float>   *photonPtSumOutsideSignalCone;
   std::vector<int>     *daughters_decayModeFindingNewDMs;
   std::vector<float>   *daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits;
   std::vector<float>   *daughters_byIsolationMVArun2v1DBoldDMwLTraw;
   std::vector<float>   *daughters_byIsolationMVA3oldDMwoLTraw;
   std::vector<float>   *daughters_byIsolationMVA3oldDMwLTraw;
   std::vector<float>   *daughters_byIsolationMVA3newDMwoLTraw;
   std::vector<float>   *daughters_byIsolationMVA3newDMwLTraw;
   std::vector<float>   *daughters_chargedIsoPtSum;
   std::vector<float>   *daughters_neutralIsoPtSum;
   std::vector<float>   *daughters_puCorrPtSum;
   std::vector<int>     *daughters_numChargedParticlesSignalCone;
   std::vector<int>     *daughters_numNeutralHadronsSignalCone;
   std::vector<int>     *daughters_numPhotonsSignalCone;
   std::vector<int>     *daughters_daughters_numParticlesSignalCone;
   std::vector<int>     *daughters_numChargedParticlesIsoCone;
   std::vector<int>     *daughters_numNeutralHadronsIsoCone;
   std::vector<int>     *daughters_numPhotonsIsoCone;
   std::vector<int>     *daughters_numParticlesIsoCone;
   std::vector<float>   *daughters_leadChargedParticlePt;
   std::vector<float>   *daughters_trackRefPt;
   std::vector<int>     *daughters_isLastTriggerObjectforPath;
   std::vector<int>     *daughters_isTriggerObjectforPath;
   std::vector<Long64_t> *daughters_trgMatched;
   std::vector<Long64_t> *daughters_FilterFired;
   std::vector<Long64_t> *daughters_isGoodTriggerType;
   std::vector<Long64_t> *daughters_L3FilterFired;
   std::vector<Long64_t> *daughters_L3FilterFiredLast;
   std::vector<float>   *daughters_HLTpt;
   std::vector<bool>    *daughters_isL1IsoTau28Matched;
   Int_t           JetsNumber;
   std::vector<float>   *jets_px;
   std::vector<float>   *jets_py;
   std::vector<float>   *jets_pz;
   std::vector<float>   *jets_e;
   std::vector<float>   *jets_rawPt;
   std::vector<float>   *jets_mT;
   std::vector<int>     *jets_Flavour;
   std::vector<int>     *jets_HadronFlavour;
   std::vector<int>     *jets_genjetIndex;
   std::vector<float>   *jets_PUJetID;
   std::vector<float>   *jets_vtxPt;
   std::vector<float>   *jets_vtxMass;
   std::vector<float>   *jets_vtx3dL;
   std::vector<float>   *jets_vtxNtrk;
   std::vector<float>   *jets_vtx3deL;
   std::vector<float>   *jets_leadTrackPt;
   std::vector<float>   *jets_leptonPtRel;
   std::vector<float>   *jets_leptonPt;
   std::vector<float>   *jets_leptonDeltaR;
   std::vector<float>   *jets_chEmEF;
   std::vector<float>   *jets_chHEF;
   std::vector<float>   *jets_nEmEF;
   std::vector<float>   *jets_nHEF;
   std::vector<int>     *jets_chMult;
   std::vector<float>   *bDiscriminator;
   std::vector<float>   *bCSVscore;
   std::vector<int>     *PFjetID;
   std::vector<float>   *jetRawf;
   std::vector<float>   *jets_jecUnc;

   std::vector<float>   *ak8jets_px;
   std::vector<float>   *ak8jets_py;
   std::vector<float>   *ak8jets_pz;
   std::vector<float>   *ak8jets_e;
   std::vector<float>   *ak8jets_SoftDropMass;
   std::vector<float>   *ak8jets_PrunedMass;
   std::vector<float>   *ak8jets_TrimmedMass;
   std::vector<float>   *ak8jets_FilteredMass;
   std::vector<float>   *ak8jets_tau1;
   std::vector<float>   *ak8jets_tau2;
   std::vector<float>   *ak8jets_tau3;
   std::vector<float>   *ak8jets_CSV;
   std::vector<int>     *ak8jets_nsubjets;     
   std::vector<float>   *subjets_px;
   std::vector<float>   *subjets_py;
   std::vector<float>   *subjets_pz;
   std::vector<float>   *subjets_e;
   std::vector<float>   *subjets_CSV;
   std::vector<int>     *subjets_ak8MotherIdx;


   // List of branches
   TBranch        *b_EventNumber;   //!
   TBranch        *b_RunNumber;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_NBadMu; //!
   TBranch        *b_triggerbit;   //!
   TBranch        *b_metfilterbit;   //!
   TBranch        *b_met;   //!
   TBranch        *b_metphi;   //!
   TBranch        *b_npv;   //!
   TBranch        *b_npu;   //!
   TBranch        *b_PUNumInteractions;   //!
   TBranch        *b_PUReweight;   //!
   TBranch        *b_rho;   //!
   TBranch        *b_mothers_px;   //!
   TBranch        *b_mothers_py;   //!
   TBranch        *b_mothers_pz;   //!
   TBranch        *b_mothers_e;   //!
   TBranch        *b_mothers_trgSeparateMatch; //!
   TBranch        *b_daughters_px;   //!
   TBranch        *b_daughters_py;   //!
   TBranch        *b_daughters_pz;   //!
   TBranch        *b_daughters_e;   //!
   TBranch        *b_daughters_px_TauUp;   //!
   TBranch        *b_daughters_py_TauUp;   //!
   TBranch        *b_daughters_pz_TauUp;   //!
   TBranch        *b_daughters_e_TauUp;   //!
   TBranch        *b_daughters_px_TauDown;   //!
   TBranch        *b_daughters_py_TauDown;   //!
   TBranch        *b_daughters_pz_TauDown;   //!
   TBranch        *b_daughters_e_TauDown;   //!
   TBranch        *b_daughters_TauUpExists;   //!
   TBranch        *b_daughters_charge;   //!
   TBranch        *b_daughters_genindex;   //!
   TBranch        *b_MC_weight;   //!
   TBranch        *b_lheHt;   //!
   TBranch        *b_lheNOutPartons; //!
   TBranch        *b_lheNOutB; //!
   TBranch        *b_aMCatNLOweight;   //!
   TBranch        *b_genpart_px;   //!
   TBranch        *b_genpart_py;   //!
   TBranch        *b_genpart_pz;   //!
   TBranch        *b_genpart_e;   //!
   TBranch        *b_genpart_pdg;   //!
   TBranch        *b_genpart_status;   //!
   TBranch        *b_genpart_HMothInd;   //!
   TBranch        *b_genpart_MSSMHMothInd;   //!
   TBranch        *b_genpart_TopMothInd;   //!
   TBranch        *b_genpart_TauMothInd;   //!
   TBranch        *b_genpart_ZMothInd;   //!
   TBranch        *b_genpart_WMothInd;   //!
   TBranch        *b_genpart_bMothInd;   //!
   TBranch        *b_genpart_HZDecayMode;   //!
   TBranch        *b_genpart_TopDecayMode;   //!
   TBranch        *b_genpart_WDecayMode;   //!
   TBranch        *b_genpart_TauGenDecayMode;   //!
   TBranch        *b_genpart_flags;   //!
   TBranch        *b_genjet_px;   //!
   TBranch        *b_genjet_py;   //!
   TBranch        *b_genjet_pz;   //!
   TBranch        *b_genjet_e;   //!
   TBranch        *b_genjet_partonFlavour;   //!
   TBranch        *b_genjet_hadronFlavour;   //!
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
   TBranch        *b_SVfitMassTauUp;   //!
   TBranch        *b_SVfit_ptTauUp;   //!
   TBranch        *b_SVfit_ptUncTauUp;   //!
   TBranch        *b_SVfit_etaTauUp;   //!
   TBranch        *b_SVfit_etaUncTauUp;   //!
   TBranch        *b_SVfit_phiTauUp;   //!
   TBranch        *b_SVfit_phiUncTauUp;   //!
   TBranch        *b_SVfit_fitMETRhoTauUp;   //!
   TBranch        *b_SVfit_fitMETPhiTauUp;   //!
   TBranch        *b_SVfitMassTauDown;   //!
   TBranch        *b_SVfit_ptTauDown;   //!
   TBranch        *b_SVfit_ptUncTauDown;   //!
   TBranch        *b_SVfit_etaTauDown;   //!
   TBranch        *b_SVfit_etaUncTauDown;   //!
   TBranch        *b_SVfit_phiTauDown;   //!
   TBranch        *b_SVfit_phiUncTauDown;   //!
   TBranch        *b_SVfit_fitMETRhoTauDown;   //!
   TBranch        *b_SVfit_fitMETPhiTauDown;   //!
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
   TBranch        *b_daughters_iseleWP80;   //!
   TBranch        *b_daughters_iseleWP90;   //!
   TBranch        *b_daughters_eleMVAnt;   //!
   TBranch        *b_daughters_passConversionVeto;   //!
   TBranch        *b_daughters_eleMissingHits;   //!
   TBranch        *b_daughters_eleCUTID;   //!
   TBranch        *b_decayMode;   //!
   TBranch        *b_tauID;   //!
   TBranch        *b_combreliso;   //!
   // TBranch        *b_daughters_IetaIeta;   //!
   // TBranch        *b_daughters_deltaPhiSuperClusterTrackAtVtx;   //!
   TBranch        *b_daughters_SCeta;   //!
   TBranch        *b_daughters_depositR03_tracker;   //!
   TBranch        *b_daughters_depositR03_ecal;   //!
   TBranch        *b_daughters_depositR03_hcal;   //!
   TBranch        *b_daughters_decayModeFindingOldDMs;   //!
   TBranch        *b_againstElectronMVA5category;   //!
   TBranch        *b_againstElectronMVA5raw;   //!
   TBranch        *b_byPileupWeightedIsolationRaw3Hits;   //!
   TBranch        *b_footprintCorrection;   //!
   TBranch        *b_neutralIsoPtSumWeight;   //!
   TBranch        *b_photonPtSumOutsideSignalCone;   //!
   TBranch        *b_daughters_decayModeFindingNewDMs;   //!
   TBranch        *b_daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits;   //!
   TBranch        *b_daughters_byIsolationMVArun2v1DBoldDMwLTraw; //!
   TBranch        *b_daughters_byIsolationMVA3oldDMwoLTraw;   //!
   TBranch        *b_daughters_byIsolationMVA3oldDMwLTraw;   //!
   TBranch        *b_daughters_byIsolationMVA3newDMwoLTraw;   //!
   TBranch        *b_daughters_byIsolationMVA3newDMwLTraw;   //!
   TBranch        *b_daughters_chargedIsoPtSum;   //!
   TBranch        *b_daughters_neutralIsoPtSum;   //!
   TBranch        *b_daughters_puCorrPtSum;   //!
   TBranch        *b_daughters_numChargedParticlesSignalCone;   //!
   TBranch        *b_daughters_numNeutralHadronsSignalCone;   //!
   TBranch        *b_daughters_numPhotonsSignalCone;   //!
   TBranch        *b_daughters_daughters_numParticlesSignalCone;   //!
   TBranch        *b_daughters_numChargedParticlesIsoCone;   //!
   TBranch        *b_daughters_numNeutralHadronsIsoCone;   //!
   TBranch        *b_daughters_numPhotonsIsoCone;   //!
   TBranch        *b_daughters_numParticlesIsoCone;   //!
   TBranch        *b_daughters_leadChargedParticlePt;   //!
   TBranch        *b_daughters_trackRefPt;   //!
   TBranch        *b_daughters_isLastTriggerObjectforPath;   //!
   TBranch        *b_daughters_isTriggerObjectforPath;   //!
   TBranch        *b_daughters_trgMatched;   //!
   TBranch        *b_daughters_FilterFired;   //!
   TBranch        *b_daughters_isGoodTriggerType;   //!
   TBranch        *b_daughters_L3FilterFired;   //!
   TBranch        *b_daughters_L3FilterFiredLast;   //!
   TBranch        *b_daughters_HLTpt;   //!
   TBranch        *b_daughters_isL1IsoTau28Matched; //!
   TBranch        *b_JetsNumber;   //!
   TBranch        *b_jets_px;   //!
   TBranch        *b_jets_py;   //!
   TBranch        *b_jets_pz;   //!
   TBranch        *b_jets_e;   //!
   TBranch        *b_jets_rawPt;   //!
   TBranch        *b_jets_mT;   //!
   TBranch        *b_jets_Flavour;   //!
   TBranch        *b_jets_HadronFlavour;   //!
   TBranch        *b_jets_genjetIndex;   //!
   TBranch        *b_jets_PUJetID;   //!
   TBranch        *b_jets_vtxPt;   //!
   TBranch        *b_jets_vtxMass;   //!
   TBranch        *b_jets_vtx3dL;   //!
   TBranch        *b_jets_vtxNtrk;   //!
   TBranch        *b_jets_vtx3deL;   //!
   TBranch        *b_jets_leadTrackPt;   //!
   TBranch        *b_jets_leptonPtRel;   //!
   TBranch        *b_jets_leptonPt;   //!
   TBranch        *b_jets_leptonDeltaR;   //!
   TBranch        *b_jets_chEmEF;   //!
   TBranch        *b_jets_chHEF;   //!
   TBranch        *b_jets_nEmEF;   //!
   TBranch        *b_jets_nHEF;   //!
   TBranch        *b_jets_chMult;   //!
   TBranch        *b_bDiscriminator;   //!
   TBranch        *b_bCSVscore;   //!
   TBranch        *b_PFjetID;   //!
   TBranch        *b_jetRawf;   //!
   TBranch        *b_jets_jecUnc ; //!
   TBranch        *b_ak8jets_px;
   TBranch        *b_ak8jets_py;
   TBranch        *b_ak8jets_pz;
   TBranch        *b_ak8jets_e;
   TBranch        *b_ak8jets_SoftDropMass;
   TBranch        *b_ak8jets_PrunedMass;
   TBranch        *b_ak8jets_TrimmedMass;
   TBranch        *b_ak8jets_FilteredMass;
   TBranch        *b_ak8jets_tau1;
   TBranch        *b_ak8jets_tau2;
   TBranch        *b_ak8jets_tau3;
   TBranch        *b_ak8jets_CSV;
   TBranch        *b_ak8jets_nsubjets;
   TBranch        *b_subjets_px;
   TBranch        *b_subjets_py;
   TBranch        *b_subjets_pz;
   TBranch        *b_subjets_e;
   TBranch        *b_subjets_CSV;
   TBranch        *b_subjets_ak8MotherIdx;


   bigTree (TChain * inputChain) : fChain (inputChain) { Init(fChain);}
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
       mothers_trgSeparateMatch = 0;
       daughters_px = 0;
       daughters_py = 0;
       daughters_pz = 0;
       daughters_e = 0;
       daughters_px_TauUp = 0;
       daughters_py_TauUp = 0;
       daughters_pz_TauUp = 0;
       daughters_e_TauUp = 0;
       daughters_px_TauDown = 0;
       daughters_py_TauDown = 0;
       daughters_pz_TauDown = 0;
       daughters_e_TauDown = 0;
       daughters_TauUpExists = 0;
       daughters_charge = 0;
       daughters_genindex = 0;
       genpart_px = 0;
       genpart_py = 0;
       genpart_pz = 0;
       genpart_e = 0;
       genpart_pdg = 0;
       genpart_status = 0;
       genpart_HMothInd = 0;
       genpart_MSSMHMothInd = 0;
       genpart_TopMothInd = 0;
       genpart_TauMothInd = 0;
       genpart_ZMothInd = 0;
       genpart_WMothInd = 0;
       genpart_bMothInd = 0;
       genpart_HZDecayMode = 0;
       genpart_TopDecayMode = 0;
       genpart_WDecayMode = 0;
       genpart_TauGenDecayMode = 0;
       genpart_flags = 0;
       genjet_px = 0;
       genjet_py = 0;
       genjet_pz = 0;
       genjet_e = 0;
       genjet_partonFlavour = 0;
       genjet_hadronFlavour = 0;
       SVfitMass = 0;
       SVfit_pt = 0;
       SVfit_ptUnc = 0;
       SVfit_eta = 0;
       SVfit_etaUnc = 0;
       SVfit_phi = 0;
       SVfit_phiUnc = 0;
       SVfit_fitMETRho = 0;
       SVfit_fitMETPhi = 0;
       SVfitMassTauUp = 0;
       SVfit_ptTauUp = 0;
       SVfit_ptUncTauUp = 0;
       SVfit_etaTauUp = 0;
       SVfit_etaUncTauUp = 0;
       SVfit_phiTauUp = 0;
       SVfit_phiUncTauUp = 0;
       SVfit_fitMETRhoTauUp = 0;
       SVfit_fitMETPhiTauUp = 0;
       SVfitMassTauDown = 0;
       SVfit_ptTauDown = 0;
       SVfit_ptUncTauDown = 0;
       SVfit_etaTauDown = 0;
       SVfit_etaUncTauDown = 0;
       SVfit_phiTauDown = 0;
       SVfit_phiUncTauDown = 0;
       SVfit_fitMETRhoTauDown = 0;
       SVfit_fitMETPhiTauDown = 0;
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
       daughters_iseleWP80 = 0;
       daughters_iseleWP90 = 0;
       daughters_eleMVAnt = 0;
       daughters_passConversionVeto = 0;
       daughters_eleMissingHits = 0;
       daughters_eleCUTID = 0;
       decayMode = 0;
       tauID = 0;
       combreliso = 0;
       // daughters_IetaIeta = 0;
       // daughters_deltaPhiSuperClusterTrackAtVtx = 0;
       daughters_SCeta = 0;
       daughters_depositR03_tracker = 0;
       daughters_depositR03_ecal = 0;
       daughters_depositR03_hcal = 0;
       daughters_decayModeFindingOldDMs = 0;
       againstElectronMVA5category = 0;
       againstElectronMVA5raw = 0;
       byPileupWeightedIsolationRaw3Hits = 0;
       footprintCorrection = 0;
       neutralIsoPtSumWeight = 0;
       photonPtSumOutsideSignalCone = 0;
       daughters_decayModeFindingNewDMs = 0;
       daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits = 0;
       daughters_byIsolationMVArun2v1DBoldDMwLTraw = 0;
       daughters_byIsolationMVA3oldDMwoLTraw = 0;
       daughters_byIsolationMVA3oldDMwLTraw = 0;
       daughters_byIsolationMVA3newDMwoLTraw = 0;
       daughters_byIsolationMVA3newDMwLTraw = 0;
       daughters_chargedIsoPtSum = 0;
       daughters_neutralIsoPtSum = 0;
       daughters_puCorrPtSum = 0;
       daughters_numChargedParticlesSignalCone = 0;
       daughters_numNeutralHadronsSignalCone = 0;
       daughters_numPhotonsSignalCone = 0;
       daughters_daughters_numParticlesSignalCone = 0;
       daughters_numChargedParticlesIsoCone = 0;
       daughters_numNeutralHadronsIsoCone = 0;
       daughters_numPhotonsIsoCone = 0;
       daughters_numParticlesIsoCone = 0;
       daughters_leadChargedParticlePt = 0;
       daughters_trackRefPt = 0;
       daughters_isLastTriggerObjectforPath = 0;
       daughters_isTriggerObjectforPath = 0;
       daughters_trgMatched = 0;
       daughters_FilterFired = 0;
       daughters_isGoodTriggerType = 0;
       daughters_L3FilterFired = 0;
       daughters_L3FilterFiredLast = 0;
       daughters_HLTpt = 0;
       daughters_isL1IsoTau28Matched = 0;
       jets_px = 0;
       jets_py = 0;
       jets_pz = 0;
       jets_e = 0;
       jets_rawPt = 0;
       jets_mT = 0;
       jets_Flavour = 0;
       jets_HadronFlavour = 0;
       jets_genjetIndex = 0;
       jets_PUJetID = 0;
       jets_vtxPt = 0;
       jets_vtxMass = 0;
       jets_vtx3dL = 0;
       jets_vtxNtrk = 0;
       jets_vtx3deL = 0;
       jets_leadTrackPt = 0;
       jets_leptonPtRel = 0;
       jets_leptonPt = 0;
       jets_leptonDeltaR = 0;
       jets_chEmEF = 0;
       jets_chHEF = 0;
       jets_nEmEF = 0;
       jets_nHEF = 0;
       jets_chMult = 0;
       bDiscriminator = 0;
       bCSVscore = 0;
       PFjetID = 0;
       jetRawf = 0;
       jets_jecUnc = 0;
       ak8jets_px = 0;
       ak8jets_py = 0;
       ak8jets_pz = 0;
       ak8jets_e = 0;
       ak8jets_SoftDropMass = 0;
       ak8jets_PrunedMass = 0;
       ak8jets_TrimmedMass = 0;
       ak8jets_FilteredMass = 0;
       ak8jets_tau1 = 0;
       ak8jets_tau2 = 0;
       ak8jets_tau3 = 0;
       ak8jets_CSV = 0;
       ak8jets_nsubjets = 0;
       subjets_px = 0;
       subjets_py = 0;
       subjets_pz = 0;
       subjets_e = 0;
       subjets_CSV = 0;
       subjets_ak8MotherIdx = 0;

       fChain->SetMakeClass(1);

       fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
       fChain->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber);
       fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
       fChain->SetBranchAddress("NBadMu", &NBadMu, &b_NBadMu);
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
       fChain->SetBranchAddress("mothers_trgSeparateMatch", &mothers_trgSeparateMatch, &b_mothers_trgSeparateMatch);
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
       fChain->SetBranchAddress("daughters_iseleWP80", &daughters_iseleWP80, &b_daughters_iseleWP80);
       fChain->SetBranchAddress("daughters_iseleWP90", &daughters_iseleWP90, &b_daughters_iseleWP90);
       fChain->SetBranchAddress("daughters_eleMVAnt", &daughters_eleMVAnt, &b_daughters_eleMVAnt);
       fChain->SetBranchAddress("daughters_passConversionVeto", &daughters_passConversionVeto, &b_daughters_passConversionVeto);
       fChain->SetBranchAddress("daughters_eleMissingHits", &daughters_eleMissingHits, &b_daughters_eleMissingHits);
       fChain->SetBranchAddress("daughters_eleCUTID", &daughters_eleCUTID, &b_daughters_eleCUTID);
       fChain->SetBranchAddress("decayMode", &decayMode, &b_decayMode);
       fChain->SetBranchAddress("tauID", &tauID, &b_tauID);
       fChain->SetBranchAddress("combreliso", &combreliso, &b_combreliso);
       // fChain->SetBranchAddress("daughters_IetaIeta", &daughters_IetaIeta, &b_daughters_IetaIeta);
       // fChain->SetBranchAddress("daughters_deltaPhiSuperClusterTrackAtVtx", &daughters_deltaPhiSuperClusterTrackAtVtx, &b_daughters_deltaPhiSuperClusterTrackAtVtx);
       fChain->SetBranchAddress("daughters_SCeta", &daughters_SCeta, &b_daughters_SCeta);
       fChain->SetBranchAddress("daughters_depositR03_tracker", &daughters_depositR03_tracker, &b_daughters_depositR03_tracker);
       fChain->SetBranchAddress("daughters_depositR03_ecal", &daughters_depositR03_ecal, &b_daughters_depositR03_ecal);
       fChain->SetBranchAddress("daughters_depositR03_hcal", &daughters_depositR03_hcal, &b_daughters_depositR03_hcal);
       fChain->SetBranchAddress("daughters_decayModeFindingOldDMs", &daughters_decayModeFindingOldDMs, &b_daughters_decayModeFindingOldDMs);
       fChain->SetBranchAddress("againstElectronMVA5category", &againstElectronMVA5category, &b_againstElectronMVA5category);
       fChain->SetBranchAddress("againstElectronMVA5raw", &againstElectronMVA5raw, &b_againstElectronMVA5raw);
       fChain->SetBranchAddress("byPileupWeightedIsolationRaw3Hits", &byPileupWeightedIsolationRaw3Hits, &b_byPileupWeightedIsolationRaw3Hits);
       fChain->SetBranchAddress("footprintCorrection", &footprintCorrection, &b_footprintCorrection);
       fChain->SetBranchAddress("neutralIsoPtSumWeight", &neutralIsoPtSumWeight, &b_neutralIsoPtSumWeight);
       fChain->SetBranchAddress("photonPtSumOutsideSignalCone", &photonPtSumOutsideSignalCone, &b_photonPtSumOutsideSignalCone);
       fChain->SetBranchAddress("daughters_decayModeFindingNewDMs", &daughters_decayModeFindingNewDMs, &b_daughters_decayModeFindingNewDMs);
       fChain->SetBranchAddress("daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits", &daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits, &b_daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits);
       fChain->SetBranchAddress("daughters_byIsolationMVArun2v1DBoldDMwLTraw", &daughters_byIsolationMVArun2v1DBoldDMwLTraw, &b_daughters_byIsolationMVArun2v1DBoldDMwLTraw);
       fChain->SetBranchAddress("daughters_byIsolationMVA3oldDMwoLTraw", &daughters_byIsolationMVA3oldDMwoLTraw, &b_daughters_byIsolationMVA3oldDMwoLTraw);
       fChain->SetBranchAddress("daughters_byIsolationMVA3oldDMwLTraw", &daughters_byIsolationMVA3oldDMwLTraw, &b_daughters_byIsolationMVA3oldDMwLTraw);
       fChain->SetBranchAddress("daughters_byIsolationMVA3newDMwoLTraw", &daughters_byIsolationMVA3newDMwoLTraw, &b_daughters_byIsolationMVA3newDMwoLTraw);
       fChain->SetBranchAddress("daughters_byIsolationMVA3newDMwLTraw", &daughters_byIsolationMVA3newDMwLTraw, &b_daughters_byIsolationMVA3newDMwLTraw);
       fChain->SetBranchAddress("daughters_chargedIsoPtSum", &daughters_chargedIsoPtSum, &b_daughters_chargedIsoPtSum);
       fChain->SetBranchAddress("daughters_neutralIsoPtSum", &daughters_neutralIsoPtSum, &b_daughters_neutralIsoPtSum);
       fChain->SetBranchAddress("daughters_puCorrPtSum", &daughters_puCorrPtSum, &b_daughters_puCorrPtSum);
       fChain->SetBranchAddress("daughters_numChargedParticlesSignalCone", &daughters_numChargedParticlesSignalCone, &b_daughters_numChargedParticlesSignalCone);
       fChain->SetBranchAddress("daughters_numNeutralHadronsSignalCone", &daughters_numNeutralHadronsSignalCone, &b_daughters_numNeutralHadronsSignalCone);
       fChain->SetBranchAddress("daughters_numPhotonsSignalCone", &daughters_numPhotonsSignalCone, &b_daughters_numPhotonsSignalCone);
       fChain->SetBranchAddress("daughters_daughters_numParticlesSignalCone", &daughters_daughters_numParticlesSignalCone, &b_daughters_daughters_numParticlesSignalCone);
       fChain->SetBranchAddress("daughters_numChargedParticlesIsoCone", &daughters_numChargedParticlesIsoCone, &b_daughters_numChargedParticlesIsoCone);
       fChain->SetBranchAddress("daughters_numNeutralHadronsIsoCone", &daughters_numNeutralHadronsIsoCone, &b_daughters_numNeutralHadronsIsoCone);
       fChain->SetBranchAddress("daughters_numPhotonsIsoCone", &daughters_numPhotonsIsoCone, &b_daughters_numPhotonsIsoCone);
       fChain->SetBranchAddress("daughters_numParticlesIsoCone", &daughters_numParticlesIsoCone, &b_daughters_numParticlesIsoCone);
       fChain->SetBranchAddress("daughters_leadChargedParticlePt", &daughters_leadChargedParticlePt, &b_daughters_leadChargedParticlePt);
       fChain->SetBranchAddress("daughters_trackRefPt", &daughters_trackRefPt, &b_daughters_trackRefPt);
       fChain->SetBranchAddress("daughters_isLastTriggerObjectforPath", &daughters_isLastTriggerObjectforPath, &b_daughters_isLastTriggerObjectforPath);
       fChain->SetBranchAddress("daughters_isTriggerObjectforPath", &daughters_isTriggerObjectforPath, &b_daughters_isTriggerObjectforPath);
       fChain->SetBranchAddress("daughters_trgMatched", &daughters_trgMatched, &b_daughters_trgMatched);
       fChain->SetBranchAddress("daughters_FilterFired", &daughters_FilterFired, &b_daughters_FilterFired);
       fChain->SetBranchAddress("daughters_isGoodTriggerType", &daughters_isGoodTriggerType, &b_daughters_isGoodTriggerType);
       fChain->SetBranchAddress("daughters_L3FilterFired", &daughters_L3FilterFired, &b_daughters_L3FilterFired);
       fChain->SetBranchAddress("daughters_L3FilterFiredLast", &daughters_L3FilterFiredLast, &b_daughters_L3FilterFiredLast);
       fChain->SetBranchAddress("daughters_HLTpt", &daughters_HLTpt, &b_daughters_HLTpt);
       fChain->SetBranchAddress("daughters_isL1IsoTau28Matched", &daughters_isL1IsoTau28Matched, &b_daughters_isL1IsoTau28Matched);
       fChain->SetBranchAddress("JetsNumber", &JetsNumber, &b_JetsNumber);
       fChain->SetBranchAddress("jets_px", &jets_px, &b_jets_px);
       fChain->SetBranchAddress("jets_py", &jets_py, &b_jets_py);
       fChain->SetBranchAddress("jets_pz", &jets_pz, &b_jets_pz);
       fChain->SetBranchAddress("jets_e", &jets_e, &b_jets_e);
       fChain->SetBranchAddress("jets_rawPt", &jets_rawPt, &b_jets_rawPt);
       fChain->SetBranchAddress("jets_mT", &jets_mT, &b_jets_mT);
       fChain->SetBranchAddress("jets_Flavour", &jets_Flavour, &b_jets_Flavour);
       fChain->SetBranchAddress("jets_HadronFlavour", &jets_HadronFlavour, &b_jets_HadronFlavour);
       fChain->SetBranchAddress("jets_PUJetID", &jets_PUJetID, &b_jets_PUJetID);
       fChain->SetBranchAddress("jets_vtxPt", &jets_vtxPt, &b_jets_vtxPt);
       fChain->SetBranchAddress("jets_vtxMass", &jets_vtxMass, &b_jets_vtxMass);
       fChain->SetBranchAddress("jets_vtx3dL", &jets_vtx3dL, &b_jets_vtx3dL);
       fChain->SetBranchAddress("jets_vtxNtrk", &jets_vtxNtrk, &b_jets_vtxNtrk);
       fChain->SetBranchAddress("jets_vtx3deL", &jets_vtx3deL, &b_jets_vtx3deL);
       fChain->SetBranchAddress("jets_leadTrackPt", &jets_leadTrackPt, &b_jets_leadTrackPt);
       fChain->SetBranchAddress("jets_leptonPtRel", &jets_leptonPtRel, &b_jets_leptonPtRel);
       fChain->SetBranchAddress("jets_leptonPt", &jets_leptonPt, &b_jets_leptonPt);
       fChain->SetBranchAddress("jets_leptonDeltaR", &jets_leptonDeltaR, &b_jets_leptonDeltaR);
       fChain->SetBranchAddress("jets_chEmEF", &jets_chEmEF, &b_jets_chEmEF);
       fChain->SetBranchAddress("jets_chHEF", &jets_chHEF, &b_jets_chHEF);
       fChain->SetBranchAddress("jets_nEmEF", &jets_nEmEF, &b_jets_nEmEF);
       fChain->SetBranchAddress("jets_nHEF", &jets_nHEF, &b_jets_nHEF);
       fChain->SetBranchAddress("jets_chMult", &jets_chMult, &b_jets_chMult);
       fChain->SetBranchAddress("bDiscriminator", &bDiscriminator, &b_bDiscriminator);
       fChain->SetBranchAddress("bCSVscore", &bCSVscore, &b_bCSVscore);
       fChain->SetBranchAddress("PFjetID", &PFjetID, &b_PFjetID);
       fChain->SetBranchAddress("jetRawf", &jetRawf, &b_jetRawf);
       fChain->SetBranchAddress("jets_jecUnc", &jets_jecUnc,&b_jets_jecUnc);
       fChain->SetBranchAddress("ak8jets_px", &ak8jets_px, &b_ak8jets_px);
       fChain->SetBranchAddress("ak8jets_py", &ak8jets_py, &b_ak8jets_py);
       fChain->SetBranchAddress("ak8jets_pz", &ak8jets_pz, &b_ak8jets_pz);
       fChain->SetBranchAddress("ak8jets_e", &ak8jets_e, &b_ak8jets_e);
       fChain->SetBranchAddress("ak8jets_SoftDropMass", &ak8jets_SoftDropMass, &b_ak8jets_SoftDropMass);
       fChain->SetBranchAddress("ak8jets_PrunedMass", &ak8jets_PrunedMass, &b_ak8jets_PrunedMass);
       fChain->SetBranchAddress("ak8jets_TrimmedMass", &ak8jets_TrimmedMass, &b_ak8jets_TrimmedMass);
       fChain->SetBranchAddress("ak8jets_FilteredMass", &ak8jets_FilteredMass, &b_ak8jets_FilteredMass);
       fChain->SetBranchAddress("ak8jets_tau1", &ak8jets_tau1, &b_ak8jets_tau1);
       fChain->SetBranchAddress("ak8jets_tau2", &ak8jets_tau2, &b_ak8jets_tau2);
       fChain->SetBranchAddress("ak8jets_tau3", &ak8jets_tau3, &b_ak8jets_tau3);
       fChain->SetBranchAddress("ak8jets_CSV", &ak8jets_CSV, &b_ak8jets_CSV);
       fChain->SetBranchAddress("ak8jets_nsubjets", &ak8jets_nsubjets, &b_ak8jets_nsubjets);
       fChain->SetBranchAddress("subjets_px", &subjets_px, &b_subjets_px);
       fChain->SetBranchAddress("subjets_py", &subjets_py, &b_subjets_py);
       fChain->SetBranchAddress("subjets_pz", &subjets_pz, &b_subjets_pz);
       fChain->SetBranchAddress("subjets_e", &subjets_e, &b_subjets_e);
       fChain->SetBranchAddress("subjets_CSV", &subjets_CSV, &b_subjets_CSV);
       fChain->SetBranchAddress("subjets_ak8MotherIdx", &subjets_ak8MotherIdx, &b_subjets_ak8MotherIdx);

       // MC only
       if(fChain->GetListOfBranches()->FindObject("MC_weight"))
       {
            fChain->SetBranchAddress("PUNumInteractions", &PUNumInteractions, &b_PUNumInteractions);
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
            fChain->SetBranchAddress("genpart_TopDecayMode", &genpart_TopDecayMode, &b_genpart_TopDecayMode);
            fChain->SetBranchAddress("genpart_flags", &genpart_flags, &b_genpart_flags);
            fChain->SetBranchAddress("genjet_px", &genjet_px, &b_genjet_px);
            fChain->SetBranchAddress("genjet_py", &genjet_py, &b_genjet_py);
            fChain->SetBranchAddress("genjet_pz", &genjet_pz, &b_genjet_pz);
            fChain->SetBranchAddress("genjet_e", &genjet_e, &b_genjet_e);
            fChain->SetBranchAddress("genjet_hadronFlavour", &genjet_hadronFlavour, &b_genjet_hadronFlavour);
            fChain->SetBranchAddress("jets_genjetIndex", &jets_genjetIndex, &b_jets_genjetIndex);
            fChain->SetBranchAddress("NUP", &NUP, &b_NUP);
            fChain->SetBranchAddress("lheHt", &lheHt, &b_lheHt);
            fChain->SetBranchAddress("lheNOutPartons", &lheNOutPartons, &b_lheNOutPartons);
            fChain->SetBranchAddress("lheNOutB", &lheNOutB, &b_lheNOutB);
            fChain->SetBranchAddress("daughters_px_TauUp", &daughters_px_TauUp, &b_daughters_px_TauUp);
            fChain->SetBranchAddress("daughters_py_TauUp", &daughters_py_TauUp, &b_daughters_py_TauUp);
            fChain->SetBranchAddress("daughters_pz_TauUp", &daughters_pz_TauUp, &b_daughters_pz_TauUp);
            fChain->SetBranchAddress("daughters_e_TauUp", &daughters_e_TauUp, &b_daughters_e_TauUp);
            fChain->SetBranchAddress("daughters_px_TauDown", &daughters_px_TauDown, &b_daughters_px_TauDown);
            fChain->SetBranchAddress("daughters_py_TauDown", &daughters_py_TauDown, &b_daughters_py_TauDown);
            fChain->SetBranchAddress("daughters_pz_TauDown", &daughters_pz_TauDown, &b_daughters_pz_TauDown);
            fChain->SetBranchAddress("daughters_e_TauDown", &daughters_e_TauDown, &b_daughters_e_TauDown);
            fChain->SetBranchAddress("daughters_TauUpExists", &daughters_TauUpExists, &b_daughters_TauUpExists);
            fChain->SetBranchAddress("SVfitMassTauUp", &SVfitMassTauUp, &b_SVfitMassTauUp);
            fChain->SetBranchAddress("SVfit_ptTauUp", &SVfit_ptTauUp, &b_SVfit_ptTauUp);
            fChain->SetBranchAddress("SVfit_ptUncTauUp", &SVfit_ptUncTauUp, &b_SVfit_ptUncTauUp);
            fChain->SetBranchAddress("SVfit_etaTauUp", &SVfit_etaTauUp, &b_SVfit_etaTauUp);
            fChain->SetBranchAddress("SVfit_etaUncTauUp", &SVfit_etaUncTauUp, &b_SVfit_etaUncTauUp);
            fChain->SetBranchAddress("SVfit_phiTauUp", &SVfit_phiTauUp, &b_SVfit_phiTauUp);
            fChain->SetBranchAddress("SVfit_phiUncTauUp", &SVfit_phiUncTauUp, &b_SVfit_phiUncTauUp);
            fChain->SetBranchAddress("SVfit_fitMETRhoTauUp", &SVfit_fitMETRhoTauUp, &b_SVfit_fitMETRhoTauUp);
            fChain->SetBranchAddress("SVfit_fitMETPhiTauUp", &SVfit_fitMETPhiTauUp, &b_SVfit_fitMETPhiTauUp);
            fChain->SetBranchAddress("SVfitMassTauDown", &SVfitMassTauDown, &b_SVfitMassTauDown);
            fChain->SetBranchAddress("SVfit_ptTauDown", &SVfit_ptTauDown, &b_SVfit_ptTauDown);
            fChain->SetBranchAddress("SVfit_ptUncTauDown", &SVfit_ptUncTauDown, &b_SVfit_ptUncTauDown);
            fChain->SetBranchAddress("SVfit_etaTauDown", &SVfit_etaTauDown, &b_SVfit_etaTauDown);
            fChain->SetBranchAddress("SVfit_etaUncTauDown", &SVfit_etaUncTauDown, &b_SVfit_etaUncTauDown);
            fChain->SetBranchAddress("SVfit_phiTauDown", &SVfit_phiTauDown, &b_SVfit_phiTauDown);
            fChain->SetBranchAddress("SVfit_phiUncTauDown", &SVfit_phiUncTauDown, &b_SVfit_phiUncTauDown);
            fChain->SetBranchAddress("SVfit_fitMETRhoTauDown", &SVfit_fitMETRhoTauDown, &b_SVfit_fitMETRhoTauDown);
            fChain->SetBranchAddress("SVfit_fitMETPhiTauDown", &SVfit_fitMETPhiTauDown, &b_SVfit_fitMETPhiTauDown);
       }
       
    }
};

#endif
