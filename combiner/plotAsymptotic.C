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
#include "TString.h"

//Capire come gestire lambdas decimali
// channels: 0=mutau,1=etau,2=tautau,3=combined (magari fare enum)
void getExpLine(TVirtualPad *,double xmin=-4, double xmax=20,double yt=1);
int getLimits(TFile *f, std::vector<double> &v_mean,std::vector<double> &v_68l,std::vector<double> &v_68h,std::vector<double> &v_95l,std::vector<double> &v_95h,std::vector<double> &v_obs, bool onepointta=false);
TGraphAsymmErrors* plotAsymptoticChannel(int,bool a,double yt=1);
TLegend *buildLegend();
double getExpValue(double l , double yt=1);
TString getFileForLambda(float l, float t=1);
TString getFileForCluster(int l);
void AddText(TString channel="");
// --------- Inputs ------- //

//TString inputDir = "cards"; //"higginputsCombineTest.Asymptotic.mH125.7.root";
//TString input="_07mar_testBR";
//TString Appendix="noBR";//_yt1_2";
//Plotting features
bool addATLAS=false;
const bool addObsLimit = true;
const bool hybrid = false;
float yt =1.0;
const float BR=1;//0.073;
TString brstring="";//_noBR";

TString input="_clusterXanda";//"_01mar_Unblind";//_09mar_noBR";
TString Appendix="XandaallClusters";

//Default inputs
bool plotMu=false;
const int nLambdas = 24;//52;
float lambdas[nLambdas];//=  {-5, -4,-2,-1,0, 1  ,2.46, 4, 5};
//float lambdas[]={-15,-8,-4, 0,1,2.46, 5  ,10,20};

// string lnames[] ={"Lambdam4","Lambda1","Lambda2dot46","Lambda20"};
// string lnames[] ={"HHNodeSM","HHNodeBox","HHNode2","HHNode3","HHNode4","HHNode5","HHNode6","HHNode7","HHNode8","HHNode9","HHNode10","HHNode11","HHNode12","HHNode13"};
//const int nXsec = 10;
const float xfac = 1000.0/0.1/BR;//37.9*0.073;//o 0.073?
const bool _DEBUG_ = true;
Double_t xLow = -1.0;
Double_t xHigh =25.0;
Double_t yLow = 1;
Double_t yHigh = 100000.;
const bool logy = true;
const bool logx = false;
const bool grid = true;
const bool gridOnTop = true;
const bool points = true;
int canvasX = 900;
int canvasY = 700;
double sqrts = 13.0;
double lumin = 2.7;
bool onepointta=true;
bool skipFailedHyb = true;
//-----Other settings-----//
//TString inputSuffix[4]={"MuTau","ETau","TauTau","Combined"};
//TString variables[4]={"HH_mass_raw","HH_mass_raw","HH_mass_raw","COMBINED"};
//TString selections[4]={"defaultBtagLLNoIsoBBTTCutKine80","defaultBtagLLNoIsoBBTTCutKine80","defaultBtagLLNoIsoBBTTCut45",""};
TString channelOut[4]={"bb #mu#tau_{h}","bb e#tau_{h}","bb #tau_{h}#tau_{h}","bb #mu#tau_{h} + bb e#tau_{h} + bb #tau_{h}#tau_{h}"};

TString inputSuffix[4]={"MuTau","ETau","TauTau","Combined"};
TString variables[4]={"HH_mass_raw","HH_mass_raw","HH_mass_raw","COMBINED"};
TString selections[4]={"defaultBtagLLNoIsoBBTTCutKine80","defaultBtagLLNoIsoBBTTCutKine80","defaultBtagLLNoIsoBBTTCut45",""};
//TString channelOut[4]={"bb #mu#tau_{h}","bb e#tau_{h}","bb #tau_{h}#tau_{h}","bb #mu#tau_{h} + bb e#tau_{h} + bb #tau_{h}#tau_{h}"};
TString xTitle = "k_{#lambda}=#lambda_{hhh}/#lambda_{hhh}^{SM}";
//TString yTitle = "95% CL limit on #sigma(pp #rightarrow hh) x BR (hh #rightarrow bb#tau#tau) [fb]";
TString yTitle = "95% CL limit on #sigma(pp #rightarrow hh) [fb]";
using namespace std;
  
