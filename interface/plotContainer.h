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

typedef std::map<std::string, TH1F *> samples_coll ;
typedef std::map<std::string, samples_coll > cuts_coll ;
typedef std::map<std::string, cuts_coll > vars_coll ;

typedef std::map<std::string, TH2F *> samples_2D_coll ;
typedef std::map<std::string, samples_2D_coll > cuts_2D_coll ;
typedef std::map<std::string, cuts_2D_coll > vars_2D_coll ;

struct plotContainer 
{
  plotContainer (std::string name) ;
  plotContainer (std::string name, std::vector<std::string> varList,
                 std::vector<std::pair <TString, TCut> > cutList, std::vector<std::string> sampleList, int histosType) ;
  plotContainer (std::string name, std::vector<std::string> varList, std::vector<std::pair<std::string,std::string>> varList2D,
                 std::vector<std::pair <TString, TCut> > cutList, std::vector<std::string> sampleList, int histosType) ;

  void init (std::vector<std::string> varList, std::vector<std::pair <TString, TCut> > cutList,
             std::vector<std::string> sampleList, int histosType) ;
  void MergeHistograms(std::vector<std::string> mergesampleList, TString mergedName);
  void createHistos (std::vector<std::string> varList, std::vector<std::pair<std::string,std::string>> varList2D,
                     std::vector<std::pair <TString, TCut> > cutList, std::vector<std::string> sampleList) ;
  
  //int addHisto (string varName, string cutName, string sampleName) ; // not implemented yet
  int addSample (std::string sampleName, const plotContainer & original) ;
  TH1F * getHisto (std::string varName, std::string cutName, std::string sampleName) ;
  TH2F * get2DHisto (std::string var1Name, std::string var2Name, std::string cutName, std::string sampleName) ;
  std::map<std::string, TH1F *> & getStackSet (std::string varName, std::string cutName) ;
  THStack * makeStack (std::string varName, std::string cutName) ;
  THStack * make2DStack (std::pair <std::string,std::string> var2DName, std::string cutName) ;
  void AddOverAndUnderFlow () ;
  TH1F * createNewHisto (std::string name, std::string title,
                         int nbinsx, double xlow, double xup,
                         int color, int histoType,
                         TString titleX, TString titleY) ;
  TH1F * createNewHisto (std::string name, std::string title,
                         int nbinsx, float binning [],
                         int color, int histoType,
                         TString titleX, TString titleY) ;
  TH2F * createNew2DHisto (std::string name, std::string title,
                         int nbinsx, double xlow, double xup,
                         int nbinsy, double ylow, double yup,
                         int color, int histoType,
                         TString titleX, TString titleY) ;
  void scale (float scaleFactor) ;
  void scale (std::vector<std::string> & variablesList, std::vector<std::pair <TString, TCut> > & selections, std::vector<std::vector<float>> scaleFactorVector) ;
  void scale2D (std::vector<std::pair<std::string,std::string>> & variables2DList, std::vector<std::pair <TString, TCut> > & selections, std::vector<std::vector<float>> scaleFactorVector) ;

  void setFillColor (int color) ;
  void save (TFile * fOut) ;
  void setHistosProperties (TH1 * h, int histoType, int color) ;
  void setHistosProperties (int histoType, int color) ;

  std::string m_name ;
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

