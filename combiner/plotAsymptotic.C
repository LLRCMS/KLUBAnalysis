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
// channels: 0=mutau,1=etau,2=tautau,3=combined (magari fare enum)

void getExpLine(TVirtualPad *);
void getLimits(TFile *f, std::vector<double> &v_mean,std::vector<double> &v_68l,std::vector<double> &v_68h,std::vector<double> &v_95l,std::vector<double> &v_95h,std::vector<double> &v_obs, bool onepointta=false);
void plotAsymptoticChannel(int);
TLegend *buildLegend();
// --------- Inputs ------- //
//TString inputDir = "cards"; //"higgsCombineTest.Asymptotic.mH125.7.root";
TString input = "_02feb_vismass_L8L8L";

//Plotting features
bool addATLAS=false;
const bool addObsLimit = false;
const bool hybrid = true;
const bool asympt = false;

//Default inputs
bool plotMu=false;
float lambdas[]={-4,1,2.46,20};
string lnames[] ={"Lambdam4","Lambda1","Lambda2dot46","Lambda20"};
const int nLambdas = 4;
const int nXsec = 10;
const float xfac = 37.9*0.073;//o 0.073?
const bool _DEBUG_ = true;
Double_t xLow = -5;
Double_t xHigh = 21.0;
Double_t yLow = 1;
Double_t yHigh = 100000.;
TString xTitle = "k_{#lambda}=#lambda_{hhh}/#lambda_{hhh}^{SM}";
TString yTitle = "95% CL limit on #sigma_{pp#rightarrow hh} #times BR(hh#rightarrow bb#tau#tau) [fb]";
const bool logy = true;
const bool logx = false;
const bool grid = true;
const bool gridOnTop = true;
const bool points = true;
const bool isTiny = false;
int canvasX = 900;
int canvasY = 700;
double sqrts = 13.0;
double lumin = 2.63;
bool onepointta=true;
// ----------------------- //

//-----Other settings-----//
TString inputSuffix[4]={"MuTau","ETau","TauTau","Combined"};
TString variables[4]={"HH_mass","HH_mass","HH_mass","COMBINED"};
TString selections[4]={"defaultBtagLLNoIsoBBTTCutKine80","defaultBtagLLNoIsoBBTTCutKine80","defaultBtagLLNoIsoBBTTCut",""};
TString channelOut[4]={"#tau_{#mu} #tau_{H}","#tau_{e} #tau_{H}","#tau_{H} #tau_{H}","#tau_{#mu} #tau_{H} + bb #tau_{e} #tau_{H} + bb #tau_{H} #tau_{H}"};
using namespace std;

void plotAsymptotic(){
for(int i=0;i<4;i++)
  plotAsymptoticChannel(i);
}

