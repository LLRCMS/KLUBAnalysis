//https://github.com/CMS-HTT/LeptonEff-interface.git
#ifndef ScaleFactor_h
#define ScaleFactor_h

#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraphAsymmErrors.h"
#include <iostream>
#include <map>
#include <cmath>
#include <string>
#include <algorithm>
#include <cstdbool>


class ScaleFactor {

private:
  std::map<std::string, TGraphAsymmErrors *> eff_data;
  std::map<std::string, TGraphAsymmErrors *> eff_mc;

  TH1D * etaBinsH;

  void  SetAxisBins(TGraphAsymmErrors*);
  bool  check_SameBinning(TGraphAsymmErrors*, TGraphAsymmErrors*);
  //std::string FindEtaLabel(double, std::string);
  int FindPtBin( std::map<std::string, TGraphAsymmErrors *>, std::string, double);

public:
  ScaleFactor(){};
  void init_ScaleFactor(TString);
  void init_ScaleFactor(TString,std::string);
  void init_ScaleFactor(TString, std::string, bool);
  void init_EG_ScaleFactor(TString, bool);
  ~ ScaleFactor(){};
  std::string FindEtaLabel(double, std::string);
  double get_EfficiencyData(double, double); //pt, eta
  double get_EfficiencyMC(double, double);
  double get_ScaleFactor(double, double);
  double get_direct_ScaleFactor(double, double);
  double get_EfficiencyDataError(double, double);
  double get_EfficiencyMCError(double, double);
  double get_ScaleFactorError(double, double);

};


#endif
