import os
import sys
import argparse
import datetime

parser = argparse.ArgumentParser(description='Command line parser of plotting options')
parser.add_argument('--cfg', dest='cfg', required=True,
                    help='name of the main cfg to run', default=None)
parser.add_argument('--tag', dest='tag', required=True, default=None,
                    help='name of working space (defaults to timestamp)', )
parser.add_argument('--njobs', dest='njobs', required=False, type=int,
                    help='number of jobs for parallelization', default=10)
FLAGS = parser.parse_args()

#hourdate = datetime.datetime.now().strftime('%Y.%m.%d_%H.%M.%S').replace('.','-')
outdir = "filljobs_" + FLAGS.tag
exe = 'testAnalysisHelper.exe'
scriptname = 'filler.sh'
logname = 'log_${1}.txt'

write = lambda stream, text: stream.write(text + '\n')

def create_dir(d):
    if not os.path.exists(d):
        os.makedirs(d)
create_dir(outdir)

scriptpath = os.path.join(outdir, scriptname)
scriptpath_condor = scriptpath.replace('.sh','.condor')
joblog = os.path.join(outdir, 'joblogs')
create_dir(joblog)
joblog = os.path.join(joblog, logname)
with open(scriptpath, 'w') as s:
    write(s,'#!/bin/bash')
    write(s,'export X509_USER_PROXY=~/.t3/proxy.cert')
    write(s,'source /cvmfs/cms.cern.ch/cmsset_default.sh')
    write(s,'cd {}'.format(os.getcwd()))
    write(s,'export SCRAM_ARCH=slc6_amd64_gcc491')
    write(s,'eval `scram r -sh`')
    write(s,'source scripts/setup.sh')
    command = ( exe + ' ' + FLAGS.cfg + ' ${1}' + ' ' +
                str(FLAGS.njobs) + ' 2>&1 | tee ' + joblog )
    write(s,command)
    os.system('chmod u+rwx ' + os.path.join(outdir, scriptname))

condlog = os.path.join(outdir, 'outputs')
create_dir(condlog)
with open(scriptpath_condor, 'w') as s:
    write(s,'Universe = vanilla')
    write(s,'Executable = {}'.format(scriptpath))
    write(s,'input = /dev/null')
    write(s,'output = {}/condor_log_$(Process).o'.format(condlog))
    write(s,'error  = {}/condor_log_$(Process).e'.format(condlog))
    write(s,'getenv = true')
    write(s,'T3Queue = short')
    write(s,'WNTag=el7')
    write(s,'+SingularityCmd = ""')
    write(s,'include : /opt/exp_soft/cms/t3_tst/t3queue |')
    write(s,'Arguments = $(Process) ')
    write(s,'queue {}'.format(FLAGS.njobs))
    
launch_command = 'condor_submit {}'.format(scriptpath_condor)
print('The following command was run: {}'.format(launch_command))
os.system(launch_command)
