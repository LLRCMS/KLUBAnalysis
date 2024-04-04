#include "utils.h"

//using namespace std ;


mysample::mysample (TString theSampleName, TString theSampleFileName,
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


float mysample::calcEfficiency ()
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
readSamples (std::vector<mysample> & samples, std::vector<std::string> & samplesList, TString readOption)
{
  for (unsigned int iSample = 0 ; iSample < samplesList.size () ; ++iSample)
  {
    TString sampleFolder = gConfigParser->readStringOption (
      TString ("samples::") + samplesList.at (iSample).c_str ()
      ) ;
    std::cout << "reading " << samplesList.at (iSample) << " from : " << sampleFolder << "\n" ;
    samples.push_back (mysample (samplesList.at (iSample), sampleFolder + "/total.root", readOption)) ;
    int done = samples.back ().sampleTree->GetEntries () ;
    std::cout << " --> read " << done << " events\n" ;
  }
  return 0 ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

int
readSamples (std::vector<mysample> & samples, std::vector<std::string> & samplesList, const std::unique_ptr<CfgParser> & lConfigParser, TString readOption)
{
  for (unsigned int iSample = 0 ; iSample < samplesList.size () ; ++iSample)
  {
    TString sampleBaseFolder = lConfigParser->readStringOpt((TString ("samples::base")).Data());
    TString sampleFolder = lConfigParser->readStringOpt (
      (TString ("samples::") + samplesList.at (iSample).c_str ()).Data()
      ) ;
    std::cout << "reading " << samplesList.at (iSample) << " from : " << sampleBaseFolder + sampleFolder << "\n" ;

	samples.push_back (mysample (samplesList.at (iSample), sampleBaseFolder + sampleFolder + "/total.root", readOption)) ;
    int done = samples.back ().sampleTree->GetEntries () ;
    std::cout << " --> read " << done << " events\n" ;
  }
  return 0 ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


std::vector<std::pair<TString, TCut> >
readCutsFile (std::string filename)
{
  std::ifstream infile (filename) ;
  std::vector<std::pair<TString, TCut> > selections ;

  std::string line ;
  while (getline (infile, line))
  {
    std::istringstream iss (line) ;
    std::string dummy ;
    iss >> dummy ;
    if (dummy.size () < 2) continue ;
    size_t found = dummy.find ("%") ;
    if (found != std::string::npos) continue ;
    size_t limit = line.find_first_of ("=") ;
    if (limit == std::string::npos) continue ;
//       cout << line << endl ;
//       cout << limit << endl ;
    std::string selection = line.substr (limit+1) ;
//       cout << dummy << "|" << selection << endl ;
    selections.push_back (std::pair<TString, TCut> (dummy.c_str (), selection.c_str ())) ;
  }
  return selections ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


std::vector<std::pair <TString, TCut> >
readCutsFile (std::vector<std::string> activeSelections, std::string filename)
{
  std::ifstream infile (filename) ;
  std::vector<std::pair<TString, TCut> > selections ;

  std::string line ;
  while (getline (infile, line))
  {
    std::istringstream iss (line) ;
    std::string dummy ;
    iss >> dummy ;
    if (dummy.size () < 2) continue ;
    size_t found = dummy.find ("#") ;
    if (found != std::string::npos) continue ;
    size_t limit = line.find_first_of ("=") ;
    if (limit == std::string::npos) continue ;
//       cout << line << endl ;
//       cout << limit << endl ;
    std::string selection = line.substr (limit+1) ;
//       cout << dummy << "|" << selection << endl ;
    if (find (activeSelections.begin (), activeSelections.end (), dummy)
	== activeSelections.end ())
      continue ;
    selections.push_back (std::pair<TString, TCut> (dummy.c_str (), selection.c_str ())) ;
  }
  return selections ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

std::vector<std::pair <std::string, std::string> > readVarNamesFile (std::vector<std::string> varList, std::string filename)
{
  std::ifstream infile (filename) ;
  std::vector<std::pair<std::string, std::string> > varNames ;

  std::string comments("@@@");

  std::string line ;
  while (getline (infile, line))
  {
    std::istringstream iss (line) ;
    std::string dummy ;
    iss >> dummy ;
    //cout << "PARSE:--" << dummy << endl;
    if (dummy.size () < 2) continue ;
    size_t found = dummy.find (comments) ;
    if (found != std::string::npos) continue ;
    size_t limit = line.find_first_of ("=") ;
    if (limit == std::string::npos) continue ;
    std::string name = line.substr (limit+1) ;
    if (find (varList.begin (), varList.end (), dummy)
	== varList.end ())
      continue ;
    // trim tring as whitespace interfere with ROOT positioning
    size_t first = name.find_first_not_of(std::string(" \t\f\v\n\r"));
    size_t last = name.find_last_not_of(std::string(" \t\f\v\n\r"));
    name = name.substr(first, (last-first+1));

    //name = string(boost::algorithm::trim( name )) ;
    //varNames.push_back (pair<string, string> (dummy, name) ) ;
    //cout << "!" << dummy << "! !" << name << "!" << endl;
    varNames.push_back(std::make_pair(dummy, name));
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


void addTo (std::vector<float> & total, std::vector<float> & addition)
{
  for (unsigned int i = 0 ; i < total.size () ; ++i)
    total.at (i) += addition.at (i) ;
  return ;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

void printTableTitle (std::ostream& out, std::vector<std::string> & sample, unsigned int NSpacesColZero, unsigned int NSpacesColumns)
{
  for (unsigned int i = 0 ; i < NSpacesColZero ; ++i) out << " " ;
  out << "| " ;
  for (unsigned int iSample = 0 ; iSample < sample.size () ; ++iSample)
  {
    std::string word = sample.at (iSample).substr (0, NSpacesColumns) ;
    out << word ;
    if ( NSpacesColumns < word.size () ) NSpacesColumns = word.size();
    for (unsigned int i = 0 ; i < NSpacesColumns - word.size () ; ++i) out << " " ;
    out << " | " ;
  }
  out << "\n" ;
  return ;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

void printTableTitle (std::ostream& out, std::vector<mysample> & sample, unsigned int NSpacesColZero, unsigned int NSpacesColumns)
{
  std::vector<std::string> names ;
  for (unsigned int iSample = 0 ; iSample < sample.size () ; ++iSample)
    names.push_back (sample.at (iSample).sampleName.Data ()) ;
  printTableTitle (out, names, NSpacesColZero, NSpacesColumns) ;
  return ;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

void printTableTitle (std::ostream& out, std::vector<mysample> & sample, std::vector<std::string> & DataDrivenBkgsName, unsigned int NSpacesColZero, unsigned int NSpacesColumns)
{
  std::vector<std::string> names ;
  for (unsigned int iSample = 0 ; iSample < sample.size () ; ++iSample)
    names.push_back (sample.at (iSample).sampleName.Data ()) ;

  for (unsigned int iDDSample = 0; iDDSample < DataDrivenBkgsName.size(); ++iDDSample)
    names.push_back (DataDrivenBkgsName.at(iDDSample));

  printTableTitle (out, names, NSpacesColZero, NSpacesColumns) ;
  return ;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
void printTableBody  (std::ostream& out, std::vector<std::pair <TString, TCut> > & selections, counters & count, std::vector<mysample> & samples,
                      unsigned int NSpacesColZero, unsigned int NSpacesColumns, unsigned int precision)
{
  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
  {
    out << selections.at (iSel).first ;
    if ( NSpacesColZero < std::string(selections.at (iSel).first.Data ()).size () ) NSpacesColZero = std::string(selections.at (iSel).first.Data ()).size () ; //guard if name too long
    for (unsigned int i = 0 ; i < NSpacesColZero - std::string(selections.at (iSel).first.Data ()).size () ; ++i) out << " " ;
    out << "|" ;
    for (unsigned int iSample = 0 ; iSample < samples.size () ; ++iSample)
    {
      float evtnum = count.counters.at (iSample).at (iSel+1) ;
      int subtractspace = 0 ;
      if (evtnum > 0) subtractspace = int (log10 (evtnum)) + precision + 1 ;
      for (unsigned int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) out << " " ;
      out << std::setprecision (precision) << std::fixed << evtnum
	  << " |" ;
    }
    out << "\n" ;
  }
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
void printTableBody  (std::ostream& out, std::vector<std::pair <TString, TCut> > & selections, counters & count, std::vector<mysample> & samples, std::vector<std::vector<float>> & DataDrivenSamplesYields,
                      unsigned int NSpacesColZero, unsigned int NSpacesColumns, unsigned int precision)
{
  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
  {
    out << selections.at (iSel).first ;
    if ( NSpacesColZero < std::string(selections.at (iSel).first.Data ()).size () ) NSpacesColZero = std::string(selections.at (iSel).first.Data ()).size () ; //guard if name too long
    for (unsigned int i = 0 ; i < NSpacesColZero - std::string(selections.at (iSel).first.Data ()).size () ; ++i) out << " " ;
    out << "|" ;

    for (unsigned int iSample = 0 ; iSample < samples.size () ; ++iSample)
    {
      float evtnum = count.counters.at (iSample).at (iSel+1) ;
      int subtractspace = 0 ;
      if (evtnum > 0) subtractspace = int (log10 (evtnum)) + precision + 1 ;
      for (unsigned int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) out << " " ;
      out << std::setprecision (precision) << std::fixed << evtnum
	  << " |" ;
    }
    for (unsigned int iDDSample = 0 ; iDDSample < DataDrivenSamplesYields.size () ; ++iDDSample)
    {
      float evtnum = DataDrivenSamplesYields.at(iDDSample).at(iSel) ;
      int subtractspace = 0 ;
      if (evtnum > 0) subtractspace = int (log10 (evtnum)) + precision + 1 ;
      for (unsigned int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) out << " " ;
      out << std::setprecision (precision) << std::fixed << evtnum
	  << " |" ;
    }

    out << "\n" ;
  }
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
void printTableBodyEff  (std::ostream& out, std::vector<std::pair <TString, TCut> > & selections, counters & count, std::vector<mysample> & samples,
			 unsigned int NSpacesColZero, unsigned int NSpacesColumns, unsigned int precision)
{
  for (unsigned int iSel = 0 ; iSel < selections.size () ; ++iSel)
  {
    out << selections.at (iSel).first ;
    if ( NSpacesColZero < std::string(selections.at (iSel).first.Data ()).size () ) NSpacesColZero = std::string(selections.at (iSel).first.Data ()).size () ; //guard if name too long
    for (unsigned int i = 0 ; i < NSpacesColZero - std::string(selections.at (iSel).first.Data ()).size () ; ++i) out << " " ;
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

      //int subtractspace = 0 ;
      //if (evtnum > 0) subtractspace = int (log10 (evtnum)) + precision + 1 ;
      //for (unsigned int i = 0 ; i < NSpacesColumns - subtractspace ; ++i) out << " " ;
      out << std::setprecision (precision) << std::fixed << totEff;
      out << " (";
      out << std::setprecision (precision) << std::fixed << relEff;
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
