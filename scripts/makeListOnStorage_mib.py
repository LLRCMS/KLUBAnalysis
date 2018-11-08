import os, sys
from subprocess import Popen, PIPE

#tag = "MC_VBF_Summer16_singleTop"
#tag = "MC_Fall17"
#tag = "MC_PU12Apr_18May2018"
#tag = "Data2017BF_31Mar2018ReReco_17May2018"
#tag = "MC_26June2018"
#tag = "MC_28June2018_ZZ4L"
#tag = "MC_3July2018"
#tag = "MC_4July2018_allFail"
#tag = "MC_28June2018_DY4J"
#tag = "MC_7July2018_GGH450"
#tag = "MC_7July2018_GGH900"
#tag = "MC_7July2018_VBF800"
#tag = "MC_7July2018_VBF850"
#tag = "MC_7July2018_VBF1250"
#tag = "MC_7July2018_DYBB"
#tag = "MC_7July2018_EWKminus"
#tag = "MC_7July2018_EWK2Jets"
#tag = "MC_7July2018_Wjets800to1200"
#tag = "MC_7July2018_ttHnonbb"
#tag = "MC_7July2018_TTWW"
#tag = "MC_7July2018_WW"
#tag = "MC_7July2018_WZTo3LNu_3Jets"
#tag = "MC_24July2018"
#tag = "MC_14JOct2018"
tag = "MC_23JOct2018_signalsTESI"

#outFolder = '/home/llr/cms/amendola/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/Files_17Nov_VBF'
#outFolder = '../inputFiles/Fall17_MC/'
#outFolder = '../inputFiles/Fall17_DATA/'
#outFolder = '../inputFiles/Files_June2018/'
#outFolder = '../inputFiles/Files_June2018/signals/'
outFolder = '../inputFiles/Files_June2018/signals_tesi/'

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
partialPath = "/store/user/fbrivio/Hhh_1718/"


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
