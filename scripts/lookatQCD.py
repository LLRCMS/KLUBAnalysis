#!/usr/bin/python
from ROOT import *
import sys,os
import modules.ConfigReader as cfgr
import argparse
import itertools

tag = "4Dec2018"
channel = "MuTau" #"ETau" "MuTau" "TauTau"
isIsoBin = False
isMakeRatio = True
isMakeComparison = False
ratioHist = ["isoDau2_QCD_SS_base","isoDau2_QCD_OSblind_base"] #["isoDau2_QCD_T_SS_base_T","isoDau2_QCD_T_OSblind_base_T"]
#ratioHist = ["isoDau2_QCD_SS_base","isoDau2_QCD_OS_base"] # D/C
#ratioHist = ["isoDau2_SR_SS_base","isoDau2_SR_OS_base"] # B/A
isoHist= ["SS_base", "OSblind_base"] #["SS_base_T", "OSblind_base_T"] #["SS_VBFTbtagM","OSblind_VBFTbtagM"]
isoSelHist=["0","1","2","SR"]
#isoSelHist=["0","1","2"]
#var = "dau2_pt"
comparHisto = ["isoDau2_SR_OS_base","isoDau2_SR_SS_base","isoDau2_QCD_OS_base","isoDau2_QCD_SS_base"] # A-B-C-D
vars = ["dau1_pt","dau1_eta","dau1_phi","dau2_pt","dau2_eta","dau2_phi","tauH_SVFIT_mass","tauH_mass","bH_mass", "bjet1_pt", "bjet2_pt", "bjet1_eta", "bjet2_eta", "met_et", "met_phi", "bH_pt", "tauH_pt", "njets"]
vars = ["bH_mass"]
#vars = ["njets","dau1_pt", "dau2_pt", "tauH_mass", "bH_mass", "bH_pt", "tauH_pt"]
#vars = ["dau1_pt","dau2_pt","tauH_SVFIT_mass","tauH_mass","bH_mass"]
#TT_isoDau2_5_OSblind_bH_mass
# dau1_pt, dau1_eta, dau1_phi, dau2_pt, dau2_eta, dau2_phi, tauH_SVFIT_mass, tauH_mass, bjet1_pt, bjet2_pt, bjet1_eta, bjet2_eta, bH_mass, tauH_SVFIT_mass


def subtractBkg(inFile, var,sel,hBkg):
    hname = "data_obs_" + sel + "_" +var
    hdata = inFile.Get(hname)
    hdiff = hdata.Clone(sel+"_"+var)
    hdiff.SetTitle(hdiff.GetName())
    hdiff.Add(hBkg[sel][0],-1.)
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
        hbkg = inFile.Get(hname)
        hstack.Add(hbkg)
        hsum.Add(hbkg)
        
    return [hsum,hstack]


