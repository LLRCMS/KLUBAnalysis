import re, optparse
import os.path
from math import *
import ROOT

#####

def redrawBorder():
   # this little macro redraws the axis tick marks and the pad border lines.
   ROOT.gPad.Update()
   ROOT.gPad.RedrawAxis()
   l = ROOT.TLine()
   l.SetLineWidth(3)
   l.DrawLine(ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymax(), ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymax())
   l.DrawLine(ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymin(), ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymax())
   l.DrawLine(ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymin(), ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymax())
   l.DrawLine(ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymin(), ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymin())

def getExpValue( kl,  yt): 
    BR =1 
    return (2.09*yt*yt*yt*yt +   0.28*yt*yt*kl*kl  -1.37*yt*yt*yt*kl)*2.44477/BR

def parseFile(filename, CL='50.0', exp=True):
    f = open(filename)
    matches = []
    for line in f:
        search = ('Expected %s%%: r_gghh <'%CL)
        if not exp: search = 'Observed Limit: r_gghh <'

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

def getXStheoGGF (kL):
    # Updated following: https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHWGHH?redirectedfrom=LHCPhysics.LHCHXSWGHH#Latest_recommendations_for_gluon
    # (old recommendation was A = 62.5339 / B = -44.323 / C = 9.6340)
    A = 70.3874
    B = -50.4111
    C = 11.0595

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

var = 'DNNoutSM_kl_1'

#year = '2018'
#tag = 'CombChan_'+year+'_27Nov2020'
year = '2018'
tag = 'CombAll_27Nov2020'

selections = ["comb_cat"]

lambdas = [x for x in range(1, 42)]
print lambdas

klval = [-20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]
print klval


