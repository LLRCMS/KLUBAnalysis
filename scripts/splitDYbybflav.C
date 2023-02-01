#include <stdlib.h>

void splitDYbybflavSingle (TString DYName)
{
    //TString DYName = "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_3/SKIM_DY_Inclusive/total.root" ;

    TString OutDir0b = DYName; OutDir0b.ReplaceAll (TString("/total.root"), "_allgenjets_0b");
    TString OutDir1b = DYName; OutDir1b.ReplaceAll (TString("/total.root"), "_allgenjets_1b");
    TString OutDir2b = DYName; OutDir2b.ReplaceAll (TString("/total.root"), "_allgenjets_2b");

    system (Form("mkdir %s" , OutDir0b.Data())) ;
    system (Form("mkdir %s" , OutDir1b.Data())) ;
    system (Form("mkdir %s" , OutDir2b.Data())) ;

    OutDir0b += "/total.root"; 
    OutDir1b += "/total.root";
    OutDir2b += "/total.root";

    TFile* DYFile = new TFile (DYName);
    TH1F* h_eff = (TH1F*) DYFile->Get("h_eff");

    int bflav1;
    int bflav2;
    int nBhadrons;

    // std::vector<float>* genjet_px = 0;
    // std::vector<float>* genjet_py = 0;
    // std::vector<float>* genjet_pz = 0;
    // std::vector<float>* genjet_e = 0;
    // std::vector<int>* genjet_hadronFlavour = 0;

    TTree* tIn = (TTree*) DYFile->Get("HTauTauTree");

    TFile* f0b = new TFile (OutDir0b, "recreate");
    TFile* f1b = new TFile (OutDir1b, "recreate");
    TFile* f2b = new TFile (OutDir2b, "recreate");

    f0b->cd();
    TTree* t0b = tIn->CloneTree(0);
    f1b->cd();
    TTree* t1b = tIn->CloneTree(0);
    f2b->cd();
    TTree* t2b = tIn->CloneTree(0);

    tIn->SetBranchAddress ("bjet1_flav", &bflav1);
    tIn->SetBranchAddress ("bjet2_flav", &bflav2);
    tIn->SetBranchAddress ("nBhadrons", &nBhadrons);
    // tIn->SetBranchAddress ("genjet_px", &genjet_px);
    // tIn->SetBranchAddress ("genjet_py", &genjet_py);
    // tIn->SetBranchAddress ("genjet_pz", &genjet_pz);
    // tIn->SetBranchAddress ("genjet_e",  &genjet_e);
    // tIn->SetBranchAddress ("genjet_hadronFlavour", &genjet_hadronFlavour);

    int nEvts = tIn->GetEntries();
    for (int iEv = 0; iEv < nEvts; iEv++)
    {
        if (iEv % 10000 == 0) cout << iEv << " / " << nEvts << endl;
        tIn->GetEntry(iEv);

        // TLorentzVector vgj;
        // int nbs = 0;

        // for (unsigned int igj = 0; igj < genjet_px->size(); igj++)
        // {
        //     vgj.SetPxPyPzE(genjet_px->at(igj), genjet_py->at(igj), genjet_pz->at(igj), genjet_e->at(igj));
        //     if (vgj.Pt() > 20 && TMath::Abs(vgj.Eta()) < 2.5)
        //     {
        //         int theFlav = genjet_hadronFlavour->at(igj);
        //         if (abs(theFlav) == 5) nbs++;
        //         if (theFlav == -999) cout << "** warning: gen jet with flav = -999 of pt: " << vgj.Pt() << " eta: " << vgj.Eta() << endl;
        //     }
        // }
        // if (nbs == 0) t0b->Fill();
        // else if (nbs == 1) t1b->Fill();
        // else t2b->Fill(); // >= 2b
	
        if (nBhadrons == 0) t0b->Fill();
        else if (nBhadrons == 1) t1b->Fill();
        else t2b->Fill(); // >= 2b

        // splitting on bjet 1 and 2 only
        // if (bflav1 == 5 && bflav2 == 5) t2b->Fill();
        // else if (bflav1 == 5 || bflav2 == 5) t1b->Fill();
        // else t0b->Fill();
    }

    f0b->cd();
    t0b->Write();
    h_eff->Write();
    f0b->Close();

    f1b->cd();
    t1b->Write();
    h_eff->Write();
    f1b->Close();
    
    f2b->cd();
    t2b->Write();
    h_eff->Write();
    f2b->Close();    
}