void plotAsymptotic(){
  for (int i=0;i<nLambdas;i++){
    lambdas[i]=i;//lambdas[i]=i-20;
  }
  channelOut[0]="bb #mu#tau_{h}";
  channelOut[1]="bb e#tau_{h}";
  channelOut[2]="bb #tau_{h}#tau_{h}";
  channelOut[3]="bb #mu#tau_{h} + bb e#tau_{h} + bb #tau_{h}#tau_{h}";

  //input="_07mar_testBR";
  //Appendix="withBR";//_yt1_2";
  //TString inputSuffix[4]={"MuTau","ETau","TauTau","Combined"};

 gStyle->SetPadLeftMargin(0.16);
 gStyle->SetPadTopMargin(0.1);
 gStyle->SetFrameLineWidth(3);

  TGraphAsymmErrors *g[4];
  TGraphAsymmErrors *gBands;
  for(int i=0;i<4;i++)
    g[i] = plotAsymptoticChannel(2,hybrid,yt);
  TString plotName ="UpperLimit_"+inputSuffix[3]+input;
  if(addObsLimit && !hybrid) plotName+="_obs";
  plotName += Appendix;
  plotName += ".root";
  TFile *fband = TFile::Open(plotName.Data());
  TCanvas *temp = (TCanvas*)fband->Get("cCombined");
  gBands = (TGraphAsymmErrors*)temp->FindObject("grBand");

  
  TCanvas *cAllLines = new TCanvas("all","all",canvasX,canvasY);
  cAllLines->cd();
  cAllLines->SetFrameLineWidth(3);
  cAllLines->cd();
  TPad *pad1 = new TPad("pad1", "foo", 0, 0.0, 1, 1);
  pad1->SetLineWidth(2);
  pad1->Draw();
  if(grid) pad1->SetGrid();
  pad1->cd();
  pad1->SetFrameLineWidth(3);
  TH1F *hr = pad1->DrawFrame(xLow,yLow,xHigh,yHigh);
  hr->GetXaxis()->SetTitle(xTitle);
  hr->GetYaxis()->SetTitle(yTitle);
  hr->GetYaxis()->SetTitleOffset(1.2);    

  if(logy)pad1->SetLogy();
  //getExpLine(pad1->cd(),xLow+1,xHigh-1) ;
  gBands->Draw("2");//2
  g[3]->Draw("2PSAME"); //2psame
  for(int i=0;i<3;i++){
    g[i]->SetMarkerStyle(19);
    g[i]->SetMarkerColor(i+1);
    g[i]->SetLineColor(i+1);
    if(i==2){
      g[i]->SetMarkerColor(kMagenta-3);
      g[i]->SetLineColor(kMagenta-3);        
    }
    g[i]->SetMarkerSize(1);
    g[i]->Draw("*sameX");
  }
  AddText(channelOut[3]);
  //TLegend *box2 = buildLegend();
  //box2->Draw();
}

