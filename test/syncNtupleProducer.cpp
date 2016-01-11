#include <cstdlib>
#include <iostream> 
#include <fstream>
#include <sstream>
#include <map>
#include <string>

#include "TChain.h"
#include "TEntryList.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TPluginManager.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TF1.h"
#include "TLegend.h"
#include "THStack.h"
#include "TCut.h"
#include "TArrayF.h"
#include "TStyle.h"
#include "ConfigParser.h"
//#include "FuncCB.h"
#include "RooAbsReal.h"
#include "RooMath.h"
#include "OfflineProducerHelper.h"

//#define Dummy true
#define RedoOrdering true//should be done properly at prod. stage, but this allows to redo a custom one.

typedef  std::pair<UInt_t, std::vector<Double_t>> mypair;

using namespace std;

Double_t ApproxErf(Double_t arg)
{
  static const double erflim = 5.0;
  if( arg > erflim )
    return 1.0;
  if( arg < -erflim )
    return -1.0;
  
  return RooMath::erf(arg);
} 

Double_t evaluateCB(Double_t m, Double_t m0, Double_t sigma, Double_t alpha, Double_t n, Double_t norm)
{ 
  const double sqrtPiOver2 = 1.2533141373; // sqrt(pi/2)
  const double sqrt2 = 1.4142135624;

  Double_t sig = fabs((Double_t) sigma);
   
  Double_t t = (m - m0)/sig ;
   
  if (alpha < 0)
    t = -t;

  Double_t absAlpha = fabs(alpha / sig);
  Double_t a = TMath::Power(n/absAlpha,n)*exp(-0.5*absAlpha*absAlpha);
  Double_t b = absAlpha - n/absAlpha;

  //cout << a << " " << b << endl;

  ////// Pour la crystal ball
  // if (t <= absAlpha){
  //   return norm * exp(-0.5*t*t);
  // }
  // else
  //   {
  //     return norm * a * TMath::Power(t-b,-n) ;
  //   }

  Double_t aireGauche = (1 + ApproxErf( absAlpha / sqrt2 )) * sqrtPiOver2 ;
  Double_t aireDroite = ( a * 1/TMath::Power(absAlpha - b,n-1)) / (n - 1);
  Double_t aire = aireGauche + aireDroite;

  if ( t <= absAlpha ){
    return norm * (1 + ApproxErf( t / sqrt2 )) * sqrtPiOver2 / aire ;
  }
  else{
    return norm * (aireGauche +  a * (1/TMath::Power(t-b,n-1) - 1/TMath::Power(absAlpha - b,n-1)) / (1 - n)) / aire ;
  }
  
}



bool comparator ( const mypair& l, const mypair& r)
{
  //Iso1
  if(l.second.at(0)!=r.second.at(0)) return l.second.at(0) < r.second.at(0);
  //Pt1
  if(l.second.at(1)!=r.second.at(1)) return l.second.at(1) > r.second.at(1);
  //Iso2
  if(l.second.at(2)!=r.second.at(2)) return l.second.at(2) < r.second.at(2);
  //Pt2
  if(l.second.at(3)!=r.second.at(3)) return l.second.at(3) > r.second.at(3);

  return true;
}

bool CheckElectronMVAIDWP80(float MVAValue, TLorentzVector electron)
{
  float eta = fabs(electron.Eta());
  float pt = electron.Pt();

  if(pt < 5) return false ;
  else if(pt > 5. && pt < 10.)
    {
      if(eta < 0.8)
	{
	  if(MVAValue > -0.253) return true ;
	}
      else if(eta > 0.8 && eta < 1.479)
	{
	  if(MVAValue > 0.081) return true ;
	}
      else if(eta > 1.479)
	{
	  if(MVAValue > -0.081) return true ;
	}
    }
  else if(pt > 10.)
    {
      if(eta < 0.8)
	{
	  if(MVAValue > 0.965) return true ;
	}
      else if(eta > 0.8 && eta < 1.479)
	{
	  if(MVAValue > 0.917) return true ;
	}
      else if(eta > 1.479)
	{
	  if(MVAValue > 0.683) return true ;
	}
    }    

  return false ;
}

bool CheckElectronMVAIDWP90(float MVAValue, TLorentzVector electron)
{
  float eta = fabs(electron.Eta());
  float pt = electron.Pt();

  if(pt < 5) return false ;
  else if(pt > 5. && pt < 10.)
    {
      if(eta < 0.8)
	{
	  if(MVAValue > -0.483) return true ;
	}
      else if(eta > 0.8 && eta < 1.479)
	{
	  if(MVAValue > -0.267) return true ;
	}
      else if(eta > 1.479)
	{
	  if(MVAValue > -0.323) return true ;
	}
    }
  else if(pt > 10.)
    {
      if(eta < 0.8)
	{
	  if(MVAValue > 0.933) return true ;
	}
      else if(eta > 0.8 && eta < 1.479)
	{
	  if(MVAValue > 0.825) return true ;
	}
      else if(eta > 1.479)
	{
	  if(MVAValue > 0.337) return true ;
	}
    }    

  return false ;
}


