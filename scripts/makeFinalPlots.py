# Coding: utf-8

_all_ = [ 'makeFinalPlots' ]

import os
import re
import argparse
import glob
import uproot as up
import hist
import pickle
import numpy as np
# import ROOT

import matplotlib; import matplotlib.pyplot as plt
import matplotlib.colors as colors
import mplhep as hep
plt.style.use(hep.style.ROOT)

from functools import wraps

class Histograms:
    def __init__(self, infile):
        self.infile = up.open(infile)
        self._hists = {}

    def _convert_to_hist(self, h):
        """
        Convert a ROOT histogram into a hist object.
        """
        # assert isinstance(h, ROOT.TH1) or isinstance(h, ROOT.TH2)
        xedges = h.axis(0).edges()
        yedges = h.axis(1).edges()
        values = h.values()
        errors = h.errors()
        newh = hist.Hist.new.Var(xedges, name="x").Var(yedges, name="y").Weight()
        for i in range(len(values)):
            for j in range(len(values[i])):
                xmid = xedges[i+1] - (xedges[i+1] - xedges[i])/2
                ymid = yedges[j+1] - (yedges[j+1] - yedges[j])/2
                newh.fill(x=xmid, y=ymid, weight=values[i][j])
                newh.view().variance = errors**2
        return newh

    def __del__(self):
        self.infile.close()

    @property
    def hists(self):
        return self._hists

    def mod_keys(self, keys):
        return [re.split(';', key)[0] for key in keys]
    
    def _read_histograms(func):
        """
        Decorator to load the histograms specified by the keys.
        If `keys` is a string it is treated as a regular expression.
        If `keys` is a list or tuple, it is treated as a list of histogram names.
        """
        @wraps(func)
        def wrapper(self, keys, *args, **kwargs):
            if not isinstance(keys, (list, tuple)):
                keys = "^" + keys + ";\d+$" # handle ROOT versioning system
                comp = re.compile(keys)
                keys = self.mod_keys([s for s in self.infile.keys() if comp.search(s)])
            missing_keys = [h for h in keys if h not in self._hists]
            
            for key in missing_keys:
                self._hists[key] = self.infile[key].to_hist()

            return func(self, keys, *args, **kwargs)
        return wrapper

    @_read_histograms
    def hists(self, keys, *args, **kwargs):
        """Return the histograms specified by the keys."""
        keys = self.mod_keys(keys)
        return {k:self._hists[k] for k in keys}

    @_read_histograms
    def stack(self, keys, *args, **kwargs):
        """Stack histograms."""
        stack = hist.Stack(*[self._hists[k] for k in keys])
        return stack

    @_read_histograms
    def stack_mc(self, keys, *args, **kwargs):
        """Stack MC histograms."""
        mc_keys = [k for k in keys if "data" not in k if "Data" not in k]
        return self.stack(mc_keys, *args, **kwargs)

