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
    
    AnalysisHelper ah(argv[1]);
    ah.readSelections();
    ah.readVariables();
    ah.readSamples();
    ah.prepareSamplesHistos();
    ah.dump();
    ah.fillHistos();
    // ah.
// fill
    ah.saveOutputsToFile();
    cout << "... exiting" << endl;    
}