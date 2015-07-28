#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cctype>
#include <algorithm>
#include "ConfigParser.h"
#include "utils.h"
#include "TString.h"
#include "TChain.h"
#include "TCut.h"
#include "HistoManager.h"
#include "TTreeFormula.h"
#include "THStack.h"
#include "TCanvas.h"

#include "histoUtils.h"
#include "ConfigParser.h"
#include "utils.h"

#include "TMVA/MsgLogger.h"
#include "TMVA/Config.h"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"

using namespace std ;

int main (int argc, char** argv)
{
  // check number of inpt parameters
  if (argc < 3)
    {
      cerr << argv[0] << " filename branchname" << endl ;
      return 1 ;
    }


   TFile f (argv[1], "update") ;
   TTree *T = (TTree*) f.Get ("HTauTauTree") ;
   TBranch *b = T->GetBranch (argv[2]) ;
   T->GetListOfBranches ()->Remove (b) ;
   T->Write () ;
   f.Close () ;
   
   return 0 ;
}   
   
   