#!/usr/bin/env python

import os
import sys
import glob
import argparse
import time
import subprocess
import ROOT

def create_dir(d):
    if not os.path.exists(d):
        os.makedirs(d)

def remove_file(f):
    if os.path.exists(f):
        os.remove(f)

def is_good_file(fname):
    f = ROOT.TFile(fname)
    if f.IsZombie():
        return False
    if f.TestBit(ROOT.TFile.kRecovered):
        return False
    return True

def parse_input_file_list(indir, insample):
    filelist = []
    glob_pattern = '*' + insample + '*'
    sample = glob.glob( os.path.join(indir, glob_pattern) )
    if len(sample) != 1:
        mes = 'Exactly one file must be found. but {} were found.\n'.format(len(sample))
        mes += '  Input folder: {}.\n'.format(indir)
        mes += '  Pattern being searched: {}.\n'.format(glob_pattern)
        raise ValueError(mes)

    with open(sample[0], 'r') as f:
        for line in f:
            line = (line.split("#")[0]).strip()
            if line:
                filelist.append(line)
    return filelist

def skim_ntuple(FLAGS, curr_folder):
    arg1 = '${1}'
    io_names = ( 'filelist_{}_{}.txt'.format(FLAGS.sample,arg1),
                 'output_{}_{}.root'.format(FLAGS.sample,arg1),
                 'output_{}_{}.log'.format(FLAGS.sample,arg1) )
    
    # verify the result of the process
    if (FLAGS.hadd != 'none') :    
        with open( os.path.join(FLAGS.output, 'hadder.sh'), 'w') as s:
            s.write('\n'.join(('#!/bin/bash',
                               'source /cvmfs/cms.cern.ch/cmsset_default.sh',
                               'cd {}'.format(curr_folder),
                               'eval `scram r -sh`',
                               'source scripts/setup.sh',
                               'mkdir ' + os.path.join(FLAGS.output, 'singleFiles'),
                               ('mv ' + os.path.join(FLAGS.output, + '*') +
                                ' ' + os.path.join(FLAGS.output, 'singleFiles')),
                               ('hadd ' + os.path.join(FLAGS.output, 'total.root') +
                                ' ' + os.path.join(FLAGS.output, '/singleFiles/*.root')),
                               'touch ' + os.path.join(FLAGS.output, 'done'),
                               'echo "Hadding finished"')) + '\n')
        os.system('chmod u+rwx ' + FLAGS.output + '/hadder.sh')

        launch_command = 'condor_submit {}'.format(os.path.join(FLAGS.output, 'hadder.sh\''))
        if FLAGS.sleep:
            time.sleep(0.1)
        print('The following command was run: \n  {}'.format(launch_command))
        os.system(launch_command)
        sys.exit(0)

    # verify the result of the process
    if (FLAGS.resub != 'none'):
        if (FLAGS.input_folder == 'none'):
            print('Input folder to be checked missing')
            print('(this is the folder that contains the jobs to be submitted)')
            sys.exit(1)

        if FLAGS.input_folder[-1] == '/' :
            FLAGS.input_folder = FLAGS.input_folder[:-1]
        tagname = FLAGS.tag + "/" if FLAGS.tag else ''
        FLAGS.input_folder = tagname + 'SKIM_' + os.path.basename(FLAGS.input_folder)

        # check the log file
        missing = []
        for num in jobs:
            rootfile = io_names[1].replace(arg, num)
            if not os.path.exists(rootfile):
                if FLAGS.verb:
                    print('ROOT file {} missing at iteration {}.'.format(rootfile, num))
                missing.append(num)
                continue
            if not is_good_file(rootfile):
                if FLAGS.verb:
                    print('ROOT file {} corrupted at iteration {}.'.format(rootfile, num))
                missing.append(num)
                continue
            logfile = io_names[2].replace(arg1, num)
            if not os.path.exists(logfile) :
                if FLAGS.verb:
                    print(num, 'missing log file')
                missing.append(num)
                continue
            with open(logfile, 'r') as logfile:
                problems = [word for word in logfile.readlines () if 'Error' in word and 'TCling' not in word]
                if len(problems) != 0:
                    if FLAGS.verb:
                        print(num, 'found error ', problems[0])
                    missing.append(num)
        print('The following jobs did not end successfully:')
        print(missing)
        # The resubmission has to be rewritten
        # for num in missing :
        #     command = '`cat ' + os.path.join(FLAGS.input_folder, 'submit.sh') + ' | grep skimJob_' + num + '.sh | tr "\'" " "`'
        #     if FLAGS.verb:
        #         print(command)
        # if FLAGS.resub == 'run':
        #     for num in missing:
        #         command = '`cat ' + os.path.join(FLAGS.input_folder + 'submit.sh') + ' | grep skimJob_' + num + '.sh | tr "\'" " "`'
        #         time.sleep(int (num) % 5)
        #         os.system(command)
        sys.exit(0)

    # submit the jobs
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    if FLAGS.year == '2018':
        skimmer = 'skimNtuple2018_HHbtag.exe'
    elif FLAGS.year == '2017':
        skimmer = 'skimNtuple2017_HHbtag.exe'
    elif FLAGS.year == '2016':
        skimmer = 'skimNtuple2016_HHbtag.exe'

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

    if not FLAGS.force and os.path.exists(FLAGS.output):
        print('The output folder {} already exists. Exiting.'.format(FLAGS.output))
        sys.exit(1)
    elif os.path.exists(FLAGS.output):
        os.system('rm -rf ' + FLAGS.output + '/*')
    create_dir(FLAGS.output)
    os.system('cp ' + FLAGS.config + ' ' + FLAGS.output)

    inputfiles = parse_input_file_list(FLAGS.input_folder, FLAGS.sample)
    njobs = len(inputfiles) if FLAGS.njobs > len(inputfiles) else FLAGS.njobs
    nfiles_per_job = (len(inputfiles) + len(inputfiles) % njobs) / njobs
    inputlists = [inputfiles[x:x+nfiles_per_job]
                  
                  for x in range(0, len(inputfiles), nfiles_per_job)]
    mes = ( '{} jobs will be scheduled for {} files ({} files per job).'
            .format(njobs,len(inputfiles),nfiles_per_job) )
    print(mes)

    tagname = '/' + FLAGS.tag if FLAGS.tag else ''
    jobs_dir = curr_folder + tagname + '/SKIM_' + os.path.basename(FLAGS.input_folder)
    jobs_dir = jobs_dir.rstrip('.txt')
    if float(FLAGS.klreweight) > -990 and FLAGS.BSMname == 'none':
        print('[WARNING] You requested manual HH reweighting, but did not set a proper BSMname! Exiting!')
        sys.exit(0)
    elif FLAGS.EFTrew != 'none':
        jobs_dir = jobs_dir + '_' + FLAGS.EFTrew
    elif FLAGS.BSMname != 'none':
        jobs_dir = jobs_dir + '_' + FLAGS.BSMname

    create_dir(jobs_dir)
    job_name_shell = os.path.join(jobs_dir,
                                  ('skimJob_{}'.format(FLAGS.sample)
                                   .replace('.', 'DOT')) + '.sh')
    remove_file(job_name_shell)
    job_name_condor = job_name_shell.replace('.sh', '.condor')

    lists_dir = os.path.join(jobs_dir, 'filelists')
    create_dir(lists_dir)
    for ij,listname in enumerate(inputlists):
        #create a wrapper for standalone cmssw job
        list_file_name = io_names[0].replace(arg1, str(ij))
        with open(os.path.join(lists_dir, list_file_name), 'w') as input_list_file:
            for line in listname:
                input_list_file.write(line + '\n')

    with open(job_name_shell, 'w') as s:
        s.write( '\n'.join(('#!/usr/bin/env bash',
                            '',
                            'export X509_USER_PROXY=~/.t3/proxy.cert',
                            'source /cvmfs/cms.cern.ch/cmsset_default.sh',
                            'eval `scram r -sh`',
                            'cd {}'.format(curr_folder),
                            'source scripts/setup.sh')) + '\n' )
                
        yes_or_no = lambda s : '1' if bool(s) else '0'

        command = ' '.join( (skimmer,
                             os.path.join(lists_dir, io_names[0]),
                             os.path.join(FLAGS.output, io_names[1]),
                             FLAGS.xs,
                             yes_or_no(FLAGS.isdata),
                             FLAGS.config,
                             yes_or_no(FLAGS.dokinfit),
                             FLAGS.xsscale,
                             FLAGS.htcut,
                             FLAGS.htcutlow,
                             yes_or_no(FLAGS.toprew),
                             yes_or_no(FLAGS.genjets),
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
                             yes_or_no(FLAGS.hhNLO)) )

        if FLAGS.year == '2016':
            if FLAGS.isAPV:
                command += ' 1 '
            else:
                command += ' 0 '
        s.write(command + '\n')
      
        if FLAGS.doSyst:
            sys_command = ' '.join( ( 'skimOutputter.exe',
                                      os.path.join(FLAGS.output,
                                                   io_names[1]),
                                      os.path.join(FLAGS.output,
                                                   'syst_'+io_names[1]),
                                      FLAGS.config,
                                      yes_or_no(FLAGS.isdata)) )

            sys_command += (' ' + '>& ' + os.path.join(FLAGS.output, 'syst_' + io_names[2]) )
            s.write(sys_command + '\n')
        
        s.write('echo "Job with id '+arg1+' completed.\n"')
        os.system('chmod u+rwx {}'.format(job_name_shell))

    condor_vars = ('infile', 'outfile')
    condouts = os.path.join(jobs_dir, 'outputs')
    create_dir(condouts)
    condlogs = os.path.join(jobs_dir, 'logs')
    create_dir(condlogs)
    with open(job_name_condor, 'w') as s:
        s.write( '\n'.join(('Universe = vanilla',
                            'Executable = {}'.format(job_name_shell),
                            'input = /dev/null',
                            'output = {}/{}_$(Process).o'.format(condouts,FLAGS.sample),
                            'error  = {}/{}_$(Process).e'.format(condouts,FLAGS.sample),
                            'log  = {}/{}_$(Process).log'.format(condlogs,FLAGS.sample),
                            'getenv = true',
                            '',
                            'T3Queue = short',
                            'WNTag=el7',
                            '+SingularityCmd = ""',
                            '',
                            'include : /opt/exp_soft/cms/t3/t3queue |',
                            '',
                            'Arguments = $(Process)',
                            'queue {}'.format(njobs))) + '\n' )

    launch_command = 'condor_submit {}'.format(job_name_condor)

    if FLAGS.sleep:
        time.sleep(0.1)
    print('The following command was run: \n  {}'.format(launch_command))
    os.system(launch_command)