### read the scan with normal width
for sel in selections:

    gr2sigma = ROOT.TGraphAsymmErrors()
    gr1sigma = ROOT.TGraphAsymmErrors()
    grexp = ROOT.TGraph()
    grobs = ROOT.TGraph()

    ptsList = [] # (x, obs, exp, p2s, p1s, m1s, m2s)

    for ipt in range(0, len(lambdas)):
        if 'Chan' in tag:
            fName = 'cards_'+tag+'/out_Asym_GGF{0}_noTH.log'.format(lambdas[ipt])
        elif 'All' in tag:
            fName = 'cards_CombAll_27Nov2020_autoMC1/out_Asym_GGF{0}_noTH.log'.format(lambdas[ipt])
        else:
            #fName = 'cards_Combined_2019_10_11/ggHH_bbtt{0}BDToutSM_kl_{1}/out_Asym_ggHH_bbtt{0}_noTH.log'.format(lambdas[ipt], klval[ipt])
            #fName = 'cards_Combined_2017_03_10_lmr70/ggHH_bbtt{0}MT2/out_Asym_ggHH_bbtt{0}_noTH.log'.format(lambdas[ipt])
            fName = 'cards_TauTau1Sept_noShape_GGF/{0}'.format(sel)+var+'/out_Asym_{0}_noTH.log'.format(lambdas[ipt])

        xval = klval[ipt]

        corrFactor = 1.034772182
        if year == "2016":
            corrFactor = 1.078076202

        xstheoGGF = getXStheoGGF (xval)

        # Can get different results on r_gghh:
        #exp  = parseFile(fName)                                   # <- How many times the SM I'm excluding
        #exp  = parseFile(fName)            * xstheoGGF            # <- Excluded HH cross section
        exp   = parseFile(fName)            * xstheoGGF # * 0.073  # <- Excluded HH cross section times BR(bbtautau)
        obs   = parseFile(fName, exp=False) * xstheoGGF # * 0.073
        m1s_t = parseFile(fName, CL='16.0') * xstheoGGF # * 0.073
        p1s_t = parseFile(fName, CL='84.0') * xstheoGGF # * 0.073
        m2s_t = parseFile(fName, CL=' 2.5') * xstheoGGF # * 0.073
        p2s_t = parseFile(fName, CL='97.5') * xstheoGGF # * 0.073

        ## because the other code wants +/ sigma vars as deviations, without sign, from the centeal exp value...
        p2s = p2s_t - exp
        p1s = p1s_t - exp
        m2s = exp - m2s_t
        m1s = exp - m1s_t

        ptsList.append((xval, obs, exp, p2s, p1s, m1s, m2s))

    ptsList.sort()
    for ipt, pt in enumerate(ptsList):
        xval = pt[0]
        obs  = pt[1]
        exp  = pt[2]
        p2s  = pt[3]
        p1s  = pt[4]
        m1s  = pt[5]
        m2s  = pt[6]
        print xval, exp
        grexp.SetPoint(ipt, xval, exp)
        grobs.SetPoint(ipt, xval, obs)
        gr1sigma.SetPoint(ipt, xval, exp)
        gr2sigma.SetPoint(ipt, xval, exp)
        gr1sigma.SetPointError(ipt, 0,0,m1s,p1s)
        gr2sigma.SetPointError(ipt, 0,0,m2s,p2s)


    ######## set styles
    grexp.SetMarkerStyle(24)
    grexp.SetMarkerColor(4)
    grexp.SetMarkerSize(0.8)
    grexp.SetLineColor(ROOT.kBlue+2)
    grexp.SetLineWidth(3)
    grexp.SetLineStyle(2)
    grexp.SetFillColor(0)

    grobs.SetLineColor(1)
    grobs.SetLineWidth(3)
    grobs.SetMarkerColor(1)
    grobs.SetMarkerStyle(20)
    grobs.SetFillStyle(0)

    gr1sigma.SetMarkerStyle(0)
    gr1sigma.SetMarkerColor(3)
    gr1sigma.SetFillColor(ROOT.kGreen+1)
    gr1sigma.SetLineColor(ROOT.kGreen+1)
    gr1sigma.SetFillStyle(1001)

    gr2sigma.SetMarkerStyle(0)
    gr2sigma.SetMarkerColor(5)
    gr2sigma.SetFillColor(ROOT.kOrange)
    gr2sigma.SetLineColor(ROOT.kOrange)
    gr2sigma.SetFillStyle(1001)

    mg.Add(gr2sigma, "3")
    mg.Add(gr1sigma, "3")
    mg.Add(grexp, "L")
    mg.Add(grobs, "L")

    ###########
    legend = ROOT.TLegend(0,0,0,0)
    legend.SetX1(0.17284)
    legend.SetY1(0.530526)
    legend.SetX2(0.520062)
    legend.SetY2(0.88)
    legend.SetFillColor(ROOT.kWhite)
    legend.SetBorderSize(0)
    legend.SetHeader('95% CL upper limits')
    legend.AddEntry(grobs,"Observed","l")
    legend.AddEntry(grexp, "Median expected", "l")
    legend.AddEntry(gr1sigma, "68% expected", "f")
    legend.AddEntry(gr2sigma, "95% expected", "f")

    fakePlot3 = ROOT.TGraphAsymmErrors()
    fakePlot3.SetFillColor(ROOT.kRed)
    fakePlot3.SetFillStyle(3001)
    fakePlot3.SetLineColor(ROOT.kRed)
    fakePlot3.SetLineWidth(3)
    legend.AddEntry(fakePlot3, "Theoretical prediction", "lf")


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

    pt4 = ROOT.TPaveText(0.4819196+0.036,0.7780357+0.015+0.02,0.9008929+0.036,0.8675595+0.015,"brNDC")
    pt4.SetTextAlign(12)
    pt4.SetFillColor(ROOT.kWhite)
    pt4.SetFillStyle(1001)
    pt4.SetTextFont(42)
    pt4.SetTextSize(0.05)
    pt4.SetBorderSize(0)
    pt4.SetTextAlign(32)
    pt4.AddText("HH GGF production")
    pt4.AddText("HH #rightarrow bb#tau#tau")

    ##### theory lines
    xmin=-20
    xmax=+20

    yt=1
    BR = 1

    # 2016 data
    #myFunc =  ROOT.TF1("myFunc","(2.09*[0]*[0]*[0]*[0] + 0.28*[0]*[0]*x*[0]*x*[0] -1.37*[0]*[0]*[0]*x*[0])*2.44185/[1]",xmin,xmax)

    # Run2 GGF - Updated following: https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHWGHH?redirectedfrom=LHCPhysics.LHCHXSWGHH#Latest_recommendations_for_gluon
    myFunc =  ROOT.TF1("myFunc","(70.3874 -50.4111*x + 11.0595*x*x)",xmin,xmax)

    graph = ROOT.TGraph(myFunc)
    ci = ROOT.TColor.GetColor("#ff0000")
    graph.SetLineColor(ci)
    graph.SetLineWidth(2)
    nP = int((xmax-xmin)*10.0)
    Graph_syst_Scale =  ROOT.TGraphAsymmErrors(nP)
    for i in range(nP) : 

        kl_x = xmin+(i*1.)/10.
        Graph_syst_Scale_x=(kl_x)
        Graph_syst_Scale_x_err=(0)

        # Graph_syst_Scale_y=(getExpValue(kl_x,yt)) # 2016 data
        xstheoGGF = getXStheoGGF (kl_x)             # Run2
        Graph_syst_Scale_y=(xstheoGGF)

        # 2016 uncertainties
        #Graph_syst_Scale_y_errup=(  (2.09*yt*yt*yt*yt+0.28*yt*yt*(kl_x)*(kl_x)-1.37*yt*yt*yt*(kl_x))*2.44185*0.053/BR)
        #Graph_syst_Scale_y_errdown=((2.09*yt*yt*yt*yt+0.28*yt*yt*(kl_x)*(kl_x)-1.37*yt*yt*yt*(kl_x))*2.44185*0.067/BR)

        # Run2 uncertainties HH GGF
        # Updated following: https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHWGHH?redirectedfrom=LHCPhysics.LHCHXSWGHH#Latest_recommendations_for_gluon
        # upper_unc[kl] = Max[72.0744-51.7362*kl+11.3712*kl2, 70.9286-51.5708*kl+11.4497*kl2] in fb.
        # lower_unc[kl] = Min[66.0621-46.7458*kl+10.1673*kl2, 66.7581-47.721*kl+10.4535*kl2] in fb.
        # pdf+alpha_s   = 0.03
        # mtop          = 0.026
        scale_up   = (max(72.0744-51.7362*kl_x+11.3712*kl_x*kl_x,70.9286-51.5708*kl_x+11.4497*kl_x*kl_x) - xstheoGGF) / xstheoGGF
        scale_down = (min(66.0621-46.7458*kl_x+10.1673*kl_x*kl_x,66.7581-47.721*kl_x+10.4535*kl_x*kl_x)  - xstheoGGF) / xstheoGGF
        GGF_erry_up   = xstheoGGF*((scale_up*scale_up + 0.03*0.03 + 0.026*0.026)**0.5)
        GGF_erry_down = xstheoGGF*((scale_down*scale_down + 0.03*0.03 + 0.026*0.026)**0.5)

        # Run2 total uncertainty (GGF+VBF)
        Graph_syst_Scale_y_errup  = (GGF_erry_up)
        Graph_syst_Scale_y_errdown= (GGF_erry_down)

        Graph_syst_Scale.SetPoint(i,Graph_syst_Scale_x,Graph_syst_Scale_y)
        Graph_syst_Scale.SetPointError(i,Graph_syst_Scale_x_err,Graph_syst_Scale_x_err,Graph_syst_Scale_y_errdown,Graph_syst_Scale_y_errup)

    Graph_syst_Scale.SetLineColor(ROOT.kRed)
    Graph_syst_Scale.SetFillColor(ROOT.kRed)
    Graph_syst_Scale.SetFillStyle(3001)
    #graph.Print()

    hframe = ROOT.TH1F('hframe', '', 100, -22, 22)
    hframe.SetMinimum(0.1)
    if '2016' in tag:
        hframe.SetMaximum(3000)
    elif '2017' in tag:
        hframe.SetMaximum(3000)
    elif '2018' in tag:
        hframe.SetMaximum(3000)
    else:
        hframe.SetMaximum(2500)

    hframe.GetYaxis().SetTitleSize(0.047)
    hframe.GetXaxis().SetTitleSize(0.055)
    hframe.GetYaxis().SetLabelSize(0.045)
    hframe.GetXaxis().SetLabelSize(0.045)
    hframe.GetXaxis().SetLabelOffset(0.012)
    hframe.GetYaxis().SetTitleOffset(1.2)
    hframe.GetXaxis().SetTitleOffset(1.1)

    hframe.GetYaxis().SetTitle("95% CL on #sigma_{ggF} (pp#rightarrow HH) [fb]")
    hframe.GetXaxis().SetTitle("k_{#lambda}")

    hframe.SetStats(0)
    ROOT.gPad.SetTicky()
    hframe.Draw()

    gr2sigma.Draw("3same")
    gr1sigma.Draw("3same")
    grexp.Draw("Lsame")
    #grobs.Draw("Lsame")

    graph.Draw("l same")
    Graph_syst_Scale.Draw("e3 same")

    pt.Draw()
    pt2.Draw()
    redrawBorder()
    c1.Update()
    c1.RedrawAxis("g")
    legend.Draw()
    pt4.Draw()
    c1.Update()

    c1.Print("plots/v3/klscanGGF_"+tag+"_theor.pdf", 'pdf')