void plotAsymptoticChannel(int ichannel) {

//custom inputs
  //TString inputfolder="cards_Combined_02feb_alpha";//"cards_MuTau_02feb_alpha";//"cards_combined_18Nov";
  //TString var= "COMBINED"; //"HHKin_mass";//"mT"; //"HH_mass";// for tautau
  //TString sel="";//"defaultBtagLLNoIsoBBTTCutKineOld90";//"defaultBtagLLMbbMttNoIso";//"dijetOneBLKineMttNoIso"; //"dijethardiso";// for tautau
  //TString plotName = "UpperLimit_02feb_combined";
  //TString channel="hh #rightarrow bb #tau_{#mu} #tau_{H} + bb #tau_{e} #tau_{H} + bb #tau_{H} #tau_{H}"; //channel name in plot
  TString inputfolder="cards_"+inputSuffix[ichannel]+input;//"cards_MuTau_02feb_alpha";//"cards_combined_18Nov";
  TString var=variables[ichannel]; //"HHKin_mass";//"mT"; //"HH_mass";// for tautau
  TString sel=selections[ichannel];//"defaultBtagLLNoIsoBBTTCutKineOld90";//"defaultBtagLLMbbMttNoIso";//"dijetOneBLKineMttNoIso"; //"dijethardiso";// for tautau
  TString plotName ="UpperLimit_"+inputSuffix[ichannel]+input;
  TString channel="hh #rightarrow bb "+channelOut[ichannel]; //channel name in plot

  float xsections[nXsec]= {12.35, 2.13, 1,  0.66, 0.47, 0.42, 0.53, 0.79, 50,  90.11};
  float deflambdas[nXsec]={-4,  0,   1,  1.5,   2,   2.46, 3,    4,    10,  20};

  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadTopMargin(0.1);
  for(int ix = 0; ix< nXsec; ix++) xsections[ix] *= xfac; //xsXBR in fb
  //TFile *inFile = new TFile(inputFile,"READ");

  // ------------------- Get Values -------------------- //
  vector<double> Val_obs, Val_mean, Val_68h, Val_68l, Val_95h, Val_95l;
  vector<double> Val_obsh, Val_meanh, Val_68hh, Val_68lh, Val_95hh, Val_95lh;
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
   filename.Form("%s/%s%s%s/higgsCombine%s_forLim.Asymptotic.mH125.root",inputfolder.Data(),lnames[ifile].c_str(),sel.Data(),var.Data(),lnames[ifile].c_str());
   TFile *inFile = TFile::Open(filename.Data());
   getLimits(inFile,Val_mean,Val_68l,Val_68h,Val_95l,Val_95h,Val_obs,onepointta);

   if(hybrid){
     for(int iq=0;iq<5;iq++){
       filename.Form("%s/%s%s%s/higgsCombineforLim_%s.HybridNew.mH125.quant%s.root",inputfolder.Data(),lnames[ifile].c_str(),sel.Data(),var.Data(),quantiles[iq].Data(),quantiles3[iq].Data());
       TFile *inFileh = TFile::Open(filename.Data());
       getLimits(inFileh,dummy[2],dummy[1],dummy[3],dummy[0],dummy[4],dummy[5],onepointta);
       val_hyb[iq].push_back(dummy[iq].at( dummy[iq].size()-1 ) );
     }
   }
   //cout<<"got"<<endl;

   vector<double> expExclusion,obsExclusion;
   vector<double> expExclusionh,obsExclusionh;

   //cout<<"point "<<ifile<<endl;
   float valxsections = 0;
   for(int ix = 0; ix< nXsec; ix++){
    if (fabs(lambdas[ifile]-deflambdas[ix])<0.1)valxsections=xsections[ix];
    if (plotMu) valxsections =1;
   }


   cout<<"qui"<<endl;
   if(hybrid){
     Val_obsh = dummy[5]; 
     Val_meanh = dummy[2];
     Val_68hh  = dummy[3];
     Val_68lh  = dummy[1];
     Val_95hh  = dummy[4]; 
     Val_95lh  = dummy[0];
     limh[ifile] = Val_meanh[ifile]*valxsections;
   }

   lim[ifile] = Val_mean[ifile]*valxsections;
   cout<<"r="<<Val_mean[ifile]<<"  exp (obs) xsection>"<<valxsections*Val_mean[ifile]<<"/"<<valxsections<<" ("<<Val_obs[ifile]*valxsections<<")"<<endl;
   if(hybrid)cout<<"r(hybrid)="<<Val_meanh[ifile]<<"  exp (obs) xsection>"<<valxsections*Val_meanh[ifile]<<"/"<<valxsections<<" ("<<Val_obs[ifile]*valxsections<<")"<<endl;

   lim68l[ifile] = min( Val_68l[ifile], Val_68h[ifile])*valxsections;
   lim95l[ifile] = min( Val_95l[ifile], Val_95h[ifile])*valxsections;
   lim68h[ifile] = max( Val_68h[ifile], Val_68l[ifile])*valxsections;
   lim95h[ifile] = max( Val_95h[ifile], Val_95l[ifile])*valxsections;
   if(hybrid){
     lim68lh[ifile] = min( Val_68lh[ifile], Val_68hh[ifile])*valxsections;
     lim95lh[ifile] = min( Val_95lh[ifile], Val_95hh[ifile])*valxsections;
     lim68hh[ifile] = max( Val_68hh[ifile], Val_68lh[ifile])*valxsections;
     lim95hh[ifile] = max( Val_95hh[ifile], Val_95lh[ifile])*valxsections;
   }
   if(lim[ifile] < 1.0) expExclusion.push_back(lambdas[ifile]);
   if( addObsLimit) limobs[ifile] = Val_obs[ifile]*valxsections;
 }
 // ------------------- Change Values to Arrays -------------------- //
  // ------------------- Draw  -------------------- //
 TString cname = "c"+inputSuffix[ichannel];
 TCanvas *c = new TCanvas(cname.Data(),cname.Data(),canvasX,canvasY);
 c->SetFrameLineWidth(3);

 //c->cd()->SetLineWidth(2);
 c->cd();
 //TPad *pad2 = new TPad("pad2", "foo", 0, 0,   1, 0.3);
 TPad *pad1 = new TPad("pad1", "foo", 0, 0.0, 1, 1);//0.3 al posto di 0.0
 //pad2->SetBottomMargin(0.22);
 pad1->SetLineWidth(2);

 pad1->Draw();
 //pad2->Draw();
 //pad2->SetGrid()
 int nMassEff = nLambdas;

 TGraph *gr = new TGraph(nMassEff, lambdas, lim);
 TGraph* grshade_68 = new TGraph(2*nMassEff);
 TGraph* grshade_95 = new TGraph(2*nMassEff);
 TGraph *grh = new TGraph(nMassEff, lambdas, limh);
 TGraph* grshade_68h = new TGraph(2*nMassEff);
 TGraph* grshade_95h = new TGraph(2*nMassEff);
 TGraph *grObs = new TGraph(nMassEff, lambdas, limobs);
 TGraphAsymmErrors *grh_points = new TGraphAsymmErrors(nMassEff, lambdas,limh);
 TGraphAsymmErrors *grh_points95 = new TGraphAsymmErrors(nMassEff, lambdas,limh);
 //TGraph *grExp = new TGraph(nXsec,deflambdas,xsections);

 grObs->SetLineWidth(3);
 grObs->SetLineColor(kBlack);
 grObs->SetMarkerStyle(20);
 grObs->SetMarkerSize(0.8);

 //grExp->SetLineColor(kRed);
 //grExp->SetLineWidth(3);

 gr->SetLineStyle(2);
 gr->SetMarkerStyle(20);
 gr->SetLineWidth(3);
 gr->SetLineColor(kBlue);
 gr->SetMarkerColor(kBlue);
 gr->SetMarkerSize(1.1);

 grh->SetLineStyle(2);
 grh->SetMarkerStyle(20);
 grh->SetLineWidth(3);
 grh->SetLineColor(kBlue);
 grh->SetMarkerColor(kBlue);
 grh->SetMarkerSize(1.1);

 grh_points->SetLineStyle(2);
 grh_points->SetMarkerStyle(20);
 grh_points->SetLineWidth(3);
 grh_points->SetLineColor(kBlue);
 grh_points->SetMarkerColor(kBlue);
 grh_points->SetMarkerSize(1.4);


 grshade_68->SetFillColor(kGreen);
 grshade_95->SetFillColor(kYellow);   
 grshade_68->SetLineStyle(1);
 grshade_95->SetLineStyle(1);
 grshade_68->SetLineWidth(3);
 grshade_95->SetLineWidth(3);
 grshade_68->SetLineColor(kBlue);
 grshade_95->SetLineColor(kBlue);

 grshade_68h->SetFillColor(kGreen);
 grshade_95h->SetFillColor(kYellow);   
 grshade_68h->SetLineStyle(1);
 grshade_95h->SetLineStyle(1);
 grshade_68h->SetLineWidth(3);
 grshade_95h->SetLineWidth(3);
 grshade_68h->SetLineColor(kBlue);
 grshade_95h->SetLineColor(kBlue);

 grh_points->SetFillColor(kGreen);
 grh_points95->SetFillColor(kYellow);   
 grh_points->SetLineStyle(1);
 grh_points95->SetLineStyle(1);
 grh_points->SetLineWidth(3);
 grh_points95->SetLineWidth(3);
 grh_points->SetLineColor(kBlue);
 grh_points95->SetLineColor(kBlue);


 int sizeV = nMassEff;
 for (int a = 0; a < nMassEff; a++)
 {
  grshade_68->SetPoint(a,lambdas[a],lim68l[a]);
  grshade_68->SetPoint(sizeV+a,lambdas[nMassEff-a-1],lim68h[nMassEff-a-1]);
  grshade_95->SetPoint(a,lambdas[a],lim95l[a]);
  grshade_95->SetPoint(nMassEff+a,lambdas[nMassEff-a-1],lim95h[nMassEff-a-1]);

  grshade_68h->SetPoint(a,lambdas[a],lim68lh[a]);
  grshade_68h->SetPoint(sizeV+a,lambdas[nMassEff-a-1],lim68hh[nMassEff-a-1]);
  grshade_95h->SetPoint(a,lambdas[a],lim95lh[a]);
  grshade_95h->SetPoint(nMassEff+a,lambdas[nMassEff-a-1],lim95hh[nMassEff-a-1]);

  //grh_points->SetPointError(a,0.2,0.2,lim68lh[a],lim68hh[nMassEff-a-1]);
  //grh_points95->SetPointError(a,0.2,0.2,lim95lh[a],lim95hh[nMassEff-a-1]);
  cout<<"point "<<a<<" lambda "<<lambdas[a]<<" limh "<<limh[a]<< " 68l "<<lim68lh[a]<<" 68h "<<lim68hh[a]<<endl;
  grh_points->SetPointError(a,0.27,0.27,limh[a]-lim68lh[a],lim68hh[a]-limh[a]);
  grh_points95->SetPointError(a,0.27,0.27,limh[a]-lim95lh[a],lim95hh[a]-limh[a]);
  }

  // --------------- Low Mass Zoom -------------- //
	/*
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
  */
	TLegend *box2 = buildLegend();

  double ptLow= 0.6, ptHigh = 0.8;
