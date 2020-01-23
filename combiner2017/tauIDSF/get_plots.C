std::vector<double> find_transitions(TGraph* g, double thresh  = 1)
{
    std::vector<double> v_tr;

    int sign_old = (g->GetY()[0]-thresh)/TMath::Abs(g->GetY()[0]-thresh); // > 0, < 0

    for (uint i = 0; i < g->GetN(); ++i)
    {
        double x = g->GetX()[i];
        double y = g->GetY()[i];
        int sign = (y-thresh)/TMath::Abs(y-thresh);
        if (sign*sign_old < 0) // it's a crossing
        {
            sign_old = sign;
            v_tr.push_back(x);
        }
    }
    return v_tr;
}

/*
std::vector<double> find_transitions_fit(TGraph* g, double thresh  = 1, npts = 1)
{
    std::vector<double> v_tr;
    std::vector<int>    v_idx_tr;

    int sign_old = (g->GetY()[0]-thresh)/TMath::Abs(g->GetY()[0]-thresh); // > 0, < 0

    for (uint i = 0; i < g->GetN(); ++i)
    {
        double x = g->GetX()[i];
        double y = g->GetY()[i];
        int sign = (y-thresh)/TMath::Abs(y-thresh);
        if (sign*sign_old < 0) // it's a crossing
        {
            sign_old = sign;
            v_tr.push_back(x);
            v_idx_tr.push_back(i);
        }
    }

    return v_tr;
}
*/

void get_error(TString filename="higgsCombine.MultiDimFit.mH120.root", , TString parameter="SF_DM0", bool dofit=true, TString fitfunc="pol2")
{
    // gStyle->SetBatch(True)

    // TFile* fIn = new TFile("higgsCombinebbbb.MultiDimFit.mH120.root");
    TFile* fIn = new TFile(filename);
    TTree* tIn = (TTree*) fIn->Get("limit");
    TGraph* gIn = new TGraph;

    float deltaNLL;
    float SF;

    tIn->SetBranchAddress("deltaNLL", &deltaNLL);
    tIn->SetBranchAddress(parameter, &SF);

    // first entry is r = 1 -> skip
    double xmin = 0;
    double xmax = 0;
    for (uint i = 1; i < tIn->GetEntries(); ++i)
    {
        tIn->GetEntry(i);
        gIn->SetPoint(gIn->GetN(), SF, 2*deltaNLL);
        // cout << i << " r: " << r << " -> deltaNLL:" << deltaNLL << endl;
        if (i == 1)
            xmin = SF;
        if (i == tIn->GetEntries() -1)
            xmax = SF;
    }

    gIn->SetMarkerStyle(8);
    gIn->SetMarkerSize(0.8);

    TCanvas* c1 = new TCanvas("c1", "c1", 600, 600);
    gIn->Draw("APL");

    // fit to generate more points
    TF1* fpol2 = new TF1("fpol2", fitfunc.Data(), xmin, xmax);
    
    std::vector<double> cross;    
    if (!dofit){
        cross = find_transitions(gIn, 1);
    }
    else
    {
        gIn->Fit("fpol2");

        TGraph* gCopy = new  TGraph;
        for (uint ipt = 0; ipt < 1000; ++ipt)
        {
            double x = xmin + ipt*(xmax-xmin)/1000;
            gCopy->SetPoint(ipt, x, fpol2->Eval(x));
        }
        gCopy->SetMarkerStyle(8);
        gCopy->SetMarkerSize(0.1);
        gCopy->SetMarkerColor(kBlue);
        gCopy->Draw("P same");

        cross = find_transitions(gCopy, 1);
    }

    TLine* ll = new TLine(xmin, 1, xmax, 1);
    ll->Draw();

    for (auto c : cross){
        cout << c << endl;
    }

    TString testo = "";
    if (cross.size() == 2 ){
        double rlow = cross.at(0);
        double rhigh = cross.at(1);

        // cout << "r = 1.00 " << rlow-1.0 << " +" << rhigh-1.0 << endl;
        printf("r = 1.00 -%.2f +%.2f\n", std::fabs(rlow-1.0), std::fabs(rhigh-1.0));
        testo = Form("r = 1.00 -%.2f +%.2f\n", std::fabs(rlow-1.0), std::fabs(rhigh-1.0));
    }
    else{
        cout << "NO crossings found? size = " << cross.size() << endl;
        testo = "No crossing found";
    }
    TLatex txt (0.1, 0.9, testo);
    txt.SetNDC(true);
    txt.Draw();

    c1->Print(TString("plot_") + filename.ReplaceAll(".root", ".pdf"), "pdf");

}



void get_correlation(TString filename="higgsCombine.MultiDimFit.mH120.root", TString par1 = "SF_DM0", TString par2="SF_DM0", bool dofit=true, TString fitfunc="pol2")
{
  // gStyle->SetBatch(True)
  
  // TFile* fIn = new TFile("higgsCombinebbbb.MultiDimFit.mH120.root");
  TFile* fIn = new TFile(filename);
  TTree* tIn = (TTree*) fIn->Get("limit");
  TH2F* hIn = new TH2F("corr","corr", 100, 0.5, 1.5, 100, 0.5, 1.5);
  
  Float deltaNLL;
  float SF1;
  float SF2;
  
  tIn->SetBranchAddress("deltaNLL", &deltaNLL);
  tIn->SetBranchAddress(param1, &SF1);
  tIn->SetBranchAddress(param2, &SF2);
  
  // first entry is r = 1 -> skip
  double xmin = 0;
  double xmax = 0;
  for (uint i = 1; i < tIn->GetEntries(); ++i)
    {
      tIn->GetEntry(i);
      hIn->Fill(SF1, SF2, deltaNLL);
    }
  
  
  TCanvas* c1 = new TCanvas("c1", "c1", 600, 600);
  hIn->Draw("colz");
  
}
