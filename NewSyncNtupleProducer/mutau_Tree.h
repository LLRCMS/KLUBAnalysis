#ifndef THTH_TREE_H
#define	THTH_TREE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include "TTree.h"
#include "TFile.h"
#include "TSystem.h"
#include "HTauTauTree.h"
#include "myHelper.h"
#include "OfflineProducerHelper.h"

using namespace std;

float nvtx;
unsigned int run, lumi, evt, NUP = -10;

float mu_px, mu_py, mu_pz, mu_pt, mu_en, mu_charge, mu_iso, mu_eta, mu_phi, mu_d0, mu_dZ, mu_mt;
float tau_px, tau_py, tau_pz, tau_en, tau_pt, tau_phi, tau_eta, tau_charge, tau_d0, tau_dZ;
int tau_id;
bool tau_antiMu3T, tau_antiMu3L, tau_antiEleMVA6VL, tau_antiEleMVA6L, tau_antiEleMVA6M, tau_antiEleMVA6T, tau_antiEleMVA6VT;
bool tau_newDmf, tau_oldDmf;
bool HPSisoLoose3Hits, HPSisoMedium3Hits, HPSisoTight3Hits;
float pu_weight, trg_weight, aMCatNLO_weight;
int nbtag, njets, njetspt20;
float met_px, met_py, met_pt, met_phi;
bool extraelec_veto, extramuon_veto, dilepton_veto=false;
bool tau_HPSisoLoose3Hits, tau_HPSisoMedium3Hits, tau_HPSisoTight3Hits;
bool tau_HPSisoLoose3HitsdR03, tau_HPSisoMedium3HitsdR03, tau_HPSisoTight3HitsdR03;
bool tau_HPSisoLooseMVAold, tau_HPSisoMediumMVAold, tau_HPSisoTightMVAold, tau_HPSisoVTightMVAold;
bool tau_HPSisoLooseMVAnew, tau_HPSisoMediumMVAnew, tau_HPSisoTightMVAnew, tau_HPSisoVTightMVAnew;
bool tau_HPSisoLooseMVAolddR03, tau_HPSisoMediumMVAolddR03, tau_HPSisoTightMVAolddR03, tau_HPSisoVTightMVAolddR03;
bool isZmt;
int tau_numChargedIsoCone, tau_numChargedSignalCone;
int tau_oldDm;
float lheHt;
int tau_isTESShifted;
float trackpt;

