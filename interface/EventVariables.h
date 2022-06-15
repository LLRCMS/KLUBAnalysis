#ifndef EventVariables_h
#define EventVariables_h

#include <functional>
#include <unordered_map>
#include <string>

// ----------------------------------------------------------------------------------------------- //
//																								   //
//	 class EventVariables																		   //
//																								   //
//	 Convenience class to store and retrieve the variables of a single event.					   //
//	 Could be used for the entire framework. Currently used for computing trigger scale factors.   //
//																								   //
//	 Author: Bruno Alves (LLR, Ecole Polytechnique, Paris)										   //
//	 Date  : June 2022																			   //
//	 Contact: bruno.alves@cern.ch																   //
//																								   //
// ----------------------------------------------------------------------------------------------- //

class EventVariables {
private:
  using FuncSign = const float& (EventVariables::*)(void) const&;
  std::unordered_map<std::string,
					 std::function<const float&(void)>> fmap;

  float dau1_pt_;
  float dau2_pt_;
  float dau1_eta_;
  float dau2_eta_;
  
 public:
  auto dau1_pt() const& -> const float& { return dau1_pt_; }
  auto dau1_pt() &      -> float&       { return dau1_pt_; }
  auto dau1_pt() &&     -> float&&      { return std::move(dau1_pt_); }

  auto dau2_pt() const& -> const float& { return dau2_pt_; }
  auto dau2_pt() &      -> float&       { return dau2_pt_; }
  auto dau2_pt() &&     -> float&&      { return std::move(dau2_pt_); }

  auto dau1_eta() const& -> const float& { return dau1_eta_; }
  auto dau1_eta() &      -> float&       { return dau1_eta_; }
  auto dau1_eta() &&     -> float&&      { return std::move(dau1_eta_); }

  auto dau2_eta() const& -> const float& { return dau2_eta_; }
  auto dau2_eta() &      -> float&       { return dau2_eta_; }
  auto dau2_eta() &&     -> float&&      { return std::move(dau2_eta_); }
  
  EventVariables() {
	// implement "false reflection" via function pointer
	fmap = { {"dau1_pt",  std::bind<FuncSign>(&EventVariables::dau1_pt,  this)},
			 {"dau2_pt",  std::bind<FuncSign>(&EventVariables::dau2_pt,  this)},
			 {"dau1_eta", std::bind<FuncSign>(&EventVariables::dau1_eta, this)},
			 {"dau2_eta", std::bind<FuncSign>(&EventVariables::dau2_eta, this)} };
  }
  
  auto operator()(std::string s) const& -> const float& {
	return this->fmap.at(s)();
  }
};

#endif // EventVariables
