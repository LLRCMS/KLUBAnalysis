#! /usr/bin/env python
import sys, pwd, commands, optparse
import os
import re
import math
from scipy.special import erf
from ROOT import *
import ROOT
from array import array
from configReader import *
from systReader import *

#cercare di automatizzare il modo in cui vengono letti i fondi

class cardMaker:
    
    def __init__(self):
        self.ID_untag = 1
        self.ID_1Btag = 2
        
        self.ID_ch_etau=1
        self.ID_ch_mutau=2
        self.ID_ch_tautau=3
        
        self.is2D = 2

        self.filename = "test.root"
        #...

    def loadIncludes(self):
        ROOT.gSystem.AddIncludePath("-I$ROOFITSYS/include/")
        ROOT.gSystem.AddIncludePath("-Iinclude/")
        ROOT.gSystem.Load("libRooFit")
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit.so")

    def set2D(self,is2D):
        self.is2D = is2D
    def setfileName(self,filename):
        self.filename = filename

    def makeCardsAndWorkspace(self, theHHLambda, theCat, theChannel, theOutputDir, theInputs):
        
        dname=""
        if(self.is2D==2):dname="2D"
        cmd = 'mkdir -p cards{0}/{1}'.format(dname,theOutputDir)
        status, output = commands.getstatusoutput(cmd)    
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
        inputFile = TFile.Open(self.filename)

        #Default
        print theInputs.AllVars, "  ",theInputs.varX, "  ",theInputs.varY
        if theInputs.varY < 0 : 
            var2 = ""
        else :
            var2 = theInputs.AllVars[theInputs.varY]
        #print "test2D_{0}{1}_Lambda{2:.0f}_{3}".format(theInputs.AllVars[theInputs.varX],var2,theHHLambda,theInputs.selectionLevel)
        templateSIG = inputFile.Get("OS_sig_{0}{1}_OS_{3}_Lambda{2:.0f}".format(theInputs.AllVars[theInputs.varX],var2,theHHLambda,theInputs.selectionLevel))
        if self.is2D==1: 
            if "TH2" in templateSIG.ClassName() : templateSIG = templateSIG.ProjectionX()
        
        ##JES syst
        #templateSIG_JESUP = inputFile.Get("") 
        #templateSIG_JESDOWN = inputFile.Get("") 
        ##QCDscale syst
        #templateSIG_QCDUP = inputFile.Get("") 
        #templateSIG_QCDDOWN = inputFile.Get("") 
        ##...
        rate_signal_Shape = templateSIG.Integral("width")*self.lumi #*2.3
        totalRate = rate_signal_Shape
        print " signal rate ", rate_signal_Shape
        putTT=False
        putDY=False
        putTWantitop=False
        putTWtop=False
        rate_bkgTT_Shape = 0.0
        rate_bkgDY_Shape = 0.0
        rate_bkgTWantitop_Shape = 0.0
        rate_bkgTWtop_Shape = 0.0
        for isample in  theInputs.background:
            if isample == "TT":
                templateBKG_TT = inputFile.Get("OS_bkg_{0}{1}_OS_{2}_{3}".format(theInputs.AllVars[theInputs.varX],var2,theInputs.selectionLevel,isample))
                if self.is2D==1 : 
                    if "TH2" in templateBKG_TT.ClassName() : templateBKG_TT = templateBKG_TT.ProjectionX()
                rate_bkgTT_Shape = templateBKG_TT.Integral("width")*self.lumi
                putTT=True
                print " bkg TT rate ", rate_bkgTT_Shape
            elif isample == "DY":
                templateBKG_DY = inputFile.Get("OS_bkg_{0}{1}_OS_{2}_{3}".format(theInputs.AllVars[theInputs.varX],var2,theInputs.selectionLevel,isample))
                if self.is2D==1 : 
                    if "TH2" in templateBKG_DY.ClassName() : templateBKG_DY = templateBKG_DY.ProjectionX()
                rate_bkgDY_Shape = templateBKG_DY.Integral("width")*self.lumi
                putDY=True
                print " bkg DY rate ", rate_bkgDY_Shape
            elif isample == "TWantitop":
                templateBKG_TWantitop = inputFile.Get("OS_bkg_{0}{1}_OS_{2}_{3}".format(theInputs.AllVars[theInputs.varX],var2,theInputs.selectionLevel,isample))
                if self.is2D==1 : 
                    if "TH2" in templateBKG_TWantitop.ClassName() : templateBKG_TWantitop = templateBKG_TWantitop.ProjectionX()
                rate_bkgTWantitop_Shape = templateBKG_TWantitop.Integral("width")*self.lumi
                putTWantitop=True
                print " bkg TWantitop rate ", rate_bkgTWantitop_Shape
            elif isample == "TWtop":
                templateBKG_TWtop = inputFile.Get("OS_bkg_{0}{1}_OS_{2}_{3}".format(theInputs.AllVars[theInputs.varX],var2,theInputs.selectionLevel,isample))
                if self.is2D==1 : 
                    if "TH2" in templateBKG_TWtop.ClassName() : templateBKG_TWtop = templateBKG_TWtop.ProjectionX()
                rate_bkgTWtop_Shape = templateBKG_TWtop.Integral("width")*self.lumi
                putTWtop=True
                print " bkg TWtop rate ", rate_bkgTWtop_Shape
        #...

        ## -------------------------- RATES  ---------------------------- ##
        #it would be better to read them from the outside...
         #*2.3
        totalRate = totalRate + rate_bkgTT_Shape + rate_bkgDY_Shape + rate_bkgTWantitop_Shape + rate_bkgTWtop_Shape
        print "total rate ", totalRate
        ## rates per lumi for scaling
        #if (self.channel == self.ID_2e2mu) : bkgRate_qqzz = 76.82#theInputs['qqZZ_rate']/theInputs['qqZZ_lumi'] #*1.8
        #elif (self.channel == self.ID_4e) :bkgRate_qqzz = 29.2900 
        #elif (self.channel == self.ID_4mu) : bkgRate_qqzz = 43.2100
        #totalRate_ggzz_Shape = totalRate_ggzz*self.lumi
        #bkgRate_qqzz_Shape = bkgRate_qqzz*self.lumi


        theRates=[rate_signal_Shape,rate_bkgTT_Shape,rate_bkgDY_Shape,rate_bkgTWantitop_Shape,rate_bkgTWtop_Shape]
        ## -------------------------- SIGNAL SHAPE VARIABLES ---------------------- ##
        binsx = templateSIG.GetNbinsX()
        binsy = 0
        print theInputs.AllvarX[theInputs.varX]
        x_name = theInputs.AllVars[theInputs.varX]
        x = ROOT.RooRealVar(x_name,x_name,float(theInputs.AllvarX[theInputs.varX]),float(theInputs.AllvarY[theInputs.varX]))
        x.setVal(250)
        x.setBins(binsx)#theInputs.AllBins[theInputs.varX])
        ral_variableList = ROOT.RooArgList(x)
        ras_variableSet  = ROOT.RooArgSet(x)

        if(self.is2D == 2):
            y_name = theInputs.AllVars[theInputs.varY]
            y = ROOT.RooRealVar(y_name,y_name,float(theInputs.AllvarX[theInputs.varY]),float(theInputs.AllvarY[theInputs.varY]))
            y.setVal(0.5)
            binsy = templateSIG.GetNbinsY()
            y.setBins(binsy)#theInputs.AllBins[theInputs.varY])
            
            ral_variableList = ROOT.RooArgList(x,y)
            ras_variableSet  = ROOT.RooArgSet(x,y)

        self.LUMI = ROOT.RooRealVar("LUMI_{0:.0f}".format(self.sqrts),"LUMI_{0:.0f}".format(self.sqrts),self.lumi)
        self.LUMI.setConstant(True)
        rrvLumi = ROOT.RooRealVar("cmshh_lumi","cmshh_lumi",self.lumi)  

        ## -------------------------- 2D ROOFIT TEMPLATES ---------------------- ##
        w = ROOT.RooWorkspace("w","w")

        #Default
        morphVarList_sig = ROOT.RooArgList()
        MorphList_sig = ROOT.RooArgList()
        TemplateName = "SIG_TempDataHist_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
        SIG_TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ral_variableList,templateSIG)
        PdfName = "SIG_TemplatePdf_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
        SIG_TemplatePdf = ROOT.RooHistPdf("sig","sig",ras_variableSet,SIG_TempDataHist)
        print templateSIG.Integral()
        ##Up
        #TemplateName = "SIG_JESUP_TempDataHist_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
        #SIG_JESUP_TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ral_variableList,templateSIG_JESUP)
        #PdfName = "SIG_JESUP_TemplatePdf_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
        #SIG_JESUP_TemplatePdf = ROOT.RooHistPdf("sig_jesup","sig_jesup",ras_variableSet,SIG_JESUP_TempDataHist)
        ##Down
        #TemplateName = "SIG_JESDOWN_TempDataHist_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
        #SIG_JESDOWN_TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ral_variableList,templateSIG_JESDOWN)
        #PdfName = "SIG_JESDOWN_TemplatePdf_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
        #SIG_JESDOWN_TemplatePdf = ROOT.RooHistPdf("sig_jesdown","sig_jesdown",ras_variableSet,SIG_JESDOWN_TempDataHist)

        #CMS_JES_syst = w.factory("JES_sig[-5,5]")
        #morphVarList_sig.add(CMS_JES_syst)
        #MorphList_sig.add(SIG_TemplatePdf)
        #MorphList_sig.add(SIG_JESUP_TemplatePdf)
        #MorphList_sig.add(SIG_JESDOWN_TemplatePdf)

        #SIGpdf = ROOT.VerticalInterpPdf("SIG","SIG",MorphList_sig,morphVarList_sig)
        SIGpdf =SIG_TemplatePdf
        if putTT :
            TemplateName = "BKG_TT_TempDataHist_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
            BKG_TT_TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ral_variableList,templateBKG_TT)
            PdfName = "BKG_TT_TemplatePdf_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
            BKG_TT_TemplatePdf = ROOT.RooHistPdf("bkg_TT","bkg_TT",ras_variableSet,BKG_TT_TempDataHist)
            getattr(w,'import')(BKG_TT_TemplatePdf,ROOT.RooFit.RecycleConflictNodes())
        if putDY :
            TemplateName = "BKG_DY_TempDataHist_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
            BKG_DY_TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ral_variableList,templateBKG_TT)
            PdfName = "BKG_DY_TemplatePdf_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
            BKG_DY_TemplatePdf = ROOT.RooHistPdf("bkg_DY","bkg_DY",ras_variableSet,BKG_DY_TempDataHist)
            getattr(w,'import')(BKG_DY_TemplatePdf,ROOT.RooFit.RecycleConflictNodes())
        if putTWantitop :
            TemplateName = "BKG_TWantitop_TempDataHist_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
            BKG_TWantitop_TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ral_variableList,templateBKG_TT)
            PdfName = "BKG_TWantitop_TemplatePdf_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
            BKG_TWantitop_TemplatePdf = ROOT.RooHistPdf("bkg_TWantitop","bkg_TWantitop",ras_variableSet,BKG_TWantitop_TempDataHist)
            getattr(w,'import')(BKG_TWantitop_TemplatePdf,ROOT.RooFit.RecycleConflictNodes())
        if putTWtop :
            TemplateName = "BKG_TWtop_TempDataHist_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
            BKG_TWtop_TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ral_variableList,templateBKG_TT)
            PdfName = "BKG_TWtop_TemplatePdf_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
            BKG_TWtop_TemplatePdf = ROOT.RooHistPdf("bkg_TWtop","bkg_TWtop",ras_variableSet,BKG_TWtop_TempDataHist)
            getattr(w,'import')(BKG_TWtop_TemplatePdf,ROOT.RooFit.RecycleConflictNodes())
        
        ## --------------------------- DATASET --------------------------- ##
        #RooDataSet ds("ds","ds",ras_variableSet,Import(*tree)) ;
        data_obs = BKG_TT_TemplatePdf.generate(ras_variableSet,1000)
        #datasetName = "data_obs_{0}".format(self.appendName)
        data_obs.SetNameTitle("data_obs","data_obs")

        #data_obs = ROOT.RooDataSet(datasetName,datasetName,data_obs_tree,ras_variableSet)
        print "observations: ", data_obs.numEntries()    

        ## --------------------------- WORKSPACE -------------------------- ##
        getattr(w,'import')(data_obs,ROOT.RooFit.Rename("data_obs"))
        getattr(w,'import')(SIG_TemplatePdf,ROOT.RooFit.RecycleConflictNodes())
        name_ShapeWS = "cards{3}/{0}/hh_{1}_L{2:.0f}_13TeV.input.root".format(theOutputDir,theChannel,theHHLambda,dname)
        name_ShapeDC = "cards{3}/{0}/hh_{1}_L{2:.0f}_13TeV.txt".format(theOutputDir,theChannel,theHHLambda,dname)
        string_ShapeWS = "hh_{0}_L{1:.0f}_13TeV.input.root".format(theChannel,theHHLambda)
        string_ShapeDC = "hh_{0}_L{1:.0f}_13TeV.txt".format(theChannel,theHHLambda)

        w.writeToFile(name_ShapeWS)

        ## --------------------------- DATACARD -------------------------- ##
        file = open( name_ShapeDC, "wb")

        #channelList=theInputs.background #['sig','bkg_TT','bkg_DY'] 
        channelName=['sig','bkg_TT','bkg_DY','bkg_TWantitop','bkg_TWtop']

        file.write("imax 1\n")
        file.write("jmax {0}\n".format(len(channelName)-1))
        file.write("kmax *\n")
        
        file.write("------------\n")
        #file.write("shapes * * {0} w:$PROCESS w:$PROCESS_$SYSTEMATIC\n".format(name_ShapeWS))
        file.write("shapes * * {0} w:$PROCESS \n".format(string_ShapeWS))
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
            file.write("{0} ".format(chan))
            #i+=1
        file.write("\n")
        file.write("process ")
        for chan in range(len(channelName)):
            file.write("{0} ".format(chan))
        file.write("\n")

        file.write("rate ")
        for chan in range(len(channelName)):
            file.write("{0:.4f} ".format(theRates[chan]))
        file.write("\n")

        file.write("------------\n")
        #syst = systReader("../config/systematics.cfg",['Lambda20'],theInputs.background)
        syst = systReader("../config/systematics.cfg",['Lambda20'],['TT','DY','TWantitop','TWtop']) #FIXME: use the one above once all bkg are in
	syst.writeSystematics(file)


#define function for parsing options
def parseOptions():

    usage = ('usage: %prog [options] datasetList\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)
    
    parser.add_option('-d', '--is2D',   dest='is2D',       type='int',    default=2,     help='number of Dimensions (default:1)')
    parser.add_option('-f', '--filename',   dest='filename', type='string', default="",  help='input plots')

    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()

    if (opt.is2D != 1 and opt.is2D != 2):
        print 'The input '+opt.is2D+' is unkown for is2D.  Please choose 1 or 2. Exiting...'
        sys.exit()
    if (opt.filename==""):
        print "you MUST specify an input file [please use -f option]"
        sys.exit()

# run as main
if __name__ == "__main__":
    parseOptions()
    global opt, args
    #(opt, args) = parser.parse_args()
    dc =cardMaker()
    dc.loadIncludes()
    dc.set2D(opt.is2D)
    dc.setfileName(opt.filename)
    #outputDir = ""
    input = configReader("../config/analysis_MuTau.cfg") 
    input.readInputs()
    dc.makeCardsAndWorkspace(20,1,2,"lambda20",input)
