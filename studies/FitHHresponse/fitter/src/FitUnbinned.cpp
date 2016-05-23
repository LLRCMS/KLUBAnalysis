#include "TTree.h"
#include "TFile.h"
#include "TF1.h"
#include "TCut.h"
#include <iostream>
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooBreitWigner.h"
#include "RooVoigtian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooDoubleCB.h"

using namespace std;
using namespace RooFit;

// c++ -lm -o FitUnbinned FitUnbinned.cpp RooDoubleCB.cxx `root-config --glibs --cflags` -lRooFit  -lRooFitCore 

// int main()
vector<float> FitUnbinnedSingle(int mass, TString fName)
{
    // float mass = 800;
    // TFile* fIn = new TFile ("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims_76X_allJets_5/SKIM_GluGluToRadionToHHTo2B2Tau_M-800/total.root");
    TFile* fIn = new TFile (fName);
    TTree* t = (TTree*) fIn->Get("HTauTauTree");

    // mu tau
    TCut cut = "pairType == 0 && dau1_pt > 19 && abs (dau1_eta) < 2.1 && dau2_pt > 20 && abs (dau2_eta) < 2.3 && nleps == 0 && bjet1_bID > 0.460 && bjet2_bID > 0.460 && bH_mass_raw > 80 && bH_mass_raw < 160 && tauH_SVFIT_mass > 80 && tauH_SVFIT_mass < 160 && bTagweightM > -999 && isOS != 0 && dau1_iso < 0.1 && dau2_iso < 3.0";
    cout << cut.GetTitle() << endl;

    // int nSelec = t->Draw("HHKin_mass_raw", cut);
    // cout << "EVENTS: " << nSelec << endl;

    // fit in 2 sigma, resolutiono f kinfit is ~10%
    // TF1* gaussiana = new TF1 ("gaussiana", "", 0.8*mass, 1.2*mass);
    // t->UnbinnedFit ("gaus", "HHKin_mass_raw", cut);

    TCanvas* c1 = new TCanvas;
    TFile* fOut = new TFile ("fOutDummy.root", "recreate"); // needed for copytree
    TTree* selTree = t->CopyTree(cut);

    cout << ".. going to fit" << endl;

    RooRealVar x("HHKin_mass_raw","HHKin_mass_raw",0.8*mass,1.2*mass) ;
    RooPlot* xframe = x.frame() ;

    RooDataSet ds("ds","ds",RooArgSet(x),Import(*selTree) ) ;

    RooRealVar mean("mean","mean",mass, 0.9*mass, 1.1*mass);
    RooRealVar sigma("sigma","sigma",0.03*mass, 0, 1.5*mass) ;    
    ds.plotOn (xframe, Name("ds"));

    // RooGaussian gaus("gaussian","gaussian fit",x,mean,sigma) ;
    // RooFitResult* fit = gaus.fitTo(ds, Save());
    // gaus.plotOn (xframe, LineColor(kGreen), Name("gaussian"));


    // RooCBShape (const char *name, const char *title, RooAbsReal &_m, RooAbsReal &_m0, RooAbsReal &_sigma, RooAbsReal &_alpha, RooAbsReal &_n)
    RooRealVar alpha("alpha","alpha",1, 0.01, 10);
    RooRealVar n("n","n",0.5, 0, 1000);
    RooDoubleCB CB("CB","CB fit",x,mean, sigma, alpha, n) ;    
    RooFitResult* fitCB = CB.fitTo(ds, Save());
    CB.plotOn(xframe, LineColor(kGreen), Name("CB"));

    // // RooBreitWigner (const char *name, const char *title, RooAbsReal &_x, RooAbsReal &_mean, RooAbsReal &_width)
    // RooBreitWigner BW("BW","BW fit",x,mean,sigma) ;
    // RooFitResult* fitBW = BW.fitTo(ds, Save());
    // BW.plotOn (xframe, LineColor(kBlue), Name("BW"));


    // // // RooVoigtian (const char *name, const char *title, RooAbsReal &_x, RooAbsReal &_mean, RooAbsReal &_width, RooAbsReal &_sigma, Bool_t doFast=kFALSE)
    // RooRealVar width("width","width",0.03*mass, 0, 1.5*mass) ;    
    // RooVoigtian Voigtian("Voigtian","Voigtian fit",x,mean,width,sigma) ;
    // RooFitResult* fitVoigtian = Voigtian.fitTo(ds, Save());
    // Voigtian.plotOn (xframe, LineColor(kRed), Name("Voigtian"));

    // cout << " ******* Gaus fit chi square         : " << xframe->chiSquare("gaussian", "ds") << endl;
    // cout << " ******* Breit Wigner fit chi square : " << xframe->chiSquare("BW", "ds") << endl;
    // cout << " ******* CrystalBall fit chi square  : " << xframe->chiSquare("CB", "ds") << endl;
    // cout << " ******* Voigtian fit chi square     : " << xframe->chiSquare("Voigtian", "ds") << endl;


    xframe->Draw();
    c1->Print (Form("fit_%i.pdf", mass), "pdf");

    // results
    // float meanf = mean.getVal();
    // float sigmaf = sigma.getVal();
    vector <float> result (8);
    result.at(0) = mean.getVal();
    result.at(1) = mean.getError();
    result.at(2) = sigma.getVal();
    result.at(3) = sigma.getError();
    result.at(4) = alpha.getVal();
    result.at(5) = alpha.getError();
    result.at(6) = n.getVal();
    result.at(7) = n.getError();

    return result;

}


int main()
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

    for (int i = 0; i < res.size(); i++)
    {
        cout << masse.at(i).first << " m: " << res.at(i).at(0) << " +/- " << res.at(i).at(1) <<
        " sigma: " << res.at(i).at(2) << " +/- " << res.at(i).at(3) <<
        " alpha: " << res.at(i).at(4) << " +/- " << res.at(i).at(5) <<
        " n: " << res.at(i).at(6) << " +/- " << res.at(i).at(7) <<
         endl;
    }
}
