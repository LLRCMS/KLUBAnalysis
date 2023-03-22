#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <cmath>
#include <cctype>
#include <algorithm>
#include "TString.h"
#include "TChain.h"
#include "TCut.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "HistoManager.h"
#include "CfgParser.h"
#include "utils.h"
#include "histoUtils.h"
#include "plotContainer.h"
#include "analysisUtils.h"


using namespace std ;


struct QCDevalTools
{
  QCDevalTools (const string & name) :
    m_name (name),
    m_DATA_plots (m_name + "_DATA"),
    m_bkg_plots (m_name + "_bkg"),
    m_QCD_plots (m_name + "_QCD")
    {}
  string m_name ;
  plotContainer m_DATA_plots ;
  plotContainer m_bkg_plots ;
  plotContainer m_QCD_plots ;
  vector<pair <TString, TCut> > m_selections ;
} ;


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// calculate the QCD in the SS region as data - other_bkg
void getQCD (QCDevalTools & QCDET, vector<string> variablesList)
{
  for (unsigned int ivar = 0 ; ivar < variablesList.size () ; ++ivar)
  {
    for (unsigned int icut = 0 ; icut < QCDET.m_selections.size () ; ++icut)
    {
      THStack * D_stack = QCDET.m_DATA_plots.makeStack (variablesList.at (ivar),
							QCDET.m_selections.at (icut).first.Data ()) ;
      TH1F * tempo = (TH1F *) D_stack->GetStack ()->Last () ;
      TString name = tempo->GetName () ;
      name = TString ("DDQCD_") + name ;
      TH1F * dummy = (TH1F *) tempo->Clone (name) ;

      THStack * b_stack = QCDET.m_bkg_plots.makeStack (variablesList.at (ivar),
						       QCDET.m_selections.at (icut).first.Data ()) ;
      TH1F * h_bkg = (TH1F *) b_stack->GetStack ()->Last () ;
      dummy->Add (h_bkg, -1) ;
      QCDET.m_QCD_plots.m_histos[variablesList.at (ivar)][QCDET.m_selections.at (icut).first.Data ()]["QCD"] = dummy ;
    }
  }
  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


int main (int argc, char** argv)
{
  // check number of inpt parameters
  if (argc < 2)
  {
    cerr << "Forgot to put the cfg file --> exit " << endl ;
    return 1 ;
  }


  unique_ptr<CfgParser> lConfigParser (new CfgParser(argv[1]));
  float lumi = lConfigParser->readFloatOpt ("general::lumi") ;
  cout << "READING lumi " << lumi << endl ;

  // get the samples to be analised
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  string sampleCfgFileName = lConfigParser->readStringOpt ("configs::sampleCfg");
  cout<<sampleCfgFileName<<endl;

  if (! (lConfigParser->init (sampleCfgFileName.c_str())))
  {
    cout << ">>> parseConfigFile::Could not open configuration file " << sampleCfgFileName << endl ;
    return -1 ;
  }
  bool split = false;
  int idx = 0;
  int njobs = 1;
  string outFolder = "/data_CMS/cms/alves/HHresonant_hist/LOOKUP_DUMMY_TAG";
  if (argc==4 or argc ==5)
  {
    split = true;
    idx = atoi(argv[2]);
    njobs = atoi(argv[3]);
  }

  if (argc==5)
    outFolder = atoi(argv[4]);

  AnalysisHelper ah(argv[1], "merge_plots");

  if (split) ah.setSplitting(idx, njobs);
  ah.readSelections();
  ah.readVariables();
  try {ah.readSamples();}
  catch (std::exception &ex)
  {
    cerr << "*** Error in reading samples because: " << ex.what() << endl;
    return 1;
  }

  ah.prepareHistos();
  ah.dump(2); // can set a level of detail
  ah.fillHistos();
  ah.mergeSamples(); // do it just at the end
  ah.saveOutputsToFile(outFolder);
}
