#include <iostream>
#include <vector>
#include "ConfigParser.h"
#include "utils.h"
#include "TString.h"
#include "TChain.h"
#include "TCut.h"

using namespace std ;


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

  // get the samples to be analised

  vector<string> sigSamplesList = gConfigParser->readStringListOption ("general::signals") ;
  vector<sample> sigSamples ;
  readSamples (sigSamples, sigSamplesList) ;

  vector<string> bkgSamplesList = gConfigParser->readStringListOption ("general::backgrounds") ;
  vector<sample> bkgSamples ;
  readSamples (bkgSamples, bkgSamplesList) ;

  vector<pair <TString, TCut> > selections ;
  vector<string> selList = gConfigParser->readStringListOption ("selections::list") ;
  for (unsigned int i = 0 ; i < selList.size () ; ++i)
    {
      string thisCut = gConfigParser->readStringOption (
          TString ("selections::") + selList.at (i)
        ) ;
      selections.push_back (pair <TString, TCut> (selList.at (i).c_str (), thisCut.c_str ())) ;
    }
  

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