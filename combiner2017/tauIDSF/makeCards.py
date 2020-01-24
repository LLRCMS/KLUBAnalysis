from ROOT import *
import argparse
import os
import sys
from array import array
import modules.ConfigReader as cfgr
from systReader import *

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


def makeSum (inFile, sumName, nameList, var, sel, reg):
    histoList = retrieveHistos  (inFile, nameList, var, sel,reg)
    hDY = getHisto("DY", histoList)
    hTT = getHisto("TT", histoList)
    hWJets = getHisto("WJets", histoList)
    hothers = getHisto("other", histoList)

    hBkgList = [hothers, hWJets, hTT, hDY]
    makeNonNegativeHistos (hBkgList) 
    for i,h in enumerate(hBkgList):
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



def writeCombCommand(decay, isqcd):
    
    param = []
    paramRange = []
    if "both" in decay: 
        if isqcd:
            datacard = "datacard_QCD" 
            param = ["SF_" + decay.replace("both", "")]
            paramRange = ["0.5,1.5"]
            algo="singles"
        else:
            datacard = "combined"
            param = ["SF_" + decay.replace("both", ""),"QCD_regC", "QCD_regD", "QCD_regB" ]
            paramRange = ["0.5,1.5", "0,1000", "0,1000", "0,1000"]
            algo="singles"

    else:
        decays=decay.split("_")
        decay1=decays[1] 
        decay2=decays[2] 
        if isqcd:
            datacard = "datacard_QCD" 
            param = ["SF_" + decay1, "SF_" + decay2]
            paramRange = ["0.5,1.5", "0.5,1.5"]
            algo="singles"
        else:
            datacard = "combined"
            param = ["SF_" + decay1, "SF_" + decay2,"QCD_regC", "QCD_regD", "QCD_regB" ]
            paramRange = ["0.5,1.5","0.5,1.5", "0,1000", "0,1000", "0,1000"]
            algo="singles"


   # create workspace
    command = "text2workspace.py "+datacard+".txt\n"
    # fit method
    command = command + "combine -M MultiDimFit "
    # datacard name
    command = command + datacard + ".root "
    # fit parameters
    for p in param:
        command = command + "-P " + p + " "
    # freeze param r
    command = command + "--floatOtherPOIs 0 "
    # which algo
    command = command + "--algo="+ algo
    # set parameter ranges
    command = command + " --setParameterRanges "
    for i,p in enumerate(param):
        command= command + p + "=" + paramRange[i]+":"
    command = command[:-1]  # remove last :
    # make robust fit
    command = command + " --robustFit 1"
    
    return command


parser = argparse.ArgumentParser(description='Command line parser of plotting options')
parser.add_argument('--sel', dest='sel', help='selection name', default=None)
parser.add_argument('--ch', dest='ch', help='channel', default="TauTau")
parser.add_argument('--tag', dest='tag', help='plots output folder name', default="./")
parser.add_argument('--decay', dest='decay', help='decayMode', default=None)
parser.add_argument('--qcd', dest='qcd', help='QCD already computed', default=False)
args = parser.parse_args()

#cfgr = imp.load_source('module.ConfigReader', '~/HH2017/CMSSW_9_0_0/src/KLUBAnalysis/scripts/modules/Co#nfigReader.py')
#cfgr.MyClass()




inFileDir = "../../analysis_"+args.ch+"_"+args.tag
inFileName = "/analyzedOutPlotter.root"
inFile = TFile.Open(inFileDir+inFileName)

cfgName  =  inFileDir + "/mainCfg_"+args.ch+"_Legacy2018_tauIDSF.cfg"
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



sel  = args.sel
regions = []
if not args.qcd:
    regions = ["SR", "OSinviso","SSinviso","SStight"]
else: 
    regions = ["SR"]

if not os.path.exists(args.tag+"/"+args.ch+"/"+sel): os.makedirs(args.tag+"/"+args.ch+"/"+sel)

