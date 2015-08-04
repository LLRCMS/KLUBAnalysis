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

                                                                                                 
THStack * normaliseStack (THStack * original)
{
  TString name = original->GetName () ;
  name += "_norm" ;
  THStack * dummy_stack = new THStack (name, name) ;
  TH1F * dummy_h = (TH1F *) original->GetStack ()->Last () ;
  float norm = dummy_h->Integral () ;
  
  TIter next (original->GetHists ()) ;
  TH1F * histo ;

  while (histo = (TH1F *) (next ())) 
    { 
      dummy_h = (TH1F *) histo->Clone (histo->GetName () + TString ("_norm")) ;
      dummy_h->Scale (1. / norm) ;
      dummy_stack->Add (dummy_h) ;
    }
    
  return dummy_stack ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void copyTitles (TH1F * histogram, THStack * hstack)
{
  TIter next (hstack->GetHists ()) ;
  TH1F * histo ;

  while (histo = (TH1F *) (next ())) 
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
vector<float> getExtremes (THStack * hstack, bool islog)
{
  float ymax = hstack->GetMaximum () ;

  TIter next (hstack->GetHists ()) ;
  TH1F * histo ;

  float xmin = 1. ;
  float xmax = 0. ;
  float ymin = 10000000000. ;
  while (histo = (TH1F *) (next ())) 
    {
      float tmpmin = findNonNullMinimum (histo) ;
      if (tmpmin < ymin) ymin = tmpmin ;
      if (xmin > xmax)
        {
          xmin = histo->GetXaxis ()->GetXmin () ;
          xmax = histo->GetXaxis ()->GetXmax () ;
        }
    }

//  ymin *= 0.9 ;
//  ymax *= 1.3 ;

  vector<float> extremes (4, 0.) ;
  extremes.at (0) = xmin ;
  extremes.at (1) = ymin ;
  extremes.at (2) = xmax ;
  extremes.at (3) = ymax ;

  return extremes ;
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


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void
addHistos (vector<sample> & samples, HistoManager * manager,
           vector<string> & variablesList,
           vector<pair <TString, TCut> > & selections,
           bool isSignal,
           bool isData)
{
  TString histoName ;
  int histoType = 0 ;
  if (isSignal) histoType = 1 ;
  if (isData) histoType = 2 ;

  // loop on sim samples
  for (unsigned int j = 0 ; j < samples.size () ; ++j)
    {
      for (unsigned int k = 0 ; k < selections.size () ; ++k)
        {
          for (unsigned int i = 0 ; i < variablesList.size () ; ++i)
            {
              histoName.Form ("%s_%s_%s",
                              variablesList.at (i).c_str (),
                              samples.at (j).sampleName.Data (),
                              selections.at (k).first.Data ()
                              ) ;
              // remove not alphanumeric symbols from the var name
              string varID = variablesList.at (i) ;
              varID.erase (std::remove_if (varID.begin (), varID.end (), isNOTalnum ()), varID.end ()) ;
              // get histo nbins and range
              vector <float> limits = 
                gConfigParser->readFloatListOption (TString ("histos::") 
                    + varID.c_str ()) ;
              manager->AddNewHisto (histoName.Data (),histoName.Data (),
                  int (limits.at (0)), limits.at (1), limits.at (2),
                  gConfigParser->readIntOption (TString ("colors::") 
                      + samples.at (j).sampleName.Data ()), 
                  histoType,
                  variablesList.at (i).c_str (), "events"
                ) ;
            }  
        }
    } // loop on sim samples

  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


counters
fillHistos (vector<sample> & samples, HistoManager * manager, 
            vector<string> & variablesList,
            vector<pair <TString, TCut> > & selections,
            float lumi,
            const vector<float> & scale,
            bool isData,
            bool isSignal)
{
  addHistos (samples, manager, variablesList, selections, isSignal, isData) ;

  TString histoName ;

  // for efficiency evaluation
  counters localCounter ;

  //Loop on the samples
  for (unsigned int iSample = 0 ; iSample < samples.size () ; ++iSample)
    {
      double eff = samples.at (iSample).eff ;
      localCounter.initEfficiencies.push_back (eff) ;

      localCounter.counters.push_back (vector<float> (selections.size () + 1, 0.)) ;
      
      TTree *tree = samples.at (iSample).sampleTree ;
      TTreeFormula * TTF[selections.size ()] ;
      for (unsigned int isel = 0 ; isel < selections.size () ; ++isel)
        {
          TString fname ; fname.Form ("ttf%d",isel) ;
          TTF[isel] = new TTreeFormula (fname.Data (), selections.at (isel).second, tree) ;
        }
  
      float weight ;
      tree->SetBranchAddress ("MC_weight", &weight) ;
      // signal scaling
      float scaling = 1. / samples.at (iSample).eff_den ;
      if (scale.size () > 0) scaling *= scale.at (iSample) ;

      cout << "Opening sample: "
           << samples.at (iSample).sampleName
           << "\t with initial weighted events\t" << samples.at (iSample).eff_den
           << endl ;

      vector<float> address (variablesList.size (), 0.) ; 
      for (unsigned int iv = 0 ; iv < variablesList.size () ; ++iv)
        tree->SetBranchAddress (variablesList.at (iv).c_str (), &(address.at (iv))) ;
    
      for (int iEvent = 0 ; iEvent < tree->GetEntries () ; ++iEvent)
        {
          tree->GetEntry (iEvent) ;
          
          if (isData) localCounter.counters.at (iSample).at (0) += 1. ;
          else        localCounter.counters.at (iSample).at (0) 
                          += weight * lumi * scaling ;
          for (unsigned int isel = 0 ; isel < selections.size () ; ++isel)
            {

              if (! TTF[isel]->EvalInstance ()) continue ;

              if (isData) localCounter.counters.at (iSample).at (isel+1) += 1. ;
              else        localCounter.counters.at (iSample).at (isel+1) 
                              += weight * lumi * scaling ;

              for (unsigned int iv = 0 ; iv < variablesList.size () ; ++iv)
                {
                  histoName.Form ("%s_%s_%s",
                      variablesList.at (iv).c_str (),
                      samples.at (iSample).sampleName.Data (),
                      selections.at (isel).first.Data ()
                    ) ;
                  TH1F * histo = manager->GetHisto (histoName.Data ()) ;
                  if (isData) 
                      histo->Fill (address[iv]) ;
                  else        
                      histo->Fill (address[iv], weight * lumi * scaling) ;
                } //loop on variables
            } //loop on selections
        } //loop on tree entries

      for (unsigned int isel = 0 ; isel < selections.size () ; ++isel) delete TTF[isel] ;
    } // Loop on the MC samples

  return localCounter ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


vector<THStack *> 
stackHistos (vector<sample> & samples, HistoManager * manager, 
            vector<string> & variablesList,
            vector<pair <TString, TCut> > & selections,
            const string & tag)
{
  int nVars = variablesList.size () ;
  int nSel = selections.size () ;
  TString outputName, histoName ;
  
  vector <THStack *> hstack (nVars*nSel) ; //one stack for variable

  for (int isel = 0 ; isel < nSel ; ++isel)
    {
      for (int iv = 0 ; iv < nVars ; ++iv)
        {
          // filling stacks for background
          outputName.Form ("stack_%s_%s_%s",
            tag.c_str (),
            variablesList.at (iv).c_str (), selections.at (isel).first.Data ()) ;
          hstack.at (iv+nVars*isel) = new THStack (outputName.Data (), outputName.Data ()) ;
          for (unsigned int i = 0 ; i < samples.size () ; ++i)
            {
              histoName.Form ("%s_%s_%s",
                  variablesList.at (iv).c_str (),
                  samples.at (i).sampleName.Data (),
                  selections.at (isel).first.Data ()
                ) ;
              addOverAndUnderFlow (manager->GetHisto (histoName.Data ())) ;
              hstack.at (iv+nVars*isel)->Add (manager->GetHisto (histoName.Data ())) ;
            } 
        }
    }
  return hstack ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


plotContainer::plotContainer (string name, vector<string> varList, 
                              vector<pair <TString, TCut> > cutList,
                              vector<string> sampleList, int histosType) :
  m_name (name),
  m_Nvar (varList.size ()),
  m_Ncut (cutList.size ()),
  m_Nsample (sampleList.size ()), 
  m_histosType (histosType)
{
  for (unsigned int ivar = 0 ; ivar < m_Nvar ; ++ivar)
    {
      map<string, map<string, TH1F *> > varDummy ;
      for (unsigned int icut = 0 ; icut < m_Ncut ; ++icut)
        {
          map<string, TH1F *> cutDummy ;
          for (unsigned int isample = 0 ; isample < m_Nsample ; ++isample)
            {
              TH1F * sampleDummy = 0 ;

              // remove not alphanumeric symbols from the var name
              string varID = varList.at (ivar) ;
              varID.erase (std::remove_if (varID.begin (), varID.end (), isNOTalnum ()), varID.end ()) ;
              // get histo nbins and range
              vector <float> limits = 
                gConfigParser->readFloatListOption (TString ("histos::") 
                    + varList.at (ivar).c_str ()) ;
              string histoName = m_name + varList.at (ivar) + cutList.at (icut).first.Data () + sampleList.at (isample) ;     
              createNewHisto (histoName, histoName,
                  int (limits.at (0)), limits.at (1), limits.at (2),
                  gConfigParser->readIntOption (TString ("colors::") 
                      + TString (sampleList.at (isample).c_str ())), 
                  m_histosType,
                  varList.at (ivar).c_str (), "events"
                ) ;
              cutDummy[sampleList.at (isample)] = sampleDummy ;
            }
          varDummy[cutList.at (icut).first.Data ()] = cutDummy ;          
        }
      m_histos[varList.at (ivar)] = varDummy ;
    }
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


TH1F * 
plotContainer::getHisto (string varName, string cutName, string sampleName)
{
  if (m_histos.find (varName) == m_histos.end ()) return 0 ;
  if (m_histos[varName].find (cutName) == m_histos[varName].end ()) return 0 ;
  if (m_histos[varName][cutName].find (sampleName) == m_histos[varName][cutName].end ()) return 0 ;
  return m_histos[varName][cutName][sampleName] ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


int 
plotContainer::addSample (string sampleName, vector<TH1F*> histos) 
{
  if (histos.size () != m_Ncut * m_Nvar) return -1 ;
  for (vars_coll::iterator iVar = m_histos.begin () ;
       iVar != m_histos.end () ;
       ++iVar)
    {
      for (cuts_coll::iterator iCut = iVar->second.begin () ; 
           iCut != iVar->second.end () ;
           ++iCut)
        {
          // iHisto = iv + nVars * isel
          int index = distance (iCut, iVar->second.begin ()) * m_Ncut + 
                      distance (iVar, m_histos.begin ()) ;
          iCut->second[sampleName] = histos.at (index) ;
        }
    }
  ++m_Nsample ;
  return 0 ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


map<string, TH1F *> & 
plotContainer::getStackSet (string varName, string cutName)
{
  if (m_histos.find (varName) == m_histos.end ()) 
    {
      cerr << "no histos for " << varName << endl ;
      exit (1) ;
    }
  if (m_histos[varName].find (cutName) == m_histos[varName].end ())
    {
      cerr << "no histos for " << varName << ", " << cutName << endl ;
      exit (1) ;
    }
  return m_histos[varName][cutName] ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


THStack * 
plotContainer::makeStack (string varName, string cutName)
{
  TString outputName, histoName ;
  outputName.Form ("%s_%s_%s",
    m_name.c_str (), varName.c_str (), cutName.c_str ()) ;
  THStack * stack = new THStack (outputName.Data (), outputName.Data ()) ;

  for (map<string, TH1F *>::iterator iSample = m_histos[varName][cutName].begin () ;
       iSample != m_histos[varName][cutName].end () ;
       ++iSample)
    {
      stack->Add (iSample->second) ;
    }

  return stack ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void
plotContainer::AddOverAndUnderFlow ()
{
  for (vars_coll::iterator iVar = m_histos.begin () ;
       iVar != m_histos.end () ;
       ++iVar)
    {
      for (cuts_coll::iterator iCut = iVar->second.begin () ; 
           iCut != iVar->second.end () ;
           ++iCut)
        {
          for (samples_coll::iterator iSample = iCut->second.begin () ; 
               iSample != iCut->second.end () ;
               ++iSample)
              addOverAndUnderFlow (iSample->second) ;
        }
    }
  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


TH1F * 
plotContainer::createNewHisto (string name, string title, 
                               int nbinsx, double xlow, double xup,
                               int color, int histoType,
                               TString titleX, TString titleY)
{
  TH1F* h = new TH1F (name.c_str (), title.c_str (), nbinsx, xlow, xup);
  h->Sumw2 () ;    
  if (histoType == 0) // background
    {//background
      h->SetFillStyle (1001) ;
      h->SetFillColor (color) ;
      h->SetLineColor (color) ;
    }else if (histoType == 1) { // signal
      h->SetFillStyle (0) ;
      h->SetMarkerColor (color) ;
      h->SetLineColor (color) ;
      h->SetLineWidth (2) ;
      h->SetMarkerStyle (20) ;
    }else if (histoType == 2) { // data
      h->SetFillStyle (0) ;
      h->SetMarkerColor (color) ;
      h->SetLineColor (color) ;
      h->SetLineWidth (2) ;
      h->SetMarkerStyle (20) ;
    }
  h->GetXaxis ()->SetTitle (titleX) ;
  h->GetYaxis ()->SetTitle (titleY) ;
  return h ;
}

