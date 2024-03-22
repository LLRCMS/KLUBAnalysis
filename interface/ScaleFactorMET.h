// Custom MET scale factors derived with https://github.com/bfonta/inclusion
#ifndef ScaleFactorMET_h
#define ScaleFactorMET_h

#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <utility>
#include <cassert>

#include "TROOT.h"
#include "TFile.h"
#include "TF1.h"

class ScaleFactorMET {
public:
  ScaleFactorMET(std::string);
  ~ScaleFactorMET();
  
  double getSF(double);
  double getSFError(double);
  float getMinThreshold();
  
  template <typename T>
  using uMap = std::unordered_map<std::string, T>;
  
private:
  uMap<std::string> inputs;
  std::unique_ptr<TFile> fileIn;
  TF1 *funcSF, *funcData, *funcMC;
  std::string mPeriod;
  std::array<std::string, 4> mPeriods = {{"2016preVFP", "2016postVFP", "2017", "2018"}};
  const uMap<std::pair<double, double>> mRange = {
	{"2016preVFP",  {160., 325.}},
	{"2016postVFP", {160., 325.}},
	{"2017",        {160., 325.}},
	{"2018",        {150., 325.}}
  };

  void mCheckFile(std::unique_ptr<TFile>&, std::string);
  void mCheckPeriod();
  void mCheckChannel(std::string);
  double mErrorQuadSumSquared(double, std::string);
  double mErrorRatio(double);
  double mImposeBounds(double);
};

#endif
