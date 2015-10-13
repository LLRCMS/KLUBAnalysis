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
        theOutLambda = str(int(theHHLambda))
        if abs(theHHLambda - int(theHHLambda) )>0.01 : 
            theOutLambda = str(int(theHHLambda))+"dot"+ str(int(100*abs(theHHLambda - int(theHHLambda) )))
        if theHHLambda <0 : 
            theOutLambda = "m"+str(abs(int(theHHLambda)))

        #theHHLambda = 20 ##FIXME, waiting for the other samples
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
        nameString = "OS_sig_{0}{1}_OS_{3}_Lambda{2}".format(theInputs.AllVars[theInputs.varX],var2,theOutLambda,theInputs.selectionLevel)
        print nameString
        templateSIG = inputFile.Get(nameString)
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
        totalRate = float(rate_signal_Shape)
        print " signal rate ", rate_signal_Shape
        theRates = [rate_signal_Shape]
        templatesBKG = []
        for isample in  theInputs.background:
            nameTemplate = "OS_bkg_{0}{1}_OS_{2}_{3}".format(theInputs.AllVars[theInputs.varX],var2,theInputs.selectionLevel,isample)
            if isample in theInputs.additional :
                index = theInputs.additional.index(isample)
                nameTemplate = theInputs.additionalName[index]
            template = inputFile.Get(nameTemplate).Clone()
            if self.is2D == 1:
                if "TH2" in template.ClassName() : 
                    template = template.ProjectionX()
            templatesBKG.append(template)
            theRates.append(template.Integral("width")*self.lumi)
            totalRate = totalRate + theRates[len(theRates)-1]

            #rate_bkgTT_Shape = templateBKG_TT.Integral("width")*self.lumi

        #theRates=[rate_signal_Shape,rate_bkgTT_Shape,rate_bkgDY_Shape,rate_bkgTWantitop_Shape,rate_bkgTWtop_Shape]
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

        self.LUMI = ROOT.RooRealVar("LUMI_{0:.0f}".format(self.sqrts),"LUMI_{0:.2f}".format(self.sqrts),self.lumi)
        self.LUMI.setConstant(True)
        rrvLumi = ROOT.RooRealVar("cmshh_lumi","cmshh_lumi",self.lumi)  

        ## -------------------------- 2D ROOFIT TEMPLATES ---------------------- ##
        w = ROOT.RooWorkspace("w","w")

        #Default
        morphVarList_sig = ROOT.RooArgList()
        MorphList_sig = ROOT.RooArgList()
        TemplateName = "SIG_TempDataHist_{0:.0f}_{1:.0f}_{2:.2f}".format(theChannel,self.sqrts,theHHLambda)
        SIG_TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ral_variableList,templateSIG)
        #PdfName = "SIG_TemplatePdf_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
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
        rdhB = []
        rhpB = []
        for ibkg in range(len(templatesBKG)):
            TemplateName = "BKG_{3}_TempDataHist_{0:.0f}_{1:.0f}_{2:.2f}".format(theChannel,self.sqrts,theHHLambda,theInputs.background[ibkg])
            rdhB.append(ROOT.RooDataHist(TemplateName,TemplateName,ral_variableList,templatesBKG[ibkg]))
            #PdfName = "BKG_{3}_TemplatePdf_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda,theInputs.background[ibkg])
            rhpB.append(ROOT.RooHistPdf("bkg_{0}".format(theInputs.background[ibkg]),"bkg_{0}".format(theInputs.background[ibkg]),ras_variableSet,rdhB[ibkg]))
            getattr(w,'import')(rhpB[ibkg],ROOT.RooFit.RecycleConflictNodes())

        
        ## --------------------------- DATASET --------------------------- ##
        #RooDataSet ds("ds","ds",ras_variableSet,Import(*tree)) ;
        data_obs = rhpB[0].generate(ras_variableSet,1000)
        #datasetName = "data_obs_{0}".format(self.appendName)
        data_obs.SetNameTitle("data_obs","data_obs")

        #data_obs = ROOT.RooDataSet(datasetName,datasetName,data_obs_tree,ras_variableSet)
        print "observations: ", data_obs.numEntries()    

        ## --------------------------- WORKSPACE -------------------------- ##
        getattr(w,'import')(data_obs,ROOT.RooFit.Rename("data_obs"))
        getattr(w,'import')(SIG_TemplatePdf,ROOT.RooFit.RecycleConflictNodes())
        ##FIXME!!!!!!
        #name_ShapeWS = "cards{3}/{0}/hh_{1}_L{2:.0f}_13TeV.input.root".format(theOutputDir,theChannel,theHHLambda,dname)
        #name_ShapeDC = "cards{3}/{0}/hh_{1}_L{2:.0f}_13TeV.txt".format(theOutputDir,theChannel,theHHLambda,dname)
        #string_ShapeWS = "hh_{0}_L{1:.0f}_13TeV.input.root".format(theChannel,theHHLambda)
        #string_ShapeDC = "hh_{0}_L{1:.0f}_13TeV.txt".format(theChannel,theHHLambda)
        name_ShapeWS = "cards{3}/{0}/hh_{1}_L{2:.2f}_13TeV.input.root".format(theOutputDir,theChannel,theHHLambda,dname)
        name_ShapeDC = "cards{3}/{0}/hh_{1}_L{2:.2f}_13TeV.txt".format(theOutputDir,theChannel,theHHLambda,dname)
        string_ShapeWS = "hh_{0}_L{1:.2f}_13TeV.input.root".format(theChannel,theHHLambda)
        string_ShapeDC = "hh_{0}_L{1:.2f}_13TeV.txt".format(theChannel,theHHLambda)

        w.writeToFile(name_ShapeWS)

        ## --------------------------- DATACARD -------------------------- ##
        file = open( name_ShapeDC, "wb")

        #channelList=theInputs.background #['sig','bkg_TT','bkg_DY'] 
        channelName=['sig']#'sig','bkg_TT','bkg_DY','bkg_TWantitop','bkg_TWtop']
        for isample in theInputs.background:
            channelName.append('bkg_'+isample)
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
        syst = systReader("../config/systematics.cfg",['Lambda{0}'.format(theOutLambda)],theInputs.background) #FIXME: use the one above once all bkg are in
	syst.writeSystematics(file)


#define function for parsing options
def parseOptions():

    usage = ('usage: %prog [options] datasetList\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)
    
    parser.add_option('-d', '--is2D',   dest='is2D',       type='int',    default=1,     help='number of Dimensions (default:1)')
    parser.add_option('-f', '--filename',   dest='filename', type='string', default="",  help='input plots')
    parser.add_option('-l', '--lambda',   dest='Lambda', type='float', default=20,  help='Lambda value')
    parser.add_option('-c', '--channel',   dest='channel', type='string', default='MuTau',  help='final state')

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
    input = configReader("../config/analysis_"+opt.channel+".cfg")
    thechannel = 1
    if opt.channel == "MuTau" : thechannel=2
    elif opt.channel == "TauTau" : thechannel = 3

    input.readInputs()
    dc.makeCardsAndWorkspace(opt.Lambda,1,thechannel,"lambda{0:.2f}".format(opt.Lambda),input)
