void doMap (TH1* hFrom, TH1* hTo, TString outFileName)
{
    if (hFrom->GetNbinsX() != hTo->GetNbinsX()) 
    {
        cout << "Histos not matching" << endl;
        return;
    }

    if (hFrom->GetNbinsX() != 60 )
    {
        cout << "** histos do not have 60 bins" << endl;
        return;
    }

    if (hFrom->GetBinLowEdge(61) != 1800 )
    {
        cout << "** histos are not limited to 1800 in mh" << endl;
        return;
    }

    TFile* fOut = new TFile (outFileName, "recreate");
    TH1F* hratio = new TH1F ("hratio", "hratio", 60, 0, 1800); // same as input

    float sum = 0.0;
    for (int i = 1; i < hFrom->GetNbinsX(); i++)
    {
        float den = hFrom->GetBinContent(i);
        float num = hTo->GetBinContent(i);
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


void RemapForXanda()
{
    TFile* fFrom = new TFile ("outMap.root");
    TH1F* hFrom = (TH1F*) fFrom->Get("allHHNodeMap1D");

    // // for central values of clusters
    // TFile* fTo = new TFile ("Distros_5p_500000ev_12sam_13TeV_JHEP_500K.root"); // clusters - central
    // for (int i = 0; i < 12; i++)
    // {
    //     cout << "... doing central cluster " << i << endl;
    //     TH1D* hTo = (TH1D*) fTo->Get(Form("%i_mhh", i));
    //     TString outFileName = Form("outMap_cluster_%i_forXanda.root", i);
    //     doMap (hFrom, hTo, outFileName);
    // }


    // for outliers
    TFile* fTo = new TFile ("Distros_envelope_5p_20000ev_6sam_13TeV.root"); // clusters - outliers
    std::vector<int> clusToDo;
    clusToDo.push_back(1);
    clusToDo.push_back(10);

    for (int iClus : clusToDo)
    {
        cout << " CLUSTER: " << iClus << endl;
        for (int i = 0; i < 6; i++)
        {
            cout << "  .. outlier " << i << endl;
            TH1D* hTo = (TH1D*) fTo->Get(Form("clu%i/%i_mhh", iClus, i));
            TString outFileName = Form("outMap_clusNum%i_outlier_%i_forXanda.root", iClus, i);
            doMap (hFrom, hTo, outFileName);
        }
    }


}
