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
calcTMVA (sample & thisSample, 
          vector<string> & trainingVariables, 
          vector<string> & spectatorVariables, 
          string mvaName, string weightsFile)
{

  TMVA::Reader * reader = new TMVA::Reader () ;

  TString outfile = "TMVA_";
  outfile.Append(mvaName);
  outfile.Append("_") ;
  outfile.Append(thisSample.sampleName);
  outfile.Append(".root");

  TFile * outputFile = TFile::Open(outfile.Data(),"RECREATE");
  TTree * tree = thisSample.sampleTree ;
  //TFile * currentFile = thisSample.sampleFile ;

  vector<float> address (trainingVariables.size () + spectatorVariables.size (), 0.) ; 
  for (unsigned int iv = 0 ; iv < trainingVariables.size () ; ++iv)
    {
      tree->SetBranchAddress (trainingVariables.at (iv).c_str (), &(address.at (iv))) ;
      reader->AddVariable (trainingVariables.at (iv), &(address.at (iv))) ;
    }  

  for (unsigned int iv = 0 ; iv < spectatorVariables.size () ; ++iv)
    {
      int addressIndex = iv + trainingVariables.size () ;
      tree->SetBranchAddress (spectatorVariables.at (iv).c_str (), &(address.at (addressIndex))) ;
      reader->AddSpectator (spectatorVariables.at (iv), &(address.at (addressIndex))) ;
    }  

  // add a new branch to store the tmva output
  float mvaValue ;
  TBranch * mvaBranch ;
  
  if (tree->GetListOfBranches ()->FindObject (mvaName.c_str ())) 
    tree->SetBranchAddress (mvaName.c_str (), &mvaValue, &mvaBranch) ;
  else   
    mvaBranch = tree->Branch (mvaName.c_str (), &mvaValue, (mvaName + "/F").c_str ()) ;

  reader->BookMVA (mvaName.c_str (),  weightsFile.c_str ()) ;

  for (int iEvent = 0 ; iEvent < tree->GetEntries () ; ++iEvent)
    {
      tree->GetEntry (iEvent) ;
      mvaValue = reader->EvaluateMVA (mvaName.c_str ()) ;  
      mvaBranch->Fill () ;    
    } //loop on tree entries

  // overwrite the tree including the new branch
  outputFile->cd () ;
  tree->Write ();//"", TObject::kOverwrite) ;
  outputFile->Close("R");
  delete reader ;
  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void
calcTMVA (vector<sample> & samples,
          vector<string> & trainingVariables,
          vector<string> & spectatorVariables,            
          string mvaName, string weightsFile)
{
  // loop on sim samples
  for (unsigned int j = 0 ; j < samples.size () ; ++j) {
    cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
    cout << "processing sample: " << j << "\n" ;
    calcTMVA (samples.at (j), trainingVariables, spectatorVariables, mvaName, weightsFile) ;
    cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
    cout << "processed " << "\n" ;

  }
  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


int main (int argc, char** argv)
{
  // check number of inpt parameters
  if (argc < 4)
    {
      cerr << ">>> Usage: " << argv[0] << " cfg_file TMVA_name weights_file" <<  endl ;
      exit (1) ;
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

  string MVAname = argv[2] ;

  // get the samples to be analised
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  vector<sample> samples ;
  vector<string> samplesList = gConfigParser->readStringListOption ("general::signals") ;
  readSamples (samples, samplesList, "READ") ;
  samplesList.clear () ;
  samplesList = gConfigParser->readStringListOption ("general::backgrounds") ;
  readSamples (samples, samplesList, "READ") ;
  samplesList.clear () ;
  samplesList = gConfigParser->readStringListOption ("general::data") ;
  readSamples (samples, samplesList, "READ") ;

  // get the variables to be cosidered in the training
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<string> trainingVariables  = gConfigParser->readStringListOption (
       (MVAname + "::variables").c_str ()) ;
  vector<string> spectatorVariables ;
  if (gConfigParser->readBoolOption (
    (MVAname + "::spectatorsPresent").c_str ())) 
    spectatorVariables = gConfigParser->readStringListOption (
        (MVAname + "::spectators").c_str ()) ;

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << "---> variables list: \n" ;
  for (unsigned int i = 0 ; i < trainingVariables.size () ; ++i)
    cout << trainingVariables.at (i) << endl ;
  cout << "---> spectators list: \n" ;
  for (unsigned int i = 0 ; i < spectatorVariables.size () ; ++i)
    cout << spectatorVariables.at (i) << endl ;

  // get the weights file
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  string mvaWeightsFile = argv[3] ;
  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << "weights file: " << mvaWeightsFile << "\n" ;

  // read the MVA
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  calcTMVA (samples, trainingVariables, spectatorVariables, MVAname, mvaWeightsFile) ;

  return 0 ;

}

