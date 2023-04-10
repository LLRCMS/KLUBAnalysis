#include "OfflineProducerHelper.h"
#include <tuple>

using namespace std;

OfflineProducerHelper::OfflineProducerHelper(){
  const int nTriggers=19;
  TString tmptrigger[nTriggers]={
    "IsoMu17_eta2p1_LooseIsoPFTau20",
    "IsoMu17_eta2p1",
    "IsoMu17_eta2p1_MediumIsoPFTau40_Trk1_eta2p1_Reg",
    "IsoMu17_eta2p1_LooseIsoPFTau20_SingleL1",
    "IsoMu24_eta2p1_IterTrk01",
    "IsoMu24_eta2p1_IterTrk02",
    "IsoMu24_eta2p1_IterTrk02_LooseIsoPFTau20",
    "Ele22_eta2p1_WP85_Gsf_LooseIsoPFTau20",
    "Ele32_eta2p1_WP85_Gsf",
    "Ele32_eta2p1_WP85_Gsf_LooseIsoPFTau20",
    "LooseIsoPFTau50_Trk30_eta2p1_MET120",
    "IsoMu16_eta2p1_CaloMET30_LooseIsoPFTau50_Trk30_eta2p1",
    "IsoMu16_eta2p1_CaloMET30",
    "Mu16_eta2p1_CaloMET30",
    "LooseIsoPFTau50_Trk30_eta2p1",
    "DoubleIsoMu17_eta2p1",
    "Mu17_TrkIsoVVL_Mu8_TrkIsoVVL",
    "Ele27_eta2p1_WP85_Gsf_LooseIsoPFTau20",
    "Ele27_eta2p1_WP85_Gsf"
  };
  for(int i=0;i<nTriggers;i++){
    tmptrigger[i].Prepend("HLT_");
    tmptrigger[i].Append("_v1");
    triggerlist.push_back(tmptrigger[i]);
  }
  tauidlist.push_back("");

  SetEleMVAIDCuts();
}

OfflineProducerHelper::OfflineProducerHelper(TH1F* hCounter, TH1F* hTauIDs){

  for(int itr=1;itr<=hCounter->GetNbinsX();itr++){
    TString binlabel = hCounter->GetXaxis()->GetBinLabel(itr);
    if(binlabel.BeginsWith("HLT"))triggerlist.push_back(hCounter->GetXaxis()->GetBinLabel(itr));
  }

  for(int itr=1;itr<=hTauIDs->GetNbinsX();itr++){
    TString binlabel = hTauIDs->GetXaxis()->GetBinLabel(itr);
    tauidlist.push_back(hTauIDs->GetXaxis()->GetBinLabel(itr));
  }

  SetEleMVAIDCuts();
}


OfflineProducerHelper::OfflineProducerHelper(TH1F* hCounter){

  for(int itr=1;itr<=hCounter->GetNbinsX();itr++){
    TString binlabel = hCounter->GetXaxis()->GetBinLabel(itr);
    if(binlabel.BeginsWith("HLT"))triggerlist.push_back(hCounter->GetXaxis()->GetBinLabel(itr));
  }
  tauidlist.push_back("");

  SetEleMVAIDCuts();
}

void OfflineProducerHelper::SetEleMVAIDCuts()
{
  // MVA ele ID from here:
  //  https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentificationRun2#Non_triggering_electron_MVA
  // 80%
  m_MVAEleIDCuts[0][0][0] =  0.287435 ; // barrel (eta<0.8) pt 5-10 GeV
  m_MVAEleIDCuts[0][0][1] =  0.221846 ; // barrel (eta>0.8) pt 5-10 GeV
  m_MVAEleIDCuts[0][0][2] = -0.303263 ; // endcap pt 5-10 GeV
  m_MVAEleIDCuts[0][1][0] =  0.967083 ; // barrel (eta<0.8) pt above 10 GeV
  m_MVAEleIDCuts[0][1][1] =  0.929117 ; // barrel (eta>0.8) pt above 10 GeV
  m_MVAEleIDCuts[0][1][2] =  0.726311 ; // endcap pt above 10 GeV

  // 90%
  m_MVAEleIDCuts[1][0][0] = -0.083313 ; // barrel (eta<0.8) pt 5-10 GeV
  m_MVAEleIDCuts[1][0][1] = -0.235222 ; // barrel (eta>0.8) pt 5-10 GeV
  m_MVAEleIDCuts[1][0][2] = -0.67099  ; // endcap pt 5-10 GeV
  m_MVAEleIDCuts[1][1][0] =  0.913286 ; // barrel (eta<0.8) pt above 10 GeV
  m_MVAEleIDCuts[1][1][1] =  0.805013 ; // barrel (eta>0.8) pt above 10 GeV
  m_MVAEleIDCuts[1][1][2] =  0.358969 ; // endcap pt above 10 GeV
}

int OfflineProducerHelper::FindTriggerNumber(TString triggername){
  for(int it=0;it< (int) triggerlist.size();it++){
    if(triggerlist.at(it).CompareTo(triggername.Data())==0)return it;
    else {
      TString newName=triggername.Data();
      newName.Prepend("HLT_");
      newName.Append("_v1");
      if(triggerlist.at(it).CompareTo(newName.Data())==0)return it;
    }
  }
  return -1;
}

bool OfflineProducerHelper::IsTriggerFired(int triggerbit, int triggernumber){
  if(triggernumber>=0 && triggernumber<(int)triggerlist.size()) return triggerbit & (1 << triggernumber);
  return false;
}

int OfflineProducerHelper::printFiredPaths(int triggerbit){
  int nFired = 0;
  for(int it=0;it<(int)triggerlist.size();it++){
    if(IsTriggerFired(triggerbit,it)) {
      printf("%s\n",triggerlist.at(it).Data());
      nFired++;
    }
  }
  return nFired;
}

bool OfflineProducerHelper::checkBit (long word, long bitpos)
{
  bool res = word & ((long)1 << bitpos);
  return res;
}


int OfflineProducerHelper::getTAUidNumber(TString tauIDname){
  int ntau = (int)tauidlist.size();
  for(int i=0;i<ntau;i++)
    if(tauidlist.at(i)==tauIDname.Data()) return i;
  return -1;
}


int OfflineProducerHelper::getPairType (int type1, int type2)
{

  int nmu = 0;
  int nele = 0;
  int ntau = 0;

  if (isMuon (type1) )     nmu++;
  if (isElectron (type1) ) nele++;
  if (isTau (type1) )      ntau++;

  if (isMuon (type2) )     nmu++;
  if (isElectron (type2) ) nele++;
  if (isTau (type2) )      ntau++;


  if (nmu == 1 && nele == 0 && ntau == 1) return (int) pairType::MuHad;
  if (nmu == 0 && nele == 1 && ntau == 1) return (int) pairType::EHad;
  if (nmu == 0 && nele == 0 && ntau == 2) return (int) pairType::HadHad;
  if (nmu == 2 && nele == 0 && ntau == 0) return (int) pairType::MuMu;
  if (nmu == 0 && nele == 2 && ntau == 0) return (int) pairType::EE;
  if (nmu == 1 && nele == 1 && ntau == 0) return (int) pairType::EMu;

  return -1;

}

