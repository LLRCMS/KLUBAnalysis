#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TMath.h"
#include "TSystem.h"
#include "TRandom.h"
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "makeHisto.h"
#include "mutau_Tree.h"
#include "OfflineProducerHelper.h"
#include "LumiReweightingStandAlone.h"

int main(int argc, char** argv) {

    using namespace std;
    myMap1 = new map<string, TH1F*>();
    myMap2 = new map<string, TH2F*>();

    string status_sample = *(argv + 1);
    bool isMC = false;
    bool isData = false;
    if (status_sample.compare("mc") == 0) isMC = true;
    if (status_sample.compare("data") == 0) isData = true;
    string out = *(argv + 2);
    string outname= out;

    string in = *(argv + 3);

    ifstream infile;
    string read_file_name(in.c_str());
    infile.open(read_file_name);
    TString infile_TStr(in);

    TFile *fout = TFile::Open(outname.c_str(), "RECREATE");

    //TFile *fout = TFile::Open("test.root", "RECREATE");

    //TFile* fIn = new TFile ("ntuple29juillet.root");
    string inname= in;

    if(infile_TStr.Contains("txt"))
      {	
	string sLine;
	while (!infile.eof())
	  {
	    infile >> sLine;
	    // cout << sLine.data() << endl;
	  }
	inname = sLine;
      }

    TFile *fIn = TFile::Open(inname.c_str());
    TTree* treePtr = (TTree*) fIn->Get("HTauTauTree/HTauTauTree");
    TH1F *evCounter = (TH1F*) fIn->Get("HTauTauTree/Counters");
    TH1F *htauid = (TH1F*) fIn->Get("HTauTauTree/TauIDs");

    cout<<"here"<<endl;

    TChain* dummy = new TChain("HTauTauTree/HTauTauTree");
    cout<<"here1"<<endl;

    HTauTauTree* tree = new HTauTauTree(dummy);
    cout<<"here1.5"<<endl;

    // HTauTauTree* tree = new HTauTauTree("HTauTauTree/HTauTauTree");
    // HTauTauTree* tree = new HTauTauTree (treePtr);

    cout<<"here2"<<endl;

    if(!infile_TStr.Contains("txt"))
      {
	tree->_tree->Add(inname.c_str());
      }
    else 
      {
	cout<<"here"<<endl;

	ifstream infile2;
	string read_file_name2(in.c_str());
	infile2.open(read_file_name2);

	int counter = 0;

	string sLine;
	while (!infile2.eof())
	  {
	    infile2 >> sLine;
	    cout << "adding --> "<<sLine.data() << endl;
	    tree->_tree->Add(sLine.c_str());
	    counter++;
	    //if(counter>2) break;
	  }
      }

    // tree->_tree->Print();
    // cout<<"entries = "<<tree->GetEntries()<<endl;

    //OfflineProducerHelper helper;

    //#################################################################################################
    TTree *Run_Tree = new TTree("RLE_tree", "RLE_tree");
    Run_Tree->SetDirectory(0);
    Run_Tree->Branch("run", &run, "run/I");
    Run_Tree->Branch("lumi", &lumi, "lumi/I");
    Run_Tree->Branch("evt", &evt, "evt/I");


    Run_Tree->Branch("NUP", &NUP, "NUP/I");
    Run_Tree->Branch("dilepton_veto", &dilepton_veto, "dilepton_veto/O");
    Run_Tree->Branch("extraelec_veto", &extraelec_veto, "extraelec_veto/O");
    Run_Tree->Branch("extramuon_veto", &extramuon_veto, "extramuon_veto/O");
    Run_Tree->Branch("isZmt", &isZmt, "isZmt/O");

    Run_Tree->Branch("trg_weight", &trg_weight, "trg_weight/F");
    Run_Tree->Branch("pu_weight", &pu_weight, "pu_weight/F");
    Run_Tree->Branch("aMCatNLO_weight", &aMCatNLO_weight, "aMCatNLO_weight/F");

    Run_Tree->Branch("nvtx", &nvtx, "nvtx/F");

    Run_Tree->Branch("mu_pt", &mu_pt, "mu_pt/F");
    Run_Tree->Branch("mu_en", &mu_en, "mu_en/F");
    Run_Tree->Branch("mu_px", &mu_px, "mu_px/F");
    Run_Tree->Branch("mu_py", &mu_py, "mu_py/F");
    Run_Tree->Branch("mu_pz", &mu_pz, "mu_pz/F");
    Run_Tree->Branch("mu_phi", &mu_phi, "mu_phi/F");
    Run_Tree->Branch("mu_eta", &mu_eta, "mu_eta/F");
    Run_Tree->Branch("mu_charge", &mu_charge, "mu_charge/F");
    Run_Tree->Branch("mu_d0", &mu_d0, "mu_d0/F");
    Run_Tree->Branch("mu_dZ", &mu_dZ, "mu_dZ/F");
    Run_Tree->Branch("mu_iso", &mu_iso, "mu_iso/F");

    Run_Tree->Branch("tau_pt", &tau_pt, "tau_pt/F");
    Run_Tree->Branch("tau_en", &tau_en, "tau_en/F");
    Run_Tree->Branch("tau_phi", &tau_phi, "tau_phi/F");
    Run_Tree->Branch("tau_eta", &tau_eta, "tau_eta/F");
    Run_Tree->Branch("tau_px", &tau_px, "tau_px/F");
    Run_Tree->Branch("tau_py", &tau_py, "tau_py/F");
    Run_Tree->Branch("tau_pz", &tau_pz, "tau_pz/F");
    Run_Tree->Branch("tau_charge", &tau_charge, "tau_charge/F");
    Run_Tree->Branch("trackpt", &trackpt, "trackpt/F");
    Run_Tree->Branch("tau_d0", &tau_d0, "tau_d0/F");
    Run_Tree->Branch("tau_dZ", &tau_dZ, "tau_dZ/F");
    Run_Tree->Branch("tau_HPSisoLoose3Hits", &tau_HPSisoLoose3Hits, "tau_HPSisoLoose3Hits/O");
    Run_Tree->Branch("tau_HPSisoMedium3Hits", &tau_HPSisoMedium3Hits, "tau_HPSisoMedium3Hits/O");
    Run_Tree->Branch("tau_HPSisoTight3Hits", &tau_HPSisoTight3Hits, "tau_HPSisoTight3Hits/O");
    Run_Tree->Branch("tau_HPSisoLoose3HitsdR03", &tau_HPSisoLoose3HitsdR03, "tau_HPSisoLoose3HitsdR03/O");
    Run_Tree->Branch("tau_HPSisoMedium3HitsdR03", &tau_HPSisoMedium3HitsdR03, "tau_HPSisoMedium3HitsdR03/O");
    Run_Tree->Branch("tau_HPSisoTight3HitsdR03", &tau_HPSisoTight3HitsdR03, "tau_HPSisoTight3HitsdR03/O");
    Run_Tree->Branch("tau_HPSisoLooseMVAold", &tau_HPSisoLooseMVAold, "tau_HPSisoLooseMVAold/O");
    Run_Tree->Branch("tau_HPSisoMediumMVAold", &tau_HPSisoMediumMVAold, "tau_HPSisoMediumMVAold/O");
    Run_Tree->Branch("tau_HPSisoTightMVAold", &tau_HPSisoTightMVAold, "tau_HPSisoTightMVAold/O");
    Run_Tree->Branch("tau_HPSisoVTightMVAold", &tau_HPSisoVTightMVAold, "tau_HPSisoVTightMVAold/O");
    Run_Tree->Branch("tau_HPSisoLooseMVAnew", &tau_HPSisoLooseMVAnew, "tau_HPSisoLooseMVAnew/O");
    Run_Tree->Branch("tau_HPSisoMediumMVAnew", &tau_HPSisoMediumMVAnew, "tau_HPSisoMediumMVAnew/O");
    Run_Tree->Branch("tau_HPSisoTightMVAnew", &tau_HPSisoTightMVAnew, "tau_HPSisoTightMVAnew/O");
    Run_Tree->Branch("tau_HPSisoVTightMVAnew", &tau_HPSisoVTightMVAnew, "tau_HPSisoVTightMVAnew/O");
    Run_Tree->Branch("tau_HPSisoLooseMVAolddR03", &tau_HPSisoLooseMVAolddR03, "tau_HPSisoLooseMVAolddR03/O");
    Run_Tree->Branch("tau_HPSisoMediumMVAolddR03", &tau_HPSisoMediumMVAolddR03, "tau_HPSisoMediumMVAolddR03/O");
    Run_Tree->Branch("tau_HPSisoTightMVAolddR03", &tau_HPSisoTightMVAolddR03, "tau_HPSisoTightMVAolddR03/O");
    Run_Tree->Branch("tau_HPSisoVTightMVAolddR03", &tau_HPSisoVTightMVAolddR03, "tau_HPSisoVTightMVAolddR03/O");
    Run_Tree->Branch("tau_antiEleMVA6L", &tau_antiEleMVA6L, "tau_antiEleMVA6L/O");
    Run_Tree->Branch("tau_antiEleMVA6M", &tau_antiEleMVA6M, "tau_antiEleMVA6M/O");
    Run_Tree->Branch("tau_antiEleMVA6T", &tau_antiEleMVA6T, "tau_antiEleMVA6T/O");
    Run_Tree->Branch("tau_antiEleMVA6VL", &tau_antiEleMVA6VL, "tau_antiEleMVA6VL/O");
    Run_Tree->Branch("tau_antiEleMVA6VT", &tau_antiEleMVA6VT, "tau_antiEleMVA6VT/O");
    Run_Tree->Branch("tau_antiMu3L", &tau_antiMu3L, "tau_antiMu3L/O");
    Run_Tree->Branch("tau_antiMu3T", &tau_antiMu3T, "tau_antiMu3T/O");
    Run_Tree->Branch("tau_newDmf", &tau_newDmf, "tau_newDmf/O");
    Run_Tree->Branch("tau_oldDmf", &tau_oldDmf, "tau_oldDmf/O");
    Run_Tree->Branch("tau_id", &tau_id, "tau_id/I");
    Run_Tree->Branch("tau_numChargedIsoCone", &tau_numChargedIsoCone, "tau_numChargedIsoCone/I");
    Run_Tree->Branch("tau_numChargedSignalCone", &tau_numChargedSignalCone, "tau_numChargedSignalCone/I");
    Run_Tree->Branch("tau_oldDm", &tau_oldDm, "tau_oldDm/I");
    Run_Tree->Branch("tau_isTESShifted", &tau_isTESShifted, "tau_isTESShifted/I");
    Run_Tree->Branch("lheHt", &lheHt, "lheHt/F");

    Run_Tree->Branch("met_pt", &met_pt, "met_pt/F");
    Run_Tree->Branch("met_px", &met_px, "met_px/F");
    Run_Tree->Branch("met_py", &met_py, "met_py/F");
    Run_Tree->Branch("met_phi", &met_phi, "met_phi/F");
    Run_Tree->Branch("nbtag", &nbtag, "nbtag/I");

    reweight::LumiReWeighting* LumiWeights_12;
    LumiWeights_12 = new reweight::LumiReWeighting("pileup-hists/MC_Fall15_PU25_V1.root", "pileup-hists/Data_Pileup_2015D_Feb02.root", "pileup", "pileup");
    float pu;

    int CounterEntriesPass = 0;
    int CounterEntries = 0;

    bool EndOfChain = false;

    //#################################################################################################
    // for (int iEntry = 0; EndOfChain==true ; iEntry++)
    for (int iEntry = 0; iEntry < 1000000000000000 ; iEntry++)
    // for (int iEntry = 0; iEntry < tree->GetEntries() ; iEntry++)
    {
        pu=1.0;
	if(tree->GetEntry(iEntry)==0)
	  {
	    EndOfChain = true;
	    cout<<"end of loop!"<<endl;
	    break;
	  }
        tree->GetEntry(iEntry);
	CounterEntries++;

	if(iEntry%1000==0) cout<<"Entry #"<<iEntry<<endl;
	bool print=false;

	plotFill("nbevt",0,9,0,9);
	plotFill("nbevt",1,9,0,9,tree->aMCatNLOweight);

	   bool PassMuTrigger=false;
	   if (print) cout<<"Evenement n°"<<tree->EventNumber<<endl;
           if (isMC && (tree->triggerbit & (1 << 2))) PassMuTrigger=true;
	   if (!isMC && (tree->triggerbit & (1 << 3))) PassMuTrigger=true;

	   if (!PassMuTrigger) continue;

           if (isMC) {
              pu = LumiWeights_12->weight(tree->npu);
           }
	//cout<<tree->npu<<endl;
	   bool isData=false; int mucharge=1; int elecharge=1; bool jet_inbetween=false;
	   bool isPair=false; 
	   int dau1index=0; int dau2index=0; int pairindex=-1;
	   float metx=0; float mety=0;
           for (int iMoth = 0; iMoth < tree->mothers_px->size() && !isPair; iMoth++)
           {
              dau1index = tree->indexDau1->at(iMoth);
              dau2index = tree->indexDau2->at(iMoth);
              if (!isMC && !(tree->daughters_FilterFired->at(dau1index) & (1 << 3))) continue;
              if (isMC && !(tree->daughters_FilterFired->at(dau1index) & (1 << 2))) continue;
              if (isMC && tree->daughters_HLTpt->at(dau1index)<18) continue;
	      if (print) cout<<"mutau decay"<<endl;
	      if (fabs(tree->particleType->at(dau1index) - tree->particleType->at(dau2index)) !=2) continue; //mutau
	      TLorentzVector dau1;
              TLorentzVector dau2;
	      dau1.SetPxPyPzE(tree->daughters_px->at(dau1index),tree->daughters_py->at(dau1index),tree->daughters_pz->at(dau1index),tree->daughters_e->at(dau1index));
	      dau2.SetPxPyPzE(tree->daughters_px->at(dau2index),tree->daughters_py->at(dau2index),tree->daughters_pz->at(dau2index),tree->daughters_e->at(dau2index));

	      if (print) cout<<"Overlap"<<endl;
	      if (Overlap_2(dau1,dau2)) continue;//Overlap
	      if (print) cout<<"Overlap global muon"<<endl;
	      TLorentzVector lep;
	      bool overlapMuon=false;
	      for (int iDau = 0; iDau < tree->daughters_px->size(); iDau++){
                  lep.SetPxPyPzE(tree->daughters_px->at(iDau),tree->daughters_py->at(iDau),tree->daughters_pz->at(iDau),tree->daughters_e->at(iDau));
       		  if (Overlap_2(dau2,lep) && tree->particleType->at(iDau)==0 && fabs(lep.Eta())<2.4 && lep.Pt()>5 && ((tree->daughters_typeOfMuon->at(iDau) >> 0) & 1))
		      overlapMuon=true;
	      }
	      if (overlapMuon) continue;
	      if (print) cout<<"dxy"<<endl;
	      if (fabs(tree->dxy->at(dau1index))>0.045) continue;//Dxy
	      if (print) cout<<"dz"<<endl;
              if (fabs(tree->dz->at(dau1index))>0.2 or fabs(tree->dz->at(dau2index))>0.2) continue;//Dz
	      if (print) cout<<"pt"<<endl;
	      if (dau1.Pt()<19 or dau2.Pt()<18) continue;//Pt
	      // if (dau1.Pt()<19 or dau2.Pt()<20) continue;//Pt
	      if (print) cout<<"eta"<<endl;
	      if (fabs(dau1.Eta())>2.1 or fabs(dau2.Eta())>2.3) continue;//Eta
	      if (print) cout<<"muon ID"<<endl;
	      if (!((tree->daughters_muonID->at(dau1index) >> 2) & 1)) continue;
	      //if (tree->daughters_trackRefPt->at(dau2index)<5) continue;
              if (isMC && tree->daughters_HLTpt->at(dau1index)<18) continue;
              if (tree->daughters_decayModeFindingNewDMs->at(dau2index)==0) continue;//Decay mode finding
              if (fabs(tree->daughters_charge->at(dau2index))>1) continue;

       	      pairindex=iMoth; 
	      isPair=true;
	  }
	  if (!isPair) continue;	
          metx=tree->METx->at(pairindex);
          mety=tree->METy->at(pairindex);
	  TLorentzVector tau1;
	  TLorentzVector tau2;
          tau2.SetPxPyPzE(tree->daughters_px->at(dau2index),tree->daughters_py->at(dau2index),tree->daughters_pz->at(dau2index),tree->daughters_e->at(dau2index));
          tau1.SetPxPyPzE(tree->daughters_px->at(dau1index),tree->daughters_py->at(dau1index),tree->daughters_pz->at(dau1index),tree->daughters_e->at(dau1index));
	  int nbJets=0; int nJets=0; int nJets20=0;
	  int b1_index=-1; int b2_index=-1; int j1_index=-1; int j2_index=-1;
	  TLorentzVector jetb1, jetb2, jet1, jet2;
          for (int iJet = 0; iJet < tree->bDiscriminator->size(); iJet++)
          {
	    TLorentzVector jet(tree->jets_px->at(iJet),tree->jets_py->at(iJet),tree->jets_pz->at(iJet),tree->jets_e->at(iJet));
	    if (Overlap_3(tau1,tau2,jet)) continue;
	    if (tree->PFjetID->at(iJet)==0) continue;
	    if (tree->bCSVscore->at(iJet)>0.80 && jet.Pt()>20 && fabs(jet.Eta()<2.4)){
		nbJets++;
	     	if (nbJets==1) {
	     	    jetb1.SetPxPyPzE(jet.Px(),jet.Py(),jet.Pz(),jet.E());
	     	    b1_index=iJet;
	     	}
	        else if (nbJets==2){
	           jetb2.SetPxPyPzE(jet.Px(),jet.Py(),jet.Pz(),jet.E());
	            b2_index=iJet;
	        }
	    }
            if (jet.Pt()>20 && fabs(jet.Eta()<4.7)){
                nJets20++;
                if (nJets20==1){
	           jet1.SetPxPyPzE(jet.Px(),jet.Py(),jet.Pz(),jet.E());
	           j1_index=iJet;
	        }
                else if (nJets20==2){
	           jet2.SetPxPyPzE(jet.Px(),jet.Py(),jet.Pz(),jet.E());
	           j2_index=iJet;
	        }
             }
             if (jet.Pt()>30 && fabs(jet.Eta()<4.7)){
                 nJets++;
	     }
          }
	  ++CounterEntriesPass;

	  TLorentzVector met(metx,mety,0,0);
	  fillTree(Run_Tree,tree,iEntry,pairindex,dau1index,dau2index,j1_index,j2_index,b1_index,b2_index,met,nJets,nJets20,nbJets,isMC,htauid,pu);

	  if(CounterEntriesPass%100000==0)
	    {
	      fout->cd();
	      Run_Tree->Write();
	    }

 //      }
    }//loop over events
    fout->cd();
    Run_Tree->Write();
    TH1F* Counters = new TH1F("Counters","Counters",2,0.,1);
    Counters->SetBinContent(1,CounterEntries);
    Counters->SetBinContent(2,CounterEntriesPass);
    Counters->Write();


    map<string, TH1F*>::const_iterator iMap1 = myMap1->begin();
    map<string, TH1F*>::const_iterator jMap1 = myMap1->end();

    for (; iMap1 != jMap1; ++iMap1)
        nplot1(iMap1->first)->Write();

    map<string, TH2F*>::const_iterator iMap2 = myMap2->begin();
    map<string, TH2F*>::const_iterator jMap2 = myMap2->end();

    for (; iMap2 != jMap2; ++iMap2)
        nplot2(iMap2->first)->Write();

    fout->Close();

    return 0;
}

