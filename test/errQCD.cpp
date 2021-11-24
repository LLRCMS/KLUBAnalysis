#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cctype>
#include <algorithm>
#include "TString.h"
#include "TChain.h"
#include "TCut.h"
#include "THStack.h"
#include "TCanvas.h"

#include "HistoManager.h"
#include "ConfigParser.h"
#include "utils.h"
#include "histoUtils.h"
#include "plotContainer.h"
#include "analysisUtils.h"
#include "TRandom3.h"

using namespace std ;


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


int main (int argc, char** argv)
{
  // check number of inpt parameters
  if (argc < 2)
  {
    cerr << "Forgot to put the cfg file --> exit " << endl ;
    return 1 ;
  }
  if (gConfigParser) return 1 ;
  gConfigParser = new ConfigParser () ;

  TString config , runs;
  config.Form ("%s",argv[1]) ;
  runs.Form("%s",argv[2]);
  if (! (gConfigParser->init (config)))
  {
    cout << ">>> parseConfigFile::Could not open configuration file " << config << endl ;
    return -1 ;
  }

  int nruns=runs.Atoi();

  float lumi = gConfigParser->readFloatOption ("general::lumi") ;
  cout << "READING lumi " << lumi << endl ;

  int maxEvtsMC = -1;
  if (gConfigParser->isDefined ("general::maxEvtsMC"))
    maxEvtsMC = gConfigParser -> readIntOption ("general::maxEvtsMC");

  // get the samples to be analised
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  // isolated samples
  // .... .... .... .... .... .... .... .... .... .... .... ....

  vector<string> sigSamplesList = gConfigParser->readStringListOption ("general::signals") ;
  vector<mysample> sigSamples ;
  readSamples (sigSamples, sigSamplesList) ;

  vector<float> signalScales ;
  for (unsigned int i = 0 ; i < sigSamplesList.size () ; ++i)
  {
    string name = string ("samples::") + sigSamplesList.at (i) + string ("FACT") ;
    float scale = gConfigParser->readFloatOption (name.c_str ()) ;
    signalScales.push_back (scale) ;
  }

  vector<string> bkgSamplesList = gConfigParser->readStringListOption ("general::backgrounds") ;
  vector<mysample> bkgSamples ;
  readSamples (bkgSamples, bkgSamplesList) ;

  vector<string> DATASamplesList = gConfigParser->readStringListOption ("general::data") ;
  vector<mysample> DATASamples ;
  readSamples (DATASamples, DATASamplesList) ;


  // get the selections to be applied
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<string> activeSelections = gConfigParser->readStringListOption ("selections::list") ;
  vector<pair <TString, TCut> > selections = readCutsFile (
    activeSelections,
    gConfigParser->readStringOption ("selections::selectionsFile")
    ) ;

  cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
  cout << "selections sequence: \n" ;
  for (unsigned int i = 0 ; i < selections.size () ; ++i)
    cout << selections.at (i).first << " : " << selections.at (i).second << endl ;

  // input selection must not have explicit iso on dau1, dau2 because QCD is relaxed and this selection is added here
  for (unsigned int i = 0 ; i < selections.size () ; ++i)
  {
    TString sel (selections.at (i).second);
    if ( sel.Contains("dau1_iso") || sel.Contains("dau2_iso") )
    {
      cout << endl;
      cout << " ---------------------------------------------------------------------------------- " << endl;
      cout << " ** WARNING: selection " << selections.at(i).first << " explicitly contains iso cut" << endl;
      cout << " ---------------------------------------------------------------------------------- " << endl;
      cout << endl;
    }
  }

  string sel_dau1_iso = gConfigParser->readStringOption ("selections::dau1iso");
  string sel_dau2_iso = gConfigParser->readStringOption ("selections::dau2iso");

  // ----------------------------------------
  // OS selections
  vector<pair <TString, TCut> > selections_OS = selections ;
  TCut dau1Cut = Form("dau1_iso < %s" , sel_dau1_iso.c_str());
  TCut dau2Cut = Form("dau2_iso < %s" , sel_dau2_iso.c_str());
  for (unsigned int i = 0 ; i < selections_OS.size () ; ++i)
  {
    selections_OS.at (i).first = TString ("OS_") + selections_OS.at (i).first ;
    selections_OS.at (i).second = selections_OS.at (i).second && TCut ("isOS != 0") && dau1Cut && dau2Cut;
  }

  // SS selections with tight iso (for QCD yield determination)
  vector<pair <TString, TCut> > selections_SS_tightIso = selections ;
  for (unsigned int i = 0 ; i < selections_SS_tightIso.size () ; ++i)
  {
    selections_SS_tightIso.at (i).first = TString ("SS_tightIso_") + selections_SS_tightIso.at (i).first ;
    selections_SS_tightIso.at (i).second = selections_SS_tightIso.at (i).second && TCut ("isOS == 0") && dau1Cut && dau2Cut;
  }

  // get the variables to be plotted
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<string> variablesList = gConfigParser->readStringListOption ("general::variables") ;
  vector<string> Buf_variables2DList(0);
  if (gConfigParser->isDefined ("general::2Dvariables")) Buf_variables2DList = gConfigParser->readStringListOption ("general::2Dvariables") ;

  vector<pair<string,string> > variables2DList ;
  for (unsigned int i = 0 ; i < Buf_variables2DList.size () ; ++i)
  {
    vector<string> dummy = split (Buf_variables2DList.at (i), ':') ;
    //cout << dummy.at (0) << " " << dummy.at (1) << " " << dummy.size () << endl ;
    variables2DList.push_back (make_pair(dummy.at (0), dummy.at (1)) ) ;
  }

  vector<pair <TString, TCut> > selections_SS = selections_SS_tightIso;

  // calculate the QCD in the SS region as data - other_bkg
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  cout << "--- MAIN reading DATA and filling SS histos" << endl ;

  // get the same-sign distributions from data
  plotContainer SS_DATA_plots ("SS_DATA", variablesList, variables2DList, selections_SS, DATASamplesList, 2) ;
  counters SS_DATACount = fillHistos (DATASamples, SS_DATA_plots,
				      variablesList, variables2DList,
				      selections_SS,
				      lumi,
				      vector<float> (0),
				      true, false) ;
  SS_DATA_plots.AddOverAndUnderFlow () ;

  cout << "--- MAIN reading bkg and filling SS histos" << endl ;

  // get the same-sign distributions from bkg
  plotContainer SS_bkg_plots ("SS_bkg", variablesList, variables2DList, selections_SS, bkgSamplesList, 0) ;
  counters SS_bkgCount = fillHistos (bkgSamples, SS_bkg_plots,
				     variablesList, variables2DList,
				     selections_SS,
				     lumi,
				     vector<float> (0),
				     false, false, maxEvtsMC) ;
  SS_bkg_plots.AddOverAndUnderFlow () ;

  cout << "--- MAIN preparing to loop on variables and selections to calc SS QCD" << endl ;

  // the index in the stack is based on variable ID (iv) and selection ID (isel):
  // iHisto = iv + nVars * isel
  vector<string> QCDsample ;
  QCDsample.push_back ("QCD") ;
  plotContainer SS_QCD ("SS_QCD", variablesList, variables2DList, selections_SS, QCDsample, 0) ;
//  vector <TH1F *> SS_QCD ;
  vector<vector<float>> QCDyieldSSregionRLXiso (variablesList.size(), vector<float>(selections_SS.size()) ); // var, cut
  vector<TH1F*> QCDYields ;


  /* FIXME should we subtract signals as well? */
  /* NB if it has to be subtracted, it cannot be scaled! */

  // now get QCD yields with the non-relaxed selections

  cout << "--- MAIN reading bkg and filling SS histos with non-relaxed iso" << endl ;

  // get the same-sign distributions from bkg
  plotContainer SS_tightIso_bkg_plots ("SS_tightIso_bkg", variablesList, variables2DList, selections_SS_tightIso, bkgSamplesList, 0) ;
  counters SS_tightIso_bkgCount = fillHistos (bkgSamples, SS_tightIso_bkg_plots,
					      variablesList, variables2DList,
					      selections_SS_tightIso,
					      lumi,
					      vector<float> (0),
					      false, false, maxEvtsMC) ;
  SS_tightIso_bkg_plots.AddOverAndUnderFlow () ;


  plotContainer SS_QCD_tightIso ("SS_tightIso_QCD", variablesList, variables2DList, selections_SS_tightIso, QCDsample, 0) ;
//  vector <TH1F *> SS_QCD ;
  vector<vector<float>> QCDyieldSSregionTightIso (variablesList.size(), vector<float>(selections_SS_tightIso.size()) ); // var, cut


  cout << "--- MAIN reading DATA and filling SS histos with non-relaxed iso" << endl ;

  // get the same-sign distributions from data
  //plotContainer SS_tightIso_DATA_array[nruns];
  TRandom3 *g = new TRandom3();

  for (unsigned int icut = 0 ; icut < selections_SS_tightIso.size () ; ++icut)
  {
    TH1F *h =new TH1F(selections_SS_tightIso.at (icut).first.Data (),selections_SS_tightIso.at (icut).first.Data (),10000,1,10001);
    QCDYields.push_back(h);
  }

  plotContainer SS_tightIso_DATA_plots ("SS_tightIso_DATA", variablesList, variables2DList, selections_SS_tightIso, DATASamplesList, 2) ;
  counters SS_tightIso_DATACount = fillHistos (DATASamples, SS_tightIso_DATA_plots,
					       variablesList, variables2DList,
					       selections_SS_tightIso,
					       lumi,
					       vector<float> (0),
					       true, false) ;
  SS_tightIso_DATA_plots.AddOverAndUnderFlow () ;
  //SS_tightIso_DATA_array[irun] = SS_tightIso_DATA_plots;
  //}

  //cout << "--- MAIN preparing to loop on variables and selections to calc SS QCD yield with non-relaxed iso" << endl ;

  // the index in the stack is based on variable ID (iv) and selection ID (isel):
  // iHisto = iv + nVars * isel

  //vector<string> QCDsample ;
  //QCDsample.push_back ("QCD") ;

  for (unsigned int icut = 0 ; icut < selections_SS_tightIso.size () ; ++icut)
  {
    for (unsigned int ivar = 0 ; ivar < variablesList.size () ; ++ivar)
    {
      //plotContainer SS_tightIso_DATA_plots = SS_tightIso_DATA_array[irun]
      THStack * D_stack = SS_tightIso_DATA_plots.makeStack (variablesList.at (ivar),
							    selections_SS_tightIso.at (icut).first.Data ()) ;
      TH1F * tempo = (TH1F *) D_stack->GetStack ()->Last () ;
      TH1F * orig = (TH1F*)tempo->Clone("original");
      THStack * b_stack = SS_tightIso_bkg_plots.makeStack (variablesList.at (ivar),
							   selections_SS_tightIso.at (icut).first.Data ()) ;
      TH1F * h_bkg = (TH1F *) b_stack->GetStack ()->Last () ;
      int nbins = tempo->GetNbinsX();

      for(int irun =0;irun<nruns;irun++){
	//TString name; name.Form("%s%d",tempo->GetName (),irun) ;
	//tempo->SetName(name);
	//tempo->SetBinErrorOption(TH1::kPoisson);
	for(int ibin=1; ibin<=nbins;ibin++){
	  tempo->SetBinContent(ibin,g->Poisson(orig->GetBinContent(ibin)));
	  //cout<<tempo->GetBinContent(ibin)<<" "<<orig->GetBinContent(ibin)<<endl;
	}

        //name = TString ("DDQCD_tightIso_") + name ;
        //TH1F * dummy = (TH1F *) tempo->Clone (name) ;
        tempo->Add (h_bkg, -1) ;
        //cout<<"integral "<<tempo->Integral()<<endl;
        QCDYields.at(icut)->Fill(tempo->Integral());
      }
    }
  }

  //}
  // get the SS-to-OS scale factor and scale the QCD distributions
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


  // Save the histograms
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  cout << "--- MAIN before saving" << endl ;
  TString outString ;
  outString.Form ("outPlotter.root") ;
  TFile * fOut = new TFile (outString.Data (), "RECREATE") ;

  fOut->cd();
  for(uint i=0; i<QCDYields.size();i++)QCDYields.at(i)->Write();
  fOut->Close () ;


  return 0 ;
}
