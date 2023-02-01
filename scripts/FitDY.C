// I wanted to use python, but RooFit in python is simply too much...

using namespace RooFit ;
using namespace std;

string makeHistoName(string name, string sel, string var)
{
    string res = name;
    res += "_";
    res += sel;
    res += "_";
    res += var;
    return res;
}

std::vector<TH1F*> retrieveHistos(TFile* rootFile, std::vector<string>& vNames, string sel, string var)
{
    std::vector<TH1F*> res;   
    for (string n : vNames)
    {
        string name = makeHistoName(n, sel, var);
        TH1F* h = (TH1F*) rootFile->Get(name.c_str());
        res.push_back(h);
        cout << "@@ " << name << " " << sel << " " << var << " -- " << h << endl; 
    }
    cout << endl;
    return res;
}

TH1F* mergeTogether (const std::vector<TH1F*>& vList, string newname)
{
    TH1F* sum;
    for (int idx = 0; idx < vList.size(); ++idx)
    {
        if (idx == 0)
            sum = (TH1F*) vList.at(0)->Clone(newname.c_str());
        else
            sum->Add(vList.at(idx));
    }
    return sum;
}


std::vector<RooDataHist*> makeDataHist(const std::vector<TH1F*>& vList, const std::vector<string>& vNames, string tag, RooRealVar& xvar, bool initW = false)
{
    std::vector<RooDataHist*> res;   
    for (uint idx = 0; idx < vList.size(); ++idx)
    {
        string nn = vNames.at(idx);
        nn += "_";
        nn += tag;
        RooDataHist* dh = (initW ? new RooDataHist (nn.c_str(), nn.c_str(), xvar, Import(*(vList.at(idx))), 0) : new RooDataHist (nn.c_str(), nn.c_str(), xvar, Import(*(vList.at(idx))))) ;
        res.push_back(dh);
    }
    return res;
}

// pdf_DY0b ("pdf_DY0b", "pdf_DY0b", x, datahist_DY0b, 0) ;
std::vector<RooHistPdf*> makeRooHistPdfs(const std::vector<RooDataHist*>& vList, const std::vector<string>& vNames, string tag, RooRealVar& xvar)
{
    std::vector<RooHistPdf*> res;   
    for (uint idx = 0; idx < vList.size(); ++idx)
    {
        string nn = vNames.at(idx);
        nn += "histpdf_";
        nn += tag;
        RooHistPdf* dh = new RooHistPdf (nn.c_str(), nn.c_str(), xvar, *(vList.at(idx)), 0) ;
        res.push_back(dh);
    }
    return res;
}

// RooRealVar var_DY0b ("var_DY0b","fraction of DY 0b", f_frac_DY0b, 0*f_frac_DY0b, 3.*f_frac_DY0b) ; // value, minvalue, maxvalue
std::vector<RooRealVar*> makeRooRealVars(const std::vector<string>& vNames, string tag, const std::vector<TH1F*> histos)
{
    std::vector<RooRealVar*> res;
    for (uint idx = 0; idx < vNames.size(); ++idx)
    {
        float yy = histos.at(idx)->Integral();
        float vstart = yy;
        float vmin = 0.;
        float vmax = 3.*yy;
        string nn = vNames.at(idx);
        nn += "RV_";
        nn += tag;
        RooRealVar* rv = new RooRealVar(nn.c_str(), nn.c_str(), vstart, vmin, vmax);
        res.push_back(rv);
    }
    return res;
}

// RooFormulaVar mean("mean","mean","sqrt(mean2)",mean2) ;
std::vector<RooFormulaVar*> makeRooFormulaVars(const std::vector<string>& vNames, string tag, const std::vector<TH1F*> histos, const RooRealVar& scale)
{
    std::vector<RooFormulaVar*> res;
    for (uint idx = 0; idx < vNames.size(); ++idx)
    {
        float yy = histos.at(idx)->Integral();
        // float vstart = yy;
        // float vmin = 0.;
        // float vmax = 3.*yy;
        string nn = vNames.at(idx);
        nn += "FV_";
        nn += tag;
        TString formula = Form("%f*%s", yy, scale.GetName());
        RooFormulaVar* fv = new RooFormulaVar(nn.c_str(), nn.c_str(), formula, scale);
        res.push_back(fv);
    }
    return res;
}

