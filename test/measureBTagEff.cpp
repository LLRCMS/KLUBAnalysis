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

    bool saveSyncTree = false;

    if (! (gConfigParser->init (config)))
    {
        cout << ">>> parseConfigFile::Could not open configuration file " << config << endl ;
        return -1 ;
    }

    // read MC sample
    vector<string> bkgSamplesList = gConfigParser->readStringListOption ("general::backgrounds") ;   
    vector<mysample> samples ;
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

    //float WPtag[3] = {0.2217, 0.6321, 0.8953}; // L, M, T -- Legacy 2016 DeepCSV
    //float WPtag[3] = {0.0614, 0.3093, 0.7221}; // L, M, T -- Legacy 2016 DeepFlavor

    //float WPtag[3] = {0.1522, 0.4941, 0.8001}; // L, M, T -- Legacy 2017 DeepCSV
    float WPtag[3] = {0.0521, 0.3033, 0.7489}; // L, M, T -- Legacy 2017 DeepFlavor

    //float WPtag[3] = {0.1241, 0.4184, 0.7527}; // L, M, T -- Legacy 2018 DeepCSV
    //float WPtag[3] = {0.0494, 0.2770, 0.7264}; // L, M, T -- Legacy 2018 DeepFlavor

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
        tree->SetBranchStatus ("jets_btag_deepFlavor" , 1);
        tree->SetBranchStatus ("jets_flav" , 1);
        tree->SetBranchStatus ("jets_hasgenjet" , 1);
        tree->SetBranchStatus ("bjet1_pt"  , 1);
        tree->SetBranchStatus ("bjet1_eta" , 1);
        tree->SetBranchStatus ("bjet1_bID" , 1);
        tree->SetBranchStatus ("bjet1_bID_deepCSV" , 1);
        tree->SetBranchStatus ("bjet1_bID_deepFlavor" , 1);
        tree->SetBranchStatus ("bjet1_flav", 1);
        tree->SetBranchStatus ("bjet1_hasgenjet", 1);
        tree->SetBranchStatus ("bjet2_pt"  , 1);
        tree->SetBranchStatus ("bjet2_eta" , 1);
        tree->SetBranchStatus ("bjet2_bID" , 1);
        tree->SetBranchStatus ("bjet2_bID_deepCSV" , 1);
        tree->SetBranchStatus ("bjet2_bID_deepFlavor" , 1);
        tree->SetBranchStatus ("bjet2_flav", 1);
        tree->SetBranchStatus ("bjet2_hasgenjet", 1);


        // set addresses
        tree->SetBranchAddress ("jets_pt", &jets_pt);
        tree->SetBranchAddress ("jets_eta", &jets_eta);
        tree->SetBranchAddress ("jets_btag", &jets_btag);
        //tree->SetBranchAddress ("jets_btag_deepCSV", &jets_btag_deepCSV);
        tree->SetBranchAddress ("jets_btag_deepFlavor", &jets_btag_deepCSV);
        tree->SetBranchAddress ("jets_flav", &jets_flav);
        tree->SetBranchAddress ("jets_hasgenjet", &jets_hasgenjet);
        
        tree->SetBranchAddress ("bjet1_pt", &bjet1_pt);
        tree->SetBranchAddress ("bjet1_eta", &bjet1_eta);
        tree->SetBranchAddress ("bjet1_bID", &bjet1_bID);
        //tree->SetBranchAddress ("bjet1_bID_deepCSV", &bjet1_bID_deepCSV);
        tree->SetBranchAddress ("bjet1_bID_deepFlavor", &bjet1_bID_deepCSV);
        tree->SetBranchAddress ("bjet1_flav", &bjet1_flav);
        tree->SetBranchAddress ("bjet1_hasgenjet", &bjet1_hasgenjet);
        
        tree->SetBranchAddress ("bjet2_pt", &bjet2_pt);
        tree->SetBranchAddress ("bjet2_eta", &bjet2_eta);
        tree->SetBranchAddress ("bjet2_bID", &bjet2_bID);
        //tree->SetBranchAddress ("bjet2_bID_deepCSV", &bjet2_bID_deepCSV);
        tree->SetBranchAddress ("bjet2_bID_deepFlavor", &bjet2_bID_deepCSV);
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
                //if (bjet1_hasgenjet)
                if (true)
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
                //if (bjet2_hasgenjet)
                if (true)
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
                    //bool hasgenjet = jets_hasgenjet->at(ijet);
                    //if (hasgenjet)
                    if (true)
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


    // add a sync tree to the fOut for comparing bTagEff LLR-PI
    if (saveSyncTree)
    {
        cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n" ;
        cout << " --- Saving sync tree for sync exercise --- \n" << endl;

        // using only the "ALL" selection
        vector<pair <TString, TCut> > new_selections;
        new_selections.push_back(cutallpair);
        for (unsigned int k = 0; k < new_selections.size(); k++)
        {
            cout << "Selections: " << k << "  " << new_selections.at(k).first << "  " << new_selections.at(k).second << endl;
        }

        double pt_min = 20.;
        double pt_max = 30.;
        double eta_min = 0.;
        double eta_max = 0.6;

        // output tree
        TTree* tOut = new TTree("bTagSyncTree","bTagSyncTree");
        int run, LS, evt, sample_id, channel;
        int n_b_Mtag, n_c_Mtag, n_usdg_Mtag;
        int n_b_jets, n_c_jets, n_usdg_jets;
        tOut->Branch("run", &run, "run/I") ;
        tOut->Branch("lumi", &LS, "lumi/I") ;
        tOut->Branch("evt", &evt, "evt/I") ;
        tOut->Branch("sample_id", &sample_id, "sample_id/I") ;
        tOut->Branch("channel", &channel, "channel/I") ;
        tOut->Branch("n_b_Mtag", &n_b_Mtag, "n_b_Mtag/I") ;
        tOut->Branch("n_c_Mtag", &n_c_Mtag, "n_c_Mtag/I") ;
        tOut->Branch("n_usdg_Mtag", &n_usdg_Mtag, "n_usdg_Mtag/I") ;
        tOut->Branch("n_b_jets", &n_b_jets, "n_b_jets/I") ;
        tOut->Branch("n_c_jets", &n_c_jets, "n_c_jets/I") ;
        tOut->Branch("n_usdg_jets", &n_usdg_jets, "n_usdg_jets/I") ;

        // input ttrees and variables
        int Run, Lumi;
        ULong64_t Evt;
        float BDT_channel;
        float bjet1_pt ;
        float bjet1_eta ;
        float bjet1_bID_deepCSV ;
        int   bjet1_flav ;
        bool  bjet1_hasgenjet ;
        float bjet2_pt ;
        float bjet2_eta ;
        float bjet2_bID_deepCSV ;
        int   bjet2_flav ;
        bool  bjet2_hasgenjet ;
        float dau1_pt, dau1_eta, dau2_pt, dau2_eta;
        int nleps, pairType;
        float tauH_SVFIT_mass, bH_mass_raw;
        std::vector<float>* jets_pt   = 0;
        std::vector<float>* jets_eta  = 0;
        std::vector<float>* jets_btag_deepCSV = 0;
        std::vector<int>*   jets_flav = 0;
        std::vector<bool>*  jets_hasgenjet = 0;

        // loop on samples
        for (unsigned int iSample = 0 ; iSample < samples.size () ; ++iSample)
        {
            //cout << "** Doing sample: " << iSample << endl;
            TTree *tree = samples.at (iSample).sampleTree ;

            // sample_ID variable
            sample_id = iSample;

            // speed up
            tree->SetBranchStatus ("*", 0);
            TObjArray *branchList = tree->GetListOfBranches();
            int nBranch   = tree->GetNbranches();
            for (int iB = 0 ; iB < nBranch; iB++)
            {
                TString bName = branchList->At(iB)->GetName();
                for (unsigned int iSel = 0; iSel < new_selections.size(); iSel++)
                {
                    TString theCut = new_selections.at(iSel).second.GetTitle(); // gives the content of tCut as char*
                    if (theCut.Contains (bName))
                    {
                      tree->SetBranchStatus (bName, 1);
                    }
                }
            }

            tree->SetBranchStatus ("EventNumber"   , 1);
            tree->SetBranchStatus ("RunNumber"   , 1);
            tree->SetBranchStatus ("lumi"   , 1);
            tree->SetBranchStatus ("BDT_channel", 1);
            tree->SetBranchStatus ("jets_pt"   , 1);
            tree->SetBranchStatus ("jets_eta"  , 1);
            tree->SetBranchStatus ("jets_btag_deepCSV" , 1);
            tree->SetBranchStatus ("jets_flav" , 1);
            tree->SetBranchStatus ("jets_hasgenjet" , 1);
            tree->SetBranchStatus ("bjet1_pt"  , 1);
            tree->SetBranchStatus ("bjet1_eta" , 1);
            tree->SetBranchStatus ("bjet1_bID_deepCSV" , 1);
            tree->SetBranchStatus ("bjet1_flav", 1);
            tree->SetBranchStatus ("bjet1_hasgenjet", 1);
            tree->SetBranchStatus ("bjet2_pt"  , 1);
            tree->SetBranchStatus ("bjet2_eta" , 1);
            tree->SetBranchStatus ("bjet2_bID_deepCSV" , 1);
            tree->SetBranchStatus ("bjet2_flav", 1);
            tree->SetBranchStatus ("bjet2_hasgenjet", 1);
            tree->SetBranchStatus ("dau1_pt", 1);
            tree->SetBranchStatus ("dau1_eta", 1);
            tree->SetBranchStatus ("dau2_pt", 1);
            tree->SetBranchStatus ("dau2_eta", 1);
            tree->SetBranchStatus ("nleps", 1);
            tree->SetBranchStatus ("pairType", 1);
            tree->SetBranchStatus ("tauH_SVFIT_mass", 1);
            tree->SetBranchStatus ("bH_mass_raw", 1);

            tree->SetBranchAddress ("EventNumber", &Evt);
            tree->SetBranchAddress ("RunNumber", &Run);
            tree->SetBranchAddress ("lumi", &Lumi);
            tree->SetBranchAddress ("BDT_channel", &BDT_channel);
            tree->SetBranchAddress ("jets_pt", &jets_pt);
            tree->SetBranchAddress ("jets_eta", &jets_eta);
            tree->SetBranchAddress ("jets_btag_deepCSV", &jets_btag_deepCSV);
            tree->SetBranchAddress ("jets_flav", &jets_flav);
            tree->SetBranchAddress ("jets_hasgenjet", &jets_hasgenjet);
            tree->SetBranchAddress ("bjet1_pt", &bjet1_pt);
            tree->SetBranchAddress ("bjet1_eta", &bjet1_eta);
            tree->SetBranchAddress ("bjet1_bID_deepCSV", &bjet1_bID_deepCSV);
            tree->SetBranchAddress ("bjet1_flav", &bjet1_flav);
            tree->SetBranchAddress ("bjet1_hasgenjet", &bjet1_hasgenjet);
            tree->SetBranchAddress ("bjet2_pt", &bjet2_pt);
            tree->SetBranchAddress ("bjet2_eta", &bjet2_eta);
            tree->SetBranchAddress ("bjet2_bID_deepCSV", &bjet2_bID_deepCSV);
            tree->SetBranchAddress ("bjet2_flav", &bjet2_flav);
            tree->SetBranchAddress ("bjet2_hasgenjet", &bjet2_hasgenjet);
            tree->SetBranchAddress ("dau1_pt", &dau1_pt);
            tree->SetBranchAddress ("dau1_eta", &dau1_eta);
            tree->SetBranchAddress ("dau2_pt", &dau2_pt);
            tree->SetBranchAddress ("dau2_eta", &dau2_eta);
            tree->SetBranchAddress ("nleps", &nleps);
            tree->SetBranchAddress ("pairType", &pairType);
            tree->SetBranchAddress ("tauH_SVFIT_mass", &tauH_SVFIT_mass);
            tree->SetBranchAddress ("bH_mass_raw", &bH_mass_raw);

            TTreeFormula** TTF = new TTreeFormula* [new_selections.size ()] ;
            for (unsigned int isel = 0 ; isel < new_selections.size () ; ++isel)
            {
                samples.at(iSample).sampleFile -> cd();
                TString fname ; fname.Form ("ttf%d",isel) ;
                TTF[isel] = new TTreeFormula (fname.Data (), new_selections.at (isel).second, tree) ;
                cout << "Selection: " << isel << "  " << fname << "  " << new_selections.at (isel).second << endl;
            }

            // event loop
            int nEvts = tree->GetEntries();
            cout << "Will process " << nEvts << " events" << endl;
            for (int iEvent = 0 ; iEvent < nEvts ; ++iEvent)
            {
                if (iEvent % 100000 == 0) cout << iEvent << " / " << nEvts << endl;
                tree->GetEntry (iEvent) ;

                bool fillEvent = false;
                bool DEBUG = false;
                if (Evt==13364954)
                {
                    DEBUG = true;
                    cout << "-------- DEBUG Event " << Run <<":"<< Lumi <<":"<<Evt<<":"<<iSample<< " -------" << endl;
                }

                // general variables
                run = Run;
                LS = Lumi;
                evt = Evt;
                channel = BDT_channel;

                // btag specific variables
                n_b_Mtag = 0;
                n_c_Mtag = 0;
                n_usdg_Mtag = 0;
                n_b_jets = 0;
                n_c_jets = 0;
                n_usdg_jets = 0;

                for (unsigned int isel = 0; isel < new_selections.size(); isel++)
                {

                    //if (DEBUG && !fillEvent)
                    if (DEBUG)
                    {
                        cout << "-> Selections:" << endl;
                        cout << "Channel: " << pairType << endl; //0:muTau  1:eTau  2:tauTau
                        if (pairType == 0)
                        {
                            cout << "Pass tau1 kine   : " << bool(dau1_pt > 10 && abs (dau1_eta) < 2.1) << endl;
                            cout << "Pass tau2 kine   : " << bool(dau2_pt > 20 && abs (dau2_eta) < 2.3) << endl;
                            cout << "Pass ExtraLepVeto: " << bool(nleps == 0) << endl;
                            cout << "Pass mass cut    : " << bool(((tauH_SVFIT_mass-116.)*(tauH_SVFIT_mass-116.))/(45.*45.) + ((bH_mass_raw-111.)*(bH_mass_raw-111.))/(55.*55.) <  1.0) << endl;
                            if (!(((tauH_SVFIT_mass-116.)*(tauH_SVFIT_mass-116.))/(45.*45.) + ((bH_mass_raw-111.)*(bH_mass_raw-111.))/(55.*55.) <  1.0))
                                cout << "   tauH_SVFIT_mass: " << tauH_SVFIT_mass << "   bH_mass: " << bH_mass_raw << endl;
                        }
                        if (pairType == 1)
                        {
                            cout << "Pass tau1 kine   : " << bool(dau1_pt > 10 && abs (dau1_eta) < 2.1) << endl;
                            cout << "Pass tau2 kine   : " << bool(dau2_pt > 20 && abs (dau2_eta) < 2.3) << endl;
                            cout << "Pass ExtraLepVeto: " << bool(nleps == 0) << endl;
                            cout << "Pass mass cut    : " << bool(((tauH_SVFIT_mass-116.)*(tauH_SVFIT_mass-116.))/(45.*45.) + ((bH_mass_raw-111.)*(bH_mass_raw-111.))/(55.*55.) <  1.0) << endl;
                            if (!(((tauH_SVFIT_mass-116.)*(tauH_SVFIT_mass-116.))/(45.*45.) + ((bH_mass_raw-111.)*(bH_mass_raw-111.))/(55.*55.) <  1.0))
                                cout << "   tauH_SVFIT_mass: " << tauH_SVFIT_mass << "   bH_mass: " << bH_mass_raw << endl;
                        }
                        if (pairType == 2)
                        {
                            cout << "Pass tau1 kine   : " << bool(dau1_pt > 20 && abs (dau1_eta) < 2.1) << endl;
                            cout << "Pass tau2 kine   : " << bool(dau2_pt > 20 && abs (dau2_eta) < 2.1) << endl;
                            cout << "Pass ExtraLepVeto: " << bool(nleps == 0) << endl;
                            cout << "Pass mass cut    : " << bool(((tauH_SVFIT_mass-116.)*(tauH_SVFIT_mass-116.))/(45.*45.) + ((bH_mass_raw-111.)*(bH_mass_raw-111.))/(55.*55.) <  1.0) << endl;
                            if (!(((tauH_SVFIT_mass-116.)*(tauH_SVFIT_mass-116.))/(45.*45.) + ((bH_mass_raw-111.)*(bH_mass_raw-111.))/(55.*55.) <  1.0))
                                cout << "   tauH_SVFIT_mass: " << tauH_SVFIT_mass << "   bH_mass: " << bH_mass_raw << endl;
                        }
                    }

                    // apply analysis selection
                    if (! TTF[isel]->EvalInstance ()) continue ;
                    fillEvent = true;

                    if (DEBUG) cout << "-> Selections: OK" << endl;

                    bool bTag;
                    //cout << " --> sel: " << isel << endl;

                    // bjet1
                    if (DEBUG) cout << "-> bjet1_hasgenjet: " << bjet1_hasgenjet << endl;
                    //if (bjet1_hasgenjet)
                    if (true)
                    {
                        bTag = (bjet1_bID_deepCSV > WPtag[1]) ;

                        int   flav =  bjet1_flav;
                        float this_pt =  bjet1_pt;
                        float this_eta = TMath::Abs(bjet1_eta);

                        //  pt (20,30) - eta (0, 0.6)
                        bool goodJet = true;
                        if (this_pt < pt_min || this_pt > pt_max || this_eta < eta_min || this_eta > eta_max) goodJet = false;
                        //cout << "   --> bjet1: " << this_pt << " // " << this_eta << endl;

                        //Fill variables
                        if (goodJet)
                        {
                            if (abs(flav) == 5) // b jets
                            {
                                if (DEBUG) cout << "  it is B"<<endl;
                                n_b_jets += 1;
                                if(bTag) n_b_Mtag += 1;
                            }

                            else if (abs(flav) == 4) // c jets
                            {
                                if (DEBUG) cout << "  it is C"<<endl;
                                n_c_jets += 1;
                                if(bTag) n_c_Mtag += 1;
                            }
                            else // udsg jets
                            {
                                if (DEBUG) cout << "  it is OTHER"<<endl;
                                n_usdg_jets += 1;
                                if(bTag) n_usdg_Mtag += 1;
                            }
                        }
                    } // end if bjet1

                    // bjet2
                    if (DEBUG) cout << "-> bjet2_hasgenjet: " << bjet2_hasgenjet << endl;
                    //if (bjet2_hasgenjet)
                    if (true)
                    {
                        bTag = (bjet2_bID_deepCSV > WPtag[1]) ;

                        int   flav =  bjet2_flav;
                        float this_pt =  bjet2_pt;
                        float this_eta = TMath::Abs(bjet2_eta);

                        //  pt (20,30) - eta (0, 0.6)
                        bool goodJet = true;
                        if (this_pt < pt_min || this_pt > pt_max || this_eta < eta_min || this_eta > eta_max) goodJet = false;
                        //cout << "   --> bjet2: " << this_pt << " // " << this_eta << endl;

                        //Fill variables
                        if (goodJet)
                        {
                            if (abs(flav) == 5) // b jets
                            {
                                if (DEBUG) cout << "  it is B"<<endl;
                                n_b_jets += 1;
                                if(bTag) n_b_Mtag += 1;
                            }

                            else if (abs(flav) == 4) // c jets
                            {
                                if (DEBUG) cout << "  it is C"<<endl;
                                n_c_jets += 1;
                                if(bTag) n_c_Mtag += 1;
                            }
                            else // udsg jets
                            {
                                if (DEBUG) cout << "  it is OTHER"<<endl;
                                n_usdg_jets += 1;
                                if(bTag) n_usdg_Mtag += 1;
                            }
                        }
                    } // end if bjet2

                    // other jets
                    for (unsigned int ijet = 0; ijet < jets_pt->size(); ijet++)
                    {
                        if (DEBUG) cout << "-> jets: " << ijet << " hasjetget: " << jets_hasgenjet->at(ijet) << endl;
                        //bool hasgenjet = jets_hasgenjet->at(ijet);
                        //if (hasgenjet)
                        if (true)
                        {
                            bTag = (jets_btag_deepCSV->at(ijet) > WPtag[1]) ;

                            int   flav =  jets_flav->at(ijet);
                            float this_pt =  jets_pt->at(ijet);
                            float this_eta = TMath::Abs(jets_eta->at(ijet));

                            //  pt (20,30) - eta (0, 0.6)
                            bool goodJet = true;
                            if (this_pt < pt_min || this_pt > pt_max || this_eta < eta_min || this_eta > eta_max) goodJet = false;

                            if (goodJet)
                            {
                                if (abs(flav) == 5) // b jets
                                {
                                    if (DEBUG) cout << "  it is B"<<endl;
                                    n_b_jets += 1;
                                    if(bTag) n_b_Mtag += 1;
                                }

                                else if (abs(flav) == 4) // c jets
                                {
                                    if (DEBUG) cout << "  it is C"<<endl;
                                    n_c_jets += 1;
                                    if(bTag) n_c_Mtag += 1;
                                }
                                else // udsg jets
                                {
                                    if (DEBUG) cout << "  it is OTHER"<<endl;
                                    n_usdg_jets += 1;
                                    if(bTag) n_usdg_Mtag += 1;
                                }
                            }
                        }
                    } // end loop on other jets

                }// end loop on new_selections


                if (DEBUG)
                {
                  cout << "b1(pt,eta,flav,deepCSV,hasgenjet): " << bjet1_pt << "  " << bjet1_eta << "  " << bjet1_flav << "  " << bjet1_bID_deepCSV << "  " << bjet1_hasgenjet << endl;
                  cout << "b2(pt,eta,flav,deepCSV,hasgenjet): " << bjet2_pt << "  " << bjet2_eta << "  " << bjet2_flav << "  " << bjet2_bID_deepCSV << "  " << bjet2_hasgenjet << endl;
                  for (unsigned int i=0; i<jets_pt->size(); i++)
                  {
                      cout << "Jet" << i << " (pt,eta,flav,deepCSV,hasgenjet): " << jets_pt->at(i) << "  " << jets_eta->at(i) << "  " << jets_flav->at(i) << "  " << jets_btag_deepCSV->at(i) << "  " << jets_hasgenjet->at(i) << endl;
                  }
                  cout << "------------------------" << endl;
                  cout << "fill,nb,nc,nusdg,nB,nC,nUSDG: "<< fillEvent << " ; " << n_b_jets << " ; " << n_c_jets << " ; " << n_usdg_jets << " ; " << n_b_Mtag << " ; " << n_c_Mtag << " ; " << n_usdg_Mtag << endl;
                  cout << "------------------------" << endl;
                }

                if (fillEvent)
                {
                  tOut->Fill();
                  //cout << "  values b-c-usdg: " << n_b_jets << "  " << n_c_jets << "  " << n_usdg_jets << endl;
                  //cout << "  sample_id: " << sample_id  << endl;
                }
            } // end loop on events
            for (unsigned int isel = 0; isel < new_selections.size(); isel++) delete TTF[isel];
            delete[] TTF;

        } // end loop on samples

    } //end save sync tree

    // save
    fOut->Write();
}
