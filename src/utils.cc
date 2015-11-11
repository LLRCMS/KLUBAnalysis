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


vector<pair <TString, TCut> > 
readCutsFile (vector<string> activeSelections, string filename)
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
      size_t found = dummy.find ("#") ;
      if (found != string::npos) continue ;
      size_t limit = line.find_first_of ("=") ;
      if (limit == string::npos) continue ;
//       cout << line << endl ;
//       cout << limit << endl ;
      string selection = line.substr (limit+1) ;
//       cout << dummy << "|" << selection << endl ;
      if (find (activeSelections.begin (), activeSelections.end (), dummy) 
          == activeSelections.end ())
        continue ;
      selections.push_back (pair<TString, TCut> (dummy.c_str (), selection.c_str ())) ;
    }
  return selections ;  
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

vector<pair <string, string> > readVarNamesFile (vector<string> varList, string filename)
{
  ifstream infile (filename) ;
  vector<pair<string, string> > varNames ;

  string comments("@@@");

  string line ;
  while (getline (infile, line))
  {
      istringstream iss (line) ;
      string dummy ;
      iss >> dummy ;
      //cout << "PARSE:--" << dummy << endl;
      if (dummy.size () < 2) continue ;
      size_t found = dummy.find (comments) ;
      if (found != string::npos) continue ;
      size_t limit = line.find_first_of ("=") ;
      if (limit == string::npos) continue ;    
      string name = line.substr (limit+1) ;
      if (find (varList.begin (), varList.end (), dummy) 
          == varList.end ())
        continue ;
      // trim tring as whitespace interfere with ROOT positioning
      size_t first = name.find_first_not_of(string(" \t\f\v\n\r"));
      size_t last = name.find_last_not_of(string(" \t\f\v\n\r"));
      name = name.substr(first, (last-first+1));

      //name = string(boost::algorithm::trim( name )) ;
      //varNames.push_back (pair<string, string> (dummy, name) ) ;
      //cout << "!" << dummy << "! !" << name << "!" << endl;
      varNames.push_back(make_pair(dummy, name));
  }
  return varNames;
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


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void addTo (vector<float> & total, vector<float> & addition)
{
  for (unsigned int i = 0 ; i < total.size () ; ++i)
    total.at (i) += addition.at (i) ;
  return ;  
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

void printTableTitle (std::ostream& out, vector<string> & sample, unsigned int NSpacesColZero, unsigned int NSpacesColumns) 
{
  for (unsigned int i = 0 ; i < NSpacesColZero ; ++i) out << " " ;
  out << "| " ;
  for (unsigned int iSample = 0 ; iSample < sample.size () ; ++iSample)
    {
      string word = sample.at (iSample).substr (0, NSpacesColumns) ;
      out << word ;
      if ( NSpacesColumns < word.size () ) NSpacesColumns = word.size();
      for (unsigned int i = 0 ; i < NSpacesColumns - word.size () ; ++i) out << " " ;
      out << " | " ;
    }
  out << "\n" ; 
  return ;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

void printTableTitle (std::ostream& out, vector<sample> & sample, unsigned int NSpacesColZero, unsigned int NSpacesColumns) 
{
  vector<string> names ;
  for (unsigned int iSample = 0 ; iSample < sample.size () ; ++iSample)
    names.push_back (sample.at (iSample).sampleName.Data ()) ;
  printTableTitle (out, names, NSpacesColZero, NSpacesColumns) ;
  return ;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

void printTableTitle (std::ostream& out, vector<sample> & sample, vector<string> & DataDrivenBkgsName, unsigned int NSpacesColZero, unsigned int NSpacesColumns)
{
  vector<string> names ;
  for (unsigned int iSample = 0 ; iSample < sample.size () ; ++iSample)
    names.push_back (sample.at (iSample).sampleName.Data ()) ;

  for (unsigned int iDDSample = 0; iDDSample < DataDrivenBkgsName.size(); ++iDDSample)
    names.push_back (DataDrivenBkgsName.at(iDDSample));
  
  printTableTitle (out, names, NSpacesColZero, NSpacesColumns) ;
  return ;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
void printTableBody  (std::ostream& out, vector<pair <TString, TCut> > & selections, counters & count, vector<sample> & samples,
                      unsigned int NSpacesColZero, unsigned int NSpacesColumns, unsigned int precision)
{
  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
  {
    out << selections.at (iSel).first ;
    if ( NSpacesColZero < string(selections.at (iSel).first.Data ()).size () ) NSpacesColZero = string(selections.at (iSel).first.Data ()).size () ; //guard if name too long
    for (unsigned int i = 0 ; i < NSpacesColZero - string(selections.at (iSel).first.Data ()).size () ; ++i) out << " " ;
    out << "|" ;
    for (unsigned int iSample = 0 ; iSample < samples.size () ; ++iSample)
      {
        float evtnum = count.counters.at (iSample).at (iSel+1) ;
        int subtractspace = 0 ;
        if (evtnum > 0) subtractspace = int (log10 (evtnum)) + precision + 1 ;
        for (unsigned int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) out << " " ;
        out << setprecision (precision) << fixed << evtnum
             << " |" ;
      }
    out << "\n" ;
  }
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
void printTableBody  (std::ostream& out, vector<pair <TString, TCut> > & selections, counters & count, vector<sample> & samples, vector<vector<float>> & DataDrivenSamplesYields,
                      unsigned int NSpacesColZero, unsigned int NSpacesColumns, unsigned int precision)
{
  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
  {
    out << selections.at (iSel).first ;
    if ( NSpacesColZero < string(selections.at (iSel).first.Data ()).size () ) NSpacesColZero = string(selections.at (iSel).first.Data ()).size () ; //guard if name too long
    for (unsigned int i = 0 ; i < NSpacesColZero - string(selections.at (iSel).first.Data ()).size () ; ++i) out << " " ;
    out << "|" ;
    
    for (unsigned int iSample = 0 ; iSample < samples.size () ; ++iSample)
      {
        float evtnum = count.counters.at (iSample).at (iSel+1) ;
        int subtractspace = 0 ;
        if (evtnum > 0) subtractspace = int (log10 (evtnum)) + precision + 1 ;
        for (unsigned int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) out << " " ;
        out << setprecision (precision) << fixed << evtnum
             << " |" ;
      }
    for (unsigned int iDDSample = 0 ; iDDSample < DataDrivenSamplesYields.size () ; ++iDDSample)
      {
        float evtnum = DataDrivenSamplesYields.at(iDDSample).at(iSel) ;
        int subtractspace = 0 ;
        if (evtnum > 0) subtractspace = int (log10 (evtnum)) + precision + 1 ;
        for (unsigned int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) out << " " ;
        out << setprecision (precision) << fixed << evtnum
             << " |" ;
      }

    out << "\n" ; 
  }
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
void printTableBodyEff  (std::ostream& out, vector<pair <TString, TCut> > & selections, counters & count, vector<sample> & samples,
                      unsigned int NSpacesColZero, unsigned int NSpacesColumns, unsigned int precision)
{
  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
  {
    out << selections.at (iSel).first ;
    if ( NSpacesColZero < string(selections.at (iSel).first.Data ()).size () ) NSpacesColZero = string(selections.at (iSel).first.Data ()).size () ; //guard if name too long
    for (unsigned int i = 0 ; i < NSpacesColZero - string(selections.at (iSel).first.Data ()).size () ; ++i) out << " " ;
    out << "|" ;
    for (unsigned int iSample = 0 ; iSample < samples.size () ; ++iSample)
      {
        //float evtnum = count.counters.at (iSample).at (iSel+1) ;
        float relEff = count.counters.at (iSample).at(iSel+1) / count.counters.at (iSample).at(0) ; // relative eff selected / (tot after skim)
        float skimEff = count.initEfficiencies.at(iSample);
        
        float totEff = relEff * skimEff ;

        // percent
        totEff *= 100;
        relEff *= 100;

        int subtractspace = 0 ;
        //if (evtnum > 0) subtractspace = int (log10 (evtnum)) + precision + 1 ;
        //for (unsigned int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) out << " " ;
        out << setprecision (precision) << fixed << totEff;
        out << " (";
        out << setprecision (precision) << fixed << relEff;
        out << " %)";
        //out << "INPUT: " << count.counters.at (iSample).at(iSel+1) << " / " << count.counters.at (iSample).at(0);
        out << " |" ;
      }
    out << "\n" ;
  }
}


/*
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
void printTable (std::ostream& out, vector<string> & sample, vector<pair <TString, TCut> > & selections, counters & count, vector<sample> & samples, vector<vector<float>> & DataDrivenSamplesYields, unsigned int precision )
{
  unsigned int NSpacesColumns = 0;
  unsigned int NSpacesColZero = 0;

  // compute required spacings
  for (int i = 0; i < selections.size(); i++)
  {
    string s (selections.at(i).first.Data());
    if (NSpacesColZero < s.size()) NSpacesColumns = s.size();
  }
  NSpacesColZero += 2; // extra space from seprarator

  for (int i = 0; i < sample.size(); i++)
  {
    if (NSpacesColumns < sample.at(i).size()) NSpacesColumns = 
  }
}
*/