bool OfflineProducerHelper::pairPassBaseline (bigTree* tree, int iPair, TString whatApply, bool debug)
{
  int dau1index = tree->indexDau1->at(iPair);
  int dau2index = tree->indexDau2->at(iPair);
  int type1 = tree->particleType->at(dau1index);
  int type2 = tree->particleType->at(dau2index);
  int pairType = getPairType (type1, type2);

  if (debug) cout << ".. checking baseline of pair " << iPair << " idx=(" << dau1index << "," << dau2index << ")" << endl;

  float dR = DeltaRDau(tree, dau1index, dau2index);
  bool drMin = (dR > 0.4);
  if (!drMin && debug)
    cout << "failed dR min as dR=" << dR << endl;

  bool isOS = tree->isOSCand->at(iPair);
  if (whatApply.Contains("OScharge") && !isOS) {
    if (debug) cout<<"check baseline: OSCharge failed"<<endl;
    return false; // do not even check the rest if requiring the charge
  }
  if (whatApply.Contains("SScharge") && isOS) {
    if (debug) cout<<"check baseline: SSCharge failed"<<endl;
    return false; // for the same sign selection at the moment full selection over SS pairs
  }

  // pairs are always ordered as: e mu | e tau | mu tau  (e < mu < tau)
  // if same type of particle, highest pt one is the first
  bool leg1=false;
  bool leg2=false;
  if (pairType == MuHad)
  {
    float tauIso = whatApply.Contains("TauRlxIzo") ? 7.0 : 3.0 ;
    leg1 = muBaseline  (tree, dau1index, 20., 2.3, 0.15, MuTight, whatApply, debug);
    leg2 = tauBaseline (tree, dau2index, 20., 2.3, aeleVVLoose, amuTight, tauIso, whatApply, debug);
  }

  if (pairType == EHad)
  {
    float tauIso = whatApply.Contains("TauRlxIzo") ? 7.0 : 3.0 ;
    leg1 = eleBaseline (tree, dau1index, 20., 2.3, 0.1, EMVATight, whatApply, debug);
    //leg2 = tauBaseline (tree, dau2index, 20., 2.3, aeleTight, amuTight, tauIso, whatApply, debug); // Switched from 'aeleTight' to 'aeleVLoose' in January 2021
    leg2 = tauBaseline (tree, dau2index, 20., 2.3, aeleVVLoose, amuTight, tauIso, whatApply, debug);  // following HTT conveners suggestion
  }

  // ordered by pT and not by most isolated, but baseline asked in sync is the same...
  if (pairType == HadHad)
  {
    float tauIso = whatApply.Contains("TauRlxIzo") ? 7.0 : 2.0 ;
    leg1 = tauBaseline (tree, dau1index, 20., 2.3, aeleVVLoose, amuTight, tauIso, whatApply, debug);
    leg2 = tauBaseline (tree, dau2index, 20., 2.3, aeleVVLoose, amuTight, tauIso, whatApply, debug);
  }

  if (pairType == EMu)
  {
    leg1 = eleBaseline (tree, dau1index, 13., 2.3, 0.15, EMVAMedium, whatApply, debug);
    leg2 = muBaseline  (tree, dau2index, 13., 2.3, 0.15, MuTight, whatApply, debug);
  }

  // e e, mu mu are still preliminary (not from baseline)
  if (pairType == EE)
  {
    leg1 = eleBaseline (tree, dau1index, 25., 2.3, 0.15, EMVAMedium, whatApply, debug);
    leg2 = eleBaseline (tree, dau2index, 25., 2.3, 0.15, EMVAMedium, whatApply, debug);
  }

  if (pairType == MuMu)
  {
    leg1 = muBaseline (tree, dau1index, 10., 2.3, 0.15, MuTight, whatApply, debug);
    leg2 = muBaseline (tree, dau2index, 10., 2.3, 0.15, MuTight, whatApply, debug);
    bool leg1ER = muBaseline (tree, dau1index, 10., 2.3, 0.15, MuTight, whatApply, debug);
    bool leg2ER = muBaseline (tree, dau2index, 10., 2.3, 0.15, MuTight, whatApply, debug);

    return ( ((leg1ER && leg2) || (leg2ER && leg1)) && drMin );
  }

  bool result = (leg1 && leg2);
  if(!leg1 && debug){
    cout<<"check baseline: leg1 failed"<<endl;
  }
  if(!leg2 && debug){
    cout<<"check baseline: leg2 failed"<<endl;
  }

  if (leg1&&leg2&&debug)
    cout << "check baseline: leg1 AND leg2 ok" << endl;

  result = (result && drMin);

  return result;
}


bool
OfflineProducerHelper::eleBaseline (bigTree* tree, int iDau,
                                    float ptMin, float relIso, int MVAIDflag,
                                    TString whatApply, bool debug)
{
  float px = tree->daughters_px->at(iDau);
  float py = tree->daughters_py->at(iDau);
  float pz = tree->daughters_pz->at(iDau);
  float e =  tree->daughters_e->at(iDau);

  TLorentzVector p4 (px, py, pz, e);

  // bypasser(s) and taker according to the string content
  bool byp_vertexS = false;
  bool byp_idS  = false;
  bool byp_isoS = false;
  bool byp_ptS  = false;
  bool byp_etaS = false;

  // whatApply: use "All", "Iso", "LepID", pTMin", "etaMax", "againstEle", "againstMu", "Vertex", "SScharge"; separate various arguments with a semicolon
  if (!whatApply.Contains("All") &&
      !whatApply.Contains("SScharge") &&
      !whatApply.Contains("OScharge"))
  {
    byp_vertexS = byp_idS = byp_isoS = byp_ptS = byp_etaS = true;
    // set selections
    if (whatApply.Contains("Vertex")) byp_vertexS = false;
    if (whatApply.Contains("Iso"))    byp_isoS = false;
    if (whatApply.Contains("LepID"))  byp_idS = false;
    if (whatApply.Contains("pTMin"))  byp_ptS = false;
    if (whatApply.Contains("etaMax")) byp_etaS = false;
  }

  bool vertexS = (fabs(tree->dxy->at(iDau)) < 0.045 && fabs(tree->dz->at(iDau)) < 0.2) || byp_vertexS;
  bool ptS = (p4.Pt() > ptMin) || byp_ptS;
  bool etaS = (fabs(p4.Eta()) < 2.5) || byp_etaS;
  bool idS = false;
  if (MVAIDflag == 0) // Tight = 80%
  {
    idS = tree->daughters_iseleWP80->at(iDau) || byp_idS ;
  }
  else if (MVAIDflag == 1) // Medium = 90 %
  {
    idS = tree->daughters_iseleWP90->at(iDau) || byp_idS ;
  }
  else // Loose WP
  {
    idS = tree->daughters_iseleWPLoose->at(iDau) || byp_idS ;
  }

  bool totalS = (vertexS && idS && ptS && etaS);

  if (debug)
  {
    cout << "@ ele baseline" << endl;
    cout << " idS     "  << idS     << " skipped? " << byp_idS << endl;
    cout << " vertexS "  << vertexS << " skipped? " << byp_vertexS << endl;
    cout << " ptS     "  << ptS     << " skipped? " << byp_ptS << endl;
    cout << " etaS    "  << etaS    << " skipped? " << byp_etaS << endl;
  }

  return totalS;
}

