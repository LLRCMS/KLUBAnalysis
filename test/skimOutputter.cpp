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

using namespace std ;
using DNNVector = ROOT::Math::LorentzVector<ROOT::Math::PxPyPzM4D<float>>;

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


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -

// Main
int main (int argc, char** argv)
{

  //TString inputFileName = "/gwteraz/users/brivio/SKIMMED_Legacy2017_27mar2020_DNN/SKIM_GGHHSM/output_0.root";
  TString inputFileName = "/afs/cern.ch/user/d/dzuolo/public/VBF2016_Sync_26mar20.root";
  TString outputFileName = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/DNN2/CMSSW_10_2_16/src/KLUBAnalysis/systTEST.root";
  cout << "** INFO: inputFile  : " << inputFileName << endl;
  cout << "** INFO: outputFile : " << outputFileName << endl;

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
  "EventNumber", "RunNumber","nleps","pairType","nbjetscand", "isOS",  // General

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

  "met_phi","met_et","met_cov00","met_cov01","met_cov10","met_cov11",  // MET

  "VBFjj_mass", "VBFjj_deltaEta",                                      // VBF selection

  "HHKin_mass","HHKin_chi2", "MT2",                                    // Old values KinFit, MT2, SVfit
  "tauH_SVFIT_pt","tauH_SVFIT_eta","tauH_SVFIT_phi","tauH_SVFIT_mass"
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
    if ( nleps!=0 || nbjetscand<2 || pairType>3) continue;
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

  // Read branches needed for computation of KinFit, MT2, SVfit, BDT, DNN
  ULong64_t EventNumber;
  Float_t dau1_pt, dau1_eta, dau1_phi, dau1_e, dau2_pt, dau2_eta, dau2_phi, dau2_e;
  Int_t DM1, DM2, pType;
  Float_t bjet1_pt, bjet1_eta, bjet1_phi, bjet1_e, bjet1_JER, bjet2_pt, bjet2_eta, bjet2_phi, bjet2_e, bjet2_JER;
  Float_t met_phi, met_et, met_cov00, met_cov01, met_cov10, met_cov11;
  Float_t HHKin_mass, HHKin_chi2, MT2, tauH_SVFIT_pt, tauH_SVFIT_eta, tauH_SVFIT_phi, tauH_SVFIT_mass;

  outTree->SetBranchAddress("EventNumber" , &EventNumber);

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

  outTree->SetBranchAddress("met_phi"   , &met_phi);
  outTree->SetBranchAddress("met_et"    , &met_et);
  outTree->SetBranchAddress("met_cov00" , &met_cov00);
  outTree->SetBranchAddress("met_cov01" , &met_cov01);
  outTree->SetBranchAddress("met_cov10" , &met_cov10);
  outTree->SetBranchAddress("met_cov11" , &met_cov11);

  outTree->SetBranchAddress("HHKin_mass", &HHKin_mass); // FIXME: To be removed later
  outTree->SetBranchAddress("HHKin_chi2", &HHKin_chi2); // FIXME: To be removed later
  outTree->SetBranchAddress("MT2", &MT2);               // FIXME: To be removed later
  outTree->SetBranchAddress("tauH_SVFIT_pt"  , &tauH_SVFIT_pt  ); // FIXME: To be removed later
  outTree->SetBranchAddress("tauH_SVFIT_eta" , &tauH_SVFIT_eta ); // FIXME: To be removed later
  outTree->SetBranchAddress("tauH_SVFIT_phi" , &tauH_SVFIT_phi ); // FIXME: To be removed later
  outTree->SetBranchAddress("tauH_SVFIT_mass", &tauH_SVFIT_mass); // FIXME: To be removed later

  // Declare new branches
  Float_t HHKin_mass_new, HHKin_mass_chi2_new, MT2_new;
  Float_t tauH_SVFIT_pt_new, tauH_SVFIT_eta_new, tauH_SVFIT_phi_new, tauH_SVFIT_mass_new;
  TBranch* b_HHKin_mass_new = outTree->Branch("HHKin_mass_new", &HHKin_mass_new);
  TBranch* b_HHKin_mass_chi2_new = outTree->Branch("HHKin_mass_chi2_new", &HHKin_mass_chi2_new);
  TBranch* b_MT2_new = outTree->Branch("MT2_new", &MT2_new);
  TBranch* b_tauH_SVFIT_pt_new   = outTree->Branch("tauH_SVFIT_pt_new"  , &tauH_SVFIT_pt_new);
  TBranch* b_tauH_SVFIT_eta_new  = outTree->Branch("tauH_SVFIT_eta_new" , &tauH_SVFIT_eta_new);
  TBranch* b_tauH_SVFIT_phi_new  = outTree->Branch("tauH_SVFIT_phi_new" , &tauH_SVFIT_phi_new);
  TBranch* b_tauH_SVFIT_mass_new = outTree->Branch("tauH_SVFIT_mass_new", &tauH_SVFIT_mass_new);

  // Loop on selected entries
  cout << "** INFO: Adding new branches..." << endl;
  for(int i=0;i<outTree->GetEntries();i++)
  {
    if (i % 500 == 0)  cout << "- reading event " << i << endl ;
    if (i == nMaxEvts ) break;

    // Get Entry
    outTree->GetEntry(i);

    // Build needed quantities
    TLorentzVector tau1, tau2, bjet1, bjet2, met;
    tau1.SetPtEtaPhiE(dau1_pt, dau1_eta, dau1_phi, dau1_e);
    tau2.SetPtEtaPhiE(dau2_pt, dau2_eta, dau2_phi, dau2_e);
    bjet1.SetPtEtaPhiE(bjet1_pt, bjet1_eta, bjet1_phi, bjet1_e);
    bjet2.SetPtEtaPhiE(bjet2_pt, bjet2_eta, bjet2_phi, bjet2_e);
    met.SetPtEtaPhiE(met_et, 0, met_phi, 0);

    TVector2 ptmiss = TVector2(met.Px(), met.Py()) ;
    TMatrixD stableMetCov (2, 2) ;
    stableMetCov (0,0) = met_cov00;
    stableMetCov (1,0) = met_cov01;
    stableMetCov (0,1) = met_cov10;
    stableMetCov (1,1) = met_cov11;

    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // KinFit computation
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


    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // MT2 computation
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


    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // SVfit computation
    SVfitKLUBinterface algo_central(0, tau1, tau2, met, stableMetCov, pType, DM1, DM2);
    std::vector<double> svfitRes = algo_central.FitAndGetResult();
    tauH_SVFIT_pt_new   = svfitRes.at(0);
    tauH_SVFIT_eta_new  = svfitRes.at(1);
    tauH_SVFIT_phi_new  = svfitRes.at(2);
    tauH_SVFIT_mass_new = svfitRes.at(3);


    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    // Fill new branches
    b_HHKin_mass_new->Fill();
    b_HHKin_mass_chi2_new->Fill();
    b_MT2_new->Fill();
    b_tauH_SVFIT_pt_new->Fill();
    b_tauH_SVFIT_eta_new->Fill();
    b_tauH_SVFIT_phi_new->Fill();
    b_tauH_SVFIT_mass_new->Fill();
  }
  cout << "** INFO: ..Added new branches" << endl;
  cout << "** INFO: Final entries: " << outTree->GetEntries() << endl;

  outFile->cd();
  outTree->Write ("", TObject::kOverwrite) ;
  outFile->Write();
  outFile->Close();


}
