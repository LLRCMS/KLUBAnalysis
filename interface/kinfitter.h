#ifndef KINFITTER_H_
#define KINFITTER_H_

// C++
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <stdexcept>
#include <utility>

// ROOT
#include <Math/VectorUtil.h>
#include <Math/LorentzVector.h>
#include <Math/PtEtaPhiM4D.h>
#include <Math/PxPyPzM4D.h>
#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TLorentzVector.h>
#include <TMatrixD.h>
#include <TString.h>

// HHKinFit2
#include "../../HHKinFit2/HHKinFit2/include/HHKinFitMasterHeavyHiggs.h"

const int Z_MASS  = 91;  //GeV
const int H_MASS  = 125; //GeV

class KinFitter {
    // class to calculate KinFit mass of a particle given a mass hypothesis and its decay products
private:
    TLorentzVector tlv_l1 = TLorentzVector();
    TLorentzVector tlv_l2 = TLorentzVector();
    TLorentzVector tlv_b1 = TLorentzVector();
    TLorentzVector tlv_b2 = TLorentzVector();
    TVector2 ptmiss = TVector2();
    TMatrixD metcov = TMatrixD(2,2);
    double b1_JER = -1.0;
    double b2_JER = -1.0;

    HHKinFit2::HHKinFitMasterHeavyHiggs KinFit = HHKinFit2::HHKinFitMasterHeavyHiggs(tlv_b1, tlv_b2, tlv_l1, tlv_l2, ptmiss, metcov, b1_JER, b2_JER);
    bool fitsNotConverging = false;

    int mh1_hpFit;
    int mh2_hpFit;

    std::pair<HHKinFit2::HHKinFitMasterHeavyHiggs,float> _fit(int mh1_hp, int mh2_hp);

public:
    KinFitter(TLorentzVector tlv_firstBjet,
              TLorentzVector tlv_secondBjet,
              TLorentzVector tlv_firstLepton,
              TLorentzVector tlv_secondLepton,
              TVector2 ptmiss,
              TMatrixD stableMetCov,
              double bjet1_JER = -1.0,
              double bjet2_JER = -1.0);
    ~KinFitter();
    bool fit(TString sgnHp = "HH");
    double getMH();
    double getChi2();
    int getConvergence();
    double getFitProb();
    TLorentzVector getFittedBJet1();
    TLorentzVector getFittedBJet2();

};

#endif /* KINFITTER_H_ */