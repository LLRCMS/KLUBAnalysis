#include "SmearedJetProducer.h"
#include <stdexcept>

#define DEBUG false

// Constructor
SmearedJetProducer::SmearedJetProducer (std::string year, bool doSmearing, int variation, bool isPostVFP)
{
  // Enable the smearing procedure
  enabled_ = doSmearing;

  // Set systematic variation
  if      (variation ==  0) systematic_variation_ = Variation::NOMINAL;
  else if (variation == +1) systematic_variation_ = Variation::UP;
  else if (variation == -1) systematic_variation_ = Variation::DOWN;
  else throw std::invalid_argument("SmearedJetProducer - Invalid value for 'variation' parameter. Only -1, 0 or 1 are supported.");

  // Get the correct txt file and set the uncertainty names
  std::string resolutionFile, scaleFactorFile;
  if (year == "2016")
  {
    if(isPostVFP){
      resolutionFile  = "weights/JERfiles_UL/Summer20UL16_JRV3_MC_PtResolution_AK4PFchs.txt";
      scaleFactorFile = "weights/JERfiles_UL/Summer20UL16_JRV3_MC_SF_AK4PFchs.txt";
    } else {
      resolutionFile  = "weights/JERfiles_UL/Summer20UL16APV_JRV3_MC_PtResolution_AK4PFchs.txt";
      scaleFactorFile = "weights/JERfiles_UL/Summer20UL16APV_JRV3_MC_SF_AK4PFchs.txt";
    }
  }
  else if (year == "2017")
  {
    resolutionFile  = "weights/JERfiles_UL/Summer19UL17_JRV2_MC_PtResolution_AK4PFchs.txt";
    scaleFactorFile = "weights/JERfiles_UL/Summer19UL17_JRV2_MC_SF_AK4PFchs.txt";
  }
  else /* year == "2018" */
  {
    resolutionFile  = "weights/JERfiles_UL/Summer19UL18_JRV2_MC_PtResolution_AK4PFchs.txt";
    scaleFactorFile = "weights/JERfiles_UL/Summer19UL18_JRV2_MC_SF_AK4PFchs.txt";
  }

  std::cout << "** INFO - SmearedJetProducer - resolutionFile : " << resolutionFile << std::endl;
  std::cout << "** INFO - SmearedJetProducer - scaleFactorFile: " << scaleFactorFile << std::endl;

  // Load files in the providers
  resolution_from_file_.reset(new JME::JetResolution(resolutionFile));
  scale_factor_from_file_.reset(new JME::JetResolutionScaleFactor(scaleFactorFile));

  // Initialize genMatching thresholds
  dR_max_ = 0.2;
  dPt_max_factor_ = 3.;
}

// Destructor
SmearedJetProducer::~SmearedJetProducer() {}

// Returns the genJet matched to the reco jet
TLorentzVector SmearedJetProducer::matchedGenJet(TLorentzVector jet,  double resolution, bigTree & theBigTree)
{
  if (DEBUG) { std::cout << "  - jet to match: "; jet.Print(); }

  // Try to find a gen jet matching with:
  // dR  < dR_max_
  // dPt < dPt_max_factor_ * resolution
  double min_dR = std::numeric_limits<double>::infinity();
  TLorentzVector matched_genJet;

  // Loop on genJets
  TLorentzVector genJet;
  for (unsigned int igj = 0; igj < theBigTree.genjet_px->size(); igj++)
  {
    genJet.SetPxPyPzE(theBigTree.genjet_px->at(igj), theBigTree.genjet_py->at(igj), theBigTree.genjet_pz->at(igj), theBigTree.genjet_e->at(igj));

    if (DEBUG) { std::cout << "  - genJet: "; genJet.Print(); }

    double dR = jet.DeltaR(genJet);
    if (dR > min_dR)
      continue;

    if (DEBUG) std::cout << "     dR: " << dR << " < " << dR_max_ << " ? " << (dR < dR_max_) << std::endl;
    if (dR < dR_max_)
    {
      double dPt = std::abs(genJet.Pt() - jet.Pt());
      if (DEBUG) std::cout << "     dPt: " << dPt << " < " << dPt_max_factor_ << " * " << resolution << " ? " << (dPt < dPt_max_factor_ * resolution) << std::endl;
      if (dPt > dPt_max_factor_ * resolution)
        continue;

      min_dR = dR;
      matched_genJet = genJet;
      if (DEBUG)
      {
        std::cout << "     --> min_dR : " << min_dR << std::endl;
        std::cout << "     --> matched: "; matched_genJet.Print();
      }
    }
  }

  if (DEBUG) { std::cout << "  MatchedgenJet: "; matched_genJet.Print(); }
  return matched_genJet;
}

