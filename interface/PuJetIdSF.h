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
//   Update: October 2020 - add up/down variations                                                                //
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

  float getSF     (float pt, float eta);
  float getSFError(float pt, float eta);

  std::vector<float> getEvtWeight (bigTree &theBigTree, TLorentzVector tau1, TLorentzVector tau2, std::map<int,double> jets_and_smearFactor);

private:
  TH2F* h_eff_SF_;
  TH2F* h_eff_SF_err_;
  
  std::string year_;
};

#endif // PuJetIdSF
