void WeightFileSingle(TFile* fInX, TFile* fInL, TString destName, int idx)
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

    TFile* fOut = new TFile (Form("map_%i.root", idx), "recreate");
    TH1F* hratio = new TH1F ("hratio", "hratio", 60, 0, 1800); // same as input

    float sum = 0.0;
    for (int i = 1; i < hL->GetNbinsX(); i++)
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


void WeightFile ()
{
    TFile* fInX = new TFile ("Distros_lambdaOnly_5p_20000ev_52sam_13TeV_JHEP_30K.root");
    TFile* fInL = new TFile ("outMap.root");
    for (int i = 0; i <= 51; i++)
    {
        cout << i << endl;
        WeightFileSingle (fInX, fInL, Form("lamdaOnly/%i_mhh", i), i);
    }
}