// Returns the smear factor to be applied to the jet TLorentzVector
double SmearedJetProducer::getSmearFactor(TLorentzVector jet, bigTree& theBigTree, bool ptJetCut)
{
  // Declare smear factor
  double smearFactor = 1.;

  // Smearing disabled: return 1.0 (no smearing of the jet)
  if (!enabled_)
  {
    return smearFactor;
  }

  // Get the resolution and SF
  double jet_resolution = resolution_from_file_->getResolution({{JME::Binning::JetPt, jet.Pt()}, {JME::Binning::JetEta, jet.Eta()}, {JME::Binning::Rho, theBigTree.rho}});
  double jer_sf = scale_factor_from_file_->getScaleFactor({{JME::Binning::JetPt, jet.Pt()}, {JME::Binning::JetEta, jet.Eta()}}, systematic_variation_);

  if (DEBUG)
  {
    std::cout << "- SmearedJetProducer - " << std::endl;
    std::cout << " jet: "; jet.Print();
    std::cout << " resolution: " << jet_resolution << std::endl;
    std::cout << " resolution scale factor: " << jer_sf << std::endl;
  }

  // Get the matched genJet
  TLorentzVector genJet = SmearedJetProducer::matchedGenJet(jet, jet.Pt() * jet_resolution, theBigTree);

  // Set the smearing factor according to "hybrid" method described in the twiki
  // https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution
  bool hybridCut = ptJetCut ? genJet.Pt() > 10 : genJet.E() > 0;
  if (hybridCut) // nanoAOD applies this cut
  {
    // Case 1: we have a "good" gen jet matched to the reco jet
    if (DEBUG)
    {
      std::cout << " gen jet:  pt: " << genJet.Pt() << "  eta: " << genJet.Eta() << "  phi: " << genJet.Phi() << "  e: " << genJet.E() << std::endl;
    }
    double dPt = jet.Pt() - genJet.Pt();
    smearFactor = 1 + (jer_sf - 1.) * dPt / jet.Pt();
  }
  else if (jer_sf > 1)
  {
    // Case 2: we don't have a gen jet. Smear jet pt using a random gaussian variation
    double sigma = jet_resolution * std::sqrt(jer_sf * jer_sf - 1);
    if (DEBUG)
    {
      std::cout << " gaussian width: " << sigma << std::endl;
    }
    std::normal_distribution<> d(0, sigma);

    // Initialize random generator with seed dependent on the jet for future reproducibility and sync
    size_t seed = theBigTree.EventNumber + size_t(jet.Pt() * 100) + size_t(std::abs(jet.Eta()) * 100) * 100 + size_t(std::abs(jet.Phi()) * 100) * 10000;
    random_generator_ = std::mt19937_64(seed);

    smearFactor = 1. + d(random_generator_);
  }
  else if (DEBUG)
  {
    std::cout << "Impossible to smear this jet" << std::endl;
  }

  // Negative or too small smearFactor. We would change direction of the jet and this is not what we want.
  // Recompute the smearing factor in order to have jet.energy() == MIN_JET_ENERGY
  if (jet.E() * smearFactor < MIN_JET_ENERGY)
  {
    double newSmearFactor = MIN_JET_ENERGY / jet.E();
    if (DEBUG)
    {
      std::cout << "The smearing factor (" << smearFactor << ") is either negative or too small. Fixing it to " << newSmearFactor << " to avoid change of direction." << std::endl;
    }
    smearFactor = newSmearFactor;
  }

  if (DEBUG)
  {
    std::cout << " smear factor: " << smearFactor << std::endl;
  }

  // Return the smear factor
  return smearFactor;
}

// Returns the smeared TLorentzVector starting from the original TLorentzVector
TLorentzVector SmearedJetProducer::getSmearedJet(TLorentzVector jet, bigTree & theBigTree)
{
  // Get the smear factor
  double smearFactor = SmearedJetProducer::getSmearFactor(jet, theBigTree);

  // Declare the smeared jet and smear it
  TLorentzVector smearedJet = jet * smearFactor;

  if (DEBUG)
  {
    std::cout << " smeared jet (" << smearFactor << "):  pt: " << smearedJet.Pt() << "  eta: " << smearedJet.Eta() << "  phi: " << smearedJet.Phi() << "  e: " << smearedJet.E() << std::endl;
  }

  // Return the smeared jet
  return smearedJet;
}

// Returns the smeared jet starting from the jet index in bigTree
TLorentzVector SmearedJetProducer::getSmearedJetFromIdx(unsigned int jetIdx, bigTree & theBigTree)
{
  // Build jet from jetIdx
  TLorentzVector jet(theBigTree.jets_px->at(jetIdx), theBigTree.jets_py->at(jetIdx), theBigTree.jets_pz->at(jetIdx), theBigTree.jets_e ->at(jetIdx));

  // Get the smear factor
  double smearFactor = SmearedJetProducer::getSmearFactor(jet, theBigTree);

  // Declare the smeared jet and smear it
  TLorentzVector smearedJet = jet * smearFactor;

  if (DEBUG)
  {
    std::cout << " smeared jet (" << smearFactor << "):  pt: " << smearedJet.Pt() << "  eta: " << smearedJet.Eta() << "  phi: " << smearedJet.Phi() << "  e: " << smearedJet.E() << std::endl;
  }

  // Return the smeared jet
  return smearedJet;
}

double SmearedJetProducer::getResolution(unsigned int jetIdx, bigTree & theBigTree)
{
  // Build jet from jetIdx
  TLorentzVector jet(theBigTree.jets_px->at(jetIdx), theBigTree.jets_py->at(jetIdx), theBigTree.jets_pz->at(jetIdx), theBigTree.jets_e ->at(jetIdx));

  // Get resolution
  double jet_resolution = resolution_from_file_->getResolution({{JME::Binning::JetPt, jet.Pt()}, {JME::Binning::JetEta, jet.Eta()}, {JME::Binning::Rho, theBigTree.rho}});

  return jet_resolution;
}
