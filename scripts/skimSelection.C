#include <stdlib.h>

void skimSelection()
{
  //TString fileName = "/data_CMS/cms/amendola/HH2017Skims/SKIMS_3Oct2018_Run2017_synch/SKIM_DY_NLO/total";
  //TString fileName = "/data_CMS/cms/amendola/HH2017Skims/SKIMS_29Oct2018_Run2017/SKIM_TT_fullyLep/total";
  //TString fileName = "/data_CMS/cms/amendola/HH2017Skims/SKIMS_29Oct2018_Run2017/SKIM_DY_NLO/total";
  //TString fileName = "/data_CMS/cms/amendola/HH2017Skims/SKIMS_6Nov2018_Run2017/SKIM_DY_NLO/total";
  //TString fileName = "/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_TT_fullyLep/total";
  //TString fileName = "/gwteraz/users/brivio/SKIMMED_6Nov2018/SKIM_TT_fullyLep_KinFit/output";
  TString fileName = "/gwteraz/users/brivio/SKIMMED_6Nov2018/SKIM_TT_fullyLep_lepSF/output";

    TFile* inFile = new TFile (Form("%s.root", fileName.Data()));
    TH1F* h_eff = (TH1F*) inFile->Get("h_eff");


    TTree* tIn = (TTree*) inFile->Get("HTauTauTree");
    Float_t dau1_pt;
    tIn-> SetBranchAddress("dau1_pt",&dau1_pt);

    //TString baseline_SR = "pairType == 2 && dau1_pt > 40 && abs (dau1_eta) < 2.1 && dau2_pt > 40 && abs (dau2_eta) < 2.1 && nleps == 0 && isOS != 0 && dau1_MVAisoNew >= 3 && dau2_MVAisoNew >= 3";
    //TString baseline_SR = "nleps==0 && pairType==2";
    //TString baseline_SR = "nleps==0";
    TString baseline_SR = "nleps == 0 && isOS != 0 && dau1_MVAisoNew >= 3 && dau2_MVAisoNew >= 3 && pairType==2";
    TFile* outFile = new TFile (Form("%s_baseline_SR.root", fileName.Data()),"recreate");
    TTree* tOut = tIn->CloneTree(0);

    TTreeFormula* formula = new TTreeFormula("baseline_SR", baseline_SR.Data(),tOut);
    int nEvts = tIn->GetEntries();
    for (int iEv = 0; iEv < nEvts; iEv++)
      {
        if (iEv % 1000 == 0) cout << iEv << " / " << nEvts << endl;
        tIn->GetEntry(iEv);
	Int_t arraySize = formula->GetNdata();

	for (int i = 0; i < arraySize; i++)
	  {
	    if(formula->EvalInstance(i) == 0){
	      //nothing
	    }else{
	      tOut->Fill();
	    }
	     
	  }
	
      }
    outFile->cd();
    tOut->Write();
    h_eff->Write();
    outFile->Close();


}


