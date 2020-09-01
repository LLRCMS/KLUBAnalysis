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
#include "../../HHKinFit2/include/HHKinFitMasterHeavyHiggs.h"
#include "SFProvider.h"
#include "BDTfunctionsUtils.h"
#include "TauIDSFTool.h"
#include "skimUtils.h"
#include "PuJetIdSF.h"

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
#include "../../cms_hh_tf_inference/inference/interface/inf_wrapper.hh"
#include "../../cms_hh_proc_interface/processing/interface/feat_comp.hh"
#include "../../cms_hh_proc_interface/processing/interface/evt_proc.hh"

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


/* NOTE ON THE COMPUTATION OF STITCH WEIGHTS:
** - to be updated at each production, using the number of processed events N_inclusive and N_njets for each sample
** - say f_i is the fraction of inclusive events in the i bin on njets (can be 2D nb-njet as well)
** = then sigma_i = f_i * sigmal_LO
** - stitchWeight (0jet) = f_0 / (f_0 * N_inclusive)
** - stitchWeight (njet) = f_n / (f_n * N_inclusive + N_njets)
*/
// computed for Legacy2016, 27Jan2020
//const float stitchWeights [][5] = {
//    {1113.1053893 , 0.0 , 0.0 , 0.0 , 0.0},
//    {1.763522979 , 1.76367228052 , 0.0 , 0.0 , 0.0},
//    {0.121313284944 , 0.121315324626 , 0.025264578338 , 0.0 , 0.0},
//    {0.121892045113 , 0.121890674183 , 0.0245692395134 , 0.0244095368559 , 0.0},
//    {0.112959002319 , 0.112961623584 , 0.0255395155779 , 0.025672220552 , 0.0241748281355}
//};

// computed for Legacy2016, 6 May 2020, including DY HT-binned samples
float stitchWeights [5][5][9] = {
 {
  {2.0214652898 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0},
  {0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0},
  {0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0},
  {0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0},
  {0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0}
 },
 {
  {0.651052057245 , 0.413673961171 , 0.367765842648 , 0.180094896131 , 0.0343370323359 , 0.00954032131527 , 0.0131222173306 , 0.0301711237283 , 0.0},
  {0.529309657185 , 0.36056089643 , 0.325974171258 , 0.171534260884 , 0.032466795382 , 0.00945749305561 , 0.0 , 0.0 , 0.0},
  {0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0},
  {0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0},
  {0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0}
 },
 {
  {0.675950344672 , 0.32568221353 , 0.367040675149 , 0.183009066172 , 0.0302107942635 , 0.00813438859745 , 0.010611777963 , 0.0121964424085 , 0.000440030333986},
  {0.543306873307 , 0.270506289373 , 0.325796314916 , 0.171988010161 , 0.0332961566334 , 0.0095407955011 , 0.0130271573311 , 0.0120330765125 , 0.0},
  {0.180013628884 , 0.137599927721 , 0.146390493132 , 0.106846640414 , 0.0294570461364 , 0.00933979747754 , 0.0126186010222 , 0.0273170985108 , 0.0},
  {0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0},
  {0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0}
 },
 {
  {0.687846534123 , 0.684357797036 , 0.368600440603 , 0.184644952054 , 0.0325122136015 , 0.00903182959861 , 0.0126509994188 , 0.0121787743827 , 0.000439986122197},
  {0.548446673425 , 0.55167674792 , 0.323846695197 , 0.17286177231 , 0.0310999917786 , 0.00954461685366 , 0.0131195412843 , 0.0122281273404 , 0.0},
  {0.178839898433 , 0.176730904692 , 0.143502984814 , 0.10389780536 , 0.0272513863363 , 0.00926873148301 , 0.0125267798314 , 0.0118359212397 , 0.000440227789824},
  {0.172146093067 , 0.158826579438 , 0.148762921015 , 0.0993078479982 , 0.0287760064141 , 0.0110900618719 , 0.0808586115919 , 0.0 , 0.0},
  {0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0 , 0.0}
 },
 {
  {0.565852537358 , 0.555308359906 , 0.332153310173 , 0.177960897092 , 0.0329882787666 , 0.00966903303917 , 0.0132228883753 , 0.0121844339873 , 0.000439991106937},
  {0.459317720411 , 0.473586356073 , 0.300262722228 , 0.16693007398 , 0.0322685431248 , 0.0101707397812 , 0.0139390193892 , 0.0120488117338 , 0.000440227789824},
  {0.166792535552 , 0.159897213879 , 0.134436541077 , 0.0974169614957 , 0.0275952936974 , 0.00917495710406 , 0.0124667137942 , 0.0115604601835 , 0.000439584558103},
  {0.120684494913 , 0.160067100927 , 0.127573323042 , 0.103097020825 , 0.0289775329435 , 0.00911453729911 , 0.0123809947417 , 0.0110458533678 , 0.0},
  {0.0 , 0.147420721388 , 0.121311850905 , 0.0852340906631 , 0.0285843898605 , 0.00897584049423 , 0.0125724757035 , 0.0 , 0.0}
 }
};


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


