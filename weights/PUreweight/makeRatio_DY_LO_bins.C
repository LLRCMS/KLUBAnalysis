void makeRatio_DY_LO_bins (string const &myFileName = "TTLep_MyMCPileupHistogram", string const &myHistoName = "MyMCPileupHistogram")
{
    gROOT->SetBatch();
    //histo MC
    TFile * fileMC = TFile::Open(Form("%s",myFileName.c_str()));
    TH1D * hPUMC   = (TH1D*)fileMC->Get(Form("%s",myHistoName.c_str()));

    //histo DATA
    TFile * fileData = TFile::Open("MyDataPileupHistogram_Legacy2017.root");
    TH1D * hPUData = (TH1D*)fileData->Get("pileup");

    //histo ratio
    TH1D * hRatio = new TH1D("hRatio","hRatio",100,0,100);

    hRatio->Add(hPUData);
    hRatio->Divide(hPUMC);

    for (int i = 0 ; i < 100 ; ++i)
        cout << "npuFall17_41c3fb[" << i << "] = " <<hRatio->GetBinContent(i+1)/hRatio->Integral() <<  ";" << endl; // Run 2017

    hPUMC->SetLineColor(kBlack);
    hPUData->SetLineColor(kRed);
    hPUMC->DrawNormalized();
    hPUData->DrawNormalized("same");
    hRatio->DrawNormalized("same");
}
