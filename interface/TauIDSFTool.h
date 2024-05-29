#ifndef TauIDSFTool_h
#define TauIDSFTool_h

/*
 * @class TauIDSFTool
 *
 * Class to retrieve tau ID SFs.
 *  - pT-dependent SFs for MVAoldDM2017v2
 *  - DM-dependent SFs for MVAoldDM2017v2
 *  - eta-dependent SFs for anti-lepton discriminators
 * Source: https://twiki.cern.ch/twiki/bin/viewauth/CMS/TauIDRecommendation13TeV
 * Inspiration from TauTriggerSFs/src/TauTriggerSFs2017.cc
 *
 * @author Izaak Neutelings (July 2019)
 * updated by Bruno Alves (April 2023)
 * to reflect Run2 UL new corrections for the DeepTauv2p1 algorithm
 * source: https://twiki.cern.ch/twiki/bin/viewauth/CMS/TauIDRecommendationForRun2#Corrections_for_the_DeepTauv2p1

 * CMS Talk threads explaining statistical and systematic uncertainties:
 * - nominal: https://cms-talk.web.cern.ch/t/new-tau-id-scale-factors-for-ul-mc/21688
 * - high pT: https://cms-talk.web.cern.ch/t/highpt-scale-factors/26015
 */

#include <TFile.h>				// TFile
#include <TH1.h>				// TH1
#include <TF1.h>				// TF1
#include <TGraphAsymmErrors.h>  // TGraphAsymmErrors
#include <TString.h>			// Form
#include <string>				// std::string
#include <array>				// std::array
#include <map>					// std::map
#include <stdlib.h>				// getenv
#include <functional>

class TauIDSFTool {

private:
  void embeddedDMcheck(const std::string& ID);
  TFile* ensureTFile(const TString filename);
  TH1* extractTH1(const TFile* file, const std::string& histname);
  const TF1* extractTF1(const TFile* file, const std::string& funcname);
  const TGraphAsymmErrors* extractTGAE(const TFile* file, const std::string& gname);
  void uncertaintyCheck(const std::string& unc) const;
  void yearCheck(const std::string& year) const;
	
protected:

  std::map<const std::string, const TF1*> func;
  std::map<const std::string, const TGraphAsymmErrors*> graph_highpt;
  const TF1* func_extrap;
  TH1* hist;
  [[noreturn]] void disabled() const;

public:
  bool mVerbose = false;
  std::string mDatapath = Form("%s/src/KLUBAnalysis/weights/tau_ID_ScaleFactors_UL",
							  getenv("CMSSW_BASE"));
  std::array<std::string, 7> mYears = {{
	  "2016Legacy", "2017ReReco", "2018ReReco",
	  "UL2016_preVFP", "UL2016_postVFP", "UL2017", "UL2018"}};
  std::array<std::string, 1> mAntiJetIDs = {{"DeepTau2017v2p1VSjet"}};
  std::array<std::string, 1> mAntiEleIDs = {{"DeepTau2017v2p1VSe"}};
  std::array<std::string, 1> mAntiMuIDs  = {{"DeepTau2017v2p1VSmu"}};
  std::array<std::string, 17> mUncertainties = {{"", //nominal
	  "Stat0Up", "Stat0Down", "Stat1Up", "Stat1Down",
	  "SystCorrDMErasUp", "SystCorrDMErasDown",
	  "SystCorrDMUncorrErasUp", "SystCorrDMUncorrErasDown",
	  "SystUncorrDMErasUp", "SystUncorrDMErasDown",
	  "Gt140StatDown", "Gt140StatUp", "Gt140SystCorrErasUp", "Gt140SystCorrErasDown",
	  "Gt140ExtrapUp", "Gt140ExtrapDown"}};
  std::unordered_set<int> mDMs = {0, 1, 10};
  std::vector<int> mGenmatches;
  bool isVsPT  = false;
  bool isVsDM  = false;
  bool isVsEta = false;
  bool isVsDMandPt = false;

  TauIDSFTool(const std::string& year,
			  const std::string& id,
			  const std::string& wp,
			  const bool dm,
			  const bool embedding=false);
  TauIDSFTool(std::string year,
			  const std::string& wp1,
			  const std::string& wp2);
  ~TauIDSFTool() { }

  float getSFvsPT(     double pt,          int genmatch, const std::string& unc="");
  float getSFvsPT(     double pt,                        const std::string& unc="");
  float getSFvsDM(     double pt,  int dm, int genmatch, const std::string& unc="") const;
  float getSFvsDM(     double pt,  int dm,               const std::string& unc="") const;
  float getSFvsEta(    double eta,         int genmatch, const std::string& unc="") const;
  float getSFvsDMandPT(double pt,  int dm, int genmatch, const std::string& unc="");

};

#endif // TauIDSFTool_h
