// c++ -o reduceNtuple `root-config --cflags --glibs --libs` -lm reduceNtuple.cpp
// taken form here https://root.cern.ch/root/html/tutorials/tree/copytree2.C.html

#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"
#include "TString.h"


using namespace std ;

int main (int argc, char** argv)
{
  if (argc < 3) 
    {
      cerr << "missing input parameters" << endl ;
      exit (1) ;
    }
  TString inputFile = argv[1] ;
  TString oututFile = argv[2] ;


//  gSystem->Load ("$ROOTSYS/test/libEvent") ;

  //Get old file, old tree and set top branch address
  TFile * oldfile = new TFile (inputFile) ;
  TTree * oldTree = (TTree*) oldfile->Get ("HTauTauTree/HTauTauTree") ;

  oldTree->SetBranchStatus ("*", 0) ;

  oldTree->SetBranchStatus ("EventNumber", 1) ;
  oldTree->SetBranchStatus ("RunNumber", 1) ;
  oldTree->SetBranchStatus ("lumi", 1) ;
  oldTree->SetBranchStatus ("triggerbit", 1) ;
  oldTree->SetBranchStatus ("metfilterbit", 1) ;
  oldTree->SetBranchStatus ("met", 1) ;
  oldTree->SetBranchStatus ("metphi", 1) ;
  oldTree->SetBranchStatus ("npv", 1) ;
  oldTree->SetBranchStatus ("npu", 1) ;
  oldTree->SetBranchStatus ("PUReweight", 1) ;
  oldTree->SetBranchStatus ("rho", 1) ;
  oldTree->SetBranchStatus ("mothers_px", 1) ;
  oldTree->SetBranchStatus ("mothers_py", 1) ;
  oldTree->SetBranchStatus ("mothers_pz", 1) ;
  oldTree->SetBranchStatus ("mothers_e", 1) ;
  oldTree->SetBranchStatus ("daughters_px", 1) ;
  oldTree->SetBranchStatus ("daughters_py", 1) ;
  oldTree->SetBranchStatus ("daughters_pz", 1) ;
  oldTree->SetBranchStatus ("daughters_e", 1) ;
  oldTree->SetBranchStatus ("daughters_charge", 1) ;

  oldTree->SetBranchStatus ("SVfitMass", 1) ;
  oldTree->SetBranchStatus ("SVfit_pt", 1) ;
  oldTree->SetBranchStatus ("SVfit_ptUnc", 1) ;
  oldTree->SetBranchStatus ("SVfit_eta", 1) ;
  oldTree->SetBranchStatus ("SVfit_etaUnc", 1) ;
  oldTree->SetBranchStatus ("SVfit_phi", 1) ;
  oldTree->SetBranchStatus ("SVfit_phiUnc", 1) ;
  oldTree->SetBranchStatus ("SVfit_fitMETRho", 1) ;
  oldTree->SetBranchStatus ("SVfit_fitMETPhi", 1) ;
  oldTree->SetBranchStatus ("isOSCand", 1) ;
  oldTree->SetBranchStatus ("METx", 1) ;
  oldTree->SetBranchStatus ("METy", 1) ;
  oldTree->SetBranchStatus ("MET_cov00", 1) ;
  oldTree->SetBranchStatus ("MET_cov01", 1) ;
  oldTree->SetBranchStatus ("MET_cov10", 1) ;
  oldTree->SetBranchStatus ("MET_cov11", 1) ;
  oldTree->SetBranchStatus ("MET_significance", 1) ;
  oldTree->SetBranchStatus ("mT_Dau1", 1) ;
  oldTree->SetBranchStatus ("mT_Dau2", 1) ;
  oldTree->SetBranchStatus ("PDGIdDaughters", 1) ;
  oldTree->SetBranchStatus ("indexDau1", 1) ;
  oldTree->SetBranchStatus ("indexDau2", 1) ;
  oldTree->SetBranchStatus ("particleType", 1) ;
  oldTree->SetBranchStatus ("discriminator", 1) ;
  oldTree->SetBranchStatus ("daughters_muonID", 1) ;
  oldTree->SetBranchStatus ("daughters_typeOfMuon", 1) ;
  oldTree->SetBranchStatus ("dxy", 1) ;
  oldTree->SetBranchStatus ("dz", 1) ; 
  oldTree->SetBranchStatus ("daughters_iseleBDT", 1) ;
  oldTree->SetBranchStatus ("daughters_eleCUTID", 1) ;
  oldTree->SetBranchStatus ("decayMode", 1) ;
  oldTree->SetBranchStatus ("combreliso", 1) ;
  oldTree->SetBranchStatus ("daughters_IetaIeta", 1) ;
  oldTree->SetBranchStatus ("daughters_deltaPhiSuperClusterTrackAtVtx", 1) ;
  oldTree->SetBranchStatus ("daughters_SCeta", 1) ; 
  oldTree->SetBranchStatus ("daughters_depositR03_tracker", 1) ; 
  oldTree->SetBranchStatus ("daughters_depositR03_ecal", 1) ; 
  oldTree->SetBranchStatus ("daughters_depositR03_hcal", 1) ; 
  oldTree->SetBranchStatus ("daughters_decayModeFindingOldDMs", 1) ;
  oldTree->SetBranchStatus ("daughters_decayModeFindingNewDMs", 1) ;
  oldTree->SetBranchStatus ("daughters_byLooseCombinedIsolationDeltaBetaCorr3Hits", 1) ;
  oldTree->SetBranchStatus ("daughters_byMediumCombinedIsolationDeltaBetaCorr3Hits", 1) ;
  oldTree->SetBranchStatus ("daughters_byTightCombinedIsolationDeltaBetaCorr3Hits", 1) ;
  oldTree->SetBranchStatus ("daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits", 1) ;
  oldTree->SetBranchStatus ("daughters_chargedIsoPtSum", 1) ; 
  oldTree->SetBranchStatus ("daughters_neutralIsoPtSum", 1) ; 
  oldTree->SetBranchStatus ("daughters_puCorrPtSum", 1) ;
  oldTree->SetBranchStatus ("daughters_againstMuonLoose3", 1) ;
  oldTree->SetBranchStatus ("daughters_againstMuonTight3", 1) ;
  oldTree->SetBranchStatus ("daughters_againstElectronVLooseMVA5", 1) ;
  oldTree->SetBranchStatus ("daughters_againstElectronLooseMVA5", 1) ;
  oldTree->SetBranchStatus ("daughters_againstElectronMediumMVA5", 1) ;
  oldTree->SetBranchStatus ("daughters_againstElectronTightMVA5", 1) ;
  oldTree->SetBranchStatus ("daughters_againstElectronVTightMVA5", 1) ;
  oldTree->SetBranchStatus ("daughters_isLastTriggerObjectforPath", 1) ;
  oldTree->SetBranchStatus ("daughters_isTriggerObjectforPath", 1) ;
  oldTree->SetBranchStatus ("daughters_FilterFired", 1) ;
  oldTree->SetBranchStatus ("daughters_isGoodTriggerType", 1) ;
  oldTree->SetBranchStatus ("daughters_L3FilterFired", 1) ; 
  oldTree->SetBranchStatus ("daughters_L3FilterFiredLast", 1) ;
  oldTree->SetBranchStatus ("JetsNumber", 1) ; 
  oldTree->SetBranchStatus ("jets_px", 1) ;
  oldTree->SetBranchStatus ("jets_py", 1) ;
  oldTree->SetBranchStatus ("jets_pz", 1) ;
  oldTree->SetBranchStatus ("jets_e", 1) ;
  oldTree->SetBranchStatus ("jets_Flavour", 1) ;
  oldTree->SetBranchStatus ("jets_PUJetID", 1) ;
  oldTree->SetBranchStatus ("bDiscriminator", 1) ;
  oldTree->SetBranchStatus ("bCSVscore", 1) ;
  oldTree->SetBranchStatus ("PFjetID", 1) ;
  oldTree->SetBranchStatus ("jetRawf", 1) ;

  // MC only
  if (oldTree->GetListOfBranches()->FindObject ("genpart_px"))
    {
      oldTree->SetBranchStatus ("daughters_genindex", 0) ;
      oldTree->SetBranchStatus ("MC_weight", 0) ; 
      oldTree->SetBranchStatus ("genpart_px", 0) ;
      oldTree->SetBranchStatus ("genpart_py", 0) ;
      oldTree->SetBranchStatus ("genpart_pz", 0) ;
      oldTree->SetBranchStatus ("genpart_e", 0) ; 
      oldTree->SetBranchStatus ("genpart_pdg", 0) ;
      oldTree->SetBranchStatus ("genpart_status", 0) ; 
      oldTree->SetBranchStatus ("genpart_HMothInd", 0) ;
      oldTree->SetBranchStatus ("genpart_TopMothInd", 0) ;
      oldTree->SetBranchStatus ("genpart_TauMothInd", 0) ;
      oldTree->SetBranchStatus ("genpart_ZMothInd", 0) ;
      oldTree->SetBranchStatus ("genpart_HZDecayMode", 0) ;
      oldTree->SetBranchStatus ("genpart_TauGenDecayMode", 0) ;
      oldTree->SetBranchStatus ("genpart_flags", 0) ;
      oldTree->SetBranchStatus ("NUP", 0) ;
     }

  //Create a new file + a clone of old tree header. Do not copy events
  TFile * newfile = new TFile (oututFile, "recreate") ;
  TTree * newTree = oldTree->CloneTree () ;

  newfile->Write () ;
  delete oldfile ;
  delete newfile ;
  return 0 ;
}