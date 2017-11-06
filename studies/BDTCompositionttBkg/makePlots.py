import ROOT
ROOT.gROOT.SetBatch(True)

# chSemiLept = ROOT.TChain('HTauTauTree')
# chFullyLept = ROOT.TChain('HTauTauTree')
# chSemiLept.Add('/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_28Feb/SKIM_TT_semiLep/*.root')
# chFullyLept.Add('/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_28Feb/SKIM_TT_fullyLep/*.root')

# ch = ROOT.TChain('HTauTauTree')
# ch.Add('/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_28Feb/SKIM_TT_semiLep/*.root')
# outname = 'plots_semilep.root'

ch = ROOT.TChain('HTauTauTree')
ch.Add('/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_28Feb/SKIM_TT_fullyLep/*.root')
outname = 'plots_fullylep.root'

cut = 'pairType == 0 && dau1_pt > 23 && abs (dau1_eta) < 2.1 && dau2_pt > 20 && abs (dau2_eta) < 2.3 && nleps == 0 && bjet1_bID > 0.8484 && bjet2_bID > 0.8484 && ((tauH_SVFIT_mass-116.)*(tauH_SVFIT_mass-116.))/(35.*35.) + ((bH_mass_raw-111.)*(bH_mass_raw-111.))/(45.*45.) <  1.0 && isOS != 0 && dau1_iso < 0.15 && dau2_MVAiso >= 3'

promptMuTauDecay           = '( (genDecMode1 == 1 && genDecMode2 == 5) || (genDecMode1 == 5 && genDecMode2 == 1) )'
fromtauMuTauDecay          = '( (genDecMode1 == 3 && genDecMode2 == 5) || (genDecMode1 == 5 && genDecMode2 == 3) )'
promptmuplusFakeTauDecay   = '( (genDecMode1 == 1 && genDecMode2 == 0) || (genDecMode1 == 0 && genDecMode2 == 1) )'
fromtaumuplusFakeTauDecay  = '( (genDecMode1 == 3 && genDecMode2 == 0) || (genDecMode1 == 0 && genDecMode2 == 3) )'

# BDTResonantLM
# genDecMode1
# genDecMode2

#     enum WDecay {
#       Had      = 0, // W->qqbar
#       MuPrompt = 1,
#       EPrompt  = 2,
#       TauMu    = 3, // W->tau->mu
#       TauE     = 4, // W->tau->e
#       TauHad   = 5, // W->tau->tauh
#       other  = 6
# };

h_promptMuTauDecay = ROOT.TH1F('h_promptMuTauDecay', 'h_promptMuTauDecay', 100, -1, 1)
h_fromtauMuTauDecay = ROOT.TH1F('h_fromtauMuTauDecay', 'h_fromtauMuTauDecay', 100, -1, 1)
h_promptmuplusFakeTauDecay = ROOT.TH1F('h_promptmuplusFakeTauDecay', 'h_promptmuplusFakeTauDecay', 100, -1, 1)
h_fromtaumuplusFakeTauDecay = ROOT.TH1F('h_fromtaumuplusFakeTauDecay', 'h_fromtaumuplusFakeTauDecay', 100, -1, 1)

ch.Draw('BDTResonantLM>>h_promptMuTauDecay', (cut + ' && ' + promptMuTauDecay) )
ch.Draw('BDTResonantLM>>h_fromtauMuTauDecay', (cut + ' && ' + fromtauMuTauDecay) )
ch.Draw('BDTResonantLM>>h_promptmuplusFakeTauDecay', (cut + ' && ' + promptmuplusFakeTauDecay) )
ch.Draw('BDTResonantLM>>h_fromtaumuplusFakeTauDecay', (cut + ' && ' + fromtaumuplusFakeTauDecay) )

fOut = ROOT.TFile(outname, 'recreate')
h_promptMuTauDecay.Write()
h_fromtauMuTauDecay.Write()
h_promptmuplusFakeTauDecay.Write()
h_fromtaumuplusFakeTauDecay.Write()