import ROOT
import numpy as np

# Enable MultiThreading
ROOT.ROOT.EnableImplicitMT(6)

# Run 2 Lumis
lumi_2016 = '36330.0'
lumi_2017 = '41529.0'
lumi_2018 = '59741.0'

# Selections definition
ETau_2016_res1b = 'pairType == 1 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 5 && ((bjet1_bID_deepFlavor > 0.3093 && bjet2_bID_deepFlavor < 0.3093) || (bjet1_bID_deepFlavor < 0.3093 && bjet2_bID_deepFlavor > 0.3093)) && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3093) || (bjet2_bID_deepFlavor > 0.3093)))'
ETau_2016_res2b = 'pairType == 1 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.3093 && bjet2_bID_deepFlavor > 0.3093) && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3093) || (bjet2_bID_deepFlavor > 0.3093)))'
ETau_2016_boost = 'pairType == 1 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.0614 && bjet2_bID_deepFlavor > 0.0614) && isBoosted == 1 && (((tauH_SVFIT_mass-128.)*(tauH_SVFIT_mass-128.))/(60.*60.) + ((bH_mass_raw-159.)*(bH_mass_raw-159.))/(94.*94.)   <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3093) || (bjet2_bID_deepFlavor > 0.3093)))'
ETau_2016_VBF   = 'pairType == 1 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 5 && isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3093) || (bjet2_bID_deepFlavor > 0.3093))'

ETau_2017_res1b = 'pairType == 1 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 5 && ((bjet1_bID_deepFlavor > 0.3033 && bjet2_bID_deepFlavor < 0.3033) || (bjet1_bID_deepFlavor < 0.3033 && bjet2_bID_deepFlavor > 0.3033))  && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.3033 || bjet2_bID_deepFlavor > 0.3033) )'
ETau_2017_res2b = 'pairType == 1 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.3033 && bjet2_bID_deepFlavor > 0.3033) && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.3033 || bjet2_bID_deepFlavor > 0.3033) )'
ETau_2017_boost = 'pairType == 1 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.0521 && bjet2_bID_deepFlavor > 0.0521) && isBoosted == 1 && (((tauH_SVFIT_mass-128.)*(tauH_SVFIT_mass-128.))/(60.*60.) + ((bH_mass_raw-159.)*(bH_mass_raw-159.))/(94.*94.)   <  1.0)  && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.3033 || bjet2_bID_deepFlavor > 0.3033) )'
ETau_2017_VBF   = 'pairType == 1 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 5 && isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3033) || (bjet2_bID_deepFlavor > 0.3033))'

ETau_2018_res1b = 'pairType == 1 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 5 && ((bjet1_bID_deepFlavor > 0.2770 && bjet2_bID_deepFlavor < 0.2770) || (bjet1_bID_deepFlavor < 0.2770 && bjet2_bID_deepFlavor > 0.2770))  && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.2770 || bjet2_bID_deepFlavor > 0.2770) )'
ETau_2018_res2b = 'pairType == 1 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.2770 && bjet2_bID_deepFlavor > 0.2770) && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.2770 || bjet2_bID_deepFlavor > 0.2770) )'
ETau_2018_boost = 'pairType == 1 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.0494 && bjet2_bID_deepFlavor > 0.0494) && isBoosted == 1 && (((tauH_SVFIT_mass-128.)*(tauH_SVFIT_mass-128.))/(60.*60.) + ((bH_mass_raw-159.)*(bH_mass_raw-159.))/(94.*94.)   <  1.0)  && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.2770 || bjet2_bID_deepFlavor > 0.2770) )'
ETau_2018_VBF   = 'pairType == 1 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 5 && isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.2770) || (bjet2_bID_deepFlavor > 0.2770))'


MuTau_2016_res1b = 'pairType == 0 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 5 && ((bjet1_bID_deepFlavor > 0.3093 && bjet2_bID_deepFlavor < 0.3093) || (bjet1_bID_deepFlavor < 0.3093 && bjet2_bID_deepFlavor > 0.3093)) && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3093) || (bjet2_bID_deepFlavor > 0.3093)))'
MuTau_2016_res2b = 'pairType == 0 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.3093 && bjet2_bID_deepFlavor > 0.3093) && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3093) || (bjet2_bID_deepFlavor > 0.3093)))'
MuTau_2016_boost = 'pairType == 0 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.0614 && bjet2_bID_deepFlavor > 0.0614) && isBoosted == 1 && (((tauH_SVFIT_mass-128.)*(tauH_SVFIT_mass-128.))/(60.*60.) + ((bH_mass_raw-159.)*(bH_mass_raw-159.))/(94.*94.)   <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3093) || (bjet2_bID_deepFlavor > 0.3093)))'
MuTau_2016_VBF   = 'pairType == 0 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 5 && isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3093) || (bjet2_bID_deepFlavor > 0.3093))'

