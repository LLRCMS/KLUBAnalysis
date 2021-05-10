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

#define DO_DECAY_INFO true

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

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

pair <int,int> getTauTaufromHHIdx(const bigTree& theBigTree, bool first=true)
{
    pair<int,int> idxs = make_pair(-1,-1);

    int idx1 = -1;
    int idx2 = -1;

    // find the idxs
    for (unsigned int igen = 0; igen < theBigTree.genpart_px->size(); igen++)
    {
        bool isFirst     = CheckBit (theBigTree.genpart_flags->at(igen), 12) ; // 12 = isFirstCopy
        bool isLast      = CheckBit (theBigTree.genpart_flags->at(igen), 13) ; // 13 = isLastCopy
        bool isPrompt    = CheckBit (theBigTree.genpart_flags->at(igen), 0) ; // 0 = isPrompt
        bool isHardProcess    = CheckBit (theBigTree.genpart_flags->at(igen), 7) ; // 7 = isHardProcess
        // bool isHardScatt = CheckBit (theBigTree.genpart_flags->at(igen), 3) ; //   3 = isPromptTauDecayProduct
        // bool isDirectPromptTauDecayProduct = CheckBit (theBigTree.genpart_flags->at(igen), 5) ; // 5 = isDirectPromptTauDecayProduct
        int pdg = theBigTree.genpart_pdg->at(igen);
        // int mothIdx = theBigTree.genpart_TauMothInd->at(igen);
        
        bool isFirstLast = (first ? isFirst : isLast);

        if (abs(pdg) == 15)
        {
            if (isPrompt && isHardProcess && isFirstLast)
            {
                if (idx1 >= 0 && idx2 >= 0)
                {
                    cout << "** ERROR: more than 2 tau from H identified by requiring first? " << first << endl;
                    continue;
                }
                (idx1 == -1) ? (idx1 = igen) : (idx2 = igen) ;
            }
        }
    }
    
    idxs.first = idx1;
    idxs.second = idx2;

    return idxs;
}

pair <int,int> getTauTauProdsfromHHIdx(const bigTree& theBigTree, bool first=true)
{
    pair<int,int> idxs = make_pair(-1,-1);

    int idx1 = -1;
    int idx2 = -1;

    // find the idxs
    for (unsigned int igen = 0; igen < theBigTree.genpart_px->size(); igen++)
    {
        bool isFirst     = CheckBit (theBigTree.genpart_flags->at(igen), 12) ; // 12 = isFirstCopy
        bool isLast      = CheckBit (theBigTree.genpart_flags->at(igen), 13) ; // 13 = isLastCopy
        // bool isPrompt    = CheckBit (theBigTree.genpart_flags->at(igen), 0) ; // 0 = isPrompt
        // bool isHardProcess    = CheckBit (theBigTree.genpart_flags->at(igen), 7) ; // 7 = isHardProcess
        // bool isDirectPromptTauDecayProduct = CheckBit (theBigTree.genpart_flags->at(igen), 5) ; // 5 = isDirectPromptTauDecayProduct
        bool isPromptTauDecayProduct = CheckBit (theBigTree.genpart_flags->at(igen), 3) ; // 3 = isPromptTauDecayProduct
        bool isDirectPromptTauDecayProduct = CheckBit (theBigTree.genpart_flags->at(igen), 5) ; // 5 = isDirectPromptTauDecayProduct
        int pdg = theBigTree.genpart_pdg->at(igen);
        // int mothIdx = theBigTree.genpart_TauMothInd->at(igen);
        
        bool isFirstLast = (first ? isFirst : isLast);

        if (abs(pdg) == 11 || abs(pdg) == 13) // ele, mu
        {
            if (isPromptTauDecayProduct && isDirectPromptTauDecayProduct && isFirstLast)
            {
                if (idx1 >= 0 && idx2 >= 0)
                {
                    cout << "** ERROR: more than 2 visible tau identified by requiring first? " << first << endl;
                    continue;
                }
                (idx1 == -1) ? (idx1 = igen) : (idx2 = igen) ;
            }
        }

        // first-last does not apply to tauh as only direct porducts are made
        // but I have to ensure to be reading the last copy of a tauh
        if (abs(pdg) == 66615) // had taus
        {
            int mothIdx = theBigTree.genpart_TauMothInd->at(igen);
            bool moth_isPrompt = CheckBit (theBigTree.genpart_flags->at(mothIdx), 0) ; // 0 = isPrompt
            // bool moth_isHardProcess = CheckBit (theBigTree.genpart_flags->at(mothIdx), 7) ; // 7 = isHardProcess
            bool moth_isFromHardProcess = CheckBit (theBigTree.genpart_flags->at(mothIdx), 8) ; // 8 = fromHardProcess
            bool moth_isLast = CheckBit (theBigTree.genpart_flags->at(mothIdx), 13) ; // 13 = isLastCopy
            // bool moth_isFirst = CheckBit (theBigTree.genpart_flags->at(mothIdx), 12) ; // 12 = isFirstCopy

            if (moth_isPrompt && moth_isLast && moth_isFromHardProcess)
            {
                if (idx1 >= 0 && idx2 >= 0)
                {
                    cout << "** ERROR: more than 2 visible tau identified by requiring first? (in 66615 if condition) " << first << endl;
                    continue;
                }
                (idx1 == -1) ? (idx1 = igen) : (idx2 = igen) ;
            }
        }
    }
    
    idxs.first = idx1;
    idxs.second = idx2;

    return idxs;
}

