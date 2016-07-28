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
        self.ID_0Btag = 0
        self.ID_1Btag = 1
        self.ID_2Btag = 2
        self.ID_Boosted = 3
        
        self.ID_ch_etau=1
        self.ID_ch_mutau=2
        self.ID_ch_tautau=3
        
        self.is2D = 2

        self.filename = "test.root"
        self.signalFile = "test.root"
        self.scale = 1
        #...
        self.outputdir=""
        self.writeThSyst = True
        self.binsysts = []
        self.binsystsNAME = []
        self.binsystsVALUE = []

        self.theChannel = -1
        self.isResonant = False

        self.addsqrtN = False
        self.addTES = True
        self.normalize = False

    def loadIncludes(self):
        ROOT.gSystem.AddIncludePath("-I$ROOFITSYS/include/")
        ROOT.gSystem.AddIncludePath("-Iinclude/")
        ROOT.gSystem.Load("libRooFit")
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit.so")

    def set2D(self,is2D):
        self.is2D = is2D
    def setfileName(self,filename):
        self.filename = filename

    def AddSingleShapeSyst(self,w,systRead,systName,inFileName,tempName,proc,ral_variableList,ras_variableSet,prefix,suffix=""):
        inputFile = TFile.Open(inFileName)
        print tempName+suffix
        templateNo = inputFile.Get(tempName+suffix)
        templateUp = inputFile.Get(tempName+prefix+"UP"+suffix)
        templateDo = inputFile.Get(tempName+prefix+"DOWN"+suffix)
        templateUp.Scale(templateNo.Integral()/templateUp.Integral())
        templateDo.Scale(templateNo.Integral()/templateDo.Integral())

        PdfName = "bkg_{0}".format(proc)

        rdhshapeUp    = ROOT.RooDataHist(systName+"Up",systName+"Up",ral_variableList,templateUp.Clone())
        rdhshapeDown  = ROOT.RooDataHist(systName+"Down",systName+"Down",ral_variableList,templateDo.Clone())
        name = PdfName+"_"+systName
        if not self.normalize :
            rdhshapeUp.SetNameTitle(name+"Up",name+"Up")
            rdhshapeDown.SetNameTitle(name+"Down",name+"Down")
            getattr(w,'import')(rdhshapeUp,ROOT.RooFit.RecycleConflictNodes())
            getattr(w,'import')(rdhshapeDown,ROOT.RooFit.RecycleConflictNodes())
        else :
            rhpshapeUp    = ROOT.RooHistPdf(name+"Up",name+"Up",ras_variableSet,rdhshapeUp)
            rhpShapeDown  = ROOT.RooHistPdf(name+"Down",name+"Down",ras_variableSet,rdhshapeDown)
            getattr(w,'import')(rhpshapeUp,ROOT.RooFit.RecycleConflictNodes())
            getattr(w,'import')(rhpShapeDown,ROOT.RooFit.RecycleConflictNodes())
        systRead.writeOneLine(proc,systName+" shape ")

    def AddTESShapeSyst(self,w,systName,inFileName,tempName,proc,ral_variableList,ras_variableSet):
        inputFile = TFile.Open(inFileName)
        templateNo = inputFile.Get(tempName.replace("_tauup",""))
        templateUp = inputFile.Get(tempName)
        templateDo = inputFile.Get(tempName)

        ### FIXME
        if "OS_sig" in tempName : #and not self.isResonant:
            templateNo.Scale(1.0/100.0)
            templateDo.Scale(1.0/100.0)
            templateUp.Scale(1.0/100.0)
        ### END FIXME
        
        for ibin in range(templateUp.GetNbinsX()):
            #if not self.isResonant :
            #    templateUp.SetBinContent(ibin+1, 1.05* templateNo.GetBinContent(ibin+1))
            templateDo.SetBinContent(ibin+1, 2.0* templateNo.GetBinContent(ibin+1)-templateUp.GetBinContent(ibin+1) )

        #templateNo.Scale(1/templateNo.Integral())
        #templateDo.Scale(1/templateNo.Integral())
        #templateUp.Scale(1/templateNo.Integral())

        if "ggHH" in proc :
            PdfName = proc
        else: 
            PdfName = "bkg_{0}".format(proc)
        print PdfName

