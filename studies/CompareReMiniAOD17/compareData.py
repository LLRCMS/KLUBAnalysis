from ROOT import *
from array import array

chOld = TChain('HTauTauTree')
chNew = TChain('HTauTauTree')

# chOld.Add('/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_4Mar/SKIM_Tau/*.root')
# chNew.Add('/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_5Mar/SKIM_Tau/*.root')

# binning = [0, 25, 50, 75, 100, 125, 150, 200, 250, 300, 500]

# hOld = TH1F("hOld", "hOld", len(binning)-1, array('d', binning))
# hNew = TH1F("hNew", "hNew", len(binning)-1, array('d', binning))

# sel = 'pairType == 2 && dau1_pt > 45 && abs (dau1_eta) < 2.1 && dau2_pt > 45 && abs (dau2_eta) < 2.1 && nleps == 0 && bjet1_bID > 0.8484 && bjet2_bID > 0.8484 && isOS != 0 && dau1_MVAiso >= 3 && dau2_MVAiso >= 3'

######################

chOld.Add('/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_4Mar/SKIM_SingleElectron/*.root')
chNew.Add('/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_5Mar/SKIM_SingleElectron/*.root')

binning = [0, 25, 50, 75, 100, 125, 150, 175, 200, 250, 300, 500]

hOld = TH1F("hOld", "hOld", len(binning)-1, array('d', binning))
hNew = TH1F("hNew", "hNew", len(binning)-1, array('d', binning))

sel = 'pairType == 1 && dau1_pt > 27 && abs(dau1_eta) < 2.1 && dau2_pt > 20 && abs(dau2_eta) < 2.3 && nleps == 0 && bjet1_bID > 0.8484 && bjet2_bID > 0.8484 && BDTResonantLM > -0.0764 && isBoosted != 1 && ((tauH_SVFIT_mass-116.)*(tauH_SVFIT_mass-116.))/(35.*35.) + ((bH_mass_raw-111.)*(bH_mass_raw-111.))/(45.*45.) <  1.0'

c1 = TCanvas("c1", "c1", 600, 600)

chOld.Draw('MT2>>hOld', sel)
chNew.Draw('MT2>>hNew', sel)

chOld.SetLineColor(kRed)
chNew.SetLineColor(kBlack)
chNew.SetMarkerColor(kBlack)
chNew.SetMarkerSize(0.8)
chNew.SetMarkerStyle(8)

print "DONE"

hOld.Draw("hist")
hNew.Draw("pesame")

c1.Update()
raw_input()
