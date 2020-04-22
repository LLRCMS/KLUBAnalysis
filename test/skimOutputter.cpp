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

const int nMaxEvts = -1;

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

  //TString inputFileName = "/gwteraz/users/brivio/SKIMMED_Legacy2017_27mar2020_DNN/SKIM_GGHHSM/output_0.root";
  //TString inputFileName = "/afs/cern.ch/user/d/dzuolo/public/VBF2016_Sync_26mar20.root";
  //TString inputFileName = "/gwteraz/users/brivio/SKIMMED_Legacy2017_27mar2020_DNN/SKIM_VBFHHSM/output_1.root";
  TString inputFileName = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/DNN2/CMSSW_10_2_16/src/KLUBAnalysis/skimmed_output1_10evts.root";
  TString outputFileName = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/DNN2/CMSSW_10_2_16/src/KLUBAnalysis/systTEST.root";
  cout << "** INFO: inputFile  : " << inputFileName << endl;
  cout << "** INFO: outputFile : " << outputFileName << endl;

  bool doMT2    = false; // FIXME: read from cfg file
  bool doKinFit = false; // FIXME: read from cfg file
  bool doSVfit  = false; // FIXME: read from cfg file
  bool doDNN    = true;  // FIXME: read from cfg file

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

  "dau1_pt","dau1_eta","dau1_phi","dau1_e",                            // Taus kinematic
  "dau2_pt","dau2_eta","dau2_phi","dau2_e",

  "bjet1_pt","bjet1_eta","bjet1_phi","bjet1_e","bjet1_JER",            // b-jets
  "bjet2_pt","bjet2_eta","bjet2_phi","bjet2_e","bjet2_JER",

  "VBFjet1_pt","VBFjet1_eta","VBFjet1_phi","VBFjet1_e",                // vbf-jets
  "VBFjet2_pt","VBFjet2_eta","VBFjet2_phi","VBFjet2_e",

  "met_phi","met_et","met_cov00","met_cov01","met_cov10","met_cov11",  // MET
  //"met_x","met_y",

  "VBFjj_mass", "VBFjj_deltaEta",                                      // VBF selection

  "HHKin_mass","HHKin_chi2", "MT2",                                    // Old values KinFit, MT2, SVfit, DNN
  "tauH_SVFIT_pt","tauH_SVFIT_eta","tauH_SVFIT_phi","tauH_SVFIT_mass",
  "DNNoutSM_kl_1"
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

  // Declare DNNKLUBinterface
  std::string model_dir = "../cms_runII_dnn_models/models/nonres_gluglu/2020-03-11-0/ensemble";
  std::string features_file = "../cms_runII_dnn_models/models/nonres_gluglu/2020-03-11-0/features.txt";
  std::vector<std::string> requested;
  std::ifstream features_infile(features_file);
  std::string featureName;
  while ( features_infile >> featureName)
  {
    requested.push_back(featureName);
  }
  features_infile.close();
  std::vector<float> target_kls {1.};
  DNNKLUBinterface DNNreader(model_dir, requested, target_kls);
  DNNreader.SetGlobalInputs(y17, nonres);

  // Read branches needed for computation of KinFit, MT2, SVfit, BDT, DNN
  ULong64_t EventNumber;
  Float_t dau1_pt, dau1_eta, dau1_phi, dau1_e, dau2_pt, dau2_eta, dau2_phi, dau2_e;
  Int_t DM1, DM2, pType, isBoosted, isVBF;
  Float_t bjet1_pt, bjet1_eta, bjet1_phi, bjet1_e, bjet1_JER, bjet2_pt, bjet2_eta, bjet2_phi, bjet2_e, bjet2_JER;
  Float_t vbfjet1_pt, vbfjet1_eta, vbfjet1_phi, vbfjet1_e, vbfjet2_pt, vbfjet2_eta, vbfjet2_phi, vbfjet2_e;
  Float_t met_phi, met_et, met_cov00, met_cov01, met_cov10, met_cov11; // met_x, met_y;
  Float_t HHKin_mass, HHKin_chi2, MT2, tauH_SVFIT_pt, tauH_SVFIT_eta, tauH_SVFIT_phi, tauH_SVFIT_mass, DNNoutSM_kl_1;
  float bjet1_bID_deepFlavor, bjet2_bID_deepFlavor, bjet1_bID_deepCSV, bjet2_bID_deepCSV;

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

  outTree->SetBranchAddress("met_phi"   , &met_phi);
  outTree->SetBranchAddress("met_et"    , &met_et);
  outTree->SetBranchAddress("met_cov00" , &met_cov00);
  outTree->SetBranchAddress("met_cov01" , &met_cov01);
  outTree->SetBranchAddress("met_cov10" , &met_cov10);
  outTree->SetBranchAddress("met_cov11" , &met_cov11);
  //outTree->SetBranchAddress("met_x"     , &met_x);
  //outTree->SetBranchAddress("met_y"     , &met_y);

  outTree->SetBranchAddress("MT2", &MT2);                         // FIXME: To be removed later
  outTree->SetBranchAddress("HHKin_mass", &HHKin_mass);           // FIXME: To be removed later
  outTree->SetBranchAddress("HHKin_chi2", &HHKin_chi2);           // FIXME: To be removed later
  outTree->SetBranchAddress("tauH_SVFIT_pt"  , &tauH_SVFIT_pt  ); // FIXME: To be removed later
  outTree->SetBranchAddress("tauH_SVFIT_eta" , &tauH_SVFIT_eta ); // FIXME: To be removed later
  outTree->SetBranchAddress("tauH_SVFIT_phi" , &tauH_SVFIT_phi ); // FIXME: To be removed later
  outTree->SetBranchAddress("tauH_SVFIT_mass", &tauH_SVFIT_mass); // FIXME: To be removed later
  outTree->SetBranchAddress("DNNoutSM_kl_1", &DNNoutSM_kl_1);     // FIXME: To be removed later

  // Declare new branches
  Float_t HHKin_mass_new, HHKin_mass_chi2_new, MT2_new;
  Float_t tauH_SVFIT_pt_new, tauH_SVFIT_eta_new, tauH_SVFIT_phi_new, tauH_SVFIT_mass_new;
  Float_t DNNoutSM_kl_1_new;
  TBranch* b_HHKin_mass_new = outTree->Branch("HHKin_mass_new", &HHKin_mass_new);
  TBranch* b_HHKin_mass_chi2_new = outTree->Branch("HHKin_mass_chi2_new", &HHKin_mass_chi2_new);
  TBranch* b_MT2_new = outTree->Branch("MT2_new", &MT2_new);
  TBranch* b_tauH_SVFIT_pt_new   = outTree->Branch("tauH_SVFIT_pt_new"  , &tauH_SVFIT_pt_new);
  TBranch* b_tauH_SVFIT_eta_new  = outTree->Branch("tauH_SVFIT_eta_new" , &tauH_SVFIT_eta_new);
  TBranch* b_tauH_SVFIT_phi_new  = outTree->Branch("tauH_SVFIT_phi_new" , &tauH_SVFIT_phi_new);
  TBranch* b_tauH_SVFIT_mass_new = outTree->Branch("tauH_SVFIT_mass_new", &tauH_SVFIT_mass_new);
  TBranch* b_DNNoutSM_kl_1_new = outTree->Branch("DNNoutSM_kl_1_new", &DNNoutSM_kl_1_new);

  // Loop on selected entries
  cout << "** INFO: Adding new branches..." << endl;
  for(int i=0;i<outTree->GetEntries();i++)
  {
    if (i % 500 == 0)  cout << "- reading event " << i << endl ;
    if (i == nMaxEvts ) break;
    //std::cout << "---------------- Event: " << i << " : " << EventNumber << " -------------------" << std::endl;

    // Get Entry
    outTree->GetEntry(i);

    // Build needed quantities
    TLorentzVector tau1, tau2, bjet1, bjet2, vbfjet1, vbfjet2, met, svfit;
    tau1.SetPtEtaPhiE(dau1_pt, dau1_eta, dau1_phi, dau1_e);
    tau2.SetPtEtaPhiE(dau2_pt, dau2_eta, dau2_phi, dau2_e);
    bjet1.SetPtEtaPhiE(bjet1_pt, bjet1_eta, bjet1_phi, bjet1_e);
    bjet2.SetPtEtaPhiE(bjet2_pt, bjet2_eta, bjet2_phi, bjet2_e);
    vbfjet1.SetPtEtaPhiE(vbfjet1_pt, vbfjet1_eta, vbfjet1_phi, vbfjet1_e);
    vbfjet2.SetPtEtaPhiE(vbfjet2_pt, vbfjet2_eta, vbfjet2_phi, vbfjet2_e);
    met.SetPtEtaPhiE(met_et, 0, met_phi, 0);
    svfit.SetPtEtaPhiM(tauH_SVFIT_pt, tauH_SVFIT_eta, tauH_SVFIT_phi, tauH_SVFIT_mass);

    TVector2 ptmiss = TVector2(met.Px(), met.Py()) ;
    TMatrixD stableMetCov (2, 2) ;
    stableMetCov (0,0) = met_cov00;
    stableMetCov (1,0) = met_cov01;
    stableMetCov (0,1) = met_cov10;
    stableMetCov (1,1) = met_cov11;

    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // KinFit computation
    if (doKinFit)
    {
      HHKinFit2::HHKinFitMasterHeavyHiggs kinFits = HHKinFit2::HHKinFitMasterHeavyHiggs(bjet1, bjet2, tau1, tau2, ptmiss, stableMetCov, bjet1_JER, bjet2_JER) ;
      kinFits.addHypo(hypo_mh1,hypo_mh2);

      // nominal kinfit
      bool wrongHHK =false;
      try {kinFits.fit();}
      catch(HHKinFit2::HHInvMConstraintException   e) {wrongHHK=true;}
      catch(HHKinFit2::HHEnergyConstraintException e) {wrongHHK=true;}
      catch(HHKinFit2::HHEnergyRangeException      e) {wrongHHK=true;}
      if(!wrongHHK)
      {
        HHKin_mass_new      = kinFits.getMH();
        HHKin_mass_chi2_new = kinFits.getChi2();
      }
      else
      {
        HHKin_mass_new      = -333.;
        HHKin_mass_chi2_new = 0.;
      }
    }


    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // MT2 computation
    if (doMT2)
    {
      double desiredPrecisionOnMt2 = 0; // Must be >=0.  If 0 alg aims for machine precision.  if >0, MT2 computed to supplied absolute precision.
      asymm_mt2_lester_bisect::disableCopyrightMessage();

      double mVisA = bjet1.M();  // mass of visible object on side A.  Must be >=0.
      double pxA   = bjet1.Px(); // x momentum of visible object on side A.
      double pyA   = bjet1.Py(); // y momentum of visible object on side A.

      double mVisB = bjet2.M();  // mass of visible object on side B.  Must be >=0.
      double pxB   = bjet2.Px(); // x momentum of visible object on side B.
      double pyB   = bjet2.Py(); // y momentum of visible object on side B.

      double pxMiss = tau1.Px() + tau2.Px() + met.Px(); // x component of missing transverse momentum.
      double pyMiss = tau1.Py() + tau2.Py() + met.Py(); // y component of missing transverse momentum.

      double chiA = tau1.M(); // hypothesised mass of invisible on side A.  Must be >=0.
      double chiB = tau2.M(); // hypothesised mass of invisible on side B.  Must be >=0.

      MT2_new = asymm_mt2_lester_bisect::get_mT2( mVisA, pxA, pyA, mVisB, pxB, pyB, pxMiss, pyMiss, chiA, chiB, desiredPrecisionOnMt2);
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
    }


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
      bool KinFitConv = HHKin_chi2 > 0;
      bool SVfitConv  = tauH_SVFIT_mass > 0;
      float mTtot     = Calculate_TotalMT(tau1, tau2, met);
      float pzeta_vis = Calculate_visiblePzeta(tau1, tau2);
      float pzeta     = Calculate_Pzeta(tau1, tau2, met);
      float mt1       = ComputeMT(tau1,met);
      float mt2       = ComputeMT(tau2,met);

      // For Calculate_topPairMasses use a redefined met (avoid rounding problems)
      TLorentzVector metDNN;
      metDNN.SetPxPyPzE( met.Px(), met.Py(), 0, std::hypot(met.Px(), met.Py()));
      std::pair<double, double> topMasses = Calculate_topPairMasses(getLVfromTLV(tau1), getLVfromTLV(tau2), getLVfromTLV(bjet1), getLVfromTLV(bjet2), getLVfromTLV(metDNN));

      DNNreader.SetShiftedInputs(bjet1, bjet2, tau1, tau2, vbfjet1, vbfjet1, met, svfit,
          HHKin_mass, HHKin_chi2, KinFitConv, SVfitConv, MT2,
          mTtot, pzeta_vis, pzeta, topMasses.first, topMasses.second, mt1, mt2);

      //std::cout << "----- getting predictions..." << std::endl;
      std::vector<float> outs = DNNreader.GetPredictions();
      //std::cout << "----- ...gotten predictions: " << outs.at(0) << std::endl;
      DNNoutSM_kl_1_new = outs.at(0);
    }


    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // Fill new branches
    b_HHKin_mass_new->Fill();
    b_HHKin_mass_chi2_new->Fill();
    b_MT2_new->Fill();
    b_tauH_SVFIT_pt_new->Fill();
    b_tauH_SVFIT_eta_new->Fill();
    b_tauH_SVFIT_phi_new->Fill();
    b_tauH_SVFIT_mass_new->Fill();
    b_DNNoutSM_kl_1_new->Fill();
  }
  cout << "** INFO: ..Added new branches" << endl;
  cout << "** INFO: Final entries: " << outTree->GetEntries() << endl;

  outFile->cd();
  outTree->Write ("", TObject::kOverwrite) ;
  outFile->Write();
  outFile->Close();


}
