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
  string outFolder = "/data_CMS/cms/alves/HHresonant_hist/HistoFiller_CHANNEL_UL18_DATE";
  if (argc==4 or argc==5)
  {
    split = true;
    idx = atoi(argv[2]);
    njobs = atoi(argv[3]);
  }

  if (argc==5)
    outFolder = argv[4];

  std::array<std::string, 2> merge_sections = {{"merge_plots", "merge_limits"}};

  for (auto ms: merge_sections)
	{
	  AnalysisHelper ah(argv[1], ms);

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

	  cout << "... saving outputs under " << outFolder << endl;
	  ah.saveOutputsToFile(outFolder);
	}
  cout << "... exiting" << endl;
}