#        Down_norm = ROOT.RooRealVar(PdfName+"dnorm",PdfName+"dnorm",templateDo.Integral())
#        Nominal_norm = ROOT.RooRealVar(PdfName+"nnorm",PdfName+"nnorm",templateNo.Integral())
#        Up_norm = ROOT.RooRealVar(PdfName+"unorm",PdfName+"unorm",templateUp.Integral())
#
#        Down_norm.setConstant(True)
#        Nominal_norm.setConstant(True)
#        Up_norm.setConstant(True)

        rdhshapeUp    = ROOT.RooDataHist("rdh_"+PdfName+"_"+systName+"Up","rdh_"+PdfName+"_"+systName+"Up",ral_variableList,templateUp.Clone())
        rdhshapeDown  = ROOT.RooDataHist("rdh_"+PdfName+"_"+systName+"Down","rdh_"+PdfName+"_"+systName+"Down",ral_variableList,templateDo.Clone())
        rhpshapeUp    = ROOT.RooHistPdf(PdfName+"_"+systName+"Up",PdfName+"_"+systName+"Up",ras_variableSet,rdhshapeUp)
        rhpShapeDown  = ROOT.RooHistPdf(PdfName+"_"+systName+"Down",PdfName+"_"+systName+"Down",ras_variableSet,rdhshapeDown)
        rdhshapeNo    = ROOT.RooDataHist("rdh_"+PdfName+"_"+systName+"No","rdh_"+PdfName+"_"+systName+"No",ral_variableList,templateNo.Clone())
        rhpNominal    = ROOT.RooHistPdf(PdfName+"_"+systName+"No",PdfName+"_"+systName+"No",ras_variableSet,rdhshapeNo)
        
        if not self.normalize :
            rdhshapeUp.SetNameTitle(PdfName+"_"+systName+"Up",PdfName+"Up")
            rdhshapeDown.SetNameTitle(PdfName+"_"+systName+"Down",PdfName+"Down")
            rdhshapeNo.SetNameTitle(PdfName,PdfName)
            getattr(w,'import')(rdhshapeUp,ROOT.RooFit.RecycleConflictNodes())
            getattr(w,'import')(rdhshapeDown,ROOT.RooFit.RecycleConflictNodes())
            getattr(w,'import')(rdhshapeNo,ROOT.RooFit.RecycleConflictNodes())
        else :
            morphVarList = ROOT.RooArgList()
            morphVarList.add(w.var("CMS_HHbbtt_scale_tau"))
            MorphList = ROOT.RooArgList()
            MorphList.add(rhpNominal)
            MorphList.add(rhpshapeUp)
            MorphList.add(rhpShapeDown)
            pdf = ROOT.VerticalInterpPdf(PdfName,PdfName,MorphList,morphVarList)

            kappalow = ROOT.RooFormulaVar("kappalow_"+PdfName,"@0/@1",ROOT.RooArgList(Down_norm,Nominal_norm))
            kappahigh = ROOT.RooFormulaVar("kappahigh_"+PdfName,"@0/@1",ROOT.RooArgList(Up_norm,Nominal_norm))
            thetaSyst = AsymPow("theta"+PdfName,"theta"+PdfName,kappalow,kappahigh,w.var("CMS_HHbbtt_scale_tau"))
            var_norm = ROOT.RooFormulaVar(PdfName+"_norm","@0",ROOT.RooArgList(thetaSyst))

            getattr(w,'import')(rhpshapeUp,ROOT.RooFit.RecycleConflictNodes())
            getattr(w,'import')(rhpShapeDown,ROOT.RooFit.RecycleConflictNodes())
            getattr(w,'import')(var_norm,ROOT.RooFit.RecycleConflictNodes())
            getattr(w,'import')(pdf,ROOT.RooFit.RecycleConflictNodes())

        #systRead.writeOneLine(proc,systName+" shape ")



    def AddBinByBinSyst(self,w,pname,PdfName,template,ral_variableList,ras_variableSet,threshold=-1) :
        tBkgIntegral = template.Integral()
        #Protection: do NOT apply if <=1 populated bins, where populated means must contain at least 1% of the histogram
        nPop = 0;
        emptyBins = []
        for iy in range(1,1+template.GetNbinsY()):
            for ix in range(1,1+template.GetNbinsX()):  
                if self.is2D == 2:
                    if template.GetBinContent(ix,iy)>0.01*tBkgIntegral : 
                        nPop = nPop+1
                    else: 
                        emptyBins.append(template.GetBin(ix,iy))
                else :
                    if template.GetBinContent(ix)>0.01*tBkgIntegral  :  
                        nPop = nPop+1
                    else: 
                        emptyBins.append(template.GetBin(ix))
                    #print "BIN {0} content {1}".format(ix,template.GetBinContent(ix))

        if threshold > 0 and self.addsqrtN :
            #is not QCD
            self.binsysts.append(pname+"_normBins_{0}_{1} lnN ".format(self.theChannel,self.theCat))
            self.binsystsNAME.append(PdfName.replace("bkg_",""))
            self.binsystsVALUE.append(1+1.0/TMath.Sqrt(template.GetEntries()))

            #syst.writeOneLine("QCD", "CMS_HHbbtt_qcd_CR_norm_{0} gmN {1:.0f} ".format(theChannel,inputFile.Get(templateName).Integral()/1.06),1.06)
        #print "POPOLAZIONE", nPop
        if nPop < 2 : return

        for iy in range(1,1+template.GetNbinsY()):
            for ix in range(1,1+template.GetNbinsX()):  
                ## APPLY BIN BY BIN UNCERTAINTY to QCD, EACH BIN SCALED BY ITS OWN UNCERTAINTY 
                nBin=template.GetBin(ix)
                histName = pname+"_{0}_{1}".format(self.theChannel,self.theCat)+str(iy)+str(ix)                                            
                templateUp= template.Clone()
                templateDown=template.Clone()
                #templateUp.Scale(1,"width")
                #templateDown.Scale(1,"width")
                templateUp.SetName(PdfName+"_templ_"+histName+"Up")
                templateUp.SetTitle(PdfName+"_templ_"+histName+"Up")
                templateDown.SetName(PdfName+"_templ_"+histName+"Down")
                templateDown.SetTitle(PdfName+"_templ_"+histName+"Down")
                if(threshold<=0) :
                    templateUp.Sumw2(False)
                    templateUp.SetBinErrorOption(TH1.kPoisson)
                    templateDown.Sumw2(False)
                    templateDown.SetBinErrorOption(TH1.kPoisson)
                error = templateUp.GetBinContent(ix)+templateUp.GetBinErrorUp(ix)
                errorDown = templateDown.GetBinContent(ix) - templateDown.GetBinErrorLow(ix)
                normFactorUp   = templateUp.Integral()+templateUp.GetBinErrorUp(ix)
                normFactorDown = templateDown.Integral()- templateDown.GetBinErrorLow(ix)
                errRate = TMath.Min(templateUp.GetBinErrorUp(ix)/templateUp.GetBinContent(ix),templateDown.GetBinErrorLow(ix)/templateDown.GetBinContent(ix))
                if self.is2D == 2:  
                    nBin = template.GetBin(ix,iy)
                    error = templateUp.GetBinContent(ix,iy)+templateUp.GetBinErrorUp(ix,iy)
                    errorDown = templateDown.GetBinContent(ix,iy) -templateDown.GetBinErrorLow(ix,iy)
                    normFactorUp   = templateUp.Integral()+templateUp.GetBinErrorUp(ix,iy)
                    normFactorDown = templateDown.Integral()- templateDown.GetBinErrorLow(ix,iy)
                    errRate = templateUp.GetBinErrorUp(ix,iy)/templateUp.GetBinContent(ix,iy)
                    if errorDown < 0 : 
                        errorDown=0
                        normFactorDown=0
                #print "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
                #print templateUp.GetBinErrorUp(ix), templateUp.GetBinContent(ix)
                #print errRate
                if nBin in emptyBins : 
                    error = TMath.Min(template.Integral()/template.GetNbinsX(),1)
                    errorDown = 0.0000001
                    errRate = threshold+1

                if errRate < threshold : continue
                #print "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
                #print histName+" shape "
                #print PdfName.replace("bkg_","")
                self.binsysts.append(histName+" shape ")
                self.binsystsNAME.append(PdfName.replace("bkg_",""))
                self.binsystsVALUE.append(1)

                templateUp.SetBinContent(ix,iy,error)
                templateDown.SetBinContent(ix,iy,errorDown)

                templateUp.Scale(tBkgIntegral/templateUp.Integral())
                templateDown.Scale(tBkgIntegral/templateDown.Integral())

                qcdbinUp = ROOT.RooDataHist(histName+"Up",histName+"Up",ral_variableList,templateUp.Clone())
                qcdbinDown = ROOT.RooDataHist(histName+"Down",histName+"Down",ral_variableList,templateDown.Clone())
                qcdbinpdfUp  = ROOT.RooHistPdf(PdfName+"_"+histName+"Up",PdfName+"_"+histName+"Up",ras_variableSet,qcdbinUp)
                qcdbinpdfDown  = ROOT.RooHistPdf(PdfName+"_"+histName+"Down",PdfName+"_"+histName+"Down",ras_variableSet,qcdbinDown)
                if not self.normalize :
                    qcdbinUp.SetNameTitle(PdfName+"_"+histName+"Up",PdfName+"_"+histName+"Up")
                    qcdbinDown.SetNameTitle(PdfName+"_"+histName+"Down",PdfName+"_"+histName+"Down")
                    getattr(w,'import')(qcdbinUp,ROOT.RooFit.RecycleConflictNodes())
                    getattr(w,'import')(qcdbinDown,ROOT.RooFit.RecycleConflictNodes())

                else:
                    getattr(w,'import')(qcdbinpdfUp,ROOT.RooFit.RecycleConflictNodes())
                    getattr(w,'import')(qcdbinpdfDown,ROOT.RooFit.RecycleConflictNodes())


    def makeCardsAndWorkspace(self, theHHLambda, theCat, theChannel, theOutputDir, theInputs):
        
        self.theChannel = theChannel
        self.theCat = theCat
        dname=dc.outputdir
        #theDataSample = "SumDATA"
        theDataSample = "DsingleMu" #"DsingleMu" #"DsingleMuPromptReco"
        if(theChannel) == 3:
            theDataSample = "DTau" #"DsingleTauPromptReco"
        if(theChannel) == 1:
            theDataSample = "DsingleEle" #"DsingleTauPromptReco"

        #theOutLambda = theHHLambda#str(int(theHHLambda))
        #if abs(theHHLambda - int(theHHLambda) )>0.01 : 
        #    theOutLambda = str(int(theHHLambda))+"dot"+ str(int(100*abs(theHHLambda - int(theHHLambda) )))
        #if theHHLambda <0 : 
        #    theOutLambda = "m"+str(abs(int(theHHLambda)))

        #theHHLambda = 20 ##FIXME, waiting for the other samples
        if(self.is2D==2):dname="2D"
        cmd = 'mkdir -p cards{0}/{1}'.format(dname,theOutputDir)
        status, output = commands.getstatusoutput(cmd)    
        ## ----- SETTING AND DECLARATIONS ----
        DEBUG = False
        self.lumi = theInputs.lumi
        self.sqrts = 13
        #...

