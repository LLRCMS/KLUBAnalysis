#! /usr/bin/env python
import sys, pwd, commands, optparse
import os
import re
import math
import string
from scipy.special import erf
from ROOT import *
import ROOT
import CombineHarvester.CombineTools.ch as ch
from configReader import *
from systReader import *


def parseOptions():

    usage = ('usage: %prog [options] datasetList\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)
    
    parser.add_option('-f', '--filename',   dest='filename',   type='string', default="",  help='input plots')
    parser.add_option('-o', '--dir', dest='outDir', type='string', default='', help='outdput dir')
    parser.add_option('-c', '--channel',   dest='channel', type='string', default='MuTau',  help='final state')
    #parser.add_option('-b', '--category',   dest='category', type='int', default='999',  help='btag category')
    parser.add_option('-i', '--config',   dest='config', type='string', default='',  help='config file')
    #parser.add_option('-o', '--selection', dest='overSel', type='string', default='', help='overwrite selection string')
    parser.add_option('-v', '--variable', dest='overVar', type='string', default='bH_mass', help='overwrite plot variable (only1D)')
    parser.add_option('-r', '--resonant',  action="store_true",  dest='isResonant', help='is Resonant analysis')
    parser.add_option('-y', '--binbybin',  action="store_true", dest='binbybin', help='add bin by bins systematics')

    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()

def  writeCard(input,theLambda) :
	out_dir = opt.outDir
	#in_dir = "/grid_mnt/vol__vol_U__u/llr/cms/ortona/diHiggs/CMSSW_7_4_7/src/KLUBAnalysis/combiner/cards_MuTauprova/HHSM2b0jMcutBDTMT2/";
	cmb1 = ch.CombineHarvester()
	cmb1.SetFlag('workspaces-use-clone', True)

	#read config
	categories = []
	variables =[]
	for icat in range(len(input.selections)) :
		categories.append((icat, input.selections[icat]))
	if opt.isResonant : variables.append('HHKin_mass_raw')
	else : variables.append('MT2')

	backgrounds=[]
	processes=[]
	for bkg in input.background:
		backgrounds.append(bkg)
		processes.append(bkg)
	processes.append(theLambda)

	#add processes to CH
	#masses->125 
	#analyses->Res/non-Res(HHKin_fit,MT2)
	#eras->13TeV 
	#channels->mutau/tautau/etau 
	#bin->bjet categories
	print signals, signals[0]
	cmb1.AddObservations(["125"], variables, ['13TeV'], [opt.channel], categories)
	cmb1.AddProcesses(["125"], variables, ['13TeV'], [opt.channel], backgrounds, categories, False)
	cmb1.AddProcesses(["125"], variables, ['13TeV'], [opt.channel], [theLambda], categories, True) #signals[0]

	#Add systematics (implement configReader!)
	systName = ""
	channels=[]
	systFile = open("../config/systematics.cfg")
	for line in systFile :
		f = line.split()
		if len(f) < 1: continue
		if f[0].startswith("#"): continue

		if f[0].startswith('['):
			f = re.split('\W+',line)
			systName = f[1]


	cmb1.cp().AddSyst(cmb1, "lumi_$ERA", "lnN", ch.SystMap()(1.013))
#	cb.cp().process(['WH', 'ZH']).AddSyst(
#		cb, "QCDscale_VH", "lnN", ch.SystMap('channel', 'era', 'bin_id')
#    (['mt'], ['7TeV', '8TeV'],  [1, 2],               1.010)
#    (['mt'], ['7TeV', '8TeV'],  [3, 4, 5, 6, 7],      1.040)
#    (['et'], ['7TeV'],          [1, 2, 3, 5, 6, 7],   1.040)
#    (['et'], ['8TeV'],          [1, 2],               1.010)
#    (['et'], ['8TeV'],          [3, 5, 6, 7],         1.040))

#	$BIN        --> proc.bin()
#	$PROCESS    --> proc.process()
#	$MASS       --> proc.mass()
#	$SYSTEMATIC --> syst.name()

	#cmb1.cp().PrintProcs()
	#cmb1.cp().signals().PrintProcs()
	cmb1.cp().backgrounds().ExtractShapes(
		opt.filename,
		"$PROCESS_$BIN_SR_{0}".format(variables[0]),
		"$PROCESS_$BIN_SR_{0}_$SYSTEMATIC".format(variables[0]))
	cmb1.cp().signals().ExtractShapes(
			opt.filename,
		"$PROCESS_$BIN_SR_{0}".format(variables[0]),
		"$PROCESS_$BIN_SR_{0}_$SYSTEMATIC".format(variables[0]))


	bbb = ch.BinByBinFactory()
	bbb.SetAddThreshold(0.1).SetMergeThreshold(0.5).SetFixNorm(True)
	bbb.MergeBinErrors(cmb1.cp().backgrounds())
	if opt.binbybin :bbb.AddBinByBin(cmb1.cp().backgrounds(), cmb1)
	
	cmb1.cp().PrintProcs()
	cmb1.WriteDatacard(opt.outDir+"/chCard{0}_{1}.txt".format(theLambda,opt.channel),opt.outDir+"/chCard{0}_{1}.input.root".format(theLambda,opt.channel))

ROOT.gSystem.AddIncludePath("-I$ROOFITSYS/include/")
ROOT.gSystem.AddIncludePath("-Iinclude/")
ROOT.gSystem.Load("libRooFit")
ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit.so")

parseOptions()
if(opt.config==""): configname = "../config/analysis_"+opt.channel+".cfg"
else: configname = opt.config
input = configReader(configname)
input.readInputs()

for theLambda in input.signals:
	writeCard(input,theLambda)
