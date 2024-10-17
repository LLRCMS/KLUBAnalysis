# coding: utf-8

_all_ = [ 'skim_ntuple' ]

import os
import sys
import re
import glob
import argparse
import time
import subprocess
import ROOT

def create_dir(d):
    if not os.path.exists(d):
        os.makedirs(d)

def double_join(*args):
    str1 = ' '.join(args)
    str2 = '\n#### Line-by-line command: ####\n'
    str2 += '# Exec: {}\n'.format(args[0])
    _, ext = args[0].split('.')
    if ext == 'py':
        regex = re.compile('(?:-|--)(.+)\s(.*)')
        for i,arg in enumerate(args[1:]):
            matches = regex.findall(arg)
            assert len(matches) == 1
            str2 += '# {}:\t{}\n'.format(matches[0][0],matches[0][1])
    else:
        str2 += ''.join( ['# {}:\t{}\n'.format(i+1,arg)
                          for i,arg in enumerate(args[1:])] )
    return str1, str2

def remove_file(f):
    if os.path.exists(f):
        os.remove(f)

def parse_input_file_list(indir, insample):
    filelist = []
    sample = os.path.join(indir, insample + ".txt")
    with open(sample, 'r') as f:
        for line in f:
            line = (line.split("#")[0]).strip()
            if line:
                filelist.append(line)
    return filelist

def write_condor_file(d, condor_name, shell_exec, c_exec, py_exec,
                      queue, qvars, queue_mode):
        condouts = os.path.join(d, 'logs')
        create_dir(condouts)
        paths = {'out': '{}/$({{}}){{}}.out'.format(condouts),
                 'err': '{}/$({{}}){{}}.err'.format(condouts),
                 'log': '{}/$({{}}){{}}.log'.format(condouts)}

        with open(condor_name, 'w') as s:
            s.write( '\n'.join(('Universe = vanilla',
                                'Executable = {}'.format(shell_exec),
                                'input = {}'.format(c_exec),
                                'output = ' + paths['out'].format(qvars[0],qvars[1]),
                                'error = ' + paths['err'].format(qvars[0],qvars[1]),
                                'log = ' + paths['log'].format(qvars[0],qvars[1]),
                                'getenv = true',
                                '+JobBatchName="{}"'.format(FLAGS.sample),
                                'should_transfer_files = YES',
                                'transfer_input_files = {}'.format(py_exec),
                                '',
                                'T3Queue = {}'.format(queue_mode),
                                'WNTag=el7',
                                '+SingularityCmd = ""',
                                '',
                                'request_cpus   = 1',
                                'request_memory = 4GB',
                                'request_disk   = 2000',
                                '',
                                'include : /opt/exp_soft/cms/t3/t3queue |',
                                '',
                                'Arguments = $({}) {}'.format(qvars[0], qvars[1]))) + '\n' )
            s.write(queue + '\n')

        return condouts

