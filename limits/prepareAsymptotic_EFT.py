#! /usr/bin/env python
import sys, pwd, commands, optparse
import os
import re

#define function for parsing options
def parseOptions():
    usage = ('usage: %prog [options] datasetList\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)
    
    parser.add_option('-n'    , '--name' , dest='n'    , type='string', default=''   , help='beanchmark')
    parser.add_option('-b'    , '--blind', dest='blind',                default=False, action='store_true')
    parser.add_option('--frTH',            dest='frTH' ,                default=False, action='store_true')
 
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
    scriptFile = open('%s/runJob_Asym_bm%s%s%s.sh'%(jobsDir, opt.n, theoryName, blindName), 'w')
    scriptFile.write('#!/bin/bash\n')
    scriptFile.write('source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
    scriptFile.write('cd {}\n'.format(cmsswBase + '/src/'))
    scriptFile.write('eval `scram r -sh`\n')
    scriptFile.write('cd %s\n'%jobsDir)
    scriptFile.write('ulimit -s unlimited \n')

    # Main command
    command = "combine -M AsymptoticLimits"

    # Blind limit or not
    if opt.blind:
        command = command + " --run blind "

    # Add workspace to the command
    currFolder = os.getcwd ()
    command = command + " %s/workspace.root" % jobsDir

    # Set POI (it's just "r" for EFT benchmarks)
    command     = command + " --redefineSignalPOIs r "

    # Freeze theory uncretainties group
    theoryName = ""
    if opt.frTH:
        command = command + " --freezeNuisanceGroups theory "
        theoryName = "_noTH"

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
    print launchcommand
    print command
    os.system (launchcommand)

