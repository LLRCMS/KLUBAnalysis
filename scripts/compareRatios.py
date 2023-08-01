import os
import os.path as op
import argparse
from array import array
from ctypes import c_double, c_float
import ROOT

## find maximum of tgraph, including error
def findMaxOfGraph(graph):
    uppers = []
    for i in range (0, graph.GetN()):
        x = c_double(0.0)
        y = c_double(0.0)
        graph.GetPoint(i, x, y)
        uppers.append(y.value + graph.GetErrorYhigh(i))
    return max(uppers)

def compareRatios(args):
    fend = op.join(args.channel, args.sel + '_' + args.reg, 'ratios.root')
    file1 = ROOT.TFile.Open(op.join(args.indir, args.tags[0], 'HH_Plots', fend))
    file2 = ROOT.TFile.Open(op.join(args.indir, args.tags[1], 'HH_Plots', fend))
    gname = '_'.join((args.var, args.sel, args.reg, args.channel))

    g1 = file1.Get(gname)
    g2 = file2.Get(gname)
    if g1.GetN() != g2.GetN():
        mes = 'Ratio graph {} has {} bins, but '.format(gnames[0], g1.GetN())
        mes += '{} has {} bins.'.format(gnames[1], g2.GetN())
        raise ValueError(mes)
    if args.channel == 'MuTau':
        chName = 'bb #mu#tau_{h}'
    elif args.channel == 'ETau':
        chName = 'bb e#tau_{h}'
    elif args.channel == 'TauTau':
        chName = 'bb #tau_{h}#tau_{h}'

    name = '_'.join((args.var, args.reg, args.channel))
    c1 = ROOT.TCanvas('c1', 'c1', 600, 600)

    ### First Pad
    pad1 = ROOT.TPad('pad1', 'pad1', 0, 0.3, 1.0, 1.0)
    pad1.SetFrameLineWidth(3)
    pad1.SetLeftMargin(0.12);
    pad1.SetBottomMargin(0.);
    pad1.SetTopMargin(0.07);
    pad1.Draw()
    pad1.cd()

    g1.SetTitle('')
    g1.GetYaxis().SetLabelSize(.05);
    g1.SetLineColor(ROOT.kBlue);
    g1.SetMarkerColor(ROOT.kBlue);
    g1.Draw("APL")
    g2.SetLineColor(ROOT.kRed);
    g2.SetMarkerColor(ROOT.kRed);
    g2.Draw("PL same")

    xmin = g1.GetXaxis().GetXmin()
    xmax = g2.GetXaxis().GetXmax()
    l1 = ROOT.TLine(xmin, 1, xmax, 1)
    l1.SetLineColor(ROOT.kGray)
    l1.SetLineStyle(4)
    l1.SetLineWidth(1)
    l1.Draw('same')
    l2 = ROOT.TLine(xmin, 0.9, xmax, 0.9)
    l2.SetLineColor(ROOT.kGray)
    l2.SetLineStyle(4)
    l2.SetLineWidth(1)
    l2.Draw('same')
    l3 = ROOT.TLine(xmin, 1.1, xmax, 1.1)
    l3.SetLineColor(ROOT.kGray)
    l3.SetLineStyle(4)
    l3.SetLineWidth(1)
    l3.Draw('same')
    # l4 = ROOT.TLine(xmin, 0.8, xmax, 0.8)
    # l4.SetLineColor(ROOT.kBlue)
    # l4.SetLineStyle(4)
    # l4.SetLineWidth(1)
    # l4.Draw('same')
    # l5 = ROOT.TLine(xmin, 1.2, xmax, 1.2)
    # l5.SetLineColor(ROOT.kBlue)
    # l5.SetLineStyle(4)
    # l5.SetLineWidth(1)
    # l5.Draw('same')

    legend = ROOT.TLegend(0.82,0.80,0.89,0.91);
    legend.AddEntry(g1, "old", "l");
    legend.AddEntry(g2, "new", "l");
    legend.Draw("same");
    
    ### Second Pad
    c1.cd()
    pad2 = ROOT.TPad('pad2', 'pad2', 0, 0.0, 1.0, 0.3)
    pad2.SetFrameLineWidth(3)
    pad2.SetLeftMargin(0.12);
    pad2.SetBottomMargin(0.28);
    pad2.SetTopMargin(0.0);
    pad2.Draw()
    pad2.cd()

    fX       = []
    fY       = []
    feYUp    = []
    feYDown  = []
    feXRight = []
    feXLeft  = []
    for n in range(g1.GetN()):
        num = g1.GetPointY(n)
        den = g2.GetPointY(n)
        if not den > 0. or not num > 0.:
            fX.append(0.)
            fY.append(0.)
            feYUp.append(0.)
            feYDown.append(0.)
            feXRight.append(0.)
            feXLeft.append(0.)
        else:
            fX.append(g1.GetPointX(n))
            fY.append(num/den)
            feYUp.append( ((g1.GetErrorYhigh(n)/num)**2 + (g2.GetErrorYhigh(n)/den)**2)**0.5 / 2)
            feYDown.append( ((g1.GetErrorYlow(n)/num)**2 + (g2.GetErrorYlow(n)/den)**2)**0.5 / 2)
            feXRight.append(0.)
            feXLeft.append(0.)

    afX       = array('d', fX      )
    afY       = array('d', fY      )
    afeYUp    = array('d', feYUp   )
    afeYDown  = array('d', feYDown )
    afeXRight = array('d', feXRight)
    afeXLeft  = array('d', feXLeft )
    hRatio = ROOT.TGraphAsymmErrors(len(afX), afX, afY, afeXLeft, afeXRight, afeYDown, afeYUp);

    # gmax = findMaxOfGraph(hRatio)
    amin = ROOT.TMath.MinElement(hRatio.GetN(),hRatio.GetY())
    amax = ROOT.TMath.MaxElement(hRatio.GetN(),hRatio.GetY())
    scale_up = (amax-amin)/4
    scale_down = (amax-amin)/10
    # hRatio.SetMinimum(amin - scale_down)
    # hRatio.SetMaximum(amax + scale_up)
    hRatio.GetYaxis().SetRangeUser(0.801,1.19);
    hRatio.SetMarkerStyle(8);
    hRatio.SetMarkerSize(1.);
    hRatio.SetMarkerColor(ROOT.kBlack);
    hRatio.SetLineColor(ROOT.kBlack);
    hRatio.SetName(name)
    hRatio.SetTitle('')
    hRatio.GetXaxis().SetLabelSize(0.12);
    hRatio.GetXaxis().SetTitleSize(0.12);
    hRatio.GetYaxis().SetLabelSize(0.12);
    hRatio.GetYaxis().SetTitleSize(0.12);
    hRatio.GetXaxis().SetTitle(args.label)
    hRatio.Draw('ALP') #'P Z'

    xmin = hRatio.GetXaxis().GetXmin()
    xmax = hRatio.GetXaxis().GetXmax()
    l6 = ROOT.TLine(xmin, 1, xmax, 1)
    l6.SetLineColor(ROOT.kRed)
    l6.SetLineStyle(4)
    l6.SetLineWidth(1)
    l6.Draw('same')
    l7 = ROOT.TLine(xmin, 0.9, xmax, 0.9)
    l7.SetLineColor(ROOT.kRed)
    l7.SetLineStyle(4)
    l7.SetLineWidth(1)
    l7.Draw('same')
    l8 = ROOT.TLine(xmin, 1.1, xmax, 1.1)
    l8.SetLineColor(ROOT.kRed)
    l8.SetLineStyle(4)
    l8.SetLineWidth(1)
    l8.Draw('same')
    l9 = ROOT.TLine(xmin, 0.8, xmax, 0.8)
    l9.SetLineColor(ROOT.kBlue)
    l9.SetLineStyle(4)
    l9.SetLineWidth(1)
    l9.Draw('same')
    l10 = ROOT.TLine(xmin, 1.2, xmax, 1.2)
    l10.SetLineColor(ROOT.kBlue)
    l10.SetLineStyle(4)
    l10.SetLineWidth(1)
    l10.Draw('same')

    cmsTextFont = 61  # font of the "CMS" label
    cmsTextSize = 0.05  # font size of the "CMS" label
    extraTextFont = 52     # for the "preliminary"
    extraTextSize = 0.76 * cmsTextSize # for the "preliminary"
    t = ROOT.gPad.GetTopMargin() + 0.06
    b = ROOT.gPad.GetBottomMargin()
    l = ROOT.gPad.GetLeftMargin()
    r = ROOT.gPad.GetRightMargin()

    c1.cd()
    
    CMSbox       = ROOT.TLatex(l , 1 - t + 0.02, 'CMS')       
    extraTextBox = ROOT.TLatex(l + 0.11 , 1 - t + 0.02, 'Preliminary')
    CMSbox.SetNDC()
    extraTextBox.SetNDC()
    CMSbox.SetTextSize(cmsTextSize)
    CMSbox.SetTextFont(cmsTextFont)
    CMSbox.SetTextColor(ROOT.kBlack)
    CMSbox.SetTextAlign(11)
    extraTextBox.SetTextSize(extraTextSize)
    extraTextBox.SetTextFont(extraTextFont)
    extraTextBox.SetTextColor(ROOT.kBlack)
    extraTextBox.SetTextAlign(11)

    lumi = '%.1f fb^{-1} (13 TeV)' % args.lumi
    lumibox = ROOT.TLatex(1-r, 1 - t + 0.02 , lumi)       
    lumibox.SetNDC()
    lumibox.SetTextAlign(31)
    lumibox.SetTextSize(extraTextSize)
    lumibox.SetTextFont(42)
    lumibox.SetTextColor(ROOT.kBlack)

    CMSbox.Draw('same')
    extraTextBox.Draw('same')
    lumibox.Draw('same')

    chBox = ROOT.TLatex(l + 0.04 , 1 - t - 0.01, chName)
    chBox.SetNDC()
    chBox.SetTextSize(cmsTextSize+20)
    chBox.SetTextFont(43)
    chBox.SetTextColor(ROOT.kBlack)
    chBox.SetTextAlign(13)
    chBox.Draw('same')

    selBox = ROOT.TLatex(l + 0.04 , 1 - t - 0.01 - 0.04, args.sel)
    selBox.SetNDC()
    selBox.SetTextSize(cmsTextSize+20)
    selBox.SetTextFont(43)
    selBox.SetTextColor(ROOT.kBlack)
    selBox.SetTextAlign(13)
    selBox.Draw('same')

    for ext in ('png', 'pdf'):
        c1.SaveAs(op.join(args.outdir,name + '.' + ext))
                
if __name__ == '__main__' :
    ROOT.gROOT.SetBatch(True)
    ROOT.TH1.AddDirectory(0)

    titleSize = 24
    labelSize = 22

    parser = argparse.ArgumentParser(description='Compare Data/MC ratios obtained with different baselines')
    parser.add_argument('--indir', default='/data_CMS/cms/alves/HHresonant_hist/',
                        help='analysis input first folder name', required=True)
    parser.add_argument('--outdir', help='analysis output folder name', default='.')
    parser.add_argument('--var', help='variable name', default=None)
    parser.add_argument('--sel', help='selection name', default=None)
    parser.add_argument('--tags', help='selection names', nargs='+', default=None)
    parser.add_argument('--reg', help='region name', default=None)
    parser.add_argument('--label', help='x label', default=None)
    parser.add_argument('--channel',  choices=('ETau', 'MuTau', 'TauTau'),
                        help='channel', required=True)
    parser.add_argument('--lumi', type=float, help='lumi in fb-1', default=None)

    FLAGS = parser.parse_args()
    assert len(FLAGS.tags) == 2
    
    compareRatios(FLAGS)
