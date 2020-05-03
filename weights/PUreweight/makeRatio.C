//void makeRatio (string const &myFileName = "MyMCPileupHistogram_TT2016")
void makeRatio (string const &myFileName = "MyMCPileupHistogram_TT2017_24Apr2020")
//void makeRatio (string const &myFileName = "MyMCPileupHistogram_TT2018")
{
  gROOT->SetBatch();
    //histo MC
    TFile * fileMC = TFile::Open(Form("%s.root",myFileName.c_str()));
    TH1D * hPUMC   = (TH1D*)fileMC->Get("myPUHisto");

    //histo DATA
    //TFile * fileData = TFile::Open("MyDataPileupHistogram_Legacy2016.root");
    TFile * fileData = TFile::Open("MyDataPileupHistogram_Legacy2017.root");
    //TFile * fileData = TFile::Open("MyDataPileupHistogram_Legacy2018.root");
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
