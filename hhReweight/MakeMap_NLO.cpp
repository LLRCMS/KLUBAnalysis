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

//c++ -lm -o MakeMap_NLO MakeMap_NLO.cpp `root-config --glibs --cflags`

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
    // TString filename0    = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2018_signalsMIB/GluGluToHHTo2B2Tau_NLO_kl0.txt";
    // TString filename1    = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2018_signalsMIB/GluGluToHHTo2B2Tau_NLO_kl1.txt";
    // TString filename2p45 = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2018_signalsMIB/GluGluToHHTo2B2Tau_NLO_kl2p45.txt";
    // TString filename5    = "/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2018_signalsMIB/GluGluToHHTo2B2Tau_NLO_kl5.txt";
    TString filename0    = "/home/llr/cms/motta/HHLegacy/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2018_signals/1_GluGluToHHTo2B2Tau_node_cHHH0_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt";
    TString filename1    = "/home/llr/cms/motta/HHLegacy/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2018_signals/2_GluGluToHHTo2B2Tau_node_cHHH1_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt";
    TString filename2p45 = "/home/llr/cms/motta/HHLegacy/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2018_signals/3_GluGluToHHTo2B2Tau_node_cHHH2p45_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt";
    TString filename5    = "/home/llr/cms/motta/HHLegacy/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2018_signals/4_GluGluToHHTo2B2Tau_node_cHHH5_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt";

    // Uncomments the weights for the considered year (obtained from MakeMap_getNLOweights.cpp)
    // Tese should correspond more or less to the xs predicted by POWHEG
    // - 2016 -
    //double total_yield0    = 56931.4;
    //double total_yield1    = 26732.6;
    //double total_yield2p45 = 10752.6;
    //double total_yield5    = 77709.6;
    // Total: 172126.2 (just for debugging)
    // - 2017 -
    //double total_yield0    = 58456.4;
    //double total_yield1    = 26052.8;
    //double total_yield2p45 = 10948.3;
    //double total_yield5    = 76938.6;
    // Total: 172396.1 (just for debugging)
    // - 2018 -
    double total_yield0    = 60219.4;
    double total_yield1    = 26753.1;
    double total_yield2p45 = 11309.5;
    double total_yield5    = 78947.3;
    // Total: 177229.3 (just for debugging)

    // Define output file
    //TFile* fOut = new TFile ("allHHnodeMap_NLO_5DdiffRew_2016.root", "recreate");
    //TFile* fOut = new TFile ("allHHnodeMap_NLO_5DdiffRew_2017.root", "recreate");
    TFile* fOut = new TFile ("allHHnodeMap_NLO_5DdiffRew_2018.root", "recreate");

    // Define TChains
    TChain* ch0    = new TChain ("HTauTauTree/HTauTauTree");
    TChain* ch1    = new TChain ("HTauTauTree/HTauTauTree");
    TChain* ch2p45 = new TChain ("HTauTauTree/HTauTauTree");
    TChain* ch5    = new TChain ("HTauTauTree/HTauTauTree");

    // Append files to TChains
    appendFromFileList (ch0   , filename0);
    appendFromFileList (ch1   , filename1);
    appendFromFileList (ch2p45, filename2p45);
    appendFromFileList (ch5   , filename5);

    // Disable all branches
    ch0   ->SetBranchStatus("*", 0);
    ch1   ->SetBranchStatus("*", 0);
    ch2p45->SetBranchStatus("*", 0);
    ch5   ->SetBranchStatus("*", 0);

    // Activate only needed branches
    ch0   ->SetBranchStatus("aMCatNLOweight", 1);
    ch0   ->SetBranchStatus("genpart_*"     , 1);
    ch1   ->SetBranchStatus("aMCatNLOweight", 1);
    ch1   ->SetBranchStatus("genpart_*"     , 1);
    ch2p45->SetBranchStatus("aMCatNLOweight", 1);
    ch2p45->SetBranchStatus("genpart_*"     , 1);
    ch5   ->SetBranchStatus("aMCatNLOweight", 1);
    ch5   ->SetBranchStatus("genpart_*"     , 1);

    // Define needed variables
    float          aMCatNLOweight0 = 1;
    vector<float>  *genpart_px0    = 0;
    vector<float>  *genpart_py0    = 0;
    vector<float>  *genpart_pz0    = 0;
    vector<float>  *genpart_e0     = 0;
    vector<int>    *genpart_pdg0   = 0;
    vector<int>    *genpart_flags0 = 0;

    float          aMCatNLOweight1 = 1;
    vector<float>  *genpart_px1    = 0;
    vector<float>  *genpart_py1    = 0;
    vector<float>  *genpart_pz1    = 0;
    vector<float>  *genpart_e1     = 0;
    vector<int>    *genpart_pdg1   = 0;
    vector<int>    *genpart_flags1 = 0;

    float          aMCatNLOweight2p45 = 1;
    vector<float>  *genpart_px2p45    = 0;
    vector<float>  *genpart_py2p45    = 0;
    vector<float>  *genpart_pz2p45    = 0;
    vector<float>  *genpart_e2p45     = 0;
    vector<int>    *genpart_pdg2p45   = 0;
    vector<int>    *genpart_flags2p45 = 0;

    float          aMCatNLOweight5 = 1;
    vector<float>  *genpart_px5    = 0;
    vector<float>  *genpart_py5    = 0;
    vector<float>  *genpart_pz5    = 0;
    vector<float>  *genpart_e5     = 0;
    vector<int>    *genpart_pdg5   = 0;
    vector<int>    *genpart_flags5 = 0;

    // Set branch addesses for all TChains
    ch0   ->SetBranchAddress("aMCatNLOweight", &aMCatNLOweight0);
    ch0   ->SetBranchAddress("genpart_px"    , &genpart_px0);
    ch0   ->SetBranchAddress("genpart_py"    , &genpart_py0);
    ch0   ->SetBranchAddress("genpart_pz"    , &genpart_pz0);
    ch0   ->SetBranchAddress("genpart_e"     , &genpart_e0);
    ch0   ->SetBranchAddress("genpart_pdg"   , &genpart_pdg0);
    ch0   ->SetBranchAddress("genpart_flags" , &genpart_flags0);

    ch1   ->SetBranchAddress("aMCatNLOweight", &aMCatNLOweight1);
    ch1   ->SetBranchAddress("genpart_px"    , &genpart_px1);
    ch1   ->SetBranchAddress("genpart_py"    , &genpart_py1);
    ch1   ->SetBranchAddress("genpart_pz"    , &genpart_pz1);
    ch1   ->SetBranchAddress("genpart_e"     , &genpart_e1);
    ch1   ->SetBranchAddress("genpart_pdg"   , &genpart_pdg1);
    ch1   ->SetBranchAddress("genpart_flags" , &genpart_flags1);

    ch2p45->SetBranchAddress("aMCatNLOweight", &aMCatNLOweight2p45);
    ch2p45->SetBranchAddress("genpart_px"    , &genpart_px2p45);
    ch2p45->SetBranchAddress("genpart_py"    , &genpart_py2p45);
    ch2p45->SetBranchAddress("genpart_pz"    , &genpart_pz2p45);
    ch2p45->SetBranchAddress("genpart_e"     , &genpart_e2p45);
    ch2p45->SetBranchAddress("genpart_pdg"   , &genpart_pdg2p45);
    ch2p45->SetBranchAddress("genpart_flags" , &genpart_flags2p45);

    ch5   ->SetBranchAddress("aMCatNLOweight", &aMCatNLOweight5);
    ch5   ->SetBranchAddress("genpart_px"    , &genpart_px5);
    ch5   ->SetBranchAddress("genpart_py"    , &genpart_py5);
    ch5   ->SetBranchAddress("genpart_pz"    , &genpart_pz5);
    ch5   ->SetBranchAddress("genpart_e"     , &genpart_e5);
    ch5   ->SetBranchAddress("genpart_pdg"   , &genpart_pdg5);
    ch5   ->SetBranchAddress("genpart_flags" , &genpart_flags5);

    // Define 2D Map
    double binning_mHH [37] = {250.,270.,290.,310.,330.,
                               350.,370.,390.,410.,430., 
                               450.,470.,490.,510.,530.,
                               550.,570.,590.,610.,630.,
                               650.,670.,700.,750.,800.,
                               850.,900.,950.,1000.,1100.,
                               1200.,1300.,1400.,1500.,1750.,2000.,5000.};
    double binning_cth [5]  = {0.0, 0.4, 0.6, 0.8, 1.0} ;
    int nbins_mHH = 36; // size of arrays - 1
    int nbins_cth = 4;  // size of arrays - 1
    TH2F* hMap = new TH2F ("allHHNodeMap", "allHHNodeMap", nbins_mHH, binning_mHH, nbins_cth, binning_cth );

    // Define utils TLorentzVectors
    TLorentzVector vH1, vH2, vBoost, vSum;

    // Fill the 2D Map looping separately on the different samples,
    // because each sample must be weighted for its total weight

    // Loop on kl = 0 sample
    cout << "---Filling 2D map for kl = 0 ---" << endl;
    for (int iEv = 0; true; ++iEv)
    {
        // Get entry
        int got = ch0->GetEntry(iEv);
        if (got == 0) break;

        // Skip events with weight larger than 0.5 (due to bug in MC production)
        if ( abs(aMCatNLOweight0) > 0.5 ) continue;

        // Logging
        if (iEv % 100000 == 0) cout << "Event: " << iEv << endl;

        // Loop on gen parts to find Higgs
        int idx1 = -1;
        int idx2 = -1;
        for (int igen = 0; igen < genpart_px0->size(); igen++)
        {
            if (genpart_pdg0->at(igen) == 25)
            {
                bool isFirst = CheckBit (genpart_flags0->at(igen), 12) ; // 12 = isFirstCopy
                if (isFirst)
                {
                    if (idx1 >= 0 && idx2 >= 0)
                    {
                        cout << "** ERROR: more than 2 H identified " << endl;
                        break;
                    }
                    (idx1 == -1) ? (idx1 = igen) : (idx2 = igen) ;
                }
            }
        }

        if (idx1 == -1 || idx2 == -1)
        {
            cout << "** ERROR: couldn't find 2 H" << endl;
            break;
        }

        // Define TLorenztVecotrs for the gen Higgs
        vH1.SetPxPyPzE (genpart_px0->at(idx1), genpart_py0->at(idx1), genpart_pz0->at(idx1), genpart_e0->at(idx1) );
        vH2.SetPxPyPzE (genpart_px0->at(idx2), genpart_py0->at(idx2), genpart_pz0->at(idx2), genpart_e0->at(idx2) );

        // Compute gen m_HH
        vSum = vH1+vH2;
        float mHH = vSum.M();

        // Boost HH system and get cos(theta)
        vH1.Boost(-vSum.BoostVector());
        float ct1 = vH1.CosTheta();

        // Fill map with weight
        double NLOweight = aMCatNLOweight0 / total_yield0;
        if (iEv % 100000 == 0) cout << "  aMCatNLOweight: " << aMCatNLOweight0 << " total_yield: " << total_yield0 << " NLOweight: " << NLOweight << endl;
        hMap->Fill(mHH, TMath::Abs(ct1), NLOweight);
    }
    // Loop on kl = 1 sample
    cout << "---Filling 2D map for kl = 1 ---" << endl;
    for (int iEv = 0; true; ++iEv)
    {
        // Get entry
        int got = ch1->GetEntry(iEv);
        if (got == 0) break;

        // Skip events with weight larger than 0.5 (due to bug in MC production)
        if ( abs(aMCatNLOweight1) > 0.5 ) continue;

        // Logging
        if (iEv % 100000 == 0) cout << "Event: " << iEv << endl;

        // Loop on gen parts to find Higgs
        int idx1 = -1;
        int idx2 = -1;
        for (int igen = 0; igen < genpart_px1->size(); igen++)
        {
            if (genpart_pdg1->at(igen) == 25)
            {
                bool isFirst = CheckBit (genpart_flags1->at(igen), 12) ; // 12 = isFirstCopy
                if (isFirst)
                {
                    if (idx1 >= 0 && idx2 >= 0)
                    {
                        cout << "** ERROR: more than 2 H identified " << endl;
                        break;
                    }
                    (idx1 == -1) ? (idx1 = igen) : (idx2 = igen) ;
                }
            }
        }

        if (idx1 == -1 || idx2 == -1)
        {
            cout << "** ERROR: couldn't find 2 H" << endl;
            break;
        }

        // Define TLorenztVecotrs for the gen Higgs
        vH1.SetPxPyPzE (genpart_px1->at(idx1), genpart_py1->at(idx1), genpart_pz1->at(idx1), genpart_e1->at(idx1) );
        vH2.SetPxPyPzE (genpart_px1->at(idx2), genpart_py1->at(idx2), genpart_pz1->at(idx2), genpart_e1->at(idx2) );

        // Compute gen m_HH
        vSum = vH1+vH2;
        float mHH = vSum.M();

        // Boost HH system and get cos(theta)
        vH1.Boost(-vSum.BoostVector());
        float ct1 = vH1.CosTheta();

        // Fill map with weight
        double NLOweight = aMCatNLOweight1 / total_yield1;
        if (iEv % 100000 == 0) cout << "  aMCatNLOweight: " << aMCatNLOweight1 << " total_yield: " << total_yield1 << " NLOweight: " << NLOweight << endl;
        hMap->Fill(mHH, TMath::Abs(ct1), NLOweight);
    }
    // Loop on kl = 2.45 sample
    cout << "---Filling 2D map for kl = 2.45 ---" << endl;
    for (int iEv = 0; true; ++iEv)
    {
        // Get entry
        int got = ch2p45->GetEntry(iEv);
        if (got == 0) break;

        // Skip events with weight larger than 0.5 (due to bug in MC production)
        if ( abs(aMCatNLOweight2p45) > 0.5 ) continue;

        // Logging
        if (iEv % 100000 == 0) cout << "Event: " << iEv << endl;

        // Loop on gen parts to find Higgs
        int idx1 = -1;
        int idx2 = -1;
        for (int igen = 0; igen < genpart_px2p45->size(); igen++)
        {
            if (genpart_pdg2p45->at(igen) == 25)
            {
                bool isFirst = CheckBit (genpart_flags2p45->at(igen), 12) ; // 12 = isFirstCopy
                if (isFirst)
                {
                    if (idx1 >= 0 && idx2 >= 0)
                    {
                        cout << "** ERROR: more than 2 H identified " << endl;
                        break;
                    }
                    (idx1 == -1) ? (idx1 = igen) : (idx2 = igen) ;
                }
            }
        }

        if (idx1 == -1 || idx2 == -1)
        {
            cout << "** ERROR: couldn't find 2 H" << endl;
            break;
        }

        // Define TLorenztVecotrs for the gen Higgs
        vH1.SetPxPyPzE (genpart_px2p45->at(idx1), genpart_py2p45->at(idx1), genpart_pz2p45->at(idx1), genpart_e2p45->at(idx1) );
        vH2.SetPxPyPzE (genpart_px2p45->at(idx2), genpart_py2p45->at(idx2), genpart_pz2p45->at(idx2), genpart_e2p45->at(idx2) );

        // Compute gen m_HH
        vSum = vH1+vH2;
        float mHH = vSum.M();

        // Boost HH system and get cos(theta)
        vH1.Boost(-vSum.BoostVector());
        float ct1 = vH1.CosTheta();

        // Fill map with weight
        double NLOweight = aMCatNLOweight2p45 / total_yield2p45;
        if (iEv % 100000 == 0) cout << "  aMCatNLOweight: " << aMCatNLOweight2p45 << " total_yield: " << total_yield2p45 << " NLOweight: " << NLOweight << endl;
        hMap->Fill(mHH, TMath::Abs(ct1), NLOweight);
    }

    // Loop on kl = 5 sample
    cout << "---Filling 2D map for kl = 5 ---" << endl;
    for (int iEv = 0; true; ++iEv)
    {
        // Get entry
        int got = ch5->GetEntry(iEv);
        if (got == 0) break;

        // Skip events with weight larger than 0.5 (due to bug in MC production)
        if ( abs(aMCatNLOweight5) > 0.5 ) continue;

        // Logging
        if (iEv % 100000 == 0) cout << "Event: " << iEv << endl;

        // Loop on gen parts to find Higgs
        int idx1 = -1;
        int idx2 = -1;
        for (int igen = 0; igen < genpart_px5->size(); igen++)
        {
            if (genpart_pdg5->at(igen) == 25)
            {
                bool isFirst = CheckBit (genpart_flags5->at(igen), 12) ; // 12 = isFirstCopy
                if (isFirst)
                {
                    if (idx1 >= 0 && idx2 >= 0)
                    {
                        cout << "** ERROR: more than 2 H identified " << endl;
                        break;
                    }
                    (idx1 == -1) ? (idx1 = igen) : (idx2 = igen) ;
                }
            }
        }

        if (idx1 == -1 || idx2 == -1)
        {
            cout << "** ERROR: couldn't find 2 H" << endl;
            break;
        }

        // Define TLorenztVecotrs for the gen Higgs
        vH1.SetPxPyPzE (genpart_px5->at(idx1), genpart_py5->at(idx1), genpart_pz5->at(idx1), genpart_e5->at(idx1) );
        vH2.SetPxPyPzE (genpart_px5->at(idx2), genpart_py5->at(idx2), genpart_pz5->at(idx2), genpart_e5->at(idx2) );

        // Compute gen m_HH
        vSum = vH1+vH2;
        float mHH = vSum.M();

        // Boost HH system and get cos(theta)
        vH1.Boost(-vSum.BoostVector());
        float ct1 = vH1.CosTheta();

        // Fill map with weight
        double NLOweight = aMCatNLOweight5 / total_yield5;
        if (iEv % 100000 == 0) cout << "  aMCatNLOweight: " << aMCatNLOweight5 << " total_yield: " << total_yield5 << " NLOweight: " << NLOweight << endl;
        hMap->Fill(mHH, TMath::Abs(ct1), NLOweight);
    }

    // Write 2D Map to output file
    fOut->cd();
    hMap->Write();
}
