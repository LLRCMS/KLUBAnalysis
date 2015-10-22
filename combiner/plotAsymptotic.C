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


void getLimits(TFile *f, std::vector<double> &v_mean,std::vector<double> &v_68l,std::vector<double> &v_68h,std::vector<double> &v_95l,std::vector<double> &v_95h,std::vector<double> &v_obs, bool onepointta=false);
// --------- Inputs ------- //
//TString inputDir = "cards"; //"higgsCombineTest.Asymptotic.mH125.7.root";

//custom inputs
TString inputfolder="cards_mutau_19oct";
TString var= "HH_mass";//"mT"; //"HH_mass";// for tautau
TString sel="baselineisoBtagCutM"; //"dijethardiso";// for tautau
TString plotName = "UpperLimit_MuTau_19oct_";

//Default inputs
float lambdas[]={-4,1,2.46,20};
const int nLambdas = 4;
const int nXsec = 10;
float xsections[nXsec]= {12, 2.13, 1,  0.66, 0.47, 0.42, 0.53, 0.79, 50,  105};
float deflambdas[nXsec]={-4,  0,   1,  1.5,   2,   2.46, 3,    4,    10,  20};
const bool addObsLimit = true;
const bool _DEBUG_ = true;
Double_t xLow = -5;
Double_t xHigh = 21.0;
Double_t yLow = 1;
const float xfac = 34.3*0.073;
Double_t yHigh = xfac*1000.;
TString xTitle = "#Lambda [GeV]";
TString yTitle = "95% CL limit on #sigma #times BR [fb]";
const bool logy = true;
const bool logx = false;
const bool grid = true;
const bool gridOnTop = true;
const bool points = true;
const bool isTiny = false;
int canvasX = 900;
int canvasY = 700;
double sqrts = 13.0;
//Double_t lumi = 1.0;
bool onepointta=true;
// ----------------------- //


using namespace std;

