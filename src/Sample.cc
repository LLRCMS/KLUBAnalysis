#include "Sample.h"
#include <fstream>

using namespace std;

// Sample::Sample (string name, string treename)
// {
//     name_ = name;
//     tree_ = new TChain (treename);
//     filelistname_ = "";
//     tree_ = 0;
//     eff_  = 0.;
//     evt_num_ = 0.;
//     evt_den_ = 0.;
//     nentries_ = 0.;
// }

Sample::Sample (string name, string filelistname, string treename, string histoname, int binEffDen)
// Sample (name, treename)
{
    name_ = name;
    tree_ = new TChain (treename.c_str());
    filelistname_ = filelistname;
    eff_  = 0.;
    evt_num_ = 0.;
    evt_den_ = 0.;
    nentries_ = 0.;
    bin_eff_den_ = binEffDen;
    openFileAndTree(treename, histoname);
    // getEfficiency(histoname);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Sample::~Sample ()
{
    delete tree_;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Sample::openFileAndTree(string treename, string histoname)
{
    cout << "@ Opening sample: " << name_ << endl;
    cout << "  ---> " << filelistname_ << endl;
    
    ifstream fList (filelistname_);
    string line;
    int counter = 0;
    while (std::getline(fList, line))
    {
        line = line.substr(0, line.find("#", 0)); // remove comments introduced by #
        //cout << line << endl;
        while (line.find(" ") != std::string::npos) line = line.erase(line.find(" "), 1); // remove white spaces
        while (line.find("\n") != std::string::npos) line = line.erase(line.find("\n"), 1); // remove new line characters
        while (line.find("\r") != std::string::npos) line = line.erase(line.find("\r"), 1); // remove carriage return characters
        if (!line.empty()) // skip empty lines
        {
            ++counter;
            tree_->Add(line.c_str());
            
            TFile* f = new TFile (line.c_str());
            TH1F* h = (TH1F*) f->Get(histoname.c_str());
            evt_num_  += h->GetBinContent (2) ;
            evt_den_  += h->GetBinContent (bin_eff_den_) ;
            nentries_ += h->GetBinContent (4) ; // NB! rounding errors could make this different from the actual entries in the tree --> better to use TH1D
            delete h;
            delete f;
        }
    }
    eff_ = evt_num_ / evt_den_ ;
    cout << "  ---> read " << counter << " files, " << nentries_ << " events" << endl;
    cout << "  ---> efficiency is " << eff_ << "(" << evt_num_ << "/" << evt_den_ << ")" << endl;

    // fIn_ = TFile::Open(filename.c_str());
    
    // if (!fIn_)
    // {
    //     cerr << "*** Sample: error: could not open file " << filename << endl;
    //     return;
    // }
    // if (!fIn_->GetListOfKeys()->Contains(treename.c_str()))
    // {
    //     cerr << "*** Sample: error: file does not contain tree " << treename << endl;
    //     return;
    // }
    
    // tree_ = (TTree*) fIn_->Get(treename.c_str());
    // nentries_ = tree_->GetEntries();
    // cout << "  ---> success, tree contains " << nentries_ << " entries" << endl;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// void Sample::getEfficiency(string histoname)
// {
//     TH1F * effHisto = (TH1F *) fIn_->Get (histoname.c_str()) ;
//     if (effHisto->GetBinContent (1) == 0) 
//     {
//         evt_num_ = 0. ;
//         evt_den_ = 0. ;
//         eff_ = 0. ;
//         return;
//     }

//     evt_num_ = effHisto->GetBinContent (2) ;
//     evt_den_ = effHisto->GetBinContent (1) ;
//     eff_ = (float) evt_num_ / (float) evt_den_ ;
// }
