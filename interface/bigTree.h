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
#include <iostream>
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
  Int_t           year;
  Float_t         prefiringweight;
  Float_t         prefiringweightup;
  Float_t         prefiringweightdown;
  Long64_t        triggerbit;
  Int_t           metfilterbit;
  Float_t         met;
  Float_t         metphi;
  Float_t         met_er;
  Float_t         met_er_phi;
  Float_t         ShiftedDeepMETresponseTune_pt;
  Float_t         ShiftedDeepMETresponseTune_phi;
  Float_t         ShiftedDeepMETresolutionTune_pt;
  Float_t         ShiftedDeepMETresolutionTune_phi;
  Int_t           npv;
  Float_t         npu;
  Float_t         PUNumInteractions;
  Float_t         rho;
  std::vector<float>   *mothers_px;
  std::vector<float>   *mothers_py;
  std::vector<float>   *mothers_pz;
  std::vector<float>   *mothers_e;
  std::vector<Long64_t> *mothers_trgSeparateMatch;
  std::vector<bool>    *daughters_isTauMatched;
  std::vector<int>     *genmatch; // tau genMatch - 1/3:ele - 2/4:mu - 5:tauh - 6:fake
  std::vector<float>   *daughters_px;
  std::vector<float>   *daughters_py;
  std::vector<float>   *daughters_pz;
  std::vector<float>   *daughters_e;
  std::vector<bool>    *daughters_hasTES;
  std::vector<float>    *daughters_TESshiftDM0;
  std::vector<float>    *daughters_TESshiftDM1;
  std::vector<float>    *daughters_TESshiftDM10;
  std::vector<float>    *daughters_TESshiftDM11;
  std::vector<float>   *daughters_px_TauUp;
  std::vector<float>   *daughters_py_TauUp;
  std::vector<float>   *daughters_pz_TauUp;
  std::vector<float>   *daughters_e_TauUp;
  std::vector<float>   *daughters_px_TauDown;
  std::vector<float>   *daughters_py_TauDown;
  std::vector<float>   *daughters_pz_TauDown;
  std::vector<float>   *daughters_e_TauDown;
  std::vector<bool>    *daughters_hasEES;
  std::vector<float>    *daughters_EESshiftDM0up;
  std::vector<float>    *daughters_EESshiftDM1up;
  std::vector<float>    *daughters_EESshiftDM0dw;
  std::vector<float>    *daughters_EESshiftDM1dw;
  std::vector<float>   *daughters_MESshiftup;
  std::vector<float>   *daughters_MESshiftdw;
  std::vector<float>   *daughters_px_EleUp;
  std::vector<float>   *daughters_py_EleUp;
  std::vector<float>   *daughters_pz_EleUp;
  std::vector<float>   *daughters_e_EleUp;
  std::vector<float>   *daughters_px_EleDown;
  std::vector<float>   *daughters_py_EleDown;
  std::vector<float>   *daughters_pz_EleDown;
  std::vector<float>   *daughters_e_EleDown;
  std::vector<int>     *daughters_charge;
  std::vector<float>   *daughters_highestEt_L1IsoTauMatched;
  std::vector<int>     *daughters_genindex;
  Float_t         MC_weight;
  Float_t         lheVPt;
  Float_t         lheHt;
  Int_t           lheNOutPartons;
  Int_t           lheNOutB;
  Float_t         aMCatNLOweight;
  //TString         susyModel;

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
  std::vector<float>   *SVfitMassUnc;
  std::vector<float>   *SVfitTransverseMass;
  std::vector<float>   *SVfitTransverseMassUnc;
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
  std::vector<float>   *METx_UP_JES;
  std::vector<float>   *METy_UP_JES;
  std::vector<float>   *METx_DOWN_JES;
  std::vector<float>   *METy_DOWN_JES;
  std::vector<float>   *METx_UP_TES;
  std::vector<float>   *METy_UP_TES;
  std::vector<float>   *METx_DOWN_TES;
  std::vector<float>   *METy_DOWN_TES;
  std::vector<float>   *METx_UP_EES;
  std::vector<float>   *METy_UP_EES;
  std::vector<float>   *METx_DOWN_EES;
  std::vector<float>   *METy_DOWN_EES;
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
  std::vector<int>     *daughters_muonID;
  std::vector<int>     *daughters_typeOfMuon;
  std::vector<float>   *dxy;
  std::vector<float>   *dz;
  std::vector<float>   *SIP;
  std::vector<bool>    *daughters_iseleWPLoose;
  std::vector<bool>    *daughters_iseleWP80;
  std::vector<bool>    *daughters_iseleWP90;
  std::vector<bool>    *daughters_iseleNoIsoWPLoose;
  std::vector<bool>    *daughters_iseleNoIsoWP80;
  std::vector<bool>    *daughters_iseleNoIsoWP90;
  std::vector<float>   *daughters_eleMVAnt;
  std::vector<int>     *decayMode;
  std::vector<Long64_t> *tauID;
  std::vector<float>   *combreliso;
  std::vector<float>   *tkRelIso;
  std::vector<float>   *daughters_depositR03_tracker;
  std::vector<float>   *daughters_depositR03_ecal;
  std::vector<float>   *daughters_depositR03_hcal;
  std::vector<int>     *daughters_decayModeFindingOldDMs;
  std::vector<int>     *daughters_decayModeFindingNewDMs;
  std::vector<float>   *daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits;
  std::vector<float>   *daughters_byIsolationMVArun2017v2DBoldDMwLTraw2017;
  std::vector<float>   *daughters_byDeepTau2017v2p1VSjetraw;
  std::vector<float>   *daughters_byDeepTau2017v2p1VSeraw;
  std::vector<float>   *daughters_byDeepTau2017v2p1VSmuraw;
  std::vector<int>     *daughters_byVVLooseIsolationMVArun2017v2DBoldDMwLT2017;
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
  std::vector<Long64_t> *daughters_trgMatched;
  std::vector<Long64_t> *daughters_FilterFired;
  std::vector<Long64_t> *daughters_isGoodTriggerType;
  std::vector<Long64_t> *daughters_L3FilterFired;
  std::vector<Long64_t> *daughters_L3FilterFiredLast;
  std::vector<float>   *daughters_HLTpt;
  Int_t           JetsNumber;
  std::vector<Long64_t> *jets_VBFleadFilterMatch;
  std::vector<Long64_t> *jets_VBFsubleadFilterMatch;
  std::vector<float>   *jets_px;
  std::vector<float>   *jets_py;
  std::vector<float>   *jets_pz;
  std::vector<float>   *jets_e;
  std::vector<float>   *jets_mT;
  std::vector<int>     *jets_Flavour;
  std::vector<int>     *jets_HadronFlavour;
  std::vector<int>     *jets_genjetIndex;
  std::vector<float>   *jets_PUJetID;
  std::vector<int>     *jets_PUJetID_WP;
  std::vector<float>   *jets_PUJetIDupdated;
  std::vector<int>     *jets_PUJetIDupdated_WP;
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
  std::vector<float>   *bDeepCSV_probb;
  std::vector<float>   *bDeepCSV_probbb;
  std::vector<float>   *bDeepFlavor_probb;
  std::vector<float>   *bDeepFlavor_probbb;
  std::vector<float>   *bDeepFlavor_problepb;
  std::vector<float>   *bDeepFlavor_probc;
  std::vector<float>   *bDeepFlavor_probuds;
  std::vector<float>   *bDeepFlavor_probg;
  std::vector<float>   *bParticleNetAK4JetTags_probbb;
  std::vector<float>   *bParticleNetAK4JetTags_probpu;
  std::vector<float>   *bParticleNetAK4JetTags_probcc;
  std::vector<float>   *bParticleNetAK4JetTags_probundef;
  std::vector<float>   *bParticleNetAK4JetTags_probc;
  std::vector<float>   *bParticleNetAK4JetTags_probb;
  std::vector<float>   *bParticleNetAK4JetTags_probuds;
  std::vector<float>   *bParticleNetAK4JetTags_probg;
  std::vector<float>   *pfCombinedMVAV2BJetTags;
  std::vector<int>     *PFjetID;
  std::vector<float>   *jetRawf;
  std::vector<float>   *jets_JER;

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
  std::vector<float>   *ak8jets_deepCSV_probb;
  std::vector<float>   *ak8jets_deepCSV_probbb;
  std::vector<float>   *ak8jets_deepFlavor_probb;
  std::vector<float>   *ak8jets_deepFlavor_probbb;
  std::vector<float>   *ak8jets_deepFlavor_problepb;
  std::vector<float>   *ak8jets_massIndependentDeepDoubleBvLJetTags_probHbb;
  std::vector<float>   *ak8jets_deepDoubleBvLJetTags_probHbb;
  std::vector<float>   *ak8jets_deepBoostedJetTags_probHbb;
  std::vector<float>   *ak8jets_particleNetJetTags_probHbb;
  std::vector<float>   *ak8jets_particleNetDiscriminatorsJetTags_HbbvsQCD;
  std::vector<int>     *ak8jets_nsubjets;
  std::vector<float>   *subjets_px;
  std::vector<float>   *subjets_py;
  std::vector<float>   *subjets_pz;
  std::vector<float>   *subjets_e;
  std::vector<float>   *subjets_CSV;
  std::vector<float>   *subjets_deepCSV_probb;
  std::vector<float>   *subjets_deepCSV_probbb;
  std::vector<float>   *subjets_deepFlavor_probb;
  std::vector<float>   *subjets_deepFlavor_probbb;
  std::vector<float>   *subjets_deepFlavor_problepb;
  std::vector<int>     *subjets_ak8MotherIdx;


  // List of branches
  TBranch        *b_EventNumber;   //!
  TBranch        *b_RunNumber;   //!
  TBranch        *b_lumi;   //!
  TBranch        *b_year;   //!
  TBranch        *b_prefiringweight;
  TBranch        *b_prefiringweightup;
  TBranch        *b_prefiringweightdown;
  TBranch        *b_triggerbit;   //!
  TBranch        *b_metfilterbit;   //!
  TBranch        *b_met;   //!
  TBranch        *b_metphi;   //!
  TBranch        *b_met_er;   //!
  TBranch        *b_met_er_phi;   //!
  TBranch        *b_ShiftedDeepMETresponseTune_pt;
  TBranch        *b_ShiftedDeepMETresponseTune_phi;
  TBranch        *b_ShiftedDeepMETresolutionTune_pt;
  TBranch        *b_ShiftedDeepMETresolutionTune_phi;
  TBranch        *b_npv;   //!
  TBranch        *b_npu;   //!
  TBranch        *b_PUNumInteractions;   //!
  TBranch        *b_rho;   //!
  TBranch        *b_mothers_px;   //!
  TBranch        *b_mothers_py;   //!
  TBranch        *b_mothers_pz;   //!
  TBranch        *b_mothers_e;   //!
  TBranch        *b_mothers_trgSeparateMatch; //!
  TBranch        *b_daughters_isTauMatched;
  TBranch        *b_genmatch;
  TBranch        *b_daughters_px;   //!
  TBranch        *b_daughters_py;   //!
  TBranch        *b_daughters_pz;   //!
  TBranch        *b_daughters_e;   //!
  TBranch        *b_daughters_hasTES;
  TBranch        *b_daughters_TESshiftDM0; //!
  TBranch        *b_daughters_TESshiftDM1; //!
  TBranch        *b_daughters_TESshiftDM10; //!
  TBranch        *b_daughters_TESshiftDM11; //!
  TBranch        *b_daughters_px_TauUp;   //!
  TBranch        *b_daughters_py_TauUp;   //!
  TBranch        *b_daughters_pz_TauUp;   //!
  TBranch        *b_daughters_e_TauUp;   //!
  TBranch        *b_daughters_px_TauDown;   //!
  TBranch        *b_daughters_py_TauDown;   //!
  TBranch        *b_daughters_pz_TauDown;   //!
  TBranch        *b_daughters_e_TauDown;   //!
  TBranch        *b_daughters_hasEES;
  TBranch        *b_daughters_EESshiftDM0up; //!
  TBranch        *b_daughters_EESshiftDM1up; //!
  TBranch        *b_daughters_EESshiftDM0dw; //!
  TBranch        *b_daughters_EESshiftDM1dw; //!
  TBranch        *b_daughters_MESshiftup;   //!
  TBranch        *b_daughters_MESshiftdw;   //!
  TBranch        *b_daughters_px_EleUp;   //!
  TBranch        *b_daughters_py_EleUp;   //!
  TBranch        *b_daughters_pz_EleUp;   //!
  TBranch        *b_daughters_e_EleUp;   //!
  TBranch        *b_daughters_px_EleDown;   //!
  TBranch        *b_daughters_py_EleDown;   //!
  TBranch        *b_daughters_pz_EleDown;   //!
  TBranch        *b_daughters_e_EleDown;   //!
  TBranch        *b_daughters_charge;   //!
  TBranch        *b_daughters_highestEt_L1IsoTauMatched;   //!
  TBranch        *b_daughters_genindex;   //!
  TBranch        *b_MC_weight;   //!
  TBranch        *b_lheVPt;   //!
  TBranch        *b_lheHt;   //!
  TBranch        *b_lheNOutPartons; //!
  TBranch        *b_lheNOutB; //!
  TBranch        *b_aMCatNLOweight;   //!
  //TBranch        *b_susyModel;   //!
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
  TBranch        *b_SVfitMassUnc;
  TBranch        *b_SVfitTransverseMass;
  TBranch        *b_SVfitTransverseMassUnc;
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
  TBranch        *b_METx_UP_JES;   //!
  TBranch        *b_METy_UP_JES;   //!
  TBranch        *b_METx_DOWN_JES;   //!
  TBranch        *b_METy_DOWN_JES;   //!
  TBranch        *b_METx_UP_TES;   //!
  TBranch        *b_METy_UP_TES;   //!
  TBranch        *b_METx_DOWN_TES;   //!
  TBranch        *b_METy_DOWN_TES;   //!
  TBranch        *b_METx_UP_EES;   //!
  TBranch        *b_METy_UP_EES;   //!
  TBranch        *b_METx_DOWN_EES;   //!
  TBranch        *b_METy_DOWN_EES;   //!
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
  TBranch        *b_daughters_muonID;   //!
  TBranch        *b_daughters_typeOfMuon;   //!
  TBranch        *b_dxy;   //!
  TBranch        *b_dz;   //!
  TBranch        *b_SIP;   //!
  TBranch        *b_daughters_iseleWPLoose;   //!
  TBranch        *b_daughters_iseleWP80;   //!
  TBranch        *b_daughters_iseleWP90;   //!
  TBranch        *b_daughters_iseleNoIsoWPLoose;   //!
  TBranch        *b_daughters_iseleNoIsoWP80;   //!
  TBranch        *b_daughters_iseleNoIsoWP90;   //!
  TBranch        *b_daughters_eleMVAnt;   //!
  TBranch        *b_decayMode;   //!
  TBranch        *b_tauID;   //!
  TBranch        *b_combreliso;   //!
  TBranch        *b_tkRelIso;   //!
  // TBranch        *b_daughters_IetaIeta;   //!
  // TBranch        *b_daughters_deltaPhiSuperClusterTrackAtVtx;   //!
  TBranch        *b_daughters_depositR03_tracker;   //!
  TBranch        *b_daughters_depositR03_ecal;   //!
  TBranch        *b_daughters_depositR03_hcal;   //!
  TBranch        *b_daughters_decayModeFindingOldDMs;   //!
  TBranch        *b_daughters_decayModeFindingNewDMs;   //!
  TBranch        *b_daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits;   //!
  //TBranch        *b_daughters_byIsolationMVArun2v1DBoldDMwLTraw; //!
  //TBranch        *b_daughters_byIsolationMVArun2017v1DBoldDMwLTraw2017; //!
  TBranch        *b_daughters_byIsolationMVArun2017v2DBoldDMwLTraw2017; //!
  //TBranch        *b_daughters_byIsolationMVArun2017v2DBoldDMdR0p3wLTraw2017;
  TBranch        *b_daughters_byDeepTau2017v2p1VSjetraw;
  TBranch        *b_daughters_byDeepTau2017v2p1VSeraw;
  TBranch        *b_daughters_byDeepTau2017v2p1VSmuraw;
  TBranch        *b_daughters_byVVLooseIsolationMVArun2017v2DBoldDMwLT2017;
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
  TBranch        *b_daughters_trgMatched;   //!
  TBranch        *b_daughters_FilterFired;   //!
  TBranch        *b_daughters_isGoodTriggerType;   //!
  TBranch        *b_daughters_L3FilterFired;   //!
  TBranch        *b_daughters_L3FilterFiredLast;   //!
  TBranch        *b_daughters_HLTpt;   //!
  TBranch        *b_JetsNumber;   //!
  TBranch        *b_jets_VBFleadFilterMatch;
  TBranch        *b_jets_VBFsubleadFilterMatch;
  TBranch        *b_jets_px;   //!
  TBranch        *b_jets_py;   //!
  TBranch        *b_jets_pz;   //!
  TBranch        *b_jets_e;   //!
  TBranch        *b_jets_mT;   //!
  TBranch        *b_jets_Flavour;   //!
  TBranch        *b_jets_HadronFlavour;   //!
  TBranch        *b_jets_genjetIndex;   //!
  TBranch        *b_jets_PUJetID;   //!
  TBranch        *b_jets_PUJetID_WP;   //!
  TBranch        *b_jets_PUJetIDupdated;   //!
  TBranch        *b_jets_PUJetIDupdated_WP;   //!
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
  TBranch        *b_bDeepCSV_probb;   //!
  TBranch        *b_bDeepCSV_probbb;   //!
  TBranch        *b_bDeepFlavor_probb;
  TBranch        *b_bDeepFlavor_probbb;
  TBranch        *b_bDeepFlavor_probc;
  TBranch        *b_bDeepFlavor_problepb;
  TBranch        *b_bDeepFlavor_probuds;
  TBranch        *b_bDeepFlavor_probg;
  TBranch        *b_bParticleNetAK4JetTags_probbb;
  TBranch        *b_bParticleNetAK4JetTags_probpu;
  TBranch        *b_bParticleNetAK4JetTags_probcc;
  TBranch        *b_bParticleNetAK4JetTags_probundef;
  TBranch        *b_bParticleNetAK4JetTags_probc;
  TBranch        *b_bParticleNetAK4JetTags_probb;
  TBranch        *b_bParticleNetAK4JetTags_probuds;
  TBranch        *b_bParticleNetAK4JetTags_probg;
  TBranch        *b_pfCombinedMVAV2BJetTags; //!
  TBranch        *b_PFjetID;   //!
  TBranch        *b_jetRawf;   //!
  TBranch        *b_jets_JER ; //!
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
  TBranch        *b_ak8jets_deepCSV_probb;
  TBranch        *b_ak8jets_deepCSV_probbb;
  TBranch        *b_ak8jets_deepFlavor_probb;
  TBranch        *b_ak8jets_deepFlavor_probbb;
  TBranch        *b_ak8jets_deepFlavor_problepb;
  TBranch        *b_ak8jets_massIndependentDeepDoubleBvLJetTags_probHbb;
  TBranch        *b_ak8jets_deepDoubleBvLJetTags_probHbb;
  TBranch        *b_ak8jets_deepBoostedJetTags_probHbb;
  TBranch        *b_ak8jets_particleNetJetTags_probHbb;
  TBranch        *b_ak8jets_particleNetDiscriminatorsJetTags_HbbvsQCD;
  TBranch        *b_ak8jets_nsubjets;
  TBranch        *b_subjets_px;
  TBranch        *b_subjets_py;
  TBranch        *b_subjets_pz;
  TBranch        *b_subjets_e;
  TBranch        *b_subjets_CSV;
  TBranch        *b_subjets_deepCSV_probb;
  TBranch        *b_subjets_deepCSV_probbb;
  TBranch        *b_subjets_deepFlavor_probb;
  TBranch        *b_subjets_deepFlavor_probbb;
  TBranch        *b_subjets_deepFlavor_problepb;
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
    daughters_isTauMatched = 0;
    genmatch = 0;
    daughters_px = 0;
    daughters_py = 0;
    daughters_pz = 0;
    daughters_e = 0;
    daughters_hasTES = 0;
    daughters_TESshiftDM0 = 0;
    daughters_TESshiftDM1 = 0;
    daughters_TESshiftDM10 = 0;
    daughters_TESshiftDM11 = 0;
    daughters_px_TauUp = 0;
    daughters_py_TauUp = 0;
    daughters_pz_TauUp = 0;
    daughters_e_TauUp = 0;
    daughters_px_TauDown = 0;
    daughters_py_TauDown = 0;
    daughters_pz_TauDown = 0;
    daughters_e_TauDown = 0;
    daughters_hasEES = 0;
    daughters_EESshiftDM0up = 0;
    daughters_EESshiftDM0dw = 0;
    daughters_EESshiftDM1up = 0;
    daughters_EESshiftDM1dw = 0;
    daughters_MESshiftup = 0;
    daughters_MESshiftdw = 0;
    daughters_px_EleUp = 0;
    daughters_py_EleUp = 0;
    daughters_pz_EleUp = 0;
    daughters_e_EleUp = 0;
    daughters_px_EleDown = 0;
    daughters_py_EleDown = 0;
    daughters_pz_EleDown = 0;
    daughters_e_EleDown = 0;
    daughters_charge = 0;
    daughters_highestEt_L1IsoTauMatched = 0;
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
    SVfitMassUnc = 0;
    SVfitTransverseMass = 0;
    SVfitTransverseMassUnc = 0;
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
    METx_UP_JES = 0;
    METy_UP_JES = 0;
    METx_DOWN_JES = 0;
    METy_DOWN_JES = 0;
    METx_UP_TES = 0;
    METy_UP_TES = 0;
    METx_DOWN_TES = 0;
    METy_DOWN_TES = 0;
    METx_UP_EES = 0;
    METy_UP_EES = 0;
    METx_DOWN_EES = 0;
    METy_DOWN_EES = 0;
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
    daughters_muonID = 0;
    daughters_typeOfMuon = 0;
    dxy = 0;
    dz = 0;
    SIP = 0;
    daughters_iseleWPLoose = 0;
    daughters_iseleWP80 = 0;
    daughters_iseleWP90 = 0;
    daughters_iseleNoIsoWPLoose = 0;
    daughters_iseleNoIsoWP80 = 0;
    daughters_iseleNoIsoWP90 = 0;
    daughters_eleMVAnt = 0;
    decayMode = 0;
    tauID = 0;
    combreliso = 0;
	tkRelIso = 0;
    daughters_depositR03_tracker = 0;
    daughters_depositR03_ecal = 0;
    daughters_depositR03_hcal = 0;
    daughters_decayModeFindingOldDMs = 0;
    daughters_decayModeFindingNewDMs = 0;
    daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits = 0;
    daughters_byIsolationMVArun2017v2DBoldDMwLTraw2017 = 0;
    daughters_byDeepTau2017v2p1VSjetraw = 0;
    daughters_byDeepTau2017v2p1VSeraw = 0;
    daughters_byDeepTau2017v2p1VSmuraw = 0;
    daughters_byVVLooseIsolationMVArun2017v2DBoldDMwLT2017 = 0;
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
    daughters_trgMatched = 0;
    daughters_FilterFired = 0;
    daughters_isGoodTriggerType = 0;
    daughters_L3FilterFired = 0;
    daughters_L3FilterFiredLast = 0;
    daughters_HLTpt = 0;
    jets_VBFleadFilterMatch = 0;
    jets_VBFsubleadFilterMatch = 0;
    jets_px = 0;
    jets_py = 0;
    jets_pz = 0;
    jets_e = 0;
    jets_mT = 0;
    jets_Flavour = 0;
    jets_HadronFlavour = 0;
    jets_genjetIndex = 0;
    jets_PUJetID = 0;
    jets_PUJetID_WP = 0;
    jets_PUJetIDupdated = 0;
    jets_PUJetIDupdated_WP = 0;
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
    bDeepCSV_probb = 0;
    bDeepCSV_probbb = 0;
    bDeepFlavor_probb = 0;
    bDeepFlavor_probbb = 0;
    bDeepFlavor_problepb = 0;
    bDeepFlavor_probc = 0;
    bDeepFlavor_probuds = 0;
    bDeepFlavor_probg = 0;
    bParticleNetAK4JetTags_probbb = 0;
    bParticleNetAK4JetTags_probpu = 0;
    bParticleNetAK4JetTags_probcc = 0;
    bParticleNetAK4JetTags_probundef = 0;
    bParticleNetAK4JetTags_probc = 0;
    bParticleNetAK4JetTags_probb = 0;
    bParticleNetAK4JetTags_probuds = 0;
    bParticleNetAK4JetTags_probg = 0;
    pfCombinedMVAV2BJetTags = 0;
    PFjetID = 0;
    jetRawf = 0;
    jets_JER = 0;
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
    ak8jets_deepCSV_probb = 0;
    ak8jets_deepCSV_probbb = 0;
    ak8jets_deepFlavor_probb = 0;
    ak8jets_deepFlavor_probbb = 0;
    ak8jets_deepFlavor_problepb = 0;
    ak8jets_massIndependentDeepDoubleBvLJetTags_probHbb = 0;
    ak8jets_deepDoubleBvLJetTags_probHbb = 0;
    ak8jets_deepBoostedJetTags_probHbb = 0;
    ak8jets_particleNetJetTags_probHbb = 0;
    ak8jets_particleNetDiscriminatorsJetTags_HbbvsQCD = 0;
    ak8jets_nsubjets = 0;
    subjets_px = 0;
    subjets_py = 0;
    subjets_pz = 0;
    subjets_e = 0;
    subjets_CSV = 0;
    subjets_deepCSV_probb = 0;
    subjets_deepCSV_probbb = 0;
    subjets_deepFlavor_probb = 0;
    subjets_deepFlavor_probbb = 0;
    subjets_deepFlavor_problepb = 0;
    subjets_ak8MotherIdx = 0;

    fChain->SetMakeClass(1);
    fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
    fChain->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber);
    fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
    fChain->SetBranchAddress("year", &year, &b_year);
    fChain->SetBranchAddress("prefiringweight", &prefiringweight, &b_prefiringweight);
    fChain->SetBranchAddress("prefiringweightup", &prefiringweightup, &b_prefiringweightup);
    fChain->SetBranchAddress("prefiringweightdown", &prefiringweightdown, &b_prefiringweightdown);
    fChain->SetBranchAddress("triggerbit", &triggerbit, &b_triggerbit);
    fChain->SetBranchAddress("metfilterbit", &metfilterbit, &b_metfilterbit);
    fChain->SetBranchAddress("met", &met, &b_met);
    fChain->SetBranchAddress("metphi", &metphi, &b_metphi);
    fChain->SetBranchAddress("met_er", &met_er, &b_met_er);
    fChain->SetBranchAddress("met_er_phi", &met_er_phi, &b_met_er_phi);
    fChain->SetBranchAddress("ShiftedDeepMETresponseTune_pt", &ShiftedDeepMETresponseTune_pt, &b_ShiftedDeepMETresponseTune_pt);
    fChain->SetBranchAddress("ShiftedDeepMETresponseTune_phi", &ShiftedDeepMETresponseTune_phi, &b_ShiftedDeepMETresponseTune_phi);
    fChain->SetBranchAddress("ShiftedDeepMETresolutionTune_pt", &ShiftedDeepMETresolutionTune_pt, &b_ShiftedDeepMETresolutionTune_pt);
    fChain->SetBranchAddress("ShiftedDeepMETresolutionTune_phi", &ShiftedDeepMETresolutionTune_phi, &b_ShiftedDeepMETresolutionTune_phi);
    fChain->SetBranchAddress("npv", &npv, &b_npv);
    fChain->SetBranchAddress("npu", &npu, &b_npu);
    fChain->SetBranchAddress("rho", &rho, &b_rho);
    fChain->SetBranchAddress("mothers_px", &mothers_px, &b_mothers_px);
    fChain->SetBranchAddress("mothers_py", &mothers_py, &b_mothers_py);
    fChain->SetBranchAddress("mothers_pz", &mothers_pz, &b_mothers_pz);
    fChain->SetBranchAddress("mothers_e", &mothers_e, &b_mothers_e);
    fChain->SetBranchAddress("mothers_trgSeparateMatch", &mothers_trgSeparateMatch, &b_mothers_trgSeparateMatch);
    fChain->SetBranchAddress("genmatch", &genmatch, &b_genmatch);
    fChain->SetBranchAddress("daughters_px", &daughters_px, &b_daughters_px);
    fChain->SetBranchAddress("daughters_py", &daughters_py, &b_daughters_py);
    fChain->SetBranchAddress("daughters_pz", &daughters_pz, &b_daughters_pz);
    fChain->SetBranchAddress("daughters_e", &daughters_e, &b_daughters_e);
    fChain->SetBranchAddress("daughters_charge", &daughters_charge, &b_daughters_charge);
    fChain->SetBranchAddress("daughters_highestEt_L1IsoTauMatched", &daughters_highestEt_L1IsoTauMatched, &b_daughters_highestEt_L1IsoTauMatched);
    fChain->SetBranchAddress("SVfitMass", &SVfitMass, &b_SVfitMass);
    fChain->SetBranchAddress("SVfitMassUnc", &SVfitMassUnc, &b_SVfitMassUnc);
    fChain->SetBranchAddress("SVfitTransverseMass", &SVfitTransverseMass, &b_SVfitTransverseMass);
    fChain->SetBranchAddress("SVfitTransverseMassUnc", &SVfitTransverseMassUnc, &b_SVfitTransverseMassUnc);
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
    fChain->SetBranchAddress("METx_UP_JES", &METx_UP_JES, &b_METx_UP_JES);
    fChain->SetBranchAddress("METy_UP_JES", &METy_UP_JES, &b_METy_UP_JES);
    fChain->SetBranchAddress("METx_DOWN_JES", &METx_DOWN_JES, &b_METx_DOWN_JES);
    fChain->SetBranchAddress("METy_DOWN_JES", &METy_DOWN_JES, &b_METy_DOWN_JES);
    fChain->SetBranchAddress("METx_UP_TES", &METx_UP_TES, &b_METx_UP_TES);
    fChain->SetBranchAddress("METy_UP_TES", &METy_UP_TES, &b_METy_UP_TES);
    fChain->SetBranchAddress("METx_DOWN_TES", &METx_DOWN_TES, &b_METx_DOWN_TES);
    fChain->SetBranchAddress("METy_DOWN_TES", &METy_DOWN_TES, &b_METy_DOWN_TES);
    fChain->SetBranchAddress("METx_UP_EES", &METx_UP_EES, &b_METx_UP_EES);
    fChain->SetBranchAddress("METy_UP_EES", &METy_UP_EES, &b_METy_UP_EES);
    fChain->SetBranchAddress("METx_DOWN_EES", &METx_DOWN_EES, &b_METx_DOWN_EES);
    fChain->SetBranchAddress("METy_DOWN_EES", &METy_DOWN_EES, &b_METy_DOWN_EES);
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
    fChain->SetBranchAddress("daughters_muonID", &daughters_muonID, &b_daughters_muonID);
    fChain->SetBranchAddress("daughters_typeOfMuon", &daughters_typeOfMuon, &b_daughters_typeOfMuon);
    fChain->SetBranchAddress("dxy", &dxy, &b_dxy);
    fChain->SetBranchAddress("dz", &dz, &b_dz);
    fChain->SetBranchAddress("SIP", &SIP, &b_SIP);
    fChain->SetBranchAddress("daughters_iseleWPLoose", &daughters_iseleWPLoose, &b_daughters_iseleWPLoose);
    fChain->SetBranchAddress("daughters_iseleWP80", &daughters_iseleWP80, &b_daughters_iseleWP80);
    fChain->SetBranchAddress("daughters_iseleWP90", &daughters_iseleWP90, &b_daughters_iseleWP90);
    fChain->SetBranchAddress("daughters_iseleNoIsoWPLoose", &daughters_iseleNoIsoWPLoose, &b_daughters_iseleNoIsoWPLoose);
    fChain->SetBranchAddress("daughters_iseleNoIsoWP80", &daughters_iseleNoIsoWP80, &b_daughters_iseleNoIsoWP80);
    fChain->SetBranchAddress("daughters_iseleNoIsoWP90", &daughters_iseleNoIsoWP90, &b_daughters_iseleNoIsoWP90);
    fChain->SetBranchAddress("daughters_eleMVAnt", &daughters_eleMVAnt, &b_daughters_eleMVAnt);
    fChain->SetBranchAddress("decayMode", &decayMode, &b_decayMode);
    fChain->SetBranchAddress("tauID", &tauID, &b_tauID);
    fChain->SetBranchAddress("combreliso", &combreliso, &b_combreliso);
	fChain->SetBranchAddress("tkRelIso", &tkRelIso, &b_tkRelIso);
    // fChain->SetBranchAddress("daughters_IetaIeta", &daughters_IetaIeta, &b_daughters_IetaIeta);
    // fChain->SetBranchAddress("daughters_deltaPhiSuperClusterTrackAtVtx", &daughters_deltaPhiSuperClusterTrackAtVtx, &b_daughters_deltaPhiSuperClusterTrackAtVtx);
    fChain->SetBranchAddress("daughters_depositR03_tracker", &daughters_depositR03_tracker, &b_daughters_depositR03_tracker);
    fChain->SetBranchAddress("daughters_depositR03_ecal", &daughters_depositR03_ecal, &b_daughters_depositR03_ecal);
    fChain->SetBranchAddress("daughters_depositR03_hcal", &daughters_depositR03_hcal, &b_daughters_depositR03_hcal);
    fChain->SetBranchAddress("daughters_decayModeFindingOldDMs", &daughters_decayModeFindingOldDMs, &b_daughters_decayModeFindingOldDMs);
    fChain->SetBranchAddress("daughters_decayModeFindingNewDMs", &daughters_decayModeFindingNewDMs, &b_daughters_decayModeFindingNewDMs);
    fChain->SetBranchAddress("daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits", &daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits, &b_daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits);
    //fChain->SetBranchAddress("daughters_byIsolationMVArun2v1DBoldDMwLTraw", &daughters_byIsolationMVArun2v1DBoldDMwLTraw, &b_daughters_byIsolationMVArun2v1DBoldDMwLTraw);
    //fChain->SetBranchAddress("daughters_byIsolationMVArun2017v1DBoldDMwLTraw2017", &daughters_byIsolationMVArun2017v1DBoldDMwLTraw2017, &b_daughters_byIsolationMVArun2017v1DBoldDMwLTraw2017);
    fChain->SetBranchAddress("daughters_byIsolationMVArun2017v2DBoldDMwLTraw2017", &daughters_byIsolationMVArun2017v2DBoldDMwLTraw2017, &b_daughters_byIsolationMVArun2017v2DBoldDMwLTraw2017);
    //fChain->SetBranchAddress("daughters_byIsolationMVArun2017v2DBoldDMdR0p3wLTraw2017", &daughters_byIsolationMVArun2017v2DBoldDMdR0p3wLTraw2017, &b_daughters_byIsolationMVArun2017v2DBoldDMdR0p3wLTraw2017);
    fChain->SetBranchAddress("daughters_byDeepTau2017v2p1VSjetraw", &daughters_byDeepTau2017v2p1VSjetraw, &b_daughters_byDeepTau2017v2p1VSjetraw);
    fChain->SetBranchAddress("daughters_byDeepTau2017v2p1VSeraw", &daughters_byDeepTau2017v2p1VSeraw, &b_daughters_byDeepTau2017v2p1VSeraw);
    fChain->SetBranchAddress("daughters_byDeepTau2017v2p1VSmuraw", &daughters_byDeepTau2017v2p1VSmuraw, &b_daughters_byDeepTau2017v2p1VSmuraw);
    fChain->SetBranchAddress("daughters_byVVLooseIsolationMVArun2017v2DBoldDMwLT2017", &daughters_byVVLooseIsolationMVArun2017v2DBoldDMwLT2017, &b_daughters_byVVLooseIsolationMVArun2017v2DBoldDMwLT2017);
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
    fChain->SetBranchAddress("daughters_trgMatched", &daughters_trgMatched, &b_daughters_trgMatched);
    fChain->SetBranchAddress("daughters_FilterFired", &daughters_FilterFired, &b_daughters_FilterFired);
    fChain->SetBranchAddress("daughters_isGoodTriggerType", &daughters_isGoodTriggerType, &b_daughters_isGoodTriggerType);
    fChain->SetBranchAddress("daughters_L3FilterFired", &daughters_L3FilterFired, &b_daughters_L3FilterFired);
    fChain->SetBranchAddress("daughters_L3FilterFiredLast", &daughters_L3FilterFiredLast, &b_daughters_L3FilterFiredLast);
    fChain->SetBranchAddress("daughters_HLTpt", &daughters_HLTpt, &b_daughters_HLTpt);
    fChain->SetBranchAddress("JetsNumber", &JetsNumber, &b_JetsNumber);
    fChain->SetBranchAddress("jets_VBFleadFilterMatch", &jets_VBFleadFilterMatch, &b_jets_VBFleadFilterMatch);
    fChain->SetBranchAddress("jets_VBFsubleadFilterMatch", &jets_VBFsubleadFilterMatch, &b_jets_VBFsubleadFilterMatch);
    fChain->SetBranchAddress("jets_px", &jets_px, &b_jets_px);
    fChain->SetBranchAddress("jets_py", &jets_py, &b_jets_py);
    fChain->SetBranchAddress("jets_pz", &jets_pz, &b_jets_pz);
    fChain->SetBranchAddress("jets_e", &jets_e, &b_jets_e);
    fChain->SetBranchAddress("jets_mT", &jets_mT, &b_jets_mT);
    fChain->SetBranchAddress("jets_Flavour", &jets_Flavour, &b_jets_Flavour);
    fChain->SetBranchAddress("jets_HadronFlavour", &jets_HadronFlavour, &b_jets_HadronFlavour);
    fChain->SetBranchAddress("jets_PUJetID", &jets_PUJetID, &b_jets_PUJetID);
    fChain->SetBranchAddress("jets_PUJetID_WP", &jets_PUJetID_WP, &b_jets_PUJetID_WP);
    fChain->SetBranchAddress("jets_PUJetIDupdated", &jets_PUJetIDupdated, &b_jets_PUJetIDupdated);
    fChain->SetBranchAddress("jets_PUJetIDupdated_WP", &jets_PUJetIDupdated_WP, &b_jets_PUJetIDupdated_WP);
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
    fChain->SetBranchAddress("bDeepCSV_probb", &bDeepCSV_probb, &b_bDeepCSV_probb);
    fChain->SetBranchAddress("bDeepCSV_probbb", &bDeepCSV_probbb, &b_bDeepCSV_probbb);
    fChain->SetBranchAddress("bDeepFlavor_probb", &bDeepFlavor_probb, &b_bDeepFlavor_probb);
    fChain->SetBranchAddress("bDeepFlavor_probbb", &bDeepFlavor_probbb, &b_bDeepFlavor_probbb);
    fChain->SetBranchAddress("bDeepFlavor_probc", &bDeepFlavor_probc, &b_bDeepFlavor_probc);
    fChain->SetBranchAddress("bDeepFlavor_problepb", &bDeepFlavor_problepb, &b_bDeepFlavor_problepb);
    fChain->SetBranchAddress("bDeepFlavor_probuds", &bDeepFlavor_probuds, &b_bDeepFlavor_probuds);
    fChain->SetBranchAddress("bDeepFlavor_probg", &bDeepFlavor_probg, &b_bDeepFlavor_probg);
    fChain->SetBranchAddress("bParticleNetAK4JetTags_probbb", &bParticleNetAK4JetTags_probbb, &b_bParticleNetAK4JetTags_probbb);
    fChain->SetBranchAddress("bParticleNetAK4JetTags_probpu", &bParticleNetAK4JetTags_probpu, &b_bParticleNetAK4JetTags_probpu);
    fChain->SetBranchAddress("bParticleNetAK4JetTags_probcc", &bParticleNetAK4JetTags_probcc, &b_bParticleNetAK4JetTags_probcc);
    fChain->SetBranchAddress("bParticleNetAK4JetTags_probundef", &bParticleNetAK4JetTags_probundef, &b_bParticleNetAK4JetTags_probundef);
    fChain->SetBranchAddress("bParticleNetAK4JetTags_probc", &bParticleNetAK4JetTags_probc, &b_bParticleNetAK4JetTags_probc);
    fChain->SetBranchAddress("bParticleNetAK4JetTags_probb", &bParticleNetAK4JetTags_probb, &b_bParticleNetAK4JetTags_probb);
    fChain->SetBranchAddress("bParticleNetAK4JetTags_probuds", &bParticleNetAK4JetTags_probuds, &b_bParticleNetAK4JetTags_probuds);
    fChain->SetBranchAddress("bParticleNetAK4JetTags_probg", &bParticleNetAK4JetTags_probg, &b_bParticleNetAK4JetTags_probg);
    fChain->SetBranchAddress("pfCombinedMVAV2BJetTags", &pfCombinedMVAV2BJetTags, &b_pfCombinedMVAV2BJetTags);
    fChain->SetBranchAddress("PFjetID", &PFjetID, &b_PFjetID);
    fChain->SetBranchAddress("jetRawf", &jetRawf, &b_jetRawf);
    fChain->SetBranchAddress("jets_JER", &jets_JER, &b_jets_JER);
    fChain->SetBranchAddress("ak8jets_px",                                 &ak8jets_px, &b_ak8jets_px);
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
    fChain->SetBranchAddress("ak8jets_deepCSV_probb", &ak8jets_deepCSV_probb, &b_ak8jets_deepCSV_probb);
    fChain->SetBranchAddress("ak8jets_deepCSV_probbb", &ak8jets_deepCSV_probbb, &b_ak8jets_deepCSV_probbb);
    fChain->SetBranchAddress("ak8jets_deepFlavor_probb", &ak8jets_deepFlavor_probb, &b_ak8jets_deepFlavor_probb);
    fChain->SetBranchAddress("ak8jets_deepFlavor_probbb", &ak8jets_deepFlavor_probbb, &b_ak8jets_deepFlavor_probbb);
    fChain->SetBranchAddress("ak8jets_deepFlavor_problepb", &ak8jets_deepFlavor_problepb, &b_ak8jets_deepFlavor_problepb);
    fChain->SetBranchAddress("ak8jets_massIndependentDeepDoubleBvLJetTags_probHbb", &ak8jets_massIndependentDeepDoubleBvLJetTags_probHbb, &b_ak8jets_massIndependentDeepDoubleBvLJetTags_probHbb);
    fChain->SetBranchAddress("ak8jets_deepDoubleBvLJetTags_probHbb", &ak8jets_deepDoubleBvLJetTags_probHbb, &b_ak8jets_deepDoubleBvLJetTags_probHbb);
    fChain->SetBranchAddress("ak8jets_deepBoostedJetTags_probHbb", &ak8jets_deepBoostedJetTags_probHbb, &b_ak8jets_deepBoostedJetTags_probHbb);
    fChain->SetBranchAddress("ak8jets_particleNetJetTags_probHbb", &ak8jets_particleNetJetTags_probHbb, &b_ak8jets_particleNetJetTags_probHbb);
    fChain->SetBranchAddress("ak8jets_particleNetDiscriminatorsJetTags_HbbvsQCD", &ak8jets_particleNetDiscriminatorsJetTags_HbbvsQCD, &b_ak8jets_particleNetDiscriminatorsJetTags_HbbvsQCD);
    fChain->SetBranchAddress("ak8jets_nsubjets", &ak8jets_nsubjets, &b_ak8jets_nsubjets);
    fChain->SetBranchAddress("subjets_px", &subjets_px, &b_subjets_px);
    fChain->SetBranchAddress("subjets_py", &subjets_py, &b_subjets_py);
    fChain->SetBranchAddress("subjets_pz", &subjets_pz, &b_subjets_pz);
    fChain->SetBranchAddress("subjets_e", &subjets_e, &b_subjets_e);
    fChain->SetBranchAddress("subjets_CSV", &subjets_CSV, &b_subjets_CSV);
    fChain->SetBranchAddress("subjets_deepCSV_probb", &subjets_deepCSV_probb, &b_subjets_deepCSV_probb);
    fChain->SetBranchAddress("subjets_deepCSV_probbb", &subjets_deepCSV_probbb, &b_subjets_deepCSV_probbb);
    fChain->SetBranchAddress("subjets_deepFlavor_probb", &subjets_deepFlavor_probb, &b_subjets_deepFlavor_probb);
    fChain->SetBranchAddress("subjets_deepFlavor_probbb", &subjets_deepFlavor_probbb, &b_subjets_deepFlavor_probbb);
    fChain->SetBranchAddress("subjets_deepFlavor_problepb", &subjets_deepFlavor_problepb, &b_subjets_deepFlavor_problepb);
    fChain->SetBranchAddress("subjets_ak8MotherIdx", &subjets_ak8MotherIdx, &b_subjets_ak8MotherIdx);

    // MC only
	std::cout << fChain->GetListOfBranches()->FindObject("MC_weight") << std::endl;
    if(fChain->GetListOfBranches()->FindObject("MC_weight"))
    {
      fChain->SetBranchAddress("PUNumInteractions", &PUNumInteractions, &b_PUNumInteractions);
      fChain->SetBranchAddress("aMCatNLOweight", &aMCatNLOweight, &b_aMCatNLOweight);
      //fChain->SetBranchAddress("susyModel", &susyModel, &b_susyModel);
      fChain->SetBranchAddress("MC_weight", &MC_weight, &b_MC_weight);
      fChain->SetBranchAddress("daughters_isTauMatched", &daughters_isTauMatched, &b_daughters_isTauMatched);
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
      fChain->SetBranchAddress("genpart_WMothInd", &genpart_WMothInd, &b_genpart_WMothInd);
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
      fChain->SetBranchAddress("lheVPt", &lheVPt, &b_lheVPt);
      fChain->SetBranchAddress("lheHt", &lheHt, &b_lheHt);
      fChain->SetBranchAddress("lheNOutPartons", &lheNOutPartons, &b_lheNOutPartons);
      fChain->SetBranchAddress("lheNOutB", &lheNOutB, &b_lheNOutB);
      fChain->SetBranchAddress("daughters_hasTES", &daughters_hasTES, &b_daughters_hasTES);
      fChain->SetBranchAddress("daughters_TESshiftDM0", &daughters_TESshiftDM0, &b_daughters_TESshiftDM0);
      fChain->SetBranchAddress("daughters_TESshiftDM1", &daughters_TESshiftDM1, &b_daughters_TESshiftDM1);
      fChain->SetBranchAddress("daughters_TESshiftDM10", &daughters_TESshiftDM10, &b_daughters_TESshiftDM10);
      fChain->SetBranchAddress("daughters_TESshiftDM11", &daughters_TESshiftDM11, &b_daughters_TESshiftDM11);
      fChain->SetBranchAddress("daughters_px_TauUp", &daughters_px_TauUp, &b_daughters_px_TauUp);
      fChain->SetBranchAddress("daughters_py_TauUp", &daughters_py_TauUp, &b_daughters_py_TauUp);
      fChain->SetBranchAddress("daughters_pz_TauUp", &daughters_pz_TauUp, &b_daughters_pz_TauUp);
      fChain->SetBranchAddress("daughters_e_TauUp", &daughters_e_TauUp, &b_daughters_e_TauUp);
      fChain->SetBranchAddress("daughters_px_TauDown", &daughters_px_TauDown, &b_daughters_px_TauDown);
      fChain->SetBranchAddress("daughters_py_TauDown", &daughters_py_TauDown, &b_daughters_py_TauDown);
      fChain->SetBranchAddress("daughters_pz_TauDown", &daughters_pz_TauDown, &b_daughters_pz_TauDown);
      fChain->SetBranchAddress("daughters_e_TauDown", &daughters_e_TauDown, &b_daughters_e_TauDown);
      fChain->SetBranchAddress("daughters_hasEES", &daughters_hasEES, &b_daughters_hasEES);
      fChain->SetBranchAddress("daughters_EESshiftDM0up", &daughters_EESshiftDM0up, &b_daughters_EESshiftDM0up);
      fChain->SetBranchAddress("daughters_EESshiftDM1up", &daughters_EESshiftDM1up, &b_daughters_EESshiftDM1up);
      fChain->SetBranchAddress("daughters_EESshiftDM0dw", &daughters_EESshiftDM0dw, &b_daughters_EESshiftDM0dw);
      fChain->SetBranchAddress("daughters_EESshiftDM1dw", &daughters_EESshiftDM1dw, &b_daughters_EESshiftDM1dw);
      fChain->SetBranchAddress("daughters_MESshiftup", &daughters_MESshiftup, &b_daughters_MESshiftup);
      fChain->SetBranchAddress("daughters_MESshiftdw", &daughters_MESshiftdw, &b_daughters_MESshiftdw);
      fChain->SetBranchAddress("daughters_px_EleUp", &daughters_px_EleUp, &b_daughters_px_EleUp);
      fChain->SetBranchAddress("daughters_py_EleUp", &daughters_py_EleUp, &b_daughters_py_EleUp);
      fChain->SetBranchAddress("daughters_pz_EleUp", &daughters_pz_EleUp, &b_daughters_pz_EleUp);
      fChain->SetBranchAddress("daughters_e_EleUp", &daughters_e_EleUp, &b_daughters_e_EleUp);
      fChain->SetBranchAddress("daughters_px_EleDown", &daughters_px_EleDown, &b_daughters_px_EleDown);
      fChain->SetBranchAddress("daughters_py_EleDown", &daughters_py_EleDown, &b_daughters_py_EleDown);
      fChain->SetBranchAddress("daughters_pz_EleDown", &daughters_pz_EleDown, &b_daughters_pz_EleDown);
      fChain->SetBranchAddress("daughters_e_EleDown", &daughters_e_EleDown, &b_daughters_e_EleDown);
    }
  }
};

#endif
