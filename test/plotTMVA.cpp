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
#include "ConfigParser.h"
#include "utils.h"

#include "TMVA/MsgLogger.h"
#include "TMVA/Config.h"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"

using namespace std ;


void
calcTMVA (sample & thisSample, vector<string> & trainingVariables, 
          string mvaName, string weightsFile)
{

  TMVA::Reader * reader = new TMVA::Reader () ;

  TTree *tree = (TTree*) thisSample.sampleChain->GetTree () ;
  vector<float> address (trainingVariables.size (), 0.) ; 
  for (unsigned int iv = 0 ; iv < trainingVariables.size () ; ++iv)
    {
      tree->SetBranchAddress (trainingVariables.at (iv).c_str (), &(address.at (iv))) ;
      reader->AddVariable (trainingVariables.at (iv), &(address.at (iv))) ;
    }  

  reader->BookMVA (mvaName.c_str (),  weightsFile.c_str ()) ;

  for (int iEvent = 0 ; iEvent < tree->GetEntries () ; ++iEvent)
    {
      tree->GetEntry (iEvent) ;
      reader->EvaluateMVA (mvaName.c_str ()) ;      
      
    } //loop on tree entries

  delete reader ;
  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void
calcTMVA (vector<sample> & samples,
           vector<string> & trainingVariables,
           string mvaName, string weightsFile)
{

  // loop on sim samples
  for (unsigned int j = 0 ; j < samples.size () ; ++j) 
    calcTMVA (samples, trainingVariables, mvaName, weightsFile) ;

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

  // get the variables to be cosidered in the training
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<string> trainingVariables  = gConfigParser->readStringListOption ("tmva::variables") ;
  
  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << "variables list: \n" ;
  for (unsigned int i = 0 ; i < trainingVariables.size () ; ++i)
    cout << trainingVariables.at (i) << endl ;

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;


  // get the variables to be plotted
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<string> variablesList = gConfigParser->readStringListOption ("general::variables") ;

  TString histoName ;
  HistoManager * manager = new HistoManager ("test") ;

  // read the MVA
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  calcTMVA (sigSamples, trainingVariables, "MVA", "pesi") ;

  return 0 ;

}