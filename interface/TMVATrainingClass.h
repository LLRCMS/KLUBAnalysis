/*
originally from here:
https://github.com/govoni/FlatNtStudy/blob/master/interface/TMVATrainingClass.h
*/

#ifndef TMVATrainingClass_h
#define TMVATrainingClass_h

#include <iostream> 
#include <cstdlib> 
#include <string> 
#include <map> 
#include <vector> 
#include <sstream> 
#include <algorithm> 
#include <memory> 

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TDirectory.h"
#include "TClass.h"
#include "TObject.h"
#include "TKey.h"

// #include "ReadInputFile.h"
#include "utils.h"
// #include "readTree.h"
// #include "fakeBackgroundUtils.h"

#if not defined (__CINT__) || defined (__MAKECINT__)
#include "TMVA/MsgLogger.h"
#include "TMVA/Config.h"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"
#endif

using namespace std ;

class TMVATrainingClass {

 public :

  // default constructor
  TMVATrainingClass (){} ;

  // constructor
  TMVATrainingClass (
            const vector<sample>& signalChainList,     // signal tree list
            const vector<sample>& backgroundChainList, // background tree list
            const string & TreeName,                   // tree name
            const string & outputFilePath ,            // output path for storing TMVA files
            const string & outputFileName,             // output file name
            const string & Label,                      // label
            const string & transformation = "") ;      // transformation to be applied on the input variables

  // default de-constructor
  ~TMVATrainingClass () ;

  // Set global event weight for signal and background
  // FIXME TBC
  void BookMVATrees () ;

  // Set Training and Spectator Variables
  void AddTrainingVariables (
      const vector<string> & trainingVariables,   // input list of training variables
      const vector<string> & spectatorVariables // input list of spectator ones 
    ) ;           
                              
  // prepare events for training
  void AddPrepareTraining (
      string weightStringSignal,     // re-weighting expression or branch for signal events 
      string weightStringBackground, // re-weighting expression or branch for background events
      const TCut & preselections,
      const int    & nTraining        = 0, // number of events used for training
      const int    & nTesting         = 0, // number of events used for testing
      const string & splitMode        = "Random",  // split mode
      const string & NormMode         = "NumEvents"
    ) ; // normalization mode

  // Train rectangular cut methods
  void BookandTrainRectangularCuts    ( const string & FitMethod) ;
  // Train Likelihood
  void BookandTrainLikelihood         ( const string & LikelihoodType = "Likelihood") ; // Likelihood training
  // Train Linear Discriminant
  void BookandTrainLinearDiscriminant () ; // linear discriminant
  // Train Fisher Discriminant
  void BookandTrainFisherDiscriminant () ; // fisher discriminant

  // Train MLP
  void BookandTrainMLP ( const int & nCycles            = 500, 
                         const string & HiddenLayers    = "N+5", 
                         const string & NeuronType      = "tanh", 
                         const string & TrainingMaethod = "BP", 
                         const int & TestRate           = 10, 
                         const int & ConvergenceTests   = 10) ;

  // Train Clemont Ferrand ANN
  void BookandTrainCFMlpANN           ( const int & nCycles         = 1000, 
                                        const string & HiddenLayers = "N+5") ;

  // Train TMVA ANN
  void BookandTrainTMlpANN            ( const int & nCycles              = 500, 
                                        const string & HiddenLayers      = "N+5", 
                                        const string & TrainingMethod    = "BFGS", 
                                        const float & ValidationFraction = 0.3) ;

  // Train BDT
  void BookandTrainBDT                ( const int & NTrees            = 400, 
                                        const bool & optimizeMethod   = false, 
                                        const string & BoostType      = "AdaBoost", 
                                        const float & AdaBoostBeta    = 0.5, 
                                        const string & PruneMethod    = "NoPruning", 
                                        const int & PruneStrength     = 5, 
                                        const int & MaxDepth          = 5, 
                                        const string & SeparationType = "GiniIndex") ;

  // Train Gradient BDT
  void BookandTrainBDTG               ( const int & NTrees                 = 300, 
                                        const bool & optimizeMethod        = false, 
                                        const float & GradBaggingFraction  = 0.5, 
                                        const string & PruneMethod    = "NoPruning", 
                                        const int & PruneStrength          = 5, 
                                        const int & MaxDepth               = 5, 
                                        const string & SeparationType = "GiniIndex") ;

  // Train Mit-Fisher BDT
  void BookandTrainBDTF               ( const int & NTrees            = 300, 
                                        const bool & optimizeMethod   = false, 
                                        const string & BoostType = "AdaBoost", 
                                        const float & AdaBoostBeta    = 0.5, 
                                        const string & PruneMethod = "CostComplexity", 
                                        const int & PruneStrength     = 5, 
                                        const int & MaxDepth          = 5, 
                                        const string & SeparationType = "GiniIndex") ;

  void ReadInputCollection (vector<TChain*> & localList, const vector<sample> & inputList) ;  

  // Set the training variables name
  void SetTrainingVariables  (const vector<string > & trainingVariables) ;

  // Set the spectator variables name
  void SetSpectatorVariables (const vector<string > & spectatorVariables) ;

  // Set the output file Name
  void SetOutputFile ( const string & outputFilePath , 
                       const string & outputFileName ) ;

  // Set the name of the tree
  void SetTreeName ( const string & TreeName ) ;

  // Set Label for the output file
  void SetLabel ( const string & Label ) ;

  // Set Event re-weight : pile-Up, efficiency, cps, interference, btag .. etc
  void SetEventWeight ( const string & weightStringSignal, 
                        const string & weightStringBackground) ;

  //Set transformation to be applied on variables. By default no transformation
  void SetTransformations (const string & transformations = "") ;

 private : 

  // list of trees for signal and background
  vector<TChain* > signalTreeList_ ;
  vector<TChain* > backgroundTreeList_ ;

  // list of input and spectator variables
  vector<string> trainingVariables_ ;
  vector<string> spectatorVariables_ ;

  // TreeName
  string TreeName_ ;

  // Label
  string Label_ ;

  //Transformations
  string transformations_ ;

  // outputFilePath
  vector<string> outputFilePath_ ;

  // output Name
  vector<string> outputFileName_ ;

  // output Complete Name = path + Name
  vector<string> outputFileNameComplete_ ;

  // Name of the final file xml with the weights
  map<string, string> outputFileWeightName_ ;

  // output file
  vector<shared_ptr<TFile> > outputFile_ ;

  // factory object
  TMVA::Factory * factory_ ; 

} ;

#endif
