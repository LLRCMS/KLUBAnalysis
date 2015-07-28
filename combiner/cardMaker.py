#! /usr/bin/env python
import sys, pwd, commands
import os
import re
import math
from scipy.special import erf
from ROOT import *
import ROOT
from array import array

class cardMaker:
    
    def __init__(self):
        self.ID_untag = 1
        self.ID_1Btag = 2
        #...

    def loadIncludes(self):
        ROOT.gSystem.AddIncludePath("-I$ROOFITSYS/include/")
        ROOT.gSystem.AddIncludePath("-Iinclude/")
        ROOT.gSystem.Load("libRooFit")
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit.so")

    def makeCardsAndWorkspace(self, theHHlambda, theChannel, theOutputDir):
        
        ## ----- SETTING AND DECLARATIONS ----
        DEBUG = False
        self.lumi =1 
        self.sqrts = 13
        #...

        if(theChannel == self.ID_untag ): self.appendName="untag"
        elif(theChannel == self.ID_1Btag ): self.appendName="1Btag"
        #...
        else: print "Input Error: Unknown channel!"

        #"uno" serve sempre
        one = ROOT.RooRealVar("one","one",1.0)
        one.setConstant(True)

        ## -------------------------- IMPORT TEMPLATES ---------------------------- ##
        inputFile = TFile.Open("total.root")
        templateSIG = inputFile.Get("")
        #...

        ## -------------------------- RATES  ---------------------------- ##
        #it would be better to read them from the outside...
        rate_signal_Shape = templateSIG.Integral("width")*self.lumi #*2.3
        totalRate = rate_signal_Shape
        ## rates per lumi for scaling
        #if (self.channel == self.ID_2e2mu) : bkgRate_qqzz = 76.82#theInputs['qqZZ_rate']/theInputs['qqZZ_lumi'] #*1.8
        #elif (self.channel == self.ID_4e) :bkgRate_qqzz = 29.2900 
        #elif (self.channel == self.ID_4mu) : bkgRate_qqzz = 43.2100
        #totalRate_ggzz_Shape = totalRate_ggzz*self.lumi
        #bkgRate_qqzz_Shape = bkgRate_qqzz*self.lumi

        print " signal rate ", rate_signal_Shape
        print "total rate ", totalRate

        ## -------------------------- SIGNAL SHAPE VARIABLES ---------------------- ##
        binsx = templateSIG.GetNBinsX()
        binsy = templateSIG.GetNBinsY()

        x_name = "HHKinFitMass"
        x = ROOT.RooRealVar(x_name,x_name,100,1000)
        x.setVal(250)
        x.setBins(binsx)

        y_name = "HHBDT"
        y = ROOT.RooRealVar(y_name,y_name,0,1)
        y.setVal(0.5)
        y.setBins(binsy)

        self.LUMI = ROOT.RooRealVar("LUMI_{0:.0f}".format(self.sqrts),"LUMI_{0:.0f}".format(self.sqrts),self.lumi)
        self.LUMI.setConstant(True)
        rrvLumi = ROOT.RooRealVar("cmshh_lumi","cmshh_lumi",self.lumi)  

        ## -------------------------- 2D ROOFIT TEMPLATES ---------------------- ##
        TemplateName = "SIG_TempDataHist_{0:.0f}_{1:.0f}_{2:.0f}".format(self.channel,self.sqrts,self.HHLambda)
        SIG_TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(x,y),templateSIG)
        PdfName = "SIG_TemplatePdf_{0:.0f}_{1:.0f}_{2:.0f}".format(self.channel,self.sqrts,self.HHLambda)
        SIG_TemplatePdf = ROOT.RooHistPdf("bkg_qqzz","bkg_qqzz",ROOT.RooArgSet(x,y),SIG_TempDataHist)



        ## --------------------------- DATASET --------------------------- ##
        dataFileDir = "CMSdata"
        dataTreeName = "HTauTauTree" 
        dataFileName = "{0}/{1}_{2}.root".format(dataFileDir,self.HHLambda,self.inputlumi)
        if (DEBUG): print dataFileName," ",dataTreeName 
        data_obs_file = ROOT.TFile(dataFileName)

        print data_obs_file.Get(dataTreeName)
        
        if not (data_obs_file.Get(dataTreeName)):
            print "File, \"",dataFileName,"\", or tree, \"",dataTreeName,"\", not found" 
            print "Exiting..."
            sys.exit()
        
        data_obs_tree = data_obs_file.Get(dataTreeName)
        data_obs = ROOT.RooDataSet()
        datasetName = "data_obs_{0}".format(self.appendName)
        

        data_obs = ROOT.RooDataSet(datasetName,datasetName,data_obs_tree,ROOT.RooArgSet(x,y))
        print "observations: ", data_obs.numEntries()    

        ## --------------------------- WORKSPACE -------------------------- ##

        w = ROOT.RooWorkspace("w","w")
        getattr(w,'import')(data_obs)
        getattr(w,'import')(SIG_TemplatePdf,ROOT.RooFit.RecycleConflictNodes())

        name_ShapeWS = "{0}/hh_{1}_{2:.0f}TeV.input.root".format(self.theOutputDir,self.theChannel,self.HHLambda)
        w.writeToFile(name_ShapeWS)

        ## --------------------------- DATACARD -------------------------- ##
        #...

# run as main
if __name__ == "__main__":
    dc =cardMaker()
    dc.loadIncludes()
    cmd = 'mkdir -p cards/'
    status, output = commands.getstatusoutput(cmd)    
    dc.makeCardsAndWorkspace(1,1,"cards")
