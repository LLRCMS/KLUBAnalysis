# coding: utf-8

_all_ = [ ]

import sys
import argparse
from ROOT import *

def parseOptions():
    usage = 'usage: %prog [options] datasetList (-h for help)'
    parser = argparse.ArgumentParser(description=usage)

    parser.add_option('-f', '--filename', default='', help='input plots')
    parser.add_option('-o', '--outname', default='w'     , help='output suffix')
    parser.add_option('-c', '--channel', default='TauTau', help='channel')
    parser.add_argument('-y', '--period', default='UL18', choices=('UL16', 'UL17', 'UL18'),
                        help='data period')
    parser.add_option('-q', '--dynamQCD', default=0, type=int,
                      help='1:do QCD as rateParam / 0:read QCD from file')
    parser.add_option('-B', '--renameH', type=int, default=1,
                      help='1:rename singleH bkgs / 0:do not rename singleH bkgs')
    parser.add_argument('--binOptimization', action='store_true',
                        help='disable removal of negative bins before doing binning optimization')

    return parser.parse_args()

opt = parseOptions()

listHistos = []

systsNamesOUT = [
    # hadronic tau energy scale for decay mode 0
    'CMS_scale_t_DM0_'    + opt.period,
    # hadronic tau energy scale for decay mode 1
    'CMS_scale_t_DM1_'    + opt.period,
    # hadronic tau energy scale for decay mode 10
    'CMS_scale_t_DM10_'   + opt.period,
    # hadronic tau energy scale for decay mode 11
    'CMS_scale_t_DM11_'   + opt.period,

    # energy scale of electrons reconstructed as hadronic taus for decay mode 0
    'CMS_scale_t_eFake_'  + opt.period + '_DM0',
    # energy scale of electrons reconstructed as hadronic taus for decay mode 1
    'CMS_scale_t_eFake_'  + opt.period + '_DM1',
    # energy scale of muons reconstructed as hadronic taus
    'CMS_scale_t_muFake_' + opt.period,

    # jet energy scale
    'CMS_JES_Abs',
    'CMS_JES_Abs_'       + opt.period,
    'CMS_JES_BBEC1',
    'CMS_JES_BBEC1_'     + opt.period,
    'CMS_JES_EC2',
    'CMS_JES_EC2_'       + opt.period,
    'CMS_JES_FlavQCD', 
    'CMS_JES_HF',
    'CMS_JES_HF_'        + opt.period,             
    'CMS_JES_RelBal', 
    'CMS_JES_RelSample_' + opt.period,

    # # jets faking taus
    # 'CMS_bbtt_' + opt.period + 'jetTauFakes_Barrel',
    # 'CMS_bbtt_' + opt.period + 'jetTauFakes_Endcap',

    # # jet energy resolution
    # 'CMS_res_j_' + opt.period,

    # # b tagging scale factors
    # 'CMS_btag_LF_2016_2017_2018',
    # 'CMS_btag_HF_2016_2017_2018',
    # 'CMS_btag_cferr1_2016_2017_2018',
    # 'CMS_btag_cferr2_2016_2017_2018',
    # 'CMS_btag_hfstats1_2016_2017_2018',
    # 'CMS_btag_hfstats2_2016_2017_2018',
    # 'CMS_btag_lfstats1_2016_2017_2018',
    # 'CMS_btag_lfstats2_2016_2017_2018',

    # # pile-up jet id scale factors
    # 'CMS_eff_j_PUJET_id_' + opt.period,

    # # deep tau scale factors versus electrons
    # 'CMS_bbtt_' + opt.period + '_etauFR_barrel',
    # 'CMS_bbtt_' + opt.period + '_etauFR_endcap',
    ]

