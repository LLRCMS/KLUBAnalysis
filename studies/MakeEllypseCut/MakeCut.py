from ROOT import *
import numpy as np

ch = TChain('HTauTauTree')
# ch.Add('/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_10Gen/SKIM_HH_node_SM/*.root')
ch.Add('/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_10Gen/SKIM_TT_semiLep/output_1*.root')

cutProto = 'pairType == 0 && dau1_pt > 23 && abs (dau1_eta) < 2.1 && dau2_pt > 20 && abs (dau2_eta) < 2.3 && nleps == 0 && dau1_iso < 0.15 && dau2_MVAiso >= 3 && isOS != 0 && bjet1_bID > 0.800 && bjet2_bID > 0.800'

# TTsteps = np.linspace(20,60,40)
# print TTsteps
# BBsteps = np.linspace(20,60,40)
# print BBsteps

# c1 = TCanvas("c1", "c1", 600, 600)
# cutTot = cutProto + '&&' + '( ((tauH_SVFIT_mass-116.)*(tauH_SVFIT_mass-116.))/(%f*%f) + ((bH_mass_raw-111.)*(bH_mass_raw-111.))/(%f*%f) ) < 1.' % (30.,30., 60.,60.)
# ch.Draw('bH_mass_raw:tauH_SVFIT_mass>>h(100, 40, 200, 100, 40, 200)',cutTot, 'colz')
# c1.Update()
# raw_input()



TTsteps = np.linspace(20,60,10)
print TTsteps
BBsteps = np.linspace(20,60,10)
print BBsteps

norm = ch.GetEntries(cutProto)

h2d = TH2F ('h2d', ';tt axis, bb axis', len(TTsteps)-1, TTsteps[0]-0.5, TTsteps[-1]-0.5, len(BBsteps)-1, BBsteps[0]-0.5, BBsteps[-1]-0.5)
for idxTT, xTT in enumerate(TTsteps):
    if idxTT % 10 == 0: print 'xTT step: ' , idxTT
    for idxBB, xBB in enumerate(BBsteps):
        if idxBB % 10 == 0: print '   .... xBB step: ' , idxBB
        cutTot = cutProto + '&&' + '( ((tauH_SVFIT_mass-116.)*(tauH_SVFIT_mass-116.))/(%f*%f) + ((bH_mass_raw-111.)*(bH_mass_raw-111.))/(%f*%f) ) < 1.' % (xTT,xTT, xBB,xBB)
        evts = ch.GetEntries(cutTot)

        bb = h2d.FindBin(xTT, xBB)
        h2d.SetBinContent(bb, 1.*evts/norm)

c1 = TCanvas("c1", "c1", 600, 600)
h2d.Draw('colz text')
c1.Update()
raw_input()

f1 = TFile('circOutput_TT.root', 'recreate')
f1.cd()
h2d.Write()