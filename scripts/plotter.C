//I must add the XSection x BR from the histogram
//gROOT->LoadMacro("../src/HistoManager.cc+");

void FillHistos(int);
TString Selection(int);

TString Selection(int selection=0){
  if(selection==0) return "HH_pt>0";
  else if(selection==1)return "HH_pt>400";
  return "";
}

void plotter(){
  gROOT->LoadMacro("../src/HistoManager.cc+");

  //Configurables: files and vars
  const int nB = 1;
  const int nS = 1;
  const int nVars =2;
  const int nSel = 2;
  TString samples[nB+nS] ={"DY","Lambda1"};//CAUTION! Bkg before Sig!
  TString files[nB+nS] = {"../test/test.root","../test/test.root"};
  TString Variables[nVars] = {"HH_pt","HHKin_mass"};//same name as in the small tree
  int xup[nVars]           = {900,900};
  int xlow[nVars]          = {0,250};
  int bins[nVars]          = {300,100};
  int colors[]={kBlue,kRed,kGreen,kYellow+2,kRed+2,kMagenta,kCyan,kBlack};//add more if needed

  //Utilities
  TBranch *branches[nVars];
  float address[nVars];for(int i =0; i<nVars; i++)address[i]=0;
  TString histoName;
  //TH1F *histos[nVars][nS+nB];
  HistoManager *manager = new HistoManager("test");
  //TFile *fIn[nS+nB]; for(int i=0;i<nS+nB;i++)fIn[i]= TFile::Open(files[i].Data());

  //Create histograms
  //cout<<"Calling histoManager..."<<endl;
  //cout<<"... called"<<endl;
  for(int i =0; i<nVars; i++){
    for(int j =0; j<nB+nS; j++){
      for(int k=0;k<nSel;k++){
	histoName.Form("%s_%s_%d",Variables[i].Data(),samples[j].Data(),k);
	//cout<<"adding "<<histoName.Data()<<" ..."<<endl;
	manager->AddNewHisto(histoName.Data(),histoName.Data(),bins[i],xlow[i],xup[i]);
	//cout<<"... added!"<<endl;
	TH1F *histos=manager->GetHisto(histoName.Data());//new TH1F(histoName.Data(),histoName.Data(),bins[i],xlow[i],xup[i]);
	if(j<nB){//background
	  manager->GetHisto(histoName.Data())->SetFillStyle(1001);
	  manager->GetHisto(histoName.Data())->SetFillColor(colors[j]);
	  manager->GetHisto(histoName.Data())->SetLineColor(colors[j]);
	}else{
	  manager->GetHisto(histoName.Data())->SetFillStyle(0);
	  manager->GetHisto(histoName.Data())->SetMarkerColor(colors[j]);
	  manager->GetHisto(histoName.Data())->SetLineColor(colors[j]);
	  manager->GetHisto(histoName.Data())->SetMarkerStyle(20);
	}
      }  
    }
  }
  //cout<<"Called"<<endl;

  //Loop on the small trees and fill histograms
  for(int i =0; i<nB+nS; i++){
    TFile *fIn=TFile::Open(files[i].Data());
    //cout<<"processing sample "<<samples[i].Data()<<endl;
    double eff = ((TH1F*)fIn->Get("h_eff"))->GetBinContent(2)/((TH1F*)fIn->Get("h_eff"))->GetBinContent(1);
    TTree *tree = (TTree*)fIn->Get("HTauTauTree");
    TTreeFormula* TTF[nSel];
    for(int isel=0;isel<nSel;isel++){
      TString fname;fname.Form("ttf%d",isel);
      TTF[isel]= new TTreeFormula(fname.Data(),Selection(isel).Data(),tree);
    }
    TBranch *mcweight = tree->GetBranch("MC_weight");
    float weight;
    mcweight->SetAddress(&weight);
    //TTF->SetTree(tree);
    //TH1F *histos[nVars];
    for(int iv =0; iv<nVars; iv++){
      cout<<"    creating branch for "<<Variables[iv].Data()<<endl;
      branches[iv]=tree->GetBranch(Variables[iv].Data());
      branches[iv]->SetAddress(&(address[iv]));
    }
    
    for(int ien=0;ien<tree->GetEntries();ien++){
      //cout<<"         event "<<ien<<endl; 
      tree->GetEntry(ien);
      for(int isel=0;isel<nSel;isel++){
	if(! TTF[isel]->EvalInstance())continue;
	for(int iv =0; iv<nVars; iv++){//This is tooooo slow, better if I create an array of histos by myself, or if I add a search by index
	  //cout<<isel<<iv<<endl;
	  histoName.Form("%s_%s_%d",Variables[iv].Data(),samples[i].Data(),isel);
	  //cout<<"going to fill "<<histoName.Data()<<endl;
	  manager->GetHisto(histoName.Data())->Fill(address[iv]);
	  //histos[iv][i]->Fill(address[iv],weight);//lets see if this is faster
	}//loop on variables
      }//loop on selections
    }//loop on tree entries

    cout<<"Filled all events"<<endl;
 
   //scale histos for effxBR
    for(int isel=0;isel<nSel;isel++){
      for(int iv =0; iv<nVars; iv++){
	histoName.Form("%s_%s_%d",Variables[iv].Data(),samples[i].Data(),isel);
	if(i>=nB)manager->GetHisto(histoName.Data())->Sumw2();
	manager->GetHisto(histoName.Data())->Scale(eff);
	//if(i>=nB)histos[iv][i]->Sumw2();
	//histos[iv][i]->Scale(eff);
      }
      delete TTF[isel];
    }
    //delete TTF;  
  }//loop on files

  
  //Output file
  //fOut->cd();
  //for(int iv =0; iv<nVars; iv++){
  //  for(int i =0; i<nB+nS; i++){
  //    histos[iv][i]->Write();
  //  }
  //}

  TString outString;outString.Form("outPlotter.root");
  TFile *fOut = new TFile(outString.Data(),"RECREATE");
  manager->SaveAllToFile(fOut);

  //make Stack plots
  THStack *hstack[nVars*nSel];//one stack for variable
  for(int isel=0;isel<nSel;isel++){
    for(int iv =0; iv<nVars; iv++){
      TString outputName;outputName.Form("stack_%s_%d",Variables[iv].Data(),isel);
      hstack[iv+nVars*isel]=new THStack(outputName.Data(),outputName.Data());
      for(int i =0; i<nB; i++){
	histoName.Form("%s_%s_%d",Variables[iv].Data(),samples[i].Data(),isel);
	hstack[iv+nVars*isel]->Add(manager->GetHisto(histoName.Data()));
      }
      TCanvas *c = new TCanvas(outputName.Data());
      c->cd();
      hstack[iv+nVars*isel]->Draw();
      for(int i =nB; i<nB+nS; i++){
	histoName.Form("%s_%s_%d",Variables[iv].Data(),samples[i].Data(),isel);
	manager->GetHisto(histoName.Data())->Draw("EPSAME");
      }
      TString coutputName;coutputName.Form("%s.pdf",outputName.Data());
      c->SaveAs(coutputName.Data());
      fOut->cd();
      hstack[iv+nVars*isel]->Write();
      delete c;
    }
  }
}
