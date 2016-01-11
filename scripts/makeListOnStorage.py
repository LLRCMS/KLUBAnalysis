import os, sys
from subprocess import Popen, PIPE

#tag = "llrNt_NoSVFit_bkg_27Ago2015" # tag, stesso nome del crab folder senza crab3_ all'inizio
#tag = "llrNt_NoSVFit_data_27Ago2015"
#tag = "llrNt_NoSVFit_data_30Ago2015_lumiMaskFix"
#tag  = "Data25ns_SVFit_6Ott2015_resub"
#tag = "Data25ns_noSVFit_npvFix_30Ott2015_newJson_trigFix2"
#tag = "MC_NoSVFit_MiniAODV2_13Nov2015"
#tag = "Data25ns_noSVFit_lumiDiff13Nov2015"
#tag = "Data_SilverJson_SVfit"
#tag = "MC_SilverJson_SVfit"
#tag = "Data_SVFit_MiniAODV2_22Nov2015_SVFix"
#tag = "MC_SVFit_MiniAODV2_22Nov2015_EssentialSamples_SVFix"

#tag = "Data_SilverJson_SVfit_31Dic2015"
tag = "MC_SilverJson_SVfit_31Dic2015"

#outFolder = "../inputFiles/28Ago2015/" # write / at the end
#outFolder = "../inputFiles/13Ott2015/25ns/"
# make list of EminiAOD from storage as publication has problems
#outFolder = "/home/llr/cms/cadamuro/HiggsTauTauFramework/CMSSW_7_4_7/src/LLRHiggsTauTau/NtupleProducer/test/inputFiles/13Ott2015/25ns/"
#outFolder = "../inputFiles/DataRunDTrigFix4Nov/"
#outFolder = "../inputFiles/miniAODv2/"
#outFolder = "../inputFiles/Data14NovLumiDiff/"
#outFolder  = "../inputFiles/miniAODv2VSfitForTalk/"
#outFolder = "/home/llr/cms/cadamuro/HHKlubAnalysis/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/miniAODv2_silverJson/"
#outFolder = "/home/llr/cms/cadamuro/HHKlubAnalysis/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/miniAODv2_silverJSON_TEST/"
outFolder = "/home/llr/cms/cadamuro/HHKlubAnalysis/CMSSW_7_4_7/src/KLUBAnalysis/inputFiles/miniAODv2_silverJSON_5Gen2016/"

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
partialPath = "/store/user/lcadamur/HHNtuples/" #folder contenente la produzione
#partialPath = "/store/user/davignon/EnrichedMiniAOD/"

path = dpmhome + partialPath + tag

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