systNames = ['tesXXX_DM0',
             'tesXXX_DM1',
             'tesXXX_DM10',
             'tesXXX_DM11',

             'eesXXX_DM0',
             'eesXXX_DM1',
             'mesXXX',

             'jesXXX1', 'jesXXX2', 'jesXXX3', 'jesXXX4', 'jesXXX5',
             'jesXXX6', 'jesXXX7', 'jesXXX8', 'jesXXX9', 'jesXXX10', 'jesXXX11',

             'jesXXXTot']

## if running WITHOUT the shape syst, uncomment these 2 lines:
# systNamesOUT = []
# systNames = []

inFile = TFile.Open(opt.filename)
toth = len(inFile.GetListOfKeys())

histos_syst_up, histos_syst_down = ([] for _ in range(2))
yieldFolder = 'scales' + opt.period + '/'

for ih, key in enumerate(inFile.GetListOfKeys()):
	if ih%500==0:
        print(key, ih, '/', toth)
	kname = key.GetName()

	# If QCD is read from file and not computed as rate parameter in the datacards
	# we can skip all the SStight/OSinviso/SSinviso shifted template and save
	# a lot of time and space
	if opt.dynamQCD == 0:
		if 'SStight' in kname and 'Up'   in kname: continue
		if 'SStight' in kname and 'Down' in kname: continue
		if 'SStight' in kname and 'up'   in kname: continue
		if 'SStight' in kname and 'down' in kname: continue

		if 'OSinviso' in kname and 'Up'   in kname: continue
		if 'OSinviso' in kname and 'Down' in kname: continue
		if 'OSinviso' in kname and 'up'   in kname: continue
		if 'OSinviso' in kname and 'down' in kname: continue

		if 'SSinviso' in kname and 'Up'   in kname: continue
		if 'SSinviso' in kname and 'Down' in kname: continue
		if 'SSinviso' in kname and 'up'   in kname: continue
		if 'SSinviso' in kname and 'down' in kname: continue

	template = inFile.Get(kname)

	# protection against empty bins
    changedInt = False
    integral = template.Integral()
    if not opt.binOptimization:
        for ibin in range(1,template.GetNbinsX()+1):
  
        if "data" in kname: continue
  
     	if template.GetBinContent(ibin) <= 0:
     		changedInt = True
     		template.SetBinContent(ibin,1E-9)
     		template.SetBinError(ibin,1E-9)

	if integral>0:
		if changedInt and template.Integral()>0: template.Scale(integral/template.Integral())
	else:
		template.Scale(-100.) # set to negative in order to identify the problematic templates to be excluded in write_card.py

    continue
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
		#if "GGHH_NLO" in kname: kname = kname.replace("GGHH_NLO","ggHH").replace("_xs","_kt_1_hbbhtt").replace("cHHH", "kl_")
		#if "VBFHH"    in kname: kname = kname.replace("VBFHH","qqHH").replace("C3","kl").replace("_xs","_hbbhtt")


		# Rename singleH processes to use the HHModel BR scaling
		# ggH --> ggH_htt
		# qqH --> qqH_htt
		# WH  --> WH_htt
		# ZH  --> ZH_hbb
		# ttH --> ttH_hbb
		if opt.renameH:
			if kname[0:4] == "ggH_": kname = kname.replace("ggH_","ggH_htt_")
			if kname[0:4] == "qqH_": kname = kname.replace("qqH_","qqH_htt_")
			if kname[0:3] == "WH_" : kname = kname.replace("WH_" ,"WH_htt_" )
			if kname[0:3] == "ZH_" : kname = kname.replace("ZH_" ,"ZH_hbb_" )
			if kname[0:4] == "ttH_": kname = kname.replace("ttH_","ttH_hbb_")

		template.SetName(kname)
		template.SetTitle(kname)
		listHistos.append(template.Clone())

outname = 'analyzedOutPlotter_{}_{}_{}.root'.format(opt.period, opt.channel, opt.outname)
with TFile.Open(outname, 'RECREATE') as outfile:
    outfile.cd()
    for h in listHistos:
        h.Write()
    for h in histos_syst_up:
        h.Write()
    for h in histos_syst_down:
        h.Write()