#        if(theChannel == self.ID_ch_etau ): self.appendName="etau"
#        elif(theChannel == self.ID_ch_mutau ): self.appendName="mutau"
#        elif(theChannel == self.ID_ch_tautau ): self.appendName="tautau"
#        #...
#        else: print "Input Error: Unknown channel!"
#
#
#        if(theCat == self.ID_0Btag ): self.appendName=self.appendName+"0Btag"
#        elif(theCat == self.ID_1Btag ): self.appendName=self.appendName+"1Btag"
#        elif(theCat == self.ID_2Btag ): self.appendName=self.appendName+"2Btag"
#        elif(theCat == self.ID_Boosted ): self.appendName=self.appendName+"Boosted"
#        #...
#        # else: print "Input Error: Unknown category!"

        #"uno" serve sempre
        one = ROOT.RooRealVar("one","one",1.0)
        one.setConstant(True)

        ## -------------------------- IMPORT Histograms ---------------------- ##
        ##NB: if you add a new histogram, you'll need to modify the 2D templates below as well
        inputFile = TFile.Open(self.filename)
        inputSigFile = TFile.Open(self.signalFile)

        #Default
        print theInputs.AllVars, "  ",theInputs.varX, "  ",theInputs.varY
        if theInputs.varY < 0 : 
            var2 = ""
        else :
            var2 = theInputs.AllVars[theInputs.varY]

        #Getting Signal
        nameString = "OS_sig_{0}{1}_OS_{3}_{2}".format(theInputs.AllVars[theInputs.varX],var2,theHHLambda,theInputs.selectionLevel)
        print nameString
        templateSIG = inputSigFile.Get(nameString)
        
        ###FIXME TEST
        #if not self.isResonant:  
        templateSIG.Scale(1.0/100.0)
        ###END TEST

        if self.is2D==1: 
            if "TH2" in templateSIG.ClassName() : templateSIG = templateSIG.ProjectionX()

        ##FIXME: REMOVABLE
        tSigIntegral = templateSIG.Integral()
        templateSIG.Scale(1,"width") 
        if templateSIG.Integral()>0:
            templateSIG.Scale(tSigIntegral/templateSIG.Integral())
        ##FIXME: END REMOVABLE

        binsx = templateSIG.GetNbinsX()
        templateSIG.Scale(self.scale)

        for ibin in range(binsx):
            if templateSIG.GetBinContent(ibin)==0 :
                templateSIG.SetBinContent(ibin, 0.000001)
        rate_signal_Shape = templateSIG.Integral()#*self.lumi #*2.3
        totalRate = float(rate_signal_Shape)
        print " signal rate ", rate_signal_Shape
        theRates = [rate_signal_Shape]
        #templateSIG.Scale(1/tSigIntegral)
        templatesBKG = []
        tobeRemoved = []

        for isample in  theInputs.background:
            #print isample
            nameTemplate = "OS_bkg_{0}{1}_OS_{2}_{3}".format(theInputs.AllVars[theInputs.varX],var2,theInputs.selectionLevel,isample)
            if isample in theInputs.additional :
                #print "ADDITIONAL ",isample
                index = theInputs.additional.index(isample)
                nameTemplate = theInputs.additionalName[index]
            print nameTemplate
            template = inputFile.Get(nameTemplate).Clone()
            template.Scale(self.scale)
            if template.Integral()>0:
                #print isample, " LARGER THAN O"
                if self.is2D == 1 and "TH2" in template.ClassName():
                    template = template.ProjectionX()
                ###FIXME: protection against empty bins####
                for ibin in range(template.GetNbinsX()):
                    if template.GetBinContent(ibin)==0 :
                        template.SetBinContent(ibin, 0.000001)
                theRates.append( template.Integral() ) #*self.lumi
                totalRate = totalRate + theRates[len(theRates)-1]
                print template.Integral()

                #if "QCD" in nameTemplate:
                #    template.Scale(0.5)
                

                #template.Scale(1/template.Integral())
                templatesBKG.append(template)       
            else:
                tobeRemoved.append(isample)
        #protection against empty BKGs
        for isample in tobeRemoved:
            theInputs.background.remove(isample)

        #self.FullRate = 
            #rate_bkgTT_Shape = templateBKG_TT.Integral("width")*self.lumi

        #theRates=[rate_signal_Shape,rate_bkgTT_Shape,rate_bkgDY_Shape,rate_bkgTWantitop_Shape,rate_bkgTWtop_Shape]
        ## -------------------------- SIGNAL SHAPE VARIABLES ---------------------- ##
        binsy = 0
        #print theInputs.AllvarX[theInputs.varX]
        x_name = theInputs.AllVars[theInputs.varX]
        #x = ROOT.RooRealVar(x_name,x_name,float(theInputs.AllvarX[theInputs.varX]),float(theInputs.AllvarY[theInputs.varX]))
        x = ROOT.RooRealVar(x_name,x_name,templateSIG.GetXaxis().GetXmin(),templateSIG.GetXaxis().GetXmax())#float(theInputs.AllvarY[theInputs.varX]))
        x.setVal(250)
        #x.setBins(binsx)#theInputs.AllBins[theInputs.varX])
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
        w.factory("CMS_HHbbtt_scale_tau[-3,3]")
        morphVarList_sig = ROOT.RooArgList()
        MorphList_sig = ROOT.RooArgList()
        TemplateName = "SIG_TempDataHist_{0:.0f}_{1:.0f}_{2:.0f}_{3}".format(theChannel,theCat,self.sqrts,theHHLambda)
        SIG_TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ral_variableList,templateSIG)
        #PdfName = "SIG_TemplatePdf_{0:.0f}_{1:.0f}_{2:.0f}".format(theChannel,self.sqrts,theHHLambda)
        SIG_TemplatePdf = ROOT.RooHistPdf("ggHH_hbbhtt","ggHH_hbbhtt",ras_variableSet,SIG_TempDataHist)
        print templateSIG.Integral()

        nameString = "OS_sig_{0}{1}_tauup_OS_{3}_{2}".format(theInputs.AllVars[theInputs.varX],var2,theHHLambda,theInputs.selectionLevel)
        if self.addTES : self.AddTESShapeSyst(w,"CMS_HHbbtt_scale_tau",self.filename,nameString,"ggHH_hbbhtt",ral_variableList,ras_variableSet)

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
        self.binsysts = []
        self.binsystsNAME = []
        self.binsystsVALUE = []
        #listTemplates = []
        for ibkg in range(len(templatesBKG)):
            TemplateName = "BKG_{3}_TempDataHist_{0:.0f}_{1:.0f}_{2:.0f}_{3}".format(theChannel,theCat,self.sqrts,theHHLambda,theInputs.background[ibkg])
            PdfName = "bkg_{0}".format(theInputs.background[ibkg])

            ## Remove negative bins, normalize and shape uncertainties
            tBkgIntegral = templatesBKG[ibkg].Integral()

            for iy in range(1,1+templatesBKG[ibkg].GetNbinsY()):
                for ix in range(1,1+templatesBKG[ibkg].GetNbinsX()):
                    #print ix, iy
                    if templatesBKG[ibkg].GetBinContent(ix,iy)<=0 : 
                        templatesBKG[ibkg].SetBinContent(ix,iy,0.000001)
                        #print templatesBKG[ibkg].GetBinContent(ix,iy)
            #templatesBKG[ibkg].Scale(1,"width") 
            templatesBKG[ibkg].Scale(tBkgIntegral/templatesBKG[ibkg].Integral(""))

            if theInputs.background[ibkg] == "QCD":

                self.AddBinByBinSyst(w,"CMS_HHbbtt_qcd",PdfName,templatesBKG[ibkg],ral_variableList,ras_variableSet, 0.18) #0.18 ~ 30 events in the bin

                index = theInputs.additional.index("QCD")
                CorrtemplateName = "UP"+theInputs.additionalName[index]
                print CorrtemplateName
                templateUp = inputFile.Get(CorrtemplateName).Clone()
                for iy in range(1,1+templateUp.GetNbinsY()):
                    for ix in range(1,1+templateUp.GetNbinsX()):
                        #print ix, iy
                        if templateUp.GetBinContent(ix,iy)<0 : templateUp.SetBinContent(ix,iy,0.000001)
                templateUp.Scale(tBkgIntegral/templateUp.Integral())
                rlxshapeUp = ROOT.RooDataHist("qcd_dhRlxToTight_{0}_{1}Up".format(theChannel,theCat),"qcd_dhRlxToTight_{0}_{1}Up".format(theChannel,theCat),ral_variableList,templateUp)
                rlxshapepdfUp  = ROOT.RooHistPdf(PdfName+"_CMS_HHbbtt_qcd_RlxToTight_{0}_{1}Up".format(theChannel,theCat),PdfName+"_CMS_HHbbtt_qcd_RlxToTight_{0}_{1}Up".format(theChannel,theCat),ras_variableSet,rlxshapeUp)

                CorrtemplateName = "DOWN"+theInputs.additionalName[index]
                print CorrtemplateName
                templateDown = inputFile.Get(CorrtemplateName)
                for iy in range(1,1+templateDown.GetNbinsY()):
                    for ix in range(1,1+templateDown.GetNbinsX()):
                        #print ix, iy
                        if templateDown.GetBinContent(ix,iy)<0 : templateDown.SetBinContent(ix,iy,0.000001)
                templateDown.Scale(tBkgIntegral/templateDown.Integral())
                rlxshapeDown = ROOT.RooDataHist("qcd_dhRlxToTight_{0}_{1}Down".format(theChannel,theCat),"qcd_dhRlxToTight_{0}_{1}Down".format(theChannel,theCat),ral_variableList,templateDown)
                rlxshapepdfDown  = ROOT.RooHistPdf(PdfName+"_CMS_HHbbtt_qcd_RlxToTight_{0}_{1}Down".format(theChannel,theCat),PdfName+"_CMS_HHbbtt_qcd_RlxToTight_{0}_{1}Down".format(theChannel,theCat),ras_variableSet,rlxshapeDown)
                if not self.normalize :
                    rlxshapeDown.SetNameTitle(PdfName+"_CMS_HHbbtt_qcd_RlxToTight_{0}_{1}Down".format(theChannel,theCat),PdfName+"_CMS_HHbbtt_qcd_RlxToTight_{0}_{1}Down".format(theChannel,theCat))
                    rlxshapeUp.SetNameTitle(PdfName+"_CMS_HHbbtt_qcd_RlxToTight_{0}_{1}Up".format(theChannel,theCat),PdfName+"_CMS_HHbbtt_qcd_RlxToTight_{0}_{1}Up".format(theChannel,theCat))
                    getattr(w,'import')(rlxshapeUp,ROOT.RooFit.RecycleConflictNodes())
                    getattr(w,'import')(rlxshapeDown,ROOT.RooFit.RecycleConflictNodes())
                else:
                    getattr(w,'import')(rlxshapepdfUp,ROOT.RooFit.RecycleConflictNodes())
                    getattr(w,'import')(rlxshapepdfDown,ROOT.RooFit.RecycleConflictNodes())

            else :
                self.AddBinByBinSyst(w,"CMS_HHbbtt_lowStat_{0}".format(ibkg),PdfName,templatesBKG[ibkg],ral_variableList,ras_variableSet, 0.18)
                #nameString = "OS_bkg_{0}{1}_tauup_OS_{3}_{2}".format(theInputs.AllVars[theInputs.varX],var2,theHHLambda,theInputs.selectionLevel)    
                nameString = "OS_bkg_{0}{1}_tauup_OS_{2}_{3}".format(theInputs.AllVars[theInputs.varX],var2,theInputs.selectionLevel,theInputs.background[ibkg])            
                if self.addTES : self.AddTESShapeSyst(w,"CMS_HHbbtt_scale_tau",self.filename,nameString,theInputs.background[ibkg],ral_variableList,ras_variableSet)

            rdhB.append(ROOT.RooDataHist(TemplateName,TemplateName,ral_variableList,templatesBKG[ibkg]))
            rhpB.append(ROOT.RooHistPdf(PdfName,PdfName,ras_variableSet,rdhB[ibkg]))
            if not self.addTES or  theInputs.background[ibkg] == "QCD":
                if not self.normalize :
                    rdhB[ibkg].SetNameTitle(PdfName,PdfName)
                    getattr(w,'import')(rdhB[ibkg],ROOT.RooFit.RecycleConflictNodes())
                else :
                   getattr(w,'import')(rhpB[ibkg],ROOT.RooFit.RecycleConflictNodes())

        ## --------------------------- DATASET --------------------------- ##
        #RooDataSet ds("ds","ds",ras_variableSet,Import(*tree)) ;
        #OS_DATA_tauH_mass_OS_defaultBtagMMNoIsoBBTTCut45_SumDATA
        TemplateName = "OS_DATA_{0}{1}_OS_{2}_{3}".format(theInputs.AllVars[theInputs.varX],var2,theInputs.selectionLevel,theDataSample)
        print TemplateName
        templateObs = inputFile.Get(TemplateName)
        print templateObs
        obsEvents = templateObs.Integral() #templateObs.GetEntries()
        print "observations: ", obsEvents    

        #data_obs = inputFile.Get(TemplateName)
        #templateObs.Add(inputFile.Get("OS_DATA_{0}{1}_OS_{2}_{3}".format(theInputs.AllVars[theInputs.varX],var2,theInputs.selectionLevel,chanst)))
        #if templateObs.Integral() <=0: #protection for low stat
        #    data_obs = rhpB[0].generate(ras_variableSet,1000)
