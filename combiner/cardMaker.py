#! /usr/bin/env python
import sys, pwd, commands
import os
import re
import math
from scipy.special import erf
from ROOT import *
import ROOT
from array import array
from configReader import *

class cardMaker:
    
    def __init__(self):
        self.ID_untag = 1
        self.ID_1Btag = 2
        
        self.ID_ch_etau=1
        self.ID_ch_mutau=2
        self.ID_ch_tautau=3
        #...

    def loadIncludes(self):
        ROOT.gSystem.AddIncludePath("-I$ROOFITSYS/include/")
        ROOT.gSystem.AddIncludePath("-Iinclude/")
        ROOT.gSystem.Load("libRooFit")
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit.so")

    def makeCardsAndWorkspace(self, theHHLambda, theCat, theChannel, theOutputDir, theInputs):
        
        ## ----- SETTING AND DECLARATIONS ----
        DEBUG = False
        self.lumi = theInputs.lumi
        self.sqrts = 13
        #...

        if(theChannel == self.ID_ch_etau ): self.appendName="etau"
        elif(theChannel == self.ID_ch_mutau ): self.appendName="mutau"
        elif(theChannel == self.ID_ch_tautau ): self.appendName="tautau"
        #...
        else: print "Input Error: Unknown channel!"


        if(theCat == self.ID_untag ): self.appendName=self.appendName+"untag"
        elif(theCat == self.ID_1Btag ): self.appendName=self.appendName+"1Btag"
        #...
        else: print "Input Error: Unknown category!"

        #"uno" serve sempre
        one = ROOT.RooRealVar("one","one",1.0)
        one.setConstant(True)

        ## -------------------------- IMPORT Histograms ---------------------- ##
        ##NB: if you add a new histogram, you'll need to modify the 2D templates below as well
        inputFile = TFile.Open("outPlotter_2D.root")

        #Default
        print theInputs.AllVars, "  ",theInputs.varX, "  ",theInputs.varY
        print "test2D_{0}{1}_Lambda{2:.0f}_{3}".format(theInputs.AllVars[theInputs.varX],theInputs.AllVars[theInputs.varY],theHHLambda,theInputs.selectionLevel)
        templateSIG = inputFile.Get("test2D_{0}{1}_Lambda{2:.0f}_{3}".format(theInputs.AllVars[theInputs.varX],theInputs.AllVars[theInputs.varY],theHHLambda,theInputs.selectionLevel))
        ##JES syst
        #templateSIG_JESUP = inputFile.Get("") 
        #templateSIG_JESDOWN = inputFile.Get("") 
        ##QCDscale syst
        #templateSIG_QCDUP = inputFile.Get("") 
        #templateSIG_QCDDOWN = inputFile.Get("") 
        ##...
        putTT=False
        putDY=False
        rate_bkgTT_Shape = 0.0
        rate_bkgDY_Shape = 0.0
        for isample in  theInputs.background:
            if isample == "TT":
                templateBKG_TT = inputFile.Get("test2D_{0}{1}_{3}_{2}".format(theInputs.AllVars[theInputs.varX],theInputs.AllVars[theInputs.varY],theInputs.selectionLevel),isample)
                rate_bkgTT_Shape = templateBKG_TT.Integral("width")*self.lumi
                putTT=True
            elif isample == "DY":
                templateBKG_DY = inputFile.Get("test2D_{0}{1}_{3}_{2}".format(theInputs.AllVars[theInputs.varX],theInputs.AllVars[theInputs.varY],theInputs.selectionLevel),isample)
                rate_bkgDY_Shape = templateBKG_DY.Integral("width")*self.lumi
                putDY=True
        
        #...

        ## -------------------------- RATES  ---------------------------- ##
        #it would be better to read them from the outside...
        rate_signal_Shape = templateSIG.Integral("width")*self.lumi #*2.3
        totalRate = rate_signal_Shape
         #*2.3
        totalRate = totalRate + rate_bkgTT_Shape + rate_bkgDY_Shape
        ## rates per lumi for scaling
        #if (self.channel == self.ID_2e2mu) : bkgRate_qqzz = 76.82#theInputs['qqZZ_rate']/theInputs['qqZZ_lumi'] #*1.8
        #elif (self.channel == self.ID_4e) :bkgRate_qqzz = 29.2900 
        #elif (self.channel == self.ID_4mu) : bkgRate_qqzz = 43.2100
        #totalRate_ggzz_Shape = totalRate_ggzz*self.lumi
        #bkgRate_qqzz_Shape = bkgRate_qqzz*self.lumi

        print " signal rate ", rate_signal_Shape
        print " bkg TT rate ", rate_bkgTT_Shape
        print "total rate ", totalRate
        theRates=[rate_signal_Shape,rate_bkgTT_Shape,rate_bkgDY_Shape]
        ## -------------------------- SIGNAL SHAPE VARIABLES ---------------------- ##
        binsx = templateSIG.GetNbinsX()
        binsy = templateSIG.GetNbinsY()
        print theInputs.AllvarX[theInputs.varX]
        x_name = theInputs.AllVars[theInputs.varX]
        x = ROOT.RooRealVar(x_name,x_name,float(theInputs.AllvarX[theInputs.varX]),float(theInputs.AllvarY[theInputs.varX]))
        x.setVal(250)
        x.setBins(binsx)#theInputs.AllBins[theInputs.varX])

        y_name = theInputs.AllVars[theInputs.varY]
        y = ROOT.RooRealVar(y_name,y_name,float(theInputs.AllvarX[theInputs.varY]),float(theInputs.AllvarY[theInputs.varY]))
        y.setVal(0.5)
        y.setBins(binsy)#theInputs.AllBins[theInputs.varY])

        self.LUMI = ROOT.RooRealVar("LUMI_{0:.0f}".format(self.sqrts),"LUMI_{0:.0f}".format(self.sqrts),self.lumi)
        self.LUMI.setConstant(True)
        rrvLumi = ROOT.RooRealVar("cmshh_lumi","cmshh_lumi",self.lumi)  

        ## -------------------------- 2D ROOFIT TEMPLATES ---------------------- ##
        w = ROOT.RooWorkspace("w","w")

        #Default
        morphVarList_sig = ROOT.RooArgList()
        MorphList_sig = ROOT.RooArgList()
        TemplateName = "SIG_TempDataHist_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
        SIG_TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(x,y),templateSIG)
        PdfName = "SIG_TemplatePdf_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
        SIG_TemplatePdf = ROOT.RooHistPdf("sig","sig",ROOT.RooArgSet(x,y),SIG_TempDataHist)
        print templateSIG.Integral()
        ##Up
        #TemplateName = "SIG_JESUP_TempDataHist_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
        #SIG_JESUP_TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(x,y),templateSIG_JESUP)
        #PdfName = "SIG_JESUP_TemplatePdf_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
        #SIG_JESUP_TemplatePdf = ROOT.RooHistPdf("sig_jesup","sig_jesup",ROOT.RooArgSet(x,y),SIG_JESUP_TempDataHist)
        ##Down
        #TemplateName = "SIG_JESDOWN_TempDataHist_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
        #SIG_JESDOWN_TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(x,y),templateSIG_JESDOWN)
        #PdfName = "SIG_JESDOWN_TemplatePdf_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
        #SIG_JESDOWN_TemplatePdf = ROOT.RooHistPdf("sig_jesdown","sig_jesdown",ROOT.RooArgSet(x,y),SIG_JESDOWN_TempDataHist)

        #CMS_JES_syst = w.factory("JES_sig[-5,5]")
        #morphVarList_sig.add(CMS_JES_syst)
        #MorphList_sig.add(SIG_TemplatePdf)
        #MorphList_sig.add(SIG_JESUP_TemplatePdf)
        #MorphList_sig.add(SIG_JESDOWN_TemplatePdf)

        #SIGpdf = ROOT.VerticalInterpPdf("SIG","SIG",MorphList_sig,morphVarList_sig)
        SIGpdf =SIG_TemplatePdf
        if putTT :
            TemplateName = "BKG_TT_TempDataHist_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
            BKG_TT_TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(x,y),templateBKG_TT)
            PdfName = "BKG_TT_TemplatePdf_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
            BKG_TT_TemplatePdf = ROOT.RooHistPdf("bkg_TT","bkg_TT",ROOT.RooArgSet(x,y),BKG_TT_TempDataHist)
            getattr(w,'import')(BKG_TT_TemplatePdf,ROOT.RooFit.RecycleConflictNodes())
        if putDY :
            TemplateName = "BKG_DY_TempDataHist_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
            BKG_DY_TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(x,y),templateBKG_TT)
            PdfName = "BKG_DY_TemplatePdf_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
            BKG_DY_TemplatePdf = ROOT.RooHistPdf("bkg_DY","bkg_DY",ROOT.RooArgSet(x,y),BKG_DY_TempDataHist)
            getattr(w,'import')(BKG_DY_TemplatePdf,ROOT.RooFit.RecycleConflictNodes())
        
        ## --------------------------- DATASET --------------------------- ##
        #RooDataSet ds("ds","ds",RooArgSet(x,y),Import(*tree)) ;
        data_obs = BKG_TT_TemplatePdf.generate(RooArgSet(x,y),1000)
        #datasetName = "data_obs_{0}".format(self.appendName)
        data_obs.SetNameTitle("data_obs","data_obs")

        #data_obs = ROOT.RooDataSet(datasetName,datasetName,data_obs_tree,ROOT.RooArgSet(x,y))
        print "observations: ", data_obs.numEntries()    

        ## --------------------------- WORKSPACE -------------------------- ##
        getattr(w,'import')(data_obs,ROOT.RooFit.Rename("data_obs"))
        getattr(w,'import')(SIG_TemplatePdf,ROOT.RooFit.RecycleConflictNodes())
        name_ShapeWS = "{0}/hh_{1}_{2:.0f}TeV.input.root".format(theOutputDir,theChannel,theHHLambda)
        name_ShapeDC = "{0}/hh_{1}_{2:.0f}TeV.txt".format(theOutputDir,theChannel,theHHLambda)
        w.writeToFile(name_ShapeWS)

        ## --------------------------- DATACARD -------------------------- ##
        file = open( name_ShapeDC, "wb")

        #channelList=theInputs.background #['sig','bkg_TT','bkg_DY'] 
        channelName=['sig','bkg_TT','bkg_DY']

        file.write("imax 1\n")
        file.write("jmax 1\n")#.format(numberSig+numberBg-1))
        file.write("kmax *\n")
        
        file.write("------------\n")
        #file.write("shapes * * {0} w:$PROCESS w:$PROCESS_$SYSTEMATIC\n".format(name_ShapeWS))
        file.write("shapes * * {0} w:$PROCESS \n".format(name_ShapeWS))
        file.write("------------\n")
        

        file.write("bin a{0} \n".format(theChannel))
        file.write("observation -1 \n")#.format(obsEvents))
        
        file.write("------------\n")
        ##file.write("## mass window [{0},{1}] \n".format(self.low_M,self.high_M))
        file.write("bin ")        
        for chan in channelName:
            file.write("a{0} ".format(theChannel))
        file.write("\n")
                                        
        file.write("process ")
        #i=0
        for chan in channelName:
            file.write("{0} ".format(chan)
            #i+=1
        file.write("\n")
        file.write("process 0 1 \n")
        file.write("rate ")
        for chan in range(len(channelName)):
            file.write("{0:.4f} ".format(theRates[chan]))
        file.write("\n")

        file.write("------------\n")
        file.write("lumi_8TeV lnN 1.026  1.026 1.026  1.026 \n")
        file.write("CMS_eff_m lnN 1.026  1.026 1.026  1.026 \n")
        file.write("CMS_eff_e lnN 1.046  1.046 1.046  1.046 \n")  
        file.write("CMS_eff_t lnN 1.066  1.066 1.066  1.066 \n") 
        file.write("TTYield lnN - 1.100 -  - \n")   
#CMS_zz4l_pdf_QCDscale_gg_syst param 0.0 1 [-3,3] 

        #...

# run as main
if __name__ == "__main__":
    dc =cardMaker()
    dc.loadIncludes()
    cmd = 'mkdir -p cards/'
    input = configReader("../config/plotter_muTau.cfg") 
    input.readInputs()
    status, output = commands.getstatusoutput(cmd)    
    dc.makeCardsAndWorkspace(20,1,2,"lambda20",input)
