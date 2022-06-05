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
	  std::string message = "[TriggerChannelLists::add()] Channel " + channel + " is not supported!";
	  throw std::invalid_argument(message);
	}
  }

 public:
  TriggerChannelLists() {}
  ~TriggerChannelLists() {}

  //channel-specific triggers
  auto mInternalAdd(std::string channel, bool isData, const vecstr& trigs) -> void
  {
	for (const auto &t : trigs) {
	  if (!isData)
		mTriggers[channel].first.push_back(t);
	  else
		mTriggers[channel].second.push_back(t);
	}
  }
  template <class... RestKeys>
  auto mInternalAdd(std::string channel, bool isData, const vecstr& trigs, RestKeys... args) -> void
  {
	return mInternalAdd(channel, isData, args...);
  }
  template <class ... Ts>
  auto add(std::string channel, bool isData, Ts... trigs) -> auto
  {
	mCheckChannel(channel);
	mInternalAdd(channel, isData, trigs...);
  }

  //channel-generic triggers
  auto mInternalAddGeneric(bool isData, const vecstr& trigs) -> void
  {
	for (const auto &t : trigs) {
	  if (!isData)
		mTriggers["generic"].first.push_back(t);
	  else
		mTriggers["generic"].second.push_back(t);
	}
  }
  template <class... RestKeys>
  auto mInternalAddGeneric(bool isData, const vecstr& trigs, RestKeys... args) -> void
  {
	return mInternalAddGeneric(isData, args...);
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
	if (!isData)
	  res.insert(res.end(),
				 mTriggers.at("generic").first.begin(),
				 mTriggers.at("generic").first.begin());
	else
	  res.insert(res.end(),
				 mTriggers.at("generic").second.begin(),
				 mTriggers.at("generic").second.begin());
	return res;
  }

};

#endif // TriggerChannelLists