###        tempfile = TFile.Open("temp.root","RECREATE")
###        if self.is2D==1: 
###            if "TH2" in templateObs.ClassName() : 
###                templateObs = templateObs.ProjectionX()
###            obstree=TNtuple("obstree","obstree",x_name)
###            for idata in range(1,templateObs.GetNbinsX()+1):
###                gbc = int(templateObs.GetBinContent(idata))
###                if gbc >0:
###                    for ig in range(gbc): 
###                        obstree.Fill(templateObs.GetXaxis().GetBinCenter(idata))
###        else:
###            obstree=TNtuple("obstree","obstree",x_name+":"+y_name)
###            for idata in range(1,templateObs.GetNbinsX()+1):
###                for idatay in range(1,templateObs.GetNbinsY()+1):
###                    gbc = int(templateObs.GetBinContent(idata,datay))
###                    if gbc >0:
###                        for ig in range(gbc): 
###                            obstree.Fill(templateObs.GetXaxis().GetBinCenter(idata),templateObs.GetXaxis().GetBinCenter(idatay))
###        print templateObs.GetEntries()
###        data_obs = RooDataSet("data_obs","data_obs",obstree,ras_variableSet)    
        rdh_obs = ROOT.RooDataHist("rdh"+TemplateName,"rdh"+TemplateName,ral_variableList,templateObs)
        print rdh_obs.numEntries()
        #rhp_obs = ROOT.RooHistPdf("rhp_obs","rhp_obs",ras_variableSet,rdh_obs)
        #data_obs = rhp_obs.generate(ras_variableSet)
        #datasetName = "data_obs_{0}".format(self.appendName)
        #data_obs.SetNameTitle("data_obs","data_obs")

        #data_obs = ROOT.RooDataSet(datasetName,datasetName,data_obs_tree,ras_variableSet)
        #obsEvents = rdh_obs.numEntries() #GetEntries() #data_obs.numEntries()        

        ## --------------------------- WORKSPACE -------------------------- ##
        #getattr(w,'import')(data_obs,ROOT.RooFit.Rename("data_obs"))
        #getattr(w,'import')(data_obs)
        getattr(w,'import')(rdh_obs,ROOT.RooFit.Rename("data_obs"))
        if not self.addTES :
         getattr(w,'import')(SIG_TemplatePdf,ROOT.RooFit.RecycleConflictNodes())
        ##FIXME!!!!!!
        #name_ShapeWS = "cards{3}/{0}/hh_{1}_L{2:.0f}_13TeV.input.root".format(theOutputDir,theChannel,theHHLambda,dname)
        #name_ShapeDC = "cards{3}/{0}/hh_{1}_L{2:.0f}_13TeV.txt".format(theOutputDir,theChannel,theHHLambda,dname)
        #string_ShapeWS = "hh_{0}_L{1:.0f}_13TeV.input.root".format(theChannel,theHHLambda)
        #string_ShapeDC = "hh_{0}_L{1:.0f}_13TeV.txt".format(theChannel,theHHLambda)
        name_ShapeWS = "cards{4}/{0}/hh_{1}_C{2}_L{3}_13TeV.input.root".format(theOutputDir,theChannel,theCat,theHHLambda,dname)
        name_ShapeDC = "cards{4}/{0}/hh_{1}_C{2}_L{3}_13TeV.txt".format(theOutputDir,theChannel,theCat,theHHLambda,dname)
        string_ShapeWS = "hh_{0}_C{1}_L{2}_13TeV.input.root".format(theChannel,theCat,theHHLambda)
        string_ShapeDC = "hh_{0}_C{1}_L{2}_13TeV.txt".format(theChannel,theCat,theHHLambda)

        ## --------------------------- DATACARD -------------------------- ##
        file = open( name_ShapeDC, "wb")

        #channelList=theInputs.background #['sig','bkg_TT','bkg_DY'] 
        channelName=['ggHH_hbbhtt']#'sig','bkg_TT','bkg_DY','bkg_TWantitop','bkg_TWtop']
        for isample in theInputs.background:
            channelName.append('bkg_'+isample)
        print channelName
        print theRates

        file.write("imax 1\n")
        file.write("jmax {0}\n".format(len(channelName)-1))
        file.write("kmax *\n")
        
        file.write("------------\n")
        #file.write("shapes * * {0} w:$PROCESS w:$PROCESS_$SYSTEMATIC\n".format(name_ShapeWS))
        file.write("shapes * * {0} w:$PROCESS w:$PROCESS_$SYSTEMATIC\n".format(string_ShapeWS))
        file.write("------------\n")
        

        file.write("bin a{0} \n".format(theChannel))
        file.write("observation {0} \n".format(obsEvents))
        
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
        syst = systReader("../config/systematics.cfg",[theHHLambda],theInputs.background,file)
        #syst = systReader("../config/systematics_test.cfg",['Lambda{0}'.format(theOutLambda)],theInputs.background) 
        if(theChannel == self.ID_ch_tautau ): 
            systChannel = systReader("../config/systematics_tautau.cfg",[theHHLambda],theInputs.background,file)
        elif(theChannel == self.ID_ch_mutau ): 
            systChannel = systReader("../config/systematics_mutau.cfg",[theHHLambda],theInputs.background,file)
            if(self.isResonant):
                syst_res=systReader("../config/systematics_resonant.cfg",[theHHLambda],theInputs.background,file)
            else : syst_res=systReader("../config/systematics_nonresonant.cfg",[theHHLambda],theInputs.background,file)
        elif(theChannel == self.ID_ch_etau ): 
            systChannel = systReader("../config/systematics_etau.cfg",[theHHLambda],theInputs.background,file)
            if(self.isResonant):
                syst_res=systReader("../config/systematics_resonant.cfg",[theHHLambda],theInputs.background,file)
            else : syst_res=systReader("../config/systematics_nonresonant.cfg",[theHHLambda],theInputs.background,file)
        syst.writeSystematics()
        systChannel.writeSystematics()
        if(self.writeThSyst) :
            syst_th = systReader("../config/syst_th.cfg",[theHHLambda],theInputs.background,file)
            syst_th.writeSystematics()
        if(theChannel == self.ID_ch_mutau or theChannel == self.ID_ch_etau):
            syst_res.writeSystematics()

        for iqcd in range(len(self.binsysts)) :
            syst.writeOneLine(self.binsystsNAME[iqcd],self.binsysts[iqcd],self.binsystsVALUE[iqcd])


        index = theInputs.additional.index("QCD")
        templateName = theInputs.additionalName[index]
        if inputFile.Get(templateName).Integral() > 0:        
            #add QCD bin-by-by shape
            #add QCD rlx to tight shape unc
            syst.writeOneLine("QCD","CMS_HHbbtt_qcd_RlxToTight_{0}_{1} shape ".format(theChannel,theCat))
            #add stat uncertainty (sqrt(N)) => is this needed?? In any case it can't be added as gamma (lnU instead?)
            #syst.writeOneLine("QCD", "CMS_HHbbtt_qcd_SR_norm_{0} lnN ".format(theChannel),1+1.0/TMath.Sqrt(inputFile.Get(templateName).Integral()))
            #add  CR->SR uncertainty (gamma*alhpa)
            #templateName = templateName.replace("CORR_","")###GRRR this is hardcoded... but I don't know how to do better
            syst.writeOneLine("QCD", "CMS_HHbbtt_qcd_CR_norm_{0}_{1} gmN {2:.0f} ".format(theChannel,theCat,inputFile.Get(templateName).Integral()/float(theInputs.SSOSfactor),float(theInputs.SSOSfactor)))
        readChTmplate = "MuTau"
        if(thechannel == self.ID_ch_etau): readChTmplate = "ETau"
        elif (thechannel == self.ID_ch_tautau): readChTmplate = "TauTau"
        #self.AddSingleShapeSyst(w,syst,"CMS_HHbbtt_pttopreweight","/home/llr/cms/cadamuro/HHKlubAnalysis/CMSSW_7_4_7/src/KLUBAnalysis/studies/TopPtReweight/topPtShapes_{0}.root".format(readChTmplate),"h","TT",ral_variableList,ras_variableSet)
        self.AddSingleShapeSyst(w,syst,"CMS_HHbbtt_pttopreweight",self.filename,"OS_bkg_{0}_OS_{1}".format(theInputs.AllVars[theInputs.varX],theInputs.selectionLevel),"TT",ral_variableList,ras_variableSet,"_top","_TT")

        ###
        no_teschannels = ["bkg_QCD"]
        if theChannel == self.ID_ch_etau or theChannel == self.ID_ch_mutau :
            no_teschannels.append("bkg_WJet")     
            no_teschannels.append("bkg_WWToLNuQQ")     
            no_teschannels.append("bkg_WZTo1L3Nu")     
            no_teschannels.append("bkg_WZTo1L1Nu2Q")     
        teschannels = channelName
        for notes in no_teschannels :
            if notes in teschannels :
                teschannels.remove(notes)
        teschannels.append(theHHLambda)
        syst.writeOneLine(teschannels,"CMS_HHbbtt_scale_tau"+" shape ")
        ###
        w.writeToFile(name_ShapeWS)

