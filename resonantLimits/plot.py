# coding: utf-8                                                                                                                             
_all_ = [ ]

import os
import argparse
from math import *
import ROOT

def redrawBorder():
   # this little macro redraws the axis tick marks and the pad border lines.
   ROOT.gPad.Update();
   ROOT.gPad.RedrawAxis();
   l = ROOT.TLine()
   l.SetLineWidth(3)
   l.DrawLine(ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymax(),
              ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymax())
   l.DrawLine(ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymin(),
              ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymax())
   l.DrawLine(ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymin(),
              ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymax())
   l.DrawLine(ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymin(),
              ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymin())

def getExpValue(kl, yt):
    BR = 1
    return (2.09*yt*yt*yt*yt + 0.28*yt*yt*kl*kl -1.37*yt*yt*yt*kl)*2.44477/BR;

def parseFile(filename, CL='50.0', exp=True):
    f = open(filename)
    matches = []
    for line in f:
        search = 'Expected %s%%: r <'%CL
        if not exp:
            search = 'Observed Limit: r <'

        if not search in line:
            continue
        val = line.replace(search, '')
        val = float(val)
        matches.append(val)
    if len(matches) == 0:
        mes = 'Did not find any expected in file: {}, CL={}, exp?={}'
        print(mes.format(filename, CL, exp))
        return -1.0
    else:
        return matches[-1]

