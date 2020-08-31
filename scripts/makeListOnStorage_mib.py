import os, sys
from subprocess import Popen, PIPE

#tag="Legacy2016_Signals_Dec2019"
#tag="Legacy2016_Backgrounds_Dec2019"
#tag="Legacy2016_Backgrounds2_Dec2019"
#tag="Legacy2016_Backgrounds5_Dec2019"
#tag="Legacy2016_Backgrounds6_Dec2019"
#tag="Legacy2016_Backgrounds7_Dec2019"
#tag="Legacy2016_Data_Muon_Dec2019"
#tag="Legacy2016_Data_Electron_Dec2019"
#tag="Legacy2016_Data_Tau_Dec2019"
#tag="Legacy2016_Data_Tau2_Dec2019"
#tag="Legacy2016_Data_Muon3_Dec2019"
#tag="Legacy2016_Data_Electron3_Dec2019"
#tag="Legacy2016_Data_Tau3_Dec2019"
#tag="Legacy2016_Data_Muon4_Dec2019"
#tag="Legacy2016_Data_Electron4_Dec2019"
#tag="Legacy2016_Data_Tau4_Dec2019"
#tag="Signals_Legacy2018"
#tag="Signals_Legacy2016_v2"
#tag="Signals_Legacy2017"
#tag="Signals_Legacy2016_v3"
tag="Signals_Legacy2017_v4"

#outFolder = '../inputFiles/Files_Legacy_Run2/2016/Signals/'
#outFolder = '../inputFiles/Files_Legacy_Run2/2016/Backgrounds/'
#outFolder = '../inputFiles/Files_Legacy_Run2/2016/Backgrounds2/'
#outFolder = '../inputFiles/Files_Legacy_Run2/2016/Backgrounds5/'
#outFolder = '../inputFiles/Files_Legacy_Run2/2016/Backgrounds6/'
#outFolder = '../inputFiles/Files_Legacy_Run2/2016/Backgrounds7/'
#outFolder = '../inputFiles/Files_Legacy_Run2/2016/Data/Muon/'
#outFolder = '../inputFiles/Files_Legacy_Run2/2016/Data/Electron/'
#outFolder = '../inputFiles/Files_Legacy_Run2/2016/Data/Tau/'
#outFolder = '../inputFiles/Files_Legacy_Run2/2016/Data/Tau2/'
#outFolder = '../inputFiles/Files_Legacy_Run2/2016/Data/Muon3/'
#outFolder = '../inputFiles/Files_Legacy_Run2/2016/Data/Electron3/'
#outFolder = '../inputFiles/Files_Legacy_Run2/2016/Data/Tau3/'
#outFolder = '../inputFiles/Files_Legacy_Run2/2016/Data/Muon4/'
#outFolder = '../inputFiles/Files_Legacy_Run2/2016/Data/Electron4/'
#outFolder = '../inputFiles/Files_Legacy_Run2/2016/Data/Tau4/'
#outFolder = '../inputFiles/Legacy2018_signalsMIB/'
#outFolder = '../inputFiles/Legacy2016_signals/'
outFolder = '../inputFiles/Legacy2017_signals/'

areEnrichedMiniAOD = False; # if true:  add a header and the /store.. etc to run ntuplizer on Tier3 on CMSSW
                                 # if false: only add the polgrid server to run the skim and submit on root
# ====================================================================

def formatName (name, pathToRem):
    name.strip(pathToRem)
    return name

def saveToFile (lista, filename, areEnrichedMiniAOD):
    f = open (filename, "w")

    if not areEnrichedMiniAOD:
        for elem in lista:
            f.write("%s\n" % elem) #vintage
    
    else:
        f.write("FILELIST = cms.untracked.vstring()\n")
        f.write("FILELIST.extend ([\n")
        for elem in lista:
            elem = elem.replace ("/gwteras/cms", "")
            f.write ("    '")
            f.write("%s',\n" % elem) #vintage  
        f.write("])\n")
 
    f.close()


# ====================================================================

useOnly = [] #empty list to do list fot all the folders

print useOnly

#dpmhome = "/dpm/in2p3.fr/home/cms/trivcat"
dpmhome = "/gwteras/cms"

#partialPath = "/store/user/lcadamur/HHNtuples/" #folder contenente la produzione
partialPath = "/store/user/fbrivio/HH_LegacyRun2/"
#partialPath = "/store/user/dzuolo/HHbbtautauNtuples/"


path = dpmhome + partialPath + tag
if outFolder[-1] != "/": outFolder += '/'

command = "/usr/bin/rfdir %s | awk '{print $9}'" % path
#print command
pipe = Popen(command, shell=True, stdout=PIPE)

allLists = {} #dictionary

for line in pipe.stdout:
    
    #print line
    if useOnly:
        if not (line.strip()) in useOnly:
            continue
    samplesPath = (path + "/" + line).strip()    
    sampleName = line
    allLists[sampleName] = []
    print sampleName.strip()
    for level in range(0, 3): #sampleName, tag, hash subfolders
        comm = "/usr/bin/rfdir %s | awk '{print $9}'" % samplesPath.strip()
        #print "comm: ==> " , comm
        pipeNested = Popen (comm, shell=True, stdout=PIPE)
        out = pipeNested.stdout.readlines()
        numLines = len (out)
        if numLines > 0 :
            if numLines > 1 : print "  *** WARNING: In %s too many subfolders, using last one (most recent submission)" % samplesPath        
            samplesPath = samplesPath + "/" + out[-1].strip()
    #print samplesPath
    # now I have to loop over the folders 0000, 1000, etc...
    comm = "/usr/bin/rfdir %s | awk '{print $9}'" % samplesPath.strip()
    pipeNested = Popen (comm, shell=True, stdout=PIPE)
    out = pipeNested.stdout.readlines()
    for subfol in out:
        finalDir = samplesPath + "/" + subfol.strip()
        getFilesComm = ""
        if areEnrichedMiniAOD :
            getFilesComm = "/usr/bin/rfdir %s | grep Enriched_miniAOD | awk '{print $9}'" % finalDir.strip()
        else :
            getFilesComm = "/usr/bin/rfdir %s | grep HTauTauAnalysis | awk '{print $9}'" % finalDir.strip()
            #print getFilesComm
        pipeGetFiles = Popen (getFilesComm, shell=True, stdout=PIPE)
        outGetFiles = pipeGetFiles.stdout.readlines()
        for filename in outGetFiles:
            name = formatName (finalDir + "/" + filename.strip(), dpmhome)
            allLists[sampleName].append (name)

# now I have all file lists, save them
for sample, lista in allLists.iteritems():
   if lista: 
      outName = outFolder + sample.strip()+".txt"
      if areEnrichedMiniAOD : outName = outFolder + sample.strip()+".py"
      saveToFile (lista, outName, areEnrichedMiniAOD)
   else:
      print "  *** WARNING: Folder for dataset %s is empty" % sample
