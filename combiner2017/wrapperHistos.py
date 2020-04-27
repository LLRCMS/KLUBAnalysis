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
    parser.add_option('-a', '--analysis',   dest='analysis',   type='string', default="GGF",  help='channel')

    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()


listHistos = []
systNamesOUT=["CMS_scale_t_13TeV","CMS_scale_j_13TeV"]
systNames=["tau","jet"]
yieldFolder="/home/llr/cms/amendola/HH2017/CMSSW_9_0_0/src/KLUBAnalysis/studies/GetScaleYieldSyst"
parseOptions()
print "Running"
inFile = TFile.Open(opt.filename)
ih = 0
toth = len (inFile.GetListOfKeys())

histos_syst_up = []
histos_syst_down = []

selnames = ["s1b1j", "s2b0j", "sboost", "VBFloose", "VBFtight"]

if opt.analysis == "GGF":
    procnames = [ "TT", "WJets", "TW","EWK", "WW", "WW", "WZ", "ZH","other",  "ZZ", "DY0b","DY1b", "DY2b","lambdarew"]
elif opt.analysis == "VBF":
    procnames = [ "TT", "WJets", "TW","EWK", "WW", "WW", "WZ", "ZH","other",  "ZZ", "DY0b","DY1b", "DY2b","GGF_xs", "VBFHH_CV_"]


#only for kL scan
#for sel in selnames:
#    for proc in procnames:
#        template_syst = []
#        ih = 0
#        for key in inFile.GetListOfKeys() :
#        	ih = ih + 1
#        	if (ih%100 == 0) : print key,ih,"/",toth
#        	kname = key.GetName()
#                if "BDToutSM" in kname and sel in kname and proc in kname:
#                    if opt.analysis == "GGF":
#                        template_syst.append(inFile.Get(kname))
#        
#        for i, k in enumerate(template_syst):
#
#            histo_syst_up = k.Clone()
#            histo_syst_down = k.Clone()
#            histo_syst_up.SetName(k.GetName().replace("lambdarew","ggHH_bbtt")+"_shapeUp")
#            histo_syst_up.SetTitle(k.GetTitle().replace("lambdarew","ggHH_bbtt")+"_shapeUp")
#            histo_syst_down.SetName(k.GetName().replace("lambdarew","ggHH_bbtt")+"_shapeDown")
#            histo_syst_down.SetTitle(k.GetTitle().replace("lambdarew","ggHH_bbtt")+"_shapeDown")
#        
#            for bin in range(0, k.GetNbinsX()+1):
#                maxbin = max([histo.GetBinContent(bin) for histo  in template_syst])
#           
#                minbin = min([histo.GetBinContent(bin) for histo  in template_syst])
#           
#                if minbin < 0: minbin = 0.0001
#                if maxbin < 0: maxbin = 0.0001
#                histo_syst_up.SetBinContent(bin,maxbin)
#                histo_syst_down.SetBinContent(bin,minbin)
#
#            if histo_syst_up.Integral() > 0.0001:
#                histo_syst_up.Scale(k.Integral()/histo_syst_up.Integral())
#            else:
#                histo_syst_up = k.Clone()
#                histo_syst_up.SetName(k.GetName().replace("lambdarew","ggHH_bbtt")+"_shapeUp")
#                histo_syst_up.SetTitle(k.GetTitle().replace("lambdarew","ggHH_bbtt")+"_shapeUp")
#
#            if histo_syst_down.Integral() > 0.0001:
#                histo_syst_down.Scale(k.Integral()/histo_syst_down.Integral())
#            else:
#                histo_syst_down = k.Clone()
#                histo_syst_down.SetName(k.GetName().replace("lambdarew","ggHH_bbtt")+"_shapeDown")
#                histo_syst_down.SetTitle(k.GetTitle().replace("lambdarew","ggHH_bbtt")+"_shapeDown")
#
#
#             
#            histos_syst_up.append(histo_syst_up)
#            histos_syst_down.append(histo_syst_down)

ih = 0


for key in inFile.GetListOfKeys() :
	ih = ih + 1
	if (ih%100 == 0) : print key,ih,"/",toth
	kname = key.GetName()
	template = inFile.Get(kname)
	newName = kname#.replace("lambdarew","ggHH_bbtt")
	if "rew" in newName:
		if "bidim" in newName :
			newName = kname.replace("bidimrew","ggHH_bbtt")
		else:
			newName = kname.replace("lambdarew","ggHH_bbtt")
                        
        if "VBFHH" in newName:

            newName = kname.replace("VBFHH","VBFHH_bbtt")

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

                                for i in range(0,len(names)):
                                    #print names[i]
                                    if (names[i].startswith('s') or names[i].startswith('VBFloose') or names[i].startswith('VBFtight')):
					for j in range(1, i):
                                            names[0] += "_"+str(names[j])

                                        names[1] = names[i]
                                        break
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
for h in histos_syst_up:
	h.Write()

for h in histos_syst_down:
	h.Write()


outFile.Close()

