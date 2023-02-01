import ROOT
import json
import numpy as np

WorkingPoint = "Tight"
period = "2018"

binedges_eta = np.array([-2.5, -2.0, -1.566, -1.444, -1.0, -0.5, 0.0, 0.5, 1.0, 1.444, 1.566, 2.0, 2.5])
binedges_pt = [10., 20., 45., 75., 100., 500.]

binval_pt = np.array([15., 32.5, 60, 87.5, 300.])
binerr_pt = np.array([5., 12.5, 15, 12.5, 200.])

binNames_eta = ["Ltm2p0","m2p0tom1p57","m1p57tom1p44","m1p44tom1p0","m1p0tom0p5","m0p5to0p0","0p0to0p5","0p5to1p0","1p0to1p44","1p44to1p57","1p57to2p0","Gt2p0"]

with open('{0}_{1}_eleSFs.json'.format(period,WorkingPoint)) as f_sf:
    sf_per_eta = json.load(f_sf)

with open('{0}_{1}_eleSys.json'.format(period,WorkingPoint)) as f_sf:
    sys_per_eta = json.load(f_sf)


binHist = ROOT.TH1F("etaBinsH","",len(binNames_eta),binedges_eta)

tgraph_per_eta = {}

for bin,name in enumerate(binNames_eta):

    binHist.GetXaxis().SetBinLabel(bin+1,'Eta'+name)


    tgraph_per_eta[name] = ROOT.TGraphAsymmErrors(len(binval_pt),
                                                  binval_pt,
                                                  np.array(sf_per_eta[name]),
                                                  binerr_pt,
                                                  binerr_pt,
                                                  np.array(sys_per_eta[name]),
                                                  np.array(sys_per_eta[name]))

#binHist.Draw()

f_out = ROOT.TFile('{0}_{1}_eleSFs.root'.format(period,WorkingPoint), 'RECREATE')

binHist.Write("etaBinsH")
for etabin in binNames_eta:
    tgraph_per_eta[etabin].Write("ZmassEta"+etabin+"_SF")

plotbin = "m0p5to0p0"
tgraph_per_eta[plotbin].Draw()

print sf_per_eta[plotbin]
print sys_per_eta[plotbin]

f_out.Close()
