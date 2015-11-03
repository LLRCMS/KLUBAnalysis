#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "TBranch.h"
#include "TString.h"
#include "TLorentzVector.h"

// bigTree is produced on an existing ntuple as follows (see at the end of the file) 
#include "bigTree.h" 
#include "smallTree.h"
#include "OfflineProducerHelper.h"
#include "PUReweight.h"
#include "triggerReader.h"
//#include "../../HHKinFit/interface/HHKinFitMaster.h"
#include "../../HHKinFit2/include/HHKinFitMasterHeavyHiggs.h"
#include "ConfigParser.h"
//#include "../../HHKinFit2/include/exceptions/HHInvMConstraintException.h"
//#include "../../HHKinFit2/include/exceptions/HHEnergyRangeException.h"
#include "exceptions/HHInvMConstraintException.h"
#include "exceptions/HHEnergyRangeException.h"
#include "exceptions/HHEnergyConstraintException.h"

#include "TMVA/MsgLogger.h"
#include "TMVA/Config.h"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"

//
using namespace std ;




/*  the modern way of making lorenzvectors (no warnings)
    here is an example of usage:

https://github.com/govoni/LHEAnalysis/blob/master/epem_01.cpp

    here are the headers needed:

#include "Math/Vector3D.h"
#include "Math/Vector4D.h"

using namespace ROOT::Math;

typedef LorentzVector<ROOT::Math::PxPyPzE4D<double> > lorentzVector ;

    I am missing about how to set one of those things up with Pt, eta, phi coordinates

*/


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// open input txt file and append all the files it contains to TChain
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

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
// open the first file in the input list, retrieve the histogram "Counters" for the trigger names and return a copy of it
TH1F* getFirstFileHisto (TString filename)
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
    TH1F* histo = new TH1F (*dummy);
    histo-> SetDirectory(0);
    histo->SetName ("Counters_perTrigger");
    fIn->Close();
    return histo;
}
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


