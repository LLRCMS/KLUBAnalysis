#ifndef TriggerSF_h
#define TriggerSF_h

// -------------------------------------------------------------------------------------------------------------- //
//                                                                                                                //
//   class TriggerSF                                                                                              //
//                                                                                                                //
//   Class to compute SF for the union of all triggers in the analysis, using the general inclusion method.       //
//   Reference: Lendermann V, Haller J, Herbst M, Krüger K, Schultz-Coulon HC, Stamen R.                          //
//              Combining Triggers in HEP data analysis.                                                          // 
//              Nucl Instruments Methods Phys Res Sect A Accel Spectrometers, Detect Assoc Equip. 2009;           // 
//              604(3):707-718.                                                                                   //
//              doi:10.1016/j.nima.2009.03.173                                                                    //
//                                                                                                                // 
//   Standalone Python framework: https://github.com/b-fontana/METTriggerStudies                                  //
//   - Triggers are named differently and already include effects from different runs, HPS, Data/MC diffs, ...    //
//                                                                                                                //
//   Author: Bruno Alves (LLR, Ecole Polytechnique, Paris)                                                        //
//   Date  : April 2022                                                                                           //
//   Contact: bruno.alves@cern.ch                                                                                 //
//                                                                                                                //
// -------------------------------------------------------------------------------------------------------------- //

// Standard libraries
#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>
#include <stdexcept>
#include <limits>
#include <string>
#include <boost/algorithm/string.hpp>

// KLUB libraries
#include "nested_map.h"
#include "VectorCombinations.h"
#include "TriggerChannelLists.h"
#include "EventVariables.h"

// ROOT libraries
#include "TFile.h"
#include "TH1F.h"

// External libraries
#include<fstream>
#include<json.h> //https://github.com/nlohmann/json


// TriggerSF class
class TriggerSF {
private:
  using json = nlohmann::json;
  using mPair = std::pair<float,float>;
  template<typename T> using vec = std::vector<T>;
  template<typename T> using vec2 = vec<vec<T>>;
  template<typename K, typename V> using umap = std::unordered_map<K,V>;

  // Contains histogram values. Skips under and overflows.
  // The first (last) bin in each axis is zero (nbins-1).
  // (x,y,z) values are stored in a 1D vector.
  // Consecutive x bins are stored in adjacent memory locations.
  class EffValue
  {
  public:
	EffValue(TH1F* h)
	{
	  mNBinsX = h->GetNbinsX();
	  mNBinsY = h->GetNbinsY();
	  mNBinsZ = h->GetNbinsZ();
	  mXAxis = h->GetXaxis();
	  mYAxis = h->GetYaxis();
	  mZAxis = h->GetZaxis();
	  this->fill(h);
	}

	//currently supporting only three dimensions
	//variadic approach could work but seems an overkill
	auto getVal(float xvar,
				float yvar=-std::numeric_limits<float>::max(),
				float zvar=-std::numeric_limits<float>::max()) const -> float
	{
	  unsigned xbin = 0, ybin = 0, zbin = 0;
	  try {
		xbin = mGetBinNumber(mNBinsX, mXAxis, xvar);
	  }
	  catch (std::underflow_error const& e) {
		throw std::underflow_error(std::string(e.what()) + " (X axis)");
	  }
	  catch (std::overflow_error const& e) {
		throw std::underflow_error(std::string(e.what()) + " (X axis)");
	  }
	  catch (std::invalid_argument const& e) {
		std::cerr << std::string(e.what()) + " (X axis)" << std::endl;
	  }

	  try {
		ybin = yvar==-std::numeric_limits<float>::max() ? 0 : mGetBinNumber(mNBinsY, mYAxis, yvar);
	  }
	  catch (std::underflow_error const& e) {
		throw std::underflow_error(std::string(e.what()) + " (Y axis)");
	  }
	  catch (std::overflow_error const& e) {
		throw std::underflow_error(std::string(e.what()) + " (Y axis)");
	  }
	  catch (std::invalid_argument const& e) {
		std::cerr << std::string(e.what()) + " (Y axis)" << std::endl;
	  }

	  try {
		zbin = zvar==-std::numeric_limits<float>::max() ? 0 : mGetBinNumber(mNBinsZ, mZAxis, zvar);
	  }
	  catch (std::underflow_error const& e) {
		throw std::underflow_error(std::string(e.what()) + " (Z axis)");
	  }
	  catch (std::overflow_error const& e) {
		throw std::underflow_error(std::string(e.what()) + " (Z axis)");
	  }
	  catch (std::invalid_argument const& e) {
		std::cerr << std::string(e.what()) + " (Z axis)" << std::endl;
	  }

	  return mGetValByBin(xbin, ybin, zbin);
	}
	
	auto getErrUp(float xvar,
				  float yvar=-std::numeric_limits<float>::max(),
				  float zvar=-std::numeric_limits<float>::max()) const -> float
	{
	  unsigned xbin = mGetBinNumber(mNBinsX, mXAxis, xvar);
	  unsigned ybin = yvar==-std::numeric_limits<float>::max() ? 0 : mGetBinNumber(mNBinsY, mYAxis, yvar);
	  unsigned zbin = zvar==-std::numeric_limits<float>::max() ? 0 : mGetBinNumber(mNBinsZ, mZAxis, zvar);
	  return mGetErrUpByBin(xbin, ybin, zbin);
	}
	auto getErrLow(float xvar,
				   float yvar=-std::numeric_limits<float>::max(),
				   float zvar=-std::numeric_limits<float>::max()) const -> float
	{
	  unsigned xbin = mGetBinNumber(mNBinsX, mXAxis, xvar);
	  unsigned ybin = yvar==-std::numeric_limits<float>::max() ? 0 : mGetBinNumber(mNBinsY, mYAxis, yvar);
	  unsigned zbin = zvar==-std::numeric_limits<float>::max() ? 0 : mGetBinNumber(mNBinsZ, mZAxis, zvar);
	  return mGetErrLowByBin(xbin, ybin, zbin);
	}

