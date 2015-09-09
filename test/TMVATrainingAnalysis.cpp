#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TChainElement.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"
#include "TMath.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TNtuple.h"

#include "ConfigParser.h"
#include "utils.h"

#include "TMVA/MsgLogger.h"
#include "TMVA/Config.h"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"

using namespace std ;


int main (int argc, char** argv)
{

  if (argc < 3)
    {
      cerr << ">>> Usage: " << argv[0] << " cfg_file TMVA_name" <<  endl ;
      exit (1) ;
    }

  // parse config file parameter                                                                                                                                            
  if (gConfigParser) return 1 ;
  gConfigParser = new ConfigParser () ;

  TString config ;
  config.Form ("%s",argv[1]) ;

  string MVAname = argv[2] ;

  if (! (gConfigParser->init (config))){
    cout << ">>> parseConfigFile::Could not open configuration file " << config << endl ;
    return -1 ;
  }

  // prepare the output folder
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TString outFolderNameBase = gConfigParser->readStringOption ("general::outputFolderName") ;
  outFolderNameBase += "/" ;
  system (TString ("mkdir -p ") + outFolderNameBase) ;
  outFolderNameBase += MVAname.c_str () ;
  outFolderNameBase += "/" ;  
  system (TString ("mkdir -p ") + outFolderNameBase) ;
  
  
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
  TCut preselections ; 
  TString selectionsName = gConfigParser->readStringOption (
       (MVAname + "::selections").c_str ()) ;
  for (unsigned int i = 0 ; i < selections.size () ; ++i)
    {
      cout << selections.at (i).first << " : " << selections.at (i).second << endl ;
      if (selections.at (i).first == selectionsName)
        {
          preselections = selections.at (i).second ;
          cout << "  `---> choosing this one as preselections\n" ;
        }
    }

  // treeName                                                                                                                                                               
  string treeName = "HTauTauTree" ;

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
  cout << "variables list: \n" ;
  for (unsigned int i = 0 ; i < trainingVariables.size () ; ++i)
    cout << trainingVariables.at (i) << endl ;
  cout << "spectators list: \n" ;
  for (unsigned int i = 0 ; i < spectatorVariables.size () ; ++i)
    cout << spectatorVariables.at (i) << endl ;

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;

  //take input variable transformation
  string variableTransformation = gConfigParser->readStringOption (
       (MVAname + "::variabletransformation").c_str ()) ;
  replace (variableTransformation.begin (), variableTransformation.end (), ':', ';') ;

  string outputFileName = string (outFolderNameBase.Data ()) + gConfigParser->readStringOption (
       (MVAname + "::outputFileName").c_str ()) ; 

  TFile * outputfile = TFile::Open (outputFileName.c_str (),"RECREATE");

  TMVA::Factory * TMVAtest = new TMVA::Factory (
      treeName, 
      outputfile,
      Form (
          "!V:!Silent:%sColor:DrawProgressBar:AnalysisType=Classification%s", 
          gROOT->IsBatch ()?"!":"", 
          (":Transformations=" + variableTransformation).c_str ()
        )
    ) ;

  for (unsigned int iChain = 0 ; iChain < sigSamples.size () ; ++iChain)
    {
      cout << "adding sig sample " 
           << sigSamples.at (iChain).sampleName << endl ;
      TMVAtest->AddSignalTree (sigSamples.at (iChain).sampleTree, 1.) ;
    }
  for (unsigned int iChain = 0 ; iChain < bkgSamples.size () ; ++iChain)
    {
      cout << "adding bkg sample " 
           << bkgSamples.at (iChain).sampleName << endl ;
      TMVAtest->AddBackgroundTree (bkgSamples.at (iChain).sampleTree, 1.) ;
    }

  for ( size_t iVar = 0 ; iVar < trainingVariables.size () ; iVar ++ )
    TMVAtest->AddVariable (trainingVariables.at (iVar), 'F') ;

  for ( size_t iVar = 0 ; iVar < spectatorVariables.size () ; ++iVar)
    TMVAtest->AddSpectator (spectatorVariables.at (iVar), 'F') ;

  string eventWeight  = gConfigParser->readStringOption (
       (MVAname + "::eventWeight").c_str ()) ;
  TMVAtest->SetSignalWeightExpression (eventWeight) ;
  TMVAtest->SetBackgroundWeightExpression (eventWeight) ;

  char trainOptions[120] ;
  sprintf (trainOptions,"nTrain_Signal=%d:nTrain_Background=%d:nTest_Signal=%d:nTest_Background=%d:SplitMode=Random:NormMode=NumEvents:!V",
           0,0,0,0) ;
  TMVAtest->PrepareTrainingAndTestTree (preselections, preselections, trainOptions) ;
  
  // adding a BDT
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  int    NTrees         = 400 ; 
  bool   optimizeMethod = false ; 
  string BoostType      = "AdaBoost" ; 
  float  AdaBoostBeta   = 0.5 ; 
  string PruneMethod    = "NoPruning" ; 
  int    PruneStrength  = 5 ; 
  int    MaxDepth       = 5 ; 
  string SeparationType = "GiniIndex" ;

  TString Option = Form ("!H:!V:CreateMVAPdfs:NTrees=%d:BoostType=%s:AdaBoostBeta=%f:PruneMethod=%s:PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.1:MaxDepth=11:UseYesNoLeaf=F:MinNodeSize=2:nCuts=200", 
      NTrees, BoostType.c_str (), AdaBoostBeta, PruneMethod.c_str (), PruneStrength, MaxDepth, SeparationType.c_str ()) ;

  string BDTname = string ("BDT_") + MVAname ;
  TMVAtest->BookMethod ( TMVA::Types::kBDT, BDTname.c_str (), Option.Data ()) ;

  // adding a BDTG
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  float GradBaggingFraction = 0.5 ; 
  NTrees              = 300 ; 
  optimizeMethod      = false ; 
  PruneMethod         = "NoPruning" ; 
  PruneStrength       = 5 ; 
  MaxDepth            = 5 ; 
  SeparationType      = "GiniIndex" ;

  Option = Form ("CreateMVAPdfs:NTrees=%d:BoostType=Grad:!UseBaggedGrad:GradBaggingFraction=%f:PruneMethod=%s:PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.1:MaxDepth=11:UseYesNoLeaf=F:nCuts=2000",
      NTrees, GradBaggingFraction, PruneMethod.c_str (), PruneStrength, MaxDepth, SeparationType.c_str ()) ;

  string BDTGname = string ("BDTG_") + MVAname ;
  TMVAtest->BookMethod ( TMVA::Types::kBDT, BDTGname.c_str (), Option.Data ()) ;

  // start the training
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  TMVAtest->TrainAllMethods () ;
  TMVAtest->TestAllMethods () ;
  TMVAtest->EvaluateAllMethods () ;
 
  delete TMVAtest ;
  delete outputfile ;

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << "Name tag of the weights file: " << BDTname << "\n" ;  

  return 0 ;
}