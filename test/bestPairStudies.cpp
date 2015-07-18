/*
** Make studies about choice of best pair.
**
** When running on simulated HH signal:
** 1) split samples according to the MC real category
** 2) apply minimal selection on gen level objects to ensure they are not out of detector acceptance
** 3) with a loose geometrical matching determine which objects are reconstructed from the gen level ones
** 4) evaluate selection efficiency at each level. Count how many pairs are formed for each event
** 5) at the end of the selection determine, in case of multiple pairs, which one is the correct one (if any),
**    and determine the best algorithm to find it
**
** When running on background:
** A) Start from point 4) by applying selections; use the same algo and fill some distributions
*/

#include "bigTree.h"
#include "OfflineProducerHelper.h"
#include "HistoManager.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "TFile.h"
#include "TLorentzVector.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"

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

    TString sel[] = {"Vertex", "OScharge", "LepID", "pTMin", "etaMax", "againstEle", "againstMu", "Iso"};
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

    std::vector<int> goodPairs; // contain the good pairs, to be used at a certain step of the selection . ONLY pairs of the same type of the MC decay
    std::vector<int> goodPairsAllTypes; // contain the good pairs, to be used at a certain step of the selection . ALL types of pairs (don't ask for the same MC category)

    HistoManager* pairCounter = new HistoManager ("pairCounter"); // for each MC type count how many pairs of the same type (reco) you have
    HistoManager* pairAllCatCounter = new HistoManager ("pairAllCatCounter"); // for each MC type count how many pairs (reco - all types) you have
    HistoManager* goodPairMaxPtCounter = new HistoManager ("goodPairMaxPtCounter"); // for each MC type count which pair of the same type are selected
    HistoManager* goodPairMaxPtAllCatCounter = new HistoManager ("goodPairMaxPtAllCatCounter"); // for each MC type count which pair of all reco types are selected
    HistoManager* MCCatVsRecoCat = new HistoManager ("MCCatVsRecoCat"); // 2D histo of MC type vs type of chose reco pair

    for (int i = 0; i < nsel; i++)
    {
        TString thisName = sel[i];
        thisName += " - step ";
        thisName += i;
        TString thisName2 = thisName;
        TString thisName3 = thisName;
        thisName += " ; MC decay ; N pairs";
        thisName2 += " ; MC decay ; code";
        thisName3 += " ; MC decay ; reco pair type";
        pairCounter->AddNew2DHisto (sel[i].Data(), thisName.Data(), 6, -0.5, 5.5,  6, -0.5, 5.5); // for each MC channel (x) count # pairs per selection (y)
        pairAllCatCounter->AddNew2DHisto (sel[i].Data(), thisName.Data(), 6, -0.5, 5.5,  6, -0.5, 5.5); // for each MC channel (x) count # pairs per selection (y)
        goodPairMaxPtCounter->AddNew2DHisto (sel[i].Data(), thisName2.Data(), 6, -0.5, 5.5,  4, -0.5, 3.5); // on x: MC channel / on y: codes: 0 = no pairs , 1 = good pair chosen , 2 = wrong pair chosen, but good pair is there, 3 = good pair is not there
        goodPairMaxPtAllCatCounter->AddNew2DHisto (sel[i].Data(), thisName2.Data(), 6, -0.5, 5.5,  4, -0.5, 3.5); // on x: MC channel / on y: codes: 0 = no pairs , 1 = good pair chosen , 2 = wrong pair chosen, but good pair is there, 3 = good pair is not there
        MCCatVsRecoCat->AddNew2DHisto (sel[i].Data(), thisName3.Data(), 6, -0.5, 5.5, 6, -0.5, 5.5); // not found are stored in underflow, do not distinguish if I choose the wrong pair of the good reco type

    }

    // ------------------------------------------------------
    // loop on events
    // ------------------------------------------------------
    for (int iEv = 0; iEv < nEvents; iEv++)
    {
        tree->GetEntry(iEv);
        if (iEv%50000 == 0) cout << iEv << " / " << nEvents << endl;
    
        /*
        for (int iMoth = 0; iMoth < tree->mothers_px->size(); iMoth++)
        {
            float px = tree->mothers_px->at(iMoth);
            float py = tree->mothers_py->at(iMoth);
            int ind1 = tree->indexDau1->at(iMoth);
            int ind2 = tree->indexDau2->at(iMoth);

            cout << iMoth << " == " << tree->isOSCand->at(iMoth) << " - ( " << tree->daughters_charge->at(ind1) << " . " << tree->daughters_charge->at(ind2) << " ) -- " << TMath::Sqrt(px*px + py*py) << endl;
        }
        cout << endl << endl;
        */

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
        
        // 1 ========= minimal selections on gen level
        TLorentzVector genV1 (helper.buildGenP4(tree, genVis1));        
        int genPdgId1 = tree->genpart_pdg->at(genVis1);
        TLorentzVector genV2 (helper.buildGenP4(tree, genVis2));        
        int genPdgId2 = tree->genpart_pdg->at(genVis2);
          
        if (TMath::Abs(genV1.Eta()) > 2.5 || TMath::Abs(genV2.Eta()) > 2.5) continue;
        if ( !ptGenSel(genV1, genPdgId1) || !ptGenSel(genV2, genPdgId2) ) continue;

        
        // 2 ==========  find matching reco leptons - last 3 vars are checkID, checkCharge, drCont
        const int matchedVis1 = helper.getRecoMatchedToGen (tree, genVis1, true, true, 0.5);
        const int matchedVis2 = helper.getRecoMatchedToGen (tree, genVis2, true, true, 0.5);
        
        if (matchedVis1 == -1 || matchedVis2 == -1)
        {
            ++matchNF;
            continue;
        }
        

        // =========== start with selections
        // start to apply selections
        for (int iSel = 0; iSel < nsel; iSel++)
        {
            //int nPairsPassing = 0;
            goodPairs.clear();
            goodPairsAllTypes.clear();

            for (int iMoth = 0; iMoth < tree->mothers_px->size(); iMoth++)
            {
                int pairType = helper.getMothPairType (tree, iMoth);

                const bool sameTypeAsMC = (pairType == MCtype);

                bool PairPassBaseline = helper.pairPassBaseline (tree, iMoth, selConc[iSel]);
                if (PairPassBaseline)
                {   
                    goodPairsAllTypes.push_back(iMoth);
                    if (sameTypeAsMC) goodPairs.push_back(iMoth);
                    //++nPairsPassing;
                }
            }

// TO DO: check when the pair selected with an algorithm is really the good best pair, i.e. the one where I matched with the gen particles

            // fill histos for this selection
            pairCounter -> Get2DHisto (sel[iSel].Data()) -> Fill (MCtype, goodPairs.size());
            pairAllCatCounter -> Get2DHisto (sel[iSel].Data()) -> Fill (MCtype, goodPairsAllTypes.size());
            
            // check if good pair is in the list
            const int theRightPair = helper.getPairByIndexes (tree, matchedVis1, matchedVis2);
            std::vector<int>::iterator rightPairInVector = find (goodPairs.begin(), goodPairs.end(), theRightPair);
            const int rightPosition = ( (rightPairInVector != goodPairs.end()) ? distance (goodPairs.begin(), rightPairInVector) : -1 ); // -1: not in vector, else is pair index

            // ---------------------- get best pairs - only on same reco type as MC
            int bestPair = helper.getBestPair (tree, goodPairs, "OSMaxPt");
            if (bestPair == -1) goodPairMaxPtCounter -> Get2DHisto (sel[iSel].Data()) -> Fill (MCtype, 0); // no pair found
            else if (bestPair == rightPosition) goodPairMaxPtCounter -> Get2DHisto (sel[iSel].Data()) -> Fill (MCtype, 1); // best pair matches 
            else if (bestPair != rightPosition && rightPosition != -1) goodPairMaxPtCounter -> Get2DHisto (sel[iSel].Data()) -> Fill (MCtype, 2); // not right but best pair existing
            else if (bestPair != rightPosition && rightPosition == -1) goodPairMaxPtCounter -> Get2DHisto (sel[iSel].Data()) -> Fill (MCtype, 3); // not right but best pair not existing

            // ---------------------- get best pairs - on all reco types
            bestPair = helper.getBestPair (tree, goodPairsAllTypes, "OSMaxPt");
            if (bestPair == -1) goodPairMaxPtAllCatCounter -> Get2DHisto (sel[iSel].Data()) -> Fill (MCtype, 0); // no pair found
            else if (bestPair == rightPosition) goodPairMaxPtAllCatCounter -> Get2DHisto (sel[iSel].Data()) -> Fill (MCtype, 1); // best pair matches 
            else if (bestPair != rightPosition && rightPosition != -1) goodPairMaxPtAllCatCounter -> Get2DHisto (sel[iSel].Data()) -> Fill (MCtype, 2); // not right but best pair existing
            else if (bestPair != rightPosition && rightPosition == -1) goodPairMaxPtAllCatCounter -> Get2DHisto (sel[iSel].Data()) -> Fill (MCtype, 3); // not right but best pair not existing

            // if a pair is found
            if (bestPair != -1)
                MCCatVsRecoCat->Get2DHisto(sel[iSel].Data()) -> Fill (MCtype, helper.getMothPairType(tree, bestPair));

        }
 
    }

    // set bin labels and save histos to file
    for (int iSel = 0; iSel < nsel; iSel++)
        for (int iBin = 0; iBin < 6; iBin++)
        {
            pairCounter -> Get2DHisto (sel[iSel].Data()) -> GetXaxis() -> SetBinLabel(iBin+1, decModeNames[iBin]);
            pairAllCatCounter -> Get2DHisto (sel[iSel].Data()) -> GetXaxis() -> SetBinLabel(iBin+1, decModeNames[iBin]);
            goodPairMaxPtCounter -> Get2DHisto (sel[iSel].Data()) -> GetXaxis() -> SetBinLabel(iBin+1, decModeNames[iBin]);
            goodPairMaxPtAllCatCounter -> Get2DHisto (sel[iSel].Data()) -> GetXaxis() -> SetBinLabel(iBin+1, decModeNames[iBin]);
            MCCatVsRecoCat->Get2DHisto (sel[iSel].Data()) -> GetXaxis() -> SetBinLabel(iBin+1, decModeNames[iBin]);
            MCCatVsRecoCat->Get2DHisto (sel[iSel].Data()) -> GetYaxis() -> SetBinLabel(iBin+1, decModeNames[iBin]);
        }

    // bin label codes for y axis in pair counter
    const char* ylabels [4] = {"No pairs", "Good pair", "Wrong pair selected", "Good pair discarded by sel."};
    for (int iSel = 0; iSel < nsel; iSel++)
        for (int iBin = 0; iBin < 4; iBin++)
            {
                goodPairMaxPtCounter -> Get2DHisto (sel[iSel].Data()) -> GetYaxis() -> SetBinLabel(iBin+1, ylabels [iBin]);
                goodPairMaxPtAllCatCounter -> Get2DHisto (sel[iSel].Data()) -> GetYaxis() -> SetBinLabel(iBin+1, ylabels [iBin]);
            }
    TFile* fOut = new TFile ("bestPair.root", "recreate");
    pairCounter->SaveAllToFile(fOut);
    pairAllCatCounter->SaveAllToFile(fOut);
    goodPairMaxPtCounter->SaveAllToFile(fOut);
    goodPairMaxPtAllCatCounter->SaveAllToFile(fOut);
    MCCatVsRecoCat->SaveAllToFile(fOut);
    fOut->Close();
    //delete fOut;
    //delete pairCounter;
}