MuTau_2017_res1b = 'pairType == 0 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 5 && ((bjet1_bID_deepFlavor > 0.3033 && bjet2_bID_deepFlavor < 0.3033) || (bjet1_bID_deepFlavor < 0.3033 && bjet2_bID_deepFlavor > 0.3033))  && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.3033 || bjet2_bID_deepFlavor > 0.3033) )'
MuTau_2017_res2b = 'pairType == 0 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.3033 && bjet2_bID_deepFlavor > 0.3033) && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.3033 || bjet2_bID_deepFlavor > 0.3033) )'
MuTau_2017_boost = 'pairType == 0 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.0521 && bjet2_bID_deepFlavor > 0.0521) && isBoosted == 1 && (((tauH_SVFIT_mass-128.)*(tauH_SVFIT_mass-128.))/(60.*60.) + ((bH_mass_raw-159.)*(bH_mass_raw-159.))/(94.*94.)   <  1.0)  && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.3033 || bjet2_bID_deepFlavor > 0.3033) )'
MuTau_2017_VBF   = 'pairType == 0 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 5 && isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3033) || (bjet2_bID_deepFlavor > 0.3033))'

MuTau_2018_res1b = 'pairType == 0 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 5 && ((bjet1_bID_deepFlavor > 0.2770 && bjet2_bID_deepFlavor < 0.2770) || (bjet1_bID_deepFlavor < 0.2770 && bjet2_bID_deepFlavor > 0.2770))  && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.2770 || bjet2_bID_deepFlavor > 0.2770) )'
MuTau_2018_res2b = 'pairType == 0 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.2770 && bjet2_bID_deepFlavor > 0.2770) && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.2770 || bjet2_bID_deepFlavor > 0.2770) )'
MuTau_2018_boost = 'pairType == 0 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.0494 && bjet2_bID_deepFlavor > 0.0494) && isBoosted == 1 && (((tauH_SVFIT_mass-128.)*(tauH_SVFIT_mass-128.))/(60.*60.) + ((bH_mass_raw-159.)*(bH_mass_raw-159.))/(94.*94.)   <  1.0)  && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.2770 || bjet2_bID_deepFlavor > 0.2770) )'
MuTau_2018_VBF   = 'pairType == 0 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 5 && isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.2770) || (bjet2_bID_deepFlavor > 0.2770))'


TauTau_2016_res1b = 'pairType == 2 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 5 && ((bjet1_bID_deepFlavor > 0.3093 && bjet2_bID_deepFlavor < 0.3093) || (bjet1_bID_deepFlavor < 0.3093 && bjet2_bID_deepFlavor > 0.3093)) && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3093) || (bjet2_bID_deepFlavor > 0.3093)))'
TauTau_2016_res2b = 'pairType == 2 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.3093 && bjet2_bID_deepFlavor > 0.3093) && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3093) || (bjet2_bID_deepFlavor > 0.3093)))'
TauTau_2016_boost = 'pairType == 2 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.0614 && bjet2_bID_deepFlavor > 0.0614) && isBoosted == 1 && (((tauH_SVFIT_mass-128.)*(tauH_SVFIT_mass-128.))/(60.*60.) + ((bH_mass_raw-159.)*(bH_mass_raw-159.))/(94.*94.)   <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3093) || (bjet2_bID_deepFlavor > 0.3093)))'
TauTau_2016_VBF   = 'pairType == 2 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 5 && isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3093) || (bjet2_bID_deepFlavor > 0.3093))'

