#ifndef PuJetIdSF_h
#define PuJetIdSF_h

// -------------------------------------------------------------------------------------------------------------- //
//                                                                                                                //
//   class PuJetIdSF                                                                                              //
//                                                                                                                //
//   Class to compute Pileup jet ID SF.                                                                           //
//   Original recipe from:                                                                                        //
//    - https://twiki.cern.ch/twiki/bin/view/CMS/PileupJetID#Efficiencies_and_data_MC_scale_f                     //
//    - similar to the class 'bTagSF'                                                                             //
//                                                                                                                //
//   Author: Francesco Brivio (Milano-Bicocca)                                                                    //
//   Date  : May 2020                                                                                             //
//                                                                                                                //
// -------------------------------------------------------------------------------------------------------------- //

// Standard libraries
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>

// KLUB libraries
#include "bigTree.h"

// ROOT libraries
#include "TLorentzVector.h"
#include "TFile.h"
#include "TH2F.h"

// PuJetIdSF class
class PuJetIdSF {

  public:
    PuJetIdSF (std::string directory, std::string year);
    ~PuJetIdSF ();

    bool CheckBit(int number, int bitpos);

    float getEff(bool isReal, float pt, float eta);
    float getSF (bool isReal, float pt, float eta);

    float getEvtWeight (bigTree &theBigTree, TLorentzVector tau1, TLorentzVector tau2);

  private:
    TH2F* h_eff_;
    TH2F* h_eff_SF_;
    TH2F* h_mistag_;
    TH2F* h_mistag_SF_;
};

#endif // PuJetIdSF