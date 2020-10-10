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
        return -1.0
    else:
        return matches[-1]

def getXStheo (kL): 
    A = 62.5339
    B = -44.323
    C = 9.6340

    val = A + B*kL + C*kL*kL
    return val


c1 = ROOT.TCanvas("c1", "c1", 500, 500)
c1.SetFrameLineWidth(3)
c1.SetBottomMargin (0.15)
c1.SetRightMargin (0.05)
c1.SetLeftMargin (0.15)
c1.SetGridx()
c1.SetGridy()

mg = ROOT.TMultiGraph()

var = 'DNNoutSM_kl_1'
tag = '4Oct2020'

years = ['2016', '2017', '2018','']

### read the scan with normal width
gr2sigma = ROOT.TGraphAsymmErrors()
gr1sigma = ROOT.TGraphAsymmErrors()
grexp = ROOT.TGraph()
grobs = ROOT.TGraph()

ptsList = [] # (x, obs, exp, p2s, p1s, m1s, m2s)
for i, year in enumerate(years):

    fName = 'cards_CombChan_'+year+'_'+tag+'/out_Asym_SM_noTH.log'
    if year == '':
        fName = 'cards_CombAll_'+tag+'/out_Asym_SM_noTH.log'

    # Can get different results on r_gghh:
    #exp = parseFile(fName)                  # <- How many times the SM I'm excluding
    #exp = parseFile(fName) * 31.05          # <- Excluded HH cross section
    #exp = parseFile(fName) * 31.05 * 0.073  # <- Excluded HH cross section times BR(bbtautau)

    exp   = parseFile(fName)
    obs   = parseFile(fName, exp=False)
    m1s_t = parseFile(fName, CL='16.0')
    p1s_t = parseFile(fName, CL='84.0')
    m2s_t = parseFile(fName, CL=' 2.5')
    p2s_t = parseFile(fName, CL='97.5')

    ## because the other code wants +/ sigma vars as deviations, without sign, from the centeal exp value...
    p2s = p2s_t - exp
    p1s = p1s_t - exp
    m2s = exp - m2s_t
    m1s = exp - m1s_t
    xval = i

    ptsList.append((xval, obs, exp, p2s, p1s, m1s, m2s))
    ptsList.append((xval+1, obs, exp, p2s, p1s, m1s, m2s))

# print lambdasfiner

# grexp.SetPoint(ipt, xval, exp)
# grobs.SetPoint(ipt, xval, obs)
# gr1sigma.SetPoint(ipt, xval, exp)
# gr2sigma.SetPoint(ipt, xval, exp)
# gr1sigma.SetPointError(ipt, 0,0,m1s,p1s)
# gr2sigma.SetPointError(ipt, 0,0,m2s,p2s)
#ptsList.sort()
print ptsList
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
legend.SetX1(0.574297)
legend.SetY1(0.70526)
legend.SetX2(0.925703)
legend.SetY2(0.88)
legend.SetFillColor(ROOT.kWhite)
legend.SetBorderSize(0)
legend.SetHeader('95% CL upper limits')
#legend.AddEntry(grobs,"Observed","l")
legend.AddEntry(grexp, "Median expected", "l")
legend.AddEntry(gr1sigma, "68% expected", "f")
legend.AddEntry(gr2sigma, "95% expected", "f")

#fakePlot3 = ROOT.TGraphAsymmErrors()
#fakePlot3.SetFillColor(ROOT.kRed)
#fakePlot3.SetFillStyle(3001)
#fakePlot3.SetLineColor(ROOT.kRed)
#fakePlot3.SetLineWidth(3)
#legend.AddEntry(fakePlot3, "Theoretical prediction", "lf")


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
pt2 = ROOT.TPaveText(0.79,0.9066667,0.8997773,0.957037,"brNDC")
pt2.SetBorderSize(0)
pt2.SetFillColor(0)
pt2.SetTextSize(0.040)
pt2.SetTextFont(42)
pt2.SetFillStyle(0)
#pt2.AddText("41.6 fb^{-1} (13 TeV)")
pt2.AddText("60.0 fb^{-1} (13 TeV)")

pt4 = ROOT.TPaveText(0.4819196+0.036,0.7780357+0.015+0.02,0.9008929+0.036,0.8675595+0.015,"brNDC")
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
#pt5.AddText('#scale[0.8]{0}'.format(sel))

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



#graph2.SetLineColor(ROOT.kRed+1);
#graph2.SetLineWidth(2);


hframe = ROOT.TH1F('hframe', '', 4, 0, 4)
hframe.SetMinimum(0.1)
hframe.SetMaximum(25)

hframe.GetYaxis().SetTitleSize(0.047)
hframe.GetXaxis().SetTitleSize(0.055)
hframe.GetYaxis().SetLabelSize(0.045)
hframe.GetXaxis().SetLabelSize(0.045)
hframe.GetXaxis().SetLabelOffset(0.012)
hframe.GetYaxis().SetTitleOffset(1.2)
hframe.GetXaxis().SetTitleOffset(1.1)

#hframe.GetYaxis().SetTitle("95% CL on #sigma #times #bf{#it{#Beta}}(HH#rightarrow bb#tau#tau) [fb]")
hframe.GetYaxis().SetTitle("95% CL on #sigma_{obs} / #sigma_{SM}")


hframe.SetStats(0)
ROOT.gPad.SetTicky()
hframe.Draw()
#hframe.GetXaxis().SetBinLabel(1, "ETau")
#hframe.GetXaxis().SetBinLabel(2, "MuTau")
#hframe.GetXaxis().SetBinLabel(3, "TauTau")
#hframe.GetXaxis().SetBinLabel(4, "Comb")
hframe.GetXaxis().SetBinLabel(1, "2016")
hframe.GetXaxis().SetBinLabel(2, "2017")
hframe.GetXaxis().SetBinLabel(3, "2018")
hframe.GetXaxis().SetBinLabel(4, "Comb")
# mg.Draw("pmc plc same")
gr2sigma.Draw("3same")
gr1sigma.Draw("3same")
grexp.Draw("Lsame")
#grobs.Draw("Lsame")

#graph.Draw("l same")
#graph2.Draw("l same")
#Graph_syst_Scale.Draw("e3 same");
#Graph_syst_Scale2.Draw("e3 same");

pt.Draw()
#pt2.Draw()
# pt4.Draw()
#txt_kt1.Draw()
#txt_kt2.Draw()
redrawBorder()
c1.Update()
c1.RedrawAxis("g")
#c1.SetLogy(1)
legend.Draw()
#pt4.Draw()
#pt5.Draw()
c1.Update()
#raw_input()


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

c1.Draw()
#raw_input()
c1.Print("plots/plot_SMpoint_"+tag+".pdf", 'pdf')
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
