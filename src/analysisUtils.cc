#include "analysisUtils.h"

using namespace std ;


vector<pair <TString, TCut> >
addSelection (vector<pair <TString, TCut> > m_cuts, string cut, string tag)
{
  vector<pair <TString, TCut> > output = m_cuts ;
  for (unsigned int i = 0 ; i < output.size () ; ++i)
    {
      output.at (i).first = TString (tag.c_str ()) + output.at (i).first ;
      output.at (i).second = output.at (i).second && TCut (cut.c_str ()) ;
    }
  return output ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


std::pair<int, int> leptonsType (int pairType)
{
  if (pairType == 0) return pair<int,int> (0, 2) ;
  if (pairType == 1) return pair<int,int> (1, 2) ;
  if (pairType == 2) return pair<int,int> (2, 2) ;
  if (pairType == 3) return pair<int,int> (0, 0) ;
  if (pairType == 4) return pair<int,int> (1, 1) ;
  if (pairType == 5) return pair<int,int> (1, 0) ; // FIXME are they ordered per flavour?
  if (pairType == 6) return pair<int,int> (1, 1) ;
  if (pairType == 7) return pair<int,int> (0, 0) ;
  return pair<int,int> (-1, -1) ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


bool isIsolated (int leptonType, float threshold, float isoDeposits, float pT)
{
  if (leptonType == 0 || leptonType == 1) return (isoDeposits/pT < threshold) ;
  if (leptonType == 2) return (isoDeposits < threshold) ;
  return false ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


void
addHistos (vector<sample> & samples, 
           HistoManager * manager,
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
fillHistos (vector<sample> & samples, 
            plotContainer & plots,
            vector<string> & variablesList,
            vector<pair <TString, TCut> > & selections,
            float lumi,
            const vector<float> & scale,
            bool isData,
            bool isSignal,
            int maxEvts)
{
    vector<pair<string,string>> variables2DList (0);
    return fillHistos (
            samples, 
            plots,
            variablesList,
            variables2DList,
            selections,
            lumi,
            scale,
            isData,
            isSignal,
            maxEvts);
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

counters
fillHistos (vector<sample> & samples, 
            plotContainer & plots,
            vector<string> & variablesList,
            vector<pair<string,string>> & variables2DList,
            vector<pair <TString, TCut> > & selections,
            float lumi,
            const vector<float> & scale,
            bool isData,
            bool isSignal,
            int maxEvts)
{
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
        //TTF[isel] -> Compile();
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


      // find how many variables needed for 2D are not in the list of the 1D vars
      vector<string> allvars (variablesList);
      vector<pair<int,int>> var2Didxmap; // this maps each entry of 2Dvarlist to allvars to be more efficient in retrieving values

      std::vector<string>::iterator it;
      for (unsigned int ivar = 0; ivar < variables2DList.size(); ivar++)
      {
        int ind1 = -1;
        int ind2 = -1;

        // var 1 (x)
        string var1 = variables2DList.at(ivar).first;
        it = find (allvars.begin(), allvars.end(), var1);
        if ( it == allvars.end() )
        {
          allvars. push_back(var1);
          ind1 = allvars.size() -1 ;
        }
        else
          ind1 = distance (allvars.begin(), it);

        // var 2 (y)
        string var2 = variables2DList.at(ivar).second;
        it = find (allvars.begin(), allvars.end(), var2);
        if ( it == allvars.end() )
        {
          allvars. push_back(var2);
          ind2 = allvars.size() -1 ;
        }
        else
          ind2 = distance (allvars.begin(), it);

        // save indexes
        var2Didxmap.push_back (make_pair(ind1, ind2));
      }

      // set tbranch addresses for 1D histos
      vector<float> address (allvars.size(), 0.) ;
      int tempnjets;
      int indexNjets = -1;
      for (unsigned int iv = 0 ; iv < allvars.size () ; ++iv)
      {
      	if(allvars.at(iv)=="njets")
        {
      	  tree->SetBranchAddress (allvars.at (iv).c_str (), &tempnjets) ;
      	  indexNjets=iv;
      	}
        else tree->SetBranchAddress (allvars.at (iv).c_str (), &(address.at (iv))) ;
      }

      // FIXME! not good for negative weights as yield does not scale as this factor
      int nEvts = tree->GetEntries();
      if (maxEvts > 0)
      {
        nEvts = min(nEvts, maxEvts);
        scaling *= (1.*tree->GetEntries() / nEvts); // scales if nEvts != entries in the tree
      }

      tree->SetBranchStatus ("*", 0);
      TObjArray *branchList = tree->GetListOfBranches();
      int nBranch   = tree->GetNbranches();
      // used vars
      for (unsigned int ivar = 0; ivar < allvars.size(); ivar++)
      {
        tree->SetBranchStatus (allvars.at(ivar).c_str(), 1);
      }
      // vars used in selection
      for (int iB = 0 ; iB < nBranch; iB++)
      {
         TString bName = branchList->At(iB)->GetName();
         for (unsigned int iSel = 0; iSel < selections.size(); iSel++)
         {
           TString theCut = selections.at(iSel).second.GetTitle(); // gives the content of tCut as char*
           if (theCut.Contains (bName))
           {
             tree->SetBranchStatus (bName, 1);
           }
         }
      }

      // create an array of histos to avoid retrieving them each time and speed up
      const int nVars = variablesList.size ();
      const int nSele = selections.size ();
      // histoArray: [iSel][iVar]
      TH1F *** histoArray = new TH1F** [nSele];
      for (int iSel = 0 ; iSel < nSele; iSel++)
      {
        histoArray [iSel] = new TH1F* [nVars];
        for (int iVar = 0; iVar < nVars; iVar++)
        {
          histoArray [iSel][iVar] = plots.getHisto (variablesList.at (iVar),
                      selections.at (iSel).first.Data (),
                      samples.at (iSample).sampleName.Data ()
                    ) ;
        }
      }

      //unsigned int nEvts = (maxEvts == -1 ? tree->GetEntries() : maxEvts);
      //nEvts = min (nEvts, tree->GetEntries());
      for (int iEvent = 0 ; iEvent < nEvts ; ++iEvent)
        {
          tree->GetEntry (iEvent) ;
          //if (iEvent%10000 == 0) cout << iEvent << " / " << nEvts << endl;

          if (isData) localCounter.counters.at (iSample).at (0) += 1. ;
          else        localCounter.counters.at (iSample).at (0) 
                          += weight * lumi * scaling ;
          for (unsigned int isel = 0 ; isel < selections.size () ; ++isel)
            {
              if (! TTF[isel]->EvalInstance ()) continue ;

              if (isData) localCounter.counters.at (iSample).at (isel+1) += 1. ;
              else        localCounter.counters.at (iSample).at (isel+1) 
                              += weight * lumi * scaling ;

              
              // fill 1D histos
              for (unsigned int iv = 0 ; iv < variablesList.size () ; ++iv)
                {
                  /*
                  TH1F * histo = 
                  plots.getHisto (variablesList.at (iv),
                      selections.at (isel).first.Data (),
                      samples.at (iSample).sampleName.Data ()
                    ) ;
                  */
                  TH1F * histo = histoArray [isel][iv];
                  
                  if (isData) 
		              {
                      if(iv!=indexNjets)histo->Fill (address[iv]) ; // perfectly fine as address is <=> varList in the first part
		                  else histo->Fill (tempnjets) ;
                  }
                  else        
                  {
                      if(iv!=indexNjets)histo->Fill (address[iv], weight * lumi * scaling) ;
		                  else histo->Fill (tempnjets, weight * lumi * scaling) ;
                  }
                } //loop on 1Dvariables

              // fill 2D histos
              for (unsigned int iv = 0 ; iv < variables2DList.size () ; ++iv)
                {
                  TH2F * histo = 
                  plots.get2DHisto (variables2DList.at (iv).first, variables2DList.at (iv).second,
                      selections.at (isel).first.Data (),
                      samples.at (iSample).sampleName.Data ()
                    ) ;
                  

                  int idx1 = var2Didxmap.at(iv).first;
                  int idx2 = var2Didxmap.at(iv).second;
                  float val1 = address[idx1];
                  float val2 = address[idx2];
                  if (idx1 == indexNjets) val1 = (float) tempnjets;
                  if (idx2 == indexNjets) val2 = (float) tempnjets;
                  if (isData) histo->Fill (val1, val2) ;
                  else histo->Fill (val1, val2, weight * lumi * scaling) ;
                } //loop on 2Dvariables
                

            } //loop on selections
        } //loop on tree entries

      for (unsigned int isel = 0 ; isel < selections.size () ; ++isel) delete TTF[isel] ;

      
      for (int iSel = 0 ; iSel < nSele; iSel++)
          delete [] histoArray [iSel];
      delete [] histoArray;
      

      // re-set all branches to active
      tree->SetBranchStatus ("*", 1);
    
    } // Loop on the MC samples

  return localCounter ;
}


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
/*
counters
makeCounter (plotContainer & plots, float initEff = 1.0, float initTotNum = 0.0)
{
  counters localCounter ;
  for (unsigned int iSample = 0 ; iSample < plots.m_Nsample ; ++iSample)
  {
    localCounter.initEfficiencies.push_back (initEff) ; // plotContainer has no memory of initial eff, set from input!
    localCounter.counters.push_back (vector<float> (selections.size () + 1, 0.)) ;
    localCounter.counters.at(iSample).at(0) = initTotNum; // also, plot container does not know about Nev before 1st selection
    for (unsigned int isel = 0 ; isel < plots.m_Ncut ; ++isel)
    {
      localCounter.counters.at(iSample).at(isel) = (plots.m_histos.begin())->; // take first variable
    }
  }
}
*/
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


