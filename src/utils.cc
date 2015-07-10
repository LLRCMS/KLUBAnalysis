#include "utils.h"


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

