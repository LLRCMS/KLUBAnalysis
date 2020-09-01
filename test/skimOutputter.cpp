// ntuple skimmer for analysis and synchronization

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <bitset>
#include <map>
#include <chrono>
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TFile.h"
#include "TBranch.h"
#include "TString.h"
#include "TLorentzVector.h"
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

// bigTree is produced on an existing ntuple as follows (see at the end of the file) 
#include "smallTree.h"
#include "OfflineProducerHelper.h"
#include "../../HHKinFit2/include/HHKinFitMasterHeavyHiggs.h"
#include "BDTfunctionsUtils.h"
#include "skimUtils.h"

#include "lester_mt2_bisect.h"

#include "SVfitKLUBinterface.h"
#include "DNNKLUBinterface.h"
#include "BDTKLUBinterface.h"
#include "../src/MulticlassInterface.cc"

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

using namespace std ;
using namespace std::chrono;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -

// Main
int main (int argc, char** argv)
{
  // Usage printout
  if (argc < 4)
  {
    cerr << "missing input parameters : argc is: " << argc << endl;
    cerr << "usage: " << argv[0]
         << " inputFileNameList outputFileName configFile isData" << endl;
    return 1;
  }

  // Input and output files
  TString inputFileName  = argv[1] ;
  TString outputFileName = argv[2] ;
  cout << "** INFO: inputFile  : " << inputFileName << endl;
  cout << "** INFO: outputFile : " << outputFileName << endl;

  // Read options from config
  if (gConfigParser) return 1;
  gConfigParser = new ConfigParser();
  TString config;
  config.Form ("%s",argv[3]) ;
  cout << "** INFO: reading config : " << config << endl;

  // Read isData
  bool isData = false;
  int isDataI = atoi (argv[4]);
  if (isDataI == 1) isData = true;
  cout << "** INFO: isData : " << isData << endl;

  if (!(gConfigParser->init(config)))
  {
    cout << ">>> parseConfigFile::Could not open configuration file " << config << endl ;
    return -1 ;
  }

  // Max events to be analyzed, read from config
  int nMaxEvts = gConfigParser->readIntOption("outPutter::nMaxEvts");

  // Read bools from config
  bool doMT2    = gConfigParser->readBoolOption("outPutter::doMT2"   );
  bool doKinFit = gConfigParser->readBoolOption("outPutter::doKinFit");
  bool doSVfit  = gConfigParser->readBoolOption("outPutter::doSVfit" );
  bool doDNN    = gConfigParser->readBoolOption("outPutter::doDNN"   );
  bool doBDT    = gConfigParser->readBoolOption("outPutter::doBDT"   );
  bool doMult   = gConfigParser->readBoolOption("outPutter::doMult"   );
  cout << "** INFO: doMT2    : " << doMT2    << endl;
  cout << "** INFO: doKinFit : " << doKinFit << endl;
  cout << "** INFO: doSVfit  : " << doSVfit  << endl;
  cout << "** INFO: doDNN    : " << doDNN    << endl;
  cout << "** INFO: doBDT    : " << doBDT    << endl;
  cout << "** INFO: doMult   : " << doMult    << endl;

  bool doNominal  = gConfigParser->readBoolOption("outPutter::doNominal" );
  bool doMES      = gConfigParser->readBoolOption("outPutter::doMES"     );
  bool doEES      = gConfigParser->readBoolOption("outPutter::doEES"     );
  bool doTES      = gConfigParser->readBoolOption("outPutter::doTES"     );
  bool doSplitJES = gConfigParser->readBoolOption("outPutter::doSplitJES");
  bool doTotalJES = gConfigParser->readBoolOption("outPutter::doTotalJES");
  cout << "** INFO: doNominal  : " << doNominal  << endl;
  cout << "** INFO: doMES      : " << doMES      << endl;
  cout << "** INFO: doEES      : " << doEES      << endl;
  cout << "** INFO: doTES      : " << doTES      << endl;
  cout << "** INFO: doSplitJES : " << doSplitJES << endl;
  cout << "** INFO: doTotalJES : " << doTotalJES << endl;

  // General settings, read from configs
  int YEAR = gConfigParser->readIntOption("outPutter::year");
  cout << "** INFO: year          : " << YEAR << endl;

  int analysis_type =  gConfigParser->readIntOption("outPutter::analysis");
  cout << "** INFO: analysis type : " << analysis_type << " (0:radion, 1:graviton, 2:nonres)" << endl;


  // Input setup
  TFile *inputFile = new TFile (inputFileName, "read") ;

  // Efficiency histograms
  TH1F* h_eff        = (TH1F*)inputFile->Get("h_eff");
  TH1F* h_effSummary = (TH1F*)inputFile->Get("h_effSummary");

  // Systematics histogram
  TH1F* h_syst = (TH1F*)inputFile->Get("h_syst");
  const int N_jecSources = h_syst->GetBinContent(1); //jec sources
  const int N_tauhDM     = h_syst->GetBinContent(2); //tauh DMs
  const int N_tauhDM_EES = h_syst->GetBinContent(3); //tauh DMs with EES
  std::cout << "** ANALYSIS: N_jecSources: " << N_jecSources << std::endl;
  std::cout << "** ANALYSIS: N_tauhDM    : " << N_tauhDM << std::endl;
  std::cout << "** ANALYSIS: N_tauhDM_EES: " << N_tauhDM_EES << std::endl;

  // Input TTree
  TTree *inputChain = (TTree*)inputFile->Get("HTauTauTree");
  Long64_t nentries = inputChain->GetEntries();
  cout << "** ANALYSIS: Initial entries: " << nentries << endl;

  // Declare the only two branches needed for skimming the initial entries 
  int nleps, pairType, nbjetscand, dau1_deepTauVsJet, dau1_eleMVAiso;
  float dau1_iso;
  inputChain->SetBranchAddress("nleps", &nleps);
  inputChain->SetBranchAddress("pairType", &pairType);
  inputChain->SetBranchAddress("nbjetscand", &nbjetscand);
  inputChain->SetBranchAddress("dau1_deepTauVsJet", &dau1_deepTauVsJet);
  inputChain->SetBranchAddress("dau1_iso", &dau1_iso);
  inputChain->SetBranchAddress("dau1_eleMVAiso", &dau1_eleMVAiso);

  // De-activate all branches
  inputChain->SetBranchStatus("*", 0);

  // Define branches to be activated
  std::vector<std::string> toBeActivated {
  "EventNumber", "RunNumber","nleps","pairType","nbjetscand",          // General
  "isOS","isBoosted","isTau1real","isTau2real",

  "MC_weight","PUReweight","PUjetID_SF","L1pref_weight*",               // Weights and SFs
  "prescaleWeight","trigSF","trigSF_DM*","VBFtrigSF",
  "DYscale_MTT","DYscale_MH","bTagweightM","bTagweightL",
  "IdAndIsoAndFakeSF_deep","IdAndIsoAndFakeSF_deep_pt",
  "TTtopPtreweight*","idAndIsoAndFakeSF_tauid*",
  "idAndIsoAndFakeSF_mutauFR*","idAndIsoAndFakeSF_etauFR*",

  "isVBFtrigger", "isVBF",                                             // Trigger vbf selection

  "dau1_deepTauVsJet","dau2_deepTauVsJet",                             // Isolation
  "dau1_iso", "dau2_iso", "dau1_eleMVAiso",

  "dau1_decayMode", "dau2_decayMode",                                  // Tau decay mode

  "bjet1_bID_deepFlavor", "bjet2_bID_deepFlavor",                      // b-tagging          
  "bjet1_bID_deepCSV", "bjet2_bID_deepCSV",
  "bjet1_Cvs*","bjet2_Cvs*","VBFjet1_Cvs*","VBFjet2_Cvs*",
  "VBFjet1_btag_deepFlavor","VBFjet2_btag_deepFlavor",

  "bjet1_HHbtag","bjet2_HHbtag","VBFjet1_HHbtag","VBFjet2_HHbtag",     // HHbtag

  "dau1_pt","dau1_eta","dau1_phi","dau1_e",                            // Tau central
  "dau2_pt","dau2_eta","dau2_phi","dau2_e",
  "ditau_deltaR",

  "dau1_pt_muup","dau1_pt_mudown",                                     // Tau MES
  "dau1_mass_muup", "dau1_mass_mudown",
  "dau2_pt_muup","dau2_pt_mudown",
  "dau2_mass_muup", "dau2_mass_mudown",

  "dau1_pt_eleup*","dau1_mass_eleup",                                  // Tau EES
  "dau1_pt_eledown*","dau1_mass_eledown",
  "dau2_pt_eleup*","dau2_mass_eleup",
  "dau2_pt_eledown*","dau2_mass_eledown",

  "dau1_pt_tauup*","dau1_pt_taudown*",                                 // Tau TES
  "dau1_mass_tauup","dau1_mass_taudown",
  "dau2_pt_tauup*","dau2_pt_taudown*",
  "dau2_mass_tauup","dau2_mass_taudown",

  "bjet1_pt","bjet1_eta","bjet1_phi","bjet1_e","bjet1_JER",            // b-jets
  "bjet2_pt","bjet2_eta","bjet2_phi","bjet2_e","bjet2_JER",
  "bH_mass_raw","fatjet_softdropMass",

  "bjet1_pt_raw_jetup*","bjet1_pt_raw_jetdown*",                       // b-jets JES
  "bjet1_mass_raw_jetup","bjet1_mass_raw_jetdown",
  "bjet1_JER_jetup","bjet1_JER_jetdown",
  "bjet2_pt_raw_jetup*","bjet2_pt_raw_jetdown*",
  "bjet2_mass_raw_jetup","bjet2_mass_raw_jetdown",
  "bjet2_JER_jetup","bjet2_JER_jetdown",

  "bjet1_pt_raw_jetupTot","bjet1_pt_raw_jetdownTot",                   // b-jets JES Total
  "bjet1_mass_raw_jetupTot","bjet1_mass_raw_jetdownTot",
  "bjet1_JER_jetupTot","bjet1_JER_jetdownTot",
  "bjet2_pt_raw_jetupTot","bjet2_pt_raw_jetdownTot",
  "bjet2_mass_raw_jetupTot","bjet2_mass_raw_jetdownTot",
  "bjet2_JER_jetupTot","bjet2_JER_jetdownTot",

  "bH_mass_raw_jetup*","bH_mass_raw_jetdown*",                         // bH masses JES

  "bH_mass_raw_jetupTot","bH_mass_raw_jetdownTot",                     // bH masses JES Total

  "VBFjet1_pt","VBFjet1_eta","VBFjet1_phi","VBFjet1_e",                // VBF-jets
  "VBFjet2_pt","VBFjet2_eta","VBFjet2_phi","VBFjet2_e",

  "VBFjet1_pt_jetup*","VBFjet1_pt_jetdown*",                           // VBF-jets JES
  "VBFjet1_mass_jetup","VBFjet1_mass_jetdown",
  "VBFjet2_pt_jetup*","VBFjet2_pt_jetdown*",
  "VBFjet2_mass_jetup","VBFjet2_mass_jetdown",

  "VBFjet1_pt_jetupTot","VBFjet1_pt_jetdownTot",                       // VBF-jets JES Total
  "VBFjet1_mass_jetupTot","VBFjet1_mass_jetdownTot",
  "VBFjet2_pt_jetupTot","VBFjet2_pt_jetdownTot",
  "VBFjet2_mass_jetupTot","VBFjet2_mass_jetdownTot",

  "VBFjj_mass_jetup*","VBFjj_mass_jetdown*",                           // VBFjj mass JES

  "VBFjj_mass_jetupTot","VBFjj_mass_jetdownTot",                       // VBFjj mass JES Total

  "met_phi","met_et","METx","METy",                                    // MET
  "met_cov00","met_cov01","met_cov10","met_cov11",

  "MET*_muup"    ,"MET*_mudown",                                       // MET MES
  "MET*_eleup"   ,"MET*_eledown",                                      // MET EES
  "MET*_tauup"   ,"MET*_taudown",                                      // MET TES
  "MET*_jetup"   ,"MET*_jetdown",                                      // MET JES
  "MET*_jetupTot","MET*_jetdownTot",                                   // MET JES Total

  "VBFjj_mass", "VBFjj_deltaEta",                                      // VBF selection

  "BDT_HT20",                                                          // BDT_HT20
  "BDT_HT20_jetup","BDT_HT20_jetdown",                                 // BDT_HT20 JES
  "BDT_HT20_jetupTot","BDT_HT20_jetdownTot",                           // BDT_HT20 JES Total

  "HHKin_mass","HHKin_chi2", "MT2",                                    // Old values KinFit, MT2, SVfit, DNN, BDT
  "tauH_SVFIT_pt","tauH_SVFIT_eta","tauH_SVFIT_phi","tauH_SVFIT_mass",
  "DNNoutSM_kl_1","BDToutSM_kl_1","mdnn*"
  };

  // Activate only branches I need/want to store
  for (auto activeBranchName : toBeActivated) 
  {
    inputChain->SetBranchStatus(activeBranchName.c_str(), 1);
  }

  //Create a new file + a clone of old tree in new file
  TFile *cloneFile = new TFile (outputFileName, "recreate") ;
  TTree *treenew = inputChain->CloneTree(0);

  // Loop on input events to apply minimal selection
  cout << "** ANALYSIS: Cloning with minimal selection..." << endl;
  for (Long64_t iEvent = 0 ; iEvent<nentries ; ++iEvent) 
  {
    if (iEvent % 500 == 0)  cout << "  - reading event " << iEvent << endl ;
    if (iEvent == nMaxEvts ) break;

    inputChain->GetEntry(iEvent);
    if ( nleps!=0 || nbjetscand<2 || pairType>2) continue;
    if ( pairType==0 && dau1_iso>=0.15 ) continue;
    if ( pairType==1 && dau1_eleMVAiso!=1 ) continue;
    if ( pairType==2 && dau1_deepTauVsJet<5 ) continue;
    treenew->Fill() ;
  }
  cout << "** ANALYSIS: ...Cloned entries: " << treenew->GetEntries() << endl;

  // Save and close cloneFile and inputFile
  cloneFile->cd();
  h_eff->Write();
  h_effSummary->Write();
  h_syst->Write();
  treenew->Write();
  cloneFile->Write();
  cloneFile->Close();
  inputFile->Close();

  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  // Now that we have cloned the TTree with a selection,
  // we re-open it to update it with new branches
  // Open file and get TTree that must be updated
  TFile *outFile = TFile::Open(outputFileName,"UPDATE");
  TTree *outTree = (TTree*)outFile->Get("HTauTauTree");

  // for HHKinFit
  int hypo_mh1 = 125;
  int hypo_mh2 = 125;

  // for MT2
  asymm_mt2_lester_bisect::disableCopyrightMessage();
  double desiredPrecisionOnMt2 = 0; // Must be >=0.  If 0 alg aims for machine precision.  if >0, MT2 computed to supplied absolute precision.

  // Target kls for DNN and BDT
  std::vector<float> target_kls;
  target_kls = gConfigParser->readFloatListOption("outPutter::kl");

  // Declare DNNKLUBinterface
  std::string model_dir     = gConfigParser->readStringOption("DNN::weights" );
  std::string features_file = gConfigParser->readStringOption("DNN::features");
  std::vector<std::string> requested;
  std::ifstream features_infile(features_file);
  std::string featureName;
  while ( features_infile >> featureName)
  {
    requested.push_back(featureName);
  }
  features_infile.close();
  DNNKLUBinterface DNNreader(model_dir, requested, target_kls);

  Year DNNyear;
  if      (YEAR == 2016)
  {
    DNNyear = y16;
  }
  else if (YEAR == 2017)
  {
    DNNyear = y17;
  }
  else  /*YEAR == 2018*/
  {
    DNNyear = y18;
  }

  Spin DNNanalysis;
  if      (analysis_type == 0)
  {
    DNNanalysis = radion;
  }
  else if (analysis_type == 1)
  {
    DNNanalysis = graviton;
  }
  else /* analysis_type == 2 */
  {
    DNNanalysis = nonres;
  }
  DNNreader.SetGlobalInputs(DNNyear, DNNanalysis);

  // Declare BDTKLUBinterface
  std::string weights = gConfigParser->readStringOption("BDTsm::weights");
  BDTKLUBinterface BDTreader(weights, target_kls);

  // Declare MulticlassInterface
  std::vector<std::pair<std::string, std::string>> modelSpecs = {
    { "v0", "kl1_c2v1_c31" },
    { "v1", "kl1_c2v1_c31" },
    { "v0", "kl1_c2v1_c31_vbfbsm" }
  };
  MulticlassInterface mci(YEAR, modelSpecs);
  mci.clearInputs();
  int mdnnSM0_size  = (mci.getNodeNames(0)).size();
  int mdnnSM1_size  = (mci.getNodeNames(1)).size();
  int mdnnBSM0_size = (mci.getNodeNames(2)).size();

  // Read branches needed for computation of KinFit, MT2, SVfit, BDT, DNN
  ULong64_t EventNumber;
  Int_t DM1, DM2, pType, isBoosted, isVBF;
  Float_t dau1_pt, dau1_eta, dau1_phi, dau1_e, dau2_pt, dau2_eta, dau2_phi, dau2_e;
  Float_t dau1_pt_muup, dau1_pt_mudown, dau1_mass_muup, dau1_mass_mudown;
  Float_t dau2_pt_muup, dau2_pt_mudown, dau2_mass_muup, dau2_mass_mudown;
  Float_t bjet1_pt, bjet1_eta, bjet1_phi, bjet1_e, bjet1_JER, bjet2_pt, bjet2_eta, bjet2_phi, bjet2_e, bjet2_JER;
  Float_t vbfjet1_pt, vbfjet1_eta, vbfjet1_phi, vbfjet1_e, vbfjet2_pt, vbfjet2_eta, vbfjet2_phi, vbfjet2_e;
  Float_t METx, METy, met_cov00, met_cov01, met_cov10, met_cov11;
  Float_t METx_muup, METy_muup, METx_mudown, METy_mudown;
  Float_t HHKin_mass, HHKin_chi2, MT2, tauH_SVFIT_pt, tauH_SVFIT_eta, tauH_SVFIT_phi, tauH_SVFIT_mass, DNNoutSM_kl_1, BDToutSM_kl_1;
  float bjet1_bID_deepFlavor, bjet2_bID_deepFlavor;
  float VBFjet1_btag_deepFlavor, VBFjet2_btag_deepFlavor;
  float CvsL_b1, CvsL_b2, CvsL_vbf1, CvsL_vbf2;
  float CvsB_b1, CvsB_b2, CvsB_vbf1, CvsB_vbf2;
  float HHbtag_b1, HHbtag_b2, HHbtag_vbf1, HHbtag_vbf2;
  float BDT_HT20;
  std::vector<Float_t> *METx_eleup    = 0, *METy_eleup      = 0, *METx_eledown    = 0, *METy_eledown      = 0;
  std::vector<Float_t> *dau1_pt_eleup = 0, *dau1_pt_eledown = 0, *dau1_mass_eleup = 0, *dau1_mass_eledown = 0;
  std::vector<Float_t> *dau2_pt_eleup = 0, *dau2_pt_eledown = 0, *dau2_mass_eleup = 0, *dau2_mass_eledown = 0;
  std::vector<Float_t> *METx_tauup    = 0, *METy_tauup      = 0, *METx_taudown    = 0 , *METy_taudown     = 0;
  std::vector<Float_t> *dau1_pt_tauup = 0, *dau1_pt_taudown = 0, *dau1_mass_tauup = 0, *dau1_mass_taudown = 0;
  std::vector<Float_t> *dau2_pt_tauup = 0, *dau2_pt_taudown = 0, *dau2_mass_tauup = 0, *dau2_mass_taudown = 0;

  std::vector<Float_t> *METx_jetup         = 0, *METy_jetup           = 0, *METx_jetdown         = 0, *METy_jetdown           = 0;
  std::vector<Float_t> *bjet1_pt_raw_jetup = 0, *bjet1_pt_raw_jetdown = 0, *bjet1_mass_raw_jetup = 0, *bjet1_mass_raw_jetdown = 0;
  std::vector<Float_t> *bjet2_pt_raw_jetup = 0, *bjet2_pt_raw_jetdown = 0, *bjet2_mass_raw_jetup = 0, *bjet2_mass_raw_jetdown = 0;
  std::vector<Float_t> *bjet1_JER_jetup    = 0, *bjet1_JER_jetdown    = 0;
  std::vector<Float_t> *bjet2_JER_jetup    = 0, *bjet2_JER_jetdown    = 0;
  std::vector<Float_t> *VBFjet1_pt_jetup   = 0, *VBFjet1_pt_jetdown   = 0, *VBFjet1_mass_jetup   = 0, *VBFjet1_mass_jetdown   = 0;
  std::vector<Float_t> *VBFjet2_pt_jetup   = 0, *VBFjet2_pt_jetdown   = 0, *VBFjet2_mass_jetup   = 0, *VBFjet2_mass_jetdown   = 0;
  std::vector<Float_t> *BDT_HT20_jetup     = 0, *BDT_HT20_jetdown     = 0;

  Float_t METx_jetupTot, METy_jetupTot, METx_jetdownTot, METy_jetdownTot;
  Float_t bjet1_pt_raw_jetupTot, bjet1_pt_raw_jetdownTot, bjet1_mass_raw_jetupTot, bjet1_mass_raw_jetdownTot;
  Float_t bjet2_pt_raw_jetupTot, bjet2_pt_raw_jetdownTot, bjet2_mass_raw_jetupTot, bjet2_mass_raw_jetdownTot;
  Float_t bjet1_JER_jetupTot, bjet1_JER_jetdownTot;
  Float_t bjet2_JER_jetupTot, bjet2_JER_jetdownTot;
  Float_t VBFjet1_pt_jetupTot, VBFjet1_pt_jetdownTot, VBFjet1_mass_jetupTot, VBFjet1_mass_jetdownTot;
  Float_t VBFjet2_pt_jetupTot, VBFjet2_pt_jetdownTot, VBFjet2_mass_jetupTot, VBFjet2_mass_jetdownTot;
  Float_t BDT_HT20_jetupTot, BDT_HT20_jetdownTot;

  outTree->SetBranchAddress("EventNumber" , &EventNumber);
  outTree->SetBranchAddress("isBoosted"   , &isBoosted);
  outTree->SetBranchAddress("isVBF"       , &isVBF);

  outTree->SetBranchAddress("dau1_pt" , &dau1_pt);
  outTree->SetBranchAddress("dau1_eta", &dau1_eta);
  outTree->SetBranchAddress("dau1_phi", &dau1_phi);
  outTree->SetBranchAddress("dau1_e"  , &dau1_e);
  outTree->SetBranchAddress("dau2_pt" , &dau2_pt);
  outTree->SetBranchAddress("dau2_eta", &dau2_eta);
  outTree->SetBranchAddress("dau2_phi", &dau2_phi);
  outTree->SetBranchAddress("dau2_e"  , &dau2_e);

  outTree->SetBranchAddress("dau1_decayMode", &DM1);
  outTree->SetBranchAddress("dau2_decayMode", &DM2);
  outTree->SetBranchAddress("pairType"      , &pType);

  outTree->SetBranchAddress("dau1_pt_muup"    , &dau1_pt_muup);
  outTree->SetBranchAddress("dau1_pt_mudown"  , &dau1_pt_mudown);
  outTree->SetBranchAddress("dau1_mass_muup"  , &dau1_mass_muup);
  outTree->SetBranchAddress("dau1_mass_mudown", &dau1_mass_mudown);
  outTree->SetBranchAddress("dau2_pt_muup"    , &dau2_pt_muup);
  outTree->SetBranchAddress("dau2_pt_mudown"  , &dau2_pt_mudown);
  outTree->SetBranchAddress("dau2_mass_muup"  , &dau2_mass_muup);
  outTree->SetBranchAddress("dau2_mass_mudown", &dau2_mass_mudown);
  outTree->SetBranchAddress("dau1_pt_eleup"    , &dau1_pt_eleup);
  outTree->SetBranchAddress("dau1_pt_eledown"  , &dau1_pt_eledown);
  outTree->SetBranchAddress("dau1_mass_eleup"  , &dau1_mass_eleup);
  outTree->SetBranchAddress("dau1_mass_eledown", &dau1_mass_eledown);
  outTree->SetBranchAddress("dau2_pt_eleup"    , &dau2_pt_eleup);
  outTree->SetBranchAddress("dau2_pt_eledown"  , &dau2_pt_eledown);
  outTree->SetBranchAddress("dau2_mass_eleup"  , &dau2_mass_eleup);
  outTree->SetBranchAddress("dau2_mass_eledown", &dau2_mass_eledown);
  outTree->SetBranchAddress("dau1_pt_tauup"    , &dau1_pt_tauup);
  outTree->SetBranchAddress("dau1_pt_taudown"  , &dau1_pt_taudown);
  outTree->SetBranchAddress("dau1_mass_tauup"  , &dau1_mass_tauup);
  outTree->SetBranchAddress("dau1_mass_taudown", &dau1_mass_taudown);
  outTree->SetBranchAddress("dau2_pt_tauup"    , &dau2_pt_tauup);
  outTree->SetBranchAddress("dau2_pt_taudown"  , &dau2_pt_taudown);
  outTree->SetBranchAddress("dau2_mass_tauup"  , &dau2_mass_tauup);
  outTree->SetBranchAddress("dau2_mass_taudown", &dau2_mass_taudown);

  outTree->SetBranchAddress("bjet1_pt" , &bjet1_pt);
  outTree->SetBranchAddress("bjet1_eta", &bjet1_eta);
  outTree->SetBranchAddress("bjet1_phi", &bjet1_phi);
  outTree->SetBranchAddress("bjet1_e"  , &bjet1_e);
  outTree->SetBranchAddress("bjet1_JER", &bjet1_JER);
  outTree->SetBranchAddress("bjet2_pt" , &bjet2_pt);
  outTree->SetBranchAddress("bjet2_eta", &bjet2_eta);
  outTree->SetBranchAddress("bjet2_phi", &bjet2_phi);
  outTree->SetBranchAddress("bjet2_e"  , &bjet2_e);
  outTree->SetBranchAddress("bjet2_JER", &bjet2_JER);

  outTree->SetBranchAddress("bjet1_pt_raw_jetup"     , &bjet1_pt_raw_jetup);
  outTree->SetBranchAddress("bjet1_pt_raw_jetdown"   , &bjet1_pt_raw_jetdown);
  outTree->SetBranchAddress("bjet1_mass_raw_jetup"   , &bjet1_mass_raw_jetup);
  outTree->SetBranchAddress("bjet1_mass_raw_jetdown" , &bjet1_mass_raw_jetdown);
  outTree->SetBranchAddress("bjet2_pt_raw_jetup"     , &bjet2_pt_raw_jetup);
  outTree->SetBranchAddress("bjet2_pt_raw_jetdown"   , &bjet2_pt_raw_jetdown);
  outTree->SetBranchAddress("bjet2_mass_raw_jetup"   , &bjet2_mass_raw_jetup);
  outTree->SetBranchAddress("bjet2_mass_raw_jetdown" , &bjet2_mass_raw_jetdown);
  outTree->SetBranchAddress("bjet1_JER_jetup"        , &bjet1_JER_jetup);
  outTree->SetBranchAddress("bjet1_JER_jetdown"      , &bjet1_JER_jetdown);
  outTree->SetBranchAddress("bjet2_JER_jetup"        , &bjet2_JER_jetup);
  outTree->SetBranchAddress("bjet2_JER_jetdown"      , &bjet2_JER_jetdown);

  outTree->SetBranchAddress("bjet1_pt_raw_jetupTot"     , &bjet1_pt_raw_jetupTot);
  outTree->SetBranchAddress("bjet1_pt_raw_jetdownTot"   , &bjet1_pt_raw_jetdownTot);
  outTree->SetBranchAddress("bjet1_mass_raw_jetupTot"   , &bjet1_mass_raw_jetupTot);
  outTree->SetBranchAddress("bjet1_mass_raw_jetdownTot" , &bjet1_mass_raw_jetdownTot);
  outTree->SetBranchAddress("bjet2_pt_raw_jetupTot"     , &bjet2_pt_raw_jetupTot);
  outTree->SetBranchAddress("bjet2_pt_raw_jetdownTot"   , &bjet2_pt_raw_jetdownTot);
  outTree->SetBranchAddress("bjet2_mass_raw_jetupTot"   , &bjet2_mass_raw_jetupTot);
  outTree->SetBranchAddress("bjet2_mass_raw_jetdownTot" , &bjet2_mass_raw_jetdownTot);
  outTree->SetBranchAddress("bjet1_JER_jetupTot"        , &bjet1_JER_jetupTot);
  outTree->SetBranchAddress("bjet1_JER_jetdownTot"      , &bjet1_JER_jetdownTot);
  outTree->SetBranchAddress("bjet2_JER_jetupTot"        , &bjet2_JER_jetupTot);
  outTree->SetBranchAddress("bjet2_JER_jetdownTot"      , &bjet2_JER_jetdownTot);

  outTree->SetBranchAddress("VBFjet1_pt" , &vbfjet1_pt);
  outTree->SetBranchAddress("VBFjet1_eta", &vbfjet1_eta);
  outTree->SetBranchAddress("VBFjet1_phi", &vbfjet1_phi);
  outTree->SetBranchAddress("VBFjet1_e"  , &vbfjet1_e);
  outTree->SetBranchAddress("VBFjet2_pt" , &vbfjet2_pt);
  outTree->SetBranchAddress("VBFjet2_eta", &vbfjet2_eta);
  outTree->SetBranchAddress("VBFjet2_phi", &vbfjet2_phi);
  outTree->SetBranchAddress("VBFjet2_e"  , &vbfjet2_e);

  outTree->SetBranchAddress("VBFjet1_pt_jetup"     , &VBFjet1_pt_jetup);
  outTree->SetBranchAddress("VBFjet1_pt_jetdown"   , &VBFjet1_pt_jetdown);
  outTree->SetBranchAddress("VBFjet1_mass_jetup"   , &VBFjet1_mass_jetup);
  outTree->SetBranchAddress("VBFjet1_mass_jetdown" , &VBFjet1_mass_jetdown);
  outTree->SetBranchAddress("VBFjet2_pt_jetup"     , &VBFjet2_pt_jetup);
  outTree->SetBranchAddress("VBFjet2_pt_jetdown"   , &VBFjet2_pt_jetdown);
  outTree->SetBranchAddress("VBFjet2_mass_jetup"   , &VBFjet2_mass_jetup);
  outTree->SetBranchAddress("VBFjet2_mass_jetdown" , &VBFjet2_mass_jetdown);

  outTree->SetBranchAddress("VBFjet1_pt_jetupTot"     , &VBFjet1_pt_jetupTot);
  outTree->SetBranchAddress("VBFjet1_pt_jetdownTot"   , &VBFjet1_pt_jetdownTot);
  outTree->SetBranchAddress("VBFjet1_mass_jetupTot"   , &VBFjet1_mass_jetupTot);
  outTree->SetBranchAddress("VBFjet1_mass_jetdownTot" , &VBFjet1_mass_jetdownTot);
  outTree->SetBranchAddress("VBFjet2_pt_jetupTot"     , &VBFjet2_pt_jetupTot);
  outTree->SetBranchAddress("VBFjet2_pt_jetdownTot"   , &VBFjet2_pt_jetdownTot);
  outTree->SetBranchAddress("VBFjet2_mass_jetupTot"   , &VBFjet2_mass_jetupTot);
  outTree->SetBranchAddress("VBFjet2_mass_jetdownTot" , &VBFjet2_mass_jetdownTot);

  outTree->SetBranchAddress("bjet1_bID_deepFlavor" , &bjet1_bID_deepFlavor);
  outTree->SetBranchAddress("bjet2_bID_deepFlavor" , &bjet2_bID_deepFlavor);
  outTree->SetBranchAddress("VBFjet1_btag_deepFlavor" , &VBFjet1_btag_deepFlavor);
  outTree->SetBranchAddress("VBFjet2_btag_deepFlavor" , &VBFjet2_btag_deepFlavor);
  outTree->SetBranchAddress("bjet1_CvsL"    , &CvsL_b1);
  outTree->SetBranchAddress("bjet2_CvsL"    , &CvsL_b2);
  outTree->SetBranchAddress("VBFjet1_CvsL"  , &CvsL_vbf1);
  outTree->SetBranchAddress("VBFjet2_CvsL"  , &CvsL_vbf2);
  outTree->SetBranchAddress("bjet1_CvsB"    , &CvsB_b1);
  outTree->SetBranchAddress("bjet2_CvsB"    , &CvsB_b2);
  outTree->SetBranchAddress("VBFjet1_CvsB"  , &CvsB_vbf1);
  outTree->SetBranchAddress("VBFjet2_CvsB"  , &CvsB_vbf2);
  outTree->SetBranchAddress("bjet1_HHbtag"  , &HHbtag_b1);
  outTree->SetBranchAddress("bjet2_HHbtag"  , &HHbtag_b2);
  outTree->SetBranchAddress("VBFjet1_HHbtag", &HHbtag_vbf1);
  outTree->SetBranchAddress("VBFjet2_HHbtag", &HHbtag_vbf2);

  outTree->SetBranchAddress("met_cov00" , &met_cov00);
  outTree->SetBranchAddress("met_cov01" , &met_cov01);
  outTree->SetBranchAddress("met_cov10" , &met_cov10);
  outTree->SetBranchAddress("met_cov11" , &met_cov11);

  outTree->SetBranchAddress("METx"       , &METx);
  outTree->SetBranchAddress("METy"       , &METy);
  outTree->SetBranchAddress("METx_muup"  , &METx_muup);
  outTree->SetBranchAddress("METy_muup"  , &METy_muup);
  outTree->SetBranchAddress("METx_mudown", &METx_mudown);
  outTree->SetBranchAddress("METy_mudown", &METy_mudown);
  outTree->SetBranchAddress("METx_eleup"  , &METx_eleup);
  outTree->SetBranchAddress("METy_eleup"  , &METy_eleup);
  outTree->SetBranchAddress("METx_eledown", &METx_eledown);
  outTree->SetBranchAddress("METy_eledown", &METy_eledown);
  outTree->SetBranchAddress("METx_tauup"  , &METx_tauup);
  outTree->SetBranchAddress("METy_tauup"  , &METy_tauup);
  outTree->SetBranchAddress("METx_taudown", &METx_taudown);
  outTree->SetBranchAddress("METy_taudown", &METy_taudown);
  outTree->SetBranchAddress("METx_jetup"  , &METx_jetup);
  outTree->SetBranchAddress("METy_jetup"  , &METy_jetup);
  outTree->SetBranchAddress("METx_jetdown", &METx_jetdown);
  outTree->SetBranchAddress("METy_jetdown", &METy_jetdown);
  outTree->SetBranchAddress("METx_jetupTot"  , &METx_jetupTot);
  outTree->SetBranchAddress("METy_jetupTot"  , &METy_jetupTot);
  outTree->SetBranchAddress("METx_jetdownTot", &METx_jetdownTot);
  outTree->SetBranchAddress("METy_jetdownTot", &METy_jetdownTot);

  outTree->SetBranchAddress("BDT_HT20"        , &BDT_HT20);
  outTree->SetBranchAddress("BDT_HT20_jetup"  , &BDT_HT20_jetup);
  outTree->SetBranchAddress("BDT_HT20_jetdown", &BDT_HT20_jetdown);
  outTree->SetBranchAddress("BDT_HT20_jetupTot"  , &BDT_HT20_jetupTot);
  outTree->SetBranchAddress("BDT_HT20_jetdownTot", &BDT_HT20_jetdownTot);

  outTree->SetBranchAddress("MT2"            , &MT2);
  outTree->SetBranchAddress("HHKin_mass"     , &HHKin_mass);
  outTree->SetBranchAddress("HHKin_chi2"     , &HHKin_chi2);
  outTree->SetBranchAddress("tauH_SVFIT_pt"  , &tauH_SVFIT_pt);
  outTree->SetBranchAddress("tauH_SVFIT_eta" , &tauH_SVFIT_eta);
  outTree->SetBranchAddress("tauH_SVFIT_phi" , &tauH_SVFIT_phi);
  outTree->SetBranchAddress("tauH_SVFIT_mass", &tauH_SVFIT_mass);
  outTree->SetBranchAddress("DNNoutSM_kl_1"  , &DNNoutSM_kl_1);
  outTree->SetBranchAddress("BDToutSM_kl_1"  , &BDToutSM_kl_1);

  // Declare new branches
  // Central values
  Float_t HHKin_mass_new, HHKin_chi2_new;
  Float_t MT2_new;
  Float_t tauH_SVFIT_pt_new, tauH_SVFIT_eta_new, tauH_SVFIT_phi_new, tauH_SVFIT_mass_new;
  Float_t DNNoutSM_kl_1_new, BDToutSM_kl_1_new;   // FIXME: read from cfg file
  std::vector<Float_t> mdnnSM0_output_new(mdnnSM0_size);
  std::vector<Float_t> mdnnSM1_output_new(mdnnSM1_size);
  std::vector<Float_t> mdnnBSM0_output_new(mdnnBSM0_size);
  TBranch* b_HHKin_mass_new      = outTree->Branch("HHKin_mass_new"  , &HHKin_mass_new);
  TBranch* b_HHKin_chi2_new      = outTree->Branch("HHKin_chi2_new"  , &HHKin_chi2_new);
  TBranch* b_MT2_new             = outTree->Branch("MT2_new"  , &MT2_new);
  TBranch* b_DNNoutSM_kl_1_new   = outTree->Branch("DNNoutSM_kl_1_new"  , &DNNoutSM_kl_1_new);
  TBranch* b_BDToutSM_kl_1_new   = outTree->Branch("BDToutSM_kl_1_new"  , &BDToutSM_kl_1_new);
  TBranch* b_tauH_SVFIT_pt_new   = outTree->Branch("tauH_SVFIT_pt_new"  , &tauH_SVFIT_pt_new);
  TBranch* b_tauH_SVFIT_eta_new  = outTree->Branch("tauH_SVFIT_eta_new" , &tauH_SVFIT_eta_new);
  TBranch* b_tauH_SVFIT_phi_new  = outTree->Branch("tauH_SVFIT_phi_new" , &tauH_SVFIT_phi_new);
  TBranch* b_tauH_SVFIT_mass_new = outTree->Branch("tauH_SVFIT_mass_new", &tauH_SVFIT_mass_new);
  std::vector<TBranch*> b_mdnnSM0_new, b_mdnnSM1_new, b_mdnnBSM0_new;
  boost::format mdnnSM0name_new  ("mdnn__v0__kl1_c2v1_c31__%1%_new");
  boost::format mdnnSM1name_new  ("mdnn__v1__kl1_c2v1_c31__%1%_new");
  boost::format mdnnBSM0name_new ("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_new");
  for (int i=0; i<mdnnSM0_size; i++)
  {
    std::string tmp_mdnnSM0_branch_name = boost::str( mdnnSM0name_new % (mci.getNodeNames(0)).at(i) );
    TBranch* tmp_mdnnSM0_branch = outTree->Branch(tmp_mdnnSM0_branch_name.c_str(), &mdnnSM0_output_new.at(i));
    b_mdnnSM0_new.push_back(tmp_mdnnSM0_branch);
  }
  for (int i=0; i<mdnnSM1_size; i++)
  {
    std::string tmp_mdnnSM1_branch_name = boost::str( mdnnSM1name_new % (mci.getNodeNames(1)).at(i) );
    TBranch* tmp_mdnnSM1_branch = outTree->Branch(tmp_mdnnSM1_branch_name.c_str(), &mdnnSM1_output_new.at(i));
    b_mdnnSM1_new.push_back(tmp_mdnnSM1_branch);
  }
  for (int i=0; i<mdnnBSM0_size; i++)
  {
    std::string tmp_mdnnBSM0_branch_name = boost::str( mdnnBSM0name_new % (mci.getNodeNames(2)).at(i) );
    TBranch* tmp_mdnnBSM0_branch = outTree->Branch(tmp_mdnnBSM0_branch_name.c_str(), &mdnnBSM0_output_new.at(i));
    b_mdnnBSM0_new.push_back(tmp_mdnnBSM0_branch);
  }

  // MES variations
  Float_t tauH_SVFIT_mass_muup, DNNoutSM_kl_1_muup, BDToutSM_kl_1_muup;
  Float_t tauH_SVFIT_mass_mudown, DNNoutSM_kl_1_mudown, BDToutSM_kl_1_mudown;
  std::vector<Float_t> mdnnSM0_output_muup (mdnnSM0_size) , mdnnSM0_output_mudown (mdnnSM0_size);
  std::vector<Float_t> mdnnSM1_output_muup (mdnnSM1_size) , mdnnSM1_output_mudown (mdnnSM1_size);
  std::vector<Float_t> mdnnBSM0_output_muup(mdnnBSM0_size), mdnnBSM0_output_mudown(mdnnBSM0_size);
  TBranch* b_tauH_SVFIT_mass_muup   = outTree->Branch("tauH_SVFIT_mass_muup"  , &tauH_SVFIT_mass_muup);
  TBranch* b_DNNoutSM_kl_1_muup     = outTree->Branch("DNNoutSM_kl_1_muup"    , &DNNoutSM_kl_1_muup);
  TBranch* b_BDToutSM_kl_1_muup     = outTree->Branch("BDToutSM_kl_1_muup"    , &BDToutSM_kl_1_muup);
  TBranch* b_tauH_SVFIT_mass_mudown = outTree->Branch("tauH_SVFIT_mass_mudown", &tauH_SVFIT_mass_mudown);
  TBranch* b_DNNoutSM_kl_1_mudown   = outTree->Branch("DNNoutSM_kl_1_mudown"  , &DNNoutSM_kl_1_mudown);
  TBranch* b_BDToutSM_kl_1_mudown   = outTree->Branch("BDToutSM_kl_1_mudown"  , &BDToutSM_kl_1_mudown);
  std::vector<TBranch*> b_mdnnSM0_muup , b_mdnnSM0_mudown;
  std::vector<TBranch*> b_mdnnSM1_muup , b_mdnnSM1_mudown;
  std::vector<TBranch*> b_mdnnBSM0_muup, b_mdnnBSM0_mudown;
  boost::format mdnnSM0name_muup   ("mdnn__v0__kl1_c2v1_c31__%1%_muup");
  boost::format mdnnSM0name_mudown ("mdnn__v0__kl1_c2v1_c31__%1%_mudown");
  boost::format mdnnSM1name_muup   ("mdnn__v1__kl1_c2v1_c31__%1%_muup");
  boost::format mdnnSM1name_mudown ("mdnn__v1__kl1_c2v1_c31__%1%_mudown");
  boost::format mdnnBSM0name_muup  ("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_muup");
  boost::format mdnnBSM0name_mudown("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_mudown");
  for (int i=0; i<mdnnSM0_size; i++)
  {
    std::string tmp_mdnnSM0_branch_name_up   = boost::str( mdnnSM0name_muup   % (mci.getNodeNames(0)).at(i) );
    std::string tmp_mdnnSM0_branch_name_down = boost::str( mdnnSM0name_mudown % (mci.getNodeNames(0)).at(i) );
    TBranch* tmp_mdnnSM0_branch_up   = outTree->Branch(tmp_mdnnSM0_branch_name_up.c_str()  , &mdnnSM0_output_muup.at(i));
    TBranch* tmp_mdnnSM0_branch_down = outTree->Branch(tmp_mdnnSM0_branch_name_down.c_str(), &mdnnSM0_output_mudown.at(i));
    b_mdnnSM0_muup  .push_back(tmp_mdnnSM0_branch_up);
    b_mdnnSM0_mudown.push_back(tmp_mdnnSM0_branch_down);
  }
  for (int i=0; i<mdnnSM1_size; i++)
  {
    std::string tmp_mdnnSM1_branch_name_up   = boost::str( mdnnSM1name_muup   % (mci.getNodeNames(1)).at(i) );
    std::string tmp_mdnnSM1_branch_name_down = boost::str( mdnnSM1name_mudown % (mci.getNodeNames(1)).at(i) );
    TBranch* tmp_mdnnSM1_branch_up   = outTree->Branch(tmp_mdnnSM1_branch_name_up.c_str()  , &mdnnSM1_output_muup.at(i));
    TBranch* tmp_mdnnSM1_branch_down = outTree->Branch(tmp_mdnnSM1_branch_name_down.c_str(), &mdnnSM1_output_mudown.at(i));
    b_mdnnSM1_muup  .push_back(tmp_mdnnSM1_branch_up);
    b_mdnnSM1_mudown.push_back(tmp_mdnnSM1_branch_down);
  }
  for (int i=0; i<mdnnBSM0_size; i++)
  {
    std::string tmp_mdnnBSM0_branch_name_up   = boost::str( mdnnBSM0name_muup   % (mci.getNodeNames(2)).at(i) );
    std::string tmp_mdnnBSM0_branch_name_down = boost::str( mdnnBSM0name_mudown % (mci.getNodeNames(2)).at(i) );
    TBranch* tmp_mdnnBSM0_branch_up   = outTree->Branch(tmp_mdnnBSM0_branch_name_up.c_str()  , &mdnnBSM0_output_muup.at(i));
    TBranch* tmp_mdnnBSM0_branch_down = outTree->Branch(tmp_mdnnBSM0_branch_name_down.c_str(), &mdnnBSM0_output_mudown.at(i));
    b_mdnnBSM0_muup  .push_back(tmp_mdnnBSM0_branch_up);
    b_mdnnBSM0_mudown.push_back(tmp_mdnnBSM0_branch_down);
  }

  // EES variations
  std::vector<Float_t> tauH_SVFIT_mass_eleup(N_tauhDM_EES), DNNoutSM_kl_1_eleup(N_tauhDM_EES), BDToutSM_kl_1_eleup(N_tauhDM_EES);
  std::vector<Float_t> tauH_SVFIT_mass_eledown(N_tauhDM_EES), DNNoutSM_kl_1_eledown(N_tauhDM_EES), BDToutSM_kl_1_eledown(N_tauhDM_EES);
  std::vector<std::vector<Float_t>> mdnnSM0_output_eleup (N_tauhDM_EES, std::vector<Float_t>(mdnnSM0_size)) , mdnnSM0_output_eledown (N_tauhDM_EES, std::vector<Float_t>(mdnnSM0_size));
  std::vector<std::vector<Float_t>> mdnnSM1_output_eleup (N_tauhDM_EES, std::vector<Float_t>(mdnnSM1_size)) , mdnnSM1_output_eledown (N_tauhDM_EES, std::vector<Float_t>(mdnnSM1_size));
  std::vector<std::vector<Float_t>> mdnnBSM0_output_eleup(N_tauhDM_EES, std::vector<Float_t>(mdnnBSM0_size)), mdnnBSM0_output_eledown(N_tauhDM_EES, std::vector<Float_t>(mdnnBSM0_size));
  TBranch* b_tauH_SVFIT_mass_eleup_DM0   = outTree->Branch("tauH_SVFIT_mass_eleup_DM0"  , &tauH_SVFIT_mass_eleup.at(0));    // DM 0
  TBranch* b_DNNoutSM_kl_1_eleup_DM0     = outTree->Branch("DNNoutSM_kl_1_eleup_DM0"    , &DNNoutSM_kl_1_eleup.at(0));
  TBranch* b_BDToutSM_kl_1_eleup_DM0     = outTree->Branch("BDToutSM_kl_1_eleup_DM0"    , &BDToutSM_kl_1_eleup.at(0));
  TBranch* b_tauH_SVFIT_mass_eledown_DM0 = outTree->Branch("tauH_SVFIT_mass_eledown_DM0", &tauH_SVFIT_mass_eledown.at(0));
  TBranch* b_DNNoutSM_kl_1_eledown_DM0   = outTree->Branch("DNNoutSM_kl_1_eledown_DM0"  , &DNNoutSM_kl_1_eledown.at(0));
  TBranch* b_BDToutSM_kl_1_eledown_DM0   = outTree->Branch("BDToutSM_kl_1_eledown_DM0"  , &BDToutSM_kl_1_eledown.at(0));
  TBranch* b_tauH_SVFIT_mass_eleup_DM1   = outTree->Branch("tauH_SVFIT_mass_eleup_DM1"  , &tauH_SVFIT_mass_eleup.at(1));    // DM 1
  TBranch* b_DNNoutSM_kl_1_eleup_DM1     = outTree->Branch("DNNoutSM_kl_1_eleup_DM1"    , &DNNoutSM_kl_1_eleup.at(1));
  TBranch* b_BDToutSM_kl_1_eleup_DM1     = outTree->Branch("BDToutSM_kl_1_eleup_DM1"    , &BDToutSM_kl_1_eleup.at(1));
  TBranch* b_tauH_SVFIT_mass_eledown_DM1 = outTree->Branch("tauH_SVFIT_mass_eledown_DM1", &tauH_SVFIT_mass_eledown.at(1));
  TBranch* b_DNNoutSM_kl_1_eledown_DM1   = outTree->Branch("DNNoutSM_kl_1_eledown_DM1"  , &DNNoutSM_kl_1_eledown.at(1));
  TBranch* b_BDToutSM_kl_1_eledown_DM1   = outTree->Branch("BDToutSM_kl_1_eledown_DM1"  , &BDToutSM_kl_1_eledown.at(1));
  std::vector<TBranch*> b_mdnnSM0_eleup_DM0 , b_mdnnSM0_eledown_DM0;
  std::vector<TBranch*> b_mdnnSM0_eleup_DM1 , b_mdnnSM0_eledown_DM1;
  std::vector<TBranch*> b_mdnnSM1_eleup_DM0 , b_mdnnSM1_eledown_DM0;
  std::vector<TBranch*> b_mdnnSM1_eleup_DM1 , b_mdnnSM1_eledown_DM1;
  std::vector<TBranch*> b_mdnnBSM0_eleup_DM0, b_mdnnBSM0_eledown_DM0;
  std::vector<TBranch*> b_mdnnBSM0_eleup_DM1, b_mdnnBSM0_eledown_DM1;
  boost::format mdnnSM0name_eleup_DM0   ("mdnn__v0__kl1_c2v1_c31__%1%_eleup_DM0");
  boost::format mdnnSM0name_eledown_DM0 ("mdnn__v0__kl1_c2v1_c31__%1%_eledown_DM0");
  boost::format mdnnSM0name_eleup_DM1   ("mdnn__v0__kl1_c2v1_c31__%1%_eleup_DM1");
  boost::format mdnnSM0name_eledown_DM1 ("mdnn__v0__kl1_c2v1_c31__%1%_eledown_DM1");
  boost::format mdnnSM1name_eleup_DM0   ("mdnn__v1__kl1_c2v1_c31__%1%_eleup_DM0");
  boost::format mdnnSM1name_eledown_DM0 ("mdnn__v1__kl1_c2v1_c31__%1%_eledown_DM0");
  boost::format mdnnSM1name_eleup_DM1   ("mdnn__v1__kl1_c2v1_c31__%1%_eleup_DM1");
  boost::format mdnnSM1name_eledown_DM1 ("mdnn__v1__kl1_c2v1_c31__%1%_eledown_DM1");
  boost::format mdnnBSM0name_eleup_DM0  ("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_eleup_DM0");
  boost::format mdnnBSM0name_eledown_DM0("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_eledown_DM0");
  boost::format mdnnBSM0name_eleup_DM1  ("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_eleup_DM1");
  boost::format mdnnBSM0name_eledown_DM1("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_eledown_DM1");
  for (int i=0; i<mdnnSM0_size; i++)
  {
    std::string tmp_mdnnSM0_branch_name_up_DM0   = boost::str( mdnnSM0name_eleup_DM0   % (mci.getNodeNames(0)).at(i) );
    std::string tmp_mdnnSM0_branch_name_down_DM0 = boost::str( mdnnSM0name_eledown_DM0 % (mci.getNodeNames(0)).at(i) );
    std::string tmp_mdnnSM0_branch_name_up_DM1   = boost::str( mdnnSM0name_eleup_DM1   % (mci.getNodeNames(0)).at(i) );
    std::string tmp_mdnnSM0_branch_name_down_DM1 = boost::str( mdnnSM0name_eledown_DM1 % (mci.getNodeNames(0)).at(i) );
    TBranch* tmp_mdnnSM0_branch_up_DM0   = outTree->Branch(tmp_mdnnSM0_branch_name_up_DM0.c_str()  , &mdnnSM0_output_eleup  [0].at(i));
    TBranch* tmp_mdnnSM0_branch_down_DM0 = outTree->Branch(tmp_mdnnSM0_branch_name_down_DM0.c_str(), &mdnnSM0_output_eledown[0].at(i));
    TBranch* tmp_mdnnSM0_branch_up_DM1   = outTree->Branch(tmp_mdnnSM0_branch_name_up_DM1.c_str()  , &mdnnSM0_output_eleup  [1].at(i));
    TBranch* tmp_mdnnSM0_branch_down_DM1 = outTree->Branch(tmp_mdnnSM0_branch_name_down_DM1.c_str(), &mdnnSM0_output_eledown[1].at(i));
    b_mdnnSM0_eleup_DM0  .push_back(tmp_mdnnSM0_branch_up_DM0);
    b_mdnnSM0_eledown_DM0.push_back(tmp_mdnnSM0_branch_down_DM0);
    b_mdnnSM0_eleup_DM1  .push_back(tmp_mdnnSM0_branch_up_DM1);
    b_mdnnSM0_eledown_DM1.push_back(tmp_mdnnSM0_branch_down_DM1);
  }
  for (int i=0; i<mdnnSM1_size; i++)
  {
    std::string tmp_mdnnSM1_branch_name_up_DM0   = boost::str( mdnnSM1name_eleup_DM0   % (mci.getNodeNames(1)).at(i) );
    std::string tmp_mdnnSM1_branch_name_down_DM0 = boost::str( mdnnSM1name_eledown_DM0 % (mci.getNodeNames(1)).at(i) );
    std::string tmp_mdnnSM1_branch_name_up_DM1   = boost::str( mdnnSM1name_eleup_DM1   % (mci.getNodeNames(1)).at(i) );
    std::string tmp_mdnnSM1_branch_name_down_DM1 = boost::str( mdnnSM1name_eledown_DM1 % (mci.getNodeNames(1)).at(i) );
    TBranch* tmp_mdnnSM1_branch_up_DM0   = outTree->Branch(tmp_mdnnSM1_branch_name_up_DM0.c_str()  , &mdnnSM1_output_eleup  [0].at(i));
    TBranch* tmp_mdnnSM1_branch_down_DM0 = outTree->Branch(tmp_mdnnSM1_branch_name_down_DM0.c_str(), &mdnnSM1_output_eledown[0].at(i));
    TBranch* tmp_mdnnSM1_branch_up_DM1   = outTree->Branch(tmp_mdnnSM1_branch_name_up_DM1.c_str()  , &mdnnSM1_output_eleup  [1].at(i));
    TBranch* tmp_mdnnSM1_branch_down_DM1 = outTree->Branch(tmp_mdnnSM1_branch_name_down_DM1.c_str(), &mdnnSM1_output_eledown[1].at(i));
    b_mdnnSM1_eleup_DM0  .push_back(tmp_mdnnSM1_branch_up_DM0);
    b_mdnnSM1_eledown_DM0.push_back(tmp_mdnnSM1_branch_down_DM0);
    b_mdnnSM1_eleup_DM1  .push_back(tmp_mdnnSM1_branch_up_DM1);
    b_mdnnSM1_eledown_DM1.push_back(tmp_mdnnSM1_branch_down_DM1);
  }
  for (int i=0; i<mdnnBSM0_size; i++)
  {
    std::string tmp_mdnnBSM0_branch_name_up_DM0   = boost::str( mdnnBSM0name_eleup_DM0   % (mci.getNodeNames(2)).at(i) );
    std::string tmp_mdnnBSM0_branch_name_down_DM0 = boost::str( mdnnBSM0name_eledown_DM0 % (mci.getNodeNames(2)).at(i) );
    std::string tmp_mdnnBSM0_branch_name_up_DM1   = boost::str( mdnnBSM0name_eleup_DM1   % (mci.getNodeNames(2)).at(i) );
    std::string tmp_mdnnBSM0_branch_name_down_DM1 = boost::str( mdnnBSM0name_eledown_DM1 % (mci.getNodeNames(2)).at(i) );
    TBranch* tmp_mdnnBSM0_branch_up_DM0   = outTree->Branch(tmp_mdnnBSM0_branch_name_up_DM0.c_str()  , &mdnnBSM0_output_eleup  [0].at(i));
    TBranch* tmp_mdnnBSM0_branch_down_DM0 = outTree->Branch(tmp_mdnnBSM0_branch_name_down_DM0.c_str(), &mdnnBSM0_output_eledown[0].at(i));
    TBranch* tmp_mdnnBSM0_branch_up_DM1   = outTree->Branch(tmp_mdnnBSM0_branch_name_up_DM1.c_str()  , &mdnnBSM0_output_eleup  [1].at(i));
    TBranch* tmp_mdnnBSM0_branch_down_DM1 = outTree->Branch(tmp_mdnnBSM0_branch_name_down_DM1.c_str(), &mdnnBSM0_output_eledown[1].at(i));
    b_mdnnBSM0_eleup_DM0  .push_back(tmp_mdnnBSM0_branch_up_DM0);
    b_mdnnBSM0_eledown_DM0.push_back(tmp_mdnnBSM0_branch_down_DM0);
    b_mdnnBSM0_eleup_DM1  .push_back(tmp_mdnnBSM0_branch_up_DM1);
    b_mdnnBSM0_eledown_DM1.push_back(tmp_mdnnBSM0_branch_down_DM1);
  }

  // TES variations
  std::vector<Float_t> tauH_SVFIT_mass_tauup(N_tauhDM), DNNoutSM_kl_1_tauup(N_tauhDM), BDToutSM_kl_1_tauup(N_tauhDM);
  std::vector<Float_t> tauH_SVFIT_mass_taudown(N_tauhDM), DNNoutSM_kl_1_taudown(N_tauhDM), BDToutSM_kl_1_taudown(N_tauhDM);
  std::vector<std::vector<Float_t>> mdnnSM0_output_tauup (N_tauhDM, std::vector<Float_t>(mdnnSM0_size)) , mdnnSM0_output_taudown (N_tauhDM, std::vector<Float_t>(mdnnSM0_size));
  std::vector<std::vector<Float_t>> mdnnSM1_output_tauup (N_tauhDM, std::vector<Float_t>(mdnnSM1_size)) , mdnnSM1_output_taudown (N_tauhDM, std::vector<Float_t>(mdnnSM1_size));
  std::vector<std::vector<Float_t>> mdnnBSM0_output_tauup(N_tauhDM, std::vector<Float_t>(mdnnBSM0_size)), mdnnBSM0_output_taudown(N_tauhDM, std::vector<Float_t>(mdnnBSM0_size));
  TBranch* b_tauH_SVFIT_mass_tauup_DM0    = outTree->Branch("tauH_SVFIT_mass_tauup_DM0"   , &tauH_SVFIT_mass_tauup.at(0));    // DM 0
  TBranch* b_DNNoutSM_kl_1_tauup_DM0      = outTree->Branch("DNNoutSM_kl_1_tauup_DM0"     , &DNNoutSM_kl_1_tauup.at(0));
  TBranch* b_BDToutSM_kl_1_tauup_DM0      = outTree->Branch("BDToutSM_kl_1_tauup_DM0"     , &BDToutSM_kl_1_tauup.at(0));
  TBranch* b_tauH_SVFIT_mass_taudown_DM0  = outTree->Branch("tauH_SVFIT_mass_taudown_DM0" , &tauH_SVFIT_mass_taudown.at(0));
  TBranch* b_DNNoutSM_kl_1_taudown_DM0    = outTree->Branch("DNNoutSM_kl_1_taudown_DM0"   , &DNNoutSM_kl_1_taudown.at(0));
  TBranch* b_BDToutSM_kl_1_taudown_DM0    = outTree->Branch("BDToutSM_kl_1_taudown_DM0"   , &BDToutSM_kl_1_taudown.at(0));
  TBranch* b_tauH_SVFIT_mass_tauup_DM1    = outTree->Branch("tauH_SVFIT_mass_tauup_DM1"   , &tauH_SVFIT_mass_tauup.at(1));    // DM 1
  TBranch* b_DNNoutSM_kl_1_tauup_DM1      = outTree->Branch("DNNoutSM_kl_1_tauup_DM1"     , &DNNoutSM_kl_1_tauup.at(1));
  TBranch* b_BDToutSM_kl_1_tauup_DM1      = outTree->Branch("BDToutSM_kl_1_tauup_DM1"     , &BDToutSM_kl_1_tauup.at(1));
  TBranch* b_tauH_SVFIT_mass_taudown_DM1  = outTree->Branch("tauH_SVFIT_mass_taudown_DM1" , &tauH_SVFIT_mass_taudown.at(1));
  TBranch* b_DNNoutSM_kl_1_taudown_DM1    = outTree->Branch("DNNoutSM_kl_1_taudown_DM1"   , &DNNoutSM_kl_1_taudown.at(1));
  TBranch* b_BDToutSM_kl_1_taudown_DM1    = outTree->Branch("BDToutSM_kl_1_taudown_DM1"   , &BDToutSM_kl_1_taudown.at(1));
  TBranch* b_tauH_SVFIT_mass_tauup_DM10   = outTree->Branch("tauH_SVFIT_mass_tauup_DM10"  , &tauH_SVFIT_mass_tauup.at(2));    // DM 10
  TBranch* b_DNNoutSM_kl_1_tauup_DM10     = outTree->Branch("DNNoutSM_kl_1_tauup_DM10"    , &DNNoutSM_kl_1_tauup.at(2));
  TBranch* b_BDToutSM_kl_1_tauup_DM10     = outTree->Branch("BDToutSM_kl_1_tauup_DM10"    , &BDToutSM_kl_1_tauup.at(2));
  TBranch* b_tauH_SVFIT_mass_taudown_DM10 = outTree->Branch("tauH_SVFIT_mass_taudown_DM10", &tauH_SVFIT_mass_taudown.at(2));
  TBranch* b_DNNoutSM_kl_1_taudown_DM10   = outTree->Branch("DNNoutSM_kl_1_taudown_DM10"  , &DNNoutSM_kl_1_taudown.at(2));
  TBranch* b_BDToutSM_kl_1_taudown_DM10   = outTree->Branch("BDToutSM_kl_1_taudown_DM10"  , &BDToutSM_kl_1_taudown.at(2));
  TBranch* b_tauH_SVFIT_mass_tauup_DM11   = outTree->Branch("tauH_SVFIT_mass_tauup_DM11"  , &tauH_SVFIT_mass_tauup.at(3));    // DM 11
  TBranch* b_DNNoutSM_kl_1_tauup_DM11     = outTree->Branch("DNNoutSM_kl_1_tauup_DM11"    , &DNNoutSM_kl_1_tauup.at(3));
  TBranch* b_BDToutSM_kl_1_tauup_DM11     = outTree->Branch("BDToutSM_kl_1_tauup_DM11"    , &BDToutSM_kl_1_tauup.at(3));
  TBranch* b_tauH_SVFIT_mass_taudown_DM11 = outTree->Branch("tauH_SVFIT_mass_taudown_DM11", &tauH_SVFIT_mass_taudown.at(3));
  TBranch* b_DNNoutSM_kl_1_taudown_DM11   = outTree->Branch("DNNoutSM_kl_1_taudown_DM11"  , &DNNoutSM_kl_1_taudown.at(3));
  TBranch* b_BDToutSM_kl_1_taudown_DM11   = outTree->Branch("BDToutSM_kl_1_taudown_DM11"  , &BDToutSM_kl_1_taudown.at(3));
  std::vector<TBranch*> b_mdnnSM0_tauup_DM0  , b_mdnnSM0_taudown_DM0;
  std::vector<TBranch*> b_mdnnSM0_tauup_DM1  , b_mdnnSM0_taudown_DM1;
  std::vector<TBranch*> b_mdnnSM0_tauup_DM10 , b_mdnnSM0_taudown_DM10;
  std::vector<TBranch*> b_mdnnSM0_tauup_DM11 , b_mdnnSM0_taudown_DM11;
  std::vector<TBranch*> b_mdnnSM1_tauup_DM0  , b_mdnnSM1_taudown_DM0;
  std::vector<TBranch*> b_mdnnSM1_tauup_DM1  , b_mdnnSM1_taudown_DM1;
  std::vector<TBranch*> b_mdnnSM1_tauup_DM10 , b_mdnnSM1_taudown_DM10;
  std::vector<TBranch*> b_mdnnSM1_tauup_DM11 , b_mdnnSM1_taudown_DM11;
  std::vector<TBranch*> b_mdnnBSM0_tauup_DM0 , b_mdnnBSM0_taudown_DM0;
  std::vector<TBranch*> b_mdnnBSM0_tauup_DM1 , b_mdnnBSM0_taudown_DM1;
  std::vector<TBranch*> b_mdnnBSM0_tauup_DM10, b_mdnnBSM0_taudown_DM10;
  std::vector<TBranch*> b_mdnnBSM0_tauup_DM11, b_mdnnBSM0_taudown_DM11;
  boost::format mdnnSM0name_tauup_DM0    ("mdnn__v0__kl1_c2v1_c31__%1%_tauup_DM0");
  boost::format mdnnSM0name_taudown_DM0  ("mdnn__v0__kl1_c2v1_c31__%1%_taudown_DM0");
  boost::format mdnnSM0name_tauup_DM1    ("mdnn__v0__kl1_c2v1_c31__%1%_tauup_DM1");
  boost::format mdnnSM0name_taudown_DM1  ("mdnn__v0__kl1_c2v1_c31__%1%_taudown_DM1");
  boost::format mdnnSM0name_tauup_DM10   ("mdnn__v0__kl1_c2v1_c31__%1%_tauup_DM10");
  boost::format mdnnSM0name_taudown_DM10 ("mdnn__v0__kl1_c2v1_c31__%1%_taudown_DM10");
  boost::format mdnnSM0name_tauup_DM11   ("mdnn__v0__kl1_c2v1_c31__%1%_tauup_DM11");
  boost::format mdnnSM0name_taudown_DM11 ("mdnn__v0__kl1_c2v1_c31__%1%_taudown_DM11");
  boost::format mdnnSM1name_tauup_DM0    ("mdnn__v1__kl1_c2v1_c31__%1%_tauup_DM0");
  boost::format mdnnSM1name_taudown_DM0  ("mdnn__v1__kl1_c2v1_c31__%1%_taudown_DM0");
  boost::format mdnnSM1name_tauup_DM1    ("mdnn__v1__kl1_c2v1_c31__%1%_tauup_DM1");
  boost::format mdnnSM1name_taudown_DM1  ("mdnn__v1__kl1_c2v1_c31__%1%_taudown_DM1");
  boost::format mdnnSM1name_tauup_DM10   ("mdnn__v1__kl1_c2v1_c31__%1%_tauup_DM10");
  boost::format mdnnSM1name_taudown_DM10 ("mdnn__v1__kl1_c2v1_c31__%1%_taudown_DM10");
  boost::format mdnnSM1name_tauup_DM11   ("mdnn__v1__kl1_c2v1_c31__%1%_tauup_DM11");
  boost::format mdnnSM1name_taudown_DM11 ("mdnn__v1__kl1_c2v1_c31__%1%_taudown_DM11");
  boost::format mdnnBSM0name_tauup_DM0   ("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_tauup_DM0");
  boost::format mdnnBSM0name_taudown_DM0 ("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_taudown_DM0");
  boost::format mdnnBSM0name_tauup_DM1   ("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_tauup_DM1");
  boost::format mdnnBSM0name_taudown_DM1 ("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_taudown_DM1");
  boost::format mdnnBSM0name_tauup_DM10  ("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_tauup_DM10");
  boost::format mdnnBSM0name_taudown_DM10("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_taudown_DM10");
  boost::format mdnnBSM0name_tauup_DM11  ("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_tauup_DM11");
  boost::format mdnnBSM0name_taudown_DM11("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_taudown_DM11");
  for (int i=0; i<mdnnSM0_size; i++)
  {
    std::string tmp_mdnnSM0_branch_name_up_DM0    = boost::str( mdnnSM0name_tauup_DM0    % (mci.getNodeNames(0)).at(i) );
    std::string tmp_mdnnSM0_branch_name_down_DM0  = boost::str( mdnnSM0name_taudown_DM0  % (mci.getNodeNames(0)).at(i) );
    std::string tmp_mdnnSM0_branch_name_up_DM1    = boost::str( mdnnSM0name_tauup_DM1    % (mci.getNodeNames(0)).at(i) );
    std::string tmp_mdnnSM0_branch_name_down_DM1  = boost::str( mdnnSM0name_taudown_DM1  % (mci.getNodeNames(0)).at(i) );
    std::string tmp_mdnnSM0_branch_name_up_DM10   = boost::str( mdnnSM0name_tauup_DM10   % (mci.getNodeNames(0)).at(i) );
    std::string tmp_mdnnSM0_branch_name_down_DM10 = boost::str( mdnnSM0name_taudown_DM10 % (mci.getNodeNames(0)).at(i) );
    std::string tmp_mdnnSM0_branch_name_up_DM11   = boost::str( mdnnSM0name_tauup_DM11   % (mci.getNodeNames(0)).at(i) );
    std::string tmp_mdnnSM0_branch_name_down_DM11 = boost::str( mdnnSM0name_taudown_DM11 % (mci.getNodeNames(0)).at(i) );
    TBranch* tmp_mdnnSM0_branch_up_DM0    = outTree->Branch(tmp_mdnnSM0_branch_name_up_DM0.c_str()   , &mdnnSM0_output_tauup  [0].at(i));
    TBranch* tmp_mdnnSM0_branch_down_DM0  = outTree->Branch(tmp_mdnnSM0_branch_name_down_DM0.c_str() , &mdnnSM0_output_taudown[0].at(i));
    TBranch* tmp_mdnnSM0_branch_up_DM1    = outTree->Branch(tmp_mdnnSM0_branch_name_up_DM1.c_str()   , &mdnnSM0_output_tauup  [1].at(i));
    TBranch* tmp_mdnnSM0_branch_down_DM1  = outTree->Branch(tmp_mdnnSM0_branch_name_down_DM1.c_str() , &mdnnSM0_output_taudown[1].at(i));
    TBranch* tmp_mdnnSM0_branch_up_DM10   = outTree->Branch(tmp_mdnnSM0_branch_name_up_DM10.c_str()  , &mdnnSM0_output_tauup  [2].at(i));
    TBranch* tmp_mdnnSM0_branch_down_DM10 = outTree->Branch(tmp_mdnnSM0_branch_name_down_DM10.c_str(), &mdnnSM0_output_taudown[2].at(i));
    TBranch* tmp_mdnnSM0_branch_up_DM11   = outTree->Branch(tmp_mdnnSM0_branch_name_up_DM11.c_str()  , &mdnnSM0_output_tauup  [3].at(i));
    TBranch* tmp_mdnnSM0_branch_down_DM11 = outTree->Branch(tmp_mdnnSM0_branch_name_down_DM11.c_str(), &mdnnSM0_output_taudown[3].at(i));
    b_mdnnSM0_tauup_DM0   .push_back(tmp_mdnnSM0_branch_up_DM0);
    b_mdnnSM0_taudown_DM0 .push_back(tmp_mdnnSM0_branch_down_DM0);
    b_mdnnSM0_tauup_DM1   .push_back(tmp_mdnnSM0_branch_up_DM1);
    b_mdnnSM0_taudown_DM1 .push_back(tmp_mdnnSM0_branch_down_DM1);
    b_mdnnSM0_tauup_DM10  .push_back(tmp_mdnnSM0_branch_up_DM10);
    b_mdnnSM0_taudown_DM10.push_back(tmp_mdnnSM0_branch_down_DM10);
    b_mdnnSM0_tauup_DM11  .push_back(tmp_mdnnSM0_branch_up_DM11);
    b_mdnnSM0_taudown_DM11.push_back(tmp_mdnnSM0_branch_down_DM11);
  }
  for (int i=0; i<mdnnSM1_size; i++)
  {
    std::string tmp_mdnnSM1_branch_name_up_DM0    = boost::str( mdnnSM1name_tauup_DM0    % (mci.getNodeNames(1)).at(i) );
    std::string tmp_mdnnSM1_branch_name_down_DM0  = boost::str( mdnnSM1name_taudown_DM0  % (mci.getNodeNames(1)).at(i) );
    std::string tmp_mdnnSM1_branch_name_up_DM1    = boost::str( mdnnSM1name_tauup_DM1    % (mci.getNodeNames(1)).at(i) );
    std::string tmp_mdnnSM1_branch_name_down_DM1  = boost::str( mdnnSM1name_taudown_DM1  % (mci.getNodeNames(1)).at(i) );
    std::string tmp_mdnnSM1_branch_name_up_DM10   = boost::str( mdnnSM1name_tauup_DM10   % (mci.getNodeNames(1)).at(i) );
    std::string tmp_mdnnSM1_branch_name_down_DM10 = boost::str( mdnnSM1name_taudown_DM10 % (mci.getNodeNames(1)).at(i) );
    std::string tmp_mdnnSM1_branch_name_up_DM11   = boost::str( mdnnSM1name_tauup_DM11   % (mci.getNodeNames(1)).at(i) );
    std::string tmp_mdnnSM1_branch_name_down_DM11 = boost::str( mdnnSM1name_taudown_DM11 % (mci.getNodeNames(1)).at(i) );
    TBranch* tmp_mdnnSM1_branch_up_DM0    = outTree->Branch(tmp_mdnnSM1_branch_name_up_DM0.c_str()   , &mdnnSM1_output_tauup  [0].at(i));
    TBranch* tmp_mdnnSM1_branch_down_DM0  = outTree->Branch(tmp_mdnnSM1_branch_name_down_DM0.c_str() , &mdnnSM1_output_taudown[0].at(i));
    TBranch* tmp_mdnnSM1_branch_up_DM1    = outTree->Branch(tmp_mdnnSM1_branch_name_up_DM1.c_str()   , &mdnnSM1_output_tauup  [1].at(i));
    TBranch* tmp_mdnnSM1_branch_down_DM1  = outTree->Branch(tmp_mdnnSM1_branch_name_down_DM1.c_str() , &mdnnSM1_output_taudown[1].at(i));
    TBranch* tmp_mdnnSM1_branch_up_DM10   = outTree->Branch(tmp_mdnnSM1_branch_name_up_DM10.c_str()  , &mdnnSM1_output_tauup  [2].at(i));
    TBranch* tmp_mdnnSM1_branch_down_DM10 = outTree->Branch(tmp_mdnnSM1_branch_name_down_DM10.c_str(), &mdnnSM1_output_taudown[2].at(i));
    TBranch* tmp_mdnnSM1_branch_up_DM11   = outTree->Branch(tmp_mdnnSM1_branch_name_up_DM11.c_str()  , &mdnnSM1_output_tauup  [3].at(i));
    TBranch* tmp_mdnnSM1_branch_down_DM11 = outTree->Branch(tmp_mdnnSM1_branch_name_down_DM11.c_str(), &mdnnSM1_output_taudown[3].at(i));
    b_mdnnSM1_tauup_DM0   .push_back(tmp_mdnnSM1_branch_up_DM0);
    b_mdnnSM1_taudown_DM0 .push_back(tmp_mdnnSM1_branch_down_DM0);
    b_mdnnSM1_tauup_DM1   .push_back(tmp_mdnnSM1_branch_up_DM1);
    b_mdnnSM1_taudown_DM1 .push_back(tmp_mdnnSM1_branch_down_DM1);
    b_mdnnSM1_tauup_DM10  .push_back(tmp_mdnnSM1_branch_up_DM10);
    b_mdnnSM1_taudown_DM10.push_back(tmp_mdnnSM1_branch_down_DM10);
    b_mdnnSM1_tauup_DM11  .push_back(tmp_mdnnSM1_branch_up_DM11);
    b_mdnnSM1_taudown_DM11.push_back(tmp_mdnnSM1_branch_down_DM11);
  }
  for (int i=0; i<mdnnBSM0_size; i++)
  {
    std::string tmp_mdnnBSM0_branch_name_up_DM0    = boost::str( mdnnBSM0name_tauup_DM0    % (mci.getNodeNames(2)).at(i) );
    std::string tmp_mdnnBSM0_branch_name_down_DM0  = boost::str( mdnnBSM0name_taudown_DM0  % (mci.getNodeNames(2)).at(i) );
    std::string tmp_mdnnBSM0_branch_name_up_DM1    = boost::str( mdnnBSM0name_tauup_DM1    % (mci.getNodeNames(2)).at(i) );
    std::string tmp_mdnnBSM0_branch_name_down_DM1  = boost::str( mdnnBSM0name_taudown_DM1  % (mci.getNodeNames(2)).at(i) );
    std::string tmp_mdnnBSM0_branch_name_up_DM10   = boost::str( mdnnBSM0name_tauup_DM10   % (mci.getNodeNames(2)).at(i) );
    std::string tmp_mdnnBSM0_branch_name_down_DM10 = boost::str( mdnnBSM0name_taudown_DM10 % (mci.getNodeNames(2)).at(i) );
    std::string tmp_mdnnBSM0_branch_name_up_DM11   = boost::str( mdnnBSM0name_tauup_DM11   % (mci.getNodeNames(2)).at(i) );
    std::string tmp_mdnnBSM0_branch_name_down_DM11 = boost::str( mdnnBSM0name_taudown_DM11 % (mci.getNodeNames(2)).at(i) );
    TBranch* tmp_mdnnBSM0_branch_up_DM0    = outTree->Branch(tmp_mdnnBSM0_branch_name_up_DM0.c_str()   , &mdnnBSM0_output_tauup  [0].at(i));
    TBranch* tmp_mdnnBSM0_branch_down_DM0  = outTree->Branch(tmp_mdnnBSM0_branch_name_down_DM0.c_str() , &mdnnBSM0_output_taudown[0].at(i));
    TBranch* tmp_mdnnBSM0_branch_up_DM1    = outTree->Branch(tmp_mdnnBSM0_branch_name_up_DM1.c_str()   , &mdnnBSM0_output_tauup  [1].at(i));
    TBranch* tmp_mdnnBSM0_branch_down_DM1  = outTree->Branch(tmp_mdnnBSM0_branch_name_down_DM1.c_str() , &mdnnBSM0_output_taudown[1].at(i));
    TBranch* tmp_mdnnBSM0_branch_up_DM10   = outTree->Branch(tmp_mdnnBSM0_branch_name_up_DM10.c_str()  , &mdnnBSM0_output_tauup  [2].at(i));
    TBranch* tmp_mdnnBSM0_branch_down_DM10 = outTree->Branch(tmp_mdnnBSM0_branch_name_down_DM10.c_str(), &mdnnBSM0_output_taudown[2].at(i));
    TBranch* tmp_mdnnBSM0_branch_up_DM11   = outTree->Branch(tmp_mdnnBSM0_branch_name_up_DM11.c_str()  , &mdnnBSM0_output_tauup  [3].at(i));
    TBranch* tmp_mdnnBSM0_branch_down_DM11 = outTree->Branch(tmp_mdnnBSM0_branch_name_down_DM11.c_str(), &mdnnBSM0_output_taudown[3].at(i));
    b_mdnnBSM0_tauup_DM0   .push_back(tmp_mdnnBSM0_branch_up_DM0);
    b_mdnnBSM0_taudown_DM0 .push_back(tmp_mdnnBSM0_branch_down_DM0);
    b_mdnnBSM0_tauup_DM1   .push_back(tmp_mdnnBSM0_branch_up_DM1);
    b_mdnnBSM0_taudown_DM1 .push_back(tmp_mdnnBSM0_branch_down_DM1);
    b_mdnnBSM0_tauup_DM10  .push_back(tmp_mdnnBSM0_branch_up_DM10);
    b_mdnnBSM0_taudown_DM10.push_back(tmp_mdnnBSM0_branch_down_DM10);
    b_mdnnBSM0_tauup_DM11  .push_back(tmp_mdnnBSM0_branch_up_DM11);
    b_mdnnBSM0_taudown_DM11.push_back(tmp_mdnnBSM0_branch_down_DM11);
  }

  // JES variations
  std::vector<Float_t> tauH_SVFIT_mass_jetup(N_jecSources), DNNoutSM_kl_1_jetup(N_jecSources), BDToutSM_kl_1_jetup(N_jecSources);
  std::vector<Float_t> tauH_SVFIT_mass_jetdown(N_jecSources), DNNoutSM_kl_1_jetdown(N_jecSources), BDToutSM_kl_1_jetdown(N_jecSources);
  std::vector<std::vector<Float_t>> mdnnSM0_output_jetup (N_jecSources, std::vector<Float_t>(mdnnSM0_size)) , mdnnSM0_output_jetdown (N_jecSources, std::vector<Float_t>(mdnnSM0_size));
  std::vector<std::vector<Float_t>> mdnnSM1_output_jetup (N_jecSources, std::vector<Float_t>(mdnnSM1_size)) , mdnnSM1_output_jetdown (N_jecSources, std::vector<Float_t>(mdnnSM1_size));
  std::vector<std::vector<Float_t>> mdnnBSM0_output_jetup(N_jecSources, std::vector<Float_t>(mdnnBSM0_size)), mdnnBSM0_output_jetdown(N_jecSources, std::vector<Float_t>(mdnnBSM0_size));
  std::vector<TBranch*> b_tauH_SVFIT_mass_jetup, b_tauH_SVFIT_mass_jetdown;
  std::vector<TBranch*> b_DNNoutSM_kl_1_jetup  , b_DNNoutSM_kl_1_jetdown  ;
  std::vector<TBranch*> b_BDToutSM_kl_1_jetup  , b_BDToutSM_kl_1_jetdown  ;
  std::vector<std::vector<TBranch*>> b_mdnnSM0_jetup (N_jecSources, std::vector<TBranch*>(mdnnSM0_size)) , b_mdnnSM0_jetdown (N_jecSources, std::vector<TBranch*>(mdnnSM0_size));
  std::vector<std::vector<TBranch*>> b_mdnnSM1_jetup (N_jecSources, std::vector<TBranch*>(mdnnSM1_size)) , b_mdnnSM1_jetdown (N_jecSources, std::vector<TBranch*>(mdnnSM1_size));
  std::vector<std::vector<TBranch*>> b_mdnnBSM0_jetup(N_jecSources, std::vector<TBranch*>(mdnnBSM0_size)), b_mdnnBSM0_jetdown(N_jecSources, std::vector<TBranch*>(mdnnBSM0_size));
  boost::format tauHName_up  ("tauH_SVFIT_mass_jetup%i");
  boost::format DNNName_up   ("DNNoutSM_kl_1_jetup%i");
  boost::format BDTName_up   ("BDToutSM_kl_1_jetup%i");
  boost::format tauHName_down("tauH_SVFIT_mass_jetdown%i");
  boost::format DNNName_down ("DNNoutSM_kl_1_jetdown%i");
  boost::format BDTName_down ("BDToutSM_kl_1_jetdown%i");
  boost::format mdnnSM0name_jetup   ("mdnn__v0__kl1_c2v1_c31__%1%_jetup%2%");
  boost::format mdnnSM0name_jetdown ("mdnn__v0__kl1_c2v1_c31__%1%_jetdown%2%");
  boost::format mdnnSM1name_jetup   ("mdnn__v1__kl1_c2v1_c31__%1%_jetup%2%");
  boost::format mdnnSM1name_jetdown ("mdnn__v1__kl1_c2v1_c31__%1%_jetdown%2%");
  boost::format mdnnBSM0name_jetup  ("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_jetup%2%");
  boost::format mdnnBSM0name_jetdown("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_jetdown%2%");
  for (int i=0; i<N_jecSources; i++)
  {
    std::string tmp_tauH_up_branch_name   = boost::str(tauHName_up   % (i+1));
    std::string tmp_DNN_up_branch_name    = boost::str(DNNName_up    % (i+1));
    std::string tmp_BDT_up_branch_name    = boost::str(BDTName_up    % (i+1));
    std::string tmp_tauH_down_branch_name = boost::str(tauHName_down % (i+1));
    std::string tmp_DNN_down_branch_name  = boost::str(DNNName_down  % (i+1));
    std::string tmp_BDT_down_branch_name  = boost::str(BDTName_down  % (i+1));

    TBranch* tmp_tauH_up_branch   = outTree->Branch(tmp_tauH_up_branch_name  .c_str(), &tauH_SVFIT_mass_jetup.at(i));
    TBranch* tmp_DNN_up_branch    = outTree->Branch(tmp_DNN_up_branch_name   .c_str(), &DNNoutSM_kl_1_jetup.at(i));
    TBranch* tmp_BDT_up_branch    = outTree->Branch(tmp_BDT_up_branch_name   .c_str(), &BDToutSM_kl_1_jetup.at(i));
    TBranch* tmp_tauH_down_branch = outTree->Branch(tmp_tauH_down_branch_name.c_str(), &tauH_SVFIT_mass_jetdown.at(i));
    TBranch* tmp_DNN_down_branch  = outTree->Branch(tmp_DNN_down_branch_name .c_str(), &DNNoutSM_kl_1_jetdown.at(i));
    TBranch* tmp_DBT_down_branch  = outTree->Branch(tmp_BDT_down_branch_name .c_str(), &BDToutSM_kl_1_jetdown.at(i));

    b_tauH_SVFIT_mass_jetup  .push_back(tmp_tauH_up_branch);
    b_DNNoutSM_kl_1_jetup    .push_back(tmp_DNN_up_branch);
    b_BDToutSM_kl_1_jetup    .push_back(tmp_BDT_up_branch);
    b_tauH_SVFIT_mass_jetdown.push_back(tmp_tauH_down_branch);
    b_DNNoutSM_kl_1_jetdown  .push_back(tmp_DNN_down_branch);
    b_BDToutSM_kl_1_jetdown  .push_back(tmp_DBT_down_branch);

    for (int k=0; k<mdnnSM0_size; k++)
    {
      std::string tmp_mdnnSM0_branch_name_up   = boost::str( mdnnSM0name_jetup   % (mci.getNodeNames(0)).at(k) % (i+1) );
      std::string tmp_mdnnSM0_branch_name_down = boost::str( mdnnSM0name_jetdown % (mci.getNodeNames(0)).at(k) % (i+1) );
      TBranch* tmp_mdnnSM0_branch_up   = outTree->Branch(tmp_mdnnSM0_branch_name_up.c_str()  , &mdnnSM0_output_jetup  [i].at(k));
      TBranch* tmp_mdnnSM0_branch_down = outTree->Branch(tmp_mdnnSM0_branch_name_down.c_str(), &mdnnSM0_output_jetdown[i].at(k));
      b_mdnnSM0_jetup  [i][k] = tmp_mdnnSM0_branch_up;
      b_mdnnSM0_jetdown[i][k] = tmp_mdnnSM0_branch_down;
    }
    for (int k=0; k<mdnnSM1_size; k++)
    {
      std::string tmp_mdnnSM1_branch_name_up   = boost::str( mdnnSM1name_jetup   % (mci.getNodeNames(1)).at(k) % (i+1) );
      std::string tmp_mdnnSM1_branch_name_down = boost::str( mdnnSM1name_jetdown % (mci.getNodeNames(1)).at(k) % (i+1) );
      TBranch* tmp_mdnnSM1_branch_up   = outTree->Branch(tmp_mdnnSM1_branch_name_up.c_str()  , &mdnnSM1_output_jetup  [i].at(k));
      TBranch* tmp_mdnnSM1_branch_down = outTree->Branch(tmp_mdnnSM1_branch_name_down.c_str(), &mdnnSM1_output_jetdown[i].at(k));
      b_mdnnSM1_jetup  [i][k] = tmp_mdnnSM1_branch_up;
      b_mdnnSM1_jetdown[i][k] = tmp_mdnnSM1_branch_down;
    }
    for (int k=0; k<mdnnBSM0_size; k++)
    {
      std::string tmp_mdnnBSM0_branch_name_up   = boost::str( mdnnBSM0name_jetup   % (mci.getNodeNames(2)).at(k) % (i+1) );
      std::string tmp_mdnnBSM0_branch_name_down = boost::str( mdnnBSM0name_jetdown % (mci.getNodeNames(2)).at(k) % (i+1) );
      TBranch* tmp_mdnnBSM0_branch_up   = outTree->Branch(tmp_mdnnBSM0_branch_name_up.c_str()  , &mdnnBSM0_output_jetup  [i].at(k));
      TBranch* tmp_mdnnBSM0_branch_down = outTree->Branch(tmp_mdnnBSM0_branch_name_down.c_str(), &mdnnBSM0_output_jetdown[i].at(k));
      b_mdnnBSM0_jetup  [i][k] = tmp_mdnnBSM0_branch_up;
      b_mdnnBSM0_jetdown[i][k] = tmp_mdnnBSM0_branch_down;
    }
  }

  // JES variations Total
  Float_t tauH_SVFIT_mass_jetupTot, DNNoutSM_kl_1_jetupTot, BDToutSM_kl_1_jetupTot;
  Float_t tauH_SVFIT_mass_jetdownTot, DNNoutSM_kl_1_jetdownTot, BDToutSM_kl_1_jetdownTot;
  std::vector<Float_t> mdnnSM0_output_jetupTot (mdnnSM0_size) , mdnnSM0_output_jetdownTot (mdnnSM0_size);
  std::vector<Float_t> mdnnSM1_output_jetupTot (mdnnSM1_size) , mdnnSM1_output_jetdownTot (mdnnSM1_size);
  std::vector<Float_t> mdnnBSM0_output_jetupTot(mdnnBSM0_size), mdnnBSM0_output_jetdownTot(mdnnBSM0_size);
  TBranch* b_tauH_SVFIT_mass_jetupTot   = outTree->Branch("tauH_SVFIT_mass_jetupTot"  , &tauH_SVFIT_mass_jetupTot);
  TBranch* b_DNNoutSM_kl_1_jetupTot     = outTree->Branch("DNNoutSM_kl_1_jetupTot"    , &DNNoutSM_kl_1_jetupTot);
  TBranch* b_BDToutSM_kl_1_jetupTot     = outTree->Branch("BDToutSM_kl_1_jetupTot"    , &BDToutSM_kl_1_jetupTot);
  TBranch* b_tauH_SVFIT_mass_jetdownTot = outTree->Branch("tauH_SVFIT_mass_jetdownTot", &tauH_SVFIT_mass_jetdownTot);
  TBranch* b_DNNoutSM_kl_1_jetdownTot   = outTree->Branch("DNNoutSM_kl_1_jetdownTot"  , &DNNoutSM_kl_1_jetdownTot);
  TBranch* b_BDToutSM_kl_1_jetdownTot   = outTree->Branch("BDToutSM_kl_1_jetdownTot"  , &BDToutSM_kl_1_jetdownTot);
  std::vector<TBranch*> b_mdnnSM0_jetupTot , b_mdnnSM0_jetdownTot;
  std::vector<TBranch*> b_mdnnSM1_jetupTot , b_mdnnSM1_jetdownTot;
  std::vector<TBranch*> b_mdnnBSM0_jetupTot, b_mdnnBSM0_jetdownTot;
  boost::format mdnnSM0name_jetupTot   ("mdnn__v0__kl1_c2v1_c31__%1%_jetupTot");
  boost::format mdnnSM0name_jetdownTot ("mdnn__v0__kl1_c2v1_c31__%1%_jetdownTot");
  boost::format mdnnSM1name_jetupTot   ("mdnn__v1__kl1_c2v1_c31__%1%_jetupTot");
  boost::format mdnnSM1name_jetdownTot ("mdnn__v1__kl1_c2v1_c31__%1%_jetdownTot");
  boost::format mdnnBSM0name_jetupTot  ("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_jetupTot");
  boost::format mdnnBSM0name_jetdownTot("mdnn__v0__kl1_c2v1_c31_vbfbsm__%1%_jetdownTot");
  for (int i=0; i<mdnnSM0_size; i++)
  {
    std::string tmp_mdnnSM0_branch_name_up   = boost::str( mdnnSM0name_jetupTot   % (mci.getNodeNames(0)).at(i) );
    std::string tmp_mdnnSM0_branch_name_down = boost::str( mdnnSM0name_jetdownTot % (mci.getNodeNames(0)).at(i) );
    TBranch* tmp_mdnnSM0_branch_up   = outTree->Branch(tmp_mdnnSM0_branch_name_up.c_str()  , &mdnnSM0_output_jetupTot.at(i));
    TBranch* tmp_mdnnSM0_branch_down = outTree->Branch(tmp_mdnnSM0_branch_name_down.c_str(), &mdnnSM0_output_jetdownTot.at(i));
    b_mdnnSM0_jetupTot  .push_back(tmp_mdnnSM0_branch_up);
    b_mdnnSM0_jetdownTot.push_back(tmp_mdnnSM0_branch_down);
  }
  for (int i=0; i<mdnnSM1_size; i++)
  {
    std::string tmp_mdnnSM1_branch_name_up   = boost::str( mdnnSM1name_jetupTot   % (mci.getNodeNames(1)).at(i) );
    std::string tmp_mdnnSM1_branch_name_down = boost::str( mdnnSM1name_jetdownTot % (mci.getNodeNames(1)).at(i) );
    TBranch* tmp_mdnnSM1_branch_up   = outTree->Branch(tmp_mdnnSM1_branch_name_up.c_str()  , &mdnnSM1_output_jetupTot.at(i));
    TBranch* tmp_mdnnSM1_branch_down = outTree->Branch(tmp_mdnnSM1_branch_name_down.c_str(), &mdnnSM1_output_jetdownTot.at(i));
    b_mdnnSM1_jetupTot  .push_back(tmp_mdnnSM1_branch_up);
    b_mdnnSM1_jetdownTot.push_back(tmp_mdnnSM1_branch_down);
  }
  for (int i=0; i<mdnnBSM0_size; i++)
  {
    std::string tmp_mdnnBSM0_branch_name_up   = boost::str( mdnnBSM0name_jetupTot   % (mci.getNodeNames(2)).at(i) );
    std::string tmp_mdnnBSM0_branch_name_down = boost::str( mdnnBSM0name_jetdownTot % (mci.getNodeNames(2)).at(i) );
    TBranch* tmp_mdnnBSM0_branch_up   = outTree->Branch(tmp_mdnnBSM0_branch_name_up.c_str()  , &mdnnBSM0_output_jetupTot.at(i));
    TBranch* tmp_mdnnBSM0_branch_down = outTree->Branch(tmp_mdnnBSM0_branch_name_down.c_str(), &mdnnBSM0_output_jetdownTot.at(i));
    b_mdnnBSM0_jetupTot  .push_back(tmp_mdnnBSM0_branch_up);
    b_mdnnBSM0_jetdownTot.push_back(tmp_mdnnBSM0_branch_down);
  }

  // Add some branches to store timing information
  double time_prep, time_nominal, time_TES, time_EES, time_MES, time_splitJES, time_totalJES, time_tot;
  TBranch* b_time_prep     = outTree->Branch("time_preparation", &time_prep);
  TBranch* b_time_nominal  = outTree->Branch("time_nominal"    , &time_nominal);
  TBranch* b_time_TES      = outTree->Branch("time_TES"        , &time_TES);
  TBranch* b_time_EES      = outTree->Branch("time_EES"        , &time_EES);
  TBranch* b_time_MES      = outTree->Branch("time_MES"        , &time_MES);
  TBranch* b_time_splitJES = outTree->Branch("time_splitJES"   , &time_splitJES);
  TBranch* b_time_totalJES = outTree->Branch("time_totalJES"   , &time_totalJES);
  TBranch* b_time_tot      = outTree->Branch("time_tot"        , &time_tot);

  // Loop on selected entries
  cout << "** ANALYSIS: Adding new branches..." << endl;
  for(int i=0;i<outTree->GetEntries();i++)
  {
    if (i % 500 == 0) cout << "- reading event " << i << endl ;
    if (i == nMaxEvts ) break;

    // Get Entry
    outTree->GetEntry(i);

    if (i % 500 == 0) std::cout << "---------------- Event: " << i << " -------------------" << std::endl;

    // Timing info
    auto start_tot  = high_resolution_clock::now();
    auto start_prep = high_resolution_clock::now();

    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // Build central quantities
    TLorentzVector tau1, tau2, bjet1, bjet2, vbfjet1, vbfjet2, met, svfit;
    tau1.SetPtEtaPhiE(dau1_pt, dau1_eta, dau1_phi, dau1_e);
    tau2.SetPtEtaPhiE(dau2_pt, dau2_eta, dau2_phi, dau2_e);
    bjet1.SetPtEtaPhiE(bjet1_pt, bjet1_eta, bjet1_phi, bjet1_e);
    bjet2.SetPtEtaPhiE(bjet2_pt, bjet2_eta, bjet2_phi, bjet2_e);
    vbfjet1.SetPtEtaPhiE(vbfjet1_pt, vbfjet1_eta, vbfjet1_phi, vbfjet1_e);
    vbfjet2.SetPtEtaPhiE(vbfjet2_pt, vbfjet2_eta, vbfjet2_phi, vbfjet2_e);
    svfit.SetPtEtaPhiM(tauH_SVFIT_pt, tauH_SVFIT_eta, tauH_SVFIT_phi, tauH_SVFIT_mass);
    met.SetPxPyPzE(METx, METy, 0, std::hypot(METx, METy));
    TVector2 ptmiss = TVector2(METx, METy);
    TMatrixD stableMetCov (2, 2) ;
    stableMetCov (0,0) = met_cov00;
    stableMetCov (1,0) = met_cov01;
    stableMetCov (0,1) = met_cov10;
    stableMetCov (1,1) = met_cov11;

    float mTtot                         = Calculate_TotalMT(tau1, tau2, met);
    float pzeta_vis                     = Calculate_visiblePzeta(tau1, tau2);
    float pzeta                         = Calculate_Pzeta(tau1, tau2, met);
    float BDT_ditau_deltaPhi            = ROOT::Math::VectorUtil::DeltaPhi(tau1, tau2);
    float BDT_tauHsvfitMet_deltaPhi     = ROOT::Math::VectorUtil::DeltaPhi(svfit, met);
    float mT_tauH_MET                   = Calculate_MT( (tau1+tau2)+met, met);
    float BDT_MX                        = Calculate_MX(tau1, tau2, bjet1, bjet2, met);
    float BDT_bH_tauH_MET_InvMass       = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1+tau2)+met);
    float BDT_bH_tauH_SVFIT_InvMass     = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), svfit);
    float BDT_bH_tauH_InvMass           = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1+tau2));
    float BDT_MET_bH_cosTheta           = Calculate_cosTheta_2bodies(getLVfromTLV(met), getLVfromTLV(bjet1+bjet2));
    bool KinFitConv                     = HHKin_chi2 > 0;
    bool SVfitConv                      = tauH_SVFIT_mass > 0;

    // --- --- --- Redefine BDT channel (defined differently in the BDT weights)
    float BDT_channel;
    if      (pType == 0) BDT_channel = 1.;
    else if (pType == 1) BDT_channel = 0.;
    else if (pType == 2) BDT_channel = 2.;

    // --- --- --- VBF multiclassinterface channel
    float mdnn_isMuTau=0., mdnn_isETau=0., mdnn_isTauTau=0.;
    if      (pType == 0) mdnn_isMuTau  = 1.;
    else if (pType == 1) mdnn_isETau   = 1.;
    else if (pType == 2) mdnn_isTauTau = 1.;

    // --- --- --- Set DNN quantities that are constant for each event
    Channel channel = tauTau;
    if (pType == 0)
    {
      channel = muTau;
    }
    else if (pType == 1)
    {
      channel = eTau;
    }
    int nvbf = 0;
    if (isVBF)
    {
      if (vbfjet1_e != -999.) nvbf++;
      if (vbfjet2_e != -999.) nvbf++;
    }
    DNNreader.SetEventInputs(channel, isBoosted, nvbf, EventNumber,
        bjet1_bID_deepFlavor, bjet2_bID_deepFlavor, CvsL_b1, CvsL_b2, CvsL_vbf1, CvsL_vbf2,
        CvsB_b1, CvsB_b2, CvsB_vbf1, CvsB_vbf2, HHbtag_b1, HHbtag_b2, HHbtag_vbf1, HHbtag_vbf2);

    // Timing info
    auto end_prep = high_resolution_clock::now();

    // ---- ---- ---- ---- ---- ---- ----
    // ---- ---- Do nominal now ---- ----
    // ---- ---- ---- ---- ---- ---- ----
    // This section is just for testing purposes and should be
    // skipped (i.e. doNominal set to false in the config)

    // Timing info
    auto start_nomin = high_resolution_clock::now();
    if (doNominal)
    {
      if (isData)
      {
        // Fill with nominal value
        tauH_SVFIT_mass_new = tauH_SVFIT_mass;
        DNNoutSM_kl_1_new   = DNNoutSM_kl_1;
        BDToutSM_kl_1_new   = BDToutSM_kl_1;

        // VBF multiclass
        mci.clearInputs();
        for (uint j=0; j<mci.getNumberOfModels(); j++)
        {
          mci.setInputs(j,
          {
            {"is_mutau", mdnn_isMuTau}, {"is_etau", mdnn_isETau}, {"is_tautau", mdnn_isTauTau},
            {"bjet1_pt", bjet1_pt}, {"bjet1_eta", bjet1_eta}, {"bjet1_phi", bjet1_phi}, {"bjet1_e", bjet1_e},
            {"bjet1_deepflavor_b", bjet1_bID_deepFlavor}, {"bjet1_deepflavor_cvsb", CvsB_b1}, {"bjet1_deepflavor_cvsl", CvsL_b1}, {"bjet1_hhbtag", HHbtag_b1},
            {"bjet2_pt", bjet2_pt}, {"bjet2_eta", bjet2_eta}, {"bjet2_phi", bjet2_phi}, {"bjet2_e", bjet2_e},
            {"bjet2_deepflavor_b", bjet2_bID_deepFlavor}, {"bjet2_deepflavor_cvsb", CvsB_b2}, {"bjet2_deepflavor_cvsl", CvsL_b2}, {"bjet2_hhbtag", HHbtag_b2},
            {"vbfjet1_pt", vbfjet1_pt}, {"vbfjet1_eta", vbfjet1_eta}, {"vbfjet1_phi", vbfjet1_phi}, {"vbfjet1_e", vbfjet1_e},
            {"vbfjet1_deepflavor_b", VBFjet1_btag_deepFlavor}, {"vbfjet1_deepflavor_cvsb", CvsB_vbf1}, {"vbfjet1_deepflavor_cvsl", CvsL_vbf1}, {"vbfjet1_hhbtag", HHbtag_vbf1},
            {"vbfjet2_pt", vbfjet2_pt}, {"vbfjet2_eta", vbfjet2_eta}, {"vbfjet2_phi", vbfjet2_phi}, {"vbfjet2_e", vbfjet2_e},
            {"vbfjet2_deepflavor_b", VBFjet2_btag_deepFlavor}, {"vbfjet2_deepflavor_cvsb", CvsB_vbf2}, {"vbfjet2_deepflavor_cvsl", CvsL_vbf2}, {"vbfjet2_hhbtag", HHbtag_vbf2},
            {"lep1_pt", dau1_pt}, {"lep1_eta", dau1_eta}, {"lep1_phi", dau1_phi}, {"lep1_e", dau1_e},
            {"lep2_pt", dau2_pt}, {"lep2_eta", dau2_eta}, {"lep2_phi", dau2_phi}, {"lep2_e", dau2_e},
            {"met_pt", met.Pt()}, {"met_phi", met.Phi()},
            {"bh_pt", (bjet1+bjet2).Pt()}, {"bh_eta", (bjet1+bjet2).Eta()}, {"bh_phi", (bjet1+bjet2).Phi()}, {"bh_e", (bjet1+bjet2).E()},
            {"tauh_sv_pt", tauH_SVFIT_pt}, {"tauh_sv_eta", tauH_SVFIT_eta}, {"tauh_sv_phi", tauH_SVFIT_phi}, {"tauh_sv_e", svfit.E()}
          });
        }
        auto mdnnSM0_score_new  = mci.predict(EventNumber, 0);
        auto mdnnSM1_score_new  = mci.predict(EventNumber, 1);
        auto mdnnBSM0_score_new = mci.predict(EventNumber, 2);
        for (uint k=0; k<mdnnSM0_score_new.size(); k++)
          mdnnSM0_output_new.at(k) = mdnnSM0_score_new.at(k).second;
        for (uint k=0; k<mdnnSM1_score_new.size(); k++)
          mdnnSM1_output_new.at(k) = mdnnSM1_score_new.at(k).second;
        for (uint k=0; k<mdnnBSM0_score_new.size(); k++)
          mdnnBSM0_output_new.at(k) = mdnnBSM0_score_new.at(k).second;
      }
      else /*isMC*/
      {
        // KinFit computation - Central value
        if (doKinFit)
        {
          // nominal kinfit
          HHKinFit2::HHKinFitMasterHeavyHiggs kinFits = HHKinFit2::HHKinFitMasterHeavyHiggs(bjet1, bjet2, tau1, tau2, ptmiss, stableMetCov, bjet1_JER, bjet2_JER) ;
          kinFits.addHypo(hypo_mh1,hypo_mh2);
          bool wrongHHK =false;
          try {kinFits.fit();}
          catch(HHKinFit2::HHInvMConstraintException   &e) {wrongHHK=true;}
          catch(HHKinFit2::HHEnergyConstraintException &e) {wrongHHK=true;}
          catch(HHKinFit2::HHEnergyRangeException      &e) {wrongHHK=true;}
          if(!wrongHHK)
          {
            HHKin_mass_new = kinFits.getMH();
            HHKin_chi2_new = kinFits.getChi2();
          }
          else
          {
            HHKin_mass_new = -333.;
            HHKin_chi2_new = 0.;
          }
        }

        // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
        // MT2 computation - Central value
        if (doMT2)
        {
          MT2_new = asymm_mt2_lester_bisect::get_mT2( bjet1.M(), bjet1.Px(), bjet1.Py(),
                                                      bjet2.M(), bjet2.Px(), bjet2.Py(),
                                                      (tau1.Px() + tau2.Px() + met.Px()),
                                                      (tau1.Py() + tau2.Py() + met.Py()),
                                                      tau1.M(), tau2.M(), desiredPrecisionOnMt2);
        }

        // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
        // SVfit computation - Central value
        if (doSVfit)
        {
          //std::cout << std::setprecision(5) << std::fixed;
          //std::cout << "pairType: " <<  pType << std::endl;
          //std::cout << "orig  tau1: " << dau1_pt << " " << dau1_eta << " " << dau1_phi << " " << dau1_e << std::endl;
          //std::cout << "input tau1: " << tau1.Pt() << " " << tau1.Eta() << " " << tau1.Phi() << " " << tau1.E() << " " << tau1.Px() << " " << tau1.Py()<< std::endl;
          //std::cout << "orig  tau2: " << dau2_pt << " " << dau2_eta << " " << dau2_phi << " " << dau2_e << std::endl;
          //std::cout << "input tau2: " << tau2.Pt() << " " << tau2.Eta() << " " << tau2.Phi() << " " << tau2.E() << " " << tau2.Px() << " " << tau2.Py()<< std::endl;
          //std::cout << "input met: " << met.Pt() << " " << met.Eta() << " " << met.Phi() << " " << met.Px() << " " << met.Py() << std::endl;

          SVfitKLUBinterface algo_central(0, tau1, tau2, met, stableMetCov, pType, DM1, DM2);
          std::vector<double> svfitRes = algo_central.FitAndGetResult();
          tauH_SVFIT_pt_new   = svfitRes.at(0);
          tauH_SVFIT_eta_new  = svfitRes.at(1);
          tauH_SVFIT_phi_new  = svfitRes.at(2);
          tauH_SVFIT_mass_new = svfitRes.at(3);
          //std::cout << " - newSVfit: " << tauH_SVFIT_pt_new << " " << tauH_SVFIT_eta_new << " " << tauH_SVFIT_phi_new << " " << tauH_SVFIT_mass_new << std::endl;
          //std::cout << " - oldSVfit: " << tauH_SVFIT_pt << " " << tauH_SVFIT_eta << " " << tauH_SVFIT_phi << " " << tauH_SVFIT_mass << std::endl;
        }

        // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
        // DNN computation - Central value
        if (doDNN)
        {
          // Set quantities that change for each event (shifted for TES, JES...)
          // Central value
          DNNreader.SetShiftedInputs(bjet1, bjet2, tau1, tau2, vbfjet1, vbfjet2, met, svfit,
              HHKin_mass, HHKin_chi2, KinFitConv, SVfitConv, MT2);
          std::vector<float> outs = DNNreader.GetPredictions();
          //std::cout << "----- ...gotten predictions: " << outs.at(0) << std::endl;
          DNNoutSM_kl_1_new = outs.at(0);
        }

        // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
        // Multiclass computation - Central value
        if (doMult)
        {
          mci.clearInputs();
          for (uint j=0; j<mci.getNumberOfModels(); j++)
          {
            mci.setInputs(j,
            {
              {"is_mutau", mdnn_isMuTau}, {"is_etau", mdnn_isETau}, {"is_tautau", mdnn_isTauTau},
              {"bjet1_pt", bjet1_pt}, {"bjet1_eta", bjet1_eta}, {"bjet1_phi", bjet1_phi}, {"bjet1_e", bjet1_e},
              {"bjet1_deepflavor_b", bjet1_bID_deepFlavor}, {"bjet1_deepflavor_cvsb", CvsB_b1}, {"bjet1_deepflavor_cvsl", CvsL_b1}, {"bjet1_hhbtag", HHbtag_b1},
              {"bjet2_pt", bjet2_pt}, {"bjet2_eta", bjet2_eta}, {"bjet2_phi", bjet2_phi}, {"bjet2_e", bjet2_e},
              {"bjet2_deepflavor_b", bjet2_bID_deepFlavor}, {"bjet2_deepflavor_cvsb", CvsB_b2}, {"bjet2_deepflavor_cvsl", CvsL_b2}, {"bjet2_hhbtag", HHbtag_b2},
              {"vbfjet1_pt", vbfjet1_pt}, {"vbfjet1_eta", vbfjet1_eta}, {"vbfjet1_phi", vbfjet1_phi}, {"vbfjet1_e", vbfjet1_e},
              {"vbfjet1_deepflavor_b", VBFjet1_btag_deepFlavor}, {"vbfjet1_deepflavor_cvsb", CvsB_vbf1}, {"vbfjet1_deepflavor_cvsl", CvsL_vbf1}, {"vbfjet1_hhbtag", HHbtag_vbf1},
              {"vbfjet2_pt", vbfjet2_pt}, {"vbfjet2_eta", vbfjet2_eta}, {"vbfjet2_phi", vbfjet2_phi}, {"vbfjet2_e", vbfjet2_e},
              {"vbfjet2_deepflavor_b", VBFjet2_btag_deepFlavor}, {"vbfjet2_deepflavor_cvsb", CvsB_vbf2}, {"vbfjet2_deepflavor_cvsl", CvsL_vbf2}, {"vbfjet2_hhbtag", HHbtag_vbf2},
              {"lep1_pt", dau1_pt}, {"lep1_eta", dau1_eta}, {"lep1_phi", dau1_phi}, {"lep1_e", dau1_e},
              {"lep2_pt", dau2_pt}, {"lep2_eta", dau2_eta}, {"lep2_phi", dau2_phi}, {"lep2_e", dau2_e},
              {"met_pt", met.Pt()}, {"met_phi", met.Phi()},
              {"bh_pt", (bjet1+bjet2).Pt()}, {"bh_eta", (bjet1+bjet2).Eta()}, {"bh_phi", (bjet1+bjet2).Phi()}, {"bh_e", (bjet1+bjet2).E()},
              {"tauh_sv_pt", tauH_SVFIT_pt}, {"tauh_sv_eta", tauH_SVFIT_eta}, {"tauh_sv_phi", tauH_SVFIT_phi}, {"tauh_sv_e", svfit.E()}
            });
          }
          auto mdnnSM0_score_new  = mci.predict(EventNumber, 0);
          auto mdnnSM1_score_new  = mci.predict(EventNumber, 1);
          auto mdnnBSM0_score_new = mci.predict(EventNumber, 2);
          for (uint k=0; k<mdnnSM0_score_new.size(); k++)
            mdnnSM0_output_new.at(k) = mdnnSM0_score_new.at(k).second;
          for (uint k=0; k<mdnnSM1_score_new.size(); k++)
            mdnnSM1_output_new.at(k) = mdnnSM1_score_new.at(k).second;
          for (uint k=0; k<mdnnBSM0_score_new.size(); k++)
            mdnnBSM0_output_new.at(k) = mdnnBSM0_score_new.at(k).second;
        }

        // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
        // BDT computation - Central value
        if (doBDT)
        {
          // Set inputs to BDT
          BDTreader.SetInputValues(bjet2.Pt(), (bjet1+bjet2).Pt(), tau1.Pt(),
            tau2.Pt(), svfit.Pt(), BDT_channel,
            BDT_HT20, pzeta, pzeta_vis, BDT_ditau_deltaPhi,
            BDT_tauHsvfitMet_deltaPhi, mT_tauH_MET, mTtot, MT2,
            BDT_MX, BDT_bH_tauH_MET_InvMass, BDT_bH_tauH_SVFIT_InvMass,
            BDT_bH_tauH_InvMass, HHKin_mass, HHKin_chi2, BDT_MET_bH_cosTheta);

          // Get BDT outputs
          std::vector<float> BDTouts = BDTreader.GetPredictions();
          BDToutSM_kl_1_new = BDTouts.at(0);
          //std::cout << " - newBDT: " << BDToutSM_kl_1_new << std::endl;
          //std::cout << " - oldBDT: " << BDToutSM_kl_1 << std::endl;
        }
      } // End isMC
    } // End doCentral
    // Timing info
    auto end_nomin = high_resolution_clock::now();

    // ---- ---- ---- ---- ---- ---- ----
    // ---- ---- Do all MES now ---- ----
    // ---- ---- ---- ---- ---- ---- ----
    // Timing info
    auto start_MES = high_resolution_clock::now();
    if (doMES)
    {
      if (isData)
      {
        // Fill with nominal value
        tauH_SVFIT_mass_muup   = tauH_SVFIT_mass;
        tauH_SVFIT_mass_mudown = tauH_SVFIT_mass;
        DNNoutSM_kl_1_muup     = DNNoutSM_kl_1;
        DNNoutSM_kl_1_mudown   = DNNoutSM_kl_1;
        BDToutSM_kl_1_muup     = BDToutSM_kl_1;
        BDToutSM_kl_1_mudown   = BDToutSM_kl_1;

        // VBF multiclass
        mci.clearInputs();
        for (uint j=0; j<mci.getNumberOfModels(); j++)
        {
          mci.setInputs(j,
          {
            {"is_mutau", mdnn_isMuTau}, {"is_etau", mdnn_isETau}, {"is_tautau", mdnn_isTauTau},
            {"bjet1_pt", bjet1_pt}, {"bjet1_eta", bjet1_eta}, {"bjet1_phi", bjet1_phi}, {"bjet1_e", bjet1_e},
            {"bjet1_deepflavor_b", bjet1_bID_deepFlavor}, {"bjet1_deepflavor_cvsb", CvsB_b1}, {"bjet1_deepflavor_cvsl", CvsL_b1}, {"bjet1_hhbtag", HHbtag_b1},
            {"bjet2_pt", bjet2_pt}, {"bjet2_eta", bjet2_eta}, {"bjet2_phi", bjet2_phi}, {"bjet2_e", bjet2_e},
            {"bjet2_deepflavor_b", bjet2_bID_deepFlavor}, {"bjet2_deepflavor_cvsb", CvsB_b2}, {"bjet2_deepflavor_cvsl", CvsL_b2}, {"bjet2_hhbtag", HHbtag_b2},
            {"vbfjet1_pt", vbfjet1_pt}, {"vbfjet1_eta", vbfjet1_eta}, {"vbfjet1_phi", vbfjet1_phi}, {"vbfjet1_e", vbfjet1_e},
            {"vbfjet1_deepflavor_b", VBFjet1_btag_deepFlavor}, {"vbfjet1_deepflavor_cvsb", CvsB_vbf1}, {"vbfjet1_deepflavor_cvsl", CvsL_vbf1}, {"vbfjet1_hhbtag", HHbtag_vbf1},
            {"vbfjet2_pt", vbfjet2_pt}, {"vbfjet2_eta", vbfjet2_eta}, {"vbfjet2_phi", vbfjet2_phi}, {"vbfjet2_e", vbfjet2_e},
            {"vbfjet2_deepflavor_b", VBFjet2_btag_deepFlavor}, {"vbfjet2_deepflavor_cvsb", CvsB_vbf2}, {"vbfjet2_deepflavor_cvsl", CvsL_vbf2}, {"vbfjet2_hhbtag", HHbtag_vbf2},
            {"lep1_pt", dau1_pt}, {"lep1_eta", dau1_eta}, {"lep1_phi", dau1_phi}, {"lep1_e", dau1_e},
            {"lep2_pt", dau2_pt}, {"lep2_eta", dau2_eta}, {"lep2_phi", dau2_phi}, {"lep2_e", dau2_e},
            {"met_pt", met.Pt()}, {"met_phi", met.Phi()},
            {"bh_pt", (bjet1+bjet2).Pt()}, {"bh_eta", (bjet1+bjet2).Eta()}, {"bh_phi", (bjet1+bjet2).Phi()}, {"bh_e", (bjet1+bjet2).E()},
            {"tauh_sv_pt", tauH_SVFIT_pt}, {"tauh_sv_eta", tauH_SVFIT_eta}, {"tauh_sv_phi", tauH_SVFIT_phi}, {"tauh_sv_e", svfit.E()}
          });
        }
        auto mdnnSM0_score_mes  = mci.predict(EventNumber, 0);
        auto mdnnSM1_score_mes  = mci.predict(EventNumber, 1);
        auto mdnnBSM0_score_mes = mci.predict(EventNumber, 2);
        for (uint k=0; k<mdnnSM0_score_mes.size(); k++)
        {
          mdnnSM0_output_muup.at(k)   = mdnnSM0_score_mes.at(k).second;
          mdnnSM0_output_mudown.at(k) = mdnnSM0_score_mes.at(k).second;
        }
        for (uint k=0; k<mdnnSM1_score_mes.size(); k++)
        {
          mdnnSM1_output_muup.at(k)   = mdnnSM1_score_mes.at(k).second;
          mdnnSM1_output_mudown.at(k) = mdnnSM1_score_mes.at(k).second;
        }
        for (uint k=0; k<mdnnBSM0_score_mes.size(); k++)
        {
          mdnnBSM0_output_muup.at(k)   = mdnnBSM0_score_mes.at(k).second;
          mdnnBSM0_output_mudown.at(k) = mdnnBSM0_score_mes.at(k).second;
        }
      }
      else /*isMC*/
      {
        // Build shifted taus and MET
        TLorentzVector tau1_muup, tau2_muup, met_muup;
        tau1_muup.SetPtEtaPhiM(dau1_pt_muup, dau1_eta, dau1_phi, dau1_mass_muup);
        tau2_muup.SetPtEtaPhiM(dau2_pt_muup, dau2_eta, dau2_phi, dau2_mass_muup);
        met_muup .SetPxPyPzE(METx_muup, METy_muup, 0, std::hypot(METx_muup, METy_muup));
        TVector2 ptmiss_muup = TVector2(METx_muup, METy_muup);

        TLorentzVector tau1_mudown, tau2_mudown, met_mudown;
        tau1_mudown.SetPtEtaPhiM(dau1_pt_mudown, dau1_eta, dau1_phi, dau1_mass_mudown);
        tau2_mudown.SetPtEtaPhiM(dau2_pt_mudown, dau2_eta, dau2_phi, dau2_mass_mudown);
        met_mudown .SetPxPyPzE(METx_mudown, METy_mudown, 0, std::hypot(METx_mudown, METy_mudown));
        TVector2 ptmiss_mudown = TVector2(METx_mudown, METy_mudown);

        // Declare other useful shifted variables
        float HHKin_mass_muup, HHKin_chi2_muup, HHKin_mass_mudown, HHKin_chi2_mudown;
        float MT2_muup, MT2_mudown;
        float tauH_SVFIT_pt_muup, tauH_SVFIT_eta_muup, tauH_SVFIT_phi_muup;
        float tauH_SVFIT_pt_mudown, tauH_SVFIT_eta_mudown, tauH_SVFIT_phi_mudown;

        if (doKinFit)
        {
          // MES variations
          HHKinFit2::HHKinFitMasterHeavyHiggs kinFits_muup = HHKinFit2::HHKinFitMasterHeavyHiggs(bjet1, bjet2, tau1_muup, tau2_muup, ptmiss_muup, stableMetCov, bjet1_JER, bjet2_JER) ;
          kinFits_muup.addHypo(hypo_mh1,hypo_mh2);
          bool wrongHHK_muup =false;
          try {kinFits_muup.fit();}
          catch(HHKinFit2::HHInvMConstraintException   &e) {wrongHHK_muup=true;}
          catch(HHKinFit2::HHEnergyConstraintException &e) {wrongHHK_muup=true;}
          catch(HHKinFit2::HHEnergyRangeException      &e) {wrongHHK_muup=true;}
          if(!wrongHHK_muup)
          {
            HHKin_mass_muup = kinFits_muup.getMH();
            HHKin_chi2_muup = kinFits_muup.getChi2();
          }
          else
          {
            HHKin_mass_muup = -333.;
            HHKin_chi2_muup = 0.;
          }

          HHKinFit2::HHKinFitMasterHeavyHiggs kinFits_mudown = HHKinFit2::HHKinFitMasterHeavyHiggs(bjet1, bjet2, tau1_mudown, tau2_mudown, ptmiss_mudown, stableMetCov, bjet1_JER, bjet2_JER) ;
          kinFits_mudown.addHypo(hypo_mh1,hypo_mh2);
          bool wrongHHK_mudown =false;
          try {kinFits_mudown.fit();}
          catch(HHKinFit2::HHInvMConstraintException   &e) {wrongHHK_mudown=true;}
          catch(HHKinFit2::HHEnergyConstraintException &e) {wrongHHK_mudown=true;}
          catch(HHKinFit2::HHEnergyRangeException      &e) {wrongHHK_mudown=true;}
          if(!wrongHHK_mudown)
          {
            HHKin_mass_mudown = kinFits_mudown.getMH();
            HHKin_chi2_mudown = kinFits_mudown.getChi2();
          }
          else
          {
            HHKin_mass_mudown = -333.;
            HHKin_chi2_mudown = 0.;
          }
        }

        if (doMT2)
        {
          MT2_muup = asymm_mt2_lester_bisect::get_mT2( bjet1.M(), bjet1.Px(), bjet1.Py(),
                                                        bjet2.M(), bjet2.Px(), bjet2.Py(),
                                                        (tau1_muup.Px() + tau2_muup.Px() + met_muup.Px()),
                                                        (tau1_muup.Py() + tau2_muup.Py() + met_muup.Py()),
                                                        tau1_muup.M(), tau2_muup.M(), desiredPrecisionOnMt2);

          MT2_mudown = asymm_mt2_lester_bisect::get_mT2( bjet1.M(), bjet1.Px(), bjet1.Py(),
                                                          bjet2.M(), bjet2.Px(), bjet2.Py(),
                                                          (tau1_mudown.Px() + tau2_mudown.Px() + met_mudown.Px()),
                                                          (tau1_mudown.Py() + tau2_mudown.Py() + met_mudown.Py()),
                                                          tau1_mudown.M(), tau2_mudown.M(), desiredPrecisionOnMt2);
        }

        if (doSVfit)
        {
          SVfitKLUBinterface algo_muup(0, tau1_muup, tau2_muup, met_muup, stableMetCov, pType, DM1, DM2);
          std::vector<double> svfitRes_muup = algo_muup.FitAndGetResult();
          tauH_SVFIT_pt_muup   = svfitRes_muup.at(0);
          tauH_SVFIT_eta_muup  = svfitRes_muup.at(1);
          tauH_SVFIT_phi_muup  = svfitRes_muup.at(2);
          tauH_SVFIT_mass_muup = svfitRes_muup.at(3);

          SVfitKLUBinterface algo_mudown(0, tau1_mudown, tau2_mudown, met_mudown, stableMetCov, pType, DM1, DM2);
          std::vector<double> svfitRes_mudown = algo_mudown.FitAndGetResult();
          tauH_SVFIT_pt_mudown   = svfitRes_mudown.at(0);
          tauH_SVFIT_eta_mudown  = svfitRes_mudown.at(1);
          tauH_SVFIT_phi_mudown  = svfitRes_mudown.at(2);
          tauH_SVFIT_mass_mudown = svfitRes_mudown.at(3);
        }

        // --- --- --- MES DNN/BDT quantities --- --- ---
        TLorentzVector svfit_muup;
        svfit_muup.SetPtEtaPhiM(tauH_SVFIT_pt_muup, tauH_SVFIT_eta_muup, tauH_SVFIT_phi_muup, tauH_SVFIT_mass_muup);
        float mTtot_muup                         = Calculate_TotalMT(tau1_muup, tau2_muup, met_muup);
        float pzeta_vis_muup                     = Calculate_visiblePzeta(tau1_muup, tau2_muup);
        float pzeta_muup                         = Calculate_Pzeta(tau1_muup, tau2_muup, met_muup);
        float BDT_ditau_deltaPhi_muup            = ROOT::Math::VectorUtil::DeltaPhi(tau1_muup, tau2_muup);
        float BDT_tauHsvfitMet_deltaPhi_muup     = ROOT::Math::VectorUtil::DeltaPhi(svfit_muup, met_muup);
        float mT_tauH_MET_muup                   = Calculate_MT( (tau1_muup+tau2_muup)+met_muup, met_muup);
        float BDT_MX_muup                        = Calculate_MX(tau1_muup, tau2_muup, bjet1, bjet2, met_muup);
        float BDT_bH_tauH_MET_InvMass_muup       = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_muup+tau2_muup)+met_muup);
        float BDT_bH_tauH_SVFIT_InvMass_muup     = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), svfit_muup);
        float BDT_bH_tauH_InvMass_muup           = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_muup+tau2_muup));
        float BDT_MET_bH_cosTheta_muup           = Calculate_cosTheta_2bodies(getLVfromTLV(met_muup), getLVfromTLV(bjet1+bjet2));
        bool KinFitConv_muup                     = HHKin_chi2_muup > 0;
        bool SVfitConv_muup                      = tauH_SVFIT_mass_muup > 0;

        TLorentzVector svfit_mudown;
        svfit_mudown.SetPtEtaPhiM(tauH_SVFIT_pt_mudown, tauH_SVFIT_eta_mudown, tauH_SVFIT_phi_mudown, tauH_SVFIT_mass_mudown);
        float mTtot_mudown                         = Calculate_TotalMT(tau1_mudown, tau2_mudown, met_mudown);
        float pzeta_vis_mudown                     = Calculate_visiblePzeta(tau1_mudown, tau2_mudown);
        float pzeta_mudown                         = Calculate_Pzeta(tau1_mudown, tau2_mudown, met_mudown);
        float BDT_ditau_deltaPhi_mudown            = ROOT::Math::VectorUtil::DeltaPhi(tau1_mudown, tau2_mudown);
        float BDT_tauHsvfitMet_deltaPhi_mudown     = ROOT::Math::VectorUtil::DeltaPhi(svfit_mudown, met_mudown);
        float mT_tauH_MET_mudown                   = Calculate_MT( (tau1_mudown+tau2_mudown)+met_mudown, met_mudown);
        float BDT_MX_mudown                        = Calculate_MX(tau1_mudown, tau2_mudown, bjet1, bjet2, met_mudown);
        float BDT_bH_tauH_MET_InvMass_mudown       = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_mudown+tau2_mudown)+met_mudown);
        float BDT_bH_tauH_SVFIT_InvMass_mudown     = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), svfit_mudown);
        float BDT_bH_tauH_InvMass_mudown           = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_mudown+tau2_mudown));
        float BDT_MET_bH_cosTheta_mudown           = Calculate_cosTheta_2bodies(getLVfromTLV(met_mudown), getLVfromTLV(bjet1+bjet2));
        bool KinFitConv_mudown                     = HHKin_chi2_mudown > 0;
        bool SVfitConv_mudown                      = tauH_SVFIT_mass_mudown > 0;

        if (doDNN)
        {
          DNNreader.SetShiftedInputs(bjet1, bjet2, tau1_muup, tau2_muup, vbfjet1, vbfjet2, met_muup, svfit_muup,
              HHKin_mass_muup, HHKin_chi2_muup, KinFitConv_muup, SVfitConv_muup, MT2_muup);
          std::vector<float> outs_muup = DNNreader.GetPredictions();
          DNNoutSM_kl_1_muup = outs_muup.at(0);

          DNNreader.SetShiftedInputs(bjet1, bjet2, tau1_mudown, tau2_mudown, vbfjet1, vbfjet2, met_mudown, svfit_mudown,
              HHKin_mass_mudown, HHKin_chi2_mudown, KinFitConv_mudown, SVfitConv_mudown, MT2_mudown);
          std::vector<float> outs_mudown = DNNreader.GetPredictions();
          DNNoutSM_kl_1_mudown = outs_mudown.at(0);
        }

        if (doMult)
        {
          mci.clearInputs();
          for (uint j=0; j<mci.getNumberOfModels(); j++)
          {
            mci.setInputs(j,
            {
              {"is_mutau", mdnn_isMuTau}, {"is_etau", mdnn_isETau}, {"is_tautau", mdnn_isTauTau},
              {"bjet1_pt", bjet1_pt}, {"bjet1_eta", bjet1_eta}, {"bjet1_phi", bjet1_phi}, {"bjet1_e", bjet1_e},
              {"bjet1_deepflavor_b", bjet1_bID_deepFlavor}, {"bjet1_deepflavor_cvsb", CvsB_b1}, {"bjet1_deepflavor_cvsl", CvsL_b1}, {"bjet1_hhbtag", HHbtag_b1},
              {"bjet2_pt", bjet2_pt}, {"bjet2_eta", bjet2_eta}, {"bjet2_phi", bjet2_phi}, {"bjet2_e", bjet2_e},
              {"bjet2_deepflavor_b", bjet2_bID_deepFlavor}, {"bjet2_deepflavor_cvsb", CvsB_b2}, {"bjet2_deepflavor_cvsl", CvsL_b2}, {"bjet2_hhbtag", HHbtag_b2},
              {"vbfjet1_pt", vbfjet1_pt}, {"vbfjet1_eta", vbfjet1_eta}, {"vbfjet1_phi", vbfjet1_phi}, {"vbfjet1_e", vbfjet1_e},
              {"vbfjet1_deepflavor_b", VBFjet1_btag_deepFlavor}, {"vbfjet1_deepflavor_cvsb", CvsB_vbf1}, {"vbfjet1_deepflavor_cvsl", CvsL_vbf1}, {"vbfjet1_hhbtag", HHbtag_vbf1},
              {"vbfjet2_pt", vbfjet2_pt}, {"vbfjet2_eta", vbfjet2_eta}, {"vbfjet2_phi", vbfjet2_phi}, {"vbfjet2_e", vbfjet2_e},
              {"vbfjet2_deepflavor_b", VBFjet2_btag_deepFlavor}, {"vbfjet2_deepflavor_cvsb", CvsB_vbf2}, {"vbfjet2_deepflavor_cvsl", CvsL_vbf2}, {"vbfjet2_hhbtag", HHbtag_vbf2},
              {"lep1_pt", tau1_muup.Pt()}, {"lep1_eta", tau1_muup.Eta()}, {"lep1_phi", tau1_muup.Phi()}, {"lep1_e", tau1_muup.E()},
              {"lep2_pt", tau2_muup.Pt()}, {"lep2_eta", tau2_muup.Eta()}, {"lep2_phi", tau2_muup.Phi()}, {"lep2_e", tau2_muup.E()},
              {"met_pt", met_muup.Pt()}, {"met_phi", met_muup.Phi()},
              {"bh_pt", (bjet1+bjet2).Pt()}, {"bh_eta", (bjet1+bjet2).Eta()}, {"bh_phi", (bjet1+bjet2).Phi()}, {"bh_e", (bjet1+bjet2).E()},
              {"tauh_sv_pt", svfit_muup.Pt()}, {"tauh_sv_eta", svfit_muup.Eta()}, {"tauh_sv_phi", svfit_muup.Phi()}, {"tauh_sv_e", svfit_muup.E()}
            });
          }
          auto mdnnSM0_score_muup  = mci.predict(EventNumber, 0);
          auto mdnnSM1_score_muup  = mci.predict(EventNumber, 1);
          auto mdnnBSM0_score_muup = mci.predict(EventNumber, 2);
          for (uint k=0; k<mdnnSM0_score_muup.size(); k++)
            mdnnSM0_output_muup.at(k) = mdnnSM0_score_muup.at(k).second;
          for (uint k=0; k<mdnnSM1_score_muup.size(); k++)
            mdnnSM1_output_muup.at(k) = mdnnSM1_score_muup.at(k).second;
          for (uint k=0; k<mdnnBSM0_score_muup.size(); k++)
            mdnnBSM0_output_muup.at(k) = mdnnBSM0_score_muup.at(k).second;

          mci.clearInputs();
          for (uint j=0; j<mci.getNumberOfModels(); j++)
          {
            mci.setInputs(j,
            {
              {"is_mutau", mdnn_isMuTau}, {"is_etau", mdnn_isETau}, {"is_tautau", mdnn_isTauTau},
              {"bjet1_pt", bjet1_pt}, {"bjet1_eta", bjet1_eta}, {"bjet1_phi", bjet1_phi}, {"bjet1_e", bjet1_e},
              {"bjet1_deepflavor_b", bjet1_bID_deepFlavor}, {"bjet1_deepflavor_cvsb", CvsB_b1}, {"bjet1_deepflavor_cvsl", CvsL_b1}, {"bjet1_hhbtag", HHbtag_b1},
              {"bjet2_pt", bjet2_pt}, {"bjet2_eta", bjet2_eta}, {"bjet2_phi", bjet2_phi}, {"bjet2_e", bjet2_e},
              {"bjet2_deepflavor_b", bjet2_bID_deepFlavor}, {"bjet2_deepflavor_cvsb", CvsB_b2}, {"bjet2_deepflavor_cvsl", CvsL_b2}, {"bjet2_hhbtag", HHbtag_b2},
              {"vbfjet1_pt", vbfjet1_pt}, {"vbfjet1_eta", vbfjet1_eta}, {"vbfjet1_phi", vbfjet1_phi}, {"vbfjet1_e", vbfjet1_e},
              {"vbfjet1_deepflavor_b", VBFjet1_btag_deepFlavor}, {"vbfjet1_deepflavor_cvsb", CvsB_vbf1}, {"vbfjet1_deepflavor_cvsl", CvsL_vbf1}, {"vbfjet1_hhbtag", HHbtag_vbf1},
              {"vbfjet2_pt", vbfjet2_pt}, {"vbfjet2_eta", vbfjet2_eta}, {"vbfjet2_phi", vbfjet2_phi}, {"vbfjet2_e", vbfjet2_e},
              {"vbfjet2_deepflavor_b", VBFjet2_btag_deepFlavor}, {"vbfjet2_deepflavor_cvsb", CvsB_vbf2}, {"vbfjet2_deepflavor_cvsl", CvsL_vbf2}, {"vbfjet2_hhbtag", HHbtag_vbf2},
              {"lep1_pt", tau1_mudown.Pt()}, {"lep1_eta", tau1_mudown.Eta()}, {"lep1_phi", tau1_mudown.Phi()}, {"lep1_e", tau1_mudown.E()},
              {"lep2_pt", tau2_mudown.Pt()}, {"lep2_eta", tau2_mudown.Eta()}, {"lep2_phi", tau2_mudown.Phi()}, {"lep2_e", tau2_mudown.E()},
              {"met_pt", met_mudown.Pt()}, {"met_phi", met_mudown.Phi()},
              {"bh_pt", (bjet1+bjet2).Pt()}, {"bh_eta", (bjet1+bjet2).Eta()}, {"bh_phi", (bjet1+bjet2).Phi()}, {"bh_e", (bjet1+bjet2).E()},
              {"tauh_sv_pt", svfit_mudown.Pt()}, {"tauh_sv_eta", svfit_mudown.Eta()}, {"tauh_sv_phi", svfit_mudown.Phi()}, {"tauh_sv_e", svfit_mudown.E()}
            });
          }
          auto mdnnSM0_score_mudown  = mci.predict(EventNumber, 0);
          auto mdnnSM1_score_mudown  = mci.predict(EventNumber, 1);
          auto mdnnBSM0_score_mudown = mci.predict(EventNumber, 2);
          for (uint k=0; k<mdnnSM0_score_mudown.size(); k++)
            mdnnSM0_output_mudown.at(k) = mdnnSM0_score_mudown.at(k).second;
          for (uint k=0; k<mdnnSM1_score_mudown.size(); k++)
            mdnnSM1_output_mudown.at(k) = mdnnSM1_score_mudown.at(k).second;
          for (uint k=0; k<mdnnBSM0_score_mudown.size(); k++)
            mdnnBSM0_output_mudown.at(k) = mdnnBSM0_score_mudown.at(k).second;
        }

        if (doBDT)
        {
          BDTreader.SetInputValues(bjet2.Pt(), (bjet1+bjet2).Pt(), tau1_muup.Pt(),
            tau2_muup.Pt(), svfit_muup.Pt(), BDT_channel,
            BDT_HT20, pzeta_muup, pzeta_vis_muup, BDT_ditau_deltaPhi_muup,
            BDT_tauHsvfitMet_deltaPhi_muup, mT_tauH_MET_muup, mTtot_muup, MT2_muup,
            BDT_MX_muup, BDT_bH_tauH_MET_InvMass_muup, BDT_bH_tauH_SVFIT_InvMass_muup,
            BDT_bH_tauH_InvMass_muup, HHKin_mass_muup, HHKin_chi2_muup, BDT_MET_bH_cosTheta_muup);
          std::vector<float> BDTouts_muup = BDTreader.GetPredictions();
          BDToutSM_kl_1_muup = BDTouts_muup.at(0);

          BDTreader.SetInputValues(bjet2.Pt(), (bjet1+bjet2).Pt(), tau1_mudown.Pt(),
            tau2_mudown.Pt(), svfit_mudown.Pt(), BDT_channel,
            BDT_HT20, pzeta_mudown, pzeta_vis_mudown, BDT_ditau_deltaPhi_mudown,
            BDT_tauHsvfitMet_deltaPhi_mudown, mT_tauH_MET_mudown, mTtot_mudown, MT2_mudown,
            BDT_MX_mudown, BDT_bH_tauH_MET_InvMass_mudown, BDT_bH_tauH_SVFIT_InvMass_mudown,
            BDT_bH_tauH_InvMass_mudown, HHKin_mass_mudown, HHKin_chi2_mudown, BDT_MET_bH_cosTheta_mudown);
          std::vector<float> BDTouts_mudown = BDTreader.GetPredictions();
          BDToutSM_kl_1_mudown = BDTouts_mudown.at(0);
        }
      } // End isMC
    }  // End doMES
    // Timing info
    auto end_MES = high_resolution_clock::now();

    // ---- ---- ---- ---- ---- ---- ----
    // ---- ---- Do all EES now ---- ----
    // ---- ---- ---- ---- ---- ---- ----
    // Timing info
    auto start_EES = high_resolution_clock::now();
    if (doEES)
    {
      for (int i=0; i<N_tauhDM_EES; i++)
      {
        if (isData)
        {
          // Fill with nominal value
          tauH_SVFIT_mass_eleup.at(i)   = tauH_SVFIT_mass;
          tauH_SVFIT_mass_eledown.at(i) = tauH_SVFIT_mass;
          DNNoutSM_kl_1_eleup.at(i)     = DNNoutSM_kl_1;
          DNNoutSM_kl_1_eledown.at(i)   = DNNoutSM_kl_1;
          BDToutSM_kl_1_eleup.at(i)     = BDToutSM_kl_1;
          BDToutSM_kl_1_eledown.at(i)   = BDToutSM_kl_1;

          // VBF multiclass
          mci.clearInputs();
          for (uint j=0; j<mci.getNumberOfModels(); j++)
          {
            mci.setInputs(j,
            {
              {"is_mutau", mdnn_isMuTau}, {"is_etau", mdnn_isETau}, {"is_tautau", mdnn_isTauTau},
              {"bjet1_pt", bjet1_pt}, {"bjet1_eta", bjet1_eta}, {"bjet1_phi", bjet1_phi}, {"bjet1_e", bjet1_e},
              {"bjet1_deepflavor_b", bjet1_bID_deepFlavor}, {"bjet1_deepflavor_cvsb", CvsB_b1}, {"bjet1_deepflavor_cvsl", CvsL_b1}, {"bjet1_hhbtag", HHbtag_b1},
              {"bjet2_pt", bjet2_pt}, {"bjet2_eta", bjet2_eta}, {"bjet2_phi", bjet2_phi}, {"bjet2_e", bjet2_e},
              {"bjet2_deepflavor_b", bjet2_bID_deepFlavor}, {"bjet2_deepflavor_cvsb", CvsB_b2}, {"bjet2_deepflavor_cvsl", CvsL_b2}, {"bjet2_hhbtag", HHbtag_b2},
              {"vbfjet1_pt", vbfjet1_pt}, {"vbfjet1_eta", vbfjet1_eta}, {"vbfjet1_phi", vbfjet1_phi}, {"vbfjet1_e", vbfjet1_e},
              {"vbfjet1_deepflavor_b", VBFjet1_btag_deepFlavor}, {"vbfjet1_deepflavor_cvsb", CvsB_vbf1}, {"vbfjet1_deepflavor_cvsl", CvsL_vbf1}, {"vbfjet1_hhbtag", HHbtag_vbf1},
              {"vbfjet2_pt", vbfjet2_pt}, {"vbfjet2_eta", vbfjet2_eta}, {"vbfjet2_phi", vbfjet2_phi}, {"vbfjet2_e", vbfjet2_e},
              {"vbfjet2_deepflavor_b", VBFjet2_btag_deepFlavor}, {"vbfjet2_deepflavor_cvsb", CvsB_vbf2}, {"vbfjet2_deepflavor_cvsl", CvsL_vbf2}, {"vbfjet2_hhbtag", HHbtag_vbf2},
              {"lep1_pt", dau1_pt}, {"lep1_eta", dau1_eta}, {"lep1_phi", dau1_phi}, {"lep1_e", dau1_e},
              {"lep2_pt", dau2_pt}, {"lep2_eta", dau2_eta}, {"lep2_phi", dau2_phi}, {"lep2_e", dau2_e},
              {"met_pt", met.Pt()}, {"met_phi", met.Phi()},
              {"bh_pt", (bjet1+bjet2).Pt()}, {"bh_eta", (bjet1+bjet2).Eta()}, {"bh_phi", (bjet1+bjet2).Phi()}, {"bh_e", (bjet1+bjet2).E()},
              {"tauh_sv_pt", tauH_SVFIT_pt}, {"tauh_sv_eta", tauH_SVFIT_eta}, {"tauh_sv_phi", tauH_SVFIT_phi}, {"tauh_sv_e", svfit.E()}
            });
          }
          auto mdnnSM0_score_ees  = mci.predict(EventNumber, 0);
          auto mdnnSM1_score_ees  = mci.predict(EventNumber, 1);
          auto mdnnBSM0_score_ees = mci.predict(EventNumber, 2);
          for (uint k=0; k<mdnnSM0_score_ees.size(); k++)
          {
            mdnnSM0_output_eleup  [i].at(k) = mdnnSM0_score_ees.at(k).second;
            mdnnSM0_output_eledown[i].at(k) = mdnnSM0_score_ees.at(k).second;
          }
          for (uint k=0; k<mdnnSM1_score_ees.size(); k++)
          {
            mdnnSM1_output_eleup  [i].at(k) = mdnnSM1_score_ees.at(k).second;
            mdnnSM1_output_eledown[i].at(k) = mdnnSM1_score_ees.at(k).second;
          }
          for (uint k=0; k<mdnnBSM0_score_ees.size(); k++)
          {
            mdnnBSM0_output_eleup  [i].at(k) = mdnnBSM0_score_ees.at(k).second;
            mdnnBSM0_output_eledown[i].at(k) = mdnnBSM0_score_ees.at(k).second;
          }
        }
        else /*isMC*/
        {
          // Build shifted taus and MET
          TLorentzVector tau1_eleup, tau2_eleup, met_eleup;
          tau1_eleup.SetPtEtaPhiM(dau1_pt_eleup->at(i), dau1_eta, dau1_phi, dau1_mass_eleup->at(i));
          tau2_eleup.SetPtEtaPhiM(dau2_pt_eleup->at(i), dau2_eta, dau2_phi, dau2_mass_eleup->at(i));
          met_eleup .SetPxPyPzE(METx_eleup->at(i), METy_eleup->at(i), 0, std::hypot(METx_eleup->at(i), METy_eleup->at(i)));
          TVector2 ptmiss_eleup = TVector2(METx_eleup->at(i), METy_eleup->at(i));

          TLorentzVector tau1_eledown, tau2_eledown, met_eledown;
          tau1_eledown.SetPtEtaPhiM(dau1_pt_eledown->at(i), dau1_eta, dau1_phi, dau1_mass_eledown->at(i));
          tau2_eledown.SetPtEtaPhiM(dau2_pt_eledown->at(i), dau2_eta, dau2_phi, dau2_mass_eledown->at(i));
          met_eledown .SetPxPyPzE(METx_eledown->at(i), METy_eledown->at(i), 0, std::hypot(METx_eledown->at(i), METy_eledown->at(i)));
          TVector2 ptmiss_eledown = TVector2(METx_eledown->at(i), METy_eledown->at(i));

          // Declare other useful shifted variables
          float HHKin_mass_eleup, HHKin_chi2_eleup, HHKin_mass_eledown, HHKin_chi2_eledown;
          float MT2_eleup, MT2_eledown;
          float tauH_SVFIT_pt_eleup, tauH_SVFIT_eta_eleup, tauH_SVFIT_phi_eleup;
          float tauH_SVFIT_pt_eledown, tauH_SVFIT_eta_eledown, tauH_SVFIT_phi_eledown;

          if (doKinFit)
          {
            HHKinFit2::HHKinFitMasterHeavyHiggs kinFits_eleup = HHKinFit2::HHKinFitMasterHeavyHiggs(bjet1, bjet2, tau1_eleup, tau2_eleup, ptmiss_eleup, stableMetCov, bjet1_JER, bjet2_JER) ;
            kinFits_eleup.addHypo(hypo_mh1,hypo_mh2);
            bool wrongHHK_eleup =false;
            try {kinFits_eleup.fit();}
            catch(HHKinFit2::HHInvMConstraintException   &e) {wrongHHK_eleup=true;}
            catch(HHKinFit2::HHEnergyConstraintException &e) {wrongHHK_eleup=true;}
            catch(HHKinFit2::HHEnergyRangeException      &e) {wrongHHK_eleup=true;}
            if(!wrongHHK_eleup)
            {
              HHKin_mass_eleup = kinFits_eleup.getMH();
              HHKin_chi2_eleup = kinFits_eleup.getChi2();
            }
            else
            {
              HHKin_mass_eleup = -333.;
              HHKin_chi2_eleup = 0.;
            }

            HHKinFit2::HHKinFitMasterHeavyHiggs kinFits_eledown = HHKinFit2::HHKinFitMasterHeavyHiggs(bjet1, bjet2, tau1_eledown, tau2_eledown, ptmiss_eledown, stableMetCov, bjet1_JER, bjet2_JER) ;
            kinFits_eledown.addHypo(hypo_mh1,hypo_mh2);
            bool wrongHHK_eledown =false;
            try {kinFits_eledown.fit();}
            catch(HHKinFit2::HHInvMConstraintException   &e) {wrongHHK_eledown=true;}
            catch(HHKinFit2::HHEnergyConstraintException &e) {wrongHHK_eledown=true;}
            catch(HHKinFit2::HHEnergyRangeException      &e) {wrongHHK_eledown=true;}
            if(!wrongHHK_eledown)
            {
              HHKin_mass_eledown = kinFits_eledown.getMH();
              HHKin_chi2_eledown = kinFits_eledown.getChi2();
            }
            else
            {
              HHKin_mass_eledown = -333.;
              HHKin_chi2_eledown = 0.;
            }
          }

          if (doMT2)
          {
            MT2_eleup = asymm_mt2_lester_bisect::get_mT2( bjet1.M(), bjet1.Px(), bjet1.Py(),
                                                          bjet2.M(), bjet2.Px(), bjet2.Py(),
                                                          (tau1_eleup.Px() + tau2_eleup.Px() + met_eleup.Px()),
                                                          (tau1_eleup.Py() + tau2_eleup.Py() + met_eleup.Py()),
                                                          tau1_eleup.M(), tau2_eleup.M(), desiredPrecisionOnMt2);

            MT2_eledown = asymm_mt2_lester_bisect::get_mT2( bjet1.M(), bjet1.Px(), bjet1.Py(),
                                                            bjet2.M(), bjet2.Px(), bjet2.Py(),
                                                            (tau1_eledown.Px() + tau2_eledown.Px() + met_eledown.Px()),
                                                            (tau1_eledown.Py() + tau2_eledown.Py() + met_eledown.Py()),
                                                            tau1_eledown.M(), tau2_eledown.M(), desiredPrecisionOnMt2);
          }

          if (doSVfit)
          {
            SVfitKLUBinterface algo_eleup(0, tau1_eleup, tau2_eleup, met_eleup, stableMetCov, pType, DM1, DM2);
            std::vector<double> svfitRes_eleup = algo_eleup.FitAndGetResult();
            tauH_SVFIT_pt_eleup         = svfitRes_eleup.at(0);
            tauH_SVFIT_eta_eleup        = svfitRes_eleup.at(1);
            tauH_SVFIT_phi_eleup        = svfitRes_eleup.at(2);
            tauH_SVFIT_mass_eleup.at(i) = svfitRes_eleup.at(3);

            SVfitKLUBinterface algo_eledown(0, tau1_eledown, tau2_eledown, met_eledown, stableMetCov, pType, DM1, DM2);
            std::vector<double> svfitRes_eledown = algo_eledown.FitAndGetResult();
            tauH_SVFIT_pt_eledown         = svfitRes_eledown.at(0);
            tauH_SVFIT_eta_eledown        = svfitRes_eledown.at(1);
            tauH_SVFIT_phi_eledown        = svfitRes_eledown.at(2);
            tauH_SVFIT_mass_eledown.at(i) = svfitRes_eledown.at(3);
          }

          // --- --- --- EES DNN/BDT quantities --- --- ---
          TLorentzVector svfit_eleup;
          svfit_eleup.SetPtEtaPhiM(tauH_SVFIT_pt_eleup, tauH_SVFIT_eta_eleup, tauH_SVFIT_phi_eleup, tauH_SVFIT_mass_eleup.at(i));
          float mTtot_eleup                         = Calculate_TotalMT(tau1_eleup, tau2_eleup, met_eleup);
          float pzeta_vis_eleup                     = Calculate_visiblePzeta(tau1_eleup, tau2_eleup);
          float pzeta_eleup                         = Calculate_Pzeta(tau1_eleup, tau2_eleup, met_eleup);
          float BDT_ditau_deltaPhi_eleup            = ROOT::Math::VectorUtil::DeltaPhi(tau1_eleup, tau2_eleup);
          float BDT_tauHsvfitMet_deltaPhi_eleup     = ROOT::Math::VectorUtil::DeltaPhi(svfit_eleup, met_eleup);
          float mT_tauH_MET_eleup                   = Calculate_MT( (tau1_eleup+tau2_eleup)+met_eleup, met_eleup);
          float BDT_MX_eleup                        = Calculate_MX(tau1_eleup, tau2_eleup, bjet1, bjet2, met_eleup);
          float BDT_bH_tauH_MET_InvMass_eleup       = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_eleup+tau2_eleup)+met_eleup);
          float BDT_bH_tauH_SVFIT_InvMass_eleup     = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), svfit_eleup);
          float BDT_bH_tauH_InvMass_eleup           = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_eleup+tau2_eleup));
          float BDT_MET_bH_cosTheta_eleup           = Calculate_cosTheta_2bodies(getLVfromTLV(met_eleup), getLVfromTLV(bjet1+bjet2));
          bool KinFitConv_eleup                     = HHKin_chi2_eleup > 0;
          bool SVfitConv_eleup                      = tauH_SVFIT_mass_eleup.at(i) > 0;

          TLorentzVector svfit_eledown;
          svfit_eledown.SetPtEtaPhiM(tauH_SVFIT_pt_eledown, tauH_SVFIT_eta_eledown, tauH_SVFIT_phi_eledown, tauH_SVFIT_mass_eledown.at(i));
          float mTtot_eledown                         = Calculate_TotalMT(tau1_eledown, tau2_eledown, met_eledown);
          float pzeta_vis_eledown                     = Calculate_visiblePzeta(tau1_eledown, tau2_eledown);
          float pzeta_eledown                         = Calculate_Pzeta(tau1_eledown, tau2_eledown, met_eledown);
          float BDT_ditau_deltaPhi_eledown            = ROOT::Math::VectorUtil::DeltaPhi(tau1_eledown, tau2_eledown);
          float BDT_tauHsvfitMet_deltaPhi_eledown     = ROOT::Math::VectorUtil::DeltaPhi(svfit_eledown, met_eledown);
          float mT_tauH_MET_eledown                   = Calculate_MT( (tau1_eledown+tau2_eledown)+met_eledown, met_eledown);
          float BDT_MX_eledown                        = Calculate_MX(tau1_eledown, tau2_eledown, bjet1, bjet2, met_eledown);
          float BDT_bH_tauH_MET_InvMass_eledown       = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_eledown+tau2_eledown)+met_eledown);
          float BDT_bH_tauH_SVFIT_InvMass_eledown     = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), svfit_eledown);
          float BDT_bH_tauH_InvMass_eledown           = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_eledown+tau2_eledown));
          float BDT_MET_bH_cosTheta_eledown           = Calculate_cosTheta_2bodies(getLVfromTLV(met_eledown), getLVfromTLV(bjet1+bjet2));
          bool KinFitConv_eledown                     = HHKin_chi2_eledown > 0;
          bool SVfitConv_eledown                      = tauH_SVFIT_mass_eledown.at(i) > 0;

          if (doDNN)
          {
            DNNreader.SetShiftedInputs(bjet1, bjet2, tau1_eleup, tau2_eleup, vbfjet1, vbfjet2, met_eleup, svfit_eleup,
                HHKin_mass_eleup, HHKin_chi2_eleup, KinFitConv_eleup, SVfitConv_eleup, MT2_eleup);
            std::vector<float> outs_eleup = DNNreader.GetPredictions();
            DNNoutSM_kl_1_eleup.at(i) = outs_eleup.at(0);

            DNNreader.SetShiftedInputs(bjet1, bjet2, tau1_eledown, tau2_eledown, vbfjet1, vbfjet2, met_eledown, svfit_eledown,
                HHKin_mass_eledown, HHKin_chi2_eledown, KinFitConv_eledown, SVfitConv_eledown, MT2_eledown);
            std::vector<float> outs_eledown = DNNreader.GetPredictions();
            DNNoutSM_kl_1_eledown.at(i) = outs_eledown.at(0);
          }

          if (doMult)
          {
            mci.clearInputs();
            for (uint j=0; j<mci.getNumberOfModels(); j++)
            {
              mci.setInputs(j,
              {
                {"is_mutau", mdnn_isMuTau}, {"is_etau", mdnn_isETau}, {"is_tautau", mdnn_isTauTau},
                {"bjet1_pt", bjet1_pt}, {"bjet1_eta", bjet1_eta}, {"bjet1_phi", bjet1_phi}, {"bjet1_e", bjet1_e},
                {"bjet1_deepflavor_b", bjet1_bID_deepFlavor}, {"bjet1_deepflavor_cvsb", CvsB_b1}, {"bjet1_deepflavor_cvsl", CvsL_b1}, {"bjet1_hhbtag", HHbtag_b1},
                {"bjet2_pt", bjet2_pt}, {"bjet2_eta", bjet2_eta}, {"bjet2_phi", bjet2_phi}, {"bjet2_e", bjet2_e},
                {"bjet2_deepflavor_b", bjet2_bID_deepFlavor}, {"bjet2_deepflavor_cvsb", CvsB_b2}, {"bjet2_deepflavor_cvsl", CvsL_b2}, {"bjet2_hhbtag", HHbtag_b2},
                {"vbfjet1_pt", vbfjet1_pt}, {"vbfjet1_eta", vbfjet1_eta}, {"vbfjet1_phi", vbfjet1_phi}, {"vbfjet1_e", vbfjet1_e},
                {"vbfjet1_deepflavor_b", VBFjet1_btag_deepFlavor}, {"vbfjet1_deepflavor_cvsb", CvsB_vbf1}, {"vbfjet1_deepflavor_cvsl", CvsL_vbf1}, {"vbfjet1_hhbtag", HHbtag_vbf1},
                {"vbfjet2_pt", vbfjet2_pt}, {"vbfjet2_eta", vbfjet2_eta}, {"vbfjet2_phi", vbfjet2_phi}, {"vbfjet2_e", vbfjet2_e},
                {"vbfjet2_deepflavor_b", VBFjet2_btag_deepFlavor}, {"vbfjet2_deepflavor_cvsb", CvsB_vbf2}, {"vbfjet2_deepflavor_cvsl", CvsL_vbf2}, {"vbfjet2_hhbtag", HHbtag_vbf2},
                {"lep1_pt", tau1_eleup.Pt()}, {"lep1_eta", tau1_eleup.Eta()}, {"lep1_phi", tau1_eleup.Phi()}, {"lep1_e", tau1_eleup.E()},
                {"lep2_pt", tau2_eleup.Pt()}, {"lep2_eta", tau2_eleup.Eta()}, {"lep2_phi", tau2_eleup.Phi()}, {"lep2_e", tau2_eleup.E()},
                {"met_pt", met_eleup.Pt()}, {"met_phi", met_eleup.Phi()},
                {"bh_pt", (bjet1+bjet2).Pt()}, {"bh_eta", (bjet1+bjet2).Eta()}, {"bh_phi", (bjet1+bjet2).Phi()}, {"bh_e", (bjet1+bjet2).E()},
                {"tauh_sv_pt", svfit_eleup.Pt()}, {"tauh_sv_eta", svfit_eleup.Eta()}, {"tauh_sv_phi", svfit_eleup.Phi()}, {"tauh_sv_e", svfit_eleup.E()}
              });
            }
            auto mdnnSM0_score_eleup  = mci.predict(EventNumber, 0);
            auto mdnnSM1_score_eleup  = mci.predict(EventNumber, 1);
            auto mdnnBSM0_score_eleup = mci.predict(EventNumber, 2);
            for (uint k=0; k<mdnnSM0_score_eleup.size(); k++)
              mdnnSM0_output_eleup[i].at(k) = mdnnSM0_score_eleup.at(k).second;
            for (uint k=0; k<mdnnSM1_score_eleup.size(); k++)
              mdnnSM1_output_eleup[i].at(k) = mdnnSM1_score_eleup.at(k).second;
            for (uint k=0; k<mdnnBSM0_score_eleup.size(); k++)
              mdnnBSM0_output_eleup[i].at(k) = mdnnBSM0_score_eleup.at(k).second;

            mci.clearInputs();
            for (uint j=0; j<mci.getNumberOfModels(); j++)
            {
              mci.setInputs(j,
              {
                {"is_mutau", mdnn_isMuTau}, {"is_etau", mdnn_isETau}, {"is_tautau", mdnn_isTauTau},
                {"bjet1_pt", bjet1_pt}, {"bjet1_eta", bjet1_eta}, {"bjet1_phi", bjet1_phi}, {"bjet1_e", bjet1_e},
                {"bjet1_deepflavor_b", bjet1_bID_deepFlavor}, {"bjet1_deepflavor_cvsb", CvsB_b1}, {"bjet1_deepflavor_cvsl", CvsL_b1}, {"bjet1_hhbtag", HHbtag_b1},
                {"bjet2_pt", bjet2_pt}, {"bjet2_eta", bjet2_eta}, {"bjet2_phi", bjet2_phi}, {"bjet2_e", bjet2_e},
                {"bjet2_deepflavor_b", bjet2_bID_deepFlavor}, {"bjet2_deepflavor_cvsb", CvsB_b2}, {"bjet2_deepflavor_cvsl", CvsL_b2}, {"bjet2_hhbtag", HHbtag_b2},
                {"vbfjet1_pt", vbfjet1_pt}, {"vbfjet1_eta", vbfjet1_eta}, {"vbfjet1_phi", vbfjet1_phi}, {"vbfjet1_e", vbfjet1_e},
                {"vbfjet1_deepflavor_b", VBFjet1_btag_deepFlavor}, {"vbfjet1_deepflavor_cvsb", CvsB_vbf1}, {"vbfjet1_deepflavor_cvsl", CvsL_vbf1}, {"vbfjet1_hhbtag", HHbtag_vbf1},
                {"vbfjet2_pt", vbfjet2_pt}, {"vbfjet2_eta", vbfjet2_eta}, {"vbfjet2_phi", vbfjet2_phi}, {"vbfjet2_e", vbfjet2_e},
                {"vbfjet2_deepflavor_b", VBFjet2_btag_deepFlavor}, {"vbfjet2_deepflavor_cvsb", CvsB_vbf2}, {"vbfjet2_deepflavor_cvsl", CvsL_vbf2}, {"vbfjet2_hhbtag", HHbtag_vbf2},
                {"lep1_pt", tau1_eledown.Pt()}, {"lep1_eta", tau1_eledown.Eta()}, {"lep1_phi", tau1_eledown.Phi()}, {"lep1_e", tau1_eledown.E()},
                {"lep2_pt", tau2_eledown.Pt()}, {"lep2_eta", tau2_eledown.Eta()}, {"lep2_phi", tau2_eledown.Phi()}, {"lep2_e", tau2_eledown.E()},
                {"met_pt", met_eledown.Pt()}, {"met_phi", met_eledown.Phi()},
                {"bh_pt", (bjet1+bjet2).Pt()}, {"bh_eta", (bjet1+bjet2).Eta()}, {"bh_phi", (bjet1+bjet2).Phi()}, {"bh_e", (bjet1+bjet2).E()},
                {"tauh_sv_pt", svfit_eledown.Pt()}, {"tauh_sv_eta", svfit_eledown.Eta()}, {"tauh_sv_phi", svfit_eledown.Phi()}, {"tauh_sv_e", svfit_eledown.E()}
              });
            }
            auto mdnnSM0_score_eledown  = mci.predict(EventNumber, 0);
            auto mdnnSM1_score_eledown  = mci.predict(EventNumber, 1);
            auto mdnnBSM0_score_eledown = mci.predict(EventNumber, 2);
            for (uint k=0; k<mdnnSM0_score_eledown.size(); k++)
              mdnnSM0_output_eledown[i].at(k) = mdnnSM0_score_eledown.at(k).second;
            for (uint k=0; k<mdnnSM1_score_eledown.size(); k++)
              mdnnSM1_output_eledown[i].at(k) = mdnnSM1_score_eledown.at(k).second;
            for (uint k=0; k<mdnnBSM0_score_eledown.size(); k++)
              mdnnBSM0_output_eledown[i].at(k) = mdnnBSM0_score_eledown.at(k).second;
          }

          if (doBDT)
          {
            BDTreader.SetInputValues(bjet2.Pt(), (bjet1+bjet2).Pt(), tau1_eleup.Pt(),
              tau2_eleup.Pt(), svfit_eleup.Pt(), BDT_channel,
              BDT_HT20, pzeta_eleup, pzeta_vis_eleup, BDT_ditau_deltaPhi_eleup,
              BDT_tauHsvfitMet_deltaPhi_eleup, mT_tauH_MET_eleup, mTtot_eleup, MT2_eleup,
              BDT_MX_eleup, BDT_bH_tauH_MET_InvMass_eleup, BDT_bH_tauH_SVFIT_InvMass_eleup,
              BDT_bH_tauH_InvMass_eleup, HHKin_mass_eleup, HHKin_chi2_eleup, BDT_MET_bH_cosTheta_eleup);
            std::vector<float> BDTouts_eleup = BDTreader.GetPredictions();
            BDToutSM_kl_1_eleup.at(i) = BDTouts_eleup.at(0);

            BDTreader.SetInputValues(bjet2.Pt(), (bjet1+bjet2).Pt(), tau1_eledown.Pt(),
              tau2_eledown.Pt(), svfit_eledown.Pt(), BDT_channel,
              BDT_HT20, pzeta_eledown, pzeta_vis_eledown, BDT_ditau_deltaPhi_eledown,
              BDT_tauHsvfitMet_deltaPhi_eledown, mT_tauH_MET_eledown, mTtot_eledown, MT2_eledown,
              BDT_MX_eledown, BDT_bH_tauH_MET_InvMass_eledown, BDT_bH_tauH_SVFIT_InvMass_eledown,
              BDT_bH_tauH_InvMass_eledown, HHKin_mass_eledown, HHKin_chi2_eledown, BDT_MET_bH_cosTheta_eledown);
            std::vector<float> BDTouts_eledown = BDTreader.GetPredictions();
            BDToutSM_kl_1_eledown.at(i) = BDTouts_eledown.at(0);
          }
        } // End isMC
      } // End Loop on EES
    }  // End doEES
    // Timing info
    auto end_EES = high_resolution_clock::now();

    // ---- ---- ---- ---- ---- ---- ----
    // ---- ---- Do all TES now ---- ----
    // ---- ---- ---- ---- ---- ---- ----
    // Timing info
    auto start_TES = high_resolution_clock::now();
    if (doTES)
    {
      for (int i=0; i<N_tauhDM; i++)
      {
        if (isData)
        {
          // Fill with nominal value
          tauH_SVFIT_mass_tauup.at(i)   = tauH_SVFIT_mass;
          tauH_SVFIT_mass_taudown.at(i) = tauH_SVFIT_mass;
          DNNoutSM_kl_1_tauup.at(i)     = DNNoutSM_kl_1;
          DNNoutSM_kl_1_taudown.at(i)   = DNNoutSM_kl_1;
          BDToutSM_kl_1_tauup.at(i)     = BDToutSM_kl_1;
          BDToutSM_kl_1_taudown.at(i)   = BDToutSM_kl_1;

          // VBF multiclass
          mci.clearInputs();
          for (uint j=0; j<mci.getNumberOfModels(); j++)
          {
            mci.setInputs(j,
            {
              {"is_mutau", mdnn_isMuTau}, {"is_etau", mdnn_isETau}, {"is_tautau", mdnn_isTauTau},
              {"bjet1_pt", bjet1_pt}, {"bjet1_eta", bjet1_eta}, {"bjet1_phi", bjet1_phi}, {"bjet1_e", bjet1_e},
              {"bjet1_deepflavor_b", bjet1_bID_deepFlavor}, {"bjet1_deepflavor_cvsb", CvsB_b1}, {"bjet1_deepflavor_cvsl", CvsL_b1}, {"bjet1_hhbtag", HHbtag_b1},
              {"bjet2_pt", bjet2_pt}, {"bjet2_eta", bjet2_eta}, {"bjet2_phi", bjet2_phi}, {"bjet2_e", bjet2_e},
              {"bjet2_deepflavor_b", bjet2_bID_deepFlavor}, {"bjet2_deepflavor_cvsb", CvsB_b2}, {"bjet2_deepflavor_cvsl", CvsL_b2}, {"bjet2_hhbtag", HHbtag_b2},
              {"vbfjet1_pt", vbfjet1_pt}, {"vbfjet1_eta", vbfjet1_eta}, {"vbfjet1_phi", vbfjet1_phi}, {"vbfjet1_e", vbfjet1_e},
              {"vbfjet1_deepflavor_b", VBFjet1_btag_deepFlavor}, {"vbfjet1_deepflavor_cvsb", CvsB_vbf1}, {"vbfjet1_deepflavor_cvsl", CvsL_vbf1}, {"vbfjet1_hhbtag", HHbtag_vbf1},
              {"vbfjet2_pt", vbfjet2_pt}, {"vbfjet2_eta", vbfjet2_eta}, {"vbfjet2_phi", vbfjet2_phi}, {"vbfjet2_e", vbfjet2_e},
              {"vbfjet2_deepflavor_b", VBFjet2_btag_deepFlavor}, {"vbfjet2_deepflavor_cvsb", CvsB_vbf2}, {"vbfjet2_deepflavor_cvsl", CvsL_vbf2}, {"vbfjet2_hhbtag", HHbtag_vbf2},
              {"lep1_pt", dau1_pt}, {"lep1_eta", dau1_eta}, {"lep1_phi", dau1_phi}, {"lep1_e", dau1_e},
              {"lep2_pt", dau2_pt}, {"lep2_eta", dau2_eta}, {"lep2_phi", dau2_phi}, {"lep2_e", dau2_e},
              {"met_pt", met.Pt()}, {"met_phi", met.Phi()},
              {"bh_pt", (bjet1+bjet2).Pt()}, {"bh_eta", (bjet1+bjet2).Eta()}, {"bh_phi", (bjet1+bjet2).Phi()}, {"bh_e", (bjet1+bjet2).E()},
              {"tauh_sv_pt", tauH_SVFIT_pt}, {"tauh_sv_eta", tauH_SVFIT_eta}, {"tauh_sv_phi", tauH_SVFIT_phi}, {"tauh_sv_e", svfit.E()}
            });
          }
          auto mdnnSM0_score_tes  = mci.predict(EventNumber, 0);
          auto mdnnSM1_score_tes  = mci.predict(EventNumber, 1);
          auto mdnnBSM0_score_tes = mci.predict(EventNumber, 2);
          for (uint k=0; k<mdnnSM0_score_tes.size(); k++)
          {
            mdnnSM0_output_tauup  [i].at(k) = mdnnSM0_score_tes.at(k).second;
            mdnnSM0_output_taudown[i].at(k) = mdnnSM0_score_tes.at(k).second;
          }
          for (uint k=0; k<mdnnSM1_score_tes.size(); k++)
          {
            mdnnSM1_output_tauup  [i].at(k) = mdnnSM1_score_tes.at(k).second;
            mdnnSM1_output_taudown[i].at(k) = mdnnSM1_score_tes.at(k).second;
          }
          for (uint k=0; k<mdnnBSM0_score_tes.size(); k++)
          {
            mdnnBSM0_output_tauup  [i].at(k) = mdnnBSM0_score_tes.at(k).second;
            mdnnBSM0_output_taudown[i].at(k) = mdnnBSM0_score_tes.at(k).second;
          }
        }
        else /*isMC*/
        {
          // Build shifted taus and MET
          TLorentzVector tau1_tauup, tau2_tauup, met_tauup;
          tau1_tauup.SetPtEtaPhiM(dau1_pt_tauup->at(i), dau1_eta, dau1_phi, dau1_mass_tauup->at(i));
          tau2_tauup.SetPtEtaPhiM(dau2_pt_tauup->at(i), dau2_eta, dau2_phi, dau2_mass_tauup->at(i));
          met_tauup .SetPxPyPzE(METx_tauup->at(i), METy_tauup->at(i), 0, std::hypot(METx_tauup->at(i), METy_tauup->at(i)));
          TVector2 ptmiss_tauup = TVector2(METx_tauup->at(i), METy_tauup->at(i));

          TLorentzVector tau1_taudown, tau2_taudown, met_taudown;
          tau1_taudown.SetPtEtaPhiM(dau1_pt_taudown->at(i), dau1_eta, dau1_phi, dau1_mass_taudown->at(i));
          tau2_taudown.SetPtEtaPhiM(dau2_pt_taudown->at(i), dau2_eta, dau2_phi, dau2_mass_taudown->at(i));
          met_taudown .SetPxPyPzE(METx_taudown->at(i), METy_taudown->at(i), 0, std::hypot(METx_taudown->at(i), METy_taudown->at(i)));
          TVector2 ptmiss_taudown = TVector2(METx_taudown->at(i), METy_taudown->at(i));

          // Declare other useful shifted variables
          float HHKin_mass_tauup, HHKin_chi2_tauup, HHKin_mass_taudown, HHKin_chi2_taudown;
          float MT2_tauup, MT2_taudown;
          float tauH_SVFIT_pt_tauup, tauH_SVFIT_eta_tauup, tauH_SVFIT_phi_tauup;
          float tauH_SVFIT_pt_taudown, tauH_SVFIT_eta_taudown, tauH_SVFIT_phi_taudown;

          if (doKinFit)
          {
            HHKinFit2::HHKinFitMasterHeavyHiggs kinFits_tauup = HHKinFit2::HHKinFitMasterHeavyHiggs(bjet1, bjet2, tau1_tauup, tau2_tauup, ptmiss_tauup, stableMetCov, bjet1_JER, bjet2_JER) ;
            kinFits_tauup.addHypo(hypo_mh1,hypo_mh2);
            bool wrongHHK_tauup =false;
            try {kinFits_tauup.fit();}
            catch(HHKinFit2::HHInvMConstraintException   &e) {wrongHHK_tauup=true;}
            catch(HHKinFit2::HHEnergyConstraintException &e) {wrongHHK_tauup=true;}
            catch(HHKinFit2::HHEnergyRangeException      &e) {wrongHHK_tauup=true;}
            if(!wrongHHK_tauup)
            {
              HHKin_mass_tauup = kinFits_tauup.getMH();
              HHKin_chi2_tauup = kinFits_tauup.getChi2();
            }
            else
            {
              HHKin_mass_tauup = -333.;
              HHKin_chi2_tauup = 0.;
            }

            HHKinFit2::HHKinFitMasterHeavyHiggs kinFits_taudown = HHKinFit2::HHKinFitMasterHeavyHiggs(bjet1, bjet2, tau1_taudown, tau2_taudown, ptmiss_taudown, stableMetCov, bjet1_JER, bjet2_JER) ;
            kinFits_taudown.addHypo(hypo_mh1,hypo_mh2);
            bool wrongHHK_taudown =false;
            try {kinFits_taudown.fit();}
            catch(HHKinFit2::HHInvMConstraintException   &e) {wrongHHK_taudown=true;}
            catch(HHKinFit2::HHEnergyConstraintException &e) {wrongHHK_taudown=true;}
            catch(HHKinFit2::HHEnergyRangeException      &e) {wrongHHK_taudown=true;}
            if(!wrongHHK_taudown)
            {
              HHKin_mass_taudown = kinFits_taudown.getMH();
              HHKin_chi2_taudown = kinFits_taudown.getChi2();
            }
            else
            {
              HHKin_mass_taudown = -333.;
              HHKin_chi2_taudown = 0.;
            }
          }

          if (doMT2)
          {
            MT2_tauup = asymm_mt2_lester_bisect::get_mT2( bjet1.M(), bjet1.Px(), bjet1.Py(),
                                                          bjet2.M(), bjet2.Px(), bjet2.Py(),
                                                          (tau1_tauup.Px() + tau2_tauup.Px() + met_tauup.Px()),
                                                          (tau1_tauup.Py() + tau2_tauup.Py() + met_tauup.Py()),
                                                          tau1_tauup.M(), tau2_tauup.M(), desiredPrecisionOnMt2);

            MT2_taudown = asymm_mt2_lester_bisect::get_mT2( bjet1.M(), bjet1.Px(), bjet1.Py(),
                                                            bjet2.M(), bjet2.Px(), bjet2.Py(),
                                                            (tau1_taudown.Px() + tau2_taudown.Px() + met_taudown.Px()),
                                                            (tau1_taudown.Py() + tau2_taudown.Py() + met_taudown.Py()),
                                                            tau1_taudown.M(), tau2_taudown.M(), desiredPrecisionOnMt2);
          }

          if (doSVfit)
          {
            SVfitKLUBinterface algo_tauup(0, tau1_tauup, tau2_tauup, met_tauup, stableMetCov, pType, DM1, DM2);
            std::vector<double> svfitRes_tauup = algo_tauup.FitAndGetResult();
            tauH_SVFIT_pt_tauup         = svfitRes_tauup.at(0);
            tauH_SVFIT_eta_tauup        = svfitRes_tauup.at(1);
            tauH_SVFIT_phi_tauup        = svfitRes_tauup.at(2);
            tauH_SVFIT_mass_tauup.at(i) = svfitRes_tauup.at(3);

            SVfitKLUBinterface algo_taudown(0, tau1_taudown, tau2_taudown, met_taudown, stableMetCov, pType, DM1, DM2);
            std::vector<double> svfitRes_taudown = algo_taudown.FitAndGetResult();
            tauH_SVFIT_pt_taudown         = svfitRes_taudown.at(0);
            tauH_SVFIT_eta_taudown        = svfitRes_taudown.at(1);
            tauH_SVFIT_phi_taudown        = svfitRes_taudown.at(2);
            tauH_SVFIT_mass_taudown.at(i) = svfitRes_taudown.at(3);
          }

          // --- --- --- TES DNN/BDT quantities --- --- ---
          TLorentzVector svfit_tauup;
          svfit_tauup.SetPtEtaPhiM(tauH_SVFIT_pt_tauup, tauH_SVFIT_eta_tauup, tauH_SVFIT_phi_tauup, tauH_SVFIT_mass_tauup.at(i));
          float mTtot_tauup                         = Calculate_TotalMT(tau1_tauup, tau2_tauup, met_tauup);
          float pzeta_vis_tauup                     = Calculate_visiblePzeta(tau1_tauup, tau2_tauup);
          float pzeta_tauup                         = Calculate_Pzeta(tau1_tauup, tau2_tauup, met_tauup);
          float BDT_ditau_deltaPhi_tauup            = ROOT::Math::VectorUtil::DeltaPhi(tau1_tauup, tau2_tauup);
          float BDT_tauHsvfitMet_deltaPhi_tauup     = ROOT::Math::VectorUtil::DeltaPhi(svfit_tauup, met_tauup);
          float mT_tauH_MET_tauup                   = Calculate_MT( (tau1_tauup+tau2_tauup)+met_tauup, met_tauup);
          float BDT_MX_tauup                        = Calculate_MX(tau1_tauup, tau2_tauup, bjet1, bjet2, met_tauup);
          float BDT_bH_tauH_MET_InvMass_tauup       = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_tauup+tau2_tauup)+met_tauup);
          float BDT_bH_tauH_SVFIT_InvMass_tauup     = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), svfit_tauup);
          float BDT_bH_tauH_InvMass_tauup           = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_tauup+tau2_tauup));
          float BDT_MET_bH_cosTheta_tauup           = Calculate_cosTheta_2bodies(getLVfromTLV(met_tauup), getLVfromTLV(bjet1+bjet2));
          bool KinFitConv_tauup                     = HHKin_chi2_tauup > 0;
          bool SVfitConv_tauup                      = tauH_SVFIT_mass_tauup.at(i) > 0;

          TLorentzVector svfit_taudown;
          svfit_taudown.SetPtEtaPhiM(tauH_SVFIT_pt_taudown, tauH_SVFIT_eta_taudown, tauH_SVFIT_phi_taudown, tauH_SVFIT_mass_taudown.at(i));
          float mTtot_taudown                         = Calculate_TotalMT(tau1_taudown, tau2_taudown, met_taudown);
          float pzeta_vis_taudown                     = Calculate_visiblePzeta(tau1_taudown, tau2_taudown);
          float pzeta_taudown                         = Calculate_Pzeta(tau1_taudown, tau2_taudown, met_taudown);
          float BDT_ditau_deltaPhi_taudown            = ROOT::Math::VectorUtil::DeltaPhi(tau1_taudown, tau2_taudown);
          float BDT_tauHsvfitMet_deltaPhi_taudown     = ROOT::Math::VectorUtil::DeltaPhi(svfit_taudown, met_taudown);
          float mT_tauH_MET_taudown                   = Calculate_MT( (tau1_taudown+tau2_taudown)+met_taudown, met_taudown);
          float BDT_MX_taudown                        = Calculate_MX(tau1_taudown, tau2_taudown, bjet1, bjet2, met_taudown);
          float BDT_bH_tauH_MET_InvMass_taudown       = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_taudown+tau2_taudown)+met_taudown);
          float BDT_bH_tauH_SVFIT_InvMass_taudown     = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), svfit_taudown);
          float BDT_bH_tauH_InvMass_taudown           = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_taudown+tau2_taudown));
          float BDT_MET_bH_cosTheta_taudown           = Calculate_cosTheta_2bodies(getLVfromTLV(met_taudown), getLVfromTLV(bjet1+bjet2));
          bool KinFitConv_taudown                     = HHKin_chi2_taudown > 0;
          bool SVfitConv_taudown                      = tauH_SVFIT_mass_taudown.at(i) > 0;

          if (doDNN)
          {
            DNNreader.SetShiftedInputs(bjet1, bjet2, tau1_tauup, tau2_tauup, vbfjet1, vbfjet1, met_tauup, svfit_tauup,
                HHKin_mass_tauup, HHKin_chi2_tauup, KinFitConv_tauup, SVfitConv_tauup, MT2_tauup);
            std::vector<float> outs_tauup = DNNreader.GetPredictions();
            DNNoutSM_kl_1_tauup.at(i) = outs_tauup.at(0);

            DNNreader.SetShiftedInputs(bjet1, bjet2, tau1_taudown, tau2_taudown, vbfjet1, vbfjet1, met_taudown, svfit_taudown,
                HHKin_mass_taudown, HHKin_chi2_taudown, KinFitConv_taudown, SVfitConv_taudown, MT2_taudown);
            std::vector<float> outs_taudown = DNNreader.GetPredictions();
            DNNoutSM_kl_1_taudown.at(i) = outs_taudown.at(0);
          }

          if (doMult)
          {
            mci.clearInputs();
            for (uint j=0; j<mci.getNumberOfModels(); j++)
            {
              mci.setInputs(j,
              {
                {"is_mutau", mdnn_isMuTau}, {"is_etau", mdnn_isETau}, {"is_tautau", mdnn_isTauTau},
                {"bjet1_pt", bjet1_pt}, {"bjet1_eta", bjet1_eta}, {"bjet1_phi", bjet1_phi}, {"bjet1_e", bjet1_e},
                {"bjet1_deepflavor_b", bjet1_bID_deepFlavor}, {"bjet1_deepflavor_cvsb", CvsB_b1}, {"bjet1_deepflavor_cvsl", CvsL_b1}, {"bjet1_hhbtag", HHbtag_b1},
                {"bjet2_pt", bjet2_pt}, {"bjet2_eta", bjet2_eta}, {"bjet2_phi", bjet2_phi}, {"bjet2_e", bjet2_e},
                {"bjet2_deepflavor_b", bjet2_bID_deepFlavor}, {"bjet2_deepflavor_cvsb", CvsB_b2}, {"bjet2_deepflavor_cvsl", CvsL_b2}, {"bjet2_hhbtag", HHbtag_b2},
                {"vbfjet1_pt", vbfjet1_pt}, {"vbfjet1_eta", vbfjet1_eta}, {"vbfjet1_phi", vbfjet1_phi}, {"vbfjet1_e", vbfjet1_e},
                {"vbfjet1_deepflavor_b", VBFjet1_btag_deepFlavor}, {"vbfjet1_deepflavor_cvsb", CvsB_vbf1}, {"vbfjet1_deepflavor_cvsl", CvsL_vbf1}, {"vbfjet1_hhbtag", HHbtag_vbf1},
                {"vbfjet2_pt", vbfjet2_pt}, {"vbfjet2_eta", vbfjet2_eta}, {"vbfjet2_phi", vbfjet2_phi}, {"vbfjet2_e", vbfjet2_e},
                {"vbfjet2_deepflavor_b", VBFjet2_btag_deepFlavor}, {"vbfjet2_deepflavor_cvsb", CvsB_vbf2}, {"vbfjet2_deepflavor_cvsl", CvsL_vbf2}, {"vbfjet2_hhbtag", HHbtag_vbf2},
                {"lep1_pt", tau1_tauup.Pt()}, {"lep1_eta", tau1_tauup.Eta()}, {"lep1_phi", tau1_tauup.Phi()}, {"lep1_e", tau1_tauup.E()},
                {"lep2_pt", tau2_tauup.Pt()}, {"lep2_eta", tau2_tauup.Eta()}, {"lep2_phi", tau2_tauup.Phi()}, {"lep2_e", tau2_tauup.E()},
                {"met_pt", met_tauup.Pt()}, {"met_phi", met_tauup.Phi()},
                {"bh_pt", (bjet1+bjet2).Pt()}, {"bh_eta", (bjet1+bjet2).Eta()}, {"bh_phi", (bjet1+bjet2).Phi()}, {"bh_e", (bjet1+bjet2).E()},
                {"tauh_sv_pt", svfit_tauup.Pt()}, {"tauh_sv_eta", svfit_tauup.Eta()}, {"tauh_sv_phi", svfit_tauup.Phi()}, {"tauh_sv_e", svfit_tauup.E()}
              });
            }
            auto mdnnSM0_score_tauup  = mci.predict(EventNumber, 0);
            auto mdnnSM1_score_tauup  = mci.predict(EventNumber, 1);
            auto mdnnBSM0_score_tauup = mci.predict(EventNumber, 2);
            for (uint k=0; k<mdnnSM0_score_tauup.size(); k++)
              mdnnSM0_output_tauup[i].at(k) = mdnnSM0_score_tauup.at(k).second;
            for (uint k=0; k<mdnnSM1_score_tauup.size(); k++)
              mdnnSM1_output_tauup[i].at(k) = mdnnSM1_score_tauup.at(k).second;
            for (uint k=0; k<mdnnBSM0_score_tauup.size(); k++)
              mdnnBSM0_output_tauup[i].at(k) = mdnnBSM0_score_tauup.at(k).second;

            mci.clearInputs();
            for (uint j=0; j<mci.getNumberOfModels(); j++)
            {
              mci.setInputs(j,
              {
                {"is_mutau", mdnn_isMuTau}, {"is_etau", mdnn_isETau}, {"is_tautau", mdnn_isTauTau},
                {"bjet1_pt", bjet1_pt}, {"bjet1_eta", bjet1_eta}, {"bjet1_phi", bjet1_phi}, {"bjet1_e", bjet1_e},
                {"bjet1_deepflavor_b", bjet1_bID_deepFlavor}, {"bjet1_deepflavor_cvsb", CvsB_b1}, {"bjet1_deepflavor_cvsl", CvsL_b1}, {"bjet1_hhbtag", HHbtag_b1},
                {"bjet2_pt", bjet2_pt}, {"bjet2_eta", bjet2_eta}, {"bjet2_phi", bjet2_phi}, {"bjet2_e", bjet2_e},
                {"bjet2_deepflavor_b", bjet2_bID_deepFlavor}, {"bjet2_deepflavor_cvsb", CvsB_b2}, {"bjet2_deepflavor_cvsl", CvsL_b2}, {"bjet2_hhbtag", HHbtag_b2},
                {"vbfjet1_pt", vbfjet1_pt}, {"vbfjet1_eta", vbfjet1_eta}, {"vbfjet1_phi", vbfjet1_phi}, {"vbfjet1_e", vbfjet1_e},
                {"vbfjet1_deepflavor_b", VBFjet1_btag_deepFlavor}, {"vbfjet1_deepflavor_cvsb", CvsB_vbf1}, {"vbfjet1_deepflavor_cvsl", CvsL_vbf1}, {"vbfjet1_hhbtag", HHbtag_vbf1},
                {"vbfjet2_pt", vbfjet2_pt}, {"vbfjet2_eta", vbfjet2_eta}, {"vbfjet2_phi", vbfjet2_phi}, {"vbfjet2_e", vbfjet2_e},
                {"vbfjet2_deepflavor_b", VBFjet2_btag_deepFlavor}, {"vbfjet2_deepflavor_cvsb", CvsB_vbf2}, {"vbfjet2_deepflavor_cvsl", CvsL_vbf2}, {"vbfjet2_hhbtag", HHbtag_vbf2},
                {"lep1_pt", tau1_taudown.Pt()}, {"lep1_eta", tau1_taudown.Eta()}, {"lep1_phi", tau1_taudown.Phi()}, {"lep1_e", tau1_taudown.E()},
                {"lep2_pt", tau2_taudown.Pt()}, {"lep2_eta", tau2_taudown.Eta()}, {"lep2_phi", tau2_taudown.Phi()}, {"lep2_e", tau2_taudown.E()},
                {"met_pt", met_taudown.Pt()}, {"met_phi", met_taudown.Phi()},
                {"bh_pt", (bjet1+bjet2).Pt()}, {"bh_eta", (bjet1+bjet2).Eta()}, {"bh_phi", (bjet1+bjet2).Phi()}, {"bh_e", (bjet1+bjet2).E()},
                {"tauh_sv_pt", svfit_taudown.Pt()}, {"tauh_sv_eta", svfit_taudown.Eta()}, {"tauh_sv_phi", svfit_taudown.Phi()}, {"tauh_sv_e", svfit_taudown.E()}
              });
            }
            auto mdnnSM0_score_taudown  = mci.predict(EventNumber, 0);
            auto mdnnSM1_score_taudown  = mci.predict(EventNumber, 1);
            auto mdnnBSM0_score_taudown = mci.predict(EventNumber, 2);
            for (uint k=0; k<mdnnSM0_score_taudown.size(); k++)
              mdnnSM0_output_taudown[i].at(k) = mdnnSM0_score_taudown.at(k).second;
            for (uint k=0; k<mdnnSM1_score_taudown.size(); k++)
              mdnnSM1_output_taudown[i].at(k) = mdnnSM1_score_taudown.at(k).second;
            for (uint k=0; k<mdnnBSM0_score_taudown.size(); k++)
              mdnnBSM0_output_taudown[i].at(k) = mdnnBSM0_score_taudown.at(k).second;
          }

          if (doBDT)
          {
            BDTreader.SetInputValues(bjet2.Pt(), (bjet1+bjet2).Pt(), tau1_tauup.Pt(),
              tau2_tauup.Pt(), svfit_tauup.Pt(), BDT_channel,
              BDT_HT20, pzeta_tauup, pzeta_vis_tauup, BDT_ditau_deltaPhi_tauup,
              BDT_tauHsvfitMet_deltaPhi_tauup, mT_tauH_MET_tauup, mTtot_tauup, MT2_tauup,
              BDT_MX_tauup, BDT_bH_tauH_MET_InvMass_tauup, BDT_bH_tauH_SVFIT_InvMass_tauup,
              BDT_bH_tauH_InvMass_tauup, HHKin_mass_tauup, HHKin_chi2_tauup, BDT_MET_bH_cosTheta_tauup);
            std::vector<float> BDTouts_tauup = BDTreader.GetPredictions();
            BDToutSM_kl_1_tauup.at(i) = BDTouts_tauup.at(0);

            BDTreader.SetInputValues(bjet2.Pt(), (bjet1+bjet2).Pt(), tau1_taudown.Pt(),
              tau2_taudown.Pt(), svfit_taudown.Pt(), BDT_channel,
              BDT_HT20, pzeta_taudown, pzeta_vis_taudown, BDT_ditau_deltaPhi_taudown,
              BDT_tauHsvfitMet_deltaPhi_taudown, mT_tauH_MET_taudown, mTtot_taudown, MT2_taudown,
              BDT_MX_taudown, BDT_bH_tauH_MET_InvMass_taudown, BDT_bH_tauH_SVFIT_InvMass_taudown,
              BDT_bH_tauH_InvMass_taudown, HHKin_mass_taudown, HHKin_chi2_taudown, BDT_MET_bH_cosTheta_taudown);
            std::vector<float> BDTouts_taudown = BDTreader.GetPredictions();
            BDToutSM_kl_1_taudown.at(i) = BDTouts_taudown.at(0);
          }
        } // End isMC
      } // End loop on TES
    }  // End doTES
    // Timing info
    auto end_TES = high_resolution_clock::now();

    // ---- ---- ---- ---- ---- ---- ---- ----
    // ---- ---- Do splitted JES now ---- ----
    // ---- ---- ---- ---- ---- ---- ---- ----
    // Timing info
    auto start_splitJES = high_resolution_clock::now();
    if (doSplitJES)
    {
      for (int i=0; i<N_jecSources; i++)
      {
        if (isData)
        {
          // Fill with nominal value
          tauH_SVFIT_mass_jetup.at(i)   = tauH_SVFIT_mass;
          tauH_SVFIT_mass_jetdown.at(i) = tauH_SVFIT_mass;
          DNNoutSM_kl_1_jetup.at(i)     = DNNoutSM_kl_1;
          DNNoutSM_kl_1_jetdown.at(i)   = DNNoutSM_kl_1;
          BDToutSM_kl_1_jetup.at(i)     = BDToutSM_kl_1;
          BDToutSM_kl_1_jetdown.at(i)   = BDToutSM_kl_1;

          // VBF multiclass
          mci.clearInputs();
          for (uint j=0; j<mci.getNumberOfModels(); j++)
          {
            mci.setInputs(j,
            {
              {"is_mutau", mdnn_isMuTau}, {"is_etau", mdnn_isETau}, {"is_tautau", mdnn_isTauTau},
              {"bjet1_pt", bjet1_pt}, {"bjet1_eta", bjet1_eta}, {"bjet1_phi", bjet1_phi}, {"bjet1_e", bjet1_e},
              {"bjet1_deepflavor_b", bjet1_bID_deepFlavor}, {"bjet1_deepflavor_cvsb", CvsB_b1}, {"bjet1_deepflavor_cvsl", CvsL_b1}, {"bjet1_hhbtag", HHbtag_b1},
              {"bjet2_pt", bjet2_pt}, {"bjet2_eta", bjet2_eta}, {"bjet2_phi", bjet2_phi}, {"bjet2_e", bjet2_e},
              {"bjet2_deepflavor_b", bjet2_bID_deepFlavor}, {"bjet2_deepflavor_cvsb", CvsB_b2}, {"bjet2_deepflavor_cvsl", CvsL_b2}, {"bjet2_hhbtag", HHbtag_b2},
              {"vbfjet1_pt", vbfjet1_pt}, {"vbfjet1_eta", vbfjet1_eta}, {"vbfjet1_phi", vbfjet1_phi}, {"vbfjet1_e", vbfjet1_e},
              {"vbfjet1_deepflavor_b", VBFjet1_btag_deepFlavor}, {"vbfjet1_deepflavor_cvsb", CvsB_vbf1}, {"vbfjet1_deepflavor_cvsl", CvsL_vbf1}, {"vbfjet1_hhbtag", HHbtag_vbf1},
              {"vbfjet2_pt", vbfjet2_pt}, {"vbfjet2_eta", vbfjet2_eta}, {"vbfjet2_phi", vbfjet2_phi}, {"vbfjet2_e", vbfjet2_e},
              {"vbfjet2_deepflavor_b", VBFjet2_btag_deepFlavor}, {"vbfjet2_deepflavor_cvsb", CvsB_vbf2}, {"vbfjet2_deepflavor_cvsl", CvsL_vbf2}, {"vbfjet2_hhbtag", HHbtag_vbf2},
              {"lep1_pt", dau1_pt}, {"lep1_eta", dau1_eta}, {"lep1_phi", dau1_phi}, {"lep1_e", dau1_e},
              {"lep2_pt", dau2_pt}, {"lep2_eta", dau2_eta}, {"lep2_phi", dau2_phi}, {"lep2_e", dau2_e},
              {"met_pt", met.Pt()}, {"met_phi", met.Phi()},
              {"bh_pt", (bjet1+bjet2).Pt()}, {"bh_eta", (bjet1+bjet2).Eta()}, {"bh_phi", (bjet1+bjet2).Phi()}, {"bh_e", (bjet1+bjet2).E()},
              {"tauh_sv_pt", tauH_SVFIT_pt}, {"tauh_sv_eta", tauH_SVFIT_eta}, {"tauh_sv_phi", tauH_SVFIT_phi}, {"tauh_sv_e", svfit.E()}
            });
          }
          auto mdnnSM0_score_jes  = mci.predict(EventNumber, 0);
          auto mdnnSM1_score_jes  = mci.predict(EventNumber, 1);
          auto mdnnBSM0_score_jes = mci.predict(EventNumber, 2);
          for (uint k=0; k<mdnnSM0_score_jes.size(); k++)
          {
            mdnnSM0_output_jetup  [i].at(k) = mdnnSM0_score_jes.at(k).second;
            mdnnSM0_output_jetdown[i].at(k) = mdnnSM0_score_jes.at(k).second;
          }
          for (uint k=0; k<mdnnSM1_score_jes.size(); k++)
          {
            mdnnSM1_output_jetup  [i].at(k) = mdnnSM1_score_jes.at(k).second;
            mdnnSM1_output_jetdown[i].at(k) = mdnnSM1_score_jes.at(k).second;
          }
          for (uint k=0; k<mdnnBSM0_score_jes.size(); k++)
          {
            mdnnBSM0_output_jetup  [i].at(k) = mdnnBSM0_score_jes.at(k).second;
            mdnnBSM0_output_jetdown[i].at(k) = mdnnBSM0_score_jes.at(k).second;
          }
        }
        else /*isMC*/
        {
          // Build shifted b-jets, MET and VBF-jets
          TLorentzVector bjet1_jetup, bjet2_jetup, met_jetup;
          bjet1_jetup.SetPtEtaPhiM(bjet1_pt_raw_jetup->at(i), bjet1_eta, bjet1_phi, bjet1_mass_raw_jetup->at(i));
          bjet2_jetup.SetPtEtaPhiM(bjet2_pt_raw_jetup->at(i), bjet2_eta, bjet2_phi, bjet2_mass_raw_jetup->at(i));
          met_jetup  .SetPxPyPzE(METx_jetup->at(i), METy_jetup->at(i), 0, std::hypot(METx_jetup->at(i), METy_jetup->at(i)));
          TVector2 ptmiss_jetup = TVector2(METx_jetup->at(i), METy_jetup->at(i));

          TLorentzVector bjet1_jetdown, bjet2_jetdown, met_jetdown;
          bjet1_jetdown.SetPtEtaPhiM(bjet1_pt_raw_jetdown->at(i), bjet1_eta, bjet1_phi, bjet1_mass_raw_jetdown->at(i));
          bjet2_jetdown.SetPtEtaPhiM(bjet2_pt_raw_jetdown->at(i), bjet2_eta, bjet2_phi, bjet2_mass_raw_jetdown->at(i));
          met_jetdown  .SetPxPyPzE(METx_jetdown->at(i), METy_jetdown->at(i), 0, std::hypot(METx_jetdown->at(i), METy_jetdown->at(i)));
          TVector2 ptmiss_jetdown = TVector2(METx_jetdown->at(i), METy_jetdown->at(i));

          TLorentzVector vbfjet1_jetup, vbfjet2_jetup;
          vbfjet1_jetup.SetPtEtaPhiM(VBFjet1_pt_jetup->at(i), vbfjet1_eta, vbfjet1_phi, VBFjet1_mass_jetup->at(i));
          vbfjet2_jetup.SetPtEtaPhiM(VBFjet2_pt_jetup->at(i), vbfjet2_eta, vbfjet1_phi, VBFjet2_mass_jetup->at(i));

          TLorentzVector vbfjet1_jetdown, vbfjet2_jetdown;
          vbfjet1_jetdown.SetPtEtaPhiM(VBFjet1_pt_jetdown->at(i), vbfjet1_eta, vbfjet1_phi, VBFjet1_mass_jetdown->at(i));
          vbfjet2_jetdown.SetPtEtaPhiM(VBFjet2_pt_jetdown->at(i), vbfjet2_eta, vbfjet1_phi, VBFjet2_mass_jetdown->at(i));

          // Declare other useful shifted variables
          float HHKin_mass_jetup, HHKin_chi2_jetup, HHKin_mass_jetdown, HHKin_chi2_jetdown;
          float MT2_jetup, MT2_jetdown;
          float tauH_SVFIT_pt_jetup, tauH_SVFIT_eta_jetup, tauH_SVFIT_phi_jetup;
          float tauH_SVFIT_pt_jetdown, tauH_SVFIT_eta_jetdown, tauH_SVFIT_phi_jetdown;

          if (doKinFit)
          {
            HHKinFit2::HHKinFitMasterHeavyHiggs kinFits_jetup = HHKinFit2::HHKinFitMasterHeavyHiggs(bjet1_jetup, bjet2_jetup, tau1, tau2, ptmiss_jetup, stableMetCov, bjet1_JER_jetup->at(i), bjet2_JER_jetup->at(i)) ;
            kinFits_jetup.addHypo(hypo_mh1,hypo_mh2);
            bool wrongHHK_jetup =false;
            try {kinFits_jetup.fit();}
            catch(HHKinFit2::HHInvMConstraintException   &e) {wrongHHK_jetup=true;}
            catch(HHKinFit2::HHEnergyConstraintException &e) {wrongHHK_jetup=true;}
            catch(HHKinFit2::HHEnergyRangeException      &e) {wrongHHK_jetup=true;}
            if(!wrongHHK_jetup)
            {
              HHKin_mass_jetup = kinFits_jetup.getMH();
              HHKin_chi2_jetup = kinFits_jetup.getChi2();
            }
            else
            {
              HHKin_mass_jetup = -333.;
              HHKin_chi2_jetup = 0.;
            }

            HHKinFit2::HHKinFitMasterHeavyHiggs kinFits_jetdown = HHKinFit2::HHKinFitMasterHeavyHiggs(bjet1_jetdown, bjet2_jetdown, tau1, tau2, ptmiss_jetdown, stableMetCov, bjet1_JER_jetdown->at(i), bjet2_JER_jetdown->at(i));
            kinFits_jetdown.addHypo(hypo_mh1,hypo_mh2);
            bool wrongHHK_jetdown =false;
            try {kinFits_jetdown.fit();}
            catch(HHKinFit2::HHInvMConstraintException   &e) {wrongHHK_jetdown=true;}
            catch(HHKinFit2::HHEnergyConstraintException &e) {wrongHHK_jetdown=true;}
            catch(HHKinFit2::HHEnergyRangeException      &e) {wrongHHK_jetdown=true;}
            if(!wrongHHK_jetdown)
            {
              HHKin_mass_jetdown = kinFits_jetdown.getMH();
              HHKin_chi2_jetdown = kinFits_jetdown.getChi2();
            }
            else
            {
              HHKin_mass_jetdown = -333.;
              HHKin_chi2_jetdown = 0.;
            }
          }

          if (doMT2)
          {
            MT2_jetup = asymm_mt2_lester_bisect::get_mT2( bjet1_jetup.M(), bjet1_jetup.Px(), bjet1_jetup.Py(),
                                                          bjet2_jetup.M(), bjet2_jetup.Px(), bjet2_jetup.Py(),
                                                          (tau1.Px() + tau2.Px() + met_jetup.Px()),
                                                          (tau1.Py() + tau2.Py() + met_jetup.Py()),
                                                          tau1.M(), tau2.M(), desiredPrecisionOnMt2);

            MT2_jetdown = asymm_mt2_lester_bisect::get_mT2( bjet1_jetdown.M(), bjet1_jetdown.Px(), bjet1_jetdown.Py(),
                                                            bjet2_jetdown.M(), bjet2_jetdown.Px(), bjet2_jetdown.Py(),
                                                            (tau1.Px() + tau2.Px() + met_jetdown.Px()),
                                                            (tau1.Py() + tau2.Py() + met_jetdown.Py()),
                                                            tau1.M(), tau2.M(), desiredPrecisionOnMt2);
          }

          if (doSVfit)
          {
            SVfitKLUBinterface algo_jetup(0, tau1, tau2, met_jetup, stableMetCov, pType, DM1, DM2);
            std::vector<double> svfitRes_jetup = algo_jetup.FitAndGetResult();
            tauH_SVFIT_pt_jetup         = svfitRes_jetup.at(0);
            tauH_SVFIT_eta_jetup        = svfitRes_jetup.at(1);
            tauH_SVFIT_phi_jetup        = svfitRes_jetup.at(2);
            tauH_SVFIT_mass_jetup.at(i) = svfitRes_jetup.at(3);

            SVfitKLUBinterface algo_jetdown(0, tau1, tau2, met_jetdown, stableMetCov, pType, DM1, DM2);
            std::vector<double> svfitRes_jetdown = algo_jetdown.FitAndGetResult();
            tauH_SVFIT_pt_jetdown         = svfitRes_jetdown.at(0);
            tauH_SVFIT_eta_jetdown        = svfitRes_jetdown.at(1);
            tauH_SVFIT_phi_jetdown        = svfitRes_jetdown.at(2);
            tauH_SVFIT_mass_jetdown.at(i) = svfitRes_jetdown.at(3);
          }

          // --- --- --- JES DNN/BDT quantities --- --- ---
          TLorentzVector svfit_jetup;
          svfit_jetup.SetPtEtaPhiM(tauH_SVFIT_pt_jetup, tauH_SVFIT_eta_jetup, tauH_SVFIT_phi_jetup, tauH_SVFIT_mass_jetup.at(i));
          float mTtot_jetup                         = Calculate_TotalMT(tau1, tau2, met_jetup);
          float pzeta_vis_jetup                     = Calculate_visiblePzeta(tau1, tau2);
          float pzeta_jetup                         = Calculate_Pzeta(tau1, tau2, met_jetup);
          float BDT_ditau_deltaPhi_jetup            = ROOT::Math::VectorUtil::DeltaPhi(tau1, tau2);
          float BDT_tauHsvfitMet_deltaPhi_jetup     = ROOT::Math::VectorUtil::DeltaPhi(svfit_jetup, met_jetup);
          float mT_tauH_MET_jetup                   = Calculate_MT( (tau1+tau2)+met_jetup, met_jetup);
          float BDT_MX_jetup                        = Calculate_MX(tau1, tau2, bjet1_jetup, bjet2_jetup, met_jetup);
          float BDT_bH_tauH_MET_InvMass_jetup       = ROOT::Math::VectorUtil::InvariantMass((bjet1_jetup+bjet2_jetup), (tau1+tau2)+met_jetup);
          float BDT_bH_tauH_SVFIT_InvMass_jetup     = ROOT::Math::VectorUtil::InvariantMass((bjet1_jetup+bjet2_jetup), svfit_jetup);
          float BDT_bH_tauH_InvMass_jetup           = ROOT::Math::VectorUtil::InvariantMass((bjet1_jetup+bjet2_jetup), (tau1+tau2));
          float BDT_MET_bH_cosTheta_jetup           = Calculate_cosTheta_2bodies(getLVfromTLV(met_jetup), getLVfromTLV(bjet1_jetup+bjet2_jetup));
          bool KinFitConv_jetup                     = HHKin_chi2_jetup > 0;
          bool SVfitConv_jetup                      = tauH_SVFIT_mass_jetup.at(i) > 0;

          TLorentzVector svfit_jetdown;
          svfit_jetdown.SetPtEtaPhiM(tauH_SVFIT_pt_jetdown, tauH_SVFIT_eta_jetdown, tauH_SVFIT_phi_jetdown, tauH_SVFIT_mass_jetdown.at(i));
          float mTtot_jetdown                         = Calculate_TotalMT(tau1, tau2, met_jetdown);
          float pzeta_vis_jetdown                     = Calculate_visiblePzeta(tau1, tau2);
          float pzeta_jetdown                         = Calculate_Pzeta(tau1, tau2, met_jetdown);
          float BDT_ditau_deltaPhi_jetdown            = ROOT::Math::VectorUtil::DeltaPhi(tau1, tau2);
          float BDT_tauHsvfitMet_deltaPhi_jetdown     = ROOT::Math::VectorUtil::DeltaPhi(svfit_jetdown, met_jetdown);
          float mT_tauH_MET_jetdown                   = Calculate_MT( (tau1+tau2)+met_jetdown, met_jetdown);
          float BDT_MX_jetdown                        = Calculate_MX(tau1, tau2, bjet1_jetdown, bjet2_jetdown, met_jetdown);
          float BDT_bH_tauH_MET_InvMass_jetdown       = ROOT::Math::VectorUtil::InvariantMass((bjet1_jetdown+bjet2_jetdown), (tau1+tau2)+met_jetdown);
          float BDT_bH_tauH_SVFIT_InvMass_jetdown     = ROOT::Math::VectorUtil::InvariantMass((bjet1_jetdown+bjet2_jetdown), svfit_jetdown);
          float BDT_bH_tauH_InvMass_jetdown           = ROOT::Math::VectorUtil::InvariantMass((bjet1_jetdown+bjet2_jetdown), (tau1+tau2));
          float BDT_MET_bH_cosTheta_jetdown           = Calculate_cosTheta_2bodies(getLVfromTLV(met_jetdown), getLVfromTLV(bjet1_jetdown+bjet2_jetdown));
          bool KinFitConv_jetdown                     = HHKin_chi2_jetdown > 0;
          bool SVfitConv_jetdown                      = tauH_SVFIT_mass_jetdown.at(i) > 0;

          if (doDNN)
          {
            DNNreader.SetShiftedInputs(bjet1_jetup, bjet2_jetup, tau1, tau2, vbfjet1_jetup, vbfjet2_jetup, met_jetup, svfit_jetup,
                HHKin_mass_jetup, HHKin_chi2_jetup, KinFitConv_jetup, SVfitConv_jetup, MT2_jetup);
            std::vector<float> outs_jetup = DNNreader.GetPredictions();
            DNNoutSM_kl_1_jetup.at(i) = outs_jetup.at(0);

            DNNreader.SetShiftedInputs(bjet1_jetdown, bjet2_jetdown, tau1, tau2, vbfjet1_jetdown, vbfjet2_jetdown, met_jetdown, svfit_jetdown,
                HHKin_mass_jetdown, HHKin_chi2_jetdown, KinFitConv_jetdown, SVfitConv_jetdown, MT2_jetdown);
            std::vector<float> outs_jetdown = DNNreader.GetPredictions();
            DNNoutSM_kl_1_jetdown.at(i) = outs_jetdown.at(0);
          }

          if (doMult)
          {
            mci.clearInputs();
            for (uint j=0; j<mci.getNumberOfModels(); j++)
            {
              mci.setInputs(j,
              {
                {"is_mutau", mdnn_isMuTau}, {"is_etau", mdnn_isETau}, {"is_tautau", mdnn_isTauTau},
                {"bjet1_pt", bjet1_jetup.Pt()}, {"bjet1_eta", bjet1_jetup.Eta()}, {"bjet1_phi", bjet1_jetup.Phi()}, {"bjet1_e", bjet1_jetup.E()},
                {"bjet1_deepflavor_b", bjet1_bID_deepFlavor}, {"bjet1_deepflavor_cvsb", CvsB_b1}, {"bjet1_deepflavor_cvsl", CvsL_b1}, {"bjet1_hhbtag", HHbtag_b1},
                {"bjet2_pt", bjet2_jetup.Pt()}, {"bjet2_eta", bjet2_jetup.Eta()}, {"bjet2_phi", bjet2_jetup.Phi()}, {"bjet2_e", bjet2_jetup.E()},
                {"bjet2_deepflavor_b", bjet2_bID_deepFlavor}, {"bjet2_deepflavor_cvsb", CvsB_b2}, {"bjet2_deepflavor_cvsl", CvsL_b2}, {"bjet2_hhbtag", HHbtag_b2},
                {"vbfjet1_pt", vbfjet1_jetup.Pt()}, {"vbfjet1_eta", vbfjet1_jetup.Eta()}, {"vbfjet1_phi", vbfjet1_jetup.Phi()}, {"vbfjet1_e", vbfjet1_jetup.E()},
                {"vbfjet1_deepflavor_b", VBFjet1_btag_deepFlavor}, {"vbfjet1_deepflavor_cvsb", CvsB_vbf1}, {"vbfjet1_deepflavor_cvsl", CvsL_vbf1}, {"vbfjet1_hhbtag", HHbtag_vbf1},
                {"vbfjet2_pt", vbfjet2_jetup.Pt()}, {"vbfjet2_eta", vbfjet2_jetup.Eta()}, {"vbfjet2_phi", vbfjet2_jetup.Phi()}, {"vbfjet2_e", vbfjet2_jetup.E()},
                {"vbfjet2_deepflavor_b", VBFjet2_btag_deepFlavor}, {"vbfjet2_deepflavor_cvsb", CvsB_vbf2}, {"vbfjet2_deepflavor_cvsl", CvsL_vbf2}, {"vbfjet2_hhbtag", HHbtag_vbf2},
                {"lep1_pt", dau1_pt}, {"lep1_eta", dau1_eta}, {"lep1_phi", dau1_phi}, {"lep1_e", dau1_e},
                {"lep2_pt", dau2_pt}, {"lep2_eta", dau2_eta}, {"lep2_phi", dau2_phi}, {"lep2_e", dau2_e},
                {"met_pt", met_jetup.Pt()}, {"met_phi", met_jetup.Phi()},
                {"bh_pt", (bjet1_jetup+bjet2_jetup).Pt()}, {"bh_eta", (bjet1_jetup+bjet2_jetup).Eta()}, {"bh_phi", (bjet1_jetup+bjet2_jetup).Phi()}, {"bh_e", (bjet1_jetup+bjet2_jetup).E()},
                {"tauh_sv_pt", svfit_jetup.Pt()}, {"tauh_sv_eta", svfit_jetup.Eta()}, {"tauh_sv_phi", svfit_jetup.Phi()}, {"tauh_sv_e", svfit_jetup.E()}
              });
            }
            auto mdnnSM0_score_jetup  = mci.predict(EventNumber, 0);
            auto mdnnSM1_score_jetup  = mci.predict(EventNumber, 1);
            auto mdnnBSM0_score_jetup = mci.predict(EventNumber, 2);
            for (uint k=0; k<mdnnSM0_score_jetup.size(); k++)
              mdnnSM0_output_jetup[i].at(k) = mdnnSM0_score_jetup.at(k).second;
            for (uint k=0; k<mdnnSM1_score_jetup.size(); k++)
              mdnnSM1_output_jetup[i].at(k) = mdnnSM1_score_jetup.at(k).second;
            for (uint k=0; k<mdnnBSM0_score_jetup.size(); k++)
              mdnnBSM0_output_jetup[i].at(k) = mdnnBSM0_score_jetup.at(k).second;

            mci.clearInputs();
            for (uint j=0; j<mci.getNumberOfModels(); j++)
            {
              mci.setInputs(j,
              {
                {"is_mutau", mdnn_isMuTau}, {"is_etau", mdnn_isETau}, {"is_tautau", mdnn_isTauTau},
                {"bjet1_pt", bjet1_jetdown.Pt()}, {"bjet1_eta", bjet1_jetdown.Eta()}, {"bjet1_phi", bjet1_jetdown.Phi()}, {"bjet1_e", bjet1_jetdown.E()},
                {"bjet1_deepflavor_b", bjet1_bID_deepFlavor}, {"bjet1_deepflavor_cvsb", CvsB_b1}, {"bjet1_deepflavor_cvsl", CvsL_b1}, {"bjet1_hhbtag", HHbtag_b1},
                {"bjet2_pt", bjet2_jetdown.Pt()}, {"bjet2_eta", bjet2_jetdown.Eta()}, {"bjet2_phi", bjet2_jetdown.Phi()}, {"bjet2_e", bjet2_jetdown.E()},
                {"bjet2_deepflavor_b", bjet2_bID_deepFlavor}, {"bjet2_deepflavor_cvsb", CvsB_b2}, {"bjet2_deepflavor_cvsl", CvsL_b2}, {"bjet2_hhbtag", HHbtag_b2},
                {"vbfjet1_pt", vbfjet1_jetdown.Pt()}, {"vbfjet1_eta", vbfjet1_jetdown.Eta()}, {"vbfjet1_phi", vbfjet1_jetdown.Phi()}, {"vbfjet1_e", vbfjet1_jetdown.E()},
                {"vbfjet1_deepflavor_b", VBFjet1_btag_deepFlavor}, {"vbfjet1_deepflavor_cvsb", CvsB_vbf1}, {"vbfjet1_deepflavor_cvsl", CvsL_vbf1}, {"vbfjet1_hhbtag", HHbtag_vbf1},
                {"vbfjet2_pt", vbfjet2_jetdown.Pt()}, {"vbfjet2_eta", vbfjet2_jetdown.Eta()}, {"vbfjet2_phi", vbfjet2_jetdown.Phi()}, {"vbfjet2_e", vbfjet2_jetdown.E()},
                {"vbfjet2_deepflavor_b", VBFjet2_btag_deepFlavor}, {"vbfjet2_deepflavor_cvsb", CvsB_vbf2}, {"vbfjet2_deepflavor_cvsl", CvsL_vbf2}, {"vbfjet2_hhbtag", HHbtag_vbf2},
                {"lep1_pt", dau1_pt}, {"lep1_eta", dau1_eta}, {"lep1_phi", dau1_phi}, {"lep1_e", dau1_e},
                {"lep2_pt", dau2_pt}, {"lep2_eta", dau2_eta}, {"lep2_phi", dau2_phi}, {"lep2_e", dau2_e},
                {"met_pt", met_jetdown.Pt()}, {"met_phi", met_jetdown.Phi()},
                {"bh_pt", (bjet1_jetdown+bjet2_jetdown).Pt()}, {"bh_eta", (bjet1_jetdown+bjet2_jetdown).Eta()}, {"bh_phi", (bjet1_jetdown+bjet2_jetdown).Phi()}, {"bh_e", (bjet1_jetdown+bjet2_jetdown).E()},
                {"tauh_sv_pt", svfit_jetdown.Pt()}, {"tauh_sv_eta", svfit_jetdown.Eta()}, {"tauh_sv_phi", svfit_jetdown.Phi()}, {"tauh_sv_e", svfit_jetdown.E()}
              });
            }
            auto mdnnSM0_score_jetdown  = mci.predict(EventNumber, 0);
            auto mdnnSM1_score_jetdown  = mci.predict(EventNumber, 1);
            auto mdnnBSM0_score_jetdown = mci.predict(EventNumber, 2);
            for (uint k=0; k<mdnnSM0_score_jetdown.size(); k++)
              mdnnSM0_output_jetdown[i].at(k) = mdnnSM0_score_jetdown.at(k).second;
            for (uint k=0; k<mdnnSM1_score_jetdown.size(); k++)
              mdnnSM1_output_jetdown[i].at(k) = mdnnSM1_score_jetdown.at(k).second;
            for (uint k=0; k<mdnnBSM0_score_jetdown.size(); k++)
              mdnnBSM0_output_jetdown[i].at(k) = mdnnBSM0_score_jetdown.at(k).second;
          }

          if (doBDT)
          {
            BDTreader.SetInputValues(bjet2_jetup.Pt(), (bjet1_jetup+bjet2_jetup).Pt(), tau1.Pt(),
              tau2.Pt(), svfit_jetup.Pt(), BDT_channel,
              BDT_HT20_jetup->at(i), pzeta_jetup, pzeta_vis_jetup, BDT_ditau_deltaPhi_jetup,
              BDT_tauHsvfitMet_deltaPhi_jetup, mT_tauH_MET_jetup, mTtot_jetup, MT2_jetup,
              BDT_MX_jetup, BDT_bH_tauH_MET_InvMass_jetup, BDT_bH_tauH_SVFIT_InvMass_jetup,
              BDT_bH_tauH_InvMass_jetup, HHKin_mass_jetup, HHKin_chi2_jetup, BDT_MET_bH_cosTheta_jetup);
            std::vector<float> BDTouts_jetup = BDTreader.GetPredictions();
            BDToutSM_kl_1_jetup.at(i) = BDTouts_jetup.at(0);

            BDTreader.SetInputValues(bjet2_jetdown.Pt(), (bjet1_jetdown+bjet2_jetdown).Pt(), tau1.Pt(),
              tau2.Pt(), svfit_jetdown.Pt(), BDT_channel,
              BDT_HT20_jetdown->at(i), pzeta_jetdown, pzeta_vis_jetdown, BDT_ditau_deltaPhi_jetdown,
              BDT_tauHsvfitMet_deltaPhi_jetdown, mT_tauH_MET_jetdown, mTtot_jetdown, MT2_jetdown,
              BDT_MX_jetdown, BDT_bH_tauH_MET_InvMass_jetdown, BDT_bH_tauH_SVFIT_InvMass_jetdown,
              BDT_bH_tauH_InvMass_jetdown, HHKin_mass_jetdown, HHKin_chi2_jetdown, BDT_MET_bH_cosTheta_jetdown);
            std::vector<float> BDTouts_jetdown = BDTreader.GetPredictions();
            BDToutSM_kl_1_jetdown.at(i) = BDTouts_jetdown.at(0);
          }
        } // End isMC
      } // End loop on JES
    }  // End doSplitJES
    // Timing info
    auto end_splitJES = high_resolution_clock::now();

    // ---- ---- ---- ---- ---- ---- ----
    // ---- ----  Do total JES  ---- ----
    // ---- ---- ---- ---- ---- ---- ----
    // Timing info
    auto start_totalJES = high_resolution_clock::now();
    if (doTotalJES)
    {
      if (isData)
      {
        // Fill with nominal value
        tauH_SVFIT_mass_jetupTot   = tauH_SVFIT_mass;
        tauH_SVFIT_mass_jetdownTot = tauH_SVFIT_mass;
        DNNoutSM_kl_1_jetupTot     = DNNoutSM_kl_1;
        DNNoutSM_kl_1_jetdownTot   = DNNoutSM_kl_1;
        BDToutSM_kl_1_jetupTot     = BDToutSM_kl_1;
        BDToutSM_kl_1_jetdownTot   = BDToutSM_kl_1;

        // VBF multiclass
        mci.clearInputs();
        for (uint j=0; j<mci.getNumberOfModels(); j++)
        {
          mci.setInputs(j,
          {
            {"is_mutau", mdnn_isMuTau}, {"is_etau", mdnn_isETau}, {"is_tautau", mdnn_isTauTau},
            {"bjet1_pt", bjet1_pt}, {"bjet1_eta", bjet1_eta}, {"bjet1_phi", bjet1_phi}, {"bjet1_e", bjet1_e},
            {"bjet1_deepflavor_b", bjet1_bID_deepFlavor}, {"bjet1_deepflavor_cvsb", CvsB_b1}, {"bjet1_deepflavor_cvsl", CvsL_b1}, {"bjet1_hhbtag", HHbtag_b1},
            {"bjet2_pt", bjet2_pt}, {"bjet2_eta", bjet2_eta}, {"bjet2_phi", bjet2_phi}, {"bjet2_e", bjet2_e},
            {"bjet2_deepflavor_b", bjet2_bID_deepFlavor}, {"bjet2_deepflavor_cvsb", CvsB_b2}, {"bjet2_deepflavor_cvsl", CvsL_b2}, {"bjet2_hhbtag", HHbtag_b2},
            {"vbfjet1_pt", vbfjet1_pt}, {"vbfjet1_eta", vbfjet1_eta}, {"vbfjet1_phi", vbfjet1_phi}, {"vbfjet1_e", vbfjet1_e},
            {"vbfjet1_deepflavor_b", VBFjet1_btag_deepFlavor}, {"vbfjet1_deepflavor_cvsb", CvsB_vbf1}, {"vbfjet1_deepflavor_cvsl", CvsL_vbf1}, {"vbfjet1_hhbtag", HHbtag_vbf1},
            {"vbfjet2_pt", vbfjet2_pt}, {"vbfjet2_eta", vbfjet2_eta}, {"vbfjet2_phi", vbfjet2_phi}, {"vbfjet2_e", vbfjet2_e},
            {"vbfjet2_deepflavor_b", VBFjet2_btag_deepFlavor}, {"vbfjet2_deepflavor_cvsb", CvsB_vbf2}, {"vbfjet2_deepflavor_cvsl", CvsL_vbf2}, {"vbfjet2_hhbtag", HHbtag_vbf2},
            {"lep1_pt", dau1_pt}, {"lep1_eta", dau1_eta}, {"lep1_phi", dau1_phi}, {"lep1_e", dau1_e},
            {"lep2_pt", dau2_pt}, {"lep2_eta", dau2_eta}, {"lep2_phi", dau2_phi}, {"lep2_e", dau2_e},
            {"met_pt", met.Pt()}, {"met_phi", met.Phi()},
            {"bh_pt", (bjet1+bjet2).Pt()}, {"bh_eta", (bjet1+bjet2).Eta()}, {"bh_phi", (bjet1+bjet2).Phi()}, {"bh_e", (bjet1+bjet2).E()},
            {"tauh_sv_pt", tauH_SVFIT_pt}, {"tauh_sv_eta", tauH_SVFIT_eta}, {"tauh_sv_phi", tauH_SVFIT_phi}, {"tauh_sv_e", svfit.E()}
          });
        }
        auto mdnnSM0_score_jetTot  = mci.predict(EventNumber, 0);
        auto mdnnSM1_score_jetTot  = mci.predict(EventNumber, 1);
        auto mdnnBSM0_score_jetTot = mci.predict(EventNumber, 2);
        for (uint k=0; k<mdnnSM0_score_jetTot.size(); k++)
        {
          mdnnSM0_output_jetupTot.at(k)   = mdnnSM0_score_jetTot.at(k).second;
          mdnnSM0_output_jetdownTot.at(k) = mdnnSM0_score_jetTot.at(k).second;
        }
        for (uint k=0; k<mdnnSM1_score_jetTot.size(); k++)
        {
          mdnnSM1_output_jetupTot.at(k)   = mdnnSM1_score_jetTot.at(k).second;
          mdnnSM1_output_jetdownTot.at(k) = mdnnSM1_score_jetTot.at(k).second;
        }
        for (uint k=0; k<mdnnBSM0_score_jetTot.size(); k++)
        {
          mdnnBSM0_output_jetupTot.at(k)   = mdnnBSM0_score_jetTot.at(k).second;
          mdnnBSM0_output_jetdownTot.at(k) = mdnnBSM0_score_jetTot.at(k).second;
        }
      }
      else /*isMC*/
      {
        // Build shifted b-jets, MET and VBF-jets
        TLorentzVector bjet1_jetupTot, bjet2_jetupTot, met_jetupTot;
        bjet1_jetupTot.SetPtEtaPhiM(bjet1_pt_raw_jetupTot, bjet1_eta, bjet1_phi, bjet1_mass_raw_jetupTot);
        bjet2_jetupTot.SetPtEtaPhiM(bjet2_pt_raw_jetupTot, bjet2_eta, bjet2_phi, bjet2_mass_raw_jetupTot);
        met_jetupTot  .SetPxPyPzE(METx_jetupTot, METy_jetupTot, 0, std::hypot(METx_jetupTot, METy_jetupTot));
        TVector2 ptmiss_jetupTot = TVector2(METx_jetupTot, METy_jetupTot);

        TLorentzVector bjet1_jetdownTot, bjet2_jetdownTot, met_jetdownTot;
        bjet1_jetdownTot.SetPtEtaPhiM(bjet1_pt_raw_jetdownTot, bjet1_eta, bjet1_phi, bjet1_mass_raw_jetdownTot);
        bjet2_jetdownTot.SetPtEtaPhiM(bjet2_pt_raw_jetdownTot, bjet2_eta, bjet2_phi, bjet2_mass_raw_jetdownTot);
        met_jetdownTot  .SetPxPyPzE(METx_jetdownTot, METy_jetdownTot, 0, std::hypot(METx_jetdownTot, METy_jetdownTot));
        TVector2 ptmiss_jetdownTot = TVector2(METx_jetdownTot, METy_jetdownTot);

        TLorentzVector vbfjet1_jetupTot, vbfjet2_jetupTot;
        vbfjet1_jetupTot.SetPtEtaPhiM(VBFjet1_pt_jetupTot, vbfjet1_eta, vbfjet1_phi, VBFjet1_mass_jetupTot);
        vbfjet2_jetupTot.SetPtEtaPhiM(VBFjet2_pt_jetupTot, vbfjet2_eta, vbfjet1_phi, VBFjet2_mass_jetupTot);

        TLorentzVector vbfjet1_jetdownTot, vbfjet2_jetdownTot;
        vbfjet1_jetdownTot.SetPtEtaPhiM(VBFjet1_pt_jetdownTot, vbfjet1_eta, vbfjet1_phi, VBFjet1_mass_jetdownTot);
        vbfjet2_jetdownTot.SetPtEtaPhiM(VBFjet2_pt_jetdownTot, vbfjet2_eta, vbfjet1_phi, VBFjet2_mass_jetdownTot);

        // Declare other useful shifted variables
        float HHKin_mass_jetupTot, HHKin_chi2_jetupTot, HHKin_mass_jetdownTot, HHKin_chi2_jetdownTot;
        float MT2_jetupTot, MT2_jetdownTot;
        float tauH_SVFIT_pt_jetupTot, tauH_SVFIT_eta_jetupTot, tauH_SVFIT_phi_jetupTot;
        float tauH_SVFIT_pt_jetdownTot, tauH_SVFIT_eta_jetdownTot, tauH_SVFIT_phi_jetdownTot;

        if (doKinFit)
        {
          HHKinFit2::HHKinFitMasterHeavyHiggs kinFits_jetupTot = HHKinFit2::HHKinFitMasterHeavyHiggs(bjet1_jetupTot, bjet2_jetupTot, tau1, tau2, ptmiss_jetupTot, stableMetCov, bjet1_JER_jetupTot, bjet2_JER_jetupTot) ;
          kinFits_jetupTot.addHypo(hypo_mh1,hypo_mh2);
          bool wrongHHK_jetupTot =false;
          try {kinFits_jetupTot.fit();}
          catch(HHKinFit2::HHInvMConstraintException   &e) {wrongHHK_jetupTot=true;}
          catch(HHKinFit2::HHEnergyConstraintException &e) {wrongHHK_jetupTot=true;}
          catch(HHKinFit2::HHEnergyRangeException      &e) {wrongHHK_jetupTot=true;}
          if(!wrongHHK_jetupTot)
          {
            HHKin_mass_jetupTot = kinFits_jetupTot.getMH();
            HHKin_chi2_jetupTot = kinFits_jetupTot.getChi2();
          }
          else
          {
            HHKin_mass_jetupTot = -333.;
            HHKin_chi2_jetupTot = 0.;
          }

          HHKinFit2::HHKinFitMasterHeavyHiggs kinFits_jetdownTot = HHKinFit2::HHKinFitMasterHeavyHiggs(bjet1_jetdownTot, bjet2_jetdownTot, tau1, tau2, ptmiss_jetdownTot, stableMetCov, bjet1_JER_jetdownTot, bjet2_JER_jetdownTot);
          kinFits_jetdownTot.addHypo(hypo_mh1,hypo_mh2);
          bool wrongHHK_jetdownTot =false;
          try {kinFits_jetdownTot.fit();}
          catch(HHKinFit2::HHInvMConstraintException   &e) {wrongHHK_jetdownTot=true;}
          catch(HHKinFit2::HHEnergyConstraintException &e) {wrongHHK_jetdownTot=true;}
          catch(HHKinFit2::HHEnergyRangeException      &e) {wrongHHK_jetdownTot=true;}
          if(!wrongHHK_jetdownTot)
          {
            HHKin_mass_jetdownTot = kinFits_jetdownTot.getMH();
            HHKin_chi2_jetdownTot = kinFits_jetdownTot.getChi2();
          }
          else
          {
            HHKin_mass_jetdownTot = -333.;
            HHKin_chi2_jetdownTot = 0.;
          }
        }

        if (doMT2)
        {
          MT2_jetupTot = asymm_mt2_lester_bisect::get_mT2( bjet1_jetupTot.M(), bjet1_jetupTot.Px(), bjet1_jetupTot.Py(),
                                                          bjet2_jetupTot.M(), bjet2_jetupTot.Px(), bjet2_jetupTot.Py(),
                                                          (tau1.Px() + tau2.Px() + met_jetupTot.Px()),
                                                          (tau1.Py() + tau2.Py() + met_jetupTot.Py()),
                                                          tau1.M(), tau2.M(), desiredPrecisionOnMt2);

          MT2_jetdownTot = asymm_mt2_lester_bisect::get_mT2( bjet1_jetdownTot.M(), bjet1_jetdownTot.Px(), bjet1_jetdownTot.Py(),
                                                            bjet2_jetdownTot.M(), bjet2_jetdownTot.Px(), bjet2_jetdownTot.Py(),
                                                            (tau1.Px() + tau2.Px() + met_jetdownTot.Px()),
                                                            (tau1.Py() + tau2.Py() + met_jetdownTot.Py()),
                                                            tau1.M(), tau2.M(), desiredPrecisionOnMt2);
        }

        if (doSVfit)
        {
          SVfitKLUBinterface algo_jetupTot(0, tau1, tau2, met_jetupTot, stableMetCov, pType, DM1, DM2);
          std::vector<double> svfitRes_jetupTot = algo_jetupTot.FitAndGetResult();
          tauH_SVFIT_pt_jetupTot   = svfitRes_jetupTot.at(0);
          tauH_SVFIT_eta_jetupTot  = svfitRes_jetupTot.at(1);
          tauH_SVFIT_phi_jetupTot  = svfitRes_jetupTot.at(2);
          tauH_SVFIT_mass_jetupTot = svfitRes_jetupTot.at(3);

          SVfitKLUBinterface algo_jetdownTot(0, tau1, tau2, met_jetdownTot, stableMetCov, pType, DM1, DM2);
          std::vector<double> svfitRes_jetdownTot = algo_jetdownTot.FitAndGetResult();
          tauH_SVFIT_pt_jetdownTot   = svfitRes_jetdownTot.at(0);
          tauH_SVFIT_eta_jetdownTot  = svfitRes_jetdownTot.at(1);
          tauH_SVFIT_phi_jetdownTot  = svfitRes_jetdownTot.at(2);
          tauH_SVFIT_mass_jetdownTot = svfitRes_jetdownTot.at(3);
        }

        // --- --- --- JES Total DNN/BDT quantities --- --- ---
        TLorentzVector svfit_jetupTot;
        svfit_jetupTot.SetPtEtaPhiM(tauH_SVFIT_pt_jetupTot, tauH_SVFIT_eta_jetupTot, tauH_SVFIT_phi_jetupTot, tauH_SVFIT_mass_jetupTot);
        float mTtot_jetupTot                         = Calculate_TotalMT(tau1, tau2, met_jetupTot);
        float pzeta_vis_jetupTot                     = Calculate_visiblePzeta(tau1, tau2);
        float pzeta_jetupTot                         = Calculate_Pzeta(tau1, tau2, met_jetupTot);
        float BDT_ditau_deltaPhi_jetupTot            = ROOT::Math::VectorUtil::DeltaPhi(tau1, tau2);
        float BDT_tauHsvfitMet_deltaPhi_jetupTot     = ROOT::Math::VectorUtil::DeltaPhi(svfit_jetupTot, met_jetupTot);
        float mT_tauH_MET_jetupTot                   = Calculate_MT( (tau1+tau2)+met_jetupTot, met_jetupTot);
        float BDT_MX_jetupTot                        = Calculate_MX(tau1, tau2, bjet1_jetupTot, bjet2_jetupTot, met_jetupTot);
        float BDT_bH_tauH_MET_InvMass_jetupTot       = ROOT::Math::VectorUtil::InvariantMass((bjet1_jetupTot+bjet2_jetupTot), (tau1+tau2)+met_jetupTot);
        float BDT_bH_tauH_SVFIT_InvMass_jetupTot     = ROOT::Math::VectorUtil::InvariantMass((bjet1_jetupTot+bjet2_jetupTot), svfit_jetupTot);
        float BDT_bH_tauH_InvMass_jetupTot           = ROOT::Math::VectorUtil::InvariantMass((bjet1_jetupTot+bjet2_jetupTot), (tau1+tau2));
        float BDT_MET_bH_cosTheta_jetupTot           = Calculate_cosTheta_2bodies(getLVfromTLV(met_jetupTot), getLVfromTLV(bjet1_jetupTot+bjet2_jetupTot));
        bool KinFitConv_jetupTot                     = HHKin_chi2_jetupTot > 0;
        bool SVfitConv_jetupTot                      = tauH_SVFIT_mass_jetupTot > 0;

        TLorentzVector svfit_jetdownTot;
        svfit_jetdownTot.SetPtEtaPhiM(tauH_SVFIT_pt_jetdownTot, tauH_SVFIT_eta_jetdownTot, tauH_SVFIT_phi_jetdownTot, tauH_SVFIT_mass_jetdownTot);
        float mTtot_jetdownTot                         = Calculate_TotalMT(tau1, tau2, met_jetdownTot);
        float pzeta_vis_jetdownTot                     = Calculate_visiblePzeta(tau1, tau2);
        float pzeta_jetdownTot                         = Calculate_Pzeta(tau1, tau2, met_jetdownTot);
        float BDT_ditau_deltaPhi_jetdownTot            = ROOT::Math::VectorUtil::DeltaPhi(tau1, tau2);
        float BDT_tauHsvfitMet_deltaPhi_jetdownTot     = ROOT::Math::VectorUtil::DeltaPhi(svfit_jetdownTot, met_jetdownTot);
        float mT_tauH_MET_jetdownTot                   = Calculate_MT( (tau1+tau2)+met_jetdownTot, met_jetdownTot);
        float BDT_MX_jetdownTot                        = Calculate_MX(tau1, tau2, bjet1_jetdownTot, bjet2_jetdownTot, met_jetdownTot);
        float BDT_bH_tauH_MET_InvMass_jetdownTot       = ROOT::Math::VectorUtil::InvariantMass((bjet1_jetdownTot+bjet2_jetdownTot), (tau1+tau2)+met_jetdownTot);
        float BDT_bH_tauH_SVFIT_InvMass_jetdownTot     = ROOT::Math::VectorUtil::InvariantMass((bjet1_jetdownTot+bjet2_jetdownTot), svfit_jetdownTot);
        float BDT_bH_tauH_InvMass_jetdownTot           = ROOT::Math::VectorUtil::InvariantMass((bjet1_jetdownTot+bjet2_jetdownTot), (tau1+tau2));
        float BDT_MET_bH_cosTheta_jetdownTot           = Calculate_cosTheta_2bodies(getLVfromTLV(met_jetdownTot), getLVfromTLV(bjet1_jetdownTot+bjet2_jetdownTot));
        bool KinFitConv_jetdownTot                     = HHKin_chi2_jetdownTot > 0;
        bool SVfitConv_jetdownTot                      = tauH_SVFIT_mass_jetdownTot > 0;

        if (doDNN)
        {
          DNNreader.SetShiftedInputs(bjet1_jetupTot, bjet2_jetupTot, tau1, tau2, vbfjet1_jetupTot, vbfjet2_jetupTot, met_jetupTot, svfit_jetupTot,
              HHKin_mass_jetupTot, HHKin_chi2_jetupTot, KinFitConv_jetupTot, SVfitConv_jetupTot, MT2_jetupTot);
          std::vector<float> outs_jetupTot = DNNreader.GetPredictions();
          DNNoutSM_kl_1_jetupTot = outs_jetupTot.at(0);

          DNNreader.SetShiftedInputs(bjet1_jetdownTot, bjet2_jetdownTot, tau1, tau2, vbfjet1_jetdownTot, vbfjet2_jetdownTot, met_jetdownTot, svfit_jetdownTot,
              HHKin_mass_jetdownTot, HHKin_chi2_jetdownTot, KinFitConv_jetdownTot, SVfitConv_jetdownTot, MT2_jetdownTot);
          std::vector<float> outs_jetdownTot = DNNreader.GetPredictions();
          DNNoutSM_kl_1_jetdownTot = outs_jetdownTot.at(0);
        }

        if (doMult)
        {
          mci.clearInputs();
          for (uint j=0; j<mci.getNumberOfModels(); j++)
          {
            mci.setInputs(j,
            {
              {"is_mutau", mdnn_isMuTau}, {"is_etau", mdnn_isETau}, {"is_tautau", mdnn_isTauTau},
              {"bjet1_pt", bjet1_jetupTot.Pt()}, {"bjet1_eta", bjet1_jetupTot.Eta()}, {"bjet1_phi", bjet1_jetupTot.Phi()}, {"bjet1_e", bjet1_jetupTot.E()},
              {"bjet1_deepflavor_b", bjet1_bID_deepFlavor}, {"bjet1_deepflavor_cvsb", CvsB_b1}, {"bjet1_deepflavor_cvsl", CvsL_b1}, {"bjet1_hhbtag", HHbtag_b1},
              {"bjet2_pt", bjet2_jetupTot.Pt()}, {"bjet2_eta", bjet2_jetupTot.Eta()}, {"bjet2_phi", bjet2_jetupTot.Phi()}, {"bjet2_e", bjet2_jetupTot.E()},
              {"bjet2_deepflavor_b", bjet2_bID_deepFlavor}, {"bjet2_deepflavor_cvsb", CvsB_b2}, {"bjet2_deepflavor_cvsl", CvsL_b2}, {"bjet2_hhbtag", HHbtag_b2},
              {"vbfjet1_pt", vbfjet1_jetupTot.Pt()}, {"vbfjet1_eta", vbfjet1_jetupTot.Eta()}, {"vbfjet1_phi", vbfjet1_jetupTot.Phi()}, {"vbfjet1_e", vbfjet1_jetupTot.E()},
              {"vbfjet1_deepflavor_b", VBFjet1_btag_deepFlavor}, {"vbfjet1_deepflavor_cvsb", CvsB_vbf1}, {"vbfjet1_deepflavor_cvsl", CvsL_vbf1}, {"vbfjet1_hhbtag", HHbtag_vbf1},
              {"vbfjet2_pt", vbfjet2_jetupTot.Pt()}, {"vbfjet2_eta", vbfjet2_jetupTot.Eta()}, {"vbfjet2_phi", vbfjet2_jetupTot.Phi()}, {"vbfjet2_e", vbfjet2_jetupTot.E()},
              {"vbfjet2_deepflavor_b", VBFjet2_btag_deepFlavor}, {"vbfjet2_deepflavor_cvsb", CvsB_vbf2}, {"vbfjet2_deepflavor_cvsl", CvsL_vbf2}, {"vbfjet2_hhbtag", HHbtag_vbf2},
              {"lep1_pt", dau1_pt}, {"lep1_eta", dau1_eta}, {"lep1_phi", dau1_phi}, {"lep1_e", dau1_e},
              {"lep2_pt", dau2_pt}, {"lep2_eta", dau2_eta}, {"lep2_phi", dau2_phi}, {"lep2_e", dau2_e},
              {"met_pt", met_jetupTot.Pt()}, {"met_phi", met_jetupTot.Phi()},
              {"bh_pt", (bjet1_jetupTot+bjet2_jetupTot).Pt()}, {"bh_eta", (bjet1_jetupTot+bjet2_jetupTot).Eta()}, {"bh_phi", (bjet1_jetupTot+bjet2_jetupTot).Phi()}, {"bh_e", (bjet1_jetupTot+bjet2_jetupTot).E()},
              {"tauh_sv_pt", svfit_jetupTot.Pt()}, {"tauh_sv_eta", svfit_jetupTot.Eta()}, {"tauh_sv_phi", svfit_jetupTot.Phi()}, {"tauh_sv_e", svfit_jetupTot.E()}
            });
          }
          auto mdnnSM0_score_jetupTot  = mci.predict(EventNumber, 0);
          auto mdnnSM1_score_jetupTot  = mci.predict(EventNumber, 1);
          auto mdnnBSM0_score_jetupTot = mci.predict(EventNumber, 2);
          for (uint k=0; k<mdnnSM0_score_jetupTot.size(); k++)
            mdnnSM0_output_jetupTot.at(k) = mdnnSM0_score_jetupTot.at(k).second;
          for (uint k=0; k<mdnnSM1_score_jetupTot.size(); k++)
            mdnnSM1_output_jetupTot.at(k) = mdnnSM1_score_jetupTot.at(k).second;
          for (uint k=0; k<mdnnBSM0_score_jetupTot.size(); k++)
            mdnnBSM0_output_jetupTot.at(k) = mdnnBSM0_score_jetupTot.at(k).second;

          mci.clearInputs();
          for (uint j=0; j<mci.getNumberOfModels(); j++)
          {
            mci.setInputs(j,
            {
              {"is_mutau", mdnn_isMuTau}, {"is_etau", mdnn_isETau}, {"is_tautau", mdnn_isTauTau},
              {"bjet1_pt", bjet1_jetdownTot.Pt()}, {"bjet1_eta", bjet1_jetdownTot.Eta()}, {"bjet1_phi", bjet1_jetdownTot.Phi()}, {"bjet1_e", bjet1_jetdownTot.E()},
              {"bjet1_deepflavor_b", bjet1_bID_deepFlavor}, {"bjet1_deepflavor_cvsb", CvsB_b1}, {"bjet1_deepflavor_cvsl", CvsL_b1}, {"bjet1_hhbtag", HHbtag_b1},
              {"bjet2_pt", bjet2_jetdownTot.Pt()}, {"bjet2_eta", bjet2_jetdownTot.Eta()}, {"bjet2_phi", bjet2_jetdownTot.Phi()}, {"bjet2_e", bjet2_jetdownTot.E()},
              {"bjet2_deepflavor_b", bjet2_bID_deepFlavor}, {"bjet2_deepflavor_cvsb", CvsB_b2}, {"bjet2_deepflavor_cvsl", CvsL_b2}, {"bjet2_hhbtag", HHbtag_b2},
              {"vbfjet1_pt", vbfjet1_jetdownTot.Pt()}, {"vbfjet1_eta", vbfjet1_jetdownTot.Eta()}, {"vbfjet1_phi", vbfjet1_jetdownTot.Phi()}, {"vbfjet1_e", vbfjet1_jetdownTot.E()},
              {"vbfjet1_deepflavor_b", VBFjet1_btag_deepFlavor}, {"vbfjet1_deepflavor_cvsb", CvsB_vbf1}, {"vbfjet1_deepflavor_cvsl", CvsL_vbf1}, {"vbfjet1_hhbtag", HHbtag_vbf1},
              {"vbfjet2_pt", vbfjet2_jetdownTot.Pt()}, {"vbfjet2_eta", vbfjet2_jetdownTot.Eta()}, {"vbfjet2_phi", vbfjet2_jetdownTot.Phi()}, {"vbfjet2_e", vbfjet2_jetdownTot.E()},
              {"vbfjet2_deepflavor_b", VBFjet2_btag_deepFlavor}, {"vbfjet2_deepflavor_cvsb", CvsB_vbf2}, {"vbfjet2_deepflavor_cvsl", CvsL_vbf2}, {"vbfjet2_hhbtag", HHbtag_vbf2},
              {"lep1_pt", dau1_pt}, {"lep1_eta", dau1_eta}, {"lep1_phi", dau1_phi}, {"lep1_e", dau1_e},
              {"lep2_pt", dau2_pt}, {"lep2_eta", dau2_eta}, {"lep2_phi", dau2_phi}, {"lep2_e", dau2_e},
              {"met_pt", met_jetdownTot.Pt()}, {"met_phi", met_jetdownTot.Phi()},
              {"bh_pt", (bjet1_jetdownTot+bjet2_jetdownTot).Pt()}, {"bh_eta", (bjet1_jetdownTot+bjet2_jetdownTot).Eta()}, {"bh_phi", (bjet1_jetdownTot+bjet2_jetdownTot).Phi()}, {"bh_e", (bjet1_jetdownTot+bjet2_jetdownTot).E()},
              {"tauh_sv_pt", svfit_jetdownTot.Pt()}, {"tauh_sv_eta", svfit_jetdownTot.Eta()}, {"tauh_sv_phi", svfit_jetdownTot.Phi()}, {"tauh_sv_e", svfit_jetdownTot.E()}
            });
          }
          auto mdnnSM0_score_jetdownTot  = mci.predict(EventNumber, 0);
          auto mdnnSM1_score_jetdownTot  = mci.predict(EventNumber, 1);
          auto mdnnBSM0_score_jetdownTot = mci.predict(EventNumber, 2);
          for (uint k=0; k<mdnnSM0_score_jetdownTot.size(); k++)
            mdnnSM0_output_jetdownTot.at(k) = mdnnSM0_score_jetdownTot.at(k).second;
          for (uint k=0; k<mdnnSM1_score_jetdownTot.size(); k++)
            mdnnSM1_output_jetdownTot.at(k) = mdnnSM1_score_jetdownTot.at(k).second;
          for (uint k=0; k<mdnnBSM0_score_jetdownTot.size(); k++)
            mdnnBSM0_output_jetdownTot.at(k) = mdnnBSM0_score_jetdownTot.at(k).second;
        }

        if (doBDT)
        {
          BDTreader.SetInputValues(bjet2_jetupTot.Pt(), (bjet1_jetupTot+bjet2_jetupTot).Pt(), tau1.Pt(),
            tau2.Pt(), svfit_jetupTot.Pt(), BDT_channel,
            BDT_HT20_jetupTot, pzeta_jetupTot, pzeta_vis_jetupTot, BDT_ditau_deltaPhi_jetupTot,
            BDT_tauHsvfitMet_deltaPhi_jetupTot, mT_tauH_MET_jetupTot, mTtot_jetupTot, MT2_jetupTot,
            BDT_MX_jetupTot, BDT_bH_tauH_MET_InvMass_jetupTot, BDT_bH_tauH_SVFIT_InvMass_jetupTot,
            BDT_bH_tauH_InvMass_jetupTot, HHKin_mass_jetupTot, HHKin_chi2_jetupTot, BDT_MET_bH_cosTheta_jetupTot);
          std::vector<float> BDTouts_jetupTot = BDTreader.GetPredictions();
          BDToutSM_kl_1_jetupTot = BDTouts_jetupTot.at(0);

          BDTreader.SetInputValues(bjet2_jetdownTot.Pt(), (bjet1_jetdownTot+bjet2_jetdownTot).Pt(), tau1.Pt(),
            tau2.Pt(), svfit_jetdownTot.Pt(), BDT_channel,
            BDT_HT20_jetdownTot, pzeta_jetdownTot, pzeta_vis_jetdownTot, BDT_ditau_deltaPhi_jetdownTot,
            BDT_tauHsvfitMet_deltaPhi_jetdownTot, mT_tauH_MET_jetdownTot, mTtot_jetdownTot, MT2_jetdownTot,
            BDT_MX_jetdownTot, BDT_bH_tauH_MET_InvMass_jetdownTot, BDT_bH_tauH_SVFIT_InvMass_jetdownTot,
            BDT_bH_tauH_InvMass_jetdownTot, HHKin_mass_jetdownTot, HHKin_chi2_jetdownTot, BDT_MET_bH_cosTheta_jetdownTot);
          std::vector<float> BDTouts_jetdownTot = BDTreader.GetPredictions();
          BDToutSM_kl_1_jetdownTot = BDTouts_jetdownTot.at(0);
        }
      } // End isMC
    } // End doTotalJES

    // Timing info
    auto end_totalJES = high_resolution_clock::now();
    auto end_tot = high_resolution_clock::now();

    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // Fill new branches
    // Central values
    b_HHKin_mass_new   ->Fill();
    b_HHKin_chi2_new   ->Fill();
    b_MT2_new          ->Fill();
    b_DNNoutSM_kl_1_new->Fill();
    b_BDToutSM_kl_1_new->Fill();
    b_tauH_SVFIT_pt_new  ->Fill();
    b_tauH_SVFIT_eta_new ->Fill();
    b_tauH_SVFIT_phi_new ->Fill();
    b_tauH_SVFIT_mass_new->Fill();

    // MES variations
    b_tauH_SVFIT_mass_muup  ->Fill();
    b_DNNoutSM_kl_1_muup    ->Fill();
    b_BDToutSM_kl_1_muup    ->Fill();
    b_tauH_SVFIT_mass_mudown->Fill();
    b_DNNoutSM_kl_1_mudown  ->Fill();
    b_BDToutSM_kl_1_mudown  ->Fill();

    // EES variations
    b_tauH_SVFIT_mass_eleup_DM0  ->Fill();
    b_DNNoutSM_kl_1_eleup_DM0    ->Fill();
    b_BDToutSM_kl_1_eleup_DM0    ->Fill();
    b_tauH_SVFIT_mass_eledown_DM0->Fill();
    b_DNNoutSM_kl_1_eledown_DM0  ->Fill();
    b_BDToutSM_kl_1_eledown_DM0  ->Fill();
    b_tauH_SVFIT_mass_eleup_DM1  ->Fill();
    b_DNNoutSM_kl_1_eleup_DM1    ->Fill();
    b_BDToutSM_kl_1_eleup_DM1    ->Fill();
    b_tauH_SVFIT_mass_eledown_DM1->Fill();
    b_DNNoutSM_kl_1_eledown_DM1  ->Fill();
    b_BDToutSM_kl_1_eledown_DM1  ->Fill();

    // TES variations
    b_tauH_SVFIT_mass_tauup_DM0   ->Fill();
    b_DNNoutSM_kl_1_tauup_DM0     ->Fill();
    b_BDToutSM_kl_1_tauup_DM0     ->Fill();
    b_tauH_SVFIT_mass_taudown_DM0 ->Fill();
    b_DNNoutSM_kl_1_taudown_DM0   ->Fill();
    b_BDToutSM_kl_1_taudown_DM0   ->Fill();
    b_tauH_SVFIT_mass_tauup_DM1   ->Fill();
    b_DNNoutSM_kl_1_tauup_DM1     ->Fill();
    b_BDToutSM_kl_1_tauup_DM1     ->Fill();
    b_tauH_SVFIT_mass_taudown_DM1 ->Fill();
    b_DNNoutSM_kl_1_taudown_DM1   ->Fill();
    b_BDToutSM_kl_1_taudown_DM1   ->Fill();
    b_tauH_SVFIT_mass_tauup_DM10  ->Fill();
    b_DNNoutSM_kl_1_tauup_DM10    ->Fill();
    b_BDToutSM_kl_1_tauup_DM10    ->Fill();
    b_tauH_SVFIT_mass_taudown_DM10->Fill();
    b_DNNoutSM_kl_1_taudown_DM10  ->Fill();
    b_BDToutSM_kl_1_taudown_DM10  ->Fill();
    b_tauH_SVFIT_mass_tauup_DM11  ->Fill();
    b_DNNoutSM_kl_1_tauup_DM11    ->Fill();
    b_BDToutSM_kl_1_tauup_DM11    ->Fill();
    b_tauH_SVFIT_mass_taudown_DM11->Fill();
    b_DNNoutSM_kl_1_taudown_DM11  ->Fill();
    b_BDToutSM_kl_1_taudown_DM11  ->Fill();

    // JES variations
    for (int i=0; i<N_jecSources; i++)
    {
      b_tauH_SVFIT_mass_jetup  .at(i)->Fill();
      b_DNNoutSM_kl_1_jetup    .at(i)->Fill();
      b_BDToutSM_kl_1_jetup    .at(i)->Fill();
      b_tauH_SVFIT_mass_jetdown.at(i)->Fill();
      b_DNNoutSM_kl_1_jetdown  .at(i)->Fill();
      b_BDToutSM_kl_1_jetdown  .at(i)->Fill();
    }

    // JES Total variations
    b_tauH_SVFIT_mass_jetupTot  ->Fill();
    b_DNNoutSM_kl_1_jetupTot    ->Fill();
    b_BDToutSM_kl_1_jetupTot    ->Fill();
    b_tauH_SVFIT_mass_jetdownTot->Fill();
    b_DNNoutSM_kl_1_jetdownTot  ->Fill();
    b_BDToutSM_kl_1_jetdownTot  ->Fill();

    // Multiclass branches
    for (int i=0; i<mdnnSM0_size; i++)
    {
      b_mdnnSM0_new.at(i)->Fill();
      b_mdnnSM0_muup.at(i)->Fill();
      b_mdnnSM0_mudown.at(i)->Fill();
      b_mdnnSM0_eleup_DM0.at(i)->Fill();
      b_mdnnSM0_eledown_DM0.at(i)->Fill();
      b_mdnnSM0_eleup_DM1.at(i)->Fill();
      b_mdnnSM0_eledown_DM1.at(i)->Fill();
      b_mdnnSM0_tauup_DM0.at(i)->Fill();
      b_mdnnSM0_taudown_DM0.at(i)->Fill();
      b_mdnnSM0_tauup_DM1.at(i)->Fill();
      b_mdnnSM0_taudown_DM1.at(i)->Fill();
      b_mdnnSM0_tauup_DM10.at(i)->Fill();
      b_mdnnSM0_taudown_DM10.at(i)->Fill();
      b_mdnnSM0_tauup_DM11.at(i)->Fill();
      b_mdnnSM0_taudown_DM11.at(i)->Fill();
      b_mdnnSM0_jetupTot.at(i)->Fill();
      b_mdnnSM0_jetdownTot.at(i)->Fill();
      for (int jec=0; jec<N_jecSources; jec++)
      {
        b_mdnnSM0_jetup  [jec].at(i)->Fill();
        b_mdnnSM0_jetdown[jec].at(i)->Fill();
      }
    }
    for (int i=0; i<mdnnSM1_size; i++)
    {
      b_mdnnSM1_new.at(i)->Fill();
      b_mdnnSM1_muup.at(i)->Fill();
      b_mdnnSM1_mudown.at(i)->Fill();
      b_mdnnSM1_eleup_DM0.at(i)->Fill();
      b_mdnnSM1_eledown_DM0.at(i)->Fill();
      b_mdnnSM1_eleup_DM1.at(i)->Fill();
      b_mdnnSM1_eledown_DM1.at(i)->Fill();
      b_mdnnSM1_tauup_DM0.at(i)->Fill();
      b_mdnnSM1_taudown_DM0.at(i)->Fill();
      b_mdnnSM1_tauup_DM1.at(i)->Fill();
      b_mdnnSM1_taudown_DM1.at(i)->Fill();
      b_mdnnSM1_tauup_DM10.at(i)->Fill();
      b_mdnnSM1_taudown_DM10.at(i)->Fill();
      b_mdnnSM1_tauup_DM11.at(i)->Fill();
      b_mdnnSM1_taudown_DM11.at(i)->Fill();
      b_mdnnSM1_jetupTot.at(i)->Fill();
      b_mdnnSM1_jetdownTot.at(i)->Fill();
      for (int jec=0; jec<N_jecSources; jec++)
      {
        b_mdnnSM1_jetup  [jec].at(i)->Fill();
        b_mdnnSM1_jetdown[jec].at(i)->Fill();
      }
    }
    for (int i=0; i<mdnnBSM0_size; i++)
    {
      b_mdnnBSM0_new.at(i)->Fill();
      b_mdnnBSM0_muup.at(i)->Fill();
      b_mdnnBSM0_mudown.at(i)->Fill();
      b_mdnnBSM0_eleup_DM0.at(i)->Fill();
      b_mdnnBSM0_eledown_DM0.at(i)->Fill();
      b_mdnnBSM0_eleup_DM1.at(i)->Fill();
      b_mdnnBSM0_eledown_DM1.at(i)->Fill();
      b_mdnnBSM0_tauup_DM0.at(i)->Fill();
      b_mdnnBSM0_taudown_DM0.at(i)->Fill();
      b_mdnnBSM0_tauup_DM1.at(i)->Fill();
      b_mdnnBSM0_taudown_DM1.at(i)->Fill();
      b_mdnnBSM0_tauup_DM10.at(i)->Fill();
      b_mdnnBSM0_taudown_DM10.at(i)->Fill();
      b_mdnnBSM0_tauup_DM11.at(i)->Fill();
      b_mdnnBSM0_taudown_DM11.at(i)->Fill();
      b_mdnnBSM0_jetupTot.at(i)->Fill();
      b_mdnnBSM0_jetdownTot.at(i)->Fill();
      for (int jec=0; jec<N_jecSources; jec++)
      {
        b_mdnnBSM0_jetup  [jec].at(i)->Fill();
        b_mdnnBSM0_jetdown[jec].at(i)->Fill();
      }
    }

    // Timing branches
    time_prep     = (end_prep     - start_prep    ).count() * 1.e-9;
    time_nominal  = (end_nomin    - start_nomin   ).count() * 1.e-9;
    time_TES      = (end_TES      - start_TES     ).count() * 1.e-9;
    time_EES      = (end_EES      - start_EES     ).count() * 1.e-9;
    time_MES      = (end_MES      - start_MES     ).count() * 1.e-9;
    time_splitJES = (end_splitJES - start_splitJES).count() * 1.e-9;
    time_totalJES = (end_totalJES - start_totalJES).count() * 1.e-9;
    time_tot      = (end_tot      - start_tot     ).count() * 1.e-9;

    b_time_prep     -> Fill();
    b_time_nominal  -> Fill();
    b_time_TES      -> Fill();
    b_time_EES      -> Fill();
    b_time_MES      -> Fill();
    b_time_splitJES -> Fill();
    b_time_totalJES -> Fill();
    b_time_tot      -> Fill();
  }
  cout << "** ANALYSIS: ..Added new branches" << endl;
  cout << "** ANALYSIS: Final entries: " << outTree->GetEntries() << endl;

  outFile->cd();
  outTree->Write ("", TObject::kOverwrite) ;
  outFile->Write();
  outFile->Close();

  cout << "... SYST finished, exiting." << endl;
}
