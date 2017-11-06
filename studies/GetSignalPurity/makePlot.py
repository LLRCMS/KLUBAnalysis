import ROOT
import glob
import argparse 

############
##########
# NOTE!! need a ROOT 6 release for the tefficiency class handling floats

parser = argparse.ArgumentParser(description='Command line parser of plotting options')
parser.add_argument('--nonres', dest='nonres', help='non resonant sample', action='store_true', default=False)
args = parser.parse_args()

channelNameLatex = [
    '#tau_{#mu}#tau_{h}',
    '#tau_{e}#tau_{h}',
    '#tau_{h}#tau_{h}',
]

channelName = [
    'MuTau',
    'ETau',
    'TauTau'
]

names = [
    '#mu#tau_{h}',
    'e#tau_{h}',
    '#tau_{h}#tau_{h}',
    '#mu#mu',
    'ee',
    'e#mu',
]

grs = [
    ROOT.TGraphAsymmErrors(),
    ROOT.TGraphAsymmErrors(),
    ROOT.TGraphAsymmErrors(),
]

colors = [
    ROOT.kGreen+1,
    ROOT.kBlue,
    ROOT.kRed,
]

for idx, gr in enumerate(grs):
    gr.SetName('gr_'+channelName[idx])

#### open histos and make purity plots
pattern = 'results/purity_nonresonant_*.root' if args.nonres else 'results/purity_resonant_*.root'
inputFiles = glob.glob(pattern)

## good! globs already sorts for me
replaceLeft = 'results/purity_nonresonant_' if args.nonres else 'results/purity_resonant_'
masses = []
for inf in inputFiles:
    inf = inf.replace(replaceLeft, '')
    inf = inf.replace('.root', '')
    inf = float(inf)
    masses.append(inf)

# print inputFiles
# print masses

