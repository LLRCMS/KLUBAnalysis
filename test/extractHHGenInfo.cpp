#include "HHReweight5D.h"
#include "bigTree.h" 
#include "ConfigParser.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <bitset>
#include <utility>
#include <map>
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TBranch.h"
#include "TString.h"
#include "TLorentzVector.h"

using namespace std;
typedef tuple<float, float, float, float, float, string> hhCoeffs; //kl, kt, c2, cg, c2g, name

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

// generic function to read content of 1D / 2D histos, taking care of x axis limit (no under/over flow)
double getContentHisto1D(TH1* histo, double x)
{
  int nbinsx = histo->GetNbinsX();
  int ibin = histo->FindBin(x);
  if (ibin == 0) ibin = 1;
  if (ibin > nbinsx) ibin = nbinsx;
  return histo->GetBinContent(ibin);
}

double getContentHisto2D(TH2* histo, double x, double y)
{
  int nbinsx = histo->GetNbinsX();
  int nbinsy = histo->GetNbinsY();
  int ibinx = histo->GetXaxis()->FindBin(x);
  int ibiny = histo->GetYaxis()->FindBin(y);

  if (ibinx == 0)     ibinx = 1;
  if (ibinx > nbinsx) ibinx = nbinsx;

  if (ibiny == 0)     ibiny = 1;
  if (ibiny > nbinsy) ibiny = nbinsy;
  
  return histo->GetBinContent(ibinx, ibiny);
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

bool CheckBit (int number, int bitpos)
{
    bool res = number & (1 << bitpos);
    return res;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

pair<int,int> getHardScatterHHIdx(const bigTree& theBigTree)
{
    pair<int,int> idxs = make_pair(-1,-1);

    int idx1 = -1;
    int idx2 = -1;

    // find the idxs
    for (unsigned int igen = 0; igen < theBigTree.genpart_px->size(); igen++)
    {
        bool isFirst     = CheckBit (theBigTree.genpart_flags->at(igen), 12) ; // 12 = isFirstCopy
        // bool isLast      = CheckBit (theBigTree.genpart_flags->at(igen), 13) ; // 13 = isLastCopy
        // bool isHardScatt = CheckBit (theBigTree.genpart_flags->at(igen), 5) ; //   3 = isPromptTauDecayProduct
        // bool isDirectPromptTauDecayProduct = CheckBit (theBigTree.genpart_flags->at(igen), 5) ; // 5 = isDirectPromptTauDecayProduct
        int pdg = theBigTree.genpart_pdg->at(igen);
        // int mothIdx = theBigTree.genpart_TauMothInd->at(igen);

        if (abs(pdg) == 25)
        {
            // cout << igen << " H boson: Px " << theBigTree.genpart_px->at(igen) << " first? " << isFirst << " decMode : " << theBigTree.genpart_HZDecayMode->at(igen) << endl;
            if (isFirst)
            {
                if (idx1 >= 0 && idx2 >= 0)
                {
                    cout << "** ERROR: more than 2 H identified (first)" << endl;
                    continue;
                }
                (idx1 == -1) ? (idx1 = igen) : (idx2 = igen) ;
            }
            // if (isLast)
            // {
            //     if (idx1last >= 0 && idx2last >= 0)
            //     {
            //         cout << "** ERROR: more than 2 H identified (last)" << endl;
            //         // continue; // no need to skip the event in this case -- dec mode just for studies
            //     }
            //     (idx1last == -1) ? (idx1last = igen) : (idx2last = igen) ;
            // }
        }
    }

    idxs.first = idx1;
    idxs.second = idx2;

    return idxs;

}


int main()
{

    if (gConfigParser) return 1 ;
    gConfigParser = new ConfigParser () ;  
    TString config = "/home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/config/skim_2016.cfg"; 
    cout << "** INFO: reading config : " << config << endl;
    gConfigParser->init(config);
    ///////////////

    TString inputFile = "/home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Files_7Feb2016/allNonResClonesBench/filelist_bench_1.txt";

    TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;
    appendFromFileList (bigChain, inputFile);
    bigTree theBigTree (bigChain) ;

    HHReweight5D* hhreweighter = nullptr;
    TH2* hhreweighterInputMap = nullptr;
    string inMapFile   = gConfigParser->readStringOption("HHReweight::inputFile");
    string inHistoName = gConfigParser->readStringOption("HHReweight::histoName");
    string coeffFile    = gConfigParser->readStringOption("HHReweight::coeffFile");
    cout << "** INFO: reading histo named: " << inHistoName << " from file: " << inMapFile << endl;
    cout << "** INFO: HH reweight coefficient file is: " << coeffFile << endl;
    TFile* fHHDynamicRew = new TFile(inMapFile.c_str());
    hhreweighterInputMap = (TH2*) fHHDynamicRew->Get(inHistoName.c_str());
    // hhreweighter = new HHReweight(coeffFile, hhreweighterInputMap);
    hhreweighter = new HHReweight5D(coeffFile, hhreweighterInputMap);

    // speed up - be careful to activate the branches that are needed!!
    theBigTree.fChain->SetBranchStatus("*", 0);
    theBigTree.fChain->SetBranchStatus("genpart_*", 1);

    ///////////////
    vector< hhCoeffs > points;
    
    points.push_back(make_tuple(1,1,0,0,0,"SM"));
    points.push_back(make_tuple(2.46,1,0,0,0,"kl_2p46"));
    points.push_back(make_tuple(20,1,0,0,0,"kl_20"));
    points.push_back(make_tuple(0,1,0,0,0,"kl_0"));
    points.push_back(make_tuple(200,1,0,0,0,"kl_200"));

    points.push_back(make_tuple(7.5  , 1.0 ,-1.0 ,  0.0 ,  0.0, "bench_1"));
    points.push_back(make_tuple(1.0  , 1.0 , 0.5 , -0.8 ,  0.6, "bench_2"));
    points.push_back(make_tuple(1.0  , 1.0 ,-1.5 ,  0.0 , -0.8, "bench_3"));
    points.push_back(make_tuple(-3.5 , 1.5 ,-3.0 ,  0.0 ,  0.0, "bench_4"));
    points.push_back(make_tuple(1.0  , 1.0 , 0.0 ,  0.8 , -1.0, "bench_5"));
    points.push_back(make_tuple(2.4  , 1.0 , 0.0 ,  0.2 , -0.2, "bench_6"));
    points.push_back(make_tuple(5.0  , 1.0 , 0.0 ,  0.2 , -0.2, "bench_7"));
    points.push_back(make_tuple(15.0 , 1.0 , 0.0 , -1.0 ,  1.0, "bench_8"));
    points.push_back(make_tuple(1.0  , 1.0 , 1.0 , -0.6 ,  0.6, "bench_9"));
    points.push_back(make_tuple(10.0 , 1.5 ,-1.0 ,  0.0 ,  0.0, "bench_10"));
    points.push_back(make_tuple(2.4  , 1.0 , 0.0 ,  1.0 , -1.0, "bench_11"));
    points.push_back(make_tuple(15.0 , 1.0 , 1.0 ,  0.0 ,  0.0, "bench_12"));


    TFile* fOut = new TFile("HH_info_file.root", "recreate");
    fOut->cd();
    TTree * tOut = new TTree("HHinfo", "HHinfo");
    float _cth;
    float _mhh;
    tOut->Branch("cth", &_cth);
    tOut->Branch("mhh", &_mhh);
    map<string, float> hhWeights;

    // make the value map
    for (hhCoeffs coeff : points){
        cout << "-- storing coeffs for " << get<0>(coeff) << " " << get<1>(coeff) << " " << get<2>(coeff) << " " << get<3>(coeff) << " " << get<4>(coeff) << endl;
        hhWeights[get<5>(coeff)] = 0.0;
    }
    
    // set branch address   
    for (hhCoeffs coeff : points){
       tOut->Branch(get<5>(coeff).c_str(), &hhWeights[get<5>(coeff)]);
    } 

    // loop over events
    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    for (Long64_t iEvent = 0 ; true ; ++iEvent) 
    {
        if (iEvent % 10000 == 0)  cout << "- reading event " << iEvent << endl ;
        int got = theBigTree.fChain->GetEntry(iEvent);
        if (got == 0) break;

        pair<int,int> hhIdxs = getHardScatterHHIdx(theBigTree);
        int idx1 = hhIdxs.first;
        int idx2 = hhIdxs.second;

        if (idx1 == -1 || idx2 == -1){
            cout << "DID not find 2 HS Hggs!" << endl;
            continue;
        }

        TLorentzVector vH1;
        TLorentzVector vH2;
        vH1.SetPxPyPzE (theBigTree.genpart_px->at(idx1), theBigTree.genpart_py->at(idx1), theBigTree.genpart_pz->at(idx1), theBigTree.genpart_e->at(idx1) );
        vH2.SetPxPyPzE (theBigTree.genpart_px->at(idx2), theBigTree.genpart_py->at(idx2), theBigTree.genpart_pz->at(idx2), theBigTree.genpart_e->at(idx2) );
        TLorentzVector vSum = vH1+vH2;
        float mHH = vSum.M();
        vH1.Boost(-vSum.BoostVector());                     
        float ct1 = vH1.CosTheta();

        for (hhCoeffs coeff : points)
        {
            float HHweight = hhreweighter->getWeight(get<0>(coeff), get<1>(coeff), get<2>(coeff), get<3>(coeff), get<4>(coeff), mHH, ct1);
            hhWeights[get<5>(coeff)] = HHweight;
        }

        _cth = ct1;
        _mhh = mHH;
        tOut->Fill();
    }

    fOut->cd();
    tOut->Write();
}