import os, sys
from subprocess import Popen, PIPE



#tag = "Data2018A_Dec2019_resub_egamma"
#tag = "Data2018ABC_Dec2019"
#tag = "Data2018D_Dec2019"
#tag = "Data2018D_Dec2019_resub"
#tag = "Data2018D_Dec2019_resub_egamma"
#tag = "MC2018_Dec2019"
#tag = "MC2018_Dec2019_resub"
#tag = "MC2018_Dec2019_resub2"
#tag = "Signals2018_Dec2019"
tag= "Tau2018D_missingFile_3"
#tag="Signals2018_GF_4"
outFolder = '/home/llr/cms/amendola/HHLegacy/CMSSW_10_2_16/src/KLUBAnalysis/inputFiles/Legacy2018_data_missing/'
#outFolder = '/home/llr/cms/amendola/HHLegacy/CMSSW_10_2_16/src/KLUBAnalysis/inputFiles/Legacy2018_backgrounds/'
#outFolder = '/home/llr/cms/amendola/HHLegacy/CMSSW_10_2_16/src/KLUBAnalysis/inputFiles/Legacy2018_signals/'

areEnrichedMiniAOD = False; # if true:  add a header and the /store.. etc to run ntuplizer on Tier3 on CMSSW
                                 # if false: only add the polgrid server to run the skim and submit on root
# ====================================================================

def formatName (name, pathToRem):
    name.strip(pathToRem)
    name = "root://polgrid4.in2p3.fr/" + name
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
            elem = elem.replace ("root://polgrid4.in2p3.fr//dpm/in2p3.fr/home/cms/trivcat", "")
            f.write ("    '")
            f.write("%s',\n" % elem) #vintage  
        f.write("])\n")
 
    f.close()


# ====================================================================

useOnly = [] #empty list to do list fot all the folders

print useOnly


dpmhome = "/dpm/in2p3.fr/home/cms/trivcat"

partialPath = "/store/user/camendol/HHNtuples2018_Legacy_v2/"


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
