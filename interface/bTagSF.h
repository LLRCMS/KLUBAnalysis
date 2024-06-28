// class to derive event weights for b tag
// for more details check https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods#b_tagging_efficiency_in_MC_sampl

#include <string>
#include <vector>
#include <utility>
#include "BTagCalibrationStandalone.h"
#include "TFile.h"
#include "TH1F.h"

// KLUB libraries
#include "bigTree.h"

class bTagSF
{
public:
  enum WP {loose = 0, medium = 1, tight = 2, reshaping = 3};
  enum SFsyst {central = 0, up = 1, down = 2};
  bTagSF(std::string SFfilename, std::string effFileName, std::string effHistoTag,
		 std::string year, std::string WPset, bool isMC);
  ~bTagSF();
  float getSF (WP wpt, SFsyst syst, int jetFlavor, float pt, float eta, float discr=0.);
  float getSFshifted (std::string systName, int jetFlavor, float pt, float eta, float discr=0.);
  float getEff (WP wpt, int jetFlavor, int channel, float pt, float eta); // FIXME: to do: retrieve MC efficiency from a data format
  void SetWPset(std::string WPset);
  void SetWPset(double loose, double medium, double tight);

  std::vector<float> getEvtWeight (const std::vector<std::pair<float, int>>& jets_and_btag,
								   bigTree &theBigTree, std::map<int,double> jets_and_smearFactor,
								   int channel, SFsyst systWP);
  std::vector<float> getEvtWeightShifted (const std::vector<std::pair<float,int>>& jets_and_btag,
										  bigTree &theBigTree, std::map<int,double> jets_and_smearFactor);

private:
  static constexpr int m_nWP = 3;
  void m_initialize(std::string, std::string, std::string, std::string, std::string);
	
  // related to scale factors
  std::unique_ptr<BTagCalibration> m_calib;
  BTagCalibrationReader m_readers [m_nWP+1]; // [loose, medium, tight, reshaping]

  // related to b tag efficiency
  TFile* m_fileEff = nullptr;
  TH1F* m_hEff [m_nWP][3][4]; // [0: loose, 1: medium, 2: tight] [0: b, 1: c 2: udsg] [0: MuTau, 1: EleTau, :2: TauTau, 3: ALL]

  double _WPtag[m_nWP]; // loose, medium, tight WP

  std::string m_year;
  bool m_isMC;
};
