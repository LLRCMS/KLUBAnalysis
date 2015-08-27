#!/usr/bin/python
import os
import re
import math
import collections
from ROOT import *
from array import array

class systReader:

    def __init__(self, inputTextFile,signalList,backgroundList):

        if not os.path.exists(inputTextFile):
            raise RuntimeError, "File {0} does not exist!!!".format(inputTextFile)
        
        # input file
        self.theInput = inputTextFile

        #list of all channels
        #self.DY_chan = False
        #self.TT_chan = False
        #self.TWtop_chan = False
        #self.TWantitop_chan = False
        #self.Tt_chan = False
        #self.WW2l2v_chan = False
        #self.WWlvqq_chan = False
        #self.WWinc_chan = False
        #self.WZinc_chan = False
        #self.WJet_chan = False
        self.channels = signalList+backgroundList
        #self.OutputLines = []

    def writeSystematics(self, theOutputFile):
        print "reading systematics"
        section = ""
        outputLine = ""
        OutputLines = []
        systLine={'ggH':"- "}
        for chan in self.channels :
            systLine[chan] = "- "

        #varYname = ""
        #varnames = []
        for line in open(self.theInput,'r'):
            f = line.split()

            if len(f) < 1: continue
            if f[0].startswith("#"): continue

            if f[0].startswith('['):
                f = re.split('\W+',line)
                section = f[1]
                    
                if outputLine is not "" :
                    #OutputLines.append(outputLine)
                    for chan in self.channels :
                        outputLine += systLine[chan]
                    OutputLines.append(outputLine)
                    systLine[chan] = "- "
                    outputLine = ""

                print "writing syst for ", section
                continue
            
            if f[0] == "type":
                #print "TYPE: ",f
                outputLine+=section+" "+f[2]+" "
                continue
            elif f[0] == "param":
                outputLine=section+line
                continue
            for chan in self.channels:
                #print "SYST: ",f
                if chan == f[0]: systLine[chan] = " "+f[2]+" "

        for chan in self.channels :
            if not "param" in outputLine: outputLine += systLine[chan]
        OutputLines.append(outputLine)

        for line in OutputLines:
            theOutputFile.write(line+"\n")

