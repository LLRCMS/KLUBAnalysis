void makeRatio (string const &myFileName = "MyMCPileupHistogram_TTToSemiLeptonicUL17")
//void makeRatio (string const &myFileName = "MyMCPileupHistogram_TTUL18_28Oct2021")
//void makeRatio (string const &myFileName = "MyMCPileupHistogram_TTUL16APV_23Mar2022")
//void makeRatio (string const &myFileName = "MyMCPileupHistogram_TTUL16_23Mar2022")
{
    //histo MC
    TFile * fileMC = TFile::Open(Form("%s.root",myFileName.c_str()));
    TH1D * hPUMC   = (TH1D*)fileMC->Get("myPUHisto");

    //histo DATA
	TFile * fileData = TFile::Open("UL_PU_distributions/2017/PileupHistogram-goldenJSON-13tev-2017-69200ub-99bins.root");
    //TFile * fileData = TFile::Open("UL_PU_distributions/2018/PileupHistogram-goldenJSON-13tev-2018-69200ub-99bins.root");
    //TFile * fileData = TFile::Open("UL_PU_distributions/2016/PileupHistogram-goldenJSON-13tev-2016-postVFP-69200ub-99bins.root");
    //TFile * fileData = TFile::Open("UL_PU_distributions/2016/PileupHistogram-goldenJSON-13tev-2016-preVFP-69200ub-99bins.root");

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
