#!/usr/bin/env python

import os
import sys
import argparse
import datetime

parser = argparse.ArgumentParser(description='Command line parser of plotting options')
parser.add_argument('--cfg'     , dest='cfg'     ,            default=None,   help='name of the main cfg to run')
parser.add_argument('--tag'     , dest='tag'     ,            default=None,   help='name of working space (defaults to timestamp)')
parser.add_argument('--queue',    dest='queue'   ,            default='long', help='which queue to use')
parser.add_argument('--njobs'   , dest='njobs'   , type=int , default=10  ,   help='number of jobs for parallelization')
parser.add_argument('--isCondor', dest='isCondor', action='store_true',       help='run on condor or not')
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

    if args.isCondor:
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

    else:
        scriptFile.write ('export X509_USER_PROXY=~/.t3/proxy.cert\n')
        scriptFile.write ('source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
        scriptFile.write ('cd %s\n' % here)
        scriptFile.write ('export SCRAM_ARCH=slc6_amd64_gcc491\n')
        scriptFile.write ('eval `scram r -sh`\n')
        scriptFile.write ('source scripts/setup.sh\n')
        command = program + ' ' + args.cfg + ' ' + str(nj) + ' ' + str(args.njobs) + ' 2>&1 | tee ' + outDir + '/' + logName
        scriptFile.write(command)
        scriptFile.close()
        os.system ('chmod u+rwx ' + outDir + '/' + scriptName)
        launchcommand = ('/home/llr/cms/motta/t3submit -'+args.queue+' \'' + outDir + '/' + scriptName +"\'")

    print launchcommand
    # os.system (launchcommand)
