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
  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;

  // OS selections
  vector<pair <TString, TCut> > selections_OS = selections ;
  for (unsigned int i = 0 ; i < selections_OS.size () ; ++i)
    {
      selections_OS.at (i).first = TString ("OS_") + selections_OS.at (i).first ;
      selections_OS.at (i).second = selections_OS.at (i).second && TCut ("isOS != 0") ;
    }
     
  // SS selections
  vector<pair <TString, TCut> > selections_SS = selections ;
  for (unsigned int i = 0 ; i < selections_SS.size () ; ++i)
    {
      selections_OS.at (i).first = TString ("SS_") + selections_OS.at (i).first ;
      selections_OS.at (i).second = selections_OS.at (i).second && TCut ("isOS == 0") ;
    }
  
  // get the variables to be plotted
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<string> variablesList = gConfigParser->readStringListOption ("general::variables") ;

  // calculate the QCD in the SS region as data - other_bkg
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  // get the same-sign distributions from data
  plotContainer SS_DATA_plots ("SS_DATA_", variablesList, selections_SS, DATASamplesList, 2) ;
  counters SS_DATACount = fillHistos (DATASamples, SS_DATA_plots, 
              variablesList,
              selections_SS,
              lumi,
              vector<float> (0),
              true, false) ;
  SS_DATA_plots.AddOverAndUnderFlow () ;

  // get the same-sign distributions from bkg
  plotContainer SS_bkg_plots ("SS_bkg_", variablesList, selections_SS, bkgSamplesList, 0) ;
  counters SS_bkgCount = fillHistos (bkgSamples, SS_bkg_plots, 
              variablesList,
              selections_SS,
              lumi,
              vector<float> (0),
              false, true) ;
  SS_bkg_plots.AddOverAndUnderFlow () ;

  // the index in the stack is based on variable ID (iv) and selection ID (isel):
  // iHisto = iv + nVars * isel
  vector <TH1F *> SS_QCD ;
  for (unsigned int ivar = 0 ; ivar < variablesList.size () ; ++ivar)
    {
      for (unsigned int icut = 0 ; icut < selections_SS.size () ; ++icut)
        {
          THStack * D_stack = SS_DATA_plots.makeStack (variablesList.at (ivar),
                                  selections_SS.at (icut).first.Data ()) ;
          
          TH1F * tempo = (TH1F *) D_stack->GetStack ()->Last () ;
          TString name = tempo->GetName () ;
          name = TString ("DDQCD_") + name ;
          SS_QCD.push_back ((TH1F *) tempo->Clone (name)) ;
          THStack * b_stack = SS_bkg_plots.makeStack (variablesList.at (ivar),
                                  selections_SS.at (icut).first.Data ()) ;
          TH1F * h_bkg = (TH1F *) b_stack->GetStack ()->Last () ;
          SS_QCD.back ()->Add (h_bkg, -1) ;
        }
    }

  /* FIXME should we subtract signals as well? */
  /* NB if it has to be subtracted, it cannot be scaled! */

  // get the SS-to-OS scale factor and scale the QCD distributions
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  float SStoOS_scaleFactor = 1.06 ; // to be calculated here at a certain moment!!
  int QCDcolor = gConfigParser->readIntOption ("colors::QCD") ;
  
  for (unsigned int iHisto = 0 ; iHisto < SS_QCD.size () ; ++iHisto)  
    {
      SS_QCD.at (iHisto)->Scale (SStoOS_scaleFactor) ; 
      SS_QCD.at (iHisto)->SetFillColor (QCDcolor) ;
    }

  // insert the QCD in the OS region
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  // get the opposite-sign distributions from data
  plotContainer OS_DATA_plots ("OS_DATA_", variablesList, selections_OS, DATASamplesList, 0) ;
  counters OS_DATACount = fillHistos (DATASamples, OS_DATA_plots, 
              variablesList,
              selections_OS,
              lumi,
              vector<float> (0),
              true, false) ;
  OS_DATA_plots.AddOverAndUnderFlow () ;

  // get the opposite-sign distributions from bkg
  plotContainer OS_bkg_plots ("OS_bkg_", variablesList, selections_OS, bkgSamplesList, 0) ;
  counters OS_bkgCount = fillHistos (bkgSamples, OS_bkg_plots, 
              variablesList,
              selections_OS,
              lumi,
              vector<float> (0),
              false, true) ;
  OS_bkg_plots.AddOverAndUnderFlow () ;
  OS_bkg_plots.addSample ("QCD", SS_QCD) ;
  
  // get the opposite-sign distributions from sig
  plotContainer OS_sig_plots ("OS_sig_", variablesList, selections_OS, sigSamplesList, 0) ;
  counters OS_sigCount = fillHistos (sigSamples, OS_sig_plots, 
              variablesList,
              selections_OS,
              lumi,
              vector<float> (0),
              false, true) ;
  OS_sig_plots.AddOverAndUnderFlow () ;

  // FIXME plotting to be copied from plotNEW, now.

  return 0 ;
}  