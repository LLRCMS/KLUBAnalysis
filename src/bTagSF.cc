#include "bTagSF.h"
#include "TLorentzVector.h"
#include "TString.h"
#include "TMath.h"
#include <stdio.h>
#include <iostream>

using namespace std;

#define DEBUG false

bTagSF::bTagSF(std::string SFfilename, std::string effFileName, std::string effHistoTag, std::string year, std::string WPset) :

  m_calib("DeepCSV", SFfilename.c_str()) ,
  m_year (year)
{
  // Fill m_readers varray with year dependent names for reshaping uncertainties
  m_readers[0] = BTagCalibrationReader(BTagEntry::OP_LOOSE,  "central", {"up", "down"});
  m_readers[1] = BTagCalibrationReader(BTagEntry::OP_MEDIUM, "central", {"up", "down"});
  m_readers[2] = BTagCalibrationReader(BTagEntry::OP_TIGHT,  "central", {"up", "down"});
  m_readers[3] = BTagCalibrationReader(BTagEntry::OP_RESHAPING, "central", {
      "up_jes", "up_lf", "up_hf", "up_hfstats1", "up_hfstats2", "up_lfstats1", "up_lfstats2", "up_cferr1", "up_cferr2",
	"down_jes", "down_lf", "down_hf", "down_hfstats1", "down_hfstats2", "down_lfstats1", "down_lfstats2", "down_cferr1", "down_cferr2",
	// Up/down JES variations: do not change order of these!!!
	"up_jesFlavorQCD", "up_jesRelativeBal", "up_jesHF", "up_jesBBEC1", "up_jesEC2", "up_jesAbsolute", "up_jesBBEC1_"+m_year, "up_jesEC2_"+m_year,
	"up_jesAbsolute_"+m_year, "up_jesHF_"+m_year, "up_jesRelativeSample_"+m_year,
	"down_jesFlavorQCD", "down_jesRelativeBal", "down_jesHF", "down_jesBBEC1", "down_jesEC2", "down_jesAbsolute", "down_jesBBEC1_"+m_year,
	"down_jesEC2_"+m_year, "down_jesAbsolute_"+m_year, "down_jesHF_"+m_year, "down_jesRelativeSample_"+m_year
	});

  // load readers [loose, medium, tight, reshaping]
  m_readers[0].load(m_calib, BTagEntry::FLAV_B, "comb");
  m_readers[0].load(m_calib, BTagEntry::FLAV_C, "comb");
  m_readers[0].load(m_calib, BTagEntry::FLAV_UDSG, "incl");

  m_readers[1].load(m_calib, BTagEntry::FLAV_B, "comb");
  m_readers[1].load(m_calib, BTagEntry::FLAV_C, "comb");
  m_readers[1].load(m_calib, BTagEntry::FLAV_UDSG, "incl");

  m_readers[2].load(m_calib, BTagEntry::FLAV_B, "comb");
  m_readers[2].load(m_calib, BTagEntry::FLAV_C, "comb");
  m_readers[2].load(m_calib, BTagEntry::FLAV_UDSG, "incl");

  m_readers[3].load(m_calib, BTagEntry::FLAV_B, "iterativefit");
  m_readers[3].load(m_calib, BTagEntry::FLAV_C, "iterativefit");
  m_readers[3].load(m_calib, BTagEntry::FLAV_UDSG, "iterativefit");

  m_fileEff = new TFile (effFileName.c_str());

  TString flavs[3]   = {"b", "c", "udsg"};
  TString chnames[4] = {"MuTau", "EleTau", "TauTau", "ALL"};
  TString WPnames[3] = {"L", "M", "T"};

  for (int iWP = 0; iWP < 3; iWP++)
  {
    for (int flav = 0; flav < 3; flav++)
    {
      for (int channel = 0; channel < 4; channel++)
      {
	TString name = Form("eff_%s_%s_%s", flavs[flav].Data(), WPnames[iWP].Data(), chnames[channel].Data());
	//cout << "Tmps name " << name << endl;
	m_hEff [iWP][flav][channel] = (TH1F*) m_fileEff->Get(name.Data());
	//cout << " --> " << m_hEff [iWP][flav][channel] -> GetName() << endl;
      }
    }
  }

  SetWPset(WPset);
}

bTagSF::~bTagSF()
{
  if (m_fileEff) delete m_fileEff;
}

