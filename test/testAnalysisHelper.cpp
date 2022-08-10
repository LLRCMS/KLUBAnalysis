#include "AnalysisHelper.h"
#include <iostream>
#include <string>

using namespace std;

// c++ -lm -o test testAnalysisHelper.cpp AnalysisHelper.cc CfgParser.cc Sample.cc Selection.cc Weight.cc TTreeFormulaGroup.cc --std=c++11 `root-config --glibs --cflags` -lTreePlayer
int main(int argc, char** argv)
{
  if (argc < 2)
  {
    cout << "No config set" << endl;
    return 0;
  }

  bool split = false;
  int idx = 0;
  int njobs = 1;
  if (argc >= 4)
  {
    split = true;
    idx = atoi(argv[2]);
    njobs = atoi(argv[3]);
  }

  AnalysisHelper ah(argv[1]);
  if (split) ah.setSplitting(idx, njobs);
  ah.readSelections();
  ah.readVariables();
  try {ah.readSamples();}
  catch (std::exception &ex)
  {
    cerr << "*** Error in reading samples because: " << ex.what() << endl;
    return 1;
  }

  // ah.prepareSamplesHistos();
  // ah.prepareSamples2DHistos();
  ah.prepareHistos();
  ah.dump(2); // can set a level of detail
  ah.fillHistos();
  ah.mergeSamples(); // do it just at the end
  ah.saveOutputsToFile();
  cout << "... exiting" << endl;
}
