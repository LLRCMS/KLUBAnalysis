import ROOT
import os
import sys

#inputDir = '/gwteraz/users/brivio/SKIMMED_Legacy2018_1Sep2020/SKIM_DY_updown'
inputDir = '/gwteraz/users/brivio/SKIMMED_Legacy2016_1September2020/SKIM_DY'

year = 2016

############################################################################################################
## NOTE: this script takes about 11 mins for a full DY sample (inclusive + NJet + NBJet)
##

print "... processing directory", inputDir

if inputDir[-1] == '/':
    inputDir = inputDir[:-1] 

inputList = inputDir + '/' + 'goodfiles.txt'
if not os.path.isfile(inputList):
    print "Could not find file:", inputList
    sys.exit()

suffix = [
    ["_0b_0JPt"  , "lheNOutB==0 && genZ_pt <= 10."],
    ["_0b_10JPt" , "lheNOutB==0 && genZ_pt >  10.  && genZ_pt <= 50."],
    ["_0b_50JPt" , "lheNOutB==0 && genZ_pt >  50.  && genZ_pt <= 80."],
    ["_0b_80JPt" , "lheNOutB==0 && genZ_pt >  80.  && genZ_pt <= 110."],
    ["_0b_110JPt", "lheNOutB==0 && genZ_pt >  110. && genZ_pt <= 190."],
    ["_0b_190JPt", "lheNOutB==0 && genZ_pt >  190."],
    ["_1b_0JPt"  , "lheNOutB==1 && genZ_pt <= 10."],
    ["_1b_10JPt" , "lheNOutB==1 && genZ_pt >  10.  && genZ_pt <= 50."],
    ["_1b_50JPt" , "lheNOutB==1 && genZ_pt >  50.  && genZ_pt <= 80."],
    ["_1b_80JPt" , "lheNOutB==1 && genZ_pt >  80.  && genZ_pt <= 110."],
    ["_1b_110JPt", "lheNOutB==1 && genZ_pt >  110. && genZ_pt <= 190."],
    ["_1b_190JPt", "lheNOutB==1 && genZ_pt >  190."],
    ["_2b_0JPt"  , "lheNOutB>=2 && genZ_pt <= 10."],
    ["_2b_10JPt" , "lheNOutB>=2 && genZ_pt >  10.  && genZ_pt <= 50."],
    ["_2b_50JPt" , "lheNOutB>=2 && genZ_pt >  50.  && genZ_pt <= 80."],
    ["_2b_80JPt" , "lheNOutB>=2 && genZ_pt >  80.  && genZ_pt <= 110."],
    ["_2b_110JPt", "lheNOutB>=2 && genZ_pt >  110. && genZ_pt <= 190."],
    ["_2b_190JPt", "lheNOutB>=2 && genZ_pt >  190."],
    # This last one "_toBeRemoved" is DUMMY, it ends up being empty, but must be kept anyway
    # otherwise ROOT won't close properly the "_2b_190JPt" sample \_(``)_/
    ["_toBeRemoved", "lheNOutB==100"]
]
if year == 2017 or year == 2018:
    suffix = [
        ["_0b_0JPt"  , "lheNOutB==0 && genZ_pt <= 10."],
        ["_0b_10JPt" , "lheNOutB==0 && genZ_pt >  10.  && genZ_pt <= 30."],
        ["_0b_30JPt" , "lheNOutB==0 && genZ_pt >  30.  && genZ_pt <= 50."],
        ["_0b_50JPt" , "lheNOutB==0 && genZ_pt >  50.  && genZ_pt <= 100."],
        ["_0b_100JPt", "lheNOutB==0 && genZ_pt >  100. && genZ_pt <= 200."],
        ["_0b_200JPt", "lheNOutB==0 && genZ_pt >  200."],
        ["_1b_0JPt"  , "lheNOutB==1 && genZ_pt <= 10."],
        ["_1b_10JPt" , "lheNOutB==1 && genZ_pt >  10.  && genZ_pt <= 30."],
        ["_1b_30JPt" , "lheNOutB==1 && genZ_pt >  30.  && genZ_pt <= 50."],
        ["_1b_50JPt" , "lheNOutB==1 && genZ_pt >  50.  && genZ_pt <= 100."],
        ["_1b_100JPt", "lheNOutB==1 && genZ_pt >  100. && genZ_pt <= 200."],
        ["_1b_200JPt", "lheNOutB==1 && genZ_pt >  200."],
        ["_2b_0JPt"  , "lheNOutB>=2 && genZ_pt <= 10."],
        ["_2b_10JPt" , "lheNOutB>=2 && genZ_pt >  10.  && genZ_pt <= 30."],
        ["_2b_30JPt" , "lheNOutB>=2 && genZ_pt >  30.  && genZ_pt <= 50."],
        ["_2b_50JPt" , "lheNOutB>=2 && genZ_pt >  50.  && genZ_pt <= 100."],
        ["_2b_100JPt", "lheNOutB>=2 && genZ_pt >  100. && genZ_pt <= 200."],
        ["_2b_200JPt", "lheNOutB>=2 && genZ_pt >  200."],
        # This last one "_toBeRemoved" is DUMMY, it ends up being empty, but must be kept anyway
        # otherwise ROOT won't close properly the "_2b_200JPt" sample \_(``)_/
        ["_toBeRemoved", "lheNOutB==100"]
    ]

outDirs = [inputDir+x[0] for x in suffix]

print "... saving outputs in: "
for i in outDirs:
    print i

for i in outDirs:
    os.system('mkdir '+i)

inputListFile = open(inputList)

outGoodFiles = []
for d in outDirs:
    outGoodFiles.append(open(d+'/goodfiles.txt', 'w'))

for idx, line in enumerate(inputListFile):
    line = line.strip()
    fileName = line.replace(inputDir+'/', '')
    print 'file num' , idx

    # if reading a goodfile, copy it to destination directory
    for ff in range(0,len(suffix)-1):
        outGoodFiles[ff].write(line.replace(inputDir, outDirs[ff])+'\n')

    fIn = ROOT.TFile(line)
    tIn = fIn.Get('HTauTauTree')
    h_eff        = fIn.Get('h_eff')
    h_effSummary = fIn.Get('h_effSummary')
    h_syst       = fIn.Get('h_syst')

    for i,el in enumerate(suffix):
        fOut = ROOT.TFile(outDirs[i]+'/'+fileName, 'recreate')
        fOut.cd()
        tOut = tIn.CopyTree(el[1])
        tOut.Write()
        h_eff.Write()
        h_effSummary.Write()
        h_syst.Write()
        fOut.Close()

    fIn.Close()

for ff in outGoodFiles:
    ff.close()
print '... finished'
