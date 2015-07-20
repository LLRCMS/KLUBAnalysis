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

void addOverAndUnderFlow (TH1F* histo){

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
THStack * stackMe (TH1F * histo){

  string name = histo->GetName () ;
  name += "_st" ;
  THStack * dummy = new THStack (name.c_str (), "") ;
  dummy->Add (histo) ;
  return dummy ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----                                                                                                   
TH1F* mirrorHistogram(string name, TH1F* h1, TH1F*h2){

  TH1F* dummy = (TH1F*) h1->Clone(name.c_str());
  dummy->Reset();

  for(int iBin = 0; iBin <= h1->GetNbinsX()+1; iBin++){
    dummy->SetBinContent(iBin,h1->GetBinContent(iBin)+(h1->GetBinContent(iBin)-h2->GetBinContent(iBin)));
    dummy->SetBinError(iBin,h1->GetBinError(iBin));
  }

  return dummy ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----                                                                                                  
void makePositiveDefine(TH2F* histo){

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
void makePositiveDefine(TH1F* histo){

  for(int iBinX = 0; iBinX <= histo->GetNbinsX()+1; iBinX++){
    if( histo->GetBinContent(iBinX) < 0){
      histo->SetBinContent(iBinX,0);
      histo->SetBinError(iBinX,0);
    }
  }
  return ;
}
