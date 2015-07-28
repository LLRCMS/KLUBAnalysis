#include "utils.h"

using namespace std ;


sample::sample (TString theSampleName, TString theSampleFileName, 
                TString readingOption, TString treeName) : 
  sampleName (theSampleName),
  sampleFileName (theSampleFileName) 
{
  sampleFile = new TFile (sampleFileName, readingOption) ;
  sampleTree = (TTree *) sampleFile->Get (treeName) ;
  calcEfficiency () ;
  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


float sample::calcEfficiency ()
{
  TH1F * effHisto = (TH1F *) sampleFile->Get ("h_eff") ;
  if (effHisto->GetBinContent (1) == 0) 
    {
      eff_num = 0. ;
      eff_den = 0. ;
      eff = 0. ;
      return 0. ;
    }
  eff_num = effHisto->GetBinContent (2) ;
  eff_den = effHisto->GetBinContent (1) ;
  eff = eff_num / eff_den ;
  return eff ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


int
readSamples (vector<sample> & samples, vector<string> & samplesList, TString readOption)
{
  for (unsigned int iSample = 0 ; iSample < samplesList.size () ; ++iSample)
    {
      TString sampleFolder = gConfigParser->readStringOption (
          TString ("samples::") + samplesList.at (iSample).c_str ()
        ) ;
      cout << "reading " << samplesList.at (iSample) << " from : " << sampleFolder << "\n" ; 
      samples.push_back (sample (samplesList.at (iSample), sampleFolder + "/total.root", readOption)) ; 
      int done = samples.back ().sampleTree->GetEntries () ;  
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


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
 {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
