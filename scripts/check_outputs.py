# coding: utf-8

_all_ = [ 'is_job_sucessful' ]

import os
import argparse
import ROOT

def is_good_file(fname):
    f = ROOT.TFile(fname)
    if f.IsZombie():
        return False
    if f.TestBit(ROOT.TFile.kRecovered):
        return False
    return True

def file_exists(afile, verb):
    if not os.path.exists(afile):
        if verb:
            print('File {} missing.'.format(afile))
        return False
    return True

def find_error_messages(afile, verb):
    with open(afile, 'r') as f:
        problems = [w for w in f.readlines()
                    if (('Error' in word and 'TCling' not in word) or
                        ('R__unzip: error' in word ) or
                        ('... SKIM finished, exiting.' not in word)) ]
        if len(problems) != 0:
            if verb:
                mes = 'Found errors in file {}:\n'.format(afile)
                for problem in problems:
                    mes += '  {}'.format(problem)
                print(mes)
            return True
    return False

def is_job_successful(rootfile, outfile, errfile, logfile, verb=False):
    files = {rootfile, outfile, errfile, logfile}
    for afile in files:
        if not file_exists(afile, verb):
            return False
    
    if not is_good_file(rootfile):
        if verb:
            print('ROOT file {} is bad.'.format(rootfile))
        return False

    files.remove(rootfile)
    for afile in files:
        if find_error_messages(afile, verb):
            return False

    return True

if __name__ == '__main__':
    usage = 'Checks the correctness of a ROOT file.'
    parser = argparse.ArgumentParser(description=usage)
    parser.add_argument('-r', '--rootfile', dest='rootfile',
                        required=True, help='ROOT file')
    parser.add_argument('-o', '--outfile', dest='outfile',
                        required=True, help='output file')
    parser.add_argument('-e', '--errfile', dest='errfile',
                        required=True, help='error file')
    parser.add_argument('-l', '--logfile', dest='logfile',
                        required=True, help='log file')
    parser.add_argument('-v', '--verbose', dest='verbose', default=False,
                        action='store_true', help='whether to have verbosity')

    FLAGS = parser.parse_args()

    success = is_job_successful(FLAGS.rootfile, FLAGS.outfile,
                                FLAGS.errfile, FLAGS.logfile, FLAGS.verbose)
    dir = os.path.dirname(FLAGS.rootfile)
    out = 'good' if success else 'bad'
    with open( os.path.join(dir, '{}files.txt'.format(out)), 'a' ) as f:
        f.write(FLAGS.rootfile + '\n')
