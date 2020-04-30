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

const int nMaxEvts = 10;

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

  "METx_muup","METy_muup","METx_mudown","METy_mudown",                 // MET MES
  "METx_eleup","METy_eleup","METx_eledown", "METy_eledown",            // MET EES
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
  DNNreader.SetGlobalInputs(y17, nonres);

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
  TBranch* b_tauH_SVFIT_mass_muup = outTree->Branch("tauH_SVFIT_mass_muup", &tauH_SVFIT_mass_muup);
  TBranch* b_DNNoutSM_kl_1_muup   = outTree->Branch("DNNoutSM_kl_1_muup", &DNNoutSM_kl_1_muup);
  TBranch* b_BDToutSM_kl_1_muup   = outTree->Branch("BDToutSM_kl_1_muup", &BDToutSM_kl_1_muup);
  TBranch* b_tauH_SVFIT_mass_mudown = outTree->Branch("tauH_SVFIT_mass_mudown", &tauH_SVFIT_mass_muup);
  TBranch* b_DNNoutSM_kl_1_mudown   = outTree->Branch("DNNoutSM_kl_1_mudown", &DNNoutSM_kl_1_mudown);
  TBranch* b_BDToutSM_kl_1_mudown   = outTree->Branch("BDToutSM_kl_1_mudown", &BDToutSM_kl_1_mudown);


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


    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // KinFit computation
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
    // MT2 computation
    if (doMT2)
    {
      asymm_mt2_lester_bisect::disableCopyrightMessage();
      double desiredPrecisionOnMt2 = 0; // Must be >=0.  If 0 alg aims for machine precision.  if >0, MT2 computed to supplied absolute precision.

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
    // SVfit computation
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


    // --- --- --- MES DNN/BDT quantities --- --- ---
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
    // DNN computation
    if (doDNN)
    {
      // Set quantities that are constant for each event
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
    // BDT computation
    if (doBDT)
    {
      // Redefine BDT channel (defined differently in the BDT weights)
      float BDT_channel;
      if      (pType == 0) BDT_channel = 1.;
      else if (pType == 1) BDT_channel = 0.;
      else if (pType == 2) BDT_channel = 2.;

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

      BDTreader.SetInputValues(bjet2.Pt(), (bjet1+bjet2).Pt(), tau1_mudown.Pt(),
        tau2_mudown.Pt(), svfit_mudown.Pt(), BDT_channel,
        BDT_HT20, pzeta_mudown, pzeta_vis_mudown, BDT_ditau_deltaPhi_mudown,
        BDT_tauHsvfitMet_deltaPhi_mudown, mT_tauH_MET_mudown, mTtot, MT2_mudown,
        BDT_MX_mudown, BDT_bH_tauH_MET_InvMass_mudown, BDT_bH_tauH_SVFIT_InvMass_mudown,
        BDT_bH_tauH_InvMass_mudown, HHKin_mass_mudown, HHKin_chi2_mudown, BDT_MET_bH_cosTheta_mudown);
      std::vector<float> BDTouts_mudown = BDTreader.GetPredictions();

    }

    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // Fill new branches
    // Central values
    b_tauH_SVFIT_pt_new->Fill();
    b_tauH_SVFIT_eta_new->Fill();
    b_tauH_SVFIT_phi_new->Fill();
    b_tauH_SVFIT_mass_new->Fill();

    // MES variations
    b_tauH_SVFIT_mass_muup->Fill();
    b_DNNoutSM_kl_1_muup->Fill();
    b_BDToutSM_kl_1_muup->Fill();
    b_tauH_SVFIT_mass_mudown->Fill();
    b_DNNoutSM_kl_1_mudown->Fill();
    b_BDToutSM_kl_1_mudown->Fill();

  }
  cout << "** INFO: ..Added new branches" << endl;
  cout << "** INFO: Final entries: " << outTree->GetEntries() << endl;

  outFile->cd();
  outTree->Write ("", TObject::kOverwrite) ;
  outFile->Write();
  outFile->Close();


}
