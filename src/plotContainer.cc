#include "plotContainer.h"


plotContainer::plotContainer (string name) :
  m_name (name),
  m_Nvar (-1) {}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


plotContainer::plotContainer (string name, vector<string> varList, 
                              vector<pair <TString, TCut> > cutList,
                              vector<string> sampleList, 
                              int histosType) :
  m_name (name),
  m_Nvar (varList.size ()),
  m_Ncut (cutList.size ()),
  m_Nsample (sampleList.size ()), 
  m_histosType (histosType)
{
  createHistos (varList, cutList, sampleList) ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void
plotContainer::createHistos (vector<string> varList, 
                             vector<pair <TString, TCut> > cutList,
                             vector<string> sampleList)
{
  for (unsigned int ivar = 0 ; ivar < m_Nvar ; ++ivar)
    {
      map<string, map<string, TH1F *> > varDummy ;
      for (unsigned int icut = 0 ; icut < m_Ncut ; ++icut)
        {
          map<string, TH1F *> cutDummy ;
          for (unsigned int isample = 0 ; isample < m_Nsample ; ++isample)
            {
              // remove not alphanumeric symbols from the var name
              string varID = varList.at (ivar) ;
              varID.erase (std::remove_if (varID.begin (), varID.end (), isNOTalnum ()), varID.end ()) ;
              // get histo nbins and range
              vector <float> limits = 
                gConfigParser->readFloatListOption (TString ("histos::") 
                    + varID.c_str ()) ;
              string histoName = m_name + "_" 
                                 + varList.at (ivar) + "_" 
                                 + cutList.at (icut).first.Data () + "_" 
                                 + sampleList.at (isample) ;     
              cutDummy[sampleList.at (isample)] = createNewHisto (
                  histoName, histoName,
                  int (limits.at (0)), limits.at (1), limits.at (2),
                  gConfigParser->readIntOption (TString ("colors::") 
                      + TString (sampleList.at (isample).c_str ())), 
                  m_histosType,
                  varList.at (ivar).c_str (), "events"
                ) ;
            }
          varDummy[cutList.at (icut).first.Data ()] = cutDummy ;          
        }
      m_histos[varList.at (ivar)] = varDummy ;
    }
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void plotContainer::init (vector<string> varList, 
                          vector<pair <TString, TCut> > cutList, 
                          vector<string> sampleList, int histosType)
{
  m_Nvar = varList.size () ;
  m_Ncut = cutList.size () ;
  m_Nsample = sampleList.size () ; 
  m_histosType = histosType ;
  createHistos (varList, cutList, sampleList) ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


TH1F * 
plotContainer::getHisto (string varName, string cutName, string sampleName)
{
  if (m_histos.find (varName) == m_histos.end ()) 
    {
      cerr << "no histograms stored in " << m_name 
           << " for variable " << varName << endl ;
      return 0 ;
    }
  if (m_histos[varName].find (cutName) == m_histos[varName].end ()) 
    {
      cerr << "no histograms stored in " << m_name 
           << " for selection " << cutName << endl ;
      return 0 ;
    }  
  if (m_histos[varName][cutName].find (sampleName) == m_histos[varName][cutName].end ()) 
    {
      cerr << "no histograms stored in " << m_name 
           << " for sample " << sampleName << endl ;
      return 0 ;
    }  
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


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void 
plotContainer::save (TFile * fOut)
{
  fOut->cd () ;
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
              iSample->second->Write () ;
        }
    }
  return ;
}
