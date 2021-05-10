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

//c++ -lm -o MakeMap MakeMap.cpp `root-config --glibs --cflags`

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
    // make tchain of all samples
    TString filename = "/home/llr/cms/motta/HHLegacy/CMSSW_11_1_0_pre6/src/KLUBAnalysis/inputFiles/Legacy2018_signals/GluGluToHHTo2B2Tau_LO_allNodes.txt"; 
    TChain* ch = new TChain ("HTauTauTree/HTauTauTree");
    appendFromFileList (ch, filename);

    ch->SetBranchStatus("*", 0);
    ch->SetBranchStatus("genpart_*", 1);

    vector<float>   *genpart_px = 0;
    vector<float>   *genpart_py = 0;
    vector<float>   *genpart_pz = 0;
    vector<float>   *genpart_e = 0;
    vector<int>     *genpart_pdg = 0;
    vector<int>     *genpart_status = 0;
    vector<int>     *genpart_HMothInd = 0;
    vector<int>     *genpart_MSSMHMothInd = 0;
    vector<int>     *genpart_TopMothInd = 0;
    vector<int>     *genpart_TauMothInd = 0;
    vector<int>     *genpart_ZMothInd = 0;
    vector<int>     *genpart_WMothInd = 0;
    vector<int>     *genpart_bMothInd = 0;
    vector<int>     *genpart_HZDecayMode = 0;
    vector<int>     *genpart_TopDecayMode = 0;
    vector<int>     *genpart_WDecayMode = 0;
    vector<int>     *genpart_TauGenDecayMode = 0;
    vector<int>     *genpart_flags = 0;

    
    ch->SetBranchAddress("genpart_px", &genpart_px);
    ch->SetBranchAddress("genpart_py", &genpart_py);
    ch->SetBranchAddress("genpart_pz", &genpart_pz);
    ch->SetBranchAddress("genpart_e", &genpart_e);
    ch->SetBranchAddress("genpart_pdg", &genpart_pdg);
    ch->SetBranchAddress("genpart_status", &genpart_status);
    ch->SetBranchAddress("genpart_HMothInd", &genpart_HMothInd);
    ch->SetBranchAddress("genpart_MSSMHMothInd", &genpart_MSSMHMothInd);
    ch->SetBranchAddress("genpart_TopMothInd", &genpart_TopMothInd);
    ch->SetBranchAddress("genpart_TauMothInd", &genpart_TauMothInd);
    ch->SetBranchAddress("genpart_ZMothInd", &genpart_ZMothInd);
    ch->SetBranchAddress("genpart_WMothInd", &genpart_WMothInd);
    ch->SetBranchAddress("genpart_bMothInd", &genpart_bMothInd);
    ch->SetBranchAddress("genpart_HZDecayMode", &genpart_HZDecayMode);
    ch->SetBranchAddress("genpart_TopDecayMode", &genpart_TopDecayMode);
    ch->SetBranchAddress("genpart_WDecayMode", &genpart_WDecayMode);
    ch->SetBranchAddress("genpart_TauGenDecayMode", &genpart_TauGenDecayMode);
    ch->SetBranchAddress("genpart_flags", &genpart_flags);

    // // must be same binning as Xanda!
    // TFile* fOut = new TFile ("outMap.root", "recreate");
    // TH2F* hMap = new TH2F ("allHHNodeMap", "allHHNodeMap", 90, 0, 1800, 10, -1, 1);
    // TH2F* hMapFolded = new TH2F ("allHHNodeMapFolded", "allHHNodeMapFolded", 90, 0, 1800, 5, 0, 1);
    // TH1F* hMap1D = new TH1F ("allHHNodeMap1D", "allHHNodeMap1D", 60, 0, 1800);

    // // new binning for dynamic reweight -- again these weird Xanda binnings
    // TFile* fOut = new TFile ("outMap_newbinning.root", "recreate");
    // double binning_mHH   [14] = {250.,270.,300.,330.,360.,390., 420.,450.,500.,550.,600.,700.,800.,1000.} ;
    // double binning_cth    [4] = {-1., -0.55,0.55,1. } ;
    // int nbins_mHH = 13; // size of arrays - 1
    // int nbins_cth = 3;  // size of arrays - 1
    // TH2F* hMap = new TH2F ("allHHNodeMap", "allHHNodeMap", nbins_mHH, binning_mHH, nbins_cth, binning_cth );
    // TH2F* hMapFolded = new TH2F ("allHHNodeMapFolded", "allHHNodeMapFolded", 90, 0, 1800, 5, 0, 1); // won't be used for reweight
    // TH1F* hMap1D = new TH1F ("allHHNodeMap1D", "allHHNodeMap1D", nbins_mHH, binning_mHH);

    TFile* fOut = new TFile ("allHHnodeMap_5DdiffRew_2018.root", "recreate");
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
    // TH2F* hMapFolded = new TH2F ("allHHNodeMapFolded", "allHHNodeMapFolded", 90, 0, 1800, 5, 0, 1); // won't be used for reweight
    // TH1F* hMap1D = new TH1F ("allHHNodeMap1D", "allHHNodeMap1D", nbins_mHH, binning_mHH);



    TLorentzVector vH1, vH2, vBoost, vSum;
    for (int iEv = 0; true; ++iEv)
    {
        int got = ch->GetEntry(iEv);
        if (got == 0) break;
        if (iEv % 100000 == 0) cout << "Event: " << iEv << endl;

        // loop on gen to find Higgs
        int idx1 = -1;
        int idx2 = -1;
        for (int igen = 0; igen < genpart_px->size(); igen++)
        {
            if (genpart_pdg->at(igen) == 25)
            {
                bool isFirst = CheckBit (genpart_flags->at(igen), 12) ; // 12 = isFirstCopy
                if (isFirst)
                {
                    if (idx1 >= 0 && idx2 >= 0)
                    {
                        cout << "** ERROR: more than 2 H identified " << endl;
                        continue;
                    }
                    (idx1 == -1) ? (idx1 = igen) : (idx2 = igen) ;
                }
            }
        }

        if (idx1 == -1 || idx2 == -1)
        {
            cout << "** ERROR: couldn't find 2 H" << endl;
            continue;
        }

        vH1.SetPxPyPzE (genpart_px->at(idx1), genpart_py->at(idx1), genpart_pz->at(idx1), genpart_e->at(idx1) );
        vH2.SetPxPyPzE (genpart_px->at(idx2), genpart_py->at(idx2), genpart_pz->at(idx2), genpart_e->at(idx2) );

        
        vSum = vH1+vH2;
        float mHH = vSum.M();

        vH1.Boost(-vSum.BoostVector());                     
        // vH2.Boost(-vSum.BoostVector());                     
        float ct1 = vH1.CosTheta();
        // float ct2 = vH2.CosTheta();
        // cout << ct1 << "  " << ct2 << endl;

/*
        // cout << vSum.M() << endl;

        // float ct1 = vH1.Pz() / TMath::Sqrt(vH1.Pt()*vH1.Pt() + vH1.Pz()*vH1.Pz());
        // float ct2 = vH2.Pz() / TMath::Sqrt(vH2.Pt()*vH2.Pt() + vH2.Pz()*vH2.Pz());
        // cout <<  ct1 << " " << ct2 << endl;

        float pzBoost = 0.5*(vH1.Pz() + vH2.Pz());
        //vBoost.SetPxPyPzE (0, 0, pzBoost, pzBoost);
        vH1.SetPxPyPzE (vH1.Px(), vH1.Py(), vH1.Pz() - pzBoost, vH1.E());
        vH2.SetPxPyPzE (vH2.Px(), vH2.Py(), vH2.Pz() - pzBoost, vH2.E());

        // vSum = vH1+vH2;
        // cout << "  --> " << vSum.M() << endl;
        
        float ct1 = vH1.Pz() / TMath::Sqrt(vH1.Pt()*vH1.Pt() + vH1.Pz()*vH1.Pz());
        float ct2 = vH2.Pz() / TMath::Sqrt(vH2.Pt()*vH2.Pt() + vH2.Pz()*vH2.Pz());
*/        
        // hMapFolded->Fill(mHH, TMath::Abs(ct1));
        // hMap->Fill(mHH, ct1);
        hMap->Fill(mHH, TMath::Abs(ct1));
        // hMap1D->Fill(mHH);

        // cout <<  " --> " << ct1 << " " << ct2 << endl;
        // if (TMath::Abs(ct1 + ct2) > 0.0001)cout <<  " --> " << ct1 << " " << ct2 << endl;
        // 

        // cout << vH1.Pt() << " " << vH2.Pt() << endl;
    }

    fOut->cd();
    hMap->Write();
    // hMapFolded->Write();
    // hMap1D->Write();

}
