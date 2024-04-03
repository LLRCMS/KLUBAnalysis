// Assing analysis regions to different triggers: MET, SingleTau and Single/Cross Lepton
// Contact: bruno.alves@cern.ch
#include "TriggerAssigner.h"

TriggerAssigner::TriggerAssigner() {
  float ele_gap = 1., muon_gap = 2., tau_gap = 5.;
  mLep1Thresh = {
	{"2016preVFP",  {{"mutau", 24. + muon_gap}, {"etau", 25. + ele_gap}}},
	{"2016postVFP", {{"mutau", 24. + muon_gap}, {"etau", 25. + ele_gap}}},
	{"2017",        {{"mutau", 27. + muon_gap}, {"etau", 32. + ele_gap}}},
	{"2018",        {{"mutau", 24. + muon_gap}, {"etau", 32. + ele_gap}}}
  };
  mLep2Thresh = {
	{"2016preVFP",  {{"mutau", 20. + tau_gap}, {"etau", -1.}}}, //no cross-etau trigger for 2016
	{"2016postVFP", {{"mutau", 20. + tau_gap}, {"etau", -1.}}}, //no cross-etau trigger for 2016
	{"2017",        {{"mutau", 27. + tau_gap}, {"etau", 30. + tau_gap}}},
	{"2018",        {{"mutau", 27. + tau_gap}, {"etau", 30. + tau_gap}}}
  };
}

void TriggerAssigner::mCheckChannel(std::string channel)
{
  if (std::find(mChannels.begin(), mChannels.end(), channel) == std::end(mChannels)) {
	std::cout << "ERROR in TriggerAssigner::mCheckChannel()" << std::endl;
	std::cout << "Channel " << channel << " is not supported. " << std::endl;
	std::exit(1);
  }
}

void TriggerAssigner::mCheckPeriod(std::string period, std::string channel, bool no16)
{
  if (std::find(mPeriods.begin(), mPeriods.end(), period) == std::end(mPeriods)) {
	std::cout << "ERROR in TriggerAssigner::mCheckPeriod()" << std::endl;
	std::cout << "Period " << period << " is not supported. " << std::endl;
	std::exit(1);
  }
  if (no16 and channel == "etau" and period.find("16") != std::string::npos) {
	std::cout << "ERROR in TriggerAssigner::mCheckPeriod()" << std::endl;
	std::cout << "Period " << period << " cannot be used in this context." << std::endl;
	std::exit(1);  
  }
}

float TriggerAssigner::getLepton1PtThresh(std::string period, std::string channel) {
  mCheckChannel(channel);
  mCheckPeriod(period, channel, false);
  return mLep1Thresh[period][channel];
}

float TriggerAssigner::getLepton2PtThresh(std::string period, std::string channel) {
  mCheckChannel(channel);
  mCheckPeriod(period, channel, true);
  return mLep2Thresh[period][channel];
}

// Legacy triggers take precedence over Single-Tau and MET triggers
TriggerAssigner::uMap<bool>
TriggerAssigner::assignTriggerRegions(int ptype, std::string period,
									  float pt1, float pt2, float eta1, float eta2,
									  float eleEtaMax, float muEtaMax, float tau_thresh)
{
  bool Legacy_region    = false;
  bool SingleTau_region = false;
  bool MET_region       = false;
  
  if (ptype == 0) { //mutau
	bool SingleLepton_validity = false;
	bool CrossLepton_validity  = false;
	bool SingleTau_validity    = false;
	
	if(period=="2018") {
	  SingleLepton_validity = pt1 >= mLep1Thresh[period]["mutau"] and eta1 < muEtaMax;
	  CrossLepton_validity	= (pt1 >= 22. and eta1 < mEtaThresh and
							   pt2 >= mLep2Thresh[period]["mutau"] and eta2 < mEtaThresh);
	  SingleTau_validity	= pt2 >= tau_thresh and eta2 < mEtaThresh;
	}
	else if(period=="2017") {
	  SingleLepton_validity = pt1 >= mLep1Thresh[period]["mutau"] and eta1 < muEtaMax;
	  CrossLepton_validity	= (pt1 >= 22. and eta1 < mEtaThresh and
							   pt2 >= mLep2Thresh[period]["mutau"] and eta2 < mEtaThresh);
	  SingleTau_validity	= pt2 >= tau_thresh and eta2 < mEtaThresh;
	}
	else if (period=="2016preVFP" or period=="2016postVFP") {
	  SingleLepton_validity = pt1 >= mLep1Thresh[period]["mutau"] and eta1 < muEtaMax;
	  CrossLepton_validity	= (pt1 >= 21. and eta1 < mEtaThresh and
							   pt2 >= mLep2Thresh[period]["mutau"] and eta2 < mEtaThresh);
	  SingleTau_validity	= pt2 >= tau_thresh and eta2 < mEtaThresh;
	}

	Legacy_region = SingleLepton_validity or CrossLepton_validity;
	SingleTau_region = SingleTau_validity and !Legacy_region;
  }

  else if (ptype == 1) { //etau
	if(period=="2018" or period=="2017") {
	  bool SingleLepton_validity = pt1 >= mLep1Thresh[period]["etau"] and eta1 < eleEtaMax;
	  bool CrossLepton_validity  = (pt1 >= 25. and eta1 < mEtaThresh and
									pt2 >= mLep2Thresh[period]["etau"] and eta2 < mEtaThresh);
	  bool SingleTau_validity    = pt2 >= tau_thresh and eta2 < mEtaThresh;

	  Legacy_region = SingleLepton_validity or CrossLepton_validity;
	  SingleTau_region = SingleTau_validity and !Legacy_region;
	}
	else if (period=="2016preVFP" or period=="2016postVFP") {
	  Legacy_region    = pt1 >= mLep1Thresh[period]["etau"] and eta1 < mEtaThresh;
	  SingleTau_region = pt2 >= tau_thresh and !Legacy_region;
	}
  }

  else if (ptype == 2) { //tautau
	Legacy_region = pt1 >= 40. and pt2 >= 40. and eta1 < mEtaThresh and eta2 < mEtaThresh;
	SingleTau_region = (((pt1 >= tau_thresh and eta1 < mEtaThresh) or (pt2 >= tau_thresh and eta2 < mEtaThresh))
						and !Legacy_region);
  }
  
  else if (ptype == 3) { //mumu
	bool SingleLepton_validity = ((pt1 >= mLep1Thresh[period]["mutau"] and eta1 < muEtaMax) or
								  (pt2 >= mLep1Thresh[period]["mutau"] and eta2 < muEtaMax));
	Legacy_region = SingleLepton_validity;
  }

  if (ptype == 4) { //ee
	bool SingleLepton_validity = ((pt1 >= mLep1Thresh[period]["etau"] and eta1 < eleEtaMax) or
								  (pt2 >= mLep1Thresh[period]["etau"] and eta2 < eleEtaMax));
	Legacy_region = SingleLepton_validity;
  }

  MET_region = !Legacy_region and !SingleTau_region;
  std::unordered_map<std::string, bool> ret{
	{"legacy", Legacy_region},
	{"legacy_cross", Legacy_region and eta2 < mEtaThresh}, //all cross triggers have this eta cut
	{"tau", SingleTau_region},
	{"met", MET_region}
  };
  return ret;
}
