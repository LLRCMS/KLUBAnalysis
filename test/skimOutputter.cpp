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

const int nMaxEvts = 100;

// systematics
const int N_tauhDM     =  4; //tauh DMs
const int N_tauhDM_EES =  2; //tauh DMs with EES
const int N_jecSources = 11; //jec sources

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
float deltaPhi (float phi1, float phi2)
{
  float deltaphi = fabs (phi1 - phi2) ;
  if (deltaphi > 6.283185308) deltaphi -= 6.283185308 ;
  if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi ;
  return deltaphi ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
float getZ (float eta, float eta1, float eta2)
{
  float Z  =  (eta - 0.5*(eta1 + eta2)) / fabs (eta1 - eta2) ;
  return Z;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
float ComputeMT (TLorentzVector visP4, TLorentzVector METP4)
{
    double dphi = deltaPhi(visP4.Phi(), METP4.Phi());
    return sqrt(2.*visP4.Pt()*METP4.Pt()*(1.-cos(dphi)));
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -

// Main
int main (int argc, char** argv)
{

  //TString inputFileName = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/DNN2/CMSSW_10_2_16/src/KLUBAnalysis/skimmed_output1_ALL.root"; // FIXME: read from cfg file
  TString inputFileName = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/DNN2/CMSSW_10_2_16/src/KLUBAnalysis/skimmed_output1_SYST.root";  // FIXME: read from cfg file
  TString outputFileName = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/DNN2/CMSSW_10_2_16/src/KLUBAnalysis/systTEST_MES.root";         // FIXME: read from cfg file
  cout << "** INFO: inputFile  : " << inputFileName << endl;
  cout << "** INFO: outputFile : " << outputFileName << endl;

  bool doMT2    = true; // FIXME: read from cfg file
  bool doKinFit = true; // FIXME: read from cfg file
  bool doSVfit  = true; // FIXME: read from cfg file
  bool doDNN    = true; // FIXME: read from cfg file
  bool doBDT    = true; // FIXME: read from cfg file

  bool doEES = true; // FIXME: read from cfg file
  bool doTES = true; // FIXME: read from cfg file

  // Input setup
  TFile *inputFile = new TFile (inputFileName, "read") ;
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

  "dau1_iso", "dau2_iso", "dau1_deepTauVsJet", "dau2_deepTauVsJet",    // Isolation

  "dau1_decayMode", "dau2_decayMode",                                  // Tau decay mode

  "bjet1_bID_deepFlavor", "bjet2_bID_deepFlavor",                      // b-tagging          
  "bjet1_bID_deepCSV", "bjet2_bID_deepCSV",

  "dau1_pt","dau1_eta","dau1_phi","dau1_e",                            // Tau central
  "dau2_pt","dau2_eta","dau2_phi","dau2_e",

  "dau1_pt_muup","dau1_pt_mudown",                                     // Tau MES
  "dau1_mass_muup", "dau1_mass_mudown",
  "dau2_pt_muup","dau2_pt_mudown",
  "dau2_mass_muup", "dau2_mass_mudown",

  "dau1_pt_eleup_DM0","dau1_pt_eleup_DM1",                             // Tau EES
  "dau1_pt_eledown_DM0","dau1_pt_eledown_DM1",
  "dau1_pt_eleup","dau1_mass_eleup",
  "dau1_pt_eledown","dau1_mass_eledown",
  "dau2_pt_eleup_DM0","dau2_pt_eleup_DM1",
  "dau2_pt_eledown_DM0","dau2_pt_eledown_DM1",
  "dau2_pt_eleup","dau2_mass_eleup",
  "dau2_pt_eledown","dau2_mass_eledown",

  "dau1_pt_tauup_DM0","dau1_pt_tauup_DM1",                             // Tau TES
  "dau1_pt_tauup_DM10","dau1_pt_tauup_DM11",
  "dau1_pt_taudown_DM0","dau1_pt_taudown_DM1",
  "dau1_pt_taudown_DM10","dau1_pt_taudown_DM11",
  "dau1_pt_tauup","dau1_pt_taudown",
  "dau1_mass_tauup","dau1_mass_taudown",
  "dau2_pt_tauup_DM0","dau2_pt_tauup_DM1",
  "dau2_pt_tauup_DM10","dau2_pt_tauup_DM11",
  "dau2_pt_taudown_DM0","dau2_pt_taudown_DM1",
  "dau2_pt_taudown_DM10","dau2_pt_taudown_DM11",
  "dau2_pt_tauup","dau2_pt_taudown",
  "dau2_mass_tauup","dau2_mass_taudown",

  "bjet1_pt","bjet1_eta","bjet1_phi","bjet1_e","bjet1_JER",            // b-jets
  "bjet2_pt","bjet2_eta","bjet2_phi","bjet2_e","bjet2_JER",

  "bjet1_pt_raw_jetup","bjet1_pt_raw_jetdown",                         // b-jets JES
  "bjet1_mass_raw_jetup","bjet1_mass_raw_jetdown",
  "bjet1_JER_jetup","bjet1_JER_jetdown",
  "bjet1_pt_raw_jetup1","bjet1_pt_raw_jetup2","bjet1_pt_raw_jetup3",
  "bjet1_pt_raw_jetup4","bjet1_pt_raw_jetup5","bjet1_pt_raw_jetup6",
  "bjet1_pt_raw_jetup7","bjet1_pt_raw_jetup8","bjet1_pt_raw_jetup9",
  "bjet1_pt_raw_jetup10","bjet1_pt_raw_jetup11",
  "bjet1_pt_raw_jetdown1","bjet1_pt_raw_jetdown2","bjet1_pt_raw_jetdown3",
  "bjet1_pt_raw_jetdown4","bjet1_pt_raw_jetdown5","bjet1_pt_raw_jetdown6",
  "bjet1_pt_raw_jetdown7","bjet1_pt_raw_jetdown8","bjet1_pt_raw_jetdown9",
  "bjet1_pt_raw_jetdown10","bjet1_pt_raw_jetdown11",
  "bjet2_pt_raw_jetup","bjet2_pt_raw_jetdown",
  "bjet2_mass_raw_jetup","bjet2_mass_raw_jetdown",
  "bjet2_JER_jetup","bjet2_JER_jetdown",
  "bjet2_pt_raw_jetup1","bjet2_pt_raw_jetup2","bjet2_pt_raw_jetup3",
  "bjet2_pt_raw_jetup4","bjet2_pt_raw_jetup5","bjet2_pt_raw_jetup6",
  "bjet2_pt_raw_jetup7","bjet2_pt_raw_jetup8","bjet2_pt_raw_jetup9",
  "bjet2_pt_raw_jetup10","bjet2_pt_raw_jetup11",
  "bjet2_pt_raw_jetdown1","bjet2_pt_raw_jetdown2","bjet2_pt_raw_jetdown3",
  "bjet2_pt_raw_jetdown4","bjet2_pt_raw_jetdown5","bjet2_pt_raw_jetdown6",
  "bjet2_pt_raw_jetdown7","bjet2_pt_raw_jetdown8","bjet2_pt_raw_jetdown9",
  "bjet2_pt_raw_jetdown10","bjet2_pt_raw_jetdown11",

  "bH_mass_raw_jetup1","bH_mass_raw_jetup2","bH_mass_raw_jetup3",      // bH masses JES
  "bH_mass_raw_jetup4","bH_mass_raw_jetup5","bH_mass_raw_jetup6",
  "bH_mass_raw_jetup7","bH_mass_raw_jetup8","bH_mass_raw_jetup9",
  "bH_mass_raw_jetup10","bH_mass_raw_jetup11",
  "bH_mass_raw_jetdown1","bH_mass_raw_jetdown2","bH_mass_raw_jetdown3",
  "bH_mass_raw_jetdown4","bH_mass_raw_jetdown5","bH_mass_raw_jetdown6",
  "bH_mass_raw_jetdown7","bH_mass_raw_jetdown8","bH_mass_raw_jetdown9",
  "bH_mass_raw_jetdown10","bH_mass_raw_jetdown11",

  "VBFjet1_pt","VBFjet1_eta","VBFjet1_phi","VBFjet1_e",                // VBF-jets
  "VBFjet2_pt","VBFjet2_eta","VBFjet2_phi","VBFjet2_e",

  "VBFjet1_pt_jetup","VBFjet1_pt_jetdown",                             // VBF-jets JES
  "VBFjet1_mass_jetup","VBFjet1_mass_jetdown",
  "VBFjet1_pt_jetup1","VBFjet1_pt_jetup2","VBFjet1_pt_jetup3",
  "VBFjet1_pt_jetup4","VBFjet1_pt_jetup5","VBFjet1_pt_jetup6",
  "VBFjet1_pt_jetup7","VBFjet1_pt_jetup8","VBFjet1_pt_jetup9",
  "VBFjet1_pt_jetup10","VBFjet1_pt_jetup11",
  "VBFjet1_pt_jetdown1","VBFjet1_pt_jetdown2","VBFjet1_pt_jetdown3",
  "VBFjet1_pt_jetdown4","VBFjet1_pt_jetdown5","VBFjet1_pt_jetdown6",
  "VBFjet1_pt_jetdown7","VBFjet1_pt_jetdown8","VBFjet1_pt_jetdown9",
  "VBFjet1_pt_jetdown10","VBFjet1_pt_jetdown11",
  "VBFjet2_pt_jetup","VBFjet2_pt_jetdown",
  "VBFjet2_mass_jetup","VBFjet2_mass_jetdown",
  "VBFjet2_pt_jetup1","VBFjet2_pt_jetup2","VBFjet2_pt_jetup3",
  "VBFjet2_pt_jetup4","VBFjet2_pt_jetup5","VBFjet2_pt_jetup6",
  "VBFjet2_pt_jetup7","VBFjet2_pt_jetup8","VBFjet2_pt_jetup9",
  "VBFjet2_pt_jetup10","VBFjet2_pt_jetup11",
  "VBFjet2_pt_jetdown1","VBFjet2_pt_jetdown2","VBFjet2_pt_jetdown3",
  "VBFjet2_pt_jetdown4","VBFjet2_pt_jetdown5","VBFjet2_pt_jetdown6",
  "VBFjet2_pt_jetdown7","VBFjet2_pt_jetdown8","VBFjet2_pt_jetdown9",
  "VBFjet2_pt_jetdown10","VBFjet2_pt_jetdown11",

  "VBFjj_mass_jetup1","VBFjj_mass_jetup2","VBFjj_mass_jetup3",         // VBFjj mass JES
  "VBFjj_mass_jetup4","VBFjj_mass_jetup5","VBFjj_mass_jetup6",
  "VBFjj_mass_jetup7","VBFjj_mass_jetup8","VBFjj_mass_jetup9",
  "VBFjj_mass_jetup10","VBFjj_mass_jetup11",
  "VBFjj_mass_jetdown1","VBFjj_mass_jetdown2","VBFjj_mass_jetdown3",
  "VBFjj_mass_jetdown4","VBFjj_mass_jetdown5","VBFjj_mass_jetdown6",
  "VBFjj_mass_jetdown7","VBFjj_mass_jetdown8","VBFjj_mass_jetdown9",
  "VBFjj_mass_jetdown10","VBFjj_mass_jetdown11",

  "met_phi","met_et","METx","METy",                                    // MET
  "met_cov00","met_cov01","met_cov10","met_cov11",

  "METx_muup" ,"METy_muup" ,"METx_mudown" ,"METy_mudown",              // MET MES
  "METx_eleup","METy_eleup","METx_eledown","METy_eledown",             // MET EES
  "METx_tauup","METy_tauup","METx_taudown","METy_taudown",             // MET TES
  "METx_jetup","METy_jetup","METx_jetdown","METy_jetdown",             // MET TES

  "VBFjj_mass", "VBFjj_deltaEta",                                      // VBF selection

  "BDT_HT20",                                                          // BDT_HT20
  "BDT_HT20_jetup","BDT_HT20_jetdown",                                 // BDT_HT20 JES

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
  std::string model_dir = "../cms_runII_dnn_models/models/nonres_gluglu/2020-03-11-0/ensemble";         // FIXME: read from cfg file
  std::string features_file = "../cms_runII_dnn_models/models/nonres_gluglu/2020-03-11-0/features.txt"; // FIXME: read from cfg file
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
  float bjet1_bID_deepFlavor, bjet2_bID_deepFlavor, bjet1_bID_deepCSV, bjet2_bID_deepCSV;
  float BDT_HT20;
  std::vector<Float_t> *METx_eleup, *METy_eleup, *METx_eledown, *METy_eledown;
  std::vector<Float_t> *dau1_pt_eleup, *dau1_pt_eledown, *dau1_mass_eleup, *dau1_mass_eledown;
  std::vector<Float_t> *dau2_pt_eleup, *dau2_pt_eledown, *dau2_mass_eleup, *dau2_mass_eledown;
  std::vector<Float_t> *METx_tauup, *METy_tauup, *METx_taudown, *METy_taudown;
  std::vector<Float_t> *dau1_pt_tauup, *dau1_pt_taudown, *dau1_mass_tauup, *dau1_mass_taudown;
  std::vector<Float_t> *dau2_pt_tauup, *dau2_pt_taudown, *dau2_mass_tauup, *dau2_mass_taudown;

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

  outTree->SetBranchAddress("VBFjet1_pt" , &vbfjet1_pt);
  outTree->SetBranchAddress("VBFjet1_eta", &vbfjet1_eta);
  outTree->SetBranchAddress("VBFjet1_phi", &vbfjet1_phi);
  outTree->SetBranchAddress("VBFjet1_e"  , &vbfjet1_e);
  outTree->SetBranchAddress("VBFjet2_pt" , &vbfjet2_pt);
  outTree->SetBranchAddress("VBFjet2_eta", &vbfjet2_eta);
  outTree->SetBranchAddress("VBFjet2_phi", &vbfjet2_phi);
  outTree->SetBranchAddress("VBFjet2_e"  , &vbfjet2_e);

  outTree->SetBranchAddress("bjet1_bID_deepFlavor" , &bjet1_bID_deepFlavor);
  outTree->SetBranchAddress("bjet2_bID_deepFlavor" , &bjet2_bID_deepFlavor);
  outTree->SetBranchAddress("bjet1_bID_deepCSV"    , &bjet1_bID_deepCSV);
  outTree->SetBranchAddress("bjet2_bID_deepCSV"    , &bjet2_bID_deepCSV);

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

  outTree->SetBranchAddress("BDT_HT20", &BDT_HT20);

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
  Float_t HHKin_mass_muup, HHKin_chi2_muup, HHKin_mass_mudown, HHKin_chi2_mudown;
  Float_t MT2_muup, MT2_mudown;
  Float_t tauH_SVFIT_pt_muup, tauH_SVFIT_eta_muup, tauH_SVFIT_phi_muup, tauH_SVFIT_mass_muup;
  Float_t tauH_SVFIT_pt_mudown, tauH_SVFIT_eta_mudown, tauH_SVFIT_phi_mudown, tauH_SVFIT_mass_mudown;
  Float_t DNNoutSM_kl_1_muup, BDToutSM_kl_1_muup, DNNoutSM_kl_1_mudown, BDToutSM_kl_1_mudown;
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

  // Loop on selected entries
  cout << "** INFO: Adding new branches..." << endl;
  for(int i=0;i<outTree->GetEntries();i++)
  {
    if (i % 500 == 0) cout << "- reading event " << i << endl ;
    if (i == nMaxEvts ) break;

    // Get Entry
    outTree->GetEntry(i);

    if (i % 500 == 0) std::cout << "---------------- Event: " << i << " : " << EventNumber << " -------------------" << std::endl;

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
    float mt1                           = ComputeMT(tau1, met);
    float mt2                           = ComputeMT(tau2, met);
    float BDT_ditau_deltaPhi            = ROOT::Math::VectorUtil::DeltaPhi(tau1, tau2);
    float BDT_tauHsvfitMet_deltaPhi     = ROOT::Math::VectorUtil::DeltaPhi(svfit, met);
    float mT_tauH_MET                   = Calculate_MT( (tau1+tau2)+met, met);
    float BDT_MX                        = Calculate_MX(tau1, tau2, bjet1, bjet2, met);
    float BDT_bH_tauH_MET_InvMass       = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1+tau2)+met);
    float BDT_bH_tauH_SVFIT_InvMass     = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), svfit);
    float BDT_bH_tauH_InvMass           = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1+tau2));
    float BDT_MET_bH_cosTheta           = Calculate_cosTheta_2bodies(getLVfromTLV(met), getLVfromTLV(bjet1+bjet2));
    std::pair<double, double> topMasses = Calculate_topPairMasses(getLVfromTLV(tau1), getLVfromTLV(tau2), getLVfromTLV(bjet1), getLVfromTLV(bjet2), getLVfromTLV(met));
    bool KinFitConv                     = HHKin_chi2 > 0;
    bool SVfitConv                      = tauH_SVFIT_mass > 0;


    // --- --- --- MES variations --- --- ---
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
    DNNreader.SetEventInputs(channel, isBoosted, bjet1_bID_deepFlavor, bjet2_bID_deepFlavor,
        bjet1_bID_deepCSV, bjet2_bID_deepCSV, nvbf, EventNumber);


    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // KinFit computation - Central & MES
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

    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // MT2 computation - Central & MES
    if (doMT2)
    {
      MT2_new = asymm_mt2_lester_bisect::get_mT2( bjet1.M(), bjet1.Px(), bjet1.Py(),
                                                  bjet2.M(), bjet2.Px(), bjet2.Py(),
                                                  (tau1.Px() + tau2.Px() + met.Px()),
                                                  (tau1.Py() + tau2.Py() + met.Py()),
                                                  tau1.M(), tau2.M(), desiredPrecisionOnMt2);

      // MES variations
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

    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // SVfit computation - Central & MES
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

      // MES variations
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

    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // MES DNN/BDT quantities
    TLorentzVector svfit_muup;
    svfit_muup.SetPtEtaPhiM(tauH_SVFIT_pt_muup, tauH_SVFIT_eta_muup, tauH_SVFIT_phi_muup, tauH_SVFIT_mass_muup);
    float mTtot_muup                         = Calculate_TotalMT(tau1_muup, tau2_muup, met_muup);
    float pzeta_vis_muup                     = Calculate_visiblePzeta(tau1_muup, tau2_muup);
    float pzeta_muup                         = Calculate_Pzeta(tau1_muup, tau2_muup, met_muup);
    float mt1_muup                           = ComputeMT(tau1_muup, met_muup);
    float mt2_muup                           = ComputeMT(tau2_muup, met_muup);
    float BDT_ditau_deltaPhi_muup            = ROOT::Math::VectorUtil::DeltaPhi(tau1_muup, tau2_muup);
    float BDT_tauHsvfitMet_deltaPhi_muup     = ROOT::Math::VectorUtil::DeltaPhi(svfit_muup, met_muup);
    float mT_tauH_MET_muup                   = Calculate_MT( (tau1_muup+tau2_muup)+met_muup, met_muup);
    float BDT_MX_muup                        = Calculate_MX(tau1_muup, tau2_muup, bjet1, bjet2, met_muup);
    float BDT_bH_tauH_MET_InvMass_muup       = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_muup+tau2_muup)+met_muup);
    float BDT_bH_tauH_SVFIT_InvMass_muup     = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), svfit_muup);
    float BDT_bH_tauH_InvMass_muup           = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_muup+tau2_muup));
    float BDT_MET_bH_cosTheta_muup           = Calculate_cosTheta_2bodies(getLVfromTLV(met_muup), getLVfromTLV(bjet1+bjet2));
    std::pair<double, double> topMasses_muup = Calculate_topPairMasses(getLVfromTLV(tau1_muup), getLVfromTLV(tau2_muup), getLVfromTLV(bjet1), getLVfromTLV(bjet2), getLVfromTLV(met_muup));
    bool KinFitConv_muup                     = HHKin_chi2_muup > 0;
    bool SVfitConv_muup                      = tauH_SVFIT_mass_muup > 0;

    TLorentzVector svfit_mudown;
    svfit_mudown.SetPtEtaPhiM(tauH_SVFIT_pt_mudown, tauH_SVFIT_eta_mudown, tauH_SVFIT_phi_mudown, tauH_SVFIT_mass_mudown);
    float mTtot_mudown                         = Calculate_TotalMT(tau1_mudown, tau2_mudown, met_mudown);
    float pzeta_vis_mudown                     = Calculate_visiblePzeta(tau1_mudown, tau2_mudown);
    float pzeta_mudown                         = Calculate_Pzeta(tau1_mudown, tau2_mudown, met_mudown);
    float mt1_mudown                           = ComputeMT(tau1_mudown, met_mudown);
    float mt2_mudown                           = ComputeMT(tau2_mudown, met_mudown);
    float BDT_ditau_deltaPhi_mudown            = ROOT::Math::VectorUtil::DeltaPhi(tau1_mudown, tau2_mudown);
    float BDT_tauHsvfitMet_deltaPhi_mudown     = ROOT::Math::VectorUtil::DeltaPhi(svfit_mudown, met_mudown);
    float mT_tauH_MET_mudown                   = Calculate_MT( (tau1_mudown+tau2_mudown)+met_mudown, met_mudown);
    float BDT_MX_mudown                        = Calculate_MX(tau1_mudown, tau2_mudown, bjet1, bjet2, met_mudown);
    float BDT_bH_tauH_MET_InvMass_mudown       = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_mudown+tau2_mudown)+met_mudown);
    float BDT_bH_tauH_SVFIT_InvMass_mudown     = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), svfit_mudown);
    float BDT_bH_tauH_InvMass_mudown           = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_mudown+tau2_mudown));
    float BDT_MET_bH_cosTheta_mudown           = Calculate_cosTheta_2bodies(getLVfromTLV(met_mudown), getLVfromTLV(bjet1+bjet2));
    std::pair<double, double> topMasses_mudown = Calculate_topPairMasses(getLVfromTLV(tau1_mudown), getLVfromTLV(tau2_mudown), getLVfromTLV(bjet1), getLVfromTLV(bjet2), getLVfromTLV(met_mudown));
    bool KinFitConv_mudown                     = HHKin_chi2_mudown > 0;
    bool SVfitConv_mudown                      = tauH_SVFIT_mass_mudown > 0;


    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // DNN computation - Central & MES
    if (doDNN)
    {
      // Set quantities that change for each event (shifted for TES, JES...)
      // Central value
      DNNreader.SetShiftedInputs(bjet1, bjet2, tau1, tau2, vbfjet1, vbfjet1, met, svfit,
          HHKin_mass, HHKin_chi2, KinFitConv, SVfitConv, MT2,
          mTtot, pzeta_vis, pzeta, topMasses.first, topMasses.second, mt1, mt2);
      std::vector<float> outs = DNNreader.GetPredictions();
      //std::cout << "----- ...gotten predictions: " << outs.at(0) << std::endl;
      DNNoutSM_kl_1_new = outs.at(0);

      // MES variations
      DNNreader.SetShiftedInputs(bjet1, bjet2, tau1_muup, tau2_muup, vbfjet1, vbfjet1, met_muup, svfit_muup,
          HHKin_mass_muup, HHKin_chi2_muup, KinFitConv_muup, SVfitConv_muup, MT2_muup,
          mTtot_muup, pzeta_vis_muup, pzeta_muup, topMasses_muup.first, topMasses_muup.second, mt1_muup, mt2_muup);
      std::vector<float> outs_muup = DNNreader.GetPredictions();
      DNNoutSM_kl_1_muup = outs_muup.at(0);

      DNNreader.SetShiftedInputs(bjet1, bjet2, tau1_mudown, tau2_mudown, vbfjet1, vbfjet1, met_mudown, svfit_mudown,
          HHKin_mass_mudown, HHKin_chi2_mudown, KinFitConv_mudown, SVfitConv_mudown, MT2_mudown,
          mTtot_mudown, pzeta_vis_mudown, pzeta_mudown, topMasses_mudown.first, topMasses_mudown.second, mt1_mudown, mt2_mudown);
      std::vector<float> outs_mudown = DNNreader.GetPredictions();
      DNNoutSM_kl_1_mudown = outs_mudown.at(0);
    }

    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // BDT computation - Central & MES
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

      // MES variations
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
        BDT_tauHsvfitMet_deltaPhi_mudown, mT_tauH_MET_mudown, mTtot, MT2_mudown,
        BDT_MX_mudown, BDT_bH_tauH_MET_InvMass_mudown, BDT_bH_tauH_SVFIT_InvMass_mudown,
        BDT_bH_tauH_InvMass_mudown, HHKin_mass_mudown, HHKin_chi2_mudown, BDT_MET_bH_cosTheta_mudown);
      std::vector<float> BDTouts_mudown = BDTreader.GetPredictions();
      BDToutSM_kl_1_mudown = BDTouts_mudown.at(0);
    }


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
        float mt1_eleup                           = ComputeMT(tau1_eleup, met_eleup);
        float mt2_eleup                           = ComputeMT(tau2_eleup, met_eleup);
        float BDT_ditau_deltaPhi_eleup            = ROOT::Math::VectorUtil::DeltaPhi(tau1_eleup, tau2_eleup);
        float BDT_tauHsvfitMet_deltaPhi_eleup     = ROOT::Math::VectorUtil::DeltaPhi(svfit_eleup, met_eleup);
        float mT_tauH_MET_eleup                   = Calculate_MT( (tau1_eleup+tau2_eleup)+met_eleup, met_eleup);
        float BDT_MX_eleup                        = Calculate_MX(tau1_eleup, tau2_eleup, bjet1, bjet2, met_eleup);
        float BDT_bH_tauH_MET_InvMass_eleup       = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_eleup+tau2_eleup)+met_eleup);
        float BDT_bH_tauH_SVFIT_InvMass_eleup     = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), svfit_eleup);
        float BDT_bH_tauH_InvMass_eleup           = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_eleup+tau2_eleup));
        float BDT_MET_bH_cosTheta_eleup           = Calculate_cosTheta_2bodies(getLVfromTLV(met_eleup), getLVfromTLV(bjet1+bjet2));
        std::pair<double, double> topMasses_eleup = Calculate_topPairMasses(getLVfromTLV(tau1_eleup), getLVfromTLV(tau2_eleup), getLVfromTLV(bjet1), getLVfromTLV(bjet2), getLVfromTLV(met_eleup));
        bool KinFitConv_eleup                     = HHKin_chi2_eleup > 0;
        bool SVfitConv_eleup                      = tauH_SVFIT_mass_eleup.at(i) > 0;

        TLorentzVector svfit_eledown;
        svfit_eledown.SetPtEtaPhiM(tauH_SVFIT_pt_eledown, tauH_SVFIT_eta_eledown, tauH_SVFIT_phi_eledown, tauH_SVFIT_mass_eledown.at(i));
        float mTtot_eledown                         = Calculate_TotalMT(tau1_eledown, tau2_eledown, met_eledown);
        float pzeta_vis_eledown                     = Calculate_visiblePzeta(tau1_eledown, tau2_eledown);
        float pzeta_eledown                         = Calculate_Pzeta(tau1_eledown, tau2_eledown, met_eledown);
        float mt1_eledown                           = ComputeMT(tau1_eledown, met_eledown);
        float mt2_eledown                           = ComputeMT(tau2_eledown, met_eledown);
        float BDT_ditau_deltaPhi_eledown            = ROOT::Math::VectorUtil::DeltaPhi(tau1_eledown, tau2_eledown);
        float BDT_tauHsvfitMet_deltaPhi_eledown     = ROOT::Math::VectorUtil::DeltaPhi(svfit_eledown, met_eledown);
        float mT_tauH_MET_eledown                   = Calculate_MT( (tau1_eledown+tau2_eledown)+met_eledown, met_eledown);
        float BDT_MX_eledown                        = Calculate_MX(tau1_eledown, tau2_eledown, bjet1, bjet2, met_eledown);
        float BDT_bH_tauH_MET_InvMass_eledown       = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_eledown+tau2_eledown)+met_eledown);
        float BDT_bH_tauH_SVFIT_InvMass_eledown     = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), svfit_eledown);
        float BDT_bH_tauH_InvMass_eledown           = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_eledown+tau2_eledown));
        float BDT_MET_bH_cosTheta_eledown           = Calculate_cosTheta_2bodies(getLVfromTLV(met_eledown), getLVfromTLV(bjet1+bjet2));
        std::pair<double, double> topMasses_eledown = Calculate_topPairMasses(getLVfromTLV(tau1_eledown), getLVfromTLV(tau2_eledown), getLVfromTLV(bjet1), getLVfromTLV(bjet2), getLVfromTLV(met_eledown));
        bool KinFitConv_eledown                     = HHKin_chi2_eledown > 0;
        bool SVfitConv_eledown                      = tauH_SVFIT_mass_eledown.at(i) > 0;

        if (doDNN)
        {
          DNNreader.SetShiftedInputs(bjet1, bjet2, tau1_eleup, tau2_eleup, vbfjet1, vbfjet1, met_eleup, svfit_eleup,
              HHKin_mass_eleup, HHKin_chi2_eleup, KinFitConv_eleup, SVfitConv_eleup, MT2_eleup,
              mTtot_eleup, pzeta_vis_eleup, pzeta_eleup, topMasses_eleup.first, topMasses_eleup.second, mt1_eleup, mt2_eleup);
          std::vector<float> outs_eleup = DNNreader.GetPredictions();
          DNNoutSM_kl_1_eleup.at(i) = outs_eleup.at(0);

          DNNreader.SetShiftedInputs(bjet1, bjet2, tau1_eledown, tau2_eledown, vbfjet1, vbfjet1, met_eledown, svfit_eledown,
              HHKin_mass_eledown, HHKin_chi2_eledown, KinFitConv_eledown, SVfitConv_eledown, MT2_eledown,
              mTtot_eledown, pzeta_vis_eledown, pzeta_eledown, topMasses_eledown.first, topMasses_eledown.second, mt1_eledown, mt2_eledown);
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
            BDT_tauHsvfitMet_deltaPhi_eledown, mT_tauH_MET_eledown, mTtot, MT2_eledown,
            BDT_MX_eledown, BDT_bH_tauH_MET_InvMass_eledown, BDT_bH_tauH_SVFIT_InvMass_eledown,
            BDT_bH_tauH_InvMass_eledown, HHKin_mass_eledown, HHKin_chi2_eledown, BDT_MET_bH_cosTheta_eledown);
          std::vector<float> BDTouts_eledown = BDTreader.GetPredictions();
          BDToutSM_kl_1_eledown.at(i) = BDTouts_eledown.at(0);
        }
      }
    }

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

        // --- --- --- EES DNN/BDT quantities --- --- ---
        TLorentzVector svfit_tauup;
        svfit_tauup.SetPtEtaPhiM(tauH_SVFIT_pt_tauup, tauH_SVFIT_eta_tauup, tauH_SVFIT_phi_tauup, tauH_SVFIT_mass_tauup.at(i));
        float mTtot_tauup                         = Calculate_TotalMT(tau1_tauup, tau2_tauup, met_tauup);
        float pzeta_vis_tauup                     = Calculate_visiblePzeta(tau1_tauup, tau2_tauup);
        float pzeta_tauup                         = Calculate_Pzeta(tau1_tauup, tau2_tauup, met_tauup);
        float mt1_tauup                           = ComputeMT(tau1_tauup, met_tauup);
        float mt2_tauup                           = ComputeMT(tau1_tauup, met_tauup);
        float BDT_ditau_deltaPhi_tauup            = ROOT::Math::VectorUtil::DeltaPhi(tau1_tauup, tau2_tauup);
        float BDT_tauHsvfitMet_deltaPhi_tauup     = ROOT::Math::VectorUtil::DeltaPhi(svfit_tauup, met_tauup);
        float mT_tauH_MET_tauup                   = Calculate_MT( (tau1_tauup+tau2_tauup)+met_tauup, met_tauup);
        float BDT_MX_tauup                        = Calculate_MX(tau1_tauup, tau2_tauup, bjet1, bjet2, met_tauup);
        float BDT_bH_tauH_MET_InvMass_tauup       = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_tauup+tau2_tauup)+met_tauup);
        float BDT_bH_tauH_SVFIT_InvMass_tauup     = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), svfit_tauup);
        float BDT_bH_tauH_InvMass_tauup           = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_tauup+tau2_tauup));
        float BDT_MET_bH_cosTheta_tauup           = Calculate_cosTheta_2bodies(getLVfromTLV(met_tauup), getLVfromTLV(bjet1+bjet2));
        std::pair<double, double> topMasses_tauup = Calculate_topPairMasses(getLVfromTLV(tau1_tauup), getLVfromTLV(tau2_tauup), getLVfromTLV(bjet1), getLVfromTLV(bjet2), getLVfromTLV(met_tauup));
        bool KinFitConv_tauup                     = HHKin_chi2_tauup > 0;
        bool SVfitConv_tauup                      = tauH_SVFIT_mass_tauup.at(i) > 0;

        TLorentzVector svfit_taudown;
        svfit_taudown.SetPtEtaPhiM(tauH_SVFIT_pt_taudown, tauH_SVFIT_eta_taudown, tauH_SVFIT_phi_taudown, tauH_SVFIT_mass_taudown.at(i));
        float mTtot_taudown                         = Calculate_TotalMT(tau1_taudown, tau2_taudown, met_taudown);
        float pzeta_vis_taudown                     = Calculate_visiblePzeta(tau1_taudown, tau2_taudown);
        float pzeta_taudown                         = Calculate_Pzeta(tau1_taudown, tau2_taudown, met_taudown);
        float mt1_taudown                           = ComputeMT(tau1_taudown, met_taudown);
        float mt2_taudown                           = ComputeMT(tau2_taudown, met_taudown);
        float BDT_ditau_deltaPhi_taudown            = ROOT::Math::VectorUtil::DeltaPhi(tau1_taudown, tau2_taudown);
        float BDT_tauHsvfitMet_deltaPhi_taudown     = ROOT::Math::VectorUtil::DeltaPhi(svfit_taudown, met_taudown);
        float mT_tauH_MET_taudown                   = Calculate_MT( (tau1_taudown+tau2_taudown)+met_taudown, met_taudown);
        float BDT_MX_taudown                        = Calculate_MX(tau1_taudown, tau2_taudown, bjet1, bjet2, met_taudown);
        float BDT_bH_tauH_MET_InvMass_taudown       = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_taudown+tau2_taudown)+met_taudown);
        float BDT_bH_tauH_SVFIT_InvMass_taudown     = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), svfit_taudown);
        float BDT_bH_tauH_InvMass_taudown           = ROOT::Math::VectorUtil::InvariantMass((bjet1+bjet2), (tau1_taudown+tau2_taudown));
        float BDT_MET_bH_cosTheta_taudown           = Calculate_cosTheta_2bodies(getLVfromTLV(met_taudown), getLVfromTLV(bjet1+bjet2));
        std::pair<double, double> topMasses_taudown = Calculate_topPairMasses(getLVfromTLV(tau1_taudown), getLVfromTLV(tau2_taudown), getLVfromTLV(bjet1), getLVfromTLV(bjet2), getLVfromTLV(met_taudown));
        bool KinFitConv_taudown                     = HHKin_chi2_taudown > 0;
        bool SVfitConv_taudown                      = tauH_SVFIT_mass_taudown.at(i) > 0;

        if (doDNN)
        {
          DNNreader.SetShiftedInputs(bjet1, bjet2, tau1_tauup, tau2_tauup, vbfjet1, vbfjet1, met_tauup, svfit_tauup,
              HHKin_mass_tauup, HHKin_chi2_tauup, KinFitConv_tauup, SVfitConv_tauup, MT2_tauup,
              mTtot_tauup, pzeta_vis_tauup, pzeta_tauup, topMasses_tauup.first, topMasses_tauup.second, mt1_tauup, mt2_tauup);
          std::vector<float> outs_tauup = DNNreader.GetPredictions();
          DNNoutSM_kl_1_tauup.at(i) = outs_tauup.at(0);

          DNNreader.SetShiftedInputs(bjet1, bjet2, tau1_taudown, tau2_taudown, vbfjet1, vbfjet1, met_taudown, svfit_taudown,
              HHKin_mass_taudown, HHKin_chi2_taudown, KinFitConv_taudown, SVfitConv_taudown, MT2_taudown,
              mTtot_taudown, pzeta_vis_taudown, pzeta_taudown, topMasses_taudown.first, topMasses_taudown.second, mt1_taudown, mt2_taudown);
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
            BDT_tauHsvfitMet_deltaPhi_taudown, mT_tauH_MET_taudown, mTtot, MT2_taudown,
            BDT_MX_taudown, BDT_bH_tauH_MET_InvMass_taudown, BDT_bH_tauH_SVFIT_InvMass_taudown,
            BDT_bH_tauH_InvMass_taudown, HHKin_mass_taudown, HHKin_chi2_taudown, BDT_MET_bH_cosTheta_taudown);
          std::vector<float> BDTouts_taudown = BDTreader.GetPredictions();
          BDToutSM_kl_1_taudown.at(i) = BDTouts_taudown.at(0);
        }
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
  }
  cout << "** INFO: ..Added new branches" << endl;
  cout << "** INFO: Final entries: " << outTree->GetEntries() << endl;

  outFile->cd();
  outTree->Write ("", TObject::kOverwrite) ;
  outFile->Write();
  outFile->Close();


}
