// #include "TTree.h"
// #include "TFile.h"
// #include "TF1.h"
// #include "TCut.h"
// #include <iostream>
// #include "RooRealVar.h"
// #include "RooDataSet.h"
// #include "RooDataHist.h"
// #include "RooGaussian.h"
// #include "TCanvas.h"
// #include "RooPlot.h"

using namespace std;
using namespace RooFit;

double CrystalBall
(double* x, double* par){ 
  //http://en.wikipedia.org/wiki/Crystal_Ball_function 
  double xcur = x[0]; 
  double alpha = par[0]; 
  double n = par[1]; 
  double mu = par[2]; 
  double sigma = par[3]; 
  // double N = par[4]; no normalization
  double N = 1.0;

  TF1* exp = new TF1("exp","exp(x)",1e-20,1e20); 
  double A; double B; 
  if (alpha < 0){ 
    A = pow((n/(-1*alpha)),n)*exp->Eval((-1)*alpha*alpha/2); 
    B = n/(-1*alpha) + alpha;} 
  else { 
    A = pow((n/alpha),n)*exp->Eval((-1)*alpha*alpha/2); 
    B = n/alpha - alpha;} 
    double f; 
  if (TMath::Abs((xcur-mu)/sigma) < TMath::Abs(alpha) ) 
    f = N*exp->Eval((-1)*(xcur-mu)*(xcur-mu)/(2*sigma*sigma)); 
  else if (((xcur-mu)/sigma) < (-1.)*alpha )
    f = N*A*pow((B- (xcur-mu)/sigma),(-1*n)); // left tail
  else
    f = N*A*pow( (B- (mu-xcur)/sigma),(-1*n)); // right tail
  delete exp; 
  return f; 
} 

