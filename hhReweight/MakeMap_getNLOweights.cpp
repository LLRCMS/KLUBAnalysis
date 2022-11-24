#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TString.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TDirectory.h"
#include "TLorentzVector.h"
#include <map>
#include <fstream>

using namespace std;

//c++ -lm -o MakeMap_getNLOweights MakeMap_getNLOweights.cpp `root-config --glibs --cflags`

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

bool CheckBit (int number, int bitpos)
{
    bool res = number & (1 << bitpos);
    return res;
}

int main ()
{
    // Choose the year to be used for computing the NLO weights (input to MakeMap_NLO.cpp)

    // 2016 input files
    //TString filename0    = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2016_signals/GluGluToHHTo2B2Tau_NLO_kl0.txt";
    //TString filename1    = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2016_signals/GluGluToHHTo2B2Tau_NLO_kl1.txt";
    //TString filename2p45 = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2016_signals/GluGluToHHTo2B2Tau_NLO_kl2p45.txt";
    //TString filename5    = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2016_signals/GluGluToHHTo2B2Tau_NLO_kl5.txt";

    // 2017 input files
    //TString filename0    = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2017_signals/GluGluToHHTo2B2Tau_node_cHHH0.txt";
    //TString filename1    = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2017_signals/GluGluToHHTo2B2Tau_node_cHHH1.txt";
    //TString filename2p45 = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2017_signals/GluGluToHHTo2B2Tau_node_cHHH2p45.txt";
    //TString filename5    = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2017_signals/GluGluToHHTo2B2Tau_node_cHHH5.txt";

    // 2018 input files
    TString filename0    = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2018_signalsMIB/GluGluToHHTo2B2Tau_NLO_kl0.txt";
    TString filename1    = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2018_signalsMIB/GluGluToHHTo2B2Tau_NLO_kl1.txt";
    TString filename2p45 = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2018_signalsMIB/GluGluToHHTo2B2Tau_NLO_kl2p45.txt";
    TString filename5    = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2018_signalsMIB/GluGluToHHTo2B2Tau_NLO_kl5.txt";

    TChain* ch0    = new TChain ("HTauTauTree/HTauTauTree");
    TChain* ch1    = new TChain ("HTauTauTree/HTauTauTree");
    TChain* ch2p45 = new TChain ("HTauTauTree/HTauTauTree");
    TChain* ch5    = new TChain ("HTauTauTree/HTauTauTree");

    appendFromFileList (ch0   , filename0);
    appendFromFileList (ch1   , filename1);
    appendFromFileList (ch2p45, filename2p45);
    appendFromFileList (ch5   , filename5);

    ch0   ->SetBranchStatus("*", 0);
    ch1   ->SetBranchStatus("*", 0);
    ch2p45->SetBranchStatus("*", 0);
    ch5   ->SetBranchStatus("*", 0);

    ch0   ->SetBranchStatus("aMCatNLOweight", 1);
    ch1   ->SetBranchStatus("aMCatNLOweight", 1);
    ch2p45->SetBranchStatus("aMCatNLOweight", 1);
    ch5   ->SetBranchStatus("aMCatNLOweight", 1);

    float aMCatNLOweight0    = 1;
    float aMCatNLOweight1    = 1;
    float aMCatNLOweight2p45 = 1;
    float aMCatNLOweight5    = 1;

    ch0   ->SetBranchAddress("aMCatNLOweight", &aMCatNLOweight0);
    ch1   ->SetBranchAddress("aMCatNLOweight", &aMCatNLOweight1);
    ch2p45->SetBranchAddress("aMCatNLOweight", &aMCatNLOweight2p45);
    ch5   ->SetBranchAddress("aMCatNLOweight", &aMCatNLOweight5);

    // ---- Get full yield kl=0
    double total_yield_0 = 0.;
    cout << "--- Getting full weight 0 ---" << endl;
    for (int iEv = 0; true; ++iEv)
    {
        int got = ch0->GetEntry(iEv);
        if (got == 0) break;
        if ( abs(aMCatNLOweight0) > 0.5 ) continue;
        if (iEv % 100000 == 0) cout << "Event: " << iEv << endl;   
        if (iEv % 50000 == 0) cout << "  aMCatNLOweight: " << aMCatNLOweight0 << endl;

        total_yield_0 += aMCatNLOweight0;
    }
    cout << "   total_yield_0 : " << total_yield_0 << endl;

    // ---- Get full yield kl=1
    double total_yield_1 = 0.;
    cout << "--- Getting full weight 1 ---" << endl;
    for (int iEv = 0; true; ++iEv)
    {
        int got = ch1->GetEntry(iEv);
        if (got == 0) break;
        if ( abs(aMCatNLOweight1) > 0.5 ) continue;
        if (iEv % 100000 == 0) cout << "Event: " << iEv << endl;   
        if (iEv % 50000 == 0) cout << "  aMCatNLOweight: " << aMCatNLOweight1 << endl;

        total_yield_1 += aMCatNLOweight1;
    }
    cout << "   total_yield_1 : " << total_yield_1 << endl;

    // ---- Get full yield kl=2p45
    double total_yield_2p45 = 0.;
    cout << "--- Getting full weight 2p45 ---" << endl;
    for (int iEv = 0; true; ++iEv)
    {
        int got = ch2p45->GetEntry(iEv);
        if (got == 0) break;
        if ( abs(aMCatNLOweight2p45) > 0.5 ) continue;
        if (iEv % 100000 == 0) cout << "Event: " << iEv << endl;   
        if (iEv % 50000 == 0) cout << "  aMCatNLOweight: " << aMCatNLOweight2p45 << endl;

        total_yield_2p45 += aMCatNLOweight2p45;
    }
    cout << "   total_yield_2p45 : " << total_yield_2p45 << endl;

    // ---- Get full yield kl=5
    double total_yield_5 = 0.;
    cout << "--- Getting full weight 5 ---" << endl;
    for (int iEv = 0; true; ++iEv)
    {
        int got = ch5->GetEntry(iEv);
        if (got == 0) break;
        if ( abs(aMCatNLOweight5) > 0.5 ) continue;
        if (iEv % 100000 == 0) cout << "Event: " << iEv << endl;   
        if (iEv % 50000 == 0) cout << "  aMCatNLOweight: " << aMCatNLOweight5 << endl;

        total_yield_5 += aMCatNLOweight5;
    }
    cout << "   total_yield_5 : " << total_yield_5 << endl;
}
