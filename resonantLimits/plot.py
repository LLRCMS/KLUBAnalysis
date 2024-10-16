# coding: utf-8                                                                                                                             

_all_ = [ 'plot' ]

import os
import argparse
import re
import numpy as np
from scipy.interpolate import make_interp_spline

import matplotlib; import matplotlib.pyplot as plt
import matplotlib.colors as colors
from matplotlib.ticker import ScalarFormatter
import mplhep as hep
plt.style.use(hep.style.ROOT)
from matplotlib.legend import Legend

import matplotlib as mpl
mpl.rcParams['axes.linewidth'] = 3 # frame thickness
mpl.rcParams['pdf.fonttype'] = 42
mpl.rcParams['ps.fonttype'] = 42  

#format: [nominal, +1sigma, -1sigma]
atlas_limits = {
    251: [338.343632385, 132.532814377, -94.548310224],
    260: [723.674375997, 283.470982053, -202.226916229],
    280: [840.891247788, 329.386082632, -234.98254127],
    300: [660.054656501, 258.550458455, -184.44872743],
    325: [471.95744985, 184.870773698, -131.885973636],
    350: [351.081800662, 137.522490937, -98.107922909],
    375: [216.738874272, 84.898931863, -60.566513925],
    400: [142.983566918, 56.008190252, -39.955989556],
    450: [67.6539757559, 26.5007848604, -18.905540035],
    500: [42.7054554468, 16.7281830005, -11.9338100776],
    550: [32.8902484269, 12.8834615826, -9.1910032108],
    600: [26.1798429167, 10.2549240759, -7.3158164445],
    700: [18.6692663031, 7.3129510021, -5.2170261625],
    800: [15.2306662884, 5.9660146515, -4.2561278632],
    900: [13.4018922009, 5.2496643098, -3.7450867701],
    1000: [12.193545018, 4.7763418128, -3.40742064207],
    1100: [13.41388346, 5.2543614139, -3.74843766307],
    1200: [13.7506650269, 5.3862823506, -3.84254946247],
    1400: [19.841377896, 7.7720796314, -5.5445664496],
    1600: [31.0459855988, 12.1610441358, -8.6756338723]
}

def parse_file(filename):
    ret = []
    cls = ('2.5', '16.0', '50.0', '84.0', '97.5')
    with open(filename) as f:
        for cl in cls:
            regexp = re.compile('.*Expected.+{}%: r < (.*)\n$'.format(cl))
            for line in f:
                mat = regexp.match(line)
                if mat is None:
                    continue
                ret.append(float(mat.group(1)))
                break

        if len(ret) != len(cls):
            mes = 'Did not find any expected in file {}.'
            raise RuntimeError(mes.format(filename))

        mat = re.match('.*Observed.+{}%: r < (.*)$', line)
        if mat is None:
            ret.append('<empty>')
        else:
            ret.append(float(mat.group(1)))

    return ret

def replace_some_ylabels(ax):
    ylabels = [item.get_text() for item in ax.get_yticklabels()]
    entered = False
    for i, ylab in enumerate(ylabels):
        if ylab == "$\\mathdefault{10^{0}}$":
            ylabels[i] = "$\\mathdefault{1}$"
            entered = True
        if ylab == "$\\mathdefault{10^{1}}$":
            ylabels[i] = "$\\mathdefault{10}$"
            entered = True
    if entered:
        ax.set_yticks(ax.get_yticks()) # avoids warning
        ax.set_yticklabels(ylabels)

