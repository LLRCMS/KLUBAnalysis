#include "TauIDSFTool.h"
#include <iostream> // std::cerr, std::endl
#include <iomanip>
#include <assert.h> // assert

TFile* TauIDSFTool::ensureTFile(const TString filename){
  if(mVerbose)
    std::cout << "Opening " << filename << std::endl;
  TFile* file = new TFile(filename);
  if(!file or file->IsZombie()) {
    std::cerr << std::endl << "ERROR! Failed to open input file = '" << filename << "'!" << std::endl;
    assert(0);
  }
  return file;
}

TH1* TauIDSFTool::extractTH1(const TFile* file, const std::string& histname){
  TH1* hist = dynamic_cast<TH1*>((const_cast<TFile*>(file))->Get(histname.data()));
  if(!hist){
    std::cerr << std::endl << "ERROR! Failed to load histogram = '" << histname << "' from input file!" << std::endl;
    assert(0);
  }
  return hist;
}

const TF1* TauIDSFTool::extractTF1(const TFile* file, const std::string& funcname){
  const TF1* function = dynamic_cast<TF1*>((const_cast<TFile*>(file))->Get(funcname.data()));
  if(!function){
    std::cerr << std::endl << "ERROR! Failed to load function = '" << funcname << "' from input file!" << std::endl;
    assert(0);
  }
  return function;
}

const TGraphAsymmErrors* TauIDSFTool::extractTGAE(const TFile* file, const std::string& gname){
  const TGraphAsymmErrors* graph = dynamic_cast<TGraphAsymmErrors*>((const_cast<TFile*>(file))->Get(gname.data()));
  if(!graph){
    std::cerr << std::endl << "ERROR! Failed to load graph = '" << gname << "' from input file!" << std::endl;
    assert(0);
  }
  return graph;
}

void TauIDSFTool::disabled() const {
  std::cerr << std::endl << "ERROR! Method has been disabled! isVsPT = "<<isVsPT<<", isVsDM = "
            << isVsDM<<", isVsEta = "<<isVsEta<< std::endl;
  assert(0);
}


TauIDSFTool::TauIDSFTool(const std::string& year, const std::string& id, const std::string& wp,
						 const bool dm, const bool embedding)
{
  yearCheck(year);

  // vsJets
  if(std::find(mAntiJetIDs.begin(), mAntiJetIDs.end(), id) != mAntiJetIDs.end())
	{
	  if(dm)
		{
		  TString filename;
		  if (embedding) {
			embeddedDMcheck(id);
			filename = Form("%s/TauID_SF_dm_%s_%s_EMB.root", mDatapath.data(), id.data(), year.data());
		  }
		  else {
			filename = Form("%s/TauID_SF_dm_%s_%s.root", mDatapath.data(), id.data(), year.data());
		  }

		  TFile* file = ensureTFile(filename);
		  hist = extractTH1(file, wp);
		  hist->SetDirectory(nullptr);
		  file->Close();
		  delete file;
		  if (id.find("oldDM") == std::string::npos) {
			mDMs.insert(11);
		  }
		  isVsDM = true;
		}
	  else
		{
		  TString filename;
		  if (embedding) {
			embeddedDMcheck(id);
			filename = Form("%s/TauID_SF_pt_%s_%s_EMB.root", mDatapath.data(), id.data(), year.data());
		  }
		  else {
			filename = Form("%s/TauID_SF_pt_%s_%s.root", mDatapath.data(), id.data(), year.data());
		  }
		  TFile* file = ensureTFile(filename);
		  func[""]     = extractTF1(file,Form("%s_cent", wp.data()));
		  func["Up"]   = extractTF1(file,Form("%s_up",   wp.data()));
		  func["Down"] = extractTF1(file,Form("%s_down", wp.data()));
		  file->Close();								 
		  delete file;
		  isVsPT = true;
		}
	}

  // vsElectrons
  else if(std::find(mAntiEleIDs.begin(), mAntiEleIDs.end(), id) != mAntiEleIDs.end())
	{
	  if (embedding) {
		std::cerr << "SF for ID " << id << " not available for the embedded samples!" << std::endl;
		assert(0);
	  }
	  TString filename = Form("%s/TauID_SF_eta_%s_%s.root", mDatapath.data(), id.data(), year.data());
	  TFile* file = ensureTFile(filename);
	  hist = extractTH1(file, wp);
	  hist->SetDirectory(nullptr);
	  file->Close();
	  delete file;
	  mGenmatches = {1,3};
	  isVsEta    = true;
	}

  // vsMuons
  else if(std::find(mAntiMuIDs.begin(), mAntiMuIDs.end(), id) != mAntiMuIDs.end())
	{
	  if (embedding) {
		std::cerr << "SF for ID " << id << " not available for the embedded samples!" << std::endl;
		assert(0);
	  }
	  TString filename = Form("%s/TauID_SF_eta_%s_%s.root", mDatapath.data(), id.data(), year.data());
	  TFile* file = ensureTFile(filename);
	  hist = extractTH1(file, wp);
	  hist->SetDirectory(nullptr);
	  file->Close();
	  delete file;
	  mGenmatches = {2,4};
	  isVsEta    = true;
	}

  else {
    std::cerr << "Did not recognize tau ID '" << id << "'!" << std::endl;
    assert(0);
  }
}

