# coding: utf-8

_all_ = [ "make_input_lists" ]

import os
from os.path import join
import sys
import warnings
import argparse
from argparse import ArgumentParser, RawTextHelpFormatter
from subprocess import Popen, PIPE

def create_dir(d):
    if not os.path.exists(d):
        os.makedirs(d)
        
def make_input_lists(args):
    xrd_door = {'llr': 'root://polgrid4.in2p3.fr/',
                'uhh': 'root://dcache-cms-xrootd.desy.de:1094/'}[args.xrd_door]

    leaf_dir = args.data_period + '_' + args.kind
    this_dir = os.path.dirname(os.path.abspath(__file__))
    out_dir  = os.path.normpath(join(this_dir, '..', 'inputFiles', leaf_dir))
    create_dir(out_dir)

    dpm_home = '/dpm/in2p3.fr/home/cms/trivcat'
    store    = os.path.join('store/user/lportale/HHNtuples_res', args.data_period)
    prefix   = 'Data' if args.kind == 'Data' else 'MC'
    path     = join(dpm_home, store, prefix + '_' + args.tag)
    
    rfdir  = '/usr/bin/rfdir'
    awk    = "| awk '{{print $9}}'"
    rfcomm = lambda s : rfdir + ' {} '.format(s) + awk
    pipe   = Popen(rfcomm(path), shell=True, stdout=PIPE)

    all_lists = {}
     
    for smpl_name in pipe.stdout:
        smpl_path = join(path, smpl_name).strip()    
        if args.sample not in smpl_name and args.sample != 'all':
            continue

        smpl_name = smpl_name.replace('\n', '')
        all_lists[smpl_name] = []

        for _ in range(3): #smpl_name, tag, hash subfolders
            comm = rfcomm(smpl_path.strip())
            out = Popen(comm, shell=True, stdout=PIPE).stdout.readlines()
            nlines = len(out)
            if nlines > 0:
                if nlines > 1:
                    mes = 'In {} there are too many subfolder. '.format(smpl_path)
                    mes += 'Using the last one (most recent submission).'
                    warnings.warn(mes)   
                smpl_path = join(smpl_path, out[-1].strip())

        # loop over the folders 0000, 1000, etc...
        comm = rfcomm(' {} '.format(smpl_path.strip()))
        out = Popen(comm, shell=True, stdout=PIPE).stdout.readlines()
        for subfold in out:
            final_dir = join(smpl_path, subfold.strip())
            str_comm = '{} | grep HTauTauAnalysis'
            file_comm = rfcomm( str_comm.format(final_dir.strip()) )

            out = Popen(file_comm, shell=True, stdout=PIPE).stdout.readlines()
            for filename in out:
                name = xrd_door + join(final_dir, filename.strip())
                all_lists[smpl_name].append(name)

    if not all_lists:
        mes = 'Sample {} was not found in {}.'.format(args.sample, path)
        raise RuntimeError(mes)
    
    for sample, alist in all_lists.items():
        if not alist: 
            warnings.warn('Folder for dataset {} is empty'.format(sample))

if __name__ == "__main__":
    usage = 'Produces lists with input files for skim jobs. \n'
    usage += 'Run example: `python scripts/makeListOnStorage.py -t Jul2022 --data_period UL18 -d 1`.'
    parser = ArgumentParser(description=usage, formatter_class=RawTextHelpFormatter)
    parser.add_argument('-t', '--tag', dest='tag', required=True, type=str,
                        help='Tag used for the input big ntuples')
    parser.add_argument('-d', '--data_period', dest='data_period', required=True, type=str,
                        help='Which data period to choose: Legacy2018, UL18, ...')
    parser.add_argument('-x', '--xrd_door', dest='xrd_door', default='llr',
                        choices=('llr', 'uhh', 'mib'),
                        required=False, type=str, help='Which set of machines to use: LLR, Hamburg or Milano.')
    help_sample = ( 'For which sample to create the input list.\n' +
                    'Passing "all" runs the macro over everything.' )
    parser.add_argument('-s', '--sample', dest='sample',
                        required=True, type=str,
                        help=help_sample)
    parser.add_argument('-k', '--kind', dest='kind', required=True, type=str,
                        choices=('Data', 'Signals', 'Backgrounds'),
                        help='Sample type.')

    FLAGS = parser.parse_args()
    make_input_lists(FLAGS)