bool
OfflineProducerHelper::eleBaseline (bigTree* tree, int iDau,
                                    float ptMin, float etaMax, float relIso, int MVAIDflag,
                                    TString whatApply, bool debug)
{
  float px = tree->daughters_px->at(iDau);
  float py = tree->daughters_py->at(iDau);
  float pz = tree->daughters_pz->at(iDau);
  float e =  tree->daughters_e->at(iDau);

  TLorentzVector p4 (px, py, pz, e);

  // bypasser(s) and taker according to the string content
  bool byp_vertexS  = false;
  bool byp_idS      = false;
  bool byp_ptS      = false;
  bool byp_etaS     = false;
  bool byp_isoS     = false;
  bool byp_noISOidS = false;

  // whatApply: use "All", "Iso", "LepID", pTMin", "etaMax", "againstEle", "againstMu", "Vertex", "SScharge"; separate various arguments with a semicolon
  if (!whatApply.Contains("All") and !whatApply.Contains("SScharge") and !whatApply.Contains("OScharge"))
	{
	  byp_vertexS = byp_idS = byp_isoS = byp_ptS = byp_etaS = byp_noISOidS = true;  // For signal elect and first lepton veto: first bypass all...
	  // set selections
	  if (whatApply.Contains("Vertex")) byp_vertexS = false;                        // ...then use only these four selections
	  if (whatApply.Contains("LepID"))  byp_idS = false;
	  if (whatApply.Contains("pTMin"))  byp_ptS = false;
	  if (whatApply.Contains("etaMax")) byp_etaS = false;
	  if (whatApply.Contains("thirdLep"))                                           // For third lepton veto use noISO-MVA and relIso
		{                                                                             // instead of ISO-MVA
		  byp_isoS     = false; // use pfRelIso < 0.3
		  byp_noISOidS = false; // use nonIsoMVA (mvaEleID-Fall17-noIso-V2-wp90)
		  byp_idS      = true;  // bypass IsoMVA (mvaEleID-Fall17-Iso-V2-wp90)
		}
	}

  bool vertexS = (fabs(tree->dxy->at(iDau)) < 0.045 && fabs(tree->dz->at(iDau)) < 0.2) || byp_vertexS;
  bool ptS = (p4.Pt() > ptMin) || byp_ptS;
  bool etaS = (fabs(p4.Eta()) < etaMax) || byp_etaS;
  bool isoS = (tree->combreliso->at(iDau) < relIso) || byp_isoS;
  bool idS = false;
  bool nISOidS = false;
  if (MVAIDflag == 0) // Tight = 80%
  {
    idS = tree->daughters_iseleWP80->at(iDau) || byp_idS ;
    nISOidS = tree->daughters_iseleNoIsoWP80->at(iDau) || byp_noISOidS;
  }
  else if (MVAIDflag == 1) // Medium = 90 %
  {
    idS = tree->daughters_iseleWP90->at(iDau) || byp_idS ;
    nISOidS = tree->daughters_iseleNoIsoWP90->at(iDau) || byp_noISOidS;
  }
  else // Loose WP
  {
    idS = tree->daughters_iseleWPLoose->at(iDau) || byp_idS ;
    nISOidS = tree->daughters_iseleNoIsoWPLoose->at(iDau) || byp_noISOidS;
  }

  bool totalS = (vertexS && idS && ptS && etaS && isoS && nISOidS);

  if (debug)
  {
    cout << "@ ele baseline" << endl;
    cout << " idS     "  << idS     << " skipped? " << byp_idS << endl;
    cout << " nISOidS "  << nISOidS << " skipped? " << byp_noISOidS << endl;
    cout << " isoS    "  << isoS    << " skipped? " << byp_isoS << endl;
    cout << " vertexS "  << vertexS << " skipped? " << byp_vertexS << endl;
    cout << " ptS     "  << ptS     << " skipped? " << byp_ptS << endl;
    cout << " etaS    "  << etaS    << " skipped? " << byp_etaS << endl;
  }

  return totalS;
}

bool OfflineProducerHelper::muBaseline (
  bigTree* tree, int iDau, float ptMin,
  float etaMax, float relIso, int muIDWP, TString whatApply, bool debug)
{
  float px = tree->daughters_px->at(iDau);
  float py = tree->daughters_py->at(iDau);
  float pz = tree->daughters_pz->at(iDau);
  float e =  tree->daughters_e->at(iDau);

  TLorentzVector p4 (px, py, pz, e);
  int discr = tree->daughters_muonID->at(iDau);

  // bypasser(s) according to the string content
  bool byp_vertexS = false;
  bool byp_idS  = false;
  bool byp_isoS = false;
  bool byp_ptS  = false;
  bool byp_etaS = false;

  // whatApply: use "All", "Iso", "LepID", pTMin", "etaMax", "againstEle",
  // "againstMu", "Vertex"; separate various arguments with a semicolon
  if (!whatApply.Contains("All") &&
      !whatApply.Contains("SScharge") &&
      !whatApply.Contains("OScharge"))
  {
    byp_vertexS = byp_idS = byp_isoS = byp_ptS = byp_etaS = true;
    // set selections
    if (whatApply.Contains("Vertex")) byp_vertexS = false;
    if (whatApply.Contains("Iso"))    byp_isoS = false;
    if (whatApply.Contains("LepID"))  byp_idS = false;
    if (whatApply.Contains("pTMin"))  byp_ptS = false;
    if (whatApply.Contains("etaMax")) byp_etaS = false;
  }

  if (muIDWP < 0 || muIDWP > 3)
  {
    cout << " ** OfflineProducerHelper::muBaseline: muIDWP must be between 0 and 3 --> using 0" << endl;
    muIDWP = 0;
  }

  bool vertexS = (fabs(tree->dxy->at(iDau)) < 0.045 && fabs(tree->dz->at(iDau)) < 0.2) || byp_vertexS;
  bool idS = checkBit (discr, muIDWP) || byp_idS; // bit 0 is LOOSE id, bit 2 is MEDIUM mu id, bit 3 is TIGHT mu id
  bool isoS = (tree->combreliso->at(iDau) < relIso) || byp_isoS;
  if (whatApply.Contains ("InvertIzo")) isoS = !isoS ;
  bool ptS = (p4.Pt() > ptMin) || byp_ptS;
  bool etaS = (fabs(p4.Eta()) < etaMax) || byp_etaS;

  bool totalS = (vertexS && idS && isoS && ptS && etaS);
  if (debug)
  {
    cout << "@ mu baseline" << endl;
	cout << " idS	   " << idS	<< " skipped? " << byp_idS << endl;
	cout << " vertexS "	 << vertexS	<< " skipped? " << byp_vertexS << endl;
	cout << " isoS	   " << isoS	<< " skipped? " << byp_isoS << endl;
	cout << " ptS	  "	 << ptS		<< " skipped? " << byp_ptS << endl;
	cout << " etaS	  "	 << etaS	<< " skipped? " << byp_etaS << endl;
  }

  return totalS;
}