TauTau_2017_res1b = 'pairType == 2 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 5 && ((bjet1_bID_deepFlavor > 0.3033 && bjet2_bID_deepFlavor < 0.3033) || (bjet1_bID_deepFlavor < 0.3033 && bjet2_bID_deepFlavor > 0.3033))  && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.3033 || bjet2_bID_deepFlavor > 0.3033) )'
TauTau_2017_res2b = 'pairType == 2 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.3033 && bjet2_bID_deepFlavor > 0.3033) && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.3033 || bjet2_bID_deepFlavor > 0.3033) )'
TauTau_2017_boost = 'pairType == 2 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.0521 && bjet2_bID_deepFlavor > 0.0521) && isBoosted == 1 && (((tauH_SVFIT_mass-128.)*(tauH_SVFIT_mass-128.))/(60.*60.) + ((bH_mass_raw-159.)*(bH_mass_raw-159.))/(94.*94.)   <  1.0)  && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.3033 || bjet2_bID_deepFlavor > 0.3033) )'
TauTau_2017_VBF   = 'pairType == 2 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 5 && isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.3033) || (bjet2_bID_deepFlavor > 0.3033))'

TauTau_2018_res1b = 'pairType == 2 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 5 && ((bjet1_bID_deepFlavor > 0.2770 && bjet2_bID_deepFlavor < 0.2770) || (bjet1_bID_deepFlavor < 0.2770 && bjet2_bID_deepFlavor > 0.2770))  && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.2770 || bjet2_bID_deepFlavor > 0.2770) )'
TauTau_2018_res2b = 'pairType == 2 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.2770 && bjet2_bID_deepFlavor > 0.2770) && isBoosted != 1 && (((tauH_SVFIT_mass-129.)*(tauH_SVFIT_mass-129.))/(53.*53.) + ((bH_mass_raw-169.)*(bH_mass_raw-169.))/(145.*145.) <  1.0) && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.2770 || bjet2_bID_deepFlavor > 0.2770) )'
TauTau_2018_boost = 'pairType == 2 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 5 && (bjet1_bID_deepFlavor > 0.0494 && bjet2_bID_deepFlavor > 0.0494) && isBoosted == 1 && (((tauH_SVFIT_mass-128.)*(tauH_SVFIT_mass-128.))/(60.*60.) + ((bH_mass_raw-159.)*(bH_mass_raw-159.))/(94.*94.)   <  1.0)  && !(isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && (bjet1_bID_deepFlavor > 0.2770 || bjet2_bID_deepFlavor > 0.2770) )'
TauTau_2018_VBF   = 'pairType == 2 && nleps == 0 && nbjetscand > 1 && isOS != 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 5 && isVBF == 1 && VBFjj_mass > 500 && VBFjj_deltaEta > 3 && ((bjet1_bID_deepFlavor > 0.2770) || (bjet2_bID_deepFlavor > 0.2770))'

# Samples
samples_et_2016 = [
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021_ETau/SKIM_ZH_HBB_ZQQ',
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021_ETau/SKIM_ZH_HBB_ZLL',
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021_ETau/SKIM_ZH_HTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021_ETau/SKIM_ggHTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021_ETau/SKIM_VBFHTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021_ETau/SKIM_WplusHTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021_ETau/SKIM_WminusHTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021_ETau/SKIM_ttHJetTononBB',
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021_ETau/SKIM_ttHJetToBB',
]

samples_et_2017 = [
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_23Feb2021/SKIM_ZH_HBB_ZLL',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_23Feb2021/SKIM_ZH_HBB_ZQQ',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_23Feb2021/SKIM_ZH_HTauTau',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_23Feb2021/SKIM_VBFHTauTau',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_23Feb2021/SKIM_ggHTauTau',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_23Feb2021/SKIM_WplusHTauTau',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_23Feb2021/SKIM_WminusHTauTau',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_23Feb2021/SKIM_ttHJetToBB',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_23Feb2021/SKIM_ttHJetTononBB',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_23Feb2021/SKIM_ttHToTauTau',
]

samples_et_2018 = [
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021_ETau/SKIM_ZH_HBB_ZLL',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021_ETau/SKIM_ZH_HTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021_ETau/SKIM_ZH_HBB_ZQQ',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021_ETau/SKIM_ggHTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021_ETau/SKIM_VBFHTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021_ETau/SKIM_WplusHTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021_ETau/SKIM_WminusHTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021_ETau/SKIM_ttHJetTononBB',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021_ETau/SKIM_ttHJetToBB',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021_ETau/SKIM_ttHJetToTauTau',
]

samples_mt_tt_2016 = [
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021/SKIM_ZH_HBB_ZQQ',
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021/SKIM_ZH_HBB_ZLL',
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021/SKIM_ZH_HTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021/SKIM_ggHTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021/SKIM_VBFHTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021/SKIM_WplusHTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021/SKIM_WminusHTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021/SKIM_ttHJetTononBB',
    '/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021/SKIM_ttHJetToBB',
]

