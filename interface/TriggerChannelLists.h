#ifndef TriggerChannelLists_h
#define TriggerChannelLists_h

// -------------------------------------------------------------------------------------------------------------- //
//                                                                                                                //
//   class TriggerChannelLists                                                                                    //
//                                                                                                                //
//   Helper class to save all triggers per channel as a single type.                                              //
//   Originally created as part of the computing trigger scale factors computation.                               //
//                                                                                                                //
//   Author: Bruno Alves (LLR, Ecole Polytechnique, Paris)                                                        //
//   Date  : April 2022                                                                                           //
//   Contact: bruno.alves@cern.ch                                                                                 //
//                                                                                                                //
// -------------------------------------------------------------------------------------------------------------- //

// Standard libraries
#include <string>
#include <set>
#include <vector>
#include <unordered_map>
#include <stdexcept>

// TriggerChannelLists class
// In the pair, Data is first and MC is second
class TriggerChannelLists {
 private:
  using vecstr = std::vector<std::string>;
  using umap = std::unordered_map<std::string, std::pair<vecstr,vecstr>>;
  umap mTriggers;

  const std::set<std::string> mChannelsAllowed = {"EleTau", "MuTau", "TauTau"};

  auto mCheckChannel(std::string channel) const -> void
  {
	if (mChannelsAllowed.find(channel) == mChannelsAllowed.end()) {
	  std::string mess = "[TriggerChannelLists::add()] Channel ";
	  mess += channel + " is not supported!";
	  throw std::invalid_argument(mess);
	}
  }
  
  //channel-specific triggers
  auto mInternalAdd(std::string channel, bool isData) -> void
  {
	std::string mess = "[mInternalAdd] ";
	mess += "The code should never get here.";
	throw std::runtime_error(mess);

  }
  template <class... RestKeys>
  auto mInternalAdd(std::string channel, bool isData,
					const vecstr& trigs, RestKeys... args) -> void
  {
	for (const auto &t : trigs) {
	  if (!isData)
		mTriggers[channel].first.push_back(t);
	  else
		mTriggers[channel].second.push_back(t);
	}
	if(sizeof...(args) != 0)
	  return mInternalAdd(channel, isData, args...);
  }

  //channel-generic triggers
  auto mInternalAddGeneric(bool isData) -> void
  {
	std::string mess = "[mInternalAddGeneric] ";
	mess += "The code should never get here.";
	throw std::runtime_error(mess);
  }
  template <class... RestKeys>
  auto mInternalAddGeneric(bool isData, const vecstr& trigs, RestKeys... args) -> void
  {
	for (const auto &t : trigs) {
	  if (!isData)
		mTriggers["generic"].first.push_back(t);
	  else
		mTriggers["generic"].second.push_back(t);
	}	
	if(sizeof...(args) != 0)
	  return mInternalAddGeneric(isData, args...);
  }

 public:
  TriggerChannelLists() {}
  ~TriggerChannelLists() {}

  template <class ... Ts>
  auto add(std::string channel, bool isData, Ts... trigs) -> auto
  {
	mCheckChannel(channel);
	mInternalAdd(channel, isData, trigs...);
  }

  template <class ... Ts>
  auto add_generic(bool isData, Ts... trigs) -> auto
  {
	mInternalAddGeneric(isData, trigs...);
  }

  auto channels() -> const std::set<std::string>
	{
	  return mChannelsAllowed;
	}

  // only returns channel specific trigger list
  auto get(std::string channel, bool isData) const -> vecstr
  {
	mCheckChannel(channel);
	vecstr res;
	if (!isData)
	  res = mTriggers.at(channel).first;
	else
	  res = mTriggers.at(channel).second;
	return res;
  }

  // returns channel specific list plus generic one
  auto get_full(std::string channel, bool isData) const -> vecstr
  {
	mCheckChannel(channel);
	vecstr res = this->get(channel, isData);
	vecstr res2;
	if (!isData)
	  res2 = mTriggers.at("generic").first;
	else
	  res2 = mTriggers.at("generic").second;
	res.reserve( res.size() + res2.size() );
	res.insert(res.end(),
			   res2.begin(),
			   res2.end());
	return res;
  }

};

#endif // TriggerChannelLists