def create_limits_plot(indirs, outfile, masses, labels, signal, period,
                       varfit, atlas, interp, nobands, legends=['Observed']):

    # covers situations without channel or selection overlays
    # when overlays are shown, sigma intervals are not displayed
    if not isinstance(indirs, (tuple, list)):
        indirs = [indirs]
    if not isinstance(masses[0], (tuple, list)):
        masses = [masses]

    ndirs = len(indirs)

    lumi = {'UL16'    : '16.8',
            'UL16APV' : '19.5',
            'UL17'    : '41.6',
            'UL18'    : '59.7',
            'All'     :  '137.1',
            'Empty'   : ''}[period]

    sig = {"GGF_Radion":   'Radion',
           "GGF_Graviton": 'Graviton',
           }[signal]


    fig, ax = plt.subplots(ncols=1, nrows=1, figsize=(20, 14))
    plt.margins(x=0.04, y=0.16)
    ax.title.set_size(100)

    ax.set_xlabel(r"$m_{X}\;[GeV]$", fontsize=26)
    ax.set_ylabel(r"95% CL on $\sigma \times \mathcal{B}(pp\rightarrow X\rightarrow HH)$ [fb]", fontsize=26)
    ax.set_yscale('log')

    for label in ax.xaxis.get_ticklabels()+ax.yaxis.get_ticklabels():
        label.set_color('black')
        label.set_rotation(0)
        label.set_fontsize(26)
        
    # replace some y labels by custom strings
    replace_some_ylabels(ax)

    hep_opt = dict(ax=ax)
    hep.cms.text(' Preliminary', fontsize=32, **hep_opt)
    if period == "Empty":
        hep.cms.lumitext(r"{} (13 TeV)".format(sig, lumi), fontsize=30, **hep_opt)
    else:
        hep.cms.lumitext(r"{} | {} $fb^{{-1}}$ (13 TeV)".format(sig, lumi), fontsize=30, **hep_opt)
        
    xtext = 0.025
    for ilab, lab in enumerate(labels):
        fig.text(xtext, 0.95-0.04*ilab, lab, horizontalalignment='left',
                 verticalalignment='center', transform=ax.transAxes)
            
    # Create graph objects
    vals = {k: [] for k in ("exp", "obs", "m1s", "m2s", "p1s", "p2s")}
    if atlas:
        atlas_masses = list(atlas_limits.keys())
        vals.update({k: [] for k in ("atlas_nom", "atlas_p1s", "atlas_m1s")})

    for mass in atlas_masses:
        exp, ps1, ms1 = [x for x in atlas_limits[mass]]
        vals['atlas_nom'].append(exp)
        vals['atlas_p1s'].append(exp+ps1)
        vals['atlas_m1s'].append(exp+ms1)

    linecolors = ('red', 'green', 'blue', 'brown', 'orange', 'purple')
    for idx, indir in enumerate(indirs):
        vals['exp'] = []
        vals['obs'] = []
        vals['m1s'] = []
        vals['m2s'] = []
        vals['p1s'] = []
        vals['p2s'] = []
        
        for mass in masses[idx]:
            if '{}' in indir:
                fName = os.path.join(indir.format(mass), 'comb.{}_{}.log'.format(signal, varfit.format(mass)))
            else:
                fName = os.path.join(indir, 'comb.{}_{}.log'.format(signal, varfit.format(mass)))
                
            m2s, m1s, exp, p1s, p2s, _ = parse_file(fName)
            scale_ss = 10.
            br_bbtt = 0.073
            vals['exp'].append(scale_ss * exp / br_bbtt)
            vals['obs'].append(scale_ss * exp / br_bbtt)
            vals['m1s'].append(scale_ss * m1s / br_bbtt)
            vals['m2s'].append(scale_ss * m2s / br_bbtt)
            vals['p1s'].append(scale_ss * p1s / br_bbtt)
            vals['p2s'].append(scale_ss * p2s / br_bbtt)            

        alpha = 0.8
        if "Run2" in legends[idx]:
            fplot = "--o"
        else:
            fplot = "--o"
 
        if not nobands or "Run2" in indir:
            if interp:
                spline_degree = 2
                spline_m1s = make_interp_spline(masses[idx], vals['m1s'], k=spline_degree)
                spline_m2s = make_interp_spline(masses[idx], vals['m2s'], k=spline_degree)
                spline_p1s = make_interp_spline(masses[idx], vals['p1s'], k=spline_degree)
                spline_p2s = make_interp_spline(masses[idx], vals['p2s'], k=spline_degree)
                masses_interp = np.linspace(masses[idx][0], masses[idx][-1], 500)
                m1s_interp = spline_m1s(masses_interp)
                m2s_interp = spline_m2s(masses_interp)
                p1s_interp = spline_p1s(masses_interp)
                p2s_interp = spline_p2s(masses_interp)
                ax.fill_between(masses_interp, m1s_interp, p1s_interp, alpha=alpha,
                                color='#FFDF7Fff', label="68% exp.")
                ax.fill_between(masses_interp, m2s_interp, m1s_interp, alpha=alpha,
                                color='#85D1FBff', label="95% exp.")
                ax.fill_between(masses_interp, p1s_interp, p2s_interp, alpha=alpha, color='#85D1FBff')
     
            else:
                ax.fill_between(masses[idx], vals['m1s'], vals['p1s'], alpha=alpha,
                                color='#FFDF7Fff', label="68% exp.")
                ax.fill_between(masses[idx], vals['m2s'], vals['m1s'], alpha=alpha,
                                color='#85D1FBff', label="95% exp.")
                ax.fill_between(masses[idx], vals['p1s'], vals['p2s'], alpha=alpha,
                                color='#85D1FBff')

        black_lines = ('Observed', 'Run2')
        ax.plot(masses[idx], vals['exp'], fplot,
                color='black' if legends[idx] in black_lines else linecolors[idx],
                label=legends[idx], linewidth=2)

    legend_opt = dict(facecolor="black", edgecolor="white", framealpha=1, prop={'size': 26}, ncols=1)
    if atlas:
        column1 = {"atlas_bands": "ATLAS exp."}
        atlas_line, = ax.plot(atlas_masses, vals['atlas_nom'], '-o', color='gray', linewidth=2)
        lines = [atlas_line]
        if not nobands:
            atlas_bands = ax.fill_between(atlas_masses, vals['atlas_m1s'], vals['atlas_p1s'], alpha=alpha, color='gray')
            lines.append(atlas_bands)
            column1.update({"atlas_exp": "ATLAS 68% exp."})
        legend1 = Legend(ax, lines, column1.values(), bbox_to_anchor=(0.995, .995), **legend_opt)

    handles, labels = plt.gca().get_legend_handles_labels()
    ax.add_artist(legend1)
    plt.legend(loc="upper right", bbox_to_anchor=(0.8, .995), **legend_opt)
 
    for ext in ('.png', '.pdf'):
        plt.savefig(outfile + ext, dpi=600)

