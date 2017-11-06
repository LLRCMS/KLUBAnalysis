import ROOT

ROOT.gROOT.SetBatch(True)

c1 = ROOT.TCanvas('c1', 'c1', 600, 300)
c1.SetLeftMargin(0.15)
c1.SetFrameLineWidth(3)

ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetPaintTextFormat('.4f')
ROOT.gStyle.SetPalette(ROOT.kBlackBody)

fIn = ROOT.TFile.Open('results/purity_nonresonant_21.root')

h2d = ROOT.TH2F('h2d', ';Generated #tau#tau decay mode;Reconstructed #tau#tau decay mode', 6, 0, 6, 3, 0, 3)
h2d.SetMarkerSize(3) # to set the size of the "TEXT" option..

channelNameLatex = [
    '#tau_{#mu}#tau_{h}',
    '#tau_{e}#tau_{h}',
    '#tau_{h}#tau_{h}',
    '#tau_{#mu}#tau_{#mu}',
    '#tau_{e}#tau_{e}',
    '#tau_{#mu}#tau_{e}',

]

for ibin in range (1, 7):
    h2d.GetXaxis().SetBinLabel(ibin, channelNameLatex[ibin-1])
for ibin in range (1, 4):
    h2d.GetYaxis().SetBinLabel(ibin, channelNameLatex[ibin-1])

h2d.GetXaxis().SetTitleSize(0.05)
h2d.GetYaxis().SetTitleSize(0.05)
h2d.GetXaxis().SetLabelSize(0.08)
h2d.GetYaxis().SetLabelSize(0.08)


h0 = fIn.Get('hDecayMode0')
h1 = fIn.Get('hDecayMode1')
h2 = fIn.Get('hDecayMode2')

b0 = h2d.FindBin(1-0.5, 1-0.5)
b1 = h2d.FindBin(2-0.5, 1-0.5)
b2 = h2d.FindBin(3-0.5, 1-0.5)
b3 = h2d.FindBin(4-0.5, 1-0.5)
b4 = h2d.FindBin(5-0.5, 1-0.5)
b5 = h2d.FindBin(6-0.5, 1-0.5)
h2d.SetBinContent(b0, h0.GetBinContent(0+1))
h2d.SetBinContent(b1, h0.GetBinContent(1+1))
h2d.SetBinContent(b2, h0.GetBinContent(2+1))
h2d.SetBinContent(b3, h0.GetBinContent(3+1))
h2d.SetBinContent(b4, h0.GetBinContent(4+1))
h2d.SetBinContent(b5, h0.GetBinContent(5+1))

b0 = h2d.FindBin(1-0.5, 2-0.5)
b1 = h2d.FindBin(2-0.5, 2-0.5)
b2 = h2d.FindBin(3-0.5, 2-0.5)
b3 = h2d.FindBin(4-0.5, 2-0.5)
b4 = h2d.FindBin(5-0.5, 2-0.5)
b5 = h2d.FindBin(6-0.5, 2-0.5)
h2d.SetBinContent(b0, h1.GetBinContent(0+1))
h2d.SetBinContent(b1, h1.GetBinContent(1+1))
h2d.SetBinContent(b2, h1.GetBinContent(2+1))
h2d.SetBinContent(b3, h1.GetBinContent(3+1))
h2d.SetBinContent(b4, h1.GetBinContent(4+1))
h2d.SetBinContent(b5, h1.GetBinContent(5+1))

b0 = h2d.FindBin(1-0.5, 3-0.5)
b1 = h2d.FindBin(2-0.5, 3-0.5)
b2 = h2d.FindBin(3-0.5, 3-0.5)
b3 = h2d.FindBin(4-0.5, 3-0.5)
b4 = h2d.FindBin(5-0.5, 3-0.5)
b5 = h2d.FindBin(6-0.5, 3-0.5)
h2d.SetBinContent(b0, h2.GetBinContent(0+1))
h2d.SetBinContent(b1, h2.GetBinContent(1+1))
h2d.SetBinContent(b2, h2.GetBinContent(2+1))
h2d.SetBinContent(b3, h2.GetBinContent(3+1))
h2d.SetBinContent(b4, h2.GetBinContent(4+1))
h2d.SetBinContent(b5, h2.GetBinContent(5+1))

### horiz lines
l1 = ROOT.TLine(0, 1, 6, 1)
l2 = ROOT.TLine(0, 2, 6, 2)
l1.SetLineWidth(2)
l2.SetLineWidth(2)
l1.SetLineStyle(7)
l2.SetLineStyle(7)

h2d.Draw('colz text')
l1.Draw()
l2.Draw()
c1.Update()
raw_input()
c1.Print('purity2Dmatrix.pdf', 'pdf')
