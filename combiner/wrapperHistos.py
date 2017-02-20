import sys, pwd, commands, optparse
from ROOT import *

def parseOptions():

    usage = ('usage: %prog [options] datasetList\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)
    
    parser.add_option('-f', '--filename',   dest='filename',   type='string', default="",  help='input plots')
    parser.add_option('-t', '--twoDalso',  action="store_true", dest='twoD', help='store also 2D histos')
    parser.add_option('-o', '--outfile',   dest='outName',   type='string', default="w",  help='output suffix')

    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()


listHistos = []
systNamesOUT=["CMS_scale_t","CMS_JES"]
systNames=["tau","jet"]
parseOptions()
inFile = TFile.Open(opt.filename)
for key in inFile.GetListOfKeys() :
	kname = key.GetName()
	template = inFile.Get(kname)
	#protection against empty bins
	for ibin in range(1,template.GetNbinsX()+1) :
		integral = template.Integral()
		if template.GetBinContent(ibin) <= 0 :
			template.SetBinContent(ibin,0.000001)
			template.SetBinError(ibin,0.000001)
	template.Scale(integral/template.Integral())
	if "TH1" in template.ClassName() or opt.twoD : 
		for isyst in range(len(systNames)):
			lineToCheckDown = systNames[isyst]+"down"
			lineToCheckUp = systNames[isyst]+"up"
			found = -1
			if lineToCheckDown in kname :
				appString = "Down"
				remString = "down"
				found = 0
			elif lineToCheckUp in kname:
				appString = "Up"
				remString = "up"
				found =1
			if found>=0 :
				newName = kname.replace("_"+systNames[isyst]+remString,"")
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

