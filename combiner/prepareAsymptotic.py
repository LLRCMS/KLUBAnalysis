#! /usr/bin/env python
import sys, pwd, commands, optparse
import os
import re

#define function for parsing options
def parseOptions():

    usage = ('usage: %prog [options] datasetList\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)
    
    parser.add_option('-n', '--quantile',   dest='n',       type='string',    default='0.500',     help='quantile')
    parser.add_option('-m', '--mass',   dest='m',       type='int',    default='21',     help='lambda')
 
    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()



if __name__ == "__main__":
    parseOptions()
    global opt, args

    cmsswBase=os.environ['CMSSW_BASE']
    jobsDir = os.getcwd()
    #create a wrapper for standalone cmssw job
    scriptFile = open('%s/runJob_Asym_%s.sh'%(jobsDir,opt.n), 'w')
    scriptFile.write('#!/bin/bash\n')
    scriptFile.write('export X509_USER_PROXY=~/.t3/proxy.cert\n')
    scriptFile.write('source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
    scriptFile.write('export SCRAM_ARCH=slc6_amd64_gcc472\n')
    scriptFile.write('cd {}\n'.format(cmsswBase + '/src/'))
    scriptFile.write('eval `scram r -sh`\n')
    scriptFile.write('cd %s\n'%jobsDir)
    #if (float(opt.n)>0) :    
        #scriptFile.write('combine -M HybridNew --frequentist -m 125.0 --testStat LHC %s/comb.root  -H ProfileLikelihood -n forLim_%s --expectedFromGrid=%s &> out_%s.log \n' % (jobsDir,opt.n,opt.n,opt.n))
    #scriptFile.write('combine -M Asymptotic %s/comb.root -m %d -n %s_forLim_blind --run=blind &> out_Asym_%s.log \n' % (jobsDir,opt.m,opt.n,opt.n))
    scriptFile.write('combine -M Asymptotic %s/comb.root -m %d -n %s_forLim &> out_Asym_%s.log \n' % (jobsDir,opt.m,opt.n,opt.n))
    scriptFile.write('combine -M Asymptotic %s/comb.root -m %d -n %s_forLim_noTH --freezeNuisances QCDscale_ggHH,pdf_ggHH,HH_BR_Hbb,HH_BR_Htt &> out_Asym_%s_noTH.log \n' % (jobsDir,opt.m,opt.n,opt.n))
    scriptFile.write('echo "All done for job %s" \n'%opt.n)
    scriptFile.close()
    os.system('chmod u+rwx %s/runJob_Asym_%s.sh'%(jobsDir,opt.n))
    #os.system("/opt/exp_soft/cms/t3/t3submit -q cms \'%s/runJob_Asym_%s.sh\'"%(jobsDir,opt.n))
    os.system("/opt/exp_soft/cms/t3/t3submit_new -short \'%s/runJob_Asym_%s.sh\'"%(jobsDir,opt.n))
