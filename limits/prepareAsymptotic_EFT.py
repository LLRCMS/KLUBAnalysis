#! /usr/bin/env python
import sys, pwd, commands, optparse
import os
import re

#define function for parsing options
def parseOptions():
    usage = ('usage: %prog [options] datasetList\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)
    
    parser.add_option('-n', '--name' ,    dest='n',        type='string', default='',     help='beanchmark')
    parser.add_option('-q', '--queue',    dest='queue',                   default='long', help='which queue to use')
    parser.add_option('-t', '--tag',      dest='tag',                     default=None,   help='tag of the cards folder')
    parser.add_option('-c', '--isCondor', dest='isCondor',                                help='run on condor or not',    action='store_true')
    parser.add_option('-b', '--blind',    dest='blind',                   default=False,                                  action='store_true')
    parser.add_option('--frTH',           dest='frTH' ,                   default=False,                                  action='store_true')
 
    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()


if __name__ == "__main__":
    parseOptions()
    global opt, args

    blindName = ""
    if opt.blind:
        blindName = "_blind"

    theoryName = ""
    if opt.frTH:
        theoryName = "_noTH"

    cmsswBase = os.environ['CMSSW_BASE']
    jobsDir = os.getcwd()

    # Main command
    command = "combine -M AsymptoticLimits"

    # Blind limit or not
    if opt.blind:
        command = command + " --run blind "

    # Add workspace to the command
    if opt.isCondor:
        command = command + " %s/workspace.root" % jobsDir
    else:
        jobsDir = jobsDir + '/EFT_cards_stackedAndCleaned/cards_' + opt.n + '/cards_CombAll_' + opt.tag
        command = command + " %s/comb.root" % jobsDir

    # Set POI (it's just "r" for EFT benchmarks)
    command     = command + " --redefineSignalPOIs r "

    # Freeze theory uncretainties group
    theoryName = ""
    if opt.frTH:
        command = command + " --freezeNuisanceGroups theory "
        theoryName = "_noTH"

    # Create submssion script
    scriptFile = open('%s/runJob_Asym_bm%s%s%s.sh'%(jobsDir, opt.n, theoryName, blindName), 'w')
    scriptFile.write('#!/bin/bash\n')

    # Fill submssion script for MIB condor queues
    if opt.isCondor:
        scriptFile.write('source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
        scriptFile.write('cd {}\n'.format(cmsswBase + '/src/'))
        scriptFile.write('eval `scram r -sh`\n')
        scriptFile.write('cd %s\n'%jobsDir)
        scriptFile.write('ulimit -s unlimited \n')
        scriptFile.write('%s -m 125 -n bm%s%s%s &> out_Asym_bm%s%s%s.log \n' % (command, opt.n, theoryName, blindName, opt.n, theoryName, blindName))
        scriptFile.write('echo "All done for job bm%s" \n'%opt.n)
        scriptFile.close()
        os.system('chmod u+rwx %s/runJob_Asym_bm%s%s%s.sh'%(jobsDir, opt.n, theoryName, blindName))

        condorFile = open ('%s/condorLauncher_bm%s.sh'% (jobsDir, opt.n), 'w')
        condorFile.write ('Universe = vanilla\n')
        condorFile.write ('Executable  = %s/runJob_Asym_bm%s%s%s.sh\n'%(jobsDir, opt.n, theoryName, blindName))
        condorFile.write ('Log         = condor_limits_$(ProcId).log\n')
        condorFile.write ('Output      = condor_limits_$(ProcId).out\n')
        condorFile.write ('Error       = condor_limits_$(ProcId).error\n')
        condorFile.write ('queue 1\n')
        condorFile.close ()

        launchcommand = ('condor_submit '+ jobsDir + '/condorLauncher_bm' + str (opt.n) + '.sh')
    
    # Fill submssion script for LLR codor queues
    else:
        scriptFile.write ('export X509_USER_PROXY=~/.t3/proxy.cert\n')
        scriptFile.write ('source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
        scriptFile.write ('cd %s\n' % jobsDir)
        scriptFile.write ('export SCRAM_ARCH=slc6_amd64_gcc491\n')
        scriptFile.write ('eval `scram r -sh`\n')
        scriptFile.write ('source scripts/setup.sh\n')
        scriptFile.write('%s -m 125 -n bm%s%s%s 2>&1 | tee out_Asym_bm%s%s%s.log \n' % (command, opt.n, theoryName, blindName, opt.n, theoryName, blindName))
        scriptFile.close()
        os.system('chmod u+rwx %s/runJob_Asym_bm%s%s%s.sh'%(jobsDir, opt.n, theoryName, blindName))
        
        launchcommand = ('/home/llr/cms/motta/t3submit -'+opt.queue+' \'' + '%s/runJob_Asym_bm%s%s%s.sh'%(jobsDir, opt.n, theoryName, blindName) +"\'")

    print launchcommand
    print command
    
    os.system (launchcommand)
