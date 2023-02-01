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

def getXStheoGGF (kL): 
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

#tag = 'TauTau2018_5Oct2020'
#tag = 'MuTau2018_5Oct2020'
#tag = 'ETau2018_5Oct2020'
tag = 'CombChan_2018_5Oct2020'

selections = ['comb_cat']

klval = [-5.5, -5  , -4.5, -4  , -3.5, -3  , -2.5, -2  , -1.5, -1  , -0.5, 0  ,0.5,1  ,1.5,2  ,2.5,3  ,3.5,4  ,4.5,5  ,5.5]
print klval

lambdas = [x for x in range(1, len(klval)+1)]
print lambdas

### read the scan with normal width
for sel in selections:

    gr2sigma = ROOT.TGraphAsymmErrors()
    gr1sigma = ROOT.TGraphAsymmErrors()
    grexp = ROOT.TGraph()
    grobs = ROOT.TGraph()

    ptsList = [] # (x, obs, exp, p2s, p1s, m1s, m2s)

    for ipt in range(0, len(lambdas)):
        if 'Chan' in tag:
            fName = 'cards_'+tag+'/out_Asym_{0}_noTH.log'.format(lambdas[ipt])
        else:
            fName = 'cards_'+tag+'/'+sel+'/out_Asym_{0}_noTH.log'.format(lambdas[ipt])

        corrFactor = 1.034772182
        year = 2018
        if '2016' in tag: 
            corrFactor = 1.078076202
            year = 2016

        # Can get different results on r_qqhh:
        #exp = parseFile(fName)                                           # <- How many times the SM I'm excluding
        #exp = parseFile(fName) * getXStheo(klval[ipt]) * 1000.0          # <- Excluded HH cross section [fb]
        #exp = parseFile(fName) * getXStheo(klval[ipt]) * 1000.0 * 0.073  # <- Excluded HH cross section times BR(bbtautau) [fb]

        exp   = parseFile(fName)            * getXStheo(klval[ipt],year) * corrFactor * 1000.0
        obs   = parseFile(fName, exp=False) * getXStheo(klval[ipt],year) * corrFactor * 1000.0
        m1s_t = parseFile(fName, CL='16.0') * getXStheo(klval[ipt],year) * corrFactor * 1000.0
        p1s_t = parseFile(fName, CL='84.0') * getXStheo(klval[ipt],year) * corrFactor * 1000.0
        m2s_t = parseFile(fName, CL=' 2.5') * getXStheo(klval[ipt],year) * corrFactor * 1000.0
        p2s_t = parseFile(fName, CL='97.5') * getXStheo(klval[ipt],year) * corrFactor * 1000.0

        #if klval[ipt] == 1: import pdb; pdb.set_trace()

        ## because the other code wants +/ sigma vars as deviations, without sign, from the centeal exp value...
        p2s = p2s_t - exp
        p1s = p1s_t - exp
        m2s = exp - m2s_t
        m1s = exp - m1s_t
        xval = klval[ipt]

        ptsList.append((xval, obs, exp, p2s, p1s, m1s, m2s))

    # print lambdasfiner

    # grexp.SetPoint(ipt, xval, exp)
    # grobs.SetPoint(ipt, xval, obs)
    # gr1sigma.SetPoint(ipt, xval, exp)
    # gr2sigma.SetPoint(ipt, xval, exp)
    # gr1sigma.SetPointError(ipt, 0,0,m1s,p1s)
    # gr2sigma.SetPointError(ipt, 0,0,m2s,p2s)
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
    # gr1sigma.SetFillColor(ROOT.TColor.GetColor(0, 220, 60))
    # gr1sigma.SetLineColor(ROOT.TColor.GetColor(0, 220, 60))
    gr1sigma.SetFillColor(ROOT.kGreen+1)
    gr1sigma.SetLineColor(ROOT.kGreen+1)
    gr1sigma.SetFillStyle(1001)

    # gr2sigma.SetName(tooLopOn[ic])
    gr2sigma.SetMarkerStyle(0)
    gr2sigma.SetMarkerColor(5)
    # gr2sigma.SetFillColor(ROOT.TColor.GetColor(254, 234, 0))
    # gr2sigma.SetLineColor(ROOT.TColor.GetColor(254, 234, 0))
    gr2sigma.SetFillColor(ROOT.kOrange)
    gr2sigma.SetLineColor(ROOT.kOrange)
    gr2sigma.SetFillStyle(1001)

    mg.Add(gr2sigma, "3")
    mg.Add(gr1sigma, "3")
    mg.Add(grexp, "L")
    mg.Add(grobs, "L")
    # mg.SetMinimum(0)
    # mg.SetMaximum(1000)
    # mg.Draw("A pmc plc")

    # mg.GetYaxis().SetTitleSize(0.047)
    # mg.GetXaxis().SetTitleSize(0.055)
    # mg.GetYaxis().SetLabelSize(0.045)
    # mg.GetXaxis().SetLabelSize(0.045)
    # mg.GetXaxis().SetLabelOffset(0.012)
    # mg.GetYaxis().SetTitleOffset(1.2)
    # mg.GetXaxis().SetTitleOffset(1.1)

    # mg.GetYaxis().SetTitle("95% CL on #sigma #times #bf{#it{#Beta}}(HH#rightarrow bb#tau#tau) [fb]")
    # mg.GetXaxis().SetTitle("k_{#lambda}/k_{t}")

    ###########
    legend = ROOT.TLegend(0,0,0,0)
    # legend.SetX1(0.17)
    # legend.SetY1(0.69)
    # legend.SetX2(0.466)
    # legend.SetY2(0.362-0.171+0.69)          

    legend.SetX1(0.17284)
    legend.SetY1(0.530526)
    # legend.SetX2(0.5)
    legend.SetX2(0.520062)
    legend.SetY2(0.88)


    legend.SetFillColor(ROOT.kWhite)
    legend.SetBorderSize(0)
    # legend
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
    # pt = ROOT.TPaveText(0.1663218,0.886316,0.3045977,0.978947,"brNDC")
    pt = ROOT.TPaveText(0.1663218-0.02,0.886316,0.3045977-0.02,0.978947,"brNDC")
    pt.SetBorderSize(0)
    pt.SetTextAlign(12)
    pt.SetTextFont(62)
    pt.SetTextSize(0.05)
    pt.SetFillColor(0)
    pt.SetFillStyle(0)
    pt.AddText("CMS #font[52]{Internal}" )
    #pt.AddText("CMS" )
    # pt.AddText("#font[52]{preliminary}")
    #pt2 = ROOT.TPaveText(0.79,0.9066667,0.8997773,0.957037,"brNDC")
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

    pt4 = ROOT.TPaveText(0.4819196+0.036,0.7780357+0.012,0.9008929+0.036,0.8675595+0.015,"brNDC")
    pt4.SetTextAlign(12)
    pt4.SetFillColor(ROOT.kWhite)
    pt4.SetFillStyle(1001)
    pt4.SetTextFont(42)
    pt4.SetTextSize(0.05)
    pt4.SetBorderSize(0)
    pt4.SetTextAlign(32)
    # pt4.AddText("bb #mu_{}#tau_{h} + bb e#tau_{h} + bb #tau_{h}#tau_{h}") 
    #pt4.AddText("bb #tau_{#mu}#tau_{h} + bb #tau_{e}#tau_{h} + bb #tau_{h}#tau_{h}") 
    #pt4.AddText("bb #tau_{e}#tau_{h}") 
    #pt4.AddText(" bb #tau_{h}#tau_{h}") 
    pt4.AddText("VBF-HH Comb. cat.")
    if 'TauTau' in tag:
        pt4.AddText("HH #rightarrow bb#tau_{h}#tau_{h}")
    elif 'MuTau' in tag:
        pt4.AddText("HH #rightarrow bb#tau_{#mu}#tau_{h}")
    elif 'ETau' in tag:
        pt4.AddText("HH #rightarrow bb#tau_{e}#tau_{h}")
    else:
        pt4.AddText("HH #rightarrow bb#tau#tau")


    offs = 0.020
    height = 0.05
    pt5 = ROOT.TPaveText(0.4819196+0.036+0.10,0.7780357+0.015-offs,0.9008929+0.036,0.7780357+0.015-offs+height,"brNDC")
    pt5.SetTextAlign(12)
    pt5.SetFillColor(ROOT.kWhite)
    pt5.SetFillStyle(1001)
    pt5.SetTextFont(42)
    pt5.SetTextSize(0.05)
    pt5.SetBorderSize(0)
    pt5.SetTextAlign(32)
    # pt5.AddText("bb #mu_{}#tau_{h} + bb e#tau_{h} + bb #tau_{h}#tau_{h}") 
    # pt5.AddText("bb #tau_{#mu}#tau_{h} + bb #tau_{e}#tau_{h} + bb #tau_{h}#tau_{h}") 
    pt5.AddText('#scale[0.8]{VBF multiclass}')

    #txt_kt1 = ROOT.TLatex(29.5, 390, "k_{t} = 1")
    #txt_kt1.SetTextAngle(60)
    #txt_kt1.SetTextAlign(31)
    #txt_kt1.SetTextSize(0.03)
    #txt_kt1.SetTextFont(42)
    #txt_kt1.SetTextColor(ROOT.kRed+1) #kGray+3
    #txt_kt1.Draw()

    #txt_kt2 = ROOT.TLatex(11, 450, "k_{t} = 2")
    #txt_kt2.SetTextAngle(80)
    #txt_kt2.SetTextAlign(31)
    #txt_kt2.SetTextSize(0.03)
    #txt_kt2.SetTextFont(42)
    #txt_kt2.SetTextColor(ROOT.kRed+2) #kGray+3
    #txt_kt2.Draw()


    ##### theory lines
    # xmin=-20.4
    # xmax=31.4
    xmin=-5.5
    xmax=+5.5
    CV = 1
    kl = 1
    yt=1
    BR = 1
    #myFunc =  ROOT.TF1("myFunc","(2.09*[0]*[0]*[0]*[0] + 0.28*[0]*[0]*x*[0]*x*[0] -1.37*[0]*[0]*[0]*x*[0])*2.44185/[1]",xmin,xmax)
    if '2016' in tag:
        myFunc =  ROOT.TF1("myFunc","( 0.001601*(-3.3*x*x + 1.3*x*[0]*[0] + 7.6*x*[0]*[1] + 2.0*[0]*[0]*[0]*[0] - 5.6*[0]*[0]*[0]*[1] - 1.0*[0]*0]*[1]*[1]) + 0.01335*(1.5*x*x + 0.5*x*[0]*[0] - 4.0*x*[0]*[1] - 2.0*[0]*[0]*[0]*[0] + 4.0*[0]*[0]*[0]*[1]) + 0.001327*(0.35*x*x - 0.0166666666666667*x*[0]*[0] - 1.03333333333333*x*[0]*[1] - 0.333333333333333*[0]*[0]*[0]*[0] + 0.533333333333333*[0]*[0]*[0]*[1] + 0.5*[0]*[0]*[1]*[1]) + 0.004259*(-0.45*x*x + 0.45*x*[0]*[0] + 0.9*x*[0]*[1] + 1.0*[0]*[0]*[0]*[0] - 2.4*[0]*[0]*[0]*[1] + 0.5*[0]*[0]*[1]*[1]) + 0.01009*(-2.0*x*x - 3.33333333333333*x*[0]*[0] + 9.33333333333333*x*[0]*[1] + 5.33333333333333*[0]*[0] - 9.33333333333333*[0]*[0]*[0]*[1]) + 0.06153*(0.4*x*x - 0.4*x*[0]*[0] - 0.8*x*[0]*[1] + 0.8*[0]*[0]*[0]*[1]) ) * 1.078076202 * 1000",xmin,xmax)
    else:
        myFunc =  ROOT.TF1("myFunc","( 0.001668*(-3.3*x*x + 1.3*x*[0]*[0] + 7.6*x*[0]*[1] + 2.0*[0]*[0]*[0]*[0] - 5.6*[0]*[0]*[0]*[1] - 1.0*[0]*[0]*[1]*[1]) + 0.01374*(1.5*x*x + 0.5*x*[0]*[0] - 4.0*x*[0]*[1] - 2.0*[0]*[0]*[0]*[0] + 4.0*[0]*[0]*[0]*[1]) + 0.001375*(0.35*x*x - 0.0166666666666667*x*[0]*[0] - 1.03333333333333*x*[0]*[1] - 0.333333333333333*[0]*[0]*[0]*[0] + 0.533333333333333*[0]*[0]*[0]*[1] + 0.5*[0]*[0]*[1]*[1]) + 0.004454*(-0.45*x*x + 0.45*x*[0]*[0] + 0.9*x*[0]*[1] + 1.0*[0]*[0]*[0]*[0] - 2.4*[0]*[0]*[0]*[1] + 0.5*[0]*[0]*[1]*[1]) + 0.01046*(-2.0*x*x - 3.33333333333333*x*[0]*[0] + 9.33333333333333*x*[0]*[1] + 5.33333333333333*[0]*[0]*[0]*[0] - 9.33333333333333*[0]*[0]*[0]*[1]) + 0.0638*(0.4*x*x - 0.4*x*[0]*[0] - 0.8*x*[0]*[1] + 0.8*[0]*[0]*[0]*[1]) ) * 1.034772182 * 1000",xmin,xmax)

    myFunc.SetParameter(0,CV); 
    myFunc.SetParameter(1,kl); 
    #myFunc.SetParameter(2,yt); 
    graph = ROOT.TGraph(myFunc);
    ci = ROOT.TColor.GetColor("#ff0000");
    graph.SetLineColor(ci);
    graph.SetLineWidth(2);
    # graph.Draw("l");
    nP = int((xmax-xmin)*10.0)
    Graph_syst_Scale =  ROOT.TGraphAsymmErrors(nP)
    for i in range(nP) : 
        Graph_syst_Scale_x=(xmin+(i*1.)/10.)
        Graph_syst_Scale_y=(getExpValue(xmin+(i*1.)/10.,yt)) 
        Graph_syst_Scale_x_err=(0)
        Graph_syst_Scale_y_errup=(  (2.09*yt*yt*yt*yt+0.28*yt*yt*(xmin+(i*1.)/10.)*(xmin+(i*1.)/10.)-1.37*yt*yt*yt*(xmin+(i*1.)/10.))*2.44185*0.053/BR)
        Graph_syst_Scale_y_errdown=((2.09*yt*yt*yt*yt+0.28*yt*yt*(xmin+(i*1.)/10.)*(xmin+(i*1.)/10.)-1.37*yt*yt*yt*(xmin+(i*1.)/10.))*2.44185*0.067/BR)
        Graph_syst_Scale.SetPoint(i,Graph_syst_Scale_x,Graph_syst_Scale_y)
        Graph_syst_Scale.SetPointError(i,Graph_syst_Scale_x_err,Graph_syst_Scale_x_err,Graph_syst_Scale_y_errup,Graph_syst_Scale_y_errdown)
    Graph_syst_Scale.SetLineColor(ROOT.kRed)
    Graph_syst_Scale.SetFillColor(ROOT.kRed)
    Graph_syst_Scale.SetFillStyle(3001)
    graph.Print()
    
    hframe = ROOT.TH1F('hframe', '', 100, -6, 6)
    hframe.SetMinimum(1)
    if 'TauTau' in tag:
        hframe.SetMaximum(10000)
    elif 'MuTau' in tag:
        hframe.SetMaximum(30000)
    elif 'ETau' in tag:
        hframe.SetMaximum(40000)
    else:
        #hframe.SetMaximum(8000)
        hframe.SetMaximum(1000000)


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

    # mg.Draw("pmc plc same")
    gr2sigma.Draw("3same")
    gr1sigma.Draw("3same")
    grexp.Draw("Lsame")
    #grobs.Draw("Lsame")

    graph.Draw("l same")
    #Graph_syst_Scale.Draw("e3 same");

    pt.Draw()
    pt2.Draw()
    # pt4.Draw()
    #txt_kt1.Draw()
    #txt_kt2.Draw()
    redrawBorder()
    c1.Update()
    c1.RedrawAxis("g")
    c1.SetLogy(1)
    legend.Draw()
    pt4.Draw()
    #pt5.Draw()
    c1.Update()
    raw_input()

    print 'Limit for sel'
    smXS = 31.05*0.073
    #smXS = 31.05
    #for ipt in range(0, grobs.GetN()):
    #    ptx = ROOT.Double(0.0)
    #    pty = ROOT.Double(0.0)
    #    grobs.GetPoint(ipt, x, y)
    #    if x == 1.0:
    #        print 'OBS LIMIT W.R.T. SM: ', y/(smXS), 'limit in fb:' , y

    #for ipt in range(0, grexp.GetN()):
    #    ptx = ROOT.Double(0.0)
    #    pty = ROOT.Double(0.0)
    #    grexp.GetPoint(ipt, x, y)
    #    if x == 1.0:
    #        print 'EXP LIMIT W.R.T. SM: ', y/smXS, 'limit in fb:' , y/0.073

    if False:
        print "OBS GRAPH"
        grobs.Print()
        print '\nEXP GRAPH'
        grexp.Print()
        print '\nEXP +2sigma'
        gr2sigma.Print()
        print '\nEXP +1sigma'
        gr1sigma.Print()

    c1.Print("plots/C2Vscan_"+tag+"_theor.pdf", 'pdf')
