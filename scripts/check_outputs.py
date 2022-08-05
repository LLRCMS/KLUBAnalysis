# coding: utf-8

_all_ = [ 'is_job_sucessful' ]

import os
import argparse

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
            print('File {} missing.'.format(rootfile))
        return False

def find_error_messages(afile, verb):
    with open(afile, 'r') as f:
        problems = [word for word in f.readlines()
                    if 'Error' in word and 'TCling' not in word]
        if len(problems) != 0:
            if verb:
                mes = ''.join("Found error '{}' ".format(problems[0]),
                              'in log file {}.'.format(afile))
                print(mes)
            return True
    return False

def is_job_sucessful(rootfile, outfile, errfile, logfile, verb=False):
    files = {rootfile, outfile, errfile, logfile}
    for afile in files:
        if not file_exists(afile, verb):
            return False
    
    if not is_good_file(rootfile):
        if FLAGS.verb:
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
    parser.add_argument('-i', '--input_folder', dest='input_folder',
                        required=True, help='input folder')
    parser.add_argument('-f', '--rootfile', dest='rootfile',
                        required=True, help='ROOT file')
    parser.add_argument('-l', '--logfile', dest='logfile',
                        required=True, help='log file')
    FLAGS = parser.parse_args()

    rootfile = os.path.join(FLAGS.input_folder, FLAGS.rootfile)
    logfile = os.path.join(FLAGS.input_folder, FLAGS.logfile)
    is_job_successful(rootfile, logfile)
