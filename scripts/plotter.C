//I must add the XSection x BR from the histogram
//gROOT->LoadMacro("../src/HistoManager.cc+");

void FillHistos(int);
TString Selection(int);

void plotter(){
  FillHistos(0);
}

TString Selection(int selection=0){
  if(selection==0) return "";
  else {}
  return "";
}

void FillHistos(int selection=0){
  //gROOT->LoadMacro("../src/HistoManager.cc+");

  //Configurables: files and vars
  const int nB = 1;
  const int nS = 1;
  const int nVars =2;
  TString samples[nB+nS] ={"DY","Lambda1"};//CAUTION! Bkg before Sig!
  TString files[nB+nS] = {"../test/test.root","../test/test.root"};
  TString Variables[nVars] = {"HH_pt","HHKin_mass"};//same name as in the small tree
  int xup[nVars]           = {900,900};
  int xlow[nVars]          = {0,250};
  int bins[nVars]          = {300,100};
  int colors[]={kBlack,kRed,kGreen,kYellow+2,kRed+2,kMagenta,kCyan};//add more if needed

  //Utilities
  TBranch *branches[nVars];
  float address[nVars];for(int i =0; i<nVars; i++)address[i]=0;
  TString histoName;
  TH1F *histos[nVars][nS+nB];
  TFile *fIn[nS+nB]; 
  for(int i=0;i<nS+nB;i++)fIn[i]= TFile::Open(files[i].Data());
  //Create histograms
  //cout<<"Calling histoManager..."<<endl;
  //HistoManager *manager = new HistoManager("test");
  //cout<<"... called"<<endl;
  for(int i =0; i<nVars; i++){
    for(int j =0; j<nB+nS; j++){
      histoName.Form("%s_%s",Variables[i].Data(),samples[j].Data());
      //cout<<"adding "<<histoName.Data()<<" ..."<<endl;
      //manager->AddNewHisto(histoName.Data(),histoName.Data(),bins[i],xlow[i],xup[i]);
      //cout<<"... added!"<<endl;
      histos[i][j]=new TH1F(histoName.Data(),histoName.Data(),bins[i],xlow[i],xup[i]);
      if(j<nB){//background
	histos[i][j]->SetFillStyle(1001);
	histos[i][j]->SetFillColor(colors[j]);
	histos[i][j]->SetLineColor(colors[j]);
      }else{
	histos[i][j]->SetFillStyle(0);
	histos[i][j]->SetMarkerColor(colors[j]);
	histos[i][j]->SetLineColor(colors[j]);
	histos[i][j]->SetMarkerStyle(20);
      }
    }
  }
  //cout<<"Called"<<endl;
 
  //Loop on the small trees and fill histograms
  for(int i =0; i<nB+nS; i++){

    cout<<"processing sample "<<samples[i].Data()<<endl;
    double eff = ((TH1F*)fIn[i]->Get("h_eff"))->GetBinContent(2)/((TH1F*)fIn[i]->Get("h_eff"))->GetBinContent(1);
    TTree *tree = (TTree*)fIn[i]->Get("HTauTauTree");
    //TH1D *histos[nVars];
    for(int iv =0; iv<nVars; iv++){
      cout<<"    creating branch for "<<Variables[iv].Data()<<endl;
      branches[iv]=tree->GetBranch(Variables[iv].Data());
      branches[iv]->SetAddress(&(address[iv]));
      //histoName.Form("%s_%s",Variables[iv].Data(),samples[i].Data());
      //histos[iv]=manager->GetHisto(histoName.Data());
      TString drawCommand;
      drawCommand.Form("%s>>%s",Variables[iv].Data(),histos[iv][i]->GetName());
      tree->Draw(drawCommand.Data(),Selection(selection).Data());
      if(i>=nB)histos[iv][i]->Sumw2();
      histos[iv][i]->Scale(eff);
    }

    //Use Draw instead of loop on the events in order to have TCut for selection
    /*
    for(int ien=0;ien<tree->GetEntries();ien++){
      //cout<<"         event "<<ien<<endl; 
      tree->GetEntry(ien);
      for(int iv =0; iv<nVars; iv++){//This is tooooo slow, better if I create an array of histos by myself, or if I add a search by index
	//histoName.Form("%s_%s",Variables[iv].Data(),samples[i].Data());
	//manager->GetHisto(histoName.Data())->Fill(address[i]);
	Bool_t passSelection=false;
	if(selection==0)passSelection=true;
	else if(selection==1){
	  //Additional selections...
	}
	if(passSelection)histos[iv][i]->Fill(address[iv]);//lets see if this is faster
      }//loop on variables
    }//loop on tree entries
    //scale histos for effxBR
    for(int iv =0; iv<nVars; iv++){
      histoName.Form("%s_%s",Variables[iv].Data(),samples[i].Data());
      //manager->GetHisto(histoName.Data())->Scale(eff);
      if(i>=nB)histos[iv][i]->Sumw2();
      histos[iv][i]->Scale(eff);
    }
    */
  }//loop on files

  //save all trees
  //manager->SaveAllToFile(fOut);
  //Output file
  TString outString;outString.Form("outPlotter_%d.root",selection);
  TFile *fOut = new TFile(outString.Data(),"RECREATE");

  fOut->cd();
  for(int iv =0; iv<nVars; iv++){
    for(int i =0; i<nB+nS; i++){
      histos[iv][i]->Write();
    }
  }
  //make Stack plots
  THStack *hstack[nVars];//one stack for variable
  for(int iv =0; iv<nVars; iv++){
    TString outputName;outputName.Form("stack_%s",Variables[iv].Data());
    hstack[iv]=new THStack(outputName.Data(),outputName.Data());
    for(int i =0; i<nB; i++)hstack[iv]->Add(histos[iv][i]);
    TCanvas *c = new TCanvas(Variables[iv].Data());
    c->cd();
    hstack[iv]->Draw();
    for(int i =nB; i<nB+nS; i++)histos[iv][i]->Draw("EPSAME");
    TString coutputName;coutputName.Form("%s_%d.pdf",outputName.Data(),selection);
    c->SaveAs(coutputName.Data());
    fOut->cd();
    hstack[iv]->Write();
    delete c;
  }
}
