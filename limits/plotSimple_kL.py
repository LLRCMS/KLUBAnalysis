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
        search = ('Expected %s%%: r <'%CL)
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

var = 'DNNoutSM_kl_1'

#year = '2018'
#tag = 'CombChan_'+year+'_23Apr2021'
year = '2018'
tag = 'CombAll_23Apr2021'

selections = ["comb_cat"]

lambdas = [x for x in range(1, 82)]
print lambdas

klval = [-20  , -19.5, -19  , -18.5, -18  , -17.5, -17  , -16.5, -16  , -15.5, -15  , -14.5, -14  , -13.5, -13  , -12.5, -12  , -11.5, -11  , -10.5, -10  , -9.5 , -9   , -8.5 , -8   , -7.5 , -7   , -6.5 , -6   , -5.5 , -5   , -4.5 , -4   , -3.5 , -3   , -2.5 , -2   , -1.5 , -1   , -0.5 , 0    , 0.5  , 1    , 1.5  , 2    , 2.5  , 3    , 3.5  , 4    , 4.5  , 5    , 5.5  , 6    , 6.5  , 7    , 7.5  , 8    , 8.5  , 9    , 9.5  , 10   , 10.5 , 11   , 11.5 , 12   , 12.5 , 13   , 13.5 , 14   , 14.5 , 15   , 15.5 , 16   , 16.5 , 17   , 17.5 , 18   , 18.5 , 19   , 19.5 , 20]
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
            fName = 'cards_'+tag+'/out_Asym_{0}_noTH.log'.format(lambdas[ipt])
        elif 'All' in tag:
            fName = 'cards_'+tag+'/out_Asym_{0}_noTH.log'.format(lambdas[ipt])
        else:
            #fName = 'cards_Combined_2019_10_11/ggHH_bbtt{0}BDToutSM_kl_{1}/out_Asym_ggHH_bbtt{0}_noTH.log'.format(lambdas[ipt], klval[ipt])
            #fName = 'cards_Combined_2017_03_10_lmr70/ggHH_bbtt{0}MT2/out_Asym_ggHH_bbtt{0}_noTH.log'.format(lambdas[ipt])
            fName = 'cards_TauTau1Sept_noShape_GGF/{0}'.format(sel)+var+'/out_Asym_{0}_noTH.log'.format(lambdas[ipt])

        xval = klval[ipt]

        corrFactor = 1.034772182
        if year == "2016":
            corrFactor = 1.078076202

        xstheoVBF = getXStheoVBF (1,xval,year) * corrFactor * 1000.0  # C2V,kl,year (VBF needs conversion to [fb])
        xstheoGGF = getXStheoGGF (xval)                               # kl
        xstheoTOT = xstheoVBF + xstheoGGF

        # Can get different results on r_gghh:
        #exp  = parseFile(fName)                                  # <- How many times the SM I'm excluding
        #exp  = parseFile(fName)            * xstheoTOT           # <- Excluded HH cross section
        exp   = parseFile(fName)            * xstheoTOT * 0.073   # <- Excluded HH cross section times BR(bbtautau)
        obs   = parseFile(fName, exp=False) * xstheoTOT * 0.073
        m1s_t = parseFile(fName, CL='16.0') * xstheoTOT * 0.073
        p1s_t = parseFile(fName, CL='84.0') * xstheoTOT * 0.073
        m2s_t = parseFile(fName, CL=' 2.5') * xstheoTOT * 0.073
        p2s_t = parseFile(fName, CL='97.5') * xstheoTOT * 0.073

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
    pt4.AddText("HH GGF+VBF production")
    pt4.AddText("HH #rightarrow bb#tau#tau")

    ##### theory lines
    xmin=-20
    xmax=+20

    yt=1
    BR = 1
    C2V = 1
    CV = 1

    # 2016 data
    #myFunc =  ROOT.TF1("myFunc","(2.09*[0]*[0]*[0]*[0] + 0.28*[0]*[0]*x*[0]*x*[0] -1.37*[0]*[0]*[0]*x*[0])*2.44185/[1]",xmin,xmax)

    # Old Run2 recommendation
    #myFunc =  ROOT.TF1("myFunc","( (62.5339 -44.323*x + 9.6340*x*x)*1.115) + ( ( 0.001668*(-3.3*[1]**2 + 1.3*[1]*[0]**2 + 7.6*[1]*[0]*x + 2.0*[0]**4 - 5.6*[0]**3*x - 1.0*[0]**2*x**2) + 0.01374*(1.5*[1]**2 + 0.5*[1]*[0]**2 - 4.0*[1]*[0]*x - 2.0*[0]**4 + 4.0*[0]**3*x) + 0.001375*(0.35*[1]**2 - 0.0166666666666667*[1]*[0]**2 - 1.03333333333333*[1]*[0]*x - 0.333333333333333*[0]**4 + 0.533333333333333*[0]**3*x + 0.5*[0]**2*x**2) + 0.004454*(-0.45*[1]**2 + 0.45*[1]*[0]**2 + 0.9*[1]*[0]*x + 1.0*[0]**4 - 2.4*[0]**3*x + 0.5*[0]**2*x**2) + 0.01046*(-2.0*[1]**2 - 3.33333333333333*[1]*[0]**2 + 9.33333333333333*[1]*[0]*x + 5.33333333333333*[0]**4 - 9.33333333333333*[0]**3*x) + 0.0638*(0.4*[1]**2 - 0.4*[1]*[0]**2 - 0.8*[1]*[0]*x + 0.8*[0]**3*x) ) * 1.034772182 * 1000.0 * 0.073) ",xmin,xmax)

    # Run2 - Updated following: https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHWGHH?redirectedfrom=LHCPhysics.LHCHXSWGHH#Latest_recommendations_for_gluon
    myFunc =  ROOT.TF1("myFunc","( (70.3874 -50.4111*x + 11.0595*x*x) * 0.073) + ( ( 0.001668*(-3.3*[1]**2 + 1.3*[1]*[0]**2 + 7.6*[1]*[0]*x + 2.0*[0]**4 - 5.6*[0]**3*x - 1.0*[0]**2*x**2) + 0.01374*(1.5*[1]**2 + 0.5*[1]*[0]**2 - 4.0*[1]*[0]*x - 2.0*[0]**4 + 4.0*[0]**3*x) + 0.001375*(0.35*[1]**2 - 0.0166666666666667*[1]*[0]**2 - 1.03333333333333*[1]*[0]*x - 0.333333333333333*[0]**4 + 0.533333333333333*[0]**3*x + 0.5*[0]**2*x**2) + 0.004454*(-0.45*[1]**2 + 0.45*[1]*[0]**2 + 0.9*[1]*[0]*x + 1.0*[0]**4 - 2.4*[0]**3*x + 0.5*[0]**2*x**2) + 0.01046*(-2.0*[1]**2 - 3.33333333333333*[1]*[0]**2 + 9.33333333333333*[1]*[0]*x + 5.33333333333333*[0]**4 - 9.33333333333333*[0]**3*x) + 0.0638*(0.4*[1]**2 - 0.4*[1]*[0]**2 - 0.8*[1]*[0]*x + 0.8*[0]**3*x) ) * 1.034772182 * 1000.0 * 0.073) ",xmin,xmax)

    myFunc.SetParameter(0,CV)
    myFunc.SetParameter(1,C2V)
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

        corrFactor = 1.034772182
        if year == "2016":
            corrFactor = 1.078076202
        xstheoVBF = getXStheoVBF (1,kl_x,year) * corrFactor * 1000.0 * 0.073 # C2V,kl,year (VBF needs conversion to [fb])
        xstheoGGF = getXStheoGGF (kl_x) * 0.073                              # kl
        Graph_syst_Scale_y=(xstheoVBF+xstheoGGF)

        # 2016 uncertainties
        #Graph_syst_Scale_y_errup=(  (2.09*yt*yt*yt*yt+0.28*yt*yt*(kl_x)*(kl_x)-1.37*yt*yt*yt*(kl_x))*2.44185*0.053/BR)
        #Graph_syst_Scale_y_errdown=((2.09*yt*yt*yt*yt+0.28*yt*yt*(kl_x)*(kl_x)-1.37*yt*yt*yt*(kl_x))*2.44185*0.067/BR)

        # Run2 uncertainties HH GGF
        # Updated following: https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHWGHH?redirectedfrom=LHCPhysics.LHCHXSWGHH#Latest_recommendations_for_gluon
        # upper_unc[kl] = Max[72.0744-51.7362*kl+11.3712*kl2, 70.9286-51.5708*kl+11.4497*kl2] in fb.
        # lower_unc[kl] = Min[66.0621-46.7458*kl+10.1673*kl2, 66.7581-47.721*kl+10.4535*kl2] in fb.
        # pdf+alpha_s   = 0.03
        # mtop          = 0.026
        scale_up   = (max(72.0744-51.7362*kl_x+11.3712*kl_x*kl_x,70.9286-51.5708*kl_x+11.4497*kl_x*kl_x)*0.073 - xstheoGGF) / xstheoGGF
        scale_down = (min(66.0621-46.7458*kl_x+10.1673*kl_x*kl_x,66.7581-47.721*kl_x+10.4535*kl_x*kl_x)*0.073  - xstheoGGF) / xstheoGGF
        GGF_erry_up   = xstheoGGF*((scale_up*scale_up + 0.03*0.03 + 0.026*0.026)**0.5)
        GGF_erry_down = xstheoGGF*((scale_down*scale_down + 0.03*0.03 + 0.026*0.026)**0.5)

        # Run2 uncertainties HH VBF
        # scale unc = {"up": 0.0003, "down": 0.0004}
        # pdf unc   = 0.021
        VBF_erry_up   = xstheoVBF * ((0.0003*0.0003 + 0.021*0.021)**0.5)
        VBF_erry_down = xstheoVBF * ((0.0004*0.0004 + 0.021*0.021)**0.5)

        # Run2 total uncertainty (GGF+VBF)
        Graph_syst_Scale_y_errup  = (GGF_erry_up+VBF_erry_up)
        Graph_syst_Scale_y_errdown= (GGF_erry_down+VBF_erry_down)

        #if i%10==0:
        #    print kl_x, 'GGF:', xstheoGGF, GGF_erry_up, GGF_erry_down
        #    print '   VBF:', xstheoVBF, VBF_erry_up, VBF_erry_down
        #    print '   TOT:', Graph_syst_Scale_y, Graph_syst_Scale_y_errup, Graph_syst_Scale_y_errdown

        Graph_syst_Scale.SetPoint(i,Graph_syst_Scale_x,Graph_syst_Scale_y)
        Graph_syst_Scale.SetPointError(i,Graph_syst_Scale_x_err,Graph_syst_Scale_x_err,Graph_syst_Scale_y_errdown,Graph_syst_Scale_y_errup)

    Graph_syst_Scale.SetLineColor(ROOT.kRed)
    Graph_syst_Scale.SetFillColor(ROOT.kRed)
    Graph_syst_Scale.SetFillStyle(3001)
    #graph.Print()
    #Graph_syst_Scale.Print()

    # hframe
    hframe = ROOT.TH1F('hframe', '', 100, -22, 22)
    hframe.SetMinimum(0.1)
    if '2016' in tag:
        hframe.SetMaximum(400)
    elif '2017' in tag:
        hframe.SetMaximum(400)
    elif '2018' in tag:
        hframe.SetMaximum(400)
    else:
        hframe.SetMaximum(200)

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

    c1.Print("../plots/v4/klscan_"+tag+"_theor_NEW.pdf", 'pdf')

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
