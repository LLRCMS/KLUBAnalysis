#!/usr/bin/env python

import os,sys
import optparse
import fileinput
import commands
import time
from os.path import basename

if __name__ == "__main__":

    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage)
    parser.add_option ('-w', '--workdir', dest='workdir', help='skim location folder', default='none')
    parser.add_option ('-T', '--tag'    , dest='tag'    , help='folder tag name'     , default='')
    parser.add_option ('-c', '--config' , dest='config' , help='skim config file'    , default='none')
    parser.add_option ('-s', '--sleep'  , dest='sleep'  , help='sleep in submission' , default=False)
    parser.add_option ('-q', '--queue'  , dest='queue'  , help='batch queue'         , default='short')
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
    
    # Input/Output setup
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    # List all files in the working dir
    allthings = os.listdir(opt.workdir)
    onlyfiles = [f for f in os.listdir(opt.workdir) if os.path.isfile(os.path.join(opt.workdir, f))]

    # Get all output_*.root files from skims to be used as input for syst
    rootfiles = [f for f in onlyfiles if f[0:6]=='output' and f[-4:]=='root']

    # Create ordered list of output_*.root files
    inputfiles = []
    inputnumbers = [int(f[7:-5]) for f in rootfiles]
   
    # Input/output folder already contains systematics files
    for inputnumber in inputnumbers:
	if opt.nI >= 0 and (inputnumber < opt.nI or inputnumber > opt.nF):
	    continue
	filename = opt.workdir + "/syst_output_" + str(inputnumber) + ".root"
	if os.path.isfile(filename):
 	    print 'A file named ' + filename + ' is already present in the directory, exiting'
	    sys.exit(1) 

    # If running on a user-define range select only the requested input files
    for x in range(0, max(inputnumbers) + 1):
	if opt.nI >= 0:
	    if x < opt.nI or x > opt.nF:
	        continue
	filename = 'output_'+str(x)+'.root'
	if filename in rootfiles:
	    inputfiles.append(filename)

    # Append full path
    inputfiles = [os.path.join(opt.workdir, f) for f in inputfiles]


    # Submit the jobs
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    skimmer = 'skimOutputter_HHbtag.exe'

    tagname = "/" + opt.tag if opt.tag else ''
    jobsDir = currFolder + tagname + '/SYST_' + basename(opt.workdir)
    #if os.path.exists (jobsDir) : os.system ('rm -f ' + jobsDir + '/*')
    #else                        : os.system ('mkdir -p ' + jobsDir)
    os.system ('mkdir -p ' + jobsDir)

    commandFile = open (jobsDir + '/submit.sh', 'w')
    for inputfile in inputfiles : 
        n = int(inputfile.split('/')[-1][7:-5])
        scriptFile = open ('%s/systJob_%d.sh'% (jobsDir,n), 'w')
        scriptFile.write ('#!/bin/bash\n')
        scriptFile.write ('export X509_USER_PROXY=~/.t3/proxy.cert\n')
        scriptFile.write ('source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
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

        command = '/home/llr/cms/motta/t3submit -' + opt.queue + ' ' + jobsDir + '/systJob_' + str (n) + '.sh'
        if opt.sleep : time.sleep (0.1)
        os.system (command)
        commandFile.write (command + '\n')
    commandFile.close ()


