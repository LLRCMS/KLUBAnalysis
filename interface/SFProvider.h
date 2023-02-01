#pragma once

#include <set>
#include <map>
#include <TFile.h>
#include <TH1F.h>

namespace tau_trigger {
  class SFProvider {
  public:
    static const std::set<int> supported_decay_modes;

    SFProvider(std::string_view input_file, std::string_view channel, std::string_view wp);

    float getEfficiencyData(float tau_pt, int tau_dm, int unc_scale = 0) const;
    float getEfficiencyMC(float tau_pt, int tau_dm, int unc_scale = 0) const;
    float getSF(float tau_pt, int tau_dm, int unc_scale = 0) const;

  private:
    static TH1F* LoadHistogram(TFile& file, std::string_view name);
    static float FindBinValue(const TH1F& hist, float x, int unc_scale);
    static int CheckDM(int tau_dm);

  private:
    std::map<int, std::unique_ptr<TH1F>> eff_data, eff_mc, sf;
  };
} // namespace tau_trigger