bool isDegenere (TLorentzVector & first, TLorentzVector & second)
{
  //if ( fabs(first.Px()-second.Px())+fabs(first.Py()-second.Py())+fabs(first.Pz()-second.Pz())<0.03 * (fabs(first.Px())+fabs(first.Py())+fabs(first.Pz()) )) return true;
  if (first.DeltaR(second) < 0.03) return true;

   return false ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


float deltaPhi (float phi1, float phi2)
{
  float deltaphi = fabs (phi1 - phi2) ;
  if (deltaphi > 6.283185308) deltaphi -= 6.283185308 ;
  if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi ;
  return deltaphi ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


struct leptSort: public std::binary_function<pair<TLorentzVector, float> &, pair<TLorentzVector, float> &, bool>
{
  bool operator() (pair<TLorentzVector, float> & x, pair<TLorentzVector, float> & y)
    {
      return x.first.Pt () < y.first.Pt () ;
    }
} ;


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


template <class T>
struct scoreSortSingle: public std::binary_function<pair <T, float> &, pair <T, float> &, bool>
{
  bool operator() (pair <T, float> & x, pair <T, float> & y)
    {
      return x.second < y.second ;
    }
} ;


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


pair<int, int>
chooseHighestPtJets (vector <pair <int, float> > & jets_and_btag)
{
  // leap of faith
  return pair<int, int> (0, 1) ;     
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


pair<int, int>
chooseHighestBtagJets (vector <pair <int, float> > & jets_and_btag)
{
  sort (jets_and_btag.rbegin (), jets_and_btag.rend (), scoreSortSingle<int> ()) ;

  int firstBjetIndex = jets_and_btag.at (0).first ;
  int secondBjetIndex = jets_and_btag.at (1).first ;
  return pair<int, int> (firstBjetIndex, secondBjetIndex) ;     
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


/**
muons = 0
electrons = 1
taus = 2
*/
float getIso (unsigned int iDau, float pt, bigTree & theBigTree)
{
  int type = theBigTree.particleType->at (iDau) ;
  // is tauH
  if (type == 2)
    return theBigTree.daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits->at(iDau) ;
  // muon
  if (type == 1 || type == 0)
    return theBigTree.combreliso->at(iDau) * pt ;

  return -1 ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char** argv)
{
  // read input file and cfg
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  if (argc < 7) 
    {
      cerr << "missing input parameters" << endl ;
      cerr << "usage: " << argv[0]
           << "inputFileNameList outputFileName crossSection isData configFile runHHKinFit" << endl ; 
      exit (1) ;
    }
  TString inputFile = argv[1] ;
  TString outputFile = argv[2] ;
  float XS = atof (argv[3]) ;
  bool isMC = true;
  int isDatabuf = atoi (argv[4]);
  if (isDatabuf == 1)
    {
      cout << "RUNNING ON DATA\n" ;
      isMC = false; 
      XS = 1.;
    }

  if (gConfigParser) return 1 ;
  gConfigParser = new ConfigParser () ;
  
  TString config ; 
  config.Form ("%s",argv[5]) ;
  
  bool runHHKinFit = false;
  string opt7 (argv[6]);
  if (opt7 == "1") runHHKinFit = true;

  cout << "Running HH in fit: " << runHHKinFit << " " << argv[6] << endl;

  if (! (gConfigParser->init (config)))
    {
      cout << ">>> parseConfigFile::Could not open configuration file " << config << endl ;
      return -1 ;
    }

  bool   beInclusive         = gConfigParser->readBoolOption   ("selections::beInclusive") ;
  float  PUjetID_minCut      = gConfigParser->readFloatOption  ("parameters::PUjetIDminCut") ;
  int    saveOS              = gConfigParser->readIntOption    ("parameters::saveOS") ;
  float  lepCleaningCone     = gConfigParser->readFloatOption  ("parameters::lepCleaningCone") ;
  int    bChoiceFlag         = gConfigParser->readFloatOption  ("parameters::bChoiceFlag") ;
  int    PUReweight_MC       = gConfigParser->readFloatOption  ("parameters::PUReweightMC") ; 
  int    PUReweight_target   = gConfigParser->readFloatOption  ("parameters::PUReweighttarget") ; 
  string leptonSelectionFlag = gConfigParser->readStringOption ("parameters::lepSelections") ;
  int maxNjetsSaved          = gConfigParser->readIntOption    ("parameters::maxNjetsSaved") ;

  string TMVAweightsTauTau   = gConfigParser->readStringOption ("TMVA::weightsTauTau");
  string TMVAweightsMuTau    = gConfigParser->readStringOption ("TMVA::weightsMuTau");
  bool TMVAspectatorsIn      = gConfigParser->readBoolOption   ("TMVA::spectatorsPresent");
  vector<string> TMVAspectators = gConfigParser->readStringListOption   ("TMVA::spectators");
  vector<string> TMVAvariables  = gConfigParser->readStringListOption   ("TMVA::variables");


  vector<string> trigMuTau   =  (isMC ? gConfigParser->readStringListOption ("triggersMC::MuTau")  : gConfigParser->readStringListOption ("triggersData::MuTau")) ;
  vector<string> trigTauTau   = (isMC ? gConfigParser->readStringListOption ("triggersMC::TauTau") : gConfigParser->readStringListOption ("triggersData::TauTau")) ;
  vector<string> trigEleTau   = (isMC ? gConfigParser->readStringListOption ("triggersMC::EleTau") : gConfigParser->readStringListOption ("triggersData::EleTau")) ;
  vector<string> trigEleMu   =  (isMC ? gConfigParser->readStringListOption ("triggersMC::EleMu")  : gConfigParser->readStringListOption ("triggersData::EleMu")) ;

  // input and output setup
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  OfflineProducerHelper oph ;

  TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;
  //bigChain->Add (inputFile) ;
  appendFromFileList (bigChain, inputFile);
  bigTree theBigTree (bigChain) ;


  //Create a new file + a clone of old tree header. Do not copy events
  TFile * smallFile = new TFile (outputFile, "recreate") ;
  smallFile->cd () ;
  smallTree theSmallTree ("HTauTauTree") ;

  // these are needed for the HHKinFit
  //vector<Int_t> hypo_mh1 ; //FIXME why is this an integer?!
  //hypo_mh1.push_back (125) ;
  //vector<Int_t> hypo_mh2 ;
  //hypo_mh2.push_back (125) ;
  int hypo_mh1=125,hypo_mh2=125;

  int eventsNumber = 1000;//theBigTree.fChain->GetEntries () ;
  float totalEvents = 0. ;
  
  float selectedEvents = 0. ;

  int totalNoWeightsEventsNum = 0 ;
  int selectedNoWeightsEventsNum = 0 ;
 
  int selectionsNumber = 3 ;
  vector<float> counter (selectionsNumber + 1, 0.) ;

  // ------------------------------

  TH1F* hTriggers = getFirstFileHisto (inputFile);
  triggerReader trigReader (hTriggers);
  trigReader.addTauTauTrigs (trigTauTau);
  trigReader.addMuTauTrigs  (trigMuTau);
  trigReader.addEleTauTrigs (trigEleTau);
  trigReader.addMuEleTrigs  (trigEleMu);

  // ------------------------------

  PUReweight reweight ;

  // ------------------------------

  // loop over events
  for (Long64_t iEvent = 0 ; iEvent < eventsNumber ; ++iEvent) 
    {
      if (iEvent % 10000 == 0)  cout << "reading event " << iEvent << endl ;
      int selID = 0 ;

      theSmallTree.clearVars () ;
      theBigTree.GetEntry (iEvent) ;
      
      if (isMC)
        {
          totalEvents += theBigTree.aMCatNLOweight * reweight.weight(PUReweight_MC,PUReweight_target,theBigTree.npu) ;
          counter.at (selID++) += theBigTree.aMCatNLOweight * reweight.weight(PUReweight_MC,PUReweight_target,theBigTree.npu) ;
        }
      else 
        {
          totalEvents += 1 ;
          counter.at (selID++) += 1 ;
        }
      ++totalNoWeightsEventsNum ;
      
      if (theBigTree.indexDau1->size () == 0) continue ;
      int metbit = theBigTree.metfilterbit;
      int metpass = metbit & (1 << 0);
      metpass += metbit & (1 << 2);
      metpass += metbit & (1 << 5);
      metpass += metbit & (1 << 6);
      if(metpass > 0) continue ;
      
      // exclusive bits asked later
      //int triggerbit = theBigTree.triggerbit;
      //if (triggerbit == 0) continue;

      if (isMC) counter.at (selID++) += theBigTree.aMCatNLOweight * reweight.weight(PUReweight_MC,PUReweight_target,theBigTree.npu) ;
      else      counter.at (selID++) += 1 ;

      // assume that the ordering of the pair numbering
      // is the same as the priority we want to give:
      //  0 : muTau
      //  1 : eTau
      //  2 : TauTau
      //  3 : mumu   
      //  4 : ee
      //  5 : emu
    
      map<int, int> foundPairs ; // pairType, pairIndexInVectors

      for (unsigned int iPair = 0 ; iPair < theBigTree.indexDau1->size () ; ++iPair)
        {
          // FIXME need to implement here the choice of iso / anti-iso
          if (!oph.pairPassBaseline (&theBigTree, iPair, leptonSelectionFlag.c_str ())) continue ;

          int firstDaughterIndex = theBigTree.indexDau1->at (iPair) ;  
          int secondDaughterIndex = theBigTree.indexDau2->at (iPair) ;
          
          TLorentzVector tlv_firstLepton
            (
              theBigTree.daughters_px->at (firstDaughterIndex),
              theBigTree.daughters_py->at (firstDaughterIndex),
              theBigTree.daughters_pz->at (firstDaughterIndex),
              theBigTree.daughters_e->at (firstDaughterIndex)
            ) ;
          TLorentzVector tlv_secondLepton
            (
              theBigTree.daughters_px->at (secondDaughterIndex),
              theBigTree.daughters_py->at (secondDaughterIndex),
              theBigTree.daughters_pz->at (secondDaughterIndex),
              theBigTree.daughters_e->at (secondDaughterIndex)
            ) ;
          //if (isDegenere (tlv_firstLepton, tlv_secondLepton)) continue; // antiEle and antiMu should already do this dirty job

          int type1 = theBigTree.particleType->at (firstDaughterIndex) ;
          int type2 = theBigTree.particleType->at (secondDaughterIndex) ;        
          int pairType = oph.getPairType (type1, type2) ;
          if (foundPairs.find (pairType) != foundPairs.end ()) continue ;
          
          foundPairs[pairType] = iPair ;
        }  

      if (foundPairs.size () == 0) continue ;

      // by now take the MOST ISOLATED pair (no OS/SS)
      int chosenTauPair = foundPairs.begin ()->second ;
      int isOS = theBigTree.isOSCand->at (chosenTauPair) ;

      if (saveOS == 1 && !isOS) continue ;
      if (saveOS == 0 &&  isOS) continue ;
      
      int triggerbit = theBigTree.triggerbit;
      int thisPairType = foundPairs.begin()->first ; // this is the pairType used
      bool ORtrigBits = trigReader.checkOR (thisPairType, triggerbit);
      if (!ORtrigBits) continue;

      if (isMC) counter.at (selID++) += theBigTree.aMCatNLOweight * reweight.weight(PUReweight_MC,PUReweight_target,theBigTree.npu) ;
      else      counter.at (selID++) += 1 ;

      int firstDaughterIndex = theBigTree.indexDau1->at (chosenTauPair) ;  
      TLorentzVector tlv_firstLepton
        (
          theBigTree.daughters_px->at (firstDaughterIndex),
          theBigTree.daughters_py->at (firstDaughterIndex),
          theBigTree.daughters_pz->at (firstDaughterIndex),
          theBigTree.daughters_e->at (firstDaughterIndex)
        ) ;
      int secondDaughterIndex = theBigTree.indexDau2->at (chosenTauPair) ;
      TLorentzVector tlv_secondLepton
        (
          theBigTree.daughters_px->at (secondDaughterIndex),
          theBigTree.daughters_py->at (secondDaughterIndex),
          theBigTree.daughters_pz->at (secondDaughterIndex),
          theBigTree.daughters_e->at (secondDaughterIndex)
        ) ;

      TLorentzVector tlv_tauH = tlv_firstLepton + tlv_secondLepton ;
      TLorentzVector tlv_tauH_SVFIT ;

      // in case the SVFIT mass is calculated
      if (theBigTree.SVfitMass->at (chosenTauPair) > -900.)
        {
          theSmallTree.m_tauH_SVFIT_mass = theBigTree.SVfitMass->at (chosenTauPair) ;

          theSmallTree.m_tauH_SVFIT_pt = theBigTree.SVfit_pt->at (chosenTauPair) ;
          theSmallTree.m_tauH_SVFIT_eta = theBigTree.SVfit_eta->at (chosenTauPair) ;
          theSmallTree.m_tauH_SVFIT_phi = theBigTree.SVfit_phi->at (chosenTauPair) ;
          theSmallTree.m_tauH_SVFIT_METphi = theBigTree.SVfit_fitMETPhi->at (chosenTauPair) ;
          theSmallTree.m_tauH_SVFIT_METrho = theBigTree.SVfit_fitMETRho->at (chosenTauPair) ;

          tlv_tauH_SVFIT.SetPtEtaPhiM (
              theBigTree.SVfit_pt->at (chosenTauPair),
              theBigTree.SVfit_eta->at (chosenTauPair),
              theBigTree.SVfit_phi->at (chosenTauPair),
              theBigTree.SVfitMass->at (chosenTauPair)
            ) ;
        }

      vector <pair <int, float> > jets_and_btag ;
      // loop over jets
      for (unsigned int iJet = 0 ; iJet < theBigTree.jets_px->size () ; ++iJet)
        {
          // PG filter jets at will
          if (theBigTree.jets_PUJetID->at (iJet) < PUjetID_minCut) continue ;
      
          TLorentzVector tlv_jet 
            (
              theBigTree.jets_px->at (iJet),
              theBigTree.jets_py->at (iJet),
              theBigTree.jets_pz->at (iJet),
              theBigTree.jets_e->at (iJet)
            ) ;
          if (tlv_jet.Pt () < 20. /*GeV*/) continue ;  
          if (tlv_jet.DeltaR (tlv_firstLepton) < lepCleaningCone) continue ;
          if (tlv_jet.DeltaR (tlv_secondLepton) < lepCleaningCone) continue ;

          jets_and_btag.push_back (std::pair <int, float> (
              iJet, theBigTree.bCSVscore->at (iJet)
            )) ;          

        } // loop over jets

      // apply some selections here
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
      
      if (!beInclusive && jets_and_btag.size () < 2) continue ;
      if (isMC) counter.at (selID++) += theBigTree.aMCatNLOweight * reweight.weight(PUReweight_MC,PUReweight_target,theBigTree.npu) ;
      else      counter.at (selID++) += 1 ;
      
      // fill the variables of interest
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

      theSmallTree.m_PUReweight = (isMC ? reweight.weight(PUReweight_MC,PUReweight_target,theBigTree.npu) : 1) ;      
      theSmallTree.m_MC_weight = (isMC ? theBigTree.aMCatNLOweight * XS : 1) ;
      theSmallTree.m_EventNumber = theBigTree.EventNumber ;
      theSmallTree.m_RunNumber = theBigTree.RunNumber ;
      theSmallTree.m_npv = theBigTree.npv ;
      theSmallTree.m_npu = theBigTree.npu ;
      theSmallTree.m_lumi = theBigTree.lumi ;
      theSmallTree.m_triggerbit = theBigTree.triggerbit ;
      theSmallTree.m_rho = theBigTree.rho ;
      theSmallTree.m_isMC = isMC ;
      theSmallTree.m_isOS = theBigTree.isOSCand->at (chosenTauPair) ;
      theSmallTree.m_met_phi = theBigTree.metphi ;
      theSmallTree.m_met_et = theBigTree.met ;
      theSmallTree.m_mT = theBigTree.mT_Dau1->at (chosenTauPair) ;

      const int type1 = theBigTree.particleType->at (firstDaughterIndex) ;
      const int type2 = theBigTree.particleType->at (secondDaughterIndex) ;
      theSmallTree.m_pairType = oph.getPairType (type1, type2) ;
      
      theSmallTree.m_tauH_pt = tlv_tauH.Pt () ;
      theSmallTree.m_tauH_eta = tlv_tauH.Eta () ;
      theSmallTree.m_tauH_phi = tlv_tauH.Phi () ;
      theSmallTree.m_tauH_e = tlv_tauH.E () ;
      theSmallTree.m_tauH_mass = tlv_tauH.M () ;

      theSmallTree.m_dau1_iso = getIso (firstDaughterIndex, tlv_firstLepton.Pt (), theBigTree) ;
      theSmallTree.m_dau1_pt = tlv_firstLepton.Pt () ;
      theSmallTree.m_dau1_eta = tlv_firstLepton.Eta () ;
      theSmallTree.m_dau1_phi = tlv_firstLepton.Phi () ;
      theSmallTree.m_dau1_e = theBigTree.daughters_e->at (firstDaughterIndex) ;
      theSmallTree.m_dau1_flav = theBigTree.daughters_charge->at (firstDaughterIndex) * 
                                 (theBigTree.particleType->at (firstDaughterIndex) + 1) ;
                                 // 1 = from muons collection
                                 // 2 = from electrons collection
                                 // 3 = from tauH collection
                                 
      theSmallTree.m_dau2_iso = getIso (secondDaughterIndex, tlv_secondLepton.Pt (), theBigTree) ;
      theSmallTree.m_dau2_pt = tlv_secondLepton.Pt () ;
      theSmallTree.m_dau2_eta = tlv_secondLepton.Eta () ;
      theSmallTree.m_dau2_phi = tlv_secondLepton.Phi () ;
      theSmallTree.m_dau2_e = theBigTree.daughters_e->at (secondDaughterIndex) ;
      theSmallTree.m_dau2_flav = theBigTree.daughters_charge->at (secondDaughterIndex) * 
                                 (theBigTree.particleType->at (secondDaughterIndex) + 1) ;

      // loop over leptons
      vector<pair<TLorentzVector, float> > dummyLeptCollection ;
      for (unsigned int iLep = 0 ; 
           (iLep < theBigTree.daughters_px->size ()) ;
           ++iLep)
        {
          // skip the H decay candiates
          if (int (iLep) == firstDaughterIndex || 
              int (iLep) == secondDaughterIndex) continue ;

          // remove taus
          if (theBigTree.particleType->at (iLep) == 2)
            {
              continue ;
            }  
          else if (theBigTree.particleType->at (iLep) == 0) // muons
            {
              if (!oph.muBaseline (&theBigTree, iLep, 10., 2.4, 0.3)) continue ;
            }
          else if (theBigTree.particleType->at (iLep) == 1) // electrons
            {
              if (!oph.eleBaseline (&theBigTree, iLep, 10., 0.3, 1)) continue ;
//              if (!oph.eleBaseline (&theBigTree, iLep, 10., 2.5, 0.3, 1)) continue ;
            }
          TLorentzVector tlv_dummyLepton
            (
              theBigTree.daughters_px->at (iLep),
              theBigTree.daughters_py->at (iLep),
              theBigTree.daughters_pz->at (iLep),
              theBigTree.daughters_e->at (iLep)
            ) ;
          dummyLeptCollection.push_back (pair<TLorentzVector, float> (
              tlv_dummyLepton, 
              theBigTree.daughters_charge->at (iLep) * 
              (theBigTree.particleType->at (iLep) + 1)              
            )) ;
          
        } // loop over leptons

      sort (dummyLeptCollection.rbegin (), dummyLeptCollection.rend (), leptSort ()) ;
      for (unsigned int iLep = 0 ; 
           (iLep < dummyLeptCollection.size ()) && (theSmallTree.m_nleps < 2) ;
           ++iLep)
        {
          theSmallTree.m_leps_pt.push_back (dummyLeptCollection.at (iLep).first.Pt ()) ;
          theSmallTree.m_leps_eta.push_back (dummyLeptCollection.at (iLep).first.Eta ()) ;
          theSmallTree.m_leps_phi.push_back (dummyLeptCollection.at (iLep).first.Phi ()) ;
          theSmallTree.m_leps_e.push_back (dummyLeptCollection.at (iLep).first.E ()) ;
          theSmallTree.m_leps_flav.push_back (dummyLeptCollection.at (iLep).second) ;
          ++theSmallTree.m_nleps ;
        } 

      if (jets_and_btag.size () >= 2) 
        { 
          pair<int, int> eventJets ;
          if (bChoiceFlag == 1)       eventJets = chooseHighestBtagJets (jets_and_btag) ;
          else if (bChoiceFlag == 2)  eventJets = chooseHighestPtJets (jets_and_btag) ;
          
          TLorentzVector tlv_firstBjet 
            (
              theBigTree.jets_px->at (eventJets.first),
              theBigTree.jets_py->at (eventJets.first),
              theBigTree.jets_pz->at (eventJets.first),
              theBigTree.jets_e->at (eventJets.first)
            ) ;
          TLorentzVector tlv_secondBjet 
            (
              theBigTree.jets_px->at (eventJets.second),
              theBigTree.jets_py->at (eventJets.second),
              theBigTree.jets_pz->at (eventJets.second),
              theBigTree.jets_e->at (eventJets.second)
            ) ;

          theSmallTree.m_bjet1_pt  = tlv_firstBjet.Pt () ;
          theSmallTree.m_bjet1_eta = tlv_firstBjet.Eta () ;
          theSmallTree.m_bjet1_phi = tlv_firstBjet.Phi () ;
          theSmallTree.m_bjet1_e = theBigTree.jets_e->at (eventJets.first) ;
          theSmallTree.m_bjet1_bID = theBigTree.bCSVscore->at (eventJets.first) ;
          
          theSmallTree.m_bjet2_pt  = tlv_secondBjet.Pt () ;
          theSmallTree.m_bjet2_eta = tlv_secondBjet.Eta () ;
          theSmallTree.m_bjet2_phi = tlv_secondBjet.Phi () ;
          theSmallTree.m_bjet2_e = theBigTree.jets_e->at (eventJets.second) ;
          theSmallTree.m_bjet2_bID = theBigTree.bCSVscore->at (eventJets.second) ;

          float METx = theBigTree.METx->at (chosenTauPair) ;
          float METy = theBigTree.METy->at (chosenTauPair) ;
          float METpt = 0;//TMath::Sqrt (METx*METx + METy*METy) ;
    
	        TLorentzVector ptmiss = TLorentzVector(METx, METy, 0., METpt) ;
          //TVector2 ptmiss = TVector2(METx,METy);
          TMatrixD metcov (2, 2) ;
          metcov (0,0) = theBigTree.MET_cov00->at (chosenTauPair) ;
          metcov (1,0) = theBigTree.MET_cov10->at (chosenTauPair) ;
          metcov (0,1) = theBigTree.MET_cov01->at (chosenTauPair) ;    
          metcov (1,1) = theBigTree.MET_cov11->at (chosenTauPair) ;
    
          TLorentzVector tlv_bH = tlv_firstBjet + tlv_secondBjet ;
          theSmallTree.m_bH_pt = tlv_bH.Pt () ;
          theSmallTree.m_bH_eta = tlv_bH.Eta () ;
          theSmallTree.m_bH_phi = tlv_bH.Phi () ;
          theSmallTree.m_bH_e = tlv_bH.E () ;
          theSmallTree.m_bH_mass = tlv_bH.M () ;

          TLorentzVector tlv_HH = tlv_bH + tlv_tauH ;
          theSmallTree.m_HH_pt = tlv_HH.Pt () ;
          theSmallTree.m_HH_eta = tlv_HH.Eta () ;
          theSmallTree.m_HH_phi = tlv_HH.Phi () ;
          theSmallTree.m_HH_e = tlv_HH.E () ;
          theSmallTree.m_HH_mass = tlv_HH.M () ;

          // in case the SVFIT mass is calculated
          if (theBigTree.SVfitMass->at (chosenTauPair) > -900.)
            {
              TLorentzVector tlv_HHsvfit = tlv_bH + tlv_tauH_SVFIT ;
              theSmallTree.m_HHsvfit_pt = tlv_HHsvfit.Pt () ;
              theSmallTree.m_HHsvfit_eta = tlv_HHsvfit.Eta () ;
              theSmallTree.m_HHsvfit_phi = tlv_HHsvfit.Phi () ;
              theSmallTree.m_HHsvfit_e = tlv_HHsvfit.E () ;
            } // in case the SVFIT mass is calculated
        
//           //intance of fitter master class
//           HHKinFitMaster kinFits = HHKinFitMaster (&tlv_firstBjet, &tlv_secondBjet,

          bool wrongHHK=false;
          float HHKmass = -999;
          float HHKChi2 = -999;
          if (runHHKinFit)
          {
            HHKinFit2::HHKinFitMasterHeavyHiggs kinFits = HHKinFit2::HHKinFitMasterHeavyHiggs (&tlv_firstBjet, &tlv_secondBjet, 
                                                      &tlv_firstLepton, &tlv_secondLepton,&ptmiss,metcov) ;
  //           kinFits.setAdvancedBalance (&ptmiss, metcov) ;
  //           kinFits.setSimpleBalance (ptmiss.Pt (),10) ; //alternative which uses only the absolute value of ptmiss in the fit
  // 
  //           kinFits.addMh1Hypothesis (hypo_mh1) ;
  //           kinFits.addMh2Hypothesis (hypo_mh2) ;
            kinFits.addHypo(hypo_mh1,hypo_mh2);
            try{           
                kinFits.doFit();//doFit () ; 
            }
            catch(HHKinFit2::HHInvMConstraintException e){
              cout<<"INVME THIS EVENT WAS WRONG, INV MASS CONSTRAIN EXCEPTION"<<endl;
              cout<<"INVME masshypo1 = 125,    masshypo2 = 125"<<endl;
              cout<<"INVME Tau1"<<endl;
              cout<<"INVME (E,Px,Py,Pz,M) "<<tlv_firstLepton.E()<<","<<tlv_firstLepton.Px()<<","<<tlv_firstLepton.Py()<<","<<tlv_firstLepton.Pz()<<","<<tlv_firstLepton.M()<<endl;//tlv_firstLepton.Print();
              cout<<"INVME Tau2"<<endl;
              cout<<"INVME (E,Px,Py,Pz,M) "<<tlv_secondLepton.E()<<","<<tlv_secondLepton.Px()<<","<<tlv_secondLepton.Py()<<","<<tlv_secondLepton.Pz()<<","<<tlv_secondLepton.M()<<endl;
              cout<<"INVME B1"<<endl;
              cout<<"INVME (E,Px,Py,Pz,M) "<<tlv_firstBjet.E()<<","<<tlv_firstBjet.Px()<<","<<tlv_firstBjet.Py()<<","<<tlv_firstBjet.Pz()<<","<<tlv_firstBjet.M()<<endl;
              cout<<"INVME B2"<<endl;
              cout<<"INVME (E,Px,Py,Pz,M) "<<tlv_secondBjet.E()<<","<<tlv_secondBjet.Px()<<","<<tlv_secondBjet.Py()<<","<<tlv_secondBjet.Pz()<<","<<tlv_secondBjet.M()<<endl;
              cout<<"INVME MET"<<endl;
              cout<<"INVME (E,Px,Py,Pz,M) "<<ptmiss.E()<<","<<ptmiss.Px()<<","<<ptmiss.Py()<<","<<ptmiss.Pz()<<","<<ptmiss.M()<<endl;
              cout<<"INVME METCOV "<<endl;
              cout<<"INVME "<<metcov (0,0)<<"  "<<metcov (0,1)<<endl;// = theBigTree.MET_cov00->at (chosenTauPair) ;
              cout<<"INVME "<<metcov (1,0)<<"  "<<metcov (1,1)<<endl;// = theBigTree.MET_cov10->at (chosenTauPair) ;
              cout<<"INVME tau1, tau2, b1, b2"<<endl;
              cout<<"INVME ";
              tlv_firstLepton.Print();
              cout<<"INVME ";
              tlv_secondLepton.Print();
              cout<<"INVME ";
              tlv_firstBjet.Print();
              cout<<"INVME ";
              tlv_secondBjet.Print();
              wrongHHK=true;
              }
            catch (HHKinFit2::HHEnergyRangeException e){
              cout<<"ERANGE THIS EVENT WAS WRONG, ENERGY RANGE EXCEPTION"<<endl;
              cout<<"ERANGE masshypo1 = 125,    masshypo2 = 125"<<endl;
              cout<<"ERANGE Tau1"<<endl;
              cout<<"ERANGE (E,Px,Py,Pz,M) "<<tlv_firstLepton.E()<<","<<tlv_firstLepton.Px()<<","<<tlv_firstLepton.Py()<<","<<tlv_firstLepton.Pz()<<","<<tlv_firstLepton.M()<<endl;//tlv_firstLepton.Print();
              cout<<"ERANGE Tau2"<<endl;
              cout<<"ERANGE (E,Px,Py,Pz,M) "<<tlv_secondLepton.E()<<","<<tlv_secondLepton.Px()<<","<<tlv_secondLepton.Py()<<","<<tlv_secondLepton.Pz()<<","<<tlv_secondLepton.M()<<endl;
              cout<<"ERANGE B1"<<endl;
              cout<<"ERANGE (E,Px,Py,Pz,M) "<<tlv_firstBjet.E()<<","<<tlv_firstBjet.Px()<<","<<tlv_firstBjet.Py()<<","<<tlv_firstBjet.Pz()<<","<<tlv_firstBjet.M()<<endl;
              cout<<"ERANGE B2"<<endl;
              cout<<"ERANGE (E,Px,Py,Pz,M) "<<tlv_secondBjet.E()<<","<<tlv_secondBjet.Px()<<","<<tlv_secondBjet.Py()<<","<<tlv_secondBjet.Pz()<<","<<tlv_secondBjet.M()<<endl;
              cout<<"ERANGE MET"<<endl;
              cout<<"ERANGE (E,Px,Py,Pz,M) "<<ptmiss.E()<<","<<ptmiss.Px()<<","<<ptmiss.Py()<<","<<ptmiss.Pz()<<","<<ptmiss.M()<<endl;
              cout<<"ERANGE METCOV "<<endl;
              cout<<"ERANGE "<<metcov (0,0)<<"  "<<metcov (0,1)<<endl;// = theBigTree.MET_cov00->at (chosenTauPair) ;
              cout<<"ERANGE "<<metcov (1,0)<<"  "<<metcov (1,1)<<endl;// = theBigTree.MET_cov10->at (chosenTauPair) ;
              cout<<"ERANGE tau1, tau2, b1, b2"<<endl;
              cout<<"ERANGE ";
              tlv_firstLepton.Print();
              cout<<"ERANGE ";
              tlv_secondLepton.Print();
              cout<<"ERANGE ";
              tlv_firstBjet.Print();
              cout<<"ERANGE ";
              tlv_secondBjet.Print();
              wrongHHK=true;
            }
            catch(HHKinFit2::HHEnergyConstraintException e){
              cout<<"ECON THIS EVENT WAS WRONG, ENERGY CONSTRAIN EXCEPTION"<<endl;
              cout<<"ECON masshypo1 = 125,    masshypo2 = 125"<<endl;
              cout<<"ECON Tau1"<<endl;
              cout<<"ECON (E,Px,Py,Pz,M) "<<tlv_firstLepton.E()<<","<<tlv_firstLepton.Px()<<","<<tlv_firstLepton.Py()<<","<<tlv_firstLepton.Pz()<<","<<tlv_firstLepton.M()<<endl;//tlv_firstLepton.Print();
              cout<<"ECON Tau2"<<endl;
              cout<<"ECON (E,Px,Py,Pz,M) "<<tlv_secondLepton.E()<<","<<tlv_secondLepton.Px()<<","<<tlv_secondLepton.Py()<<","<<tlv_secondLepton.Pz()<<","<<tlv_secondLepton.M()<<endl;
              cout<<"ECON B1"<<endl;
              cout<<"ECON (E,Px,Py,Pz,M) "<<tlv_firstBjet.E()<<","<<tlv_firstBjet.Px()<<","<<tlv_firstBjet.Py()<<","<<tlv_firstBjet.Pz()<<","<<tlv_firstBjet.M()<<endl;
              cout<<"ECON B2"<<endl;
              cout<<"ECON (E,Px,Py,Pz,M) "<<tlv_secondBjet.E()<<","<<tlv_secondBjet.Px()<<","<<tlv_secondBjet.Py()<<","<<tlv_secondBjet.Pz()<<","<<tlv_secondBjet.M()<<endl;
              cout<<"ECON MET"<<endl;
              cout<<"ECON (E,Px,Py,Pz,M) "<<ptmiss.E()<<","<<ptmiss.Px()<<","<<ptmiss.Py()<<","<<ptmiss.Pz()<<","<<ptmiss.M()<<endl;
              cout<<"ECON METCOV "<<endl;
              cout<<"ECON "<<metcov (0,0)<<"  "<<metcov (0,1)<<endl;// = theBigTree.MET_cov00->at (chosenTauPair) ;
              cout<<"ECON "<<metcov (1,0)<<"  "<<metcov (1,1)<<endl;// = theBigTree.MET_cov10->at (chosenTauPair) ;
              cout<<"ECON tau1, tau2, b1, b2"<<endl;
              cout<<"ECON ";
              tlv_firstLepton.Print();
              cout<<"ECON ";
              tlv_secondLepton.Print();
              cout<<"ECON ";
              tlv_firstBjet.Print();
              cout<<"ECON ";
              tlv_secondBjet.Print();
              wrongHHK=true;
              }
            if(!wrongHHK){
              HHKmass = kinFits.getMH () ;
              HHKChi2 = kinFits.getChi2 () ;
            }else{
              if(isOS)HHKmass = -333;
            }
          } // end if doing HHKinFit
      
          theSmallTree.m_HHKin_mass = HHKmass;//kinFits.getMH () ;
          theSmallTree.m_HHKin_chi2 = HHKChi2;//kinFits.getChi2 () ;

          theSmallTree.m_HH_deltaPhi = deltaPhi (tlv_bH.Phi (), tlv_tauH.Phi ()) ;
          theSmallTree.m_tauHMet_deltaPhi = deltaPhi (theBigTree.metphi, tlv_tauH.Phi ()) ;
          theSmallTree.m_bHMet_deltaPhi = deltaPhi (theBigTree.metphi, tlv_bH.Phi ()) ;

          theSmallTree.m_ditau_deltaPhi = deltaPhi (tlv_firstLepton.Phi (), tlv_secondLepton.Phi ()) ;
          theSmallTree.m_dib_deltaPhi = deltaPhi (tlv_firstBjet.Phi (), tlv_secondBjet.Phi ()) ;

          // loop over jets
          for (unsigned int iJet = 0 ; 
               (iJet < theBigTree.jets_px->size ()) && (theSmallTree.m_njets < maxNjetsSaved) ;
               ++iJet)
            {
              // PG filter jets at will
              if (theBigTree.jets_PUJetID->at (iJet) < PUjetID_minCut) continue ;
          
              // skip the H decay candiates
              if (int (iJet) == eventJets.first || 
                  int (iJet) == eventJets.second) continue ;

              TLorentzVector tlv_dummyJet (
                  theBigTree.jets_px->at (iJet),
                  theBigTree.jets_py->at (iJet),
                  theBigTree.jets_pz->at (iJet),
                  theBigTree.jets_e->at (iJet)
                ) ;

              // remove jets that overlap with the tau selected in the leg 1 and 2
              if (type1 == 2) {if (tlv_firstLepton.DeltaR(tlv_dummyJet) < 0.5) continue;}
              if (type2 == 2) {if (tlv_secondLepton.DeltaR(tlv_dummyJet) < 0.5) continue;}
      
              theSmallTree.m_jets_pt.push_back (tlv_dummyJet.Pt ()) ;
              theSmallTree.m_jets_eta.push_back (tlv_dummyJet.Eta ()) ;
              theSmallTree.m_jets_phi.push_back (tlv_dummyJet.Phi ()) ;
              theSmallTree.m_jets_e.push_back (theBigTree.jets_e->at (iJet)) ;
              theSmallTree.m_jets_btag.push_back (theBigTree.bCSVscore->at (iJet)) ;
              ++theSmallTree.m_njets ;
            } // loop over jets

        } // if there's two jets in the event, at least
        
      if (isMC) selectedEvents += theBigTree.aMCatNLOweight ; 
      else selectedEvents += 1 ;
      ++selectedNoWeightsEventsNum ;





      theSmallTree.Fill () ;
    } // loop over events

  cout << "1: " << totalEvents << endl ;
  cout << "2: " << selectedEvents << endl ;
  cout << "3: " << totalNoWeightsEventsNum << endl ;
  cout << "4: " << selectedNoWeightsEventsNum << endl ;
  for (unsigned int i = 0 ; i < counter.size () ; ++i)
    cout << "5 + i: " << counter.at (i) << endl ;

  if (totalEvents != 0) cout << "efficiency = " << selectedEvents / totalEvents << endl ;
  else                  cout << "NO events found\n" ;
  TH1F h_eff ("h_eff", "h_eff", 4 + counter.size (), 0., float (4 + counter.size ())) ;
  h_eff.SetBinContent (1, totalEvents) ;
  h_eff.SetBinContent (2, selectedEvents) ;
  h_eff.SetBinContent (3, totalNoWeightsEventsNum) ;
  h_eff.SetBinContent (4, selectedNoWeightsEventsNum) ;
  for (unsigned int i = 0 ; i < counter.size () ; ++i)
    h_eff.SetBinContent (5 + i, counter.at (i)) ;

  h_eff.Write () ;
  smallFile->Write () ;
  smallFile->Close () ;

  TFile *outFile = TFile::Open(outputFile,"UPDATE");
  TTree *treenew = (TTree*)outFile->Get("HTauTauTree");

  TMVA::Reader * reader = new TMVA::Reader () ;
  Float_t mvatautau,mvamutau;
  TBranch *mvaBranchmutau;
  TBranch *mvaBranchtautau;

  vector<float> address (TMVAvariables.size () + TMVAspectators.size () * TMVAspectatorsIn, 0.) ; 
  for (unsigned int iv = 0 ; iv < TMVAvariables.size () ; ++iv)
  {
    treenew->SetBranchAddress (TMVAvariables.at (iv).c_str (), &(address.at (iv))) ;
    reader->AddVariable (TMVAvariables.at (iv), &(address.at (iv))) ;
  }  

  for (unsigned int iv = 0 ; iv < TMVAspectators.size () && TMVAspectatorsIn ; ++iv)
  {
    int addressIndex = iv + TMVAvariables.size () ;
    treenew->SetBranchAddress (TMVAspectators.at (iv).c_str (), &(address.at (addressIndex))) ;
    reader->AddSpectator (TMVAspectators.at (iv), &(address.at (addressIndex))) ;
  }  

  //if (treenew->GetListOfBranches ()->FindObject (mvaName.c_str ())) {
  //  treenew->SetBranchAddress ("MuTauKine", &mvamutau, &mvaBranchmutau) ;
  //  treenew->SetBranchAddress ("TauTauKine", &mvatautau, &mvaBranchtautau) ;
  //}
  //else{   
    mvaBranchmutau = treenew->Branch ("MuTauKine", &mvamutau, "MuTauKine/F") ;
    mvaBranchtautau = treenew->Branch ("TauTauKine", &mvatautau, "TauTauKine/F") ;
  //}
  reader->BookMVA ("MuTauKine",  TMVAweightsMuTau.c_str ()) ;
  reader->BookMVA ("TauTauKine",  TMVAweightsTauTau.c_str ()) ;

  int nentries = treenew->GetEntries();
  for(int i=0;i<nentries;i++){
    treenew->GetEntry(i);

    mvamutau= reader->EvaluateMVA ("MuTauKine") ;  
    mvatautau= reader->EvaluateMVA ("TauTauKine") ;  
    mvaBranchtautau->Fill();
    mvaBranchmutau->Fill();
  }

  outFile->cd () ;
  h_eff.Write () ;
  treenew->Write ("", TObject::kOverwrite) ;

  delete reader;
  return 0 ;
  }



/* how to prepare bigTree:

 prepare the base class with ROOT
 ------------------------------------------------

   root /data_CMS/cms/govoni/test_submit_to_tier3/HiggsTauTauOutput_DY_-1Events_0Skipped_1436202480.82/output_9.root 
   TTree * t = (TTree *) _file0->Get ("HTauTauTree/HTauTauTree")
   t->MakeSelector ("bigTree")

 remove the following functions from the .h
 ------------------------------------------------
 
   virtual void    Init(TTree *tree);
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

 move the Init implementation in the class definition
 ------------------------------------------------
 
 change the following line
 ------------------------------------------------
 
    bigTree(TTree ETC....  --->
    bigTree (TChain * inputChain) : fChain (inputChain) { Init(fChain) ; }

 remove all the rest of the implementations
 ------------------------------------------------

 delete bigTree.C
 ------------------------------------------------

 remove the following lines to simplify things
 ------------------------------------------------

#endif

#ifdef bigTree_cxx

 remove the inheritance:
 ------------------------------------------------

class bigTree : public TSelector { --->
class bigTree { --->

 remove the following lines
 ------------------------------------------------

 #include <TSelector.h>

 remove the following lines from Init
 ------------------------------------------------

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

 remove the ClassDef call
 ------------------------------------------------

*/
