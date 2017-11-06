from ROOT import *

def retrieveHisto (rootFile, name, var, sel):
    fullName = name + '_' + sel + '_' + var
    if not rootFile.GetListOfKeys().Contains(fullName):
        print "*** WARNING: histo " , fullName , " not available"
        return None
    hFound = rootFile.Get(fullName)
    return hFound

def makeROC(hSig, hBkg):
    gr = TGraph()
    totSig = hSig.Integral()
    totBkg = hBkg.Integral()
    for ipt in range(0, hSig.GetNbinsX()):
        effSig = hSig.Integral(ipt+1, -1) / totSig
        effBkg = hBkg.Integral(ipt+1, -1) / totBkg
        gr.SetPoint(ipt, effSig, 1.-effBkg)
    return gr

def makeMarker(hSigCut, hBkgCut, hSig, hBkg):
    sigEff = hSigCut.Integral()/hSig.Integral()
    bkgEff = hBkgCut.Integral()/hBkg.Integral()
    bkgRej = 1.-bkgEff
    mr = TMarker(sigEff, bkgRej, 29)
    return mr

def makeName(signame, thetype):
    if thetype == 'rad':
        if not 'Radion' in signame:
            print "cannot make name", signame, thetype
            return signame
        x = signame.replace('Radion', 'M_{X} = ')
        x += " GeV"
        return x
    
    elif thetype == 'lambda':
        if not 'lambdarew' in signame:
            print "cannot make name", signame, thetype
            return signame
        num = int(signame.replace('lambdarew', ''))
        num = num - 20 # the offset in nmbering histos
        # print num
        x = 'k_{#lambda} = '
        x += str(num)
        return x

    else:
        return signame

fIn = TFile.Open('/home/llr/cms/cadamuro/testAnalysisHelper2/CMSSW_7_4_7/src/KLUBAnalysis/analysis_MuTau_30Gen_BDTROC/outPlotter.root')
sel = 's2b0jresolvedMcut_SR'
sel_mtcut = 's2b0jresolvedMcutMTCut_SR'
# sel = 's2b0jMcut_SR'
# sel = '2b0j_SR'
# var = 'BDTResonantLM'
# var = 'BDTResonantHM'
var = 'LepTauKine'

thetype = 'rad'
# sigList = ["Radion250", "Radion270", "Radion300", "Radion340", "Radion350", "Radion400", "Radion500", "Radion550", "Radion650", "Radion700", "Radion750", "Radion800", "Radion900"]
sigList = ["Radion250", "Radion270", "Radion300", "Radion350", "Radion400", "Radion500", "Radion650", "Radion750", "Radion900"]

# thetype = 'lambda'
# sigList = ["lambdarew0", "lambdarew10", "lambdarew18", "lambdarew19", "lambdarew20", "lambdarew21", "lambdarew22", "lambdarew30", "lambdarew40"]

hSigs = []
for sig in sigList:
    hSigs.append(retrieveHisto(fIn, sig, var, sel))

hTT = retrieveHisto(fIn, 'TTfullyLep', var, sel)
hTT.Add(retrieveHisto(fIn, 'TTsemiLep', var, sel))
hTT.Add(retrieveHisto(fIn, 'TTfullyHad', var, sel))


ROCs = []
for idx, sig in enumerate(hSigs):
    ROCs.append(makeROC(sig, hTT))
    ROCs[-1].SetLineColor(idx+1)
    ROCs[-1].SetMarkerColor(idx+1)
    # ROCs[-1].SetLineStyle(7)
    ROCs[-1].SetMarkerSize(0.4)
    ROCs[-1].SetLineWidth(3)
    ROCs[-1].SetMarkerStyle(8)


## comparing against the mt cut
hSigsMTCut = []
for sig in sigList:
    hSigsMTCut.append(retrieveHisto(fIn, sig, var, sel_mtcut))
hTTMTCut = retrieveHisto(fIn, 'TTfullyLep', var, sel_mtcut)
hTTMTCut.Add(retrieveHisto(fIn, 'TTsemiLep', var, sel_mtcut))
hTTMTCut.Add(retrieveHisto(fIn, 'TTfullyHad', var, sel_mtcut))
markers = []
for idx , sig in enumerate(hSigsMTCut):
    markers.append(makeMarker(sig, hTTMTCut, hSigs[idx], hTT))
    markers[-1].SetMarkerColor(idx+1)
    markers[-1].SetMarkerSize(3)
    markers[-1].SetMarkerStyle(29)
fakeMarker = markers[0].Clone()
fakeMarker.SetMarkerColor(kGray)
######


leg = TLegend(0.14, 0.14, 0.6, 0.6)
for idx, sig in enumerate(sigList):
  entryName = makeName(sig, thetype)
  leg.AddEntry(ROCs[idx], entryName, 'l')
    # leg.AddEntry(ROC_SM, 'SM', 'l')
# leg.SetFillColor(kWhite)
leg.AddEntry (fakeMarker, 'm_{T} < 30 GeV', 'p')

leg.SetFillStyle(0)
leg.SetBorderSize(0)
leg.SetTextFont(42)
leg.SetTextSize(0.05)

c1 = TCanvas("c1", "c1", 600, 600)
c1.SetGridx()
c1.SetGridy()
ROCs[0].Draw("AL")
ROCs[0].SetTitle(var)
ROCs[0].GetXaxis().SetTitle('sig. eff.')
ROCs[0].GetYaxis().SetTitle('bkg. rej.')
ROCs[0].Draw("AL")
for i in range(1, len(ROCs)):
    ROCs[i].Draw("L same")

for m in markers:
    m.Draw()

leg.Draw()
c1.Update()
raw_input()

c1.Print('ROC_'+var+'_'+thetype+'.pdf', 'pdf')