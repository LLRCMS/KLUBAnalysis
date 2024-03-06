#ifndef skimUtils_h
#define skimUtils_h

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <bitset>
#include <map>

#include "bigTree.h"
#include "JECKLUBinterface.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TFile.h"

using namespace std ;

// ------------------------------------------------
// Returns pair<key,value> for highest value in map
template<typename KeyType, typename ValueType>
  std::pair<KeyType,ValueType> get_max_map( const std::map<KeyType,ValueType>& x ) {
  using pairtype=std::pair<KeyType,ValueType>;
  return *std::max_element(x.begin(), x.end(), [] (const pairtype & p1, const pairtype & p2) {
      return p1.second < p2.second;
    });
}


// -----------------------------------------
// General function to check bit variables
bool CheckBit (int number, int bitpos)
{
  bool res = number & (1 << bitpos);
  return res;
}


// -----------------------------------------
// Open input txt file and append all the files it contains to TChain
void appendFromFileList (TChain* chain, TString filename)
{
  //cout << "=== inizio parser ===" << endl;
  std::ifstream infile(filename.Data());
  std::string line;
  while (std::getline(infile, line))
  {
    line = line.substr(0, line.find("#", 0)); // remove comments introduced by #
    while (line.find(" ") != std::string::npos) line = line.erase(line.find(" "), 1); // remove white spaces
    while (line.find("\n") != std::string::npos) line = line.erase(line.find("\n"), 1); // remove new line characters
    while (line.find("\r") != std::string::npos) line = line.erase(line.find("\r"), 1); // remove carriage return characters
    if (!line.empty()) // skip empty lines
      chain->Add(line.c_str());
  }
  return;
}


// -----------------------------------------
// Open the first file in the input list,
// retrieve the histogram "Counters" for the trigger names and return a copy of it
TH1F* getFirstFileHisto (TString filename, bool isForTriggers=true)
{
  std::ifstream infile(filename.Data());
  std::string line;
  while (std::getline(infile, line))
  {
    line = line.substr(0, line.find("#", 0)); // remove comments introduced by #
    while (line.find(" ") != std::string::npos) line = line.erase(line.find(" "), 1); // remove white spaces
    while (line.find("\n") != std::string::npos) line = line.erase(line.find("\n"), 1); // remove new line characters
    while (line.find("\r") != std::string::npos) line = line.erase(line.find("\r"), 1); // remove carriage return characters
    if (!line.empty()) // skip empty lines
      break;
  }

  TFile* fIn = TFile::Open (line.c_str());
  TH1F* dummy = (TH1F*) fIn->Get ("HTauTauTree/Counters");
  TString name = "Counters_perTrigger";
  if(!isForTriggers) {
    dummy = (TH1F*) fIn->Get ("HTauTauTree/TauIDs");
    name = "Counters_pertauID";
  }
  TH1F* histo = new TH1F (*dummy);
  histo-> SetDirectory(0);
  histo->SetName (name.Data());
  fIn->Close();
  return histo;
}


// -----------------------------------------
// Isolation functions
//   muons = 0
//   electrons = 1
//   taus = 2

// Raw isolation value
float getIso (unsigned int iDau, float pt, bigTree & theBigTree)
{
  int type = theBigTree.particleType->at (iDau) ;
  // is tauH
  if (type == 2)
    //return theBigTree.daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits->at(iDau);
    //return theBigTree.daughters_byIsolationMVArun2v1DBoldDMwLTraw->at(iDau);
    //return theBigTree.daughters_byIsolationMVArun2017v2DBoldDMwLTraw2017->at(iDau);
    return theBigTree.daughters_byDeepTau2017v2p1VSjetraw->at(iDau);
  // muon
  if (type == 1 || type == 0)
    return theBigTree.combreliso->at(iDau);

  return -1 ;
}

