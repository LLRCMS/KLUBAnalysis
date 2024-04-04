#ifndef TriggerAssigner_h
#define TriggerAssigner_h

#include <iostream>
#include <unordered_map>
#include <string>
#include <algorithm>

class TriggerAssigner {
public:
  TriggerAssigner();
  ~TriggerAssigner(){};

  template <typename T>
  using uMap = std::unordered_map<std::string, T>;

  uMap<bool> assignTriggerRegions(int, std::string,
								  float, float, float, float,
								  float, float, float);

  float getLepton1PtThresh(std::string, std::string);
  float getLepton2PtThresh(std::string, std::string);
	
private:
  uMap<uMap<float>> mLep1Thresh, mLep2Thresh;
  float mEtaThresh = 2.1; // eta threshold in all triggers (when present)
  std::array<std::string, 4> mPeriods = {{"2016preVFP", "2016postVFP", "2017", "2018"}};
  std::array<std::string, 2> mChannels = {{"etau", "mutau"}};

  void mCheckPeriod(std::string, std::string, bool);
  void mCheckChannel(std::string);
};
	
#endif
