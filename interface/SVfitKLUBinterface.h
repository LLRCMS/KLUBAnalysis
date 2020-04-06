#ifndef SVfitKLUBinterface_h
#define SVfitKLUBinterface_h

// -------------------------------------------------------------------------------------------------------------- //
//                                                                                                                //
//   class SVfitKLUBinterface                                                                                     //
//                                                                                                                //
//   Class to compute ClassicSVfit. Based on:                                                                     //
//    - https://github.com/LLRCMS/LLRHiggsTauTau/blob/102X_HH/NtupleProducer/plugins/ClassicSVfitInterface.cc     //
//                                                                                                                //
//   Original SVfit package from:                                                                                 //
//    - https://github.com/SVfit/ClassicSVfit                                                                     //
//                                                                                                                //
//   Author: Francesco Brivio (Milano-Bicocca)                                                                    //
//   Date  : April 2020                                                                                           //
//                                                                                                                //
// -------------------------------------------------------------------------------------------------------------- //

// Standard libraries
#include <vector>
#include <string>
#include <cmath>

// ROOT libraries
#include <TLorentzVector.h>
#include "TMatrixD.h"

// ClassicSVfit libraries
#include "../../TauAnalysis/ClassicSVfit/interface/ClassicSVfit.h"
#include "../../TauAnalysis/ClassicSVfit/interface/ClassicSVfitIntegrand.h"
#include "../../TauAnalysis/ClassicSVfit/interface/MeasuredTauLepton.h"
#include "../../TauAnalysis/ClassicSVfit/interface/svFitHistogramAdapter.h"


// SVfitKLUBinterface class
class SVfitKLUBinterface {

  public:
    SVfitKLUBinterface (int verbosity, TLorentzVector tau1, TLorentzVector tau2, TLorentzVector met, TMatrixD met_cov, int pairType, int DM1, int DM2);
    ~SVfitKLUBinterface ();

    std::vector<double> FitAndGetResult();

  private:
    int verbosity_;
    std::vector<classic_svFit::MeasuredTauLepton> measuredTauLeptons_;
    double METx_;
    double METy_;
    TMatrixD covMET_;

};

#endif // SVfitKLUBinterface_h