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

def is_job_sucessful(rootfile, logfile, verb=False):
    if not os.path.exists(rootfile):
        if FLAGS.verb:
            print('ROOT file {} missing.'.format(rootfile))
        return False

    if not is_good_file(rootfile):
        if FLAGS.verb:
            print('ROOT file {} is bad.'.format(rootfile))
        return False

    if not os.path.exists(logfile) :
        if FLAGS.verb:
            print('Log file {} missing.'.format(logfile))
        return False
    
    with open(logfile, 'r') as logfile:
        problems = [word for word in logfile.readlines()
                    if 'Error' in word and 'TCling' not in word]
        if len(problems) != 0:
            if FLAGS.verb:
                mes = ''.join("Found error '{}' ".format(problems[0]),
                              'in log file {}.'.format(logfile))
                print(mes)
            return False

    return True

if __name__ == "__main__":
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
