import subprocess
import re
import argparse
import sys


finstates = ['1', '2', '3'] ##etau, mutau, tautau
categs    = ['1', '2', '3'] ##1b1j, 2b0j, boosted

# folder = '/home/llr/cms/ortona/diHiggs/CMSSW_7_4_7/src/KLUBAnalysis/combiner/cards_Combined_2017_02_28_unblind_hmr90/Radion300HHKin_mass_raw/'
# folder = '/home/llr/cms/ortona/diHiggs/CMSSW_7_4_7/src/KLUBAnalysis/combiner/cards_Combined_2017_02_28_unblind_lmr70/ggHH_bbtt21MT2'

parser = argparse.ArgumentParser(description='Command line parser of plotting options')
#string opts
parser.add_argument('--dir', dest='dir', help='directory name', default=None)
args = parser.parse_args()
if not args.dir:
    print " need directory : --dir <name>"
    sys.exit()

folder = args.dir

###############

sigtype = "Radion" if "Radion" in folder else "ggHH_bbtt"

command = 'cat %s/comb.txt | grep "shapes %s"' % (folder, sigtype)
output = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True).communicate()[0]
lines = output.splitlines()

for l in lines:
    print l
print ""

if len(lines) != 9:
    print "** error: did not find 9 categories as expected"
    sys.exit()

### make the dict
dd = {}
for l in lines:
    tokens = l.split()
    chcomb = tokens[2].replace('ch', '')
    partname = tokens[3]    
    #hh_3_C1_LggHH_bbtt21_13TeV.input.root, and ch, categ are never beyond 3 --> 1 digit
    channel  = partname[3]
    category = partname[6]
    print chcomb, '<--', channel, category
    dd[(channel, category)] = chcomb

print ""
print "ETau : 1, MuTau : 2, TauTau : 3"
print "1b1j : 1, 2b0j : 2, boosted : 3"
print ""

### now print
print "chtransl = {"
for fs in finstates:
    for cat in categs:
        print "   (%s, %s) : %s," % (fs, cat, dd[(fs, cat)])
print '}'
