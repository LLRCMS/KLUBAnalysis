/*
** class  : HHReweight
** author : L. Cadamuro (LLR)
** date   : 17 Feb 2017
** brief  : Compute a reweight coefficient to transform an input distribution into a general point in kl, kt plane
*/


#ifndef HHREWEIGHT_H
#define HHREWEIGHT_H

#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <utility>

#include "TH1.h"
#include "TH2.h"
#include "TAxis.h"
#include "TFile.h"

class HHReweight{

public:
  HHReweight(std::string coeffFile, const TH2* hInput);
  ~HHReweight();
  double getWeight(double kl, double kt, double mhh, double cth);

private:
  void readInputFile(std::string coeffFile);
  double functionGF(double kl, double kt, double c2, double cg, double c2g, double A1, double A3, double A7);
  std::vector<std::string> tokenize(std::string input);
  std::pair<int,int> find2DBin(TH2* h, double x, double y);

  // adapted from ROOT to check histogram consistency
  bool CheckConsistency(const TH1* h1, const TH1* h2);
  bool CheckAxisLimits(const TAxis *a1, const TAxis *a2 );
  bool CheckBinLimits(const TAxis* a1, const TAxis * a2);

  // the coefficients of the reweight - read from the input file
  std::shared_ptr<TH2> h_A1_;
  std::shared_ptr<TH2> h_A3_;
  std::shared_ptr<TH2> h_A7_;
  std::shared_ptr<TH2> h_SM_;
  std::shared_ptr<TH2> h_sumv1_;

  double A1_13TeV_;
  double A3_13TeV_;
  double A7_13TeV_;

  // the distribution of the input events
  // must the all the generated events before preselections
  std::shared_ptr<TH2> h_input_;
};

#endif