// int main()
vector<float> FitUnbinnedSingle(int mass, TString fInName)
{
    TCanvas* c1 = new TCanvas;

    // float mass = 800;
    TFile* fIn = new TFile (fInName);
    TTree* t = (TTree*) fIn->Get("HTauTauTree");

    // // mu tau, b tag M
    // TCut cut = "pairType == 0 && dau1_pt > 19 && abs (dau1_eta) < 2.1 && dau2_pt > 20 && abs (dau2_eta) < 2.3 && nleps == 0 && bjet1_bID > 0.800 && bjet2_bID > 0.800 && bH_mass_raw > 80 && bH_mass_raw < 160 && tauH_SVFIT_mass > 80 && tauH_SVFIT_mass < 160 && isOS != 0 && dau1_iso < 0.1 && dau2_iso < 3.0";
    // cout << cut.GetTitle() << endl;

    // // e tau
    // TCut cut = "pairType == 1 && dau1_pt > 24 && abs (dau1_eta) < 2.1 && dau2_pt > 20 && abs (dau2_eta) < 2.3 && nleps == 0 && bjet1_bID > 0.800 && bjet2_bID > 0.800 && bH_mass_raw > 80 && bH_mass_raw < 160 && tauH_SVFIT_mass > 80 && tauH_SVFIT_mass < 160 && isOS != 0 && dau1_iso < 0.1 && dau2_iso < 3.0";
    // cout << cut.GetTitle() << endl;

    // tau tau
    TCut cut = "pairType == 2 && dau1_pt > 45 && abs (dau1_eta) < 2.1 && dau2_pt > 45 && abs (dau2_eta) < 2.1 && nleps == 0 && bjet1_bID > 0.460 && bjet2_bID > 0.460 && bH_mass_raw > 80 && bH_mass_raw < 160 && tauH_SVFIT_mass > 80 && tauH_SVFIT_mass < 160 && HH_mass_raw > 200 && bTagweightL > -999 && isOS != 0 && dau1_iso < 2.0 && dau2_iso < 2.0";
    cout << cut.GetTitle() << endl;    

    // int nSelec = t->Draw("HHKin_mass_raw", cut);
    // cout << "EVENTS: " << nSelec << endl;

    // fit in 2 sigma, resolutiono f kinfit is ~10%
    // TF1* gaussiana = new TF1 ("gaussiana", "", 0.8*mass, 1.2*mass);
    // t->UnbinnedFit ("gaus", "HHKin_mass_raw", cut);

    TFile* fOut = new TFile ("fOut.root", "recreate");
    TTree* selTree = t->CopyTree(cut);
    float ww;
    
    TBranch *bww = selTree->Branch ("prodWeights", &ww);
    float IdAndIsoSF, trigSF, bTagweightM, bTagweightL;
    selTree->SetBranchAddress("IdAndIsoSF", &IdAndIsoSF);
    selTree->SetBranchAddress("trigSF", &trigSF);
    selTree->SetBranchAddress("bTagweightM", &bTagweightM);
    selTree->SetBranchAddress("bTagweightL", &bTagweightL);

    // create a new branch with the weights
    for (int i = 0; i < selTree->GetEntries(); ++i)
    {
        selTree->GetEntry(i);
        // ww = IdAndIsoSF*trigSF*bTagweightM;
        ww = IdAndIsoSF*trigSF*bTagweightL;
        bww->Fill();
    }

    cout << ".. going to fit" << endl;

    RooRealVar x("HHKin_mass_raw","HHKin_mass_raw",0.7*mass,1.3*mass) ;
    RooPlot* xframe = x.frame() ;

    RooRealVar weights("prodWeights","prodWeights", 0, 100);

    // FIXME: non va
    // RooRealVar IdAndIsoSF("IdAndIsoSF","IdAndIsoSF",0,100);
    // RooRealVar trigSF("trigSF","trigSF",0,100);
    // RooRealVar bTagweightM("bTagweightM", "bTagweightM", 0, 100);
    // RooRealVar bTagweightL("bTagweightL", "bTagweightL", 0, 100);

    // RooRealVar EventNumber("EventNumber", "EventNumber", 0, 1000);

    // RooProduct weights("weights","weights",RooArgSet(IdAndIsoSF,trigSF, bTagweightM));
    // RooProduct weights("weights","weights",RooArgSet(IdAndIsoSF,trigSF, bTagweightL));

    // RooDataSet ds("ds","ds",RooArgSet(x), Import(*selTree) ) ;
    RooDataSet* ds = new RooDataSet("ds","ds",RooArgSet(x,weights), Import(*selTree) ) ;

    // creo weighted dataset (SF, trig, etc...)
    RooDataSet dataW("ds","ds",ds,*ds->get(),0,"prodWeights");
    // RooDataSet dsw(ds.GetName(), ds.GetTitle(), ds, ds.get(), 0, dau1_pt.GetName());

    RooRealVar mean("mean","mean",mass, 0.9*mass, 1.1*mass);
    RooRealVar sigma("sigma","sigma",0.03*mass, 0, 1.5*mass) ;    
    dataW.plotOn (xframe, Name("ds"));

    // RooGaussian gaus("gaussian","gaussian fit",x,mean,sigma) ;
    // RooFitResult* fit = gaus.fitTo(ds, Save());
    // gaus.plotOn (xframe, LineColor(kGreen), Name("gaussian"));


    // RooCBShape (const char *name, const char *title, RooAbsReal &_m, RooAbsReal &_m0, RooAbsReal &_sigma, RooAbsReal &_alpha, RooAbsReal &_n)
    // RooRealVar alpha("alpha","alpha",2, 0.0001, 10);
    // RooRealVar n("n","n",1, 0, 1000);
    // RooCBShape CB("CB","CB fit",x,mean, sigma, alpha, n) ;
    // RooFitResult* fitCB = CB.fitTo(ds, Save());
    // CB.plotOn (xframe, LineColor(kOrange), Name("CB"));

    // // RooBreitWigner (const char *name, const char *title, RooAbsReal &_x, RooAbsReal &_mean, RooAbsReal &_width)
    RooBreitWigner BW("BW","BW fit",x,mean,sigma) ;
    RooFitResult* fitBW = BW.fitTo(dataW, Save());
    BW.plotOn (xframe, LineColor(kBlue), Name("BW"));

    // // // RooVoigtian (const char *name, const char *title, RooAbsReal &_x, RooAbsReal &_mean, RooAbsReal &_width, RooAbsReal &_sigma, Bool_t doFast=kFALSE)
    // RooRealVar width("width","width",0.03*mass, 0, 1.5*mass) ;    
    // RooVoigtian Voigtian("Voigtian","Voigtian fit",x,mean,width,sigma) ;
    // RooFitResult* fitVoigtian = Voigtian.fitTo(ds, Save());
    // Voigtian.plotOn (xframe, LineColor(kRed), Name("Voigtian"));

    // NON RIESCO A FARE LA DOPPIA CRYSTAL BALL!!!!
    // // // double sided CB
    // TF1* tf1DoubleCB = new TF1("tf1DoubleCB",&CrystalBall,0.8*mass, 1.2*mass, 4);
    // cout << tf1DoubleCB->Eval(0) << endl;
    // RooRealVar alpha("alpha","alpha",1, 0, 5);
    // RooRealVar n("n","n",0.1, 0, 100);
    // // RooAbsPdf* RooDoubleCB =  bindPdf("RooDoubleCB", tf1DoubleCB, x, mean, sigma, alpha, n);
    // RooAbsPdf* RooDoubleCB =  bindPdf("RooDoubleCB", CrystalBall, x, mean, sigma, alpha, n);
    // // RooAbsPdf* RooDoubleCB =  bindPdf(tf1DoubleCB, x);
    // RooFitResult* fitCB = RooDoubleCB->fitTo(ds, Save());
// RooDoubleCB->plotOn (xframe, LineColor(kOrange), Name("DoubleCB"));
    // RooRealVar null("null","null",0);
    
    // RooGenericPdf (const char *name, const char *title, const char *formula, const RooArgList &dependents)
    
    // RooGenericPdf CB ("CB", "CB", "((x-mean)/sigma > )", RooArgList (x, mean, sigma, alpha, n))

    // RooRealVar x2("x2","x2",0,0.999) ;
    // RooRealVar a("a","a",5,0,10) ;
    // RooRealVar b("b","b",2,0,10) ;
    // RooAbsPdf* beta = bindPdf("beta",ROOT::Math::beta_pdf,x2,a,b) ;

    // beta->fitTo (ds);
    // beta->plotOn(xframe, LineColor(kOrange));

    // RooFormulaVar absV("absV", "absV", "TMath::Abs(HHKin_mass_raw-mean)", RooArgSet(x, mean)); 
    // RooCBShape CB("CB","CB fit",absV,null, sigma, alpha, n) ;
    // RooFitResult* fitCB = CB.fitTo(ds, Save());
    // CB.plotOn (xframe, LineColor(kOrange), Name("CB"));


    // cout << " ******* Gaus fit chi square         : " << xframe->chiSquare("gaussian", "ds") << endl;
    // cout << " ******* Breit Wigner fit chi square : " << xframe->chiSquare("BW", "ds") << endl;
    // cout << " ******* Voigtian fit chi square     : " << xframe->chiSquare("Voigtian", "ds") << endl;

   
    // CB.plotOn(xframe);
    
    float meanf = mean.getVal();
    float sigmaf = sigma.getVal();
    vector <float> result (4);
    result.at(0) = meanf;
    result.at(1) = sigmaf;
    result.at(2) = mean.getError();
    result.at(3) = sigma.getError();

    xframe->Draw();
    c1->Print (Form("fit_%i.pdf", mass), "pdf");
    
    return result;
}


