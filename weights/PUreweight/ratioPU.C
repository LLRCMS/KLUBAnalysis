//void ratioPU (uint begin = 0, uint max=266483000, string const &infileName = "../../inputFiles/Resonant2016APV_backgrounds/TT.txt", string const &myFileName = "MyMCPileupHistogram_TTUL16APV_23Mar2022")
//void ratioPU (uint begin = 0, uint max=297984000, string const &infileName = "../../inputFiles/Resonant2016_backgrounds/TT.txt", string const &myFileName = "MyMCPileupHistogram_TTUL16_23Mar2022")
void ratioPU (uint begin = 0, uint max=300000000,string const &infileName = "../../filelists_UL/ttbar.txt", string const &myFileName = "MyMCPileupHistogram_TTUL18_28Oct2021")
{

  TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;

  std::ifstream infile(infileName);

    std::string line;
    while (std::getline(infile, line))
    {
        line = line.substr(0, line.find("#", 0)); // remove comments introduced by #
        while (line.find(" ") != std::string::npos) line = line.erase(line.find(" "), 1); // remove white spaces
        while (line.find("\n") != std::string::npos) line = line.erase(line.find("\n"), 1); // remove new line characters
        while (line.find("\r") != std::string::npos) line = line.erase(line.find("\r"), 1); // remove carriage return characters
        if (!line.empty()) // skip empty lines
	  bigChain->Add(line.c_str());
    }

    cout << "...begin " << begin << " max " << max << endl;

    TH1D * myPUHisto = new TH1D("myPUHisto","myPUHisto",100,0,100);

    float npu;
    bigChain->SetBranchAddress("npu", &npu);
    bigChain->SetBranchStatus("*", 0);
    bigChain->SetBranchStatus("npu", 1);

    cout << "...start big loop" << endl;
    uint stop = begin + max;
    for (uint i = begin; i < stop; ++i) {
      int got = bigChain->GetEntry(i);
      if (got == 0) break;
      if (i % 100000 == 0) cout << i << endl;
      myPUHisto->Fill(npu);
    }

    TFile *myFile = new TFile(Form("%s.root",myFileName.c_str()),"RECREATE");
    myPUHisto->Write();
}
