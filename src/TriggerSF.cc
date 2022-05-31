#include "TriggerSF.h"

#define DEBUG false

// Constructor
TriggerSF::TriggerSF( TriggerChannelLists triggers,
					  umap<std::string,std::string> eff_files,
					  umap<std::string,std::string> var_files,
					  unsigned year)
{
  // inputs consistency checks
  assert( eff_files.size() == var_files.size() );
  assert(  triggers.channels().size() == var_files.size() );
  
  for (auto &x: triggers.channels()) {
	// sanity check
	mCheckExtension(eff_files[x], "root");
	mCheckExtension(var_files[x], "json");

	// load efficiency and scale factor files
	mEffFiles[x] = TFile::Open(eff_files[x].c_str(), "READ");

	// load files with variables
	std::ifstream i(var_files[x]);
	std::cout << var_files[x] << std::endl;
	std::exit(0);
	i >> mVarFiles[x];

	//load intersection efficiency strings [first: MC; second: Data]
	mInters[x] = std::make_pair( mGetTriggerIntersections( triggers, x, false ),
								 mGetTriggerIntersections( triggers, x, true ) );
  }
}

// Destructor
TriggerSF::~TriggerSF()
{
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

auto TriggerSF::mEffVariablesToHistoName( const vec2<std::string>& effVars ) -> std::string
{
  assert( effVars[0].size() == 1);
  return effVars[0][0]; /// CHANGE according to #dimensions and histogram naming scheme
}

auto TriggerSF::getEvtWeight(const EventVariables& vars,
							 std::string channel) -> const float
{
  //TriggerChannelLists::mCheckChannel(channel);
  const float probability_data = mGetUnionEfficiency( vars, channel, true );
  const float probability_mc = mGetUnionEfficiency( vars, channel, false );
  const float eventWeight = probability_data / probability_mc;
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
  float un_eff = 0.f;

  for (auto &inters : mMCOrDataIntersections(channel, isData))
	{
	  const TriggerSF::EffValue eff = mGetIntersectionEfficiencies( channel, inters, isData );
	  un_eff += eff.getVal( vars.dau1_pt() );
	}
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

	  un_eup += eff.getErrUp( vars.dau1_pt() ); /// CHANGE how should the error be calculated
	  un_elow += eff.getErrLow( vars.dau1_pt() ); /// CHANGE how should the error be calculated
	}
  return std::make_pair(un_eup, un_elow);
}

/// CHANGE to consider more dimensions
auto TriggerSF::mReadIntersectionEfficiencies( std::string channel,
											   std::string trigInters,
											   std::string hname,
											   const bool isData ) -> EffValue
{
  TH1 *h = (TH1*)mEffFiles[channel]->Get(hname.c_str());
  TriggerSF::EffValue val(h);
  return val;
}

auto TriggerSF::mGetIntersectionEfficiencies( std::string channel,
											  std::string trigInters,
											  const bool isData ) -> const EffValue
{
  vec2<std::string> var = mVarFiles[channel][trigInters].get<vec2<std::string>>();
  std::string hname = mEffVariablesToHistoName(var);
  
  if ( ! mValues.exists(channel, trigInters, hname) )
	{
	  EffValue eff_val = mReadIntersectionEfficiencies(channel, trigInters, hname, isData);
	  mValues.set(eff_val, channel, trigInters, hname);
	}

  return mValues(channel, trigInters, hname);
}

auto TriggerSF::mGetTriggerIntersections( const TriggerChannelLists& list,
										  std::string channel,
										  const bool isData ) -> const vec<std::string>
{
  VectorCombinations comb;
  vec<std::string> strs = list.get(channel, isData);
  vec<std::string> res = comb.combine_all_k<std::string>(strs).flatten();
  // comb.combine_all_k<std::string>(strs).print();
  return res;                                                                     
}
