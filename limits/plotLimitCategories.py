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

def parseFile(filename, CL='50.0', exp=True):
    f = open(filename)
    matches = []
    for line in f:
        search = ('Expected %s%%: r <'%CL)
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

def parseROOTFile(filename, CL=0.5):
    f = ROOT.TFile(filename,"read")
    ttree = f.Get("limit")

    res = -1.0
    for i,ev in enumerate(ttree):
        if ev.quantileExpected != CL: continue
        else: res = ev.limit

    return res

def getXStheoGGF (kL):
    # Updated following: https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHWGHH?redirectedfrom=LHCPhysics.LHCHXSWGHH#Latest_recommendations_for_gluon
    # (old recommendation was A = 62.5339 / B = -44.323 / C = 9.6340)
    A = 70.3874
    B = -50.4111
    C = 11.0595

    val = A + B*kL + C*kL*kL
    return val

def getXStheoVBF (c2v,KL,year): 
    C2V = c2v
    kl = KL
    CV = 1.0

    #s1 = HHModel.VBF_sample_list[0].val_xs # VBFHHSample(1,1,1,   val_xs = 0.00054/(0.3364), label = 'qqHH_CV_1_C2V_1_kl_1'  ),
    #s2 = HHModel.VBF_sample_list[1].val_xs # VBFHHSample(1,2,1,   val_xs = 0.00472/(0.3364), label = 'qqHH_CV_1_C2V_2_kl_1'  ),
    #s3 = HHModel.VBF_sample_list[2].val_xs # VBFHHSample(1,1,2,   val_xs = 0.00044/(0.3364), label = 'qqHH_CV_1_C2V_1_kl_2'  ),
    #s4 = HHModel.VBF_sample_list[3].val_xs # VBFHHSample(1,1,0,   val_xs = 0.00145/(0.3364), label = 'qqHH_CV_1_C2V_1_kl_0'  ),
    #s5 = HHModel.VBF_sample_list[4].val_xs # VBFHHSample(0.5,1,1, val_xs = 0.00353/(0.3364), label = 'qqHH_CV_0p5_C2V_1_kl_1'),
    #s6 = HHModel.VBF_sample_list[5].val_xs # VBFHHSample(1.5,1,1, val_xs = 0.02149/(0.3364), label = 'qqHH_CV_1p5_C2V_1_kl_1'),

    if year == '2016':
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

year = "2018" # 2016 2017 2018


if   year == "2016":
    tag = [year,"DNNoutSM_kl_1","2016 - 35.9"]
elif year == "2017":
    tag = [year,"DNNoutSM_kl_1","2017 - 41.5"]
else:  #year == "2018"
    tag = [year,"DNNoutSM_kl_1","2018 - 59.7"]

colors = [ROOT.kBlue, ROOT.kRed, ROOT.kCyan, ROOT.kBlack]

channels = ['res1b','res2b','boost','VBFcomb']
names    = ['res1b','res2b','boosted','VBFcomb']

lambdas = [x for x in range(1, 82)]
print lambdas

klval = [-20  , -19.5, -19  , -18.5, -18  , -17.5, -17  , -16.5, -16  , -15.5, -15  , -14.5, -14  , -13.5, -13  , -12.5, -12  , -11.5, -11  , -10.5, -10  , -9.5 , -9   , -8.5 , -8   , -7.5 , -7   , -6.5 , -6   , -5.5 , -5   , -4.5 , -4   , -3.5 , -3   , -2.5 , -2   , -1.5 , -1   , -0.5 , 0    , 0.5  , 1    , 1.5  , 2    , 2.5  , 3    , 3.5  , 4    , 4.5  , 5    , 5.5  , 6    , 6.5  , 7    , 7.5  , 8    , 8.5  , 9    , 9.5  , 10   , 10.5 , 11   , 11.5 , 12   , 12.5 , 13   , 13.5 , 14   , 14.5 , 15   , 15.5 , 16   , 16.5 , 17   , 17.5 , 18   , 18.5 , 19   , 19.5 , 20]
print klval

graphs = []

for i,channel in enumerate(channels):
    print '--- Doing category {0} ---'.format(names[i])

    grexp = ROOT.TGraph()
    ptsList = [] # (x, exp)

    for ipt,ikl in enumerate(klval):
        #fName = 'datacards_'+year+'_'+channel+'/m125.0/r__kl/test1/limit__r__kl_{0}.0.root'.format(str(ikl))
        fName = 'datacards_'+year+'_'+channel+'/out_Asym_{0}_noTH.log'.format(lambdas[ipt])

        xval = ikl

        corrFactor = 1.034772182
        if year == "2016": 
            corrFactor = 1.078076202

        xstheoVBF = getXStheoVBF (1,xval,year) * corrFactor   # C2V,kl,year
        xstheoGGF = getXStheoGGF (xval)                       # kl
        xstheoTOT = xstheoVBF + xstheoGGF

        # Can get different results on r_gghh:
        #exp = parseFile(fName)                      # <- How many times the SM I'm excluding
        #exp = parseFile(fName) * xstheoTOT          # <- Excluded HH cross section
        exp = parseFile(fName)  * xstheoTOT * 0.073  # <- Excluded HH cross section times BR(bbtautau)

        # read directly from root file
        #exp = parseROOTFile(fName)  * xstheoTOT * 0.073  # <- Excluded HH cross section times BR(bbtautau)

        ptsList.append((xval, exp))

    ptsList.sort()
    for ipt, pt in enumerate(ptsList):
        xval = pt[0]
        exp  = pt[1]
        print xval, exp
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
pt3.AddText("Combined channels")


hframe = ROOT.TH1F('hframe', '', 100, -22, 22)
#hframe = ROOT.TH1F('hframe', '', 100, -5, 5)
hframe.SetMinimum(0.1)
if year == "2016" or year=="2017" : hframe.SetMaximum(900)
else                              : hframe.SetMaximum(900)
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

c1.Print('../plots/v4/comparison_categoriesHH_'+year+'_NEW.pdf','pdf')

import pdb; pdb.set_trace()
