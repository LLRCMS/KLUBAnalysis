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
        self.debug_on = True

    def _debug(self, msg):
        """Prints a debug message."""
        if self.debug_on:
            print("[DEBUG] " + msg)

    def __del__(self):
        self.infile.close()

    @property
    def hists(self):
        return self._hists

    @property
    def keys(self):
        return self._hists.keys()
    
    def keys_all(self, keys):
        return self._filter_keys(keys)

    def _filter_keys(self, keys, mode=None):
        """
        If `keys` is a string it is treated as a regular expression.
        If `keys` is a list or tuple, it is treated as a list of histogram names.
        """
        data_prefixes = {"data", "Data"}
        signal_prefixes = {"Radion", "Graviton"}

        if not isinstance(keys, (list, tuple)):
            keys = "^" + keys + ";\d+$" # handle ROOT versioning system
            comp = re.compile(keys)
            keys = self.mod_keys([s for s in self.infile.keys() if comp.search(s)])

        # filter keys based on mode
        if mode == "mc":
            keys = [k for k in keys if not any(x in k for x in (data_prefixes | signal_prefixes))]
        elif mode == "data":
            keys = [k for k in keys if any(x in k for x in data_prefixes)]
        elif mode == "signal":
            keys = [k for k in keys if any(x in k for x in signal_prefixes)]

        # checks which keys correspond to histograms that have not been loaded
        miss_keys = [h for h in keys if h not in self._hists]
        
        return keys, miss_keys

    def mod_keys(self, keys):
        return [re.split(';', key)[0] for key in keys]

    def _read_histograms(mode=None):
        def _read_histograms_inner(func):
            """Decorator to load the histograms specified by the keys."""
            @wraps(func)
            def wrapper(self, keys, *args, **kwargs):
                keys, miss_keys = self._filter_keys(keys, mode=mode)
                for key in miss_keys:
                    self._hists[key] = self.infile[key].to_hist()
                    self._remove_neg_bins(self._hists[key])
                
                return func(self, keys, *args, **kwargs)
            return wrapper
        return _read_histograms_inner

    @_read_histograms()
    def read(self, keys, *args, **kwargs):
        """Read histograms."""
        pass
    
    @_read_histograms()
    def hists(self, keys, scale=1., rebin=1., *args, **kwargs):
        """Return the histograms specified by the keys."""
        keys = self.mod_keys(keys)

        if 'label' in kwargs:
            for key in keys:
                self._hists[key].label = kwargs['label']
                
        if scale != 1.:
            for key in keys:
                self._hists[key] *= scale
                self._hists[key].label += r"GeV (" + str(round(scale,3)) + " pb)"

        if rebin != 1.:
            for key in keys:
                self._hists[key] = self._hists[key][::hist.rebin(rebin)]
                
        return {k:self._hists[k] for k in keys}

    def _remove_neg_bins(self, h):
        """Remove negative bins from histogram, and renormalize."""
        sum_before = h.sum().value
        h.values()[h.values() < 0] = 0.
        sum_after = h.sum().value
        
        if sum_before != sum_after:
            self._debug("Removed negative bins from histogram " + h.name + ".")
            
            if sum_after < 0.:
                raise ValueError("Negative bins in histogram after removal.")
            elif sum_after > 0.:
                if sum_before > 0:
                    h *= sum_before/sum_after
                else:
                    h *= 0.
            else:
                h *= 0.

    @_read_histograms()
    def group(self, keys, *args, **kwargs):
        """Group histograms."""
        return sum([self._hists[k] for k in keys])

    def groups(self, group_names, *args, **kwargs):
        """Create several groups of histograms."""
        ret = {k:self.group(keys=v) for k,v in group_names.items()}
        for k,v in ret.items():
            v.name = k
        return ret

    @_read_histograms()
    def stack(self, keys, group_names=None, order=None, *args, **kwargs):
        """Stack histograms."""
        hsplit_old = [self._hists[k] for k in keys]

        if group_names is not None and order is not None:
            for names in group_names.values():
                for name in names:
                    if name in order:
                        mes = "The order list must not contain the names in the groups."
                        mes += " (name={})".format(name)
                        raise RuntimeError(mes)
            
        if group_names is not None:
            grouped_histos = self.groups(group_names)

            hsplit = []
            for h in hsplit_old:
                for _, groupnames in group_names.items():
                    if h.name not in groupnames:
                        hsplit.append(h)
            for _, vgroup in grouped_histos.items():
                hsplit.append(vgroup)

        else:
            hsplit = hsplit_old
                        
        if order is not None:
            if set([k.name for k in hsplit]) != set(order):
                mes = "Ensure that the order list contains all the histograms. We have:\n"
                mes += " - order = {}\n".format(order)
                mes += " - hsplit = {}\n".format([k.name for k in hsplit])
                raise RuntimeError(mes)

            # order the histograms
            names = [k.name for k in hsplit]
            index_order = [names.index(o) for o in order]
            hsplit = [hsplit[i] for i in index_order]

        if 'rebin' in kwargs:
            hsplit = [k[::hist.rebin(kwargs['rebin'])] for k in hsplit]
        return hist.Stack(*hsplit)

    @_read_histograms("mc")
    def stack_mc(self, keys, groups=None, order=None, *args, **kwargs):
        """Stack MC histograms, removing data and signal histograms."""
        keys, _ = self._filter_keys(keys, mode="mc")
        return self.stack(keys, groups, order, *args, **kwargs)

