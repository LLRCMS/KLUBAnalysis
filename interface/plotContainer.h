#ifndef plotContainer_h
#define plotContainer_h

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <fstream>

#include "RooHist.h"
#include "RooCurve.h"

#include "TH1F.h"
#include "TH2F.h"
#include "THStack.h"
#include "TFile.h"
#include "TMath.h"
#include "TTreeFormula.h"

#include "utils.h"
#include "histoUtils.h"
#include "ConfigParser.h"

typedef map<string, TH1F *> samples_coll ;
typedef map<string, samples_coll > cuts_coll ;
typedef map<string, cuts_coll > vars_coll ;

struct plotContainer 
{
  plotContainer (string name) ;
  plotContainer (string name, vector<string> varList, vector<pair <TString, TCut> > cutList, 
                 vector<string> sampleList, int histosType) ;
  void init (vector<string> varList, vector<pair <TString, TCut> > cutList, 
             vector<string> sampleList, int histosType) ;
  void createHistos (vector<string> varList, vector<pair <TString, TCut> > cutList, 
                     vector<string> sampleList) ;      
  int addHisto (string varName, string cutName, string sampleName) ;
  int addSample (string sampleName, const plotContainer & original) ;
  TH1F * getHisto (string varName, string cutName, string sampleName) ;
  map<string, TH1F *> & getStackSet (string varName, string cutName) ;
  THStack * makeStack (string varName, string cutName) ;
  void AddOverAndUnderFlow () ;
  TH1F * createNewHisto (string name, string title, 
                         int nbinsx, double xlow, double xup,
                         int color, int histoType,
                         TString titleX, TString titleY) ;
  void scale (float scaleFactor) ;
  void setFillColor (int color) ;
  void save (TFile * fOut) ;
  void setHistosProperties (TH1F * h, int histoType, int color) ;
  void setHistosProperties (int histoType, int color) ;

  string m_name ;
  unsigned int m_Nvar ;
  unsigned int m_Ncut ;
  unsigned int m_Nsample ;
  int m_histosType ;
  vars_coll m_histos ; // hierarchy: variable [selection [sample]]
} ;

#endif

