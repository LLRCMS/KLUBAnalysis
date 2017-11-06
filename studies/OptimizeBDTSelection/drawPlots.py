import ROOT
ROOT.gStyle.SetOptStat(0)

def setStyle(grIn, color, lstyle):
    grIn.SetMarkerColor(color)
    grIn.SetLineColor(color)
    grIn.SetLineStyle(lstyle)

def setStyleArr(grInArr, colorArr, lstyleArr=None):
    for idx, grIn in enumerate(grInArr):
        color = colorArr[idx]
        lstyle = lstyleArr[idx] if lstyleArr else 1
        grIn.SetMarkerColor(color)
        grIn.SetLineColor(color)
        grIn.SetLineStyle(lstyle)

fIn = ROOT.TFile.Open('limit_plots.root')

### make plot of low mass bdt resonant
base = ['s2b0jresolvedMcut']
baseMT = ['s2b0jresolvedMcutMTCut']
basenr = ['s2b0jresolvedMcutnr50', 's2b0jresolvedMcutnr60', 's2b0jresolvedMcutnr70', 's2b0jresolvedMcutnr80', 's2b0jresolvedMcutnr85', 's2b0jresolvedMcutnr90']
baserlm = ['s2b0jresolvedMcutlmr50', 's2b0jresolvedMcutlmr60', 's2b0jresolvedMcutlmr70', 's2b0jresolvedMcutlmr80', 's2b0jresolvedMcutlmr85', 's2b0jresolvedMcutlmr90']
baserhm = ['s2b0jresolvedMcuthmr50', 's2b0jresolvedMcuthmr60', 's2b0jresolvedMcuthmr70', 's2b0jresolvedMcuthmr80', 's2b0jresolvedMcuthmr85', 's2b0jresolvedMcuthmr90']

## the ICHEP16 selection
grBase = fIn.Get('gr_'+base[0])
setStyle(grBase, ROOT.kBlack, 7)

## the RunI selection
grRunI = fIn.Get('gr_'+baseMT[0])
setStyle(grRunI, ROOT.kGray+1, 7)

WPs = [50, 60, 70, 80, 85, 90]
colors = [ROOT.kBlue, ROOT.kRed, ROOT.kGreen+1, ROOT.kMagenta, ROOT.kOrange, ROOT.kViolet-6]


## non resonant BDT
grNR = [fIn.Get('gr_'+x) for x in basenr]

## low mass resonant BTD
grLMR = [fIn.Get('gr_'+x) for x in baserlm]

## high mass resonant BTD
grHMR = [fIn.Get('gr_'+x) for x in baserhm]

################################################
c1 = ROOT.TCanvas("c1", "c1", 600, 600)
c1.SetLogy()
c1.SetGridx()
c1.SetGridy()

#### plot1 : the various WPs of bdt low mass resonant - what is the best one?
setStyleArr(grLMR, colors)

mg = ROOT.TMultiGraph("mg1", "Resonant BDT - low mass;m_{X} [GeV]; 95% upper C.L. on #sigma #times BR(bb#tau#tau)")
mg.Add(grBase)
mg.Add(grRunI)
for x in grLMR:
    mg.Add(x)
mg.Draw("APL")
mg.GetXaxis().SetLimits(250, 400)
mg.Draw()

leg = ROOT.TLegend(0.5, 0.5, 0.88, 0.88)
leg.AddEntry(grBase, "only inv. mass cut", "pl")
leg.AddEntry(grRunI, "mT < 30 GeV", "pl")
for idx, x in enumerate(grLMR):
    leg.AddEntry(x, "BDT bkg rej." + str(WPs[idx]) + '%', "pl")

leg.SetFillStyle(0)
leg.SetBorderSize(0)
leg.Draw()
# grRunI.Draw("APL")
# grRunI.Draw("PL same")
# for x in grLMR:
#     x.Draw("PL same")
c1.Update()
raw_input()