import ROOT

year = '2018'
kl = '1'

fIn1 = ROOT.TFile('/gwteraz/users/brivio/SKIMMED_HHcomb_overlap_checks/SKIM_GGHH4b_'+year+'_NLO_cHHH'+kl+'/total.root')

tIn1 = fIn1.Get('HTauTauTree')


# bTag 2016
btagM      = "((bjet1_bID_deepFlavor > 0.3093 && bjet2_bID_deepFlavor < 0.3093) || (bjet1_bID_deepFlavor < 0.3093 && bjet2_bID_deepFlavor > 0.3093))"
btagMfirst = "((bjet1_bID_deepFlavor > 0.3093) || (bjet2_bID_deepFlavor > 0.3093))"
btagLL     = "bjet1_bID_deepFlavor > 0.0614 && bjet2_bID_deepFlavor > 0.0614"
btagMM     = "bjet1_bID_deepFlavor > 0.3093 && bjet2_bID_deepFlavor > 0.3093"


# channels
pair_et = "pairType == 1"
pair_mt = "pairType == 0"
pair_tt = "pairType == 2"

# Baselines
pairSR_et  = "pairType == 1 && isOS != 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 5"
pairSR_mt  = "pairType == 0 && isOS != 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 5"
pairSR_tt  = "pairType == 2 && isOS != 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 5"

# vetoes
vetoes = "nleps == 0 && nbjetscand > 1"

# Masscut
boostMassCut2  = "((tauH_SVFIT_mass-128.)*(tauH_SVFIT_mass-128.))/(60.*60.) + ((bH_mass_raw-159.)*(bH_mass_raw-159.))/(94.*94.)   <  1.0"
ellypsMassCut2 = "((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0"


# Categories
s1b1jresolvedMcut = btagM  + " && isBoosted != 1 && " + ellypsMassCut2 + " && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3093) || (bjet2_bID_deepFlavor > 0.3093))) "
s2b0jresolvedMcut = btagMM + " && isBoosted != 1 && " + ellypsMassCut2 + " && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3093) || (bjet2_bID_deepFlavor > 0.3093))) "
sboostedLLMcut    = btagLL + " && isBoosted == 1 && " + boostMassCut2  + " && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3093) || (bjet2_bID_deepFlavor > 0.3093))) "
VBFloose          = "isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && " + btagMfirst



print '----------------------------------'
print '     BBBB -', year, '- kl:', kl 
print '----------------------------------'
print ' Pass trigMatch      : ', tIn1.GetEntries()
print ''
print ' + Pair ETau   : ', tIn1.GetEntries(pair_et)
print ' + Pair MuTau  : ', tIn1.GetEntries(pair_mt)
print ' + Pair TauTau : ', tIn1.GetEntries(pair_tt)
print ''
print ' + SR ETau   : ', tIn1.GetEntries(pairSR_et)
print ' + SR MuTau  : ', tIn1.GetEntries(pairSR_mt)
print ' + SR TauTau : ', tIn1.GetEntries(pairSR_tt)
print ''
print ' + 3lep&2bjets ETau   : ', tIn1.GetEntries(pairSR_et + " && " + vetoes)
print ' + 3lep&2bjets MuTau  : ', tIn1.GetEntries(pairSR_mt + " && " + vetoes)
print ' + 3lep&2bjets TauTau : ', tIn1.GetEntries(pairSR_tt + " && " + vetoes)
print ''
print ' s1b1jresolvedMcut ETau   : ', tIn1.GetEntries(pairSR_et + " && " + vetoes + " && " + s1b1jresolvedMcut)
print ' s1b1jresolvedMcut MuTau  : ', tIn1.GetEntries(pairSR_mt + " && " + vetoes + " && " + s1b1jresolvedMcut)
print ' s1b1jresolvedMcut TauTau : ', tIn1.GetEntries(pairSR_tt + " && " + vetoes + " && " + s1b1jresolvedMcut)
print ''
print ' s2b0jresolvedMcut ETau   : ', tIn1.GetEntries(pairSR_et + " && " + vetoes + " && " + s2b0jresolvedMcut)
print ' s2b0jresolvedMcut MuTau  : ', tIn1.GetEntries(pairSR_mt + " && " + vetoes + " && " + s2b0jresolvedMcut)
print ' s2b0jresolvedMcut TauTau : ', tIn1.GetEntries(pairSR_tt + " && " + vetoes + " && " + s2b0jresolvedMcut)
print ''
print ' sboostedLLMcut ETau   : ', tIn1.GetEntries(pairSR_et + " && " + vetoes + " && " + sboostedLLMcut)
print ' sboostedLLMcut MuTau  : ', tIn1.GetEntries(pairSR_mt + " && " + vetoes + " && " + sboostedLLMcut)
print ' sboostedLLMcut TauTau : ', tIn1.GetEntries(pairSR_tt + " && " + vetoes + " && " + sboostedLLMcut)
print ''
print ' VBFloose ETau   : ', tIn1.GetEntries(pairSR_et + " && " + vetoes + " && " + VBFloose)
print ' VBFloose MuTau  : ', tIn1.GetEntries(pairSR_mt + " && " + vetoes + " && " + VBFloose)
print ' VBFloose TauTau : ', tIn1.GetEntries(pairSR_tt + " && " + vetoes + " && " + VBFloose)

print '----------------------------------'



