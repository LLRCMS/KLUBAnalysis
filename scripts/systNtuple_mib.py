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

    # Input/output folder already contains systematics files
    if os.path.isfile(opt.workdir + "/syst_output_0.log"):
        print 'A file named', opt.workdir+'/syst_output_0.log', 'is already present in the directory, exiting'
        sys.exit(1)


    # Input/Output setup
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    # List all files in the working dir
    allthings = os.listdir(opt.workdir)
    onlyfiles = [f for f in os.listdir(opt.workdir) if os.path.isfile(os.path.join(opt.workdir, f))]

    # Get output_*.root from skims to be used as input for syst
    inputfiles = [f for f in onlyfiles if f[0:6]=='output' and f[-4:]=='root']

    # Append full path
    inputfiles = [os.path.join(opt.workdir, f) for f in inputfiles]


    # Submit the jobs
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    skimmer = 'skimOutputter.exe'

    tagname = "/" + opt.tag if opt.tag else ''
    jobsDir = currFolder + tagname + '/SYST_' + basename(opt.workdir)
    if os.path.exists (jobsDir) : os.system ('rm -f ' + jobsDir + '/*')
    else                        : os.system ('mkdir -p ' + jobsDir)

    n = int (0)
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


