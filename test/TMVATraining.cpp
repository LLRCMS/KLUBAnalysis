/*
originally from here:
https://github.com/govoni/FlatNtStudy/blob/master/test/TMVATrainingSelections.cpp
*/

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

#include "ConfigParser.h"
#include "utils.h"
#include "TMVATrainingClass.h"

using namespace std ;


TFile * SetOutputFile (
    string Label,
    string outputFilePath, 
    string outputFileName 
  )
{
  string outputFileNameComplete ;
  if (Label != "") 
    outputFileNameComplete = outputFilePath+"/"+outputFileName+""+Label+".root" ;
  else
    outputFileNameComplete = outputFilePath+"/"+outputFileName+".root" ;

  TFile * outputFile = new TFile (outputFileNameComplete.c_str (), "recreate") ;   
  outputFile->cd () ;
  return outputFile ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

int main (int argc, char** argv)
{

  if (argc < 2)
    {
      cerr << ">>> Usage: " << argv[1] << " cfg file" <<  endl ;
      exit (1) ;
    }

  // parse config file parameter                                                                                                                                            
  if (gConfigParser) return 1 ;
  gConfigParser = new ConfigParser () ;

  TString config ;
  config.Form ("%s",argv[1]) ;

  if (! (gConfigParser->init (config))){
    cout << ">>> parseConfigFile::Could not open configuration file " << config << endl ;
    return -1 ;
  }
  
  TMVA::Tools::Instance () ;

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
      gConfigParser->readStringOption ("selections::preSelectionsFile")
    ) ;

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << "selections sequence: \n" ;
  TCut preselections ; 
  for (unsigned int i = 0 ; i < selections.size () ; ++i)
    {
      cout << selections.at (i).first << " : " << selections.at (i).second << endl ;
      if (selections.at (i).first == "presel")
        {
          preselections = selections.at (i).second ;
          cout << "  `---> choosing this one as preselections\n" ;
        }
    }

  // treeName                                                                                                                                                               
  string treeName = "HTauTauTree" ;

  // get the variables to be plotted
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<string> variablesList  = gConfigParser->readStringListOption ("tmva::variables") ;
  vector<string> spectatorsList ;
  
  if (gConfigParser->readBoolOption ("tmva::spectatorsPresent")) 
    spectatorsList = gConfigParser->readStringListOption ("tmva::spectators") ;

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << "variables list: \n" ;
  for (unsigned int i = 0 ; i < variablesList.size () ; ++i)
    cout << variablesList.at (i) << endl ;
  cout << "spectators list: \n" ;
  for (unsigned int i = 0 ; i < spectatorsList.size () ; ++i)
    cout << spectatorsList.at (i) << endl ;

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
 
  //take input variable transformation
  string variableTransformation = gConfigParser->readStringOption ("tmva::variabletransformation") ;
  replace (variableTransformation.begin (), variableTransformation.end (), ':', ';') ;

  // take the method to be trained
  vector<string> TrainMVAMethodName = gConfigParser->readStringListOption ("tmva::trainmodes") ;

  // label in order to name the output file
  string outputLabel = gConfigParser->readStringOption ("tmva::outputlabel") ;

//  float lumi = gConfigParser->readFloatOption ("general::lumi") ; // pb^ (-1)                                                                                            

  string eventWeight  = gConfigParser->readStringOption ("tmva::eventWeight") ;

  // output directory                                                                                                                                                       
  string outputFileDirectory = gConfigParser->readStringOption ("tmva::outputFileDirectory") ;
  outputFileDirectory = "output/" + outputFileDirectory ;
  system ( ("mkdir -p " + outputFileDirectory).c_str ()) ;

  string outputFileName = gConfigParser->readStringOption ("tmva::outputFileName") ; 
  TFile * outputFile ;
  outputFile = SetOutputFile (outputLabel, outputFileDirectory, outputFileName) ;

  TMVA::Factory * factory = new TMVA::Factory (
      treeName + "_" + outputLabel, 
      outputFile,
      Form (
          "!V:!Silent:%sColor:DrawProgressBar:AnalysisType=Classification%s", 
          gROOT->IsBatch ()?"!":"", 
          (":Transformations=" + variableTransformation).c_str ()
        )
    ) ;

  TMVATrainingClass * trainer = new TMVATrainingClass
      (factory, outputFile, outputFileDirectory, sigSamples, bkgSamples, outputLabel) ;

  // book the TMVA training class
//   TMVATrainingClass * trainer = new TMVATrainingClass
//     (
//       sigSamples,
//       bkgSamples,
//       treeName, 
//       outputFileDirectory, 
//       outputFileName,
//       outputLabel,
//       ":Transformations=" + variableTransformation
//     ) ;

  trainer->AddTrainingVariables (variablesList, spectatorsList) ;
  trainer->AddPrepareTraining (eventWeight, eventWeight, preselections) ;
  trainer->BookMVATrees () ;  

  // loop on training methods
  for (size_t iMethod = 0 ; iMethod < TrainMVAMethodName.size () ; ++iMethod)
    {
      // Rectangular Cuts
      if (TrainMVAMethodName.at (iMethod) == "CutsMC" )      
        trainer->BookandTrainRectangularCuts ("MC") ;
      else if (TrainMVAMethodName.at (iMethod) == "CutsGA" ) 
        trainer->BookandTrainRectangularCuts ("GA") ;
      else if (TrainMVAMethodName.at (iMethod) == "CutsSA" ) 
        trainer->BookandTrainRectangularCuts ("SA") ;
      // Likelihoods
      else if (TrainMVAMethodName.at (iMethod) == "Likelihood")     
        trainer->BookandTrainLikelihood () ; 
      else if (TrainMVAMethodName.at (iMethod) == "LikelihoodKDE")  
        trainer->BookandTrainLikelihood ("LikelihoodKDE") ; 
      else if (TrainMVAMethodName.at (iMethod) == "PDERS")          
        trainer->BookandTrainLikelihood ("PDERS") ; 
      else if (TrainMVAMethodName.at (iMethod) == "PDEFoam")        
        trainer->BookandTrainLikelihood ("PDEFoam") ; 
      else if (TrainMVAMethodName.at (iMethod) == "PDEFoamBoost")   
        trainer->BookandTrainLikelihood ("PDEFoamBoost") ;         
      // Fisher Discriminant
      else if (TrainMVAMethodName.at (iMethod) == "Fisher")  
        trainer->BookandTrainFisherDiscriminant () ; 
      // Linear Discriminant
      else if (TrainMVAMethodName.at (iMethod) == "LD")      
        trainer->BookandTrainLinearDiscriminant () ;
      // MLP
      else if (TrainMVAMethodName.at (iMethod) == "MLP")        
        trainer->BookandTrainMLP () ;
      else if (TrainMVAMethodName.at (iMethod) == "MLPBFG")     
        trainer->BookandTrainMLP (1000,"N+5","sigmoid","BFGS",10,10) ;
      else if (TrainMVAMethodName.at (iMethod) == "CFMlpANN")   
        trainer->BookandTrainCFMlpANN () ;
      else if (TrainMVAMethodName.at (iMethod) == "TMlpANN")    
        trainer->BookandTrainTMlpANN () ;
      // BDT
      else if (TrainMVAMethodName.at (iMethod) == "BDT")     
        trainer->BookandTrainBDT () ;
      // BDTG
      else if (TrainMVAMethodName.at (iMethod) == "BDTG")    
        trainer->BookandTrainBDTG () ;
      // BDTF
      else if (TrainMVAMethodName.at (iMethod) == "BDTF")    
        trainer->BookandTrainBDTF () ;
      else 
        { 
          cerr << "Training Method not implemented in the TMVATrainingClass --> Go to the next one" << endl ; 
          cout << endl ;
        }
    } // loop on training methods

  cerr << "EVERITHING IS FINISHED HERE" << endl ;

  return 0 ;
}