/*		
  TPaveText *pt = new TPaveText(ptLow,0.44,ptHigh,0.5,"NDC");
  pt->SetFillColor(0);
  pt->SetTextFont(42);
  pt->AddText("CMS Preliminary");
*/
  //TPaveText *pt4 = new TPaveText(ptLow,0.38,ptHigh,0.44,"NDC");
  TPaveText *pt4 = new TPaveText(0.1819196,0.8080357,0.7008929,0.8675595,"brNDC");
  pt4->SetTextAlign(12);
  pt4->SetFillColor(0);
  pt4->SetTextFont(42);
  pt4->AddText(channel.Data()); 
/*
  //TPaveText *pt2 = new TPaveText(0.69,0.94,0.98,0.99,"NDC");
  TPaveText *pt2 = new TPaveText(0.7,0.9,0.9,0.95,"NDC");
  pt2->SetFillColor(0);
  pt2->SetTextFont(42);
  char lum[192];
  //sprintf(lum," #sqrt{s} = 8 TeV, L = %.2f fb^{-1}",5.261);
  sprintf(lum," #sqrt{s} = 13 TeV, L = %.1f fb^{-1}",lumin);
  pt2->AddText(lum); 
  //TPaveText *pt3 = new TPaveText(0.69,0.90,0.98,0.95,"NDC");
 */


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
   pt2->AddText(" #sqrt{s} = 13 TeV, L = 2.6 fb^{-1}");
   //pt->Draw();

  if(grid) pad1->SetGrid();

  pad1->cd();
  pad1->SetFrameLineWidth(2);
  TH1F *hr = pad1->DrawFrame(xLow,yLow,xHigh,yHigh);
  if(logy)pad1->SetLogy();
  
  if(asympt) {
    gr->Draw("lp");
    grshade_95->Draw("f");
    grshade_68->Draw("f");
  }
  if(hybrid) {
//    if(asympt){
//      grshade_68h->Draw("l");
//      grshade_95h->Draw("l");
//    }else{
//      grshade_95h->Draw("2");
//      grshade_68h->Draw("2p");
//    }
//    grh->Draw("2lp");
    grh_points95->Draw("2");
    grh_points->Draw("2p");


  }
  if(!plotMu) getExpLine(pad1->cd()) ; //grExp->Draw("l");
  if(addObsLimit)
    {
      if(points)grObs->Draw("lP");
      else grObs->Draw("l");
    }
  if (addATLAS){
    TGraph *ATLAS = new TGraph(0);
    ATLAS->SetPoint(0,1,130*34.3*0.073);
    ATLAS->SetMarkerStyle(29);
    ATLAS->SetMarkerColor(4);
    ATLAS->SetMarkerSize(2);
    ATLAS->Draw("p");
  }


  hr->GetXaxis()->SetTitle(xTitle);
  hr->GetYaxis()->SetTitle(yTitle);
  hr->GetYaxis()->SetTitleOffset(1.2);		

  
  c->SetFrameLineWidth(3);
  c->Update();
  if(gridOnTop)gPad->RedrawAxis("g");
  gPad->SetFrameLineWidth(3);
  box2->Draw();
  c->SetFrameLineWidth(3);
  c->Update();


  //pad2->cd();
  //TH1F *hr2 = pad2->DrawFrame(105.0,yLow,180.0,yHigh);
  //hr2->GetYaxis()->SetTitleOffset(0.06);

  pt->Draw("SAME");
  pt2->Draw("SAME");
  //pt3->Draw("SAME");
  pt4->Draw("SAME");

  //plotName.Append(sel.Data());
  //plotName.Append(var.Data());

  TString outfileName=plotName.Data();
  outfileName.Append(".eps");
  c->SaveAs(outfileName.Data());
  outfileName=plotName.Data();
  outfileName.Append(".pdf");
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