  private:
	unsigned mNBinsX=0, mNBinsY=0, mNBinsZ=0;
	TAxis *mXAxis, *mYAxis, *mZAxis;
	
	typedef std::vector<float> type;
	type mEff;
	type mErrUp;
	type mErrLow;

	inline const unsigned mGetLinearIndex(unsigned xbin, unsigned ybin, unsigned zbin) const
	{
	  return xbin + mNBinsX*ybin + mNBinsX*mNBinsY*zbin;
	}
	
	void fill(TH1F* h)
	{
	  for(unsigned i=0; i<=mNBinsZ; ++i) {
		for(unsigned j=0; j<=mNBinsY; ++j) {
		  for(unsigned k=0; k<=mNBinsX; ++k) {
			unsigned bin = h->GetBin(k,j,i);
			if (h->IsBinUnderflow(bin) or h->IsBinOverflow(bin))
			  continue;

			this->mEff.push_back( h->GetBinContent(bin) );
			this->mErrUp.push_back( h->GetBinErrorUp(bin) );
			this->mErrLow.push_back( h->GetBinErrorLow(bin) );
		  }
		}
	  }
	}

	auto mGetBinNumber(unsigned nbins, TAxis *ax, float var) const -> unsigned
	{
	  if (var < ax->GetBinLowEdge(1)) {
		std::string mess = "[WARNING TriggerSF.h] Underflow ";
		mess += std::to_string(var) + " < ";
		mess += std::to_string(ax->GetBinLowEdge(1)) + ".";
	    throw std::underflow_error(mess);
	  }
	  if (var > ax->GetBinUpEdge(nbins)) {
		std::string mess = "[WARNING TriggerSF.h] Overflow ";
		mess += std::to_string(var) + " > ";
		mess += std::to_string(ax->GetBinUpEdge(nbins)) + ".";
	    throw std::overflow_error(mess);
	  }

	  for(unsigned i=1; i<=nbins; ++i) {
		if(ax->GetBinLowEdge(i)<var and ax->GetBinUpEdge(i)>=var)
		  return i-1;
	  }
	  std::string m = ( "Should not get to this point. Var " +
						std::to_string(var) );
	  throw std::invalid_argument(m);
	}

	auto mGetValByBin(unsigned xbin, unsigned ybin=0, unsigned zbin=0) const -> float
	{
	  const unsigned index = mGetLinearIndex(xbin, ybin, zbin);
	  return this->mEff[index];
	}
	auto mGetErrUpByBin(unsigned xbin, unsigned ybin=0, unsigned zbin=0) const -> float
	{
	  const unsigned index = mGetLinearIndex(xbin, ybin, zbin);
	  return this->mErrUp[index];
	}
	auto mGetErrLowByBin(unsigned xbin, unsigned ybin=0, unsigned zbin=0) const -> float
	{
	  const unsigned index = mGetLinearIndex(xbin, ybin, zbin);
	  return this->mErrLow[index];
	}

  }; // class EffValue
  
  //first: MC; second: Data (per channel)
  umap< std::string, std::pair< vec<std::string>,
								vec<std::string> > > mInters;
  
  nested_map<std::string, EffValue, 4> mValues;

  // one file per channel
  umap<std::string, TFile*> mEffFiles;
  umap<std::string, json> mVarFiles;

  const std::set<std::string> mChannelsAllowed = {"EleTau", "MuTau", "TauTau"};
  const std::string mTriggerStrConnector = "_PLUS_";

  auto mBuildObjectName(std::string varname,
						std::string trigInters,
						bool isData) const -> std::string;

  auto mCheckChannel(std::string channel) const -> void;

  auto mCheckExtension(std::string str, std::string ext) -> void;

  auto mCountNumberTriggerItems(std::string str) const -> const int;
  
  auto mGetTriggerIntersections( const TriggerChannelLists& list,
								 std::string channel,
								 const bool isData ) -> const vec<std::string>;

  auto mExceptionPrint(const std::runtime_error& e,
					   std::string var, std::string chn,
					   std::string inters, bool isData) const -> void;

  auto mGetUnionEfficiency( const EventVariables& vars,
							std::string channel,
							const bool isData ) -> const float;
  auto mGetUnionEfficiencyErrors( const EventVariables& vars,
								  std::string channel,
								  const bool isData ) -> const mPair;

  auto mGetIntersectionEfficiencies( std::string channel,
									 std::string trigInters,
									 const bool isData ) -> const EffValue&;

  auto mEffVariablesToVarName( const vec2<std::string>& effVars ) -> std::string;

  auto mKLUBStandaloneNameMatching(vec<std::string> old_strs,
								   std::string channel,
								   const bool isData ) -> const vec<std::string>;

  auto mMCOrDataIntersections( std::string channel, const bool isData ) -> vec<std::string>;

  auto mReadIntersectionEfficiencies( std::string channel,
									  std::string trigInters,
									  std::string varname,
									  const bool isData ) -> EffValue;
  auto mTest() const -> bool;

  auto toStr(bool b) const -> std::string;
  
public:
  TriggerSF(TriggerChannelLists triggers,
			umap<std::string,std::string> eff_files,
			umap<std::string,std::string> var_files, unsigned year);
  ~TriggerSF();

  auto getEvtWeight( const EventVariables& vars,
					 std::string channel ) -> const float;
  auto getEvtWeightErrors( const EventVariables& vars,
						   std::string channel ) -> const mPair;

};

#endif // TriggerSF