void bTagSF::SetWPset(std::string WPset)
{
  // Legacy WP
  if      (WPset == "80X_DeepCSV_V1")
    SetWPset(0.2217, 0.6321, 0.8953);

  else if (WPset == "80X_DeepFlavor_V1")
    SetWPset(0.0614, 0.3093, 0.7221);

  else if (WPset == "94X_DeepCSV_V1")
    SetWPset(0.1522, 0.4941, 0.8001);

  else if (WPset == "94X_DeepFlavor_V1")
    SetWPset(0.0521, 0.3033, 0.7489);

  else if (WPset == "102X_DeepCSV_V1")
    SetWPset(0.1241, 0.4184, 0.7527);

  else if (WPset =="102X_DeepFlavor_V1")
    SetWPset(0.0494, 0.2770, 0.7264);
  
  // UL WP
  else if (WPset == "106X18_DeepCSV_V1")
    SetWPset(0.1208, 0.4168, 0.7665);
  else if (WPset =="106X18_DeepFlavor_V1")
    SetWPset(0.0490, 0.2783, 0.7100);

  else if (WPset == "106X17_DeepCSV_V1")
    SetWPset(0.1355, 0.4506, 0.7738);
  else if (WPset =="106X17_DeepFlavor_V1")
    SetWPset(0.0532, 0.3040, 0.7476);

  else if (WPset == "106X16postVFP_DeepCSV_V1")
    SetWPset(0.1918, 0.5847, 0.8767);
  else if (WPset =="106X16postVFP_DeepFlavor_V1")
    SetWPset(0.0480, 0.2489, 0.6377);

  else if (WPset == "106X16preVFP_DeepCSV_V1")
    SetWPset(0.2027, 0.6001, 0.8819);
  else if (WPset =="106X16preVFP_DeepFlavor_V1")
    SetWPset(0.0508, 0.2598, 0.6502);

  else
  {
    cout << "bTagSF :: WARNING! SF set string " << WPset << " not recognized, going to use values from 80X_DeepCSV_V1" << endl;
    SetWPset(0.2217, 0.6321, 0.8953);
  }

  printf("bTagSF :: info : WP set is %s i.e.;%.3f %.3f %.3f\n", WPset.c_str(), _WPtag[0], _WPtag[1], _WPtag[2]);
}

void bTagSF::SetWPset(double loose, double medium, double tight)
{
  _WPtag[0] = loose;
  _WPtag[1] = medium;
  _WPtag[2] = tight;
}

float bTagSF::getSF (WP wpt, SFsyst syst, int jetFlavor, float pt, float eta, float discr)
{
  // cout << "   ~~ requesting SF for WP=" << wpt << " SFsyst=" << syst << " jetFlavor=" << jetFlavor << " pt=" << pt << " eta=" << eta << endl;

  float SF = 1.0;

  BTagEntry::JetFlavor flav;
  int mySystIndex = (int) syst;
  int myWPIndex   = (int) wpt;
  float mypt = pt;
  if (mypt < 20.) mypt = 20.;
  if (mypt > 1000.) mypt = 1000.;

  if (abs(jetFlavor) == 5)
  {
    flav = BTagEntry::FLAV_B;
  }
  else if (abs(jetFlavor) == 4)
  {
    flav = BTagEntry::FLAV_C;
  }
  else
  {
    flav = BTagEntry::FLAV_UDSG;
  }

  if (DEBUG) cout << "   ~~ getSF(): requesting SF for WP=" << wpt << "," << myWPIndex << " SFsyst=" << syst << "," << mySystIndex << " jetFlavor=" << jetFlavor <<"("<<flav<<")"<< " pt=" << pt << " eta=" << eta << endl;

  string systName[3] = {"central", "up", "down"}; // like SFsyst enum;
  SF = m_readers[myWPIndex].eval_auto_bounds(systName[mySystIndex], flav, eta, pt, discr);

  if (DEBUG) cout << "   ~~ returning " << SF << endl;
  return SF;
}

float bTagSF::getSFshifted (std::string systName, int jetFlavor, float pt, float eta, float discr)
{
  float SF = 1.0;

  BTagEntry::JetFlavor flav;
  float mypt = pt;
  if (mypt < 20.) mypt = 20.;
  if (mypt > 1000.) mypt = 1000.;

  if (abs(jetFlavor) == 5)
  {
    flav = BTagEntry::FLAV_B;
  }
  else if (abs(jetFlavor) == 4)
  {
    flav = BTagEntry::FLAV_C;
  }
  else
  {
    flav = BTagEntry::FLAV_UDSG;
  }

  if (DEBUG) cout << "   ~~ requesting SF for systName= " << systName << " BTagEntry::FLAV=" << flav << " pt=" << pt << " eta=" << eta << endl;

  // From Twiki: Owing to the iterative approach, "jes", "lf", "hf", "hfstats1/2",
  // and "lfstats1/2" uncertainties are applied to both b and udsg jets.
  // For c-flavored jets, only "cferr1/2" uncertainties are applied.
  // For other cases (e.g. b-jet with cferr uncrtainty): use the central value of the SF.
  if (flav != BTagEntry::FLAV_C && systName.find("cferr") != std::string::npos)
  {
    SF = m_readers[3].eval_auto_bounds("central", flav, eta, pt, discr);
  }
  else if (flav == BTagEntry::FLAV_C && systName.find("cferr") == std::string::npos)
  {
    SF = m_readers[3].eval_auto_bounds("central", flav, eta, pt, discr);
  }
  else
  {
    SF = m_readers[3].eval_auto_bounds(systName, flav, eta, pt, discr);
  }

  if (DEBUG) cout << "   ~~ returning " << SF << endl;

  return SF;
}