samples_mt_tt_2017 = [
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_19Feb2021/SKIM_ZH_HBB_ZLL',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_19Feb2021/SKIM_ZH_HBB_ZQQ',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_19Feb2021/SKIM_ZH_HTauTau',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_19Feb2021/SKIM_VBFHTauTau',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_19Feb2021/SKIM_ggHTauTau',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_19Feb2021/SKIM_WplusHTauTau',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_19Feb2021/SKIM_WminusHTauTau',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_19Feb2021/SKIM_ttHJetToBB',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_19Feb2021/SKIM_ttHJetTononBB',
    '/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_19Feb2021/SKIM_ttHToTauTau',
]

samples_mt_tt_2018 = [
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021/SKIM_ZH_HBB_ZLL',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021/SKIM_ZH_HTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021/SKIM_ZH_HBB_ZQQ',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021/SKIM_ggHTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021/SKIM_VBFHTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021/SKIM_WplusHTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021/SKIM_WminusHTauTau',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021/SKIM_ttHJetTononBB',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021/SKIM_ttHJetToBB',
    '/gwteraz/users/brivio/SKIMS_Legacy2018_16Feb2021/SKIM_ttHJetToTauTau',
]


# --------------------------------------------------------------------------------------------------------
# Pretty print to txt file
def writeToTxt(df, fileName):

    # Get runs/lumis/events in arrays
    runs       = df['RunNumber']
    lumis      = df['lumi']
    events     = df['EventNumber']
    weights    = df['weight']
    categories = df['category']
    npv        = df['npv']
    rho        = df['rho']

    with open(fileName, 'w') as f:
        f.write('{: >3} {: >9} {: >13} {: >12} {: >10} {: >5} {: >10}\n'.format('Run', 'Lumi', 'EventNumber', 'Weight', 'category', 'npv', 'rho'))
        for i in range(len(runs)):
            string_to_print = '{: >3} {: >9} {: >13} {: >12.2E} {: >10} {: >5} {: >10.3f}\n' \
                              .format(runs[i],lumis[i],events[i],weights[i],categories[i],npv[i],rho[i])
            f.write(string_to_print)


# --------------------------------------------------------------------------------------------------------
# Add categories together and print
def addAndPrint(cat1,cat2,cat3,cat4,names):

    # Loop on the samples
    for i in range(0,len(cat1)):

        print "Adding dicts for sample", names[i]

        # For each sample - create a tmp_dict to add different categories together
        tmp_dict = {}

        # loop on keys and merge the dictionaries
        for key in cat1[i].keys():
            tmp_dict[key] = np.concatenate( (cat1[i][key],cat2[i][key],cat3[i][key],cat4[i][key]), axis=None )

        # print the new dict
        writeToTxt(tmp_dict, names[i])



# --------------------------------------------------------------------------------------------------------
# Get the filtered RDF as dictionary of Numpy arrays
def getFilteredRDFs (samples, lumiN, selection, catName, chanName):

    # Output arrays
    RDFs = []
    outputNames = []

    # Loop on samples and make the outputs
    for sample in samples:

        print 'Analysing --> ', sample.split('/')[-1], ' cat: ', catName

        # Get the normalization number
        infile = ROOT.TFile(sample+'/total.root')
        h_eff = infile.Get('h_eff')
        norm = h_eff.GetBinContent(1)

        # Define the RDF
        df = ROOT.RDataFrame('HTauTauTree', sample+'/output_*.root')

        filtered_df = df.Filter(selection , 'selection')
        #print "   Events:", filtered_df.Count().GetValue()

        # Add total weight branch to the RDF
        total_weight = '( '+lumiN+' / '+str(norm)+' ) * MC_weight * PUReweight * DYscale_MTT * trigSF * IdAndIsoAndFakeSF_deep_pt * L1pref_weight * PUjetID_SF * prescaleWeight'
        filtered_df = filtered_df.Define("weight", total_weight)

        # Transform the filtered RDF into a dictionary
        branchList = ['EventNumber','lumi','RunNumber', 'weight', 'npv', 'rho']
        ret = filtered_df.AsNumpy(branchList)

        # Add key to dict with category name
        ret['category'] = np.repeat(catName, len(ret['lumi']))

        # Define output txt filename
        outputName = 'singleH_checks/'+chanName+'_'+sample.split('/')[-1]+'.txt'

        # Append the results to the output arrays
        RDFs.append(ret)
        outputNames.append(outputName)

    return RDFs, outputNames

