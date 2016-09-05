//#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TClonesArray.h>
#include <TVector2.h>
#include <TMath.h>
#include <TChain.h>
#include <TH1.h>
#include <TLine.h>
#include <TProfile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "Math/PtEtaPhiM4D.h"
#include "Math/LorentzVector.h"
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
//#endif

typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > LorentzVector;

Double_t mTsq(TVector2 bT, TVector2 cT, Float_t mB, Float_t mC) {
  Float_t eB = TMath::Sqrt(mB*mB+ bT*bT);
  Float_t eC = TMath::Sqrt(mC*mC+ cT*cT);
  return mB*mB+mC*mC+2*(eB*eC - bT*cT);
}

class smT2
{
public:
  smT2() { name="thing"; }
  double operator()(const double *thing);
 
  void SetB1(TVector2 b1) { B1=b1; }
  void SetB2(TVector2 b2) { B2=b2; }
  void SetT1(TVector2 t1) { T1=t1; }
  void SetT2(TVector2 t2) { T2=t2; }
  void SetMPT(TVector2 mpt) { MPT=mpt; }
  void SetMB1(Float_t mB1) { MB1=mB1; }
  void SetMB2(Float_t mB2) { MB2=mB2; }
  void SetMT1(Float_t mT1) { MT1=mT1; }
  void SetMT2(Float_t mT2) { MT2=mT2; }
protected:
  TString name;
  TVector2 B1;
  TVector2 B2;
  TVector2 T1;
  TVector2 T2;
  TVector2 MPT;
  Float_t MB1;
  Float_t MB2;
  Float_t MT1;
  Float_t MT2;
};

double smT2::operator()(const double *thing) {
  double cT = thing[0];
  double cPhi = thing[1];

  TVector2 temp1(0,0);
  temp1.SetMagPhi(cT,cPhi);
  TVector2 temp2 = MPT-temp1;
  SetT1(temp1);
  SetT2(temp2);

  return TMath::Max(TMath::Sqrt(mTsq(B1, T1, MB1, MT1)), TMath::Sqrt(mTsq(B2, T2, MB2, MT2)));
}