float bTagSF::getEff (WP wpt, int jetFlavor, int channel, float pt, float eta)
{
  // use always "ALL" measurement (last in the array)
  //if (channel >= 3) return 0;
  channel = 3; // "ALL"

  int flav;
  if (abs(jetFlavor) == 5) flav = 0;
  else if (abs(jetFlavor) == 4) flav = 1;
  else flav = 2;

  TH1F* h = m_hEff [(int) wpt] [flav] [channel];
  float aEta = TMath::Abs(eta);

  int binglobal = h->FindBin (pt, aEta);
  int binx, biny, binz;
  h->GetBinXYZ (binglobal, binx, biny, binz); // converts to x, y bins
  int nx = h->GetNbinsX();
  int ny = h->GetNbinsY();

  // under-overflows
  if (binx < 1) binx = 1;
  if (biny < 1) biny = 1;
  if (binx > nx) binx = nx;
  if (biny > ny) biny = ny;

  float eff = h->GetBinContent (binx, biny);

  // protection againts wrongly measured efficiencies (low stat) --> reduce pT bin
  while (eff < 0.00000000001 && binx > 0)
  {
    binx-- ;
    eff = h->GetBinContent (binx, biny);
  }

  return eff;
}

// the collection jets_and_btag in input contain all the final list of jets, already cleaned from PU and leptons
// returns a collection of weights according to the tested WP
vector<float> bTagSF::getEvtWeight (std::vector <std::pair <int, float> >& jets_and_btag, bigTree &theBigTree, std::map<int,double> jets_and_smearFactor, int channel, SFsyst systWP)
{

  vector<double> P_MC   (3, 1.0); // 0 = L, 1 = M, 2 = T
  vector<double> P_Data (3, 1.0); // 0 = L, 1 = M, 2 = T
  double SFreshaping = 1.;        // reshaping SF

  TLorentzVector vJet (0,0,0,0);

  for (unsigned int ijet = 0; ijet < jets_and_btag.size(); ijet++)
  {
    if (DEBUG) cout << "DEB: ijet " << ijet << " , size = " << jets_and_btag.size() << endl;

    int idx = jets_and_btag.at(ijet).first;
    float discr = jets_and_btag.at(ijet).second;
    vJet.SetPxPyPzE (theBigTree.jets_px->at(idx), theBigTree.jets_py->at(idx), theBigTree.jets_pz->at(idx), theBigTree.jets_e->at(idx));
    vJet = vJet * jets_and_smearFactor[idx];

    int flav = theBigTree.jets_HadronFlavour->at(idx);
    double SF[3];
    SF[0] = getSF (loose,  systWP, flav, vJet.Pt(), vJet.Eta());
    SF[1] = getSF (medium, systWP, flav, vJet.Pt(), vJet.Eta());
    SF[2] = getSF (tight,  systWP, flav, vJet.Pt(), vJet.Eta());
    if (systWP == central)
    {
      SFreshaping *= getSF (reshaping,  systWP, flav, vJet.Pt(), vJet.Eta(), discr);
    }
    if (DEBUG) cout << "  >> DEB: SFs " << SF[0] << " " << SF[1] << " " << SF[2] << " " << SFreshaping << endl;

    double effBTag[3];
    effBTag[0] = getEff (static_cast<WP> (0), flav, channel, vJet.Pt(), vJet.Eta()) ;
    effBTag[1] = getEff (static_cast<WP> (1), flav, channel, vJet.Pt(), vJet.Eta()) ;
    effBTag[2] = getEff (static_cast<WP> (2), flav, channel, vJet.Pt(), vJet.Eta()) ;
    if (DEBUG) cout << "  >> DEB: EFFs " << effBTag[0] << " " << effBTag[1] << " " << effBTag[2] << endl;

    double CSV = jets_and_btag.at(ijet).second;
    bool tagged[3];
    tagged[0] = (CSV > _WPtag[0]);
    tagged[1] = (CSV > _WPtag[1]);
    tagged[2] = (CSV > _WPtag[2]);
    if (DEBUG) cout << "  >> DEB: tagged " << tagged[0] << " " << tagged[1] << " " << tagged[2] << endl;
    for (int iWP = 0; iWP < 3; iWP++)
    {
      //cout << " ---> WP: " << iWP << endl;
      double tmpMC   = P_MC.at(iWP);
      double tmpData = P_Data.at(iWP);

      if (tagged[iWP])
      {
	tmpMC *= effBTag[iWP];
	tmpData *= effBTag[iWP]*SF[iWP];
      }
      else
      {
	tmpMC *= (1. - effBTag[iWP]);
	tmpData *= (1. - (effBTag[iWP]*SF[iWP]) );
      }

      //cout << "       effBTag: " << effBTag[iWP] << endl;
      //cout << "       SF: " << SF[iWP] << endl;
      //cout << "       tmpMC: " << tmpMC << endl;
      //cout << "       tmpData: " << tmpData << endl;

      P_MC.at(iWP)  = tmpMC;
      P_Data.at(iWP) = tmpData;
    }
  }
  // return ratio
  vector<float> weight (4);
  weight.at(0) = P_Data.at(0) / P_MC.at(0);
  weight.at(1) = P_Data.at(1) / P_MC.at(1);
  weight.at(2) = P_Data.at(2) / P_MC.at(2);
  weight.at(3) = SFreshaping;

  if (weight.at(0) < 0.05)
  {
    cout << "------ ERROR Null B-TAG weight!!" << endl;
    cout << "    >>> " << P_Data.at(0) << " / " << P_MC.at(0) << " , "
	 << P_Data.at(1) << " / " << P_MC.at(1) << " , "
	 << P_Data.at(2) << " / " << P_MC.at(2) << " , "
	 << SFreshaping << endl;
  }
  //cout << "weights: " << weight.at(0) << " " << weight.at(1) << " " << weight.at(2) << endl;
  return weight;
}


