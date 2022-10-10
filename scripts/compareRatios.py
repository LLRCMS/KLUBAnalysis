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
    files, gnames = ([] for _ in range(2))
    for sel in args.sel:
        fname = op.join(args.indir, args.channel, sel + '_' + args.reg, 'ratios.root')
        files.append(ROOT.TFile.Open(fname))
        gnames.append('_'.join((args.var, sel, args.reg, args.channel)))

    g1 = files[0].Get(gnames[0])
    g2 = files[1].Get(gnames[1])
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
    pad1 = ROOT.TPad('pad1', 'pad1', 0, 0.0, 1.0, 1.0)
    pad1.SetFrameLineWidth(3)
    pad1.SetLeftMargin(0.12);
    pad1.SetBottomMargin(0.12);
    pad1.SetTopMargin(0.055);
    pad1.Draw()
    pad1.cd()

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
            feYUp.append( ((g1.GetErrorYhigh(n)/num)**2 + (g2.GetErrorYhigh(n)/den)**2)**0.5 )
            feYDown.append( ((g1.GetErrorYlow(n)/num)**2 + (g2.GetErrorYlow(n)/den)**2)**0.5 )
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
    hRatio.GetYaxis().SetRangeUser(0.699,+1.301);
    hRatio.SetMarkerStyle(8);
    hRatio.SetMarkerSize(1.);
    hRatio.SetMarkerColor(ROOT.kBlack);
    hRatio.SetLineColor(ROOT.kBlack);
    hRatio.SetName(name)
    hRatio.SetTitle('')
    hRatio.GetXaxis().SetTitle(args.label)
    hRatio.Draw('ALP') #'P Z'

    xmin = hRatio.GetXaxis().GetXmin()
    xmax = hRatio.GetXaxis().GetXmax()
    l1 = ROOT.TLine(xmin, 1, xmax, 1)
    l1.SetLineColor(ROOT.kRed)
    l1.SetLineStyle(4)
    l1.SetLineWidth(1)
    l1.Draw('same')
    l2 = ROOT.TLine(xmin, 0.95, xmax, 0.95)
    l2.SetLineColor(ROOT.kRed)
    l2.SetLineStyle(4)
    l2.SetLineWidth(1)
    l2.Draw('same')
    l3 = ROOT.TLine(xmin, 1.05, xmax, 1.05)
    l3.SetLineColor(ROOT.kRed)
    l3.SetLineStyle(4)
    l3.SetLineWidth(1)
    l3.Draw('same')

    cmsTextFont = 61  # font of the "CMS" label
    cmsTextSize = 0.05  # font size of the "CMS" label
    extraTextFont = 52     # for the "preliminary"
    extraTextSize = 0.76 * cmsTextSize # for the "preliminary"
    t = ROOT.gPad.GetTopMargin()
    b = ROOT.gPad.GetBottomMargin()
    l = ROOT.gPad.GetLeftMargin()
    r = ROOT.gPad.GetRightMargin()

    CMSbox       = ROOT.TLatex(l , 1 - t + 0.02, 'CMS')       
    extraTextBox = ROOT.TLatex(l + 0.1 , 1 - t + 0.02, 'Preliminary')
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

    selName = args.sel[0] + ' / ' + args.sel[1]
    selBox = ROOT.TLatex(l + 0.04 , 1 - t - 0.02 - 0.06, selName)
    selBox.SetNDC()
    selBox.SetTextSize(cmsTextSize+20)
    selBox.SetTextFont(43)
    selBox.SetTextColor(ROOT.kBlack)
    selBox.SetTextAlign(13)
    selBox.Draw('same')

    chBox = ROOT.TLatex(l + 0.04 , 1 - t - 0.02, chName)
    chBox.SetNDC()
    chBox.SetTextSize(cmsTextSize+20)
    chBox.SetTextFont(43)
    chBox.SetTextColor(ROOT.kBlack)
    chBox.SetTextAlign(13)
    chBox.Draw('same')

    for ext in ('png',):
        c1.SaveAs(op.join(args.outdir,name + '.' + ext))
                
if __name__ == '__main__' :
    ROOT.gROOT.SetBatch(True)
    ROOT.TH1.AddDirectory(0)

    titleSize = 24
    labelSize = 22

    parser = argparse.ArgumentParser(description='Compare Data/MC ratios obtained with different baselines')
    parser.add_argument('--indir', dest='indir',
                        help='analysis input folder name', default='.')
    parser.add_argument('--outdir', dest='outdir',
                        help='analysis output folder name', default='.')
    parser.add_argument('--var', dest='var', help='variable name', default=None)
    parser.add_argument('--sel', dest='sel', help='selection names', nargs='+', default=None)
    parser.add_argument('--reg', dest='reg', help='region name', default=None)
    parser.add_argument('--label', dest='label', help='x label', default=None)
    parser.add_argument('--channel', dest='channel',
                        choices=('ETau', 'MuTau', 'TauTau'),
                        help='channel', required=True)
    parser.add_argument('--main_cfg',
                        help='Main configuration file (used in previous steps as well)',
                        default=None)
    parser.add_argument('--lumi', dest='lumi', type=float, help='lumi in fb-1', default=None)

    FLAGS = parser.parse_args()
    assert len(FLAGS.sel) == 2
    
    compareRatios(FLAGS)