///////////////////////////////////////////////////////////////////////////////////////////////
void ProduceSyncNtuple(Bool_t DataTrue_MCFalse, TString InputFileName, TString OutputFileName, TString Channel, ConfigParser* gConfigParser)
{
  //automated trigger histo finding
  TString CommandToExecute = "ls "+InputFileName;
  TString ResultOfLs = gSystem->GetFromPipe(CommandToExecute.Data());
  string ResultOfLs_str(ResultOfLs.Data());
  string toFind = ".root";
  std::size_t found = ResultOfLs_str.find(toFind);
  string FileForTempHisto = ResultOfLs_str.substr(0,found);
  TString FileForTempHisto_TStr(FileForTempHisto);
  FileForTempHisto_TStr += ".root";
  TFile tempToGetHisto(FileForTempHisto_TStr.Data(),"READ");
  TH1F* hCounter = (TH1F*)tempToGetHisto.Get("HTauTauTree/Counters");
  TH1F* hTauID = (TH1F*)tempToGetHisto.Get("HTauTauTree/TauIDs");

  // cout<<"printing config parser: "<<endl;
  // gConfigParser->print();

  //trigger paths for MC and data
  vector<string> triggerPathsMC = gConfigParser->readStringListOption("event::triggersMC");
  vector<string> triggerPathsData = gConfigParser->readStringListOption("event::triggersData");
  float DR = gConfigParser->readFloatOption("event::DR");

  float cutLepton1_Pt = -99.;
  float cutLepton1_Eta = -99.;
  string cutLepton1_DMV ;
  string cutLepton1_IDV ;
  string cutLepton1_againstEleV ;
  float cutLepton1_againstEle = -99.;
  string cutLepton1_againstMuV ;
  float cutLepton1_againstMu = -99.;
  float cutLepton1_ID = -99.;
  string cutLepton1_IsoV ;
  float cutLepton1_Iso = -99.;
  float cutLepton1_dxy = -99.;
  float cutLepton1_dz = -99.;

  float cutLepton2_Pt = -99.;
  float cutLepton2_Eta = -99.;
  string cutLepton2_DMV ;
  string cutLepton2_IDV ;
  string cutLepton2_againstEleV ;
  float cutLepton2_againstEle = -99.;
  string cutLepton2_againstMuV ;
  float cutLepton2_againstMu = -99.;
  float cutLepton2_ID = -99.;
  string cutLepton2_IsoV ;
  float cutLepton2_Iso = -99.;
  float cutLepton2_dxy = -99.;
  float cutLepton2_dz = -99.;

  if(Channel=="mt")
    {
      //muon
      cutLepton1_Pt = gConfigParser->readFloatOption("muon::pT");
      cutLepton1_Eta = gConfigParser->readFloatOption("muon::Eta");
      cutLepton1_IDV = gConfigParser->readStringOption("muon::IDV");
      cutLepton1_ID = gConfigParser->readFloatOption("muon::ID");
      cutLepton1_Iso = gConfigParser->readFloatOption("muon::Iso");
      cutLepton1_dxy = gConfigParser->readFloatOption("muon::dxy");
      cutLepton1_dz = gConfigParser->readFloatOption("muon::dz");

      //tau
      cutLepton2_Pt = gConfigParser->readFloatOption("tau::pT");
      cutLepton2_Eta = gConfigParser->readFloatOption("tau::Eta");
      cutLepton2_DMV = gConfigParser->readStringOption("tau::DMV");
      cutLepton2_againstEleV = gConfigParser->readStringOption("tau::againstEleV");
      cutLepton2_againstEle = gConfigParser->readFloatOption("tau::againstEle");
      cutLepton2_againstMuV = gConfigParser->readStringOption("tau::againstMuV");
      cutLepton2_againstMu = gConfigParser->readFloatOption("tau::againstMu");
      cutLepton2_IsoV = gConfigParser->readStringOption("tau::IsoV");
      cutLepton2_Iso = gConfigParser->readFloatOption("tau::Iso");
      cutLepton2_dz = gConfigParser->readFloatOption("tau::dz");

    }
  else if(Channel=="et")
    {
      //electron
      cutLepton1_Pt = gConfigParser->readFloatOption("electron::pT");
      cutLepton1_Eta = gConfigParser->readFloatOption("electron::Eta");
      cutLepton1_IDV = gConfigParser->readStringOption("electron::IDV");
      cutLepton1_ID = gConfigParser->readFloatOption("electron::ID");
      cutLepton1_Iso = gConfigParser->readFloatOption("electron::Iso");
      cutLepton1_dxy = gConfigParser->readFloatOption("electron::dxy");
      cutLepton1_dz = gConfigParser->readFloatOption("electron::dz");

      //tau
      cutLepton2_Pt = gConfigParser->readFloatOption("tau::pT");
      cutLepton2_Eta = gConfigParser->readFloatOption("tau::Eta");
      cutLepton2_DMV = gConfigParser->readStringOption("tau::DMV");
      cutLepton2_againstEleV = gConfigParser->readStringOption("tau::againstEleV");
      cutLepton2_againstEle = gConfigParser->readFloatOption("tau::againstEle");
      cutLepton2_againstMuV = gConfigParser->readStringOption("tau::againstMuV");
      cutLepton2_againstMu = gConfigParser->readFloatOption("tau::againstMu");
      cutLepton2_IsoV = gConfigParser->readStringOption("tau::IsoV");
      cutLepton2_Iso = gConfigParser->readFloatOption("tau::Iso");
      cutLepton2_dz = gConfigParser->readFloatOption("tau::dz");

    }
  else if(Channel=="em")
    {
      //electron
      cutLepton1_Pt = gConfigParser->readFloatOption("electron::pT");
      cutLepton1_Eta = gConfigParser->readFloatOption("electron::Eta");
      cutLepton1_IDV = gConfigParser->readStringOption("electron::IDV");
      cutLepton1_ID = gConfigParser->readFloatOption("electron::ID");
      cutLepton1_Iso = gConfigParser->readFloatOption("electron::Iso");
      cutLepton1_dxy = gConfigParser->readFloatOption("electron::dxy");
      cutLepton1_dz = gConfigParser->readFloatOption("electron::dz");

      //muon
      cutLepton2_Pt = gConfigParser->readFloatOption("muon::pT");
      cutLepton2_Eta = gConfigParser->readFloatOption("muon::Eta");
      cutLepton2_IDV = gConfigParser->readStringOption("muon::IDV");
      cutLepton2_ID = gConfigParser->readFloatOption("muon::ID");
      cutLepton2_Iso = gConfigParser->readFloatOption("muon::Iso");
      cutLepton2_dxy = gConfigParser->readFloatOption("muon::dxy");
      cutLepton2_dz = gConfigParser->readFloatOption("muon::dz");

    }
  else if(Channel=="tt")
    {
      //tauOne
      cutLepton1_Pt = gConfigParser->readFloatOption("tauOne::pT");
      cutLepton1_Eta = gConfigParser->readFloatOption("tauOne::Eta");
      cutLepton1_DMV = gConfigParser->readStringOption("tauOne::DMV");
      cutLepton1_againstEleV = gConfigParser->readStringOption("tauOne::againstEleV");
      cutLepton1_againstEle = gConfigParser->readFloatOption("tauOne::againstEle");
      cutLepton1_againstMuV = gConfigParser->readStringOption("tauOne::againstMuV");
      cutLepton1_againstMu = gConfigParser->readFloatOption("tauOne::againstMu");
      cutLepton1_IsoV = gConfigParser->readStringOption("tauOne::IsoV");
      cutLepton1_Iso = gConfigParser->readFloatOption("tauOne::Iso");
      cutLepton1_dz = gConfigParser->readFloatOption("tauOne::dz");

      //tauTwo
      cutLepton2_Pt = gConfigParser->readFloatOption("tauTwo::pT");
      cutLepton2_Eta = gConfigParser->readFloatOption("tauTwo::Eta");
      cutLepton2_DMV = gConfigParser->readStringOption("tauTwo::DMV");
      cutLepton2_againstEleV = gConfigParser->readStringOption("tauTwo::againstEleV");
      cutLepton2_againstEle = gConfigParser->readFloatOption("tauTwo::againstEle");
      cutLepton2_againstMuV = gConfigParser->readStringOption("tauTwo::againstMuV");
      cutLepton2_againstMu = gConfigParser->readFloatOption("tauTwo::againstMu");
      cutLepton2_IsoV = gConfigParser->readStringOption("tauTwo::IsoV");
      cutLepton2_Iso = gConfigParser->readFloatOption("tauTwo::Iso");
      cutLepton2_dz = gConfigParser->readFloatOption("tauTwo::dz");
    }

  TChain* m_SampleChain = new TChain("HTauTauTree/HTauTauTree");
  m_SampleChain->Add(InputFileName.Data());

  Int_t           EventNumber;
  Int_t           RunNumber;
  Int_t           lumi_LLR;
  Long64_t        triggerbit;
  Int_t           metfilterbit;
  Float_t         met_LLR;
  Float_t         metphi_LLR;
  Int_t           npv_LLR;
  Int_t           npu_LLR;
  Float_t         PUReweight;
  Float_t         rho_LLR;
  vector<float>   *mothers_px = 0;
  vector<float>   *mothers_py = 0;
  vector<float>   *mothers_pz = 0;
  vector<float>   *mothers_e = 0;
  vector<float>   *daughters_px = 0;
  vector<float>   *daughters_py = 0;
  vector<float>   *daughters_pz = 0;
  vector<float>   *daughters_e = 0;
  vector<int>     *daughters_charge = 0;
  vector<int>     *daughters_genindex = 0;
  Float_t         MC_weight;
  Float_t         lheHt;
  Float_t         aMCatNLOweight;
  vector<float>   *genpart_px = 0;
  vector<float>   *genpart_py = 0;
  vector<float>   *genpart_pz = 0;
  vector<float>   *genpart_e = 0;
  vector<int>     *genpart_pdg = 0;
  vector<int>     *genpart_status = 0;
  vector<int>     *genpart_HMothInd = 0;
  vector<int>     *genpart_MSSMHMothInd = 0;
  vector<int>     *genpart_TopMothInd = 0;
  vector<int>     *genpart_TauMothInd = 0;
  vector<int>     *genpart_ZMothInd = 0;
  vector<int>     *genpart_WMothInd = 0;
  vector<int>     *genpart_bMothInd = 0;
  vector<int>     *genpart_HZDecayMode = 0;
  vector<int>     *genpart_TopDecayMode = 0;
  vector<int>     *genpart_WDecayMode = 0;
  vector<int>     *genpart_TauGenDecayMode = 0;
  vector<int>     *genpart_flags = 0;
  vector<float>   *genjet_px = 0;
  vector<float>   *genjet_py = 0;
  vector<float>   *genjet_pz = 0;
  vector<float>   *genjet_e = 0;
  vector<int>     *genjet_partonFlavour = 0;
  vector<int>     *genjet_hadronFlavour = 0;
  Int_t           NUP_LLR;
  vector<float>   *SVfitMass = 0;
  vector<float>   *SVfit_pt = 0;
  vector<float>   *SVfit_ptUnc = 0;
  vector<float>   *SVfit_eta = 0;
  vector<float>   *SVfit_etaUnc = 0;
  vector<float>   *SVfit_phi = 0;
  vector<float>   *SVfit_phiUnc = 0;
  vector<float>   *SVfit_fitMETRho = 0;
  vector<float>   *SVfit_fitMETPhi = 0;
  vector<bool>    *isOSCand = 0;
  vector<float>   *METx = 0;
  vector<float>   *METy = 0;
  vector<float>   *MET_cov00 = 0;
  vector<float>   *MET_cov01 = 0;
  vector<float>   *MET_cov10 = 0;
  vector<float>   *MET_cov11 = 0;
  vector<float>   *MET_significance = 0;
  vector<float>   *mT_Dau1 = 0;
  vector<float>   *mT_Dau2 = 0;
  vector<int>     *PDGIdDaughters = 0;
  vector<int>     *indexDau1 = 0;
  vector<int>     *indexDau2 = 0;
  vector<int>     *particleType = 0;
  vector<float>   *discriminator = 0;
  vector<int>     *daughters_muonID = 0;
  vector<int>     *daughters_typeOfMuon = 0;
  vector<float>   *dxy = 0;
  vector<float>   *dz = 0;
  vector<float>   *SIP = 0;
  vector<bool>    *daughters_iseleBDT = 0;
  vector<bool>    *daughters_iseleWP80 = 0;
  vector<bool>    *daughters_iseleWP90 = 0;
  vector<float>   *daughters_eleMVAnt = 0;
  vector<bool>    *daughters_passConversionVeto = 0;
  vector<int>     *daughters_eleMissingHits = 0;
  vector<int>     *daughters_eleCUTID = 0;
  vector<int>     *decayMode = 0;
  vector<Long64_t> *tauID = 0;
  vector<float>   *combreliso = 0;
  vector<float>   *daughters_IetaIeta = 0;
  vector<float>   *daughters_deltaPhiSuperClusterTrackAtVtx = 0;
  vector<float>   *daughters_SCeta = 0;
  vector<float>   *daughters_depositR03_tracker = 0;
  vector<float>   *daughters_depositR03_ecal = 0;
  vector<float>   *daughters_depositR03_hcal = 0;
  vector<int>     *daughters_decayModeFindingOldDMs = 0;
  vector<float>   *againstElectronMVA5category = 0;
  vector<float>   *againstElectronMVA5raw = 0;
  vector<float>   *byPileupWeightedIsolationRaw3Hits = 0;
  vector<float>   *footprintCorrection = 0;
  vector<float>   *neutralIsoPtSumWeight = 0;
  vector<float>   *photonPtSumOutsideSignalCone = 0;
  vector<int>     *daughters_decayModeFindingNewDMs = 0;
  vector<float>   *daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits = 0;
  vector<float>   *daughters_byIsolationMVA3oldDMwoLTraw = 0;
  vector<float>   *daughters_byIsolationMVA3oldDMwLTraw = 0;
  vector<float>   *daughters_byIsolationMVA3newDMwoLTraw = 0;
  vector<float>   *daughters_byIsolationMVA3newDMwLTraw = 0;
  vector<float>   *daughters_chargedIsoPtSum = 0;
  vector<float>   *daughters_neutralIsoPtSum = 0;
  vector<float>   *daughters_puCorrPtSum = 0;
  vector<int>     *daughters_numChargedParticlesSignalCone = 0;
  vector<int>     *daughters_numNeutralHadronsSignalCone = 0;
  vector<int>     *daughters_numPhotonsSignalCone = 0;
  vector<int>     *daughters_daughters_numParticlesSignalCone = 0;
  vector<int>     *daughters_numChargedParticlesIsoCone = 0;
  vector<int>     *daughters_numNeutralHadronsIsoCone = 0;
  vector<int>     *daughters_numPhotonsIsoCone = 0;
  vector<int>     *daughters_numParticlesIsoCone = 0;
  vector<float>   *daughters_leadChargedParticlePt = 0;
  vector<float>   *daughters_trackRefPt = 0;
  vector<int>     *daughters_isLastTriggerObjectforPath = 0;
  vector<int>     *daughters_isTriggerObjectforPath = 0;
  vector<int>     *daughters_FilterFired = 0;
  vector<int>     *daughters_isGoodTriggerType = 0;
  vector<int>     *daughters_L3FilterFired = 0;
  vector<int>     *daughters_L3FilterFiredLast = 0;
  vector<float>   *daughters_HLTpt = 0;
  Int_t           JetsNumber;
  vector<float>   *jets_px = 0;
  vector<float>   *jets_py = 0;
  vector<float>   *jets_pz = 0;
  vector<float>   *jets_e = 0;
  vector<int>     *jets_Flavour = 0;
  vector<int>     *jets_HadronFlavour = 0;
  vector<int>     *jets_genjetIndex = 0;
  vector<float>   *jets_PUJetID = 0;
  vector<float>   *bDiscriminator = 0;
  vector<float>   *bCSVscore = 0;
  vector<int>     *PFjetID = 0;
  vector<float>   *jetRawf = 0;

  m_SampleChain->SetBranchAddress("EventNumber", &EventNumber);
  m_SampleChain->SetBranchAddress("RunNumber", &RunNumber);
  m_SampleChain->SetBranchAddress("lumi", &lumi_LLR);
  m_SampleChain->SetBranchAddress("triggerbit", &triggerbit);
  m_SampleChain->SetBranchAddress("metfilterbit", &metfilterbit);
  m_SampleChain->SetBranchAddress("met", &met_LLR);
  m_SampleChain->SetBranchAddress("metphi", &metphi_LLR);
  m_SampleChain->SetBranchAddress("npv", &npv_LLR);
  m_SampleChain->SetBranchAddress("npu", &npu_LLR);
  m_SampleChain->SetBranchAddress("PUReweight", &PUReweight);
  m_SampleChain->SetBranchAddress("rho", &rho_LLR);
  m_SampleChain->SetBranchAddress("mothers_px", &mothers_px);
  m_SampleChain->SetBranchAddress("mothers_py", &mothers_py);
  m_SampleChain->SetBranchAddress("mothers_pz", &mothers_pz);
  m_SampleChain->SetBranchAddress("mothers_e", &mothers_e);
  m_SampleChain->SetBranchAddress("daughters_px", &daughters_px);
  m_SampleChain->SetBranchAddress("daughters_py", &daughters_py);
  m_SampleChain->SetBranchAddress("daughters_pz", &daughters_pz);
  m_SampleChain->SetBranchAddress("daughters_e", &daughters_e);
  m_SampleChain->SetBranchAddress("daughters_charge", &daughters_charge);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("daughters_genindex", &daughters_genindex);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("MC_weight", &MC_weight);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("lheHt", &lheHt);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("aMCatNLOweight", &aMCatNLOweight);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_px", &genpart_px);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_py", &genpart_py);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_pz", &genpart_pz);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_e", &genpart_e);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_pdg", &genpart_pdg);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_status", &genpart_status);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_HMothInd", &genpart_HMothInd);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_MSSMHMothInd", &genpart_MSSMHMothInd);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_TopMothInd", &genpart_TopMothInd);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_TauMothInd", &genpart_TauMothInd);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_ZMothInd", &genpart_ZMothInd);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_WMothInd", &genpart_WMothInd);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_bMothInd", &genpart_bMothInd);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_HZDecayMode", &genpart_HZDecayMode);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_TopDecayMode", &genpart_TopDecayMode);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_WDecayMode", &genpart_WDecayMode);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_TauGenDecayMode", &genpart_TauGenDecayMode);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genpart_flags", &genpart_flags);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genjet_px", &genjet_px);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genjet_py", &genjet_py);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genjet_pz", &genjet_pz);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genjet_e", &genjet_e);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genjet_partonFlavour", &genjet_partonFlavour);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("genjet_hadronFlavour", &genjet_hadronFlavour);
  m_SampleChain->SetBranchAddress("NUP", &NUP_LLR);
  m_SampleChain->SetBranchAddress("SVfitMass", &SVfitMass);
  m_SampleChain->SetBranchAddress("SVfit_pt", &SVfit_pt);
  m_SampleChain->SetBranchAddress("SVfit_ptUnc", &SVfit_ptUnc);
  m_SampleChain->SetBranchAddress("SVfit_eta", &SVfit_eta);
  m_SampleChain->SetBranchAddress("SVfit_etaUnc", &SVfit_etaUnc);
  m_SampleChain->SetBranchAddress("SVfit_phi", &SVfit_phi);
  m_SampleChain->SetBranchAddress("SVfit_phiUnc", &SVfit_phiUnc);
  m_SampleChain->SetBranchAddress("SVfit_fitMETRho", &SVfit_fitMETRho);
  m_SampleChain->SetBranchAddress("SVfit_fitMETPhi", &SVfit_fitMETPhi);
  m_SampleChain->SetBranchAddress("isOSCand", &isOSCand);
  m_SampleChain->SetBranchAddress("METx", &METx);
  m_SampleChain->SetBranchAddress("METy", &METy);
  m_SampleChain->SetBranchAddress("MET_cov00", &MET_cov00);
  m_SampleChain->SetBranchAddress("MET_cov01", &MET_cov01);
  m_SampleChain->SetBranchAddress("MET_cov10", &MET_cov10);
  m_SampleChain->SetBranchAddress("MET_cov11", &MET_cov11);
  m_SampleChain->SetBranchAddress("MET_significance", &MET_significance);
  m_SampleChain->SetBranchAddress("mT_Dau1", &mT_Dau1);
  m_SampleChain->SetBranchAddress("mT_Dau2", &mT_Dau2);
  m_SampleChain->SetBranchAddress("PDGIdDaughters", &PDGIdDaughters);
  m_SampleChain->SetBranchAddress("indexDau1", &indexDau1);
  m_SampleChain->SetBranchAddress("indexDau2", &indexDau2);
  m_SampleChain->SetBranchAddress("particleType", &particleType);
  m_SampleChain->SetBranchAddress("discriminator", &discriminator);
  m_SampleChain->SetBranchAddress("daughters_muonID", &daughters_muonID);
  m_SampleChain->SetBranchAddress("daughters_typeOfMuon", &daughters_typeOfMuon);
  m_SampleChain->SetBranchAddress("dxy", &dxy);
  m_SampleChain->SetBranchAddress("dz", &dz);
  m_SampleChain->SetBranchAddress("SIP", &SIP);
  m_SampleChain->SetBranchAddress("daughters_iseleBDT", &daughters_iseleBDT);
  m_SampleChain->SetBranchAddress("daughters_iseleWP80", &daughters_iseleWP80);
  m_SampleChain->SetBranchAddress("daughters_iseleWP90", &daughters_iseleWP90);
  m_SampleChain->SetBranchAddress("daughters_eleMVAnt", &daughters_eleMVAnt);
  m_SampleChain->SetBranchAddress("daughters_passConversionVeto", &daughters_passConversionVeto);
  m_SampleChain->SetBranchAddress("daughters_eleMissingHits", &daughters_eleMissingHits);
  m_SampleChain->SetBranchAddress("daughters_eleCUTID", &daughters_eleCUTID);
  m_SampleChain->SetBranchAddress("decayMode", &decayMode);
  m_SampleChain->SetBranchAddress("tauID", &tauID);
  m_SampleChain->SetBranchAddress("combreliso", &combreliso);
  m_SampleChain->SetBranchAddress("daughters_IetaIeta", &daughters_IetaIeta);
  m_SampleChain->SetBranchAddress("daughters_deltaPhiSuperClusterTrackAtVtx", &daughters_deltaPhiSuperClusterTrackAtVtx);
  m_SampleChain->SetBranchAddress("daughters_SCeta", &daughters_SCeta);
  m_SampleChain->SetBranchAddress("daughters_depositR03_tracker", &daughters_depositR03_tracker);
  m_SampleChain->SetBranchAddress("daughters_depositR03_ecal", &daughters_depositR03_ecal);
  m_SampleChain->SetBranchAddress("daughters_depositR03_hcal", &daughters_depositR03_hcal);
  m_SampleChain->SetBranchAddress("daughters_decayModeFindingOldDMs", &daughters_decayModeFindingOldDMs);
  m_SampleChain->SetBranchAddress("againstElectronMVA5category", &againstElectronMVA5category);
  m_SampleChain->SetBranchAddress("againstElectronMVA5raw", &againstElectronMVA5raw);
  m_SampleChain->SetBranchAddress("byPileupWeightedIsolationRaw3Hits", &byPileupWeightedIsolationRaw3Hits);
  m_SampleChain->SetBranchAddress("footprintCorrection", &footprintCorrection);
  m_SampleChain->SetBranchAddress("neutralIsoPtSumWeight", &neutralIsoPtSumWeight);
  m_SampleChain->SetBranchAddress("photonPtSumOutsideSignalCone", &photonPtSumOutsideSignalCone);
  m_SampleChain->SetBranchAddress("daughters_decayModeFindingNewDMs", &daughters_decayModeFindingNewDMs);
  m_SampleChain->SetBranchAddress("daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits", &daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits);
  m_SampleChain->SetBranchAddress("daughters_byIsolationMVA3oldDMwoLTraw", &daughters_byIsolationMVA3oldDMwoLTraw);
  m_SampleChain->SetBranchAddress("daughters_byIsolationMVA3oldDMwLTraw", &daughters_byIsolationMVA3oldDMwLTraw);
  m_SampleChain->SetBranchAddress("daughters_byIsolationMVA3newDMwoLTraw", &daughters_byIsolationMVA3newDMwoLTraw);
  m_SampleChain->SetBranchAddress("daughters_byIsolationMVA3newDMwLTraw", &daughters_byIsolationMVA3newDMwLTraw);
  m_SampleChain->SetBranchAddress("daughters_chargedIsoPtSum", &daughters_chargedIsoPtSum);
  m_SampleChain->SetBranchAddress("daughters_neutralIsoPtSum", &daughters_neutralIsoPtSum);
  m_SampleChain->SetBranchAddress("daughters_puCorrPtSum", &daughters_puCorrPtSum);
  m_SampleChain->SetBranchAddress("daughters_numChargedParticlesSignalCone", &daughters_numChargedParticlesSignalCone);
  m_SampleChain->SetBranchAddress("daughters_numNeutralHadronsSignalCone", &daughters_numNeutralHadronsSignalCone);
  m_SampleChain->SetBranchAddress("daughters_numPhotonsSignalCone", &daughters_numPhotonsSignalCone);
  m_SampleChain->SetBranchAddress("daughters_daughters_numParticlesSignalCone", &daughters_daughters_numParticlesSignalCone);
  m_SampleChain->SetBranchAddress("daughters_numChargedParticlesIsoCone", &daughters_numChargedParticlesIsoCone);
  m_SampleChain->SetBranchAddress("daughters_numNeutralHadronsIsoCone", &daughters_numNeutralHadronsIsoCone);
  m_SampleChain->SetBranchAddress("daughters_numPhotonsIsoCone", &daughters_numPhotonsIsoCone);
  m_SampleChain->SetBranchAddress("daughters_numParticlesIsoCone", &daughters_numParticlesIsoCone);
  m_SampleChain->SetBranchAddress("daughters_leadChargedParticlePt", &daughters_leadChargedParticlePt);
  m_SampleChain->SetBranchAddress("daughters_trackRefPt", &daughters_trackRefPt);
  m_SampleChain->SetBranchAddress("daughters_isLastTriggerObjectforPath", &daughters_isLastTriggerObjectforPath);
  m_SampleChain->SetBranchAddress("daughters_isTriggerObjectforPath", &daughters_isTriggerObjectforPath);
  m_SampleChain->SetBranchAddress("daughters_FilterFired", &daughters_FilterFired);
  m_SampleChain->SetBranchAddress("daughters_isGoodTriggerType", &daughters_isGoodTriggerType);
  m_SampleChain->SetBranchAddress("daughters_L3FilterFired", &daughters_L3FilterFired);
  m_SampleChain->SetBranchAddress("daughters_L3FilterFiredLast", &daughters_L3FilterFiredLast);
  m_SampleChain->SetBranchAddress("daughters_HLTpt", &daughters_HLTpt);
  m_SampleChain->SetBranchAddress("JetsNumber", &JetsNumber);
  m_SampleChain->SetBranchAddress("jets_px", &jets_px);
  m_SampleChain->SetBranchAddress("jets_py", &jets_py);
  m_SampleChain->SetBranchAddress("jets_pz", &jets_pz);
  m_SampleChain->SetBranchAddress("jets_e", &jets_e);
  m_SampleChain->SetBranchAddress("jets_Flavour", &jets_Flavour);
  m_SampleChain->SetBranchAddress("jets_HadronFlavour", &jets_HadronFlavour);
  if(!DataTrue_MCFalse) m_SampleChain->SetBranchAddress("jets_genjetIndex", &jets_genjetIndex);
  m_SampleChain->SetBranchAddress("jets_PUJetID", &jets_PUJetID);
  m_SampleChain->SetBranchAddress("bDiscriminator", &bDiscriminator);
  m_SampleChain->SetBranchAddress("bCSVscore", &bCSVscore);
  m_SampleChain->SetBranchAddress("PFjetID", &PFjetID);
  m_SampleChain->SetBranchAddress("jetRawf", &jetRawf);

  TFile f_SyncTree(OutputFileName.Data(),"RECREATE");

  TTree *SyncTree = new TTree("SyncTree","SyncTree");

  Int_t           run;
  Int_t           lumi;
  Int_t           evt;
  Bool_t          isZtt;
  Bool_t          isZmt;
  Bool_t          isZet;
  Bool_t          isZee;
  Bool_t          isZem;
  Bool_t          isZmm;
  Bool_t          isZEE;
  Bool_t          isZMM;
  Bool_t          isZLL;
  Bool_t          isFake;
  Int_t           NUP;
  Bool_t          secondMuon;
  Float_t         weight;
  Float_t         puweight;
  Float_t         npv;
  Float_t         npu;
  Float_t         rho;
  Float_t         pt_1;
  Float_t         px_1;
  Float_t         py_1;
  Float_t         pz_1;
  Float_t         phi_1;
  Float_t         eta_1;
  Float_t         m_1;
  Float_t         q_1;
  Float_t         d0_1;
  Float_t         dZ_1;
  Float_t         mt_1;
  Float_t         iso_1;
  Float_t         l1_decayMode;
  Bool_t          id_m_loose_1;
  Bool_t          id_m_medium_1;
  Bool_t          id_m_tight_1;
  Bool_t          id_m_tightnovtx_1;
  Bool_t          id_m_highpt_1;
  Bool_t          id_e_mva_nt_loose_1;
  Bool_t          id_e_mva_nt_tight_1;
  Bool_t          id_e_cut_veto_1;
  Bool_t          id_e_cut_loose_1;
  Bool_t          id_e_cut_medium_1;
  Bool_t          id_e_cut_tight_1;
  Float_t         trigweight_1;
  Bool_t          againstElectronLooseMVA5_1;
  Bool_t          againstElectronMediumMVA5_1;
  Bool_t          againstElectronTightMVA5_1;
  Bool_t          againstElectronVLooseMVA5_1;
  Bool_t          againstElectronVTightMVA5_1;
  Bool_t          againstMuonLoose3_1;
  Bool_t          againstMuonTight3_1;
  Float_t         byCombinedIsolationDeltaBetaCorrRaw3Hits_1;
  Float_t         byIsolationMVA3oldDMwoLTraw_1;
  Float_t         byIsolationMVA3oldDMwLTraw_1;
  Float_t         byIsolationMVA3newDMwoLTraw_1;
  Float_t         byIsolationMVA3newDMwLTraw_1;
  Float_t         chargedIsoPtSum_1;
  Bool_t          decayModeFinding_1;
  Bool_t          decayModeFindingNewDMs_1;
  Float_t         neutralIsoPtSum_1;
  Float_t         puCorrPtSum_1;
  Float_t         pt_2;
  Float_t         phi_2;
  Float_t         eta_2;
  Float_t         px_2;
  Float_t         py_2;
  Float_t         pz_2;
  Float_t         m_2;
  Float_t         q_2;
  Float_t         d0_2;
  Float_t         dZ_2;
  Float_t         mt_2;
  Float_t         iso_2;
  Float_t         l2_decayMode;
  Bool_t          id_m_loose_2;
  Bool_t          id_m_medium_2;
  Bool_t          id_m_tight_2;
  Bool_t          id_m_tightnovtx_2;
  Bool_t          id_m_highpt_2;
  Bool_t          id_e_mva_nt_loose_2;
  Bool_t          id_e_mva_nt_tight_2;
  Bool_t          id_e_cut_veto_2;
  Bool_t          id_e_cut_loose_2;
  Bool_t          id_e_cut_medium_2;
  Bool_t          id_e_cut_tight_2;
  Float_t         trigweight_2;
  Bool_t          againstElectronLooseMVA5_2;
  Bool_t          againstElectronMediumMVA5_2;
  Bool_t          againstElectronTightMVA5_2;
  Bool_t          againstElectronVLooseMVA5_2;
  Bool_t          againstElectronVTightMVA5_2;
  Bool_t          againstMuonLoose3_2;
  Bool_t          againstMuonTight3_2;
  Float_t         byCombinedIsolationDeltaBetaCorrRaw3Hits_2;
  Float_t         byIsolationMVA3oldDMwoLTraw_2;
  Float_t         byIsolationMVA3oldDMwLTraw_2;
  Float_t         byIsolationMVA3newDMwoLTraw_2;
  Float_t         byIsolationMVA3newDMwLTraw_2;
  Float_t         chargedIsoPtSum_2;
  Bool_t          decayModeFinding_2;
  Bool_t          decayModeFindingNewDMs_2;
  Float_t         neutralIsoPtSum_2;
  Float_t         puCorrPtSum_2;
  Float_t         pth;
  Float_t         m_vis;
  Float_t         m_sv;
  Float_t         pt_sv;
  Float_t         eta_sv;
  Float_t         phi_sv;
  Float_t         met_sv;
  Float_t         met;
  Float_t         metphi;
  Float_t         mvamet;
  Float_t         mvametphi;
  Float_t         pzetavis;
  Float_t         pzetamiss;
  Float_t         mvacov00;
  Float_t         mvacov10;
  Float_t         mvacov01;
  Float_t         mvacov11;
  Float_t         mjj;
  Float_t         jdeta;
  Int_t           njetingap;
  Float_t         jdphi;
  Float_t         dijetpt;
  Float_t         dijetphi;
  Float_t         hdijetphi;
  Float_t         visjeteta;
  Float_t         ptvis;
  Int_t           nbtag;
  Int_t           njets;
  Int_t           njetspt20;
  Float_t         jpt_1;
  Float_t         jeta_1;
  Float_t         jphi_1;
  Float_t         jrawf_1;
  Float_t         jmva_1;
  Float_t         jpfid_1;
  Float_t         jpuid_1;
  Float_t         jcsv_1;
  Float_t         jpt_2;
  Float_t         jeta_2;
  Float_t         jphi_2;
  Float_t         jrawf_2;
  Float_t         jmva_2;
  Float_t         jpfid_2;
  Float_t         jpuid_2;
  Float_t         jcsv_2;
  Float_t         bjpt_1;
  Float_t         bjeta_1;
  Float_t         bjphi_1;
  Float_t         bjrawf_1;
  Float_t         bjmva_1;
  Float_t         bjpfid_1;
  Float_t         bjpuid_1;
  Float_t         bjcsv_1;
  Float_t         bjpt_2;
  Float_t         bjeta_2;
  Float_t         bjphi_2;
  Float_t         bjrawf_2;
  Float_t         bjmva_2;
  Float_t         bjpfid_2;
  Float_t         bjpuid_2;
  Float_t         bjcsv_2;
  Int_t           nb_extra_electrons;
  Int_t           nb_extra_muons;
  Float_t         gen_Higgs_pt;
  Float_t         gen_Higgs_mass;
  Bool_t isLeg1MatchedToTauh = kFALSE;
  Bool_t isLeg1MatchedToTaum = kFALSE;
  Bool_t isLeg1MatchedToTaue = kFALSE;
  Bool_t isLeg2MatchedToTauh = kFALSE;
  Bool_t isLeg2MatchedToTaum = kFALSE;
  Bool_t isLeg2MatchedToTaue = kFALSE;
  Bool_t isLeg1FromSMHiggs = kFALSE;
  Bool_t isLeg2FromSMHiggs = kFALSE;
  Bool_t isLeg1FromMSSMHiggs = kFALSE;
  Bool_t isLeg2FromMSSMHiggs = kFALSE;
  Float_t Leg1MatchPt = -99.;
  Float_t Leg1MatchEta = -99.;
  Float_t Leg1MatchPhi = -99.;
  Float_t Leg1MatchE = -99.;
  Float_t Leg2MatchPt = -99.;
  Float_t Leg2MatchEta = -99.;
  Float_t Leg2MatchPhi = -99.;
  Float_t Leg2MatchE = -99.;
  Float_t TriggerWeightLeg1Stage2_30GeV = -99.;
  Float_t TriggerWeightLeg1Stage2_35GeV = -99.;
  Float_t TriggerWeightLeg1Stage2_42GeV = -99.;
  Float_t TriggerWeightLeg1Stage2_70GeV = -99.;
  Float_t TriggerWeightLeg2Stage2_30GeV = -99.;
  Float_t TriggerWeightLeg2Stage2_35GeV = -99.;
  Float_t TriggerWeightLeg2Stage2_42GeV = -99.;
  Float_t TriggerWeightLeg2Stage2_70GeV = -99.;
  Float_t TriggerWeightLeg1Stage1_45GeV = -99.;
  Float_t TriggerWeightLeg2Stage1_45GeV = -99.;
  Float_t TriggerWeightLeg1Stage2 = -99.;
  Float_t TriggerWeightLeg2Stage2 = -99.;

  TBranch*        b_run = SyncTree->Branch("run",&run);
  TBranch*        b_lumi = SyncTree->Branch("lumi",&lumi);
  TBranch*        b_evt = SyncTree->Branch("evt",&evt);
  TBranch*        b_isZtt = SyncTree->Branch("isZtt",&isZtt);
  TBranch*        b_isZmt = SyncTree->Branch("isZmt",&isZmt);
  TBranch*        b_isZet = SyncTree->Branch("isZet",&isZet);
  TBranch*        b_isZee = SyncTree->Branch("isZee",&isZee);
  TBranch*        b_isZem = SyncTree->Branch("isZem",&isZem);
  TBranch*        b_isZmm = SyncTree->Branch("isZmm",&isZmm);
  TBranch*        b_isZEE = SyncTree->Branch("isZEE",&isZEE);
  TBranch*        b_isZMM = SyncTree->Branch("isZMM",&isZMM);
  TBranch*        b_isZLL = SyncTree->Branch("isZLL",&isZLL);
  TBranch*        b_isFake = SyncTree->Branch("isFake",&isFake);
  TBranch*        b_NUP = SyncTree->Branch("NUP",&NUP);
  TBranch*        b_secondMuon = SyncTree->Branch("secondMuon",&secondMuon);
  TBranch*        b_weight = SyncTree->Branch("weight",&weight);
  TBranch*        b_puweight = SyncTree->Branch("puweight",&puweight);
  TBranch*        b_npv = SyncTree->Branch("npv",&npv);
  TBranch*        b_npu = SyncTree->Branch("npu",&npu);
  TBranch*        b_rho = SyncTree->Branch("rho",&rho);
  TBranch*        b_pt_1 = SyncTree->Branch("pt_1",&pt_1);
  TBranch*        b_px_1 = SyncTree->Branch("px_1",&px_1);
  TBranch*        b_py_1 = SyncTree->Branch("py_1",&py_1);
  TBranch*        b_pz_1 = SyncTree->Branch("pz_1",&pz_1);
  TBranch*        b_phi_1 = SyncTree->Branch("phi_1",&phi_1);
  TBranch*        b_eta_1 = SyncTree->Branch("eta_1",&eta_1);
  TBranch*        b_m_1 = SyncTree->Branch("m_1",&m_1);
  TBranch*        b_q_1 = SyncTree->Branch("q_1",&q_1);
  TBranch*        b_d0_1 = SyncTree->Branch("d0_1",&d0_1);
  TBranch*        b_dZ_1 = SyncTree->Branch("dZ_1",&dZ_1);
  TBranch*        b_mt_1 = SyncTree->Branch("mt_1",&mt_1);
  TBranch*        b_iso_1 = SyncTree->Branch("iso_1",&iso_1);
  TBranch*        b_l1_decayMode = SyncTree->Branch("l1_decayMode",&l1_decayMode);
  TBranch*        b_id_m_loose_1 = SyncTree->Branch("id_m_loose_1",&id_m_loose_1);
  TBranch*        b_id_m_medium_1 = SyncTree->Branch("id_m_medium_1",&id_m_medium_1);
  TBranch*        b_id_m_tight_1 = SyncTree->Branch("id_m_tight_1",&id_m_tight_1);
  TBranch*        b_id_m_tightnovtx_1 = SyncTree->Branch("id_m_tightnovtx_1",&id_m_tightnovtx_1);
  TBranch*        b_id_m_highpt_1 = SyncTree->Branch("id_m_highpt_1",&id_m_highpt_1);
  TBranch*        b_id_e_mva_nt_loose_1 = SyncTree->Branch("id_e_mva_nt_loose_1",&id_e_mva_nt_loose_1);
  TBranch*        b_id_e_mva_nt_tight_1 = SyncTree->Branch("id_e_mva_nt_tight_1",&id_e_mva_nt_tight_1);
  TBranch*        b_id_e_cut_veto_1 = SyncTree->Branch("id_e_cut_veto_1",&id_e_cut_veto_1);
  TBranch*        b_id_e_cut_loose_1 = SyncTree->Branch("id_e_cut_loose_1",&id_e_cut_loose_1);
  TBranch*        b_id_e_cut_medium_1 = SyncTree->Branch("id_e_cut_medium_1",&id_e_cut_medium_1);
  TBranch*        b_id_e_cut_tight_1 = SyncTree->Branch("id_e_cut_tight_1",&id_e_cut_tight_1);
  TBranch*        b_trigweight_1 = SyncTree->Branch("trigweight_1",&trigweight_1);
  TBranch*        b_againstElectronLooseMVA5_1 = SyncTree->Branch("againstElectronLooseMVA5_1",&againstElectronLooseMVA5_1);
  TBranch*        b_againstElectronMediumMVA5_1 = SyncTree->Branch("againstElectronMediumMVA5_1",&againstElectronMediumMVA5_1);
  TBranch*        b_againstElectronTightMVA5_1 = SyncTree->Branch("againstElectronTightMVA5_1",&againstElectronTightMVA5_1);
  TBranch*        b_againstElectronVLooseMVA5_1 = SyncTree->Branch("againstElectronVLooseMVA5_1",&againstElectronVLooseMVA5_1);
  TBranch*        b_againstElectronVTightMVA5_1 = SyncTree->Branch("againstElectronVTightMVA5_1",&againstElectronVTightMVA5_1);
  TBranch*        b_againstMuonLoose3_1 = SyncTree->Branch("againstMuonLoose3_1",&againstMuonLoose3_1);
  TBranch*        b_againstMuonTight3_1 = SyncTree->Branch("againstMuonTight3_1",&againstMuonTight3_1);
  TBranch*        b_byCombinedIsolationDeltaBetaCorrRaw3Hits_1 = SyncTree->Branch("byCombinedIsolationDeltaBetaCorrRaw3Hits_1",&byCombinedIsolationDeltaBetaCorrRaw3Hits_1);
  TBranch*        b_byIsolationMVA3oldDMwoLTraw_1 = SyncTree->Branch("byIsolationMVA3oldDMwoLTraw_1",&byIsolationMVA3oldDMwoLTraw_1);
  TBranch*        b_byIsolationMVA3oldDMwLTraw_1 = SyncTree->Branch("byIsolationMVA3oldDMwLTraw_1",&byIsolationMVA3oldDMwLTraw_1);
  TBranch*        b_byIsolationMVA3newDMwoLTraw_1 = SyncTree->Branch("byIsolationMVA3newDMwoLTraw_1",&byIsolationMVA3newDMwoLTraw_1);
  TBranch*        b_byIsolationMVA3newDMwLTraw_1 = SyncTree->Branch("byIsolationMVA3newDMwLTraw_1",&byIsolationMVA3newDMwLTraw_1);
  TBranch*        b_chargedIsoPtSum_1 = SyncTree->Branch("chargedIsoPtSum_1",&chargedIsoPtSum_1);
  TBranch*        b_decayModeFinding_1 = SyncTree->Branch("decayModeFinding_1",&decayModeFinding_1);
  TBranch*        b_decayModeFindingNewDMs_1 = SyncTree->Branch("decayModeFindingNewDMs_1",&decayModeFindingNewDMs_1);
  TBranch*        b_neutralIsoPtSum_1 = SyncTree->Branch("neutralIsoPtSum_1",&neutralIsoPtSum_1);
  TBranch*        b_puCorrPtSum_1 = SyncTree->Branch("puCorrPtSum_1",&puCorrPtSum_1);
  TBranch*        b_pt_2 = SyncTree->Branch("pt_2",&pt_2);
  TBranch*        b_phi_2 = SyncTree->Branch("phi_2",&phi_2);
  TBranch*        b_eta_2 = SyncTree->Branch("eta_2",&eta_2);
  TBranch*        b_px_2 = SyncTree->Branch("px_2",&px_2);
  TBranch*        b_py_2 = SyncTree->Branch("py_2",&py_2);
  TBranch*        b_pz_2 = SyncTree->Branch("pz_2",&pz_2);
  TBranch*        b_m_2 = SyncTree->Branch("m_2",&m_2);
  TBranch*        b_q_2 = SyncTree->Branch("q_2",&q_2);
  TBranch*        b_d0_2 = SyncTree->Branch("d0_2",&d0_2);
  TBranch*        b_dZ_2 = SyncTree->Branch("dZ_2",&dZ_2);
  TBranch*        b_mt_2 = SyncTree->Branch("mt_2",&mt_2);
  TBranch*        b_iso_2 = SyncTree->Branch("iso_2",&iso_2);
  TBranch*        b_l2_decayMode = SyncTree->Branch("l2_decayMode",&l2_decayMode);
  TBranch*        b_id_m_loose_2 = SyncTree->Branch("id_m_loose_2",&id_m_loose_2);
  TBranch*        b_id_m_medium_2 = SyncTree->Branch("id_m_medium_2",&id_m_medium_2);
  TBranch*        b_id_m_tight_2 = SyncTree->Branch("id_m_tight_2",&id_m_tight_2);
  TBranch*        b_id_m_tightnovtx_2 = SyncTree->Branch("id_m_tightnovtx_2",&id_m_tightnovtx_2);
  TBranch*        b_id_m_highpt_2 = SyncTree->Branch("id_m_highpt_2",&id_m_highpt_2);
  TBranch*        b_id_e_mva_nt_loose_2 = SyncTree->Branch("id_e_mva_nt_loose_2",&id_e_mva_nt_loose_2);
  TBranch*        b_id_e_mva_nt_tight_2 = SyncTree->Branch("id_e_mva_nt_tight_2",&id_e_mva_nt_tight_2);
  TBranch*        b_id_e_cut_veto_2 = SyncTree->Branch("id_e_cut_veto_2",&id_e_cut_veto_2);
  TBranch*        b_id_e_cut_loose_2 = SyncTree->Branch("id_e_cut_loose_2",&id_e_cut_loose_2);
  TBranch*        b_id_e_cut_medium_2 = SyncTree->Branch("id_e_cut_medium_2",&id_e_cut_medium_2);
  TBranch*        b_id_e_cut_tight_2 = SyncTree->Branch("id_e_cut_tight_2",&id_e_cut_tight_2);
  TBranch*        b_trigweight_2 = SyncTree->Branch("trigweight_2",&trigweight_2);
  TBranch*        b_againstElectronLooseMVA5_2 = SyncTree->Branch("againstElectronLooseMVA5_2",&againstElectronLooseMVA5_2);
  TBranch*        b_againstElectronMediumMVA5_2 = SyncTree->Branch("againstElectronMediumMVA5_2",&againstElectronMediumMVA5_2);
  TBranch*        b_againstElectronTightMVA5_2 = SyncTree->Branch("againstElectronTightMVA5_2",&againstElectronTightMVA5_2);
  TBranch*        b_againstElectronVLooseMVA5_2 = SyncTree->Branch("againstElectronVLooseMVA5_2",&againstElectronVLooseMVA5_2);
  TBranch*        b_againstElectronVTightMVA5_2 = SyncTree->Branch("againstElectronVTightMVA5_2",&againstElectronVTightMVA5_2);
  TBranch*        b_againstMuonLoose3_2 = SyncTree->Branch("againstMuonLoose3_2",&againstMuonLoose3_2);
  TBranch*        b_againstMuonTight3_2 = SyncTree->Branch("againstMuonTight3_2",&againstMuonTight3_2);
  TBranch*        b_byCombinedIsolationDeltaBetaCorrRaw3Hits_2 = SyncTree->Branch("byCombinedIsolationDeltaBetaCorrRaw3Hits_2",&byCombinedIsolationDeltaBetaCorrRaw3Hits_2);
  TBranch*        b_byIsolationMVA3oldDMwoLTraw_2 = SyncTree->Branch("byIsolationMVA3oldDMwoLTraw_2",&byIsolationMVA3oldDMwoLTraw_2);
  TBranch*        b_byIsolationMVA3oldDMwLTraw_2 = SyncTree->Branch("byIsolationMVA3oldDMwLTraw_2",&byIsolationMVA3oldDMwLTraw_2);
  TBranch*        b_byIsolationMVA3newDMwoLTraw_2 = SyncTree->Branch("byIsolationMVA3newDMwoLTraw_2",&byIsolationMVA3newDMwoLTraw_2);
  TBranch*        b_byIsolationMVA3newDMwLTraw_2 = SyncTree->Branch("byIsolationMVA3newDMwLTraw_2",&byIsolationMVA3newDMwLTraw_2);
  TBranch*        b_chargedIsoPtSum_2 = SyncTree->Branch("chargedIsoPtSum_2",&chargedIsoPtSum_2);
  TBranch*        b_decayModeFinding_2 = SyncTree->Branch("decayModeFinding_2",&decayModeFinding_2);
  TBranch*        b_decayModeFindingNewDMs_2 = SyncTree->Branch("decayModeFindingNewDMs_2",&decayModeFindingNewDMs_2);
  TBranch*        b_neutralIsoPtSum_2 = SyncTree->Branch("neutralIsoPtSum_2",&neutralIsoPtSum_2);
  TBranch*        b_puCorrPtSum_2 = SyncTree->Branch("puCorrPtSum_2",&puCorrPtSum_2);
  TBranch*        b_pth = SyncTree->Branch("pth",&pth );
  TBranch*        b_m_vis = SyncTree->Branch("m_vis",&m_vis);
  TBranch*        b_m_sv = SyncTree->Branch("m_sv",&m_sv);
  TBranch*        b_pt_sv = SyncTree->Branch("pt_sv",&pt_sv);
  TBranch*        b_eta_sv = SyncTree->Branch("eta_sv",&eta_sv);
  TBranch*        b_phi_sv = SyncTree->Branch("phi_sv",&phi_sv);
  TBranch*        b_met_sv = SyncTree->Branch("met_sv",&met_sv);
  TBranch*        b_met = SyncTree->Branch("met",&met );
  TBranch*        b_metphi = SyncTree->Branch("metphi",&metphi );
  TBranch*        b_mvamet = SyncTree->Branch("mvamet",&mvamet );
  TBranch*        b_mvametphi = SyncTree->Branch("mvametphi",&mvametphi );
  TBranch*        b_pzetavis = SyncTree->Branch("pzetavis",&pzetavis );
  TBranch*        b_pzetamiss = SyncTree->Branch("pzetamiss",&pzetamiss );
  TBranch*        b_mvacov00 = SyncTree->Branch("mvacov00",&mvacov00 );
  TBranch*        b_mvacov10 = SyncTree->Branch("mvacov10",&mvacov10 );
  TBranch*        b_mvacov01 = SyncTree->Branch("mvacov01",&mvacov01 );
  TBranch*        b_mvacov11 = SyncTree->Branch("mvacov11",&mvacov11 );
  TBranch*        b_mjj = SyncTree->Branch("mjj",&mjj );
  TBranch*        b_jdeta = SyncTree->Branch("jdeta",&jdeta );
  TBranch*        b_njetingap = SyncTree->Branch("njetingap",&njetingap );
  TBranch*        b_jdphi = SyncTree->Branch("jdphi",&jdphi );
  TBranch*        b_dijetpt = SyncTree->Branch("dijetpt",&dijetpt );
  TBranch*        b_dijetphi = SyncTree->Branch("dijetphi",&dijetphi );
  TBranch*        b_hdijetphi = SyncTree->Branch("hdijetphi",&hdijetphi );
  TBranch*        b_visjeteta = SyncTree->Branch("visjeteta",&visjeteta );
  TBranch*        b_ptvis = SyncTree->Branch("ptvis",&ptvis );
  TBranch*        b_nbtag = SyncTree->Branch("nbtag",&nbtag );
  TBranch*        b_njets = SyncTree->Branch("njets",&njets );
  TBranch*        b_njetspt20 = SyncTree->Branch("njetspt20",&njetspt20 );
  TBranch*        b_jpt_1 = SyncTree->Branch("jpt_1",&jpt_1);
  TBranch*        b_jeta_1 = SyncTree->Branch("jeta_1",&jeta_1);
  TBranch*        b_jphi_1 = SyncTree->Branch("jphi_1",&jphi_1);
  TBranch*        b_jrawf_1 = SyncTree->Branch("jrawf_1",&jrawf_1);
  TBranch*        b_jmva_1 = SyncTree->Branch("jmva_1",&jmva_1);
  TBranch*        b_jpfid_1 = SyncTree->Branch("jpfid_1",&jpfid_1);
  TBranch*        b_jpuid_1 = SyncTree->Branch("jpuid_1",&jpuid_1);
  TBranch*        b_jcsv_1 = SyncTree->Branch("jcsv_1",&jcsv_1);
  TBranch*        b_jpt_2 = SyncTree->Branch("jpt_2",&jpt_2);
  TBranch*        b_jeta_2 = SyncTree->Branch("jeta_2",&jeta_2);
  TBranch*        b_jphi_2 = SyncTree->Branch("jphi_2",&jphi_2);
  TBranch*        b_jrawf_2 = SyncTree->Branch("jrawf_2",&jrawf_2);
  TBranch*        b_jmva_2 = SyncTree->Branch("jmva_2",&jmva_2);
  TBranch*        b_jpfid_2 = SyncTree->Branch("jpfid_2",&jpfid_2);
  TBranch*        b_jpuid_2 = SyncTree->Branch("jpuid_2",&jpuid_2);
  TBranch*        b_jcsv_2 = SyncTree->Branch("jcsv_2",&jcsv_2);
  TBranch*        b_bjpt_1 = SyncTree->Branch("bjpt_1",&bjpt_1);
  TBranch*        b_bjeta_1 = SyncTree->Branch("bjeta_1",&bjeta_1);
  TBranch*        b_bjphi_1 = SyncTree->Branch("bjphi_1",&bjphi_1);
  TBranch*        b_bjrawf_1 = SyncTree->Branch("bjrawf_1",&bjrawf_1);
  TBranch*        b_bjmva_1 = SyncTree->Branch("bjmva_1",&bjmva_1);
  TBranch*        b_bjpfid_1 = SyncTree->Branch("bjpfid_1",&bjpfid_1);
  TBranch*        b_bjpuid_1 = SyncTree->Branch("bjpuid_1",&bjpuid_1);
  TBranch*        b_bjcsv_1 = SyncTree->Branch("bjcsv_1",&bjcsv_1);
  TBranch*        b_bjpt_2 = SyncTree->Branch("bjpt_2",&bjpt_2);
  TBranch*        b_bjeta_2 = SyncTree->Branch("bjeta_2",&bjeta_2);
  TBranch*        b_bjphi_2 = SyncTree->Branch("bjphi_2",&bjphi_2);
  TBranch*        b_bjrawf_2 = SyncTree->Branch("bjrawf_2",&bjrawf_2);
  TBranch*        b_bjmva_2 = SyncTree->Branch("bjmva_2",&bjmva_2);
  TBranch*        b_bjpfid_2 = SyncTree->Branch("bjpfid_2",&bjpfid_2);
  TBranch*        b_bjpuid_2 = SyncTree->Branch("bjpuid_2",&bjpuid_2);
  TBranch*        b_bjcsv_2 = SyncTree->Branch("bjcsv_2",&bjcsv_2);
  TBranch*        b_nb_extra_electrons = SyncTree->Branch("nb_extra_electrons",&nb_extra_electrons);
  TBranch*        b_nb_extra_muons = SyncTree->Branch("nb_extra_muons",&nb_extra_muons);
  TBranch*        b_gen_Higgs_pt = SyncTree->Branch("gen_Higgs_pt",&gen_Higgs_pt);
  TBranch*        b_gen_Higgs_mass = SyncTree->Branch("gen_Higgs_mass",&gen_Higgs_mass);
  TBranch*        b_isLeg1MatchedToTauh = SyncTree->Branch("isLeg1MatchedToTauh",&isLeg1MatchedToTauh);
  TBranch*        b_isLeg1MatchedToTaum = SyncTree->Branch("isLeg1MatchedToTaum",&isLeg1MatchedToTaum);
  TBranch*        b_isLeg1MatchedToTaue = SyncTree->Branch("isLeg1MatchedToTaue",&isLeg1MatchedToTaue);
  TBranch*        b_isLeg2MatchedToTauh = SyncTree->Branch("isLeg2MatchedToTauh",&isLeg2MatchedToTauh);
  TBranch*        b_isLeg2MatchedToTaum = SyncTree->Branch("isLeg2MatchedToTaum",&isLeg2MatchedToTaum);
  TBranch*        b_isLeg2MatchedToTaue = SyncTree->Branch("isLeg2MatchedToTaue",&isLeg2MatchedToTaue);
  TBranch*        b_isLeg1FromSMHiggs = SyncTree->Branch("isLeg1FromSMHiggs",&isLeg1FromSMHiggs);
  TBranch*        b_isLeg2FromSMHiggs = SyncTree->Branch("isLeg2FromSMHiggs",&isLeg2FromSMHiggs);
  TBranch*        b_isLeg1FromMSSMHiggs = SyncTree->Branch("isLeg1FromMSSMHiggs",&isLeg1FromMSSMHiggs);
  TBranch*        b_isLeg2FromMSSMHiggs = SyncTree->Branch("isLeg2FromMSSMHiggs",&isLeg2FromMSSMHiggs);
  TBranch*        b_Leg1MatchPt        = SyncTree->Branch("Leg1MatchPt",&Leg1MatchPt);
  TBranch*        b_Leg1MatchEta       = SyncTree->Branch("Leg1MatchEta",&Leg1MatchEta);
  TBranch*        b_Leg1MatchPhi       = SyncTree->Branch("Leg1MatchPhi",&Leg1MatchPhi);
  TBranch*        b_Leg1MatchE         = SyncTree->Branch("Leg1MatchE",&Leg1MatchE);
  TBranch*        b_Leg2MatchPt        = SyncTree->Branch("Leg2MatchPt",&Leg2MatchPt);
  TBranch*        b_Leg2MatchEta       = SyncTree->Branch("Leg2MatchEta",&Leg2MatchEta);
  TBranch*        b_Leg2MatchPhi       = SyncTree->Branch("Leg2MatchPhi",&Leg2MatchPhi);
  TBranch*        b_Leg2MatchE         = SyncTree->Branch("Leg2MatchE",&Leg2MatchE);
  TBranch*        b_TriggerWeightLeg1Stage2_30GeV = SyncTree->Branch("TriggerWeightLeg1Stage2_30GeV",&TriggerWeightLeg1Stage2_30GeV);
  TBranch*        b_TriggerWeightLeg1Stage2_35GeV = SyncTree->Branch("TriggerWeightLeg1Stage2_35GeV",&TriggerWeightLeg1Stage2_35GeV);
  TBranch*        b_TriggerWeightLeg1Stage2_42GeV = SyncTree->Branch("TriggerWeightLeg1Stage2_42GeV",&TriggerWeightLeg1Stage2_42GeV);
  TBranch*        b_TriggerWeightLeg1Stage2_70GeV = SyncTree->Branch("TriggerWeightLeg1Stage2_70GeV",&TriggerWeightLeg1Stage2_70GeV);
  TBranch*        b_TriggerWeightLeg2Stage2_30GeV = SyncTree->Branch("TriggerWeightLeg2Stage2_30GeV",&TriggerWeightLeg2Stage2_30GeV);
  TBranch*        b_TriggerWeightLeg2Stage2_35GeV = SyncTree->Branch("TriggerWeightLeg2Stage2_35GeV",&TriggerWeightLeg2Stage2_35GeV);
  TBranch*        b_TriggerWeightLeg2Stage2_42GeV = SyncTree->Branch("TriggerWeightLeg2Stage2_42GeV",&TriggerWeightLeg2Stage2_42GeV);
  TBranch*        b_TriggerWeightLeg2Stage2_70GeV = SyncTree->Branch("TriggerWeightLeg2Stage2_70GeV",&TriggerWeightLeg2Stage2_70GeV);
  TBranch*        b_TriggerWeightLeg1Stage1_45GeV = SyncTree->Branch("TriggerWeightLeg1Stage1_45GeV",&TriggerWeightLeg1Stage1_45GeV);
  TBranch*        b_TriggerWeightLeg2Stage1_45GeV = SyncTree->Branch("TriggerWeightLeg2Stage1_45GeV",&TriggerWeightLeg2Stage1_45GeV);

  TBranch*        b_TriggerWeightLeg1Stage2 = SyncTree->Branch("TriggerWeightLeg1Stage2",&TriggerWeightLeg1Stage2);
  TBranch*        b_TriggerWeightLeg2Stage2 = SyncTree->Branch("TriggerWeightLeg2Stage2",&TriggerWeightLeg2Stage2);


  TTree *TruthTree = new TTree("TruthTree","TruthTree");
  Int_t           gen_ntau = -99;
  Int_t           gen_ntauh = -99;
  Int_t           gen_ntaum = -99;
  Int_t           gen_ntaue = -99;
  Float_t         gen_Higgs_PDG = -99;
  Float_t         gen_truth_Higgs_pt = -99;
  Float_t         gen_Higgs_eta = -99;
  Float_t         gen_Higgs_phi = -99;
  Float_t         gen_Higgs_e = -99;
  Float_t         gen_tau1_PDG = -99;
  Float_t         gen_tau1_pt = -99;
  Float_t         gen_tau1_eta = -99;
  Float_t         gen_tau1_phi = -99;
  Float_t         gen_tau1_e = -99;
  Float_t         gen_tau2_PDG = -99;
  Float_t         gen_tau2_pt = -99;
  Float_t         gen_tau2_eta = -99;
  Float_t         gen_tau2_phi = -99;
  Float_t         gen_tau2_e = -99;
  Float_t         gen_lep1_PDG = -99;
  Float_t         gen_lep1_pt = -99;
  Float_t         gen_lep1_eta = -99;
  Float_t         gen_lep1_phi = -99;
  Float_t         gen_lep1_e = -99;
  Float_t         gen_lep2_PDG = -99;
  Float_t         gen_lep2_pt = -99;
  Float_t         gen_lep2_eta = -99;
  Float_t         gen_lep2_phi = -99;
  Float_t         gen_lep2_e = -99;
  TBranch*        b_gen_ntau  = TruthTree->Branch("gen_ntau",&gen_ntau);
  TBranch*        b_gen_ntauh = TruthTree->Branch("gen_ntauh",&gen_ntauh);
  TBranch*        b_gen_ntaum = TruthTree->Branch("gen_ntaum",&gen_ntaum);
  TBranch*        b_gen_ntaue = TruthTree->Branch("gen_ntaue",&gen_ntaue);
  TBranch*        b_gen_tau1_PDG = TruthTree->Branch("gen_tau1_PDG",&gen_tau1_PDG);
  TBranch*        b_gen_tau1_pt  = TruthTree->Branch("gen_tau1_pt",&gen_tau1_pt);
  TBranch*        b_gen_tau1_eta  = TruthTree->Branch("gen_tau1_eta",&gen_tau1_eta);
  TBranch*        b_gen_tau1_phi  = TruthTree->Branch("gen_tau1_phi",&gen_tau1_phi);
  TBranch*        b_gen_tau1_e  = TruthTree->Branch("gen_tau1_e",&gen_tau1_e); 
  TBranch*        b_gen_Higgs_PDG = TruthTree->Branch("gen_Higgs_PDG",&gen_Higgs_PDG);
  TBranch*        b_gen_truth_Higgs_pt  = TruthTree->Branch("gen_Higgs_pt",&gen_truth_Higgs_pt);
  TBranch*        b_gen_Higgs_eta  = TruthTree->Branch("gen_Higgs_eta",&gen_Higgs_eta);
  TBranch*        b_gen_Higgs_phi  = TruthTree->Branch("gen_Higgs_phi",&gen_Higgs_phi);
  TBranch*        b_gen_Higgs_e  = TruthTree->Branch("gen_Higgs_e",&gen_Higgs_e); 
  TBranch*        b_gen_tau2_PDG = TruthTree->Branch("gen_tau2_PDG",&gen_tau2_PDG);
  TBranch*        b_gen_tau2_pt  = TruthTree->Branch("gen_tau2_pt",&gen_tau2_pt);
  TBranch*        b_gen_tau2_eta  = TruthTree->Branch("gen_tau2_eta",&gen_tau2_eta);
  TBranch*        b_gen_tau2_phi  = TruthTree->Branch("gen_tau2_phi",&gen_tau2_phi);
  TBranch*        b_gen_tau2_e  = TruthTree->Branch("gen_tau2_e",&gen_tau2_e); 
  TBranch*        b_gen_lep1_PDG = TruthTree->Branch("gen_lep1_PDG",&gen_lep1_PDG);
  TBranch*        b_gen_lep1_pt  = TruthTree->Branch("gen_lep1_pt",&gen_lep1_pt);
  TBranch*        b_gen_lep1_eta  = TruthTree->Branch("gen_lep1_eta",&gen_lep1_eta);
  TBranch*        b_gen_lep1_phi  = TruthTree->Branch("gen_lep1_phi",&gen_lep1_phi);
  TBranch*        b_gen_lep1_e  = TruthTree->Branch("gen_lep1_e",&gen_lep1_e);
  TBranch*        b_gen_lep2_PDG = TruthTree->Branch("gen_lep2_PDG",&gen_lep2_PDG);
  TBranch*        b_gen_lep2_pt  = TruthTree->Branch("gen_lep2_pt",&gen_lep2_pt);
  TBranch*        b_gen_lep2_eta  = TruthTree->Branch("gen_lep2_eta",&gen_lep2_eta);
  TBranch*        b_gen_lep2_phi  = TruthTree->Branch("gen_lep2_phi",&gen_lep2_phi);
  TBranch*        b_gen_lep2_e  = TruthTree->Branch("gen_lep2_e",&gen_lep2_e);

  // RooAbsReal(const char* name, const char* title, Double_t minVal, Double_t maxVal, const char* unit = "")
  //RooAbsReal *test2 = new RooAbsReal("name","title","unit");
  //RooAbsReal* test = new RooAbsReal("mean","mean",1.,1.,"GeV");

  //FuncCB* test = new FuncCB();

  for(UInt_t i = 0 ; i < m_SampleChain->GetEntries() ; ++i)
    {
      m_SampleChain->GetEntry(i);
      // if(EventNumber==46117) cout<<"event #"<<EventNumber<<" is here!"<<endl;
      // else continue;

      //cout<<"Evt = "<<EventNumber<<endl;

      // if(i>1000) break;

      if(i%10000==0) cout<<"Entry #"<<i<<endl;


      //loop on truth particles
      gen_ntau = 0;
      gen_ntauh = 0;
      gen_ntaum = 0;
      gen_ntaue = 0;

      gen_Higgs_PDG = -99;
      gen_truth_Higgs_pt = -99.;
      gen_Higgs_eta = -99.;
      gen_Higgs_phi = -99.;
      gen_Higgs_e = -99.;

      gen_tau1_PDG = -99;
      gen_tau1_pt = -99.;
      gen_tau1_eta = -99.;
      gen_tau1_phi = -99.;
      gen_tau1_e = -99.;

      gen_tau2_PDG = -99;
      gen_tau2_pt = -99.;
      gen_tau2_eta = -99.;
      gen_tau2_phi = -99.;
      gen_tau2_e = -99.;

      Leg1MatchPt = -99.;
      Leg1MatchEta = -99.;
      Leg1MatchPhi = -99.;
      Leg1MatchE = -99.;

      Leg2MatchPt = -99.;
      Leg2MatchEta = -99.;
      Leg2MatchPhi = -99.;
      Leg2MatchE = -99.;

      for(UInt_t iGen = 0 ; iGen < genpart_px->size() ; ++iGen)
	{
	  Bool_t isHiggsSM = abs(genpart_pdg->at(iGen))==25 ? kTRUE : kFALSE ;
	  Bool_t isHiggsMSSM = abs(genpart_pdg->at(iGen))==36 ? kTRUE : kFALSE ;
	  Bool_t isTau = abs(genpart_pdg->at(iGen))==15 ? kTRUE : kFALSE ;
	  Bool_t motherIdx = abs(genpart_TauMothInd->at(iGen)) > -1 ? kTRUE : kFALSE;
	  Bool_t isTauh = motherIdx && abs(genpart_pdg->at(iGen))==66615 ? kTRUE : kFALSE ;
	  Bool_t isTaum = motherIdx && abs(genpart_pdg->at(iGen))==13 ? kTRUE : kFALSE ;
	  Bool_t isTaue = motherIdx && abs(genpart_pdg->at(iGen))==11 ? kTRUE : kFALSE ;

	  if(isTau && (genpart_HMothInd->at(iGen)>-1 || genpart_MSSMHMothInd->at(iGen)>-1)) ++gen_ntau;
	  if(isTauh && (genpart_HMothInd->at(iGen)>-1 || genpart_MSSMHMothInd->at(iGen)>-1)) ++gen_ntauh;
	  if(isTaum && (genpart_HMothInd->at(iGen)>-1 || genpart_MSSMHMothInd->at(iGen)>-1)) ++gen_ntaum;
	  if(isTaue && (genpart_HMothInd->at(iGen)>-1 || genpart_MSSMHMothInd->at(iGen)>-1)) ++gen_ntaue;

	  TLorentzVector gen_part;
	  gen_part.SetPxPyPzE(genpart_px->at(iGen),genpart_py->at(iGen),genpart_pz->at(iGen),genpart_e->at(iGen));

	  if(isHiggsSM || isHiggsMSSM)
	    {
	      gen_Higgs_PDG = genpart_pdg->at(iGen);
	      gen_truth_Higgs_pt = gen_part.Pt();
	      gen_Higgs_eta = gen_part.Eta();
	      gen_Higgs_phi = gen_part.Phi();
	      gen_Higgs_e = gen_part.E();
	    }

	  if(!isTauh && !isTaum && !isTaue) continue ;
	  if(isTauh && fabs(gen_part.Eta())>2.5) continue;
	  if(isTaum && fabs(gen_part.Eta())>2.1) continue;
	  if(isTaue && fabs(gen_part.Eta())>2.1) continue;

	  if(gen_part.Pt()>gen_tau1_pt)
	    {
	      gen_tau2_PDG = gen_tau1_PDG;
	      gen_tau2_pt = gen_tau1_pt;
	      gen_tau2_eta = gen_tau1_eta;
	      gen_tau2_phi = gen_tau1_phi;
	      gen_tau2_e = gen_tau1_e;

	      gen_tau1_PDG = genpart_pdg->at(iGen);
	      gen_tau1_pt = gen_part.Pt();
	      gen_tau1_eta = gen_part.Eta();
	      gen_tau1_phi = gen_part.Phi();
	      gen_tau1_e = gen_part.E();
	    }
	  else if(gen_part.Pt()>gen_tau2_pt)
	    {
	      gen_tau2_PDG = genpart_pdg->at(iGen);	      
	      gen_tau2_pt = gen_part.Pt();   
	      gen_tau2_eta = gen_part.Eta();   
	      gen_tau2_phi = gen_part.Phi();   
	      gen_tau2_e = gen_part.E();   
	    }
	}

      TruthTree->Fill();

      // cout<<"event = "<<i<<endl;

      //Event level selections

      //-> at least 2 leptons
      if(daughters_e->size()<2) continue ;
      
      Int_t lepPairIndex = -99 ;
      Int_t lep1Index = -99 ;
      Int_t lep2Index = -99 ;

      OfflineProducerHelper* HelperTrigger_TauID = new OfflineProducerHelper(hCounter, hTauID);
      
      std::vector<mypair> sortedPairs;
  
      if(RedoOrdering)
	{
	  for(UInt_t p = 0 ; p < isOSCand->size() ; ++p)
	    {
	      std::vector<Double_t> sortvalues_tmp ;
	      mypair temp_pair;

	      lep1Index = indexDau1->at(p);
	      lep2Index = indexDau2->at(p);

	      Float_t CurrentIsoLep1 = -99.;
	      if(Channel=="mt" || Channel=="et" || Channel=="em") CurrentIsoLep1 = combreliso->at(lep1Index);
	      else if(Channel=="tt") CurrentIsoLep1 = daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits->at(lep1Index);

	      Float_t CurrentIsoLep2 = -99.;
	      if(Channel=="mt" || Channel=="et" || Channel=="tt") CurrentIsoLep2 = daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits->at(lep2Index);
	      else if(Channel=="em") CurrentIsoLep2 = combreliso->at(lep2Index);

	      TLorentzVector lep1 ;
	      lep1.SetPxPyPzE(daughters_px->at(indexDau1->at(p)),daughters_py->at(indexDau1->at(p)),daughters_pz->at(indexDau1->at(p)),daughters_e->at(indexDau1->at(p)));
	      TLorentzVector lep2 ;
	      lep2.SetPxPyPzE(daughters_px->at(indexDau2->at(p)),daughters_py->at(indexDau2->at(p)),daughters_pz->at(indexDau2->at(p)),daughters_e->at(indexDau2->at(p)));	  
	  
	      Float_t CurrentPtLep1 = lep1.Pt();
	      Float_t CurrentPtLep2 = lep2.Pt();
	  
	      sortvalues_tmp.push_back(CurrentIsoLep1);
	      sortvalues_tmp.push_back(CurrentPtLep1);
	      sortvalues_tmp.push_back(CurrentIsoLep2);
	      sortvalues_tmp.push_back(CurrentPtLep2);
	      temp_pair = std::make_pair(p,sortvalues_tmp);
	      sortvalues_tmp.clear();
	  
	      sortedPairs.push_back(temp_pair);

	      sortvalues_tmp.clear();
	    }

	  std::sort(sortedPairs.begin(), sortedPairs.end(), comparator);
	}

      // for(UInt_t d = 0 ; d < sortedPairs.size() ; ++d)
      // 	{
      // 	  cout<<"sortedPairs = "<<d<<": "<<endl;
      // 	  cout<<"    index = "<<sortedPairs.at(d).first<<endl;
      // 	  cout<<"      iso_1 = "<<sortedPairs.at(d).second.at(0)<<endl;
      // 	  cout<<"      pt_1  = "<<sortedPairs.at(d).second.at(1)<<endl;
      // 	  cout<<"      iso_2 = "<<sortedPairs.at(d).second.at(2)<<endl;
      // 	  cout<<"      pt_2  = "<<sortedPairs.at(d).second.at(3)<<endl;	  
      // 	}    

      //loop on ordered lepton pairs
      for(UInt_t q = 0 ; q < isOSCand->size() ; ++q)
	// for(UInt_t p = 0 ; p < isOSCand->size() ; ++p)
	{
	  UInt_t p = 0;
	  if(RedoOrdering) p = sortedPairs.at(q).first;
	  else p = q;

	  // if(p!=5) continue;

	  //cout<<"pair is: "<<p+1<<"/"<<isOSCand->size()<<endl;

	  //check lepton pT --> build LorentzVector
	  TLorentzVector lep1 ;
	  lep1.SetPxPyPzE(daughters_px->at(indexDau1->at(p)),daughters_py->at(indexDau1->at(p)),daughters_pz->at(indexDau1->at(p)),daughters_e->at(indexDau1->at(p)));
	  TLorentzVector lep2 ;
	  lep2.SetPxPyPzE(daughters_px->at(indexDau2->at(p)),daughters_py->at(indexDau2->at(p)),daughters_pz->at(indexDau2->at(p)),daughters_e->at(indexDau2->at(p)));

	  // cout<<"lep1.Pt(), lep1.Eta(), lep1.Phi(), lep1.Eta(), iso, pdg = "<<lep1.Pt()<<", "<<lep1.Eta()<<", "<<lep1.Phi()<<", "<<combreliso->at(indexDau1->at(p))<<", "<<PDGIdDaughters->at(indexDau1->at(p))<<endl;
	  // cout<<"lep2.Pt(), lep2.Eta(), lep2.Phi(), lep2.Eta(), iso, pdg = "<<lep2.Pt()<<", "<<lep2.Eta()<<", "<<lep2.Phi()<<", "<<daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits->at(indexDau2->at(p))<<", "<<PDGIdDaughters->at(indexDau2->at(p))<<endl;


	  //check PDGIDs...
	  if(Channel=="mt")
	    {
	      if(fabs(PDGIdDaughters->at(indexDau1->at(p)))!=13) continue;
	      if(fabs(PDGIdDaughters->at(indexDau2->at(p)))!=15) continue;
	    }
	  else if(Channel=="et")
	    {
	      if(fabs(PDGIdDaughters->at(indexDau1->at(p)))!=11) continue;
	      if(fabs(PDGIdDaughters->at(indexDau2->at(p)))!=15) continue;
	    }
	  else if(Channel=="tt")
	    {
	      if(fabs(PDGIdDaughters->at(indexDau1->at(p)))!=15) continue;
	      if(fabs(PDGIdDaughters->at(indexDau2->at(p)))!=15) continue;
	    }
	  else if(Channel=="em")
	    {
	      if(fabs(PDGIdDaughters->at(indexDau1->at(p)))!=11) continue;
	      if(fabs(PDGIdDaughters->at(indexDau2->at(p)))!=13) continue;
	    }

	  //-> opposite sign candidate
	  // if(!isOSCand->at(p)) continue ;//removed

	  //initialize trigger menus
	  std::vector<TString> TriggerNames ;
	  std::vector<Int_t> TriggerBits ;

	  std::vector<TString> TriggerFiredNames ;
	  std::vector<Bool_t> TriggerFiredIsDoubleObject ;
	  std::vector<Int_t> TriggerFiredBits;

	  std::vector<TString> TriggerFiredAndGoodTriggerTypeNames ;
	  std::vector<Bool_t> TriggerFiredAndGoodTriggerTypeIsDoubleObject ;
	  std::vector<Int_t> TriggerFiredAndGoodTriggerTypeBits ;

	  std::vector<TString> TriggerNamesFinal ;
	  std::vector<Int_t> TriggerBitsFinal ;

	  if(DataTrue_MCFalse)
	    {
	      for(UInt_t iTrig = 0 ; iTrig < triggerPathsData.size() ; ++iTrig) TriggerNames.push_back(TString(triggerPathsData.at(iTrig)));
	    }
	  else
	    {
	      for(UInt_t iTrig = 0 ; iTrig < triggerPathsMC.size() ; ++iTrig) TriggerNames.push_back(TString(triggerPathsMC.at(iTrig)));
	    }

	  //trigger checks
	  if(Channel=="mt" || Channel=="et" || Channel=="tt" || Channel=="em")
	    {
	      // if(Channel=="mt") cout<<"mt!"<<endl;

	      //loop on triggers
	      for(UInt_t iTrig = 0 ; iTrig < TriggerNames.size() ; ++iTrig)
		{
		  // if(Channel=="et") cout<<"Trigger name = "<<TriggerNames.at(iTrig)<<endl;
		  // if(Channel=="et") cout<<"HelperTrigger_TauID->FindTriggerNumber(TriggerNames.at(iTrig)) = "<<HelperTrigger_TauID->FindTriggerNumber(TriggerNames.at(iTrig))<<endl;
		  // if(Channel=="mt") cout<<HelperTrigger_TauID->FindTriggerNumber(TriggerNames.at(iTrig))<<endl;
		  TriggerBits.push_back(HelperTrigger_TauID->FindTriggerNumber(TriggerNames.at(iTrig)));
		  if(HelperTrigger_TauID->IsTriggerFired(triggerbit,TriggerNames.at(iTrig)))
		    {
		      TriggerFiredNames.push_back(TriggerNames.at(iTrig));
		      TString tmp_name = TriggerNames.at(iTrig);
		      //cout<<"Trigger name / bit = "<<TriggerNames.at(iTrig)<<"/"<<HelperTrigger_TauID->FindTriggerNumber(TriggerNames.at(iTrig))<<endl;
		      TriggerFiredBits.push_back(HelperTrigger_TauID->FindTriggerNumber(TriggerNames.at(iTrig)));
		      if(Channel=="mt")
			{
			  if(tmp_name.Contains("Mu") && tmp_name.Contains("Tau")) TriggerFiredIsDoubleObject.push_back(kTRUE);
			  else TriggerFiredIsDoubleObject.push_back(kFALSE);
			}
		      else if(Channel=="et")
			{
			  if(tmp_name.Contains("Ele") && tmp_name.Contains("Tau")) TriggerFiredIsDoubleObject.push_back(kTRUE);
			  else TriggerFiredIsDoubleObject.push_back(kFALSE);
			}
		      else if(Channel=="tt")
			{
			  if(tmp_name.Contains("Double")) TriggerFiredIsDoubleObject.push_back(kTRUE);
			  else TriggerFiredIsDoubleObject.push_back(kFALSE);
			}
		      else if(Channel=="em")
			{
			  if(tmp_name.Contains("Ele") && tmp_name.Contains("Mu")) TriggerFiredIsDoubleObject.push_back(kTRUE);
			  else TriggerFiredIsDoubleObject.push_back(kFALSE);
			}
		      //cout<<"TriggerFiredBits.at(size-1) = "<<TriggerFiredBits.at(TriggerFiredBits.size()-1)<<endl;

		    }
		  
		}

	      //at least one trigger fired
	      if(TriggerFiredNames.size()<1) continue ;

	      //cout<<"passes trigger event level"<<endl;

	      
	      //check isGoodTriggerType
	      Bool_t BothDaughtersAreGoodTriggerTypes = kFALSE ;
	      for(UInt_t iTrigFired = 0 ; iTrigFired < TriggerFiredNames.size() ; ++iTrigFired)
		{
		  //cout<<"pair is: "<<p<<endl;
		  //cout<<"daughters_isGoodTriggerType->at(indexDau1->at(p)) = "<<daughters_isGoodTriggerType->at(indexDau1->at(p))<<endl;
		  //cout<<"daughters_isGoodTriggerType->at(indexDau2->at(p)) = "<<daughters_isGoodTriggerType->at(indexDau2->at(p))<<endl;
		  //cout<<"TriggerFiredBits.at(iTrigFired) = "<<TriggerFiredBits.at(iTrigFired)<<endl;
		  
		  if(TriggerFiredIsDoubleObject.at(iTrigFired)==kTRUE)
		    {
		      //cout<<"double trigger, testing GoodTriggerType: "<<endl;
		      //cout<<"daughter1 = "<<((daughters_isGoodTriggerType->at(indexDau1->at(p)) >> TriggerFiredBits.at(iTrigFired)) & 1)<<endl;
		      //cout<<"daughter2 = "<<((daughters_isGoodTriggerType->at(indexDau2->at(p)) >> TriggerFiredBits.at(iTrigFired)) & 1)<<endl;
		      if(!((daughters_isGoodTriggerType->at(indexDau1->at(p)) >> TriggerFiredBits.at(iTrigFired)) & 1)){continue;}
		      if(!((daughters_isGoodTriggerType->at(indexDau2->at(p)) >> TriggerFiredBits.at(iTrigFired)) & 1)){continue;}

		      if(Channel=="em")//special case of emu channel... to be corrected for swapping / improved
			{
			  if(TriggerFiredNames.at(iTrigFired)=="HLT_Mu23_TrkIsoVVL_Ele12_Gsf_CaloId_TrackId_Iso_MediumWP_v1" && ((daughters_isGoodTriggerType->at(indexDau2->at(p)) >> TriggerFiredBits.at(iTrigFired)) & 1) && lep2.Pt()<25.) continue;
			  if(TriggerFiredNames.at(iTrigFired)=="HLT_Mu8_TrkIsoVVL_Ele23_Gsf_CaloId_TrackId_Iso_MediumWP_v1" && ((daughters_isGoodTriggerType->at(indexDau1->at(p)) >> TriggerFiredBits.at(iTrigFired)) & 1) && lep1.Pt()<25.) continue;
			}
		    }
		  else
		    {
		      //cout<<"single trigger, testing GoodTriggerType: "<<endl;
		      //cout<<"daughter1 = "<<((daughters_isGoodTriggerType->at(indexDau1->at(p)) >> TriggerFiredBits.at(iTrigFired)) & 1)<<endl;
		      //cout<<"daughter2 = "<<((daughters_isGoodTriggerType->at(indexDau2->at(p)) >> TriggerFiredBits.at(iTrigFired)) & 1)<<endl;
		      if(!((daughters_isGoodTriggerType->at(indexDau1->at(p)) >> TriggerFiredBits.at(iTrigFired)) & 1) && !((daughters_isGoodTriggerType->at(indexDau2->at(p)) >> TriggerFiredBits.at(iTrigFired)) & 1)){continue;}
		      if(Channel=="mt" && ((daughters_isGoodTriggerType->at(indexDau1->at(p)) >> TriggerFiredBits.at(iTrigFired)) & 1) && lep1.Pt()<25.) continue ;
		      if(Channel=="et" && ((daughters_isGoodTriggerType->at(indexDau1->at(p)) >> TriggerFiredBits.at(iTrigFired)) & 1) && lep1.Pt()<33.) continue ;
		    }

		  TString tmp_name = TriggerFiredNames.at(iTrigFired);

		  if(Channel=="mt")
		    {
		      if(tmp_name.Contains("Mu") && tmp_name.Contains("Tau")) TriggerFiredAndGoodTriggerTypeIsDoubleObject.push_back(kTRUE);
		      else TriggerFiredAndGoodTriggerTypeIsDoubleObject.push_back(kFALSE);
		    }
		  else if(Channel=="et")
		    {
		      if(tmp_name.Contains("Ele") && tmp_name.Contains("Tau")) TriggerFiredAndGoodTriggerTypeIsDoubleObject.push_back(kTRUE);
		      else TriggerFiredAndGoodTriggerTypeIsDoubleObject.push_back(kFALSE);
		    }
		  else if(Channel=="tt")
		    {
		      if(tmp_name.Contains("Double")) TriggerFiredAndGoodTriggerTypeIsDoubleObject.push_back(kTRUE);
		      else TriggerFiredAndGoodTriggerTypeIsDoubleObject.push_back(kFALSE);
		    }
		  else if(Channel=="em")
		    {
		      if(tmp_name.Contains("Ele") && tmp_name.Contains("Mu")) TriggerFiredAndGoodTriggerTypeIsDoubleObject.push_back(kTRUE);
		      else TriggerFiredAndGoodTriggerTypeIsDoubleObject.push_back(kFALSE);
		    }

		  TriggerFiredAndGoodTriggerTypeNames.push_back(TriggerFiredNames.at(iTrigFired));
		  TriggerFiredAndGoodTriggerTypeBits.push_back(HelperTrigger_TauID->FindTriggerNumber(TriggerFiredNames.at(iTrigFired)));
		  BothDaughtersAreGoodTriggerTypes = kTRUE;
		}
	      //cout<<"-----"<<endl;
	      if(!BothDaughtersAreGoodTriggerTypes) continue;
	      //cout<<"passes trigger types"<<endl;

	      //check trigger filters
	      Bool_t BothDaughtersAreGoodTriggerTypesAndPassFilter = kFALSE ;
	      for(UInt_t iTrigFiredAndGoodTriggerType = 0 ; iTrigFiredAndGoodTriggerType < TriggerFiredAndGoodTriggerTypeBits.size() ; ++iTrigFiredAndGoodTriggerType)
		{
		  //cout<<"testing the filters from this trigger = "<<TriggerFiredAndGoodTriggerTypeNames.at(iTrigFiredAndGoodTriggerType)<<endl;

		  if(TriggerFiredAndGoodTriggerTypeIsDoubleObject.at(iTrigFiredAndGoodTriggerType)==kTRUE)
		    {
		      if(!((daughters_FilterFired->at(indexDau1->at(p)) >> TriggerFiredAndGoodTriggerTypeBits.at(iTrigFiredAndGoodTriggerType)) & 1)) continue;
		      if(!((daughters_FilterFired->at(indexDau2->at(p)) >> TriggerFiredAndGoodTriggerTypeBits.at(iTrigFiredAndGoodTriggerType)) & 1)) continue;
		      if(!((daughters_L3FilterFired->at(indexDau1->at(p)) >> TriggerFiredAndGoodTriggerTypeBits.at(iTrigFiredAndGoodTriggerType)) & 1)) continue;
		      if(!((daughters_L3FilterFired->at(indexDau2->at(p)) >> TriggerFiredAndGoodTriggerTypeBits.at(iTrigFiredAndGoodTriggerType)) & 1)) continue;
		      if(!((daughters_L3FilterFiredLast->at(indexDau1->at(p)) >> TriggerFiredAndGoodTriggerTypeBits.at(iTrigFiredAndGoodTriggerType)) & 1)) continue;
		      if(!((daughters_L3FilterFiredLast->at(indexDau2->at(p)) >> TriggerFiredAndGoodTriggerTypeBits.at(iTrigFiredAndGoodTriggerType)) & 1)) continue;
		      //cout<<"passing double trigger filters"<<endl;
		    }
		  else
		    {
		      if(!((daughters_FilterFired->at(indexDau1->at(p)) >> TriggerFiredAndGoodTriggerTypeBits.at(iTrigFiredAndGoodTriggerType)) & 1)) continue;
		      if(!((daughters_L3FilterFired->at(indexDau1->at(p)) >> TriggerFiredAndGoodTriggerTypeBits.at(iTrigFiredAndGoodTriggerType)) & 1)) continue;
		      if(!((daughters_L3FilterFiredLast->at(indexDau1->at(p)) >> TriggerFiredAndGoodTriggerTypeBits.at(iTrigFiredAndGoodTriggerType)) & 1)) continue;
		      //cout<<"passing single trigger filters"<<endl;
		      // if(!((daughters_FilterFired->at(indexDau1->at(p)) >> TriggerFiredAndGoodTriggerTypeBits.at(iTrigFiredAndGoodTriggerType)) & 1) && !((daughters_L3FilterFired->at(indexDau2->at(p)) >> TriggerFiredAndGoodTriggerTypeBits.at(iTrigFiredAndGoodTriggerType)) & 1)) continue;
		      // if(!((daughters_L3FilterFired->at(indexDau1->at(p)) >> TriggerFiredAndGoodTriggerTypeBits.at(iTrigFiredAndGoodTriggerType)) & 1) && !((daughters_FilterFired->at(indexDau2->at(p)) >> TriggerFiredAndGoodTriggerTypeBits.at(iTrigFiredAndGoodTriggerType)) & 1)) continue;
		      // if(!((daughters_L3FilterFiredLast->at(indexDau1->at(p)) >> TriggerFiredAndGoodTriggerTypeBits.at(iTrigFiredAndGoodTriggerType)) & 1) && !((daughters_L3FilterFiredLast->at(indexDau2->at(p)) >> TriggerFiredAndGoodTriggerTypeBits.at(iTrigFiredAndGoodTriggerType)) & 1)) continue;
		      // cout<<"passing single trigger filters"<<endl;
		    }


		  TriggerNamesFinal.push_back(TriggerFiredAndGoodTriggerTypeNames.at(iTrigFiredAndGoodTriggerType));
		  TriggerBitsFinal.push_back(HelperTrigger_TauID->FindTriggerNumber(TriggerFiredAndGoodTriggerTypeNames.at(iTrigFiredAndGoodTriggerType)));
		  BothDaughtersAreGoodTriggerTypesAndPassFilter = kTRUE;
		}
	      
	      if(!BothDaughtersAreGoodTriggerTypesAndPassFilter) continue;
	    }

	  //cout<<"passes trigger"<<endl;

	  // cout<<"types"<<endl;
	  // cout<<" particleType->at(indexDau1->at(p)) = "<< particleType->at(indexDau1->at(p))<<endl;
	  // cout<<" particleType->at(indexDau2->at(p)) = "<< particleType->at(indexDau2->at(p))<<endl;

	  //check if candidate is mu+tau or something else
	  if(Channel=="mt" && !(particleType->at(indexDau1->at(p))==0 && particleType->at(indexDau2->at(p))==2)) continue ;
	  if(Channel=="et" && !(particleType->at(indexDau1->at(p))==1 && particleType->at(indexDau2->at(p))==2)) continue ;
	  if(Channel=="tt" && !(particleType->at(indexDau1->at(p))==2 && particleType->at(indexDau2->at(p))==2)) continue ;
	  if(Channel=="em" && !(particleType->at(indexDau1->at(p))==1 && particleType->at(indexDau2->at(p))==0)) continue ;
	  // cout<<"passes mt pair selection"<<endl;

	  // //check lepton pT --> build LorentzVector
	  // TLorentzVector lep1 ;
	  // lep1.SetPxPyPzE(daughters_px->at(indexDau1->at(p)),daughters_py->at(indexDau1->at(p)),daughters_pz->at(indexDau1->at(p)),daughters_e->at(indexDau1->at(p)));
	  // TLorentzVector lep2 ;
	  // lep2.SetPxPyPzE(daughters_px->at(indexDau2->at(p)),daughters_py->at(indexDau2->at(p)),daughters_pz->at(indexDau2->at(p)),daughters_e->at(indexDau2->at(p)));

	  // cout<<"lep1.Pt(), lep1.Eta(), iso = "<<lep1.Pt()<<", "<<lep1.Eta()<<", "<<combreliso->at(indexDau1->at(p))<<endl;
	  // cout<<"lep2.Pt(), lep2.Eta(), iso = "<<lep2.Pt()<<", "<<lep2.Eta()<<", "<<daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits->at(indexDau2->at(p))<<endl;

	  // cout<<"checking leptons pT"<<endl;
	  if(lep1.Pt()<cutLepton1_Pt || lep2.Pt()<cutLepton2_Pt) continue ;//changed here
	  // cout<<"checking leptons eta"<<endl;
	  if(fabs(lep1.Eta())>cutLepton1_Eta) continue ;
	  if(fabs(lep2.Eta())>cutLepton2_Eta) continue ;
	  // cout<<"passes eta cuts"<<endl;

	  // cout<<"checking leptons dxy and dz"<<endl;
	  if(Channel=="mt" || Channel=="et" || Channel=="em")
	    {
	      //lepton 1 = muon or electron
	      if(fabs(dxy->at(indexDau1->at(p)))>cutLepton1_dxy) continue;
	      if(fabs(dz->at(indexDau1->at(p)))>cutLepton1_dz) continue;
	      // cout<<"lep1 dz cut is passed?"<<endl;
	    }
	  if(Channel=="em")
	    {
	      //lepton 2 = muon
	      if(fabs(dxy->at(indexDau2->at(p)))>cutLepton2_dxy) continue;
	      if(fabs(dz->at(indexDau2->at(p)))>cutLepton2_dz) continue;
	    }

	  if(Channel=="tt")
	    {
	      //lepton 1 = tau
	      if(fabs(dz->at(indexDau1->at(p)))>cutLepton1_dz) continue;	
	    }

	  if(Channel=="mt" || Channel=="et" || Channel=="tt")
	    {
	      //lepton 2 = tau
	      //cout<<"dz = "<<dz->at(indexDau2->at(p))<<endl;
	      if(fabs(dz->at(indexDau2->at(p)))>cutLepton2_dz) continue;
	      //cout<<"abs(daughters_charge->at(indexDau2->at(p))) = "<<abs(daughters_charge->at(indexDau2->at(p)))<<endl;
	      if(abs(daughters_charge->at(indexDau2->at(p)))!=1) continue;
	      // cout<<"lep2 dz cut is passed?"<<endl;
	    }

	  //check lepton1 ID
	  // cout<<"checking electron / muon ID"<<endl;
	  if(Channel=="mt")
	    {
	      //muon
	      int discriminator_muon = int(discriminator->at(indexDau1->at(p)));
	      if(cutLepton1_IDV=="medium")
		{
		  if(!((discriminator_muon >> 2) & 1)) continue ;//medium//changed here
		}
	    }
	  else if(Channel=="et")
	    {
	      //electron
	      bool pass = false ;
	      float discriminatorValue = discriminator->at(indexDau1->at(p));
	      if(cutLepton1_IDV=="80") pass = CheckElectronMVAIDWP80(discriminatorValue, lep1);
	      else if(cutLepton1_IDV=="90") pass = CheckElectronMVAIDWP90(discriminatorValue, lep1);

	      if(!pass) continue;
	    }
	  else if(Channel=="em")
	    {
	      bool pass = false ;

	      //electron
	      float discriminatorValue = discriminator->at(indexDau1->at(p));
	      if(cutLepton1_IDV=="80") pass = CheckElectronMVAIDWP80(discriminatorValue, lep1);	      
	      else if(cutLepton1_IDV=="90") pass = CheckElectronMVAIDWP90(discriminatorValue, lep1);	      

	      if(!pass) continue;

	      //muon
	      int discriminator_muon = int(discriminator->at(indexDau2->at(p)));
	      if(cutLepton1_IDV=="medium")
		{
		  if(!((discriminator_muon >> 2) & 1)) continue ;//medium//changed here
		}
	    }


	  //check muon iso
	  // cout<<"checking muon iso"<<endl;
	  // cout<<"lep1.Pt(), combreliso->at(indexDau1->at(p)) = "<<lep1.Pt()<<", "<<combreliso->at(indexDau1->at(p))<<endl;
	  // cout<<"lep1.Pt(), lep1.Eta() = "<<lep1.Pt()<<", "<<lep1.Eta()<<endl;
	  // cout<<"lep2.Pt(), lep2.Eta() = "<<lep2.Pt()<<", "<<lep2.Eta()<<endl;
	  // if(combreliso->at(indexDau1->at(p))>cutLepton1_Iso) continue ;

	  //check tau DM finding
	  // cout<<"checking DM finding"<<endl;
	  if(Channel=="mt" || Channel=="et" || Channel == "tt")
	    {
	      if(cutLepton2_DMV=="new" && daughters_decayModeFindingNewDMs->at(indexDau2->at(p))!=1) continue ;
	      else if(cutLepton2_DMV=="old" && daughters_decayModeFindingOldDMs->at(indexDau2->at(p))!=1) continue ;
	      // if(daughters_decayModeFindingOldDMs->at(indexDau2->at(p))!=1 && daughters_decayModeFindingNewDMs->at(indexDau2->at(p))!=1) continue ;
	    }
	  if(Channel=="tt")
	    {
	      if(cutLepton1_DMV=="new" && daughters_decayModeFindingNewDMs->at(indexDau1->at(p))!=1) continue ;
	      else if(cutLepton1_DMV=="old" && daughters_decayModeFindingOldDMs->at(indexDau1->at(p))!=1) continue ;
	    }

	  //check tau ID
	  // int discriminator_tau = int(discriminator->at(indexDau2->at(p)));
	  // if(!((discriminator_tau >> 3) & 1)) continue ;	
	  // cout<<"checking tau iso"<<endl;
	  if(Channel=="tt")
	    {
	      if((tauID->at(indexDau1->at(p)) >>  HelperTrigger_TauID->FindTriggerNumber("againstMuonTight3")) & 1) continue;;
	      if((tauID->at(indexDau2->at(p)) >>  HelperTrigger_TauID->FindTriggerNumber("againstMuonTight3")) & 1) continue;;
	      if((tauID->at(indexDau1->at(p)) >>  HelperTrigger_TauID->FindTriggerNumber("againstElectronVLooseMVA5")) & 1) continue;;
	      if((tauID->at(indexDau2->at(p)) >>  HelperTrigger_TauID->FindTriggerNumber("againstElectronVLooseMVA5")) & 1) continue;;
	      // if(!((daughters_isGoodTriggerType->at(indexDau1->at(p)) >> TriggerFiredBits.at(iTrigFired)) & 1)){continue;}

	      if(daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits->at(indexDau1->at(p))>1.) continue ;
	      if(daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits->at(indexDau2->at(p))>1.) continue ;
	      // if(daughters_againstMuonTight3->at(indexDau1->at(p))!=1) continue ;
	      // if(daughters_againstMuonTight3->at(indexDau2->at(p))!=1) continue ;
	      // if(daughters_againstElectronVLooseMVA5->at(indexDau1->at(p))!=1) continue ;
	      // if(daughters_againstElectronVLooseMVA5->at(indexDau2->at(p))!=1) continue ;
	    }

	  //check antiMu and antiE
	  // cout<<"checking anti muon and electron"<<endl;
	  // if(daughters_againstMuonTight3->at(indexDau2->at(p))!=1) continue ;
	  // if(daughters_againstElectronVLooseMVA5->at(indexDau2->at(p))!=1) continue ;

	  //check mT
	  // cout<<"checking mT"<<endl;
	  //if(mT_Dau1->at(p)>30.) continue ;//changed here

	  //check overlap between muon and tau
	  if(lep1.DeltaR(lep2)<DR) continue ;

	  lepPairIndex = p ;

	  //good pair
	  lep1Index = indexDau1->at(p);
	  lep2Index = indexDau2->at(p);
	  // cout<<"good pair!!!"<<endl;

	  //break;

	  if(lep1Index < 0 || lep2Index < 0) continue ;

	  //trigger weights stage 2	    
	  Double_t m0_Stage2_30GeV = 28.9025;
	  Double_t sigma_Stage2_30GeV = 6.03092;
	  Double_t alpha_Stage2_30GeV = 14.605;
	  Double_t n_Stage2_30GeV = 1.02663;
	  Double_t norm_Stage2_30GeV = 0.94351;

	  Double_t m0_Stage2_35GeV = 32.8406;
	  Double_t sigma_Stage2_35GeV = 6.71412;
	  Double_t alpha_Stage2_35GeV = 14.5035;
	  Double_t n_Stage2_35GeV = 1.14221;
	  Double_t norm_Stage2_35GeV = 0.901589;

	  Double_t m0_Stage2_42GeV = 39.2981;
	  Double_t sigma_Stage2_42GeV = 7.73986;
	  Double_t alpha_Stage2_42GeV = 9.80126;
	  Double_t n_Stage2_42GeV = 99.7786;
	  Double_t norm_Stage2_42GeV = 0.907926;

	  Double_t m0_Stage2_70GeV = 71.9576;
	  Double_t sigma_Stage2_70GeV = 9.99992;
	  Double_t alpha_Stage2_70GeV = 19.2197;
	  Double_t n_Stage2_70GeV = 7.44799;
	  Double_t norm_Stage2_70GeV = 1.;

	  //trigger weights stage 2	    
	  Double_t m0_Stage1_45GeV = 50.;
	  Double_t sigma_Stage1_45GeV = 8.43056;
	  Double_t alpha_Stage1_45GeV = 4.28079;
	  Double_t n_Stage1_45GeV = 99.9996;
	  Double_t norm_Stage1_45GeV = 0.988516;

	  //filling pT TurnOn Stage2 turn-on 1
	  Double_t m0_Stage2_36GeV = 32.8632;
	  Double_t sigma_Stage2_36GeV = 8.24761;
	  Double_t alpha_Stage2_36GeV = 28.5536;
	  Double_t n_Stage2_36GeV = 1.00109;
	  Double_t norm_Stage2_36GeV = 0.900089;
	  
	  //filling pT TurnOn Stage2 turn-on 1
	  Double_t m0_Stage2_80GeV = 79.8039;
	  Double_t sigma_Stage2_80GeV = 14.8249;
	  Double_t alpha_Stage2_80GeV = 50.;
	  Double_t n_Stage2_80GeV = 92.8865;
	  Double_t norm_Stage2_80GeV = 0.99962;
	  
	  //stage 2
	  TriggerWeightLeg1Stage2_30GeV = evaluateCB(lep1.Pt(),m0_Stage2_30GeV,sigma_Stage2_30GeV,alpha_Stage2_30GeV,n_Stage2_30GeV,norm_Stage2_30GeV);
	  TriggerWeightLeg1Stage2_35GeV = evaluateCB(lep1.Pt(),m0_Stage2_35GeV,sigma_Stage2_35GeV,alpha_Stage2_35GeV,n_Stage2_35GeV,norm_Stage2_35GeV);
	  TriggerWeightLeg1Stage2_42GeV = evaluateCB(lep1.Pt(),m0_Stage2_42GeV,sigma_Stage2_42GeV,alpha_Stage2_42GeV,n_Stage2_42GeV,norm_Stage2_42GeV);
	  TriggerWeightLeg1Stage2_70GeV = evaluateCB(lep1.Pt(),m0_Stage2_70GeV,sigma_Stage2_70GeV,alpha_Stage2_70GeV,n_Stage2_70GeV,norm_Stage2_70GeV);
	  TriggerWeightLeg2Stage2_30GeV = evaluateCB(lep2.Pt(),m0_Stage2_30GeV,sigma_Stage2_30GeV,alpha_Stage2_30GeV,n_Stage2_30GeV,norm_Stage2_30GeV);
	  TriggerWeightLeg2Stage2_35GeV = evaluateCB(lep2.Pt(),m0_Stage2_35GeV,sigma_Stage2_35GeV,alpha_Stage2_35GeV,n_Stage2_35GeV,norm_Stage2_35GeV);
	  TriggerWeightLeg2Stage2_42GeV = evaluateCB(lep2.Pt(),m0_Stage2_42GeV,sigma_Stage2_42GeV,alpha_Stage2_42GeV,n_Stage2_42GeV,norm_Stage2_42GeV);
	  TriggerWeightLeg2Stage2_70GeV = evaluateCB(lep2.Pt(),m0_Stage2_70GeV,sigma_Stage2_70GeV,alpha_Stage2_70GeV,n_Stage2_70GeV,norm_Stage2_70GeV);

	  TriggerWeightLeg1Stage2 = TMath::Max(evaluateCB(lep1.Pt(),m0_Stage2_36GeV,sigma_Stage2_36GeV,alpha_Stage2_36GeV,n_Stage2_36GeV,norm_Stage2_36GeV),evaluateCB(lep1.Pt(),m0_Stage2_80GeV,sigma_Stage2_80GeV,alpha_Stage2_80GeV,n_Stage2_80GeV,norm_Stage2_80GeV));
	  TriggerWeightLeg2Stage2 = TMath::Max(evaluateCB(lep2.Pt(),m0_Stage2_36GeV,sigma_Stage2_36GeV,alpha_Stage2_36GeV,n_Stage2_36GeV,norm_Stage2_36GeV),evaluateCB(lep2.Pt(),m0_Stage2_80GeV,sigma_Stage2_80GeV,alpha_Stage2_80GeV,n_Stage2_80GeV,norm_Stage2_80GeV));

	  //stage 1
	  TriggerWeightLeg1Stage1_45GeV = evaluateCB(lep1.Pt(),m0_Stage1_45GeV,sigma_Stage1_45GeV,alpha_Stage1_45GeV,n_Stage1_45GeV,norm_Stage1_45GeV);
	  TriggerWeightLeg2Stage1_45GeV = evaluateCB(lep2.Pt(),m0_Stage1_45GeV,sigma_Stage1_45GeV,alpha_Stage1_45GeV,n_Stage1_45GeV,norm_Stage1_45GeV);
	  
	  //check truth matching
	  isLeg1MatchedToTauh = kFALSE;
	  isLeg1MatchedToTaum = kFALSE;
	  isLeg1MatchedToTaue = kFALSE;
	  isLeg2MatchedToTauh = kFALSE;
	  isLeg2MatchedToTaum = kFALSE;
	  isLeg2MatchedToTaue = kFALSE;
	  isLeg1FromSMHiggs = kFALSE;
	  isLeg2FromSMHiggs = kFALSE;
	  isLeg1FromMSSMHiggs = kFALSE;
	  isLeg2FromMSSMHiggs = kFALSE;

	  // cout<<"EventNumber =  "<<EventNumber<<endl;
	  // cout<<"lep1.Pt(), lep1.Eta(), lep1.Phi(), lep1.Eta(), iso, pdg = "<<lep1.Pt()<<", "<<lep1.Eta()<<", "<<lep1.Phi()<<", "<<combreliso->at(indexDau1->at(p))<<", "<<PDGIdDaughters->at(indexDau1->at(p))<<endl;
	  // cout<<"lep2.Pt(), lep2.Eta(), lep2.Phi(), lep2.Eta(), iso, pdg = "<<lep2.Pt()<<", "<<lep2.Eta()<<", "<<lep2.Phi()<<", "<<daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits->at(indexDau2->at(p))<<", "<<PDGIdDaughters->at(indexDau2->at(p))<<endl;

	  for(UInt_t iGen = 0 ; iGen < genpart_px->size() ; ++iGen)
	    {
	      Bool_t isHiggsSM = abs(genpart_pdg->at(iGen))==25 ? kTRUE : kFALSE ;
	      Bool_t isHiggsMSSM = abs(genpart_pdg->at(iGen))==36 ? kTRUE : kFALSE ;
	      Bool_t isTau = abs(genpart_pdg->at(iGen))==15 ? kTRUE : kFALSE ;
	      Bool_t motherIdx = abs(genpart_TauMothInd->at(iGen)) > -1 ? kTRUE : kFALSE;
	      Bool_t isTauh = motherIdx && abs(genpart_pdg->at(iGen))==66615 ? kTRUE : kFALSE ;
	      Bool_t isTaum = motherIdx && abs(genpart_pdg->at(iGen))==13 ? kTRUE : kFALSE ;
	      Bool_t isTaue = motherIdx && abs(genpart_pdg->at(iGen))==11 ? kTRUE : kFALSE ;

	      if(!isHiggsSM && !isHiggsMSSM && !isTau && !isTauh && !isTaum && !isTaue) continue;
	      
	      TLorentzVector gen_part;
	      gen_part.SetPxPyPzE(genpart_px->at(iGen),genpart_py->at(iGen),genpart_pz->at(iGen),genpart_e->at(iGen));

	      // cout<<"checking gen pT, eta, phi, E, PDG = "<<gen_part.Pt()<<", "<<gen_part.Eta()<<", "<<gen_part.Phi()<<", "<<gen_part.E()<<", "<<genpart_pdg->at(iGen)<<endl;

	      // if(lep1.DeltaR(gen_part)<0.5) cout<<"lep1: checking gen pT, eta, phi, E = "<<gen_part.Pt()<<", "<<gen_part.Eta()<<", "<<gen_part.Phi()<<", "<<gen_part.E()<<endl;
	      // if(lep2.DeltaR(gen_part)<0.5) cout<<"lep2: checking gen pT, eta, phi, E = "<<gen_part.Pt()<<", "<<gen_part.Eta()<<", "<<gen_part.Phi()<<", "<<gen_part.E()<<endl;

	      Bool_t isTauFromSMHiggs = kFALSE ;
	      Bool_t isTauFromMSSMHiggs = kFALSE ;
	      if(isTauh || isTaum || isTaue)
		{
		  if(genpart_TauMothInd->at(iGen)<0) continue;
		  //cout<<"genpart_TauMothInd->at(iGen)= "<<genpart_TauMothInd->at(iGen)<<endl;
		  if(genpart_HMothInd->at(genpart_TauMothInd->at(iGen))) isTauFromSMHiggs = kTRUE;
		  if(genpart_MSSMHMothInd->at(genpart_TauMothInd->at(iGen))) isTauFromMSSMHiggs = kTRUE;
		}
	      
	      // isLeg1MatchedToTauh = kFALSE;
	      // isLeg1MatchedToTaum = kFALSE;
	      // isLeg1MatchedToTaue = kFALSE;
	      // isLeg2MatchedToTauh = kFALSE;
	      // isLeg2MatchedToTaum = kFALSE;
	      // isLeg2MatchedToTaue = kFALSE;

	      Bool_t JustMatched = kFALSE ;

	      if(isTauh && lep1.DeltaR(gen_part)<0.5)
		{
		  isLeg1MatchedToTauh = kTRUE;
		  JustMatched = kTRUE ;
		}
	      if(isTaum && lep1.DeltaR(gen_part)<0.5)
		{
		  isLeg1MatchedToTaum = kTRUE;
		  JustMatched = kTRUE ;
		}
	      if(isTaue && lep1.DeltaR(gen_part)<0.5)
		{
		  isLeg1MatchedToTaue = kTRUE;
		  JustMatched = kTRUE ;
		}
	      
	      if(isTauh && lep2.DeltaR(gen_part)<0.5)
		{
		  isLeg2MatchedToTauh = kTRUE;
		  JustMatched = kTRUE ;
		}
	      if(isTaum && lep2.DeltaR(gen_part)<0.5)
		{
		  isLeg2MatchedToTaum = kTRUE;
		  JustMatched = kTRUE ;
		}
	      if(isTaue && lep2.DeltaR(gen_part)<0.5)
		{
		  isLeg2MatchedToTaue = kTRUE;
		  JustMatched = kTRUE ;
		}

	      if((isLeg1MatchedToTauh || isLeg1MatchedToTaum || isLeg1MatchedToTaue) && gen_part.Pt()>Leg1MatchPt && JustMatched)
		{
		  Leg1MatchPt = gen_part.Pt();
		  Leg1MatchEta = gen_part.Eta();
		  Leg1MatchPhi = gen_part.Phi();
		  Leg1MatchE = gen_part.E();
		}
	      if((isLeg2MatchedToTauh || isLeg2MatchedToTaum || isLeg2MatchedToTaue) && gen_part.Pt()>Leg2MatchPt && JustMatched)
		{
		  Leg2MatchPt = gen_part.Pt();
		  Leg2MatchEta = gen_part.Eta();
		  Leg2MatchPhi = gen_part.Phi();
		  Leg2MatchE = gen_part.E();
		}

	      if((isLeg1MatchedToTauh || isLeg1MatchedToTaum || isLeg1MatchedToTaue) && isTauFromSMHiggs && JustMatched) isLeg1FromSMHiggs = kTRUE;
	      if((isLeg2MatchedToTauh || isLeg2MatchedToTaum || isLeg2MatchedToTaue) && isTauFromSMHiggs && JustMatched) isLeg2FromSMHiggs = kTRUE;

	      if((isLeg1MatchedToTauh || isLeg1MatchedToTaum || isLeg1MatchedToTaue) && isTauFromMSSMHiggs && JustMatched) isLeg1FromMSSMHiggs = kTRUE;
	      if((isLeg2MatchedToTauh || isLeg2MatchedToTaum || isLeg2MatchedToTaue) && isTauFromMSSMHiggs && JustMatched) isLeg2FromMSSMHiggs = kTRUE;
	    }
	  
      
	  //event level
	  run = RunNumber ;
	  lumi = lumi_LLR ;
	  evt = EventNumber ;
	  isZtt = false;//not there in LLR
	  isZmt = false ;//not there in LLR
	  isZet = false ;//not there in LLR
	  isZee = false ;//not there in LLR
	  isZem = false ;//not there in LLR
	  isZmm = false ;//not there in LLR
	  isZEE = false ;//not there in LLR
	  isZMM = false ;//not there in LLR
	  isZLL = false ;//not there in LLR
	  isFake = false ;//not there in LLR
	  NUP = NUP_LLR;//not there in LLR
	  secondMuon = 0;//not there in LLR
	  weight = 1;//not there in LLR
	  puweight = PUReweight;
	  npv = npv_LLR;
	  npu = npu_LLR;
	  rho = rho_LLR;

	  //lep1
	  TLorentzVector Lep1  ;
	  Lep1.SetPxPyPzE(daughters_px->at(lep1Index),daughters_py->at(lep1Index),daughters_pz->at(lep1Index),daughters_e->at(lep1Index));      
	  pt_1 = Lep1.Pt();
	  px_1 = Lep1.Px();
	  py_1 = Lep1.Py();
	  pz_1 = Lep1.Pz();
	  phi_1 = Lep1.Phi();
	  eta_1 = Lep1.Eta();
	  m_1 = Lep1.M();
	  if(PDGIdDaughters->at(lep1Index)>0) q_1 = -1;
	  else if(PDGIdDaughters->at(lep1Index)<0) q_1 = +1;
	  else  q_1 = 0;
	  d0_1 = dxy->at(lep1Index);
	  dZ_1 = dz->at(lep1Index);
	  mt_1 = mT_Dau1->at(lepPairIndex);
	  iso_1 = combreliso->at(lep1Index);
	  l1_decayMode = decayMode->at(lep1Index);
	  int discriminator_m = int(discriminator->at(lep1Index));
	  id_m_loose_1 = ((discriminator_m >> 1) & 1) ;//loose
	  id_m_medium_1 = ((discriminator_m >> 2) & 1) ;//medium
	  id_m_tight_1 = ((discriminator_m >> 3) & 1) ;//tight
	  id_m_tightnovtx_1 = ((discriminator_m >> 5) & 1);//tight no vtx
	  id_m_highpt_1 = ((discriminator_m >> 4) & 1);//high pt
	  id_e_mva_nt_loose_1 = -99;//not there in LLR
	  id_e_mva_nt_tight_1 = -99;//not there in LLR
	  id_e_cut_veto_1 = ((daughters_eleCUTID->at(lep1Index) >> 0) & 1) ;
	  id_e_cut_loose_1 = ((daughters_eleCUTID->at(lep1Index) >> 1) & 1) ;
	  id_e_cut_medium_1 = ((daughters_eleCUTID->at(lep1Index) >> 2) & 1) ;
	  id_e_cut_tight_1 = ((daughters_eleCUTID->at(lep1Index) >> 3) & 1) ;
	  trigweight_1 = -99.;//not there in LLR
	  againstElectronLooseMVA5_1 = (tauID->at(indexDau1->at(p)) >>  HelperTrigger_TauID->getTAUidNumber("againstElectronLooseMVA5")) & 1;
	  againstElectronMediumMVA5_1 = (tauID->at(indexDau1->at(p)) >>  HelperTrigger_TauID->getTAUidNumber("againstElectronMediumMVA5")) & 1;
	  againstElectronTightMVA5_1 = false;//not there in LLR
	  againstElectronVLooseMVA5_1 = (tauID->at(indexDau1->at(p)) >>  HelperTrigger_TauID->getTAUidNumber("againstElectronVLooseMVA5")) & 1;
	  againstElectronVTightMVA5_1 = false;//not there in LLR
	  againstMuonLoose3_1 = (tauID->at(indexDau1->at(p)) >>  HelperTrigger_TauID->getTAUidNumber("againstMuonLoose3")) & 1;
	  againstMuonTight3_1 = (tauID->at(indexDau1->at(p)) >>  HelperTrigger_TauID->getTAUidNumber("againstMuonTight3")) & 1;
	  byCombinedIsolationDeltaBetaCorrRaw3Hits_1 = daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits->at(lep1Index);
	  byIsolationMVA3oldDMwoLTraw_1 = -99.;//not there in LLR
	  byIsolationMVA3oldDMwLTraw_1 = -99.;//not there in LLR
	  byIsolationMVA3newDMwoLTraw_1 = -99.;//not there in LLR
	  byIsolationMVA3newDMwLTraw_1 = -99.;//not there in LLR
	  chargedIsoPtSum_1 = daughters_chargedIsoPtSum->at(lep1Index);
	  decayModeFinding_1 = daughters_decayModeFindingOldDMs->at(lep1Index);
	  decayModeFindingNewDMs_1 = daughters_decayModeFindingNewDMs->at(lep1Index);
	  neutralIsoPtSum_1 = daughters_neutralIsoPtSum->at(lep1Index);
	  puCorrPtSum_1 = daughters_puCorrPtSum->at(lep1Index);

	  //lep2
	  TLorentzVector Lep2  ;
	  Lep2.SetPxPyPzE(daughters_px->at(lep2Index),daughters_py->at(lep2Index),daughters_pz->at(lep2Index),daughters_e->at(lep2Index));      
	  pt_2 = Lep2.Pt();
	  px_2 = Lep2.Px();
	  py_2 = Lep2.Py();
	  pz_2 = Lep2.Pz();
	  phi_2 = Lep2.Phi();
	  eta_2 = Lep2.Eta();
	  m_2 = Lep2.M();
	  if(PDGIdDaughters->at(lep2Index)>0) q_2 = -1;
	  else if(PDGIdDaughters->at(lep2Index)<0) q_2 = +1;
	  else  q_2 = 0;
	  d0_2 = dxy->at(lep2Index);
	  dZ_2 = dz->at(lep2Index);
	  mt_2 = mT_Dau2->at(lepPairIndex);
	  iso_2 = combreliso->at(lep2Index);
	  l2_decayMode = decayMode->at(lep2Index);
	  int discriminator_m2 = int(discriminator->at(lep2Index));
	  id_m_loose_2 = ((discriminator_m2 >> 1) & 1) ;//loose
	  id_m_medium_2 = ((discriminator_m2 >> 2) & 1) ;//medium
	  id_m_tight_2 = ((discriminator_m2 >> 3) & 1) ;//tight
	  id_m_tightnovtx_2 = ((discriminator_m2 >> 5) & 1);//tight no vtx
	  id_m_highpt_2 = ((discriminator_m >> 4) & 1);//high pt
	  id_e_mva_nt_loose_2 = -99;//not there in LLR
	  id_e_mva_nt_tight_2 = -99;//not there in LLR
	  id_e_cut_veto_2 = ((daughters_eleCUTID->at(lep2Index) >> 0) & 1) ;
	  id_e_cut_loose_2 = ((daughters_eleCUTID->at(lep2Index) >> 1) & 1) ;
	  id_e_cut_medium_2 = ((daughters_eleCUTID->at(lep2Index) >> 2) & 1) ;
	  id_e_cut_tight_2 = ((daughters_eleCUTID->at(lep2Index) >> 3) & 1) ;
	  trigweight_2 = -99.;//not there in LLR
	  againstElectronLooseMVA5_2 = (tauID->at(indexDau2->at(p)) >>  HelperTrigger_TauID->getTAUidNumber("againstElectronLooseMVA5")) & 1;
	  againstElectronMediumMVA5_2 = (tauID->at(indexDau2->at(p)) >>  HelperTrigger_TauID->getTAUidNumber("againstElectronMediumMVA5")) & 1;
	  againstElectronTightMVA5_2 = false;//not there in LLR
	  againstElectronVLooseMVA5_2 = (tauID->at(indexDau2->at(p)) >>  HelperTrigger_TauID->getTAUidNumber("againstElectronVLooseMVA5")) & 1;
	  againstElectronVTightMVA5_2 = false;//not there in LLR
	  againstMuonLoose3_2 = (tauID->at(indexDau2->at(p)) >>  HelperTrigger_TauID->getTAUidNumber("againstMuonLoose3")) & 1;
	  againstMuonTight3_2 = (tauID->at(indexDau2->at(p)) >>  HelperTrigger_TauID->getTAUidNumber("againstMuonTight3")) & 1;	  

	  byCombinedIsolationDeltaBetaCorrRaw3Hits_2 = daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits->at(lep2Index);
	  byIsolationMVA3oldDMwoLTraw_2 = -99.;//not there in LLR
	  byIsolationMVA3oldDMwLTraw_2 = -99.;//not there in LLR
	  byIsolationMVA3newDMwoLTraw_2 = -99.;//not there in LLR
	  byIsolationMVA3newDMwLTraw_2 = -99.;//not there in LLR
	  chargedIsoPtSum_2 = daughters_chargedIsoPtSum->at(lep2Index);
	  decayModeFinding_2 = daughters_decayModeFindingOldDMs->at(lep2Index);
	  decayModeFindingNewDMs_2 = daughters_decayModeFindingNewDMs->at(lep2Index);
	  neutralIsoPtSum_2 = daughters_neutralIsoPtSum->at(lep2Index);
	  puCorrPtSum_2 = daughters_puCorrPtSum->at(lep2Index);

	  m_vis = (Lep1+Lep2).M();
	  m_sv = SVfitMass->at(lepPairIndex);
	  pt_sv = SVfit_pt->at(lepPairIndex);
	  eta_sv = SVfit_eta->at(lepPairIndex);
	  phi_sv = SVfit_phi->at(lepPairIndex);
	  met_sv = sqrt(METx->at(lepPairIndex)*METx->at(lepPairIndex)+METy->at(lepPairIndex)*METy->at(lepPairIndex));//TBC
	  met =  sqrt(METx->at(lepPairIndex)*METx->at(lepPairIndex)+METy->at(lepPairIndex)*METy->at(lepPairIndex));//TBC
	  TLorentzVector MET_tmp;
	  MET_tmp.SetPxPyPzE(METx->at(lepPairIndex),METy->at(lepPairIndex),0.,met);
	  metphi = MET_tmp.Phi();
	  mvamet = -99.;//not there in LLR
	  mvametphi = -99.;//not there in LLR

	  float cos1 = Lep1.Px()/Lep1.Pt();
	  float cos2 = Lep2.Px()/Lep2.Pt();
	  float sen1 = Lep1.Py()/Lep1.Pt();
	  float sen2 = Lep2.Py()/Lep2.Pt();
      
	  float bisecX = cos1 + cos2;
	  float bisecY = sen1 + sen2;
      
	  float norm = TMath::Sqrt(bisecX*bisecX + bisecY*bisecY);
      
	  if(norm>0.){
	    bisecX /= norm;
	    bisecY /= norm;
	  }

	  pzetavis = (Lep1+Lep2).Px()*bisecX + (Lep1+Lep2).Py()*bisecY;
	  pzetamiss = (Lep1+Lep2+MET_tmp).Px()*bisecX + (Lep1+Lep2+MET_tmp).Py()*bisecY; 
	  mvacov00 = MET_cov00->at(lepPairIndex);
	  mvacov10 = MET_cov10->at(lepPairIndex);
	  mvacov01 = MET_cov01->at(lepPairIndex);
	  mvacov11 = MET_cov11->at(lepPairIndex);
	  pth = (Lep1+Lep2+MET_tmp).Pt();
      
	  //Now going to jets

	  //-> at least 2 jets
	  // if(jets_e->size()<2) continue ;

	  Int_t IndexJet1 = -99;
	  Int_t IndexJet2 = -99;

	  Int_t IndexBJet1 = -99;
	  Int_t IndexBJet2 = -99;

	  // cout<<"Event = "<<EventNumber<<endl;
	  // cout<<"lep1Index = "<<lep1Index<<endl;
	  // cout<<"lep2Index = "<<lep2Index<<endl;
	  // cout<<"****"<<endl;

	  //loop on jets

	  Int_t Njets30 = 0;
	  Int_t Njets20 = 0;
	  Int_t Nbjets20 = 0;

	  for(UInt_t ijet = 0 ; ijet < jets_e->size()  ; ++ijet)
	    {
	      //build jet
	      TLorentzVector jet ;
	      jet.SetPxPyPzE(jets_px->at(ijet),jets_py->at(ijet),jets_pz->at(ijet),jets_e->at(ijet));
	  
	      if(fabs(jet.Eta())>4.8) continue;

	      //tau-jet overlap removal
	      // bool continueflag = false;

	      //building leptons
	      TLorentzVector Lep1_ForOverlapRemoval  ;
	      Lep1_ForOverlapRemoval.SetPxPyPzE(daughters_px->at(lep1Index),daughters_py->at(lep1Index),daughters_pz->at(lep1Index),daughters_e->at(lep1Index));
	      TLorentzVector Lep2_ForOverlapRemoval ;
	      Lep2_ForOverlapRemoval.SetPxPyPzE(daughters_px->at(lep2Index),daughters_py->at(lep2Index),daughters_pz->at(lep2Index),daughters_e->at(lep2Index));
	      

	      if(Lep1_ForOverlapRemoval.DeltaR(jet)<0.5) continue ;
	      if(Lep2_ForOverlapRemoval.DeltaR(jet)<0.5) continue ;

	      // Float_t PUjetID_PtBoundries[5] = {0.0, 10.0, 20.0, 30.0, 50.0}; 
	      // Float_t PUjetID_AbsEtaBoundries[5] = {0.0, 2.5, 2.75, 3.0, 5.0};

	      // Float_t ptRange_010[4] =  {-0.95,-0.97,-0.97,-0.97};
	      // Float_t ptRange_1020[4] = {-0.95,-0.97,-0.97,-0.97};
	      // Float_t ptRange_2030[4] = {-0.63,-0.60,-0.55,-0.45}; 
	      // Float_t ptRange_3050[4] =  {-0.63,-0.60,-0.55,-0.45};

	      Float_t PUJetIDCut = -1.;
	      if(jet.Pt()>0. && jet.Pt()<10.)
		{
		  if(fabs(jet.Eta())>0. && fabs(jet.Eta())<2.5) PUJetIDCut = -0.95;
		  if(fabs(jet.Eta())>2.5 && fabs(jet.Eta())<2.75) PUJetIDCut = -0.97;
		  if(fabs(jet.Eta())>2.75 && fabs(jet.Eta())<3.0) PUJetIDCut = -0.97;
		  if(fabs(jet.Eta())>3.0 && fabs(jet.Eta())<5.0) PUJetIDCut = -0.97;
		}
	      else if(jet.Pt()>10. && jet.Pt()<20.)
		{
		  if(fabs(jet.Eta())>0. && fabs(jet.Eta())<2.5) PUJetIDCut = -0.95;
		  if(fabs(jet.Eta())>2.5 && fabs(jet.Eta())<2.75) PUJetIDCut = -0.97;
		  if(fabs(jet.Eta())>2.75 && fabs(jet.Eta())<3.0) PUJetIDCut = -0.97;
		  if(fabs(jet.Eta())>3.0 && fabs(jet.Eta())<5.0) PUJetIDCut = -0.97;
		}
	      else if(jet.Pt()>20. && jet.Pt()<30.)
		{
		  if(fabs(jet.Eta())>0. && fabs(jet.Eta())<2.5) PUJetIDCut = -0.63;
		  if(fabs(jet.Eta())>2.5 && fabs(jet.Eta())<2.75) PUJetIDCut = -0.60;
		  if(fabs(jet.Eta())>2.75 && fabs(jet.Eta())<3.0) PUJetIDCut = -0.55;
		  if(fabs(jet.Eta())>3.0 && fabs(jet.Eta())<5.0) PUJetIDCut = -0.45;
		}		    
	      else if(jet.Pt()>30.)
		{
		  if(fabs(jet.Eta())>0. && fabs(jet.Eta())<2.5) PUJetIDCut = -0.63;
		  if(fabs(jet.Eta())>2.5 && fabs(jet.Eta())<2.75) PUJetIDCut = -0.60;
		  if(fabs(jet.Eta())>2.75 && fabs(jet.Eta())<3.0) PUJetIDCut = -0.55;
		  if(fabs(jet.Eta())>3.0 && fabs(jet.Eta())<5.0) PUJetIDCut = -0.45;
		}
	  

	      Bool_t isBJet = kFALSE ;
	      if(bCSVscore->at(ijet)>0.679 && fabs(jet.Eta())<2.4 && jet.Pt()>20.) isBJet = kTRUE ;

	      // cout<<"jet #"<<ijet<<", pT = "<<jet.Pt()<<", eta = "<<jet.Eta()<<", phi = "<<jet.Phi()<<", BJET? "<<isBJet<<endl;
	  	      

	      if(jet.Pt()>30. && !isBJet) Njets30++; 
	      if(jet.Pt()>20. && !isBJet) Njets20++; 

	      // cout<<"PFjetID->at(ijet) = "<<PFjetID->at(ijet)<<endl;

	      // if(IndexJet1>=0 && !(bCSVscore->at(ijet)>0.679 && fabs(jet.Eta())<2.4) && jets_PUJetID->at(ijet)>PUJetIDCut && PFjetID->at(ijet)>1)
	      if(jet.Pt()>30. && IndexJet1>=0 && !(IndexJet2>=0) && !(bCSVscore->at(ijet)>0.679 && fabs(jet.Eta())<2.4) && jets_PUJetID->at(ijet)>PUJetIDCut && PFjetID->at(ijet)>1)
		{
		  IndexJet2 = ijet;
		  continue;
		}
	      if(jet.Pt()>30. && IndexJet1<0 && !(bCSVscore->at(ijet)>0.679 && fabs(jet.Eta())<2.4) && jets_PUJetID->at(ijet)>PUJetIDCut && PFjetID->at(ijet)>1)
		// if(IndexJet1<0 && !(bCSVscore->at(ijet)>0.679 && fabs(jet.Eta())<2.4) && jets_PUJetID->at(ijet)>PUJetIDCut && PFjetID->at(ijet)>1)
		{
		  IndexJet1 = ijet ;
		}
	    }

	  for(UInt_t ijet = 0 ; ijet < jets_e->size()  ; ++ijet)
	    {
	      //loop on jets

	      //build jet
	      TLorentzVector jet ;
	      jet.SetPxPyPzE(jets_px->at(ijet),jets_py->at(ijet),jets_pz->at(ijet),jets_e->at(ijet));
	  
	      if(fabs(jet.Eta())>4.8) continue;

	      //tau-jet overlap removal
	      // bool continueflag = false;

	      //building leptons
	      TLorentzVector Lep1_ForOverlapRemoval  ;
	      Lep1_ForOverlapRemoval.SetPxPyPzE(daughters_px->at(lep1Index),daughters_py->at(lep1Index),daughters_pz->at(lep1Index),daughters_e->at(lep1Index));
	      TLorentzVector Lep2_ForOverlapRemoval ;
	      Lep2_ForOverlapRemoval.SetPxPyPzE(daughters_px->at(lep2Index),daughters_py->at(lep2Index),daughters_pz->at(lep2Index),daughters_e->at(lep2Index));

	      if(Lep1_ForOverlapRemoval.DeltaR(jet)<0.5) continue ;
	      if(Lep2_ForOverlapRemoval.DeltaR(jet)<0.5) continue ;

	      if(jet.Pt()>20. && IndexBJet1>=0 && bCSVscore->at(ijet)>0.679 && fabs(jet.Eta())<2.4)// && jets_PUJetID->at(ijet)>0.0)
		{
		  IndexBJet2 = ijet;
		  break;
		}
	      if(jet.Pt()>20. && IndexBJet1<0 && bCSVscore->at(ijet)>0.679 && fabs(jet.Eta())<2.4)//&& jets_PUJetID->at(ijet)>0.0)
		{
		  IndexBJet1 = ijet ;
		}

	      Bool_t isBJet = kFALSE ;
	      if(bCSVscore->at(ijet)>0.679 && fabs(jet.Eta())<2.4 && jet.Pt()>20.) isBJet = kTRUE ;
	      if(isBJet) Nbjets20++;
	    }

	  // if(lep1Index < 0 || lep2Index < 0 || IndexJet1 < 0 || IndexJet2 < 0) continue ;


	  // cout<<"Event = "<<EventNumber<<endl;
	  // cout<<"lep1Index = "<<lep1Index<<endl;
	  // cout<<"lep2Index = "<<lep2Index<<endl;
	  // cout<<"IndexJet1 = "<<IndexJet1<<endl;
	  // cout<<"IndexJet2 = "<<IndexJet2<<endl;
	  // cout<<"****"<<endl;

	  TLorentzVector Jet1 ;
	  TLorentzVector Jet2 ;
	  if(IndexJet1>=0) Jet1.SetPxPyPzE(jets_px->at(IndexJet1),jets_py->at(IndexJet1),jets_pz->at(IndexJet1),jets_e->at(IndexJet1));
	  if(IndexJet2>=0) Jet2.SetPxPyPzE(jets_px->at(IndexJet2),jets_py->at(IndexJet2),jets_pz->at(IndexJet2),jets_e->at(IndexJet2));

	  TLorentzVector BJet1 ;
	  TLorentzVector BJet2 ;
	  if(IndexBJet1>=0) BJet1.SetPxPyPzE(jets_px->at(IndexBJet1),jets_py->at(IndexBJet1),jets_pz->at(IndexBJet1),jets_e->at(IndexBJet1));
	  if(IndexBJet2>=0) BJet2.SetPxPyPzE(jets_px->at(IndexBJet2),jets_py->at(IndexBJet2),jets_pz->at(IndexBJet2),jets_e->at(IndexBJet2));


	  TLorentzVector DiJet;

	  if(IndexJet1>=0 && IndexJet2>=0)
	    {

	      //build final vectors for leptons and jets
	      TLorentzVector Muon ;
	      Muon.SetPxPyPzE(daughters_px->at(lep1Index),daughters_py->at(lep1Index),daughters_pz->at(lep1Index),daughters_e->at(lep1Index));
	      TLorentzVector Tau ;
	      Tau.SetPxPyPzE(daughters_px->at(lep2Index),daughters_py->at(lep2Index),daughters_pz->at(lep2Index),daughters_e->at(lep2Index));

	      // cout<<"Lepton 1 (muon) #"<<lep1Index<<":"<<endl;
	      // cout<<"   pT  = "<<Muon.Pt()<<endl;
	      // cout<<"   eta = "<<Muon.Eta()<<endl;
	      // cout<<"   phi = "<<Muon.Phi()<<endl;
	      // cout<<"Lepton 2 (tau)  #"<<lep1Index<<":"<<endl;
	      // cout<<"   pT  = "<<Tau.Pt()<<endl;
	      // cout<<"   eta = "<<Tau.Eta()<<endl;
	      // cout<<"   phi = "<<Tau.Phi()<<endl;
	      // cout<<"Jet 1           #"<<IndexJet1<<":"<<endl;
	      // cout<<"   pT  = "<<Jet1.Pt()<<endl;
	      // cout<<"   eta = "<<Jet1.Eta()<<endl;
	      // cout<<"   phi = "<<Jet1.Phi()<<endl;
	      // cout<<"Jet 2           #"<<IndexJet2<<":"<<endl;
	      // cout<<"   pT  = "<<Jet2.Pt()<<endl;
	      // cout<<"   eta = "<<Jet2.Eta()<<endl;
	      // cout<<"   phi = "<<Jet2.Phi()<<endl;

	      Double_t LeadingPartonPt = 0.;
	      Double_t LeadingPartonEta = 0.;

	      if(!DataTrue_MCFalse)
		{
		  for(UInt_t igen = 0 ; igen < genpart_px->size() ; ++igen)
		    {
		      if(!((genpart_flags->at(igen) >> 15) & 1)) continue;
		      if(genpart_pdg->at(igen)==21) continue;
		      TLorentzVector GenPart ;
		      GenPart.SetPxPyPzE(genpart_px->at(igen), genpart_py->at(igen), genpart_pz->at(igen), genpart_e->at(igen));
		      // cout<<"GenPart           #"<<igen<<":"<<endl;
		      // cout<<"   pT  = "<<GenPart.Pt()<<endl;
		      // cout<<"   eta = "<<GenPart.Eta()<<endl;
		      // cout<<"   phi = "<<GenPart.Phi()<<endl;
		      
		      if(fabs(GenPart.Pt())>LeadingPartonPt)
			{
			  LeadingPartonPt = fabs(GenPart.Pt());
			  LeadingPartonEta = fabs(GenPart.Eta());
			}
		    }
		}

	      // cout<<"------"<<endl;


	      //compute Mjj and Deta
	      DiJet = Jet1 + Jet2;
	    }


	  if(IndexJet1>=0 && IndexJet2>=0)
	    {
	      mjj = DiJet.M();
	      // cout<<"mjj = "<<mjj<<endl;
	      jdeta = fabs(Jet1.Eta()-Jet2.Eta());
	      njetingap = 0;
	      for(UInt_t ijet = 0 ; ijet < jets_e->size()  ; ++ijet)
		{
		  //build jet
		  TLorentzVector jet ;
		  jet.SetPxPyPzE(jets_px->at(ijet),jets_py->at(ijet),jets_pz->at(ijet),jets_e->at(ijet));

		  TLorentzVector Lep1_ForOverlapRemoval  ;
		  Lep1_ForOverlapRemoval.SetPxPyPzE(daughters_px->at(lep1Index),daughters_py->at(lep1Index),daughters_pz->at(lep1Index),daughters_e->at(lep1Index));
		  TLorentzVector Lep2_ForOverlapRemoval ;
		  Lep2_ForOverlapRemoval.SetPxPyPzE(daughters_px->at(lep2Index),daughters_py->at(lep2Index),daughters_pz->at(lep2Index),daughters_e->at(lep2Index));
	      
		  if(Lep1_ForOverlapRemoval.DeltaR(jet)<0.5) continue ;
		  if(Lep2_ForOverlapRemoval.DeltaR(jet)<0.5) continue ;
	      
		  if(fabs(jet.Eta())>4.8 || jet.Pt()<20.) continue;
		  if(ijet==IndexJet1 || ijet==IndexJet2) continue;
		  if(jet.Eta()<0. && Jet1.Eta()<0.)
		    {
		      if(jet.Eta()<Jet1.Eta()) continue ;
		    }
		  if(jet.Eta()>0. && Jet1.Eta()>0.)
		    {
		      if(jet.Eta()>Jet1.Eta()) continue ;
		    }
		  if(jet.Eta()<0. && Jet2.Eta()<0.)
		    {
		      if(jet.Eta()<Jet2.Eta()) continue ;
		    }
		  if(jet.Eta()>0. && Jet2.Eta()>0.)
		    {
		      if(jet.Eta()>Jet2.Eta()) continue ;
		    }		
		  njetingap++;
		}
	      jdphi = Jet1.DeltaPhi(Jet2);
	      dijetpt = DiJet.Pt();
	      dijetphi = DiJet.Phi();
	      TLorentzVector DiLep = Lep1+Lep2;
	      visjeteta = fabs(DiJet.Eta()-DiLep.Eta());
	      hdijetphi = DiJet.DeltaPhi(DiLep);
	    }
	  else
	    {
	      mjj = jdeta = jdphi = dijetpt = dijetphi = visjeteta = hdijetphi = -99.;
	      njetingap = -99;
	    }

      
	  ptvis = (Lep1+Lep2).Pt();
      
	  nbtag = Nbjets20;
	  njets = Njets30;
	  njetspt20 = Njets20;

	  if(IndexJet1>=0)
	    {
	      jpt_1 = Jet1.Pt();
	      jeta_1 = Jet1.Eta();
	      jphi_1 = Jet1.Phi();
	      jrawf_1 = -99.;//not there in LLR
	      jmva_1 =  -99.;//not there in LLR
	      jpfid_1 = -99.;//not there in LLR
	      jpuid_1 = jets_PUJetID->at(IndexJet1);
	      jcsv_1 = bCSVscore->at(IndexJet1);
	    }
	  else
	    {
	      jpt_1 = jeta_1 = jphi_1 = jrawf_1 = jmva_1 = jpfid_1 = jpuid_1 = jcsv_1 = -99.;
	    }

	  if(IndexJet2>=0)
	    {
	      jpt_2 = Jet2.Pt();
	      jeta_2 = Jet2.Eta();
	      jphi_2 = Jet2.Phi();
	      jrawf_2 = -99.;//not there in LLR
	      jmva_2 =  -99.;//not there in LLR
	      jpfid_2 = -99.;//not there in LLR
	      jpuid_2 = jets_PUJetID->at(IndexJet2);
	      jcsv_2 = bCSVscore->at(IndexJet2);
	    }
	  else
	    {
	      jpt_2 = jeta_2 = jphi_2 = jrawf_2 = jmva_2 = jpfid_2 = jpuid_2 = jcsv_2 = -99.;
	    }


	  if(IndexBJet1>=0)
	    {
	      bjpt_1 = BJet1.Pt();
	      bjeta_1 = BJet1.Eta();
	      bjphi_1 = BJet1.Phi();
	      bjrawf_1=  -99.;//not there in LLR
	      bjmva_1=  -99.;//not there in LLR
	      bjpfid_1 = -99.;//not there in LLR
	      bjpuid_1 = jets_PUJetID->at(IndexBJet1);
	      bjcsv_1 = bCSVscore->at(IndexBJet1);
	    }
	  else
	    {
	      bjpt_1 = bjeta_1 = bjphi_1 = bjrawf_1 = bjmva_1 = bjpfid_1 = bjpuid_1 = bjcsv_1 = -99.;
	    }


	  if(IndexBJet2>=0)
	    {
	      bjpt_2 = BJet2.Pt();
	      bjeta_2 = BJet2.Eta();
	      bjphi_2 = BJet2.Phi();
	      bjrawf_2=  -99.;//not there in LLR
	      bjmva_2=  -99.;//not there in LLR
	      bjpfid_2 = -99.;//not there in LLR
	      bjpuid_2 = jets_PUJetID->at(IndexBJet2);
	      bjcsv_2 = bCSVscore->at(IndexBJet2);
	    }
	  else
	    {
	      bjpt_2 = bjeta_2 = bjphi_2 = bjrawf_2 = bjmva_2 = bjpfid_2 = bjpuid_2 = bjcsv_2 = -99.;
	    }

	  nb_extra_electrons = -99;//not there in LLR
	  nb_extra_muons = -99;//not there in LLR

	  if(!DataTrue_MCFalse)
	    {
	      for(UInt_t igen = 0 ; igen < genpart_pdg->size() ; ++igen)
		{
		  if(genpart_pdg->at(igen)==25)
		    {
		      TLorentzVector Higgs(genpart_px->at(igen),genpart_py->at(igen),genpart_pz->at(igen),genpart_e->at(igen));
		      gen_Higgs_pt = Higgs.Pt();
		      gen_Higgs_mass = Higgs.M();
		    }
		}
	      
	    }
	  // cout<<"before fill"<<endl;


	  //will fill only the best ranked pairs
	  SyncTree->Fill();
	  // sortedPairs.clear(); 
	  TriggerNames.clear();
	  TriggerBits.clear();

	  TriggerFiredNames.clear() ;
	  TriggerFiredIsDoubleObject.clear() ;
	  TriggerFiredBits.clear();
	  
	  TriggerFiredAndGoodTriggerTypeNames.clear() ;
	  TriggerFiredAndGoodTriggerTypeIsDoubleObject.clear() ;
	  TriggerFiredAndGoodTriggerTypeBits.clear() ;
	  
	  TriggerNamesFinal.clear() ;
	  TriggerBitsFinal.clear() ;	  

	  break;
	  //cout<<"Event = "<<EventNumber<<endl; break;
            
	}
      sortedPairs.clear();
    }

  SyncTree->Write();
  TruthTree->Write();
  hCounter->Write();
  // cout<<"     "<<FileName<<" produced successfully (final entries = "<<SyncTree->GetEntries()<<")"<<endl;

  return ;
}

