#!/usr/bin/python
import os
import re
import math
import collections
from ROOT import *
from array import array

## ---------------------------------------------------------------
## card reader class
## ---------------------------------------------------------------

class configReader:

    def __init__(self, inputTextFile):

        if not os.path.exists(inputTextFile):
            raise RuntimeError, "File {0} does not exist!!!".format(inputTextFile)
        
        # input file
        self.theInput = inputTextFile
        # lumi
        self.lumi = -999.9
        # sqrts
        #self.sqrts = -999.9
        # channels
        self.signals=[]
        self.background=[]
        self.datasamples=[]
        self.selections=[]
        self.additional = []
        self.additionalName = []

        # Variables
        self.AllVars = []
        self.AllBins = []
        self.AllvarX = []
        self.AllvarY = []
        self.varX = -999
        self.varY = -999
        self.selectionLevel = ""
        self.dimensions = -999
 
        # systematics 
        self.lumiUnc = -999.9
        self.SSOSfactor = 1.06

        self.CMS_zz4l_mean_m_sig = -999.9
        self.CMS_zz4l_sigma_m_sig = -999.9
        self.CMS_zz4l_mean_e_sig = -999.9
        self.CMS_zz4l_sigma_e_sig = -999.9

        self.useLumiUnc = False
        self.useCMS_zz4l_mean = False
        self.useCMS_zz4l_sigma = False

        self.inputFolder = ""

    def goodEntry(self,variable):
        if variable == -999.9:
            return False
        else:
            return True


    def parseBoolString(self,theString):

        return theString[0].upper()=='T'


    def readInputs(self):
        print "reading inputs"
        section = ""
        varXname = ""
        varYname = ""
        varnames = []
        for line in open(self.theInput,'r'):
            f = line.split()

            if len(f) < 1: continue
            if f[0].startswith("#"): continue

            if f[0].startswith('['):
                #print f
                #f = re.split('\W+',line)
                f = re.split('\W+',line)
                #print f
                section = f[1]
                print section
                continue
            if section is not "histos":
                if section is not "cardmaker" : f = re.split('\W+',line)
            #f = re.split('\W+',line)
            if section == "general":
                #f = re.split('\W+',line)
                #if f[0] == 'signalsFitted' : 
                if f[0] == 'signals' : 
                    for sample in range(1,len(f)-1): self.signals.append(f[sample])
                elif f[0] == "backgrounds": 
                    for sample in range(1,len(f)-1): self.background.append(f[sample])
                elif f[0] == "data": 
                    for sample in range(1,len(f)-1): self.datasamples.append(f[sample])
                elif f[0] == "outputFolderName":
                    self.inputFolder = f[1] 
                elif f[0] == "variables":
                    for iword in range(1,len(f)-1): varnames.append(f[iword])
                    #print varnames
 #               for chan in f:
 #                   if chan == f[0]: continue
 #                   if chan.lower().startswith("ggh"):     self.ggH_chan = True
 #                   elif chan.lower().startswith("qqh"):   self.qqH_chan = True
 #                   elif chan.lower().startswith("wh"):    self.WH_chan = True
 #                   elif chan.lower().startswith("zh"):    self.ZH_chan = True
 #                   elif chan.lower().startswith("tth"):   self.ttH_chan = True
 #                   elif chan.lower().startswith("qqzz"):  self.qqZZ_chan = True
 #                   elif chan.lower().startswith("ggzz"):  self.ggZZ_chan = True
 #                   elif chan.lower().startswith("zjets"): self.zjets_chan = True
 #                   elif chan.lower().startswith("ttbar"): self.ttbar_chan = True
 #                   elif chan.lower().startswith("zbb"):   self.zbb_chan = True
 #                   elif chan.lower().startswith("all"):   self.all_chan = True
 #                   else : raise RuntimeError, "Unknown channel {0}, choices are ggH, qqH, WH, ZH, ttH, qqZZ, ggZZ, zjets".format(chan)
  	        #continue
            #if section == "stitch":
            #    if f[0] not in self.background :
            #        self.background.append(f[0])
                if f[0] == "additionalBkgs":
                    for sample in range(1,len(f)-1): 
                        self.background.append(f[sample])
                        self.additional.append(f[sample])

                if f[0] == "additionalName" :
                    for sample in range(1,len(f)-1): self.additionalName.append(f[sample])

                if f[0] == "selections" :
                    for sample in range(1,len(f)-1): 
                        if "BDT" in f[sample] or "boosted" in f[sample] :
                            self.selections.append(f[sample])

            if section == "evalQCD":
                if f[0] == "outputFolderName":
                    self.inputFolder = self.inputFolder + "/" +f[1] 
                if f[0] == "SStoOSscaleFactor":
                    self.SSOSfactor = f[1]
            #if section == "systematics": #devo capire che formato sare    
            #    continue
            #print f 
            if section == "merge" or section == "pp_merge":
                if not "data_obs" in f[0] : 
                    self.background.append(f[0])
                    for ibmerge in range(1, len(f)):
                        if f[ibmerge] is not "":
                            #print self.background, ibmerge, f,f[ibmerge]
                            self.background.remove(f[ibmerge])                 
            if f[0] == "lumi" :
                self.lumi = float(f[1])
                print "lumi ", self.lumi
            #print section
            if section == "histos" or section == "binning":
                #print f
                self.AllVars.append(f[0]) #f[1]="="
                self.AllBins.append(re.sub(',', '', f[1]))
                self.AllvarX.append(re.sub(',', '', f[2]))
                self.AllvarY.append(re.sub(',', '', f[3]))
                
        self.AllVars = varnames
        self.background.append("QCD")
        #if len(self.additionalName)<1: self.additionalName.append("QCD")

