import re, optparse
import os.path
from math import *
# from ROOT import *
import ROOT

#####

def redrawBorder():
   # this little macro redraws the axis tick marks and the pad border lines.
   ROOT.gPad.Update();
   ROOT.gPad.RedrawAxis();
   l = ROOT.TLine()
   l.SetLineWidth(3)
   l.DrawLine(ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymax(), ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymax());
   l.DrawLine(ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymin(), ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymax());
   l.DrawLine(ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymin(), ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymax());
   l.DrawLine(ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymin(), ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymin());

def getExpValue( kl,  yt): 
    BR =1 
    return (2.09*yt*yt*yt*yt +   0.28*yt*yt*kl*kl  -1.37*yt*yt*yt*kl)*2.44477/BR;


def parseFile(filename, CL='50.0', exp=True):
    f = open(filename)
    matches = []
    for line in f:
        search = ('Expected %s%%: r_gghh <'%CL)
        if not exp: search = 'Observed Limit: r <'

        if not search in line:
            continue
        val = line.replace(search, '')
        val = float(val)
        matches.append(val)

    if len(matches) == 0:
        print "did not find any expected in file: " , filename, 'CL=', CL, 'exp?=', exp
        #return -1.0
        return 0
    else:
        return matches[-1]

def getXStheo (kL): 
    A = 62.5339
    B = -44.323
    C = 9.6340

    val = A + B*kL + C*kL*kL
    return val


c1 = ROOT.TCanvas("c1", "c1", 650, 500)
c1.SetFrameLineWidth(3)
c1.SetBottomMargin (0.15)
c1.SetRightMargin (0.05)
c1.SetLeftMargin (0.15)
c1.SetGridx()
c1.SetGridy()

mg = ROOT.TMultiGraph()

category = "comb_cat" # sboostedLLMcut  s1b1jresolvedMcut  s2b0jresolvedMcut  VBFloose
year = "2016" # 2016 2017 2018
tagName = "2Oct2020"


if   year == "2016":
    tag = [year+"_"+tagName,"DNNoutSM_kl_1","2016 - 35.9"]
elif year == "2017":
    tag = [year+"_"+tagName,"DNNoutSM_kl_1","2017 - 41.5"]
else:  #year == "2018"
    tag = [year+"_"+tagName,"DNNoutSM_kl_1","2018 - 59.7"]

colors = [ROOT.kBlack, ROOT.kBlue, ROOT.kRed, ROOT.kCyan]

channels = ['TauTau','MuTau','ETau']
names    = ['TauTau','MuTau','ETau']

lambdas = [x for x in range(1, 42)]
print lambdas

klval = [-20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]
print klval

graphs = []

for i,channel in enumerate(channels):
    print '--- Doing channel {0} ---'.format(names[i])

    grexp = ROOT.TGraph()
    ptsList = [] # (x, exp)

    for ipt in range(0, len(lambdas)):
        if 'comb' in category:
            fName = 'cards_'+channel+tag[0]+'/'+category+'/out_Asym_{0}_noTH.log'.format(lambdas[ipt])
        else:
            fName = 'cards_'+channel+tag[0]+'/'+category+tag[1]+'/out_Asym_{0}_noTH.log'.format(lambdas[ipt])

        xval = klval[ipt]

        # Can get different results on r_gghh:
        #exp = parseFile(fName)                                          # <- How many times the SM I'm excluding
        #exp = parseFile(fName) * getXStheo(klval[ipt]) * 1.115          # <- Excluded HH cross section
        exp = parseFile(fName)  * getXStheo(klval[ipt]) * 1.115 * 0.073  # <- Excluded HH cross section times BR(bbtautau)

        ptsList.append((xval, exp))

    ptsList.sort()
    for ipt, pt in enumerate(ptsList):
        xval = pt[0]
        exp  = pt[1]
        #print xval, exp
        grexp.SetPoint(ipt, xval, exp)

    ######## set styles
    grexp.SetMarkerStyle(8)
    grexp.SetMarkerColor(colors[i])
    grexp.SetLineColor(colors[i])
    grexp.SetLineWidth(1)

    graphs.append(grexp)


