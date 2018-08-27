#ifndef TauTriggerSFs2017_h
#define TauTriggerSFs2017_h

/** \class TauTriggerSFs2017
 *
 * Class to access information on tau trigger efficiencies in data and MC and related data/MC scale-factors.
 * The trigger efficiencies are parametrized as function of pT, eta, and phi of the offline reconstructed tau,
 * cf. https://indico.cern.ch/event/700928/contributions/2883477/attachments/1596523/2529036/Ruggles_TauTriggers_TauPOG_20180207_v4.pdf 
 *
 * \authors Tyler Ruggles, Wisconsin; Christian Veelken, Tallin
 *
 */

#include <TFile.h> // TFile
#include <TH1.h> // TH1
#include <TH2.h> // TH2

#include <string> // std::string

class TauTriggerSFs2017
{
public:

  enum { kCentral, kStatUp, kStatDown };

  TauTriggerSFs2017(const std::string& inputFileName, const std::string& inputFileNameOld, const std::string& tauWP = "medium", const std::string& wpType = "MVA");
  ~TauTriggerSFs2017();

  /**
   * @brief Funtions to access efficiencies in data
   * @return Values of efficiencies
   */
  // This is the efficiency for a single leg of the di-tau trigger
  double getDiTauEfficiencyData(double pt, double eta, double phi, int central_or_shift = TauTriggerSFs2017::kCentral);
  // This is the efficiency for the tau leg of the mu-tau trigger
  double getMuTauEfficiencyData(double pt, double eta, double phi, int central_or_shift = TauTriggerSFs2017::kCentral);
  // This is the efficiency for the tau leg of the e-tau trigger
  double getETauEfficiencyData(double pt, double eta, double phi, int central_or_shift = TauTriggerSFs2017::kCentral);

  /**
   * @brief Funtions to access efficiencies in MC
   * @return Values of efficiencies
   */
  // This is the efficiency for a single leg of the di-tau trigger
  double getDiTauEfficiencyMC(double pt, double eta, double phi, int central_or_shift = TauTriggerSFs2017::kCentral);
  // This is the efficiency for the tau leg of the mu-tau trigger
  double getMuTauEfficiencyMC(double pt, double eta, double phi, int central_or_shift = TauTriggerSFs2017::kCentral);
  // This is the efficiency for the tau leg of the e-tau trigger
  double getETauEfficiencyMC(double pt, double eta, double phi, int central_or_shift = TauTriggerSFs2017::kCentral);

  /**
   * @brief Funtions to access data/MC scale-factors
   * @return Values of scale-factors
   */ 
  // This is the SF for a single leg of the di-tau trigger
  double getDiTauScaleFactor(double pt, double eta, double phi, int central_or_shift = TauTriggerSFs2017::kCentral);
  // This is the SF for the tau leg of the mu-tau trigger
  double getMuTauScaleFactor(double pt, double eta, double phi, int central_or_shift = TauTriggerSFs2017::kCentral);
  // This is the SF for the tau leg of the e-tau trigger
  double getETauScaleFactor(double pt, double eta, double phi, int central_or_shift = TauTriggerSFs2017::kCentral);

protected:
  std::string inputFileName_;
  TFile* inputFile_;
  std::string inputFileNameOld_;
  TFile* inputFileOld_;

  std::string tauWP_;
  std::string wpType_;

  const TH1* diTauData_;
  const TH1* diTauMC_;
  const TH1* eTauData_;
  const TH1* eTauMC_;
  const TH1* muTauData_;
  const TH1* muTauMC_;

  const TH2* diTauEtaPhiData_;
  const TH2* diTauEtaPhiMC_;
  const TH2* eTauEtaPhiData_;
  const TH2* eTauEtaPhiMC_;
  const TH2* muTauEtaPhiData_;
  const TH2* muTauEtaPhiMC_;

  const TH2* diTauEtaPhiAvgData_;
  const TH2* diTauEtaPhiAvgMC_;
  const TH2* eTauEtaPhiAvgData_;
  const TH2* eTauEtaPhiAvgMC_;
  const TH2* muTauEtaPhiAvgData_;
  const TH2* muTauEtaPhiAvgMC_;
};

#endif // TauTriggerSFs2017_h

