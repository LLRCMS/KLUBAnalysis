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

// c++ -lm -o bJetChoice bJetChoice.cpp `root-config --glibs --cflags` -I ../../interface/

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

bool minGenCuts(TLorentzVector& vgen)
{
    if (vgen.Pt() < 20) return false;
    if (TMath::Abs(vgen.Eta()) > 2.4) return false;
    return true;
}

int main (int argc, char** argv)
{
  // input and output setup
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  TString inputFile = "/home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Files_7Feb2016/allRadion/61_GluGluToRadionToHHTo2B2Tau_M-300_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt" ;

  cout << ".. start" << endl;

  TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;
  cout << ".. going to append to file" << endl;
  appendFromFileList (bigChain, inputFile);
  cout << ".. going to make the bigTree" << endl;
  bigTree theBigTree (bigChain) ;
  
  theBigTree.fChain->SetBranchStatus("*", 0);
  theBigTree.fChain->SetBranchStatus("genpart_*", 1);
  theBigTree.fChain->SetBranchStatus("jets_*", 1);
  theBigTree.fChain->SetBranchStatus("bCSVscore", 1);
  theBigTree.fChain->SetBranchStatus("PFjetID", 1);
  
  cout << ".. going in the loop" << endl;

  int nGoodSelected = 0;
  int nGoodSelectedGenCuts = 0;
  
  int nAllJetsMatched = 0;
  int nAllJetsMatchedGenCuts = 0;
  
  int nTot = 0;
  int nTotGenCuts = 0;

  TH1F* hMjjMatched = new TH1F("hMjjMatched", "hMjjMatched", 200, 0, 200);
  TH1F* hMjjAll = new TH1F("hMjjAll", "hMjjAll", 200, 0, 200);
  TH1F* dPhiMatched = new TH1F("dPhiMatched", "dPhiMatched", 100, -2.*TMath::Pi(), 2.*TMath::Pi());

  for (uint iEv = 0; true; ++iEv)
  {
    int got = theBigTree.fChain->GetEntry(iEv);
    if (got == 0) break;
    if (iEv > 50000) break;

    if (iEv % 10000 == 0) cout << "... " << iEv << endl;

    std::vector<int> vGenQuarks;
    for (uint igen = 0; igen < theBigTree.genpart_px->size(); ++igen)
    {
        if (abs(theBigTree.genpart_pdg->at(igen)) != 5) continue;
        // if (!CheckBit(theBigTree.genpart_flags->at(igen), 13)) continue; // 13: last copy
        if (!CheckBit(theBigTree.genpart_flags->at(igen), 12)) continue; // 12: first copy
        if (!CheckBit(theBigTree.genpart_flags->at(igen), 7)) continue; // 7: isHardProcess
        // if (!CheckBit(theBigTree.genpart_flags->at(igen), 8)) continue; // 8: fromHardProcess
        vGenQuarks.push_back(igen);
    }
    
    if (vGenQuarks.size() != 2)
    {
        cout << "Wrong b quark size: " << vGenQuarks.size() << endl;
        continue;
    }

    TLorentzVector vgenb1;
    TLorentzVector vgenb2;
    vgenb1.SetPxPyPzE(
        theBigTree.genpart_px->at(vGenQuarks.at(0)),
        theBigTree.genpart_py->at(vGenQuarks.at(0)),
        theBigTree.genpart_pz->at(vGenQuarks.at(0)),
        theBigTree.genpart_e->at(vGenQuarks.at(0))
    );
    vgenb2.SetPxPyPzE(
        theBigTree.genpart_px->at(vGenQuarks.at(1)),
        theBigTree.genpart_py->at(vGenQuarks.at(1)),
        theBigTree.genpart_pz->at(vGenQuarks.at(1)),
        theBigTree.genpart_e->at(vGenQuarks.at(1))
    );

    // select the jets - note: no overlap removal for simplicity, hopefully it is a small effect
    vector<pair<float, int>> jets_and_btag;

    vector<int> matchedTo1;
    vector<int> matchedTo2;


    int nBtagMed = 0;
    for (uint ijet = 0; ijet < theBigTree.jets_px->size(); ++ijet)
    {
        TLorentzVector tlv_jet 
        (
            theBigTree.jets_px->at (ijet),
            theBigTree.jets_py->at (ijet),
            theBigTree.jets_pz->at (ijet),
            theBigTree.jets_e->at (ijet)
        );

        // if (theBigTree.jets_PUJetID->at (iJet) < PUjetID_minCut) continue ;
        if (theBigTree.PFjetID->at (ijet) < 1) continue; // 0 ; don't pass PF Jet ID; 1: loose, 2: tight, 3: tightLepVeto
        if (tlv_jet.Pt () < 20. /*GeV*/) continue ; 
        if (TMath::Abs(tlv_jet.Eta()) > 2.4) continue; // 2.4 for b-tag

        jets_and_btag.push_back(make_pair(theBigTree.bCSVscore->at (ijet), ijet));
        if (theBigTree.bCSVscore->at (ijet) > 0.8484) nBtagMed +=1;
        
        if (vgenb1.DeltaR(tlv_jet) < 0.5) matchedTo1.push_back(ijet);
        if (vgenb2.DeltaR(tlv_jet) < 0.5) matchedTo2.push_back(ijet);

    }
    
    if (jets_and_btag.size() < 2) continue;
    stable_sort (jets_and_btag.begin(), jets_and_btag.end());

    int ijet1 = jets_and_btag.at(jets_and_btag.size()-1).second;
    int ijet2 = jets_and_btag.at(jets_and_btag.size()-2).second;
    
    TLorentzVector tlv_jet1 
    (
        theBigTree.jets_px->at (ijet1),
        theBigTree.jets_py->at (ijet1),
        theBigTree.jets_pz->at (ijet1),
        theBigTree.jets_e->at (ijet1)
    );

    TLorentzVector tlv_jet2 
    (
        theBigTree.jets_px->at (ijet2),
        theBigTree.jets_py->at (ijet2),
        theBigTree.jets_pz->at (ijet2),
        theBigTree.jets_e->at (ijet2)
    );

    hMjjAll->Fill((tlv_jet1+tlv_jet2).M());

    bool drA1 = vgenb1.DeltaR(tlv_jet1) < 0.5;
    bool drA2 = vgenb1.DeltaR(tlv_jet2) < 0.5;
    bool drB1 = vgenb2.DeltaR(tlv_jet1) < 0.5;
    bool drB2 = vgenb2.DeltaR(tlv_jet2) < 0.5;

    bool drA1B2 = (drA1 && drB2);
    bool drA2B1 = (drA2 && drB1);

    nTot += 1;
    
    bool genCuts = minGenCuts(vgenb1) && minGenCuts(vgenb2) ;

    if (genCuts)
        nTotGenCuts += 1;
    
    if (drA1B2 || drA2B1)
    {
        ++nGoodSelected;
        if (genCuts)
        {
            nGoodSelectedGenCuts+=1;
        }
        hMjjMatched->Fill((tlv_jet1+tlv_jet2).M());
        dPhiMatched->Fill(tlv_jet1.DeltaPhi(tlv_jet2));
    }

    // check total events that matched one jet
    int allMatches = 0;
    for (uint im1 = 0; im1 < matchedTo1.size(); ++im1)
    {
        for (uint im2 = 0; im2 < matchedTo2.size(); ++im2)
        {
            if (matchedTo1.at(im1) != matchedTo2.at(im2))
            {
                allMatches += 1;
            }
        }
    }
    if (allMatches > 0)
    {
        nAllJetsMatched += 1;
        if (genCuts)
        {
            nAllJetsMatchedGenCuts += 1;            
        }
    }

    ///// for debuggin and understanding
    if (genCuts && !(drA1B2 || drA2B1) && allMatches > 0 && nBtagMed >= 2)
    {
        cout << "--- debug of a non selected event " << iEv << endl;
        cout << endl;
        cout << "**** GEN INFO ****" << endl;
        cout << "Gen 1 : " << Form("pt = %.1f , eta = %.2f, phi = %.2f" , vgenb1.Pt(), vgenb1.Eta(), vgenb1.Phi()) << endl;
        cout << "Gen 2 : " << Form("pt = %.1f , eta = %.2f, phi = %.2f" , vgenb2.Pt(), vgenb2.Eta(), vgenb2.Phi()) << endl;
        cout << endl;
        cout << "**** JET INFO ****" << endl;
        for (uint ijet = 0; ijet< jets_and_btag.size(); ++ijet)
        {
            TLorentzVector tlv_deb
            (
                theBigTree.jets_px->at (jets_and_btag.at(ijet).second),
                theBigTree.jets_py->at (jets_and_btag.at(ijet).second),
                theBigTree.jets_pz->at (jets_and_btag.at(ijet).second),
                theBigTree.jets_e->at (jets_and_btag.at(ijet).second)
            );
            cout << "jet " << ijet << Form("pt = %.1f , eta = %.2f, phi = %.2f , CSV = %.3f" , tlv_deb.Pt(), tlv_deb.Eta(), tlv_deb.Phi(), jets_and_btag.at(ijet).first ) << endl;            
        }
    }
  }

  TFile* fOut = new TFile("fOut.root", "recreate");
  dPhiMatched->Write();
  hMjjMatched->Write();
  hMjjAll->Write();

  cout << "DONE" << endl;
  cout << "percentage good selected             : " << 100.*nGoodSelected/nTot << endl;
  cout << "percentage good selected w/ gen cuts : " << 100.*nGoodSelectedGenCuts/nTotGenCuts << endl;
  cout << "there was something good             : " << 100.*nAllJetsMatched/nTot << endl;
  cout << "there was something good w/ gen cuts : " << 100.*nAllJetsMatchedGenCuts/nTotGenCuts << endl;

}