void plotAsymptotic() {

  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadTopMargin(0.1);
  for(int ix = 0; ix< nXsec; ix++) xsections[ix] *= xfac; //xsXBR in fb
  //TFile *inFile = new TFile(inputFile,"READ");

  // ------------------- Get Values -------------------- //
  vector<double> Val_obs, Val_mean, Val_68h, Val_68l, Val_95h, Val_95l;
  //cout<<"Getting limits"<<endl;

  float lim[nLambdas], limobs[nLambdas],lim68l[nLambdas],lim68h[nLambdas],lim95l[nLambdas],lim95h[nLambdas];
  for(int ifile=0;ifile<nLambdas;ifile++){
   TString filename;
   filename.Form("%s/lambda%.2f%s%s/higgsCombineLambda%.2f_forLim.Asymptotic.mH125.root",inputfolder.Data(),lambdas[ifile],sel.Data(),var.Data(),lambdas[ifile]);
   TFile *inFile = TFile::Open(filename.Data());
   getLimits(inFile,Val_mean,Val_68l,Val_68h,Val_95l,Val_95h,Val_obs,onepointta);
   //cout<<"got"<<endl;

   vector<double> expExclusion,obsExclusion;

   //cout<<"point "<<ifile<<endl;
   float valxsections = 0;
   for(int ix = 0; ix< nXsec; ix++){
    if (fabs(lambdas[ifile]-deflambdas[ix])<0.1)valxsections=xsections[ix];
   }
   cout<<valxsections<<endl;
   lim[ifile] = Val_mean[ifile]*valxsections;
   cout<<Val_mean[ifile]<<endl;
   lim68l[ifile] = min( Val_68l[ifile], Val_68h[ifile])*valxsections;
   lim95l[ifile] = min( Val_95l[ifile], Val_95h[ifile])*valxsections;
   lim68h[ifile] = max( Val_68h[ifile], Val_68l[ifile])*valxsections;
   lim95h[ifile] = max( Val_95h[ifile], Val_95l[ifile])*valxsections;

   if(lim[ifile] < 1.0) expExclusion.push_back(lambdas[ifile]);
   if( addObsLimit) limobs[ifile] = Val_obs[ifile]*valxsections;
 }
 // ------------------- Change Values to Arrays -------------------- //
  // ------------------- Draw  -------------------- //
 TCanvas *c = new TCanvas("c","c",canvasX,canvasY);
 c->cd();
 //TPad *pad2 = new TPad("pad2", "foo", 0, 0,   1, 0.3);
 TPad *pad1 = new TPad("pad1", "foo", 0, 0.0, 1, 1);//0.3 al posto di 0.0
 //pad2->SetBottomMargin(0.22);
 pad1->Draw();
 //pad2->Draw();
 //pad2->SetGrid()
 int nMassEff = nLambdas;

 TGraph *gr = new TGraph(nMassEff, lambdas, lim);
 TGraph* grshade_68 = new TGraph(2*nMassEff);
 TGraph* grshade_95 = new TGraph(2*nMassEff);
 TGraph *grObs = new TGraph(nMassEff, lambdas, limobs);
 TGraph *grExp = new TGraph(nXsec,deflambdas,xsections);

 grObs->SetLineWidth(3);
 grObs->SetLineColor(kBlack);
 grObs->SetMarkerStyle(21);
 grObs->SetMarkerSize(0.8);

 grExp->SetLineColor(kRed);
 grExp->SetLineWidth(3);

 gr->SetLineStyle(2);
 gr->SetLineWidth(3);
 gr->SetLineColor(kBlue);
 grshade_68->SetFillColor(kGreen);
 grshade_95->SetFillColor(kYellow);		
 grshade_68->SetLineStyle(1);
 grshade_95->SetLineStyle(1);
 grshade_68->SetLineWidth(3);
 grshade_95->SetLineWidth(3);
 grshade_68->SetLineColor(kBlue);
 grshade_95->SetLineColor(kBlue);

 int sizeV = nMassEff;
 for (int a = 0; a < nMassEff; a++)
 {
  grshade_68->SetPoint(a,lambdas[a],lim68l[a]);
  grshade_68->SetPoint(sizeV+a,lambdas[nMassEff-a-1],lim68h[nMassEff-a-1]);
  grshade_95->SetPoint(a,lambdas[a],lim95l[a]);
  grshade_95->SetPoint(nMassEff+a,lambdas[nMassEff-a-1],lim95h[nMassEff-a-1]);
  }

  // --------------- Low Mass Zoom -------------- //
	
  TLegend * box2 = new TLegend(0.51,0.1,0.88,0.380);
  box2->SetFillColor(0);
  //box2->SetBorderSize(0);
  if (addObsLimit){ box2->AddEntry(grObs,"1D Fit Observed Asym. CLs","l"); }
  box2->AddEntry(gr,"1D Fit Expected Asym. CLs","l");
  //box2->AddEntry(grshade_68,"68% expectation","f");
  //box2->AddEntry(grshade_95,"95% expectation","f");
  box2->AddEntry(grshade_68,"Expected #pm 1#sigma","lf");
  box2->AddEntry(grshade_95,"Expected #pm 2#sigma","lf");
  //box2->AddEntry(oneLine,"#sigma / #sigma_{SM}","l");

		
  double ptLow= 0.6, ptHigh = 0.8;
		
  TPaveText *pt = new TPaveText(ptLow,0.44,ptHigh,0.5,"NDC");
  pt->SetFillColor(0);
  pt->SetTextFont(42);
  pt->AddText("CMS Preliminary");
  TPaveText *pt4 = new TPaveText(ptLow,0.38,ptHigh,0.44,"NDC");
  pt4->SetFillColor(0);
  pt4->SetTextFont(42);
  pt4->AddText("H #rightarrow hh #rightarrow bb #tau #tau"); 

  //TPaveText *pt2 = new TPaveText(0.69,0.94,0.98,0.99,"NDC");
  TPaveText *pt2 = new TPaveText(ptLow,0.76,ptHigh,0.8,"NDC");
  pt2->SetFillColor(0);
  pt2->SetTextFont(42);
  char lum[192];
  //sprintf(lum," #sqrt{s} = 8 TeV, L = %.2f fb^{-1}",5.261);
  sprintf(lum," #sqrt{s} = 13 TeV, L = %.2f pb^{-1}",295.);
  pt2->AddText(lum); 
  //TPaveText *pt3 = new TPaveText(0.69,0.90,0.98,0.95,"NDC");
 

  if(grid) pad1->SetGrid();

  pad1->cd();
  TH1F *hr = pad1->DrawFrame(xLow,yLow,xHigh,yHigh);
  if(logy)pad1->SetLogy();
  
  grshade_95->Draw("f");
  grshade_68->Draw("f");
  gr->Draw("l");
  grExp->Draw("l");
  if(addObsLimit)
    {
      if(points)grObs->Draw("lP");
      else grObs->Draw("l");
    }


  hr->GetXaxis()->SetTitle(xTitle);
  hr->GetYaxis()->SetTitle(yTitle);
  hr->GetYaxis()->SetTitleOffset(1.2);		

  
  
  c->Update();
  if(gridOnTop)gPad->RedrawAxis("g");
  box2->Draw();

  //pad2->cd();
  //TH1F *hr2 = pad2->DrawFrame(105.0,yLow,180.0,yHigh);
  //hr2->GetYaxis()->SetTitleOffset(0.06);

  pt->Draw("SAME");
  //pt2->Draw("SAME");
  //pt3->Draw("SAME");
  pt4->Draw("SAME");

  plotName.Append(sel.Data());
  plotName.Append(var.Data());

  TString outfileName=plotName.Data();
  outfileName.Append(".eps");
  c->SaveAs(outfileName.Data());
  outfileName=plotName.Data();
  outfileName.Append(".png");
  c->SaveAs(outfileName.Data());
  outfileName=plotName.Data();
  outfileName.Append(".root");
  c->SaveAs(outfileName.Data());


  // --------------- Full Mass Range ---------------- //
	/*
  TLegend * box3 = new TLegend(0.4,0.62,0.8,0.82);
  box3->SetFillColor(0);
  //box3->SetBorderSize(0);
  if (addObsLimit){ box3->AddEntry(grObs,"1D Fit Observed Asym. CLs","l"); }
  box3->AddEntry(gr,"1D Fit Expected Asym. CLs","l");
  box3->AddEntry(grshade_68,"Expected #pm 1#sigma","lf");
  box3->AddEntry(grshade_95,"Expected #pm 2#sigma","lf");
  //box3->AddEntry(oneLine,"#sigma / #sigma_{SM}","l");
  */
	
  }



