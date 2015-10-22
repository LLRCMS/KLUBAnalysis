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
#include "TLegend.h"
#include "TStyle.h"

#include "HistoManager.h"
#include "ConfigParser.h"
#include "utils.h"
#include "histoUtils.h"
#include "plotContainer.h"
#include "analysisUtils.h"


using namespace std ;


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// compute the ratio of two identical plotContainer structures and return a third plotContainer
// with  ratio histogram for each variable and selection
// DO: pc1 = OS, pc2 = SS
// it will compute OS/SS

/*
plotContainer* makeRatio (plotContainer& pcOS, plotContainer& pcSS,
                          vector<pair <TString, TCut> >& selections,
                          vector<string>& variablesList,
                          vector <string>& samples,
                          vector <float>& isoThr1,
                          vector <float>& isoThr2
                          )
*/
TH1F* makeRatio (plotContainer& pcOS, plotContainer& pcSS,
                 pair <TString, TCut>& selection,
                 string& varName,
                 string& sampleName,
                 float thr1,
                 float thr2
                )

{
  TString OS_selName = Form("OSaiso_%f_%f_", thr1, thr2);
  TString SS_selName = Form("SSaiso_%f_%f_", thr1, thr2);
  OS_selName = OS_selName + selection.first;
  SS_selName = SS_selName + selection.first;

  TH1F* hOS = pcOS.getHisto (varName, string(OS_selName.Data()), sampleName);
  TH1F* hSS = pcSS.getHisto (varName, string(SS_selName.Data()), sampleName);
  
  TString newName = "ratio_" + varName + "_" + selection.first + Form("_%f_%f", thr1, thr2) ;

  TH1F* hratio = (TH1F*)hOS->Clone(newName);
  hratio -> Divide (hSS);
  hratio -> SetMinimum(0.85);
  hratio -> SetMaximum(1.25);

  return hratio;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


pair<TH1F*, TH1F*> getPlotPair (plotContainer& pcOS, plotContainer& pcSS,
                 pair <TString, TCut>& selection,
                 string& varName,
                 string& sampleName,
                 float thr1,
                 float thr2
                )
{
  TString OS_selName = Form("OSaiso_%f_%f_", thr1, thr2);
  TString SS_selName = Form("SSaiso_%f_%f_", thr1, thr2);
  OS_selName = OS_selName + selection.first;
  SS_selName = SS_selName + selection.first;

  TH1F* hOS = pcOS.getHisto (varName, string(OS_selName.Data()), sampleName);
  TH1F* hSS = pcSS.getHisto (varName, string(SS_selName.Data()), sampleName);

  pair <TH1F*, TH1F*> res = make_pair (hOS, hSS);
  return res;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


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

  // read thresholds for non iso WPs
  std::vector<float> dau1iso_thrLow = gConfigParser->readFloatListOption ("computeQCDratio::dau1isothrLow");
  std::vector<float> dau2iso_thrLow = gConfigParser->readFloatListOption ("computeQCDratio::dau2isothrLow"); // iso > thrLow

  vector <vector<pair <TString, TCut> > > vec_allselections_SS;
  vector <vector<pair <TString, TCut> > > vec_allselections_OS;

  // create SS and OS selections
  for (unsigned int ithr = 0; ithr < dau1iso_thrLow.size(); ithr++)
  {
    // OS
    vector<pair <TString, TCut> > this_selections_OS = selections ;
    for (unsigned int i = 0 ; i < this_selections_OS.size () ; ++i)
    {
      this_selections_OS.at (i).first = TString (Form("OSaiso_%f_%f_", dau1iso_thrLow.at(ithr), dau2iso_thrLow.at(ithr))) + this_selections_OS.at (i).first ;
      TString cut = Form("dau1_iso > %f && dau2_iso > %f && isOS != 0", dau1iso_thrLow.at(ithr), dau2iso_thrLow.at(ithr));
      cout << "OS anti-iso selecton: " << cut << endl;
      this_selections_OS.at (i).second = this_selections_OS.at (i).second && TCut(cut) ;
    }
    vec_allselections_OS.push_back(this_selections_OS);

    // SS
    vector<pair <TString, TCut> > this_selections_SS = selections ;
    for (unsigned int i = 0 ; i < this_selections_SS.size () ; ++i)
    {
      this_selections_SS.at (i).first = TString (Form("SSaiso_%f_%f_", dau1iso_thrLow.at(ithr), dau2iso_thrLow.at(ithr))) + this_selections_SS.at (i).first ;
      TString cut = Form("dau1_iso > %f && dau2_iso > %f && isOS == 0", dau1iso_thrLow.at(ithr), dau2iso_thrLow.at(ithr));
      cout << "SS anti-iso selecton: " << cut << endl;
      this_selections_SS.at (i).second = this_selections_SS.at (i).second && TCut(cut) ;
    }
    vec_allselections_SS.push_back(this_selections_SS);


  }

  // now merge all selections for different iso thresholds into a single one to be faster
  vector<pair <TString, TCut> > selections_OS;
  for (unsigned int iv = 0; iv < vec_allselections_OS.size(); iv++)
  {
    for (unsigned int isel = 0; isel < vec_allselections_OS.at(iv).size(); isel++)
    {
      pair <TString, TCut> thisPair = vec_allselections_OS.at(iv).at(isel);
      selections_OS.push_back (thisPair);
    }
  }

  vector<pair <TString, TCut> > selections_SS;
  for (unsigned int iv = 0; iv < vec_allselections_SS.size(); iv++)
  {
    for (unsigned int isel = 0; isel < vec_allselections_SS.at(iv).size(); isel++)
    {
      pair <TString, TCut> thisPair = vec_allselections_SS.at(iv).at(isel);
      selections_SS.push_back (thisPair);
    }
  }

  // check that everything is fine:
  for (unsigned int isel = 0; isel < selections_OS.size(); isel++)
    cout << selections_OS.at(isel).first << endl;
  for (unsigned int isel = 0; isel < selections_SS.size(); isel++)
    cout << selections_SS.at(isel).first << endl;


  // get the variables to be plotted -- will make OS / SS plot ratio for each
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<string> variablesList = gConfigParser->readStringListOption ("general::variables") ;
  
  vector<string> Buf_variables2DList(0); 
  if (gConfigParser->isDefined ("general::2Dvariables")) Buf_variables2DList = gConfigParser->readStringListOption ("general::2Dvariables") ;
  vector<pair<string,string> > variables2DList(0);
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
  SS_DATA_plots.AddOverAndUnderFlow () ;

  cout << "--- MAIN reading bkg and filling SS histos" << endl ;

  // get the same-sign distributions from bkg
  plotContainer SS_bkg_plots ("SS_bkg", variablesList, variables2DList, selections_SS, bkgSamplesList, 0) ;
  counters SS_bkgCount = fillHistos (bkgSamples, SS_bkg_plots, 
              variablesList, variables2DList,
              selections_SS,
              lumi,
              vector<float> (0),
              false, false, maxEvtsMC) ;
  SS_bkg_plots.AddOverAndUnderFlow () ;

  cout << "--- MAIN preparing to loop on variables and selections to calc SS QCD" << endl ;

  // the index in the stack is based on variable ID (iv) and selection ID (isel):
  // iHisto = iv + nVars * isel
  vector<string> QCDsample ;
  QCDsample.push_back ("QCD") ;
  plotContainer SS_QCD ("SS_QCD", variablesList, variables2DList, selections_SS, QCDsample, 0) ;
  //  vector <TH1F *> SS_QCD ;
  for (unsigned int ivar = 0 ; ivar < variablesList.size () ; ++ivar)
    {
      for (unsigned int icut = 0 ; icut < selections_SS.size () ; ++icut)
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
        }
    }


  // calculate the QCD in the OS region as data - other_bkg
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  cout << "--- MAIN reading DATA and filling OS histos" << endl ;

  // get the same-sign distributions from data
  plotContainer OS_DATA_plots ("OS_DATA", variablesList, variables2DList, selections_OS, DATASamplesList, 2) ;
  counters OS_DATACount = fillHistos (DATASamples, OS_DATA_plots, 
              variablesList, variables2DList,
              selections_OS,
              lumi,
              vector<float> (0),
              true, false) ;
  OS_DATA_plots.AddOverAndUnderFlow () ;

  cout << "--- MAIN reading bkg and filling OS histos" << endl ;

  // get the same-sign distributions from bkg
  plotContainer OS_bkg_plots ("OS_bkg", variablesList, variables2DList, selections_OS, bkgSamplesList, 0) ;
  counters OS_bkgCount = fillHistos (bkgSamples, OS_bkg_plots, 
              variablesList, variables2DList,
              selections_OS,
              lumi,
              vector<float> (0),
              false, false, maxEvtsMC) ;
  OS_bkg_plots.AddOverAndUnderFlow () ;

  cout << "--- MAIN preparing to loop on variables and selections to calc OS QCD" << endl ;

  // the index in the stack is based on variable ID (iv) and selection ID (isel):
  // iHisto = iv + nVars * isel
  //vector<string> QCDsample ;
  //QCDsample.push_back ("QCD") ;
  plotContainer OS_QCD ("OS_QCD", variablesList, variables2DList, selections_OS, QCDsample, 0) ;
  //  vector <TH1F *> OS_QCD ;
  for (unsigned int ivar = 0 ; ivar < variablesList.size () ; ++ivar)
    {
      for (unsigned int icut = 0 ; icut < selections_OS.size () ; ++icut)
        {
          THStack * D_stack = OS_DATA_plots.makeStack (variablesList.at (ivar),
                                  selections_OS.at (icut).first.Data ()) ;
          TH1F * tempo = (TH1F *) D_stack->GetStack ()->Last () ;
          TString name = tempo->GetName () ;
          name = TString ("DDQCD_") + name ;
          TH1F * dummy = (TH1F *) tempo->Clone (name) ;

          THStack * b_stack = OS_bkg_plots.makeStack (variablesList.at (ivar),
                                  selections_OS.at (icut).first.Data ()) ;
          TH1F * h_bkg = (TH1F *) b_stack->GetStack ()->Last () ;
          dummy->Add (h_bkg, -1) ;
          OS_QCD.m_histos[variablesList.at (ivar)][selections_OS.at (icut).first.Data ()]["QCD"] = dummy ;
        }
    }


  // calculate the QCD in the OS region as data - other_bkg
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  cout << "--- MAIN calculating ratios OS / SS" << endl ;
  //gStyle->SetOptStat(0);

  TString outFolderNameBase = gConfigParser->readStringOption ("general::outputFolderName") ;
  outFolderNameBase += "/" ;
  system (TString ("mkdir -p ") + outFolderNameBase) ;
  outFolderNameBase += gConfigParser->readStringOption ("computeQCDratio::outputFolderName") ;
  outFolderNameBase += "/" ;  
  system (TString ("mkdir -p ") + outFolderNameBase) ;
  system (TString ("mkdir -p ") + outFolderNameBase + TString("ratioplots/")) ; 
  system (TString ("mkdir -p ") + outFolderNameBase + TString("num_denom_plots/")) ; 
  system (TString ("mkdir -p ") + outFolderNameBase + TString("num_denom_plots_log/")) ; 
  system (TString ("mkdir -p ") + outFolderNameBase + TString("events/")) ; 
  system (TString ("mkdir -p ") + outFolderNameBase + TString("events_log/")) ; 

  TFile* fOut = new TFile (outFolderNameBase + "ratioPlots.root", "RECREATE");
  fOut->cd();
  TCanvas* c1 = new TCanvas;

  gStyle->SetOptFit(1111);

  //for (unsigned int isample = 0; isample < samples.size(); isample++)
  for (unsigned int ivar = 0; ivar < variablesList.size(); ivar++)
  {
    for (unsigned int isel = 0; isel < selections.size(); isel++)
    {
      for (unsigned int ithr = 0; ithr < dau1iso_thrLow.size(); ithr++)
      {
          TH1F* thisRatio = makeRatio (OS_QCD, SS_QCD,
                 selections.at(isel),
                 variablesList.at(ivar),
                 QCDsample.at(0), // "QCD"
                 dau1iso_thrLow.at(ithr),
                 dau2iso_thrLow.at(ithr) );
     
          thisRatio -> SetMarkerStyle(8);
          thisRatio -> SetMarkerSize(1);
          thisRatio -> SetMarkerColor(kBlack);

          c1->SetLogy(false);
          thisRatio -> Draw();
          thisRatio -> Fit("pol1");
          TString newName = outFolderNameBase + "ratioplots/ratio_" + variablesList.at(ivar) + "_" + selections.at(isel).first + Form("_%f_%f.pdf", dau1iso_thrLow.at(ithr), dau2iso_thrLow.at(ithr)) ;
          c1->Print (newName, "pdf");

          thisRatio -> Write();

          // now draw together SS and OS for this selection and draw plot
          pair <TH1F*, TH1F*> res = getPlotPair (OS_QCD, SS_QCD,
                 selections.at(isel),
                 variablesList.at(ivar),
                 QCDsample.at(0), // "QCD"
                 dau1iso_thrLow.at(ithr),
                 dau2iso_thrLow.at(ithr) );

          TH1F* hOS = res.first;
          TH1F* hSS = res.second;
          hOS->SetStats(false);
          hSS->SetStats(false);
          hOS->SetLineColor (kRed);
          hSS->SetLineColor (kBlue);
          hOS->SetMarkerColor (kRed);
          hSS->SetMarkerColor (kBlue);
          hOS->SetMarkerStyle (4);
          hSS->SetMarkerStyle (5);

          if (hSS->GetMaximum() > hOS->GetMaximum()) hOS->SetMaximum(hSS->GetMaximum());
          hOS->SetMaximum(1.2*hOS->GetMaximum());
      
          TLegend leg (0.41, 0.79, 0.89, 0.89);
          leg.SetNColumns(2);
          leg.SetFillColor(kWhite);
          leg.SetLineWidth(0);
          leg.SetBorderSize(0);
          leg.AddEntry(hOS, "OS", "lp");
          leg.AddEntry(hSS, "SS", "lp");
          hOS->Draw();
          hSS->Draw("same");
          leg.Draw();

          newName = outFolderNameBase + "num_denom_plots/ratio_" + variablesList.at(ivar) + "_" + selections.at(isel).first + Form("_%f_%f.pdf", dau1iso_thrLow.at(ithr), dau2iso_thrLow.at(ithr)) ;
          c1->Print (newName, "pdf");

          // ------- log ----
          c1->SetLogy(true);
          hOS->Draw();
          hSS->Draw("same");
          //leg.Draw();

          newName = outFolderNameBase + "num_denom_plots_log/ratio_" + variablesList.at(ivar) + "_" + selections.at(isel).first + Form("_%f_%f_log.pdf", dau1iso_thrLow.at(ithr), dau2iso_thrLow.at(ithr)) ;
          c1->Print (newName, "pdf");
          c1->SetLogy(false);

      }
    }
  }


  // ------------------------------------------------------------------
  cout << "--- MAIN printing event plots" << endl ;

  for (unsigned int ivar = 0 ; ivar < variablesList.size () ; ++ivar)
  {
    for (unsigned int icut = 0 ; icut < selections_OS.size () ; ++icut)
    {
      THStack * D_stack = OS_DATA_plots.makeStack (variablesList.at (ivar),
                              selections_OS.at (icut).first.Data ()) ;
      //TH1F * tempo = (TH1F *) D_stack->GetStack ()->Last () ;
    
      THStack * b_stack = OS_bkg_plots.makeStack (variablesList.at (ivar),
                              selections_OS.at (icut).first.Data ()) ;
      //TH1F * h_bkg = (TH1F *) b_stack->GetStack ()->Last () ;

      D_stack->Draw(); // data surely above bkg as it is QCD dominated
      b_stack->Draw("hist same"); // data surely above bkg as it is QCD dominated

      TString newName = outFolderNameBase + "events/plot_" + variablesList.at(ivar) + "_" + selections_OS.at (icut).first + "_OS.pdf" ;
      c1->Print (newName, "pdf");

      c1->SetLogy(true);
      newName = outFolderNameBase + "events_log/plot_" + variablesList.at(ivar) + "_" + selections_OS.at (icut).first + "_log_OS.pdf" ;
      c1->Print (newName, "pdf");
      c1->SetLogy(false);

    }
  } 


  for (unsigned int ivar = 0 ; ivar < variablesList.size () ; ++ivar)
  {
    for (unsigned int icut = 0 ; icut < selections_SS.size () ; ++icut)
    {
      THStack * D_stack = SS_DATA_plots.makeStack (variablesList.at (ivar),
                              selections_SS.at (icut).first.Data ()) ;
      //TH1F * tempo = (TH1F *) D_stack->GetStack ()->Last () ;
    
      THStack * b_stack = SS_bkg_plots.makeStack (variablesList.at (ivar),
                              selections_SS.at (icut).first.Data ()) ;
      //TH1F * h_bkg = (TH1F *) b_stack->GetStack ()->Last () ;

      D_stack->Draw(); // data surely above bkg as it is QCD dominated
      b_stack->Draw("hist same"); // data surely above bkg as it is QCD dominated

      TString newName = outFolderNameBase + "events/plot_" + variablesList.at(ivar) + "_" + selections_SS.at (icut).first + "_SS.pdf" ;
      c1->Print (newName, "pdf");

      c1->SetLogy(true);
      newName = outFolderNameBase + "events_log/plot_" + variablesList.at(ivar) + "_" + selections_OS.at (icut).first + "_log_SS.pdf" ;
      c1->Print (newName, "pdf");
      c1->SetLogy(false);


    }
  }  
//   /* NB if it has to be subtracted, it cannot be scaled! */

//   // get the SS-to-OS scale factor and scale the QCD distributions
//   // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

//   float SStoOS_scaleFactor = 1.06 ; // to be calculated here at a certain moment!!
//   SS_QCD.scale (SStoOS_scaleFactor) ;

//   int QCDcolor = gConfigParser->readIntOption ("colors::QCD") ;
//   SS_QCD.setHistosProperties (0, QCDcolor) ; 

//   // insert the QCD in the OS region
//   // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

//   cout << "--- MAIN reading DATA and filling OS histos" << endl ;

//   // get the opposite-sign distributions from data
//   plotContainer OS_DATA_plots ("OS_DATA", variablesList, variables2DList, selections_OS, DATASamplesList, 2) ;
//   counters OS_DATACount = fillHistos (DATASamples, OS_DATA_plots, 
//               variablesList, variables2DList,
//               selections_OS,
//               lumi,
//               vector<float> (0),
//               true, false) ;
//   OS_DATA_plots.AddOverAndUnderFlow () ;

//   cout << "--- MAIN reading bkg and filling OS histos" << endl ;

//   // get the opposite-sign distributions from bkg
//   plotContainer OS_bkg_plots ("OS_bkg", variablesList, variables2DList, selections_OS, bkgSamplesList, 0) ;
//   counters OS_bkgCount = fillHistos (bkgSamples, OS_bkg_plots, 
//               variablesList, variables2DList,
//               selections_OS,
//               lumi,
//               vector<float> (0),
//               false, false, maxEvtsMC) ;
//   OS_bkg_plots.AddOverAndUnderFlow () ;
//   OS_bkg_plots.addSample ("QCD", SS_QCD) ;
  
//   cout << "--- MAIN reading sig and filling OS histos" << endl ;

//   // get the opposite-sign distributions from sig
//   plotContainer OS_sig_plots ("OS_sig", variablesList, variables2DList, selections_OS, sigSamplesList, 1) ;
//   counters OS_sigCount = fillHistos (sigSamples, OS_sig_plots, 
//               variablesList, variables2DList,
//               selections_OS,
//               lumi,
//               vector<float> (0),
//               false, true) ;
//   OS_sig_plots.AddOverAndUnderFlow () ;

//   // Save the histograms
//   // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

//   cout << "--- MAIN before saving" << endl ;

//   TString outFolderNameBase = gConfigParser->readStringOption ("general::outputFolderName") ;
//   outFolderNameBase += "/" ;
//   system (TString ("mkdir -p ") + outFolderNameBase) ;
//   outFolderNameBase += gConfigParser->readStringOption ("evalQCD::outputFolderName") ;
//   outFolderNameBase += "/" ;  
//   system (TString ("mkdir -p ") + outFolderNameBase) ;

//   TString outString ;
//   outString.Form (outFolderNameBase + "outPlotter.root") ;
//   TFile * fOut = new TFile (outString.Data (), "RECREATE") ;
//   SS_DATA_plots.save (fOut) ;
//   SS_bkg_plots.save (fOut) ;
//   OS_DATA_plots.save (fOut) ;
//   OS_bkg_plots.save (fOut) ;
//   OS_sig_plots.save (fOut) ;
//   SS_QCD.save (fOut); // this is the estimated QCD after scaling to OS region 1.06

//   fOut->Close () ;

//   // Plot the histograms
//   // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

//   system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events/")) ;
//   system (TString ("mkdir -p ") + outFolderNameBase + TString ("/shapes/")) ;

//   TCanvas * c = new TCanvas () ;

//   cout << "--- MAIN before plotting" << endl ;

//   // loop on selections
//   for (unsigned int isel = 0 ; isel < selections.size () ; ++isel)
//     {
//       // loop on variables
//       for (unsigned int iv = 0 ; iv < variablesList.size () ; ++iv)
//         {
//           c->cd () ;
//           TString outputName ; 
//           outputName.Form ("plot_%s_%s",
//             variablesList.at (iv).c_str (), selections.at (isel).first.Data ()) ;

//           TString outFolderName = outFolderNameBase + TString ("/events/") ;

//           // get the extremes for the plot
//           THStack * sig_stack = OS_sig_plots.makeStack ( variablesList.at (iv), 
//                                     selections_OS.at (isel).first.Data ()) ;
//           THStack * bkg_stack = OS_bkg_plots.makeStack ( variablesList.at (iv), 
//                                     selections_OS.at (isel).first.Data ()) ;
//           THStack * DATA_stack = OS_DATA_plots.makeStack ( variablesList.at (iv), 
//                                     selections_OS.at (isel).first.Data ()) ;

//           vector<float> extremes_bkg  = getExtremes (bkg_stack) ;
//           vector<float> extremes_sig  = getExtremes (sig_stack) ;
//           vector<float> extremes_DATA = getExtremes (DATA_stack) ;
//           TH1F * bkg = c->DrawFrame (
//               extremes_bkg.at (0) ,
//               0.9 * min3 (extremes_bkg.at (1), extremes_sig.at (1), 
//                           extremes_DATA.at (1)) ,
//               extremes_bkg.at (2) ,
//               1.3 * max3 (extremes_bkg.at (3), extremes_sig.at (3), 
//                           extremes_DATA.at (3) + sqrt (extremes_DATA.at (3)))
//             ) ;  
     
//           copyTitles (bkg, bkg_stack) ;

//           bkg->Draw () ;
//           bkg_stack->Draw ("hist same") ;
//           sig_stack->Draw ("nostack hist same") ;
//           TH1F * h_data = (TH1F *) DATA_stack->GetStack ()->Last () ;
//           // FIXME probably the data uncertainties need to be fixed
//           h_data->Draw ("same") ;
          
//           TString coutputName ;
//           coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
//           c->SaveAs (coutputName.Data ()) ;

//           c->SetLogy (1) ;
//           bkg->Draw () ;
//           bkg_stack->Draw ("hist same") ;
//           sig_stack->Draw ("nostack hist same") ;
//           h_data->Draw ("same") ;

//           coutputName.Form ("%s_log.pdf", (outFolderName + outputName).Data ()) ;
//           c->SaveAs (coutputName.Data ()) ;
//           c->SetLogy (0) ;
          
//           // plotting shapes
//           // ---- ---- ---- ---- ---- ---- ---- ---- ----

//           outFolderName = outFolderNameBase + TString ("/shapes/") ;
//           TString basename ;
//           basename.Form ("shape_%s_%s",
//                   variablesList.at (iv).c_str (),
//                   selections.at (isel).first.Data ()
//                 ) ;

//           THStack * hstack_bkg_norm = normaliseStack (bkg_stack) ;
//           TH1F * shape_bkg = (TH1F *) hstack_bkg_norm->GetStack ()->Last () ;
          
//           THStack * hstack_sig_norm = normaliseStack (sig_stack) ;
//           TH1F * shape_sig = (TH1F *) hstack_sig_norm->GetStack ()->Last () ;
          
//           if (shape_sig->GetMaximum () > shape_bkg->GetMaximum ()) 
//             hstack_sig_norm->Draw ("hist") ;
//           else   
//             hstack_bkg_norm->Draw ("hist") ;

//           hstack_bkg_norm->Draw ("hist same") ;
//           hstack_sig_norm->Draw ("hist same") ;
          
//           TString name = basename + "_norm" ;
//           coutputName.Form ("%s.pdf", (outFolderName + basename).Data ()) ;
//           c->SaveAs (coutputName.Data ()) ;
 
//           /*
//           // plotting 2D histos, one per sample as it is hard to superimpose them
//           // ---- ---- ---- ---- ---- ---- ---- ---- ----
//           outFolderName = outFolderNameBase + TString ("/plots2D/") ;
//           OS_sig_plots.         
//           */

//           delete hstack_bkg_norm;
//           delete hstack_sig_norm;
//           delete sig_stack;
//           delete bkg_stack;
//           delete DATA_stack;
          
     
//         } // loop on variables
//     } // loop on selections

//   delete c ;


//   // print yields
//   // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//   TString yieldsFileName = outFolderNameBase + "/yields.txt";
//   std::ofstream yieldsFile (yieldsFileName.Data());

//   unsigned int NSpacesColZero = 16;
//   unsigned int NSpacesColumns = 10;
  
//   cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
//   cout << " EXPECTED NUMBER OF SIG EVENTS\n\n" ;
//   yieldsFile << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
//   yieldsFile << " EXPECTED NUMBER OF SIG EVENTS\n\n" ;
  
//   printTableTitle (std::cout,  sigSamples) ;
//   printTableTitle (yieldsFile, sigSamples) ;
//   printTableBody  (std::cout,  selections, OS_sigCount, sigSamples) ;
//   printTableBody  (yieldsFile, selections, OS_sigCount, sigSamples) ;

//   vector<string> DataDriven_names (1, string("QCD"));
//   vector <vector<float>> DataDriven_yields;
//   vector<float> QCD_yields;
//   // QCD -- prepare yields
//   for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
//   {
//         int nQCD = SS_QCD.getHisto (variablesList.at(0), string(selections_SS.at(iSel).first.Data()), QCDsample.at(0)) -> Integral();
//         QCD_yields.push_back (nQCD);
//   }
//   DataDriven_yields.push_back (QCD_yields);

//   cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
//   cout << " EXPECTED NUMBER OF BKG EVENTS\n\n" ;
//   yieldsFile << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
//   yieldsFile << " EXPECTED NUMBER OF BKG EVENTS\n\n" ;
//   printTableTitle (std::cout,  bkgSamples, DataDriven_names) ;
//   printTableTitle (yieldsFile, bkgSamples, DataDriven_names) ;
//   printTableBody  (std::cout,  selections, OS_bkgCount, bkgSamples, DataDriven_yields) ;
//   printTableBody  (yieldsFile, selections, OS_bkgCount, bkgSamples, DataDriven_yields) ;

//   // mini debug to cjeck that each var has the same QCD yield
//   /*  
//   // QCD
//   for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
//   {
//       for (int ivar = 0 ; ivar < variablesList.size(); ivar++)
//       {
//         int nQCD = SS_QCD.getHisto (variablesList.at(ivar), string(selections_SS.at(iSel).first.Data()), QCDsample.at(0)) -> Integral();
//         cout << nQCD << " ";
//       }
//       cout <<  "  --- " << endl;
//   }
//   */

//   cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
//   cout << " OBSERVED NUMBER OF EVENTS\n\n" ;
//   yieldsFile << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
//   yieldsFile << " OBSERVED NUMBER OF EVENTS\n\n" ;
//   printTableTitle (std::cout,  DATASamples) ;
//   printTableTitle (yieldsFile, DATASamples) ;
//   printTableBody  (std::cout,  selections, OS_DATACount, DATASamples) ;
//   printTableBody  (yieldsFile, selections, OS_DATACount, DATASamples) ;

//   cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
//   cout << " TOTALS\n\n" ;
//   yieldsFile << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
//   yieldsFile << " TOTALS\n\n" ;
//   vector<float> DATAtotal = OS_DATACount.getTotalCountsPerCut () ;
//   vector<float> bkgtotal = OS_bkgCount.getTotalCountsPerCut () ;
//   vector<string> titles ; titles.push_back ("DATA") ; titles.push_back ("bkg") ;
//   printTableTitle (std::cout, titles) ;
//   printTableTitle (yieldsFile, titles) ;
  
//   for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
//     {
//       cout << selections.at (iSel).first ;
//       yieldsFile << selections.at (iSel).first ;
//       unsigned int nspacetojump = (NSpacesColZero > string(selections.at (iSel).first.Data ()).size () ? NSpacesColZero - string(selections.at (iSel).first.Data ()).size () : 0);
//       for (unsigned int i = 0 ; i < nspacetojump; ++i){
//         cout << " " ;
//         yieldsFile << " " ;
//       } 
//       cout << "|" ;
//       yieldsFile << "|" ;
      
//       float evtnum = DATAtotal.at (iSel+1) ;
//       int subtractspace = 0 ;
//       if (evtnum > 0) subtractspace = int (log10 (evtnum)) ;
//       for (unsigned int i = 0 ; i < NSpacesColumns - subtractspace ; ++i)
//       {
//         cout << " " ;
//         yieldsFile << " " ;
//       }
//       cout << setprecision (0) << fixed << evtnum
//            << " |" ;
//       yieldsFile << setprecision (0) << fixed << evtnum
//            << " |" ;

//       evtnum = bkgtotal.at (iSel+1);
//       for (unsigned int iDD = 0; iDD < DataDriven_yields.size(); iDD++) evtnum += DataDriven_yields.at(iDD).at(iSel);
//       subtractspace = 0 ;
//       if (evtnum > 0) subtractspace = int (log10 (evtnum)) ;
//       for (unsigned int i = 0 ; i < NSpacesColumns - subtractspace ; ++i)
//       {
//           cout << " " ;
//           yieldsFile << " " ;
//       }
//       cout << setprecision (0) << fixed << evtnum
//            << " |\n" ;
//       yieldsFile << setprecision (0) << fixed << evtnum
//            << " |\n" ;
//     }
// /*
//   cout << " ============ MINI DEBUG ON QCD YIELDS ============ " << endl;
//   for (unsigned int isel = 0 ; isel < selections.size () ; ++isel)
//     {
//       // loop on variables
//       for (unsigned int iv = 0 ; iv < variablesList.size () ; ++iv) 

//     }
// */

//   // save on root file for debug
//   //TFile* fRootOut = new TFile (outFolderNameBase + "/" + "SS_QCD_histos.root", "recreate");
//   //fRootOut->Close();

  return 0 ;
}  