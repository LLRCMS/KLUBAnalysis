/*
** class: Sample
** author: L. Cadamuro (LLR)
** date: 26/05/2016 (updated by B. Alves on 2024)
** description: class representing a data sample (either data or MC). Handles file opening and histograms.
**              input files are listed in a txt file "filename".
*/

#ifndef SAMPLE_H
#define SAMPLE_H

#include <memory>
#include <string>
#include <vector>
#include <regex>
#include "Weight.h"
#include "Weight_ext.h"
#include "ordered_map.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTreeIndex.h"

class Sample
{
public:
  // hierarchy of objects [selection[variable[systematics]]]
  typedef ordered_map<std::string, std::shared_ptr<TH1F> >  systColl;  // histograms stored - map is for systematics
  typedef ordered_map<std::string, systColl>                varColl;   // variables plotted
  typedef ordered_map<std::string, varColl>                 selColl;   // all selections

  // note: for 2D histos, a 2D plot is still associated to a "single" variable, eg, if I want to plot A vs B, var2D = A:B (as in TTree)
  typedef ordered_map<std::string, std::shared_ptr<TH2F> >  systColl2D;  // histograms stored - map is for systematics
  typedef ordered_map<std::string, systColl2D>              varColl2D;   // variables plotted
  typedef ordered_map<std::string, varColl2D>               selColl2D;   // all selections

  enum sType {
    kData = 0,
    kSig  = 1,
    kBkg  = 2
  };

  // standard ctor/dtor
  Sample(std::string, std::vector<std::string>,
		 std::string, std::string, int);
  Sample(std::string, std::vector<std::string>,
		 std::string, std::string,
		 std::string, std::string, std::string, int);
  
  ~Sample() {};
  std::string getName() const {return name_;}

  void  setType (sType sampleType) {sampleType_ = sampleType;}
  sType getType () {return sampleType_;}

  // file handling and preparation
  bool openFileAndTree();
  long long int getEntries(){return nentries_;}
  void setEffBin(int ibin){
    std::cout << "  ---> INFO: setting eff. bin " << ibin << " for sample " << name_ << std::endl;
    bin_eff_den_ = ibin;
  }
  double getEffDenom(){return evt_den_;}

  // plot handling
  selColl& plots() {return plots_;}
  selColl2D& plots2D() {return plots2D_;}
  void scaleAll(double scale);

  // specific weights for a certain sample
  void setWeights (std::vector<Weight> weights) {weights_ = weights;}
  void addWeight  (Weight weight) {weights_.push_back(weight);}
  void clearWeights() {weights_.clear();}
  void clearExtWeights() {weights_ext_.clear();}

  TChain* getTree() {return skim_;}
  std::map<std::string, std::string> getBranches(bool force=false);
  unsigned getNBranches(bool force=false);

  const std::vector<Weight>& getWeights() const {return weights_;}
  std::vector<Weight>& getWeights() {return weights_;}

private:
  bool friend_added_ = false;
  std::string name_;
  double eff_;
  double evt_num_, evt_den_;
  long long int nentries_;
  std::vector<std::string> filelistname_;
  int bin_eff_den_;
  std::string skimname_ = "", evalname_ = "NOT_SET";
  std::string skimpath_ = "", evalpath_ = "NOT_SET";
  std::string histoname_ = "";
  TChain *skim_ = nullptr, *eval_ = nullptr;

  sType  sampleType_; // used in general to label the type of sample

  selColl plots_;
  selColl2D plots2D_;

  std::vector<Weight> weights_;
  std::vector<Weight_ext> weights_ext_;
};

#endif