// againstEleWP: 0 = VLoose  , 1 = Loose  , 2 = Medium, 3 = Tight, 4 = VTight
// againstMuWP:  0 = Loose   , 1 = Tight
// deepTauVsEle: 0 = VVVLoose, 1 = VVLoose, 2 = VLoose, 3 = Loose, 4 = Medium, 5 = Tight, 6 = VTight, 7 = VVTight
// deepTauVsMu:  0 = VLoose  , 1 = Loose  , 2 = Medium, 3 = Tight

bool OfflineProducerHelper::tauBaseline (bigTree* tree, int iDau, float ptMin,
					 float etaMax, int againstEleWP, int againstMuWP, float isoRaw3Hits,
					 TString whatApply, bool debug)
{
  if (tree->decayMode->at(iDau) == 5 || tree->decayMode->at(iDau) == 6)
  {
    if (debug) std::cout<< "Rejecting decay mode: "<< tree->decayMode->at(iDau) << std::endl;
    return false;
  }

  float px = tree->daughters_px->at(iDau);
  float py = tree->daughters_py->at(iDau);
  float pz = tree->daughters_pz->at(iDau);
  float e =  tree->daughters_e->at(iDau);

  TLorentzVector p4 (px, py, pz, e);

  // bypasser(s) according to the string content
  bool byp_vertexS = false;
  bool byp_dmfS  = false;
  bool byp_agEleS = false;
  bool byp_agMuS = false;
  bool byp_isoS = false;
  bool byp_ptS  = false;
  bool byp_etaS = false;

  // whatApply: use "All", "Iso", "LepID", pTMin", "etaMax", "againstEle",
  // "againstMu", "Vertex"; separate various arguments with a semicolon
  if (!whatApply.Contains("All") &&
      !whatApply.Contains("SScharge") &&
      !whatApply.Contains("OScharge"))
  {
    byp_vertexS = byp_dmfS = byp_agEleS = byp_agMuS = byp_isoS = byp_ptS = byp_etaS = true;
    // set selections
    if (whatApply.Contains("Vertex")) byp_vertexS = false;
    if (whatApply.Contains("Iso"))    byp_isoS = false;
    if (whatApply.Contains("LepID"))  byp_dmfS = false;
    if (whatApply.Contains("againstEle"))  byp_agEleS = false;
    if (whatApply.Contains("againstMu"))   byp_agMuS = false;
    if (whatApply.Contains("pTMin"))  byp_ptS = false;
    if (whatApply.Contains("etaMax")) byp_etaS = false;
  }

  // MVA2017v2
  //if (againstEleWP < 0 || againstEleWP > 4) {
  //    cout << " ** OfflineProducerHelper::tauBaseline: againstEleWP must be between 0 and 4 --> using 0" << endl;
  //    againstEleWP = 0;
  //}

  //if (againstMuWP < 0 || againstMuWP > 1) {
  //    cout << " ** OfflineProducerHelper::tauBaseline: againstMuWP must be between 0 and 1 --> using 0" << endl;
  //    againstMuWP = 0;
  //}

  // DeepTauV2p1
  if (againstEleWP < 0 || againstEleWP > 7) {
    cout << " ** OfflineProducerHelper::tauBaseline: againstEleWP must be between 0 and 7 --> using 0" << endl;
    againstEleWP = 0;
  }

  if (againstMuWP < 0 || againstMuWP > 3) {
    cout << " ** OfflineProducerHelper::tauBaseline: againstMuWP must be between 0 and 3 --> using 0" << endl;
    againstMuWP = 0;
  }

  int agEleVal = 0;
  int agMuVal = 0;

  // MVA2017v2
  // ag ele:
  //if (againstEleWP == 0)      agEleVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("againstElectronVLooseMVA6"));
  //else if (againstEleWP == 1) agEleVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("againstElectronLooseMVA6"));
  //else if (againstEleWP == 2) agEleVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("againstElectronMediumMVA6"));
  //else if (againstEleWP == 3) agEleVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("againstElectronTightMVA6"));
  //else if (againstEleWP == 4) agEleVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("againstElectronVTightMVA6"));

  // ag mu:
  //if (againstMuWP == 0)      agMuVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("againstMuonLoose3"));
  //else if (againstMuWP == 1) agMuVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("againstMuonTight3"));

  // DeepTauV2p1
  // ag ele:
  if (againstEleWP == 0)      agEleVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("byVVVLooseDeepTau2017v2p1VSe"));
  else if (againstEleWP == 1) agEleVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("byVVLooseDeepTau2017v2p1VSe"));
  else if (againstEleWP == 2) agEleVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("byVLooseDeepTau2017v2p1VSe"));
  else if (againstEleWP == 3) agEleVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("byLooseDeepTau2017v2p1VSe"));
  else if (againstEleWP == 4) agEleVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("byMediumDeepTau2017v2p1VSe"));
  else if (againstEleWP == 5) agEleVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("byTightDeepTau2017v2p1VSe"));
  else if (againstEleWP == 6) agEleVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("byVTightDeepTau2017v2p1VSe"));
  else if (againstEleWP == 7) agEleVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("byVVTightDeepTau2017v2p1VSe"));

  // ag mu:
  if (againstMuWP == 0)      agMuVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("byVLooseDeepTau2017v2p1VSmu"));
  else if (againstMuWP == 1) agMuVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("byLooseDeepTau2017v2p1VSmu"));
  else if (againstMuWP == 2) agMuVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("byMediumDeepTau2017v2p1VSmu"));
  else if (againstMuWP == 3) agMuVal = checkBit(tree->tauID->at(iDau),getTAUidNumber("byTightDeepTau2017v2p1VSmu"));

  //bool dmfS = (tree->daughters_decayModeFindingOldDMs->at(iDau) == 1) || byp_dmfS;  // MVA2017v2
  bool dmfS = (tree->daughters_decayModeFindingNewDMs->at(iDau) == 1) || byp_dmfS;    // DeepTauV2p1
  bool vertexS = (fabs(tree->dz->at(iDau)) < 0.2) || byp_vertexS;
  bool agEleS = (agEleVal == 1) || byp_agEleS;
  bool agMuS  = (agMuVal == 1) || byp_agMuS;

  //bool isoS = (tree->daughters_byVVLooseIsolationMVArun2017v2DBoldDMwLT2017->at(iDau) > 0.5) || byp_isoS;  // MVA2017v2
  bool isoS =  checkBit(tree->tauID->at(iDau),getTAUidNumber("byVVVLooseDeepTau2017v2p1VSjet")) || byp_isoS; // DeepTauV2p1
  if (whatApply.Contains ("InvertIzo")) isoS = !isoS ;

  bool ptS = (p4.Pt() > ptMin) || byp_ptS;
  bool etaS = (fabs(p4.Eta()) < etaMax) || byp_etaS;

  bool totalS = (dmfS && vertexS && agEleS && agMuS && isoS && ptS && etaS);
  if (debug)
  {
    cout << "@ tau baseline" << endl;
    cout << " dmfS    "  << dmfS    << " skipped? " << byp_dmfS << endl;
    cout << " vertexS "  << vertexS << " skipped? " << byp_vertexS << endl;
    cout << " agEleS  "  << agEleS  << " skipped? " << byp_agEleS << endl;
    cout << " agMuS   "  << agMuS   << " skipped? " << byp_agMuS << endl;
    cout << " isoS    "  << isoS    << " skipped? " << byp_isoS << endl;
    cout << " ptS     "  << ptS     << " skipped? " << byp_ptS << endl;
    cout << " etaS    "  << etaS    << " skipped? " << byp_etaS << endl;
  }
  return totalS;
}

