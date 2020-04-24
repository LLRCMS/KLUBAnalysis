// NOTE: I tried to write this macro in C, but it is as fast as the python one (limiting factor ids xrootd opening)
// so let's use the python one that is nicer

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "TFile.h"
#include <stdlib.h>
using namespace std;
int main(int argc, char** argv)
{   
    if (argc < 2)
    {
        cout << ".. no input list set" << endl;
        return 1;
    }

    string infilename = argv[1];

    vector<string> corr;
    vector<string> good;

    cout << "=== opening file === " << infilename << endl;
    std::ifstream infile(infilename);
    std::string line;
    int idx = 0;
    while (std::getline(infile, line))
    {
        if (idx % 1000 == 0)
            cout << "file num: " << idx << endl;

        line = line.substr(0, line.find("#", 0)); // remove comments introduced by #
        //cout << line << endl;
        while (line.find(" ") != std::string::npos) line = line.erase(line.find(" "), 1); // remove white spaces
        while (line.find("\n") != std::string::npos) line = line.erase(line.find("\n"), 1); // remove new line characters
        while (line.find("\r") != std::string::npos) line = line.erase(line.find("\r"), 1); // remove carriage return characters
        if (!line.empty()) // skip empty lines
        {
            idx += 1;
            TFile* rfile = TFile::Open(line.c_str());
            
            if (!rfile)
                corr.push_back(line);
            else if (rfile->IsZombie())
                corr.push_back(line);
            else if (rfile->TestBit(TFile::kRecovered))
                corr.push_back(line);
            else
                good.push_back(line);

            if (rfile)
                rfile->Close();
        
            // delete rfile
        }
    }

    infile.close();

    if (corr.size() == 0)
    {
        cout << "... all ok for" << infilename << endl;
        return 0;
    }

    cout << "... ** I found " << corr.size() << "files (" << 1.*corr.size()/(good.size()+corr.size()) << ") corrupted, making new list" << endl;

    string newname = (infilename + string("_WITHCORRUPTED"));

    string command = "mv ";
    command += infilename;
    command += " ";
    command += newname;
    system(command.c_str());

    std::ofstream ofile(infilename);
    for (string x : good)
        ofile << x << endl;
    ofile.close();
}
