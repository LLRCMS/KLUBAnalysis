#ifndef JECKLUBinterface_h
#define JECKLUBinterface_h

// -------------------------------------------------------------------------------------------------------------- //
//                                                                                                                //
//   class JECKLUBinterface                                                                                       //
//                                                                                                                //
//   Class to read the JEC uncertainties from the txt files                                                       //
//   Reference:                                                                                                   //
//    - https://twiki.cern.ch/twiki/bin/view/CMS/JECUncertaintySources#Run_2_reduced_set_of_uncertainty           //
//   Original recipe from LLRHiggsTauTau/plugins/HTauTauNtuplizer.cc:                                             //
//    - https://github.com/LLRCMS/LLRHiggsTauTau/blob/102X_HH/NtupleProducer/plugins/HTauTauNtuplizer.cc          //
//                                                                                                                //
//   Author: Francesco Brivio (Milano-Bicocca)                                                                    //
//   Date  : Dec 2020                                                                                             //
//                                                                                                                //
// -------------------------------------------------------------------------------------------------------------- //

// Standard libraries
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <regex>

// KLUB libraries
#include "bigTree.h"

// ROOT libraries
#include "TLorentzVector.h"

//CMSSW libraries
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

// Map for JEC uncertainty sources
typedef std::map<std::string, std::unique_ptr<JetCorrectionUncertainty>> myJECMap;

// JECKLUBinterface class
class JECKLUBinterface {

public:
  JECKLUBinterface (std::string year, bool isPostVFP = false);
  ~JECKLUBinterface () {};

  // Returns a pair of vectors (up_variations vector, down_variations vector)
  // To be used in skimNtuple201*.cpp
  std::pair<std::vector<double>,std::vector<double>> getJECUncVectors(int idx, bigTree & theBigTree);

  // Returns the shifted TLorentzVector starting from "index of JEC source" and the original TLorentzVector
  // To be used (mainly) in skimOutputter*.cpp
  // unc = +1 --> up variation
  // unc = -1 --> down variation
  // TLorentzVector getJECUncJet(int idx, TLorentzVector nominalJet, float unc);

private:
  std::vector<std::string> m_jec_sources_regrouped_;
  myJECMap jecSourceUncRegroupedProviders_;
};

#endif // JECKLUBinterface
