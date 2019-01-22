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
#include "utils.h"

using namespace std ;

/* TODO list

- add shape plots of each bkg and signal
- add the legenda, axis titles from cfg
- get the output folder from the command line
*/


//Replace histomanager with something that goes by index instead og looking for strings

//void FillHistos (int) ;
//TString Selection (int) ;

//TString Selection (int selection=0){
//  if (selection==0) return "HH_pt>0" ;
//  else if (selection==1)return "HH_pt>400" ;
//  return "" ;
//}




/* how much space to leave for the legenda:
https://github.com/govoni/FlatNtStudy/blob/master/interface/plotter.h#L324
*/



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

  // for efficiency evaluation
  vector<vector<float> > counters ; // [sample][selection]
  vector<float> initEfficiencies ; // [sample]

  for (int j = 0 ; j<nB+nS ; ++j)
    {
      counters.push_back (vector<float> (nSel+1, 0.)) ;
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
                  variablesList.at (i).c_str (), "a.u"
                ) ;
            }  
        }
    }

  // Fill the histograms
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  //Loop on the samples
  for (int iSample = 0 ; iSample < nB+nS ; ++iSample)
    {
      double eff = allSamples.at (iSample).eff ;
      initEfficiencies.push_back (eff) ;
      TTree *tree = allSamples.at (iSample).sampleTree ;
      TTreeFormula * TTF[nSel] ;
      for (int isel = 0 ; isel < nSel ; ++isel)
        {
          TString fname ; fname.Form ("ttf%d",isel) ;
          TTF[isel] = new TTreeFormula (fname.Data (), selections.at (isel).second, tree) ;
        }
      float weight ;
      tree->SetBranchAddress ("MC_weight", &weight) ;
      // signal scaling
      float scaling = 1. ;
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
    } // Loop on the samples

  // Plot the histograms
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TString outFolderName = gConfigParser->readStringOption ("general::outputFolderName") ;
  
  system (TString ("mkdir -p ") + outFolderName) ;
  TString outString ;
  outString.Form (outFolderName + "outPlotter.root") ;
  TFile * fOut = new TFile (outString.Data (), "RECREATE") ;
  manager->SaveAllToFile (fOut) ;

  outFolderName = "./plotter/events/";
  system (TString ("mkdir -p ") + outFolderName) ;
  outFolderName = "./plotter/shapes/";
  system (TString ("mkdir -p ") + outFolderName) ;

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

          outFolderName = "./plotter/events/";
          c->cd () ;

          vector<float> extremes_bkg = getExtremes (hstack_bkg.at (iv+nVars*isel)) ;
          vector<float> extremes_sig = getExtremes (hstack_sig.at (iv+nVars*isel)) ;

          TH1F * bkg = c->DrawFrame (
              extremes_bkg.at (0) ,
              std::min (extremes_bkg.at (1), extremes_sig.at (1)) ,
              extremes_bkg.at (2) ,
              std::max (extremes_bkg.at (3), extremes_sig.at (3))
            ) ;  
          copyTitles (bkg, hstack_bkg.at (iv+nVars*isel)) ;
          
          hstack_bkg.at (iv+nVars*isel)->Draw ("hist same") ;
          hstack_sig.at (iv+nVars*isel)->Draw ("nostack hist same") ;
          TString coutputName ;
          coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;
          
          c->SetLogy (1) ;
          bkg->Draw () ;
          hstack_bkg.at (iv+nVars*isel)->Draw ("hist same") ;
          hstack_sig.at (iv+nVars*isel)->Draw ("nostack hist same") ;
          coutputName.Form ("%s_log.pdf", (outFolderName + outputName).Data ()) ;
          c->SaveAs (coutputName.Data ()) ;
          c->SetLogy (0) ;
          
          // plotting shapes
          // ---- ---- ---- ---- ---- ---- ---- ---- ----

          outFolderName = "./plotter/shapes/";
          TString basename ;
          basename.Form ("shape_%s_%s",
                  variablesList.at (iv).c_str (),
                  selections.at (isel).first.Data ()
                ) ;

//           TString name = basename + "_bkg_norm" ;
//           TH1F * dummy = (TH1F *) hstack_bkg.at (iv+nVars*isel)->GetStack ()->Last () ;
//           TH1F * shape_bkg = (TH1F *) dummy->Clone (name) ;
//           shape_bkg->Scale (1. / shape_bkg->Integral ()) ;
//           shape_bkg->SetFillColor (8) ;
          
          THStack * hstack_bkg_norm = normaliseStack (hstack_bkg.at (iv+nVars*isel)) ;
          TH1F * shape_bkg = (TH1F *) hstack_bkg_norm->GetStack ()->Last () ;
          
          THStack * hstack_sig_norm = normaliseStack (hstack_sig.at (iv+nVars*isel)) ;
          TH1F * shape_sig = (TH1F *) hstack_sig_norm->GetStack ()->Last () ;
          
