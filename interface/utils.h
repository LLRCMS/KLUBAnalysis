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
#include "THStack.h"

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

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

void addTo (vector<float> & total, vector<float> & addition) ;

struct counters 
{
  vector<vector<float> > counters ; // [sample][selection]
  vector<float> initEfficiencies ; // [sample]

  vector<float> getTotalCountsPerCut ()
    {
      vector<float> total (counters.at(0).size (), 0.) ;
      // loop on the samples
      for (unsigned int i = 0 ; i < counters.size () ; ++i) 
        addTo (total, counters.at (i)) ;
      return total ;
    }

} ;


int readSamples (vector<sample> & samples, vector<string> & samplesList, 
                 TString readOption = "READ") ;
vector<pair<TString, TCut> > readCutsFile (string filename) ;
vector<pair <TString, TCut> > readCutsFile (vector<string> activeSelections, string filename) ;

struct isNOTalnum : std::unary_function<int, int>
{
    int operator()(int i) const { return !std::isalnum (i) ; }
} ;


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) ;
std::vector<std::string> split(const std::string &s, char delim) ;

#endif