TGraphAsymmErrors* plotAsymptoticChannel(int ichannel,bool hybrid, double yt) {
  //input="_07mar_testBR";
  //Appendix="noBR";//_yt1_2";
  //TString inputSuffix[4]={"MuTau","ETau","TauTau","Combined"};

  bool asympt = false;
  if(!hybrid) asympt = true;
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
  TString channel="hh #rightarrow "+channelOut[ichannel]; //channel name in plot

  //float xsections[nXsec]= {12.35, 2.13, 1,  0.66, 0.47, 0.42, 0.53, 0.79, 50,  90.11};
  //float deflambdas[nXsec]={-4,  0,   1,  1.5,   2,   2.46, 3,    4,    10,  20};

  //for(int ix = 0; ix< nXsec; ix++) xsections[ix] *= xfac; //xsXBR in fb
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
  float templim[nLambdas], templimobs[nLambdas],templim68l[nLambdas],templim68h[nLambdas],templim95l[nLambdas],templim95h[nLambdas];

  //float limh[nLambdas], limobsh[nLambdas],lim68lh[nLambdas],lim68hh[nLambdas],lim95lh[nLambdas],lim95hh[nLambdas];
  TString quantiles[6]={"0.025","0.16","0.5","0.84","0.975","-1.0"};
  TString quantiles3[6]={".quant0.025",".quant0.160",".quant0.500",".quant0.840",".quant0.975",""};

  for(int ifile=0;ifile<nLambdas;ifile++){
   TString filename;
   cout<<ifile<<endl;
   bool skipLambda = false;
   // filename.Form("%s/%s%s%s/higgsCombine%s_forLim.Asymptotic.mH125.root",inputfolder.Data(),lnames[ifile].c_str(),sel.Data(),var.Data(),lnames[ifile].c_str());
   // TFile *inFile = TFile::Open(filename.Data());
   // getLimits(inFile,Val_mean,Val_68l,Val_68h,Val_95l,Val_95h,Val_obs,onepointta);

   TString lname = getFileForCluster((int)lambdas[ifile]);;//getFileForLambda(lambdas[ifile]);
   if(hybrid){
     for(int iq=0;iq<6;iq++){
       filename.Form("%s/%s%s%s/higgsCombineforLim_%s.HybridNew.mH125%s.root",inputfolder.Data(),lname.Data(),sel.Data(),var.Data(),quantiles[iq].Data(),quantiles3[iq].Data());
       cout<<filename.Data()<<endl;
       TFile *inFileh = TFile::Open(filename.Data());
       cout<<inFileh<<endl;
       //getLimits(inFileh,dummy[2],dummy[1],dummy[3],dummy[0],dummy[4],dummy[5],onepointta);
       int check = getLimits(inFileh,Val_mean,Val_68l,Val_68h,Val_95l,Val_95h,Val_obs,onepointta);
       check=1;
       inFileh->Close();
       int addition = -1;
       if(ifile==0) addition=1;
       while (!check) {
        skipLambda = true;
        if (addition==0)addition++;
        lname = getFileForLambda(lambdas[ifile+addition]);
        filename.Form("%s/%s%s%s/higgsCombineforLim_%s.HybridNew.mH125%s.root",inputfolder.Data(),lname.Data(),sel.Data(),var.Data(),quantiles[iq].Data(),quantiles3[iq].Data());
        inFileh = TFile::Open(filename.Data());      
        check = getLimits(inFileh,Val_mean,Val_68l,Val_68h,Val_95l,Val_95h,Val_obs,onepointta);
        inFileh->Close();       
        addition++;
       }
       //val_hyb[iq].push_back(dummy[iq].at( dummy[iq].size()-1 ) );
     }
     //dummy[5].push_back(1);
   }else {
     filename.Form("%s/%s%s%s/higgsCombine%s_forLim%s.Asymptotic.mH125.root",inputfolder.Data(),lname.Data(),sel.Data(),var.Data(),lname.Data(),brstring.Data());
     TFile *inFile = TFile::Open(filename.Data());
     getLimits(inFile,Val_mean,Val_68l,Val_68h,Val_95l,Val_95h,Val_obs,onepointta);
   }

   vector<double> expExclusion,obsExclusion;
   vector<double> expExclusionh,obsExclusionh;

   //cout<<"point "<<ifile<<endl;
   float valxsections = xfac;//getExpValue(lambdas[ifile])*xfac;
   // for(int ix = 0; ix< nXsec; ix++){
   //  if (fabs(lambdas[ifile]-deflambdas[ix])<0.1)valxsections=xsections[ix];
   //  if (plotMu) valxsections =1;
   // }
   if (plotMu) valxsections =1;
   //cout<<"qui"<<endl;
   // if(hybrid){
   //   Val_obs = dummy[5]; 
   //   Val_mean = dummy[2];
   //   Val_68h  = dummy[3];
   //   Val_68l  = dummy[1];
   //   Val_95h  = dummy[4]; 
   //   Val_95l  = dummy[0];
   //   //lim[ifile] = Val_meanh[ifile]*valxsections;
   // }
   //cout<<Val_mean[ifile]<<endl;
   //cout<<"r="<<Val_mean[ifile]<<"  exp (obs) xsection>"<<valxsections*Val_mean[ifile]<<"/"<<valxsections<<" ("<<Val_obs[ifile]*valxsections<<")"<<endl;
   //if(hybrid)cout<<"r(hybrid)="<<Val_mean[ifile]<<"  exp (obs) xsection>"<<valxsections*Val_mean[ifile]<<"/"<<valxsections<<" ("<<Val_obs[ifile]*valxsections<<")"<<endl;
   // cout<<Val_68l[ifile]<<endl;
   // cout<<Val_95l[ifile]<<endl;
   // cout<<Val_68h[ifile]<<endl;
   // cout<<Val_95h[ifile]<<endl;
   if(!skipLambda){
     templim[ifile] = Val_mean[ifile]*valxsections;
     templim68l[ifile] = min( Val_68l[ifile], Val_68h[ifile])*valxsections;
     templim95l[ifile] = min( Val_95l[ifile], Val_95h[ifile])*valxsections;
     templim68h[ifile] = max( Val_68h[ifile], Val_68l[ifile])*valxsections;
     templim95h[ifile] = max( Val_95h[ifile], Val_95l[ifile])*valxsections;
     if( addObsLimit) templimobs[ifile] = Val_obs[ifile]*valxsections;
   }else {
     templim[ifile] = -1; //Val_mean[ifile]*valxsections;
     templim68l[ifile] = -1; //min( Val_68l[ifile], Val_68h[ifile])*valxsections;
     templim95l[ifile] = -1; //min( Val_95l[ifile], Val_95h[ifile])*valxsections;
     templim68h[ifile] = -1; //max( Val_68h[ifile], Val_68l[ifile])*valxsections;
     templim95h[ifile] = -1; //max( Val_95h[ifile], Val_95l[ifile])*valxsections;
     if( addObsLimit) templimobs[ifile] = -1; //Val_obs[ifile]*valxsections;

   }
   if(templim[ifile] < 1.0) expExclusion.push_back(lambdas[ifile]);

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
 int nmass = 0;
 for (int i=0;i<nLambdas;i++) {
  if(templim[i]>0)nmass++;
 }
 const int nMassEff = nmass;//nLambdas;
 float lim[nMassEff], limobs[nMassEff],lim68l[nMassEff],lim68h[nMassEff],lim95l[nMassEff],lim95h[nMassEff],newlambdas[nMassEff];
 int iEff = 0;
 for (int i=0;i<nLambdas;i++) {
  if(templim[i]<0)continue;
   lim[iEff]    = templim[i];
   lim68l[iEff] = templim68l[i];
   lim95l[iEff] = templim95l[i];
   lim68h[iEff] = templim68h[i];
   lim95h[iEff] = templim95h[i];
   if( addObsLimit) limobs[iEff] = templimobs[i];
   newlambdas[iEff] = lambdas[i];
   iEff++;
   //if(lim[iEff] < 1.0) expExclusion.push_back(lambdas[i]);
 }

 // TGraph *gr = new TGraph(nMassEff, lambdas, lim);
 // TGraph* grshade_68 = new TGraph(2*nMassEff);
 // TGraph* grshade_95 = new TGraph(2*nMassEff);
 // TGraph *grh = new TGraph(nMassEff, lambdas, limh);
 // TGraph* grshade_68h = new TGraph(2*nMassEff);
 // TGraph* grshade_95h = new TGraph(2*nMassEff);
 TGraph *grObs = new TGraph(nMassEff, newlambdas, limobs);
 TGraphAsymmErrors *gr_points = new TGraphAsymmErrors(nMassEff, newlambdas,lim);
 TGraphAsymmErrors *gr_points95 = new TGraphAsymmErrors(nMassEff, newlambdas,lim);
 gr_points->SetName("gr");
 gr_points->SetTitle("gr");
 grObs->SetName("grObs");
 grObs->SetTitle("grObs");
 gr_points95->SetName("grBand");
 gr_points95->SetTitle("grBand");

 //TGraph *grExp = new TGraph(nXsec,deflambdas,xsections);

 grObs->SetLineWidth(3);
 grObs->SetLineColor(kBlack);
 grObs->SetMarkerStyle(20);
 grObs->SetMarkerSize(1.0);

 //grExp->SetLineColor(kRed);
 //grExp->SetLineWidth(3);

 // gr->SetLineStyle(2);
 // gr->SetMarkerStyle(20);
 // gr->SetLineWidth(3);
 // gr->SetLineColor(kBlue);
 // gr->SetMarkerColor(kBlue);
 // gr->SetMarkerSize(1.1);

 // grh->SetLineStyle(2);
 // grh->SetMarkerStyle(20);
 // grh->SetLineWidth(3);
 // grh->SetLineColor(kBlue);
 // grh->SetMarkerColor(kBlue);
 // grh->SetMarkerSize(1.1);

 gr_points->SetLineStyle(2);
 gr_points->SetMarkerStyle(5);
 gr_points->SetLineWidth(3);
 gr_points->SetLineColor(kBlue);
 gr_points->SetMarkerColor(kBlue);
 gr_points->SetMarkerSize(1.4);


 // grshade_68->SetFillColor(kGreen);
 // grshade_95->SetFillColor(kYellow);   
 // grshade_68->SetLineStyle(1);
 // grshade_95->SetLineStyle(1);
 // grshade_68->SetLineWidth(3);
 // grshade_95->SetLineWidth(3);
 // grshade_68->SetLineColor(kBlue);
 // grshade_95->SetLineColor(kBlue);

 // grshade_68h->SetFillColor(kGreen);
 // grshade_95h->SetFillColor(kYellow);   
 // grshade_68h->SetLineStyle(1);
 // grshade_95h->SetLineStyle(1);
 // grshade_68h->SetLineWidth(3);
 // grshade_95h->SetLineWidth(3);
 // grshade_68h->SetLineColor(kBlue);
 // grshade_95h->SetLineColor(kBlue);

 gr_points->SetFillColor(kGreen);
 gr_points95->SetFillColor(kYellow);   
 gr_points->SetLineStyle(1);
 gr_points95->SetLineStyle(1);
 gr_points->SetLineWidth(3);
 gr_points95->SetLineWidth(3);
 gr_points->SetLineColor(kBlue);
 gr_points95->SetLineColor(kBlue);


 int sizeV = nMassEff;
 for (int a = 0; a < nMassEff; a++)
 {
  // grshade_68->SetPoint(a,lambdas[a],lim68l[a]);
  // grshade_68->SetPoint(sizeV+a,lambdas[nMassEff-a-1],lim68h[nMassEff-a-1]);
  // grshade_95->SetPoint(a,lambdas[a],lim95l[a]);
  // grshade_95->SetPoint(nMassEff+a,lambdas[nMassEff-a-1],lim95h[nMassEff-a-1]);

  // grshade_68h->SetPoint(a,lambdas[a],lim68lh[a]);
  // grshade_68h->SetPoint(sizeV+a,lambdas[nMassEff-a-1],lim68hh[nMassEff-a-1]);
  // grshade_95h->SetPoint(a,lambdas[a],lim95lh[a]);
  // grshade_95h->SetPoint(nMassEff+a,lambdas[nMassEff-a-1],lim95hh[nMassEff-a-1]);

  //grh_points->SetPointError(a,0.2,0.2,lim68lh[a],lim68hh[nMassEff-a-1]);
  //grh_points95->SetPointError(a,0.2,0.2,lim95lh[a],lim95hh[nMassEff-a-1]);
  //cout<<"point "<<a<<" lambda "<<lambdas[a]<<" limh "<<limh[a]<< " 68l "<<lim68lh[a]<<" 68h "<<lim68hh[a]<<endl;
  float larger = 0.4;
  if(fabs(yt-1)>0.05)larger = 0.12;
  gr_points->SetPointError(a,larger,larger,lim[a]-lim68l[a],lim68h[a]-lim[a]);
  gr_points95->SetPointError(a,larger,larger,lim[a]-lim95l[a],lim95h[a]-lim[a]);  //quello giusto e 0.27
  }

  // --------------- Low Mass Zoom -------------- //
	TLegend *box2 = buildLegend();

  double ptLow= 0.6, ptHigh = 0.8;
  if(grid) pad1->SetGrid();

  pad1->cd();
  pad1->SetFrameLineWidth(3);
  TH1F *hr = pad1->DrawFrame(xLow,yLow,xHigh,yHigh);
  if(logy)pad1->SetLogy();

  
  // if(asympt) {
  //   gr->Draw("p");
  //   grshade_95->Draw("f");
  //   grshade_68->Draw("f");
  // }
  // if(hybrid) {
//    if(asympt){
//      grshade_68h->Draw("l");
//      grshade_95h->Draw("l");
//    }else{
//      grshade_95h->Draw("2");
//      grshade_68h->Draw("2p");
//    }
//    grh->Draw("2lp");
    gr_points95->Draw("2");
    gr_points->Draw("2p");
//   }
    gr_points->Print();


  if(!plotMu) getExpLine(pad1->cd(),xLow+1,xHigh-1,yt) ; //grExp->Draw("l");
  if(addObsLimit)
    {
      if(points)grObs->Draw("P");
      else grObs->Draw("lp");
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

  //pt->Draw("SAME");
  //pt2->Draw("SAME");
  AddText(channel);
  //pt3->Draw("SAME");

  //plotName.Append(sel.Data());
  //plotName.Append(var.Data());
  if(addObsLimit && asympt) plotName+="_obs";
  plotName+=Appendix;
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

  return gr_points;
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
   pt4->SetFillStyle(0);
   pt4->SetLineColor(0);
   pt4->SetLineStyle(0);

   TPaveText *pt5 = new TPaveText(0.75,0.8080357,0.8908686,0.8675595,"brNDC");
   pt5->SetTextAlign(12);
   pt5->SetFillColor(0);
   pt5->SetFillStyle(0);
   pt5->SetTextFont(42);
   //pt5->AddText(channel.Data()); 
   pt5->SetTextSize(0.035);
   TString ytstr; ytstr.Form("y_{t}/y^{SM}_{t}=%.1f",yt);
   pt5->AddText(ytstr.Data());
   pt5->SetLineColor(0);
   pt5->SetLineStyle(0);


   pt->Draw();
   pt2->Draw();
   pt4->Draw("SAME");

   if(fabs(yt-1)>0.05)pt5->Draw("SAME");


}

int getLimits(TFile *f, std::vector<double> &v_mean,std::vector<double> &v_68l,std::vector<double> &v_68h,std::vector<double> &v_95l,std::vector<double> &v_95h,std::vector<double> &v_obs,bool onepointta)
{
  //cout<<"getting tree"<<endl;
  TTree *tree =(TTree*)f->Get("limit");
  //cout<<tree<<endl;
  //cout<<tree->GetEntries()<<endl;
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
  return tree->GetEntries();
 }

void getExpLine(TVirtualPad *c, double xmin, double xmax, double yt){

   //TF1 myFunc("myFunc","(2.11 + 0.29*x*x -1.40*x)*2.7667",xmin,xmax); 
   TF1 myFunc("myFunc","(2.09*[0]*[0]*[0]*[0] + 0.28*[0]*[0]*x*x -1.37*[0]*[0]*[0]*x)*2.7667/[1]",xmin,xmax);
   myFunc.SetParameter(0,yt); 
   myFunc.SetParameter(1,BR); 
   TGraph * graph = new TGraph(&myFunc);
   int ci = TColor::GetColor("#ff0000");
   c->cd();
   graph->SetLineColor(ci);
   graph->SetLineWidth(3);
   graph->Draw("l");
   const int nP = (xmax-xmin)*10.0;
   //systematics band
   float * Graph_syst_Scale_x = new float[nP];
   float * Graph_syst_Scale_y= new float[nP];
   float * Graph_syst_Scale_x_err = new float[nP];
   float * Graph_syst_Scale_y_errup= new float[nP];
   float * Graph_syst_Scale_y_errdown= new float[nP] ;


   for (int i = 0 ; i < nP ; ++i) {
      Graph_syst_Scale_x[i] = xmin+(i*1.)/10. ;
      Graph_syst_Scale_y[i] = getExpValue(xmin+(i*1.)/10.,yt); //(2.11+0.29*(-4.+(i*1.)/10.)*(-4.+(i*1.)/10.)-1.40*(-4.+(i*1.)/10.))*2.5039 ;
      Graph_syst_Scale_x_err[i] = 0; 
      Graph_syst_Scale_y_errup[i] =   (2.09*yt*yt*yt*yt+0.28*yt*yt*(xmin+(i*1.)/10.)*(xmin+(i*1.)/10.)-1.37*yt*yt*yt*(xmin+(i*1.)/10.))*2.7667*0.1301/BR; 
      Graph_syst_Scale_y_errdown[i] = (2.09*yt*yt*yt*yt+0.28*yt*yt*(xmin+(i*1.)/10.)*(xmin+(i*1.)/10.)-1.37*yt*yt*yt*(xmin+(i*1.)/10.))*2.7667*0.137/BR; 
   } 
   TGraphAsymmErrors *Graph_syst_Scale = new TGraphAsymmErrors(nP,Graph_syst_Scale_x,Graph_syst_Scale_y,Graph_syst_Scale_x_err,Graph_syst_Scale_x_err,Graph_syst_Scale_y_errup,Graph_syst_Scale_y_errdown);
   Graph_syst_Scale->SetLineColor(kRed); // also kRed-9
   Graph_syst_Scale->SetFillColor(kRed);
   Graph_syst_Scale->SetFillStyle(3001); // 3005: lines, 3001: dots
   Graph_syst_Scale->Draw("e3");
   return ;
}

double getExpValue(double kl, double yt){
  //return (2.11+0.29*x*x-1.40*x)*2.7667 ;
  return (2.11*yt*yt*yt*yt +   0.29*yt*yt*kl*kl  -1.40*yt*yt*yt*kl)*2.7667/BR;
}


// 
TString getFileForCluster(int l){
  //string lnames[] ={"HHNodebox","HHNodeSM","HHNode2","HHNode3","HHNode4","HHNode5","HHNode6","HHNode7","HHNode8","HHNode9","HHNode10","HHNode11","HHNode12","HHNode13"};
  string lnames[] ={"HHrewclus0","HHrewclus1","HHrewclus2","HHrewclus3","HHrewclus4","HHrewclus5","HHrewclus6","HHrewclus7","HHrewclus8","HHrewclus9","HHrewclus10","HHrewclus11","HHrewoutlier0clus1","HHrewoutlier1clus1","HHrewoutlier2clus1","HHrewoutlier3clus1","HHrewoutlier4clus1","HHrewoutlier5clus1","HHrewoutlier0clus10","HHrewoutlier1clus10","HHrewoutlier2clus10","HHrewoutlier3clus10","HHrewoutlier4clus10","HHrewoutlier5clus10"};
  return lnames[l];

}
TString getFileForLambda(float l, float t){
  if(fabs(t-1)<0.01){
    TString result;
    result.Form("HHrew%d",(int)l+20);
    return result;
  }
  if(t<1){
    if (l<-6)
      return "HHNode9" ;
    else if (l<-5)
      return "HHNode10" ;
    else if (l<-3)
      return "HHNode9" ;
    else if (l<0)
      return "HHNode10" ;
    else if (fabs(l-0)<0.0001)
      return "HHNodebox" ;
    else if (fabs(l-1)<0.0001) 
      return "HHNode6";
    else if (fabs(l-2.46)<0.0001)
      return "HHNode5";
    else if (l>2.46 && l<6)
      return "HHNode8";
    else if (l>6 && l<13)
      return "HHNode8" ;
    else return "HHNode9";

  }
  else if(fabs(t-1)<0.01){
    if (l<-5)
      return "HHNode9" ;
    else if (l<0)
      return "HHNode10" ;
    else if (fabs(l-0)<0.0001)
      return "HHNodebox" ;
    else if (fabs(l-1)<0.0001) 
      return "HHNodeSM";
    else if (fabs(l-2.46)<0.0001)
      return "HHNode2";
    else if (l>2.46 && l<13)
      return "HHNode8" ;
    else return "HHNode9";
  }
  else{
    if (l<-6)
      return "HHNode9" ;
    else if (l<0)
      return "HHNode10" ;
    else if (fabs(l-0)<0.0001)
      return "HHNodebox" ;
    else if (fabs(l-1)<0.0001) 
      return "HHNodeSM";
    else if (fabs(l-2.46)<0.0001)
      return "HHNode6";
    else if (l>2.46 && l<5)
      return "HHNode6";
    else if (l>5 && l<13)
      return "HHNode8" ;
    else return "HHNode9";



  }
}

TLegend *buildLegend(){
  float x[4]={0.5803571,0.110119,0.8928571,0.3452381};
  if(fabs(yt-1)>0.05) {x[0] = 0.1803571;x[2]=0.4928571;}
  int ci;
  TLegend *leg = new TLegend(x[0],x[1],x[2],x[3],NULL,"brNDC");
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
  if(addObsLimit){
     entry=leg->AddEntry("NULL","Observed CLs","p");
     ci = TColor::GetColor("#0000ff");
     entry->SetLineColor(1);
     entry->SetLineStyle(1);
     entry->SetLineWidth(3);
     entry->SetMarkerColor(1);
     entry->SetMarkerStyle(20);
     entry->SetMarkerSize(1.0);
     entry->SetTextFont(42);
  }
   entry=leg->AddEntry("NULL","Expected CLs","p");
   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   entry->SetLineStyle(2);
   entry->SetLineWidth(3);
   //entry->SetLineColor(0);
   entry->SetMarkerColor(kBlue);
   entry->SetMarkerStyle(5);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("NULL","Expected #pm 1#sigma","f");
   ci = TColor::GetColor("#00ff00");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);
   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   //entry->SetLineStyle(2);
   //entry->SetLineWidth(3);
   entry->SetLineColor(0);
   entry->SetTextFont(42);
   entry=leg->AddEntry("NULL","Expected #pm 2#sigma","f");
   ci = TColor::GetColor("#ffff00");
   entry->SetFillColor(ci);
   entry->SetFillStyle(1001);
   ci = TColor::GetColor("#0000ff");
   entry->SetLineColor(ci);
   //entry->SetLineStyle(2);
   //entry->SetLineWidth(3);
   entry->SetLineColor(0);
   entry->SetTextFont(42);
   return leg;
   //leg->Draw();
   // <- end Legend

}
