#include "utils.h"

using namespace std ;


float sample::calcEfficiency ()
{
  TObjArray * fileElements = sampleChain->GetListOfFiles () ;
  TIter next (fileElements) ;
  TChainElement *chEl = (TChainElement*) next () ;
  TFile f (chEl->GetTitle ()) ;
  TH1F * effHisto = (TH1F *) ((TH1F *) f.Get ("h_eff"))->Clone (TString ("effHisto_") + sampleName) ;
  while (( chEl = (TChainElement*) next ())) 
    {
      TFile f (chEl->GetTitle ()) ;
      effHisto->Add ((TH1F *) f.Get ("h_eff")) ;
    }
  if (effHisto->GetBinContent (1) == 0) return 0. ;
  eff_num = effHisto->GetBinContent (2) ;
  eff_den = effHisto->GetBinContent (1) ;
  return effHisto->GetBinContent (2) / effHisto->GetBinContent (1) ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


int
readSamples (vector<sample> & samples, vector<string> & samplesList)
{
  for (unsigned int iSample = 0 ; iSample < samplesList.size () ; ++iSample)
    {
      TString sampleFolder = gConfigParser->readStringOption (
          TString ("samples::") + samplesList.at (iSample).c_str ()
        ) ;
      cout << "reading " << samplesList.at (iSample) << " : " << sampleFolder << "\n" ; 
      samples.push_back (sample (samplesList.at (iSample))) ; 
      int done = samples.back ().addFiles (sampleFolder) ;  
      cout << " --> read " << done << " events\n" ; 
    }
  return 0 ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


vector<pair<TString, TCut> >
readCutsFile (string filename)
{
  ifstream infile (filename) ;
  vector<pair<TString, TCut> > selections ;

  string line ;
  while (getline (infile, line))
    {
      istringstream iss (line) ;
      string dummy ;
      iss >> dummy ;
      if (dummy.size () < 2) continue ;
      size_t found = dummy.find ("%") ;
      if (found != string::npos) continue ;
      size_t limit = line.find_first_of ("=") ;
      if (limit == string::npos) continue ;
//       cout << line << endl ;
//       cout << limit << endl ;
      string selection = line.substr (limit+1) ;
//       cout << dummy << "|" << selection << endl ;
      selections.push_back (pair<TString, TCut> (dummy.c_str (), selection.c_str ())) ;
    }
  return selections ;  
}