void FitUnbinned()
{
    vector<pair <int, TString> > masse;
    masse.push_back( make_pair( 250, "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_GluGluToRadionToHHTo2B2Tau_M-250/total.root"));
    masse.push_back( make_pair( 260, "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_GluGluToRadionToHHTo2B2Tau_M-260/total.root"));
    masse.push_back( make_pair( 270, "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_GluGluToRadionToHHTo2B2Tau_M-270/total.root"));
    masse.push_back( make_pair( 280, "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_GluGluToRadionToHHTo2B2Tau_M-280/total.root"));
    masse.push_back( make_pair( 300, "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_GluGluToRadionToHHTo2B2Tau_M-300/total.root"));
    masse.push_back( make_pair( 320, "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_GluGluToRadionToHHTo2B2Tau_M-320/total.root"));
    masse.push_back( make_pair( 340, "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_GluGluToRadionToHHTo2B2Tau_M-340/total.root"));
    masse.push_back( make_pair( 350, "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_GluGluToRadionToHHTo2B2Tau_M-350/total.root"));
    masse.push_back( make_pair( 400, "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_GluGluToRadionToHHTo2B2Tau_M-400/total.root"));
    masse.push_back( make_pair( 450, "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_new450/SKIM_GluGluToRadionToHHTo2B2Tau_M-450/total.root"));
    masse.push_back( make_pair( 500, "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_GluGluToRadionToHHTo2B2Tau_M-500/total.root"));
    masse.push_back( make_pair( 550, "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_GluGluToRadionToHHTo2B2Tau_M-550/total.root"));
    masse.push_back( make_pair( 600, "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_GluGluToRadionToHHTo2B2Tau_M-600/total.root"));
    masse.push_back( make_pair( 650, "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_GluGluToRadionToHHTo2B2Tau_M-650/total.root"));
    masse.push_back( make_pair( 700, "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_GluGluToRadionToHHTo2B2Tau_M-700/total.root"));
    masse.push_back( make_pair( 800, "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_GluGluToRadionToHHTo2B2Tau_M-800/total.root"));
    masse.push_back( make_pair( 900, "/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_GluGluToRadionToHHTo2B2Tau_M-900/total.root"));

    vector <vector<float> > res;
    for (int i = 0; i < masse.size(); i++)
    {
        vector<float> ll = FitUnbinnedSingle (masse.at(i).first, masse.at(i).second);
        res.push_back(ll);
    }


    cout << "MEAN" << endl;
    for (int i = 0; i < res.size(); i++)
    {
        // cout << masse.at(i).first << " m: " << res.at(i).at(0) << " +/- " << res.at(i).at(2) << " sigma: " << res.at(i).at(1) << " +/- " << res.at(i).at(3) << endl;
        cout << masse.at(i).first << " " << res.at(i).at(0) << " 0 " << res.at(i).at(2) << endl;
    }

    cout << endl << "SIGMA" << endl;
    for (int i = 0; i < res.size(); i++)
    {
        // cout << masse.at(i).first << " m: " << res.at(i).at(0) << " +/- " << res.at(i).at(2) << " sigma: " << res.at(i).at(1) << " +/- " << res.at(i).at(3) << endl;
        cout << masse.at(i).first << " " << res.at(i).at(1) << " 0 " << res.at(i).at(3) << endl;
    }


    // cout << "OOOOOOOOOOO FINITO" << endl;

}