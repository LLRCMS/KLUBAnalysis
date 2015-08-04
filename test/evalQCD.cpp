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

  // OS histos, iso
  vector<pair <TString, TCut> > selections_OS = selections ;
  for (unsigned int i = 0 ; i < selections_OS.size () ; ++i)
    selections_OS.at (i).second = selections_OS.at (i).second && TCut ("isOS != 0") ;
  
  vector<pair <TString, TCut> > selections_SS = selections ;
  for (unsigned int i = 0 ; i < selections_SS.size () ; ++i)
    selections_SS.at (i).second = selections_SS.at (i).second && TCut ("isOS == 0") ;
  
  // get the variables to be plotted
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<string> variablesList = gConfigParser->readStringListOption ("general::variables") ;

  TString histoName ;
  HistoManager * manager = new HistoManager ("SS") ;

  // get the same-sign distributions from data
  counters SS_DATACount = fillHistos (DATASamples, manager, 
              variablesList,
              selections_SS,
              lumi,
              vector<float> (0),
              true, false) ;
  vector<THStack *> hstack_DATA = stackHistos (
      DATASamples, manager, 
      variablesList,
      selections, "DATA") ;

  // get the same-sign distributions from MC
  vector<THStack *> hstack_bkg = stackHistos (
      bkgSamples, manager, 
      variablesList,
      selections_SS, "bkg") ;


/* should we subtract signals as well?
  counters SS_sigCount = fillHistos (sigSamples, manager, 
              variablesList,
              selections_SS,
              lumi,
              vector<float> (0),
              false, true) ;
*/
  

  /* 
   - get QCD shape from SS, removing the other backgrounds
   - get SS/OS scale factor (can put to a number by now)
   - add QCD shape to final plots
  */
  
  
  
  // SS histos, iso
  // OS histos, not iso
  // SS histos, not iso




/*
 from one skim
 - SS 
 - OS

 from another skim
 - SS not iso
 - OS not iso
*/






  return 0 ;
}  