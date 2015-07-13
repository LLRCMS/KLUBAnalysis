#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include "ConfigParser.h"
#include "utils.h"
#include "TString.h"
#include "TChain.h"
#include "TCut.h"
#include "HistoManager.h"
#include "TTreeFormula.h"
#include "THStack.h"
#include "TCanvas.h"

using namespace std ;

//Replace histomanager with something that goes by index instead og looking for strings

//void FillHistos (int) ;
//TString Selection (int) ;

//TString Selection (int selection=0){
//  if (selection==0) return "HH_pt>0" ;
//  else if (selection==1)return "HH_pt>400" ;
//  return "" ;
//}

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

  vector<string> bkgSamplesList = gConfigParser->readStringListOption ("general::backgrounds") ;
  vector<sample> bkgSamples ;
  readSamples (bkgSamples, bkgSamplesList) ;

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

// come mai e' duplicato questo?
  vector<sample> allSamples ;  
  for (int i=0 ;i<nB ;i++) allSamples.push_back (bkgSamples.at (i)) ;
  for (int i=0 ;i<nS ;i++) allSamples.push_back (sigSamples.at (i)) ;
  
  int xup[nVars] ;//           = {900,900} ;
  int xlow[nVars] ;//          = {0,250} ;
  int bins[nVars] ;//          = {300,100} ;
  for (int iVar = 0 ; iVar < nVars ; ++iVar)
    {
      xup[iVar]=-999 ;xlow[iVar]=0 ;bins[iVar]=100 ;
      for (int i = 0 ; i < nS ; ++i)
        {
          if (sigSamples.at (i).sampleChain->GetMaximum (variablesList.at (iVar).c_str ()) > xup[iVar]) 
            xup[iVar] = sigSamples.at (i).sampleChain->GetMaximum (variablesList.at (iVar).c_str ()) ;
          if (sigSamples.at (i).sampleChain->GetMinimum (variablesList.at (iVar).c_str ()) < xlow[iVar])
            xlow[iVar] = sigSamples.at (i).sampleChain->GetMinimum (variablesList.at (iVar).c_str ()) ;
        }
      for (int i = 0 ; i < nB ; ++i)
        {
          if (bkgSamples.at (i).sampleChain->GetMaximum (variablesList.at (iVar).c_str ()) > xup[iVar])
            xup[iVar] = bkgSamples.at (i).sampleChain->GetMaximum (variablesList.at (iVar).c_str ()) ;
          if (bkgSamples.at (i).sampleChain->GetMinimum (variablesList.at (iVar).c_str ()) < xlow[iVar])
            xlow[iVar] = bkgSamples.at (i).sampleChain->GetMinimum (variablesList.at (iVar).c_str ()) ;
        }
    }

  // FIXME PG are they in agreement with the ones of the HTT group?
  int colors[]={kBlue,kRed,kGreen,kYellow+2,kRed+2,kMagenta,kCyan,kBlack} ;//add more if needed

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
              manager->AddNewHisto (histoName.Data (),histoName.Data (),
                                    bins[i], xlow[i], xup[i],
                                    colors[j], (j >= nB)) ;
            }  
        }
    }

  // Fill the histograms
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<float> address (nVars, 0.) ; 

  //Loop on the samples
  for (int iSample = 0 ; iSample < nB+nS ; ++iSample)
    {
      double eff = allSamples.at (iSample).eff ;
      initEfficiencies.push_back (eff) ;
      TTree *tree = (TTree*) allSamples.at (iSample).sampleChain->GetTree () ;
      TTreeFormula * TTF[nSel] ;
      for (int isel = 0 ; isel < nSel ; ++isel)
        {
          TString fname ; fname.Form ("ttf%d",isel) ;
          TTF[isel] = new TTreeFormula (fname.Data (), selections.at (isel).second, tree) ;
        }
      float weight ;
      tree->SetBranchAddress ("MC_weight", &weight) ;

      cout << "Opening sample "
           << allSamples.at (iSample).sampleName
           << " with efficiency " << eff
           << endl ;

      for (int iv = 0 ; iv < nVars ; ++iv)
        tree->SetBranchAddress (variablesList.at (iv).c_str (), &(address.at (iv))) ;
    
      for (int iEvent = 0 ; iEvent < tree->GetEntries () ; ++iEvent)
        {
          tree->GetEntry (iEvent) ;
          counters.at (iSample).at (0) += weight * lumi * eff ;
          for (int isel = 0 ; isel < nSel ; ++isel)
            {
              if (! TTF[isel]->EvalInstance ()) continue ;
              counters.at (iSample).at (isel + 1) += weight * lumi * eff ;
              for (int iv = 0 ; iv < nVars ; ++iv)
                {
                  histoName.Form ("%s_%s_%s",
                      variablesList.at (iv).c_str (),
                      allSamples.at (iSample).sampleName.Data (),
                      selections.at (isel).first.Data ()
                    ) ;
                  manager->GetHisto (histoName.Data ())->Fill (address[iv], weight * eff * lumi) ;
                } //loop on variables
            } //loop on selections
        } //loop on tree entries

      for (int isel = 0 ; isel < nSel ; ++isel) delete TTF[isel] ;
    } // Loop on the samples

  // Plot the histograms
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  TString outString ;outString.Form ("outPlotter.root") ;
  TFile *fOut = new TFile (outString.Data (),"RECREATE") ;
  manager->SaveAllToFile (fOut) ;

  //make Stack plots
  THStack *hstack[nVars*nSel] ;//one stack for variable
  for (int isel = 0 ; isel < nSel ; ++isel)
    {
      for (int iv = 0 ; iv < nVars ; ++iv)
        {
          TString outputName ; 
          outputName.Form ("stack_%s_%s",
            variablesList.at (iv).c_str (), selections.at (isel).first.Data ()) ;
          hstack[iv+nVars*isel] = new THStack (outputName.Data (),outputName.Data ()) ;
          for (int i = 0 ; i < nB ; ++i)
            {
              histoName.Form ("%s_%s_%s",
                  variablesList.at (iv).c_str (),
                  allSamples.at (i).sampleName.Data (),
                  selections.at (isel).first.Data ()
                ) ;
              hstack[iv+nVars*isel]->Add (manager->GetHisto (histoName.Data ())) ;
            }
          TCanvas * c = new TCanvas (outputName.Data ()) ;
          c->cd () ;
          hstack[iv+nVars*isel]->Draw ("hist") ;
          
          //superimpose the signals
          for (int i = nB ; i< nB+nS ; ++i)
            {
               histoName.Form ("%s_%s_%s",
                   variablesList.at (iv).c_str (),
                   allSamples.at (i).sampleName.Data (),
                   selections.at (isel).first.Data ()
                 ) ;
               manager->GetHisto (histoName.Data ())->Draw ("EPSAME") ;
            }
          TString coutputName ;
          coutputName.Form ("%s.pdf",outputName.Data ()) ;
          c->SaveAs (coutputName.Data ()) ;
          fOut->cd () ;
          hstack[iv+nVars*isel]->Write () ;
          delete c ;
        }
    } //make Stack plots

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
      for (int i = 0 ; i < NSpacesColumns - int (log10 (value)) ; ++i) cout << " " ;
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
      for (int i = 0 ; i < NSpacesColumns - int (log10 (value)) ; ++i) cout << " " ;
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
      for (int i = 0 ; i < NSpacesColumns - int (log10 (value)) - precision - 1 ; ++i) cout << " " ;
      cout << setprecision (2) << fixed << value << " |" ;
    }
  cout << "\n" ; 

// std::cout << std::setprecision(5) << f << '\n';
}