void rebinAllHistos (std::vector<TH1F*>& vhistos)
{
    int nbins = vhistos.at(0)->GetNbinsX();
    for (TH1F* h : vhistos)
    {
        h->Rebin(nbins);
    }
    return;
}

void printYields(
    const std::vector<TH1F*>& hcoll_DY0b, // 0 tag region, 1 tag region, 2 tag region
    const std::vector<TH1F*>& hcoll_DY1b,
    const std::vector<TH1F*>& hcoll_DY2b,
    const std::vector<TH1F*>& hcoll_bkgs,
    const std::vector<TH1F*>& hcoll_data
    )
{
    // DY0b, DY1b, DY2b, bkg, data
    vector<double> vals0b = {hcoll_DY0b.at(0)->Integral(), hcoll_DY1b.at(0)->Integral(), hcoll_DY2b.at(0)->Integral(), hcoll_bkgs.at(0)->Integral(), hcoll_data.at(0)->Integral()};
    vector<double> vals1b = {hcoll_DY0b.at(1)->Integral(), hcoll_DY1b.at(1)->Integral(), hcoll_DY2b.at(1)->Integral(), hcoll_bkgs.at(1)->Integral(), hcoll_data.at(1)->Integral()};
    vector<double> vals2b = {hcoll_DY0b.at(2)->Integral(), hcoll_DY1b.at(2)->Integral(), hcoll_DY2b.at(2)->Integral(), hcoll_bkgs.at(2)->Integral(), hcoll_data.at(2)->Integral()};

    cout << "h_DY0b->SetBinContent(1," << vals0b[0] << ");" << endl;
    cout << "h_DY0b->SetBinContent(2," << vals1b[0] << ");" << endl;
    cout << "h_DY0b->SetBinContent(3," << vals2b[0] << ");\n" << endl;
    cout << "h_DY1b->SetBinContent(1," << vals0b[1] << ");" << endl;
    cout << "h_DY1b->SetBinContent(2," << vals1b[1] << ");" << endl;
    cout << "h_DY1b->SetBinContent(3," << vals2b[1] << ");\n" << endl;
    cout << "h_DY2b->SetBinContent(1," << vals0b[2] << ");" << endl;
    cout << "h_DY2b->SetBinContent(2," << vals1b[2] << ");" << endl;
    cout << "h_DY2b->SetBinContent(3," << vals2b[2] << ");\n" << endl;
    cout << "h_bkg->SetBinContent(1," << vals0b[3] << ");" << endl;
    cout << "h_bkg->SetBinContent(2," << vals1b[3] << ");" << endl;
    cout << "h_bkg->SetBinContent(3," << vals2b[3] << ");\n" << endl;
    cout << "h_ObsDATA->SetBinContent(1," << vals0b[4] << ");" << endl;
    cout << "h_ObsDATA->SetBinContent(2," << vals1b[4] << ");" << endl;
    cout << "h_ObsDATA->SetBinContent(3," << vals2b[4] << ");\n" << endl;
    cout << "const float errsMC [3] = {" << vals0b[5] << "," << vals1b[5] << "," << vals2b[5] << "};\n"  << endl;
}

