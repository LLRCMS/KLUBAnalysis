import sys, pwd, commands, optparse
from ROOT import *

def parseOptions():

    usage = ('usage: %prog [options] datasetList\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)

    parser.add_option('-f', '--filename', dest='filename', type='string', default=""      , help='input plots')
    parser.add_option('-o', '--outfile' , dest='outName' , type='string', default="w"     , help='output suffix')
    parser.add_option('-c', '--channel' , dest='channel' , type='string', default="TauTau", help='channel')
    parser.add_option('-y', '--year'    , dest='year'    , type='string', default="2016"  , help='year')

    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()

parseOptions()

listHistos = []

systNamesOUT = ["CMS_scale_t_13TeV_"+opt.year+"_DM0","CMS_scale_t_13TeV_"+opt.year+"_DM1","CMS_scale_t_13TeV_"+opt.year+"_DM10","CMS_scale_t_13TeV_"+opt.year+"_DM11",
                "CMS_scale_es_13TeV_"+opt.year+"_DM0", "CMS_scale_es_13TeV_"+opt.year+"_DM1", "CMS_scale_mes_13TeV_"+opt.year+"", "CMS_scale_j_13TeV_"+opt.year+"",
                "CMS_JES_FlavQCD", "CMS_JES_RelBal", "CMS_JES_HF", "CMS_JES_BBEC1", "CMS_JES_EC2", "CMS_JES_Abs", "CMS_JES_BBEC1_"+opt.year, "CMS_JES_EC2_"+opt.year,
                "CMS_JES_Abs_"+opt.year, "CMS_JES_HF_"+opt.year, "CMS_JES_RelSample_"+opt.year]

systNames = ['tesXXX_DM0', 'tesXXX_DM1', 'tesXXX_DM10', 'tesXXX_DM11', 'eesXXX_DM0', 'eesXXX_DM1', 'mesXXX', 'jesXXXTot',
             'jesXXX1', 'jesXXX2', 'jesXXX3', 'jesXXX4', 'jesXXX5', 'jesXXX6', 'jesXXX7', 'jesXXX8', 'jesXXX9', 'jesXXX10', 'jesXXX11']

# if running WITHOUT the shape syst, uncomment these 2 lines:
#systNamesOUT = []
#systNames = []

print "Running"
inFile = TFile.Open(opt.filename)
ih = 0
toth = len (inFile.GetListOfKeys())

histos_syst_up = []
histos_syst_down = []

yieldFolder = "scales"+opt.year+"/"

ih = 0

for key in inFile.GetListOfKeys():
	ih = ih + 1
	if (ih%500 == 0): print key,ih,"/",toth
	kname = key.GetName()
	template = inFile.Get(kname)

	#protection against empty bins
	changedInt = False
	integral = template.Integral()
	for ibin in range(1,template.GetNbinsX()+1):
		if template.GetBinContent(ibin) <= 0:
			changedInt = True
			template.SetBinContent(ibin,0.000001)
			template.SetBinError(ibin,0.000001)

	if integral>0 and changedInt and template.Integral()>0: template.Scale(integral/template.Integral())

	if "TH1" in template.ClassName():
		for isyst in range(len(systNames)):
			lineToCheckDown = systNames[isyst].replace('XXX',"down").replace("tes","tau").replace("ees","ele").replace("mes","mu").replace("jes","jet")
			lineToCheckUp = systNames[isyst].replace('XXX',"up").replace("tes","tau").replace("ees","ele").replace("mes","mu").replace("jes","jet")
			found = -1

			if kname.endswith(lineToCheckDown):
				appString = "Down"
				remString = "down"
				found = 1
			elif kname.endswith(lineToCheckUp):
				appString = "Up"
				remString = "up"
				found = 0

			if found>=0:
				names = kname.split("_")

				for i in range(0,len(names)):
					#if (names[i].startswith('s') or names[i].startswith('VBFloose') or 'class' in names[i]):
					if ('resolved' in names[i] or 'boosted' in names[i] or 'class' in names[i] or 'VBFloose' in names[i]):
						for j in range(1, i):
							names[0] += "_"+str(names[j])
						#if not (names[i]== "singleT"): names[1] = names[i]
						names[1] = names[i]
						break

				proc = names[0]
				yieldName = yieldFolder+opt.channel+"_"+names[1]
				yieldName = yieldName+'_'+systNames[isyst].replace('XXX','').replace("jes","jes_")+".txt"
				infile = open(yieldName)
				scale = 1.000
				for line in infile:
					words = line.split()
					if words[0] == proc:
						#print "found ",words, proc,float(words[1+found])
						scale = float(words[1+found])
						break
				template.Scale(scale)
				#print proc,scale,yieldName

				kname = kname.replace("_"+systNames[isyst].replace('XXX',remString).replace("tes","tau").replace("ees","ele").replace("mes","mu").replace("jes","jet"),"")
				#print "toReplace:",systNames[isyst]+remString,"temporary:", newName
				kname = kname + "_"+ systNamesOUT[isyst] + appString

		# Fix the signal names after the systs have been read
		if "GGHH_NLO" in kname: kname = kname.replace("GGHH_NLO","ggHH").replace("_xs","_kt_1_hbbhtt").replace("cHHH", "kl_")
		if "VBFHH"    in kname: kname = kname.replace("VBFHH","qqHH").replace("C3","kl").replace("_xs","_hbbhtt")

		template.SetName(kname)
		template.SetTitle(kname)
		listHistos.append(template.Clone())

outFile = TFile.Open("analyzedOutPlotter_{0}_{1}_{2}.root".format(opt.year, opt.channel, opt.outName),"RECREATE")
outFile.cd()

for h in listHistos :
	h.Write()
for h in histos_syst_up:
	h.Write()
for h in histos_syst_down:
	h.Write()

outFile.Close()