void getLimits(TFile *f, std::vector<double> &v_mean,std::vector<double> &v_68l,std::vector<double> &v_68h,std::vector<double> &v_95l,std::vector<double> &v_95h,std::vector<double> &v_obs,bool onepointta)
{
  cout<<"getting tree"<<endl;
  TTree *tree =(TTree*)f->Get("limit");
  cout<<"entries "<<tree->GetEntries()<<endl;
  double limit;
  float quant;
  //tree->SetBranchAddress("mh",&mh);
  tree->SetBranchAddress("limit",&limit);
  tree->SetBranchAddress("quantileExpected",&quant);

  for(int i=0;i<tree->GetEntries();i++)
  {
    tree->GetEntry(i);
    //if(_DEBUG_)cout << "mH: " << mh << " limit: " << limit << " quantileExpected: " << quant << endl;  
    if(quant>-1.01&&quant<-0.99)  v_obs.push_back(limit);
    else if(quant>0.024 && quant<0.026) v_95l.push_back(limit);
    else if(quant>0.15  && quant<0.17 ) v_68l.push_back(limit);
    else if(quant>0.49  && quant<0.51 ) v_mean.push_back(limit);
    else if(quant>0.83  && quant<0.85 ) v_68h.push_back(limit);
    else if(quant>0.974 && quant<0.976) v_95h.push_back(limit);
    else {cout<<"Error! Unknown Quantile =  " << quant << endl;}

  }
 }
