#include "Sample.h"
#include <fstream>
#include <assert.h>
#include <regex>

Sample::Sample(std::string name, std::vector<std::string> filelistname,
			   std::string skimname, std::string histoname, int binEffDen):
  name_(name), eff_(0.), evt_num_(0.), evt_den_(0.), nentries_(0.), bin_eff_den_(binEffDen), skimname_(skimname), histoname_(histoname)
{
  skim_ = new TChain(skimname.c_str());
  filelistname_ = filelistname;
}

// Constructor that handles two chains: the nominal one from the skimming step and the one with the evaluation of the parameterised DNN.
// The chains are first filled indepedently and later the evaluation chain is added as friend of the nominal chain.
Sample::Sample(std::string name, std::vector<std::string> filelistname,
			   std::string skimname, std::string evalname,
			   std::string skimpath, std::string evalpath,
			   std::string histoname, int binEffDen):
  name_(name), eff_(0.), evt_num_(0.), evt_den_(0.), nentries_(0.), bin_eff_den_(binEffDen), skimname_(skimname), evalname_(evalname), skimpath_(skimpath), evalpath_(evalpath), histoname_(histoname)
{
  friend_added_ = false;
  skim_ = new TChain(skimname.c_str());
  eval_ = new TChain(evalname.c_str());
  filelistname_ = filelistname;
}

std::map<std::string, std::string> Sample::getBranches(bool force)
{
  if (eval_ and !friend_added_ and !force) {
	throw std::runtime_error("You can only call getBranches() after adding the evaluation tree as a friend.");
  }

  TObjArray* skim_branches = skim_->GetListOfBranches();
  unsigned nbr_skim = skim_->GetNbranches();
  std::map<std::string, std::string> branches;
  for (unsigned iB=0; iB < nbr_skim; ++iB)	{
	branches[ skim_branches->At(iB)->GetName() ] = skim_branches->At(iB)->GetTitle();
  }
  if (eval_) {
	TObjArray* eval_branches = eval_->GetListOfBranches();
	unsigned nbr_eval = eval_->GetNbranches();
	for (unsigned iB=0; iB < nbr_eval; ++iB)	{
	  branches[ eval_branches->At(iB)->GetName() ] = eval_branches->At(iB)->GetTitle();
	}
  }

  return branches;
}

unsigned Sample::getNBranches(bool force)
{
  if (eval_ and !friend_added_ and !force) {
	throw std::runtime_error("You can only call getBranches() after adding the evaluation tree as a friend.");
  }

  unsigned nbranches = skim_->GetNbranches();
  if (eval_) {
	nbranches += eval_->GetNbranches();
  }
  return nbranches;
}

bool Sample::openFileAndTree()
{
  int counter = 0;
  std::cout << "@ Opening sample(s): " << name_ << std::endl;
  for(std::string fn : filelistname_)
	{
	  std::cout << "  ---> " << fn << std::endl;
	  std::ifstream fList(fn);
	  if (!fList.good())
		{
		  std::regex r1(".+goodfiles_resub\\d\\.txt$");
		  std::regex r2(".+goodfiles\\.txt$");
		  std::smatch m;
		  if(std::regex_match(fn, m, r1) or std::regex_match(fn, m, r2)) {
			std::string mes = "*** Sample::openFileAndTree : WARNING: File " + fn + " was not found.";
			std::cerr << mes << std::endl;
			continue;
		  }
		  else {
			std::cerr << "*** Sample::openFileAndTree : ERROR : could not open file " << fn << std::endl;
			return false;
		  }
		}
	  std::string line, line_eval = "";
	  while (std::getline(fList, line))
		{
		  line = line.substr(0, line.find("#", 0)); // remove comments introduced by #
		  while (line.find(" ") != std::string::npos) {
			line = line.erase(line.find(" "), 1); // remove white spaces
		  }
		  while (line.find("\n") != std::string::npos) {
			line = line.erase(line.find("\n"), 1); // remove new line characters
		  }
		  while (line.find("\r") != std::string::npos) {
			line = line.erase(line.find("\r"), 1); // remove carriage return characters
		  }

		  if (!line.empty()) // skip empty lines
			{
			  ++counter;
			  skim_->Add(line.c_str());
			  if (eval_) {
				line_eval = std::regex_replace(line, std::regex(skimpath_), evalpath_);
				eval_->Add(line_eval.c_str());
			  }
			  
			  TFile* f = new TFile (line.c_str());
			  if(f->IsZombie()) {
				throw std::runtime_error("File " + line + " has issues!");
			  }

			  TH1F* h = (TH1F*) f->Get(histoname_.c_str());
			  if(!h) {
				throw std::runtime_error("Histogram " + histoname_ + " not found in file " + line + "!");
			  }

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

  if (eval_) {
	friend_added_ = true;

	// Error in <TChainIndex::TChainIndex>: The indices in files of this chain aren't sorted
	// eval_->BuildIndex("EventNumber", "RunNumber"); 
	TTreeIndex *index = new TTreeIndex(eval_, "EventNumber", "RunNumber");
	eval_->SetTreeIndex(index);

	skim_->AddFriend(eval_);
  }
  
  return true;
}
 
void Sample::scaleAll(double scale)
{
  // 1D
  for (uint isel = 0; isel < plots_.size(); ++isel) {
    for (uint ivar = 0; ivar < plots_.at(isel).size(); ++ivar) {
      for (uint isyst = 0; isyst < plots_.at(isel).at(ivar).size(); ++isyst) {
		plots_.at(isel).at(ivar).at(isyst)->Scale(scale);
      }
    }
  }
  
  // 2D
  for (uint isel = 0; isel < plots2D_.size(); ++isel) {
	for (uint ivar = 0; ivar < plots2D_.at(isel).size(); ++ivar) {
	  for (uint isyst = 0; isyst < plots2D_.at(isel).at(ivar).size(); ++isyst) {
		plots2D_.at(isel).at(ivar).at(isyst)->Scale(scale);
	  }
	}
  }
}