class Plotter:
    def __init__(self, output, channel="ETau", cat="baseline", year="2018", npads=1):
        self.output = output
        assert npads <= 2, "Only 1 or 2 pads are supported."

        self.fig, self.axes = plt.subplots(npads, 1, figsize=(16, 16), squeeze=False,
                                           gridspec_kw={'height_ratios': [3,1]})
        if npads > 1:
            plt.subplots_adjust(left=0.1, right=.95, top=.95, bottom=0.1,
                                wspace=0., hspace=0.)
        self.fontsize = 35
        self.fontscale = 0.8
        hep.cms.text(' Preliminary', fontsize=self.fontsize, ax=self.axes[0][0])

        
        lumi = {"2016APV": "19.5", "2016": "16.8", "2017": "41.5", "2018": "59.7"}[year]
        mu, tau = '\u03BC','\u03C4'
        chn_map = {"ETau": r"$bb\;e$"+tau, "TauTau":r"$bb\;$"+tau+tau,
                   "MuMu": r"$bb\;$"+mu+mu, "MuTau": r"$bb\;$"+mu+tau}
        hep.cms.lumitext(r"{} $fb^{{-1}}$ (13 TeV)".format(lumi),
                         fontsize=self.fontscale*self.fontsize, ax=self.axes[0][0])
        self.axes[0][0].text(0.03, 0.95, chn_map[channel], transform=self.axes[0][0].transAxes)
        self.axes[0][0].text(0.03, 0.92, cat, transform=self.axes[0][0].transAxes)
        
        self.colors = plt.cm.tab20.colors

    def _add_top_margin(self, data, margin=0.2):
        """Define vertical margin between data and subplot border."""
        max_y = max(sum(data).values()) if isinstance(data, hist.Stack) else max(data.values())
        if self.ax.get_yscale() == 'log':
            margin = 10
        margin *= max_y
        self.ax.set_ylim(self.ax.get_ylim()[0], max_y + margin)

    def _select_axis(func):
        """
        Decorator to select a particular figure axis.
        The `loc` argument is the location of the axis in the matplotlib figure.
        """
        @wraps(func)
        def wrapper(self, *args, **kwargs):
            if not hasattr(self, 'axes'):
                raise RuntimeError("No axes defined.")

            if 'loc' in kwargs:
                loc = kwargs['loc']
                del kwargs['loc']
            else:
                loc = 0

            if isinstance(self.axes, (list, tuple)) and loc >= len(self.axes):
                raise RuntimeError("Invalid axis location.")

            self.ax = self.axes[loc][0] # no pads along the horizontal direction
            return func(self, *args, **kwargs)
        return wrapper

    @_select_axis
    def histo(self, h, *args, **kwargs):
        """ Plot a histogram. """
        if 'equalwidth' in kwargs and kwargs['equalwidth']:
            h = self._histo_equalwidth(h, set_xticks=True)

        plot_opt = dict(linewidth=kwargs['linewidth'] if 'linewidth' in kwargs else 1,
                        edgecolor='black')
        self.ax.bar(h.axes[0].centers, h.values(),
                    width=h.axes[0].edges[1:] - h.axes[0].edges[:-1],
                    **plot_opt)

        if 'ylabel' not in kwargs:
            self.ax.set_ylabel("Weighted Counts", fontsize=self.fontscale*self.fontsize)

        self._set_options(data=h, *args, **kwargs)
        
    def graph(self, g, *args, **kwargs):
        """Plot a histogram like if it was a graph."""
        if 'equalwidth' in kwargs and kwargs['equalwidth']:
            g = self._histo_equalwidth(g, set_xticks=True)

        self.ax.errorbar(x=g.axes[0].centers, y=g.values(), yerr=np.sqrt(g.variances()),
                          fmt="o", color="black", markersize=0.4*self.fontsize, capsize=0.,
                          label=kwargs['leglabel'] if 'leglabel' in kwargs else "")

        self.ax.set_ylabel("Weighted Counts", fontsize=self.fontscale*self.fontsize)
        self._set_options(data=g, *args, **kwargs)

    @_select_axis
    def histo2d(self, h, *args, **kwargs):
        """ Plot a 2D histogram."""
        pass

    def _histo_equalwidth(self, h, set_xticks=False):
        """ Return a histogram with equal bin widths."""
        xedges = h.axes[0].edges
        values = h.values()
        variances = h.variances()
        
        newh = hist.Hist.new.Reg(len(xedges)-1, xedges[0], xedges[-1], name="x").Weight()
        newh.view().value = values
        newh.view().variance = variances

        if set_xticks:
            xedges = [round(x,6) for x in xedges]
            self.ax.set_xticks(newh.axes[0].edges, xedges, rotation=60)

        return newh

    @_select_axis
    def legend(self, ncols=1):
        self.ax.legend(fontsize=self.fontscale*self.fontsize, loc="best", ncols=ncols)
        
    @_select_axis
    def ratio(self, hup, hdo, *args, **kwargs):
        """Plot the ratio of two histograms."""
        self.ax.set_ylabel("Ratio", ha="center")

        plot_opt = dict(linewidth=kwargs['linewidth'] if 'linewidth' in kwargs else 1,
                        edgecolor='black')

        upvals = sum(hup).values() if isinstance(hup, hist.Stack) else hup.values()
        dovals = sum(hdo).values() if isinstance(hdo, hist.Stack) else hdo.values()
        
        if 'equalwidth' in kwargs and kwargs['equalwidth']:
            if isinstance(hup, hist.Stack):
                hup = self._stack_equalwidth(hup, set_xticks=True)
            else:
                hup = self._histo_equalwidth(hup, set_xticks=True)
        
        variance = hup.variances() / dovals**2
        ratio = upvals / dovals

        self.ax.errorbar(x=hup.axes[0].centers, y=ratio, yerr=np.sqrt(variance),
                         fmt='o', color='black',
                         markersize=0.4*self.fontsize, capsize=0.,
                         label=kwargs['leglabel'] if 'leglabel' in kwargs else "")

        # remove axis labels of top plot
        self.axes[0][0].axes.xaxis.set_ticklabels([])

        # force x limits to match the ones of top plot
        self.ax.set_xlim(self.axes[0][0].get_xlim())

        self.ax.set_ylim((0.4, 1.6))

        for i in (0.5, 1., 1.5):
            self.ax.axhline(y=i, color='gray', linestyle='--')
        
        self._set_options(*args, **kwargs)

    def save(self, name):
        if not os.path.exists(self.output):
            os.makedirs(self.output)
        name = os.path.join(self.output, name)
        for ext in ("pdf", "png"):
            plt.savefig(name + "." + ext)

    @_select_axis
    def stack(self, stack, *args, **kwargs):
        """Plot a stack of histograms."""
        self.ax.set_ylabel("Weighted Counts")
        if 'equalwidth' in kwargs and kwargs['equalwidth']:
            stack = self._stack_equalwidth(stack)
        plot_opt = dict(linewidth=kwargs['linewidth'] if 'linewidth' in kwargs else 1,
                        edgecolor='black')

        weights = {k.name:k.values() for k in stack.__dict__['_stack']}
        bins = stack.axes[0].edges
        labels = [k.split('_')[0] for k in list(weights.keys())]
        self.ax.hist([bins[:-1] for _ in range(len(weights))], bins=bins, weights=weights.values(), stacked=True,
                     color=self.colors[:len(weights)], label=labels)

        self._set_options(data=stack, *args, **kwargs)
        
    def _stack_equalwidth(self, stack, set_xticks=False):
        """Return a stack with histograms having equal bin widths."""
        xedges = stack.axes[0].edges
        newhistos = {h.name:self._histo_equalwidth(h)
                     for h in stack.__dict__['_stack']}
        newstack = hist.Stack.from_dict(newhistos)

        if set_xticks:
            xedges = [round(x,6) for x in xedges]
            self.ax.set_xticks(newstack.axes[0].edges, xedges, rotation=60)

        return newstack

    def _set_options(self, data=None, *args, **kwargs):
        """ Set the options for the plot."""
        options = {
            ('title', 'xlabel', 'ylabel'): dict(fontsize=self.fontscale*self.fontsize),
            ('xscale', 'yscale'): dict()
        }

        for option in options:
            for opt in option:
                if opt in kwargs:
                    try:
                        getattr(self.ax, "set_"+opt)(kwargs[opt], **options[option])
                    except KeyError or AttributeError:
                        getattr(self.ax, opt)(kwargs[opt], **options[option])


        # define bottom limit for log scale    
        if 'yscale' in kwargs and kwargs['yscale'] == 'log':
            self.ax.set_ylim(bottom=5E-2)

        if data:
            self._add_top_margin(data, margin=0.2)
        
    def __del__(self):
        plt.close()