// Systematics according to: https://twiki.cern.ch/twiki/bin/view/CMS/BTagShapeCalibration#Systematic_uncertainties
// Returns a vector containing the reshape weights for all the systematics described in the twiki:
// weights = {
//             "up_jes", "up_lf", "up_hf", "up_hfstats1", "up_hfstats2", "up_lfstats1",
//             "up_lfstats2", "up_cferr1", "up_cferr2",
//             "down_jes", "down_lf", "down_hf", "down_hfstats1", "down_hfstats2", "down_lfstats1",
//             "down_lfstats2", "down_cferr1", "down_cferr2"
//           }
std::vector<float> bTagSF::getEvtWeightShifted (std::vector <std::pair <int, float> >& jets_and_btag, bigTree &theBigTree, std::map<int,double> jets_and_smearFactor)
{
  // Systematics names
  std::vector<std::string> systNames = {
    "up_jes", "up_lf", "up_hf", "up_hfstats1", "up_hfstats2", "up_lfstats1", "up_lfstats2", "up_cferr1", "up_cferr2",
    "down_jes", "down_lf", "down_hf", "down_hfstats1", "down_hfstats2", "down_lfstats1", "down_lfstats2", "down_cferr1", "down_cferr2",
    // Up/down JES variations: do not change order of these!!!
    "up_jesFlavorQCD", "up_jesRelativeBal", "up_jesHF", "up_jesBBEC1", "up_jesEC2", "up_jesAbsolute", "up_jesBBEC1_"+m_year, "up_jesEC2_"+m_year,
    "up_jesAbsolute_"+m_year, "up_jesHF_"+m_year, "up_jesRelativeSample_"+m_year,
    "down_jesFlavorQCD", "down_jesRelativeBal", "down_jesHF", "down_jesBBEC1", "down_jesEC2", "down_jesAbsolute", "down_jesBBEC1_"+m_year,
    "down_jesEC2_"+m_year, "down_jesAbsolute_"+m_year, "down_jesHF_"+m_year, "down_jesRelativeSample_"+m_year
  };

  // Values of shifted SFs all initialized to 1
  std::vector<float> SFs (systNames.size(), 1.);

  // Loop on jets
  TLorentzVector vJet (0,0,0,0);
  for (unsigned int ijet = 0; ijet < jets_and_btag.size(); ijet++)
  {
    if (DEBUG) cout << "DEB: ijet " << ijet << " , size = " << jets_and_btag.size() << endl;

    // Build jet quantities
    int idx = jets_and_btag.at(ijet).first;
    float discr = jets_and_btag.at(ijet).second;
    vJet.SetPxPyPzE(theBigTree.jets_px->at(idx), theBigTree.jets_py->at(idx), theBigTree.jets_pz->at(idx), theBigTree.jets_e->at(idx));
    vJet = vJet * jets_and_smearFactor[idx];
    int flav = theBigTree.jets_HadronFlavour->at(idx);

    // Loop on systematics
    for (unsigned int iname = 0; iname < systNames.size(); iname++)
    {
      SFs.at(iname) *= getSFshifted(systNames.at(iname), flav, vJet.Pt(), vJet.Eta(), discr);
    }
  }

  // Return SFs shifted
  return SFs;
}
