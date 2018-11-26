// measure the b tag efficiency / mistag rate for MC

// c++ -lm -o measureBTagEff measureBTagEff.cpp `root-config --glibs --cflags` -I ../../interface/

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cctype>
#include <algorithm>
#include "TString.h"
#include "TChain.h"
#include "TCut.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TF2.h"
#include "TGraph2DErrors.h"
#include "TEfficiency.h"

#include "HistoManager.h"
#include "ConfigParser.h"
#include "utils.h"
#include "histoUtils.h"
#include "plotContainer.h"
#include "analysisUtils.h"

using namespace std;

int main(int argc, char** argv)
{
    // check number of inpt parameters
    if (argc < 2)
    {
        cerr << "Forgot to put the cfg file --> exit " << endl ;
        return 1 ;
    }

    if (gConfigParser) return 1 ;
    gConfigParser = new ConfigParser () ;

    TString config ; 
    config.Form ("%s",argv[1]) ;

    if (! (gConfigParser->init (config)))
    {
        cout << ">>> parseConfigFile::Could not open configuration file " << config << endl ;
        return -1 ;
    }

    // read MC sample
    vector<string> bkgSamplesList = gConfigParser->readStringListOption ("general::backgrounds") ;   
    vector<sample> samples ;
    readSamples (samples, bkgSamplesList) ;

    // read list of selections
    vector<string> activeSelections = gConfigParser->readStringListOption ("selections::list") ;
    vector<pair <TString, TCut> > selections = readCutsFile (
        activeSelections,
        gConfigParser->readStringOption ("selections::selectionsFile")
    ) ;


    // global selection: MuTau || ETau || TauTau -- WARNING!! this will chain any selection
    TCut cutall = selections.at(0).second;
    for (unsigned int isel = 1; isel < selections.size(); isel++) cutall = cutall || selections.at(isel).second;
    cout << "OR of all the selection created with name: ALL" << endl;
    cout << "    ---> " << cutall << endl;
    auto cutallpair = make_pair (TString("ALL"), cutall);
    selections.push_back(cutallpair);

    cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
    cout << "selections sequence: \n" ;
    for (unsigned int i = 0 ; i < selections.size () ; ++i)
        cout << selections.at (i).first << " : " << selections.at (i).second << endl ;

    // -------------------------------------------------------------------------------------------
    // histos
    float PtBins[]  =  {20, 30, 40, 60, 100, 150, 200, 300, 1000};
    //float PtBins[]  =  {20, 30, 40, 50, 70, 100, 150, 200, 300, 640} ;
    float EtaBins[] =  {0, 0.6, 1.2, 2.1, 2.4} ;
    int nPtBins  = sizeof(PtBins)/sizeof(float) - 1;
    int nEtaBins = sizeof(EtaBins)/sizeof(float) - 1;

    // float WPtag[3] = {0.460, 0.800, 0.935}; // L, M, T
    // float WPtag[3] = {0.5426, 0.8484, 0.9535}; // L, M, T -- 80X for Moriond 2017, 23SepReReco
    float WPtag[3] = {0.1522, 0.4941, 0.8001}; // L, M, T -- 94X for Moriond 2018, 17NovReReco, version1 of the SF
    string WPname[3] = {"L", "M", "T"};

    // FIXME: do for the 3 WP tag x the number of selections
    vector<vector<TH2F*>> h2_BTaggingEff_Denom_b    (3, vector<TH2F*>(selections.size()) ) ; // .at(WP).at(selection)
    vector<vector<TH2F*>> h2_BTaggingEff_Denom_c    (3, vector<TH2F*>(selections.size()) ) ;
    vector<vector<TH2F*>> h2_BTaggingEff_Denom_udsg (3, vector<TH2F*>(selections.size()) ) ;
    vector<vector<TH2F*>> h2_BTaggingEff_Num_b      (3, vector<TH2F*>(selections.size()) ) ;
    vector<vector<TH2F*>> h2_BTaggingEff_Num_c      (3, vector<TH2F*>(selections.size()) ) ;
    vector<vector<TH2F*>> h2_BTaggingEff_Num_udsg   (3, vector<TH2F*>(selections.size()) ) ;

    string outputFile = gConfigParser->readStringOption ("general::output");
    cout << "Saving efficiency weights into: " << outputFile << endl;
    TFile* fOut = new TFile (outputFile.c_str(), "recreate");
    fOut->cd();

    for (int iWP = 0; iWP < 3; iWP++)
    {
        for (unsigned int isel = 0; isel < selections.size(); isel++)
        {
            h2_BTaggingEff_Denom_b    .at(iWP).at(isel) = new TH2F (Form("h2_BTaggingEff_Denom_b_%s_%s"   , WPname[iWP].c_str(), selections.at(isel).first.Data()), ";p_{T} [GeV];#eta", nPtBins, PtBins, nEtaBins, EtaBins);
            h2_BTaggingEff_Denom_c    .at(iWP).at(isel) = new TH2F (Form("h2_BTaggingEff_Denom_c_%s_%s"   , WPname[iWP].c_str(), selections.at(isel).first.Data()), ";p_{T} [GeV];#eta", nPtBins, PtBins, nEtaBins, EtaBins);
            h2_BTaggingEff_Denom_udsg .at(iWP).at(isel) = new TH2F (Form("h2_BTaggingEff_Denom_udsg_%s_%s", WPname[iWP].c_str(), selections.at(isel).first.Data()), ";p_{T} [GeV];#eta", nPtBins, PtBins, nEtaBins, EtaBins);
            h2_BTaggingEff_Num_b      .at(iWP).at(isel) = new TH2F (Form("h2_BTaggingEff_Num_b_%s_%s"     , WPname[iWP].c_str(), selections.at(isel).first.Data()), ";p_{T} [GeV];#eta", nPtBins, PtBins, nEtaBins, EtaBins);
            h2_BTaggingEff_Num_c      .at(iWP).at(isel) = new TH2F (Form("h2_BTaggingEff_Num_c_%s_%s"     , WPname[iWP].c_str(), selections.at(isel).first.Data()), ";p_{T} [GeV];#eta", nPtBins, PtBins, nEtaBins, EtaBins);
            h2_BTaggingEff_Num_udsg   .at(iWP).at(isel) = new TH2F (Form("h2_BTaggingEff_Num_udsg_%s_%s"  , WPname[iWP].c_str(), selections.at(isel).first.Data()), ";p_{T} [GeV];#eta", nPtBins, PtBins, nEtaBins, EtaBins);
        }
    }

    // control histos
    TH1F* h_pT_b = new TH1F ("h_pT_b", "b jets pT distribution; pT; events", 200, 0, 2000);
    TH1F* h_pT_c = new TH1F ("h_pT_c", "c jets pT distribution; pT; events", 200, 0, 2000);
    TH1F* h_pT_udsg = new TH1F ("h_pT_udsg", "udsg jets pT distribution; pT; events", 200, 0, 2000);

    TH1F* h_eta_b = new TH1F ("h_eta_b", "b jets #eta distribution; #eta; events", 50, -2.5, 2.5);
    TH1F* h_eta_c = new TH1F ("h_eta_c", "c jets #eta distribution; #eta; events", 50, -2.5, 2.5);
    TH1F* h_eta_udsg = new TH1F ("h_eta_udsg", "udsg jets #eta distribution; #eta; events", 50, -2.5, 2.5);

    TH1F* h_CSV_b = new TH1F ("h_CSV_b", "b jets CSV distribution; CSV; events", 120, 0, 1.2);
    TH1F* h_CSV_c = new TH1F ("h_CSV_c", "c jets CSV distribution; CSV; events", 120, 0, 1.2);
    TH1F* h_CSV_udsg = new TH1F ("h_CSV_udsg", "udsg jets CSV distribution; CSV; events", 120, 0, 1.2);

    // -------------------------------------------------------------------------------------------
    // variables specific to bTag   
    // chosen jets
    float bjet1_pt ;
    float bjet1_eta ;
    float bjet1_bID ;
    float bjet1_bID_deepCSV ;
    int   bjet1_flav ;
    bool  bjet1_hasgenjet ;

    float bjet2_pt ;
    float bjet2_eta ;
    float bjet2_bID ;
    float bjet2_bID_deepCSV ;
    int   bjet2_flav ;
    bool  bjet2_hasgenjet ;

    // additional jets
    std::vector<float>* jets_pt   = 0;
    std::vector<float>* jets_eta  = 0;
    std::vector<float>* jets_btag = 0;
    std::vector<float>* jets_btag_deepCSV = 0;
    std::vector<int>*   jets_flav = 0;
    std::vector<bool>*  jets_hasgenjet = 0;

    // loop on samples and fill num / denom histos
    for (unsigned int iSample = 0 ; iSample < samples.size () ; ++iSample)
    {
        TTree *tree = samples.at (iSample).sampleTree ;

        // speed up
        tree->SetBranchStatus ("*", 0);
        tree->SetBranchStatus ("MC_weight", 1) ;
        tree->SetBranchStatus ("PUReweight", 1) ;
        TObjArray *branchList = tree->GetListOfBranches();
        int nBranch   = tree->GetNbranches();
        /*
        // used vars
        for (unsigned int ivar = 0; ivar < allvars.size(); ivar++)
        {
            tree->SetBranchStatus (allvars.at(ivar).c_str(), 1);
        }
        */
        // vars used in selection
        for (int iB = 0 ; iB < nBranch; iB++)
        {
            TString bName = branchList->At(iB)->GetName();
            for (unsigned int iSel = 0; iSel < selections.size(); iSel++)
            {
                TString theCut = selections.at(iSel).second.GetTitle(); // gives the content of tCut as char*
                if (theCut.Contains (bName))
                {
                  tree->SetBranchStatus (bName, 1);
                }
            }
        }
        tree->SetBranchStatus ("jets_pt"   , 1);
        tree->SetBranchStatus ("jets_eta"  , 1);
        tree->SetBranchStatus ("jets_btag" , 1);
        tree->SetBranchStatus ("jets_btag_deepCSV" , 1);
        tree->SetBranchStatus ("jets_flav" , 1);
        tree->SetBranchStatus ("jets_hasgenjet" , 1);
        tree->SetBranchStatus ("bjet1_pt"  , 1);
        tree->SetBranchStatus ("bjet1_eta" , 1);
        tree->SetBranchStatus ("bjet1_bID" , 1);
        tree->SetBranchStatus ("bjet1_bID_deepCSV" , 1);
        tree->SetBranchStatus ("bjet1_flav", 1);
        tree->SetBranchStatus ("bjet1_hasgenjet", 1);
        tree->SetBranchStatus ("bjet2_pt"  , 1);
        tree->SetBranchStatus ("bjet2_eta" , 1);
        tree->SetBranchStatus ("bjet2_bID" , 1);
        tree->SetBranchStatus ("bjet2_bID_deepCSV" , 1);
        tree->SetBranchStatus ("bjet2_flav", 1);
        tree->SetBranchStatus ("bjet2_hasgenjet", 1);


        // set addresses
        tree->SetBranchAddress ("jets_pt", &jets_pt);
        tree->SetBranchAddress ("jets_eta", &jets_eta);
        tree->SetBranchAddress ("jets_btag", &jets_btag);
        tree->SetBranchAddress ("jets_btag_deepCSV", &jets_btag_deepCSV);
        tree->SetBranchAddress ("jets_flav", &jets_flav);
        tree->SetBranchAddress ("jets_hasgenjet", &jets_hasgenjet);
        
        tree->SetBranchAddress ("bjet1_pt", &bjet1_pt);
        tree->SetBranchAddress ("bjet1_eta", &bjet1_eta);
        tree->SetBranchAddress ("bjet1_bID", &bjet1_bID);
        tree->SetBranchAddress ("bjet1_bID_deepCSV", &bjet1_bID_deepCSV);
        tree->SetBranchAddress ("bjet1_flav", &bjet1_flav);
        tree->SetBranchAddress ("bjet1_hasgenjet", &bjet1_hasgenjet);
        
        tree->SetBranchAddress ("bjet2_pt", &bjet2_pt);
        tree->SetBranchAddress ("bjet2_eta", &bjet2_eta);
        tree->SetBranchAddress ("bjet2_bID", &bjet2_bID);
        tree->SetBranchAddress ("bjet2_bID_deepCSV", &bjet2_bID_deepCSV);
        tree->SetBranchAddress ("bjet2_flav", &bjet2_flav);
        tree->SetBranchAddress ("bjet2_hasgenjet", &bjet2_hasgenjet);

        /// add part to remove gen jets

        
        TTreeFormula** TTF = new TTreeFormula* [selections.size ()] ;
        for (unsigned int isel = 0 ; isel < selections.size () ; ++isel)
        {
            samples.at(iSample).sampleFile -> cd();
            TString fname ; fname.Form ("ttf%d",isel) ;
            TTF[isel] = new TTreeFormula (fname.Data (), selections.at (isel).second, tree) ;
        }
        
        // event loop
        int nEvts = tree->GetEntries();
        //int nEvts = 100000;

        cout << "Will process " << nEvts << " events" << endl;
        for (int iEvent = 0 ; iEvent < nEvts ; ++iEvent)
        {
            if (iEvent % 100000 == 0) cout << iEvent << " / " << nEvts << endl;
            //cout << iEvent << " / " << nEvts << endl;
            tree->GetEntry (iEvent) ;   
                //if (iEvent%10000 == 0) cout << iEvent << " / " << nEvts << endl;
                //float toAdd = PUReweight * weight * lumi * scaling ;
                
                /////////////////
                // REMEMBER: not using MC weights, which is fine for LO sample like TT
                /////////////////

            for (unsigned int isel = 0; isel < selections.size(); isel++)
            {
                // apply analysis selection
                if (! TTF[isel]->EvalInstance ()) continue ;
                bool bTag [3] ; 

                ///////////////////////////////////////////////////////
                // bjet1
                if (bjet1_hasgenjet)
                {
                    //bTag[0] = (bjet1_bID > WPtag[0]) ;
                    //bTag[1] = (bjet1_bID > WPtag[1]) ;
                    //bTag[2] = (bjet1_bID > WPtag[2]) ;
                    bTag[0] = (bjet1_bID_deepCSV > WPtag[0]) ;
                    bTag[1] = (bjet1_bID_deepCSV > WPtag[1]) ;
                    bTag[2] = (bjet1_bID_deepCSV > WPtag[2]) ;
                    
                    if (abs(bjet1_flav) == 5) // b jets
                    {
                        h2_BTaggingEff_Denom_b.at(0).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));
                        h2_BTaggingEff_Denom_b.at(1).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));
                        h2_BTaggingEff_Denom_b.at(2).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));
                        if (bTag[0]) h2_BTaggingEff_Num_b.at(0).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));
                        if (bTag[1]) h2_BTaggingEff_Num_b.at(1).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));
                        if (bTag[2]) h2_BTaggingEff_Num_b.at(2).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));

                        h_pT_b->Fill (bjet1_pt);
                        h_eta_b->Fill (bjet1_eta);
                        //h_CSV_b->Fill (bjet1_bID);
                        h_CSV_b->Fill (bjet1_bID_deepCSV);
                    }
                    else if (abs(bjet1_flav) == 4) // c jets
                    {
                        h2_BTaggingEff_Denom_c.at(0).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));
                        h2_BTaggingEff_Denom_c.at(1).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));
                        h2_BTaggingEff_Denom_c.at(2).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));
                        if (bTag[0]) h2_BTaggingEff_Num_c.at(0).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));
                        if (bTag[1]) h2_BTaggingEff_Num_c.at(1).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));
                        if (bTag[2]) h2_BTaggingEff_Num_c.at(2).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));

                        h_pT_c->Fill (bjet1_pt);
                        h_eta_c->Fill (bjet1_eta);
                        //h_CSV_c->Fill (bjet1_bID);
                        h_CSV_c->Fill (bjet1_bID_deepCSV);
                    }
                    else // udsg jets
                    {
                        h2_BTaggingEff_Denom_udsg.at(0).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));
                        h2_BTaggingEff_Denom_udsg.at(1).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));
                        h2_BTaggingEff_Denom_udsg.at(2).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));
                        if (bTag[0]) h2_BTaggingEff_Num_udsg.at(0).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));
                        if (bTag[1]) h2_BTaggingEff_Num_udsg.at(1).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));
                        if (bTag[2]) h2_BTaggingEff_Num_udsg.at(2).at(isel) -> Fill (bjet1_pt, TMath::Abs(bjet1_eta));

                        h_pT_udsg->Fill (bjet1_pt);
                        h_eta_udsg->Fill (bjet1_eta);
                        //h_CSV_udsg->Fill (bjet1_bID);
                        h_CSV_udsg->Fill (bjet1_bID_deepCSV);
                    }
                }
                ///////////////////////////////////////////////////////
                // bjet2
                if (bjet2_hasgenjet)
                {
                    //bTag[0] = (bjet2_bID > WPtag[0]) ;
                    //bTag[1] = (bjet2_bID > WPtag[1]) ;
                    //bTag[2] = (bjet2_bID > WPtag[2]) ;
                    bTag[0] = (bjet2_bID_deepCSV > WPtag[0]) ;
                    bTag[1] = (bjet2_bID_deepCSV > WPtag[1]) ;
                    bTag[2] = (bjet2_bID_deepCSV > WPtag[2]) ;
                    
                    if (abs(bjet2_flav) == 5) // b jets
                    {
                        h2_BTaggingEff_Denom_b.at(0).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));
                        h2_BTaggingEff_Denom_b.at(1).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));
                        h2_BTaggingEff_Denom_b.at(2).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));
                        if (bTag[0]) h2_BTaggingEff_Num_b.at(0).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));
                        if (bTag[1]) h2_BTaggingEff_Num_b.at(1).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));
                        if (bTag[2]) h2_BTaggingEff_Num_b.at(2).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));
                 
                        h_pT_b->Fill (bjet2_pt);
                        h_eta_b->Fill (bjet2_eta);
                        //h_CSV_b->Fill (bjet2_bID);
                        h_CSV_b->Fill (bjet2_bID_deepCSV);
                    }
                    else if (abs(bjet2_flav) == 4) // c jets
                    {
                        h2_BTaggingEff_Denom_c.at(0).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));
                        h2_BTaggingEff_Denom_c.at(1).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));
                        h2_BTaggingEff_Denom_c.at(2).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));
                        if (bTag[0]) h2_BTaggingEff_Num_c.at(0).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));
                        if (bTag[1]) h2_BTaggingEff_Num_c.at(1).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));
                        if (bTag[2]) h2_BTaggingEff_Num_c.at(2).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));

                        h_pT_c->Fill (bjet2_pt);
                        h_eta_c->Fill (bjet2_eta);
                        //h_CSV_c->Fill (bjet2_bID);
                        h_CSV_c->Fill (bjet2_bID_deepCSV);
                    }
                    else // udsg jets
                    {
                        h2_BTaggingEff_Denom_udsg.at(0).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));
                        h2_BTaggingEff_Denom_udsg.at(1).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));
                        h2_BTaggingEff_Denom_udsg.at(2).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));
                        if (bTag[0]) h2_BTaggingEff_Num_udsg.at(0).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));
                        if (bTag[1]) h2_BTaggingEff_Num_udsg.at(1).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));
                        if (bTag[2]) h2_BTaggingEff_Num_udsg.at(2).at(isel) -> Fill (bjet2_pt, TMath::Abs(bjet2_eta));

                        h_pT_udsg->Fill (bjet2_pt);
                        h_eta_udsg->Fill (bjet2_eta);
                        //h_CSV_udsg->Fill (bjet2_bID);
                        h_CSV_udsg->Fill (bjet2_bID_deepCSV);
                    }
                }

                ///////////////////////////////////////////////////////
                // other jets
