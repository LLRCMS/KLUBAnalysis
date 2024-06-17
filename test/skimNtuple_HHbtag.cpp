// ntuple skimmer for analysis and synchronization

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <bitset>
#include <map>
#include <unordered_map>
#include <utility>
#include <cassert>
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
#include "met_phi_corr.h"

#include "pnetSF.h"

#include "ScaleFactor.h"
#include "ScaleFactorMET.h"
#include "TriggerAssigner.h"
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

using namespace std;
using DNNVector = ROOT::Math::LorentzVector<ROOT::Math::PxPyPzM4D<float>>;

const double aTopRW = 0.0615;
const double bTopRW = -0.0005;

/* NOTE ON THE COMPUTATION OF STITCH WEIGHTS FOR NLO:
** 3 samples merged: Inclusive, Njet-sliced, pT-sliced.
** merging done according to cross-section from xsdb
** [0,50] GeV pT slice is buggy and not included in the stitching
** -> 2 dedicated scale factors are needed (for pT>50 and pT<50) to restore the total cross-section
*/
const std::array<float, 2> stitchWeights = {1./2., 1./3.};


int main (int argc, char** argv)
{
  // read input file and cfg
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  if (argc < 26)
	{
	  cerr << "missing input parameters : argc is: " << argc << endl ;
	  cerr << "usage: " << argv[0]
		   << " inputFileNameList outputFileName crossSection isData configFile runHHKinFit"
		   << " xsecScale(stitch) HTMax(stitch) HTMin(stitch) isTTBar DY_tostitch TT_stitchType"
		   << " runMT2 isHHsignal NjetRequired(stitch) EFTbm order_rew uncertainty_rew cms_fake_rew kl_rew kt_rew c2_rew cg_rew c2g_rew susyModel" << endl ;

	  return 1;
	}

  std::string inputFile = argv[1] ;
  std::string outputFile = argv[2] ;
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

  assert(atoi(argv[10]) == 0 or atoi(argv[10]) == 1);
  bool isTTBar = atoi(argv[10]);
  if (!isMC) isTTBar = false; // force it, you never know...
  cout << "** INFO: is this a TTbar sample? : " << isTTBar << endl;

  bool DY_tostitch = static_cast<bool>(atoi(argv[11]));
  cout << "** INFO: DY stitching? " << DY_tostitch << endl;
  
  int TT_stitchType = atoi(argv[12]);
  if (!isTTBar) TT_stitchType = 0; // just force if not TT...
  cout << "** INFO: TT stitch type: " << TT_stitchType << " [0: no stitch , 1: fully had, 2: semilept t, 3: semilept tbar, 4: fully lept, 5: semilept all]" << endl;

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
  //int isDYI = atoi(argv[29]);

  bool isttHToNonBB = false;
  int isttHToNonBBI = atoi(argv[30]);
  if (isttHToNonBBI == 1) isttHToNonBB = true;
  cout << "** INFO: isttHToNonBB: " << isttHToNonBB << endl;

  bool isHHNLO = false;
  int isHHNLOI = atoi(argv[31]);
  if (isHHNLOI == 1) isHHNLO = true;
  cout << "** INFO: isHHNLO: " << isHHNLO << endl;

  string PERIOD = argv[32];
  assert(PERIOD=="2018" or PERIOD=="2017" or PERIOD=="2016preVFP" or PERIOD=="2016postVFP");
  bool isPostVFP = PERIOD=="2016postVFP" ? true : false;
  cout << "** INFO: PERIOD: " << PERIOD << ", isPostVFP: " << isPostVFP << endl;

  enum DataType {
    kDefault	= 0, //default, no specifity
    kMET		= 1, //MET dataset
    kSingleTau  = 2, //SingleTau dataset
  };

  int datasetType = atoi(argv[33]);
  bool isMETDataset = datasetType == DataType::kMET;
  bool isTauDataset = datasetType == DataType::kSingleTau; // currently not used
  cout << "** INFO: isMETDataset  : " << isMETDataset << endl;
  cout << "** INFO: isTauDataset  : " << isTauDataset << endl;
  if (isMC) {
	assert (datasetType==0);
  }

  // MET scale Factors
  ScaleFactorMET metSF(PERIOD);

  float tau_thresh;
  float met_thresh = metSF.getMinThreshold();
  if (PERIOD=="2016preVFP" or PERIOD=="2016postVFP") {
	tau_thresh = 130.;
  }
  else if (PERIOD=="2017") {
	tau_thresh = 190.;
  }
  else { // 2018
	tau_thresh = 190.;
  }
  
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

  //// NEW TRIGGERS
  vector<string> trigMET  =  (isMC ? gConfigParser->readStringListOption ("triggersMC::METtriggers") : gConfigParser->readStringListOption ("triggersData::METtriggers")) ;
  vector<string> trigSingleTau  =  (isMC ? gConfigParser->readStringListOption ("triggersMC::SingleTau") : gConfigParser->readStringListOption ("triggersData::SingleTau")); 

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

	  ////NEW TRIGGERS
	  cout << "  @ METtriggers" << endl; cout << "   --> ";
	  for (unsigned int i = 0 ; i < trigMET.size(); i++) cout << "  " << trigMET.at(i);
	  cout << endl;

	  cout << "  @ SingleTau" << endl; cout << "   --> ";
	  for (unsigned int i = 0 ; i < trigSingleTau.size(); i++) cout << "  " << trigSingleTau.at(i);
	  cout << endl;

	}
  
  // input and output setup
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;

  appendFromFileList (bigChain, inputFile.c_str());
  bigChain->SetCacheSize(0);
  bigTree theBigTree (bigChain) ;
  //Create a new file + a clone of old tree header. Do not copy events

  TFile * smallFile = new TFile (outputFile.c_str(), "RECREATE") ;
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

  // ------------------------------
  // systematics
  int N_jecSources = 11; //jec sources
  int N_tauhDM = 4;      //tauh DMs
  int N_tauhDM_EES = 2;  //tauh DMs with EES

  // ------------------------------
  TH1F* hTriggers = getFirstFileHisto (inputFile.c_str());
  TH1F* hTauIDS = getFirstFileHisto (inputFile.c_str(), false);

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

  ////NEW TRIGGERS
  trigReader.addMETTrigs (trigMET);
  trigReader.addSingleTauTrigs (trigSingleTau);

  // print full list (this is needed to identify the the triggers that fired in the bitwise variable)
  pair <int, int> triggerFlags = trigReader.printTriggerList(); //bitwise variable for the position of HPS triggers and tau triggers

  int isHPSTauTrigger = 0;
  int hasTau = 0;
  if (PERIOD=="2018") {
	isHPSTauTrigger = triggerFlags.first;
	hasTau = triggerFlags.second;
  }
  int isTauTrigger = hasTau & (~isHPSTauTrigger);
  cout << "isHPSTauTrigger    = " << std::bitset<16>(isHPSTauTrigger) << endl;
  cout << "hasTau             = " << std::bitset<16>(hasTau) << endl;
  cout << "isTauTrigger       = " << std::bitset<16>(isTauTrigger) << endl;
  cout << endl;
  
  // ------------------------------

  OfflineProducerHelper oph (hTriggers, hTauIDS) ;

  // ------------------------------
  //PUReweight reweight (PUReweight::RUN2ANALYSIS); // NONE : no PU reweight (always returns 1) - RUN2ANALYSIS: get weights according to MC and data targets
  PUReweight reweight (PUReweight::RUN2ANALYSIS, PUreweightFile);

  // ------------------------------
  string home = gConfigParser->readStringOption("parameters::home");
  string bTag_SFFile = home + gConfigParser->readStringOption("bTagScaleFactors::SFFileDeepFlavor");
  string bTag_effFile = home + gConfigParser->readStringOption("bTagScaleFactors::effFileDeepFlavor");
  cout << "B Tag SF file: " << bTag_SFFile << endl;
  
  string wpset, wpyear;
  if (PERIOD=="2018") {
	wpset  = "106X18_DeepFlavor_V1";
	wpyear = "2018";
  }
  else if (PERIOD=="2017") {
	wpset  = "106X17_DeepFlavor_V1";
	wpyear = "2017";
  }
  else if (PERIOD == "2016preVFP") {
	wpset = "106X16preVFP_DeepFlavor_V1";
	wpyear = "2016";
  }
  else if (PERIOD == "2016postVFP") {
	wpset = "106X16postVFP_DeepFlavor_V1";
	wpyear = "2016";
  }
  bTagSF bTagSFHelper(bTag_SFFile, bTag_effFile, "", wpyear, wpset, isMC);


  // ------------------------------
  std::string PUjetID_SF_directory = home + gConfigParser->readStringOption("PUjetIDScaleFactors::files");
  cout << "** INFO: PU jet ID SF directory: " << PUjetID_SF_directory << std::endl;
  string puYear;
  if (PERIOD=="2018") {
	puYear = "2018";
  }
  else if (PERIOD=="2017") {
	puYear = "2017";
  }
  else if (PERIOD == "2016preVFP") {
	puYear = "2016APV";
  }
  else if (PERIOD == "2016postVFP") {
	puYear = "2016";
  }
  PuJetIdSF PUjetIDSFprovider(PUjetID_SF_directory, puYear);

  // ------------------------------
  JECKLUBinterface JECprovider(wpyear, isPostVFP);

  // ------------------------------
  // JER smearing provider: year - doSmearing - variation (0:nominal  +1:up  -1:down)
  bool doSmearing    = (isMC ? gConfigParser->readBoolOption("JetSmearing::doSmearing") : false);
  int smearVariation = (isMC ? gConfigParser->readIntOption("JetSmearing::smearVariation") : 0);
  cout << "** INFO: doSmearing? " << doSmearing << endl;
  cout << "** INFO: smearVariation: " << smearVariation << " [0:nominal  +1:up  -1:down]" << endl;
  SmearedJetProducer Smearer_AK4(puYear, doSmearing, smearVariation, false);
  SmearedJetProducer Smearer_AK8(puYear, doSmearing, smearVariation, true);

  // ------------------------------

  // HHbtag model
  std::string HHbtag_model = home + gConfigParser->readStringOption("HHbtag::weights");
  cout << "** INFO: HHbtag_model: " << HHbtag_model << endl;
  std::array<std::string, 2> models;
  for(size_t n = 0; n < 2; ++n)
	{
	  std::ostringstream ss_model;
	  ss_model <<HHbtag_model << n;
	  models.at(n) = ss_model.str();
	}
  HHbtagKLUBinterface HHbtagTagger(models, std::stoi(wpyear));

  // ------------------------------
  //tau legs trigger SF for data and mc
  //from: https://github.com/cms-tau-pog/TauTriggerSFs/tree/run2_SFs
  tau_trigger::SFProvider * tauTrgSF_ditau;
  tau_trigger::SFProvider * tauTrgSF_mutau;
  tau_trigger::SFProvider * tauTrgSF_etau;
  string sf_wp = "Medium";
  if(PERIOD == "2018") {
    tauTrgSF_ditau = new tau_trigger::SFProvider("weights/trigger_SF_UL/2018/2018UL_tauTriggerEff_DeepTau2017v2p1.root", "ditau", sf_wp);
	tauTrgSF_mutau = new tau_trigger::SFProvider("weights/trigger_SF_UL/2018/2018UL_tauTriggerEff_DeepTau2017v2p1.root", "mutau", sf_wp);
	tauTrgSF_etau  = new tau_trigger::SFProvider("weights/trigger_SF_UL/2018/2018UL_tauTriggerEff_DeepTau2017v2p1.root", "etau" , sf_wp);
  }
  else if (PERIOD=="2017") {
	tauTrgSF_ditau = new tau_trigger::SFProvider("weights/trigger_SF_UL/2017/2017UL_tauTriggerEff_DeepTau2017v2p1.root", "ditau", sf_wp);
	tauTrgSF_mutau = new tau_trigger::SFProvider("weights/trigger_SF_UL/2017/2017UL_tauTriggerEff_DeepTau2017v2p1.root", "mutau", sf_wp);
	tauTrgSF_etau  = new tau_trigger::SFProvider("weights/trigger_SF_UL/2017/2017UL_tauTriggerEff_DeepTau2017v2p1.root", "etau" , sf_wp);
  }
  else if (PERIOD == "2016preVFP" or PERIOD == "2016postVFP") {
	TString vfp = isPostVFP ? "postVFP" : "preVFP";
	tauTrgSF_ditau = new tau_trigger::SFProvider("weights/trigger_SF_UL/2016"+vfp+"/2016UL"+vfp+"_tauTriggerEff_DeepTau2017v2p1.root", "ditau", "Medium");
	tauTrgSF_mutau = new tau_trigger::SFProvider("weights/trigger_SF_UL/2016"+vfp+"/2016UL"+vfp+"_tauTriggerEff_DeepTau2017v2p1.root", "mutau", "Medium");
	tauTrgSF_etau  = nullptr; //threshold higher than single lepton //new tau_trigger::SFProvider("weights/trigger_SF_Legacy/2016/2016_tauTriggerEff_DeepTau2017v2p1.root", "etau" , "Medium");
  }
	
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
  ScaleFactor* muTauTrgSF = new ScaleFactor();
  ScaleFactor* eTauTrgSF  = new ScaleFactor();
  ScaleFactor* muTrgSF    = new ScaleFactor();
  ScaleFactor* eTrgSF     = new ScaleFactor();

  TString customTrgSFDir = "weights/trigSFs_UL_eleMu/";
  if (PERIOD == "2018") {
    muTauTrgSF->init_EG_ScaleFactor(customTrgSFDir + "sf_mu_2018_HLTMu20Tau27.root", true);
    muTrgSF   ->init_ScaleFactor("weights/MuPogSF_UL/2018/Efficiencies_muon_generalTracks_Z_Run2018_UL_SingleMuonTriggers.root",
								 "NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight_abseta_pt", true);
    eTauTrgSF ->init_EG_ScaleFactor(customTrgSFDir + "sf_el_2018_HLTEle24Tau30.root", true);
    eTrgSF    ->init_EG_ScaleFactor(customTrgSFDir + "sf_el_2018_HLTEle32.root", true);
  }
  else if (PERIOD == "2017") {
    muTauTrgSF->init_EG_ScaleFactor(customTrgSFDir + "sf_mu_2017_HLTMu20Tau27.root", true);
    muTrgSF   ->init_ScaleFactor("weights/MuPogSF_UL/2017/Efficiencies_muon_generalTracks_Z_Run2017_UL_SingleMuonTriggers.root",
								 "NUM_IsoMu27_DEN_CutBasedIdTight_and_PFIsoTight_abseta_pt", true);
    eTauTrgSF ->init_EG_ScaleFactor(customTrgSFDir + "sf_el_2017_HLTEle24Tau30.root", true);
    eTrgSF    ->init_EG_ScaleFactor(customTrgSFDir + "sf_el_2017_HLTEle32.root", true);
  }
  else if (PERIOD == "2016preVFP") {
    muTauTrgSF->init_EG_ScaleFactor(customTrgSFDir + "sf_mu_2016pre_HLTMu20Tau27.root", true);
    muTrgSF   ->init_ScaleFactor("weights/MuPogSF_UL/2016/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_SingleMuonTriggers.root",
								 "NUM_IsoMu24_or_IsoTkMu24_DEN_CutBasedIdTight_and_PFIsoTight_abseta_pt", true);
    eTrgSF    ->init_EG_ScaleFactor(customTrgSFDir + "sf_el_2016pre_HLTEle25.root", true);
  }
  else if (PERIOD == "2016postVFP") {
    muTauTrgSF->init_EG_ScaleFactor(customTrgSFDir + "sf_mu_2016post_HLTMu20Tau27.root", true);
    muTrgSF   ->init_ScaleFactor("weights/MuPogSF_UL/2016/Efficiencies_muon_generalTracks_Z_Run2016_UL_SingleMuonTriggers.root",
								 "NUM_IsoMu24_or_IsoTkMu24_DEN_CutBasedIdTight_and_PFIsoTight_abseta_pt", true);
    eTrgSF    ->init_EG_ScaleFactor(customTrgSFDir + "sf_el_2016post_HLTEle25.root", true);
  }
    
  // electron/muon IdAndIso SF
  ScaleFactor * myIDandISOScaleFactor[3]; // [0: muID, 1: eleID, 2:muISO,]
  for (int i=0; i<3; i++) {
    myIDandISOScaleFactor[i] = new ScaleFactor();
  }
  if (PERIOD == "2018") {
	myIDandISOScaleFactor[0]->init_ScaleFactor("weights/MuPogSF_UL/2018/Efficiencies_muon_generalTracks_Z_Run2018_UL_ID.root",
                                               "NUM_TightID_DEN_TrackerMuons_abseta_pt", true);
	myIDandISOScaleFactor[1] -> init_EG_ScaleFactor("weights/EgammaPOGSF_UL/2018/egammaEffi.txt_Ele_wp80iso_EGM2D.root", false);
	myIDandISOScaleFactor[2] -> init_ScaleFactor("weights/MuPogSF_UL/2018/Efficiencies_muon_generalTracks_Z_Run2018_UL_ISO.root",
												 "NUM_TightRelIso_DEN_TightIDandIPCut_abseta_pt", true);
  }
  else if (PERIOD == "2017") {
	myIDandISOScaleFactor[0] -> init_ScaleFactor("weights/MuPogSF_UL/2017/Efficiencies_muon_generalTracks_Z_Run2017_UL_ID.root",
												 "NUM_TightID_DEN_TrackerMuons_abseta_pt", true);
	myIDandISOScaleFactor[1] -> init_EG_ScaleFactor("weights/EgammaPOGSF_UL/2017/egammaEffi.txt_EGM2D_MVA80iso_UL17.root", false);
	myIDandISOScaleFactor[2] -> init_ScaleFactor("weights/MuPogSF_UL/2017/Efficiencies_muon_generalTracks_Z_Run2017_UL_ISO.root",
												 "NUM_TightRelIso_DEN_TightIDandIPCut_abseta_pt", true);
  }
  else if (PERIOD == "2016preVFP") {
	myIDandISOScaleFactor[0] -> init_ScaleFactor("weights/MuPogSF_UL/2016/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_ID.root",
												 "NUM_TightID_DEN_TrackerMuons_abseta_pt", true);
	myIDandISOScaleFactor[1] -> init_EG_ScaleFactor("weights/EgammaPOGSF_UL/2016/egammaEffi.txt_Ele_wp80iso_preVFP_EGM2D.root",false);
	myIDandISOScaleFactor[2] -> init_ScaleFactor("weights/MuPogSF_UL/2016/Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_ISO.root",
												 "NUM_TightRelIso_DEN_TightIDandIPCut_abseta_pt", true);
  }
  else if (PERIOD == "2016postVFP") {
	myIDandISOScaleFactor[0] -> init_ScaleFactor("weights/MuPogSF_UL/2016/Efficiencies_muon_generalTracks_Z_Run2016_UL_ID.root",
												 "NUM_TightID_DEN_TrackerMuons_abseta_pt", true);
	myIDandISOScaleFactor[1] -> init_EG_ScaleFactor("weights/EgammaPOGSF_UL/2016/egammaEffi.txt_Ele_wp80iso_postVFP_EGM2D.root", false);
	myIDandISOScaleFactor[2] -> init_ScaleFactor("weights/MuPogSF_UL/2016/Efficiencies_muon_generalTracks_Z_Run2016_UL_ISO.root",
												 "NUM_TightRelIso_DEN_TightIDandIPCut_abseta_pt", true);
  }
  
  // tau IdAndIso SF
  string tauidsf_period;
  if (PERIOD == "2018") {
	tauidsf_period = "UL2018";
  }
  else if (PERIOD == "2017") {
	tauidsf_period = "UL2017";
  }
  else if (PERIOD == "2016preVFP") {
	tauidsf_period = "UL2016_preVFP";
  }
  else if (PERIOD == "2016postVFP") {
	tauidsf_period = "UL2016_postVFP";
  }
  TauIDSFTool * Deep_antiJet_2d		   = new TauIDSFTool(tauidsf_period, "Medium","VVLoose"); // for DeepTauv2p1 vsJets Medium and vsElectrons VVLoose in DM and pT bins
  TauIDSFTool * Deep_antiEle_vvloose   = new TauIDSFTool(tauidsf_period, "DeepTau2017v2p1VSe","VVLoose",0);  // for DeepTauv2p1 vs ele VVLoose
  TauIDSFTool * Deep_antiMu_tight	   = new TauIDSFTool(tauidsf_period, "DeepTau2017v2p1VSmu","Tight",0);	  // for DeepTauv2p1 vs mu Tight

  // ------------------------------
  // reweight file according to NLO differential reweighting procedure
  // there is a unique input map, read it from the cfg file
  // HHReweight* hhreweighter = nullptr;
  HHReweight5D* hhreweighter = nullptr;
  TH2* hhreweighterInputMap = nullptr;
  if (HHrewType != kNone)
	{
	  string inMapFile   = home + gConfigParser->readStringOption("HHReweight::inputFile");
	  string inHistoName = gConfigParser->readStringOption("HHReweight::histoName");
	  string coeffFile   = home + gConfigParser->readStringOption("HHReweight::coeffFileNLO");
	  if (order_rew == string("lo")) {
		coeffFile = home + gConfigParser->readStringOption("HHReweight::coeffFileLO");
	  }
	  cout << "** INFO: reading histo named: " << inHistoName << " from file: " << inMapFile << endl;
	  cout << "** INFO: HH reweight coefficient file is: " << coeffFile << endl;
	  TFile* fHHDiffRew = new TFile(inMapFile.c_str());
	  hhreweighterInputMap = (TH2*) fHHDiffRew->Get(inHistoName.c_str());
	  hhreweighter = new HHReweight5D(coeffFile, hhreweighterInputMap, EFTbm, string(wpyear), order_rew, uncertainty_rew, cms_fake_rew);
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
	  cout << "** WARNING!! did not find some cut-based tau IDs" << endl;
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
	  cout << "** WARNING!! did not find some cut-based tau IDs" << endl;
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
	  cout << "** WARNING!! did not find some cut-based tau IDs" << endl;
	  for (unsigned int i = 0; i < deepTauVsMuIdx.size(); ++i)
		cout << deepTauVsMuIdx.at(i) << " " ;
	  cout << endl;
	}

  // event counters for efficiency study
  EffCounter ec;
  ec.AddMarker("all");
  ec.AddMarker("METfilter");
  ec.AddMarker("NoBadMuons");
  ec.AddMarker("PairExists");
  ec.AddMarker("PairFoundBaseline");
  ec.AddMarker("Trigger");
  ec.AddMarker("TwoJets");

  std::map<std::string, int> channels = {{"MuTau",  0},
										 {"ETau",   1},
										 {"TauTau", 2},
										 {"MuMu",   3},
										 {"EE",     4},
										 {"EMu",    5}};

  // event counters for acceptance x efficiency plots
  EffCounter eAccEff;
  if (isHHsignal) {
	for(auto const& chn : channels) {
	  eAccEff.AddMarker(chn.first);
	  eAccEff.AddMarker(chn.first + "_legacy");
	  eAccEff.AddMarker(chn.first + "_singletau");
	  eAccEff.AddMarker(chn.first + "_met");
	}
  }

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

  const float eleEtaMax = oph.getEtaCut("ele");
  const float muEtaMax  = oph.getEtaCut("mu");
  
  // loop over events
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  for (Long64_t iEvent = 0 ; true ; ++iEvent)
	{
	  if (iEvent % 10000 == 0)  cout << "- reading event " << iEvent << endl ;
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

	  float stitchWeight = 1.;
	  if (DY_tostitch)
		{
		  float Zpt = theBigTree.lheVPt;
		  if (Zpt==0.0)
			stitchWeight = stitchWeights[0];
		  else
			stitchWeight = stitchWeights[1];
		}

	  // Should never enter here (DY_tostitch should be always true)
	  if (!DY_tostitch && DY_nJets >= 0)
		{
		  int njets = theBigTree.lheNOutPartons;
		  int nb    = theBigTree.lheNOutB;
		  if (njets != DY_nJets || nb != DY_nBJets) continue;
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

	  // For ttHToNonBB only: loop on gen particles to find the Higgs
	  // if the Higgs decays to TauTau --> throw away the event!
	  if (isttHToNonBB)
		{
		  // Loop on gen to find Higgs
		  int idx1 = -1;
		  for (unsigned int igen = 0; igen < theBigTree.genpart_px->size(); igen++)
			{
			  bool isLast = CheckBit (theBigTree.genpart_flags->at(igen), 13) ; // 13 = isLastCopy

			  int pdg = theBigTree.genpart_pdg->at(igen);
			  if (abs(pdg) == 25)
				{
				  if (isLast)
					{
					  if (idx1 >= 0) {
						cout << "** WARNING: ttH - more than 1 H identified" << endl;
						continue;
					  }
					  else {
						idx1 = igen;
					  }
					}
				}
			} // end loop on gen part to find H from ttH

		  // Find decay mode of the Higgs
		  if (idx1 == -1) {
			cout << "** WARNING: ttH - couldn't find 1 H" << endl;
			continue;
		  }
		  else {
			int ttHdecayMode = theBigTree.genpart_HZDecayMode->at(idx1);
			if (ttHdecayMode <= 6) {
			  if (DEBUG) cout << "** ttH stitcher: found ttH->TauTau event with decay mode: " << ttHdecayMode << " --> rejecting event!" << endl;
			  continue;
			}
		  }
		} // end ttHToNonBB only

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


			  if (abs(pdg) == 25)
				{
				  // cout << igen << " H boson: Px " << theBigTree.genpart_px->at(igen) << " first? " << isFirst << " decMode : " << theBigTree.genpart_HZDecayMode->at(igen) << endl;
				  if (isFirst)
					{
					  if (idx1 >= 0 && idx2 >= 0) {
						cout << "** WARNING: more than 2 H identified (first)" << endl;
						continue;
					  }
					  (idx1 == -1) ? (idx1 = igen) : (idx2 = igen) ;
					}
				  if (isLast)
					{
					  if (idx1last >= 0 && idx2last >= 0) {
						cout << "** WARNING: more than 2 H identified (last)" << endl;
						// continue; // no need to skip the event in this case -- dec mode just for studies
					  }
					  (idx1last == -1) ? (idx1last = igen) : (idx2last = igen) ;
					}
				}

			  if ((abs(pdg) == 11 || abs(pdg) == 13 ) && isHardScatt && isLast && mothIsHardScatt)
				{
				  if (idx1hs == -1) idx1hs = igen;
				  else if (idx2hs == -1) idx2hs = igen;
				  else {
					cout << "** WARNING: there are more than 2 hard scatter tau dec prod: evt = " << theBigTree.EventNumber << endl;
				  }
				}
			  
			  if (abs(pdg) == 66615 && mothIsHardScatt)
				{
				  if (idx1hs == -1) idx1hs = igen;
				  else if (idx2hs == -1) idx2hs = igen;
				  else {
					cout << "** WARNING: there are more than 2 hard scatter tau dec prod: evt = " << theBigTree.EventNumber << endl;
				  }
				}

			  // FRA DEBUG - find the bjets from the Higgs decay
			  if ( abs(pdg) == 5 && isHardProcess)
				{
				  if (idx1hs_b == -1) idx1hs_b = igen;
				  else if (idx2hs_b == -1) idx2hs_b = igen;
				  else {
					cout << "** WARNING: there are more than 2 hard scatter b quarks: evt = " << theBigTree.EventNumber << endl;
				  }
				}

			}
		
		  if (idx1 == -1 || idx2 == -1) {
			cout << "** WARNING: couldn't find 2 H (first)" << endl;
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
		  else {
			cout << "** WARNING: couldn't find 2 H (last)" << endl;
		  }

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

			  if (oph.getPairType(t1hs, t2hs) != (theSmallTree.m_genDecMode1 + theSmallTree.m_genDecMode2 - 8)) {
				cout << "** WARNING: decay modes do not match! " << theBigTree.genpart_pdg->at(idx1hs) << " " << theBigTree.genpart_pdg->at(idx2hs) << " != "
					 << ( theSmallTree.m_genDecMode1 + theSmallTree.m_genDecMode2 - 8) << endl;
			  }
			  vHardScatter1.SetPxPyPzE (theBigTree.genpart_px->at(idx1hs), theBigTree.genpart_py->at(idx1hs), theBigTree.genpart_pz->at(idx1hs), theBigTree.genpart_e->at(idx1hs));
			  vHardScatter2.SetPxPyPzE (theBigTree.genpart_px->at(idx2hs), theBigTree.genpart_py->at(idx2hs), theBigTree.genpart_pz->at(idx2hs), theBigTree.genpart_e->at(idx2hs));
			}
		  else {
			cout << "** WARNING: couldn't find 2 H->tautau gen dec prod " << idx1hs << " " << idx2hs << endl;
		  }

		  vH1.SetPxPyPzE (theBigTree.genpart_px->at(idx1), theBigTree.genpart_py->at(idx1), theBigTree.genpart_pz->at(idx1), theBigTree.genpart_e->at(idx1) );
		  vH2.SetPxPyPzE (theBigTree.genpart_px->at(idx2), theBigTree.genpart_py->at(idx2), theBigTree.genpart_pz->at(idx2), theBigTree.genpart_e->at(idx2) );
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
		  else {
			cout << "** WARNING: couldn't find 2 H->bb gen dec prod " << idx1hs_b << " " << idx2hs_b << endl;
		  }

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
		  cout << "** WARNING: negative dec mode, for safety set it ot 0" << endl;
		}
	  double EvtW;
	  if (theBigTree.npu >= 0 && theBigTree.npu <= 99) { // good PU weights
		EvtW = isMC ? (theBigTree.aMCatNLOweight * reweight.weight(PUReweight_MC, PUReweight_target, theBigTree.npu, PUreweightFile) * topPtReweight * HHweight) : 1.0;
	  }
	  else if (theBigTree.npu >= 100) {                  // use the last available bin for the PU weight
		EvtW = isMC ? (theBigTree.aMCatNLOweight * reweight.weight(PUReweight_MC, PUReweight_target, 99, PUreweightFile) * topPtReweight * HHweight) : 1.0;
	  }
	  else {                                             // if npu<0 --> bug in MC --> weight=0
		EvtW = isMC ? 0.0 : 1.0;
	  }

	  if (isMC)	{
		totalEvents += EvtW;
	  }
	  else {
		totalEvents += 1 ;
	  }

	  ec.Increment("all", EvtW);
	  if (isHHsignal) {
		ecHHsig[genHHDecMode].Increment("all", EvtW);
	  }

	  ++totalNoWeightsEventsNum;


	  // ----------------------------------------------------------
	  //  apply MET filters

	  int metbit = theBigTree.metfilterbit;

	  // Twiki: https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2#MET_Filter_Recommendations_for_R
	  int metpass = (metbit & (1 << 0)) ? 1 : 0; //"Flag_goodVertices"
	  metpass    += (metbit & (1 << 1)) ? 1 : 0; //"Flag_HBHENoiseFilter"
	  metpass    += (metbit & (1 << 2)) ? 1 : 0; //"Flag_HBHENoiseIsoFilter"
	  metpass    += (metbit & (1 << 3)) ? 1 : 0; //"Flag_EcalDeadCellTriggerPrimitiveFilter"
	  metpass    += (metbit & (1 << 4)) ? 1 : 0; //"Flag_globalSuperTightHalo2016Filter"
	  metpass    += (metbit & (1 << 5)) ? 1 : 0; //"Flag_BadPFMuonFilter"
	  metpass    += (metbit & (1 << 6)) ? 1 : 0; //"Flag_eeBadScFilter"
	  if(PERIOD == "2018" or PERIOD == "2017") {
		metpass    += (metbit & (1 << 7)) ? 1 : 0; //"Flag_ecalBadCalibFilter"
	  }
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
		  cout << "Flag_ecalBadCalibFilter                : " << (metbit & (1 << 7)) << endl;
		  cout << "Flag_BadPFMuonDzFilter                 : " << (metbit & (1 << 8)) << endl;
		}

	  int metpass_thresh;
	  if(PERIOD == "2018" or PERIOD == "2017") {
		metpass_thresh = 9;
	  }
	  if(PERIOD == "2016preVFP" or PERIOD == "2016postVFP") {
		metpass_thresh = 8;
	  }
	  if(metpass < metpass_thresh) continue ;

	  ec.Increment("METfilter", EvtW);
	  if (isHHsignal) {
		ecHHsig[genHHDecMode].Increment ("METfilter", EvtW);
	  }

	  // ----------------------------------------------------------
	  // require that the event is not affected by the Bad/Clone Muon problem -- for 2016 data
	  //if (theBigTree.NBadMu > 0) continue ; //FRA: Sync Feb2018
	  ec.Increment("NoBadMuons", EvtW);
	  if (isHHsignal) {
		ecHHsig[genHHDecMode].Increment ("NoBadMuons", EvtW);
	  }

	  // ----------------------------------------------------------
	  // require at least 1 pair
	  if (theBigTree.indexDau1->size () == 0) continue ;
	  ec.Increment("PairExists", EvtW);
	  if (isHHsignal) {
		ecHHsig[genHHDecMode].Increment ("PairExists", EvtW);
	  }

	  // ----------------------------------------------------------
	  // assess the pair type
	  // loop over the daughters to select pair type: mu > e > tau
	  // apply tight baseline (with iso to check)

	  int nmu = 0;
	  int nmu10 = 0; // low pt muons for DY sideband, not entering in nmu
	  int nele = 0;
	  int nele10 = 0;

	  if(DEBUG)	{
		cout << "***** DEBUG: reco particles (remember: check if baseline sels are aligned to OfflineProducerHelper)" << endl;
	  }

	  for (unsigned int idau = 0; idau < theBigTree.daughters_px->size(); ++idau)
		{
		  int dauType = theBigTree.particleType->at(idau);
		  if (oph.isMuon(dauType))
			{
			  bool passMu   = oph.muBaseline (&theBigTree, idau, 15., muEtaMax,
											  0.15, OfflineProducerHelper::MuTight,
											  0.15, OfflineProducerHelper::MuHighPt, string("All"), (DEBUG ? true : false));
			  bool passMu10 = oph.muBaseline (&theBigTree, idau, 15., muEtaMax,
											  0.30, OfflineProducerHelper::MuTight,
											  0.30, OfflineProducerHelper::MuHighPt, string("All"), (DEBUG ? true : false));

			  if (passMu) ++nmu;
			  else if (passMu10) ++nmu10;
			}
		  else if (oph.isElectron(dauType))
			{
			  bool passEle   = oph.eleBaseline (&theBigTree, idau, 10., eleEtaMax, 0.1,
												OfflineProducerHelper::EMVATight, string("Vertex-LepID-pTMin-etaMax"), (DEBUG ? true : false));
			  bool passEle10 = oph.eleBaseline (&theBigTree, idau, 10., eleEtaMax, 0.3,
												OfflineProducerHelper::EMVATight, string("Vertex-LepID-pTMin-etaMax"), (DEBUG ? true : false));

			  if (passEle) ++nele;
			  else if (passEle10) ++nele10;
			}

		  if(DEBUG)	{
			TLorentzVector dauTlvDebug(theBigTree.daughters_px->at (idau),
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
	  if (nmu > 0) {
		if (nmu == 1 && nmu10 == 0) {
		  pairType = 0 ; // mu tau
		}
		else {
		  pairType = 3 ; // mu mu
		}
	  }
	  else if (nele > 0) {
		if (nele == 1 && nele10 == 0) {
		  pairType = 1;  // ele tau
		}
		else {
		  pairType = 4 ; // ele ele
		}
	  }

	  if (isHHsignal) {
		for(auto const& chn : channels) {
		  if (chn.second == pairType) {
			eAccEff.Increment(chn.first, EvtW);
		  }
		}
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
	  if (isHHsignal) {
		ecHHsig[genHHDecMode].Increment ("PairFoundBaseline", EvtW);
		if (pairType == genHHDecMode)
		  ecHHsig[genHHDecMode].Increment ("PairMatchesGen", EvtW);
	  }

	  TMatrixD metcov (2, 2);
	  metcov(0,0) = theBigTree.MET_cov00->at(chosenTauPair);
	  metcov(1,0) = theBigTree.MET_cov10->at(chosenTauPair);
	  metcov(0,1) = theBigTree.MET_cov01->at(chosenTauPair);
	  metcov(1,1) = theBigTree.MET_cov11->at(chosenTauPair);

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
	  if(pairType == 0) //mutauh
		{
		  theSmallTree.m_dau1_decayMode = -1;
		  theSmallTree.m_dau2_decayMode = DM2;
		}

	  if(pairType == 1) //etauh
		{
		  theSmallTree.m_dau1_decayMode = -1;
		  theSmallTree.m_dau2_decayMode = DM2;
		}

	  if(pairType == 2) //tauhtauh
		{
		  theSmallTree.m_dau1_decayMode = DM1;
		  theSmallTree.m_dau2_decayMode = DM2;
		}

	  if(pairType > 2) //pairs without tauh
		{
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

	  const TLorentzVector tlv_firstLepton (theBigTree.daughters_px->at (firstDaughterIndex),
											theBigTree.daughters_py->at (firstDaughterIndex),
											theBigTree.daughters_pz->at (firstDaughterIndex),
											theBigTree.daughters_e->at (firstDaughterIndex)
											);

	  const TLorentzVector tlv_secondLepton (theBigTree.daughters_px->at (secondDaughterIndex),
											 theBigTree.daughters_py->at (secondDaughterIndex),
											 theBigTree.daughters_pz->at (secondDaughterIndex),
											 theBigTree.daughters_e->at (secondDaughterIndex)
											 );

	  auto met_phi_corr = met_phi_correction_pxpy(
												  theBigTree.METx->at(chosenTauPair),
												  theBigTree.METy->at(chosenTauPair),
												  theBigTree.npv, theBigTree.RunNumber, PERIOD, isMC
												  );
	  TLorentzVector tlv_MET;
	  tlv_MET.SetPxPyPzE(met_phi_corr.first, met_phi_corr.second,
						 0, std::hypot(met_phi_corr.first, met_phi_corr.second));
	  TVector2 vMET(tlv_MET.Px(), tlv_MET.Py());
	  TVector2 vMUON(0., 0.);
	  if (pairType==0) {
		// single muon in evt, vetoing events with 3rd lepton
		vMUON.Set(tlv_firstLepton.Px(), tlv_firstLepton.Py());
	  }
	  else if(pairType==3) {
		// two muons in evt (looser cuts), vetoing events with 3rd lepton
		vMUON.Set(tlv_firstLepton.Px()+tlv_secondLepton.Px(),
				  tlv_firstLepton.Py()+tlv_secondLepton.Py());
	  }
	  TVector2 vMETnoMu = vMET + vMUON;

	  // MHT(noMu) variables. This might be a quick&dirty definition though, as not removing soft contributions
	  // mht: transverse component of jet momentum sum (here MET(tot)-MET(leptons))
	  TVector2 vLEP(tlv_firstLepton.Px()+tlv_secondLepton.Px(),
					tlv_firstLepton.Py()+tlv_secondLepton.Py()); // no matter what lepton type
	  TVector2 vMHT = vMET + vLEP;
	  // mhtnomu: ~ as defined in HLT code
	  // https://github.com/cms-sw/cmssw/blob/6d2f66057131baacc2fcbdd203588c41c885b42c/HLTrigger/JetMET/src/HLTHtMhtProducer.cc#L76
	  // > transverse component of jets momentum sum minus muons momentum sum (I still need to figure this one out...)
	  TVector2 vMHTnoMu = vMHT+vMUON;
	  
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
	  vector<bool> isthisDM_first = {
		(theSmallTree.m_dau1_decayMode == 0? true :  false),
		(theSmallTree.m_dau1_decayMode == 1? true :  false),
		(theSmallTree.m_dau1_decayMode == 10? true :  false),
		(theSmallTree.m_dau1_decayMode == 11? true :  false)
	  };

	  vector<bool> isthisDM_second = {
		(theSmallTree.m_dau2_decayMode == 0? true :  false),
		(theSmallTree.m_dau2_decayMode == 1? true :  false),
		(theSmallTree.m_dau2_decayMode == 10? true :  false),
		(theSmallTree.m_dau2_decayMode == 11? true :  false)
	  };

	  // loop over DMs to fill the shifted TLVs
	  for (int idm  = 0; idm < N_tauhDM; idm ++)
		{
		  if (lep1HasTES)
			{
			  tlv_firstLepton_tauup[idm]   = getShiftedDau(tlv_firstLepton, 1.,  unc_TES_first[idm], isthisDM_first[idm], (idm != 0));  // no mass shift for DM == 0 (idm == 0)
			  tlv_firstLepton_taudown[idm] = getShiftedDau(tlv_firstLepton, -1., unc_TES_first[idm], isthisDM_first[idm], (idm != 0));  // no mass shift for DM == 0 (idm == 0)
			}
		  else if(lep1HasEES && idm < N_tauhDM_EES)
			{
			  tlv_firstLepton_eleup[idm]   = getShiftedDau(tlv_firstLepton, 1.,  unc_EESup_first[idm], isthisDM_first[idm]);
			  tlv_firstLepton_eledown[idm] = getShiftedDau(tlv_firstLepton, -1., unc_EESdw_first[idm], isthisDM_first[idm]);
			}
		  if (lep2HasTES)
			{
			  tlv_secondLepton_tauup[idm]   = getShiftedDau(tlv_secondLepton, 1.,  unc_TES_second[idm], isthisDM_second[idm], (idm != 0));  // no mass shift for DM == 0 (idm == 0)
			  tlv_secondLepton_taudown[idm] = getShiftedDau(tlv_secondLepton, -1., unc_TES_second[idm], isthisDM_second[idm], (idm != 0));  // no mass shift for DM == 0 (idm == 0)
			}
		  else if(lep2HasEES && idm < N_tauhDM_EES)
			{
			  tlv_secondLepton_eleup[idm]   = getShiftedDau(tlv_secondLepton, 1.,  unc_EESup_second[idm], isthisDM_second[idm]);
			  tlv_secondLepton_eledown[idm] = getShiftedDau(tlv_secondLepton, -1., unc_EESdw_second[idm], isthisDM_second[idm]);
			}
		} // end loop over DMs

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

	  theSmallTree.m_dau1_pt_muup   = tlv_firstLepton_muup.Pt();
	  theSmallTree.m_dau1_pt_mudown	= tlv_firstLepton_mudown.Pt();
	  theSmallTree.m_dau1_e_muup	= tlv_firstLepton_muup.E();
	  theSmallTree.m_dau1_e_mudown	= tlv_firstLepton_mudown.E();
	  theSmallTree.m_dau2_pt_muup	= tlv_secondLepton_muup.Pt();
	  theSmallTree.m_dau2_pt_mudown	= tlv_secondLepton_mudown.Pt();
	  theSmallTree.m_dau2_e_muup	= tlv_secondLepton_muup.E();
	  theSmallTree.m_dau2_e_mudown	= tlv_secondLepton_mudown.E();

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

	  // Twiki (UL SFs not available as of September 2023)
	  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/TauTrigger#Run_II_Trigger_Scale_Factors
	  std::unordered_map<std::string, std::pair<float,float>> singleTauSF = {
		{"2016preVFP",  std::make_pair(0.88, 0.08)},
		{"2016postVFP", std::make_pair(0.88, 0.08)},
		{"2017",        std::make_pair(1.08, 0.10)},
		{"2018",        std::make_pair(0.87, 0.11)}};

	  TriggerAssigner trgAssigner;
	  std::unordered_map<std::string,bool> trgRegions;
	  
	  if (applyTriggers)
		{
		  Long64_t triggerbit = theBigTree.triggerbit;

		  Long64_t matchFlag1 = (Long64_t) theBigTree.daughters_trgMatched->at(firstDaughterIndex);
		  Long64_t matchFlag2 = (Long64_t) theBigTree.daughters_trgMatched->at(secondDaughterIndex);
		  Long64_t goodTriggerType1 = (Long64_t) theBigTree.daughters_isGoodTriggerType->at(firstDaughterIndex);
		  Long64_t goodTriggerType2 = (Long64_t) theBigTree.daughters_isGoodTriggerType->at(secondDaughterIndex);

		  Long64_t trgNotOverlapFlag = (Long64_t) theBigTree.mothers_trgSeparateMatch->at(chosenTauPair);
		  passTrg = trigReader.checkOR (pairType, triggerbit, &pass_triggerbit,
										matchFlag1, matchFlag2, trgNotOverlapFlag,
										goodTriggerType1, goodTriggerType2,
										tlv_firstLepton.Pt(), tlv_firstLepton.Eta(),
										tlv_secondLepton.Pt(), tlv_secondLepton.Eta()) ; // check only lepton triggers

		  passMETTrg = trigReader.checkMET(triggerbit, &pass_triggerbit, vMETnoMu.Mod(), met_thresh);

		  passSingleTau = trigReader.checkSingleTau(triggerbit, matchFlag1, matchFlag2, trgNotOverlapFlag,
													goodTriggerType1, goodTriggerType2,
													tlv_firstLepton.Pt(), tlv_firstLepton.Eta(),
													tlv_secondLepton.Pt(), tlv_secondLepton.Eta(), &pass_triggerbit);

		  if (PERIOD=="2018" and !isMC and passTrg)
			{
			  if(theBigTree.RunNumber < 317509)
				{
				  if (DEBUG)
					{
					  cout << "@@@ Run in period without HPS triggers "<<endl;
					  cout << "isHPSTauTrigger = " << std::bitset<16>(isHPSTauTrigger) << endl;
					  cout << "pass_triggerbit = " << std::bitset<16>(pass_triggerbit) << endl;
					}
				  if ((pass_triggerbit & (~ isHPSTauTrigger)) == 0)
					{
					  if (DEBUG) cout << "only HPS trigger fired, reject"<<endl;
					  passTrg = false;
					}
				}
			  else
				{
				  if (DEBUG)
					{
					  cout << "@@@ Run in period with HPS triggers "<<endl;
					  cout << "isHPSTauTrigger = " << std::bitset<16>(isHPSTauTrigger) << endl;
					  cout << "isTauTrigger =    " << std::bitset<16>(isTauTrigger) << endl;
					  cout << "pass_triggerbit = " << std::bitset<16>(pass_triggerbit) << endl;
					}
				  if ((pass_triggerbit & (~ isTauTrigger)) == 0) // not HPS but still Tau trigger -> VBF?
					{
					  if (DEBUG) cout << "only non-HPS trigger fired, reject"<<endl;
					  passTrg = false;
					}
				}
			}

		  trgRegions = trgAssigner.assignTriggerRegions(pairType, PERIOD,
														tlv_firstLepton.Pt(), tlv_secondLepton.Pt(),
														fabs(tlv_firstLepton.Eta()),
														fabs(tlv_secondLepton.Eta()),
														eleEtaMax, muEtaMax, tau_thresh);

		  bool legacyAccept    = passTrg       and trgRegions["legacy"];
		  bool metAccept       = passMETTrg    and trgRegions["met"]; 
		  bool singletauAccept = passSingleTau and trgRegions["tau"];
		  if (!isMC) {
			legacyAccept    = legacyAccept    and !isMETDataset;
			metAccept       = metAccept       and isMETDataset;
			singletauAccept = singletauAccept and !isMETDataset;
		  }
		  bool triggerAccept = legacyAccept or metAccept or singletauAccept;

		  if (isHHsignal) {
			for(auto const& chn : channels) {
			  if (chn.second == pairType) {
				if (legacyAccept) {
				  eAccEff.Increment(chn.first + "_legacy", EvtW);
				}
				else if (metAccept) {
				  eAccEff.Increment(chn.first + "_met", EvtW);
				}
				else if (singletauAccept) {
				  eAccEff.Increment(chn.first + "_singletau", EvtW);
				}
			  }
			}
		  }

		  if(DEBUG)
			{
			  if(pairType == 0) { //MuTau
				trigReader.listMuTau(triggerbit, matchFlag1, matchFlag2, trgNotOverlapFlag, goodTriggerType1, goodTriggerType2);
			  }
			  if(pairType == 1) { //ETau
				trigReader.listETau(triggerbit, matchFlag1, matchFlag2, trgNotOverlapFlag, goodTriggerType1, goodTriggerType2);
			  }
			  if(pairType == 2) { //TauTau
				trigReader.listTauTau(triggerbit, matchFlag1, matchFlag2, trgNotOverlapFlag, goodTriggerType1, goodTriggerType2);
			  }
			}

		  if (DEBUG) std::cout << "----> Final Trigger passed? " << triggerAccept << std::endl;
		  if (!triggerAccept) continue;
		  
		  theSmallTree.m_pass_triggerbit = pass_triggerbit;
		  ec.Increment ("Trigger", EvtW); // for data, EvtW is 1.0
		  if (isHHsignal && pairType == genHHDecMode) {
			ecHHsig[genHHDecMode].Increment ("Trigger", EvtW);
		  }

		  theSmallTree.m_isLeptrigger = passTrg;
		  theSmallTree.m_isMETtrigger = passMETTrg;
		  theSmallTree.m_isSingleTautrigger = passSingleTau;
		} // end if applyTriggers

      // ----------------------------------------------------------
      // pair selection is now complete, compute oher quantitites

      // If skimming the final ntuples skip not interesting channels
	  if (onlyFinalChannels && pairType>3) continue;

	  // First create a map to associate <jetIdx, smearFactor> that will be
	  // used to smear the jets always in the same way in the event.
	  // Since the smearing is by definition a random process the smearing factors of
	  // the jets must be computed only one time per event.
	  std::map<int,double> smears_AK4, smears_AK8;
	  for (unsigned int iJet = 0 ; iJet < theBigTree.jets_px->size () ; ++iJet)
		{
		  // Build jet tlv
		  TLorentzVector tlv_jet(theBigTree.jets_px->at(iJet), theBigTree.jets_py->at(iJet), theBigTree.jets_pz->at(iJet), theBigTree.jets_e->at(iJet)) ;

		  // Get the smearFactor
		  double smearFactor = Smearer_AK4.getSmearFactor(tlv_jet, theBigTree, true);

		  // Store the smearFacotr in the map
		  if (doSmearing)
			smears_AK4[iJet] = smearFactor;
		  else
			smears_AK4[iJet] = 1.;
		}

	  if (DEBUG)
		{
		  std::cout << "---------- SmearMap DEBUG ----------" << std::endl;
		  for (auto el : smears_AK4) std::cout << "Element: " << el.first << "  " << el.second << std::endl;
		  std::cout << "------------------------------------" << std::endl;
		}

	  // Compute quantities
	  TLorentzVector tlv_tauH = tlv_firstLepton + tlv_secondLepton ;
	  TLorentzVector tlv_tauH_SVFIT ;

	  // METnoMu,MHT,MHTnoMu for trigger study
	  /*
		Most basic implementation accounting only for muons passing selection cuts.
		Would we need to include soft muons somehow as well?
		(3rd lepton veto already looks for soft leptons (>10 GeV))
	  */
	  TLorentzVector tlv_DeepMET_ResponseTune;
	  tlv_DeepMET_ResponseTune.SetPtEtaPhiM(theBigTree.ShiftedDeepMETresponseTune_pt, 0, theBigTree.ShiftedDeepMETresponseTune_phi, 0);
	  TVector2 vDeepMET_ResponseTune(tlv_DeepMET_ResponseTune.Px(), tlv_DeepMET_ResponseTune.Py());

	  TLorentzVector tlv_DeepMET_ResolutionTune;
	  tlv_DeepMET_ResolutionTune.SetPtEtaPhiM(theBigTree.ShiftedDeepMETresolutionTune_pt, 0, theBigTree.ShiftedDeepMETresolutionTune_phi, 0);
	  TVector2 vDeepMET_ResolutionTune(tlv_DeepMET_ResolutionTune.Px(), tlv_DeepMET_ResolutionTune.Py());

	  // Temporary SVFit recomputation for data.
	  if(!isMC){
		SVfitKLUBinterface algo_tmp(0, tlv_firstLepton, tlv_secondLepton, tlv_MET, metcov, pType, theSmallTree.m_dau1_decayMode, theSmallTree.m_dau2_decayMode);
		std::vector<double> svfitResTmp = algo_tmp.FitAndGetResult();
		theSmallTree.m_tauH_SVFIT_mass = svfitResTmp.at(3);
		if (theSmallTree.m_tauH_SVFIT_mass > 0)
		  {
			theSmallTree.m_tauH_SVFIT_pt   = svfitResTmp.at(0);
			theSmallTree.m_tauH_SVFIT_eta  = svfitResTmp.at(1);
			theSmallTree.m_tauH_SVFIT_phi  = svfitResTmp.at(2);
			tlv_tauH_SVFIT.SetPtEtaPhiM(svfitResTmp.at(0), svfitResTmp.at(1), svfitResTmp.at(2), svfitResTmp.at(3));

			theSmallTree.m_tauHsvfitMet_deltaPhi = deltaPhi(vMET.Phi(), tlv_tauH_SVFIT.Phi ()) ;
		  }
	  }
	  //theSmallTree.m_tauH_SVFIT_mass = theBigTree.SVfitMass->at (chosenTauPair) ;

	  if (doSmearing)
		{
		  // Smear MET
		  float METx = met_phi_corr.first;
		  float METy = met_phi_corr.second;
		  TVector2 metSmeared = getShiftedMET_smear(METx, METy, theBigTree, smears_AK4);
		  tlv_MET.SetPxPyPzE(metSmeared.Px(), metSmeared.Py(), 0, std::hypot(metSmeared.Px(), metSmeared.Py()));
		  vMET.Set(tlv_MET.Px(), tlv_MET.Py());

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
			}
		}

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
		}

	  if (DEBUG)
		{
		  cout << "------- SVFIT ------" << endl;
		  cout << " is calculated ? " << theBigTree.SVfitMass->at(chosenTauPair) << endl;
		  cout << " pt/eta/phi: " << tlv_tauH_SVFIT.Pt() << " / " << tlv_tauH_SVFIT.Eta() << " / " << tlv_tauH_SVFIT.Phi() << endl;
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
	  theSmallTree.m_METx      = vMET.Px();
	  theSmallTree.m_METy      = vMET.Py();
	  theSmallTree.m_met_cov00 = theBigTree.MET_cov00->at (chosenTauPair);
	  theSmallTree.m_met_cov01 = theBigTree.MET_cov01->at (chosenTauPair);
	  theSmallTree.m_met_cov10 = theBigTree.MET_cov10->at (chosenTauPair);
	  theSmallTree.m_met_cov11 = theBigTree.MET_cov11->at (chosenTauPair);
	  theSmallTree.m_DeepMET_ResponseTune_px = vDeepMET_ResponseTune.X();
	  theSmallTree.m_DeepMET_ResponseTune_py = vDeepMET_ResponseTune.Y();
	  theSmallTree.m_DeepMET_ResolutionTune_px = vDeepMET_ResolutionTune.X();
	  theSmallTree.m_DeepMET_ResolutionTune_py = vDeepMET_ResolutionTune.Y();

	  theSmallTree.m_metnomu_phi   = vMETnoMu.Phi();
	  theSmallTree.m_metnomu_et    = vMETnoMu.Mod();
	  theSmallTree.m_METnoMux      = vMETnoMu.X();
	  theSmallTree.m_METnoMuy      = vMETnoMu.Y();
	  theSmallTree.m_mht_phi   = vMHT.Phi();
	  theSmallTree.m_mht_et    = vMHT.Mod();
	  theSmallTree.m_MHTx      = vMHT.X();
	  theSmallTree.m_MHTy      = vMHT.Y();
	  theSmallTree.m_mhtnomu_phi   = vMHTnoMu.Phi();
	  theSmallTree.m_mhtnomu_et    = vMHTnoMu.Mod();
	  theSmallTree.m_MHTnoMux      = vMHTnoMu.X();
	  theSmallTree.m_MHTnoMuy      = vMHTnoMu.Y();
	
	  // L1ECALPrefiringWeight - https://twiki.cern.ch/twiki/bin/viewauth/CMS/L1ECALPrefiringWeightRecipe
	  theSmallTree.m_L1pref_weight      = theBigTree.prefiringweight;
	  theSmallTree.m_L1pref_weight_up   = theBigTree.prefiringweightup;
	  theSmallTree.m_L1pref_weight_down = theBigTree.prefiringweightdown;
	
	  if (DEBUG)
		{
		  cout << "------- MET DEBUG -------" << endl;
		  cout << " met centr : " << theSmallTree.m_met_et << " / " << theSmallTree.m_met_phi << endl;
		  cout << "-------------------------" << endl;
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

	  theSmallTree.m_dau1_iso = getIso (firstDaughterIndex, tlv_firstLepton.Pt (), theBigTree) ;
	  theSmallTree.m_dau1_eleMVAiso = theBigTree.daughters_iseleWP80->at(firstDaughterIndex) ? 1 : 0;
	  theSmallTree.m_dau1_deepTauVsJet = makeIsoDiscr (firstDaughterIndex, deepTauVsJetIdx , theBigTree) ;
	  theSmallTree.m_dau1_deepTauVsEle = makeIsoDiscr (firstDaughterIndex, deepTauVsEleIdx , theBigTree) ;
	  theSmallTree.m_dau1_deepTauVsMu = makeIsoDiscr (firstDaughterIndex, deepTauVsMuIdx , theBigTree) ;

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

	  theSmallTree.m_dau1_pt_tauup_DM0    = tlv_firstLepton_tauup[0].Pt();
	  theSmallTree.m_dau1_pt_tauup_DM1	  = tlv_firstLepton_tauup[1].Pt();
	  theSmallTree.m_dau1_pt_tauup_DM10	  = tlv_firstLepton_tauup[2].Pt();
	  theSmallTree.m_dau1_pt_tauup_DM11	  = tlv_firstLepton_tauup[3].Pt();
	  theSmallTree.m_dau1_pt_taudown_DM0  = tlv_firstLepton_taudown[0].Pt();
	  theSmallTree.m_dau1_pt_taudown_DM1  = tlv_firstLepton_taudown[1].Pt();
	  theSmallTree.m_dau1_pt_taudown_DM10 = tlv_firstLepton_taudown[2].Pt();
	  theSmallTree.m_dau1_pt_taudown_DM11 = tlv_firstLepton_taudown[3].Pt();
	  theSmallTree.m_dau1_e_tauup_DM0     = tlv_firstLepton_tauup[0].E();
	  theSmallTree.m_dau1_e_tauup_DM1	  = tlv_firstLepton_tauup[1].E();
	  theSmallTree.m_dau1_e_tauup_DM10	  = tlv_firstLepton_tauup[2].E();
	  theSmallTree.m_dau1_e_tauup_DM11	  = tlv_firstLepton_tauup[3].E();
	  theSmallTree.m_dau1_e_taudown_DM0   = tlv_firstLepton_taudown[0].E();
	  theSmallTree.m_dau1_e_taudown_DM1   = tlv_firstLepton_taudown[1].E();
	  theSmallTree.m_dau1_e_taudown_DM10  = tlv_firstLepton_taudown[2].E();
	  theSmallTree.m_dau1_e_taudown_DM11  = tlv_firstLepton_taudown[3].E();

	  theSmallTree.m_dau1_pt_eleup_DM0 = tlv_firstLepton_eleup[0].Pt();
	  theSmallTree.m_dau1_pt_eleup_DM1 = tlv_firstLepton_eleup[1].Pt();
	  theSmallTree.m_dau1_e_eleup_DM0  = tlv_firstLepton_eleup[0].E();
	  theSmallTree.m_dau1_e_eleup_DM1  = tlv_firstLepton_eleup[1].E();

	  theSmallTree.m_dau1_pt_eledown_DM0 = tlv_firstLepton_eledown[0].Pt();
	  theSmallTree.m_dau1_pt_eledown_DM1 = tlv_firstLepton_eledown[1].Pt();
	  theSmallTree.m_dau1_e_eledown_DM0  = tlv_firstLepton_eledown[0].E();
	  theSmallTree.m_dau1_e_eledown_DM1  = tlv_firstLepton_eledown[1].E();

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
	  theSmallTree.m_dau2_deepTauVsJet = makeIsoDiscr (secondDaughterIndex, deepTauVsJetIdx , theBigTree) ;
	  theSmallTree.m_dau2_deepTauVsEle = makeIsoDiscr (secondDaughterIndex, deepTauVsEleIdx , theBigTree) ;
	  theSmallTree.m_dau2_deepTauVsMu = makeIsoDiscr (secondDaughterIndex, deepTauVsMuIdx , theBigTree) ;
	  theSmallTree.m_dau2_pt = tlv_secondLepton.Pt () ;

	  theSmallTree.m_dau2_pt_tauup_DM0    = tlv_secondLepton_tauup[0].Pt();
	  theSmallTree.m_dau2_pt_tauup_DM1	  = tlv_secondLepton_tauup[1].Pt();
	  theSmallTree.m_dau2_pt_tauup_DM10	  = tlv_secondLepton_tauup[2].Pt();
	  theSmallTree.m_dau2_pt_tauup_DM11	  = tlv_secondLepton_tauup[3].Pt();
	  theSmallTree.m_dau2_pt_taudown_DM0  = tlv_secondLepton_taudown[0].Pt();
	  theSmallTree.m_dau2_pt_taudown_DM1  = tlv_secondLepton_taudown[1].Pt();
	  theSmallTree.m_dau2_pt_taudown_DM10 = tlv_secondLepton_taudown[2].Pt();
	  theSmallTree.m_dau2_pt_taudown_DM11 = tlv_secondLepton_taudown[3].Pt();
	  theSmallTree.m_dau2_e_tauup_DM0     = tlv_secondLepton_tauup[0].E();
	  theSmallTree.m_dau2_e_tauup_DM1	  = tlv_secondLepton_tauup[1].E();
	  theSmallTree.m_dau2_e_tauup_DM10	  = tlv_secondLepton_tauup[2].E();
	  theSmallTree.m_dau2_e_tauup_DM11	  = tlv_secondLepton_tauup[3].E();
	  theSmallTree.m_dau2_e_taudown_DM0   = tlv_secondLepton_taudown[0].E();
	  theSmallTree.m_dau2_e_taudown_DM1   = tlv_secondLepton_taudown[1].E();
	  theSmallTree.m_dau2_e_taudown_DM10  = tlv_secondLepton_taudown[2].E();
	  theSmallTree.m_dau2_e_taudown_DM11  = tlv_secondLepton_taudown[3].E();

	  theSmallTree.m_dau2_pt_eleup_DM0 = tlv_secondLepton_eleup[0].Pt();
	  theSmallTree.m_dau2_pt_eleup_DM1 = tlv_secondLepton_eleup[1].Pt();
	  theSmallTree.m_dau2_e_eleup_DM0  = tlv_secondLepton_eleup[0].E();
	  theSmallTree.m_dau2_e_eleup_DM1  = tlv_secondLepton_eleup[1].E();

	  theSmallTree.m_dau2_pt_eledown_DM0 = tlv_secondLepton_eledown[0].Pt();
	  theSmallTree.m_dau2_pt_eledown_DM1 = tlv_secondLepton_eledown[1].Pt();
	  theSmallTree.m_dau2_e_eledown_DM0 = tlv_secondLepton_eledown[0].E();
	  theSmallTree.m_dau2_e_eledown_DM1 = tlv_secondLepton_eledown[1].E();

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
	  float idSF_deep_2d		= -99.0;  // use this for DeepTauV2p1 DM and pT dependent
	  float idFakeSF_deep_2d	= -99.0;	// use this for DeepTauV2p1 pt dependent + e/mu->tauh fake SF
	  float fakeRateSF_deep		= -99.0;  // use this for e/mu->tauh fake SF DeepTau

	  float idFakeSF_tauid_2d_stat0_DM0_up				= -99.f;
	  float idFakeSF_tauid_2d_stat0_DM0_down			= -99.f;				  
	  float idFakeSF_tauid_2d_stat1_DM0_up				= -99.f;					
	  float idFakeSF_tauid_2d_stat1_DM0_down			= -99.f;				  
	  float idFakeSF_tauid_2d_systuncorrdmeras_DM0_up	= -99.f;
	  float idFakeSF_tauid_2d_systuncorrdmeras_DM0_down	= -99.f;

	  float idFakeSF_tauid_2d_stat0_DM1_up				= -99.f;
	  float idFakeSF_tauid_2d_stat0_DM1_down			= -99.f;				  
	  float idFakeSF_tauid_2d_stat1_DM1_up				= -99.f;					
	  float idFakeSF_tauid_2d_stat1_DM1_down			= -99.f;				  
	  float idFakeSF_tauid_2d_systuncorrdmeras_DM1_up   = -99.f;
	  float idFakeSF_tauid_2d_systuncorrdmeras_DM1_down	= -99.f;	   

	  float idFakeSF_tauid_2d_stat0_DM10_up				 = -99.f;
	  float idFakeSF_tauid_2d_stat0_DM10_down			 = -99.f;				  
	  float idFakeSF_tauid_2d_stat1_DM10_up				 = -99.f;					
	  float idFakeSF_tauid_2d_stat1_DM10_down			 = -99.f;				  
	  float idFakeSF_tauid_2d_systuncorrdmeras_DM10_up   = -99.f;
	  float idFakeSF_tauid_2d_systuncorrdmeras_DM10_down = -99.f;	   

	  float idFakeSF_tauid_2d_stat0_DM11_up				 = -99.f;
	  float idFakeSF_tauid_2d_stat0_DM11_down			 = -99.f;				  
	  float idFakeSF_tauid_2d_stat1_DM11_up				 = -99.f;					
	  float idFakeSF_tauid_2d_stat1_DM11_down			 = -99.f;				  
	  float idFakeSF_tauid_2d_systuncorrdmeras_DM11_up   = -99.f;
	  float idFakeSF_tauid_2d_systuncorrdmeras_DM11_down = -99.f;	   

	  float idFakeSF_tauid_2d_systcorrdmeras_up			= -99.f;		   
	  float idFakeSF_tauid_2d_systcorrdmeras_down		= -99.f;		 
	  float idFakeSF_tauid_2d_systcorrdmuncorreras_up	= -99.f;	 
	  float idFakeSF_tauid_2d_systcorrdmuncorreras_down	= -99.f; 
	  float idFakeSF_tauid_2d_systcorrerasgt140_up      = -99.f;
	  float idFakeSF_tauid_2d_systcorrerasgt140_down	= -99.f;
	  float idFakeSF_tauid_2d_stat0gt140_up				= -99.f;
	  float idFakeSF_tauid_2d_stat0gt140_down			= -99.f;
	  float idFakeSF_tauid_2d_stat1gt140_up				= -99.f;
	  float idFakeSF_tauid_2d_stat1gt140_down			= -99.f;
	  float idFakeSF_tauid_2d_extrapgt140_up     		= -99.f;
	  float idFakeSF_tauid_2d_extrapgt140_down     		= -99.f;
	
	  float idFakeSF_mutauFR_etaLt0p4_up      = -99.0;
	  float idFakeSF_mutauFR_eta0p4to0p8_up   = -99.0;
	  float idFakeSF_mutauFR_eta0p8to1p2_up   = -99.0;
	  float idFakeSF_mutauFR_eta1p2to1p7_up   = -99.0;
	  float idFakeSF_mutauFR_etaGt1p7_up      = -99.0;
	  float idFakeSF_mutauFR_etaLt0p4_down    = -99.0;
	  float idFakeSF_mutauFR_eta0p4to0p8_down = -99.0;
	  float idFakeSF_mutauFR_eta0p8to1p2_down = -99.0;
	  float idFakeSF_mutauFR_eta1p2to1p7_down = -99.0;
	  float idFakeSF_mutauFR_etaGt1p7_down    = -99.0;

	  float idFakeSF_etauFR_barrel_up   = -99.0;
	  float idFakeSF_etauFR_endcap_up   = -99.0;
	  float idFakeSF_etauFR_barrel_down = -99.0;
	  float idFakeSF_etauFR_endcap_down = -99.0;

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
	  int tau1DM  = static_cast<int>(theSmallTree.m_dau1_decayMode);
	  int tau2DM  = static_cast<int>(theSmallTree.m_dau2_decayMode);
	  float tau1Genmatch = theBigTree.genmatch->at(firstDaughterIndex);
	  float tau2Genmatch = theBigTree.genmatch->at(secondDaughterIndex);

	  float idSF_leg1				= -99.f;
	  float idSF_leg1_deep_vsJet_2d = -99.f;
	  float idSF_leg1_deep_vsEle    = -99.f;
	  float idSF_leg1_deep_vsMu     = -99.f;

	  float idSF_leg2				= -99.f;
	  float idSF_leg2_deep_vsJet_2d = -99.f;
	  float idSF_leg2_deep_vsEle	= -99.f;
	  float idSF_leg2_deep_vsMu		= -99.f;

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
		  idSF_leg1_deep_vsJet_2d = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch);
		  idSF_leg1_deep_vsEle    = Deep_antiEle_vvloose->getSFvsEta(leg1eta, tau1Genmatch);
		  idSF_leg1_deep_vsMu     = Deep_antiMu_tight->getSFvsEta(leg1eta, tau1Genmatch);
		}

	  // all channels with one tau in the second leg
	  if (isMC and pType<3)
		{
		  idSF_leg2_deep_vsJet_2d = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch);
		  idSF_leg2_deep_vsEle = Deep_antiEle_vvloose->getSFvsEta(leg2eta, tau2Genmatch);
		  idSF_leg2_deep_vsMu  = Deep_antiMu_tight->getSFvsEta(leg2eta, tau2Genmatch);
		}
	  
	  Float_t idSF_leg1_deep_vsJet_2d_stat0_DM0_up				= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_stat0_DM0_down			= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_stat1_DM0_up				= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_stat1_DM0_down			= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM0_up	= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM0_down	= 1.f;

	  Float_t idSF_leg1_deep_vsJet_2d_stat0_DM1_up				= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_stat0_DM1_down			= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_stat1_DM1_up				= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_stat1_DM1_down			= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM1_up	= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM1_down	= 1.f;

	  Float_t idSF_leg1_deep_vsJet_2d_stat0_DM10_up				 = 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_stat0_DM10_down			 = 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_stat1_DM10_up				 = 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_stat1_DM10_down			 = 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM10_up	 = 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM10_down = 1.f;

	  Float_t idSF_leg1_deep_vsJet_2d_stat0_DM11_up				 = 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_stat0_DM11_down			 = 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_stat1_DM11_up				 = 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_stat1_DM11_down			 = 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM11_up	 = 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM11_down = 1.f;

	  Float_t idSF_leg1_deep_vsJet_2d_systcorrdmeras_up			= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_systcorrdmeras_down		= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_systcorrdmuncorreras_up	= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_systcorrdmuncorreras_down = 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_systcorrerasgt140_up	    = 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_systcorrerasgt140_down	= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_stat0gt140_up				= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_stat0gt140_down			= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_stat1gt140_up				= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_stat1gt140_down			= 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_extrapgt140_up            = 1.f;
	  Float_t idSF_leg1_deep_vsJet_2d_extrapgt140_down          = 1.f;
		  
	  Float_t idSF_leg2_deep_vsJet_2d_stat0_DM0_up				= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_stat0_DM0_down			= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_stat1_DM0_up				= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_stat1_DM0_down			= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM0_up	= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM0_down	= 1.f;

	  Float_t idSF_leg2_deep_vsJet_2d_stat0_DM1_up				= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_stat0_DM1_down			= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_stat1_DM1_up				= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_stat1_DM1_down			= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM1_up	= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM1_down	= 1.f;

	  Float_t idSF_leg2_deep_vsJet_2d_stat0_DM10_up				 = 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_stat0_DM10_down			 = 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_stat1_DM10_up				 = 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_stat1_DM10_down			 = 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM10_up	 = 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM10_down = 1.f;

	  Float_t idSF_leg2_deep_vsJet_2d_stat0_DM11_up				 = 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_stat0_DM11_down			 = 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_stat1_DM11_up				 = 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_stat1_DM11_down			 = 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM11_up	 = 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM11_down = 1.f;

	  Float_t idSF_leg2_deep_vsJet_2d_systcorrdmeras_up			= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_systcorrdmeras_down		= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_systcorrdmuncorreras_up	= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_systcorrdmuncorreras_down = 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_systcorrerasgt140_up	    = 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_systcorrerasgt140_down	= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_stat0gt140_up				= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_stat0gt140_down			= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_stat1gt140_up				= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_stat1gt140_down			= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_extrapgt140_up			= 1.f;
	  Float_t idSF_leg2_deep_vsJet_2d_extrapgt140_down			= 1.f;
	  
	  // up and down variations of the ID and isolation of the first leg (only relevant when it is a tau)
	  vector<float> idSF_leg1_deep_vsEle_up      (2, idSF_leg1_deep_vsEle);    // in bins of eta: barrel, endcap
	  vector<float> idSF_leg1_deep_vsMu_up       (5, idSF_leg1_deep_vsMu);     // in bins of eta, edges at 0, 0.4, 0.8, 1.2, 1.7, infty
	  vector<float> idSF_leg1_deep_vsEle_down    (2, idSF_leg1_deep_vsEle);    // in bins of eta: barrel, endcap
	  vector<float> idSF_leg1_deep_vsMu_down     (5, idSF_leg1_deep_vsMu);     // in bins of eta, edges at 0, 0.4, 0.8, 1.2, 1.7, infty
	  
	  // only TauTau has a tau as the first leg
	  if (isMC and pType==2) {	
		for (int bin = 0; bin < (int) isthisEta_IDbin_first.size(); bin++) {
		  if (isthisEta_IDbin_first[bin])
			{
			  idSF_leg1_deep_vsMu_up[bin]   = Deep_antiMu_tight->getSFvsEta(leg1eta, tau1Genmatch,   "Up");
			  idSF_leg1_deep_vsMu_down[bin] = Deep_antiMu_tight->getSFvsEta(leg1eta, tau1Genmatch, "Down");
			}
		}

		for (int bin = 0; bin < (int) isthisSDet_IDbin_first.size(); bin++) {
		  if (isthisSDet_IDbin_first[bin])
			{
			  idSF_leg1_deep_vsEle_up[bin]   = Deep_antiEle_vvloose ->getSFvsEta(leg1eta, tau1Genmatch,   "Up");
			  idSF_leg1_deep_vsEle_down[bin] = Deep_antiEle_vvloose ->getSFvsEta(leg1eta, tau1Genmatch, "Down");
			}
		}

		switch (tau1DM) {
		case 0:
		  idSF_leg1_deep_vsJet_2d_stat0_DM0_up       = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat0Up");
		  idSF_leg1_deep_vsJet_2d_stat0_DM0_down     = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat0Down");
		  idSF_leg1_deep_vsJet_2d_stat1_DM0_up       = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat1Up");
		  idSF_leg1_deep_vsJet_2d_stat1_DM0_down     = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat1Down");
		  idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM0_up   = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystUncorrDMErasUp");
		  idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM0_down = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystUncorrDMErasDown");
		  break;
		case 1:
		  idSF_leg1_deep_vsJet_2d_stat0_DM1_up       = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat0Up");
		  idSF_leg1_deep_vsJet_2d_stat0_DM1_down     = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat0Down");
		  idSF_leg1_deep_vsJet_2d_stat1_DM1_up       = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat1Up");
		  idSF_leg1_deep_vsJet_2d_stat1_DM1_down	 = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat1Down");
		  idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM1_up   = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystUncorrDMErasUp");
		  idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM1_down = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystUncorrDMErasDown");
		  break;
		case 10:
		  idSF_leg1_deep_vsJet_2d_stat0_DM10_up       = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat0Up");
		  idSF_leg1_deep_vsJet_2d_stat0_DM10_down     = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat0Down");
		  idSF_leg1_deep_vsJet_2d_stat1_DM10_up       = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat1Up");
		  idSF_leg1_deep_vsJet_2d_stat1_DM10_down     = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat1Down");
		  idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM10_up   = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystUncorrDMErasUp");
		  idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM10_down = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystUncorrDMErasDown");
		  break;
		case 11:
		  idSF_leg1_deep_vsJet_2d_stat0_DM11_up       = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat0Up");
		  idSF_leg1_deep_vsJet_2d_stat0_DM11_down     = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat0Down");
		  idSF_leg1_deep_vsJet_2d_stat1_DM11_up       = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat1Up");
		  idSF_leg1_deep_vsJet_2d_stat1_DM11_down     = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Stat1Down");
		  idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM11_up   = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystUncorrDMErasUp");
		  idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM11_down = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystUncorrDMErasDown");
		  break;
		}

		if (leg1pt > 140. and leg1pt <= 200.) {
		  idSF_leg1_deep_vsJet_2d_stat0gt140_up   = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Gt140StatUp");
		  idSF_leg1_deep_vsJet_2d_stat0gt140_down = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Gt140StatDown");
		}
		else if (leg1pt > 200.) {
		  idSF_leg1_deep_vsJet_2d_stat1gt140_up   = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Gt140StatUp");
		  idSF_leg1_deep_vsJet_2d_stat1gt140_down = Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Gt140StatDown");
		}
		else { // set to the nominal values
		  idSF_leg1_deep_vsJet_2d_stat1gt140_up   = idSF_leg1_deep_vsJet_2d;
		  idSF_leg1_deep_vsJet_2d_stat1gt140_down = idSF_leg1_deep_vsJet_2d;
		}

		idSF_leg1_deep_vsJet_2d_systcorrdmeras_up			= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystCorrDMErasUp");
		idSF_leg1_deep_vsJet_2d_systcorrdmeras_down			= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystCorrDMErasDown");
		idSF_leg1_deep_vsJet_2d_systcorrdmuncorreras_up		= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystCorrDMUncorrErasUp");
		idSF_leg1_deep_vsJet_2d_systcorrdmuncorreras_down	= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "SystCorrDMUncorrErasDown");
		idSF_leg1_deep_vsJet_2d_systcorrerasgt140_up		= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Gt140SystCorrErasUp");
		idSF_leg1_deep_vsJet_2d_systcorrerasgt140_down		= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Gt140SystCorrErasDown");
		idSF_leg1_deep_vsJet_2d_extrapgt140_up				= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Gt140ExtrapUp");
		idSF_leg1_deep_vsJet_2d_extrapgt140_down			= Deep_antiJet_2d->getSFvsDMandPT(leg1pt, tau1DM, tau1Genmatch, "Gt140ExtrapDown");
	  }
	  
	  // up and down variations of the ID and isolation of the second leg (only relevant when it is a tau)
	  vector<float> idSF_leg2_deep_vsEle_up      (2, idSF_leg2_deep_vsEle);    // in bins of eta: barrel, endcap
	  vector<float> idSF_leg2_deep_vsMu_up       (5, idSF_leg2_deep_vsMu);     // in bins of eta, edges at 0, 0.4, 0.8, 1.2, 1.7, infty
	  vector<float> idSF_leg2_deep_vsEle_down    (2, idSF_leg2_deep_vsEle);    // in bins of eta: barrel, endcap
	  vector<float> idSF_leg2_deep_vsMu_down     (5, idSF_leg2_deep_vsMu);     // in bins of eta, edges at 0, 0.4, 0.8, 1.2, 1.7, infty

	  if (isMC and pType<3) {
		for (int bin = 0; bin < (int) isthisEta_IDbin_second.size(); bin++) {
		  if (isthisEta_IDbin_second[bin]) {
			idSF_leg2_deep_vsMu_up[bin]   = Deep_antiMu_tight->getSFvsEta(leg2eta, tau2Genmatch,	"Up");
			idSF_leg2_deep_vsMu_down[bin] = Deep_antiMu_tight->getSFvsEta(leg2eta, tau2Genmatch, "Down");
		  }
		}

		for (int bin = 0; bin < (int) isthisSDet_IDbin_second.size(); bin++) {
		  if (isthisSDet_IDbin_second[bin]) {
			idSF_leg2_deep_vsEle_up[bin]   = Deep_antiEle_vvloose ->getSFvsEta(leg2eta, tau2Genmatch,   "Up");
			idSF_leg2_deep_vsEle_down[bin] = Deep_antiEle_vvloose ->getSFvsEta(leg2eta, tau2Genmatch, "Down");
		  }
		}

		switch (tau2DM) {
		case 0:
		  idSF_leg2_deep_vsJet_2d_stat0_DM0_up       = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat0Up");
		  idSF_leg2_deep_vsJet_2d_stat0_DM0_down     = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat0Down");
		  idSF_leg2_deep_vsJet_2d_stat1_DM0_up       = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat1Up");
		  idSF_leg2_deep_vsJet_2d_stat1_DM0_down     = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat1Down");
		  idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM0_up   = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystUncorrDMErasUp");
		  idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM0_down = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystUncorrDMErasDown");
		  break;
		case 1:
		  idSF_leg2_deep_vsJet_2d_stat0_DM1_up       = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat0Up");
		  idSF_leg2_deep_vsJet_2d_stat0_DM1_down     = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat0Down");
		  idSF_leg2_deep_vsJet_2d_stat1_DM1_up       = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat1Up");
		  idSF_leg2_deep_vsJet_2d_stat1_DM1_down	 = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat1Down");
		  idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM1_up   = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystUncorrDMErasUp");
		  idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM1_down = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystUncorrDMErasDown");
		  break;
		case 10:
		  idSF_leg2_deep_vsJet_2d_stat0_DM10_up       = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat0Up");
		  idSF_leg2_deep_vsJet_2d_stat0_DM10_down     = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat0Down");
		  idSF_leg2_deep_vsJet_2d_stat1_DM10_up       = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat1Up");
		  idSF_leg2_deep_vsJet_2d_stat1_DM10_down     = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat1Down");
		  idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM10_up   = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystUncorrDMErasUp");
		  idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM10_down = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystUncorrDMErasDown");
		  break;
		case 11:
		  idSF_leg2_deep_vsJet_2d_stat0_DM11_up       = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat0Up");
		  idSF_leg2_deep_vsJet_2d_stat0_DM11_down     = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat0Down");
		  idSF_leg2_deep_vsJet_2d_stat1_DM11_up       = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat1Up");
		  idSF_leg2_deep_vsJet_2d_stat1_DM11_down     = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Stat1Down");
		  idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM11_up   = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystUncorrDMErasUp");
		  idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM11_down = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystUncorrDMErasDown");
		  break;
		}

		if (leg2pt > 140. and leg2pt <= 200.) {
		  idSF_leg2_deep_vsJet_2d_stat0gt140_up   = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Gt140StatUp");
		  idSF_leg2_deep_vsJet_2d_stat0gt140_down = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Gt140StatDown");
		}
		else if (leg2pt > 200.) {
		  idSF_leg2_deep_vsJet_2d_stat1gt140_up   = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Gt140StatUp");
		  idSF_leg2_deep_vsJet_2d_stat1gt140_down = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Gt140StatDown");
		}
		else { // set to the nominal values
		  idSF_leg2_deep_vsJet_2d_stat1gt140_up   = idSF_leg2_deep_vsJet_2d;
		  idSF_leg2_deep_vsJet_2d_stat1gt140_down = idSF_leg2_deep_vsJet_2d;
		}

		idSF_leg2_deep_vsJet_2d_systcorrdmeras_up		  = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystCorrDMErasUp");
		idSF_leg2_deep_vsJet_2d_systcorrdmeras_down		  = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystCorrDMErasDown");
		idSF_leg2_deep_vsJet_2d_systcorrdmuncorreras_up	  = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystCorrDMUncorrErasUp");
		idSF_leg2_deep_vsJet_2d_systcorrdmuncorreras_down = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "SystCorrDMUncorrErasDown");
		idSF_leg2_deep_vsJet_2d_systcorrerasgt140_up	  = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Gt140SystCorrErasUp");
		idSF_leg2_deep_vsJet_2d_systcorrerasgt140_down	  = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Gt140SystCorrErasDown");
		idSF_leg2_deep_vsJet_2d_extrapgt140_up            = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Gt140ExtrapUp");
		idSF_leg2_deep_vsJet_2d_extrapgt140_down          = Deep_antiJet_2d->getSFvsDMandPT(leg2pt, tau2DM, tau2Genmatch, "Gt140ExtrapDown");
	  }
	
	  if (isMC) {
		if (pType == 0 or pType == 3) {
		  idSF_leg1 = myIDandISOScaleFactor[0]->get_ScaleFactor(leg1pt, fabs(leg1eta), pType) * myIDandISOScaleFactor[2]->get_ScaleFactor(leg1pt, fabs(leg1eta), pType);
		}
		else if (pType == 1 or pType == 4) {
		  idSF_leg1 = myIDandISOScaleFactor[1]->get_direct_ScaleFactor(leg1pt, leg1eta, pType);
		}
	  }

	  if(isMC and pType == 3) { //MuMu
		idSF_leg2 = myIDandISOScaleFactor[0]->get_ScaleFactor(leg2pt, fabs(leg2eta), pType) * myIDandISOScaleFactor[2]->get_ScaleFactor(leg2pt, fabs(leg2eta), pType);
	  }
	  else if(isMC and pType == 4) { //EleEle
		idSF_leg2 = myIDandISOScaleFactor[1]->get_direct_ScaleFactor(leg2pt, leg2eta, pType);
	  }

	  float except_VsJet, except_vsMu, except_vsEle;
	  // EleTau and MuTau
	  if (isMC and (pType == 0 or pType == 1))
		{
		  idSF_deep_2d = idSF_leg1 * idSF_leg2_deep_vsJet_2d;
		  fakeRateSF_deep = idSF_leg2_deep_vsEle * idSF_leg2_deep_vsMu;

		  except_VsJet = idSF_leg1 * idSF_leg2_deep_vsEle    * idSF_leg2_deep_vsMu;
		  except_vsMu  = idSF_leg1 * idSF_leg2_deep_vsJet_2d * idSF_leg2_deep_vsEle;
		  except_vsEle = idSF_leg1 * idSF_leg2_deep_vsJet_2d * idSF_leg2_deep_vsMu;
		
		  idFakeSF_deep_2d = except_VsJet * idSF_leg2_deep_vsJet_2d;

		  idFakeSF_tauid_2d_stat0_DM0_up				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat0_DM0_up;
		  idFakeSF_tauid_2d_stat0_DM0_down				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat0_DM0_down;
		  idFakeSF_tauid_2d_stat1_DM0_up				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat1_DM0_up;
		  idFakeSF_tauid_2d_stat1_DM0_down				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat1_DM0_down;
		  idFakeSF_tauid_2d_systuncorrdmeras_DM0_up		= except_VsJet * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM0_up;
		  idFakeSF_tauid_2d_systuncorrdmeras_DM0_down	= except_VsJet * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM0_down;

		  idFakeSF_tauid_2d_stat0_DM1_up				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat0_DM1_up;
		  idFakeSF_tauid_2d_stat0_DM1_down				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat0_DM1_down;
		  idFakeSF_tauid_2d_stat1_DM1_up				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat1_DM1_up;
		  idFakeSF_tauid_2d_stat1_DM1_down				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat1_DM1_down;
		  idFakeSF_tauid_2d_systuncorrdmeras_DM1_up		= except_VsJet * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM1_up;
		  idFakeSF_tauid_2d_systuncorrdmeras_DM1_down	= except_VsJet * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM1_down;

		  idFakeSF_tauid_2d_stat0_DM10_up				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat0_DM10_up;
		  idFakeSF_tauid_2d_stat0_DM10_down				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat0_DM10_down;
		  idFakeSF_tauid_2d_stat1_DM10_up				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat1_DM10_up;
		  idFakeSF_tauid_2d_stat1_DM10_down				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat1_DM10_down;
		  idFakeSF_tauid_2d_systuncorrdmeras_DM10_up	= except_VsJet * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM10_up;
		  idFakeSF_tauid_2d_systuncorrdmeras_DM10_down	= except_VsJet * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM10_down;

		  idFakeSF_tauid_2d_stat0_DM11_up				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat0_DM11_up;
		  idFakeSF_tauid_2d_stat0_DM11_down				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat0_DM11_down;
		  idFakeSF_tauid_2d_stat1_DM11_up				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat1_DM11_up;
		  idFakeSF_tauid_2d_stat1_DM11_down				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat1_DM11_down;
		  idFakeSF_tauid_2d_systuncorrdmeras_DM11_up	= except_VsJet * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM11_up;
		  idFakeSF_tauid_2d_systuncorrdmeras_DM11_down	= except_VsJet * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM11_down;

		  idFakeSF_tauid_2d_systcorrdmeras_up			= except_VsJet * idSF_leg2_deep_vsJet_2d_systcorrdmeras_up;
		  idFakeSF_tauid_2d_systcorrdmeras_down			= except_VsJet * idSF_leg2_deep_vsJet_2d_systcorrdmeras_down;
		  idFakeSF_tauid_2d_systcorrdmuncorreras_up		= except_VsJet * idSF_leg2_deep_vsJet_2d_systcorrdmuncorreras_up;
		  idFakeSF_tauid_2d_systcorrdmuncorreras_down	= except_VsJet * idSF_leg2_deep_vsJet_2d_systcorrdmuncorreras_down;
		  idFakeSF_tauid_2d_systcorrerasgt140_up		= except_VsJet * idSF_leg2_deep_vsJet_2d_systcorrerasgt140_up;
		  idFakeSF_tauid_2d_systcorrerasgt140_down   	= except_VsJet * idSF_leg2_deep_vsJet_2d_systcorrerasgt140_down;
		  idFakeSF_tauid_2d_stat0gt140_up				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat0gt140_up;
		  idFakeSF_tauid_2d_stat0gt140_down				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat0gt140_down;
		  idFakeSF_tauid_2d_stat1gt140_up				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat1gt140_up;
		  idFakeSF_tauid_2d_stat1gt140_down				= except_VsJet * idSF_leg2_deep_vsJet_2d_stat1gt140_down;
		  idFakeSF_tauid_2d_extrapgt140_up 		        = except_VsJet * idSF_leg2_deep_vsJet_2d_extrapgt140_up;
		  idFakeSF_tauid_2d_extrapgt140_down	        = except_VsJet * idSF_leg2_deep_vsJet_2d_extrapgt140_down;
		
		  idFakeSF_mutauFR_etaLt0p4_up      = except_vsMu * idSF_leg2_deep_vsMu_up[0];
		  idFakeSF_mutauFR_eta0p4to0p8_up   = except_vsMu * idSF_leg2_deep_vsMu_up[1];
		  idFakeSF_mutauFR_eta0p8to1p2_up   = except_vsMu * idSF_leg2_deep_vsMu_up[2];
		  idFakeSF_mutauFR_eta1p2to1p7_up   = except_vsMu * idSF_leg2_deep_vsMu_up[3];
		  idFakeSF_mutauFR_etaGt1p7_up	    = except_vsMu * idSF_leg2_deep_vsMu_up[4];
		  idFakeSF_mutauFR_etaLt0p4_down    = except_vsMu * idSF_leg2_deep_vsMu_down[0];
		  idFakeSF_mutauFR_eta0p4to0p8_down = except_vsMu * idSF_leg2_deep_vsMu_down[1];
		  idFakeSF_mutauFR_eta0p8to1p2_down = except_vsMu * idSF_leg2_deep_vsMu_down[2];
		  idFakeSF_mutauFR_eta1p2to1p7_down = except_vsMu * idSF_leg2_deep_vsMu_down[3];
		  idFakeSF_mutauFR_etaGt1p7_down    = except_vsMu * idSF_leg2_deep_vsMu_down[4];
	
		  idFakeSF_etauFR_barrel_up		= except_vsEle * idSF_leg2_deep_vsEle_up[0];
		  idFakeSF_etauFR_endcap_up		= except_vsEle * idSF_leg2_deep_vsEle_up[1];  
		  idFakeSF_etauFR_barrel_down	= except_vsEle * idSF_leg2_deep_vsEle_down[0];
		  idFakeSF_etauFR_endcap_down	= except_vsEle * idSF_leg2_deep_vsEle_down[1];
		}
	  else if (isMC and pType == 2) // TauTau
		{
		  idSF_deep_2d	= idSF_leg1_deep_vsJet_2d * idSF_leg2_deep_vsJet_2d;

		  except_VsJet	= idSF_leg1_deep_vsEle * idSF_leg1_deep_vsMu * idSF_leg2_deep_vsEle * idSF_leg2_deep_vsMu;
		  except_vsMu	= idSF_leg1_deep_vsJet_2d * idSF_leg1_deep_vsEle * idSF_leg2_deep_vsJet_2d * idSF_leg2_deep_vsEle;
		  except_vsEle	= idSF_leg1_deep_vsJet_2d * idSF_leg1_deep_vsMu  * idSF_leg2_deep_vsJet_2d * idSF_leg2_deep_vsMu;

		  idFakeSF_deep_2d	= except_VsJet * idSF_deep_2d;
		  fakeRateSF_deep	= except_VsJet;

		  idFakeSF_tauid_2d_stat0_DM0_up			  = except_VsJet * idSF_leg1_deep_vsJet_2d_stat0_DM0_up				 * idSF_leg2_deep_vsJet_2d_stat0_DM0_up;
		  idFakeSF_tauid_2d_stat0_DM0_down			  = except_VsJet * idSF_leg1_deep_vsJet_2d_stat0_DM0_down			 * idSF_leg2_deep_vsJet_2d_stat0_DM0_down;
		  idFakeSF_tauid_2d_stat1_DM0_up			  = except_VsJet * idSF_leg1_deep_vsJet_2d_stat1_DM0_up				 * idSF_leg2_deep_vsJet_2d_stat1_DM0_up;
		  idFakeSF_tauid_2d_stat1_DM0_down			  = except_VsJet * idSF_leg1_deep_vsJet_2d_stat1_DM0_down			 * idSF_leg2_deep_vsJet_2d_stat1_DM0_down;
		  idFakeSF_tauid_2d_systuncorrdmeras_DM0_up	  = except_VsJet * idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM0_up	 * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM0_up;
		  idFakeSF_tauid_2d_systuncorrdmeras_DM0_down = except_VsJet * idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM0_down * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM0_down;

		  idFakeSF_tauid_2d_stat0_DM1_up			  = except_VsJet * idSF_leg1_deep_vsJet_2d_stat0_DM1_up				 * idSF_leg2_deep_vsJet_2d_stat0_DM1_up;
		  idFakeSF_tauid_2d_stat0_DM1_down			  = except_VsJet * idSF_leg1_deep_vsJet_2d_stat0_DM1_down			 * idSF_leg2_deep_vsJet_2d_stat0_DM1_down;
		  idFakeSF_tauid_2d_stat1_DM1_up			  = except_VsJet * idSF_leg1_deep_vsJet_2d_stat1_DM1_up				 * idSF_leg2_deep_vsJet_2d_stat1_DM1_up;
		  idFakeSF_tauid_2d_stat1_DM1_down			  = except_VsJet * idSF_leg1_deep_vsJet_2d_stat1_DM1_down			 * idSF_leg2_deep_vsJet_2d_stat1_DM1_down;
		  idFakeSF_tauid_2d_systuncorrdmeras_DM1_up	  = except_VsJet * idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM1_up	 * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM1_up;
		  idFakeSF_tauid_2d_systuncorrdmeras_DM1_down = except_VsJet * idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM1_down * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM1_down;

		  idFakeSF_tauid_2d_stat0_DM10_up				= except_VsJet * idSF_leg1_deep_vsJet_2d_stat0_DM10_up			    * idSF_leg2_deep_vsJet_2d_stat0_DM10_up;
		  idFakeSF_tauid_2d_stat0_DM10_down				= except_VsJet * idSF_leg1_deep_vsJet_2d_stat0_DM10_down		    * idSF_leg2_deep_vsJet_2d_stat0_DM10_down;
		  idFakeSF_tauid_2d_stat1_DM10_up				= except_VsJet * idSF_leg1_deep_vsJet_2d_stat1_DM10_up			    * idSF_leg2_deep_vsJet_2d_stat1_DM10_up;
		  idFakeSF_tauid_2d_stat1_DM10_down				= except_VsJet * idSF_leg1_deep_vsJet_2d_stat1_DM10_down		    * idSF_leg2_deep_vsJet_2d_stat1_DM10_down;
		  idFakeSF_tauid_2d_systuncorrdmeras_DM10_up	= except_VsJet * idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM10_up   * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM10_up;
		  idFakeSF_tauid_2d_systuncorrdmeras_DM10_down	= except_VsJet * idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM10_down * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM10_down;

		  idFakeSF_tauid_2d_stat0_DM11_up				= except_VsJet * idSF_leg1_deep_vsJet_2d_stat0_DM11_up			    * idSF_leg2_deep_vsJet_2d_stat0_DM11_up;
		  idFakeSF_tauid_2d_stat0_DM11_down				= except_VsJet * idSF_leg1_deep_vsJet_2d_stat0_DM11_down		    * idSF_leg2_deep_vsJet_2d_stat0_DM11_down;
		  idFakeSF_tauid_2d_stat1_DM11_up				= except_VsJet * idSF_leg1_deep_vsJet_2d_stat1_DM11_up			    * idSF_leg2_deep_vsJet_2d_stat1_DM11_up;
		  idFakeSF_tauid_2d_stat1_DM11_down				= except_VsJet * idSF_leg1_deep_vsJet_2d_stat1_DM11_down		    * idSF_leg2_deep_vsJet_2d_stat1_DM11_down;
		  idFakeSF_tauid_2d_systuncorrdmeras_DM11_up	= except_VsJet * idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM11_up   * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM11_up;
		  idFakeSF_tauid_2d_systuncorrdmeras_DM11_down	= except_VsJet * idSF_leg1_deep_vsJet_2d_systuncorrdmeras_DM11_down * idSF_leg2_deep_vsJet_2d_systuncorrdmeras_DM11_down;

		  idFakeSF_tauid_2d_systcorrdmeras_up			= except_VsJet * idSF_leg1_deep_vsJet_2d_systcorrdmeras_up		   * idSF_leg2_deep_vsJet_2d_systcorrdmeras_up;
		  idFakeSF_tauid_2d_systcorrdmeras_down			= except_VsJet * idSF_leg1_deep_vsJet_2d_systcorrdmeras_down	   * idSF_leg2_deep_vsJet_2d_systcorrdmeras_down;
		  idFakeSF_tauid_2d_systcorrdmuncorreras_up		= except_VsJet * idSF_leg1_deep_vsJet_2d_systcorrdmuncorreras_up   * idSF_leg2_deep_vsJet_2d_systcorrdmuncorreras_up;
		  idFakeSF_tauid_2d_systcorrdmuncorreras_down	= except_VsJet * idSF_leg1_deep_vsJet_2d_systcorrdmuncorreras_down * idSF_leg2_deep_vsJet_2d_systcorrdmuncorreras_down;
		  idFakeSF_tauid_2d_systcorrerasgt140_up		= except_VsJet * idSF_leg1_deep_vsJet_2d_systcorrerasgt140_up      * idSF_leg2_deep_vsJet_2d_systcorrerasgt140_up;
		  idFakeSF_tauid_2d_systcorrerasgt140_down   	= except_VsJet * idSF_leg1_deep_vsJet_2d_systcorrerasgt140_down    * idSF_leg2_deep_vsJet_2d_systcorrerasgt140_down;
		  idFakeSF_tauid_2d_stat0gt140_up				= except_VsJet * idSF_leg1_deep_vsJet_2d_stat0gt140_up		       * idSF_leg2_deep_vsJet_2d_stat0gt140_up;
		  idFakeSF_tauid_2d_stat0gt140_down				= except_VsJet * idSF_leg1_deep_vsJet_2d_stat0gt140_down		   * idSF_leg2_deep_vsJet_2d_stat0gt140_down;
		  idFakeSF_tauid_2d_stat1gt140_up				= except_VsJet * idSF_leg1_deep_vsJet_2d_stat1gt140_up		       * idSF_leg2_deep_vsJet_2d_stat1gt140_up;
		  idFakeSF_tauid_2d_stat1gt140_down				= except_VsJet * idSF_leg1_deep_vsJet_2d_stat1gt140_down		   * idSF_leg2_deep_vsJet_2d_stat1gt140_down;
		  idFakeSF_tauid_2d_extrapgt140_up            	= except_VsJet * idSF_leg1_deep_vsJet_2d_extrapgt140_up            * idSF_leg2_deep_vsJet_2d_extrapgt140_up;
		  idFakeSF_tauid_2d_extrapgt140_down          	= except_VsJet * idSF_leg1_deep_vsJet_2d_extrapgt140_down          * idSF_leg2_deep_vsJet_2d_extrapgt140_down;

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
	  else if(isMC and (pType == 3 or pType == 4))  // MuMu and EleEle channels
		{
		  idSF_deep_2d = idFakeSF_deep_2d = idSF_leg1 * idSF_leg2;
		}
	  
	  if (DEBUG) {
		cout << "--- DEBUG idSF ---" << endl;
		cout << "pairType  : "              << pType                   << endl;
		cout << "totSF deep_2d: "           << idFakeSF_deep_2d        << endl;
		cout << "idSF_leg1: "               << idSF_leg1               << endl;
		cout << "idSF_leg1_deep_vsJet_2d: " << idSF_leg1_deep_vsJet_2d << endl;
		cout << "idSF_leg1_deep_vsEle: "    << idSF_leg1_deep_vsEle    << endl;
		cout << "idSF_leg1_deep_vsMu: "     << idSF_leg1_deep_vsMu     << endl;
		cout << "idSF_leg2_deep_vsJet_2d: " << idSF_leg2_deep_vsJet_2d << endl;
		cout << "idSF_leg2_deep_vsEle: "    << idSF_leg2_deep_vsEle    << endl;
		cout << "idSF_leg2_deep_vsMu: "     << idSF_leg2_deep_vsMu     << endl;
	  }

	  // Save the IDandISO SF (event per event)
	  theSmallTree.m_IdSF_deep_2d				= idSF_deep_2d;
	  theSmallTree.m_IdSF_leg1_deep_vsJet_2d	= idSF_leg1_deep_vsJet_2d;
	  theSmallTree.m_IdSF_leg2_deep_vsJet_2d	= idSF_leg2_deep_vsJet_2d;
	  
	  theSmallTree.m_IdFakeSF_deep_2d = idFakeSF_deep_2d;
	  theSmallTree.m_FakeRateSF_deep  = fakeRateSF_deep;

	  theSmallTree.m_idFakeSF_tauid_2d_stat0_DM0_up				 = idFakeSF_tauid_2d_stat0_DM0_up;
	  theSmallTree.m_idFakeSF_tauid_2d_stat0_DM0_down			 = idFakeSF_tauid_2d_stat0_DM0_down;
	  theSmallTree.m_idFakeSF_tauid_2d_stat1_DM0_up				 = idFakeSF_tauid_2d_stat1_DM0_up;
	  theSmallTree.m_idFakeSF_tauid_2d_stat1_DM0_down			 = idFakeSF_tauid_2d_stat1_DM0_down;
	  theSmallTree.m_idFakeSF_tauid_2d_systuncorrdmeras_DM0_up	 = idFakeSF_tauid_2d_systuncorrdmeras_DM0_up;
	  theSmallTree.m_idFakeSF_tauid_2d_systuncorrdmeras_DM0_down = idFakeSF_tauid_2d_systuncorrdmeras_DM0_down;
	  
	  theSmallTree.m_idFakeSF_tauid_2d_stat0_DM1_up				 = idFakeSF_tauid_2d_stat0_DM1_up;
	  theSmallTree.m_idFakeSF_tauid_2d_stat0_DM1_down			 = idFakeSF_tauid_2d_stat0_DM1_down;
	  theSmallTree.m_idFakeSF_tauid_2d_stat1_DM1_up				 = idFakeSF_tauid_2d_stat1_DM1_up;
	  theSmallTree.m_idFakeSF_tauid_2d_stat1_DM1_down			 = idFakeSF_tauid_2d_stat1_DM1_down;
	  theSmallTree.m_idFakeSF_tauid_2d_systuncorrdmeras_DM1_up	 = idFakeSF_tauid_2d_systuncorrdmeras_DM1_up;
	  theSmallTree.m_idFakeSF_tauid_2d_systuncorrdmeras_DM1_down = idFakeSF_tauid_2d_systuncorrdmeras_DM1_down;

	  theSmallTree.m_idFakeSF_tauid_2d_stat0_DM10_up			  = idFakeSF_tauid_2d_stat0_DM10_up;
	  theSmallTree.m_idFakeSF_tauid_2d_stat0_DM10_down			  = idFakeSF_tauid_2d_stat0_DM10_down;
	  theSmallTree.m_idFakeSF_tauid_2d_stat1_DM10_up			  = idFakeSF_tauid_2d_stat1_DM10_up;
	  theSmallTree.m_idFakeSF_tauid_2d_stat1_DM10_down			  = idFakeSF_tauid_2d_stat1_DM10_down;
	  theSmallTree.m_idFakeSF_tauid_2d_systuncorrdmeras_DM10_up	  = idFakeSF_tauid_2d_systuncorrdmeras_DM10_up;
	  theSmallTree.m_idFakeSF_tauid_2d_systuncorrdmeras_DM10_down = idFakeSF_tauid_2d_systuncorrdmeras_DM10_down;
	  
	  theSmallTree.m_idFakeSF_tauid_2d_stat0_DM11_up			  = idFakeSF_tauid_2d_stat0_DM11_up;
	  theSmallTree.m_idFakeSF_tauid_2d_stat0_DM11_down			  = idFakeSF_tauid_2d_stat0_DM11_down;
	  theSmallTree.m_idFakeSF_tauid_2d_stat1_DM11_up			  = idFakeSF_tauid_2d_stat1_DM11_up;
	  theSmallTree.m_idFakeSF_tauid_2d_stat1_DM11_down			  = idFakeSF_tauid_2d_stat1_DM11_down;
	  theSmallTree.m_idFakeSF_tauid_2d_systuncorrdmeras_DM11_up	  = idFakeSF_tauid_2d_systuncorrdmeras_DM11_up;
	  theSmallTree.m_idFakeSF_tauid_2d_systuncorrdmeras_DM11_down = idFakeSF_tauid_2d_systuncorrdmeras_DM11_down;

	  theSmallTree.m_idFakeSF_tauid_2d_systcorrdmeras_up			= idFakeSF_tauid_2d_systcorrdmeras_up;
	  theSmallTree.m_idFakeSF_tauid_2d_systcorrdmeras_down			= idFakeSF_tauid_2d_systcorrdmeras_down;
	  theSmallTree.m_idFakeSF_tauid_2d_systcorrdmuncorreras_up		= idFakeSF_tauid_2d_systcorrdmuncorreras_up;
	  theSmallTree.m_idFakeSF_tauid_2d_systcorrdmuncorreras_down	= idFakeSF_tauid_2d_systcorrdmuncorreras_down;
	  theSmallTree.m_idFakeSF_tauid_2d_systcorrerasgt140_up			= idFakeSF_tauid_2d_systcorrerasgt140_up;
	  theSmallTree.m_idFakeSF_tauid_2d_systcorrerasgt140_down		= idFakeSF_tauid_2d_systcorrerasgt140_down;
	  theSmallTree.m_idFakeSF_tauid_2d_stat0gt140_up				= idFakeSF_tauid_2d_stat0gt140_up;
	  theSmallTree.m_idFakeSF_tauid_2d_stat0gt140_down				= idFakeSF_tauid_2d_stat0gt140_down;
	  theSmallTree.m_idFakeSF_tauid_2d_stat1gt140_up				= idFakeSF_tauid_2d_stat1gt140_up;
	  theSmallTree.m_idFakeSF_tauid_2d_stat1gt140_down				= idFakeSF_tauid_2d_stat1gt140_down;
	  theSmallTree.m_idFakeSF_tauid_2d_extrapgt140_up				= idFakeSF_tauid_2d_extrapgt140_up;
	  theSmallTree.m_idFakeSF_tauid_2d_extrapgt140_down				= idFakeSF_tauid_2d_extrapgt140_down;
	
	  theSmallTree.m_idFakeSF_mutauFR_etaLt0p4_up		= idFakeSF_mutauFR_etaLt0p4_up;		
	  theSmallTree.m_idFakeSF_mutauFR_eta0p4to0p8_up	= idFakeSF_mutauFR_eta0p4to0p8_up;	
	  theSmallTree.m_idFakeSF_mutauFR_eta0p8to1p2_up	= idFakeSF_mutauFR_eta0p8to1p2_up;	
	  theSmallTree.m_idFakeSF_mutauFR_eta1p2to1p7_up	= idFakeSF_mutauFR_eta1p2to1p7_up;	
	  theSmallTree.m_idFakeSF_mutauFR_etaGt1p7_up		= idFakeSF_mutauFR_etaGt1p7_up;		
	  theSmallTree.m_idFakeSF_mutauFR_etaLt0p4_down		= idFakeSF_mutauFR_etaLt0p4_down;	
	  theSmallTree.m_idFakeSF_mutauFR_eta0p4to0p8_down	= idFakeSF_mutauFR_eta0p4to0p8_down;
	  theSmallTree.m_idFakeSF_mutauFR_eta0p8to1p2_down	= idFakeSF_mutauFR_eta0p8to1p2_down;
	  theSmallTree.m_idFakeSF_mutauFR_eta1p2to1p7_down	= idFakeSF_mutauFR_eta1p2to1p7_down;
	  theSmallTree.m_idFakeSF_mutauFR_etaGt1p7_down		= idFakeSF_mutauFR_etaGt1p7_down;
	  theSmallTree.m_idFakeSF_etauFR_barrel_up			= idFakeSF_etauFR_barrel_up;	
	  theSmallTree.m_idFakeSF_etauFR_endcap_up			= idFakeSF_etauFR_endcap_up;		
	  theSmallTree.m_idFakeSF_etauFR_barrel_down		= idFakeSF_etauFR_barrel_down;		
	  theSmallTree.m_idFakeSF_etauFR_endcap_down		= idFakeSF_etauFR_endcap_down;
	  
	  //Jet faking Tau SF
	  //derived from WJet sideband: http://camendol.web.cern.ch/camendol/HH2017/plotsHH2017MuTau/31Oct2018_DYNLO_ctrlWJets_SS/antiB_jets30_tau30_SStight/
	  double jetFakeSF1 = 1.;
	  double jetFakeSF2 = 1.;
	  if (isMC and pType<3) { // 2nd tau
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
	  float trigSF					= -1.0;
	  float trigSFnoMET				= -1.0;
	  float trigSFnoTau				= -1.0;
	  float trigSF_ele_up			= -1.0;
	  float trigSF_mu_up			= -1.0;
	  float trigSF_SL_mu_up			= -1.0;
	  float trigSF_cross_mu_up		= -1.0;
	  float trigSF_tau_DM0_up		= -1.0;
	  float trigSF_tau_DM1_up		= -1.0;
	  float trigSF_tau_DM10_up		= -1.0;
	  float trigSF_tau_DM11_up		= -1.0;
	  float trigSF_ele_down			= -1.0;
	  float trigSF_mu_down			= -1.0;
	  float trigSF_SL_mu_down		= -1.0;
	  float trigSF_cross_mu_down	= -1.0;
	  float trigSF_tau_DM0_down		= -1.0;
	  float trigSF_tau_DM1_down		= -1.0;
	  float trigSF_tau_DM10_down	= -1.0;
	  float trigSF_tau_DM11_down	= -1.0;
	  float trigSF_met_up			= -1.0;
	  float trigSF_met_down			= -1.0;
	  float trigSF_stau_up			= -1.0;
	  float trigSF_stau_down		= -1.0;
	  
	  if(applyTriggers)
		{
		  // MuTau Channel
		  if (pType == 0 and isMC)
			{
			  if (trgRegions["met"])
				{
				  trigSF           = metSF.getSF(vMETnoMu.Mod());
				  trigSFnoMET      = 1.;
				  trigSFnoTau      = trigSF;
				  trigSF_met_up    = trigSF + metSF.getSFError(vMETnoMu.Mod());
				  trigSF_met_down  = trigSF - metSF.getSFError(vMETnoMu.Mod());
				  trigSF_stau_up   = trigSF;
				  trigSF_stau_down = trigSF;
				  trigSF_tau_DM0_up	   = trigSF; 
				  trigSF_tau_DM1_up	   = trigSF; 
				  trigSF_tau_DM10_up   = trigSF; 
				  trigSF_tau_DM11_up   = trigSF; 
				  trigSF_tau_DM0_down  = trigSF; 
				  trigSF_tau_DM1_down  = trigSF; 
				  trigSF_tau_DM10_down = trigSF; 
				  trigSF_tau_DM11_down = trigSF;
				  trigSF_ele_up	  = trigSF;
				  trigSF_ele_down = trigSF;
				  trigSF_mu_up	  = trigSF;
				  trigSF_mu_down  = trigSF;
				  trigSF_SL_mu_up = trigSF;
				  trigSF_SL_mu_down = trigSF;
				  trigSF_cross_mu_up = trigSF;
				  trigSF_cross_mu_down = trigSF;
				}

			  else if (trgRegions["tau"])
				{
				  trigSF           = singleTauSF[PERIOD].first;
				  trigSFnoMET      = trigSF;
				  trigSFnoTau      = 1.;
				  trigSF_stau_up   = trigSF + singleTauSF[PERIOD].second;
				  trigSF_stau_down = trigSF - singleTauSF[PERIOD].second;
				  trigSF_met_up    = trigSF;
				  trigSF_met_down  = trigSF;
				  trigSF_tau_DM0_up	   = trigSF; 
				  trigSF_tau_DM1_up	   = trigSF; 
				  trigSF_tau_DM10_up   = trigSF; 
				  trigSF_tau_DM11_up   = trigSF; 
				  trigSF_tau_DM0_down  = trigSF; 
				  trigSF_tau_DM1_down  = trigSF; 
				  trigSF_tau_DM10_down = trigSF; 
				  trigSF_tau_DM11_down = trigSF; 
				  trigSF_ele_up	  = trigSF;
				  trigSF_ele_down = trigSF;
				  trigSF_mu_up	  = trigSF;
				  trigSF_mu_down  = trigSF;
				  trigSF_SL_mu_up = trigSF;
				  trigSF_SL_mu_down = trigSF;
				  trigSF_cross_mu_up = trigSF;
				  trigSF_cross_mu_down = trigSF;
				}

			  // eta region covered both by cross-trigger and single lepton trigger
			  else if (trgRegions["legacy_cross"])
				{
				  int passSingle = 1, passCross = 1;

				  float lep1_thresh = trgAssigner.getLepton1PtThresh(PERIOD, "mutau");
				  float lep2_thresh = trgAssigner.getLepton2PtThresh(PERIOD, "mutau");
				  if (tlv_firstLepton.Pt()  < lep1_thresh) passSingle = 0;
				  if (tlv_secondLepton.Pt() < lep2_thresh) passCross  = 0;

				  // lepton trigger
				  double SFL_Data = 1., SFL_MC = 1., SFL_Data_Err = 0., SFL_MC_Err = 0.;
				  if (passSingle) {
					SFL_Data = muTrgSF->get_EfficiencyData(tlv_firstLepton.Pt(), fabs(tlv_firstLepton.Eta()), pType);
					SFL_MC   = muTrgSF->get_EfficiencyMC(  tlv_firstLepton.Pt(), fabs(tlv_firstLepton.Eta()), pType);
					SFL_Data_Err = muTrgSF->get_EfficiencyDataError(tlv_firstLepton.Pt(), fabs(tlv_firstLepton.Eta()), pType);
					SFL_MC_Err   = muTrgSF->get_EfficiencyMCError(  tlv_firstLepton.Pt(), fabs(tlv_firstLepton.Eta()), pType);
				  }

				  //cross-trigger
				  //mu leg
				  double SFl_Data = 1., SFl_MC = 1., SFl_Data_Err = 0., SFl_MC_Err = 0.;
				  if (passCross) {
					SFl_Data = muTauTrgSF->get_EfficiencyData(tlv_firstLepton.Pt(), fabs(tlv_firstLepton.Eta()), pType);
					SFl_MC   = muTauTrgSF->get_EfficiencyMC(  tlv_firstLepton.Pt(), fabs(tlv_firstLepton.Eta()), pType);
					SFl_Data_Err = muTauTrgSF->get_EfficiencyDataError(tlv_firstLepton.Pt(), fabs(tlv_firstLepton.Eta()), pType);
					SFl_MC_Err   = muTauTrgSF->get_EfficiencyMCError(  tlv_firstLepton.Pt(), fabs(tlv_firstLepton.Eta()), pType);
				  }

				  //tau leg
				  double SFtau_Data = tauTrgSF_mutau->getEfficiencyData(tlv_secondLepton.Pt(), DM2, 0); // last entry is uncertainty: 0 central, +1 up, -1 down
				  double SFtau_MC   = tauTrgSF_mutau->getEfficiencyMC  (tlv_secondLepton.Pt(), DM2, 0); // last entry is uncertainty: 0 central, +1 up, -1 down

				  // Compute efficiencies
				  double Eff_Data = passSingle * SFL_Data - passCross * passSingle * std::min(SFl_Data, SFL_Data) * SFtau_Data + passCross * SFl_Data * SFtau_Data;
				  double Eff_MC   = passSingle * SFL_MC   - passCross * passSingle * std::min(SFl_MC  , SFL_MC)   * SFtau_MC   + passCross * SFl_MC   * SFtau_MC;

				  // Compute uncertainties (separate for SingleMuon and mu leg of cross trigger)
				  double Err_Data_SL_mu = passSingle * SFL_Data_Err - passCross * passSingle * (SFl_Data > SFL_Data) * SFL_Data_Err * SFtau_Data;
				  double Err_MC_SL_mu   = passSingle * SFL_MC_Err   - passCross * passSingle * (SFl_MC   > SFL_MC)   * SFL_MC_Err   * SFtau_MC;

				  double trigSF_SL_mu_err = 0;
				  if(passSingle) trigSF_SL_mu_err = muTrgSF->get_ScaleFactorError(Eff_Data, Eff_MC, Err_Data_SL_mu, Err_MC_SL_mu);

				  double Err_Data_cross_mu = - passCross * passSingle * (SFl_Data < SFL_Data) * SFl_Data_Err * SFtau_Data + passCross * SFl_Data_Err * SFtau_Data;
				  double Err_MC_cross_mu   = - passCross * passSingle * (SFl_MC   < SFL_MC)   * SFl_MC_Err   * SFtau_MC   + passCross * SFl_MC_Err   * SFtau_MC;

				  double trigSF_cross_mu_err = 0;
				  if(passCross && !(passSingle && (SFl_Data < SFL_Data))) trigSF_cross_mu_err = muTrgSF->get_ScaleFactorError(Eff_Data, Eff_MC, Err_Data_cross_mu, Err_MC_cross_mu);

				  double Err_Data_mu = Err_Data_SL_mu + Err_Data_cross_mu;
				  double Err_MC_mu   = Err_MC_SL_mu   + Err_MC_cross_mu;
				  double trigSF_mu_err   = muTrgSF->get_ScaleFactorError(Eff_Data, Eff_MC, Err_Data_mu, Err_MC_mu);

				  // for each DM, get the trigSF error if the tauh has the corresponding DM (and the cross trigger thresholds are passed), otherwise 0
				  vector <double> trigSF_err    (N_tauhDM, 0);
				  if(passCross && ((SFl_Data > SFL_Data) || (SFl_MC > SFL_MC))){
					for (int idm  = 0; idm < N_tauhDM; idm ++){
						if (isthisDM_second[idm]){
							double SFtau_Data_Up = tauTrgSF_mutau->getEfficiencyData(tlv_secondLepton.Pt(), DM2, 1);
							double SFtau_MC_Up   = tauTrgSF_mutau->getEfficiencyMC  (tlv_secondLepton.Pt(), DM2, 1);
							double Eff_Data_Up = passSingle * SFL_Data - passCross * passSingle * std::min(SFl_Data, SFL_Data) * SFtau_Data_Up   + passCross * SFl_Data * SFtau_Data_Up;
							double Eff_MC_Up   = passSingle * SFL_MC   - passCross * passSingle * std::min(SFl_MC  , SFL_MC)   * SFtau_MC_Up     + passCross * SFl_MC   * SFtau_MC_Up;
							trigSF_err[idm] = muTrgSF->get_ScaleFactorError(Eff_Data, Eff_MC, Eff_Data_Up - Eff_Data, Eff_MC_Up - Eff_MC);
						}
					}
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

				  trigSF                = muTrgSF->get_ScaleFactor(Eff_Data, Eff_MC);
				  trigSFnoMET           = trigSF;
				  trigSFnoTau           = trigSF;
				  trigSF_mu_up          = trigSF + trigSF_mu_err;
				  trigSF_mu_down        = trigSF - trigSF_mu_err;
				  trigSF_SL_mu_up       = trigSF + trigSF_SL_mu_err;
				  trigSF_SL_mu_down     = trigSF - trigSF_SL_mu_err;
				  trigSF_cross_mu_up    = trigSF + trigSF_cross_mu_err;
				  trigSF_cross_mu_down  = trigSF - trigSF_cross_mu_err;
				  trigSF_tau_DM0_up     = trigSF + trigSF_err[0];
				  trigSF_tau_DM0_down   = trigSF - trigSF_err[0];
				  trigSF_tau_DM1_up     = trigSF + trigSF_err[1];
				  trigSF_tau_DM1_down   = trigSF - trigSF_err[1];
				  trigSF_tau_DM10_up    = trigSF + trigSF_err[2];
				  trigSF_tau_DM10_down  = trigSF - trigSF_err[2];
				  trigSF_tau_DM11_up    = trigSF + trigSF_err[3];
				  trigSF_tau_DM11_down  = trigSF - trigSF_err[3];
				  trigSF_met_up         = trigSF;
				  trigSF_met_down       = trigSF;
				  trigSF_stau_up        = trigSF;
				  trigSF_stau_down      = trigSF;
				}
			  else // eta region covered only by single lepton trigger
				{
				  assert (trgRegions["legacy"]);
				  double SF = 1., SF_Err = 0.;
					
				  float lep1_thresh = trgAssigner.getLepton1PtThresh(PERIOD, "mutau");
				  if (tlv_firstLepton.Pt() < lep1_thresh) {
					cout <<  "[WARNING] Unexpected event in mutau channel: pT = " << tlv_firstLepton.Pt()
						 << ", eta = " << tlv_firstLepton.Eta() << "."
						 << " The weight will be set to one." << endl;
				  }
				  else {
					SF     = muTrgSF->get_ScaleFactor(tlv_firstLepton.Pt(),      fabs(tlv_firstLepton.Eta()), pType);
					SF_Err = muTrgSF->get_ScaleFactorError(tlv_firstLepton.Pt(), fabs(tlv_firstLepton.Eta()), pType);
				  }
				  
				  trigSF               = SF;
				  trigSFnoMET          = trigSF;
				  trigSFnoTau          = trigSF;
				  trigSF_mu_up         = SF + 1. * SF_Err;
				  trigSF_mu_down       = SF - 1. * SF_Err;
				  trigSF_SL_mu_up      = trigSF_mu_up;
				  trigSF_SL_mu_down    = trigSF_mu_down;
				  trigSF_cross_mu_up   = trigSF;
				  trigSF_cross_mu_down = trigSF;
				  trigSF_ele_up	       = trigSF;
				  trigSF_ele_down      = trigSF;
				  trigSF_met_up        = trigSF;
				  trigSF_met_down      = trigSF;
				  trigSF_stau_up       = trigSF;
				  trigSF_stau_down     = trigSF;
				  trigSF_tau_DM0_up	   = trigSF; 
				  trigSF_tau_DM1_up	   = trigSF; 
				  trigSF_tau_DM10_up   = trigSF; 
				  trigSF_tau_DM11_up   = trigSF; 
				  trigSF_tau_DM0_down  = trigSF; 
				  trigSF_tau_DM1_down  = trigSF; 
				  trigSF_tau_DM10_down = trigSF; 
				  trigSF_tau_DM11_down = trigSF; 
				}
			}

		  // EleTau Channel
		  else if (pType == 1 and isMC)
			{
			  if (trgRegions["met"])
				{
				  trigSF               = metSF.getSF(vMETnoMu.Mod());
				  trigSFnoMET          = 1.;
				  trigSFnoTau          = trigSF;
				  trigSF_met_up        = trigSF + metSF.getSFError(vMETnoMu.Mod());
				  trigSF_met_down      = trigSF - metSF.getSFError(vMETnoMu.Mod());
				  trigSF_stau_up       = trigSF;
				  trigSF_stau_down     = trigSF;
				  trigSF_tau_DM0_up	   = trigSF; 
				  trigSF_tau_DM1_up	   = trigSF; 
				  trigSF_tau_DM10_up   = trigSF; 
				  trigSF_tau_DM11_up   = trigSF; 
				  trigSF_tau_DM0_down  = trigSF; 
				  trigSF_tau_DM1_down  = trigSF; 
				  trigSF_tau_DM10_down = trigSF; 
				  trigSF_tau_DM11_down = trigSF; 
				  trigSF_ele_up	       = trigSF;
				  trigSF_ele_down      = trigSF;
				  trigSF_mu_up	       = trigSF;
				  trigSF_mu_down       = trigSF;
				  trigSF_SL_mu_up      = trigSF;
				  trigSF_SL_mu_down    = trigSF;
				  trigSF_cross_mu_up   = trigSF;
				  trigSF_cross_mu_down = trigSF;
				}

			  else if (trgRegions["tau"])
				{
				  trigSF           = singleTauSF[PERIOD].first;
				  trigSFnoMET      = trigSF;
				  trigSFnoTau      = 1.;
				  trigSF_stau_up   = trigSF + singleTauSF[PERIOD].second;
				  trigSF_stau_down = trigSF - singleTauSF[PERIOD].second;
				  trigSF_met_up    = trigSF;
				  trigSF_met_down  = trigSF;
				  trigSF_tau_DM0_up	   = trigSF; 
				  trigSF_tau_DM1_up	   = trigSF; 
				  trigSF_tau_DM10_up   = trigSF; 
				  trigSF_tau_DM11_up   = trigSF; 
				  trigSF_tau_DM0_down  = trigSF; 
				  trigSF_tau_DM1_down  = trigSF; 
				  trigSF_tau_DM10_down = trigSF; 
				  trigSF_tau_DM11_down = trigSF; 
				  trigSF_ele_up	  = trigSF;
				  trigSF_ele_down = trigSF;
				  trigSF_mu_up	  = trigSF;
				  trigSF_mu_down  = trigSF;
				  trigSF_SL_mu_up = trigSF;
				  trigSF_SL_mu_down = trigSF;
				  trigSF_cross_mu_up = trigSF;
				  trigSF_cross_mu_down = trigSF;
				}

			  // eta region covered both by cross-trigger and single lepton trigger
			  else if(PERIOD != "2016preVFP" and PERIOD != "2016postVFP" and trgRegions["legacy_cross"])
				{
				  int passSingle = 1, passCross = 1;

				  float lep1_thresh = trgAssigner.getLepton1PtThresh(PERIOD, "etau");
				  float lep2_thresh = trgAssigner.getLepton2PtThresh(PERIOD, "etau");
				  if (tlv_firstLepton.Pt() < lep1_thresh) passSingle = 0;
				  if (tlv_secondLepton.Pt() < lep2_thresh) passCross = 0;

				  //lepton trigger
				  double SFL_Data = 1., SFL_MC = 1., SFL_Data_Err = 0., SFL_MC_Err = 0.;
				  if (passSingle) {
					SFL_Data = eTrgSF->get_EfficiencyData(tlv_firstLepton.Pt(), tlv_firstLepton.Eta(), pType);
					SFL_MC   = eTrgSF->get_EfficiencyMC(  tlv_firstLepton.Pt(), tlv_firstLepton.Eta(), pType);
					SFL_Data_Err = eTrgSF->get_EfficiencyDataError(tlv_firstLepton.Pt(), tlv_firstLepton.Eta(), pType);
					SFL_MC_Err   = eTrgSF->get_EfficiencyMCError(  tlv_firstLepton.Pt(), tlv_firstLepton.Eta(), pType);
				  }

				  //cross-trigger
				  //e leg
				  double SFl_Data = 1., SFl_MC = 1., SFl_Data_Err = 0., SFl_MC_Err = 0.;
				  if (passCross) {
					SFl_Data = eTauTrgSF->get_EfficiencyData(tlv_firstLepton.Pt(), tlv_firstLepton.Eta(), pType);
					SFl_MC	 = eTauTrgSF->get_EfficiencyMC(tlv_firstLepton.Pt(),   tlv_firstLepton.Eta(), pType);
					SFl_Data_Err = eTauTrgSF->get_EfficiencyDataError(tlv_firstLepton.Pt(), tlv_firstLepton.Eta(), pType);
					SFl_MC_Err	 = eTauTrgSF->get_EfficiencyMCError(tlv_firstLepton.Pt(),	tlv_firstLepton.Eta(), pType);
				  }

				  //tau leg
				  double SFtau_Data = tauTrgSF_etau->getEfficiencyData(tlv_secondLepton.Pt(), DM2, 0); // last entry is uncertainty: 0 central, +1 up, -1 down
				  double SFtau_MC   = tauTrgSF_etau->getEfficiencyMC  (tlv_secondLepton.Pt(), DM2, 0); // last entry is uncertainty: 0 central, +1 up, -1 down

				  // Compute efficiencies
				  double Eff_Data = passSingle * SFL_Data - passCross * passSingle * std::min(SFl_Data, SFL_Data) * SFtau_Data + passCross * SFl_Data * SFtau_Data;
				  double Eff_MC   = passSingle * SFL_MC   - passCross * passSingle * std::min(SFl_MC  , SFL_MC)   * SFtau_MC   + passCross * SFl_MC   * SFtau_MC;

				  double Err_Data_ele   = passSingle * SFL_Data_Err   - passCross * passSingle * ((SFl_Data < SFL_Data) ? SFl_Data_Err  : SFL_Data_Err)   * SFtau_Data + passCross * SFl_Data_Err   * SFtau_Data;
				  double Err_MC_ele     = passSingle * SFL_MC_Err     - passCross * passSingle * ((SFl_MC   < SFL_MC)   ? SFl_MC_Err    : SFL_MC_Err)     * SFtau_MC   + passCross * SFl_MC_Err     * SFtau_MC;
				  double trigSF_ele_err   = eTrgSF->get_ScaleFactorError(Eff_Data, Eff_MC, Err_Data_ele, Err_MC_ele);

				  // for each DM, get the trigSF error if the tauh has the corresponding DM (and the cross trigger thresholds are passed), otherwise 0
				  vector <double> trigSF_err    (N_tauhDM, 0);
				  if(passCross && ((SFl_Data > SFL_Data) || (SFl_MC > SFL_MC))){
					for (int idm  = 0; idm < N_tauhDM; idm ++){
						if (isthisDM_second[idm]){
							double SFtau_Data_Up = tauTrgSF_mutau->getEfficiencyData(tlv_secondLepton.Pt(), DM2, 1);
							double SFtau_MC_Up   = tauTrgSF_mutau->getEfficiencyMC  (tlv_secondLepton.Pt(), DM2, 1);
							double Eff_Data_Up = passSingle * SFL_Data - passCross * passSingle * std::min(SFl_Data, SFL_Data) * SFtau_Data_Up   + passCross * SFl_Data * SFtau_Data_Up;
							double Eff_MC_Up   = passSingle * SFL_MC   - passCross * passSingle * std::min(SFl_MC  , SFL_MC)   * SFtau_MC_Up     + passCross * SFl_MC   * SFtau_MC_Up;
							trigSF_err[idm] = eTrgSF->get_ScaleFactorError(Eff_Data, Eff_MC, Eff_Data_Up - Eff_Data, Eff_MC_Up - Eff_MC);
						}
					}
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

				  trigSF                = eTrgSF->get_ScaleFactor(Eff_Data, Eff_MC);
				  trigSFnoMET           = trigSF;
				  trigSFnoTau           = trigSF;
				  trigSF_ele_up         = trigSF + trigSF_ele_err;
				  trigSF_ele_down       = trigSF - trigSF_ele_err;
				  trigSF_mu_up          = trigSF;
				  trigSF_mu_down        = trigSF;
				  trigSF_SL_mu_up       = trigSF;
				  trigSF_SL_mu_down     = trigSF;
				  trigSF_cross_mu_up    = trigSF;
				  trigSF_cross_mu_down  = trigSF;
				  trigSF_tau_DM0_up     = trigSF + trigSF_err[0];
				  trigSF_tau_DM0_down   = trigSF - trigSF_err[0];
				  trigSF_tau_DM1_up     = trigSF + trigSF_err[1];
				  trigSF_tau_DM1_down   = trigSF - trigSF_err[1];
				  trigSF_tau_DM10_up    = trigSF + trigSF_err[2];
				  trigSF_tau_DM10_down  = trigSF - trigSF_err[2];
				  trigSF_tau_DM11_up    = trigSF + trigSF_err[3];
				  trigSF_tau_DM11_down  = trigSF - trigSF_err[3];
				  trigSF_met_up         = trigSF;
				  trigSF_met_down       = trigSF;
				  trigSF_stau_up        = trigSF;
				  trigSF_stau_down      = trigSF;
				}

			  else //eta region covered only by single lepton trigger (including all 2016)
				{
				  assert (trgRegions["legacy"]);
				  
				  double SF = 1., SF_Err = 0.;

				  float lep1_thresh = trgAssigner.getLepton1PtThresh(PERIOD, "etau");
				  if (tlv_firstLepton.Pt() < lep1_thresh) {
					cout <<  "[WARNING] Unexpected event in etau channel: pT = " << tlv_firstLepton.Pt()
						 << ", eta = " << tlv_firstLepton.Eta() << "."
						 << " The weight will be set to one." << endl;
				  }
				  else {
					// dirty trick to deal with bad efficiency values in a (very) low-stat bins
					// should only affect one bin in sf_el_2016post_HLTEle25.root
					if(PERIOD == "2016postVFP" and tlv_firstLepton.Pt() >= 100. and tlv_firstLepton.Eta() <= -2.) {
					  SF     = eTrgSF->get_ScaleFactor(     tlv_firstLepton.Pt(), -tlv_firstLepton.Eta(), pType);
					  SF_Err = eTrgSF->get_ScaleFactorError(tlv_firstLepton.Pt(), -tlv_firstLepton.Eta(), pType);
					}
					else {
					  SF     = eTrgSF->get_ScaleFactor(     tlv_firstLepton.Pt(), tlv_firstLepton.Eta(), pType);
					  SF_Err = eTrgSF->get_ScaleFactorError(tlv_firstLepton.Pt(), tlv_firstLepton.Eta(), pType);
					}
				  }

				  trigSF          = SF;
				  trigSFnoMET     = trigSF;
				  trigSFnoTau     = trigSF;
				  trigSF_ele_up   = SF + 1. * SF_Err;
				  trigSF_ele_down = SF - 1. * SF_Err;
				  trigSF_mu_up	  = trigSF;
				  trigSF_mu_down  = trigSF;
				  trigSF_SL_mu_up = trigSF;
				  trigSF_SL_mu_down = trigSF;
				  trigSF_cross_mu_up = trigSF;
				  trigSF_cross_mu_down = trigSF;
				  trigSF_met_up   = trigSF;
				  trigSF_met_down = trigSF;
				  trigSF_stau_up   = trigSF;
				  trigSF_stau_down = trigSF;
				  trigSF_tau_DM0_up	   = trigSF; 
				  trigSF_tau_DM1_up	   = trigSF; 
				  trigSF_tau_DM10_up   = trigSF; 
				  trigSF_tau_DM11_up   = trigSF; 
				  trigSF_tau_DM0_down  = trigSF; 
				  trigSF_tau_DM1_down  = trigSF; 
				  trigSF_tau_DM10_down = trigSF; 
				  trigSF_tau_DM11_down = trigSF; 
				}
			}

		  // TauTau Channel
		  else if (pType == 2 and isMC)
			{
			  if (trgRegions["met"])
				{
				  trigSF          = metSF.getSF(vMETnoMu.Mod());
				  trigSFnoMET     = 1.;
				  trigSFnoTau     = trigSF;
				  trigSF_met_up   = trigSF + metSF.getSFError(vMETnoMu.Mod());
				  trigSF_met_down = trigSF - metSF.getSFError(vMETnoMu.Mod());
				  trigSF_stau_up   = trigSF;
				  trigSF_stau_down = trigSF;
				  trigSF_tau_DM0_up	   = trigSF; 
				  trigSF_tau_DM1_up	   = trigSF; 
				  trigSF_tau_DM10_up   = trigSF; 
				  trigSF_tau_DM11_up   = trigSF; 
				  trigSF_tau_DM0_down  = trigSF; 
				  trigSF_tau_DM1_down  = trigSF; 
				  trigSF_tau_DM10_down = trigSF; 
				  trigSF_tau_DM11_down = trigSF; 
				  trigSF_ele_up	  = trigSF;
				  trigSF_ele_down = trigSF;
				  trigSF_mu_up	  = trigSF;
				  trigSF_mu_down  = trigSF;
				  trigSF_SL_mu_up = trigSF;
				  trigSF_SL_mu_down = trigSF;
				  trigSF_cross_mu_up = trigSF;
				  trigSF_cross_mu_down = trigSF;
				}

			  else if (trgRegions["tau"])
				{
				  trigSF           = singleTauSF[PERIOD].first;
				  trigSFnoMET      = trigSF;
				  trigSFnoTau      = 1.;
				  trigSF_stau_up   = trigSF + singleTauSF[PERIOD].second;
				  trigSF_stau_down = trigSF - singleTauSF[PERIOD].second;
				  trigSF_met_up    = trigSF;
				  trigSF_met_down  = trigSF;
				  trigSF_tau_DM0_up	   = trigSF; 
				  trigSF_tau_DM1_up	   = trigSF; 
				  trigSF_tau_DM10_up   = trigSF; 
				  trigSF_tau_DM11_up   = trigSF; 
				  trigSF_tau_DM0_down  = trigSF; 
				  trigSF_tau_DM1_down  = trigSF; 
				  trigSF_tau_DM10_down = trigSF; 
				  trigSF_tau_DM11_down = trigSF; 
				  trigSF_ele_up	  = trigSF;
				  trigSF_ele_down = trigSF;
				  trigSF_mu_up	  = trigSF;
				  trigSF_mu_down  = trigSF;
				  trigSF_SL_mu_up = trigSF;
				  trigSF_SL_mu_down = trigSF;
				  trigSF_cross_mu_up = trigSF;
				  trigSF_cross_mu_down = trigSF;
				}

			  // DiTau region
			  else {
				assert (trgRegions["legacy"]);

				// last entry is uncertainty: 0 central, +1 up, -1 down
				double SF1 = tauTrgSF_ditau->getSF(tlv_firstLepton.Pt() , DM1, 0);
				double SF2 = tauTrgSF_ditau->getSF(tlv_secondLepton.Pt(), DM2, 0);
			  
				// for each DM, fill a trigSF branch with the up/down values if taus have
				// the corresponding DM, otherwise fill with nominal trigSF value
				vector <double> SF1_up(N_tauhDM, SF1), SF1_down (N_tauhDM, SF1);
				vector <double> SF2_up(N_tauhDM, SF2), SF2_down (N_tauhDM, SF2);

				for (int idm  = 0; idm < N_tauhDM; idm++)
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
				
				trigSF					= SF1         * SF2;
				trigSFnoMET				= trigSF;
				trigSFnoTau				= trigSF;
				trigSF_tau_DM0_up		= SF1_up[0]   * SF2_up[0];
				trigSF_tau_DM1_up		= SF1_up[1]   * SF2_up[1];
				trigSF_tau_DM10_up		= SF1_up[2]   * SF2_up[2];
				trigSF_tau_DM11_up		= SF1_up[3]   * SF2_up[3];
				trigSF_tau_DM0_down		= SF1_down[0] * SF2_down[0];
				trigSF_tau_DM1_down		= SF1_down[1] * SF2_down[1];
				trigSF_tau_DM10_down	= SF1_down[2] * SF2_down[2];
				trigSF_tau_DM11_down	= SF1_down[3] * SF2_down[3];
				trigSF_met_up    = trigSF;
				trigSF_met_down  = trigSF;
				trigSF_stau_up   = trigSF;
				trigSF_stau_down = trigSF;
				trigSF_tau_DM0_up	 = trigSF; 
				trigSF_tau_DM1_up	 = trigSF; 
				trigSF_tau_DM10_up   = trigSF; 
				trigSF_tau_DM11_up   = trigSF; 
				trigSF_tau_DM0_down  = trigSF; 
				trigSF_tau_DM1_down  = trigSF; 
				trigSF_tau_DM10_down = trigSF; 
				trigSF_tau_DM11_down = trigSF; 
				trigSF_ele_up   = trigSF;
				trigSF_ele_down = trigSF;
				trigSF_mu_up    = trigSF;
				trigSF_mu_down  = trigSF;
				trigSF_SL_mu_up = trigSF;
				trigSF_SL_mu_down = trigSF;
				trigSF_cross_mu_up = trigSF;
				trigSF_cross_mu_down = trigSF;
			  }
			}
		  
		  // MuMu Channel
		  else if (pType == 3 && isMC)
			{
			  if (trgRegions["met"])
				{
				  trigSF          = metSF.getSF(vMETnoMu.Mod());
				  trigSFnoMET     = 1.;
				  trigSFnoTau     = trigSF;
				  trigSF_met_up   = trigSF + metSF.getSFError(vMETnoMu.Mod());
				  trigSF_met_down = trigSF - metSF.getSFError(vMETnoMu.Mod());
				  trigSF_tau_DM0_up	   = trigSF; 
				  trigSF_tau_DM1_up	   = trigSF; 
				  trigSF_tau_DM10_up   = trigSF; 
				  trigSF_tau_DM11_up   = trigSF; 
				  trigSF_tau_DM0_down  = trigSF; 
				  trigSF_tau_DM1_down  = trigSF; 
				  trigSF_tau_DM10_down = trigSF; 
				  trigSF_tau_DM11_down = trigSF; 
				  trigSF_ele_up	  = trigSF;
				  trigSF_ele_down = trigSF;
				  trigSF_mu_up	  = trigSF;
				  trigSF_mu_down  = trigSF;
				  trigSF_SL_mu_up = trigSF;
				  trigSF_SL_mu_down = trigSF;
				  trigSF_cross_mu_up = trigSF;
				  trigSF_cross_mu_down = trigSF;
				}
			  else if (trgRegions["legacy"])
				{
				  double SF     = muTrgSF->get_ScaleFactor(     tlv_firstLepton.Pt(), fabs(tlv_firstLepton.Eta()), pType);
				  double SF_Err = muTrgSF->get_ScaleFactorError(tlv_firstLepton.Pt(), fabs(tlv_firstLepton.Eta()), pType);
				  trigSF = SF;
				  trigSFnoMET    = trigSF;
				  trigSFnoTau	 = trigSF;
				  trigSF_mu_up   = SF + 1. * SF_Err;
				  trigSF_mu_down = SF - 1. * SF_Err;
				  trigSF_SL_mu_up = trigSF_mu_up;
				  trigSF_SL_mu_down = trigSF_mu_down;
				  trigSF_cross_mu_up = trigSF;
				  trigSF_cross_mu_down = trigSF;
				  trigSF_ele_up	  = trigSF;
				  trigSF_ele_down = trigSF;
				  trigSF_met_up   = trigSF;
				  trigSF_met_down = trigSF;
				  trigSF_tau_DM0_up	   = trigSF; 
				  trigSF_tau_DM1_up	   = trigSF; 
				  trigSF_tau_DM10_up   = trigSF; 
				  trigSF_tau_DM11_up   = trigSF; 
				  trigSF_tau_DM0_down  = trigSF; 
				  trigSF_tau_DM1_down  = trigSF; 
				  trigSF_tau_DM10_down = trigSF; 
				  trigSF_tau_DM11_down = trigSF; 
				}
			}

		  // EleEle Channel
		  else if (pType == 4 && isMC)
			{
			  if (trgRegions["met"])
				{
				  trigSF          = metSF.getSF(vMETnoMu.Mod());
				  trigSFnoMET     = 1.;
				  trigSFnoTau     = trigSF;
				  trigSF_met_up   = trigSF + metSF.getSFError(vMETnoMu.Mod());
				  trigSF_met_down = trigSF - metSF.getSFError(vMETnoMu.Mod());
				  trigSF_stau_up   = trigSF;
				  trigSF_stau_down = trigSF;
				  trigSF_tau_DM0_up	   = trigSF; 
				  trigSF_tau_DM1_up	   = trigSF; 
				  trigSF_tau_DM10_up   = trigSF; 
				  trigSF_tau_DM11_up   = trigSF; 
				  trigSF_tau_DM0_down  = trigSF; 
				  trigSF_tau_DM1_down  = trigSF; 
				  trigSF_tau_DM10_down = trigSF; 
				  trigSF_tau_DM11_down = trigSF; 
				  trigSF_ele_up	  = trigSF;
				  trigSF_ele_down = trigSF;
				  trigSF_mu_up	  = trigSF;
				  trigSF_mu_down  = trigSF;
				  trigSF_SL_mu_up = trigSF;
				  trigSF_SL_mu_down = trigSF;
				  trigSF_cross_mu_up = trigSF;
				  trigSF_cross_mu_down = trigSF;
				}
			  else if (trgRegions["legacy"]) {
				double SF     = eTrgSF->get_ScaleFactor(     tlv_firstLepton.Pt(), tlv_firstLepton.Eta(), pType);
				double SF_Err = eTrgSF->get_ScaleFactorError(tlv_firstLepton.Pt(), tlv_firstLepton.Eta(), pType);
				trigSF = SF;
				trigSFnoMET          = trigSF;
				trigSFnoTau          = trigSF;
				trigSF_ele_up        = SF + 1. * SF_Err;
				trigSF_ele_down      = SF - 1. * SF_Err;
				trigSF_mu_up	     = trigSF;
				trigSF_mu_down       = trigSF;
				trigSF_SL_mu_up      = trigSF;
				trigSF_SL_mu_down    = trigSF;
				trigSF_cross_mu_up   = trigSF;
				trigSF_cross_mu_down = trigSF;
				trigSF_met_up        = trigSF;
				trigSF_met_down      = trigSF;
				trigSF_stau_up       = trigSF;
				trigSF_stau_down     = trigSF;
				trigSF_tau_DM0_up	 = trigSF; 
				trigSF_tau_DM1_up	 = trigSF; 
				trigSF_tau_DM10_up   = trigSF; 
				trigSF_tau_DM11_up   = trigSF; 
				trigSF_tau_DM0_down  = trigSF; 
				trigSF_tau_DM1_down  = trigSF; 
				trigSF_tau_DM10_down = trigSF; 
				trigSF_tau_DM11_down = trigSF; 
			  }
			}
		} // end if(applytriggers)

	  theSmallTree.m_trigSF					= isMC ? trigSF           : 1.0;
	  theSmallTree.m_trigSFnoMET			= isMC ? trigSFnoMET      : 1.0;
	  theSmallTree.m_trigSFnoTau			= isMC ? trigSFnoTau      : 1.0;
	  theSmallTree.m_trigSF_ele_up			= isMC ? trigSF_ele_up    : 1.0;
	  theSmallTree.m_trigSF_mu_up			= isMC ? trigSF_mu_up     : 1.0;
	  theSmallTree.m_trigSF_SL_mu_up		= isMC ? trigSF_SL_mu_up      : 1.0;
	  theSmallTree.m_trigSF_cross_mu_up		= isMC ? trigSF_cross_mu_up   : 1.0;
	  theSmallTree.m_trigSF_tau_DM0_up		= isMC ? trigSF_tau_DM0_up    : 1.0;
	  theSmallTree.m_trigSF_tau_DM1_up		= isMC ? trigSF_tau_DM1_up    : 1.0;
	  theSmallTree.m_trigSF_tau_DM10_up		= isMC ? trigSF_tau_DM10_up   : 1.0;
	  theSmallTree.m_trigSF_tau_DM11_up		= isMC ? trigSF_tau_DM11_up   : 1.0;
	  theSmallTree.m_trigSF_ele_down		= isMC ? trigSF_ele_down  : 1.0;
	  theSmallTree.m_trigSF_mu_down			= isMC ? trigSF_mu_down   : 1.0;
	  theSmallTree.m_trigSF_SL_mu_down		= isMC ? trigSF_SL_mu_down    : 1.0;
	  theSmallTree.m_trigSF_cross_mu_down	= isMC ? trigSF_cross_mu_down : 1.0;
	  theSmallTree.m_trigSF_tau_DM0_down	= isMC ? trigSF_tau_DM0_down  : 1.0;
	  theSmallTree.m_trigSF_tau_DM1_down	= isMC ? trigSF_tau_DM1_down  : 1.0;
	  theSmallTree.m_trigSF_tau_DM10_down	= isMC ? trigSF_tau_DM10_down : 1.0;
	  theSmallTree.m_trigSF_tau_DM11_down	= isMC ? trigSF_tau_DM11_down : 1.0;
	  theSmallTree.m_trigSF_met_up			= isMC ? trigSF_met_up    : 1.0;
	  theSmallTree.m_trigSF_met_down		= isMC ? trigSF_met_down  : 1.0;
	  theSmallTree.m_trigSF_stau_up			= isMC ? trigSF_stau_up   : 1.0;
	  theSmallTree.m_trigSF_stau_down		= isMC ? trigSF_stau_down : 1.0;

	  theSmallTree.m_totalWeight = (isMC? (59970./7.20811e+10) * theSmallTree.m_MC_weight * theSmallTree.m_PUReweight *
									trigSF * theSmallTree.m_IdFakeSF_deep_2d: 1.0);

	  //total weight used for sync: the denominator must be changed for each sample as h_eff->GetBinContent(1), the numerator is the luminosity

	  // Third lepton veto
	  // loop over leptons
	  vector<pair<float, int> > thirdLeptons ; // pt, idx
	  for (unsigned int iLep = 0 ; (iLep < theBigTree.daughters_px->size ()) ; ++iLep)
		{
		  // skip the H decay candidates
		  if (int (iLep) == firstDaughterIndex || int (iLep) == secondDaughterIndex)
			continue;

		  // remove taus
		  if (theBigTree.particleType->at (iLep) == 2)
			continue;
		  else if (theBigTree.particleType->at (iLep) == 0) // muons
			{
			  // Fra Mar2020: for muon, Tight does not imply Medium so we check both
			  bool passMed = oph.muBaseline (&theBigTree, iLep, 10., muEtaMax,
											 0.3, OfflineProducerHelper::MuMedium,
											 0.3, OfflineProducerHelper::MuHighPt);
			  bool passTig = oph.muBaseline (&theBigTree, iLep, 10., muEtaMax,
											 0.3, OfflineProducerHelper::MuTight,
											 0.3, OfflineProducerHelper::MuHighPt);
			  if (!passMed && !passTig) continue; // if it passes one of the two --> the "if" is false and the lepton is saved as an extra lepton
			}
		  else if (theBigTree.particleType->at (iLep) == 1) // electrons
			{
			  // Fra Mar2020: for electron, we check (mvaEleID-Fall17-iso-V2-wp90 OR (mvaEleID-Fall17-noIso-V2-wp90 AND pfRelIso < 0.3))
			  if (DEBUG) std::cout << "--- Debug for extra electrons:" << std::endl;
			  bool passIsoMVA = oph.eleBaseline(&theBigTree, iLep, 10., eleEtaMax, 0.3,
												OfflineProducerHelper::EMVAMedium, string("Vertex-LepID-pTMin-etaMax"), (DEBUG ? true : false));
			  //bool passNonIsoMVA = oph.eleBaseline (&theBigTree, iLep, 10., eleEtaMax, 0.3,
			  //                     OfflineProducerHelper::EMVAMedium, string("Vertex-pTMin-etaMax-thirdLep"), (DEBUG ? true : false));
			  if (!passIsoMVA) // if it passes --> the "if" is false and the lepton is saved as an extra lepton
				continue; 
			}

		  TLorentzVector tlv_dummyLepton(
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
		  TLorentzVector tlv_dummyLepton(
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
	  float jet_eta_thresh;
	  if (PERIOD=="2018" or PERIOD=="2017") {
		jet_eta_thresh = 2.5;
	  }
	  else if (PERIOD=="2016preVFP" or PERIOD=="2016postVFP") {
		jet_eta_thresh = 2.4;
	  }
	  vector <pair <float, int> > jets_and_sortPar;
	  // loop over jets
	  TLorentzVector jetVecSum (0,0,0,0);
	  for (unsigned int iJet = 0 ; iJet < theBigTree.jets_px->size () ; ++iJet)
		{
		  // JET PF ID cut
		  if (theBigTree.PFjetID->at (iJet) < PFjetID_WP) continue; // 0 ; don't pass PF Jet ID; 1: tight, 2: tightLepVeto

		  TLorentzVector tlv_jet(theBigTree.jets_px->at(iJet), theBigTree.jets_py->at(iJet), theBigTree.jets_pz->at(iJet), theBigTree.jets_e->at(iJet)) ;
		  if (doSmearing) tlv_jet = tlv_jet * smears_AK4[iJet];

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
			  cout << "eta > " <<  jet_eta_thresh << " : " << (TMath::Abs(tlv_jet.Eta()) > jet_eta_thresh) << endl;
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

		  if (TMath::Abs(tlv_jet.Eta()) > jet_eta_thresh) continue;

		  // n bjets candidates
		  if (tlv_jet.Pt () > 20)  ++theSmallTree.m_nbjets20 ;
		  if (tlv_jet.Pt () > 50)  ++theSmallTree.m_nbjets50 ;

		  //SortParameter = (bChoiceFlag == 1 ) ? bTag : Pt ;
		  float sortPar = (bChoiceFlag == 1) ? theBigTree.bDeepFlavor_probb->at(iJet) + theBigTree.bDeepFlavor_probbb->at(iJet) + theBigTree.bDeepFlavor_problepb->at(iJet) : tlv_jet.Pt() ;
		  if (bChoiceFlag != 1 && bChoiceFlag != 2) {
			cout << "** WARNING : bChoiceFlag not known :" << bChoiceFlag << endl;
		  }
		  jets_and_sortPar.push_back (make_pair (sortPar, iJet) );

		} // loop over jets

      // Save PUjetID for jets with pT > 15 GeV for sync with Pisa
	  for (unsigned int iJet = 0 ; iJet < theBigTree.jets_px->size () ; ++iJet)
		{
		  if (theBigTree.PFjetID->at (iJet) < PFjetID_WP) continue; // 0 ; don't pass PF Jet ID; 1: tight, 2: tightLepVeto
		  TLorentzVector tlv_jet(theBigTree.jets_px->at (iJet), theBigTree.jets_py->at (iJet), theBigTree.jets_pz->at (iJet), theBigTree.jets_e->at (iJet));
		  if (doSmearing) tlv_jet = tlv_jet * smears_AK4[iJet];
		  if (tlv_jet.Pt() > 15.) theSmallTree.m_PUjetID.push_back(theBigTree.jets_PUJetIDupdated->at(iJet));
		}

	  theSmallTree.m_nbjetscand = jets_and_sortPar.size();
	  theSmallTree.m_nfatjets = theBigTree.ak8jets_px->size();

	  ec.Increment("TwoJets", EvtW);
	  if (isHHsignal && pairType == genHHDecMode) ecHHsig[genHHDecMode].Increment("TwoJets", EvtW);

	  TLorentzVector tlv_firstBjet, tlv_secondBjet;

	  // sort by first parameter, then pt (dummy if pt order chosen)
	  // bJetSort defined in interface/skimUtils.h
	  sort (jets_and_sortPar.begin(), jets_and_sortPar.end(), bJetSort);
	  
	  if (jets_and_sortPar.size() >= 2)
		{
		  vector<float> bTagWeight				= bTagSFHelper.getEvtWeight(jets_and_sortPar, theBigTree, smears_AK4, pType, bTagSF::central);
		  vector<float> bTagWeight_up			= bTagSFHelper.getEvtWeight(jets_and_sortPar, theBigTree, smears_AK4, pType, bTagSF::up);
		  vector<float> bTagWeight_down			= bTagSFHelper.getEvtWeight(jets_and_sortPar, theBigTree, smears_AK4, pType, bTagSF::down);
		  vector<float> bTagWeightReshapeshifts = bTagSFHelper.getEvtWeightShifted(jets_and_sortPar, theBigTree, smears_AK4);

		  theSmallTree.m_bTagweightL = bTagWeight.at(0);
		  theSmallTree.m_bTagweightM = bTagWeight.at(1);
		  theSmallTree.m_bTagweightT = bTagWeight.at(2);
		  theSmallTree.m_bTagweightReshape = bTagWeight.at(3);

		  theSmallTree.m_bTagweightL_up = bTagWeight_up.at(0);
		  theSmallTree.m_bTagweightM_up = bTagWeight_up.at(1);
		  theSmallTree.m_bTagweightT_up = bTagWeight_up.at(2);

		  theSmallTree.m_bTagweightL_down = bTagWeight_down.at(0);
		  theSmallTree.m_bTagweightM_down = bTagWeight_down.at(1);
		  theSmallTree.m_bTagweightT_down = bTagWeight_down.at(2);

		  theSmallTree.m_bTagweightReshape_jes_up        = bTagWeightReshapeshifts.at(0); 
		  theSmallTree.m_bTagweightReshape_lf_up		 = bTagWeightReshapeshifts.at(1); 
		  theSmallTree.m_bTagweightReshape_hf_up		 = bTagWeightReshapeshifts.at(2); 
		  theSmallTree.m_bTagweightReshape_hfstats1_up	 = bTagWeightReshapeshifts.at(3); 
		  theSmallTree.m_bTagweightReshape_hfstats2_up	 = bTagWeightReshapeshifts.at(4); 
		  theSmallTree.m_bTagweightReshape_lfstats1_up	 = bTagWeightReshapeshifts.at(5); 
		  theSmallTree.m_bTagweightReshape_lfstats2_up	 = bTagWeightReshapeshifts.at(6); 
		  theSmallTree.m_bTagweightReshape_cferr1_up	 = bTagWeightReshapeshifts.at(7); 
		  theSmallTree.m_bTagweightReshape_cferr2_up	 = bTagWeightReshapeshifts.at(8); 
		  theSmallTree.m_bTagweightReshape_jes_down		 = bTagWeightReshapeshifts.at(9); 
		  theSmallTree.m_bTagweightReshape_lf_down		 = bTagWeightReshapeshifts.at(10);
		  theSmallTree.m_bTagweightReshape_hf_down		 = bTagWeightReshapeshifts.at(11);
		  theSmallTree.m_bTagweightReshape_hfstats1_down = bTagWeightReshapeshifts.at(12);
		  theSmallTree.m_bTagweightReshape_hfstats2_down = bTagWeightReshapeshifts.at(13);
		  theSmallTree.m_bTagweightReshape_lfstats1_down = bTagWeightReshapeshifts.at(14);
		  theSmallTree.m_bTagweightReshape_lfstats2_down = bTagWeightReshapeshifts.at(15);
		  theSmallTree.m_bTagweightReshape_cferr1_down	 = bTagWeightReshapeshifts.at(16);
		  theSmallTree.m_bTagweightReshape_cferr2_down	 = bTagWeightReshapeshifts.at(17);
		  theSmallTree.m_bTagweightReshape_jetup1		 = bTagWeightReshapeshifts.at(18);
		  theSmallTree.m_bTagweightReshape_jetup2		 = bTagWeightReshapeshifts.at(19);
		  theSmallTree.m_bTagweightReshape_jetup3		 = bTagWeightReshapeshifts.at(20);
		  theSmallTree.m_bTagweightReshape_jetup4		 = bTagWeightReshapeshifts.at(21);
		  theSmallTree.m_bTagweightReshape_jetup5		 = bTagWeightReshapeshifts.at(22);
		  theSmallTree.m_bTagweightReshape_jetup6		 = bTagWeightReshapeshifts.at(23);
		  theSmallTree.m_bTagweightReshape_jetup7		 = bTagWeightReshapeshifts.at(24);
		  theSmallTree.m_bTagweightReshape_jetup8		 = bTagWeightReshapeshifts.at(25);
		  theSmallTree.m_bTagweightReshape_jetup9		 = bTagWeightReshapeshifts.at(26);
		  theSmallTree.m_bTagweightReshape_jetup10		 = bTagWeightReshapeshifts.at(27);
		  theSmallTree.m_bTagweightReshape_jetup11		 = bTagWeightReshapeshifts.at(28);
		  theSmallTree.m_bTagweightReshape_jetdown1		 = bTagWeightReshapeshifts.at(29);
		  theSmallTree.m_bTagweightReshape_jetdown2		 = bTagWeightReshapeshifts.at(30);
		  theSmallTree.m_bTagweightReshape_jetdown3		 = bTagWeightReshapeshifts.at(31);
		  theSmallTree.m_bTagweightReshape_jetdown4		 = bTagWeightReshapeshifts.at(32);
		  theSmallTree.m_bTagweightReshape_jetdown5		 = bTagWeightReshapeshifts.at(33);
		  theSmallTree.m_bTagweightReshape_jetdown6		 = bTagWeightReshapeshifts.at(34);
		  theSmallTree.m_bTagweightReshape_jetdown7		 = bTagWeightReshapeshifts.at(35);
		  theSmallTree.m_bTagweightReshape_jetdown8		 = bTagWeightReshapeshifts.at(36);
		  theSmallTree.m_bTagweightReshape_jetdown9		 = bTagWeightReshapeshifts.at(37);
		  theSmallTree.m_bTagweightReshape_jetdown10	 = bTagWeightReshapeshifts.at(38);
		  theSmallTree.m_bTagweightReshape_jetdown11	 = bTagWeightReshapeshifts.at(39);
			  
		  // Conversion between LLR and Pisa channel id convention
		  int HHbTag_chn = -1;
		  if      (theSmallTree.m_pairType == 0) HHbTag_chn = 1; //mutau
		  else if (theSmallTree.m_pairType == 1) HHbTag_chn = 0; //etau
		  else if (theSmallTree.m_pairType == 2) HHbTag_chn = 2; //tautau
		  else if (theSmallTree.m_pairType == 3) HHbTag_chn = 3; //mumu
		  assert(HHbTag_chn <= 3 and HHbTag_chn >= 0);

		  // bjet1 and bjet2 indexes
		  int bjet1idx, bjet2idx;
		  std::map<int,float> jets_and_HHbtag;
		  
		  if (HHbTag_chn < 3) { // HH-bTag is for the moment not considering mumu
			// Set HHbtaginterface for ordering jets
			HHbtagTagger.SetInputValues(theBigTree, jets_and_sortPar, HHbTag_chn,
										tlv_firstLepton, tlv_secondLepton, tlv_tauH, tlv_MET, theSmallTree.m_EventNumber, smears_AK4);
			
			// Get HHbtag scores in a map <jet_idx, HHbtag_score>
			jets_and_HHbtag = HHbtagTagger.GetScore();

			std::map<int,float> HHbtag_idx_map = jets_and_HHbtag;
			auto HHbtagmax  = get_max_map(HHbtag_idx_map); // from skimUtils.h
			HHbtag_idx_map.erase(HHbtagmax.first);
			auto HHbtagmax2 = get_max_map(HHbtag_idx_map);

			bjet1idx = HHbtagmax .first;
			bjet2idx = HHbtagmax2.first;

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
		  }
		  else { 
			// Jet IDs of the two highest DeepJet scores
			bjet1idx = jets_and_sortPar.rbegin()[0].second;
			bjet2idx = jets_and_sortPar.rbegin()[1].second;
			if (DEBUG)
			  {
				std::cout << "bjet1 DeepJet score: " << jets_and_sortPar.rbegin()[0].first << std::endl;
				std::cout << "bjet2 DeepJet score: " << jets_and_sortPar.rbegin()[1].first << std::endl;
			  }
		  }

		  // Now that I've selected the bjets build the 4-vectors
		  tlv_firstBjet = TLorentzVector(theBigTree.jets_px->at(bjet1idx), theBigTree.jets_py->at(bjet1idx),
										 theBigTree.jets_pz->at(bjet1idx), theBigTree.jets_e->at(bjet1idx));
		  tlv_secondBjet = TLorentzVector(theBigTree.jets_px->at(bjet2idx), theBigTree.jets_py->at(bjet2idx),
										  theBigTree.jets_pz->at(bjet2idx), theBigTree.jets_e->at(bjet2idx));
		  if (doSmearing) {
			tlv_firstBjet = tlv_firstBjet * smears_AK4[bjet1idx];
			tlv_secondBjet = tlv_secondBjet * smears_AK4[bjet2idx];
		  }

		  if (DEBUG)
			{
			  std::cout << "------BJETS----" << std::endl;
			  std::cout << "B1(idx,pt,eta,phi): "<<bjet1idx<<" "<<tlv_firstBjet.Pt()<<" "<<tlv_firstBjet.Eta()<<" "<<tlv_firstBjet.Phi()<<std::endl;
			  std::cout << "B2(idx,pt,eta,phi): "<<bjet2idx<<" "<<tlv_secondBjet.Pt()<<" "<<tlv_secondBjet.Eta()<<" "<<tlv_secondBjet.Phi()<<std::endl;
			  std::cout << "---------------" << std::endl;
			}

		  bool bjets_gen_matched = ((tlv_firstBjet .DeltaR(vGenB1)<0.5 && tlv_secondBjet.DeltaR(vGenB2)<0.5) ||
									(tlv_firstBjet.DeltaR(vGenB2)<0.5 && tlv_secondBjet.DeltaR(vGenB1)<0.5));
		  bool bjet1_gen_matched = tlv_firstBjet .DeltaR(vGenB1)<0.5 || tlv_firstBjet .DeltaR(vGenB2)<0.5;
		  bool bjet2_gen_matched = tlv_secondBjet.DeltaR(vGenB1)<0.5 || tlv_secondBjet.DeltaR(vGenB2)<0.5;

		  theSmallTree.m_bjets_gen_matched = bjets_gen_matched ? 1 : 0;
		  theSmallTree.m_bjet1_gen_matched = bjet1_gen_matched ? 1 : 0;
		  theSmallTree.m_bjet2_gen_matched = bjet2_gen_matched ? 1 : 0;

		  // ----- up/down variation using 11 JEC sources + total uncertainty
		  // store the up down variations in vectors
		  // last position is total uncertainty
		  pair <vector <double>, vector<double>> unc_first_updown  = JECprovider.getJECUncVectors(bjet1idx, theBigTree);
		  pair <vector <double>, vector<double>> unc_second_updown = JECprovider.getJECUncVectors(bjet2idx, theBigTree);

		  // Total JES up/down variation
		  TLorentzVector tlv_firstBjet_jetupTot    = tlv_firstBjet ;
		  TLorentzVector tlv_firstBjet_jetdownTot  = tlv_firstBjet ;
		  TLorentzVector tlv_secondBjet_jetupTot   = tlv_secondBjet;
		  TLorentzVector tlv_secondBjet_jetdownTot = tlv_secondBjet;
		  TLorentzVector tlv_bH_jetupTot           = tlv_firstBjet + tlv_secondBjet;
		  TLorentzVector tlv_bH_jetdownTot         = tlv_firstBjet + tlv_secondBjet;

		  if (isMC)
			{
			  tlv_firstBjet_jetupTot    = getShiftedJet(tlv_firstBjet , +1., unc_first_updown.first[N_jecSources]);
			  tlv_firstBjet_jetdownTot  = getShiftedJet(tlv_firstBjet , -1., unc_first_updown.second[N_jecSources]);
			  tlv_secondBjet_jetupTot   = getShiftedJet(tlv_secondBjet, +1., unc_second_updown.first[N_jecSources]);
			  tlv_secondBjet_jetdownTot = getShiftedJet(tlv_secondBjet, -1., unc_second_updown.second[N_jecSources]);
			  tlv_bH_jetupTot           = tlv_firstBjet_jetupTot + tlv_secondBjet_jetupTot;
			  tlv_bH_jetdownTot         = tlv_firstBjet_jetdownTot + tlv_secondBjet_jetdownTot;
			}

		  theSmallTree.m_bjet1_pt_jetupTot     = tlv_firstBjet_jetupTot.Pt();
		  theSmallTree.m_bjet1_pt_jetdownTot   = tlv_firstBjet_jetdownTot.Pt();
		  theSmallTree.m_bjet2_pt_jetupTot     = tlv_secondBjet_jetupTot.Pt();
		  theSmallTree.m_bjet2_pt_jetdownTot   = tlv_secondBjet_jetdownTot.Pt();
		  theSmallTree.m_bjet1_e_jetupTot   = tlv_firstBjet_jetupTot.E();
		  theSmallTree.m_bjet1_e_jetdownTot = tlv_firstBjet_jetdownTot.E();
		  theSmallTree.m_bjet2_e_jetupTot   = tlv_secondBjet_jetupTot.E();
		  theSmallTree.m_bjet2_e_jetdownTot = tlv_secondBjet_jetdownTot.E();
		  theSmallTree.m_bH_mass_jetupTot      = tlv_bH_jetupTot.M();
		  theSmallTree.m_bH_mass_jetdownTot    = tlv_bH_jetdownTot.M();

		  vector <TLorentzVector> tlv_firstBjet_jetup   (N_jecSources, tlv_firstBjet);
		  vector <TLorentzVector> tlv_firstBjet_jetdown (N_jecSources, tlv_firstBjet);
		  vector <TLorentzVector> tlv_secondBjet_jetup  (N_jecSources, tlv_secondBjet);
		  vector <TLorentzVector> tlv_secondBjet_jetdown(N_jecSources, tlv_secondBjet);

		  theSmallTree.m_bjet1_pt = tlv_firstBjet.Pt();
		  theSmallTree.m_bjet2_pt = tlv_secondBjet.Pt();

		  TLorentzVector tlv_bH(tlv_firstBjet + tlv_secondBjet);
		  vector <TLorentzVector> tlv_bH_jetup  (N_jecSources, tlv_bH);
		  vector <TLorentzVector> tlv_bH_jetdown(N_jecSources, tlv_bH);

		  //JER: in LLR it is saved as jets_JER = JER (in %) * jet_energy:
		  //https://github.com/LLRCMS/LLRHiggsTauTau/blob/102X_HH/NtupleProducer/plugins/HTauTauNtuplizer.cc#L2316
		  double bjet1_JER = theBigTree.jets_JER->at(bjet1idx);
		  double bjet2_JER = theBigTree.jets_JER->at(bjet2idx);
		  if (doSmearing)
			{
			  bjet1_JER = Smearer_AK4.getResolution(bjet1idx, theBigTree) * tlv_firstBjet.E();
			  bjet2_JER = Smearer_AK4.getResolution(bjet2idx, theBigTree) * tlv_secondBjet.E();
			}
		  theSmallTree.m_bjet1_JER = bjet1_JER;
		  theSmallTree.m_bjet2_JER = bjet2_JER;

		  // Total JES up/down variation of JER
		  double bjet1_JER_jetupTot   = bjet1_JER;
		  double bjet2_JER_jetupTot	  = bjet2_JER;
		  double bjet1_JER_jetdownTot = bjet1_JER;
		  double bjet2_JER_jetdownTot = bjet2_JER;

		  if (isMC)
			{
			  bjet1_JER_jetupTot   = bjet1_JER * tlv_firstBjet_jetupTot.E()	   / tlv_firstBjet.E();
			  bjet2_JER_jetupTot   = bjet2_JER * tlv_secondBjet_jetupTot.E()   / tlv_secondBjet.E();
			  bjet1_JER_jetdownTot = bjet1_JER * tlv_firstBjet_jetdownTot.E()  / tlv_firstBjet.E();
			  bjet2_JER_jetdownTot = bjet2_JER * tlv_secondBjet_jetdownTot.E() / tlv_secondBjet.E();
			}

		  theSmallTree.m_bjet1_JER_jetupTot   = bjet1_JER_jetupTot;
		  theSmallTree.m_bjet2_JER_jetupTot   = bjet2_JER_jetupTot;
		  theSmallTree.m_bjet1_JER_jetdownTot = bjet1_JER_jetdownTot;
		  theSmallTree.m_bjet2_JER_jetdownTot = bjet2_JER_jetdownTot;

		  //compute and store variations
		  for (int isource = 0; isource < N_jecSources; isource++)
			{
			  // variations propagated to bH
			  tlv_bH_jetup[isource]   = tlv_firstBjet_jetup.at(isource)   + tlv_secondBjet_jetup.at(isource);
			  tlv_bH_jetdown[isource] = tlv_firstBjet_jetdown.at(isource) + tlv_secondBjet_jetdown.at(isource);
			}

		  if (isMC)
			{
			  for (int isource = 0; isource < N_jecSources; isource++)
				{
				  // compute all shifted bjets
				  tlv_firstBjet_jetup[isource]   = getShiftedJet(tlv_firstBjet, +1., unc_first_updown.first[isource]);
				  tlv_firstBjet_jetdown[isource] = getShiftedJet(tlv_firstBjet, -1., unc_first_updown.second[isource]);
				  tlv_secondBjet_jetup[isource]  = getShiftedJet(tlv_secondBjet, +1., unc_second_updown.first[isource]);
				  tlv_secondBjet_jetdown[isource]= getShiftedJet(tlv_secondBjet, -1., unc_second_updown.second[isource]);

				  // variations propagated to bH
				  tlv_bH_jetup[isource]   = tlv_firstBjet_jetup.at(isource) + tlv_secondBjet_jetup.at(isource);
				  tlv_bH_jetdown[isource] = tlv_firstBjet_jetdown.at(isource) + tlv_secondBjet_jetdown.at(isource);
				}
			}

		  for (int isource = 0; isource < N_jecSources; isource++)
			{
			  theSmallTree.m_bH_mass_jetup.push_back((tlv_bH_jetup[isource]).M());
			  theSmallTree.m_bH_mass_jetdown.push_back((tlv_bH_jetdown[isource]).M());
			  theSmallTree.m_bH_pt_jetup.push_back((tlv_bH_jetup[isource]).Pt());
			  theSmallTree.m_bH_pt_jetdown.push_back((tlv_bH_jetdown[isource]).Pt());

			  // JER variations
			  theSmallTree.m_bjet1_JER_jetup.push_back  (bjet1_JER * tlv_firstBjet_jetup[isource].E()    /  tlv_firstBjet.E());
			  theSmallTree.m_bjet1_JER_jetdown.push_back(bjet1_JER * tlv_firstBjet_jetdown[isource].E()  /  tlv_firstBjet.E());
			  theSmallTree.m_bjet2_JER_jetup.push_back  (bjet2_JER * tlv_secondBjet_jetup[isource].E()   /  tlv_secondBjet.E());
			  theSmallTree.m_bjet2_JER_jetdown.push_back(bjet2_JER * tlv_secondBjet_jetdown[isource].E() /  tlv_secondBjet.E());
			}

		  theSmallTree.m_bjet1_pt_jetup1  = tlv_firstBjet_jetup[0].Pt();
		  theSmallTree.m_bjet1_pt_jetup2  = tlv_firstBjet_jetup[1].Pt();
		  theSmallTree.m_bjet1_pt_jetup3  = tlv_firstBjet_jetup[2].Pt();
		  theSmallTree.m_bjet1_pt_jetup4  = tlv_firstBjet_jetup[3].Pt();
		  theSmallTree.m_bjet1_pt_jetup5  = tlv_firstBjet_jetup[4].Pt();
		  theSmallTree.m_bjet1_pt_jetup6  = tlv_firstBjet_jetup[5].Pt();
		  theSmallTree.m_bjet1_pt_jetup7  = tlv_firstBjet_jetup[6].Pt();
		  theSmallTree.m_bjet1_pt_jetup8  = tlv_firstBjet_jetup[7].Pt();
		  theSmallTree.m_bjet1_pt_jetup9  = tlv_firstBjet_jetup[8].Pt();
		  theSmallTree.m_bjet1_pt_jetup10 = tlv_firstBjet_jetup[9].Pt();
		  theSmallTree.m_bjet1_pt_jetup11 = tlv_firstBjet_jetup[10].Pt();
		  theSmallTree.m_bjet1_e_jetup1   = tlv_firstBjet_jetup[0].E();
		  theSmallTree.m_bjet1_e_jetup2   = tlv_firstBjet_jetup[1].E();
		  theSmallTree.m_bjet1_e_jetup3   = tlv_firstBjet_jetup[2].E();
		  theSmallTree.m_bjet1_e_jetup4   = tlv_firstBjet_jetup[3].E();
		  theSmallTree.m_bjet1_e_jetup5   = tlv_firstBjet_jetup[4].E();
		  theSmallTree.m_bjet1_e_jetup6   = tlv_firstBjet_jetup[5].E();
		  theSmallTree.m_bjet1_e_jetup7   = tlv_firstBjet_jetup[6].E();
		  theSmallTree.m_bjet1_e_jetup8   = tlv_firstBjet_jetup[7].E();
		  theSmallTree.m_bjet1_e_jetup9   = tlv_firstBjet_jetup[8].E();
		  theSmallTree.m_bjet1_e_jetup10  = tlv_firstBjet_jetup[9].E();
		  theSmallTree.m_bjet1_e_jetup11  = tlv_firstBjet_jetup[10].E();

		  theSmallTree.m_bjet2_pt_jetup1  = tlv_secondBjet_jetup[0].Pt();
		  theSmallTree.m_bjet2_pt_jetup2  = tlv_secondBjet_jetup[1].Pt();
		  theSmallTree.m_bjet2_pt_jetup3  = tlv_secondBjet_jetup[2].Pt();
		  theSmallTree.m_bjet2_pt_jetup4  = tlv_secondBjet_jetup[3].Pt();
		  theSmallTree.m_bjet2_pt_jetup5  = tlv_secondBjet_jetup[4].Pt();
		  theSmallTree.m_bjet2_pt_jetup6  = tlv_secondBjet_jetup[5].Pt();
		  theSmallTree.m_bjet2_pt_jetup7  = tlv_secondBjet_jetup[6].Pt();
		  theSmallTree.m_bjet2_pt_jetup8  = tlv_secondBjet_jetup[7].Pt();
		  theSmallTree.m_bjet2_pt_jetup9  = tlv_secondBjet_jetup[8].Pt();
		  theSmallTree.m_bjet2_pt_jetup10 = tlv_secondBjet_jetup[9].Pt();
		  theSmallTree.m_bjet2_pt_jetup11 = tlv_secondBjet_jetup[10].Pt();
		  theSmallTree.m_bjet2_e_jetup1   = tlv_secondBjet_jetup[0].E();
		  theSmallTree.m_bjet2_e_jetup2   = tlv_secondBjet_jetup[1].E();
		  theSmallTree.m_bjet2_e_jetup3   = tlv_secondBjet_jetup[2].E();
		  theSmallTree.m_bjet2_e_jetup4   = tlv_secondBjet_jetup[3].E();
		  theSmallTree.m_bjet2_e_jetup5   = tlv_secondBjet_jetup[4].E();
		  theSmallTree.m_bjet2_e_jetup6   = tlv_secondBjet_jetup[5].E();
		  theSmallTree.m_bjet2_e_jetup7   = tlv_secondBjet_jetup[6].E();
		  theSmallTree.m_bjet2_e_jetup8   = tlv_secondBjet_jetup[7].E();
		  theSmallTree.m_bjet2_e_jetup9   = tlv_secondBjet_jetup[8].E();
		  theSmallTree.m_bjet2_e_jetup10  = tlv_secondBjet_jetup[9].E();
		  theSmallTree.m_bjet2_e_jetup11  = tlv_secondBjet_jetup[10].E();

		  theSmallTree.m_bjet1_pt_jetdown1  = tlv_firstBjet_jetdown[0].Pt();
		  theSmallTree.m_bjet1_pt_jetdown2	= tlv_firstBjet_jetdown[1].Pt();
		  theSmallTree.m_bjet1_pt_jetdown3	= tlv_firstBjet_jetdown[2].Pt();
		  theSmallTree.m_bjet1_pt_jetdown4	= tlv_firstBjet_jetdown[3].Pt();
		  theSmallTree.m_bjet1_pt_jetdown5	= tlv_firstBjet_jetdown[4].Pt();
		  theSmallTree.m_bjet1_pt_jetdown6	= tlv_firstBjet_jetdown[5].Pt();
		  theSmallTree.m_bjet1_pt_jetdown7	= tlv_firstBjet_jetdown[6].Pt();
		  theSmallTree.m_bjet1_pt_jetdown8	= tlv_firstBjet_jetdown[7].Pt();
		  theSmallTree.m_bjet1_pt_jetdown9	= tlv_firstBjet_jetdown[8].Pt();
		  theSmallTree.m_bjet1_pt_jetdown10 = tlv_firstBjet_jetdown[9].Pt();
		  theSmallTree.m_bjet1_pt_jetdown11 = tlv_firstBjet_jetdown[10].Pt();
		  theSmallTree.m_bjet1_e_jetdown1   = tlv_firstBjet_jetdown[0].E();
		  theSmallTree.m_bjet1_e_jetdown2	= tlv_firstBjet_jetdown[1].E();
		  theSmallTree.m_bjet1_e_jetdown3	= tlv_firstBjet_jetdown[2].E();
		  theSmallTree.m_bjet1_e_jetdown4	= tlv_firstBjet_jetdown[3].E();
		  theSmallTree.m_bjet1_e_jetdown5	= tlv_firstBjet_jetdown[4].E();
		  theSmallTree.m_bjet1_e_jetdown6	= tlv_firstBjet_jetdown[5].E();
		  theSmallTree.m_bjet1_e_jetdown7	= tlv_firstBjet_jetdown[6].E();
		  theSmallTree.m_bjet1_e_jetdown8	= tlv_firstBjet_jetdown[7].E();
		  theSmallTree.m_bjet1_e_jetdown9	= tlv_firstBjet_jetdown[8].E();
		  theSmallTree.m_bjet1_e_jetdown10  = tlv_firstBjet_jetdown[9].E();
		  theSmallTree.m_bjet1_e_jetdown11  = tlv_firstBjet_jetdown[10].E();

		  theSmallTree.m_bjet2_pt_jetdown1  = tlv_secondBjet_jetdown[0].Pt();
		  theSmallTree.m_bjet2_pt_jetdown2	= tlv_secondBjet_jetdown[1].Pt();
		  theSmallTree.m_bjet2_pt_jetdown3	= tlv_secondBjet_jetdown[2].Pt();
		  theSmallTree.m_bjet2_pt_jetdown4	= tlv_secondBjet_jetdown[3].Pt();
		  theSmallTree.m_bjet2_pt_jetdown5	= tlv_secondBjet_jetdown[4].Pt();
		  theSmallTree.m_bjet2_pt_jetdown6	= tlv_secondBjet_jetdown[5].Pt();
		  theSmallTree.m_bjet2_pt_jetdown7	= tlv_secondBjet_jetdown[6].Pt();
		  theSmallTree.m_bjet2_pt_jetdown8	= tlv_secondBjet_jetdown[7].Pt();
		  theSmallTree.m_bjet2_pt_jetdown9	= tlv_secondBjet_jetdown[8].Pt();
		  theSmallTree.m_bjet2_pt_jetdown10 = tlv_secondBjet_jetdown[9].Pt();
		  theSmallTree.m_bjet2_pt_jetdown11 = tlv_secondBjet_jetdown[10].Pt();
		  theSmallTree.m_bjet2_e_jetdown1   = tlv_secondBjet_jetdown[0].E();
		  theSmallTree.m_bjet2_e_jetdown2	= tlv_secondBjet_jetdown[1].E();
		  theSmallTree.m_bjet2_e_jetdown3	= tlv_secondBjet_jetdown[2].E();
		  theSmallTree.m_bjet2_e_jetdown4	= tlv_secondBjet_jetdown[3].E();
		  theSmallTree.m_bjet2_e_jetdown5	= tlv_secondBjet_jetdown[4].E();
		  theSmallTree.m_bjet2_e_jetdown6	= tlv_secondBjet_jetdown[5].E();
		  theSmallTree.m_bjet2_e_jetdown7	= tlv_secondBjet_jetdown[6].E();
		  theSmallTree.m_bjet2_e_jetdown8	= tlv_secondBjet_jetdown[7].E();
		  theSmallTree.m_bjet2_e_jetdown9	= tlv_secondBjet_jetdown[8].E();
		  theSmallTree.m_bjet2_e_jetdown10  = tlv_secondBjet_jetdown[9].E();
		  theSmallTree.m_bjet2_e_jetdown11  = tlv_secondBjet_jetdown[10].E();
		  
		  // also store flat bH_mass variations, because it's needed for ellyptic mass cut
		  theSmallTree.m_bH_mass_jetup1    = tlv_bH_jetup[0].M();
		  theSmallTree.m_bH_mass_jetup2	   = tlv_bH_jetup[1].M();
		  theSmallTree.m_bH_mass_jetup3	   = tlv_bH_jetup[2].M();
		  theSmallTree.m_bH_mass_jetup4	   = tlv_bH_jetup[3].M();
		  theSmallTree.m_bH_mass_jetup5	   = tlv_bH_jetup[4].M();
		  theSmallTree.m_bH_mass_jetup6	   = tlv_bH_jetup[5].M();
		  theSmallTree.m_bH_mass_jetup7	   = tlv_bH_jetup[6].M();
		  theSmallTree.m_bH_mass_jetup8	   = tlv_bH_jetup[7].M();
		  theSmallTree.m_bH_mass_jetup9	   = tlv_bH_jetup[8].M();
		  theSmallTree.m_bH_mass_jetup10   = tlv_bH_jetup[9].M();
		  theSmallTree.m_bH_mass_jetup11   = tlv_bH_jetup[10].M();

		  theSmallTree.m_bH_mass_jetdown1  = tlv_bH_jetdown[0].M();
		  theSmallTree.m_bH_mass_jetdown2  = tlv_bH_jetdown[1].M();
		  theSmallTree.m_bH_mass_jetdown3  = tlv_bH_jetdown[2].M();
		  theSmallTree.m_bH_mass_jetdown4  = tlv_bH_jetdown[3].M();
		  theSmallTree.m_bH_mass_jetdown5  = tlv_bH_jetdown[4].M();
		  theSmallTree.m_bH_mass_jetdown6  = tlv_bH_jetdown[5].M();
		  theSmallTree.m_bH_mass_jetdown7  = tlv_bH_jetdown[6].M();
		  theSmallTree.m_bH_mass_jetdown8  = tlv_bH_jetdown[7].M();
		  theSmallTree.m_bH_mass_jetdown9  = tlv_bH_jetdown[8].M();
		  theSmallTree.m_bH_mass_jetdown10 = tlv_bH_jetdown[9].M();
		  theSmallTree.m_bH_mass_jetdown11 = tlv_bH_jetdown[10].M();
		  
		  theSmallTree.m_bjet1_pt   = tlv_firstBjet.Pt () ;
		  theSmallTree.m_bjet1_eta  = tlv_firstBjet.Eta () ;
		  theSmallTree.m_bjet1_phi  = tlv_firstBjet.Phi () ;
		  theSmallTree.m_bjet1_e    = tlv_firstBjet.E () ;
		  theSmallTree.m_bjet1_bID_deepFlavor  = theBigTree.bDeepFlavor_probb->at(bjet1idx) + theBigTree.bDeepFlavor_probbb->at(bjet1idx) + theBigTree.bDeepFlavor_problepb->at(bjet1idx);
		  theSmallTree.m_bjet1_cID_deepFlavor  = theBigTree.bDeepFlavor_probc->at(bjet1idx);
		  theSmallTree.m_bjet1_CvsL = getCvsL(theBigTree, bjet1idx);
		  theSmallTree.m_bjet1_CvsB = getCvsB(theBigTree, bjet1idx);
		  theSmallTree.m_bjet1_bMVAID  = theBigTree.pfCombinedMVAV2BJetTags->at (bjet1idx) ;
		  theSmallTree.m_bjet1_PUjetIDupdated = theBigTree.jets_PUJetIDupdated->at(bjet1idx);
		  theSmallTree.m_bjet1_flav = theBigTree.jets_HadronFlavour->at (bjet1idx) ;
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
		  theSmallTree.m_bjet2_bID_deepFlavor  = theBigTree.bDeepFlavor_probb->at(bjet2idx) + theBigTree.bDeepFlavor_probbb->at(bjet2idx) + theBigTree.bDeepFlavor_problepb->at(bjet2idx);
		  theSmallTree.m_bjet2_cID_deepFlavor  = theBigTree.bDeepFlavor_probc->at(bjet2idx) ;
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
				  TLorentzVector gen(theBigTree.genjet_px->at(mcind),theBigTree.genjet_py->at(mcind),
									 theBigTree.genjet_pz->at(mcind),theBigTree.genjet_e->at(mcind));
				  theSmallTree.m_genjet1_pt = gen.Pt();
				  theSmallTree.m_genjet1_eta = gen.Eta();
				  theSmallTree.m_genjet1_phi = gen.Phi();
				  theSmallTree.m_genjet1_e = gen.E();
				  if (gen.Pt() > 8) hasgj1 = true;
				}

			  mcind = theBigTree.jets_genjetIndex->at(bjet2idx);
			  if (mcind>=0)
				{
				  TLorentzVector gen(theBigTree.genjet_px->at(mcind),theBigTree.genjet_py->at(mcind),
									 theBigTree.genjet_pz->at(mcind),theBigTree.genjet_e->at(mcind));
				  theSmallTree.m_genjet2_pt = gen.Pt();
				  theSmallTree.m_genjet2_eta = gen.Eta();
				  theSmallTree.m_genjet2_phi = gen.Phi();
				  theSmallTree.m_genjet2_e = gen.E();
				  if (gen.Pt() > 8) hasgj2 = true;
				}
			}
		  theSmallTree.m_bjet1_hasgenjet = hasgj1;
		  theSmallTree.m_bjet2_hasgenjet = hasgj2;

		  // Save HT_20 and HT_50 (with cut on |eta|<4.7)
		  TLorentzVector jetVecSum (0,0,0,0);
		  for (unsigned int iJet = 0 ; iJet < theBigTree.jets_px->size () ; ++iJet)
			{
			  // JET PF ID cut
			  if (theBigTree.PFjetID->at (iJet) < PFjetID_WP) continue; // 0 ; don't pass PF Jet ID; 1: tight, 2: tightLepVeto

			  // Build the jet TLorentzVector
			  TLorentzVector tlv_jet(theBigTree.jets_px->at(iJet), theBigTree.jets_py->at(iJet),
									 theBigTree.jets_pz->at(iJet), theBigTree.jets_e->at(iJet)) ;
			  if (doSmearing) tlv_jet = tlv_jet * smears_AK4[iJet];

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

			  if (tlv_jet.Pt () > 50)
				{
				  ++theSmallTree.m_njets50 ;
				  theSmallTree.m_HT50 += tlv_jet.Pt() ;
				}
			}
		  theSmallTree.m_HT20Full = theSmallTree.m_HT20 + tlv_firstLepton.Pt() + tlv_secondLepton.Pt() ;
		  theSmallTree.m_jet20centrality = jetVecSum.Pt() / theSmallTree.m_HT20Full ;

		  theSmallTree.m_bH_pt	 = tlv_bH.Pt();
		  theSmallTree.m_bH_eta	 = tlv_bH.Eta();
		  theSmallTree.m_bH_phi	 = tlv_bH.Phi();
		  theSmallTree.m_bH_e	 = tlv_bH.E();
		  theSmallTree.m_bH_mass = tlv_bH.M();

		  theSmallTree.m_bH_MET_deltaEta    = std::abs(tlv_bH.Eta()); // since MET.Eta()==0 by definition, dEta(bH,MET)=|bH.Eta()|
		  theSmallTree.m_bH_MET_deltaR      = tlv_bH.DeltaR(tlv_MET);
		  theSmallTree.m_bH_tauH_MET_deltaR = tlv_bH.DeltaR(tlv_tauH + tlv_MET);

		  TLorentzVector tlv_HH(tlv_bH + tlv_tauH);
		  theSmallTree.m_HH_pt		 = tlv_HH.Pt();
		  theSmallTree.m_HH_eta		 = tlv_HH.Eta();
		  theSmallTree.m_HH_phi		 = tlv_HH.Phi();
		  theSmallTree.m_HH_e		 = tlv_HH.E();
		  theSmallTree.m_HH_mass	 = tlv_HH.M();
		  theSmallTree.m_HH_deltaR	 = tlv_bH.DeltaR(tlv_tauH);

		  // in case the SVFIT mass is calculated
		  if (theBigTree.SVfitMass->at(chosenTauPair) > -900. || (doSmearing && theSmallTree.m_tauH_SVFIT_mass > 0))
			{
			  TLorentzVector tlv_HHsvfit(tlv_bH + tlv_tauH_SVFIT);
			  theSmallTree.m_HHsvfit_pt   = tlv_HHsvfit.Pt () ;
			  theSmallTree.m_HHsvfit_eta  = tlv_HHsvfit.Eta () ;
			  theSmallTree.m_HHsvfit_phi  = tlv_HHsvfit.Phi () ;
			  theSmallTree.m_HHsvfit_e    = tlv_HHsvfit.E () ;
			  theSmallTree.m_HHsvfit_mass = tlv_HHsvfit.M () ;
			} // in case the SVFIT mass is calculated

		  // compute HHKinFit -- ask a reasonable mass window to suppress most error messages
		  bool wrongHHK=false;
		  float HHKmass = -999;
		  float HHKChi2 = -999;
		  if (runHHKinFit && pairType <= 3) // FIXME: temporary
			{
			  const TMatrixD stableMetCov = metcov;
			  HHKinFit2::HHKinFitMasterHeavyHiggs kinFits = HHKinFit2::HHKinFitMasterHeavyHiggs(tlv_firstBjet, tlv_secondBjet, tlv_firstLepton, tlv_secondLepton, vMET, stableMetCov, bjet1_JER, bjet2_JER) ;
			  kinFits.   addHypo(hypo_mh1,hypo_mh2);

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
				  cout<<"(Px,Py) "<<","<<tlv_MET.Px()<<","<<tlv_MET.Py()<<endl;
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
				  cout<<"INVME (E,Px,Py,Pz,M) "<<","<<tlv_MET.Px()<<","<<tlv_MET.Py()<<endl;
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
				  cout<<"ERANGE (E,Px,Py,Pz,M) "<<","<<tlv_MET.Px()<<","<<tlv_MET.Py()<<endl;
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
				  cout<<"ECON (E,Px,Py,Pz,M) "<<","<<tlv_MET.Px()<<","<<tlv_MET.Py()<<endl;
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

		  theSmallTree.m_HHKin_mass = HHKmass;//kinFits.getMH () ;
		  theSmallTree.m_HHKin_chi2 = HHKChi2;//kinFits.getChi2 () ;

		  theSmallTree.m_HH_deltaPhi		 = deltaPhi (tlv_bH.Phi (), tlv_tauH.Phi ()) ;
		  theSmallTree.m_HH_deltaEta		 = fabs(tlv_bH.Eta()- tlv_tauH.Eta ()) ;
		  theSmallTree.m_HHsvfit_deltaPhi	 = deltaPhi (tlv_bH.Phi (), tlv_tauH_SVFIT.Phi ()) ;
		  theSmallTree.m_bHMet_deltaPhi		 = deltaPhi (tlv_MET.Phi(), tlv_bH.Phi ()) ;
		  theSmallTree.m_dib_deltaPhi		 = deltaPhi (tlv_firstBjet.Phi (), tlv_secondBjet.Phi ()) ;
		  theSmallTree.m_dib_deltaEta		 = fabs(tlv_firstBjet.Eta()-tlv_secondBjet.Eta ()) ;
		  theSmallTree.m_dib_deltaR			 = tlv_firstBjet.DeltaR(tlv_secondBjet) ;
		  theSmallTree.m_dib_deltaR_per_bHpt = theSmallTree.m_dib_deltaR * tlv_bH.Pt();

		  vector <float> dRBTau;
		  dRBTau.push_back (tlv_firstLepton.DeltaR(tlv_firstBjet));
		  dRBTau.push_back (tlv_firstLepton.DeltaR(tlv_secondBjet));
		  dRBTau.push_back (tlv_secondLepton.DeltaR(tlv_firstBjet));
		  dRBTau.push_back (tlv_secondLepton.DeltaR(tlv_secondBjet));
		  theSmallTree.m_btau_deltaRmin = *std::min_element(dRBTau.begin(), dRBTau.end());
		  theSmallTree.m_btau_deltaRmax = *std::max_element(dRBTau.begin(), dRBTau.end());

		  // loop over jets
		  int genjets = 0;
		  for (unsigned int iJet = 0; (iJet < theBigTree.jets_px->size ()) && (theSmallTree.m_njets < maxNjetsSaved); ++iJet)
			{
			  // PG filter jets at will
			  if (theBigTree.PFjetID->at (iJet) < PFjetID_WP) continue; // 0 ; don't pass PF Jet ID; 1: tight, 2: tightLepVeto

			  // skip the H decay candidates
			  if (int (iJet) == bjet1idx)
				{
				  continue;
				}
			  else if(int (iJet) == bjet2idx)
				{
				  continue;
				}
			  TLorentzVector tlv_dummyJet(theBigTree.jets_px->at(iJet), theBigTree.jets_py->at(iJet), theBigTree.jets_pz->at(iJet), theBigTree.jets_e->at(iJet));
			  if (doSmearing) tlv_dummyJet = tlv_dummyJet * smears_AK4[iJet];

			  // Apply PUjetID only to jets with pt < 50 GeV ( https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorkingLegacyRun2#Jets )
			  // PU jet ID WP = 2: loose
			  if (PUjetID_WP > -1)
				{
				  if (!(CheckBit(theBigTree.jets_PUJetIDupdated_WP->at(iJet), PUjetID_WP)) and tlv_dummyJet.Pt()<50.) continue;
				}

			  // remove jets that overlap with the tau selected in the leg 1 and 2
			  if (tlv_firstLepton.DeltaR(tlv_dummyJet) < lepCleaningCone)
				{
				  continue;
				}
			  if (tlv_secondLepton.DeltaR(tlv_dummyJet) < lepCleaningCone)
				{
				  continue;
				}

			  // find matching gen jet
			  bool hasgj = false;
			  if (isMC)
				{
				  int mcind = theBigTree.jets_genjetIndex->at(iJet);
				  if (mcind>=0)
					{
					  TLorentzVector thisGenJet(theBigTree.genjet_px->at(mcind), theBigTree.genjet_py->at(mcind),
												theBigTree.genjet_pz->at(mcind), theBigTree.genjet_e->at(mcind));
					  if (thisGenJet.Pt() > 8)
						{
						  hasgj = true;
						}
					  genjets ++;
					}
				}

			  theSmallTree.m_jets_pt.push_back (tlv_dummyJet.Pt ()) ;
			  theSmallTree.m_jets_eta.push_back (tlv_dummyJet.Eta ()) ;
			  theSmallTree.m_jets_phi.push_back (tlv_dummyJet.Phi ()) ;
			  theSmallTree.m_jets_e.push_back (tlv_dummyJet.E ()) ;
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

		  // --------------------------------------------
		  // PUjetIDSFprovider
		  if (isMC)
			{
			  std::vector<float> PUjetID_SF_values = PUjetIDSFprovider.getEvtWeight(theBigTree, tlv_firstLepton, tlv_secondLepton,
																					smears_AK4);
			  theSmallTree.m_PUjetID_SF      = PUjetID_SF_values.at(0);
			  theSmallTree.m_PUjetID_SF_up   = PUjetID_SF_values.at(1);
			  theSmallTree.m_PUjetID_SF_down = PUjetID_SF_values.at(2);

			  if (DEBUG)
				{
				  std::cout << "---- PUjetID_SF debug ----" << std::endl;
				  std::cout << "PUjetID_SF     : " << theSmallTree.m_PUjetID_SF << std::endl;
				  std::cout << "PUjetID_SF_up  : " << theSmallTree.m_PUjetID_SF_up << std::endl;
				  std::cout << "PUjetID_SF_down: " << theSmallTree.m_PUjetID_SF_down << std::endl;
				  std::cout << "--------------------------" << std::endl;
				}
			}

		  // --------------------------------------------
		  // Save smear factors in smallTree
		  if (doSmearing)
			{
			  theSmallTree.m_bjet1_smearFactor   = smears_AK4[bjet1idx];
			  theSmallTree.m_bjet2_smearFactor   = smears_AK4[bjet2idx];
			}

		  // --------------------------------------------
		  // Save HHbtag values in smallTree
		  // b-jet 1
		  if (pType < 3)
			{
			  if (jets_and_HHbtag.find(bjet1idx) != jets_and_HHbtag.end()) {
				theSmallTree.m_bjet1_HHbtag = jets_and_HHbtag[bjet1idx];
			  }
			  else {
				std::cout << "** WARNING: HHbtag score not found for bjet1, setting to -1!" << endl;
				theSmallTree.m_bjet1_HHbtag = -2.;
			  }

			  if (jets_and_HHbtag.find(bjet2idx) != jets_and_HHbtag.end()) {
				theSmallTree.m_bjet2_HHbtag = jets_and_HHbtag[bjet2idx];
			  }
			  else {
				std::cout << "** WARNING: HHbtag score not found for bjet2, setting to -1!" << endl;
				theSmallTree.m_bjet2_HHbtag = -2.;
			  }
			}
		  else {
			theSmallTree.m_bjet1_HHbtag = -3.;
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
			  if (doSmearing) tlv_dummyJet = tlv_dummyJet * smears_AK4[iJet];

			  // Apply PUjetID only to jets with pt < 50 GeV ( https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorkingLegacyRun2#Jets )
			  // PU jet ID WP = 2: loose
			  if (PUjetID_WP > -1)
				{
				  if ( !(CheckBit(theBigTree.jets_PUJetIDupdated_WP->at(iJet), PUjetID_WP)) && tlv_dummyJet.Pt()<50.) continue;
				}

			  // remove jets that overlap with the tau selected in the leg 1 and 2
			  if (tlv_firstLepton.DeltaR(tlv_dummyJet)  < lepCleaningCone) continue;
			  if (tlv_secondLepton.DeltaR(tlv_dummyJet) < lepCleaningCone) continue;

			  if (pType < 3 and jets_and_HHbtag.find(iJet) != jets_and_HHbtag.end())
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

			  theSmallTree.m_smearFactor_AK4.push_back(smears_AK4[iJet]);
			}

		  if (DEBUG)
			{
			  std::cout << "---- HHbtag debug ----" << std::endl;
			  std::cout << "b1   - idx: " << bjet1idx << " HHbtag: " << theSmallTree.m_bjet1_HHbtag << std::endl;
			  std::cout << "b2   - idx: " << bjet2idx << " HHbtag: " << theSmallTree.m_bjet2_HHbtag << std::endl;
			  std::cout << "----------------------" << std::endl;
			}
		} // end if (jets_and_sortPar.size () >= 2)
	  

	  // Boosted section
	  theSmallTree.m_isBoosted = 0;
	  vector<pair<float, int>> fatjets_pT, fatjets_bTag;
	  for (unsigned ifj=0; ifj < theBigTree.ak8jets_px->size(); ++ifj)
		{
		  TLorentzVector tlv_fj((*theBigTree.ak8jets_px)[ifj], (*theBigTree.ak8jets_py)[ifj],
								(*theBigTree.ak8jets_pz)[ifj], (*theBigTree.ak8jets_e)[ifj]);

		  smears_AK8[ifj] = doSmearing ? Smearer_AK8.getSmearFactor(tlv_fj, theBigTree, true) : 1.;
		  theSmallTree.m_smearFactor_AK8.push_back(smears_AK8[ifj]);

		  if (doSmearing) {
			tlv_fj *= smears_AK8[ifj];
		  }
		  
		  if (tlv_fj.Pt() < 250 or theBigTree.ak8jets_SoftDropMass->at(ifj) < 30 or
			  tlv_fj.DeltaR(tlv_firstLepton) < 0.8 or tlv_fj.DeltaR(tlv_secondLepton) < 0.8) {
			continue;
		  }
			    
		  fatjets_pT.push_back(make_pair(tlv_fj.Pt(), ifj));
		  fatjets_bTag.push_back(make_pair( theBigTree.ak8jets_particleNetMDJetTags_probXbb->at(ifj) /
											(theBigTree.ak8jets_particleNetMDJetTags_probXbb->at(ifj)+theBigTree.ak8jets_particleNetMDJetTags_probQCD->at(ifj)), ifj) );
		}
		            
	  if (fatjets_bTag.size() != 0)
		{
		  if (DEBUG) {
			cout << " N selected fatjets : " << fatjets_bTag.size() << endl;
		  }
		  theSmallTree.m_isBoosted = 1;

		  // sort and take the highest (last) one
		  sort (fatjets_pT.begin(), fatjets_pT.end());
		  sort (fatjets_bTag.begin(), fatjets_bTag.end());
		  int fjIdx = fatjets_bTag.back().second;  
		  int fjIdx_pT = fatjets_pT.back().second;
		  theSmallTree.m_fatjet_highest_pT_bTag = fjIdx_pT == fjIdx;
			    
		  TLorentzVector tlv_fj(theBigTree.ak8jets_px->at(fjIdx), theBigTree.ak8jets_py->at(fjIdx),
								theBigTree.ak8jets_pz->at(fjIdx), theBigTree.ak8jets_e->at(fjIdx));

		  if (doSmearing) {
			tlv_fj *= smears_AK8[fjIdx];
		  }

		  theSmallTree.m_fatjet_pt  = tlv_fj.Pt();
		  theSmallTree.m_fatjet_eta = tlv_fj.Eta();
		  theSmallTree.m_fatjet_phi = tlv_fj.Phi();
		  theSmallTree.m_fatjet_e	= tlv_fj.E();
		  theSmallTree.m_fatjet_bID_deepFlavor = theBigTree.ak8jets_deepFlavor_probb->at(fjIdx) + theBigTree.ak8jets_deepFlavor_probbb->at(fjIdx) + theBigTree.ak8jets_deepFlavor_problepb->at(fjIdx);
		  theSmallTree.m_fatjet_filteredMass = theBigTree.ak8jets_FilteredMass->at(fjIdx);
		  theSmallTree.m_fatjet_prunedMass	 = theBigTree.ak8jets_PrunedMass  ->at(fjIdx);
		  theSmallTree.m_fatjet_trimmedMass	 = theBigTree.ak8jets_TrimmedMass ->at(fjIdx);
		  theSmallTree.m_fatjet_softdropMass = theBigTree.ak8jets_SoftDropMass->at(fjIdx);
		  theSmallTree.m_fatjet_tau1 = theBigTree.ak8jets_tau1->at(fjIdx);
		  theSmallTree.m_fatjet_tau2 = theBigTree.ak8jets_tau2->at(fjIdx);
		  theSmallTree.m_fatjet_tau3 = theBigTree.ak8jets_tau3->at(fjIdx);
		  theSmallTree.m_fatjet_nsubjets = theBigTree.ak8jets_nsubjets->at(fjIdx);
		  theSmallTree.m_fatjet_particleNetMDJetTags_probXbb = theBigTree.ak8jets_particleNetMDJetTags_probXbb->at(fjIdx);
		  theSmallTree.m_fatjet_particleNetMDJetTags_probXcc = theBigTree.ak8jets_particleNetMDJetTags_probXcc->at(fjIdx);
		  theSmallTree.m_fatjet_particleNetMDJetTags_probXqq = theBigTree.ak8jets_particleNetMDJetTags_probXqq->at(fjIdx);
		  theSmallTree.m_fatjet_particleNetMDJetTags_probQCD = theBigTree.ak8jets_particleNetMDJetTags_probQCD->at(fjIdx);
		  theSmallTree.m_fatjet_particleNetMDJetTags_score = theBigTree.ak8jets_particleNetMDJetTags_probXbb->at(fjIdx)/(theBigTree.ak8jets_particleNetMDJetTags_probXbb->at(fjIdx)+theBigTree.ak8jets_particleNetMDJetTags_probQCD->at(fjIdx));
		  theSmallTree.m_fatjet_particleNetMDJetTags_mass = theBigTree.ak8jets_particleNetMDJetTags_mass->at(fjIdx);
			    
		  // computing the vector made of the fatjet (H-bb) + svfit  H-tt
		  TLorentzVector tlv_bH_particleNet_regression;
		  tlv_bH_particleNet_regression.SetPtEtaPhiM(tlv_fj.Pt(), tlv_fj.Eta(), tlv_fj.Phi(),theBigTree.ak8jets_particleNetMDJetTags_mass->at(fjIdx));
			    
		  TLorentzVector tlv_HHbregrsvfit = tlv_bH_particleNet_regression + tlv_tauH_SVFIT;
		  theSmallTree.m_HHbregrsvfit_pt  = tlv_HHbregrsvfit.Pt();
		  theSmallTree.m_HHbregrsvfit_eta = tlv_HHbregrsvfit.Eta();
		  theSmallTree.m_HHbregrsvfit_phi = tlv_HHbregrsvfit.Phi();
		  theSmallTree.m_HHbregrsvfit_m	  = tlv_HHbregrsvfit.M();

		  // Getting ParticleNet scale factors from the pnetSF.cc
		  pnetSF pnetSF_helper;
		  std::map<std::string, std::vector<float>> pnetSF_map = pnetSF_helper.getSFmap(tlv_fj.Pt(), PERIOD);

		  if (isHHsignal){
		    // saving each working point
		    std::tuple<float, float, float> SF_HP = pnetSF_helper.getSF(pnetSF_map, "HP");
		    theSmallTree.m_fatjet_particleNetMDJetTags_HP_SF      = std::get<0>(SF_HP);
		    theSmallTree.m_fatjet_particleNetMDJetTags_HP_SF_up   = std::get<1>(SF_HP);
		    theSmallTree.m_fatjet_particleNetMDJetTags_HP_SF_down = std::get<2>(SF_HP);

		    std::tuple<float, float, float> SF_MP = pnetSF_helper.getSF(pnetSF_map, "MP");
		    theSmallTree.m_fatjet_particleNetMDJetTags_MP_SF      = std::get<0>(SF_MP);
		    theSmallTree.m_fatjet_particleNetMDJetTags_MP_SF_up   = std::get<1>(SF_MP);
		    theSmallTree.m_fatjet_particleNetMDJetTags_MP_SF_down = std::get<2>(SF_MP);

		    std::tuple<float, float, float> SF_LP = pnetSF_helper.getSF(pnetSF_map, "LP");
		    theSmallTree.m_fatjet_particleNetMDJetTags_LP_SF      = std::get<0>(SF_LP);
		    theSmallTree.m_fatjet_particleNetMDJetTags_LP_SF_up   = std::get<1>(SF_LP);
		    theSmallTree.m_fatjet_particleNetMDJetTags_LP_SF_down = std::get<2>(SF_LP);
		  }
		  //else 
		  //     --> handle bkg samples differently . For now, their values remain set to 1


		  // saving infos for subjets
		  if (theBigTree.ak8jets_nsubjets->at(fjIdx) >= 2) 
			{
			  TLorentzVector tlv_subj1, tlv_subj2;
			  vector<int> sjIdxs = findSubjetIdxs(fjIdx, theBigTree);
			            
			  int nSJ = 0;
			  for (int isj : sjIdxs)
				{
				  ++nSJ;
				  if (nSJ > 2) break;  //storing first two <--> highest pt
				  if (nSJ == 1)
					{
					  tlv_subj1.SetPxPyPzE (theBigTree.subjets_px->at(isj), theBigTree.subjets_py->at(isj), theBigTree.subjets_pz->at(isj), theBigTree.subjets_e->at(isj));
					  theSmallTree.m_subjetjet1_pt   = tlv_subj1.Pt();
					  theSmallTree.m_subjetjet1_eta  = tlv_subj1.Eta();
					  theSmallTree.m_subjetjet1_phi  = tlv_subj1.Phi();
					  theSmallTree.m_subjetjet1_e    = tlv_subj1.E();
					  theSmallTree.m_subjetjet1_bID_deepFlavor  = theBigTree.subjets_deepFlavor_probb->at(isj) + theBigTree.subjets_deepFlavor_probbb->at(isj) + theBigTree.subjets_deepFlavor_problepb->at(isj) ;
					}
				  else if (nSJ == 2)
					{
					  tlv_subj2.SetPxPyPzE (theBigTree.subjets_px->at(isj), theBigTree.subjets_py->at(isj), theBigTree.subjets_pz->at(isj), theBigTree.subjets_e->at(isj));
					  theSmallTree.m_subjetjet2_pt   = tlv_subj2.Pt();
					  theSmallTree.m_subjetjet2_eta  = tlv_subj2.Eta();
					  theSmallTree.m_subjetjet2_phi  = tlv_subj2.Phi();
					  theSmallTree.m_subjetjet2_e    = tlv_subj2.E();
					  theSmallTree.m_subjetjet2_bID_deepFlavor  = theBigTree.subjets_deepFlavor_probb->at(isj) + theBigTree.subjets_deepFlavor_probbb->at(isj) + theBigTree.subjets_deepFlavor_problepb->at(isj);
					}
				}

			  bool A1B2 = (tlv_subj1.DeltaR(tlv_firstBjet)  < 0.4) and (tlv_subj2.DeltaR(tlv_secondBjet) < 0.4);
			  bool A2B1 = (tlv_subj1.DeltaR(tlv_secondBjet) < 0.4) and (tlv_subj2.DeltaR(tlv_firstBjet)  < 0.4);
			            
			  if(DEBUG)
				{
				  cout << " fatjet: idx " << fjIdx << " nsj=" << sjIdxs.size()
					   << " sj1pt=" << tlv_subj1.Pt() << " sj1eta=" << tlv_subj1.Eta() << " sj1phi=" << tlv_subj1.Phi()
					   << " sj2pt=" << tlv_subj2.Pt() << " sj2eta=" << tlv_subj2.Eta() << " sj2phi=" << tlv_subj2.Phi()
					   << " passMatch=" << (A1B2 || A2B1) << endl;
				}
			            
			  theSmallTree.m_dR_subj1_subj2 = tlv_subj1.DeltaR(tlv_subj2);
			            
			  if (A1B2 || A2B1) theSmallTree.m_fatjet_hasMatchedSj = true;  //has two sub-jets matched to the resolved jets			            
			}

		  pair<vector<double>, vector<double>> unc_AK8_updown = JECprovider.getJECUncVectors(fjIdx, theBigTree);

		  // Total JES up/down variation for AK8 jets
		  TLorentzVector tlv_fj_jetupTot(tlv_fj);
		  TLorentzVector tlv_fj_jetdownTot(tlv_fj);
		  if (isMC)
			{
			  tlv_fj_jetupTot   = getShiftedJet(tlv_fj, +1., unc_AK8_updown.first[N_jecSources]);
			  tlv_fj_jetdownTot = getShiftedJet(tlv_fj, -1., unc_AK8_updown.second[N_jecSources]);
			}
		  theSmallTree.m_fatjet_pt_jetupTot	  = tlv_fj_jetupTot.Pt();
		  theSmallTree.m_fatjet_pt_jetdownTot = tlv_fj_jetdownTot.Pt();
		  theSmallTree.m_fatjet_e_jetupTot	  = tlv_fj_jetupTot.E();
		  theSmallTree.m_fatjet_e_jetdownTot  = tlv_fj_jetdownTot.E();

		  // reduced set of JEC uncertainties for AK8 jets
		  vector <TLorentzVector> tlv_fj_jetup  (N_jecSources, tlv_fj);
		  vector <TLorentzVector> tlv_fj_jetdown(N_jecSources, tlv_fj);
		  if (isMC) {
			for (int isource = 0; isource < N_jecSources; isource++)
			  {
				// compute all shifted AK8 jets
				tlv_fj_jetup[isource]   = getShiftedJet(tlv_fj, +1., unc_AK8_updown.first[isource]);
				tlv_fj_jetdown[isource] = getShiftedJet(tlv_fj, -1., unc_AK8_updown.second[isource]);
			  }
		  }

		  theSmallTree.m_fatjet_pt_jetup1  = tlv_fj_jetup[0].Pt();
		  theSmallTree.m_fatjet_pt_jetup2  = tlv_fj_jetup[1].Pt();
		  theSmallTree.m_fatjet_pt_jetup3  = tlv_fj_jetup[2].Pt();
		  theSmallTree.m_fatjet_pt_jetup4  = tlv_fj_jetup[3].Pt();
		  theSmallTree.m_fatjet_pt_jetup5  = tlv_fj_jetup[4].Pt();
		  theSmallTree.m_fatjet_pt_jetup6  = tlv_fj_jetup[5].Pt();
		  theSmallTree.m_fatjet_pt_jetup7  = tlv_fj_jetup[6].Pt();
		  theSmallTree.m_fatjet_pt_jetup8  = tlv_fj_jetup[7].Pt();
		  theSmallTree.m_fatjet_pt_jetup9  = tlv_fj_jetup[8].Pt();
		  theSmallTree.m_fatjet_pt_jetup10 = tlv_fj_jetup[9].Pt();
		  theSmallTree.m_fatjet_pt_jetup11 = tlv_fj_jetup[10].Pt();
		  theSmallTree.m_fatjet_e_jetup1   = tlv_fj_jetup[0].E();
		  theSmallTree.m_fatjet_e_jetup2   = tlv_fj_jetup[1].E();
		  theSmallTree.m_fatjet_e_jetup3   = tlv_fj_jetup[2].E();
		  theSmallTree.m_fatjet_e_jetup4   = tlv_fj_jetup[3].E();
		  theSmallTree.m_fatjet_e_jetup5   = tlv_fj_jetup[4].E();
		  theSmallTree.m_fatjet_e_jetup6   = tlv_fj_jetup[5].E();
		  theSmallTree.m_fatjet_e_jetup7   = tlv_fj_jetup[6].E();
		  theSmallTree.m_fatjet_e_jetup8   = tlv_fj_jetup[7].E();
		  theSmallTree.m_fatjet_e_jetup9   = tlv_fj_jetup[8].E();
		  theSmallTree.m_fatjet_e_jetup10  = tlv_fj_jetup[9].E();
		  theSmallTree.m_fatjet_e_jetup11  = tlv_fj_jetup[10].E();

		  theSmallTree.m_fatjet_pt_jetdown1  = tlv_fj_jetdown[0].Pt();
		  theSmallTree.m_fatjet_pt_jetdown2  = tlv_fj_jetdown[1].Pt();
		  theSmallTree.m_fatjet_pt_jetdown3  = tlv_fj_jetdown[2].Pt();
		  theSmallTree.m_fatjet_pt_jetdown4  = tlv_fj_jetdown[3].Pt();
		  theSmallTree.m_fatjet_pt_jetdown5  = tlv_fj_jetdown[4].Pt();
		  theSmallTree.m_fatjet_pt_jetdown6  = tlv_fj_jetdown[5].Pt();
		  theSmallTree.m_fatjet_pt_jetdown7  = tlv_fj_jetdown[6].Pt();
		  theSmallTree.m_fatjet_pt_jetdown8  = tlv_fj_jetdown[7].Pt();
		  theSmallTree.m_fatjet_pt_jetdown9  = tlv_fj_jetdown[8].Pt();
		  theSmallTree.m_fatjet_pt_jetdown10 = tlv_fj_jetdown[9].Pt();
		  theSmallTree.m_fatjet_pt_jetdown11 = tlv_fj_jetdown[10].Pt();
		  theSmallTree.m_fatjet_e_jetdown1   = tlv_fj_jetdown[0].E();
		  theSmallTree.m_fatjet_e_jetdown2   = tlv_fj_jetdown[1].E();
		  theSmallTree.m_fatjet_e_jetdown3   = tlv_fj_jetdown[2].E();
		  theSmallTree.m_fatjet_e_jetdown4   = tlv_fj_jetdown[3].E();
		  theSmallTree.m_fatjet_e_jetdown5   = tlv_fj_jetdown[4].E();
		  theSmallTree.m_fatjet_e_jetdown6   = tlv_fj_jetdown[5].E();
		  theSmallTree.m_fatjet_e_jetdown7   = tlv_fj_jetdown[6].E();
		  theSmallTree.m_fatjet_e_jetdown8   = tlv_fj_jetdown[7].E();
		  theSmallTree.m_fatjet_e_jetdown9   = tlv_fj_jetdown[8].E();
		  theSmallTree.m_fatjet_e_jetdown10  = tlv_fj_jetdown[9].E();
		  theSmallTree.m_fatjet_e_jetdown11  = tlv_fj_jetdown[10].E();

		} // end if fatjet_bTag not empty 
	  // end of the boosted section

	  // MET shifted for JES
	  auto vMET_shift_jet = getShiftedMET_jet(N_jecSources, vMET, theBigTree, JECprovider, DEBUG);

	  theSmallTree.m_METx_jetup1  = vMET_shift_jet.first.at(0).X();
	  theSmallTree.m_METx_jetup2  = vMET_shift_jet.first.at(1).X();
	  theSmallTree.m_METx_jetup3  = vMET_shift_jet.first.at(2).X();
	  theSmallTree.m_METx_jetup4  = vMET_shift_jet.first.at(3).X();
	  theSmallTree.m_METx_jetup5  = vMET_shift_jet.first.at(4).X();
	  theSmallTree.m_METx_jetup6  = vMET_shift_jet.first.at(5).X();
	  theSmallTree.m_METx_jetup7  = vMET_shift_jet.first.at(6).X();
	  theSmallTree.m_METx_jetup8  = vMET_shift_jet.first.at(7).X();
	  theSmallTree.m_METx_jetup9  = vMET_shift_jet.first.at(8).X();
	  theSmallTree.m_METx_jetup10 = vMET_shift_jet.first.at(9).X();
	  theSmallTree.m_METx_jetup11 = vMET_shift_jet.first.at(10).X();

	  theSmallTree.m_METy_jetup1  = vMET_shift_jet.first.at(0).Y();
	  theSmallTree.m_METy_jetup2  = vMET_shift_jet.first.at(1).Y();
	  theSmallTree.m_METy_jetup3  = vMET_shift_jet.first.at(2).Y();
	  theSmallTree.m_METy_jetup4  = vMET_shift_jet.first.at(3).Y();
	  theSmallTree.m_METy_jetup5  = vMET_shift_jet.first.at(4).Y();
	  theSmallTree.m_METy_jetup6  = vMET_shift_jet.first.at(5).Y();
	  theSmallTree.m_METy_jetup7  = vMET_shift_jet.first.at(6).Y();
	  theSmallTree.m_METy_jetup8  = vMET_shift_jet.first.at(7).Y();
	  theSmallTree.m_METy_jetup9  = vMET_shift_jet.first.at(8).Y();
	  theSmallTree.m_METy_jetup10 = vMET_shift_jet.first.at(9).Y();
	  theSmallTree.m_METy_jetup11 = vMET_shift_jet.first.at(10).Y();

	  theSmallTree.m_METx_jetdown1  = vMET_shift_jet.second.at(0).X();
	  theSmallTree.m_METx_jetdown2  = vMET_shift_jet.second.at(1).X();
	  theSmallTree.m_METx_jetdown3  = vMET_shift_jet.second.at(2).X();
	  theSmallTree.m_METx_jetdown4  = vMET_shift_jet.second.at(3).X();
	  theSmallTree.m_METx_jetdown5  = vMET_shift_jet.second.at(4).X();
	  theSmallTree.m_METx_jetdown6  = vMET_shift_jet.second.at(5).X();
	  theSmallTree.m_METx_jetdown7  = vMET_shift_jet.second.at(6).X();
	  theSmallTree.m_METx_jetdown8  = vMET_shift_jet.second.at(7).X();
	  theSmallTree.m_METx_jetdown9  = vMET_shift_jet.second.at(8).X();
	  theSmallTree.m_METx_jetdown10 = vMET_shift_jet.second.at(9).X();
	  theSmallTree.m_METx_jetdown11 = vMET_shift_jet.second.at(10).X();

	  theSmallTree.m_METy_jetdown1  = vMET_shift_jet.second.at(0).Y();
	  theSmallTree.m_METy_jetdown2  = vMET_shift_jet.second.at(1).Y();
	  theSmallTree.m_METy_jetdown3  = vMET_shift_jet.second.at(2).Y();
	  theSmallTree.m_METy_jetdown4  = vMET_shift_jet.second.at(3).Y();
	  theSmallTree.m_METy_jetdown5  = vMET_shift_jet.second.at(4).Y();
	  theSmallTree.m_METy_jetdown6  = vMET_shift_jet.second.at(5).Y();
	  theSmallTree.m_METy_jetdown7  = vMET_shift_jet.second.at(6).Y();
	  theSmallTree.m_METy_jetdown8  = vMET_shift_jet.second.at(7).Y();
	  theSmallTree.m_METy_jetdown9  = vMET_shift_jet.second.at(8).Y();
	  theSmallTree.m_METy_jetdown10 = vMET_shift_jet.second.at(9).Y();
	  theSmallTree.m_METy_jetdown11 = vMET_shift_jet.second.at(10).Y();

	  if (isMC)
		{
		  // Shifted MET for TES/EES (and unpacK: first is tes, second is ees)
		  auto vMET_shifts_tes_ees = getShiftedMET_tes_ees(N_tauhDM, N_tauhDM_EES, vMET, theBigTree, DEBUG);
		  auto vMET_shift_tes = vMET_shifts_tes_ees.first;
		  auto vMET_shift_ees = vMET_shifts_tes_ees.second;

		  // Shifted MET for TES
		  theSmallTree.m_METx_tauup_DM0  = vMET_shift_tes.first.at(0).X();
		  theSmallTree.m_METx_tauup_DM1  = vMET_shift_tes.first.at(1).X();
		  theSmallTree.m_METx_tauup_DM10 = vMET_shift_tes.first.at(2).X();
		  theSmallTree.m_METx_tauup_DM11 = vMET_shift_tes.first.at(3).X();

		  theSmallTree.m_METy_tauup_DM0  = vMET_shift_tes.first.at(0).Y();
		  theSmallTree.m_METy_tauup_DM1  = vMET_shift_tes.first.at(1).Y();
		  theSmallTree.m_METy_tauup_DM10 = vMET_shift_tes.first.at(2).Y();
		  theSmallTree.m_METy_tauup_DM11 = vMET_shift_tes.first.at(3).Y();

		  theSmallTree.m_METx_taudown_DM0  = vMET_shift_tes.second.at(0).X();
		  theSmallTree.m_METx_taudown_DM1  = vMET_shift_tes.second.at(1).X();
		  theSmallTree.m_METx_taudown_DM10 = vMET_shift_tes.second.at(2).X();
		  theSmallTree.m_METx_taudown_DM11 = vMET_shift_tes.second.at(3).X();

		  theSmallTree.m_METy_taudown_DM0  = vMET_shift_tes.second.at(0).Y();
		  theSmallTree.m_METy_taudown_DM1  = vMET_shift_tes.second.at(1).Y();
		  theSmallTree.m_METy_taudown_DM10 = vMET_shift_tes.second.at(2).Y();
		  theSmallTree.m_METy_taudown_DM11 = vMET_shift_tes.second.at(3).Y();

		  // Shifted MET for EES
		  theSmallTree.m_METx_eleup_DM0 = vMET_shift_ees.first.at(0).X();
		  theSmallTree.m_METx_eleup_DM1 = vMET_shift_ees.first.at(1).X();

		  theSmallTree.m_METy_eleup_DM0 = vMET_shift_ees.first.at(0).Y();
		  theSmallTree.m_METy_eleup_DM1 = vMET_shift_ees.first.at(1).Y();

		  theSmallTree.m_METx_eledown_DM0 = vMET_shift_ees.second.at(0).X();
		  theSmallTree.m_METx_eledown_DM1 = vMET_shift_ees.second.at(1).X();

		  theSmallTree.m_METy_eledown_DM0 = vMET_shift_ees.second.at(0).Y();
		  theSmallTree.m_METy_eledown_DM1 = vMET_shift_ees.second.at(1).Y();
		  
		  // Shifted MET for MES
		  auto vMET_shift_mes = getShiftedMET_mes(vMET, theBigTree, DEBUG);
		  theSmallTree.m_METx_muup   = vMET_shift_mes.first.X();
		  theSmallTree.m_METy_muup   = vMET_shift_mes.first.Y();
		  theSmallTree.m_METx_mudown = vMET_shift_mes.second.X();
		  theSmallTree.m_METy_mudown = vMET_shift_mes.second.Y();

		  // Shifted MET for JES total
		  auto vMET_shift_jetTot = getShiftedMET_jetTot(N_jecSources, vMET, theBigTree, JECprovider, DEBUG);
		  theSmallTree.m_METx_jetupTot   = vMET_shift_jetTot.first.X();
		  theSmallTree.m_METy_jetupTot   = vMET_shift_jetTot.first.Y();
		  theSmallTree.m_METx_jetdownTot = vMET_shift_jetTot.second.X();
		  theSmallTree.m_METy_jetdownTot = vMET_shift_jetTot.second.Y();
		}

	  if (isMC) selectedEvents += theBigTree.aMCatNLOweight ;  //FIXME: probably wrong, but unused up to now
	  else selectedEvents += 1 ;
	  ++selectedNoWeightsEventsNum ;

	  if (DEBUG)
		{
		  cout << "--------------"			 << endl;
		  cout << " - Debug SFs -"			 << endl;
		  cout << "	PU		   : "			 << theSmallTree.m_PUReweight		<< endl;
		  cout << "	 IDandISO deep (2D): "	 << theSmallTree.m_IdSF_deep_2d		<< endl;
		  cout << "	   w/ FakeRate: "		 << theSmallTree.m_IdFakeSF_deep_2d << endl;
		  cout << "	trig		   : "		 << theSmallTree.m_trigSF			<< endl;
		  cout << "	bTag		   : "		 << theSmallTree.m_bTagweightM		<< endl;
		  cout << "	prescale	   : "		 << theSmallTree.m_prescaleWeight	<< endl;
		  cout << "	prefiring	   : "		 << theSmallTree.m_L1pref_weight	<< endl;
		  cout << "--------------"			 << endl;
		  cout << "--- DEBUG MC weights ---" << endl;
		  cout << "aMCatNLOweight: "         << theBigTree.aMCatNLOweight << endl;
		  cout << "XS			 : "		 << XS << endl;
		  cout << "stitchWeight	 : "		 << stitchWeight << endl;
		  cout << "HHweight		 : "		 << HHweight << endl;
		  cout << "MC_weight	 : "		 << theSmallTree.m_MC_weight << endl;
		  cout << "Yield weight deep (2D): " << theSmallTree.m_MC_weight * theSmallTree.m_PUReweight * theSmallTree.m_IdFakeSF_deep_2d * theSmallTree.m_trigSF << endl;
		  cout << "------------------------" << endl;
		  cout << "--- FINAL DEBUG ---"      << endl;
		  cout << "nbjetscand: "             << theSmallTree.m_nbjetscand << endl;
		  cout << "-------------------"      << endl;
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

  vector<pair<string, double> > vAccEff = eAccEff.GetSummary();
  TH1F* h_AccEff = new TH1F("h_AccEff", "h_AccEff", vAccEff.size(), 0, vAccEff.size());
  if (isHHsignal) {
	for (uint isumm = 0; isumm < vAccEff.size(); ++isumm) {
	  h_AccEff->SetBinContent(isumm+1, vAccEff.at(isumm).second);
	  h_AccEff->GetXaxis()->SetBinLabel(isumm+1, vAccEff.at(isumm).first.c_str());
	}
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
	  for(auto const& chn : channels) {
		string hname = string("h_effSummary_") + chn.first;
		vector<pair<string, double> > vEffSummHH = ecHHsig[chn.second].GetSummary();
		hEffHHSigsSummary[chn.second] = new TH1F (hname.c_str(), hname.c_str(), vEffSummHH.size(), 0, vEffSummHH.size());
		for (uint isumm = 0; isumm < vEffSummHH.size(); ++isumm) {
		  hEffHHSigsSummary[chn.second]->SetBinContent(isumm+1, vEffSummHH.at(isumm).second);
		  hEffHHSigsSummary[chn.second]->GetXaxis()->SetBinLabel(isumm+1, vEffSummHH.at(isumm).first.c_str());
		}
	  }	  
	}

  smallFile->cd();
  h_eff.Write();
  h_effSummary->Write();
  h_AccEff->Write();
  h_syst.Write();
  if (isHHsignal) {
	for (uint ich = 0; ich < 6; ++ich)
	  hEffHHSigsSummary[ich]->Write();
  }

  smallFile->Write();
  smallFile->Close();

  // free memory used by histos for eff
  delete h_effSummary;
  delete h_AccEff;

  if (isHHsignal) {
	for (uint ich = 0; ich < 6; ++ich)
	  delete hEffHHSigsSummary[ich];
  }

  // NEW DNN
  bool computeDNN = (gConfigParser->isDefined("DNN::computeMVA") ? gConfigParser->readBoolOption("DNN::computeMVA") : false);
  if (computeDNN)
	{
	  cout << " ------------ ############### ----- NEW DNN ----- ############### ------------ " <<endl;

	  // Declare constants
	  const double E_MASS  = 0.0005109989; //GeV
	  const double MU_MASS = 0.1056583715; //GeV

	  // Reaf from configs
	  std::string model_dir = home + gConfigParser->readStringOption("DNN::weights");
	  std::cout << "DNN::weights   : " << model_dir << std::endl;

	  vector<float> DNN_kl;
	  DNN_kl = gConfigParser->readFloatListOption("DNN::kl");

	  std::string features_file = home + gConfigParser->readStringOption("DNN::features");
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
	  TFile* in_file = TFile::Open(outputFile.c_str());

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
	  Year DNN_e_year;
	  if(PERIOD=="2018") {
		DNN_e_year = Year::y18;
	  }
	  else if(PERIOD=="2017") {
		DNN_e_year = Year::y17;
	  }
	  else if(PERIOD=="2016preVFP" or PERIOD=="2016postVFP") {
		DNN_e_year = Year::y16;
	  }
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

	  TTreeReaderValue<float> rv_kinfit_mass(reader, "HHKin_mass");
	  TTreeReaderValue<float> rv_kinfit_chi2(reader, "HHKin_chi2");
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
		  pep_l_1  .SetPtEtaPhiE (*rv_l_1_pT  , *rv_l_1_eta  , *rv_l_1_phi  , *rv_l_1_e);
		  pep_l_2  .SetPtEtaPhiE (*rv_l_2_pT  , *rv_l_2_eta  , *rv_l_2_phi  , *rv_l_2_e);
		  pep_met  .SetPtEtaPhiM (*rv_met_pT  , 0            , *rv_met_phi  , 0);
		  pep_svfit.SetPtEtaPhiM (*rv_svfit_pT, *rv_svfit_eta, *rv_svfit_phi, *rv_svfit_mass);
		  pep_b_1  .SetPtEtaPhiE (*rv_b_1_pT  , *rv_b_1_eta  , *rv_b_1_phi  , *rv_b_1_e);
		  pep_b_2  .SetPtEtaPhiE (*rv_b_2_pT  , *rv_b_2_eta  , *rv_b_2_phi  , *rv_b_2_e);
		  pep_vbf_1.SetPtEtaPhiE (*rv_vbf_1_pT, *rv_vbf_1_eta, *rv_vbf_1_phi, *rv_vbf_1_e);
		  pep_vbf_2.SetPtEtaPhiE (*rv_vbf_2_pT, *rv_vbf_2_eta, *rv_vbf_2_phi, *rv_vbf_2_e);

		  // Use PEP Vectors to declare DNN vectors
		  DNN_l_1.SetCoordinates(pep_l_1.Px(), pep_l_1.Py(), pep_l_1.Pz(), pep_l_1.M());
		  if      (DNN_pType == 0)
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
		  DNN_l_2.SetCoordinates  (pep_l_2.Px()  , pep_l_2.Py()  , pep_l_2.Pz()  , pep_l_2.M());
		  DNN_met.SetCoordinates  (pep_met.Px()  , pep_met.Py()  , 0             , 0);
		  DNN_svfit.SetCoordinates(pep_svfit.Px(), pep_svfit.Py(), pep_svfit.Pz(), pep_svfit.M());
		  DNN_b_1.SetCoordinates  (pep_b_1.Px()  , pep_b_1.Py()  , pep_b_1.Pz()  , pep_b_1.M());
		  DNN_b_2.SetCoordinates  (pep_b_2.Px()  , pep_b_2.Py()  , pep_b_2.Pz()  , pep_b_2.M());
		  DNN_vbf_1.SetCoordinates(pep_vbf_1.Px(), pep_vbf_1.Py(), pep_vbf_1.Pz(), pep_vbf_1.M());
		  DNN_vbf_2.SetCoordinates(pep_vbf_2.Px(), pep_vbf_2.Py(), pep_vbf_2.Pz(), pep_vbf_2.M());

		  DNN_n_vbf = 0;
		  if (DNN_isvbf)
			{
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
	  TFile *outFile = TFile::Open(outputFile.c_str(), "UPDATE");
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
