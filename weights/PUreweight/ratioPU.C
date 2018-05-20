void ratioPU (uint begin = 0, uint max=50000000) 
{

  TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;

    //std::ifstream infile("/home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Files_7Feb2016/TT_powheg_semiLep_7Feb2017.txt");
    //std::ifstream infile("/gwpool/users/brivio/Hhh_1718/syncFeb2018/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Fall17_MC/2_TTToHadronic_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt");
    std::ifstream infile("/gwpool/users/brivio/Hhh_1718/syncFeb2018/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Fall17_MC/3_TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt");

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

  // bigChain->GetEntries();
  TH1D * myPUHisto = new TH1D("myPUHisto","myPUHisto",100,0,100);
  // for (int i = 0 ; i < bigChain->GetEntriesFast() ; ++i) {
  //   if (i%10000 == 0) cout << "Done " << i << " of " << bigChain->GetEntriesFast() << " entries" << endl;
    // bigChain->Draw("PUNumInteractions >> myPUHisto");
  
  // int PUNumInteractions;
  // bigChain->SetBranchAddress("PUNumInteractions", &PUNumInteractions);
  // bigChain->SetBranchStatus("*", 0);
  // bigChain->SetBranchStatus("PUNumInteractions", 1);

  // cout << "...start big loop" << endl;
  // uint stop = begin + max;
  // for (uint i = begin; i < stop; ++i) {
  //   int got = bigChain->GetEntry(i);
  //   if (got == 0) break;
  //   if (i % 1000000 == 0) cout << i << endl;
  //   myPUHisto->Fill(PUNumInteractions);
  // }

  float npu;
  bigChain->SetBranchAddress("npu", &npu);
  bigChain->SetBranchStatus("*", 0);
  bigChain->SetBranchStatus("npu", 1);

  cout << "...start big loop" << endl;
  uint stop = begin + max;
  for (uint i = begin; i < stop; ++i) {
    int got = bigChain->GetEntry(i);
    if (got == 0) break;
    if (i % 1000000 == 0) cout << i << endl;
    myPUHisto->Fill(npu);
  }

  // }
  //TFile *myFile = new TFile(Form("MyMCPileupHistogram%i.root" , begin),"RECREATE");
  //TFile *myFile = new TFile(Form("TTHad_MyMCPileupHistogram%i.root" , begin),"RECREATE");
  TFile *myFile = new TFile(Form("TTSemiLep_MyMCPileupHistogram%i.root" , begin),"RECREATE");
  myPUHisto->Write();
}