class Plotter:
    def __init__(self, output, channel="ETau", cat="baseline", region="SR", year="2018", npads=1):
        self.output = output
        self.npads = npads
        assert self.npads <= 2, "Only 1 or 2 pads are supported."
        self.was_ratio_run = 0
        self.ymax = 0
        self.yunits = self._define_yunits()
        self.debug_on = True

        if self.npads == 2:
            fig, self.axes = plt.subplots(self.npads, 1, figsize=(16, 16), squeeze=False,
                                          gridspec_kw={'height_ratios': [3,1]})
        elif self.npads == 1:
            fig, self.axes = plt.subplots(self.npads, 1, figsize=(16, 16), squeeze=False)


        if self.npads > 1:
            plt.subplots_adjust(left=0.12, right=.95, top=.95, bottom=0.12,
                                wspace=0., hspace=0.04)
        self.fontsize = 45
        self.fontscales = (0.6, 0.8)
        
        hep.cms.text(' Preliminary', fontsize=self.fontsize, ax=self.axes[0][0])

        lumi = {"2016APV": "19.5", "2016": "16.8", "2017": "41.5", "2018": "59.7"}[year]
        chn_map = {"ETau": r"$bb\;e\tau$", "TauTau":r"$bb\;\tau\tau$",
                   "MuMu": r"$bb\;\mu\mu$", "MuTau": r"$bb\;\mu\tau$"}
        cat_map = {"baseline": "baseline",
                   "res1b": "res1b", "res2b": "res2b", "boosted": "boosted", "boostedL_pnet": "boosted",
                   "dyCR": "DY CR", "dyCR_res1b": "res1b DY CR", "dyCR_res2b": "res2b DY CR",
                   "ttbarCR": r"$t\bar{t}$ CR"}
        reg_map = {"SR": "SR",
                   "SStight": "SS, iso",
                   "OSinviso": "OS, anti-iso",
                   "SSinviso": "SS, anti-iso"}
        hep.cms.lumitext(r"{} $fb^{{-1}}$ (13 TeV)".format(lumi),
                         fontsize=self.fontscales[1]*self.fontsize, ax=self.axes[0][0])
        self.axes[0][0].text(0.03, 0.95, chn_map[channel], transform=self.axes[0][0].transAxes)
        self.axes[0][0].text(0.03, 0.91, cat_map[cat], transform=self.axes[0][0].transAxes)
        self.axes[0][0].text(0.03, 0.87, reg_map[region], transform=self.axes[0][0].transAxes)
        
        self.colors = plt.cm.tab20.colors
        self.iter_colors = iter(self.colors)

    def _debug(self, msg):
        """Prints a debug message."""
        if self.debug_on:
            print("[DEBUG] " + msg)

    def _add_top_margin(self, data, margin=0.25):
        """Define vertical margin between data and subplot border."""
        max_y = max(sum(data).values()) if isinstance(data, hist.Stack) else max(data.values())
        if self.ax.get_yscale() == 'log':
            margin = 10
        margin *= max_y
        new_max = max_y + margin
        if new_max > self.ymax:
            self.ax.set_ylim(self.ax.get_ylim()[0], new_max)
            self.ymax = new_max

    def mc(self, stackmc, *args, **kwargs):
        """Convenience function to plot data and MC with ratio."""
        self.stack(stackmc, loc=0, *args, **kwargs)
        self._stats_band(stackmc, loc=0, mode="standard", *args, **kwargs)

    def data_mc(self, hdata, stackmc, *args, **kwargs):
        """Convenience function to plot data and MC with ratio."""
        self.stack(stackmc, loc=0, *args, **kwargs)
        self._stats_band(stackmc, loc=0, mode="standard", *args, **kwargs)
        self.graph(hdata, loc=0, label="Data", *args, **kwargs)

    def mc_signal(self, stackmc, hsignals, *args, **kwargs):
        """Convenience function to plot data and MC with ratio, and the signal."""
        self.mc(stackmc, *args, **kwargs)
        self.signal(hsignals, *args, **kwargs)

    def data_mc_signal(self, hdata, stackmc, hsignals, *args, **kwargs):
        """Convenience function to plot data and MC with ratio, and the signal."""
        self.data_mc(hdata, stackmc, *args, **kwargs)
        self.signal(hsignals, *args, **kwargs)
        
    def data_mc_with_ratio(self, hdata, stackmc, *args, **kwargs):
        """Convenience function to plot data and MC with ratio."""
        assert self.npads > 1, "Need at least two pads to plot ratio."
        self.data_mc(hdata, stackmc, *args, **kwargs)

        # the ratio has always a vertical linear scale
        if 'yscale' in kwargs and kwargs['yscale'] == 'log':
            kwargs['yscale'] = 'linear'

        self.ratio(hdata, stackmc, loc=1, *args, **kwargs)
    
    def data_mc_signal_with_ratio(self, hdata, stackmc, hsignals, *args, **kwargs):
        """Convenience function to plot data and MC with ratio, and the signal."""
        assert self.npads > 1, "Need at least two pads to plot ratio."
        self.signal(hsignals, *args, **kwargs)
        self.data_mc_with_ratio(hdata, stackmc, *args, **kwargs)
        
    def _define_yunits(self):
        return {
            ("dau1_pt", "dau2_pt", "bjet1_pt", "bjet2_pt", "tauH_mass", "tauH_pt", "bH_mass", "bH_pt",
             "met_et", "metnomu_et", "METx", "METy", "tauH_SVFit_mass", "tauH_SVFit_pt",
             "HHbregrsvfit_m", "HHbregrsvfit_pt", "HH_mass", "HHKin_mass"): "GeV",
        }
        
    def __del__(self):
        plt.close()
        
    def _div(self, num, den):
        """Ignore division by zero; they are correctly handled by the plot."""
        with np.errstate(divide='ignore', invalid='ignore'):
            ratio = num / den
        return ratio

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
            bin_label = self._read_bins(h)
            self.ax.set_ylabel(bin_label, fontsize=self.fontscales[1]*self.fontsize)

        self._set_options(data=h, *args, **kwargs)

    def _get_current_axis_location(self):
        """Assumes only vertical axis."""
        for iax, ax in enumerate(self.axes):
            for a in ax:
                if a == self.ax:
                    return iax
    
    def graph(self, g, *args, **kwargs):
        """Plot a histogram like if it was a graph."""
        if 'equalwidth' in kwargs and kwargs['equalwidth']:
            g = self._histo_equalwidth(g, set_xticks=True)

        self.ax.errorbar(x=g.axes[0].centers, y=g.values(), yerr=np.sqrt(g.variances()),
                         fmt="o", color="black", markersize=0.4*self.fontsize, capsize=0.,
                         label=kwargs['label'] if 'label' in kwargs else "")

        bin_label = self._read_bins(g)
        self.ax.set_ylabel(bin_label, fontsize=self.fontscales[1]*self.fontsize)
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
        newh.label = h.label
        newh.view().value = values
        newh.view().variance = variances
        if set_xticks:
            self._set_xticks(newh, xedges)

        return newh

    def _legend(self, ncols=1):
        for ax in self.axes:
            for a in ax:
                if a != ax[0]:
                    continue

                # do not draw the legend for the ratio plot if it was run at most once
                if len(self.axes)>1 and ax==self.axes[1] and self.was_ratio_run < 2:
                    continue

                leg = a.legend(fontsize=0.7*self.fontscales[1]*self.fontsize,
                               ncols=ncols, bbox_to_anchor=(0.99,0.99), loc="upper right",
                               frameon=True, facecolor='white', edgecolor='black', framealpha=1.)
                leg.get_frame().set_boxstyle('Square', pad=0.0)
        
    @_select_axis
    def ratio(self, hup, hdo, mode="errorbar", ylim=(0.4, 1.6), *args, **kwargs):
        """Plot the ratio of two histograms."""
        self.was_ratio_run += 1
        self.ax.set_ylabel("Ratio", ha="center", fontsize=self.fontscales[1]*self.fontsize)

        plot_opt = dict(
            linewidth=kwargs['linewidth'] if 'linewidth' in kwargs else 1,
            label=kwargs['label'] if 'label' in kwargs else ""
        )
        
        upvals = sum(hup).values() if isinstance(hup, hist.Stack) else hup.values()
        dovals = sum(hdo).values() if isinstance(hdo, hist.Stack) else hdo.values()
        upvars = sum(hup).variances() if isinstance(hup, hist.Stack) else hup.variances()
        dovars = sum(hdo).variances() if isinstance(hdo, hist.Stack) else hdo.variances()

        if 'equalwidth' in kwargs and kwargs['equalwidth']:
            if isinstance(hup, hist.Stack):
                hup = self._stack_equalwidth(hup, set_xticks=True)
            else:
                hup = self._histo_equalwidth(hup, set_xticks=True)

        ratio = self._div(upvals, dovals)

        edges = hup.axes[0].edges
        if mode == "errorbar":
            # plot uncertainty bands on top of the current axis
            curr_loc = self._get_current_axis_location()
            self._stats_band(hdo, loc=curr_loc, mode="ratio")
            
            uncert_up = self._div(np.sqrt(upvars), upvals)
            self.ax.errorbar(x=hup.axes[0].centers, y=ratio, yerr=uncert_up,
                             fmt='o', color='black', 
                             markersize=0.4*self.fontsize, capsize=0.,
                             **plot_opt)

        elif mode == "line":
            bins = hup.axes[0].edges
            ratio = np.nan_to_num(ratio, nan=0.) # replace NaN by 0 for displying purposes
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

    def _read_bins(self, h):
        """Return the bin centers and bin widths."""
        edges = h.axes[0].edges
        if ((np.diff(edges) - np.diff(edges)[0]) == 0).all():
            bin_size_str = " / " + str(round(np.diff(edges)[0], 2))
        else:
            bin_size_str = ""
        label = h[0].label if isinstance(h, hist.Stack) else h.label
        
        for tup, unit in self.yunits.items():
            if bin_size_str != "" and any(x in label for x in tup):
                bin_size_str += " " + unit
        return "Events " + bin_size_str
        
    def save(self, name, ncols_leg=1):
        """
        Save the plot.
        Controls the number of columns of the plot's legend.
        """
        self._legend(ncols_leg)
        name = os.path.join(self.output, name)
        self._debug("Save {}.".format(name))

        for ext in ("pdf", "png"):
            plt.savefig(name + "." + ext)

    def _set_xticks(self, h, xedges):
        """
        Set the x-ticks for the histogram.
        Remove xtick labels when there are too many (limit defined at 25 labels)
        """
        xedges = [str(round(x,6)) for x in xedges]
        freq_remove = np.ceil(len(xedges) / 25)
        xedges = [x if xedges.index(x)%freq_remove==0 else "" for x in xedges]        
        self.ax.set_xticks(h.axes[0].edges, xedges, rotation=60)

    def signal(self, hsignals, *args, **kwargs):
        if not isinstance(hsignals, (tuple, list)):
            hsignals = [hsignals]

        signal_colors = self.colors[::-1] # invert palette to distinguish signal from mc
        for hsignal in hsignals:
            self.histo(hsignal, loc=0, label=hsignal.label, histtype="step",
                       edgecolor=signal_colors[hsignals.index(hsignal)], *args, **kwargs)

    @_select_axis
    def stack(self, stack, *args, **kwargs):
        """Plot a stack of histograms."""
        bin_label = self._read_bins(stack)
        self.ax.set_ylabel(bin_label, fontsize=self.fontscales[1]*self.fontsize)
        if 'equalwidth' in kwargs and kwargs['equalwidth']:
            stack = self._stack_equalwidth(stack)
            
        plot_opt = dict(linewidth=0., edgecolor=None)

        weights = {k.name:k.values() for k in stack.__dict__['_stack']}
        bins = stack.axes[0].edges
        labels = [k.split('_')[0] for k in list(weights.keys())]
        self.ax.hist([bins[:-1] for _ in range(len(weights))], bins=bins,
                     weights=weights.values(), stacked=True,
                     color=self.colors[:len(weights)], label=labels, **plot_opt)

        self._set_options(data=stack, *args, **kwargs)
        
    def _stack_equalwidth(self, stack, set_xticks=False):
        """Return a stack with histograms having equal bin widths."""
        xedges = stack.axes[0].edges
        newhistos = {h.name:self._histo_equalwidth(h)
                     for h in stack.__dict__['_stack']}
        newstack = hist.Stack.from_dict(newhistos)

        if set_xticks:
            self._set_xticks(new_stack, xedges)
            
        return newstack

    @_select_axis
    def _stats_band(self, h, mode="ratio", *args, **kwargs):
        """Plot statistical bands."""
        edges = h.axes[0].edges
        vals = sum(h).values() if isinstance(h, hist.Stack) else h.values()
        variances = sum(h).variances() if isinstance(h, hist.Stack) else h.variances()
        
        uncert_band = hist.Hist.new.Var(edges, name="uncert_band").Weight()

        if 'equalwidth' in kwargs and kwargs['equalwidth']:
            uncert_band = self._histo_equalwidth(uncert_band)

        assert mode in ("ratio", "standard"), "Invalid mode."
        if mode == "ratio":
            bottom = 1.
            uncert_band.view().value = np.nan_to_num(self._div(np.sqrt(variances), vals), nan=0.)
            # remove infinities (or very large values)
            uncert_band.view().value[(np.isinf(uncert_band.view().value)) | (uncert_band.view().value>1E300)] = 0.
        elif mode == "standard":
            bottom = sum(h).values() if isinstance(h, hist.Stack) else h.values()
            uncert_band.view().value = np.sqrt(variances)

        new_edges = uncert_band.axes[0].edges
        uncert_band.view().variance = 0.
        for sign in (-1., 1.):
            self.ax.hist([new_edges[:-1]], bins=new_edges, weights=sign*uncert_band.values(),
                         bottom=bottom, histtype="stepfilled", color="gainsboro",
                         hatch='///', edgecolor="gray", linewidth=0., alpha=0.6)

        self._set_options(data=uncert_band+bottom, *args, **kwargs)
        
    def _set_options(self, data=None, *args, **kwargs):
        """ Set the options for the plot."""
        options = {
            ('title', 'xlabel', 'ylabel', 'xticks', 'yticks'): dict(fontsize=self.fontscales[1]*self.fontsize),
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
                if min_y < 0:
                    mes = "Minimum value is negative: {}".format(min_y)
                    raise RuntimeError(mes)
                if min_y > 0:
                    self.ax.set_ylim(0.05*min_y)

        self.ax.tick_params(axis='both', which='major', labelsize=self.fontscales[0]*self.fontsize)

        # set the position of the offset text used in the y-axis for scientific notation
        self.ax.get_yaxis().get_offset_text().set_position((-0.04,0.))
        
        if data:
            self._add_top_margin(data, margin=0.25)

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
