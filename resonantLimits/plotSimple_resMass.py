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


c1 = ROOT.TCanvas("c1", "c1", 650, 500)
c1.SetFrameLineWidth(3)
c1.SetBottomMargin (0.15)
c1.SetRightMargin (0.05)
c1.SetLeftMargin (0.15)
c1.SetGridx()
c1.SetGridy()

mg = ROOT.TMultiGraph()

var = 'DNNoutSM_kl_1'

tags = ["cards_TauTauLegacy2018_06Apr2021"]
#tags = ["cards_TauTauLegacy2018_29Mar2021_m","cards_MuTauLegacy2018_29Mar2021_m","cards_ETauLegacy2018_29Mar2021_m"]
#tags = ['cards_ETauLegacy2018_test','cards_MuTauLegacy2018_test','cards_TauTauLegacy2018_test']
#tag='CombChan2018_res_17Feb2021'

#selections = ["sboostedLLMcut"]
selections = ["s1b1jresolvedMcutmHH250_335","s1b1jresolvedMcutmHH335_475","s1b1jresolvedMcutmHH475_725","s1b1jresolvedMcutmHH725_1100","s1b1jresolvedMcutmHH1100_3500","s2b0jresolvedMcutmHH250_335","s2b0jresolvedMcutmHH335_475","s2b0jresolvedMcutmHH475_725","s2b0jresolvedMcutmHH725_1100","s2b0jresolvedMcutmHH1100_3500","sboostedLLMcutmHH250_335","sboostedLLMcutmHH335_475","sboostedLLMcutmHH475_725","sboostedLLMcutmHH725_1100","sboostedLLMcutmHH1100_3500","VBFloosemHH250_335","VBFloosemHH335_475","VBFloosemHH475_725","VBFloosemHH725_1100","VBFloosemHH1100_3500"]
#selections = ["s1b1jresolvedMcut", "s2b0jresolvedMcut", "sboostedLLMcut", "VBFloose"]
#selections = ["comb_cat"]

massval = [250, 260, 270, 280, 300, 320, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 1000, 1250, 1500, 1750, 2000, 2500, 3000]
print massval