def create_limits_plot(indir, outfile, masses, labels, signal, period, multigraph, canvas):
    gr2sigma = ROOT.TGraphAsymmErrors()
    gr1sigma = ROOT.TGraphAsymmErrors()
    grexp = ROOT.TGraph()
    grobs = ROOT.TGraph()
     
    ptsList = [] # (x, obs, exp, p2s, p1s, m1s, m2s)

    for mass in masses:
        fName = os.path.join(indir, 'comb.{}{}.log'.format(signal, mass))

        exp   = 10.*parseFile(fName)            
        obs   = exp #parseFile(fName, exp=False) 
        m1s_t = 10.*parseFile(fName, CL='16.0') 
        p1s_t = 10.*parseFile(fName, CL='84.0') 
        m2s_t = 10.*parseFile(fName, CL=' 2.5') 
        p2s_t = 10.*parseFile(fName, CL='97.5') 

        ## because the other code wants +/ sigma vars as deviations, without sign, from the centeal exp value...
        p2s = p2s_t - exp
        p1s = p1s_t - exp
        m2s = exp - m2s_t
        m1s = exp - m1s_t
        xval = mass
        ptsList.append((xval, obs, exp, p2s, p1s, m1s, m2s))
        
    ptsList.sort()
    for ipt, pt in enumerate(ptsList):
        xval = pt[0]
        obs  = pt[1]
        exp  = pt[2]
        p2s  = pt[3]
        p1s  = pt[4]
        m1s  = pt[5]
        m2s  = pt[6]

        if exp > 0:
            grexp.SetPoint(ipt, xval, exp)
            grobs.SetPoint(ipt, xval, obs)
            gr1sigma.SetPoint(ipt, xval, exp)
            gr2sigma.SetPoint(ipt, xval, exp)
            gr1sigma.SetPointError(ipt, 0, 0, m1s, p1s)
            gr2sigma.SetPointError(ipt, 0, 0, m2s, p2s)       

    ######## set styles
    grexp.SetMarkerStyle(24)
    grexp.SetMarkerColor(4)
    grexp.SetMarkerSize(0.8)
    grexp.SetLineColor(ROOT.kBlue+2)
    grexp.SetLineWidth(3)
    grexp.SetLineStyle(2)
    grexp.SetFillColor(0)
    
    grobs.SetLineColor(1)
    grobs.SetLineWidth(3)
    grobs.SetMarkerColor(1)
    grobs.SetMarkerStyle(20)
    grobs.SetFillStyle(0)
    
    gr1sigma.SetMarkerStyle(0)
    gr1sigma.SetMarkerColor(3)
    gr1sigma.SetFillColor(ROOT.kGreen+1)
    gr1sigma.SetLineColor(ROOT.kGreen+1)
    gr1sigma.SetFillStyle(1001)
    
    gr2sigma.SetMarkerStyle(0)
    gr2sigma.SetMarkerColor(5)
    gr2sigma.SetFillColor(ROOT.kOrange)
    gr2sigma.SetLineColor(ROOT.kOrange)
    gr2sigma.SetFillStyle(1001)
    
    multigraph.Add(gr2sigma, '3')
    multigraph.Add(gr1sigma, '3')
    multigraph.Add(grexp, 'L')
    multigraph.Add(grobs, 'L')

    ###########
    legend = ROOT.TLegend(0,0,0,0)
    legend.SetX1(0.17284)
    legend.SetY1(0.530526)
    legend.SetX2(0.520062)
    legend.SetY2(0.88)
    legend.SetX1(0.17284)
    legend.SetY1(0.170526)
    legend.SetX2(0.520062)
    legend.SetY2(0.450)
    legend.SetFillColor(ROOT.kWhite)
    legend.SetBorderSize(0)
    legend.SetHeader('95% CL upper limits')
    #legend.AddEntry(grobs, 'Observed', 'l')
    legend.AddEntry(grexp, 'Median expected', 'l')
    legend.AddEntry(gr1sigma, '68% expected', 'f')
    legend.AddEntry(gr2sigma, '95% expected', 'f')
    
    fakePlot3 = ROOT.TGraphAsymmErrors()
    fakePlot3.SetFillColor(ROOT.kRed)
    fakePlot3.SetFillStyle(3001)
    fakePlot3.SetLineColor(ROOT.kRed)
    fakePlot3.SetLineWidth(3)
      
    ##### text
    pt = ROOT.TPaveText(0.1663218-0.02,0.886316,0.3045977-0.02,0.978947,"brNDC")
    pt.SetBorderSize(0)
    pt.SetTextAlign(12)
    pt.SetTextFont(62)
    pt.SetTextSize(0.05)
    pt.SetFillColor(0)
    pt.SetFillStyle(0)
    pt.AddText("CMS #font[52]{Internal}" )
    #pt.AddText("CMS" )
    #pt.AddText("#font[52]{preliminary}")
    pt2 = ROOT.TPaveText(0.736111,0.9066667,0.847222,0.954641,"brNDC")
    pt2.SetBorderSize(0)
    pt2.SetFillColor(0)
    pt2.SetTextSize(0.040)
    pt2.SetTextFont(42)
    pt2.SetFillStyle(0)
    if period == 'UL16':
       pt2.AddText('2016 - 35.9 fb^{-1} (13 TeV)')
    elif period == 'UL17':
       pt2.AddText('2017 - 41.6 fb^{-1} (13 TeV)')
    elif period == 'UL18':
       pt2.AddText('2018 - 59.7 fb^{-1} (13 TeV)')
    else:
       pt2.AddText('Run2 - 137.1 fb^{-1} (13 TeV)')

    pt4 = ROOT.TPaveText(0.4819196+0.036, 0.7780357+0.015+0.02,
                         0.9008929+0.036, 0.8675595+0.015, 'brNDC')
    pt4.SetTextAlign(12)
    pt4.SetFillColor(ROOT.kWhite)
    pt4.SetFillStyle(1001)
    pt4.SetTextFont(42)
    pt4.SetTextSize(0.05)
    pt4.SetBorderSize(0)
    pt4.SetTextAlign(32)
    pt4.AddText(labels[0])
        
    offs = 0.020
    height = 0.05
    pt5 = ROOT.TPaveText(0.4819196+0.036+0.10, 0.7780357+0.015-offs,
                         0.9008929+0.036, 0.7780357+0.015-offs+height, 'brNDC')
    pt5.SetTextAlign(12)
    pt5.SetFillColor(ROOT.kWhite)
    pt5.SetFillStyle(1001)
    pt5.SetTextFont(42)
    pt5.SetTextSize(0.05)
    pt5.SetBorderSize(0)
    pt5.SetTextAlign(32)
    pt5.AddText(labels[1])
    
    ##### theory lines
    xmin=200
    xmax=3100
    
    yt = 1
    BR = 1
    myFunc = ROOT.TF1('myFunc', '(62.5339 -44.323*x + 9.6340*x*x)*1.115', xmin, xmax)
    graph = ROOT.TGraph(myFunc)
    ci = ROOT.TColor.GetColor('#ff0000')
    graph.SetLineColor(ci)
    graph.SetLineWidth(2)
    nP = int((xmax-xmin)*10.0)
    Graph_syst_Scale = ROOT.TGraphAsymmErrors(nP)

    for i in range(nP):
        Graph_syst_Scale_x=(xmin+(i*1.)/10.)
        Graph_syst_Scale_y=(getExpValue(xmin+(i*1.)/10.,yt)) 
        Graph_syst_Scale_x_err=(0)
        Graph_syst_Scale_y_errup = ((2.09*yt*yt*yt*yt+0.28*yt*yt*(xmin+(i*1.)/10.) *
                                    (xmin+(i*1.)/10.)-1.37*yt*yt*yt*(xmin+(i*1.)/10.))*2.44185*0.053/BR)
        Graph_syst_Scale_y_errdown = ((2.09*yt*yt*yt*yt+0.28*yt*yt*(xmin+(i*1.)/10.) *
                                       (xmin+(i*1.)/10.)-1.37*yt*yt*yt*(xmin+(i*1.)/10.))*2.44185*0.067/BR)
        Graph_syst_Scale.SetPoint(i,Graph_syst_Scale_x,Graph_syst_Scale_y)
        Graph_syst_Scale.SetPointError(i, Graph_syst_Scale_x_err, Graph_syst_Scale_x_err,
                                       Graph_syst_Scale_y_errup, Graph_syst_Scale_y_errdown)
    Graph_syst_Scale.SetLineColor(ROOT.kRed)
    Graph_syst_Scale.SetFillColor(ROOT.kRed)
    Graph_syst_Scale.SetFillStyle(3001)

    hframe = ROOT.TH1F('hframe_'+outfile, '', 100, 250, 3100)
    hframe.SetMinimum(0.1)
    if period == 'UL16':
        hframe.SetMaximum(10000)
        hframe.SetMinimum(0.1)#5000)
    elif period == 'UL17':
        hframe.SetMaximum(10000)
        hframe.SetMinimum(0.1)
    elif period == 'UL18':
        hframe.SetMinimum(0.1)
        hframe.SetMaximum(100000)
    else:
       hframe.SetMaximum(10000)
       hframe.SetMinimum(0.1)

    hframe.GetYaxis().SetTitleSize(0.047)
    hframe.GetXaxis().SetTitleSize(0.055)
    hframe.GetYaxis().SetLabelSize(0.045)
    hframe.GetXaxis().SetLabelSize(0.045)
    hframe.GetXaxis().SetLabelOffset(0.012)
    hframe.GetYaxis().SetTitleOffset(1.2)
    hframe.GetXaxis().SetTitleOffset(1.1)

    hframe.GetYaxis().SetTitle("95% CL on #sigma #times #bf{#it{#Beta}}(S#rightarrowHH#rightarrow bb#tau#tau) [fb]")
    hframe.GetXaxis().SetTitle("m_{S} [GeV]")

    hframe.SetStats(0)
    ROOT.gPad.SetTicky()
    hframe.Draw()

    gr2sigma.Draw("3same")
    gr1sigma.Draw("3same")
    grexp.Draw("Lsame")

    pt.Draw()
    pt2.Draw()

    canvas.Update()
    canvas.RedrawAxis("g")
    canvas.SetLogy()
    #legend.Draw()
    pt4.Draw()
    pt5.Draw()
    canvas.Update()
    
    smXS = 31.05 * 0.073

    if False:
       print('OBS GRAPH')
       grobs.Print()
       print('EXP GRAPH')
       grexp.Print()
       print('\nEXP +2sigma')
       gr2sigma.Print()
       print('\nEXP +1sigma')
       gr1sigma.Print()

    for e in ('png',):
        canvas.SaveAs(os.path.join(outdir, outfile + '.' + e))

