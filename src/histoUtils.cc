#include "histoUtils.h"

void avoidEmptyBins(TH1F* input){

  for (int iBin = 1 ; iBin < input->GetNbinsX () + 1 ; ++iBin){
    if(input->GetBinContent(iBin) <= 0){
      input->SetBinContent(iBin,1e-5);
      input->SetBinError(iBin,0);      
    }      
    if(input->GetBinContent(iBin)-input->GetBinError(iBin) < 0)
      input->SetBinError(iBin,input->GetBinContent(iBin)-1e-5);
  }

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// add the overflow bin to a histogram                                                                                                                                          
void addOverFlow (TH1F * input) {

  TH1F * dummy = new TH1F (
			   "tempo",
			   input->GetTitle (),
			   input->GetNbinsX () + 1,
			   input->GetXaxis ()->GetXmin (),
			   input->GetXaxis ()->GetXmax () + input->GetBinWidth (1)
			   ) ;

  for (int iBin = 0 ; iBin <= input->GetNbinsX () + 1 ; ++iBin) {
    dummy->SetBinContent (iBin, input->GetBinContent (iBin)) ;
    dummy->SetBinError (iBin, input->GetBinError (iBin)) ;
  }

  if(input->GetDefaultSumw2())
    dummy->Sumw2 () ;

  string name = input->GetName () ;
  input->SetName ("trash") ;
  dummy->GetXaxis()->SetTitle(input->GetXaxis()->GetTitle());
  dummy->SetName (name.c_str ()) ;
  swap (*input, *dummy) ;
  return ;

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void addOverAndUnderFlow (TH1F* histo)
{
  // content
  histo->SetBinContent(histo->GetNbinsX(),histo->GetBinContent(histo->GetNbinsX())+histo->GetBinContent(histo->GetNbinsX()+1));  
  histo->SetBinContent(1,histo->GetBinContent(1)+histo->GetBinContent(0));
  // errors
  histo->SetBinError(histo->GetNbinsX(),sqrt(pow(histo->GetBinError(histo->GetNbinsX()),2)+pow(histo->GetBinError(histo->GetNbinsX()+1),2)));    
  histo->SetBinError(1,sqrt(pow(histo->GetBinError(1),2)+pow(histo->GetBinError(0),2)));    
  // unset
  histo->SetBinContent(0,0);
  histo->SetBinError(0,0);
  histo->SetBinContent(histo->GetNbinsX()+1,0);
  histo->SetBinError(histo->GetNbinsX()+1,0);

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----        

                                                                                         
void setPoissonErrorsToHisto (TH1F * input) { // set the error in each bin as sqrt(binContent)                                                                                  
  for (int iBin = 0 ; iBin <= input->GetNbinsX () + 1 ; ++iBin) {
    input->SetBinError (iBin, sqrt (input->GetBinContent (iBin))) ;
  }

  return ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----         

                                                                                         
void setPoissonErrorsTo2DHisto (TH2F * input) { // set the error in each bin as sqrt(binContent)                                                                                
  for (int iBinX = 0 ; iBinX <= input->GetNbinsX () + 1 ; ++iBinX) {
    for (int iBinY = 0 ; iBinY <= input->GetNbinsY () + 1 ; ++iBinY) {
      input->SetBinError (iBinX,iBinY,sqrt (input->GetBinContent (iBinX,iBinY))) ;
    }
  }

  return ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----     

                                                                                           
void setAsymmetricErrorsToHisto (TH1F * input) { // set the error in each bin as sqrt(binContent)                                                                              
  input->SetBinErrorOption(TH1::kPoisson);
  return ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----   

                                                                                              
void setAsymmetricErrorsTo2DHisto (TH2F * input) { // set the error in each bin as sqrt(binContent)                                                                            
  input->SetBinErrorOption(TH1::kPoisson);

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----    

                                                                                              
TH1F *  getHistoOfErrors (TH1F * input, int isLog) {

  string name = "err_" ;
  name += input->GetName () ;
  if(isLog) name +="_log";
  string title = "errors of " ;
  title += input->GetTitle () ;
  TH1F * dummy = new TH1F (
			   name.c_str (),
			   title.c_str (),
			   input->GetNbinsX (),
			   input->GetXaxis ()->GetXmin (),
			   input->GetXaxis ()->GetXmax ()
			   ) ;

  for (int iBin = 0 ; iBin <= input->GetNbinsX () + 1 ; ++iBin) {
    dummy->SetBinContent (iBin, input->GetBinError (iBin)) ;
  }

  return dummy ;

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

                                                                                                 
TH1F* unRollingHistogram (TH2F* histo, int errorType){

  TH1F* dummy = new TH1F("dummy","",(histo->GetNbinsX())*(histo->GetNbinsY()),0,histo->GetNbinsX()*histo->GetNbinsY());
  dummy->SetName((string(histo->GetName())+"_dummy").c_str());
  dummy->Sumw2();

  // copy all the entries (basic structure inside the matrix)                                                                                                                  
  for(int iBinX = 1; iBinX <= histo->GetNbinsX() ; iBinX++){
    for(int iBinY = 1; iBinY <= histo->GetNbinsY() ; iBinY++){
      dummy->SetBinContent((iBinX-1)*histo->GetNbinsY()+iBinY,histo->GetBinContent(iBinX,iBinY));
      dummy->SetBinError((iBinX-1)*histo->GetNbinsY()+iBinY,histo->GetBinError(iBinX,iBinY));
    }
  }

  // now add underflow alogn Y axis                                                                                                                                            
  for(int iBinY = 1; iBinY <= histo->GetNbinsY() ; iBinY++){
    dummy->SetBinContent(iBinY,histo->GetBinContent(1,iBinY)+histo->GetBinContent(0,iBinY));
  }

  // now add overflow alogn Y axis                                                                                                                                             
  for(int iBinY = 1; iBinY <= histo->GetNbinsY() ; iBinY++){
    dummy->SetBinContent(histo->GetNbinsY()*(histo->GetNbinsX()-1)+iBinY,histo->GetBinContent(histo->GetNbinsX(),iBinY)+histo->GetBinContent(histo->GetNbinsX()+1,iBinY));
  }


  // now add underflow alogn X axis                                                                                                                                            
  for(int iBinX = 1; iBinX <= histo->GetNbinsX() ; iBinX++){
    dummy->SetBinContent((histo->GetNbinsY()*(iBinX-1)+1),histo->GetBinContent(iBinX,1)+histo->GetBinContent(iBinX,0));
  }

  // now add overflow alogn X axis                                                                                                                                             
  for(int iBinX = 0; iBinX <= histo->GetNbinsX()+1 ; iBinX++){

    dummy->SetBinContent((histo->GetNbinsY()*(iBinX)),histo->GetBinContent(iBinX,histo->GetNbinsY())+histo->GetBinContent(iBinX,histo->GetNbinsY()+1));
  }

  dummy->SetBinContent(1,histo->GetBinContent(0,0)+dummy->GetBinContent(1));
  dummy->SetBinContent(histo->GetNbinsY(),histo->GetBinContent(0,histo->GetNbinsY()+1)+dummy->GetBinContent(histo->GetNbinsY()));
  dummy->SetBinContent(histo->GetNbinsY()*(histo->GetNbinsX()-1),histo->GetBinContent(histo->GetNbinsX()+1,0)+dummy->GetBinContent(histo->GetNbinsY()*(histo->GetNbinsX()-1)));
  dummy->SetBinContent(histo->GetNbinsX()*histo->GetNbinsY(),histo->GetBinContent(histo->GetNbinsX()+1,histo->GetNbinsY()+1)+dummy->GetBinContent(histo->GetNbinsY()*histo->GetNbinsX()));

  if(errorType == 0){
    setPoissonErrorsToHisto(dummy);
  }
  else if(errorType ==1){
    setAsymmetricErrorsToHisto(dummy);
  }

  return dummy;

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----     

                                                                                             
THStack * stackMe (TH1F * histo)
{
  string name = histo->GetName () ;
  name += "_st" ;
  THStack * dummy = new THStack (name.c_str (), "") ;
  dummy->Add (histo) ;
  return dummy ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----  

                                                                                                 
TH1F* mirrorHistogram(string name, TH1F* h1, TH1F*h2)
{
  TH1F* dummy = (TH1F*) h1->Clone(name.c_str());
  dummy->Reset();

  for(int iBin = 0; iBin <= h1->GetNbinsX()+1; iBin++){
    dummy->SetBinContent(iBin,h1->GetBinContent(iBin)+(h1->GetBinContent(iBin)-h2->GetBinContent(iBin)));
    dummy->SetBinError(iBin,h1->GetBinError(iBin));
  }

  return dummy ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

                                                                                                  
void makePositiveDefine(TH2F* histo)
{
  for(int iBinX = 0; iBinX <= histo->GetNbinsX()+1; iBinX++){
    for(int iBinY = 0; iBinY <= histo->GetNbinsY()+1; iBinY++){
      if( histo->GetBinContent(iBinX,iBinY) < 0){
        histo->SetBinContent(iBinX,iBinY,0);
        histo->SetBinError(iBinX,0);
      }
    }
  }

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

                                                                                                 
void makePositiveDefine(TH1F* histo)
{
  for(int iBinX = 0; iBinX <= histo->GetNbinsX()+1; iBinX++){
    if( histo->GetBinContent(iBinX) < 0){
      histo->SetBinContent(iBinX,0);
      histo->SetBinError(iBinX,0);
    }
  }
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

                                                                                                 
THStack * normaliseStack (THStack * original, bool nostack )
{
  TString name = original->GetName () ;
  name += "_norm" ;
  THStack * dummy_stack = new THStack (name, name) ;
  TH1F * dummy_h = (TH1F *) original->GetStack ()->Last () ;
  float norm = dummy_h->Integral () ;
  
  TIter next (original->GetHists ()) ;
  TH1F * histo ;

  while ((histo = (TH1F *) (next ()))) 
    { 
      dummy_h = (TH1F *) histo->Clone (histo->GetName () + TString ("_norm")) ;
      if (nostack) dummy_h->Scale (1. / dummy_h->Integral()) ;
      else dummy_h->Scale (1. / norm) ;
      dummy_stack->Add (dummy_h) ;
    }
    
  return dummy_stack ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void copyTitles (TH1F * histogram, THStack * hstack)
{
  TIter next (hstack->GetHists ()) ;
  TH1F * histo ;

  while ((histo = (TH1F *) (next ()))) 
    {
      histogram->GetXaxis ()->SetTitle (
        histo->GetXaxis ()->GetTitle ()) ;
      histogram->GetYaxis ()->SetTitle (
        histo->GetYaxis ()->GetTitle ()) ;
      break ;  
    }
  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


float findNonNullMinimum (TH1F * histo)
  {
    float min = 10000000000. ;
    for (int i = 1 ; i <= histo->GetNbinsX () ; ++i)
      { 
        float tmpmin = histo->GetBinContent (i) ;
        if (tmpmin == 0.) continue ;
        if (tmpmin < min) min = tmpmin ;
      }
    return min ;
  }


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


// xmin ymin xmax ymax
// islog : search only non-null minima
// nostack: search the max of each histo and not the max of the stack itself
vector<float> getExtremes (THStack * hstack, bool islog, bool nostack)
{
  float ymax = hstack->GetMaximum () ;
  float xmin = ((TH1F*)(hstack->GetStack()->Last()))->GetXaxis()->GetXmin() ; 
  float xmax = ((TH1F*)(hstack->GetStack()->Last()))->GetXaxis()->GetXmax() ;
  float ymin = -1;

  TIter next (hstack->GetHists ()) ;
  TH1F * histo ;

  int counter = 0;
  std::vector<float> ymins;
  std::vector<float> ymaxs; // used only if nostack = true
  while ((histo = (TH1F *) (next ()))) 
  {
    counter++;
    if (islog) 
    {
      if (histo->GetMaximum() > 0)
      {
        ymins.push_back(histo->GetMinimum(0)) ; // non-null min, skip all neg histos
        ymaxs.push_back (histo->GetMaximum());
      }
    }
    else
    {
      ymins.push_back (histo->GetMinimum()) ;
      ymaxs.push_back (histo->GetMaximum());
    }
  }

  ymin = *(min_element (ymins.begin(), ymins.end()));
  if (nostack) ymax = *(max_element (ymaxs.begin(), ymaxs.end()));

  vector<float> extremes (4, 0.) ;
  extremes.at (0) = xmin ;
  extremes.at (1) = ymin ;
  extremes.at (2) = xmax ;
  extremes.at (3) = ymax ;

//cout << "IN GETEXTREMES: y min is: " << ymin << " (isLOG: " << islog << ")" << " Nostack: " << nostack << endl;

  return extremes ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// xmin ymin xmax ymax
vector<float> getExtremes (std::vector<TH1F*>& histos, bool islog)
{
  float xmin = 1. ;
  float xmax = 0. ;
  float ymin = 90000000000. ; // FIXME: check: non trova il minimo??
  float ymax = 0.;

  for (unsigned int ihisto = 0; ihisto < histos.size(); ihisto++)
  {
    TH1F* histo = histos.at(ihisto);
    float tmpmin = findNonNullMinimum (histo) ;
    // if (tmpmin <= 0) continue ;
    // if (tmpmin < ymin && tmpmin <= 0) ymin = tmpmin ;
    if (islog)
    {  if (tmpmin < ymin && tmpmin > 0) ymin = tmpmin ; }
    else
    {  if (tmpmin < ymin) ymin = tmpmin;} // not log --> can have minimum < 0 (happens...)

    float tmpmax = histo->GetMaximum();
    // update with error
    float errUp = histo->GetBinErrorUp(histo->GetMaximumBin());
    tmpmax += errUp;

    if (tmpmax > ymax) ymax = tmpmax ;

    if (xmin > xmax)
    {
      xmin = histo->GetXaxis ()->GetXmin () ;
      xmax = histo->GetXaxis ()->GetXmax () ;
    }
  }
  vector<float> extremes (4, 0.) ;
  extremes.at (0) = xmin ;
  extremes.at (1) = ymin ;
  extremes.at (2) = xmax ;
  extremes.at (3) = ymax ;
  return extremes;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---



float min3 (float uno, float due, float tre)
{
  return min (min (uno, due), tre) ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


float max3 (float uno, float due, float tre)
{
  return max (max (uno, due), tre) ;
}


