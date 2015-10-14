import os, sys
from subprocess import Popen, PIPE

#tag = "llrNt_NoSVFit_bkg_27Ago2015" # tag, stesso nome del crab folder senza crab3_ all'inizio
#tag = "llrNt_NoSVFit_data_27Ago2015"
#tag = "llrNt_NoSVFit_data_30Ago2015_lumiMaskFix"
tag  = "Data50ns_SVFit_6Ott2015"
#outFolder = "../inputFiles/28Ago2015/" # write / at the end
outFolder = "../inputFiles/13Ott2015/"

# ====================================================================

def formatName (name, pathToRem):
    name.strip(pathToRem)
    name = "root://polgrid4.in2p3.fr/" + name
    return name

def saveToFile (lista, filename):
    f = open (filename, "w")
    for elem in lista:
        f.write("%s\n" % elem) #vintage
    f.close()


# ====================================================================

dpmhome = "/dpm/in2p3.fr/home/cms/trivcat"
partialPath = "/store/user/lcadamur/HHNtuples/" #folder contenente la produzione
path = dpmhome + partialPath + tag

command = "/usr/bin/rfdir %s | awk '{print $9}'" % path
pipe = Popen(command, shell=True, stdout=PIPE)

allLists = {} #dictionary

for line in pipe.stdout:
    
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
        getFilesComm = "/usr/bin/rfdir %s | grep .root | awk '{print $9}'" % finalDir.strip()
        pipeGetFiles = Popen (getFilesComm, shell=True, stdout=PIPE)
        outGetFiles = pipeGetFiles.stdout.readlines()
        for filename in outGetFiles:
            name = formatName (finalDir + "/" + filename.strip(), dpmhome)
            allLists[sampleName].append (name)

# now I have all file lists, save them
for sample, lista in allLists.iteritems():
   if lista: 
      saveToFile (lista, outFolder + sample.strip()+".txt")
   else:
      print "  *** WARNING: Folder for dataset %s is empty" % sample