def channel_label(chn):
    if chn == 'ETau':
        label = 'bb e#tau_{h}'
    elif chn == 'MuTau':
        label = 'bb #mu_{}#tau_{h}'
    elif chn == 'TauTau':
        label = 'bb #tau_{h}#tau_{h}'
    return label

def sel_masses(sel, masses):
    if 'boosted' in sel:
        m = [x for x in masses if x >= 320]
    else:
        m = masses
    return m

def plot(args, outdir):
    """main function"""
    ROOT.gROOT.SetBatch(ROOT.kTRUE)
    canvas = ROOT.TCanvas('canvas', 'canvas', 650, 500)
    canvas.SetFrameLineWidth(3)
    canvas.SetBottomMargin(0.15)
    canvas.SetRightMargin(0.05)
    canvas.SetLeftMargin(0.15)
    canvas.SetGridx()
    canvas.SetGridy()

    opt = dict(signal=args.signal, period=args.period,
               multigraph=ROOT.TMultiGraph(), canvas=canvas)
    comb_chn = 'bb #tau_{e}#tau_{h} + bb #tau_{#mu}#tau_{h} + bb #tau_{h}#tau_{h}'
    comb_cat = 'Combined categories'
    if args.mode == 'separate':

        for chn in args.channels:
            label = channel_label(chn)
            for sel in args.selections:
                indir = os.path.join('cards_' + args.tag + '_' + chn,
                                    sel + '_' + args.var, 'combined_out')
                masses_sel = sel_masses(sel, args.masses)
                outfile = '_'.join(('limit', args.tag, args.mode, chn, sel))
                create_limits_plot(indir, outfile, masses=masses_sel, labels=(label, sel), **opt)

    elif args.mode == 'sel_group':

        for chn in args.channels:
            label = channel_label(chn)
            indir = os.path.join('cards_' + args.tag + '_' + chn, 'comb_cat', 'combined_out')
            outfile = '_'.join(('limit', args.tag, args.mode, chn))
            create_limits_plot(indir, outfile, masses=args.masses, labels=(label, comb_cat), **opt)

    elif args.mode == 'chn_group':

        for sel in args.selections:
            indir = os.path.join('cards_' + args.tag + '_CombChn', sel + '_' + args.var, 'combined_out')
            masses_sel = sel_masses(sel, args.masses)
            outfile = '_'.join(('limit', args.tag, args.mode, sel))
            create_limits_plot(indir, outfile, masses=masses_sel, labels=(comb_chn, sel), **opt)

    elif args.mode == 'all_group':

       indir = os.path.join('cards_' + args.tag + '_All', 'combined_out')
       outfile = '_'.join(('limit', args.tag, args.mode))
       create_limits_plot(indir, outfile, masses=args.masses, labels=(comb_chn, comb_cat), **opt)