///////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, const char* argv[])
{
  if(argc < 2){
    cerr<<"Forgot to parse the cfg file --> exit "<<endl;
    return -1;
  }
  
  gConfigParser = new ConfigParser();
  TString config ; 
  config.Form("%s",argv[1]);

  if(!(gConfigParser->init(config))){
    cout << ">>> parseConfigFile::Could not open configuration file " << config << endl;
    return -1;
  }

  vector<string> samplesToSkim = gConfigParser->readStringListOption("general::samplesToSkim");
  vector<string> channels = gConfigParser->readStringListOption("general::channels");
  string outputFolderName =  gConfigParser->readStringOption("general::outputFolderName");

  for(UInt_t iSample ; iSample < samplesToSkim.size() ; ++iSample)
    {
      string thisSampleName = samplesToSkim.at(iSample);
      string toGet = "locations::";
      toGet += thisSampleName ;
      string thisSampleLocation =  gConfigParser->readStringOption(toGet.c_str());
      TString ThisSampleLocation(thisSampleLocation);
      ThisSampleLocation += "*.root";

      cout<<"Processing sample = "<<samplesToSkim.at(iSample)<<endl;
      cout<<"   from --> "<<thisSampleLocation<<endl;
      cout<<"   - "<<endl;

      for(UInt_t iChannel = 0 ; iChannel < channels.size() ; iChannel++)
	{
	  string cut_Config;
	  TString CutConfigName = "selections::selectionsFile";
	  CutConfigName += channels.at(iChannel);
	  cut_Config =  gConfigParser->readStringOption(CutConfigName.Data());	  
	  // cout<<"cut_Config = "<<cut_Config<<endl;
	  gConfigParser->reset();
	  gConfigParser->init(cut_Config.c_str());

	  TString FileName(outputFolderName);
	  FileName += "/syncNtuple_";
	  FileName += channels.at(iChannel);
	  TString Channel = channels.at(iChannel);
	  FileName += "_";
	  FileName += TString(thisSampleName);
	  FileName += ".root";
	  cout<<"   filling "<<channels.at(iChannel)<<" to   --> "<<FileName<<endl;

	  Bool_t DataOrMC = kFALSE ;
	  TString thisSampleName_TStr(thisSampleName);
	  if(thisSampleName_TStr.Contains("Data")) DataOrMC = kTRUE;
	  ProduceSyncNtuple(DataOrMC, ThisSampleLocation, FileName, Channel, gConfigParser);

	  // cout<<"config = "<<config<<endl;
	  gConfigParser->reset();
	  gConfigParser->init(config);
	  // cout<<"    ::done."<<endl;
	}



      cout<<"*******"<<endl;
    }

  // string Channel = gConfigParser->readStringOption("General::Channel");
  // float Lumi = gConfigParser->readFloatOption("General::Lumi");
  //bool ProduceFlatTrees = gConfigParser->readBoolOption("General::ProduceFlatTrees");
  //vector<string> BaselineSelection = gConfigParser->readStringListOption("General::BaselineSelection");

  // cout<<"---- RUNNING ----"<<endl;
  // cout<<"  Analysis            : "<<Analysis<<endl;
  // cout<<"  Channel             : "<<Channel<<endl;
  // cout<<"  ProduceFlatTrees?   : "<<ProduceFlatTrees<<endl;
  // cout<<"  Lumi                : "<<Lumi<<endl;


  
  return 0;
}

