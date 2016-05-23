double propagateError (double E1, double E2, double val1, double val2)
{
    double rel1 = E1 / val1;
    double rel2 = E2 / val2;

    double comb = TMath::Sqrt (rel1*rel1 + rel2*rel2);
    return comb;
}

// does gr2 / gr1
TGraphAsymmErrors* makeRatio (TGraphAsymmErrors* gr1, TGraphAsymmErrors* gr2)
{
    TGraphAsymmErrors* grRatio = new TGraphAsymmErrors;
    
    int npoint = 0;
    for (int i = 0; i < gr1->GetN(); i++ )
    {
        double exlow[2]  = {gr1->GetErrorXlow(i), gr2->GetErrorXlow(i)} ;
        double exhigh[2] = {gr1->GetErrorXhigh(i), gr2->GetErrorXhigh(i)} ;
        double eylow[2]  = {gr1->GetErrorYlow(i), gr2->GetErrorYlow(i)} ;
        double eyhigh[2] = {gr1->GetErrorYhigh(i), gr2->GetErrorYhigh(i)} ;
    
        double x [2];
        double y [2];

        gr1->GetPoint (i, x[0], y[0]);
        gr2->GetPoint (i, x[1], y[1]);

        if (y[0] == 0 || y[1] == 0) continue;
        if (x[0] < 21) continue; // pT cut fro SF trigger only
        cout << x[0] << endl;
        cout << y[0] << " " << y[1] << " ratio = " << y[1]/y[0] << endl;

        grRatio->SetPoint (npoint, x[0], y[1]/y[0]);
        grRatio->SetPointError(npoint, exlow[0], exhigh[0],
            propagateError (eylow[0], eylow[1], y[0], y[1]) * y[1]/y[0] ,
            propagateError (eyhigh[0], eyhigh[1], y[0], y[1]) * y[1]/y[0]
        );
        npoint++;
    }
    return grRatio;
}

void DrawLeptonSF()
{
    // TFile* fIn = new TFile ("../weights/data/Electron/Electron_IdIso0p10_eff.root");
    // TGraphAsymmErrors* grData = (TGraphAsymmErrors*) fIn->Get("ZMassEtaLt1p48_Data");
    // TGraphAsymmErrors* grMC =   (TGraphAsymmErrors*) fIn->Get("ZMassEtaLt1p48_MC");


    vector<TGraphAsymmErrors*> grData;
    vector<TGraphAsymmErrors*> grMC;

    // MUON
    //TFile* fIn = new TFile ("../weights/data/Muon/Muon_SingleMu_eff.root");
    // TFile* fIn = new TFile ("../weights/data/Muon/Muon_IdIso0p1_fall15.root");
    // TFile* fIn = new TFile ("../weights/data/Muon/Muon_IsoMu18_fall15.root");

    // grData.push_back((TGraphAsymmErrors*) fIn->Get("ZMassEtaLt0p9_Data"));
    // grMC.push_back((TGraphAsymmErrors*) fIn->Get("ZMassEtaLt0p9_MC"));

    // grData.push_back((TGraphAsymmErrors*) fIn->Get("ZMassEta0p9to1p2_Data"));
    // grMC.push_back((TGraphAsymmErrors*) fIn->Get("ZMassEta0p9to1p2_MC"));

    // grData.push_back((TGraphAsymmErrors*) fIn->Get("ZMassEtaGt1p2_Data"));
    // grMC.push_back((TGraphAsymmErrors*) fIn->Get("ZMassEtaGt1p2_MC"));


    // TGraphAsymmErrors* grRatio1 = makeRatio (grMC.at(0), grData.at(0)); // data / MC
    // TGraphAsymmErrors* grRatio2 = makeRatio (grMC.at(1), grData.at(1)); // data / MC
    // TGraphAsymmErrors* grRatio3 = makeRatio (grMC.at(2), grData.at(2)); // data / MC

    // ELECTRON
    // TFile* fIn = new TFile ("../weights/data/Electron/Electron_Ele23_fall15.root");
    TFile* fIn = new TFile ("../weights/data/Electron/Electron_IdIso0p1_fall15.root");

    grData.push_back((TGraphAsymmErrors*) fIn->Get("ZMassEtaLt1p48_Data"));
    grMC.push_back((TGraphAsymmErrors*) fIn->Get("ZMassEtaLt1p48_MC"));

    grData.push_back((TGraphAsymmErrors*) fIn->Get("ZMassEtaGt1p48_Data"));
    grMC.push_back((TGraphAsymmErrors*) fIn->Get("ZMassEtaGt1p48_MC"));


    TGraphAsymmErrors* grRatio1 = makeRatio (grMC.at(0), grData.at(0)); // data / MC
    TGraphAsymmErrors* grRatio2 = makeRatio (grMC.at(1), grData.at(1)); // data / MC


    //TGraphAsymmErrors* grData = (TGraphAsymmErrors*) fIn->Get("ZMassEtaLt0p9_Data");
    //TGraphAsymmErrors* grMC =   (TGraphAsymmErrors*) fIn->Get("ZMassEtaLt0p9_MC");

    
    

    TCanvas* c1 = new TCanvas ("c1", "c1", 600, 600);
    c1->SetGridx();
    c1->SetGridy();

    grRatio1->SetMarkerStyle(8);
    grRatio1->SetMarkerSize(1.0);
    grRatio1->SetMarkerColor(kGreen+2);
    grRatio1->SetLineColor(kGreen+2);

    grRatio2->SetMarkerStyle(8);
    grRatio2->SetMarkerSize(1.0);
    grRatio2->SetMarkerColor(kRed);
    grRatio2->SetLineColor(kRed);

    // grRatio3->SetMarkerStyle(8);
    // grRatio3->SetMarkerSize(1.0);
    // grRatio3->SetMarkerColor(kBlue);
    // grRatio3->SetLineColor(kBlue);


    grRatio1->Draw("AP");
    grRatio1->GetXaxis()->SetTitle ("p_{T} [GeV]");
    grRatio1->GetYaxis()->SetTitle ("Data / MC");
    grRatio1->GetXaxis()->SetTitleOffset(1.1);
    grRatio1->GetYaxis()->SetTitleOffset(1.4);
    // grRatio1->GetHistogram()->SetMinimum(0.8); // muon
    grRatio1->GetHistogram()->SetMinimum(0.6); // ele
    grRatio1->GetHistogram()->SetMaximum(1.1);
    // grRatio1->SetTitle ("Muon ID and iso");
    grRatio1->SetTitle ("Electron ID and iso");
    // grRatio1->SetTitle ("Single Electron trigger");
    grRatio1->Draw("AP");
    grRatio2->Draw("P same");
    // grRatio3->Draw("P same");


    TLegend* l1 = new TLegend (0.5, 0.12, 0.88, 0.40);
    l1->SetFillStyle(0);
    l1->SetBorderSize(0);
    // l1->AddEntry(grRatio1, "|#eta| < 0.9", "ple");
    // l1->AddEntry(grRatio2, "0.9 < |#eta| < 1.2", "ple");
    // l1->AddEntry(grRatio3, "|#eta| > 1.2", "ple");
    l1->AddEntry(grRatio1, "|#eta| < 1.48", "ple");
    l1->AddEntry(grRatio2, "|#eta| > 1.48", "ple");
    l1->Draw();

    c1->Print ("ele_id_iso_SF.pdf");

}