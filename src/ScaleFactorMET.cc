// Custom MET scale factors derived with https://github.com/bfonta/inclusion
// Contact: bruno.alves@cern.ch
#include "ScaleFactorMET.h"

ScaleFactorMET::ScaleFactorMET()
{
  const std::string fname = "eff_Data_Mu_MC_TT_DY_WJets_mumu_metnomu_et_TRG_METNoMu120_CUTS_mhtnomu_et_L_0p0_default";
  const std::string name16pre  = "weights/trigger_SF_UL/2016preVFP/";
  const std::string name16post = "weights/trigger_SF_UL/2016postVFP/";
  const std::string name17     = "weights/trigger_SF_UL/2017/";
  const std::string name18     = "weights/trigger_SF_UL/2018/";
  inputs = {
	{"2016preVFP",  name16pre  + fname + ".root"},
	{"2016postVFP", name16post + fname + ".root"},
	{"2017",		name17	   + fname + ".root"},
	{"2018",		name18	   + fname + ".root"},
  };
	
  // open files
  for (auto& in : inputs) {
	fileIn[in.first] = std::unique_ptr<TFile>{TFile::Open(in.second.c_str(), "READ")};
	mCheckFile(fileIn[in.first], in.second);
  }

  // view TF1's
  for (auto& in : inputs) {
	funcSF[in.first]   = fileIn[in.first]->Get<TF1>("SigmoidFuncSF");
	funcData[in.first] = fileIn[in.first]->Get<TF1>("SigmoidFuncData");
	funcMC[in.first]   = fileIn[in.first]->Get<TF1>("SigmoidFuncMC");
  }

  // access TF1's ranges to address out-of-bounds events
  mRange = std::make_pair(funcSF[mPeriods[0]]->GetMinimumX(),
						  funcSF[mPeriods[0]]->GetMaximumX());
  // the bounds should be the same for all functions
  for (auto& in : inputs) {
	assert(funcSF[in.first]->GetMinimumX() == mRange.first);
	assert(funcSF[in.first]->GetMaximumX() == mRange.second);
  }
}

ScaleFactorMET::~ScaleFactorMET() {
  for (auto& in : inputs) {
	fileIn[in.first]->Close();
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

/* propagation of errors of sigmoid function
   assumes no correlations for simplicity
   returns the squared error for efficiency */ 
double ScaleFactorMET::mErrorQuadSumSquared(double x, std::string period, std::string mode)
{
  TF1* func;
  if (mode == "Data") {
	func = funcData[period];
  }
  else if (mode == "MC") {
	func = funcMC[period];
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
double ScaleFactorMET::mErrorRatio(double x, std::string period)
{
  double n; // numerator
  double d; // denominator

  if (x > mRange.second) {
	x = mRange.second;
  }
  else if (x < mRange.first) {
	x = mRange.first;
  }
  
  n = funcData[period]->Eval(x);
  d = funcMC[period]->Eval(x);

  double en2 = mErrorQuadSumSquared(x, period, "Data"); // numerator
  double ed2 = mErrorQuadSumSquared(x, period, "MC");   // denominator
  double eratio2 = en2/(n*n) + ed2/(d*d);

  /* add quadratically the 5% "channel transfer" error
	 this accounts for using mumu to estimate etau, mutau or tautau MET SFs
	 currently judged not necessary since channels are compatible within available stats */
  // eratio2 += (n/d) * (n/d) * 0.05 * 0.05; // 
  
  return (n/d) * sqrt(eratio2);
}
  
double ScaleFactorMET::getSF(double metnomu, std::string period)
{
  mCheckPeriod(period);

  metnomu = mImposeBounds(metnomu);
  return funcSF[period]->Eval(metnomu);
}

double ScaleFactorMET::getSFError(double metnomu, std::string period)
{
  mCheckPeriod(period);

  metnomu = mImposeBounds(metnomu);
  return mErrorRatio(metnomu, period); //equal up and down variations
}

double ScaleFactorMET::mImposeBounds(double x)
{
  if (x > mRange.second) {
	x = mRange.second;
  }
  else if (x < mRange.first) {
	x = mRange.first;
  }
  return x;
}
