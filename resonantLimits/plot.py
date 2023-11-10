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

def create_limits_plot(indirs, outfile, masses, labels, signal, period,
                       varfit, canvas):

    # covers situations without channel or selection overlays
    # when overlays are shown, sigma intervals are not displayed
    if not isinstance(indirs, (tuple, list)):
        indirs = [indirs]
    if not isinstance(masses[0], (tuple, list)):
        masses = [masses]

    ndirs = len(indirs)

    # Legend definition
    legend = ROOT.TLegend(0,0,0,0)
    legend.SetX1(0.18)
    legend.SetY1(0.7)
    legend.SetX2(0.35)
    legend.SetY2(0.89)
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
       ptext2.AddText('2016 - 36.3 fb^{-1} (13 TeV)')
    elif period == 'UL17':
       ptext2.AddText('2017 - 41.6 fb^{-1} (13 TeV)')
    elif period == 'UL18':
       ptext2.AddText('2018 - 59.7 fb^{-1} (13 TeV)')
    elif period == "All":
       ptext2.AddText('Run2 - 137.1 fb^{-1} (13 TeV)')

    ptexts = []
    vshift = 0.04
    for ipt in range(3):
       ptexts.append(ROOT.TPaveText(0.7, 0.85-ipt*vshift, 0.92, 0.88-ipt*vshift, 'brNDC'))
       ptexts[-1].SetTextAlign(12)
       ptexts[-1].SetFillColor(ROOT.kWhite)
       ptexts[-1].SetFillStyle(1001)
       ptexts[-1].SetTextFont(42)
       ptexts[-1].SetTextSize(0.03)
       ptexts[-1].SetBorderSize(0)
       ptexts[-1].SetTextAlign(32)
       if ipt == 2:
          ptexts[-1].AddText('ML fit: {}'.format(varfit))
       else:
          ptexts[-1].AddText(labels[ipt])

    # Outside frame
    frame_bounds = 220, 2030
    hframe = ROOT.TH1F('hframe_'+outfile, '',
                       100, frame_bounds[0], frame_bounds[1])
    hframe.SetMinimum(0.1)
    if period == 'UL16':
        hframe.SetMaximum(1e5)
        hframe.SetMinimum(5e-1)
    elif period == 'UL17':
        hframe.SetMaximum(1e5)
        hframe.SetMinimum(5e-1)
    elif period == 'UL18':
        hframe.SetMaximum(6e3)
        hframe.SetMinimum(1e-1)
    elif period == "All": # years combined
       hframe.SetMaximum(6e3)
       hframe.SetMinimum(1e-1)

    hframe.GetYaxis().SetTitleSize(0.047)
    hframe.GetXaxis().SetTitleSize(0.055)
    hframe.GetYaxis().SetLabelSize(0.045)
    hframe.GetXaxis().SetLabelSize(0.045)
    hframe.GetXaxis().SetLabelOffset(0.012)
    hframe.GetYaxis().SetTitleOffset(1.2)
    hframe.GetXaxis().SetTitleOffset(1.1)
    hframe.GetYaxis().SetTitle("95% CL on #sigma #times #bf{#it{#Beta}}(X#rightarrowHH#rightarrow bb#tau#tau) [fb]")
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

    # if plot_atlas: #format: [nominal, +1sigma, -1sigma]
    #     atlas_limits = {
    #         251: [338.343632385, 132.532814377, -94.548310224],
    #         260: [723.674375997, 283.470982053, -202.226916229],
    #         280: [840.891247788, 329.386082632, -234.98254127],
    #         300: [660.054656501, 258.550458455, -184.44872743],
    #         325: [471.95744985, 184.870773698, -131.885973636],
    #         350: [351.081800662, 137.522490937, -98.107922909],
    #         375: [216.738874272, 84.898931863, -60.566513925],
    #         400: [142.983566918, 56.008190252, -39.955989556],
    #         450: [67.6539757559, 26.5007848604, -18.905540035],
    #         500: [42.7054554468, 16.7281830005, -11.9338100776],
    #         550: [32.8902484269, 12.8834615826, -9.1910032108],
    #         600: [26.1798429167, 10.2549240759, -7.3158164445],
    #         700: [18.6692663031, 7.3129510021, -5.2170261625],
    #         800: [15.2306662884, 5.9660146515, -4.2561278632],
    #         900: [13.4018922009, 5.2496643098, -3.7450867701],
    #         1000: [12.193545018, 4.7763418128, -3.40742064207],
    #         1100: [13.41388346, 5.2543614139, -3.74843766307],
    #         1200: [13.7506650269, 5.3862823506, -3.84254946247],
    #         1400: [19.841377896, 7.7720796314, -5.5445664496],
    #         1600: [31.0459855988, 12.1610441358, -8.6756338723]
    #     }
            
    # Create graph objects
    agraph = {}
    for idx,indir in enumerate(indirs):
        agraph["exp"]  = ROOT.TGraph()
        agraph["obs"]  = ROOT.TGraph()
        agraph["sig1"] = ROOT.TGraphAsymmErrors()
        agraph["sig2"] = ROOT.TGraphAsymmErrors()
        ptsList = [] # (x, obs, exp, p2s, p1s, m1s, m2s)

        # if plot_atlas:
        #     agraph["atlas_nom"]  = ROOT.TGraph()
        #     agraph["atlas_sig1"] = ROOT.TGraphAsymmErrors()
        #     atlas_masses = sorted(list(atlas_limits.keys()))
        #     for ipt, mass in enumerate(atlas_masses):
        #         exp, ps1, ms1 = [x*0.073 for x in atlas_limits[mass]] #bbtt BR
        #         # ps1 = ps1 - exp
        #         ms1 = abs(ms1)
        #         agraph["atlas_nom"].SetPoint(ipt, mass, exp)
        #         agraph["atlas_sig1"].SetPoint(ipt, mass, exp)
        #         agraph["atlas_sig1"].SetPointError(ipt, 0, 0, ms1, ps1)
                
        for mass in masses[idx]:
            if mass < frame_bounds[0] or mass > frame_bounds[1]:
                continue
            
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
            xval, obs, exp, p2s, p1s, m1s, m2s = pt
            if exp > 0:
                agraph["exp"].SetPoint(ipt, xval, exp)
                agraph["obs"].SetPoint(ipt, xval, obs)
                agraph["sig1"].SetPoint(ipt, xval, exp)
                agraph["sig1"].SetPointError(ipt, 0, 0, m1s, p1s)
                agraph["sig2"].SetPoint(ipt, xval, exp)
                agraph["sig2"].SetPointError(ipt, 0, 0, m2s, p2s)
         
        # set styles
        agraph["exp"].SetMarkerStyle(24)
        agraph["exp"].SetMarkerColor(4)
        agraph["exp"].SetMarkerSize(0.8)
        agraph["exp"].SetLineColor(idx+1)
        agraph["exp"].SetLineWidth(3)
        agraph["exp"].SetLineStyle(2)
        agraph["exp"].SetFillColor(0)
         
        agraph["obs"].SetLineColor(1)
        agraph["obs"].SetLineWidth(3)
        agraph["obs"].SetMarkerColor(1)
        agraph["obs"].SetMarkerStyle(20)
        agraph["obs"].SetFillStyle(0)

        if ndirs==1:
            agraph["sig1"].SetMarkerStyle(0)
            agraph["sig1"].SetMarkerColor(3)
            agraph["sig1"].SetFillColor(ROOT.kGreen+1)
            agraph["sig1"].SetLineColor(ROOT.kGreen+1)
            agraph["sig1"].SetFillStyle(1001)
         
            agraph["sig2"].SetMarkerStyle(0)
            agraph["sig2"].SetMarkerColor(5)
            agraph["sig2"].SetFillColor(ROOT.kOrange)
            agraph["sig2"].SetLineColor(ROOT.kOrange)
            agraph["sig2"].SetFillStyle(1001)

        # if plot_atlas:
        #     agraph["atlas_nom"].SetLineColor(39)
        #     agraph["atlas_nom"].SetLineWidth(3)
        #     agraph["atlas_nom"].SetLineStyle(2)

        #     agraph["atlas_sig1"].SetMarkerStyle(0)
        #     agraph["atlas_sig1"].SetMarkerColor(3)
        #     agraph["atlas_sig1"].SetFillColor(18)
        #     agraph["atlas_sig1"].SetLineColor(18)
        #     agraph["atlas_sig1"].SetFillStyle(1001)

            
        if ndirs==1:
           leg_labels = ('Observed', 'Median expected')
        else:
           leg_labels = ('Obs. ' + labels[2+idx][:20],
                         'Exp. ' + labels[2+idx][:20])
        legend.AddEntry(agraph["exp"], leg_labels[1], 'l')
        if ndirs==1:
            legend.AddEntry(agraph["sig1"], '68% expected', 'f')
            legend.AddEntry(agraph["sig2"], '95% expected', 'f')
            agraph["sig2"].Draw('3same')
            agraph["sig1"].Draw('3same')
        # if plot_atlas:
        #     legend.AddEntry(agraph["atlas_nom"], "ATLAS Full Run2", 'l')
        #     legend.AddEntry(agraph["atlas_sig1"], 'ATLAS: 68% expected', 'f')
        #     agraph["atlas_sig1"].Draw('3same')
        #     agraph["atlas_nom"].Draw('Lsame')
        agraph["exp"].Draw('Lsame')

    ptext.Draw()
    ptext2.Draw()

    canvas.Update()
    canvas.RedrawAxis('g')
    canvas.SetLogy()

    legend.Draw()
    for i in range(len(ptexts)):
       ptexts[i].Draw()
    canvas.Update()

    for e in ('png', 'pdf'):
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

    # opt = dict(signal=args.signal, period=args.period, canvas=canvas, varfit=args.var,
    #            plot_atlas=args.atlas)
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

    elif args.mode == 'sel_years':

        for chn in args.channels:
            label = channel_label(chn)
            indir = os.path.join(base, 'cards_Years_' + args.var + '_CombCat', chn, 'combined_out')
            outfile = '_'.join(('limit', args.mode, args.var, chn))
            create_limits_plot(indir, outfile, masses=args.masses, labels=(label, comb_cat), **opt)

    elif args.mode == 'chn_group':

        for sel in args.selections:
            indir = os.path.join(base, 'cards_' + args.tag + '_CombChn',
                                 sel + '_' + args.var, 'combined_out')
            masses_sel = sel_masses(sel, args.masses)
            outfile = '_'.join(('limit', args.tag, args.mode, args.var, sel))
            create_limits_plot(indir, outfile, masses=masses_sel, labels=(comb_chn, sel), **opt)

    elif args.mode == 'chn_years':

        for sel in args.selections:
            indir = os.path.join(base, 'cards_Years_' + args.var + '_CombChn',
                                 sel + '_' + args.var, 'combined_out')
            masses_sel = sel_masses(sel, args.masses)
            outfile = '_'.join(('limit', args.tag, args.mode, args.var, sel))
            create_limits_plot(indir, outfile, masses=masses_sel, labels=(comb_chn, sel), **opt)

    elif args.mode == 'all_group':

       indir = os.path.join(base, 'cards_' + args.tag + '_All', 'combined_out')
       outfile = '_'.join(('limit', args.tag, args.mode, args.var))
       create_limits_plot(indir, outfile, masses=args.masses, labels=(comb_chn, comb_cat), **opt)

    elif args.mode == 'all_years':

       indir = os.path.join(base, 'cards_Years_' + args.var + '_All', 'combined_out')
       outfile = '_'.join(('limit', args.mode, args.var))
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

    elif args.mode == 'overlay_channels_years':

        indirs = []
        for chn in args.channels:
            indirs.append(os.path.join(base, 'cards_Years_' + args.var + '_CombCat',
                                       chn, 'combined_out'))
        label = channel_label(chn)
        outfile = '_'.join(('limit', args.mode, args.var))
        chn_labels = tuple([channel_label(x) for x in args.channels])
        create_limits_plot(indirs, outfile,
                           masses=len(args.channels)*[args.masses],
                           labels=('Overlay channels', comb_cat)+chn_labels,
                           **opt)

    elif args.mode == 'overlay_selections_years':

        indirs = []
        list_masses = []
        for sel in args.selections:
            indirs.append(os.path.join(base, 'cards_Years_' + args.var + '_CombChn',
                                       sel + '_' + args.var, 'combined_out'))
            list_masses.append(sel_masses(sel, args.masses))
        outfile = '_'.join(('limit', args.mode, args.var))
        create_limits_plot(indirs, outfile, masses=list_masses,
                           labels=(comb_chn, 'Overlay categories')+tuple(args.selections),
                           **opt)
        
