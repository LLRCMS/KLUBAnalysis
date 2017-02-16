from ROOT import *

def retrieveHisto (rootFile, name, var, sel):
    fullName = name + '_' + sel + '_' + var
    if not rootFile.GetListOfKeys().Contains(fullName):
        print "*** WARNING: histo " , fullName , " not available"
        return None
    hFound = rootFile.Get(fullName)
    return hFound

def makeRej(hBkg):
    gr = TGraph()
    totBkg = hBkg.Integral()
    thresh = [.50, .60, .70, .80, .85, .90]
    passed = [False for idx in range(0, len(thresh))]

    for ipt in range(0, hBkg.GetNbinsX()):
        effBkg = hBkg.Integral(ipt+1, -1) / totBkg
        gr.SetPoint(ipt, hBkg.GetBinCenter(ipt+1), 1.-effBkg)
        rejBkg = 1.-effBkg
        for idx, x in enumerate(thresh):
            if rejBkg > x and not passed[idx]:
                print "bkj rej: " , x , 'for BDT > ', hBkg.GetBinCenter(ipt+1)
                passed[idx] = True
    return gr


# fIn = TFile.Open('/home/llr/cms/cadamuro/testAnalysisHelper2/CMSSW_7_4_7/src/KLUBAnalysis/analysis_MuTau_30Gen_BDTROC/outPlotter.root')
fIn = TFile.Open('/home/llr/cms/cadamuro/testAnalysisHelper2/CMSSW_7_4_7/src/KLUBAnalysis/analysis_MuTau_15Feb/outPlotter.root')
sel = 's2b0jresolvedMcut_SR'
# sel = 's2b0jMcut_SR'
# sel = '2b0j_SR'
# var = 'BDTResonantLM'
var = 'BDTNonResonant'
# var = 'LepTauKine'

hTT = retrieveHisto(fIn, 'TT', var, sel)
# hTT = retrieveHisto(fIn, 'TTfullyLep', var, sel)
# hTT.Add(retrieveHisto(fIn, 'TTsemiLep', var, sel))
# hTT.Add(retrieveHisto(fIn, 'TTfullyHad', var, sel))

grRej = makeRej(hTT)
grRej.SetLineColor(kBlue)
grRej.SetMarkerColor(kBlue)
# grRej.SetLineStyle(7)
grRej.SetMarkerSize(0.4)
grRej.SetLineWidth(3)
grRej.SetMarkerStyle(8)

c1 = TCanvas("c1", "c1", 600, 600)
c1.SetGridx()
c1.SetGridy()
grRej.Draw("AL")
grRej.SetTitle(var)
grRej.GetXaxis().SetTitle('BDT output cut')
grRej.GetYaxis().SetTitle('bkg. rej.')
grRej.Draw("AL")

c1.Update()
raw_input()

