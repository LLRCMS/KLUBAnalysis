#include "Sample.h"
#include <fstream>
#include <assert.h>
#include <regex>

using namespace std;

Sample::Sample(string name, string filelistname, string treename, string histoname, int binEffDen):
  name_(name), eff_(0.), evt_num_(0.), evt_den_(0.), nentries_(0.), treename_(treename), histoname_(histoname), bin_eff_den_(binEffDen)
{
  tree_ = new TChain (treename.c_str());
  filelistname_.push_back(filelistname);
}

Sample::Sample(string name, std::vector<std::string> filelistname, string treename, string histoname, int binEffDen):
  name_(name), eff_(0.), evt_num_(0.), evt_den_(0.), nentries_(0.), bin_eff_den_(binEffDen), treename_(treename), histoname_(histoname)
{
  tree_ = new TChain (treename.c_str());
  filelistname_ = filelistname;
}

Sample::~Sample ()
{
  // delete tree_;
}

bool Sample::openFileAndTree()
{
  int counter = 0;
  std::cout << "@ Opening sample(s): " << name_ << std::endl;
  for(std::string fn : filelistname_)
	{
	  std::cout << "  ---> " << fn << std::endl;

	  ifstream fList(fn);
	  if (!fList.good())
		{
		  std::regex r1(".+goodfiles_resub\\d\\.txt$");
		  std::regex r2(".+goodfiles\\.txt$");
		  std::smatch m;
		  if(std::regex_match(fn, m, r1) or std::regex_match(fn, m, r2)) {
			std::string mes = "*** Sample::openFileAndTree : WARNING: File " + fn + " was not found.";
			cerr << mes << endl;
			continue;
		  }
		  else {
			cerr << "*** Sample::openFileAndTree : ERROR : could not open file " << fn << endl;
			return false;
		  }
		}
	  string line;
	  while (std::getline(fList, line))
		{
		  line = line.substr(0, line.find("#", 0)); // remove comments introduced by #
		  while (line.find(" ") != std::string::npos)
			line = line.erase(line.find(" "), 1); // remove white spaces
		  while (line.find("\n") != std::string::npos)
			line = line.erase(line.find("\n"), 1); // remove new line characters
		  while (line.find("\r") != std::string::npos)
			line = line.erase(line.find("\r"), 1); // remove carriage return characters

		  if (!line.empty()) // skip empty lines
			{
			  ++counter;
			  tree_->Add(line.c_str());

			  TFile* f = new TFile (line.c_str());
			  if(f->IsZombie())
				throw std::runtime_error("File " + line + " has issues!");

			  TH1F* h = (TH1F*) f->Get(histoname_.c_str());
			  if(!h)
				throw std::runtime_error("Histogram " + histoname_ + " not found!");

			  evt_num_  += h->GetBinContent(2) ;
			  evt_den_  += h->GetBinContent(bin_eff_den_) ;
			  nentries_ += h->GetBinContent(4) ; // NB! rounding errors could make this different from the actual entries in the tree --> better to use TH1D
			  delete h;
			  delete f;
			}
		}
	}
  eff_ = evt_num_ / evt_den_ ;
  std::cout << "  ---> read " << counter << " files, " << nentries_ << " events" << std::endl;
  std::cout << "  ---> efficiency is " << eff_ << "(" << evt_num_ << "/" << evt_den_ << ")" << std::endl;

  return true;
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
