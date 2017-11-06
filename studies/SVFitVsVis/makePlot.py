import ROOT
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)

c1 = ROOT.TCanvas("c1", "c1", 600, 600)
c1.SetFrameLineWidth(3)

ch = ROOT.TChain('HTauTauTree')
ch.Add('/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_28Feb/SKIM_HH_lambdarew_21/*.root')

cut = 'MC_weight * IdAndIsoSF * trigSF * PUReweight * ((pairType==0 || pairType==1) && nleps == 0 && dau2_MVAiso >= 3 && isOS == 1)'

hVis   = ROOT.TH1F('hVis', '', 50, 0, 200)
hSVfit = ROOT.TH1F('hSVfit', '', 50, 0, 200)

ch.Draw("tauH_mass>>hVis", cut)
ch.Draw("tauH_SVFIT_mass>>hSVfit", cut)

hVis.SetLineColor(ROOT.kBlue)
hSVfit.SetLineColor(ROOT.kRed)

hVis.Scale(1./hVis.Integral())
hSVfit.Scale(1./hSVfit.Integral())

mmax = max(hVis.GetMaximum(),hSVfit.GetMaximum())

hVis.SetTitle(';m_{#tau#tau} [GeV]; a.u.')
hVis.GetXaxis().SetTitleOffset(1.4)
hVis.GetYaxis().SetTitleOffset(1.4)

hVis.SetMaximum(1.15*mmax)
hVis.Draw('hist')
hSVfit.Draw('hist same')


leg = ROOT.TLegend(0.2, 0.82, 0.8, 0.90)
leg.SetBorderSize(0)
leg.SetFillStyle(0)
leg.SetNColumns(2)
leg.AddEntry(hVis, "Visible mass")
leg.AddEntry(hSVfit, "SVfit mass")
leg.Draw()

c1.Update()
c1.Print('svfit_vs_vis.pdf', 'pdf')


print hVis.GetRMS()/hVis.GetMean()
print hSVfit.GetRMS()/hSVfit.GetMean()