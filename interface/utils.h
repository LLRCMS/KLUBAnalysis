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
#include "TChainElement.h"
#include "TFile.h"
#include "TObjArray.h"
#include "TCut.h"
#include "TH1F.h"

struct sample
{
  sample (TString sampleName,
          TString theSampleFileName, 
          TString readingOption = "READ",
          TString treeName = "HTauTauTree") ;
  float calcEfficiency () ;
  TString sampleName ;
  TString sampleFileName ;
  TString rootFilesFolder ;
  TTree * sampleTree ;
  TFile * sampleFile ;
  float eff_num ;
  float eff_den ;
  float eff ;
} ;


int readSamples (vector<sample> & samples, vector<string> & samplesList, 
                 TString readOption = "READ") ;
vector<pair<TString, TCut> > readCutsFile (string filename) ;


#endif