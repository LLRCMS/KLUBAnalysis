import os
import sys
import argparse
import datetime

samples = [
    "DYmerged",
    "EWKWMinus2Jets_WToLNu",
    "EWKWPlus2Jets_WToLNu",
    "EWKZ2Jets_ZToLL",
    "ST_tW_antitop",
    "ST_tW_top",
    "ST_tchannel_antitop",
    "ST_tchannel_top",
    "TTWJetsToLNu",
    "TTWJetsToQQ",
    "TTWW",
    "TTWZ",
    "TTZToLLNuNu",
    "TTZZ",
    "TT_fullyHad",
    "TT_fullyLep",
    "TT_semiLep",
    "VBFHTauTau",
    "WJets_HT_0_70",
    "WJets_HT_100_200",
    "WJets_HT_1200_2500",
    "WJets_HT_200_400",
    "WJets_HT_2500_Inf",
    "WJets_HT_400_600",
    "WJets_HT_600_800",
    "WJets_HT_70_100",
    "WJets_HT_800_1200",
    "WW",
    "WZ",
    "WminusHTauTau",
    "WplusHTauTau",
    "ZH_HTauTau",
    "ZZ",
    "ggHTauTau",
    "ttHToBB",
    "ttHToNonBB",
    "ttHToTauTau"
]

input_dir = '/data_CMS/cms/portales/HHresonant_SKIMS/SKIMS_UL18_220420/'

here = os.getcwd()

program = 'scripts/split_skims_realfaketau.py'

proto = 'splitter_' ## job .sh fie name
logproto = 'log_' ## job .sh fie name

outDir = "./splitting_logs"

if not os.path.isdir(outDir):
    os.mkdir(outDir)

for sample in samples:
    scriptName = proto + sample + '.sh'
    logName    = logproto + sample + '.txt'
    scriptFile = open (outDir + '/' + scriptName, 'w')
    scriptFile.write ('#!/bin/bash\n')
    scriptFile.write ('export X509_USER_PROXY=~/.t3/proxy.cert\n')
    scriptFile.write ('source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
    scriptFile.write ('cd %s\n' % here)
    scriptFile.write ('eval `scram r -sh`\n')
    scriptFile.write ('source scripts/setup.sh\n')
    
    command = 'python ' + program + ' --sample ' + sample + ' --path ' + input_dir

    scriptFile.write(command)
    scriptFile.close()
    
    os.system ('chmod u+rwx ' + outDir + '/' + scriptName)
    os.system ('chmod u+rwx ' + program)
    launchcommand = ('/opt/exp_soft/cms/t3/t3submit -long \'' + outDir + '/' + scriptName +"\'")
    print launchcommand
    os.system (launchcommand)