leg = ROOT.TLegend(0,0,0,0)
leg.SetX1(0.17284)
leg.SetY1(0.630526)
leg.SetX2(0.420062)
leg.SetY2(0.88)
leg.SetFillColor(ROOT.kWhite)

for i,graph in enumerate(graphs):
    leg.AddEntry(graph, names[i], "pl")


##### text
pt = ROOT.TPaveText(0.1663218-0.02,0.886316,0.3045977-0.02,0.978947,"brNDC")
pt.SetBorderSize(0)
pt.SetTextAlign(12)
pt.SetTextFont(62)
pt.SetTextSize(0.05)
pt.SetFillColor(0)
pt.SetFillStyle(0)
pt.AddText("CMS #font[52]{Internal}" )

pt2 = ROOT.TPaveText(0.736111,0.9066667,0.847222,0.954641,"brNDC")
pt2.SetBorderSize(0)
pt2.SetFillColor(0)
pt2.SetTextSize(0.040)
pt2.SetTextFont(42)
pt2.SetFillStyle(0)
pt2.AddText(tag[2]+" fb^{-1} (13 TeV)")

pt3 = ROOT.TPaveText(0.6819196+0.036,0.7780357+0.015+0.02,0.9008929+0.036,0.8675595+0.015,"brNDC")
pt3.SetTextAlign(12)
pt3.SetFillColor(ROOT.kWhite)
pt3.SetFillStyle(1001)
pt3.SetTextFont(42)
pt3.SetTextSize(0.05)
pt3.SetBorderSize(0)
pt3.SetTextAlign(32)
if   category == "comb_cat"          : pt3.AddText("combined categories")
elif category == "sboostedLLMcut"    : pt3.AddText("boosted category")
elif category == "s1b1jresolvedMcut" : pt3.AddText("1b category")
elif category == "s2b0jresolvedMcut" : pt3.AddText("2b category")
elif category == "VBFloose"          : pt3.AddText("VBF category")
else                                 : pt3.AddText("?category not defined?")


hframe = ROOT.TH1F('hframe', '', 100, -22, 22)
#hframe = ROOT.TH1F('hframe', '', 100, -5, 5)
hframe.SetMinimum(0.1)
if   year == "2016"          : hframe.SetMaximum(600)
else                         : hframe.SetMaximum(500)
#if   category == "comb_cat"          : hframe.SetMaximum(15000)
#elif category == "sboostedLLMcut"    : hframe.SetMaximum(5000)
#elif category == "s1b1jresolvedMcut" : hframe.SetMaximum(2000)
#elif category == "s2b0jresolvedMcut" : hframe.SetMaximum(1000)
#elif category == "VBFloose"          : hframe.SetMaximum(5000)
#else                                 : hframe.SetMaximum(5000)

hframe.GetYaxis().SetTitleSize(0.047)
hframe.GetXaxis().SetTitleSize(0.055)
hframe.GetYaxis().SetLabelSize(0.045)
hframe.GetXaxis().SetLabelSize(0.045)
hframe.GetXaxis().SetLabelOffset(0.012)
hframe.GetYaxis().SetTitleOffset(1.2)
hframe.GetXaxis().SetTitleOffset(1.1)
hframe.GetYaxis().SetTitle("95% CL on #sigma #times #bf{#it{#Beta}}(HH#rightarrow bb#tau#tau) [fb]")
hframe.GetXaxis().SetTitle("k_{#lambda}")
hframe.SetStats(0)
ROOT.gPad.SetTicky()
hframe.Draw()


for i,graph in enumerate(graphs):
    graph.Draw("plsame")
pt.Draw()
pt2.Draw()
pt3.Draw()

redrawBorder()
c1.Update()
c1.RedrawAxis("g")

leg.Draw()
c1.Update()

c1.Print('plots/comparison_channels_'+category+'_'+tag[0]+'.pdf','pdf')

import pdb; pdb.set_trace()
