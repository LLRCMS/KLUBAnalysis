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

typedef map<string, TH2F *> samples_2D_coll ;
typedef map<string, samples_2D_coll > cuts_2D_coll ;
typedef map<string, cuts_2D_coll > vars_2D_coll ;

struct plotContainer 
{
  plotContainer (string name) ;
  plotContainer (string name, vector<string> varList,
                 vector<pair <TString, TCut> > cutList, vector<string> sampleList, int histosType) ;  
  plotContainer (string name, vector<string> varList, vector<pair<string,string>> varList2D,
                 vector<pair <TString, TCut> > cutList, vector<string> sampleList, int histosType) ;

  void init (vector<string> varList, vector<pair <TString, TCut> > cutList, 
             vector<string> sampleList, int histosType) ;
  void MergeHistograms(vector<string> mergesampleList, TString mergedName);
  void createHistos (vector<string> varList, vector<pair<string,string>> varList2D,
                     vector<pair <TString, TCut> > cutList, vector<string> sampleList) ;      
  
  //int addHisto (string varName, string cutName, string sampleName) ; // not implemented yet
  int addSample (string sampleName, const plotContainer & original) ;
  TH1F * getHisto (string varName, string cutName, string sampleName) ;
  TH2F * get2DHisto (string var1Name, string var2Name, string cutName, string sampleName) ;
  map<string, TH1F *> & getStackSet (string varName, string cutName) ;
  THStack * makeStack (string varName, string cutName) ;
  THStack * make2DStack (pair <string,string> var2DName, string cutName) ;
  void AddOverAndUnderFlow () ;
  TH1F * createNewHisto (string name, string title, 
                         int nbinsx, double xlow, double xup,
                         int color, int histoType,
                         TString titleX, TString titleY) ;
  TH1F * createNewHisto (string name, string title, 
                         int nbinsx, float binning [],
                         int color, int histoType,
                         TString titleX, TString titleY) ;
  TH2F * createNew2DHisto (string name, string title, 
                         int nbinsx, double xlow, double xup,
                         int nbinsy, double ylow, double yup,
                         int color, int histoType,
                         TString titleX, TString titleY) ;
  void scale (float scaleFactor) ;
  void scale (vector<string> & variablesList, vector<pair <TString, TCut> > & selections, vector<vector<float>> scaleFactorVector) ;
  void scale2D (vector<pair<string,string>> & variables2DList, vector<pair <TString, TCut> > & selections, vector<vector<float>> scaleFactorVector) ;

  void setFillColor (int color) ;
  void save (TFile * fOut) ;
  void setHistosProperties (TH1 * h, int histoType, int color) ;
  void setHistosProperties (int histoType, int color) ;

  string m_name ;
  unsigned int m_Nvar ;    // 1D plots
  unsigned int m_N2Dvar ;  // 2D plots ("2D variable")
  unsigned int m_Ncut ;    // same for 1D and 2D
  unsigned int m_Nsample ; // same for 1D and 2D
  int m_histosType ;
  vars_coll m_histos ;     // hierarchy:   variable [selection [sample]]
  vars_2D_coll m_2Dhistos ;// hierarchy: 2Dvariable [selection [sample]]

  //vector<string> m_varList ; // vectors of keys that index the m_histos maps
  //vector<string> m_sampleList ; // they are in the same order that is followed by counters
  //vector<string> m_List ; // i.e. the order we want to apply the selections

} ;

#endif

