void makeRatio_2017special (string const &myFileName = "MyMCPileupHistogram_ZH_HToBB_ZToLL_2017")
//void makeRatio_2017special (string const &myFileName = "MyMCPileupHistogram_ZH_HToBB_ZToQQ_2017")
//void makeRatio_2017special (string const &myFileName = "MyMCPileupHistogram_ggZH_HToBB_ZToLL_2017")
//void makeRatio_2017special (string const &myFileName = "MyMCPileupHistogram_WplusHToTauTau_2017")
//void makeRatio_2017special (string const &myFileName = "MyMCPileupHistogram_WminusHToTauTau_2017")
{
  gROOT->SetBatch();
    //histo MC
    TFile * fileMC = TFile::Open(Form("%s.root",myFileName.c_str()));
    TH1D * hPUMC   = (TH1D*)fileMC->Get("myPUHisto");

    //histo DATA
    TFile * fileData = TFile::Open("MyDataPileupHistogram_Legacy2017.root");
    TH1D * hPUData = (TH1D*)fileData->Get("pileup");

    //histo ratio
    TH1D * hRatio = new TH1D("hRatio","hRatio",100,0,100);

    hRatio->Add(hPUData);
    hRatio->Divide(hPUMC);

    for (int i = 0 ; i < 100 ; ++i)
        cout << hRatio->GetBinContent(i+1)/hRatio->Integral() << endl;

    hPUMC->SetLineColor(kBlack);
    hPUData->SetLineColor(kRed);
    hPUMC->DrawNormalized();
    hPUData->DrawNormalized("same");
    hRatio->DrawNormalized("same");
}
