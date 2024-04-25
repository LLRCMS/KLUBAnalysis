// Custom MET scale factors derived with https://github.com/bfonta/inclusion
// Contact: bruno.alves@cern.ch
#include "ScaleFactorMET.h"

ScaleFactorMET::ScaleFactorMET(std::string period): mPeriod(period)
{
  mCheckPeriod();
  
  const std::string name16pre  = "weights/trigger_SF_UL/2016preVFP/";
  const std::string name16post = "weights/trigger_SF_UL/2016postVFP/";
  const std::string name17     = "weights/trigger_SF_UL/2017/";
  const std::string name18     = "weights/trigger_SF_UL/2018/";
  inputs = {
	{"2016preVFP",  name16pre  + "150_mumu_fit_2016APV.root"},
	{"2016postVFP", name16post + "150_mumu_fit_2016.root"},
	{"2017",		name17	   + "150_mumu_fit_2017.root"},
	{"2018",		name18	   + "150_mumu_fit_2018.root"},
  };
	
  // open file
  fileIn = std::unique_ptr<TFile>{TFile::Open(inputs[mPeriod].c_str(), "READ")};
  mCheckFile(fileIn, inputs[mPeriod]);

  // view TF1's
  funcSF   = fileIn->Get<TF1>("SigmoidFuncSF");
  funcData = fileIn->Get<TF1>("SigmoidFuncData");
  funcMC   = fileIn->Get<TF1>("SigmoidFuncMC");

  // sanity check
  assert(funcSF->GetMinimumX() == mRange.at(mPeriod).first);
  assert(funcSF->GetMaximumX() == mRange.at(mPeriod).second);
}

ScaleFactorMET::~ScaleFactorMET() {
  fileIn->Close();
}

void ScaleFactorMET::mCheckFile(std::unique_ptr<TFile>& f, std::string input)
{
  if (f->IsZombie()) {
	std::cout << "ERROR in ScaleFactorMET::mCheckFile(string input)" << std::endl;
	std::cout << "File " << input << " does not exist. Please check. " << std::endl;
	std::exit(1);
  }
}

void ScaleFactorMET::mCheckPeriod()
{
  if (std::find(mPeriods.begin(), mPeriods.end(), mPeriod) == std::end(mPeriods)) {
	std::cout << "ERROR in ScaleFactorMET::mCheckPeriod()" << std::endl;
	std::cout << "Period " << mPeriod << " is not supported. " << std::endl;
	std::exit(1);
  }
}

/* propagation of errors of sigmoid function
   assumes no correlations for simplicity
   returns the squared error for efficiency */ 
double ScaleFactorMET::mErrorQuadSumSquared(double x, std::string mode)
{
  TF1* func;
  if (mode == "Data") {
	func = funcData;
  }
  else if (mode == "MC") {
	func = funcMC;
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
double ScaleFactorMET::mErrorRatio(double x)
{
  double n; // numerator
  double d; // denominator
  
  n = funcData->Eval(x);
  d = funcMC->Eval(x);

  double en2 = mErrorQuadSumSquared(x, "Data"); // numerator
  double ed2 = mErrorQuadSumSquared(x, "MC");   // denominator
  double eratio2 = en2/(n*n) + ed2/(d*d);

  /* add quadratically the 5% "channel transfer" error
	 this accounts for using mumu to estimate etau, mutau or tautau MET SFs
	 currently judged not necessary since channels are compatible within available stats */
  // eratio2 += (n/d) * (n/d) * 0.05 * 0.05; // 
  
  return (n/d) * sqrt(eratio2);
}

float ScaleFactorMET::getMinThreshold()
{
  return mRange.at(mPeriod).first;
}
  
double ScaleFactorMET::getSF(double metnomu)
{
  metnomu = mImposeBounds(metnomu);

  double sf;
  if (mPeriod != "2017" and metnomu == mRange.at(mPeriod).second) {
	sf = 1.;
  }
  else {
	sf = funcSF->Eval(metnomu);
  }
  return sf;
}

double ScaleFactorMET::getSFError(double metnomu)
{
  metnomu = mImposeBounds(metnomu);
  return mErrorRatio(metnomu); //equal up and down variations
}

double ScaleFactorMET::mImposeBounds(double x)
{
  if (x > mRange.at(mPeriod).second) {
	x = mRange.at(mPeriod).second;
  }
  else if (x < mRange.at(mPeriod).first) {
	x = mRange.at(mPeriod).first;
  }
  return x;
}
