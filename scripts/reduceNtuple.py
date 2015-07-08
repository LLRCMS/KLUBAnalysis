#!/usr/bin/env python

import os,sys
import optparse
import fileinput
import commands
import time
import glob
from os.path import basename


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


if __name__ == "__main__":

    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage)
    parser.add_option('-i', '--input'  ,    dest='input'  , help='input folder'  , default='none')
    parser.add_option('-o', '--output' ,    dest='output' , help='output folder' , default='none')
    (opt, args) = parser.parse_args()
    if opt.input[-1] == '/' : opt.input = opt.input[:-1]
    if opt.output == 'none' : opt.output = opt.input + '_RED'
    os.system ('mkdir ' + opt.output)
    
    inputfiles = glob.glob (opt.input + '/*.root')    
    
    for filename in inputfiles : 
        print ('./reduceNtuple ' + filename + ' ' + opt.output + '/' + basename (filename))
        os.system ('./reduceNtuple ' + filename + ' ' + opt.output + '/' + basename (filename))




