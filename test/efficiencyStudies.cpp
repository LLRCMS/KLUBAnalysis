/*
** Study efficiency.
** Select the "good" reco pair via a geometrical matching with the gen particles
** and see what it the effect of the selection on them.
*/

#include "bigTree.h"
#include "OfflineProducerHelper.h"
#include <iostream>
#include <iomanip>
#include "TFile.h"
#include "TLorentzVector.h"
#include "TH1D.h"

using namespace std;

// merge array of selection strings
void Concatenate (TString* sel, int nsel, TString* selConc)
{
    for (int i = 0; i < nsel; i++)
    {
        TString thisName = "";
        for (int j = 0; j <=i; j++)
        {
            thisName += sel[j];
            thisName += ";" ;
        }
        selConc [i] = thisName;
    }
    return;
}

bool ptGenSel (TLorentzVector& v, int pdgId)
{
    if (abs(pdgId) == 11) // electron
        return (v.Pt() > 10);
    if (abs(pdgId) == 13) // muon
        return (v.Pt() > 5);
    if (abs(pdgId) == 66615)
        return (v.Pt() > 20);
    cout << "What is this pdgId??" << endl;
    return false;
}

int getHistoIndex (int pdgId)
{
    if (abs(pdgId) == 11) return 1;
    if (abs(pdgId) == 13) return 0;
    if (abs(pdgId) == 66615) return 2;
    return -1;
}