void getExpLine(TVirtualPad *c){

   TF1 myFunc("myFunc","(2.11 + 0.29*x*x -1.40*x)*2.5039",-4,20); 
   TGraph * graph = new TGraph(&myFunc);
   int ci = TColor::GetColor("#ff0000");
   c->cd();
   graph->SetLineColor(ci);
   graph->SetLineWidth(3);
   graph->Draw("l");
   //systematics band
   float Graph_syst_Scale_x[240] , Graph_syst_Scale_y[240], Graph_syst_Scale_x_err[240], Graph_syst_Scale_y_errup[240], Graph_syst_Scale_y_errdown[240] ;
   for (int i = 0 ; i < 240 ; ++i) {
      Graph_syst_Scale_x[i] = -4.+(i*1.)/10. ;
      Graph_syst_Scale_y[i] = (2.11+0.29*(-4.+(i*1.)/10.)*(-4.+(i*1.)/10.)-1.40*(-4.+(i*1.)/10.))*2.5039 ;
      Graph_syst_Scale_x_err[i] = 0; 
      Graph_syst_Scale_y_errup[i] =   (2.11+0.29*(-4.+(i*1.)/10.)*(-4.+(i*1.)/10.)-1.40*(-4.+(i*1.)/10.))*2.5039*0.1301; 
      Graph_syst_Scale_y_errdown[i] = (2.11+0.29*(-4.+(i*1.)/10.)*(-4.+(i*1.)/10.)-1.40*(-4.+(i*1.)/10.))*2.5039*0.137; 
   } 
   TGraphAsymmErrors *Graph_syst_Scale = new TGraphAsymmErrors(240,Graph_syst_Scale_x,Graph_syst_Scale_y,Graph_syst_Scale_x_err,Graph_syst_Scale_x_err,Graph_syst_Scale_y_errup,Graph_syst_Scale_y_errdown);
   Graph_syst_Scale->SetLineColor(kRed); // also kRed-9
   Graph_syst_Scale->SetFillColor(kRed);
   Graph_syst_Scale->SetFillStyle(3001); // 3005: lines, 3001: dots
   Graph_syst_Scale->Draw("e3");
   return ;
}

TLegend *buildLegend(){
  TLegend *leg = new TLegend(0.5803571,0.110119,0.8928571,0.3452381,NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("NULL","Theory #sigma #times BR(hh #rightarrowbb#tau#tau)","fl");
   entry->SetLineColor(2);
   entry->SetLineStyle(0);
   entry->SetLineWidth(3);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("NULL","Theory syst. uncert.","f");
   entry->SetLineColor(0);
   entry->SetFillColor(2);
   entry->SetFillStyle(3001);
   entry->SetLineWidth(3);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("NULL","1D Fit Expected Asym. CLs","fl");
   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(2);
   entry->SetLineWidth(3);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
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