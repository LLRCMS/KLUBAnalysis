#include "AnalysisHelper.h"
#include <iostream>
#include <string>

// c++ -lm -o test testAnalysisHelper.cpp AnalysisHelper.cc CfgParser.cc Sample.cc Selection.cc Weight.cc TTreeFormulaGroup.cc --std=c++11 `root-config --glibs --cflags` -lTreePlayer
int main(int argc, char** argv)
{
  if (argc != 5) {
	std::cout << "[ERROR::testAnalysisHelper] Expecting five input arguments." << std::endl;
    return 1;
  }

  int idx    = atoi(argv[2]);
  int njobs  = atoi(argv[3]);
  std::string outFolder = argv[4];

  std::array<std::string, 2> merge_sections = {{"merge_plots", "merge_limits"}};

  for (auto ms: merge_sections)
	{
	  AnalysisHelper ah(argv[1], ms, idx, njobs);
	  ah.readSelections();
	  ah.readVariables();

	  ah.readSamples();
	  
	  ah.prepareHistos();
	  ah.dump(2); // can set a level of detail
	  ah.fillHistos();
	  ah.mergeSamples(); // do it just at the end

	  std::cout << "[INFO::testAnalysisHelper] Saving outputs under " << outFolder << std::endl;
	  ah.saveOutputsToFile(outFolder);
	}
  std::cout << "[INFO::testAnalysisHelper] Exiting." << std::endl;
  return 0;
}