void fillTree(TTree *Run_Tree, HTauTauTree *tree, int entry_tree, int indice_paire, int indice_tau1, int indice_tau2, int indice_jet1, int indice_jet2, int indice_jetb1, int indice_jetb2, TLorentzVector MEt, int nJets, int nJets20, int nbJets, bool ismc, TH1F* hTauID, float puw) {
  tree->GetEntry(entry_tree);
  run = tree->RunNumber;
  lumi = tree->lumi;
  evt =tree->EventNumber;

  nvtx = tree->npv;
  pu_weight=puw;
  trg_weight=1;
  if (ismc)
    aMCatNLO_weight=tree->aMCatNLOweight;
  else
    aMCatNLO_weight=1;

  TLorentzVector ETM (tree->METx->at(indice_paire),tree->METy->at(indice_paire),0,0);
  met_pt=ETM.Pt();
  met_phi=ETM.Phi();
  met_px=ETM.Px();
  met_py=ETM.Py();
  njets = nJets;
  nbtag = nbJets;
  njetspt20=nJets20;
  TLorentzVector tau1;
  TLorentzVector tau2;
  tau2.SetPxPyPzE(tree->daughters_px->at(indice_tau2),tree->daughters_py->at(indice_tau2),tree->daughters_pz->at(indice_tau2),tree->daughters_e->at(indice_tau2));
  tau1.SetPxPyPzE(tree->daughters_px->at(indice_tau1),tree->daughters_py->at(indice_tau1),tree->daughters_pz->at(indice_tau1),tree->daughters_e->at(indice_tau1));
  TLorentzVector lep;
  dilepton_veto=false; extraelec_veto=false; extramuon_veto=false;
  for (int iDau = 0; iDau < tree->daughters_px->size(); iDau++){
    lep.SetPxPyPzE(tree->daughters_px->at(iDau),tree->daughters_py->at(iDau),tree->daughters_pz->at(iDau),tree->daughters_e->at(iDau));
    if (tree->particleType->at(iDau)==1 && tree->combreliso->at(iDau)<0.3 && tree->daughters_iseleWP90->at(iDau) && fabs(tree->dxy->at(iDau))<0.045 && fabs(tree->dz->at(iDau))<0.2 && fabs(lep.Eta())<2.5 && lep.Pt()>10)
      extraelec_veto=true;
    if (!Overlap_015(tau1,lep) && tree->particleType->at(iDau)==0 && tree->combreliso->at(iDau)<0.3 && fabs(tree->dxy->at(iDau))<0.045 && fabs(tree->dz->at(iDau))<0.2 && fabs(lep.Eta())<2.4 && lep.Pt()>10 && ((tree->daughters_muonID->at(iDau) >> 2) & 1))
      extramuon_veto=true;
    if (!Overlap_015(tau1,lep) && tree->daughters_charge->at(indice_tau1)*tree->daughters_charge->at(iDau)<0 && tree->particleType->at(iDau)==0 && tree->combreliso->at(iDau)<0.3 && fabs(tree->dxy->at(iDau))<0.045 && fabs(tree->dz->at(iDau))<0.2 && fabs(lep.Eta())<2.4 && lep.Pt()>15 && ((tree->daughters_typeOfMuon->at(iDau) >> 0) & 1) && ((tree->daughters_typeOfMuon->at(iDau) >> 1) & 1) && ((tree->daughters_typeOfMuon->at(iDau) >> 2) & 1))
      dilepton_veto=true;
  }

  mu_px = tau1.Px();
  mu_py = tau1.Py();
  mu_pz = tau1.Pz();
  mu_en = tau1.E();
  mu_pt = tau1.Pt();
  mu_phi = tau1.Phi();
  mu_eta = tau1.Eta();
  mu_mt = tree->mT_Dau1->at(indice_paire);
  mu_dZ = tree->dz->at(indice_tau1);
  mu_d0 = tree->dxy->at(indice_tau1);
  mu_iso = tree->combreliso->at(indice_tau1);
  mu_charge = tree->daughters_charge->at(indice_tau1);
  tau_px = tau2.Px();
  tau_py = tau2.Py();
  tau_pz = tau2.Pz();
  tau_en = tau2.E();
  tau_pt = tau2.Pt();
  tau_phi = tau2.Phi();
  tau_eta = tau2.Eta();
  tau_dZ = tree->dz->at(indice_tau2);
  tau_d0=tree->dxy->at(indice_tau2);
  trackpt=tree->daughters_trackRefPt->at(indice_tau2);
  tau_charge = tree->daughters_charge->at(indice_tau2);
  OfflineProducerHelper* HelperTauID = new OfflineProducerHelper(hTauID,hTauID);
  tau_antiMu3T = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("againstMuonTight3")) & 1);
  tau_antiMu3L = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("againstMuonLoose3")) & 1);
  tau_antiEleMVA6VL = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("againstElectronVLooseMVA6")) & 1);
  tau_antiEleMVA6L = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("againstElectronLooseMVA6")) & 1);
  tau_antiEleMVA6M = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("againstElectronMediumMVA6")) & 1);
  tau_antiEleMVA6T = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("againstElectronTightMVA6")) & 1);
  tau_antiEleMVA6VT = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("againstElectronVTightMVA6")) & 1);
  tau_HPSisoLoose3Hits = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byLooseCombinedIsolationDeltaBetaCorr3Hits")) & 1);
  tau_HPSisoMedium3Hits = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byMediumCombinedIsolationDeltaBetaCorr3Hits")) & 1);
  tau_HPSisoTight3Hits = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byTightCombinedIsolationDeltaBetaCorr3Hits")) & 1);
  tau_HPSisoLooseMVAold = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byLooseIsolationMVArun2v1DBoldDMwLT")) & 1);
  tau_HPSisoMediumMVAold = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byMediumIsolationMVArun2v1DBoldDMwLT")) & 1);
  tau_HPSisoTightMVAold = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byTightIsolationMVArun2v1DBoldDMwLT")) & 1);
  tau_HPSisoVTightMVAold = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byVTightIsolationMVArun2v1DBoldDMwLT")) & 1);
  tau_HPSisoLooseMVAnew = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byLooseIsolationMVArun2v1DBnewDMwLT")) & 1);
  tau_HPSisoMediumMVAnew = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byMediumIsolationMVArun2v1DBnewDMwLT")) & 1);
  tau_HPSisoTightMVAnew = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byTightIsolationMVArun2v1DBnewDMwLT")) & 1);
  tau_HPSisoVTightMVAnew = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byVTightIsolationMVArun2v1DBnewDMwLT")) & 1);
  tau_HPSisoLoose3HitsdR03 = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byLooseCombinedIsolationDeltaBetaCorr3HitsdR03")) & 1);
  tau_HPSisoMedium3HitsdR03 = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byMediumCombinedIsolationDeltaBetaCorr3HitsdR03")) & 1);
  tau_HPSisoTight3HitsdR03 = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byTightCombinedIsolationDeltaBetaCorr3HitsdR03")) & 1);
  tau_HPSisoLooseMVAolddR03 = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byLooseIsolationMVArun2v1DBdR03oldDMwLT")) & 1);
  tau_HPSisoMediumMVAolddR03 = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byMediumIsolationMVArun2v1DBdR03oldDMwLT")) & 1);
  tau_HPSisoTightMVAolddR03 = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byTightIsolationMVArun2v1DBdR03oldDMwLT")) & 1);
  tau_HPSisoVTightMVAolddR03 = ((tree->tauID->at(indice_tau2) >>  HelperTauID->getTAUidNumber("byVTightIsolationMVArun2v1DBdR03oldDMwLT")) & 1);

  tau_oldDmf=tree->daughters_decayModeFindingOldDMs->at(indice_tau2);
  tau_newDmf=tree->daughters_decayModeFindingNewDMs->at(indice_tau2);
  tau_numChargedIsoCone= tree->daughters_numChargedParticlesIsoCone->at(indice_tau2);
  tau_numChargedSignalCone= tree->daughters_numChargedParticlesSignalCone->at(indice_tau2);
  tau_oldDm = tree->decayMode->at(indice_tau2);
  tau_isTESShifted = tree->daughters_TauUpExists->at(indice_tau2);
  lheHt = tree->lheHt;

  if (ismc)
    NUP=tree->lheNOutPartons;
  //NUP=tree->NUP;
  float newdr=0.5;
  int indice_gen=0;
  if (ismc){
    for (int iGen = 2; iGen < tree->genpart_pdg->size(); iGen++){
      TLorentzVector g;
      if (tree->genpart_px->at(iGen)==0 and tree->genpart_py->at(iGen)==0) continue;
      g.SetPxPyPzE(tree->genpart_px->at(iGen),tree->genpart_py->at(iGen),tree->genpart_pz->at(iGen),tree->genpart_e->at(iGen));
      if ((fabs(tree->genpart_pdg->at(iGen))==11 or fabs(tree->genpart_pdg->at(iGen))==13 or fabs(tree->genpart_pdg->at(iGen))==15) && g.DeltaR(tau2)<0.5 && g.DeltaR(tau2)<newdr){
	newdr=g.DeltaR(tau2);
	indice_gen=iGen;
      }
    }
    if (indice_gen==0){
      for (int iGen = 2; iGen < tree->genpart_pdg->size(); iGen++){
	TLorentzVector g;
	g.SetPxPyPzE(tree->genpart_px->at(iGen),tree->genpart_py->at(iGen),tree->genpart_pz->at(iGen),tree->genpart_e->at(iGen));
	if ((fabs(tree->genpart_pdg->at(iGen))<30) && g.DeltaR(tau2)<0.5 && g.DeltaR(tau2)<newdr){
	  newdr=g.DeltaR(tau2);
	  indice_gen=iGen;
	}
      }
    }
    tau_id=tree->genpart_pdg->at(indice_gen);
    isZmt=false;
    for (int iGen = 2; iGen < tree->genpart_px->size(); iGen++){
      if (tree->genpart_pdg->at(iGen)==23){
	if (tree->genpart_HZDecayMode->at(iGen)==0)
	  isZmt=true;
      }
    }
  }
  Run_Tree->Fill();
}

#endif