// used for updated vsJet UL SFs
TauIDSFTool::TauIDSFTool(std::string year, const std::string& wp1, const std::string& wp2)
{
  yearCheck(year);
  if (year.find("UL") == std::string::npos) {
	std::cerr << "Only Ultra Legacy is supported for updated vsJet SFs." << std::endl;
    assert(0);
  }
  year.erase(0,2); //delete the "UL" substring for correct naming scheme
  
  if(wp2 != "Tight" and wp2 != "VVLoose") {
    std::cerr << "The latest tau ID vsJet scale factors do not support the '" <<
	  wp2 << "' working point for electrons." << std::endl;
    assert(0);	
  }
  isVsDMandPt = true;

  TString fname = Form("%s/TauID_SF_dm_DeepTau2017v2p1VSjet_VSjet%s_VSele%s_Mar07.root",
					   mDatapath.data(), wp1.data(), wp2.data());
  TFile* fl = ensureTFile(fname);

  TString fname_highpt = Form("%s/TauID_SF_Highpt_DeepTau2017v2p1VSjet_VSjet%s_VSele%s_Mar07.root",
								 mDatapath.data(), wp1.data(), wp2.data());
  TFile* fl_highpt = ensureTFile(fname_highpt);

  TString fname_extrap = Form("%s/TauID_SF_HighptExtrap_DeepTau2017v2p1VSjet_Mar07.root",
								 mDatapath.data());
  TFile* fl_extrap = ensureTFile(fname_extrap);

  mDMs.insert(11);
  std::string key, dm;
  for(auto x : mDMs) {
	key = "DM" + std::to_string(x);
	dm  = "dm" + std::to_string(x);
	func[key] = extractTF1(fl, Form("%s_%s_fit", key.data(), year.data()));

	// statistical ucnertainties of linear fit
	func[key + "Stat0Up"]   = extractTF1(fl, Form("%s_%s_fit_uncert0_up", key.data(), year.data()));
	func[key + "Stat0Down"] = extractTF1(fl, Form("%s_%s_fit_uncert0_down", key.data(), year.data()));
	func[key + "Stat1Up"]	= extractTF1(fl, Form("%s_%s_fit_uncert1_up", key.data(), year.data()));
	func[key + "Stat1Down"] = extractTF1(fl, Form("%s_%s_fit_uncert1_down", key.data(), year.data()));

	// systematic uncertainties correlated across DMs and eras
	func[key + "SystCorrDMErasUp"]   = extractTF1(fl, Form("%s_%s_syst_alleras_up_fit", key.data(), year.data()));
	func[key + "SystCorrDMErasDown"] = extractTF1(fl, Form("%s_%s_syst_alleras_down_fit", key.data(), year.data()));

	// systematic uncertainties correlated across DMs but uncorrelated by eras
	func[key + "SystCorrDMUncorrErasUp"]   = extractTF1(fl, Form("%s_%s_syst_%s_up_fit", key.data(), year.data(), year.data()));
	func[key + "SystCorrDMUncorrErasDown"] = extractTF1(fl, Form("%s_%s_syst_%s_down_fit", key.data(), year.data(), year.data()));

	// systematic uncertainties uncorrelated across DMs and eras
	func[key + "SystUncorrDMErasUp"]   = extractTF1(fl, Form("%s_%s_syst_%s_%s_up_fit", key.data(), year.data(), dm.data(), year.data()));
	func[key + "SystUncorrDMErasDown"] = extractTF1(fl, Form("%s_%s_syst_%s_%s_down_fit", key.data(), year.data(), dm.data(), year.data()));
  }

  fl->Close();
  delete fl;

  // for pTs higher than 140GeV
  graph_highpt["Gt140"]				= extractTGAE(fl_highpt, Form("DMinclusive_%s", year.data()));
  graph_highpt["Gt140SystCorrEras"]	= extractTGAE(fl_highpt, Form("DMinclusive_%s_syst_alleras", year.data()));
  graph_highpt["Gt140Stat"]			= extractTGAE(fl_highpt, Form("DMinclusive_%s_statandsyst_%s", year.data(), year.data()));
  
  fl_highpt->Close();
  delete fl_highpt;

  func_extrap = extractTF1(fl_extrap, Form("uncert_func_%sVSjet_%sVSe", wp1.data(), wp2.data()));
  fl_extrap->Close();
  delete fl_extrap;
}

void TauIDSFTool::embeddedDMcheck(const std::string& ID)
{
  if (ID.find("oldDM") != std::string::npos)
	{
	  std::cerr << "Scale factors for embedded samples are not provided for the MVA IDs." << std::endl;
	  assert(0);
	}
}

