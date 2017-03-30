import sys, pwd, commands, optparse
from ROOT import *

def parseOptions():

    usage = ('usage: %prog [options] datasetList\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)
    
    parser.add_option('-f', '--filename',   dest='filename',   type='string', default="",  help='input plots')
    parser.add_option('-t', '--twoDalso',  action="store_true", dest='twoD', help='store also 2D histos')
    parser.add_option('-o', '--outfile',   dest='outName',   type='string', default="w",  help='output suffix')
    parser.add_option('-c', '--channel',   dest='channel',   type='string', default="TauTau",  help='channel')

    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()


listHistos = []
systNamesOUT=["CMS_scale_t_13TeV","CMS_scale_j_13TeV"]
systNames=["tau","jet"]
yieldFolder="/home/llr/cms/cadamuro/testAnalysisHelper2/CMSSW_7_4_7/src/KLUBAnalysis/studies/GetScaleYieldSyst"
parseOptions()
print "Running"
inFile = TFile.Open(opt.filename)
ih = 0
toth = len (inFile.GetListOfKeys())
for key in inFile.GetListOfKeys() :
	ih = ih + 1
	if (ih%20 == 0) : print key,ih,"/",toth
	kname = key.GetName()
	template = inFile.Get(kname)
	newName = kname#.replace("lambdarew","ggHH_bbtt")
	if "rew" in newName:
		if "bidim" in newName :
			newName = kname.replace("bidimrew","ggHH_bbtt")
		else:
			newName = kname.replace("lambdarew","ggHH_bbtt")
	#	template.Scale(0.01)
	#if "Radion" in newName in newName:
	#	template.Scale(0.1)
	#protection against empty bins
	changedInt = False
	for ibin in range(1,template.GetNbinsX()+1) :
		integral = template.Integral()
		if template.GetBinContent(ibin) <= 0 :
			changedInt = True
			template.SetBinContent(ibin,0.000001)
			template.SetBinError(ibin,0.000001)
	#print key,integral,template.Integral()
	if template.Integral()>0 and changedInt : template.Scale(integral/template.Integral())
	if "TH1" in template.ClassName() or opt.twoD : 
		for isyst in range(len(systNames)):
			lineToCheckDown = systNames[isyst]+"down"
			lineToCheckUp = systNames[isyst]+"up"
			found = -1
			if lineToCheckDown in kname :
				appString = "Down"
				remString = "down"
				found = 1
			elif lineToCheckUp in kname:
				appString = "Up"
				remString = "up"
				found =0
			if found>=0 :
				names = kname.split("_")
				if not names[1].startswith('s') :
					names[0] = names[0]+"_"+names[1]
					names.remove(names[1]) 
				proc = names[0]
				if "bidimrew" in proc :
					proc = "lambdarew21"
				#print names
				yieldName=yieldFolder+"/"+opt.channel+"_"+names[1]
				if isyst == 0 :
					yieldName = yieldName+"_tes.txt"
				else :
					yieldName = yieldName+"_jes.txt"
				infile = open(yieldName)
				scale = 1.000
				for line in infile :
					words = line.split()
					if words[0] == proc :
						#print "found ",words, proc,float(words[1+found])
						scale = float(words[1+found])
						break
				template.Scale(scale)
				#print proc,scale,yieldName
				#if abs(1-scale)>0.02 : print "correct",yieldName, proc,scale
				newName = newName.replace("_"+systNames[isyst]+remString,"")
				#print "toReplace:",systNames[isyst]+remString,"temporary:", newName
				newName = newName + "_"+ systNamesOUT[isyst] + appString
				#print kname,"transformed to",newName
		template.SetName(newName)
		template.SetTitle(newName)
		#if "QCD" in kname :
		listHistos.append(template.Clone())

outFile = TFile.Open("analyzedOutPlotter_{0}.root".format(opt.outName),"RECREATE")
outFile.cd()
for h in listHistos :
	h.Write()
outFile.Close()

