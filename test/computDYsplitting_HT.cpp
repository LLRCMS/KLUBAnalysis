// run over the inclusive DY sample and get the fraction of events with 0,1,2,3, >=4 jets and b, bb

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "TTree.h"
#include "TH1F.h"
#include "TH3F.h"
#include "TH3I.h"
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
        cout << "computeDYsplitting <inputFileList> <OPTIONAL : outputFileName>" << endl;
        return 1;
    }

    string inputFile = argv[1];
    string outputFileName = "histos_DYCount.root" ;
    if (argc > 2) outputFileName = argv[2];

    cout << "** INFO: using filelist : " << inputFile << endl;

    // input and output setup
    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;
    appendFromFileList (bigChain, inputFile);
    bigTree theBigTree (bigChain) ;

    int totEvents = 0;
    int allEvts[5][5][9]; // njet,nb,ht
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            for (int k = 0; k < 9; k++)
                allEvts[i][j][k] = 0;
		
    float xbins[6]  = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0};
    float ybins[6]  = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0};
    float zbins[10] = {0.0,70.0,100.0,200.0,400.0,600.0,800.0,1200.0,2500.0,10000.0};
	    
    TFile* fOut = new TFile (outputFileName.c_str(), "recreate");
    TH3F* h_nJets_nBs_ht    = new TH3F ("h_nJets_nBs_ht"   , ";nJets;nBs;ht", 5, xbins, 5, ybins, 9, zbins);
    TH3I* hINT_nJets_nBs_ht = new TH3I ("hINT_nJets_nBs_ht", ";nJets;nBs;ht", 5, xbins, 5, ybins, 9, zbins);

    // speed up
    theBigTree.fChain->SetBranchStatus("*", 0);
    theBigTree.fChain->SetBranchStatus("lheNOutPartons", 1);
    theBigTree.fChain->SetBranchStatus("lheNOutB", 1);
    theBigTree.fChain->SetBranchStatus("lheHt", 1);

    cout << "** INFO: most branches are desactivated to speed up reading..." << endl;
    // loop over events
    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    for (Long64_t iEvent = 0 ; true ; ++iEvent) 
    {
        if (iEvent % 10000 == 0)  cout << "- reading event " << iEvent << endl ;

        if (DEBUG) cout << "====== new event ====== " << iEvent << endl ;

        int got = theBigTree.fChain->GetEntry(iEvent);
        if (got == 0) break;

        ++totEvents ;

        int npartons = theBigTree.lheNOutPartons ;
        if (npartons > 4)
        {
            cout << " --> ev : " << iEvent << " npartons is " << npartons << " lowered to 4" << endl;
            npartons = 4;
        }
        
        if (DEBUG) cout << "n partons: " << theBigTree.lheNOutPartons << endl;

        int nbs = theBigTree.lheNOutB;        
        if (nbs > 4)
        {
            cout << " --> ev : " << iEvent << " nbs is " << nbs << " lowered to 4" << endl;
            nbs = 4;
        }
        
        if (DEBUG) cout << "NBs = " << nbs << endl;
	
	float ht = theBigTree.lheHt;
	if (ht >= 10000)
        {
            cout << " --> ev: " << iEvent << " ht is " << ht << " lowered to 9999" << endl;
            ht = 9999.;
	}    
	
        h_nJets_nBs_ht   ->Fill (npartons, nbs, ht);
        hINT_nJets_nBs_ht->Fill (npartons, nbs, ht);

	if (ht >= 0    && ht < 70  ) allEvts[npartons][nbs][0] += 1;
	if (ht >= 70   && ht < 100 ) allEvts[npartons][nbs][1] += 1;
	if (ht >= 100  && ht < 200 ) allEvts[npartons][nbs][2] += 1;
	if (ht >= 200  && ht < 400 ) allEvts[npartons][nbs][3] += 1;
	if (ht >= 400  && ht < 600 ) allEvts[npartons][nbs][4] += 1;
	if (ht >= 600  && ht < 800 ) allEvts[npartons][nbs][5] += 1;
	if (ht >= 800  && ht < 1200) allEvts[npartons][nbs][6] += 1;
	if (ht >= 1200 && ht < 2500) allEvts[npartons][nbs][7] += 1;
	if (ht >= 2500             ) allEvts[npartons][nbs][8] += 1;
    }

    fOut->cd();
    h_nJets_nBs_ht   -> Write();
    hINT_nJets_nBs_ht-> Write();

    for (int nj = 0; nj < 5; nj++)
    {
        cout << "NJET: " << nj << endl;
        
	for (int nb = 0; nb < 5; nb++)
        {
	    cout<< "NB: " << nb << endl;
	    
	    for (int ht = 0; ht < 9; ht++)
            {
                 cout << allEvts[nj][nb][ht] << " ";
	    }
	    
	    cout << endl;
        }
	
        cout << endl;
    }
    
    fOut->Close();
}