import pdb; pdb.set_trace()

##### print a YAML format for HepDATA
#print "\n\nHEP DATA - x variable"
#for ipt in range(0, grexp.GetN()):
#    x_exp = ROOT.Double(0.0)
#    y_exp = ROOT.Double(0.0)
#
#    x_obs = ROOT.Double(0.0)
#    y_obs = ROOT.Double(0.0)
#
#    grexp.GetPoint(ipt, x_exp, y_exp)
#    grobs.GetPoint(ipt, x_obs, y_obs)
#    ey_p2s = gr2sigma.GetErrorYhigh(ipt)
#    ey_p1s = gr1sigma.GetErrorYhigh(ipt)
#    ey_m1s = gr1sigma.GetErrorYlow(ipt)
#    ey_m2s = gr2sigma.GetErrorYlow(ipt)
#    print "- value: %f" % x_exp
#
#print "\n\nHEP DATA - exp"
#for ipt in range(0, grexp.GetN()):
#    x_exp = ROOT.Double(0.0)
#    y_exp = ROOT.Double(0.0)
#
#    x_obs = ROOT.Double(0.0)
#    y_obs = ROOT.Double(0.0)
#
#    grexp.GetPoint(ipt, x_exp, y_exp)
#    grobs.GetPoint(ipt, x_obs, y_obs)
#    ey_p2s = gr2sigma.GetErrorYhigh(ipt)
#    ey_p1s = gr1sigma.GetErrorYhigh(ipt)
#    ey_m1s = gr1sigma.GetErrorYlow(ipt)
#    ey_m2s = gr2sigma.GetErrorYlow(ipt)
#    print "- value: %f" % y_exp
#    print "  errors:"
#    print "  - asymerror: {minus: %f,  plus: %f,  label: '1 sigma'}" % (ey_m1s, ey_p1s)
#    print "  - asymerror: {minus: %f,  plus: %f,  label: '2 sigma'}" % (ey_m2s, ey_p2s)
#
#print "\n\nHEP DATA - obs"
#for ipt in range(0, grexp.GetN()):
#    x_exp = ROOT.Double(0.0)
#    y_exp = ROOT.Double(0.0)
#
#    x_obs = ROOT.Double(0.0)
#    y_obs = ROOT.Double(0.0)
#
#    grexp.GetPoint(ipt, x_exp, y_exp)
#    grobs.GetPoint(ipt, x_obs, y_obs)
#    ey_p2s = gr2sigma.GetErrorYhigh(ipt)
#    ey_p1s = gr1sigma.GetErrorYhigh(ipt)
#    ey_m1s = gr1sigma.GetErrorYlow(ipt)
#    ey_m2s = gr2sigma.GetErrorYlow(ipt)
#    print "- value: %f" % y_obs
