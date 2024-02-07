import sys
import os
import numpy as np
from array import array
from ROOT import *

def makeRatio (hNominal, hVariation, newName):
    nPoints = hNominal.GetNbinsX()
    fX       = []
    fY       = []
    feYUp    = []
    feYDown  = []
    feXRight = []
    feXLeft  = []

    for ibin in range (1, nPoints+1):
        den = hNominal.GetBinContent(ibin)
        num = hVariation.GetBinContent(ibin)
        if den > 0:
            # Y
            fY.append(num/den)
            feYUp.append(hVariation.GetBinErrorUp(ibin) / den)
            feYDown.append(hVariation.GetBinErrorLow(ibin) / den)

            # X
            fX.append (ibin)
            feXLeft.append(0.0)
            feXRight.append(0.0)

    afX       = array ("d", fX      )
    afY       = array ("d", fY      )
    afeYUp    = array ("d", feYUp   )
    afeYDown  = array ("d", feYDown )
    afeXRight = array ("d", feXRight)
    afeXLeft  = array ("d", feXLeft )
    gRatio = TGraphAsymmErrors (len(afX), afX, afY, afeXLeft, afeXRight, afeYDown, afeYUp)
    
    gRatio.SetMarkerStyle(8)
    gRatio.SetMarkerSize(1.)
    gRatio.SetName(newName)

    return gRatio


# Make global style changes
gStyle.SetTextFont(42)

plotdir = '/home/llr/cms/motta/HHLegacy/CMSSW_11_1_0_pre6/src/KLUBAnalysis/studies/EFTreweight/reco_allNLO_to_kl_comparison'
os.system('mkdir -p '+plotdir)

indir_base = '/data_CMS/cms/motta/HHLegacy_SKIMS/SKIMS2018/SKIMMED_Legacy2018_2022_11_24_EFTrew_closureTests'
indir_rew = '/data_CMS/cms/motta/HHLegacy_SKIMS/SKIMS2018/SKIMMED_Legacy2018_2022_11_24_EFTrew_closureTests'
benchmarks = ['kl0', 'kl1', 'kl2p45', 'kl5']

for bm in benchmarks:
    print("\n--------------------------")
    print(" kl = ", bm)

    base = TFile.Open(indir_base+'/SKIM_plain_'+bm+'/total.root')
    tree = base.Get('HTauTauTree')
    h_eff = base.Get('h_eff')

    rew2 = TFile.Open(indir_rew+'/SKIM_allNLO_to_'+bm+'_Opt1_reco1/total.root')
    tree_rew2 = rew2.Get('HTauTauTree')
    h_eff_rew2 = rew2.Get('h_eff')

    gStyle.SetOptStat(0) # Disable the statistics box

    tree.SetLineColor(4)
    tree_rew2.SetLineColor(2)
    gROOT.ForceStyle()


    # ***************************************************************************************
    # plot gen HH mass
    h = TH1F('h', ';genMHH;N_{events}', 150, 0, 1500)
    h.SetLineColor(4)
    h.SetLineWidth(2)
    h2 = TH1F('h2', ';genMHH;N_{events}', 150, 0, 1500)
    h2.SetLineColor(2)
    h2.SetLineWidth(2)
    h2.GetXaxis().SetTitle("m_{HH} (GeV)")

    c = TCanvas('c', 'c', 1200, 800)
    #c.SetLogy()

    leg = TLegend(0.6,0.6,0.9,0.9)
    leg.SetTextFont(43)
    leg.SetTextSize(18)

    # Gen-like plotting (minimal weights)
    # aMCatNLOweight = event.aMCatNLOweight * addWeight
    weights = "aMCatNLOweight * HHweight"

    # Full weights rescaling: MC_weight, PUReweight, PUjetID_SF, L1pref_weight, prescaleWeight, trigSF, IdAndIsoAndFakeSF_deep_pt, DYscale_MTT, bTagweightReshape
    # MC_weight = event.aMCatNLOweight * addWeight * XS * stitchWeight * HHweight
    #weights = "MC_weight * PUReweight * PUjetID_SF * L1pref_weight * prescaleWeight * trigSF * IdAndIsoAndFakeSF_deep_pt * DYscale_MTT * bTagweightReshape"

    # Draw histos
    tree_rew2.Draw("genMHH>>h2", weights, "hist")
    tree     .Draw("genMHH>>h ", weights, "hist same")

    # TLegend
    leg.AddEntry(tree_rew2, "NLO samples reweighted", "LPE")
    leg.AddEntry(tree, "NLO "+bm, "LPE")
    leg.Draw("same")

    # Luminosity for the re-scaling
    lumi = 137200 # 137.2/fb
    lumi = 1 # 59.7/fb
    lumi = 36330 # pb^-1 full 2016

    # Normalized to 1
    # h.Scale(1./h.Integral())
    # h2.Scale(1./h2.Integral())

    # Full weights rescaling
    h.Scale(lumi/h_eff.GetBinContent(1))
    h2.Scale(lumi/h_eff_rew2.GetBinContent(1))

    # Compute integrals and print them
    tree_int = h.Integral()
    tree_int_rew2 = h2.Integral()
    print(" NLO integral = {0}".format(tree_int))
    print(" NLO2NLO Rew integral = {0}".format(tree_int_rew2))
    print("  --> ratio = {0}".format(tree_int_rew2/tree_int))

    # Save plots
    # c.SaveAs(plotdir+'/'+bm+'_comparison_genHHmass_Opt1_reco1.pdf')
    # c.SaveAs(plotdir+'/'+bm+'_comparison_genHHmass_Opt1_reco1_unScaled.pdf')
    c.SaveAs(plotdir+'/'+bm+'_comparison_genHHmass_Opt1_reco1_fullWeights.pdf')

    del c, h, h2 # delete to allow new plot without warnings and errors

# import pdb; pdb.set_trace()

