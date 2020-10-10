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
tag = "4Oct2020"

colors = [ROOT.kRed, ROOT.kBlue, ROOT.kCyan, ROOT.kBlack]
years  = ["2016", "2017", "2018", "comb"]
years  = ["2016", "2017", "2018"]

leg = ROOT.TLegend(0,0,0,0)
leg.SetX1(0.45)
leg.SetY1(0.630526)
leg.SetX2(0.6)
leg.SetY2(0.88)
leg.SetFillColor(ROOT.kWhite)

mg = ROOT.TMultiGraph()
mg.GetYaxis().SetTitle("-2 #Delta ln(L)")
mg.GetXaxis().SetTitle("k_{#lambda}")

mg_zoom = ROOT.TMultiGraph()
mg_zoom.GetYaxis().SetTitle("-2 #Delta ln(L)")
mg_zoom.GetXaxis().SetTitle("k_{#lambda}")


for i,year in enumerate(years):

    if "comb" in year:
        file = "cards_CombAll_"+tag+"/higgsCombinekl_scan.MultiDimFit.mH120.root"
    else:
        file = "cards_CombChan_"+year+"_"+tag+"/higgsCombinekl_scan.MultiDimFit.mH120.root"
    fIn = ROOT.TFile(file, "READ")
    tree = fIn.Get("limit")

    kls  = []
    vals = []
    kls_zoom  = []
    vals_zoom = []

    for iev,ev in enumerate(tree):
        if iev==0: continue
        #print 'kl:', ev.kl, '2*deltaNLL:', 2*ev.deltaNLL

        if 2*ev.deltaNLL>12.5: continue
        kls.append(ev.kl)
        vals.append(2*ev.deltaNLL)

        if 2*ev.deltaNLL>1.1: continue
        kls_zoom.append(ev.kl)
        vals_zoom.append(2*ev.deltaNLL)

    gr = ROOT.TGraph()
    for ikl, kl in enumerate(kls):
        gr.SetPoint(ikl, kl, vals[ikl])

    gr_zoom = ROOT.TGraph()
    for jkl, kl in enumerate(kls_zoom):
        gr_zoom.SetPoint(jkl, kl, vals_zoom[jkl])

    gr.SetLineColor(colors[i])
    gr.SetMarkerColor(colors[i])
    gr.SetMarkerStyle(8)

    gr_zoom.SetLineColor(colors[i])
    gr_zoom.SetMarkerColor(colors[i])
    gr_zoom.SetMarkerStyle(8)

    mg.Add(gr, "lp")
    mg_zoom.Add(gr_zoom, "lp")

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
hframe.GetXaxis().SetTitle("k_{#lambda}")
hframe.SetStats(0)
ROOT.gPad.SetTicky()
hframe.Draw()

mg.Draw("apl")
pt.Draw()

redrawBorder()
c1.Update()
c1.RedrawAxis("g")
leg.Draw()
c1.Update()
c1.Print('plots/comparison_kl_scan_'+tag+'.pdf','pdf')

#### 
c2 = ROOT.TCanvas("c1", "c1", 650, 500)
c2.SetFrameLineWidth(3)
c2.SetBottomMargin (0.15)
c2.SetRightMargin (0.05)
c2.SetLeftMargin (0.15)
c2.SetGridx()
c2.SetGridy()

hframe2 = ROOT.TH1F('hframe', '', 100, -22, 22)
hframe2.SetMinimum(0)
hframe2.SetMaximum(1.5)

hframe2.GetYaxis().SetTitleSize(0.047)
hframe2.GetXaxis().SetTitleSize(0.055)
hframe2.GetYaxis().SetLabelSize(0.045)
hframe2.GetXaxis().SetLabelSize(0.045)
hframe2.GetXaxis().SetLabelOffset(0.012)
hframe2.GetYaxis().SetTitleOffset(1.2)
hframe2.GetXaxis().SetTitleOffset(1.1)
hframe2.GetYaxis().SetTitle("-2*#Deltaln(L)")
hframe2.GetXaxis().SetTitle("k_{#lambda}")
hframe2.SetStats(0)
ROOT.gPad.SetTicky()
hframe2.Draw()

mg_zoom.Draw("apl")
pt.Draw()

redrawBorder()
c2.Update()
c2.RedrawAxis("g")
leg.Draw()
c2.Update()
c2.Print('plots/comparison_kl_scan_zoom_'+tag+'.pdf','pdf')


import pdb; pdb.set_trace()
