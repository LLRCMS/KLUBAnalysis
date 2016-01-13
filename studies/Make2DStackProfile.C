/*
void
set_plot_style()
{
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
}
*/

void
set_plot_style()
{
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;
    //Double_t stops[9] = { 0.0000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000};
    Double_t stops[9] = { 0.0000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000};
    for (int i = 0; i < 7; i++) stops[i] *= stops[i]/0.75;
      Double_t red[9]   = { 255./255., 234./255., 237./255., 230./255., 212./255., 156./255., 99./255., 45./255., 0./255.};
      Double_t green[9] = { 255./255., 238./255., 238./255., 168./255., 101./255.,  45./255.,  0./255.,  0./255., 0./255.};
      Double_t blue[9]  = { 255./255.,  95./255.,  11./255.,   8./255.,   9./255.,   3./255.,  1./255.,  1./255., 0./255.};
      TColor::CreateGradientColorTable(7, stops, red, green, blue, 255);
    gStyle->SetNumberContours(NCont);
}

void MakeStackProfile(bool doSig = false)
{

    TString selection = "defaultBtagLLNoIso";
    //TString selection = "defaultBtagLLNoIso";
    //TString selection = "baseline";
    //TString variable  = "bH_mass_tauH_mass";
    TString variable  = "bH_mass_tauH_SVFIT_mass";

    //TFile* fIn = new TFile ("/Users/Luca/Desktop/Radion300/mutau/evalQCD_Plots_miniAODv2_talk_2dtest/canvases2D/2Dplots.root"); // mu tau
    //TFile* fIn = new TFile ("/Users/Luca/Desktop/Radion300/mutau/evalQCD_Plots_miniAODv2_Plots_25_11_MassCut_talk27/canvases2D/2Dplots.root"); // tau tau
    //TFile* fIn = new TFile ("/Users/Luca/Desktop/Radion300/tautau/evalQCD_Plots_miniAODv2_Plots_25_11_MassCut_talk27/canvases2D/2Dplots.root"); // tau tau
   TFile* fIn = TFile::Open ("../analysis_MuTau/evalQCD_Plots_08Dec_FullStat/canvases2D/2Dplots.root");
   THStack* sSig  = (THStack*) fIn->Get(Form("OS_sig_%s_OS_%s", variable.Data(), selection.Data()));
    THStack* sBkg  = (THStack*) fIn->Get(Form("OS_bkg_%s_OS_%s", variable.Data(), selection.Data()));
    THStack* sDATA = (THStack*) fIn->Get(Form("OS_DATA_%s_OS_%s", variable.Data(), selection.Data()));
    cout<<Form("OS_bkg_%s_OS_%s", variable.Data(), selection.Data())<<endl;
    TH2F* hSig  = (TH2F*) sSig  ->GetStack()->Last();
    TH2F* hBkg  = (TH2F*) sBkg  ->GetStack()->Last();
    TH2F* hDATA = (TH2F*) sDATA ->GetStack()->Last();

    hBkg->SetMinimum(-0.0000001);
    hSig->SetMinimum(-0.0000001);
    hDATA->SetMarkerSize(0.4);
    hDATA->SetMarkerStyle(4);
    //hDATA->SetLineColor(kGray+3);
    //hDATA->SetMarkerColor(kGray+2);
    hDATA->SetLineWidth(1);

    cout << "bkg yield: " << hBkg->Integral() << endl;
    cout << "data yield: " << hDATA->Integral() << endl;
    cout << "sig yield: " << hSig->Integral() << endl;
    TString name = "c1";
    if(doSig)name.Append("sig");
    TCanvas* c1 = new TCanvas (name.Data(),name.Data(), 600, 600);
    gStyle->SetOptStat(0);
    
    //gStyle->SetPalette(56); // 53: scuro --> chiaro ; 56 : chiaro --> scuro
    //hBkg->SetContour(1000);
    set_plot_style();

    if (!doSig)
    {
        hBkg->Draw("COLZ");
        hBkg->GetXaxis()->SetRangeUser(0,300);
        hBkg->GetYaxis()->SetRangeUser(0,300);
        hBkg->GetXaxis()->SetTitle ("HH Kin  mass [GeV]");
        hBkg->GetYaxis()->SetTitle ("BDT");
        hBkg->GetYaxis()->SetTitleOffset(1.4);
        hBkg->SetTitle("Background distribution");
        hBkg->Draw("COLZ");
    }
    
    else
    {
        hSig->Draw("COLZ");
        hSig->GetXaxis()->SetRangeUser(0,300);
        hSig->GetYaxis()->SetRangeUser(0,300);
        hSig->GetXaxis()->SetTitle ("HH Kin mass [GeV]");
        hSig->GetYaxis()->SetTitle ("BDT");
        hSig->GetYaxis()->SetTitleOffset(1.4);
        hSig->SetTitle("Signal H(300) distribution");
        hSig->Draw("COLZ");
    }    
    //hDATA->Scale(0.1);
    //hDATA->Draw("scat");
    //hSig->Draw("COLZ");
    hDATA->Draw("scat=1.0 same");

    //c1->SaveAs ("muTau_bkg.pdf", "pdf");
    if (doSig) c1->SaveAs ("muTau_sig.pdf", "pdf");
    else c1->SaveAs ("muTau_bkg.pdf", "pdf");
}

void Make2DStackProfile(){
    MakeStackProfile(false);
    MakeStackProfile(true);

}