// Convert the tau iso discriminators into an integer, from 0 == VLoose to 6 == VVTight
// example for MVA id (same for cut based):
// == 0 : NotIso
// >= 1 : pass VLoose
// >= 2 : pass Loose
// >= 3 : pass Medium
// >= 4 : pass Tigth
// >= 5 : pass VTight
// each number denotes the most stringent discriminator passed, so that selection candidates as:
// MVAiso >= 3 --> all candidates that are *at least* medium iso (or more isolated)
// MVAiso <= 3 --> sideband : all anti-isolated candidates, *not more* isolated than medium WP
int makeIsoDiscr (unsigned int iDau, vector<int>& nameToIdxMap, bigTree & theBigTree)
{
  int isoInt = 0;
  Long64_t tauID = theBigTree.tauID->at(iDau);
  while (isoInt < (int) nameToIdxMap.size())
  {
    int bit = nameToIdxMap.at(isoInt) ;
    bool pass = (((tauID >> bit) & 1 ) > 0);
    if (!pass) break; // will freeze isoInt to the previous value
    isoInt += 1;
  }
  return isoInt;
}

// Tau idolation index
int getTauIDIdx (TH1F* h_tauID, string tauIDName)
{
  int ibin = -1;
  for (int i = 1; i <= h_tauID->GetNbinsX(); ++i)
  {
    string binlabel = h_tauID->GetXaxis()->GetBinLabel(i);
    if (tauIDName == binlabel)
    {
      ibin = i;
      break;
    }
  }
  return ibin-1;
}


// -----------------------------------------
// deltaPhi function
float deltaPhi (float phi1, float phi2)
{
  float deltaphi = fabs (phi1 - phi2) ;
  if (deltaphi > 6.283185308) deltaphi -= 6.283185308 ;
  if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi ;
  return deltaphi ;
}

// Z function
float getZ (float eta, float eta1, float eta2)
{
  float Z  =  (eta - 0.5*(eta1 + eta2)) / fabs (eta1 - eta2) ;
  return Z;
}


// -----------------------------------------
// Check that the VBF jets are correctly matched to different TriggerObjects
bool checkVBFjetMatch(bool DEBUG, int iJet, int kJet, bigTree & theBigTree)
{
  bool goodmatch= true;

  Long64_t jet1lead = theBigTree.jets_VBFleadFilterMatch   ->at(iJet);
  Long64_t jet1subl = theBigTree.jets_VBFsubleadFilterMatch->at(iJet);
  Long64_t jet2lead = theBigTree.jets_VBFleadFilterMatch   ->at(kJet);
  Long64_t jet2subl = theBigTree.jets_VBFsubleadFilterMatch->at(kJet);

  if (DEBUG)
  {
    cout << "---   checking VBF jet legs   --- " << endl;
    cout << "jet "<< iJet << " - leading    : " << std::bitset<64>(jet1lead) << endl;
    cout << "jet "<< iJet << " - subleading : " << std::bitset<64>(jet1subl) << endl;
    cout << "jet "<< kJet << " - leading    : " << std::bitset<64>(jet2lead) << endl;
    cout << "jet "<< kJet << " - subleading : " << std::bitset<64>(jet2subl) << endl;
    cout << "XOR leading       : " << (jet1lead^jet2lead) << endl;
    cout << "XOR subleading    : " << (jet1subl^jet2subl) << endl;
    if ( (jet1subl^jet2subl) == 0 ) cout << " -- same TO for the subleading jets" << endl;
    if ( (jet1lead^jet2lead) == 0 ) cout << " -- same TO for the leading jets" << endl;
    cout << "--- end checking VBF jet legs --- " << endl;
  }

  bool firstJetZero  = (jet1lead==0 && jet1subl==0);
  bool secondJetZero = (jet2lead==0 && jet2subl==0);
  if (firstJetZero || secondJetZero) goodmatch = false; //one of the two jets is not matched to any filter
  if ( (jet1subl^jet2subl) == 0 )    goodmatch = false; //two jets are matched to the same TrigObj (subleading filter)
  if ( (jet1lead^jet2lead) == 0 )    goodmatch = false; //two jets are matched to the same TrigObj (subleading filter)

  return goodmatch;
}


// -----------------------------------------
// Generic functions to get the CvsL / CvsB discriminators as described in:
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X#Supported_Algorithms_and_Operati