// branch isBDT has been updated, this will soon be unnecessary (and eventually obsolete)
bool OfflineProducerHelper::tightEleMVAID (float BDT, float fSCeta)
{
  // POG_MVA_ID_Run2_NonTrig_Tight PHYS14
  if (fSCeta < 0) fSCeta *= -1;
  bool isBDT = false;
  if (fSCeta <0.8) isBDT = (BDT>0.73);
  else if (fSCeta < 1.479) isBDT = (BDT>0.57);
  else isBDT = (BDT >0.05);

  return isBDT;
}

int OfflineProducerHelper::getMothPairType (bigTree* tree, int iMoth)
{
  if (iMoth < 0 || iMoth >= (int)tree->indexDau1->size())
  {
    cout << "warning ** getMothPairType: iMoth out of range" << endl;
    return -1;
  }

  int iDau1 = tree->indexDau1->at(iMoth);
  int iDau2 = tree->indexDau2->at(iMoth);
  int type1 = tree->particleType->at(iDau1);
  int type2 = tree->particleType->at(iDau2);

  return (getPairType(type1, type2));
}


bool OfflineProducerHelper::EleMVAID (float BDT, float eta, float pT, int strength)
{
  // 0 = tight (80%), 1 = loose (90%)
  if (pT < 5) return false ;
  int etaBin = 0 ;
  if (fabs (eta > 0.8)) etaBin = 1 ;
  if (fabs (eta > 1.479)) etaBin = 2 ;
  return BDT > m_MVAEleIDCuts[strength][pT>10][etaBin] ;
}


TLorentzVector OfflineProducerHelper::buildDauP4 (bigTree* tree, int iDau)
{
  float px = tree->daughters_px->at(iDau);
  float py = tree->daughters_py->at(iDau);
  float pz = tree->daughters_pz->at(iDau);
  float e =  tree->daughters_e->at(iDau);

  TLorentzVector p4 (px, py, pz, e);
  return p4;
}

TLorentzVector OfflineProducerHelper::buildMothP4 (bigTree* tree, int iMoth)
{
  float px = tree->mothers_px->at(iMoth);
  float py = tree->mothers_py->at(iMoth);
  float pz = tree->mothers_pz->at(iMoth);
  float e =  tree->mothers_e->at(iMoth);

  TLorentzVector p4 (px, py, pz, e);
  return p4;
}

TLorentzVector OfflineProducerHelper::buildGenP4 (bigTree* tree, int iGen)
{
  float px = tree->genpart_px->at(iGen);
  float py = tree->genpart_py->at(iGen);
  float pz = tree->genpart_pz->at(iGen);
  float e =  tree->genpart_e->at(iGen);

  TLorentzVector p4 (px, py, pz, e);
  return p4;
}

int OfflineProducerHelper::MCHiggsTauTauDecayMode (bigTree* tree)
{
  int decay = -1; // good decays go from 0 to 7, see enum
  for (int i = 0; i < (int)tree->genpart_HZDecayMode->size(); i++)
  {
    int val = tree->genpart_HZDecayMode->at(i);
    if (val >= 0 && val <= 7)
    {
      decay = val;
      break; // I don;t expect more than 1 H to tau tau per event
    }
  }
  return decay;
}


bool OfflineProducerHelper::getBestJets (bigTree* tree, int& jet1, int& jet2, int strategy)
{
  jet1 = jet2 = -1;
  switch (strategy)
  {
  case(0): // two with highest b score
  {
    int njets = tree->bCSVscore->size();
    if (njets < 2) return false;
    std::vector<std::pair<float, int>> scores;
    for (int i = 0; i < njets; i++) scores.push_back (std::make_pair(tree->bCSVscore->at(i), i));
    std::sort (scores.begin(), scores.end()); // are sorted according to the first index, i.e. the CSV score
    jet1 = (scores.at(njets-1)).second; //leading
    jet2 = (scores.at(njets-2)).second; // subleading
    return true;
    break;
  }

  default:
  {
    std::cout << "B jet selection strategy" << strategy << " not implemented" << std::endl;
    return false;
  }
  }
}


int OfflineProducerHelper::getBestPair (bigTree* tree, std::vector<int>& pairIdxs, TString strategy)
{
  int strat = -1;
  if (strategy == "OSMaxPt") strat = 0;
  if (strat == -1)
  {
    cout << "Best pair strategy not defined: " << strategy << endl;
    return -1;
  }

  // empty vector --> no pair
  if (pairIdxs.size() == 0) return -1;

  // ======= STRATEGIES ======
  // 0: favor OS, then max scalar sum PT
  if (strat == 0)
  {
    // pairs are already ordered by OS , SS, and inside each category by decreasing scalar sum pT
    // so it is enough to order indexes
    return ( *std::min_element (pairIdxs.begin(), pairIdxs.end()) );
  }

  return -1; // redudant, for safety
}


