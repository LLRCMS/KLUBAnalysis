#ifndef histoUtils_h
#define histoUtils_h

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

using namespace std ;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----                                                                                                

void avoidEmptyBins(TH1F* input);

// add the overflow bin as real bin to the histo                                                                                                                                
void   addOverFlow (TH1F * input) ;
void   addOverAndUnderFlow (TH1F* histo);

// give an histogram set the poisson error bars                                                                                                                                
void   setPoissonErrorsToHisto (TH1F * input) ;

// give an histogram set the poisson error bars                                                                                                                              
void   setPoissonErrorsTo2DHisto (TH2F * input) ;

// give an histogram set the poisson error bars                                                                                                                                 
void   setAsymmetricErrorsToHisto (TH1F * input) ;

// give an histogram set the poisson error bars                                                                                                                                
void   setAsymmetricErrorsTo2DHisto (TH2F * input) ;

// unrolled a 2D histo in a 1D one
TH1F*  unRollingHistogram(TH2F* histo, int errorType);

// take an histogram with bin errors                                                                                                                                          
TH1F*  getHistoOfErrors (TH1F * input,int isLog) ;

// create a stack from a histo                                                                                                                                            
THStack* CreateStack (TH1F * histo) ;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
THStack * stackMe (TH1F * histo);

// make positive defined histos
void makePositiveDefine(TH1F* histo);
void makePositiveDefine(TH2F* histo);

//mirror an histogram h1 wrt to h2
TH1F* mirrorHistogram(string name, TH1F* h1, TH1F*h2);


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----                                                                                                 
// re-roll unrolled 2D distributions and return a TH2F.                                                                                                                    
// templated on RooFit objects, works for RooHist and RooCurve,                                                                                                                 
// which are the toy and the fitting functions in the combine                                                                                                           
// output, respectively.                                                                                                                                                       

template <class T>
TH2F * roll ( T * original,    // thing to be re-rolled                                                                                                                     
	     int nBinsX, float Xmin, float Xmax,
	     int nBinsY, float Ymin, float Ymax,
	     RooHist * reference     // unrolled events, to get                                                                                                              
	     )
{
  int nBinsOriginal = reference->GetN () ;
   
  if (nBinsY * nBinsX != nBinsOriginal)
    {
      cerr << " problems in number of bins" << endl ;
      exit (1) ;
    }
  TString name = "roll_" ;
  name += original->GetName () ;
  TH2F * dummy = new TH2F (name, name,
			   nBinsX, Xmin, Xmax,
			   nBinsY, Ymin, Ymax
			   ) ;
  for (int i = 1 ; i <= nBinsOriginal ; ++i)
    {
      Double_t yvalue = original->Eval (i - 0.5) ;
      int xbin = i / nBinsY + 1 ;
      int ybin = i % nBinsY ;

      dummy->SetBinContent (xbin, ybin, yvalue) ;
    }
  return dummy ;
}
 

#endif
