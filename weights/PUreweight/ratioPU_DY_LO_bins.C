void ratioPU_DY_LO_bins (uint begin = 0, uint max=200000000)
{
    // Read input files and create a TChain
    TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;
    std::ifstream infile("../../inputFiles/Legacy2017_backgrounds/DY_merged.txt");
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

    // Declare 1 hist for each bin
    //TH1D * myPUHisto = new TH1D("myPUHisto","myPUHisto",100,0,100);
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


    // Activate branches and loop on entries
    float npu;
    int njets;
    int nb;
    
    bigChain->SetBranchAddress("npu", &npu);
    bigChain->SetBranchAddress("lheNOutPartons", &njets);
    bigChain->SetBranchAddress("lheNOutB", &nb);
    
    bigChain->SetBranchStatus("*", 0);
    bigChain->SetBranchStatus("npu", 1);
    bigChain->SetBranchStatus("lheNOutPartons", 1);
    bigChain->SetBranchStatus("lheNOutB", 1);

    cout << "...start big loop" << endl;
    cout << "...begin " << begin << " max " << max << endl;

    uint stop = begin + max;
    for (uint i = begin; i < stop; ++i)
    {
        int got = bigChain->GetEntry(i);
        if (got == 0) break;
        if (i % 100000 == 0) cout << i << endl;

        if      (njets==0 && nb==0) myPUHisto_0j0b->Fill(npu);
        else if (njets==1 && nb==0) myPUHisto_1j0b->Fill(npu);
        else if (njets==1 && nb==1) myPUHisto_1j1b->Fill(npu);
        else if (njets==2 && nb==0) myPUHisto_2j0b->Fill(npu);
        else if (njets==2 && nb==1) myPUHisto_2j1b->Fill(npu);
        else if (njets==2 && nb==2) myPUHisto_2j2b->Fill(npu);
        else if (njets==3 && nb==0) myPUHisto_3j0b->Fill(npu);
        else if (njets==3 && nb==1) myPUHisto_3j1b->Fill(npu);
        else if (njets==3 && nb==2) myPUHisto_3j2b->Fill(npu);
        else if (njets==3 && nb==3) myPUHisto_3j3b->Fill(npu);
        else if (njets==4 && nb==0) myPUHisto_4j0b->Fill(npu);
        else if (njets==4 && nb==1) myPUHisto_4j1b->Fill(npu);
        else if (njets==4 && nb==2) myPUHisto_4j2b->Fill(npu);
        else if (njets==4 && nb==3) myPUHisto_4j3b->Fill(npu);
        else if (njets==4 && nb==4) myPUHisto_4j4b->Fill(npu);

    }

    // Declare output file and save histos
    TFile *myFile = new TFile("MyMCPileupHistogram_DYspecial2017.root","RECREATE");
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
