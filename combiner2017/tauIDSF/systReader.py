#!/usr/bin/python
import os
import re
import math
import collections
from ROOT import *
#from array import array

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
                #print f
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






