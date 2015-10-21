#include "TH1F.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <vector>
#include "TCutG.h"
#include "TFile.h"
#include "TH2.h"
#include "TPad.h"

//Capire come gestire lambdas decimali


double getLimits(TFile *f);
// --------- Inputs ------- //
//TString inputDir = "cards"; //"higgsCombineTest.Asymptotic.mH125.7.root";

//Inputs
TString outputName="CutTable_btag";//"CutTable_tautau_19oct";

//Global variables
float lambdas[]= {-4,1,2.46,20};
TString baselines[]={"btag0","btag1","btag2","btag3","btag4","btag5","btag6","btag7","btag8","btag89","btag95","btag6b89","btag8b89"};//{"baselineiso", "dijethardiso", "baselineisoBtagCutM", "baselineisoBtagCutMMbbMtt", "dijethardisoBtagCutMMbbMtt"};
TString variables[]={"HH_mass", "mT"};//{"HH_pt", "HH_mass", "tauH_mass", "met_et", "mT", "bH_mass"};
const int nLambdas = 4;
const int nVars = 2;
const int nBase = 13;
// ----------------------- //


using namespace std;

void plotCutTable() {

gStyle->SetPalette(5); //greyscale
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetOptStat(0);
  //TFile *inFile = new TFile(inputFile,"READ");

  // ------------------- Get Values -------------------- //
  cout<<"Getting limits"<<endl;
  TString name = outputName.Data();
  name.Append(".root");
  TFile *fout = new TFile(name.Data(),"RECREATE");
  for(int ifile=0;ifile<nLambdas;ifile++){
    TString name; name.Form("h2d_L%.2f",lambdas[ifile]); 
    cout<<name.Data()<<endl;
    TH2F *h2d = new TH2F(name.Data(),name.Data(),nBase,0,nBase,nVars,0,nVars);
    for(int ibin=0;ibin<nBase;ibin++)h2d->GetXaxis()->SetBinLabel(ibin+1,baselines[ibin]);
    for(int ibin=0;ibin<nVars;ibin++)h2d->GetYaxis()->SetBinLabel(ibin+1,variables[ibin]);
    for(int ibase=0;ibase<nBase;ibase++){
      for(int ivar=0;ivar<nVars;ivar++){
       TString filename;
       filename.Form("cards_mutau_testbtag/lambda%.2f%s%s/higgsCombineLambda%.2f.Asymptotic.mH125.root",lambdas[ifile],baselines[ibase].Data(),variables[ivar].Data(),lambdas[ifile]);
       TFile *inFile = TFile::Open(filename.Data());
       h2d->SetBinContent(ibase+1,ivar+1,getLimits(inFile));
       //cout<<"got"<<endl;
      }
    }
    TString slambda;slambda.Form("c%.2f",lambdas[ifile]);
    TCanvas *c = new TCanvas(slambda,slambda);
    c->cd();
    h2d->Draw("COLTEXT");
    name = "";outputName.Data();
    name.Form("%s_lambda%.2f.pdf",outputName.Data(),lambdas[ifile]);
    c->SaveAs(name.Data());
    fout->cd();
    h2d->Write();
  }
  fout->Close();
}


double getLimits(TFile *f)
{
  cout<<"getting tree"<<endl;
  TTree *tree =(TTree*)f->Get("limit");
  //cout<<"entries "<<tree->GetEntries()<<endl;
  double limit;
  float quant;
  //tree->SetBranchAddress("mh",&mh);
  tree->SetBranchAddress("limit",&limit);
  tree->SetBranchAddress("quantileExpected",&quant);

  for(int i=0;i<tree->GetEntries();i++)
  {
    tree->GetEntry(i);
    //if(_DEBUG_)cout << "mH: " << mh << " limit: " << limit << " quantileExpected: " << quant << endl;  
    if(quant>0.49&&quant<0.51)  return limit;
    if(tree->GetEntries()==1 )return limit;

  }
  return -1;
 }
