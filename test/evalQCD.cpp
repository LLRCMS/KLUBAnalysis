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

  plotContainer SS_DATA_plots ("SS_DATA", variablesList, selections_SS, DATASamplesList, 2) ;
// 0 = bkg
// 1 = sig
// 2 = data

  TString histoName ;
  HistoManager * manager = new HistoManager ("SS") ;

  // get the same-sign distributions from data
  counters SS_DATACount = fillHistos (DATASamples, manager, 
              variablesList,
              selections_SS,
              lumi,
              vector<float> (0),
              true, false) ;

  vector<THStack *> SS_hstack_DATA = stackHistos (
      DATASamples, manager, 
      variablesList,
      selections_SS, "DATA") ;
      
  // get the same-sign distributions from bkg
  counters SS_bkgCount = fillHistos (bkgSamples, manager, 
              variablesList,
              selections,
              lumi,
              vector<float> (0),
              false, false) ;

  vector<THStack *> SS_hstack_bkg = stackHistos (
      bkgSamples, manager, 
      variablesList,
      selections_SS, "SSbkg") ;


  // the index in the stack is based on variable ID (iv) and selection ID (isel):
  // iHisto = iv + nVars * isel
  vector <TH1F *> SS_QCD (SS_hstack_DATA.size ()) ;
  for (unsigned int iHisto = 0 ; iHisto < SS_hstack_DATA.size () ; ++iHisto)    
    {
      TH1F * tempo = (TH1F *) SS_hstack_DATA.at (iHisto)->GetStack ()->Last () ;
      TString name = tempo->GetName () ;
      name = TString ("DDQCD_") + name ;
      SS_QCD.at (iHisto) =  (TH1F *) tempo->Clone (name) ;
      TH1F * h_bkg = (TH1F *) SS_hstack_bkg.at (iHisto)->GetStack ()->Last () ;
      SS_QCD.at (iHisto)->Add (h_bkg, -1) ;
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


// fino a qui
// -------------
// -------------
// -------------
// -------------


  // insert the QCD in the OS region
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  // get the same-sign distributions from data
  counters OS_DATACount = fillHistos (DATASamples, manager, 
              variablesList,
              selections_OS,
              lumi,
              vector<float> (0),
              true, false) ;

  vector<THStack *> OS_hstack_DATA = stackHistos (
      DATASamples, manager, 
      variablesList,
      selections_OS, "DATA") ;
      
  // get the same-sign distributions from bkg
  counters OS_bkgCount = fillHistos (bkgSamples, manager, 
              variablesList,
              selections_OS,
              lumi,
              vector<float> (0),
              false, false) ;

  vector<THStack *> OS_hstack_bkg = stackHistos (
      bkgSamples, manager, 
      variablesList,
      selections_OS, "SSbkg") ;











  counters OS_sigCount = fillHistos (sigSamples, manager, 
              variablesList,
              selections_OS,
              lumi,
              signalScales,
              false, true) ;


  // get the same-sign distributions from MC

  vector<THStack *> OS_hstack_sig = stackHistos (
      sigSamples, manager, 
      variablesList,
      selections_OS, "sig") ;




  // add the QCD to the collection of plots for the bkg
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  for (unsigned int iHisto = 0 ; iHisto < SS_QCD.size () ; ++iHisto)  
    {
      OS_hstack_bkg.at (iHisto)->Add (SS_QCD.at (iHisto)) ;     
    }

  // plot the resulting distributions
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    
  TString outFolderNameBase = gConfigParser->readStringOption ("general::outputFolderName") ;
  
  system (TString ("mkdir -p ") + outFolderNameBase) ;
  TString outString ;
  outString.Form (outFolderNameBase + "outPlotter.root") ;
  TFile * fOut = new TFile (outString.Data (), "RECREATE") ;
  manager->SaveAllToFile (fOut) ;

  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events/")) ;
  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/shapes/")) ;

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

          TString outFolderName = outFolderNameBase + TString ("/events/") ;

          // get the extremes for the plot
          vector<float> extremes_bkg  = getExtremes (OS_hstack_bkg.at  (iv+nVars*isel)) ;
          vector<float> extremes_sig  = getExtremes (OS_hstack_sig.at  (iv+nVars*isel)) ;
          vector<float> extremes_DATA = getExtremes (OS_hstack_DATA.at (iv+nVars*isel)) ;
          TH1F * bkg = c->DrawFrame (
              extremes_bkg.at (0) ,
              0.9 * min3 (extremes_bkg.at (1), extremes_sig.at (1), 
                          extremes_DATA.at (1)) ,
              extremes_bkg.at (2) ,
              1.3 * max3 (extremes_bkg.at (3), extremes_sig.at (3), 
                          extremes_DATA.at (3) + sqrt (extremes_DATA.at (3)))
            ) ;  
          copyTitles (bkg, OS_hstack_bkg.at (iv+nVars*isel)) ;

          bkg->Draw () ;
          OS_hstack_bkg.at (iv+nVars*isel)->Draw ("hist same") ;
          OS_hstack_sig.at (iv+nVars*isel)->Draw ("nostack hist same") ;
          TH1F * h_data = (TH1F *) OS_hstack_DATA.at (iv+nVars*isel)->GetStack ()->Last () ;
          // FIXME probably the data uncertainties need to be fixed
          h_data->Draw ("same") ;
          
          TString coutputName ;
          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;
          
          c->SetLogy (1) ;
          bkg->Draw () ;
          OS_hstack_bkg.at (iv+nVars*isel)->Draw ("hist same") ;
          OS_hstack_sig.at (iv+nVars*isel)->Draw ("nostack hist same") ;
          h_data->Draw ("same") ;

          coutputName.Form ("%s_log.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;
          c->SetLogy (0) ;
          
          // plotting shapes
          // ---- ---- ---- ---- ---- ---- ---- ---- ----

          outFolderName = outFolderNameBase + TString ("/shapes/") ;
          TString basename ;
          basename.Form ("shape_%s_%s",
                  variablesList.at (iv).c_str (),
                  selections.at (isel).first.Data ()
                ) ;

          THStack * hstack_bkg_norm = normaliseStack (OS_hstack_bkg.at (iv+nVars*isel)) ;
          TH1F * shape_bkg = (TH1F *) hstack_bkg_norm->GetStack ()->Last () ;
          
          THStack * hstack_sig_norm = normaliseStack (OS_hstack_sig.at (iv+nVars*isel)) ;
          TH1F * shape_sig = (TH1F *) hstack_sig_norm->GetStack ()->Last () ;
          
          if (shape_sig->GetMaximum () > shape_bkg->GetMaximum ()) 
            hstack_sig_norm->Draw ("hist") ;
          else   
            hstack_bkg_norm->Draw ("hist") ;

          hstack_bkg_norm->Draw ("hist same") ;
          hstack_sig_norm->Draw ("hist same") ;
          
          TString name = basename + "_norm" ;
          coutputName.Form ("%s.pdf", (outFolderName + basename).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;

        } // loop on variables
    } // loop on selections

  delete c ;
  

  return 0 ;
}  