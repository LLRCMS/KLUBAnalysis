#!/usr/bin/env python

###### LAUNCH COMMAND EXAMPLE:

# - TauTau -
# python scripts/submitHistoFiller_UL2016APV.py --cfg config/mainCfg_TauTau_UL2016APV.cfg --tag analysis_TauTau_UL2016APV_5Apr2022 --n 100
# python scripts/submitHistoFiller_UL2016APV.py --cfg config/mainCfg_TauTau_UL2016APV.cfg --tag analysis_TauTau_UL2016APV_8Apr2022_correctPostVFP --n 100

# - MuTau -
# python scripts/submitHistoFiller_UL2016APV.py --cfg config/mainCfg_MuTau_UL2016APV.cfg --tag analysis_MuTau_UL2016APV_5Apr2022 --n 100
# python scripts/submitHistoFiller_UL2016APV.py --cfg config/mainCfg_MuTau_UL2016APV.cfg --tag analysis_MuTau_UL2016APV_8Apr2022_correctPostVFP --n 100

# - ETau -
# python scripts/submitHistoFiller_UL2016APV.py --cfg config/mainCfg_ETau_UL2016APV.cfg --tag analysis_ETau_UL2016APV_5Apr2022 --n 100
# python scripts/submitHistoFiller_UL2016APV.py --cfg config/mainCfg_ETau_UL2016APV.cfg --tag analysis_ETau_UL2016APV_8Apr2022_correctPostVFP --n 100

# - MuMu -
# python scripts/submitHistoFiller_UL2016APV.py --cfg config/mainCfg_MuMu_UL2016APV.cfg --tag analysis_MuMu_2016APV_6June2020 --n 150

# - TauTau tauIDSF -
# python scripts/submitHistoFiller_UL2016APV.py --cfg config/mainCfg_TauTau_UL2016APV_tauIDSF.cfg --tag analysis_TauTau_tauIDSF_12May2020_Legacy2016APV --n 90

################

import os
import sys
import argparse
import datetime

parser = argparse.ArgumentParser(description='Command line parser of plotting options')
parser.add_argument('--cfg',    dest='cfg',   help='name of the main cfg to run', default=None)
parser.add_argument('--tag',    dest='tag',   help='name of working space (defaults to timestamp)', default=None)
parser.add_argument('--njobs',  dest='njobs', type=int, help='number of jobs for parallelization',  default=17)
args = parser.parse_args()


if not args.cfg:
    print "** Please provide a cfg name: --cfg <relative path from KLUBanalysis/ or absolute path>"
    sys.exit()

# datetime.datetime.now()
outDir = datetime.datetime.now().strftime('%Y.%m.%d_%H.%M.%S')
if args.tag:
    outDir = args.tag
#outDir = "JEC_jobs_pt25/"+outDir

# pathname = os.path.dirname(sys.argv[0])        
# here     = os.path.abspath(pathname)
here = os.getcwd()

program = 'testAnalysisHelper.exe'

proto = 'filler_' ## job .sh fie name
logproto = 'log_' ## job .sh fie name

os.system('mkdir ' + outDir)
for nj in range(0, args.njobs):
    scriptName = proto + str(nj) + '.sh'
    logName    = logproto + str(nj) + '.txt'
    scriptFile = open (outDir + '/' + scriptName, 'w')
    scriptFile.write ('#!/bin/bash\n')
    scriptFile.write ('source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
    scriptFile.write ('eval `scram r -sh`\n')
    scriptFile.write ('cd %s\n' % here)
    scriptFile.write ('eval `scram r -sh`\n')
    scriptFile.write ('source scripts/setup.sh\n')
    command = program + ' ' + args.cfg + ' ' + str(nj) + ' ' + str(args.njobs) + ' 2>&1 | tee ' + outDir + '/' + logName
    scriptFile.write(command)
    scriptFile.close()
    os.system ('chmod u+rwx ' + outDir + '/' + scriptName)

    condorFile = open ('%s/condorLauncher_%d.sh'% (outDir,nj), 'w')
    condorFile.write ('Universe = vanilla\n')
    condorFile.write ('Executable  = '+outDir + '/' + scriptName +'\n')
    condorFile.write ('Log         = condor_filler_$(ProcId).log\n')
    condorFile.write ('Output      = condor_filler_$(ProcId).out\n')
    condorFile.write ('Error       = condor_filler_$(ProcId).error\n')
    condorFile.write ('Requirements = ((machine != "hercules.hcms.it")&&(machine != "pcmaster01.hcms.it")&&(machine != "catalina.hcms.it"))\n')
    condorFile.write ('queue 1\n')
    condorFile.close ()

    #launchcommand = ('/usr/bin/qsub -q  longcms ' + outDir + '/' + scriptName)
    #launchcommand = ('/usr/bin/qsub -q  longcms ' + outDir + '/' + scriptName)
    launchcommand = ('condor_submit '+ outDir + '/condorLauncher_' + str (nj) + '.sh')

    print launchcommand
    os.system (launchcommand)
