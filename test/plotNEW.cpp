#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cctype>
#include <algorithm>
#include "TString.h"
#include "TChain.h"
#include "TCut.h"
#include "HistoManager.h"
#include "TTreeFormula.h"
#include "THStack.h"
#include "TCanvas.h"

#include "ConfigParser.h"
#include "utils.h"
#include "histoUtils.h"

using namespace std ;

/* TODO list
- add the legenda, axis titles from cfg
*/


/* how much space to leave for the legenda:
https://github.com/govoni/FlatNtStudy/blob/master/interface/plotter.h#L324
*/

void addTo (vector<float> & total, vector<float> & addition)
{
  for (unsigned int i = 0 ; i < total.size () ; ++i)
    total.at (i) += addition.at (i) ;
  return ;  
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


struct counters 
{
  vector<vector<float> > counters ; // [sample][selection]
  vector<float> initEfficiencies ; // [sample]

  vector<float> getTotalCountsPerCut ()
    {
      vector<float> total (counters.at(0).size (), 0.) ;
      // loop on the samples
      for (unsigned int i = 0 ; i < counters.size () ; ++i) 
        addTo (total, counters.at (i)) ;
      return total ;
    }

} ;


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// void printTitle (vector<sample> & sample, unsigned int NSpacesColZero, unsigned int NSpacesColumns) 
// {
//   for (unsigned int i = 0 ; i < NSpacesColZero ; ++i) cout << " " ;
//   cout << "| " ;
//   for (unsigned int iSample = 0 ; iSample < sample.size () ; ++iSample)
//     {
//       string word = string (sample.at (iSample).sampleName.Data ()).substr (0, NSpacesColumns) ;
//       cout << word ;
//       for (unsigned int i = 0 ; i < NSpacesColumns - word.size () ; ++i) cout << " " ;
//       cout << " | " ;
//     }
//   cout << "\n" ; 
// }
// 
// 


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


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void
addHistos (vector<sample> & samples, HistoManager * manager,
           vector<string> & variablesList,
           vector<pair <TString, TCut> > & selections,
           bool isSignal,
           bool isData = false)
{
  TString histoName ;
  int histoType = 0 ;
  if (isSignal) histoType = 1 ;
  if (isData) histoType = 2 ;

  // loop on sim samples
  for (unsigned int j = 0 ; j < samples.size () ; ++j)
    {
      for (unsigned int k = 0 ; k < selections.size () ; ++k)
        {
          for (unsigned int i = 0 ; i < variablesList.size () ; ++i)
            {
              histoName.Form ("%s_%s_%s",
                              variablesList.at (i).c_str (),
                              samples.at (j).sampleName.Data (),
                              selections.at (k).first.Data ()
                              ) ;
              // remove not alphanumeric symbols from the var name
              string varID = variablesList.at (i) ;
              varID.erase (std::remove_if (varID.begin (), varID.end (), isNOTalnum ()), varID.end ()) ;
              // get histo nbins and range
              vector <float> limits = 
                gConfigParser->readFloatListOption (TString ("histos::") 
                    + varID.c_str ()) ;
              manager->AddNewHisto (histoName.Data (),histoName.Data (),
                  int (limits.at (0)), limits.at (1), limits.at (2),
                  gConfigParser->readIntOption (TString ("colors::") 
                      + samples.at (j).sampleName.Data ()), 
                  histoType,
                  variablesList.at (i).c_str (), "events"
                ) ;
            }  
        }
    } // loop on sim samples

  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


counters
fillHistos (vector<sample> & samples, HistoManager * manager, 
            vector<string> & variablesList,
            vector<pair <TString, TCut> > & selections,
            float lumi,
            const vector<float> & scale,
            bool isData = false,
            bool isSignal = false)
{
  addHistos (samples, manager, variablesList, selections, isSignal, isData) ;

  TString histoName ;

  // for efficiency evaluation
  counters localCounter ;

  //Loop on the samples
  for (unsigned int iSample = 0 ; iSample < samples.size () ; ++iSample)
    {
      double eff = samples.at (iSample).eff ;
      localCounter.initEfficiencies.push_back (eff) ;
      localCounter.counters.push_back (vector<float> (variablesList.size () + 1, 0.)) ;

      TTree *tree = samples.at (iSample).sampleTree ;
      TTreeFormula * TTF[selections.size ()] ;
      for (unsigned int isel = 0 ; isel < selections.size () ; ++isel)
        {
          TString fname ; fname.Form ("ttf%d",isel) ;
          TTF[isel] = new TTreeFormula (fname.Data (), selections.at (isel).second, tree) ;
        }
      float weight ;
      tree->SetBranchAddress ("MC_weight", &weight) ;
      // signal scaling
      float scaling = 1. / samples.at (iSample).eff_den ;
      if (scale.size () > 0) scaling *= scale.at (iSample) ;

      cout << "Opening sample: "
           << samples.at (iSample).sampleName
           << "\t with initial weighted events\t" << samples.at (iSample).eff_den
           << endl ;

      vector<float> address (variablesList.size (), 0.) ; 
      for (unsigned int iv = 0 ; iv < variablesList.size () ; ++iv)
        tree->SetBranchAddress (variablesList.at (iv).c_str (), &(address.at (iv))) ;
    
      for (int iEvent = 0 ; iEvent < tree->GetEntries () ; ++iEvent)
        {
          tree->GetEntry (iEvent) ;
          if (isData) localCounter.counters.at (iSample).at (0) += 1. ;
          else        localCounter.counters.at (iSample).at (0) 
                          += weight * lumi * scaling ;
          for (unsigned int isel = 0 ; isel < selections.size () ; ++isel)
            {
              if (! TTF[isel]->EvalInstance ()) continue ;

              if (isData) localCounter.counters.at (iSample).at (isel+1) += 1. ;
              else        localCounter.counters.at (iSample).at (isel+1) 
                              += weight * lumi * scaling ;

              for (unsigned int iv = 0 ; iv < variablesList.size () ; ++iv)
                {
                  histoName.Form ("%s_%s_%s",
                      variablesList.at (iv).c_str (),
                      samples.at (iSample).sampleName.Data (),
                      selections.at (isel).first.Data ()
                    ) ;
                  TH1F * histo = manager->GetHisto (histoName.Data ()) ;
                  if (isData) 
                      histo->Fill (address[iv]) ;
                  else        
                      histo->Fill (address[iv], weight * lumi * scaling) ;
                } //loop on variables
            } //loop on selections
        } //loop on tree entries

      for (unsigned int isel = 0 ; isel < selections.size () ; ++isel) delete TTF[isel] ;
    } // Loop on the MC samples

  return localCounter ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


vector<THStack *> 
stackHistos (vector<sample> & samples, HistoManager * manager, 
            vector<string> & variablesList,
            vector<pair <TString, TCut> > & selections,
            const string & tag)
{
  int nVars = variablesList.size () ;
  int nSel = selections.size () ;
  TString outputName, histoName ;
  
  vector <THStack *> hstack (nVars*nSel) ; //one stack for variable

  for (int isel = 0 ; isel < nSel ; ++isel)
    {
      for (int iv = 0 ; iv < nVars ; ++iv)
        {
          // filling stacks for background
          outputName.Form ("stack_%s_%s_%s",
            tag.c_str (),
            variablesList.at (iv).c_str (), selections.at (isel).first.Data ()) ;
          hstack.at (iv+nVars*isel) = new THStack (outputName.Data (), outputName.Data ()) ;
          for (unsigned int i = 0 ; i < samples.size () ; ++i)
            {
              histoName.Form ("%s_%s_%s",
                  variablesList.at (iv).c_str (),
                  samples.at (i).sampleName.Data (),
                  selections.at (isel).first.Data ()
                ) ;
              addOverAndUnderFlow (manager->GetHisto (histoName.Data ())) ;
              hstack.at (iv+nVars*isel)->Add (manager->GetHisto (histoName.Data ())) ;
            } 
        }
    }
  return hstack ;
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

  vector<pair <TString, TCut> > selections = readCutsFile (
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

  TString histoName ;
  HistoManager * manager = new HistoManager ("test") ;

  counters bkgCount = fillHistos (bkgSamples, manager, 
              variablesList,
              selections,
              lumi,
              vector<float> (0),
              false, false) ;

  counters sigCount = fillHistos (sigSamples, manager, 
              variablesList,
              selections,
              lumi,
              signalScales,
              false, true) ;

  counters DATACount = fillHistos (DATASamples, manager, 
              variablesList,
              selections,
              lumi,
              vector<float> (0),
              true, false) ;


  // Plot the histograms
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TString outFolderNameBase = gConfigParser->readStringOption ("general::outputFolderName") ;
  
  system (TString ("mkdir -p ") + outFolderNameBase) ;
  TString outString ;
  outString.Form (outFolderNameBase + "outPlotter.root") ;
  TFile * fOut = new TFile (outString.Data (), "RECREATE") ;
  manager->SaveAllToFile (fOut) ;

  vector<THStack *> hstack_bkg = stackHistos (
      bkgSamples, manager, 
      variablesList,
      selections, "bkg") ;

  vector<THStack *> hstack_sig = stackHistos (
      sigSamples, manager, 
      variablesList,
      selections, "sig") ;

  vector<THStack *> hstack_DATA = stackHistos (
      DATASamples, manager, 
      variablesList,
      selections, "DATA") ;

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
          vector<float> extremes_bkg  = getExtremes (hstack_bkg.at  (iv+nVars*isel)) ;
          vector<float> extremes_sig  = getExtremes (hstack_sig.at  (iv+nVars*isel)) ;
          vector<float> extremes_DATA = getExtremes (hstack_DATA.at (iv+nVars*isel)) ;
          TH1F * bkg = c->DrawFrame (
              extremes_bkg.at (0) ,
              0.9 * min3 (extremes_bkg.at (1), extremes_sig.at (1), 
                          extremes_DATA.at (1)) ,
              extremes_bkg.at (2) ,
              1.3 * max3 (extremes_bkg.at (3), extremes_sig.at (3), 
                          extremes_DATA.at (3) + sqrt (extremes_DATA.at (3)))
            ) ;  
          copyTitles (bkg, hstack_bkg.at (iv+nVars*isel)) ;

          bkg->Draw () ;
          hstack_bkg.at (iv+nVars*isel)->Draw ("hist same") ;
          hstack_sig.at (iv+nVars*isel)->Draw ("nostack hist same") ;
          TH1F * h_data = (TH1F *) hstack_DATA.at (iv+nVars*isel)->GetStack ()->Last () ;
          // FIXME probably the data uncertainties need to be fixed
          h_data->Draw ("same") ;
          
          TString coutputName ;
          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;
          
          c->SetLogy (1) ;
          bkg->Draw () ;
          hstack_bkg.at (iv+nVars*isel)->Draw ("hist same") ;
          hstack_sig.at (iv+nVars*isel)->Draw ("nostack hist same") ;
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

          THStack * hstack_bkg_norm = normaliseStack (hstack_bkg.at (iv+nVars*isel)) ;
          TH1F * shape_bkg = (TH1F *) hstack_bkg_norm->GetStack ()->Last () ;
          
          THStack * hstack_sig_norm = normaliseStack (hstack_sig.at (iv+nVars*isel)) ;
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
 
  unsigned int NSpacesColZero = 12 ;
  unsigned int NSpacesColumns = 8 ;
  unsigned int precision = 1 ;
  
  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " EXPECTED NUMBER OF SIG EVENTS\n\n" ;

  printTitle (sigSamples, NSpacesColZero, NSpacesColumns) ;

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

  printTitle (bkgSamples, NSpacesColZero, NSpacesColumns) ;

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
  printTitle (DATASamples, NSpacesColZero, NSpacesColumns) ;

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
  printTitle (titles, NSpacesColZero, NSpacesColumns) ;
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
