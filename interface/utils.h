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
#include <utility>
#include <memory>
#include "AnalysisHelper.h"
#include "ConfigParser.h"
#include "CfgParser.h"
#include "Sample.h"
#include "TString.h"
#include "TChain.h"
#include "TChainElement.h"
#include "TFile.h"
#include "TObjArray.h"
#include "TCut.h"
#include "TH1F.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TGaxis.h"
#include "TObject.h"
#include "plotContainer.h"

struct mysample
{
  mysample (TString sampleName,
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

void addTo (std::vector<float> & total, std::vector<float> & addition) ;

struct counters
{
  std::vector<std::vector<float> > counters ; // [sample][selection]
  std::vector<float> initEfficiencies ; // [sample]

  std::vector<float> getTotalCountsPerCut ()
    {
      std::vector<float> total (counters.at(0).size (), 0.) ;
      // loop on the samples
      for (unsigned int i = 0 ; i < counters.size () ; ++i)
        addTo (total, counters.at (i)) ;
      return total ;
    }

} ;


int readSamples (std::vector<mysample> & samples, std::vector<std::string> & samplesList,
                 TString readOption = "READ") ;
int readSamples (std::vector<mysample> & samples, std::vector<std::string> & samplesList, const std::unique_ptr<CfgParser> & lConfigParser,
                 TString readOption = "READ") ;

std::vector<std::pair<TString, TCut> > readCutsFile (std::string filename) ;
std::vector<std::pair <TString, TCut> > readCutsFile (std::vector<std::string> activeSelections, std::string filename) ;
std::vector<std::pair <std::string, std::string> > readVarNamesFile (std::vector<std::string> varList, std::string filename) ;



struct isNOTalnum : std::unary_function<int, int>
{
  int operator()(int i) const { return !std::isalnum (i) ; }
} ;


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) ;
std::vector<std::string> split(const std::string &s, char delim) ;



// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// utilities to draw tables of yields

void printTableTitle (std::ostream& out, std::vector<std::string> & sample, unsigned int NSpacesColZero = 16, unsigned int NSpacesColumns = 10) ;
void printTableTitle (std::ostream& out, std::vector<mysample> & sample, unsigned int NSpacesColZero = 16, unsigned int NSpacesColumns = 10) ;
void printTableTitle (std::ostream& out, std::vector<mysample> & sample, std::vector<std::string> & DataDrivenBkgsName, unsigned int NSpacesColZero = 16, unsigned int NSpacesColumns = 10) ; // adding e.g. QCD that has not a coutner structure

void printTableBody  (std::ostream& out, std::vector<std::pair <TString, TCut> > & selections, counters & count, std::vector<mysample> & samples, unsigned int NSpacesColZero = 16, unsigned int NSpacesColumns = 10, unsigned int precision = 1) ;
void printTableBody  (std::ostream& out, std::vector<std::pair <TString, TCut> > & selections, counters & count, std::vector<mysample> & samples, std::vector<std::vector<float>> & DataDrivenSamplesYields, unsigned int NSpacesColZero = 16, unsigned int NSpacesColumns = 10, unsigned int precision = 1) ;

void printTableBodyEff  (std::ostream& out, std::vector<std::pair <TString, TCut> > & selections, counters & count, std::vector<mysample> & samples, unsigned int NSpacesColZero = 16, unsigned int NSpacesColumns = 10, unsigned int precision = 1) ;


// title + body, flush to file or std cout
//void printTable (vector<sample> & sample, vector<pair <TString, TCut> > & selections, counters & count, vector<sample> & samples, vector<vector<float>> & DataDrivenSamplesYields, unsigned int NSpacesColZero = 16, unsigned int NSpacesColumns = 10, unsigned int precision = 1) ;

#endif
