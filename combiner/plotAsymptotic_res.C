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

void AddText(TString channel);
void plotAsymptoticChannel(int ch);
TLegend *buildLegend();
void getLimits(TFile *f, std::vector<double> &v_mean,std::vector<double> &v_68l,std::vector<double> &v_68h,std::vector<double> &v_95l,std::vector<double> &v_95h,std::vector<double> &v_obs, bool onepointta=false);
// --------- Inputs ------- //
//TString inputDir = "cards"; //"higgsCombineTest.Asymptotic.mH125.7.root";

//custom inputs
TString input="_07mar_Resonant_noBR";//"cards_combined_18Nov";
//TString var="COMBINED";//"COMBINED";//HHKin_mass";//"mT"; //"HH_mass";// for tautau
//TString sel="";//defaultBtagMMNoIsoBBTTCut";//defaultBtagMMNoIsoBBTTCut";//"defaultBtagLLNoIsoBBTTCut";//"defaultBtagLLMbbMttNoIso";//"dijetOneBLKineMttNoIso"; //"dijethardiso";// for tautau
TString output = "_07mar_Resonant_withBR";
//TString channel="H #rightarrow hh #rightarrow bb #tau_{H} #tau_{H} + bb #tau_{#mu} #tau_{H} + bb #tau_{e} #tau_{H}"; //channel name in plot

//Plotting features
bool addATLAS=false;
const bool addObsLimit = true;

//Default inputs
bool plotMu=false;
float lambdas[]={250, 260, 270, 280, 300, 325, 350, 375, 400, 425, 450, 475, 500, 525, 550, 575, 600, 625, 650, 675, 700, 725, 750, 775, 800, 825, 850, 875, 900};
string lnames[] ={"Radion250", "Radion260", "Radion270", "Radion280", "Radion300", "Radion325", "Radion350", "Radion375", "Radion400", "Radion425", "Radion450", "Radion475", "Radion500", "Radion525", "Radion550", "Radion575", "Radion600", "Radion625", "Radion650", "Radion675", "Radion700", "Radion725", "Radion750", "Radion775", "Radion800", "Radion825", "Radion850", "Radion875", "Radion900"};
const int nLambdas = 29;
const int nXsec = 29;
//float xsections[nXsec]= {1,1,1,1,1,1,1,1,1,1,1};
//loat deflambdas[nXsec]={-4,  0,   1,  1.5,   2,   2.46, 3,    4,    10,  20};
const bool _DEBUG_ = true;
Double_t xLow = 250;
Double_t xHigh = 900.0;
Double_t yLow = 0.01;
const float xfac = 10/0.073;
Double_t yHigh = 800.; //60
TString xTitle = "m_{H} [GeV]";
//TString yTitle = "95% CL limit on #sigma(pp #rightarrow H) x BR (H #rightarrow hh #rightarrow bb#tau#tau) [fb]";
TString yTitle = "95% CL limit on #sigma(pp #rightarrow H) [fb]";
const bool logy = true;
const bool logx = false;
const bool grid = true;
const bool gridOnTop = true;
const bool points = true;
const bool isTiny = false;
int canvasX = 900;
int canvasY = 700;
double sqrts = 13.0;
double lumin = 2.7;
bool onepointta=true;
bool hybrid = false;
// ----------------------- //

TString inputSuffix[4]={"MuTau","ETau","TauTau","Combined"};
TString variables[4]={"HHKin_mass_raw","HHKin_mass_raw","HHKin_mass_raw","COMBINED"};
TString selections[4]={"defaultBtagMMNoIsoBBTTCut","defaultBtagMMNoIsoBBTTCut","defaultBtagLLNoIsoBBTTCut45",""};
TString channelOut[4]={"bb #mu#tau_{h}","bb e#tau_{h}","bb #tau_{h}#tau_{h}","bb #mu#tau_{h} + bb e#tau_{h} + bb #tau_{h}#tau_{h}"};
using namespace std;

void plotAsymptotic_res(){

  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetFrameLineWidth(3);

  for(int i=0;i<4;i++)
    plotAsymptoticChannel(i);
}

