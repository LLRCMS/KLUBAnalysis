import ROOT
import os
import sys

#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_5Gen/SKIM_DY'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_9Gen/SKIM_DY'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_10Gen/SKIM_DY/'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_27Gen/SKIM_DY/'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_1Feb/SKIM_DY/'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_9Feb/SKIM_DY/'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_11Feb/SKIM_DY/'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_26Feb/SKIM_DY/'
# inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_28Feb/SKIM_DY/'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_13Giu_All/SKIM_DY/'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_11Lug_textdxyz/SKIM_DY/'

#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_0j0b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_1j0b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_1j1b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_2j0b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_2j1b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_2j2b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_3j0b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_3j1b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_3j2b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_3j3b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_4j0b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_4j1b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_4j2b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_4j3b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_19Nov2018_Run2017_0jets/SKIM_DY_NLO'
inputDir = '/gwteraz/users/brivio/SKIMMED_7Feb2019/SKIM_DY_NLO'

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

suffix = ['_allgenjets_0b', '_allgenjets_1b', '_allgenjets_2b']

outDirs = [inputDir+x for x in suffix]
# outDirProto = '/'.join(inputDir.split('/')[:-1])

print "... saving outputs in: "
for i in outDirs:
    print i

for i in outDirs:
    os.system('mkdir '+i)

inputListFile = open(inputList)

outGoodFiles = []
for d in outDirs:
    # print d+'/goodfiles.txt'
    outGoodFiles.append(open(d+'/goodfiles.txt', 'w'))

for idx, line in enumerate(inputListFile):
    line = line.strip()
    fileName = line.replace(inputDir+'/', '')
    # if idx % 50 == 0: print 'file num' , idx
    print 'file num' , idx

    # if reading a goodfile, copy it to destination directory
    for ff in range(0,3):
        outGoodFiles[ff].write(line.replace(inputDir, outDirs[ff])+'\n')

    fIn = ROOT.TFile(line)
    tIn = fIn.Get('HTauTauTree')
    h_eff = fIn.Get('h_eff')

    ## 0 jet
    fOut = ROOT.TFile(outDirs[0]+'/'+fileName, 'recreate')
    fOut.cd()
    tOut = tIn.CopyTree('nBhadrons==0')
    tOut.Write()
    h_eff.Write()
    fOut.Close()

    ## 1 jet
    fOut = ROOT.TFile(outDirs[1]+'/'+fileName, 'recreate')
    fOut.cd()
    tOut = tIn.CopyTree('nBhadrons==1')
    tOut.Write()
    h_eff.Write()
    fOut.Close()

    ## >=2 jet
    fOut = ROOT.TFile(outDirs[2]+'/'+fileName, 'recreate')
    fOut.cd()
    tOut = tIn.CopyTree('nBhadrons>=2')
    tOut.Write()
    h_eff.Write()
    fOut.Close()

    fIn.Close()

for ff in outGoodFiles:
    ff.close()
print '... finished'
