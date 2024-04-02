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
                    if ((('Error' in w or 'ERROR' in w) and
                         'WARNING' not in w and 'Warning' not in w and 'TCling' not in w) or
                        ('R__unzip: error' in w) or
                        ('SysError in <TFile::WriteBuffer>: error writing to file' in w) or
                        ('The system macro SYSTEM_PERIODIC_REMOVE expression') in w)]
        if len(problems) != 0:
            if verb:
                mes = 'Found errors in file {}:\n'.format(afile)
                for problem in problems:
                    mes += '  {}'.format(problem)
                print(mes)
            return True
    return False

def did_job_complete(afile, verb):
    with open(afile, 'r') as f:
        lines = f.readlines()
        if '... SKIM finished, exiting.' not in lines[-1]:
            if verb:
                mes = 'The last line of file {} shows: '.format(afile)
                mes += lines[-1]
                print(mes)
            return False
    return True

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

    if not did_job_complete(outfile, verb):
        return True

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

    outdir = os.path.dirname(FLAGS.rootfile)
    out = 'goodfiles' if success else 'badfiles'
    extract = os.path.basename(FLAGS.errfile).split('.')[0]
    if '_' in extract:
        out += '_' + extract.split('_')[1]
    out += '.txt'
    with open(os.path.join(outdir, out), 'a') as f:
        f.write(FLAGS.rootfile + '\n')