# for m in masses :
#     fileLocation = "cards_"+channels[c]+"_"+folder+"/"+app+str(m)+catstring+"/higgsCombine"+app+str(m)+"_forLim_noTH.Asymptotic.mH"+str(m)+".root"
#     if plotByCategory :
#         fileLocation = "cards_Combined_"+folder+"/"+app+str(m)+tooLopOn[c]+"/higgsCombine"+app+str(m)+"_forLim_noTH.Asymptotic.mH"+str(m)+".root"
#     if not os.path.isfile(fileLocation) : 
#         print "FILE NOT FOUND: " , fileLocation
#         continue
#     if benchmark<0 : pointNumbers.append(m)
#     fin = TFile.Open(fileLocation)
#     tree = fin.Get("limit")
#     if not tree :
#         print "MALEDETTO TREE", fileLocation, fin, tree
#         gAll.SetPoint(gAll.GetN(),m-offsetX,-1)
#         g95[c].SetPoint(g95[c].GetN(),m-offsetX,-1)
#         gObs.SetPoint(gObs.GetN(),m-offsetX,-1) 

#         continue
#     high=0
#     low=0
#     limit = 0
#     obs = 0
#     errX = 0.38
#     if benchmark == 1 : 
#         offsetX = -1
#         errX = 0.15
#     for entry in tree :
#         if tree.quantileExpected < 0 :
#             obs = tree.limit*scale
#         elif tree.quantileExpected == 0.5 : 
#             limit = tree.limit*scale
#         elif tree.quantileExpected > 0.83 and tree.quantileExpected < 0.85 : 
#             high = tree.limit*scale
#         elif tree.quantileExpected > 0.15 and tree.quantileExpected < 0.17: 
#             low = tree.limit*scale 
#         elif tree.quantileExpected > 0.024 and tree.quantileExpected < 0.026 : 
#             low95 = tree.limit*scale
#         elif tree.quantileExpected > 0.974 and tree.quantileExpected < 0.976 : 
#             high95 = tree.limit*scale

#     if limit>0 : #and limit < 50 : 
#         if m == 750 and limit > 900 :
#             limit = limit/100
#             obs = obs/100
#             high = high/100
#             high95=high95/100
#             low = low/100
#             low95=low95/100
#         gAll.SetPoint(gAll.GetN(),m-offsetX,limit)
#         gAll.SetPointError(gAll.GetN()-1,0+errX,0+errX,limit-low,high-limit)
#         g95[c].SetPoint(g95[c].GetN(),m-offsetX,limit)
#         g95[c].SetPointError(g95[c].GetN()-1,0+errX,0+errX,limit-low95,high95-limit)
#         gObs.SetPoint(gObs.GetN(),m-offsetX,obs)    


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