### read the scan with normal width
for tag in tags:
   #if tag is not "cards_TauTauLegacy2018_29Mar2021_m":
   #   selections = ["s1b1jresolvedMcut", "s2b0jresolvedMcut", "sboostedLLMcut", "VBFloose","s1b1jresolvedMcutInvDR", "s2b0jresolvedMcutInvDR", "sboostedLLMcutInvDR", "VBFlooseInvDR"]
   #else:
   #   selections = ["s1b1jresolvedMcut", "s2b0jresolvedMcut", "sboostedLLMcut", "VBFloose"]
   for sel in selections:

      gr2sigma = ROOT.TGraphAsymmErrors()
      gr1sigma = ROOT.TGraphAsymmErrors()
      grexp = ROOT.TGraph()
      grobs = ROOT.TGraph()
      
      ptsList = [] # (x, obs, exp, p2s, p1s, m1s, m2s)
      if sel is "sboostedLLMcut":
         massval = [320, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 1000, 1250, 1500, 1750, 2000, 2500, 3000]
      else:
         massval = [250, 260, 270, 300, 320, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 1000, 1250, 1500, 1750, 2000, 2500, 3000]

      for mass in massval:
         fName = tag+'/'+sel+var+'/combined_out/comb.Radion{0}.log'.format(mass)
         #fName = 'cards_'+tag+'/'+sel+var+'/comb.{1}.Radion{0}.log'.format(mass,sel)


         exp   = 10.*parseFile(fName)            
         obs   = exp#parseFile(fName, exp=False) 
         m1s_t = 10.*parseFile(fName, CL='16.0') 
         p1s_t = 10.*parseFile(fName, CL='84.0') 
         m2s_t = 10.*parseFile(fName, CL=' 2.5') 
         p2s_t = 10.*parseFile(fName, CL='97.5') 

         ## because the other code wants +/ sigma vars as deviations, without sign, from the centeal exp value...
         p2s = p2s_t - exp
         p1s = p1s_t - exp
         m2s = exp - m2s_t
         m1s = exp - m1s_t
         xval = mass

         ptsList.append((xval, obs, exp, p2s, p1s, m1s, m2s))

         # print lambdasfiner

         # grexp.SetPoint(ipt, xval, exp)
         # grobs.SetPoint(ipt, xval, obs)
         # gr1sigma.SetPoint(ipt, xval, exp)
         # gr2sigma.SetPoint(ipt, xval, exp)
         # gr1sigma.SetPointError(ipt, 0,0,m1s,p1s)
         # gr2sigma.SetPointError(ipt, 0,0,m2s,p2s)
      ptsList.sort()
      i=0
      for ipt, pt in enumerate(ptsList):
         xval = pt[0]
         obs  = pt[1]
         exp  = pt[2]
         p2s  = pt[3]
         p1s  = pt[4]
         m1s  = pt[5]
         m2s  = pt[6]
         print xval, exp
	 if exp > 0:
         	grexp.SetPoint(i, xval, exp)
         	grobs.SetPoint(i, xval, obs)
         	gr1sigma.SetPoint(i, xval, exp)
         	gr2sigma.SetPoint(i, xval, exp)
         	gr1sigma.SetPointError(i, 0,0,m1s,p1s)
         	gr2sigma.SetPointError(i, 0,0,m2s,p2s)
		i=i+1


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
      
      legend.SetX1(0.17284)
      legend.SetY1(0.170526)
      # legend.SetX2(0.5)
      legend.SetX2(0.520062)
      legend.SetY2(0.450)
      
      legend.SetFillColor(ROOT.kWhite)
      legend.SetBorderSize(0)
      # legend
      legend.SetHeader('95% CL upper limits')
      #    legend.AddEntry(grobs,"Observed","l")
      legend.AddEntry(grexp, "Median expected", "l")
      legend.AddEntry(gr1sigma, "68% expected", "f")
      legend.AddEntry(gr2sigma, "95% expected", "f")
      
      fakePlot3 = ROOT.TGraphAsymmErrors()
      fakePlot3.SetFillColor(ROOT.kRed)
      fakePlot3.SetFillStyle(3001)
      fakePlot3.SetLineColor(ROOT.kRed)
      fakePlot3.SetLineWidth(3)
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
      if 'ETau' in tag:
         pt4.AddText("bb e#tau_{h}") 
      elif 'MuTau' in tag:
         pt4.AddText("bb #mu_{}#tau_{h}")
      elif 'TauTau' in tag:
         pt4.AddText("bb #tau_{h}#tau_{h}")
      #pt4.AddText("bb #tau_{#mu}#tau_{h} + bb #tau_{e}#tau_{h} + bb #tau_{h}#tau_{h}") 
      #pt4.AddText("bb #tau_{e}#tau_{h}") 
      #pt4.AddText(sel + ", bb #tau_{h}#tau_{h}")
      #pt4.AddText("GGF-HH Comb. cat.")
      #pt4.AddText("HH #rightarrow bb#tau#tau")
      
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
      #pt5.AddText('#scale[0.8]{{0}}'.format(sel))
      #sel="Combine categories"
      pt5.AddText(sel)

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
      xmin=200
      xmax=3100

      # yt == 1 line
      yt=1
      BR = 1
      #myFunc =  ROOT.TF1("myFunc","(2.09*[0]*[0]*[0]*[0] + 0.28*[0]*[0]*x*[0]*x*[0] -1.37*[0]*[0]*[0]*x*[0])*2.44185/[1]",xmin,xmax);
      myFunc =  ROOT.TF1("myFunc","(62.5339 -44.323*x + 9.6340*x*x)*1.115",xmin,xmax);
      #myFunc.SetParameter(0,yt);
      #myFunc.SetParameter(1,BR);
      #myFunc.SetParameter(2,yt); 
      graph = ROOT.TGraph(myFunc);
      ci = ROOT.TColor.GetColor("#ff0000");
      graph.SetLineColor(ci);
      graph.SetLineWidth(2);
      #graph.Draw("l");
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
      #graph.Print()

      ## yt == 2 line
      #ytbis = 2
      ## Graph_syst_Scale2 =  ROOT.TGraphAsymmErrors(nP)
      #Graph_syst_Scale2 =  ROOT.TGraphAsymmErrors()
      #for i in range(nP) :
      #    # print Graph_syst_Scale2.GetN(), i
      #    Graph_syst_Scale2_x=(xmin+(i*1.)/10.)
      #    Graph_syst_Scale2_y=(getExpValue(2*(xmin+(i*1.)/10.),ytbis))
      #    # if Graph_syst_Scale2_y > 800:
      #    if Graph_syst_Scale2_x > 10.9 or Graph_syst_Scale2_x < -5.3:
      #        continue
      #    # print i
      #    Graph_syst_Scale2_x_err=(0)
      #    Graph_syst_Scale2_y_errup=(  (2.09*ytbis*ytbis*ytbis*ytbis+0.28*ytbis*ytbis*2*(xmin+(i*1.)/10.)*2*(xmin+(i*1.)/10.)-1.37*ytbis*ytbis*ytbis*2*(xmin+(i*1.)/10.))*2.44185*0.053/BR)
      #    Graph_syst_Scale2_y_errdown=((2.09*ytbis*ytbis*ytbis*ytbis+0.28*ytbis*ytbis*2*(xmin+(i*1.)/10.)*2*(xmin+(i*1.)/10.)-1.37*ytbis*ytbis*ytbis*2*(xmin+(i*1.)/10.))*2.44185*0.067/BR)
      #    Graph_syst_Scale2.SetPoint(Graph_syst_Scale2.GetN(),Graph_syst_Scale2_x,Graph_syst_Scale2_y)
      #    Graph_syst_Scale2.SetPointError(Graph_syst_Scale2.GetN()-1,Graph_syst_Scale2_x_err,Graph_syst_Scale2_x_err,Graph_syst_Scale2_y_errup,Graph_syst_Scale2_y_errdown)
      #Graph_syst_Scale2.SetLineColor(ROOT.kRed+2)
      #Graph_syst_Scale2.SetFillColor(ROOT.kRed+2)
      #Graph_syst_Scale2.SetFillStyle(3001)
      #myFunc.SetParameter(0,ytbis);
      #myFunc.SetParameter(1,BR);
      #myFunc.SetNpx(200)
      #graph2 = ROOT.TGraph(myFunc);
      #for ipt in reversed(range(0, graph2.GetN())):
      #    x = ROOT.Double(0)
      #    y = ROOT.Double(0)
      #    graph2.GetPoint(ipt, x, y)
      #    if x >= 10.9 or x < -5.3    :
      #        graph2.RemovePoint(ipt)
      #
      ##graph2.SetLineColor(ROOT.kRed+1);
      ##graph2.SetLineWidth(2);


      hframe = ROOT.TH1F('hframe', '', 100, 250, 3100)
      hframe.SetMinimum(0.1)
      if '2016' in tag:
         hframe.SetMaximum(10000)
         hframe.SetMinimum(0.1)#5000)
      elif '2017' in tag:
         hframe.SetMaximum(10000)
         hframe.SetMinimum(0.1)#5000)
      elif '2018' in tag:
         hframe.SetMinimum(0.1)
         hframe.SetMaximum(100000)#4000)
      else:
         hframe.SetMaximum(10000)
         hframe.SetMinimum(0.1)#5000)

      hframe.GetYaxis().SetTitleSize(0.047)
      hframe.GetXaxis().SetTitleSize(0.055)
      hframe.GetYaxis().SetLabelSize(0.045)
      hframe.GetXaxis().SetLabelSize(0.045)
      hframe.GetXaxis().SetLabelOffset(0.012)
      hframe.GetYaxis().SetTitleOffset(1.2)
      hframe.GetXaxis().SetTitleOffset(1.1)

      hframe.GetYaxis().SetTitle("95% CL on #sigma #times #bf{#it{#Beta}}(S#rightarrowHH#rightarrow bb#tau#tau) [fb]")
      #hframe.GetYaxis().SetTitle("95% CL on #sigma#times B (S#rightarrow HH#rightarrow#tau#tau) [fb]")
      hframe.GetXaxis().SetTitle("m_{S} [GeV]")

      hframe.SetStats(0)
      ROOT.gPad.SetTicky()
      hframe.Draw()

      # mg.Draw("pmc plc same")
      gr2sigma.Draw("3same")
      gr1sigma.Draw("3same")
      grexp.Draw("Lsame")
      #grobs.Draw("Lsame")

      #graph.Draw("l same") #LP NOTE: THIS SHOULD BE THE THEORY GRAPH

      #graph2.Draw("l same")
      #Graph_syst_Scale.Draw("e3 same");
      #Graph_syst_Scale2.Draw("e3 same");

      pt.Draw()
      pt2.Draw()
      # pt4.Draw()
      #txt_kt1.Draw()
      #txt_kt2.Draw()
      #redrawBorder()
      c1.Update()
      c1.RedrawAxis("g")
      c1.SetLogy()
      #legend.Draw()
      pt4.Draw()
      pt5.Draw()
      c1.Update()
      #raw_input()

      print 'Limit for ', sel
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
         
         
      #c1.Print("plots/klscan_"+tag+".pdf", 'pdf')
      c1.Print("plots/massScan_"+tag+sel+".pdf", 'pdf')
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
