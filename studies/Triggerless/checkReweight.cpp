#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "TBranch.h"
#include "TString.h"
#include "TLorentzVector.h"
#include "bigTree.h" 
#include "OfflineProducerHelper.h"
#include "triggerReader.h"

using namespace std;

// apply 
// c++ -lm -o checkReweight checkReweight.cpp ../../src/OfflineProducerHelper.cc ../../src/triggerReader.cc -I ../../interface/ `root-config --glibs --cflags`

float getTriggerWeight(int partType, float pt, TH1F* weightHisto)
{
    if (partType == 0) return 0.95;
    else if (partType == 1) return 0.95;
    else if (partType == 2)
    {
        int ibin = weightHisto->FindBin(pt);
        if (ibin < 1) ibin = 1;
        if (ibin > weightHisto->GetNbinsX()) ibin = weightHisto->GetNbinsX() ;
        return weightHisto->GetBinContent(ibin);
    }
    cout << "** WARNING: trigger weight now known for particle type " << partType << endl;
    return 1.;
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
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


int main()
{
    bool askGetMatch = true;
    bool askIsoInBaseline = true;

    cout << endl;
    cout << "** asking gen match? " << askGetMatch << endl;
    cout << "** asking iso in baseline? " << askIsoInBaseline << endl;
    cout << endl;
    cout << ".. start, opening files" << endl;
    string filelist = "Radion700_fixMatch.txt";
    TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;
    appendFromFileList (bigChain, filelist);
    bigTree theBigTree (bigChain) ;
    cout << ".. files opened" << endl;

    TH1F* hTriggers = getFirstFileHisto (filelist);
    TH1F* hTauIDS = getFirstFileHisto (filelist,false);
    OfflineProducerHelper oph (hTriggers, hTauIDS) ;

    TFile* fOut = new TFile ("reweightTree.root", "recreate");
    TTree* tOut = new TTree("triggerlessTree", "triggerlessTree");
    int triggerAccept;
    float evtWeight;
    float evtLeg1weight;
    float evtLeg2weight;
    int pairType ;
    float pt1, pt2;

    tOut->Branch("triggerAccept", &triggerAccept);
    tOut->Branch("evtWeight",     &evtWeight);
    tOut->Branch("evtLeg1weight", &evtLeg1weight);
    tOut->Branch("evtLeg2weight", &evtLeg2weight);
    tOut->Branch("pairType",      &pairType);
    tOut->Branch("pt1",           &pt1);
    tOut->Branch("pt2",           &pt2);

    // to check trigger match flag
    triggerReader trigReader (hTriggers);

    vector<string> trigMuTau;   trigMuTau  .push_back("HLT_IsoMu18_v");
    vector<string> trigEleTau;  trigEleTau .push_back("HLT_Ele23_WPLoose_Gsf_v");
    vector<string> trigTauTau;  trigTauTau .push_back("HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg_v");

    trigReader.addMuTauTrigs  (trigMuTau);
    trigReader.addEleTauTrigs (trigEleTau);
    trigReader.addTauTauTrigs (trigTauTau);
  
    TFile* fHistoTriggerRew = new TFile ("/home/llr/cms/cadamuro/TauTagAndProbe/CMSSW_7_6_3/src/TauTagAndProbe/TauTagAndProbe/test/turnOn_2GevIso.root");
    TH1F* weightHisto = (TH1F*) fHistoTriggerRew->Get("hTurnOn");

    int totAccDATA[3] = {0,0,0};
    float totAccMC[3] = {0,0,0};

    for (Long64_t iEvent = 0 ; true ; ++iEvent) 
    {
        if (iEvent % 10000 == 0)  cout << "reading event " << iEvent << endl ;

        int got = theBigTree.fChain->GetEntry(iEvent);
        if (got == 0) break;

        // loop over the daughters to select pair type: mu > e > tau
        // apply tight baseline (with iso to check)
        int nmu = 0;
        int nele = 0;
        // int ntau = 0;
        for (unsigned int idau = 0; idau < theBigTree.daughters_px->size(); ++idau)
        {
            int dauType = theBigTree.particleType->at(idau);
            if (oph.isMuon(dauType))
            {
                if (oph.muBaseline (&theBigTree, idau, 19., 2.1, 0.1, string("All")) ) ++nmu;
            }
            else if (oph.isElectron(dauType))
            {
                if (oph.eleBaseline (&theBigTree, idau, 24., 0.1, 0, string("All")) ) ++nele;
            }
        }
        pairType = 2; // tau tau
        if (nmu > 0) pairType = 0 ; // mu tau
        else if (nele > 0) pairType = 1 ; // ele tau

        /////////////////////////////
        ////////////// choose the first pair passing baseline and being of the right pair type
        /////////////////////////////

        int pairIndex = -1;
        for (unsigned int iPair = 0 ; iPair < theBigTree.indexDau1->size () ; ++iPair)
        {
            int t_firstDaughterIndex  = theBigTree.indexDau1->at (iPair) ;  
            int t_secondDaughterIndex = theBigTree.indexDau2->at (iPair) ;
            int t_type1 = theBigTree.particleType->at (t_firstDaughterIndex) ;
            int t_type2 = theBigTree.particleType->at (t_secondDaughterIndex) ;        
            if ( oph.getPairType (t_type1, t_type2) != pairType ) continue ;
        
            if (askGetMatch)
            {
                
                TLorentzVector t_vdau1 (
                      theBigTree.daughters_px->at (t_firstDaughterIndex),
                      theBigTree.daughters_py->at (t_firstDaughterIndex),
                      theBigTree.daughters_pz->at (t_firstDaughterIndex),
                      theBigTree.daughters_e->at (t_firstDaughterIndex)
                );

                TLorentzVector t_vdau2 (
                      theBigTree.daughters_px->at (t_secondDaughterIndex),
                      theBigTree.daughters_py->at (t_secondDaughterIndex),
                      theBigTree.daughters_pz->at (t_secondDaughterIndex),
                      theBigTree.daughters_e->at (t_secondDaughterIndex)
                );

                bool match1 = false;
                bool match2 = false;
                
                int idToMatch1 = 66615;
                if (t_type1 == 0) idToMatch1 = 13;
                else if (t_type1 == 1) idToMatch1 = 11;

                int idToMatch2 = 66615;
                if (t_type2 == 0) idToMatch2 = 13;
                else if (t_type2 == 1) idToMatch2 = 11;

                for (int igen = 0; igen < theBigTree.genpart_px->size(); ++igen)
                {
                    TLorentzVector t_vgen (
                          theBigTree.genpart_px->at (igen),
                          theBigTree.genpart_py->at (igen),
                          theBigTree.genpart_pz->at (igen),
                          theBigTree.genpart_e->at (igen)
                    );
                    int agenId = abs(theBigTree.genpart_pdg->at(igen));

                    // match to 1
                    if (agenId == idToMatch1 && t_vgen.DeltaR(t_vdau1) < 0.3) match1 = true;

                    // match to 2
                    if (agenId == idToMatch2 && t_vgen.DeltaR(t_vdau2) < 0.3) match2 = true;
                    
                    // skip if done
                    if (match1 && match2) continue;
                }
                if (!match1 || !match2) continue;   
            }

            string whatApply = askIsoInBaseline ? "All" : "Vertex-LepID-pTMin-etaMax-againstEle-againstMu" ;
            // if ( oph.pairPassBaseline (&theBigTree, iPair, string("Vertex-LepID-pTMin-etaMax-againstEle-againstMu") ) )
            if ( oph.pairPassBaseline (&theBigTree, iPair, whatApply ) )
            {
                pairIndex = iPair;
                break;
            }
        }
        if (pairIndex < 0) continue; // no pair found over baseline

        /////////////////////////////
        ////////////// trigger reweight vs trigger matching
        /////////////////////////////

        int firstDaughterIndex  = theBigTree.indexDau1->at (pairIndex) ;  
        int secondDaughterIndex = theBigTree.indexDau2->at (pairIndex) ;
        int type1 = theBigTree.particleType->at (firstDaughterIndex) ;
        int type2 = theBigTree.particleType->at (secondDaughterIndex) ;        

        TLorentzVector vdau1 (
              theBigTree.daughters_px->at (firstDaughterIndex),
              theBigTree.daughters_py->at (firstDaughterIndex),
              theBigTree.daughters_pz->at (firstDaughterIndex),
              theBigTree.daughters_e->at (firstDaughterIndex)
        );

        TLorentzVector vdau2 (
              theBigTree.daughters_px->at (secondDaughterIndex),
              theBigTree.daughters_py->at (secondDaughterIndex),
              theBigTree.daughters_pz->at (secondDaughterIndex),
              theBigTree.daughters_e->at (secondDaughterIndex)
        );

        pt1 = vdau1.Pt();
        pt2 = vdau2.Pt();

        //  -----  DATA STRATEGY
        Long64_t triggerbit = theBigTree.triggerbit;
        bool passTrg = trigReader.checkOR (pairType, triggerbit) ;
        
        Long64_t matchFlag1 = (Long64_t) theBigTree.daughters_L3FilterFired->at(firstDaughterIndex);
        Long64_t matchFlag2 = (Long64_t) theBigTree.daughters_L3FilterFired->at(secondDaughterIndex);
        bool passMatch1 = false;
        bool passMatch2 = false;
        if (pairType == 0 || pairType == 1)
        {
            passMatch1 = trigReader.checkOR (pairType, matchFlag1) ;
            passMatch2 = true;
        }
        else if (pairType == 2)
        {
            passMatch1 = trigReader.checkOR (pairType, matchFlag1) ;
            passMatch2 = trigReader.checkOR (pairType, matchFlag2) ;            
        }

        triggerAccept = (passTrg && passMatch1 && passMatch2) ? 1 : 0 ;
        if (triggerAccept == 1) totAccDATA[pairType] += 1 ;
        //  -----  MC STRATEGY
        evtLeg1weight = 1.0;
        evtLeg2weight = 1.0;

        if (pairType == 0 || pairType == 1)
        {
            evtLeg1weight = getTriggerWeight(type1, vdau1.Pt(), weightHisto) ;
            evtLeg2weight = 1.0;
        }
        else if (pairType == 2)
        {
            evtLeg1weight = getTriggerWeight(type1, vdau1.Pt(), weightHisto) ;
            evtLeg2weight = getTriggerWeight(type2, vdau2.Pt(), weightHisto) ;
        }
        evtWeight = evtLeg1weight*evtLeg2weight;
        totAccMC[pairType] += evtWeight;

        /////////////////////////////
        ////////////// fill output
        /////////////////////////////
        tOut->Fill();
    }

    cout << "=== Mu Tau === " << endl; 
    cout << "ACCEPTED DATA: " << totAccDATA[0] << endl;
    cout << "ACCEPTED MC  : " << totAccMC[0] << endl;
    cout << endl;

    cout << "=== E Tau === " << endl; 
    cout << "ACCEPTED DATA: " << totAccDATA[1] << endl;
    cout << "ACCEPTED MC  : " << totAccMC[1] << endl;
    cout << endl;

    cout << "=== Tau Tau === " << endl; 
    cout << "ACCEPTED DATA: " << totAccDATA[2] << endl;
    cout << "ACCEPTED MC  : " << totAccMC[2] << endl;
    cout << endl;

    fOut->cd();
    tOut->Write();

}