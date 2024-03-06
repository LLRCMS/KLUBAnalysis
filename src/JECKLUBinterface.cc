#include "JECKLUBinterface.h"

#define DEBUG false

// Constructor
JECKLUBinterface::JECKLUBinterface (std::string year, bool isPostVFP)
{
  // Get the correct txt file and set the uncertainty names
  std::string TXTfile = "weights/JECregrouped/RegroupedV2_";
  if (year == "2016" && !isPostVFP)
  {
    TXTfile += "Summer19UL16APV_V7_MC_UncertaintySources_AK4PFchs.txt";
    m_jec_sources_regrouped_ =
      {
		"FlavorQCD", "RelativeBal", "HF", "BBEC1", "EC2", "Absolute", "BBEC1_2016",
		"EC2_2016", "Absolute_2016", "HF_2016", "RelativeSample_2016", "Total"
      };
  }
  else if (year == "2016" && isPostVFP)
  {
    TXTfile += "Summer19UL16_V7_MC_UncertaintySources_AK4PFchs.txt";
    m_jec_sources_regrouped_ =
      {
		"FlavorQCD", "RelativeBal", "HF", "BBEC1", "EC2", "Absolute", "BBEC1_2016",
		"EC2_2016", "Absolute_2016", "HF_2016", "RelativeSample_2016", "Total"
      };
  }
  else if (year == "2017")
  {
    TXTfile += "Summer19UL17_V5_MC_UncertaintySources_AK4PFchs.txt";
    m_jec_sources_regrouped_ = {
      "FlavorQCD", "RelativeBal", "HF", "BBEC1", "EC2", "Absolute", "BBEC1_2017",
      "EC2_2017", "Absolute_2017", "HF_2017", "RelativeSample_2017", "Total"
    };
  }
  else if (year == "2018")
  {
    TXTfile += "Summer19UL18_V5_MC_UncertaintySources_AK4PFchs.txt";
    m_jec_sources_regrouped_ = {
      "FlavorQCD", "RelativeBal", "HF", "BBEC1", "EC2", "Absolute", "BBEC1_2018",
      "EC2_2018", "Absolute_2018", "HF_2018", "RelativeSample_2018", "Total"
    };
  }

  std::cout << "** INFO - JECKLUBinterface: " << TXTfile << std::endl;

  // Emplace uncertainties in the provider
  for (const auto& sourceName : m_jec_sources_regrouped_)
  {
    JetCorrectorParameters params(TXTfile, sourceName);
    std::unique_ptr<JetCorrectionUncertainty> uncert(new JetCorrectionUncertainty(params));
    jecSourceUncRegroupedProviders_.emplace(sourceName, std::move(uncert));
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

  // Loop on names (preserves the order) and compute value of uncertainty for each source
  for (auto sourceName : m_jec_sources_regrouped_)
  {
    // up variations
    jecSourceUncRegroupedProviders_[sourceName]->setJetEta(jet.Eta());
    jecSourceUncRegroupedProviders_[sourceName]->setJetPt(jet.Pt());
    double uncertainty_up = jecSourceUncRegroupedProviders_[sourceName]->getUncertainty(true);
    unc_up.push_back(uncertainty_up);

    // down variations
    jecSourceUncRegroupedProviders_[sourceName]->setJetEta(jet.Eta());
    jecSourceUncRegroupedProviders_[sourceName]->setJetPt(jet.Pt());
    double uncertainty_down = jecSourceUncRegroupedProviders_[sourceName]->getUncertainty(false);
    unc_down.push_back(uncertainty_down);

    if (DEBUG) std::cout << "\t" << sourceName << "\t" << uncertainty_up << std::endl;
  }

  // Return pair
  return make_pair(unc_up, unc_down);
}


// Returns the shifted TLorentzVector starting from "index of JEC source" and the original TLorentzVector
// TLorentzVector JECKLUBinterface::getJECUncJet(int idx, TLorentzVector nominalJet, float unc)
// {
//   // Declare the shifted jet
//   TLorentzVector shiftedJet;

//   // Get the name of the source
//   std::string sourceName = m_jec_sources_regrouped_.at(idx);

//   // Get the shift
//   jecSourceUncRegroupedProviders_[sourceName]->setJetEta(nominalJet.Eta());
//   jecSourceUncRegroupedProviders_[sourceName]->setJetPt(nominalJet.Pt());
//   float shift = 1.;
//   if (unc > 0) // up variations
//   {
//     shift = jecSourceUncRegroupedProviders_[sourceName]->getUncertainty(true);
//   }
//   else // down variations
//   {
//     shift = jecSourceUncRegroupedProviders_[sourceName]->getUncertainty(false);
//   }

//   // Shift the jet
//   shiftedJet.SetPtEtaPhiE(
//     (1.+(shift*unc)) * nominalJet.Pt(),
//     nominalJet.Eta(),
//     nominalJet.Phi(),
//     (1.+(shift*unc)) * nominalJet.E()
//     );

//   if (DEBUG)
//   {
//     std::cout << "   Original jet: "; nominalJet.Print();
//     std::cout << "   Shift: " << shift << "  unc: " << unc << std::endl;
//     std::cout << "   Shifted jet : "; shiftedJet.Print();
//   }

//   // Return the shifted jet
//   return shiftedJet;
// }
