#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <cctype>
#include <algorithm>
#include "TString.h"
#include "TChain.h"
#include "TCut.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TF2.h"
#include "TGraph2DErrors.h"

#include "HistoManager.h"
#include "ConfigParser.h"
#include "utils.h"
#include "histoUtils.h"
#include "plotContainer.h"
#include "analysisUtils.h"
#include "TRandom3.h"

using namespace std ;

// ----------------------------------------------------------------------------------------

int main (int argc, char** argv)
{
    // check number of inpt parameters
    if (argc < 2)
    {
        cerr << "Forgot to put the cfg file --> exit " << endl ;
        return 1 ;
    }
    if (gConfigParser) return 1 ;
    gConfigParser = new ConfigParser () ;
    
    TString config ; 
    config.Form ("%s",argv[1]) ;
    if (! (gConfigParser->init (config)))
    {
        cout << ">>> parseConfigFile::Could not open configuration file " << config << endl ;
        return -1 ;
    }
  
    float lumi = gConfigParser->readFloatOption ("general::lumi") ;
    cout << "READING lumi " << lumi << endl ;
  
    int maxEvtsMC = -1;
    if (gConfigParser->isDefined ("general::maxEvtsMC"))
        maxEvtsMC = gConfigParser -> readIntOption ("general::maxEvtsMC");
  
    bool doOverUnderFlow = true;
    if (gConfigParser->isDefined ("general::useOverUnderFlow"))
        doOverUnderFlow = gConfigParser->readBoolOption ("general::useOverUnderFlow");
  
    bool doPlots = false;
    if (gConfigParser->isDefined ("general::doPlots"))
        doPlots = gConfigParser->readBoolOption ("general::doPlots");
    
    // .... .... .... .... .... .... .... .... .... .... .... ....
    
    vector<string> sigSamplesList; 
    vector<float> signalScales ;
    vector<mysample> sigSamples ;
  
    vector<string> bkgSamplesList;
    vector<mysample> bkgSamples ;
  
    vector<string> DATASamplesList;
    vector<mysample> DATASamples ;
  
  
    if (gConfigParser->isDefined ("general::signals"))
    {
        sigSamplesList = gConfigParser->readStringListOption ("general::signals") ;
        readSamples (sigSamples, sigSamplesList) ;
      
        for (unsigned int i = 0 ; i < sigSamplesList.size () ; ++i)
        {
          string name = string ("samples::") + sigSamplesList.at (i) + string ("FACT") ;
          float scale = 1.0;
          if (gConfigParser->isDefined (name.c_str ()) ) gConfigParser->readFloatOption (name.c_str ()) ;
          signalScales.push_back (scale) ;        
        }
    }
  
    if (gConfigParser->isDefined ("general::backgrounds"))
    {
        bkgSamplesList = gConfigParser->readStringListOption ("general::backgrounds") ;
        readSamples (bkgSamples, bkgSamplesList) ;
    }
    
    if (gConfigParser->isDefined ("general::data"))
    {
        DATASamplesList = gConfigParser->readStringListOption ("general::data") ;
        readSamples (DATASamples, DATASamplesList) ;
    }

    vector<string> activeSelections = gConfigParser->readStringListOption ("selections::list") ;
    vector<pair <TString, TCut> > selections = readCutsFile (
        activeSelections,
        gConfigParser->readStringOption ("selections::selectionsFile")) ;

    cout << "\n-====-====-====-====-====-====-====-====-====-====-====-====-====-\n\n" ;
    cout << "selections sequence: \n" ;
    for (unsigned int i = 0 ; i < selections.size () ; ++i)
        cout << selections.at (i).first << " : " << selections.at (i).second << endl ;

    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

    const bool drawbkg = (bkgSamplesList.size() > 0);
    const bool drawsig = (sigSamplesList.size() > 0);
    const bool drawDATA = (DATASamplesList.size() > 0);

    cout << " *** plotSimple : INFO : drawing :" << endl;
    cout << "     MC bkg " << drawbkg << endl;
    cout << "     signal " << drawsig << endl;
    cout << "     data   " << drawDATA << endl;

    // get the variables to be plotted
    // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
    vector<string> variablesList = gConfigParser->readStringListOption ("general::variables") ;
    vector<string> Buf_variables2DList(0);
    if (gConfigParser->isDefined ("general::2Dvariables")) Buf_variables2DList = gConfigParser->readStringListOption ("general::2Dvariables") ;
    
    vector<pair<string,string> > variables2DList ;
    for (unsigned int i = 0 ; i < Buf_variables2DList.size () ; ++i)
    {
        vector<string> dummy = split (Buf_variables2DList.at (i), ':') ;
        //cout << dummy.at (0) << " " << dummy.at (1) << " " << dummy.size () << endl ;
        variables2DList.push_back (make_pair(dummy.at (0), dummy.at (1)) ) ;
    }

    // ------------------ READ AND FILL HISTOS
    cout << "--- MAIN reading DATA and filling histos" << endl ;
    
    plotContainer DATA_plots ("DATA", variablesList, variables2DList, selections, DATASamplesList, 2) ;
    if (drawDATA)
    {
        cout << " ... DATA" << endl;
        counters DATACount = fillHistos (DATASamples, DATA_plots, 
                variablesList, variables2DList,
                selections,
                lumi,
                vector<float> (0),
                true, false) ;
        if (doOverUnderFlow) DATA_plots.AddOverAndUnderFlow () ;
    }
    
    plotContainer bkg_plots ("bkg", variablesList, variables2DList, selections, bkgSamplesList, 0) ;
    if (drawbkg)
    {
        cout << " ... MC bkg" << endl;
        counters bkgCount = fillHistos (bkgSamples, bkg_plots, 
                variablesList, variables2DList,
                selections,
                lumi,
                vector<float> (0),
                false, false, maxEvtsMC) ;
        if (doOverUnderFlow) bkg_plots.AddOverAndUnderFlow () ;
    }

    plotContainer sig_plots ("sig", variablesList, variables2DList, selections, sigSamplesList, 1) ;
    if (drawsig)
    {
        cout << " ... signal" << endl;
        counters sigCount = fillHistos (sigSamples, sig_plots, 
                    variablesList, variables2DList,
                    selections,
                    lumi,
                    signalScales,
                    false, true) ;
        if (doOverUnderFlow) sig_plots.AddOverAndUnderFlow () ;
    }


    // =================================================================================
    // ========== SAVE CANVAS ==========================================================
    // =================================================================================


    TCanvas * c = new TCanvas ("c", "c", 600, 600) ;
    //TCanvas * c = new TCanvas () ;

    cout << "--- MAIN before plotting" << endl ;

    // retrieve legend names
    vector <pair <string, string> > addToLegend;
    vector <pair <string, string> > addToLegend_bis;
    vector <pair <string, string> > variablesLabels;

    string legendSampleNamesFile = gConfigParser->readStringOption ("labels::sampleLabels");
    addToLegend = readVarNamesFile (sigSamplesList, legendSampleNamesFile);
    addToLegend_bis = readVarNamesFile (bkgSamplesList, legendSampleNamesFile);
    addToLegend.insert (addToLegend.end(), addToLegend_bis.begin(), addToLegend_bis.end());

    string legendVarLabelsFile = gConfigParser->readStringOption ("labels::variableLabels");
    variablesLabels = readVarNamesFile (variablesList, legendVarLabelsFile);


    TString outFolderNameBase = gConfigParser->readStringOption ("general::outputFolderName") ;
    outFolderNameBase += "/" ;

    system (TString ("mkdir -p ") + outFolderNameBase) ;
    system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events/")) ;
    system (TString ("mkdir -p ") + outFolderNameBase + TString ("/events_log/")) ;
    system (TString ("mkdir -p ") + outFolderNameBase + TString ("/rootCanvas/")) ;
    // cout << TString ("cp ") + config + " " + outFolderNameBase << endl;
    system (TString ("cp ") + config + " " + outFolderNameBase ) ;

    // save to file
    TString outString ;
    outString.Form (outFolderNameBase + "/outPlotter.root") ;
    TFile * fOut = new TFile (outString.Data (), "RECREATE") ;
    
    if (drawsig)  sig_plots.save (fOut) ;
    if (drawDATA) DATA_plots.save (fOut) ;
    if (drawbkg)  bkg_plots.save (fOut) ;

    if (doPlots)
    {    
        // loop on selections
        for (unsigned int isel = 0 ; isel < selections.size () ; ++isel)
        {
            // loop on variables
            for (unsigned int iv = 0 ; iv < variablesList.size () ; ++iv)
            {
                c->cd () ;
                TString outputName ; 
                outputName.Form ("plot_%s_%s", variablesList.at (iv).c_str (), selections.at (isel).first.Data ()) ;
                
                TString coutputName ;
                TString outFolderName ;

                // prototype of makeStackPlot:
                // std::vector<TObject*> makeStackPlot (plotContainer& dataPlots, plotContainer& bkgPlots, plotContainer& sigPlots,
                //                             string varName, string selName,
                //                             TCanvas* canvas, std::vector <pair <string, string> > & addInLegend, std::vector <pair <string, string> >& axisTitles,
                //                             bool LogY = false, bool makeRatioPlot = true, bool drawLegend = true, bool doShapes = false, bool forceNonNegMin = false, bool drawGrassForData = false,
                //                             bool drawSignal = true, bool drawData = true, bool drawMC = true) ;

                outFolderName = outFolderNameBase + TString ("/events/") ;
                std::vector<TObject*> drawings_nonScaled = makeStackPlot (DATA_plots, bkg_plots, sig_plots,
                                          variablesList.at (iv), selections.at (isel).first.Data (),
                                          c, addToLegend, variablesLabels,
                                          false, false, true, false, false, false,
                                          drawsig, drawDATA, drawbkg) ;
                                          //c, addToLegend, variablesLabels, false, false, true, false, true, false, false, true, true) ;

                coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
                c->SaveAs (coutputName.Data ()) ;

                outFolderName = outFolderNameBase + TString ("/events_log/") ;
                std::vector<TObject*> drawings_nonScaled_1 = makeStackPlot (DATA_plots, bkg_plots, sig_plots,
                                          variablesList.at (iv), selections.at (isel).first.Data (),
                                          c, addToLegend, variablesLabels,
                                          true, false, true, false, false, false,
                                          drawsig, drawDATA, drawbkg) ;
                                          //c, addToLegend, variablesLabels, false, false, true, false, true, false, false, true, true) ;

                coutputName.Form ("%s.pdf", (outFolderName + outputName).Data ()) ;
                c->SaveAs (coutputName.Data ()) ;


                outFolderName = outFolderNameBase + TString ("/rootCanvas/") ;
                coutputName.Form ("%s.root", (outFolderName + outputName).Data ()) ;
                c->SaveAs (coutputName.Data ());
            }
        }
    }
    
    cout << " ... exiting" << endl;
    return 0;
}