/*
originally from here:
https://github.com/govoni/FlatNtStudy/blob/master/src/TMVATrainingClass.cc
*/

#include "TMVATrainingClass.h"
#include "utils.h"
#include "TSystem.h"


// constructor
TMVATrainingClass::TMVATrainingClass 
  (
    TMVA::Factory * factory,
    TFile * outputFile,
    string outputFilePath,
    const vector<sample> & signalTreeList, 
    const vector<sample> & backgroundTreeList,  
    const string & Label 
  ) :
  factory_ (factory),
  outputFile_ (outputFile),
  Label_ (Label),
  outputFilePath_ (outputFilePath)
{
//  SetTreeName          (TreeName) ;
  ReadInputCollection  (signalTreeList_,     signalTreeList) ;
  ReadInputCollection  (backgroundTreeList_, backgroundTreeList) ;

//  SetOutputFile        (outputFilePath, outputFileName) ;
  
//   factory_ = new TMVA::Factory (TreeName_ + "_" + Label_, 
//       outputFile_, 
//       Form (
//           "!V:!Silent:%sColor:DrawProgressBar:AnalysisType=Classification%s", 
//           gROOT->IsBatch ()?"!":"", 
//           transformation.c_str ()
//         )
//     ) ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Destructor
TMVATrainingClass::~TMVATrainingClass ()
{
  outputFile_->Close () ;
  delete factory_ ;
  delete outputFile_ ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

                   
void TMVATrainingClass::ReadInputCollection (
    vector<TTree *> & localList,
    const vector<sample> & inputList  
  )
{
  localList.clear () ;  
  for (unsigned int iChain = 0 ; iChain < inputList.size () ; ++iChain)
    {
      cout << "TMVATrainingClass::ReadInputCollection reading sample " 
           << inputList.at (iChain).sampleName << endl ;
      localList.push_back (inputList.at (iChain).sampleTree) ;
    }
  cout << "  `-->   DONE " << endl ;
  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// set output file
void 
TMVATrainingClass::SetOutputFile (
    const string & outputFilePath, 
    const string & outputFileName 
  )
{
  outputFilePath_ = outputFilePath ; 
  outputFileName_ = outputFileName ;

  replace (outputFileName_.begin (), outputFileName_.end (), '/', '_') ;
  replace (outputFileName_.begin (), outputFileName_.end (), '[', '_') ;
  replace (outputFileName_.begin (), outputFileName_.end (), ']', '_') ;
  replace (outputFileName_.begin (), outputFileName_.end (), '(', '_') ;
  replace (outputFileName_.begin (), outputFileName_.end (), ')', '_') ;
  replace (outputFileName_.begin (), outputFileName_.end (), ':', '_') ;

  if (Label_!="") 
    outputFileNameComplete_ = outputFilePath_+"/"+outputFileName_+"_"+Label_+".root" ;
  else
    outputFileNameComplete_ = outputFilePath_+"/"+outputFileName_+".root" ;

  cout << "TMVATrainingClass::SetOutputFile " << outputFileNameComplete_ << endl ;

  outputFile_ = new TFile ( outputFileNameComplete_.c_str (), "RECREATE") ;   
  outputFile_->cd () ;
  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// AddTrainingVariables in the MVA
void 
TMVATrainingClass::AddTrainingVariables 
  ( 
    const vector<string> & trainingVariables, 
    const vector<string> & spectatorVariables
  )
{
  SetTrainingVariables (trainingVariables) ;
  SetSpectatorVariables (spectatorVariables) ;

  for ( size_t iVar = 0 ; iVar < trainingVariables_.size () ; iVar ++ )
    {
      factory_->AddVariable (trainingVariables_.at (iVar)+" := "+trainingVariables_.at (iVar), 'F') ;
    }

  for ( size_t iVar = 0 ; iVar < spectatorVariables_.size () ; ++iVar)
    {
      factory_->AddSpectator (spectatorVariables_.at (iVar), 'F') ;
    }    
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Set Training Variables 
void 
TMVATrainingClass::SetTrainingVariables  (const vector<string> & trainingVariables){

   trainingVariables_.clear () ;
   if (trainingVariables.size ()!=0) 
     trainingVariables_ = trainingVariables ;
   return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Set Spectator Variables
void 
TMVATrainingClass::SetSpectatorVariables (const vector<string> & spectatorVariables)
{
  spectatorVariables_.clear () ;
  if (spectatorVariables.size ()!=0) 
    spectatorVariables_ = spectatorVariables ;
   return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Prepare the training adding the preselection cut and the train/testing composition
void 
TMVATrainingClass::AddPrepareTraining 
  (
    string  weightStringSignal, 
    string  weightStringBackground, 
    const TCut & preselections,
    const int & nTraining, 
    const int & nTesting, 
    const string & splitMode, 
    const string & NormMode
  )
{

  // declare the option for the training and set the weight expression for the signal 
  // and background samples 
  // --> weight expression can be a formula, not only a single branch 
  TString Option = Form ("nTrain_Signal=%d:nTrain_Background=%d:nTest_Signal=%d:nTest_Background=%d:SplitMode=%s:NormMode=%s:!V", 
                         nTraining, nTraining, nTesting, nTesting, splitMode.c_str (), NormMode.c_str ()) ;

  SetEventWeight (weightStringSignal, weightStringBackground) ; // set the event basis weight in the factory

  factory_->PrepareTrainingAndTestTree (
      preselections, preselections, Option.Data ()) ;  // set the options

}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Book MVA Training Variables 
void TMVATrainingClass::BookMVATrees ()
{
  for (size_t iNtuple = 0 ; iNtuple < signalTreeList_.size () ; iNtuple++)
    {
      cout << "TMVATrainingClass::BookMVATrees : add signal to the MVA factory " << iNtuple << endl ;
      factory_->AddSignalTree (signalTreeList_.at (iNtuple), 1.) ;
    }
  for (size_t iNtuple = 0 ; iNtuple < backgroundTreeList_.size () ; iNtuple++)
    {
      cout << "TMVATrainingClass::BookMVATrees : add background to the MVA factory " << iNtuple << endl ;
      factory_->AddBackgroundTree (backgroundTreeList_.at (iNtuple), 1.) ;
    }
  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void 
TMVATrainingClass::SetEventWeight
  (
    const string & weightStringSignal, 
    const string & weightStringBackground
  )
{
  factory_->RootBaseDir ()->cd () ;
  factory_->SetSignalWeightExpression (weightStringSignal) ;
  factory_->SetBackgroundWeightExpression (weightStringBackground) ;
    
  return ;

}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Train Rectangular Cuts
void TMVATrainingClass::BookandTrainRectangularCuts (const string & FitMethod)
{
  // create output directory if not there and change the variable name in case of a list
  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_ << endl ; 

  factory_->TrainAllMethods () ;
  factory_->TestAllMethods () ;
  factory_->EvaluateAllMethods () ;
  factory_->DeleteAllMethods () ;

  cout << "==> TMVAClassification is done!" << endl ;

}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Train Likelihood
void TMVATrainingClass::BookandTrainLikelihood ( const string & LikelihoodType )
{
  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_ << endl ; 

  // Set Name of the Weight file for TMVA evaluating procedure
  if (Label_ !=""){
    outputFileWeightName_[LikelihoodType+"_"+Label_] = outputFilePath_+"/TMVAWeight_"+LikelihoodType+"_"+Label_ ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_[LikelihoodType+"_"+Label_] ;
  }
  else {
    outputFileWeightName_[LikelihoodType] = outputFilePath_+"/TMVAWeight_"+LikelihoodType ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_[LikelihoodType] ;
  }

  TString Option ;

  outputFile_->cd () ;
  factory_->RootBaseDir ()->cd () ;

  if ( LikelihoodType == "LikelihoodKDE") { 
    Option = Form ("LikelihoodKDE") ;
    factory_->BookMethod (TMVA::Types::kLikelihood, Option.Data (), "!H:!V:IgnoreNegWeightsInTraining:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:"
                                                     "KDEiter=Adaptive:CreateMVAPdfs:KDEFineFactor=0.3:KDEborder=None") ;
  }
  else if ( LikelihoodType == "PDERS") { 
      Option = Form ("%s", LikelihoodType.c_str ()) ;
      factory_->BookMethod (TMVA::Types::kPDERS, Option.Data (), 
                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:CreateMVAPdfs:DeltaFrac=4:GaussSigma=0.3:NormTree=T") ;
  }
  else if ( LikelihoodType == "PDEFoam") { 
       Option = Form ("%s", LikelihoodType.c_str ()) ;
       factory_->BookMethod (TMVA::Types::kPDEFoam, Option.Data (), "!H:!V:CreateMVAPdfs:IgnoreNegWeightsInTraining:SigBgSeparate=F:TailCut=0.001"
                                                                 ":VolFrac=0.0666:nActiveCells=500:nSampl=2000:nBin=5:Nmin=100:Kernel=None:Compress=T") ;
  }
  else if ( LikelihoodType == "PDEFoamBoost") { 
      Option = Form ("%s", LikelihoodType.c_str ()) ;
      factory_->BookMethod (TMVA::Types::kPDEFoam, Option.Data (), 
                           "!H:!V:IgnoreNegWeightsInTraining:Boost_Num=30:CreateMVAPdfs:Boost_Transform=linear:SigBgSeparate=F:MaxDepth=4"
                           ":UseYesNoCell=T:DTLogic=MisClassificationError:FillFoamWithOrigWeights=F:TailCut=0:nActiveCells=300:nBin=20:Nmin=300:Kernel=None:Compress=T") ;
  }
  else{ Option = Form ("%s", LikelihoodType.c_str ()) ;
        factory_->BookMethod ( TMVA::Types::kLikelihood, Option.Data (), "!H:!V:!TransformOutput:CreateMVAPdfs:IgnoreNegWeightsInTraining:PDFInterpol=Spline2"
                                                          ":NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50") ;
  }

  factory_->TrainAllMethods () ;
  factory_->TestAllMethods () ;
  factory_->EvaluateAllMethods () ;
  factory_->DeleteAllMethods () ;

 cout << "==> TMVAClassification is done!" << endl ;

}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Train Fisher Discriminant
void TMVATrainingClass::BookandTrainFisherDiscriminant ()
{
  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_ << endl ; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                              
  if (Label_ !=""){
    outputFileWeightName_["Fisher_"+Label_] = outputFilePath_+"/TMVAWeight_Fisher_"+Label_ ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["Fisher_"+Label_] ;
  }
  else{
    outputFileWeightName_["Fisher"] = outputFilePath_+"/TMVAWeight_Fisher" ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["Fisher"] ;
  }

  outputFile_->cd () ;
  factory_->RootBaseDir ()->cd () ;
  factory_->BookMethod ( TMVA::Types::kFisher, "Fisher", 
                        "!H:!V:CreateMVAPdfs:IgnoreNegWeightsInTraining:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10:Fisher") ;


  factory_->TrainAllMethods () ;
  factory_->TestAllMethods () ;
  factory_->EvaluateAllMethods () ;
  factory_->DeleteAllMethods () ;

  cout << "==> TMVAClassification is done!" << endl ;

}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Train Linear Discriminant
void TMVATrainingClass::BookandTrainLinearDiscriminant (){

  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_ << endl ;
 
  // Set Name of the Weight file for TMVA evaluating procedure
  if (Label_ !=""){
    outputFileWeightName_["LD_"+Label_] = outputFilePath_+"/TMVAWeight_LD_"+Label_ ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["LD_"+Label_] ;
  }
  else {
    outputFileWeightName_["LD"] = outputFilePath_+"/TMVAWeight_LD" ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["LD"] ;
  }

  // Training Testing and Evaluating   
  outputFile_->cd () ;
  factory_->RootBaseDir ()->cd () ;

  factory_->BookMethod ( TMVA::Types::kLD, "LD", "H:!V:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10") ;

  factory_->TrainAllMethods () ;
  factory_->TestAllMethods () ;
  factory_->EvaluateAllMethods () ;
  factory_->DeleteAllMethods () ;

  cout << "==> TMVAClassification is done!" << endl ;

}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Train MLP
void TMVATrainingClass::BookandTrainMLP (const int & nCycles, 
                    const string & HiddenLayers, 
                    const string & NeuronType, 
                    const string & TrainingMethod, 
                    const int & TestRate, 
                    const int & ConvergenceTests){

  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_ << endl ; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                            
  if (Label_ != ""){
    outputFileWeightName_["MLP_"+Label_] = outputFilePath_+"/TMVAWeight_MLP_"+Label_ ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["MLP_"+Label_] ;
  }
  else{
    outputFileWeightName_["MLP"] = outputFilePath_+"/TMVAWeight_MLP" ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["MLP"] ;
  }

  TString Option = Form ("!H:!V:NCycles=%d:CalculateErrors:HiddenLayers=%s:NeuronType=%s:CreateMVAPdfs:TrainingMethod=%s:TestRate=%d"
             ":ConvergenceTests=%d:UseRegulator", nCycles, HiddenLayers.c_str (), NeuronType.c_str (), TrainingMethod.c_str (), TestRate, 
                         ConvergenceTests) ;

  outputFile_->cd () ;
  factory_->RootBaseDir ()->cd () ;

  factory_->BookMethod ( TMVA::Types::kMLP, "MLP", Option.Data ()) ;
  factory_->TrainAllMethods () ;
  factory_->TestAllMethods () ;
  factory_->EvaluateAllMethods () ;
  factory_->DeleteAllMethods () ;

  cout << "==> TMVAClassification is done!" << endl ;

}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Train Clemont Ferrand ANN
void TMVATrainingClass::BookandTrainCFMlpANN ( const int & nCycles, 
                           const string & HiddenLayers){

  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_ << endl ; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                            
  if (Label_ !=""){
    outputFileWeightName_["CFMlpANN_"+Label_] = outputFilePath_+"/TMVAWeight_CFMlpANN_"+Label_ ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["CFMlpANN_"+Label_] ;
  }
  else {
    outputFileWeightName_["CFMlpANN"] = outputFilePath_+"/TMVAWeight_CFMlpANN" ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["CFMlpANN"] ;
  }

  TString Option = Form ("!H:!V:NCycles=%d:HiddenLayers=%s:CreateMVAPdfs", nCycles, HiddenLayers.c_str ()) ;
 
  outputFile_->cd () ;
  factory_->RootBaseDir ()->cd () ;

  factory_->BookMethod ( TMVA::Types::kCFMlpANN, "CFMlpANN", Option.Data ()) ;

  factory_->TrainAllMethods () ;
  factory_->TestAllMethods () ;
  factory_->EvaluateAllMethods () ;
  factory_->DeleteAllMethods () ;

  cout << "==> TMVAClassification is done!" << endl ;

}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Train TMVA ANN
void TMVATrainingClass::BookandTrainTMlpANN  ( const int & nCycles, 
                           const string & HiddenLayers,  
                           const string & TrainingMethod, 
                           const float & ValidationFraction){

  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_ << endl ; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                        
  if (Label_ != ""){         
    outputFileWeightName_["TMlpANN_"+Label_] = outputFilePath_+"/TMVAWeight_TMlpANN_"+Label_ ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["TMlpANN_"+Label_] ;
  }
  else {
    outputFileWeightName_["TMlpANN"] = outputFilePath_+"/TMVAWeight_TMlpANN" ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["TMlpANN"] ;
  }

  TString Option = Form ("!H:!V:NCycles=%d:HiddenLayers=%s:LearningMethod=%s:ValidationFraction=%f:CreateMVAPdfs", 
             nCycles, HiddenLayers.c_str (), TrainingMethod.c_str (), ValidationFraction) ;

  outputFile_->cd () ;
  factory_->RootBaseDir ()->cd () ;

  factory_->BookMethod ( TMVA::Types::kTMlpANN, "TMlpANN", Option.Data ()) ;

  factory_->TrainAllMethods () ;
  factory_->TestAllMethods () ;
  factory_->EvaluateAllMethods () ;
  factory_->DeleteAllMethods () ;

  cout << "==> TMVAClassification is done!" << endl ;

}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Train BDT
void TMVATrainingClass::BookandTrainBDT ( const int & NTrees, 
                      const bool & optimizeMethods, 
                      const string & BoostType, 
                      const float & AdaBoostBeta, 
                      const string & PruneMethod, 
                      const int & PruneStrength, 
                      const int & MaxDepth, 
                      const string & SeparationType){


  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_ << endl ; 

  // Set Name of the Weight file for TMVA evaluating procedure
  if (Label_ != ""){                                                          
    outputFileWeightName_["BDT_"+Label_] = outputFilePath_+"/TMVAWeight_BDT_"+Label_ ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["BDT_"+Label_] ;
  }
  else {
    outputFileWeightName_["BDT"] = outputFilePath_+"/TMVAWeight_BDT" ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["BDT"] ;
  }

  TString Option = Form ("!H:!V:CreateMVAPdfs:NTrees=%d:BoostType=%s:AdaBoostBeta=%f:PruneMethod=%s:PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.1:MaxDepth=11:UseYesNoLeaf=F:MinNodeSize=2:nCuts=200", NTrees, BoostType.c_str (), AdaBoostBeta, PruneMethod.c_str (), PruneStrength, MaxDepth, SeparationType.c_str ()) ;

  outputFile_->cd () ;
  factory_->RootBaseDir ()->cd () ;

  factory_->BookMethod ( TMVA::Types::kBDT, "BDT", Option.Data ()) ;

  if (optimizeMethods) factory_->OptimizeAllMethods () ;                                                                                                                                                            

  factory_->TrainAllMethods () ;
  factory_->TestAllMethods () ;
  factory_->EvaluateAllMethods () ;
  factory_->DeleteAllMethods () ;

  cout << "==> TMVAClassification is done!" << endl ;

}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Train BDTG
void TMVATrainingClass::BookandTrainBDTG ( const int & NTrees, 
                       const bool & optimizeMethods, 
                       const float & GradBaggingFraction, 
                       const string & PruneMethod, 
                       const int & PruneStrength, 
                       const int & MaxDepth, 
                       const string & SeparationType){


  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_ << endl ; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                          
  if (Label_ != ""){
    outputFileWeightName_["BDTG_"+Label_] = outputFilePath_+"/TMVAWeight_BDTG_"+Label_ ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["BDTG_"+Label_] ;
      }
      else {
    outputFileWeightName_["BDTG"] = outputFilePath_+"/TMVAWeight_BDTG" ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["BDTG"] ;
  }

  TString Option = Form ("CreateMVAPdfs:NTrees=%d:BoostType=Grad:!UseBaggedGrad:GradBaggingFraction=%f:PruneMethod=%s:PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.1:NNodesMax=100000:UseYesNoLeaf=F:nCuts=2000",NTrees,GradBaggingFraction,PruneMethod.c_str (),PruneStrength,MaxDepth,SeparationType.c_str ()) ;

  outputFile_->cd () ;
  factory_->RootBaseDir ()->cd () ;

  factory_->BookMethod ( TMVA::Types::kBDT, "BDTG", Option.Data ()) ;
  
  if (optimizeMethods) factory_->OptimizeAllMethods () ;                                                                                                             
                             
  factory_->TrainAllMethods () ;
  factory_->TestAllMethods () ;
  factory_->EvaluateAllMethods () ;
  factory_->DeleteAllMethods () ;

  // PG e deleteAllMethods? FIXME
  
  cout << "==> TMVAClassification is done!" << endl ;

}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Train BDTF
void TMVATrainingClass::BookandTrainBDTF ( const int & NTrees, 
                       const bool & optimizeMethods, 
                       const string & BoostType, 
                       const float & AdaBoostBeta, 
                       const string & PruneMethod, 
                       const int & PruneStrength, 
                       const int & MaxDepth, 
                       const string & SeparationType){



  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_ << endl ; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                                 
  if (Label_ !=""){
    outputFileWeightName_["BDTF_"+Label_] = outputFilePath_+"/TMVAWeight_BDTF_"+Label_ ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["BDTF_"+Label_] ;
  }
  else{
    outputFileWeightName_["BDTF"] = outputFilePath_+"/TMVAWeight_BDTF" ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["BDTF"] ;
  }

  TString Option = Form ("!H:!V:CreateMVAPdfs:UseFisherCuts:NTrees=%d:BoostType=%s:AdaBoostBeta=%f:PruneMethod=%s:"
                         "PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.10:nCuts=20", NTrees, BoostType.c_str (), 
                          AdaBoostBeta, PruneMethod.c_str (), PruneStrength, MaxDepth, SeparationType.c_str ()) ;

  outputFile_->cd () ;
  factory_->RootBaseDir ()->cd () ;

  factory_->BookMethod ( TMVA::Types::kBDT, "BDTF", Option.Data ()) ;

  if (optimizeMethods) factory_->OptimizeAllMethods () ;
                                                                                                                                                             
  factory_->TrainAllMethods () ;
  factory_->TestAllMethods () ;
  factory_->EvaluateAllMethods () ;
  factory_->DeleteAllMethods () ;

  cout << "==> TMVAClassification is done!" << endl ;
}