void splitDYbybflav()
{
    vector <TString> v;
    //v.push_back("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_DY_Inclusive/total.root");
    //v.push_back("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_DY_100_200/total.root");
    //v.push_back("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_DY_200_400/total.root");
    //v.push_back("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_DY_400_600/total.root");
    //v.push_back("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_DY_600_Inf/total.root");

    // v.push_back("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_DYjetbinned/SKIM_DY_Inclusive/total.root"); 
    //v.push_back("/data_CMS/cms/cadamuro/test_submit_to_tier3/Samples2016_12Lug/SKIM_DY/total.root");
    //v.push_back("/data_CMS/cms/cadamuro/test_submit_to_tier3/Samples2016_14Lug/SKIM_DY/total.root");
    //v.push_back("/data_CMS/cms/cadamuro/test_submit_to_tier3/Samples2016_14Lug_DYupdateSF/SKIM_DY/total.root");
    //v.push_back("/data_CMS2/cadamuro/test_submit_to_tier3/Samples2016_18Lug/SKIM_DY/total.root");
    // v.push_back("/data_CMS2/cadamuro/test_submit_to_tier3/Samples2016_20Lug_HHmassupdownfix/SKIM_DY/total.root");
    //v.push_back ("/data_CMS2/cadamuro/test_submit_to_tier3/Samples2016_23Lug_data12fb/SKIM_DY/total.root");
    //v.push_back("/data_CMS/cms/cadamuro/test_submit_to_tier3/Samples2016_23Lug_data12fb_DYSFeff/SKIM_DY/total.root");
    //v.push_back("/data_CMS2/cadamuro/test_submit_to_tier3/Samples2016_3Sett_data12fb_MT213Sett_prod2/SKIM_DY/total.root");
    //v.push_back("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2016_30Nov2016/SKIM_DY/total.root");
    //v.push_back("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2016_30Nov2016_oldPair/SKIM_DY/total.root");
    //v.push_back("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_5Gen/SKIM_DY/total.root");
    //v.push_back("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_13Giu_All/SKIM_DY/total.root");
    //v.push_back("/data_CMS/cms/amendola/HH2017Skims/Skims2017_17Nov/SKIM_DY/total.root");

    // v.push_back("/data_CMS/cms/amendola/HH2017Skims/SKIMS_24Jul2018_Run2017/SKIM_DY/total.root");
    //v.push_back("/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_0j0b/total.root");
    // v.push_back("/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_1j0b/total.root");
    // v.push_back("/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_1j1b/total.root");
    // v.push_back("/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_2j0b/total.root");
    //  v.push_back("/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_2j1b/total.root");
    // v.push_back("/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_2j2b/total.root");
     ////v.push_back("/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_3j0b/total.root");
    //     v.push_back("/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_3j1b/total.root");
    // v.push_back("/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_3j2b/total.root");
    // v.push_back("/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_3j3b/total.root");
    //v.push_back("/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_4j0b/total.root");
    //v.push_back("/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_4j1b/total.root");
    // v.push_back("/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_4j2b/total.root");
    // v.push_back("/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_4j3b/total.root");
    //v.push_back("/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_4j4b/total.root");

    for (int i = 0; i < v.size(); i++)
    {
        cout << "SAMPLE: " << i << endl;
        splitDYbybflavSingle(v.at(i));
    }
}
