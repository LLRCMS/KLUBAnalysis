from array import array
from ROOT import *

## do ratio of Data/MC
# horErrs : do horizonta errors
def makeDataOverMCRatioPlot (hData, hMC, newName, horErrs=False):
    nPoints = hData.GetNbinsX()
    fX       = []
    fY       = []
    feYUp    = []
    feYDown  = []
    feXRight = []
    feXLeft  = []

    for ibin in range (1, nPoints+1):
        num = hData.GetBinContent(ibin)
        den = hMC.GetBinContent(ibin)
        if den > 0:
            # Y
            fY.append(num/den)
            feYUp.append(hData.GetBinErrorUp(ibin) / den)
            feYDown.append(hData.GetBinErrorLow(ibin) / den)

            # X
            fX.append (hData.GetBinCenter(ibin))
            if horErrs:
                feXRight.append(hData.GetBinLowEdge(ibin+1) - hData.GetBinCenter(ibin))
                feXLeft.append(hData.GetBinCenter(ibin) - hData.GetBinLowEdge(ibin))
            else:
                feXLeft.append(0.0)
                feXRight.append(0.0)

    afX       = array ("d", fX      )
    afY       = array ("d", fY      )
    afeYUp    = array ("d", feYUp   )
    afeYDown  = array ("d", feYDown )
    afeXRight = array ("d", feXRight)
    afeXLeft  = array ("d", feXLeft )

    gRatio = TGraphAsymmErrors (len(afX), afX, afY, afeXLeft, afeXRight, afeYDown, afeYUp);
    
    gRatio.SetMarkerStyle(8);
    gRatio.SetMarkerSize(1.);
    gRatio.SetMarkerColor(kBlack);
    gRatio.SetLineColor(kBlack);
    gRatio.SetName(newName)

    return gRatio;

# remove all points with content = 0
def removeEmptyPoints (graph):
    zeroes = []
    for ipt in range (0, graph.GetN()):
        x = Double(0.0)
        y = Double(0.0)
        graph.GetPoint(ipt,x,y)
        if y == 0:
            zeroes.append(ipt)
    for i in reversed (zeroes):
        graph.RemovePoint(i) 

######################

