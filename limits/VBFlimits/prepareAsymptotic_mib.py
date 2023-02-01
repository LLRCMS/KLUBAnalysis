#! /usr/bin/env python
import sys, pwd, commands, optparse
import os
import re

#define function for parsing options
def parseOptions():
    usage = ('usage: %prog [options] datasetList\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)
    
    parser.add_option('-n'   , '--name' , dest='n'    , type='string', default=''   , help='name'        )
    parser.add_option('-b'   , '--blind', dest='blind',                default=False, action='store_true')
    parser.add_option('--ggF',            dest='ggF'  ,                default=False, action='store_true')
    parser.add_option('--VBF',            dest='VBF'  ,                default=False, action='store_true')
    parser.add_option('--kl' ,            dest='kl'   , type='string', default=None                      )
    parser.add_option('--kt' ,            dest='kt'   , type='string', default=None                      )
    parser.add_option('--CV' ,            dest='CV'   , type='string', default=None                      )
    parser.add_option('--C2V',            dest='C2V'  , type='string', default=None                      )
 
    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()



if __name__ == "__main__":
    parseOptions()
    global opt, args

    cmsswBase=os.environ['CMSSW_BASE']
    jobsDir = os.getcwd()
    scriptFile = open('%s/runJob_Asym_%s.sh'%(jobsDir,opt.n), 'w')
    scriptFile.write('#!/bin/bash\n')
    scriptFile.write('source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
    scriptFile.write('cd {}\n'.format(cmsswBase + '/src/'))
    scriptFile.write('eval `scram r -sh`\n')
    scriptFile.write('cd %s\n'%jobsDir)
    command = "combine -M AsymptoticLimits"
    if opt.blind:
        command = command + " --run blind "

    currFolder = os.getcwd ()
    command = command + " %s/comb.root" % jobsDir
    set_parameters = []
    red_parameters = []
    fre_parameters = []
    freeze = True

    # limits only on r_gghh
    if opt.ggF and not opt.VBF:
        red_parameters.append("r_gghh")
        fre_parameters.append("r")
        set_parameters.append("r_qqhh=0")

    # limits only on r_qqhh
    if opt.VBF and not opt.ggF:
        red_parameters.append("r_qqhh")
        fre_parameters.append("r")
        set_parameters.append("r_gghh=1")

    # limits on r - NOT to be used for now!
    #if opt.ggF and opt.VBF:
    #    red_parameters.append("r")
    #    freeze   = False
    #    set_parameters.append("r_qqhh=0")

    command     = command + " --redefineSignalPOIs " + ",".join(red_parameters)
    if freeze:
        command = command + " --freezeParameters "   + ",".join(fre_parameters)
    command     = command + " --setParameters "
    if len(set_parameters)>0:
        command = command + ",".join(set_parameters) + ","

    couplings = []

    if opt.kt:  couplings.append("kt="+opt.kt)
    if opt.kl:  couplings.append("kl="+opt.kl)
    if opt.CV:  couplings.append("CV="+opt.CV)
    if opt.C2V: couplings.append("C2V="+opt.C2V)

    command = command + ",".join(couplings)
    #scriptFile.write('%s -n %s_forLim &> out_Asym_%s.log \n' % (command, opt.n,opt.n))
    scriptFile.write('%s -n %s_forLim_noTH --freezeNuisanceGroups theory &> out_Asym_%s_noTH.log \n' % (command,opt.n,opt.n))
    scriptFile.write('echo "All done for job %s" \n'%opt.n)
    scriptFile.close()
    os.system('chmod u+rwx %s/runJob_Asym_%s.sh'%(jobsDir,opt.n))

    condorFile = open ('%s/condorLauncher_%s.sh'% (jobsDir,opt.n), 'w')
    condorFile.write ('Universe = vanilla\n')
    condorFile.write ('Executable  = %s/runJob_Asym_%s.sh\n'%(jobsDir,opt.n))
    condorFile.write ('Log         = condor_limits_$(ProcId).log\n')
    condorFile.write ('Output      = condor_limits_$(ProcId).out\n')
    condorFile.write ('Error       = condor_limits_$(ProcId).error\n')
    condorFile.write ('queue 1\n')
    condorFile.close ()

    launchcommand = ('condor_submit '+ jobsDir + '/condorLauncher_' + str (opt.n) + '.sh')
    #print launchcommand
    #print command
    os.system (launchcommand)