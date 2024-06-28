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

def set_axis_limits(graph):
    xmin = graph.GetXaxis().GetXmin()
    xmax = graph.GetXaxis().GetXmax()
    # ymin = graph.GetYaxis().GetXmin()
    # ymax = graph.GetYaxis().GetXmax()
    # xaxis = graph.GetXaxis()
    # diff = (xmax-xmin)/4
    # xaxis.SetLimits(xmin - diff/4, xmax + diff/4)
    # yaxis = graph.GetYaxis()
    # diff = (ymax-ymin)/4
    # yaxis.SetLimits(ymin, ymax + diff/5)
    return xmin, xmax

def compareRatios(args):
    fend = op.join(args.channel, args.sel + '_' + args.reg, 'ratios.root')
    file1 = ROOT.TFile.Open(op.join(args.indir, args.tags[0], args.channel, 'HH_Plots', fend))
    file2 = ROOT.TFile.Open(op.join(args.indir, args.tags[1], args.channel, 'HH_Plots', fend))
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
    g1.GetYaxis().SetTitle("Data/MC")
    g1.GetYaxis().SetTitleSize(0.06)
    g1.GetYaxis().SetTitleOffset(1.0)
    g1.Draw("APL")
    g2.SetLineColor(ROOT.kRed);
    g2.SetMarkerColor(ROOT.kRed);
    g2.GetYaxis().SetTitle("Ratio")
    g2.GetYaxis().SetTitleSize(0.01)
    g2.Draw("PL same")

    xmin, xmax = set_axis_limits(g1)
    set_axis_limits(g2)
        
    lines = []
    for val in (0.8, 0.9, 1., 1.1, 1.2):
        lines.append(ROOT.TLine(xmin, val, xmax, val))
        lines[-1].SetLineColor(ROOT.kGray)
        lines[-1].SetLineStyle(4)
        lines[-1].SetLineWidth(1)
        lines[-1].Draw('same')

    legend = ROOT.TLegend(0.62,0.81,0.89,0.91);
    legend.AddEntry(g1, args.tags[0], "l");
    legend.AddEntry(g2, args.tags[1], "l");
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
    lines2 = []
    for val in (.8, .9, 1., 1.1, 1.2):
        lines2.append(ROOT.TLine(xmin, val, xmax, val))
        lines2[-1].SetLineColor(ROOT.kRed)
        lines2[-1].SetLineStyle(4)
        lines2[-1].SetLineWidth(1)
        lines2[-1].Draw('same')

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

    if args.year == "UL18":
        lumi = "59.7"
    elif args.year == "UL17":
        lumi = "41.5"
    elif args.year == "UL16":
        lumi = "16.8"
    elif args.year == "UL16APV":
        lumi = "19.5"
    lumi = '{} fb^{{-1}} (13 TeV)'.format(lumi)
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
                        help='analysis input first folder name')
    parser.add_argument('--outdir', help='analysis output folder name', default='.')
    parser.add_argument('--var', help='variable name', default=None)
    parser.add_argument('--sel', help='selection name', default=None)
    parser.add_argument('--tags', help='selection names', nargs='+', default=None)
    parser.add_argument('--reg', help='region name', default=None)
    parser.add_argument('--label', help='x label', default=None)
    parser.add_argument('--channel',  choices=('ETau', 'MuTau', 'TauTau'),
                        help='channel', required=True)
    parser.add_argument('--year', required=True, choices=("UL61", "UL16APV", "UL17", "UL18"),
                        help='lumi in fb-1', default=None)

    FLAGS = parser.parse_args()
    assert len(FLAGS.tags) == 2
    
    compareRatios(FLAGS)
