#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cctype>
#include <algorithm>
#include "ConfigParser.h"
#include "utils.h"
#include "TString.h"
#include "TChain.h"
#include "TCut.h"
#include "HistoManager.h"
#include "TTreeFormula.h"
#include "THStack.h"
#include "TCanvas.h"

#include "histoUtils.h"

using namespace std ;

/* TODO list
- add the legenda, axis titles from cfg
*/


/* how much space to leave for the legenda:
https://github.com/govoni/FlatNtStudy/blob/master/interface/plotter.h#L324
*/

// xmin ymin xmax ymax
vector<float> getExtremes (THStack * hstack, bool islog = false)
{
  float ymax = hstack->GetMaximum () * 1.3;

  TIter next (hstack->GetHists ()) ;
  TH1F * histo ;

  float xmin = 1. ;
  float xmax = 0. ;
  float ymin = 10000000000. ;
  while (histo = (TH1F *) (next ())) 
    {
      float tmpmin = findNonNullMinimum (histo) ;
      if (tmpmin < ymin) ymin = tmpmin ;
      if (xmin > xmax)
        {
          xmin = histo->GetXaxis ()->GetXmin () ;
          xmax = histo->GetXaxis ()->GetXmax () ;
        }
    }

//  ymin *= 0.9 ;

  vector<float> extremes (4, 0.) ;
  extremes.at (0) = xmin ;
  extremes.at (1) = ymin ;
  extremes.at (2) = xmax ;
  extremes.at (3) = ymax ;

  return extremes ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


struct isNOTalnum : std::unary_function<int, int>
{
    int operator()(int i) const { return !std::isalnum (i) ; }
};


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void printTitle (vector<sample> & sample, int NSpacesColZero, int NSpacesColumns) 
{
  for (unsigned int i = 0 ; i < NSpacesColZero ; ++i) cout << " " ;
  cout << "| " ;
  for (unsigned int iSample = 0 ; iSample < sample.size () ; ++iSample)
    {
      string word = string (sample.at (iSample).sampleName.Data ()).substr (0, NSpacesColumns) ;
      cout << word ;
      for (unsigned int i = 0 ; i < NSpacesColumns - word.size () ; ++i) cout << " " ;
      cout << " | " ;
    }
  cout << "\n" ; 
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


float min3 (float uno, float due, float tre)
{
  return min (min (uno, due), tre) ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


float max3 (float uno, float due, float tre)
{
  return max (max (uno, due), tre) ;
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

  // FIXME si riesce ad evitare questa duplicazione?
  vector<sample> allSamples (bkgSamples) ;  
  allSamples.insert (allSamples.end (), sigSamples.begin (), sigSamples.end ()) ;
  
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

  //Configurables: files and vars
  const int nB = bkgSamples.size () ;
  const int nS = sigSamples.size () ;
  const int nVars =variablesList.size () ;
  const int nSel = selections.size () ;

  TString histoName ;
  HistoManager * manager = new HistoManager ("test") ;

  // Create the histograms
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  // loop on sim samples
  for (int j = 0 ; j<nB+nS ; ++j)
    {
      for (int k = 0 ; k < nSel ; ++k)
        {
          for (int i = 0 ; i < nVars ; ++i)
            {
              histoName.Form ("%s_%s_%s",
                              variablesList.at (i).c_str (),
                              allSamples.at (j).sampleName.Data (),
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
                      + allSamples.at (j).sampleName.Data ()), 
                  (j >= nB),
                  variablesList.at (i).c_str (), "events"
                ) ;
            }  
        }
    } // loop on sim samples

  // data
  for (int k = 0 ; k < nSel ; ++k)
    {
      for (int i = 0 ; i < nVars ; ++i)
        {
          histoName.Form ("data_%s_%s",
                          variablesList.at (i).c_str (),
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
              gConfigParser->readIntOption ("colors::data"), 
              2,
              variablesList.at (i).c_str (), "events"
            ) ;
        }  
    } // data

  // Fill the histograms
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  // for efficiency evaluation
  vector<vector<float> > counters ; // [sample][selection]
  vector<float> initEfficiencies ; // [sample]

  //Loop on the MC samples
  for (int iSample = 0 ; iSample < nB+nS ; ++iSample)
    {
      double eff = allSamples.at (iSample).eff ;
      initEfficiencies.push_back (eff) ;
      counters.push_back (vector<float> (nSel+1, 0.)) ;
      TTree *tree = (TTree*) allSamples.at (iSample).sampleChain->GetTree () ;
      TTreeFormula * TTF[nSel] ;
      for (int isel = 0 ; isel < nSel ; ++isel)
        {
          TString fname ; fname.Form ("ttf%d",isel) ;
          TTF[isel] = new TTreeFormula (fname.Data (), selections.at (isel).second, tree) ;
        }
      float weight ;
      tree->SetBranchAddress ("MC_weight", &weight) ;
      // signal scaling
      float scaling = 1./allSamples.at (iSample).eff_num ;
      if (iSample >= nB) scaling *= signalScales.at (iSample - nB) ;

      cout << "Opening sample "
           << allSamples.at (iSample).sampleName
           << " with efficiency " << eff
           << endl ;

      vector<float> address (nVars, 0.) ; 
      for (int iv = 0 ; iv < nVars ; ++iv)
        tree->SetBranchAddress (variablesList.at (iv).c_str (), &(address.at (iv))) ;
    
      for (int iEvent = 0 ; iEvent < tree->GetEntries () ; ++iEvent)
        {
          tree->GetEntry (iEvent) ;
          counters.at (iSample).at (0) += weight * lumi ;
          for (int isel = 0 ; isel < nSel ; ++isel)
            {
              if (! TTF[isel]->EvalInstance ()) continue ;
              counters.at (iSample).at (isel + 1) += weight * lumi ;
              for (int iv = 0 ; iv < nVars ; ++iv)
                {
                  histoName.Form ("%s_%s_%s",
                      variablesList.at (iv).c_str (),
                      allSamples.at (iSample).sampleName.Data (),
                      selections.at (isel).first.Data ()
                    ) ;
                  manager->GetHisto (histoName.Data ())->Fill (address[iv], weight * lumi * scaling) ;
                } //loop on variables
            } //loop on selections
        } //loop on tree entries

      for (int isel = 0 ; isel < nSel ; ++isel) delete TTF[isel] ;
    } // Loop on the MC samples

  // for efficiency evaluation
  vector<vector<float> > data_counters ; // [sample][selection]
  vector<float> data_initEfficiencies ; // [sample]

  //Loop on the data samples
  for (int iData = 0 ; iData < DATASamples.size () ; ++iData)
    {
      double eff = DATASamples.at (iData).eff ;
      data_initEfficiencies.push_back (eff) ;
      data_counters.push_back (vector<float> (nSel+1, 0.)) ;
      TTree *tree = (TTree*) DATASamples.at (iData).sampleChain->GetTree () ;
      TTreeFormula * TTF[nSel] ;
      for (int isel = 0 ; isel < nSel ; ++isel)
        {
          TString fname ; fname.Form ("ttf%d",isel) ;
          TTF[isel] = new TTreeFormula (fname.Data (), selections.at (isel).second, tree) ;
        }
      float weight ;
      tree->SetBranchAddress ("MC_weight", &weight) ;
      
      cout << "Opening sample "
           << DATASamples.at (iData).sampleName
           << " with efficiency " << eff
           << endl ;

      vector<float> address (nVars, 0.) ; 
      for (int iv = 0 ; iv < nVars ; ++iv)
        tree->SetBranchAddress (variablesList.at (iv).c_str (), &(address.at (iv))) ;
    
      for (int iEvent = 0 ; iEvent < tree->GetEntries () ; ++iEvent)
        {
          tree->GetEntry (iEvent) ;
          data_counters.at (iData).at (0) += 1. ;
          for (int isel = 0 ; isel < nSel ; ++isel)
            {
              if (! TTF[isel]->EvalInstance ()) continue ;
              data_counters.at (iData).at (isel + 1) += 1. ;
              for (int iv = 0 ; iv < nVars ; ++iv)
                {
                  histoName.Form ("data_%s_%s",
                      variablesList.at (iv).c_str (),
                      selections.at (isel).first.Data ()
                    ) ;
                  manager->GetHisto (histoName.Data ())->Fill (address[iv]) ;
                } //loop on variables
            } //loop on selections
        } //loop on tree entries

      for (int isel = 0 ; isel < nSel ; ++isel) delete TTF[isel] ;
    } // Loop on the data samples

  // Plot the histograms
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TString outFolderNameBase = gConfigParser->readStringOption ("general::outputFolderName") ;
  
  system (TString ("mkdir -p ") + outFolderNameBase) ;
  TString outString ;
  outString.Form (outFolderNameBase + "outPlotter.root") ;
  TFile * fOut = new TFile (outString.Data (), "RECREATE") ;
  manager->SaveAllToFile (fOut) ;

  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events/")) ;
  system (TString ("mkdir -p ") + outFolderNameBase + TString ("/shapes/")) ;

  //make Stack plots
  vector <THStack *> hstack_bkg (nVars*nSel) ; //one stack for variable
  vector <THStack *> hstack_sig (nVars*nSel) ; //one stack for variable

  TCanvas * c = new TCanvas () ;

  for (int isel = 0 ; isel < nSel ; ++isel)
    {
      for (int iv = 0 ; iv < nVars ; ++iv)
        {
          TString outputName ; 

          // filling stacks for background
          outputName.Form ("stack_bkg_%s_%s",
            variablesList.at (iv).c_str (), selections.at (isel).first.Data ()) ;
          hstack_bkg.at (iv+nVars*isel) = new THStack (outputName.Data (), outputName.Data ()) ;
          for (int i = 0 ; i < nB ; ++i)
            {
              histoName.Form ("%s_%s_%s",
                  variablesList.at (iv).c_str (),
                  allSamples.at (i).sampleName.Data (),
                  selections.at (isel).first.Data ()
                ) ;
              addOverAndUnderFlow (manager->GetHisto (histoName.Data ())) ;
              hstack_bkg.at (iv+nVars*isel)->Add (manager->GetHisto (histoName.Data ())) ;
            }

          // filling stacks for signal

          outputName.Form ("stack_sig_%s_%s",
            variablesList.at (iv).c_str (), selections.at (isel).first.Data ()) ;
          hstack_sig.at (iv+nVars*isel) = new THStack (outputName.Data (), outputName.Data ()) ;
          //superimpose the signals
          for (int i = nB ; i< nB+nS ; ++i)
            {
              histoName.Form ("%s_%s_%s",
                  variablesList.at (iv).c_str (),
                  allSamples.at (i).sampleName.Data (),
                  selections.at (isel).first.Data ()
                ) ;
              addOverAndUnderFlow (manager->GetHisto (histoName.Data ())) ;
              hstack_sig.at (iv+nVars*isel)->Add (manager->GetHisto (histoName.Data ())) ;
            }
          
          // saving stacks
          
          fOut->cd () ;
          hstack_bkg.at (iv+nVars*isel)->Write () ;
          hstack_sig.at (iv+nVars*isel)->Write () ;

          // plotting with normalisation
          // ---- ---- ---- ---- ---- ---- ---- ---- ----

          TString outFolderName = outFolderNameBase + TString ("/events/") ;
          c->cd () ;

          vector<float> extremes_bkg = getExtremes (hstack_bkg.at (iv+nVars*isel)) ;
          vector<float> extremes_sig = getExtremes (hstack_sig.at (iv+nVars*isel)) ;

          histoName.Form ("data_%s_%s",
              variablesList.at (iv).c_str (),
              selections.at (isel).first.Data ()
            ) ;
          TH1F * h_data = manager->GetHisto (histoName.Data ()) ;
          addOverAndUnderFlow (h_data) ;

          TH1F * bkg = c->DrawFrame (
              extremes_bkg.at (0) ,
              0.9 * min3 (extremes_bkg.at (1), extremes_sig.at (1), 
                          findNonNullMinimum (h_data)) ,
//              std::min (extremes_bkg.at (1), extremes_sig.at (1)) ,
              extremes_bkg.at (2) ,
//              std::max (extremes_bkg.at (3), extremes_sig.at (3))
              1.1 * max3 (extremes_bkg.at (3), extremes_sig.at (3), 
                          h_data->GetMaximum () + sqrt (h_data->GetMaximum ()))
            ) ;  
          copyTitles (bkg, hstack_bkg.at (iv+nVars*isel)) ;
          
          hstack_bkg.at (iv+nVars*isel)->Draw ("hist same") ;
          hstack_sig.at (iv+nVars*isel)->Draw ("nostack hist same") ;
          
          h_data->Draw ("same") ;
          
          TString coutputName ;
          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;
          
          c->SetLogy (1) ;
          bkg->Draw () ;
          hstack_bkg.at (iv+nVars*isel)->Draw ("hist same") ;
          hstack_sig.at (iv+nVars*isel)->Draw ("nostack hist same") ;
          manager->GetHisto (histoName.Data ())->Draw ("same") ;

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

        }
    } //make stack plots

  delete c ;

  // printout efficiency tables
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " TOTAL EFFICIENCY WRT THE SKIMMED NTUPLES (%)\n\n" ;
  unsigned int NSpacesColZero = 12 ;
  unsigned int NSpacesColumns = 6 ;
  unsigned int precision = 2 ;

  printTitle (allSamples, NSpacesColZero, NSpacesColumns) ;

  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
    {
      cout << selections.at (iSel).first ;
      for (unsigned int i = 0 ; i < NSpacesColZero - string(selections.at (iSel).first.Data ()).size () ; ++i) cout << " " ;
      cout << "|" ;
      for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
        {
          float efficiency = 100. * counters.at (iSample).at (iSel+1) / counters.at (iSample).at (0) ;
          cout << " " ;
          if (efficiency < 100) cout << " " ;
          if (efficiency < 10) cout << " " ;
          cout << setprecision (precision) << fixed << efficiency
               << " |" ;
        }
      cout << "\n" ;
    }
  
  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " TOTAL EFFICIENCY WRT THE INITIAL SAMPLE (%)\n\n" ;

  printTitle (allSamples, NSpacesColZero, NSpacesColumns) ;

  string name = "skim" ;
  cout << name ;
  for (unsigned int i = 0 ; i < NSpacesColZero - name.size () ; ++i) cout << " " ;
  cout << "|" ;
  for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
    {
      float efficiency = 100. * initEfficiencies.at (iSample) ;
      cout << " " ;
      if (efficiency < 100) cout << " " ;
      if (efficiency < 10) cout << " " ;
      cout << setprecision (precision) << fixed << efficiency
           << " |" ;
    }
  cout << "\n" ; 

  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
    {
      cout << selections.at (iSel).first ;
      for (unsigned int i = 0 ; i < NSpacesColZero - string(selections.at (iSel).first.Data ()).size () ; ++i) cout << " " ;
      cout << "|" ;
      for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
        {
          float efficiency = 100. * initEfficiencies.at (iSample) 
                             * counters.at (iSample).at (iSel+1) / counters.at (iSample).at (0) ;
          cout << " " ;
          if (efficiency < 100) cout << " " ;
          if (efficiency < 10) cout << " " ;
          cout << setprecision (precision) << fixed << efficiency
               << " |" ;
        }
      cout << "\n" ;
    }
  
  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " RELATIVE EFFICIENCY WRT THE PREVIOUS STEP (%)\n\n" ;

  printTitle (allSamples, NSpacesColZero, NSpacesColumns) ;

  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
    {
      cout << selections.at (iSel).first ;
      for (unsigned int i = 0 ; i < NSpacesColZero - string(selections.at (iSel).first.Data ()).size () ; ++i) cout << " " ;
      cout << "|" ;
      for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
        {
          float efficiency = 100. * counters.at (iSample).at (iSel+1) / counters.at (iSample).at (iSel) ;
          cout << " " ;
          if (efficiency < 100) cout << " " ;
          if (efficiency < 10) cout << " " ;
          cout << setprecision (precision) << fixed << efficiency
               << " |" ;
        }
      cout << "\n" ;
    }
  
  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " DETAILS OF THE SKIM OF THE INITIAL SAMPLE (%)\n\n" ;

  NSpacesColumns = 14 ;
  printTitle (allSamples, NSpacesColZero, NSpacesColumns) ;

  name = "total" ;
  cout << name ;
  for (unsigned int i = 0 ; i < NSpacesColZero - name.size () ; ++i) cout << " " ;
  cout << "|" ;
  for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
    {
      float value = allSamples.at (iSample).eff_den * lumi ;
      int subtractspace = 0 ;
      if (value > 0) subtractspace = int (log10 (value)) ;
      for (int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) cout << " " ;
      cout << setprecision (0) << fixed << value << " |" ;
    }
  cout << "\n" ; 

  name = "skimmed" ;
  cout << name ;
  for (unsigned int i = 0 ; i < NSpacesColZero - name.size () ; ++i) cout << " " ;
  cout << "|" ;
  for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
    {
      float value = allSamples.at (iSample).eff_num * lumi ;
      int subtractspace = 0 ;
      if (value > 0) subtractspace = int (log10 (value)) ;
      for (int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) cout << " " ;
      cout << setprecision (0) << fixed << value << " |" ;
    }
  cout << "\n" ; 

  name = "eff (%)" ;
  cout << name ;
  for (unsigned int i = 0 ; i < NSpacesColZero - name.size () ; ++i) cout << " " ;
  cout << "|" ;
  for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
    {
      float value = allSamples.at (iSample).eff * 100 ;
      int subtractspace = -1 * precision - 1 ;
      if (value > 0) subtractspace = int (log10 (value))  - precision - 1 ;
      for (int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) cout << " " ;
      cout << setprecision (2) << fixed << value << " |" ;
    }
  cout << "\n" ; 

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " EXPECTED NUMBER OF EVENTS\n\n" ;

  printTitle (allSamples, NSpacesColZero, NSpacesColumns) ;

  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
    {
      cout << selections.at (iSel).first ;
      for (unsigned int i = 0 ; i < NSpacesColZero - string(selections.at (iSel).first.Data ()).size () ; ++i) cout << " " ;
      cout << "|" ;
      for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
        {
          int subtractspace = 0 ;
          if (counters.at (iSample).at (iSel+1) > 0) 
              subtractspace = int (log10 (counters.at (iSample).at (iSel+1))) + 3 ;
          if (counters.at (iSample).at (iSel+1) < 0.1) ++subtractspace ;
          for (int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) cout << " " ;

          cout << setprecision (2) << fixed << counters.at (iSample).at (iSel+1)
               << " |" ;
        }
      cout << "\n" ;
    }
  
  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " TOTAL EXPECTED NUMBER OF EVENTS\n\n" ;

  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
    {
      cout << selections.at (iSel).first ;
      for (unsigned int i = 0 ; i < NSpacesColZero - string(selections.at (iSel).first.Data ()).size () ; ++i) cout << " " ;
      cout << "|" ;
      float total = 0. ;
      for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
        {
          total += counters.at (iSample).at (iSel+1) ;
        }
      int subtractspace = 0 ;
      if (total > 0) subtractspace = int (log10 (total)) + 3 ;
      if (total < 0.1) ++subtractspace ;
      for (int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) cout << " " ;

      cout << setprecision (2) << fixed << total << "\n" ;
    }

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << " OBSERVED NUMBER OF EVENTS\n\n" ;

  printTitle (DATASamples, NSpacesColZero, NSpacesColumns) ;

  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
    {
      cout << selections.at (iSel).first ;
      for (unsigned int i = 0 ; i < NSpacesColZero - string(selections.at (iSel).first.Data ()).size () ; ++i) cout << " " ;
      cout << "|" ;
      for (unsigned int iSample = 0 ; iSample < DATASamples.size () ; ++iSample)
        {
          float evtnum = data_counters.at (iSample).at (iSel+1) ;
          int subtractspace = 0 ;
          if (evtnum > 0) subtractspace = int (log10 (evtnum)) ;
          for (int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) cout << " " ;
          cout << setprecision (0) << fixed << evtnum
               << " |" ;
        }
      cout << "\n" ;
    }
  
}