float TauIDSFTool::getSFvsDMandPT(double pt, int dm, int genmatch, const std::string& unc)
{
  if(!isVsDMandPt) disabled();
  if(genmatch!=5) {
	return 1.f;
  }
  uncertaintyCheck(unc);
  
  float SF = 1.f;
  if(unc=="") { //nominal value
	if(pt>140.) {
	  int ipt = pt > 200. ? 1 : 0;
	  SF = static_cast<float>(graph_highpt["Gt140"]->GetPointY(ipt));
	}
	else {
	  std::string key = "DM" + std::to_string(dm);
	  SF = static_cast<float>(func[key]->Eval(pt));
	}
  }
  else if (unc.find("Gt140") != std::string::npos) { // high pT uncertainty
	if(pt > 140.) {
	  int ipt = pt > 200. ? 1 : 0;
	  if(unc=="Gt140StatUp") {
		SF = static_cast<float>(graph_highpt["Gt140Stat"]->GetErrorYhigh(ipt));
	  }
	  else if(unc=="Gt140StatDown") {
		SF = static_cast<float>(graph_highpt["Gt140Stat"]->GetErrorYlow(ipt));
	  }
	  else if(unc=="Gt140SystCorrErasUp") {
		SF = static_cast<float>(graph_highpt["Gt140SystCorrEras"]->GetErrorYhigh(ipt));
	  }
	  else if(unc=="Gt140SystCorrErasDown") {
		SF = static_cast<float>(graph_highpt["Gt140SystCorrEras"]->GetErrorYlow(ipt));
	  }
	  else if(unc=="Gt140Extrap" and pt>300.) {
		SF = static_cast<float>(func_extrap->Eval(pt));
	  }
	}
  }
  else { // low pT uncertainty
	if(pt < 140.) {
	  std::string key = "DM" + std::to_string(dm);
	  SF = static_cast<float>(func[key + unc]->Eval(pt));
	}
  }
  return SF;
}

float TauIDSFTool::getSFvsPT(double pt, int genmatch, const std::string& unc)
{
  if(!isVsPT) disabled();
  float SF = 1.f;
  if(genmatch==5) {
    SF = static_cast<float>(func[unc]->Eval(pt));
  }
  return SF;
}

float TauIDSFTool::getSFvsPT(double pt, const std::string& unc)
{
  return getSFvsPT(pt,5,unc);
}

float TauIDSFTool::getSFvsDM(double pt, int dm, int genmatch, const std::string& unc) const
{
  if(!isVsDM) disabled();
  if(std::find(mDMs.begin(), mDMs.end(), dm) != mDMs.end() or pt<=40)
	{
	  if(genmatch==5) {
		Int_t bin = hist->GetXaxis()->FindBin(dm);
		float SF  = static_cast<float>(hist->GetBinContent(bin));
		if(unc=="Up")
		  SF += hist->GetBinError(bin);
		else if(unc=="Down")
		  SF -= hist->GetBinError(bin);
		return SF;
	  }
	  return 1.0;
	}
  return 0.0;
}

float TauIDSFTool::getSFvsDM(double pt, int dm, const std::string& unc) const{
  return getSFvsDM(pt,dm,5,unc);
}

float TauIDSFTool::getSFvsEta(double eta, int genmatch, const std::string& unc) const{
  if(!isVsEta) disabled();
  if(std::find(mGenmatches.begin(), mGenmatches.end(), genmatch) != mGenmatches.end())
	{
	  Int_t bin = hist->GetXaxis()->FindBin(eta);
	  float SF  = static_cast<float>(hist->GetBinContent(bin));
	  if(unc=="Up")
		SF += hist->GetBinError(bin);
	  else if(unc=="Down")
		SF -= hist->GetBinError(bin);
	  return SF;
	}
  return 1.0;
}

void TauIDSFTool::uncertaintyCheck(const std::string& unc) const
{
  if(std::find(mUncertainties.begin(), mUncertainties.end(), unc) == mUncertainties.end())
	{
	  std::cerr << std::endl << "ERROR! '" << unc << "' is not a valid uncertainty! Please choose from ";
	  for(auto it=mUncertainties.begin(); it!=mUncertainties.end(); it++) {
		if(it!=mUncertainties.begin()) std::cerr << ", ";
		std::cerr << *it;
	  }
	  std::cerr << std::endl;
	  assert(0);
	}
}

void TauIDSFTool::yearCheck(const std::string& year) const
{
  if(std::find(mYears.begin(), mYears.end(), year) == mYears.end())
	{
	  std::cerr << std::endl << "ERROR! '" << year << "' is not a valid year! Please choose from ";
	  for(auto it=mYears.begin(); it!=mYears.end(); it++) {
		if(it!=mYears.begin()) std::cerr << ", ";
		std::cerr << *it;
	  }
	  std::cerr << std::endl;
	  assert(0);
	}
}