def skim_ntuple(FLAGS, curr_folder):
    arg1, arg2 = '${1}', '${2}'
    io_names = ( '{}.txt'.format(arg1),
                 'output_{}.root'.format(arg1),
                 '{}{}.log'.format(arg1,arg2) )
    py_exec = os.path.join(os.path.dirname(os.path.dirname(FLAGS.config)), 'scripts', 'check_outputs.py')

    if FLAGS.config == 'none':
        print('Config file missing, exiting')
        sys.exit(1)

    if FLAGS.input_folder[-1] == '/':
        FLAGS.input_folder = FLAGS.input_folder[:-1]
    if FLAGS.output == 'none':
        FLAGS.output = FLAGS.input_folder + '_SKIM'

    if not os.path.exists(FLAGS.input_folder) :
        print('The input folder {} does not exists. Exiting.'.format(FLAGS.input_folder))
        sys.exit(1)

    jobs_dir = os.path.join(FLAGS.output, FLAGS.sample)
    create_dir(jobs_dir)
    os.system('cp ' + FLAGS.config + ' ' + jobs_dir)

    jobs_dir = jobs_dir.rstrip('.txt')
    if float(FLAGS.klreweight) > -990 and FLAGS.BSMname == 'none':
        print('[WARNING] You requested manual HH reweighting, but did not set a proper BSMname! Exiting!')
        sys.exit(0)
    elif FLAGS.EFTrew != 'none':
        jobs_dir += '_' + FLAGS.EFTrew
    elif FLAGS.BSMname != 'none':
        jobs_dir += '_' + FLAGS.BSMname

    create_dir(jobs_dir)
    job_name_shell = os.path.join(jobs_dir, 'job.sh')
    if not FLAGS.resub:
        remove_file(job_name_shell)
    condor_name = job_name_shell.replace('.sh', '.condor')

    if FLAGS.resub:
        rcount = 0
        nsplit = condor_name.split('.')
        while os.path.exists(condor_name):
            rcount += 1
            condor_name = nsplit[0] + '_resub' + str(rcount) + '.' + nsplit[1]

        regex = re.compile('.*output_(.+)\.root')
        failed = []

        badfiles_folder = os.path.join(FLAGS.output, FLAGS.sample)
        glob_badfiles = glob.glob(os.path.join(badfiles_folder, 'bad*.txt'))
        if len(glob_badfiles)==0:
            sys.exit(0)

        badfiles_recent = max(glob_badfiles, key=os.path.getctime)
        badfiles = os.path.join(FLAGS.output, FLAGS.sample, badfiles_recent)

        with open(badfiles, 'r') as badf:
            for line in badf.readlines():
                fail = line
                finded = regex.findall(line)
                assert len(finded)==1
                failed.append(finded[0])

        queue_vars = 'Item', '_resub' + str(rcount)
        queue = 'queue {} from (\n'.format(queue_vars[0])
        queue += '\n'.join(failed)
        queue += '\n)'
        
        write_condor_file(d=jobs_dir, condor_name=condor_name,
                          shell_exec=job_name_shell,
                          c_exec=FLAGS.exec_file,
                          py_exec=py_exec,
                          queue=queue, qvars=queue_vars,
                          queue_mode=FLAGS.queue_mode)
            
        launch_command = 'condor_submit {}'.format(condor_name)
        if FLAGS.verb:
            print('Resubmission with: {}'.format(launch_command))
        print('Resubmission with: {}'.format(launch_command))
        os.system(launch_command)
        time.sleep(0.5)
        sys.exit(0)

    # submit the jobs
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    inputfiles = parse_input_file_list(FLAGS.input_folder, FLAGS.sample)
    nfiles = len(inputfiles)
    njobs = nfiles if FLAGS.njobs > nfiles else FLAGS.njobs
    div, mod = divmod(nfiles, njobs)
    if mod >= njobs:
        mes = ' '.join( ('The method being used to assign files to jobs does not work.',
                         'with {} files and {} jobs.'.format(nfiles,njobs),
                         'Please increase the number of jobs.') )
        raise ValueError(mes)
    nfiles_per_job = [ div if i >= mod else div+1 for i in range(njobs)]
    assert sum(nfiles_per_job) == nfiles

    accumulate = lambda l : [sum(l[:y]) for y in range(1, len(l)+1)]
    inputlists = [ inputfiles[x-y:x] for x,y in zip(accumulate(nfiles_per_job), nfiles_per_job) ]
    assert len(inputlists) == njobs
    assert len([item for sublist in inputlists for item in sublist]) == nfiles
    
    mes = ( '{} jobs will be scheduled for {} files.'
            .format(njobs,len(inputfiles),nfiles_per_job) )
    print(mes)

    lists_dir = os.path.join(jobs_dir, 'filelists')
    create_dir(lists_dir)
    for ij,listname in enumerate(inputlists):
        list_file_name = io_names[0].replace(arg1, str(ij))
        with open(os.path.join(lists_dir, list_file_name), 'w') as input_list_file:
            for line in listname:
                input_list_file.write(line + '\n')
 
    clog = write_condor_file(d=jobs_dir, condor_name=condor_name,
                             shell_exec=job_name_shell, c_exec=FLAGS.exec_file,
                             py_exec=py_exec,
                             queue='queue ' + str(njobs), qvars=('Process',''),
                             queue_mode=FLAGS.queue_mode)

    with open(job_name_shell, 'w') as s:
        s.write( '\n'.join(('#!/usr/bin/env bash',
                            '',
                            'export X509_USER_PROXY=~/.t3/proxy.cert',
                            'source /cvmfs/cms.cern.ch/cmsset_default.sh',
                            'eval `scram r -sh`',
                            'cd {}'.format(curr_folder),
                            'source scripts/setup.sh')) + '\n' )
                
        yes_or_no = lambda s : '1' if bool(s) else '0'

        command, comment = double_join(FLAGS.exec_file,
                                       os.path.join(lists_dir, io_names[0]),
                                       os.path.join(jobs_dir, io_names[1]),
                                       FLAGS.xs,
                                       yes_or_no(FLAGS.isdata),
                                       FLAGS.config,
                                       yes_or_no(FLAGS.dokinfit),
                                       FLAGS.xsscale,
                                       FLAGS.htcut,
                                       FLAGS.htcutlow,
                                       yes_or_no(FLAGS.toprew),
                                       yes_or_no(FLAGS.dystitching),
                                       FLAGS.topstitch,
                                       yes_or_no(FLAGS.domt2),
                                       yes_or_no(FLAGS.ishhsignal),
                                       FLAGS.njets,
                                       FLAGS.EFTrew,
                                       FLAGS.order,
                                       FLAGS.uncert,
                                       FLAGS.cms_fake,
                                       FLAGS.klreweight,
                                       FLAGS.ktreweight,
                                       FLAGS.c2reweight,
                                       FLAGS.cgreweight,
                                       FLAGS.c2greweight,
                                       FLAGS.susyModel,
                                       FLAGS.PUweights,
                                       FLAGS.DY_nJets,
                                       FLAGS.DY_nBJets,
                                       yes_or_no(FLAGS.DY),
                                       yes_or_no(FLAGS.ttHToNonBB),
                                       yes_or_no(FLAGS.hhNLO),
                                       FLAGS.year,
                                       str(FLAGS.datasetType),
                                       yes_or_no(FLAGS.isDYlike),
                                       yes_or_no(FLAGS.isTTlike))

        s.write(comment + '\n')

        # out and err files are written by htcondor once the job completes
        # we need to check them 'live', i.e., while the job is running
        livedir = os.path.join(jobs_dir, 'live_logs')
        create_dir(livedir)
        local_out = os.path.join(livedir, '{}{}.out'.format(arg1,arg2))
        local_err = os.path.join(livedir, '{}{}.err'.format(arg1,arg2))
        s.write(command + ' 1>{} 2>{}\n'.format(local_out, local_err))
        
        command, comment = double_join('python {}'.format(py_exec),
                                       '-r ' + os.path.join(jobs_dir, io_names[1]),
                                       '-o ' + local_out,
                                       '-e ' + local_err,
                                       '-l ' + os.path.join(clog, '{}{}.log'.format(arg1,arg2)),
                                       '-v ')

        s.write(comment + '\n')
        fd = 9 # any above 3 should work
        # s.write('\n'.join(('(',
        #                    '  flock -x -w 5.0 {} || exit 1'.format(fd),
        #                    '  ' + command,
        #                    '  echo "Job {} is exiting the lock."'.format(arg1),
        #                    ') {}>{}/lock_file\n\n'.format(fd, livedir))))
        s.write(command + '\n\n') # the lock above ocasionally fails for unknown reasons
        
        if FLAGS.doSyst:
            sys_command, sys_comment = double_join('skimOutputter.exe',
                                                   os.path.join(jobs_dir, io_names[1]),
                                                   os.path.join(jobs_dir, 'syst_'+io_names[1]),
                                                   FLAGS.config,
                                                   yes_or_no(FLAGS.isdata))

            sys_command += (' ' + '>& ' + os.path.join(jobs_dir, 'syst_' + io_names[2]) )
            sys_comment += ('# extra:\t' + '>& ' + os.path.join(jobs_dir, 'syst_' + io_names[2]) )
            s.write(sys_command + '\n')
            s.write(sys_comment + '\n')
        
        s.write('echo "Job with id '+arg1+' completed."\n')
        os.system('chmod u+rwx {}'.format(job_name_shell))
 
    launch_command = 'condor_submit {}'.format(condor_name)

    time.sleep(0.1)
    print('The following command was run: \n  {}'.format(launch_command))
    os.system(launch_command)