void plotAsymptoticChannel(int ch) {


  TString var = variables[ch];
  TString channel = channelOut[ch];
  TString sel = selections[ch];
  TString inputfolder="cards_"+inputSuffix[ch]+input;//"cards_MuTau_02feb_alpha";//"cards_combined_18Nov";
  TString plotName = "UpperLimit_"+inputSuffix[ch]+output;//"cards_MuTau_02feb_alpha";//"cards_combined_18Nov";

  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetFrameLineWidth(3);

  //for(int ix = 0; ix< nXsec; ix++) xsections[ix] *= xfac; //xsXBR in fb
  //TFile *inFile = new TFile(inputFile,"READ");

  // ------------------- Get Values -------------------- //
  vector<double> Val_obs, Val_mean, Val_68h, Val_68l, Val_95h, Val_95l;
  //vector<double> Val_obsh, Val_meanh, Val_68hh, Val_68lh, Val_95hh, Val_95lh;
  //vector<double> Val_obsh, Val_meanh, Val_68hh, Val_68lh, Val_95hh, Val_95lh;
  vector<double> val_hyb[6], dummy[6];

  //cout<<"Getting limits"<<endl;
  if(plotMu) {
    //yHigh=11000.;
    plotName.Append("_mu_");
    yTitle = "95% CL limit on #sigma / #sigma_{exp}";
  }
  float lim[nLambdas], limobs[nLambdas],lim68l[nLambdas],lim68h[nLambdas],lim95l[nLambdas],lim95h[nLambdas];
  float limh[nLambdas], limobsh[nLambdas],lim68lh[nLambdas],lim68hh[nLambdas],lim95lh[nLambdas],lim95hh[nLambdas];
  TString quantiles[5]={"0.025","0.16","0.5","0.84","0.975"};
  TString quantiles3[5]={"0.025","0.160","0.500","0.840","0.975"};

  for(int ifile=0;ifile<nLambdas;ifile++){
   TString filename;
   //cout<<filename.Data()<<endl;
   if(hybrid){
     for(int iq=0;iq<5;iq++){
       filename.Form("%s/%s%s%s/higgsCombineforLim_%s.HybridNew.mH125.quant%s.root",inputfolder.Data(),lnames[ifile].c_str(),sel.Data(),var.Data(),quantiles[iq].Data(),quantiles3[iq].Data());
       TFile *inFileh = TFile::Open(filename.Data());
       getLimits(inFileh,dummy[2],dummy[1],dummy[3],dummy[0],dummy[4],dummy[5],onepointta);
       //val_hyb[iq].push_back(dummy[iq].at( dummy[iq].size()-1 ) );
     }
     dummy[5].push_back(1);
   }else {
     filename.Form("%s/%s%s%s/higgsCombine%s_forLim.Asymptotic.mH125.root",inputfolder.Data(),lnames[ifile].c_str(),sel.Data(),var.Data(),lnames[ifile].c_str());
     TFile *inFile = TFile::Open(filename.Data());
     getLimits(inFile,Val_mean,Val_68l,Val_68h,Val_95l,Val_95h,Val_obs,onepointta);
   }

   //cout<<"got"<<endl;
   if(hybrid){
     Val_obs = dummy[5]; 
     Val_mean = dummy[2];
     Val_68h  = dummy[3];
     Val_68l  = dummy[1];
     Val_95h  = dummy[4]; 
     Val_95l  = dummy[0];
     lim[ifile] = Val_mean[ifile];//*valxsections;
   }

   vector<double> expExclusion,obsExclusion;

   //cout<<"point "<<ifile<<endl;
   float valxsections = xfac;
   for(int ix = 0; ix< nXsec; ix++){
    //xsections[ix];
    if (plotMu) valxsections =1;
   }
   lim[ifile] = Val_mean[ifile]*valxsections;
   cout<<"r="<<Val_mean[ifile]<<"  exp (obs) xsection>"<<valxsections*Val_mean[ifile]<<"/"<<valxsections<<" ("<<Val_obs[ifile]*valxsections<<")"<<endl;
   lim68l[ifile] = Val_68l[ifile]*valxsections; //min( Val_68l[ifile], Val_68h[ifile])*valxsections;
   lim95l[ifile] = Val_95l[ifile]*valxsections; //min( Val_95l[ifile], Val_95h[ifile])*valxsections;
   lim68h[ifile] = Val_68h[ifile]*valxsections; //max( Val_68h[ifile], Val_68l[ifile])*valxsections;
   lim95h[ifile] = Val_95h[ifile]*valxsections; //max( Val_95h[ifile], Val_95l[ifile])*valxsections;

   if(lim[ifile] < 1.0) expExclusion.push_back(lambdas[ifile]);
   if( addObsLimit) limobs[ifile] = Val_obs[ifile]*valxsections;
 }
 // ------------------- Change Values to Arrays -------------------- //
  // ------------------- Draw  -------------------- //
 TCanvas *c = new TCanvas("c","c",canvasX,canvasY);
 c->cd();
 c->SetFrameLineWidth(3);

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
 //TGraph *grExp = new TGraph(nMassEff,lambdas,xsections);

 grObs->SetLineWidth(3);
 grObs->SetLineColor(1);
 grObs->SetLineStyle(1);
 grObs->SetLineColor(kBlack);
 grObs->SetMarkerStyle(20);
 grObs->SetMarkerSize(0.8);

 //grExp->SetLineColor(kRed);
 //grExp->SetLineWidth(3);

 gr->SetLineStyle(2);
 gr->SetLineWidth(3);
 gr->SetLineColor(kBlue);
 gr->SetMarkerColor(kBlue);
 gr->SetMarkerStyle(24);
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
	/*
  TLegend * box2 = new TLegend(0.21,0.12,0.45,0.40);
  box2->SetFillColor(0);
  //box2->SetBorderSize(0);
  if (addObsLimit){ box2->AddEntry(grObs,"1D Fit Observed Asym. CLs","l"); }
  box2->AddEntry(gr,"1D Fit Expected Asym. CLs","l");
  //box2->AddEntry(grshade_68,"68% expectation","f");
  //box2->AddEntry(grshade_95,"95% expectation","f");
  box2->AddEntry(grshade_68,"Expected #pm 1#sigma","lf");
  box2->AddEntry(grshade_95,"Expected #pm 2#sigma","lf");
  //box2->AddEntry(oneLine,"#sigma / #sigma_{SM}","l");
*/
		
  double ptLow= 0.6, ptHigh = 0.8;
		
 
  if(grid) pad1->SetGrid();

  pad1->cd();
  TH1F *hr = pad1->DrawFrame(xLow,yLow,xHigh,yHigh);
  if(logy)pad1->SetLogy();

  
  grshade_95->Draw("f");
  grshade_68->Draw("f");
  gr->Draw("PL");
  gr->Print();  
//if(!plotMu)grExp->Draw("l");
  if(addObsLimit)
    {
      if(points)grObs->Draw("lP");
      else grObs->Draw("l");
    }
  // if (addATLAS){
  //   TGraph *ATLAS = new TGraph(0);
  //   ATLAS->SetPoint(0,1,130*34.3*0.073);
  //   ATLAS->SetMarkerStyle(29);
  //   ATLAS->SetMarkerColor(4);
  //   ATLAS->SetMarkerSize(2);
  //   ATLAS->Draw("p");
  // }


  hr->GetXaxis()->SetLabelSize(0.045);
  hr->GetYaxis()->SetLabelSize(0.045);
  hr->GetXaxis()->SetTitle(xTitle);
  hr->GetYaxis()->SetTitle(yTitle);
  // hr->GetXaxis()->SetTitleSize(0.05);
  // hr->GetYaxis()->SetTitleSize(0.05);
  hr->GetXaxis()->SetTitleOffset(0.9);    
  hr->GetYaxis()->SetTitleOffset(1.2);    

  
  
  c->Update();
  if(gridOnTop)gPad->RedrawAxis("g");
  TLegend * box2= buildLegend();
  box2->Draw();
  //pad2->cd();
  //TH1F *hr2 = pad2->DrawFrame(105.0,yLow,180.0,yHigh);
  //hr2->GetYaxis()->SetTitleOffset(0.06);

  AddText(channel);

  plotName.Append(sel.Data());
  plotName.Append(var.Data());

  TString outfileName=plotName.Data();
  outfileName.Append(".eps");
  c->SaveAs(outfileName.Data());
  outfileName=plotName.Data();
  outfileName.Append(".pdf");
  c->SaveAs(outfileName.Data());
  outfileName=plotName.Data();
  outfileName.Append(".root");
  c->SaveAs(outfileName.Data());
  outfileName=plotName.Data();
  outfileName.Append(".png");
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

void AddText(TString channel){

    TPaveText *pt = new TPaveText(0.15902,0.9066667,0.3563474,0.9674074,"brNDC");
   pt->SetBorderSize(0);
   pt->SetTextAlign(12);
   pt->SetTextFont(42);
   pt->SetTextSize(0.05);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->AddText("CMS Preliminary");
   //pt->Draw();
   TPaveText *pt2 = new TPaveText(0.623,0.9066667,0.8997773,0.957037,"brNDC");
   pt2->SetBorderSize(0);
   pt2->SetFillColor(0);
   pt2->SetTextSize(0.035);
   pt2->SetTextFont(42);
   pt2->SetFillStyle(0);
   //pt2->SetTextAlign();
   pt2->AddText(" L = 2.7 fb^{-1} (13 TeV)");

   TPaveText *pt4 = new TPaveText(0.1819196,0.8080357,0.7008929,0.8675595,"brNDC");
   pt4->SetTextAlign(12);
   pt4->SetFillColor(0);
   pt4->SetTextFont(42);
   pt4->AddText(channel.Data()); 

   pt->Draw();
   pt2->Draw();
   pt4->Draw("SAME");

}

TLegend *buildLegend(){
  //TLegend *leg = new TLegend(0.5803571,0.110119,0.8928571,0.3452381,NULL,"brNDC");0.21,0.12,0.45,0.40
  TLegend *leg = new TLegend(0.17,0.12,0.35,0.30,NULL,"brNDC");
  leg->SetBorderSize(1);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  TLegendEntry *entry;
  if(addObsLimit){
     entry=leg->AddEntry("NULL","Observed CLs","lp");
     ci = TColor::GetColor("#0000ff");
     entry->SetLineColor(1);
     entry->SetLineStyle(1);
     entry->SetLineWidth(3);
     entry->SetMarkerColor(1);
     entry->SetMarkerStyle(20);
     entry->SetMarkerSize(1);
     entry->SetTextFont(42);
  }
  entry=leg->AddEntry("NULL","Expected CLs","lp");
  ci = TColor::GetColor("#0000ff");
  entry->SetLineColor(ci);
  entry->SetLineStyle(2);
  entry->SetLineWidth(3);
  entry->SetMarkerColor(kBlue);
  entry->SetMarkerStyle(24);
  entry->SetMarkerSize(1);
  entry->SetTextFont(42);
  entry=leg->AddEntry("NULL","Expected #pm 1#sigma","lf");
  ci = TColor::GetColor("#00ff00");
  entry->SetFillColor(ci);
  entry->SetFillStyle(1001);
  ci = TColor::GetColor("#0000ff");
  entry->SetLineColor(ci);
  entry->SetLineStyle(2);
  entry->SetLineWidth(3);
  entry->SetTextFont(42);
  entry=leg->AddEntry("NULL","Expected #pm 2#sigma","lf");
  ci = TColor::GetColor("#ffff00");
  entry->SetFillColor(ci);
  entry->SetFillStyle(1001);
  ci = TColor::GetColor("#0000ff");
  entry->SetLineColor(ci);
  entry->SetLineStyle(2);
  entry->SetLineWidth(3);
  entry->SetTextFont(42);
  return leg;
   //leg->Draw();
   // <- end Legend
}

void getLimits(TFile *f, std::vector<double> &v_mean,std::vector<double> &v_68l,std::vector<double> &v_68h,std::vector<double> &v_95l,std::vector<double> &v_95h,std::vector<double> &v_obs,bool onepointta)
{
  //cout<<"getting tree from"<<endl;
  //f->Print();
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
    cout<<"entry "<<i<< " limit: " << limit << " quantileExpected: " << quant << endl;  
    if(quant>-1.01&&quant<-0.99)  v_obs.push_back(limit);
    else if(quant>0.024 && quant<0.026) v_95l.push_back(limit);
    else if(quant>0.15  && quant<0.17 ) v_68l.push_back(limit);
    else if(quant>0.49  && quant<0.51 ) v_mean.push_back(limit);
    else if(quant>0.83  && quant<0.85 ) v_68h.push_back(limit);
    else if(quant>0.974 && quant<0.976) v_95h.push_back(limit);
    else {cout<<"Error! Unknown Quantile =  " << quant << endl;}

  }
 }
