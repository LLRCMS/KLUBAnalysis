#!/usr/bin/python
import os
import re
import math
import collections
from ROOT import *
from array import array

class systReader:

    def __init__(self, inputTextFile, signalList, backgroundList, outputFile):
        if not os.path.exists(inputTextFile):
            raise RuntimeError('File {} does not exist!!!'.format(inputTextFile))
        
        # input file
        self.theInput = [inputTextFile]
        self.channels = signalList + backgroundList
        self.out = outputFile
        self.writeOut = True
        self.printResult = False

        self.SystNames = []
        self.SystTypes = []
        self.SystProcesses = []
        self.SystValues = []

    def addSystFile(self, theOtherFile):
        self.theInput.append(theOtherFile)

    def writeSystematics(self):
        section = ""
        outputLine = ""
        OutputLines = []
        activeProc, activeVal = ([] for _ in range(2))
        systLine = {'ggH': '- '}
        for chan in self.channels :
            systLine[chan] = "- "

        for ifile in range(len(self.theInput)):
            section = ""
            outputLine = ""
            activeProc = []
            activeVal = []
            for chan in self.channels :
                systLine[chan] = '- '
            for line in open(self.theInput[ifile],'r'):
                f = line.split()
                if len(f) < 1: continue
                if f[0].startswith('#'):
                    continue

                if f[0].startswith('['):
                    f = re.split('\W+',line)
                    section = f[1]
                    newSection = True

                    if outputLine != '':
                        #OutputLines.append(outputLine)
                        for chan in self.channels :
                            outputLine += systLine[chan]
                            systLine[chan] = '- '

                        OutputLines.append(outputLine)
                        if self.printResult: 
                            print(outputLine)
                        outputLine = ""
                        self.SystProcesses.append(activeProc)
                        self.SystValues.append(activeVal)
                        activeProc = []
                        activeVal = []

                    if self.printResult: 
                        print('Systematics section: {}'.format(section))
                    self.SystNames.append(section)
                    continue
                
                if f[0] == 'type':
                    outputLine += section + ' ' + f[2] + ' '
                    self.SystTypes.append(f[2])
                    continue
                elif f[0] == 'param':
                    outputLine = section + ' ' + line
                    continue

                for chan in self.channels:
                    if chan == f[0]: 
                        systLine[chan] = ' ' + f[2] + ' '
                        activeProc.append(f[0])
                        activeVal.append(f[2])

            for chan in self.channels:
                if not 'param' in outputLine: 
                    outputLine += systLine[chan]
            OutputLines.append(outputLine)

            if self.printResult: 
                print(outputLine)

            self.SystProcesses.append(activeProc)
            self.SystValues.append(activeVal)

        if self.printResult:
            for line in OutputLines:
                print(outputLine)

        if self.writeOut:
            for line in OutputLines:
                self.out.write(line+'\n')

    def writeOneLine(self,channel,string,value=1):
        outputLine = string
        for chan in self.channels :
            if (chan in channel) or ('bkg_'+chan in channel) :
                outputLine += str(value)+" "
            else:
                outputLine += "- "
        self.out.write(outputLine + '\n')

    def writeOutput(self,doWrite) :
        self.writeOut = doWrite

    def verbose(self, v) :
        self.printResult = v
