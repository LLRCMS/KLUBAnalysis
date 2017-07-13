#include "Sample.h"
#include <fstream>
#include <assert.h>

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
    // tree_ = new TChain (treename.c_str());
    tree_ = unique_ptr<TChain>(new TChain(treename.c_str()));
    filelistname_ = filelistname;
    eff_         = 0.;
    evt_num_     = 0.;
    evt_den_     = 0.;
    nentries_    = 0.;
    bin_eff_den_ = binEffDen;
    treename_    = treename;
    histoname_   = histoname;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Sample::~Sample ()
{
    // delete tree_;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Sample::Sample(std::string name, std::vector<Sample const *> sampleList)
// {
//     name_ = name;
    
//     // all the following are set to dummy values
//     tree_ = nullptr;
//     filelistname_ = "";
//     eff_         = 0.;
//     evt_num_     = 0.;
//     evt_den_     = 0.;
//     nentries_    = 0.;
//     bin_eff_den_ = -1;
//     treename_    = "";
//     histoname_   = "";    

//     if (sampleList.size() == 0)
//     {
//         cerr << "*** Sample::Sample (ctor from list of samples) : ERROR : the input list is empty, will create and empty sample" << endl;
//         return;
//     }

//     cout << "@ Creating sample " << name_ << " summing the following samples" << endl;    
//     for (auto s : sampleList)
//         cout << "  ---> " << s->getName() << endl;

//     // clone the structure and histos from sample number 1
// }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool Sample::openFileAndTree()
{
    cout << "@ Opening sample: " << name_ << endl;
    cout << "  ---> " << filelistname_ << endl;
    
    ifstream fList (filelistname_);
    if (!fList.good())
    {
        cerr << "*** Sample::openFileAndTree : ERROR : could not open file " << filelistname_ << endl;
        return false;
        // cerr << " ===> Terminating execution" << endl;
        // assert(0);
    }
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
            TH1F* h = (TH1F*) f->Get(histoname_.c_str());
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

    return true;
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

void Sample::scaleAll(double scale)
{
    // 1D
    for (uint isel = 0; isel < plots_.size(); ++isel)
    {
        // cout << "isel " << isel << "/" << plots_.size() << endl;
        for (uint ivar = 0; ivar < plots_.at(isel).size(); ++ivar)
        {
            // cout << "ivar " << ivar << "/" << plots_.at(isel).size() << endl;
            for (uint isyst = 0; isyst < plots_.at(isel).at(ivar).size(); ++isyst)
            {
                // cout << "isyst " << isyst << "/" << plots_.at(isel).at(ivar).size() << endl;
                // cout << " >>>>> : >>>>> scaling histo " << plots_.at(isel).at(ivar).at(isyst)->GetName() << " integral = " << plots_.at(isel).at(ivar).at(isyst)->Integral() << " by " << scale << endl;
                plots_.at(isel).at(ivar).at(isyst)->Scale(scale);
                // cout << "DONE" << endl;
            }
        }
    }

    // 2D
    for (uint isel = 0; isel < plots2D_.size(); ++isel)
    {
        // cout << "isel " << isel << "/" << plots_.size() << endl;
        for (uint ivar = 0; ivar < plots2D_.at(isel).size(); ++ivar)
        {
            // cout << "ivar " << ivar << "/" << plots_.at(isel).size() << endl;
            for (uint isyst = 0; isyst < plots2D_.at(isel).at(ivar).size(); ++isyst)
            {
                // cout << "isyst " << isyst << "/" << plots_.at(isel).at(ivar).size() << endl;
                plots2D_.at(isel).at(ivar).at(isyst)->Scale(scale);
                // cout << "DONE" << endl;
            }
        }
    }
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
