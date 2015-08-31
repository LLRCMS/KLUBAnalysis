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

  vector<string> activeSelections = gConfigParser->readStringListOption ("selections::list") ;
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

  vector<pair<string, string> > isoThresholds ; // <dau1iso , dau2iso> threshold values
  isoThresholds.push_back (pair<string,string> ("1.0", "1.0")) ; // GeV", "GeV
  isoThresholds.push_back (pair<string,string> ("2.0", "2.0")) ; // GeV", "GeV
  isoThresholds.push_back (pair<string,string> ("2.5", "2.5")) ; // GeV", "GeV
  isoThresholds.push_back (pair<string,string> ("3.0", "3.0")) ; // GeV", "GeV
  isoThresholds.push_back (pair<string,string> ("4.0", "4.0")) ; // GeV", "GeV
  isoThresholds.push_back (pair<string,string> ("5.0", "5.0")) ; // GeV", "GeV

  vector<pair <string, QCDevalTools> > QCDEvalAttempts ;

  vector<string> QCDsample ;
  QCDsample.push_back ("QCD") ;
  // loop on working points for the isolation
  for (unsigned int iWP = 0 ; iWP < isoThresholds.size () ; ++iWP)
    {
      string name = "ATT_" + isoThresholds.at (iWP).first
                    + "_" + isoThresholds.at (iWP).second ;
      QCDEvalAttempts.push_back (pair <string, QCDevalTools> (name, QCDevalTools (name))) ;

      string isolation = "dau1_iso < " + isoThresholds.at (iWP).first
                         + " && dau2_iso < " + isoThresholds.at (iWP).second ;

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
  
  return 0 ;
}  