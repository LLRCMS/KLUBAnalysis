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
      selections_SS.at (i).first = TString ("SS_") + selections_SS.at (i).first ;
      selections_SS.at (i).second = selections_SS.at (i).second && TCut ("isOS == 0") ;
    }

  // get the variables to be plotted
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<string> variablesList = gConfigParser->readStringListOption ("general::variables") ;

  // calculate the QCD in the SS region as data - other_bkg
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  cout << "--- MAIN reading DATA and filling SS histos" << endl ;

  // get the same-sign distributions from data
  plotContainer SS_DATA_plots ("SS_DATA", variablesList, selections_SS, DATASamplesList, 2) ;
  counters SS_DATACount = fillHistos (DATASamples, SS_DATA_plots, 
              variablesList,
              selections_SS,
              lumi,
              vector<float> (0),
              true, false) ;
  SS_DATA_plots.AddOverAndUnderFlow () ;

  cout << "--- MAIN reading bkg and filling SS histos" << endl ;

  // get the same-sign distributions from bkg
  plotContainer SS_bkg_plots ("SS_bkg", variablesList, selections_SS, bkgSamplesList, 0) ;
  counters SS_bkgCount = fillHistos (bkgSamples, SS_bkg_plots, 
              variablesList,
              selections_SS,
              lumi,
              vector<float> (0),
              false, false) ;
  SS_bkg_plots.AddOverAndUnderFlow () ;

  cout << "--- MAIN preparing to loop on variables and selections to calc SS QCD" << endl ;

  // the index in the stack is based on variable ID (iv) and selection ID (isel):
  // iHisto = iv + nVars * isel
  vector<string> QCDsample ;
  QCDsample.push_back ("QCD") ;
  plotContainer SS_QCD ("SS_QCD", variablesList, selections_SS, QCDsample, 0) ;
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

  /* FIXME should we subtract signals as well? */
  /* NB if it has to be subtracted, it cannot be scaled! */

  // get the SS-to-OS scale factor and scale the QCD distributions
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  float SStoOS_scaleFactor = 1.06 ; // to be calculated here at a certain moment!!
  SS_QCD.scale (SStoOS_scaleFactor) ;

  int QCDcolor = gConfigParser->readIntOption ("colors::QCD") ;
  SS_QCD.setHistosProperties (0, QCDcolor) ; 

  // insert the QCD in the OS region
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  cout << "--- MAIN reading DATA and filling OS histos" << endl ;

  // get the opposite-sign distributions from data
  plotContainer OS_DATA_plots ("OS_DATA", variablesList, selections_OS, DATASamplesList, 2) ;
  counters OS_DATACount = fillHistos (DATASamples, OS_DATA_plots, 
              variablesList,
              selections_OS,
              lumi,
              vector<float> (0),
              true, false) ;
  OS_DATA_plots.AddOverAndUnderFlow () ;

  cout << "--- MAIN reading bkg and filling OS histos" << endl ;

  // get the opposite-sign distributions from bkg
  plotContainer OS_bkg_plots ("OS_bkg", variablesList, selections_OS, bkgSamplesList, 0) ;
  counters OS_bkgCount = fillHistos (bkgSamples, OS_bkg_plots, 
              variablesList,
              selections_OS,
              lumi,
              vector<float> (0),
              false, false) ;
  OS_bkg_plots.AddOverAndUnderFlow () ;
  OS_bkg_plots.addSample ("QCD", SS_QCD) ;
  
  cout << "--- MAIN reading sig and filling OS histos" << endl ;

  // get the opposite-sign distributions from sig
  plotContainer OS_sig_plots ("OS_sig", variablesList, selections_OS, sigSamplesList, 1) ;
  counters OS_sigCount = fillHistos (sigSamples, OS_sig_plots, 
              variablesList,
              selections_OS,
              lumi,
              vector<float> (0),
              false, true) ;
  OS_sig_plots.AddOverAndUnderFlow () ;

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
  fOut->Close () ;

  // Plot the histograms
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events/")) ;
  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/shapes/")) ;

  TCanvas * c = new TCanvas () ;

  cout << "--- MAIN before plotting" << endl ;

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

          TString outFolderName = outFolderNameBase + TString ("/events/") ;

          // get the extremes for the plot
          THStack * sig_stack = OS_sig_plots.makeStack ( variablesList.at (iv), 
                                    selections_OS.at (isel).first.Data ()) ;
          THStack * bkg_stack = OS_bkg_plots.makeStack ( variablesList.at (iv), 
                                    selections_OS.at (isel).first.Data ()) ;
          THStack * DATA_stack = OS_DATA_plots.makeStack ( variablesList.at (iv), 
                                    selections_OS.at (isel).first.Data ()) ;

          vector<float> extremes_bkg  = getExtremes (bkg_stack) ;
          vector<float> extremes_sig  = getExtremes (sig_stack) ;
          vector<float> extremes_DATA = getExtremes (DATA_stack) ;
          TH1F * bkg = c->DrawFrame (
              extremes_bkg.at (0) ,
              0.9 * min3 (extremes_bkg.at (1), extremes_sig.at (1), 
                          extremes_DATA.at (1)) ,
              extremes_bkg.at (2) ,
              1.3 * max3 (extremes_bkg.at (3), extremes_sig.at (3), 
                          extremes_DATA.at (3) + sqrt (extremes_DATA.at (3)))
            ) ;  
     
          copyTitles (bkg, bkg_stack) ;

          bkg->Draw () ;
          bkg_stack->Draw ("hist same") ;
          sig_stack->Draw ("nostack hist same") ;
          TH1F * h_data = (TH1F *) DATA_stack->GetStack ()->Last () ;
          // FIXME probably the data uncertainties need to be fixed
          h_data->Draw ("same") ;
          
          TString coutputName ;
          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;
          
          c->SetLogy (1) ;
          bkg->Draw () ;
          bkg_stack->Draw ("hist same") ;
          sig_stack->Draw ("nostack hist same") ;
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

          THStack * hstack_bkg_norm = normaliseStack (bkg_stack) ;
          TH1F * shape_bkg = (TH1F *) hstack_bkg_norm->GetStack ()->Last () ;
          
          THStack * hstack_sig_norm = normaliseStack (sig_stack) ;
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


  // print yields
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  unsigned int NSpacesColZero = 20 ;
  unsigned int NSpacesColumns = 10 ;
  unsigned int precision = 1 ;
  
  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " EXPECTED NUMBER OF SIG EVENTS\n\n" ;

  printTableTitle (sigSamples, NSpacesColZero, NSpacesColumns) ;
  printTableBody  (selections, OS_sigCount, sigSamples) ;

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " EXPECTED NUMBER OF BKG EVENTS\n\n" ;

  printTableTitle (bkgSamples, NSpacesColZero, NSpacesColumns) ;
  printTableBody  (selections, OS_bkgCount, bkgSamples) ;

  return 0 ;
}  