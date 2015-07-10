#ifndef utils_h
#define utils_h

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include "ConfigParser.h"
#include "TString.h"
#include "TChain.h"
#include "TCut.h"

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


int readSamples (vector<sample> & samples, vector<string> & samplesList) ;
vector<pair<TString, TCut> > readCutsFile (string filename) ;


#endif