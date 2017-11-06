from ROOT import *

def retrieveHisto (rootFile, name, var, sel):
    fullName = name + '_' + sel + '_' + var
    if not rootFile.GetListOfKeys().Contains(fullName):
        print "*** WARNING: histo " , fullName , " not available"
        return None
    hFound = rootFile.Get(fullName)
    return hFound

# fIn = TFile.Open('/home/llr/cms/cadamuro/testAnalysisHelper2/CMSSW_7_4_7/src/KLUBAnalysis/analysis_MuTau_30Gen_BDTROC/outPlotter.root')
fIn = TFile.Open('/home/llr/cms/cadamuro/testAnalysisHelper2/CMSSW_7_4_7/src/KLUBAnalysis/analysis_MuTau_15Feb/outPlotter.root')
sel = 's2b0jresolvedMcut_SR'
sig = 'lambdarew21'
# sel = 's2b0jMcut_SR'
# sel = '2b0j_SR'

varList = ['BDTResonantLM', 'BDTResonantHM', 'BDTNonResonant', 'LepTauKine']
varNames = {
    'BDTResonantLM' : "LM resonant",
    'BDTResonantHM' : "HM resonant",
    'BDTNonResonant' : "Non-resonant 2017",
    'LepTauKine' : "Non-resonant ICHEP 2016",
}

colors = {
    'BDTResonantLM'   : kRed,
    'BDTResonantHM'   : kBlue,
    'BDTNonResonant'  : kGreen+1,
    'LepTauKine'      : kGray,
}


# var = 'LepTauKine'

def makeROC(hSig, hBkg):
    gr = TGraph()
    totSig = hSig.Integral()
    totBkg = hBkg.Integral()
    for ipt in range(0, hSig.GetNbinsX()):
        effSig = hSig.Integral(ipt+1, -1) / totSig
        effBkg = hBkg.Integral(ipt+1, -1) / totBkg
        gr.SetPoint(ipt, effSig, 1.-effBkg)
    return gr

hSigs = []
for var in varList:
    h = retrieveHisto(fIn, sig, var, sel)
    hSigs.append(h)

# hSigSM = retrieveHisto(fIn, 'HHSM', var, sel)
# hSigR300 = retrieveHisto(fIn, 'Radion300', var, sel)
# hSigR650 = retrieveHisto(fIn, 'Radion650', var, sel)
# hSigR900 = retrieveHisto(fIn, 'Radion900', var, sel)

hTTs = []
for var in varList:
    # hTT = retrieveHisto(fIn, 'TTfullyLep', var, sel)
    # hTT.Add(retrieveHisto(fIn, 'TTsemiLep', var, sel))
    # hTT.Add(retrieveHisto(fIn, 'TTfullyHad', var, sel))
    hTT = retrieveHisto(fIn, 'TT', var, sel)
    hTTs.append(hTT)

ROCs = []
for idx , sig in enumerate(hSigs):
    ROC = makeROC(sig, hTTs[idx])
    ROC.SetMarkerColor(colors[varList[idx]])
    ROC.SetLineColor(colors[varList[idx]])
    ROC.SetLineStyle(1)
    ROC.SetMarkerSize(0.4)
    ROC.SetLineWidth(2)
    ROC.SetMarkerStyle(8)    
    ROCs.append(ROC)

mg = TMultiGraph()
for roc in ROCs:
    mg.Add(roc)

# ROC_SM = makeROC(hSigSM, hTT)
# ROC_SM.SetMarkerColor(kRed)
# ROC_SM.SetLineColor(kRed)
# ROC_SM.SetLineStyle(7)
# ROC_SM.SetMarkerSize(0.4)
# ROC_SM.SetLineWidth(3)
# ROC_SM.SetMarkerStyle(8)

# ROC_R300 = makeROC(hSigR300, hTT)
# ROC_R300.SetMarkerColor(kBlue)
# ROC_R300.SetLineColor(kBlue)
# ROC_R300.SetMarkerSize(0.4)
# ROC_R300.SetLineWidth(3)
# ROC_R300.SetMarkerStyle(8)

# ROC_R650 = makeROC(hSigR650, hTT)
# ROC_R650.SetMarkerColor(kGreen+1)
# ROC_R650.SetLineColor(kGreen+1)
# ROC_R650.SetMarkerSize(0.4)
# ROC_R650.SetLineWidth(3)
# ROC_R650.SetMarkerStyle(8)

# ROC_R900 = makeROC(hSigR900, hTT)
# ROC_R900.SetMarkerColor(kOrange-1)
# ROC_R900.SetLineColor(kOrange-1)
# ROC_R900.SetMarkerSize(0.4)
# ROC_R900.SetLineWidth(3)
# ROC_R900.SetMarkerStyle(8)

leg = TLegend(0.14, 0.14, 0.6, 0.4)
for idx, var in enumerate(varList):
    print idx, var
    leg.AddEntry(ROCs[idx], varNames[var], "l")
# leg.AddEntry(ROC_SM, 'SM', 'l')
# leg.AddEntry(ROC_R300, 'M_{X} = 300 GeV', 'l')
# leg.AddEntry(ROC_R650, 'M_{X} = 650 GeV', 'l')
# leg.AddEntry(ROC_R900, 'M_{X} = 900 GeV', 'l')
leg.SetFillColor(kWhite)
leg.SetTextFont(42)
leg.SetTextSize(0.03)

c1 = TCanvas("c1", "c1", 600, 600)
c1.SetGridx()
c1.SetGridy()
mg.SetTitle(";sig. eff.;bkg. rej")
mg.Draw("APL")
# ROC_SM.Draw("AL")
# ROC_SM.GetXaxis().SetTitle('sig. eff.')
# ROC_SM.GetYaxis().SetTitle('bkg. rej.')
# ROC_SM.Draw("AL")
# ROC_R300.Draw("L same")
# ROC_R650.Draw("L same")
# ROC_R900.Draw("L same")
leg.Draw()
c1.Update()
raw_input()

# c1.Print('ROC_'+var+'.pdf', 'pdf')
c1.Print('ROCs_BDTS.pdf', 'pdf')