int OfflineProducerHelper::getBestPair (bigTree* tree, TString strategy)
{
  // FIXME: TO IMPLEMENT YET
  cout << "GET BEST PAIR FUNCTION TO BE IMPLEMENTED" << endl;
  return -1;
}

int OfflineProducerHelper::getPairByIndexes (bigTree* tree, int dau1, int dau2)
{
  int pair = -1;
  for (int iPair = 0; iPair < (int)tree->indexDau1->size(); iPair++)
  {
    int ind1 = tree->indexDau1->at(iPair);
    int ind2 = tree->indexDau2->at(iPair);
    if (ind1 == dau1 && ind2 == dau2) pair = iPair;
    else if (ind2 == dau1 && ind1 == dau2) pair = iPair;

    if (pair != -1) break; // don't continue search, pairs are unique
  }
  return pair;
}

int OfflineProducerHelper::getBestPairHTauTau (bigTree* tree, TString whatApply, bool debug)
{
  // prepare a tuple with all the needed sorting info
  vector<tauPair_t> vPairs;
  for (unsigned int ipair = 0; ipair < tree->indexDau1->size(); ++ipair)
  {
    int t_firstDaughterIndex  = tree->indexDau1->at (ipair) ;
    int t_secondDaughterIndex = tree->indexDau2->at (ipair) ;
    int t_type1 = tree->particleType->at (t_firstDaughterIndex) ;
    int t_type2 = tree->particleType->at (t_secondDaughterIndex) ;
    if ( getPairType (t_type1, t_type2) != 2 ) continue ; // tau tau only

    float px1 = tree->daughters_px->at(t_firstDaughterIndex);
    float py1 = tree->daughters_py->at(t_firstDaughterIndex);
    float pz1 = tree->daughters_pz->at(t_firstDaughterIndex);
    float e1 =  tree->daughters_e->at(t_firstDaughterIndex);
    TLorentzVector p4_1 (px1, py1, pz1, e1);

    float px2 = tree->daughters_px->at(t_secondDaughterIndex);
    float py2 = tree->daughters_py->at(t_secondDaughterIndex);
    float pz2 = tree->daughters_pz->at(t_secondDaughterIndex);
    float e2 =  tree->daughters_e->at(t_secondDaughterIndex);
    TLorentzVector p4_2 (px2, py2, pz2, e2);

    //float iso1 = tree->daughters_byIsolationMVArun2017v2DBoldDMwLTraw2017->at(t_firstDaughterIndex);  // MVA2017v2
    //float iso2 = tree->daughters_byIsolationMVArun2017v2DBoldDMwLTraw2017->at(t_secondDaughterIndex); // MVA2017v2
    float iso1 = tree->daughters_byDeepTau2017v2p1VSjetraw->at(t_firstDaughterIndex);                   // DeepTauV2p1
    float iso2 = tree->daughters_byDeepTau2017v2p1VSjetraw->at(t_secondDaughterIndex);                  // DeepTauV2p1

    tauPair_t pp = make_tuple(p4_1.Pt(), iso1, t_firstDaughterIndex, p4_2.Pt(), iso2, t_secondDaughterIndex, ipair);
    vPairs.push_back(pp);
  }

  // now extend the vector -- double the pairs
  // FIXME: could do v.resize(size x 2) to speed up
  uint totPairs = vPairs.size();
  for (unsigned int ipair = 0; ipair < totPairs; ++ipair)
  {
    tauPair_t pp = vPairs.at(ipair);
    tauPair_t pp_new = make_tuple(get<3>(pp), get<4>(pp), get<5>(pp), get<0>(pp), get<1>(pp), get<2>(pp), get<6>(pp));
    vPairs.push_back(pp_new);
  }

  // now sort by iso, then pt criteria
  stable_sort(vPairs.begin(), vPairs.end(), pairSort);

  // now search through pairs until one passes baseline
  int selPair = -1;

  if (debug)
  {
    cout << " .. debug: listing extended TauTau pair list for HTauTau ordering" << endl;
    for (uint ipair = 0; ipair < vPairs.size(); ++ipair)
    {
      auto pp = vPairs.at(ipair) ;
      int dau1index = get<2> (vPairs.at(ipair));
      int dau2index = get<5> (vPairs.at(ipair));
      int ipair_orig = get<6> (vPairs.at(ipair));
      //bool leg1 = tauBaseline (tree, dau1index, 20., 2.3, aeleVLoose, amuLoose, 99999., whatApply);  // MVA2017v2
      //bool leg2 = tauBaseline (tree, dau2index, 20., 2.3, aeleVLoose, amuLoose, 99999., whatApply);  // MVA2017v2
      bool leg1 = tauBaseline (tree, dau1index, 20., 2.3, aeleVVLoose, amuTight, 99999., whatApply); // DeepTauV2p1
      bool leg2 = tauBaseline (tree, dau2index, 20., 2.3, aeleVVLoose, amuTight, 99999., whatApply); // DeepTauV2p1
      cout << "  > " << ipair << " tau_idx1=" << dau1index << " tau_idx2=" << dau2index << " orig_pair_idx=" << ipair_orig
           << " | iso1=" << get<1> (pp) << " pt1=" << get<0> (pp) << " iso2=" << get<4> (pp) << " pt2=" << get<3> (pp)
           << " base1=" << leg1 << " base2=" << leg2 << " dR="<< DeltaRDau(tree, dau1index, dau2index) <<endl;
    }
  }

  for (uint ipair = 0; ipair < vPairs.size(); ++ipair)
  {
    int dau1index = get<2> (vPairs.at(ipair));
    int dau2index = get<5> (vPairs.at(ipair));
    int ipair_orig = get<6> (vPairs.at(ipair));
    //bool leg1 = tauBaseline (tree, dau1index, 20., 2.3, aeleVLoose, amuLoose, 99999., whatApply, debug);  // MVA2017v2
    //bool leg2 = tauBaseline (tree, dau2index, 20., 2.3, aeleVLoose, amuLoose, 99999., whatApply, debug);  // MVA2017v2
    bool leg1 = tauBaseline (tree, dau1index, 20., 2.3, aeleVVLoose, amuTight, 99999., whatApply, debug); // DeepTauV2p1
    bool leg2 = tauBaseline (tree, dau2index, 20., 2.3, aeleVVLoose, amuTight, 99999., whatApply, debug); // DeepTauV2p1

    bool isOS = tree->isOSCand->at(ipair_orig);
    if (whatApply.Contains("OScharge") && !isOS) {
      if (debug) cout<<"check baseline: OSCharge failed"<<endl;
      continue; // do not even check the rest if requiring the charge
    }
    if (whatApply.Contains("SScharge") && isOS) {
      if (debug) cout<<"check baseline: SSCharge failed"<<endl;
      continue; // for the same sign selection at the moment full selection over SS pairs
    }

    float dR = DeltaRDau(tree, dau1index, dau2index);
    //bool drMin = (dR > 0.1);
    bool drMin = (dR > 0.4);

    if (leg1 && leg2 && drMin)
    {
      selPair = ipair_orig;
      if (debug)
      {
        cout << "OPH final pair - orig_pair_idx:" <<selPair<<endl;
        cout << " - idx1:"<<dau1index<<" - iso1:"<<get<1>(vPairs.at(ipair))<<" - pt1:"<<get<0>(vPairs.at(ipair))<<endl;
        cout << " - idx2:"<<dau2index<<" - iso2:"<<get<4>(vPairs.at(ipair))<<" - pt2:"<<get<3>(vPairs.at(ipair))<<endl;
      }
      break;
    }
  }

  if (debug)
    cout << " .. debug: returning pair idx: " << selPair << endl;

  return selPair;
}


