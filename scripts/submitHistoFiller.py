import os
import sys
import argparse
import datetime
import itertools

def create_dir(d):
    if not os.path.exists(d):
        os.makedirs(d)
    else:
        mes = 'Folder {d} already exists. You may want to remove it with `rm -r {d}`'.format(d=d)
        raise ValueError(mes)

def launch_jobs(args):
    tagdir = os.path.join(args.outdir, args.tag + '_' + args.year, args.channel)
    create_dir(tagdir)

    if args.with_dnn:
        from create_limit_configs import Params
        pars = Params(args.year)
        for spin, mass in itertools.product(pars.spins, pars.masses):
            subdir = os.path.join(tagdir, "Spin" + spin + "_Mass" + mass)
            create_dir(subdir)

    exe        = 'testAnalysisHelper.exe'
    scriptname = 'filler.sh'

    scriptpath = os.path.join(tagdir, scriptname)
    scriptpath_condor = scriptpath.replace('.sh','.condor')

    if args.cfg is None:
        if args.with_dnn:
            main_cfg = "config/limits/mainCfg_{}_{}_m${{3}}_s${{2}}.cfg".format(args.channel, args.year)
        else:
            main_cfg = "config/mainCfg_{}_{}.cfg".format(args.channel, args.year)
    else:
        main_cfg = "config/" + args.cfg + ".cfg"

    with open(scriptpath, 'w') as s:
        if args.with_dnn:
            command = ' '.join((exe, main_cfg, '${1}', str(args.njobs),
                                tagdir + '/Spin${2}_Mass${3}', str(int(args.use_friend))))
        else:
            command = ' '.join((exe, main_cfg, '${1}', str(args.njobs), tagdir, str(int(args.use_friend))))
            
        s.write('\n'.join(('#!/bin/bash',
                            'export X509_USER_PROXY=~/.t3/proxy.cert',
                            'export EXTRA_CLING_ARGS=-O2',
                            '. /opt/exp_soft/cms/t3/eos-login -username {} -wn'.format(args.eos),
                            'source /cvmfs/cms.cern.ch/cmsset_default.sh',
                            'cd {}'.format(os.getcwd()),
                            # 'export SCRAM_ARCH=slc6_amd64_gcc491',
                            'eval `scram r -sh`',
                            'source scripts/setup.sh',
                            command + '\n')))
        os.system('chmod u+rwx ' + os.path.join(tagdir, scriptname))
     
    condlog = os.path.join(tagdir, 'logs')
    create_dir(condlog)

    content = '\n'.join(('Universe = vanilla',
                         'Executable = {}'.format(scriptpath),
                         'input = /dev/null',
                         'getenv = true',
                         'should_transfer_files = YES',
                         '',
                         '+JobFlavour = "longlunch"',
                         '+JobBatchName = "{}-{}-{}"'.format(args.tag, args.channel, args.year),
                         '', ''))
    if args.with_dnn:
        content += '\n'.join(('output = {}/$(Spin)_$(Mass)_$(Idx).out'.format(condlog),
                              'error = {}/$(Spin)_$(Mass)_$(Idx).err'.format(condlog),
                              'log = {}/$(Spin)_$(Mass)_$(Idx).log'.format(condlog)))
    else:
        content += '\n'.join(('output = {}/$(Process).out'.format(condlog),
                              'error = {}/$(Process).err'.format(condlog),
                              'log = {}/$(Process).log'.format(condlog)))

    content += '\n'.join(('', '',
                          'T3Queue = {}'.format(args.queue),
                          'WNTag=el7',
                          '+SingularityCmd = ""',
                          '',
                          'request_cpus   = 1',
                          'request_memory = 4GB',
                          'request_disk   = 512MB',
                          'max_retries = 1',
                          '',
                          'include : /opt/exp_soft/cms/t3/t3queue |',
                          '', ''))

    if args.with_dnn:
        idxs = tuple(x for x in range(args.njobs))

        arguments = []
        for idx, spin, mass in itertools.product(idxs, pars.spins, pars.masses):
            arguments.append("  " + ', '.join((str(idx), spin, mass)))
        arguments = '\n'.join((arguments))
            
        content += '\n'.join(('Arguments = $(Idx) $(Spin) $(Mass) ',
                              'queue Idx, Spin, Mass from (',
                              arguments,
                              ')'
                              ))
    else:
        content += '\n'.join(('Arguments = $(Process) ',
                              'queue {}'.format(args.njobs)))
    
    with open(scriptpath_condor, 'w') as s:
        s.write(content)
    
    launch_command = 'condor_submit {}'.format(scriptpath_condor)
    print('The following command was run: {}'.format(launch_command))
    os.system(launch_command)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Command line parser of plotting options')
    parser.add_argument('-o', '--outdir', dest='outdir', required=False,
                        default=os.path.join('/data_CMS/cms/', os.environ['USER'], 'HHresonant_hist'),
                        help='name of working space (defaults to timestamp)', )
    parser.add_argument('-t', '--tag', required=True, help='Name of working space.')
    parser.add_argument('--cfg', help='Configuration file.', default=None)
    parser.add_argument('--njobs', required=False, type=int,
                        help='number of jobs for parallelization', default=10)
    parser.add_argument('--use_friend', action='store_true',
                        help='Use TTree friend mechanism (implies the existence of another tree).')
    parser.add_argument('--with_dnn', action='store_true',
                        help='Parallellize over spin and masses for DNN.')
    parser.add_argument('--year', required=True, choices=("UL16APV", "UL16", "UL17", "UL18"),
                        help='Data period.')
    parser.add_argument('--channel', required=True, choices=("ETau", "MuTau", "TauTau", "MuMu"),
                        help='Analysis channel.')
    parser.add_argument('--eos', default='bfontana',
                        help='EOS username (can be ignored if input files are not stored in /eos/.')
    qhelp = ('size of the HTCondor queue; use `short` for nominals ' +
             ' and `long` when running on systematic uncertainties')
    parser.add_argument('--queue', required=False, type=str, default='long', help=qhelp)
    FLAGS = parser.parse_args()

    launch_jobs(FLAGS)