float getCvsL (bigTree &theBigTree, int idx)
{
  float prob_c   = theBigTree.bDeepFlavor_probc->at(idx);
  float prob_uds = theBigTree.bDeepFlavor_probuds->at(idx);
  float prob_g   = theBigTree.bDeepFlavor_probg->at(idx);

  return prob_c / (prob_c + prob_uds + prob_g);
}

float getCvsB (bigTree &theBigTree, int idx)
{
  float prob_c    = theBigTree.bDeepFlavor_probc   ->at(idx);
  float prob_b    = theBigTree.bDeepFlavor_probb   ->at(idx);
  float prob_bb   = theBigTree.bDeepFlavor_probbb  ->at(idx);
  float prob_lepb = theBigTree.bDeepFlavor_problepb->at(idx);

  return prob_c / (prob_c + prob_b + prob_bb + prob_lepb);
}


// -----------------------------------------
// Generic functions to read content of 1D / 2D / 3D histos, taking care of x axis limit (no under/over flow)
double getContentHisto1D(TH1* histo, double x)
{
  int nbinsx = histo->GetNbinsX();
  int ibin = histo->FindBin(x);
  if (ibin == 0) ibin = 1;
  if (ibin > nbinsx) ibin = nbinsx;
  return histo->GetBinContent(ibin);
}

double getContentHisto2D(TH2* histo, double x, double y)
{
  int nbinsx = histo->GetNbinsX();
  int nbinsy = histo->GetNbinsY();
  int ibinx = histo->GetXaxis()->FindBin(x);
  int ibiny = histo->GetYaxis()->FindBin(y);

  if (ibinx == 0)     ibinx = 1;
  if (ibinx > nbinsx) ibinx = nbinsx;

  if (ibiny == 0)     ibiny = 1;
  if (ibiny > nbinsy) ibiny = nbinsy;

  return histo->GetBinContent(ibinx, ibiny);
}

double getContentHisto3D(TH3* histo, double x, double y, double z, int unc_scale = 0)
{
  int nbinsx = histo->GetNbinsX();
  int nbinsy = histo->GetNbinsY();
  int nbinsz = histo->GetNbinsZ();

  int ibinx = histo->GetXaxis()->FindBin(x);
  int ibiny = histo->GetYaxis()->FindBin(y);
  int ibinz = histo->GetZaxis()->FindBin(z);

  if (ibinx == 0)     ibinx = 1;
  if (ibinx > nbinsx) ibinx = nbinsx;

  if (ibiny == 0)     ibiny = 1;
  if (ibiny > nbinsy) ibiny = nbinsy;

  if (ibinz == 0)     ibinz = 1;
  if (ibinz > nbinsz) ibinz = nbinsz;

  return ( unc_scale == 0 ? histo->GetBinContent(ibinx, ibiny, ibinz) : histo->GetBinError(ibinx, ibiny, ibinz) );
}


// -----------------------------------------
// Returns indexes of subjets related to one fatjet
vector<int> findSubjetIdxs (unsigned int iFatJet, bigTree & theBigTree)
{
  vector<int> indexes;
  int idxFatJet = (int) iFatJet;
  for (unsigned int isj = 0; isj < theBigTree.subjets_ak8MotherIdx->size(); isj++)
  {
    if (theBigTree.subjets_ak8MotherIdx->at(isj) == idxFatJet)
      indexes.push_back(isj);
  }
  return indexes;
}


