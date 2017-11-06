from ROOT import *

fIn = TFile.Open("Distros_lambdaOnly_5p_20000ev_52sam_13TeV_JHEP_30K.root")

# pts = [0, 10, 15, 21, 30, 50]
# col = [kBlue, kBlue+1, kBlue+3, kBlue+5, kBlue+7, kBlue+9]

pts = [0, 21, 23, 30, 50]
# col = [kBlue, kBlue+1, kBlue+3, kBlue+5, kBlue+7, kBlue+9]
# col = [kBlue+2, kBlue-1, kGreen+1, kOrange+1, kRed+3]
col = [kGray+1, kBlue-2, kGreen+1, kOrange+1, kRed+3]
gStyle.SetOptStat(0)

c1 = TCanvas("c2", "c2", 600, 600)
c1.SetLogy()

leg = TLegend(0.6, 0.6, 0.88, 0.88)
leg.SetFillColor(kWhite)
leg.SetBorderSize(0)

for idx, pt in enumerate(pts):
    nn = "lamdaOnly/%s_mhh" %str(pt)
    print nn
    h = fIn.Get(nn)
    h.SetLineColor(col[idx])
    h.SetLineWidth(2)
    if idx == 0:
        h.SetTitle (";m_{hh} [GeV]; a.u.")
        h.GetXaxis().SetTitleOffset(1.2)
        h.GetYaxis().SetTitleOffset(1.4)
        h.Scale(1./h.Integral())
        h.SetMaximum(0.35)
        h.SetMinimum(1.e-3)
        h.Draw()
    else:
        h.Scale(1./h.Integral())
        h.Draw("same")
    leg.AddEntry(h, "k_{#lambda} = %i" % (pt-20), "l")

leg.Draw()
c1.Print("lambdas.pdf", "pdf")