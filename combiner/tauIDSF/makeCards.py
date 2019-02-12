from ROOT import *
import argparse
import os
import sys
from array import array
import modules.ConfigReader as cfgr


def retrieveHistos (rootFile, namelist, var, sel, reg):
    res = {}
    for name in namelist:
        fullName = name + "_" + sel + "_" + reg + "_" + var

        if not rootFile.GetListOfKeys().Contains(fullName):
            print "*** WARNING: histo " , fullName , " not available"
            continue
        h = rootFile.Get(fullName)
        res[name] = h
    return res



def getHisto (histoName,inputList):
        for idx, name in enumerate(inputList):
                if (name.startswith(histoName) and name.endswith(histoName)):
                        h = inputList[name].Clone (histoName)
                        break
        return h


def makeSum (sumName, histoList):
    for i,h in enumerate(histoList):
        if i == 0: hsum = h.Clone(sumName)
        else: hsum.Add(h)
    return hsum



def makeNonNegativeHistos (hList):
    for h in hList:
        integral = h.Integral()
        for b in range (1, h.GetNbinsX()+1):
            if (h.GetBinContent(b) < 0):
               h.SetBinContent (b, 0)
        integralNew = h.Integral()        
        if (integralNew != integral):
            print "** INFO: removed neg bins from histo " , h.GetName() 
        
        # print h.GetName() , integral , integralNew
        if integralNew == 0:
            h.Scale(0)
        else:
            h.Scale(integral/integralNew) 



parser = argparse.ArgumentParser(description='Command line parser of plotting options')
parser.add_argument('--sel', dest='sel', help='selection name', default=None)
parser.add_argument('--tag', dest='tag', help='plots output folder name', default="./")
parser.add_argument('--decay', dest='decay', help='decayMode', default=None)
parser.add_argument('--qcd', dest='qcd', help='decayMode', default=False)
args = parser.parse_args()

#cfgr = imp.load_source('module.ConfigReader', '~/HH2017/CMSSW_9_0_0/src/KLUBAnalysis/scripts/modules/Co#nfigReader.py')
#cfgr.MyClass()




inFileDir = "../../analysis_TauTau_"+args.tag
inFileName = "/analyzedOutPlotter.root"
inFile = TFile.Open(inFileDir+inFileName)

cfgName  =  inFileDir + "/mainCfg_TauTau.cfg"
cfg        = cfgr.ConfigReader (cfgName)
bkgList    = cfg.readListOption("general::backgrounds")
dataList   = ["data_obs"]

if cfg.hasSection("merge"): 
    for groupname in cfg.config['merge']:
        if "data" in groupname: continue
        mergelist = cfg.readListOption('merge::'+groupname)
        for x in mergelist:
            bkgList.remove(x)
        bkgList.append(groupname)

qcdList = ["QCD"]

sel  = args.sel
regions = []
if not args.qcd:
    regions = ["SR", "OSinviso","SSinviso","SStight"]
else: 
    regions = ["SR"]

if not os.path.exists(args.tag+"/"+sel): os.makedirs(args.tag+"/"+sel)

for reg in regions:
    hDatas = retrieveHistos  (inFile, dataList, "nRealTaus", sel,reg)
    hData = getHisto("data_obs", hDatas ).Clone("hData")
    
    
    hBkgs = retrieveHistos  (inFile, bkgList, "nRealTaus", sel,reg)
    hDY = getHisto("DY", hBkgs)
    hTT = getHisto("TT", hBkgs)
    hWJets = getHisto("WJets", hBkgs)
    hothers = getHisto("other", hBkgs)
    if args.qcd:
        hQCDs = retrieveHistos  (inFile, qcdList, "dau1_eta", sel,reg)     ##just retrieving some histo where QCD has some meaning
        hQCD = getHisto("QCD", hQCDs)
    hBkgList = [hothers, hWJets, hTT, hDY]
    makeNonNegativeHistos (hBkgList) 
    bkgSum = makeSum ("bkgSum", hBkgList)
    
    print "Region ", reg
    print " 0taus ", str(bkgSum.GetBinContent(1))
    print " 1taus ", str(bkgSum.GetBinContent(2))
    print " 2taus ", str(bkgSum.GetBinContent(3))
    if args.qcd: print " QCD ", str(hQCD.Integral())
    print " data ", str(hData.GetBinContent(0))

    suffix = ""
    if reg == "SStight": suffix = "_regB"
    if reg == "OSinviso": suffix = "_regC"
    if reg == "SSinviso": suffix = "_regD"

    if args.qcd: 
        with open("templates/datacard_template"+suffix+".txt","r") as template:
            lines = template.readlines()
            with open(args.tag+"/"+sel+"/datacard_QCD.txt", "w+") as outfile:
                for line in lines:
                    line = line.replace('_NDATA'+suffix+'_',  str(round(hData.GetBinContent(0),2)))    
                    line = line.replace('_N0TAUS'+suffix+'_', str(round(bkgSum.GetBinContent(1),2)))    
                    line = line.replace('_N1TAUS'+suffix+'_', str(round(bkgSum.GetBinContent(2),2)))    
                    line = line.replace('_N2TAUS'+suffix+'_', str(round(bkgSum.GetBinContent(3),2)))
                    line = line.replace('_QCD_', str(round(hQCD.Integral(),2)))
                    line = line.replace('_DM_', args.decay)    
                    line = line.replace('_SF_', 'SF_'+args.decay)    
                    if not line.startswith("alpha") : 
                        outfile.write(line)

    else:
        with open("templates/datacard_template"+suffix+".txt","r") as template:
            card = template.read()

        card = card.replace('_NDATA'+suffix+'_',  str(round(hData.GetBinContent(0),2)))    
        card = card.replace('_N0TAUS'+suffix+'_', str(round(bkgSum.GetBinContent(1),2)))    
        card = card.replace('_N1TAUS'+suffix+'_', str(round(bkgSum.GetBinContent(2),2)))    
        card = card.replace('_N2TAUS'+suffix+'_', str(round(bkgSum.GetBinContent(3),2)))
        card = card.replace('_QCD'+suffix+'_', "1")
        card = card.replace('_DM_', args.decay)    
        card = card.replace('_SF_', 'SF_'+args.decay)    

        with open(args.tag+"/"+sel+"/datacard"+suffix+".txt", "w+") as outfile:
            outfile.write(card)


if not args.qcd:
    os.chdir(args.tag+'/'+sel)
    os.system('echo $PWD')    
    os.system('combineCards.py datacard.txt datacard_regC.txt datacard_regD.txt datacard_regB.txt > combined.txt')
    os.chdir('../..')
