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

#include "../interface/HistoManager.h"
#include "../interface/ConfigParser.h"
#include "../interface/utils.h"
#include "../interface/histoUtils.h"
#include "../interface/plotContainer.h"
#include "../interface/analysisUtils.h"

using namespace std ;

/* TODO list
   - add the legenda, axis titles from cfg
*/


/* how much space to leave for the legenda:
   https://github.com/govoni/FlatNtStudy/blob/master/interface/plotter.h#L324
*/


/*
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void printTitle (vector<string> & sample, unsigned int NSpacesColZero, unsigned int NSpacesColumns)
{
for (unsigned int i = 0 ; i < NSpacesColZero ; ++i) cout << " " ;
cout << "| " ;
for (unsigned int iSample = 0 ; iSample < sample.size () ; ++iSample)
{
string word = sample.at (iSample).substr (0, NSpacesColumns) ;
cout << word ;
for (unsigned int i = 0 ; i < NSpacesColumns - word.size () ; ++i) cout << " " ;
cout << " | " ;
}
cout << "\n" ;
return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void printTitle (vector<sample> & sample, unsigned int NSpacesColZero, unsigned int NSpacesColumns)
{
vector<string> names ;
for (unsigned int iSample = 0 ; iSample < sample.size () ; ++iSample)
names.push_back (sample.at (iSample).sampleName.Data ()) ;
printTitle (names, NSpacesColZero, NSpacesColumns) ;
return ;
}
*/

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
  vector<mysample> sigSamples ;
  readSamples (sigSamples, sigSamplesList) ;

  vector<float> signalScales ;
  for (unsigned int i = 0 ; i < sigSamplesList.size () ; ++i)
  {
    string name = string ("samples::") + sigSamplesList.at (i) + string ("FACT") ;
    float scale = gConfigParser->readFloatOption (name.c_str ()) ;
    signalScales.push_back (scale) ;
  }

  vector<string> bkgSamplesList = gConfigParser->readStringListOption ("general::backgrounds") ;
  vector<mysample> bkgSamples ;
  readSamples (bkgSamples, bkgSamplesList) ;

  vector<string> DATASamplesList = gConfigParser->readStringListOption ("general::data") ;
  vector<mysample> DATASamples ;
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
  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;

  // get the variables to be plotted
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<string> variablesList = gConfigParser->readStringListOption ("general::variables") ;

  plotContainer bkg_plots ("bkg", variablesList, selections, bkgSamplesList, 0) ;
  counters bkgCount = fillHistos (bkgSamples, bkg_plots,
				  variablesList,
				  selections,
				  lumi,
				  vector<float> (0),
				  false, false) ;
  bkg_plots.AddOverAndUnderFlow () ;

  plotContainer sig_plots ("sig", variablesList, selections, sigSamplesList, 1) ;
  counters sigCount = fillHistos (sigSamples, sig_plots,
				  variablesList,
				  selections,
				  lumi,
				  signalScales,
				  false, true) ;
  sig_plots.AddOverAndUnderFlow () ;

  plotContainer DATA_plots ("DATA", variablesList, selections, DATASamplesList, 1) ;
  counters DATACount = fillHistos (DATASamples, DATA_plots,
				   variablesList,
				   selections,
				   lumi,
				   vector<float> (0),
				   true, false) ;
  DATA_plots.AddOverAndUnderFlow () ;

  // Save the histograms
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TString outFolderNameBase = gConfigParser->readStringOption ("general::outputFolderName") ;
  outFolderNameBase += "/" ;
  system (TString ("mkdir -p ") + outFolderNameBase) ;
  outFolderNameBase += gConfigParser->readStringOption ("plotNEW::outputFolderName") ;
  outFolderNameBase += "/" ;
  system (TString ("mkdir -p ") + outFolderNameBase) ;

  TString outString ;
  outString.Form (outFolderNameBase + "outPlotter.root") ;
  TFile * fOut = new TFile (outString.Data (), "RECREATE") ;
  bkg_plots.save (fOut) ;
  sig_plots.save (fOut) ;
  DATA_plots.save (fOut) ;
  fOut->Close () ;

  // Plot the histograms
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

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
      THStack * sig_stack = sig_plots.makeStack ( variablesList.at (iv),
						  selections.at (isel).first.Data ()) ;
      THStack * bkg_stack = bkg_plots.makeStack ( variablesList.at (iv),
						  selections.at (isel).first.Data ()) ;
      THStack * DATA_stack = DATA_plots.makeStack ( variablesList.at (iv),
						    selections.at (isel).first.Data ()) ;

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

  // printout tables
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  unsigned int NSpacesColZero = 20 ;
  unsigned int NSpacesColumns = 12 ;
  unsigned int precision = 1 ;

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " EXPECTED NUMBER OF SIG EVENTS\n\n" ;

  printTableTitle (std::cout, sigSamples, NSpacesColZero, NSpacesColumns) ;

  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
  {
    cout << selections.at (iSel).first ;
    for (unsigned int i = 0 ; i < NSpacesColZero - string(selections.at (iSel).first.Data ()).size () ; ++i) cout << " " ;
    cout << "|" ;
    for (unsigned int iSample = 0 ; iSample < sigSamples.size () ; ++iSample)
    {
      float evtnum = sigCount.counters.at (iSample).at (iSel+1) ;
      int subtractspace = 0 ;
      if (evtnum > 0) subtractspace = int (log10 (evtnum)) + precision + 1 ;
      for (unsigned int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) cout << " " ;
      cout << setprecision (precision) << fixed << evtnum
	   << " |" ;
    }
    cout << "\n" ;
  }

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " EXPECTED NUMBER OF BKG EVENTS\n\n" ;

  printTableTitle (std::cout, bkgSamples, NSpacesColZero, NSpacesColumns) ;

  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
  {
    cout << selections.at (iSel).first ;
    for (unsigned int i = 0 ; i < NSpacesColZero - string(selections.at (iSel).first.Data ()).size () ; ++i) cout << " " ;
    cout << "|" ;
    for (unsigned int iSample = 0 ; iSample < bkgSamples.size () ; ++iSample)
    {
      float evtnum = bkgCount.counters.at (iSample).at (iSel+1) ;
      int subtractspace = 0 ;
      if (evtnum > 0) subtractspace = int (log10 (evtnum)) + precision + 1 ;
      for (unsigned int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) cout << " " ;
      cout << setprecision (1) << fixed << evtnum
	   << " |" ;
    }
    cout << "\n" ;
  }

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " OBSERVED NUMBER OF EVENTS\n\n" ;

  NSpacesColumns = 12 ;
  printTableTitle (std::cout, DATASamples, NSpacesColZero, NSpacesColumns) ;

  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
  {
    cout << selections.at (iSel).first ;
    for (unsigned int i = 0 ; i < NSpacesColZero - string(selections.at (iSel).first.Data ()).size () ; ++i) cout << " " ;
    cout << "|" ;
    for (unsigned int iSample = 0 ; iSample < DATASamples.size () ; ++iSample)
    {
      float evtnum = DATACount.counters.at (iSample).at (iSel+1) ;
      int subtractspace = 0 ;
      if (evtnum > 0) subtractspace = int (log10 (evtnum)) ;
      for (unsigned int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) cout << " " ;
      cout << setprecision (0) << fixed << evtnum
	   << " |" ;
    }
    cout << "\n" ;
  }

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " TOTALS\n\n" ;

  vector<float> DATAtotal = DATACount.getTotalCountsPerCut () ;
  vector<float> bkgtotal = bkgCount.getTotalCountsPerCut () ;
  vector<string> titles ; titles.push_back ("DATA") ; titles.push_back ("bkg") ;
  printTableTitle (std::cout, titles, NSpacesColZero, NSpacesColumns) ;
  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
  {
    cout << selections.at (iSel).first ;
    for (unsigned int i = 0 ; i < NSpacesColZero - string(selections.at (iSel).first.Data ()).size () ; ++i) cout << " " ;
    cout << "|" ;

    float evtnum = DATAtotal.at (iSel+1) ;
    int subtractspace = 0 ;
    if (evtnum > 0) subtractspace = int (log10 (evtnum)) ;
    for (unsigned int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) cout << " " ;
    cout << setprecision (0) << fixed << evtnum
	 << " |" ;

    evtnum = bkgtotal.at (iSel+1) ;
    subtractspace = 0 ;
    if (evtnum > 0) subtractspace = int (log10 (evtnum)) ;
    for (unsigned int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) cout << " " ;
    cout << setprecision (0) << fixed << evtnum
	 << " |\n" ;
  }

  return 0 ;

}
