import ROOT
import os
import sys
#import numpy as n
from array import array
from glob import glob


# Find max and min for a specific variable (in all files of all samples)
def getMaxMin(inputDirs, var):

    max = -10.
    min = +10.
    
    for inputDir in inputDirs:
        
        inputList = inputDir + '/' + 'goodfiles.txt'
        
        if not os.path.isfile(inputList):
            print "Could not find file:", inputList
            sys.exit()
        inputListFile = open(inputList)

        for idx, line in enumerate(inputListFile):
            line = line.strip()
            fileName = line.replace(inputDir+'/', '')
            fIn = ROOT.TFile(line,'update')
            tIn = fIn.Get('HTauTauTree')
            
            if (tIn.GetMaximum(var) > max) and (tIn.GetMaximum(var)<=+1.): max = tIn.GetMaximum(var)
            if (tIn.GetMinimum(var) < min) and (tIn.GetMinimum(var)>=-1.): min = tIn.GetMinimum(var)

    return (var, max, min)




##############
###  MAIN  ###
##############

#inputDirs = ['/gwteraz/users/brivio/SKIMMED_6Nov2018/SKIM_ggHTauTau_TEST']
skimLocation = '/gwteraz/users/brivio/SKIMMED_6Nov2018/'
skimLocation = '/gwteraz/users/brivio/SKIMMED_TEST/'
print 'Skim directory set to:', skimLocation
inputDirs = glob(skimLocation+'*/')


# Get list of BDT variables from the 1st file in the first directory
print 'Now fetching list of all variables containing (BDToutSM, BDToutLM, BDToutMM, BDToutHM)...'
varList = []
varFile = ROOT.TFile(inputDirs[0]+'output_0.root')
varTree = varFile.Get('HTauTauTree')
varBranches = varTree.GetListOfBranches()
for var in varBranches:
    name = var.GetName()
    if ('BDToutSM' in name) or ('BDToutLM' in name) or ('BDToutMM' in name) or ('BDToutHM' in name) :
    #if ('BDToutSM_kl_1' in name) :
        varList.append(name)


# For each variable find max e min between all samples
print 'Now finding max and min for each variable (Variable, Max, Min) and modifying BDT values:'
tups = []
for var in varList:

    tup = getMaxMin(inputDirs, var)
    tups.append(tup)
    print ' --- :', tup[0], tup[1], tup[2]


# Now transforming the BDT output scores from [minBDT,maxBDT] to [0.,+1.] using the GLOBAL max and min for each variable (between all samples)
print 'Now transforming BDT outputs...'
for k,inputDir in enumerate(inputDirs):
    print 'Transforming directory:', inputDir

    if inputDir[-1] == '/':
        inputDir = inputDir[:-1] 

    inputList = inputDir + '/' + 'goodfiles.txt'

    if not os.path.isfile(inputList):
        print "Could not find file:", inputList
        sys.exit()

    outDir = inputDir+'_modif'
    os.system('mkdir '+outDir)
    os.system('cp '+inputDir+'/goodfiles.txt '+outDir)

    inputListFile = open(inputList)

    for idx, line in enumerate(inputListFile):

        line = line.strip()
        fileName = line.replace(inputDir+'/', '')

        fIn = ROOT.TFile(line)
        h_eff = fIn.Get('h_eff')
        h_effSummary = fIn.Get('h_effSummary')
        tIn = fIn.Get('HTauTauTree')
        
        fOut = ROOT.TFile((outDir+'/output_%i.root')%idx,"recreate");
        h_eff.Write()
        h_effSummary.Write()
        tOut = tIn.CloneTree(0);

        # create the new branches
        newBs = []
        for j,tup in enumerate(tups):
            newBs.append(array("f",[0]))
            tOut.Branch('m_'+tup[0], newBs[j], 'm_'+tup[0]+'/F')

        # loop on entries to fill everything
        for i,evt in enumerate(tIn):
        
            for n,newB in enumerate(newBs):
                brName = tups[n][0]
                max    = tups[n][1]
                min    = tups[n][2]
                score  = evt.__getattr__(brName)
                newB[0] = (1./(max-min))*(score - min)

            tOut.Fill()

        tOut.AutoSave()
        tOut.Write()
        ROOT.gDirectory.Purge()
        fOut.Close()

        fIn.Close()

print '...all done!'