def makeRatio(histo1,histo2,hlist,canvas):
    canvas.cd()

    pad1 = None
    pad2 = None
    pad1 = TPad ("pad1", "pad1", 0, 0.25, 1, 1.0)
    pad1.SetBottomMargin(0.001);
    pad1.SetFrameLineWidth(2)
    #pad1.SetGridx(True)
    #pad1.SetGridy(True)                                  
    pad1.Draw()
    pad2 = TPad ("pad2", "pad2", 0, 0.01, 1, 0.2496)

    pad2.SetTopMargin(0.05)
    pad2.SetBottomMargin(0.4)
    pad2.SetGridy(True)
    pad2.SetGridx(True)
    pad2.SetFrameLineWidth(2)
    pad2.Draw()
    print "OS/SS = "+str(hlist[histo2].Integral(1,2000)/hlist[histo1].Integral(1,2000))

    text_OSSS = TPaveText(0.6,0.55,0.89,0.7,'blNDC')
    text_OSSS.SetBorderSize(1)
    text_OSSS.SetFillColor(kWhite)
    text_OSSS.AddText('OS/SS = ' + str(hlist[histo2].Integral(1,2000)/hlist[histo1].Integral(1,2000)))

    pad1.cd()
    norm = 1./hlist[histo1].Integral()
    hlist[histo1].Scale(norm)
    norm = 1./hlist[histo2].Integral()
    hlist[histo2].Scale(norm)
    ymax = max([hlist[hist].GetMaximum() for hist in ratioHist])

    hlist[histo1].SetMaximum(ymax+0.15)

    hlist[histo1].SetLineColor(colors[0])
    hlist[histo1].GetXaxis().SetTitle(var)
    hlist[histo1].GetYaxis().SetTitle("Normalized events")
    
    hlist[histo1].GetYaxis().SetTitleOffset(1.2)
    hlist[histo1].Draw("hist")
    hlist[histo2].SetLineColor(colors[1])
    hlist[histo2].Draw("hist same")
    leg = TLegend(0.4, 0.7,0.89,0.89)
    leg.AddEntry(hlist[histo1], hlist[histo1].GetTitle(),"l")
    leg.AddEntry(hlist[histo2], hlist[histo2].GetTitle(),"l")
    leg.Draw("same")
    hlist[histo1].SetTitle("")
    xmin =hlist[histo1].GetXaxis().GetXmin()
    xmax = hlist[histo1].GetXaxis().GetXmax()
    l0 = TLine(xmin, 0, xmax, 0)
    l0.SetLineColor(kBlack)
    l0.SetLineStyle(3)
    l0.Draw("same")
    text_OSSS.Draw()

    pad2.cd()
    bkg = pad2.DrawFrame(xmin,0.8,xmax,1.2)
    #bkg = pad2.DrawFrame(xmin,0.5,xmax,2.)
  
    ratio = TGraphAsymmErrors()

    hratio = hlist[histo2].Clone()
    nbins = hlist[histo2].GetNbinsX()

    for i in range(1,nbins+1):

        if not (hlist[histo1].GetBinContent(i) == 0):
            
            ratio.SetPoint(i, hlist[histo1].GetBinCenter(i), hlist[histo2].GetBinContent(i)/hlist[histo1].GetBinContent(i))

        else:
            ratio.SetPoint(i, hlist[histo1].GetBinCenter(i), 0)
    ratio.SetTitle("")
#    ratio.GetXaxis().SetLimits(0.,1.)
    hratio.Divide(hlist[histo1])
    hratio.SetLineColor(colors[1])
    bkg.GetYaxis().SetNdivisions(505)
    bkg.GetYaxis().SetTitle("Ratio")
    bkg.GetXaxis().SetLabelFont(45)
    bkg.GetXaxis().SetLabelSize(15)
    bkg.GetYaxis().SetLabelFont(43)
    bkg.GetYaxis().SetLabelSize(15)
    ratio.SetMarkerStyle(20)
    ratio.SetMarkerSize(0.7)
    bkg.GetXaxis().SetTitleSize(0.1)
    bkg.GetXaxis().SetTitle(hratio.GetXaxis().GetTitle())
    bkg.GetYaxis().SetTitleSize(0.1)

    ratio.Draw("p same")
    ratio.GetXaxis().SetLimits(0,1)
    hratio.SetMarkerStyle(1);
    hratio.SetFillColor(kGray+3);
    hratio.SetFillStyle(3002);

    hratio.Draw("e2 same")
    
    xmin =hratio.GetXaxis().GetXmin()
    xmax = hratio.GetXaxis().GetXmax()
    l1 = TLine(xmin, 1, xmax, 1)
    l1.SetLineColor(colors[0])
    l1.SetLineWidth(2)
    l1.Draw("same")
    canvas.Modified()
    canvas.Update()
    canvas.SaveAs(outDir+ratioHist[0]+"_"+ratioHist[1]+"_"+var+".pdf")
    canvas.SaveAs(outDir+ratioHist[0]+"_"+ratioHist[1]+"_"+var+".png")
    #canvas.SaveAs(outDir+"/C_D_base/"+ratioHist[0]+"_"+ratioHist[1]+"_"+var+".pdf")
    #canvas.SaveAs(outDir+"/C_D_base/"+ratioHist[0]+"_"+ratioHist[1]+"_"+var+".png")
    #canvas.SaveAs(outDir+"/A_B_base/"+ratioHist[0]+"_"+ratioHist[1]+"_"+var+".pdf")
    #canvas.SaveAs(outDir+"/A_B_base/"+ratioHist[0]+"_"+ratioHist[1]+"_"+var+".png")

    





