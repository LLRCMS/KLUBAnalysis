# def parseInputFileList (fileName) :
#     filelist = []
#     with open (fileName) as fIn:
#         for line in fIn:
#             line = (line.split("#")[0]).strip()
#             if line:
#                 filelist.append(line)
# return filelist

import argparse
import ROOT 
import os
import sys

parser = argparse.ArgumentParser(description='Command line parser of plotting options')
parser.add_argument('--filelist', dest='filelist', help='input file list', default=None)
args = parser.parse_args()

# zombies = []
# recover = []
corr = []
good = []
# totfiles = 0
infile = open(args.filelist)
for idx , line in enumerate(infile):
    line = (line.split("#")[0]).strip()
    if idx % 1000 == 0: print " >> file num" , idx
    if line:
        # totfiles += 1
        rfile = ROOT.TFile.Open(line)
        if not rfile:
            corr.append(line)
        elif rfile.IsZombie():
            corr.append(line)
        elif rfile.TestBit(ROOT.TFile.kRecovered):
            corr.append(line)
        else:
            good.append(line)
        
        # close only if it was opened
        if rfile:
            rfile.Close()
infile.close()

if len(corr) == 0:
    print "... all ok for", args.filelist
    sys.exit()

print "... ** I found" , len(corr) , "files (", 1.*len(corr)/(len(good)+len(corr)), ") corrupted, making new list"
newname = args.filelist + '_WITHCORRUPTED'

# print good
# print corr
# print newname

os.system('mv %s %s' % (args.filelist, newname))
fnew = open (args.filelist, 'w')
for l in good:
    fnew.write(l+'\n')
fnew.close()

