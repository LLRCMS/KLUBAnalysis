import re, optparse
import os.path
from math import *
# from ROOT import *
import ROOT
import HHModel


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
        search = ('Expected %s%%: r_qqhh <'%CL)
        if not exp: search = 'Observed Limit: r <'

        if not search in line:
            continue
        val = line.replace(search, '')
        val = float(val)
        matches.append(val)

    if len(matches) == 0:
        print "did not find any expected in file: " , filename, 'CL=', CL, 'exp?=', exp
        return -1.0
    else:
        return matches[-1]

def getXStheo (c2v,year): 
    C2V = c2v
    kl = 1.0
    CV = 1.0

    #s1 = HHModel.VBF_sample_list[0].val_xs # VBFHHSample(1,1,1,   val_xs = 0.00054/(0.3364), label = 'qqHH_CV_1_C2V_1_kl_1'  ),
    #s2 = HHModel.VBF_sample_list[1].val_xs # VBFHHSample(1,2,1,   val_xs = 0.00472/(0.3364), label = 'qqHH_CV_1_C2V_2_kl_1'  ),
    #s3 = HHModel.VBF_sample_list[2].val_xs # VBFHHSample(1,1,2,   val_xs = 0.00044/(0.3364), label = 'qqHH_CV_1_C2V_1_kl_2'  ),
    #s4 = HHModel.VBF_sample_list[3].val_xs # VBFHHSample(1,1,0,   val_xs = 0.00145/(0.3364), label = 'qqHH_CV_1_C2V_1_kl_0'  ),
    #s5 = HHModel.VBF_sample_list[4].val_xs # VBFHHSample(0.5,1,1, val_xs = 0.00353/(0.3364), label = 'qqHH_CV_0p5_C2V_1_kl_1'),
    #s6 = HHModel.VBF_sample_list[5].val_xs # VBFHHSample(1.5,1,1, val_xs = 0.02149/(0.3364), label = 'qqHH_CV_1p5_C2V_1_kl_1'),

    if year == 2016:
        s1 = 0.001601
        s2 = 0.01335
        s3 = 0.001327
        s4 = 0.004259
        s5 = 0.01009
        s6 = 0.06153
    else:
        s1 = 0.001668
        s2 = 0.01374
        s3 = 0.001375
        s4 = 0.004454
        s5 = 0.01046
        s6 = 0.0638

    val = s1*(-3.3*C2V**2 + 1.3*C2V*CV**2 + 7.6*C2V*CV*kl + 2.0*CV**4 - 5.6*CV**3*kl - 1.0*CV**2*kl**2) + s2*(1.5*C2V**2 + 0.5*C2V*CV**2 - 4.0*C2V*CV*kl - 2.0*CV**4 + 4.0*CV**3*kl) + s3*(0.35*C2V**2 - 0.0166666666666667*C2V*CV**2 - 1.03333333333333*C2V*CV*kl - 0.333333333333333*CV**4 + 0.533333333333333*CV**3*kl + 0.5*CV**2*kl**2) + s4*(-0.45*C2V**2 + 0.45*C2V*CV**2 + 0.9*C2V*CV*kl + 1.0*CV**4 - 2.4*CV**3*kl + 0.5*CV**2*kl**2) + s5*(-2.0*C2V**2 - 3.33333333333333*C2V*CV**2 + 9.33333333333333*C2V*CV*kl + 5.33333333333333*CV**4 - 9.33333333333333*CV**3*kl) + s6*(0.4*C2V**2 - 0.4*C2V*CV**2 - 0.8*C2V*CV*kl + 0.8*CV**3*kl)

    return val


c1 = ROOT.TCanvas("c1", "c1", 650, 500)
c1.SetFrameLineWidth(3)
c1.SetBottomMargin (0.15)
c1.SetRightMargin (0.05)
c1.SetLeftMargin (0.15)
c1.SetGridx()
c1.SetGridy()

mg = ROOT.TMultiGraph()

tag = "2018_5Oct2020"

channels = ['TauTau','MuTau','ETau']
colors = [ROOT.kBlack, ROOT.kBlue, ROOT.kRed]
names = ['TauTau', 'MuTau', 'ETau']


