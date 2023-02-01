import os
import ROOT
import array


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



# User settables
tag = "27Nov2020"

colors = [ROOT.kBlue, ROOT.kRed, ROOT.kCyan, ROOT.kBlack]
years  = ["comb"]                 # use this for Run2 combination
years  = ["2016", "2017", "2018"] # use this to comapre the 3 years

leg = ROOT.TLegend(0,0,0,0)
leg.SetX1(0.45)
leg.SetY1(0.630526)
leg.SetX2(0.6)
leg.SetY2(0.88)
leg.SetFillColor(ROOT.kWhite)

mg = ROOT.TMultiGraph()
mg.GetYaxis().SetTitle("-2 #Delta ln(L)")
mg.GetXaxis().SetTitle("k_{V}")


for i,year in enumerate(years):

    if "comb" in year:
        file = "cards_CombAll_"+tag+"_autoMC1/higgsCombineCV_scan.MultiDimFit.mH120.root"
    else:
        if "2018" in year:
            file = "cards_CombChan_"+year+"_"+tag+"/higgsCombineCV_scan.MultiDimFit.mH120.root"
        else:
            file = "cards_CombChan_"+year+"_"+tag+"_autoMC1/higgsCombineCV_scan.MultiDimFit.mH120.root"

    fIn = ROOT.TFile(file, "READ")
    tree = fIn.Get("limit")

    CVs  = []
    vals = []

    for iev,ev in enumerate(tree):
        if iev==0: continue
        #print 'CV:', ev.CV, '2*deltaNLL:', 2*ev.deltaNLL

        if 2*ev.deltaNLL>10.0: continue
        CVs.append(ev.CV)
        vals.append(2*ev.deltaNLL)

    gr = ROOT.TGraph()
    for iCV, CV in enumerate(CVs):
        gr.SetPoint(iCV, CV, vals[iCV])

    gr.SetLineColor(colors[i])
    gr.SetMarkerColor(colors[i])
    gr.SetMarkerStyle(8)

    mg.Add(gr, "lp")
    leg.AddEntry(gr, year, "pl")


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
pt2.AddText("Run2 - 137.1 fb^{-1} (13 TeV)")


###
c1 = ROOT.TCanvas("c1", "c1", 650, 500)
c1.SetFrameLineWidth(3)
c1.SetBottomMargin (0.15)
c1.SetRightMargin (0.05)
c1.SetLeftMargin (0.15)
c1.SetGridx()
c1.SetGridy()

hframe = ROOT.TH1F('hframe', '', 100, -22, 22)
hframe.SetMinimum(0)
hframe.SetMaximum(10)

hframe.GetYaxis().SetTitleSize(0.047)
hframe.GetXaxis().SetTitleSize(0.055)
hframe.GetYaxis().SetLabelSize(0.045)
hframe.GetXaxis().SetLabelSize(0.045)
hframe.GetXaxis().SetLabelOffset(0.012)
hframe.GetYaxis().SetTitleOffset(1.2)
hframe.GetXaxis().SetTitleOffset(1.1)
hframe.GetYaxis().SetTitle("-2 #Delta ln(L)")
hframe.GetXaxis().SetTitle("k_{V}")
hframe.SetStats(0)
ROOT.gPad.SetTicky()
hframe.Draw()

mg.Draw("apl")
pt.Draw()
if 'comb' in year:
    pt2.Draw()

# Horizontal lines
c1.Update()
line1 = ROOT.TLine(c1.GetUxmin(),1.0,c1.GetUxmax(),1.0)
line4 = ROOT.TLine(c1.GetUxmin(),3.84,c1.GetUxmax(),3.84)
line1.SetLineColor(13)
line4.SetLineColor(13)
line1.SetLineStyle(2)
line4.SetLineStyle(2)
line1.SetLineWidth(3)
line4.SetLineWidth(3)
line1.Draw()
line4.Draw()

redrawBorder()
c1.Update()
c1.RedrawAxis("g")
if 'comb' not in year:
    leg.Draw()
c1.Update()
if 'comb' in year:
    c1.Print('plots/CV_1Dlikelihood_Run2_'+tag+'.pdf','pdf')
else:
    c1.Print('plots/CV_1Dlikelihood_comparison_'+tag+'.pdf','pdf')

import pdb; pdb.set_trace()
