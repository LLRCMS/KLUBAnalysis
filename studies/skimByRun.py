#! /usr/bin/env python
import sys, pwd, commands, optparse
import os
import re
import math
from scipy.special import erf
from ROOT import *
import ROOT
from array import array

fileIn = TFile.Open("/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims25_AllJets_NI/SKIM_SingleMuon/total.root")
fileOut = TFile.Open("oldLumi.root","RECREATE")
tree = fileIn.Get("HTauTauTree")
#
tree2 = tree.CloneTree(0);

skipping=0
for event in tree:
	lumi = tree.lumi
	run = tree.RunNumber
	skip=False
	#print run
	silver = open("diff.txt","r")
	for line in silver:
		#print int(line)," ",run
		if int(line) == run :
			skip=True
			#print "TROVATO"
			#break
	if skip==False:
		tree2.Fill()
	else:
		skipping=skipping+1

print "skip ", skipping
fileOut.cd()
tree2.Write()
fileOut.Close()

