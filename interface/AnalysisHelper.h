/*
** class: AnalysisHelper
** author: L. Cadamuro (LLR)
** date: 26/05/2016
** description: class handling and coordinating all the part of the histo fillling analysis step.
** It is interfaced with the parser to keep all the analysis relevant info
** note about cfg organization:
** information is supposed to be stored in different configs according to the information
** that must be shared or not between the different channels.
** Only main cfg is passed to the program, and it must contain the names of the other secondary cfgs
** Configs handled:
** - main cfg: specific to the analysis, contains selection set etc..
** - sample cfg: list of samples and their pahts (supposed to be common)
** - cut cfg: contains the definition of the selections
*/

#ifndef ANALYSISHELPER_H
#define ANALYSISHELPER_H

#include <array>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <iostream>

#include <boost/variant.hpp>

#include "CfgParser.h"
#include "Sample.h"
#include "Weight.h"
#include "Weight_ext.h"
#include "Selection.h"
#include "ordered_map.h"

#include "TCut.h"

class AnalysisHelper
{
public:

  typedef ordered_map <std::string, std::shared_ptr<Sample>> sampleColl; // the type of a collection of samples

  AnalysisHelper(std::string cfgname, std::string merge_section, int idxsplit, int njobs);
  ~AnalysisHelper();

  CfgParser& mainCfg() {return *mainCfg_;}
  bool readMainInfo(); // all analysis info read here

  bool sanityChecks();
  
  std::shared_ptr<Sample> openSample(std::string sampleName);
  void prepareHistos();

  void readSamples(); // inits the samles
  void readSelections();
  void readVariables();

  sampleColl & getSignals() {return sig_samples_;};
  sampleColl & getData() {return data_samples_;};
  sampleColl & getBackgrounds() {return bkg_samples_;};

  void fillHistos();

  void mergeSamples();

  void printSelections(bool printWeights=false, bool printSysts=false);
  void printSamples(bool printWeights=false, bool printSysts=false);

  void saveOutputsToFile(std::string);

  void setVerbosity (int v) {verbosity_ = v;}
  void dump(int detail=0);

private:
  std::string formHistoName (std::string sample, std::string sel, std::string var, std::string syst);
  std::string formHisto2DName (std::string sample, std::string sel, std::string var1, std::string var2, std::string syst);
  Selection readSingleSelection (std::string name);
  std::vector<std::pair<std::string, std::string> > readWeightSysts(std::string name, std::string section); // read w list of section::name
  void fillHistosSample(Sample& sample);
  void activateBranches(Sample& sample);
  std::pair<std::string, std::string> unpack2DName(std::string packedName);
  std::string pack2DName (std::string name1, std::string name2);
  void prepareSamplesHistos();
  void prepareSamples2DHistos();
  void setSplitting (int idxsplit, int nsplit);
  
  std::unique_ptr<CfgParser> mainCfg_;
  std::unique_ptr<CfgParser> cutCfg_;
  std::unique_ptr<CfgParser> sampleCfg_;
  // FIXME: sytematics cfg?

  int nsplit_;
  int idxsplit_;

  float lumi_;
  std::string outputFolder_;
  std::string outputFileName_;

  // ordered_map<std::string, TCut> selections_;
  std::vector<Selection> selections_;
  std::vector<std::string> variables_;
  std::vector<std::pair<std::string, std::string>> variables2D_;

  ordered_map <std::string, std::shared_ptr<Sample>> data_samples_;
  ordered_map <std::string, std::shared_ptr<Sample>> sig_samples_;
  ordered_map <std::string, std::shared_ptr<Sample>> bkg_samples_;
  ordered_map <std::string, std::vector<std::string>> sample_merge_list_;

  std::string nominal_name_;
  std::string merge_section_;
  int verbosity_;
};

// used to access the variant that stores weights and variables
// and to return always a double to be used for TH1.Fill
class get_variant_as_double : public boost::static_visitor<double>
{
public:
  double operator()(int& x) const
  {
    return (double) x;
  }

  double operator()(float& x) const
  {
    return (double) x;
  }

  double operator()(double& x) const
  {
    return x;
  }

  double operator()(bool& x) const
  {
    return x ? 1.0 : 0.0;
  }
};

#endif
