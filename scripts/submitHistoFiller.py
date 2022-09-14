import os
import sys
import argparse
import datetime

hourdate = datetime.datetime.now().strftime('%Y.%m.%d_%H.%M.%S').replace('.','-')

parser = argparse.ArgumentParser(description='Command line parser of plotting options')
parser.add_argument('--cfg', dest='cfg', required=True,
                    help='name of the main cfg to run', default=None)
parser.add_argument('-o', '--outdir', dest='outdir', required=False,
                    default=os.path.join('/data_CMS/cms/', os.environ['USER'], 'HHresonant_hist'),
                    help='name of working space (defaults to timestamp)', )
parser.add_argument('-t', '--tag', dest='tag', required=False,
                    default='HistoFiller_'+hourdate,
                    help='name of working space (defaults to timestamp)', )
parser.add_argument('--njobs', dest='njobs', required=False, type=int,
                    help='number of jobs for parallelization', default=10)
FLAGS = parser.parse_args()

exe        = 'testAnalysisHelper.exe'
scriptname = 'filler.sh'

def create_dir(d):
    if not os.path.exists(d):
        os.makedirs(d)
    else:
        mes = 'Folder {d} already exists. You may want to remove it ith `rm -r {d}`'.format(d=d)
        raise ValueError(mes)

tagdir = os.path.join(FLAGS.outdir, FLAGS.tag)
create_dir(tagdir)

scriptpath = os.path.join(tagdir, scriptname)
scriptpath_condor = scriptpath.replace('.sh','.condor')
with open(scriptpath, 'w') as s:
    command = (exe + ' ' + FLAGS.cfg + ' ${1}' + ' ' +
               str(FLAGS.njobs) + ' ' + tagdir)
    s.write('\n'.join(('#!/bin/bash',
                        'export X509_USER_PROXY=~/.t3/proxy.cert',
                        'source /cvmfs/cms.cern.ch/cmsset_default.sh',
                        'cd {}'.format(os.getcwd()),
                        'export SCRAM_ARCH=slc6_amd64_gcc491',
                        'eval `scram r -sh`',
                        'source scripts/setup.sh',
                        command + '\n')))
    os.system('chmod u+rwx ' + scriptpath)

condlog = os.path.join(tagdir, 'logs')
create_dir(condlog)
with open(scriptpath_condor, 'w') as s:
    s.write('\n'.join(('Universe = vanilla',
                       'Executable = {}'.format(scriptpath),
                       'input = /dev/null',
                       'output = {}/$(Process).out'.format(condlog),
                       'error  = {}/$(Process).err'.format(condlog),
                       'log  = {}/$(Process).log'.format(condlog),
                       'getenv = true',
                       'should_transfer_files = YES',
                       '+JobBatchName="{}"'.format('Histos_'+FLAGS.tag),
                       '',
                       'T3Queue = short',
                       'WNTag=el7',
                       '+SingularityCmd = ""',
                       'include : /opt/exp_soft/cms/t3_tst/t3queue |',
                       '',
                       'Arguments = $(Process) ',
                       'queue {}\n'.format(FLAGS.njobs))))
    
launch_command = 'condor_submit {}'.format(scriptpath_condor)
print('The following command was run: {}'.format(launch_command))
os.system(launch_command)
