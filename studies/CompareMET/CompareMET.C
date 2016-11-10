float max3 (float v1, float v2, float v3)
{
    float max = std::max(v1, v2);
    max = std::max(max, v3);
    return max;
}

TStyle* SetLucaStyle ()
{
    TStyle *LS = new TStyle(*gStyle);
    LS->SetName("LucaStyle");
    LS->SetTitle("Luca Style");

    // pad
    LS ->SetOptStat(0);
    LS->SetTickLength(0.02,"X");
    LS->SetTickLength(0.02,"Y");
    LS->SetPadTickY(1);
    LS->SetPadTickX(1);
    LS->SetPadGridY(1);
    LS->SetPadGridX(1);
    //LS->SetPadTopMargin(0.05);
    //LS->SetPadBottomMargin(0.13);
    LS->SetPadLeftMargin(0.15);
    LS->SetPadBottomMargin(0.12);
    LS->SetFrameLineWidth(2);
    
    // axis
    LS->SetTitleYOffset(1.4);
    LS->SetTitleXOffset(1.0);
    LS->SetLabelOffset(0.009, "XYZ");
    LS->SetTitleSize(0.050, "XYZ");

    LS->SetCanvasDefH(600);
    LS->SetCanvasDefW(600);

    // legend
    LS->SetLegendBorderSize(0);
    //LS->SetLegendFillStyle(0); // NOT AVAILABLE in root, need to set by hand

    // // histos
    // if (histoType == 0)
    // {
    //     LS->SetMarkerStyle(8);
    //     LS->SetMarkerSize(0.8);
    // }

    LS->cd(); // set as the current style
    return LS;
}