#        for iname in range(len(self.AllVars)):
#            #print self.AllVars[iname]#, varXname
#            #varnames = f[2].split(':')
#            #print varnames
#            if self.AllVars[iname] == re.sub('_','',varnames[0]):
#                    #print "GOTCHA"
#                    self.varX = iname
#                    self.AllVars[iname] = varnames[0]
#            elif self.dimensions>1:
#                    if self.AllVars[iname] == re.sub('_','',varnames[1]):
#                        self.varY = iname
#                        self.AllVars[iname] = varnames[1]


    
    def getInputs(self):

        dict = {}

        ## check settings ##
        
        if self.varX <0:
            print "Variable X is not set. Using {0} for X.".format("HHKinMass")
            self.AllVars[0] = "HHKinMass"
        if self.cardsselection == "":
            print "Selection level not set. Using all instead"
            self.selectionLevel = "all"

      ## Set dictionary entries to be passed to datacard class ##
        
        dict['lumi'] = float(self.lumi)
        #dict['varX'] = self.varX
#        dict['signals'] = self.signals


        return dict



class systReader:

    def __init__(self, inputTextFile,signalList,backgroundList,outputFile):

        if not os.path.exists(inputTextFile):
            raise RuntimeError, "File {0} does not exist!!!".format(inputTextFile)
        
        # input file
        self.theInput = [inputTextFile]
        self.channels = signalList+backgroundList
        self.theOutputFile = outputFile
        #self.OutputLines = []
        self.writeOut = True
        self.printResult = True

        self.SystNames = []
        self.SystTypes = []
        self.SystProcesses = []
        self.SystValues = []

    def addSystFile(self, theOtherFile):
        self.theInput.append(theOtherFile)

    def writeSystematics(self):
        print "reading systematics"
        section = ""
        outputLine = ""
        OutputLines = []
        activeProc = []
        activeVal = []
        systLine={'ggH':"- "}
        for chan in self.channels :
            systLine[chan] = "- "

        #varYname = ""
        #varnames = []
        for ifile in range(len(self.theInput)):
            print "reading systematics"
            section = ""
            outputLine = ""
            activeProc = []
            activeVal = []
            for chan in self.channels :
                systLine[chan] = "- "
            for line in open(self.theInput[ifile],'r'):
                f = line.split()

                if len(f) < 1: continue
                if f[0].startswith("#"): continue

                if f[0].startswith('['):
                    f = re.split('\W+',line)
                    section = f[1]
                    newSection = True

                    #print "before ",outputLine
                    if outputLine is not "" :
                        #OutputLines.append(outputLine)
                        for chan in self.channels :
                            outputLine += systLine[chan]
                            systLine[chan] = "- "

                        OutputLines.append(outputLine)
                        if self.printResult : print outputLine
                        outputLine = ""
                        self.SystProcesses.append(activeProc)
                        self.SystValues.append(activeVal)
                        activeProc = []
                        activeVal = []

                    if self.printResult : print "writing syst for ", section
                    self.SystNames.append(section)
                    continue
                
                if f[0] == "type":
                    #print "TYPE: ",f
                    outputLine+=section+" "+f[2]+" "
                    self.SystTypes.append(f[2])
                    continue
                elif f[0] == "param":
                    outputLine=section+" "+line
                    continue
                for chan in self.channels:
                    #print "SYST: ",f
                    if chan == f[0]: 
                        systLine[chan] = " "+f[2]+" "
                        activeProc.append(f[0])
                        activeVal.append(f[2])
            #indent qui
            for chan in self.channels :
                if not "param" in outputLine: 
                    outputLine += systLine[chan]
            OutputLines.append(outputLine)
            if self.printResult : print outputLine
            self.SystProcesses.append(activeProc)
            self.SystValues.append(activeVal)
        if self.printResult : 
            for line in OutputLines:
                print outputLine
        if self.writeOut:
            for line in OutputLines:
                self.theOutputFile.write(line+"\n")

    def writeOneLine(self,channel,string,value=1):
        outputLine = string
        #print "channels:", channel
        for chan in self.channels :
            #print chan
            if (chan in channel) or ("bkg_"+chan in channel) :
                outputLine += str(value)+" "
            else:
                outputLine += "- "
        self.theOutputFile.write(outputLine+"\n")

    def writeOutput(self,doWrite) :
        self.writeOut = doWrite

    def verbose(self, v) :
        self.printResult = v






