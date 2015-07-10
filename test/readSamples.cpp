#include <iostream>
#include <vector>
#include "ConfigParser.h"
#include "TString.h"
#include "TChain.h"

using namespace std ;

struct sample
{
  sample (TString name) : 
    sampleName (name) ,
    sampleChain (new TChain ("HTauTauTree")) {}
  int addFiles (TString name) 
    {
      if (sampleChain->GetEntries () != 0) return -1 ;
      rootFilesFolder = name ;
      sampleChain->Add (rootFilesFolder + "/*.root") ;
      return sampleChain->GetEntriesFast () ;
    } 
  TString sampleName ;
  TString rootFilesFolder ;
  TChain * sampleChain ;
} ;


int
readSamples (vector<sample> & samples, vector<string> & samplesList)
{
  for (unsigned int iSample = 0 ; iSample < samplesList.size () ; ++iSample)
    {
      TString sampleFolder = gConfigParser->readStringOption (
          TString ("samples::") + samplesList.at (iSample).c_str ()
        ) ;
      cout << "reading " << samplesList.at (iSample) << " : " << sampleFolder << "\n" ; 
      samples.push_back (sample (samplesList.at (iSample))) ; 
      int done = samples.back ().addFiles (sampleFolder) ;  
      cout << " --> read " << done << " events\n" ; 
    }
  return 0 ;
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

  vector<string> sigSamplesList = gConfigParser->readStringListOption ("general::signals") ;
  vector<sample> sigSamples ;
  readSamples (sigSamples, sigSamplesList) ;

  vector<string> bkgSamplesList = gConfigParser->readStringListOption ("general::backgrounds") ;
  vector<sample> bkgSamples ;
  readSamples (bkgSamples, bkgSamplesList) ;

//   vector<string> sigSamplesList = gConfigParser->readStringListOption ("general::signals") ;
//   
//   
//   vector<string> bkgSamplesList = gConfigParser->readStringListOption ("general::backgrounds") ;
//   vector<string> variablesList = gConfigParser->readStringListOption ("general::variables") ;

  
/*

 // readBoolOption
  string sTest          = gConfigParser->readStringOption ("field::stringa") ;
  int iTest             = gConfigParser->readIntOption ("field::intero") ;
  float fTest           = gConfigParser->readFloatOption ("field::reale") ;
  vector<int> ivTest    = gConfigParser->readIntListOption ("field::intlist") ;
  vector<float> fvTest  = gConfigParser->readFloatListOption ("field::floatlist") ;

  cout << sTest << endl ;
  cout << iTest << endl ;
  cout << fTest << endl ;
  cout << endl ;
  for (int i = 0 ; i < ivTest.size () ; ++i)
    cout << ivTest.at (i) << " " ;
    
  cout << endl ;
  for (int i = 0 ; i < fvTest.size () ; ++i)
    cout << fvTest.at (i) << " " ;
    
  cout << endl ;
  for (int i = 0 ; i < svTest.size () ; ++i)
    cout << svTest.at (i) << " " ;
  cout << endl ;
*/    
  return 0 ;

}