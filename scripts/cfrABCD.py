#!/usr/bin/python
from ROOT import *
import sys,os
import modules.ConfigReader as cfgr
import argparse
import fnmatch
import itertools

#tag = "30Jul2018_newPU_oldSF"
tag = "17Sep2018"
pref = ""
is2016 = False
if is2016: pref =  "../../../../HH2016/CMSSW_9_0_0/src/KLUBAnalysis/" 

makeABCD = False
makeRatioAB_CD = False
if makeABCD: makeRatioAB_CD = True

channel = "TauTau" #"ETau" "MuTau" "TauTau"

#compare ABCD
selections= ["baseline"] 
#regions=["SR","SStight","OSinviso","SSinviso"]
#legEntries = ["A - OS iso", "B - SS iso", "C - OS invIso", "D - SS invIso"]
vars = ["dau2_MVAisoNew"]
#vars = ["dau1_pt","tauH_mass", "dau1_eta","bjet1_pt","bH_mass","njets"]
#vars = ["dau1_eta"]
regions = ["OSloose", "SSloose"]
legEntries = ["OS", "SS"]

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


def ratioAB_CD(hList,var):
    ratioAB = hList[0].Clone("ratioAB")
    print "A: ", ratioAB.Integral()," - ", ratioAB.Integral(-1,-1)
    plotB = hList[1]
    print "B: ", plotB.Integral()," - ", plotB.Integral(-1,-1)
    ratioCD = hList[2].Clone("ratioCD")
    print "C: ", ratioCD.Integral(), " - ", ratioCD.Integral(-1,-1)
    plotD = hList[3]
    print "D: ", plotD.Integral(), " - ", plotD.Integral(-1,-1)

    ratioAB.Divide(plotB)
    ratioCD.Divide(plotD)
    ymax = max(ratioAB.GetMaximum(),ratioCD.GetMaximum())
        
    cratio = TCanvas("cratio","cratio", 600,600)
    cratio.Draw()
    legRatioEnt = ["A/B", "C/D"]
    ratioAB.SetLineColor(kRed)
    ratioCD.SetLineColor(kAzure)
    ratioAB.Draw("hist")
    ratioAB.SetMaximum(ymax*1.5)
    ratioCD.Draw("hist same")
    ratioAB.GetXaxis().SetTitle(var)
    ratioAB.GetYaxis().SetTitle("ratio")
    ratioAB.GetYaxis().SetTitleOffset(1.4)
    legRatio =  TLegend(0.5, 0.75,0.89,0.89)
    legRatio.AddEntry(ratioAB, legRatioEnt[0], "l")
    legRatio.AddEntry(ratioCD, legRatioEnt[1], "l")
    legRatio.Draw("same")
    cratio.Update()
    cratio.Modified()
    raw_input()
#    return ratioAB, ratioCD

#def cfrABCDrho(rho,hThisSel,hQCDan):
#    QCD_ratioCD = hThisSel[2].Clone("QCD_ratioCD")
#    QCD_D =  hThisSel[3].Clone("QCD_D")


def subtractBkg(inFile, var,sel,hBkg):
    hname = "data_obs_" + sel + "_" +var
    hdata = inFile.Get(hname)
    hdiff = hdata.Clone(sel+"_"+var)
    hdiff.SetTitle(hdiff.GetName())
    hdiff.Add(hBkg[sel][0],-1.)
    return hdiff



def sumBkg(inFile, var,sel,bkgList):
    #print bkgList
    hname = bkgList[0] + "_" + sel + "_" +var

    hnameTot = "bkgTot_" + sel + "_" +var
    hstack = THStack(hnameTot+"_stack",hnameTot+"_stack")

    hsum = inFile.Get(hname).Clone(hnameTot)
    hsum.SetTitle(hsum.GetName())
    hstack.Add(inFile.Get(hname))
    
    for i in range(1,len(bkgList)):
        #print len(bkgList)
        hname = bkgList[i] + "_" + sel + "_" +var
        #print bkgList[i] 
        #        inFile.Get(hname).SetFillColor(30+i)
        hbkg = inFile.Get(hname)
        hstack.Add(hbkg)
        hsum.Add(hbkg)
        # cstack = TCanvas()
        # cstack.Draw()
        # hstack.Draw("hist")
        # cstack.Update()
        # cstack.Modified()
        # raw_input()
        # csum = TCanvas()
        # cstack.Draw()
        # hsum.Draw()
        # cstack.Update()
        # cstack.Modified()
        # raw_input()
    return [hsum,hstack]



