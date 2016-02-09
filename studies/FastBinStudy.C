void FastBinStudy()
{
    float binning[] = {225, 250, 275, 300, 350, 400, 450, 500, 600, 700, 850, 1000, 1020};
    int nbins = sizeof(binning)/sizeof(float) -1;

    //TFile * f = new TFile ("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims32_AllJets_NI/SKIM_TT_merged/total.root");
    TFile * f = new TFile ("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims32_AllJets_NI/SKIM_TT_merged/total.root");
    TTree* tr = (TTree*) f->Get("HTauTauTree");

    // int mass [] = {250, 260, 270, 280, 300, 320, 340, 350, 400, 450, 500, 550, 600, 650, 700, 800, 900};
    // int nmasses = sizeof (mass) / sizeof(int);

    // int colors [] = {kBlack, kRed-3, kRed-1, kRed+1, kRed+3, kBlue-3, kBlue-1, kBlue+1, kBlue+3, kGreen-3, kGreen-1, kGreen+1, kGreen+3, kYellow-3, kYellow-1, kYellow+1, kYellow+3, kCyan-3, kCyan-1, kCyan+1, kCyan+3};

    // TH1F**  hSig  = new TH1F* [nmasses];
    // TTree** tSig = new TTree* [nmasses];
    // TFile** fSig = new TFile* [nmasses];

    //TCut sel = "pairType == 0 && dau1_pt > 18 && abs (dau1_eta) < 2.1 && dau2_pt > 20 && abs (dau2_eta) < 2.3 && nleps == 0 && bjet1_bID > 0.890 && bjet2_bID > 0.890 && bH_mass > 80 && bH_mass < 160 && tauH_SVFIT_mass > 80 && tauH_SVFIT_mass < 160 && dau1_iso < 0.1 && dau2_iso < 3.0 && isOS != 0" ;
    //TCut sel = "pairType == 1 && dau1_pt > 23 && abs (dau1_eta) < 2.1 && dau2_pt > 20 && abs (dau2_eta) < 2.3 && nleps == 0 && bjet1_bID > 0.890 && bjet2_bID > 0.890 && bH_mass > 80 && bH_mass < 160 && tauH_SVFIT_mass > 80 && tauH_SVFIT_mass < 160 && dau1_iso < 0.1 && dau2_iso < 3.0 && isOS != 0" ;
    TCut sel = "pairType == 2 && dau1_pt > 45 && abs (dau1_eta) < 2.1 && dau2_pt > 45 && abs (dau2_eta) < 2.1 && nleps == 0 && bjet1_bID > 0.605 && bjet2_bID > 0.605 && bH_mass > 80 && bH_mass < 160 && tauH_SVFIT_mass > 80 && tauH_SVFIT_mass < 160 && dau1_iso < 0.1 && dau2_iso < 3.0 && isOS != 0" ;
    // TLegend* l = new TLegend (0.6, 0.6, 0.9, 0.9);

    // for (int im = 0; im < nmasses; im++)
    // {
    //     fSig[im] = new TFile (Form ("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims32_AllJets_NI/SKIM_GluGluToRadionToHHTo2B2Tau_M-%i/total.root", mass[im]));
    //     tSig[im] = (TTree*) fSig[im]->Get("HTauTauTree");
    //     hSig[im] = new TH1F (Form("hAA_%i", im), "hAA", nbins, binning);
    //     tSig[im] ->Draw(Form("HHKin_mass>>hAA_%i", im), sel);
    //     //hSig[im]->SetLineColor(im+1);
    //     hSig[im]->SetLineColor(colors[im]);
    //     hSig[im]->SetLineWidth(2);
    //     l->AddEntry (hSig[im], Form("m %i", mass[im]), "l");
    // }

    TH1F* hAA = new TH1F ("hAA", "hAA", nbins, binning);
    hAA->Sumw2();
    
        tr->Draw("HHKin_mass>>hAA", sel);

    for (int i = 1; i <= nbins; i++) cout << i << " " << hAA->GetBinContent(i) << endl;
    
    hAA->Scale (1., "width");
    
    TCanvas* cd2 = new TCanvas;
    hAA->Draw();


    // TCanvas* cd2 = new TCanvas;

    // hSig[0]->DrawNormalized();
    // for (int im = 1; im < nmasses; im++) hSig[im]->DrawNormalized("same");

    // l->Draw();
}