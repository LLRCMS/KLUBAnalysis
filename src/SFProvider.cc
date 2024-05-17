#include "../interface/SFProvider.h"

#include <algorithm>
#include <sstream>

namespace tau_trigger {

  const std::set<int> SFProvider::supported_decay_modes = { 0, 1, 10, 11 };

  SFProvider::SFProvider(std::string_view input_file, std::string_view channel, std::string_view wp)
  {
    TFile root_file(input_file.data(), "READ");
    if(root_file.IsZombie()) {
      std::ostringstream ss;
      ss << "tau_trigger::SFProvider: unable to open \"" << input_file << "\".";
      throw std::runtime_error(ss.str());
    }

    std::map<std::string, std::map<int, std::unique_ptr<TH1F>>*> histograms = {
      { "data", &eff_data }, { "mc", &eff_mc }, { "sf", &sf }
    };
    for(int dm : supported_decay_modes) {
      for(const auto& entry : histograms) {
		std::ostringstream ss_hist_name;
		// For the vbf ditau trigger, the efficiencies of the 3 prong decay modes are merged.
		if (channel == "ditauvbf" && (dm == 10 || dm == 11))
		  {
			ss_hist_name << entry.first << "_" << channel << "_" << wp << "_dm1011_fitted";
		  }
		else
		  {
			ss_hist_name << entry.first << "_" << channel << "_" << wp << "_dm" << dm << "_fitted";
		  }
		(*entry.second)[dm].reset(LoadHistogram(root_file, ss_hist_name.str()));
      }
    }
  }

  float SFProvider::getEfficiencyData(float tau_pt, int tau_dm, int unc_scale) const
  {
    tau_dm = CheckDM(tau_dm);
    return FindBinValue(*eff_data.at(tau_dm), tau_pt, unc_scale);
  }

  float SFProvider::getEfficiencyMC(float tau_pt, int tau_dm, int unc_scale) const
  {
    tau_dm = CheckDM(tau_dm);
    return FindBinValue(*eff_mc.at(tau_dm), tau_pt, unc_scale);
  }

  float SFProvider::getSF(float tau_pt, int tau_dm, int unc_scale) const
  {
    tau_dm = CheckDM(tau_dm);
    return FindBinValue(*sf.at(tau_dm), tau_pt, unc_scale);
  }

  TH1F* SFProvider::LoadHistogram(TFile& file, std::string_view name)
  {
    TH1F* hist = dynamic_cast<TH1F*>(file.Get(name.data()));
    if(!hist) {
      std::ostringstream ss;
      ss << "tau_trigger::SFProvider: unable to load \"" << name << "\" from the root file \""
	 << file.GetName() << "\".";
      throw std::runtime_error(ss.str());
    }
    TH1F* cloned_hist = dynamic_cast<TH1F*>(hist->Clone());
    cloned_hist->SetDirectory(nullptr);
    return cloned_hist;
  }

  float SFProvider::FindBinValue(const TH1F& hist, float x, int unc_scale)
  {
    int bin = hist.FindFixBin(x);
    //bin = std::clamp(bin, 1, hist.GetNbinsX()); // needs C++17
    bin = std::min(hist.GetNbinsX(), std::max(1, bin));
    return hist.GetBinContent(bin) + unc_scale * hist.GetBinError(bin);
  }

  int SFProvider::CheckDM(int tau_dm)
  {
    if(tau_dm == 2) tau_dm = 1;
    if(!supported_decay_modes.count(tau_dm)) {
      std::ostringstream ss;
      ss << "tau_trigger::SFProvider: decay mode = " << tau_dm << " is not supported.";
      throw std::runtime_error(ss.str());
    }
    return tau_dm;
  }

} // namespace tau_trigger
