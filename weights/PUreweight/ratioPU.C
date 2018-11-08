void ratioPU (uint begin = 0, uint max=150000000)
{

  TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;

    //std::ifstream infile("/home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Files_7Feb2016/TT_powheg_semiLep_7Feb2017.txt");
    //std::ifstream infile("/gwpool/users/brivio/Hhh_1718/syncFeb2018/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Fall17_MC/2_TTToHadronic_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt");
    //std::ifstream infile("/gwpool/users/brivio/Hhh_1718/syncFeb2018/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Fall17_MC/3_TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt");
    std::ifstream infile("/gwpool/users/brivio/Hhh_1718/syncFeb2018/CMSSW_9_0_0/src/KLUBAnalysis/inputFiles/Files_June2018/DYJets.txt");


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
  TH1D * myPUHisto_0j0b = new TH1D("myPUHisto_0j0b","myPUHisto_0j0b",100,0,100);
  TH1D * myPUHisto_1j0b = new TH1D("myPUHisto_1j0b","myPUHisto_1j0b",100,0,100);
  TH1D * myPUHisto_1j1b = new TH1D("myPUHisto_1j1b","myPUHisto_1j1b",100,0,100);
  TH1D * myPUHisto_2j0b = new TH1D("myPUHisto_2j0b","myPUHisto_2j0b",100,0,100);
  TH1D * myPUHisto_2j1b = new TH1D("myPUHisto_2j1b","myPUHisto_2j1b",100,0,100);
  TH1D * myPUHisto_2j2b = new TH1D("myPUHisto_2j2b","myPUHisto_2j2b",100,0,100);
  TH1D * myPUHisto_3j0b = new TH1D("myPUHisto_3j0b","myPUHisto_3j0b",100,0,100);
  TH1D * myPUHisto_3j1b = new TH1D("myPUHisto_3j1b","myPUHisto_3j1b",100,0,100);
  TH1D * myPUHisto_3j2b = new TH1D("myPUHisto_3j2b","myPUHisto_3j2b",100,0,100);
  TH1D * myPUHisto_3j3b = new TH1D("myPUHisto_3j3b","myPUHisto_3j3b",100,0,100);
  TH1D * myPUHisto_4j0b = new TH1D("myPUHisto_4j0b","myPUHisto_4j0b",100,0,100);
  TH1D * myPUHisto_4j1b = new TH1D("myPUHisto_4j1b","myPUHisto_4j1b",100,0,100);
  TH1D * myPUHisto_4j2b = new TH1D("myPUHisto_4j2b","myPUHisto_4j2b",100,0,100);
  TH1D * myPUHisto_4j3b = new TH1D("myPUHisto_4j3b","myPUHisto_4j3b",100,0,100);
  TH1D * myPUHisto_4j4b = new TH1D("myPUHisto_4j4b","myPUHisto_4j4b",100,0,100);
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
  int lheNOutPartons;
  int lheNOutB;
  //int EventNumber;
  bigChain->SetBranchAddress("npu", &npu);
  bigChain->SetBranchAddress("lheNOutPartons", &lheNOutPartons);
  bigChain->SetBranchAddress("lheNOutB", &lheNOutB);
  //bigChain->SetBranchAddress("EventNumber", &EventNumber);
  bigChain->SetBranchStatus("*", 0);
  bigChain->SetBranchStatus("npu", 1);
  bigChain->SetBranchStatus("lheNOutPartons", 1);
  bigChain->SetBranchStatus("lheNOutB", 1);

  cout << "...start big loop" << endl;
  uint stop = begin + max;
  for (uint i = begin; i < stop; ++i) {
    int got = bigChain->GetEntry(i);
    if (got == 0) break;
    if (i % 1000000 == 0) cout << i << endl;
    /*if (i % 1 == 0)
    {
     cout << "Entry: " <<  i << endl;
     cout << "evt: "<< EventNumber << " - npu: " << npu << " - Jets: " << lheNOutPartons << " - B: " << lheNOutB << endl;
    }*/
    if (lheNOutPartons == 0 && lheNOutB == 0)
        myPUHisto_0j0b->Fill(npu);
    else if (lheNOutPartons == 1 && lheNOutB == 0)
        myPUHisto_1j0b->Fill(npu);
    else if (lheNOutPartons == 1 && lheNOutB == 1)
        myPUHisto_1j1b->Fill(npu);
    else if (lheNOutPartons == 2 && lheNOutB == 0)
        myPUHisto_2j0b->Fill(npu);
    else if (lheNOutPartons == 2 && lheNOutB == 1)
        myPUHisto_2j1b->Fill(npu);
    else if (lheNOutPartons == 2 && lheNOutB == 2)
        myPUHisto_2j2b->Fill(npu);
    else if (lheNOutPartons == 3 && lheNOutB == 0)
        myPUHisto_3j0b->Fill(npu);
    else if (lheNOutPartons == 3 && lheNOutB == 1)
        myPUHisto_3j1b->Fill(npu);
    else if (lheNOutPartons == 3 && lheNOutB == 2)
        myPUHisto_3j2b->Fill(npu);
    else if (lheNOutPartons == 3 && lheNOutB == 3)
        myPUHisto_3j3b->Fill(npu);
    else if (lheNOutPartons == 4 && lheNOutB == 0)
        myPUHisto_4j0b->Fill(npu);
    else if (lheNOutPartons == 4 && lheNOutB == 1)
        myPUHisto_4j1b->Fill(npu);
    else if (lheNOutPartons == 4 && lheNOutB == 2)
        myPUHisto_4j2b->Fill(npu);
    else if (lheNOutPartons == 4 && lheNOutB == 3)
        myPUHisto_4j3b->Fill(npu);
    else if (lheNOutPartons == 4 && lheNOutB == 4)
        myPUHisto_4j4b->Fill(npu);
  }

  // }
  //TFile *myFile = new TFile(Form("MyMCPileupHistogram%i.root" , begin),"RECREATE");
  //TFile *myFile = new TFile(Form("TTHad_MyMCPileupHistogram%i.root" , begin),"RECREATE");
  TFile *myFile = new TFile(Form("DYJets_special_MyMCPileupHistogram%i.root" , begin),"RECREATE");
  myPUHisto_0j0b->Write();
  myPUHisto_1j0b->Write();
  myPUHisto_1j1b->Write();
  myPUHisto_2j0b->Write();
  myPUHisto_2j1b->Write();
  myPUHisto_2j2b->Write();
  myPUHisto_3j0b->Write();
  myPUHisto_3j1b->Write();
  myPUHisto_3j2b->Write();
  myPUHisto_3j3b->Write();
  myPUHisto_4j0b->Write();
  myPUHisto_4j1b->Write();
  myPUHisto_4j2b->Write();
  myPUHisto_4j3b->Write();
  myPUHisto_4j4b->Write();
}