int main (int argc, char** argv)
{
  // read input file and cfg
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  if (argc < 22)
    { 
      cerr << "missing input parameters : argc is: " << argc << endl ;
      cerr << "usage: " << argv[0]
           << " inputFileNameList outputFileName crossSection isData configFile runHHKinFit"
           << " xsecScale(stitch) HTMax(stitch) HTMin(stitch) isTTBar DY_Nbs HHreweightFile TT_stitchType"
           << " runMT2 isHHsignal NjetRequired(stitch) kl_rew kt_rew c2_rew cg_rew c2g_rew susyModel" << endl ; 
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

  TFile* HHreweightFile = 0;
  TString doreweight = argv[12];
  cout << "** INFO: reweightin file for non-resonant hh is: " << doreweight << " [ 0 for no reweghting done ]" << endl;
  if (doreweight != TString("0"))
    HHreweightFile = new TFile (doreweight);

  int TT_stitchType = atoi(argv[13]);
  if (!isTTBar) TT_stitchType = 0; // just force if not TT...
  cout << "** INFO: TT stitch type: " << TT_stitchType << " [0: no stitch , 1: fully had, 2: semilept t, 3: semilept tbar, 4: fully lept, 5: semilept all]" << endl;

  bool runMT2 = false;
  string opt14 (argv[14]);
  if (opt14 == "1") runMT2 = true;
  cout << "** INFO: running MT2: " << runMT2 << endl;

  bool isHHsignal = false;
  string opt15 (argv[15]);
  if (opt15 == "1") isHHsignal = true;
  cout << "** INFO: is HH signal: " << isHHsignal << endl;

  int NjetRequired = atoi(argv[16]);
  cout << "** INFO: requiring exactly " << NjetRequired << " outgoing partons [<0 for no cut on this]" << endl;

  float kl_rew = atof(argv[17]);
  float kt_rew = atof(argv[18]);
  float c2_rew = atof(argv[19]);
  float cg_rew = atof(argv[20]);
  float c2g_rew = atof(argv[21]);
  cout << "** INFO: kl, kt reweight " << kl_rew << " " << kt_rew << " [kt < -990 || kl < -990 : no HH reweight]" << endl;
  cout << "**       c2, cg, c2g reweight " << c2_rew << " " << cg_rew << " " << c2g_rew << " [if any is < -990: will do only a klambda / kt reweight if requested]" << endl;

  string susyModel = argv[22];
  cout << "** INFO: requesting SUSY model to be: -" << susyModel << "- [NOTSUSY: no request on this parameter]" << endl;

  // external weight file for PUreweight - sample per sample
  TString PUreweightFile = argv[23];
  cout << "** INFO: PU reweight external file: " << PUreweightFile << endl;
  
  int DY_nJets  = atoi(argv[24]);
  int DY_nBJets = atoi(argv[25]);
  cout << "** INFO: nJets/nBjets for DY bin weights: " << DY_nJets << " / " << DY_nBJets << endl;
  int isDYI = atoi(argv[26]);
  bool isDY = (isDYI == 1) ? true : false;

  // this value is read just for "consistency", but is never actually used,
  // since in 2016 there is no ttHToTauTau sample, hence this stitching is not needed
  bool isttHToNonBB = false;
  int isttHToNonBBI = atoi(argv[27]);
  if (isttHToNonBBI == 1) isttHToNonBB = true;
  cout << "** INFO: isttHToNonBB: " << isttHToNonBB << endl;

  bool isHHNLO = false;
  int isHHNLOI = atoi(argv[28]);
  if (isHHNLOI == 1) isHHNLO = true;
  cout << "** INFO: isHHNLO: " << isHHNLO << endl;

  // ------------------  decide what to do for the reweight of HH samples
  enum HHrewTypeList {
    kNone      = 0,
    kFromHisto = 1,
    kDynamic   = 2
  };
  int HHrewType = kNone; // default is no reweight
  if (HHreweightFile && kl_rew >= -990 && kt_rew >= -990) {
    cout << "** WARNING: you required both histo based and dynamic reweight, cannot do both at the same time. Will set histo" << endl;
    HHrewType = kFromHisto;
  }
  else if (HHreweightFile)
    HHrewType = kFromHisto;
  else if (kl_rew >= -990 && kt_rew >= -990)
    HHrewType = kDynamic;
  cout << "** INFO: HH reweight type is " << HHrewType << " [ 0: no reweight, 1: from histo, 2: dynamic ]" << endl;



  // prepare variables needed throughout the code
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----    
  if (! (gConfigParser->init (config)))
    {
      cout << ">>> parseConfigFile::Could not open configuration file " << config << endl ;
      return -1 ;
    }

  bool   beInclusive         = gConfigParser->readBoolOption   ("selections::beInclusive") ;
  bool   useDeepFlavor       = gConfigParser->readBoolOption   ("selections::useDeepFlavor") ;
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
  bTagSF bTagSFHelper (bTag_SFFile, bTag_effFile, "", "80X_DeepCSV_V1");
  if(useDeepFlavor)
    bTagSFHelper.SetWPset("80X_DeepFlavor_V1");

  // ------------------------------

  std::string PUjetID_SF_directory = gConfigParser->readStringOption ("PUjetIDScaleFactors::files");
  cout << "** INFO: PU jet ID SF directory: " << PUjetID_SF_directory << std::endl;
  PuJetIdSF PUjetIDSFprovider(PUjetID_SF_directory, "2016");

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
  tau_trigger::SFProvider * tauTrgSF_ditau = new tau_trigger::SFProvider("weights/trigger_SF_Legacy/2016/2016_tauTriggerEff_DeepTau2017v2p1.root", "ditau", "Medium");
  tau_trigger::SFProvider * tauTrgSF_mutau = new tau_trigger::SFProvider("weights/trigger_SF_Legacy/2016/2016_tauTriggerEff_DeepTau2017v2p1.root", "mutau", "Medium");
  //tau_trigger::SFProvider * tauTrgSF_etau  = new tau_trigger::SFProvider("weights/trigger_SF_Legacy/2016/2016_tauTriggerEff_DeepTau2017v2p1.root", "etau" , "Medium");

  // electron/muon leg trigger SF for data and mc
  ScaleFactor * muTauTrgSF = new ScaleFactor();
  ScaleFactor * eTauTrgSF  = new ScaleFactor();
  ScaleFactor * muTrgSF    = new ScaleFactor();
  ScaleFactor * eTrgSF     = new ScaleFactor();

  muTauTrgSF->init_ScaleFactor("weights/trigger_SF_Legacy/2016/Muon_Mu19leg_2016BtoH_eff.root");
  muTrgSF   ->init_ScaleFactor("weights/trigger_SF_Legacy/2016/Muon_Mu22OR_eta2p1_eff.root");
  eTauTrgSF ->init_ScaleFactor("weights/trigger_SF_Legacy/2016/Electron_Ele24_eff.root");
  eTrgSF    ->init_ScaleFactor("weights/trigger_SF_Legacy/2016/Electron_Ele25WPTight_eff.root");

  // ------------------------------

  // electron/muon IdAndIso SF
  ScaleFactor * myIDandISOScaleFactor[2]; // [0: mu, 1: ele]
  for (int i = 0 ; i < 2; i++)
        myIDandISOScaleFactor[i] = new ScaleFactor();

  myIDandISOScaleFactor[0] -> init_ScaleFactor("weights/HTT_IdAndIso_SF_Legacy/2016/Muon_Run2016_legacy_IdIso.root");
  myIDandISOScaleFactor[1] -> init_ScaleFactor("weights/HTT_IdAndIso_SF_Legacy/2016/Electron_Run2016_legacy_IdIso.root");

  // tau IdAndIso SF
  TauIDSFTool * MVA_antiJet_medium = new TauIDSFTool("2016Legacy","MVAoldDM2017v2","Medium",1);         // for MVA2017v2 vs jets Medium
  TauIDSFTool * MVA_antiEle_vloose = new TauIDSFTool("2016Legacy","antiEleMVA6"   ,"VLoose");           // for MVA2017v2 vs ele VLoose
  TauIDSFTool * MVA_antiEle_tight  = new TauIDSFTool("2016Legacy","antiEleMVA6"   ,"Tight");            // for MVA2017v2 vs ele Tight
  TauIDSFTool * MVA_antiMu_loose   = new TauIDSFTool("2016Legacy","antiMu3"       ,"Loose");            // for MVA2017v2 vs mu Loose
  TauIDSFTool * MVA_antiMu_tight   = new TauIDSFTool("2016Legacy","antiMu3"       ,"Tight");            // for MVA2017v2 vs mu Tight

  TauIDSFTool * Deep_antiJet_medium     = new TauIDSFTool("2016Legacy","DeepTau2017v2p1VSjet","Medium",1); // for DeepTauv2p1 vs jets Medium
  TauIDSFTool * Deep_antiJet_medium_pt  = new TauIDSFTool("2016Legacy","DeepTau2017v2p1VSjet","Medium");   // for DeepTauv2p1 vs jets Medium
  TauIDSFTool * Deep_antiEle_vvloose    = new TauIDSFTool("2016Legacy","DeepTau2017v2p1VSe"  ,"VVLoose");  // for DeepTauv2p1 vs ele VVLoose
  TauIDSFTool * Deep_antiEle_vloose     = new TauIDSFTool("2016Legacy","DeepTau2017v2p1VSe"  ,"VLoose");   // for DeepTauv2p1 vs ele VLoose
  TauIDSFTool * Deep_antiEle_tight      = new TauIDSFTool("2016Legacy","DeepTau2017v2p1VSe"  ,"Tight");    // for DeepTauv2p1 vs ele Tight
  TauIDSFTool * Deep_antiMu_vloose      = new TauIDSFTool("2016Legacy","DeepTau2017v2p1VSmu" ,"VLoose");   // for DeepTauv2p1 vs mu VLoose
  TauIDSFTool * Deep_antiMu_tight       = new TauIDSFTool("2016Legacy","DeepTau2017v2p1VSmu" ,"Tight");    // for DeepTauv2p1 vs mu Tight

  // ------------------------------

  // reweighting file for HH non resonant
  TH1F* hreweightHH   = 0;
  TH2F* hreweightHH2D = 0;
  // if (HHreweightFile)
  if (HHrewType == kFromHisto)
    {
      cout << "** INFO: doing reweight for HH samples" << endl;
      if (HHreweightFile->GetListOfKeys()->Contains("hratio") )
	{  
	  hreweightHH = (TH1F*) HHreweightFile->Get("hratio");
	  cout << "** INFO: 1D reweight using hratio" << endl;
	}
      else if (HHreweightFile->GetListOfKeys()->Contains("hratio2D") )
	{
	  hreweightHH2D = (TH2F*) HHreweightFile->Get("hratio2D");            
	  cout << "** INFO: 2D reweight using hratio2D" << endl;
	}
      else
	{
	  cout << "** ERROR: reweight histo not found in file provided, stopping execuction" << endl;
	  return 1;
	}
    }

  // ------------------------------
  // reweight file in case of "dynamic" reweight
  // there is a unique input map, read it from the cfg file
  // HHReweight* hhreweighter = nullptr;
  HHReweight5D* hhreweighter = nullptr;
  TH2* hhreweighterInputMap = nullptr;
  if (HHrewType == kDynamic)
    {
      string inMapFile   = gConfigParser->readStringOption("HHReweight::inputFile");
      string inHistoName = gConfigParser->readStringOption("HHReweight::histoName");
      string coeffFile    = gConfigParser->readStringOption("HHReweight::coeffFile");
      cout << "** INFO: reading histo named: " << inHistoName << " from file: " << inMapFile << endl;
      cout << "** INFO: HH reweight coefficient file is: " << coeffFile << endl;
      TFile* fHHDynamicRew = new TFile(inMapFile.c_str());
      hhreweighterInputMap = (TH2*) fHHDynamicRew->Get(inHistoName.c_str());
      // hhreweighter = new HHReweight(coeffFile, hhreweighterInputMap);
      hhreweighter = new HHReweight5D(coeffFile, hhreweighterInputMap);
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

  // MVA tau ID
  vector<int> tauMVAIDIdx;
  //tauMVAIDIdx.push_back(getTauIDIdx(hTauIDS, "byVVLooseIsolationMVArun2v1DBoldDMwLT"));
  tauMVAIDIdx.push_back(getTauIDIdx(hTauIDS, "byVLooseIsolationMVArun2v1DBoldDMwLT"));
  tauMVAIDIdx.push_back(getTauIDIdx(hTauIDS, "byLooseIsolationMVArun2v1DBoldDMwLT"));
  tauMVAIDIdx.push_back(getTauIDIdx(hTauIDS, "byMediumIsolationMVArun2v1DBoldDMwLT"));
  tauMVAIDIdx.push_back(getTauIDIdx(hTauIDS, "byTightIsolationMVArun2v1DBoldDMwLT"));
  tauMVAIDIdx.push_back(getTauIDIdx(hTauIDS, "byVTightIsolationMVArun2v1DBoldDMwLT"));
  if (find(tauMVAIDIdx.begin(), tauMVAIDIdx.end(), -1) != tauMVAIDIdx.end())
    {
      cout << "** WARNING!! did not found some MVA tau IDs" << endl;
      for (unsigned int i = 0; i < tauMVAIDIdx.size(); ++i)
	cout << tauMVAIDIdx.at(i) << " " ;
      cout << endl;
    }
    
  // new MVA tau ID // FRA syncFeb2018
  vector<int> tauMVAIDIdxNew;
  //tauMVAIDIdxNew.push_back(getTauIDIdx(hTauIDS, "byVVLooseIsolationMVArun2017v2DBoldDMwLT2017")); //SYNCH HTT
  tauMVAIDIdxNew.push_back(getTauIDIdx(hTauIDS, "byVLooseIsolationMVArun2017v2DBoldDMwLT2017"));
  tauMVAIDIdxNew.push_back(getTauIDIdx(hTauIDS, "byLooseIsolationMVArun2017v2DBoldDMwLT2017"));
  tauMVAIDIdxNew.push_back(getTauIDIdx(hTauIDS, "byMediumIsolationMVArun2017v2DBoldDMwLT2017"));
  tauMVAIDIdxNew.push_back(getTauIDIdx(hTauIDS, "byTightIsolationMVArun2017v2DBoldDMwLT2017"));
  tauMVAIDIdxNew.push_back(getTauIDIdx(hTauIDS, "byVTightIsolationMVArun2017v2DBoldDMwLT2017"));
  if (find(tauMVAIDIdxNew.begin(), tauMVAIDIdxNew.end(), -1) != tauMVAIDIdxNew.end())
    {
      cout << "** WARNING!! did not found some MVA tau IDs New" << endl;
      for (unsigned int i = 0; i < tauMVAIDIdxNew.size(); ++i)
	cout << tauMVAIDIdxNew.at(i) << " " ;
      cout << endl;
    }

  // new MVA tau ID // FRA syncFeb2018
  vector<int> tauMVAIDIdxNewdR0p3;
  tauMVAIDIdxNewdR0p3.push_back(getTauIDIdx(hTauIDS, "byVLooseIsolationMVArun2017v2DBoldDMdR0p3wLT2017"));
  tauMVAIDIdxNewdR0p3.push_back(getTauIDIdx(hTauIDS, "byLooseIsolationMVArun2017v2DBoldDMdR0p3wLT2017"));
  tauMVAIDIdxNewdR0p3.push_back(getTauIDIdx(hTauIDS, "byMediumIsolationMVArun2017v2DBoldDMdR0p3wLT2017"));
  tauMVAIDIdxNewdR0p3.push_back(getTauIDIdx(hTauIDS, "byTightIsolationMVArun2017v2DBoldDMdR0p3wLT2017"));
  tauMVAIDIdxNewdR0p3.push_back(getTauIDIdx(hTauIDS, "byVTightIsolationMVArun2017v2DBoldDMdR0p3wLT2017"));
  if (find(tauMVAIDIdxNewdR0p3.begin(), tauMVAIDIdxNewdR0p3.end(), -1) != tauMVAIDIdxNewdR0p3.end())
    {
      cout << "** WARNING!! did not found some MVA tau IDs New" << endl;
      for (unsigned int i = 0; i < tauMVAIDIdxNewdR0p3.size(); ++i)
	cout << tauMVAIDIdxNewdR0p3.at(i) << " " ;
      cout << endl;
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


  // anti-ele discr
  vector<int> tauAntiEleIdx;
  tauAntiEleIdx.push_back(getTauIDIdx(hTauIDS, "againstElectronVLooseMVA6"));
  tauAntiEleIdx.push_back(getTauIDIdx(hTauIDS, "againstElectronLooseMVA6"));
  tauAntiEleIdx.push_back(getTauIDIdx(hTauIDS, "againstElectronMediumMVA6"));
  tauAntiEleIdx.push_back(getTauIDIdx(hTauIDS, "againstElectronTightMVA6"));
  tauAntiEleIdx.push_back(getTauIDIdx(hTauIDS, "againstElectronVTightMVA6"));
  if (find(tauAntiEleIdx.begin(), tauAntiEleIdx.end(), -1) != tauAntiEleIdx.end())
    {
      cout << "** WARNING!! did not found some cut-based tau IDs" << endl;
      for (unsigned int i = 0; i < tauAntiEleIdx.size(); ++i)
	cout << tauAntiEleIdx.at(i) << " " ;
      cout << endl;
    }

  // anti-mu discr
  vector<int> tauAntiMuIdx;
  tauAntiMuIdx.push_back(getTauIDIdx(hTauIDS, "againstMuonLoose3"));
  tauAntiMuIdx.push_back(getTauIDIdx(hTauIDS, "againstMuonTight3"));
  if (find(tauAntiMuIdx.begin(), tauAntiMuIdx.end(), -1) != tauAntiMuIdx.end())
    {
      cout << "** WARNING!! did not found some cut-based tau IDs" << endl;
      for (unsigned int i = 0; i < tauAntiMuIdx.size(); ++i)
	cout << tauAntiMuIdx.at(i) << " " ;
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

      float stitchWeight = 1.0;
      if (DY_tostitch)
	{
	  int njets = theBigTree.lheNOutPartons;
	  int nb    = theBigTree.lheNOutB;
	  // these protections should be useless
	  if (njets < 0) njets = 0;
	  if (njets > 4) njets = 4;
	  if (nb < 0)    nb = 0;
	  if (nb > 4)    nb = 4;

	  float ht = theBigTree.lheHt;
	  int nht = 0;
	  if      (ht  < 0                ) nht = 0;
	  else if (ht >= 0    && ht < 70  ) nht = 0;
	  else if (ht >= 70   && ht < 100 ) nht = 1;
	  else if (ht >= 100  && ht < 200 ) nht = 2;
	  else if (ht >= 200  && ht < 400 ) nht = 3;
	  else if (ht >= 400  && ht < 600 ) nht = 4;
	  else if (ht >= 600  && ht < 800 ) nht = 5;
	  else if (ht >= 800  && ht < 1200) nht = 6;
	  else if (ht >= 1200 && ht < 2500) nht = 7;
	  else  /* ht >= 2500 */            nht = 8;

	  stitchWeight = stitchWeights[njets][nb][nht];
	}

    // Should never enter here (DY_tostitch should be always true)
    if (!DY_tostitch && DY_nJets >= 0)
    {
        int njets = theBigTree.lheNOutPartons;
        int nb    = theBigTree.lheNOutB;
        //cout << "- njets: " << njets << " - nb: " << nb << endl; //FRA debug
        if (njets != DY_nJets || nb != DY_nBJets) continue;
    }

      if (DEBUG && isMC)
	{
	  cout << "** DEBUG : gen particle list" << endl;
	  for (unsigned int igen = 0; igen < theBigTree.genpart_pdg->size(); igen++)
	    {
	      int pdg = theBigTree.genpart_pdg->at(igen);
	      if (abs(pdg) == 66615 || abs(pdg) == 11 || abs(pdg) == 13)
		{
		  TLorentzVector vGPDebug;
		  vGPDebug.SetPxPyPzE (theBigTree.genpart_px->at(igen), theBigTree.genpart_py->at(igen), theBigTree.genpart_pz->at(igen), theBigTree.genpart_e->at(igen) ) ;
		  cout << igen << " pdg=" << pdg << " pt=" << vGPDebug.Pt() << " eta=" << vGPDebug.Eta() << " phi=" << vGPDebug.Phi() << endl;          
		}
	      if (abs(pdg) == 25)
		{
		  cout << igen << " pdg=" << pdg << " decay=" << theBigTree.genpart_HZDecayMode->at(igen) << endl; 
		}
	    }
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
	      bool isFirst = CheckBit (genflags, 12);  //if (fl.isFirstCopy()) flags |= (1 << 12);
	      //int topDM = theBigTree.genpart_TopDecayMode->at(igen);

	      if (abs(pdg) == 6 && isFirst) // top -- pt reweight wants to have ME tops
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

          // Save DY LO weights according to nbs and pT(Z)
          if (genZ_pt <= 10.)
          {
            theSmallTree.m_DYscale_MH  = DYscale_MH_vLowPt [n_bJets];
            theSmallTree.m_DYscale_MTT = DYscale_MTT_vLowPt[n_bJets];
          }
          else if (genZ_pt > 10. && genZ_pt <= 50.)
          {
            theSmallTree.m_DYscale_MH  = DYscale_MH_LowPt [n_bJets];
            theSmallTree.m_DYscale_MTT = DYscale_MTT_LowPt[n_bJets];
          }
          else if (genZ_pt > 50. && genZ_pt <= 80.)
          {
            theSmallTree.m_DYscale_MH  = DYscale_MH_Med1Pt [n_bJets];
            theSmallTree.m_DYscale_MTT = DYscale_MTT_Med1Pt[n_bJets];
          }
          else if (genZ_pt > 80. && genZ_pt <= 110.)
          {
            theSmallTree.m_DYscale_MH  = DYscale_MH_Med2Pt [n_bJets];
            theSmallTree.m_DYscale_MTT = DYscale_MTT_Med2Pt[n_bJets];
          }
          else if (genZ_pt > 110. && genZ_pt <= 190.)
          {
            theSmallTree.m_DYscale_MH  = DYscale_MH_HighPt [n_bJets];
            theSmallTree.m_DYscale_MTT = DYscale_MTT_HighPt[n_bJets];
          }
          else /* pT(Z)>=190. */
          {
            theSmallTree.m_DYscale_MH  = DYscale_MH_vHighPt [n_bJets];
            theSmallTree.m_DYscale_MTT = DYscale_MTT_vHighPt[n_bJets];
          }
	    }

        else // genZ not found: use the lowest pT bin
        {
            theSmallTree.m_DYscale_MH  = DYscale_MH_vLowPt [n_bJets];
            theSmallTree.m_DYscale_MTT = DYscale_MTT_vLowPt[n_bJets];
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

      // if (hreweightHH || hreweightHH2D || isHHsignal) // isHHsignal: only to do loop on genparts, but no rew
      if (isHHsignal || HHrewType == kFromHisto || HHrewType == kDynamic) // isHHsignal: only to do loop on genparts, but no rew
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


	  // assign a weight depending on the reweight type 

	  if (hreweightHH && HHrewType == kFromHisto) // 1D
	    {
	      int ibin = hreweightHH->FindBin(mHH);
	      HHweight = hreweightHH->GetBinContent(ibin);
	    }
	  else if (hreweightHH2D && HHrewType == kFromHisto) // 2D
	    {
	      int ibin = hreweightHH2D->FindBin(mHH, ct1);
	      HHweight = hreweightHH2D->GetBinContent(ibin);        
	    }
	  else if (HHrewType == kDynamic)
	    {
	      // HHweight = hhreweighter->getWeight(kl_rew, kt_rew, mHH, ct1);
	      if (c2_rew < -990 || cg_rew < -990 || c2g_rew < -990) // no valid BSM coefficients -- just kl/kt reweight (for backwards compatibility)
		HHweight = hhreweighter->getWeight(kl_rew, kt_rew, mHH, ct1);
	      else // full 5D reweight
		HHweight = hhreweighter->getWeight(kl_rew, kt_rew, c2_rew, cg_rew, c2g_rew, mHH, ct1);
	    }

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
      if(!isMC) metpass += (metbit & (1 << 7)) ? 1 : 0; // "Flag_eeBadScFilter" not suggested on twiki; EDIT: now suggested for data (Moriond2018)

      if(DEBUG)
      {
        cout << "metpass: " << metpass << endl;
        cout << "metfilterbit (int): " << metbit << endl;
        cout << "metfilterbit (binary):         " << std::bitset<10>(theBigTree.metfilterbit) << endl;
        cout << "metfilterbit (metbit binary) : " << std::bitset<10>(metbit) << endl;
        cout << "Flag_goodVertices      : " << (metbit & (1 << 0)) << endl;
        cout << "Flag_HBHENoiseIsoFilter: " << (metbit & (1 << 2)) << endl;
        cout << "Flag_BadPFMuonFilter   : " << (metbit & (1 << 5)) << endl;
        cout << "Flag_HBHENoiseFilter   : " << (metbit & (1 << 1)) << endl;
        cout << "Flag_EcalDeadCellTriggerPrimitiveFilter: " << (metbit & (1 << 3)) << endl;
        cout << "Flag_globalSuperTightHalo2016Filter    : " << (metbit & (1 << 4)) << endl;
        if(!isMC) cout << "Flag_eeBadScFilter: " << (metbit & (1 << 7)) << endl;
      }

      if(isMC && metpass < 6) continue ;
      if(!isMC && metpass < 7) continue ;
     
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
          bool passMu   = oph.muBaseline (&theBigTree, idau, 19., 2.1, 0.15, OfflineProducerHelper::MuTight, string("All") , (DEBUG ? true : false));
          bool passMu10 = oph.muBaseline (&theBigTree, idau, 10., 2.4, 0.30, OfflineProducerHelper::MuTight, string("All") , (DEBUG ? true : false));

          if (passMu) ++nmu;
          else if (passMu10) ++nmu10;
        }
        else if (oph.isElectron(dauType))
        {
          bool passEle   = oph.eleBaseline (&theBigTree, idau, 25., 2.1, 0.1, OfflineProducerHelper::EMVATight, string("Vertex-LepID-pTMin-etaMax") , (DEBUG ? true : false));
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
          << " mubase="    << setw(3)  << left << oph.muBaseline (&theBigTree, idau, 10., 2.1, 0.15, OfflineProducerHelper::MuTight, string("All"))
          << " ebase="     << setw(3)  << left << oph.eleBaseline(&theBigTree, idau, 10., 2.1, 0.1, OfflineProducerHelper::EMVATight, string("Vertex-LepID-pTMin-etaMax"))
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
      if (applyTriggers)
      {
        Long64_t triggerbit = theBigTree.triggerbit;

        Long64_t matchFlag1 = (Long64_t) theBigTree.daughters_trgMatched->at(firstDaughterIndex);
        Long64_t matchFlag2 = (Long64_t) theBigTree.daughters_trgMatched->at(secondDaughterIndex);
        Long64_t goodTriggerType1 = (Long64_t) theBigTree.daughters_isGoodTriggerType->at(firstDaughterIndex);
        Long64_t goodTriggerType2 = (Long64_t) theBigTree.daughters_isGoodTriggerType->at(secondDaughterIndex);

        Long64_t trgNotOverlapFlag = (Long64_t) theBigTree.mothers_trgSeparateMatch->at(chosenTauPair);
        bool passTrg = trigReader.checkOR (pairType,triggerbit, &pass_triggerbit, matchFlag1, matchFlag2, trgNotOverlapFlag, goodTriggerType1, goodTriggerType2, tlv_firstLepton.Pt(), tlv_firstLepton.Eta(), tlv_secondLepton.Pt(), tlv_secondLepton.Eta()) ;

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
        if (pairType == 0 && isMC)
        {
          if ( CheckBit(pass_triggerbit,7) || CheckBit(pass_triggerbit,8) )
          {
            theSmallTree.m_prescaleWeight = 1.; // no need for prescale for cross triggers
          }
          else if ( CheckBit(pass_triggerbit,2) || CheckBit(pass_triggerbit,3) )
          {
            theSmallTree.m_prescaleWeight = 0.923782353; // passes (HLT_IsoMu22_eta2p1_v OR HLT_IsoTkMu22_eta2p1_v)
          }
          else if ( CheckBit(pass_triggerbit,0) || CheckBit(pass_triggerbit,1) )
          {
            theSmallTree.m_prescaleWeight = 0.795221971; // passes only (HLT_IsoMu22_v OR HLT_IsoTkMu22_v)
          }
        }

        // MuMu
        if (pairType == 3 && isMC)
        {
          if ( CheckBit(pass_triggerbit,2) || CheckBit(pass_triggerbit,3) )
          {
            theSmallTree.m_prescaleWeight = 0.923782353; // passes (HLT_IsoMu22_eta2p1_v OR HLT_IsoTkMu22_eta2p1_v)
          }
          else if ( CheckBit(pass_triggerbit,0) || CheckBit(pass_triggerbit,1) )
          {
            theSmallTree.m_prescaleWeight = 0.795221971; // passes only (HLT_IsoMu22_v OR HLT_IsoTkMu22_v)
          }
        }

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
        triggerAccept = passTrg;

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
      }


      // ----------------------------------------------------------
      // pair selection is now complete, compute oher quantitites
    
      TLorentzVector tlv_tauH = tlv_firstLepton + tlv_secondLepton ;
      TLorentzVector tlv_tauH_SVFIT ;

      theSmallTree.m_tauH_SVFIT_mass = theBigTree.SVfitMass->at (chosenTauPair) ;
      //theSmallTree.m_tauH_SVFIT_mass_up   = (isMC ? theBigTree.SVfitMassTauUp->at (chosenTauPair) : theSmallTree.m_tauH_SVFIT_mass);
      //theSmallTree.m_tauH_SVFIT_mass_down = (isMC ? theBigTree.SVfitMassTauDown->at (chosenTauPair) : theSmallTree.m_tauH_SVFIT_mass);
      //theSmallTree.m_tauH_SVFIT_mass_METup   = (isMC ? theBigTree.SVfitMassMETUp->at (chosenTauPair) : theSmallTree.m_tauH_SVFIT_mass);
      //theSmallTree.m_tauH_SVFIT_mass_METdown = (isMC ? theBigTree.SVfitMassMETDown->at (chosenTauPair) : theSmallTree.m_tauH_SVFIT_mass);
      // in case the SVFIT mass is calculated
      if (theBigTree.SVfitMass->at (chosenTauPair) > -900.)
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

	  theSmallTree.m_tauHsvfitMet_deltaPhi = deltaPhi (theBigTree.metphi, tlv_tauH_SVFIT.Phi ()) ;
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
      // theSmallTree.m_met_phi = theBigTree.metphi ;
      // theSmallTree.m_met_et = theBigTree.met ;
      TVector2 vMET (theBigTree.METx->at(chosenTauPair) , theBigTree.METy->at(chosenTauPair));
      theSmallTree.m_met_phi   = vMET.Phi();
      theSmallTree.m_met_et    = vMET.Mod();
      theSmallTree.m_METx    = vMET.X();
      theSmallTree.m_METy    = vMET.Y();
      theSmallTree.m_met_cov00 = theBigTree.MET_cov00->at (chosenTauPair);
      theSmallTree.m_met_cov01 = theBigTree.MET_cov01->at (chosenTauPair);
      theSmallTree.m_met_cov10 = theBigTree.MET_cov10->at (chosenTauPair);
      theSmallTree.m_met_cov11 = theBigTree.MET_cov11->at (chosenTauPair);

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

      theSmallTree.m_tauHMet_deltaPhi = deltaPhi (theBigTree.metphi, tlv_tauH.Phi ()) ;
      theSmallTree.m_ditau_deltaPhi = deltaPhi (tlv_firstLepton.Phi (), tlv_secondLepton.Phi ()) ;
      theSmallTree.m_ditau_deltaEta = fabs(tlv_firstLepton.Eta ()- tlv_secondLepton.Eta ()) ;
      theSmallTree.m_ditau_deltaR = tlv_firstLepton.DeltaR(tlv_secondLepton) ;
      theSmallTree.m_dau1MET_deltaphi = deltaPhi (theBigTree.metphi , tlv_firstLepton.Phi ()) ;
      theSmallTree.m_dau2MET_deltaphi = deltaPhi (theBigTree.metphi , tlv_secondLepton.Phi ()) ;

      // Create the MET TLorentzVector for BDT variables, since it's MET --> (px,py,0,0)
      TLorentzVector tlv_MET;
      tlv_MET.SetPxPyPzE( theBigTree.METx->at(chosenTauPair), theBigTree.METy->at(chosenTauPair), 0, std::hypot(theBigTree.METx->at(chosenTauPair), theBigTree.METy->at(chosenTauPair)) );

      theSmallTree.m_tauH_MET_pt                  = (tlv_tauH + tlv_MET).Pt();
      theSmallTree.m_dau2_MET_deltaEta            = fabs(tlv_secondLepton.Eta()); // since MET.Eta()==0 by definition, dEta(tau2,MET)=|tau2.Eta()|
      theSmallTree.m_ditau_deltaR_per_tauH_MET_pt = theSmallTree.m_ditau_deltaR * theSmallTree.m_tauH_MET_pt;

      theSmallTree.m_p_zeta         = Calculate_Pzeta(tlv_firstLepton, tlv_secondLepton, tlv_MET);
      theSmallTree.m_p_zeta_visible = Calculate_visiblePzeta(tlv_firstLepton, tlv_secondLepton);

      theSmallTree.m_mT_tauH_MET       = Calculate_MT(tlv_tauH + tlv_MET, tlv_MET);
      theSmallTree.m_mT_total          = Calculate_TotalMT(tlv_firstLepton, tlv_secondLepton, tlv_MET);
      if (theBigTree.SVfitMass->at (chosenTauPair) > -900.) // in case SVfit is calculated
      {
        theSmallTree.m_mT_tauH_SVFIT_MET             = Calculate_MT(tlv_tauH_SVFIT, tlv_MET);
        theSmallTree.m_BDT_tauHsvfitMet_abs_deltaPhi = fabs(ROOT::Math::VectorUtil::DeltaPhi(tlv_tauH_SVFIT, tlv_MET));
        theSmallTree.m_BDT_tauHsvfitMet_deltaPhi     = ROOT::Math::VectorUtil::DeltaPhi(tlv_tauH_SVFIT, tlv_MET);
      }

      theSmallTree.m_BDT_ditau_deltaPhi = ROOT::Math::VectorUtil::DeltaPhi(tlv_firstLepton, tlv_secondLepton);
      theSmallTree.m_BDT_dau1MET_deltaPhi = ROOT::Math::VectorUtil::DeltaPhi(tlv_firstLepton, tlv_MET);

      theSmallTree.m_dau1_iso = getIso (firstDaughterIndex, tlv_firstLepton.Pt (), theBigTree) ;
      theSmallTree.m_dau1_eleMVAiso = theBigTree.daughters_iseleWP80->at(firstDaughterIndex) ? 1 : 0;
      theSmallTree.m_dau1_MVAiso = makeIsoDiscr (firstDaughterIndex, tauMVAIDIdx, theBigTree) ;
      theSmallTree.m_dau1_MVAisoNew = makeIsoDiscr (firstDaughterIndex, tauMVAIDIdxNew, theBigTree) ; //FRA syncFeb2018
      theSmallTree.m_dau1_MVAisoNewdR0p3 = makeIsoDiscr (firstDaughterIndex, tauMVAIDIdxNewdR0p3, theBigTree) ; // FRA syncApr2018
      theSmallTree.m_dau1_CUTiso = makeIsoDiscr (firstDaughterIndex, tauCUTIDIdx, theBigTree) ;
      theSmallTree.m_dau1_antiele = makeIsoDiscr (firstDaughterIndex, tauAntiEleIdx, theBigTree) ;
      theSmallTree.m_dau1_antimu  = makeIsoDiscr (firstDaughterIndex, tauAntiMuIdx, theBigTree) ;
      theSmallTree.m_dau1_deepTauVsJet = makeIsoDiscr (firstDaughterIndex, deepTauVsJetIdx , theBigTree) ;
      theSmallTree.m_dau1_deepTauVsEle = makeIsoDiscr (firstDaughterIndex, deepTauVsEleIdx , theBigTree) ;
      theSmallTree.m_dau1_deepTauVsMu = makeIsoDiscr (firstDaughterIndex, deepTauVsMuIdx , theBigTree) ;

      theSmallTree.m_dau1_photonPtSumOutsideSignalCone = theBigTree.photonPtSumOutsideSignalCone->at (firstDaughterIndex) ;

      int ibit = tauIDsMap["byLooseCombinedIsolationDeltaBetaCorr3Hits"] ;
      theSmallTree.m_dau1_byLooseCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (firstDaughterIndex)  & (1 << ibit) ) ? true : false ;
      theSmallTree.m_dau2_byLooseCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (secondDaughterIndex) & (1 << ibit) ) ? true : false ;

      ibit = tauIDsMap["byMediumCombinedIsolationDeltaBetaCorr3Hits"] ;
      theSmallTree.m_dau1_byMediumCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (firstDaughterIndex)  & (1 << ibit) ) ? true : false ;
      theSmallTree.m_dau2_byMediumCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (secondDaughterIndex) & (1 << ibit) ) ? true : false ;

      ibit = tauIDsMap["byTightCombinedIsolationDeltaBetaCorr3Hits"] ;
      theSmallTree.m_dau1_byTightCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (firstDaughterIndex)  & (1 << ibit) ) ? true : false ;
      theSmallTree.m_dau2_byTightCombinedIsolationDeltaBetaCorr3Hits = ( theBigTree.tauID->at (secondDaughterIndex) & (1 << ibit) ) ? true : false ;

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
      theSmallTree.m_dau2_MVAiso = makeIsoDiscr (secondDaughterIndex, tauMVAIDIdx, theBigTree) ;
      theSmallTree.m_dau2_MVAisoNew = makeIsoDiscr (secondDaughterIndex, tauMVAIDIdxNew, theBigTree) ; //FRA syncFeb2018
      theSmallTree.m_dau2_MVAisoNewdR0p3 = makeIsoDiscr (secondDaughterIndex, tauMVAIDIdxNewdR0p3, theBigTree) ; // FRA syncApr2018
      theSmallTree.m_dau2_CUTiso = makeIsoDiscr (secondDaughterIndex, tauCUTIDIdx, theBigTree) ;
      theSmallTree.m_dau2_antiele = makeIsoDiscr (secondDaughterIndex, tauAntiEleIdx, theBigTree) ;
      theSmallTree.m_dau2_antimu  = makeIsoDiscr (secondDaughterIndex, tauAntiMuIdx, theBigTree) ;
      theSmallTree.m_dau2_deepTauVsJet = makeIsoDiscr (secondDaughterIndex, deepTauVsJetIdx , theBigTree) ;
      theSmallTree.m_dau2_deepTauVsEle = makeIsoDiscr (secondDaughterIndex, deepTauVsEleIdx , theBigTree) ;
      theSmallTree.m_dau2_deepTauVsMu = makeIsoDiscr (secondDaughterIndex, deepTauVsMuIdx , theBigTree) ;
      theSmallTree.m_dau2_photonPtSumOutsideSignalCone = theBigTree.photonPtSumOutsideSignalCone->at (secondDaughterIndex) ;      
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
      bool isFakeJet1 = true;
      bool isFakeJet2 = true;

      float idAndIsoSF_MVA  = 1.0;           // use this for MVA2017v2
      float idAndIsoSF_deep = 1.0;           // use this for DeepTauV2p1 DM dependent
      float idAndIsoSF_deep_pt = 1.0;        // use this for DeepTauV2p1 pt dependent
      float idAndIsoAndFakeSF_MVA  = 1.0;    // use this for MVA2017v2 + e/mu->tauh fake SF
      float idAndIsoAndFakeSF_deep = 1.0;    // use this for DeepTauV2p1 DM dependent + e/mu->tauh fake SF
      float idAndIsoAndFakeSF_deep_pt = 1.0; // use this for DeepTauV2p1 pt dependent + e/mu->tauh fake SF
      float fakeRateSF_MVA  = 1.0;           // use this for e/mu->tauh fake SF MVA
      float fakeRateSF_deep = 1.0;           // use this for e/mu->tauh fake SF DeepTau

      float idAndIsoAndFakeSF_tauid_pt20to25_up    = 1.0;
      float idAndIsoAndFakeSF_tauid_pt25to30_up    = 1.0;
      float idAndIsoAndFakeSF_tauid_pt30to35_up    = 1.0;
      float idAndIsoAndFakeSF_tauid_pt35to40_up    = 1.0;
      float idAndIsoAndFakeSF_tauid_pt40toInf_up   = 1.0;
      float idAndIsoAndFakeSF_tauid_pt20to25_down  = 1.0;
      float idAndIsoAndFakeSF_tauid_pt25to30_down  = 1.0;
      float idAndIsoAndFakeSF_tauid_pt30to35_down  = 1.0;
      float idAndIsoAndFakeSF_tauid_pt35to40_down  = 1.0;
      float idAndIsoAndFakeSF_tauid_pt40toInf_down = 1.0;

      float idAndIsoAndFakeSF_mutauFR_etaLt0p4_up      = 1.0;
      float idAndIsoAndFakeSF_mutauFR_eta0p4to0p8_up   = 1.0;
      float idAndIsoAndFakeSF_mutauFR_eta0p8to1p2_up   = 1.0;
      float idAndIsoAndFakeSF_mutauFR_eta1p2to1p7_up   = 1.0;
      float idAndIsoAndFakeSF_mutauFR_etaGt1p7_up      = 1.0;
      float idAndIsoAndFakeSF_mutauFR_etaLt0p4_down    = 1.0;
      float idAndIsoAndFakeSF_mutauFR_eta0p4to0p8_down = 1.0;
      float idAndIsoAndFakeSF_mutauFR_eta0p8to1p2_down = 1.0;
      float idAndIsoAndFakeSF_mutauFR_eta1p2to1p7_down = 1.0;
      float idAndIsoAndFakeSF_mutauFR_etaGt1p7_down    = 1.0;

      float idAndIsoAndFakeSF_etauFR_barrel_up   = 1.0;
      float idAndIsoAndFakeSF_etauFR_endcap_up   = 1.0;
      float idAndIsoAndFakeSF_etauFR_barrel_down = 1.0;
      float idAndIsoAndFakeSF_etauFR_endcap_down = 1.0;

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

      // MuTau Channel // anti-ele VLoose / anti-mu Tight / anti-jet Medium
      if (pType == 0 && isMC)
      {
        float mu1pt  = tlv_firstLepton.Pt();
        float mu1eta = TMath::Abs(tlv_firstLepton.Eta());

        float tau2pt  = tlv_secondLepton.Pt();
        float tau2eta = TMath::Abs(tlv_secondLepton.Eta());
        float tau2DM  = theSmallTree.m_dau2_decayMode;
        float tau2Genmatch = theBigTree.genmatch->at(secondDaughterIndex);

        float idAndIsoSF_leg1               = 1.;
        float idAndIsoSF_leg2_MVA_vsJet     = 1.;
        float idAndIsoSF_leg2_MVA_vsEle     = 1.;
        float idAndIsoSF_leg2_MVA_vsMu      = 1.;
        float idAndIsoSF_leg2_deep_vsJet    = 1.;
        float idAndIsoSF_leg2_deep_vsJet_pt = 1.;
        float idAndIsoSF_leg2_deep_vsEle    = 1.;
        float idAndIsoSF_leg2_deep_vsMu     = 1.;

        if (mu1eta < 2.4)
        {
          idAndIsoSF_leg1 = myIDandISOScaleFactor[0]->get_ScaleFactor(mu1pt, mu1eta);
        }

        idAndIsoSF_leg2_MVA_vsJet = MVA_antiJet_medium ->getSFvsDM (tau2pt , tau2DM, tau2Genmatch);
        idAndIsoSF_leg2_MVA_vsEle = MVA_antiEle_vloose ->getSFvsEta(tau2eta, tau2Genmatch);
        idAndIsoSF_leg2_MVA_vsMu  = MVA_antiMu_tight   ->getSFvsEta(tau2eta, tau2Genmatch);

        idAndIsoSF_leg2_deep_vsJet    = Deep_antiJet_medium   ->getSFvsDM (tau2pt , tau2DM, tau2Genmatch);
        idAndIsoSF_leg2_deep_vsJet_pt = Deep_antiJet_medium_pt->getSFvsPT (tau2pt , tau2Genmatch);
        idAndIsoSF_leg2_deep_vsEle    = Deep_antiEle_vloose   ->getSFvsEta(tau2eta, tau2Genmatch);
        idAndIsoSF_leg2_deep_vsMu     = Deep_antiMu_tight     ->getSFvsEta(tau2eta, tau2Genmatch);

        vector<float> idAndIsoSF_leg2_deep_vsJet_pt_up (5, idAndIsoSF_leg2_deep_vsJet_pt); // in bins of pt: 20, 25, 30, 35, 40, infty
        vector<float> idAndIsoSF_leg2_deep_vsEle_up    (2, idAndIsoSF_leg2_deep_vsEle);    // in bins of eta: barrel, endcap
        vector<float> idAndIsoSF_leg2_deep_vsMu_up     (5, idAndIsoSF_leg2_deep_vsMu);     // in bins of eta, edges at 0, 0.4, 0.8, 1.2, 1.7, infty

        vector<float> idAndIsoSF_leg2_deep_vsJet_pt_down (5, idAndIsoSF_leg2_deep_vsJet_pt); // in bins of pt: 20, 25, 30, 35, 40, infty
        vector<float> idAndIsoSF_leg2_deep_vsEle_down    (2, idAndIsoSF_leg2_deep_vsEle);    // in bins of eta: barrel, endcap
        vector<float> idAndIsoSF_leg2_deep_vsMu_down     (5, idAndIsoSF_leg2_deep_vsMu);     // in bins of eta, edges at 0, 0.4, 0.8, 1.2, 1.7, infty

        for (int bin = 0; bin < (int) isthisPt_IDbin_second.size(); bin++)
        {
          if (isthisPt_IDbin_second[bin])
          {
            idAndIsoSF_leg2_deep_vsJet_pt_up[bin]   = Deep_antiJet_medium_pt ->getSFvsPT(tau2pt, tau2Genmatch,   "Up");
            idAndIsoSF_leg2_deep_vsJet_pt_down[bin] = Deep_antiJet_medium_pt ->getSFvsPT(tau2pt, tau2Genmatch, "Down");
          }
        }

        for (int bin = 0; bin < (int) isthisEta_IDbin_second.size(); bin++)
        {
          if (isthisEta_IDbin_second[bin])
          {
            idAndIsoSF_leg2_deep_vsMu_up[bin]   = Deep_antiMu_tight ->getSFvsEta(tau2eta, tau2Genmatch,   "Up");
            idAndIsoSF_leg2_deep_vsMu_down[bin] = Deep_antiMu_tight ->getSFvsEta(tau2eta, tau2Genmatch, "Down");
          }
        }

        for (int bin = 0; bin < (int) isthisSDet_IDbin_second.size(); bin++)
        {
          if (isthisSDet_IDbin_second[bin])
          {
            idAndIsoSF_leg2_deep_vsEle_up[bin]   = Deep_antiEle_vloose ->getSFvsEta(tau2eta, tau2Genmatch,   "Up");
            idAndIsoSF_leg2_deep_vsEle_down[bin] = Deep_antiEle_vloose ->getSFvsEta(tau2eta, tau2Genmatch, "Down");
          }
        }

        if (tau2Genmatch==1 || tau2Genmatch==2 || tau2Genmatch==3 || tau2Genmatch==4 || tau2Genmatch==5)
        {
          isFakeJet2 = false;
        }

        idAndIsoSF_MVA  = idAndIsoSF_leg1 * idAndIsoSF_leg2_MVA_vsJet;
        idAndIsoSF_deep    = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet;
        idAndIsoSF_deep_pt = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt;
        idAndIsoAndFakeSF_MVA  = idAndIsoSF_leg1 * idAndIsoSF_leg2_MVA_vsJet  * idAndIsoSF_leg2_MVA_vsEle  * idAndIsoSF_leg2_MVA_vsMu;
        idAndIsoAndFakeSF_deep = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_deep_pt = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        fakeRateSF_MVA  = idAndIsoSF_leg2_MVA_vsEle  * idAndIsoSF_leg2_MVA_vsMu;
        fakeRateSF_deep = idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;

        idAndIsoAndFakeSF_tauid_pt20to25_up    = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_up[0]   * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt25to30_up    = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_up[1]   * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt30to35_up    = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_up[2]   * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt35to40_up    = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_up[3]   * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt40toInf_up   = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_up[4]   * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt20to25_down  = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_down[0] * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt25to30_down  = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_down[1] * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt30to35_down  = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_down[2] * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt35to40_down  = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_down[3] * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt40toInf_down = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_down[4] * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;

        idAndIsoAndFakeSF_mutauFR_etaLt0p4_up      = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_up[0];
        idAndIsoAndFakeSF_mutauFR_eta0p4to0p8_up   = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_up[1];
        idAndIsoAndFakeSF_mutauFR_eta0p8to1p2_up   = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_up[2];
        idAndIsoAndFakeSF_mutauFR_eta1p2to1p7_up   = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_up[3];
        idAndIsoAndFakeSF_mutauFR_etaGt1p7_up      = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_up[4];
        idAndIsoAndFakeSF_mutauFR_etaLt0p4_down    = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_down[0];
        idAndIsoAndFakeSF_mutauFR_eta0p4to0p8_down = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_down[1];
        idAndIsoAndFakeSF_mutauFR_eta0p8to1p2_down = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_down[2];
        idAndIsoAndFakeSF_mutauFR_eta1p2to1p7_down = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_down[3];
        idAndIsoAndFakeSF_mutauFR_etaGt1p7_down    = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_down[4];

        idAndIsoAndFakeSF_etauFR_barrel_up   = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle_up[0]   * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_etauFR_endcap_up   = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle_up[1]   * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_etauFR_barrel_down = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle_down[0] * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_etauFR_endcap_down = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle_down[1] * idAndIsoSF_leg2_deep_vsMu;

        if (DEBUG)
        {
          cout << "--- DEBUG idAndIsoSF ---" << endl;
          cout << "pairType  : "                    << pType                         << endl;
          cout << "totSF deep: "                    << idAndIsoAndFakeSF_deep        << endl;
          cout << "totSF deep_pt: "                 << idAndIsoAndFakeSF_deep_pt     << endl;
          cout << "idAndIsoSF_leg1: "               << idAndIsoSF_leg1               << endl;
          cout << "idAndIsoSF_leg2_deep_vsJet: "    << idAndIsoSF_leg2_deep_vsJet    << endl;
          cout << "idAndIsoSF_leg2_deep_vsJet_pt: " << idAndIsoSF_leg2_deep_vsJet_pt << endl;
          cout << "idAndIsoSF_leg2_deep_vsEle: "    << idAndIsoSF_leg2_deep_vsEle    << endl;
          cout << "idAndIsoSF_leg2_deep_vsMu: "     << idAndIsoSF_leg2_deep_vsMu     << endl;
        }
      }

      // EleTau Channel // anti-ele Tight / anti-mu Tight / anti-jet Medium
      else if (pType == 1 && isMC)
      {
        float ele1pt  = tlv_firstLepton.Pt();
        float ele1eta = TMath::Abs(tlv_firstLepton.Eta());

        float tau2pt  = tlv_secondLepton.Pt();
        float tau2eta = TMath::Abs(tlv_secondLepton.Eta());
        float tau2DM  = theSmallTree.m_dau2_decayMode;
        float tau2Genmatch = theBigTree.genmatch->at(secondDaughterIndex);

        float idAndIsoSF_leg1               = 1.;
        float idAndIsoSF_leg2_MVA_vsJet     = 1.;
        float idAndIsoSF_leg2_MVA_vsEle     = 1.;
        float idAndIsoSF_leg2_MVA_vsMu      = 1.;
        float idAndIsoSF_leg2_deep_vsJet    = 1.;
        float idAndIsoSF_leg2_deep_vsJet_pt = 1.;
        float idAndIsoSF_leg2_deep_vsEle    = 1.;
        float idAndIsoSF_leg2_deep_vsMu     = 1.;

        if (ele1eta < 2.4)
        {
          idAndIsoSF_leg1 = myIDandISOScaleFactor[1]->get_ScaleFactor(ele1pt, ele1eta);
        }

        idAndIsoSF_leg2_MVA_vsJet = MVA_antiJet_medium ->getSFvsDM (tau2pt , tau2DM, tau2Genmatch);
        idAndIsoSF_leg2_MVA_vsEle = MVA_antiEle_tight  ->getSFvsEta(tau2eta, tau2Genmatch);
        idAndIsoSF_leg2_MVA_vsMu  = MVA_antiMu_loose   ->getSFvsEta(tau2eta, tau2Genmatch);

        idAndIsoSF_leg2_deep_vsJet    = Deep_antiJet_medium    ->getSFvsDM (tau2pt,  tau2DM, tau2Genmatch);
        idAndIsoSF_leg2_deep_vsJet_pt = Deep_antiJet_medium_pt ->getSFvsPT (tau2pt,  tau2Genmatch);
        idAndIsoSF_leg2_deep_vsEle    = Deep_antiEle_tight     ->getSFvsEta(tau2eta, tau2Genmatch);
        idAndIsoSF_leg2_deep_vsMu     = Deep_antiMu_tight      ->getSFvsEta(tau2eta, tau2Genmatch);

        vector<float> idAndIsoSF_leg2_deep_vsJet_pt_up (5, idAndIsoSF_leg2_deep_vsJet_pt); // in bins of pt: 20, 25, 30, 35, 40, infty
        vector<float> idAndIsoSF_leg2_deep_vsEle_up    (2, idAndIsoSF_leg2_deep_vsEle);    // in bins of eta: barrel, endcap
        vector<float> idAndIsoSF_leg2_deep_vsMu_up     (5, idAndIsoSF_leg2_deep_vsMu);     // in bins of eta, edges at 0, 0.4, 0.8, 1.2, 1.7, infty

        vector<float> idAndIsoSF_leg2_deep_vsJet_pt_down (5, idAndIsoSF_leg2_deep_vsJet_pt); // in bins of pt: 20, 25, 30, 35, 40, infty
        vector<float> idAndIsoSF_leg2_deep_vsEle_down    (2, idAndIsoSF_leg2_deep_vsEle);    // in bins of eta: barrel, endcap
        vector<float> idAndIsoSF_leg2_deep_vsMu_down     (5, idAndIsoSF_leg2_deep_vsMu);     // in bins of eta, edges at 0, 0.4, 0.8, 1.2, 1.7, infty

        for (int bin = 0; bin < (int) isthisPt_IDbin_second.size(); bin++)
        {
          if (isthisPt_IDbin_second[bin])
          {
            idAndIsoSF_leg2_deep_vsJet_pt_up[bin]   = Deep_antiJet_medium_pt ->getSFvsPT(tau2pt, tau2Genmatch,   "Up");
            idAndIsoSF_leg2_deep_vsJet_pt_down[bin] = Deep_antiJet_medium_pt ->getSFvsPT(tau2pt, tau2Genmatch, "Down");
          }
        }

        for (int bin = 0; bin < (int) isthisEta_IDbin_second.size(); bin++)
        {
          if (isthisEta_IDbin_second[bin])
          {
            idAndIsoSF_leg2_deep_vsMu_up[bin]   = Deep_antiMu_tight ->getSFvsEta(tau2eta, tau2Genmatch,   "Up");
            idAndIsoSF_leg2_deep_vsMu_down[bin] = Deep_antiMu_tight ->getSFvsEta(tau2eta, tau2Genmatch, "Down");
          }
        }

        for (int bin = 0; bin < (int) isthisSDet_IDbin_second.size(); bin++)
        {
          if (isthisSDet_IDbin_second[bin])
          {
            idAndIsoSF_leg2_deep_vsEle_up[bin]   = Deep_antiEle_tight ->getSFvsEta(tau2eta, tau2Genmatch,   "Up");
            idAndIsoSF_leg2_deep_vsEle_down[bin] = Deep_antiEle_tight ->getSFvsEta(tau2eta, tau2Genmatch, "Down");
          }
        }

        if (tau2Genmatch==1 || tau2Genmatch==2 || tau2Genmatch==3 || tau2Genmatch==4 || tau2Genmatch==5)
        {
          isFakeJet2 = false;
        }

        idAndIsoSF_MVA  = idAndIsoSF_leg1 * idAndIsoSF_leg2_MVA_vsJet;
        idAndIsoSF_deep = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet;
        idAndIsoSF_deep_pt = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt;
        idAndIsoAndFakeSF_MVA  = idAndIsoSF_leg1 * idAndIsoSF_leg2_MVA_vsJet  * idAndIsoSF_leg2_MVA_vsEle  * idAndIsoSF_leg2_MVA_vsMu;
        idAndIsoAndFakeSF_deep = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_deep_pt = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        fakeRateSF_MVA  = idAndIsoSF_leg2_MVA_vsEle  * idAndIsoSF_leg2_MVA_vsMu;
        fakeRateSF_deep = idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;

        idAndIsoAndFakeSF_tauid_pt20to25_up    = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_up[0]   * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt25to30_up    = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_up[1]   * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt30to35_up    = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_up[2]   * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt35to40_up    = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_up[3]   * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt40toInf_up   = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_up[4]   * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt20to25_down  = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_down[0] * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt25to30_down  = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_down[1] * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt30to35_down  = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_down[2] * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt35to40_down  = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_down[3] * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt40toInf_down = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt_down[4] * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;

        idAndIsoAndFakeSF_mutauFR_etaLt0p4_up      = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_up[0];
        idAndIsoAndFakeSF_mutauFR_eta0p4to0p8_up   = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_up[1];
        idAndIsoAndFakeSF_mutauFR_eta0p8to1p2_up   = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_up[2];
        idAndIsoAndFakeSF_mutauFR_eta1p2to1p7_up   = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_up[3];
        idAndIsoAndFakeSF_mutauFR_etaGt1p7_up      = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_up[4];
        idAndIsoAndFakeSF_mutauFR_etaLt0p4_down    = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_down[0];
        idAndIsoAndFakeSF_mutauFR_eta0p4to0p8_down = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_down[1];
        idAndIsoAndFakeSF_mutauFR_eta0p8to1p2_down = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_down[2];
        idAndIsoAndFakeSF_mutauFR_eta1p2to1p7_down = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_down[3];
        idAndIsoAndFakeSF_mutauFR_etaGt1p7_down    = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_down[4];

        idAndIsoAndFakeSF_etauFR_barrel_up   = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle_up[0]   * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_etauFR_endcap_up   = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle_up[1]   * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_etauFR_barrel_down = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle_down[0] * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_etauFR_endcap_down = idAndIsoSF_leg1 * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle_down[1] * idAndIsoSF_leg2_deep_vsMu;

        if (DEBUG)
        {
          cout << "--- DEBUG idAndIsoSF ---" << endl;
          cout << "pairType  : "                    << pType                         << endl;
          cout << "totSF deep: "                    << idAndIsoAndFakeSF_deep        << endl;
          cout << "totSF deep_pt: "                 << idAndIsoAndFakeSF_deep_pt     << endl;
          cout << "idAndIsoSF_leg1: "               << idAndIsoSF_leg1               << endl;
          cout << "idAndIsoSF_leg2_deep_vsJet: "    << idAndIsoSF_leg2_deep_vsJet    << endl;
          cout << "idAndIsoSF_leg2_deep_vsJet_pt: " << idAndIsoSF_leg2_deep_vsJet_pt << endl;
          cout << "idAndIsoSF_leg2_deep_vsEle: "    << idAndIsoSF_leg2_deep_vsEle    << endl;
          cout << "idAndIsoSF_leg2_deep_vsMu: "     << idAndIsoSF_leg2_deep_vsMu     << endl;
        }
      }

      // TauTau Channel // anti-ele VVLoose / anti-mu VLoose / anti-jet Medium
      else if (pType == 2 && isMC)
      {

        float tau1pt  = tlv_firstLepton.Pt();
        float tau1eta = TMath::Abs(tlv_firstLepton.Eta());
        float tau1DM  = theSmallTree.m_dau1_decayMode;
        float tau1Genmatch = theBigTree.genmatch->at(firstDaughterIndex);

        float tau2pt  = tlv_secondLepton.Pt();
        float tau2eta = TMath::Abs(tlv_secondLepton.Eta());
        float tau2DM  = theSmallTree.m_dau2_decayMode;
        float tau2Genmatch = theBigTree.genmatch->at(secondDaughterIndex);

        float idAndIsoSF_leg1_MVA_vsJet     = 1.;
        float idAndIsoSF_leg1_MVA_vsEle     = 1.;
        float idAndIsoSF_leg1_MVA_vsMu      = 1.;
        float idAndIsoSF_leg1_deep_vsJet    = 1.;
        float idAndIsoSF_leg1_deep_vsJet_pt = 1.;
        float idAndIsoSF_leg1_deep_vsEle    = 1.;
        float idAndIsoSF_leg1_deep_vsMu     = 1.;

        float idAndIsoSF_leg2_MVA_vsJet     = 1.;
        float idAndIsoSF_leg2_MVA_vsEle     = 1.;
        float idAndIsoSF_leg2_MVA_vsMu      = 1.;
        float idAndIsoSF_leg2_deep_vsJet    = 1.;
        float idAndIsoSF_leg2_deep_vsJet_pt = 1.;
        float idAndIsoSF_leg2_deep_vsEle    = 1.;
        float idAndIsoSF_leg2_deep_vsMu     = 1.;

        // Leg 1
        idAndIsoSF_leg1_MVA_vsJet     = MVA_antiJet_medium    ->getSFvsDM (tau1pt , tau1DM, tau1Genmatch);
        idAndIsoSF_leg1_MVA_vsEle     = MVA_antiEle_vloose    ->getSFvsEta(tau1eta, tau1Genmatch);
        idAndIsoSF_leg1_MVA_vsMu      = MVA_antiMu_loose      ->getSFvsEta(tau1eta, tau1Genmatch);

        idAndIsoSF_leg1_deep_vsJet    = Deep_antiJet_medium   ->getSFvsDM(tau1pt, tau1DM, tau1Genmatch);
        idAndIsoSF_leg1_deep_vsJet_pt = Deep_antiJet_medium_pt->getSFvsPT(tau1pt, tau1Genmatch);
        idAndIsoSF_leg1_deep_vsEle    = Deep_antiEle_vvloose  ->getSFvsEta(tau1eta, tau1Genmatch);
        idAndIsoSF_leg1_deep_vsMu     = Deep_antiMu_vloose    ->getSFvsEta(tau1eta, tau1Genmatch);

        // Leg 2
        idAndIsoSF_leg2_MVA_vsJet     = MVA_antiJet_medium    ->getSFvsDM (tau2pt , tau2DM, tau2Genmatch);
        idAndIsoSF_leg2_MVA_vsEle     = MVA_antiEle_vloose    ->getSFvsEta(tau2eta, tau2Genmatch);
        idAndIsoSF_leg2_MVA_vsMu      = MVA_antiMu_loose      ->getSFvsEta(tau2eta, tau2Genmatch);

        idAndIsoSF_leg2_deep_vsJet    = Deep_antiJet_medium   ->getSFvsDM(tau2pt, tau2DM, tau2Genmatch);
        idAndIsoSF_leg2_deep_vsJet_pt = Deep_antiJet_medium_pt->getSFvsPT(tau2pt, tau2Genmatch);
        idAndIsoSF_leg2_deep_vsEle    = Deep_antiEle_vvloose  ->getSFvsEta(tau2eta, tau2Genmatch);
        idAndIsoSF_leg2_deep_vsMu     = Deep_antiMu_vloose    ->getSFvsEta(tau2eta, tau2Genmatch);

        // Up/down variations Leg 1
        vector<float> idAndIsoSF_leg1_deep_vsJet_pt_up (5, idAndIsoSF_leg1_deep_vsJet_pt); // in bins of pt: 20, 25, 30, 35, 40, infty
        vector<float> idAndIsoSF_leg1_deep_vsEle_up    (2, idAndIsoSF_leg1_deep_vsEle);    // in bins of eta: barrel, endcap
        vector<float> idAndIsoSF_leg1_deep_vsMu_up     (5, idAndIsoSF_leg1_deep_vsMu);     // in bins of eta, edges at 0, 0.4, 0.8, 1.2, 1.7, infty

        vector<float> idAndIsoSF_leg1_deep_vsJet_pt_down (5, idAndIsoSF_leg1_deep_vsJet_pt); // in bins of pt: 20, 25, 30, 35, 40, infty
        vector<float> idAndIsoSF_leg1_deep_vsEle_down    (2, idAndIsoSF_leg1_deep_vsEle);    // in bins of eta: barrel, endcap
        vector<float> idAndIsoSF_leg1_deep_vsMu_down     (5, idAndIsoSF_leg1_deep_vsMu);     // in bins of eta, edges at 0, 0.4, 0.8, 1.2, 1.7, infty

        for (int bin = 0; bin < (int) isthisPt_IDbin_first.size(); bin++)
        {
          if (isthisPt_IDbin_first[bin])
          {
            idAndIsoSF_leg1_deep_vsJet_pt_up[bin]   = Deep_antiJet_medium_pt ->getSFvsPT(tau1pt, tau1Genmatch,   "Up");
            idAndIsoSF_leg1_deep_vsJet_pt_down[bin] = Deep_antiJet_medium_pt ->getSFvsPT(tau1pt, tau1Genmatch, "Down");
          }
        }

        for (int bin = 0; bin < (int) isthisEta_IDbin_first.size(); bin++)
        {
          if (isthisEta_IDbin_first[bin])
          {
            idAndIsoSF_leg1_deep_vsMu_up[bin]   = Deep_antiMu_vloose ->getSFvsEta(tau1eta, tau1Genmatch,   "Up");
            idAndIsoSF_leg1_deep_vsMu_down[bin] = Deep_antiMu_vloose ->getSFvsEta(tau1eta, tau1Genmatch, "Down");
          }
        }

        for (int bin = 0; bin < (int) isthisSDet_IDbin_first.size(); bin++)
        {
          if (isthisSDet_IDbin_first[bin])
          {
            idAndIsoSF_leg1_deep_vsEle_up[bin]   = Deep_antiEle_vvloose ->getSFvsEta(tau1eta, tau1Genmatch,   "Up");
            idAndIsoSF_leg1_deep_vsEle_down[bin] = Deep_antiEle_vvloose ->getSFvsEta(tau1eta, tau1Genmatch, "Down");
          }
        }

        // Up/down variations Leg 2
        vector<float> idAndIsoSF_leg2_deep_vsJet_pt_up (5, idAndIsoSF_leg2_deep_vsJet_pt); // in bins of pt: 20, 25, 30, 35, 40, infty
        vector<float> idAndIsoSF_leg2_deep_vsEle_up    (2, idAndIsoSF_leg2_deep_vsEle);    // in bins of eta: barrel, endcap
        vector<float> idAndIsoSF_leg2_deep_vsMu_up     (5, idAndIsoSF_leg2_deep_vsMu);     // in bins of eta, edges at 0, 0.4, 0.8, 1.2, 1.7, infty

        vector<float> idAndIsoSF_leg2_deep_vsJet_pt_down (5, idAndIsoSF_leg2_deep_vsJet_pt); // in bins of pt: 20, 25, 30, 35, 40, infty
        vector<float> idAndIsoSF_leg2_deep_vsEle_down    (2, idAndIsoSF_leg2_deep_vsEle);    // in bins of eta: barrel, endcap
        vector<float> idAndIsoSF_leg2_deep_vsMu_down     (5, idAndIsoSF_leg2_deep_vsMu);     // in bins of eta, edges at 0, 0.4, 0.8, 1.2, 1.7, infty

        for (int bin = 0; bin < (int) isthisPt_IDbin_second.size(); bin++)
        {
          if (isthisPt_IDbin_second[bin])
          {
            idAndIsoSF_leg2_deep_vsJet_pt_up[bin]   = Deep_antiJet_medium_pt ->getSFvsPT(tau2pt, tau2Genmatch,   "Up");
            idAndIsoSF_leg2_deep_vsJet_pt_down[bin] = Deep_antiJet_medium_pt ->getSFvsPT(tau2pt, tau2Genmatch, "Down");
          }
        }

        for (int bin = 0; bin < (int) isthisEta_IDbin_second.size(); bin++)
        {
          if (isthisEta_IDbin_second[bin])
          {
            idAndIsoSF_leg2_deep_vsMu_up[bin]   = Deep_antiMu_vloose ->getSFvsEta(tau2eta, tau2Genmatch,   "Up");
            idAndIsoSF_leg2_deep_vsMu_down[bin] = Deep_antiMu_vloose ->getSFvsEta(tau2eta, tau2Genmatch, "Down");
          }
        }

        for (int bin = 0; bin < (int) isthisSDet_IDbin_second.size(); bin++)
        {
          if (isthisSDet_IDbin_second[bin])
          {
            idAndIsoSF_leg2_deep_vsEle_up[bin]   = Deep_antiEle_vvloose ->getSFvsEta(tau2eta, tau2Genmatch,   "Up");
            idAndIsoSF_leg2_deep_vsEle_down[bin] = Deep_antiEle_vvloose ->getSFvsEta(tau2eta, tau2Genmatch, "Down");
          }
        }

        if (tau1Genmatch==1 || tau1Genmatch==2 || tau1Genmatch==3 || tau1Genmatch==4 || tau1Genmatch==5)
        {
          isFakeJet1 = false;
        }

        if (tau2Genmatch==1 || tau2Genmatch==2 || tau2Genmatch==3 || tau2Genmatch==4 || tau2Genmatch==5)
        {
          isFakeJet2 = false;
        }

        idAndIsoSF_MVA  = idAndIsoSF_leg1_MVA_vsJet * idAndIsoSF_leg2_MVA_vsJet;
        idAndIsoSF_deep = idAndIsoSF_leg1_deep_vsJet * idAndIsoSF_leg2_deep_vsJet;
        idAndIsoSF_deep_pt = idAndIsoSF_leg1_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsJet_pt;
        idAndIsoAndFakeSF_MVA  = idAndIsoSF_leg1_MVA_vsJet * idAndIsoSF_leg1_MVA_vsEle * idAndIsoSF_leg1_MVA_vsMu * idAndIsoSF_leg2_MVA_vsJet  * idAndIsoSF_leg2_MVA_vsEle  * idAndIsoSF_leg2_MVA_vsMu;
        idAndIsoAndFakeSF_deep = idAndIsoSF_leg1_deep_vsJet * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu * idAndIsoSF_leg2_deep_vsJet * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_deep_pt = idAndIsoSF_leg1_deep_vsJet_pt * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        fakeRateSF_MVA  = idAndIsoSF_leg1_MVA_vsEle * idAndIsoSF_leg1_MVA_vsMu * idAndIsoSF_leg2_MVA_vsEle  * idAndIsoSF_leg2_MVA_vsMu;
        fakeRateSF_deep = idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;

        idAndIsoAndFakeSF_tauid_pt20to25_up    = idAndIsoSF_leg1_deep_vsJet_pt_up[0]   * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu * idAndIsoSF_leg2_deep_vsJet_pt_up[0]   * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt25to30_up    = idAndIsoSF_leg1_deep_vsJet_pt_up[1]   * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu * idAndIsoSF_leg2_deep_vsJet_pt_up[1]   * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt30to35_up    = idAndIsoSF_leg1_deep_vsJet_pt_up[2]   * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu * idAndIsoSF_leg2_deep_vsJet_pt_up[2]   * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt35to40_up    = idAndIsoSF_leg1_deep_vsJet_pt_up[3]   * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu * idAndIsoSF_leg2_deep_vsJet_pt_up[3]   * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt40toInf_up   = idAndIsoSF_leg1_deep_vsJet_pt_up[4]   * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu * idAndIsoSF_leg2_deep_vsJet_pt_up[4]   * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt20to25_down  = idAndIsoSF_leg1_deep_vsJet_pt_down[0] * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu * idAndIsoSF_leg2_deep_vsJet_pt_down[0] * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt25to30_down  = idAndIsoSF_leg1_deep_vsJet_pt_down[1] * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu * idAndIsoSF_leg2_deep_vsJet_pt_down[1] * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt30to35_down  = idAndIsoSF_leg1_deep_vsJet_pt_down[2] * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu * idAndIsoSF_leg2_deep_vsJet_pt_down[2] * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt35to40_down  = idAndIsoSF_leg1_deep_vsJet_pt_down[3] * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu * idAndIsoSF_leg2_deep_vsJet_pt_down[3] * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_tauid_pt40toInf_down = idAndIsoSF_leg1_deep_vsJet_pt_down[4] * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu * idAndIsoSF_leg2_deep_vsJet_pt_down[4] * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu;

        idAndIsoAndFakeSF_mutauFR_etaLt0p4_up      = idAndIsoSF_leg1_deep_vsJet_pt * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu_up[0]   * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_up[0];
        idAndIsoAndFakeSF_mutauFR_eta0p4to0p8_up   = idAndIsoSF_leg1_deep_vsJet_pt * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu_up[1]   * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_up[1];
        idAndIsoAndFakeSF_mutauFR_eta0p8to1p2_up   = idAndIsoSF_leg1_deep_vsJet_pt * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu_up[2]   * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_up[2];
        idAndIsoAndFakeSF_mutauFR_eta1p2to1p7_up   = idAndIsoSF_leg1_deep_vsJet_pt * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu_up[3]   * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_up[3];
        idAndIsoAndFakeSF_mutauFR_etaGt1p7_up      = idAndIsoSF_leg1_deep_vsJet_pt * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu_up[4]   * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_up[4];
        idAndIsoAndFakeSF_mutauFR_etaLt0p4_down    = idAndIsoSF_leg1_deep_vsJet_pt * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu_down[0] * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_down[0];
        idAndIsoAndFakeSF_mutauFR_eta0p4to0p8_down = idAndIsoSF_leg1_deep_vsJet_pt * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu_down[1] * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_down[1];
        idAndIsoAndFakeSF_mutauFR_eta0p8to1p2_down = idAndIsoSF_leg1_deep_vsJet_pt * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu_down[2] * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_down[2];
        idAndIsoAndFakeSF_mutauFR_eta1p2to1p7_down = idAndIsoSF_leg1_deep_vsJet_pt * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu_down[3] * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_down[3];
        idAndIsoAndFakeSF_mutauFR_etaGt1p7_down    = idAndIsoSF_leg1_deep_vsJet_pt * idAndIsoSF_leg1_deep_vsEle * idAndIsoSF_leg1_deep_vsMu_down[4] * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle * idAndIsoSF_leg2_deep_vsMu_down[4];

        idAndIsoAndFakeSF_etauFR_barrel_up   = idAndIsoSF_leg1_deep_vsJet_pt * idAndIsoSF_leg1_deep_vsEle_up[0]   * idAndIsoSF_leg1_deep_vsMu * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle_up[0]   * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_etauFR_endcap_up   = idAndIsoSF_leg1_deep_vsJet_pt * idAndIsoSF_leg1_deep_vsEle_up[1]   * idAndIsoSF_leg1_deep_vsMu * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle_up[1]   * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_etauFR_barrel_down = idAndIsoSF_leg1_deep_vsJet_pt * idAndIsoSF_leg1_deep_vsEle_down[0] * idAndIsoSF_leg1_deep_vsMu * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle_down[0] * idAndIsoSF_leg2_deep_vsMu;
        idAndIsoAndFakeSF_etauFR_endcap_down = idAndIsoSF_leg1_deep_vsJet_pt * idAndIsoSF_leg1_deep_vsEle_down[1] * idAndIsoSF_leg1_deep_vsMu * idAndIsoSF_leg2_deep_vsJet_pt * idAndIsoSF_leg2_deep_vsEle_down[1] * idAndIsoSF_leg2_deep_vsMu;

        if (DEBUG)
        {
          cout << "--- DEBUG idAndIsoSF ---" << endl;
          cout << "pairType  : "                    << pType                         << endl;
          cout << "totSF deep: "                    << idAndIsoAndFakeSF_deep        << endl;
          cout << "totSF deep_pt: "                 << idAndIsoAndFakeSF_deep_pt     << endl;
          cout << "idAndIsoSF_leg1_deep_vsJet: "    << idAndIsoSF_leg1_deep_vsJet    << endl;
          cout << "idAndIsoSF_leg1_deep_vsJet_pt: " << idAndIsoSF_leg1_deep_vsJet_pt << endl;
          cout << "idAndIsoSF_leg1_deep_vsEle: "    << idAndIsoSF_leg1_deep_vsEle    << endl;
          cout << "idAndIsoSF_leg1_deep_vsMu: "     << idAndIsoSF_leg1_deep_vsMu     << endl;
          cout << "idAndIsoSF_leg2_deep_vsJet: "    << idAndIsoSF_leg2_deep_vsJet    << endl;
          cout << "idAndIsoSF_leg2_deep_vsJet_pt: " << idAndIsoSF_leg2_deep_vsJet_pt << endl;
          cout << "idAndIsoSF_leg2_deep_vsEle: "    << idAndIsoSF_leg2_deep_vsEle    << endl;
          cout << "idAndIsoSF_leg2_deep_vsMu: "     << idAndIsoSF_leg2_deep_vsMu     << endl;
        }
      }

      // MuMu Channel
      else if (pType == 3 && isMC)
      {
        float mu1pt  = tlv_firstLepton.Pt();
        float mu1eta = TMath::Abs(tlv_firstLepton.Eta());
        float mu2pt  = tlv_secondLepton.Pt();
        float mu2eta = TMath::Abs(tlv_secondLepton.Eta());

        float idAndIsoSF_leg1 = 1.;
        float idAndIsoSF_leg2 = 1.;

        if (mu1eta < 2.4)
        {
          idAndIsoSF_leg1 = myIDandISOScaleFactor[0]->get_ScaleFactor(mu1pt, mu1eta);
        }
        if (mu2eta < 2.4)
        {
          idAndIsoSF_leg2 = myIDandISOScaleFactor[0]->get_ScaleFactor(mu2pt, mu2eta);
        }

        idAndIsoSF_MVA = idAndIsoSF_deep = idAndIsoAndFakeSF_MVA = idAndIsoAndFakeSF_deep = idAndIsoSF_leg1 * idAndIsoSF_leg2;

        if (DEBUG)
        {
          cout << "--- DEBUG idAndIsoSF ---" << endl;
          cout << "pairType  : " << pType << endl;
          cout << "totSF MVA : " << idAndIsoSF_MVA << endl;
          cout << "totSF deep: " << idAndIsoSF_deep << endl;
        }
      }
      
      // EleEle Channel
      else if (pType == 4 && isMC)
      {
        float ele1pt  = tlv_firstLepton.Pt();
        float ele1eta = TMath::Abs(tlv_firstLepton.Eta());
        float ele2pt  = tlv_secondLepton.Pt();
        float ele2eta = TMath::Abs(tlv_secondLepton.Eta());

        float idAndIsoSF_leg1 = 1.;
        float idAndIsoSF_leg2 = 1.;

        if (ele1eta < 2.4)
        {
          idAndIsoSF_leg1 = myIDandISOScaleFactor[1]->get_ScaleFactor(ele1pt, ele1eta);
        }
        if (ele2eta < 2.4)
        {
          idAndIsoSF_leg2 = myIDandISOScaleFactor[1]->get_ScaleFactor(ele2pt, ele2eta);
        }

        idAndIsoSF_MVA = idAndIsoSF_deep = idAndIsoAndFakeSF_MVA = idAndIsoAndFakeSF_deep = idAndIsoSF_leg1 * idAndIsoSF_leg2;

        if (DEBUG)
        {
          cout << "--- DEBUG idAndIsoSF ---" << endl;
          cout << "pairType  : " << pType << endl;
          cout << "totSF MVA : " << idAndIsoSF_MVA << endl;
          cout << "totSF deep: " << idAndIsoSF_deep << endl;
        }
      }

      // Save the IDandISO SF (event per event)
      theSmallTree.m_IdAndIsoSF_MVA            = (isMC ? idAndIsoSF_MVA            : 1.0);
      theSmallTree.m_IdAndIsoSF_deep           = (isMC ? idAndIsoSF_deep           : 1.0);
      theSmallTree.m_IdAndIsoSF_deep_pt        = (isMC ? idAndIsoSF_deep_pt        : 1.0);
      theSmallTree.m_IdAndIsoAndFakeSF_MVA     = (isMC ? idAndIsoAndFakeSF_MVA     : 1.0);
      theSmallTree.m_IdAndIsoAndFakeSF_deep    = (isMC ? idAndIsoAndFakeSF_deep    : 1.0);
      theSmallTree.m_IdAndIsoAndFakeSF_deep_pt = (isMC ? idAndIsoAndFakeSF_deep_pt : 1.0);
      theSmallTree.m_FakeRateSF_MVA            = (isMC ? fakeRateSF_MVA            : 1.0);
      theSmallTree.m_FakeRateSF_deep           = (isMC ? fakeRateSF_deep           : 1.0);

      theSmallTree.m_idAndIsoAndFakeSF_tauid_pt20to25_up        = (isMC ? idAndIsoAndFakeSF_tauid_pt20to25_up        : 1);
      theSmallTree.m_idAndIsoAndFakeSF_tauid_pt25to30_up        = (isMC ? idAndIsoAndFakeSF_tauid_pt25to30_up        : 1);
      theSmallTree.m_idAndIsoAndFakeSF_tauid_pt30to35_up        = (isMC ? idAndIsoAndFakeSF_tauid_pt30to35_up        : 1);
      theSmallTree.m_idAndIsoAndFakeSF_tauid_pt35to40_up        = (isMC ? idAndIsoAndFakeSF_tauid_pt35to40_up        : 1);
      theSmallTree.m_idAndIsoAndFakeSF_tauid_pt40toInf_up       = (isMC ? idAndIsoAndFakeSF_tauid_pt40toInf_up       : 1);
      theSmallTree.m_idAndIsoAndFakeSF_tauid_pt20to25_down      = (isMC ? idAndIsoAndFakeSF_tauid_pt20to25_down      : 1);
      theSmallTree.m_idAndIsoAndFakeSF_tauid_pt25to30_down      = (isMC ? idAndIsoAndFakeSF_tauid_pt25to30_down      : 1);
      theSmallTree.m_idAndIsoAndFakeSF_tauid_pt30to35_down      = (isMC ? idAndIsoAndFakeSF_tauid_pt30to35_down      : 1);
      theSmallTree.m_idAndIsoAndFakeSF_tauid_pt35to40_down      = (isMC ? idAndIsoAndFakeSF_tauid_pt35to40_down      : 1);
      theSmallTree.m_idAndIsoAndFakeSF_tauid_pt40toInf_down     = (isMC ? idAndIsoAndFakeSF_tauid_pt40toInf_down     : 1);
      theSmallTree.m_idAndIsoAndFakeSF_mutauFR_etaLt0p4_up      = (isMC ? idAndIsoAndFakeSF_mutauFR_etaLt0p4_up      : 1);
      theSmallTree.m_idAndIsoAndFakeSF_mutauFR_eta0p4to0p8_up   = (isMC ? idAndIsoAndFakeSF_mutauFR_eta0p4to0p8_up   : 1);
      theSmallTree.m_idAndIsoAndFakeSF_mutauFR_eta0p8to1p2_up   = (isMC ? idAndIsoAndFakeSF_mutauFR_eta0p8to1p2_up   : 1);
      theSmallTree.m_idAndIsoAndFakeSF_mutauFR_eta1p2to1p7_up   = (isMC ? idAndIsoAndFakeSF_mutauFR_eta1p2to1p7_up   : 1);
      theSmallTree.m_idAndIsoAndFakeSF_mutauFR_etaGt1p7_up      = (isMC ? idAndIsoAndFakeSF_mutauFR_etaGt1p7_up      : 1);
      theSmallTree.m_idAndIsoAndFakeSF_mutauFR_etaLt0p4_down    = (isMC ? idAndIsoAndFakeSF_mutauFR_etaLt0p4_down    : 1);
      theSmallTree.m_idAndIsoAndFakeSF_mutauFR_eta0p4to0p8_down = (isMC ? idAndIsoAndFakeSF_mutauFR_eta0p4to0p8_down : 1);
      theSmallTree.m_idAndIsoAndFakeSF_mutauFR_eta0p8to1p2_down = (isMC ? idAndIsoAndFakeSF_mutauFR_eta0p8to1p2_down : 1);
      theSmallTree.m_idAndIsoAndFakeSF_mutauFR_eta1p2to1p7_down = (isMC ? idAndIsoAndFakeSF_mutauFR_eta1p2to1p7_down : 1);
      theSmallTree.m_idAndIsoAndFakeSF_mutauFR_etaGt1p7_down    = (isMC ? idAndIsoAndFakeSF_mutauFR_etaGt1p7_down    : 1);
      theSmallTree.m_idAndIsoAndFakeSF_etauFR_barrel_up         = (isMC ? idAndIsoAndFakeSF_etauFR_barrel_up         : 1);
      theSmallTree.m_idAndIsoAndFakeSF_etauFR_endcap_up         = (isMC ? idAndIsoAndFakeSF_etauFR_endcap_up         : 1);
      theSmallTree.m_idAndIsoAndFakeSF_etauFR_barrel_down       = (isMC ? idAndIsoAndFakeSF_etauFR_barrel_down       : 1);
      theSmallTree.m_idAndIsoAndFakeSF_etauFR_endcap_down       = (isMC ? idAndIsoAndFakeSF_etauFR_endcap_down       : 1);

      //Jet faking Tau SF
      //derived from WJet sideband: http://camendol.web.cern.ch/camendol/HH2017/plotsHH2017MuTau/31Oct2018_DYNLO_ctrlWJets_SS/antiB_jets30_tau30_SStight/
      double jetFakeSF1 = 1.;
      double jetFakeSF2 = 1.;
      if (pType == 0 || pType == 1 || pType == 2) // 2nd tau
	{
	  if (isFakeJet2){
	    if(fabs(tlv_secondLepton.Eta()) < 1.46){
	      jetFakeSF2 = 1.38469;
	    }else{
	      jetFakeSF2 = 1.69035;
	    }     
	  }
	}
      
      if(pType == 2)                //1st tau
	{
	  if (isFakeJet1){
	    if(fabs(tlv_firstLepton.Eta()) < 1.46){
	      jetFakeSF1 = 1.38469;
	    }else{
	      jetFakeSF1 = 1.69035;
	    }     
	  }
	}
      theSmallTree.m_jetFakeSF = (isMC ? jetFakeSF1*jetFakeSF2 : 1.0);


      // DATA/MC Trigger ScaleFactors
      // https://github.com/CMS-HTT/LeptonEfficiencies
      // https://github.com/truggles/TauTriggerSFs2017

      // recommendations for cross triggers:  https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2017#Trigger_Information

      float trigSF = 1.0;
      float trigSF_DM0_up    = 1.0;
      float trigSF_DM1_up    = 1.0;
      float trigSF_DM10_up   = 1.0;
      float trigSF_DM11_up   = 1.0;
      float trigSF_DM0_down  = 1.0;
      float trigSF_DM1_down  = 1.0;
      float trigSF_DM10_down = 1.0;
      float trigSF_DM11_down = 1.0;
      float trigSF_single = 1.0;
      float trigSF_cross = 1.0;

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

            //cross-trigger
            //mu leg
            double SFl_Data = muTauTrgSF->get_EfficiencyData(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
            double SFl_MC = muTauTrgSF->get_EfficiencyMC(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());

            //tau leg
            double SFtau_Data = tauTrgSF_mutau->getEfficiencyData(tlv_secondLepton.Pt(), DM2, 0); // last entry is uncertainty: 0 central, +1 up, -1 down
            double SFtau_MC   = tauTrgSF_mutau->getEfficiencyMC  (tlv_secondLepton.Pt(), DM2, 0); // last entry is uncertainty: 0 central, +1 up, -1 down

            //double Eff_Data =  passSingle * SFL_Data * (1 - passCross * SFtau_Data) + passCross * SFl_Data * SFtau_Data;
            //double Eff_MC   =  passSingle * SFL_MC * (1 - passCross * SFtau_MC) + passCross * SFl_MC * SFtau_MC;
            double Eff_Data = passSingle * SFL_Data - passCross * passSingle * std::min(SFl_Data, SFL_Data) * SFtau_Data + passCross * SFl_Data * SFtau_Data;
            double Eff_MC   = passSingle * SFL_MC   - passCross * passSingle * std::min(SFl_MC  , SFL_MC)   * SFtau_MC   + passCross * SFl_MC   * SFtau_MC;

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
            trigSF = SF;
          }
          //trig SF for analysis only with single-mu trigger
          trigSF_single =  muTrgSF->get_ScaleFactor(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
        }

        // EleTau Channel
        else if (pType == 1 && isMC)
        {
          // In 2016 there is no cross etau trigger --> only single ele trigger SF is needed
          trigSF        = eTrgSF->get_ScaleFactor(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
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
          trigSF = SF;
	    }

        // EleEle Channel
        else if (pType == 4 && isMC)
        {
          double SF = eTrgSF->get_ScaleFactor(tlv_firstLepton.Pt(), tlv_firstLepton.Eta());
          trigSF = SF;
        }
      } // end if(applytriggers)

      theSmallTree.m_trigSF           = (isMC ? trigSF : 1.0);
      theSmallTree.m_trigSF_DM0_up    = (isMC ? trigSF_DM0_up : 1.0);
      theSmallTree.m_trigSF_DM1_up    = (isMC ? trigSF_DM1_up : 1.0);
      theSmallTree.m_trigSF_DM10_up   = (isMC ? trigSF_DM10_up : 1.0);
      theSmallTree.m_trigSF_DM11_up   = (isMC ? trigSF_DM11_up : 1.0);
      theSmallTree.m_trigSF_DM0_down  = (isMC ? trigSF_DM0_down : 1.0);
      theSmallTree.m_trigSF_DM1_down  = (isMC ? trigSF_DM1_down : 1.0);
      theSmallTree.m_trigSF_DM10_down = (isMC ? trigSF_DM10_down : 1.0);
      theSmallTree.m_trigSF_DM11_down = (isMC ? trigSF_DM11_down : 1.0);
      theSmallTree.m_trigSF_single = (isMC ? trigSF_single : 1.0);
      theSmallTree.m_trigSF_cross  = (isMC ? trigSF_cross : 1.0);

      theSmallTree.m_totalWeight = (isMC? (35920./7.20811e+10) * theSmallTree.m_MC_weight * theSmallTree.m_PUReweight * theSmallTree.m_DYscale_MTT * trigSF * theSmallTree.m_IdAndIsoAndFakeSF_deep_pt * theSmallTree.m_L1pref_weight: 1.0);
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
          bool passIso    = oph.eleBaseline (&theBigTree, iLep, 10., 2.5, 0.3, OfflineProducerHelper::EMVAMedium, string("Vertex-LepID-pTMin-etaMax"), (DEBUG ? true : false));
          bool passNonISo = oph.eleBaseline (&theBigTree, iLep, 10., 2.5, 0.3, OfflineProducerHelper::EMVAMedium, string("Vertex-pTMin-etaMax-thirdLep"), (DEBUG ? true : false));
          if (!passIso && !passNonISo) continue; // if it passes one of the two --> the "if" is false and the lepton is saved as an extra lepton
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

        TLorentzVector tlv_jet
        (
         theBigTree.jets_px->at (iJet),
         theBigTree.jets_py->at (iJet),
         theBigTree.jets_pz->at (iJet),
         theBigTree.jets_e->at (iJet)
        ) ;

        if (DEBUG)
        {
          cout << "------- Jets DEBUG---------" << endl;
          cout << "iJet: " << iJet << "  -- pT/Eta/Phi: " << tlv_jet.Pt() << "/" << tlv_jet.Eta() << "/" << tlv_jet.Phi() << endl;
          cout << "discr: " << theBigTree.jets_PUJetID->at(iJet) << " --  discrUpdated: " << theBigTree.jets_PUJetIDupdated->at(iJet) << endl;
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
        vector<float> bTagWeight = bTagSFHelper.getEvtWeight (jets_and_BTag, theBigTree.jets_px, theBigTree.jets_py, theBigTree.jets_pz, theBigTree.jets_e, theBigTree.jets_HadronFlavour, pType, bTagSF::central) ;
        theSmallTree.m_bTagweightL = (isMC ? bTagWeight.at(0) : 1.0) ;
        theSmallTree.m_bTagweightM = (isMC ? bTagWeight.at(1) : 1.0) ;
        theSmallTree.m_bTagweightT = (isMC ? bTagWeight.at(2) : 1.0) ;

        vector<float> bTagWeight_up = bTagSFHelper.getEvtWeight (jets_and_BTag, theBigTree.jets_px, theBigTree.jets_py, theBigTree.jets_pz, theBigTree.jets_e, theBigTree.jets_HadronFlavour, pType, bTagSF::up) ;
        theSmallTree.m_bTagweightL_up = (isMC ? bTagWeight_up.at(0) : 1.0) ;
        theSmallTree.m_bTagweightM_up = (isMC ? bTagWeight_up.at(1) : 1.0) ;
        theSmallTree.m_bTagweightT_up = (isMC ? bTagWeight_up.at(2) : 1.0) ;

        vector<float> bTagWeight_down = bTagSFHelper.getEvtWeight (jets_and_BTag, theBigTree.jets_px, theBigTree.jets_py, theBigTree.jets_pz, theBigTree.jets_e, theBigTree.jets_HadronFlavour, pType, bTagSF::down) ;
        theSmallTree.m_bTagweightL_down = (isMC ? bTagWeight_down.at(0) : 1.0) ;
        theSmallTree.m_bTagweightM_down = (isMC ? bTagWeight_down.at(1) : 1.0) ;
        theSmallTree.m_bTagweightT_down = (isMC ? bTagWeight_down.at(2) : 1.0) ;

        // Set HHbtaginterface for ordering jets
        HHbtagTagger.SetInputValues(theBigTree, jets_and_sortPar, theSmallTree.m_BDT_channel,
          tlv_firstLepton, tlv_secondLepton, tlv_tauH, tlv_MET, theSmallTree.m_EventNumber);

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
            ijet.SetPxPyPzE(
                           theBigTree.jets_px->at (iJet),
                           theBigTree.jets_py->at (iJet),
                           theBigTree.jets_pz->at (iJet),
                           theBigTree.jets_e ->at (iJet)
                           );

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
              kjet.SetPxPyPzE(
                             theBigTree.jets_px->at (kJet),
                             theBigTree.jets_py->at (kJet),
                             theBigTree.jets_pz->at (kJet),
                             theBigTree.jets_e ->at (kJet)
                             );

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
        TLorentzVector tlv_secondBjet(theBigTree.jets_px->at(bjet2idx), theBigTree.jets_py->at(bjet2idx), theBigTree.jets_pz->at(bjet2idx), theBigTree.jets_e->at(bjet2idx));
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

        // Total JES up/down variation
        TLorentzVector tlv_firstBjet_raw_jetupTot    = getShiftedJet(tlv_firstBjet_raw , +1., theBigTree.jets_jetUncRegrouped_Total_up->at(bjet1idx));
        TLorentzVector tlv_firstBjet_raw_jetdownTot  = getShiftedJet(tlv_firstBjet_raw , -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(bjet1idx));
        TLorentzVector tlv_secondBjet_raw_jetupTot   = getShiftedJet(tlv_secondBjet_raw, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(bjet2idx));
        TLorentzVector tlv_secondBjet_raw_jetdownTot = getShiftedJet(tlv_secondBjet_raw, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(bjet2idx));
        TLorentzVector tlv_bH_raw_jetupTot           = tlv_firstBjet_raw_jetupTot + tlv_secondBjet_raw_jetupTot;
        TLorentzVector tlv_bH_raw_jetdownTot         = tlv_firstBjet_raw_jetdownTot + tlv_secondBjet_raw_jetdownTot;

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

        // ----- up/down variation using 11 JEC sources
        // https://github.com/LLRCMS/LLRHiggsTauTau/blob/102X_HH/NtupleProducer/plugins/HTauTauNtuplizer.cc#L2182-L2238
        // store the up down variations in vectors:
	
        pair <vector <double>, vector<double>> unc_first_updown = getJetUpDown(bjet1idx, theBigTree);
        pair <vector <double>, vector<double>> unc_second_updown = getJetUpDown(bjet2idx, theBigTree);

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
        theSmallTree.m_bjet1_JER = bjet1_JER;
        theSmallTree.m_bjet2_JER = bjet2_JER;

        // Total JES up/down variation of JER
        double bjet1_JER_jetupTot   = bjet1_JER * tlv_firstBjet_raw_jetupTot.E() / tlv_firstBjet_raw.E();
        double bjet2_JER_jetupTot   = bjet2_JER * tlv_secondBjet_raw_jetupTot.E() / tlv_secondBjet_raw.E();
        double bjet1_JER_jetdownTot = bjet1_JER * tlv_firstBjet_raw_jetdownTot.E() / tlv_firstBjet_raw.E();
        double bjet2_JER_jetdownTot = bjet2_JER * tlv_secondBjet_raw_jetdownTot.E() / tlv_secondBjet_raw.E();
        theSmallTree.m_bjet1_JER_jetupTot   = bjet1_JER_jetupTot;
        theSmallTree.m_bjet2_JER_jetupTot   = bjet2_JER_jetupTot;
        theSmallTree.m_bjet1_JER_jetdownTot = bjet1_JER_jetdownTot;
        theSmallTree.m_bjet2_JER_jetdownTot = bjet2_JER_jetdownTot;

        //compute and store variations
        for (int isource = 0; isource < N_jecSources; isource++)
        {
	   // compute all shifted bjets
           tlv_firstBjet_raw_jetup[isource]   = getShiftedJet(tlv_firstBjet_raw, +1., unc_first_updown.first[isource]);
           tlv_firstBjet_raw_jetdown[isource] = getShiftedJet(tlv_firstBjet_raw, -1., unc_first_updown.second[isource]);
           tlv_secondBjet_raw_jetup[isource]  = getShiftedJet(tlv_secondBjet_raw, +1., unc_second_updown.first[isource]);
           tlv_secondBjet_raw_jetdown[isource]= getShiftedJet(tlv_secondBjet_raw, -1., unc_second_updown.second[isource]);

           theSmallTree.m_bjet1_mass_raw_jetup.push_back(tlv_firstBjet_raw_jetup[isource].M()); 
           theSmallTree.m_bjet1_mass_raw_jetdown.push_back(tlv_firstBjet_raw_jetdown[isource].M()); 
           theSmallTree.m_bjet1_pt_raw_jetup.push_back(tlv_firstBjet_raw_jetup[isource].Pt()); 
           theSmallTree.m_bjet1_pt_raw_jetdown.push_back(tlv_firstBjet_raw_jetdown[isource].Pt()); 
           theSmallTree.m_bjet2_mass_raw_jetup.push_back(tlv_secondBjet_raw_jetup[isource].M()); 
           theSmallTree.m_bjet2_mass_raw_jetdown.push_back(tlv_secondBjet_raw_jetdown[isource].M()); 
           theSmallTree.m_bjet2_pt_raw_jetup.push_back(tlv_secondBjet_raw_jetup[isource].Pt()); 
           theSmallTree.m_bjet2_pt_raw_jetdown.push_back(tlv_secondBjet_raw_jetdown[isource].Pt()); 
	   
           // variations propagated to bH 
           tlv_bH_raw_jetup.push_back(tlv_firstBjet_raw_jetup.at(isource) + tlv_secondBjet_raw_jetup.at(isource)) ;
           tlv_bH_raw_jetdown.push_back(tlv_firstBjet_raw_jetdown.at(isource) + tlv_secondBjet_raw_jetdown.at(isource));
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

        theSmallTree.m_bjet1_pt_raw_jetdown1	= (tlv_firstBjet_raw_jetdown[0]).Pt();
        theSmallTree.m_bjet1_pt_raw_jetdown2	= (tlv_firstBjet_raw_jetdown[1]).Pt();
        theSmallTree.m_bjet1_pt_raw_jetdown3	= (tlv_firstBjet_raw_jetdown[2]).Pt();
        theSmallTree.m_bjet1_pt_raw_jetdown4	= (tlv_firstBjet_raw_jetdown[3]).Pt();
        theSmallTree.m_bjet1_pt_raw_jetdown5	= (tlv_firstBjet_raw_jetdown[4]).Pt();
        theSmallTree.m_bjet1_pt_raw_jetdown6	= (tlv_firstBjet_raw_jetdown[5]).Pt();
        theSmallTree.m_bjet1_pt_raw_jetdown7	= (tlv_firstBjet_raw_jetdown[6]).Pt();
        theSmallTree.m_bjet1_pt_raw_jetdown8	= (tlv_firstBjet_raw_jetdown[7]).Pt();
        theSmallTree.m_bjet1_pt_raw_jetdown9	= (tlv_firstBjet_raw_jetdown[8]).Pt();
        theSmallTree.m_bjet1_pt_raw_jetdown10	= (tlv_firstBjet_raw_jetdown[9]).Pt();
        theSmallTree.m_bjet1_pt_raw_jetdown11	= (tlv_firstBjet_raw_jetdown[10]).Pt();

        theSmallTree.m_bjet2_pt_raw_jetdown1	= (tlv_secondBjet_raw_jetdown[0]).Pt();
        theSmallTree.m_bjet2_pt_raw_jetdown2	= (tlv_secondBjet_raw_jetdown[1]).Pt();
        theSmallTree.m_bjet2_pt_raw_jetdown3	= (tlv_secondBjet_raw_jetdown[2]).Pt();
        theSmallTree.m_bjet2_pt_raw_jetdown4	= (tlv_secondBjet_raw_jetdown[3]).Pt();
        theSmallTree.m_bjet2_pt_raw_jetdown5	= (tlv_secondBjet_raw_jetdown[4]).Pt();
        theSmallTree.m_bjet2_pt_raw_jetdown6	= (tlv_secondBjet_raw_jetdown[5]).Pt();
        theSmallTree.m_bjet2_pt_raw_jetdown7	= (tlv_secondBjet_raw_jetdown[6]).Pt();
        theSmallTree.m_bjet2_pt_raw_jetdown8	= (tlv_secondBjet_raw_jetdown[7]).Pt();
        theSmallTree.m_bjet2_pt_raw_jetdown9	= (tlv_secondBjet_raw_jetdown[8]).Pt();
        theSmallTree.m_bjet2_pt_raw_jetdown10	= (tlv_secondBjet_raw_jetdown[9]).Pt();
        theSmallTree.m_bjet2_pt_raw_jetdown11	= (tlv_secondBjet_raw_jetdown[10]).Pt();

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

        theSmallTree.m_bH_mass_raw_jetdown1    = (tlv_bH_raw_jetdown[0]).M();		 
        theSmallTree.m_bH_mass_raw_jetdown2    = (tlv_bH_raw_jetdown[1]).M();		 
        theSmallTree.m_bH_mass_raw_jetdown3    = (tlv_bH_raw_jetdown[2]).M();		 
        theSmallTree.m_bH_mass_raw_jetdown4    = (tlv_bH_raw_jetdown[3]).M();		 
        theSmallTree.m_bH_mass_raw_jetdown5    = (tlv_bH_raw_jetdown[4]).M();		 
        theSmallTree.m_bH_mass_raw_jetdown6    = (tlv_bH_raw_jetdown[5]).M();		 
        theSmallTree.m_bH_mass_raw_jetdown7    = (tlv_bH_raw_jetdown[6]).M();		 
        theSmallTree.m_bH_mass_raw_jetdown8    = (tlv_bH_raw_jetdown[7]).M();		 
        theSmallTree.m_bH_mass_raw_jetdown9    = (tlv_bH_raw_jetdown[8]).M();		 
        theSmallTree.m_bH_mass_raw_jetdown10   = (tlv_bH_raw_jetdown[9]).M();		 
        theSmallTree.m_bH_mass_raw_jetdown11   = (tlv_bH_raw_jetdown[10]).M();	

        // FIXME : here mass is manually set to 0, should we change it?
        float ptScale1 = ptRegr[0] / tlv_firstBjet.Pt() ;
        float ptScale2 = ptRegr[1] / tlv_secondBjet.Pt() ;
        tlv_firstBjet.SetPtEtaPhiE (ptRegr[0], tlv_firstBjet.Eta(), tlv_firstBjet.Phi(), ptScale1*tlv_firstBjet.Energy());
        tlv_secondBjet.SetPtEtaPhiE (ptRegr[1], tlv_secondBjet.Eta(), tlv_secondBjet.Phi(), ptScale2*tlv_secondBjet.Energy());

        theSmallTree.m_bjet1_pt   = tlv_firstBjet.Pt () ;
        theSmallTree.m_bjet1_eta  = tlv_firstBjet.Eta () ;
        theSmallTree.m_bjet1_phi  = tlv_firstBjet.Phi () ;
        theSmallTree.m_bjet1_e    = theBigTree.jets_e->at (bjet1idx) ;
        theSmallTree.m_bjet1_bID  = theBigTree.bCSVscore->at (bjet1idx) ;
        theSmallTree.m_bjet1_bID_deepCSV  = theBigTree.bDeepCSV_probb->at(bjet1idx) + theBigTree.bDeepCSV_probbb->at(bjet1idx) ;
        theSmallTree.m_bjet1_bID_deepFlavor  = theBigTree.bDeepFlavor_probb->at(bjet1idx) + theBigTree.bDeepFlavor_probbb->at(bjet1idx) + theBigTree.bDeepFlavor_problepb->at(bjet1idx);
        theSmallTree.m_bjet1_cID_deepFlavor  = theBigTree.bDeepFlavor_probc->at(bjet1idx);
        theSmallTree.m_bjet1_CvsL = getCvsL(theBigTree, bjet1idx);
        theSmallTree.m_bjet1_CvsB = getCvsB(theBigTree, bjet1idx);
        theSmallTree.m_bjet1_bMVAID  = theBigTree.pfCombinedMVAV2BJetTags->at (bjet1idx) ;
        theSmallTree.m_bjet1_PUjetIDupdated = theBigTree.jets_PUJetIDupdated->at(bjet1idx);
        theSmallTree.m_bjet1_flav = theBigTree.jets_HadronFlavour->at (bjet1idx) ;

        theSmallTree.m_bjet2_pt   = tlv_secondBjet.Pt () ;
        theSmallTree.m_bjet2_eta  = tlv_secondBjet.Eta () ;
        theSmallTree.m_bjet2_phi  = tlv_secondBjet.Phi () ;
        theSmallTree.m_bjet2_e    = theBigTree.jets_e->at (bjet2idx) ;
        theSmallTree.m_bjet2_bID  = theBigTree.bCSVscore->at (bjet2idx) ;
        theSmallTree.m_bjet2_bID_deepCSV  = theBigTree.bDeepCSV_probb->at(bjet2idx) + theBigTree.bDeepCSV_probbb->at(bjet2idx) ;
        theSmallTree.m_bjet2_bID_deepFlavor  = theBigTree.bDeepFlavor_probb->at(bjet2idx) + theBigTree.bDeepFlavor_probbb->at(bjet2idx) + theBigTree.bDeepFlavor_problepb->at(bjet2idx);
        theSmallTree.m_bjet2_cID_deepFlavor  = theBigTree.bDeepFlavor_probc->at(bjet2idx);
        theSmallTree.m_bjet2_CvsL = getCvsL(theBigTree, bjet2idx);
        theSmallTree.m_bjet2_CvsB = getCvsB(theBigTree, bjet2idx);
        theSmallTree.m_bjet2_bMVAID  = theBigTree.pfCombinedMVAV2BJetTags->at (bjet2idx) ;
        theSmallTree.m_bjet2_PUjetIDupdated = theBigTree.jets_PUJetIDupdated->at(bjet2idx);
        theSmallTree.m_bjet2_flav = theBigTree.jets_HadronFlavour->at (bjet2idx) ;

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

              // JES total shift
              TLorentzVector tlv_jetupTot   = getShiftedJet(tlv_jet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet));
              TLorentzVector tlv_jetdownTot = getShiftedJet(tlv_jet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet));
              if (tlv_jetupTot.Pt()   > 20) theSmallTree.m_BDT_HT20_jetupTot += tlv_jetupTot.Pt();
              if (tlv_jetdownTot.Pt() > 20) theSmallTree.m_BDT_HT20_jetdownTot += tlv_jetdownTot.Pt();

	      pair <vector <double>, vector<double>> unc_updown = getJetUpDown(iJet, theBigTree);
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
        //float METpt = 0;//TMath::Sqrt (METx*METx + METy*METy) ;

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
        auto vMET_shift_jet = getShiftedMET_jet(N_jecSources, vMET, theBigTree, DEBUG);
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
          //unpack: first is tes, second is ees
          auto vMET_shift_tes = vMET_shifts_tes_ees.first;
          auto vMET_shift_ees = vMET_shifts_tes_ees.second;
          for (int idm = 0; idm < N_tauhDM; idm++)
          {
            theSmallTree.m_METx_tauup.push_back(vMET_shift_tes.first.at(idm).X());
            theSmallTree.m_METy_tauup.push_back(vMET_shift_tes.first.at(idm).Y());
            theSmallTree.m_METx_taudown.push_back(vMET_shift_tes.second.at(idm).X());
            theSmallTree.m_METy_taudown.push_back(vMET_shift_tes.second.at(idm).Y());

            if (idm <N_tauhDM_EES)
            {
              theSmallTree.m_METx_eleup.push_back(vMET_shift_ees.first.at(idm).X());
              theSmallTree.m_METy_eleup.push_back(vMET_shift_ees.first.at(idm).Y());
              theSmallTree.m_METx_eledown.push_back(vMET_shift_ees.second.at(idm).X());
              theSmallTree.m_METy_eledown.push_back(vMET_shift_ees.second.at(idm).Y());
            }
          }

          // Shifted MET for MES
          auto vMET_shift_mes = getShiftedMET_mes(vMET, theBigTree, DEBUG);
          theSmallTree.m_METx_muup = vMET_shift_mes.first.X();
          theSmallTree.m_METy_muup = vMET_shift_mes.first.Y();
          theSmallTree.m_METx_mudown = vMET_shift_mes.second.X();
          theSmallTree.m_METy_mudown = vMET_shift_mes.second.Y();

          // Shifted MET for JES total
          auto vMET_shift_jetTot = getShiftedMET_jetTot(vMET, theBigTree, DEBUG);
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
        if (theBigTree.SVfitMass->at (chosenTauPair) > -900.)
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
          if (theBigTree.SVfitMass->at (chosenTauPair) > -900. && !wrongHHK)
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

          double pxMiss = tlv_firstLepton.Px() + tlv_secondLepton.Px() + theBigTree.METx->at(chosenTauPair); // x component of missing transverse momentum.
          double pyMiss = tlv_firstLepton.Py() + tlv_secondLepton.Py() + theBigTree.METy->at(chosenTauPair); // y component of missing transverse momentum.

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
        theSmallTree.m_bHMet_deltaPhi = deltaPhi (theBigTree.metphi, tlv_bH.Phi ()) ;
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

          // Total JES up/down variation
          TLorentzVector tlv_VBFjet1_jetupTot   = getShiftedJet(VBFjet1, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(VBFidx1));
          TLorentzVector tlv_VBFjet1_jetdownTot = getShiftedJet(VBFjet1, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(VBFidx1));
          TLorentzVector tlv_VBFjet2_jetupTot   = getShiftedJet(VBFjet2, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(VBFidx2));
          TLorentzVector tlv_VBFjet2_jetdownTot = getShiftedJet(VBFjet2, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(VBFidx2));

          theSmallTree.m_VBFjet1_pt_jetupTot       = tlv_VBFjet1_jetupTot.Pt();
          theSmallTree.m_VBFjet1_pt_jetdownTot     = tlv_VBFjet1_jetdownTot.Pt();
          theSmallTree.m_VBFjet2_pt_jetupTot       = tlv_VBFjet2_jetupTot.Pt();
          theSmallTree.m_VBFjet2_pt_jetdownTot     = tlv_VBFjet2_jetdownTot.Pt();
          theSmallTree.m_VBFjet1_mass_jetupTot     = tlv_VBFjet1_jetupTot.M();
          theSmallTree.m_VBFjet1_mass_jetdownTot   = tlv_VBFjet1_jetdownTot.M();
          theSmallTree.m_VBFjet2_mass_jetupTot     = tlv_VBFjet2_jetupTot.M();
          theSmallTree.m_VBFjet2_mass_jetdownTot   = tlv_VBFjet2_jetdownTot.M();
          theSmallTree.m_VBFjj_mass_jetupTot       = (tlv_VBFjet1_jetupTot + tlv_VBFjet2_jetupTot).M();
          theSmallTree.m_VBFjj_mass_jetdownTot     = (tlv_VBFjet1_jetdownTot + tlv_VBFjet2_jetdownTot).M();

 	  pair <vector <double>, vector<double>> unc_VBF1_updown = getJetUpDown(VBFidx1, theBigTree);
 	  pair <vector <double>, vector<double>> unc_VBF2_updown = getJetUpDown(VBFidx2, theBigTree);

 	  // compute all shifted VBFjets
 	  vector <TLorentzVector> VBFjet1_jetup(N_jecSources,VBFjet1); 
 	  vector <TLorentzVector> VBFjet1_jetdown(N_jecSources,VBFjet1);
 	  vector <TLorentzVector> VBFjet2_jetup(N_jecSources,VBFjet2); 
 	  vector <TLorentzVector> VBFjet2_jetdown(N_jecSources,VBFjet2);

 	  vector <TLorentzVector> VBFjj_jetup(N_jecSources, VBFjet1+VBFjet2); 
 	  vector <TLorentzVector> VBFjj_jetdown(N_jecSources, VBFjet1+VBFjet2);

 	  for (int isource = 0; isource < N_jecSources; isource++)
 	  {

           VBFjet1_jetup[isource]   = getShiftedJet(VBFjet1, +1., unc_VBF1_updown.first[isource]);
           VBFjet1_jetdown[isource] = getShiftedJet(VBFjet1, -1., unc_VBF1_updown.second[isource]);
           VBFjet2_jetup[isource]  = getShiftedJet(VBFjet2, +1., unc_VBF2_updown.first[isource]);
           VBFjet2_jetdown[isource]= getShiftedJet(VBFjet2, -1., unc_VBF2_updown.second[isource]);

           theSmallTree.m_VBFjet1_pt_jetup    [isource] = VBFjet1_jetup[isource].Pt();
           theSmallTree.m_VBFjet1_pt_jetdown  [isource] = VBFjet1_jetdown[isource].Pt();
           theSmallTree.m_VBFjet1_mass_jetup  [isource] = VBFjet1_jetup[isource].M();
           theSmallTree.m_VBFjet1_mass_jetdown[isource] = VBFjet1_jetdown[isource].M();
           theSmallTree.m_VBFjet2_pt_jetup    [isource] = VBFjet2_jetup[isource].Pt();
           theSmallTree.m_VBFjet2_pt_jetdown  [isource] = VBFjet2_jetdown[isource].Pt();
           theSmallTree.m_VBFjet2_mass_jetup  [isource] = VBFjet2_jetup[isource].M();
           theSmallTree.m_VBFjet2_mass_jetdown[isource] = VBFjet2_jetdown[isource].M();
	   
           VBFjj_jetup[isource] = VBFjet1_jetup[isource] + VBFjet2_jetup[isource];
           VBFjj_jetdown[isource] = VBFjet1_jetdown[isource] + VBFjet2_jetdown[isource];
	      
           theSmallTree.m_VBFjj_mass_jetup  [isource] = VBFjj_jetup[isource].M();
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

          theSmallTree.m_VBFjj_mass_jetup1	 = VBFjj_jetup[0].M();   
          theSmallTree.m_VBFjj_mass_jetup2	 = VBFjj_jetup[1].M();        
          theSmallTree.m_VBFjj_mass_jetup3	 = VBFjj_jetup[2].M();        
          theSmallTree.m_VBFjj_mass_jetup4	 = VBFjj_jetup[3].M();        
          theSmallTree.m_VBFjj_mass_jetup5	 = VBFjj_jetup[4].M();        
          theSmallTree.m_VBFjj_mass_jetup6	 = VBFjj_jetup[5].M();        
          theSmallTree.m_VBFjj_mass_jetup7	 = VBFjj_jetup[6].M();        
          theSmallTree.m_VBFjj_mass_jetup8	 = VBFjj_jetup[7].M();        
          theSmallTree.m_VBFjj_mass_jetup9	 = VBFjj_jetup[8].M();        
          theSmallTree.m_VBFjj_mass_jetup10	 = VBFjj_jetup[9].M();        
          theSmallTree.m_VBFjj_mass_jetup11	 = VBFjj_jetup[10].M();       

          theSmallTree.m_VBFjj_mass_jetdown1	 = VBFjj_jetdown[0].M();      
          theSmallTree.m_VBFjj_mass_jetdown2	 = VBFjj_jetdown[1].M();      
          theSmallTree.m_VBFjj_mass_jetdown3	 = VBFjj_jetdown[2].M();      
          theSmallTree.m_VBFjj_mass_jetdown4	 = VBFjj_jetdown[3].M();      
          theSmallTree.m_VBFjj_mass_jetdown5	 = VBFjj_jetdown[4].M();      
          theSmallTree.m_VBFjj_mass_jetdown6	 = VBFjj_jetdown[5].M();      
          theSmallTree.m_VBFjj_mass_jetdown7	 = VBFjj_jetdown[6].M();      
          theSmallTree.m_VBFjj_mass_jetdown8	 = VBFjj_jetdown[7].M();      
          theSmallTree.m_VBFjj_mass_jetdown9	 = VBFjj_jetdown[8].M();      
          theSmallTree.m_VBFjj_mass_jetdown10	 = VBFjj_jetdown[9].M();      
          theSmallTree.m_VBFjj_mass_jetdown11	 = VBFjj_jetdown[10].M();     

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
          theSmallTree.m_VBFjet2_PUjetIDupdated  = theBigTree.jets_PUJetIDupdated->at (VBFidx2) ;
          theSmallTree.m_VBFjet2_flav            = (theBigTree.jets_HadronFlavour->at (VBFidx2)) ;
          theSmallTree.m_VBFjet2_hasgenjet  	 = hasgj2_VBF ;
          theSmallTree.m_VBFjet2_PUjetID    	 = (theBigTree.jets_PUJetID->at (VBFidx2)); // VBF BDT
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
            theSmallTree.m_bjet1_jecUnc = theBigTree.jets_jecUnc->at(iJet);
            continue;
          }else if(int (iJet) == bjet2idx){
            theSmallTree.m_bjet2_jecUnc = theBigTree.jets_jecUnc->at(iJet);
            continue ;
          }
          TLorentzVector tlv_dummyJet(
                                      theBigTree.jets_px->at (iJet),
                                      theBigTree.jets_py->at (iJet),
                                      theBigTree.jets_pz->at (iJet),
                                      theBigTree.jets_e->at (iJet)
                                     );

          // Apply PUjetID only to jets with pt < 50 GeV ( https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorkingLegacyRun2#Jets )
          // PU jet ID WP = 2: loose
          if (PUjetID_WP > -1)
          {
            if ( !(CheckBit(theBigTree.jets_PUJetIDupdated_WP->at(iJet), PUjetID_WP)) && tlv_dummyJet.Pt()<50.) continue;
          }

          // remove jets that overlap with the tau selected in the leg 1 and 2
          if (tlv_firstLepton.DeltaR(tlv_dummyJet) < lepCleaningCone){
            theSmallTree.m_dau1_jecUnc = theBigTree.jets_jecUnc->at(iJet);
            continue;
          }
          if (tlv_secondLepton.DeltaR(tlv_dummyJet) < lepCleaningCone){
            theSmallTree.m_dau2_jecUnc = theBigTree.jets_jecUnc->at(iJet);
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
          theSmallTree.m_jets_e.push_back (theBigTree.jets_e->at (iJet)) ;
          theSmallTree.m_jets_btag.push_back (theBigTree.bCSVscore->at (iJet)) ;
          theSmallTree.m_jets_btag_deepCSV.push_back (theBigTree.bDeepCSV_probb->at(iJet) + theBigTree.bDeepCSV_probbb->at(iJet)) ;
          theSmallTree.m_jets_btag_deepFlavor.push_back (theBigTree.bDeepFlavor_probb->at(iJet) + theBigTree.bDeepFlavor_probbb->at(iJet) + theBigTree.bDeepFlavor_problepb->at(iJet)) ;
          theSmallTree.m_jets_ctag_deepFlavor.push_back (theBigTree.bDeepFlavor_probc->at(iJet));          
          theSmallTree.m_jets_CvsL.push_back( getCvsL(theBigTree, iJet) );
          theSmallTree.m_jets_CvsB.push_back( getCvsB(theBigTree, iJet) );
          theSmallTree.m_jets_flav.push_back (theBigTree.jets_HadronFlavour->at (iJet)) ;
          theSmallTree.m_jets_jecUnc.push_back (theBigTree.jets_jecUnc->at (iJet)) ;
          theSmallTree.m_jets_hasgenjet.push_back (hasgj) ;
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
        theSmallTree.m_PUjetID_SF = PUjetIDSFprovider.getEvtWeight(theBigTree, tlv_firstLepton, tlv_secondLepton);
        if (DEBUG)
        {
          std::cout << "---- PUjetID_SF debug ----" << std::endl;
          std::cout << "PUjetID_SF: " << theSmallTree.m_PUjetID_SF << std::endl;
          std::cout << "--------------------------" << std::endl;
        }
      }

      // --------------------------------------------
      // HHbtag: set input values
      //HHbtagTagger.SetInputValues(theBigTree, jets_and_sortPar, theSmallTree.m_BDT_channel,
      //    tlv_firstLepton, tlv_secondLepton, tlv_tauH, tlv_MET, theSmallTree.m_EventNumber);

      // HHbtag: get scores and save them in a map<jet_idx,HHbtag_score>
      //std::map<int,float> jets_and_HHbtag = HHbtagTagger.GetScore();

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
        pair <vector <double>, vector<double>> unc_additionalJet_updown = getJetUpDown(iJet, theBigTree);

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
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetCentr1_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
              theSmallTree.m_addJetCentr1_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
              theSmallTree.m_addJetCentr1_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
              theSmallTree.m_addJetCentr1_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
            }
            theSmallTree.m_addJetCentr1_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).Pt();
            theSmallTree.m_addJetCentr1_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).Pt();
            theSmallTree.m_addJetCentr1_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).M();
            theSmallTree.m_addJetCentr1_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).M();
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
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetCentr2_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
              theSmallTree.m_addJetCentr2_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
              theSmallTree.m_addJetCentr2_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
              theSmallTree.m_addJetCentr2_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
            }
            theSmallTree.m_addJetCentr2_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).Pt();
            theSmallTree.m_addJetCentr2_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).Pt();
            theSmallTree.m_addJetCentr2_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).M();
            theSmallTree.m_addJetCentr2_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).M();
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
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetCentr3_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
              theSmallTree.m_addJetCentr3_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
              theSmallTree.m_addJetCentr3_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
              theSmallTree.m_addJetCentr3_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
            }
            theSmallTree.m_addJetCentr3_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).Pt();
            theSmallTree.m_addJetCentr3_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).Pt();
            theSmallTree.m_addJetCentr3_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).M();
            theSmallTree.m_addJetCentr3_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).M();
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
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetCentr4_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
              theSmallTree.m_addJetCentr4_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
              theSmallTree.m_addJetCentr4_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
              theSmallTree.m_addJetCentr4_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
            }
            theSmallTree.m_addJetCentr4_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).Pt();
            theSmallTree.m_addJetCentr4_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).Pt();
            theSmallTree.m_addJetCentr4_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).M();
            theSmallTree.m_addJetCentr4_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).M();
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
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetCentr5_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
              theSmallTree.m_addJetCentr5_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
              theSmallTree.m_addJetCentr5_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
              theSmallTree.m_addJetCentr5_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
            }
            theSmallTree.m_addJetCentr5_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).Pt();
            theSmallTree.m_addJetCentr5_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).Pt();
            theSmallTree.m_addJetCentr5_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).M();
            theSmallTree.m_addJetCentr5_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).M();
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
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetForw1_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
              theSmallTree.m_addJetForw1_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
              theSmallTree.m_addJetForw1_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
              theSmallTree.m_addJetForw1_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
            }
            theSmallTree.m_addJetForw1_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).Pt();
            theSmallTree.m_addJetForw1_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).Pt();
            theSmallTree.m_addJetForw1_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).M();
            theSmallTree.m_addJetForw1_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).M();
          }
          else if (theSmallTree.m_addJetForw2_pt < 0.)
          {
            theSmallTree.m_addJetForw2_pt  = tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw2_eta = tlv_additionalJet.Eta();
            theSmallTree.m_addJetForw2_phi = tlv_additionalJet.Phi();
            theSmallTree.m_addJetForw2_e   = tlv_additionalJet.E();
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetForw2_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
              theSmallTree.m_addJetForw2_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
              theSmallTree.m_addJetForw2_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
              theSmallTree.m_addJetForw2_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
            }
            theSmallTree.m_addJetForw2_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).Pt();
            theSmallTree.m_addJetForw2_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).Pt();
            theSmallTree.m_addJetForw2_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).M();
            theSmallTree.m_addJetForw2_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).M();
          }
          else if (theSmallTree.m_addJetForw3_pt < 0.)
          {
            theSmallTree.m_addJetForw3_pt  = tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw3_eta = tlv_additionalJet.Eta();
            theSmallTree.m_addJetForw3_phi = tlv_additionalJet.Phi();
            theSmallTree.m_addJetForw3_e   = tlv_additionalJet.E();
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetForw3_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
              theSmallTree.m_addJetForw3_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
              theSmallTree.m_addJetForw3_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
              theSmallTree.m_addJetForw3_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
            }
            theSmallTree.m_addJetForw3_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).Pt();
            theSmallTree.m_addJetForw3_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).Pt();
            theSmallTree.m_addJetForw3_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).M();
            theSmallTree.m_addJetForw3_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).M();
          }
          else if (theSmallTree.m_addJetForw4_pt < 0.)
          {
            theSmallTree.m_addJetForw4_pt  = tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw4_eta = tlv_additionalJet.Eta();
            theSmallTree.m_addJetForw4_phi = tlv_additionalJet.Phi();
            theSmallTree.m_addJetForw4_e   = tlv_additionalJet.E();
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetForw4_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
              theSmallTree.m_addJetForw4_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
              theSmallTree.m_addJetForw4_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
              theSmallTree.m_addJetForw4_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
            }
            theSmallTree.m_addJetForw4_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).Pt();
            theSmallTree.m_addJetForw4_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).Pt();
            theSmallTree.m_addJetForw4_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).M();
            theSmallTree.m_addJetForw4_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).M();
          }
          else if (theSmallTree.m_addJetForw5_pt < 0.)
          {
            theSmallTree.m_addJetForw5_pt  = tlv_additionalJet.Pt();
            theSmallTree.m_addJetForw5_eta = tlv_additionalJet.Eta();
            theSmallTree.m_addJetForw5_phi = tlv_additionalJet.Phi();
            theSmallTree.m_addJetForw5_e   = tlv_additionalJet.E();
            for (int isource = 0; isource < N_jecSources; isource++)
            {
              theSmallTree.m_addJetForw5_pt_jetup    [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).Pt();
              theSmallTree.m_addJetForw5_pt_jetdown  [isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).Pt();
              theSmallTree.m_addJetForw5_mass_jetup  [isource] = (getShiftedJet(tlv_additionalJet, +1., unc_additionalJet_updown.first[isource]) ).M();
              theSmallTree.m_addJetForw5_mass_jetdown[isource] = (getShiftedJet(tlv_additionalJet, -1., unc_additionalJet_updown.second[isource])).M();
            }
            theSmallTree.m_addJetForw5_pt_jetupTot     = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).Pt();
            theSmallTree.m_addJetForw5_pt_jetdownTot   = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).Pt();
            theSmallTree.m_addJetForw5_mass_jetupTot   = (getShiftedJet(tlv_additionalJet, +1., theBigTree.jets_jetUncRegrouped_Total_up->at(iJet))).M();
            theSmallTree.m_addJetForw5_mass_jetdownTot = (getShiftedJet(tlv_additionalJet, -1., theBigTree.jets_jetUncRegrouped_Total_dw->at(iJet))).M();
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

      if (DEBUG) //FRA DEBUG
      {
        cout << "--------------" << endl;
        cout << " - Debug SFs -" << endl;
        cout << "  PU           : " << theSmallTree.m_PUReweight << endl;
        cout << "  IDandISO MVA : " << theSmallTree.m_IdAndIsoSF_MVA << endl;
        cout << "    w/ FakeRate: " << theSmallTree.m_IdAndIsoAndFakeSF_MVA << endl;
        cout << "  IDandISO deep: " << theSmallTree.m_IdAndIsoSF_deep << endl;
        cout << "    w/ FakeRate: " << theSmallTree.m_IdAndIsoAndFakeSF_deep << endl;
        cout << "  trig         : " << theSmallTree.m_trigSF << endl;
        cout << "  bTag         : " << theSmallTree.m_bTagweightM << endl;
        cout << "  prescale     : " << theSmallTree.m_prescaleWeight<< endl;
        cout << "  prefiring    : " << theSmallTree.m_L1pref_weight<< endl;
        cout << "--------------" << endl;
      }

      if (DEBUG) //FRA DEBUG
      {
          cout << "--- DEBUG MC weights ---" << endl;
          cout << "aMCatNLOweight: " << theBigTree.aMCatNLOweight << endl;
          cout << "XS            : " << XS << endl;
          cout << "stitchWeight  : " << stitchWeight << endl;
          cout << "HHweight      : " << HHweight << endl;
          cout << "MC_weight     : " << theSmallTree.m_MC_weight << endl;
          cout << "Yield weight MVA : " << theSmallTree.m_MC_weight * theSmallTree.m_PUReweight * theSmallTree.m_IdAndIsoAndFakeSF_MVA * theSmallTree.m_trigSF << endl;
          cout << "Yield weight deep: " << theSmallTree.m_MC_weight * theSmallTree.m_PUReweight * theSmallTree.m_IdAndIsoAndFakeSF_deep * theSmallTree.m_trigSF << endl;
          cout << "------------------------" << endl;
      }

      if (DEBUG) //FRA DEBUG
      {
          cout << "--- FINAL DEBUG ---" << endl;
          cout << "nbjetscand: " << theSmallTree.m_nbjetscand << endl;
          cout << "-------------------" << endl;
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
          //{ "v0", "kl1_c2v1_c31" },
          //{ "v0", "kl1_c2v1_c31_vbfbsm" }
          { "v1", "kl1_c2v1_c31" },
          { "v2", "kl1_c2v1_c31" }
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
