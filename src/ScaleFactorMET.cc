// Custom MET scale factors derived with https://github.com/bfonta/inclusion
// Contact: bruno.alves@cern.ch
#include "ScaleFactorMET.h"

ScaleFactorMET::ScaleFactorMET()
{
  const std::string fname = "eff_Data_Mu_MC_TT_DY_mutau_metnomu_et_TRG_METNoMu120_CUTS_mhtnomu_et_L_100_default_";
  const std::string name16pre  = "weights/trigger_SF_UL/2016preVFP/";
  const std::string name16post = "weights/trigger_SF_UL/2016postVFP/";
  const std::string name17     = "weights/trigger_SF_UL/2017/";
  const std::string name18     = "weights/trigger_SF_UL/2018/";
  inputs = {
	{"2016preVFP",  {{"etau",   name16pre+fname+"etau.root"},
					 {"mutau",  name16pre+fname+"mutau.root"},
					 {"tautau", name16pre+fname+"tautau.root"}}},
	{"2016postVFP", {{"etau",	name16post+fname+"etau.root"},
					 {"mutau",	name16post+fname+"mutau.root"},
					 {"tautau", name16post+fname+"tautau.root"}}},
	{"2017",        {{"etau",	name17+fname+"etau.root"},
					 {"mutau",	name17+fname+"mutau.root"},
					 {"tautau", name17+fname+"tautau.root"}}},
	{"2018",        {{"etau",	name18+fname+"etau.root"},
					 {"mutau",	name18+fname+"mutau.root"},
					 {"tautau", name18+fname+"tautau.root"}}}
  };
	
  // open files
  for (auto& in : inputs) {
	for (auto const& chn : mChannels) {
	  fileIn[in.first][chn] = std::unique_ptr<TFile>{TFile::Open(in.second[chn].c_str(), "READ")};
	  mCheckFile(fileIn[in.first][chn], in.second[chn]);
	}
  }

  // view TF1's
  for (auto& in : inputs) {
	for (auto const& chn : mChannels) {
	  funcSF[in.first][chn]   = fileIn[in.first][chn]->Get<TF1>("SigmoidFuncSF");
	  funcData[in.first][chn] = fileIn[in.first][chn]->Get<TF1>("SigmoidFuncData");
	  funcMC[in.first][chn]   = fileIn[in.first][chn]->Get<TF1>("SigmoidFuncMC");
	}
  }

  // access TF1's ranges to address out-of-bounds events
  mRange = std::make_pair(funcSF[mPeriods[0]]["etau"]->GetMinimumX(),
						  funcSF[mPeriods[0]]["etau"]->GetMaximumX());
  // the bounds should be the same for all functions
  for (auto& in : inputs) {
	for (auto const& chn : mChannels) {
	  assert(funcSF[in.first][chn]->GetMinimumX() == mRange.first);
	  assert(funcSF[in.first][chn]->GetMaximumX() == mRange.second);
	}
  }
}

ScaleFactorMET::~ScaleFactorMET() {
  for (auto& in : inputs) {
	for (auto const& chn : mChannels) {
	  fileIn[in.first][chn]->Close();
	}
  }
}

void ScaleFactorMET::mCheckFile(std::unique_ptr<TFile>& f, std::string input)
{
  if (f->IsZombie()) {
	std::cout << "ERROR in ScaleFactorMET::mCheckFile(string input)" << std::endl;
	std::cout << "File " << input << " does not exist. Please check. " << std::endl;
	std::exit(1);
  }
}

void ScaleFactorMET::mCheckPeriod(std::string period)
{
  if (std::find(mPeriods.begin(), mPeriods.end(), period) == std::end(mPeriods)) {
	std::cout << "ERROR in ScaleFactorMET::mCheckPeriod(string period)" << std::endl;
	std::cout << "Period " << period << " is not supported. " << std::endl;
	std::exit(1);
  }
}

void ScaleFactorMET::mCheckChannel(std::string chn)
{
  if (std::find(mChannels.begin(), mChannels.end(), chn) == std::end(mChannels)) {
	std::cout << "ERROR in ScaleFactorMET::mCheckChannel(string period)" << std::endl;
	std::cout << "Channel " << chn << " is not supported. " << std::endl;
	std::exit(1);
  }
}

/* propagation of errors of sigmoid function
   assumes no correlations for simplicity
   returns the squared error for efficiency */ 
double ScaleFactorMET::mErrorQuadSumSquared(double x, std::string period, std::string chn, std::string mode)
{
  TF1* func;
  if (mode == "Data") {
	func = funcData[period][chn];
  }
  else if (mode == "MC") {
	func = funcMC[period][chn];
  }
  else {
	std::cout << "ERROR in ScaleFactorMET::mErrorQuadSum(string inputRootFile)" << std::endl;
	std::cout << "Mode " << mode << " is not supported. Please check. " << std::endl;
	std::exit(1);
  }

  double a = func->GetParameter(0);
  double b = func->GetParameter(1);
  double c = func->GetParameter(2);
  double ea = func->GetParError(0);
  double eb = func->GetParError(1);
  double ec = func->GetParError(2);

  double sigexp = exp(-1*a*(x-b)); //sigmoid exponential
  return (c*c*sigexp*sigexp)*((x-b)*(x-b)*ea*ea + a*a*eb*eb)/pow(1+sigexp,4) + ec*ec/pow(1+sigexp,2);
}

/* propagation of errors of division function
   assumes no correlations for simplicity */
double ScaleFactorMET::mErrorRatio(double x, std::string period, std::string chn)
{
  double n; // numerator
  double d; // denominator

  if (x > mRange.second) {
	n = funcData[period][chn]->Eval(mRange.second);
	d = funcMC[period][chn]->Eval(mRange.second);
  }
  else if (x < mRange.first) {
	n = funcData[period][chn]->Eval(mRange.first);
	d = funcMC[period][chn]->Eval(mRange.first);
  }
  else {
	n = funcData[period][chn]->Eval(x);
	d = funcMC[period][chn]->Eval(x);
  }
  
  double en2 = mErrorQuadSumSquared(x, period, chn, "Data"); // numerator
  double ed2 = mErrorQuadSumSquared(x, period, chn, "MC");   // denominator
  double eratio2 = en2/(n*n) + ed2/(d*d);
  return (n/d) * sqrt(eratio2);
}
  
double ScaleFactorMET::getSF(double metnomu, std::string period, std::string chn)
{
  mCheckPeriod(period);
  mCheckChannel(chn);
  double sf;
  if (metnomu > mRange.second) {
	sf = funcSF[period][chn]->Eval(mRange.second);
  }
  else if (metnomu < mRange.first) {
	sf = funcSF[period][chn]->Eval(mRange.first);
  }
  else {
	sf = funcSF[period][chn]->Eval(metnomu);
  }
  return sf;
}

double ScaleFactorMET::getSFError(double metnomu, std::string period, std::string chn)
{
  mCheckPeriod(period);
  mCheckChannel(chn);
  double err = mErrorRatio(metnomu, period, chn);
  return err/2.; //equal up and down variations
}
