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
#from systReader import *


def parseOptions():

    usage = ('usage: %prog [options] datasetList\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)
    
    parser.add_option('-f', '--filename',   dest='filename',   type='string', default="",  help='input plots')
    parser.add_option('-o', '--dir', dest='outDir', type='string', default='', help='outdput dir')
    parser.add_option('-c', '--channel',   dest='channel', type='string', default='TauTau',  help='final state')
    parser.add_option('-i', '--config',   dest='config', type='string', default='',  help='config file')
    parser.add_option('-s', '--selection', dest='overSel', type='string', default='', help='overwrite selection string')
    parser.add_option('-l', '--lambda', dest='overLambda', type='string', default='', help='use only this signal sample')
    parser.add_option('-r', '--resonant',  action="store_true",  dest='isResonant', help='is Resonant analysis')
    parser.add_option('-y', '--binbybin',  action="store_true", dest='binbybin', help='add bin by bins systematics')
    parser.add_option('-t', '--theory',  action="store_true", dest='theory', help='add theory systematics')
    parser.add_option('-u', '--shape', dest='shapeUnc', type='int', default=1, help='1:add 0:disable shape uncertainties')

    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()
    global lambdaName

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

def  writeCard(input,theLambda,select,kLambda,region=-1) :
	print "writing cards"
	variables =[]
	if opt.isResonant : variables.append('HHKin_mass_raw')
	else : variables.append('BDToutSM_kl_'+kLambda)

	#out_dir = opt.outDir
	theOutputDir = "{0}{1}{2}".format(theLambda,select,variables[0])
	dname = "_"+opt.channel+opt.outDir
	out_dir = "cards{1}/{0}/".format(theOutputDir,dname)
	print "out_dir = ", out_dir
	cmb1 = ch.CombineHarvester()
	cmb1.SetFlag('workspaces-use-clone', True)

	cmd = "mkdir -p {0}".format(out_dir)
	print cmd
	regionName = ["","regB","regC","regD"]
	regionSuffix = ["SR","SStight","OSinviso","SSinviso"]
	status, output = commands.getstatusoutput(cmd)   
	#outFile = opt.outDir+"/chCard{0}{2}_{1}_{3}.txt".format(theLambda,opt.channel,regionName[region+1],select)
	thechannel = "1"
	if opt.channel == "MuTau" : thechannel="2"
	elif opt.channel == "TauTau" : thechannel = "3"

	if "0b0j" in select : theCat = "0"
	if "2b0j" in select : theCat = "2"
	elif "1b1j" in select : theCat = "1"
	elif "boosted" in select : theCat = "3"
	elif "VBFloose" in select : theCat = "4"
	elif "VBFtight" in select : theCat = "5"

	outFile = "hh_{0}_C{1}_L{2}_13TeV.txt".format(thechannel,theCat,theLambda)
	file = open( "temp.txt", "wb")


	#read config
	categories = []
	categories.append((0,select))
	backgrounds=[]
	MCbackgrounds=[]
	processes=[]
	processes.append(lambdaName)
	inRoot = TFile.Open(opt.filename)
	for bkg in input.background:
		#Add protection against empty processes => If I remove this I could build all bins at once instead of looping on the selections
		templateName = "{0}_{1}_SR_{2}".format(bkg,select,variables[0])
		print templateName
		template = inRoot.Get(templateName)
		if template.Integral()>0.000001 :
			backgrounds.append(bkg)
			processes.append(bkg)
			if bkg is not "QCD" :
				MCbackgrounds.append(bkg)

	#print backgrounds
	allQCD = False
	allQCDs = [0,0,0,0]
	for regionsuff in range(len(regionSuffix)) :
		for ichan in range(len(backgrounds)):
			if "QCD" in backgrounds[ichan] :
				fname = "data_obs"
				if regionSuffix[regionsuff] == "SR" :
					fname="QCD"
				templateName = "{0}_{1}_{3}_{2}".format(fname,select,variables[0],regionSuffix[regionsuff])
				template = inRoot.Get(templateName)
				#allQCDs.append(template.Integral())
				allQCDs[regionsuff]= allQCDs[regionsuff]+template.Integral()
				iQCD = ichan
			elif regionSuffix[regionsuff] is not "SR" :
				templateName = "{0}_{1}_{3}_{2}".format(backgrounds[ichan],select,variables[0],regionSuffix[regionsuff])
				template = inRoot.Get(templateName)
				allQCDs[regionsuff] = allQCDs[regionsuff] - template.Integral()

	if allQCDs[0]>0 and allQCDs[1]>0 and allQCDs[2]>0 and allQCDs[3]>0 : allQCD = True
	for i in range(4) : print allQCDs[i]

	cmb1.AddObservations([theLambda.replace(lambdaName,"")], variables, ['13TeV'], [opt.channel], categories)
	cmb1.AddProcesses([theLambda.replace(lambdaName,"")], variables, ['13TeV'], [opt.channel], backgrounds, categories, False)
	cmb1.AddProcesses([theLambda.replace(lambdaName,"")], variables, ['13TeV'], [opt.channel], [lambdaName], categories, True) #signals[0]

	if region < 0 :

		#Systematics (I need to add by hand the shape ones)
		syst = systReader("../config/systematics.cfg",[lambdaName],backgrounds,file)
		syst.writeOutput(False)
		syst.verbose(True)
		if(opt.channel == "TauTau" ): 
			syst.addSystFile("../config/systematics_tautau.cfg")
                        if ("tight" in select):
                            syst.addSystFile("../config/systematics_VBFtight.cfg")
		elif(opt.channel == "MuTau" ): 
			syst.addSystFile("../config/systematics_mutau.cfg")
                elif(opt.channel == "ETau" ): 
			syst.addSystFile("../config/systematics_etau.cfg")
		if opt.theory : syst.addSystFile("../config/syst_th.cfg")
		syst.writeSystematics()
                
		for isy in range(len(syst.SystNames)) :
			if "CMS_scale_t" in syst.SystNames[isy] or "CMS_scale_j" in syst.SystNames[isy]: continue
			for iproc in range(len(syst.SystProcesses[isy])) :
				if "/" in syst.SystValues[isy][iproc] :
					f = syst.SystValues[isy][iproc].split("/")
					systVal = (float(f[0]),float(f[1]))
				else :
					systVal = float(syst.SystValues[isy][iproc])
				print isy, iproc, systVal
                                
				print "adding Syst",systVal,syst.SystNames[isy],syst.SystTypes[isy],"to",syst.SystProcesses[isy][iproc]
				cmb1.cp().process([syst.SystProcesses[isy][iproc]]).AddSyst(cmb1, syst.SystNames[isy],syst.SystTypes[isy],ch.SystMap('channel','bin_id')([opt.channel],[0],systVal))
		if opt.shapeUnc > 0:
			jesproc = MCbackgrounds
			jesproc.append(lambdaName)
                        jesproc.remove("WW")
                        jesproc.remove("DY0b")
                        jesproc.remove("EWK")
                        jesproc.remove("TW")
                        if "boost" in select:
                            jesproc.remove("DY1b")
                            jesproc.remove("DY2b")
                            jesproc.remove("WZ")
                            if opt.channel== "TauTau": jesproc.remove("WJets")
                            
                        if "VBF" in select:
                            if opt.channel== "TauTau": jesproc.remove("WZ")
                        cmb1.cp().process(jesproc).AddSyst(cmb1, "CMS_scale_j_13TeV","shape",ch.SystMap('channel','bin_id')([opt.channel],[0],1.000))
			cmb1.cp().process(jesproc).AddSyst(cmb1, "CMS_scale_t_13TeV","shape",ch.SystMap('channel','bin_id')([opt.channel],[0],1.000))
                        #cmb1.cp().process(jesproc).AddSyst(cmb1, "shape","shape",ch.SystMap('channel','bin_id')([opt.channel],[0],1.000))
			cmb1.cp().process(["TT"]).AddSyst(cmb1, "top","shape",ch.SystMap('channel','bin_id')([opt.channel],[0],1.000))

	    #	$BIN        --> proc.bin()
	    #	$PROCESS    --> proc.process()
	    #	$MASS       --> proc.mass()
	    #	$SYSTEMATIC --> syst.name()


		cmb1.cp().backgrounds().ExtractShapes(
			opt.filename,
			"$PROCESS_$BIN_{1}_{0}".format(variables[0],regionSuffix[region+1]),
                        "$PROCESS_$BIN_{1}_{0}_$SYSTEMATIC".format(variables[0],regionSuffix[region+1])) 
		cmb1.cp().signals().ExtractShapes(
			opt.filename,
			"$PROCESS$MASS_$BIN_{1}_{0}".format(variables[0],regionSuffix[region+1]),
			"$PROCESS$MASS_$BIN_{1}_{0}_$SYSTEMATIC".format(variables[0],regionSuffix[region+1])) 

		bbb = ch.BinByBinFactory()
		bbb.SetAddThreshold(0.1).SetMergeThreshold(0.5).SetFixNorm(True)
		bbbQCD = ch.BinByBinFactory()
		bbbQCD.SetAddThreshold(0.0).SetMergeThreshold(0.5).SetFixNorm(True)
		if opt.binbybin : 
			bbb.MergeBinErrors(cmb1.cp().process(MCbackgrounds))
			bbbQCD.MergeBinErrors(cmb1.cp().process(["QCD"]))
			bbbQCD.AddBinByBin(cmb1.cp().process(["QCD"]), cmb1)
			bbb.AddBinByBin(cmb1.cp().process(MCbackgrounds), cmb1)
		#cmb1.cp().PrintProcs().PrintSysts()


		outroot = TFile.Open(out_dir+"hh_{0}_C{1}_L{2}_13TeV.input.root".format(thechannel,theCat,theLambda),"RECREATE")
                cmb1.SetGroup("theory", ["QCDscale_ggHH","pdf_ggHH","HH_BR_Hbb","HH_BR_Htt"])
                
                cmb1.WriteDatacard(out_dir+outFile,outroot)

		if allQCD :
			file = open( out_dir+outFile, "a")	
			file.write("alpha rateParam {0} QCD (@0*@1/@2) QCD_regB,QCD_regC,QCD_regD".format(select))
	elif allQCD :
		print thechannel,theCat,theLambda #,regionName2[region+1]
		outFile = "hh_{0}_C{1}_L{2}_13TeV.txt".format(thechannel,theCat,theLambda)
		#print region, allQCD
		#print regionName2[region+1]
		#print outFile
		#print "hh_"+thechannel#+"_C"+theCat+"_L"+theLambda+"_13TeV_"+regionName[region+1]+".txt"
		#print "hh_"+thechannel+"_C"+theCat#+"_L"+theLambda+"_13TeV_"+regionName[region+1]+".txt"
		#print "hh_"+thechannel+"_C"+theCat+"_L"+theLambda#+"_13TeV_"+regionName[region+1]+".txt"
		#print "hh_"+thechannel+"_C"+theCat+"_L"+theLambda+"_13TeV_"#+regionName[region+1]+".txt"
		#print outFile
		outFile = "hh_{0}_C{1}_L{2}_13TeV_{3}.txt".format(thechannel,theCat,theLambda,regionName[region+1])
		file = open( out_dir+outFile, "wb")

		file.write("imax 1\n")
		file.write("jmax {0}\n".format(len(backgrounds)-1))
		file.write("kmax *\n")

		file.write("------------\n")
		file.write("shapes * * FAKE\n".format(opt.channel,regionName[region+1]))
		file.write("------------\n")

		templateName = "data_obs_{1}_{3}_{2}".format(bkg,select,variables[0],regionSuffix[region+1])
		template = inRoot.Get(templateName)        
		file.write("bin {0} \n".format(select))
		obs = template.GetEntries()
		file.write("observation {0} \n".format(obs))

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
		for chan in range(len(backgrounds)): #+1 for the QCD
			file.write("{0} ".format(chan+1))
		file.write("\n")

		file.write("rate ")
		rates = []
		iQCD = -1
		totRate = 0
		for ichan in range(len(backgrounds)):
			if "QCD" in backgrounds[ichan] :
				rates.append(-1)
				iQCD = ichan
			else :
				templateName = "{0}_{1}_{3}_{2}".format(backgrounds[ichan],select,variables[0],regionSuffix[region+1])
				template = inRoot.Get(templateName)
				#print templateName
				brate = template.Integral()
				rates.append(brate)
				totRate = totRate + brate
		if iQCD >= 0 : rates[iQCD] = TMath.Max(0.0000001,obs-totRate)
		for ichan in range(len(backgrounds)):
			file.write("{0:.4f} ".format(rates[ichan]))
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
print configname
input = configReader(configname)
input.readInputs()

print input

lambdaName="ggHH_bbtt"

if opt.overSel == "" :
	allSel = ["s1b1jresolvedMcut", "s2b0jresolvedMcut", "sboostedLLMcut", "VBFloose", "VBFtight"]
else : allSel = [opt.overSel]

if not opt.overLambda == "" :
	input.signals = [opt.overLambda]

print input.signals
for il in range(len(input.signals)) :
	input.signals[il] = input.signals[il].replace("lambdarew","ggHH_bbtt")	
	input.signals[il] = input.signals[il].replace("bidimrew","ggHH_bbtt")	

#kLambdas=[-20, -15, -10, -8, -6, -4, -3, -2, -1, 0.001, 1, 2,  3, 2.45,  4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20, 25, 30]
kLambdas=[1]

for (theLambda,kLambda) in zip(input.signals, kLambdas):
	if not lambdaName in theLambda : 
		continue
        for sel in allSel : 
	    for ireg in range(-1,3) :
                        print "theLambda = ",theLambda
			writeCard(input,theLambda,sel,str(kLambda),ireg)