if __name__ == "__main__":
    
    varName = "pt1"
    title = ";lead #tau p_{T} [GeV];Events"
    pairType = "2"
    plotBinsRange = [100, 0, 500]

    # varName = "pt2"
    # title = ";sublead #tau p_{T} [GeV];Events"
    # pairType = "2"
    # plotBinsRange = [50, 0, 250]


    ###############
    gROOT.SetBatch(True)
    gStyle.SetOptStat(0)

    titleSize = 30
    labelSize = 22
    
    fIn = TFile("reweightTree.root")
    tree = fIn.Get("triggerlessTree")

    hTrg = TH1F ("hTrg", "hTrg", plotBinsRange[0], plotBinsRange[1], plotBinsRange[2])
    hRew = TH1F ("hRew", "hRew", plotBinsRange[0], plotBinsRange[1], plotBinsRange[2])

    # hTrg_pt2 = TH1F ("hTrg_pt2", "hTrg_pt2", 100, 0, 200)
    # hRew_pt2 = TH1F ("hRew_pt2", "hRew_pt2", 100, 0, 200)

    c1 = TCanvas ("c1", "c1", 600, 600)

    tree.Draw(("%s>>hTrg" % varName), ("pairType == %s && triggerAccept==1" %  pairType))
    tree.Draw(("%s>>hRew" % varName), ("(pairType == %s)*evtWeight" %  pairType))

    # tree.Draw("pt2>>hRew_pt2", "(paitType == %s)*evtWeight" % pairType)
    # tree.Draw("pt2>>hTrg_pt2", "paitType == %s && triggerAccept==1" % pairType)

    hTrg.Sumw2()
    hTrg.SetLineColor(kBlack)
    hTrg.SetMarkerColor(kBlack)
    hTrg.SetMarkerStyle(8)
    hTrg.SetMarkerSize(0.7)
    hRew.SetLineColor(kRed)
    hRew.SetLineWidth(2)
    # hTrg_pt2.SetLineColor(kRed)

    hRew.GetYaxis().SetTitleFont(43) # so that size is in pixels
    hRew.GetYaxis().SetLabelFont(43) # so that size is in pixels
    # hRew.GetYaxis().SetNdivisions(505)
    hRew.GetYaxis().SetTitle ("Trg/Rew")
    hRew.GetYaxis().SetTitleOffset(1.2)
    hRew.GetYaxis().SetTitleSize(titleSize);
    hRew.GetYaxis().SetLabelSize(labelSize);
    hRew.GetYaxis().SetTickSize(0.05)
    hRew.SetTitle(title)

    ##### plot
    pad1 = TPad ("pad1", "pad1", 0, 0.25, 1, 1.0)
    pad1.SetFrameLineWidth(3)
    pad1.SetLeftMargin(0.15);
    pad1.SetBottomMargin(0.02);
    pad1.SetTopMargin(0.055);

    pad1.Draw()
    pad1.cd()
    mm = max (hTrg.GetMaximum(), hRew.GetMaximum())
    hRew.SetMaximum(1.2*mm)
    hRew.GetXaxis().SetTitleSize(0.00);
    hRew.GetXaxis().SetLabelSize(0.00);
    hRew.Draw("hist")
    hTrg.Draw("same")

    ## legend
    leg = TLegend (0.48, 0.75, 0.85, 0.93)
    leg.SetFillStyle(0)
    leg.SetBorderSize(0)
    leg.SetTextFont(43)
    leg.SetTextSize(20)
    leg.AddEntry(hRew, "Reweighting", "l")
    leg.AddEntry(hTrg, "Trigger + macthing", "p")
    leg.Draw()
    #### ratio plot

    c1.cd()
    pad2 = TPad ("pad2", "pad2", 0, 0.0, 1, 0.2496)
    pad2.SetLeftMargin(0.15);
    pad2.SetTopMargin(0.05);
    pad2.SetBottomMargin(0.35);
    pad2.SetGridy(True);
    pad2.SetFrameLineWidth(3)
    #pad2.SetGridx(True);
    pad2.Draw()
    pad2.cd()


    grRatio = makeDataOverMCRatioPlot (hTrg, hRew, "grRatio")
    hRatio = hRew.Clone("hRatioAxis") # for ranges only

    hRatio.GetXaxis().SetTitleFont(43) # so that size is in pixels
    hRatio.GetYaxis().SetTitleFont(43) # so that size is in pixels
    hRatio.GetXaxis().SetLabelFont(43) # so that size is in pixels
    hRatio.GetYaxis().SetLabelFont(43) # so that size is in pixels
    hRatio.GetYaxis().SetNdivisions(505)

    #hRatio.GetXaxis().SetTitle(bkgStack.GetXaxis().GetName())
    hRatio.SetTitle(title)
    hRatio.GetYaxis().SetTitle ("Trg/Rew")
    hRatio.GetXaxis().SetTitleOffset(2.8)
    hRatio.GetYaxis().SetTitleOffset(1.2)

    hRatio.GetXaxis().SetTitleSize(titleSize);
    hRatio.GetXaxis().SetLabelSize(labelSize);
    hRatio.GetYaxis().SetTitleSize(titleSize);
    hRatio.GetYaxis().SetLabelSize(labelSize);

    hRatio.GetXaxis().SetTickSize(0.10)
    hRatio.GetYaxis().SetTickSize(0.05)

    hRatio.SetStats(0)
    # hRatio.SetMinimum(0.5)
    # hRatio.SetMaximum(1.5)
    hRatio.SetMinimum(0.1)
    hRatio.SetMaximum(1.9)

    removeEmptyPoints (grRatio)
    hRatio.Draw("axis")
    grRatio.Draw("P Z same") # Z : no small limes at the end of points
    pad2.RedrawAxis();
    pad2.RedrawAxis("g"); #otherwise no grid..

    c1.Update()
    c1.Print("%s_pair%s.pdf" % (varName, pairType) )
    raw_input()

