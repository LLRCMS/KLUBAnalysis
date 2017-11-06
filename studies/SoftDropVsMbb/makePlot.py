import ROOT
ROOT.gROOT.SetBatch(True)

ch = ROOT.TChain("HTauTauTree")
ch.Add('/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_28Feb/SKIM_GluGluToRadionToHHTo2B2Tau_M-750/*.root')

hbb = ROOT.TH1F('hbb', '', 50, 50, 250)
hSoftDrop = ROOT.TH1F('hSoftDrop', '', 50, 50, 250)

cut = '(pairType==0 || pairType==1) && isBoosted && nleps == 0 && bjet1_bID > 0.800 && bjet2_bID > 0.800'
ch.Draw('bH_mass_raw>>hbb', cut)
ch.Draw('fatjet_softdropMass>>hSoftDrop', cut)

c1 = ROOT.TCanvas("c1", "c1", 600, 600)

print hbb.Integral(), hSoftDrop.Integral()

hbb.SetLineColor(ROOT.kBlue)
hSoftDrop.SetLineColor(ROOT.kRed)

hbb.Scale(1./hbb.Integral())
hSoftDrop.Scale(1./hSoftDrop.Integral())

mmax = max(hbb.GetMaximum(), hSoftDrop.GetMaximum())

print hbb.GetMean(), hbb.GetRMS(), hbb.GetRMS()/hbb.GetMean()
print hSoftDrop.GetMean(), hSoftDrop.GetRMS(), hSoftDrop.GetRMS()/hSoftDrop.GetMean()

hbb.SetMaximum(1.15*mmax)

hbb.Draw()
hSoftDrop.Draw('same')

c1.Update()
c1.Print("mbb_vs_softdrop.pdf", 'pdf')