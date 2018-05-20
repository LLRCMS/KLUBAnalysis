void makeRatio () 
{
    //histo MC
    //TFile * fileMC = TFile::Open("MyMCPileupHistogram.root");
    //TFile * fileMC = TFile::Open("TTHad_MyMCPileupHistogram.root");
    TFile * fileMC = TFile::Open("TTSemiLep_MyMCPileupHistogram.root");
    TH1D * hPUMC   = (TH1D*)fileMC->Get("myPUHisto");

    cout << "MC: " << hPUMC->GetName() << endl;

    //histo DATA
    //TFile * fileData = TFile::Open("MyDataPileupHistogram_2016data.root");
    //TH1D * hPUData = (TH1D*)fileData->Get("pileup");
    // //histo Data26
    // TFile * file26 = TFile::Open("MyDataPileupHistogram_92fb.root");
    // TH1F * hPU26 = (TH1F*)file26->Get("pileup");
    // //histo Data40
    // TFile * file40 = TFile::Open("MyDataPileupHistogram_129fb.root");
    // TH1F * hPU40 = (TH1F*)file40->Get("pileup");
    //TFile * fileData = TFile::Open("MyDataPileupHistogram_2017Bdata.root");
    //TH1D * hPUData = (TH1D*)fileData->Get("pileup");
    TFile * fileData = TFile::Open("MyDataPileupHistogram_2017Fdata.root");
    TH1D * hPUData = (TH1D*)fileData->Get("pileup");
    //TFile * fileData = TFile::Open("MyDataPileupHistogram_2017data.root");
    //TH1D * hPUData = (TH1D*)fileData->Get("pileup");

    cout << "DATA: " << hPUData->GetName() << endl;

    //histo ratio
    TH1D * hRatio = new TH1D("hRatio","hRatio",100,0,100);
    // TH1F * hRatio40 = new TH1F("hRatio40","hRatio40",100,0,100);

    // TH1F * hRatioScale = new TH1F("hRatioScale","hRatioScale",100,0,100);

    hRatio->Add(hPUData);
    hRatio->Divide(hPUMC);
    // hRatio40->Add(hPU40);
    // hRatio40->Divide(hPUMC);

    // hRatioScale->Add(hRatio26);
    // hRatioScale->Divide(hRatio40);

    // hRatioScale->Draw();

    for (int i = 0 ; i < 100 ; ++i)
        //cout << "npuSummer16_36fb[" << i << "] = " <<hRatio->GetBinContent(i+1)/hRatio->Integral() <<  ";" << endl;
        //cout << "npuFall17_4c7fb[" << i << "] = " <<hRatio->GetBinContent(i+1)/hRatio->Integral() <<  ";" << endl; // Run 2017B
        cout << "npuFall17_13c4fb[" << i << "] = " <<hRatio->GetBinContent(i+1)/hRatio->Integral() <<  ";" << endl; // Run 2017F

    hPUMC->SetLineColor(kBlack);
    hPUData->SetLineColor(kRed);
    hPUMC->DrawNormalized();
    hPUData->DrawNormalized("same");
    hRatio->DrawNormalized("same");
}