def makeFinalPlots(infile, outdir):
    h = Histograms(infile)
    # hists = h.hists(keys='WH_boostedL_pnet_SStight_pdnn_m1000_s0_hh')

    # plot1 = Plotter(outdir)
    # plot1.histo(hists['WH_boostedL_pnet_SStight_pdnn_m1000_s0_hh'],
    #             xlabel=r"pDNN (mX=1000) [GeV]", equalwidth=True, linewidth=3)
    # plot1.save('test1')


    
    stack_mc = h.stack_mc(keys='.*_res1b_SR_pdnn_m1000_s0_hh', xlabel=r"pDNN (mX=1000) [GeV]", equalwidth=True)
    h_data = h.hists(keys='data_obs_res1b_SR_pdnn_m1000_s0_hh', leglabel="Data", equalwidth=True)['data_obs_res1b_SR_pdnn_m1000_s0_hh']

    plot2 = Plotter(outdir, npads=2)
    plot2.stack(stack_mc, equalwidth=True, linewidth=1, yscale='linear')
    plot2.graph(h_data, equalwidth=True, linewidth=1, leglabel="Data")
    plot2.ratio(h_data, stack_mc, loc=1, xlabel=r"pDNN (mX=1000) [GeV]", equalwidth=True)
    
    plot2.legend(ncols=4)
    plot2.save('test2')
    

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Make final plots for the analysis.')
    parser.add_argument('-i', '--infile', type=str, required=True, help='Input file')
    parser.add_argument('-o', '--outdir', type=str, required=True, help='Output directory')
    args = parser.parse_args()

    # Load the histograms
    infile = os.path.join(args.infile, 'combined_outLimits.root')
    makeFinalPlots(infile, args.outdir)    
