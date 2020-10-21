#!/usr/bin/env python

import os,sys
import optparse
import fileinput
import commands
import time
import glob
import subprocess
from os.path import basename
import ROOT



if __name__ == "__main__":

    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage)
    parser.add_option ('-w', '--workdir', dest='workdir', help='skim location folder', default='none')
    parser.add_option ('-T', '--tag'    , dest='tag'    , help='folder tag name'     , default='')
    parser.add_option ('-c', '--config' , dest='config' , help='skim config file'    , default='none')
    parser.add_option ('-s', '--sleep'  , dest='sleep'  , help='sleep in submission' , default=False)
    parser.add_option ('-d', '--isData' , dest='isData' , help='isData'              , default=False)
    parser.add_option ('-i', '--i'      , dest='nI'     , default=-1  , help='first file to be processed', type=int)
    parser.add_option ('-f', '--f'      , dest='nF'     , default=-1  , help='last file to be processed' , type=int)

    (opt, args) = parser.parse_args()

    currFolder = os.getcwd ()


    # Setup checks
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    # Input/output folder exists
    if not os.path.exists (opt.workdir) :
        print 'Input skim folder', opt.workdir, 'not existing, exiting'
        sys.exit (1)

    # Config is provided
    if opt.config == 'none' :
        print 'Config file missing, exiting'
        sys.exit (1)

    # The sub-range [nI,nF] can be correctly defined
    if opt.nI >= 0 and opt.nF < 0:
        print 'Final file is missing, exiting'
        sys.exit (1)

    if opt.nI < 0 and opt.nF >= 0:
        print 'Initial file is missing, exiting'
        sys.exit (1)

    if opt.nI > opt.nF:
        print 'Initial file is n', opt.nI, 'while final file is n', opt.nF, ': can not create proper range, exiting'
        sys.exit (1)

    # Input/output folder already contains systematics files
    if opt.nI < 0:
        if os.path.isfile(opt.workdir + "/syst_output_0.root"):
            print 'A file named', opt.workdir+'/syst_output_0.root', 'is already present in the directory, exiting'
            sys.exit(1)
    else:
        if os.path.isfile(opt.workdir + "/syst_output_"+str(opt.nI)+".root"):
            print "A file named", opt.workdir+"/syst_output_"+str(opt.nI)+".root", "is already present in the directory, exiting"
            sys.exit(1)


    # Input/Output setup
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    # List all files in the working dir
    allthings = os.listdir(opt.workdir)
    onlyfiles = [f for f in os.listdir(opt.workdir) if os.path.isfile(os.path.join(opt.workdir, f))]

    # Get all output_*.root files from skims to be used as input for syst
    rootfiles = [f for f in onlyfiles if f[0:6]=='output' and f[-4:]=='root']

    # Create ordered list of output_*.root files
    inputfiles = []
    for x in range(0,len(rootfiles)):
        inputfiles.append('output_'+str(x)+'.root')

    # If running on a user-define range select only the requested input files
    if opt.nI >= 0:
        userfiles = []
        for jobIdx in range(opt.nI,opt.nF+1):
            userfiles.append('output_'+str(jobIdx)+'.root')

        inputfiles = [f for f in inputfiles if f in userfiles]

    # Append full path
    inputfiles = [os.path.join(opt.workdir, f) for f in inputfiles]


    # Submit the jobs
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    skimmer = 'skimOutputter_HHbtag.exe'

    tagname = "/" + opt.tag if opt.tag else ''
    jobsDir = currFolder + tagname + '/SYST_' + basename(opt.workdir)
    if os.path.exists (jobsDir) : os.system ('rm -f ' + jobsDir + '/*')
    else                        : os.system ('mkdir -p ' + jobsDir)

    n = int (0)
    if opt.nI >=0:
        n = int (opt.nI)
    commandFile = open (jobsDir + '/submit.sh', 'w')
    for inputfile in inputfiles : 

        scriptFile = open ('%s/systJob_%d.sh'% (jobsDir,n), 'w')
        scriptFile.write ('#!/bin/bash\n')
        scriptFile.write ('echo $HOSTNAME\n')
        scriptFile.write ('source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
        scriptFile.write ('eval `scram r -sh`\n')
        scriptFile.write ('cd %s\n'%currFolder)
        scriptFile.write ('eval `scram r -sh`\n')
        scriptFile.write ('source scripts/setup.sh\n')
        command  = skimmer + ' ' + inputfile
        command += (" " + opt.workdir + "/syst_output_"+str(n)+".root")
        command += (" " + opt.config)
        if opt.isData : command += " 1 "
        else          : command += " 0 "
        command += (" " + ">& " + opt.workdir + "/syst_output_"+str(n)+".log\n")
        scriptFile.write(command)
        scriptFile.write ('touch ' + jobsDir + '/done_%d\n'%n)
        scriptFile.write ('echo "All done for job %d" \n'%n)
        scriptFile.close ()
        os.system ('chmod u+rwx %s/systJob_%d.sh'% (jobsDir,n))

        condorFile = open ('%s/condorLauncher_%d.sh'% (jobsDir,n), 'w')
        condorFile.write ('Universe = vanilla\n')
        condorFile.write ('Executable  = '+jobsDir + '/systJob_' + str (n) + '.sh\n')
        condorFile.write ('Log         = condor_job_$(ProcId).log\n')
        condorFile.write ('Output      = condor_job_$(ProcId).out\n')
        condorFile.write ('Error       = condor_job_$(ProcId).error\n')
        condorFile.write ('queue 1\n')
        condorFile.close ()

        command = 'condor_submit '+ jobsDir + '/condorLauncher_' + str (n) + '.sh'
        if opt.sleep : time.sleep (0.1)
        os.system (command)
        commandFile.write (command + '\n')
        n = n + 1
    commandFile.close ()