def channel_label(chn):
    label = {"ETau":   r'$bb\: e\tau$',
             "MuTau":  r'$bb\: \mu\tau$',
             "TauTau": r'$bb\: \tau\tau$',
             "MuMu":   r'$bb\: \mu\mu$',
             "All":    r'$bb\: \tau_{e}\tau_{h} + bb\: \tau_{\mu}\tau_{h} + bb\: \tau_{h}\tau_{h}$'}
    return label[chn]

def sel_masses(sel, masses):
    if 'boosted' in sel:
        m = [x for x in masses if x >= 320]
    else:
        m = masses
    return m

def plot(args, outdir):
    """main function"""
    base = os.path.join(args.basedir, 'resonantLimits')

    opt = dict(signal=args.signal, varfit=args.var, atlas=args.atlas,
               interp=args.interpolate, nobands=args.nobands)
    comb_chn = channel_label('All')
    comb_cat = 'Combined categories'

    if args.mode == 'separate':

        for chn in args.channels:
            label = channel_label(chn)
            for sel in args.selections:
                indir = os.path.join(base, 'cards_' + args.tag, chn, sel + '_' + args.var, 'combined_out')
                masses_sel = sel_masses(sel, args.masses)
                outfile = os.path.join(outdir, '_'.join(('limit_pDNN', args.mode, chn, sel)))
                create_limits_plot(indir, outfile, masses=masses_sel, labels=(label, sel), period=args.period, **opt)

    elif args.mode == 'sel_group':

        for chn in args.channels:
            label = channel_label(chn)
            indir = os.path.join(base, 'cards_' + args.tag, chn,
                                 'comb_cat', 'AllCategories/', 'combined_out')
            outfile = os.path.join(outdir,'_'.join(('limit_pDNN', args.mode, chn, 'AllCategories')))
            create_limits_plot(indir, outfile, masses=args.masses, labels=(label, comb_cat), period=args.period, **opt)

            # for sel in args.selections:
            #     masses_sel = sel_masses(sel, args.masses)
            #     indir = os.path.join(base, 'cards_' + args.tag, chn,
            #                         'comb_cat', '{}_{}'.format(sel, args.var, 'combined_out')
            #     outfile = os.path.join(outdir, '_'.join(('limit_pDNN', args.mode, chn, sel)))
            #     create_limits_plot(indir, outfile, masses=masses_sel, labels=(label, sel), period=args.period, **opt)

    elif args.mode == 'sel_years':

        for chn in args.channels:
            label = channel_label(chn)
            indir = os.path.join(base, 'cards_Years_' + args.var + '_CombCat', chn, 'combined_out')
            outfile = os.path.join(outdir,'_'.join(('limit', args.mode, args.var, chn)))
            create_limits_plot(indir, outfile, masses=args.masses, labels=(label, comb_cat), period=args.period, **opt)

    elif args.mode == 'chn_group':

        for sel in args.selections:
            indir = os.path.join(base, 'cards_' + args.tag + '_CombChn', sel + '_' + args.var, 'combined_out')
            masses_sel = sel_masses(sel, args.masses)
            outfile = os.path.join(outdir,'_'.join(('limit_pDNN', args.mode, sel)))
            create_limits_plot(indir, outfile, masses=masses_sel, labels=(comb_chn, sel), period=args.period, **opt)

    elif args.mode == 'chn_years':

        for sel in args.selections:
            indir = os.path.join(base, 'cards_Years_' + args.var + '_CombChn',
                                 sel + '_' + args.var, 'combined_out')
            masses_sel = sel_masses(sel, args.masses)
            outfile = os.path.join(outdir,'_'.join(('limit', args.tag, args.mode, args.var, sel)))
            create_limits_plot(indir, outfile, masses=masses_sel, labels=(comb_chn, sel), period=args.period, **opt)

    elif args.mode == 'all_group':

       indir = os.path.join(base, 'cards_' + args.tag + '_All', 'combined_out')
       outfile = os.path.join(outdir, '_'.join(('limit_pDNN', args.mode)))
       create_limits_plot(indir, outfile, masses=args.masses, labels=(comb_chn, comb_cat), period=args.period, **opt)

    elif args.mode == 'all_years':

       indir = os.path.join(base, 'cards_Years_' + args.var + '_All', 'combined_out')
       outfile = os.path.join(outdir,'_'.join(('limit', args.mode, args.var)))
       create_limits_plot(indir, outfile, masses=args.masses, labels=(comb_chn, comb_cat), period=args.period, **opt)

    elif args.mode == 'overlay_channels':

        indirs = []
        for chn in args.channels:
            indirs.append(os.path.join(base, 'cards_' + args.tag + '_' + chn,
                                       'comb_cat', 'AllCategories', 'combined_out'))
        label = channel_label(chn)
        outfile = os.path.join(outdir,'_'.join(('limit', args.tag, args.mode, args.var)))
        chn_labels = tuple([channel_label(x) for x in args.channels])
        create_limits_plot(indirs, outfile,
                           masses=len(args.channels)*[args.masses],
                           labels=('Overlay channels', comb_cat)+chn_labels,
                           period=args.period, **opt)

    elif args.mode == 'overlay_selections':

        indirs = []
        list_masses = []
        for sel in args.selections:
            indirs.append(os.path.join(base, 'cards_' + args.tag + '_CombChn',
                                       sel + '_' + args.var, 'combined_out'))
            list_masses.append(sel_masses(sel, args.masses))
        outfile = os.path.join(outdir,'_'.join(('limit', args.tag, args.mode, args.var)))
        create_limits_plot(indirs, outfile, masses=list_masses,
                           labels=(comb_chn,'Overlay categories')+tuple(args.selections),
                           period=args.period, **opt)

    elif args.mode == 'overlay_channels_years':

        indirs = []
        for chn in args.channels:
            indirs.append(os.path.join(base, 'cards_Years_' + args.var + '_CombCat',
                                       chn, 'combined_out'))
        label = channel_label(chn)
        outfile = os.path.join(outdir,'_'.join(('limit', args.mode, args.var)))
        chn_labels = tuple([channel_label(x) for x in args.channels])
        create_limits_plot(indirs, outfile,
                           masses=len(args.channels)*[args.masses],
                           labels=('Overlay channels', comb_cat)+chn_labels,
                           period=args.period, **opt)

    elif args.mode == 'overlay_selections_years':

        indirs = []
        list_masses = []
        for sel in args.selections:
            indirs.append(os.path.join(base, 'cards_Years_' + args.var + '_CombChn',
                                       sel + '_' + args.var, 'combined_out'))
            list_masses.append(sel_masses(sel, args.masses))
        outfile = os.path.join(outdir,'_'.join(('limit', args.mode, args.var)))
        create_limits_plot(indirs, outfile, masses=list_masses,
                           labels=(comb_chn, comb_cat), **opt)
        
    elif args.mode == 'overlay_years':

        indirs = []
        list_masses = []
        periods = ("UL16", "UL16APV", "UL17", "UL18", "Run2")
        for period in periods:
            indirs.append(os.path.join(base, 'cards_' + args.tag + '_' + period + '_All', 'combined_out'))
            list_masses.append(args.masses)

        outfile = os.path.join(outdir, '_'.join(('limit', args.mode, args.var)))
        create_limits_plot(indirs, outfile, masses=list_masses,
                           labels=(comb_chn, ''),
                           legends=periods,
                           period="Empty", **opt)

