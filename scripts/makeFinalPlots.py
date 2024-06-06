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
    def __init__(self, output, channel="ETau", year="2018"):
        self.output = output

        self.fig, self.axis = plt.subplots(1, 1, figsize=(16, 16))
        self.fontsize = 30
        self.fontscale = 0.8
        hep.cms.text(' Preliminary', fontsize=self.fontsize)
        
        lumi = {"2016APV": "19.5", "2016": "16.8", "2017": "41.5", "2018": "59.7"}[year]
        mu, tau = '\u03BC','\u03C4'
        dd = {"ETau": "e"+tau, "TauTau":tau+tau, "MuMu": mu+mu, "MuTau": mu+tau}
        hep.cms.lumitext(dd[channel] + " channel | " + r"{} $fb^{{-1}}$ (13 TeV)".format(lumi),
                         fontsize=self.fontscale*self.fontsize)
        
        self.colors = plt.cm.tab20.colors
        
    def _select_axis(func):
        """
        Decorator to select a particular figure axis.
        The `loc` argument is the location of the axis in the matplotlib figure.
        """
        @wraps(func)
        def wrapper(self, plot_obj, loc=0, *args, **kwargs):
            if not hasattr(self, 'axis'):
                raise RuntimeError("No axis defined.")
            if isinstance(self.axis, (list, tuple)) and loc >= len(self.axis):
                raise RuntimeError("Invalid axis location.")
     
            if isinstance(self.axis, (list, tuple)):
                self.ax = self.axis[loc]
            else:
                self.ax = self.axis
     
            return func(self, plot_obj, *args, **kwargs)
        return wrapper

    @_select_axis
    def histo(self, h, *args, **kwargs):
        """ Plot a histogram. """
        if 'equalwidth' in kwargs and kwargs['equalwidth']:
            h = self._histo_equalwidth(h, set_xticks=True)
        #hep.histplot(h, ax=self.axis)
        plot_opt = dict(linewidth=kwargs['linewidth'] if 'linewidth' in kwargs else 1,
                        edgecolor='black')
        self.ax.bar(h.axes[0].centers, h.values(),
                    width=h.axes[0].edges[1:] - h.axes[0].edges[:-1],
                    **plot_opt)
        self.ax.set_ylabel("Weighted Counts", fontsize=self.fontscale*self.fontsize)
        self._set_options(*args, **kwargs)
        
    def graph(self, g, *args, **kwargs):
        """Plot a histogram like if it was a graph."""
        if 'equalwidth' in kwargs and kwargs['equalwidth']:
            g = self._histo_equalwidth(g, set_xticks=True)

        self.ax.errorbar(x=g.axes[0].centers, y=g.values(), yerr=np.sqrt(g.variances()),
                          fmt="o", color="black", markersize=0.4*self.fontsize, capsize=0.,
                          label=kwargs['leglabel'] if 'leglabel' in kwargs else "")
        self.ax.set_ylabel("Weighted Counts", fontsize=self.fontscale*self.fontsize)
        self._set_options(*args, **kwargs)

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

        xedges = [round(x,6) for x in xedges]
        self.ax.set_xticks(newh.axes[0].edges, xedges, rotation=60)

        return newh

    def legend(self, ncols=1):
        plt.legend(fontsize=self.fontscale*self.fontsize, loc="best", ncols=ncols)
        
    def save(self, name):
        if not os.path.exists(self.output):
            os.makedirs(self.output)
        name = os.path.join(self.output, name)
        self.fig.tight_layout()
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
        plt.hist([bins[:-1] for _ in range(len(weights))], bins=bins, weights=weights.values(), stacked=True,
                 color=self.colors[:len(weights)], label=labels)

        #stack.plot(stack=True, histtype="fill", **plot_opt)
        self._set_options(*args, **kwargs)

    def _stack_equalwidth(self, stack):
        """Return a stack with histograms having equal bin widths."""
        xedges = stack.axes[0].edges
        newhistos = {h.name:self._histo_equalwidth(h)
                     for h in stack.__dict__['_stack']}
        newstack = hist.Stack.from_dict(newhistos)

        xedges = [round(x,6) for x in xedges]
        self.ax.set_xticks(newstack.axes[0].edges, xedges, rotation=60)

        return newstack

    def _set_options(self, *args, **kwargs):
        """ Set the options for the plot."""
        options = ['title', 'xlabel', 'ylabel']
        
        for opt in options:
            if opt in kwargs:
                try:
                    getattr(self.ax, "set_"+opt)(kwargs[opt], fontsize=self.fontscale*self.fontsize)
                except KeyError or AttributeError:
                    getattr(self.ax, opt)(kwargs[opt])

        
    def __del__(self):
        plt.close()


def makeFinalPlots(infile, outdir):
    h = Histograms(infile)
    hists = h.hists(keys='WH_boostedL_pnet_SStight_pdnn_m1000_s0_hh')

    plot1 = Plotter(outdir)
    plot1.histo(hists['WH_boostedL_pnet_SStight_pdnn_m1000_s0_hh'],
                xlabel=r"pDNN (mX=1000) [GeV]", equalwidth=True, linewidth=3)
    plot1.save('test1')


    plot2 = Plotter(outdir)

    stack_mc = h.stack_mc(keys='.*_res1b_SR_pdnn_m1000_s0_hh', xlabel=r"pDNN (mX=1000) [GeV]", equalwidth=True)
    plot2.stack(stack_mc, xlabel=r"pDNN (mX=1000) [GeV]", equalwidth=True, linewidth=1)

    h_data = h.hists(keys='data_obs_res1b_SR_pdnn_m1000_s0_hh',
                     leglabel="Data", equalwidth=True)
    plot2.graph(h_data['data_obs_res1b_SR_pdnn_m1000_s0_hh'], equalwidth=True, linewidth=1)
    
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

    
    