for reg in regions:
    hDatas = retrieveHistos  (inFile, dataList, "nRealTaus", sel,reg)
    hData = getHisto("data_obs", hDatas ).Clone("hData")

    if args.qcd:
        qcdList = ["QCD"]
        hQCDs = retrieveHistos  (inFile, qcdList, "dau1_eta", sel,reg)     ##just retrieving some histo where QCD has some meaning
        hQCD = getHisto("QCD", hQCDs)    

    bkgSum = makeSum (inFile, "bkgSum", bkgList, "nRealTaus", sel, reg)
    bkgSum1 = makeSum (inFile, "bkgSum", bkgList, "isTau1real", sel, reg)
    bkgSum2 = makeSum (inFile, "bkgSum", bkgList, "isTau2real", sel, reg)
    
    print "Region ", reg
    print " 0taus ", str(bkgSum.GetBinContent(1))
    print " 1taus ", str(bkgSum.GetBinContent(2))
    print " - ", str(bkgSum2.GetBinContent(2))
    print " - ", str(bkgSum1.GetBinContent(2))
    print " 2taus ", str(bkgSum.GetBinContent(3))
    if args.qcd: print " QCD ", str(hQCD.Integral())
    print " data ", str(hData.GetBinContent(0))

    suffix = ""
    if reg == "SStight": suffix = "_regB"
    if reg == "OSinviso": suffix = "_regC"
    if reg == "SSinviso": suffix = "_regD"

    ### EVENT YIELD
    #### same decay mode
    if "both" in args.decay:
    	if args.qcd: 
    	    with open("templates/datacard_template"+suffix+".txt","r") as template:
    	        lines = template.readlines()
    	        with open(args.tag+"/"+args.ch+"/"+sel+"/datacard_QCD.txt", "w+") as outfile:
    	            for line in lines:
    	                line = line.replace('_NDATA'+suffix+'_',  str(round(hData.GetBinContent(0),2)))    
    	                line = line.replace('_N0TAUS'+suffix+'_', str(round(bkgSum.GetBinContent(1),2)))    
    	                line = line.replace('_N1TAUS'+suffix+'_', str(round(bkgSum.GetBinContent(2),2)))    
    	                line = line.replace('_N2TAUS'+suffix+'_', str(round(bkgSum.GetBinContent(3),2)))
    	                line = line.replace('_NQCD_', str(round(hQCD.Integral(),2)))
    	                line = line.replace('_DM_', args.decay.replace("both", ""))    
    	                line = line.replace('_SF_', 'SF_'+args.decay.replace("both", ""))    
    	                if not line.startswith("alpha") : 
    	                    outfile.write(line)
    	
    	else:
    	    with open("templates/datacard_template"+suffix+".txt","r") as template:
    	        card = template.read()
    	
    	    card = card.replace('_NDATA'+suffix+'_',  str(round(hData.GetBinContent(0),2)))    
    	    card = card.replace('_N0TAUS'+suffix+'_', str(round(bkgSum.GetBinContent(1),2)))    
    	    card = card.replace('_N1TAUS'+suffix+'_', str(round(bkgSum.GetBinContent(2),2)))    
    	    card = card.replace('_N2TAUS'+suffix+'_', str(round(bkgSum.GetBinContent(3),2)))
    	    card = card.replace('_QCD'+suffix+'_',   str(round(hData.GetBinContent(0)-bkgSum.Integral(),2)))
            card = card.replace('_NQCD_', "1")
    	    card = card.replace('_DM_', args.decay.replace("both",""))    
    	    card = card.replace('_SF_', 'SF_'+args.decay.replace("both",""))    
    	
    	    with open(args.tag+"/"+args.ch+"/"+sel+"/datacard"+suffix+".txt", "w+") as outfile:
    	        outfile.write(card)
    #### cross decay mode
    else:
        decays = args.decay.split("_")
        decay1 = decays[1]
        decay2 = decays[2]
        print decay1, decay2
        if args.qcd: 
    	    with open("templates/datacard_template_cross"+suffix+".txt","r") as template:
    	        lines = template.readlines()
    	        with open(args.tag+"/"+args.ch+"/"+sel+"/datacard_QCD.txt", "w+") as outfile:
    	            for line in lines:
    	                line = line.replace('_NDATA'+suffix+'_',  str(round(hData.GetBinContent(0),2)))    
    	                line = line.replace('_N0TAUS'+suffix+'_', str(round(bkgSum.GetBinContent(1),2)))    
    	                line = line.replace('_N1TAUS1'+suffix+'_', str(round((bkgSum.GetBinContent(2) + bkgSum.GetBinContent(3) - bkgSum2.GetBinContent(2)),2)))  # events with only tau1 real  
    	                line = line.replace('_N1TAUS2'+suffix+'_', str(round((bkgSum.GetBinContent(2) + bkgSum.GetBinContent(3) - bkgSum1.GetBinContent(2)),2)))  # events with only tau2 real  
    	                line = line.replace('_N2TAUS'+suffix+'_', str(round(bkgSum.GetBinContent(3),2)))
    	                line = line.replace('_NQCD_', str(round(hQCD.Integral(),2)))
    	                line = line.replace('_DM_', args.decay)    
    	                line = line.replace('_SF1_', 'SF_'+decay1)    
    	                line = line.replace('_SF2_', 'SF_'+decay2)    
    	                line = line.replace('_SFcross_', 'SF_'+decay1+"_"+decay2)    
    	                if not line.startswith("alpha") : 
    	                    outfile.write(line)
    	
    	else:
    	    with open("templates/datacard_template_cross"+suffix+".txt","r") as template:
    	        card = template.read()
    	
    	    card = card.replace('_NDATA'+suffix+'_',  str(round(hData.GetBinContent(0),2)))    
    	    card = card.replace('_N0TAUS'+suffix+'_', str(round(bkgSum.GetBinContent(1),2)))    
    	    card = card.replace('_N1TAUS1'+suffix+'_', str(round((bkgSum.GetBinContent(2) + bkgSum.GetBinContent(3) - bkgSum2.GetBinContent(2)),2)))    
            card = card.replace('_N1TAUS2'+suffix+'_', str(round((bkgSum.GetBinContent(2) + bkgSum.GetBinContent(3) - bkgSum1.GetBinContent(2)),2)))    
    	    card = card.replace('_N2TAUS'+suffix+'_', str(round(bkgSum.GetBinContent(3),2)))
    	    card = card.replace('_QCD'+suffix+'_',   str(round(hData.GetBinContent(0)-bkgSum.Integral(),2)))
            card = card.replace('_NQCD_', "1")
            card = card.replace('_SF1_', 'SF_'+decay1)    
            card = card.replace('_SF2_', 'SF_'+decay2)    
            card = card.replace('_SFcross_', 'SF_'+decay1+"_"+decay2)    
    	
    	    with open(args.tag+"/"+args.ch+"/"+sel+"/datacard"+suffix+".txt", "w+") as outfile:
    	        outfile.write(card)


    ### SYSTEMATICS
    # reg=="SR":
    #  sigList = []
    #  syst = systReader("../../config/systematics.cfg",sigList,bkgList,file)
    #  syst.writeOutput(False)
    #  syst.verbose(True)
    #  syst.addSystFile("../../config/systematics_tautau.cfg")
    #  
    #  syst.writeSystematics()
    #  for isy in range(len(syst.SystNames)) :
    #      if "CMS_scale_t" in syst.SystNames[isy] or "CMS_scale_j" in syst.SystNames[isy]: continue
    #      for iproc in range(len(syst.SystProcesses[isy])) :
    #          if "/" in syst.SystValues[isy][iproc] :
    #              f = syst.SystValues[isy][iproc].split("/")
    #              systVal = (float(f[0]),float(f[1]))
    #          else :
    #              systVal = float(syst.SystValues[isy][iproc])
    #          #print isy, iproc, systVal
    #          print "adding Syst",systVal,syst.SystNames[isy],syst.SystTypes[isy],"to",syst.SystProcesses[isy][iproc]






if not args.qcd:
    os.chdir(args.tag+'/'+args.ch+"/"+sel)
    os.system('echo $PWD')    
    os.system('combineCards.py SR=datacard.txt regC=datacard_regC.txt regD=datacard_regD.txt regB=datacard_regB.txt > combined.txt')
    os.chdir('../../..')
    with open(args.tag+"/"+args.ch+"/"+sel+"/runCombine.sh", "w+") as outScript:
        outScript.write(writeCombCommand(args.decay, False))
else:
    with open(args.tag+"/"+args.ch+"/"+sel+"/runCombine_QCDfixed.sh", "w+") as outScript:
        outScript.write(writeCombCommand(args.decay, True))
