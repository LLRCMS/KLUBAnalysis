#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <bitset>
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TBranch.h"
#include "TString.h"
#include "TLorentzVector.h"

#include "bigTree.h" 

using namespace std;

// c++ -lm -o getGenMetFractions getGenMetFractions.cpp `root-config --glibs --cflags` -I ../../interface/

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
        if (!line.empty()){ // skip empty lines
            chain->Add(line.c_str());
            // cout << ".. added file:" << line.c_str() << endl;
        }
    }
    return;
}

bool CheckBit (int number, int bitpos)
{
    bool res = number & (1 << bitpos);
    return res;
}

// bool minGenCuts(TLorentzVector& vgen)
// {
//     if (vgen.Pt() < 20) return false;
//     if (TMath::Abs(vgen.Eta()) > 2.4) return false;
//     return true;
// }

int main (int argc, char** argv)
{
  // input and output setup
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  // TString inputFile = "/home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Files_7Feb2016/allRadion/61_GluGluToRadionToHHTo2B2Tau_M-300_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt" ; TString fOutName = "fOut_MET_Radion300.root";
  // TString inputFile = "/home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Files_7Feb2016/allRadion/67_GluGluToRadionToHHTo2B2Tau_M-600_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt" ; TString fOutName = "fOut_MET_Radion600.root";
  TString inputFile = "/home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Files_7Feb2016/allRadion/70_GluGluToRadionToHHTo2B2Tau_M-900_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt" ; TString fOutName = "fOut_MET_Radion900.root";

  cout << ".. start" << endl;

  TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;
  cout << ".. going to append to file" << endl;
  appendFromFileList (bigChain, inputFile);
  cout << ".. going to make the bigTree" << endl;
  bigTree theBigTree (bigChain) ;
  
  theBigTree.fChain->SetBranchStatus("*", 0);
  theBigTree.fChain->SetBranchStatus("genpart_*", 1);
  theBigTree.fChain->SetBranchStatus("METx", 1);
  theBigTree.fChain->SetBranchStatus("METy", 1);
  theBigTree.fChain->SetBranchStatus("met", 1);
  theBigTree.fChain->SetBranchStatus("metphi", 1);
  // theBigTree.fChain->SetBranchStatus("jets_*", 1);
  // theBigTree.fChain->SetBranchStatus("bCSVscore", 1);
  // theBigTree.fChain->SetBranchStatus("PFjetID", 1);
  
  cout << ".. going in the loop" << endl;

  TFile* fOut = new TFile(fOutName, "recreate");
  TH1F* hMET_etresol = new TH1F("hMET_etresol", "hMET_etresol", 100, 0, 10);
  TH1F* hMET_phiresol = new TH1F("hMET_phiresol", "hMET_phiresol", 100, -1.*TMath::Pi(), TMath::Pi());
  TH1F* hFracTau = new TH1F("hFracTau", "hFracTau", 100, 0, 1);

  for (uint iEv = 0; true; ++iEv)
  {
    int got = theBigTree.fChain->GetEntry(iEv);
    if (got == 0) break;
    if (iEv > 50000) break;

    if (iEv % 10000 == 0) cout << "... " << iEv << endl;

    TLorentzVector vMETtau (0,0,0,0);
    TLorentzVector vMETbs (0,0,0,0);

    // cout << "son qui" << iEv << endl;

    int nvisitedTau = 0;
    for (uint igen = 0; igen < theBigTree.genpart_px->size(); ++igen)
    {
      int apdg = theBigTree.genpart_pdg->at(igen);
      if (apdg != 12 && apdg != 14 && apdg != 16) continue;
      // if (!CheckBit(theBigTree.genpart_flags->at(igen), 12)) continue; // 12: first copy
      if (!CheckBit(theBigTree.genpart_flags->at(igen), 13)) {cout << "Not a last copy neutrino" << endl; continue;} // 13: last copy

      TLorentzVector vmore;
      vmore.SetPxPyPzE(
        theBigTree.genpart_px->at(igen),
        theBigTree.genpart_py->at(igen),
        theBigTree.genpart_pz->at(igen),
        theBigTree.genpart_e->at(igen)
        );


      if (CheckBit(theBigTree.genpart_flags->at(igen), 2)) // 2: isTauDecayProduct
      {
        vMETtau += vmore;
        nvisitedTau ++;
      }
      else
      {
        vMETbs += vmore;
      }
    }

    // sometimes it gives 1.. it is possible that there is a min pt cut in neutrinos stored in miniAOD
    // if (nvisitedTau <= 1)
    // {
    //   cout << "Did not found neutrinos from tautau in this event?? " << nvisitedTau << endl;
    // }


    // TLorentzVector metReco(0,0,0,0);
    // metReco.SetPxPyPzE(
    //   theBigTree.METx->at(0),
    //   theBigTree.METy->at(0),
    //   0,
    //   0
    // );

    // TLorentzVector metGenTot = vMETbs + vMETtau;

    // double tauMetFrac = vMETtau.Perp() / metGenTot.Perp();
    // hMET_etresol->Fill(metReco.Perp() / metGenTot.Perp());
    // hMET_phiresol->Fill(metReco.Phi() - metGenTot.Phi());

    TLorentzVector metGenTot = vMETbs + vMETtau;

    double tauMetFrac = vMETtau.Perp() / metGenTot.Perp();
    hFracTau->Fill(tauMetFrac);
    hMET_etresol->Fill(theBigTree.met / metGenTot.Perp());
    hMET_phiresol->Fill(TVector2::Phi_mpi_pi(theBigTree.metphi - metGenTot.Phi()));


  }

fOut->Write();



}