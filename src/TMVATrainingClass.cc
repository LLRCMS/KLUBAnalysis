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
    const vector<sample> & signalTreeList, 
    const vector<sample> & backgroundTreeList,  
    const string & TreeName, 
    const string & outputFilePath , 
    const string & outputFileName, 
    const string & Label, 
    const string & transformation
  )
{
  SetTreeName          (TreeName) ;
  ReadInputCollection  (signalTreeList_,     signalTreeList) ;
  ReadInputCollection  (backgroundTreeList_, backgroundTreeList) ;
  SetLabel             (Label) ;
  SetTransformations   (transformation) ;  
  SetOutputFile        (outputFilePath, outputFileName) ;
  
  factory_.push_back (shared_ptr<TMVA::Factory> (new TMVA::Factory (TreeName_+"_"+Label_, 
      outputFile_.back ().get (), 
      Form ("!V:!Silent:%sColor:DrawProgressBar:AnalysisType=Classification%s", gROOT->IsBatch ()?"!":"", transformation.c_str ())))) ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Destructor
TMVATrainingClass::~TMVATrainingClass ()
{
  outputFileWeightName_.clear () ;

  factory_.clear () ;

  for (size_t iFile = 0 ; iFile < outputFile_.size () ; iFile++)
    outputFile_.at (iFile)->Close () ;

  outputFile_.clear () ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

                   
void TMVATrainingClass::ReadInputCollection (
    vector<TChain*> & localList,
    const vector<sample> & inputList  
  )
{
  localList.clear () ;  
  for (unsigned int iChain = 0 ; iChain < inputList.size () ; ++iChain)
    {
      cout << "TMVATrainingClass::ReadInputCollection reading sample " 
           << inputList.at (iChain).sampleName << endl ;
      localList.push_back (inputList.at (iChain).sampleChain) ;
    }
  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Set label
void TMVATrainingClass::SetLabel (const string & Label ){
  Label_ = Label ;
  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Set Tree Name
void TMVATrainingClass::SetTreeName (const string & TreeName ){

  if (TreeName!="") TreeName_ = TreeName ;
  else TreeName_ = "easyDelphes" ;

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
  if ( !outputFilePath.empty () && !outputFileName.empty ()) 
    { 
 
      outputFilePath_.push_back (outputFilePath) ; 
      outputFileName_.push_back (outputFileName) ;

      replace (outputFileName_.back ().begin (), outputFileName_.back ().end (), '/', '_') ;
      replace (outputFileName_.back ().begin (), outputFileName_.back ().end (), '[', '_') ;
      replace (outputFileName_.back ().begin (), outputFileName_.back ().end (), ']', '_') ;
      replace (outputFileName_.back ().begin (), outputFileName_.back ().end (), '(', '_') ;
      replace (outputFileName_.back ().begin (), outputFileName_.back ().end (), ')', '_') ;
      replace (outputFileName_.back ().begin (), outputFileName_.back ().end (), ':', '_') ;

      if (Label_!="") 
        outputFileNameComplete_.push_back (outputFilePath_.back ()+"/"+outputFileName_.back ()+"_"+Label_+".root") ;
      else
        outputFileNameComplete_.push_back (outputFilePath_.back ()+"/"+outputFileName_.back ()+".root") ;
 
      cout << "TMVATrainingClass::SetOutputFile " << outputFileNameComplete_.back () << endl ;

      outputFile_.push_back (shared_ptr<TFile> (new TFile ( (outputFileNameComplete_.back ()).c_str (), "RECREATE"))) ;   
      outputFile_.back ()->cd () ;
    }
  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void 
TMVATrainingClass::SetTransformations (const string & transformations)
{
  transformations_ = transformations ;
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
      factory_.back ()->AddVariable (trainingVariables_.at (iVar)+" := "+trainingVariables_.at (iVar), 'F') ;
    }

  for ( size_t iVar = 0 ; iVar < spectatorVariables_.size () ; ++iVar)
    {
      factory_.back ()->AddSpectator (spectatorVariables_.at (iVar), 'F') ;
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

  for (size_t ifact = 0 ; ifact < factory_.size () ; ++ifact)
    {
      factory_.at (ifact)->PrepareTrainingAndTestTree ("", "", Option.Data ()) ;  // set the options
    }
  
  // create the varlist for the TNtupla --> variables to be used as input, spectator and weights
  vector<string> varListSignal ;
  vector<string> varListBackground ;

  varListSignal.assign (1, "") ;

  // loop on training variables
  for (size_t iVar = 0 ; iVar < trainingVariables_.size () ; iVar++)
    { 
      if (iVar!=0) varListSignal.at (0) += ":"+trainingVariables_.at (iVar) ;
      else         varListSignal.at (0) += trainingVariables_.at (iVar) ;
    }
  varListSignal.at (0) += ":" ;

  // loop on spectator variables
  for (size_t iVar = 0 ; iVar < spectatorVariables_.size () ; iVar++)
    { 
      if (iVar!=0) varListSignal.at (0) += ":"+spectatorVariables_.at (iVar) ;
      else         varListSignal.at (0) += spectatorVariables_.at (iVar) ;
    }

  varListBackground = varListSignal ;
  
 /* questo non capisco a che serva
      // the re-weight can be different for signal and background, 
      // while the input and spectator must be the same
      replace (weightStringBackground.begin (), weightStringBackground.end (), '*', ':') ;
      replace (weightStringBackground.begin (), weightStringBackground.end (), '=', ':') ;
      replace (weightStringBackground.begin (), weightStringBackground.end (), '/', ':') ;
      varListBackground.at (0) += ":" + weightStringBackground ;

      replace (weightStringSignal.begin (), weightStringSignal.end (), '*', ':') ;
      replace (weightStringSignal.begin (), weightStringSignal.end (), '=', ':') ;
      replace (weightStringSignal.begin (), weightStringSignal.end (), '/', ':') ;
      varListSignal.at (0) += ":" + weightStringSignal ;  
*/
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Book MVA Training Variables 
void TMVATrainingClass::BookMVATrees ()
{
  for (size_t iNtuple = 0 ; iNtuple < signalTreeList_.size () ; iNtuple++)
    {
      cout << "TMVATrainingClass::BookMVATrees : add signal to the MVA factory " << iNtuple << endl ;
      factory_.back ()->AddSignalTree (signalTreeList_.at (iNtuple), 1.) ;
    }
  for (size_t iNtuple = 0 ; iNtuple < backgroundTreeList_.size () ; iNtuple++)
    {
      cout << "TMVATrainingClass::BookMVATrees : add background to the MVA factory " << iNtuple << endl ;
      factory_.back ()->AddBackgroundTree (backgroundTreeList_.at (iNtuple), 1.) ;
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
  for (size_t ifact = 0 ; ifact < factory_.size () ; ifact++)
    {
      factory_.at (ifact)->RootBaseDir ()->cd () ;
      factory_.at (ifact)->SetSignalWeightExpression (weightStringSignal) ;
      factory_.at (ifact)->SetBackgroundWeightExpression (weightStringBackground) ;
    }
    
  return ;

}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Train Rectangular Cuts
void TMVATrainingClass::BookandTrainRectangularCuts (const string & FitMethod)
{
  // create output directory if not there and change the variable name in case of a list
  string command = " if [ ! -e "+outputFilePath_.back ()+" ] ; then mkdir "+outputFilePath_.back ()+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_.back () << endl ; 

  for (size_t iVar = 0 ; iVar < trainingVariables_.size () ; iVar++)
    {

      string variable = trainingVariables_.at (iVar) ;
      replace (variable.begin (), variable.end (), '/', '_') ;
      replace (variable.begin (), variable.end (), '[', '_') ;
      replace (variable.begin (), variable.end (), ']', '_') ;
      replace (variable.begin (), variable.end (), '(', '_') ;
      replace (variable.begin (), variable.end (), ')', '_') ;
      replace (variable.begin (), variable.end (), ':', '_') ;

      outputFile_.at (iVar)->cd () ;
      factory_.at (iVar)->RootBaseDir ()->cd () ;

      // Set Name of the Weight file for TMVA evaluating procedure
      if (Label_ !=""){
    outputFileWeightName_["Cuts"+FitMethod+"_"+Label_] = outputFilePath_.back ()+"/TMVAWeight_Cuts"+FitMethod+"_"+Label_+"_"+variable ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["Cuts"+FitMethod+"_"+Label_] ;
      }
      else {
    outputFileWeightName_["Cuts"+FitMethod] = outputFilePath_.back ()+"/TMVAWeight_Cuts"+FitMethod+"_"+variable ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["Cuts"+FitMethod] ;
      }

      // book the method
      if (FitMethod!=""){ 

    TString Option = Form ("!H:!V:FitMethod=%s:EffSel", FitMethod.c_str ()) ;
    TString Name   = Form ("Cuts%s", FitMethod.c_str ()) ;

    if (TString (Name).Contains ("CutsGA"))  
      Option = Option+":CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95" ;

    factory_.at (iVar)->BookMethod ( TMVA::Types::kCuts, Name.Data (), Option.Data ()) ;
      }

      else{

        TString Option = Form ("!H:!V:FitMethod=MC:EffSel") ;

        factory_.at (iVar)->BookMethod ( TMVA::Types::kCuts, "CutsMC"+FitMethod, Option.Data ()) ;

        Option = Form ("!H:!V:FitMethod=GA::CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95") ;
    
        factory_.at (iVar)->BookMethod ( TMVA::Types::kCuts, "CutsGA"+FitMethod, Option.Data ()) ;
    
        Option = Form ("!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale") ;

        factory_.at (iVar)->BookMethod ( TMVA::Types::kCuts, "CutsSA"+FitMethod, Option.Data ()) ;
      }    

      factory_.at (iVar)->TrainAllMethods () ;

      factory_.at (iVar)->TestAllMethods () ;

      factory_.at (iVar)->EvaluateAllMethods () ;
    
      factory_.at (iVar)->DeleteAllMethods () ;
    }

  cout << "==> TMVAClassification is done!" << endl ;

}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Train Likelihood
void TMVATrainingClass::BookandTrainLikelihood ( const string & LikelihoodType )
{
  string command = " if [ ! -e "+outputFilePath_.back ()+" ] ; then mkdir "+outputFilePath_.back ()+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_.back () << endl ; 

  // Set Name of the Weight file for TMVA evaluating procedure
  if (Label_ !=""){
    outputFileWeightName_[LikelihoodType+"_"+Label_] = outputFilePath_.back ()+"/TMVAWeight_"+LikelihoodType+"_"+Label_ ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_[LikelihoodType+"_"+Label_] ;
  }
  else {
    outputFileWeightName_[LikelihoodType] = outputFilePath_.back ()+"/TMVAWeight_"+LikelihoodType ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_[LikelihoodType] ;
  }

  TString Option ;

  outputFile_.back ()->cd () ;
  factory_.back ()->RootBaseDir ()->cd () ;

  if ( LikelihoodType == "LikelihoodKDE") { 
    Option = Form ("LikelihoodKDE") ;
    factory_.back ()->BookMethod (TMVA::Types::kLikelihood, Option.Data (), "!H:!V:IgnoreNegWeightsInTraining:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:"
                                                     "KDEiter=Adaptive:CreateMVAPdfs:KDEFineFactor=0.3:KDEborder=None") ;
  }
  else if ( LikelihoodType == "PDERS") { 
      Option = Form ("%s", LikelihoodType.c_str ()) ;
      factory_.back ()->BookMethod (TMVA::Types::kPDERS, Option.Data (), 
                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:CreateMVAPdfs:DeltaFrac=4:GaussSigma=0.3:NormTree=T") ;
  }
  else if ( LikelihoodType == "PDEFoam") { 
       Option = Form ("%s", LikelihoodType.c_str ()) ;
       factory_.back ()->BookMethod (TMVA::Types::kPDEFoam, Option.Data (), "!H:!V:CreateMVAPdfs:IgnoreNegWeightsInTraining:SigBgSeparate=F:TailCut=0.001"
                                                                 ":VolFrac=0.0666:nActiveCells=500:nSampl=2000:nBin=5:Nmin=100:Kernel=None:Compress=T") ;
  }
  else if ( LikelihoodType == "PDEFoamBoost") { 
      Option = Form ("%s", LikelihoodType.c_str ()) ;
      factory_.back ()->BookMethod (TMVA::Types::kPDEFoam, Option.Data (), 
                           "!H:!V:IgnoreNegWeightsInTraining:Boost_Num=30:CreateMVAPdfs:Boost_Transform=linear:SigBgSeparate=F:MaxDepth=4"
                           ":UseYesNoCell=T:DTLogic=MisClassificationError:FillFoamWithOrigWeights=F:TailCut=0:nActiveCells=300:nBin=20:Nmin=300:Kernel=None:Compress=T") ;
  }
  else{ Option = Form ("%s", LikelihoodType.c_str ()) ;
        factory_.back ()->BookMethod ( TMVA::Types::kLikelihood, Option.Data (), "!H:!V:!TransformOutput:CreateMVAPdfs:IgnoreNegWeightsInTraining:PDFInterpol=Spline2"
                                                          ":NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50") ;
  }


  factory_.back ()->TrainAllMethods () ;

  factory_.back ()->TestAllMethods () ;

  factory_.back ()->EvaluateAllMethods () ;

  factory_.back ()->DeleteAllMethods () ;

 cout << "==> TMVAClassification is done!" << endl ;

}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Train Fisher Discriminant
void TMVATrainingClass::BookandTrainFisherDiscriminant ()
{
  string command = " if [ ! -e "+outputFilePath_.back ()+" ] ; then mkdir "+outputFilePath_.back ()+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_.back () << endl ; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                              
  if (Label_ !=""){
    outputFileWeightName_["Fisher_"+Label_] = outputFilePath_.back ()+"/TMVAWeight_Fisher_"+Label_ ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["Fisher_"+Label_] ;
  }
  else{
    outputFileWeightName_["Fisher"] = outputFilePath_.back ()+"/TMVAWeight_Fisher" ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["Fisher"] ;
  }

  outputFile_.back ()->cd () ;
  factory_.back ()->RootBaseDir ()->cd () ;

  factory_.back ()->BookMethod ( TMVA::Types::kFisher, "Fisher", 
                        "!H:!V:CreateMVAPdfs:IgnoreNegWeightsInTraining:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10:Fisher") ;


  factory_.back ()->TrainAllMethods () ;

  factory_.back ()->TestAllMethods () ;

  factory_.back ()->EvaluateAllMethods () ;

  factory_.back ()->DeleteAllMethods () ;

  cout << "==> TMVAClassification is done!" << endl ;

}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Train Linear Discriminant
void TMVATrainingClass::BookandTrainLinearDiscriminant (){

  string command = " if [ ! -e "+outputFilePath_.back ()+" ] ; then mkdir "+outputFilePath_.back ()+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_.back () << endl ;
 
  // Set Name of the Weight file for TMVA evaluating procedure
  if (Label_ !=""){
    outputFileWeightName_["LD_"+Label_] = outputFilePath_.back ()+"/TMVAWeight_LD_"+Label_ ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["LD_"+Label_] ;
  }
  else {
    outputFileWeightName_["LD"] = outputFilePath_.back ()+"/TMVAWeight_LD" ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["LD"] ;
  }

  // Training Testing and Evaluating   
  outputFile_.back ()->cd () ;
  factory_.back ()->RootBaseDir ()->cd () ;

  factory_.back ()->BookMethod ( TMVA::Types::kLD, "LD", "H:!V:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10") ;

  factory_.back ()->TrainAllMethods () ;

  factory_.back ()->TestAllMethods () ;

  factory_.back ()->EvaluateAllMethods () ;

  factory_.back ()->DeleteAllMethods () ;

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

  string command = " if [ ! -e "+outputFilePath_.back ()+" ] ; then mkdir "+outputFilePath_.back ()+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_.back () << endl ; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                            
  if (Label_ != ""){
    outputFileWeightName_["MLP_"+Label_] = outputFilePath_.back ()+"/TMVAWeight_MLP_"+Label_ ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["MLP_"+Label_] ;
  }
  else{
    outputFileWeightName_["MLP"] = outputFilePath_.back ()+"/TMVAWeight_MLP" ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["MLP"] ;
  }

  TString Option = Form ("!H:!V:NCycles=%d:CalculateErrors:HiddenLayers=%s:NeuronType=%s:CreateMVAPdfs:TrainingMethod=%s:TestRate=%d"
             ":ConvergenceTests=%d:UseRegulator", nCycles, HiddenLayers.c_str (), NeuronType.c_str (), TrainingMethod.c_str (), TestRate, 
                         ConvergenceTests) ;

  outputFile_.back ()->cd () ;
  factory_.back ()->RootBaseDir ()->cd () ;

  factory_.back ()->BookMethod ( TMVA::Types::kMLP, "MLP", Option.Data ()) ;
  
  factory_.back ()->TrainAllMethods () ;

  factory_.back ()->TestAllMethods () ;

  factory_.back ()->EvaluateAllMethods () ;

  factory_.back ()->DeleteAllMethods () ;

  cout << "==> TMVAClassification is done!" << endl ;

}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Train Clemont Ferrand ANN
void TMVATrainingClass::BookandTrainCFMlpANN ( const int & nCycles, 
                           const string & HiddenLayers){

  string command = " if [ ! -e "+outputFilePath_.back ()+" ] ; then mkdir "+outputFilePath_.back ()+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_.back () << endl ; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                            
  if (Label_ !=""){
    outputFileWeightName_["CFMlpANN_"+Label_] = outputFilePath_.back ()+"/TMVAWeight_CFMlpANN_"+Label_ ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["CFMlpANN_"+Label_] ;
  }
  else {
    outputFileWeightName_["CFMlpANN"] = outputFilePath_.back ()+"/TMVAWeight_CFMlpANN" ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["CFMlpANN"] ;
  }

  TString Option = Form ("!H:!V:NCycles=%d:HiddenLayers=%s:CreateMVAPdfs", nCycles, HiddenLayers.c_str ()) ;
 
  outputFile_.back ()->cd () ;
  factory_.back ()->RootBaseDir ()->cd () ;

  factory_.back ()->BookMethod ( TMVA::Types::kCFMlpANN, "CFMlpANN", Option.Data ()) ;

  factory_.back ()->TrainAllMethods () ;

  factory_.back ()->TestAllMethods () ;

  factory_.back ()->EvaluateAllMethods () ;

  factory_.back ()->DeleteAllMethods () ;

  cout << "==> TMVAClassification is done!" << endl ;

}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// Train TMVA ANN
void TMVATrainingClass::BookandTrainTMlpANN  ( const int & nCycles, 
                           const string & HiddenLayers,  
                           const string & TrainingMethod, 
                           const float & ValidationFraction){

  string command = " if [ ! -e "+outputFilePath_.back ()+" ] ; then mkdir "+outputFilePath_.back ()+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_.back () << endl ; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                        
  if (Label_ != ""){         
    outputFileWeightName_["TMlpANN_"+Label_] = outputFilePath_.back ()+"/TMVAWeight_TMlpANN_"+Label_ ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["TMlpANN_"+Label_] ;
  }
  else {
    outputFileWeightName_["TMlpANN"] = outputFilePath_.back ()+"/TMVAWeight_TMlpANN" ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["TMlpANN"] ;
  }

  TString Option = Form ("!H:!V:NCycles=%d:HiddenLayers=%s:LearningMethod=%s:ValidationFraction=%f:CreateMVAPdfs", 
             nCycles, HiddenLayers.c_str (), TrainingMethod.c_str (), ValidationFraction) ;

  outputFile_.back ()->cd () ;
  factory_.back ()->RootBaseDir ()->cd () ;

  factory_.back ()->BookMethod ( TMVA::Types::kTMlpANN, "TMlpANN", Option.Data ()) ;

  factory_.back ()->TrainAllMethods () ;

  factory_.back ()->TestAllMethods () ;

  factory_.back ()->EvaluateAllMethods () ;

  factory_.back ()->DeleteAllMethods () ;

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


  string command = " if [ ! -e "+outputFilePath_.back ()+" ] ; then mkdir "+outputFilePath_.back ()+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_.back () << endl ; 

  // Set Name of the Weight file for TMVA evaluating procedure
  if (Label_ != ""){                                                          
    outputFileWeightName_["BDT_"+Label_] = outputFilePath_.back ()+"/TMVAWeight_BDT_"+Label_ ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["BDT_"+Label_] ;
  }
  else {
    outputFileWeightName_["BDT"] = outputFilePath_.back ()+"/TMVAWeight_BDT" ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["BDT"] ;
  }

  TString Option = Form ("!H:!V:CreateMVAPdfs:NTrees=%d:BoostType=%s:AdaBoostBeta=%f:PruneMethod=%s:PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.1:NNodesMax=100000:UseYesNoLeaf=F:MinNodeSize=2:nCuts=200", NTrees, BoostType.c_str (), AdaBoostBeta, PruneMethod.c_str (), PruneStrength, MaxDepth, SeparationType.c_str ()) ;

  outputFile_.back ()->cd () ;
  factory_.back ()->RootBaseDir ()->cd () ;

  factory_.back ()->BookMethod ( TMVA::Types::kBDT, "BDT", Option.Data ()) ;

  if (optimizeMethods) factory_.back ()->OptimizeAllMethods () ;                                                                                                                                                            
  factory_.back ()->TrainAllMethods () ;

  factory_.back ()->TestAllMethods () ;

  factory_.back ()->EvaluateAllMethods () ;

  factory_.back ()->DeleteAllMethods () ;

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


  string command = " if [ ! -e "+outputFilePath_.back ()+" ] ; then mkdir "+outputFilePath_.back ()+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_.back () << endl ; 


    for (size_t iVar = 0 ; iVar < trainingVariables_.size () ; iVar++){

      string variable = trainingVariables_.at (iVar) ;
      replace (variable.begin (), variable.end (), '/', '_') ;
      replace (variable.begin (), variable.end (), '[', '_') ;
      replace (variable.begin (), variable.end (), ']', '_') ;
      replace (variable.begin (), variable.end (), '(', '_') ;
      replace (variable.begin (), variable.end (), ')', '_') ;
      replace (variable.begin (), variable.end (), ':', '_') ;

      outputFile_.at (iVar)->cd () ;
      factory_.at (iVar)->RootBaseDir ()->cd () ;

      // Set Name of the Weight file for TMVA evaluating procedure                                                                                                          
      if (Label_ != ""){
    outputFileWeightName_["BDTG_"+Label_] = outputFilePath_.back ()+"/TMVAWeight_BDTG_"+Label_+"_"+variable ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["BDTG_"+Label_] ;
      }
      else {
    outputFileWeightName_["BDTG"] = outputFilePath_.back ()+"/TMVAWeight_BDTG_"+variable ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["BDTG"] ;
      }

      TString Option = Form ("CreateMVAPdfs:NTrees=%d:BoostType=Grad:!UseBaggedGrad:GradBaggingFraction=%f:PruneMethod=%s:PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.1:NNodesMax=100000:UseYesNoLeaf=F:nCuts=2000:IgnoreNegWeightsInTraining", NTrees, GradBaggingFraction, PruneMethod.c_str (), PruneStrength, MaxDepth, SeparationType.c_str ()) ;
      
      factory_.at (iVar)->BookMethod ( TMVA::Types::kBDT, "BDTG", Option.Data ()) ;
      
      if (optimizeMethods) factory_.at (iVar)->OptimizeAllMethods () ;                                                                                                             
                                 
      factory_.at (iVar)->TrainAllMethods () ;
      
      factory_.at (iVar)->TestAllMethods () ;
  
      factory_.at (iVar)->EvaluateAllMethods () ;

    }
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



  string command = " if [ ! -e "+outputFilePath_.back ()+" ] ; then mkdir "+outputFilePath_.back ()+" ; fi" ;
  int result = system (command.c_str ()) ;
  if (result) cout << "Directory created " << outputFilePath_.back () << endl ; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                                 
  if (Label_ !=""){
    outputFileWeightName_["BDTF_"+Label_] = outputFilePath_.back ()+"/TMVAWeight_BDTF_"+Label_ ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["BDTF_"+Label_] ;
  }
  else{
    outputFileWeightName_["BDTF"] = outputFilePath_.back ()+"/TMVAWeight_BDTF" ;
    (TMVA::gConfig ().GetIONames ()).fWeightFileDir = outputFileWeightName_["BDTF"] ;
  }

  TString Option = Form ("!H:!V:CreateMVAPdfs:UseFisherCuts:NTrees=%d:BoostType=%s:AdaBoostBeta=%f:PruneMethod=%s:"
                         "PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.10:nCuts=20", NTrees, BoostType.c_str (), 
                          AdaBoostBeta, PruneMethod.c_str (), PruneStrength, MaxDepth, SeparationType.c_str ()) ;

  outputFile_.back ()->cd () ;
  factory_.back ()->RootBaseDir ()->cd () ;

  factory_.back ()->BookMethod ( TMVA::Types::kBDT, "BDTF", Option.Data ()) ;

  if (optimizeMethods) factory_.back ()->OptimizeAllMethods () ;
                                                                                                                                                             
  factory_.back ()->TrainAllMethods () ;

  factory_.back ()->TestAllMethods () ;

  factory_.back ()->EvaluateAllMethods () ;

  factory_.back ()->DeleteAllMethods () ;

  cout << "==> TMVAClassification is done!" << endl ;
}

