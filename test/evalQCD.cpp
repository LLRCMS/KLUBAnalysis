#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cctype>
#include <algorithm>
#include "TString.h"
#include "TChain.h"
#include "TCut.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TF2.h"
#include "TGraph2DErrors.h"

#include "HistoManager.h"
#include "ConfigParser.h"
#include "utils.h"
#include "histoUtils.h"
#include "plotContainer.h"
#include "analysisUtils.h"
#include "TRandom3.h"
using namespace std ;


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

TGraphAsymmErrors* FitShape(TH1F * htight, TH1F * hloose){
      //Poisson errors are a mess, but for this we can stay on gaussian...
  int ip = 0;
  for(int i = 0; i<htight->GetNbinsX();i++){
    float n1 = htight->GetBinContent(i+1);
    float n2 = hloose->GetBinContent(i+1);
    if(n1<=0)continue;
    if(n2<=0)continue;
    ip++;
  }
  TGraphAsymmErrors *g = new TGraphAsymmErrors(ip);// = new TGraphAsymmErrors();//htight,hloose,"pois");
  
  for(int i = 0; i<htight->GetNbinsX();i++){
    float x = htight->GetXaxis()->GetBinCenter(i+1);
    float n1 = htight->GetBinContent(i+1);
    float n2 = hloose->GetBinContent(i+1);
    if(n1<=0)continue;
    if(n2<=0)continue;
  /*float n1_up = htight->GetBinContent(i+1)+htight->GetBinErrorUp(i+1);
    float n1_down = htight->GetBinContent(i+1)-htight->GetBinErrorLow(i+1);
    float n2_up = hloose->GetBinContent(i+1)+hloose->GetBinErrorUp(i+1);
    float n2_down = hloose->GetBinContent(i+1)-hloose->GetBinErrorLow(i+1);
    float epsilon = n1/(n1+n2);
    float epsilonUp = n1_up*n2_up/(n1_up+n2_up)/(n1_up+n2_up);
    float epsilonDown = n1_down*n2_down/(n1_down+n2_down)/(n1_down+n2_down);
    float fepsi = 1/(1.-epsilon)/(1.-epsilon);*/
    g->SetPoint(ip,x,n1/n2);
    float eyh = htight->GetBinErrorUp(i+1)/htight->GetBinContent(i+1)+hloose->GetBinErrorUp(i+1)/hloose->GetBinContent(i+1);/*fepsi * epsilonUp;*/
    eyh = eyh * n1/n2;
    float eyl = htight->GetBinErrorLow(i+1)/htight->GetBinContent(i+1)+hloose->GetBinErrorLow(i+1)/hloose->GetBinContent(i+1);/*fepsi * epsilonDown;*/
    eyl = eyl * n1/n2;
    //cout<<"mass "<<x<<" eyh "<<eyh<<" eyl "<<eyl<<" n1,n2 "<<n1<<","<<n2<<endl;
    float exh = htight->GetXaxis()->GetBinLowEdge(i+2) - x;
    float exl = x - htight->GetXaxis()->GetBinLowEdge(i+1);
    g->SetPointError(ip,exl,exh,eyl,eyh);
    ip++;
  }
  return g;  
}

TGraph2DErrors* FitShape2D(TH2F * htight, TH2F * hloose){
      //Poisson errors are a mess, but for this we can stay on gaussian...
  int ip = 0;
  for(int i = 0; i<htight->GetNbinsX();i++){
    for(int j = 0; j<htight->GetNbinsY();j++){
      float n1 = htight->GetBinContent(i+1,j+1);
      float n2 = hloose->GetBinContent(i+1,j+1);
      if(n1<=0)continue;
      if(n2<=0)continue;
      ip++;
    }
  }
  if (ip==0)ip=1;
  TGraph2DErrors *g = new TGraph2DErrors(ip);// = new TGraphAsymmErrors();//htight,hloose,"pois");
  
  for(int i = 0; i<htight->GetNbinsX();i++){
    for(int j = 0; i<htight->GetNbinsY();i++){
      float x = htight->GetXaxis()->GetBinCenter(i+1);
      float y = htight->GetYaxis()->GetBinCenter(j+1);
      float n1 = htight->GetBinContent(i+1,j+1);
      float n2 = hloose->GetBinContent(i+1,j+1);
      if(n1<=0)continue;
      if(n2<=0)continue;
  /*float n1_up = htight->GetBinContent(i+1)+htight->GetBinErrorUp(i+1);
    float n1_down = htight->GetBinContent(i+1)-htight->GetBinErrorLow(i+1);
    float n2_up = hloose->GetBinContent(i+1)+hloose->GetBinErrorUp(i+1);
    float n2_down = hloose->GetBinContent(i+1)-hloose->GetBinErrorLow(i+1);
    float epsilon = n1/(n1+n2);
    float epsilonUp = n1_up*n2_up/(n1_up+n2_up)/(n1_up+n2_up);
    float epsilonDown = n1_down*n2_down/(n1_down+n2_down)/(n1_down+n2_down);
    float fepsi = 1/(1.-epsilon)/(1.-epsilon);*/
    g->SetPoint(ip,x,y,n1/n2);
    float ezh = htight->GetBinError(i+1,j+1)/htight->GetBinContent(i+1,j+1)+hloose->GetBinError(i+1,j+1)/hloose->GetBinContent(i+1,j+1);/*fepsi * epsilonUp;*/
    ezh = ezh * n1/n2;
    //float ezl = htight->GetBinErrorLow(i+1,j+1)/htight->GetBinContent(i+1,j+1)+hloose->GetBinErrorLow(i+1,j+1)/hloose->GetBinContent(i+1,j+1);/*fepsi * epsilonDown;*/
    //ezl = ezl * n1/n2;
    //cout<<"mass "<<x<<" eyh "<<eyh<<" eyl "<<eyl<<" n1,n2 "<<n1<<","<<n2<<endl;
    //float exh = htight->GetXaxis()->GetBinLowEdge(i+2) - x;
    float exl = x - htight->GetXaxis()->GetBinLowEdge(i+1);
    //float eyh = htight->GetYaxis()->GetBinLowEdge(j+2) - y;
    float eyl = x - htight->GetYaxis()->GetBinLowEdge(j+1);
    g->SetPointError(ip,exl,eyl,ezh);
  }
  }
  return g;  
}


