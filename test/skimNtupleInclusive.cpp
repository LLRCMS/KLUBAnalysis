#include <iostream>
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
#include "../../HHKinFit/interface/HHKinFitMaster.h"
#include "ConfigParser.h"

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


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char** argv)
{

  // read input file and cfg
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  if (argc < 6) 
    {
      cerr << "missing input parameters" << endl ;
      cerr << "usage: " << argv[0]
           << "inputFileName outputFileName crossSection isMC configFile" << endl ; 
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
  if (! (gConfigParser->init (config)))
    {
      cout << ">>> parseConfigFile::Could not open configuration file " << config << endl ;
      return -1 ;
    }

  bool beInclusive      = gConfigParser->readBoolOption ("selections::beInclusive") ;
  float PUjetID_minCut  = gConfigParser->readFloatOption ("parameters::PUjetIDminCut") ;
  bool  saveOS          = gConfigParser->readBoolOption ("parameters::saveOS") ;
  float lepCleaningCone = gConfigParser->readFloatOption ("parameters::lepCleaningCone") ;
  int   bChoiceFlag     = gConfigParser->readFloatOption ("parameters::bChoiceFlag") ;
  int PUReweight_MC     = gConfigParser->readFloatOption ("parameters::PUReweightMC") ; 
  int PUReweight_target = gConfigParser->readFloatOption ("parameters::PUReweighttarget") ; 

  // input and output setup
  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  OfflineProducerHelper oph ;

  TChain * bigChain = new TChain ("HTauTauTree/HTauTauTree") ;
  bigChain->Add (inputFile) ;
  bigTree theBigTree (bigChain) ;

  //Create a new file + a clone of old tree header. Do not copy events
  TFile * smallFile = new TFile (outputFile, "recreate") ;
  smallFile->cd () ;
  smallTree theSmallTree ("HTauTauTree") ;

  // these are needed for the HHKinFit
  vector<Int_t> hypo_mh1 ; //FIXME why is this an integer?!
  hypo_mh1.push_back (125) ;
  vector<Int_t> hypo_mh2 ;
  hypo_mh2.push_back (125) ;

  int eventsNumber = theBigTree.fChain->GetEntries () ;
  float totalEvents = 0. ;
  
  float selectedEvents = 0. ;

  int totalNoWeightsEventsNum = 0 ;
  int selectedNoWeightsEventsNum = 0 ;
 
  int selectionsNumber = 3 ;
  vector<float> counter (selectionsNumber + 1, 0.) ;

  PUReweight reweight ;
  // loop over events
  for (Long64_t iEvent = 0 ; iEvent < eventsNumber ; ++iEvent) 
    {
      if (iEvent % 10000 == 0)  cout << "reading event " << iEvent << endl ;
      int selID = 0 ;

      theSmallTree.clearVars () ;
      theBigTree.GetEntry (iEvent) ;
      
      if (isMC)
        {
          totalEvents += theBigTree.MC_weight ;
          counter.at (selID++) += theBigTree.MC_weight ;
        }
      else 
        {
          totalEvents += 1 ;
          counter.at (selID++) += 1 ;
        }
      ++totalNoWeightsEventsNum ;
      
      if (theBigTree.indexDau1->size () == 0) continue ;
      if (isMC) counter.at (selID++) += theBigTree.MC_weight ;
      else      counter.at (selID++) += 1 ;

      // by now take the OS pair with largest pT
      int chosenTauPair = -1 ;
      
      for (int iPair = 0 ; iPair < theBigTree.indexDau1->size () ; ++iPair)
        {
          if (theBigTree.isOSCand->at (iPair) != saveOS) continue ;
          if (saveOS && oph.pairPassBaseline (&theBigTree, iPair, "All"))
            {
              chosenTauPair = iPair ;
              break ; 
            }
          if (!saveOS && oph.pairPassBaseline (&theBigTree, iPair, "SScharge"))
            {
              chosenTauPair = iPair ;
              break ; 
            }
        }  
      if (chosenTauPair < 0) continue ;
      if (isMC) counter.at (selID++) += theBigTree.MC_weight ;
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
      for (int iJet = 0 ; iJet < theBigTree.jets_px->size () ; ++iJet)
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
      if (isMC) counter.at (selID++) += theBigTree.MC_weight ;
      else      counter.at (selID++) += 1 ;
      
      // fill the variables of interest
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

      theSmallTree.m_PUReweight = (isMC ? reweight.weight(PUReweight_MC,PUReweight_target,theBigTree.npv) : 1) ;      
      theSmallTree.m_MC_weight = (isMC ? theBigTree.MC_weight * XS : 1) ;
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

      int type1 = theBigTree.particleType->at (firstDaughterIndex) ;
      int type2 = theBigTree.particleType->at (secondDaughterIndex) ;
      theSmallTree.m_pairType = oph.getPairType (type1, type2) ;
      
      theSmallTree.m_tauH_pt = tlv_tauH.Pt () ;
      theSmallTree.m_tauH_eta = tlv_tauH.Eta () ;
      theSmallTree.m_tauH_phi = tlv_tauH.Phi () ;
      theSmallTree.m_tauH_e = tlv_tauH.E () ;
      theSmallTree.m_tauH_mass = tlv_tauH.M () ;

      theSmallTree.m_dau1_pt = tlv_firstLepton.Pt () ;
      theSmallTree.m_dau1_eta = tlv_firstLepton.Eta () ;
      theSmallTree.m_dau1_phi = tlv_firstLepton.Phi () ;
      theSmallTree.m_dau1_e = theBigTree.daughters_e->at (firstDaughterIndex) ;
      theSmallTree.m_dau1_flav = theBigTree.daughters_charge->at (firstDaughterIndex) * 
                                 (theBigTree.particleType->at (firstDaughterIndex) + 1) ;
                                 // 1 = from muons collection
                                 // 2 = from electrons collection
                                 // 3 = from tauH collection
                                 
      theSmallTree.m_dau2_pt = tlv_secondLepton.Pt () ;
      theSmallTree.m_dau2_eta = tlv_secondLepton.Eta () ;
      theSmallTree.m_dau2_phi = tlv_secondLepton.Phi () ;
      theSmallTree.m_dau2_e = theBigTree.daughters_e->at (secondDaughterIndex) ;
      theSmallTree.m_dau2_flav = theBigTree.daughters_charge->at (secondDaughterIndex) * 
                                 (theBigTree.particleType->at (secondDaughterIndex) + 1) ;

      // loop over leptons
      vector<pair<TLorentzVector, float> > dummyLeptCollection ;
      for (int iLep = 0 ; 
           (iLep < theBigTree.daughters_px->size ()) ;
           ++iLep)
        {
          // skip the H decay candiates
          if (iLep == firstDaughterIndex || iLep == secondDaughterIndex) continue ;

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
              if (!oph.eleBaseline (&theBigTree, iLep, 10., 2.5, 0.3, 1)) continue ;
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
      for (int iLep = 0 ; 
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
          float METpt = TMath::Sqrt (METx*METx + METy*METy) ;
    
          TLorentzVector ptmiss = TLorentzVector (METx, METy, 0., METpt) ;
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
        
          //intance of fitter master class
          HHKinFitMaster kinFits = HHKinFitMaster (&tlv_firstBjet, &tlv_secondBjet, 
                                                   &tlv_firstLepton, &tlv_secondLepton) ;
          kinFits.setAdvancedBalance (&ptmiss, metcov) ;
          //kinFits.setSimpleBalance (ptmiss.Pt (),10) ; //alternative which uses only the absolute value of ptmiss in the fit

          kinFits.addMh1Hypothesis (hypo_mh1) ;
          kinFits.addMh2Hypothesis (hypo_mh2) ;
          kinFits.doFullFit () ;

          theSmallTree.m_HHKin_mass = kinFits.getBestMHFullFit () ;
          theSmallTree.m_HHKin_chi2 = kinFits.getBestChi2FullFit () ;

          theSmallTree.m_HH_deltaPhi = deltaPhi (tlv_bH.Phi (), tlv_tauH.Phi ()) ;
          theSmallTree.m_tauHMet_deltaPhi = deltaPhi (theBigTree.metphi, tlv_tauH.Phi ()) ;
          theSmallTree.m_bHMet_deltaPhi = deltaPhi (theBigTree.metphi, tlv_bH.Phi ()) ;

          theSmallTree.m_ditau_deltaPhi = deltaPhi (tlv_firstLepton.Phi (), tlv_secondLepton.Phi ()) ;
          theSmallTree.m_dib_deltaPhi = deltaPhi (tlv_firstBjet.Phi (), tlv_secondBjet.Phi ()) ;

          // loop over jets
          for (int iJet = 0 ; 
               (iJet < theBigTree.jets_px->size ()) && (theSmallTree.m_njets < 3) ;
               ++iJet)
            {
              // PG filter jets at will
              if (theBigTree.jets_PUJetID->at (iJet) < PUjetID_minCut) continue ;
          
              // skip the H decay candiates
              if (iJet == eventJets.first || iJet == eventJets.second) continue ;

              TLorentzVector tlv_dummyJet (
                  theBigTree.jets_px->at (iJet),
                  theBigTree.jets_py->at (iJet),
                  theBigTree.jets_pz->at (iJet),
                  theBigTree.jets_e->at (iJet)
                ) ;
      
              theSmallTree.m_jets_pt.push_back (tlv_dummyJet.Pt ()) ;
              theSmallTree.m_jets_eta.push_back (tlv_dummyJet.Eta ()) ;
              theSmallTree.m_jets_phi.push_back (tlv_dummyJet.Phi ()) ;
              theSmallTree.m_jets_e.push_back (theBigTree.jets_e->at (iJet)) ;
              theSmallTree.m_jets_btag.push_back (theBigTree.bCSVscore->at (iJet)) ;
              ++theSmallTree.m_njets ;
            } // loop over jets

        } // if there's two jets in the event, at least
        
      if (isMC) selectedEvents += theBigTree.MC_weight ; 
      else selectedEvents += 1 ;
      ++selectedNoWeightsEventsNum ;
      theSmallTree.Fill () ;
    } // loop over events

  cout << "1: " << totalEvents << endl ;
  cout << "2: " << selectedEvents << endl ;
  cout << "3: " << totalNoWeightsEventsNum << endl ;
  cout << "4: " << selectedNoWeightsEventsNum << endl ;
  for (int i = 0 ; i < counter.size () ; ++i)
    cout << "5 + i: " << counter.at (i) << endl ;

  if (totalEvents != 0) cout << "efficiency = " << selectedEvents / totalEvents << endl ;
  else                  cout << "NO events found\n" ;
  TH1F h_eff ("h_eff", "h_eff", 4 + counter.size (), 0., float (4 + counter.size ())) ;
  h_eff.SetBinContent (1, totalEvents) ;
  h_eff.SetBinContent (2, selectedEvents) ;
  h_eff.SetBinContent (3, totalNoWeightsEventsNum) ;
  h_eff.SetBinContent (4, selectedNoWeightsEventsNum) ;
  for (int i = 0 ; i < counter.size () ; ++i)
    h_eff.SetBinContent (5 + i, counter.at (i)) ;

  h_eff.Write () ;
  smallFile->Write () ;
  smallFile->Close () ;
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