// -----------------------------------------
// Implement operator < for b tag . first : DeepJet score ;  second : index
bool bJetSort (const pair<float, int>& i, const pair<float, int>& j) {
  if (i.first != j.first) return (i.first < j.first) ; // lowest DeepJet
  return i.second > j.second ; // highest index == lowest pt
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- ----      Systematics up/down shifts    ---- ----
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

// -----------------------------------------
// get shifted dau (tes; ees)
TLorentzVector getShiftedDau(TLorentzVector tlv_nominal, double shift, double unc, bool isthisDM, bool shiftMass = true)
{
  TLorentzVector tlv_shifted;
  double px_up = (isthisDM? tlv_nominal.Px() * (1 + (shift*unc)) : tlv_nominal.Px());
  double py_up = (isthisDM? tlv_nominal.Py() * (1 + (shift*unc)) : tlv_nominal.Py());
  double pz_up = (isthisDM? tlv_nominal.Pz() * (1 + (shift*unc)) : tlv_nominal.Pz());
  double m_up =  ((isthisDM && shiftMass)? tlv_nominal.M() * ( 1 + (shift*unc)) : tlv_nominal.M());
  double e_up = TMath::Sqrt(px_up*px_up + py_up*py_up + pz_up*pz_up + m_up*m_up);
  tlv_shifted.SetPxPyPzE (px_up, py_up, pz_up, e_up);
  return tlv_shifted;
}


// -----------------------------------------
// get shifted jet (I could just use the function above, I separe the jet case just to make the functions easier to read)
TLorentzVector getShiftedJet(TLorentzVector tlv_nominal, double shift, double unc)
{
  TLorentzVector tlv_shifted;
  tlv_shifted.SetPtEtaPhiE(
    (1.+(shift*unc)) * tlv_nominal.Pt(),
    tlv_nominal.Eta(),
    tlv_nominal.Phi(),
    (1.+(shift*unc)) * tlv_nominal.E()
    );
  return tlv_shifted;
}


// -----------------------------------------
// get shifted MET (jet)
// returns a pair of vectors of TVector2 objects
// first: up variations
// second: down variations
pair <vector <TVector2>, vector <TVector2> > getShiftedMET_jet (int N_jecSources, TVector2 MET, bigTree & theBigTree, JECKLUBinterface & JECprovider, bool DEBUG=false)
{

  if (DEBUG) cout << "*********** DEBUGGING JETS *********** "<< endl;

  vector <double> corrMETx_up(N_jecSources, MET.Px());
  vector <double> corrMETy_up(N_jecSources, MET.Py());
  vector <double> corrMETx_down(N_jecSources, MET.Px());
  vector <double> corrMETy_down(N_jecSources, MET.Py());

  vector <TVector2> shiftedMET_up(N_jecSources);
  vector <TVector2> shiftedMET_down(N_jecSources);

  for (unsigned int iJet = 0 ; iJet < theBigTree.jets_px->size () ; ++iJet)
  {

    // build original jet
    TLorentzVector tlv_jet (theBigTree.jets_px->at(iJet), theBigTree.jets_py->at(iJet), theBigTree.jets_pz->at(iJet), theBigTree.jets_e->at(iJet));

    // get uncertainty
    pair <vector <double>, vector<double>> unc_updown = JECprovider.getJECUncVectors(iJet, theBigTree);

    // build shifted jet
    for (int isource = 0; isource < N_jecSources; isource++)
    {
      TLorentzVector tlv_jetup   = getShiftedJet(tlv_jet, +1., unc_updown.first[isource]);
      TLorentzVector tlv_jetdown = getShiftedJet(tlv_jet, -1., unc_updown.second[isource]);

      // shift MET - first the original jet
      corrMETx_up[isource] += tlv_jet.Px();
      corrMETy_up[isource] += tlv_jet.Py();
      corrMETx_down[isource] += tlv_jet.Px();
      corrMETy_down[isource] += tlv_jet.Py();

      // shift MET - then the shifted jet
      corrMETx_up[isource] -= tlv_jetup.Px();
      corrMETy_up[isource] -= tlv_jetup.Py();
      corrMETx_down[isource] -= tlv_jetdown.Px();
      corrMETy_down[isource] -= tlv_jetdown.Py();

    }
  }

  for (int isource = 0; isource < N_jecSources; isource++)
  {
    shiftedMET_up[isource].Set(corrMETx_up[isource], corrMETy_up[isource]);
    shiftedMET_down[isource].Set(corrMETx_down[isource], corrMETy_down[isource]);
  }

  return make_pair(shiftedMET_up, shiftedMET_down);
}


// -----------------------------------------
// get shifted MET (total JES)
// returns a pair of TVector2 objects
// first: up variations
// second: down variations
pair <TVector2, TVector2> getShiftedMET_jetTot (int N_jecSources, TVector2 MET, bigTree & theBigTree, JECKLUBinterface & JECprovider, bool DEBUG=false)
{
  double corrMETx_up   = MET.Px();
  double corrMETy_up   = MET.Py();
  double corrMETx_down = MET.Px();
  double corrMETy_down = MET.Py();

  TVector2 shiftedMET_up;
  TVector2 shiftedMET_down;

  for (unsigned int iJet = 0 ; iJet < theBigTree.jets_px->size () ; ++iJet)
  {
    // build original jet
    TLorentzVector tlv_jet (theBigTree.jets_px->at(iJet), theBigTree.jets_py->at(iJet), theBigTree.jets_pz->at(iJet), theBigTree.jets_e->at(iJet));

    // get uncertainty
    pair <vector <double>, vector<double>> unc_updown = JECprovider.getJECUncVectors(iJet, theBigTree);

    // build shifted jet
    TLorentzVector tlv_jetup   = getShiftedJet(tlv_jet, +1., unc_updown.first[N_jecSources]);
    TLorentzVector tlv_jetdown = getShiftedJet(tlv_jet, -1., unc_updown.second[N_jecSources]);

    // shift MET - first the original jet
    corrMETx_up   += tlv_jet.Px();
    corrMETy_up   += tlv_jet.Py();
    corrMETx_down += tlv_jet.Px();
    corrMETy_down += tlv_jet.Py();

    // shift MET - then the shifted jet
    corrMETx_up   -= tlv_jetup.Px();
    corrMETy_up   -= tlv_jetup.Py();
    corrMETx_down -= tlv_jetdown.Px();
    corrMETy_down -= tlv_jetdown.Py();
  }

  shiftedMET_up  .Set(corrMETx_up  , corrMETy_up);
  shiftedMET_down.Set(corrMETx_down, corrMETy_down);

  return make_pair(shiftedMET_up, shiftedMET_down);
}

// -----------------------------------------
// get shifted MET due to jet smearing (JER)
// returns a TVector2 of the smeared MET
TVector2 getShiftedMET_smear (float METx, float METy, bigTree & theBigTree, std::map<int,double> jets_and_smearFactor)
{
  float smearedMETx = METx;
  float smearedMETy = METy;

  TVector2 smearedMET;

  for (unsigned int iJet = 0 ; iJet < theBigTree.jets_px->size () ; ++iJet)
  {

    // Build original jet
    TLorentzVector tlv_jet (theBigTree.jets_px->at(iJet), theBigTree.jets_py->at(iJet), theBigTree.jets_pz->at(iJet), theBigTree.jets_e->at(iJet));

    // Build smeared jet
    TLorentzVector tlv_jet_smeared = tlv_jet * jets_and_smearFactor[iJet];

    // shift MET - first the original jet
    smearedMETx += tlv_jet.Px();
    smearedMETy += tlv_jet.Py();

    // shift MET - then the shifted jet
    smearedMETx -= tlv_jet_smeared.Px();
    smearedMETy -= tlv_jet_smeared.Py();
  }

  smearedMET.Set(smearedMETx, smearedMETy);

  return smearedMET;
}


// -----------------------------------------
//returns a pair of pairs of vectors of TVector2 objects (too nested? \_(``)_/)
// get shifted MET
//returns a pair of vectors
//first: tes variations
//    first: up variations
//    second: down variations
//second: ees variations
//    first: up variations
//    second: down variations

auto getShiftedMET_tes_ees (int N_tauhDM, int N_tauhDM_EES, TVector2 MET, bigTree & theBigTree, bool DEBUG=false)
{
  vector <double> corrMETx_tauup(N_tauhDM, MET.Px());
  vector <double> corrMETy_tauup(N_tauhDM, MET.Py());
  vector <double> corrMETx_taudown(N_tauhDM, MET.Px());
  vector <double> corrMETy_taudown(N_tauhDM, MET.Py());

  vector <TVector2> shiftedMET_tauup(N_tauhDM);
  vector <TVector2> shiftedMET_taudown(N_tauhDM);

  vector <double> corrMETx_eleup(N_tauhDM_EES, MET.Px());
  vector <double> corrMETy_eleup(N_tauhDM_EES, MET.Py());
  vector <double> corrMETx_eledown(N_tauhDM_EES, MET.Px());
  vector <double> corrMETy_eledown(N_tauhDM_EES, MET.Py());

  vector <TVector2> shiftedMET_eleup(N_tauhDM_EES);
  vector <TVector2> shiftedMET_eledown(N_tauhDM_EES);

  for (unsigned int idau = 0 ; idau < theBigTree.daughters_px->size () ; ++idau)
  {
    // build original jet
    TLorentzVector tlv_dau(theBigTree.daughters_px->at(idau), theBigTree.daughters_py->at(idau),
						   theBigTree.daughters_pz->at(idau), theBigTree.daughters_e->at(idau));

    // get uncertainty
    vector <double> unc_TES;
    vector <double> unc_EESup;
    vector <double> unc_EESdw;
    unc_TES.push_back(theBigTree.daughters_TESshiftDM0->at(idau)); // first daughter, DM 0
    unc_TES.push_back(theBigTree.daughters_TESshiftDM1->at(idau)); // first daughter, DM 1
    unc_TES.push_back(theBigTree.daughters_TESshiftDM10->at(idau)); // first daughter, DM 10
    unc_TES.push_back(theBigTree.daughters_TESshiftDM11->at(idau)); // first daughter, DM 11
    unc_EESup.push_back(theBigTree.daughters_EESshiftDM0up->at(idau)); // first daughter, DM 0
    unc_EESup.push_back(theBigTree.daughters_EESshiftDM1up->at(idau)); // first daughter, DM 1
    unc_EESdw.push_back(theBigTree.daughters_EESshiftDM0dw->at(idau)); // first daughter, DM 0
    unc_EESdw.push_back(theBigTree.daughters_EESshiftDM1dw->at(idau)); // first daughter, DM 1

    // for each decay mode, bool indicating if this lepton matches the dacay mode in the loop
    // just for protection, probably it's not needed
    vector<bool> isthisDM =
      {
		(theBigTree.decayMode->at (idau) == 0 ? true : false),
		(theBigTree.decayMode->at (idau) == 1 ? true : false),
		(theBigTree.decayMode->at (idau) == 10? true : false),
		(theBigTree.decayMode->at (idau) == 11? true : false)
      };

    bool dauHasTES = (theBigTree.genmatch->at(idau)  == 5 ? true : false);
    bool dauHasEES = ((theBigTree.genmatch->at(idau)==1 || theBigTree.genmatch->at(idau)==3) ? true : false);

    // loop over DMs to fill the shifted TLVs
    for (int idm = 0; idm< N_tauhDM; idm++)
    {
      if (dauHasTES)
      {
        TLorentzVector tlv_dau_tauup   = getShiftedDau(tlv_dau,  1.,  unc_TES[idm], isthisDM[idm], (idm != 0));  // no mass shift for DM == 0 (idm == 0)
        TLorentzVector tlv_dau_taudown = getShiftedDau(tlv_dau, -1.,  unc_TES[idm], isthisDM[idm], (idm != 0));  // no mass shift for DM == 0 (idm == 0)

        // shift MET - first the original tau
        corrMETx_tauup[idm] += tlv_dau.Px();
        corrMETy_tauup[idm] += tlv_dau.Py();
        corrMETx_taudown[idm] += tlv_dau.Px();
        corrMETy_taudown[idm] += tlv_dau.Py();

        // shift MET - then the shifted tau
        corrMETx_tauup[idm] -= tlv_dau_tauup.Px();
        corrMETy_tauup[idm] -= tlv_dau_tauup.Py();
        corrMETx_taudown[idm] -= tlv_dau_taudown.Px();
        corrMETy_taudown[idm] -= tlv_dau_taudown.Py();
      }

      if (dauHasEES && idm < N_tauhDM_EES)
      {
        TLorentzVector tlv_dau_eleup   = getShiftedDau(tlv_dau, 1.,  unc_EESup[idm], isthisDM[idm]);
        TLorentzVector tlv_dau_eledown = getShiftedDau(tlv_dau, -1., unc_EESdw[idm], isthisDM[idm]);

        // shift MET - first the original tau
        corrMETx_eleup[idm]   += tlv_dau.Px();
        corrMETy_eleup[idm]   += tlv_dau.Py();
        corrMETx_eledown[idm] += tlv_dau.Px();
        corrMETy_eledown[idm] += tlv_dau.Py();

        // shift MET - then the shifted tau
        corrMETx_eleup[idm]   -= tlv_dau_eleup.Px();
        corrMETy_eleup[idm]   -= tlv_dau_eleup.Py();
        corrMETx_eledown[idm] -= tlv_dau_eledown.Px();
        corrMETy_eledown[idm] -= tlv_dau_eledown.Py();
      }
    }
  }

  for (int idm = 0; idm < N_tauhDM; idm++)
  {
    shiftedMET_tauup[idm].Set(corrMETx_tauup[idm], corrMETy_tauup[idm]);
    shiftedMET_taudown[idm].Set(corrMETx_taudown[idm], corrMETy_taudown[idm]);

    if (idm < N_tauhDM_EES)
    {
      shiftedMET_eleup[idm].Set(corrMETx_eleup[idm], corrMETy_eleup[idm]);
      shiftedMET_eledown[idm].Set(corrMETx_eledown[idm], corrMETy_eledown[idm]);
    }
  }

  return make_pair(make_pair(shiftedMET_tauup, shiftedMET_taudown),make_pair(shiftedMET_eleup, shiftedMET_eledown));
}


// -----------------------------------------
// returns a pair of TVector2 objects
// first: up variation
// second: down variation
pair <TVector2, TVector2> getShiftedMET_mes (TVector2 MET, bigTree & theBigTree, bool DEBUG=false)
{
  double corrMETx_muup =  MET.Px();
  double corrMETy_muup =  MET.Py();
  double corrMETx_mudown =  MET.Px();
  double corrMETy_mudown =  MET.Py();

  TVector2 shiftedMET_muup;
  TVector2 shiftedMET_mudown;

  for (unsigned int idau = 0 ; idau < theBigTree.daughters_px->size () ; ++idau)
  {
    // build original jet
    TLorentzVector tlv_dau (theBigTree.daughters_px->at(idau), theBigTree.daughters_py->at(idau), theBigTree.daughters_pz->at(idau), theBigTree.daughters_e->at(idau));

    // get uncertainty
    double unc_MESup = theBigTree.daughters_MESshiftup ->at (idau);
    double unc_MESdw = theBigTree.daughters_MESshiftdw ->at (idau);

    bool dauHasMES = ((theBigTree.genmatch->at(idau)==2 || theBigTree.genmatch->at(idau)==4) ? true : false);

    if (dauHasMES)
    {
      TLorentzVector tlv_dau_muup   = getShiftedDau(tlv_dau,  1.,  unc_MESup, true);
      TLorentzVector tlv_dau_mudown = getShiftedDau(tlv_dau, -1.,  unc_MESdw, true);

      // shift MET - first the original tau
      corrMETx_muup += tlv_dau.Px();
      corrMETy_muup += tlv_dau.Py();
      corrMETx_mudown += tlv_dau.Px();
      corrMETy_mudown += tlv_dau.Py();

      // shift MET - then the shifted tau
      corrMETx_muup -= tlv_dau_muup.Px();
      corrMETy_muup -= tlv_dau_muup.Py();
      corrMETx_mudown -= tlv_dau_mudown.Px();
      corrMETy_mudown -= tlv_dau_mudown.Py();
    }
  }

  shiftedMET_muup.Set(corrMETx_muup, corrMETy_muup);
  shiftedMET_mudown.Set(corrMETx_mudown, corrMETy_mudown);

  return make_pair(shiftedMET_muup, shiftedMET_mudown);
}

#endif
