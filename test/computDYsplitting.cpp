// run over the inclusive DY sample and get the fraction of events with 0,1,2,3, >=4 jets and b, bb

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH2I.h"
#include "TFile.h"
#include "TBranch.h"
#include "TString.h"
#include "TLorentzVector.h"

// bigTree is produced on an existing ntuple as follows (see at the end of the file) 
#include "bigTree.h" 
#include "smallTree.h"
#include "OfflineProducerHelper.h"
#include "PUReweight.h"
#include "triggerReader.h"
#include "bJetRegrVars.h"
#include "bTagSF.h"

using namespace std ;

#define DEBUG false

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

bool CheckBit (int number, int bitpos)
{
    bool res = number & (1 << bitpos);
    return res;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
string printFlag (int flag, int max = 16)
{
    string s = "";
    for (int i = max; i >=0; i--)
    {
        if (CheckBit (flag, i)) s+="1";
        else s+="0";
    }
    return s;
}

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

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cout << "** error of inputs . Usage:" << endl;
        // cout << "computeDYsplitting <inputFileList> <OPTIONAL : DY LO section>" << endl;
        cout << "computeDYsplitting <inputFileList> <OPTIONAL : outputFileName>" << endl;
        return 1;
    }

    string inputFile = argv[1];
    string outputFileName = "histos_DYCount.root" ;
    if (argc > 2) outputFileName = argv[2];
    // float DY_lo_XS = 4954.0 ; // pb
    // if (argc > 2) DY_lo_XS = atof(argv[2]);

    cout << "** INFO: using filelist : " << inputFile << endl;
    // cout << "** INFO: DY LO cross section : " << DY_lo_XS << endl;

    // input and output setup
    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;
    appendFromFileList (bigChain, inputFile);
    bigTree theBigTree (bigChain) ;

    int totEvents = 0;
    int EvtsNjets[5] = {0,0,0,0,0}; // 0, 1, 2, 3, >=4
    int EvtsNBs[5] = {0,0,0,0,0}; //0,1,>=2 b
    int allEvts[5][5]; // njet, nb
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            allEvts[i][j] = 0;


    TFile* fOut = new TFile (outputFileName.c_str(), "recreate");
    TH2F* h_nJets_nBs = new TH2F ("h_nJets_nBs", ";nJets;nBs", 5, 0, 5, 5, 0, 5);
    TH2I* hINT_nJets_nBs = new TH2I ("hINT_nJets_nBs", ";nJets;nBs", 5, 0, 5, 5, 0, 5);

    // speed up
    theBigTree.fChain->SetBranchStatus("*", 0);
    // theBigTree.fChain->SetBranchStatus("genpart_*", 1);
    theBigTree.fChain->SetBranchStatus("lheNOutPartons", 1);
    theBigTree.fChain->SetBranchStatus("lheNOutB", 1);
    // theBigTree.fChain->SetBranchStatus("lheHt", 1);

    cout << "** INFO: most branches are desactivated to speed up reading..." << endl;
    // loop over events
    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    for (Long64_t iEvent = 0 ; true ; ++iEvent) 
    {
        if (iEvent % 10000 == 0)  cout << "- reading event " << iEvent << endl ;

        if (DEBUG) cout << "====== new event ====== " << iEvent << endl ;

        int got = theBigTree.fChain->GetEntry(iEvent);
        if (got == 0) break;
        // if (iEvent > 1000000) break;

        ++totEvents ;

        int npartons = theBigTree.lheNOutPartons ;
        if (npartons > 4)
        {
            cout << " --> ev : " << iEvent << " npartons is " << npartons << " lowered to 4" << endl;
            npartons = 4;
        }
        
        if (DEBUG) cout << "n partons: " << theBigTree.lheNOutPartons << endl;
        EvtsNjets[npartons] += 1;

        // ////// FIXME: to replace ////////// // will use nOutBs
        // int nbs = 0;
        // for (uint igen = 0; igen < theBigTree.genpart_px->size(); ++igen)
        // {
        //     if (abs(theBigTree.genpart_pdg->at(igen)) != 5 ) continue;
        //     // if (fl.isHardProcess())             flags |= (1 << 7);
        //     // if (fl.fromHardProcess()) flags |= (1 << 8);
        //     // if (fl.isFirstCopy()) flags |= (1 << 12);
        //     // if (!CheckBit(theBigTree.genpart_flags->at(igen) , 8) || !!CheckBit(theBigTree.genpart_flags->at(igen) , 13) ) continue;
        //     if (!CheckBit(theBigTree.genpart_flags->at(igen) , 7) ) continue ;
        //     if (DEBUG) cout << igen << " --> " << printFlag(theBigTree.genpart_flags->at(igen)) << " " << theBigTree.genpart_pdg->at(igen) << endl;
        //     ++nbs ;
        // }

        int nbs = theBigTree.lheNOutB;        
        if (nbs > 4)
        {
            cout << " --> ev : " << iEvent << " nbs is " << nbs << " lowered to 4" << endl;
            nbs = 4;
        }
        ////// END OF FIXME: to replace //////////
        
        if (DEBUG) cout << "NBs = " << nbs << endl;
        EvtsNBs[nbs] +=1 ;

        h_nJets_nBs->Fill (npartons, nbs);
        hINT_nJets_nBs->Fill (npartons, nbs);
        allEvts[npartons][nbs] += 1;
    }

    fOut->cd();
    h_nJets_nBs -> Write();
    hINT_nJets_nBs -> Write();
    fOut->Close();

    for (int nj = 0; nj < 5; nj++)
    {
        cout << "NJET: " << nj << endl;
        for (int nb = 0; nb < 5; nb++)
        {
            cout << allEvts[nj][nb] << " ";
        }
        cout << endl;
    }
}
