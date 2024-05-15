// pnet.cc
#include "pnetSF.h"
#include <iostream>

pnetSF::pnetSF() {}

//scale factors for ParticleNet page 76 http://cms.cern.ch/iCMS/jsp/openfile.jsp?tp=draft&files=AN2021_005_v10.pdf
std::map<std::string, std::vector<float>> pnetSF::getSFmap(float pT_, const std::string& period_){
  std::map<std::string, std::vector<float>> scaleFactors;

  if(period_=="2016preVFP"){
    if (pT_ < 500) {
      scaleFactors["HP"] = {1.054, 0.080, -0.077};
      scaleFactors["MP"] = {1.052, 0.087, -0.081};
      scaleFactors["LP"] = {1.032, 0.096, -0.090};
    } else if (pT_ >= 500 && pT_ < 600) {
      scaleFactors["HP"] = {1.139, 0.083, -0.081};
      scaleFactors["MP"] = {1.068, 0.078, -0.073};
      scaleFactors["LP"] = {1.062, 0.092, -0.082};
    } else if (pT_ >= 600) {
      scaleFactors["HP"] = {1.049, 0.133, -0.130};
      scaleFactors["MP"] = {0.996, 0.101, -0.097};
      scaleFactors["LP"] = {1.002, 0.106, -0.101};
    }
  }
  else if (period_=="2016postVFP") {
    if (pT_ < 500) {
      scaleFactors["HP"] = {1.031, 0.050, -0.046};
      scaleFactors["MP"] = {1.029, 0.051, -0.045};
      scaleFactors["LP"] = {1.031, 0.058, -0.050};
    } else if (pT_ >= 500 && pT_ < 600) {
      scaleFactors["HP"] = {1.055, 0.069, -0.067};
      scaleFactors["MP"] = {1.070, 0.066, -0.062};
      scaleFactors["LP"] = {1.089, 0.076, -0.068};
    } else if (pT_ >= 600) {
      scaleFactors["HP"] = {1.088, 0.076, -0.072};
      scaleFactors["MP"] = {1.077, 0.067, -0.059};
      scaleFactors["LP"] = {1.057, 0.077, -0.056};
    }
  }
  else if (period_=="2017"){
    if (pT_ < 500) {
      scaleFactors["HP"] = {1.055, 0.057, -0.054};
      scaleFactors["MP"] = {1.006, 0.052, -0.052};
      scaleFactors["LP"] = {0.966, 0.055, -0.057};
    } else if (pT_ >= 500 && pT_ < 600) {
      scaleFactors["HP"] = {1.067 , 0.057, -0.055};
      scaleFactors["MP"] = {1.051 , 0.056, -0.055};
      scaleFactors["LP"] = {1.021 , 0.053, -0.052};
    } else if (pT_ >= 600) {
      scaleFactors["HP"] = {1.045 , 0.045, -0.046};
      scaleFactors["MP"] = {0.991 , 0.038, -0.043};
      scaleFactors["LP"] = {0.979 , 0.035, -0.038};
    }
  }
  else if (period_=="2018"){
    if (pT_ < 500) {
      scaleFactors["HP"] = {0.994 , 0.064, -0.064};
      scaleFactors["MP"] = {0.966 , 0.056, -0.057};
      scaleFactors["LP"] = {0.921 , 0.071, -0.077};
    } else if (pT_ >= 500 && pT_ < 600) {
      scaleFactors["HP"] = {1.072 , 0.041, -0.036};
      scaleFactors["MP"] = {1.033 , 0.030, -0.025};
      scaleFactors["LP"] = {1.006 , 0.024, -0.026};
    } else if (pT_ >= 600) {
      scaleFactors["HP"] = {1.046 , 0.038, -0.038};
      scaleFactors["MP"] = {1.010 , 0.030, -0.035};
      scaleFactors["LP"] = {1.001 , 0.035, -0.037};
    }
  }
  return scaleFactors;
}


std::tuple<float, float, float> pnetSF::getSF(const std::map<std::string, std::vector<float>>& result, const std::string& key) {
  if (result.find(key) != result.end()) {
    const std::vector<float>& SF = result.at(key);
    float SFvar = SF[0];
    float SFup = SFvar + SF[1];
    float SFdown = SFvar + SF[2];
    return {SFvar, SFup, SFdown};
  } else {
    std::cerr << "WARNING! ParticleNet scale factors for " << key << " not found in the map. Going to set them to 1" << std::endl;
    return {1.0f, 0.0f, 0.0f};
  }
}
