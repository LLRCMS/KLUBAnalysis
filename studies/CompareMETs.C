void CompareMETs()
{
    gStyle->SetOptStat(0);

    TFile* f74X = new TFile ("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims32_AllJets_NI/SKIM_TT_merged/total.root");
    TFile* f76X = new TFile ("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims76X/SKIM_TT_merged/total.root");

    TTree* t74X = (TTree*) f74X->Get("HTauTauTree");
    TTree* t76X = (TTree*) f76X->Get("HTauTauTree");

    TCut cut = "pairType == 0 && dau1_pt > 18 && abs (dau1_eta) < 2.1 && dau2_pt > 20 && abs (dau2_eta) < 2.3 && nleps == 0 && bjet1_bID > 0.890 && bjet2_bID > 0.890 && dau1_iso < 0.1 && dau2_iso < 3.0 && isOS != 0";

    t74X -> Draw ("met_et>> h74X (80, 0, 400)", cut);
    t76X -> Draw ("met_et>> h76X (80, 0, 400)", cut);

    TH1F* h74X = (TH1F*) gDirectory->Get("h74X");
    TH1F* h76X = (TH1F*) gDirectory->Get("h76X");

    h74X->SetLineWidth(2);
    h76X->SetLineWidth(2);

    h74X->SetLineColor(kRed);
    h76X->SetLineColor(kBlue);

    h74X->Scale(1./h74X->Integral());
    h76X->Scale(1./h76X->Integral());

    maxx = TMath::Max (h74X->GetMaximum(), h76X->GetMaximum());

    h74X->SetMaximum(1.15*maxx);
    h74X->SetMinimum(0);

    h74X->SetTitle("#slash{E}_{T} comparison;#slash{E}_{T}[GeV]; a.u.");
    h74X->GetYaxis()->SetTitleOffset(1.4);

    TCanvas* c1 = new TCanvas ("c1", "c1", 600, 600);
    c1->SetFrameLineWidth(3);

    h74X->Draw("hist");
    h76X->Draw("hist same");

    TLegend* l2 = new TLegend(0.5, 0.65, 0.89, 0.89);
    l2->SetFillStyle(0);
    l2->SetBorderSize(0);
    l2->AddEntry(h74X, "PT MET noHF (74X)");
    l2->AddEntry(h76X, "PT MET with HF (76X)");

    l2->Draw();

    c1->Print ("METs.pdf", "pdf");
}