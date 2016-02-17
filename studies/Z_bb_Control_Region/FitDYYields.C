using namespace RooFit ;

void FitDYYields()
{
    gStyle->SetOptStat(0);

    // bool doRatioPlot = true;

    // prepare histos
    // bin 1 --> 0 b tag
    // bin 2 --> 1 b tag
    // bin 3 --> 2 b tag
    TH1F* h_DY0b = new TH1F ("h_DY0b", "h_DY0b", 3, 0, 3);
    TH1F* h_DY1b = new TH1F ("h_DY1b", "h_DY1b", 3, 0, 3);
    TH1F* h_DY2b = new TH1F ("h_DY2b", "h_DY2b", 3, 0, 3);
    TH1F* h_bkg = new TH1F ("h_bkg", "h_bkg", 3, 0, 3);

    TH1F* h_ObsDATA = new TH1F ("h_ObsDATA", "h_ObsDATA", 3, 0, 3);

    // NUMBERS FOR MM tag (resonant analysis)
    // // set here bin content
    // h_DY0b->SetBinContent(1, 4520.38363647);
    // h_DY0b->SetBinContent(2, 207.534370422);
    // h_DY0b->SetBinContent(3, 1.95717564225);

    // h_DY1b->SetBinContent(1, 138.183671951);
    // h_DY1b->SetBinContent(2, 258.580339432);
    // h_DY1b->SetBinContent(3, 3.67949934117);

    // h_DY2b->SetBinContent(1, 9.77861857414);
    // h_DY2b->SetBinContent(2, 40.6705380678);
    // h_DY2b->SetBinContent(3, 36.9535586834);

    // h_bkg->SetBinContent(1, 93.4317743303);
    // h_bkg->SetBinContent(2, 25.7837350979);
    // h_bkg->SetBinContent(3, 14.5152006105);

    // h_ObsDATA->SetBinContent(1, 5456);
    // h_ObsDATA->SetBinContent(2, 504);
    // h_ObsDATA->SetBinContent(3, 73);

    // NUMBERS FOR LL tag (non-resonant analysis)
    // set here bin content
    h_DY0b->SetBinContent(1, 3757.44448853);
    h_DY0b->SetBinContent(2, 970.256820679);
    h_DY0b->SetBinContent(3, 108.373338699);

    h_DY1b->SetBinContent(1, 53.5524144173);
    h_DY1b->SetBinContent(2, 314.9933815);
    h_DY1b->SetBinContent(3, 31.8977227211);

    h_DY2b->SetBinContent(1, 1.57392059825);
    h_DY2b->SetBinContent(2, 27.8603755236);
    h_DY2b->SetBinContent(3, 57.9684224129);

    h_bkg->SetBinContent(1, 69.184968248);
    h_bkg->SetBinContent(2, 39.0817103345);
    h_bkg->SetBinContent(3, 25.4637945539);

    h_ObsDATA->SetBinContent(1, 4423);
    h_ObsDATA->SetBinContent(2, 1372);
    h_ObsDATA->SetBinContent(3, 238.0);




    // stima le componenti (pre-fit)
    float tot = h_DY0b->Integral() + h_DY1b->Integral() + h_DY2b->Integral() ;
    // float f_frac_DY0b = h_DY0b->Integral() / tot;
    // float f_frac_DY1b = h_DY1b->Integral() / tot;
    // float f_frac_DY2b = h_DY2b->Integral() / tot;
    float f_frac_DY0b = h_DY0b->Integral() ;
    float f_frac_DY1b = h_DY1b->Integral() ;
    float f_frac_DY2b = h_DY2b->Integral() ;
    float f_frac_bkg = h_bkg->Integral() ;



    //subtract data residul bkg from data
    // for (int i = 1; i < 3; i++)
    // {
    //     float bc = h_ObsDATA->GetBinContent(i);
    //     float bkgct = h_bkg->GetBinContent(i);
    //     h_ObsDATA->SetBinContent (i, bc - bkgct);
    //     if (bc - bkgct < 0) cout << "Warning: when subracting other bkg from data, neagtive! ndata = " << bc << " nbkg = " << bkgct << " in bin " << i << endl;
    // }
    // observable: is the bin numer (events in each bin)
    RooRealVar x("x", "x", 0, 3) ;

    RooDataHist dh ("dh", "dh", x, Import(*h_ObsDATA)) ;
    RooPlot* frame = x.frame(Title("Observed data yields")) ;
    dh.plotOn(frame) ;

    // pdfs -- binned
    // temporaty conversion to dataHist
    RooDataHist datahist_DY0b ("datahist_DY0b", "datahist_DY0b", x, Import(*h_DY0b), 0) ;
    RooDataHist datahist_DY1b ("datahist_DY1b", "datahist_DY1b", x, Import(*h_DY1b), 0) ;
    RooDataHist datahist_DY2b ("datahist_DY2b", "datahist_DY2b", x, Import(*h_DY2b), 0) ;
    RooDataHist datahist_bkg ("datahist_bkg", "datahist_bkg", x, Import(*h_bkg), 0) ;
    
    // datahist_DY0b.plotOn(frame, LineColor(kRed));
    // datahist_DY1b.plotOn(frame, LineColor(kGreen));
    // datahist_DY2b.plotOn(frame, LineColor(kCyan));

    // binnedPDF
    RooHistPdf pdf_DY0b ("pdf_DY0b", "pdf_DY0b", x, datahist_DY0b, 0) ;
    RooHistPdf pdf_DY1b ("pdf_DY1b", "pdf_DY1b", x, datahist_DY1b, 0) ;
    RooHistPdf pdf_DY2b ("pdf_DY2b", "pdf_DY2b", x, datahist_DY2b, 0) ;
    RooHistPdf pdf_bkg ("pdf_bkg", "pdf_bkg", x, datahist_bkg, 0) ;

    // pdf_DY0b.plotOn(frame, LineColor(kRed));
    // pdf_DY1b.plotOn(frame, LineColor(kGreen));
    // pdf_DY2b.plotOn(frame, LineColor(kCyan));


    // total PDF
    // [0.5 - 2] as SF range
    RooRealVar frac_DY0b ("frac_DY0b","fraction of DY 0b", f_frac_DY0b, 0.5*f_frac_DY0b, 3.*f_frac_DY0b) ; // value, minvalue, maxvalue
    RooRealVar frac_DY1b ("frac_DY1b","fraction of DY 1b", f_frac_DY1b, 0.5*f_frac_DY1b, 3.*f_frac_DY1b) ; // value, minvalue, maxvalue
    RooRealVar frac_DY2b ("frac_DY2b","fraction of DY 2b", f_frac_DY2b, 0.5*f_frac_DY2b, 3.*f_frac_DY2b) ; // value, minvalue, maxvalue
    RooRealVar frac_bkg ("frac_bkg","fraction of DY 2b", f_frac_bkg) ; // value, minvalue, maxvalue
    frac_bkg.setConstant();
    // RooRealVar frac_DY0b ("frac_DY0b","fraction of DY 0b", 60,  0, 120.) ; // value, minvalue, maxvalue
    // RooRealVar frac_DY1b ("frac_DY1b","fraction of DY 1b", 130, 0, 260.) ; // value, minvalue, maxvalue
    // RooRealVar frac_DY2b ("frac_DY2b","fraction of DY 2b", 210, 0, 420.) ; // value, minvalue, maxvalue


    RooAddPdf model("model","3 component model", RooArgList(pdf_DY0b, pdf_DY1b, pdf_DY2b, pdf_bkg), RooArgList(frac_DY0b, frac_DY1b, frac_DY2b, frac_bkg)) ;

    RooFitResult* fitRes = model.fitTo(dh, Save()) ;
    model.plotOn(frame) ;

    fitRes->Print();

    cout << "SFs" << endl;
    cout << "DY 0B :: " << frac_DY0b.getVal()/f_frac_DY0b << " +/- " << frac_DY0b.getError()/f_frac_DY0b << endl;
    cout << "DY 1B :: " << frac_DY1b.getVal()/f_frac_DY1b << " +/- " << frac_DY1b.getError()/f_frac_DY1b << endl;
    cout << "DY 2B :: " << frac_DY2b.getVal()/f_frac_DY2b << " +/- " << frac_DY2b.getError()/f_frac_DY2b << endl;
    cout << "bkg   :: " << frac_bkg.getVal()/f_frac_bkg << " +/- " << frac_bkg.getError()/f_frac_bkg << endl;

    //cout << "DY 0b " << frac_DY0b.getVal() << " + " << frac_DY0b.getAsymErrorHi() << " - " << frac_DY0b.getAsymErrorLo() << " i.e. " << frac_DY0b.getError() << endl;
    // frame->Draw();

    ////////// MAKE STACK PLOTS ///////////////
    THStack* st_prefit = new THStack ("st_prefit", "st_prefit");
    h_bkg->SetFillColor(kAzure+2);
    h_DY0b->SetFillColor(90);
    h_DY1b->SetFillColor(92);
    h_DY2b->SetFillColor(94);
    h_bkg->SetLineColor(kAzure+2);
    h_DY0b->SetLineColor(90);
    h_DY1b->SetLineColor(92);
    h_DY2b->SetLineColor(94);

    h_ObsDATA->SetLineColor(kBlack);
    h_ObsDATA->SetMarkerColor(kBlack);
    h_ObsDATA->SetMarkerStyle(8);
    h_ObsDATA->SetMarkerSize(0.9);
    
    st_prefit->Add (h_bkg);
    st_prefit->Add (h_DY0b);
    st_prefit->Add (h_DY1b);
    st_prefit->Add (h_DY2b);

    TLegend* leg = new TLegend (0.7, 0.6, 0.89, 0.89);
    leg->AddEntry (h_DY2b, "DY + 2b", "f");
    leg->AddEntry (h_DY1b, "DY + 1b1j", "f");
    leg->AddEntry (h_DY0b, "DY + 2j", "f");
    leg->AddEntry (h_bkg,  "other bkg.", "f");
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);

    TCanvas* c2 = new TCanvas ("c2", "c2", 600, 600);
    pad1 = new TPad("pad1", "pad1", 0, 0.25, 1, 1.0);    
    pad1->SetBottomMargin(0.02);
    pad1->Draw();
    pad1->cd();

    pad1->SetLogy();
     // c2->SetFrameLineWidth(3);
    st_prefit->Draw();
    st_prefit->SetMinimum(10);
    st_prefit->SetTitle (";;events");
    // st_prefit->GetXaxis()->SetLabelSize(0.08);
    st_prefit->GetXaxis()->SetLabelSize(0.0);
    st_prefit->GetYaxis()->SetLabelSize(0.05);
    st_prefit->GetYaxis()->SetTitleSize(0.05);
    st_prefit->GetXaxis()->SetBinLabel (1,"0 b-tag");
    st_prefit->GetXaxis()->SetBinLabel (2,"1 b-tag");
    st_prefit->GetXaxis()->SetBinLabel (3,"2 b-tag");
    st_prefit->Draw();
    h_ObsDATA->Sumw2();
    h_ObsDATA->Draw("p same");
    leg->Draw();

    c2->cd();
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.2496);
    pad2->SetTopMargin(0.05);
    pad2->SetBottomMargin(0.35);
    // pad2->SetRightMargin(0.035);
    pad2->SetGridy();
    //pad2->SetGridx(); // vertical grid
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad

    TH1F* hRatio_prefit = (TH1F*) h_ObsDATA->Clone ("hRatio_prefit");
    hRatio_prefit->SetTitle (";;Data/MC");
    hRatio_prefit->GetXaxis()->SetBinLabel (1,"0 b-tag");
    hRatio_prefit->GetXaxis()->SetBinLabel (2,"1 b-tag");
    hRatio_prefit->GetXaxis()->SetBinLabel (3,"2 b-tag");
    hRatio_prefit->Divide ((TH1F*) st_prefit->GetStack()->Last());
    hRatio_prefit->SetMarkerStyle(8);
    hRatio_prefit->SetMarkerSize(0.9);
    hRatio_prefit->GetYaxis()->SetNdivisions(505);
    hRatio_prefit->GetYaxis()->SetLabelSize(0.15);
    hRatio_prefit->GetXaxis()->SetLabelSize(0.3);
    hRatio_prefit->GetXaxis()->SetLabelOffset(0.05);
    hRatio_prefit->GetYaxis()->SetTitleOffset(0.3);
    hRatio_prefit->GetYaxis()->SetTitleSize(0.15);
    hRatio_prefit->SetFillColor(kRed-4);
    
    hRatio_prefit_data = (TH1F*) hRatio_prefit->Clone ("hRatio_prefit_data");
    
    // set uncertainties in correct way
    for (int i = 1; i <= 3; i++)
    {
        float nData = h_ObsDATA->GetBinContent (i);
        float nBkg = ((TH1F*) st_prefit->GetStack()->Last() ) -> GetBinContent(i);
        float err = TMath::Sqrt(nData) / nBkg;
        hRatio_prefit_data->SetBinError(i, err);
    }

    // set MC uncertainties in correct way
    // const float errsMC [3] = {0.00807848721005, 0.0316666738269, 0.0612605580677}; //for MM (resonant)
    const float errsMC [3] = {0.00866633166927, 0.01849629617, 0.0407631104079}; //for LL (non-resonant)
    for (int i = 1; i <= 3; i++)
    {
        hRatio_prefit->SetBinContent(i,1);
        hRatio_prefit->SetBinError(i, errsMC[i-1]);
    }

    hRatio_prefit->SetMinimum(0.8);
    // hRatio_prefit->SetMaximum(1.59); // MM
    hRatio_prefit->SetMaximum(1.39); // LL
    hRatio_prefit->SetMarkerSize(0);
    hRatio_prefit->Draw("e2");
    hRatio_prefit_data->Draw("p e same");
    pad2->RedrawAxis();
    pad2->RedrawAxis("g");

    c2->Print ("SF_prefit.pdf", "pdf");

    // /////////// clone to do postfit plot //////////////////////////
    TH1F* h_bkg_fit  = (TH1F*) h_bkg  -> Clone("h_bkg_fit"); 
    TH1F* h_DY0b_fit = (TH1F*) h_DY0b -> Clone("h_DY0b_fit");
    TH1F* h_DY1b_fit = (TH1F*) h_DY1b -> Clone("h_DY1b_fit");
    TH1F* h_DY2b_fit = (TH1F*) h_DY2b -> Clone("h_DY2b_fit");
    
    h_DY0b_fit -> Scale (frac_DY0b.getVal()/f_frac_DY0b);
    h_DY1b_fit -> Scale (frac_DY1b.getVal()/f_frac_DY1b);
    h_DY2b_fit -> Scale (frac_DY2b.getVal()/f_frac_DY2b);

    THStack* st_postfit = new THStack ("st_postfit", "st_postfit");

    TCanvas* c3 = new TCanvas ("c3", "c3", 600, 600);
    c3->cd();
    pad3 = new TPad("pad3", "pad3", 0, 0.25, 1, 1.0);    
    pad3->SetBottomMargin(0.02);
    pad3->SetLogy();
    pad3->Draw();
    pad3->cd();
    
    // c3->SetLogy();
    // c3->SetFrameLineWidth(3);

    st_postfit->Add (h_bkg_fit);
    st_postfit->Add (h_DY0b_fit);
    st_postfit->Add (h_DY1b_fit);
    st_postfit->Add (h_DY2b_fit);

    st_postfit->Draw();
    st_postfit->SetMinimum(10);
    st_postfit->SetTitle (";;events");
    st_postfit->GetXaxis()->SetLabelSize(0.08);
    st_postfit->GetYaxis()->SetLabelSize(0.05);
    st_postfit->GetYaxis()->SetTitleSize(0.05);
    st_postfit->GetXaxis()->SetBinLabel (1,"0 b-tag");
    st_postfit->GetXaxis()->SetBinLabel (2,"1 b-tag");
    st_postfit->GetXaxis()->SetBinLabel (3,"2 b-tag");
    st_postfit->GetXaxis()->SetLabelSize(0);
    st_postfit->Draw();
    h_ObsDATA->Sumw2();
    h_ObsDATA->Draw("p same");
    leg->Draw();


    c3->cd();
    TPad *pad4 = new TPad("pad4", "pad4", 0, 0.0, 1, 0.2496);
    pad4->SetTopMargin(0.05);
    pad4->SetBottomMargin(0.35);
    // pad4->SetRightMargin(0.035);
    pad4->SetGridy();
    //pad4->SetGridx(); // vertical grid
    pad4->Draw();
    pad4->cd();       // pad2 becomes the current pad

    TH1F* hRatio_postfit = (TH1F*) h_ObsDATA->Clone ("hRatio_postfit");
    hRatio_postfit->SetTitle (";;Data/MC");
    hRatio_postfit->GetXaxis()->SetBinLabel (1,"0 b-tag");
    hRatio_postfit->GetXaxis()->SetBinLabel (2,"1 b-tag");
    hRatio_postfit->GetXaxis()->SetBinLabel (3,"2 b-tag");
    hRatio_postfit->Divide ((TH1F*) st_postfit->GetStack()->Last());
    hRatio_postfit->SetMarkerStyle(8);
    hRatio_postfit->SetMarkerSize(0.9);
    hRatio_postfit->GetYaxis()->SetNdivisions(505);
    hRatio_postfit->GetYaxis()->SetLabelSize(0.15);
    hRatio_postfit->GetXaxis()->SetLabelSize(0.3);
    hRatio_postfit->GetXaxis()->SetLabelOffset(0.05);
    hRatio_postfit->GetYaxis()->SetTitleOffset(0.3);
    hRatio_postfit->GetYaxis()->SetTitleSize(0.15);
    hRatio_postfit->SetFillColor(kRed-4);
    
    hRatio_postfit_data = (TH1F*) hRatio_postfit->Clone ("hRatio_postfit_data");
    
    // set uncertainties in correct way
    for (int i = 1; i <= 3; i++)
    {
        float nData = h_ObsDATA->GetBinContent (i);
        float nBkg = ((TH1F*) st_postfit->GetStack()->Last() ) -> GetBinContent(i);
        float err = TMath::Sqrt(nData) / nBkg;
        hRatio_postfit_data->SetBinError(i, err);
    }

    // set MC uncertainties in correct way
    // float errsMC [3] = {0.00807848721005, 0.0316666738269, 0.0612605580677};
    for (int i = 1; i <= 3; i++)
    {
        hRatio_postfit->SetBinContent(i,1);
        hRatio_postfit->SetBinError(i, errsMC[i-1]);
    }

    hRatio_postfit->SetMinimum(0.8);
    // hRatio_postfit->SetMaximum(1.59); // MM
    hRatio_postfit->SetMaximum(1.39); // LL
    hRatio_postfit->SetMarkerSize(0);
    hRatio_postfit->Draw("e2");
    hRatio_postfit_data->Draw("p e same");
    pad3->RedrawAxis();
    pad3->RedrawAxis("g");

    c3->Print ("SF_postfit.pdf", "pdf");




}