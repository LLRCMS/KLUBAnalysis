# coding: utf-8

_all_ = []

import os
import unittest

import sys
sys.path.append('../scripts/')
from check_outputs import is_job_successful

class TestCheckOutputs(unittest.TestCase):

    def setUp(self):
        base = os.path.join('..', 'test', 'test_data')
        infile = 'VBFToBulkGravitonToHHTo2B2Tau_M-1500_test'
        self.rootf = os.path.join(base, '{}.root'.format(infile))
        self.outf  = os.path.join(base, '{}.out'.format(infile))
        self.errf  = os.path.join(base, '{}.err'.format(infile))
        self.logf  = os.path.join(base, '{}.log'.format(infile))

    def test_files(self):
        success = is_job_successful(self.rootf, self.outf,
                                    self.errf, self.logf,
                                    verb=True)
        self.assertTrue(success)
        

if __name__ == '__main__':
    unittest.main()
