//void makeRatio_2017special (string const &myFileName = "MyMCPileupHistogram_WZ2017")
//void makeRatio_2017special (string const &myFileName = "MyMCPileupHistogram_ZH2017")
//void makeRatio_2017special (string const &myFileName = "MyMCPileupHistogram_ggZHtoLL2017")
//void makeRatio_2017special (string const &myFileName = "MyMCPileupHistogram_ggZHtoNuNu2017")
//void makeRatio_2017special (string const &myFileName = "MyMCPileupHistogram_Wplus2017")
void makeRatio_2017special (string const &myFileName = "MyMCPileupHistogram_Wminus2017")
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
