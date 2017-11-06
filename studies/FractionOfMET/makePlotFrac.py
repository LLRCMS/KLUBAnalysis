import ROOT

def setStyles(h, color):
    h.SetLineColor(color)
    h.SetLineWidth(2)

ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)

c1 = ROOT.TCanvas("c1", "c1", 600, 600)
c1.cd()
c1.SetFrameLineWidth(3)
c1.SetLeftMargin(0.15);
# c1.SetBottomMargin(0.12);
# c1.SetTopMargin(0.055);

fIn300 = ROOT.TFile.Open("fOut_MET_Radion300.root")
fIn600 = ROOT.TFile.Open("fOut_MET_Radion600.root")
fIn900 = ROOT.TFile.Open("fOut_MET_Radion900.root")

hIn300 = fIn300.Get("hFracTau")
hIn600 = fIn600.Get("hFracTau")
hIn900 = fIn900.Get("hFracTau")

setStyles(hIn300, ROOT.kRed+2)
setStyles(hIn600, ROOT.kRed+0)
setStyles(hIn900, ROOT.kRed-7)

hIn900.SetTitle(";Fraction of p_{T}^{miss} from #tau decays; a.u.")
hIn900.GetXaxis().SetTitleOffset(1.2)
hIn900.GetYaxis().SetTitleOffset(1.5)

hIn900.DrawNormalized()
hIn600.DrawNormalized("same")
hIn300.DrawNormalized("same")

leg = ROOT.TLegend(0.25, 0.5, 0.55, 0.70)
leg.SetBorderSize(0)
leg.SetFillStyle(0)
leg.AddEntry(hIn300, "m_{X} = 300 GeV", "l")
leg.AddEntry(hIn600, "m_{X} = 600 GeV", "l")
leg.AddEntry(hIn900, "m_{X} = 900 GeV", "l")
leg.Draw()

c1.Update()
c1.Print("met_plot.pdf", "pdf")