void FitDY(bool medTag = true)
{
    //TFile* inFile = new TFile ("../analysis_MuMu_3Oct2018_DYNLO/outPlotter.root") ;
    //TFile* inFile = new TFile ("../analysis_MuMu_15Feb2019_DYNLO_weights/outPlotter.root") ;
    //TFile* inFile = new TFile ("../analysis_MuMu_09July2019_ZplusJetsStudy/outPlotter.root") ;
    //TFile* inFile = new TFile ("../analysis_MuMu_09July2019_ZplusJetsStudy_LooseWP/outPlotter.root") ;
    //TFile* inFile = new TFile ("../analysis_MuMu_23July2019_splitDY_deepFlavor/outPlotter.root") ;
    TFile* inFile = new TFile ("../analysis_MuMu_23July2019_splitDY_deepFlavor_LooseWP/outPlotter.root") ;

    string sel0tag;
    string sel1tag;
    string sel2tag;
    string var = "tauH_mass";
    string varName = "m_{#mu#mu}";


    //0b2jhig17, 1b1jhig17, 2b0jhig17 -- 0b2jchia17,1b1jchia17,2b0jchia17 -- 0b2jPI 1b1jPI 2b0jPI
    if (medTag)
    {
        /*sel0tag = "0b2jMmetbcut_SR" ;
        sel1tag = "1b1jMmetbcut_SR" ;
        sel2tag = "2b0jMmetbcut_SR" ;*/
        sel0tag = "0b2jLmetbcut_SR" ;
        sel1tag = "1b1jLmetbcut_SR" ;
        sel2tag = "2b0jLmetbcut_SR" ;
    }
    else
    {
        sel0tag = "0b2jLmetbcut_SR" ;
        sel1tag = "1b1jLmetbcut_SR" ;
        sel2tag = "2b0jLmetbcut_SR" ;
    }

    /*    
    string sel0tag = "0b2jMmetbcut_SR" ;
    string sel1tag = "1b1jMmetbcut_SR" ;
    string sel2tag = "2b0jMmetbcut_SR" ;
    string var = "tauH_mass";
    string varName = "m_{#mu#mu}";

    // string sel0tag = "0b2jLmetbcut_SR" ;
    // string sel1tag = "1b1jLmetbcut_SR" ;
    // string sel2tag = "2b0jLmetbcut_SR" ;
    // string var = "tauH_mass";
    // string varName = "m_{#mu#mu}";

    // string sel0tag = "0b2jMtaubcut_SR" ;
    // string sel1tag = "1b1jMtaubcut_SR" ;
    // string sel2tag = "2b0jMtaubcut_SR" ;
    // string var = "met_et";
    // string varName = "E_{T}^{miss}"
    */

    // leave in the order 0b, 1b, 2b
    vector<string> vDY = {"DY_NLO_allgenjets_0b","DY_NLO_allgenjets_1b", "DY_NLO_allgenjets_2b"};
    //vector<string> vDY = {"DY0b", "DY1b", "DY2b"};
    // vector<string> vBkgs = {"TT", "WJets", "TWtop", "TWantitop", "WWToLNuQQ", "WZTo1L1Nu2Q", "WZTo1L3Nu", "WZTo2L2Q", "ZZTo2L2Q"};
    //vector<string> vBkgs = {"TT", "WJets" , "others"};
    vector<string> vBkgs = {"TT", "WJets"};
    vector<string> vData = {"data_obs"};
    // vector<string> vData = {"DsingleMu"};

    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////

    std::vector<string> names = {"0tag", "1tag", "2tag"};

    std::vector<TH1F*> hBkgs0tag = retrieveHistos(inFile, vBkgs, sel0tag, var);
    std::vector<TH1F*> hBkgs1tag = retrieveHistos(inFile, vBkgs, sel1tag, var);
    std::vector<TH1F*> hBkgs2tag = retrieveHistos(inFile, vBkgs, sel2tag, var);

    // histos fro 0ta, 1tag, and 2 tag sidebands
    std::vector<TH1F*> hDY0tag = retrieveHistos(inFile, vDY, sel0tag, var);
    std::vector<TH1F*> hDY1tag = retrieveHistos(inFile, vDY, sel1tag, var);
    std::vector<TH1F*> hDY2tag = retrieveHistos(inFile, vDY, sel2tag, var);

    TH1F* hData0tag = retrieveHistos(inFile, vData, sel0tag, var).at(0);
    TH1F* hData1tag = retrieveHistos(inFile, vData, sel1tag, var).at(0);
    TH1F* hData2tag = retrieveHistos(inFile, vData, sel2tag, var).at(0);
    
    // merge together the bkgs into a single template
    // for (auto x : hBkgs0tag)
    //     cout << x << " " << x->GetName() << " " << x->Integral() << endl;

    TH1F* hBkgSum0tag = mergeTogether(hBkgs0tag, "bkgSum0tag");
    TH1F* hBkgSum1tag = mergeTogether(hBkgs1tag, "bkgSum1tag");
    TH1F* hBkgSum2tag = mergeTogether(hBkgs2tag, "bkgSum2tag");

    // put everything into the same structure : a vector [0tag, 1tag, 2tag]
    std::vector<TH1F*> hcoll_DY0b = {hDY0tag.at(0), hDY1tag.at(0), hDY2tag.at(0)};
    std::vector<TH1F*> hcoll_DY1b = {hDY0tag.at(1), hDY1tag.at(1), hDY2tag.at(1)};
    std::vector<TH1F*> hcoll_DY2b = {hDY0tag.at(2), hDY1tag.at(2), hDY2tag.at(2)};
    std::vector<TH1F*> hcoll_bkgs = {hBkgSum0tag, hBkgSum1tag, hBkgSum2tag};
    std::vector<TH1F*> hcoll_data = {hData0tag, hData1tag, hData2tag};

    // if I want to go back to a siple counting: rebin everything into a single big bin
    // rebinAllHistos(hcoll_DY0b);
    // rebinAllHistos(hcoll_DY1b);
    // rebinAllHistos(hcoll_DY2b);
    // rebinAllHistos(hcoll_bkgs);
    // rebinAllHistos(hcoll_data);

    printYields(hcoll_DY0b, hcoll_DY1b, hcoll_DY2b, hcoll_bkgs, hcoll_data);

    // make datasets
    RooRealVar xvar("xvar", varName.c_str(), hData0tag->GetXaxis()->GetXmin(), hData0tag->GetXaxis()->GetXmax()) ;

    std::vector<RooDataHist*> vDatahist_data = makeDataHist(hcoll_data, names, "data", xvar);
    std::vector<RooDataHist*> vDatahist_DY0b = makeDataHist(hcoll_DY0b, names, "DY0b", xvar, true);
    std::vector<RooDataHist*> vDatahist_DY1b = makeDataHist(hcoll_DY1b, names, "DY1b", xvar, true);
    std::vector<RooDataHist*> vDatahist_DY2b = makeDataHist(hcoll_DY2b, names, "DY2b", xvar, true);
    std::vector<RooDataHist*> vDatahist_bkgs = makeDataHist(hcoll_bkgs, names, "bkgs", xvar, true);

    // make hist pdfs
    std::vector<RooHistPdf*> vHistPdf_DY0b = makeRooHistPdfs(vDatahist_DY0b, names, "DY0b", xvar);
    std::vector<RooHistPdf*> vHistPdf_DY1b = makeRooHistPdfs(vDatahist_DY1b, names, "DY1b", xvar);
    std::vector<RooHistPdf*> vHistPdf_DY2b = makeRooHistPdfs(vDatahist_DY2b, names, "DY2b", xvar);
    std::vector<RooHistPdf*> vHistPdf_bkgs = makeRooHistPdfs(vDatahist_bkgs, names, "bkgs", xvar);


    // make the categorization of the 3 regions 
    RooCategory region("region","region") ;
    region.defineType("0tag") ;
    region.defineType("1tag") ;
    region.defineType("2tag") ;
    RooDataHist combData("combData","combined data",xvar,
        Index(region),
        Import("0tag",*(vDatahist_data.at(0))),
        Import("1tag",*(vDatahist_data.at(1))),
        Import("2tag",*(vDatahist_data.at(2)))
    ) ;

    // the partial yields
    // makeRooRealVars(const std::vector<string>& vNames, string tag, const std::vector<TH1F*> histos)
    // RooRealVar var_DY0b ("var_DY0b","fraction of DY 0b", f_frac_DY0b, 0*f_frac_DY0b, 3.*f_frac_DY0b) ; // value, minvalue, maxvalue
    RooRealVar scale_DY0b ("scale_DY0b", "scale_DY0b", 1., 0., 3.); 
    RooRealVar scale_DY1b ("scale_DY1b", "scale_DY1b", 1., 0., 3.); 
    RooRealVar scale_DY2b ("scale_DY2b", "scale_DY2b", 1., 0., 3.); 
    RooRealVar scale_bkgs ("scale_bkgs", "scale_bkgs", 1., 0.5, 1.5);         
    // scale_bkgs.setConstant();

    // std::vector<RooRealVar*> frac_DY0b = makeRooRealVars(names, "DY0b", hcoll_DY0b);
    // std::vector<RooRealVar*> frac_DY1b = makeRooRealVars(names, "DY1b", hcoll_DY1b);
    // std::vector<RooRealVar*> frac_DY2b = makeRooRealVars(names, "DY2b", hcoll_DY2b);
    // std::vector<RooRealVar*> frac_bkgs = makeRooRealVars(names, "bkgs", hcoll_bkgs);
    std::vector<RooFormulaVar*> frac_DY0b = makeRooFormulaVars(names, "DY0b", hcoll_DY0b, scale_DY0b);
    std::vector<RooFormulaVar*> frac_DY1b = makeRooFormulaVars(names, "DY1b", hcoll_DY1b, scale_DY1b);
    std::vector<RooFormulaVar*> frac_DY2b = makeRooFormulaVars(names, "DY2b", hcoll_DY2b, scale_DY2b);
    std::vector<RooFormulaVar*> frac_bkgs = makeRooFormulaVars(names, "bkgs", hcoll_bkgs, scale_bkgs);


    // Construct composite pdf
    RooAddPdf model0tag("model0tag","3 component model - 0 tag",
        RooArgList(*(vHistPdf_DY0b.at(0)), *(vHistPdf_DY1b.at(0)), *(vHistPdf_DY2b.at(0)), *(vHistPdf_bkgs.at(0))),
        RooArgList(*(frac_DY0b.at(0)), *(frac_DY1b.at(0)), *(frac_DY2b.at(0)), *(frac_bkgs.at(0)))
        ) ;

    RooAddPdf model1tag("model1tag","3 component model - 1 tag",
        RooArgList(*(vHistPdf_DY0b.at(1)), *(vHistPdf_DY1b.at(1)), *(vHistPdf_DY2b.at(1)), *(vHistPdf_bkgs.at(1))),
        RooArgList(*(frac_DY0b.at(1)), *(frac_DY1b.at(1)), *(frac_DY2b.at(1)), *(frac_bkgs.at(1)))
        ) ;

    RooAddPdf model2tag("model2tag","3 component model - 2 tag",
        RooArgList(*(vHistPdf_DY0b.at(2)), *(vHistPdf_DY1b.at(2)), *(vHistPdf_DY2b.at(2)), *(vHistPdf_bkgs.at(2))),
        RooArgList(*(frac_DY0b.at(2)), *(frac_DY1b.at(2)), *(frac_DY2b.at(2)), *(frac_bkgs.at(2)))
        ) ;

    model0tag.pdfList().Print();
    model1tag.pdfList().Print();
    model2tag.pdfList().Print();

    // make the composito pdf
    // Associate model with the physics state and model_ctl with the control state
    RooSimultaneous simPdf("simPdf","simultaneous pdf",region) ;
    simPdf.addPdf(model0tag,"0tag") ;
    simPdf.addPdf(model1tag,"1tag") ;
    simPdf.addPdf(model2tag,"2tag") ;

    RooFitResult* rfit = simPdf.fitTo(combData, Save()) ;
    // simPdf.fitTo(combData, Extended(true)) ;

    TCanvas* c1 = new TCanvas("c1", "c1", 1200, 400);
    // c1->SetLogy();
    c1->Divide(3, 1) ;

    c1->cd(1);
    // gPad->SetLogy(true);
    RooPlot* frame0 = xvar.frame(Title("0 tag region")) ;
    frame0->SetYTitle("Events");
    frame0->GetYaxis()->SetTitleOffset(1.5);
    vDatahist_data.at(0)->plotOn(frame0);
    model0tag.plotOn(frame0);
    model0tag.plotOn(frame0,Slice(region,"0tag"),Components("0taghistpdf_bkgs"),ProjWData(region,combData),LineStyle(kDashed)) ;
    // model0tag.plotOn(frame0,Slice(region,"0tag"),Components("0taghistpdf_bkgs"),ProjWData(region,combData),FillColor(kGray),FillStyle(3013)) ;
    // model0tag.plotOn(frame0,Slice(region,"0tag"),Components("0taghistpdf_DY0b"),ProjWData(region,combData),FillColor(kRed), LineColor(kRed), RooFit::DrawOption("F")) ;
    frame0->Draw();

    c1->cd(2);    
    // gPad->SetLogy(true);
    RooPlot* frame1 = xvar.frame(Title("1 tag region")) ;
    frame1->SetYTitle("Events");
    frame1->GetYaxis()->SetTitleOffset(1.5);
    vDatahist_data.at(1)->plotOn(frame1);
    model1tag.plotOn(frame1);
    model1tag.plotOn(frame1,Slice(region,"1tag"),Components("1taghistpdf_bkgs"),ProjWData(region,combData),LineStyle(kDashed)) ;
    frame1->Draw();

    c1->cd(3);    
    // gPad->SetLogy(true);
    RooPlot* frame2 = xvar.frame(Title("2 tag region")) ;
    frame2->SetYTitle("Events");
    frame2->GetYaxis()->SetTitleOffset(1.5);
    vDatahist_data.at(2)->plotOn(frame2);
    model2tag.plotOn(frame2);
    model2tag.plotOn(frame2,Slice(region,"2tag"),Components("2taghistpdf_bkgs"),ProjWData(region,combData),LineStyle(kDashed)) ;
    frame2->Draw();
    
    c1->Print("SF_DY_fit.pdf", "pdf");


    cout << "... printing covariance matrix" << endl;
    const TMatrixDSym& cov = rfit->covarianceMatrix() ;
    cov.Print();

    cout << "... printing correlation matrix" << endl;
    const TMatrixDSym& cor = rfit->correlationMatrix() ;
    cor.Print();


    string WPnametext = (medTag ? "Medium WP" : "Loose WP");
    cout << "***** The derived scale factors for the " << WPnametext << " are *****" << endl;
    cout << scale_DY0b.GetName() << " = " << scale_DY0b.getVal() << " +/- " << scale_DY0b.getError() << endl;
    cout << scale_DY1b.GetName() << " = " << scale_DY1b.getVal() << " +/- " << scale_DY1b.getError() << endl;
    cout << scale_DY2b.GetName() << " = " << scale_DY2b.getVal() << " +/- " << scale_DY2b.getError() << endl;
    cout << scale_bkgs.GetName() << " = " << scale_bkgs.getVal() << " +/- " << scale_bkgs.getError() << endl;

    /*
    /////////////////////////////////////////////////////////////////////////////////////

    TCanvas* c1 = new TCanvas("c1", "c1", 800, 400);
    c1->Divide(3, 1) ;
    
    RooPlot* frame1 = xvar.frame(Title("0 tag region")) ;
    combData.plotOn(frame1,Cut("region==region::0tag")) ;
    simPdf.plotOn(frame1,Slice(region,"0tag"),ProjWData(region,combData)) ;
    // simPdf.plotOn(frame1,Slice(region,"0tag"),ProjWData(region,combData),LineStyle(kDashed)) ;
    c1->cd(1);
    frame1->Draw();


    //   // NBL You _must_ project the sample index category with data using ProjWData 
    // as a RooSimultaneous makes no prediction on the shape in the index category 
    // and can thus not be integrated
    RooPlot* frame2 = xvar.frame(Title("1 tag region")) ;
    combData.plotOn(frame2,Cut("region==region::1tag")) ;
    simPdf.plotOn(frame2,Slice(region,"1tag"),ProjWData(region,combData)) ;
    // simPdf.plotOn(frame1,Slice(region,"0tag"),ProjWData(region,combData),LineStyle(kDashed)) ;
    c1->cd(2);
    frame2->Draw();

    RooPlot* frame3 = xvar.frame(Title("2 tag region")) ;
    combData.plotOn(frame3,Cut("region==region::2tag")) ;
    simPdf.plotOn(frame3,Slice(region,"2tag"),ProjWData(region,combData)) ;
    simPdf.plotOn(frame3,Slice(region,"2tag"),Components("2taghistpdf_bkgs"),ProjWData(region,combData),LineStyle(kDashed)) ;
    c1->cd(3);
    frame3->Draw();

    // RooPlot* frame = xvar.frame(Title("Observed data yields")) ;
    // vHistPdf_bkgs.at(2)->plotOn(frame) ;
    // frame->Draw();
    */
}  
