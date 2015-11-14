#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include "../../interface/OfflineProducerHelper.h"

#include "../../interface/bigTree.h"
#include "../../interface/triggerReader.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//c++ -lm -o genMatchStudy genMatchStudy.cpp ../../src/OfflineProducerHelper.cc ../../src/triggerReader.cc -I ../../interface/ `root-config --glibs --cflags`
// call once voms-proxy-init -voms cms before running

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
        {
            //cout << "Adding file: " << line.c_str() << endl;
            chain->Add(line.c_str());
        }
    }
    return;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// open the first file in the input list, retrieve the histogram "Counters" for the trigger names and return a copy of it
TH1F* getFirstFileHisto (TString filename)
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
    TH1F* histo = new TH1F (*dummy);
    histo-> SetDirectory(0);
    histo->SetName ("Counters_perTrigger");
    fIn->Close();
    return histo;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -

bool CheckBit (int number, int bitpos)
{
    bool res = number & (1 << bitpos);
    return res;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -

int DecayMode (int pdg1, int pdg2)
{
    int a1 = abs(pdg1);
    int a2 = abs(pdg2);
    int nMu = 0;
    int nEle = 0;
    int nTau = 0;
  
    if (a1 == 11) nEle++;
    if (a1 == 13) nMu++;
    if (a1 == 66615) nTau++;
  
    if (a2 == 11) nEle++;
    if (a2 == 13) nMu++;
    if (a2 == 66615) nTau++;

    //cout << pdg1 << " " << pdg2 << " " << nEle << " " << nMu << " " << nTau << endl;

    if (nEle == 0 && nMu == 1 && nTau == 1) return 0;
    if (nEle == 1 && nMu == 0 && nTau == 1) return 1;
    if (nEle == 0 && nMu == 0 && nTau == 2) return 2;

    if (nEle == 0 && nMu == 2 && nTau == 0) return 3;
    if (nEle == 2 && nMu == 0 && nTau == 0) return 4;
    if (nEle == 1 && nMu == 1 && nTau == 0) return 5;

    return 8; // other
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -

int makePDG (int particleType)
{
    if (particleType == 0) return 13;
    if (particleType == 1) return 11;
    if (particleType == 2) return 66615;
    return -1;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -

int makeParticleType (int pdg)
{
    int a = abs(pdg);
    if (a == 11) return 1;
    if (a == 13) return 0;
    if (a == 66615) return 2;
    return -1;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -

int getPairByIndexes (bigTree* tree, int dau1, int dau2)
{
    int pair = -1;
    for (int iPair = 0; iPair < tree->indexDau1->size(); iPair++)
    {
        int ind1 = tree->indexDau1->at(iPair);
        int ind2 = tree->indexDau2->at(iPair);
        if (ind1 == dau1 && ind2 == dau2) pair = iPair;
        else if (ind2 == dau1 && ind1 == dau2) pair = iPair;
        
        if (pair != -1) break; // don't continue search, pairs are unique
    }
    return pair;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -

int getPairType (int type1, int type2)
{
    int nmu = 0;
    int nele = 0;
    int ntau = 0;
    
    if ( type1 == 0 ) nmu++;
    if ( type1 == 1 ) nele++;
    if ( type1 == 2 ) ntau++;

    if ( type2 == 0 ) nmu++;
    if ( type2 == 1 ) nele++;
    if ( type2 == 2 ) ntau++;

    if (nmu == 1 && nele == 0 && ntau == 1) return 0;
    if (nmu == 0 && nele == 1 && ntau == 1) return 1;
    if (nmu == 0 && nele == 0 && ntau == 2) return 2;
    if (nmu == 2 && nele == 0 && ntau == 0) return 3;
    if (nmu == 0 && nele == 2 && ntau == 0) return 4;
    if (nmu == 1 && nele == 1 && ntau == 0) return 5;
    
    return -1;

}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -

/*
bool minimalGenCut (TLorentzVector& vec, int pdg)
{
    int aPdg = abs(pdg);
    bool accept = true;

    if (aPdg == 11) // ele
    {
        //if (vec.Pt() < 7) accept = false;
        if (vec.Pt() < 23) accept = false;
        if (TMath::Abs(vec.Eta()) > 2.5) accept = false;
    }
    
    else if (aPdg == 13)
    {
        //if (vec.Pt() < 7) accept = false;
        if (vec.Pt() < 18) accept = false;
        //if (TMath::Abs(vec.Eta()) > 2.5) accept = false;
        if (TMath::Abs(vec.Eta()) > 2.1) accept = false;
    }

    else if (aPdg == 66615)
    {
        //if (vec.Pt() < 20 ) accept = false;
        if (vec.Pt() < 45 ) accept = false;
        //if (TMath::Abs(vec.Eta()) > 2.5) accept = false;
        if (TMath::Abs(vec.Eta()) > 2.1) accept = false;
    }

    else accept = false;

    return accept;
}
*/


bool minimalGenCut (TLorentzVector& vec, int pdg)
{
    return true;
}

int main()
{
    //TString filelist = "../inputFiles/KLUB_prod/DY_KLUB.txt"; // some files corrupted
    //TString filelist = "../inputFiles/28Ago2015/3_DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8__RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1.txt";
   
    //TString filelist = "myDrell-YanM50.txt"; // less files
    TString filelist = "myRadion300H_hh.txt";

    TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;
    //bigChain->Add (inputFile) ;
    appendFromFileList (bigChain, filelist);
    bigTree theBigTree (bigChain) ;

    // ------------- set only branches used to speed up execution
    /*
    theBigTree.fChain->SetBranchStatus("*", 0);
    theBigTree.fChain->SetBranchStatus("genpart_*", 1);
    theBigTree.fChain->SetBranchStatus("daughters_*", 1);
    theBigTree.fChain->SetBranchStatus("particleType", 1);
    theBigTree.fChain->SetBranchStatus("isOSCand", 1);
    theBigTree.fChain->SetBranchStatus("indexDau*", 1);
    */
    // ---------------------------------

    enum HZDecay {
    MuHad  = 0,
    EHad   = 1,
    HadHad = 2,
    MuMu   = 3,
    EE     = 4,
    EMu    = 5,
    EEPrompt = 6, // prompt Z->ee/mumu decays
    MuMuPrompt = 7,
    Other  = 8 // for e.g. h->bb
    };

    const double matchCone = 0.3 ;
    const bool askTriggers = false; // ask triggers for the selected pair type
    const bool askFlavourFromTriggers = false; // decide the flavour acording to triggers; MuTau > EleTAu > TauTau
    const bool ask3rdlepVeto = false;
    const int askThisTriggerType = -1; // skip all events without the triggers for this type (-1 for not applying this requirement and keep all events)
    string leptonSelectionFlag ("Vertex-LepID-pTMin-etaMax-againstEle-againstMu");
    //string leptonSelectionFlag ("Vertex-LepID-pTMin-etaMax-againstEle-againstMu-Iso");

    const int PDGProcess = 25; // decay of boson that is analyzed, 23=Z, 25=H

    // --------------------------------
    // histos
    TFile* fOut = new TFile ("genMatchHistos.root", "recreate");
    TH1F* hZDecMode = new TH1F ("hZDecMode", "Z decays into #tau #tau", 6, 0, 6);
    TH1F* hNDaughters = new TH1F ("hNDaughters", "Size of daughters_ collection", 10, 0, 10);
    TH1F* hNDaughtersPreGen = new TH1F ("hNDaughtersPreGen", "Size of daughters_ collection right from the llr ntuple", 10, 0, 10);

    TH2F* hMatchedPairIsOS = new TH2F ("hMatchedPairIsOS", "pair matched to MC is OS?" , 6, 0, 6, 2, 0, 2) ;
    hMatchedPairIsOS->GetXaxis()->SetBinLabel (1, "MuTau");
    hMatchedPairIsOS->GetXaxis()->SetBinLabel (2, "EleTau");
    hMatchedPairIsOS->GetXaxis()->SetBinLabel (3, "TauTau");
    hMatchedPairIsOS->GetXaxis()->SetBinLabel (4, "MuMu");
    hMatchedPairIsOS->GetXaxis()->SetBinLabel (5, "EleEle");
    hMatchedPairIsOS->GetXaxis()->SetBinLabel (6, "EleMu");

    TH2F* hChosenPairIsOS = new TH2F ("hChosenPairIsOS", "pair chosen is OS?" , 6, 0, 6, 2, 0, 2) ;
    hChosenPairIsOS->GetXaxis()->SetBinLabel (1, "MuTau");
    hChosenPairIsOS->GetXaxis()->SetBinLabel (2, "EleTau");
    hChosenPairIsOS->GetXaxis()->SetBinLabel (3, "TauTau");
    hChosenPairIsOS->GetXaxis()->SetBinLabel (4, "MuMu");
    hChosenPairIsOS->GetXaxis()->SetBinLabel (5, "EleEle");
    hChosenPairIsOS->GetXaxis()->SetBinLabel (6, "EleMu");

    TH2F* hMCvsChosenPairType = new TH2F ("hMCvsChosenPairType", "chosen vs MC type; MC type; reco type" , 6, 0, 6, 7, 0, 7) ;
    hMCvsChosenPairType->GetXaxis()->SetBinLabel (1, "MuTau");
    hMCvsChosenPairType->GetXaxis()->SetBinLabel (2, "EleTau");
    hMCvsChosenPairType->GetXaxis()->SetBinLabel (3, "TauTau");
    hMCvsChosenPairType->GetXaxis()->SetBinLabel (4, "MuMu");
    hMCvsChosenPairType->GetXaxis()->SetBinLabel (5, "EleEle");
    hMCvsChosenPairType->GetXaxis()->SetBinLabel (6, "EleMu");

    hMCvsChosenPairType->GetYaxis()->SetBinLabel (1, "MuTau");
    hMCvsChosenPairType->GetYaxis()->SetBinLabel (2, "EleTau");
    hMCvsChosenPairType->GetYaxis()->SetBinLabel (3, "TauTau");
    hMCvsChosenPairType->GetYaxis()->SetBinLabel (4, "MuMu");
    hMCvsChosenPairType->GetYaxis()->SetBinLabel (5, "EleEle");
    hMCvsChosenPairType->GetYaxis()->SetBinLabel (6, "EleMu");
    hMCvsChosenPairType->GetYaxis()->SetBinLabel (7, "None");


    TH2F* hNMatches = new TH2F ("hNMatches", "number of reco matches per gen", 3, 0, 3, 5, 0, 5);
    hNMatches->GetXaxis()->SetBinLabel (1, "Mu");
    hNMatches->GetXaxis()->SetBinLabel (2, "Ele");
    hNMatches->GetXaxis()->SetBinLabel (3, "Tau");

    TH2F* hRecoDauCharge = new TH2F ("hRecoDauCharge", "reco lepton has same charge as gen?", 3, 0, 3, 2, 0, 2);
    hRecoDauCharge->GetXaxis()->SetBinLabel (1, "Mu");
    hRecoDauCharge->GetXaxis()->SetBinLabel (2, "Ele");
    hRecoDauCharge->GetXaxis()->SetBinLabel (3, "Tau");

    TH2F* hPairChosenStat = new TH2F ("hPairChosenStat", "chosen tau pair vs MC truth (g.p.); MC pair type; status", 6, 0, 6, 4, 0, 4);
    hPairChosenStat->GetXaxis()->SetBinLabel (1, "MuTau");
    hPairChosenStat->GetXaxis()->SetBinLabel (2, "EleTau");
    hPairChosenStat->GetXaxis()->SetBinLabel (3, "TauTau");
    hPairChosenStat->GetXaxis()->SetBinLabel (4, "MuMu");
    hPairChosenStat->GetXaxis()->SetBinLabel (5, "EleEle");
    hPairChosenStat->GetXaxis()->SetBinLabel (6, "EleMu");

    hPairChosenStat->GetYaxis()->SetBinLabel (1, "g.p. chosen");
    hPairChosenStat->GetYaxis()->SetBinLabel (2, "g.p. not chosen but pass sel");
    hPairChosenStat->GetYaxis()->SetBinLabel (3, "g.p. not pass sel, other chosen");
    hPairChosenStat->GetYaxis()->SetBinLabel (4, "no pair chosen (zero after sel.)");

    // ----- kinematics histos

    TH1F* hPt_MC [3][2]; // idx1: [0]: ele, [1] : mu, [2]: tau --- idx2: [0] : all incoming evts , [1] : matched events 
    hPt_MC[0][0] = new TH1F ("hPt_Mu_MC_allEvts",  "pt Mu (all evts); pT [GeV]; evts", 100, 0, 200);
    hPt_MC[0][1] = new TH1F ("hPt_Mu_MC_matchEvts", "pt Mu (reco matched evts); pT [GeV]; evts", 100, 0, 200);

    hPt_MC[1][0] = new TH1F ("hPt_Ele_MC_allEvts",  "pt ele (all evts); pT [GeV]; evts", 100, 0, 200);
    hPt_MC[1][1] = new TH1F ("hPt_Ele_MC_matchEvts", "pt ele (reco matched evts); pT [GeV]; evts", 100, 0, 200);

    hPt_MC[2][0] = new TH1F ("hPt_Tau_MC_allEvts",  "pt Tau (all evts); pT [GeV]; evts", 100, 0, 200);
    hPt_MC[2][1] = new TH1F ("hPt_Tau_MC_matchEvts", "pt Tau (reco matched evts); pT [GeV]; evts", 100, 0, 200);


    TH1F* hIso_reco [3][2]; // idx1: [0]: ele, [1] : mu, [2]: tau --- idx2: [0] : all matched evts , [1] : events passing selection
    hIso_reco[0][0] = new TH1F ("hIso_Mu_reco_matchEvts",  "Iso Mu (all evts); rel. iso ; evts", 100, 0, 2);
    hIso_reco[0][1] = new TH1F ("hIso_Mu_reco_passSel", "Iso Mu (reco matched evts); rel. iso; evts", 100, 0, 2);

    hIso_reco[1][0] = new TH1F ("hIso_Ele_reco_matchEvts",  "Iso ele (all evts); rel. iso; evts", 100, 0, 2);
    hIso_reco[1][1] = new TH1F ("hIso_Ele_reco_passSel", "Iso ele (reco matched evts); rel. iso; evts", 100, 0, 2);

    hIso_reco[2][0] = new TH1F ("hIso_Tau_reco_matchEvts",  "Iso Tau (all evts); Iso [GeV]; evts", 100, 0, 20);
    hIso_reco[2][1] = new TH1F ("hIso_Tau_reco_passSel", "Iso Tau (reco matched evts); Iso [GeV]; evts", 100, 0, 20);

    TH1F* hIsOS [6]; // index is MC category
    hIsOS[0] = new TH1F ("hIsOS_MuTau",  "isOS - MC MuTau",  2, 0, 2);
    hIsOS[1] = new TH1F ("hIsOS_EleTau", "isOS - MC EleTau", 2, 0, 2);
    hIsOS[2] = new TH1F ("hIsOS_TauTau", "isOS - MC TauTau", 2, 0, 2);
    hIsOS[3] = new TH1F ("hIsOS_MuMu",   "isOS - MC MuMu",   2, 0, 2);
    hIsOS[4] = new TH1F ("hIsOS_EleEle", "isOS - MC EleEle", 2, 0, 2);
    hIsOS[5] = new TH1F ("hIsOS_MuEle",  "isOS - MC MuEle",  2, 0, 2);


    // --------------------------------

    int nEventsIn = theBigTree.fChain->GetEntries();
    int nEventsMax = 1000000;

    int nEvents = min (nEventsIn, nEventsMax) ;

    OfflineProducerHelper oph;

    // ---------------------------------
    
    TH1F* hTriggers = getFirstFileHisto (filelist);

    vector<string> trigMuTau  ; //  =  (isMC ? gConfigParser->readStringListOption ("triggersMC::MuTau")  : gConfigParser->readStringListOption ("triggersData::MuTau")) ;
    vector<string> trigTauTau ; //  = (isMC ? gConfigParser->readStringListOption ("triggersMC::TauTau") : gConfigParser->readStringListOption ("triggersData::TauTau")) ;
    vector<string> trigEleTau ; //  = (isMC ? gConfigParser->readStringListOption ("triggersMC::EleTau") : gConfigParser->readStringListOption ("triggersData::EleTau")) ;
    vector<string> trigEleMu  ; //  =  (isMC ? gConfigParser->readStringListOption ("triggersMC::EleMu")  : gConfigParser->readStringListOption ("triggersData::EleMu")) ;

    trigMuTau.push_back (string("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v1"));
    trigMuTau.push_back (string("HLT_IsoMu24_eta2p1_v1"));
    trigMuTau.push_back (string("HLT_IsoMu27_v1"));
    trigMuTau.push_back (string("HLT_IsoMu17_eta2p1"));

    trigEleTau.push_back (string("HLT_Ele22_eta2p1_WP75_Gsf_LooseIsoPFTau20_v1"));
    trigEleTau.push_back (string("HLT_Ele32_eta2p1_WP75_Gsf_v1"));
    trigEleTau.push_back (string("HLT_Ele22_eta2p1_WP75_Gsf"));

    trigTauTau.push_back (string("HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v1"));

    trigEleMu.push_back (string("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v1"));
    trigEleMu.push_back (string("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v1"));
    trigEleMu.push_back (string("HLT_IsoMu24_eta2p1_v1"));
    trigEleMu.push_back (string("HLT_IsoMu27_v1"));
 
    triggerReader trigReader (hTriggers);
    trigReader.addTauTauTrigs (trigTauTau);
    trigReader.addMuTauTrigs  (trigMuTau);
    trigReader.addEleTauTrigs (trigEleTau);
    trigReader.addMuEleTrigs  (trigEleMu);

    // ---------------------------------

    cout << "... starting" << endl;

    // --------------------------------

    TLorentzVector vDec1 (0,0,0,0);
    TLorentzVector vDec2 (0,0,0,0);
    TLorentzVector vReco1 (0,0,0,0); // matched with gen
    TLorentzVector vReco2 (0,0,0,0);
    TLorentzVector vRecoChosen1 (0,0,0,0); // chosen with our algo
    TLorentzVector vRecoChosen2 (0,0,0,0);
    
    TLorentzVector vBuf (0,0,0,0);

    vector<pair<float , int> > matchGen1_dR; // dR, idx
    vector<pair<float , int> > matchGen2_dR; // dR, idx

    int nPairsBoosted = 0; // when both gen are matched to the same reco objects

    // --------------------------------

    for (int iEv = 0; iEv < nEvents; iEv++)
    {
        theBigTree.GetEntry(iEv);
        if (iEv%10000 == 0) cout << iEv << " / " << nEvents << endl;
        //cout << iEv << " / " << nEvents << endl;

        // ---- gen
        int thisDecayMode = -1; // MC category of the event
        int idxdecProd1 = -1;
        int idxdecProd2 = -1;
        int idxTau1     = -1;
        int idxTau2     = -1;
        int pdg1        = -1;
        int pdg2        = -1;

        // ---- reco
        int idxreco1 = -1;
        int idxreco2 = -1;
        int MCMatchedPairidx  = -1;
        int MCMatchedPairType = -1;
        int dau1Matchedidx    = -1;
        int dau2Matchedidx    = -1;
        matchGen1_dR.clear();
        matchGen2_dR.clear();

        // for check
        hNDaughtersPreGen->Fill (theBigTree.daughters_px->size()) ;

        // triggers for this event
        Long64_t triggerbit = theBigTree.triggerbit;
        if (askThisTriggerType != -1)
        {
            bool hasTriggers = trigReader.checkOR (askThisTriggerType, triggerbit);
            if (!hasTriggers) continue;
        }

        // loop over gen particles and select leptons coming from Z decay
        for (unsigned int igen = 0; igen < theBigTree.genpart_pdg->size(); igen++)
        {
            int pdg = theBigTree.genpart_pdg->at(igen);
            int genflags = theBigTree.genpart_flags->at(igen);

            //cout << pdg << endl;
            if (abs(pdg) == PDGProcess ) // Z = 23, H = 25
            {
                int decayMode = theBigTree.genpart_HZDecayMode->at(igen);
                //if (decayMode == (int) Other) cout << " ** WARNING: Z decay unexpected" << endl;
                //cout << "Z: " << decayMode << endl;
                if (decayMode != (int) Other) thisDecayMode = decayMode;
            }
            
            if (abs(pdg) == 11 || abs(pdg) == 13 || abs(pdg) == 66615 ) // ele, mu, had taus
            {
                //vBuf.SetPxPyPzE (theBigTree.genpart_px->at(igen) , theBigTree.genpart_py->at(igen) , theBigTree.genpart_pz->at(igen) , theBigTree.genpart_e->at(igen)) ;
                //cout << igen << " pdg: " << pdg << " pT: " << vBuf.Pt() << " flag3: " << CheckBit (genflags, 3) << " flag5: " << CheckBit (genflags, 5) << " flag9: " << CheckBit (genflags, 9) << " flag10: " << CheckBit (genflags, 10) << endl;

                bool goodLep = CheckBit (genflags, 10) ; // 3 = isPromptTauDecayProduct , 5 = isDirectPromptTauDecayProduct , 9 = isHardProcessTauDecayProduct , 10 = isDirectHardProcessTauDecayProduct
                if (abs(pdg) == 66615) // tau h does not have falgs copied, neet to check its mother
                {
                    int tauhmoth = theBigTree.genpart_TauMothInd->at (igen);
                    int flagmoth = theBigTree.genpart_flags->at (tauhmoth) ;
                    goodLep = CheckBit (flagmoth , 8) ; // 8 = fromHardProcess
                }

                if (goodLep)
                {
                    if (idxdecProd1 == -1)
                    {
                        idxdecProd1 = igen;
                        pdg1 = pdg;
                    }
                    else if (idxdecProd2 == -1)
                    {
                        idxdecProd2 = igen;
                        pdg2 = pdg;
                    }
                    else cout << " ** WARNING: n tau decay prods from hard process is > 2" << endl;
                }
            }            

            if (abs(pdg) == 15) // original tau
            {
                bool isFromHardProcess = CheckBit (genflags, 8);
                if (isFromHardProcess)
                {
                    if (idxTau1 == -1) idxTau1 = igen;
                    else if (idxTau2 == -1) idxTau2 = igen; 
                    else cout << " ** WARNING: n taus from hard process is > 2" << endl;
                }
            }
        }

        if (thisDecayMode > 5 || thisDecayMode < 0) continue; // select only Z -> tau tau
        if (thisDecayMode > 2) continue ; // etau, mu tau, tau tau only

        // goodness checks
        if (idxTau1 == -1 || idxTau2 == -1)
        {
            cout << " ** WARNING: taus from decay not found " << idxTau1 << " , " << idxTau2 << endl;
            continue;
        }        
        if (idxdecProd1 == -1 || idxdecProd2 == -1)
        {
            cout << " ** WARNING: tau decay products from decay not found " << idxdecProd1 << " , " << idxdecProd2 << endl;
            continue;
        }
        if (DecayMode (pdg1, pdg2) != thisDecayMode)
        {
            cout << " ** WARNING: pdg of selected gen leptons does not match decay mode " << thisDecayMode << " != " << pdg1 << " , " << pdg2 << " -- " << DecayMode(pdg1, pdg2) << endl ;
        }

        vDec1.SetPxPyPzE (theBigTree.genpart_px->at(idxdecProd1) , theBigTree.genpart_py->at(idxdecProd1) , theBigTree.genpart_pz->at(idxdecProd1) , theBigTree.genpart_e->at(idxdecProd1)) ;
        vDec2.SetPxPyPzE (theBigTree.genpart_px->at(idxdecProd2) , theBigTree.genpart_py->at(idxdecProd2) , theBigTree.genpart_pz->at(idxdecProd2) , theBigTree.genpart_e->at(idxdecProd2)) ;

        if (!minimalGenCut (vDec1, pdg1)) continue; // minimal selections for detector acceptance
        if (!minimalGenCut (vDec2, pdg2)) continue; 

        hZDecMode->Fill (thisDecayMode); // BR checked, are OK

        //cout << "GEN 1: " << pdg1 << " pT: " << vDec1.Pt() << " eta: " << vDec1.Eta() << " Phi: " << vDec1.Phi() << endl;
        //cout << "GEN 2: " << pdg2 << " pT: " << vDec2.Pt() << " eta: " << vDec2.Eta() << " Phi: " << vDec2.Phi() << endl;

        // ------------------------------------------------------------------------
        // now loop on reco leptons and perform match
        //
        hNDaughters->Fill (theBigTree.daughters_px->size()) ;
 
        for (unsigned int idau = 0; idau < theBigTree.daughters_px->size(); idau++)
        {

            int thisAbsPDG = makePDG (theBigTree.particleType->at(idau)) ; // mu = 0; ele = 1, tau = 2 --> converted to pdg

            //vBuf.SetPxPyPzE (theBigTree.daughters_px->at(idau) , theBigTree.daughters_py->at(idau) , theBigTree.daughters_pz->at(idau) , theBigTree.daughters_e->at(idau)) ;
            //cout << "  --> " << idau << " ) id: " << thisAbsPDG << " pt: " << vBuf.Pt() << " eta: " << vBuf.Eta() << " phi: " << vBuf.Phi() << endl;
            
            if (thisAbsPDG == abs(pdg1)) // match only good types
            {
                vBuf.SetPxPyPzE (theBigTree.daughters_px->at(idau) , theBigTree.daughters_py->at(idau) , theBigTree.daughters_pz->at(idau) , theBigTree.daughters_e->at(idau)) ;
                float dR = vBuf.DeltaR(vDec1);
                if (dR < matchCone) matchGen1_dR.push_back (make_pair(dR, idau)); 
            }

            if (thisAbsPDG == abs(pdg2)) // match only good types
            {
                vBuf.SetPxPyPzE (theBigTree.daughters_px->at(idau) , theBigTree.daughters_py->at(idau) , theBigTree.daughters_pz->at(idau) , theBigTree.daughters_e->at(idau)) ;
                float dR = vBuf.DeltaR(vDec2);
                if (dR < matchCone) matchGen2_dR.push_back (make_pair(dR, idau)); 
            }
        }

        //cout << endl << " ================================================================== " << endl;

        // get best matching candidate
        sort (matchGen1_dR.begin(), matchGen1_dR.end() );
        sort (matchGen2_dR.begin(), matchGen2_dR.end() );

        hNMatches->Fill (makeParticleType (pdg1) , matchGen1_dR.size());
        hNMatches->Fill (makeParticleType (pdg2) , matchGen2_dR.size());

        /*
        cout << endl;
        cout << iEv << " / " << nEvents << endl;
        cout << "Size dR1, dR2: " << matchGen1_dR.size() << " " << matchGen2_dR.size() << endl;             
        */

        if (matchGen1_dR.size() > 0 && matchGen2_dR.size() > 0)
        {
            dau1Matchedidx = matchGen1_dR.at(0).second;
            dau2Matchedidx = matchGen2_dR.at(0).second;
            MCMatchedPairidx = getPairByIndexes (&theBigTree, dau1Matchedidx, dau2Matchedidx) ;

            if (MCMatchedPairidx == -1)
            {
                nPairsBoosted++;
                cout << " --> Evt: " << iEv << " . Both gen matched to the same reco. idx1,2 = " << dau1Matchedidx << " , " << dau2Matchedidx << "  -- gen dR = " << vDec1.DeltaR(vDec2) << endl; 
                continue;
            }

            MCMatchedPairType = getPairType (theBigTree.particleType->at(dau1Matchedidx) , theBigTree.particleType->at(dau2Matchedidx) ) ;
            bool isOS = theBigTree.isOSCand->at (MCMatchedPairidx) ;

            /*
            cout << "Matched idxs: " << dau1Matchedidx << " " << dau2Matchedidx << " --> " << MCMatchedPairidx << endl;             
            cout << "    --> pairType     : " << MCMatchedPairType << endl;
            cout << "    --> type   1, 2  : " << theBigTree.particleType->at(dau1Matchedidx) << " , " << theBigTree.particleType->at(dau2Matchedidx) << endl;
            cout << "    --> reliso 1 , 2 : " << theBigTree.combreliso->at(dau1Matchedidx) << " , " << theBigTree.combreliso->at(dau2Matchedidx) << endl;
            cout << "    --> tauiso 1 , 2 : " << theBigTree.daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits ->at(dau1Matchedidx) << " , " << theBigTree.daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits->at(dau2Matchedidx) << endl;
            */

            if (isOS) hMatchedPairIsOS -> Fill (MCMatchedPairType, 1) ;
            else      hMatchedPairIsOS -> Fill (MCMatchedPairType, 0) ;

            // single lepton charge
            int dau1Charge = theBigTree.daughters_charge->at(dau1Matchedidx) ;
            int dau2Charge = theBigTree.daughters_charge->at(dau2Matchedidx) ;
            int MC1Charge  = pdg1 / abs(pdg1); 
            int MC2Charge  = pdg2 / abs(pdg2);

            //cout << dau1Charge << " " << dau2Charge << " || " << MC1Charge << " " << MC2Charge << endl;
            // get only sign
            dau1Charge /= abs(dau1Charge);
            dau2Charge /= abs(dau2Charge);
            MC1Charge *= -1;
            MC2Charge *= -1; // pdg id + --> ele -, mu - , ...

            //cout << "   --> " << dau1Charge << " " << dau2Charge << endl;

            if (dau1Charge / MC1Charge == 1) hRecoDauCharge->Fill (makeParticleType (pdg1) , 1) ;
            else hRecoDauCharge->Fill (makeParticleType (pdg1) , 0) ;

            if (dau2Charge / MC2Charge == 1) hRecoDauCharge->Fill (makeParticleType (pdg2) , 1) ;
            else hRecoDauCharge->Fill (makeParticleType (pdg2) , 0) ;

            // never happens :-)
            if (dau1Charge * dau2Charge == -1 && !isOS ) cout << " ** isOS!! dau1, dau2 = " << dau1Charge << " , " << dau2Charge  << " -- isOS: " << isOS << endl;
            if (dau1Charge * dau2Charge == 1  && isOS )  cout << " ** isOS!! dau1, dau2 = " << dau1Charge << " , " << dau2Charge  << " -- isOS: " << isOS << endl;
            if (MC1Charge * MC2Charge != -1) cout << " ** two MC leptons have same charge?? " << MC1Charge << " " << MC2Charge << endl;
        }


        // ------------------------------------ pair choice based on most iso as from sync
        //map<int, int> foundPairs ; // pairType, pairIndexInVectors
        
        // decide flavour based on trigger
        //bool ORtrigs = trigReader.checkOR (pairType, triggerbit);
        bool isMuTau = trigReader.checkOR (0, triggerbit);
        bool isEleTau = trigReader.checkOR (1, triggerbit);
        bool isTauTau = trigReader.checkOR (2, triggerbit);
        int flavFromTrigger = -999;
        if (isMuTau) flavFromTrigger = 0;
        else if (isEleTau) flavFromTrigger = 1;
        else if (isTauTau) flavFromTrigger = 2; 


        vector<vector<int> > foundPairs (6, vector<int>(0)) ; // pairType is high-level index, then store indexes of individual pairs inside
        //int goodPairs = 0;
        for (unsigned int iPair = 0 ; iPair < theBigTree.indexDau1->size () ; ++iPair)
        {
          // FIXME need to implement here the choice of iso / anti-iso
          if (!oph.pairPassBaseline (&theBigTree, iPair, leptonSelectionFlag.c_str ())) continue ;
          //goodPairs++;
          int firstDaughterIndex = theBigTree.indexDau1->at (iPair) ;  
          int secondDaughterIndex = theBigTree.indexDau2->at (iPair) ;
          TLorentzVector tlv_firstLepton
            (
              theBigTree.daughters_px->at (firstDaughterIndex),
              theBigTree.daughters_py->at (firstDaughterIndex),
              theBigTree.daughters_pz->at (firstDaughterIndex),
              theBigTree.daughters_e->at (firstDaughterIndex)
            ) ;
          TLorentzVector tlv_secondLepton
            (
              theBigTree.daughters_px->at (secondDaughterIndex),
              theBigTree.daughters_py->at (secondDaughterIndex),
              theBigTree.daughters_pz->at (secondDaughterIndex),
              theBigTree.daughters_e->at (secondDaughterIndex)
            ) ;
          //if (isDegenere (tlv_firstLepton, tlv_secondLepton)) continue; // antiEle and antiMu should already do this dirty job
          int type1 = theBigTree.particleType->at (firstDaughterIndex) ;
          int type2 = theBigTree.particleType->at (secondDaughterIndex) ;        
          int pairType = oph.getPairType (type1, type2) ;
          
          // -------------- check TRIGGERS --------------------------------
          bool ORtrigs = trigReader.checkOR (pairType, triggerbit);
          if (askTriggers)
          {
            if (!ORtrigs) continue;
          }

          if (askFlavourFromTriggers)
          {
            if (pairType != flavFromTrigger) continue;
          }
          
          foundPairs.at(pairType).push_back (iPair);
          
          //cout << iPair << " - reco pair Chosen: " << tlv_firstLepton.Pt() << " " << tlv_secondLepton.Pt() << endl;

          //if (foundPairs.find (pairType) != foundPairs.end ()) continue ;          
          //foundPairs[pairType] = iPair ;
        }

        int chosenTauPair = -1;
        int chosenTauPairType = -1;
        int idxChosenDau1 = -1;
        int idxChosenDau2 = -1;
        int typeChosenDau1 = -1;
        int typeChosenDau2 = -1;
        // if (foundPairs.size () == 0) continue ;
        for (int i = 0; i < 6; i++)
        {
            if (foundPairs.at(i).size() > 0 )
            {
                chosenTauPair = foundPairs.at(i).at(0);
                chosenTauPairType = i;
                idxChosenDau1 = theBigTree.indexDau1->at(chosenTauPair);
                idxChosenDau2 = theBigTree.indexDau2->at(chosenTauPair);
                typeChosenDau1 = theBigTree.particleType->at (idxChosenDau1);
                typeChosenDau2 = theBigTree.particleType->at (idxChosenDau2);
                vRecoChosen1.SetPxPyPzE (theBigTree.daughters_px->at(idxChosenDau1), theBigTree.daughters_py->at(idxChosenDau1), theBigTree.daughters_pz->at(idxChosenDau1), theBigTree.daughters_e->at(idxChosenDau1) );
                vRecoChosen2.SetPxPyPzE (theBigTree.daughters_px->at(idxChosenDau2), theBigTree.daughters_py->at(idxChosenDau2), theBigTree.daughters_pz->at(idxChosenDau2), theBigTree.daughters_e->at(idxChosenDau2) );
                break;
            }
        }

        // --------------------------- third lepton veto
              // loop over leptons
        vector<pair<TLorentzVector, float> > dummyLeptCollection ;
        for (unsigned int iLep = 0 ; 
           (iLep < theBigTree.daughters_px->size ()) ;
           ++iLep)
        {
          // skip the H decay candiates
          if (int (iLep) == idxChosenDau1 || 
              int (iLep) == idxChosenDau2) continue ;

          // remove taus
          if (theBigTree.particleType->at (iLep) == 2)
            {
              continue ;
            }  
          else if (theBigTree.particleType->at (iLep) == 0) // muons
            {
              if (!oph.muBaseline (&theBigTree, iLep, 10., 2.4, 0.3)) continue ;
            }
          else if (theBigTree.particleType->at (iLep) == 1) // electrons
            {
              if (!oph.eleBaseline (&theBigTree, iLep, 10., 0.3, 1)) continue ;
//              if (!oph.eleBaseline (&theBigTree, iLep, 10., 2.5, 0.3, 1)) continue ;
            }
          TLorentzVector tlv_dummyLepton
            (
              theBigTree.daughters_px->at (iLep),
              theBigTree.daughters_py->at (iLep),
              theBigTree.daughters_pz->at (iLep),
              theBigTree.daughters_e->at (iLep)
            ) ;
          dummyLeptCollection.push_back (pair<TLorentzVector, float> (
              tlv_dummyLepton, 
              theBigTree.daughters_charge->at (iLep) * 
              (theBigTree.particleType->at (iLep) + 1)              
            )) ;
          
        } // loop over leptons

      
        int nleps = dummyLeptCollection.size();
        if (ask3rdlepVeto && nleps > 0) continue;

      //sort (dummyLeptCollection.rbegin (), dummyLeptCollection.rend (), leptSort ()) ;
      // ----------------------------------------------------

        //int isChosenOS = theBigTree.isOSCand->at (chosenTauPair) ;
        // now I have the chosen tau pair, compare it with the MC truth
        vector <int> allFoundPairs; // copy of foundPairs to search in an easier way
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < foundPairs.at(i).size(); j++)
            {
                allFoundPairs.push_back (foundPairs.at(i).at(j)); // should not contain doubles as it is divided by pair type so exclusive
            }
        }
        std::vector<int>::iterator it = find (allFoundPairs.begin(), allFoundPairs.end(), MCMatchedPairidx) ;
        bool goodPairPassesSelection = (it != allFoundPairs.end());
        bool goodPairIsChosen = (MCMatchedPairidx == chosenTauPair && MCMatchedPairidx != -1);
        bool somethingChosen  = (chosenTauPair != -1);

        if (goodPairIsChosen)             hPairChosenStat->Fill (thisDecayMode, 0);
        else if (goodPairPassesSelection) hPairChosenStat->Fill (thisDecayMode, 1);
        else if (somethingChosen)         hPairChosenStat->Fill (thisDecayMode, 2);
        else                              hPairChosenStat->Fill (thisDecayMode, 3);

        if (somethingChosen)
        {
            bool isChosenOS = theBigTree.isOSCand->at (chosenTauPair) ;
            if (isChosenOS) hChosenPairIsOS->Fill (chosenTauPairType, 1) ;
            else hChosenPairIsOS->Fill (chosenTauPairType, 0) ;
            hMCvsChosenPairType->Fill (thisDecayMode, chosenTauPairType);
        
            if (isChosenOS) hIsOS[thisDecayMode]->Fill(1);
            else            hIsOS[thisDecayMode]->Fill(0);

        }
        else
        {
            hMCvsChosenPairType->Fill (thisDecayMode, 6);
        }

        //cout << "Good pair passes selections? " << goodPairPassesSelection << endl;

        // ------------ fill histos with kinematics ----------------------
        
        // --- MC
        hPt_MC[makeParticleType(pdg1)][0] -> Fill (vDec1.Pt());
        hPt_MC[makeParticleType(pdg2)][0] -> Fill (vDec2.Pt());

        if (goodPairPassesSelection)
        {
            hPt_MC[makeParticleType(pdg1)][1] -> Fill (vDec1.Pt());
            hPt_MC[makeParticleType(pdg2)][1] -> Fill (vDec2.Pt());
        }

        // --- reco
        if (MCMatchedPairType != -1) // match
        {
            int matchedDau1Type = theBigTree.particleType->at(dau1Matchedidx);
            int matchedDau2Type = theBigTree.particleType->at(dau2Matchedidx);
            
            float iso1 = (matchedDau1Type == 2 ? theBigTree.daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits->at(dau1Matchedidx) : theBigTree.combreliso->at(dau1Matchedidx));
            float iso2 = (matchedDau2Type == 2 ? theBigTree.daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits->at(dau2Matchedidx) : theBigTree.combreliso->at(dau2Matchedidx));
            
            hIso_reco [matchedDau1Type][0] -> Fill (iso1);
            hIso_reco [matchedDau2Type][0] -> Fill (iso2);
            if (goodPairPassesSelection)
            {
                hIso_reco [matchedDau1Type][1] -> Fill (iso1);
                hIso_reco [matchedDau2Type][1] -> Fill (iso2);                
            }
        }

    }

    // print results
    cout << endl;
    cout << " ************* MATCHING ************* " << endl;
    cout << "Fraction of events with both gen matched to same reco: " << nPairsBoosted << " / " << nEvents << " = " << 100.*nPairsBoosted/nEvents << " %" << endl;

    cout << endl;
    cout << " ************* RECONSTRUCTION EFFICIENCY ************* " << endl;
    cout << "Muons:     " << 100. * hNMatches -> Integral (1,1, 2,hNMatches->GetNbinsY()) / hNMatches -> Integral (1,1, 1,hNMatches->GetNbinsY())  << " %" << endl;
    cout << "Electrons: " << 100. * hNMatches -> Integral (2,2, 2,hNMatches->GetNbinsY()) / hNMatches -> Integral (2,2, 1,hNMatches->GetNbinsY())  << " %" << endl;
    cout << "Tau:       " << 100. * hNMatches -> Integral (3,3, 2,hNMatches->GetNbinsY()) / hNMatches -> Integral (3,3, 1,hNMatches->GetNbinsY())  << " %" << endl;

    cout << endl;
    cout << " ************* CHARGE RECONSTRUCTION OS (MC matched pair) ************* " << endl;
    cout << "Mu Tau:  " << 100. * hMatchedPairIsOS -> Integral (1,1, 2,hMatchedPairIsOS->GetNbinsY()) / hMatchedPairIsOS -> Integral (1,1, 1,hMatchedPairIsOS->GetNbinsY())  << " %" << endl;
    cout << "Ele Tau: " << 100. * hMatchedPairIsOS -> Integral (2,2, 2,hMatchedPairIsOS->GetNbinsY()) / hMatchedPairIsOS -> Integral (2,2, 1,hMatchedPairIsOS->GetNbinsY())  << " %" << endl;
    cout << "Tau Tau: " << 100. * hMatchedPairIsOS -> Integral (3,3, 2,hMatchedPairIsOS->GetNbinsY()) / hMatchedPairIsOS -> Integral (3,3, 1,hMatchedPairIsOS->GetNbinsY())  << " %" << endl;
    cout << "Ele Mu:  " << 100. * hMatchedPairIsOS -> Integral (5,5, 2,hMatchedPairIsOS->GetNbinsY()) / hMatchedPairIsOS -> Integral (5,5, 1,hMatchedPairIsOS->GetNbinsY())  << " %" << endl;

    cout << endl;
    cout << " ************* CHARGE RECONSTRUCTION OS (lepton) ************* " << endl;
    cout << "Muons:     " << 100. * hRecoDauCharge -> Integral (1,1, 2,hRecoDauCharge->GetNbinsY()) / hRecoDauCharge -> Integral (1,1, 1,hRecoDauCharge->GetNbinsY())  << " %" << endl;
    cout << "Electrons: " << 100. * hRecoDauCharge -> Integral (2,2, 2,hRecoDauCharge->GetNbinsY()) / hRecoDauCharge -> Integral (2,2, 1,hRecoDauCharge->GetNbinsY())  << " %" << endl;
    cout << "Tau:       " << 100. * hRecoDauCharge -> Integral (3,3, 2,hRecoDauCharge->GetNbinsY()) / hRecoDauCharge -> Integral (3,3, 1,hRecoDauCharge->GetNbinsY())  << " %" << endl;

    // ------------------- save to file
    fOut->cd();
    hZDecMode->Write();
    hNMatches->Write();
    hNDaughters->Write();
    hNDaughtersPreGen->Write();
    hMatchedPairIsOS->Write();
    hRecoDauCharge->Write();
    hPairChosenStat->Write();
    hChosenPairIsOS->Write();
    hMCvsChosenPairType->Write();
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 2; j++)
            hPt_MC[i][j]->Write();
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 2; j++)
            hIso_reco[i][j]->Write();
    for (int i = 0; i < 6; i++) hIsOS[i]->Write();
}