# --------------------------
# Main
#------------
# ETau 2016
evts_ETau_2016_s1 , output_txt_names = getFilteredRDFs(samples_et_2016, lumi_2016, ETau_2016_res1b, 'res1b', 'ETau_2016')
evts_ETau_2016_s2 , output_txt_names = getFilteredRDFs(samples_et_2016, lumi_2016, ETau_2016_res2b, 'res2b', 'ETau_2016')
evts_ETau_2016_sb , output_txt_names = getFilteredRDFs(samples_et_2016, lumi_2016, ETau_2016_boost, 'boost', 'ETau_2016')
evts_ETau_2016_vbf, output_txt_names = getFilteredRDFs(samples_et_2016, lumi_2016, ETau_2016_VBF  , 'vbf'  , 'ETau_2016')
addAndPrint(evts_ETau_2016_s1, evts_ETau_2016_s2, evts_ETau_2016_sb, evts_ETau_2016_vbf, output_txt_names)

# ETau 2017
evts_ETau_2017_s1 , output_txt_names = getFilteredRDFs(samples_et_2017, lumi_2017, ETau_2017_res1b, 'res1b', 'ETau_2017')
evts_ETau_2017_s2 , output_txt_names = getFilteredRDFs(samples_et_2017, lumi_2017, ETau_2017_res2b, 'res2b', 'ETau_2017')
evts_ETau_2017_sb , output_txt_names = getFilteredRDFs(samples_et_2017, lumi_2017, ETau_2017_boost, 'boost', 'ETau_2017')
evts_ETau_2017_vbf, output_txt_names = getFilteredRDFs(samples_et_2017, lumi_2017, ETau_2017_VBF  , 'vbf'  , 'ETau_2017')
addAndPrint(evts_ETau_2017_s1, evts_ETau_2017_s2, evts_ETau_2017_sb, evts_ETau_2017_vbf, output_txt_names)

# ETau 2018
evts_ETau_2018_s1 , output_txt_names = getFilteredRDFs(samples_et_2018, lumi_2018, ETau_2018_res1b, 'res1b', 'ETau_2019')
evts_ETau_2018_s2 , output_txt_names = getFilteredRDFs(samples_et_2018, lumi_2018, ETau_2018_res2b, 'res2b', 'ETau_2019')
evts_ETau_2018_sb , output_txt_names = getFilteredRDFs(samples_et_2018, lumi_2018, ETau_2018_boost, 'boost', 'ETau_2019')
evts_ETau_2018_vbf, output_txt_names = getFilteredRDFs(samples_et_2018, lumi_2018, ETau_2018_VBF  , 'vbf'  , 'ETau_2019')
addAndPrint(evts_ETau_2018_s1, evts_ETau_2018_s2, evts_ETau_2018_sb, evts_ETau_2018_vbf, output_txt_names)


#------------
# MuTau 2016
evts_MuTau_2016_s1 , output_txt_names = getFilteredRDFs(samples_mt_tt_2016, lumi_2016, MuTau_2016_res1b, 'res1b', 'MuTau_2016')
evts_MuTau_2016_s2 , output_txt_names = getFilteredRDFs(samples_mt_tt_2016, lumi_2016, MuTau_2016_res2b, 'res2b', 'MuTau_2016')
evts_MuTau_2016_sb , output_txt_names = getFilteredRDFs(samples_mt_tt_2016, lumi_2016, MuTau_2016_boost, 'boost', 'MuTau_2016')
evts_MuTau_2016_vbf, output_txt_names = getFilteredRDFs(samples_mt_tt_2016, lumi_2016, MuTau_2016_VBF  , 'vbf'  , 'MuTau_2016')
addAndPrint(evts_MuTau_2016_s1, evts_MuTau_2016_s2, evts_MuTau_2016_sb, evts_MuTau_2016_vbf, output_txt_names)

# MuTau 2017
evts_MuTau_2017_s1 , output_txt_names = getFilteredRDFs(samples_mt_tt_2017, lumi_2017, MuTau_2017_res1b, 'res1b', 'MuTau_2017')
evts_MuTau_2017_s2 , output_txt_names = getFilteredRDFs(samples_mt_tt_2017, lumi_2017, MuTau_2017_res2b, 'res2b', 'MuTau_2017')
evts_MuTau_2017_sb , output_txt_names = getFilteredRDFs(samples_mt_tt_2017, lumi_2017, MuTau_2017_boost, 'boost', 'MuTau_2017')
evts_MuTau_2017_vbf, output_txt_names = getFilteredRDFs(samples_mt_tt_2017, lumi_2017, MuTau_2017_VBF  , 'vbf'  , 'MuTau_2017')
addAndPrint(evts_MuTau_2017_s1, evts_MuTau_2017_s2, evts_MuTau_2017_sb, evts_MuTau_2017_vbf, output_txt_names)

