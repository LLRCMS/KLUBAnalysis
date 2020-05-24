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

#include "lester_mt2_bisect.h"

#include "SVfitKLUBinterface.h"
#include "DNNKLUBinterface.h"
#include "BDTKLUBinterface.h"

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

const int nMaxEvts = 1;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// open input txt file and append all the files it contains to TChain
void appendFromFileList (TChain* chain, TString filename)
{
  //cout << "=== inizio parser ===" << endl;
  std::ifstream infile(filename.Data());
  std::string line;
  while (std::getline(infile, line))
    {
      line = line.substr(0, line.find("#", 0)); // remove comments introduced by #
      while (line.find(" ") != std::string::npos) line = line.erase(line.find(" "), 1); // remove white spaces
      while (line.find("\n") != std::string::npos) line = line.erase(line.find("\n"), 1); // remove new line characters
      while (line.find("\r") != std::string::npos) line = line.erase(line.find("\r"), 1); // remove carriage return characters
      if (!line.empty()) // skip empty lines
	chain->Add(line.c_str());
    }
  return;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// open the first file in the input list, retrieve the histogram "Counters" for the trigger names and return a copy of it
TH1F* getFirstFileHisto (TString filename, bool isForTriggers=true)
{
  std::ifstream infile(filename.Data());
  std::string line;
  while (std::getline(infile, line))
    {
      line = line.substr(0, line.find("#", 0)); // remove comments introduced by #
      while (line.find(" ") != std::string::npos) line = line.erase(line.find(" "), 1); // remove white spaces
      while (line.find("\n") != std::string::npos) line = line.erase(line.find("\n"), 1); // remove new line characters
      while (line.find("\r") != std::string::npos) line = line.erase(line.find("\r"), 1); // remove carriage return characters
      if (!line.empty()) // skip empty lines
	break;
    }
    
  TFile* fIn = TFile::Open (line.c_str());
  TH1F* dummy = (TH1F*) fIn->Get ("HTauTauTree/Counters");
  TString name = "Counters_perTrigger";
  if(!isForTriggers) {
    dummy = (TH1F*) fIn->Get ("HTauTauTree/TauIDs");
    name = "Counters_pertauID";
  }
  TH1F* histo = new TH1F (*dummy);
  histo-> SetDirectory(0);
  histo->SetName (name.Data());
  fIn->Close();
  return histo;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -

// Main
int main (int argc, char** argv)
{

  TString inputFileName = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/HHbtag/CMSSW_10_2_16/src/KLUBAnalysis/skimmed_output2016_SYST.root";  // FIXME: read from cfg file
  TString outputFileName = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/HHbtag/CMSSW_10_2_16/src/KLUBAnalysis/systTEST_DNN.root";            // FIXME: read from cfg file
  cout << "** INFO: inputFile  : " << inputFileName << endl;
  cout << "** INFO: outputFile : " << outputFileName << endl;

  bool doMT2    = true; // FIXME: read from cfg file
  bool doKinFit = true; // FIXME: read from cfg file
  bool doSVfit  = true; // FIXME: read from cfg file
  bool doDNN    = true; // FIXME: read from cfg file
  bool doBDT    = true; // FIXME: read from cfg file

  bool doMES      = true; // FIXME: read from cfg file
  bool doEES      = true; // FIXME: read from cfg file
  bool doTES      = true; // FIXME: read from cfg file
  bool doSplitJES = false; // FIXME: read from cfg file
  bool doTotalJES = true; // FIXME: read from cfg file

  // Input setup
  TFile *inputFile = new TFile (inputFileName, "read") ;

  // Systematics histogram
  TH1F* h_syst = (TH1F*)inputFile->Get("h_syst");
  const int N_jecSources = h_syst->GetBinContent(1); //jec sources
  const int N_tauhDM     = h_syst->GetBinContent(2); //tauh DMs
  const int N_tauhDM_EES = h_syst->GetBinContent(3); //tauh DMs with EES
  std::cout << "** INFO: N_jecSources: " << N_jecSources << std::endl;
  std::cout << "** INFO: N_tauhDM    : " << N_tauhDM << std::endl;
  std::cout << "** INFO: N_tauhDM_EES: " << N_tauhDM_EES << std::endl;

  // Input TTree
  TTree *inputChain = (TTree*)inputFile->Get("HTauTauTree");
  Long64_t nentries = inputChain->GetEntries();
  cout << "** INFO: Initial entries: " << nentries << endl;

  // Declare the only two branches needed for skimming the initial entries 
  int nleps, pairType, nbjetscand;
  inputChain->SetBranchAddress("nleps", &nleps);
  inputChain->SetBranchAddress("pairType", &pairType);
  inputChain->SetBranchAddress("nbjetscand", &nbjetscand);

  // De-activate all branches
  inputChain->SetBranchStatus("*", 0);

  // Define branches to be activated
  std::vector<std::string> toBeActivated {
  "EventNumber", "RunNumber","nleps","pairType","nbjetscand",          // General
  "isOS", "isBoosted",

  "MC_weight", "PUReweight", "L1pref_weight", "trigSF",                // Weights
  "IdAndIsoAndFakeSF_deep", "DYscale_MTT", "TTtopPtreweight",

  "isVBFtrigger", "isVBF",                                             // Trigger vbf selection

  "dau1_deepTauVsJet","dau2_deepTauVsJet",                             // Isolation
  "dau1_iso", "dau2_iso", "dau1_eleMVAiso"

  "dau1_decayMode", "dau2_decayMode",                                  // Tau decay mode

  "bjet1_bID_deepFlavor", "bjet2_bID_deepFlavor",                      // b-tagging          
  "bjet1_bID_deepCSV", "bjet2_bID_deepCSV",
  "bjet1_Cvs*","bjet2_Cvs*","VBFjet1_Cvs*","VBFjet2_Cvs*",

  "bjet1_HHbtag","bjet2_HHbtag","VBFjet1_HHbtag","VBFjet2_HHbtag",     // HHbtag

  "dau1_pt","dau1_eta","dau1_phi","dau1_e",                            // Tau central
  "dau2_pt","dau2_eta","dau2_phi","dau2_e",

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
  "DNNoutSM_kl_1","BDToutSM_kl_1"
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
  cout << "** INFO: Cloning with minimal selection..." << endl;
  for (Long64_t iEvent = 0 ; iEvent<nentries ; ++iEvent) 
  {
    if (iEvent % 500 == 0)  cout << "  - reading event " << iEvent << endl ;
    if (iEvent == nMaxEvts ) break;

    inputChain->GetEntry(iEvent);
    if ( nleps!=0 || nbjetscand<2 || pairType>2) continue;
    treenew->Fill() ;
  }
  cout << "** INFO: ...Cloned entries: " << treenew->GetEntries() << endl;

  // Save and close cloneFile and inputFile
  cloneFile->cd();
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
  std::vector<float> target_kls {1.}; // FIXME: read from cfg file

  // Declare DNNKLUBinterface
  std::string model_dir = "../cms_runII_dnn_models/models/nonres_gluglu/2020-05-18-2/ensemble";         // FIXME: read from cfg file
  std::string features_file = "../cms_runII_dnn_models/models/nonres_gluglu/2020-05-18-2/features.txt"; // FIXME: read from cfg file
  std::vector<std::string> requested;
  std::ifstream features_infile(features_file);
  std::string featureName;
  while ( features_infile >> featureName)
  {
    requested.push_back(featureName);
  }
  features_infile.close();
  DNNKLUBinterface DNNreader(model_dir, requested, target_kls);
  DNNreader.SetGlobalInputs(y17, nonres); // FIXME: read from cfg file

  // Declare BDTKLUBinterface
  std::string weights = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/CMSSW_10_2_16/src/KLUBAnalysis/weights/newBDT/BDTnewSM.xml"; // FIXME: read from cfg file
  BDTKLUBinterface BDTreader(weights, target_kls);

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
  float CvsL_b1, CvsL_b2, CvsL_vbf1, CvsL_vbf2;
  float CvsB_b1, CvsB_b2, CvsB_vbf1, CvsB_vbf2;
  float HHbtag_b1, HHbtag_b2, HHbtag_vbf1, HHbtag_vbf2;
  float BDT_HT20;
  std::vector<Float_t> *METx_eleup, *METy_eleup, *METx_eledown, *METy_eledown;
  std::vector<Float_t> *dau1_pt_eleup, *dau1_pt_eledown, *dau1_mass_eleup, *dau1_mass_eledown;
  std::vector<Float_t> *dau2_pt_eleup, *dau2_pt_eledown, *dau2_mass_eleup, *dau2_mass_eledown;
  std::vector<Float_t> *METx_tauup, *METy_tauup, *METx_taudown, *METy_taudown;
  std::vector<Float_t> *dau1_pt_tauup, *dau1_pt_taudown, *dau1_mass_tauup, *dau1_mass_taudown;
  std::vector<Float_t> *dau2_pt_tauup, *dau2_pt_taudown, *dau2_mass_tauup, *dau2_mass_taudown;

  std::vector<Float_t> *METx_jetup, *METy_jetup, *METx_jetdown, *METy_jetdown;
  std::vector<Float_t> *bjet1_pt_raw_jetup, *bjet1_pt_raw_jetdown, *bjet1_mass_raw_jetup, *bjet1_mass_raw_jetdown;
  std::vector<Float_t> *bjet2_pt_raw_jetup, *bjet2_pt_raw_jetdown, *bjet2_mass_raw_jetup, *bjet2_mass_raw_jetdown;
  std::vector<Float_t> *bjet1_JER_jetup, *bjet1_JER_jetdown;
  std::vector<Float_t> *bjet2_JER_jetup, *bjet2_JER_jetdown;
  std::vector<Float_t> *VBFjet1_pt_jetup, *VBFjet1_pt_jetdown, *VBFjet1_mass_jetup, *VBFjet1_mass_jetdown;
  std::vector<Float_t> *VBFjet2_pt_jetup, *VBFjet2_pt_jetdown, *VBFjet2_mass_jetup, *VBFjet2_mass_jetdown;
  std::vector<Float_t> *BDT_HT20_jetup, *BDT_HT20_jetdown;

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

  outTree->SetBranchAddress("MT2", &MT2);                         // FIXME: To be removed later
  outTree->SetBranchAddress("HHKin_mass", &HHKin_mass);           // FIXME: To be removed later
  outTree->SetBranchAddress("HHKin_chi2", &HHKin_chi2);           // FIXME: To be removed later
  outTree->SetBranchAddress("tauH_SVFIT_pt"  , &tauH_SVFIT_pt  ); // FIXME: To be removed later
  outTree->SetBranchAddress("tauH_SVFIT_eta" , &tauH_SVFIT_eta ); // FIXME: To be removed later
  outTree->SetBranchAddress("tauH_SVFIT_phi" , &tauH_SVFIT_phi ); // FIXME: To be removed later
  outTree->SetBranchAddress("tauH_SVFIT_mass", &tauH_SVFIT_mass); // FIXME: To be removed later
  outTree->SetBranchAddress("DNNoutSM_kl_1", &DNNoutSM_kl_1);     // FIXME: To be removed later
  outTree->SetBranchAddress("BDToutSM_kl_1", &BDToutSM_kl_1);     // FIXME: To be removed later

  // Declare new branches
  // Central values
  Float_t HHKin_mass_new, HHKin_chi2_new;
  Float_t MT2_new;
  Float_t tauH_SVFIT_pt_new, tauH_SVFIT_eta_new, tauH_SVFIT_phi_new, tauH_SVFIT_mass_new;
  Float_t DNNoutSM_kl_1_new, BDToutSM_kl_1_new;   // FIXME: read from cfg file
  TBranch* b_HHKin_mass_new      = outTree->Branch("HHKin_mass_new"  , &HHKin_mass_new);
  TBranch* b_HHKin_chi2_new      = outTree->Branch("HHKin_chi2_new"  , &HHKin_chi2_new);
  TBranch* b_MT2_new             = outTree->Branch("MT2_new"  , &MT2_new);
  TBranch* b_DNNoutSM_kl_1_new   = outTree->Branch("DNNoutSM_kl_1_new"  , &DNNoutSM_kl_1_new);
  TBranch* b_BDToutSM_kl_1_new   = outTree->Branch("BDToutSM_kl_1_new"  , &BDToutSM_kl_1_new);
  TBranch* b_tauH_SVFIT_pt_new   = outTree->Branch("tauH_SVFIT_pt_new"  , &tauH_SVFIT_pt_new);
  TBranch* b_tauH_SVFIT_eta_new  = outTree->Branch("tauH_SVFIT_eta_new" , &tauH_SVFIT_eta_new);
  TBranch* b_tauH_SVFIT_phi_new  = outTree->Branch("tauH_SVFIT_phi_new" , &tauH_SVFIT_phi_new);
  TBranch* b_tauH_SVFIT_mass_new = outTree->Branch("tauH_SVFIT_mass_new", &tauH_SVFIT_mass_new);

  // MES variations
  Float_t tauH_SVFIT_mass_muup, DNNoutSM_kl_1_muup, BDToutSM_kl_1_muup;
  Float_t tauH_SVFIT_mass_mudown, DNNoutSM_kl_1_mudown, BDToutSM_kl_1_mudown;
  TBranch* b_tauH_SVFIT_mass_muup   = outTree->Branch("tauH_SVFIT_mass_muup"  , &tauH_SVFIT_mass_muup);
  TBranch* b_DNNoutSM_kl_1_muup     = outTree->Branch("DNNoutSM_kl_1_muup"    , &DNNoutSM_kl_1_muup);
  TBranch* b_BDToutSM_kl_1_muup     = outTree->Branch("BDToutSM_kl_1_muup"    , &BDToutSM_kl_1_muup);
  TBranch* b_tauH_SVFIT_mass_mudown = outTree->Branch("tauH_SVFIT_mass_mudown", &tauH_SVFIT_mass_mudown);
  TBranch* b_DNNoutSM_kl_1_mudown   = outTree->Branch("DNNoutSM_kl_1_mudown"  , &DNNoutSM_kl_1_mudown);
  TBranch* b_BDToutSM_kl_1_mudown   = outTree->Branch("BDToutSM_kl_1_mudown"  , &BDToutSM_kl_1_mudown);

  // EES variations
  std::vector<Float_t> tauH_SVFIT_mass_eleup(2), DNNoutSM_kl_1_eleup(2), BDToutSM_kl_1_eleup(2);
  std::vector<Float_t> tauH_SVFIT_mass_eledown(2), DNNoutSM_kl_1_eledown(2), BDToutSM_kl_1_eledown(2);
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

  // TES variations
  std::vector<Float_t> tauH_SVFIT_mass_tauup(4), DNNoutSM_kl_1_tauup(4), BDToutSM_kl_1_tauup(4);
  std::vector<Float_t> tauH_SVFIT_mass_taudown(4), DNNoutSM_kl_1_taudown(4), BDToutSM_kl_1_taudown(4);
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

  // JES variations
  std::vector<Float_t> tauH_SVFIT_mass_jetup(N_jecSources), DNNoutSM_kl_1_jetup(N_jecSources), BDToutSM_kl_1_jetup(N_jecSources);
  std::vector<Float_t> tauH_SVFIT_mass_jetdown(N_jecSources), DNNoutSM_kl_1_jetdown(N_jecSources), BDToutSM_kl_1_jetdown(N_jecSources);
  std::vector<TBranch*> b_tauH_SVFIT_mass_jetup, b_tauH_SVFIT_mass_jetdown;
  std::vector<TBranch*> b_DNNoutSM_kl_1_jetup  , b_DNNoutSM_kl_1_jetdown  ;
  std::vector<TBranch*> b_BDToutSM_kl_1_jetup  , b_BDToutSM_kl_1_jetdown  ;

  boost::format tauHName_up  ("tauH_SVFIT_mass_jetup%i");
  boost::format DNNName_up   ("DNNoutSM_kl_1_jetup%i");
  boost::format BDTName_up   ("BDToutSM_kl_1_jetup%i");
  boost::format tauHName_down("tauH_SVFIT_mass_jetdown%i");
  boost::format DNNName_down ("DNNoutSM_kl_1_jetdown%i");
  boost::format BDTName_down ("BDToutSM_kl_1_jetdown%i");
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
  }

  // JES variations Total
  Float_t tauH_SVFIT_mass_jetupTot, DNNoutSM_kl_1_jetupTot, BDToutSM_kl_1_jetupTot;
  Float_t tauH_SVFIT_mass_jetdownTot, DNNoutSM_kl_1_jetdownTot, BDToutSM_kl_1_jetdownTot;
  TBranch* b_tauH_SVFIT_mass_jetupTot   = outTree->Branch("tauH_SVFIT_mass_jetupTot"  , &tauH_SVFIT_mass_jetupTot);
  TBranch* b_DNNoutSM_kl_1_jetupTot     = outTree->Branch("DNNoutSM_kl_1_jetupTot"    , &DNNoutSM_kl_1_jetupTot);
  TBranch* b_BDToutSM_kl_1_jetupTot     = outTree->Branch("BDToutSM_kl_1_jetupTot"    , &BDToutSM_kl_1_jetupTot);
  TBranch* b_tauH_SVFIT_mass_jetdownTot = outTree->Branch("tauH_SVFIT_mass_jetdownTot", &tauH_SVFIT_mass_jetdownTot);
  TBranch* b_DNNoutSM_kl_1_jetdownTot   = outTree->Branch("DNNoutSM_kl_1_jetdownTot"  , &DNNoutSM_kl_1_jetdownTot);
  TBranch* b_BDToutSM_kl_1_jetdownTot   = outTree->Branch("BDToutSM_kl_1_jetdownTot"  , &BDToutSM_kl_1_jetdownTot);

  // Loop on selected entries
  cout << "** INFO: Adding new branches..." << endl;
  for(int i=0;i<outTree->GetEntries();i++)
  {
    if (i % 500 == 0) cout << "- reading event " << i << endl ;
    if (i == nMaxEvts ) break;

    // Get Entry
    outTree->GetEntry(i);

    if (i % 500 == 0) std::cout << "---------------- Event: " << i << " -------------------" << std::endl;

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


    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // KinFit computation - Central value
    if (doKinFit)
    {
      // nominal kinfit
      HHKinFit2::HHKinFitMasterHeavyHiggs kinFits = HHKinFit2::HHKinFitMasterHeavyHiggs(bjet1, bjet2, tau1, tau2, ptmiss, stableMetCov, bjet1_JER, bjet2_JER) ;
      kinFits.addHypo(hypo_mh1,hypo_mh2);
      bool wrongHHK =false;
      try {kinFits.fit();}
      catch(HHKinFit2::HHInvMConstraintException   e) {wrongHHK=true;}
      catch(HHKinFit2::HHEnergyConstraintException e) {wrongHHK=true;}
      catch(HHKinFit2::HHEnergyRangeException      e) {wrongHHK=true;}
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

    // ---- ---- ---- ---- ---- ---- ----
    // ---- ---- Do all MES now ---- ----
    // ---- ---- ---- ---- ---- ---- ----
    if (doMES)
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
        catch(HHKinFit2::HHInvMConstraintException   e) {wrongHHK_muup=true;}
        catch(HHKinFit2::HHEnergyConstraintException e) {wrongHHK_muup=true;}
        catch(HHKinFit2::HHEnergyRangeException      e) {wrongHHK_muup=true;}
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
        catch(HHKinFit2::HHInvMConstraintException   e) {wrongHHK_mudown=true;}
        catch(HHKinFit2::HHEnergyConstraintException e) {wrongHHK_mudown=true;}
        catch(HHKinFit2::HHEnergyRangeException      e) {wrongHHK_mudown=true;}
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
    }  // End doMES

    // ---- ---- ---- ---- ---- ---- ----
    // ---- ---- Do all EES now ---- ----
    // ---- ---- ---- ---- ---- ---- ----
    if (doEES)
    {
      for (int i=0; i<N_tauhDM_EES; i++)
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
          catch(HHKinFit2::HHInvMConstraintException   e) {wrongHHK_eleup=true;}
          catch(HHKinFit2::HHEnergyConstraintException e) {wrongHHK_eleup=true;}
          catch(HHKinFit2::HHEnergyRangeException      e) {wrongHHK_eleup=true;}
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
          catch(HHKinFit2::HHInvMConstraintException   e) {wrongHHK_eledown=true;}
          catch(HHKinFit2::HHEnergyConstraintException e) {wrongHHK_eledown=true;}
          catch(HHKinFit2::HHEnergyRangeException      e) {wrongHHK_eledown=true;}
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
      }
    }  // End doEES

    // ---- ---- ---- ---- ---- ---- ----
    // ---- ---- Do all TES now ---- ----
    // ---- ---- ---- ---- ---- ---- ----
    if (doTES)
    {
      for (int i=0; i<N_tauhDM; i++)
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
          catch(HHKinFit2::HHInvMConstraintException   e) {wrongHHK_tauup=true;}
          catch(HHKinFit2::HHEnergyConstraintException e) {wrongHHK_tauup=true;}
          catch(HHKinFit2::HHEnergyRangeException      e) {wrongHHK_tauup=true;}
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
          catch(HHKinFit2::HHInvMConstraintException   e) {wrongHHK_taudown=true;}
          catch(HHKinFit2::HHEnergyConstraintException e) {wrongHHK_taudown=true;}
          catch(HHKinFit2::HHEnergyRangeException      e) {wrongHHK_taudown=true;}
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
      }
    }  // End doTES

    // ---- ---- ---- ---- ---- ---- ---- ----
    // ---- ---- Do splitted JES now ---- ----
    // ---- ---- ---- ---- ---- ---- ---- ----
    if (doSplitJES)
    {
      for (int i=0; i<N_jecSources; i++)
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
          catch(HHKinFit2::HHInvMConstraintException   e) {wrongHHK_jetup=true;}
          catch(HHKinFit2::HHEnergyConstraintException e) {wrongHHK_jetup=true;}
          catch(HHKinFit2::HHEnergyRangeException      e) {wrongHHK_jetup=true;}
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
          catch(HHKinFit2::HHInvMConstraintException   e) {wrongHHK_jetdown=true;}
          catch(HHKinFit2::HHEnergyConstraintException e) {wrongHHK_jetdown=true;}
          catch(HHKinFit2::HHEnergyRangeException      e) {wrongHHK_jetdown=true;}
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
      }
    }  // End doSplitJES

    // ---- ---- ---- ---- ---- ---- ----
    // ---- ----  Do total JES  ---- ----
    // ---- ---- ---- ---- ---- ---- ----
    if (doTotalJES)
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
        catch(HHKinFit2::HHInvMConstraintException   e) {wrongHHK_jetupTot=true;}
        catch(HHKinFit2::HHEnergyConstraintException e) {wrongHHK_jetupTot=true;}
        catch(HHKinFit2::HHEnergyRangeException      e) {wrongHHK_jetupTot=true;}
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
        catch(HHKinFit2::HHInvMConstraintException   e) {wrongHHK_jetdownTot=true;}
        catch(HHKinFit2::HHEnergyConstraintException e) {wrongHHK_jetdownTot=true;}
        catch(HHKinFit2::HHEnergyRangeException      e) {wrongHHK_jetdownTot=true;}
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
    }

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
  }
  cout << "** INFO: ..Added new branches" << endl;
  cout << "** INFO: Final entries: " << outTree->GetEntries() << endl;

  outFile->cd();
  outTree->Write ("", TObject::kOverwrite) ;
  outFile->Write();
  outFile->Close();

  cout << "... SYST finished, exiting." << endl;
}
