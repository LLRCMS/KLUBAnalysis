# coding: utf-8

_all_ = [ 'make_input_lists' ]

import os
from os import path as op
import sys
import warnings
import argparse
from argparse import ArgumentParser, RawTextHelpFormatter
from subprocess import Popen, PIPE

def create_dir(d):
    if not op.exists(d):
        os.makedirs(d)
        
def make_input_lists(args):
    xrd_door = {'llr': 'root://eos.grif.fr/',
                'uhh': 'root://dcache-cms-xrootd.desy.de:1094/',
                'mib': '???'}[args.institute]

    store = {'llr': '/eos/grif/cms/llr/store/user/' + args.user + '/HHNtuples_res',
             'uhh': '???',
             'mib': '???'}[args.institute]
    if '16' in args.data_period:
        store = op.join(store, 'UL16')
    else:
        store = op.join(store, args.data_period)
        
    prefix   = args.kind
    path     = xrd_door + op.join(store, prefix + '_' + args.tag)

    leaf_dir = args.data_period + '_' + args.kind
    this_dir = op.dirname(op.abspath(__file__))
    out_dir  = op.normpath( op.join(op.dirname(this_dir), 'inputFiles', leaf_dir) )
    create_dir(out_dir)
    
    rfdir  = '/usr/bin/gfal-ls -lH'
    awk    = "| awk '{{print $9}}'"
    rfcomm = lambda s : rfdir + ' {} '.format(s) + awk
    print('Command run: {}'.format(rfcomm(path)))
    pipeopt = dict(shell=True, stdout=PIPE, encoding='utf-8')
    pipe   = Popen(rfcomm(path), **pipeopt)

    all_lists = {}
    for smpl_name in pipe.stdout:
        smpl_path = op.join(path, smpl_name).strip()
        if args.sample not in smpl_name and args.sample != 'all':
            continue
        smpl_name = smpl_name.replace('\n', '')
        all_lists[smpl_name] = []

        for _ in range(3): #smpl_name, tag, hash subfolders
            comm = rfcomm(smpl_path.strip())
            out = Popen(comm, **pipeopt).stdout.readlines()
            nlines = len(out)
            if nlines > 0:
                if nlines > 1:
                    mes = 'In {} there are too many subfolder. '.format(smpl_path)
                    mes += 'Using the last one (most recent submission).'
                    warnings.warn(mes)   
                smpl_path = op.join(smpl_path, out[-1].strip())

        # loop over the folders 0000, 1000, etc...
        comm = rfcomm(' {} '.format(smpl_path.strip()))
        out = Popen(comm, **pipeopt).stdout.readlines()
        for subfold in out:
            final_dir = op.join(smpl_path, subfold.strip())
            str_comm = '{} | grep HTauTauAnalysis'
            file_comm = rfcomm( str_comm.format(final_dir.strip()) )
            out = Popen(file_comm, **pipeopt).stdout.readlines()
            for filename in out:
                name = op.join(final_dir, filename.strip())
                all_lists[smpl_name].append(name)

    if len(all_lists.keys()) != 1: # currently calling this script once per sample
        mes = 'Number of compatible files: {}'.format(len(all_lists.keys()))
        raise RuntimeError(mes)

    if not all_lists:
        mes = 'Sample {} was not found in {}.'.format(args.sample, path)
        raise RuntimeError(mes)
    
    for sample, alist in all_lists.items():
        if not alist: 
            warnings.warn('Folder for dataset {} is empty'.format(sample))
        else:
            with open(op.join(out_dir, args.outtxt+'.txt'), 'w') as f:
                for l in alist:
                    f.write(l + '\n')

if __name__ == "__main__":
    usage = 'Produces lists with input files for skim jobs. \n'
    usage += 'Run example: `python scripts/makeListOnStorage.py -t Jul2022 --data_period UL18 -d 1`.'
    parser = ArgumentParser(description=usage, formatter_class=RawTextHelpFormatter)
    parser.add_argument('-t', '--tag', required=True, type=str, help='Tag used for the input big ntuples')
    parser.add_argument('-d', '--data_period', required=True, type=str,
                        help='Which data period to choose: Legacy2018, UL18, ...')
    parser.add_argument('-u', '--user', required=True, type=str, help='User who produced the input data')
    parser.add_argument('-x', '--institute', dest='institute', default='llr',
                        choices=('llr', 'uhh', 'mib'), required=False, type=str,
                        help='Which set of machines to use: LLR, Hamburg or Milano.')
    help_sample = ( 'For which sample to create the input list.\n' +
                    'Passing "all" runs the macro over everything.' )
    parser.add_argument('-s', '--sample', required=True, type=str, help=help_sample)
    parser.add_argument('-o', '--outtxt', required=True, type=str, help="Name of the txt output file.")
    parser.add_argument('-k', '--kind', required=True, type=str,
                        choices=('Data', 'Sig', 'MC'),
                        help='Sample type.')

    FLAGS = parser.parse_args()
    make_input_lists(FLAGS)
