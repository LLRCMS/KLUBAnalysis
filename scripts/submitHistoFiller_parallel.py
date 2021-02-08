#!/usr/bin/env python

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

outDir = datetime.datetime.now().strftime('%Y.%m.%d_%H.%M.%S')
if args.tag:
    outDir = args.tag

here = os.getcwd()

program = 'testAnalysisHelper.exe'

proto = 'filler_' ## job .sh fie name
logproto = 'log_' ## job .sh fie name

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
    condorFile.write ('queue 1\n')
    condorFile.close ()

    launchcommand = ('condor_submit '+ outDir + '/condorLauncher_' + str (nj) + '.sh')

    print launchcommand
    os.system (launchcommand)