inDir = pref+"analysis_"+channel+"_"+tag+"/"
cfgName  = inDir + "mainCfg_"+channel+".cfg"
fileName = inDir + "outPlotter.root"
outDir = pref+"plots2017_"+channel
analizedName = inDir+'analyzedOutPlotter.cfg'


cfg        = cfgr.ConfigReader (cfgName)
selDefList = cfg.readListOption("general::selections") ## the selection definition
regDefList = cfg.readListOption("general::regions") ## the regions that are combined with the previous
bkgList    = cfg.readListOption("general::backgrounds")

#dataList   = cfg.readListOption("general::data")
selList    = [x[0] + '_' + x[1] for x in list(itertools.product(selDefList, regDefList))]


#colors = [kRed-4,
#          kOrange+1,
#          kAzure-2,
#          kGreen+2
#]

colors = [kRed-4, kAzure-2]

print bkgList
inFile = TFile.Open(fileName)
if makeABCD:
    analyzedFile = TFile.Open(analyzedName)
print inFile


gStyle.SetOptStat(0)
hBkg = {}
hQCD = {}
if cfg.hasSection("merge"):
    for groupname in cfg.config['merge']:
        if "data" in groupname: continue
        mergelist = cfg.readListOption('merge::'+groupname)
        for x in mergelist:
            bkgList.remove(x)
        bkgList.append(groupname)

print bkgList
        
for var in vars:
    for i in range(len(selList)):

        sel = selList[i]
        hBkg[sel] = sumBkg(inFile, var, sel,bkgList)
        hBkg[sel][0].SetLineWidth(4)
        hQCD[sel] = subtractBkg(inFile, var, sel, hBkg)
        hQCD[sel].SetLineWidth(2)
        #if not ("eta" in var) or ("iso" in var): hQCD[sel].Rebin(2)

        
    canvas = TCanvas("c1","c1",600,600)

    print regDefList
    print "#"
    print selList


    if makeRatioAB_CD:

        for sel in selections:
            hThisSel = []
            print sel
        

            i = 0
            for reg in regions:
                
                reg_sel = sel+"_"+reg
                
                if hQCD[reg_sel].Integral() < 0.01: continue
                hThisSel.append(hQCD[reg_sel])
                makeNonNegativeHistos(hThisSel)
            ratios = ratioAB_CD(hThisSel, var)
            if makeABCD:
                hnameQCD =  "QCD_" + sel + "_SR_" +var
                hQCDan = analyzedFile.Get(hnameQCD)
                rho = ratios[0].Clone("rho")
                rho.Divide(ratios[1])
                cfrABCDrho(rho,hThisSel,hQCDan)
            
            
    
    for sel in selections:
        hThisSelNorm = []
        hThisSel = []
        print sel
        
        leg = TLegend(0.5, 0.6,0.89,0.89)
        i = 0
        for reg in regions:
            
            reg_sel = sel+"_"+reg

            if hQCD[reg_sel].Integral() < 0.01: continue
            hThisSel.append(hQCD[reg_sel])
            norm = 1./hQCD[reg_sel].Integral()
            hQCD[reg_sel].Scale(norm)
            hThisSelNorm.append(hQCD[reg_sel])
            makeNonNegativeHistos(hThisSelNorm)
        ymax = max([hist.GetMaximum() for hist in hThisSelNorm])
        print ymax
        for i, histo in enumerate(hThisSel):
           

            histo.SetLineColor(colors[i])
            if i == 0:
                histo.GetXaxis().SetTitle(var)
                histo.GetYaxis().SetTitle("Normalized events")
                histo.GetYaxis().SetTitleOffset(1.4)
                histo.SetMaximum(ymax*1.8)
                histo.Draw("hist")
            
            else:
                histo.Draw("hist same")
            leg.AddEntry(histo, legEntries[i],"l")
            leg.Draw()


        canvas.Modified()
        canvas.Update()
        
            
        raw_input()
        


     
