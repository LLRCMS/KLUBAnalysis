#ifndef SmearedJetProducer_h
#define SmearedJetProducer_h

// -------------------------------------------------------------------------------------------------------------- //
//                                                                                                                //
//   class SmearedJetProducer                                                                                     //
//                                                                                                                //
//   Class to compute Smear the Jets.                                                                             //
//   Original recipe from:                                                                                        //
//    - https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution#Smearing_procedures                            //
//    - https://github.com/cms-sw/cmssw/blob/CMSSW_8_0_25/PhysicsTools/PatUtils/interface/SmearedJetProducerT.h   //
//                                                                                                                //
//   Author: Francesco Brivio (Milano-Bicocca)                                                                    //
//   Date  : January 2021                                                                                         //
//                                                                                                                //
// -------------------------------------------------------------------------------------------------------------- //

// Standard libraries
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <memory>
#include <random>

// KLUB libraries
#include "bigTree.h"

// ROOT libraries
#include "TLorentzVector.h"
#include "TFile.h"
#include "TH1F.h"

//CMSSW libraries
#include "JetMETCorrections/Modules/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetResolutionObject.h"

// SmearedJetProducer class
class SmearedJetProducer {

public:
  // Initialize the smearer with year, doSmear option and syst variation:
  //  0: nominal
  // +1: up
  // -1: down
  SmearedJetProducer (std::string year, bool doSmearing, int variation, bool isPostVFP = false);
  ~SmearedJetProducer ();

  // Returns the matched genJet
  TLorentzVector matchedGenJet(TLorentzVector jet, double resolution, bigTree & theBigTree);

  // Returns the smear factor to be applied to the jet TLorentzVector
  double getSmearFactor(TLorentzVector jet, bigTree & theBigTree,
						bool ptJetCut = true);

  // Returns the smeared jet starting from the original TLorentzVector
  TLorentzVector getSmearedJet(TLorentzVector jet, bigTree & theBigTree);

  // Returns the smeared jet starting from the jet index in bigTree
  TLorentzVector getSmearedJetFromIdx(unsigned int jetIdx, bigTree & theBigTree);

  // Returns the resolution
  double getResolution(unsigned int jetIdx, bigTree & theBigTree);

private:
  std::unique_ptr<JME::JetResolution> resolution_from_file_;
  std::unique_ptr<JME::JetResolutionScaleFactor> scale_factor_from_file_;
  double dR_max_;
  double dPt_max_factor_;
  bool enabled_;
  Variation systematic_variation_;
  std::mt19937_64 random_generator_;
  static constexpr const double MIN_JET_ENERGY = 1e-2;
};

#endif // SmearedJetProducer
