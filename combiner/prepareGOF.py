#! /usr/bin/env python
import sys, pwd, commands, optparse
import os
import re

#define function for parsing options
def parseOptions():

    usage = ('usage: %prog [options] datasetList\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)
    
    parser.add_option('-n', '--quantile',   dest='n',       type='string',    default='0',     help='quantile')
    
    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()



if __name__ == "__main__":
    parseOptions()
    global opt, args

    cmsswBase=os.environ['CMSSW_BASE']
    jobsDir = os.getcwd()
    #create a wrapper for standalone cmssw job
    scriptFile = open('%s/runJob_%s.sh'%(jobsDir,opt.n), 'w')
    scriptFile.write('#!/bin/bash\n')
    scriptFile.write('export X509_USER_PROXY=~/.t3/proxy.cert\n')
    scriptFile.write('source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
    scriptFile.write('export SCRAM_ARCH=slc6_amd64_gcc472\n')
    scriptFile.write('cd {}\n'.format(cmsswBase + '/src/'))
    scriptFile.write('eval `scram r -sh`\n')
    scriptFile.write('cd %s\n'%jobsDir)
    #scriptFile.write('cmsRun cmssw_%d_cfg.py &> log_%d_job.txt \n' % (n,n) )
    #scriptFile.write('cp SingleElectronPt35_PU0_GEN_SIM_%d.root %s\n'%(n,opt.output))
    #scriptFile.write('rm SingleElectronPt35_PU0_GEN_SIM_%d.root\n'%n)    
    scriptFile.write('combine -M GoodnessOfFit --algo=saturated -t 100 -s -1 -m 21 %s/comb.root  -n gof_%s &> out_gof_%s.log \n' % (jobsDir,opt.n,opt.n))
    scriptFile.write('echo "All done for job %s" \n'%opt.n)
    scriptFile.close()
    os.system('chmod u+rwx %s/runJob_%s.sh'%(jobsDir,opt.n))
    #os.system("/opt/exp_soft/cms/t3/t3submit -q cms \'%s/runJob_%s.sh\'"%(jobsDir,opt.n))
    os.system("/opt/exp_soft/cms/t3/t3submit_new -short  \'%s/runJob_%s.sh\'"%(jobsDir,opt.n))
