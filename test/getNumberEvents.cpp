
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TBranch.h"
#include "TString.h"
#include "TLorentzVector.h"
#include <assert.h>

// bigTree is produced on an existing ntuple as follows (see at the end of the file) 
#include "bigTree.h" 
#include "smallTree.h"
#include "OfflineProducerHelper.h"
#include "PUReweight.h"
#include "triggerReader.h"
#include "bJetRegrVars.h"
#include "bTagSF.h"

using namespace std ;

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
int totEvtsFromFileHisto (TString filename, int nFiles)
{
    std::ifstream infile(filename.Data());
    std::string line;
    int nEvts = 0;

    int done = 0;

    while (std::getline(infile, line))
    {
        line = line.substr(0, line.find("#", 0)); // remove comments introduced by #
        while (line.find(" ") != std::string::npos) line = line.erase(line.find(" "), 1); // remove white spaces
        while (line.find("\n") != std::string::npos) line = line.erase(line.find("\n"), 1); // remove new line characters
        while (line.find("\r") != std::string::npos) line = line.erase(line.find("\r"), 1); // remove carriage return characters
        if (!line.empty()) // skip empty lines
        {
            // cout << line << endl;
            int barWidth = 70;
            float progress = 1.*done/nFiles;
            std::cout << "[";
            int pos = barWidth * progress;
            for (int i = 0; i < barWidth; ++i) {
                if (i < pos) std::cout << "=";
                else if (i == pos) std::cout << ">";
                else std::cout << " ";
            }
            std::cout << "] " << int(progress * 100.0) << " % (" << nFiles << " files)\r";
            std::cout.flush();
	    TFile* fIn = TFile::Open (line.c_str());
            TH1F* dummy = (TH1F*) fIn->Get ("HTauTauTree/Counters");
            nEvts += (int) dummy->GetBinContent(1) ;
            fIn->Close();
            done += 1;
        }
    }
    cout << endl;
    cout.flush();
    return nEvts;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// count number of files in filelist
int getNumOfFiles (TString filename)
{
    std::ifstream infile(filename.Data());
    std::string line;
    int nFiles = 0;
    while (std::getline(infile, line))
    {
        line = line.substr(0, line.find("#", 0)); // remove comments introduced by #
        while (line.find(" ") != std::string::npos) line = line.erase(line.find(" "), 1); // remove white spaces
        while (line.find("\n") != std::string::npos) line = line.erase(line.find("\n"), 1); // remove new line characters
        while (line.find("\r") != std::string::npos) line = line.erase(line.find("\r"), 1); // remove carriage return characters
        if (!line.empty()) // skip empty lines
        {
            nFiles += 1;
        }
    }
    return nFiles;
}


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cout << "** error of inputs . Usage:" << endl;
        cout << "getNumberEvents <inputFileList>" << endl;
        return 1;
    }

    string inputFile = argv[1];
    cout << "** INFO: using filelist : " << inputFile << endl;

    int nFiles = getNumOfFiles(inputFile);
    int TOTfromHisto = totEvtsFromFileHisto(inputFile, nFiles); // this is O(10) faster than looping over all tree entries

    // int TOTfromHisto = 0;

    // TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;
    // appendFromFileList (bigChain, inputFile);
    // bigTree theBigTree (bigChain) ;

    // theBigTree.fChain->SetBranchStatus("*", 0);
    // theBigTree.fChain->SetBranchStatus("EventNumber", 1); // you need at least one brancha activated to have getEntry() != 0

    // for (Long64_t iEvent = 0 ; true ; ++iEvent) 
    // {
    //     if (iEvent % 10000 == 0)  cout << "- reading event " << iEvent << endl ;
    //     int got = theBigTree.fChain->GetEntry(iEvent);
    //     if (got == 0) break;
    //     totEvents+=1;
    // }

    cout << "TOT: " << TOTfromHisto << endl;

}