klval = [-5.5, -5  , -4.5, -4  , -3.5, -3  , -2.5, -2  , -1.5, -1  , -0.5, 0  ,0.5,1  ,1.5,2  ,2.5,3  ,3.5,4  ,4.5,5  ,5.5]
print klval

lambdas = [x for x in range(1, len(klval)+1)]
print lambdas

graphs = []

for i,chan in enumerate(channels):
    print '--- Doing channel {0} ---'.format(names[i])

    grexp = ROOT.TGraph()
    ptsList = [] # (x, exp)

    for ipt in range(0, len(lambdas)):
        fName = 'cards_'+chan+tag+'/comb_cat/out_Asym_{0}_noTH.log'.format(lambdas[ipt])

        corrFactor = 1.034772182
        year = 2018
        if '2016' in tag: 
            corrFactor = 1.078076202
            year = 2016

        # Can get different results on r_qqhh:
        #exp = parseFile(fName)                                           # <- How many times the SM I'm excluding
        #exp = parseFile(fName) * getXStheo(klval[ipt]) * 1000.0          # <- Excluded HH cross section [fb]
        #exp = parseFile(fName) * getXStheo(klval[ipt]) * 1000.0 * 0.073  # <- Excluded HH cross section times BR(bbtautau) [fb]

        exp   = parseFile(fName) * getXStheo(klval[ipt],year) * corrFactor * 1000.0

        xval = klval[ipt]
        ptsList.append((xval, exp))

    ptsList.sort()
    for ipt, pt in enumerate(ptsList):
        xval = pt[0]
        exp  = pt[1]
        print xval, exp, ' [fb]'
        grexp.SetPoint(ipt, xval, exp)

    ######## set styles
    grexp.SetMarkerStyle(8)
    grexp.SetMarkerColor(colors[i])
    grexp.SetLineColor(colors[i])
    grexp.SetLineWidth(1)

    graphs.append(grexp)


######## set styles

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
if '2016' in tag:
    pt2.AddText("2016 - 35.9 fb^{-1} (13 TeV)")
elif '2017' in tag:
    pt2.AddText("2017 - 41.6 fb^{-1} (13 TeV)")
elif '2018' in tag:
    pt2.AddText("2018 - 59.7 fb^{-1} (13 TeV)")
else:
    pt2.AddText("Run2 - 137.1 fb^{-1} (13 TeV)")


pt3 = ROOT.TPaveText(0.6819196+0.036,0.7780357+0.015+0.02,0.9008929+0.036,0.8675595+0.015,"brNDC")
pt3.SetTextAlign(12)
pt3.SetFillColor(ROOT.kWhite)
pt3.SetFillStyle(1001)
pt3.SetTextFont(42)
pt3.SetTextSize(0.05)
pt3.SetBorderSize(0)
pt3.SetTextAlign(32)
pt3.AddText("VBF-HH Comb. cat.")

#hframe = ROOT.TH1F('hframe', '', 100, -22, 22)
hframe = ROOT.TH1F('hframe', '', 100, -5, 6)
hframe.SetMinimum(100)
hframe.SetMaximum(8000)

hframe.GetYaxis().SetTitleSize(0.047)
hframe.GetXaxis().SetTitleSize(0.055)
hframe.GetYaxis().SetLabelSize(0.045)
hframe.GetXaxis().SetLabelSize(0.045)
hframe.GetXaxis().SetLabelOffset(0.012)
hframe.GetYaxis().SetTitleOffset(1.2)
hframe.GetXaxis().SetTitleOffset(1.1)
#hframe.GetYaxis().SetTitle("95% CL on #sigma #times #bf{#it{#Beta}}(HH#rightarrow bb#tau#tau) [fb]")
hframe.GetYaxis().SetTitle("95% CL on #sigma_{VBF} (pp#rightarrow HH) [fb]")
hframe.GetXaxis().SetTitle("C_{2V}")
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
c1.SetLogy()
c1.RedrawAxis("g")

leg.Draw()
c1.Update()

c1.Print("plots/comparison_Channels_"+tag+".pdf", 'pdf')

import pdb; pdb.set_trace()
