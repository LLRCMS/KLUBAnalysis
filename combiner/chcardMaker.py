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
    parser.add_option('-s', '--selection', dest='overSel', type='string', default='', help='overwrite selection string')
    #parser.add_option('-v', '--variable', dest='overVar', type='string', default='bH_mass', help='overwrite plot variable (only1D)')
    parser.add_option('-r', '--resonant',  action="store_true",  dest='isResonant', help='is Resonant analysis')
    parser.add_option('-y', '--binbybin',  action="store_true", dest='binbybin', help='add bin by bins systematics')

    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()

def hackTheCard(origName,destName) :
	#I need to do this for the rateParams
    infile = open(origName)
    print "opening",origName,"and sending to",destName
    file = open(destName, "wb")
    nbinSub = 0
    binName = ""
    for line in infile :
        outline = line
        f = line.split()
        if line.startswith('imax') :
            outline = "imax    4 number of bins\n"
        if line.startswith('shapes') and f[1]  == "ggHH_hbbhtt" :
            outline = line + "shapes * B{0} FAKE \n shapes * C{0} FAKE \n shapes * D{0} FAKE \n".format(self.theChannel)
        if line.startswith('bin') and nbinSub == 0 :
            outline = f[0]+" "+f[1]+" B{0} C{0} D{0} \n".format(self.theChannel)
            binName = f[1]
            nbinSub = 1
        if line.startswith('bin') and nbinSub == 1 :
            nProc = len(f)
            outline = ""
            for iword in range(len(f)-3):
                outline = outline + " " + f[iword]
            outline = outline + " B{0} C{0} D{0} \n".format(self.theChannel)
        if line.startswith("alpha") and "rateParam" in line :
            outline = "alpha rateParam {0} bkg_QCD (@0*@1/@2) beta,gamma,delta \n".format(binName)
        if line.startswith("beta" )and "rateParam" in line :
            outline = line.replace("*","B{0}".format(self.theChannel))
        if line.startswith("gamma") and "rateParam" in line :
            outline = line.replace("*","C{0}".format(self.theChannel))
        if line.startswith("delta") and "rateParam" in line :
            outline = line.replace("*","D{0}".format(self.theChannel))
        file.write(outline)

def  writeCard(input,theLambda,select,region=-1) :
	out_dir = opt.outDir
	#in_dir = "/grid_mnt/vol__vol_U__u/llr/cms/ortona/diHiggs/CMSSW_7_4_7/src/KLUBAnalysis/combiner/cards_MuTauprova/HHSM2b0jMcutBDTMT2/";
	cmb1 = ch.CombineHarvester()
	cmb1.SetFlag('workspaces-use-clone', True)

	#read config
	categories = []
	variables =[]
	#for icat in range(len(input.selections)) :
	#	categories.append((icat, input.selections[icat]))
	categories.append((0,select))
	if opt.isResonant : variables.append('HHKin_mass_raw')
	else : variables.append('MT2')

	backgrounds=[]
	processes=[]
	processes.append(theLambda)
	inRoot = TFile.Open(opt.filename)
	for bkg in input.background:
		#Add protection against empty processes => If I remove this I could build all bins at once instead of looping on the selections
		templateName = "{0}_{1}_SR_{2}".format(bkg,select,variables[0])
		template = inRoot.Get(templateName)
		if template.Integral()>0.000001 :
			backgrounds.append(bkg)
			processes.append(bkg)

	#add processes to CH
	#masses->125 
	#analyses->Res/non-Res(HHKin_fit,MT2)
	#eras->13TeV 
	#channels->mutau/tautau/etau 
	#bin->bjet categories
	#print signals, signals[0]
	cmb1.AddObservations(["125"], variables, ['13TeV'], [opt.channel], categories)
	cmb1.AddProcesses(["125"], variables, ['13TeV'], [opt.channel], backgrounds, categories, False)
	cmb1.AddProcesses(["125"], variables, ['13TeV'], [opt.channel], [theLambda], categories, True) #signals[0]

	#Add systematics (implement configReader!)
