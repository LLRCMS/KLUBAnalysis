#include "JECKLUBinterface.h"

#define DEBUG false

// Constructor
JECKLUBinterface::JECKLUBinterface (std::string year, std::string version)
{
//  - dichiaro i nomi in base all anno (m_jec_sources_regrouped_2016) e il provider (jecSourceUncRegroupedProviders)
//  - loop sui nomi per riempire il provider con le unc

  // Get the correct txt file and set the uncertainty names
  std::string TXTfile;
  if (year == "2016")
  {
    TXTfile = "weights/JECregrouped/Regrouped_Summer16_07Aug2017_V11_MC_UncertaintySources_AK4PFchs.txt";
    m_jec_sources_regrouped_ = 
    {
      "FlavorQCD", "RelativeBal", "HF", "BBEC1", "EC2", "Absolute", "BBEC1_2016", 
      "EC2_2016", "Absolute_2016", "HF_2016", "RelativeSample_2016" //, "Total"
    };
  }
  else if (year == "2017")
  {
    TXTfile = "weights/JECregrouped/Regrouped_Fall17_17Nov2017_V32_MC_UncertaintySources_AK4PFchs.txt";
    m_jec_sources_regrouped_ = {
      "FlavorQCD", "RelativeBal", "HF", "BBEC1", "EC2", "Absolute", "BBEC1_2017",
      "EC2_2017", "Absolute_2017", "HF_2017", "RelativeSample_2017" //, "Total"
    };
  }
  else /* year == "2018" */
  {
    TXTfile = "weights/JECregrouped/Regrouped_Autumn18_V19_MC_UncertaintySources_AK4PFchs.txt";
    m_jec_sources_regrouped_ = {
      "FlavorQCD", "RelativeBal", "HF", "BBEC1", "EC2", "Absolute", "BBEC1_2018",
      "EC2_2018", "Absolute_2018", "HF_2018", "RelativeSample_2018" //, "Total"
    };
  }

  // Change input file if using V2
  if (version == "V2")
  {
    TXTfile = std::regex_replace(TXTfile, std::regex("Regrouped_"), "RegroupedV2_");
  }

  std::cout << "** INFO - JECKLUBinterface: " << TXTfile << std::endl;

  // Emplace uncertainties in the provider
  for (const auto& sourceName : m_jec_sources_regrouped_)
  {
    JetCorrectorParameters source_parameters_reduced(TXTfile, sourceName);
    std::unique_ptr<JetCorrectionUncertainty> source_uncertainty_reduced(new JetCorrectionUncertainty(source_parameters_reduced));
    jecSourceUncRegroupedProviders_.emplace(sourceName, std::move(source_uncertainty_reduced));
  }

  if (DEBUG) 
  {
    std::cout << "   JEC SourceNames:";
    for (auto sourceName : m_jec_sources_regrouped_) std::cout << " " << sourceName;
    std::cout << std::endl;
  }
}


// Returns a pair of vectors (up_variations vector, down_variations vector)
std::pair<std::vector<double>,std::vector<double>> JECKLUBinterface::getJECUncVectors(int idx, bigTree & theBigTree)
{
  // Vectors to store uncertainties
  std::vector<double> unc_up;
  std::vector<double> unc_down;

  // Create TLorentzVector of the jet
  TLorentzVector jet(
    theBigTree.jets_px->at(idx),
    theBigTree.jets_py->at(idx),
    theBigTree.jets_pz->at(idx),
    theBigTree.jets_e->at(idx)
  );

  // Loop on provider (i.e. on all sources) and compute value of uncertainty for each source
  for (auto sourceName : m_jec_sources_regrouped_)
  {
    // up variations
    jecSourceUncRegroupedProviders_[sourceName]->setJetEta(jet.Eta());
    jecSourceUncRegroupedProviders_[sourceName]->setJetPt(jet.Pt());
    double uncertainty_up = jecSourceUncRegroupedProviders_[sourceName]->getUncertainty(true);
    unc_up.push_back(uncertainty_up);

    jecSourceUncRegroupedProviders_[sourceName]->setJetEta(jet.Eta());
    jecSourceUncRegroupedProviders_[sourceName]->setJetPt(jet.Pt());
    double uncertainty_down = jecSourceUncRegroupedProviders_[sourceName]->getUncertainty(false);
    unc_down.push_back(uncertainty_down);

    if (DEBUG) std::cout << "\t" << sourceName << "\t" << uncertainty_up << std::endl;
  }

  // Return pair
  return make_pair(unc_up, unc_down);
}


// Returns directly the shifted TlorentzVector starting from "index of JEC source" and the original TLorentzVector
// To be used (mainly) in skimOutputter*.cpp
//TLorentzVector JECKLUBinterface::getJECUncJet(int idx, TLorentzVector bjet1)
//{}