int OfflineProducerHelper::getBestPairPtAndRawIsoOrd (bigTree* tree, TString whatApply, bool debug)
{
  // prepare a tuple with all the needed sorting info
  // pt1 - iso1 - idx1 - pt2 - iso2 - idx2 - idxoriginalPair
  vector<tauPair_t> vPairs;
  for (unsigned int ipair = 0; ipair < tree->indexDau1->size(); ++ipair)
  {
    int t_firstDaughterIndex  = tree->indexDau1->at (ipair) ;
    int t_secondDaughterIndex = tree->indexDau2->at (ipair) ;
    int t_type1 = tree->particleType->at (t_firstDaughterIndex) ;
    int t_type2 = tree->particleType->at (t_secondDaughterIndex) ;
    if ( getPairType (t_type1, t_type2) != 2 ) continue ; // tau tau only

    float px1 = tree->daughters_px->at(t_firstDaughterIndex);
    float py1 = tree->daughters_py->at(t_firstDaughterIndex);
    float pz1 = tree->daughters_pz->at(t_firstDaughterIndex);
    float e1 =  tree->daughters_e->at(t_firstDaughterIndex);
    TLorentzVector p4_1 (px1, py1, pz1, e1);

    float px2 = tree->daughters_px->at(t_secondDaughterIndex);
    float py2 = tree->daughters_py->at(t_secondDaughterIndex);
    float pz2 = tree->daughters_pz->at(t_secondDaughterIndex);
    float e2 =  tree->daughters_e->at(t_secondDaughterIndex);
    TLorentzVector p4_2 (px2, py2, pz2, e2);

    //float iso1 = tree->daughters_byIsolationMVArun2017v2DBoldDMwLTraw2017->at(t_firstDaughterIndex);   // MVA2017v2
    //float iso2 = tree->daughters_byIsolationMVArun2017v2DBoldDMwLTraw2017->at(t_secondDaughterIndex);  // MVA2017v2
    float iso1 = tree->daughters_byDeepTau2017v2p1VSjetraw->at(t_firstDaughterIndex);                    // DeepTauV2p1
    float iso2 = tree->daughters_byDeepTau2017v2p1VSjetraw->at(t_secondDaughterIndex);                   // DeepTauV2p1

    // first one is highest pt
    tauPair_t pp;
    if (p4_1.Pt() > p4_2.Pt())
      pp = make_tuple(p4_1.Pt(), iso1, t_firstDaughterIndex, p4_2.Pt(), iso2, t_secondDaughterIndex, ipair);
    else
      pp = make_tuple(p4_2.Pt(), iso2, t_secondDaughterIndex, p4_1.Pt(), iso1, t_firstDaughterIndex, ipair);

    vPairs.push_back(pp);
  }

  // now sort by iso, then pt criteria
  stable_sort(vPairs.begin(), vPairs.end(), pairSortRawIso);

  // now search through pairs until one passes baseline
  int selPair = -1;

  if (debug)
  {
    cout << " .. debug: TauTau pairs list for raw iso + pt ordering" << endl;
    for (uint ipair = 0; ipair < vPairs.size(); ++ipair)
    {
      auto pp = vPairs.at(ipair) ;
      int dau1index = get<2> (vPairs.at(ipair));
      int dau2index = get<5> (vPairs.at(ipair));
      int ipair_orig = get<6> (vPairs.at(ipair));
      //bool leg1 = tauBaseline (tree, dau1index, 20., 2.3, aeleVLoose, amuLoose, 99999., whatApply);  // MVA2017v2
      //bool leg2 = tauBaseline (tree, dau2index, 20., 2.3, aeleVLoose, amuLoose, 99999., whatApply);  // MVA2017v2
      bool leg1 = tauBaseline (tree, dau1index, 20., 2.3, aeleVVLoose, amuTight, 99999., whatApply); // DeepTauV2p1
      bool leg2 = tauBaseline (tree, dau2index, 20., 2.3, aeleVVLoose, amuTight, 99999., whatApply); // DeepTauV2p1

      cout << "  > " << ipair << " idx1=" << dau1index << " idx2=" << dau2index << " ipair=" << ipair_orig
           << " | iso1=" << get<1> (pp) << " pt1=" << get<0> (pp) << " iso2=" << get<4> (pp) << " pt2=" << get<3> (pp)
           << " base1=" << leg1 << " base2=" << leg2 << endl;
    }
  }

  for (uint ipair = 0; ipair < vPairs.size(); ++ipair)
  {
    int dau1index = get<2> (vPairs.at(ipair));
    int dau2index = get<5> (vPairs.at(ipair));
    int ipair_orig = get<6> (vPairs.at(ipair));
    //bool leg1 = tauBaseline (tree, dau1index, 20., 2.3, aeleVLoose, amuLoose, 99999., whatApply, debug);  // MVA2017v2
    //bool leg2 = tauBaseline (tree, dau2index, 20., 2.3, aeleVLoose, amuLoose, 99999., whatApply, debug);  // MVA2017v2
    bool leg1 = tauBaseline (tree, dau1index, 20., 2.3, aeleVVLoose, amuTight, 99999., whatApply, debug); // DeepTauV2p1
    bool leg2 = tauBaseline (tree, dau2index, 20., 2.3, aeleVVLoose, amuTight, 99999., whatApply, debug); // DeepTauV2p1

    bool isOS = tree->isOSCand->at(ipair_orig);
    if (whatApply.Contains("OScharge") && !isOS) {
      if (debug) cout<<"check baseline: OSCharge failed"<<endl;
      continue; // do not even check the rest if requiring the charge
    }
    if (whatApply.Contains("SScharge") && isOS) {
      if (debug) cout<<"check baseline: SSCharge failed"<<endl;
      continue; // for the same sign selection at the moment full selection over SS pairs
    }

    float dR = DeltaRDau(tree, dau1index, dau2index);
    //bool drMin = (dR > 0.1);
    bool drMin = (dR > 0.4);

    if (leg1 && leg2 && drMin)
    {
      selPair = ipair_orig;
      break;
    }
  }

  if (debug)
    cout << " .. debug: returning pair idx: " << selPair << endl;

  return selPair;
}