if __name__ == "__main__":
    usage = ('usage: %prog [options] datasetList\n %prog -h for help')
    parser = argparse.ArgumentParser(description=usage)
    parser.add_argument('-n', '--dry_run', action='store_true', help='dry run mode')
    parser.add_argument('-c', '--channels', nargs='+', help='channels')
    parser.add_argument('-l', '--selections', nargs='+', help='selections')
    parser.add_argument('-m', '--masses', type=int, nargs='+', help='masses')
    parser.add_argument('-p', '--period', choices=('UL16','UL17','UL18','All'),
                        help='data period')
    parser.add_argument('-s', '--signal', type=str, help='signal type')
    parser.add_argument('-t', '--tag', help='tag')
    parser.add_argument('-b', '--basedir', help='Base directory')
    parser.add_argument('--mode', choices=('separate', 'sel_group', 'chn_group', 'all_group', 'all_years',
                                           'overlay_channels', 'overlay_selections',
                                           'overlay_channels_years', 'overlay_selections_years',), help='mode')
    parser.add_argument('-u', '--user', default='', help='EOS username to store the plots.')
    parser.add_argument('-v', '--var', help='variable to perform the maximum likelihood fit')
    #parser.add_argument('--atlas', action='store_true', help='overlay ATLAS full Run2 result')
    FLAGS = parser.parse_args()

    user = os.environ['USER'] if FLAGS.user=='' else FLAGS.user
    outdir = os.path.join('/eos/home-' + user[0] + '/',  user, 'www/HH_Limits/')
    if 'years' not in FLAGS.mode:
        outdir = os.path.join(outdir, FLAGS.tag)
    else:
        outdir = os.path.join(outdir, FLAGS.var + '_' + FLAGS.period + '_Years')
    if not os.path.exists(outdir):
        os.makedirs(outdir)
        
    plot(FLAGS, outdir)