void dumpGenParts(const bigTree& theBigTree)
{
    cout << " ===== Dumping gen particles ===== " << endl;
    // find the idxs
    for (unsigned int igen = 0; igen < theBigTree.genpart_px->size(); igen++)
    {
        bool isFirst     = CheckBit (theBigTree.genpart_flags->at(igen), 12) ; // 12 = isFirstCopy
        bool isLast      = CheckBit (theBigTree.genpart_flags->at(igen), 13) ; // 13 = isLastCopy
        bool isPrompt    = CheckBit (theBigTree.genpart_flags->at(igen), 0) ; // 0 = isPrompt
        bool isHardProcess    = CheckBit (theBigTree.genpart_flags->at(igen), 7) ; // 7 = isHardProcess
        bool isFromHardProcess = CheckBit (theBigTree.genpart_flags->at(igen), 8) ; // 8 = fromHardProcess
        // bool isDirectPromptTauDecayProduct = CheckBit (theBigTree.genpart_flags->at(igen), 5) ; // 5 = isDirectPromptTauDecayProduct
        bool isPromptTauDecayProduct = CheckBit (theBigTree.genpart_flags->at(igen), 3) ; // 3 = isPromptTauDecayProduct
        // bool isDirectPromptTauDecayProduct = CheckBit (theBigTree.genpart_flags->at(igen), 5) ; // 5 = isDirectPromptTauDecayProduct
        int pdg = theBigTree.genpart_pdg->at(igen);
        // int mothIdx = theBigTree.genpart_TauMothInd->at(igen);
        int mothIdx = theBigTree.genpart_TauMothInd->at(igen);

        TLorentzVector vGen;
        vGen.SetPxPyPzE(theBigTree.genpart_px->at(igen), theBigTree.genpart_py->at(igen), theBigTree.genpart_pz->at(igen), theBigTree.genpart_e->at(igen));

        cout << igen << " ) pdg: " << pdg << " pt: " << vGen.Pt() << " eta: " << vGen.Eta() << " phi: " << vGen.Phi() << " isFirst: " << isFirst << " isLast: " << isLast
                     <<  " isPrompt: " << isPrompt << " isHardProcess: " << isHardProcess << " isFromHardProcess: " << isFromHardProcess << " isPromptTauDecayProduct: " << isPromptTauDecayProduct << " tauMothIdx: " << mothIdx << endl;
    }

    cout << endl << endl;
}

// true if I need to switch candidates, else false
bool switchCands (TLorentzVector& v1, TLorentzVector& v2, int pdg1, int pdg2)
{
    int apdg1 = abs(pdg1);
    int apdg2 = abs(pdg2);

    if (apdg1 == apdg2)
    {
        if (v1.Pt() > v2.Pt()) return false;
        else return true;
    }

    else
    {
        // e > mu > tau
        if (apdg1 < apdg2) return false;
        else return true;
    }

    // just in case..
    return false;
}