#define function for parsing options
def parseOptions():

    usage = ('usage: %prog [options] datasetList\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)
    
    parser.add_option('-d', '--is2D',   dest='is2D',       type='int',    default=1,     help='number of Dimensions (default:1)')
    parser.add_option('-f', '--filename',   dest='filename',   type='string', default="",  help='input plots')
    parser.add_option('-S', '--sigfilename',dest='sigfilename',type='string', default="",  help='input signalplots')
    parser.add_option('-l', '--lambda',   dest='Lambda', type='string', default="",  help='Lambda name')
    parser.add_option('-c', '--channel',   dest='channel', type='string', default='MuTau',  help='final state')
    parser.add_option('-b', '--category',   dest='category', type='int', default='999',  help='btag category')
    parser.add_option('-i', '--config',   dest='config', type='string', default='',  help='config file')
    parser.add_option('-o', '--selection', dest='overSel', type='string', default='', help='overwrite selection string')
    parser.add_option('-v', '--variable', dest='overVar', type='string', default='bH_mass', help='overwrite plot variable (only1D)')
    parser.add_option('-s', '--scale', dest='scale', type='float', default='1', help='scale templates')
    parser.add_option('-q', '--dir', dest='outDir', type='string', default='', help='outdput dir')
    parser.add_option('-t', '--theory', dest='theorySyst', type='int', default=1, help='write theory systematics in the card')
    parser.add_option('-r', '--resonant', dest='resAnalysis', type='int', default=0, help='Resonant analysis')

    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()

    if (opt.is2D != 1 and opt.is2D != 2):
        print 'The input '+opt.is2D+' is unkown for is2D.  Please choose 1 or 2. Exiting...'
        sys.exit()
    #if (opt.filename==""):
    #    print "you MUST specify an input file [please use -f option]"
    #    sys.exit()
    if opt.category>100:
        if "2b0j" in opt.overSel : opt.category = 2
        elif "1b1j" in opt.overSel : opt.category = 1
        elif "boosted" in opt.overSel : opt.category = 3

# run as main
if __name__ == "__main__":
    parseOptions()
    global opt, args
    #(opt, args) = parser.parse_args()
    dc =cardMaker()
    dc.loadIncludes()
    dc.set2D(opt.is2D)
    dc.scale=opt.scale
    dc.outputdir="_"+opt.channel+opt.outDir
    if opt.sigfilename=="":
        dc.signalFile = opt.filename
    else : dc.signalFile = opt.sigfilename

    if(opt.config==""):
        configname = "../config/analysis_"+opt.channel+".cfg"
    else :
        configname = opt.config
    input = configReader(configname)

    #input = configReader("../config/analysis_TESTMT.cfg")
    thechannel = 1
    if opt.channel == "MuTau" : thechannel=2
    elif opt.channel == "TauTau" : thechannel = 3

    input.readInputs()

    if opt.filename == "":
        filename = "../"+input.inputFolder+"/outPlotter.root"
    else: filename = opt.filename
    dc.setfileName(filename)

    #Optionally overwrite config instructions (for testing purposes)
    if opt.overSel is not "":
        #print "OVERSEL="+opt.overSel
        input.selectionLevel = opt.overSel
        for iad in range(len(input.additionalName)) :
            input.additionalName[iad] = re.sub('dijethardisoBtagCutM',opt.overSel,input.additionalName[iad])
            input.additionalName[iad] = re.sub('RunD',"",input.additionalName[iad])
    if opt.is2D == 1 and opt.overVar is not "bH_mass" :
        #print "INSIDE OPTVAR"
        for ivx in range(len(input.AllVars)):
            #print input.AllVars[ivx],"   ",opt.overVar
            if input.AllVars[ivx] == re.sub('_','',opt.overVar) :
                #print "FOUND new VAR ", ivx,"  ",opt.overVar
                input.varX = ivx
                input.AllVars[ivx] = opt.overVar
                for iad in range(len(input.additionalName)) :
                    input.additionalName[iad] = re.sub('bH_mass',opt.overVar,input.additionalName[iad])
    if opt.theorySyst > 0: 
        dc.writeThSyst = True
    else : dc.writeThSyst = False
    if opt.resAnalysis > 0 :
        dc.isResonant = True

    print input.signals
    if opt.Lambda == "" :
        for signal in input.signals :
            dc.makeCardsAndWorkspace(signal,opt.category,thechannel,"{0}{1}{2}".format(signal,opt.overSel,opt.overVar),input)
    else : 
        print opt.Lambda
        dc.makeCardsAndWorkspace(opt.Lambda,opt.category,thechannel,"{0}{1}{2}".format(opt.Lambda,opt.overSel,opt.overVar),input)