inDir = "lookatQCD_"+channel+"_"+tag+"/"
cfgName  = inDir + "lookAtQCD_VBF_"+channel+".cfg"
fileName = inDir + "outPlotter.root"
outDir = "lookatQCD_"+channel+"_"+tag+"/"

cfg        = cfgr.ConfigReader (cfgName)
selDefList = cfg.readListOption("general::selections") ## the selection definition
regDefList = cfg.readListOption("general::regions") ## the regions that are combined with the previous
bkgList    = cfg.readListOption("general::backgrounds")

#dataList   = cfg.readListOption("general::data")
selList    = [x[0] + '_' + x[1] for x in list(itertools.product(selDefList, regDefList))]


colors = [kRed,
          kBlue+2,
          kOrange+7,
          kBlue,
          kViolet
]

colorsComp = [
kBlue,
kRed,
kGreen,
kCyan
]

namesComp = ['A - OS iso','B - SS iso','C - OS antiIso','D - SS antiIso']

inFile = TFile.Open(fileName)


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


for var in vars:
    for i in range(len(selList)):

        sel = selList[i]
        if "SSVBF" in sel: continue
        hBkg[sel] = sumBkg(inFile, var, sel,bkgList)
        hBkg[sel][0].SetLineWidth(4)
        hQCD[sel] = subtractBkg(inFile, var, sel, hBkg)
        hQCD[sel].SetLineWidth(2)
        hQCD[sel].Rebin(2)

        
    canvas = TCanvas("c1","c1",600,600)

    print ' --- VAR: ', var
    #print regDefList
    #print "#"
    #print selList

    if isIsoBin:
        for reg in isoHist:
            print reg
            
            leg = TLegend(0.4, 0.5,0.89,0.89)
            i = 0
            for sel in isoSelHist:
                #if "btag" in sel: continue

                reg_sel = "isoDau2_"+sel+"_"+reg

                if hQCD[reg_sel].Integral() < 0.1: continue
                hQCD[reg_sel].SetLineColor(colors[i])
                norm = 1./hQCD[reg_sel].Integral()
                hQCD[reg_sel].Scale(norm)
                if i == 0:
                    hQCD[reg_sel].GetXaxis().SetTitle(var)
                    hQCD[reg_sel].GetYaxis().SetTitle("Normalized events")
                    hQCD[reg_sel].GetYaxis().SetTitleOffset(1.4)
                    hQCD[reg_sel].SetMaximum(0.50)
                    hQCD[reg_sel].Draw("hist")
                else:
                    hQCD[reg_sel].Draw("hist same")

                i += 1
                canvas.Modified()
                canvas.Update()

                leg.AddEntry(hQCD[reg_sel], hQCD[reg_sel].GetTitle(),"l")
            leg.Draw()
            canvas.Modified()
            canvas.Update()
            #raw_input()
            canvas.SaveAs(inDir+reg+"_"+var+"_QCD.pdf")
            canvas.SaveAs(inDir+reg+"_"+var+"_QCD.png")

    if isMakeComparison:

        leg = TLegend(0.4, 0.5,0.89,0.89)
        i = 0

        for reg in comparHisto:
            print reg

            if hQCD[reg].Integral() < 0.1: continue
            hQCD[reg].SetLineColor(colorsComp[i])
            norm = 1./hQCD[reg].Integral()
            hQCD[reg].Scale(norm)
            if i == 0:
                hQCD[reg].GetXaxis().SetTitle(var)
                hQCD[reg].GetYaxis().SetTitle("Normalized events")
                hQCD[reg].GetYaxis().SetTitleOffset(1.4)
                hQCD[reg].SetMaximum(0.70)
                hQCD[reg].Draw("hist")
            else:
                hQCD[reg].Draw("hist same")

            canvas.Modified()
            canvas.Update()
            leg.AddEntry(hQCD[reg], namesComp[i], "l")
            i += 1

        leg.Draw()
        canvas.Modified()
        canvas.Update()
        #raw_input()
        canvas.SaveAs(inDir+"ABCD_base/"+var+"_ABCD.pdf")


    if isMakeRatio:
       makeRatio(ratioHist[0],ratioHist[1],hQCD,canvas)
       #raw_input()
    

