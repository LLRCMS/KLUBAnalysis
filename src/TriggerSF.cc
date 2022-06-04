#include "TriggerSF.h"

// #define DEBUG

// Constructor
TriggerSF::TriggerSF( TriggerChannelLists triggers,
					  umap<std::string,std::string> eff_files,
					  umap<std::string,std::string> var_files,
					  unsigned year)
{
  if (!this->mTest())
	throw std::invalid_argument("Test failed.");

#ifdef DEBUG
  std::cout << "====== Enter Constructor" << std::endl;
#endif
  // inputs consistency checks
  assert( eff_files.size() == var_files.size() );
  assert(  triggers.channels().size() == var_files.size() );
  
  for (auto &x: triggers.channels()) {
	// sanity check
	mCheckExtension(eff_files[x], "root");
	mCheckExtension(var_files[x], "json");

	// load efficiency and scale factor files
	mEffFiles[x] = TFile::Open(eff_files[x].c_str(), "READ");
	if (mEffFiles[x]->IsZombie()) {
	  std::string mess = "File " + eff_files[x] + " not found.";
	  throw std::invalid_argument(mess);
	}
	
	// load files with variables
	std::ifstream i(var_files[x]);
	i >> mVarFiles[x];

	//load intersection efficiency strings [first: MC; second: Data]
	mInters[x] = std::make_pair( mGetTriggerIntersections( triggers, x, false ),
								 mGetTriggerIntersections( triggers, x, true ) );
  }
#ifdef DEBUG
  std::cout << "====== Exit Constructor" << std::endl;
#endif
}

// Destructor
TriggerSF::~TriggerSF()
{
}

auto TriggerSF::mBuildObjectName(std::string varname,
								 std::string trigInters,
								 bool isData) const -> std::string
{
  std::string res = isData ? "Data" : "MC";
  res += "_VAR_" + varname;
  res += "_TRG_" + trigInters;
  res += "_CUT_";
  int nitems = mCountNumberTriggerItems(trigInters);
  for(int i=0; i<nitems-1; ++i)
	res += "NoCut_PLUS_";
  res += "NoCut";
  return res;
}

auto TriggerSF::mCheckChannel(std::string channel) const -> void
{
  if (mChannelsAllowed.find(channel) == mChannelsAllowed.end()) {
	std::string message = "[TriggerChannelLists::add()] Channel " + channel + " is not supported!";
	throw std::invalid_argument(message);
  }
}

auto TriggerSF::mCheckExtension( std::string str,
								 std::string ext ) -> void
{
  vec<std::string> results;
  boost::split(results, str, boost::is_any_of("."));
  if(results.size() != 2)
	throw std::invalid_argument("The filename must have exactly one dot (`.`) (as expected).");
  if(results[1] != ext)
	throw std::invalid_argument("The extension is wrong.");
}

auto TriggerSF::mCountNumberTriggerItems(std::string str) const -> const int
{
   int occurrences = 0;
   std::string::size_type pos = 0;
   while ((pos = str.find(mTriggerStrConnector, pos)) != std::string::npos)
	 {
	   ++occurrences;
	   pos += mTriggerStrConnector.length();
	 }
   return occurrences + 1; //number of items is one plus number of connector strings
}

auto TriggerSF::mEffVariablesToVarName( const vec2<std::string>& effVars ) -> std::string
{
  return effVars[0][0]; /// CHANGE according to #dimensions and histogram naming scheme
}

auto TriggerSF::getEvtWeight(const EventVariables& vars,
							 std::string channel) -> const float
{
#ifdef DEBUG
  std::cout << "====== Enter getEvtWeight" << std::endl;
#endif
  //TriggerChannelLists::mCheckChannel(channel);
  const float probability_data = mGetUnionEfficiency( vars, channel, true );
  const float probability_mc = mGetUnionEfficiency( vars, channel, false );
  const float eventWeight = probability_data / probability_mc;
#ifdef DEBUG
  std::cout << "====== Exit getEvtWeight" << std::endl;
#endif
  return eventWeight;
}

auto TriggerSF::getEvtWeightErrors( const EventVariables& vars,
									std::string channel ) -> const mPair
{
  //TriggerChannelLists::mCheckChannel(channel);
  const TriggerSF::mPair errors_data = mGetUnionEfficiencyErrors( vars, channel, true );
  const TriggerSF::mPair errors_mc = mGetUnionEfficiencyErrors( vars, channel, false );

  const float error_up = errors_data.first / errors_mc.first; /// CHANGE
  const float error_down = errors_data.second / errors_mc.second; /// CHANGE
  const TriggerSF::mPair errors = std::make_pair(error_up, error_down);
  return errors; 
}

auto TriggerSF::mMCOrDataIntersections( std::string channel,
										const bool isData ) -> vec<std::string>
{
  return isData==1 ? std::get<1>(mInters[channel]) : std::get<0>(mInters[channel]);
}