int main()
{

    // ------------------------------------------------------
    // input files reading
    // ------------------------------------------------------
    TString pathToFile = "/data_CMS/cms/cadamuro/test_submit_to_tier3/";

    // lambda 20
    TString fIn = pathToFile + "HiggsTauTauOutput_HH_Lambda20_NoSvfit_Prod5Lug2015_300000Events_0Skipped_1436090974.69/HH_Lambda20_NoSVFit_5Lug2015.root";

    // lambda -4
    //TString fIn = pathToFile + "HiggsTauTauOutput_HH_Lambdam4_NoSvFit_Prod5Lug2015_300000Events_0Skipped_1436091004.38/HH_Lambdam4_NoSVFit_5Lug2015.root";
 
    // lambda 2.46
    //TString fIn = pathToFile + "HiggsTauTauOutput_HH_Lambda2dot46_NoSvFit_Prod5Lug2015_300000Events_0Skipped_1436090991.56/HH_Lambda2dot46_NoSVFit_5Lug2015.root";

    // lambda 1, only 174k entries!
    //TString fIn = pathToFile + "HiggsTauTauOutput_HH_Lambda1_Fix_10Lug2015_300000Events_0Skipped_1436545696.05/HH_Lambda1_Fix.root";

    cout << "Reading file: " << fIn << endl;

    TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;
    bigChain->Add (fIn) ;
    bigTree* tree = new bigTree(bigChain);

    
    // ------------------------------------------------------
    // prepare categories and selections
    // ------------------------------------------------------

    const char * decModeNames[6] = {"MuTau", "ETau", "TauTau", "MuMu", "EE", "EMu"}; // follows pairType enum
    OfflineProducerHelper helper;

    TString sel[] = {"Vertex", "OSCharge", "LepID", "pTMin", "etaMax", "againstEle", "againstMu", "Iso"};
    const int nsel = sizeof(sel)/sizeof(TString);
    TString selConc[nsel];
    Concatenate (sel, nsel, selConc);

    int nEvents = tree->fChain->GetEntries () ;
    //int nEvents = 1000;
    //cout << nEvents << endl;


    // ------------------------------------------------------
    // histograms and counters
    // ------------------------------------------------------
   
    int failedFindMC = 0; // counts events in which final Higgs decay products could not be found
    int matchNF = 0; // number of gen matches not found
    int pairNF = 0; // number of pairs not found from dau indexes (must be 0)
    const int extraSel = 3; // count input events, acceptance and reco match events
    int passSelections [6][nsel+extraSel]; // first index is MC basket, second one is selection step. nsel selections + input events + acceptance + reco gen match

    // to fill before gen acceptance
    TFile* fOut = new TFile ("plotsEfficiency.root", "recreate");    
    TH1D* etaMu = new TH1D ("etaMu", "etaMu", 100, -4, 4);
    TH1D* etaEle = new TH1D ("etaEle", "etaEle", 100, -4, 4);
    TH1D* etaTauh = new TH1D ("etaTauh", "etaTauh", 100, -4, 4);
    TH1D* ptMu = new TH1D ("ptMu", "ptMu", 100, 0, 200);
    TH1D* ptEle = new TH1D ("ptEle", "ptEle", 100, 0, 200);
    TH1D* ptTauh = new TH1D ("ptTauh", "ptTauh", 100, 0, 200);
    TH1D* etaGenHisto[3] = {etaMu, etaEle, etaTauh};
    TH1D* ptGenHisto[3] = {ptMu, ptEle, ptTauh};

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < nsel+extraSel; j++)
            passSelections[i][j] = 0;
    
    // ------------------------------------------------------
    // loop on events
    // ------------------------------------------------------
   
    for (int iEv = 0; iEv < nEvents; iEv++)
    {
        tree->GetEntry(iEv);
        if (iEv%50000 == 0) cout << iEv << " / " << nEvents << endl;
        
        // determine MC category
        int MCtype = helper.MCHiggsTauTauDecayMode(tree);
        
        // get final products coming from hard scatter process
        int genVis1 = -1;
        int genVis2 = -1;
        
        if (!helper.getHardTauFinalVisGenProducts(tree, genVis1, genVis2))
        {
            ++failedFindMC;
            continue;
        }
        // ==== 0 ====
        passSelections[MCtype][0]++;

        /*
        // testato, funziona bene e trova sempre la coppia giusta
        int nele = 0;
        int nmu = 0;
        if (abs(tree->genpart_pdg->at(genVis1)) == 11) nele++;
        if (abs(tree->genpart_pdg->at(genVis2)) == 11) nele++;
        if (abs(tree->genpart_pdg->at(genVis1)) == 13) nmu++;
        if (abs(tree->genpart_pdg->at(genVis2)) == 13) nmu++;
        
        int type = -1;
        if (nmu == 1 && nele == 0) type = 0;
        if (nmu == 0 && nele == 1) type = 1;
        if (nmu == 0 && nele == 0) type = 2;
        if (nmu == 2 && nele == 0) type = 3;
        if (nmu == 0 && nele == 2) type = 4;
        if (nmu == 1 && nele == 1) type = 5;

        if (type != MCtype)
            cout << decModeNames[MCtype] << " " << tree->genpart_pdg->at(genVis1) << " " << tree->genpart_pdg->at(genVis2) << endl; 
        */
        
        // ==== 1 ====
        // minimal selections on gen level
        TLorentzVector genV1 (helper.buildGenP4(tree, genVis1));        
        int genPdgId1 = tree->genpart_pdg->at(genVis1);
        TLorentzVector genV2 (helper.buildGenP4(tree, genVis2));        
        int genPdgId2 = tree->genpart_pdg->at(genVis2);
  
        etaGenHisto [getHistoIndex(genPdgId1)] -> Fill (genV1.Eta());        
        etaGenHisto [getHistoIndex(genPdgId2)] -> Fill (genV2.Eta());        
        ptGenHisto  [getHistoIndex(genPdgId1)] -> Fill (genV1.Pt());        
        ptGenHisto  [getHistoIndex(genPdgId2)] -> Fill (genV2.Pt());        

        
        if (TMath::Abs(genV1.Eta()) > 2.5 || TMath::Abs(genV2.Eta()) > 2.5) continue;
        if ( !ptGenSel(genV1, genPdgId1) || !ptGenSel(genV2, genPdgId2) ) continue;
        
        passSelections[MCtype][1]++;
        
        // ==== 2 ====
        // find matching reco leptons - last 3 vars are checkID, checkCharge, drCont
        int matchedVis1 = helper.getRecoMatchedToGen (tree, genVis1, true, true, 0.5);
        int matchedVis2 = helper.getRecoMatchedToGen (tree, genVis2, true, true, 0.5);
        
        if (matchedVis1 == -1 || matchedVis2 == -1)
        {
            ++matchNF;
            continue;
            /*
            cout << endl << endl << endl;
            cout << "ID1: " << genPdgId1 << " == pt: " << genV1.Pt() << " eta: " << genV1.Eta() << " phi: " << genV1.Phi() << endl;
            cout << "ID2: " << genPdgId2 << " == pt: " << genV2.Pt() << " eta: " << genV2.Eta() << " phi: " << genV2.Phi() << endl;
            cout << "Matched: " << matchedVis1 << " " << matchedVis2 << endl;
            for (int iReco = 0; iReco < tree->daughters_px->size(); iReco++)
            {
                TLorentzVector reco (tree->daughters_px->at(iReco), tree->daughters_py->at(iReco), tree->daughters_pz->at(iReco), tree->daughters_e->at(iReco));
                cout << iReco << ") - ID: " << tree->PDGIdDaughters->at(iReco) << " pt: " << reco.Pt() << " eta: " << reco.Eta() << " phi: " << reco.Phi() << endl;
                
            }
            */            
        }
        
        passSelections[MCtype][2]++;

        
        // get pair given by the match
        int iPair = helper.getPairByIndexes (tree, matchedVis1, matchedVis2);
        if (iPair == -1)
        {
            ++pairNF;
            cout << "Pair not found - this should never happen" << endl;
            continue;
        }
        
        // ==== 3 and more... ====
        // start to apply selections
        for (int iSel = 0; iSel < nsel; iSel++)
        {
            bool PairPassBaseline = helper.pairPassBaseline (tree, iPair, selConc[iSel]);
            if (PairPassBaseline)
            {
                passSelections[MCtype][iSel+extraSel]++;
            }
        }
        
    }
    

    // ------------------------------------------------------
    // print nice output 
    // ------------------------------------------------------
   cout << "FINISH" << endl;
   
    TString selNames [nsel+extraSel];
    selNames[0] = "Input events";
    selNames[1] = "Gen acceptance";
    selNames[2] =  "Reco-gen match";
    for (int i = 0; i < nsel; i++) selNames [i+extraSel] = sel[i];

    cout << " =============== SUMMARY ==============" << endl << endl;
    cout << "Failed to find MC products in : " << failedFindMC << " events" << endl; 
    cout << "Failed to find matches with gen in : " << matchNF << " events" << endl; 
    cout << "Failed to find pairs by dau indexes in : " << pairNF << " events" << endl; 
    
    cout << endl;
    
    for (int iDec = 0; iDec< 6; iDec++)
    {
        cout << " =============== " << decModeNames[iDec] << " ================= " << endl;
        
        for (int iStep = 0; iStep < nsel+extraSel; iStep++)
        {   
            float thisEff = (iStep!=0 ? 100.*passSelections[iDec][iStep]/passSelections[iDec][iStep-1]  : 100.);
            float totEff  = 100.*passSelections[iDec][iStep]/passSelections[iDec][0];
           
            cout << setw(2) << left << iStep
                 << " : "
                 << setw(20) << left << selNames[iStep]
                 << " nEvts: "
                 << setw(15) << left << passSelections[iDec][iStep]
                 << " eff [%]: "
                 << setw(10) << left << thisEff
                 << " - total: "                 
                 << setw(10) << left << totEff
                 << endl;
        }
    }
    
    fOut->Write();
}
