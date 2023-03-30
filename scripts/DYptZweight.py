
#!/usr/bin/python
from ROOT import *
import sys,os
import modules.ConfigReader as cfgr
import argparse
import fnmatch
import itertools


tag = "3Oct2018_DYsep_goodPU_goodXS"
channel = "MuMu" 

selection="baseline_noQCD"
#selection="baseline_MET40"
#selection="baseline_pt0to50"
#selection="baseline_pt50to150"
#selection="baseline_pt150"


doNpv = True
doPt = False

def subtractBkg(inFile, var,sel,hBkg):
    hname = "data_obs_" + sel + "_" +var
    hdata = inFile.Get(hname)
    hdiff = hdata.Clone(sel+"_"+var)
    hdiff.SetTitle(hdiff.GetName())
    hdiff.Add(hBkg[0],-1.)
    return hdiff



def sumBkg(inFile, var,sel,bkgList):

    hname = bkgList[0] + "_" + sel + "_" +var
    hnameTot = "bkgTot_" + sel + "_" +var
    hstack = THStack(hnameTot+"_stack",hnameTot+"_stack")

    hsum = inFile.Get(hname).Clone(hnameTot)
    hsum.SetTitle(hsum.GetName())
    hstack.Add(inFile.Get(hname))
    for i in range(1,len(bkgList)):
        hname = bkgList[i] + "_" + sel + "_" +var
        
        #        inFile.Get(hname).SetFillColor(30+i)
        if not "DY" in hname:
            hbkg = inFile.Get(hname)
            hsum.Add(hbkg)
        
    return [hsum,hstack]


def sumDY(inFile, var,sel):

    hname = "DY0b_" + sel + "_" +var
    hnameTot = "DY_" + sel + "_" +var
    hstack = THStack(hnameTot+"_stack",hnameTot+"_stack")

    
    hsum = inFile.Get(hname).Clone(hnameTot)
    hsum.SetTitle(hsum.GetName())
    hstack.Add(inFile.Get(hname))
    hname = "DY1b_" + sel + "_" +var
    hbkg = inFile.Get(hname)
    hsum.Add(hbkg)

    hname = "DY2b_" + sel + "_" +var
    hbkg = inFile.Get(hname)
    hsum.Add(hbkg)

    return [hsum,hstack]


def makeFit(graph, xmin, xmax):
 #   func = TF1("lin","([0]+[1]*x +[2]*erf(x))",xmin,xmax)
    func = TF1("f"+str(xmax),"[0]+x*[1]",xmin,xmax)
    
    func.SetParameters(2.0,1.0)

    graph.Fit(func, "R")


    
def makeRatio(data,MC):
    canvas = TCanvas("c1", "c1",600,600)


    pad1 = TPad ("pad1", "pad1", 0, 0.25, 1, 1.0)
    pad1.SetBottomMargin(0.001);
    pad1.SetFrameLineWidth(2)
    pad1.Draw()
    pad2 = TPad ("pad2", "pad2", 0, 0.01, 1, 0.2496)

    pad2.SetTopMargin(0.05)
    pad2.SetBottomMargin(0.4)
    pad2.SetGridy(True)
    pad2.SetGridx(True)
    pad2.SetFrameLineWidth(2)
    pad2.Draw()
    
    pad1.cd()
    
    ymax = data.GetMaximum()

    data.SetMaximum(ymax+150)

    data.SetLineColor(kRed)
    data.SetLineWidth(2)
    data.GetXaxis().SetTitle(var)
    data.GetYaxis().SetTitle("Events")
    
    data.GetYaxis().SetTitleOffset(1.2)
    data.Draw("hist")
    MC.SetLineWidth(2)
    MC.Draw("hist same")
    leg = TLegend(0.4, 0.7,0.89,0.89)
    leg.AddEntry(data, "(Data - other bkgs)","l")
    leg.AddEntry(MC,"DY","l")
    leg.Draw("same")
    data.SetTitle("")
    xmin =data.GetXaxis().GetXmin()
    xmax =data.GetXaxis().GetXmax()
    
    pad2.cd()
    bkg = pad2.DrawFrame(xmin,0.5,xmax,1.5)
      
    ratio = TGraphAsymmErrors()


    nbins = MC.GetNbinsX()
   
    for i in range(1,nbins+1):

        if not (MC.GetBinContent(i) == 0):
            
            ratio.SetPoint(i, MC.GetBinCenter(i), data.GetBinContent(i)/MC.GetBinContent(i))

        else:
            ratio.SetPoint(i, MC.GetBinCenter(i), 0)
    ratio.SetTitle("")
    
    bkg.GetYaxis().SetNdivisions(505)
    bkg.GetYaxis().SetTitle("Ratio")
    bkg.GetXaxis().SetLabelFont(45)
    bkg.GetXaxis().SetLabelSize(15)
    bkg.GetYaxis().SetLabelFont(43)
    bkg.GetYaxis().SetLabelSize(15)
    ratio.SetMarkerStyle(20)
    ratio.SetMarkerSize(0.7)
    bkg.GetXaxis().SetTitleSize(0.1)
    bkg.GetXaxis().SetTitle(var)
    bkg.GetYaxis().SetTitleSize(0.1)

    ratio.Draw("p same")
    
    xmin =MC.GetXaxis().GetXmin()
    xmax = MC.GetXaxis().GetXmax()

    makeFit(ratio, 2, 15)
    makeFit(ratio, 10, xmax)
#    makeFit(ratio, 10, xmax)
    canvas.Modified()
    canvas.Update()
    raw_input()


inDir = "analysis_"+channel+"_"+tag+"/"
cfgName  = inDir + "mainCfg_"+channel+".cfg"
fileName = inDir + "analyzedOutPlotter.root"


cfg        = cfgr.ConfigReader (cfgName)
sel =  selection+"_SR" ## the selection definition

bkgList    = cfg.readListOption("general::backgrounds")

#dataList   = cfg.readListOption("general::data")



inFile = TFile.Open(fileName)


if cfg.hasSection("merge"):
    for groupname in cfg.config['merge']:
        if "data" in groupname: continue
        mergelist = cfg.readListOption('merge::'+groupname)
        for x in mergelist:
            bkgList.remove(x)
        bkgList.append(groupname)

if doPt:
    var = "tauH_pt"
    
    hBkg = sumBkg(inFile, var, sel,bkgList)
    hDY = sumDY(inFile, var, sel)
    
    hData = subtractBkg(inFile, var, sel, hBkg)
    
    print "data ", hData.Integral(-1,- 1)        
    print "DY ", hDY[0].Integral(-1,-1)
    print ""
    weight = hData.Integral(-1, -1)/ hDY[0].Integral(-1,-1)
    print "data/DY = ", weight 
    
    
    makeRatio (hData, hDY[0])

if doNpv:
    var = "npv"
    hBkg = sumBkg(inFile, var, sel,bkgList)
    hDY = sumDY(inFile, var, sel)
    
    hData = subtractBkg(inFile, var, sel, hBkg)

    for i in range(1, hData.GetNbinsX() +1):
        bindata=hData.GetBinContent(i)
        binDY = hDY[0].GetBinContent(i)
        if binDY > 0.:
            binsRatio = bindata/binDY
        else:
            binsRatio = 1
        print str(i-1), binsRatio
    
