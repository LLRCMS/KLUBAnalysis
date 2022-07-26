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

plotdir = '/home/llr/cms/motta/HHLegacy/CMSSW_11_1_0_pre6/src/KLUBAnalysis/MY_EFTreweightingClosureTests/2021_10_22_SKIMS/2018_bincontent5_MCweight/linY'
os.system('mkdir -p '+plotdir)

indir_base = '/data_CMS/cms/motta/HHLegacy_SKIMS/SKIMS2018/SKIMMED_Legacy2018_2021_10_22_EFTrew_closureTests'
indir_rew = '/data_CMS/cms/motta/HHLegacy_SKIMS/SKIMS2018/SKIMMED_Legacy2018_2021_10_22_EFTrew_closureTests'
benchmarks = ['cHHH0', 'cHHH1', 'cHHH5']
benchmarks = ['cHHH1']

for bm in benchmarks:
    print("\n--------------------------")
    print(bm)

    base = TFile.Open(indir_base+'/SKIM_GGHH_'+bm+'_NLO/merged.root')
    tree = base.Get('HTauTauTree')
    h_eff = base.Get('h_eff')

    rew1 = TFile.Open(indir_rew+'/SKIM_GGHH_'+bm+'_LO2NLOrew/merged.root')
    tree_rew1 = rew1.Get('HTauTauTree')
    h_eff_rew1 = rew1.Get('h_eff')

    rew2 = TFile.Open(indir_rew+'/SKIM_GGHH_'+bm+'_NLO2NLOrew/merged.root')
    tree_rew2 = rew2.Get('HTauTauTree')
    h_eff_rew2 = rew2.Get('h_eff')

    gStyle.SetOptStat(0) # Disable the statistics box

    tree.SetLineColor(9)
    tree_rew1.SetLineColor(2)
    tree_rew2.SetLineColor(3)
    gROOT.ForceStyle()


    # ***************************************************************************************
    # plot gen HH mass
    #bins = [250.0, 270.0, 290.0, 310.0, 330.0, 350.0, 370.0, 390.0, 410.0, 430.0, 450.0, 470.0, 490.0, 510.0, 530.0, 550.0, 570.0, 590.0, 610.0, 630.0, 650.0, 670.0, 700.0, 750.0, 800.0, 850.0, 900.0, 950.0, 1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1500.0, 1750.0, 2000.0, 5000.0]
    #bins_array = array('d', bins)
    #h = TH1F('h', ';genMHH;N_{events}', 36, bins_array); h.SetLineColor(9)
    #h1 = TH1F('h1', ';genMHH;N_{events}', 36, bins_array); h1.SetLineColor(2)
    #h2 = TH1F('h2', ';genMHH;N_{events}', 36, bins_array); h2.SetLineColor(3)

    h = TH1F('h', ';genMHH;N_{events}', 200, 200, 1400); h.SetLineColor(9)
    h1 = TH1F('h1', ';genMHH;N_{events}', 200, 200, 1400); h1.SetLineColor(2)
    h2 = TH1F('h2', ';genMHH;N_{events}', 200, 200, 1400); h2.SetLineColor(3)

    c = TCanvas('c', 'c', 1200, 800)
    #c.SetLogy()

    leg = TLegend(0.75,0.75,0.85,0.85)
    leg.SetBorderSize(0)
    leg.SetTextFont(43)
    leg.SetTextSize(15)
    
    tree_rew2.Draw("genMHH>>+h2", "aMCatNLOweight*HHweight", "hist"); leg.AddEntry(tree_rew2, bm+"_nlo2nlo", "LPE")
    tree_rew1.Draw("genMHH>>+h1", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree_rew1, bm+"_lo2nlo", "LPE")
    tree.Draw("genMHH>>+h", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree, bm, "LPE")
    leg.Draw("same")

    lumi = 137200 # 137.2/fb
    lumi = 1 # 59.7/fb

    h.Scale(lumi/h_eff.GetBinContent(5))
    h1.Scale(lumi/h_eff_rew1.GetBinContent(5))
    h2.Scale(lumi/h_eff_rew2.GetBinContent(5))

    tree_int = h.Integral()
    tree_int_rew1 = h1.Integral()
    tree_int_rew2 = h2.Integral()

    print("NLO integral = {0}".format(tree_int))
    print("LO2NLO Rew integral = {0}".format(tree_int_rew1))
    print("    --> ratio = {0}".format(tree_int_rew1/tree_int))
    print("NLO2NLO Rew integral = {0}".format(tree_int_rew2))
    print("    --> ratio = {0}".format(tree_int_rew2/tree_int))
  
    #c.SaveAs(plotdir+'/'+bm+'_genMHH.png')
    c.SaveAs(plotdir+'/'+bm+'_genHHmass.pdf')

    del c, h, h1, h2 # delete to allow new plot without warnings and errors

    # ***************************************************************************************
    # plot gen HH pT
    h = TH1F('h', ';genHH_pt;N_{events}', 100, 0, 500); h.SetLineColor(9)
    h1 = TH1F('h1', ';genHH_pt;N_{events}', 100, 0, 500); h1.SetLineColor(2)
    h2 = TH1F('h2', ';genHH_pt;N_{events}', 100, 0, 500); h2.SetLineColor(3)

    c = TCanvas('c', 'c', 1200, 800)
    #c.SetLogy()

    leg = TLegend(0.75,0.75,0.85,0.85)
    leg.SetBorderSize(0)
    leg.SetTextFont(43)
    leg.SetTextSize(15)
    
    tree_rew1.Draw("genHHpt>>+h1", "aMCatNLOweight*HHweight", "hist"); leg.AddEntry(tree_rew1, bm+"_lo2nlo", "LPE")
    tree_rew2.Draw("genHHpt>>+h2", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree_rew2, bm+"_nlo2nlo", "LPE")
    tree.Draw("genHHpt>>+h", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree, bm, "LPE")
    leg.Draw("same")

    lumi = 137200 # 137.2/fb
    lumi = 1 # 59.7/fb

    h.Scale(lumi/h_eff.GetBinContent(5))
    h1.Scale(lumi/h_eff_rew1.GetBinContent(5))
    h2.Scale(lumi/h_eff_rew2.GetBinContent(5))

    #c.SaveAs(plotdir+'/'+bm+'_HHpt.png')
    c.SaveAs(plotdir+'/'+bm+'_genHHpt.pdf')

    del c, h, h1, h2 # delete to allow new plot without warnings and errors

    # ***************************************************************************************
    # plot reco HH mass
    #bins = [250.0, 270.0, 290.0, 310.0, 330.0, 350.0, 370.0, 390.0, 410.0, 430.0, 450.0, 470.0, 490.0, 510.0, 530.0, 550.0, 570.0, 590.0, 610.0, 630.0, 650.0, 670.0, 700.0, 750.0, 800.0, 850.0, 900.0, 950.0, 1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1500.0, 1750.0, 2000.0, 5000.0]
    #bins_array = array('d', bins)
    #h = TH1F('h', ';recoMHH;N_{events}', 36, bins_array); h.SetLineColor(9)
    #h1 = TH1F('h1', ';recoMHH;N_{events}', 36, bins_array); h1.SetLineColor(2)
    #h2 = TH1F('h2', ';recoMHH;N_{events}', 36, bins_array); h2.SetLineColor(3)

    # h = TH1F('h', ';HH_mass;N_{events}', 200, 200, 1400); h.SetLineColor(9)
    # h1 = TH1F('h1', ';HH_mass;N_{events}', 200, 200, 1400); h1.SetLineColor(2)
    # h2 = TH1F('h2', ';HH_mass;N_{events}', 200, 200, 1400); h2.SetLineColor(3)

    # c = TCanvas('c', 'c', 1200, 800)
    # #c.SetLogy()

    # leg = TLegend(0.75,0.75,0.85,0.85)
    # leg.SetBorderSize(0)
    # leg.SetTextFont(43)
    # leg.SetTextSize(15)
    
    # tree_rew2.Draw("HH_mass>>+h2", "aMCatNLOweight*HHweight", "hist"); leg.AddEntry(tree_rew2, bm+"_nlo2nlo", "LPE")
    # tree_rew1.Draw("HH_mass>>+h1", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree_rew1, bm+"_lo2nlo", "LPE")
    # tree.Draw("HH_mass>>+h", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree, bm, "LPE")
    # leg.Draw("same")

    # lumi = 137200 # 137.2/fb
    # lumi = 1 # 59.7/fb

    # h.Scale(lumi/h_eff.GetBinContent(5))
    # h1.Scale(lumi/h_eff_rew1.GetBinContent(5))
    # h2.Scale(lumi/h_eff_rew2.GetBinContent(5))
    # #h2.SetMaximum(2.0e-8)
  
    # #c.SaveAs(plotdir+'/'+bm+'_genMHH.png')
    # c.SaveAs(plotdir+'/'+bm+'_recoHHmass.pdf')

    # del c, h, h1, h2 # delete to allow new plot without warnings and errors


    # ***************************************************************************************
    # plot reco HH pT
    # h = TH1F('h', ';HH_pt;N_{events}', 100, 0, 500); h.SetLineColor(9)
    # h1 = TH1F('h1', ';HH_pt;N_{events}', 100, 0, 500); h1.SetLineColor(2)
    # h2 = TH1F('h2', ';HH_pt;N_{events}', 100, 0, 500); h2.SetLineColor(3)

    # c = TCanvas('c', 'c', 1200, 800)
    # #c.SetLogy()

    # leg = TLegend(0.75,0.75,0.85,0.85)
    # leg.SetBorderSize(0)
    # leg.SetTextFont(43)
    # leg.SetTextSize(15)
    
    # tree_rew2.Draw("genHHpt>>+h2", "aMCatNLOweight*HHweight", "hist"); leg.AddEntry(tree_rew2, bm+"_nlo2nlo", "LPE")
    # tree_rew1.Draw("genHHpt>>+h1", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree_rew1, bm+"_lo2nlo", "LPE")
    # tree.Draw("genHHpt>>+h", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree, bm, "LPE")
    # leg.Draw("same")

    # lumi = 137200 # 137.2/fb
    # lumi = 1 # 59.7/fb

    # h.Scale(lumi/h_eff.GetBinContent(5))
    # h1.Scale(lumi/h_eff_rew1.GetBinContent(5))
    # h2.Scale(lumi/h_eff_rew2.GetBinContent(5))
    # #h2.SetMaximum(1.0e-8)

    # #c.SaveAs(plotdir+'/'+bm+'_HHpt.png')
    # c.SaveAs(plotdir+'/'+bm+'_genHHpt.pdf')

    # del c, h, h1, h2 # delete to allow new plot without warnings and errors


    # ***************************************************************************************
    # plot gen leading H pT
    h = TH1F('h', ';leading genH pT;N_{events}', 100, 0, 500); h.SetLineColor(9)
    h1 = TH1F('h1', ';leading genH pT;N_{events}', 100, 0, 500); h1.SetLineColor(2)
    h2 = TH1F('h2', ';leading genH pT;N_{events}', 100, 0, 500); h2.SetLineColor(3)

    c = TCanvas('c', 'c', 1200, 800)
    #c.SetLogy()

    leg = TLegend(0.75,0.75,0.85,0.85)
    leg.SetBorderSize(0)
    leg.SetTextFont(43)
    leg.SetTextSize(15)
    
    tree_rew2.Draw("leadHpt>>+h2", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree_rew2, bm+"_nlo2nlo", "LPE")
    tree_rew1.Draw("leadHpt>>+h1", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree_rew1, bm+"_lo2nlo", "LPE")
    tree.Draw("leadHpt>>+h", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree, bm, "LPE")
    
    leg.Draw("same")

    lumi = 137200 # 137.2/fb
    lumi = 1 # 59.7/fb

    h.Scale(lumi/h_eff.GetBinContent(5))
    h1.Scale(lumi/h_eff_rew1.GetBinContent(5))
    h2.Scale(lumi/h_eff_rew2.GetBinContent(5))
    #h2.SetMaximum(0.8e-8)

    #c.SaveAs(plotdir+'/'+bm+'_leadingHpt.png')
    c.SaveAs(plotdir+'/'+bm+'_leadingGenHpt.pdf')

    del c, h, h1, h2 # delete to allow new plot without warnings and errors


    # ***************************************************************************************
    # plot reco leading H pT
    # h = TH1F('h', ';leading H pT;N_{events}', 100, 0, 500); h.SetLineColor(9)
    # h1 = TH1F('h1', ';leading H pT;N_{events}', 100, 0, 500); h1.SetLineColor(2)
    # h2 = TH1F('h2', ';leading H pT;N_{events}', 100, 0, 500); h2.SetLineColor(3)
    
    # c = TCanvas('c', 'c', 1200, 800)
    # #c.SetLogy()

    # leg = TLegend(0.75,0.75,0.85,0.85)
    # leg.SetBorderSize(0)
    # leg.SetTextFont(43)
    # leg.SetTextSize(15)
    
    # tree_rew2.Draw("tauH_pt>>+h2", "aMCatNLOweight*HHweight*(tauH_pt>bH_pt)", "hist");
    # tree_rew2.Draw("bH_pt>>+h2", "aMCatNLOweight*HHweight*(tauH_pt<bH_pt)", "hist same"); leg.AddEntry(tree_rew2, bm+"_nlo2nlo", "LPE")

    # tree_rew1.Draw("tauH_pt>>+h1", "aMCatNLOweight*HHweight*(tauH_pt>bH_pt)", "hist same");
    # tree_rew1.Draw("bH_pt>>+h1", "aMCatNLOweight*HHweight*(tauH_pt<bH_pt)", "hist same"); leg.AddEntry(tree_rew1, bm+"_lo2nlo", "LPE")
    
    # tree.Draw("tauH_pt>>+h", "aMCatNLOweight*HHweight*(tauH_pt>bH_pt)", "hist same");
    # tree.Draw("bH_pt>>+h", "aMCatNLOweight*HHweight*(tauH_pt<bH_pt)", "hist same"); leg.AddEntry(tree, bm, "LPE")
    
    # leg.Draw("same")

    # lumi = 137200 # 137.2/fb
    # lumi = 1 # 59.7/fb

    # h.Scale(lumi/h_eff.GetBinContent(5))
    # h1.Scale(lumi/h_eff_rew1.GetBinContent(5))
    # h2.Scale(lumi/h_eff_rew2.GetBinContent(5))
    # #h2.SetMaximum(0.8e-8)

    # #c.SaveAs(plotdir+'/'+bm+'_leadingHpt.png')
    # c.SaveAs(plotdir+'/'+bm+'_leadingRecoHpt.pdf')

    # del c, h, h1, h2 # delete to allow new plot without warnings and errors


    # ***************************************************************************************
    # plot gen leading H pT divided by gen HH mass
    h = TH1F('h', ';leading genH pT / genMHH;N_{events}', 100, 0, 0.7); h.SetLineColor(9)
    h1 = TH1F('h1', ';leading genH pT / genMHH;N_{events}', 100, 0, 0.7); h1.SetLineColor(2)
    h2 = TH1F('h2', ';leading genH pT / genMHH;N_{events}', 100, 0, 0.7); h2.SetLineColor(3)

    c = TCanvas('c', 'c', 1200, 800)
    #c.SetLogy()

    leg = TLegend(0.75,0.75,0.85,0.85)
    leg.SetBorderSize(0)
    leg.SetTextFont(43)
    leg.SetTextSize(15)

    tree_rew2.Draw("leadHpt/genMHH>>+h2", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree_rew2, bm+"_nlo2nlo", "LPE")
    tree_rew1.Draw("leadHpt/genMHH>>+h1", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree_rew1, bm+"_lo2nlo", "LPE")
    tree.Draw("leadHpt/genMHH>>+h", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree, bm, "LPE")
    
    leg.Draw("same")

    lumi = 137200 # 137.2/fb
    lumi = 1 # 59.7/fb

    h.Scale(lumi/h_eff.GetBinContent(5))
    h1.Scale(lumi/h_eff_rew1.GetBinContent(5))
    h2.Scale(lumi/h_eff_rew2.GetBinContent(5))
    #h2.SetMaximum(1.0e-8)

    #c.SaveAs(plotdir+'/'+bm+'_leadingHpt_o_genMHH.png')
    c.SaveAs(plotdir+'/'+bm+'_leadingGenHpt_o_genMHH.pdf')

    del c, h, h1, h2 # delete to allow new plot without warnings and errors


    # ***************************************************************************************
    # plot gen sunleading H pT
    h = TH1F('h', ';subleading genH pT;N_{events}', 100, 0, 500); h.SetLineColor(9)
    h1 = TH1F('h1', ';subleading genH pT;N_{events}', 100, 0, 500); h1.SetLineColor(2)
    h2 = TH1F('h2', ';subleading genH pT;N_{events}', 100, 0, 500); h2.SetLineColor(3)

    c = TCanvas('c', 'c', 1200, 800)
    #c.SetLogy()

    leg = TLegend(0.75,0.75,0.85,0.85)
    leg.SetBorderSize(0)
    leg.SetTextFont(43)
    leg.SetTextSize(15)
    
    tree_rew2.Draw("sublHpt>>+h2", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree_rew2, bm+"_nlo2nlo", "LPE")
    tree_rew1.Draw("sublHpt>>+h1", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree_rew1, bm+"_lo2nlo", "LPE")
    tree.Draw("sublHpt>>+h", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree, bm, "LPE")
    
    leg.Draw("same")

    lumi = 137200 # 137.2/fb
    lumi = 1 # 59.7/fb

    h.Scale(lumi/h_eff.GetBinContent(5))
    h1.Scale(lumi/h_eff_rew1.GetBinContent(5))
    h2.Scale(lumi/h_eff_rew2.GetBinContent(5))
    #h2.SetMaximum(1.0e-8)

    #c.SaveAs(plotdir+'/'+bm+'_leadingHpt.png')
    c.SaveAs(plotdir+'/'+bm+'_subleadingGenHpt.pdf')

    del c, h, h1, h2 # delete to allow new plot without warnings and errors


    # ***************************************************************************************
    # plot reco subleading H pT
    # h = TH1F('h', ';subleading H pT;N_{events}', 100, 0, 500); h.SetLineColor(9)
    # h1 = TH1F('h1', ';subleading H pT;N_{events}', 100, 0, 500); h1.SetLineColor(2)
    # h2 = TH1F('h2', ';subleading H pT;N_{events}', 100, 0, 500); h2.SetLineColor(3)

    # c = TCanvas('c', 'c', 1200, 800)
    # #c.SetLogy()

    # leg = TLegend(0.75,0.75,0.85,0.85)
    # leg.SetBorderSize(0)
    # leg.SetTextFont(43)
    # leg.SetTextSize(15)
    
    # tree_rew2.Draw("tauH_pt>>+h2", "aMCatNLOweight*HHweight*(tauH_pt<bH_pt)", "hist");
    # tree_rew2.Draw("bH_pt>>+h2", "aMCatNLOweight*HHweight*(tauH_pt>bH_pt)", "hist same"); leg.AddEntry(tree_rew2, bm+"_nlo2nlo", "LPE")

    # tree_rew1.Draw("tauH_pt>>+h1", "aMCatNLOweight*HHweight*(tauH_pt<bH_pt)", "hist same");
    # tree_rew1.Draw("bH_pt>>+h1", "aMCatNLOweight*HHweight*(tauH_pt>bH_pt)", "hist same"); leg.AddEntry(tree_rew1, bm+"_lo2nlo", "LPE")
    
    # tree.Draw("tauH_pt>>+h", "aMCatNLOweight*HHweight*(tauH_pt<bH_pt)", "hist same");
    # tree.Draw("bH_pt>>+h", "aMCatNLOweight*HHweight*(tauH_pt>bH_pt)", "hist same"); leg.AddEntry(tree, bm, "LPE")
    
    # leg.Draw("same")

    # lumi = 137200 # 137.2/fb
    # lumi = 1 # 59.7/fb

    # h.Scale(lumi/h_eff.GetBinContent(5))
    # h1.Scale(lumi/h_eff_rew1.GetBinContent(5))
    # h2.Scale(lumi/h_eff_rew2.GetBinContent(5))
    # #h2.SetMaximum(1.0e-8)

    # #c.SaveAs(plotdir+'/'+bm+'_subleadingHpt.png')
    # c.SaveAs(plotdir+'/'+bm+'_subleadingRecoHpt.pdf')

    # del c, h, h1, h2 # delete to allow new plot without warnings and errors


    # ***************************************************************************************
    # plot gen subleading H pT divided by gen HH mass
    h = TH1F('h', ';subleading H pT / genMHH;N_{events}', 100, 0, 0.7); h.SetLineColor(9)
    h1 = TH1F('h1', ';subleading H pT / genMHH;N_{events}', 100, 0, 0.7); h1.SetLineColor(2)
    h2 = TH1F('h2', ';subleading H pT / genMHH;N_{events}', 100, 0, 0.7); h2.SetLineColor(3)

    c = TCanvas('c', 'c', 1200, 800)
    #c.SetLogy()

    leg = TLegend(0.75,0.75,0.85,0.85)
    leg.SetBorderSize(0)
    leg.SetTextFont(43)
    leg.SetTextSize(15)
    
    tree_rew2.Draw("sublHpt/genMHH>>+h2", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree_rew2, bm+"_nlo2nlo", "LPE")
    tree_rew1.Draw("sublHpt/genMHH>>+h1", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree_rew1, bm+"_lo2nlo", "LPE")
    tree.Draw("sublHpt/genMHH>>+h", "aMCatNLOweight*HHweight", "hist same"); leg.AddEntry(tree, bm, "LPE")
    
    leg.Draw("same")

    lumi = 137200 # 137.2/fb
    lumi = 1 # 59.7/fb

    h.Scale(lumi/h_eff.GetBinContent(5))
    h1.Scale(lumi/h_eff_rew1.GetBinContent(5))
    h2.Scale(lumi/h_eff_rew2.GetBinContent(5))
    #h2.SetMaximum(1.0e-8)

    #c.SaveAs(plotdir+'/'+bm+'_subleadingHpt_o_genMHH.png')
    c.SaveAs(plotdir+'/'+bm+'_subleadingGenHpt_o_genMHH.pdf')

    del c, h, h1, h2 # delete to allow new plot without warnings and errors


    del base, tree, h_eff, rew1, tree_rew1, h_eff_rew1, rew2, tree_rew2, h_eff_rew2



