//cout << " ON More Jest: " << jets_pt->size() << endl;
                for (unsigned int ijet = 0; ijet < jets_pt->size(); ijet++)
                {
                    bool hasgenjet = jets_hasgenjet->at(ijet);
                    if (hasgenjet)
                    {
                        //bTag[0] = (jets_btag->at(ijet) > WPtag[0]) ;
                        //bTag[1] = (jets_btag->at(ijet) > WPtag[1]) ;
                        //bTag[2] = (jets_btag->at(ijet) > WPtag[2]) ;
                        bTag[0] = (jets_btag_deepCSV->at(ijet) > WPtag[0]) ;
                        bTag[1] = (jets_btag_deepCSV->at(ijet) > WPtag[1]) ;
                        bTag[2] = (jets_btag_deepCSV->at(ijet) > WPtag[2]) ;
                        
                        int   flav =  jets_flav->at(ijet);
                        float this_pt =  jets_pt->at(ijet);
                        float this_eta = TMath::Abs(jets_eta->at(ijet));
                        float this_eta_noAbs = jets_eta->at(ijet);
                        
                        if (abs(flav) == 5) // b jets
                        {
                            h2_BTaggingEff_Denom_b.at(0).at(isel) -> Fill (this_pt, this_eta);
                            h2_BTaggingEff_Denom_b.at(1).at(isel) -> Fill (this_pt, this_eta);
                            h2_BTaggingEff_Denom_b.at(2).at(isel) -> Fill (this_pt, this_eta);
                            if (bTag[0]) h2_BTaggingEff_Num_b.at(0).at(isel) -> Fill (this_pt, this_eta);
                            if (bTag[1]) h2_BTaggingEff_Num_b.at(1).at(isel) -> Fill (this_pt, this_eta);
                            if (bTag[2]) h2_BTaggingEff_Num_b.at(2).at(isel) -> Fill (this_pt, this_eta);

                            h_pT_b->Fill (this_pt);
                            h_eta_b->Fill (this_eta_noAbs);
                            //h_CSV_b->Fill (jets_btag->at(ijet));
                            h_CSV_b->Fill (jets_btag_deepCSV->at(ijet));
                        }
                        else if (abs(flav) == 4) // c jets
                        {
                            h2_BTaggingEff_Denom_c.at(0).at(isel) -> Fill (this_pt, this_eta);
                            h2_BTaggingEff_Denom_c.at(1).at(isel) -> Fill (this_pt, this_eta);
                            h2_BTaggingEff_Denom_c.at(2).at(isel) -> Fill (this_pt, this_eta);
                            if (bTag[0]) h2_BTaggingEff_Num_c.at(0).at(isel) -> Fill (this_pt, this_eta);
                            if (bTag[1]) h2_BTaggingEff_Num_c.at(1).at(isel) -> Fill (this_pt, this_eta);
                            if (bTag[2]) h2_BTaggingEff_Num_c.at(2).at(isel) -> Fill (this_pt, this_eta);

                            h_pT_c->Fill (this_pt);
                            h_eta_c->Fill (this_eta_noAbs);
                            //h_CSV_c->Fill (jets_btag->at(ijet));
                            h_CSV_c->Fill (jets_btag_deepCSV->at(ijet));
                        }
                        else // udsg jets
                        {
                            h2_BTaggingEff_Denom_udsg.at(0).at(isel) -> Fill (this_pt, this_eta);
                            h2_BTaggingEff_Denom_udsg.at(1).at(isel) -> Fill (this_pt, this_eta);
                            h2_BTaggingEff_Denom_udsg.at(2).at(isel) -> Fill (this_pt, this_eta);
                            if (bTag[0]) h2_BTaggingEff_Num_udsg.at(0).at(isel) -> Fill (this_pt, this_eta);
                            if (bTag[1]) h2_BTaggingEff_Num_udsg.at(1).at(isel) -> Fill (this_pt, this_eta);
                            if (bTag[2]) h2_BTaggingEff_Num_udsg.at(2).at(isel) -> Fill (this_pt, this_eta);
     
                            h_pT_udsg->Fill (this_pt);
                            h_eta_udsg->Fill (this_eta_noAbs);
                            //h_CSV_udsg->Fill (jets_btag->at(ijet));
                            h_CSV_udsg->Fill (jets_btag_deepCSV->at(ijet));
                        }
                    }
                }
            }    
        }
        for (unsigned int isel = 0; isel < selections.size(); isel++) delete TTF[isel];
        delete[] TTF;
    }

    cout << "...Finished histo filling, going to compute efficiencies" << endl;
    ///////////////////////////////////////////////////////////
    // num / denom are ready, compute efficiencies and store them

    fOut->cd();
    vector<vector<TH2F*>> eff_b    (3, vector<TH2F*>(selections.size()) ) ; // selection, WP indexed
    vector<vector<TH2F*>> eff_c    (3, vector<TH2F*>(selections.size()) ) ; // selection, WP indexed
    vector<vector<TH2F*>> eff_udsg (3, vector<TH2F*>(selections.size()) ) ; // selection, WP indexed

    for (int iWP = 0; iWP < 3; iWP++)
    {
        for (unsigned int isel = 0; isel < selections.size(); isel++)
        {
            //TEfficiency* pEff_b = 0;
            //TEfficiency* pEff_c = 0;
            //TEfficiency* pEff_udsg = 0;

            //eff_b.at(iWP).at(isel) = new TH2F    (Form("eff_b_%s_%s"   , WPname[iWP].c_str(), selections.at(isel).first.Data() ) , "eff_b; pT; |#eta|", nPtBins, PtBins, nEtaBins, EtaBins);
            //eff_c.at(iWP).at(isel) = new TH2F    (Form("eff_c_%s_%s"   , WPname[iWP].c_str(), selections.at(isel).first.Data() ) , "eff_c; pT; |#eta|", nPtBins, PtBins, nEtaBins, EtaBins);
            //eff_udsg.at(iWP).at(isel) = new TH2F (Form("eff_udsg_%s_%s", WPname[iWP].c_str(), selections.at(isel).first.Data() ) , "eff_udsg; pT; |#eta|", nPtBins, PtBins, nEtaBins, EtaBins);

            (eff_b.at(iWP).at(isel)) = (TH2F*)(h2_BTaggingEff_Num_b.at(iWP).at(isel))->Clone(Form("eff_b_%s_%s", WPname[iWP].c_str(), selections.at(isel).first.Data() ));
            (eff_b.at(iWP).at(isel))->Sumw2();
            (eff_b.at(iWP).at(isel))->Divide((h2_BTaggingEff_Denom_b.at(iWP).at(isel)));

            (eff_c.at(iWP).at(isel)) = (TH2F*)(h2_BTaggingEff_Num_c.at(iWP).at(isel))->Clone(Form("eff_c_%s_%s", WPname[iWP].c_str(), selections.at(isel).first.Data() ));
            (eff_c.at(iWP).at(isel))->Sumw2();
            (eff_c.at(iWP).at(isel))->Divide((h2_BTaggingEff_Denom_c.at(iWP).at(isel)));

            (eff_udsg.at(iWP).at(isel)) = (TH2F*)(h2_BTaggingEff_Num_udsg.at(iWP).at(isel))->Clone(Form("eff_udsg_%s_%s", WPname[iWP].c_str(), selections.at(isel).first.Data() ));
            (eff_udsg.at(iWP).at(isel))->Sumw2();
            (eff_udsg.at(iWP).at(isel))->Divide((h2_BTaggingEff_Denom_udsg.at(iWP).at(isel)));

            // b jets
            /*if(TEfficiency::CheckConsistency( *(h2_BTaggingEff_Num_b.at(iWP).at(isel)), *(h2_BTaggingEff_Denom_b.at(iWP).at(isel)) ) )
            {
                pEff_b = new TEfficiency(*(h2_BTaggingEff_Num_b.at(iWP).at(isel)), *(h2_BTaggingEff_Denom_b.at(iWP).at(isel)));

                for (int gBin = 0; gBin < (h2_BTaggingEff_Num_b.at(iWP).at(isel))->GetSize(); gBin++)
                {
                    (eff_b.at(iWP).at(isel))->SetBinContent(gBin, pEff_b->GetEfficiency(gBin));

                    //cout << "  *** TESTING: "<< h2_BTaggingEff_Num_b.at(iWP).at(isel) << endl;
                    //float numX = h2_BTaggingEff_Num_b.at(iWP).at(isel)->GetBinContent(gBin);
                    //float denX = h2_BTaggingEff_Denom_b.at(iWP).at(isel)->GetBinContent(gBin);
                    //cout << " ***DEB num, den, rap: " << numX << "  " << denX << "  " << numX/denX << endl;
                    //eff_b_2D->SetBinContent(gBin, numX);
                    //eff_b_2D = (h2_BTaggingEff_Num_b.at(iWP).at(isel))->Clone();
                    //std::cout << " ***DEB: getErrLow: " << pEff_b->GetEfficiencyErrorLow(gBin) << endl;
                    //std::cout << "         getErrUp : " << pEff_b->GetEfficiencyErrorUp(gBin) << endl;
                }
                //delete pEff_b; // ? could crash, stupid ROOT...
            }
            // c jets
            if(TEfficiency::CheckConsistency( *(h2_BTaggingEff_Num_c.at(iWP).at(isel)), *(h2_BTaggingEff_Denom_c.at(iWP).at(isel)) ) )
            {
                pEff_c = new TEfficiency(*(h2_BTaggingEff_Num_c.at(iWP).at(isel)), *(h2_BTaggingEff_Denom_c.at(iWP).at(isel)));
                for (int gBin = 0; gBin < (h2_BTaggingEff_Num_c.at(iWP).at(isel))->GetSize(); gBin++)
                {
                    (eff_c.at(iWP).at(isel))->SetBinContent(gBin, pEff_c->GetEfficiency(gBin));
                }
                //delete pEff_b; // ? could crash, stupid ROOT...
            }
            // udsg jets
            if(TEfficiency::CheckConsistency( *(h2_BTaggingEff_Num_udsg.at(iWP).at(isel)), *(h2_BTaggingEff_Denom_udsg.at(iWP).at(isel)) ) )
            {
                pEff_udsg = new TEfficiency(*(h2_BTaggingEff_Num_udsg.at(iWP).at(isel)), *(h2_BTaggingEff_Denom_udsg.at(iWP).at(isel)));
                for (int gBin = 0; gBin < (h2_BTaggingEff_Num_udsg.at(iWP).at(isel))->GetSize(); gBin++)
                {
                    (eff_udsg.at(iWP).at(isel))->SetBinContent(gBin, pEff_udsg->GetEfficiency(gBin));
                }
                //delete pEff_b; // ? could crash, stupid ROOT...
            }*/
        }
    }
    // save
    fOut->Write();
}