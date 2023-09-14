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
  ScaleFactorMET();
  ~ScaleFactorMET();
  
  double getSF(double, std::string);
  double getSFError(double, std::string);

  template <typename T>
  using uMap = std::unordered_map<std::string, T>;
  
private:
  uMap<std::string> inputs;
  uMap<std::unique_ptr<TFile>> fileIn;
  uMap<TF1*> funcSF, funcData, funcMC;
  std::array<std::string, 4> mPeriods = {{"2016preVFP", "2016postVFP", "2017", "2018"}};
  std::pair<double, double> mRange;

  void mCheckFile(std::unique_ptr<TFile>&, std::string);
  void mCheckPeriod(std::string);
  double mErrorQuadSumSquared(double, std::string, std::string);
  double mErrorRatio(double, std::string);
};

#endif