if __name__ == "__main__":
    usage = ('usage: %prog [options] datasetList\n %prog -h for help')
    parser = argparse.ArgumentParser(description=usage)
    parser.add_argument('-n', '--dry_run', action='store_true', help='dry run mode')
    parser.add_argument('-c', '--channels', nargs='+', help='channels')
    parser.add_argument('-l', '--selections', nargs='+', help='selections')
    parser.add_argument('-m', '--masses', type=int, nargs='+', help='masses')
    parser.add_argument('-p', '--period', choices=('UL16', 'UL16APV', 'UL17', 'UL18', 'All'),
                        help='data period')
    parser.add_argument('-s', '--signal', type=str, help='signal type')
    parser.add_argument('-t', '--tag', help='tag')
    parser.add_argument('-b', '--basedir', help='Base directory')
    parser.add_argument('--mode', choices=('separate', 'sel_group', 'chn_group', 'all_group', 'all_years',
                                           'overlay_channels', 'overlay_selections',
                                           'overlay_channels_years', 'overlay_selections_years',
                                           'overlay_years'), help='mode')
    parser.add_argument('-u', '--user', default='', help='EOS username to store the plots.')
    parser.add_argument('-v', '--var', help='variable to perform the maximum likelihood fit')
    parser.add_argument('--atlas', action='store_true', help='overlay ATLAS full Run2 result')
    parser.add_argument('--interpolate', action='store_true', help='Interpolate uncertainty bands.')
    parser.add_argument('--nobands', action='store_true', help='Plot only the nominal value.')
    FLAGS = parser.parse_args()

    FLAGS.var = FLAGS.var.replace('{1}', '{}')
    
    user = os.environ['USER'] if FLAGS.user=='' else FLAGS.user
    outdir = os.path.join('/eos/home-' + user[0] + '/',  user, 'www/HH_Limits/')
    if 'years' not in FLAGS.mode:
        outdir = os.path.join(outdir, FLAGS.tag)
    else:
        outdir = os.path.join(outdir, FLAGS.tag + '_Run2_All')

    if not os.path.exists(outdir):
        os.makedirs(outdir)
        
    plot(FLAGS, outdir)