## make the plot
for idx, fname in enumerate(inputFiles):
    fIn = ROOT.TFile.Open(fname)
    h0 = fIn.Get('hDecayMode0')
    h1 = fIn.Get('hDecayMode1')
    h2 = fIn.Get('hDecayMode2')

    h0unsc = fIn.Get('unscaled_hDecayMode0')
    h1unsc = fIn.Get('unscaled_hDecayMode1')
    h2unsc = fIn.Get('unscaled_hDecayMode2')

    purity0 = h0.GetBinContent(0 + 1)
    purity1 = h1.GetBinContent(1 + 1)
    purity2 = h2.GetBinContent(2 + 1)

    ## by hand
    err0up = ROOT.TEfficiency.Bayesian(float(h0unsc.GetEntries()), float(h0unsc.GetBinContent(0 + 1)), 0.683, 1, 1, True) / float(h0unsc.GetBinContent(0 + 1))
    err1up = ROOT.TEfficiency.Bayesian(float(h1unsc.GetEntries()), float(h1unsc.GetBinContent(1 + 1)), 0.683, 1, 1, True) / float(h1unsc.GetBinContent(1 + 1))
    err2up = ROOT.TEfficiency.Bayesian(float(h2unsc.GetEntries()), float(h2unsc.GetBinContent(2 + 1)), 0.683, 1, 1, True) / float(h2unsc.GetBinContent(2 + 1))

    err0do = ROOT.TEfficiency.Bayesian(float(h0unsc.GetEntries()), float(h0unsc.GetBinContent(0 + 1)), 0.683, 1, 1, False) / float(h0unsc.GetBinContent(0 + 1))
    err1do = ROOT.TEfficiency.Bayesian(float(h1unsc.GetEntries()), float(h1unsc.GetBinContent(1 + 1)), 0.683, 1, 1, False) / float(h1unsc.GetBinContent(1 + 1))
    err2do = ROOT.TEfficiency.Bayesian(float(h2unsc.GetEntries()), float(h2unsc.GetBinContent(2 + 1)), 0.683, 1, 1, False) / float(h2unsc.GetBinContent(2 + 1))

    ### not sure this is 100% correct.. but I get upper errors even if eff = 100%
    if purity0 == 1.0:
        err0up = 0.0
    if purity1 == 1.0:
        err1up = 0.0
    if purity2 == 1.0:
        err2up = 0.0

    # print purity0, purity1, purity2
    # # ## uisng tEFf
    # h0unscTOT = h0unsc.Clone(h0unsc.GetName()+'TOT')
    # h1unscTOT = h1unsc.Clone(h1unsc.GetName()+'TOT')
    # h2unscTOT = h2unsc.Clone(h2unsc.GetName()+'TOT')
    # h0unscTOT.Reset()
    # h1unscTOT.Reset()
    # h2unscTOT.Reset()
    # for ibin in range (1,h0unscTOT.GetNbinsX()+1):
    #     h0unscTOT.SetBinContent(ibin, h0unsc.Integral())
    #     h1unscTOT.SetBinContent(ibin, h1unsc.Integral())
    #     h2unscTOT.SetBinContent(ibin, h2unsc.Integral())

    # # for ibin in range (1, h0unsc.GetNbinsX()+1):
    # #     print ibin, h0unsc.GetBinContent(ibin), h0unscTOT.GetBinContent(ibin)
    # eff0 = ROOT.TEfficiency(h0unsc, h0unscTOT)
    # eff1 = ROOT.TEfficiency(h1unsc, h1unscTOT)
    # eff2 = ROOT.TEfficiency(h2unsc, h2unscTOT)
    
    # gr0 = eff0.CreateGraph()
    # gr1 = eff1.CreateGraph()
    # gr2 = eff2.CreateGraph()


    # err0up = gr0.GetErrorYhigh(0)
    # err1up = gr1.GetErrorYhigh(1)
    # err2up = gr2.GetErrorYhigh(2)

    # err0do = gr0.GetErrorYlow(0)
    # err1do = gr1.GetErrorYlow(1)
    # err2do = gr2.GetErrorYlow(2)


    massval = masses[idx]
    if args.nonres:
        massval -= 20.0
    grs[0].SetPoint(grs[0].GetN(), massval, purity0)
    grs[1].SetPoint(grs[1].GetN(), massval, purity1)
    grs[2].SetPoint(grs[2].GetN(), massval, purity2)

    grs[0].SetPointEXlow(grs[0].GetN()-1, 0)
    grs[1].SetPointEXlow(grs[1].GetN()-1, 0)
    grs[2].SetPointEXlow(grs[2].GetN()-1, 0)

    grs[0].SetPointEXhigh(grs[0].GetN()-1, 0)
    grs[1].SetPointEXhigh(grs[1].GetN()-1, 0)
    grs[2].SetPointEXhigh(grs[2].GetN()-1, 0)

    grs[0].SetPointEYlow(grs[0].GetN()-1, err0do)
    grs[1].SetPointEYlow(grs[1].GetN()-1, err1do)
    grs[2].SetPointEYlow(grs[2].GetN()-1, err2do)

    grs[0].SetPointEYhigh(grs[0].GetN()-1, err0up)
    grs[1].SetPointEYhigh(grs[1].GetN()-1, err1up)
    grs[2].SetPointEYhigh(grs[2].GetN()-1, err2up)


mg = ROOT.TMultiGraph('mg', '')
for idx, gr in enumerate(grs):
    gr.SetLineColor(colors[idx])
    gr.SetMarkerColor(colors[idx])
    gr.SetMarkerStyle(8)
    gr.SetMarkerSize(0.8)
    # gr.SetLineWidth(1.0)
    mg.Add(gr)

c1 = ROOT.TCanvas('c1', 'c1', 600, 600)
c1.SetLeftMargin(0.15)
c1.SetFrameLineWidth(3)
mg.Draw('APL')

if args.nonres: mg.SetMinimum(0.98)
else : mg.SetMinimum(0.96)
xtitle = 'k_{#lambda}' if args.nonres else 'm_{X} [GeV]'
mg.GetXaxis().SetTitle(xtitle)
mg.GetYaxis().SetTitleOffset(2.)
mg.GetYaxis().SetTitle("Fraction of events correcly classified")

mg.Draw('APL')

leg = ROOT.TLegend(0.265, 0.134, 0.865, 0.178)
leg.SetNColumns(3)
for idx, gr in enumerate(grs):
    leg.AddEntry(gr, channelNameLatex[idx], 'lep')
leg.SetBorderSize(0)
leg.Draw()
c1.Update()
raw_input()

outName = 'classification_'
outName += "nonres.pdf" if args.nonres else 'res.pdf'
c1.Print(outName, 'pdf')