if __name__ == "__main__":
    usage = 'Command line parser of skimming a bit Ntuple.'
    parser = argparse.ArgumentParser(description=usage)
    parser.add_argument('-i', '--input_folder', dest='input_folder', default='none', help='input folder')
    parser.add_argument('--exec_file', required=True, help='folder where the C++ skimmer executable is stored')
    parser.add_argument('--sample', default='none', help='input sample')
    parser.add_argument('-Y', '--year', default='2018', help='year', choices=['2016preVFP', '2016postVFP', '2017', '2018'])
    parser.add_argument('-x', '--xs', help='sample xs', default='1.')
    parser.add_argument('-o', '--output', default='none', help='output folder')
    parser.add_argument('-q', '--queue_mode', default='long', choices=('short', 'long'), help='batch queue mode')
    parser.add_argument('-r', '--resub', action='store_true', help='resubmit failed jobs')
    parser.add_argument('-v', '--verb', default=0, type=int, help='verbose')
    parser.add_argument('-d', '--isdata', default=0, type=int, help='data flag')
    parser.add_argument('--datasetType', dest='datasetType', default=0, type=int, choices=[0,1,2],
                        help='Type of dataset being considered, used for avoiding duplicated events. 0: default, 1: MET dataset 2: SingleTau dataset.')
    parser.add_argument('-T', '--tag', default='', help='folder tag name')
    parser.add_argument('-H', '--hadd', default='none', help='hadd the resulting ntuples')
    parser.add_argument('-c', '--config', default='none', help='skim config file')
    parser.add_argument('-n', '--njobs', default=100, type=int, help='number of skim jobs')
    parser.add_argument('-k', '--kinfit', dest='dokinfit', default=1, type=int, help='run HH kin fitter')
    parser.add_argument('-m', '--mt2', dest='domt2', default=1, type=int, help='run stransverse mass calculation')
    parser.add_argument('-y', '--xsscale', default='1.0',  help='scale to apply on XS for stitching')
    parser.add_argument('-Z', '--htcutlow', default='-999.0', help='HT low cut for stitching on inclusive')
    parser.add_argument('-z', '--htcut', default='-999.0', help='HT cut for stitching on inclusive')
    parser.add_argument('-e', '--njets', default='-999', help='njets required for stitching on inclusive')
    parser.add_argument('-t', '--toprew', default=0, type=int, help='is TT bar sample to compute reweight?')
    parser.add_argument('-b', '--topstitch', default='0', help='type of TT gen level decay pruning for stitch')
    parser.add_argument('-g', '--dystitching', default=0, type=int, help='whether to use DY stitching or not.')
    parser.add_argument('-a', '--ishhsignal', default=0, type=int, help='isHHsignal')
    parser.add_argument('--BSMname', default='none', help='additional name for EFT benchmarks')
    parser.add_argument('--EFTbm', dest='EFTrew', default='none',
                        help='EFT benchmarks [SM, 1..12, 1b..7b, 8a, c2scan, manual]')
    parser.add_argument('--order', default='nlo', help='order of reweight: lo/nlo')
    parser.add_argument('--uncert', default='0', help='uncertainty on the reweight coeffs')
    parser.add_argument('--cms_fake', default='0', help='invert some couplings for 2017/2018')
    parser.add_argument('--kl', dest='klreweight', default='-999.0', help='invert some couplings for 2017/2018')
    parser.add_argument('--kt', dest='ktreweight', default='-999.0', help='kt for dynamic reweight')
    parser.add_argument('--c2', dest='c2reweight', default='-999.0', help='c2 for dynamic reweight')
    parser.add_argument('--cg', dest='cgreweight', default='-999.0', help='cg for dynamic reweight')
    parser.add_argument('--c2g', dest='c2greweight', default='-999.0', help='c2g for dynamic reweight')
    parser.add_argument('--susy', dest='susyModel', default='NOTSUSY', help='name of susy model to select')
    parser.add_argument('--pu', dest='PUweights', default='none', help='name of susy model to select')
    parser.add_argument('--nj', dest='DY_nJets', default='-1', help='number of gen Jets for DY bins')
    parser.add_argument('--nb', dest='DY_nBJets', default='-1', help='number of gen BJets for DY bins')
    parser.add_argument('--DY', default=0, type=int, help='if it is a DY sample')
    parser.add_argument('--ttHToNonBB', default=0, type=int, help='if it is a ttHToNonBB sample')
    parser.add_argument('--hhNLO', default=False, action='store_true', help='if it is an HH NLO sample')
    parser.add_argument('--doSyst', default=False, action='store_true', help='compute up/down values of outputs')

    parser.add_option ('--isDYlike', help="Boosted corrections : if the process is of the kind V+jets", default=False, action='store_true')
    parser.add_option ('--isTTlike', help="Boosted corrections : if the process is top-enriched", default=False, action='store_true')


    FLAGS = parser.parse_args()
    if not FLAGS.isdata:
        assert FLAGS.datasetType == 0
    print("-----------  Configuration Arguments -----------")
    for arg, value in sorted(vars(FLAGS).items()):
        print("%s: %s" % (arg, value))
    print("------------------------------------------------") 
    curr_folder = os.getcwd()
    skim_ntuple(FLAGS, curr_folder)
