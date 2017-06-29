void WeightFileSingle(TFile* fInX, TFile* fInL, TString destName, int idx, string outFile)
{
    // TFile* fInX = new TFile ("Distros_lambdaOnly_5p_20000ev_52sam_13TeV_JHEP_30K.root");
    // TFile* fInL = new TFile ("outMap.root");

    TH1F* hL = (TH1F*) fInL->Get("allHHNodeMap1D");
    // TH1D* hX = (TH1D*) fInX->Get("lamdaOnly/0_mhh");
    TH1D* hX = (TH1D*) fInX->Get(destName);

    if (hL->GetNbinsX() != hX->GetNbinsX() ) 
    {
        cout << "Histos not matching" << endl;
        return;
    }

    TFile* fOut = new TFile (Form("%s_%i.root", outFile.c_str(), idx), "recreate");
    TH1F* hratio = new TH1F ("hratio", "hratio", 60, 0, 1800); // same as input

    float sum = 0.0;
    for (int i = 1; i <= hL->GetNbinsX(); i++)
    {
        float den = hL->GetBinContent(i);
        float num = hX->GetBinContent(i);
        float ratio;
        if (den == 0) ratio = 0;
        else ratio = num/den;

        sum += ratio;
        hratio->SetBinContent(i, ratio);
    }   

    hratio->Scale(1./sum);

    fOut->cd();
    hratio->Write();
}

void WeightFile2DSingle(TFile* fInX, TFile* fInL, TString destName, int idx, string outFile)
{
    // TFile* fInX = new TFile ("Distros_lambdaOnly_5p_20000ev_52sam_13TeV_JHEP_30K.root");
    // TFile* fInL = new TFile ("outMap.root");

    TH2F* hL = (TH2F*) fInL->Get("allHHNodeMap");
    // TH1D* hX = (TH1D*) fInX->Get("lamdaOnly/0_mhh");
    TH1D* hX = (TH1D*) fInX->Get(destName);

    if (hL->GetNbinsX() != hX->GetNbinsX() || hL->GetNbinsY() != hX->GetNbinsY() ) 
    {
        cout << "Histos not matching" << endl;
        return;
    }

    TFile* fOut = new TFile (Form("%s_%i.root", outFile.c_str(), idx), "recreate");
    // TH2F* hratio = new TH2F ("hratio", "hratio", 60, 0, 1800); // same as input
    // TH2F* hratio = (TH2F*) hL->Clone("hratio2D");
    TH2F* hratio = new TH2F ("hratio2D", "hratio2D", 90, 0, 1800, 10, -1, 1);
    hratio->Reset();

    float sum = 0.0;
    for (int ix = 1; ix <= hL->GetNbinsX(); ix++)
        for (int iy = 1; iy <= hL->GetNbinsY(); iy++)
        {
            float den = hL->GetBinContent(ix, iy);
            float num = hX->GetBinContent(ix, iy);
            float ratio;
            if (den == 0) ratio = 0;
            else ratio = num/den;

            sum += ratio;
            hratio->SetBinContent(ix, iy, ratio);
        }   

    hratio->Scale(1./sum);

    fOut->cd();
    hratio->Write();
    fOut->Close();
}

void WeightFile ()
{
    // for hh reweight into lambda points
    TFile* fInX = new TFile ("Distros_lambdaOnly_5p_20000ev_52sam_13TeV_JHEP_30K.root");
    TFile* fInL = new TFile ("outMap.root");
    for (int i = 0; i <= 51; i++)
    {
        cout << i << endl;
        WeightFileSingle (fInX, fInL, Form("lamdaOnly/%i_mhh", i), i, "lambdaMap");
    }

    // // for hh reweight into v3 clusters
    // TFile* fInX = new TFile ("Distros_benchmarks_5p_500000ev_12sam_13TeV_JHEPv3.root");
    // TFile* fInL = new TFile ("outMap.root");
    // for (int i = 0; i <= 11; i++) // clus name in [0,12]
    // {
    //     cout << i << endl;
    //     WeightFile2DSingle (fInX, fInL, Form("%i_bin1", i), i, "clusv3Map");
    // }

    // // to reweight the 1507 points of the 5d space
    // TFile* fInX = new TFile ("Distros_all_5p_20000ev_1507sam_13TeV_JHEPv3.root");
    // TFile* fInL = new TFile ("outMap.root");
    // for (int i = 0; i <= 1506; i++) // point name in [0,1506]
    // {
    //     cout << i << endl;
    //     WeightFile2DSingle (fInX, fInL, Form("%i_bin1", i), i, "all5DplanePointsMap");
    // }

}