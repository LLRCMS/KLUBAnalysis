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

#systNamesOUT=["CMS_scale_t_13TeV_DM0","CMS_scale_t_13TeV_DM1","CMS_scale_t_13TeV_DM10","CMS_scale_t_13TeV_DM11", "CMS_scale_es_13TeV_DM0", "CMS_scale_es_13TeV_DM1", "CMS_scale_mes_13TeV", "CMS_scale_j_13TeV"]
systNamesOUT=["CMS_scale_t_13TeV_"+opt.year+"_DM0","CMS_scale_t_13TeV_"+opt.year+"_DM1","CMS_scale_t_13TeV_"+opt.year+"_DM10","CMS_scale_t_13TeV_"+opt.year+"_DM11", "CMS_scale_es_13TeV_"+opt.year+"_DM0", "CMS_scale_es_13TeV_"+opt.year+"_DM1", "CMS_scale_mes_13TeV_"+opt.year+"", "CMS_scale_j_13TeV_"+opt.year+""]
systNames = ['tesXXX_DM0','tesXXX_DM1','tesXXX_DM10','tesXXX_DM11','eesXXX_DM0','eesXXX_DM1','mesXXX', 'jesXXXTot']

# if running WITHOUT the shape syst, uncomment uncomment these 2 lines:
#systNamesOUT=[]
#systNames = []

print "Running"
inFile = TFile.Open(opt.filename)
ih = 0
toth = len (inFile.GetListOfKeys())

histos_syst_up = []
histos_syst_down = []

yieldFolder = "scales"+opt.year+"/"

procnames = ['TT', 'W', 'EWK', 'singleT', 'TW', 'ZH', 'WH', 'VV', 'ttH', 'TTX', 'ggH', 'VBFH', 'VVV', 'DY_0b_1Pt', 'DY_0b_2Pt', 'DY_0b_3Pt', 'DY_0b_4Pt', 'DY_0b_5Pt', 'DY_0b_6Pt', 'DY_1b_1Pt', 'DY_1b_2Pt', 'DY_1b_3Pt', 'DY_1b_4Pt', 'DY_1b_5Pt', 'DY_1b_6Pt', 'DY_2b_1Pt', 'DY_2b_2Pt', 'DY_2b_3Pt', 'DY_2b_4Pt', 'DY_2b_5Pt', 'DY_2b_6Pt', 'DY_LM', 'GGHH_NLO_cHHH1_xs', 'GGHH_NLO_cHHH2p45_xs', 'GGHH_NLO_cHHH5_xs', 'VBFHH_CV_1_C2V_1_C3_1_xs' , 'VBFHH_CV_0p5_C2V_1_C3_1_xs', 'VBFHH_CV_1p5_C2V_1_C3_1_xs', 'VBFHH_CV_1_C2V_1_C3_0_xs', 'VBFHH_CV_1_C2V_1_C3_2_xs', 'VBFHH_CV_1_C2V_2_C3_1_xs']

ih = 0

for key in inFile.GetListOfKeys():
	ih = ih + 1
	if (ih%100 == 0): print key,ih,"/",toth
	kname = key.GetName()
	template = inFile.Get(kname)

	if "GGHH_NLO" in kname: kname = kname.replace("GGHH_NLO","ggHH").replace("_xs","_kt_1_hbbhtautau").replace("cHHH", "kl_")
	if "VBFHH"    in kname: kname = kname.replace("VBFHH","qqHH").replace("C3","kl").replace("_xs","_hbbhtautau") #write 1_5 as 1p5 from the beginning

	print kname

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
			print '---->', lineToCheckDown
			if kname.endswith(lineToCheckDown):
				appString = "Down"
				remString = "down"
				found = 1
			elif kname.endswith(lineToCheckUp):
				appString = "Up"
				remString = "up"
				found =0
			if found>=0:
				names = kname.split("_")
				print names
				for i in range(0,len(names)):
					print names[i]
					if (names[i].startswith('s') or names[i].startswith('VBFloose')):
						for j in range(1, i):
							names[0] += "_"+str(names[j])
						if not (names[i]== "singleT"): names[1] = names[i]
						break
				proc = names[0]
				#print names
				yieldName=yieldFolder+"/"+opt.channel+"_"+names[1]
				yieldName =yieldName+'_'+systNames[isyst].replace('XXX','').replace("jesTot","jes_Tot")+".txt"
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
				#if abs(1-scale)>0.02 : print "correct",yieldName, proc,scale
				kname = kname.replace("_"+systNames[isyst].replace('XXX',remString).replace("tes","tau").replace("ees","ele").replace("mes","mu").replace("jes","jet"),"")
				#print "toReplace:",systNames[isyst]+remString,"temporary:", newName
				kname = kname + "_"+ systNamesOUT[isyst] + appString
				print kname
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

