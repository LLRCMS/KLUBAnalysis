#include <iostream>
#include <vector>
#include "ConfigParser.h"
#include "utils.h"
#include "TString.h"
#include "TChain.h"
#include "TCut.h"
#include "HistoManager.h"
#include "TTreeFormula.h"
#include "THStack.h"
#include "TCanvas.h"

using namespace std ;

//Replace histomanager with something that goes by index instead og looking for strings

//void FillHistos(int);
//TString Selection(int);

//TString Selection(int selection=0){
//  if(selection==0) return "HH_pt>0";
//  else if(selection==1)return "HH_pt>400";
//  return "";
//}

int main(int argc, char** argv){
  // check number of inpt parameters
  if (argc < 1)
    {
      cerr << "Forgot to put the cfg file --> exit " << endl ;
      return 1 ;
    }
  if (gConfigParser) return 1 ;
  gConfigParser = new ConfigParser () ;
  
  TString config ; 
  config.Form ("%s",argv[1]) ;
  if (! (gConfigParser->init (config)))
    {
      cout << ">>> parseConfigFile::Could not open configuration file " << config << endl ;
      return -1 ;
    }
  
  // get the samples to be analised
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  vector<string> sigSamplesList = gConfigParser->readStringListOption ("general::signals") ;
  vector<sample> sigSamples ;
  readSamples (sigSamples, sigSamplesList) ;

  vector<string> bkgSamplesList = gConfigParser->readStringListOption ("general::backgrounds") ;
  vector<sample> bkgSamples ;
  readSamples (bkgSamples, bkgSamplesList) ;

  vector<sample> allSamples;
  // get the selections to be applied
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<pair <TString, TCut> > selections = readCutsFile (
      gConfigParser->readStringOption ("selections::selectionsFile")
    ) ;

  for (unsigned int i = 0 ; i < selections.size () ; ++i)
    cout << selections.at (i).first << " : " << selections.at (i).second << endl ;

  // get the variables to be plotted
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<string> variablesList = gConfigParser->readStringListOption ("general::variables") ;

  //Configurables: files and vars
  const int nB = bkgSamples.size();
  const int nS = sigSamples.size();
  const int nVars =variablesList.size();
  const int nSel = selections.size();
  for(int i=0;i<nB;i++)allSamples.push_back(bkgSamples.at(i));
  for(int i=0;i<nS;i++)allSamples.push_back(sigSamples.at(i));
  //TString samples[nB+nS] ={"DY","Lambda1"};//CAUTION! Bkg before Sig!
  //TString files[nB+nS] = {"../test/test.root","../test/test.root"};
  //TString Variables[nVars] = {"HH_pt","HHKin_mass"};//same name as in the small tree
  int xup[nVars];//           = {900,900};
  int xlow[nVars];//          = {0,250};
  int bins[nVars];//          = {300,100};
  for(int iVar=0;iVar<nVars;iVar++){
    xup[iVar]=900;xlow[iVar]=100;bins[iVar]=100;
    for(int i=0;i<nS;i++){//FIXME: allSamples
      if(sigSamples.at(i).sampleChain->GetMaximum(variablesList.at(iVar).c_str())>xup[iVar])xup[iVar]=sigSamples.at(i).sampleChain->GetMaximum(variablesList.at(iVar).c_str());
      if(sigSamples.at(i).sampleChain->GetMinimum(variablesList.at(iVar).c_str())<xlow[iVar])xlow[iVar]=sigSamples.at(i).sampleChain->GetMinimum(variablesList.at(iVar).c_str());
    }
    for(int i=0;i<nB;i++){
      if(bkgSamples.at(i).sampleChain->GetMaximum(variablesList.at(iVar).c_str())>xup[iVar])xup[iVar]=bkgSamples.at(i).sampleChain->GetMaximum(variablesList.at(iVar).c_str());
      if(bkgSamples.at(i).sampleChain->GetMinimum(variablesList.at(iVar).c_str())<xlow[iVar])xup[iVar]=bkgSamples.at(i).sampleChain->GetMinimum(variablesList.at(iVar).c_str());
    }
  }
  int colors[]={kBlue,kRed,kGreen,kYellow+2,kRed+2,kMagenta,kCyan,kBlack};//add more if needed

  //Utilities
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
	histoName.Form("%s_%s_%s",variablesList.at(i).c_str(),allSamples.at(j).sampleName.Data(),selections.at(k).first.Data());
	//cout<<"adding "<<histoName.Data()<<" ..."<<endl;
	manager->AddNewHisto(histoName.Data(),histoName.Data(),bins[i],xlow[i],xup[i]);
	//cout<<"... added!"<<endl;
	//TH1F *histos=manager->GetHisto(histoName.Data());//new TH1F(histoName.Data(),histoName.Data(),bins[i],xlow[i],xup[i]);
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
    //TFile *fIn=TFile::Open(files[i].Data());
    //cout<<"processing sample "<<samples[i].Data()<<endl;
    double eff = allSamples.at(i).eff;//((TH1F*)fIn->Get("h_eff"))->GetBinContent(2)/((TH1F*)fIn->Get("h_eff"))->GetBinContent(1);
    TTree *tree = (TTree*)allSamples.at(i).sampleChain->GetTree();//fIn->Get("HTauTauTree");
    TTreeFormula* TTF[nSel];
    for(int isel=0;isel<nSel;isel++){
      TString fname;fname.Form("ttf%d",isel);
      TTF[isel]= new TTreeFormula(fname.Data(),selections.at(isel).second,tree);
    }
float weight;
tree->SetBranchAddress("MC_weight",&weight);
    //TTF->SetTree(tree);
    //TH1F *histos[nVars];
    for(int iv =0; iv<nVars; iv++){
      cout<<"    creating branch for "<<variablesList.at(iv).c_str()<<endl;
      tree->SetBranchAddress(variablesList.at(iv).c_str(),&(address[iv]));      
    }
    
    for(int ien=0;ien<tree->GetEntries();ien++){
      //cout<<"         event "<<ien<<endl; 
      tree->GetEntry(ien);
      for(int isel=0;isel<nSel;isel++){
        //cout<<isel<<endl;
	if(! TTF[isel]->EvalInstance())continue;
	for(int iv =0; iv<nVars; iv++){//This is tooooo slow, better if I create an array of histos by myself, or if I add a search by index
	  //cout<<isel<<iv<<endl;
	  histoName.Form("%s_%s_%s",variablesList.at(iv).c_str(),allSamples.at(i).sampleName.Data(),selections.at(isel).first.Data());
	  //cout<<"going to fill "<<histoName.Data()<<endl;
	//cout<<"weight "<<weight<<"  "<<address[0]<<endl;  
	manager->GetHisto(histoName.Data())->Fill(address[iv],weight);
	  //histos[iv][i]->Fill(address[iv],weight);//lets see if this is faster
	}//loop on variables
      }//loop on selections
    }//loop on tree entries

    cout<<"Filled all events"<<endl;
 
   //scale histos for effxBR
    for(int isel=0;isel<nSel;isel++){
      for(int iv =0; iv<nVars; iv++){
	histoName.Form("%s_%s_%s",variablesList.at(iv).c_str(),allSamples.at(i).sampleName.Data(),selections.at(isel).first.Data());
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
      TString outputName;outputName.Form("stack_%s_%s",variablesList.at(iv).c_str(),selections.at(isel).first.Data());
      hstack[iv+nVars*isel]=new THStack(outputName.Data(),outputName.Data());
      for(int i =0; i<nB; i++){
	histoName.Form("%s_%s_%s",variablesList.at(iv).c_str(),allSamples.at(i).sampleName.Data(),selections.at(isel).first.Data());
	hstack[iv+nVars*isel]->Add(manager->GetHisto(histoName.Data()));
      }
      TCanvas *c = new TCanvas(outputName.Data());
      c->cd();
      hstack[iv+nVars*isel]->Draw();
      for(int i =nB; i<nB+nS; i++){
	histoName.Form("%s_%s_%s",variablesList.at(iv).c_str(),allSamples.at(i).sampleName.Data(),selections.at(isel).first.Data());
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