int main (int argc, char** argv)
{
  // check number of inpt parameters
  if (argc < 2)
    {
      cerr << "Forgot to put the cfg file --> exit " << endl ;
      return 1 ;
    }
  if (gConfigParser) return 1 ;
  gConfigParser = new ConfigParser () ;
  
  int nruns=0;
  if (argc>2){
    TString runs;
    runs.Form("%s",argv[2]);
    nruns = runs.Atoi();
  }
  TString config ; 
  config.Form ("%s",argv[1]) ;
  if (! (gConfigParser->init (config)))
    {
      cout << ">>> parseConfigFile::Could not open configuration file " << config << endl ;
      return -1 ;
    }

  float lumi = gConfigParser->readFloatOption ("general::lumi") ;
  cout << "READING lumi " << lumi << endl ;

  int maxEvtsMC = -1;
  if (gConfigParser->isDefined ("general::maxEvtsMC"))
        maxEvtsMC = gConfigParser -> readIntOption ("general::maxEvtsMC");

  bool doOverUnderFlow = false;

  // prepare files to contain tree with selectd events

  TString treeFileName = gConfigParser->readStringOption ("general::outputFolderName") ;
  treeFileName += "/" ;
  system (TString ("mkdir -p ") + treeFileName) ;
  treeFileName += gConfigParser->readStringOption ("evalQCD::outputFolderName") ;
  treeFileName += "/filtered_trees/" ;  
  system (TString ("mkdir -p ") + treeFileName) ;
  treeFileName += "filteredTrees.root";
  TFile* fFilteredTrees = new TFile (treeFileName, "RECREATE");

  // get the samples to be analised
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  // isolated samples
  // .... .... .... .... .... .... .... .... .... .... .... ....
  
  vector<string> sigSamplesList = gConfigParser->readStringListOption ("general::signals") ;
  vector<sample> sigSamples ;
  readSamples (sigSamples, sigSamplesList) ;

  vector<float> signalScales ;
  for (unsigned int i = 0 ; i < sigSamplesList.size () ; ++i)
    {
      string name = string ("samples::") + sigSamplesList.at (i) + string ("FACT") ;
      float scale = gConfigParser->readFloatOption (name.c_str ()) ;
      signalScales.push_back (scale) ;        
    }

  vector<string> bkgSamplesList = gConfigParser->readStringListOption ("general::backgrounds") ;
  vector<sample> bkgSamples ;
  readSamples (bkgSamples, bkgSamplesList) ;

  vector<string> DATASamplesList = gConfigParser->readStringListOption ("general::data") ;
  vector<sample> DATASamples ;
  readSamples (DATASamples, DATASamplesList) ;

/*
  // NOT isolated samples
  // not needed anymore from Skims_7_NI because we don't ask isolation during the skim
  // .... .... .... .... .... .... .... .... .... .... .... ....
  
  vector<string> NI_sigSamplesList = gConfigParser->readStringListOption ("general::NIsignals") ;
  vector<sample> NI_sigSamples ;
  readSamples (NI_sigSamples, NI_sigSamplesList) ;

  vector<string> NI_bkgSamplesList = gConfigParser->readStringListOption ("general::NIbackgrounds") ;
  vector<sample> NI_bkgSamples ;
  readSamples (NI_bkgSamples, NI_bkgSamplesList) ;

  vector<string> NI_DATASamplesList = gConfigParser->readStringListOption ("general::NIdata") ;
  vector<sample> NI_DATASamples ;
  readSamples (NI_DATASamples, NI_DATASamplesList) ;
*/

  // get the selections to be applied
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<string> activeSelections = gConfigParser->readStringListOption ("selections::list") ;
  vector<pair <TString, TCut> > selections = readCutsFile (
      activeSelections,
      gConfigParser->readStringOption ("selections::selectionsFile")
    ) ;

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << "selections sequence: \n" ;
  for (unsigned int i = 0 ; i < selections.size () ; ++i)
    cout << selections.at (i).first << " : " << selections.at (i).second << endl ;

  // input selection must not have explicit iso on dau1, dau2 because QCD is relaxed and this selection is added here
  for (unsigned int i = 0 ; i < selections.size () ; ++i)
  {
    TString sel (selections.at (i).second);
    if ( sel.Contains("dau1_iso") || sel.Contains("dau2_iso") )
    {  
      cout << endl;
      cout << " ---------------------------------------------------------------------------------- " << endl;
      cout << " ** WARNING: selection " << selections.at(i).first << " explicitly contains iso cut" << endl; 
      cout << " ---------------------------------------------------------------------------------- " << endl;
      cout << endl;
    }
  }

  string sel_dau1_iso = gConfigParser->readStringOption ("selections::dau1iso");
  string sel_dau2_iso = gConfigParser->readStringOption ("selections::dau2iso");
  string sel_dau1_RLXiso = gConfigParser->readStringOption ("selections::dau1RLXiso");
  string sel_dau2_RLXiso = gConfigParser->readStringOption ("selections::dau2RLXiso");

  // ----------------------------------------
  // OS selections
  vector<pair <TString, TCut> > selections_OS = selections ;
  TCut dau1Cut = Form("dau1_iso < %s" , sel_dau1_iso.c_str());
  TCut dau2Cut = Form("dau2_iso < %s" , sel_dau2_iso.c_str());
  for (unsigned int i = 0 ; i < selections_OS.size () ; ++i)
    {
      selections_OS.at (i).first = TString ("OS_") + selections_OS.at (i).first ;
      selections_OS.at (i).second = selections_OS.at (i).second && TCut ("isOS != 0") && dau1Cut && dau2Cut;
    }

  // OS selections with rlx iso for SS/OS yield
  vector<pair <TString, TCut> > selections_OS_rlxIso = selections ;
  TCut dau1CutRLX = Form("dau1_iso < %s" , sel_dau1_RLXiso.c_str());
  TCut dau2CutRLX = Form("dau2_iso < %s" , sel_dau2_RLXiso.c_str());
  for (unsigned int i = 0 ; i < selections_OS_rlxIso.size () ; ++i)
    {
      selections_OS_rlxIso.at (i).first = TString ("OS_") + selections_OS.at (i).first ;
      selections_OS_rlxIso.at (i).second = selections_OS.at (i).second && TCut ("isOS != 0") && dau1Cut && dau2Cut;
    }
     
  // SS selections with tight iso (for QCD yield determination)
  vector<pair <TString, TCut> > selections_SS_tightIso = selections ;
  //TRandom3 *g = new TRandom3();
  for (unsigned int i = 0 ; i < selections_SS_tightIso.size () ; ++i)
    {
      selections_SS_tightIso.at (i).first = TString ("SS_tightIso_") + selections_SS_tightIso.at (i).first ;
      selections_SS_tightIso.at (i).second = selections_SS_tightIso.at (i).second && TCut ("isOS == 0") && dau1Cut && dau2Cut;
    }
     
  // SS selections with rlx iso for QCD shape
  vector<pair <TString, TCut> > selections_SS = selections ;
  for (unsigned int i = 0 ; i < selections_SS.size () ; ++i)
    {
      selections_SS.at (i).first = TString ("SS_") + selections_SS.at (i).first ;
      selections_SS.at (i).second = selections_SS.at (i).second && TCut ("isOS == 0") && dau1CutRLX && dau2CutRLX;
    }

  // get the variables to be plotted
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<string> variablesList = gConfigParser->readStringListOption ("general::variables") ;
  vector<string> Buf_variables2DList(0);
  if (gConfigParser->isDefined ("general::2Dvariables")) Buf_variables2DList = gConfigParser->readStringListOption ("general::2Dvariables") ;
  
  vector<pair<string,string> > variables2DList ;
  for (unsigned int i = 0 ; i < Buf_variables2DList.size () ; ++i)
    {
      vector<string> dummy = split (Buf_variables2DList.at (i), ':') ;
      //cout << dummy.at (0) << " " << dummy.at (1) << " " << dummy.size () << endl ;
      variables2DList.push_back (make_pair(dummy.at (0), dummy.at (1)) ) ;
    }



  // calculate the QCD in the SS region as data - other_bkg
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  cout << "--- MAIN reading DATA and filling SS histos" << endl ;

  // get the same-sign distributions from data
  plotContainer SS_DATA_plots ("SS_DATA", variablesList, variables2DList, selections_SS, DATASamplesList, 2) ;
  counters SS_DATACount = fillHistos (DATASamples, SS_DATA_plots, 
              variablesList, variables2DList,
              selections_SS,
              lumi,
              vector<float> (0),
              true, false) ;
  if (doOverUnderFlow) SS_DATA_plots.AddOverAndUnderFlow () ;

  cout << "--- MAIN reading bkg and filling SS histos" << endl ;

  // get the same-sign distributions from bkg
  plotContainer SS_bkg_plots ("SS_bkg", variablesList, variables2DList, selections_SS, bkgSamplesList, 0) ;
  counters SS_bkgCount = fillHistos (bkgSamples, SS_bkg_plots, 
              variablesList, variables2DList,
              selections_SS,
              lumi,
              vector<float> (0),
              false, false, maxEvtsMC) ;
  if (doOverUnderFlow) SS_bkg_plots.AddOverAndUnderFlow () ;

  // get the same-sign distributions from bkg
  plotContainer SS_sig_plots ("SS_sig", variablesList, variables2DList, selections_SS, sigSamplesList, 1) ;
  counters SS_sigCount = fillHistos (sigSamples, SS_sig_plots, 
              variablesList, variables2DList,
              selections_SS,
              lumi,
              signalScales,
              false, true) ;
  if (doOverUnderFlow) SS_sig_plots.AddOverAndUnderFlow () ;

  cout << "--- MAIN preparing to loop on variables and selections to calc SS QCD" << endl ;

  // the index in the stack is based on variable ID (iv) and selection ID (isel):
  // iHisto = iv + nVars * isel
  vector<string> QCDsample ;
  QCDsample.push_back ("QCD") ;
  plotContainer SS_QCD ("SS_QCD", variablesList, variables2DList, selections_SS, QCDsample, 0) ;
//  vector <TH1F *> SS_QCD ;
  vector<vector<float>> QCDyieldSSregionRLXiso (variablesList.size(), vector<float>(selections_SS.size()) ); // var, cut
  vector<vector<float>> QCDyieldSSregionRLXiso2D (variables2DList.size(), vector<float>(selections_SS.size()) ); // 2Dvar, cut (for 2D histos)

  for (unsigned int icut = 0 ; icut < selections_SS.size () ; ++icut)
  {
    for (unsigned int ivar = 0 ; ivar < variablesList.size () ; ++ivar)
      {
          THStack * D_stack = SS_DATA_plots.makeStack (variablesList.at (ivar),
                                  selections_SS.at (icut).first.Data ()) ;
          TH1F * tempo = (TH1F *) D_stack->GetStack ()->Last () ;
          TString name = tempo->GetName () ;
          name = TString ("DDQCD_") + name ;
          TH1F * dummy = (TH1F *) tempo->Clone (name) ;

          THStack * b_stack = SS_bkg_plots.makeStack (variablesList.at (ivar),
                                  selections_SS.at (icut).first.Data ()) ;
          TH1F * h_bkg = (TH1F *) b_stack->GetStack ()->Last () ;
          dummy->Add (h_bkg, -1) ;
          SS_QCD.m_histos[variablesList.at (ivar)][selections_SS.at (icut).first.Data ()]["QCD"] = dummy ;
          QCDyieldSSregionRLXiso.at(ivar).at(icut) = dummy->Integral();
        }
    for (unsigned int i2dvar = 0 ; i2dvar < variables2DList.size () ; ++i2dvar)
      {
          THStack * D_stack = SS_DATA_plots.make2DStack (variables2DList.at (i2dvar),
                                  selections_SS.at (icut).first.Data ()) ;
          TH2F * tempo = (TH2F *) D_stack->GetStack ()->Last () ;
          TString name = tempo->GetName () ;
          name = TString ("DDQCD2d_") + name ;
          TH2F * dummy = (TH2F *) tempo->Clone (name) ;

          THStack * b_stack = SS_bkg_plots.make2DStack (variables2DList.at (i2dvar),
                                  selections_SS.at (icut).first.Data ()) ;
          TH2F * h_bkg = (TH2F *) b_stack->GetStack ()->Last () ;
          dummy->Add (h_bkg, -1) ;
          SS_QCD.m_2Dhistos[variables2DList.at(i2dvar).first + variables2DList.at(i2dvar).second][selections_SS.at (icut).first.Data ()]["QCD"] = dummy ;
          QCDyieldSSregionRLXiso2D.at(i2dvar).at(icut) = dummy->Integral();
        }

    }

  /* FIXME should we subtract signals as well? */
  /* NB if it has to be subtracted, it cannot be scaled! */

cout << "--- MAIN reading and filling OS histos with relaxed ISO" << endl ;

  // get the same-sign distributions from data
  plotContainer OS_rlxIso_DATA_plots ("OS_rlxIso_DATA", variablesList, variables2DList, selections_OS_rlxIso, DATASamplesList, 2) ;
  counters OS_rlxIso_DATACount = fillHistos (DATASamples, OS_rlxIso_DATA_plots, 
              variablesList, variables2DList,
              selections_OS_rlxIso,
              lumi,
              vector<float> (0),
              true, false) ;
  OS_rlxIso_DATA_plots.AddOverAndUnderFlow () ;

  // get the same-sign distributions from bkg
  plotContainer OS_rlxIso_bkg_plots ("OS_rlxIso_bkg", variablesList, variables2DList, selections_OS_rlxIso, bkgSamplesList, 0) ;
  counters OS_rlxIso_bkgCount = fillHistos (bkgSamples, OS_rlxIso_bkg_plots, 
              variablesList, variables2DList,
              selections_OS_rlxIso,
              lumi,
              vector<float> (0),
              false, false, maxEvtsMC) ;
  OS_rlxIso_bkg_plots.AddOverAndUnderFlow () ;

  plotContainer OS_rlxIso_QCD ("OS_rlxIso_QCD", variablesList, variables2DList, selections_OS_rlxIso, QCDsample, 0) ;

  for (unsigned int icut = 0 ; icut < selections_OS_rlxIso.size () ; ++icut)
  {
    for (unsigned int ivar = 0 ; ivar < variablesList.size () ; ++ivar)
      {
          THStack * D_stack = OS_rlxIso_DATA_plots.makeStack (variablesList.at (ivar),
                                  selections_OS_rlxIso.at (icut).first.Data ()) ;
          TH1F * tempo = (TH1F *) D_stack->GetStack ()->Last () ;
          TString name = tempo->GetName () ;
          name = TString ("DDQCD_") + name ;
          TH1F * dummy = (TH1F *) tempo->Clone (name) ;

          THStack * b_stack = OS_rlxIso_bkg_plots.makeStack (variablesList.at (ivar),
                                  selections_OS_rlxIso.at (icut).first.Data ()) ;
          TH1F * h_bkg = (TH1F *) b_stack->GetStack ()->Last () ;
          dummy->Add (h_bkg, -1) ;
          OS_rlxIso_QCD.m_histos[variablesList.at (ivar)][selections_OS_rlxIso.at (icut).first.Data ()]["QCD"] = dummy ;
        }
    for (unsigned int i2dvar = 0 ; i2dvar < variables2DList.size () ; ++i2dvar)
      {
          THStack * D_stack = OS_rlxIso_DATA_plots.make2DStack (variables2DList.at (i2dvar),
                                  selections_OS_rlxIso.at (icut).first.Data ()) ;
          TH2F * tempo = (TH2F *) D_stack->GetStack ()->Last () ;
          TString name = tempo->GetName () ;
          name = TString ("DDQCD2d_") + name ;
          TH2F * dummy = (TH2F *) tempo->Clone (name) ;

          THStack * b_stack = OS_rlxIso_bkg_plots.make2DStack (variables2DList.at (i2dvar),
                                  selections_OS_rlxIso.at (icut).first.Data ()) ;
          TH2F * h_bkg = (TH2F *) b_stack->GetStack ()->Last () ;
          dummy->Add (h_bkg, -1) ;
          OS_rlxIso_QCD.m_2Dhistos[variables2DList.at(i2dvar).first + variables2DList.at(i2dvar).second][selections_OS_rlxIso.at (icut).first.Data ()]["QCD"] = dummy ;
        }

    }

  // now get QCD yields with the non-relaxed selections

  cout << "--- MAIN reading DATA and filling SS histos with non-relaxed iso" << endl ;

  // get the same-sign distributions from data
  plotContainer SS_tightIso_DATA_plots ("SS_tightIso_DATA", variablesList, variables2DList, selections_SS_tightIso, DATASamplesList, 2) ;
  counters SS_tightIso_DATACount = fillHistos (DATASamples, SS_tightIso_DATA_plots, 
              variablesList, variables2DList,
              selections_SS_tightIso,
              lumi,
              vector<float> (0),
              true, false) ;
  if (doOverUnderFlow) SS_tightIso_DATA_plots.AddOverAndUnderFlow () ;

  cout << "--- MAIN reading bkg and filling SS histos with non-relaxed iso" << endl ;

  // get the same-sign distributions from bkg
  plotContainer SS_tightIso_bkg_plots ("SS_tightIso_bkg", variablesList, variables2DList, selections_SS_tightIso, bkgSamplesList, 0) ;
  counters SS_tightIso_bkgCount = fillHistos (bkgSamples, SS_tightIso_bkg_plots, 
              variablesList, variables2DList,
              selections_SS_tightIso,
              lumi,
              vector<float> (0),
              false, false, maxEvtsMC) ;
  if (doOverUnderFlow) SS_tightIso_bkg_plots.AddOverAndUnderFlow () ;

  
  // get the same-sign distributions from bkg
  plotContainer SS_tightIso_sig_plots ("SS_tightIso_sig", variablesList, variables2DList, selections_SS_tightIso, sigSamplesList, 1) ;
  counters SS_tightIso_sigCount = fillHistos (sigSamples, SS_tightIso_sig_plots, 
              variablesList, variables2DList,
              selections_SS_tightIso,
              lumi,
              signalScales,
              false, true) ;
  if (doOverUnderFlow) SS_tightIso_sig_plots.AddOverAndUnderFlow () ;
  
  cout << "--- MAIN preparing to loop on variables and selections to calc SS QCD yield with non-relaxed iso" << endl ;

  // the index in the stack is based on variable ID (iv) and selection ID (isel):
  // iHisto = iv + nVars * isel
  
  //vector<string> QCDsample ;
  //QCDsample.push_back ("QCD") ;
  plotContainer SS_QCD_tightIso ("SS_tightIso_QCD", variablesList, variables2DList, selections_SS_tightIso, QCDsample, 0) ;
//  vector <TH1F *> SS_QCD ;
  vector<vector<float>> QCDyieldSSregionTightIso (variablesList.size(), vector<float>(selections_SS_tightIso.size()) ); // var, cut
  vector<vector<float>> QCDyieldSSregionTightIso2D (variables2DList.size(), vector<float>(selections_SS_tightIso.size()) ); // var, cut
  
 for (unsigned int icut = 0 ; icut < selections_SS_tightIso.size () ; ++icut)
      {
    for (unsigned int ivar = 0 ; ivar < variablesList.size () ; ++ivar)
      {     
          THStack * D_stack = SS_tightIso_DATA_plots.makeStack (variablesList.at (ivar),
                                  selections_SS_tightIso.at (icut).first.Data ()) ;
          TH1F * tempo = (TH1F *) D_stack->GetStack ()->Last () ;
          TString name = tempo->GetName () ;
          name = TString ("DDQCD_tightIso_") + name ;
          TH1F * dummy = (TH1F *) tempo->Clone (name) ;

          THStack * b_stack = SS_tightIso_bkg_plots.makeStack (variablesList.at (ivar),
                                  selections_SS_tightIso.at (icut).first.Data ()) ;
          TH1F * h_bkg = (TH1F *) b_stack->GetStack ()->Last () ;
          dummy->Add (h_bkg, -1) ;

          QCDyieldSSregionTightIso.at(ivar).at(icut) = dummy->Integral(); // if AddUnderAndOverFlow is called they will be all identical
          SS_QCD_tightIso.m_histos[variablesList.at (ivar)][selections_SS.at (icut).first.Data ()]["QCD"] = dummy ;
        }
    
    for (unsigned int i2dvar = 0 ; i2dvar < variables2DList.size () ; ++i2dvar)
      {     
          THStack * D_stack = SS_tightIso_DATA_plots.make2DStack (variables2DList.at (i2dvar),
                                  selections_SS_tightIso.at (icut).first.Data ()) ;
          TH2F * tempo = (TH2F *) D_stack->GetStack ()->Last () ;
          TString name = tempo->GetName () ;
          name = TString ("DDQCD2d_tightIso_") + name ;
          TH2F * dummy = (TH2F *) tempo->Clone (name) ;

          THStack * b_stack = SS_tightIso_bkg_plots.make2DStack (variables2DList.at (i2dvar),
                                  selections_SS_tightIso.at (icut).first.Data ()) ;
          TH2F * h_bkg = (TH2F *) b_stack->GetStack ()->Last () ;
          dummy->Add (h_bkg, -1) ;

          QCDyieldSSregionTightIso2D.at(i2dvar).at(icut) = dummy->Integral(); // if AddUnderAndOverFlow is called they will be all identical
          SS_QCD_tightIso.m_2Dhistos[variables2DList.at(i2dvar).first + variables2DList.at(i2dvar).second][selections_SS.at (icut).first.Data ()]["QCD"] = dummy ;
        }
    }


  // get the SS-to-OS scale factor and scale the QCD distributions
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  float SStoOS_scaleFactor = 1.06 ; // to be calculated here at a certain moment!!
  
  for (unsigned int icut = 0 ; icut < selections_SS.size () ; ++icut)
  {
    for (unsigned int ivar = 0 ; ivar < variablesList.size () ; ++ivar)
      {
  //        THStack * D_stack = 
   //       TH1F * tempo = (TH1F *) D_stack->GetStack ()->Last () ;
        THStack * D_stack =SS_DATA_plots.makeStack (variablesList.at (ivar),selections_SS.at (icut).first.Data ());
        TH1F * regionD = (TH1F*)D_stack->GetStack ()->Last () ;//SS_DATA_plots.m_histos[variablesList.at (ivar)][selections_SS.at (icut).first.Data ()]["QCD"];
        THStack * D_stack2 =OS_rlxIso_DATA_plots.makeStack (variablesList.at (ivar),selections_OS_rlxIso.at (icut).first.Data ());
        TH1F * regionC = (TH1F*)D_stack2->GetStack ()->Last () ;//OS_rlxIso_DATA_plots.m_histos[variablesList.at (ivar)][selections_OS_rlxIso.at (icut).first.Data ()]["QCD"];
        TF1 *fit = new TF1("dummy","pol1");
        fit->SetRange(250,1000);
        TH1F *ratioCD = (TH1F*)regionC->Clone();
        ratioCD->Divide(regionD);
        TCanvas * c = new TCanvas();
        c->Divide(2,1);
        c->cd(1);
        ratioCD->Draw();
        ratioCD->Fit(fit,"R","SAME");
        c->cd(2);
        regionD->Draw();
        regionC->SetLineColor(kRed+1);
        regionC->Draw("SAME");
        TString name; name.Form("%s%s.root",variablesList.at (ivar).c_str(),selections_OS_rlxIso.at (icut).first.Data());
        //c->SaveAs(name.Data());
        delete fit;
      }
    }
    
//
//
//  SS_QCD_tightIso.m_histos
  SS_QCD.scale (SStoOS_scaleFactor) ;

  cout << "--- MAIN Correcting RLX Iso shape" << endl ;
  plotContainer SS_QCD_CORR ("SS_QCD_CORR", variablesList, variables2DList, selections_SS, QCDsample, 0) ;
  plotContainer SS_QCD_CORRUP ("SS_QCD_CORRUP", variablesList, variables2DList, selections_SS, QCDsample, 0) ;
  plotContainer SS_QCD_CORRDOWN ("SS_QCD_CORRDOWN", variablesList, variables2DList, selections_SS, QCDsample, 0) ;
  // now scale with rlx to tight iso ratio (normalization taken from tight iso region)
  vector<vector<float>> rlxToTightIsoScale (variablesList.size(), vector<float> (selections_SS.size()) );
  vector<vector<float>> rlxToTightIsoScale2D (variables2DList.size(), vector<float> (selections_SS.size()) );
  vector<vector<TH1F*>> rlxToTightIsoScaleFIT(variablesList.size(), vector<TH1F*> (2*selections_SS.size()) );
  vector<vector<TF2*>> rlxToTightIsoScale2DFIT(variables2DList.size(), vector<TF2*> (2*selections_SS.size()) );
  for (unsigned int icut = 0; icut < selections_SS.size(); icut++)
  {
    for (unsigned int ivar = 0; ivar < variablesList.size(); ivar++)
    {
      rlxToTightIsoScale.at(ivar).at(icut) = QCDyieldSSregionTightIso.at(ivar).at(icut) / QCDyieldSSregionRLXiso.at(ivar).at(icut) ;

      //TString fitname; fitname.Form("FIT%s%s",variablesList.at (ivar).c_str(),selections_SS.at (icut).first.Data ());
      //rlxToTightIsoScaleFIT.at(ivar).at(icut) = new TF1(fitname.Data(),"pol1");//[0]*x+{1}*y+[2] for 2D
      //cout<<fitname.Data()<<"  "<<rlxToTightIsoScale.at(ivar).at(icut)<<endl;

      TH1F *dt = SS_QCD_tightIso.m_histos[variablesList.at (ivar)][selections_SS.at (icut).first.Data ()]["QCD"];
      TH1F *htight = new TH1F("htight","htight",dt->GetNbinsX(),dt->GetBinLowEdge(1),dt->GetBinLowEdge(dt->GetNbinsX())+dt->GetBinWidth(dt->GetNbinsX()));
      TH1F *dl = SS_QCD.m_histos[variablesList.at (ivar)][selections_SS.at (icut).first.Data ()]["QCD"];
      TH1F *hloose = new TH1F("hloose","hloose",dl->GetNbinsX(),dl->GetBinLowEdge(1),dl->GetBinLowEdge(dl->GetNbinsX())+dl->GetBinWidth(dl->GetNbinsX()));
      for(int i = 0; i<dt->GetNbinsX();i++){
        htight->SetBinContent(i+1,dt->GetBinContent(i+1));
        hloose->SetBinContent(i+1,dl->GetBinContent(i+1));
      }

      //htight->Rebin(2);
      //hloose->Rebin(2);
      hloose->Sumw2();//SetBinErrorOption(TH1::kPoisson);//FIXME only works with all positive histos
      htight->Sumw2();//etBinErrorOption(TH1::kPoisson);
      //TH1F * g = (TH1F*)htight->Clone("ratio");
      //g->Divide(hloose);
      TF1 *fit = new TF1("dummy1","pol1");
      TGraphAsymmErrors *g = FitShape(htight,hloose);
      
      cout << "FITTING "<<selections_SS.at (icut).first.Data ()<<" FOR VAR "<<variablesList.at (ivar) <<endl;
      //TF1 *fit = new TF1()
      int fitStatus = g->Fit(fit);
      TF1 *fitcheck = new TF1("dummy0","pol0");
      g->Fit(fitcheck);

      float delta = htight->GetXaxis()->GetXmax()-htight->GetXaxis()->GetXmin();
      float media = 1.0/delta*(fit->GetParameter(0)+fit->GetParameter(1)*delta/2.0);
      if(fitStatus!=0 || fabs(media-fitcheck->GetParameter(0))/media>0.02){
        htight->Rebin(2);
        hloose->Rebin(2);
        g = FitShape(htight,hloose);
        g->Fit(fit);
      }
      //cout<<"status "<<fitStatus<<endl;
      //cout<<"Integral "<<rlxToTightIsoScale.at(ivar).at(icut)<<endl;
      //cout<<"pol0 "<<fitcheck->GetParameter(0)<<endl;
      //cout<<"pol1 "<<fit->GetParameter(0)+fit->GetParameter(1)*500<<endl;

      TH1F * h = (TH1F*)dl->Clone("temp");
      TH1F * hu = (TH1F*)dl->Clone("tempUp");
      TH1F * hd = (TH1F*)dl->Clone("tempDo");

      TF1 *fitup = new TF1("dummyup","pol1");
      TF1 *fitdo = new TF1("dummydo","pol1");
      fitup->SetParameter(0,fit->GetParameter(0)+fit->GetParError(0));
      fitup->SetParameter(1,fit->GetParameter(1)+fit->GetParError(1));
      fitdo->SetParameter(0,fit->GetParameter(0)-fit->GetParError(0));
      fitdo->SetParameter(1,fit->GetParameter(1)-fit->GetParError(1));
      TString nameorig = dl->GetName () ;
      TString name = TString ("CORR_") + nameorig ;
      h->SetName(name.Data());
      h->SetTitle(name.Data());
      name = TString ("UPCORR_") + nameorig ;
      hu->SetName(name.Data());
      hu->SetTitle(name.Data());
      name = TString ("DOWNCORR_") + nameorig ;
      hd->SetName(name.Data());
      hd->SetTitle(name.Data());
      float integral = h->Integral();
      for(int i =0;i<dl->GetNbinsX();i++){
        h->SetBinContent(i+1,h->GetBinContent(i+1)*fit->Eval(h->GetXaxis()->GetBinCenter(i+1)));
        //h->SetBinError(i+1,h->GetBinError(i+1)*fit->Eval(h->GetXaxis()->GetBinCenter(i+1)));
        hu->SetBinContent(i+1,h->GetBinContent(i+1)*fitup->Eval(h->GetXaxis()->GetBinCenter(i+1)));
        //hu->SetBinError(i+1,h->GetBinError(i+1)*fitup->Eval(h->GetXaxis()->GetBinCenter(i+1)));
        hd->SetBinContent(i+1,h->GetBinContent(i+1)*fitdo->Eval(h->GetXaxis()->GetBinCenter(i+1)));
        //hd->SetBinError(i+1,h->GetBinError(i+1)*fitdo->Eval(h->GetXaxis()->GetBinCenter(i+1)));
      }
      h-> Scale(integral/h->Integral());
      hu->Scale(integral/hu->Integral());
      hd->Scale(integral/hd->Integral());


      SS_QCD_CORR.m_histos[variablesList.at (ivar)][selections_SS.at (icut).first.Data ()]["QCD"] = h;
      SS_QCD_CORRUP.m_histos[variablesList.at (ivar)][selections_SS.at (icut).first.Data ()]["QCD"] = hu;
      SS_QCD_CORRDOWN.m_histos[variablesList.at (ivar)][selections_SS.at (icut).first.Data ()]["QCD"] = hd;
      //g = FitShape(htight,hloose);
      //g->Fit(rlxToTightIsoScaleFIT.at(ivar).at(icut));
      //g->Fit(fitcheck);
      //cout<<"REBINNED"<<endl;
      //cout<<"pol0 "<<fitcheck->GetParameter(0)<<endl;
      //cout<<"pol1 "<<fit->GetParameter(0)+fit->GetParameter(1)*500<<endl;
      /*
      TCanvas *c = new TCanvas();
      c->Divide(3,1);
      c->cd(1);
      g->Draw("ALP");

      c->cd(2);
      htight->Draw();
      c->cd(3);
      hloose->Draw();
      TString outc = selections_SS.at (icut).first.Data () + variablesList.at (ivar) + ".root";
      c->SaveAs(outc.Data());
      */
      delete htight;
      delete hloose;
      delete fit;
      delete fitcheck;
      delete fitup;
      delete fitdo;
      g->Set(0);
      delete g;

    }
    for (unsigned int i2dvar = 0; i2dvar < variables2DList.size(); i2dvar++)
    {
      int ivar = i2dvar;
      rlxToTightIsoScale2D.at(i2dvar).at(icut) = QCDyieldSSregionTightIso2D.at(i2dvar).at(icut) / QCDyieldSSregionRLXiso2D.at(i2dvar).at(icut) ;

      TH2F *dt = SS_QCD_tightIso.m_2Dhistos[variables2DList.at(i2dvar).first + variables2DList.at(i2dvar).second][selections_SS.at (icut).first.Data ()]["QCD"];
      TH2F *htight = new TH2F("htight","htight",
        dt->GetNbinsX(),dt->GetXaxis()->GetBinLowEdge(1),dt->GetXaxis()->GetBinLowEdge(dt->GetNbinsX())+dt->GetXaxis()->GetBinWidth(dt->GetNbinsX()),
        dt->GetNbinsY(),dt->GetYaxis()->GetBinLowEdge(1),dt->GetYaxis()->GetBinLowEdge(dt->GetNbinsY())+dt->GetYaxis()->GetBinWidth(dt->GetNbinsY())
        );
      TH2F *dl = SS_QCD.m_2Dhistos[variables2DList.at(i2dvar).first + variables2DList.at(i2dvar).second][selections_SS.at (icut).first.Data ()]["QCD"];
      TH2F *hloose = new TH2F("hloose","hloose",
       dl->GetNbinsX(),dl->GetXaxis()->GetBinLowEdge(1),dl->GetXaxis()->GetBinLowEdge(dl->GetNbinsX())+dl->GetXaxis()->GetBinWidth(dl->GetNbinsX()),
       dl->GetNbinsY(),dl->GetYaxis()->GetBinLowEdge(1),dl->GetYaxis()->GetBinLowEdge(dl->GetNbinsY())+dl->GetYaxis()->GetBinWidth(dl->GetNbinsY())
       );
      for(int i = 0; i<dt->GetNbinsX();i++){
        for(int j=0;j<dt->GetNbinsY();j++){
          htight->SetBinContent(i+1,j+1,dt->GetBinContent(i+1,j+1));
          hloose->SetBinContent(i+1,j+1,dl->GetBinContent(i+1,j+1));
        }
      }
      //hloose->SetBinErrorOption(TH1::kPoisson);//FIXME only works with all positive histos
      //htight->SetBinErrorOption(TH1::kPoisson);
      hloose->Sumw2();//FIXME only works with all positive histos
      htight->Sumw2();
      TF2 *fit = new TF2("dummy1","[0]+[1]*x+[2]*y");
      TGraph2DErrors *g = FitShape2D(htight,hloose);
      
      //cout << "FITTING "<<selections_SS.at (icut).first.Data ()<<" FOR VAR "<<variables2DList.at (ivar) <<endl;
      //TF1 *fit = new TF1()
      int fitStatus = g->Fit(fit);
      /*   //need to check ho to do the 2D check
      TF2 *fitcheck = new TF1("dummy0","[0]");
      g->Fit(fitcheck);

      float delta = htight->GetXaxis()->GetXmax()-htight->GetXaxis()->GetXmin();
      float media = 1.0/delta*(fit->GetParameter(0)+fit->GetParameter(1)*delta/2.0);
      if(fitStatus!=0 || fabs(media-fitcheck->GetParameter(0))/media>0.02){
        htight->Rebin(2);
        hloose->Rebin(2);
        g = FitShape(htight,hloose);
        g->Fit(fit);
      }

      cout<<"status "<<fitStatus<<endl;
      cout<<"Integral "<<rlxToTightIsoScale.at(ivar).at(icut)<<endl;
      cout<<"pol0 "<<fitcheck->GetParameter(0)<<endl;
      cout<<"pol1 "<<fit->GetParameter(0)+fit->GetParameter(1)*500<<endl;
      */
      TH2F * h = (TH2F*)dl->Clone("temp");
      TH2F * hu = (TH2F*)dl->Clone("tempUp");
      TH2F * hd = (TH2F*)dl->Clone("tempDo");
      TF2 *fitup = new TF2("dummyup","[0]+[1]*x+[2]*y");
      TF2 *fitdo = new TF2("dummydo","[0]+[1]*x+[2]*y");
      fitup->SetParameter(0,fit->GetParameter(0)+fit->GetParError(0));
      fitup->SetParameter(1,fit->GetParameter(1)+fit->GetParError(1));
      fitup->SetParameter(2,fit->GetParameter(2)+fit->GetParError(2));
      fitdo->SetParameter(0,fit->GetParameter(0)-fit->GetParError(0));
      fitdo->SetParameter(1,fit->GetParameter(1)-fit->GetParError(1));
      fitdo->SetParameter(2,fit->GetParameter(2)-fit->GetParError(2));
      TString nameorig = dl->GetName () ;
      TString name = TString ("CORR_") + nameorig ;
      h->SetName(name.Data());
      h->SetTitle(name.Data());
      name = TString ("UPCORR_") + nameorig ;
      hu->SetName(name.Data());
      hu->SetTitle(name.Data());
      name = TString ("DOWNCORR_") + nameorig ;
      hd->SetName(name.Data());
      hd->SetTitle(name.Data());
      float integral=h->Integral();
      for(int i =0;i<dl->GetNbinsX();i++){
        for(int j =0;j<dl->GetNbinsY();j++){
          h->SetBinContent(i+1,j+1,h->GetBinContent(i+1,j+1)*fit->Eval(h->GetXaxis()->GetBinCenter(i+1),h->GetYaxis()->GetBinCenter(j+1)));
          //h->SetBinError(i+1,j+1,h->GetBinError(i+1,j+1)*fit->Eval(h->GetXaxis()->GetBinCenter(i+1),h->GetYaxis()->GetBinCenter(j+1)));
          hu->SetBinContent(i+1,j+1,h->GetBinContent(i+1,j+1)*fitup->Eval(h->GetXaxis()->GetBinCenter(i+1),h->GetYaxis()->GetBinCenter(j+1)));
          //hu->SetBinError(i+1,j+1,h->GetBinError(i+1,j+1)*fitup->Eval(h->GetXaxis()->GetBinCenter(i+1),h->GetYaxis()->GetBinCenter(j+1)));
          hd->SetBinContent(i+1,j+1,h->GetBinContent(i+1,j+1)*fitdo->Eval(h->GetXaxis()->GetBinCenter(i+1),h->GetYaxis()->GetBinCenter(j+1)));
          //hd->SetBinError(i+1,j+1,h->GetBinError(i+1,j+1)*fitdo->Eval(h->GetXaxis()->GetBinCenter(i+1),h->GetYaxis()->GetBinCenter(j+1)));
        }
      }
      h-> Scale(integral/h->Integral());
      hu->Scale(integral/hu->Integral());
      hd->Scale(integral/hd->Integral());
      SS_QCD_CORR.m_2Dhistos[variables2DList.at(i2dvar).first + variables2DList.at(i2dvar).second][selections_SS.at (icut).first.Data ()]["QCD"] = h;
      SS_QCD_CORRUP.m_2Dhistos[variables2DList.at(i2dvar).first + variables2DList.at(i2dvar).second][selections_SS.at (icut).first.Data ()]["QCD"] = hu;
      SS_QCD_CORRDOWN.m_2Dhistos[variables2DList.at(i2dvar).first + variables2DList.at(i2dvar).second][selections_SS.at (icut).first.Data ()]["QCD"] = hd;
      delete htight;
      delete hloose;
      g->Set(0);
      delete g;
    }
  }

  cout << "QCD info: scale from rlx iso to tight iso is: " << rlxToTightIsoScale.at(0).at(0) << endl;

  SS_QCD.scale (variablesList, selections_SS, rlxToTightIsoScale) ;
  SS_QCD_CORR.scale(variablesList, selections_SS, rlxToTightIsoScale);
  SS_QCD_CORRUP.scale(variablesList, selections_SS, rlxToTightIsoScale);
  SS_QCD_CORRDOWN.scale(variablesList, selections_SS, rlxToTightIsoScale);
  SS_QCD.scale2D (variables2DList, selections_SS, rlxToTightIsoScale2D) ;
  SS_QCD_CORR.scale2D(variables2DList, selections_SS, rlxToTightIsoScale2D);
  SS_QCD_CORRUP.scale2D(variables2DList, selections_SS, rlxToTightIsoScale2D);
  SS_QCD_CORRDOWN.scale2D(variables2DList, selections_SS, rlxToTightIsoScale2D);
  int QCDcolor = gConfigParser->readIntOption ("colors::QCD") ;
  SS_QCD.setHistosProperties (0, QCDcolor) ; 
  SS_QCD_CORR.setHistosProperties (0, QCDcolor) ; 

  // insert the QCD in the OS region
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  cout << "--- MAIN reading DATA and filling OS histos" << endl ;

  // get the opposite-sign distributions from data
  plotContainer OS_DATA_plots ("OS_DATA", variablesList, variables2DList, selections_OS, DATASamplesList, 2) ;
  counters OS_DATACount = fillHistos (DATASamples, OS_DATA_plots, 
              variablesList, variables2DList,
              selections_OS,
              lumi,
              vector<float> (0),
              true, false, -1, fFilteredTrees) ;
  if (doOverUnderFlow) OS_DATA_plots.AddOverAndUnderFlow () ;

  cout << "--- MAIN reading bkg and filling OS histos" << endl ;

  // get the opposite-sign distributions from bkg
  plotContainer OS_bkg_plots ("OS_bkg", variablesList, variables2DList, selections_OS, bkgSamplesList, 0) ;
  counters OS_bkgCount = fillHistos (bkgSamples, OS_bkg_plots, 
              variablesList, variables2DList,
              selections_OS,
              lumi,
              vector<float> (0),
              false, false, maxEvtsMC) ;
  if (doOverUnderFlow) OS_bkg_plots.AddOverAndUnderFlow () ;
  OS_bkg_plots.addSample ("QCD", SS_QCD_CORR) ;
  
  cout << "--- MAIN reading sig and filling OS histos" << endl ;

  // get the opposite-sign distributions from sig
  plotContainer OS_sig_plots ("OS_sig", variablesList, variables2DList, selections_OS, sigSamplesList, 1) ;
  counters OS_sigCount = fillHistos (sigSamples, OS_sig_plots, 
              variablesList, variables2DList,
              selections_OS,
              lumi,
              signalScales,
              false, true) ;
  if (doOverUnderFlow) OS_sig_plots.AddOverAndUnderFlow () ;

  // Save the histograms
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  cout << "--- MAIN before saving" << endl ;

  TString outFolderNameBase = gConfigParser->readStringOption ("general::outputFolderName") ;
  outFolderNameBase += "/" ;
  system (TString ("mkdir -p ") + outFolderNameBase) ;
  outFolderNameBase += gConfigParser->readStringOption ("evalQCD::outputFolderName") ;
  outFolderNameBase += "/" ;  
  system (TString ("mkdir -p ") + outFolderNameBase) ;

  TString outString ;
  outString.Form (outFolderNameBase + "outPlotter.root") ;
  TFile * fOut = new TFile (outString.Data (), "RECREATE") ;
  SS_DATA_plots.save (fOut) ;
  SS_bkg_plots.save (fOut) ;
  OS_DATA_plots.save (fOut) ;
  OS_bkg_plots.save (fOut) ;
  OS_sig_plots.save (fOut) ;

  SS_QCD.save (fOut);
  //SS_QCD_CORR.save (fOut); // this is the estimated QCD after scaling to OS region 1.06, already added to OS_bkg
  SS_QCD_CORRDOWN.save (fOut);
  SS_QCD_CORRUP.save (fOut);
  fOut->Close () ;

  // Plot the histograms
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events/")) ;
  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events/rootCanvas/")) ;
  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/shapes/")) ;

  // for test
  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events_log/")) ;
  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/shapes_log/")) ;

  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events_ratio/")) ;
  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events_log_ratio/")) ;

  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events_nonZero/")) ;
  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/shapes_nonZero/")) ;

  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events_nonZero_ratio/")) ;

  // SS iso region (the one used for the QCD)
  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events_SSrlxiso/")) ;
  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events_SStightiso/")) ;

  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events_SSrlxiso_noSig/")) ;
  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events_SStightiso_noSig/")) ;

  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events_noSignal/")) ;
  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events_noData/")) ;
  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events_noDatanoSig/")) ;

  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events_noData/")) ;

  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/canvases2D/")) ;
  TFile* fPlots2D = new TFile (outFolderNameBase + TString ("/canvases2D/2Dplots.root"), "recreate");


  TCanvas * c = new TCanvas ("c", "c", 600, 600) ;
  //TCanvas * c = new TCanvas () ;

  cout << "--- MAIN before plotting" << endl ;

  // retrieve legend names
  vector <pair <string, string> > addToLegend;
  vector <pair <string, string> > addToLegend_bis;
  vector <pair <string, string> > variablesLabels;

  string legendSampleNamesFile = gConfigParser->readStringOption ("labels::sampleLabels");
  addToLegend = readVarNamesFile (sigSamplesList, legendSampleNamesFile);
  addToLegend_bis = readVarNamesFile (bkgSamplesList, legendSampleNamesFile);
  addToLegend.insert (addToLegend.end(), addToLegend_bis.begin(), addToLegend_bis.end());

  string legendVarLabelsFile = gConfigParser->readStringOption ("labels::variableLabels");
  variablesLabels = readVarNamesFile (variablesList, legendVarLabelsFile);
  
  /*
  cout << "LEGEND: " << legendSampleNamesFile << endl;
  for (int i = 0; i < addToLegend.size(); i++) cout << addToLegend.at(i).first << " --> " << addToLegend.at(i).second << endl;
  cout << "LABELS: " << legendVarLabelsFile << endl;
  for (int i = 0; i < variablesLabels.size(); i++) cout << variablesLabels.at(i).first << " --> " << variablesLabels.at(i).second << endl;
  */

  // loop on selections
  for (unsigned int isel = 0 ; isel < selections.size () ; ++isel)
    {
      // loop on variables
      for (unsigned int iv = 0 ; iv < variablesList.size () ; ++iv)
        {
          c->cd () ;
          TString outputName ; 
          outputName.Form ("plot_%s_%s",
            variablesList.at (iv).c_str (), selections.at (isel).first.Data ()) ;


          // TString coutputName ;
          // TString outFolderName ;

          // // get the extremes for the plot
          // THStack * sig_stack = OS_sig_plots.makeStack ( variablesList.at (iv), 
          //                           selections_OS.at (isel).first.Data ()) ;
          // THStack * bkg_stack = OS_bkg_plots.makeStack ( variablesList.at (iv), 
          //                           selections_OS.at (isel).first.Data ()) ;
          // THStack * DATA_stack = OS_DATA_plots.makeStack ( variablesList.at (iv), 
          //                           selections_OS.at (isel).first.Data ()) ;

          // vector<float> extremes_bkg  = getExtremes (bkg_stack) ;
          // vector<float> extremes_sig  = getExtremes (sig_stack) ;
          // vector<float> extremes_DATA = getExtremes (DATA_stack) ;
          // TH1F * bkg = c->DrawFrame (
          //     extremes_bkg.at (0) ,
          //     0.9 * min3 (extremes_bkg.at (1), extremes_sig.at (1), 
          //                 extremes_DATA.at (1)) ,
          //     extremes_bkg.at (2) ,
          //     1.3 * max3 (extremes_bkg.at (3), extremes_sig.at (3), 
          //                 extremes_DATA.at (3) + sqrt (extremes_DATA.at (3)))
          //   ) ;  
     
          // copyTitles (bkg, bkg_stack) ;

          // bkg->Draw () ;
          // bkg_stack->Draw ("hist same") ;
          // sig_stack->Draw ("nostack hist same") ;
          // TH1F * h_data = (TH1F *) DATA_stack->GetStack ()->Last () ;
          // // FIXME probably the data uncertainties need to be fixed
          // h_data->Draw ("same") ;

          // prototype of makeStackPlot:
          // std::vector<TObject*> makeStackPlot (plotContainer& dataPlots, plotContainer& bkgPlots, plotContainer& sigPlots,
          //                             string varName, string selName,
          //                             TCanvas* canvas, std::vector <pair <string, string> > & addInLegend, std::vector <pair <string, string> >& axisTitles,
          //                             bool LogY = false, bool makeRatioPlot = true, bool drawLegend = true, bool doShapes = false, bool forceNonNegMin = false, bool drawGrassForData = false,
          //                             bool drawSignal = true, bool drawData = true, bool drawMC = true) ;

         
          // normal

          TString coutputName ;
          TString outFolderName ;
          
          outFolderName = outFolderNameBase + TString ("/events/") ;
          std::vector<TObject*> drawings_nonScaled = makeStackPlot (OS_DATA_plots, OS_bkg_plots, OS_sig_plots,
                                      variablesList.at (iv), selections_OS.at (isel).first.Data (),
                                      c, addToLegend, variablesLabels, false, false, true, false) ;
                                      //c, addToLegend, variablesLabels, false, false, true, false, true, false, false, true, true) ;

          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;
          // FIXME : save canvases but in a dedicated folder
          
          coutputName.Form ("%s.root", (outFolderName + TString("rootCanvas/") + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ());

          // ---------------
          outFolderName = outFolderNameBase + TString ("/shapes/") ;
          std::vector<TObject*> drawings_nonScaled_1 = makeStackPlot (OS_DATA_plots, OS_bkg_plots, OS_sig_plots,
                                      variablesList.at (iv), selections_OS.at (isel).first.Data (),
                                      c, addToLegend, variablesLabels, false, false, true, true) ;

          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;

          // ---------------
          outFolderName = outFolderNameBase + TString ("/events_log/") ;
          std::vector<TObject*> drawings_nonScaled_2 = makeStackPlot (OS_DATA_plots, OS_bkg_plots, OS_sig_plots,
                                      variablesList.at (iv), selections_OS.at (isel).first.Data (),
                                      c, addToLegend, variablesLabels, true, false, true, false) ;

          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;

          // ---------------
          outFolderName = outFolderNameBase + TString ("/shapes_log/") ;
          std::vector<TObject*> drawings_nonScaled_3 = makeStackPlot (OS_DATA_plots, OS_bkg_plots, OS_sig_plots,
                                      variablesList.at (iv), selections_OS.at (isel).first.Data (),
                                      c, addToLegend, variablesLabels, true, false, true, true) ;

          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;
          // ---------------
          outFolderName = outFolderNameBase + TString ("/events_ratio/") ;
          std::vector<TObject*> drawings_nonScaled_4 = makeStackPlot (OS_DATA_plots, OS_bkg_plots, OS_sig_plots,
                                      variablesList.at (iv), selections_OS.at (isel).first.Data (),
                                      c, addToLegend, variablesLabels, false, true, true, false) ;

          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;

          // ---------------
          outFolderName = outFolderNameBase + TString ("/events_log_ratio/") ;
          std::vector<TObject*> drawings_nonScaled_5 = makeStackPlot (OS_DATA_plots, OS_bkg_plots, OS_sig_plots,
                                      variablesList.at (iv), selections_OS.at (isel).first.Data (),
                                      c, addToLegend, variablesLabels, true, true, true, false) ;

          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;

          // ---------------
          outFolderName = outFolderNameBase + TString ("/events_nonZero_ratio/") ;
          std::vector<TObject*> drawings_nonScaled_6 = makeStackPlot (OS_DATA_plots, OS_bkg_plots, OS_sig_plots,
                                      variablesList.at (iv), selections_OS.at (isel).first.Data (),
                                      c, addToLegend, variablesLabels, false, true, true, false, true) ;

          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;

          // ---------------
          outFolderName = outFolderNameBase + TString ("/shapes_nonZero/") ;
          std::vector<TObject*> drawings_nonScaled_7 = makeStackPlot (OS_DATA_plots, OS_bkg_plots, OS_sig_plots,
                                      variablesList.at (iv), selections_OS.at (isel).first.Data (),
                                      c, addToLegend, variablesLabels, false, false, true, true, true) ;

          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;

          // ---------------
          outFolderName = outFolderNameBase + TString ("/events_SSrlxiso/") ;
          std::vector<TObject*> drawings_nonScaled_8 = makeStackPlot (SS_DATA_plots, SS_bkg_plots, SS_sig_plots,
                                      variablesList.at (iv), selections_SS.at (isel).first.Data (),
                                      c, addToLegend, variablesLabels, false, false, true, false, false) ;

          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;

          // ---------------
          outFolderName = outFolderNameBase + TString ("/events_SStightiso/") ;
          std::vector<TObject*> drawings_nonScaled_9 = makeStackPlot (SS_tightIso_DATA_plots, SS_tightIso_bkg_plots, SS_tightIso_sig_plots,
                                      variablesList.at (iv), selections_SS_tightIso.at (isel).first.Data (),
                                      c, addToLegend, variablesLabels, false, false, true, false, false) ;

          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;

          // ---------------
          outFolderName = outFolderNameBase + TString ("/events_SSrlxiso_noSig/") ;
          std::vector<TObject*> drawings_nonScaled_8bis = makeStackPlot (SS_DATA_plots, SS_bkg_plots, SS_sig_plots,
                                      variablesList.at (iv), selections_SS.at (isel).first.Data (),
                                      c, addToLegend, variablesLabels, false, false, true, false, false, false, false) ;

          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;

          // ---------------
          outFolderName = outFolderNameBase + TString ("/events_SStightiso_noSig/") ;
          std::vector<TObject*> drawings_nonScaled_9bis = makeStackPlot (SS_tightIso_DATA_plots, SS_tightIso_bkg_plots, SS_tightIso_sig_plots,
                                      variablesList.at (iv), selections_SS_tightIso.at (isel).first.Data (),
                                      c, addToLegend, variablesLabels, false, false, true, false, false, false, false) ;

          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;


          // ---------------
          outFolderName = outFolderNameBase + TString ("/events_noSignal/") ;
          std::vector<TObject*> drawings_nonScaled_10 = makeStackPlot (OS_DATA_plots, OS_bkg_plots, OS_sig_plots,
                                      variablesList.at (iv), selections_OS.at (isel).first.Data (),
                                      c, addToLegend, variablesLabels, false, false, true, false, true, false, false) ;

          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;

          // ---------------
          outFolderName = outFolderNameBase + TString ("/events_noData/") ;
          std::vector<TObject*> drawings_nonScaled_11 = makeStackPlot (OS_DATA_plots, OS_bkg_plots, OS_sig_plots,
                                      variablesList.at (iv), selections_OS.at (isel).first.Data (),
                                      c, addToLegend, variablesLabels, false, false, true, false, true, false, true, false, true) ;

          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;

          // ---------------
          outFolderName = outFolderNameBase + TString ("/events_noDatanoSig/") ;
          std::vector<TObject*> drawings_nonScaled_12 = makeStackPlot (OS_DATA_plots, OS_bkg_plots, OS_sig_plots,
                                      variablesList.at (iv), selections_OS.at (isel).first.Data (),
                                      c, addToLegend, variablesLabels, false, false, true, false, true, false, false, false, true) ;

          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;
          
          // ---------------
          outFolderName = outFolderNameBase + TString ("/events_noData/") ;
          std::vector<TObject*> drawings_nonScaled_11 = makeStackPlot (OS_DATA_plots, OS_bkg_plots, OS_sig_plots,
                                      variablesList.at (iv), selections_OS.at (isel).first.Data (),
                                      c, addToLegend, variablesLabels, false, false, true, false, true, false, true, false, true) ;
                    coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
            c->SaveAs (coutputName.Data ()) ;
          // c->SetLogy (1) ;
          // bkg->Draw () ;
          // bkg_stack->Draw ("hist same") ;
          // sig_stack->Draw ("nostack hist same") ;
          // h_data->Draw ("same") ;
          
          // coutputName.Form ("%s_log.pdf", (outFolderName + outputName).Data ()) ;
          // c->SaveAs (coutputName.Data ()) ;
          // c->SetLogy (0) ;
         

          // // plotting shapes
          // // ---- ---- ---- ---- ---- ---- ---- ---- ----

          // outFolderName = outFolderNameBase + TString ("/shapes/") ;
          // TString basename ;
          // basename.Form ("shape_%s_%s",
          //         variablesList.at (iv).c_str (),
          //         selections.at (isel).first.Data ()
          //       ) ;

          // THStack * hstack_bkg_norm = normaliseStack (bkg_stack) ;
          // TH1F * shape_bkg = (TH1F *) hstack_bkg_norm->GetStack ()->Last () ;
          
          // THStack * hstack_sig_norm = normaliseStack (sig_stack) ;
          // TH1F * shape_sig = (TH1F *) hstack_sig_norm->GetStack ()->Last () ;
          
          // if (shape_sig->GetMaximum () > shape_bkg->GetMaximum ()) 
          //   hstack_sig_norm->Draw ("hist") ;
          // else   
          //   hstack_bkg_norm->Draw ("hist") ;

          // hstack_bkg_norm->Draw ("hist same") ;
          // hstack_sig_norm->Draw ("hist same") ;
          
          // TString name = basename + "_norm" ;
          // coutputName.Form ("%s.pdf", (outFolderName + basename).Data ()) ;
          // c->SaveAs (coutputName.Data ()) ;
         

          // delete hstack_bkg_norm;
          // delete hstack_sig_norm;
          // delete sig_stack;
          // delete bkg_stack;
          // delete DATA_stack;
          // for (unsigned int id = 0; id < drawings_nonScaled.size(); id++)
          //   delete drawings_nonScaled.at(id);
          // for (unsigned int id = 0; id < drawings_nonScaled_1.size(); id++)
          //   delete drawings_nonScaled_1.at(id);
          // for (unsigned int id = 0; id < drawings_nonScaled_2.size(); id++)
          //   delete drawings_nonScaled_2.at(id);
          // for (unsigned int id = 0; id < drawings_nonScaled_3.size(); id++)
          //   delete drawings_nonScaled_3.at(id);
          // for (unsigned int id = 0; id < drawings_nonScaled_4.size(); id++)
          //   delete drawings_nonScaled_4.at(id);
          // for (unsigned int id = 0; id < drawings_nonScaled_5.size(); id++)
          //   delete drawings_nonScaled_5.at(id);
          
     
        } // loop on variables
    
        for (unsigned int i2dvar = 0; i2dvar < variables2DList.size(); i2dvar++)
        {
          fPlots2D->cd();
          //TString outFolderName ;
          //outFolderName = outFolderNameBase + TString ("/canvases2D/") ;
          THStack* bkg_2d_stack = OS_bkg_plots.make2DStack (variables2DList.at(i2dvar), selections_OS.at (isel).first.Data()); 
          THStack* DATA_2d_stack = OS_DATA_plots.make2DStack (variables2DList.at(i2dvar), selections_OS.at (isel).first.Data()); 
          THStack* sig_2d_stack = OS_sig_plots.make2DStack (variables2DList.at(i2dvar), selections_OS.at (isel).first.Data()); 
          //coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          bkg_2d_stack->Write();
          DATA_2d_stack->Write();
          sig_2d_stack->Write();
        }
    } // loop on selections

  delete c ;
  fPlots2D->Close();

  // print yields
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  TString yieldsFileName = outFolderNameBase + "/yields.txt";
  std::ofstream yieldsFile (yieldsFileName.Data());

  unsigned int NSpacesColZero = 16;
  unsigned int NSpacesColumns = 10;
/*  
  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " FITS for SYST TIGHT TO LOOSE \n\n" ;
  for (uint ivar = 0 ; ivar < variablesList.size(); ivar++){
    cout<<"   VARIABLE: "<<variablesList.at(ivar) << endl;    
    for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel){
      cout<<"   SELECTION: "<<selections.at(iSel).first<<endl;
      rlxToTightIsoScaleFIT.at(ivar).at(iSel)->Print();
    }
  }
*/
  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " EXPECTED NUMBER OF SIG EVENTS\n\n" ;
  yieldsFile << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  yieldsFile << " EXPECTED NUMBER OF SIG EVENTS\n\n" ;
  
  printTableTitle (std::cout,  sigSamples) ;
  printTableTitle (yieldsFile, sigSamples) ;
  printTableBody  (std::cout,  selections, OS_sigCount, sigSamples) ;
  printTableBody  (yieldsFile, selections, OS_sigCount, sigSamples) ;

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " EFFICIENCIES OF SIG EVENTS\n\n" ;
  yieldsFile << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  yieldsFile << " EFFICIENCIES OF SIG EVENTS\n\n" ;
  printTableTitle (std::cout,  sigSamples) ;
  printTableTitle (yieldsFile, sigSamples) ;
  printTableBodyEff  (std::cout,  selections, OS_sigCount, sigSamples, 4) ;
  printTableBodyEff  (yieldsFile, selections, OS_sigCount, sigSamples, 4) ;



  vector<string> DataDriven_names (1, string("QCD"));
  vector <vector<float>> DataDriven_yields;
  vector<float> QCD_yields;
  // QCD -- prepare yields
  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
  {
        float nQCD = SS_QCD.getHisto (variablesList.at(0), string(selections_SS.at(iSel).first.Data()), QCDsample.at(0)) -> Integral();
        QCD_yields.push_back (nQCD);
  }
  DataDriven_yields.push_back (QCD_yields);

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " EXPECTED NUMBER OF BKG EVENTS\n\n" ;
  yieldsFile << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  yieldsFile << " EXPECTED NUMBER OF BKG EVENTS\n\n" ;
  printTableTitle (std::cout,  bkgSamples, DataDriven_names) ;
  printTableTitle (yieldsFile, bkgSamples, DataDriven_names) ;
  printTableBody  (std::cout,  selections, OS_bkgCount, bkgSamples, DataDriven_yields) ;
  printTableBody  (yieldsFile, selections, OS_bkgCount, bkgSamples, DataDriven_yields) ;
 
  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " EFFICIENCIES OF BKG EVENTS\n\n" ;
  yieldsFile << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  yieldsFile << " EFFICIENCIES OF BKG EVENTS\n\n" ;
  printTableTitle (std::cout,  bkgSamples) ;
  printTableTitle (yieldsFile, bkgSamples) ;
  printTableBodyEff  (std::cout,  selections, OS_bkgCount, bkgSamples, 4) ;
  printTableBodyEff  (yieldsFile, selections, OS_bkgCount, bkgSamples, 4) ;
 

  // mini debug to cjeck that each var has the same QCD yield
  /*  
  // QCD
  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
  {
      for (int ivar = 0 ; ivar < variablesList.size(); ivar++)
      {
        int nQCD = SS_QCD.getHisto (variablesList.at(ivar), string(selections_SS.at(iSel).first.Data()), QCDsample.at(0)) -> Integral();
        cout << nQCD << " ";
      }
      cout <<  "  --- " << endl;
  }
  */

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " OBSERVED NUMBER OF EVENTS\n\n" ;
  yieldsFile << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  yieldsFile << " OBSERVED NUMBER OF EVENTS\n\n" ;
  printTableTitle (std::cout,  DATASamples) ;
  printTableTitle (yieldsFile, DATASamples) ;
  printTableBody  (std::cout,  selections, OS_DATACount, DATASamples) ;
  printTableBody  (yieldsFile, selections, OS_DATACount, DATASamples) ;

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " TOTALS\n\n" ;
  yieldsFile << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  yieldsFile << " TOTALS\n\n" ;
  vector<float> DATAtotal = OS_DATACount.getTotalCountsPerCut () ;
  vector<float> bkgtotal = OS_bkgCount.getTotalCountsPerCut () ;
  vector<string> titles ; titles.push_back ("DATA") ; titles.push_back ("bkg") ;
  printTableTitle (std::cout, titles) ;
  printTableTitle (yieldsFile, titles) ;
  
  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
    {
      cout << selections.at (iSel).first ;
      yieldsFile << selections.at (iSel).first ;
      unsigned int nspacetojump = (NSpacesColZero > string(selections.at (iSel).first.Data ()).size () ? NSpacesColZero - string(selections.at (iSel).first.Data ()).size () : 0);
      for (unsigned int i = 0 ; i < nspacetojump; ++i){
        cout << " " ;
        yieldsFile << " " ;
      } 
      cout << "|" ;
      yieldsFile << "|" ;
      
      float evtnum = DATAtotal.at (iSel+1) ;
      int subtractspace = 0 ;
      if (evtnum > 0) subtractspace = int (log10 (evtnum)) ;
      for (unsigned int i = 0 ; i < NSpacesColumns - subtractspace ; ++i)
      {
        cout << " " ;
        yieldsFile << " " ;
      }
      cout << setprecision (0) << fixed << evtnum
           << " |" ;
      yieldsFile << setprecision (0) << fixed << evtnum
           << " |" ;

      evtnum = bkgtotal.at (iSel+1);
      for (unsigned int iDD = 0; iDD < DataDriven_yields.size(); iDD++) evtnum += DataDriven_yields.at(iDD).at(iSel);
      subtractspace = 0 ;
      if (evtnum > 0) subtractspace = int (log10 (evtnum)) ;
      for (unsigned int i = 0 ; i < NSpacesColumns - subtractspace ; ++i)
      {
          cout << " " ;
          yieldsFile << " " ;
      }
      cout << setprecision (0) << fixed << evtnum
           << " |\n" ;
      yieldsFile << setprecision (0) << fixed << evtnum
           << " |\n" ;
    }
/*
  cout << " ============ MINI DEBUG ON QCD YIELDS ============ " << endl;
  for (unsigned int isel = 0 ; isel < selections.size () ; ++isel)
    {
      // loop on variables
      for (unsigned int iv = 0 ; iv < variablesList.size () ; ++iv) 

    }
*/

  // save on root file for debug
  //TFile* fRootOut = new TFile (outFolderNameBase + "/" + "SS_QCD_histos.root", "recreate");
  //fRootOut->Close();

  return 0 ;
}  