#	systName = ""
#	channels=[]
#	systFile = open("../config/systematics.cfg")
#	for line in systFile :
#		f = line.split()
#		if len(f) < 1: continue
#		if f[0].startswith("#"): continue
#
#		if f[0].startswith('['):
#			f = re.split('\W+',line)
#			systName = f[1]
#
	#print input.background
	cmb1.cp().AddSyst(cmb1, "lumi_$ERA", "lnN", ch.SystMap()(1.013))
	#now I can simply loop over the results of configReader
	#cmb1.cp().AddSyst(cmb1, "testSyst","lnN",ch.SystMap('channel','bin_id')([opt.channel],[backgrounds.index("TT")+1],1.999))
	cmb1.cp().process(['TT']).AddSyst(cmb1, "testSyst","lnN",ch.SystMap('channel','bin_id')([opt.channel],[0],1.999))
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
	regionName = ["","regB","regC","regD"]
	regionSuffix = ["SR","SStight","OSinviso","SSinviso"]
	outFile = opt.outDir+"/chCard{0}{2}_{1}_{3}.txt".format(theLambda,opt.channel,regionName[region+1],select)
	cmd = "mkdir -p {0}".format(opt.outDir)
	status, output = commands.getstatusoutput(cmd)   

	if region < 0 :
		cmb1.cp().backgrounds().ExtractShapes(
			opt.filename,
			"$PROCESS_$BIN_{1}_{0}".format(variables[0],regionSuffix[region+1]),
			"$PROCESS_$BIN_{1}_{0}_$SYSTEMATIC".format(variables[0],regionSuffix[region+1]))
		cmb1.cp().signals().ExtractShapes(
			opt.filename,
			"$PROCESS_$BIN_{1}_{0}".format(variables[0],regionSuffix[region+1]),
			"$PROCESS_$BIN_{1}_{0}_$SYSTEMATIC".format(variables[0],regionSuffix[region+1]))

		bbb = ch.BinByBinFactory()
		bbb.SetAddThreshold(0.1).SetMergeThreshold(0.5).SetFixNorm(True)
		bbb.MergeBinErrors(cmb1.cp().backgrounds())
		if opt.binbybin : bbb.AddBinByBin(cmb1.cp().backgrounds(), cmb1)
		cmb1.cp().PrintProcs().PrintSysts()

		outroot = TFile.Open(opt.outDir+"/chCard{0}{2}_{1}_{3}.input.root".format(theLambda,opt.channel,regionName[region+1],select),"RECREATE")
		cmb1.WriteDatacard(outFile,opt.outDir+"/chCard{0}{2}_{1}_{3}.input.root".format(theLambda,opt.channel,regionName[region+1],select))
		file = open( outFile, "wb")	
		file.write("alpha rateParam {0} QCD (@0*@1/@2) QCD_regB,QCD_regC,QCD_regD".format(select))
	else :
		file = open( outFile, "wb")
		file.write("imax 1\n")
		file.write("jmax {0}\n".format(len(backgrounds)-1))
		file.write("kmax *\n")

		file.write("------------\n")
		file.write("shapes * * FAKE\n".format(opt.channel,regionName[region+1]))
		file.write("------------\n")

		templateName = "data_obs_{1}_SR_{2}".format(bkg,select,variables[0])
		template = inRoot.Get(templateName)        
		file.write("bin {0} \n".format(select))
		file.write("observation {0} \n".format(template.GetEntries()))

		file.write("------------\n")

		file.write("bin ")        
		for chan in backgrounds:
			file.write("{0} ".format(select))
		file.write("\n")      

		file.write("process ")
		for chan in backgrounds:
			file.write("{0} ".format(chan))
		file.write("\n")

		file.write("process ")
		for chan in range(len(backgrounds)):
			file.write("{0} ".format(chan+1))
		file.write("\n")

		file.write("rate ")
		for chan in backgrounds:
			if "QCD" in chan :
				file.write("1.0 ")
			else :
				templateName = "{0}_{1}_SR_{2}".format(chan,select,variables[0])
				template = inRoot.Get(templateName)
				file.write("{0:.4f} ".format(template.Integral()))
		file.write("\n")
		file.write("------------\n")
		file.write("QCD_{0} rateParam  {1} QCD 1 \n".format(regionName[region+1],select))

ROOT.gSystem.AddIncludePath("-I$ROOFITSYS/include/")
ROOT.gSystem.AddIncludePath("-Iinclude/")
ROOT.gSystem.Load("libRooFit")
ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit.so")

parseOptions()
if(opt.config==""): configname = "../config/analysis_"+opt.channel+".cfg"
else: configname = opt.config
input = configReader(configname)
input.readInputs()

if opt.overSel == "" :
	allSel = ["s1b1jresolvedMcut", "s2b0jresolvedMcut", "sboostedLLMcut"]
else : allSel = [opt.overSel]

for theLambda in input.signals:
	for sel in allSel : 
		if not "lambda" in theLambda and not "Radion" in theLambda : continue
		for ireg in range(-1,3) :
			writeCard(input,theLambda,sel,ireg)