if __name__ == "__main__":
    usage = 'Command line parser of skimming a bit Ntuple.'
    parser = argparse.ArgumentParser(description=usage)
    parser.add_argument('-i', '--input_folder', dest='input_folder', default='none', help='input folder')
    parser.add_argument('--sample', dest='sample', default='none', help='input sample')
    parser.add_argument('-Y', '--year', dest='year', default='2018', help='year', choices=['2016', '2017', '2018'])
    parser.add_argument('-A', '--APV', dest='isAPV', default=False, help='isAPV')
    parser.add_argument('-x', '--xs', dest='xs', help='sample xs', default='1.')
    parser.add_argument('-f', '--force', dest='force', default=0, type=int,
                        help='replace existing reduced ntuples')
    parser.add_argument('-o', '--output', dest='output', default='none', help='output folder')
    parser.add_argument('-q', '--queue', dest='queue', default='short', help='batch queue')
    parser.add_argument('-r', '--resub', dest='resub', default='none', help='resubmit failed jobs')
    parser.add_argument('-v', '--verb', dest='verb', default=False, help='verbose')
    parser.add_argument('-s', '--sleep', dest='sleep', default=False, help='sleep in submission')
    parser.add_argument('-d', '--isdata', dest='isdata', default=False, help='data flag')
    parser.add_argument('-T', '--tag', dest='tag', default='', help='folder tag name')
    parser.add_argument('-H', '--hadd', dest='hadd', default='none', help='hadd the resulting ntuples')
    parser.add_argument('-c', '--config', dest='config', default='none', help='skim config file')
    parser.add_argument('-n', '--njobs', dest='njobs', default=100, type=int, help='number of skim jobs')
    parser.add_argument('-k', '--kinfit', dest='dokinfit', default='True', help='run HH kin fitter')
    parser.add_argument('-m', '--mt2', dest='domt2', default=True, help='run stransverse mass calculation')
    parser.add_argument('-y', '--xsscale', dest='xsscale', default='1.0',
                        help='scale to apply on XS for stitching')
    parser.add_argument('-Z', '--htcutlow', dest='htcutlow', default='-999.0',
                        help='HT low cut for stitching on inclusive')
    parser.add_argument('-z', '--htcut', dest='htcut', default='-999.0',
                        help='HT cut for stitching on inclusive')
    parser.add_argument('-e', '--njets', dest='njets', default='-999',
                        help='njets required for stitching on inclusive')
    parser.add_argument('-t', '--toprew', dest='toprew', default=False,
                        help='is TT bar sample to compute reweight?')
    parser.add_argument('-b', '--topstitch' , dest='topstitch' , default='0',
                        help='type of TT gen level decay pruning for stitch')
    parser.add_argument('-g', '--genjets', dest='genjets', default=False,
                        help='loop on genjets to determine the number of b hadrons')
    parser.add_argument('-a', '--ishhsignal', dest='ishhsignal', default=False, help='isHHsignal')
    parser.add_argument('--BSMname', dest='BSMname', default='none', help='additional name for EFT benchmarks')
    parser.add_argument('--EFTbm', dest='EFTrew', default='none',
                        help='EFT benchmarks [SM, 1..12, 1b..7b, 8a, c2scan, manual]')
    parser.add_argument('--order', dest='order', default='nlo', help='order of reweight: lo/nlo')
    parser.add_argument('--uncert', dest='uncert', default='0', help='uncertainty on the reweight coeffs')
    parser.add_argument('--cms_fake', dest='cms_fake', default='0', help='invert some couplings for 2017/2018')
    parser.add_argument('--kl', dest='klreweight', default='-999.0', help='invert some couplings for 2017/2018')
    parser.add_argument('--kt', dest='ktreweight', default='-999.0', help='kt for dynamic reweight')
    parser.add_argument('--c2', dest='c2reweight', default='-999.0', help='c2 for dynamic reweight')
    parser.add_argument('--cg', dest='cgreweight', default='-999.0', help='cg for dynamic reweight')
    parser.add_argument('--c2g', dest='c2greweight', default='-999.0', help='c2g for dynamic reweight')
    parser.add_argument('--susy', dest='susyModel', default='NOTSUSY', help='name of susy model to select')
    parser.add_argument('--pu', dest='PUweights', default='none', help='name of susy model to select')
    parser.add_argument('--nj', dest='DY_nJets', default='-1', help='number of gen Jets for DY bins')
    parser.add_argument('--nb', dest='DY_nBJets', default='-1', help='number of gen BJets for DY bins')
    parser.add_argument('--DY', dest='DY', default=False, help='if it is a DY sample')
    parser.add_argument('--ttHToNonBB', dest='ttHToNonBB', default=False,
                        help='if it is a ttHToNonBB sample')
    parser.add_argument('--hhNLO', dest='hhNLO', default=False, action='store_true',
                        help='if it is an HH NLO sample')
    parser.add_argument('--doSyst', dest='doSyst', default=False, action='store_true',
                        help='compute up/down values of outputs')

    FLAGS = parser.parse_args()
    curr_folder = os.getcwd()
    skim_ntuple(FLAGS, curr_folder)