bool OfflineProducerHelper::pairSort (const tauPair_t& pA, const tauPair_t& pB)
{
  // first leg 1 iso
  float isoA = get<1> (pA);
  float isoB = get<1> (pB);
  if (isoA > isoB) return true; // NB: MVA iso ! Most iso --> highest MVA score
  else if (isoA < isoB) return false;

  // then leg 1 pt
  float ptA = get<0> (pA);
  float ptB = get<0> (pB);
  if (ptA > ptB) return true;
  else if (ptA < ptB) return false;

  // then leg 2 iso
  isoA = get<4> (pA);
  isoB = get<4> (pB);
  if (isoA > isoB) return true;
  else if (isoA < isoB) return false;

  // then leg 2 pt
  ptA = get<3> (pA);
  ptB = get<3> (pB);
  if (ptA > ptB) return true;
  else if (ptA < ptB) return false;

  // should be never here..
  return false;
}


bool OfflineProducerHelper::pairSortRawIso (const tauPair_t& pA, const tauPair_t& pB)
{
  // first leg 1 iso
  float isoA = get<1> (pA);
  float isoB = get<1> (pB);
  if (isoA < isoB) return true; // NB: raw iso ! Most iso --> lowest value
  else if (isoA > isoB) return false;

  // then leg 1 pt
  float ptA = get<0> (pA);
  float ptB = get<0> (pB);
  if (ptA > ptB) return true;
  else if (ptA < ptB) return false;

  // then leg 2 iso
  isoA = get<4> (pA);
  isoB = get<4> (pB);
  if (isoA < isoB) return true;
  else if (isoA > isoB) return false;

  // then leg 2 pt
  ptA = get<3> (pA);
  ptB = get<3> (pB);
  if (ptA > ptB) return true;
  else if (ptA < ptB) return false;

  // should be never here..
  return false;
}

bool OfflineProducerHelper::getHardTauFinalVisGenProducts (bigTree* tree, int& ind1, int& ind2)
{
  int finalProds = 0;
  for (int iPart = 0; iPart < (int)tree->genpart_pdg->size(); iPart++)
  {
    int HInd = tree->genpart_HMothInd->at(iPart);
    int TauInd = tree->genpart_TauMothInd->at(iPart);
    int Pdg = tree->genpart_pdg->at(iPart);
    int aPdg = abs(Pdg);
    bool fromH = ( HInd != -1 );
    bool fromTau = ( TauInd != -1 );
    bool isPromptTauDecayProduct = checkBit (tree->genpart_flags->at(iPart), 5);
    //cout << iPart << " " << Pdg << " | fromH: " << fromH << " | fromTau: " << fromTau << endl;

    // e, mu
    if (aPdg == 11 || aPdg == 13)
    {
      if (fromH && fromTau && isPromptTauDecayProduct)
      {
	if (finalProds == 0)      ind1 = iPart;
	else if (finalProds == 1) ind2 = iPart;
	finalProds++;
      }
    }

    // tau h
    if (aPdg == 66615)
    {
      bool promptTau = checkBit (tree->genpart_flags->at(TauInd), 0); // check if mother tau is prompt
      if (fromH && fromTau && promptTau)
      {
	if (finalProds == 0)      ind1 = iPart;
	else if (finalProds == 1) ind2 = iPart;
	finalProds++;
      }
    }
  }

  if (finalProds != 2) return false; // I expect 2 and only 2 visible products (e, mu, tauh 66615)
  else return true;

}


bool OfflineProducerHelper::drMatchGenReco (bigTree* tree, int iGen, int iReco, float dRcone)
{
  TLorentzVector genP4  (tree->genpart_px->at(iGen), tree->genpart_py->at(iGen), tree->genpart_pz->at(iGen), tree->genpart_e->at(iGen));
  TLorentzVector recoP4 (tree->daughters_px->at(iReco), tree->daughters_py->at(iReco), tree->daughters_pz->at(iReco), tree->daughters_e->at(iReco));

  if (genP4.DeltaR(recoP4) < dRcone) return true;
  else return false;
}

int OfflineProducerHelper::getRecoMatchedToGen (bigTree* tree, int iGen, bool checkId, bool checkCharge, float dRcone)
{
  TLorentzVector genP4  (tree->genpart_px->at(iGen), tree->genpart_py->at(iGen), tree->genpart_pz->at(iGen), tree->genpart_e->at(iGen));
  int genID = tree->genpart_pdg->at(iGen);
  // change to tau Id for tauh
  if (abs(genID) == 66615) genID = 15*genID/abs(genID);
  int AgenID = abs(genID);

  std::vector < std::pair<float, int> > matchedReco;
  for (int iReco = 0; iReco < (int)tree->daughters_px->size(); iReco++)
  {
    int recoID = tree->PDGIdDaughters->at(iReco);
    bool IDCheck;
    if (checkCharge) IDCheck = ( genID == recoID );
    else             IDCheck = ( AgenID == abs(recoID) );

    if (!checkId) IDCheck = true; // bypass this requirement if I don't want ID to be checked
    if (IDCheck)
    {
      TLorentzVector recoP4 (tree->daughters_px->at(iReco), tree->daughters_py->at(iReco), tree->daughters_pz->at(iReco), tree->daughters_e->at(iReco));
      float dR = genP4.DeltaR(recoP4);
      if (dR < dRcone)   matchedReco.push_back (std::make_pair(dR, iReco));
    }
  }

  // find closest matched
  if (matchedReco.size() == 0) return -1;
  std::sort (matchedReco.begin(), matchedReco.end()); // are sorted according to the first index, i.e. the dR
  return ((matchedReco.at(0)).second );
}


float OfflineProducerHelper::DeltaRDau(bigTree* tree, int dau1idx, int dau2idx)
{
  TLorentzVector v1, v2;
  v1.SetPxPyPzE (tree->daughters_px->at(dau1idx), tree->daughters_py->at(dau1idx), tree->daughters_pz->at(dau1idx), tree->daughters_e->at(dau1idx));
  v2.SetPxPyPzE (tree->daughters_px->at(dau2idx), tree->daughters_py->at(dau2idx), tree->daughters_pz->at(dau2idx), tree->daughters_e->at(dau2idx));
  return v1.DeltaR(v2);
}
