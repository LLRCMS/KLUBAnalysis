#include "plotContainer.h"


plotContainer::plotContainer (string name) :
  m_name   (name),
  m_Nvar   (-1),
  m_N2Dvar (-1) {}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


plotContainer::plotContainer (string name, vector<string> varList, 
                              vector<pair <TString, TCut> > cutList,
                              vector<string> sampleList, 
                              int histosType) :
  m_name     (name),
  m_Nvar     (varList.size ()),
  m_N2Dvar   (0),
  m_Ncut (cutList.size ()),
  m_Nsample (sampleList.size ()), 
  m_histosType (histosType)
{
  vector<pair<string,string>> varList2D (0); // empty vec
  createHistos (varList, varList2D, cutList, sampleList) ;
}


plotContainer::plotContainer (string name, vector<string> varList, vector<pair<string,string>> varList2D,
                vector<pair <TString, TCut> > cutList, vector<string> sampleList, int histosType) :
  m_name   (name),
  m_Nvar   (varList.size ()),
  m_N2Dvar (varList2D.size()),
  m_Ncut (cutList.size ()),
  m_Nsample (sampleList.size ()), 
  m_histosType (histosType)   
{
  createHistos (varList, varList2D, cutList, sampleList) ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void
plotContainer::createHistos (vector<string> varList, vector<pair<string,string>> varList2D,
                             vector<pair <TString, TCut> > cutList,
                             vector<string> sampleList)
{
  
  // for 1D plots
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
              
              bool userBinning = gConfigParser->isDefined (TString ("binning::") + varID.c_str ()) ;
              if (userBinning)
              {
                vector<float> binning = gConfigParser->readFloatListOption (TString ("binning::") + varID.c_str ()) ;
                float* bins = new float [binning.size()];
                for (unsigned int i = 0; i < binning.size(); i++) bins[i] = binning.at(i);
                
                string histoName = m_name + "_" 
                                   + varList.at (ivar) + "_" 
                                   + cutList.at (icut).first.Data () + "_" 
                                   + sampleList.at (isample) ;     
                
                int hcolor = gConfigParser->isDefined (TString ("colors::") + TString (sampleList.at (isample).c_str ()))
                ? gConfigParser->readIntOption (TString ("colors::") + TString (sampleList.at (isample).c_str ()))
                : 1;

                cutDummy[sampleList.at (isample)] = createNewHisto (
                    histoName, histoName,
                    binning.size()-1, bins,
                    hcolor, 
                    m_histosType,
                    varList.at (ivar).c_str (), "events"
                  ) ;
                delete[] bins;
              }
              else
              {
                int hcolor = gConfigParser->isDefined (TString ("colors::") + TString (sampleList.at (isample).c_str ()))
                ? gConfigParser->readIntOption (TString ("colors::") + TString (sampleList.at (isample).c_str ()))
                : 1;
 
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
                    hcolor, 
                    m_histosType,
                    varList.at (ivar).c_str (), "events"
                  ) ;
              }
            }
          varDummy[cutList.at (icut).first.Data ()] = cutDummy ;          
        }
      m_histos[varList.at (ivar)] = varDummy ;
    }

  // for 2D plots
  for (unsigned int ivar = 0 ; ivar < m_N2Dvar ; ++ivar)
    {
      map<string, map<string, TH2F *> > varDummy ;
      for (unsigned int icut = 0 ; icut < m_Ncut ; ++icut)
        {
          map<string, TH2F *> cutDummy ;
          for (unsigned int isample = 0 ; isample < m_Nsample ; ++isample)
            {
              // remove not alphanumeric symbols from the var name
              string varID1 = varList2D.at (ivar).first ;
              string varID2 = varList2D.at (ivar).second ;
              string varID1full = varID1;
              string varID2full = varID2;

              varID1.erase (std::remove_if (varID1.begin (), varID1.end (), isNOTalnum ()), varID1.end ()) ;
              varID2.erase (std::remove_if (varID2.begin (), varID2.end (), isNOTalnum ()), varID2.end ()) ;
              string varID = varID1 + varID2; // compone name of string as is seen by this stupid parser

              int hcolor = gConfigParser->isDefined (TString ("colors::") + TString (sampleList.at (isample).c_str ()))
              ? gConfigParser->readIntOption (TString ("colors::") + TString (sampleList.at (isample).c_str ()))
              : 1;

              // get histo nbins and range
              vector <float> limits = 
                gConfigParser->readFloatListOption (TString ("2Dhistos::") 
                    + varID.c_str ()) ;
              string histoName = m_name + "_" 
                                 + varID1full + varID2full + "_" // I skip the : but keep the rest of the var name
                                 + cutList.at (icut).first.Data () + "_" 
                                 + sampleList.at (isample) ;     
              cutDummy[sampleList.at (isample)] = createNew2DHisto (
                  histoName, histoName,
                  int (limits.at (0)), limits.at (1), limits.at (2), (int)(limits.at (3)), limits.at (4), limits.at (5),
                  hcolor, 
                  m_histosType,
                  varID1full.c_str(), varID2full.c_str()
                ) ;
            }
          varDummy[cutList.at (icut).first.Data ()] = cutDummy ;          
        }
      string varKeyName = varList2D.at (ivar).first + varList2D.at (ivar).second;
      m_2Dhistos[varKeyName] = varDummy ;
    }
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void plotContainer::init (vector<string> varList, 
                          vector<pair <TString, TCut> > cutList, 
                          vector<string> sampleList, int histosType)
{
  m_Nvar = varList.size () ;
  m_N2Dvar = 0 ;
  m_Ncut = cutList.size () ;
  m_Nsample = sampleList.size () ; 
  m_histosType = histosType ;
  
  vector<pair<string,string>> varList2D (0);
  createHistos (varList, varList2D, cutList, sampleList) ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void plotContainer::MergeHistograms(vector<string> mergesampleList, TString mergedName)
{
  for (vars_coll::iterator iVar = m_histos.begin () ;
   iVar != m_histos.end () ;++iVar)
  {
    for (cuts_coll::iterator iCut = iVar->second.begin () ; 
     iCut != iVar->second.end () ;++iCut)
    {
      TH1F *dummyH;//  //var sel sam
      for (uint isamp=0; isamp<mergesampleList.size(); isamp++){
        // if(isamp==0) dummyH = ((TH1F*)m_histos[iVar->first][iCut->first][mergesampleList.at(isamp)]->Clone());
        // else dummyH->Add((TH1F*)m_histos[iVar->first][iCut->first][mergesampleList.at(isamp)]);
        // cout << "NAME: " << mergesampleList.at(isamp) << endl;
        // cout << "ONE: " << endl;
        // cout << "  --> " << &m_histos.at(iVar->first) << endl;
        // cout << "TWO: " << endl;
        // cout << "  --> " << &m_histos.at(iVar->first).at(iCut->first) << endl;
        // cout << "THREE: " << endl;
        // cout << "  --> " << &m_histos.at(iVar->first).at(iCut->first).at(mergesampleList.at(isamp)) << endl;
        // for (auto it = m_histos.at(iVar->first).at(iCut->first).begin(); it !=  m_histos.at(iVar->first).at(iCut->first).end(); ++it)
        //   cout << "ee: " << it->first << endl;

        if(isamp==0) dummyH = ((TH1F*)m_histos[iVar->first][iCut->first].at(mergesampleList.at(isamp))->Clone());
        else dummyH->Add((TH1F*)m_histos[iVar->first][iCut->first].at(mergesampleList.at(isamp)));
      }
      TString histoName = m_name + "_" 
      + iVar->first + "_" 
      + iCut->first + "_" 
      + mergedName;
      dummyH->SetName(histoName.Data());     
      dummyH->SetTitle(histoName.Data());     
      //cout<<"CREATING HISTOGRAM "<<endl<<histoName.Data()<<endl;
      m_histos[iVar->first][iCut->first][mergedName.Data()] = (TH1F*)dummyH->Clone();
      m_Nsample++;
    }
  }
/*
  for (vars_2D_coll::iterator iVar = m_2Dhistos.begin () ;
       iVar != m_2Dhistos.end () ;
       ++iVar)
    {
      for (cuts_2D_coll::iterator iCut = iVar->second.begin () ; 
           iCut != iVar->second.end () ;
           ++iCut)
        {
          for (samples_2D_coll::iterator iSample = iCut->second.begin () ; 
               iSample != iCut->second.end () ;
               ++iSample)
              iSample->second->Scale (scaleFactor) ;
        }
    }
*/
  return ;
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

TH2F *
plotContainer::get2DHisto (string var1Name, string var2Name, string cutName, string sampleName)
{
  string varName = var1Name + var2Name;
  if (m_2Dhistos.find (varName) == m_2Dhistos.end ()) 
    {
      cerr << "no histograms stored in " << m_name 
           << " for variable " << var1Name << ":" << var2Name << endl ;
      return 0 ;
    }
  if (m_2Dhistos[varName].find (cutName) == m_2Dhistos[varName].end ()) 
    {
      cerr << "no histograms stored in " << m_name 
           << " for selection " << cutName << endl ;
      return 0 ;
    }  
  if (m_2Dhistos[varName][cutName].find (sampleName) == m_2Dhistos[varName][cutName].end ()) 
    {
      cerr << "no histograms stored in " << m_name 
           << " for sample " << sampleName << endl ;
      return 0 ;
    }  
  return m_2Dhistos[varName][cutName][sampleName] ;   
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


THStack * 
plotContainer::make2DStack (pair<string,string> var2DName, string cutName)
{
  TString outputName, histoName ;
  outputName.Form ("%s_%s_%s_%s", m_name.c_str (), var2DName.first.c_str (), var2DName.second.c_str (), cutName.c_str ()) ;
  THStack * stack = new THStack (outputName.Data (), outputName.Data ()) ;
  string varKeyName = var2DName.first + var2DName.second;
  
  for (map<string, TH2F *>::iterator iSample = m_2Dhistos[varKeyName][cutName].begin () ;
       iSample != m_2Dhistos[varKeyName][cutName].end () ;
       ++iSample)
    {
      stack->Add (iSample->second) ;
      //cout << "yield: " << iSample->first << " " << iSample->second->Integral(0, 99999) << endl;
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
  if (histoType == 2) h->SetBinErrorOption(TH1::kPoisson); // data
  else h->Sumw2 () ; // MC sig and bkgr (okay, data driven is special but will be overridden by systematics)
  setHistosProperties (h, histoType, color) ;
  h->GetXaxis ()->SetTitle (titleX) ;
  h->GetYaxis ()->SetTitle (titleY) ;
  return h ;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


TH1F * 
plotContainer::createNewHisto (string name, string title, 
                               int nbinsx, float binning[],
                               int color, int histoType,
                               TString titleX, TString titleY)
{
  TH1F* h = new TH1F (name.c_str (), title.c_str (), nbinsx, binning);
  if (histoType == 2) h->SetBinErrorOption(TH1::kPoisson); // data
  else h->Sumw2 () ; // MC sig and bkgr (okay, data driven is special but will be overridden by systematics)
  setHistosProperties (h, histoType, color) ;
  h->GetXaxis ()->SetTitle (titleX) ;
  h->GetYaxis ()->SetTitle (titleY) ;
  return h ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

TH2F *
plotContainer::createNew2DHisto (string name, string title, 
                         int nbinsx, double xlow, double xup,
                         int nbinsy, double ylow, double yup,
                         int color, int histoType,
                         TString titleX, TString titleY)
{
  TH2F* h = new TH2F (name.c_str (), title.c_str (), nbinsx, xlow, xup, nbinsy, ylow, yup);
  if (histoType == 2) h->SetBinErrorOption(TH1::kPoisson); // data
  else h->Sumw2 () ; // MC sig and bkgr (okay, data driven is special but will be overridden by systematics)   
  setHistosProperties (h, histoType, color) ;
  h->GetXaxis ()->SetTitle (titleX) ;
  h->GetYaxis ()->SetTitle (titleY) ;
  return h ;
}
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void 
plotContainer::save (TFile * fOut)
{
  fOut->cd () ;
  
  // 1D
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

  // 2D
  for (vars_2D_coll::iterator iVar = m_2Dhistos.begin () ;
       iVar != m_2Dhistos.end () ;
       ++iVar)
    {
      for (cuts_2D_coll::iterator iCut = iVar->second.begin () ; 
           iCut != iVar->second.end () ;
           ++iCut)
        {
          for (samples_2D_coll::iterator iSample = iCut->second.begin () ; 
               iSample != iCut->second.end () ;
               ++iSample)
              iSample->second->Write () ;
        }
    }
  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void 
plotContainer::scale (float scaleFactor)
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
              iSample->second->Scale (scaleFactor) ;
        }
    }

  for (vars_2D_coll::iterator iVar = m_2Dhistos.begin () ;
       iVar != m_2Dhistos.end () ;
       ++iVar)
    {
      for (cuts_2D_coll::iterator iCut = iVar->second.begin () ; 
           iCut != iVar->second.end () ;
           ++iCut)
        {
          for (samples_2D_coll::iterator iSample = iCut->second.begin () ; 
               iSample != iCut->second.end () ;
               ++iSample)
              iSample->second->Scale (scaleFactor) ;
        }
    }

  return ;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void 
plotContainer::scale (vector<string> & variablesList, vector<pair <TString, TCut> > & selections, vector<vector<float>> scaleFactorVector)
{
  for (unsigned int ivar = 0; ivar < variablesList.size(); ivar++)
  {
    for (unsigned int icut = 0; icut < selections.size(); icut++)
    {
      // loop over all samples
      for (samples_coll::iterator iSample =  (m_histos[variablesList.at (ivar)][selections.at(icut).first.Data ()]).begin () ; 
            iSample != (m_histos[variablesList.at (ivar)][selections.at(icut).first.Data ()]).end () ; 
            ++iSample)
        iSample->second->Scale (scaleFactorVector.at(ivar).at(icut)) ;
    }
  }

  /*
  for (vars_coll::iterator iVar = m_histos.begin () ; iVar != m_histos.end () ; ++iVar)
  {
    for (cuts_coll::iterator iCut = iVar->second.begin () ; iCut != iVar->second.end () ; ++iCut)
    {
      for (samples_coll::iterator iSample = iCut->second.begin () ; iSample != iCut->second.end () ; ++iSample)
      {
        iSample->second->Scale (scaleFactorVector.at(idxvar).at(idxcut)) ;
      }
      idxcut++;
    }
    idxvar++;
  }
  */
  return ;
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void 
plotContainer::scale2D (vector<pair<string,string>> & variables2DList, vector<pair <TString, TCut> > & selections, vector<vector<float>> scaleFactorVector)
{
  for (unsigned int ivar = 0; ivar < variables2DList.size(); ivar++)
  {
    for (unsigned int icut = 0; icut < selections.size(); icut++)
    {
      // loop over all samples
      for (samples_2D_coll::iterator iSample =  (m_2Dhistos[variables2DList.at (ivar).first+variables2DList.at (ivar).second][selections.at(icut).first.Data ()]).begin () ; 
            iSample != (m_2Dhistos[variables2DList.at (ivar).first+variables2DList.at (ivar).second][selections.at(icut).first.Data ()]).end () ; 
            ++iSample)
        iSample->second->Scale (scaleFactorVector.at(ivar).at(icut)) ;
    }
  }

  /*
  for (vars_coll::iterator iVar = m_histos.begin () ; iVar != m_histos.end () ; ++iVar)
  {
    for (cuts_coll::iterator iCut = iVar->second.begin () ; iCut != iVar->second.end () ; ++iCut)
    {
      for (samples_coll::iterator iSample = iCut->second.begin () ; iSample != iCut->second.end () ; ++iSample)
      {
        iSample->second->Scale (scaleFactorVector.at(idxvar).at(idxcut)) ;
      }
      idxcut++;
    }
    idxvar++;
  }
  */
  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void 
plotContainer::setFillColor (int color)
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
              iSample->second->SetFillColor (color) ;
        }
    }
  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


int
plotContainer::addSample (string sampleName, const plotContainer & original) // iHisto = iv + nVars * isel
{
  // shallow check
  if (original.m_Nvar != m_Nvar ||
      original.m_Ncut != m_Ncut) 
    {
      cerr << "the two plot containers don't match in size\n" ;
      exit (1) ;
    }

  // shallow check
  if (original.m_N2Dvar != m_N2Dvar ||
      original.m_Ncut != m_Ncut) 
    {
      cerr << "the two plot containers 2D don't match in size\n" ;
      exit (1) ;
    }

  vars_coll::const_iterator iVarOrig = original.m_histos.begin () ;
  for (vars_coll::iterator iVar = m_histos.begin () ;
       iVar != m_histos.end () ;
       ++iVar, ++iVarOrig)
    {
      cuts_coll::const_iterator iCutOrig = iVarOrig->second.begin () ;
      for (cuts_coll::iterator iCut = iVar->second.begin () ; 
           iCut != iVar->second.end () ;
           ++iCut, ++iCutOrig)
        {
          if (iCutOrig->second.size () != 1)
            {
              cerr << "container " << original.m_name
                   << "does not have a single sample\n" ;
              exit (1) ;
            }
          iCut->second.insert (pair<string, TH1F *> (
              sampleName,
              iCutOrig->second.begin()->second
            )) ;
        }
    }

  vars_2D_coll::const_iterator iVarOrig2D = original.m_2Dhistos.begin () ;
  for (vars_2D_coll::iterator iVar = m_2Dhistos.begin () ;
       iVar != m_2Dhistos.end () ;
       ++iVar, ++iVarOrig2D)
    {
      cuts_2D_coll::const_iterator iCutOrig = iVarOrig2D->second.begin () ;
      for (cuts_2D_coll::iterator iCut = iVar->second.begin () ; 
           iCut != iVar->second.end () ;
           ++iCut, ++iCutOrig)
        {
          if (iCutOrig->second.size () != 1)
            {
              cerr << "container " << original.m_name
                   << "does not have a single sample\n" ;
              exit (1) ;
            }
          iCut->second.insert (pair<string, TH2F *> (
              sampleName,
              iCutOrig->second.begin()->second
            )) ;
        }
    }
  m_Nsample += 1;  
  return 0 ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void plotContainer::setHistosProperties (TH1 * h, int histoType, int color) 
{
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
  return ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void plotContainer::setHistosProperties (int histoType, int color) 
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
              setHistosProperties (iSample->second, histoType, color) ;
        }
    }
  return ;
}

