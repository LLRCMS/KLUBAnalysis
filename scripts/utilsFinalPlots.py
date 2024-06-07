# Coding: utf-8

_all_ = [ ]

import os
import re
import argparse
import glob
import uproot as up
import hist
import pickle
import numpy as np

import matplotlib; import matplotlib.pyplot as plt
import matplotlib.colors as colors
import mplhep as hep
plt.style.use(hep.style.ROOT)

from functools import wraps

class Histograms:
    def __init__(self, infile):
        self.infile = up.open(infile)
        self._hists = {}

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
    def read(self, keys, *args, **kwargs):
        """Read histograms."""
        pass
    
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
        """Stack MC histograms, removing data and signal histograms."""
        mc_keys = [k for k in keys if not any(x in k for x in ("data", "Data", "Radion", "Graviton"))]
        return self.stack(mc_keys, *args, **kwargs)

class Plotter:
    def __init__(self, output, channel="ETau", cat="baseline", year="2018", npads=1):
        self.output = output
        assert npads <= 2, "Only 1 or 2 pads are supported."
        self.was_ratio_run = 0

        self.fig, self.axes = plt.subplots(npads, 1, figsize=(16, 16), squeeze=False,
                                           gridspec_kw={'height_ratios': [3,1]})
        if npads > 1:
            plt.subplots_adjust(left=0.1, right=.95, top=.95, bottom=0.1,
                                wspace=0., hspace=0.04)
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
        self.axes[0][0].text(0.03, 0.91, cat, transform=self.axes[0][0].transAxes)
        
        self.colors = plt.cm.tab20.colors
        self.iter_colors = iter(self.colors)

    def _add_top_margin(self, data, margin=0.2):
        """Define vertical margin between data and subplot border."""
        max_y = max(sum(data).values()) if isinstance(data, hist.Stack) else max(data.values())
        if self.ax.get_yscale() == 'log':
            margin = 10
        margin *= max_y
        self.ax.set_ylim(self.ax.get_ylim()[0], max_y + margin)

    def data_mc_with_ratio(self, hdata, stackmc, *args, **kwargs):
        """Convenience function to plot data and MC with ratio."""
        self.stack(stackmc, loc=0, *args, **kwargs)
        self.graph(hdata, loc=0, label="Data", *args, **kwargs)
        self.ratio(hdata, stackmc, loc=1, *args, **kwargs)

    def __del__(self):
        plt.close()
        
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

        plot_opt = dict(
            linewidth=kwargs['linewidth'] if 'linewidth' in kwargs else 1,
            edgecolor=next(self.iter_colors) if 'edgecolor' not in kwargs else kwargs['edgecolor'],
            label=kwargs['label'] if 'label' in kwargs else "",
            histtype=kwargs['histtype'] if 'histtype' in kwargs else "stepfilled",
        )

        bins = h.axes[0].edges
        self.ax.hist([bins[:-1]], bins=bins, weights=h.values(), **plot_opt)

        if 'ylabel' not in kwargs:
            self.ax.set_ylabel("Weighted Counts", fontsize=self.fontscale*self.fontsize)

        self._set_options(data=h, *args, **kwargs)
        
    def graph(self, g, *args, **kwargs):
        """Plot a histogram like if it was a graph."""
        if 'equalwidth' in kwargs and kwargs['equalwidth']:
            g = self._histo_equalwidth(g, set_xticks=True)

        self.ax.errorbar(x=g.axes[0].centers, y=g.values(), yerr=np.sqrt(g.variances()),
                         fmt="o", color="black", markersize=0.4*self.fontsize, capsize=0.,
                         label=kwargs['label'] if 'label' in kwargs else "")

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

    def _legend(self, ncols=1):
        for ax in self.axes:
            for a in ax:
                if a != ax[0]:
                    continue

                # do not draw the legend for the ratio plot if it was run at most once
                if ax==self.axes[1] and self.was_ratio_run < 2:
                    continue

                leg = a.legend(fontsize=0.7*self.fontscale*self.fontsize,
                               loc="best", ncols=ncols,
                               frameon=True, facecolor='white', edgecolor='black', framealpha=1.)
                leg.get_frame().set_boxstyle('Square', pad=0.0)
        
    @_select_axis
    def ratio(self, hup, hdo, mode="errorbar", ylim=(0.4, 1.6), *args, **kwargs):
        """Plot the ratio of two histograms."""
        self.was_ratio_run += 1
        self.ax.set_ylabel("Ratio", ha="center")

        plot_opt = dict(
            linewidth=kwargs['linewidth'] if 'linewidth' in kwargs else 1,
            label=kwargs['label'] if 'label' in kwargs else ""
        )
        
        upvals = sum(hup).values() if isinstance(hup, hist.Stack) else hup.values()
        dovals = sum(hdo).values() if isinstance(hdo, hist.Stack) else hdo.values()
        
        if 'equalwidth' in kwargs and kwargs['equalwidth']:
            if isinstance(hup, hist.Stack):
                hup = self._stack_equalwidth(hup, set_xticks=True)
            else:
                hup = self._histo_equalwidth(hup, set_xticks=True)

        # ignore division by zero; they are correctly handled by the plot
        with np.errstate(divide='ignore', invalid='ignore'):
            variance = hup.variances() / dovals**2
            ratio = upvals / dovals - 1.

        if mode == "errorbar":
            self.ax.errorbar(x=hup.axes[0].centers, y=ratio, yerr=np.sqrt(variance),
                             fmt='o', color='black', 
                             markersize=0.4*self.fontsize, capsize=0.,
                             **plot_opt)
        elif mode == "line":
            ratio = np.nan_to_num(ratio, nan=0.) # replace NaN by 0 for displying purposes
            bins = hup.axes[0].edges
            self.ax.hist([bins[:-1]], bins=bins, weights=ratio, bottom=1.,
                         histtype="step",
                         color=next(self.iter_colors) if 'color' not in kwargs else kwargs['color'],
                         **plot_opt)
        else:
            raise ValueError("Invalid mode.")

        # remove axis labels of top plot
        self.axes[0][0].axes.xaxis.label.set_visible(False)
        self.axes[0][0].axes.xaxis.set_ticklabels([])

        # force x limits to match the ones of top plot
        self.ax.set_xlim(self.axes[0][0].get_xlim())

        self.ax.set_ylim(ylim)
        firstline = 0.5 if ylim[0] < 0.5 else 0.9
        thirdline = 1.5 if ylim[1] > 1.5 else 1.1
        for i in (firstline, 1., thirdline):
            self.ax.axhline(y=i, color='gray', linestyle='--')
        
        self._set_options(*args, **kwargs)
        
    def save(self, name, ncols_leg=1):
        self._legend(ncols_leg)
        if not os.path.exists(self.output):
            os.makedirs(self.output)
        name = os.path.join(self.output, name)
        # for ext in ("pdf", "png"):
        #     plt.savefig(name + "." + ext)

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
        self.ax.hist([bins[:-1] for _ in range(len(weights))], bins=bins,
                     weights=weights.values(), stacked=True,
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
            self.ax.set_ylim(bottom=5E-2) # default
            if data:
                min_y = (min(sum(data).values()) if isinstance(data, hist.Stack)
                         else min(data.values()))
                assert min_y >= 0, "Minimum value is negative."
                if min_y > 0:
                    self.ax.set_ylim(0.05*min_y)
            
                
        if data:
            self._add_top_margin(data, margin=0.2)

    def syst_shape_with_ratio(self, hnom, hup, hdo, label='', *args, **kwargs):
        """Plot the shape systematic uncertainty with ratio."""
        if isinstance(hnom, hist.Stack):
            hnom = sum(hnom)
        if isinstance(hup, hist.Stack):
            hup = sum(hup)
        if isinstance(hdo, hist.Stack):
            hdo = sum(hdo)

        opt = dict(histtype="step", *args, **kwargs)
        self.histo(hnom, loc=0, label=label+" Nominal", edgecolor="blue", **opt)
        self.histo(hup,  loc=0, label=label+" Up", edgecolor="green", **opt)
        self.histo(hdo,  loc=0, label=label+" Down", edgecolor="orange", **opt)

        # the ratio has always a vertical linear scale
        if 'yscale' in kwargs and kwargs['yscale'] == 'log':
            kwargs['yscale'] = 'linear'

        self.ratio(hup, hnom, mode="line", loc=1,
                   label="Up/Nom.", color="green", *args, **kwargs)
        self.ratio(hdo, hnom, mode="line", loc=1, ylim=(0.85, 1.15),
                   label="Down/Nom.", color="orange", *args, **kwargs)
