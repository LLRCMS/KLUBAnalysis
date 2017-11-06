import ROOT
# import fnmatch

def mergeHistos(*histos):
    for idx, h in enumerate(histos):
        if idx == 0:
            hnew = h.Clone(h.GetName() + '_clone')
        else:
            hnew.Add(h)
    return hnew

## radion
# masses = [250, 270, 280, 300, 350, 400, 450, 500, 550, 600, 650, 750, 900]
# proto_skim_file = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_18Lug_efftest2/SKIM_GluGluToRadionToHHTo2B2Tau_M-{0}/total.root'
# stype = 'Radion'

## graviton
# masses = [250, 260, 270, 280, 300, 320, 340, 350, 400, 450, 500, 550, 600, 650, 750, 800]
# proto_skim_file = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_18Lug_efftest2/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-{0}/total.root'
# stype = 'Graviton'

masses = [x for x in range(0, 52)]

channel = 'ETau'
proto_skim_file = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_18Lug_efftest2/SKIM_HH_lambdarew_{0}/total.root'
stype = 'lambdarew'

proto_filler_file = '/home/llr/cms/cadamuro/testAnalysisHelper2/CMSSW_7_4_7/src/KLUBAnalysis/analysis_%s_18Lug_EffPlots/outPlotter.root' % channel


################################################################
proto_filler_histo = '%s{0}_{1}_SRforEff_MT2' % stype

## xs is 1 pb in skims 
lumi = 35867.

# baseline, sboostedLL, s1b1jresolved, s2b0jresolved, sboostedLLMcut, s1b1jresolvedMcut, s2b0jresolvedMcut, s2b0jresolvedMcutlmr90, s2b0jresolvedMcuthmr90, s1b1jresolvedMcutlmr90, s1b1jresolvedMcuthmr90, s1b1jresolvedMcutlmr70, s2b0jresolvedMcutlmr70

gr_trg = ROOT.TGraph()
gr_tautau = ROOT.TGraph()
gr_3lep = ROOT.TGraph()
gr_2jets = ROOT.TGraph()
gr_btag = ROOT.TGraph()
gr_mcut = ROOT.TGraph()
gr_BDT = ROOT.TGraph() if channel != 'TauTau' else None ## also BDT

for m in masses:
    print ".. doing", m

    f_skim = ROOT.TFile.Open(proto_skim_file.format(m))
    h_skim = f_skim.Get('h_effSummary_effSpecific%s' % channel)
    h_skim_tot = f_skim.Get('h_eff')
    denom_skim = h_skim_tot.GetBinContent(1)

    n_all    = lumi * h_skim.GetBinContent(1) / denom_skim ## no selections, just BR
    n_trg    = lumi * h_skim.GetBinContent(2) / denom_skim
    n_tautau = lumi * h_skim.GetBinContent(8) / denom_skim
    n_3lep   = lumi * h_skim.GetBinContent(9) / denom_skim
    n_2jets  = lumi * h_skim.GetBinContent(10) / denom_skim

    f_filler = ROOT.TFile.Open(proto_filler_file)    
    h_baseline = f_filler.Get(proto_filler_histo.format(m, 'baseline'))
    n_baseline = h_baseline.Integral(-1, -1)

    h_1b1j = f_filler.Get(proto_filler_histo.format(m, 's1b1jresolved'))
    h_2b0j = f_filler.Get(proto_filler_histo.format(m, 's2b0jresolved'))
    h_boos = f_filler.Get(proto_filler_histo.format(m, 'sboostedLL'))
    h_btag = mergeHistos(h_1b1j, h_2b0j, h_boos)
    n_btag = h_btag.Integral(-1, -1)

    h_1b1jMcut = f_filler.Get(proto_filler_histo.format(m, 's1b1jresolvedMcut'))
    h_2b0jMcut = f_filler.Get(proto_filler_histo.format(m, 's2b0jresolvedMcut'))
    h_boosMcut = f_filler.Get(proto_filler_histo.format(m, 'sboostedLLMcut'))
    h_mcut = mergeHistos(h_1b1jMcut, h_2b0jMcut, h_boosMcut)
    n_mcut = h_mcut.Integral(-1, -1)

    if channel != 'TauTau': ## also BDT
        # protoBDT = 'lmr90' if m <= 350 else 'hmr90'
        protoBDT = 'lmr70'
        h_1b1jMcutBDT = f_filler.Get(proto_filler_histo.format(m, 's1b1jresolvedMcut%s' % protoBDT))
        h_2b0jMcutBDT = f_filler.Get(proto_filler_histo.format(m, 's2b0jresolvedMcut%s' % protoBDT))
        h_boosMcutBDT = f_filler.Get(proto_filler_histo.format(m, 'sboostedLLMcut')) ## no changes, there's no BDT for boosted!
        h_BDT = mergeHistos(h_1b1jMcutBDT, h_2b0jMcutBDT, h_boosMcutBDT)
        n_BDT = h_BDT.Integral(-1, -1)

    ## quality check: n_2jets and baseline should be the same
    if abs(n_2jets - n_baseline)/n_2jets > 0.02:
        print ' --> difference in last eff and baseline: ', n_2jets , n_baseline , ' ~> ', abs(n_2jets - n_baseline)/n_2jets

    gr_trg.SetPoint(gr_trg.GetN(), m-20., n_trg/n_all)
    gr_tautau.SetPoint(gr_tautau.GetN(), m-20., n_tautau/n_all)
    gr_3lep.SetPoint(gr_3lep.GetN(), m-20., n_3lep/n_all)
    gr_2jets.SetPoint(gr_2jets.GetN(), m-20., n_2jets/n_all)
    gr_btag.SetPoint(gr_btag.GetN(), m-20., n_btag/n_all)
    gr_mcut.SetPoint(gr_mcut.GetN(), m-20., n_mcut/n_all)
    if gr_BDT : gr_BDT.SetPoint(gr_BDT.GetN(), m-20., n_BDT/n_all)


gr_trg.SetName('gr_trg')
gr_tautau.SetName('gr_tautau')
gr_3lep.SetName('gr_3lep')
gr_2jets.SetName('gr_2jets')
gr_btag.SetName('gr_btag')
gr_mcut.SetName('gr_mcut')
if gr_BDT : gr_BDT.SetName('gr_BDT')

fOut = ROOT.TFile('effPlots_%s_nonresonantKlambda_%s.root' % (channel, stype), 'recreate')
gr_trg.Write()
gr_tautau.Write()
gr_3lep.Write()
gr_2jets.Write()
gr_btag.Write()
gr_mcut.Write()
if gr_BDT : gr_BDT.Write()

