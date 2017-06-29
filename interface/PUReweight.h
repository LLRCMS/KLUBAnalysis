#ifndef PUReweight_h
#define PUReweight_h

#include <TH1F.h>
#include <string>

class PUReweight {
public:
  enum Type {NONE=0, RUN1ANALYSIS=1, RUN2ANALYSIS=2};

  PUReweight(Type type=RUN2ANALYSIS); //default RUN2Analysis 

  ~PUReweight();

  /// Get weights for a sample of MC 
  /// to reproduce conditions of target
  float weight(int MC, int target, int input);

public:
  Type theType;

  // 2016 Samples
  std::vector<float> hT2016_MCSpring16_Data2c6fb ;
  std::vector<float> hT2016_MCSpring16_Data4c0fb ;
  std::vector<float> hT2016_MCSpring16_Data9c2fb ;
  std::vector<float> hT2016_MCSpring16_Data12c9fb ;
  std::vector<float> hT2016_MCSpring16_Data36c4fb ;
  std::vector<float> hT2016_MCSummer16_Data36c4fb ;

  // 2015 Samples
  std::vector<float> hT2015_MC25ns_Data25ns ;
  std::vector<float> hT2015_MC25ns_Data50ns ;
  std::vector<float> hT2015_MC50ns_Data25ns ;
  std::vector<float> hT2015_MC50ns_Data50ns ;

  // 2011 Samples
  std::vector<float> hT2011 ;
  
  // 2012 Samples
  std::vector<float> hT2012 ;
  
};
#endif
