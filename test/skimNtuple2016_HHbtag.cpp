// ntuple skimmer for analysis and synchronization

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <bitset>
#include <map>
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TBranch.h"
#include "TString.h"
#include "TLorentzVector.h"
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

// bigTree is produced on an existing ntuple as follows (see at the end of the file)
#include "bigTree.h"
#include "smallTree_HHbtag.h"
#include "OfflineProducerHelper.h"
#include "PUReweight.h"
#include "triggerReader_cross.h"
#include "bJetRegrVars.h"
#include "bTagSF.h"
#include "HHReweight5D.h"
#include "HHKinFit2/include/HHKinFitMasterHeavyHiggs.h"
#include "SFProvider.h"
#include "BDTfunctionsUtils.h"
#include "TauIDSFTool.h"
#include "skimUtils.h"
#include "PuJetIdSF.h"
#include "JECKLUBinterface.h"
#include "SVfitKLUBinterface.h"
#include "SmearedJetProducer.h"

#include "lester_mt2_bisect.h"

#include "ScaleFactor.h"
#include "ConfigParser.h"
#include "EffCounter.h"
#include "exceptions/HHInvMConstraintException.h"
#include "exceptions/HHEnergyRangeException.h"
#include "exceptions/HHEnergyConstraintException.h"

#include "TMVA/MsgLogger.h"
#include "TMVA/Config.h"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"

#include <boost/algorithm/string/replace.hpp>
#include <boost/format.hpp>
#include <Math/VectorUtil.h>
#include <Math/LorentzVector.h>
#include <Math/PxPyPzM4D.h>

// DNN
#include "cms_hh_tf_inference/inference/interface/inf_wrapper.hh"
#include "cms_hh_proc_interface/processing/interface/feat_comp.hh"
#include "cms_hh_proc_interface/processing/interface/evt_proc.hh"

// HHbtag
#include "HHbtagKLUBinterface.h"

// Multiclass
#include "../src/MulticlassInterface.cc"

using namespace std ;
using DNNVector = ROOT::Math::LorentzVector<ROOT::Math::PxPyPzM4D<float>>;

const double aTopRW = 0.0615;
const double bTopRW = -0.0005;

// Computed in 2019 for 2018 data with deepFlavor - NOT USED for Legacy
const float DYscale_LL[3] = {0.748154,2.15445,1.63619} ; // for now we use the same numbers computed with DY NLO sample
const float DYscale_MM[3] = {0.862686,1.08509,1.10947} ; // for now we use the same numbers computed with DY NLO sample


// Computed from PI group for DY LO binned for Legacy2016
// - number of b-jets [0b, 1b, 2b]
// - pT(MuMu)
//   - vLowPt  : <= 10 GeV
//   - LowPt   : >10 and <=50
//   - Med1Pt  : >50 and <=80
//   - Med2Pt  : >80 and <=110
//   - HighPt  : >110 and <=190
//   - vHighPt : >190
// Requiring an elliptical mass cut (relaxed by +5 GeV)
const float DYscale_MH_vLowPt [3] = {1.161, 0.515, 0.1  };
const float DYscale_MH_LowPt  [3] = {1.151, 1.042, 1.150};
const float DYscale_MH_Med1Pt [3] = {1.144, 1.152, 1.149};
const float DYscale_MH_Med2Pt [3] = {1.151, 1.333, 1.218};
const float DYscale_MH_HighPt [3] = {1.169, 1.458, 0.997};
const float DYscale_MH_vHighPt[3] = {1.061, 1.963, 1.185};

// Requiring M(mumu) > 50 GeV
//const float DYscale_MTT_vLowPt [3] = {1.195, 0.536, 0.1  };
//const float DYscale_MTT_LowPt  [3] = {1.17 , 1.046, 1.408};
//const float DYscale_MTT_Med1Pt [3] = {1.172, 1.128, 1.644};
//const float DYscale_MTT_Med2Pt [3] = {1.184, 1.182, 1.911};
//const float DYscale_MTT_HighPt [3] = {1.182, 1.476, 1.537};
//const float DYscale_MTT_vHighPt[3] = {1.092, 1.778, 0.934};

// Computed August 2020 - Requiring M(mumu) > 50 GeV
const float DYscale_MTT_vLowPt [3] = {1.1630144, 0.010000393, 0.010000000};
const float DYscale_MTT_LowPt  [3] = {1.2194740, 1.1250249  , 1.0609708  };
const float DYscale_MTT_Med1Pt [3] = {1.2536864, 1.2376837  , 1.1901911  };
const float DYscale_MTT_Med2Pt [3] = {1.2763251, 1.2972053  , 1.2731480  };
const float DYscale_MTT_HighPt [3] = {1.2785250, 1.4578434  , 1.2241989  };
const float DYscale_MTT_vHighPt[3] = {1.1649714, 1.6778047  , 1.1510545  };

const float DYscale_MTT_vLowPt_err [3] = {0.0037446320, 0.011371985, 0.0071346649};
const float DYscale_MTT_LowPt_err  [3] = {0.0017404799, 0.023737485, 0.035623816 };
const float DYscale_MTT_Med1Pt_err [3] = {0.0027420531, 0.034357252, 0.043622931 };
const float DYscale_MTT_Med2Pt_err [3] = {0.0042018892, 0.055512921, 0.061654929 };
const float DYscale_MTT_HighPt_err [3] = {0.0045670912, 0.065499641, 0.064115483 };
const float DYscale_MTT_vHighPt_err[3] = {0.0084286985, 0.17367880 , 0.13585326  };

/* NOTE ON THE COMPUTATION OF STITCH WEIGHTS:
** 'Old' approach:
** - to be updated at each production, using the number of processed events N_inclusive and N_njets for each sample
** - say f_i is the fraction of inclusive events in the i bin on njets (can be 2D nb-njet as well)
** = then sigma_i = f_i * sigmal_LO
** - stitchWeight (0jet) = f_0 / (f_0 * N_inclusive)
** - stitchWeight (njet) = f_n / (f_n * N_inclusive + N_njets)
**==============================================================
** new approach for NLO:
** 3 samples merged: Inclusive, Njet-sliced, pT-sliced.
** merging done according to cross-section from xsdb
** [0,50] GeV pT slice is buggy and not included in the stitching
** -> 2 dedicated scale factors are needed (for pT>50 and pT<50) to restore the total cross-section
*/
const float stitchWeights [2] = {1./2., 1./3.};

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


int main (int argc, char** argv)
{
  // read input file and cfg
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  if (argc < 26)
  {
    cerr << "missing input parameters : argc is: " << argc << endl ;
    cerr << "usage: " << argv[0]
	 << " inputFileNameList outputFileName crossSection isData configFile runHHKinFit"
	 << " xsecScale(stitch) HTMax(stitch) HTMin(stitch) isTTBar DY_Nbs TT_stitchType"
	 << " runMT2 isHHsignal NjetRequired(stitch) EFTbm order_rew uncertainty_rew cms_fake_rew kl_rew kt_rew c2_rew cg_rew c2g_rew susyModel" << endl ;
    return 1;
  }

  TString inputFile = argv[1] ;
  TString outputFile = argv[2] ;
  cout << "** INFO: inputFile  : " << inputFile << endl;
  cout << "** INFO: outputFile : " << outputFile << endl;

  float XS = atof (argv[3]) ;
  bool isMC = true;
  int isDatabuf = atoi (argv[4]);
  if (isDatabuf == 1)
  {
    isMC = false;
    XS = 1.;
  }
  cout << "** INFO: x-section: " << XS << endl;
  cout << "** INFO: is it MC?  " << isMC << endl;

  if (gConfigParser) return 1 ;
  gConfigParser = new ConfigParser () ;
  TString config ;
  config.Form ("%s",argv[5]) ;
  cout << "** INFO: reading config : " << config << endl;

  bool runHHKinFit = false;
  string opt7 (argv[6]);
  if (opt7 == "1") runHHKinFit = true;
  cout << "** INFO: running HHKinFit: " << runHHKinFit << endl;

  float xsecScale = 1.0;
  xsecScale = atof (argv[7]);
  cout << "** INFO: xsec scaled by: " << xsecScale << endl;
  XS = XS*xsecScale;

  float HTMax = -999.0;
  HTMax = atof(argv[8]);
  cout << "** INFO: removing HT < " << HTMax << " [-999 means no removal]" << endl;

  float HTMin = -999.0;
  HTMin = atof(argv[9]);
  cout << "** INFO: removing HT > " << HTMin << " [-999 means no removal]" << endl;

  int isTTBarI = atoi(argv[10]);
  bool isTTBar = (isTTBarI == 1) ? true : false;
  if (!isMC) isTTBar = false; // force it, you never know...
  cout << "** INFO: is this a TTbar sample? : " << isTTBar << endl;

  bool DY_Nbs = false; // run on genjets to count in DY samples the number of b jets
  bool DY_tostitch = false;
  int I_DY_Nbs = atoi(argv[11]);
  if (I_DY_Nbs == 1)
  {
    DY_Nbs = true;
    DY_tostitch = true; // FIXME!! this is ok only if we use jet binned samples
  }
  cout << "** INFO: loop on gen jet to do a b-based DY split? " << DY_Nbs << " " << DY_tostitch << endl;

  int TT_stitchType = atoi(argv[12]);
  if (!isTTBar) TT_stitchType = 0; // just force if not TT...
  cout << "** INFO: TT stitch type: " << TT_stitchType << " [0: no stitch , 1: fully had, 2: semilept t, 3: semilept tbar, 4: fully lept, 5: semilept all]" << endl;

  bool runMT2 = false;
  string opt14 (argv[13]);
  if (opt14 == "1") runMT2 = true;
  cout << "** INFO: running MT2: " << runMT2 << endl;

  bool isHHsignal = false;
  string opt15 (argv[14]);
  if (opt15 == "1") isHHsignal = true;
  cout << "** INFO: is HH signal: " << isHHsignal << endl;

  int NjetRequired = atoi(argv[15]);
  cout << "** INFO: requiring exactly " << NjetRequired << " outgoing partons [<0 for no cut on this]" << endl;

  // reweight file according to NLO differential reweighting procedure https://gitlab.cern.ch/hh/eft-benchmarks
  string EFTbm = argv[16];
  string order_rew = argv[17];
  string uncertainty_rew = "\"\"";
  if (argv[18] != string("0")) uncertainty_rew = argv[18];
  bool cms_fake_rew = false;
  string opt20 (argv[19]);
  if (opt20 == "1") cms_fake_rew = true;
  float kl_rew = atof(argv[20]);
  float kt_rew = atof(argv[21]);
  float c2_rew = atof(argv[22]);
  float cg_rew = atof(argv[23]);
  float c2g_rew = atof(argv[24]);
  cout << "** INFO: EFT reweighting asked for benchmark " << EFTbm << " at order " << order_rew << endl;
  if (c2_rew > -999.0) cout << "** INFO: EFT reweighting overridden with coplings kl=" << kl_rew << " ; kt=" << kt_rew << " ; c2=" << c2_rew << " ; cg=" << cg_rew << " ; c2g=" << c2g_rew << " at order " << order_rew << "[all -999 means no override; only c2!=-999 means only c2 overridden]" << endl;

  string susyModel = argv[25];
  cout << "** INFO: requesting SUSY model to be: -" << susyModel << "- [NOTSUSY: no request on this parameter]" << endl;

  // external weight file for PUreweight - sample per sample
  TString PUreweightFile = argv[26];
  cout << "** INFO: PU reweight external file: " << PUreweightFile << endl;

  int DY_nJets  = atoi(argv[27]);
  int DY_nBJets = atoi(argv[28]);
  cout << "** INFO: nJets/nBjets for DY bin weights: " << DY_nJets << " / " << DY_nBJets << endl;
  int isDYI = atoi(argv[29]);
  bool isDY = (isDYI == 1) ? true : false;

  // this value is read just for "consistency", but is never actually used,
  // since in 2016 there is no ttHToTauTau sample, hence this stitching is not needed
  bool isttHToNonBB = false;
  int isttHToNonBBI = atoi(argv[30]);
  if (isttHToNonBBI == 1) isttHToNonBB = true;
  cout << "** INFO: isttHToNonBB: " << isttHToNonBB << endl;

  bool isHHNLO = false;
  int isHHNLOI = atoi(argv[31]);
  if (isHHNLOI == 1) isHHNLO = true;
  cout << "** INFO: isHHNLO: " << isHHNLO << endl;


  bool isPostVFP = true;
  int isAPV = atoi(argv[32]);
  if(isAPV==1) isPostVFP = false;
  cout << "** INFO: isAPV: " << isAPV << endl;

  // ------------------  decide what to do for the reweight of HH samples
  enum HHrewTypeList {
    kNone    = 0, //no reweighting
    kDiffRew = 1, //differential reweight using EFT benchmarks
    kC2scan  = 2, //differential reweight for c2 scan
    kOverRew = 3  //differential reweight with manual override of the coupling's values
  };

  int HHrewType = kNone; // default is no reweight
  if (EFTbm != "none") HHrewType = kDiffRew;
  if (EFTbm == "c2scan") HHrewType = kC2scan;
  if (EFTbm == "manual") {
    HHrewType = kOverRew;
    if (kl_rew <= -990.0 or kt_rew <= -990.0 or c2_rew <= -990.0 or cg_rew <= -990.0 or c2g_rew <= -990.0) {
      cout << "ERROR! You requested the manual override of the coupling, but probably you forgot to set the couplings! Exiting!" << endl;
      return 1;
    }
  }
  if ( (kl_rew > -990.0 or kt_rew > -990.0 or cg_rew > -990.0 or c2g_rew > -990.0) and (HHrewType != kOverRew) ) // c2_rew not in this check as HHrewTypeList::kC2scan allows for reweighting only c2
  {
    cout << "ERROR! You requested reweighting of type " << HHrewType << " which is not manual, but you also set the couplings manually! Exiting!" << endl;
    return 1;
  }
  cout << "** INFO: HH reweight type requested is " << HHrewType << " [ 0: no reweight, 1: differential reweight for fixed benchmark, 2: differential reweight for c2 scan, 3: differential reweight with couplings manual override ]" << endl;


  // prepare variables needed throughout the code
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  if (! (gConfigParser->init (config)))
  {
    cout << ">>> parseConfigFile::Could not open configuration file " << config << endl ;
    return -1 ;
  }

  bool   beInclusive         = gConfigParser->readBoolOption   ("selections::beInclusive") ;
  bool   useDeepFlavor       = gConfigParser->readBoolOption   ("selections::useDeepFlavor") ;
  bool   onlyFinalChannels   = gConfigParser->readBoolOption   ("selections::onlyFinalChannels") ;
  float  PUjetID_WP          = gConfigParser->readFloatOption  ("parameters::PUjetIDWP") ;
  float  PFjetID_WP          = gConfigParser->readIntOption    ("parameters::PFjetIDWP") ;
  // int    saveOS              = gConfigParser->readIntOption    ("parameters::saveOS") ;
  float  lepCleaningCone     = gConfigParser->readFloatOption  ("parameters::lepCleaningCone") ;
  int    bChoiceFlag         = gConfigParser->readFloatOption  ("parameters::bChoiceFlag") ;
  int    PUReweight_MC       = gConfigParser->readFloatOption  ("parameters::PUReweightMC") ;
  int    PUReweight_target   = gConfigParser->readFloatOption  ("parameters::PUReweighttarget") ;
  string leptonSelectionFlag = gConfigParser->readStringOption ("parameters::lepSelections") ;
  int maxNjetsSaved          = gConfigParser->readIntOption    ("parameters::maxNjetsSaved") ;

  enum sortingStrategy {
    kLLRFramDefault = 0, // taking order from LLR framework <--> ordered by MVA ID
    kHTauTau = 1,        // using HTauTau of lowest iso on 1st candidate, including (A,B) and (B,A)
    kPtAndRawIso = 2     // order each pair leg by pt (ptA > ptB), then compare *raw* iso of first leg
  };

  int sortStrategyThTh = 0;
  if (gConfigParser->isDefined("parameters::pairStrategy"))
  {
    sortStrategyThTh = gConfigParser->readIntOption("parameters::pairStrategy");
  }
  cout << "** INFO: thth sorting strategy? [0: kLLRFramDefault, 1: kHTauTau, 2: kPtAndRawIso]" << sortStrategyThTh << endl;

  vector<ULong64_t> debugEvents;
  if (gConfigParser->isDefined("parameters::debugEvent"))
  {
    string stringEvent = gConfigParser->readStringOption("parameters::debugEvent");
    stringstream streamEvent(stringEvent);
    std::string line;
    cout << "** INFO: Debugging events:"<<endl;
    while(std::getline(streamEvent,line,'-'))
    {
      ULong64_t nevent = std::stoul(line);
      debugEvents.push_back(nevent);
      cout<<"         "<<nevent<<endl;
    }
  }

  vector<string> trigMuTau   =  (isMC ? gConfigParser->readStringListOption ("triggersMC::MuTau")  : gConfigParser->readStringListOption ("triggersData::MuTau")) ;
  vector<string> trigTauTau  =  (isMC ? gConfigParser->readStringListOption ("triggersMC::TauTau") : gConfigParser->readStringListOption ("triggersData::TauTau")) ;
  vector<string> trigEleTau  =  (isMC ? gConfigParser->readStringListOption ("triggersMC::EleTau") : gConfigParser->readStringListOption ("triggersData::EleTau")) ;
  // vector<string> trigEleMu   =  (isMC ? gConfigParser->readStringListOption ("triggersMC::EleMu")  : gConfigParser->readStringListOption ("triggersData::EleMu")) ;
  //I didn't store MuMu and I don't care for eleele
  vector<string> trigEleEle  =  (isMC ? gConfigParser->readStringListOption ("triggersMC::EleEle")  : gConfigParser->readStringListOption ("triggersData::EleEle")) ;
  vector<string> trigMuMu    =  (isMC ? gConfigParser->readStringListOption ("triggersMC::MuMu")  : gConfigParser->readStringListOption ("triggersData::MuMu")) ;
  // cross triggers for muTau and eleTau
  vector<string> crossTrigTauTau = (isMC ? gConfigParser->readStringListOption ("triggersMC::crossTauTau") : gConfigParser->readStringListOption ("triggersData::crossTauTau")) ;
  vector<string> crossTrigMuTau  = (isMC ? gConfigParser->readStringListOption ("triggersMC::crossMuTau")  : gConfigParser->readStringListOption ("triggersData::crossMuTau") ) ;
  vector<string> crossTrigEleTau = (isMC ? gConfigParser->readStringListOption ("triggersMC::crossEleTau") : gConfigParser->readStringListOption ("triggersData::crossEleTau")) ;
  vector<string> vbfTriggers     = (isMC ? gConfigParser->readStringListOption ("triggersMC::vbfTriggers") : gConfigParser->readStringListOption ("triggersData::vbfTriggers")) ;

  //// NEW TRIGGERS
  vector<string> trigMET  =  (isMC ? gConfigParser->readStringListOption ("triggersMC::METtriggers") : gConfigParser->readStringListOption ("triggersData::METtriggers")) ;
  vector<string> trigSingleTau  =  (isMC ? gConfigParser->readStringListOption ("triggersMC::SingleTau") : gConfigParser->readStringListOption ("triggersData::SingleTau")); // TODO

  // bool applyTriggers = isMC ? false : true; // true if ask triggerbit + matching, false if doing reweight
  //bool applyTriggers = isMC ? gConfigParser->readBoolOption ("parameters::applyTriggersMC") : true; // true if ask triggerbit + matching, false if doing reweight

  bool applyTriggers = gConfigParser->readBoolOption ("parameters::applyTriggersMC") ;

  // applyTriggers = true;
  cout << "** INFO: apply triggers? " << applyTriggers << " [ 0: reweight , 1: triggerbit+matching ]" << endl;
  if (applyTriggers)
  {
    cout << "** INFO: triggers used in the skim : " << endl;

    cout << "  @ MuTau" << endl; cout << "   --> ";
    for (unsigned int i = 0 ; i < trigMuTau.size(); i++) cout << "  " << trigMuTau.at(i);
    cout << endl;

    cout << "  @ EleTau" << endl; cout << "   --> ";
    for (unsigned int i = 0 ; i < trigEleTau.size(); i++) cout << "  " << trigEleTau.at(i);
    cout << endl;

    cout << "  @ TauTau" << endl; cout << "   --> ";
    for (unsigned int i = 0 ; i < trigTauTau.size(); i++) cout << "  " << trigTauTau.at(i);
    cout << endl;

    cout << "  @ crossMuTau" << endl; cout << "   --> ";
    for (unsigned int i = 0 ; i < crossTrigMuTau.size(); i++) cout << "  " << crossTrigMuTau.at(i);
    cout << endl;

    cout << "  @ crossEleTau" << endl; cout << "   --> ";
    for (unsigned int i = 0 ; i < crossTrigEleTau.size(); i++) cout << "  " << crossTrigEleTau.at(i);
    cout << endl;

    cout << "  @ crossTauTau" << endl; cout << "   --> ";
    for (unsigned int i = 0 ; i < crossTrigTauTau.size(); i++) cout << "  " << crossTrigTauTau.at(i);
    cout << endl;

    cout << "  @ vbfTriggers" << endl; cout << "   --> ";
    for (unsigned int i = 0 ; i < vbfTriggers.size(); i++) cout << "  " << vbfTriggers.at(i);
    cout << endl;

    ////NEW TRIGGERS
    cout << "  @ METtriggers" << endl; cout << "   --> ";
    for (unsigned int i = 0 ; i < trigMET.size(); i++) cout << "  " << trigMET.at(i);
    cout << endl;

    cout << "  @ SingleTau" << endl; cout << "   --> ";
    for (unsigned int i = 0 ; i < trigSingleTau.size(); i++) cout << "  " << trigSingleTau.at(i);
    cout << endl;
  }

  //string bRegrWeights("");
  //bool computeBregr = gConfigParser->readBoolOption ("bRegression::computeBregr");
  //if (computeBregr) bRegrWeights = gConfigParser->readStringOption("bRegression::weights");

  //cout << "** INFO: computing b jet regression? " << computeBregr << " with weights " << bRegrWeights << endl;

  // input and output setup
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;

  appendFromFileList (bigChain, inputFile);
  bigChain->SetCacheSize(0);
  bigTree theBigTree (bigChain) ;
  //Create a new file + a clone of old tree header. Do not copy events
  TFile * smallFile = new TFile (outputFile, "recreate") ;
  smallFile->cd () ;
  smallTree theSmallTree ("HTauTauTree") ;

  // for HHKinFit
  int hypo_mh1 = 125;
  int hypo_mh2 = 125;

  // for efficiencies
  float totalEvents = 0. ;
  float selectedEvents = 0. ;
  int totalNoWeightsEventsNum = 0 ;
  int selectedNoWeightsEventsNum = 0 ;

  // systematics
  int N_jecSources = 11; //jec sources
  int N_tauhDM = 4;	 //tauh DMs
  int N_tauhDM_EES = 2;  //tauh DMs with EES

  TH1F* hTriggers = getFirstFileHisto (inputFile);
  TH1F* hTauIDS = getFirstFileHisto (inputFile,false);

  //FRA new triggerReader_cross to take into account the usage of crossTriggers
  triggerReader_cross trigReader (hTriggers);
  trigReader.addTauTauTrigs (trigTauTau);
  trigReader.addMuTauTrigs  (trigMuTau);
  trigReader.addEleTauTrigs (trigEleTau);
  trigReader.addMuMuTrigs   (trigMuMu);
  trigReader.addEleEleTrigs (trigEleEle);

  // add crossTriggers
  trigReader.addTauTauCrossTrigs (crossTrigTauTau);
  trigReader.addMuTauCrossTrigs  (crossTrigMuTau);
  trigReader.addEleTauCrossTrigs (crossTrigEleTau);

  // add VBF triggers for jet matching
  trigReader.addVBFTrigs (vbfTriggers);

  ////NEW TRIGGERS
  trigReader.addMETTrigs (trigMET);
  trigReader.addSingleTauTrigs (trigSingleTau);

  // print full list (this is needed to identify the the triggers that fired in the bitwise variable)
  trigReader.printTriggerList();

  // ------------------------------

  OfflineProducerHelper oph (hTriggers, hTauIDS) ;

  // ------------------------------

  //bJetRegrVars bjrv;
  //TMVA::Reader *bRreader = new TMVA::Reader();
  //bjrv.setReader (bRreader);
  //string bRegrMethodName = "BDTG method";
  //if (computeBregr)
  //  bRreader->BookMVA( bRegrMethodName.c_str(), bRegrWeights.c_str() );

  // ------------------------------

  //PUReweight reweight (PUReweight::RUN2ANALYSIS); // NONE : no PU reweight (always returns 1) - RUN2ANALYSIS: get weights according to MC and data targets
  PUReweight reweight (PUReweight::RUN2ANALYSIS, PUreweightFile);

  // ------------------------------

  cout << "** INFO: useDepFlavor? " << useDeepFlavor << endl;

  string bTag_SFFile;
  string bTag_effFile;

  if(useDeepFlavor)
  {
    bTag_SFFile = gConfigParser->readStringOption("bTagScaleFactors::SFFileDeepFlavor");
    bTag_effFile = gConfigParser->readStringOption("bTagScaleFactors::effFileDeepFlavor");
  }
  else
  {
    bTag_SFFile = gConfigParser->readStringOption("bTagScaleFactors::SFFileDeepCSV");
    bTag_effFile = gConfigParser->readStringOption("bTagScaleFactors::effFileDeepCSV");
  }

  cout << "B Tag SF file: " << bTag_SFFile << endl;
  string wpset = isPostVFP? "106X16postVFP_DeepCSV_V1":"106X16preVFP_DeepCSV_V1";
  bTagSF bTagSFHelper (bTag_SFFile, bTag_effFile, "", "2016", wpset);
  if(useDeepFlavor) {
    wpset = isPostVFP? "106X16postVFP_DeepFlavor_V1":"106X16preVFP_DeepFlavor_V1";
    bTagSFHelper.SetWPset(wpset);
  }

  // ------------------------------

  std::string PUjetID_SF_directory = gConfigParser->readStringOption ("PUjetIDScaleFactors::files");
  cout << "** INFO: PU jet ID SF directory: " << PUjetID_SF_directory << std::endl;
  string puYear = isAPV ? "2016APV" : "2016";
  PuJetIdSF PUjetIDSFprovider(PUjetID_SF_directory, puYear);

  // ------------------------------

   
  JECKLUBinterface JECprovider("2016", isPostVFP);

  // ------------------------------

  // JER smearing provider: year - doSmearing - variation (0:nominal  +1:up  -1:down)
  bool doSmearing    = (isMC ? gConfigParser->readBoolOption("JetSmearing::doSmearing") : false);
  int smearVariation = (isMC ? gConfigParser->readIntOption("JetSmearing::smearVariation") : 0 );
  cout << "** INFO: doSmearing? " << doSmearing << endl;
  cout << "** INFO: smearVariation: " << smearVariation << " [0:nominal  +1:up  -1:down]" << endl;
  SmearedJetProducer Smearer("2016", doSmearing, smearVariation, isPostVFP);

  // ------------------------------

  // HHbtag model
  std::string HHbtag_model = gConfigParser->readStringOption ("HHbtag::weights");
  cout << "** INFO: HHbtag_model: " << HHbtag_model << endl;
  std::array<std::string, 2> models;
  for(size_t n = 0; n < 2; ++n)
  {
    std::ostringstream ss_model;
    ss_model <<HHbtag_model << n;
    models.at(n) = ss_model.str();
  }
  HHbtagKLUBinterface HHbtagTagger(models, 2016);

  // ------------------------------
  //tau legs trigger SF for data and mc
  //from: https://github.com/cms-tau-pog/TauTriggerSFs/tree/run2_SFs
  TString vfp = isPostVFP?"postVFP":"preVFP";
  tau_trigger::SFProvider * tauTrgSF_ditau = new tau_trigger::SFProvider("weights/trigger_SF_UL/2016"+vfp+"/2016UL"+vfp+"_tauTriggerEff_DeepTau2017v2p1.root", "ditau", "Medium");
  tau_trigger::SFProvider * tauTrgSF_mutau = new tau_trigger::SFProvider("weights/trigger_SF_UL/2016"+vfp+"/2016UL"+vfp+"_tauTriggerEff_DeepTau2017v2p1.root", "mutau", "Medium");
  //tau_trigger::SFProvider * tauTrgSF_etau  = new tau_trigger::SFProvider("weights/trigger_SF_Legacy/2016/2016_tauTriggerEff_DeepTau2017v2p1.root", "etau" , "Medium");



  /********************************************************************************************************************
   * Single-tau triggers SF (from: https://twiki.cern.ch/twiki/bin/view/CMS/TauTrigger#Run_II_Trigger_Scale_Factors): *
   * ________________________________________________________________________________________________________________ *
   * 														      *
   * TLDR: Default (flat) SF are provided, but safer to recompute them as potential impact from other tau trig OR     *
   * ________________________________________________________________________________________________________________ *
   * ________________________________________________________________________________________________________________ *
   *                                                                                                                  *
   * For the data/MC scale factors of the single tau trigger, the recommendation is to use a flat scale factor 	      *
   * in the region where the trigger is fully efficient. This region is defined by the transverse momentum of	      *
   * the hadronic tau lepton to be 10 GeV above the pT threshold of the trigger used in the respective era.	      *
   * 														      *
   * The threshold therefore is at 130 GeV in the 2016 data-taking period and at 190 GeV in the 2017 and 2018	      *
   * data-taking periods.											      *
   * 														      *
   * The measured data/MC scale factors are:									      *
   *														      *
   * Era 	Scale Factor 	Trigger Selection								      *
   * 2016 	0.88 ± 0.08 	(HLT_VLooseIsoPFTau120_Trk50_eta2p1_v OR HLT_VLooseIsoPFTau140_Trk50_eta2p1_v)	      *
   * 2017 	1.08 ± 0.10 	(HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v)			      *
   * 2018 	0.87 ± 0.11 	(HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v) 			      *
   *														      *
   * While the data/MC scale factors are only weakly dependent on the selection used in the measurement, the 	      *
   * efficiencies showed a non-negligible dependence on the used selection.					      *
   * In analyses that combine multiple triggers for hadronic tau leptons and therefore make use of the efficiencies   *
   * directly in the formula for the combined efficiency, it is thus recommended to derive the efficiency of the      *
   * single tau trigger for the analysis selection in MC and calculate the efficiency in data via 		      *
   * eff(data) = SF*eff(MC), where SF is the measured data/MC scale factor quoted above and eff(MC) denotes the       *
   * efficiency measured for the analysis selection.								      *
   *                                                                                                                  *
   ********************************************************************************************************************/


  // electron/muon leg trigger SF for data and mc
  ScaleFactor * muTauTrgSF = new ScaleFactor();
  //ScaleFactor * eTauTrgSF  = new ScaleFactor();
  ScaleFactor * muTrgSF    = new ScaleFactor();
  ScaleFactor * eTrgSF     = new ScaleFactor();

  muTauTrgSF->init_ScaleFactor("weights/trigger_SF_Legacy/2016/Muon_Mu19leg_2016BtoH_eff.root");
  muTrgSF   ->init_ScaleFactor("weights/trigger_SF_Legacy/2016/Muon_Run2016_legacy_IsoMu22.root");
  //eTauTrgSF ->init_ScaleFactor("weights/trigger_SF_Legacy/2016/Electron_Ele24_eff.root");
  eTrgSF    ->init_ScaleFactor("weights/trigger_SF_Legacy/2016/Electron_Run2016_legacy_Ele25.root");

  // ------------------------------

  // electron/muon IdAndIso SF
  ScaleFactor * myIDandISOScaleFactor[3]; // [0: mu, 1: ele]
  for (int i = 0 ; i < 3; i++)
    myIDandISOScaleFactor[i] = new ScaleFactor();

  std::string VFP = isPostVFP?"postVFP":"preVFP";

  myIDandISOScaleFactor[0] -> init_ScaleFactor(isPostVFP?"weights/MuPogSF_UL/2016/Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root":"weights/MuPogSF_UL/2016/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_ID.root",
                                               "NUM_TightID_DEN_TrackerMuons_abseta_pt",
                                               true);
  myIDandISOScaleFactor[1] -> init_EG_ScaleFactor(isPostVFP? "weights/EgammaPOGSF_UL/2016/egammaEffi.txt_Ele_wp80iso_postVFP_EGM2D.root":"weights/EgammaPOGSF_UL/2016/egammaEffi.txt_Ele_wp80iso_preVFP_EGM2D.root");
  myIDandISOScaleFactor[2] -> init_ScaleFactor(isPostVFP?"weights/MuPogSF_UL/2016/Efficiencies_muon_generalTracks_Z_Run2016_UL_ISO.root":"weights/MuPogSF_UL/2016/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_ISO.root",
                                               "NUM_TightRelIso_DEN_TightIDandIPCut_abseta_pt",
                                               true);
  // tau IdAndIso SF
  VFP = isPostVFP ? "UL2016_postVFP" : "UL2016_preVFP";
  TauIDSFTool *Deep_antiJet_medium_dm = new TauIDSFTool(VFP,"DeepTau2017v2p1VSjet","Medium",1); // for DeepTauv2p1 vs jets Medium
  TauIDSFTool *Deep_antiJet_medium_pt = new TauIDSFTool(VFP,"DeepTau2017v2p1VSjet","Medium",0); // for DeepTauv2p1 vs jets Medium
  TauIDSFTool *Deep_antiJet_2d        = new TauIDSFTool(VFP,"Medium","VVLoose"); // for DeepTauv2p1 vs jets Medium in DM/pT bins
  TauIDSFTool *Deep_antiEle_vvloose   = new TauIDSFTool(VFP,"DeepTau2017v2p1VSe","VVLoose",0);  // for DeepTauv2p1 vs ele VVLoose
  TauIDSFTool *Deep_antiMu_tight      = new TauIDSFTool(VFP, "DeepTau2017v2p1VSmu","Tight",0);  // for DeepTauv2p1 vs mu Tight

  // ------------------------------
  // reweight file according to NLO differential reweighting procedure
  // there is a unique input map, read it from the cfg file
  // HHReweight* hhreweighter = nullptr;
  HHReweight5D* hhreweighter = nullptr;
  TH2* hhreweighterInputMap = nullptr;
  if (HHrewType != kNone)
  {
    string inMapFile   = gConfigParser->readStringOption("HHReweight::inputFile");
    string inHistoName = gConfigParser->readStringOption("HHReweight::histoName");
    string coeffFile   = gConfigParser->readStringOption("HHReweight::coeffFileNLO");
    if (order_rew == "lo") coeffFile    = gConfigParser->readStringOption("HHReweight::coeffFileLO");
    cout << "** INFO: reading histo named: " << inHistoName << " from file: " << inMapFile << endl;
    cout << "** INFO: HH reweight coefficient file is: " << coeffFile << endl;
    TFile* fHHDiffRew = new TFile(inMapFile.c_str());
    hhreweighterInputMap = (TH2*) fHHDiffRew->Get(inHistoName.c_str());
    hhreweighter = new HHReweight5D(coeffFile, hhreweighterInputMap, EFTbm, string("2016"), order_rew, uncertainty_rew, cms_fake_rew);
  }

  // ------------------------------
  // indexes of tau ID bits

  map <string, int> tauIDsMap;
  for (int ibin = 0; ibin < hTauIDS->GetNbinsX(); ++ibin)
  {
    if (string(hTauIDS->GetXaxis()->GetBinLabel(ibin+1)) == string("byLooseCombinedIsolationDeltaBetaCorr3Hits") )
      tauIDsMap ["byLooseCombinedIsolationDeltaBetaCorr3Hits"] = ibin ;

    if (string(hTauIDS->GetXaxis()->GetBinLabel(ibin+1)) == string("byMediumCombinedIsolationDeltaBetaCorr3Hits") )
      tauIDsMap ["byLooseCombinedIsolationDeltaBetaCorr3Hits"] = ibin ;

    if (string(hTauIDS->GetXaxis()->GetBinLabel(ibin+1)) == string("byTightCombinedIsolationDeltaBetaCorr3Hits") )
      tauIDsMap ["byLooseCombinedIsolationDeltaBetaCorr3Hits"] = ibin ;
  }

  // cut based tau ID
  vector<int> tauCUTIDIdx;
  tauCUTIDIdx.push_back(getTauIDIdx(hTauIDS, "byLooseCombinedIsolationDeltaBetaCorr3Hits"));
  tauCUTIDIdx.push_back(getTauIDIdx(hTauIDS, "byMediumCombinedIsolationDeltaBetaCorr3Hits"));
  tauCUTIDIdx.push_back(getTauIDIdx(hTauIDS, "byTightCombinedIsolationDeltaBetaCorr3Hits"));
  if (find(tauCUTIDIdx.begin(), tauCUTIDIdx.end(), -1) != tauCUTIDIdx.end())
  {
    cout << "** WARNING!! did not found some cut-based tau IDs" << endl;
    for (unsigned int i = 0; i < tauCUTIDIdx.size(); ++i)
      cout << tauCUTIDIdx.at(i) << " " ;
    cout << endl;
  }

  // DNN Tau ID vs jet
  vector<int> deepTauVsJetIdx;
  deepTauVsJetIdx.push_back(getTauIDIdx(hTauIDS, "byVVVLooseDeepTau2017v2p1VSjet"));
  deepTauVsJetIdx.push_back(getTauIDIdx(hTauIDS, "byVVLooseDeepTau2017v2p1VSjet"));
  deepTauVsJetIdx.push_back(getTauIDIdx(hTauIDS, "byVLooseDeepTau2017v2p1VSjet"));
  deepTauVsJetIdx.push_back(getTauIDIdx(hTauIDS, "byLooseDeepTau2017v2p1VSjet"));
  deepTauVsJetIdx.push_back(getTauIDIdx(hTauIDS, "byMediumDeepTau2017v2p1VSjet"));
  deepTauVsJetIdx.push_back(getTauIDIdx(hTauIDS, "byTightDeepTau2017v2p1VSjet"));
  deepTauVsJetIdx.push_back(getTauIDIdx(hTauIDS, "byVTightDeepTau2017v2p1VSjet"));
  deepTauVsJetIdx.push_back(getTauIDIdx(hTauIDS, "byVVTightDeepTau2017v2p1VSjet"));
  if (find(deepTauVsJetIdx.begin(), deepTauVsJetIdx.end(), -1) != deepTauVsJetIdx.end())
  {
    cout << "** WARNING!! did not found some cut-based tau IDs" << endl;
    for (unsigned int i = 0; i < deepTauVsJetIdx.size(); ++i)
      cout << deepTauVsJetIdx.at(i) << " " ;
    cout << endl;
  }

  // DNN Tau ID vs ele
  vector<int> deepTauVsEleIdx;
  deepTauVsEleIdx.push_back(getTauIDIdx(hTauIDS, "byVVVLooseDeepTau2017v2p1VSe"));
  deepTauVsEleIdx.push_back(getTauIDIdx(hTauIDS, "byVVLooseDeepTau2017v2p1VSe"));
  deepTauVsEleIdx.push_back(getTauIDIdx(hTauIDS, "byVLooseDeepTau2017v2p1VSe"));
  deepTauVsEleIdx.push_back(getTauIDIdx(hTauIDS, "byLooseDeepTau2017v2p1VSe"));
  deepTauVsEleIdx.push_back(getTauIDIdx(hTauIDS, "byMediumDeepTau2017v2p1VSe"));
  deepTauVsEleIdx.push_back(getTauIDIdx(hTauIDS, "byTightDeepTau2017v2p1VSe"));
  deepTauVsEleIdx.push_back(getTauIDIdx(hTauIDS, "byVTightDeepTau2017v2p1VSe"));
  deepTauVsEleIdx.push_back(getTauIDIdx(hTauIDS, "byVVTightDeepTau2017v2p1VSe"));
  if (find(deepTauVsEleIdx.begin(), deepTauVsEleIdx.end(), -1) != deepTauVsEleIdx.end())
  {
    cout << "** WARNING!! did not found some cut-based tau IDs" << endl;
    for (unsigned int i = 0; i < deepTauVsEleIdx.size(); ++i)
      cout << deepTauVsEleIdx.at(i) << " " ;
    cout << endl;
  }

  // DNN Tau ID vs mu
  vector<int> deepTauVsMuIdx;
  deepTauVsMuIdx.push_back(getTauIDIdx(hTauIDS, "byVLooseDeepTau2017v2p1VSmu"));
  deepTauVsMuIdx.push_back(getTauIDIdx(hTauIDS, "byLooseDeepTau2017v2p1VSmu"));
  deepTauVsMuIdx.push_back(getTauIDIdx(hTauIDS, "byMediumDeepTau2017v2p1VSmu"));
  deepTauVsMuIdx.push_back(getTauIDIdx(hTauIDS, "byTightDeepTau2017v2p1VSmu"));
  if (find(deepTauVsMuIdx.begin(), deepTauVsMuIdx.end(), -1) != deepTauVsMuIdx.end())
  {
    cout << "** WARNING!! did not found some cut-based tau IDs" << endl;
    for (unsigned int i = 0; i < deepTauVsMuIdx.size(); ++i)
      cout << deepTauVsMuIdx.at(i) << " " ;
    cout << endl;
  }

  // -----------------------------------
  // event counters for efficiency study
  EffCounter ec;
  ec.AddMarker ("all");
  ec.AddMarker ("METfilter");
  ec.AddMarker ("NoBadMuons");
  ec.AddMarker ("PairExists");
  ec.AddMarker ("PairFoundBaseline");
  ec.AddMarker ("Trigger");
  ec.AddMarker ("TwoJets");

  // for hh signal only -- split by gen decay
  EffCounter* ecHHsig;
  if (isHHsignal)
  {
    ecHHsig = new EffCounter[6];
    for (int ic = 0; ic < 6; ++ic)
    {
      ecHHsig[ic].AddMarker ("all");
      ecHHsig[ic].AddMarker ("METfilter");
      ecHHsig[ic].AddMarker ("NoBadMuons");
      ecHHsig[ic].AddMarker ("PairExists");
      ecHHsig[ic].AddMarker ("PairFoundBaseline");
      ecHHsig[ic].AddMarker ("PairMatchesGen");
      ecHHsig[ic].AddMarker ("Trigger");
      ecHHsig[ic].AddMarker ("TwoJets");
    }
  }


  // loop over events
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  for (Long64_t iEvent = 0 ; true ; ++iEvent)
  {
    if (iEvent % 10000 == 0)  cout << "- reading event " << iEvent << endl ;
    //if (iEvent == 20000)  break ;
    //cout << "-------- reading event " << iEvent << endl ;
    theSmallTree.clearVars () ;

    int got = theBigTree.fChain->GetEntry(iEvent);

    if (got == 0) break;
    bool DEBUG = false;

    if (std::find(debugEvents.begin(), debugEvents.end(), theBigTree.EventNumber) != debugEvents.end())
    {
      cout << "****** DEBUG : debugging event=" << theBigTree.EventNumber << " run=" << theBigTree.RunNumber << " lumi=" << theBigTree.lumi << " (entry number=" << iEvent << ")" << endl;
      DEBUG = true;
    }
    if (debugEvents.size() > 0 && DEBUG == false) continue;

    // directly reject events outside HT range in case of stitching of inclusive sample-- they should not count in weights
    //cout << " ********** HTMAX - MIN - LHE: " << HTMax << " - " << HTMin << " - " << theBigTree.lheHt << endl;
    if (HTMax > 0)
    {
      if (theBigTree.lheHt > HTMax) continue;
    }

    if (HTMin > 0)
    {
      if (theBigTree.lheHt < HTMin) continue;
    }

    // directly reject events I want to remove in W+Jets stitching on njets
    if (NjetRequired >= 0)
    {
      if (theBigTree.lheNOutPartons != NjetRequired) continue;
    }

    // For ggHH NLO samples only: if the genWeight is > 0.5 the event should be rejected
    if (isHHNLO)
    {
      if ( abs(theBigTree.aMCatNLOweight) > 0.5 ) continue;
    } // end isHHNLO

      // skip event if I want a specific SUSY point from the fastsim
      //if (susyModel != string("NOTSUSY"))
      //{
      //  if (string(theBigTree.susyModel.Data()) != susyModel) continue;
      //}

	float stitchWeight = 1.;
	if (DY_tostitch)
	  {
		float Zpt = theBigTree.lheVPt;
		if (Zpt==0.0)
		  stitchWeight = stitchWeights[0];
		else
		  stitchWeight = stitchWeights[1];
	  }

    // gen info -- fetch tt pair and compute top PT reweight
    float topPtReweight = 1.0; // 1 for all the other samples
    theSmallTree.m_TTtopPtreweight =  1.0 ;
    theSmallTree.m_TTtopPtreweight_up =  1.0 ;
    theSmallTree.m_TTtopPtreweight_down =  1.0 ;
    if (isTTBar)
    {
      float ptTop1 = -1.0;
      float ptTop2 = -1.0;
      int decayTop1 = -999;
      int decayTop2 = -999;
      int pdgIdTop1 = -999;
      int pdgIdTop2 = -999;

      for (unsigned int igen = 0; igen < theBigTree.genpart_pdg->size(); igen++)
      {
	int pdg = theBigTree.genpart_pdg->at(igen);
	int genflags = theBigTree.genpart_flags->at(igen);
	//bool isFirst = CheckBit (genflags, 12);  //if (fl.isFirstCopy()) flags |= (1 << 12);
	// From: https://twiki.cern.ch/twiki/bin/view/CMS/TopPtReweighting?rev=30#How_to_practically_apply_default
	bool isLast = CheckBit (genflags, 13);  //if (fl.isLastCopy()) flags |= (1 << 13);
	//int topDM = theBigTree.genpart_TopDecayMode->at(igen);

	if (abs(pdg) == 6 && isLast) // top -- pt reweight (derived after radiation and before decay)
	{
	  TLorentzVector TopV;
	  TopV.SetPxPyPzE (theBigTree.genpart_px->at(igen), theBigTree.genpart_py->at(igen), theBigTree.genpart_pz->at(igen), theBigTree.genpart_e->at(igen) ) ;
	  if (ptTop1 < 0)
	  {
	    ptTop1 = TopV.Pt();
	    decayTop1 = theBigTree.genpart_TopDecayMode->at(igen);
	    pdgIdTop1 = theBigTree.genpart_pdg->at(igen);
	  }
	  else if (ptTop2 < 0)
	  {
	    ptTop2 = TopV.Pt();
	    decayTop2 = theBigTree.genpart_TopDecayMode->at(igen);
	    pdgIdTop2 = theBigTree.genpart_pdg->at(igen);
	    break;
	  } // check done in paralles shows that I never have > 2 top .  break is safe .
	  // else cout << " !! skim warning: sample is declared as as ttbar, but I have > 2 gen top in the event! " << endl;
	}
      }
      if (ptTop1 < 0 || ptTop2 < 0)
      {
	cout << "** WARNING: sample is declared as TTbar but in the event I didn't find 2 tops (1,2) :" << ptTop1 << " " << ptTop2 << endl;
      }
      else
      {
	// filter by decay mode if needed for stitching
	// [0: no stitch , 1: fully had, 2: semilept t, 3: semilept tbar, 4: fully lept, 5: semilept all]
	// TopDecayMode: 0: Had, 1-5: leptonic, 6: other -- consider "other" as a possible hadronic decay (includes rare W->bc)

	bool isT1Lept = (decayTop1 >= 1 && decayTop1 <= 5) ;
	bool isT2Lept = (decayTop2 >= 1 && decayTop2 <= 5) ;
	if (pdgIdTop1 / pdgIdTop2 > 0) cout << "** WARNING: I found two tops with the same sign " << pdgIdTop1 << " " << pdgIdTop2 << endl;

	// cout << "DEBUG: event with " << pdgIdTop1 << "-->  " << decayTop1 << " | " << pdgIdTop2 << "--> " << decayTop2 << " SKIMTYPE=" << TT_stitchType << endl;
	switch (TT_stitchType)
	{
	case 0:
	  break; // no stitching

	case 1:
	  if (isT1Lept || isT2Lept) continue; // fully had
	  break;

	case 2: // top leptonic, antitop hadronic
	  if (pdgIdTop1 > 0) // 1 is top
	  {
	    if (! (isT1Lept && !isT2Lept) ) continue;
	  }
	  else // 1 is antitop
	  {
	    if (! (isT2Lept && !isT1Lept) ) continue;
	  }
	  break;

	case 3:
	  if (pdgIdTop1 > 0) // 1 is top
	  {
	    if (! (!isT1Lept && isT2Lept) ) continue;
	  }
	  else // 1 is antitop
	  {
	    if (! (!isT2Lept && isT1Lept) ) continue;
	  }
	  break;

	case 4:
	  if (!isT1Lept || !isT2Lept) continue;
	  break;

	case 5:
	  if (isT1Lept == isT2Lept) continue; // must be one had and the other lep, not equal
	  break;

	default:
	  cout << "** WARNING: unknown TT stytch type " << TT_stitchType << endl;
	}

	// cout << "WAS ACCEPTED" << endl;

	if(DEBUG)
	{
	  cout << "@ TOP pt reweight: " << endl;
	  cout << "  top1 pt=" << ptTop1 << endl;
	  cout << "  top2 pt=" << ptTop2 << endl;
	}

	float SFTop1 = TMath::Exp(aTopRW+bTopRW*ptTop1);
	float SFTop2 = TMath::Exp(aTopRW+bTopRW*ptTop2);
	topPtReweight = TMath::Sqrt (SFTop1*SFTop2);

	// old recipe
	// theSmallTree.m_TTtopPtreweight      = topPtReweight ;
	// theSmallTree.m_TTtopPtreweight_up   = topPtReweight*topPtReweight ;
	// theSmallTree.m_TTtopPtreweight_down = 1.0 ;

	// new recipe
	theSmallTree.m_TTtopPtreweight      = 1.0; // nominal has no weight
	theSmallTree.m_TTtopPtreweight_up   = topPtReweight; // pt rew to be used as a syst
	theSmallTree.m_TTtopPtreweight_down = 1.0 ; // and down unused

	theSmallTree.m_genDecMode1 = decayTop1;
	theSmallTree.m_genDecMode2 = decayTop2;
      }
    }

    // For Drell-Yan only: loop on genjets and count how many are there with 0,1,2 b
    // 0: 0bjet, 1: 1 b jet, 2: >= 2 b jet
    theSmallTree.m_DYscale_LL  = 1.0; // all the other MC samples + data have no weight
    theSmallTree.m_DYscale_MM  = 1.0;
    theSmallTree.m_DYscale_MH  = 1.0; // to be used with elliptical mass cut
    theSmallTree.m_DYscale_MTT = 1.0; // to be used with M(tautau) > 50 GeV
    theSmallTree.m_DYscale_MTT_up   = 1.0; // up variation of DYscale_MTT (val+variation)
    theSmallTree.m_DYscale_MTT_down = 1.0; // down variation of DYscale_MTT (val-variation)

    if (isMC && isDY) //to be done both for DY NLO and DY in jet bins
    {
      TLorentzVector vgj;
      int nbs = 0;
      for (unsigned int igj = 0; igj < theBigTree.genjet_px->size(); igj++)
      {
	vgj.SetPxPyPzE(theBigTree.genjet_px->at(igj), theBigTree.genjet_py->at(igj), theBigTree.genjet_pz->at(igj), theBigTree.genjet_e->at(igj));
	if (vgj.Pt() > 20 && TMath::Abs(vgj.Eta()) < 2.5)
	{
	  int theFlav = theBigTree.genjet_hadronFlavour->at(igj);
	  if (abs(theFlav) == 5) nbs++;
	}

	if(DEBUG)
	{
	  cout << " -- gen jet : " << igj << " pt=" << vgj.Pt() << " eta=" << vgj.Eta() <<  " hadFlav=" << theBigTree.genjet_hadronFlavour->at(igj) << endl;
	}
      }
      if (nbs > 2) nbs = 2;

      theSmallTree.m_nBhadrons = nbs;
      theSmallTree.m_DYscale_LL = DYscale_LL[nbs];
      theSmallTree.m_DYscale_MM = DYscale_MM[nbs];

      // Get LHE nBPartons
      int n_bJets = theBigTree.lheNOutB ;
      if (n_bJets>=2) n_bJets=2; // Make sure that events with n_bJets == 3 are included in the n_bJets == 2 case

      // loop through gen parts ot identify Z boson
      int idx1 = -1;
      for (unsigned int igen = 0; igen < theBigTree.genpart_px->size(); igen++)
      {
	bool isLast   = CheckBit(theBigTree.genpart_flags->at(igen), 13) ; // 13 = isLastCopy
	bool isPrompt = CheckBit(theBigTree.genpart_flags->at(igen),  0) ; //  0 = isPrompt
	if (theBigTree.genpart_pdg->at(igen) == 23 && isLast && isPrompt) // Z0 + isLast + isPrompt
	{
	  idx1 = igen;
	}
      }

      // if found, Build the genZ TLorentzVector
      float genZ_pt = -999.;
      if (idx1 >= 0)
      {
	// store gen decay mode of the Z identified
	theSmallTree.m_genDecMode1 = theBigTree.genpart_HZDecayMode->at(idx1);

	// build the genZ TLorentzVector
	TLorentzVector genZ;
	genZ.SetPxPyPzE(theBigTree.genpart_px->at(idx1), theBigTree.genpart_py->at(idx1), theBigTree.genpart_pz->at(idx1), theBigTree.genpart_e->at(idx1));
	genZ_pt = genZ.Pt();
	theSmallTree.m_genZ_pt = genZ_pt;

	// Save DY LO weights according to nbs and pT(Z)
	if (genZ_pt <= 10.)
	{
	  theSmallTree.m_DYscale_MH  = DYscale_MH_vLowPt [n_bJets];
	  theSmallTree.m_DYscale_MTT = DYscale_MTT_vLowPt[n_bJets];
	  theSmallTree.m_DYscale_MTT_up   = DYscale_MTT_vLowPt[n_bJets] + DYscale_MTT_vLowPt_err[n_bJets];
	  theSmallTree.m_DYscale_MTT_down = DYscale_MTT_vLowPt[n_bJets] - DYscale_MTT_vLowPt_err[n_bJets];
	}
	else if (genZ_pt > 10. && genZ_pt <= 50.)
	{
	  theSmallTree.m_DYscale_MH  = DYscale_MH_LowPt [n_bJets];
	  theSmallTree.m_DYscale_MTT = DYscale_MTT_LowPt[n_bJets];
	  theSmallTree.m_DYscale_MTT_up   = DYscale_MTT_LowPt[n_bJets] + DYscale_MTT_LowPt_err[n_bJets];
	  theSmallTree.m_DYscale_MTT_down = DYscale_MTT_LowPt[n_bJets] - DYscale_MTT_LowPt_err[n_bJets];
	}
	else if (genZ_pt > 50. && genZ_pt <= 80.)
	{
	  theSmallTree.m_DYscale_MH  = DYscale_MH_Med1Pt [n_bJets];
	  theSmallTree.m_DYscale_MTT = DYscale_MTT_Med1Pt[n_bJets];
	  theSmallTree.m_DYscale_MTT_up   = DYscale_MTT_Med1Pt[n_bJets] + DYscale_MTT_Med1Pt_err[n_bJets];
	  theSmallTree.m_DYscale_MTT_down = DYscale_MTT_Med1Pt[n_bJets] - DYscale_MTT_Med1Pt_err[n_bJets];
	}
	else if (genZ_pt > 80. && genZ_pt <= 110.)
	{
	  theSmallTree.m_DYscale_MH  = DYscale_MH_Med2Pt [n_bJets];
	  theSmallTree.m_DYscale_MTT = DYscale_MTT_Med2Pt[n_bJets];
	  theSmallTree.m_DYscale_MTT_up   = DYscale_MTT_Med2Pt[n_bJets] + DYscale_MTT_Med2Pt_err[n_bJets];
	  theSmallTree.m_DYscale_MTT_down = DYscale_MTT_Med2Pt[n_bJets] - DYscale_MTT_Med2Pt_err[n_bJets];
	}
	else if (genZ_pt > 110. && genZ_pt <= 190.)
	{
	  theSmallTree.m_DYscale_MH  = DYscale_MH_HighPt [n_bJets];
	  theSmallTree.m_DYscale_MTT = DYscale_MTT_HighPt[n_bJets];
	  theSmallTree.m_DYscale_MTT_up   = DYscale_MTT_HighPt[n_bJets] + DYscale_MTT_HighPt_err[n_bJets];
	  theSmallTree.m_DYscale_MTT_down = DYscale_MTT_HighPt[n_bJets] - DYscale_MTT_HighPt_err[n_bJets];
	}
	else /* pT(Z)>=190. */
	{
	  theSmallTree.m_DYscale_MH  = DYscale_MH_vHighPt [n_bJets];
	  theSmallTree.m_DYscale_MTT = DYscale_MTT_vHighPt[n_bJets];
	  theSmallTree.m_DYscale_MTT_up   = DYscale_MTT_vHighPt[n_bJets] + DYscale_MTT_vHighPt_err[n_bJets];
	  theSmallTree.m_DYscale_MTT_down = DYscale_MTT_vHighPt[n_bJets] - DYscale_MTT_vHighPt_err[n_bJets];
	}
      }

      else // genZ not found: use the lowest pT bin
      {
	theSmallTree.m_DYscale_MH  = DYscale_MH_vLowPt [n_bJets];
	theSmallTree.m_DYscale_MTT = DYscale_MTT_vLowPt[n_bJets];
	theSmallTree.m_DYscale_MTT_up   = DYscale_MTT_vLowPt[n_bJets] + DYscale_MTT_vLowPt_err[n_bJets];
	theSmallTree.m_DYscale_MTT_down = DYscale_MTT_vLowPt[n_bJets] - DYscale_MTT_vLowPt_err[n_bJets];
      }

      // Debug printout
      if(DEBUG)
      {
	cout << "------- DY reweight ------" << endl;
	cout << " - nbs  : " << nbs << endl;
	cout << " - pT(Z): " << genZ_pt << endl;
	cout << " - DYscale_MM  : " << theSmallTree.m_DYscale_MM << endl;
	cout << " - DYscale_MH  : " << theSmallTree.m_DYscale_MH << endl;
	cout << " - DYscale_MTT : " << theSmallTree.m_DYscale_MTT << endl;
	cout << "--------------------------" << endl;
      }
    }

    // HH reweight for non resonant
    float HHweight = 1.0;
    TLorentzVector vHardScatter1;
    TLorentzVector vHardScatter2;
    int idx1hs = -1; // hard scatted product
    int idx2hs = -1;
    int pdg1hs = -999;
    int pdg2hs = -999;
    int t1hs = -1;
    int t2hs = -1;

    int idx1hs_b = -1;     // bjet-1 index     // FRA DEBUG
    int idx2hs_b = -1;     // bjet-2 index
    TLorentzVector vGenB1; // bjet-1 tlv
    TLorentzVector vGenB2; // bjet-2 tlv
    if (isHHsignal || HHrewType != kNone) // isHHsignal: only to do loop on genparts, but no rew
    {
      // cout << "DEBUG: reweight!!!" << endl;
      TLorentzVector vH1, vH2, vBoost, vSum;
      float mHH = -1;
      float ct1 = -999;
      // loop on gen to find Higgs
      int idx1 = -1;
      int idx2 = -1;
      int idx1last = -1;
      int idx2last = -1;
      // cout << " ------------------------ " << endl;
      for (unsigned int igen = 0; igen < theBigTree.genpart_px->size(); igen++)
      {
	bool isFirst     = CheckBit (theBigTree.genpart_flags->at(igen), 12) ; // 12 = isFirstCopy
	bool isLast      = CheckBit (theBigTree.genpart_flags->at(igen), 13) ; // 13 = isLastCopy
	bool isHardScatt = CheckBit (theBigTree.genpart_flags->at(igen), 5) ; //   3 = isPromptTauDecayProduct
	bool isHardProcess = CheckBit (theBigTree.genpart_flags->at(igen), 7) ; //  7 = isHardProcess, for b coming from H
	// bool isDirectPromptTauDecayProduct = CheckBit (theBigTree.genpart_flags->at(igen), 5) ; // 5 = isDirectPromptTauDecayProduct
	int pdg = theBigTree.genpart_pdg->at(igen);
	int mothIdx = theBigTree.genpart_TauMothInd->at(igen);

	bool mothIsHardScatt = false;
	if (mothIdx > -1)
	{
	  bool mothIsLast =  CheckBit(theBigTree.genpart_flags->at(mothIdx), 13);
	  // NB: I need t ask that the mother is last idx, otherwise I get a nonphysics "tauh" by the tauh builder function from the tau->tau "decay" in pythia
	  mothIsHardScatt = (mothIsLast && CheckBit (theBigTree.genpart_flags->at(mothIdx), 8)); // 0 = isPrompt(), 7: hardProcess , 8: fromHardProcess
	}

	// if (abs(pdg) == 11 || abs(pdg) == 13 || abs(pdg) == 15 || abs(pdg) == 66615)
	// {
	//   bitset<32> bs (theBigTree.genpart_flags->at(igen)) ;
	//   cout << "/// igen = " << igen << " pdgId " << pdg << " flag=" << bs << " mothidx=" <<  theBigTree.genpart_TauMothInd->at(igen) << " px=" << theBigTree.genpart_px->at(igen) << endl;
	//   // cout << "/// igen = " << igen << " pdgId " << pdg << " isFirst=" << isFirst << " isLast=" << isLast << " isHardScatt=" << isHardScatt << " mothIsHardScatt=" << mothIsHardScatt << " isDirectPromptTauDecayProduct=" << isDirectPromptTauDecayProduct << " mothIdx=" << theBigTree.genpart_TauMothInd->at(igen) << endl;
	// }


	if (abs(pdg) == 25)
	{
	  // cout << igen << " H boson: Px " << theBigTree.genpart_px->at(igen) << " first? " << isFirst << " decMode : " << theBigTree.genpart_HZDecayMode->at(igen) << endl;
	  if (isFirst)
	  {
	    if (idx1 >= 0 && idx2 >= 0)
	    {
	      cout << "** ERROR: more than 2 H identified (first)" << endl;
	      continue;
	    }
	    (idx1 == -1) ? (idx1 = igen) : (idx2 = igen) ;
	  }
	  if (isLast)
	  {
	    if (idx1last >= 0 && idx2last >= 0)
	    {
	      cout << "** ERROR: more than 2 H identified (last)" << endl;
	      // continue; // no need to skip the event in this case -- dec mode just for studies
	    }
	    (idx1last == -1) ? (idx1last = igen) : (idx2last = igen) ;
	  }
	}

	if ( (abs(pdg) == 11 || abs(pdg) == 13 ) && isHardScatt && isLast && mothIsHardScatt)
	{
	  if (idx1hs == -1) idx1hs = igen;
	  else if (idx2hs == -1) idx2hs = igen;
	  else
	  {
	    cout << "** ERROR: there are more than 2 hard scatter tau dec prod: evt = " << theBigTree.EventNumber << endl;
	    // cout << "idx1: " << idx1hs << " --> pdg = " << theBigTree.genpart_pdg->at(idx1hs) << " px = " << theBigTree.genpart_px->at(idx1hs) << endl;
	    // cout << "idx2: " << idx2hs << " --> pdg = " << theBigTree.genpart_pdg->at(idx2hs) << " px = " << theBigTree.genpart_px->at(idx2hs) << endl;
	    // cout << "THIS: " << pdg << " px=" << theBigTree.genpart_px->at(igen) << endl;
	  }
	}

	if ( abs(pdg) == 66615 && mothIsHardScatt)
	{
	  // cout << "  <<< preso" << endl;
	  if (idx1hs == -1) idx1hs = igen;
	  else if (idx2hs == -1) idx2hs = igen;
	  else
	  {
	    cout << "** ERROR: there are more than 2 hard scatter tau dec prod: evt = " << theBigTree.EventNumber << endl;
	    // cout << "idx1: " << idx1hs << " --> pdg = " << theBigTree.genpart_pdg->at(idx1hs) << " px = " << theBigTree.genpart_px->at(idx1hs) << endl;
	    // cout << "idx2: " << idx2hs << " --> pdg = " << theBigTree.genpart_pdg->at(idx2hs) << " px = " << theBigTree.genpart_px->at(idx2hs) << endl;
	    // cout << "THIS: " << pdg << " px=" << theBigTree.genpart_px->at(igen) << endl;
	  }
	}

	// FRA DEBUG - find the bjets from the Higgs decay
	if ( abs(pdg) == 5 && isHardProcess)
	{
	  if (idx1hs_b == -1) idx1hs_b = igen;
	  else if (idx2hs_b == -1) idx2hs_b = igen;
	  else
	  {
	    cout << "** ERROR: there are more than 2 hard scatter b quarks: evt = " << theBigTree.EventNumber << endl;
	  }
	}

      }


      if (idx1 == -1 || idx2 == -1)
      {
	cout << "** ERROR: couldn't find 2 H (first)" << endl;
	continue;
      }

      if (idx1last != -1 && idx2last != -1) // this is not critical if not found
      {
	// store gen decay mode of the two H identified
	theSmallTree.m_genDecMode1 = theBigTree.genpart_HZDecayMode->at(idx1last);
	theSmallTree.m_genDecMode2 = theBigTree.genpart_HZDecayMode->at(idx2last);

	// cout << "THIS H decay mode: " << theSmallTree.m_genDecMode1 << " " << theSmallTree.m_genDecMode2 << endl;

	// // get tau decaying one
	// int idxTauDecayed = (theBigTree.genpart_HZDecayMode->at(idx1last) != 8 ? idx1last : idx2last);

	// // find hard scatter daughters and check if they match this decay type
	// pair<int, int> hsProds = oph.getHardScatterSons()
	// int hsIdx1 = hsProds.first;
	// int hsIdx2 = hsProds.second;
      }
      else
	cout << "** ERROR: couldn't find 2 H (last)" << endl;

      if (idx1hs != -1 && idx2hs != -1)
      {
	pdg1hs = theBigTree.genpart_pdg->at(idx1hs);
	pdg2hs = theBigTree.genpart_pdg->at(idx2hs);

	if      (abs(pdg1hs) == 11) t1hs = 1;
	else if (abs(pdg1hs) == 13) t1hs = 0;
	else                        t1hs = 2;

	if      (abs(pdg2hs) == 11) t2hs = 1;
	else if (abs(pdg2hs) == 13) t2hs = 0;
	else                        t2hs = 2;

	if (oph.getPairType(t1hs, t2hs) != (theSmallTree.m_genDecMode1 + theSmallTree.m_genDecMode2 - 8))
	{
	  cout << "** ERROR: decay modes do not match! " << theBigTree.genpart_pdg->at(idx1hs) << " " << theBigTree.genpart_pdg->at(idx2hs) << " != "
	       << ( theSmallTree.m_genDecMode1 + theSmallTree.m_genDecMode2 - 8) << endl;
	}
	vHardScatter1.SetPxPyPzE (theBigTree.genpart_px->at(idx1hs), theBigTree.genpart_py->at(idx1hs), theBigTree.genpart_pz->at(idx1hs), theBigTree.genpart_e->at(idx1hs));
	vHardScatter2.SetPxPyPzE (theBigTree.genpart_px->at(idx2hs), theBigTree.genpart_py->at(idx2hs), theBigTree.genpart_pz->at(idx2hs), theBigTree.genpart_e->at(idx2hs));
      }
      else
	cout << "** ERROR: couldn't find 2 H->tautau gen dec prod " << idx1hs << " " << idx2hs << endl;


      vH1.SetPxPyPzE (theBigTree.genpart_px->at(idx1), theBigTree.genpart_py->at(idx1), theBigTree.genpart_pz->at(idx1), theBigTree.genpart_e->at(idx1) );
      vH2.SetPxPyPzE (theBigTree.genpart_px->at(idx2), theBigTree.genpart_py->at(idx2), theBigTree.genpart_pz->at(idx2), theBigTree.genpart_e->at(idx2) );
      vSum = vH1+vH2;
      mHH = vSum.M();
      theSmallTree.m_genH1_pt = vH1.Pt();
      theSmallTree.m_genH1_eta = vH1.Eta();
      theSmallTree.m_genH1_phi = vH1.Phi();
      theSmallTree.m_genH1_e = vH1.E();
      theSmallTree.m_genH2_pt = vH2.Pt();
      theSmallTree.m_genH2_eta = vH2.Eta();
      theSmallTree.m_genH2_phi = vH2.Phi();
      theSmallTree.m_genH2_e = vH2.E();
      vSum = vH1+vH2;
      mHH = vSum.M();
      vH1.Boost(-vSum.BoostVector());
      ct1 = vH1.CosTheta();


      // FRA DEBUG - build gen b jets
      if (idx1hs_b != -1 && idx2hs_b != -1)
      {
	vGenB1.SetPxPyPzE (theBigTree.genpart_px->at(idx1hs_b), theBigTree.genpart_py->at(idx1hs_b), theBigTree.genpart_pz->at(idx1hs_b), theBigTree.genpart_e->at(idx1hs_b) );
	vGenB2.SetPxPyPzE (theBigTree.genpart_px->at(idx2hs_b), theBigTree.genpart_py->at(idx2hs_b), theBigTree.genpart_pz->at(idx2hs_b), theBigTree.genpart_e->at(idx2hs_b) );
      }
      else
	cout << "** ERROR: couldn't find 2 H->bb gen dec prod " << idx1hs_b << " " << idx2hs_b << endl;


      if (HHrewType == kDiffRew)      HHweight = hhreweighter->getWeight(mHH, ct1);
      else if (HHrewType == kC2scan)  HHweight = hhreweighter->getWeight(mHH, ct1, c2_rew);
      else if (HHrewType == kOverRew) HHweight = hhreweighter->getWeight(mHH, ct1, kl_rew, kt_rew, c2_rew, cg_rew, c2g_rew);


      theSmallTree.m_genMHH = mHH;
      theSmallTree.m_genCosth = ct1;

      // cout << " ........... GEN FINISHED ........... " << " evt=" << theBigTree.EventNumber << " run=" << theBigTree.RunNumber << " lumi=" << theBigTree.lumi << endl;

    }

    ///////////////////////////////////////////////////////////
    // END of gen related stuff -- compute tot number of events

    int genHHDecMode = (isHHsignal ? theSmallTree.m_genDecMode1 + theSmallTree.m_genDecMode2 - 8 : 0);
    if (genHHDecMode < 0)
    {
      genHHDecMode = 0; // dummy protection if couldn't find initial H
      cout << "** ERROR: negative dec mode, for safety set it ot 0" << endl;
    }
    double EvtW;
    if (theBigTree.npu >= 0 && theBigTree.npu <= 99) // good PU weights
      EvtW = isMC ? (theBigTree.aMCatNLOweight * reweight.weight(PUReweight_MC,PUReweight_target,theBigTree.npu,PUreweightFile) * topPtReweight * HHweight) : 1.0;
    else if (theBigTree.npu >= 100)                  // use the last available bin for the PU weight
      EvtW = isMC ? (theBigTree.aMCatNLOweight * reweight.weight(PUReweight_MC,PUReweight_target,99, PUreweightFile) * topPtReweight * HHweight) : 1.0;
    else                                             // if npu<0 --> bug in MC --> weight=0
      EvtW = isMC ? 0.0 : 1.0;

    if (isMC)
    {
      totalEvents += EvtW;
    }
    else
    {
      totalEvents += 1 ;
    }
    ec.Increment ("all", EvtW);
    if (isHHsignal) ecHHsig[genHHDecMode].Increment ("all", EvtW);

    ++totalNoWeightsEventsNum ;


    // ----------------------------------------------------------
    //  apply MET filters

    int metbit = theBigTree.metfilterbit;

    int metpass = (metbit & (1 << 0)) ? 1 : 0; //"Flag_goodVertices"
    metpass    += (metbit & (1 << 1)) ? 1 : 0; //"Flag_HBHENoiseFilter"
    metpass    += (metbit & (1 << 2)) ? 1 : 0; //"Flag_HBHENoiseIsoFilter"
    metpass    += (metbit & (1 << 3)) ? 1 : 0; //"Flag_EcalDeadCellTriggerPrimitiveFilter"
    metpass    += (metbit & (1 << 4)) ? 1 : 0; //"Flag_globalSuperTightHalo2016Filter"
    metpass    += (metbit & (1 << 5)) ? 1 : 0; //"Flag_BadPFMuonFilter"
    metpass    += (metbit & (1 << 6)) ? 1 : 0; //"Flag_eeBadScFilter"
    metpass    += (metbit & (1 << 8)) ? 1 : 0; //"Flag_BadPFMuonDzFilter"

    if(DEBUG)
    {
      cout << "metpass: " << metpass << endl;
      cout << "metfilterbit (int): " << metbit << endl;
      cout << "metfilterbit (binary):         " << std::bitset<10>(theBigTree.metfilterbit) << endl;
      cout << "metfilterbit (metbit binary) : " << std::bitset<10>(metbit) << endl;
      cout << "Flag_goodVertices                      : " << (metbit & (1 << 0)) << endl;
      cout << "Flag_HBHENoiseFilter                   : " << (metbit & (1 << 1)) << endl;
      cout << "Flag_HBHENoiseIsoFilter                : " << (metbit & (1 << 2)) << endl;
      cout << "Flag_EcalDeadCellTriggerPrimitiveFilter: " << (metbit & (1 << 3)) << endl;
      cout << "Flag_globalSuperTightHalo2016Filter    : " << (metbit & (1 << 4)) << endl;
      cout << "Flag_BadPFMuonFilter                   : " << (metbit & (1 << 5)) << endl;
      cout << "Flag_eeBadScFilter                     : " << (metbit & (1 << 6)) << endl;
      cout << "Flag_BadPFMuonDzFilter                 : " << (metbit & (1 << 8)) << endl;
    }

    if(metpass < 8) continue ;

    ec.Increment ("METfilter", EvtW);
    if (isHHsignal) ecHHsig[genHHDecMode].Increment ("METfilter", EvtW);

    // ----------------------------------------------------------
    // require that the event is not affected by the Bad/Clone Muon problem -- for 2016 data
    //if (theBigTree.NBadMu > 0) continue ; //FRA: Sync Feb2018
    ec.Increment ("NoBadMuons", EvtW);
    if (isHHsignal) ecHHsig[genHHDecMode].Increment ("NoBadMuons", EvtW);

    // ----------------------------------------------------------
    // require at least 1 pair
    if (theBigTree.indexDau1->size () == 0) continue ;
    ec.Increment ("PairExists", EvtW);
    if (isHHsignal) ecHHsig[genHHDecMode].Increment ("PairExists", EvtW);

    // ----------------------------------------------------------
    // assess the pair type
    // loop over the daughters to select pair type: mu > e > tau
    // apply tight baseline (with iso to check)

    int nmu = 0;
    int nmu10 = 0; // low pt muons for DY sideband, not entering in nmu
    int nele = 0;
    int nele10 = 0;

    if(DEBUG)
    {
      cout << "***** DEBUG: reco particles (remember: check if baseline sels are aligned to OfflineProducerHelper)" << endl;
    }

    for (unsigned int idau = 0; idau < theBigTree.daughters_px->size(); ++idau)
    {
      int dauType = theBigTree.particleType->at(idau);
      if (oph.isMuon(dauType))
      {
	bool passMu   = oph.muBaseline (&theBigTree, idau, 20., 2.4, 0.15, OfflineProducerHelper::MuTight, string("All") , (DEBUG ? true : false));
	bool passMu10 = oph.muBaseline (&theBigTree, idau, 10., 2.4, 0.30, OfflineProducerHelper::MuTight, string("All") , (DEBUG ? true : false));

	if (passMu) ++nmu;
	else if (passMu10) ++nmu10;
      }
      else if (oph.isElectron(dauType))
      {
	bool passEle   = oph.eleBaseline (&theBigTree, idau, 20., 2.5, 0.1, OfflineProducerHelper::EMVATight, string("Vertex-LepID-pTMin-etaMax") , (DEBUG ? true : false));
	bool passEle10 = oph.eleBaseline (&theBigTree, idau, 10., 2.5, 0.3, OfflineProducerHelper::EMVATight, string("Vertex-LepID-pTMin-etaMax") , (DEBUG ? true : false));

	if (passEle) ++nele;
	else if (passEle10) ++nele10;
      }

      if(DEBUG)
      {
	TLorentzVector dauTlvDebug (
	  theBigTree.daughters_px->at (idau),
	  theBigTree.daughters_py->at (idau),
	  theBigTree.daughters_pz->at (idau),
	  theBigTree.daughters_e ->at (idau));

	// NB: remember to align this debug to the content of OfflineProducerHelper
	cout << ".... reco part "
	     << " idx dau="   << setw(3)  << left << idau
	     << " type="      << setw(3)  << left << dauType
	     << " pt="        << setw(10) << left << dauTlvDebug.Pt()
	     << " eta="       << setw(10) << left << dauTlvDebug.Eta()
	     << " phi="       << setw(10) << left << dauTlvDebug.Phi()
	     << " iso="       << setw(10) << left << getIso (idau, dauTlvDebug.Pt (), theBigTree)
	     << " dxy="       << setw(15) << left << theBigTree.dxy->at(idau)
	     << " dz="        << setw(15) << left << theBigTree.dz->at(idau)
	     << " mutightID=" << setw(3)  << left << CheckBit(theBigTree.daughters_muonID->at(idau),3)
	     << endl;
      }
    } // end loop on daughters

    int pairType = 2; // tau tau
    if (nmu > 0)
    {
      if (nmu == 1 && nmu10 == 0)
	pairType = 0 ; // mu tau
      else
	pairType = 3 ; // mu mu
    }
    else if (nele > 0)
    {
      if (nele == 1 && nele10 == 0)
        pairType = 1;  // ele tau
      else
        pairType = 4 ; // ele ele
    }

    // ----------------------------------------------------------
    // choose the first pair passing baseline and being of the right pair type

    int chosenTauPair = -1;

    if (pairType == 2 && sortStrategyThTh == kHTauTau)
    {
      chosenTauPair = oph.getBestPairHTauTau(&theBigTree, leptonSelectionFlag, (DEBUG ? true : false));
    }

    else if (pairType == 2 && sortStrategyThTh == kPtAndRawIso)
    {
      chosenTauPair = oph.getBestPairPtAndRawIsoOrd(&theBigTree, leptonSelectionFlag, (DEBUG ? true : false));
    }

    // (mu tauh), (e tauh), (tauhtauh && kLLRFramDefault)
    else
    {
      if(DEBUG)
      {
	for (unsigned int iPair = 0 ; iPair < theBigTree.indexDau1->size () ; ++iPair)
	{
	  int t_firstDaughterIndex  = theBigTree.indexDau1->at (iPair) ;
	  int t_secondDaughterIndex = theBigTree.indexDau2->at (iPair) ;
	  int t_type1 = theBigTree.particleType->at (t_firstDaughterIndex) ;
	  int t_type2 = theBigTree.particleType->at (t_secondDaughterIndex) ;
	  cout << " **## Pair: " << iPair << " indexes(" <<t_firstDaughterIndex << "," << t_secondDaughterIndex << ") pairType: "<< pairType << " getPairType: "<< oph.getPairType (t_type1, t_type2) << endl;
	}

	for (unsigned int iLep = 0 ; (iLep < theBigTree.daughters_px->size ()) ; ++iLep)
	{
	  TLorentzVector tlv_dummyLepton(
	    theBigTree.daughters_px->at (iLep),
	    theBigTree.daughters_py->at (iLep),
	    theBigTree.daughters_pz->at (iLep),
	    theBigTree.daughters_e ->at (iLep)) ;

	  cout << " idx="  << iLep
               << " type=" << theBigTree.particleType->at(iLep)
               << " DM="   << theBigTree.decayMode->at(iLep)
               << " DMold="<< theBigTree.daughters_decayModeFindingOldDMs->at(iLep)
               << " pt="   << tlv_dummyLepton.Pt()
               << " eta="  << tlv_dummyLepton.Eta()
               << " phi="  << tlv_dummyLepton.Phi()
               << " iso="  << getIso (iLep, tlv_dummyLepton.Pt (), theBigTree)
               << " dxy="  << theBigTree.dxy->at(iLep)
               << " dz="   << theBigTree.dz->at(iLep)
               << endl;
	}
      } // end DEBUG

      for (unsigned int iPair = 0 ; iPair < theBigTree.indexDau1->size () ; ++iPair)
      {
	int t_firstDaughterIndex  = theBigTree.indexDau1->at (iPair) ;
	int t_secondDaughterIndex = theBigTree.indexDau2->at (iPair) ;
	int t_type1 = theBigTree.particleType->at (t_firstDaughterIndex) ;
	int t_type2 = theBigTree.particleType->at (t_secondDaughterIndex) ;
	if ( oph.getPairType (t_type1, t_type2) != pairType ) continue ;
	string baselineSels = ( (pairType <= 2) ? leptonSelectionFlag : (leptonSelectionFlag + "-Iso")) ; // for ee, mumu, emu, ask isolation in baseline

	if ( oph.pairPassBaseline (&theBigTree, iPair, baselineSels, (DEBUG ? true : false) ) ) // rlx izo to limit to tau iso < 7 -- good for sideband
	{
	  chosenTauPair = iPair;
	  break;
	}
      }
    } // end else (mu tauh), (e tauh), (tauhtauh && kLLRFramDefault)

    if(DEBUG)
    {
      cout << "**** DEBUG : chosen pair : " << chosenTauPair << " str=" << leptonSelectionFlag << " pairType==" << pairType << endl;
      cout << "     ... going to list all pairs of same pairType as the one assessed with reco leptons" << endl;
      for (unsigned int iPair = 0 ; iPair < theBigTree.indexDau1->size () ; ++iPair)
      {
	int t_firstDaughterIndex  = theBigTree.indexDau1->at (iPair) ;
	int t_secondDaughterIndex = theBigTree.indexDau2->at (iPair) ;
	int t_type1 = theBigTree.particleType->at (t_firstDaughterIndex) ;
	int t_type2 = theBigTree.particleType->at (t_secondDaughterIndex) ;
	if ( oph.getPairType (t_type1, t_type2) != pairType ) continue ;
	TLorentzVector tttt (
	  theBigTree.daughters_px->at (t_secondDaughterIndex),
	  theBigTree.daughters_py->at (t_secondDaughterIndex),
	  theBigTree.daughters_pz->at (t_secondDaughterIndex),
	  theBigTree.daughters_e ->at (t_secondDaughterIndex));

	cout << "- " << iPair << " idx1=" << t_firstDaughterIndex << " idx2=" << t_secondDaughterIndex << " isoTau=" <<  getIso (t_secondDaughterIndex, tttt.Pt (), theBigTree) << " tauPt=" << tttt.Pt() << " type2=" << t_type2 << " eta=" << tttt.Eta() << " phi=" << tttt.Phi() << endl;
	cout << "   >>> DM=" << theBigTree.daughters_decayModeFindingOldDMs->at(t_secondDaughterIndex) << " dxy=" << theBigTree.dxy->at(t_secondDaughterIndex) << " dz=" << theBigTree.dz->at(t_secondDaughterIndex) << endl;
      }
    }

    if (chosenTauPair < 0) continue; // no pair found over baseline

    ec.Increment ("PairFoundBaseline", EvtW);
    if (isHHsignal)
    {
      ecHHsig[genHHDecMode].Increment ("PairFoundBaseline", EvtW);
      if (pairType == genHHDecMode)
	ecHHsig[genHHDecMode].Increment ("PairMatchesGen", EvtW);
    }

    // ----------------------------------------------------------
    // pair has been assessed , check trigger information

    // in TauTau channel make sure the first tau is the most isolated one
    int tmp_firstDaughterIndex  = theBigTree.indexDau1->at (chosenTauPair) ;
    int tmp_secondDaughterIndex = theBigTree.indexDau2->at (chosenTauPair) ;

    if (pairType == 2 && (theBigTree.daughters_byDeepTau2017v2p1VSjetraw->at(theBigTree.indexDau1->at (chosenTauPair)) < theBigTree.daughters_byDeepTau2017v2p1VSjetraw->at(theBigTree.indexDau2->at (chosenTauPair))) )
    {
      tmp_firstDaughterIndex  = theBigTree.indexDau2->at (chosenTauPair) ;
      tmp_secondDaughterIndex = theBigTree.indexDau1->at (chosenTauPair) ;
    }

    const int firstDaughterIndex  = tmp_firstDaughterIndex ;
    const int secondDaughterIndex = tmp_secondDaughterIndex ;
    const int type1 = theBigTree.particleType->at (firstDaughterIndex) ;
    const int type2 = theBigTree.particleType->at (secondDaughterIndex) ;
    const int pType = pairType ;

    const int isOS  = theBigTree.isOSCand->at (chosenTauPair) ;
    bool lep1HasTES = false;
    bool lep2HasTES = false;
    bool lep1HasEES = false;
    bool lep2HasEES = false;
    bool lep1HasMES = false;
    bool lep2HasMES = false;

    int DM1 = theBigTree.decayMode->at(firstDaughterIndex);
    int DM2 = theBigTree.decayMode->at(secondDaughterIndex);
    if (DM1 == 2) DM1 = 1;
    if (DM2 == 2) DM2 = 1;

    /*if (DM1 == 11) DM1 = 10; //Davide: DM 11 should be used in combination with deepTau: temporarily forced to 10 because SF ar not available
      if (DM2 == 11) DM2 = 10;*/

    //save decaymode
    if(pairType == 0){ //mutauh
      theSmallTree.m_dau1_decayMode = -1;
      theSmallTree.m_dau2_decayMode = DM2;
    }

    if(pairType == 1){ //etauh
      theSmallTree.m_dau1_decayMode = -1;
      theSmallTree.m_dau2_decayMode = DM2;
    }

    if(pairType == 2){ //tauhtauh
      theSmallTree.m_dau1_decayMode = DM1;
      theSmallTree.m_dau2_decayMode = DM2;
    }

    if(pairType > 2){ //pairs without tauh
      theSmallTree.m_dau1_decayMode = -1;
      theSmallTree.m_dau2_decayMode = -1;
    }

    if (isMC)
    {
      int nRealTaus= 0;
      lep1HasTES = (theBigTree.genmatch->at(firstDaughterIndex)  == 5 ? true : false);
      lep2HasTES = (theBigTree.genmatch->at(secondDaughterIndex) == 5 ? true : false);
      lep1HasEES = ((theBigTree.genmatch->at(firstDaughterIndex)  == 1 || theBigTree.genmatch->at(firstDaughterIndex)  == 3) ? true : false);
      lep2HasEES = ((theBigTree.genmatch->at(secondDaughterIndex) == 1 || theBigTree.genmatch->at(secondDaughterIndex) == 3) ? true : false);
      lep1HasMES = ((theBigTree.genmatch->at(firstDaughterIndex)  == 2 || theBigTree.genmatch->at(firstDaughterIndex)  == 4) ? true : false);
      lep2HasMES = ((theBigTree.genmatch->at(secondDaughterIndex) == 2 || theBigTree.genmatch->at(secondDaughterIndex) == 4) ? true : false);
      theSmallTree.m_isTau1real = (lep1HasTES == true ? 1 : 0); // -1: data; 0: fake tau, 1: real tau
      theSmallTree.m_isTau2real = (lep2HasTES == true ? 1 : 0);
      if (lep1HasTES) nRealTaus +=1;
      if (lep2HasTES) nRealTaus +=1;
      theSmallTree.m_nRealTaus = nRealTaus;                     // -1: data; > 0: # real taus in MC
    }


    const TLorentzVector tlv_firstLepton (
      theBigTree.daughters_px->at (firstDaughterIndex),
      theBigTree.daughters_py->at (firstDaughterIndex),
      theBigTree.daughters_pz->at (firstDaughterIndex),
      theBigTree.daughters_e->at (firstDaughterIndex)
      );

    const TLorentzVector tlv_secondLepton (
      theBigTree.daughters_px->at (secondDaughterIndex),
      theBigTree.daughters_py->at (secondDaughterIndex),
      theBigTree.daughters_pz->at (secondDaughterIndex),
      theBigTree.daughters_e->at (secondDaughterIndex)
      );

    //////////
    // -- GEN NEUTRINO, LEPTON, B-QUARK DEFINITION:
    // -> Adding gen-matched info for the 2 taus neutrinos for tauTau ID training tests
    TLorentzVector vGenNu1; // neutrino associated to tau1
    TLorentzVector vGenNu2; // neutrino associated to tau2
    TLorentzVector vGenNuNoMatch; // neutrino associated to tau2
    // also getting gen vis tau 4vec for reco matching
    TLorentzVector vGenTau1Vis;
    TLorentzVector vGenTau2Vis;
    int gentau1_idx = -1;
    int gentau2_idx = -1;

    std::vector<TLorentzVector> vGenLeptons;
    std::vector<Int_t> vGenLeptonPdgIds;
    std::vector<Int_t> vGenLeptonMotherPdgIds;

    std::vector<TLorentzVector> vGenBQuarks;
    std::vector<Int_t> vGenBQuarkMotherPdgIds;
    std::vector<Int_t> vGenBQuarkStatus;

    if(isMC) {
      for (unsigned int igen = 0; igen < theBigTree.genpart_px->size(); igen++) {
        // only looking at gen e/mu/tau and neutrinos
        int pdg = fabs(theBigTree.genpart_pdg->at(igen));

        if(pdg == 5 && CheckBit(theBigTree.genpart_flags->at(igen), 8) && CheckBit(theBigTree.genpart_flags->at(igen), 12) && theBigTree.genpart_status->at(igen) != 21){
            // statusFlags 8: fromHardProcess, 12: isFirstCopy
            // status 21: incoming particles
            TLorentzVector genBQuarkTLV;
            genBQuarkTLV.SetPxPyPzE(theBigTree.genpart_px->at(igen),
                                    theBigTree.genpart_py->at(igen),
                                    theBigTree.genpart_pz->at(igen),
                                    theBigTree.genpart_e->at(igen));
            vGenBQuarks.push_back(genBQuarkTLV);
            int motherPdgId = -1;
            if(theBigTree.genpart_HMothInd->at(igen) > -1)
                motherPdgId = theBigTree.genpart_pdg->at(theBigTree.genpart_HMothInd->at(igen));
            if(theBigTree.genpart_TopMothInd->at(igen) > -1)
                motherPdgId = theBigTree.genpart_pdg->at(theBigTree.genpart_TopMothInd->at(igen));
            if(theBigTree.genpart_ZMothInd->at(igen) > -1)
                motherPdgId = theBigTree.genpart_pdg->at(theBigTree.genpart_ZMothInd->at(igen));
            vGenBQuarkMotherPdgIds.push_back(motherPdgId);
            vGenBQuarkStatus.push_back(theBigTree.genpart_status->at(igen));
        }

        bool isLepton = (pdg==11||pdg==13||pdg==15);
        bool isNeutrino =  (pdg==12||pdg==14||pdg==16);
        if(!isLepton && !isNeutrino) continue;

        if(isLepton && CheckBit(theBigTree.genpart_flags->at(igen), 8) && CheckBit(theBigTree.genpart_flags->at(igen), 13)){ // 8: fromHardProcess, 13 isLastCopy
            TLorentzVector genLeptonTLV;
            genLeptonTLV.SetPxPyPzE(theBigTree.genpart_px->at(igen),
                                    theBigTree.genpart_py->at(igen),
                                    theBigTree.genpart_pz->at(igen),
                                    theBigTree.genpart_e->at(igen));
            vGenLeptons.push_back(genLeptonTLV);
            vGenLeptonPdgIds.push_back(theBigTree.genpart_pdg->at(igen));

            int motherPdgId = -1;
            int tmpIdx = igen;
            if(theBigTree.genpart_TauMothInd->at(tmpIdx) > -1)
                tmpIdx = theBigTree.genpart_TauMothInd->at(tmpIdx);
            if(theBigTree.genpart_HMothInd->at(tmpIdx) > -1)
                motherPdgId = theBigTree.genpart_pdg->at(theBigTree.genpart_HMothInd->at(tmpIdx));
            if(theBigTree.genpart_ZMothInd->at(tmpIdx) > -1)
                motherPdgId = theBigTree.genpart_pdg->at(theBigTree.genpart_ZMothInd->at(tmpIdx));
            if(theBigTree.genpart_WMothInd->at(tmpIdx) > -1)
                motherPdgId = theBigTree.genpart_pdg->at(theBigTree.genpart_WMothInd->at(tmpIdx));
            if(theBigTree.genpart_TopMothInd->at(tmpIdx) > -1)
                motherPdgId = theBigTree.genpart_pdg->at(theBigTree.genpart_TopMothInd->at(tmpIdx));

            vGenLeptonMotherPdgIds.push_back(motherPdgId);
        }

        int mothIdx = theBigTree.genpart_TauMothInd->at(igen);
        // check particle is from tau decay
        if(mothIdx<0) continue;


        bool mothIsHardScatt = false;
        if (mothIdx > -1)
        {
          bool mothIsLast =  CheckBit(theBigTree.genpart_flags->at(mothIdx), 13); // tru
          // NB: I need t ask that the mother is last idx, otherwise I get a nonphysics "tauh" by the tauh builder function from the tau->tau "decay" in pythia
          mothIsHardScatt = (mothIsLast && CheckBit (theBigTree.genpart_flags->at(mothIdx), 8)); // 0 = isPrompt(), 7: hardProcess , 8: fromHardProcess
        }
        if(pdg==15 && !mothIsHardScatt) continue;

        // check if gen tau matched reco tau
        TLorentzVector vGenTauVis;
        bool match1 = false;
        bool match2 = false;
        if(isLepton){
          // looking at all leptons in decay chain, but end up comparing the last one (e/mu/tau) or the mother for nu count
          // which one it is should not make much difference with DR<0.3
          vGenTauVis.SetPxPyPzE (theBigTree.genpart_px->at(igen),
               theBigTree.genpart_py->at(igen),
               theBigTree.genpart_pz->at(igen),
               theBigTree.genpart_e->at(igen));
          match1 = (vGenTauVis.DeltaR(tlv_firstLepton)<0.3);
          match2 = (vGenTauVis.DeltaR(tlv_secondLepton)<0.3);

          if( match1 && !match2 ) {
            vGenTau1Vis = vGenTauVis;
            gentau1_idx = mothIdx;
          }
          if( !match1 &&  match2 ) {
            vGenTau2Vis = vGenTauVis;
            gentau2_idx = mothIdx;
          }
          if( match1 &&  match2 ) { // unlikely I guess
            if (vGenTauVis.DeltaR(tlv_firstLepton)<vGenTauVis.DeltaR(tlv_secondLepton)){
              vGenTau1Vis = vGenTauVis;
              gentau1_idx = mothIdx;
            } else {
              vGenTau2Vis = vGenTauVis;
              gentau2_idx = mothIdx;
            }
          }
        } // endif(isLepton)

        TLorentzVector vGenNuVis;
        if(isNeutrino){
          vGenNuVis.SetPxPyPzE (theBigTree.genpart_px->at(igen), theBigTree.genpart_py->at(igen), theBigTree.genpart_pz->at(igen), theBigTree.genpart_e->at(igen));

          //cout << "Found gen neutrino : id=" << igen << " | tauMothID=" <<theBigTree.genpart_TauMothInd->at(igen) << endl;

          if(theBigTree.genpart_TauMothInd->at(igen)==gentau1_idx) // neutrino comes from tau1
            vGenNu1+=vGenNuVis;
          else if(theBigTree.genpart_TauMothInd->at(igen)==gentau2_idx) // neutrino comes from tau2
            vGenNu2+=vGenNuVis;
          else // neutrino is unmatched
            vGenNuNoMatch+=vGenNuVis;
        }
      } //end loop on gen part
    } // endif(isMC)
      // -- END GEN NEUTRINO, LEPTON, B-QUARK DEFINITION
      /////////////////////////////////

    // scale up: only applies to tau
    // TES up/down

    vector <double> unc_TES_first;
    vector <double> unc_TES_second;
    if (isMC)
    {
      unc_TES_first.push_back(theBigTree.daughters_TESshiftDM0 ->at (firstDaughterIndex)); // first daughter, DM 0
      unc_TES_first.push_back(theBigTree.daughters_TESshiftDM1 ->at (firstDaughterIndex)); // first daughter, DM 1
      unc_TES_first.push_back(theBigTree.daughters_TESshiftDM10 ->at (firstDaughterIndex)); // first daughter, DM 10
      unc_TES_first.push_back(theBigTree.daughters_TESshiftDM11 ->at (firstDaughterIndex)); // first daughter, DM 11

      unc_TES_second.push_back(theBigTree.daughters_TESshiftDM0 ->at (secondDaughterIndex)); // second daughter, DM 0
      unc_TES_second.push_back(theBigTree.daughters_TESshiftDM1 ->at (secondDaughterIndex)); // second daughter, DM 1
      unc_TES_second.push_back(theBigTree.daughters_TESshiftDM10 ->at (secondDaughterIndex)); // second daughter, DM 10
      unc_TES_second.push_back(theBigTree.daughters_TESshiftDM11 ->at (secondDaughterIndex)); // second daughter, DM 11
    }

    vector <TLorentzVector> tlv_firstLepton_tauup    (N_tauhDM, tlv_firstLepton);
    vector <TLorentzVector> tlv_firstLepton_taudown  (N_tauhDM, tlv_firstLepton);
    vector <TLorentzVector> tlv_secondLepton_tauup   (N_tauhDM, tlv_secondLepton);
    vector <TLorentzVector> tlv_secondLepton_taudown (N_tauhDM, tlv_secondLepton);

    //EES:
    vector <double> unc_EESup_first ;
    vector <double> unc_EESup_second;
    vector <double> unc_EESdw_first ;
    vector <double> unc_EESdw_second;

    if (isMC)
    {
      unc_EESup_first.push_back(theBigTree.daughters_EESshiftDM0up ->at (firstDaughterIndex)); // first daughter, DM 0
      unc_EESup_first.push_back(theBigTree.daughters_EESshiftDM1up ->at (firstDaughterIndex)); // first daughter, DM 1
      unc_EESdw_first.push_back(theBigTree.daughters_EESshiftDM0dw ->at (firstDaughterIndex)); // first daughter, DM 0
      unc_EESdw_first.push_back(theBigTree.daughters_EESshiftDM1dw ->at (firstDaughterIndex)); // first daughter, DM 1

      unc_EESup_second.push_back(theBigTree.daughters_EESshiftDM0up ->at (secondDaughterIndex)); // second daughter, DM 0
      unc_EESup_second.push_back(theBigTree.daughters_EESshiftDM1up ->at (secondDaughterIndex)); // second daughter, DM 1
      unc_EESdw_second.push_back(theBigTree.daughters_EESshiftDM0dw ->at (secondDaughterIndex)); // second daughter, DM 0
      unc_EESdw_second.push_back(theBigTree.daughters_EESshiftDM1dw ->at (secondDaughterIndex)); // second daughter, DM 1
    }

    vector <TLorentzVector> tlv_firstLepton_eleup    (N_tauhDM_EES, tlv_firstLepton);
    vector <TLorentzVector> tlv_firstLepton_eledown  (N_tauhDM_EES, tlv_firstLepton);
    vector <TLorentzVector> tlv_secondLepton_eleup   (N_tauhDM_EES, tlv_secondLepton);
    vector <TLorentzVector> tlv_secondLepton_eledown (N_tauhDM_EES, tlv_secondLepton);

    // for each decay mode, bool indicating if this lepton matches the dacay mode in the loop
    // just for protection, probably it's not needed
    vector<bool> isthisDM_first =
      {
        (theSmallTree.m_dau1_decayMode == 0? true :  false),
        (theSmallTree.m_dau1_decayMode == 1? true :  false),
        (theSmallTree.m_dau1_decayMode == 10? true :  false),
        (theSmallTree.m_dau1_decayMode == 11? true :  false)
      };

    vector<bool> isthisDM_second =
      {
        (theSmallTree.m_dau2_decayMode == 0? true :  false),
        (theSmallTree.m_dau2_decayMode == 1? true :  false),
        (theSmallTree.m_dau2_decayMode == 10? true :  false),
        (theSmallTree.m_dau2_decayMode == 11? true :  false)
      };

    // loop over DMs to fill the shifted TLVs
    for (int idm  = 0; idm < N_tauhDM; idm ++)
    {
      if(lep1HasTES)
      {
	tlv_firstLepton_tauup[idm]   = getShiftedDau(tlv_firstLepton, 1.,  unc_TES_first[idm], isthisDM_first[idm], (idm != 0));  // no mass shift for DM == 0 (idm == 0)
	tlv_firstLepton_taudown[idm] = getShiftedDau(tlv_firstLepton, -1., unc_TES_first[idm], isthisDM_first[idm], (idm != 0));  // no mass shift for DM == 0 (idm == 0)
      }

      else if(lep1HasEES && idm < N_tauhDM_EES)
      {
	tlv_firstLepton_eleup[idm]   = getShiftedDau(tlv_firstLepton, 1.,  unc_EESup_first[idm], isthisDM_first[idm]);
	tlv_firstLepton_eledown[idm] = getShiftedDau(tlv_firstLepton, -1., unc_EESdw_first[idm], isthisDM_first[idm]);
      }

      if(lep2HasTES)
      {
	tlv_secondLepton_tauup[idm]   = getShiftedDau(tlv_secondLepton, 1.,  unc_TES_second[idm], isthisDM_second[idm], (idm != 0));  // no mass shift for DM == 0 (idm == 0)
	tlv_secondLepton_taudown[idm] = getShiftedDau(tlv_secondLepton, -1., unc_TES_second[idm], isthisDM_second[idm], (idm != 0));  // no mass shift for DM == 0 (idm == 0)
      }

      else if(lep2HasEES && idm < N_tauhDM_EES)
      {
	tlv_secondLepton_eleup[idm]   = getShiftedDau(tlv_secondLepton, 1.,  unc_EESup_second[idm], isthisDM_second[idm]);
	tlv_secondLepton_eledown[idm] = getShiftedDau(tlv_secondLepton, -1., unc_EESdw_second[idm], isthisDM_second[idm]);
      }

      theSmallTree.m_dau1_pt_tauup.push_back(tlv_firstLepton_tauup[idm].Pt());
      theSmallTree.m_dau1_pt_taudown.push_back(tlv_firstLepton_taudown[idm].Pt());
      theSmallTree.m_dau1_mass_tauup.push_back(tlv_firstLepton_tauup[idm].M());
      theSmallTree.m_dau1_mass_taudown.push_back(tlv_firstLepton_taudown[idm].M());
      theSmallTree.m_dau2_pt_tauup.push_back(tlv_secondLepton_tauup[idm].Pt());
      theSmallTree.m_dau2_pt_taudown.push_back(tlv_secondLepton_taudown[idm].Pt());
      theSmallTree.m_dau2_mass_tauup.push_back(tlv_secondLepton_tauup[idm].M());
      theSmallTree.m_dau2_mass_taudown.push_back(tlv_secondLepton_taudown[idm].M());

      if (idm < N_tauhDM_EES)
      {
	theSmallTree.m_dau1_pt_eleup.push_back(tlv_firstLepton_eleup[idm].Pt());
	theSmallTree.m_dau1_pt_eledown.push_back(tlv_firstLepton_eledown[idm].Pt());
	theSmallTree.m_dau1_mass_eleup.push_back(tlv_firstLepton_eleup[idm].M());
	theSmallTree.m_dau1_mass_eledown.push_back(tlv_firstLepton_eledown[idm].M());
	theSmallTree.m_dau2_pt_eleup.push_back(tlv_secondLepton_eleup[idm].Pt());
	theSmallTree.m_dau2_pt_eledown.push_back(tlv_secondLepton_eledown[idm].Pt());
	theSmallTree.m_dau2_mass_eleup.push_back(tlv_secondLepton_eleup[idm].M());
	theSmallTree.m_dau2_mass_eledown.push_back(tlv_secondLepton_eledown[idm].M());

      }
    }

    //MES:
    double unc_MESup_first;
    double unc_MESdw_first;
    double unc_MESup_second;
    double unc_MESdw_second;

    if (isMC)
    {
      unc_MESup_first = theBigTree.daughters_MESshiftup->at (firstDaughterIndex); // first daughter, up
      unc_MESdw_first = theBigTree.daughters_MESshiftdw->at (firstDaughterIndex); // first daughter, down
      unc_MESup_second = theBigTree.daughters_MESshiftup ->at (secondDaughterIndex); // second daughter, up
      unc_MESdw_second = theBigTree.daughters_MESshiftdw ->at (secondDaughterIndex); // second daughter, down
    }

    TLorentzVector tlv_firstLepton_muup    =tlv_firstLepton;
    TLorentzVector tlv_firstLepton_mudown  =tlv_firstLepton;
    TLorentzVector tlv_secondLepton_muup   =tlv_secondLepton;
    TLorentzVector tlv_secondLepton_mudown = tlv_secondLepton;

    if (lep1HasMES)
    {
      tlv_firstLepton_muup    = getShiftedDau(tlv_firstLepton, 1.,  unc_MESup_first, true);  // isthisDM -> set to true because MES is for all DMs
      tlv_firstLepton_mudown  = getShiftedDau(tlv_firstLepton, -1.,  unc_MESdw_first, true);
    }

    if (lep2HasMES)
    {
      tlv_secondLepton_muup   = getShiftedDau(tlv_secondLepton, 1.,  unc_MESup_second, true);
      tlv_secondLepton_mudown = getShiftedDau(tlv_secondLepton, -1.,  unc_MESdw_second, true);
    }

    theSmallTree.m_dau1_pt_muup    = tlv_firstLepton_muup.Pt();
    theSmallTree.m_dau1_pt_mudown  = tlv_firstLepton_mudown.Pt();
    theSmallTree.m_dau1_mass_muup  = tlv_firstLepton_muup.M();
    theSmallTree.m_dau1_mass_mudown= tlv_firstLepton_mudown.M();
    theSmallTree.m_dau2_pt_muup    = tlv_secondLepton_muup.Pt();
    theSmallTree.m_dau2_pt_mudown  = tlv_secondLepton_mudown.Pt();
    theSmallTree.m_dau2_mass_muup  = tlv_secondLepton_muup.M();
    theSmallTree.m_dau2_mass_mudown= tlv_secondLepton_mudown.M();

    if (DEBUG)
    {
      cout << "------- CHOSEN PAIR -------" << endl;
      cout << " dau_1 (pt,eta,phi,iso): "<<tlv_firstLepton.Pt()<<" "<<tlv_firstLepton.Eta()<<" "<<tlv_firstLepton.Phi()<<" "<<getIso(firstDaughterIndex, tlv_firstLepton.Pt (), theBigTree)<<endl;
      cout << " dau_2 (pt,eta,phi,iso): "<<tlv_secondLepton.Pt()<<" "<<tlv_secondLepton.Eta()<<" "<<tlv_secondLepton.Phi()<<" "<<getIso(secondDaughterIndex, tlv_secondLepton.Pt (), theBigTree)<<endl;
      cout << "---------------------"<< endl;
    }

    // DATA strategy
    int pass_triggerbit = 0;
    bool passTrg = false;

    // NEW TRIGGERS
    bool passMETTrg = false;
    bool passSingleTau = false;

    if (applyTriggers)
    {
      Long64_t triggerbit = theBigTree.triggerbit;

      Long64_t matchFlag1 = (Long64_t) theBigTree.daughters_trgMatched->at(firstDaughterIndex);
      Long64_t matchFlag2 = (Long64_t) theBigTree.daughters_trgMatched->at(secondDaughterIndex);
      Long64_t goodTriggerType1 = (Long64_t) theBigTree.daughters_isGoodTriggerType->at(firstDaughterIndex);
      Long64_t goodTriggerType2 = (Long64_t) theBigTree.daughters_isGoodTriggerType->at(secondDaughterIndex);

      Long64_t trgNotOverlapFlag = (Long64_t) theBigTree.mothers_trgSeparateMatch->at(chosenTauPair);
      passTrg = trigReader.checkOR (pairType,triggerbit, &pass_triggerbit, matchFlag1, matchFlag2, trgNotOverlapFlag, goodTriggerType1, goodTriggerType2, tlv_firstLepton.Pt(), tlv_firstLepton.Eta(), tlv_secondLepton.Pt(), tlv_secondLepton.Eta()) ;

      // check NEW TRIGGERS separately
      passMETTrg = trigReader.checkMET(triggerbit, &pass_triggerbit);
      passSingleTau = trigReader.checkSingleTau(triggerbit, matchFlag1, matchFlag2, trgNotOverlapFlag, goodTriggerType1, goodTriggerType2, tlv_firstLepton.Pt(), tlv_firstLepton.Eta(), tlv_secondLepton.Pt(), tlv_secondLepton.Eta(), &pass_triggerbit);

      // !! WARNING !! --> update the trigger bits to the right paths
      // Weight to be applied in 2016 from https://twiki.cern.ch/twiki/bin/viewauth/CMS/DoubleHiggsToBBTauTauWorkingLegacyRun2#Triggers
      //   - HLT_IsoMu22 and HLT_IsoTkMu22               : 0.795221971
      //   - HLT_IsoMu22_eta2p1 and HLT_IsoTkMu22_eta2p1 : 0.923782353
      //@ bit position - path
      // 0 - HLT_IsoMu22_v
      // 1 - HLT_IsoTkMu22_v
      // 2 - HLT_IsoMu22_eta2p1_v
      // 3 - HLT_IsoTkMu22_eta2p1_v
      // 4 - HLT_Ele25_eta2p1_WPTight_Gsf_v
      // 5 - HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg_v
      // 6 - HLT_DoubleMediumCombinedIsoPFTau35_Trk1_eta2p1_Reg_v
      // 7 - HLT_IsoMu19_eta2p1_LooseIsoPFTau20_v
      // 8 - HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v

      // MuTau
      // Jan 2021: prescales commented as they are already included in the trigger SFs
      //if (pairType == 0 && isMC)
      //{
      //  if ( CheckBit(pass_triggerbit,7) || CheckBit(pass_triggerbit,8) )
      //  {
      //    theSmallTree.m_prescaleWeight = 1.; // no need for prescale for cross triggers
      //  }
      //  else if ( CheckBit(pass_triggerbit,2) || CheckBit(pass_triggerbit,3) )
      //  {
      //    theSmallTree.m_prescaleWeight = 0.923782353; // passes (HLT_IsoMu22_eta2p1_v OR HLT_IsoTkMu22_eta2p1_v)
      //  }
      //  else if ( CheckBit(pass_triggerbit,0) || CheckBit(pass_triggerbit,1) )
      //  {
      //    theSmallTree.m_prescaleWeight = 0.795221971; // passes only (HLT_IsoMu22_v OR HLT_IsoTkMu22_v)
      //  }
      //}

      // MuMu
      // Jan 2021: prescales commented as they are already included in the trigger SFs
      //if (pairType == 3 && isMC)
      //{
      //  if ( CheckBit(pass_triggerbit,2) || CheckBit(pass_triggerbit,3) )
      //  {
      //    theSmallTree.m_prescaleWeight = 0.923782353; // passes (HLT_IsoMu22_eta2p1_v OR HLT_IsoTkMu22_eta2p1_v)
      //  }
      //  else if ( CheckBit(pass_triggerbit,0) || CheckBit(pass_triggerbit,1) )
      //  {
      //    theSmallTree.m_prescaleWeight = 0.795221971; // passes only (HLT_IsoMu22_v OR HLT_IsoTkMu22_v)
      //  }
      //}

      if(DEBUG)
      {
	if (isMC && (pairType == 2 || pairType == 1 || pairType == 0))
	{
	  cout << "passTrg? "<< trigReader.checkOR (pairType,triggerbit, &pass_triggerbit, matchFlag1, matchFlag2, trgNotOverlapFlag, goodTriggerType1, goodTriggerType2, tlv_firstLepton.Pt(), tlv_firstLepton.Eta(), tlv_secondLepton.Pt(), tlv_secondLepton.Eta())<<endl;
	  cout << "L1 pt1 "<<theBigTree.daughters_highestEt_L1IsoTauMatched->at(firstDaughterIndex) << "L1 pt2 "<<theBigTree.daughters_highestEt_L1IsoTauMatched->at(secondDaughterIndex)<<endl;
	  cout << "---> passTrg? "<<passTrg<<endl;
	}
      }

      bool triggerAccept = false;
      triggerAccept = passTrg || passMETTrg || passSingleTau;

      if(DEBUG)
      {
	if(pairType == 0)//MuTau
	{
	  trigReader.listMuTau(triggerbit, matchFlag1, matchFlag2, trgNotOverlapFlag, goodTriggerType1, goodTriggerType2);
	}
	if(pairType == 1)//ETau
	{
	  trigReader.listETau(triggerbit, matchFlag1, matchFlag2, trgNotOverlapFlag, goodTriggerType1, goodTriggerType2);
	}
	if(pairType == 2)//TauTau
	{
	  trigReader.listTauTau(triggerbit, matchFlag1, matchFlag2, trgNotOverlapFlag, goodTriggerType1, goodTriggerType2);
	}
      }

      if (DEBUG) std::cout << "----> Final Trigger passed? " << triggerAccept << std::endl;
      if (!triggerAccept) continue;

      theSmallTree.m_pass_triggerbit = pass_triggerbit;
      ec.Increment ("Trigger", EvtW); // for data, EvtW is 1.0
      if (isHHsignal && pairType == genHHDecMode) ecHHsig[genHHDecMode].Increment ("Trigger", EvtW);

      theSmallTree.m_isLeptrigger = passTrg;

      // NEW TRIGGERS: fill trig info in output tree
      theSmallTree.m_isMETtrigger = passMETTrg;
      theSmallTree.m_isSingleTautrigger = passSingleTau;
    }


    // ----------------------------------------------------------
    // pair selection is now complete, compute oher quantitites

    // If skimming the final ntuples skip not interesting channels
    if (onlyFinalChannels && pairType>2) continue;

    // First create a map to associate <jetIdx, smearFactor> that will be
    // used to smear the jets always in the same way in the event.
    // Since the smearing is by definition a random process the smearing factors of
    // the jets must be computed only one time per event.
    std::map<int,double> jets_and_smearFactor;
    for (unsigned int iJet = 0 ; iJet < theBigTree.jets_px->size () ; ++iJet)
    {
      // Build jet tlv
      TLorentzVector tlv_jet(theBigTree.jets_px->at(iJet), theBigTree.jets_py->at(iJet), theBigTree.jets_pz->at(iJet), theBigTree.jets_e->at(iJet)) ;

      // Get the smearFactor
      double smearFactor = Smearer.getSmearFactor(tlv_jet, theBigTree);

      // Store the smearFacotr in the map
      if (doSmearing)
	jets_and_smearFactor[iJet] = smearFactor;
      else
	jets_and_smearFactor[iJet] = 1.;
    }

    if (DEBUG)
    {
      std::cout << "---------- SmearMap DEBUG ----------" << std::endl;
      for (auto el : jets_and_smearFactor) std::cout << "Element: " << el.first << "  " << el.second << std::endl;
      std::cout << "------------------------------------" << std::endl;
    }

    // Compute quantities
    TLorentzVector tlv_tauH = tlv_firstLepton + tlv_secondLepton ;
    TLorentzVector tlv_tauH_SVFIT ;

    TVector2 vMET(theBigTree.METx->at(chosenTauPair) , theBigTree.METy->at(chosenTauPair));
    TLorentzVector tlv_MET;
    tlv_MET.SetPxPyPzE(theBigTree.METx->at(chosenTauPair), theBigTree.METy->at(chosenTauPair), 0, std::hypot(theBigTree.METx->at(chosenTauPair), theBigTree.METy->at(chosenTauPair)));

    TLorentzVector tlv_DeepMET_ResponseTune;
    tlv_DeepMET_ResponseTune.SetPtEtaPhiM(theBigTree.ShiftedDeepMETresponseTune_pt, 0, theBigTree.ShiftedDeepMETresponseTune_phi, 0);
    TVector2 vDeepMET_ResponseTune(tlv_DeepMET_ResponseTune.Px(), tlv_DeepMET_ResponseTune.Py());

    TLorentzVector tlv_DeepMET_ResolutionTune;
    tlv_DeepMET_ResolutionTune.SetPtEtaPhiM(theBigTree.ShiftedDeepMETresolutionTune_pt, 0, theBigTree.ShiftedDeepMETresolutionTune_phi, 0);
    TVector2 vDeepMET_ResolutionTune(tlv_DeepMET_ResolutionTune.Px(), tlv_DeepMET_ResolutionTune.Py());

    // Temporary SVFit recomputation for data.
    if(!isMC){
      TMatrixD metcov_tmp (2, 2) ;
      metcov_tmp (0,0) = theBigTree.MET_cov00->at (chosenTauPair) ;
      metcov_tmp (1,0) = theBigTree.MET_cov10->at (chosenTauPair) ;
      metcov_tmp (0,1) = theBigTree.MET_cov01->at (chosenTauPair) ;
      metcov_tmp (1,1) = theBigTree.MET_cov11->at (chosenTauPair) ;

      SVfitKLUBinterface algo_tmp(0, tlv_firstLepton, tlv_secondLepton, tlv_MET, metcov_tmp, pType, theSmallTree.m_dau1_decayMode, theSmallTree.m_dau2_decayMode);
      std::vector<double> svfitResTmp = algo_tmp.FitAndGetResult();
      theSmallTree.m_tauH_SVFIT_mass = svfitResTmp.at(3);
      if (theSmallTree.m_tauH_SVFIT_mass > 0)
      {
	theSmallTree.m_tauH_SVFIT_pt   = svfitResTmp.at(0);
	theSmallTree.m_tauH_SVFIT_eta  = svfitResTmp.at(1);
	theSmallTree.m_tauH_SVFIT_phi  = svfitResTmp.at(2);
	tlv_tauH_SVFIT.SetPtEtaPhiM(svfitResTmp.at(0), svfitResTmp.at(1), svfitResTmp.at(2), svfitResTmp.at(3));

	theSmallTree.m_tauHsvfitMet_deltaPhi = deltaPhi (vMET.Phi(), tlv_tauH_SVFIT.Phi ()) ;
	theSmallTree.m_ditau_deltaR_per_tauHsvfitpt = tlv_firstLepton.DeltaR(tlv_secondLepton) * tlv_tauH_SVFIT.Pt();
      }
    }
    //theSmallTree.m_tauH_SVFIT_mass = theBigTree.SVfitMass->at (chosenTauPair) ;

    if (doSmearing)
    {
      // Smear MET
      float METx = theBigTree.METx->at(chosenTauPair);
      float METy = theBigTree.METy->at(chosenTauPair);
      TVector2 metSmeared = getShiftedMET_smear(METx, METy, theBigTree, jets_and_smearFactor);
      vMET = metSmeared;
      tlv_MET.SetPxPyPzE(metSmeared.Px(), metSmeared.Py(), 0, std::hypot(metSmeared.Px(), metSmeared.Py()));

      // Recompute SVfit after smearing the MET
      TMatrixD metcov (2, 2) ;
      metcov (0,0) = theBigTree.MET_cov00->at (chosenTauPair) ;
      metcov (1,0) = theBigTree.MET_cov10->at (chosenTauPair) ;
      metcov (0,1) = theBigTree.MET_cov01->at (chosenTauPair) ;
      metcov (1,1) = theBigTree.MET_cov11->at (chosenTauPair) ;

      SVfitKLUBinterface algo_central(0, tlv_firstLepton, tlv_secondLepton, tlv_MET, metcov, pType, theSmallTree.m_dau1_decayMode, theSmallTree.m_dau2_decayMode);
      std::vector<double> svfitResSmeared = algo_central.FitAndGetResult();
      theSmallTree.m_tauH_SVFIT_mass = svfitResSmeared.at(3);
      if (theSmallTree.m_tauH_SVFIT_mass > 0)
      {
	theSmallTree.m_tauH_SVFIT_pt   = svfitResSmeared.at(0);
	theSmallTree.m_tauH_SVFIT_eta  = svfitResSmeared.at(1);
	theSmallTree.m_tauH_SVFIT_phi  = svfitResSmeared.at(2);
	tlv_tauH_SVFIT.SetPtEtaPhiM(svfitResSmeared.at(0), svfitResSmeared.at(1), svfitResSmeared.at(2), svfitResSmeared.at(3));

	theSmallTree.m_tauHsvfitMet_deltaPhi = deltaPhi (vMET.Phi(), tlv_tauH_SVFIT.Phi ()) ;
	theSmallTree.m_ditau_deltaR_per_tauHsvfitpt = tlv_firstLepton.DeltaR(tlv_secondLepton) * tlv_tauH_SVFIT.Pt();
      }
    }

    // in case the SVFIT mass is calculated
    if (!doSmearing && theBigTree.SVfitMass->at (chosenTauPair) > -900.)
    {
      theSmallTree.m_tauH_SVFIT_pt     = theBigTree.SVfit_pt->at  (chosenTauPair) ;
      theSmallTree.m_tauH_SVFIT_eta    = theBigTree.SVfit_eta->at (chosenTauPair) ;
      theSmallTree.m_tauH_SVFIT_phi    = theBigTree.SVfit_phi->at (chosenTauPair) ;
      theSmallTree.m_tauH_SVFIT_METphi = theBigTree.SVfit_fitMETPhi->at (chosenTauPair) ;
      theSmallTree.m_tauH_SVFIT_METrho = theBigTree.SVfit_fitMETRho->at (chosenTauPair) ;

      tlv_tauH_SVFIT.SetPtEtaPhiM (
	theBigTree.SVfit_pt->at (chosenTauPair),
	theBigTree.SVfit_eta->at (chosenTauPair),
	theBigTree.SVfit_phi->at (chosenTauPair),
	theBigTree.SVfitMass->at (chosenTauPair)
	) ;

      theSmallTree.m_tauHsvfitMet_deltaPhi = deltaPhi (vMET.Phi(), tlv_tauH_SVFIT.Phi ()) ;
      theSmallTree.m_ditau_deltaR_per_tauHsvfitpt = tlv_firstLepton.DeltaR(tlv_secondLepton) * tlv_tauH_SVFIT.Pt();
    }

    if (DEBUG)
    {
      cout << "------- SVFIT ------" << endl;
      cout << " is calculated ? " << theBigTree.SVfitMass->at(chosenTauPair) << endl;
      cout << " pt/eta/phi: " << tlv_tauH_SVFIT.Pt() << " / " << tlv_tauH_SVFIT.Eta() << " / " << tlv_tauH_SVFIT.Phi() << endl;
      cout << " is calculated UP ? " << theSmallTree.m_tauH_SVFIT_mass_up << endl;
      cout << " is calculated DOWN ? " << theSmallTree.m_tauH_SVFIT_mass_down << endl;
      cout << "--------------------" << endl;
    }

    // check if the selected leptons A,B match the gen hard scatter products 1,2
    if (isHHsignal)
    {
      bool type1A = (abs(t1hs) == abs(type1));
      bool type1B = (abs(t1hs) == abs(type2));
      bool type2A = (abs(t2hs) == abs(type1));
      bool type2B = (abs(t2hs) == abs(type2));

      bool dR1A = (vHardScatter1.DeltaR(tlv_firstLepton) < 0.5);
      bool dR1B = (vHardScatter1.DeltaR(tlv_secondLepton) < 0.5);
      bool dR2A = (vHardScatter2.DeltaR(tlv_firstLepton) < 0.5);
      bool dR2B = (vHardScatter2.DeltaR(tlv_secondLepton) < 0.5);

      // FIXME: fill gen matched info pt/eta/phi/e
      if ( (type1A && dR1A) || (type2A && dR2A))
	theSmallTree.m_hasgenmatch1 = true;
      if ( (type1B && dR1B) || (type2B && dR2B))
	theSmallTree.m_hasgenmatch2 = true;
    }

    theSmallTree.m_pairType    = pType ;
    // Need to change the channel: LLR-> 0:muTau - 1:eTau  /  PI-> 0:eTau - 1:muTau
    if      (theSmallTree.m_pairType == 0) theSmallTree.m_BDT_channel = 1.;
    else if (theSmallTree.m_pairType == 1) theSmallTree.m_BDT_channel = 0.;
    else if (theSmallTree.m_pairType == 2) theSmallTree.m_BDT_channel = 2.;

    //cout << " ------------------> CHECK CHANNEL pairType/BDT_chan: " << theSmallTree.m_pairType << "/" << theSmallTree.m_BDT_channel << endl;

    if (theBigTree.npu >= 0 && theBigTree.npu <= 99) // good PU weights
      theSmallTree.m_PUReweight  = (isMC ? reweight.weight(PUReweight_MC,PUReweight_target,theBigTree.npu,PUreweightFile) : 1) ;
    else if (theBigTree.npu >= 100)                  // use the last available bin for the PU weight
      theSmallTree.m_PUReweight  = (isMC ? reweight.weight(PUReweight_MC,PUReweight_target,99,PUreweightFile) : 1) ;
    else                                             // if npu<0 --> bug in MC --> weight=0
      theSmallTree.m_PUReweight  = (isMC ? 0 : 1) ;
    theSmallTree.m_MC_weight   = (isMC ? theBigTree.aMCatNLOweight * XS * stitchWeight * HHweight : 1) ;
    theSmallTree.m_lheht       = (isMC ? theBigTree.lheHt : 0) ;
    theSmallTree.m_EventNumber = theBigTree.EventNumber ;
    theSmallTree.m_RunNumber   = theBigTree.RunNumber ;
    theSmallTree.m_npv  = theBigTree.npv ;
    theSmallTree.m_npu  = theBigTree.npu ;
    theSmallTree.m_lumi = theBigTree.lumi ;
    theSmallTree.m_triggerbit = theBigTree.triggerbit ;
    theSmallTree.m_rho  = theBigTree.rho ;
    theSmallTree.m_isMC = isMC ;
    theSmallTree.m_isOS = theBigTree.isOSCand->at (chosenTauPair) ;
    theSmallTree.m_lheNOutPartons = theBigTree.lheNOutPartons ;
    theSmallTree.m_lheNOutB = theBigTree.lheNOutB ;
    theSmallTree.m_met_phi   = TVector2::Phi_mpi_pi(vMET.Phi());
    theSmallTree.m_met_et    = vMET.Mod();
    theSmallTree.m_METx    = vMET.X();
    theSmallTree.m_METy    = vMET.Y();
    theSmallTree.m_met_cov00 = theBigTree.MET_cov00->at (chosenTauPair);
    theSmallTree.m_met_cov01 = theBigTree.MET_cov01->at (chosenTauPair);
    theSmallTree.m_met_cov10 = theBigTree.MET_cov10->at (chosenTauPair);
    theSmallTree.m_met_cov11 = theBigTree.MET_cov11->at (chosenTauPair);
    theSmallTree.m_DeepMET_ResponseTune_px = vDeepMET_ResponseTune.X();
    theSmallTree.m_DeepMET_ResponseTune_py = vDeepMET_ResponseTune.Y();
    theSmallTree.m_DeepMET_ResolutionTune_px = vDeepMET_ResolutionTune.X();
    theSmallTree.m_DeepMET_ResolutionTune_py = vDeepMET_ResolutionTune.Y();

    // L1ECALPrefiringWeight - https://twiki.cern.ch/twiki/bin/viewauth/CMS/L1ECALPrefiringWeightRecipe
    theSmallTree.m_L1pref_weight = theBigTree.prefiringweight;

    if (DEBUG)
    {
      cout << "------- MET DEBUG -------" << endl;
      cout << " met centr : " << theSmallTree.m_met_et << " / " << theSmallTree.m_met_phi << endl;
      cout << "-------------------------" << endl;
    }

    // // in TauTau channel make sure the first tau is the most isolated one
    if (pairType == 2 && (theBigTree.daughters_byDeepTau2017v2p1VSjetraw->at(theBigTree.indexDau1->at (chosenTauPair)) < theBigTree.daughters_byDeepTau2017v2p1VSjetraw->at(theBigTree.indexDau2->at (chosenTauPair))) )
    {
      theSmallTree.m_mT1       = theBigTree.mT_Dau2->at (chosenTauPair) ;
      theSmallTree.m_mT2       = theBigTree.mT_Dau1->at (chosenTauPair) ;
    }
    else
    {
      theSmallTree.m_mT1       = theBigTree.mT_Dau1->at (chosenTauPair) ;
      theSmallTree.m_mT2       = theBigTree.mT_Dau2->at (chosenTauPair) ;
    }

    theSmallTree.m_tauH_pt   = tlv_tauH.Pt () ;
    theSmallTree.m_tauH_eta  = tlv_tauH.Eta () ;
    theSmallTree.m_tauH_phi  = tlv_tauH.Phi () ;
    theSmallTree.m_tauH_e    = tlv_tauH.E () ;
    theSmallTree.m_tauH_mass = tlv_tauH.M () ;

    theSmallTree.m_tauHMet_deltaPhi = deltaPhi (vMET.Phi(), tlv_tauH.Phi ()) ;
    theSmallTree.m_ditau_deltaPhi = deltaPhi (tlv_firstLepton.Phi (), tlv_secondLepton.Phi ()) ;
    theSmallTree.m_ditau_deltaEta = fabs(tlv_firstLepton.Eta ()- tlv_secondLepton.Eta ()) ;
    theSmallTree.m_ditau_deltaR = tlv_firstLepton.DeltaR(tlv_secondLepton) ;
    theSmallTree.m_dau1MET_deltaphi = deltaPhi (vMET.Phi() , tlv_firstLepton.Phi ()) ;
    theSmallTree.m_dau2MET_deltaphi = deltaPhi (vMET.Phi() , tlv_secondLepton.Phi ()) ;

    theSmallTree.m_tauH_MET_pt                  = (tlv_tauH + tlv_MET).Pt();
    theSmallTree.m_dau2_MET_deltaEta            = fabs(tlv_secondLepton.Eta()); // since MET.Eta()==0 by definition, dEta(tau2,MET)=|tau2.Eta()|
    theSmallTree.m_ditau_deltaR_per_tauH_MET_pt = theSmallTree.m_ditau_deltaR * theSmallTree.m_tauH_MET_pt;

    theSmallTree.m_p_zeta         = Calculate_Pzeta(tlv_firstLepton, tlv_secondLepton, tlv_MET);
    theSmallTree.m_p_zeta_visible = Calculate_visiblePzeta(tlv_firstLepton, tlv_secondLepton);

    theSmallTree.m_mT_tauH_MET       = Calculate_MT(tlv_tauH + tlv_MET, tlv_MET);
    theSmallTree.m_mT_total          = Calculate_TotalMT(tlv_firstLepton, tlv_secondLepton, tlv_MET);
    if (theBigTree.SVfitMass->at(chosenTauPair) > -900. || (doSmearing && theSmallTree.m_tauH_SVFIT_mass > 0))
    {
      theSmallTree.m_mT_tauH_SVFIT_MET             = Calculate_MT(tlv_tauH_SVFIT, tlv_MET);
      theSmallTree.m_BDT_tauHsvfitMet_abs_deltaPhi = fabs(ROOT::Math::VectorUtil::DeltaPhi(tlv_tauH_SVFIT, tlv_MET));
      theSmallTree.m_BDT_tauHsvfitMet_deltaPhi     = ROOT::Math::VectorUtil::DeltaPhi(tlv_tauH_SVFIT, tlv_MET);
    }

    theSmallTree.m_BDT_ditau_deltaPhi = ROOT::Math::VectorUtil::DeltaPhi(tlv_firstLepton, tlv_secondLepton);
    theSmallTree.m_BDT_dau1MET_deltaPhi = ROOT::Math::VectorUtil::DeltaPhi(tlv_firstLepton, tlv_MET);

    theSmallTree.m_dau1_iso = getIso (firstDaughterIndex, tlv_firstLepton.Pt (), theBigTree) ;
    theSmallTree.m_dau1_eleMVAiso = theBigTree.daughters_iseleWP80->at(firstDaughterIndex) ? 1 : 0;
    theSmallTree.m_dau1_CUTiso = makeIsoDiscr (firstDaughterIndex, tauCUTIDIdx, theBigTree) ;
    theSmallTree.m_dau1_deepTauVsJet = makeIsoDiscr (firstDaughterIndex, deepTauVsJetIdx , theBigTree) ;
    theSmallTree.m_dau1_deepTauVsEle = makeIsoDiscr (firstDaughterIndex, deepTauVsEleIdx , theBigTree) ;
    theSmallTree.m_dau1_deepTauVsMu = makeIsoDiscr (firstDaughterIndex, deepTauVsMuIdx , theBigTree) ;


    int ibit = tauIDsMap["byLooseCombinedIsolationDeltaBetaCorr3Hits"] ;
    theSmallTree.m_dau1_byLooseCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (firstDaughterIndex)  & (1 << ibit) ) ? true : false ;
    theSmallTree.m_dau2_byLooseCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (secondDaughterIndex) & (1 << ibit) ) ? true : false ;

    ibit = tauIDsMap["byMediumCombinedIsolationDeltaBetaCorr3Hits"] ;
    theSmallTree.m_dau1_byMediumCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (firstDaughterIndex)  & (1 << ibit) ) ? true : false ;
    theSmallTree.m_dau2_byMediumCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (secondDaughterIndex) & (1 << ibit) ) ? true : false ;

    ibit = tauIDsMap["byTightCombinedIsolationDeltaBetaCorr3Hits"] ;
    theSmallTree.m_dau1_byTightCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (firstDaughterIndex)  & (1 << ibit) ) ? true : false ;
    theSmallTree.m_dau2_byTightCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (secondDaughterIndex) & (1 << ibit) ) ? true : false ;

    if(isHHsignal){
      theSmallTree.m_genB1_pt = vGenB1.Pt();
      theSmallTree.m_genB1_eta = vGenB1.Eta();
      theSmallTree.m_genB1_phi = vGenB1.Phi();
      theSmallTree.m_genB1_e = vGenB1.E();
      theSmallTree.m_genB2_pt = vGenB2.Pt();
      theSmallTree.m_genB2_eta = vGenB2.Eta();
      theSmallTree.m_genB2_phi = vGenB2.Phi();
      theSmallTree.m_genB2_e = vGenB2.E();
    }

    int nGenBQuarks = vGenBQuarks.size();
    if(nGenBQuarks>=1){
        theSmallTree.m_genBQuark1_pt = vGenBQuarks.at(0).Pt();
        theSmallTree.m_genBQuark1_eta = vGenBQuarks.at(0).Eta();
        theSmallTree.m_genBQuark1_phi = vGenBQuarks.at(0).Phi();
        theSmallTree.m_genBQuark1_e = vGenBQuarks.at(0).E();
        theSmallTree.m_genBQuark1_motherPdgId = vGenBQuarkMotherPdgIds.at(0);
    }
    if(nGenBQuarks>=2){
        theSmallTree.m_genBQuark2_pt = vGenBQuarks.at(1).Pt();
        theSmallTree.m_genBQuark2_eta = vGenBQuarks.at(1).Eta();
        theSmallTree.m_genBQuark2_phi = vGenBQuarks.at(1).Phi();
        theSmallTree.m_genBQuark2_e = vGenBQuarks.at(1).E();
        theSmallTree.m_genBQuark2_motherPdgId = vGenBQuarkMotherPdgIds.at(1);
    }
    if(nGenBQuarks>=3){
        theSmallTree.m_genBQuark3_pt = vGenBQuarks.at(2).Pt();
        theSmallTree.m_genBQuark3_eta = vGenBQuarks.at(2).Eta();
        theSmallTree.m_genBQuark3_phi = vGenBQuarks.at(2).Phi();
        theSmallTree.m_genBQuark3_e = vGenBQuarks.at(2).E();
        theSmallTree.m_genBQuark3_motherPdgId = vGenBQuarkMotherPdgIds.at(2);
    }
    if(nGenBQuarks>=4){
        theSmallTree.m_genBQuark4_pt = vGenBQuarks.at(3).Pt();
        theSmallTree.m_genBQuark4_eta = vGenBQuarks.at(3).Eta();
        theSmallTree.m_genBQuark4_phi = vGenBQuarks.at(3).Phi();
        theSmallTree.m_genBQuark4_e = vGenBQuarks.at(3).E();
        theSmallTree.m_genBQuark4_motherPdgId = vGenBQuarkMotherPdgIds.at(3);
    }
    if(nGenBQuarks>=5){
        std::cout<<">= 5 GenBQuarks"<<std::endl;
        for(unsigned int tmpindex = 0; tmpindex < vGenBQuarkMotherPdgIds.size(); tmpindex++){
            std::cout<<"mother pdgId: "<<vGenBQuarkMotherPdgIds.at(tmpindex)<<", status: "<<vGenBQuarkStatus.at(tmpindex)<<std::endl;
        }
    }

    int nGenLeptons = vGenLeptons.size();
    if(nGenLeptons>=1){
        theSmallTree.m_genLepton1_pt = vGenLeptons.at(0).Pt();
        theSmallTree.m_genLepton1_eta = vGenLeptons.at(0).Eta();
        theSmallTree.m_genLepton1_phi = vGenLeptons.at(0).Phi();
        theSmallTree.m_genLepton1_e = vGenLeptons.at(0).E();
        theSmallTree.m_genLepton1_pdgId = vGenLeptonPdgIds.at(0);
        theSmallTree.m_genLepton1_motherPdgId = vGenLeptonMotherPdgIds.at(0);
    }
     if(nGenLeptons>=2){
        theSmallTree.m_genLepton2_pt = vGenLeptons.at(1).Pt();
        theSmallTree.m_genLepton2_eta = vGenLeptons.at(1).Eta();
        theSmallTree.m_genLepton2_phi = vGenLeptons.at(1).Phi();
        theSmallTree.m_genLepton2_e = vGenLeptons.at(1).E();
        theSmallTree.m_genLepton2_pdgId = vGenLeptonPdgIds.at(1);
        theSmallTree.m_genLepton2_motherPdgId = vGenLeptonMotherPdgIds.at(1);
    }
     if(nGenLeptons>=3){
        theSmallTree.m_genLepton3_pt = vGenLeptons.at(2).Pt();
        theSmallTree.m_genLepton3_eta = vGenLeptons.at(2).Eta();
        theSmallTree.m_genLepton3_phi = vGenLeptons.at(2).Phi();
        theSmallTree.m_genLepton3_e = vGenLeptons.at(2).E();
        theSmallTree.m_genLepton3_pdgId = vGenLeptonPdgIds.at(2);
        theSmallTree.m_genLepton3_motherPdgId = vGenLeptonMotherPdgIds.at(2);
    }
     if(nGenLeptons>=4){
        theSmallTree.m_genLepton4_pt = vGenLeptons.at(3).Pt();
        theSmallTree.m_genLepton4_eta = vGenLeptons.at(3).Eta();
        theSmallTree.m_genLepton4_phi = vGenLeptons.at(3).Phi();
        theSmallTree.m_genLepton4_e = vGenLeptons.at(3).E();
        theSmallTree.m_genLepton4_pdgId = vGenLeptonPdgIds.at(3);
        theSmallTree.m_genLepton4_motherPdgId = vGenLeptonMotherPdgIds.at(3);
    }

    ///////////////////////////////////
    // -- FILLING GEN NEUTRINO BRANCHES
    theSmallTree.m_genNu1_pt = vGenNu1.Pt();
    theSmallTree.m_genNu1_eta = vGenNu1.Eta();
    theSmallTree.m_genNu1_phi = vGenNu1.Phi();
    theSmallTree.m_genNu1_e = vGenNu1.E();

    theSmallTree.m_genNu2_pt = vGenNu2.Pt();
    theSmallTree.m_genNu2_eta = vGenNu2.Eta();
    theSmallTree.m_genNu2_phi = vGenNu2.Phi();
    theSmallTree.m_genNu2_e = vGenNu2.E();

    theSmallTree.m_genNuNoMatch_pt = vGenNuNoMatch.Pt();
    theSmallTree.m_genNuNoMatch_eta = vGenNuNoMatch.Eta();
    theSmallTree.m_genNuNoMatch_phi = vGenNuNoMatch.Phi();
    theSmallTree.m_genNuNoMatch_e = vGenNuNoMatch.E();

    TLorentzVector vGenNuTot = vGenNu1+vGenNu2;
    TLorentzVector vGenNuTotWithNoMatch = vGenNu1+vGenNu2+vGenNuNoMatch;

    theSmallTree.m_genNuTot_pt = vGenNuTot.Pt();
    theSmallTree.m_genNuTot_eta = vGenNuTot.Eta();
    theSmallTree.m_genNuTot_phi = vGenNuTot.Phi();
    theSmallTree.m_genNuTot_e = vGenNuTot.E();

    theSmallTree.m_genNuTotWithNoMatch_pt = vGenNuTotWithNoMatch.Pt();
    theSmallTree.m_genNuTotWithNoMatch_eta = vGenNuTotWithNoMatch.Eta();
    theSmallTree.m_genNuTotWithNoMatch_phi = vGenNuTotWithNoMatch.Phi();
    theSmallTree.m_genNuTotWithNoMatch_e = vGenNuTotWithNoMatch.E();

    TLorentzVector vRecoGenTauH = vGenNu1+vGenNu2+tlv_firstLepton+tlv_secondLepton;
    theSmallTree.m_recoGenTauH_pt   = vRecoGenTauH.Pt();
    theSmallTree.m_recoGenTauH_eta  = vRecoGenTauH.Eta();
    theSmallTree.m_recoGenTauH_phi  = vRecoGenTauH.Phi();
    theSmallTree.m_recoGenTauH_e    = vRecoGenTauH.E();
    theSmallTree.m_recoGenTauH_mass = vRecoGenTauH.M();

    TLorentzVector vRecoGenWithNoMatchTauH = vGenNu1+vGenNu2+vGenNuNoMatch+tlv_firstLepton+tlv_secondLepton;
    theSmallTree.m_recoGenWithNoMatchTauH_pt   = vRecoGenWithNoMatchTauH.Pt();
    theSmallTree.m_recoGenWithNoMatchTauH_eta  = vRecoGenWithNoMatchTauH.Eta();
    theSmallTree.m_recoGenWithNoMatchTauH_phi  = vRecoGenWithNoMatchTauH.Phi();
    theSmallTree.m_recoGenWithNoMatchTauH_e    = vRecoGenWithNoMatchTauH.E();
    theSmallTree.m_recoGenWithNoMatchTauH_mass = vRecoGenWithNoMatchTauH.M();
    // -- END FILLING GEN NEUTRINO BRANCHES
    ///////////////////////////////////////

    theSmallTree.m_dau1_pt = tlv_firstLepton.Pt () ;

    theSmallTree.m_dau1_pt_tauup_DM0    = (tlv_firstLepton_tauup  [0]).Pt();
    theSmallTree.m_dau1_pt_tauup_DM1    = (tlv_firstLepton_tauup  [1]).Pt();
    theSmallTree.m_dau1_pt_tauup_DM10   = (tlv_firstLepton_tauup  [2]).Pt();
    theSmallTree.m_dau1_pt_tauup_DM11   = (tlv_firstLepton_tauup  [3]).Pt();
    theSmallTree.m_dau1_pt_taudown_DM0  = (tlv_firstLepton_taudown[0]).Pt();
    theSmallTree.m_dau1_pt_taudown_DM1  = (tlv_firstLepton_taudown[1]).Pt();
    theSmallTree.m_dau1_pt_taudown_DM10 = (tlv_firstLepton_taudown[2]).Pt();
    theSmallTree.m_dau1_pt_taudown_DM11 = (tlv_firstLepton_taudown[3]).Pt();
    theSmallTree.m_dau1_pt_eleup_DM0    = (tlv_firstLepton_eleup  [0]).Pt();
    theSmallTree.m_dau1_pt_eleup_DM1    = (tlv_firstLepton_eleup  [1]).Pt();
    theSmallTree.m_dau1_pt_eledown_DM0  = (tlv_firstLepton_eledown[0]).Pt();
    theSmallTree.m_dau1_pt_eledown_DM1  = (tlv_firstLepton_eledown[1]).Pt();

    theSmallTree.m_dau1_eta = tlv_firstLepton.Eta () ;
    theSmallTree.m_dau1_phi = tlv_firstLepton.Phi () ;
    theSmallTree.m_dau1_e = theBigTree.daughters_e->at (firstDaughterIndex) ;
    theSmallTree.m_dau1_dxy = theBigTree.dxy->at(firstDaughterIndex) ;
    theSmallTree.m_dau1_dz  = theBigTree.dz->at(firstDaughterIndex) ;
    theSmallTree.m_dau1_flav = theBigTree.daughters_charge->at (firstDaughterIndex) * (theBigTree.particleType->at (firstDaughterIndex) + 1) ;
    // 1 = from muons collection
    // 2 = from electrons collection
    // 3 = from tauH collection
    theSmallTree.m_dau2_iso = getIso (secondDaughterIndex, tlv_secondLepton.Pt (), theBigTree) ;
    theSmallTree.m_dau2_CUTiso = makeIsoDiscr (secondDaughterIndex, tauCUTIDIdx, theBigTree) ;
    theSmallTree.m_dau2_deepTauVsJet = makeIsoDiscr (secondDaughterIndex, deepTauVsJetIdx , theBigTree) ;
    theSmallTree.m_dau2_deepTauVsEle = makeIsoDiscr (secondDaughterIndex, deepTauVsEleIdx , theBigTree) ;
    theSmallTree.m_dau2_deepTauVsMu = makeIsoDiscr (secondDaughterIndex, deepTauVsMuIdx , theBigTree) ;
    theSmallTree.m_dau2_pt = tlv_secondLepton.Pt () ;

    theSmallTree.m_dau2_pt_tauup_DM0    = (tlv_secondLepton_tauup  [0]).Pt();
    theSmallTree.m_dau2_pt_tauup_DM1    = (tlv_secondLepton_tauup  [1]).Pt();
    theSmallTree.m_dau2_pt_tauup_DM10   = (tlv_secondLepton_tauup  [2]).Pt();
    theSmallTree.m_dau2_pt_tauup_DM11   = (tlv_secondLepton_tauup  [3]).Pt();
    theSmallTree.m_dau2_pt_taudown_DM0  = (tlv_secondLepton_taudown[0]).Pt();
    theSmallTree.m_dau2_pt_taudown_DM1  = (tlv_secondLepton_taudown[1]).Pt();
    theSmallTree.m_dau2_pt_taudown_DM10 = (tlv_secondLepton_taudown[2]).Pt();
    theSmallTree.m_dau2_pt_taudown_DM11 = (tlv_secondLepton_taudown[3]).Pt();
    theSmallTree.m_dau2_pt_eleup_DM0    = (tlv_secondLepton_eleup  [0]).Pt();
    theSmallTree.m_dau2_pt_eleup_DM1    = (tlv_secondLepton_eleup  [1]).Pt();
    theSmallTree.m_dau2_pt_eledown_DM0  = (tlv_secondLepton_eledown[0]).Pt();
    theSmallTree.m_dau2_pt_eledown_DM1  = (tlv_secondLepton_eledown[1]).Pt();

    theSmallTree.m_dau2_eta = tlv_secondLepton.Eta () ;
    theSmallTree.m_dau2_phi = tlv_secondLepton.Phi () ;
    theSmallTree.m_dau2_e = theBigTree.daughters_e->at (secondDaughterIndex) ;
    theSmallTree.m_dau2_dxy = theBigTree.dxy->at(secondDaughterIndex) ;
    theSmallTree.m_dau2_dz  = theBigTree.dz->at(secondDaughterIndex) ;
    theSmallTree.m_dau2_flav = theBigTree.daughters_charge->at (secondDaughterIndex) * (theBigTree.particleType->at (secondDaughterIndex) + 1) ;

    // DATA/MC ID and ISO ScaleFactors
    // Mu and Ele ID and ISO: https://github.com/CMS-HTT/LeptonEfficiencies
    // Tau ID and ISO (MVA and DeepTau): https://twiki.cern.ch/twiki/bin/view/CMS/TauIDRecommendationForRun2
    // Still preliminary:
    // DeepTauVSele: https://indico.cern.ch/event/865792/contributions/3659828/attachments/1954858/3246751/ETauFR-update2Dec.pdf
    // DeepTauVSmu : https://indico.cern.ch/event/866243/contributions/3650016/attachments/1950974/3238736/mutauFRRun2_Yiwen_20191121.pdf
    float idSF_deep_dm	   = 1.0; // use this for DeepTauV2p1 DM dependent
	float idSF_deep_pt	   = 1.0; // use this for DeepTauV2p1 pT dependent
	float idSF_deep_2d	   = 1.0; // use this for DeepTauV2p1 DM and pT dependent
	float idFakeSF_deep_dm = 1.0; // use this for DeepTauV2p1 DM dependent + e/mu->tauh fake SF
	float idFakeSF_deep_pt = 1.0; // use this for DeepTauV2p1 pt dependent + e/mu->tauh fake SF
	float idFakeSF_deep_2d = 1.0; // use this for DeepTauV2p1 pt dependent + e/mu->tauh fake SF
	float fakeRateSF_deep  = 1.0; // use this for e/mu->tauh fake SF DeepTau

	float idFakeSF_tauid_2d_stat0_up					= 1.f;
	float idFakeSF_tauid_2d_stat0_down					= 1.f;				  
	float idFakeSF_tauid_2d_stat1_up					= 1.f;					
	float idFakeSF_tauid_2d_stat1_down					= 1.f;				  
	float idFakeSF_tauid_2d_systcorrdmeras_up			= 1.f;		   
	float idFakeSF_tauid_2d_systcorrdmeras_down			= 1.f;		 
	float idFakeSF_tauid_2d_systcorrdmuncorreras_up		= 1.f;	 
	float idFakeSF_tauid_2d_systcorrdmuncorreras_down	= 1.f; 
	float idFakeSF_tauid_2d_systuncorrdmeras_up			= 1.f;		 
	float idFakeSF_tauid_2d_systuncorrdmeras_down		= 1.f;	   
	float idFakeSF_tauid_2d_systcorrerasgt140_up		= 1.f;
	float idFakeSF_tauid_2d_systcorrerasgt140_down		= 1.f;
	float idFakeSF_tauid_2d_statgt140_up				= 1.f;
	float idFakeSF_tauid_2d_statgt140_down				= 1.f;
	float idFakeSF_tauid_2d_extrapgt140					= 1.f;

	float idFakeSF_tauid_pt20to25_up    = 1.0;
    float idFakeSF_tauid_pt25to30_up    = 1.0;
    float idFakeSF_tauid_pt30to35_up    = 1.0;
    float idFakeSF_tauid_pt35to40_up    = 1.0;
    float idFakeSF_tauid_pt40toInf_up   = 1.0;
    float idFakeSF_tauid_pt20to25_down  = 1.0;
    float idFakeSF_tauid_pt25to30_down  = 1.0;
    float idFakeSF_tauid_pt30to35_down  = 1.0;
    float idFakeSF_tauid_pt35to40_down  = 1.0;
    float idFakeSF_tauid_pt40toInf_down = 1.0;

    float idFakeSF_mutauFR_etaLt0p4_up      = 1.0;
    float idFakeSF_mutauFR_eta0p4to0p8_up   = 1.0;
    float idFakeSF_mutauFR_eta0p8to1p2_up   = 1.0;
    float idFakeSF_mutauFR_eta1p2to1p7_up   = 1.0;
    float idFakeSF_mutauFR_etaGt1p7_up      = 1.0;
    float idFakeSF_mutauFR_etaLt0p4_down    = 1.0;
    float idFakeSF_mutauFR_eta0p4to0p8_down = 1.0;
    float idFakeSF_mutauFR_eta0p8to1p2_down = 1.0;
    float idFakeSF_mutauFR_eta1p2to1p7_down = 1.0;
    float idFakeSF_mutauFR_etaGt1p7_down    = 1.0;

    float idFakeSF_etauFR_barrel_up   = 1.0;
    float idFakeSF_etauFR_endcap_up   = 1.0;
    float idFakeSF_etauFR_barrel_down = 1.0;
    float idFakeSF_etauFR_endcap_down = 1.0;

    // bool helpers for tauID syst bins
    vector<bool> isthisPt_IDbin_first = { // 20, 25, 30, 35, 40, infty
      ((theSmallTree.m_dau1_pt >= 20 && theSmallTree.m_dau1_pt < 25) ? true : false),
      ((theSmallTree.m_dau1_pt >= 25 && theSmallTree.m_dau1_pt < 30) ? true : false),
      ((theSmallTree.m_dau1_pt >= 30 && theSmallTree.m_dau1_pt < 35) ? true : false),
      ((theSmallTree.m_dau1_pt >= 35 && theSmallTree.m_dau1_pt < 40) ? true : false),
      ((theSmallTree.m_dau1_pt >= 40) ? true : false)
    };
    vector<bool> isthisPt_IDbin_second = { // 20, 25, 30, 35, 40, infty
      ((theSmallTree.m_dau2_pt >= 20 && theSmallTree.m_dau2_pt < 25) ? true : false),
      ((theSmallTree.m_dau2_pt >= 25 && theSmallTree.m_dau2_pt < 30) ? true : false),
      ((theSmallTree.m_dau2_pt >= 30 && theSmallTree.m_dau2_pt < 35) ? true : false),
      ((theSmallTree.m_dau2_pt >= 35 && theSmallTree.m_dau2_pt < 40) ? true : false),
      ((theSmallTree.m_dau2_pt >= 40) ? true : false)
    };

    vector<bool> isthisEta_IDbin_first = { // 0, 0.4, 0.8, 1.2, 1.7, infty
      ((fabs(theSmallTree.m_dau1_eta) >= 0   && fabs(theSmallTree.m_dau1_eta < 0.4)) ? true : false),
      ((fabs(theSmallTree.m_dau1_eta) >= 0.4 && fabs(theSmallTree.m_dau1_eta < 0.8)) ? true : false),
      ((fabs(theSmallTree.m_dau1_eta) >= 0.8 && fabs(theSmallTree.m_dau1_eta < 1.2)) ? true : false),
      ((fabs(theSmallTree.m_dau1_eta) >= 1.2 && fabs(theSmallTree.m_dau1_eta < 1.7)) ? true : false),
      ((fabs(theSmallTree.m_dau1_eta) >= 1.7) ? true : false)
    };
    vector<bool> isthisEta_IDbin_second = {  // 0, 0.4, 0.8, 1.2, 1.7, infty
      ((fabs(theSmallTree.m_dau2_eta) >= 0   && fabs(theSmallTree.m_dau2_eta < 0.4)) ? true : false),
      ((fabs(theSmallTree.m_dau2_eta) >= 0.4 && fabs(theSmallTree.m_dau2_eta < 0.8)) ? true : false),
      ((fabs(theSmallTree.m_dau2_eta) >= 0.8 && fabs(theSmallTree.m_dau2_eta < 1.2)) ? true : false),
      ((fabs(theSmallTree.m_dau2_eta) >= 1.2 && fabs(theSmallTree.m_dau2_eta < 1.7)) ? true : false),
      ((fabs(theSmallTree.m_dau2_eta) >= 1.7) ? true : false)
    };

    vector<bool> isthisSDet_IDbin_first = {  // barrel, endcap
      ((fabs(theSmallTree.m_dau1_eta) >= 0   && fabs(theSmallTree.m_dau1_eta < 1.46)) ? true : false),
      ((fabs(theSmallTree.m_dau1_eta) >= 1.46) ? true : false)
    };
    vector<bool> isthisSDet_IDbin_second = {  // barrel, endcap
      ((fabs(theSmallTree.m_dau2_eta) >= 0   && fabs(theSmallTree.m_dau2_eta < 1.46)) ? true : false),
      ((fabs(theSmallTree.m_dau2_eta) >= 1.46) ? true : false)
    };

	float leg1pt  = tlv_firstLepton.Pt();
	float leg1eta = TMath::Abs(tlv_firstLepton.Eta());
	float leg2pt  = tlv_secondLepton.Pt();
	float leg2eta = TMath::Abs(tlv_secondLepton.Eta());

	// the first tau only makes sense in the TauTau channel
	float tau1DM  = theSmallTree.m_dau1_decayMode;
	float tau2DM  = theSmallTree.m_dau2_decayMode;
	float tau1Genmatch = theBigTree.genmatch->at(firstDaughterIndex);
	float tau2Genmatch = theBigTree.genmatch->at(secondDaughterIndex);

	float idSF_leg1				  = 1.f;
	float idSF_leg1_deep_vsJet_dm = 1.f;
	float idSF_leg1_deep_vsJet_pt = 1.f;
	float idSF_leg1_deep_vsJet_2d = 1.f;
	float idSF_leg1_deep_vsEle    = 1.f;
	float idSF_leg1_deep_vsMu     = 1.f;

	float idSF_leg2				  = 1.f;
	float idSF_leg2_deep_vsJet_dm = 1.f;
	float idSF_leg2_deep_vsJet_pt = 1.f;
	float idSF_leg2_deep_vsJet_2d = 1.f;
	float idSF_leg2_deep_vsEle	  = 1.f;
	float idSF_leg2_deep_vsMu	  = 1.f;

	bool isFakeJet1 = true;
    bool isFakeJet2 = true;
	if (tau1Genmatch<6) {
	  isFakeJet1 = false;
	}
	if (tau2Genmatch<6) {
	  isFakeJet2 = false;
	}

	// only TauTau has a tau as the first leg
	if (isMC and pType==2)
	  {
		idSF_leg1_deep_vsJet_dm = Deep_antiJet_medium_dm->getSFvsDM(leg1pt, tau1DM, tau1Genmatch);
		idSF_leg1_deep_vsJet_pt = Deep_antiJet_medium_pt->getSFvsPT(leg1pt, tau1Genmatch);
		idSF_leg1_deep_vsJet_2d = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch);
		idSF_leg1_deep_vsEle    = Deep_antiEle_vvloose->getSFvsEta(leg1eta, tau1Genmatch);
		idSF_leg1_deep_vsMu     = Deep_antiMu_tight->getSFvsEta(leg1eta, tau1Genmatch);
	  }

	// all channels with one tau in the second leg
	if (isMC and (pType==0 or pType==1 or pType==2))
	  {
		idSF_leg2_deep_vsJet_dm = Deep_antiJet_medium_dm->getSFvsDM(leg2pt, tau2DM, tau2Genmatch);
		idSF_leg2_deep_vsJet_pt = Deep_antiJet_medium_pt->getSFvsPT(leg2pt, tau2Genmatch);
		idSF_leg2_deep_vsJet_2d = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch);
		idSF_leg2_deep_vsEle = Deep_antiEle_vvloose->getSFvsEta(leg2eta, tau2Genmatch);
		idSF_leg2_deep_vsMu  = Deep_antiMu_tight->getSFvsEta(leg2eta, tau2Genmatch);
	  }

	Float_t idSF_leg1_deep_vsJet_2d_stat0_up					= 1.f;
	Float_t idSF_leg1_deep_vsJet_2d_stat0_down					= 1.f;
	Float_t idSF_leg1_deep_vsJet_2d_stat1_up					= 1.f;
	Float_t idSF_leg1_deep_vsJet_2d_stat1_down					= 1.f;
	Float_t idSF_leg1_deep_vsJet_2d_systcorrdmeras_up			= 1.f;
	Float_t idSF_leg1_deep_vsJet_2d_systcorrdmeras_down			= 1.f;
	Float_t idSF_leg1_deep_vsJet_2d_systcorrdmuncorreras_up		= 1.f;
	Float_t idSF_leg1_deep_vsJet_2d_systcorrdmuncorreras_down	= 1.f;
	Float_t idSF_leg1_deep_vsJet_2d_systuncorrdmeras_up			= 1.f;
	Float_t idSF_leg1_deep_vsJet_2d_systuncorrdmeras_down		= 1.f;
	Float_t idSF_leg1_deep_vsJet_2d_systcorrerasgt140_up	    = 1.f;
	Float_t idSF_leg1_deep_vsJet_2d_systcorrerasgt140_down		= 1.f;
	Float_t idSF_leg1_deep_vsJet_2d_statgt140_up				= 1.f;
	Float_t idSF_leg1_deep_vsJet_2d_statgt140_down				= 1.f;
	Float_t idSF_leg1_deep_vsJet_2d_extrapgt140					= 1.f;

	Float_t idSF_leg2_deep_vsJet_2d_stat0_up					= 1.f;
	Float_t idSF_leg2_deep_vsJet_2d_stat0_down					= 1.f;
	Float_t idSF_leg2_deep_vsJet_2d_stat1_up					= 1.f;
	Float_t idSF_leg2_deep_vsJet_2d_stat1_down					= 1.f;
	Float_t idSF_leg2_deep_vsJet_2d_systcorrdmeras_up			= 1.f;
	Float_t idSF_leg2_deep_vsJet_2d_systcorrdmeras_down			= 1.f;
	Float_t idSF_leg2_deep_vsJet_2d_systcorrdmuncorreras_up		= 1.f;
	Float_t idSF_leg2_deep_vsJet_2d_systcorrdmuncorreras_down	= 1.f;
	Float_t idSF_leg2_deep_vsJet_2d_systuncorrdmeras_up			= 1.f;
	Float_t idSF_leg2_deep_vsJet_2d_systuncorrdmeras_down		= 1.f;
	Float_t idSF_leg2_deep_vsJet_2d_systcorrerasgt140_up	    = 1.f;
	Float_t idSF_leg2_deep_vsJet_2d_systcorrerasgt140_down		= 1.f;
	Float_t idSF_leg2_deep_vsJet_2d_statgt140_up				= 1.f;
	Float_t idSF_leg2_deep_vsJet_2d_statgt140_down				= 1.f;
	Float_t idSF_leg2_deep_vsJet_2d_extrapgt140					= 1.f;

	// up and down variations of the ID and isolation of the first leg (only relevant when it is a tau)
	vector<float> idSF_leg1_deep_vsJet_pt_up   (5, idSF_leg1_deep_vsJet_pt); // in bins of pt: 20, 25, 30, 35, 40, infty
	vector<float> idSF_leg1_deep_vsEle_up      (2, idSF_leg1_deep_vsEle);    // in bins of eta: barrel, endcap
	vector<float> idSF_leg1_deep_vsMu_up       (5, idSF_leg1_deep_vsMu);     // in bins of eta, edges at 0, 0.4, 0.8, 1.2, 1.7, infty
	vector<float> idSF_leg1_deep_vsJet_pt_down (5, idSF_leg1_deep_vsJet_pt); // in bins of pt: 20, 25, 30, 35, 40, infty
	vector<float> idSF_leg1_deep_vsEle_down    (2, idSF_leg1_deep_vsEle);    // in bins of eta: barrel, endcap
	vector<float> idSF_leg1_deep_vsMu_down     (5, idSF_leg1_deep_vsMu);     // in bins of eta, edges at 0, 0.4, 0.8, 1.2, 1.7, infty
	  
	// only TauTau has a tau as the first leg
	if (isMC and pType==2) {
      for (int bin = 0; bin < (int) isthisPt_IDbin_first.size(); bin++)	{
		if (isthisPt_IDbin_first[bin])
		  {
			idSF_leg1_deep_vsJet_pt_up[bin]   = Deep_antiJet_medium_pt ->getSFvsPT(leg1pt, tau1Genmatch,   "Up");
			idSF_leg1_deep_vsJet_pt_down[bin] = Deep_antiJet_medium_pt ->getSFvsPT(leg1pt, tau1Genmatch, "Down");

			//Additional uncertainty see https://twiki.cern.ch/twiki/bin/viewauth/CMS/TauIDRecommendationForRun2#Corrections_to_be_applied_to_gen
			double error_up   = fabs(idSF_leg1_deep_vsJet_pt - idSF_leg1_deep_vsJet_pt_up[bin]);
			double error_down = fabs(idSF_leg1_deep_vsJet_pt - idSF_leg1_deep_vsJet_pt_down[bin]);
			
			if (leg1pt < 100) {
				idSF_leg1_deep_vsJet_pt_up[bin]   = idSF_leg1_deep_vsJet_pt*1.03 + error_up;
				idSF_leg1_deep_vsJet_pt_down[bin] = idSF_leg1_deep_vsJet_pt*0.97 - error_down;
			}
			else {
			  idSF_leg1_deep_vsJet_pt_up[bin]   = idSF_leg1_deep_vsJet_pt*1.15 + error_up;
			  idSF_leg1_deep_vsJet_pt_down[bin] = idSF_leg1_deep_vsJet_pt*0.85 - error_down;
			}
		  }
      }
	
	  for (int bin = 0; bin < (int) isthisEta_IDbin_first.size(); bin++) {
		if (isthisEta_IDbin_first[bin])
		  {
			idSF_leg1_deep_vsMu_up[bin]   = Deep_antiMu_tight ->getSFvsEta(leg1eta, tau1Genmatch,   "Up");
			idSF_leg1_deep_vsMu_down[bin] = Deep_antiMu_tight ->getSFvsEta(leg1eta, tau1Genmatch, "Down");
		  }
	  }

      for (int bin = 0; bin < (int) isthisSDet_IDbin_first.size(); bin++) {
		if (isthisSDet_IDbin_first[bin])
		  {
			idSF_leg1_deep_vsEle_up[bin]   = Deep_antiEle_vvloose ->getSFvsEta(leg1eta, tau1Genmatch,   "Up");
			idSF_leg1_deep_vsEle_down[bin] = Deep_antiEle_vvloose ->getSFvsEta(leg1eta, tau1Genmatch, "Down");
		  }
	  }

	  idSF_leg1_deep_vsJet_2d_stat0_up					= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat0Up");
	  idSF_leg1_deep_vsJet_2d_stat0_down				= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat0Down");
	  idSF_leg1_deep_vsJet_2d_stat1_up					= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat1Up");
	  idSF_leg1_deep_vsJet_2d_stat1_down				= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat1Down");
	  idSF_leg1_deep_vsJet_2d_systcorrdmeras_up			= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystCorrDMErasUp");
	  idSF_leg1_deep_vsJet_2d_systcorrdmeras_down		= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystCorrDMErasDown");
	  idSF_leg1_deep_vsJet_2d_systcorrdmuncorreras_up	= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystCorrDMUncorrErasUp");
	  idSF_leg1_deep_vsJet_2d_systcorrdmuncorreras_down	= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystCorrDMUncorrErasDown");
	  idSF_leg1_deep_vsJet_2d_systuncorrdmeras_up		= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystUncorrDMErasUp");
	  idSF_leg1_deep_vsJet_2d_systuncorrdmeras_down		= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystUncorrDMErasDown");

	  idSF_leg1_deep_vsJet_2d_statgt140_up				= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Gt140StatUp");
	  idSF_leg1_deep_vsJet_2d_statgt140_down			= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Gt140StatDown");
	  idSF_leg1_deep_vsJet_2d_systcorrerasgt140_up		= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Gt140SystCorrErasUp");
	  idSF_leg1_deep_vsJet_2d_systcorrerasgt140_down	= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Gt140SystCorrErasDown");
	  idSF_leg1_deep_vsJet_2d_extrapgt140				= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Gt140Extrap");
	}

	// up and down variations of the ID and isolation of the second leg (only relevant when it is a tau)
	vector<float> idSF_leg2_deep_vsJet_pt_up   (5, idSF_leg2_deep_vsJet_pt); // in bins of pt: 20, 25, 30, 35, 40, infty
	vector<float> idSF_leg2_deep_vsEle_up      (2, idSF_leg2_deep_vsEle);    // in bins of eta: barrel, endcap
	vector<float> idSF_leg2_deep_vsMu_up       (5, idSF_leg2_deep_vsMu);     // in bins of eta, edges at 0, 0.4, 0.8, 1.2, 1.7, infty
	vector<float> idSF_leg2_deep_vsJet_pt_down (5, idSF_leg2_deep_vsJet_pt); // in bins of pt: 20, 25, 30, 35, 40, infty
	vector<float> idSF_leg2_deep_vsEle_down    (2, idSF_leg2_deep_vsEle);    // in bins of eta: barrel, endcap
	vector<float> idSF_leg2_deep_vsMu_down     (5, idSF_leg2_deep_vsMu);     // in bins of eta, edges at 0, 0.4, 0.8, 1.2, 1.7, infty

	if (isMC and (pType==0 or pType==1 or pType==2)) {
	  for (int bin = 0; bin < (int) isthisPt_IDbin_second.size(); bin++) {
		if (isthisPt_IDbin_second[bin])
		  {
			idSF_leg2_deep_vsJet_pt_up[bin]   = Deep_antiJet_medium_pt ->getSFvsPT(leg2pt, tau2Genmatch,   "Up");
			idSF_leg2_deep_vsJet_pt_down[bin] = Deep_antiJet_medium_pt ->getSFvsPT(leg2pt, tau2Genmatch, "Down");
		  }

		//Additional uncertainty see https://twiki.cern.ch/twiki/bin/viewauth/CMS/TauIDRecommendationForRun2#Corrections_to_be_applied_to_gen
		if (pType==2) {
		  double error_up   = fabs(idSF_leg2_deep_vsJet_pt - idSF_leg2_deep_vsJet_pt_up[bin]);
		  double error_down = fabs(idSF_leg2_deep_vsJet_pt - idSF_leg2_deep_vsJet_pt_down[bin]);
		  if (leg2pt < 100)
			{
			  idSF_leg2_deep_vsJet_pt_up[bin]   = idSF_leg2_deep_vsJet_pt*1.03 + error_up;
			  idSF_leg2_deep_vsJet_pt_down[bin] = idSF_leg2_deep_vsJet_pt*0.97 - error_down;
			}
		  else
			{
			  idSF_leg2_deep_vsJet_pt_up[bin]   = idSF_leg2_deep_vsJet_pt*1.15 + error_up;
			  idSF_leg2_deep_vsJet_pt_down[bin] = idSF_leg2_deep_vsJet_pt*0.85 - error_down;
			}
		}


		for (int bin = 0; bin < (int) isthisEta_IDbin_second.size(); bin++) {
		  if (isthisEta_IDbin_second[bin]) {
			idSF_leg2_deep_vsMu_up[bin]   = Deep_antiMu_tight->getSFvsEta(leg2eta, tau2Genmatch,	"Up");
			idSF_leg2_deep_vsMu_down[bin] = Deep_antiMu_tight->getSFvsEta(leg2eta, tau2Genmatch, "Down");
		  }
		}

		for (int bin = 0; bin < (int) isthisSDet_IDbin_second.size(); bin++) {
		  if (isthisSDet_IDbin_second[bin])	{
			idSF_leg2_deep_vsEle_up[bin]   = Deep_antiEle_vvloose ->getSFvsEta(leg2eta, tau2Genmatch,   "Up");
			idSF_leg2_deep_vsEle_down[bin] = Deep_antiEle_vvloose ->getSFvsEta(leg2eta, tau2Genmatch, "Down");
		  }
		}
		
	  }

	 idSF_leg2_deep_vsJet_2d_stat0_up					= Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat0Up");
	 idSF_leg2_deep_vsJet_2d_stat0_down					= Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat0Down");
	 idSF_leg2_deep_vsJet_2d_stat1_up					= Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat1Up");
	 idSF_leg2_deep_vsJet_2d_stat1_down					= Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat1Down");
	 idSF_leg2_deep_vsJet_2d_systcorrdmeras_up			= Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystCorrDMErasUp");
	 idSF_leg2_deep_vsJet_2d_systcorrdmeras_down		= Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystCorrDMErasDown");
	 idSF_leg2_deep_vsJet_2d_systcorrdmuncorreras_up	= Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystCorrDMUncorrErasUp");
	 idSF_leg2_deep_vsJet_2d_systcorrdmuncorreras_down	= Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystCorrDMUncorrErasDown");
	 idSF_leg2_deep_vsJet_2d_systuncorrdmeras_up		= Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystUncorrDMErasUp");
	 idSF_leg2_deep_vsJet_2d_systuncorrdmeras_down		= Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystUncorrDMErasDown");

	 idSF_leg2_deep_vsJet_2d_statgt140_up				= Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Gt140StatUp");
	 idSF_leg2_deep_vsJet_2d_statgt140_down				= Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Gt140StatDown");
	 idSF_leg2_deep_vsJet_2d_systcorrerasgt140_up		= Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Gt140SystCorrErasUp");
	 idSF_leg2_deep_vsJet_2d_systcorrerasgt140_down		= Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Gt140SystCorrErasDown");
	 idSF_leg2_deep_vsJet_2d_extrapgt140             	= Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Gt140Extrap");
	}
	
    if (isMC and leg1eta < 2.4) {
	  if (pType == 0 or pType == 3) {
		idSF_leg1 = myIDandISOScaleFactor[0]->get_ScaleFactor(leg1pt, leg1eta)*myIDandISOScaleFactor[2]->get_ScaleFactor(leg1pt, leg1eta);
	  }
	  else if (pType == 1 or pType == 4) {
		idSF_leg1 = myIDandISOScaleFactor[1]->get_direct_ScaleFactor(leg1pt, leg1eta);
	  }
	}

	if(isMC and leg2eta < 2.4 and pType == 3) { //MuMu
	  idSF_leg2 = myIDandISOScaleFactor[0]->get_ScaleFactor(leg2pt, leg2eta)*myIDandISOScaleFactor[2]->get_ScaleFactor(leg2pt, leg2eta);
	}
	else if(isMC and leg2eta < 2.4 and pType == 4) { //EleEle
	  idSF_leg2 = myIDandISOScaleFactor[1]->get_direct_ScaleFactor(leg2pt, leg2eta);
	}

	float except_VsJet, except_vsMu, except_vsEle;
	// EleTau and MuTau
	if (isMC and (pType == 0 or pType == 1))
	  {
		idSF_deep_dm	 = idSF_leg1 * idSF_leg2_deep_vsJet_dm;
		idSF_deep_pt	 = idSF_leg1 * idSF_leg2_deep_vsJet_pt;
		idSF_deep_2d	 = idSF_leg1 * idSF_leg2_deep_vsJet_2d;
		fakeRateSF_deep	 = idSF_leg2_deep_vsEle * idSF_leg2_deep_vsMu;

		except_VsJet = idSF_leg1 * idSF_leg2_deep_vsEle * idSF_leg2_deep_vsMu;
		except_vsMu	 = idSF_leg1 * idSF_leg2_deep_vsJet_2d * idSF_leg2_deep_vsEle;
		except_vsEle = idSF_leg1 * idSF_leg2_deep_vsJet_2d * idSF_leg2_deep_vsMu;
		
		idFakeSF_deep_dm = except_VsJet * idSF_leg2_deep_vsJet_dm;
		idFakeSF_deep_pt = except_VsJet * idSF_leg2_deep_vsJet_pt;
		idFakeSF_deep_2d = except_VsJet * idSF_leg2_deep_vsJet_2d;

		idFakeSF_tauid_2d_stat0_up					= except_VsJet * idSF_leg2_deep_vsJet_2d_stat0_up;
		idFakeSF_tauid_2d_stat0_down				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat0_down;
		idFakeSF_tauid_2d_stat1_up					= except_VsJet * idSF_leg2_deep_vsJet_2d_stat1_up;
		idFakeSF_tauid_2d_stat1_down				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat1_down;
		idFakeSF_tauid_2d_systcorrdmeras_up			= except_VsJet * idSF_leg2_deep_vsJet_2d_systcorrdmeras_up;
		idFakeSF_tauid_2d_systcorrdmeras_down		= except_VsJet * idSF_leg2_deep_vsJet_2d_systcorrdmeras_down;
		idFakeSF_tauid_2d_systcorrdmuncorreras_up	= except_VsJet * idSF_leg2_deep_vsJet_2d_systcorrdmuncorreras_up;
		idFakeSF_tauid_2d_systcorrdmuncorreras_down = except_VsJet * idSF_leg2_deep_vsJet_2d_systcorrdmuncorreras_down;
		idFakeSF_tauid_2d_systuncorrdmeras_up		= except_VsJet * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_up;
		idFakeSF_tauid_2d_systuncorrdmeras_down		= except_VsJet * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_down;

		idFakeSF_tauid_2d_systcorrerasgt140_up   	= except_VsJet * idSF_leg2_deep_vsJet_2d_systcorrerasgt140_up;
		idFakeSF_tauid_2d_systcorrerasgt140_down   	= except_VsJet * idSF_leg2_deep_vsJet_2d_systcorrerasgt140_down;
		idFakeSF_tauid_2d_statgt140_up		        = except_VsJet * idSF_leg2_deep_vsJet_2d_statgt140_up;
		idFakeSF_tauid_2d_statgt140_down		    = except_VsJet * idSF_leg2_deep_vsJet_2d_statgt140_down;
		idFakeSF_tauid_2d_extrapgt140 		        = except_VsJet * idSF_leg2_deep_vsJet_2d_extrapgt140;
		
		idFakeSF_tauid_pt20to25_up		= except_VsJet * idSF_leg2_deep_vsJet_pt_up[0];
		idFakeSF_tauid_pt25to30_up		= except_VsJet * idSF_leg2_deep_vsJet_pt_up[1];
		idFakeSF_tauid_pt30to35_up		= except_VsJet * idSF_leg2_deep_vsJet_pt_up[2];
		idFakeSF_tauid_pt35to40_up		= except_VsJet * idSF_leg2_deep_vsJet_pt_up[3];
		idFakeSF_tauid_pt40toInf_up		= except_VsJet * idSF_leg2_deep_vsJet_pt_up[4];
		idFakeSF_tauid_pt20to25_down	= except_VsJet * idSF_leg2_deep_vsJet_pt_down[0];
		idFakeSF_tauid_pt25to30_down	= except_VsJet * idSF_leg2_deep_vsJet_pt_down[1];
		idFakeSF_tauid_pt30to35_down	= except_VsJet * idSF_leg2_deep_vsJet_pt_down[2];
		idFakeSF_tauid_pt35to40_down	= except_VsJet * idSF_leg2_deep_vsJet_pt_down[3];
		idFakeSF_tauid_pt40toInf_down	= except_VsJet * idSF_leg2_deep_vsJet_pt_down[4];

		idFakeSF_mutauFR_etaLt0p4_up      = except_vsMu * idSF_leg2_deep_vsMu_up[0];
		idFakeSF_mutauFR_eta0p4to0p8_up   = except_vsMu * idSF_leg2_deep_vsMu_up[1];
		idFakeSF_mutauFR_eta0p8to1p2_up	  = except_vsMu * idSF_leg2_deep_vsMu_up[2];
		idFakeSF_mutauFR_eta1p2to1p7_up	  = except_vsMu * idSF_leg2_deep_vsMu_up[3];
		idFakeSF_mutauFR_etaGt1p7_up	  = except_vsMu * idSF_leg2_deep_vsMu_up[4];
		idFakeSF_mutauFR_etaLt0p4_down	  = except_vsMu * idSF_leg2_deep_vsMu_down[0];
		idFakeSF_mutauFR_eta0p4to0p8_down = except_vsMu * idSF_leg2_deep_vsMu_down[1];
		idFakeSF_mutauFR_eta0p8to1p2_down = except_vsMu * idSF_leg2_deep_vsMu_down[2];
		idFakeSF_mutauFR_eta1p2to1p7_down = except_vsMu * idSF_leg2_deep_vsMu_down[3];
		idFakeSF_mutauFR_etaGt1p7_down	  = except_vsMu * idSF_leg2_deep_vsMu_down[4];
	
		idFakeSF_etauFR_barrel_up   = except_vsEle * idSF_leg2_deep_vsEle_up[0];
		idFakeSF_etauFR_endcap_up	= except_vsEle * idSF_leg2_deep_vsEle_up[1];  
		idFakeSF_etauFR_barrel_down = except_vsEle * idSF_leg2_deep_vsEle_down[0];
		idFakeSF_etauFR_endcap_down = except_vsEle * idSF_leg2_deep_vsEle_down[1];
	  }
    else if (isMC and pType == 2) // TauTau
	  {
		idSF_deep_dm = idSF_leg1_deep_vsJet_dm * idSF_leg2_deep_vsJet_dm;
		idSF_deep_pt = idSF_leg1_deep_vsJet_pt * idSF_leg2_deep_vsJet_pt;
		idSF_deep_2d = idSF_leg1_deep_vsJet_2d * idSF_leg2_deep_vsJet_2d;

		except_VsJet = idSF_leg1_deep_vsEle * idSF_leg1_deep_vsMu * idSF_leg2_deep_vsEle * idSF_leg2_deep_vsMu;
		except_vsMu	 = idSF_leg1_deep_vsJet_2d * idSF_leg1_deep_vsEle * idSF_leg2_deep_vsJet_2d * idSF_leg2_deep_vsEle;
		except_vsEle = idSF_leg1_deep_vsJet_2d * idSF_leg1_deep_vsMu  * idSF_leg2_deep_vsJet_2d * idSF_leg2_deep_vsMu;

		idFakeSF_deep_dm = except_VsJet * idSF_deep_dm;
		idFakeSF_deep_pt = except_VsJet * idSF_deep_pt;
		idFakeSF_deep_2d = except_VsJet * idSF_deep_2d;
		fakeRateSF_deep	 = except_VsJet;

		idFakeSF_tauid_2d_stat0_up					= except_VsJet * idSF_leg1_deep_vsJet_2d_stat0_up				   * idSF_leg2_deep_vsJet_2d_stat0_up;
		idFakeSF_tauid_2d_stat0_down				= except_VsJet * idSF_leg1_deep_vsJet_2d_stat0_down				   * idSF_leg2_deep_vsJet_2d_stat0_down;
		idFakeSF_tauid_2d_stat1_up					= except_VsJet * idSF_leg1_deep_vsJet_2d_stat1_up				   * idSF_leg2_deep_vsJet_2d_stat1_up;
		idFakeSF_tauid_2d_stat1_down				= except_VsJet * idSF_leg1_deep_vsJet_2d_stat1_down				   * idSF_leg2_deep_vsJet_2d_stat1_down;
		idFakeSF_tauid_2d_systcorrdmeras_up			= except_VsJet * idSF_leg1_deep_vsJet_2d_systcorrdmeras_up		   * idSF_leg2_deep_vsJet_2d_systcorrdmeras_up;
		idFakeSF_tauid_2d_systcorrdmeras_down		= except_VsJet * idSF_leg1_deep_vsJet_2d_systcorrdmeras_down	   * idSF_leg2_deep_vsJet_2d_systcorrdmeras_down;
		idFakeSF_tauid_2d_systcorrdmuncorreras_up	= except_VsJet * idSF_leg1_deep_vsJet_2d_systcorrdmuncorreras_up   * idSF_leg2_deep_vsJet_2d_systcorrdmuncorreras_up;
		idFakeSF_tauid_2d_systcorrdmuncorreras_down = except_VsJet * idSF_leg1_deep_vsJet_2d_systcorrdmuncorreras_down * idSF_leg2_deep_vsJet_2d_systcorrdmuncorreras_down;
		idFakeSF_tauid_2d_systuncorrdmeras_up		= except_VsJet * idSF_leg1_deep_vsJet_2d_systuncorrdmeras_up	   * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_up;
		idFakeSF_tauid_2d_systuncorrdmeras_down		= except_VsJet * idSF_leg1_deep_vsJet_2d_systuncorrdmeras_down	   * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_down;

		idFakeSF_tauid_2d_systcorrerasgt140_up   	= except_VsJet * idSF_leg1_deep_vsJet_2d_systcorrerasgt140_up      * idSF_leg2_deep_vsJet_2d_systcorrerasgt140_up;
		idFakeSF_tauid_2d_systcorrerasgt140_down   	= except_VsJet * idSF_leg1_deep_vsJet_2d_systcorrerasgt140_down    * idSF_leg2_deep_vsJet_2d_systcorrerasgt140_down;
		idFakeSF_tauid_2d_statgt140_up        		= except_VsJet * idSF_leg1_deep_vsJet_2d_statgt140_up              * idSF_leg2_deep_vsJet_2d_statgt140_up;
		idFakeSF_tauid_2d_statgt140_down          	= except_VsJet * idSF_leg1_deep_vsJet_2d_statgt140_down            * idSF_leg2_deep_vsJet_2d_statgt140_down;
		idFakeSF_tauid_2d_extrapgt140              	= except_VsJet * idSF_leg1_deep_vsJet_2d_extrapgt140               * idSF_leg2_deep_vsJet_2d_extrapgt140;

		idFakeSF_tauid_pt20to25_up    = except_VsJet * idSF_leg1_deep_vsJet_pt_up[0]   * idSF_leg2_deep_vsJet_pt_up[0];
		idFakeSF_tauid_pt25to30_up    = except_VsJet * idSF_leg1_deep_vsJet_pt_up[1]   * idSF_leg2_deep_vsJet_pt_up[1];
		idFakeSF_tauid_pt30to35_up    = except_VsJet * idSF_leg1_deep_vsJet_pt_up[2]   * idSF_leg2_deep_vsJet_pt_up[2];
		idFakeSF_tauid_pt35to40_up    = except_VsJet * idSF_leg1_deep_vsJet_pt_up[3]   * idSF_leg2_deep_vsJet_pt_up[3];
		idFakeSF_tauid_pt40toInf_up   = except_VsJet * idSF_leg1_deep_vsJet_pt_up[4]   * idSF_leg2_deep_vsJet_pt_up[4];
		idFakeSF_tauid_pt20to25_down  = except_VsJet * idSF_leg1_deep_vsJet_pt_down[0] * idSF_leg2_deep_vsJet_pt_down[0];
		idFakeSF_tauid_pt25to30_down  = except_VsJet * idSF_leg1_deep_vsJet_pt_down[1] * idSF_leg2_deep_vsJet_pt_down[1];
		idFakeSF_tauid_pt30to35_down  = except_VsJet * idSF_leg1_deep_vsJet_pt_down[2] * idSF_leg2_deep_vsJet_pt_down[2];
		idFakeSF_tauid_pt35to40_down  = except_VsJet * idSF_leg1_deep_vsJet_pt_down[3] * idSF_leg2_deep_vsJet_pt_down[3];
		idFakeSF_tauid_pt40toInf_down = except_VsJet * idSF_leg1_deep_vsJet_pt_down[4] * idSF_leg2_deep_vsJet_pt_down[4];

		idFakeSF_mutauFR_etaLt0p4_up      = except_vsMu * idSF_leg1_deep_vsMu_up[0]   * idSF_leg2_deep_vsMu_up[0];
		idFakeSF_mutauFR_eta0p4to0p8_up   = except_vsMu * idSF_leg1_deep_vsMu_up[1]   * idSF_leg2_deep_vsMu_up[1];
		idFakeSF_mutauFR_eta0p8to1p2_up   = except_vsMu * idSF_leg1_deep_vsMu_up[2]   * idSF_leg2_deep_vsMu_up[2];
		idFakeSF_mutauFR_eta1p2to1p7_up   = except_vsMu * idSF_leg1_deep_vsMu_up[3]   * idSF_leg2_deep_vsMu_up[3];
		idFakeSF_mutauFR_etaGt1p7_up      = except_vsMu * idSF_leg1_deep_vsMu_up[4]   * idSF_leg2_deep_vsMu_up[4];
		idFakeSF_mutauFR_etaLt0p4_down    = except_vsMu * idSF_leg1_deep_vsMu_down[0] * idSF_leg2_deep_vsMu_down[0];
		idFakeSF_mutauFR_eta0p4to0p8_down = except_vsMu * idSF_leg1_deep_vsMu_down[1] * idSF_leg2_deep_vsMu_down[1];
		idFakeSF_mutauFR_eta0p8to1p2_down = except_vsMu * idSF_leg1_deep_vsMu_down[2] * idSF_leg2_deep_vsMu_down[2];
		idFakeSF_mutauFR_eta1p2to1p7_down = except_vsMu * idSF_leg1_deep_vsMu_down[3] * idSF_leg2_deep_vsMu_down[3];
		idFakeSF_mutauFR_etaGt1p7_down    = except_vsMu * idSF_leg1_deep_vsMu_down[4] * idSF_leg2_deep_vsMu_down[4];

		idFakeSF_etauFR_barrel_up   = except_vsEle * idSF_leg1_deep_vsEle_up[0]   * idSF_leg2_deep_vsEle_up[0];
		idFakeSF_etauFR_endcap_up   = except_vsEle * idSF_leg1_deep_vsEle_up[1]   * idSF_leg2_deep_vsEle_up[1];
		idFakeSF_etauFR_barrel_down = except_vsEle * idSF_leg1_deep_vsEle_down[0] * idSF_leg2_deep_vsEle_down[0];
		idFakeSF_etauFR_endcap_down = except_vsEle * idSF_leg1_deep_vsEle_down[1] * idSF_leg2_deep_vsEle_down[1];
	  }
	else if(isMC and (pType == 3 or pType == 4))  // MuMu and EleELe channels
	  {
		idSF_deep_dm = idFakeSF_deep_dm = idSF_leg1 * idSF_leg2;
	  }
	
	if (DEBUG) {
	  cout << "--- DEBUG idSF ---" << endl;
	  cout << "pairType  : "              << pType                   << endl;
	  cout << "totSF deep_dm: "           << idFakeSF_deep_dm        << endl;
	  cout << "totSF deep_pt: "           << idFakeSF_deep_pt        << endl;
	  cout << "totSF deep_2d: "           << idFakeSF_deep_2d        << endl;
	  cout << "idSF_leg1: "               << idSF_leg1               << endl;
	  cout << "idSF_leg1_deep_vsJet_dm: " << idSF_leg1_deep_vsJet_dm << endl;
	  cout << "idSF_leg1_deep_vsJet_pt: " << idSF_leg1_deep_vsJet_pt << endl;
	  cout << "idSF_leg1_deep_vsJet_2d: " << idSF_leg1_deep_vsJet_2d << endl;
	  cout << "idSF_leg1_deep_vsEle: "    << idSF_leg1_deep_vsEle    << endl;
	  cout << "idSF_leg1_deep_vsMu: "     << idSF_leg1_deep_vsMu     << endl;
	  cout << "idSF_leg2_deep_vsJet_dm: " << idSF_leg2_deep_vsJet_dm << endl;
	  cout << "idSF_leg2_deep_vsJet_pt: " << idSF_leg2_deep_vsJet_pt << endl;
	  cout << "idSF_leg2_deep_vsJet_2d: " << idSF_leg2_deep_vsJet_2d << endl;
	  cout << "idSF_leg2_deep_vsEle: "    << idSF_leg2_deep_vsEle    << endl;
	  cout << "idSF_leg2_deep_vsMu: "     << idSF_leg2_deep_vsMu     << endl;
	}

    // Save the IDandISO SF (event per event)
	theSmallTree.m_IdSF_deep_dm		= idSF_deep_dm;
	theSmallTree.m_IdSF_deep_pt		= idSF_deep_pt;
	theSmallTree.m_IdSF_deep_2d		= idSF_deep_2d;
	theSmallTree.m_IdFakeSF_deep_dm	= idFakeSF_deep_dm;
	theSmallTree.m_IdFakeSF_deep_pt	= idFakeSF_deep_pt;
	theSmallTree.m_IdFakeSF_deep_2d	= idFakeSF_deep_2d;
	theSmallTree.m_FakeRateSF_deep	= fakeRateSF_deep;

	theSmallTree.m_idFakeSF_tauid_2d_stat0_up					= idFakeSF_tauid_2d_stat0_up;
	theSmallTree.m_idFakeSF_tauid_2d_stat0_down					= idFakeSF_tauid_2d_stat0_down;
	theSmallTree.m_idFakeSF_tauid_2d_stat1_up					= idFakeSF_tauid_2d_stat1_up;
	theSmallTree.m_idFakeSF_tauid_2d_stat1_down					= idFakeSF_tauid_2d_stat1_down;
	theSmallTree.m_idFakeSF_tauid_2d_systcorrdmeras_up			= idFakeSF_tauid_2d_systcorrdmeras_up;
	theSmallTree.m_idFakeSF_tauid_2d_systcorrdmeras_down		= idFakeSF_tauid_2d_systcorrdmeras_down;
	theSmallTree.m_idFakeSF_tauid_2d_systcorrdmuncorreras_up	= idFakeSF_tauid_2d_systcorrdmuncorreras_up;
	theSmallTree.m_idFakeSF_tauid_2d_systcorrdmuncorreras_down	= idFakeSF_tauid_2d_systcorrdmuncorreras_down;
	theSmallTree.m_idFakeSF_tauid_2d_systuncorrdmeras_up		= idFakeSF_tauid_2d_systuncorrdmeras_up;
	theSmallTree.m_idFakeSF_tauid_2d_systuncorrdmeras_down		= idFakeSF_tauid_2d_systuncorrdmeras_down;

	theSmallTree.m_idFakeSF_tauid_2d_systcorrerasgt140_up		= idFakeSF_tauid_2d_systcorrerasgt140_up;
	theSmallTree.m_idFakeSF_tauid_2d_systcorrerasgt140_down		= idFakeSF_tauid_2d_systcorrerasgt140_down;
	theSmallTree.m_idFakeSF_tauid_2d_statgt140_up				= idFakeSF_tauid_2d_statgt140_up;
	theSmallTree.m_idFakeSF_tauid_2d_statgt140_down				= idFakeSF_tauid_2d_statgt140_down;
	theSmallTree.m_idFakeSF_tauid_2d_extrapgt140				= idFakeSF_tauid_2d_extrapgt140;

	theSmallTree.m_idFakeSF_tauid_pt20to25_up		= idFakeSF_tauid_pt20to25_up;	
	theSmallTree.m_idFakeSF_tauid_pt25to30_up		= idFakeSF_tauid_pt25to30_up;		
	theSmallTree.m_idFakeSF_tauid_pt30to35_up		= idFakeSF_tauid_pt30to35_up;		
	theSmallTree.m_idFakeSF_tauid_pt35to40_up		= idFakeSF_tauid_pt35to40_up;		
	theSmallTree.m_idFakeSF_tauid_pt40toInf_up		= idFakeSF_tauid_pt40toInf_up;		
	theSmallTree.m_idFakeSF_tauid_pt20to25_down		= idFakeSF_tauid_pt20to25_down;		
	theSmallTree.m_idFakeSF_tauid_pt25to30_down		= idFakeSF_tauid_pt25to30_down;	
	theSmallTree.m_idFakeSF_tauid_pt30to35_down		= idFakeSF_tauid_pt30to35_down;	
	theSmallTree.m_idFakeSF_tauid_pt35to40_down		= idFakeSF_tauid_pt35to40_down;		
	theSmallTree.m_idFakeSF_tauid_pt40toInf_down	= idFakeSF_tauid_pt40toInf_down;	

	theSmallTree.m_idFakeSF_mutauFR_etaLt0p4_up		 = idFakeSF_mutauFR_etaLt0p4_up;		
	theSmallTree.m_idFakeSF_mutauFR_eta0p4to0p8_up	 = idFakeSF_mutauFR_eta0p4to0p8_up;	
	theSmallTree.m_idFakeSF_mutauFR_eta0p8to1p2_up	 = idFakeSF_mutauFR_eta0p8to1p2_up;	
	theSmallTree.m_idFakeSF_mutauFR_eta1p2to1p7_up	 = idFakeSF_mutauFR_eta1p2to1p7_up;	
	theSmallTree.m_idFakeSF_mutauFR_etaGt1p7_up		 = idFakeSF_mutauFR_etaGt1p7_up;		
	theSmallTree.m_idFakeSF_mutauFR_etaLt0p4_down	 = idFakeSF_mutauFR_etaLt0p4_down;	
	theSmallTree.m_idFakeSF_mutauFR_eta0p4to0p8_down = idFakeSF_mutauFR_eta0p4to0p8_down;
	theSmallTree.m_idFakeSF_mutauFR_eta0p8to1p2_down = idFakeSF_mutauFR_eta0p8to1p2_down;
	theSmallTree.m_idFakeSF_mutauFR_eta1p2to1p7_down = idFakeSF_mutauFR_eta1p2to1p7_down;
	theSmallTree.m_idFakeSF_mutauFR_etaGt1p7_down	 = idFakeSF_mutauFR_etaGt1p7_down;
	theSmallTree.m_idFakeSF_etauFR_barrel_up		 = idFakeSF_etauFR_barrel_up;	
	theSmallTree.m_idFakeSF_etauFR_endcap_up		 = idFakeSF_etauFR_endcap_up;		
	theSmallTree.m_idFakeSF_etauFR_barrel_down		 = idFakeSF_etauFR_barrel_down;		
	theSmallTree.m_idFakeSF_etauFR_endcap_down		 = idFakeSF_etauFR_endcap_down;

    //Jet faking Tau SF
    //derived from WJet sideband: http://camendol.web.cern.ch/camendol/HH2017/plotsHH2017MuTau/31Oct2018_DYNLO_ctrlWJets_SS/antiB_jets30_tau30_SStight/
    double jetFakeSF1 = 1.;
    double jetFakeSF2 = 1.;
    if (isMC and (pType == 0 or pType == 1 or pType == 2)) { // 2nd tau
      if (isFakeJet2)
		{
		  if(fabs(tlv_secondLepton.Eta()) < 1.46) jetFakeSF2 = 1.38469;
		  else jetFakeSF2 = 1.69035;
		}
	  if (pType == 2) { //1st tau
		if (isFakeJet1)
		  {
			if(fabs(tlv_firstLepton.Eta()) < 1.46) jetFakeSF1 = 1.38469;
			else jetFakeSF1 = 1.69035;
		  }
	  }
    }
    theSmallTree.m_jetFakeSF = jetFakeSF1*jetFakeSF2;

	
    // DATA/MC Trigger ScaleFactors
    // https://github.com/CMS-HTT/LeptonEfficiencies
    // https://github.com/truggles/TauTriggerSFs2017

    // recommendations for cross triggers:  https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2017#Trigger_Information

    float trigSF			= 1.0;
    float trigSF_ele_up		= 1.0;
    float trigSF_mu_up		= 1.0;
    float trigSF_DM0_up		= 1.0;
    float trigSF_DM1_up		= 1.0;
    float trigSF_DM10_up	= 1.0;
    float trigSF_DM11_up	= 1.0;
    float trigSF_ele_down	= 1.0;
    float trigSF_mu_down	= 1.0;
    float trigSF_DM0_down	= 1.0;
    float trigSF_DM1_down	= 1.0;
    float trigSF_DM10_down	= 1.0;
    float trigSF_DM11_down	= 1.0;
    float trigSF_single		= 1.0;
    float trigSF_cross		= 1.0;

    if(applyTriggers)
    {
      // MuTau Channel
      if (pType == 0 && isMC)
      {
	if(fabs(tlv_secondLepton.Eta()) < 2.1) //eta region covered both by cross-trigger and single lepton trigger
	{
	  int passCross = 1;
	  int passSingle = 1;

	  if (tlv_firstLepton.Pt() < 23.) passSingle = 0;
	  if (tlv_secondLepton.Pt() < 25.) passCross = 0;

	  //lepton trigger
	  double SFL_Data = muTrgSF->get_EfficiencyData(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
	  double SFL_MC = muTrgSF->get_EfficiencyMC(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());

	  double SFL_Data_Err = muTrgSF->get_EfficiencyDataError(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
	  double SFL_MC_Err = muTrgSF->get_EfficiencyMCError(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());

	  double SFL_Data_up   = SFL_Data + 1. * SFL_Data_Err;
	  double SFL_Data_down = SFL_Data - 1. * SFL_Data_Err;
	  double SFL_MC_up     = SFL_MC   + 1. * SFL_MC_Err;
	  double SFL_MC_down   = SFL_MC   - 1. * SFL_MC_Err;

	  //cross-trigger
	  //mu leg
	  double SFl_Data = muTauTrgSF->get_EfficiencyData(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
	  double SFl_MC = muTauTrgSF->get_EfficiencyMC(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());

	  double SFl_Data_Err = muTauTrgSF->get_EfficiencyDataError(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
	  double SFl_MC_Err = muTauTrgSF->get_EfficiencyMCError(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());

	  double SFl_Data_up   = SFl_Data + 1. * SFl_Data_Err;
	  double SFl_Data_down = SFl_Data - 1. * SFl_Data_Err;
	  double SFl_MC_up     = SFl_MC   + 1. * SFl_MC_Err;
	  double SFl_MC_down   = SFl_MC   - 1. * SFl_MC_Err;

	  //tau leg
	  double SFtau_Data = tauTrgSF_mutau->getEfficiencyData(tlv_secondLepton.Pt(), DM2, 0); // last entry is uncertainty: 0 central, +1 up, -1 down
	  double SFtau_MC   = tauTrgSF_mutau->getEfficiencyMC  (tlv_secondLepton.Pt(), DM2, 0); // last entry is uncertainty: 0 central, +1 up, -1 down

	  // Compute efficiencies
	  double Eff_Data = passSingle * SFL_Data - passCross * passSingle * std::min(SFl_Data, SFL_Data) * SFtau_Data + passCross * SFl_Data * SFtau_Data;
	  double Eff_MC   = passSingle * SFL_MC   - passCross * passSingle * std::min(SFl_MC  , SFL_MC)   * SFtau_MC   + passCross * SFl_MC   * SFtau_MC;

	  double Eff_Data_mu_up   = passSingle * SFL_Data_up   - passCross * passSingle * std::min(SFl_Data_up  , SFL_Data_up)   * SFtau_Data + passCross * SFl_Data_up   * SFtau_Data;
	  double Eff_MC_mu_up     = passSingle * SFL_MC_up     - passCross * passSingle * std::min(SFl_MC_up    , SFL_MC_up)     * SFtau_MC   + passCross * SFl_MC_up     * SFtau_MC;
	  double Eff_Data_mu_down = passSingle * SFL_Data_down - passCross * passSingle * std::min(SFl_Data_down, SFL_Data_down) * SFtau_Data + passCross * SFl_Data_down * SFtau_Data;
	  double Eff_MC_mu_down   = passSingle * SFL_MC_down   - passCross * passSingle * std::min(SFl_MC_down  , SFL_MC_down)   * SFtau_MC   + passCross * SFl_MC_down   * SFtau_MC;

	  // for each DM, fill a trigSF branch with the up/down values if the tauh has the corresponding DM, otherwise fill with nominal trigSF value
	  vector <double> SFtau_Data_up   (N_tauhDM, SFtau_Data);
	  vector <double> SFtau_Data_down (N_tauhDM, SFtau_Data);
	  vector <double> SFtau_MC_up     (N_tauhDM, SFtau_MC);
	  vector <double> SFtau_MC_down   (N_tauhDM, SFtau_MC);
	  vector <double> Eff_Data_up     (N_tauhDM, Eff_Data);
	  vector <double> Eff_Data_down   (N_tauhDM, Eff_Data);
	  vector <double> Eff_MC_up       (N_tauhDM, Eff_MC);
	  vector <double> Eff_MC_down     (N_tauhDM, Eff_MC);

	  for (int idm  = 0; idm < N_tauhDM; idm ++)
	  {
	    if (isthisDM_second[idm])
	    {
	      SFtau_Data_up[idm]   = tauTrgSF_mutau->getEfficiencyData(tlv_secondLepton.Pt(), DM2, 1);
	      SFtau_Data_down[idm] = tauTrgSF_mutau->getEfficiencyData(tlv_secondLepton.Pt(), DM2, -1);
	      SFtau_MC_up[idm]     = tauTrgSF_mutau->getEfficiencyMC  (tlv_secondLepton.Pt(), DM2, 1);
	      SFtau_MC_down[idm]   = tauTrgSF_mutau->getEfficiencyMC  (tlv_secondLepton.Pt(), DM2, -1);
	    }
	    Eff_Data_up[idm]   = passSingle * SFL_Data - passCross * passSingle * std::min(SFl_Data, SFL_Data) * SFtau_Data_up[idm]   + passCross * SFl_Data * SFtau_Data_up[idm];
	    Eff_Data_down[idm] = passSingle * SFL_Data - passCross * passSingle * std::min(SFl_Data, SFL_Data) * SFtau_Data_down[idm] + passCross * SFl_Data * SFtau_Data_down[idm];
	    Eff_MC_up[idm]     = passSingle * SFL_MC   - passCross * passSingle * std::min(SFl_MC  , SFL_MC)   * SFtau_MC_up[idm]     + passCross * SFl_MC   * SFtau_MC_up[idm];
	    Eff_MC_down[idm]   = passSingle * SFL_MC   - passCross * passSingle * std::min(SFl_MC  , SFL_MC)   * SFtau_MC_down[idm]   + passCross * SFl_MC   * SFtau_MC_down[idm];
	  }

	  if(DEBUG)
	  {
	    cout << "--- DEBUG Trigger weights --- " << endl;
	    cout << "SFL_Data: "   << SFL_Data   << endl;
	    cout << "SFL_MC: "     << SFL_MC     << endl;
	    cout << "SFl_Data: "   << SFl_Data   << endl;
	    cout << "SFl_MC: "     << SFl_MC     << endl;
	    cout << "SFtau_Data: " << SFtau_Data << endl;
	    cout << "SFtau_MC: "   << SFtau_MC   << endl;
	  }

	  trigSF = Eff_Data / Eff_MC;
	  trigSF_mu_up     = Eff_Data_mu_up / Eff_MC_mu_up;
	  trigSF_mu_down   = Eff_Data_mu_down / Eff_MC_mu_down;
	  trigSF_DM0_up    = Eff_Data_up[0] / Eff_MC_up[0];
	  trigSF_DM1_up    = Eff_Data_up[1] / Eff_MC_up[1];
	  trigSF_DM10_up   = Eff_Data_up[2] / Eff_MC_up[2];
	  trigSF_DM11_up   = Eff_Data_up[3] / Eff_MC_up[3];
	  trigSF_DM0_down  = Eff_Data_down[0] / Eff_MC_down[0];
	  trigSF_DM1_down  = Eff_Data_down[1] / Eff_MC_down[1];
	  trigSF_DM10_down = Eff_Data_down[2] / Eff_MC_down[2];
	  trigSF_DM11_down = Eff_Data_down[3] / Eff_MC_down[3];

	  //trig SF for analysis only with cross-trigger
	  double SFl = muTauTrgSF->get_ScaleFactor(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
	  double SFtau = tauTrgSF_mutau->getSF(tlv_secondLepton.Pt(), DM2, 0); // last entry is uncertainty: 0 central, +1 up, -1 down
	  trigSF_cross = SFl*SFtau;
	}
	else //eta region covered only by single lepton trigger
	{
	  double SF = muTrgSF->get_ScaleFactor(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
	  double SF_Err = muTrgSF->get_ScaleFactorError(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
	  trigSF = SF;
	  trigSF_mu_up   = SF + 1. * SF_Err;
	  trigSF_mu_down = SF - 1. * SF_Err;
	}
	//trig SF for analysis only with single-mu trigger
	trigSF_single =  muTrgSF->get_ScaleFactor(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
      }

      // EleTau Channel
      else if (pType == 1 && isMC)
      {
	// In 2016 there is no cross etau trigger --> only single ele trigger SF is needed
	double SF = eTrgSF->get_ScaleFactor(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
	double SF_Err = eTrgSF->get_ScaleFactorError(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
	trigSF = SF;
	trigSF_ele_up   = SF + 1. * SF_Err;
	trigSF_ele_down = SF - 1. * SF_Err;

	trigSF_single = eTrgSF->get_ScaleFactor(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
      }

      // TauTau Channel
      else if (pType == 2 && isMC)
      {
	double SF1 = tauTrgSF_ditau->getSF(tlv_firstLepton.Pt() , DM1, 0); // last entry is uncertainty: 0 central, +1 up, -1 down
	double SF2 = tauTrgSF_ditau->getSF(tlv_secondLepton.Pt(), DM2, 0); // last entry is uncertainty: 0 central, +1 up, -1 down

	// for each DM, fill a trigSF branch with the up/down values if tauhs have the corresponding DM, otherwise fill with nominal trigSF value
	vector <double> SF1_up (N_tauhDM, SF1);
	vector <double> SF2_up (N_tauhDM, SF2);
	vector <double> SF1_down (N_tauhDM, SF1);
	vector <double> SF2_down (N_tauhDM, SF2);

	for (int idm  = 0; idm < N_tauhDM; idm ++)
	{
	  if (isthisDM_first[idm])
	  {
	    SF1_up[idm]   = tauTrgSF_ditau->getSF(tlv_firstLepton.Pt(), DM1, 1);
	    SF1_down[idm] = tauTrgSF_ditau->getSF(tlv_firstLepton.Pt(), DM1, -1);
	  }
	  if (isthisDM_second[idm])
	  {
	    SF2_up[idm]   = tauTrgSF_ditau->getSF(tlv_secondLepton.Pt(), DM2, 1);
	    SF2_down[idm] = tauTrgSF_ditau->getSF(tlv_secondLepton.Pt(), DM2, -1);
	  }
	}

	trigSF = SF1 * SF2;
	trigSF_DM0_up    = SF1_up[0]   *	SF2_up[0];
	trigSF_DM1_up    = SF1_up[1]   *	SF2_up[1];
	trigSF_DM10_up   = SF1_up[2]   *	SF2_up[2];
	trigSF_DM11_up   = SF1_up[3]   *	SF2_up[3];
	trigSF_DM0_down  = SF1_down[0] *	SF2_down[0];
	trigSF_DM1_down  = SF1_down[1] *	SF2_down[1];
	trigSF_DM10_down = SF1_down[2] *	SF2_down[2];
	trigSF_DM11_down = SF1_down[3] *	SF2_down[3];
      }

      // MuMu Channel
      else if (pType == 3 && isMC)
      {
	double SF = muTrgSF->get_ScaleFactor(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
	double SF_Err = muTrgSF->get_ScaleFactorError(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
	trigSF = SF;
	trigSF_mu_up   = SF + 1. * SF_Err;
	trigSF_mu_down = SF - 1. * SF_Err;
      }

      // EleEle Channel
      else if (pType == 4 && isMC)
      {
	double SF = eTrgSF->get_ScaleFactor(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
	double SF_Err = eTrgSF->get_ScaleFactorError(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
	trigSF = SF;
	trigSF_ele_up   = SF + 1. * SF_Err;
	trigSF_ele_down = SF - 1. * SF_Err;
      }
    } // end if(applytriggers)

    theSmallTree.m_trigSF           = (isMC ? trigSF : 1.0);
    theSmallTree.m_trigSF_ele_up    = (isMC ? trigSF_ele_up : 1.0);
    theSmallTree.m_trigSF_mu_up     = (isMC ? trigSF_mu_up : 1.0);
    theSmallTree.m_trigSF_DM0_up    = (isMC ? trigSF_DM0_up : 1.0);
    theSmallTree.m_trigSF_DM1_up    = (isMC ? trigSF_DM1_up : 1.0);
    theSmallTree.m_trigSF_DM10_up   = (isMC ? trigSF_DM10_up : 1.0);
    theSmallTree.m_trigSF_DM11_up   = (isMC ? trigSF_DM11_up : 1.0);
    theSmallTree.m_trigSF_ele_down  = (isMC ? trigSF_ele_down : 1.0);
    theSmallTree.m_trigSF_mu_down   = (isMC ? trigSF_mu_down : 1.0);
    theSmallTree.m_trigSF_DM0_down  = (isMC ? trigSF_DM0_down : 1.0);
    theSmallTree.m_trigSF_DM1_down  = (isMC ? trigSF_DM1_down : 1.0);
    theSmallTree.m_trigSF_DM10_down = (isMC ? trigSF_DM10_down : 1.0);
    theSmallTree.m_trigSF_DM11_down = (isMC ? trigSF_DM11_down : 1.0);
    theSmallTree.m_trigSF_single = (isMC ? trigSF_single : 1.0);
    theSmallTree.m_trigSF_cross  = (isMC ? trigSF_cross : 1.0);

    theSmallTree.m_totalWeight = (isMC? (35920./7.20811e+10) * theSmallTree.m_MC_weight * theSmallTree.m_PUReweight * theSmallTree.m_DYscale_MTT * trigSF * theSmallTree.m_IdFakeSF_deep_pt * theSmallTree.m_L1pref_weight: 1.0);
    //total weight used for sync: the denominator must be changed for each sample as h_eff->GetBinContent(1), the numerator is the luminosity

    // Third lepton veto
    // loop over leptons
    vector<pair<float, int> > thirdLeptons ; // pt, idx
    for (unsigned int iLep = 0 ; (iLep < theBigTree.daughters_px->size ()) ; ++iLep)
    {
      // skip the H decay candiates
      if (int (iLep) == firstDaughterIndex || int (iLep) == secondDaughterIndex) continue;

      // remove taus
      if (theBigTree.particleType->at (iLep) == 2)
      {
	continue ;
      }
      else if (theBigTree.particleType->at (iLep) == 0) // muons
      {
	// Fra Mar2020: for muon, Tight does not imply Medium so we check both
	bool passMed = oph.muBaseline (&theBigTree, iLep, 10., 2.4, 0.3, OfflineProducerHelper::MuMedium);
	bool passTig = oph.muBaseline (&theBigTree, iLep, 10., 2.4, 0.3, OfflineProducerHelper::MuTight);
	if (!passMed && !passTig) continue; // if it passes one of the two --> the "if" is false and the lepton is saved as an extra lepton
      }
      else if (theBigTree.particleType->at (iLep) == 1) // electrons
      {
	// Fra Mar2020: for electron, we check (mvaEleID-Fall17-iso-V2-wp90 OR (mvaEleID-Fall17-noIso-V2-wp90 AND pfRelIso < 0.3))
	if (DEBUG) std::cout << "--- Debug for extra electrons:" << std::endl;
	bool passIsoMVA = oph.eleBaseline (&theBigTree, iLep, 10., 2.5, 0.3, OfflineProducerHelper::EMVAMedium, string("Vertex-LepID-pTMin-etaMax"), (DEBUG ? true : false));
	//bool passNonISo = oph.eleBaseline (&theBigTree, iLep, 10., 2.5, 0.3, OfflineProducerHelper::EMVAMedium, string("Vertex-pTMin-etaMax-thirdLep"), (DEBUG ? true : false));
	if (!passIsoMVA) continue; // if it passes --> the "if" is false and the lepton is saved as an extra lepton
      }

      TLorentzVector tlv_dummyLepton
        (
          theBigTree.daughters_px->at (iLep),
          theBigTree.daughters_py->at (iLep),
          theBigTree.daughters_pz->at (iLep),
          theBigTree.daughters_e->at (iLep)
	  );
      thirdLeptons.push_back (make_pair(tlv_dummyLepton.Pt(), iLep)) ;

      if(DEBUG)
      {
	cout << "** 3rd lep veto passed"
	     << " idx="  << iLep
	     << " type=" << theBigTree.particleType->at(iLep)
	     << " pt="   << tlv_dummyLepton.Pt()
	     << " eta="  << tlv_dummyLepton.Eta()
	     << " phi="  << tlv_dummyLepton.Phi()
	     << " iso="  << getIso (iLep, tlv_dummyLepton.Pt (), theBigTree)
	     << " dxy="  << theBigTree.dxy->at(iLep)
	     << " dz="   << theBigTree.dz->at(iLep)
	     << endl;
      }
    } // loop over leptons

    sort (thirdLeptons.begin(), thirdLeptons.end()) ;
    // reverse loop to start from last one == highest pT
    for (int iLep = thirdLeptons.size() -1; (iLep >=0) && (theSmallTree.m_nleps < 2) ; iLep--)
    {
      TLorentzVector tlv_dummyLepton
        (
          theBigTree.daughters_px->at (iLep),
          theBigTree.daughters_py->at (iLep),
          theBigTree.daughters_pz->at (iLep),
          theBigTree.daughters_e->at (iLep)
	  );

      theSmallTree.m_leps_pt.push_back   (tlv_dummyLepton.Pt ()) ;
      theSmallTree.m_leps_eta.push_back  (tlv_dummyLepton.Eta ()) ;
      theSmallTree.m_leps_phi.push_back  (tlv_dummyLepton.Phi ()) ;
      theSmallTree.m_leps_e.push_back    (tlv_dummyLepton.E ()) ;
      theSmallTree.m_leps_flav.push_back (theBigTree.particleType->at (iLep)) ;
      ++theSmallTree.m_nleps ;
    }

    if(DEBUG)
    {
      cout << "***** DEBUG: nleps="<< theSmallTree.m_nleps<< endl;
    }

    // ----------------------------------------------------------
    // select jets
    // ----------------------------------------------------------

    vector <pair <float, int> > jets_and_sortPar ;
    // loop over jets
    TLorentzVector jetVecSum (0,0,0,0);
    for (unsigned int iJet = 0 ; iJet < theBigTree.jets_px->size () ; ++iJet)
    {
      // JET PF ID cut
      if (theBigTree.PFjetID->at (iJet) < PFjetID_WP) continue; // 0 ; don't pass PF Jet ID; 1: tight, 2: tightLepVeto

      TLorentzVector tlv_jet(theBigTree.jets_px->at(iJet), theBigTree.jets_py->at(iJet), theBigTree.jets_pz->at(iJet), theBigTree.jets_e->at(iJet)) ;
      if (doSmearing) tlv_jet = tlv_jet * jets_and_smearFactor[iJet];

      if (DEBUG)
      {
	cout << "------- Jets DEBUG---------" << endl;
	cout << "iJet: " << iJet << "  -- pT/Eta/Phi: " << tlv_jet.Pt() << "/" << tlv_jet.Eta() << "/" << tlv_jet.Phi() << endl;
	cout << "discr: " << theBigTree.jets_PUJetIDupdated->at(iJet) << endl;
	cout << "discrWP: " << theBigTree.jets_PUJetIDupdated_WP->at(iJet) << " -- bitwise: " << std::bitset<5>(theBigTree.jets_PUJetIDupdated_WP->at(iJet)) << endl;
	cout << "Pass Loose : " << CheckBit(theBigTree.jets_PUJetIDupdated_WP->at(iJet), 2) << endl;
	cout << "Pass Medium: " << CheckBit(theBigTree.jets_PUJetIDupdated_WP->at(iJet), 1) << endl;
	cout << "Pass Tight : " << CheckBit(theBigTree.jets_PUJetIDupdated_WP->at(iJet), 0) << endl;
	cout << "dR(tau1)   : " << tlv_jet.DeltaR (tlv_firstLepton) << " - lepCleaningCone: " << lepCleaningCone << endl;
	cout << "dR(tau2)   : " << tlv_jet.DeltaR (tlv_secondLepton) << " - lepCleaningCone: " << lepCleaningCone << endl;
	cout << "pT < 20    : " << (tlv_jet.Pt () < 20.) << endl;
	cout << "eta > 2.4  : " << (TMath::Abs(tlv_jet.Eta()) > 2.4) << endl;
	cout << "deepFlavour: " << theBigTree.bDeepFlavor_probb->at(iJet) + theBigTree.bDeepFlavor_probbb->at(iJet) + theBigTree.bDeepFlavor_problepb->at(iJet) << endl;
	cout << "Only PF Jet ID Cut applied before this printout" << endl;
	cout << "---------------------------------" << endl;
      }

      if (tlv_jet.Pt () < 20.) continue ;
      if (tlv_jet.DeltaR (tlv_firstLepton) < lepCleaningCone) continue ;
      if (tlv_jet.DeltaR (tlv_secondLepton) < lepCleaningCone) continue ;

      // Apply PUjetID only to jets with pt < 50 GeV ( https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorkingLegacyRun2#Jets )
      // PU jet ID WP = 2: loose
      if (PUjetID_WP > -1)
      {
	if ( !(CheckBit(theBigTree.jets_PUJetIDupdated_WP->at(iJet), PUjetID_WP)) && tlv_jet.Pt()<50.) continue;
      }

      // all jets selected as btag cands apart from eta cut
      int ajetHadFlav = abs(theBigTree.jets_HadronFlavour->at(iJet));
      if (ajetHadFlav == 5) ++theSmallTree.m_njetsBHadFlav;
      if (ajetHadFlav == 4) ++theSmallTree.m_njetsCHadFlav;

      // 2.4 for b-tag
      if (TMath::Abs(tlv_jet.Eta()) > 2.4) continue;

      // n bjets candidates
      if (tlv_jet.Pt () > 20)  ++theSmallTree.m_nbjets20 ;
      if (tlv_jet.Pt () > 50)  ++theSmallTree.m_nbjets50 ;

      //SortParameter = (bChoiceFlag == 1 ) ? bTag : Pt ;
      float sortPar;
      if(useDeepFlavor)
	sortPar = (bChoiceFlag == 1 ) ? theBigTree.bDeepFlavor_probb->at(iJet) + theBigTree.bDeepFlavor_probbb->at(iJet) + theBigTree.bDeepFlavor_problepb->at(iJet) : tlv_jet.Pt() ;
      else
	sortPar = (bChoiceFlag == 1 ) ? theBigTree.bDeepCSV_probb->at(iJet) + theBigTree.bDeepCSV_probbb->at(iJet) : tlv_jet.Pt() ;

      if (bChoiceFlag != 1 && bChoiceFlag != 2) cout << "** WARNING : bChoiceFlag not known :" << bChoiceFlag << endl;
      jets_and_sortPar.push_back (make_pair (sortPar, iJet) );

    } // loop over jets

      // Save PUjetID for jets with pT > 15 GeV for sync with Pisa
    for (unsigned int iJet = 0 ; iJet < theBigTree.jets_px->size () ; ++iJet)
    {
      if (theBigTree.PFjetID->at (iJet) < PFjetID_WP) continue; // 0 ; don't pass PF Jet ID; 1: tight, 2: tightLepVeto
      TLorentzVector tlv_jet(theBigTree.jets_px->at (iJet), theBigTree.jets_py->at (iJet), theBigTree.jets_pz->at (iJet), theBigTree.jets_e->at (iJet));
      if (doSmearing) tlv_jet = tlv_jet * jets_and_smearFactor[iJet];
      if (tlv_jet.Pt() > 15.) theSmallTree.m_PUjetID.push_back(theBigTree.jets_PUJetIDupdated->at(iJet));
    }

    theSmallTree.m_nbjetscand = jets_and_sortPar.size();
    theSmallTree.m_nfatjets = theBigTree.ak8jets_px->size();

    if (!beInclusive && jets_and_sortPar.size () < 2) continue ;
    ec.Increment("TwoJets", EvtW);
    if (isHHsignal && pairType == genHHDecMode) ecHHsig[genHHDecMode].Increment ("TwoJets", EvtW);

    // sort jet collection by deepCSV
    sort (jets_and_sortPar.begin(), jets_and_sortPar.end(), bJetSort); //sort by first parameter, then pt (dummy if pt order chosen)
    if (jets_and_sortPar.size () >= 2)
    {
      bool isVBF = false;
      vector<pair <int, float> > jets_and_BTag;
      for (auto pair : jets_and_sortPar) jets_and_BTag.push_back (make_pair(pair.second, pair.first)); // just for compatibility...

      // NB !!! the following function only works if jets_and_sortPar contains <CVSscore, idx>
      vector<float> bTagWeight = bTagSFHelper.getEvtWeight (jets_and_BTag, theBigTree, jets_and_smearFactor, pType, bTagSF::central) ;
      theSmallTree.m_bTagweightL = (isMC ? bTagWeight.at(0) : 1.0) ;
      theSmallTree.m_bTagweightM = (isMC ? bTagWeight.at(1) : 1.0) ;
      theSmallTree.m_bTagweightT = (isMC ? bTagWeight.at(2) : 1.0) ;
      theSmallTree.m_bTagweightReshape = (isMC ? bTagWeight.at(3) : 1.0) ;

      vector<float> bTagWeight_up = bTagSFHelper.getEvtWeight (jets_and_BTag, theBigTree, jets_and_smearFactor, pType, bTagSF::up) ;
      theSmallTree.m_bTagweightL_up = (isMC ? bTagWeight_up.at(0) : 1.0) ;
      theSmallTree.m_bTagweightM_up = (isMC ? bTagWeight_up.at(1) : 1.0) ;
      theSmallTree.m_bTagweightT_up = (isMC ? bTagWeight_up.at(2) : 1.0) ;

      vector<float> bTagWeight_down = bTagSFHelper.getEvtWeight (jets_and_BTag, theBigTree, jets_and_smearFactor, pType, bTagSF::down) ;
      theSmallTree.m_bTagweightL_down = (isMC ? bTagWeight_down.at(0) : 1.0) ;
      theSmallTree.m_bTagweightM_down = (isMC ? bTagWeight_down.at(1) : 1.0) ;
      theSmallTree.m_bTagweightT_down = (isMC ? bTagWeight_down.at(2) : 1.0) ;

      vector<float> bTagWeightReshapeshifts = bTagSFHelper.getEvtWeightShifted (jets_and_BTag, theBigTree, jets_and_smearFactor) ;
      theSmallTree.m_bTagweightReshape_jes_up        = (isMC ? bTagWeightReshapeshifts.at(0) : 1.0) ;
      theSmallTree.m_bTagweightReshape_lf_up         = (isMC ? bTagWeightReshapeshifts.at(1) : 1.0) ;
      theSmallTree.m_bTagweightReshape_hf_up         = (isMC ? bTagWeightReshapeshifts.at(2) : 1.0) ;
      theSmallTree.m_bTagweightReshape_hfstats1_up   = (isMC ? bTagWeightReshapeshifts.at(3) : 1.0) ;
      theSmallTree.m_bTagweightReshape_hfstats2_up   = (isMC ? bTagWeightReshapeshifts.at(4) : 1.0) ;
      theSmallTree.m_bTagweightReshape_lfstats1_up   = (isMC ? bTagWeightReshapeshifts.at(5) : 1.0) ;
      theSmallTree.m_bTagweightReshape_lfstats2_up   = (isMC ? bTagWeightReshapeshifts.at(6) : 1.0) ;
      theSmallTree.m_bTagweightReshape_cferr1_up     = (isMC ? bTagWeightReshapeshifts.at(7) : 1.0) ;
      theSmallTree.m_bTagweightReshape_cferr2_up     = (isMC ? bTagWeightReshapeshifts.at(8) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jes_down      = (isMC ? bTagWeightReshapeshifts.at(9) : 1.0) ;
      theSmallTree.m_bTagweightReshape_lf_down       = (isMC ? bTagWeightReshapeshifts.at(10) : 1.0) ;
      theSmallTree.m_bTagweightReshape_hf_down       = (isMC ? bTagWeightReshapeshifts.at(11) : 1.0) ;
      theSmallTree.m_bTagweightReshape_hfstats1_down = (isMC ? bTagWeightReshapeshifts.at(12) : 1.0) ;
      theSmallTree.m_bTagweightReshape_hfstats2_down = (isMC ? bTagWeightReshapeshifts.at(13) : 1.0) ;
      theSmallTree.m_bTagweightReshape_lfstats1_down = (isMC ? bTagWeightReshapeshifts.at(14) : 1.0) ;
      theSmallTree.m_bTagweightReshape_lfstats2_down = (isMC ? bTagWeightReshapeshifts.at(15) : 1.0) ;
      theSmallTree.m_bTagweightReshape_cferr1_down   = (isMC ? bTagWeightReshapeshifts.at(16) : 1.0) ;
      theSmallTree.m_bTagweightReshape_cferr2_down   = (isMC ? bTagWeightReshapeshifts.at(17) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetup1        = (isMC ? bTagWeightReshapeshifts.at(18) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetup2        = (isMC ? bTagWeightReshapeshifts.at(19) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetup3        = (isMC ? bTagWeightReshapeshifts.at(20) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetup4        = (isMC ? bTagWeightReshapeshifts.at(21) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetup5        = (isMC ? bTagWeightReshapeshifts.at(22) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetup6        = (isMC ? bTagWeightReshapeshifts.at(23) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetup7        = (isMC ? bTagWeightReshapeshifts.at(24) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetup8        = (isMC ? bTagWeightReshapeshifts.at(25) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetup9        = (isMC ? bTagWeightReshapeshifts.at(26) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetup10       = (isMC ? bTagWeightReshapeshifts.at(27) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetup11       = (isMC ? bTagWeightReshapeshifts.at(28) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetdown1      = (isMC ? bTagWeightReshapeshifts.at(29) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetdown2      = (isMC ? bTagWeightReshapeshifts.at(30) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetdown3      = (isMC ? bTagWeightReshapeshifts.at(31) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetdown4      = (isMC ? bTagWeightReshapeshifts.at(32) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetdown5      = (isMC ? bTagWeightReshapeshifts.at(33) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetdown6      = (isMC ? bTagWeightReshapeshifts.at(34) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetdown7      = (isMC ? bTagWeightReshapeshifts.at(35) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetdown8      = (isMC ? bTagWeightReshapeshifts.at(36) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetdown9      = (isMC ? bTagWeightReshapeshifts.at(37) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetdown10     = (isMC ? bTagWeightReshapeshifts.at(38) : 1.0) ;
      theSmallTree.m_bTagweightReshape_jetdown11     = (isMC ? bTagWeightReshapeshifts.at(39) : 1.0) ;

      // Set HHbtaginterface for ordering jets
      HHbtagTagger.SetInputValues(theBigTree, jets_and_sortPar, theSmallTree.m_BDT_channel,
				  tlv_firstLepton, tlv_secondLepton, tlv_tauH, tlv_MET, theSmallTree.m_EventNumber, jets_and_smearFactor);

      // Get HHbtag scores in a map <jet_idx, HHbtag_score>
      std::map<int,float> jets_and_HHbtag = HHbtagTagger.GetScore();

      // Use a duplicated map to get the two highest values
      // through get_max_map() from skimUtils.h
      std::map<int,float> HHbtag_idx_map = jets_and_HHbtag;
      auto HHbtagmax  = get_max_map(HHbtag_idx_map);
      HHbtag_idx_map.erase(HHbtagmax.first);
      auto HHbtagmax2 = get_max_map(HHbtag_idx_map);

      if (DEBUG)
      {
	std::cout << "-------------- jets_and_HHbtag --------------" << std::endl;
	for (auto elem : jets_and_HHbtag)
	{
	  std::cout << "  - idx: " << elem.first << " / value: " << elem.second << std::endl;
	}
	std::cout << "  B1 idx: " << HHbtagmax.first  << " value: " << HHbtagmax.second  << std::endl;
	std::cout << "  B2 idx: " << HHbtagmax2.first << " value: " << HHbtagmax2.second << std::endl;
	std::cout << "---------------------------------------------" << std::endl;
      }

      // bjet1 and bjet2 indexes
      const int bjet1idx = HHbtagmax .first ;
      const int bjet2idx = HHbtagmax2.first ;

      // Check that both bjets are assigned
      if (bjet1idx < 0)
      {
	std::cout << "**ERROR: HHbtag score not found for bjet1 --> skipping event !!" << std::endl;
	continue;
      }
      if (bjet2idx < 0)
      {
	std::cout << "**ERROR: HHbtag score not found for bjet2 --> skipping event !!" << std::endl;
	continue;
      }

      //VBF tag
      int VBFidx1 = -1;
      int VBFidx2 = -1;
      theSmallTree.m_isVBF = 0;

      // build all the possible VBF-jet pairs
      std::vector< tuple<float, int, int> > VBFcand_Mjj;
      if (theBigTree.jets_px->size ()>1)
      {
	for (unsigned int iJet = 0; (iJet < theBigTree.jets_px->size ()) && (theSmallTree.m_njets < maxNjetsSaved); ++iJet)
	{
	  // JET PF ID cut
	  if (theBigTree.PFjetID->at (iJet) < PFjetID_WP) continue; // 0 ; don't pass PF Jet ID; 1: tight, 2: tightLepVeto

	  // Skip the already selected b-jets
	  if (int (iJet) == bjet1idx) continue;
	  if (int (iJet) == bjet2idx) continue;

	  TLorentzVector ijet;
	  ijet.SetPxPyPzE(theBigTree.jets_px->at(iJet), theBigTree.jets_py->at(iJet), theBigTree.jets_pz->at(iJet), theBigTree.jets_e ->at(iJet));
	  if (doSmearing) ijet = ijet * jets_and_smearFactor[iJet];

	  if (ijet.DeltaR (tlv_firstLepton) < lepCleaningCone) continue ;
	  if (ijet.DeltaR (tlv_secondLepton) < lepCleaningCone) continue ;
	  if(ijet.Pt() < 30.) continue;
	  if(fabs(ijet.Eta()) > 4.7) continue; // max eta allowed: https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorkingLegacyRun2#Jets

	  // Apply PUjetID only to jets with pt < 50 GeV ( https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorkingLegacyRun2#Jets )
	  // PU jet ID WP = 2: loose
	  if (PUjetID_WP > -1)
	  {
	    if ( !(CheckBit(theBigTree.jets_PUJetIDupdated_WP->at(iJet), PUjetID_WP)) && ijet.Pt()<50.) continue;
	  }

	  for (unsigned int kJet = iJet+1 ;   (kJet < theBigTree.jets_px->size ()) && (theSmallTree.m_njets < maxNjetsSaved) ;  ++kJet)
	  {
	    // JET PF ID cut
	    if (theBigTree.PFjetID->at (kJet) < PFjetID_WP) continue; // 0 ; don't pass PF Jet ID; 1: tight, 2: tightLepVeto

	    // Skip the already selected b-jets
	    if (int (kJet) == bjet1idx) continue;
	    if (int (kJet) == bjet2idx) continue;

	    TLorentzVector kjet;
	    kjet.SetPxPyPzE(theBigTree.jets_px->at(kJet), theBigTree.jets_py->at(kJet), theBigTree.jets_pz->at(kJet), theBigTree.jets_e ->at(kJet));
	    if (doSmearing) kjet = kjet * jets_and_smearFactor[kJet];

	    if (kjet.DeltaR (tlv_firstLepton) < lepCleaningCone) continue ;
	    if (kjet.DeltaR (tlv_secondLepton) < lepCleaningCone) continue ;
	    if(kjet.Pt() < 30.) continue;
	    if(fabs(kjet.Eta()) > 4.7) continue; // max eta allowed: https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorkingLegacyRun2#Jets

	    // Apply PUjetID only to jets with pt < 50 GeV ( https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorkingLegacyRun2#Jets )
	    // PU jet ID WP = 2: loose
	    if (PUjetID_WP > -1)
	    {
	      if ( !(CheckBit(theBigTree.jets_PUJetIDupdated_WP->at(kJet), PUjetID_WP)) && kjet.Pt()<50.) continue;
	    }

	    TLorentzVector jetPair = ijet+kjet;
	    VBFcand_Mjj.push_back(make_tuple(jetPair.M(),iJet,kJet));
	  }
	}

	if (VBFcand_Mjj.size()>0)
	{
	  std::sort(VBFcand_Mjj.begin(),VBFcand_Mjj.end());
	  isVBF = true;
	  VBFidx1 = std::get<1>(*(VBFcand_Mjj.rbegin()));
	  VBFidx2 = std::get<2>(*(VBFcand_Mjj.rbegin()));
	}

      } // all possible VBF jets pairs built

      if (isVBF)
	theSmallTree.m_isVBF = 1;
      else
	theSmallTree.m_isVBF = 0;

      // Now that I've selected the bjets build the TLorentzVectors
      TLorentzVector tlv_firstBjet (theBigTree.jets_px->at(bjet1idx), theBigTree.jets_py->at(bjet1idx), theBigTree.jets_pz->at(bjet1idx), theBigTree.jets_e->at(bjet1idx));
      if (doSmearing) tlv_firstBjet = tlv_firstBjet * jets_and_smearFactor[bjet1idx];
      TLorentzVector tlv_secondBjet(theBigTree.jets_px->at(bjet2idx), theBigTree.jets_py->at(bjet2idx), theBigTree.jets_pz->at(bjet2idx), theBigTree.jets_e->at(bjet2idx));
      if (doSmearing) tlv_secondBjet = tlv_secondBjet * jets_and_smearFactor[bjet2idx];

      if (DEBUG)
      {
	std::cout << "------BJETS----" << std::endl;
	std::cout << "B1(idx,pt,eta,phi,HHbtag): "<<bjet1idx<<" "<<tlv_firstBjet.Pt()<<" "<<tlv_firstBjet.Eta()<<" "<<tlv_firstBjet.Phi()<<" "<<HHbtagmax.second<<std::endl;
	std::cout << "B2(idx,pt,eta,phi,HHbtag): "<<bjet2idx<<" "<<tlv_secondBjet.Pt()<<" "<<tlv_secondBjet.Eta()<<" "<<tlv_secondBjet.Phi()<<" "<<HHbtagmax2.second<<std::endl;
	std::cout << "---------------" << std::endl;
      }

      bool bjets_gen_matched = ((tlv_firstBjet .DeltaR(vGenB1)<0.5 && tlv_secondBjet.DeltaR(vGenB2)<0.5) || (tlv_firstBjet.DeltaR(vGenB2)<0.5 && tlv_secondBjet.DeltaR(vGenB1)<0.5) );
      bool bjet1_gen_matched = ( tlv_firstBjet .DeltaR(vGenB1)<0.5 || tlv_firstBjet .DeltaR(vGenB2)<0.5 );
      bool bjet2_gen_matched = ( tlv_secondBjet.DeltaR(vGenB1)<0.5 || tlv_secondBjet.DeltaR(vGenB2)<0.5 );

      theSmallTree.m_bjets_gen_matched = bjets_gen_matched ? 1 : 0;
      theSmallTree.m_bjet1_gen_matched = bjet1_gen_matched ? 1 : 0;
      theSmallTree.m_bjet2_gen_matched = bjet2_gen_matched ? 1 : 0;

      double ptRegr[2] = {tlv_firstBjet.Pt(), tlv_secondBjet.Pt()};
      //if (computeBregr)
      //{
      //  for (int iBJet = 0; iBJet <=1; iBJet++)
      //  {
      //    int bidx = (iBJet == 0 ? bjet1idx : bjet2idx);
      //    bjrv.Jet_pt     = (iBJet == 0 ? tlv_firstBjet.Pt()  : tlv_secondBjet.Pt());
      //    bjrv.Jet_eta    = (iBJet == 0 ? tlv_firstBjet.Eta() : tlv_secondBjet.Eta());
      //    //bjrv.Jet_corr         = theBigTree.jets_rawPt->at(bidx);
      //    bjrv.Jet_corr         = theBigTree.jetRawf->at(bidx); // should be 1./jetrawf ??
      //    bjrv.rho              = theBigTree.rho;
      //    bjrv.Jet_mt           = theBigTree.jets_mT->at(bidx);
      //    bjrv.Jet_leadTrackPt  = theBigTree.jets_leadTrackPt->at(bidx);
      //    bjrv.Jet_leptonPtRel  = theBigTree.jets_leptonPtRel->at(bidx);
      //    bjrv.Jet_leptonPt     = theBigTree.jets_leptonPt->at(bidx);
      //    bjrv.Jet_leptonDeltaR = theBigTree.jets_leptonDeltaR->at(bidx);
      //    bjrv.Jet_neHEF   = theBigTree.jets_nHEF->at(bidx);
      //    bjrv.Jet_neEmEF  = theBigTree.jets_nEmEF->at(bidx);
      //    bjrv.Jet_chMult  = theBigTree.jets_chMult->at(bidx);
      //    bjrv.Jet_vtxPt   = theBigTree.jets_vtxPt->at(bidx);
      //    bjrv.Jet_vtxMass = theBigTree.jets_vtxMass->at(bidx);
      //    bjrv.Jet_vtx3dL  = theBigTree.jets_vtx3dL->at(bidx);
      //    bjrv.Jet_vtxNtrk = theBigTree.jets_vtxNtrk->at(bidx);
      //    bjrv.Jet_vtx3deL = theBigTree.jets_vtx3deL->at(bidx);

      //    ptRegr[iBJet] = (bRreader->EvaluateRegression (bRegrMethodName.c_str()))[0];
      //  }
      //}

      // save the b-jets
      TLorentzVector tlv_firstBjet_raw = tlv_firstBjet;
      TLorentzVector tlv_secondBjet_raw = tlv_secondBjet;

      // ----- up/down variation using 11 JEC sources + total uncertainty
      // store the up down variations in vectors
      // last position is total uncertainty

      pair <vector <double>, vector<double>> unc_first_updown  = JECprovider.getJECUncVectors(bjet1idx, theBigTree);
      pair <vector <double>, vector<double>> unc_second_updown = JECprovider.getJECUncVectors(bjet2idx, theBigTree);

      // Total JES up/down variation
      TLorentzVector tlv_firstBjet_raw_jetupTot    = tlv_firstBjet_raw ;
      TLorentzVector tlv_firstBjet_raw_jetdownTot  = tlv_firstBjet_raw ;
      TLorentzVector tlv_secondBjet_raw_jetupTot   = tlv_secondBjet_raw;
      TLorentzVector tlv_secondBjet_raw_jetdownTot = tlv_secondBjet_raw;
      TLorentzVector tlv_bH_raw_jetupTot           = tlv_firstBjet_raw + tlv_secondBjet;
      TLorentzVector tlv_bH_raw_jetdownTot         = tlv_firstBjet_raw + tlv_secondBjet;

      if (isMC)
      {
	tlv_firstBjet_raw_jetupTot    = getShiftedJet(tlv_firstBjet_raw , +1., unc_first_updown.first[N_jecSources]);
	tlv_firstBjet_raw_jetdownTot  = getShiftedJet(tlv_firstBjet_raw , -1., unc_first_updown.second[N_jecSources]);
	tlv_secondBjet_raw_jetupTot   = getShiftedJet(tlv_secondBjet_raw, +1., unc_second_updown.first[N_jecSources]);
	tlv_secondBjet_raw_jetdownTot = getShiftedJet(tlv_secondBjet_raw, -1., unc_second_updown.second[N_jecSources]);
	tlv_bH_raw_jetupTot           = tlv_firstBjet_raw_jetupTot + tlv_secondBjet_raw_jetupTot;
	tlv_bH_raw_jetdownTot         = tlv_firstBjet_raw_jetdownTot + tlv_secondBjet_raw_jetdownTot;
      }

      theSmallTree.m_bjet1_pt_raw_jetupTot     = tlv_firstBjet_raw_jetupTot.Pt();
      theSmallTree.m_bjet1_pt_raw_jetdownTot   = tlv_firstBjet_raw_jetdownTot.Pt();
      theSmallTree.m_bjet2_pt_raw_jetupTot     = tlv_secondBjet_raw_jetupTot.Pt();
      theSmallTree.m_bjet2_pt_raw_jetdownTot   = tlv_secondBjet_raw_jetdownTot.Pt();
      theSmallTree.m_bjet1_mass_raw_jetupTot   = tlv_firstBjet_raw_jetupTot.M();
      theSmallTree.m_bjet1_mass_raw_jetdownTot = tlv_firstBjet_raw_jetdownTot.M();
      theSmallTree.m_bjet2_mass_raw_jetupTot   = tlv_secondBjet_raw_jetupTot.M();
      theSmallTree.m_bjet2_mass_raw_jetdownTot = tlv_secondBjet_raw_jetdownTot.M();
      theSmallTree.m_bH_mass_raw_jetupTot      = tlv_bH_raw_jetupTot.M();
      theSmallTree.m_bH_mass_raw_jetdownTot    = tlv_bH_raw_jetdownTot.M();

      vector <TLorentzVector> tlv_firstBjet_raw_jetup(N_jecSources, tlv_firstBjet_raw);
      vector <TLorentzVector> tlv_firstBjet_raw_jetdown(N_jecSources,tlv_firstBjet_raw);
      vector <TLorentzVector> tlv_secondBjet_raw_jetup(N_jecSources, tlv_secondBjet_raw);
      vector <TLorentzVector> tlv_secondBjet_raw_jetdown(N_jecSources,tlv_secondBjet_raw);

      theSmallTree.m_bjet1_pt_raw = tlv_firstBjet_raw.Pt();
      theSmallTree.m_bjet2_pt_raw = tlv_secondBjet_raw.Pt();

      TLorentzVector tlv_bH_raw = tlv_firstBjet + tlv_secondBjet ;
      vector <TLorentzVector> tlv_bH_raw_jetup(N_jecSources, tlv_bH_raw);
      vector <TLorentzVector> tlv_bH_raw_jetdown(N_jecSources, tlv_bH_raw);

      //JER: in LLR it is saved as jets_JER = JER (in %) * jet_energy:
      //https://github.com/LLRCMS/LLRHiggsTauTau/blob/102X_HH/NtupleProducer/plugins/HTauTauNtuplizer.cc#L2316

      double bjet1_JER = theBigTree.jets_JER->at(bjet1idx);
      double bjet2_JER = theBigTree.jets_JER->at(bjet2idx);
      if (doSmearing)
      {
	bjet1_JER = Smearer.getResolution(bjet1idx, theBigTree) * tlv_firstBjet_raw.E();
	bjet2_JER = Smearer.getResolution(bjet2idx, theBigTree) * tlv_secondBjet_raw.E();
      }
      theSmallTree.m_bjet1_JER = bjet1_JER;
      theSmallTree.m_bjet2_JER = bjet2_JER;

      // Total JES up/down variation of JER
      double bjet1_JER_jetupTot   = bjet1_JER ;
      double bjet2_JER_jetupTot   = bjet2_JER ;
      double bjet1_JER_jetdownTot = bjet1_JER ;
      double bjet2_JER_jetdownTot = bjet2_JER ;

      if (isMC)
      {
	bjet1_JER_jetupTot   = bjet1_JER * tlv_firstBjet_raw_jetupTot.E() / tlv_firstBjet_raw.E();
	bjet2_JER_jetupTot   = bjet2_JER * tlv_secondBjet_raw_jetupTot.E() / tlv_secondBjet_raw.E();
	bjet1_JER_jetdownTot = bjet1_JER * tlv_firstBjet_raw_jetdownTot.E() / tlv_firstBjet_raw.E();
	bjet2_JER_jetdownTot = bjet2_JER * tlv_secondBjet_raw_jetdownTot.E() / tlv_secondBjet_raw.E();
      }

      theSmallTree.m_bjet1_JER_jetupTot   = bjet1_JER_jetupTot;
      theSmallTree.m_bjet2_JER_jetupTot   = bjet2_JER_jetupTot;
      theSmallTree.m_bjet1_JER_jetdownTot = bjet1_JER_jetdownTot;
      theSmallTree.m_bjet2_JER_jetdownTot = bjet2_JER_jetdownTot;

      //compute and store variations
      for (int isource = 0; isource < N_jecSources; isource++)
      {
	// compute all shifted bjets
	tlv_firstBjet_raw_jetup[isource]   = tlv_firstBjet_raw ;
	tlv_firstBjet_raw_jetdown[isource] = tlv_firstBjet_raw ;
	tlv_secondBjet_raw_jetup[isource]  = tlv_secondBjet_raw;
	tlv_secondBjet_raw_jetdown[isource]= tlv_secondBjet_raw;

	// variations propagated to bH
	tlv_bH_raw_jetup[isource]   = (tlv_firstBjet_raw_jetup.at(isource) + tlv_secondBjet_raw_jetup.at(isource)) ;
	tlv_bH_raw_jetdown[isource] = (tlv_firstBjet_raw_jetdown.at(isource) + tlv_secondBjet_raw_jetdown.at(isource));
      }

      if (isMC)
      {
	for (int isource = 0; isource < N_jecSources; isource++)
	{
	  // compute all shifted bjets
	  tlv_firstBjet_raw_jetup[isource]   = getShiftedJet(tlv_firstBjet_raw, +1., unc_first_updown.first[isource]);
	  tlv_firstBjet_raw_jetdown[isource] = getShiftedJet(tlv_firstBjet_raw, -1., unc_first_updown.second[isource]);
	  tlv_secondBjet_raw_jetup[isource]  = getShiftedJet(tlv_secondBjet_raw, +1., unc_second_updown.first[isource]);
	  tlv_secondBjet_raw_jetdown[isource]= getShiftedJet(tlv_secondBjet_raw, -1., unc_second_updown.second[isource]);

	  // variations propagated to bH
	  tlv_bH_raw_jetup[isource]   = (tlv_firstBjet_raw_jetup.at(isource) + tlv_secondBjet_raw_jetup.at(isource)) ;
	  tlv_bH_raw_jetdown[isource] = (tlv_firstBjet_raw_jetdown.at(isource) + tlv_secondBjet_raw_jetdown.at(isource));
	}
      }

      for (int isource = 0; isource < N_jecSources; isource++)
      {
	theSmallTree.m_bjet1_mass_raw_jetup.push_back(tlv_firstBjet_raw_jetup[isource].M());
	theSmallTree.m_bjet1_mass_raw_jetdown.push_back(tlv_firstBjet_raw_jetdown[isource].M());
	theSmallTree.m_bjet1_pt_raw_jetup.push_back(tlv_firstBjet_raw_jetup[isource].Pt());
	theSmallTree.m_bjet1_pt_raw_jetdown.push_back(tlv_firstBjet_raw_jetdown[isource].Pt());
	theSmallTree.m_bjet2_mass_raw_jetup.push_back(tlv_secondBjet_raw_jetup[isource].M());
	theSmallTree.m_bjet2_mass_raw_jetdown.push_back(tlv_secondBjet_raw_jetdown[isource].M());
	theSmallTree.m_bjet2_pt_raw_jetup.push_back(tlv_secondBjet_raw_jetup[isource].Pt());
	theSmallTree.m_bjet2_pt_raw_jetdown.push_back(tlv_secondBjet_raw_jetdown[isource].Pt());

	theSmallTree.m_bH_mass_raw_jetup.push_back((tlv_bH_raw_jetup[isource]).M());
	theSmallTree.m_bH_mass_raw_jetdown.push_back((tlv_bH_raw_jetdown[isource]).M());
	theSmallTree.m_bH_pt_raw_jetup.push_back((tlv_bH_raw_jetup[isource]).Pt());
	theSmallTree.m_bH_pt_raw_jetdown.push_back((tlv_bH_raw_jetdown[isource]).Pt());

	// JER variations
	theSmallTree.m_bjet1_JER_jetup.push_back(bjet1_JER * tlv_firstBjet_raw_jetup[isource].E() /  tlv_firstBjet_raw.E() );
	theSmallTree.m_bjet1_JER_jetdown.push_back(bjet1_JER * tlv_firstBjet_raw_jetdown[isource].E() /  tlv_firstBjet_raw.E()) ;
	theSmallTree.m_bjet2_JER_jetup.push_back(bjet2_JER * tlv_secondBjet_raw_jetup[isource].E() /  tlv_secondBjet_raw.E()) ;
	theSmallTree.m_bjet2_JER_jetdown.push_back(bjet2_JER * tlv_secondBjet_raw_jetdown[isource].E() /  tlv_secondBjet_raw.E()) ;
      }


      theSmallTree.m_bjet1_pt_raw_jetup1    = (tlv_firstBjet_raw_jetup[0]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetup2    = (tlv_firstBjet_raw_jetup[1]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetup3    = (tlv_firstBjet_raw_jetup[2]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetup4    = (tlv_firstBjet_raw_jetup[3]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetup5    = (tlv_firstBjet_raw_jetup[4]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetup6    = (tlv_firstBjet_raw_jetup[5]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetup7    = (tlv_firstBjet_raw_jetup[6]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetup8    = (tlv_firstBjet_raw_jetup[7]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetup9    = (tlv_firstBjet_raw_jetup[8]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetup10   = (tlv_firstBjet_raw_jetup[9]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetup11   = (tlv_firstBjet_raw_jetup[10]).Pt();

      theSmallTree.m_bjet2_pt_raw_jetup1    = (tlv_secondBjet_raw_jetup[0]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetup2    = (tlv_secondBjet_raw_jetup[1]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetup3    = (tlv_secondBjet_raw_jetup[2]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetup4    = (tlv_secondBjet_raw_jetup[3]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetup5    = (tlv_secondBjet_raw_jetup[4]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetup6    = (tlv_secondBjet_raw_jetup[5]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetup7    = (tlv_secondBjet_raw_jetup[6]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetup8    = (tlv_secondBjet_raw_jetup[7]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetup9    = (tlv_secondBjet_raw_jetup[8]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetup10   = (tlv_secondBjet_raw_jetup[9]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetup11   = (tlv_secondBjet_raw_jetup[10]).Pt();

      theSmallTree.m_bjet1_pt_raw_jetdown1  = (tlv_firstBjet_raw_jetdown[0]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetdown2  = (tlv_firstBjet_raw_jetdown[1]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetdown3  = (tlv_firstBjet_raw_jetdown[2]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetdown4  = (tlv_firstBjet_raw_jetdown[3]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetdown5  = (tlv_firstBjet_raw_jetdown[4]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetdown6  = (tlv_firstBjet_raw_jetdown[5]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetdown7  = (tlv_firstBjet_raw_jetdown[6]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetdown8  = (tlv_firstBjet_raw_jetdown[7]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetdown9  = (tlv_firstBjet_raw_jetdown[8]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetdown10 = (tlv_firstBjet_raw_jetdown[9]).Pt();
      theSmallTree.m_bjet1_pt_raw_jetdown11 = (tlv_firstBjet_raw_jetdown[10]).Pt();

      theSmallTree.m_bjet2_pt_raw_jetdown1  = (tlv_secondBjet_raw_jetdown[0]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetdown2  = (tlv_secondBjet_raw_jetdown[1]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetdown3  = (tlv_secondBjet_raw_jetdown[2]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetdown4  = (tlv_secondBjet_raw_jetdown[3]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetdown5  = (tlv_secondBjet_raw_jetdown[4]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetdown6  = (tlv_secondBjet_raw_jetdown[5]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetdown7  = (tlv_secondBjet_raw_jetdown[6]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetdown8  = (tlv_secondBjet_raw_jetdown[7]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetdown9  = (tlv_secondBjet_raw_jetdown[8]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetdown10 = (tlv_secondBjet_raw_jetdown[9]).Pt();
      theSmallTree.m_bjet2_pt_raw_jetdown11 = (tlv_secondBjet_raw_jetdown[10]).Pt();

      theSmallTree.m_bH_mass_raw = tlv_bH_raw.M();
      theSmallTree.m_bH_pt_raw = tlv_bH_raw.Pt();

      // also store flat bH_mass variations, because it's needed for ellyptic mass cut
      theSmallTree.m_bH_mass_raw_jetup1    = (tlv_bH_raw_jetup[0]).M();
      theSmallTree.m_bH_mass_raw_jetup2    = (tlv_bH_raw_jetup[1]).M();
      theSmallTree.m_bH_mass_raw_jetup3    = (tlv_bH_raw_jetup[2]).M();
      theSmallTree.m_bH_mass_raw_jetup4    = (tlv_bH_raw_jetup[3]).M();
      theSmallTree.m_bH_mass_raw_jetup5    = (tlv_bH_raw_jetup[4]).M();
      theSmallTree.m_bH_mass_raw_jetup6    = (tlv_bH_raw_jetup[5]).M();
      theSmallTree.m_bH_mass_raw_jetup7    = (tlv_bH_raw_jetup[6]).M();
      theSmallTree.m_bH_mass_raw_jetup8    = (tlv_bH_raw_jetup[7]).M();
      theSmallTree.m_bH_mass_raw_jetup9    = (tlv_bH_raw_jetup[8]).M();
      theSmallTree.m_bH_mass_raw_jetup10   = (tlv_bH_raw_jetup[9]).M();
      theSmallTree.m_bH_mass_raw_jetup11   = (tlv_bH_raw_jetup[10]).M();

      theSmallTree.m_bH_mass_raw_jetdown1  = (tlv_bH_raw_jetdown[0]).M();
      theSmallTree.m_bH_mass_raw_jetdown2  = (tlv_bH_raw_jetdown[1]).M();
      theSmallTree.m_bH_mass_raw_jetdown3  = (tlv_bH_raw_jetdown[2]).M();
      theSmallTree.m_bH_mass_raw_jetdown4  = (tlv_bH_raw_jetdown[3]).M();
      theSmallTree.m_bH_mass_raw_jetdown5  = (tlv_bH_raw_jetdown[4]).M();
      theSmallTree.m_bH_mass_raw_jetdown6  = (tlv_bH_raw_jetdown[5]).M();
      theSmallTree.m_bH_mass_raw_jetdown7  = (tlv_bH_raw_jetdown[6]).M();
      theSmallTree.m_bH_mass_raw_jetdown8  = (tlv_bH_raw_jetdown[7]).M();
      theSmallTree.m_bH_mass_raw_jetdown9  = (tlv_bH_raw_jetdown[8]).M();
      theSmallTree.m_bH_mass_raw_jetdown10 = (tlv_bH_raw_jetdown[9]).M();
      theSmallTree.m_bH_mass_raw_jetdown11 = (tlv_bH_raw_jetdown[10]).M();

      // FIXME : here mass is manually set to 0, should we change it?
      float ptScale1 = ptRegr[0] / tlv_firstBjet.Pt() ;
      float ptScale2 = ptRegr[1] / tlv_secondBjet.Pt() ;
      tlv_firstBjet.SetPtEtaPhiE (ptRegr[0], tlv_firstBjet.Eta(), tlv_firstBjet.Phi(), ptScale1*tlv_firstBjet.Energy());
      tlv_secondBjet.SetPtEtaPhiE (ptRegr[1], tlv_secondBjet.Eta(), tlv_secondBjet.Phi(), ptScale2*tlv_secondBjet.Energy());

      theSmallTree.m_bjet1_pt   = tlv_firstBjet.Pt () ;
      theSmallTree.m_bjet1_eta  = tlv_firstBjet.Eta () ;
      theSmallTree.m_bjet1_phi  = tlv_firstBjet.Phi () ;
      theSmallTree.m_bjet1_e    = tlv_firstBjet.E () ;
      theSmallTree.m_bjet1_bID  = theBigTree.bCSVscore->at (bjet1idx) ;
      theSmallTree.m_bjet1_bID_deepCSV  = theBigTree.bDeepCSV_probb->at(bjet1idx) + theBigTree.bDeepCSV_probbb->at(bjet1idx) ;
      theSmallTree.m_bjet1_bID_deepFlavor  = theBigTree.bDeepFlavor_probb->at(bjet1idx) + theBigTree.bDeepFlavor_probbb->at(bjet1idx) + theBigTree.bDeepFlavor_problepb->at(bjet1idx);
      theSmallTree.m_bjet1_cID_deepFlavor  = theBigTree.bDeepFlavor_probc->at(bjet1idx);
      theSmallTree.m_bjet1_CvsL = getCvsL(theBigTree, bjet1idx);
      theSmallTree.m_bjet1_CvsB = getCvsB(theBigTree, bjet1idx);
      theSmallTree.m_bjet1_bMVAID  = theBigTree.pfCombinedMVAV2BJetTags->at (bjet1idx) ;
      theSmallTree.m_bjet1_PUjetIDupdated = theBigTree.jets_PUJetIDupdated->at(bjet1idx);
      theSmallTree.m_bjet1_flav = theBigTree.jets_HadronFlavour->at (bjet1idx);
      theSmallTree.m_bjet1_pnet_bb = theBigTree.bParticleNetAK4JetTags_probbb->at(bjet1idx);
      theSmallTree.m_bjet1_pnet_cc = theBigTree.bParticleNetAK4JetTags_probcc->at(bjet1idx);
      theSmallTree.m_bjet1_pnet_b = theBigTree.bParticleNetAK4JetTags_probb->at(bjet1idx);
      theSmallTree.m_bjet1_pnet_c = theBigTree.bParticleNetAK4JetTags_probc->at(bjet1idx);
      theSmallTree.m_bjet1_pnet_g = theBigTree.bParticleNetAK4JetTags_probg->at(bjet1idx);
      theSmallTree.m_bjet1_pnet_uds = theBigTree.bParticleNetAK4JetTags_probuds->at(bjet1idx);
      theSmallTree.m_bjet1_pnet_pu = theBigTree.bParticleNetAK4JetTags_probpu->at(bjet1idx);
      theSmallTree.m_bjet1_pnet_undef = theBigTree.bParticleNetAK4JetTags_probundef->at(bjet1idx);

      theSmallTree.m_bjet2_pt   = tlv_secondBjet.Pt () ;
      theSmallTree.m_bjet2_eta  = tlv_secondBjet.Eta () ;
      theSmallTree.m_bjet2_phi  = tlv_secondBjet.Phi () ;
      theSmallTree.m_bjet2_e    = tlv_secondBjet.E () ;
      theSmallTree.m_bjet2_bID  = theBigTree.bCSVscore->at (bjet2idx) ;
      theSmallTree.m_bjet2_bID_deepCSV  = theBigTree.bDeepCSV_probb->at(bjet2idx) + theBigTree.bDeepCSV_probbb->at(bjet2idx) ;
      theSmallTree.m_bjet2_bID_deepFlavor  = theBigTree.bDeepFlavor_probb->at(bjet2idx) + theBigTree.bDeepFlavor_probbb->at(bjet2idx) + theBigTree.bDeepFlavor_problepb->at(bjet2idx);
      theSmallTree.m_bjet2_cID_deepFlavor  = theBigTree.bDeepFlavor_probc->at(bjet2idx);
      theSmallTree.m_bjet2_CvsL = getCvsL(theBigTree, bjet2idx);
      theSmallTree.m_bjet2_CvsB = getCvsB(theBigTree, bjet2idx);
      theSmallTree.m_bjet2_bMVAID  = theBigTree.pfCombinedMVAV2BJetTags->at (bjet2idx) ;
      theSmallTree.m_bjet2_PUjetIDupdated = theBigTree.jets_PUJetIDupdated->at(bjet2idx);
      theSmallTree.m_bjet2_flav = theBigTree.jets_HadronFlavour->at (bjet2idx) ;
      theSmallTree.m_bjet2_pnet_bb = theBigTree.bParticleNetAK4JetTags_probbb->at(bjet2idx);
      theSmallTree.m_bjet2_pnet_cc = theBigTree.bParticleNetAK4JetTags_probcc->at(bjet2idx);
      theSmallTree.m_bjet2_pnet_b = theBigTree.bParticleNetAK4JetTags_probb->at(bjet2idx);
      theSmallTree.m_bjet2_pnet_c = theBigTree.bParticleNetAK4JetTags_probc->at(bjet2idx);
      theSmallTree.m_bjet2_pnet_g = theBigTree.bParticleNetAK4JetTags_probg->at(bjet2idx);
      theSmallTree.m_bjet2_pnet_uds = theBigTree.bParticleNetAK4JetTags_probuds->at(bjet2idx);
      theSmallTree.m_bjet2_pnet_pu = theBigTree.bParticleNetAK4JetTags_probpu->at(bjet2idx);
      theSmallTree.m_bjet2_pnet_undef = theBigTree.bParticleNetAK4JetTags_probundef->at(bjet2idx);

      theSmallTree.m_bjets_bID  = theBigTree.bCSVscore->at (bjet1idx) +theBigTree.bCSVscore->at (bjet2idx) ;
      theSmallTree.m_bjets_bID_deepCSV  = theBigTree.bDeepCSV_probb->at(bjet1idx) + theBigTree.bDeepCSV_probbb->at(bjet1idx) + theBigTree.bDeepCSV_probb->at(bjet2idx) + theBigTree.bDeepCSV_probbb->at(bjet2idx);
      theSmallTree.m_bjets_bID_deepFlavor  = theBigTree.bDeepFlavor_probb->at(bjet1idx) + theBigTree.bDeepFlavor_probbb->at(bjet1idx) + theBigTree.bDeepFlavor_problepb->at(bjet1idx) + theBigTree.bDeepFlavor_probb->at(bjet2idx) + theBigTree.bDeepFlavor_probbb->at(bjet2idx) + theBigTree.bDeepFlavor_problepb->at(bjet2idx);
      theSmallTree.m_bjets_cID_deepFlavor  = theBigTree.bDeepFlavor_probc->at(bjet1idx) + theBigTree.bDeepFlavor_probc->at(bjet2idx) ;

      // Save gen info for b-jets
      bool hasgj1 = false;
      bool hasgj2 = false;

      if (isMC)
      {
	int mcind = theBigTree.jets_genjetIndex->at(bjet1idx);
	if (mcind>=0)
	{
	  TLorentzVector gen(theBigTree.genjet_px->at(mcind),theBigTree.genjet_py->at(mcind),theBigTree.genjet_pz->at(mcind),theBigTree.genjet_e->at(mcind));
	  theSmallTree.m_genjet1_pt = gen.Pt();
	  theSmallTree.m_genjet1_eta = gen.Eta();
	  theSmallTree.m_genjet1_phi = gen.Phi();
	  theSmallTree.m_genjet1_e = gen.E();
	  if (gen.Pt() > 8) hasgj1 = true;
	}

	mcind = theBigTree.jets_genjetIndex->at(bjet2idx);
	if (mcind>=0)
	{
	  TLorentzVector gen(theBigTree.genjet_px->at(mcind),theBigTree.genjet_py->at(mcind),theBigTree.genjet_pz->at(mcind),theBigTree.genjet_e->at(mcind));
	  theSmallTree.m_genjet2_pt = gen.Pt();
	  theSmallTree.m_genjet2_eta = gen.Eta();
	  theSmallTree.m_genjet2_phi = gen.Phi();
	  theSmallTree.m_genjet2_e = gen.E();
	  if (gen.Pt() > 8) hasgj2 = true;
	}
      }
      theSmallTree.m_bjet1_hasgenjet = hasgj1 ;
      theSmallTree.m_bjet2_hasgenjet = hasgj2 ;

      // Save HT_20, HT_50 and HT_20_BDT(with cut on |eta|<4.7)
      TLorentzVector jetVecSum (0,0,0,0);
      vector <Float_t> BDT_HT20_jetup(N_jecSources);
      vector <Float_t> BDT_HT20_jetdown(N_jecSources);
      if (DEBUG) cout << "----- BDT HT debug ------" << endl;

      for (unsigned int iJet = 0 ; iJet < theBigTree.jets_px->size () ; ++iJet)
      {
	// JET PF ID cut
	if (theBigTree.PFjetID->at (iJet) < PFjetID_WP) continue; // 0 ; don't pass PF Jet ID; 1: tight, 2: tightLepVeto

	// Build the jet TLorentzVector
	TLorentzVector tlv_jet(theBigTree.jets_px->at(iJet), theBigTree.jets_py->at(iJet), theBigTree.jets_pz->at(iJet), theBigTree.jets_e->at(iJet)) ;
	if (doSmearing) tlv_jet = tlv_jet * jets_and_smearFactor[iJet];

	// Pt cut for jets
	if (tlv_jet.Pt () < 20.) continue ;

	// Lepton and b-jet cleaning
	if (tlv_jet.DeltaR (tlv_firstLepton) < lepCleaningCone) continue ;
	if (tlv_jet.DeltaR (tlv_secondLepton) < lepCleaningCone) continue ;
	if ( (int) iJet == bjet1idx || (int) iJet == bjet2idx ) continue ;

	// Apply PUjetID only to jets with pt < 50 GeV ( https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorkingLegacyRun2#Jets )
	// PU jet ID WP = 2: loose
	if (PUjetID_WP > -1)
	{
	  if ( !(CheckBit(theBigTree.jets_PUJetIDupdated_WP->at(iJet), PUjetID_WP)) && tlv_jet.Pt()<50.) continue;
	}

	// use these jets for HT
	if (tlv_jet.Pt () > 20)
	{
	  ++theSmallTree.m_njets20 ;
	  theSmallTree.m_HT20 += tlv_jet.Pt() ;
	  jetVecSum += tlv_jet ;
	}

	if (TMath::Abs(tlv_jet.Eta()) < 4.7)
	{
	  if (tlv_jet.Pt () > 20) theSmallTree.m_BDT_HT20 += tlv_jet.Pt() ;
	  if (DEBUG) cout << " ---> Jet " << iJet << " - pt: " << tlv_jet.Pt() << " - HT: " << theSmallTree.m_BDT_HT20 << endl;

	  pair <vector <double>, vector<double>> unc_updown = JECprovider.getJECUncVectors(iJet, theBigTree);

	  // JES total shift
	  TLorentzVector tlv_jetupTot   = getShiftedJet(tlv_jet, +1., unc_updown.first[N_jecSources]);
	  TLorentzVector tlv_jetdownTot = getShiftedJet(tlv_jet, -1., unc_updown.first[N_jecSources]);
	  if (tlv_jetupTot.Pt()   > 20) theSmallTree.m_BDT_HT20_jetupTot += tlv_jetupTot.Pt();
	  if (tlv_jetdownTot.Pt() > 20) theSmallTree.m_BDT_HT20_jetdownTot += tlv_jetdownTot.Pt();

	  // build shifted jet
	  for (int isource = 0; isource < N_jecSources; isource++)
	  {
	    TLorentzVector tlv_jetup   = getShiftedJet(tlv_jet, +1., unc_updown.first[isource]);
	    TLorentzVector tlv_jetdown = getShiftedJet(tlv_jet, -1., unc_updown.second[isource]);
	    if (tlv_jetup.Pt () > 20)   BDT_HT20_jetup[isource]   += tlv_jetup.Pt();
	    if (tlv_jetdown.Pt () > 20) BDT_HT20_jetdown[isource] += tlv_jetdown.Pt();
	  }
	}

	if (tlv_jet.Pt () > 50)
	{
	  ++theSmallTree.m_njets50 ;
	  theSmallTree.m_HT50 += tlv_jet.Pt() ;
	}
      }

      theSmallTree.m_HT20Full = theSmallTree.m_HT20 + tlv_firstLepton.Pt() + tlv_secondLepton.Pt() ;
      theSmallTree.m_jet20centrality = jetVecSum.Pt() / theSmallTree.m_HT20Full ;

      theSmallTree.m_BDT_HT20_jetup   = BDT_HT20_jetup;
      theSmallTree.m_BDT_HT20_jetdown = BDT_HT20_jetdown;

      if (DEBUG) cout << "  HT = " << theSmallTree.m_BDT_HT20 << endl;
      if (DEBUG) cout << "---------------------" << endl;

      float METx = theBigTree.METx->at (chosenTauPair) ;
      float METy = theBigTree.METy->at (chosenTauPair) ;
      if (doSmearing)
      {
	TVector2 metSmeared = getShiftedMET_smear(METx, METy, theBigTree, jets_and_smearFactor);
	METx = metSmeared.Px();
	METy = metSmeared.Py();
      }

      TLorentzVector tlv_bH = tlv_firstBjet + tlv_secondBjet ;
      TLorentzVector tlv_neutrinos =  tlv_bH - tlv_bH_raw;
      theSmallTree.m_met_et_corr = theBigTree.met - tlv_neutrinos.Et() ;

      const TVector2 ptmiss = TVector2(METx, METy) ;
      TMatrixD metcov (2, 2) ;
      metcov (0,0) = theBigTree.MET_cov00->at (chosenTauPair) ;
      metcov (1,0) = theBigTree.MET_cov10->at (chosenTauPair) ;
      metcov (0,1) = theBigTree.MET_cov01->at (chosenTauPair) ;
      metcov (1,1) = theBigTree.MET_cov11->at (chosenTauPair) ;
      const TMatrixD stableMetCov = metcov;

      // Shifted MET for JES
      auto vMET_shift_jet = getShiftedMET_jet(N_jecSources, vMET, theBigTree, JECprovider,  DEBUG);
      for (int isource = 0; isource < N_jecSources; isource++)
      {
	theSmallTree.m_METx_jetup.push_back(vMET_shift_jet.first.at(isource).X());
	theSmallTree.m_METy_jetup.push_back(vMET_shift_jet.first.at(isource).Y());
	theSmallTree.m_METx_jetdown.push_back(vMET_shift_jet.second.at(isource).X());
	theSmallTree.m_METy_jetdown.push_back(vMET_shift_jet.second.at(isource).Y());
      }

      if (isMC)
      {
	// Shifted MET for TES/EES
	auto vMET_shifts_tes_ees = getShiftedMET_tes_ees(N_tauhDM, N_tauhDM_EES, vMET, theBigTree, DEBUG);

	// Shifted DeepMET for TES/EES
	auto vDeepMET_ResponseTune_shifts_tes_ees = getShiftedMET_tes_ees(N_tauhDM, N_tauhDM_EES, vDeepMET_ResponseTune, theBigTree, DEBUG);
	auto vDeepMET_ResolutionTune_shifts_tes_ees = getShiftedMET_tes_ees(N_tauhDM, N_tauhDM_EES, vDeepMET_ResolutionTune, theBigTree, DEBUG);

	//unpack: first is tes, second is ees
	auto vMET_shift_tes = vMET_shifts_tes_ees.first;
	auto vMET_shift_ees = vMET_shifts_tes_ees.second;

	auto vDeepMET_ResponseTune_shifts_tes = vDeepMET_ResponseTune_shifts_tes_ees.first;
	auto vDeepMET_ResponseTune_shifts_ees = vDeepMET_ResponseTune_shifts_tes_ees.second;

	auto vDeepMET_ResolutionTune_shifts_tes = vDeepMET_ResolutionTune_shifts_tes_ees.first;
	auto vDeepMET_ResolutionTune_shifts_ees = vDeepMET_ResolutionTune_shifts_tes_ees.second;
	for (int idm = 0; idm < N_tauhDM; idm++)
	{
	  theSmallTree.m_METx_tauup.push_back(vMET_shift_tes.first.at(idm).X());
	  theSmallTree.m_METy_tauup.push_back(vMET_shift_tes.first.at(idm).Y());
	  theSmallTree.m_METx_taudown.push_back(vMET_shift_tes.second.at(idm).X());
	  theSmallTree.m_METy_taudown.push_back(vMET_shift_tes.second.at(idm).Y());

	  theSmallTree.m_DeepMET_ResponseTune_px_tauup.push_back(vDeepMET_ResponseTune_shifts_tes.first.at(idm).X());
	  theSmallTree.m_DeepMET_ResponseTune_py_tauup.push_back(vDeepMET_ResponseTune_shifts_tes.first.at(idm).Y());
	  theSmallTree.m_DeepMET_ResponseTune_px_taudown.push_back(vDeepMET_ResponseTune_shifts_tes.second.at(idm).X());
	  theSmallTree.m_DeepMET_ResponseTune_py_taudown.push_back(vDeepMET_ResponseTune_shifts_tes.second.at(idm).Y());

	  theSmallTree.m_DeepMET_ResolutionTune_px_tauup.push_back(vDeepMET_ResolutionTune_shifts_tes.first.at(idm).X());
	  theSmallTree.m_DeepMET_ResolutionTune_py_tauup.push_back(vDeepMET_ResolutionTune_shifts_tes.first.at(idm).Y());
	  theSmallTree.m_DeepMET_ResolutionTune_px_taudown.push_back(vDeepMET_ResolutionTune_shifts_tes.second.at(idm).X());
	  theSmallTree.m_DeepMET_ResolutionTune_py_taudown.push_back(vDeepMET_ResolutionTune_shifts_tes.second.at(idm).Y());

	  if (idm <N_tauhDM_EES)
	  {
	    theSmallTree.m_METx_eleup.push_back(vMET_shift_ees.first.at(idm).X());
	    theSmallTree.m_METy_eleup.push_back(vMET_shift_ees.first.at(idm).Y());
	    theSmallTree.m_METx_eledown.push_back(vMET_shift_ees.second.at(idm).X());
	    theSmallTree.m_METy_eledown.push_back(vMET_shift_ees.second.at(idm).Y());

	    theSmallTree.m_DeepMET_ResponseTune_px_eleup.push_back(vDeepMET_ResponseTune_shifts_ees.first.at(idm).X());
	    theSmallTree.m_DeepMET_ResponseTune_py_eleup.push_back(vDeepMET_ResponseTune_shifts_ees.first.at(idm).Y());
	    theSmallTree.m_DeepMET_ResponseTune_px_eledown.push_back(vDeepMET_ResponseTune_shifts_ees.second.at(idm).X());
	    theSmallTree.m_DeepMET_ResponseTune_py_eledown.push_back(vDeepMET_ResponseTune_shifts_ees.second.at(idm).Y());

	    theSmallTree.m_DeepMET_ResolutionTune_px_eleup.push_back(vDeepMET_ResolutionTune_shifts_ees.first.at(idm).X());
	    theSmallTree.m_DeepMET_ResolutionTune_py_eleup.push_back(vDeepMET_ResolutionTune_shifts_ees.first.at(idm).Y());
	    theSmallTree.m_DeepMET_ResolutionTune_px_eledown.push_back(vDeepMET_ResolutionTune_shifts_ees.second.at(idm).X());
	    theSmallTree.m_DeepMET_ResolutionTune_py_eledown.push_back(vDeepMET_ResolutionTune_shifts_ees.second.at(idm).Y());
	  }
	}

	// Shifted MET for MES
	auto vMET_shift_mes = getShiftedMET_mes(vMET, theBigTree, DEBUG);
	theSmallTree.m_METx_muup = vMET_shift_mes.first.X();
	theSmallTree.m_METy_muup = vMET_shift_mes.first.Y();
	theSmallTree.m_METx_mudown = vMET_shift_mes.second.X();
	theSmallTree.m_METy_mudown = vMET_shift_mes.second.Y();

	// Shifted DeepMET for MES
	auto vDeepMET_ResponseTune_shift_mes = getShiftedMET_mes(vDeepMET_ResponseTune, theBigTree, DEBUG);
	theSmallTree.m_DeepMET_ResponseTune_px_muup = vDeepMET_ResponseTune_shift_mes.first.X();
	theSmallTree.m_DeepMET_ResponseTune_py_muup = vDeepMET_ResponseTune_shift_mes.first.Y();
	theSmallTree.m_DeepMET_ResponseTune_px_mudown = vDeepMET_ResponseTune_shift_mes.second.X();
	theSmallTree.m_DeepMET_ResponseTune_py_mudown = vDeepMET_ResponseTune_shift_mes.second.Y();

	auto vDeepMET_ResolutionTune_shift_mes = getShiftedMET_mes(vDeepMET_ResolutionTune, theBigTree, DEBUG);
	theSmallTree.m_DeepMET_ResolutionTune_px_muup = vDeepMET_ResolutionTune_shift_mes.first.X();
	theSmallTree.m_DeepMET_ResolutionTune_py_muup = vDeepMET_ResolutionTune_shift_mes.first.Y();
	theSmallTree.m_DeepMET_ResolutionTune_px_mudown = vDeepMET_ResolutionTune_shift_mes.second.X();
	theSmallTree.m_DeepMET_ResolutionTune_py_mudown = vDeepMET_ResolutionTune_shift_mes.second.Y();

	// Shifted MET for JES total
	auto vMET_shift_jetTot = getShiftedMET_jetTot(N_jecSources, vMET, theBigTree, JECprovider, DEBUG);
	theSmallTree.m_METx_jetupTot   = vMET_shift_jetTot.first.X();
	theSmallTree.m_METy_jetupTot   = vMET_shift_jetTot.first.Y();
	theSmallTree.m_METx_jetdownTot = vMET_shift_jetTot.second.X();
	theSmallTree.m_METy_jetdownTot = vMET_shift_jetTot.second.Y();
      }

      theSmallTree.m_bH_pt = tlv_bH.Pt () ;
      theSmallTree.m_bH_eta = tlv_bH.Eta () ;
      theSmallTree.m_bH_phi = tlv_bH.Phi () ;
      theSmallTree.m_bH_e = tlv_bH.E () ;
      theSmallTree.m_bH_mass = tlv_bH.M () ;

      theSmallTree.m_bH_MET_deltaEta    = std::abs(tlv_bH.Eta()); // since MET.Eta()==0 by definition, dEta(bH,MET)=|bH.Eta()|
      theSmallTree.m_bH_MET_deltaR      = tlv_bH.DeltaR(tlv_MET);
      theSmallTree.m_bH_tauH_MET_deltaR = tlv_bH.DeltaR(tlv_tauH + tlv_MET);
      theSmallTree.m_BDT_bHMet_deltaPhi = ROOT::Math::VectorUtil::DeltaPhi(tlv_bH, tlv_MET);
      theSmallTree.m_BDT_topPairMasses   = Calculate_topPairMasses(getLVfromTLV(tlv_firstLepton), getLVfromTLV(tlv_secondLepton), getLVfromTLV(tlv_firstBjet), getLVfromTLV(tlv_secondBjet), getLVfromTLV(tlv_MET)).first;
      theSmallTree.m_BDT_topPairMasses2  = Calculate_topPairMasses(getLVfromTLV(tlv_firstLepton), getLVfromTLV(tlv_secondLepton), getLVfromTLV(tlv_firstBjet), getLVfromTLV(tlv_secondBjet), getLVfromTLV(tlv_MET)).second;
      theSmallTree.m_BDT_MX             = Calculate_MX(tlv_firstLepton, tlv_secondLepton, tlv_firstBjet, tlv_secondBjet, tlv_MET);
      theSmallTree.m_BDT_bH_tauH_MET_InvMass = ROOT::Math::VectorUtil::InvariantMass(tlv_bH, tlv_tauH + tlv_MET);
      theSmallTree.m_BDT_bH_tauH_InvMass     = ROOT::Math::VectorUtil::InvariantMass(tlv_bH, tlv_tauH);
      theSmallTree.m_BDT_MET_bH_cosTheta = Calculate_cosTheta_2bodies(getLVfromTLV(tlv_MET), getLVfromTLV(tlv_bH));
      theSmallTree.m_BDT_b1_bH_cosTheta  = Calculate_cosTheta_2bodies(getLVfromTLV(tlv_firstBjet), getLVfromTLV(tlv_bH));

      TLorentzVector tlv_HH = tlv_bH + tlv_tauH ;
      TLorentzVector tlv_HH_raw = tlv_bH_raw + tlv_tauH ;
      theSmallTree.m_HH_pt = tlv_HH.Pt () ;
      theSmallTree.m_HH_eta = tlv_HH.Eta () ;
      theSmallTree.m_HH_phi = tlv_HH.Phi () ;
      theSmallTree.m_HH_e = tlv_HH.E () ;
      theSmallTree.m_HH_mass = tlv_HH.M () ;
      theSmallTree.m_HH_mass_raw = tlv_HH_raw.M () ;
      theSmallTree.m_HH_deltaR = tlv_bH.DeltaR(tlv_tauH);

      // in case the SVFIT mass is calculated
      if (theBigTree.SVfitMass->at(chosenTauPair) > -900. || (doSmearing && theSmallTree.m_tauH_SVFIT_mass > 0))
      {
	TLorentzVector tlv_HHsvfit  = tlv_bH + tlv_tauH_SVFIT ;
	theSmallTree.m_HHsvfit_pt   = tlv_HHsvfit.Pt () ;
	theSmallTree.m_HHsvfit_eta  = tlv_HHsvfit.Eta () ;
	theSmallTree.m_HHsvfit_phi  = tlv_HHsvfit.Phi () ;
	theSmallTree.m_HHsvfit_e    = tlv_HHsvfit.E () ;
	theSmallTree.m_HHsvfit_mass = tlv_HHsvfit.M () ;

	theSmallTree.m_BDT_HHsvfit_abs_deltaPhi  = fabs(ROOT::Math::VectorUtil::DeltaPhi(tlv_bH, tlv_tauH_SVFIT));
	theSmallTree.m_BDT_bH_tauH_SVFIT_InvMass = ROOT::Math::VectorUtil::InvariantMass(tlv_bH, tlv_tauH_SVFIT);
	theSmallTree.m_BDT_total_CalcPhi = Calculate_phi (getLVfromTLV(tlv_firstLepton), getLVfromTLV(tlv_secondLepton), getLVfromTLV(tlv_firstBjet), getLVfromTLV(tlv_secondBjet), getLVfromTLV(tlv_tauH_SVFIT), getLVfromTLV(tlv_bH));
	theSmallTree.m_BDT_ditau_CalcPhi = Calculate_phi1(getLVfromTLV(tlv_firstLepton), getLVfromTLV(tlv_secondLepton), getLVfromTLV(tlv_tauH_SVFIT), getLVfromTLV(tlv_bH));
	theSmallTree.m_BDT_dib_CalcPhi   = Calculate_phi1(getLVfromTLV(tlv_firstBjet), getLVfromTLV(tlv_secondBjet), getLVfromTLV(tlv_tauH_SVFIT), getLVfromTLV(tlv_bH));
	theSmallTree.m_BDT_MET_tauH_SVFIT_cosTheta   = Calculate_cosTheta_2bodies(getLVfromTLV(tlv_MET), getLVfromTLV(tlv_tauH_SVFIT));
	theSmallTree.m_BDT_tauH_SVFIT_reson_cosTheta = Calculate_cosTheta_2bodies(getLVfromTLV(tlv_tauH_SVFIT), getLVfromTLV((tlv_firstLepton+tlv_secondLepton+tlv_firstBjet+tlv_secondBjet+tlv_MET)) );

      } // in case the SVFIT mass is calculated

        // compute HHKinFit -- ask a reasonable mass window to suppress most error messages
      bool wrongHHK=false;
      float HHKmass = -999;
      float HHKChi2 = -999;
      // if (runHHKinFit && tlv_HH_raw.M() > 20 && tlv_HH_raw.M() < 200)
      //if (runHHKinFit && pairType <= 2 && tlv_bH_raw.M() > 50 && tlv_bH_raw.M() < 200 && theBigTree.SVfitMass->at (chosenTauPair) > 50 && theBigTree.SVfitMass->at (chosenTauPair) < 200) // no kinfit for ee / mumu + very loose mass window
      if (runHHKinFit && pairType <= 3) // FIXME: temporary
      {
	HHKinFit2::HHKinFitMasterHeavyHiggs kinFits = HHKinFit2::HHKinFitMasterHeavyHiggs(tlv_firstBjet, tlv_secondBjet, tlv_firstLepton, tlv_secondLepton, ptmiss, stableMetCov, bjet1_JER, bjet2_JER) ;
	HHKinFit2::HHKinFitMasterHeavyHiggs kinFitsraw = HHKinFit2::HHKinFitMasterHeavyHiggs(tlv_firstBjet, tlv_secondBjet, tlv_firstLepton, tlv_secondLepton,  ptmiss, stableMetCov, bjet1_JER, bjet2_JER) ;

	//           kinFits.setAdvancedBalance (&ptmiss, metcov) ;
	//           kinFits.setSimpleBalance (ptmiss.Pt (),10) ; //alternative which uses only the absolute value of ptmiss in the fit
	//
	//           kinFits.addMh1Hypothesis (hypo_mh1) ;
	//           kinFits.addMh2Hypothesis (hypo_mh2) ;
	kinFits.   addHypo(hypo_mh1,hypo_mh2);
	kinFitsraw.addHypo(hypo_mh1,hypo_mh2);

	if(DEBUG)
	{
	  cout<<"---Kinfit Input debug---"<<endl;
	  cout<<"L1"<<endl;
	  cout<<"(pt,eta,phi,e) "<<tlv_firstLepton.Pt()<<","<<tlv_firstLepton.Eta()<<","<<tlv_firstLepton.Phi()<<","<<tlv_firstLepton.E()<<endl;
	  cout<<"L2"<<endl;
	  cout<<"(pt,eta,phi,e) "<<tlv_secondLepton.Pt()<<","<<tlv_secondLepton.Eta()<<","<<tlv_secondLepton.Phi()<<","<<tlv_secondLepton.E()<<endl;
	  cout<<"B1"<<endl;
	  cout<<"(pt,eta,phi,e,res) "<<tlv_firstBjet.Pt()<<","<<tlv_firstBjet.Eta()<<","<<tlv_firstBjet.Phi()<<","<<tlv_firstBjet.E()<<","<<bjet1_JER<<endl;
	  cout<<"B2"<<endl;
	  cout<<"(pt,eta,phi,e,res) "<<tlv_secondBjet.Pt()<<","<<tlv_secondBjet.Eta()<<","<<tlv_secondBjet.Phi()<<","<<tlv_secondBjet.E()<<","<<bjet2_JER<<endl;
	  cout<<"MET"<<endl;
	  cout<<"(Px,Py) "<<","<<ptmiss.Px()<<","<<ptmiss.Py()<<endl;
	  cout<<"METCOV "<<endl;
	  cout<<metcov(0,0)<<"  "<<metcov(0,1)<<endl;
	  cout<<metcov(1,0)<<"  "<<metcov(1,1)<<endl;
	}

	try{ kinFits.fit();}
	catch(HHKinFit2::HHInvMConstraintException &e)
	{
	  cout<<"INVME THIS EVENT WAS WRONG, INV MASS CONSTRAIN EXCEPTION"<<endl;
	  cout<<"INVME masshypo1 = 125,    masshypo2 = 125"<<endl;
	  cout<<"INVME Tau1"<<endl;
	  cout<<"INVME (E,Px,Py,Pz,M) "<<tlv_firstLepton.E()<<","<<tlv_firstLepton.Px()<<","<<tlv_firstLepton.Py()<<","<<tlv_firstLepton.Pz()<<","<<tlv_firstLepton.M()<<endl;
	  cout<<"INVME Tau2"<<endl;
	  cout<<"INVME (E,Px,Py,Pz,M) "<<tlv_secondLepton.E()<<","<<tlv_secondLepton.Px()<<","<<tlv_secondLepton.Py()<<","<<tlv_secondLepton.Pz()<<","<<tlv_secondLepton.M()<<endl;
	  cout<<"INVME B1"<<endl;
	  cout<<"INVME (E,Px,Py,Pz,M) "<<tlv_firstBjet.E()<<","<<tlv_firstBjet.Px()<<","<<tlv_firstBjet.Py()<<","<<tlv_firstBjet.Pz()<<","<<tlv_firstBjet.M()<<endl;
	  cout<<"INVME B2"<<endl;
	  cout<<"INVME (E,Px,Py,Pz,M) "<<tlv_secondBjet.E()<<","<<tlv_secondBjet.Px()<<","<<tlv_secondBjet.Py()<<","<<tlv_secondBjet.Pz()<<","<<tlv_secondBjet.M()<<endl;
	  cout<<"INVME MET"<<endl;
	  cout<<"INVME (E,Px,Py,Pz,M) "<<","<<ptmiss.Px()<<","<<ptmiss.Py()<<endl;
	  cout<<"INVME METCOV "<<endl;
	  cout<<"INVME "<<metcov (0,0)<<"  "<<metcov (0,1)<<endl;// = theBigTree.MET_cov00->at (chosenTauPair) ;
	  cout<<"INVME "<<metcov (1,0)<<"  "<<metcov (1,1)<<endl;// = theBigTree.MET_cov10->at (chosenTauPair) ;
	  cout<<"INVME tau1, tau2, b1, b2"<<endl;
	  cout<<"INVME ";
	  tlv_firstLepton.Print();
	  cout<<"INVME ";
	  tlv_secondLepton.Print();
	  cout<<"INVME ";
	  tlv_firstBjet.Print();
	  cout<<"INVME ";
	  tlv_secondBjet.Print();
	  wrongHHK=true;
	}
	catch (HHKinFit2::HHEnergyRangeException &e)
	{
	  cout<<"ERANGE THIS EVENT WAS WRONG, ENERGY RANGE EXCEPTION"<<endl;
	  cout<<"ERANGE masshypo1 = 125,    masshypo2 = 125"<<endl;
	  cout<<"ERANGE Tau1"<<endl;
	  cout<<"ERANGE (E,Px,Py,Pz,M) "<<tlv_firstLepton.E()<<","<<tlv_firstLepton.Px()<<","<<tlv_firstLepton.Py()<<","<<tlv_firstLepton.Pz()<<","<<tlv_firstLepton.M()<<endl;
	  cout<<"ERANGE Tau2"<<endl;
	  cout<<"ERANGE (E,Px,Py,Pz,M) "<<tlv_secondLepton.E()<<","<<tlv_secondLepton.Px()<<","<<tlv_secondLepton.Py()<<","<<tlv_secondLepton.Pz()<<","<<tlv_secondLepton.M()<<endl;
	  cout<<"ERANGE B1"<<endl;
	  cout<<"ERANGE (E,Px,Py,Pz,M) "<<tlv_firstBjet.E()<<","<<tlv_firstBjet.Px()<<","<<tlv_firstBjet.Py()<<","<<tlv_firstBjet.Pz()<<","<<tlv_firstBjet.M()<<endl;
	  cout<<"ERANGE B2"<<endl;
	  cout<<"ERANGE (E,Px,Py,Pz,M) "<<tlv_secondBjet.E()<<","<<tlv_secondBjet.Px()<<","<<tlv_secondBjet.Py()<<","<<tlv_secondBjet.Pz()<<","<<tlv_secondBjet.M()<<endl;
	  cout<<"ERANGE MET"<<endl;
	  cout<<"ERANGE (E,Px,Py,Pz,M) "<<","<<ptmiss.Px()<<","<<ptmiss.Py()<<endl;
	  cout<<"ERANGE METCOV "<<endl;
	  cout<<"ERANGE "<<metcov (0,0)<<"  "<<metcov (0,1)<<endl;// = theBigTree.MET_cov00->at (chosenTauPair) ;
	  cout<<"ERANGE "<<metcov (1,0)<<"  "<<metcov (1,1)<<endl;// = theBigTree.MET_cov10->at (chosenTauPair) ;
	  cout<<"ERANGE tau1, tau2, b1, b2"<<endl;
	  cout<<"ERANGE ";
	  tlv_firstLepton.Print();
	  cout<<"ERANGE ";
	  tlv_secondLepton.Print();
	  cout<<"ERANGE ";
	  tlv_firstBjet.Print();
	  cout<<"ERANGE ";
	  tlv_secondBjet.Print();
	  wrongHHK=true;
	}
	catch(HHKinFit2::HHEnergyConstraintException &e)
	{
	  cout<<"ECON THIS EVENT WAS WRONG, ENERGY CONSTRAIN EXCEPTION"<<endl;
	  cout<<"ECON masshypo1 = 125,    masshypo2 = 125"<<endl;
	  cout<<"ECON Tau1"<<endl;
	  cout<<"ECON (E,Px,Py,Pz,M) "<<tlv_firstLepton.E()<<","<<tlv_firstLepton.Px()<<","<<tlv_firstLepton.Py()<<","<<tlv_firstLepton.Pz()<<","<<tlv_firstLepton.M()<<endl;
	  cout<<"ECON Tau2"<<endl;
	  cout<<"ECON (E,Px,Py,Pz,M) "<<tlv_secondLepton.E()<<","<<tlv_secondLepton.Px()<<","<<tlv_secondLepton.Py()<<","<<tlv_secondLepton.Pz()<<","<<tlv_secondLepton.M()<<endl;
	  cout<<"ECON B1"<<endl;
	  cout<<"ECON (E,Px,Py,Pz,M) "<<tlv_firstBjet.E()<<","<<tlv_firstBjet.Px()<<","<<tlv_firstBjet.Py()<<","<<tlv_firstBjet.Pz()<<","<<tlv_firstBjet.M()<<endl;
	  cout<<"ECON B2"<<endl;
	  cout<<"ECON (E,Px,Py,Pz,M) "<<tlv_secondBjet.E()<<","<<tlv_secondBjet.Px()<<","<<tlv_secondBjet.Py()<<","<<tlv_secondBjet.Pz()<<","<<tlv_secondBjet.M()<<endl;
	  cout<<"ECON MET"<<endl;
	  cout<<"ECON (E,Px,Py,Pz,M) "<<","<<ptmiss.Px()<<","<<ptmiss.Py()<<endl;
	  cout<<"ECON METCOV "<<endl;
	  cout<<"ECON "<<metcov (0,0)<<"  "<<metcov (0,1)<<endl;// = theBigTree.MET_cov00->at (chosenTauPair) ;
	  cout<<"ECON "<<metcov (1,0)<<"  "<<metcov (1,1)<<endl;// = theBigTree.MET_cov10->at (chosenTauPair) ;
	  cout<<"ECON tau1, tau2, b1, b2"<<endl;
	  cout<<"ECON ";
	  tlv_firstLepton.Print();
	  cout<<"ECON ";
	  tlv_secondLepton.Print();
	  cout<<"ECON ";
	  tlv_firstBjet.Print();
	  cout<<"ECON ";
	  tlv_secondBjet.Print();
	  wrongHHK=true;
	}
	if(!wrongHHK)
	{
	  HHKmass = kinFits.getMH () ;
	  HHKChi2 = kinFits.getChi2 () ;
	}
	else
	{
	  if(isOS)HHKmass = -333;
	}

	// nominal kinfit raw
	bool wrongHHKraw =false;
	try {kinFitsraw.fit();}
	catch(HHKinFit2::HHInvMConstraintException   &e){wrongHHKraw=true;}
	catch(HHKinFit2::HHEnergyConstraintException &e){wrongHHKraw=true;}
	catch(HHKinFit2::HHEnergyRangeException      &e){wrongHHKraw=true;}
	if(!wrongHHKraw)
	{
	  theSmallTree.m_HHKin_mass_raw = kinFitsraw.getMH();
	  theSmallTree.m_HHKin_mass_raw_chi2        = kinFitsraw.getChi2();
	  theSmallTree.m_HHKin_mass_raw_convergence = kinFitsraw.getConvergence();
	  theSmallTree.m_HHKin_mass_raw_prob        = kinFitsraw.getFitProb();
	  if(DEBUG)
	  {
	    cout<<"---Kinfit Output debug---"<<endl;
	    cout<<"KinFit mass = " << kinFitsraw.getMH() << endl;
	    cout<<"KinFit chi2 = " << kinFitsraw.getChi2()<< endl;
	    cout<<"KinFit conv = " << kinFitsraw.getConvergence()<< endl;
	  }
	}
	else theSmallTree.m_HHKin_mass_raw = -100 ;
	if ( (theBigTree.SVfitMass->at(chosenTauPair) > -900. || (doSmearing && theSmallTree.m_tauH_SVFIT_mass > 0)) && !wrongHHK)
	{
	  TLorentzVector b1 = kinFits.getFittedBJet1();
	  TLorentzVector b2 = kinFits.getFittedBJet2();
	  TLorentzVector bH_HKin = b1 + b2;
	  TLorentzVector tlv_HHsvfit = bH_HKin + tlv_tauH_SVFIT ;

	  theSmallTree.m_HHkinsvfit_bHmass = bH_HKin.M();
	  theSmallTree.m_HHkinsvfit_pt  = tlv_HHsvfit.Pt () ;
	  theSmallTree.m_HHkinsvfit_eta = tlv_HHsvfit.Eta () ;
	  theSmallTree.m_HHkinsvfit_phi = tlv_HHsvfit.Phi () ;
	  theSmallTree.m_HHkinsvfit_e   = tlv_HHsvfit.E () ;
	  theSmallTree.m_HHkinsvfit_m   = tlv_HHsvfit.M () ;
	} // in case the SVFIT mass is calculated

      } // end if doing HHKinFit

      theSmallTree.m_HHKin_mass_raw_copy = theSmallTree.m_HHKin_mass_raw ; // store twice if different binning needed

      theSmallTree.m_HHKin_mass = HHKmass;//kinFits.getMH () ;
      theSmallTree.m_HHKin_chi2 = HHKChi2;//kinFits.getChi2 () ;

      // Stransverse mass
      if (runMT2)
      {
	double mVisA = tlv_firstBjet_raw.M(); // mass of visible object on side A.  Must be >=0.
	double pxA = tlv_firstBjet_raw.Px();  // x momentum of visible object on side A.
	double pyA = tlv_firstBjet_raw.Py();  // y momentum of visible object on side A.

	double mVisB = tlv_secondBjet_raw.M(); // mass of visible object on side B.  Must be >=0.
	double pxB = tlv_secondBjet_raw.Px();  // x momentum of visible object on side B.
	double pyB = tlv_secondBjet_raw.Py();  // y momentum of visible object on side B.

	double pxMiss = tlv_firstLepton.Px() + tlv_secondLepton.Px() + theSmallTree.m_METx; // x component of missing transverse momentum.
	double pyMiss = tlv_firstLepton.Py() + tlv_secondLepton.Py() + theSmallTree.m_METy; // y component of missing transverse momentum.

	double chiA = tlv_firstLepton.M();  // hypothesised mass of invisible on side A.  Must be >=0.
	double chiB = tlv_secondLepton.M(); // hypothesised mass of invisible on side B.  Must be >=0.

	double desiredPrecisionOnMt2 = 0; // Must be >=0.  If 0 alg aims for machine precision.  if >0, MT2 computed to supplied absolute precision.

	asymm_mt2_lester_bisect::disableCopyrightMessage();

	double MT2 = asymm_mt2_lester_bisect::get_mT2(
	  mVisA, pxA, pyA,
	  mVisB, pxB, pyB,
	  pxMiss, pyMiss,
	  chiA, chiB,
	  desiredPrecisionOnMt2);

	theSmallTree.m_MT2 = MT2;

      } // end calcultion of MT2

      if (DEBUG)  cout << "---------- MT2 DEBUG: " << theSmallTree.m_MT2 << endl;

      theSmallTree.m_HH_deltaPhi = deltaPhi (tlv_bH.Phi (), tlv_tauH.Phi ()) ;
      theSmallTree.m_HH_deltaEta = fabs(tlv_bH.Eta()- tlv_tauH.Eta ()) ;
      theSmallTree.m_HHsvfit_deltaPhi = deltaPhi (tlv_bH.Phi (), tlv_tauH_SVFIT.Phi ()) ;
      theSmallTree.m_bHMet_deltaPhi = deltaPhi (vMET.Phi(), tlv_bH.Phi ()) ;
      theSmallTree.m_dib_deltaPhi = deltaPhi (tlv_firstBjet.Phi (), tlv_secondBjet.Phi ()) ;
      theSmallTree.m_dib_deltaEta = fabs(tlv_firstBjet.Eta()-tlv_secondBjet.Eta ()) ;
      theSmallTree.m_dib_deltaR = tlv_firstBjet.DeltaR(tlv_secondBjet) ;
      theSmallTree.m_dib_deltaR_per_bHpt = theSmallTree.m_dib_deltaR * tlv_bH_raw.Pt();
      theSmallTree.m_dib_dEtaSign = tlv_firstBjet.Eta() * tlv_secondBjet.Eta(); // VBF BDT

      theSmallTree.m_BDT_dib_deltaPhi     = ROOT::Math::VectorUtil::DeltaPhi(tlv_firstBjet, tlv_secondBjet);
      theSmallTree.m_BDT_dib_abs_deltaPhi = fabs(ROOT::Math::VectorUtil::DeltaPhi(tlv_firstBjet, tlv_secondBjet));

      vector <float> dRBTau;
      dRBTau.push_back (tlv_firstLepton.DeltaR(tlv_firstBjet));
      dRBTau.push_back (tlv_firstLepton.DeltaR(tlv_secondBjet));
      dRBTau.push_back (tlv_secondLepton.DeltaR(tlv_firstBjet));
      dRBTau.push_back (tlv_secondLepton.DeltaR(tlv_secondBjet));
      theSmallTree.m_btau_deltaRmin = *std::min_element(dRBTau.begin(), dRBTau.end());
      theSmallTree.m_btau_deltaRmax = *std::max_element(dRBTau.begin(), dRBTau.end());

      // Resize VBF up/down variations to N_jecSources
      theSmallTree.m_VBFjet1_pt_jetup    .resize(N_jecSources,-999.);
      theSmallTree.m_VBFjet1_pt_jetdown  .resize(N_jecSources,-999.);
      theSmallTree.m_VBFjet1_mass_jetup  .resize(N_jecSources,-999.);
      theSmallTree.m_VBFjet1_mass_jetdown.resize(N_jecSources,-999.);
      theSmallTree.m_VBFjet2_pt_jetup    .resize(N_jecSources,-999.);
      theSmallTree.m_VBFjet2_pt_jetdown  .resize(N_jecSources,-999.);
      theSmallTree.m_VBFjet2_mass_jetup  .resize(N_jecSources,-999.);
      theSmallTree.m_VBFjet2_mass_jetdown.resize(N_jecSources,-999.);
      theSmallTree.m_VBFjj_mass_jetup    .resize(N_jecSources,-999.);
      theSmallTree.m_VBFjj_mass_jetdown  .resize(N_jecSources,-999.);

      // Save other VBF related quantities
      if ( theBigTree.jets_px->size()> 1 && VBFcand_Mjj.size()>0 )
      {
	// save the VBF-jets
	TLorentzVector VBFjet1;
	VBFjet1.SetPxPyPzE( theBigTree.jets_px->at(VBFidx1), theBigTree.jets_py->at(VBFidx1), theBigTree.jets_pz->at(VBFidx1), theBigTree.jets_e->at(VBFidx1) );
	TLorentzVector VBFjet2;
	VBFjet2.SetPxPyPzE( theBigTree.jets_px->at(VBFidx2), theBigTree.jets_py->at(VBFidx2), theBigTree.jets_pz->at(VBFidx2), theBigTree.jets_e->at(VBFidx2) );

	if (doSmearing)
	{
	  VBFjet1 = VBFjet1 * jets_and_smearFactor[VBFidx1];
	  VBFjet2 = VBFjet2 * jets_and_smearFactor[VBFidx2];
	}

	pair <vector <double>, vector<double>> unc_VBF1_updown = JECprovider.getJECUncVectors(VBFidx1, theBigTree);
	pair <vector <double>, vector<double>> unc_VBF2_updown = JECprovider.getJECUncVectors(VBFidx2, theBigTree);

	// Total JES up/down variation
	TLorentzVector tlv_VBFjet1_jetupTot   = VBFjet1;
	TLorentzVector tlv_VBFjet1_jetdownTot = VBFjet1;
	TLorentzVector tlv_VBFjet2_jetupTot   = VBFjet2;
	TLorentzVector tlv_VBFjet2_jetdownTot = VBFjet2;

	if (isMC)
	{

	  tlv_VBFjet1_jetupTot   = getShiftedJet(VBFjet1, +1., unc_VBF1_updown.first [N_jecSources]);
	  tlv_VBFjet1_jetdownTot = getShiftedJet(VBFjet1, -1., unc_VBF1_updown.second[N_jecSources]);
	  tlv_VBFjet2_jetupTot   = getShiftedJet(VBFjet2, +1., unc_VBF2_updown.first [N_jecSources]);
	  tlv_VBFjet2_jetdownTot = getShiftedJet(VBFjet2, -1., unc_VBF2_updown.second[N_jecSources]);
	}

	theSmallTree.m_VBFjet1_pt_jetupTot       = tlv_VBFjet1_jetupTot.Pt();
	theSmallTree.m_VBFjet1_pt_jetdownTot     = tlv_VBFjet1_jetdownTot.Pt();
	theSmallTree.m_VBFjet2_pt_jetupTot       = tlv_VBFjet2_jetupTot.Pt();
	theSmallTree.m_VBFjet2_pt_jetdownTot     = tlv_VBFjet2_jetdownTot.Pt();
	theSmallTree.m_VBFjet1_mass_jetupTot     = tlv_VBFjet1_jetupTot.M();
	theSmallTree.m_VBFjet1_mass_jetdownTot   = tlv_VBFjet1_jetdownTot.M();
	theSmallTree.m_VBFjet2_mass_jetupTot     = tlv_VBFjet2_jetupTot.M();
	theSmallTree.m_VBFjet2_mass_jetdownTot   = tlv_VBFjet2_jetdownTot.M();
	theSmallTree.m_VBFjj_mass_jetupTot       = (tlv_VBFjet1_jetupTot   + tlv_VBFjet2_jetupTot).M();
	theSmallTree.m_VBFjj_mass_jetdownTot     = (tlv_VBFjet1_jetdownTot + tlv_VBFjet2_jetdownTot).M();

	// compute all shifted VBFjets
	vector <TLorentzVector> VBFjet1_jetup(N_jecSources,VBFjet1);
	vector <TLorentzVector> VBFjet1_jetdown(N_jecSources,VBFjet1);
	vector <TLorentzVector> VBFjet2_jetup(N_jecSources,VBFjet2);
	vector <TLorentzVector> VBFjet2_jetdown(N_jecSources,VBFjet2);

	vector <TLorentzVector> VBFjj_jetup(N_jecSources, VBFjet1+VBFjet2);
	vector <TLorentzVector> VBFjj_jetdown(N_jecSources, VBFjet1+VBFjet2);

	for (int isource = 0; isource < N_jecSources; isource++)
	{
	  VBFjet1_jetup  [isource] = VBFjet1;
	  VBFjet1_jetdown[isource] = VBFjet1;
	  VBFjet2_jetup  [isource] = VBFjet2;
	  VBFjet2_jetdown[isource] = VBFjet2;
	}

	if (isMC)
	{
	  for (int isource = 0; isource < N_jecSources; isource++)
	  {
	    VBFjet1_jetup  [isource] = getShiftedJet(VBFjet1, +1., unc_VBF1_updown.first [isource]);
	    VBFjet1_jetdown[isource] = getShiftedJet(VBFjet1, -1., unc_VBF1_updown.second[isource]);
	    VBFjet2_jetup  [isource] = getShiftedJet(VBFjet2, +1., unc_VBF2_updown.first [isource]);
	    VBFjet2_jetdown[isource] = getShiftedJet(VBFjet2, -1., unc_VBF2_updown.second[isource]);
	  }
	}

	for (int isource = 0; isource < N_jecSources; isource++)
	{
	  theSmallTree.m_VBFjet1_pt_jetup    [isource] = VBFjet1_jetup  [isource].Pt();
	  theSmallTree.m_VBFjet1_pt_jetdown  [isource] = VBFjet1_jetdown[isource].Pt();
	  theSmallTree.m_VBFjet1_mass_jetup  [isource] = VBFjet1_jetup  [isource].M();
	  theSmallTree.m_VBFjet1_mass_jetdown[isource] = VBFjet1_jetdown[isource].M();
	  theSmallTree.m_VBFjet2_pt_jetup    [isource] = VBFjet2_jetup  [isource].Pt();
	  theSmallTree.m_VBFjet2_pt_jetdown  [isource] = VBFjet2_jetdown[isource].Pt();
	  theSmallTree.m_VBFjet2_mass_jetup  [isource] = VBFjet2_jetup  [isource].M();
	  theSmallTree.m_VBFjet2_mass_jetdown[isource] = VBFjet2_jetdown[isource].M();

	  VBFjj_jetup  [isource] = VBFjet1_jetup  [isource] + VBFjet2_jetup  [isource];
	  VBFjj_jetdown[isource] = VBFjet1_jetdown[isource] + VBFjet2_jetdown[isource];

	  theSmallTree.m_VBFjj_mass_jetup  [isource] = VBFjj_jetup  [isource].M();
	  theSmallTree.m_VBFjj_mass_jetdown[isource] = VBFjj_jetdown[isource].M();
	}

	bool hasgj1_VBF = false;
	bool hasgj2_VBF = false;

	// Save gen info for VBF jets
	if (isMC)
	{
	  int mcind = theBigTree.jets_genjetIndex->at(VBFidx1);
	  if (mcind>=0)
	  {
	    TLorentzVector thisGenJet(theBigTree.genjet_px->at(mcind),theBigTree.genjet_py->at(mcind),theBigTree.genjet_pz->at(mcind),theBigTree.genjet_e->at(mcind));
	    if (thisGenJet.Pt() > 8)
	    {
	      hasgj1_VBF = true;
	      theSmallTree.m_VBFgenjet1_pt =  thisGenJet.Pt() ;
	      theSmallTree.m_VBFgenjet1_eta = thisGenJet.Eta();
	      theSmallTree.m_VBFgenjet1_phi = thisGenJet.Phi();
	      theSmallTree.m_VBFgenjet1_e =   thisGenJet.E();
	    }
	  }
	  mcind = theBigTree.jets_genjetIndex->at(VBFidx2);
	  if (mcind>=0)
	  {
	    TLorentzVector thisGenJet(theBigTree.genjet_px->at(mcind),theBigTree.genjet_py->at(mcind),theBigTree.genjet_pz->at(mcind),theBigTree.genjet_e->at(mcind));
	    if (thisGenJet.Pt() > 8)
	    {
	      hasgj2_VBF = true;
	      theSmallTree.m_VBFgenjet2_pt =  thisGenJet.Pt() ;
	      theSmallTree.m_VBFgenjet2_eta = thisGenJet.Eta();
	      theSmallTree.m_VBFgenjet2_phi = thisGenJet.Phi();
	      theSmallTree.m_VBFgenjet2_e =   thisGenJet.E();
	    }
	  }
	}

	// Save VBF variables
	theSmallTree.m_VBFjj_mass         = std::get<0>(*(VBFcand_Mjj.rbegin()));

	theSmallTree.m_VBFjj_mass_jetup1    = VBFjj_jetup[0].M();
	theSmallTree.m_VBFjj_mass_jetup2    = VBFjj_jetup[1].M();
	theSmallTree.m_VBFjj_mass_jetup3    = VBFjj_jetup[2].M();
	theSmallTree.m_VBFjj_mass_jetup4    = VBFjj_jetup[3].M();
	theSmallTree.m_VBFjj_mass_jetup5    = VBFjj_jetup[4].M();
	theSmallTree.m_VBFjj_mass_jetup6    = VBFjj_jetup[5].M();
	theSmallTree.m_VBFjj_mass_jetup7    = VBFjj_jetup[6].M();
	theSmallTree.m_VBFjj_mass_jetup8    = VBFjj_jetup[7].M();
	theSmallTree.m_VBFjj_mass_jetup9    = VBFjj_jetup[8].M();
	theSmallTree.m_VBFjj_mass_jetup10   = VBFjj_jetup[9].M();
	theSmallTree.m_VBFjj_mass_jetup11   = VBFjj_jetup[10].M();

	theSmallTree.m_VBFjj_mass_jetdown1  = VBFjj_jetdown[0].M();
	theSmallTree.m_VBFjj_mass_jetdown2  = VBFjj_jetdown[1].M();
	theSmallTree.m_VBFjj_mass_jetdown3  = VBFjj_jetdown[2].M();
	theSmallTree.m_VBFjj_mass_jetdown4  = VBFjj_jetdown[3].M();
	theSmallTree.m_VBFjj_mass_jetdown5  = VBFjj_jetdown[4].M();
	theSmallTree.m_VBFjj_mass_jetdown6  = VBFjj_jetdown[5].M();
	theSmallTree.m_VBFjj_mass_jetdown7  = VBFjj_jetdown[6].M();
	theSmallTree.m_VBFjj_mass_jetdown8  = VBFjj_jetdown[7].M();
	theSmallTree.m_VBFjj_mass_jetdown9  = VBFjj_jetdown[8].M();
	theSmallTree.m_VBFjj_mass_jetdown10 = VBFjj_jetdown[9].M();
	theSmallTree.m_VBFjj_mass_jetdown11 = VBFjj_jetdown[10].M();

	theSmallTree.m_VBFjj_deltaEta     	 = fabs(VBFjet1.Eta()-VBFjet2.Eta());
	theSmallTree.m_VBFjj_deltaPhi     	 = VBFjet1.DeltaPhi(VBFjet2);
	theSmallTree.m_VBFjj_HT           	 = VBFjet1.Pt()+VBFjet2.Pt();
	theSmallTree.m_VBFjj_dEtaSign     	 = VBFjet1.Eta() * VBFjet2.Eta(); // VBF BDT

	theSmallTree.m_VBFjet1_pt         	 = VBFjet1.Pt() ;

	theSmallTree.m_VBFjet1_pt_jetup1	 = VBFjet1_jetup[0].Pt();
	theSmallTree.m_VBFjet1_pt_jetup2	 = VBFjet1_jetup[1].Pt();
	theSmallTree.m_VBFjet1_pt_jetup3	 = VBFjet1_jetup[2].Pt();
	theSmallTree.m_VBFjet1_pt_jetup4	 = VBFjet1_jetup[3].Pt();
	theSmallTree.m_VBFjet1_pt_jetup5	 = VBFjet1_jetup[4].Pt();
	theSmallTree.m_VBFjet1_pt_jetup6	 = VBFjet1_jetup[5].Pt();
	theSmallTree.m_VBFjet1_pt_jetup7	 = VBFjet1_jetup[6].Pt();
	theSmallTree.m_VBFjet1_pt_jetup8	 = VBFjet1_jetup[7].Pt();
	theSmallTree.m_VBFjet1_pt_jetup9	 = VBFjet1_jetup[8].Pt();
	theSmallTree.m_VBFjet1_pt_jetup10	 = VBFjet1_jetup[9].Pt();
	theSmallTree.m_VBFjet1_pt_jetup11	 = VBFjet1_jetup[10].Pt();

	theSmallTree.m_VBFjet1_pt_jetdown1	 = VBFjet1_jetdown[0].Pt();
	theSmallTree.m_VBFjet1_pt_jetdown2	 = VBFjet1_jetdown[1].Pt();
	theSmallTree.m_VBFjet1_pt_jetdown3	 = VBFjet1_jetdown[2].Pt();
	theSmallTree.m_VBFjet1_pt_jetdown4	 = VBFjet1_jetdown[3].Pt();
	theSmallTree.m_VBFjet1_pt_jetdown5	 = VBFjet1_jetdown[4].Pt();
	theSmallTree.m_VBFjet1_pt_jetdown6	 = VBFjet1_jetdown[5].Pt();
	theSmallTree.m_VBFjet1_pt_jetdown7	 = VBFjet1_jetdown[6].Pt();
	theSmallTree.m_VBFjet1_pt_jetdown8	 = VBFjet1_jetdown[7].Pt();
	theSmallTree.m_VBFjet1_pt_jetdown9	 = VBFjet1_jetdown[8].Pt();
	theSmallTree.m_VBFjet1_pt_jetdown10	 = VBFjet1_jetdown[9].Pt();
	theSmallTree.m_VBFjet1_pt_jetdown11	 = VBFjet1_jetdown[10].Pt();

	theSmallTree.m_VBFjet1_eta        	 = VBFjet1.Eta();
	theSmallTree.m_VBFjet1_phi     	 = VBFjet1.Phi();
	theSmallTree.m_VBFjet1_e          	 = VBFjet1.E();
	theSmallTree.m_VBFjet1_btag       	 = (theBigTree.bCSVscore->at (VBFidx1)) ;
	theSmallTree.m_VBFjet1_btag_deepCSV    = theBigTree.bDeepCSV_probb->at(VBFidx1) + theBigTree.bDeepCSV_probbb->at(VBFidx1) ;
	theSmallTree.m_VBFjet1_btag_deepFlavor = theBigTree.bDeepFlavor_probb->at(VBFidx1) + theBigTree.bDeepFlavor_probbb->at(VBFidx1) + theBigTree.bDeepFlavor_problepb->at(VBFidx1);
	theSmallTree.m_VBFjet1_ctag_deepFlavor = theBigTree.bDeepFlavor_probc->at(VBFidx1) ;
	theSmallTree.m_VBFjet1_CvsL = getCvsL(theBigTree, VBFidx1);
	theSmallTree.m_VBFjet1_CvsB = getCvsB(theBigTree, VBFidx1);
	theSmallTree.m_VBFjet1_pnet_bb = theBigTree.bParticleNetAK4JetTags_probbb->at(VBFidx1);
	theSmallTree.m_VBFjet1_pnet_cc = theBigTree.bParticleNetAK4JetTags_probcc->at(VBFidx1);
	theSmallTree.m_VBFjet1_pnet_b = theBigTree.bParticleNetAK4JetTags_probb->at(VBFidx1);
	theSmallTree.m_VBFjet1_pnet_c = theBigTree.bParticleNetAK4JetTags_probc->at(VBFidx1);
	theSmallTree.m_VBFjet1_pnet_g = theBigTree.bParticleNetAK4JetTags_probg->at(VBFidx1);
	theSmallTree.m_VBFjet1_pnet_uds = theBigTree.bParticleNetAK4JetTags_probuds->at(VBFidx1);
	theSmallTree.m_VBFjet1_pnet_pu = theBigTree.bParticleNetAK4JetTags_probpu->at(VBFidx1);
	theSmallTree.m_VBFjet1_pnet_undef = theBigTree.bParticleNetAK4JetTags_probundef->at(VBFidx1);
	theSmallTree.m_VBFjet1_PUjetIDupdated  = theBigTree.jets_PUJetIDupdated->at (VBFidx1) ;
	theSmallTree.m_VBFjet1_flav            = (theBigTree.jets_HadronFlavour->at (VBFidx1)) ;
	theSmallTree.m_VBFjet1_hasgenjet       = hasgj1_VBF ;

	theSmallTree.m_VBFjet2_pt         	 = VBFjet2.Pt() ;

	theSmallTree.m_VBFjet2_pt_jetup1         = VBFjet2_jetup[0].Pt();
	theSmallTree.m_VBFjet2_pt_jetup2	   = VBFjet2_jetup[1].Pt();
	theSmallTree.m_VBFjet2_pt_jetup3	   = VBFjet2_jetup[2].Pt();
	theSmallTree.m_VBFjet2_pt_jetup4	   = VBFjet2_jetup[3].Pt();
	theSmallTree.m_VBFjet2_pt_jetup5	   = VBFjet2_jetup[4].Pt();
	theSmallTree.m_VBFjet2_pt_jetup6	   = VBFjet2_jetup[5].Pt();
	theSmallTree.m_VBFjet2_pt_jetup7	   = VBFjet2_jetup[6].Pt();
	theSmallTree.m_VBFjet2_pt_jetup8	   = VBFjet2_jetup[7].Pt();
	theSmallTree.m_VBFjet2_pt_jetup9	   = VBFjet2_jetup[8].Pt();
	theSmallTree.m_VBFjet2_pt_jetup10	   = VBFjet2_jetup[9].Pt();
	theSmallTree.m_VBFjet2_pt_jetup11	   = VBFjet2_jetup[10].Pt();

	theSmallTree.m_VBFjet2_pt_jetdown1	     = VBFjet2_jetdown[0].Pt();
	theSmallTree.m_VBFjet2_pt_jetdown2	     = VBFjet2_jetdown[1].Pt();
	theSmallTree.m_VBFjet2_pt_jetdown3	     = VBFjet2_jetdown[2].Pt();
	theSmallTree.m_VBFjet2_pt_jetdown4	     = VBFjet2_jetdown[3].Pt();
	theSmallTree.m_VBFjet2_pt_jetdown5	     = VBFjet2_jetdown[4].Pt();
	theSmallTree.m_VBFjet2_pt_jetdown6	     = VBFjet2_jetdown[5].Pt();
	theSmallTree.m_VBFjet2_pt_jetdown7	     = VBFjet2_jetdown[6].Pt();
	theSmallTree.m_VBFjet2_pt_jetdown8	     = VBFjet2_jetdown[7].Pt();
	theSmallTree.m_VBFjet2_pt_jetdown9	     = VBFjet2_jetdown[8].Pt();
	theSmallTree.m_VBFjet2_pt_jetdown10	     = VBFjet2_jetdown[9].Pt();
	theSmallTree.m_VBFjet2_pt_jetdown11	     = VBFjet2_jetdown[10].Pt();

	theSmallTree.m_VBFjet2_eta        	 = VBFjet2.Eta();
	theSmallTree.m_VBFjet2_phi        	 = VBFjet2.Phi();
	theSmallTree.m_VBFjet2_e          	 = VBFjet2.E();
	theSmallTree.m_VBFjet2_btag       	 = (theBigTree.bCSVscore->at (VBFidx2)) ;
	theSmallTree.m_VBFjet2_btag_deepCSV    = theBigTree.bDeepCSV_probb->at(VBFidx2) + theBigTree.bDeepCSV_probbb->at(VBFidx2) ;
	theSmallTree.m_VBFjet2_btag_deepFlavor = theBigTree.bDeepFlavor_probb->at(VBFidx2) + theBigTree.bDeepFlavor_probbb->at(VBFidx2) + theBigTree.bDeepFlavor_problepb->at(VBFidx2);
	theSmallTree.m_VBFjet2_ctag_deepFlavor = theBigTree.bDeepFlavor_probc->at(VBFidx2) ;
	theSmallTree.m_VBFjet2_CvsL = getCvsL(theBigTree, VBFidx2);
	theSmallTree.m_VBFjet2_CvsB = getCvsB(theBigTree, VBFidx2);
	theSmallTree.m_VBFjet2_pnet_bb = theBigTree.bParticleNetAK4JetTags_probbb->at(VBFidx2);
	theSmallTree.m_VBFjet2_pnet_cc = theBigTree.bParticleNetAK4JetTags_probcc->at(VBFidx2);
	theSmallTree.m_VBFjet2_pnet_b = theBigTree.bParticleNetAK4JetTags_probb->at(VBFidx2);
	theSmallTree.m_VBFjet2_pnet_c = theBigTree.bParticleNetAK4JetTags_probc->at(VBFidx2);
	theSmallTree.m_VBFjet2_pnet_g = theBigTree.bParticleNetAK4JetTags_probg->at(VBFidx2);
	theSmallTree.m_VBFjet2_pnet_uds = theBigTree.bParticleNetAK4JetTags_probuds->at(VBFidx2);
	theSmallTree.m_VBFjet2_pnet_pu = theBigTree.bParticleNetAK4JetTags_probpu->at(VBFidx2);
	theSmallTree.m_VBFjet2_pnet_undef = theBigTree.bParticleNetAK4JetTags_probundef->at(VBFidx2);
	theSmallTree.m_VBFjet2_PUjetIDupdated  = theBigTree.jets_PUJetIDupdated->at (VBFidx2) ;
	theSmallTree.m_VBFjet2_flav            = (theBigTree.jets_HadronFlavour->at (VBFidx2)) ;
	theSmallTree.m_VBFjet2_hasgenjet  	 = hasgj2_VBF ;
	theSmallTree.m_VBFjet2_PUjetID    	 = (theBigTree.jets_PUJetIDupdated->at (VBFidx2)); // VBF BDT
	theSmallTree.m_bH_VBF1_deltaEta   	 = fabs(tlv_bH.Eta() - VBFjet1.Eta()); // VBF BDT

	theSmallTree.m_dau1_z = getZ(tlv_firstLepton.Eta(),VBFjet1.Eta(),VBFjet2.Eta());
	theSmallTree.m_dau2_z = getZ(tlv_secondLepton.Eta(),VBFjet1.Eta(),VBFjet2.Eta());
	theSmallTree.m_bjet1_z = getZ(tlv_firstBjet.Eta(),VBFjet1.Eta(),VBFjet2.Eta());
	theSmallTree.m_bjet2_z = getZ(tlv_secondBjet.Eta(),VBFjet1.Eta(),VBFjet2.Eta());
	theSmallTree.m_tauH_z = getZ(tlv_tauH.Eta(),VBFjet1.Eta(),VBFjet2.Eta());
	theSmallTree.m_bH_z = getZ(tlv_bH.Eta(),VBFjet1.Eta(),VBFjet2.Eta());
	theSmallTree.m_HH_z = getZ(tlv_HH.Eta(),VBFjet1.Eta(),VBFjet2.Eta());

	//boson centrality
	float DeltaEta_minus = std::min(tlv_tauH.Eta(), tlv_bH.Eta())- std::min(VBFjet1.Eta(), VBFjet2.Eta());
	float DeltaEta_plus =std::max(VBFjet1.Eta(), VBFjet2.Eta()) - std::max(tlv_tauH.Eta(), tlv_bH.Eta());
	float zV = std::min(DeltaEta_minus, DeltaEta_plus);
	theSmallTree.m_HH_zV = zV;

	//pT balance
	TVector3 v_tauH = tlv_tauH.Vect();
	TVector3 v_bH = tlv_bH.Vect();
	float HH_A = (v_tauH + v_bH).Mag()/(tlv_tauH.Pt() + tlv_bH.Pt());
	theSmallTree.m_HH_A = HH_A;
      }

      // loop over jets
      int genjets = 0;
      int jets  = 0;
      for (unsigned int iJet = 0; (iJet < theBigTree.jets_px->size ()) && (theSmallTree.m_njets < maxNjetsSaved); ++iJet)
      {
	// PG filter jets at will
	if (theBigTree.PFjetID->at (iJet) < PFjetID_WP) continue; // 0 ; don't pass PF Jet ID; 1: tight, 2: tightLepVeto

	// skip the H decay candiates
	if (int (iJet) == bjet1idx ){
	  continue;
	}else if(int (iJet) == bjet2idx){
	  continue ;
	}
	TLorentzVector tlv_dummyJet(theBigTree.jets_px->at(iJet), theBigTree.jets_py->at(iJet), theBigTree.jets_pz->at(iJet), theBigTree.jets_e->at(iJet));
	if (doSmearing) tlv_dummyJet = tlv_dummyJet * jets_and_smearFactor[iJet];

	// Apply PUjetID only to jets with pt < 50 GeV ( https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorkingLegacyRun2#Jets )
	// PU jet ID WP = 2: loose
	if (PUjetID_WP > -1)
	{
	  if ( !(CheckBit(theBigTree.jets_PUJetIDupdated_WP->at(iJet), PUjetID_WP)) && tlv_dummyJet.Pt()<50.) continue;
	}

	// remove jets that overlap with the tau selected in the leg 1 and 2
	if (tlv_firstLepton.DeltaR(tlv_dummyJet) < lepCleaningCone){
	  continue;
	}
	if (tlv_secondLepton.DeltaR(tlv_dummyJet) < lepCleaningCone){
	  continue;
	}

	// find matching gen jet
	bool hasgj = false;
	if (isMC)
	{
	  int mcind = theBigTree.jets_genjetIndex->at(iJet);
	  if (mcind>=0)
	  {
	    TLorentzVector thisGenJet(theBigTree.genjet_px->at(mcind),theBigTree.genjet_py->at(mcind),theBigTree.genjet_pz->at(mcind),theBigTree.genjet_e->at(mcind));
	    if (thisGenJet.Pt() > 8)
	    {
	      hasgj = true;
	    }
	    genjets ++;
	  }
	}

	//if VBF, skip VBF jets candidates and save 5th jet
	if (VBFcand_Mjj.size()>0)
	{
	  if(int (iJet) != VBFidx1 && int (iJet) != VBFidx2 and jets == 0)
	  {
	    jets++;
	  }
	  if(int (iJet) != VBFidx1 && int (iJet) != VBFidx2) ++ theSmallTree.m_addjets;
	}

	theSmallTree.m_jets_pt.push_back (tlv_dummyJet.Pt ()) ;
	theSmallTree.m_jets_eta.push_back (tlv_dummyJet.Eta ()) ;
	theSmallTree.m_jets_phi.push_back (tlv_dummyJet.Phi ()) ;
	theSmallTree.m_jets_e.push_back (tlv_dummyJet.E ()) ;
	theSmallTree.m_jets_btag.push_back (theBigTree.bCSVscore->at (iJet)) ;
	theSmallTree.m_jets_btag_deepCSV.push_back (theBigTree.bDeepCSV_probb->at(iJet) + theBigTree.bDeepCSV_probbb->at(iJet)) ;
	theSmallTree.m_jets_btag_deepFlavor.push_back (theBigTree.bDeepFlavor_probb->at(iJet) + theBigTree.bDeepFlavor_probbb->at(iJet) + theBigTree.bDeepFlavor_problepb->at(iJet)) ;
	theSmallTree.m_jets_ctag_deepFlavor.push_back (theBigTree.bDeepFlavor_probc->at(iJet));
	theSmallTree.m_jets_CvsL.push_back( getCvsL(theBigTree, iJet) );
	theSmallTree.m_jets_CvsB.push_back( getCvsB(theBigTree, iJet) );
	theSmallTree.m_jets_flav.push_back (theBigTree.jets_HadronFlavour->at (iJet)) ;
	theSmallTree.m_jets_hasgenjet.push_back (hasgj) ;
	theSmallTree.m_jets_pnet_bb.push_back(theBigTree.bParticleNetAK4JetTags_probbb->at(iJet));
	theSmallTree.m_jets_pnet_cc.push_back(theBigTree.bParticleNetAK4JetTags_probcc->at(iJet));
	theSmallTree.m_jets_pnet_b.push_back(theBigTree.bParticleNetAK4JetTags_probb->at(iJet));
	theSmallTree.m_jets_pnet_c.push_back(theBigTree.bParticleNetAK4JetTags_probc->at(iJet));
	theSmallTree.m_jets_pnet_g.push_back(theBigTree.bParticleNetAK4JetTags_probg->at(iJet));
	theSmallTree.m_jets_pnet_uds.push_back(theBigTree.bParticleNetAK4JetTags_probuds->at(iJet));
	theSmallTree.m_jets_pnet_pu.push_back(theBigTree.bParticleNetAK4JetTags_probpu->at(iJet));
	theSmallTree.m_jets_pnet_undef.push_back(theBigTree.bParticleNetAK4JetTags_probundef->at(iJet));
	++theSmallTree.m_njets ;
      } // loop over jets

      if (DEBUG)
      {
	cout << "-------- JETS DEBUG -------" << endl;
	cout << "b1 (pt,eta,flav, deepFlavor): " << theSmallTree.m_bjet1_pt << "  " << theSmallTree.m_bjet1_eta << "  " << theSmallTree.m_bjet1_flav << "  " << theSmallTree.m_bjet1_bID_deepFlavor << endl;
	cout << "b2 (pt,eta,flav, deepFlavor): " << theSmallTree.m_bjet2_pt << "  " << theSmallTree.m_bjet2_eta << "  " << theSmallTree.m_bjet2_flav << "  " << theSmallTree.m_bjet2_bID_deepFlavor << endl;
	cout << "Other jets:" << endl;
	for (unsigned int i=0; i<theSmallTree.m_jets_pt.size(); i++)
	{
	  cout << "Jet " << i << " (pt,eta,flav, deepFlavor): " << theSmallTree.m_jets_pt.at(i) << "  " << theSmallTree.m_jets_eta.at(i) << "  " << theSmallTree.m_jets_flav.at(i) << "  " << theSmallTree.m_jets_btag_deepFlavor.at(i) << endl;
	}
	cout << "------------------------" << endl;
      }

      if (DEBUG)
      {
	cout << "--- VBF jets ---" << endl;
	cout << "isVBF: " << theSmallTree.m_isVBF << endl;
	cout << "VBF1(idx,pt,eta,phi): " << VBFidx1 << " / " << theSmallTree.m_VBFjet1_pt << " / " << theSmallTree.m_VBFjet1_eta << " / " << theSmallTree.m_VBFjet1_phi << endl;
	cout << "VBF2(idx,pt,eta,phi): " << VBFidx2 << " / " << theSmallTree.m_VBFjet2_pt << " / " << theSmallTree.m_VBFjet2_eta << " / " << theSmallTree.m_VBFjet2_phi << endl;
	cout << "----------------" << endl;
      }

      // --------------------------------------------
      // PUjetIDSFprovider
      if (isMC)
      {
        std::vector<float> PUjetID_SF_values = PUjetIDSFprovider.getEvtWeight(theBigTree, tlv_firstLepton, tlv_secondLepton, jets_and_smearFactor);
        theSmallTree.m_PUjetID_SF      = PUjetID_SF_values.at(0);
        theSmallTree.m_PUjetID_SF_up   = PUjetID_SF_values.at(1);
        theSmallTree.m_PUjetID_SF_down = PUjetID_SF_values.at(2);
        theSmallTree.m_PUjetID_SF_eff_up      = PUjetID_SF_values.at(3);
        theSmallTree.m_PUjetID_SF_eff_down    = PUjetID_SF_values.at(4);
        theSmallTree.m_PUjetID_SF_mistag_up   = PUjetID_SF_values.at(5);
        theSmallTree.m_PUjetID_SF_mistag_down = PUjetID_SF_values.at(6);
        theSmallTree.m_PUjetID_SF_eff_eta_s2p5_up      = PUjetID_SF_values.at(7);
        theSmallTree.m_PUjetID_SF_eff_eta_s2p5_down    = PUjetID_SF_values.at(8);
        theSmallTree.m_PUjetID_SF_mistag_eta_s2p5_up   = PUjetID_SF_values.at(9);
        theSmallTree.m_PUjetID_SF_mistag_eta_s2p5_down = PUjetID_SF_values.at(10);
        theSmallTree.m_PUjetID_SF_eff_eta_l2p5_up      = PUjetID_SF_values.at(11);
        theSmallTree.m_PUjetID_SF_eff_eta_l2p5_down    = PUjetID_SF_values.at(12);
        theSmallTree.m_PUjetID_SF_mistag_eta_l2p5_up   = PUjetID_SF_values.at(13);
        theSmallTree.m_PUjetID_SF_mistag_eta_l2p5_down = PUjetID_SF_values.at(14);
        if (DEBUG)
        {
          std::cout << "---- PUjetID_SF debug ----" << std::endl;
          std::cout << "PUjetID_SF     : " << theSmallTree.m_PUjetID_SF << std::endl;
          std::cout << "PUjetID_SF_up  : " << theSmallTree.m_PUjetID_SF_up << std::endl;
          std::cout << "PUjetID_SF_down: " << theSmallTree.m_PUjetID_SF_down << std::endl;
          std::cout << "PUjetID_SF_eff_up     : " << theSmallTree.m_PUjetID_SF_eff_up << std::endl;
          std::cout << "PUjetID_SF_eff_down   : " << theSmallTree.m_PUjetID_SF_eff_down << std::endl;
          std::cout << "PUjetID_SF_mistag_up  : " << theSmallTree.m_PUjetID_SF_mistag_up << std::endl;
          std::cout << "PUjetID_SF_mistag_down: " << theSmallTree.m_PUjetID_SF_mistag_down << std::endl;
          std::cout << "--------------------------" << std::endl;
        }
      }

      // --------------------------------------------
      // Save smear factors in smallTree
      if (doSmearing)
      {
        theSmallTree.m_bjet1_smearFactor   = jets_and_smearFactor[bjet1idx];
        theSmallTree.m_bjet2_smearFactor   = jets_and_smearFactor[bjet2idx];
        theSmallTree.m_VBFjet1_smearFactor = jets_and_smearFactor[VBFidx1];
        theSmallTree.m_VBFjet2_smearFactor = jets_and_smearFactor[VBFidx2];
      }

      // --------------------------------------------
      // Save HHbtag values in smallTree
      // b-jet 1
      if (jets_and_HHbtag.find(bjet1idx) != jets_and_HHbtag.end())
      {
        theSmallTree.m_bjet1_HHbtag = jets_and_HHbtag[bjet1idx];
      }
      else
      {
        std::cout << "**ERROR: HHbtag score not found for bjet1, setting to -1 !!" << endl;
        theSmallTree.m_bjet1_HHbtag = -1.;
      }

      // b-jet 2
      if (jets_and_HHbtag.find(bjet2idx) != jets_and_HHbtag.end())
      {
        theSmallTree.m_bjet2_HHbtag = jets_and_HHbtag[bjet2idx];
      }
      else
      {
        std::cout << "**ERROR: HHbtag score not found for bjet2, setting to -1 !!" << endl;
        theSmallTree.m_bjet2_HHbtag = -1.;
      }

      // VBF-jet 1
      if (jets_and_HHbtag.find(VBFidx1) != jets_and_HHbtag.end())
      {
        theSmallTree.m_VBFjet1_HHbtag = jets_and_HHbtag[VBFidx1];
      }
      else
      {
        // The VBFjets might fall outside eta=2.4 or the event can be non-VBF
        // so they don't have an HHbtag score. In this case
        // the HHbtag score MUST be set manually to -1
        theSmallTree.m_VBFjet1_HHbtag = -1.;
      }

      // VBF-jet 2
      if (jets_and_HHbtag.find(VBFidx2) != jets_and_HHbtag.end())
      {
        theSmallTree.m_VBFjet2_HHbtag = jets_and_HHbtag[VBFidx2];
      }
      else
      {
        // The VBFjets might fall outside eta=2.4 or the event can be non-VBF
        // so they don't have an HHbtag score. In this case
        // the HHbtag score MUST be set manually to -1
        theSmallTree.m_VBFjet2_HHbtag = -1.;
      }

      // Store additional jets (excluding bjets and VBFjets):
      // - 5 central jets that pass b-jet selection and have |eta|<=2.4
      // - 5 forward jets that pass VBF-jet selection and have 2.4<|eta|<=4.7

      // Resize additional jets up/down variations to N_jecSources
      theSmallTree.m_addJetCentr1_pt_jetup    .resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr1_pt_jetdown  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr1_mass_jetup  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr1_mass_jetdown.resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr2_pt_jetup    .resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr2_pt_jetdown  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr2_mass_jetup  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr2_mass_jetdown.resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr3_pt_jetup    .resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr3_pt_jetdown  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr3_mass_jetup  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr3_mass_jetdown.resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr4_pt_jetup    .resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr4_pt_jetdown  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr4_mass_jetup  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr4_mass_jetdown.resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr5_pt_jetup    .resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr5_pt_jetdown  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr5_mass_jetup  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetCentr5_mass_jetdown.resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw1_pt_jetup    .resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw1_pt_jetdown  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw1_mass_jetup  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw1_mass_jetdown.resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw2_pt_jetup    .resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw2_pt_jetdown  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw2_mass_jetup  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw2_mass_jetdown.resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw3_pt_jetup    .resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw3_pt_jetdown  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw3_mass_jetup  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw3_mass_jetdown.resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw4_pt_jetup    .resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw4_pt_jetdown  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw4_mass_jetup  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw4_mass_jetdown.resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw5_pt_jetup    .resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw5_pt_jetdown  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw5_mass_jetup  .resize(N_jecSources,-999.);
      theSmallTree.m_addJetForw5_mass_jetdown.resize(N_jecSources,-999.);

      // Jets are ordered by pT in the theBigTree.jets_px collection
      for (unsigned int iJet = 0; (iJet < theBigTree.jets_px->size ()) && (theSmallTree.m_njets < maxNjetsSaved); ++iJet)
      {
        //PF jet ID
        if (theBigTree.PFjetID->at (iJet) < PFjetID_WP) continue; // 0 ; don't pass PF Jet ID; 1: tight, 2: tightLepVeto

        // Skip H->bb candidates and VBF candidates
        if (int(iJet)==bjet1idx || int(iJet)==bjet2idx || int(iJet)==VBFidx1 || int(iJet)==VBFidx2) continue;

        // build the tlv
        TLorentzVector tlv_additionalJet(theBigTree.jets_px->at(iJet), theBigTree.jets_py->at(iJet), theBigTree.jets_pz->at(iJet), theBigTree.jets_e->at(iJet));
        if (doSmearing) tlv_additionalJet = tlv_additionalJet * jets_and_smearFactor[iJet];

        // Kinematic selections + lepton cleaning
        if (tlv_additionalJet.Pt() < 20.) continue ;
        if (tlv_additionalJet.DeltaR(tlv_firstLepton)  < lepCleaningCone) continue ;
        if (tlv_additionalJet.DeltaR(tlv_secondLepton) < lepCleaningCone) continue ;

        // Apply PUjetID only to jets with pt < 50 GeV (https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorkingLegacyRun2#Jets)
        if (PUjetID_WP > -1) //PU jet ID WP = 2: loose
        {
          if ( !(CheckBit(theBigTree.jets_PUJetIDupdated_WP->at(iJet), PUjetID_WP)) && tlv_additionalJet.Pt()<50.) continue;
        }

        //std::cout << "  -IDX: " << iJet << " --> pt/eta/phi: " << tlv_additionalJet.Pt() <<" "<< tlv_additionalJet.Eta() <<" "<< tlv_additionalJet.Phi() << std::endl;

        // get up/down uncertainty for this additional jet
        pair <vector <double>, vector<double>> unc_additionalJet_updown = JECprovider.getJECUncVectors(iJet, theBigTree);

        // Central jets ( |eta| <= 2.4 ) that pass b-jet selection
        if (TMath::Abs(tlv_additionalJet.Eta()) <= 2.4)
        {
          if (theSmallTree.m_addJetCentr1_pt < 0.)
          {
            theSmallTree.m_addJetCentr1_pt  = tlv_additionalJet.Pt();
            theSmallTree.m_addJetCentr1_eta = tlv_additionalJet.Eta();
            theSmallTree.m_addJetCentr1_phi = tlv_additionalJet.Phi();
            theSmallTree.m_addJetCentr1_e   = tlv_additionalJet.E();
            theSmallTree.m_addJetCentr1_btag_deepFlavor = theBigTree.bDeepFlavor_probb->at(iJet) + theBigTree.bDeepFlavor_probbb->at(iJet) + theBigTree.bDeepFlavor_problepb->at(iJet);
            theSmallTree.m_addJetCentr1_CvsL            = getCvsL(theBigTree, iJet);
            theSmallTree.m_addJetCentr1_CvsB            = getCvsB(theBigTree, iJet);
            if (jets_and_HHbtag.find(iJet) != jets_and_HHbtag.end()) theSmallTree.m_addJetCentr1_HHbtag = jets_and_HHbtag[iJet];
            theSmallTree.m_addJetCentr1_pnet_bb = theBigTree.bParticleNetAK4JetTags_probbb->at(iJet);
            theSmallTree.m_addJetCentr1_pnet_cc = theBigTree.bParticleNetAK4JetTags_probcc->at(iJet);
            theSmallTree.m_addJetCentr1_pnet_b = theBigTree.bParticleNetAK4JetTags_probb->at(iJet);
            theSmallTree.m_addJetCentr1_pnet_c = theBigTree.bParticleNetAK4JetTags_probc->at(iJet);
            theSmallTree.m_addJetCentr1_pnet_g = theBigTree.bParticleNetAK4JetTags_probg->at(iJet);
            theSmallTree.m_addJetCentr1_pnet_uds = theBigTree.bParticleNetAK4JetTags_probuds->at(iJet);
            theSmallTree.m_addJetCentr1_pnet_pu = theBigTree.bParticleNetAK4JetTags_probpu->at(iJet);
            theSmallTree.m_addJetCentr1_pnet_undef = theBigTree.bParticleNetAK4JetTags_probundef->at(iJet);
            theSmallTree.m_addJetCentr1_smearFactor = jets_and_smearFactor[iJet];
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetCentr1_pt_jetup    [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetCentr1_pt_jetdown  [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetCentr1_mass_jetup  [isource] = tlv_additionalJet.M();
              theSmallTree.m_addJetCentr1_mass_jetdown[isource] = tlv_additionalJet.M();
            }

            theSmallTree.m_addJetCentr1_pt_jetupTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetCentr1_pt_jetdownTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetCentr1_mass_jetupTot	= tlv_additionalJet.M();
            theSmallTree.m_addJetCentr1_mass_jetdownTot = tlv_additionalJet.M();
            if(isMC)
            {
	      for (int isource = 0; isource < N_jecSources; isource++)
	      {
		theSmallTree.m_addJetCentr1_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
		theSmallTree.m_addJetCentr1_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
		theSmallTree.m_addJetCentr1_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
		theSmallTree.m_addJetCentr1_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
	      }

	      theSmallTree.m_addJetCentr1_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).Pt();
	      theSmallTree.m_addJetCentr1_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).Pt();
	      theSmallTree.m_addJetCentr1_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).M();
	      theSmallTree.m_addJetCentr1_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).M();
            }
          }
          else if (theSmallTree.m_addJetCentr2_pt < 0.)
          {
            theSmallTree.m_addJetCentr2_pt  = tlv_additionalJet.Pt();
            theSmallTree.m_addJetCentr2_eta = tlv_additionalJet.Eta();
            theSmallTree.m_addJetCentr2_phi = tlv_additionalJet.Phi();
            theSmallTree.m_addJetCentr2_e   = tlv_additionalJet.E();
            theSmallTree.m_addJetCentr2_btag_deepFlavor = theBigTree.bDeepFlavor_probb->at(iJet) + theBigTree.bDeepFlavor_probbb->at(iJet) + theBigTree.bDeepFlavor_problepb->at(iJet);
            theSmallTree.m_addJetCentr2_CvsL            = getCvsL(theBigTree, iJet);
            theSmallTree.m_addJetCentr2_CvsB            = getCvsB(theBigTree, iJet);
            if (jets_and_HHbtag.find(iJet) != jets_and_HHbtag.end()) theSmallTree.m_addJetCentr2_HHbtag = jets_and_HHbtag[iJet];
            theSmallTree.m_addJetCentr2_pnet_bb = theBigTree.bParticleNetAK4JetTags_probbb->at(iJet);
            theSmallTree.m_addJetCentr2_pnet_cc = theBigTree.bParticleNetAK4JetTags_probcc->at(iJet);
            theSmallTree.m_addJetCentr2_pnet_b = theBigTree.bParticleNetAK4JetTags_probb->at(iJet);
            theSmallTree.m_addJetCentr2_pnet_c = theBigTree.bParticleNetAK4JetTags_probc->at(iJet);
            theSmallTree.m_addJetCentr2_pnet_g = theBigTree.bParticleNetAK4JetTags_probg->at(iJet);
            theSmallTree.m_addJetCentr2_pnet_uds = theBigTree.bParticleNetAK4JetTags_probuds->at(iJet);
            theSmallTree.m_addJetCentr2_pnet_pu = theBigTree.bParticleNetAK4JetTags_probpu->at(iJet);
            theSmallTree.m_addJetCentr2_pnet_undef = theBigTree.bParticleNetAK4JetTags_probundef->at(iJet);
            theSmallTree.m_addJetCentr2_smearFactor = jets_and_smearFactor[iJet];
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetCentr2_pt_jetup    [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetCentr2_pt_jetdown  [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetCentr2_mass_jetup  [isource] = tlv_additionalJet.M();
              theSmallTree.m_addJetCentr2_mass_jetdown[isource] = tlv_additionalJet.M();
            }

            theSmallTree.m_addJetCentr2_pt_jetupTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetCentr2_pt_jetdownTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetCentr2_mass_jetupTot	= tlv_additionalJet.M();
            theSmallTree.m_addJetCentr2_mass_jetdownTot = tlv_additionalJet.M();
            if(isMC)
            {
	      for (int isource = 0; isource < N_jecSources; isource++)
	      {
		theSmallTree.m_addJetCentr2_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
		theSmallTree.m_addJetCentr2_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
		theSmallTree.m_addJetCentr2_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
		theSmallTree.m_addJetCentr2_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
	      }

	      theSmallTree.m_addJetCentr2_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).Pt();
	      theSmallTree.m_addJetCentr2_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).Pt();
	      theSmallTree.m_addJetCentr2_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).M();
	      theSmallTree.m_addJetCentr2_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).M();
            }
          }
          else if (theSmallTree.m_addJetCentr3_pt < 0.)
          {
            theSmallTree.m_addJetCentr3_pt  = tlv_additionalJet.Pt();
            theSmallTree.m_addJetCentr3_eta = tlv_additionalJet.Eta();
            theSmallTree.m_addJetCentr3_phi = tlv_additionalJet.Phi();
            theSmallTree.m_addJetCentr3_e   = tlv_additionalJet.E();
            theSmallTree.m_addJetCentr3_btag_deepFlavor = theBigTree.bDeepFlavor_probb->at(iJet) + theBigTree.bDeepFlavor_probbb->at(iJet) + theBigTree.bDeepFlavor_problepb->at(iJet);
            theSmallTree.m_addJetCentr3_CvsL            = getCvsL(theBigTree, iJet);
            theSmallTree.m_addJetCentr3_CvsB            = getCvsB(theBigTree, iJet);
            if (jets_and_HHbtag.find(iJet) != jets_and_HHbtag.end()) theSmallTree.m_addJetCentr3_HHbtag = jets_and_HHbtag[iJet];
            theSmallTree.m_addJetCentr3_pnet_bb = theBigTree.bParticleNetAK4JetTags_probbb->at(iJet);
            theSmallTree.m_addJetCentr3_pnet_cc = theBigTree.bParticleNetAK4JetTags_probcc->at(iJet);
            theSmallTree.m_addJetCentr3_pnet_b = theBigTree.bParticleNetAK4JetTags_probb->at(iJet);
            theSmallTree.m_addJetCentr3_pnet_c = theBigTree.bParticleNetAK4JetTags_probc->at(iJet);
            theSmallTree.m_addJetCentr3_pnet_g = theBigTree.bParticleNetAK4JetTags_probg->at(iJet);
            theSmallTree.m_addJetCentr3_pnet_uds = theBigTree.bParticleNetAK4JetTags_probuds->at(iJet);
            theSmallTree.m_addJetCentr3_pnet_pu = theBigTree.bParticleNetAK4JetTags_probpu->at(iJet);
            theSmallTree.m_addJetCentr3_pnet_undef = theBigTree.bParticleNetAK4JetTags_probundef->at(iJet);
            theSmallTree.m_addJetCentr3_smearFactor = jets_and_smearFactor[iJet];
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetCentr3_pt_jetup    [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetCentr3_pt_jetdown  [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetCentr3_mass_jetup  [isource] = tlv_additionalJet.M();
              theSmallTree.m_addJetCentr3_mass_jetdown[isource] = tlv_additionalJet.M();
            }

            theSmallTree.m_addJetCentr3_pt_jetupTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetCentr3_pt_jetdownTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetCentr3_mass_jetupTot	= tlv_additionalJet.M();
            theSmallTree.m_addJetCentr3_mass_jetdownTot = tlv_additionalJet.M();
            if(isMC)
            {
	      for (int isource = 0; isource < N_jecSources; isource++)
	      {
		theSmallTree.m_addJetCentr3_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
		theSmallTree.m_addJetCentr3_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
		theSmallTree.m_addJetCentr3_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
		theSmallTree.m_addJetCentr3_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
	      }

	      theSmallTree.m_addJetCentr3_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).Pt();
	      theSmallTree.m_addJetCentr3_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).Pt();
	      theSmallTree.m_addJetCentr3_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).M();
	      theSmallTree.m_addJetCentr3_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).M();
            }
          }
          else if (theSmallTree.m_addJetCentr4_pt < 0.)
          {
            theSmallTree.m_addJetCentr4_pt  = tlv_additionalJet.Pt();
            theSmallTree.m_addJetCentr4_eta = tlv_additionalJet.Eta();
            theSmallTree.m_addJetCentr4_phi = tlv_additionalJet.Phi();
            theSmallTree.m_addJetCentr4_e   = tlv_additionalJet.E();
            theSmallTree.m_addJetCentr4_btag_deepFlavor = theBigTree.bDeepFlavor_probb->at(iJet) + theBigTree.bDeepFlavor_probbb->at(iJet) + theBigTree.bDeepFlavor_problepb->at(iJet);
            theSmallTree.m_addJetCentr4_CvsL            = getCvsL(theBigTree, iJet);
            theSmallTree.m_addJetCentr4_CvsB            = getCvsB(theBigTree, iJet);
            if (jets_and_HHbtag.find(iJet) != jets_and_HHbtag.end()) theSmallTree.m_addJetCentr4_HHbtag = jets_and_HHbtag[iJet];
            theSmallTree.m_addJetCentr4_pnet_bb = theBigTree.bParticleNetAK4JetTags_probbb->at(iJet);
            theSmallTree.m_addJetCentr4_pnet_cc = theBigTree.bParticleNetAK4JetTags_probcc->at(iJet);
            theSmallTree.m_addJetCentr4_pnet_b = theBigTree.bParticleNetAK4JetTags_probb->at(iJet);
            theSmallTree.m_addJetCentr4_pnet_c = theBigTree.bParticleNetAK4JetTags_probc->at(iJet);
            theSmallTree.m_addJetCentr4_pnet_g = theBigTree.bParticleNetAK4JetTags_probg->at(iJet);
            theSmallTree.m_addJetCentr4_pnet_uds = theBigTree.bParticleNetAK4JetTags_probuds->at(iJet);
            theSmallTree.m_addJetCentr4_pnet_pu = theBigTree.bParticleNetAK4JetTags_probpu->at(iJet);
            theSmallTree.m_addJetCentr4_pnet_undef = theBigTree.bParticleNetAK4JetTags_probundef->at(iJet);
            theSmallTree.m_addJetCentr4_smearFactor = jets_and_smearFactor[iJet];
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetCentr4_pt_jetup    [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetCentr4_pt_jetdown  [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetCentr4_mass_jetup  [isource] = tlv_additionalJet.M();
              theSmallTree.m_addJetCentr4_mass_jetdown[isource] = tlv_additionalJet.M();
            }

            theSmallTree.m_addJetCentr4_pt_jetupTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetCentr4_pt_jetdownTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetCentr4_mass_jetupTot	= tlv_additionalJet.M();
            theSmallTree.m_addJetCentr4_mass_jetdownTot = tlv_additionalJet.M();
            if(isMC)
            {
	      for (int isource = 0; isource < N_jecSources; isource++)
	      {
		theSmallTree.m_addJetCentr4_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
		theSmallTree.m_addJetCentr4_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
		theSmallTree.m_addJetCentr4_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
		theSmallTree.m_addJetCentr4_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
	      }

	      theSmallTree.m_addJetCentr4_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).Pt();
	      theSmallTree.m_addJetCentr4_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).Pt();
	      theSmallTree.m_addJetCentr4_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).M();
	      theSmallTree.m_addJetCentr4_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).M();
            }
          }
          else if (theSmallTree.m_addJetCentr5_pt < 0.)
          {
            theSmallTree.m_addJetCentr5_pt  = tlv_additionalJet.Pt();
            theSmallTree.m_addJetCentr5_eta = tlv_additionalJet.Eta();
            theSmallTree.m_addJetCentr5_phi = tlv_additionalJet.Phi();
            theSmallTree.m_addJetCentr5_e   = tlv_additionalJet.E();
            theSmallTree.m_addJetCentr5_btag_deepFlavor = theBigTree.bDeepFlavor_probb->at(iJet) + theBigTree.bDeepFlavor_probbb->at(iJet) + theBigTree.bDeepFlavor_problepb->at(iJet);
            theSmallTree.m_addJetCentr5_CvsL            = getCvsL(theBigTree, iJet);
            theSmallTree.m_addJetCentr5_CvsB            = getCvsB(theBigTree, iJet);
            if (jets_and_HHbtag.find(iJet) != jets_and_HHbtag.end()) theSmallTree.m_addJetCentr5_HHbtag = jets_and_HHbtag[iJet];
            theSmallTree.m_addJetCentr5_pnet_bb = theBigTree.bParticleNetAK4JetTags_probbb->at(iJet);
            theSmallTree.m_addJetCentr5_pnet_cc = theBigTree.bParticleNetAK4JetTags_probcc->at(iJet);
            theSmallTree.m_addJetCentr5_pnet_b = theBigTree.bParticleNetAK4JetTags_probb->at(iJet);
            theSmallTree.m_addJetCentr5_pnet_c = theBigTree.bParticleNetAK4JetTags_probc->at(iJet);
            theSmallTree.m_addJetCentr5_pnet_g = theBigTree.bParticleNetAK4JetTags_probg->at(iJet);
            theSmallTree.m_addJetCentr5_pnet_uds = theBigTree.bParticleNetAK4JetTags_probuds->at(iJet);
            theSmallTree.m_addJetCentr5_pnet_pu = theBigTree.bParticleNetAK4JetTags_probpu->at(iJet);
            theSmallTree.m_addJetCentr5_pnet_undef = theBigTree.bParticleNetAK4JetTags_probundef->at(iJet);
            theSmallTree.m_addJetCentr5_smearFactor = jets_and_smearFactor[iJet];
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetCentr5_pt_jetup    [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetCentr5_pt_jetdown  [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetCentr5_mass_jetup  [isource] = tlv_additionalJet.M();
              theSmallTree.m_addJetCentr5_mass_jetdown[isource] = tlv_additionalJet.M();
            }

            theSmallTree.m_addJetCentr5_pt_jetupTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetCentr5_pt_jetdownTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetCentr5_mass_jetupTot	= tlv_additionalJet.M();
            theSmallTree.m_addJetCentr5_mass_jetdownTot = tlv_additionalJet.M();
            if(isMC)
            {
	      for (int isource = 0; isource < N_jecSources; isource++)
	      {
		theSmallTree.m_addJetCentr5_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
		theSmallTree.m_addJetCentr5_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
		theSmallTree.m_addJetCentr5_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
		theSmallTree.m_addJetCentr5_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
	      }

	      theSmallTree.m_addJetCentr5_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).Pt();
	      theSmallTree.m_addJetCentr5_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).Pt();
	      theSmallTree.m_addJetCentr5_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).M();
	      theSmallTree.m_addJetCentr5_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).M();
            }
          }
        }

        // Forward jets ( |eta| > 2.4 ) that pass VBF-jet selection ( pT >= 30. && |eta| <= 4.7 );
        if (TMath::Abs(tlv_additionalJet.Eta()) > 2.4 && TMath::Abs(tlv_additionalJet.Eta()) <= 4.7 && tlv_additionalJet.Pt() >= 30.)
        {
          if (theSmallTree.m_addJetForw1_pt < 0.)
          {
            theSmallTree.m_addJetForw1_pt  = tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw1_eta = tlv_additionalJet.Eta();
            theSmallTree.m_addJetForw1_phi = tlv_additionalJet.Phi();
            theSmallTree.m_addJetForw1_e   = tlv_additionalJet.E();
            theSmallTree.m_addJetForw1_smearFactor = jets_and_smearFactor[iJet];
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetForw1_pt_jetup    [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetForw1_pt_jetdown  [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetForw1_mass_jetup  [isource] = tlv_additionalJet.M();
              theSmallTree.m_addJetForw1_mass_jetdown[isource] = tlv_additionalJet.M();
            }

            theSmallTree.m_addJetForw1_pt_jetupTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw1_pt_jetdownTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw1_mass_jetupTot	= tlv_additionalJet.M();
            theSmallTree.m_addJetForw1_mass_jetdownTot = tlv_additionalJet.M();
            if(isMC)
            {
	      for (int isource = 0; isource < N_jecSources; isource++)
	      {
		theSmallTree.m_addJetForw1_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
		theSmallTree.m_addJetForw1_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
		theSmallTree.m_addJetForw1_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
		theSmallTree.m_addJetForw1_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
	      }

	      theSmallTree.m_addJetForw1_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).Pt();
	      theSmallTree.m_addJetForw1_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).Pt();
	      theSmallTree.m_addJetForw1_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).M();
	      theSmallTree.m_addJetForw1_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).M();
            }
          }
          else if (theSmallTree.m_addJetForw2_pt < 0.)
          {
            theSmallTree.m_addJetForw2_pt  = tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw2_eta = tlv_additionalJet.Eta();
            theSmallTree.m_addJetForw2_phi = tlv_additionalJet.Phi();
            theSmallTree.m_addJetForw2_e   = tlv_additionalJet.E();
            theSmallTree.m_addJetForw2_smearFactor = jets_and_smearFactor[iJet];
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetForw2_pt_jetup    [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetForw2_pt_jetdown  [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetForw2_mass_jetup  [isource] = tlv_additionalJet.M();
              theSmallTree.m_addJetForw2_mass_jetdown[isource] = tlv_additionalJet.M();
            }

            theSmallTree.m_addJetForw2_pt_jetupTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw2_pt_jetdownTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw2_mass_jetupTot	= tlv_additionalJet.M();
            theSmallTree.m_addJetForw2_mass_jetdownTot = tlv_additionalJet.M();
            if(isMC)
            {
	      for (int isource = 0; isource < N_jecSources; isource++)
	      {
		theSmallTree.m_addJetForw2_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
		theSmallTree.m_addJetForw2_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
		theSmallTree.m_addJetForw2_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
		theSmallTree.m_addJetForw2_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
	      }

	      theSmallTree.m_addJetForw2_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).Pt();
	      theSmallTree.m_addJetForw2_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).Pt();
	      theSmallTree.m_addJetForw2_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).M();
	      theSmallTree.m_addJetForw2_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).M();
            }
          }
          else if (theSmallTree.m_addJetForw3_pt < 0.)
          {
            theSmallTree.m_addJetForw3_pt  = tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw3_eta = tlv_additionalJet.Eta();
            theSmallTree.m_addJetForw3_phi = tlv_additionalJet.Phi();
            theSmallTree.m_addJetForw3_e   = tlv_additionalJet.E();
            theSmallTree.m_addJetForw3_smearFactor = jets_and_smearFactor[iJet];
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetForw3_pt_jetup    [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetForw3_pt_jetdown  [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetForw3_mass_jetup  [isource] = tlv_additionalJet.M();
              theSmallTree.m_addJetForw3_mass_jetdown[isource] = tlv_additionalJet.M();
            }

            theSmallTree.m_addJetForw3_pt_jetupTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw3_pt_jetdownTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw3_mass_jetupTot	= tlv_additionalJet.M();
            theSmallTree.m_addJetForw3_mass_jetdownTot = tlv_additionalJet.M();
            if(isMC)
            {
	      for (int isource = 0; isource < N_jecSources; isource++)
	      {
		theSmallTree.m_addJetForw3_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
		theSmallTree.m_addJetForw3_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
		theSmallTree.m_addJetForw3_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
		theSmallTree.m_addJetForw3_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
	      }

	      theSmallTree.m_addJetForw3_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).Pt();
	      theSmallTree.m_addJetForw3_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).Pt();
	      theSmallTree.m_addJetForw3_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).M();
	      theSmallTree.m_addJetForw3_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).M();
            }
          }
          else if (theSmallTree.m_addJetForw4_pt < 0.)
          {
            theSmallTree.m_addJetForw4_pt  = tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw4_eta = tlv_additionalJet.Eta();
            theSmallTree.m_addJetForw4_phi = tlv_additionalJet.Phi();
            theSmallTree.m_addJetForw4_e   = tlv_additionalJet.E();
            theSmallTree.m_addJetForw4_smearFactor = jets_and_smearFactor[iJet];
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetForw4_pt_jetup    [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetForw4_pt_jetdown  [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetForw4_mass_jetup  [isource] = tlv_additionalJet.M();
              theSmallTree.m_addJetForw4_mass_jetdown[isource] = tlv_additionalJet.M();
            }

            theSmallTree.m_addJetForw4_pt_jetupTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw4_pt_jetdownTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw4_mass_jetupTot	= tlv_additionalJet.M();
            theSmallTree.m_addJetForw4_mass_jetdownTot = tlv_additionalJet.M();
            if(isMC)
            {
	      for (int isource = 0; isource < N_jecSources; isource++)
	      {
		theSmallTree.m_addJetForw4_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
		theSmallTree.m_addJetForw4_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
		theSmallTree.m_addJetForw4_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
		theSmallTree.m_addJetForw4_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
	      }

	      theSmallTree.m_addJetForw4_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).Pt();
	      theSmallTree.m_addJetForw4_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).Pt();
	      theSmallTree.m_addJetForw4_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).M();
	      theSmallTree.m_addJetForw4_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).M();
            }
          }
          else if (theSmallTree.m_addJetForw5_pt < 0.)
          {
            theSmallTree.m_addJetForw5_pt  = tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw5_eta = tlv_additionalJet.Eta();
            theSmallTree.m_addJetForw5_phi = tlv_additionalJet.Phi();
            theSmallTree.m_addJetForw5_e   = tlv_additionalJet.E();
            theSmallTree.m_addJetForw5_smearFactor = jets_and_smearFactor[iJet];
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetForw5_pt_jetup    [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetForw5_pt_jetdown  [isource] = tlv_additionalJet.Pt();
              theSmallTree.m_addJetForw5_mass_jetup  [isource] = tlv_additionalJet.M();
              theSmallTree.m_addJetForw5_mass_jetdown[isource] = tlv_additionalJet.M();
            }

            theSmallTree.m_addJetForw5_pt_jetupTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw5_pt_jetdownTot	= tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw5_mass_jetupTot	= tlv_additionalJet.M();
            theSmallTree.m_addJetForw5_mass_jetdownTot = tlv_additionalJet.M();
            if(isMC)
            {
	      for (int isource = 0; isource < N_jecSources; isource++)
	      {
		theSmallTree.m_addJetForw5_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
		theSmallTree.m_addJetForw5_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
		theSmallTree.m_addJetForw5_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
		theSmallTree.m_addJetForw5_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
	      }

	      theSmallTree.m_addJetForw5_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).Pt();
	      theSmallTree.m_addJetForw5_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).Pt();
	      theSmallTree.m_addJetForw5_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[N_jecSources])).M();
	      theSmallTree.m_addJetForw5_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[N_jecSources])).M();
            }
          }
        }
      } // end loop on additional jets

      if (DEBUG)
      {
        std::cout << "---------- Debugging Additional Jets ----------" << std::endl;
        std::cout << " - c1 pT: " << theSmallTree.m_addJetCentr1_pt<<std::endl;
        std::cout << " - c2 pT: " << theSmallTree.m_addJetCentr2_pt<<std::endl;
        std::cout << " - c3 pT: " << theSmallTree.m_addJetCentr3_pt<<std::endl;
        std::cout << " - c4 pT: " << theSmallTree.m_addJetCentr4_pt<<std::endl;
        std::cout << " - c5 pT: " << theSmallTree.m_addJetCentr5_pt<<std::endl;
        std::cout << " - f1 pT: " << theSmallTree.m_addJetForw1_pt<<std::endl;
        std::cout << " - f2 pT: " << theSmallTree.m_addJetForw2_pt<<std::endl;
        std::cout << " - f3 pT: " << theSmallTree.m_addJetForw3_pt<<std::endl;
        std::cout << " - f4 pT: " << theSmallTree.m_addJetForw4_pt<<std::endl;
        std::cout << " - f5 pT: " << theSmallTree.m_addJetForw5_pt<<std::endl;
        std::cout << "-----------------------------------------------" << std::endl;
      }

      //Other jets
      for (unsigned int iJet = 0; (iJet < theBigTree.jets_px->size ()) && (theSmallTree.m_njets < maxNjetsSaved); ++iJet)
      {
        // PG filter jets at will
        if (theBigTree.PFjetID->at (iJet) < PFjetID_WP) continue; // 0 ; don't pass PF Jet ID; 1: tight, 2: tightLepVeto

        // skip the H decay candiates
        if (int (iJet) == bjet1idx) continue;
        if (int (iJet) == bjet2idx) continue ;

        TLorentzVector tlv_dummyJet(theBigTree.jets_px->at(iJet), theBigTree.jets_py->at(iJet), theBigTree.jets_pz->at(iJet), theBigTree.jets_e->at(iJet));
        if (doSmearing) tlv_dummyJet = tlv_dummyJet * jets_and_smearFactor[iJet];

        // Apply PUjetID only to jets with pt < 50 GeV ( https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorkingLegacyRun2#Jets )
        // PU jet ID WP = 2: loose
        if (PUjetID_WP > -1)
        {
          if ( !(CheckBit(theBigTree.jets_PUJetIDupdated_WP->at(iJet), PUjetID_WP)) && tlv_dummyJet.Pt()<50.) continue;
        }

        // remove jets that overlap with the tau selected in the leg 1 and 2
        if (tlv_firstLepton.DeltaR(tlv_dummyJet)  < lepCleaningCone) continue;
        if (tlv_secondLepton.DeltaR(tlv_dummyJet) < lepCleaningCone) continue;

        if (jets_and_HHbtag.find(iJet) != jets_and_HHbtag.end())
        {
          theSmallTree.m_jets_HHbtag.push_back(jets_and_HHbtag[iJet]);
        }
        else
        {
          if(DEBUG)
          {
            std::cout << "**WARNING: HHbtag score not found for jet " << iJet << " , setting to -1 !!" << endl;
          }
          theSmallTree.m_jets_HHbtag.push_back(-1.);
        }

        theSmallTree.m_jets_smearFactor.push_back(jets_and_smearFactor[iJet]);
      }

      if (DEBUG)
      {
        std::cout << "---- HHbtag debug ----" << std::endl;
        std::cout << "isVBF: " << theSmallTree.m_isVBF << "  - Evt: " << theSmallTree.m_EventNumber << std::endl;
        std::cout << "HHbtag scores: ";
        for(auto elem : jets_and_HHbtag)  std::cout << " " << elem.first << ":" << elem.second;
        std::cout << std::endl;
        std::cout << "b1   - idx: " << bjet1idx << " HHbtag: " << theSmallTree.m_bjet1_HHbtag << std::endl;
        std::cout << "b2   - idx: " << bjet2idx << " HHbtag: " << theSmallTree.m_bjet2_HHbtag << std::endl;
        std::cout << "VBF1 - idx: " << VBFidx1  << " HHbtag: " << theSmallTree.m_VBFjet1_HHbtag << std::endl;
        std::cout << "VBF2 - idx: " << VBFidx2  << " HHbtag: " << theSmallTree.m_VBFjet2_HHbtag << std::endl;
        std::cout << "----------------------" << std::endl;
      }

      // --------------------------------------------
      // Boosted section
      theSmallTree.m_isBoosted = 0;
      if (theBigTree.ak8jets_px->size() > 0)
      {
	// int idxSub1 = -1;
	// int idxSub2 = -1;
	// int idxFatj = -1;

	vector<pair<float, int>> fatjets_bTag;
	for (unsigned int ifj = 0; ifj < theBigTree.ak8jets_px->size(); ++ifj)
	{
	  TLorentzVector tlv_fj (theBigTree.ak8jets_px->at(ifj) , theBigTree.ak8jets_py->at(ifj) , theBigTree.ak8jets_pz->at(ifj) , theBigTree.ak8jets_e->at(ifj));
	  if (theBigTree.ak8jets_SoftDropMass -> at(ifj) < 30) continue;
	  if ( theBigTree.ak8jets_nsubjets->at(ifj) < 2 ) continue;

	  TLorentzVector tlv_subj1;
	  TLorentzVector tlv_subj2;
	  vector<int> sjIdxs = findSubjetIdxs(ifj, theBigTree);

	  int nSJ = 0;
	  for (int isj : sjIdxs)
	  {
	    ++nSJ;
	    if (nSJ > 2) break; // FIXME: storing first two <--> highest pt, order subjets for b tag?
	    if (nSJ == 1)
	    {
	      tlv_subj1.SetPxPyPzE (theBigTree.subjets_px->at(isj), theBigTree.subjets_py->at(isj), theBigTree.subjets_pz->at(isj), theBigTree.subjets_e->at(isj));
	    }
	    if (nSJ == 2)
	    {
	      tlv_subj2.SetPxPyPzE (theBigTree.subjets_px->at(isj), theBigTree.subjets_py->at(isj), theBigTree.subjets_pz->at(isj), theBigTree.subjets_e->at(isj));
	    }

	    if(DEBUG)
	    {
	      cout << "- nSJ=" << nSJ << " px=" << theBigTree.subjets_px->at(isj) << endl;
	    }

	  }

	  bool A1B2 = (tlv_subj1.DeltaR(tlv_firstBjet) < 0.4)   && (tlv_subj2.DeltaR(tlv_secondBjet) < 0.4 );
	  bool A2B1 = (tlv_subj1.DeltaR(tlv_secondBjet) < 0.4)  && (tlv_subj2.DeltaR(tlv_firstBjet) < 0.4 );

	  if(DEBUG)
	  {
	    cout << " fatjet: idx " << ifj << " nsj=" << sjIdxs.size()
		 << " sj1pt=" << tlv_subj1.Pt() << " sj1eta=" << tlv_subj1.Eta() << " sj1phi=" << tlv_subj1.Phi()
		 << " sj2pt=" << tlv_subj2.Pt() << " sj2eta=" << tlv_subj2.Eta() << " sj2phi=" << tlv_subj2.Phi()
		 << " !passMatch=" << (!A1B2 && !A2B1) << endl;
	  }

	  if (!A1B2 && !A2B1) continue; // is not matched to resolved jets

	  //fatjets_bTag.push_back(make_pair(theBigTree.ak8jets_CSV->size(), ifj));
	  if(useDeepFlavor)
	    fatjets_bTag.push_back(make_pair(theBigTree.ak8jets_deepFlavor_probb->at(ifj)+theBigTree.ak8jets_deepFlavor_probbb->at(ifj)+theBigTree.ak8jets_deepFlavor_problepb->at(ifj), ifj));
	  else
	    fatjets_bTag.push_back(make_pair(theBigTree.ak8jets_deepCSV_probb->at(ifj)+theBigTree.ak8jets_deepCSV_probbb->at(ifj), ifj));
	}

	if(DEBUG)
	{
	  cout << " N selected fatjets : " << fatjets_bTag.size() << endl;
	}

	if (fatjets_bTag.size() != 0)
	{
	  theSmallTree.m_isBoosted = 1;
	  sort (fatjets_bTag.begin(), fatjets_bTag.end());
	  int fjIdx = fatjets_bTag.back().second;
	  TLorentzVector tlv_fj (theBigTree.ak8jets_px->at(fjIdx) , theBigTree.ak8jets_py->at(fjIdx) , theBigTree.ak8jets_pz->at(fjIdx) , theBigTree.ak8jets_e->at(fjIdx));
	  theSmallTree.m_fatjet_pt   = tlv_fj.Pt();
	  theSmallTree.m_fatjet_eta  = tlv_fj.Eta();
	  theSmallTree.m_fatjet_phi  = tlv_fj.Phi();
	  theSmallTree.m_fatjet_e    = tlv_fj.E();
	  theSmallTree.m_fatjet_bID  = theBigTree.ak8jets_CSV->at(fjIdx);
	  theSmallTree.m_fatjet_bID_deepCSV  = theBigTree.ak8jets_deepCSV_probb->at(fjIdx) + theBigTree.ak8jets_deepCSV_probbb->at(fjIdx);
	  theSmallTree.m_fatjet_bID_deepFlavor  = theBigTree.ak8jets_deepFlavor_probb->at(fjIdx) + theBigTree.ak8jets_deepFlavor_probbb->at(fjIdx) + theBigTree.ak8jets_deepFlavor_problepb->at(fjIdx);
	  theSmallTree.m_fatjet_filteredMass = theBigTree.ak8jets_FilteredMass -> at(fjIdx) ;
	  theSmallTree.m_fatjet_prunedMass   = theBigTree.ak8jets_PrunedMass   -> at(fjIdx) ;
	  theSmallTree.m_fatjet_trimmedMass  = theBigTree.ak8jets_TrimmedMass  -> at(fjIdx) ;
	  theSmallTree.m_fatjet_softdropMass = theBigTree.ak8jets_SoftDropMass -> at(fjIdx) ;
	  theSmallTree.m_fatjet_tau1 = theBigTree.ak8jets_tau1->at(fjIdx);
	  theSmallTree.m_fatjet_tau2 = theBigTree.ak8jets_tau2->at(fjIdx);
	  theSmallTree.m_fatjet_tau3 = theBigTree.ak8jets_tau3->at(fjIdx);
	  theSmallTree.m_fatjet_nsubjets = theBigTree.ak8jets_nsubjets->at(fjIdx);
	  // FIXME: redoing this a second time, can be optimized
	  if ( theBigTree.ak8jets_nsubjets->at(fjIdx) < 2) cout << "ERROR: there are not 2 subjets. Should not happen!!" << endl;
	  TLorentzVector tlv_subj1;
	  TLorentzVector tlv_subj2;
	  vector<int> sjIdxs = findSubjetIdxs(fjIdx, theBigTree);
	  int nSJ = 0;
	  for (int isj : sjIdxs)
	  {
	    ++nSJ;
	    if (nSJ > 2) break; // FIXME: storing first two <--> highest pt, order subjets for b tag?
	    if (nSJ == 1)
	    {
	      tlv_subj1.SetPxPyPzE (theBigTree.subjets_px->at(isj), theBigTree.subjets_py->at(isj), theBigTree.subjets_pz->at(isj), theBigTree.subjets_e->at(isj));
	      theSmallTree.m_subjetjet1_pt   = tlv_subj1.Pt();
	      theSmallTree.m_subjetjet1_eta  = tlv_subj1.Eta();
	      theSmallTree.m_subjetjet1_phi  = tlv_subj1.Phi();
	      theSmallTree.m_subjetjet1_e    = tlv_subj1.E();
	      theSmallTree.m_subjetjet1_bID  = theBigTree.subjets_CSV->at(isj) ;
	      theSmallTree.m_subjetjet1_bID_deepCSV  = theBigTree.subjets_deepCSV_probb->at(isj) + theBigTree.subjets_deepCSV_probbb->at(isj) ;
	      theSmallTree.m_subjetjet1_bID_deepFlavor  = theBigTree.subjets_deepFlavor_probb->at(isj) + theBigTree.subjets_deepFlavor_probbb->at(isj) + theBigTree.subjets_deepFlavor_problepb->at(isj) ;
	    }
	    if (nSJ == 2)
	    {
	      tlv_subj2.SetPxPyPzE (theBigTree.subjets_px->at(isj), theBigTree.subjets_py->at(isj), theBigTree.subjets_pz->at(isj), theBigTree.subjets_e->at(isj));
	      theSmallTree.m_subjetjet2_pt   = tlv_subj2.Pt();
	      theSmallTree.m_subjetjet2_eta  = tlv_subj2.Eta();
	      theSmallTree.m_subjetjet2_phi  = tlv_subj2.Phi();
	      theSmallTree.m_subjetjet2_e    = tlv_subj2.E();
	      theSmallTree.m_subjetjet2_bID  = theBigTree.subjets_CSV->at(isj) ;
	      theSmallTree.m_subjetjet2_bID_deepCSV  = theBigTree.subjets_deepCSV_probb->at(isj) + theBigTree.subjets_deepCSV_probbb->at(isj) ;
	      theSmallTree.m_subjetjet2_bID_deepFlavor  = theBigTree.subjets_deepFlavor_probb->at(isj) + theBigTree.subjets_deepFlavor_probbb->at(isj) + theBigTree.subjets_deepFlavor_problepb->at(isj);
	    }
	    theSmallTree.m_dR_subj1_subj2 = tlv_subj1.DeltaR(tlv_subj2);
	  }
	}
      }
    }// if there's two jets in the event, at least

    if (isMC) selectedEvents += theBigTree.aMCatNLOweight ;  //FIXME: probably wrong, but unused up to now
    else selectedEvents += 1 ;
    ++selectedNoWeightsEventsNum ;

    if (DEBUG)
    {
      cout << "--------------			" << endl;
	  cout << " - Debug SFs -			" << endl;
	  cout << "	PU			 :			" << theSmallTree.m_PUReweight << endl;
	  cout << "	IDandISO deep (DM):		" << theSmallTree.m_IdSF_deep_dm << endl;
	  cout << "	  w/ FakeRate:			" << theSmallTree.m_IdFakeSF_deep_dm << endl;
	  cout << "	IDandISO deep (pT):		" << theSmallTree.m_IdSF_deep_pt << endl;
	  cout << "	  w/ FakeRate:			" << theSmallTree.m_IdFakeSF_deep_pt << endl;
	  cout << "	IDandISO deep (2D):		" << theSmallTree.m_IdSF_deep_2d << endl;
	  cout << "	  w/ FakeRate:			" << theSmallTree.m_IdFakeSF_deep_2d << endl;
	  cout << "	trig		 :			" << theSmallTree.m_trigSF << endl;
	  cout << "	bTag		 :			" << theSmallTree.m_bTagweightM << endl;
	  cout << "	prescale	 :			" << theSmallTree.m_prescaleWeight<< endl;
	  cout << "	prefiring	 :			" << theSmallTree.m_L1pref_weight<< endl;
	  cout << "--------------			" << endl;
      cout << "--- DEBUG MC weights ---	" << endl;
      cout << "aMCatNLOweight:			" << theBigTree.aMCatNLOweight << endl;
      cout << "XS            :			" << XS << endl;
      cout << "stitchWeight  :			" << stitchWeight << endl;
      cout << "HHweight      :			" << HHweight << endl;
      cout << "MC_weight     :			" << theSmallTree.m_MC_weight << endl;
      cout << "Yield weight deep (DM):	" << theSmallTree.m_MC_weight * theSmallTree.m_PUReweight * theSmallTree.m_IdFakeSF_deep_dm * theSmallTree.m_trigSF << endl;
	  cout << "Yield weight deep (pT):	" << theSmallTree.m_MC_weight * theSmallTree.m_PUReweight * theSmallTree.m_IdFakeSF_deep_pt * theSmallTree.m_trigSF << endl;
	  cout << "Yield weight deep (2D):	" << theSmallTree.m_MC_weight * theSmallTree.m_PUReweight * theSmallTree.m_IdFakeSF_deep_2d * theSmallTree.m_trigSF << endl;
      cout << "------------------------	" << endl;
      cout << "--- FINAL DEBUG ---		" << endl;
      cout << "nbjetscand:				" << theSmallTree.m_nbjetscand << endl;
      cout << "-------------------		" << endl;
    }

    theSmallTree.Fill () ;
  }

  cout << "1: " << totalEvents << endl ;
  cout << "2: " << selectedEvents << endl ;
  cout << "3: " << totalNoWeightsEventsNum << endl ;
  cout << "4: " << selectedNoWeightsEventsNum << endl ;

  if (totalEvents != 0) cout << "efficiency = " << selectedEvents / totalEvents << endl ;
  else                  cout << "NO events found\n" ;
  TH1F h_eff ("h_eff", "h_eff", 4 , 0, 4) ;
  h_eff.SetBinContent (1, totalEvents) ;
  h_eff.SetBinContent (2, selectedEvents) ;
  h_eff.SetBinContent (3, totalNoWeightsEventsNum) ;
  h_eff.SetBinContent (4, selectedNoWeightsEventsNum) ;

  // store more detailed eff counter in output
  vector<pair<string, double> > vEffSumm = ec.GetSummary();
  TH1F* h_effSummary = new TH1F ("h_effSummary", "h_effSummary", vEffSumm.size(), 0, vEffSumm.size());
  for (uint isumm = 0; isumm < vEffSumm.size(); ++isumm)
  {
    h_effSummary->SetBinContent(isumm+1, vEffSumm.at(isumm).second);
    h_effSummary->GetXaxis()->SetBinLabel(isumm+1, vEffSumm.at(isumm).first.c_str());
  }

  TH1F h_syst ("h_syst", "h_syst", 3 , 0, 3) ; //systematics
  h_syst.SetBinContent (1, N_jecSources) ;
  h_syst.SetBinContent (2, N_tauhDM) ;
  h_syst.SetBinContent (3, N_tauhDM_EES) ;
  h_syst.GetXaxis()->SetBinLabel(1, "jec unc sources");
  h_syst.GetXaxis()->SetBinLabel(2, "tauh decay modes for TES");
  h_syst.GetXaxis()->SetBinLabel(3, "tauh decay modes for EES");

  TH1F* hEffHHSigsSummary [6];
  if (isHHsignal)
  {
    std::vector<string> vNames = {
      "MuTau",
      "ETau",
      "TauTau",
      "MuMu",
      "EE",
      "EMu"
    };


    for (uint ich = 0; ich < 6; ++ich)
    {
      string hname = string("h_effSummary_") + vNames.at(ich);
      vector<pair<string, double> > vEffSummHH = ecHHsig[ich].GetSummary();
      hEffHHSigsSummary[ich] = new TH1F (hname.c_str(), hname.c_str(), vEffSummHH.size(), 0, vEffSummHH.size());
      for (uint isumm = 0; isumm < vEffSummHH.size(); ++isumm)
      {
	hEffHHSigsSummary[ich]->SetBinContent(isumm+1, vEffSummHH.at(isumm).second);
	hEffHHSigsSummary[ich]->GetXaxis()->SetBinLabel(isumm+1, vEffSummHH.at(isumm).first.c_str());
      }
    }

  }

  // for (unsigned int i = 0 ; i < counter.size () ; ++i)
  //   h_eff.SetBinContent (5 + i, counter.at (i)) ;

  smallFile->cd() ;
  h_eff.Write () ;
  h_effSummary->Write() ;
  h_syst.Write();
  if (isHHsignal)
  {
    for (uint ich = 0; ich < 6; ++ich)
      hEffHHSigsSummary[ich]->Write();
  }

  smallFile->Write () ;
  smallFile->Close () ;

  // free memory used by histos for eff
  delete h_effSummary;

  if (isHHsignal)
  {
    for (uint ich = 0; ich < 6; ++ich)
      delete hEffHHSigsSummary[ich];
  }



  bool computeMVA    = (gConfigParser->isDefined("TMVA::computeMVA")        ? gConfigParser->readBoolOption ("TMVA::computeMVA")        : false);
  bool computeMVARes = (gConfigParser->isDefined("BDTResonant::computeMVA") ? gConfigParser->readBoolOption ("BDTResonant::computeMVA") : false);
  bool computeMVAResHM = (gConfigParser->isDefined("BDTResonantHM::computeMVA") ? gConfigParser->readBoolOption ("BDTResonantHM::computeMVA") : false);
  bool computeMVAResLM = (gConfigParser->isDefined("BDTResonantLM::computeMVA") ? gConfigParser->readBoolOption ("BDTResonantLM::computeMVA") : false);
  bool computeMVANonRes = (gConfigParser->isDefined("BDTNonResonant::computeMVA") ? gConfigParser->readBoolOption ("BDTNonResonant::computeMVA") : false);

  if (computeMVA || computeMVARes || computeMVAResHM || computeMVAResLM)
  {
    bool doMuTau  = gConfigParser->isDefined("TMVA::weightsMuTau");
    bool doETau   = gConfigParser->isDefined("TMVA::weightsETau");
    bool doTauTau = gConfigParser->isDefined("TMVA::weightsTauTau");
    bool doLepTau = gConfigParser->isDefined("TMVA::weightsLepTau");
    bool doResonant = computeMVARes;
    bool doResonantHM = computeMVAResHM;
    bool doResonantLM = computeMVAResLM;
    bool doNonResonant = computeMVANonRes;

    string TMVAweightsTauTau   = "";
    string TMVAweightsMuTau    = "";
    string TMVAweightsETau     = "";
    string TMVAweightsLepTau   = "";
    string TMVAweightsResonant = "";
    string TMVAweightsResonantHM = "";
    string TMVAweightsResonantLM = "";
    string TMVAweightsNonResonant = "";

    if (doMuTau)    TMVAweightsMuTau  = gConfigParser->readStringOption ("TMVA::weightsMuTau");
    if (doETau)     TMVAweightsETau   = gConfigParser->readStringOption ("TMVA::weightsETau");
    if (doTauTau)   TMVAweightsTauTau = gConfigParser->readStringOption ("TMVA::weightsTauTau");
    if (doLepTau)   TMVAweightsLepTau = gConfigParser->readStringOption ("TMVA::weightsLepTau");
    if (doResonant) TMVAweightsResonant = gConfigParser->readStringOption ("BDTResonant::weights");
    if (doResonantHM) TMVAweightsResonantHM = gConfigParser->readStringOption ("BDTResonantHM::weights");
    if (doResonantLM) TMVAweightsResonantLM = gConfigParser->readStringOption ("BDTResonantLM::weights");
    if (doNonResonant) TMVAweightsNonResonant = gConfigParser->readStringOption ("BDTNonResonant::weights");

    // bool TMVAspectatorsIn      = gConfigParser->readBoolOption   ("TMVA::spectatorsPresent");
    vector<string> TMVAspectators = ( computeMVA ? gConfigParser->readStringListOption   ("TMVA::spectators") : vector<string>(0) );
    vector<string> TMVAvariables  = ( computeMVA ? gConfigParser->readStringListOption   ("TMVA::variables") : vector<string>(0) );
    vector<string> TMVAvariablesResonant   = ( doResonant ? gConfigParser->readStringListOption   ("BDTResonant::variables") : vector<string>(0) );
    vector<string> TMVAvariablesResonantHM = ( doResonantHM ? gConfigParser->readStringListOption   ("BDTResonantHM::variables") : vector<string>(0) );
    vector<string> TMVAvariablesResonantLM = ( doResonantLM ? gConfigParser->readStringListOption   ("BDTResonantLM::variables") : vector<string>(0) );
    vector<string> TMVAvariablesNonResonant = ( doNonResonant ? gConfigParser->readStringListOption   ("BDTNonResonant::variables") : vector<string>(0) );

    // split the resonant name in two strings
    vector<pair<string, string>> splitTMVAvariablesResonant;
    for (unsigned int iv = 0 ; iv < TMVAvariablesResonant.size () ; ++iv)
    {
      // split my_name:BDT_name in two strings
      std::stringstream packedName(TMVAvariablesResonant.at(iv));
      std::string segment;
      std::vector<std::string> unpackedNames;
      while(std::getline(packedName, segment, ':'))
	unpackedNames.push_back(segment);

      splitTMVAvariablesResonant.push_back(make_pair(unpackedNames.at(0), unpackedNames.at(1)));
    }

    // split the resonant name in two strings
    cout << "BDT resonant HIGH MASS vars:" << endl;
    vector<pair<string, string>> splitTMVAvariablesResonantHM;
    for (unsigned int iv = 0 ; iv < TMVAvariablesResonantHM.size () ; ++iv)
    {
      // split my_name:BDT_name in two strings
      std::stringstream packedName(TMVAvariablesResonantHM.at(iv));
      std::string segment;
      std::vector<std::string> unpackedNames;
      while(std::getline(packedName, segment, ':'))
	unpackedNames.push_back(segment);

      // replace "internal" names for graphics names -- shitty parser!!
      boost::replace_all(unpackedNames.at(1), "_T_", "*");
      boost::replace_all(unpackedNames.at(1), "__", "()");

      splitTMVAvariablesResonantHM.push_back(make_pair(unpackedNames.at(0), unpackedNames.at(1)));
      cout << " ... " << iv << " " << unpackedNames.at(0) << " --> " << unpackedNames.at(1) << endl;
    }
    cout << endl;

    // split the resonant name in two strings
    vector<pair<string, string>> splitTMVAvariablesResonantLM;
    cout << "BDT resonant LOW MASS vars:" << endl;
    for (unsigned int iv = 0 ; iv < TMVAvariablesResonantLM.size () ; ++iv)
    {
      // split my_name:BDT_name in two strings
      std::stringstream packedName(TMVAvariablesResonantLM.at(iv));
      std::string segment;
      std::vector<std::string> unpackedNames;
      while(std::getline(packedName, segment, ':'))
	unpackedNames.push_back(segment);

      // replace "internal" names for graphics names -- shitty parser!!
      boost::replace_all(unpackedNames.at(1), "_T_", "*");
      boost::replace_all(unpackedNames.at(1), "__", "()");

      splitTMVAvariablesResonantLM.push_back(make_pair(unpackedNames.at(0), unpackedNames.at(1)));
      cout << " ... " << iv << " " << unpackedNames.at(0) << " --> " << unpackedNames.at(1) << endl;
    }

    // split the non resonant name in two strings
    vector<pair<string, string>> splitTMVAvariablesNonResonant;
    cout << "BDT non resonant vars:" << endl;
    for (unsigned int iv = 0 ; iv < TMVAvariablesNonResonant.size () ; ++iv)
    {
      // split my_name:BDT_name in two strings
      std::stringstream packedName(TMVAvariablesNonResonant.at(iv));
      std::string segment;
      std::vector<std::string> unpackedNames;
      while(std::getline(packedName, segment, ':'))
	unpackedNames.push_back(segment);

      // replace "internal" names for graphics names -- shitty parser!!
      boost::replace_all(unpackedNames.at(1), "_T_", "*");
      boost::replace_all(unpackedNames.at(1), "__", "()");

      splitTMVAvariablesNonResonant.push_back(make_pair(unpackedNames.at(0), unpackedNames.at(1)));
      cout << " ... " << iv << " " << unpackedNames.at(0) << " --> " << unpackedNames.at(1) << endl;
    }


    // now merge all names into a vector to get a list of uniquely needed elements
    std::vector<string> allVars;
    allVars.insert(allVars.end(), TMVAspectators.begin(), TMVAspectators.end());
    allVars.insert(allVars.end(), TMVAvariables.begin(), TMVAvariables.end());
    for (unsigned int iv = 0; iv < splitTMVAvariablesResonant.size(); ++iv)
      allVars.push_back(splitTMVAvariablesResonant.at(iv).first);
    for (unsigned int iv = 0; iv < splitTMVAvariablesResonantHM.size(); ++iv)
      allVars.push_back(splitTMVAvariablesResonantHM.at(iv).first);
    for (unsigned int iv = 0; iv < splitTMVAvariablesResonantLM.size(); ++iv)
      allVars.push_back(splitTMVAvariablesResonantLM.at(iv).first);
    for (unsigned int iv = 0; iv < splitTMVAvariablesNonResonant.size(); ++iv)
      allVars.push_back(splitTMVAvariablesNonResonant.at(iv).first);

    sort(allVars.begin(), allVars.end());
    allVars.erase( unique( allVars.begin(), allVars.end() ), allVars.end() );
    std::map<string, float> allVarsMap;
    for (string var : allVars)
      allVarsMap[var] = 0.0;

    TFile *outFile = TFile::Open(outputFile,"UPDATE");
    TTree *treenew = (TTree*)outFile->Get("HTauTauTree");

    TMVA::Reader * reader = new TMVA::Reader () ;
    TMVA::Reader * readerResonant = new TMVA::Reader () ;
    TMVA::Reader * readerResonantHM = new TMVA::Reader () ;
    TMVA::Reader * readerResonantLM = new TMVA::Reader () ;
    TMVA::Reader * readerNonResonant = new TMVA::Reader () ;
    Float_t mvatautau,mvamutau, mvaetau, mvaleptau, mvaresonant, mvaresonantHM, mvaresonantLM, mvanonresonant;
    TBranch *mvaBranchmutau;
    TBranch *mvaBranchtautau;
    TBranch *mvaBranchetau;
    TBranch *mvaBranchleptau;
    TBranch *mvaBranchResonant;
    TBranch *mvaBranchResonantHM;
    TBranch *mvaBranchResonantLM;
    TBranch *mvaBranchNonResonant;

    for (string var : TMVAvariables)
    {
      treenew->SetBranchAddress (var.c_str (), &(allVarsMap.at (var))) ;
      reader->AddVariable (var, &(allVarsMap.at (var))) ;
    }

    for (string var : TMVAspectators)
    {
      treenew->SetBranchAddress (var.c_str (), &(allVarsMap.at (var))) ;
      reader->AddSpectator (var, &(allVarsMap.at (var))) ;
    }

    for (pair<string, string> vpair : splitTMVAvariablesResonant)
    {
      treenew->SetBranchAddress (vpair.first.c_str (), &(allVarsMap.at (vpair.first))) ;
      readerResonant->AddVariable (vpair.second.c_str (), &(allVarsMap.at (vpair.first))) ;
    }

    for (pair<string, string> vpair : splitTMVAvariablesResonantHM)
    {
      treenew->SetBranchAddress (vpair.first.c_str (), &(allVarsMap.at (vpair.first))) ;
      readerResonantHM->AddVariable (vpair.second.c_str (), &(allVarsMap.at (vpair.first))) ;
      // cout << "DEBUG HM: " << vpair.second.c_str () <<  " <-- " << vpair.first.c_str () << endl;
    }

    for (pair<string, string> vpair : splitTMVAvariablesResonantLM)
    {
      treenew->SetBranchAddress (vpair.first.c_str (), &(allVarsMap.at (vpair.first))) ;
      readerResonantLM->AddVariable (vpair.second.c_str (), &(allVarsMap.at (vpair.first))) ;
    }

    for (pair<string, string> vpair : splitTMVAvariablesNonResonant)
    {
      treenew->SetBranchAddress (vpair.first.c_str (), &(allVarsMap.at (vpair.first))) ;
      readerNonResonant->AddVariable (vpair.second.c_str (), &(allVarsMap.at (vpair.first))) ;
    }

    if (doMuTau)  mvaBranchmutau = treenew->Branch ("MuTauKine", &mvamutau, "MuTauKine/F") ;
    if (doETau)   mvaBranchetau = treenew->Branch ("ETauKine", &mvaetau, "ETauKine/F") ;
    if (doTauTau) mvaBranchtautau = treenew->Branch ("TauTauKine", &mvatautau, "TauTauKine/F") ;
    if (doLepTau) mvaBranchleptau = treenew->Branch ("LepTauKine", &mvaleptau, "LepTauKine/F") ;
    if (doResonant) mvaBranchResonant = treenew->Branch ("BDTResonant", &mvaresonant, "BDTResonant/F") ;
    if (doResonantHM) mvaBranchResonantHM = treenew->Branch ("BDTResonantHM", &mvaresonantHM, "BDTResonantHM/F") ;
    if (doResonantLM) mvaBranchResonantLM = treenew->Branch ("BDTResonantLM", &mvaresonantLM, "BDTResonantLM/F") ;
    if (doNonResonant) mvaBranchNonResonant = treenew->Branch ("BDTNonResonant", &mvanonresonant, "BDTNonResonant/F") ;
    //}
    if (doMuTau)   reader->BookMVA ("MuTauKine",  TMVAweightsMuTau.c_str ()) ;
    if (doETau)    reader->BookMVA ("ETauKine",  TMVAweightsETau.c_str ()) ;
    if (doTauTau)  reader->BookMVA ("TauTauKine",  TMVAweightsTauTau.c_str ()) ;
    if (doLepTau)  reader->BookMVA ("LepTauKine",  TMVAweightsLepTau.c_str ()) ;
    if (doResonant)  readerResonant->BookMVA ("BDT_full_mass_iso_nodrbbsv",  TMVAweightsResonant.c_str ()) ;
    if (doResonantHM)  readerResonantHM->BookMVA ("500t_PU_mass_newvars_HIGH_oldvars",  TMVAweightsResonantHM.c_str ()) ;
    if (doResonantLM)  readerResonantLM->BookMVA ("500t_PU_mass_newvars_LOW",  TMVAweightsResonantLM.c_str ()) ;
    if (doNonResonant)  readerNonResonant->BookMVA ("BDT_nonres_SM",  TMVAweightsNonResonant.c_str ()) ;

    int nentries = treenew->GetEntries();
    for(int i=0;i<nentries;i++){
      treenew->GetEntry(i);

      if (doMuTau)   mvamutau= reader->EvaluateMVA ("MuTauKine") ;
      if (doETau)    mvaetau= reader->EvaluateMVA ("ETauKine") ;
      if (doTauTau)  mvatautau= reader->EvaluateMVA ("TauTauKine") ;
      if (doLepTau)  mvaleptau= reader->EvaluateMVA ("LepTauKine") ;
      if (doResonant)  mvaresonant= readerResonant->EvaluateMVA ("BDT_full_mass_iso_nodrbbsv") ;
      if (doResonantHM)  mvaresonantHM= readerResonantHM->EvaluateMVA ("500t_PU_mass_newvars_HIGH_oldvars") ;
      if (doResonantLM)  mvaresonantLM= readerResonantLM->EvaluateMVA ("500t_PU_mass_newvars_LOW") ;
      if (doNonResonant)  mvanonresonant= readerNonResonant->EvaluateMVA ("BDT_nonres_SM") ;

      if (doMuTau)    mvaBranchmutau->Fill();
      if (doETau)     mvaBranchetau->Fill();
      if (doTauTau)   mvaBranchtautau->Fill();
      if (doLepTau)   mvaBranchleptau->Fill();
      if (doResonant)  mvaBranchResonant->Fill();
      if (doResonantHM)  mvaBranchResonantHM->Fill();
      if (doResonantLM)  mvaBranchResonantLM->Fill();
      if (doNonResonant)  mvaBranchNonResonant->Fill();
    }

    outFile->cd () ;
    h_eff.Write () ;
    treenew->Write ("", TObject::kOverwrite) ;
    outFile->Write();
    outFile->Close();

    delete reader;
    delete readerResonant;
    delete readerResonantHM;
    delete readerResonantLM;
    delete readerNonResonant;
  }
  // MULTICLASS
  bool compute_multiclass = (gConfigParser->isDefined("Multiclass::computeMVA") ? gConfigParser->readBoolOption("Multiclass::computeMVA") : false);
  if (compute_multiclass)
  {
    cout << " ------------ ############### ----- Multiclass ----- ############### ------------ " << endl;

    // set the multiclass year
    int year = 2016;

    // models to load for inference
    std::vector<std::pair<std::string, std::string>> modelSpecs = {
      //{ "v0" , "kl1_c2v1_c31"    },
      //{ "v0" , "kl1_c2v1_c31_vbfbsm" }
      //{ "v1" , "kl1_c2v1_c31"    },
      //{ "v2" , "kl1_c2v1_c31"    },
      //{ "v3" , "kl1_c2v1_c31_vbf"},
      //{ "v3" , "kl1_c2v1_c31_vr" },
      //{ "v3b", "kl1_c2v1_c31_vbf"},
      //{ "v3b", "kl1_c2v1_c31_vr" },
      //{ "v4" , "kl1_c2v1_c31_vbf"},
      //{ "v4" , "kl1_c2v1_c31_vr" },
      { "v5" , "kl1_c2v1_c31_vbf"}
    };

    // read the input tree
    TFile* outFile = TFile::Open(outputFile, "UPDATE");
    TTree* outTree = (TTree*)outFile->Get("HTauTauTree");

    // create the multiclass inferface and run it
    MulticlassInterface mci(year, modelSpecs);
    mci.extendTree(outTree);

    // write the output file
    outTree->Write("", TObject::kOverwrite);
    outFile->Close();

  } // END MULTICLASS

  // NEW BDT
  bool computeBDTsm = (gConfigParser->isDefined("BDTsm::computeMVA") ? gConfigParser->readBoolOption ("BDTsm::computeMVA") : false);
  bool computeBDTlm = (gConfigParser->isDefined("BDTlm::computeMVA") ? gConfigParser->readBoolOption ("BDTlm::computeMVA") : false);
  bool computeBDTmm = (gConfigParser->isDefined("BDTmm::computeMVA") ? gConfigParser->readBoolOption ("BDTmm::computeMVA") : false);
  bool computeBDThm = (gConfigParser->isDefined("BDThm::computeMVA") ? gConfigParser->readBoolOption ("BDThm::computeMVA") : false);
  bool computeVBFBDT = (gConfigParser->isDefined("BDTVBF::computeMVA") ? gConfigParser->readBoolOption ("BDTVBF::computeMVA") : false);

  if (computeBDTsm || computeBDTlm || computeBDTmm || computeBDThm || computeVBFBDT)
  {
    cout << " ------------ ############### ----- NEW BDT ----- ############### ------------ " <<endl;

    bool doSM = computeBDTsm;
    bool doLM = computeBDTlm;
    bool doMM = computeBDTmm;
    bool doHM = computeBDThm;
    bool doVBF = computeVBFBDT;

    // weights file
    string TMVAweightsSM = "";
    string TMVAweightsLM = "";
    string TMVAweightsMM = "";
    string TMVAweightsHM = "";
    string TMVAweightsVBF = "";
    vector<float> SM_kl;
    vector<float> LM_mass;
    vector<float> MM_mass;
    vector<float> HM_mass;
    vector<int> LM_spin;
    vector<int> MM_spin;
    vector<int> HM_spin;

    if (doSM)
    {
      TMVAweightsSM = gConfigParser->readStringOption ("BDTsm::weights");
      SM_kl         = gConfigParser->readFloatListOption("BDTsm::kl");
    }
    if (doLM)
    {
      TMVAweightsLM = gConfigParser->readStringOption ("BDTlm::weights");
      LM_mass       = gConfigParser->readFloatListOption ("BDTlm::mass");
      LM_spin       = gConfigParser->readIntListOption ("BDTlm::spin");
    }
    if (doMM)
    {
      TMVAweightsMM = gConfigParser->readStringOption ("BDTmm::weights");
      MM_mass       = gConfigParser->readFloatListOption ("BDTmm::mass");
      MM_spin       = gConfigParser->readIntListOption ("BDTmm::spin");
    }
    if (doHM)
    {
      TMVAweightsHM = gConfigParser->readStringOption ("BDThm::weights");
      HM_mass       = gConfigParser->readFloatListOption ("BDThm::mass");
      HM_spin       = gConfigParser->readIntListOption ("BDThm::spin");
    }
    if (doVBF)
    {
      TMVAweightsVBF = gConfigParser->readStringOption ("BDTVBF::weights");
    }

    // Input variables
    vector<string> TMVAvariablesSM = ( doSM ? gConfigParser->readStringListOption ("BDTsm::variables") : vector<string>(0) );
    vector<string> TMVAvariablesLM = ( doLM ? gConfigParser->readStringListOption ("BDTlm::variables") : vector<string>(0) );
    vector<string> TMVAvariablesMM = ( doMM ? gConfigParser->readStringListOption ("BDTmm::variables") : vector<string>(0) );
    vector<string> TMVAvariablesHM = ( doHM ? gConfigParser->readStringListOption ("BDThm::variables") : vector<string>(0) );
    vector<string> TMVAvariablesVBF = ( doVBF ? gConfigParser->readStringListOption ("BDTVBF::variables") : vector<string>(0) );

    // Split the resonant name in two strings
    vector<pair<string, string>> splitTMVAvariablesSM;
    for (unsigned int iv = 0 ; iv < TMVAvariablesSM.size () ; ++iv)
    {
      // Split my_name:BDT_name in two strings
      std::stringstream packedName(TMVAvariablesSM.at(iv));
      std::string segment;
      std::vector<std::string> unpackedNames;
      while(std::getline(packedName, segment, ':'))
        unpackedNames.push_back(segment);

      splitTMVAvariablesSM.push_back(make_pair(unpackedNames.at(0), unpackedNames.at(1)));
    }

    vector<pair<string, string>> splitTMVAvariablesLM;
    for (unsigned int iv = 0 ; iv < TMVAvariablesLM.size () ; ++iv)
    {
      // Split my_name:BDT_name in two strings
      std::stringstream packedName(TMVAvariablesLM.at(iv));
      std::string segment;
      std::vector<std::string> unpackedNames;
      while(std::getline(packedName, segment, ':'))
        unpackedNames.push_back(segment);

      splitTMVAvariablesLM.push_back(make_pair(unpackedNames.at(0), unpackedNames.at(1)));
    }

    vector<pair<string, string>> splitTMVAvariablesMM;
    for (unsigned int iv = 0 ; iv < TMVAvariablesMM.size () ; ++iv)
    {
      // Split my_name:BDT_name in two strings
      std::stringstream packedName(TMVAvariablesMM.at(iv));
      std::string segment;
      std::vector<std::string> unpackedNames;
      while(std::getline(packedName, segment, ':'))
        unpackedNames.push_back(segment);

      splitTMVAvariablesMM.push_back(make_pair(unpackedNames.at(0), unpackedNames.at(1)));
    }

    vector<pair<string, string>> splitTMVAvariablesHM;
    for (unsigned int iv = 0 ; iv < TMVAvariablesHM.size () ; ++iv)
    {
      // Split my_name:BDT_name in two strings
      std::stringstream packedName(TMVAvariablesHM.at(iv));
      std::string segment;
      std::vector<std::string> unpackedNames;
      while(std::getline(packedName, segment, ':'))
        unpackedNames.push_back(segment);

      splitTMVAvariablesHM.push_back(make_pair(unpackedNames.at(0), unpackedNames.at(1)));
    }

    vector<pair<string, string>> splitTMVAvariablesVBF;
    for (unsigned int iv = 0 ; iv < TMVAvariablesVBF.size () ; ++iv)
    {
      // Split my_name:BDT_name in two strings
      std::stringstream packedName(TMVAvariablesVBF.at(iv));
      std::string segment;
      std::vector<std::string> unpackedNames;
      while(std::getline(packedName, segment, ':'))
        unpackedNames.push_back(segment);

      splitTMVAvariablesVBF.push_back(make_pair(unpackedNames.at(0), unpackedNames.at(1)));
    }

    // Now merge all names into a vector to get a list of uniquely needed elements
    std::vector<string> allVars;
    for (unsigned int iv = 0; iv < splitTMVAvariablesSM.size(); ++iv)
      allVars.push_back(splitTMVAvariablesSM.at(iv).first);
    for (unsigned int iv = 0; iv < splitTMVAvariablesLM.size(); ++iv)
      allVars.push_back(splitTMVAvariablesLM.at(iv).first);
    for (unsigned int iv = 0; iv < splitTMVAvariablesMM.size(); ++iv)
      allVars.push_back(splitTMVAvariablesMM.at(iv).first);
    for (unsigned int iv = 0; iv < splitTMVAvariablesHM.size(); ++iv)
      allVars.push_back(splitTMVAvariablesHM.at(iv).first);
    for (unsigned int iv = 0; iv < splitTMVAvariablesVBF.size(); ++iv)
      allVars.push_back(splitTMVAvariablesVBF.at(iv).first);

    sort(allVars.begin(), allVars.end());
    allVars.erase( unique( allVars.begin(), allVars.end() ), allVars.end() );

    // Create map to contain values of variables
    std::map<string, float> allVarsMap;
    for (string var : allVars)
      allVarsMap[var] = 0.0;

    // Open tree to be updated
    TFile *outFile = TFile::Open(outputFile,"UPDATE");
    TTree *treenew = (TTree*)outFile->Get("HTauTauTree");
    int nentries = treenew->GetEntries();

    // Create vectors to store all the BDT outputs and relative vectors of TBranches
    std::vector<float> outSM (SM_kl.size());
    std::vector<float> outLM (LM_spin.size()*LM_mass.size());
    std::vector<float> outMM (MM_spin.size()*MM_mass.size());
    std::vector<float> outHM (HM_spin.size()*HM_mass.size());
    float mvaVBF;

    std::vector<TBranch*> branchSM (SM_kl.size());
    std::vector<TBranch*> branchLM (LM_spin.size()*LM_mass.size());
    std::vector<TBranch*> branchMM (MM_spin.size()*MM_mass.size());
    std::vector<TBranch*> branchHM (HM_spin.size()*HM_mass.size());
    TBranch *mvaBranchVBF;

    // Declare the TMVA readers
    TMVA::Reader * readerSM = new TMVA::Reader () ;
    TMVA::Reader * readerLM = new TMVA::Reader () ;
    TMVA::Reader * readerMM = new TMVA::Reader () ;
    TMVA::Reader * readerHM = new TMVA::Reader () ;
    TMVA::Reader * readerVBF = new TMVA::Reader () ;

    // Use a different variable for channel, otherwise it does not work, I don't know why
    float channel_BDT;
    treenew ->SetBranchAddress ("BDT_channel", &channel_BDT) ;

    // Assign variables to SM reader
    for (pair<string, string> vpair : splitTMVAvariablesSM)
    {
      treenew ->SetBranchAddress (vpair.first.c_str (), &(allVarsMap.at (vpair.first))) ;
      readerSM->AddVariable (vpair.second.c_str (), &(allVarsMap.at (vpair.first))) ;
    }
    // Add the channel and kl variable to the SM reader
    readerSM->AddVariable("channel", &channel_BDT);
    float kl_var;
    readerSM->AddVariable("kl", &kl_var);


    // Assign variables to LM reader
    for (pair<string, string> vpair : splitTMVAvariablesLM)
    {
      treenew ->SetBranchAddress (vpair.first.c_str (), &(allVarsMap.at (vpair.first))) ;
      readerLM->AddVariable (vpair.second.c_str (), &(allVarsMap.at (vpair.first))) ;
    }
    // Add mass, channel and spin to the LM reader
    float mass_LM;
    float spin_LM;
    readerLM->AddVariable("mass", &mass_LM);
    readerLM->AddVariable("channel", &channel_BDT);
    readerLM->AddVariable("spin", &spin_LM);


    // Assign variables to MM reader
    for (pair<string, string> vpair : splitTMVAvariablesMM)
    {
      treenew ->SetBranchAddress (vpair.first.c_str (), &(allVarsMap.at (vpair.first))) ;
      readerMM->AddVariable (vpair.second.c_str (), &(allVarsMap.at (vpair.first))) ;
    }
    // Add mass, channel and spin to the LM reader
    float mass_MM;
    float spin_MM;
    readerMM->AddVariable("mass", &mass_MM);
    readerMM->AddVariable("channel", &channel_BDT);
    readerMM->AddVariable("spin", &spin_MM);


    // Assign variables to HM reader
    for (pair<string, string> vpair : splitTMVAvariablesHM)
    {
      treenew ->SetBranchAddress (vpair.first.c_str (), &(allVarsMap.at (vpair.first))) ;
      readerHM->AddVariable (vpair.second.c_str (), &(allVarsMap.at (vpair.first))) ;
    }
    // Add mass, channel and spin to the LM reader
    float mass_HM;
    float spin_HM;
    readerHM->AddVariable("mass", &mass_HM);
    readerHM->AddVariable("channel", &channel_BDT);
    readerHM->AddVariable("spin", &spin_HM);


    // Assign variables to VBF reader
    for (pair<string, string> vpair : splitTMVAvariablesVBF)
    {
      treenew ->SetBranchAddress (vpair.first.c_str (), &(allVarsMap.at (vpair.first))) ;
      readerVBF->AddVariable (vpair.second.c_str (), &(allVarsMap.at (vpair.first))) ;
    }

    // Book the MVA methods
    if(doSM) readerSM->BookMVA("Grad_1", TMVAweightsSM.c_str() );
    if(doLM) readerLM->BookMVA("Grad_2", TMVAweightsLM.c_str() );
    if(doMM) readerMM->BookMVA("Grad_3", TMVAweightsMM.c_str() );
    if(doHM) readerHM->BookMVA("Grad_4", TMVAweightsHM.c_str() );
    if(doVBF) readerVBF->BookMVA("VBF", TMVAweightsVBF.c_str() );

    // Calculate BDT output for SM
    if (doSM)
    {
      int idxSM = 0;
      for (unsigned int ikl = 0; ikl < SM_kl.size(); ++ikl)
      {
	// Declare the BDT output branch
	std::string branch_name = boost::str(boost::format("BDToutSM_kl_%d") % SM_kl.at(ikl));
	branchSM.at(idxSM) = treenew->Branch(branch_name.c_str(), &outSM.at(idxSM));

	// Assign value to parametrization variables
	kl_var = SM_kl.at(ikl);

	// Calculate BDT output
	for(int i=0;i<nentries;i++)
	{
	  treenew->GetEntry(i);
	  outSM.at(idxSM) = readerSM->EvaluateMVA("Grad_1");
	  branchSM.at(idxSM)->Fill();
	}
	++idxSM;
      }
    }


    // Calculate BDT output for LM
    if (doLM)
    {
      int idxLM = 0;
      for (unsigned int ispin = 0; ispin < LM_spin.size(); ++ispin)
      {
	for (unsigned int imass = 0; imass < LM_mass.size(); ++imass)
	{
	  // Declare the BDT output branch
	  std::string branch_name = boost::str(boost::format("BDToutLM_spin_%d_mass_%d") % LM_spin.at(ispin) % LM_mass.at(imass));
	  branchLM.at(idxLM) = treenew->Branch(branch_name.c_str(), &outLM.at(idxLM));

	  // Assign value to parametrization variables
	  mass_LM = LM_mass.at(imass);
	  spin_LM = LM_spin.at(ispin);

	  // Calculate BDT output
	  for(int i=0;i<nentries;i++)
	  {
	    treenew->GetEntry(i);
	    outLM.at(idxLM) = readerLM->EvaluateMVA("Grad_2");
	    branchLM.at(idxLM)->Fill();
	  }
	  ++idxLM;
	}
      }
    }


    // Calculate BDT output for MM
    if (doMM)
    {
      int idxMM = 0;
      for (unsigned int ispin = 0; ispin < MM_spin.size(); ++ispin)
      {
	for (unsigned int imass = 0; imass < MM_mass.size(); ++imass)
	{
	  // Declare the BDT output branch
	  std::string branch_name = boost::str(boost::format("BDToutMM_spin_%d_mass_%d") % MM_spin.at(ispin) % MM_mass.at(imass));
	  branchMM.at(idxMM) = treenew->Branch(branch_name.c_str(), &outMM.at(idxMM));

	  // Assign value to parametrization variables
	  mass_MM = MM_mass.at(imass);
	  spin_MM = MM_spin.at(ispin);

	  // Calculate BDT output
	  for(int i=0;i<nentries;i++)
	  {
	    treenew->GetEntry(i);
	    outMM.at(idxMM) = readerMM->EvaluateMVA("Grad_3");
	    branchMM.at(idxMM)->Fill();
	  }
	  ++idxMM;
	}
      }
    }


    // Calculate BDT output for HM
    if (doHM)
    {
      int idxHM = 0;
      for (unsigned int ispin = 0; ispin < HM_spin.size(); ++ispin)
      {
	for (unsigned int imass = 0; imass < HM_mass.size(); ++imass)
	{
	  // Declare the BDT output branch
	  std::string branch_name = boost::str(boost::format("BDToutHM_spin_%d_mass_%d") % HM_spin.at(ispin) % HM_mass.at(imass));
	  branchHM.at(idxHM) = treenew->Branch(branch_name.c_str(), &outHM.at(idxHM));

	  // Assign value to parametrization variables
	  mass_HM = HM_mass.at(imass);
	  spin_HM = HM_spin.at(ispin);

	  // Calculate BDT output
	  for(int i=0;i<nentries;i++)
	  {
	    treenew->GetEntry(i);
	    outHM.at(idxHM) = readerHM->EvaluateMVA("Grad_4");
	    branchHM.at(idxHM)->Fill();
	  }
	  ++idxHM;
	}
      }
    }


    // Calculate BDT output for VBF
    if (doVBF)
    {
      mvaBranchVBF = treenew->Branch("BDToutVBF", &mvaVBF, "BDToutVBF/F") ;
      for(int i=0;i<nentries;i++)
      {
	treenew->GetEntry(i);
	mvaVBF= readerVBF->EvaluateMVA("VBF") ;
	mvaBranchVBF->Fill();
      }
    }

    // Update tree and delete readers
    outFile->cd();
    treenew->Write ("", TObject::kOverwrite) ;
    outFile->Write();
    outFile->Close();

    delete readerSM;
    delete readerLM;
    delete readerMM;
    delete readerHM;
    delete readerVBF;

  } // End new BDT

  // NEW DNN
  bool computeDNN = (gConfigParser->isDefined("DNN::computeMVA") ? gConfigParser->readBoolOption("DNN::computeMVA") : false);
  if (computeDNN)
  {
    cout << " ------------ ############### ----- NEW DNN ----- ############### ------------ " <<endl;

    // Declare constants
    const double E_MASS  = 0.0005109989; //GeV
    const double MU_MASS = 0.1056583715; //GeV

    // Reaf from configs
    std::string model_dir = gConfigParser->readStringOption ("DNN::weights");
    std::cout << "DNN::weights   : " << model_dir << std::endl;

    vector<float> DNN_kl;
    DNN_kl = gConfigParser->readFloatListOption("DNN::kl");

    std::string features_file = gConfigParser->readStringOption ("DNN::features");
    std::cout << "DNN::features  : " << features_file << std::endl;

    // Read from file the requested features to be computed
    std::ifstream features_infile(features_file);
    std::vector<std::string> requested;
    std::string featureName;
    while ( features_infile >> featureName)
    {
      requested.push_back(featureName);
    }
    features_infile.close();

    // Declare the wrapper
    InfWrapper wrapper(model_dir, 1, false);

    // Open file to read values and compute predictions
    TFile* in_file = TFile::Open(outputFile);

    // Store prediction in vector of vectors of floats:
    // [kl=1 : [evt1_pred, evt2_pred, evt3_pred ...], kl=2 : [evt1_opred, evt2_pred, evt3_pred...]]
    std::vector<std::vector<float>> outDNN;

    // Initialize the EvtProc
    EvtProc evt_proc(false, requested, true);

    // Declare "service" variables
    TLorentzVector pep_b_1, pep_b_2, pep_l_1, pep_l_2, pep_met, pep_svfit, pep_vbf_1, pep_vbf_2;
    int DNN_pType;

    // Declare variables input to the EvtProc
    DNNVector DNN_b_1, DNN_b_2, DNN_l_1, DNN_l_2, DNN_met, DNN_svfit, DNN_vbf_1, DNN_vbf_2;
    float DNN_kinfit_mass, DNN_kinfit_chi2, DNN_mt2;
    float DNN_b_1_deepflav, DNN_b_2_deepflav;
    float DNN_b_1_CvsL, DNN_b_2_CvsL, DNN_vbf_1_CvsL, DNN_vbf_2_CvsL;
    float DNN_b_1_CvsB, DNN_b_2_CvsB, DNN_vbf_1_CvsB, DNN_vbf_2_CvsB;
    float DNN_b_1_HHbtag, DNN_b_2_HHbtag, DNN_vbf_1_HHbtag, DNN_vbf_2_HHbtag;
    int DNN_is_boosted, DNN_n_vbf, DNN_isvbf;
    unsigned long long int DNN_evt;
    bool DNN_svfit_conv, DNN_hh_kinfit_conv, DNN_pass_massCut;
    int DNN_nleps, DNN_nbjetscand;

    Channel DNN_e_channel;
    Year DNN_e_year(y16);
    Spin DNN_spin(nonres);
    float DNN_klambda;
    float DNN_res_mass = 125.; // FIXME

    // Declare TTreeReaders
    TTreeReader reader("HTauTauTree", in_file);

    // Declare TTreeReaderValue
    TTreeReaderValue<unsigned long long> rv_evt(reader, "EventNumber");
    TTreeReaderValue<int> rv_ptype(reader, "pairType");
    TTreeReaderValue<int> rv_isboosted(reader, "isBoosted");
    TTreeReaderValue<int> rv_isvbf(reader, "isVBF");
    TTreeReaderValue<int> rv_nleps(reader, "nleps");
    TTreeReaderValue<int> rv_nbjetscand(reader, "nbjetscand");

    TTreeReaderValue<float> rv_kinfit_mass(reader, "HHKin_mass_raw");
    TTreeReaderValue<float> rv_kinfit_chi2(reader, "HHKin_mass_raw_chi2");
    TTreeReaderValue<float> rv_mt2(reader, "MT2");

    TTreeReaderValue<float> rv_b_1_deepflav(reader, "bjet1_bID_deepFlavor");
    TTreeReaderValue<float> rv_b_2_deepflav(reader, "bjet2_bID_deepFlavor");
    TTreeReaderValue<float> rv_b_1_CvsL    (reader, "bjet1_CvsL");
    TTreeReaderValue<float> rv_b_2_CvsL    (reader, "bjet2_CvsL");
    TTreeReaderValue<float> rv_vbf_1_CvsL  (reader, "VBFjet1_CvsL");
    TTreeReaderValue<float> rv_vbf_2_CvsL  (reader, "VBFjet2_CvsL");
    TTreeReaderValue<float> rv_b_1_CvsB    (reader, "bjet1_CvsB");
    TTreeReaderValue<float> rv_b_2_CvsB    (reader, "bjet2_CvsB");
    TTreeReaderValue<float> rv_vbf_1_CvsB  (reader, "VBFjet1_CvsB");
    TTreeReaderValue<float> rv_vbf_2_CvsB  (reader, "VBFjet2_CvsB");
    TTreeReaderValue<float> rv_b_1_HHbtag  (reader, "bjet1_HHbtag");
    TTreeReaderValue<float> rv_b_2_HHbtag  (reader, "bjet2_HHbtag");
    TTreeReaderValue<float> rv_vbf_1_HHbtag(reader, "VBFjet1_HHbtag");
    TTreeReaderValue<float> rv_vbf_2_HHbtag(reader, "VBFjet2_HHbtag");

    TTreeReaderValue<float> rv_svfit_pT(reader, "tauH_SVFIT_pt");
    TTreeReaderValue<float> rv_svfit_eta(reader, "tauH_SVFIT_eta");
    TTreeReaderValue<float> rv_svfit_phi(reader, "tauH_SVFIT_phi");
    TTreeReaderValue<float> rv_svfit_mass(reader, "tauH_SVFIT_mass");

    TTreeReaderValue<float> rv_l_1_pT(reader, "dau1_pt");
    TTreeReaderValue<float> rv_l_1_eta(reader, "dau1_eta");
    TTreeReaderValue<float> rv_l_1_phi(reader, "dau1_phi");
    TTreeReaderValue<float> rv_l_1_e(reader, "dau1_e");

    TTreeReaderValue<float> rv_l_2_pT(reader, "dau2_pt");
    TTreeReaderValue<float> rv_l_2_eta(reader, "dau2_eta");
    TTreeReaderValue<float> rv_l_2_phi(reader, "dau2_phi");
    TTreeReaderValue<float> rv_l_2_e(reader, "dau2_e");

    TTreeReaderValue<float> rv_met_pT(reader, "met_et");
    TTreeReaderValue<float> rv_met_phi(reader, "met_phi");

    TTreeReaderValue<float> rv_b_1_pT(reader, "bjet1_pt");
    TTreeReaderValue<float> rv_b_1_eta(reader, "bjet1_eta");
    TTreeReaderValue<float> rv_b_1_phi(reader, "bjet1_phi");
    TTreeReaderValue<float> rv_b_1_e(reader, "bjet1_e");

    TTreeReaderValue<float> rv_b_2_pT(reader, "bjet2_pt");
    TTreeReaderValue<float> rv_b_2_eta(reader, "bjet2_eta");
    TTreeReaderValue<float> rv_b_2_phi(reader, "bjet2_phi");
    TTreeReaderValue<float> rv_b_2_e(reader, "bjet2_e");

    TTreeReaderValue<float> rv_vbf_1_pT(reader, "VBFjet1_pt");
    TTreeReaderValue<float> rv_vbf_1_eta(reader, "VBFjet1_eta");
    TTreeReaderValue<float> rv_vbf_1_phi(reader, "VBFjet1_phi");
    TTreeReaderValue<float> rv_vbf_1_e(reader, "VBFjet1_e");

    TTreeReaderValue<float> rv_vbf_2_pT(reader, "VBFjet2_pt");
    TTreeReaderValue<float> rv_vbf_2_eta(reader, "VBFjet2_eta");
    TTreeReaderValue<float> rv_vbf_2_phi(reader, "VBFjet2_phi");
    TTreeReaderValue<float> rv_vbf_2_e(reader, "VBFjet2_e");

    // Declare feature values and model prediction values
    std::vector<float> feat_vals;
    float DNN_pred;

    // Index and number of entries for loop on entries
    long int c_event(0), n_tot_events(reader.GetEntries(true));

    // Resize output vector to store all DNN predictions (initialized to -1.0)
    outDNN.resize(DNN_kl.size(), std::vector<float>(n_tot_events,-1.));
    std::cout << "DNN::DNN_kl size   : " << DNN_kl.size() << endl;
    std::cout << "DNN::DNN_kl values : ";
    for (uint i=0; i<DNN_kl.size();i++) cout << DNN_kl.at(i) << " ";
    std::cout << endl;
    std::cout << "DNN::n_tot_events  : " << n_tot_events << endl;

    // Loop on entries with TTreeReader
    while (reader.Next())
    {
      if (c_event%5000 == 0) std::cout << "DNN::event " << c_event << " / " << n_tot_events << "\n";

      // Apply minimal baseline selection (3rd lept veto, n bjet cands and channel)
      DNN_nbjetscand = *rv_nbjetscand;
      DNN_nleps      = *rv_nleps;
      DNN_pType      = *rv_ptype;
      if (DNN_nleps!=0 || DNN_nbjetscand < 2 || DNN_pType>2)
      {
	//std::cout << "DNN::skipping evtNumber: " << *rv_evt << " because nleps=" << DNN_nleps << ", nbjetscand=" << DNN_nbjetscand  << "and channel=" << DNN_pType << endl;
	c_event++;
	continue;
      }

      // Load values
      DNN_evt        = *rv_evt;
      DNN_is_boosted = *rv_isboosted;
      DNN_isvbf      = *rv_isvbf;

      DNN_kinfit_mass   = *rv_kinfit_mass;
      DNN_kinfit_chi2   = *rv_kinfit_chi2;
      DNN_mt2           = *rv_mt2;

      DNN_b_1_deepflav = *rv_b_1_deepflav;
      DNN_b_2_deepflav = *rv_b_2_deepflav;

      DNN_b_1_CvsL     = *rv_b_1_CvsL;
      DNN_b_2_CvsL     = *rv_b_2_CvsL;
      DNN_vbf_1_CvsL   = *rv_vbf_1_CvsL;
      DNN_vbf_2_CvsL   = *rv_vbf_2_CvsL;
      DNN_b_1_CvsB     = *rv_b_1_CvsB;
      DNN_b_2_CvsB     = *rv_b_2_CvsB;
      DNN_vbf_1_CvsB   = *rv_vbf_1_CvsB;
      DNN_vbf_2_CvsB   = *rv_vbf_2_CvsB;
      DNN_b_1_HHbtag   = *rv_b_1_HHbtag;
      DNN_b_2_HHbtag   = *rv_b_2_HHbtag;
      DNN_vbf_1_HHbtag = *rv_vbf_1_HHbtag;
      DNN_vbf_2_HHbtag = *rv_vbf_2_HHbtag;

      DNN_svfit_conv     = *rv_svfit_mass  > 0;
      DNN_hh_kinfit_conv = DNN_kinfit_chi2 > 0;

      // Load PEP Vectors first
      pep_l_1  .SetPtEtaPhiE(*rv_l_1_pT  , *rv_l_1_eta  , *rv_l_1_phi  , *rv_l_1_e	);
      pep_l_2  .SetPtEtaPhiE(*rv_l_2_pT  , *rv_l_2_eta  , *rv_l_2_phi  , *rv_l_2_e	);
      pep_met  .SetPtEtaPhiE(*rv_met_pT  , 0            , *rv_met_phi  , 0		);
      pep_svfit.SetPtEtaPhiM(*rv_svfit_pT, *rv_svfit_eta, *rv_svfit_phi, *rv_svfit_mass);
      pep_b_1  .SetPtEtaPhiE(*rv_b_1_pT  , *rv_b_1_eta  , *rv_b_1_phi  , *rv_b_1_e	);
      pep_b_2  .SetPtEtaPhiE(*rv_b_2_pT  , *rv_b_2_eta  , *rv_b_2_phi  , *rv_b_2_e	);
      pep_vbf_1.SetPtEtaPhiE(*rv_vbf_1_pT, *rv_vbf_1_eta, *rv_vbf_1_phi, *rv_vbf_1_e	);
      pep_vbf_2.SetPtEtaPhiE(*rv_vbf_2_pT, *rv_vbf_2_eta, *rv_vbf_2_phi, *rv_vbf_2_e	);

      // Use PEP Vectors to declare DNN vectors
      DNN_l_1.SetCoordinates(pep_l_1.Px(),     pep_l_1.Py(),   pep_l_1.Pz(),   pep_l_1.M());
      if      (DNN_pType == 0 )
      {
	DNN_e_channel = muTau;
	DNN_l_1.SetM(MU_MASS);
      }
      else if (DNN_pType == 1)
      {
	DNN_e_channel = eTau;
	DNN_l_1.SetM(E_MASS);
      }
      else if (DNN_pType == 2)
      {
	DNN_e_channel = tauTau;
      }
      DNN_l_2.SetCoordinates  (pep_l_2.Px()  , pep_l_2.Py()  , pep_l_2.Pz()  , pep_l_2.M()  );
      DNN_met.SetCoordinates  (pep_met.Px()  , pep_met.Py()  , 0             , 0            );
      DNN_svfit.SetCoordinates(pep_svfit.Px(), pep_svfit.Py(), pep_svfit.Pz(), pep_svfit.M());
      DNN_b_1.SetCoordinates  (pep_b_1.Px()  , pep_b_1.Py()  , pep_b_1.Pz()  , pep_b_1.M()  );
      DNN_b_2.SetCoordinates  (pep_b_2.Px()  , pep_b_2.Py()  , pep_b_2.Pz()  , pep_b_2.M()  );
      DNN_vbf_1.SetCoordinates(pep_vbf_1.Px(), pep_vbf_1.Py(), pep_vbf_1.Pz(), pep_vbf_1.M());
      DNN_vbf_2.SetCoordinates(pep_vbf_2.Px(), pep_vbf_2.Py(), pep_vbf_2.Pz(), pep_vbf_2.M());

      DNN_n_vbf = 0;
      if (DNN_isvbf) {
	if (*rv_vbf_1_e != -999.) DNN_n_vbf++;
	if (*rv_vbf_2_e != -999.) DNN_n_vbf++;
      }

      //DNN_pass_massCut = ( ((DNN_svfit.M()-116.)*(DNN_svfit.M()-116.))/(35.*35.) + (((DNN_b_1+DNN_b_2).M()-111.)*((DNN_b_1+DNN_b_2).M()-111.))/(45.*45.) <  1.0 );
      DNN_pass_massCut = true; // since training 2020-07-31-0 this feature is not used, so it is set always to true

      // Loop on configurable options to get the output prediction
      // For each event save the predictions for all the kl values requested
      for (unsigned int jkl = 0; jkl < DNN_kl.size(); ++jkl)
      {
	// Assign external values
	DNN_klambda = DNN_kl.at(jkl);

	// Compute fatures
	feat_vals = evt_proc.process_as_vec(DNN_b_1, DNN_b_2, DNN_l_1, DNN_l_2, DNN_met, DNN_svfit, DNN_vbf_1, DNN_vbf_2,
					    DNN_kinfit_mass, DNN_kinfit_chi2, DNN_mt2, DNN_is_boosted, DNN_b_1_deepflav, DNN_b_2_deepflav,
					    DNN_e_channel, DNN_e_year, DNN_res_mass, DNN_spin, DNN_klambda,
					    DNN_n_vbf, DNN_svfit_conv, DNN_hh_kinfit_conv,
					    DNN_b_1_HHbtag, DNN_b_2_HHbtag, DNN_vbf_1_HHbtag, DNN_vbf_2_HHbtag,
					    DNN_b_1_CvsL, DNN_b_2_CvsL, DNN_vbf_1_CvsL, DNN_vbf_2_CvsL,
					    DNN_b_1_CvsB, DNN_b_2_CvsB, DNN_vbf_1_CvsB, DNN_vbf_2_CvsB,
					    0, 0, 0, // cv, c2v, c3
					    DNN_pass_massCut);

	// Get model prediction
	DNN_pred = wrapper.predict(feat_vals, DNN_evt);
	if (c_event%5000 == 0) cout << " -> kl: "<< DNN_klambda << " - pred: " << DNN_pred << endl;
	//std::vector<std::string>  debug_feats = evt_proc.get_feats();
	//cout << "   - feats: " << endl;
	//for (uint i=0;i<debug_feats.size(); i++) cout << "      " << debug_feats.at(i) << " : " << feat_vals.at(i) << endl ;

	// store prediction in vector as:
	// [evt_1_kl_1, evt_1_kl_2, evt_2_kl_1, evt_2_kl_2 ....]
	outDNN[jkl][c_event] = DNN_pred;
      }

      c_event++;
    } // end loop on entries with TTreeReader

    // Open file and get TTree that must be updated
    TFile *outFile = TFile::Open(outputFile,"UPDATE");
    TTree *treenew = (TTree*)outFile->Get("HTauTauTree");

    // Declare one new branch for each value of klambda
    std::vector<float> outDNNval (DNN_kl.size());
    std::vector<TBranch*> branchDNN (DNN_kl.size()); // prediction branches
    //cout << " - branchDNN size: " << branchDNN.size() << endl;

    for (unsigned int ikl = 0; ikl < DNN_kl.size(); ++ikl)
    {
      std::string branch_name = boost::str(boost::format("DNNoutSM_kl_%d") % DNN_kl.at(ikl));
      branchDNN.at(ikl) = treenew->Branch(branch_name.c_str(), &outDNNval.at(ikl));
    }

    // Fill the new branches
    for (int i=0; i<n_tot_events; i++)
    {
      treenew->GetEntry(i);
      for (unsigned int jkl=0; jkl<DNN_kl.size(); ++jkl)
      {
	outDNNval[jkl] = outDNN[jkl][i];
	branchDNN.at(jkl)->Fill();
      }
    }

    // Update tree and delete readers
    treenew->Write ("", TObject::kOverwrite) ;
    in_file->Close();

  } // END NEW DNN


  cout << "... SKIM finished, exiting." << endl;
  return 0 ;
}