auto TriggerSF::mGetUnionEfficiency( const EventVariables& vars,
									 std::string channel,
									 const bool isData ) -> const float
{
#ifdef DEBUG  
  std::cout << "====== Enter getUnionEfficiency" << std::endl;
#endif
  float un_eff = 0.f;

  for (auto &inters : mMCOrDataIntersections(channel, isData))
	{
	  if (mVarFiles[channel].contains(inters))
		{
		  const TriggerSF::EffValue eff = mGetIntersectionEfficiencies( channel, inters, isData );
		  if(mCountNumberTriggerItems(inters)%2==0)
			un_eff -= eff.getVal( vars("dau1_pt") );
		  else
			un_eff += eff.getVal( vars("dau1_pt") );
		}
	  else {
		std::string mess = "JSON key does not exist. Channel: " + channel + "; Inters: " + inters + ".";
		throw std::invalid_argument(mess);
	  }
	}
#ifdef DEBUG  
  std::cout << "====== Exit getUnionEfficiency" << std::endl;
#endif
  return un_eff;
}

auto TriggerSF::mGetUnionEfficiencyErrors( const EventVariables& vars,
										   std::string channel,
										   const bool isData ) -> const mPair
{
  float un_eup = 0.f;
  float un_elow = 0.f;

  for (const auto &inters : mMCOrDataIntersections(channel, isData))
	{
	  TriggerSF::EffValue eff = mGetIntersectionEfficiencies( channel, inters, isData );

	  un_eup += eff.getErrUp( vars("dau1_pt") ); /// CHANGE how should the error be calculated
	  un_elow += eff.getErrLow( vars("dau1_pt") ); /// CHANGE how should the error be calculated
	}
  return std::make_pair(un_eup, un_elow);
}

/// CHANGE to consider more dimensions
auto TriggerSF::mReadIntersectionEfficiencies( std::string channel,
											   std::string trigInters,
											   std::string varname,
											   const bool isData ) -> EffValue
{
#ifdef DEBUG  
  std::cout << "====== Enter mReadIntersectionEfficiencies" << std::endl;
#endif
  std::string hname = mBuildObjectName(varname, trigInters, isData);
  TH1F *histo = (TH1F*)mEffFiles[channel]->Get(hname.c_str());
  if (histo==nullptr) {
	std::string mess = "Graph " + hname + " was not found ";
	mess += "(channel " + channel + ", ";
	mess += "trigger intersection " + trigInters + ").";
	throw std::invalid_argument(mess);
  }
  TriggerSF::EffValue val(histo);
#ifdef DEBUG  
  std::cout << "====== Exit mReadIntersectionEfficiencies" << std::endl;
#endif
  return val;
}

auto TriggerSF::mGetIntersectionEfficiencies( std::string channel,
											  std::string trigInters,
											  const bool isData ) -> const EffValue
{
#ifdef DEBUG  
  std::cout << "====== Enter mGetIntersectionEfficiencies" << std::endl;
#endif
  vec2<std::string> var = mVarFiles[channel][trigInters].get<vec2<std::string>>();
  std::string varname = mEffVariablesToVarName(var);
  
  if ( ! mValues.contains(channel, trigInters, varname) )
	{
	  EffValue eff_val = mReadIntersectionEfficiencies(channel, trigInters, varname, isData);
	  mValues.set(eff_val, channel, trigInters, varname);
	}
#ifdef DEBUG
  std::cout << "====== Exit mGetIntersectionEfficiencies" << std::endl;
#endif
  return mValues(channel, trigInters, varname);
}

auto TriggerSF::mGetTriggerIntersections( const TriggerChannelLists& list,
										  std::string channel,
										  const bool isData ) -> const vec<std::string>
{
  VectorCombinations comb;
  vec<std::string> strs = list.get(channel, isData);
  vec<std::string> new_strs = mKLUBStandaloneNameMatching(strs, channel, isData); //KLUB - Python standalone matching
  vec<std::string> res = comb.combine_all_k<std::string>(new_strs).flatten(mTriggerStrConnector);
  // comb.combine_all_k<std::string>(strs).print();
  return res;                                                                     
}

