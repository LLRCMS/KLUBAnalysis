#include "analysisUtils.h"
#include <algorithm>

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
            int maxEvts, TFile* fOut)
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
            maxEvts, fOut);
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
            int maxEvts, TFile* fOut)
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
      float PUReweight ;
      tree->SetBranchAddress ("PUReweight", &PUReweight) ;
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
      vector<int> addressInt (allvars.size(), 0.) ;
      vector<int> indexInt ;
      for (unsigned int iv = 0 ; iv < allvars.size () ; ++iv)
      {
        if(allvars.at(iv)=="njets" || allvars.at(iv)=="npu" || allvars.at(iv)=="npv")
        {
      	  indexInt.push_back(iv);
          tree->SetBranchAddress (allvars.at (iv).c_str (), &(addressInt.at (iv))) ;
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
      tree->SetBranchStatus ("MC_weight", 1) ;
      tree->SetBranchStatus ("PUReweight", 1) ;
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

      // create a vector of ttree*, one for each selection
      std::vector<TTree*> outTrees (selections.size());
      if (fOut != 0)
      {
        for (unsigned int isel = 0; isel < selections.size(); isel++)
        {
          TTree* outTree = 0; // used only if saving events in file <--> if a valid tfile is passed
          fOut->cd();
          TString treeName = Form ("tree_%s_%s", samples.at(iSample).sampleName.Data(), selections.at(isel).first.Data());
          outTree = tree->CloneTree(0);
          outTree->SetName(treeName);
          outTrees.at(isel) = outTree;
          //outTree->SetTitle(treeName);
        }
      }

      //unsigned int nEvts = (maxEvts == -1 ? tree->GetEntries() : maxEvts);
      //nEvts = min (nEvts, tree->GetEntries());
      for (int iEvent = 0 ; iEvent < nEvts ; ++iEvent)
        {
          tree->GetEntry (iEvent) ;
          //if (iEvent%10000 == 0) cout << iEvent << " / " << nEvts << endl;

          float toAdd = PUReweight * weight * lumi * scaling ;

          if (isData) localCounter.counters.at (iSample).at (0) += 1. ;
          else        localCounter.counters.at (iSample).at (0) += toAdd ;
          
          for (unsigned int isel = 0 ; isel < selections.size () ; ++isel)
            {
              if (! TTF[isel]->EvalInstance ()) continue ;

              if (fOut != 0) outTrees.at(isel)->Fill();

              if (isData) localCounter.counters.at (iSample).at (isel+1) += 1. ;
              else        localCounter.counters.at (iSample).at (isel+1) += toAdd ;
              
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
                      if( (std::find( indexInt.begin(), indexInt.end(), (int)iv) != indexInt.end() ) )
                      histo->Fill (addressInt[iv]) ; 
		                  else {
                       histo->Fill (address[iv]);
                      }
                  }
                  else        
                  {
                      if( (std::find( indexInt.begin(), indexInt.end(), (int)iv) != indexInt.end() ) )
                      histo->Fill (addressInt[iv], toAdd) ; 
                      else {
                       histo->Fill (address[iv], toAdd);
                      }
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
                  if ( ( std::find( indexInt.begin(), indexInt.end(), idx1) != indexInt.end() ) ) val1 = (float) addressInt[idx1];
                  if ( ( std::find( indexInt.begin(), indexInt.end(), idx1) != indexInt.end() ) ) val2 = (float) addressInt[idx2];
                  if (isData) histo->Fill (val1, val2) ;
                  else histo->Fill (val1, val2, toAdd) ;
                } //loop on 2Dvariables                
            } //loop on selections
        } //loop on tree entries

      for (unsigned int isel = 0 ; isel < selections.size () ; ++isel)
      {
        delete TTF[isel] ;
        if (fOut != 0)
        {
          fOut->cd();
          outTrees.at(isel)->Write();
        }
      }
      
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


// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---0
std::vector<TObject*> makeStackPlot (plotContainer& dataPlots, plotContainer& bkgPlots, plotContainer& sigPlots,
                                      string varName, string selName,
                                      TCanvas* canvas, std::vector <pair <string, string> >& addInLegend, std::vector <pair <string, string> >& axisTitles,
                                      bool LogY, bool makeRatioPlot, bool drawLegend, bool doShapes, bool forceNonNegMin, bool drawGrassForData,
                                      bool drawSignal, bool drawData, bool drawMC)
{
  const int axislabelsize   = 18;  // title of axis
  const int axisnumsize     = 14;  // numbers on axis
  const int axisnumberfont  = 43;  // font of numbres on axis (helvetica)
  const int axistitlefont   = 43;  // font of labels of axis (helvetica)
  const int cmsTextFont     = 61;  // font of the "CMS" label
  const float cmsTextSize   = 0.04;  // font size of the "CMS" label
  const int extraTextFont   = 52;     // for the "preliminary"
  const float extraTextSize   = 0.76 * cmsTextSize; // for the "preliminary"


  if (! (drawData && drawMC) ) makeRatioPlot = false; // as ratio is data/MC
  if (doShapes)                makeRatioPlot = false;
  if (LogY)                    forceNonNegMin = false; // the min finder already thinsk about it if LOG
  if (! (drawSignal || drawData || drawMC) )
  {
    cout << " ** analysisUtils::makeStackPlot: qualcosa lo devi pur disegnare, enabling MC+data+sig" << endl;
    drawSignal = drawData = drawMC = true;
  }  
  //if (doShapes) drawSignal = true; // FIXME: enable shapes with bkgr only

  std::vector<TObject*> allocatedStuff;

  canvas->Clear();
  canvas->cd();
  
  THStack * sig_stack =  (drawSignal ? sigPlots.makeStack ( varName, selName )  : 0);
  THStack * bkg_stack =  (drawMC     ? bkgPlots.makeStack ( varName, selName )  : 0);
  THStack * DATA_stack = (drawData   ? dataPlots.makeStack ( varName, selName ) : 0);

  vector<float> extremes_bkg; 
  vector<float> extremes_sig; 
  vector<float> extremes_DATA;
  
  if (drawMC)     extremes_bkg = getExtremes (bkg_stack, LogY) ;
  if (drawSignal) extremes_sig = getExtremes (sig_stack, LogY) ;
  if (drawData)   extremes_DATA = getExtremes (DATA_stack, LogY) ;
  
  // fill all vectors with dummy values to avoid many other "if" everywhere
  vector<float> vZeroes (4, 0.);
  if (extremes_bkg.size() == 0) extremes_bkg.insert (extremes_bkg.begin(),    vZeroes.begin(), vZeroes.end());
  if (extremes_sig.size() == 0) extremes_sig.insert (extremes_sig.begin(),    vZeroes.begin(), vZeroes.end());
  if (extremes_DATA.size() == 0) extremes_DATA.insert (extremes_DATA.begin(), vZeroes.begin(), vZeroes.end());

  if (drawSignal) allocatedStuff.push_back (sig_stack);
  if (drawMC)     allocatedStuff.push_back (bkg_stack);
  if (drawData)   allocatedStuff.push_back (DATA_stack);

  // ------------  compute boundaries for main plotpad ---------------

  TPad *pad1; // upper pad if ratio plot
  
  if (makeRatioPlot)
  {
    pad1 = new TPad("pad1", "pad1", 0, 0.25, 1, 1.0);
    pad1->SetBottomMargin(0.02); // Upper and lower plot are joined
    //pad1->SetGridx();         // Vertical grid
  }
  
  else
  {
    pad1 = new TPad("pad1", "pad1", 0, 0.0, 1, 1.0);
    //pad1->SetBottomMargin(0); // Upper and lower plot are joined
    //pad1->SetGridx();         // Vertical grid
  }
  pad1->SetTopMargin(0.06);
  pad1->SetRightMargin(0.035);

  allocatedStuff.push_back (pad1);

  if (LogY) pad1->SetLogy(true);
  else pad1->SetLogy(false);

  pad1->Draw();  // Draw the upper pad: pad1
  pad1->cd();    // pad1 becomes the current pad

  float minx;
  float maxx;
  
  if (drawMC)
  {
    minx = extremes_bkg.at (0);
    maxx = extremes_bkg.at (2) ; // all plots have same range for a certain variable
  }
  else if (drawSignal)
  {
    minx = extremes_sig.at (0);
    maxx = extremes_sig.at (2) ; // all plots have same range for a certain variable
  }
  else if (drawData)
  {
    minx = extremes_DATA.at (0);
    maxx = extremes_DATA.at (2) ; // all plots have same range for a certain variable
  }

  float miny; 
  float maxy; 

  TH1F * shape_bkg = 0; // use only id doShapes
  THStack * hstack_bkg_norm = 0;
  THStack * hstack_sig_norm = 0;
  //THStack * hstack_data_norm = 0;
  //TH1F * hshape_data = 0;
  float scaleData = 1.0;

  if (!doShapes)
  {
    miny = min3Select (extremes_bkg.at (1), extremes_sig.at (1), extremes_DATA.at (1), drawMC, drawSignal, drawData) ;
    maxy = max3Select (extremes_bkg.at (3), extremes_sig.at (3), extremes_DATA.at (3) + sqrt (extremes_DATA.at (3)), drawMC, drawSignal, drawData);
 
    // compute the upper space in log scale
    if (LogY)
    {
      //leave a 0.3 of space for legend
      float rangeInLog = (log10 (maxy) - log10 (miny));
      float lymax = log10(maxy) + 0.3*rangeInLog;
      maxy += pow(10, lymax);
      miny = miny / 5. ;
    }
    else
    {
      miny *= 0.9;
      maxy *= 1.3;
    }
  }

  else // no data
  {
    hstack_bkg_norm = normaliseStack (bkg_stack) ;
    shape_bkg = (TH1F *) hstack_bkg_norm->GetStack ()->Last () ;
    allocatedStuff.push_back(hstack_bkg_norm);

    hstack_sig_norm = normaliseStack (sig_stack, true) ;
    vector<float> extremes_sig_norm = getExtremes (hstack_sig_norm, LogY, true) ;

    //hstack_data_norm = normaliseStack (DATA_stack) ;
    //hshape_data = (TH1F*) hstack_data_norm->GetStack () ->Last() ;
    TH1F* histoDATA = (TH1F*) DATA_stack->GetStack () ->Last();
    scaleData = histoDATA->Integral();
    float dataShapeMax = (histoDATA->GetMaximum() + sqrt (histoDATA->GetMaximum()))/scaleData;
    float dataShapeMin = (histoDATA->GetMinimum())/scaleData;
    /*
    hshape_data -> SetMarkerStyle (8);
    hshape_data -> SetMarkerSize (1.);
    hshape_data -> SetMarkerColor (kBlack);
    */

    //shape_sig = (TH1F *) hstack_sig_norm->GetStack ()->Last () ;
    //allocatedStuff.push_back(hstack_sig_norm);

    if (LogY)
    {
      float tmpMin = min3Select ((float)shape_bkg->GetMinimum(0), extremes_sig_norm.at(1), dataShapeMin, drawMC, drawSignal, drawData);
      float tmpMax = max3Select ((float)shape_bkg->GetMaximum(), extremes_sig_norm.at(3), dataShapeMax, drawMC, drawSignal, drawData);
      //leave a 0.3 of space for legend
      float rangeInLog = (log10 (tmpMax) - log10 (tmpMin));
      float lymax = log10(tmpMax) + 0.3*rangeInLog;   
      maxy = pow(10, lymax);
      miny = tmpMin / 5. ;
    }
    else
    {

      //cout << "DDD: " << (float)shape_bkg->GetMaximum() << " " << extremes_sig_norm.at(3) << " " << dataShapeMax << " || " <<  drawMC << " " << drawSignal << " " << drawData  << " ---> " << max3Select ((float)shape_bkg->GetMaximum(), extremes_sig_norm.at(3), dataShapeMax, drawMC, drawSignal, drawData) << endl;
      miny = 0.9*min3Select ((float)shape_bkg->GetMinimum(), extremes_sig_norm.at(1), dataShapeMin, drawMC, drawSignal, drawData);
      maxy = 1.3*max3Select ((float)shape_bkg->GetMaximum(), extremes_sig_norm.at(3), dataShapeMax, drawMC, drawSignal, drawData);
    }
  }

  if (forceNonNegMin && miny < 0) miny = 0;
  
  // ------------- prepare and draw frame -----

  TH1F * frame = pad1->DrawFrame ( minx, miny, maxx, maxy ); // do not add it to the list of objects to delete! Or it will cause a segmentation fault
  copyTitles (frame, bkg_stack) ;

  // re-draw correctly y axis as it will be overlapped by the histos
  //frame->GetYaxis()->SetLabelSize(0);
  //frame->GetYaxis()->SetTickSize(0);
  frame->GetYaxis()->SetLabelSize(axisnumsize);
  frame->GetYaxis()->SetLabelFont(axisnumberfont);
  frame->GetYaxis()->SetTitleSize(axislabelsize);
  frame->GetYaxis()->SetTitleFont(axistitlefont);
  frame->GetYaxis()->SetTitleOffset(1.2);
  
  // frame->GetXaxis()->SetTickSize(0);

  TString xTitle = frame->GetXaxis()->GetTitle();
  string xTitlestr (xTitle.Data());
  auto it = std::find_if(axisTitles.begin(), 
                   axisTitles.end(), 
                  [&xTitlestr](const pair<string, string>& p)
                  { return p.first == xTitlestr; });

  if (it != axisTitles.end()) xTitle = (it->second).c_str();
 

  if (makeRatioPlot)
  {
    frame->GetXaxis()->SetLabelSize(0);
    frame->GetXaxis()->SetTitle("");
  }
  else
  {
    frame->GetXaxis()->SetLabelSize(axisnumsize);
    frame->GetXaxis()->SetTitleFont(axistitlefont);
    frame->GetXaxis()->SetTitleSize(axislabelsize);
    frame->GetXaxis()->SetTitleOffset(1.1);
    frame->GetXaxis()->SetLabelFont(axisnumberfont); // Absolute font size in pixel (precision 3)
    frame->GetXaxis()->SetTitle(xTitle);
  } 

  if (doShapes)
    frame->GetYaxis()->SetTitle ("a.u.") ;    

  frame->Draw () ;

  // -------------------- draw the plot(s) ----------

  TH1F * h_data = 0 ;
  TH1F * h_bkg  = 0;
  if (!doShapes)
  {
    // bkg_stack->SetMinimum (ymin);
    // bkg_stack->SetMaximum (ymax);
    // bkg_stack->Draw ("hist") ;
    if (drawMC)     bkg_stack->Draw ("hist same") ;
    if (drawSignal) sig_stack->Draw ("nostack hist same") ; // should be safe for plot limits
    h_bkg = (TH1F *) bkg_stack->GetStack ()->Last () ; // FIXME: is it allocated with new and needs to be deleted? stupid ROOT!
    if (drawData)
    {
      h_data = (TH1F *) DATA_stack->GetStack ()->Last () ; // FIXME: is it allocated with new and needs to be deleted? stupid ROOT!
      h_data->Sumw2(false);
      h_data->SetBinErrorOption(TH1::kPoisson);
      TGraphAsymmErrors* gData = makeDataGraphPlot (h_data, false, drawGrassForData);
      allocatedStuff.push_back(gData);
      gData->Draw ("P Z same") ;
    }
  }
  else
  {
    // hstack_bkg_norm->SetMinimum (ymin);
    // hstack_bkg_norm->SetMaximum (ymax);
    /*
    TIter next (hstack_bkg_norm->GetHists ()) ;
    TH1F * histo ;
    while ((histo = (TH1F *) (next ()))) 
       histo->SetFillStyle(0);
    */
    if (drawMC) hstack_bkg_norm -> Draw ("hist same") ;
    if (drawSignal) hstack_sig_norm->Draw ("nostack hist same") ;
    if (drawData)
    {
      h_data = (TH1F *) DATA_stack->GetStack ()->Last () ; // FIXME: is it allocated with new and needs to be deleted? stupid ROOT!
      h_data->Sumw2(false);
      h_data->SetBinErrorOption(TH1::kPoisson);
      TGraphAsymmErrors* gData = makeDataGraphPlot (h_data, false, drawGrassForData);
      allocatedStuff.push_back(gData);
      scaleDataGraph (gData, 1./scaleData);
      gData->Draw ("P Z same") ;
    }
  }

  // axis will be covered by the plots so redraw axis!
  pad1->RedrawAxis();
  pad1->RedrawAxis("g"); // for the grid, if enabled

  // ------------------- legend  ----------------
  if (drawLegend)
  {
    TLegend* leg = new TLegend (0.3, 0.77, 0.95, 0.94);
    leg->SetFillColor(kWhite);
    leg->SetFillStyle(0);
    leg->SetLineWidth(0);
    leg->SetBorderSize(0);
    // let's consider that 4 sample names can stay in the top legend
    leg->SetNColumns(4);
      
    allocatedStuff.push_back(leg);

    // sample names -- bkg
    for (map<string, TH1F *>::iterator iSample = bkgPlots.m_histos[varName][selName].begin () ;
        iSample != bkgPlots.m_histos[varName][selName].end () && drawMC; ++iSample)
    {
      string samplename = iSample->first ;
      string thisname = "EMPTY";

      auto it = std::find_if(addInLegend.begin(), 
                       addInLegend.end(), 
                      [&samplename](const pair<string, string>& p)
                      { return p.first == samplename; });

      if (it != addInLegend.end()) thisname = it->second;
      else thisname = samplename;
      
      if (thisname != string ("NODRAW") ) leg->AddEntry (iSample->second, thisname.c_str(), "f") ;
    }    

    // sample names -- signal
    for (map<string, TH1F *>::iterator iSample = sigPlots.m_histos[varName][selName].begin () ;
        iSample != sigPlots.m_histos[varName][selName].end () && drawSignal; ++iSample)
    {
      string samplename = iSample->first ;
      string thisname = "EMPTY";

      auto it = std::find_if(addInLegend.begin(), 
                       addInLegend.end(), 
                      [&samplename](const pair<string, string>& p)
                      { return p.first == samplename; });

      if (it != addInLegend.end()) thisname = it->second;
      else thisname = samplename;      
      if (thisname != string ("NODRAW") ) leg->AddEntry (iSample->second, thisname.c_str(), "l") ;

    }

    if (drawData)
    {
      //TH1F* dataHisto = dataPlots.getHisto(varName, selName, addInLegend.at(i).first );      
      map<string, TH1F *>::iterator iData = dataPlots.m_histos[varName][selName].begin() ;  // all data have the same format!
      leg->AddEntry (iData->second, "data", "lep") ;
    }        
    leg->Draw();
  }

  // redo y axis that is overlapped
  /*
  TGaxis *axis;
  if (!LogY) axis = new TGaxis( minx, miny, minx, maxy, miny, maxy, 510, "");
  else axis = new TGaxis( minx, miny, minx, maxy, miny, maxy, 510, "G");
  allocatedStuff.push_back(axis);
  axis->SetLabelFont(axisnumberfont); // Absolute font size in pixel (precision 3)
  axis->SetLabelSize(axisnumsize);
  axis->Draw();

  // redo x axis that is overlapped
  TGaxis *x_axis;
  x_axis = new TGaxis( minx, miny, maxx, miny, minx, maxx, 510, "");
  allocatedStuff.push_back(x_axis);
  x_axis->SetLabelFont(axisnumberfont); // Absolute font size in pixel (precision 3)
  x_axis->SetLabelSize(axisnumsize);
  x_axis->Draw();
  */

  // ------------ ratio plot --------------------

  if (makeRatioPlot)
  {

    // lower plot will be in pad
    canvas->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 0.2496);
    allocatedStuff.push_back(pad2);
    pad2->SetTopMargin(0.05);
    pad2->SetBottomMargin(0.35);
    pad2->SetRightMargin(0.035);
    pad2->SetGridy();
    //pad2->SetGridx(); // vertical grid
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad

    // Define the ratio plot
    TH1F *hratio = (TH1F*)h_data->Clone (Form("hratio_%s_%s", varName.c_str(), selName.c_str()));
    allocatedStuff.push_back (hratio);
    
    hratio->SetLineColor(kBlack);
    hratio->SetMinimum(0.501);  // Define Y ..
    hratio->SetMaximum(1.499); // .. range
    hratio->SetTitle("");
    
    hratio->GetXaxis()->SetTitleSize(axislabelsize);
    hratio->GetXaxis()->SetTitleFont(axistitlefont);
    hratio->GetXaxis()->SetTitleOffset(3.5);
    hratio->GetXaxis()->SetLabelFont(axisnumberfont); // Absolute font size in pixel (precision 3)
    hratio->GetXaxis()->SetLabelSize(axisnumsize);
    hratio->GetXaxis()->SetTitle (xTitle);
    
    hratio->GetYaxis()->SetTitle ("data / MC");    
    hratio->GetYaxis()->SetNdivisions(505);
    hratio->GetYaxis()->SetTitleSize(axislabelsize);
    hratio->GetYaxis()->SetTitleFont(axistitlefont);
    hratio->GetYaxis()->SetTitleOffset(1.2);
    hratio->GetYaxis()->SetLabelFont(axisnumberfont); // Absolute font size in pixel (precision 3)
    hratio->GetYaxis()->SetLabelSize(axisnumsize);

    //hratio->Sumw2(); // should inherit from bkg_stack
    hratio->SetStats(0);      // No statistics on lower plot
    hratio->SetMarkerStyle (8);
    hratio->SetMarkerSize (0.8);

    /*
    //hratio->Divide(h_bkg); // this will inherit errors from bkg too and I don't want it
    for (int ibin = 1; ibin <= hratio->GetNbinsX(); ibin++)
    {
      float binContent = hratio->GetBinContent(ibin);
      float binError   = hratio->GetBinError(ibin);
      float divBy      = h_bkg->GetBinContent(ibin);
      if (divBy > 0)
      {
        hratio->SetBinContent (ibin, binContent / divBy);
        hratio->SetBinError   (ibin, binError / divBy);
      }
      else
      {
        hratio->SetBinContent (ibin, 0);
        hratio->SetBinError   (ibin, 0);        
      }
    }
    */
    TGraphAsymmErrors* gRatio = makeDataOverMCRatioPlot (h_data, h_bkg, false);
    allocatedStuff.push_back(gRatio);
    hratio->Draw("axis");       // axis only
    gRatio->Draw("p z same"); // z: no small lines at the end of the bar
                                // 0: draw error lines for points outside the plot range
    pad2->RedrawAxis();
    pad2->RedrawAxis("g");

    /*
    TLine* line = new TLine (minx, 1. , h2->GetBinLowEdge(h2->GetNbinsX()+1), 1.);
    allocatedStuff.push_back(line);
    line->SetLineColor(kGray+2);
    line->SetLineStyle (7);
    line->Draw();
    */
  }

  // ---------------- cms text --------------

  // now do header with luminosity and CMS preliminary  
  pad1->cd();
  TString CMStext = "CMS";
  TString extraText = "preliminary";

  /*
  float yoffs = 0.25; // fractional shift
  // TLatex want units as the axes! So I have to rescale...
  float yrange = maxy - miny;
  float xpos = 0.05* (maxx - minx) + minx;
  float ypos = 0.95*yrange + miny;
  ///cout << "X pos: " << xpos << endl;
  //cout << "Y pos: " << ypos << endl;
  */
  float xpos  = 0.13;
  float ypos  = 0.925;
  float yoffs = 0.05; // fractional shift

  TLatex* CMSbox       = new TLatex  (xpos, ypos         , CMStext.Data());       
  TLatex* extraTextBox = new TLatex  (xpos, ypos - yoffs , extraText.Data());
  CMSbox->SetNDC();
  extraTextBox->SetNDC();
  allocatedStuff.push_back (CMSbox);  
  allocatedStuff.push_back (extraTextBox);  

  CMSbox->SetTextSize(cmsTextSize);
  CMSbox->SetTextFont(cmsTextFont);
  CMSbox->SetTextColor(kBlack);
  CMSbox->SetTextAlign(13);

  extraTextBox->SetTextSize(extraTextSize);
  extraTextBox->SetTextFont(extraTextFont);
  extraTextBox->SetTextColor(kBlack);
  extraTextBox->SetTextAlign(13);

  CMSbox->Draw();
  extraTextBox->Draw();

  // now plot lumi!
  float lumi_num = gConfigParser->readFloatOption ("general::lumi");
  lumi_num = lumi_num/1000.; // is in pb-1
  stringstream stream;
  stream << fixed << setprecision(1) << lumi_num;
  TString lumi = stream.str().c_str();
  lumi += " fb^{-1} (13 TeV)";

  xpos = 0.963;
  ypos = 0.962;
  TLatex* lumibox = new TLatex  (xpos, ypos, lumi.Data());       
  allocatedStuff.push_back(lumibox);
  lumibox->SetNDC();
  lumibox->SetTextAlign(31);
  lumibox->SetTextSize(extraTextSize);
  lumibox->SetTextFont(42);
  lumibox->SetTextColor(kBlack);

  lumibox->Draw();

  return allocatedStuff;

}



