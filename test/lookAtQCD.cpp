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

#include "HistoManager.h"
#include "ConfigParser.h"
#include "utils.h"
#include "histoUtils.h"
#include "plotContainer.h"
#include "analysisUtils.h"


using namespace std ;


struct QCDevalTools
{
  QCDevalTools (const string & name) :
    m_name (name),
    m_DATA_plots (m_name + "_DATA"),
    m_bkg_plots (m_name + "_bkg"),
    m_QCD_plots (m_name + "_QCD")
    {}
  string m_name ;
  plotContainer m_DATA_plots ;
  plotContainer m_bkg_plots ;
  plotContainer m_QCD_plots ;
  vector<pair <TString, TCut> > m_selections ;
} ;


/*
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
THStack* getDataMCStack (QCDevalTools & QCDET, string variable, string selection)
{
  THStack* h_Stack = new THStack(Form("stack_%s_%s" , variable.c_str, selection.c_str), Form("stack_%s_%s" , variable.c_str, selection.c_str));
  for (int iSample = 0; iSample < QCDET.m_bkg_plots[variable][selection].second.size(); iSample++)
  {
    h_Stack -> Add (QCDET.m_bkg_plots[variable][selection].second.at(iSample));
  }
}
*/

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// calculate the QCD in the SS region as data - other_bkg
void getQCD (QCDevalTools & QCDET, vector<string> variablesList)
{
  for (unsigned int ivar = 0 ; ivar < variablesList.size () ; ++ivar)
    {
      for (unsigned int icut = 0 ; icut < QCDET.m_selections.size () ; ++icut)
        {
          THStack * D_stack = QCDET.m_DATA_plots.makeStack (variablesList.at (ivar),
                                  QCDET.m_selections.at (icut).first.Data ()) ;
          TH1F * tempo = (TH1F *) D_stack->GetStack ()->Last () ;
          TString name = tempo->GetName () ;
          name = TString ("DDQCD_") + name ;
          TH1F * dummy = (TH1F *) tempo->Clone (name) ;

          THStack * b_stack = QCDET.m_bkg_plots.makeStack (variablesList.at (ivar),
                                  QCDET.m_selections.at (icut).first.Data ()) ;
          TH1F * h_bkg = (TH1F *) b_stack->GetStack ()->Last () ;
          dummy->Add (h_bkg, -1) ;
          QCDET.m_QCD_plots.m_histos[variablesList.at (ivar)][QCDET.m_selections.at (icut).first.Data ()]["QCD"] = dummy ;
        }
    }
  return ;
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

  // get the samples to be analised
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

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

  // get the selections to be applied
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<string> activeSelections = gConfigParser->readStringListOption ("lookAtQCD::list") ;
  vector<pair <TString, TCut> > selections = readCutsFile (
      activeSelections,
      gConfigParser->readStringOption ("selections::selectionsFile")
    ) ;

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << "selections sequence: \n" ;
  for (unsigned int i = 0 ; i < selections.size () ; ++i)
    cout << selections.at (i).first << " : " << selections.at (i).second << endl ;

  // SS selections
  vector<pair <TString, TCut> > selections_SS = addSelection (
      selections, "isOS == 0", "SS_") ;

  // get the variables to be plotted
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<string> variablesList = gConfigParser->readStringListOption ("general::variables") ;

  // prepare selections and plot containers
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  bool useRelIso = gConfigParser->readBoolOption ("lookAtQCD::useRelIso");
  vector<pair<string, string> > isoThresholds ; // <dau1iso , dau2iso> threshold values
  if (!useRelIso) 
  {
    std::vector<string> sel1 = gConfigParser->readStringListOption ("lookAtQCD::l1AbsIso");
    std::vector<string> sel2 = gConfigParser->readStringListOption ("lookAtQCD::l2AbsIso");
    for (uint i = 0; i < sel1.size(); i++) isoThresholds.push_back (pair<string, string> (sel1.at(i), sel2.at(i)) );
  }
  else
  {
    std::vector<string> sel1 = gConfigParser->readStringListOption ("lookAtQCD::l1RelIso");
    std::vector<string> sel2 = gConfigParser->readStringListOption ("lookAtQCD::l2RelIso");
    for (uint i = 0; i < sel1.size(); i++) isoThresholds.push_back (pair<string, string> (sel1.at(i), sel2.at(i)) );
  }

  cout << "Iso thresholds tested:   ";
  for (uint i = 0; i < isoThresholds.size(); i++) cout << " (" << isoThresholds.at(i).first << " , " << isoThresholds.at(i).second << " )   "; 
  cout << endl;

  vector<pair <string, QCDevalTools> > QCDEvalAttempts ;

  vector<string> QCDsample ;
  QCDsample.push_back ("QCD") ;
  // loop on working points for the isolation
  for (unsigned int iWP = 0 ; iWP < isoThresholds.size () ; ++iWP)
    {
      string name = "ATT_" + isoThresholds.at (iWP).first
                    + "_" + isoThresholds.at (iWP).second ;
      QCDEvalAttempts.push_back (pair <string, QCDevalTools> (name, QCDevalTools (name))) ;

      string isolation = "";
      if (!useRelIso) {
        isolation = "dau1_iso < " + isoThresholds.at (iWP).first
                         + " && dau2_iso < " + isoThresholds.at (iWP).second ;
      }
      else {
        isolation = "(dau1_iso/dau1_pt) < " + isoThresholds.at (iWP).first
                 + " && (dau2_iso/dau2_pt) < " + isoThresholds.at (iWP).second ;
      }

      QCDEvalAttempts.back ().second.m_selections = addSelection (
          selections, isolation, "iso_") ;

      // prepare and fill data plots
      QCDEvalAttempts.back ().second.m_DATA_plots.init (
          variablesList, QCDEvalAttempts.back ().second.m_selections, DATASamplesList, 2) ;          
      fillHistos (DATASamples, QCDEvalAttempts.back ().second.m_DATA_plots, 
              variablesList,
              QCDEvalAttempts.back ().second.m_selections,
              lumi,
              vector<float> (0),
              true, false) ;
      QCDEvalAttempts.back ().second.m_DATA_plots.AddOverAndUnderFlow () ;

      // prepare and fill bkg plots
      QCDEvalAttempts.back ().second.m_bkg_plots.init (
          variablesList, QCDEvalAttempts.back ().second.m_selections, bkgSamplesList, 0) ;
      fillHistos (bkgSamples, QCDEvalAttempts.back ().second.m_bkg_plots, 
              variablesList,
              QCDEvalAttempts.back ().second.m_selections,
              lumi,
              vector<float> (0),
              false, false) ;
      QCDEvalAttempts.back ().second.m_bkg_plots.AddOverAndUnderFlow () ;
                    
      QCDEvalAttempts.back ().second.m_QCD_plots.init (
          variablesList, QCDEvalAttempts.back ().second.m_selections, QCDsample, 0) ;
      getQCD (QCDEvalAttempts.back ().second, variablesList) ;

      // now create some histos to be saved
          
    } // loop on working points for the isolation

  // Save the histograms
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << "Saving output plots: \n" ;
  TString outFolderNameBase = gConfigParser->readStringOption ("general::outputFolderName") ;
  outFolderNameBase += "/" ;
  system (TString ("mkdir -p ") + outFolderNameBase) ;
  outFolderNameBase += gConfigParser->readStringOption ("lookAtQCD::outputFolderName") ;
  outFolderNameBase += "/" ;  
  system (TString ("mkdir -p ") + outFolderNameBase) ;

  for (unsigned int iAtt = 0; iAtt < QCDEvalAttempts.size(); iAtt++)
  {
    TString outString ;
    outString.Form (outFolderNameBase + QCDEvalAttempts.at(iAtt).first.c_str() + "_outPlotter.root") ;
    TFile * fOut = new TFile (outString.Data (), "RECREATE") ;
    //QCDEvalAttempts.at(iAtt).second.m_DATA_plots.save (fOut);
    //QCDEvalAttempts.at(iAtt).second.m_bkg_plots.save (fOut);
    QCDEvalAttempts.at(iAtt).second.m_QCD_plots.save (fOut);
    fOut->Close () ;
    delete fOut;
  }


  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << "Making comparison plots: \n" ;

  //system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events/")) ;
  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/shapes/")) ;
  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/rootfiles/")) ;

  TCanvas * c = new TCanvas () ;
  int nVars = variablesList.size () ;
  // loop on selections
  for (unsigned int isel = 0 ; isel < selections.size () ; ++isel)
    {
      // loop on variables
      for (int iv = 0 ; iv < nVars ; ++iv)
        {
          c->cd () ;
          TString outputName ; 
          outputName.Form ("plot_%s_%s",
            variablesList.at (iv).c_str (), selections.at (isel).first.Data ()) ;

          std::vector<TH1F*> QCDhistos;

          TString outFolderName = outFolderNameBase + TString ("/shapes/") ;
          TString basename ;
          basename.Form ("shape_%s_%s",
                  variablesList.at (iv).c_str (),
                  selections.at (isel).first.Data ()
                ) ;

          for (unsigned int iWP = 0; iWP < QCDEvalAttempts.size(); iWP++)
          {
            // bisogna fare tutto questo casino per recuperare un istogramma con questo codice di merda
            TString histoTag = QCDEvalAttempts.at(iWP).second.m_selections.at(isel).first;
            THStack * QCD_stack = QCDEvalAttempts.at(iWP).second.m_QCD_plots.makeStack ( variablesList.at (iv), 
                                    histoTag.Data ()) ;
            THStack * hstack_QCD_norm = normaliseStack (QCD_stack) ;
            TH1F * shape_QCD = (TH1F *) hstack_QCD_norm->GetStack ()->Last () ;
            //hstack_QCD_norm->Draw ("hist") ;
            QCDhistos.push_back(shape_QCD);
          }

          // draw QCD histos
          vector<float> extr = getExtremes(QCDhistos);
          TH1F * bkg = c->DrawFrame (
              extr.at (0) ,
              0.9 * extr.at(1) ,
              extr.at (2) ,
              1.3 * extr.at(3)
          ) ;  

          TLegend* legend = new TLegend(0.11,0.75,0.85,0.89);
          legend->SetHeader ("l1, l2 iso upper thr. [GeV]");
          legend->SetBorderSize(0);
          legend->SetFillColor(0);
          legend->SetFillStyle(0);
          legend->SetTextSize(0.04);
          legend->SetTextFont(42);
          legend->SetNColumns (3);

          for (unsigned int iWP = 0; iWP < QCDhistos.size(); iWP++)
          {
            TString WPname = Form ("%s , %s" , isoThresholds.at(iWP).first.c_str(), isoThresholds.at(iWP).second.c_str() );
            legend->AddEntry (QCDhistos.at(iWP), WPname, "l");
            QCDhistos.at(iWP)->SetLineColor(50+5*iWP);
            QCDhistos.at(iWP)->SetLineWidth(2);
            QCDhistos.at(iWP)->SetLineStyle(iWP);
            QCDhistos.at(iWP)->Draw ("same hist E1");
          }
          legend->Draw();
          TString name = basename + "_norm" ;
          TString coutputName = "";
          coutputName.Form ("%s.pdf", (outFolderName + basename).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;
          
          // now save all histo into a file too for further drawing
          outFolderName = outFolderNameBase + TString ("/rootfiles/") ;
          basename.Form ("shape_%s_%s.root",
                  variablesList.at (iv).c_str (),
                  selections.at (isel).first.Data ()
                ) ;
          TFile* fOut = new TFile (outFolderName + basename, "RECREATE");
          c->Write();
          for (uint h = 0; h < QCDhistos.size(); h++) QCDhistos.at(h)->Write();
          fOut->Close();
          delete fOut;

          delete bkg;
          delete legend;
        } // loop on variables
    } // loop on selections

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << "Making DATA / MC stacked plots: \n" ;
  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events/")) ;

  for (unsigned int isel = 0 ; isel < selections.size () ; ++isel)
  {
    // loop on variables
    for (int iv = 0 ; iv < nVars ; ++iv)
    {
      c->cd () ;

      TString outFolderName = outFolderNameBase + TString ("/events/") ;

      // each one produces a stack data / MC
      for (unsigned int iWP = 0; iWP < QCDEvalAttempts.size(); iWP++)
      {
        // bisogna fare tutto questo casino per recuperare un istogramma con questo codice di merda
        TString histoTag = QCDEvalAttempts.at(iWP).second.m_selections.at(isel).first;
        THStack * data_stack = QCDEvalAttempts.at(iWP).second.m_DATA_plots.makeStack ( variablesList.at (iv), 
                                histoTag.Data ()) ;
        THStack * bkg_stack = QCDEvalAttempts.at(iWP).second.m_bkg_plots.makeStack ( variablesList.at (iv), 
                                histoTag.Data ()) ;
        vector<float> extr = getExtremes (data_stack);
        TH1F * bkg = c->DrawFrame (
          extr.at (0) ,
          0.9 * extr.at(1) ,
          extr.at (2) ,
          1.3 * extr.at(3)
        ) ;  

        bkg_stack -> Draw ("hist same");
        data_stack->GetStack()->Last()->Draw("P same");
        TString basename = "";
        basename.Form ("stack_%s_%s_WP_%s_%s",
                variablesList.at (iv).c_str (),
                selections.at (isel).first.Data (),
                isoThresholds.at(iWP).first.c_str(), isoThresholds.at(iWP).second.c_str()
              ) ;
        TString coutputName = "";
        coutputName.Form ("%s.pdf", (outFolderName + basename).Data ()) ;
        c->SaveAs (coutputName.Data());

        delete bkg;
      }
    }
  }
  delete c ;
  
  return 0 ;
}  