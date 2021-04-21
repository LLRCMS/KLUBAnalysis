#!/usr/bin/python
import os
import re
import math
import collections
import ConfigParser
from ROOT import *
from array import array

class systReader:

    def __init__(self, inputTextFile,signalList,backgroundList,outputFile,renameH):

        if not os.path.exists(inputTextFile):
            raise RuntimeError, "File {0} does not exist!!!".format(inputTextFile)
        
        # input file
        self.theInput = [inputTextFile]
        self.processes = signalList+backgroundList
        self.theOutputFile = outputFile
        self.renameSingleH = renameH
        self.writeOut = True
        self.printResult = True
        self.theQCDdict = {}
        self.theVBFdipoledict = {}

        self.SystNames = []
        self.SystTypes = []
        self.SystProcesses = []
        self.SystValues = []

    def addSystFile(self, theOtherFile):
        self.theInput.append(theOtherFile)

    def writeSystematics(self):
        print "- Reading systematics -"
        section = ""
        outputLine = ""
        OutputLines = []
        activeProc = []
        activeVal = []
        systLine = {}

        # At first set all syst values to '-' for all processes
        for proc in self.processes :
            systLine[proc] = "- "

        # Read config files to parse the syst and their values
        for ifile in range(len(self.theInput)):
            section = ""
            outputLine = ""
            activeProc = []
            activeVal = []

            # Set all syst values to '-' for all processes
            # (for safety and in case of multiple syst files)
            for proc in self.processes :
                systLine[proc] = "- "

            # Actually parse the file line by line
            for line in open(self.theInput[ifile],'r'):
                f = line.split()
                #print ' Line: ', f

                # Skip empty lines
                if len(f) < 1: continue

                # Skip commented lines
                if f[0].startswith("#"): continue

                # If new section (i.e. new systematic) start parsing
                if f[0].startswith('['):
                    f = re.split('\W+',line)
                    section = f[1]

                    # If outputLine is filled already ('sysName sysType') add correct
                    # values for each process and add outputLine to OutputLines.
                    # Also add to activeProcesses the processes from this section (i.e. this syst)
                    #print " Before outputLine: ", outputLine
                    if outputLine is not "" :
                        for proc in self.processes :
                            outputLine += systLine[proc]
                            systLine[proc] = "- "
                        OutputLines.append(outputLine)
                        self.SystProcesses.append(activeProc)
                        self.SystValues.append(activeVal)
                        #if self.printResult : print " ", outputLine

                        # Restore outputLine and activeProcvesses before going to next section
                        outputLine = ""
                        activeProc = []
                        activeVal = []

                    # If outputLine is empty (at the beginning or after every section) parse the next line
                    if self.printResult : print " Writing syst for ", section
                    self.SystNames.append(section)
                    continue

                # Else (not new section) start filling the outputLine with 'sysName sysType'...
                if f[0] == "type":
                    #print " TYPE: ", f
                    outputLine+=section+" "+f[2]+" "
                    self.SystTypes.append(f[2])
                    continue

                elif f[0] == "param":
                    outputLine=section+" "+line
                    continue

                # ... and parse the values of this syst for each process
                #print " SYST: ", f
                for proc in self.processes:
                    if self.renameSingleH:
                        if f[0] == 'ggH': f[0] = f[0]+'_htt'
                        if f[0] == 'qqH': f[0] = f[0]+'_htt'
                        if f[0] == 'WH' : f[0] = f[0]+'_htt'
                        if f[0] == 'ZH' : f[0] = f[0]+'_hbb'
                        if f[0] == 'ttH': f[0] = f[0]+'_hbb'
                    if proc == f[0]:
                        systLine[proc] = " "+f[2]+" "
                        activeProc.append(f[0])
                        activeVal.append(f[2])

            # The last systematic parsed needs to be dumped in the outputLine
            # after the loop on all lines and it's also added to the OutputLines.
            # Also add to activeProcesses the processes from this last systematic parsed
            for proc in self.processes :
                if not "param" in outputLine: 
                    outputLine += systLine[proc]
            OutputLines.append(outputLine)
            self.SystProcesses.append(activeProc)
            self.SystValues.append(activeVal)
            #if self.printResult : print " ", outputLine

        if self.printResult:
            print " Parsed systematics:"
            for line in OutputLines:
                print " ", line

        if self.writeOut:
            for line in OutputLines:
                self.theOutputFile.write(line+"\n")

    def addQCDSystFile(self, theQCDFile):
        if not os.path.exists(theQCDFile):
            raise RuntimeError, "File {0} does not exist!!!".format(theQCDFile)

        # Read from config and save in dictionary
        config = ConfigParser.SafeConfigParser()
        config.read(theQCDFile)

        # Get sections and iterate over each
        sections = config.sections()
        for section in sections:
            options = config.options(section)
            temp_dict = {}
            for option in options:
                temp_dict[option] = config.get(section,option)
            self.theQCDdict[section] = temp_dict

    def writeQCDSystematics(self, theChan,theCat):
        print "- Reading QCD systematics -"
        outputLine = ""
        OutputLines = []
        activeProc = []
        activeVal = []
        systLine = {}

        if len(self.theQCDdict) == 1:
            raise RuntimeError, "Dictionary for QCD systematics is empty!!!"

        sectionname = theChan+'_'+theCat
        QCDuncs = self.theQCDdict[sectionname].keys()

        for unc in QCDuncs:
            f = re.split(':',self.theQCDdict[sectionname][unc])
            systname = f[0]
            systval = f[1]

            outputLine = systname + " lnN "
            for proc in self.processes:
                if proc == "QCD":
                    outputLine += " "+systval+" "
                    activeProc.append("QCD")
                    activeVal.append(systval)
                else:
                    outputLine += "- "

            OutputLines.append(outputLine)
            self.SystNames.append(systname)
            self.SystTypes.append("lnN")
            self.SystProcesses.append(activeProc)
            self.SystValues.append(activeVal)

            # Restore outputLine and activeProcvesses before going to next section
            outputLine = ""
            activeProc = []
            activeVal = []

        if self.printResult:
            print " Parsed QCD systematics:"
            for line in OutputLines:
                print " ", line

    def addVBFdipoleSystFile(self, theVBFFile):
        if not os.path.exists(theVBFFile):
            raise RuntimeError, "File {0} does not exist!!!".format(theVBFFile)

        # Read from config and save in dictionary
        config = ConfigParser.SafeConfigParser()
        config.read(theVBFFile)

        # Get sections and iterate over each
        sections = config.sections()
        for section in sections:
            options = config.options(section)
            temp_dict = {}
            for option in options:
                temp_dict[option] = config.get(section,option)
            self.theVBFdipoledict[section] = temp_dict

    def writeVBFdipoleSystematics(self,theChan,theCat):
        print "- Reading VBFdipole systematics -"
        outputLine = ""
        OutputLines = []
        activeProc = []
        activeVal = []
        systLine = {}

        if len(self.theVBFdipoledict) == 1:
            raise RuntimeError, "Dictionary for VBFdipole systematics is empty!!!"

        sectionname = theChan+'_'+theCat
        VBFuncs = self.theVBFdipoledict[sectionname].keys()

        for unc in VBFuncs:
            f = re.split(':',self.theVBFdipoledict[sectionname][unc])
            systname = f[0]
            systval = f[1]

            outputLine = systname + " lnN "
            for proc in self.processes:
                if "qqHH" in proc:
                    outputLine += " "+systval+" "
                    activeProc.append(proc)
                    activeVal.append(systval)
                else:
                    outputLine += "- "

            OutputLines.append(outputLine)
            self.SystNames.append(systname)
            self.SystTypes.append("lnN")
            self.SystProcesses.append(activeProc)
            self.SystValues.append(activeVal)

            # Restore outputLine and activeProcvesses before going to next section
            outputLine = ""
            activeProc = []
            activeVal = []

        if self.printResult:
            print " Parsed VBFdipole systematics:"
            for line in OutputLines:
                print " ", line

    def writeOneLine(self,process,string,value=1):
        outputLine = string
        #print " processes:", process
        for proc in self.processes :
            #print " ", proc
            if (proc in process) or ("bkg_"+proc in process) :
                outputLine += str(value)+" "
            else:
                outputLine += "- "
        self.theOutputFile.write(outputLine+"\n")

    def writeOutput(self,doWrite) :
        self.writeOut = doWrite

    def verbose(self, v) :
        self.printResult = v






