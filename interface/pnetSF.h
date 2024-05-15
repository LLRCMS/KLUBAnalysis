// pnetSF.h
#ifndef PNET_SF_H
#define PNET_SF_H

#include <map>
#include <string>
#include <vector>

class pnetSF {
public:
  pnetSF();

  std::map<std::string, std::vector<float>> getSFmap(float pT_, const std::string& period_);

  std::tuple<float, float, float> getSF(const std::map<std::string, std::vector<float>>& result, const std::string& key);
};

#endif // PNET_SF_H