# MuTau 2018
evts_MuTau_2018_s1 , output_txt_names = getFilteredRDFs(samples_mt_tt_2018, lumi_2018, MuTau_2018_res1b, 'res1b', 'MuTau_2018')
evts_MuTau_2018_s2 , output_txt_names = getFilteredRDFs(samples_mt_tt_2018, lumi_2018, MuTau_2018_res2b, 'res2b', 'MuTau_2018')
evts_MuTau_2018_sb , output_txt_names = getFilteredRDFs(samples_mt_tt_2018, lumi_2018, MuTau_2018_boost, 'boost', 'MuTau_2018')
evts_MuTau_2018_vbf, output_txt_names = getFilteredRDFs(samples_mt_tt_2018, lumi_2018, MuTau_2018_VBF  , 'vbf'  , 'MuTau_2018')
addAndPrint(evts_MuTau_2018_s1, evts_MuTau_2018_s2, evts_MuTau_2018_sb, evts_MuTau_2018_vbf, output_txt_names)


#------------
# TauTau 2016
evts_TauTau_2016_s1 , output_txt_names = getFilteredRDFs(samples_mt_tt_2016, lumi_2016, TauTau_2016_res1b, 'res1b', 'TauTau_2016')
evts_TauTau_2016_s2 , output_txt_names = getFilteredRDFs(samples_mt_tt_2016, lumi_2016, TauTau_2016_res2b, 'res2b', 'TauTau_2016')
evts_TauTau_2016_sb , output_txt_names = getFilteredRDFs(samples_mt_tt_2016, lumi_2016, TauTau_2016_boost, 'boost', 'TauTau_2016')
evts_TauTau_2016_vbf, output_txt_names = getFilteredRDFs(samples_mt_tt_2016, lumi_2016, TauTau_2016_VBF  , 'vbf'  , 'TauTau_2016')
addAndPrint(evts_TauTau_2016_s1, evts_TauTau_2016_s2, evts_TauTau_2016_sb, evts_TauTau_2016_vbf, output_txt_names)

# TauTau 2017
evts_TauTau_2017_s1 , output_txt_names = getFilteredRDFs(samples_mt_tt_2017, lumi_2017, TauTau_2017_res1b, 'res1b', 'TauTau_2017')
evts_TauTau_2017_s2 , output_txt_names = getFilteredRDFs(samples_mt_tt_2017, lumi_2017, TauTau_2017_res2b, 'res2b', 'TauTau_2017')
evts_TauTau_2017_sb , output_txt_names = getFilteredRDFs(samples_mt_tt_2017, lumi_2017, TauTau_2017_boost, 'boost', 'TauTau_2017')
evts_TauTau_2017_vbf, output_txt_names = getFilteredRDFs(samples_mt_tt_2017, lumi_2017, TauTau_2017_VBF  , 'vbf'  , 'TauTau_2017')
addAndPrint(evts_TauTau_2017_s1, evts_TauTau_2017_s2, evts_TauTau_2017_sb, evts_TauTau_2017_vbf, output_txt_names)

# TauTau 2018
evts_TauTau_2018_s1 , output_txt_names = getFilteredRDFs(samples_mt_tt_2018, lumi_2018, TauTau_2018_res1b, 'res1b', 'TauTau_2018')
evts_TauTau_2018_s2 , output_txt_names = getFilteredRDFs(samples_mt_tt_2018, lumi_2018, TauTau_2018_res2b, 'res2b', 'TauTau_2018')
evts_TauTau_2018_sb , output_txt_names = getFilteredRDFs(samples_mt_tt_2018, lumi_2018, TauTau_2018_boost, 'boost', 'TauTau_2018')
evts_TauTau_2018_vbf, output_txt_names = getFilteredRDFs(samples_mt_tt_2018, lumi_2018, TauTau_2018_VBF  , 'vbf'  , 'TauTau_2018')
addAndPrint(evts_TauTau_2018_s1, evts_TauTau_2018_s2, evts_TauTau_2018_sb, evts_TauTau_2018_vbf, output_txt_names)
