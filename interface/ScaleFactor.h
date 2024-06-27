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

  TH1D *etaBinsH;

  void SetAxisBins(TGraphAsymmErrors*);
  bool check_SameBinning(TGraphAsymmErrors*, TGraphAsymmErrors*);
  std::string FindEtaLabel(double, std::string);
  int FindPtPoint(std::map<std::string, TGraphAsymmErrors *>, std::string, double, double, int);

public:
  ScaleFactor(){};
  ~ScaleFactor(){};
  void init_ScaleFactor(TString, std::string, bool);
  void init_EG_ScaleFactor(TString, bool);
  double get_EfficiencyData(double, double, int); //pt, eta, pType
  double get_EfficiencyMC(double, double, int);
  double get_ScaleFactor(double, double); // effData, effMC
  double get_ScaleFactor(double, double, int);
  double get_direct_ScaleFactor(double, double, int);
  double get_EfficiencyDataError(double, double, int);
  double get_EfficiencyMCError(double, double, int);
  double get_ScaleFactorError(double, double, double, double); // effData, effMC, errData, errMC
  double get_ScaleFactorError(double, double, int);

};


#endif