//           name = basename + "_sig_norm" ;
//           dummy = (TH1F *) hstack_sig.at (iv+nVars*isel)->GetStack ()->Last () ;
//           TH1F * shape_sig = (TH1F *) dummy->Clone (name) ;
//           shape_sig->Scale (1. / shape_sig->Integral ()) ;
          
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
  for (unsigned int i = 0 ; i < NSpacesColZero ; ++i) cout << " " ;
  cout << "| " ;
  for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
    {
      string word = string (allSamples.at (iSample).sampleName.Data ()).substr (0, NSpacesColumns) ;
      cout << word ;
      for (unsigned int i = 0 ; i < NSpacesColumns - word.size () ; ++i) cout << " " ;
      cout << " | " ;
    }
  cout << "\n" ; 

  for (unsigned int i = 0 ; i < NSpacesColZero ; ++i) cout << "-" ;
  cout << "+-" ;
  for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
    {
      for (unsigned int i = 0 ; i < NSpacesColumns ; ++i) cout << "-" ;
      cout << "-+-" ;
    }
  cout << "\n" ; 

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
  for (unsigned int i = 0 ; i < NSpacesColZero ; ++i) cout << " " ;
  cout << "| " ;
  for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
    {
      string word = string (allSamples.at (iSample).sampleName.Data ()).substr (0, NSpacesColumns) ;
      cout << word ;
      for (unsigned int i = 0 ; i < NSpacesColumns - word.size () ; ++i) cout << " " ;
      cout << " | " ;
    }
  cout << "\n" ; 

  for (unsigned int i = 0 ; i < NSpacesColZero ; ++i) cout << "-" ;
  cout << "+-" ;
  for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
    {
      for (unsigned int i = 0 ; i < NSpacesColumns ; ++i) cout << "-" ;
      cout << "-+-" ;
    }
  cout << "\n" ; 

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

  for (unsigned int i = 0 ; i < NSpacesColZero ; ++i) cout << "-" ;
  cout << "+-" ;
  for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
    {
      for (unsigned int i = 0 ; i < NSpacesColumns ; ++i) cout << "-" ;
      cout << "-+-" ;
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
  for (unsigned int i = 0 ; i < NSpacesColZero ; ++i) cout << " " ;
  cout << "| " ;
  for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
    {
      string word = string (allSamples.at (iSample).sampleName.Data ()).substr (0, NSpacesColumns) ;
      cout << word ;
      for (unsigned int i = 0 ; i < NSpacesColumns - word.size () ; ++i) cout << " " ;
      cout << " | " ;
    }
  cout << "\n" ; 

  for (unsigned int i = 0 ; i < NSpacesColZero ; ++i) cout << "-" ;
  cout << "+-" ;
  for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
    {
      for (unsigned int i = 0 ; i < NSpacesColumns ; ++i) cout << "-" ;
      cout << "-+-" ;
    }
  cout << "\n" ; 

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

  for (unsigned int i = 0 ; i < NSpacesColZero ; ++i) cout << " " ;
  cout << "| " ;
  for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
    {
      string word = string (allSamples.at (iSample).sampleName.Data ()).substr (0, NSpacesColumns) ;
      cout << word ;
      for (unsigned int i = 0 ; i < NSpacesColumns - word.size () ; ++i) cout << " " ;
      cout << " | " ;
    }
  cout << "\n" ; 

  for (unsigned int i = 0 ; i < NSpacesColZero ; ++i) cout << "-" ;
  cout << "+-" ;
  for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
    {
      for (unsigned int i = 0 ; i < NSpacesColumns ; ++i) cout << "-" ;
      cout << "-+-" ;
    }
  cout << "\n" ; 

  name = "total" ;
  cout << name ;
  for (unsigned int i = 0 ; i < NSpacesColZero - name.size () ; ++i) cout << " " ;
  cout << "|" ;
  for (unsigned int iSample = 0 ; iSample < allSamples.size () ; ++iSample)
    {
      float value = allSamples.at (iSample).eff_den * lumi ;
      for (unsigned int i = 0 ; i < NSpacesColumns - int (log10 (value)) ; ++i) cout << " " ;
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
      for (unsigned int i = 0 ; i < NSpacesColumns - int (log10 (value)) ; ++i) cout << " " ;
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
      for (unsigned int i = 0 ; i < NSpacesColumns - int (log10 (value)) - precision - 1 ; ++i) cout << " " ;
      cout << setprecision (2) << fixed << value << " |" ;
    }
  cout << "\n" ; 

// std::cout << std::setprecision(5) << f << '\n';
}