//The following matching must follow the trigger naming scheme adopted in the Python standalone framework
auto TriggerSF::mKLUBStandaloneNameMatching(vec<std::string> old_strs,
											std::string channel,
											const bool isData ) -> const vec<std::string>
{
  std::set<std::string> new_set; //automatically removes duplicates
  for (const auto &s : old_strs)
	{
	  // === channel-generic triggers ===
	  // MET
	  if (s=="HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v") {
		new_set.insert("METNoMu120");
		continue;
	  }
	  //VBF
	  else if (s=="HLT_VBF_DoubleLooseChargedIsoPFTau20_Trk1_eta2p1_v" or
			   s=="HLT_VBF_DoubleLooseChargedIsoPFTauHPS20_Trk1_eta2p1_v") {
		new_set.insert("VBFTauCustom");
		continue;
	  }
	  //Single Tau
	  else if (s=="HLT_MediumChargedIsoPFTau50_Trk30_eta2p1_1pr_MET100_v")
		new_set.insert("IsoTau50");
	  else if (s=="HLT_MediumChargedIsoPFTau180HighPtRelaxedIso_Trk50_eta2p1_v")
		new_set.insert("IsoTau180");
	  
	  // === channel-specific triggers ===
	  if(channel=="EleTau")
		{
		  if (s=="HLT_Ele32_WPTight_Gsf_v")
			new_set.insert("Ele32");
		  else if (s=="HLT_Ele35_WPTight_Gsf_v")
			new_set.insert("Ele35");
		  else if (s=="HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTauHPS30_eta2p1_CrossL1_v" or
				   s=="HLT_Ele24_eta2p1_WPTight_Gsf_LooseChargedIsoPFTau30_eta2p1_CrossL1_v")
			new_set.insert("EleIsoTauCustom");
		  else {
			std::string mess = "Trigger " + s + " not supported for channel " + channel + ".";
			throw std::invalid_argument(mess);
		  }
		}
	  else if(channel=="MuTau")
		{
		  if (s=="HLT_IsoMu24_v")
			new_set.insert("IsoMu24");
		  else if (s=="HLT_IsoMu27_v")
			new_set.insert("IsoMu27");
		  else if (s=="HLT_IsoMu20_eta2p1_LooseChargedIsoPFTauHPS27_eta2p1_CrossL1_v" or
				   s=="HLT_IsoMu20_eta2p1_LooseChargedIsoPFTau27_eta2p1_CrossL1_v")
			new_set.insert("IsoMuIsoTauCustom");
		  else {
			std::string mess = "Trigger " + s + " not supported for channel " + channel + ".";
			throw std::invalid_argument(mess);
		  }
		}
	  else if(channel=="TauTau")
		{
		  if (s=="HLT_DoubleTightChargedIsoPFTau35_Trk1_TightID_eta2p1_Reg_v"  or
			  s=="HLT_DoubleMediumChargedIsoPFTau40_Trk1_TightID_eta2p1_Reg_v" or
			  s=="HLT_DoubleTightChargedIsoPFTau40_Trk1_eta2p1_Reg_v"          or
			  s=="HLT_DoubleMediumChargedIsoPFTauHPS35_Trk1_eta2p1_Reg_v")
			new_set.insert("IsoDoubleTauCustom");
		  else {
			std::string mess = "Trigger " + s + " not supported for channel " + channel + ".";
			throw std::invalid_argument(mess);
		  }
		}
	  else
		throw std::invalid_argument("Channel " + channel + " not supported.");
	}

  vec<std::string> new_strs; 
  for (const auto &x : new_set)
	new_strs.push_back(x);

  return new_strs;
}

//TODO: should be incuded in a proper unit test
auto TriggerSF::mTest() const -> bool
{
  std::string fname = "weights/TriggerSF/trigSF_etau.root";
  std::string hname = "MC_VAR_dau2_eta_TRG_EleIsoTauCustom_CUT_NoCut";
  
  TFile* fin = TFile::Open(fname.c_str(), "READ");
  TH1F *h = (TH1F*)fin->Get(hname.c_str());
  
  if (h==nullptr) {
	std::string mess = "[EffValues::mTest()] Graph " + hname + " was not found.";
	throw std::invalid_argument(mess);
  }

  TriggerSF::EffValue val(h);
  float minDist = 1e-8;
  
  float v1 = val.getVal(-2.);
  if( fabs(v1-h->GetBinContent(1))<minDist )
	return true;
  float v2 = val.getVal(0.);
  if( fabs(v2-h->GetBinContent(4))<minDist )
	return true;
  float v3 = val.getVal(1.5);
  if( fabs(v3-h->GetBinContent(6))<minDist )
	return true;

  v1 = val.getErrLow(-2.);
  if( fabs(v1-h->GetBinErrorLow(1))<minDist )
	return true;
  v2 = val.getErrLow(0.);
  if( fabs(v2-h->GetBinErrorLow(4))<minDist )
	return true;
  v3 = val.getErrLow(1.5);
  if( fabs(v3-h->GetBinErrorLow(6))<minDist )
	return true;

  v1 = val.getErrUp(-2.);
  if( fabs(v1-h->GetBinErrorUp(1))<minDist )
	return true;
  v2 = val.getErrUp(0.);
  if( fabs(v2-h->GetBinErrorUp(4))<minDist )
	return true;
  v3 = val.getErrUp(1.5);
  if( fabs(v3-h->GetBinErrorUp(6))<minDist )
	return true;

  return false;
}