void CompareMET(int mRadion=900)
{
    SetLucaStyle();
    // Mu Tau
    // TCut cut = "pairType == 0 && dau1_pt > 19 && abs (dau1_eta) < 2.1 && dau2_pt > 20 && abs (dau2_eta) < 2.3 && nleps == 0 && (dau1_iso/dau1_pt) < 0.1 && dau2_iso < 3.0 && isOS != 0";
    // // Tau Tau
    TCut cut = "pairType == 2 && dau1_pt > 40 && abs (dau1_eta) < 2.1 && dau2_pt > 40 && abs (dau2_eta) < 2.1 && nleps == 0 && dau1_iso < 2.0 && dau2_iso < 2.0 && isOS != 0";


    // int mRadion = 900;
    TFile* fPFMET = new TFile (Form("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_MVAMET_tests_24Mag/SKIM_PFMET_GluGluToRadionToHHTo2B2Tau_M-%i/total.root", mRadion));
    TFile* fMVAMETnoCorr = new TFile (Form("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_MVAMET_tests_24Mag/SKIM_MVAMETNoRecoil_GluGluToRadionToHHTo2B2Tau_M-%i/total.root", mRadion));
    TFile* fMVAMETwithCorr = new TFile (Form("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_MVAMET_tests_24Mag/SKIM_MVAMETWithRecoil_GluGluToRadionToHHTo2B2Tau_M-%i/total.root", mRadion));

    TTree* tPFMET = (TTree*) fPFMET->Get("HTauTauTree");
    TTree* tMVAMETnoCorr = (TTree*) fMVAMETnoCorr->Get("HTauTauTree");
    TTree* tMVAMETwithCorr = (TTree*) fMVAMETwithCorr->Get("HTauTauTree");

    cout << "ENTRIES: " << tPFMET->GetEntries() << " " << tMVAMETnoCorr->GetEntries() << " " << tMVAMETwithCorr->GetEntries() << endl;

    // histos
    TH1F* hSVFIT_PFMET = new TH1F ("hSVFIT_PFMET", "hSVFIT_PFMET", 50, 30, 230);
    TH1F* hSVFIT_MVAMETnoCorr = new TH1F ("hSVFIT_MVAMETnoCorr", "hSVFIT_MVAMETnoCorr", 50, 30, 230);
    TH1F* hSVFIT_MVAMETwithCorr = new TH1F ("hSVFIT_MVAMETwithCorr", "hSVFIT_MVAMETwithCorr", 50, 30, 230);

    TH1F* hHHKinFit_PFMET = new TH1F ("hHHKinFIT_PFMET", "hHHKinFIT_PFMET", 100, 0.5*mRadion, 1.5*mRadion);
    TH1F* hHHKinFit_MVAMETnoCorr = new TH1F ("hHHKinFIT_MVAMETnoCorr", "hHHKinFIT_MVAMETnoCorr", 100, 0.5*mRadion, 1.5*mRadion);
    TH1F* hHHKinFit_MVAMETwithCorr = new TH1F ("hHHKinFIT_MVAMETwithCorr", "hHHKinFIT_MVAMETwithCorr", 100, 0.5*mRadion, 1.5*mRadion);

    TH1F* hMET_PFMET = new TH1F ("hMET_PFMET", "hMET_PFMET", 100, 0, 500);
    TH1F* hMET_MVAMETnoCorr = new TH1F ("hMET_MVAMETnoCorr", "hMET_MVAMETnoCorr", 100, 0, 500);
    TH1F* hMET_MVAMETwithCorr = new TH1F ("hMET_MVAMETwithCorr", "hMET_MVAMETwithCorr", 100, 0, 500);

    TH1F* hmT_PFMET = new TH1F ("hmT_PFMET", "hmT_PFMET", 50, 0, 150);
    TH1F* hmT_MVAMETnoCorr = new TH1F ("hmT_MVAMETnoCorr", "hmT_MVAMETnoCorr", 50, 0, 150);
    TH1F* hmT_MVAMETwithCorr = new TH1F ("hmT_MVAMETwithCorr", "hmT_MVAMETwithCorr", 50, 0, 150);


    tPFMET->Draw("tauH_SVFIT_mass>>hSVFIT_PFMET", cut);
    tMVAMETnoCorr->Draw("tauH_SVFIT_mass>>hSVFIT_MVAMETnoCorr", cut);
    tMVAMETwithCorr->Draw("tauH_SVFIT_mass>>hSVFIT_MVAMETwithCorr", cut);

    tPFMET->Draw("HHKin_mass_raw>>hHHKinFIT_PFMET", cut);
    tMVAMETnoCorr->Draw("HHKin_mass_raw>>hHHKinFIT_MVAMETnoCorr", cut);
    tMVAMETwithCorr->Draw("HHKin_mass_raw>>hHHKinFIT_MVAMETwithCorr", cut);

    tPFMET->Draw("met_et>>hMET_PFMET", cut);
    tMVAMETnoCorr->Draw("met_et>>hMET_MVAMETnoCorr", cut);
    tMVAMETwithCorr->Draw("met_et>>hMET_MVAMETwithCorr", cut);

    tPFMET->Draw("mT>>hmT_PFMET", cut);
    tMVAMETnoCorr->Draw("mT>>hmT_MVAMETnoCorr", cut);
    tMVAMETwithCorr->Draw("mT>>hmT_MVAMETwithCorr", cut);

    // cout << hMET_PFMET->Integral() << " " << hMET_MVAMETnoCorr->Integral() << " " << hMET_MVAMETwithCorr->Integral() << endl;

    hSVFIT_PFMET->SetLineColor(kRed);
    hSVFIT_MVAMETnoCorr->SetLineColor(kGreen+2);
    hSVFIT_MVAMETwithCorr->SetLineColor(kBlue);
    hSVFIT_PFMET->SetLineWidth(2);
    hSVFIT_MVAMETnoCorr->SetLineWidth(2);
    hSVFIT_MVAMETwithCorr->SetLineWidth(2);
    hSVFIT_PFMET->Scale(1./hSVFIT_PFMET->Integral());
    hSVFIT_MVAMETnoCorr->Scale(1./hSVFIT_MVAMETnoCorr->Integral());
    hSVFIT_MVAMETwithCorr->Scale(1./hSVFIT_MVAMETwithCorr->Integral());

    hHHKinFit_PFMET->SetLineColor(kRed);
    hHHKinFit_MVAMETnoCorr->SetLineColor(kGreen+2);
    hHHKinFit_MVAMETwithCorr->SetLineColor(kBlue);
    hHHKinFit_PFMET->SetLineWidth(2);
    hHHKinFit_MVAMETnoCorr->SetLineWidth(2);
    hHHKinFit_MVAMETwithCorr->SetLineWidth(2);
    hHHKinFit_PFMET->Scale(1./hHHKinFit_PFMET->Integral());
    hHHKinFit_MVAMETnoCorr->Scale(1./hHHKinFit_MVAMETnoCorr->Integral());
    hHHKinFit_MVAMETwithCorr->Scale(1./hHHKinFit_MVAMETwithCorr->Integral());

    hMET_PFMET->SetLineColor(kRed);
    hMET_MVAMETnoCorr->SetLineColor(kGreen+2);
    hMET_MVAMETwithCorr->SetLineColor(kBlue);
    hMET_PFMET->SetLineWidth(2);
    hMET_MVAMETnoCorr->SetLineWidth(2);
    hMET_MVAMETwithCorr->SetLineWidth(2);
    hMET_PFMET->Scale(1./hMET_PFMET->Integral());
    hMET_MVAMETnoCorr->Scale(1./hMET_MVAMETnoCorr->Integral());
    hMET_MVAMETwithCorr->Scale(1./hMET_MVAMETwithCorr->Integral());

    hmT_PFMET->SetLineColor(kRed);
    hmT_MVAMETnoCorr->SetLineColor(kGreen+2);
    hmT_MVAMETwithCorr->SetLineColor(kBlue);
    hmT_PFMET->SetLineWidth(2);
    hmT_MVAMETnoCorr->SetLineWidth(2);
    hmT_MVAMETwithCorr->SetLineWidth(2);
    hmT_PFMET->Scale(1./hmT_PFMET->Integral());
    hmT_MVAMETnoCorr->Scale(1./hmT_MVAMETnoCorr->Integral());
    hmT_MVAMETwithCorr->Scale(1./hmT_MVAMETwithCorr->Integral());

    TLegend* leg = new TLegend (0.12, 0.93, 0.93, 0.99);
    leg->SetNColumns(3);
    leg->AddEntry(hSVFIT_PFMET, "PF #slash{E}_{T}" , "l");
    leg->AddEntry(hSVFIT_MVAMETnoCorr, "MVA #slash{E}_{T}, w/o recoil corr." , "l");
    leg->AddEntry(hSVFIT_MVAMETwithCorr, "MVA #slash{E}_{T}, w/ recoil corr." , "l");

    // testo
    float shiftText = 0.11;

    TPaveText* textSVFIT_PFMET = new TPaveText(0.63, 0.78, 0.88, 0.88, "blNDC");
    textSVFIT_PFMET->SetTextFont(40);
    // textSVFIT_PFMET->SetTextSize(10);   
    textSVFIT_PFMET->AddText(Form("mean:            %.1f", hSVFIT_PFMET->GetMean()));
    textSVFIT_PFMET->AddText(Form("RMS/mean:   %.3f", hSVFIT_PFMET->GetRMS() / hSVFIT_PFMET->GetMean()));
    textSVFIT_PFMET->SetLineColor(hSVFIT_PFMET->GetLineColor());
    textSVFIT_PFMET->SetTextColor(hSVFIT_PFMET->GetLineColor());
    textSVFIT_PFMET->SetShadowColor(0);
    textSVFIT_PFMET->SetBorderSize(5);
    textSVFIT_PFMET->SetFillColor(kWhite);

    TPaveText* textSVFIT_MVAMETnoCorr = new TPaveText(0.63, 0.78-shiftText, 0.88, 0.88-shiftText, "blNDC");
    textSVFIT_MVAMETnoCorr->SetTextFont(40);
    // textSVFIT_MVAMETnoCorr->SetTextSize(10);   
    textSVFIT_MVAMETnoCorr->AddText(Form("mean:            %.1f", hSVFIT_MVAMETnoCorr->GetMean()));
    textSVFIT_MVAMETnoCorr->AddText(Form("RMS/mean:   %.3f", hSVFIT_MVAMETnoCorr->GetRMS() / hSVFIT_MVAMETnoCorr->GetMean()));
    textSVFIT_MVAMETnoCorr->SetLineColor(hSVFIT_MVAMETnoCorr->GetLineColor());
    textSVFIT_MVAMETnoCorr->SetTextColor(hSVFIT_MVAMETnoCorr->GetLineColor());
    textSVFIT_MVAMETnoCorr->SetShadowColor(0);
    textSVFIT_MVAMETnoCorr->SetBorderSize(5);
    textSVFIT_MVAMETnoCorr->SetFillColor(kWhite);

    TPaveText* textSVFIT_MVAMETwithCorr = new TPaveText(0.63, 0.78-2.*shiftText, 0.88, 0.88-2.*shiftText, "blNDC");
    textSVFIT_MVAMETwithCorr->SetTextFont(40);
    // textSVFIT_MVAMETwithCorr->SetTextSize(10);   
    textSVFIT_MVAMETwithCorr->AddText(Form("mean:            %.1f", hSVFIT_MVAMETwithCorr->GetMean()));
    textSVFIT_MVAMETwithCorr->AddText(Form("RMS/mean:   %.3f", hSVFIT_MVAMETwithCorr->GetRMS() / hSVFIT_MVAMETwithCorr->GetMean()));
    textSVFIT_MVAMETwithCorr->SetLineColor(hSVFIT_MVAMETwithCorr->GetLineColor());
    textSVFIT_MVAMETwithCorr->SetTextColor(hSVFIT_MVAMETwithCorr->GetLineColor());
    textSVFIT_MVAMETwithCorr->SetShadowColor(0);
    textSVFIT_MVAMETwithCorr->SetBorderSize(5);
    textSVFIT_MVAMETwithCorr->SetFillColor(kWhite);


    TCanvas* c1 = new TCanvas("c1", "c1", 600, 600);
    float max = max3 (hSVFIT_PFMET->GetMaximum(), hSVFIT_MVAMETnoCorr->GetMaximum(), hSVFIT_MVAMETwithCorr->GetMaximum());
    hSVFIT_PFMET->SetTitle(";m_{#tau#tau}^{SVFit} [GeV]; a.u.");
    hSVFIT_PFMET->SetMaximum(1.2*max);
    hSVFIT_PFMET->Draw();
    hSVFIT_MVAMETnoCorr->Draw("same");
    hSVFIT_MVAMETwithCorr->Draw("same");
    leg->Draw();
    textSVFIT_PFMET->Draw();
    textSVFIT_MVAMETnoCorr->Draw();
    textSVFIT_MVAMETwithCorr->Draw();
    c1->Print(Form("svfit_%i.pdf", mRadion), "pdf");


    TPaveText* textHHKinFit_PFMET = new TPaveText(0.63, 0.78, 0.88, 0.88, "blNDC");
    textHHKinFit_PFMET->SetTextFont(40);
    // textHHKinFit_PFMET->SetTextSize(10);   
    textHHKinFit_PFMET->AddText(Form("mean:            %.1f", hHHKinFit_PFMET->GetMean()));
    textHHKinFit_PFMET->AddText(Form("RMS/mean:   %.3f", hHHKinFit_PFMET->GetRMS() / hHHKinFit_PFMET->GetMean()));
    textHHKinFit_PFMET->SetLineColor(hHHKinFit_PFMET->GetLineColor());
    textHHKinFit_PFMET->SetTextColor(hHHKinFit_PFMET->GetLineColor());
    textHHKinFit_PFMET->SetTextAlign(12);
    textHHKinFit_PFMET->SetShadowColor(0);
    textHHKinFit_PFMET->SetBorderSize(5);
    textHHKinFit_PFMET->SetFillColor(kWhite);

    TPaveText* textHHKinFit_MVAMETnoCorr = new TPaveText(0.63, 0.78-shiftText, 0.88, 0.88-shiftText, "blNDC");
    textHHKinFit_MVAMETnoCorr->SetTextFont(40);
    // textHHKinFit_MVAMETnoCorr->SetTextSize(10);   
    textHHKinFit_MVAMETnoCorr->AddText(Form("mean:            %.1f", hHHKinFit_MVAMETnoCorr->GetMean()));
    textHHKinFit_MVAMETnoCorr->AddText(Form("RMS/mean:   %.3f", hHHKinFit_MVAMETnoCorr->GetRMS() / hHHKinFit_MVAMETnoCorr->GetMean()));
    textHHKinFit_MVAMETnoCorr->SetLineColor(hHHKinFit_MVAMETnoCorr->GetLineColor());
    textHHKinFit_MVAMETnoCorr->SetTextColor(hHHKinFit_MVAMETnoCorr->GetLineColor());
    textHHKinFit_MVAMETnoCorr->SetTextAlign(12);
    textHHKinFit_MVAMETnoCorr->SetShadowColor(0);
    textHHKinFit_MVAMETnoCorr->SetBorderSize(5);
    textHHKinFit_MVAMETnoCorr->SetFillColor(kWhite);

    TPaveText* textHHKinFit_MVAMETwithCorr = new TPaveText(0.63, 0.78-2.*shiftText, 0.88, 0.88-2.*shiftText, "blNDC");
    textHHKinFit_MVAMETwithCorr->SetTextFont(40);
    // textHHKinFit_MVAMETwithCorr->SetTextSize(10);   
    textHHKinFit_MVAMETwithCorr->AddText(Form("mean:            %.1f", hHHKinFit_MVAMETwithCorr->GetMean()));
    textHHKinFit_MVAMETwithCorr->AddText(Form("RMS/mean:   %.3f", hHHKinFit_MVAMETwithCorr->GetRMS() / hHHKinFit_MVAMETwithCorr->GetMean()));
    textHHKinFit_MVAMETwithCorr->SetLineColor(hHHKinFit_MVAMETwithCorr->GetLineColor());
    textHHKinFit_MVAMETwithCorr->SetTextColor(hHHKinFit_MVAMETwithCorr->GetLineColor());
    textHHKinFit_MVAMETwithCorr->SetTextAlign(12);
    textHHKinFit_MVAMETwithCorr->SetShadowColor(0);
    textHHKinFit_MVAMETwithCorr->SetBorderSize(5);
    textHHKinFit_MVAMETwithCorr->SetFillColor(kWhite);

    TCanvas* c2 = new TCanvas("c2", "c2", 600, 600);
    max = max3 (hHHKinFit_PFMET->GetMaximum(), hHHKinFit_MVAMETnoCorr->GetMaximum(), hHHKinFit_MVAMETwithCorr->GetMaximum());
    hHHKinFit_PFMET->SetTitle(";m_{hh}^{HHKinFit} [GeV]; a.u.");
    hHHKinFit_PFMET->SetMaximum(1.1*max);
    hHHKinFit_PFMET->Draw();
    hHHKinFit_MVAMETnoCorr->Draw("same");
    hHHKinFit_MVAMETwithCorr->Draw("same");
    leg->Draw();
    textHHKinFit_PFMET->Draw();
    textHHKinFit_MVAMETnoCorr->Draw();
    textHHKinFit_MVAMETwithCorr->Draw();
    c2->Print(Form("hhkinfit_%i.pdf", mRadion), "pdf");

    TCanvas* c3 = new TCanvas("c3", "c3", 600, 600);
    max = max3 (hMET_PFMET->GetMaximum(), hMET_MVAMETnoCorr->GetMaximum(), hMET_MVAMETwithCorr->GetMaximum());
    hMET_PFMET->SetTitle(";#slash{E}_{T} [GeV]; a.u.");
    hMET_PFMET->SetMaximum(1.1*max);
    hMET_PFMET->Draw();
    hMET_MVAMETnoCorr->Draw("same");
    hMET_MVAMETwithCorr->Draw("same");
    leg->Draw();
    c3->Print(Form("MET_%i.pdf", mRadion), "pdf");

    TCanvas* c4 = new TCanvas("c4", "c4", 600, 600);
    max = max3 (hmT_PFMET->GetMaximum(), hmT_MVAMETnoCorr->GetMaximum(), hmT_MVAMETwithCorr->GetMaximum());
    hmT_PFMET->SetTitle(";m_{T} [GeV]; a.u.");
    hmT_PFMET->SetMaximum(1.1*max);
    hmT_PFMET->Draw();
    hmT_MVAMETnoCorr->Draw("same");
    hmT_MVAMETwithCorr->Draw("same");
    leg->Draw();
    c4->Print(Form("mT_%i.pdf", mRadion), "pdf");

}