int main()
{

    if (gConfigParser) return 1 ;
    gConfigParser = new ConfigParser () ;  
    TString config = "/home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/config/skim_2016.cfg"; 
    cout << "** INFO: reading config : " << config << endl;
    gConfigParser->init(config);
    ///////////////

    // TString inputFile = "/home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Files_7Feb2016/allNonResClonesBench/filelist_bench_1.txt";
    // TString fOutName = "HH_info_file.root";

    // TString inputFile = "/home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Files_7Feb2016/38_GluGluToHHTo2B2Tau_node_SM_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt";
    // TString fOutName = "HH_info_file_SM.root";

    // TString inputFile = "/home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Files_7Feb2016/39_GluGluToHHTo2B2Tau_node_box_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt";
    // TString fOutName = "HH_info_file_box.root";

    // TString inputFile = "/home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Files_7Feb2016/42_GluGluToHHTo2B2Tau_node_11_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt";
    // TString fOutName = "HH_info_file_bench11.root";

     TString inputFile = "/home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Files_7Feb2016/allNonResClonesBench/filelist_bench_1.txt";
     TString fOutName = "HH_info_file_decayinfo_wl5.root";

    //TString inputFile = "/home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Files_7Feb2016/38_GluGluToHHTo2B2Tau_node_SM_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt";
    //TString fOutName = "HH_info_file_SM_decayinfo.root";

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
    TFile* fHHDiffRew = new TFile(inMapFile.c_str());
    hhreweighterInputMap = (TH2*) fHHDiffRew->Get(inHistoName.c_str());
    hhreweighter = new HHReweight5D(coeffFile, hhreweighterInputMap, string("manual"), string(""), string("nlo"));

    // speed up - be careful to activate the branches that are needed!!
    theBigTree.fChain->SetBranchStatus("*", 0);
    theBigTree.fChain->SetBranchStatus("genpart_*", 1);

    // getWeight(double kl, double kt, double c2, double cg, double c2g, double mhh, double cth)
    ///////////////
    vector< hhCoeffs > points;
    
    points.push_back(make_tuple(1,1,0,0,0,"SM"));
    points.push_back(make_tuple(2.46,1,0,0,0,"kl_2p46"));
    points.push_back(make_tuple(5,1,0,0,0,"kl_5"));
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

    points.push_back(make_tuple(1 , 1 , 0 ,  0 ,  0, "tri_box_only"));
    points.push_back(make_tuple(0 , 1 , 0 ,  0 ,  0, "box_only"));
    points.push_back(make_tuple(0 , 0 , 1 ,  0 ,  0, "c2_only"));
    points.push_back(make_tuple(1 , 0 , 0 ,  1 ,  0, "cg_only"));
    points.push_back(make_tuple(0 , 0 , 0 ,  0 ,  1, "c2g_only"));


    TFile* fOut = new TFile(fOutName, "recreate");
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

    //////// info on the decay... just make branches is required
    // leptons from H decay
    float tau1_pt;
    float tau1_eta;
    float tau1_phi;
    float tau1_e;
    // int tau1_pdg;
    
    float tau2_pt;
    float tau2_eta;
    float tau2_phi;
    float tau2_e;
    // int tau2_pdg;
    
    // tau decay products
    float tau1_prod_pt;
    float tau1_prod_eta;
    float tau1_prod_phi;
    float tau1_prod_e;
    int  tau1_prod_pdg;
    float tau2_prod_pt;
    float tau2_prod_eta;
    float tau2_prod_phi;
    float tau2_prod_e;
    int tau2_prod_pdg;

    if (DO_DECAY_INFO)
    {
        tOut->Branch("tau1_pt", &tau1_pt);
        tOut->Branch("tau1_eta", &tau1_eta);
        tOut->Branch("tau1_phi", &tau1_phi);
        tOut->Branch("tau1_e", &tau1_e);
        
        tOut->Branch("tau2_pt", &tau2_pt);
        tOut->Branch("tau2_eta", &tau2_eta);
        tOut->Branch("tau2_phi", &tau2_phi);
        tOut->Branch("tau2_e", &tau2_e);

        tOut->Branch("tau1_prod_pt", &tau1_prod_pt);
        tOut->Branch("tau1_prod_eta", &tau1_prod_eta);
        tOut->Branch("tau1_prod_phi", &tau1_prod_phi);
        tOut->Branch("tau1_prod_e", &tau1_prod_e);
        tOut->Branch("tau1_prod_pdg", &tau1_prod_pdg);
        
        tOut->Branch("tau2_prod_pt", &tau2_prod_pt);
        tOut->Branch("tau2_prod_eta", &tau2_prod_eta);
        tOut->Branch("tau2_prod_phi", &tau2_prod_phi);
        tOut->Branch("tau2_prod_e", &tau2_prod_e);
        tOut->Branch("tau2_prod_pdg", &tau2_prod_pdg);

    }

    // loop over events
    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    for (Long64_t iEvent = 0 ; true ; ++iEvent) 
    {
        if (iEvent % 10000 == 0)  cout << "- reading event " << iEvent << endl ;
        int got = theBigTree.fChain->GetEntry(iEvent);
        if (got == 0) break;

        // dumpGenParts(theBigTree);

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

        /// optional decay stuff
        if (DO_DECAY_INFO)
        {
            pair<int,int> tautauIdxs_firstcopy = getTauTaufromHHIdx(theBigTree);
            int idx_tau_1 = tautauIdxs_firstcopy.first;
            int idx_tau_2 = tautauIdxs_firstcopy.second;

            if (idx_tau_1 == -1 || idx_tau_2 == -1)
            {
                cout << "DID not find 2 tautau , setting dummy vals!" << endl;
                tau1_pt = -1.;
                tau1_eta = -1.;
                tau1_phi = -1.;
                tau1_e = -1.;
                // tau1_pdg = -1.;

                tau2_pt = -1.;
                tau2_eta = -1.;
                tau2_phi = -1.;
                tau2_e = -1.;
                // tau2_pdg = -1.;
            }
            else
            {
                // order by highest pt: pt1 > pt2
                TLorentzVector vTau1_cp;
                TLorentzVector vTau2_cp;
                vTau1_cp.SetPxPyPzE (theBigTree.genpart_px->at(idx_tau_1), theBigTree.genpart_py->at(idx_tau_1), theBigTree.genpart_pz->at(idx_tau_1), theBigTree.genpart_e->at(idx_tau_1) );
                vTau2_cp.SetPxPyPzE (theBigTree.genpart_px->at(idx_tau_2), theBigTree.genpart_py->at(idx_tau_2), theBigTree.genpart_pz->at(idx_tau_2), theBigTree.genpart_e->at(idx_tau_2) );

                TLorentzVector vTau1;
                TLorentzVector vTau2;
                vTau1 = (vTau1_cp.Pt() > vTau2_cp.Pt() ? vTau1_cp : vTau2_cp) ;
                vTau2 = (vTau1_cp.Pt() > vTau2_cp.Pt() ? vTau2_cp : vTau1_cp) ;

                tau1_pt = vTau1.Pt();
                tau1_eta = vTau1.Eta();
                tau1_phi = vTau1.Phi();
                tau1_e = vTau1.E();
                
                tau2_pt = vTau2.Pt();
                tau2_eta = vTau2.Eta();
                tau2_phi = vTau2.Phi();
                tau2_e = vTau2.E();
            }

            pair<int,int> tautauDecayIdxs_firstcopy = getTauTauProdsfromHHIdx(theBigTree);
            int idx_tau_decay_1 = tautauDecayIdxs_firstcopy.first;
            int idx_tau_decay_2 = tautauDecayIdxs_firstcopy.second;

            if (idx_tau_decay_1 == -1 || idx_tau_decay_2 == -1)
            {
                cout << "DID not find 2 tautau decay prods, setting dummy vals! " << idx_tau_decay_1 << " " << idx_tau_decay_2 << endl;
                tau1_prod_pt = -1.;
                tau1_prod_eta = -1.;
                tau1_prod_phi = -1.;
                tau1_prod_e = -1.;
                tau1_prod_pdg = -1.;
                tau2_prod_pt = -1.;
                tau2_prod_eta = -1.;
                tau2_prod_phi = -1.;
                tau2_prod_e = -1.;
                tau2_prod_pdg = -1.;
            }
            else
            {
                // order by highest pt: pt1 > pt2
                TLorentzVector vTau1_cp;
                TLorentzVector vTau2_cp;
                vTau1_cp.SetPxPyPzE (theBigTree.genpart_px->at(idx_tau_decay_1), theBigTree.genpart_py->at(idx_tau_decay_1), theBigTree.genpart_pz->at(idx_tau_decay_1), theBigTree.genpart_e->at(idx_tau_decay_1) );
                vTau2_cp.SetPxPyPzE (theBigTree.genpart_px->at(idx_tau_decay_2), theBigTree.genpart_py->at(idx_tau_decay_2), theBigTree.genpart_pz->at(idx_tau_decay_2), theBigTree.genpart_e->at(idx_tau_decay_2) );
                int pdg1_cp = theBigTree.genpart_pdg->at(idx_tau_decay_1);
                int pdg2_cp = theBigTree.genpart_pdg->at(idx_tau_decay_2);

                TLorentzVector vTau1;
                TLorentzVector vTau2;
                bool switchCand = switchCands (vTau1_cp, vTau2_cp, pdg1_cp, pdg2_cp);

                vTau1 = ((!switchCand) ? vTau1_cp : vTau2_cp) ;
                vTau2 = ((!switchCand) ? vTau2_cp : vTau1_cp) ;
                int pdg1 = ((!switchCand) ? pdg1_cp : pdg2_cp) ;
                int pdg2 = ((!switchCand) ? pdg2_cp : pdg1_cp) ;
                
                tau1_prod_pt = vTau1.Pt();
                tau1_prod_eta = vTau1.Eta();
                tau1_prod_phi = vTau1.Phi();
                tau1_prod_e = vTau1.E();
                tau1_prod_pdg = pdg1;
                
                tau2_prod_pt = vTau2.Pt();
                tau2_prod_eta = vTau2.Eta();
                tau2_prod_phi = vTau2.Phi();
                tau2_prod_e = vTau2.E();
                tau2_prod_pdg = pdg2;
            }


        }
        tOut->Fill();
    }

    fOut->cd();
    tOut->Write();
}