if __name__ == "__main__":
    usage = ('usage: %prog [options] datasetList\n %prog -h for help')
    parser = argparse.ArgumentParser(description=usage)
    parser.add_argument('-n', '--dry_run', action='store_true', help='dry run mode')
    parser.add_argument('-c', '--channels', nargs='+', help='channels')
    parser.add_argument('-l', '--selections', nargs='+', help='selections')
    parser.add_argument('-m', '--masses', type=int, nargs='+', help='masses')
    parser.add_argument('-p', '--period', choices=('UL16','UL17','UL18'),
                        help='data period')
    parser.add_argument('-s', '--signal', type=str, help='signal type')
    parser.add_argument('-t', '--tag', help='tag')
    parser.add_argument('--mode', choices=("separate", "sel_group", "chn_group", "all_group"), help='mode')
    parser.add_argument('-u', '--user', default='', help='EOS username to store the plots.')
    parser.add_argument('-v', '--var', help='variable to perform the maximum likelihood fit')
    FLAGS = parser.parse_args()

    user = os.environ['USER'] if FLAGS.user=='' else FLAGS.user
    outdir = os.path.join('/eos/user/', user[0], user, 'www/HH_Limits/')
    if not os.path.exists(outdir):
        os.makedirs(outdir)
        
    plot(FLAGS, outdir)
