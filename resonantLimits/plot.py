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
        search = 'Expected {}%: r <'.format(CL)
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

def create_limits_plot(indirs, outfile, masses, labels, signal, period, varfit, canvas):
    # covers situations without channel or selection overlays
    # when overlays are shown, sigma intervals are not displayed
    if not isinstance(indirs, (tuple, list)):
        indirs = [indirs]
    if not isinstance(masses[0], (tuple, list)):
        masses = [masses]

    ndirs = len(indirs)

    # Legend definition
    legend = ROOT.TLegend(0,0,0,0)
    legend.SetX1(0.73)
    legend.SetY1(0.18)
    legend.SetX2(0.92)
    legend.SetY2(0.35)
    #legend.SetFillColor(ROOT.kWhite)
    legend.SetBorderSize(0)
    # if ndirs == 1:
    #    legend.SetHeader('95% CL upper limits')

    # Canvas text
    ptext = ROOT.TPaveText(0.1663218-0.02, 0.886316, 0.3045977-0.02, 0.978947, 'brNDC')
    ptext.SetBorderSize(0)
    ptext.SetTextAlign(12)
    ptext.SetTextFont(62)
    ptext.SetTextSize(0.05)
    ptext.SetFillColor(0)
    ptext.SetFillStyle(0)
    ptext.AddText('CMS #font[52]{Internal}')

    ptext2 = ROOT.TPaveText(0.74, 0.91, 0.85, 0.95, 'brNDC')
    ptext2.SetBorderSize(0)
    ptext2.SetFillColor(0)
    ptext2.SetTextSize(0.040)
    ptext2.SetTextFont(42)
    ptext2.SetFillStyle(0)
    if period == 'UL16':
       ptext2.AddText('2016 - 35.9 fb^{-1} (13 TeV)')
    elif period == 'UL17':
       ptext2.AddText('2017 - 41.6 fb^{-1} (13 TeV)')
    elif period == 'UL18':
       ptext2.AddText('2018 - 59.7 fb^{-1} (13 TeV)')
    else:
       ptext2.AddText('Run2 - 137.1 fb^{-1} (13 TeV)')

    ptexts = []
    vshift = 0.06
    for ipt in range(3):
       ptexts.append(ROOT.TPaveText(0.62, 0.8-ipt*vshift, 0.92, 0.88-ipt*vshift, 'brNDC'))
       ptexts[-1].SetTextAlign(12)
       ptexts[-1].SetFillColor(ROOT.kWhite)
       ptexts[-1].SetFillStyle(1001)
       ptexts[-1].SetTextFont(42)
       ptexts[-1].SetTextSize(0.04)
       ptexts[-1].SetBorderSize(0)
       ptexts[-1].SetTextAlign(32)
       if ipt == 2:
          ptexts[-1].AddText('Fit: {}'.format(varfit))
       else:
          ptexts[-1].AddText(labels[ipt])

    # Outside frame
    hframe = ROOT.TH1F('hframe_'+outfile, '', 100, 250, 3100)
    hframe.SetMinimum(0.1)
    if period == 'UL16':
        hframe.SetMaximum(1e5)
        hframe.SetMinimum(1e-1)
    elif period == 'UL17':
        hframe.SetMaximum(1e5)
        hframe.SetMinimum(1e-1)
    elif period == 'UL18':
        hframe.SetMaximum(1e3)
        hframe.SetMinimum(1e-1)
    else:
       hframe.SetMaximum(1e4)
       hframe.SetMinimum(1e-1)

    hframe.GetYaxis().SetTitleSize(0.047)
    hframe.GetXaxis().SetTitleSize(0.055)
    hframe.GetYaxis().SetLabelSize(0.045)
    hframe.GetXaxis().SetLabelSize(0.045)
    hframe.GetXaxis().SetLabelOffset(0.012)
    hframe.GetYaxis().SetTitleOffset(1.2)
    hframe.GetXaxis().SetTitleOffset(1.1)

    hframe.GetYaxis().SetTitle("95% CL on #sigma #times #bf{#it{#Beta}}(S#rightarrowHH#rightarrow bb#tau#tau) [fb]")
    hframe.GetXaxis().SetTitle("m_{X} [GeV]")

    hframe.SetStats(0)
    ROOT.gPad.SetTicky()
    hframe.Draw()

    # Theory lines
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

    # Create graph objects
    grexp, grobs, gr2sigma, gr1sigma = ([] for _ in range(4))
    for idx,indir in enumerate(indirs):
        grexp.append(ROOT.TGraph())
        grobs.append(ROOT.TGraph())
        gr2sigma.append(ROOT.TGraphAsymmErrors())
        gr1sigma.append(ROOT.TGraphAsymmErrors())
       
        ptsList = [] # (x, obs, exp, p2s, p1s, m1s, m2s)

        for mass in masses[idx]:
            fName = os.path.join(indir, 'comb.{}_{}_{}.log'.format(signal, varfit, mass))
         
            exp   = 10.*parseFile(fName)            
            obs   = exp #parseFile(fName, exp=False)
            m1s_t = 10.*parseFile(fName, CL='16.0') 
            p1s_t = 10.*parseFile(fName, CL='84.0') 
            m2s_t = 10.*parseFile(fName, CL=' 2.5') 
            p2s_t = 10.*parseFile(fName, CL='97.5') 
            # because the other code wants +/ sigma vars as deviations,
            # without sign, from the centeal exp value...
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
                grexp[-1].SetPoint(ipt, xval, exp)
                grobs[-1].SetPoint(ipt, xval, obs)
                gr1sigma[-1].SetPoint(ipt, xval, exp)
                gr2sigma[-1].SetPoint(ipt, xval, exp)
                gr1sigma[-1].SetPointError(ipt, 0, 0, m1s, p1s)
                gr2sigma[-1].SetPointError(ipt, 0, 0, m2s, p2s)       
         
        # set styles
        grexp[-1].SetMarkerStyle(24)
        grexp[-1].SetMarkerColor(4)
        grexp[-1].SetMarkerSize(0.8)
        grexp[-1].SetLineColor(idx+1)
        grexp[-1].SetLineWidth(3)
        grexp[-1].SetLineStyle(2)
        grexp[-1].SetFillColor(0)
         
        grobs[-1].SetLineColor(1)
        grobs[-1].SetLineWidth(3)
        grobs[-1].SetMarkerColor(1)
        grobs[-1].SetMarkerStyle(20)
        grobs[-1].SetFillStyle(0)

        if ndirs==1:
            gr1sigma[-1].SetMarkerStyle(0)
            gr1sigma[-1].SetMarkerColor(3)
            gr1sigma[-1].SetFillColor(ROOT.kGreen+1)
            gr1sigma[-1].SetLineColor(ROOT.kGreen+1)
            gr1sigma[-1].SetFillStyle(1001)
         
            gr2sigma[-1].SetMarkerStyle(0)
            gr2sigma[-1].SetMarkerColor(5)
            gr2sigma[-1].SetFillColor(ROOT.kOrange)
            gr2sigma[-1].SetLineColor(ROOT.kOrange)
            gr2sigma[-1].SetFillStyle(1001)

        if ndirs==1:
           leg_labels = ('Observed', 'Median expected')
        else:
           leg_labels = ('Obs. ' + labels[2+idx][:20],
                         'Exp. ' + labels[2+idx][:20])
        #legend.AddEntry(grobs[-1], leg_labels[0], 'l')
        legend.AddEntry(grexp[-1], leg_labels[1], 'l')
        if ndirs==1:
            legend.AddEntry(gr1sigma[-1], '68% expected', 'f')
            legend.AddEntry(gr2sigma[-1], '95% expected', 'f')
            gr2sigma[-1].Draw('3same')
            gr1sigma[-1].Draw('3same')
        grexp[-1].Draw('Lsame')

    ptext.Draw()
    ptext2.Draw()

    canvas.Update()
    canvas.RedrawAxis('g')
    canvas.SetLogy()

    legend.Draw()
    for i in range(len(ptexts)):
       ptexts[i].Draw()
    canvas.Update()

    for e in ('png','pdf'):
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
    base = os.path.join(args.basedir, 'resonantLimits')
    
    ROOT.gROOT.SetBatch(ROOT.kTRUE)
    canvas = ROOT.TCanvas('canvas', 'canvas', 650, 500)
    canvas.SetFrameLineWidth(3)
    canvas.SetBottomMargin(0.15)
    canvas.SetRightMargin(0.05)
    canvas.SetLeftMargin(0.15)
    canvas.SetGridx()
    canvas.SetGridy()

    opt = dict(signal=args.signal, period=args.period, canvas=canvas, varfit=args.var)
    comb_chn = 'bb #tau_{e}#tau_{h} + bb #tau_{#mu}#tau_{h} + bb #tau_{h}#tau_{h}'
    comb_cat = 'Combined categories'
    if args.mode == 'separate':

        for chn in args.channels:
            label = channel_label(chn)
            for sel in args.selections:
                indir = os.path.join(base, 'cards_' + args.tag + '_' + chn,
                                     sel + '_' + args.var, 'combined_out')
                masses_sel = sel_masses(sel, args.masses)
                outfile = '_'.join(('limit', args.tag, args.mode, args.var, chn, sel))
                create_limits_plot(indir, outfile, masses=masses_sel, labels=(label, sel), **opt)

    elif args.mode == 'sel_group':

        for chn in args.channels:
            label = channel_label(chn)
            indir = os.path.join(base, 'cards_' + args.tag + '_' + chn,
                                 'comb_cat', 'AllCategories/', 'combined_out')
            outfile = '_'.join(('limit', args.tag, args.mode, args.var, chn, 'AllCategories'))
            create_limits_plot(indir, outfile, masses=args.masses, labels=(label, comb_cat), **opt)

            for sel in args.selections:
                masses_sel = sel_masses(sel, args.masses)
                indir = os.path.join(base, 'cards_' + args.tag + '_' + chn,
                                    'comb_cat', '{}_{}'.format(sel, args.var), 'combined_out')
                outfile = '_'.join(('limit', args.tag, args.mode, args.var, chn, sel))
                create_limits_plot(indir, outfile, masses=masses_sel, labels=(label, sel), **opt)

    elif args.mode == 'chn_group':

        for sel in args.selections:
            indir = os.path.join(base, 'cards_' + args.tag + '_CombChn',
                                 sel + '_' + args.var, 'combined_out')
            masses_sel = sel_masses(sel, args.masses)
            outfile = '_'.join(('limit', args.tag, args.mode, args.var, sel))
            create_limits_plot(indir, outfile, masses=masses_sel, labels=(comb_chn, sel), **opt)

    elif args.mode == 'all_group':

       indir = os.path.join(base, 'cards_' + args.tag + '_All', 'combined_out')
       outfile = '_'.join(('limit', args.tag, args.mode, args.var))
       create_limits_plot(indir, outfile, masses=args.masses, labels=(comb_chn, comb_cat), **opt)

    elif args.mode == 'overlay_channels':

        indirs = []
        for chn in args.channels:
            indirs.append(os.path.join(base, 'cards_' + args.tag + '_' + chn,
                                       'comb_cat', 'AllCategories', 'combined_out'))
        label = channel_label(chn)
        outfile = '_'.join(('limit', args.tag, args.mode, args.var))
        chn_labels = tuple([channel_label(x) for x in args.channels])
        create_limits_plot(indirs, outfile,
                           masses=len(args.channels)*[args.masses],
                           labels=('Overlay channels', comb_cat)+chn_labels,
                           **opt)

    elif args.mode == 'overlay_selections':

        indirs = []
        list_masses = []
        for sel in args.selections:
            indirs.append(os.path.join(base, 'cards_' + args.tag + '_CombChn',
                                       sel + '_' + args.var, 'combined_out'))
            list_masses.append(sel_masses(sel, args.masses))
        outfile = '_'.join(('limit', args.tag, args.mode, args.var))
        create_limits_plot(indirs, outfile, masses=list_masses,
                           labels=(comb_chn,'Overlay categories')+tuple(args.selections),
                           **opt)
    
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
    parser.add_argument('-b', '--basedir', help='Base directory')
    parser.add_argument('--mode', help='mode',
                        choices=('separate', 'sel_group', 'chn_group', 'all_group',
                                 'overlay_channels', 'overlay_selections'))
    parser.add_argument('-u', '--user', default='', help='EOS username to store the plots.')
    parser.add_argument('-v', '--var', help='variable to perform the maximum likelihood fit')
    FLAGS = parser.parse_args()

    user = os.environ['USER'] if FLAGS.user=='' else FLAGS.user
    outdir = os.path.join('/eos/user/', user[0], user, 'www/HH_Limits/', FLAGS.tag)
    if not os.path.exists(outdir):
        os.makedirs(outdir)
        
    plot(FLAGS, outdir)
