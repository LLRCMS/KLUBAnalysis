# Coding: utf-8

_all_ = [ 'inspectSingleAndCrossTriggers' ]

import os
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

class WP:
    def __init__(self, idwp, isowp):
        self.id = idwp
        self.iso = isowp
        self.idstr = "ID="+str(idwp)
        self.isostr = "Iso="+str(isowp)


def getRootFilePaths(channel, year):
    """
    Get the paths to the ROOT files containing the single and cross triggers.
    """
    if channel not in ('ETau', 'MuTau'):
        raise ValueError('Invalid channel: {}'.format(channel))
    if year not in ('2016', '2016APV', '2017', '2018'):
        raise ValueError('Invalid year: {}'.format(year))

    base_pog = "weights/MuPogSF_UL/"
    base_cecile = "weights/trigSFs_UL_eleMu/"
    path_single = {
        "ETau": os.path.join(
            base_cecile, "sf_el_" + year + "_HLTEle32.root"),
        "MuTau": os.path.join(
            base_pog, year,
            "Efficiencies_muon_generalTracks_Z_Run" + year + "_UL_SingleMuonTriggers.root")
    }
    path_cross  = { 
        "ETau": os.path.join(
            base_cecile, "sf_el_" + year + "_HLTEle24Tau30.root"),
        "MuTau": os.path.join(
            base_cecile, "sf_mu_" + year + "_HLTMu20Tau27.root")
    }
    return path_single[channel], path_cross[channel]

def openHistogramsEff(path_single, path_cross, channel, year, wp, mode):
    """
    Open the histograms contained in the ROOT files.
    """
    single_name = lambda mode : {"2018": {"ETau": "eff_"+mode,
                                          "MuTau": "NUM_IsoMu24_DEN_CutBasedId"+wp.id+"_and_PFIso"+wp.iso+"_abseta_pt_efficiencyData"},
                                 "2017": {"ETau": "eff_"+mode,
                                          "MuTau": "NUM_IsoMu27_DEN_CutBasedId"+wp.id+"_and_PFIso"+wp.iso+"_abseta_pt_efficiencyData"},
                                 }[year][channel]
                   
    cross_name = lambda mode : {"ETau": "eff_"+mode,
                                "MuTau": "eff_"+mode
                                }[channel]
    
    with up.open(path_single) as f_single:
        h_single = f_single[single_name(mode)]
    with up.open(path_cross) as f_cross:
        h_cross = f_cross[cross_name(mode)]
    return h_single, h_cross

def openHistogramsSF(path_single, path_cross, channel, year, wp):
    """
    Open the histograms contained in the ROOT files.
    """
    mode_map = {"data": "Data", "mc": "MC"}
    single_name = lambda mode : {"2018": {"ETau": "eff_"+mode,
                                          "MuTau": "NUM_IsoMu24_DEN_CutBasedId"+wp.id+"_and_PFIso"+wp.iso+"_abseta_pt_efficiency"+mode_map[mode]},
                                 "2017": {"ETau": "eff_"+mode,
                                          "MuTau": "NUM_IsoMu27_DEN_CutBasedId"+wp.id+"_and_PFIso"+wp.iso+"_abseta_pt_efficiency"+mode_map[mode]},
                                 }[year][channel]
                   
    cross_name = lambda mode : {"ETau": "eff_"+mode,
                                "MuTau": "eff_"+mode
                                }[channel]
    
    with up.open(path_single) as f_single:
        h_single_data = f_single[single_name('data')]
        h_single_mc = f_single[single_name('mc')]
    with up.open(path_cross) as f_cross:
        h_cross_data = f_cross[cross_name('data')]
        h_cross_mc = f_cross[cross_name('mc')]

    return h_single_data, h_single_mc, h_cross_data, h_cross_mc


# convert ROOT histogram TH1 into hist object
def convertToHist(h):
    """
    Convert a ROOT histogram into a hist object.
    """
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

class Plotter:
    def __init__(self, output, channel, mode, wp_ratio, year, npads):
        self.output = output
        figsize = (25, 10) if npads == 3 else (16, 10)
        self.channel = channel
        self.fontsize = 40
        
        self.fig, self.axis = plt.subplots(1, npads, figsize=figsize)
        chn_map = {"ETau": r"$e\tau$", "MuTau": r"$\mu\tau$"}

        if wp_ratio:
            if mode == "data":
                title = "Data, Tight/Medium"
            elif mode == "mc":
                title = "MC, Tight/Medium"
            else:
                title = "SFs, Tight/Medium"
        else:
            if mode == "data":
                title = "Data efficiencies"
            elif mode == "mc":
                title = "MC efficiencies"
            else:
                title = "SFs: Data/MC"

        title += " | Channel: {} | Year: {}".format(chn_map[channel], year)
        self.fig.suptitle(title, fontsize=self.fontsize)
        
    def histo(self, h, loc, title="", ratio=True, labelpad=30):
        """ Plot a histogram. """
        ax = self.axis[loc]
        h = self._histo_2d_equalwidth(h, loc)
        _, cbar, _ = hep.hist2dplot(h, ax=ax) # h_single.plot2d(ax=ax1, label='Single triggers')
        if ratio:
            cbar.set_label('Ratio', rotation=90, labelpad=labelpad, y=0.6)
        else:
            cbar.set_label(r'$\varepsilon_{\text{trigger}}$', rotation=90, labelpad=labelpad, y=0.6)

        ax.set_xlabel(r"$p_{T}\:[GeV]$")
        if self.channel == "ETau":
            ax.set_ylabel(r"$\eta$")
        else:
            ax.set_ylabel(r"$|\eta|$")
        ax.set_title(title, pad=20, fontsize=0.65*self.fontsize)

        if loc == len(self.axis)-1:
            self.fig.tight_layout()
            for ext in ("pdf", "png"):
                plt.savefig(self.output + "." + ext)
        
    def _histo_2d_equalwidth(self, h, loc, set_xticks=False):
        """ Return a 2D histogram with equal bin widths."""
        xedges, yedges = h.axes[0].edges, h.axes[1].edges
        values = h.values()
        variances = h.variances()
        
        newh = (hist.Hist.new.Reg(len(xedges)-1, xedges[0], xedges[-1], name="x")
                .Reg(len(yedges)-1, yedges[0], yedges[-1], name="y").Weight())
        newh.view().value = values
        newh.view().variance = variances

        xedges = [round(x,6) for x in xedges]
        yedges = [round(x,6) for x in yedges]
        self.axis[loc].set_xticks(newh.axes[0].edges, xedges, rotation=60)
        self.axis[loc].set_yticks(newh.axes[1].edges, yedges, rotation=0)

        return newh
                
    def __del__(self):
        plt.close()

# write function that supports division of two weighted histograms
def divideHistos(h1, h2):
    """
    Divide two histograms.
    This is not currently supported by hist for weighted histograms with variable axis.
    """
    h_div = h1.copy()
    h_div.view().value = h1.view().value / h2.view().value
    h_div.view().variance = h1.view().variance / h2.view().value**2
    return h_div

def rebin_cross_low_pt_bin_mutau(h, first_xedge):
    """
    Rebin the cross trigger histogram to match the binning of the single trigger histogram.
    The contents are left unchanged, since each bin contains an efficiency and not a count.
    """
    edges = np.concatenate((np.array([first_xedge,]), h.axes[0].edges[1:]))
    newh = hist.Hist.new.Var(edges, name="x").Var(h.axes[1].edges, name="y").Weight()
    newh.view().value = h.values()
    newh.variance = h.variances()
    return newh

def rebin_cross_low_pt_bin_etau(h, first_xedge):
    """
    Rebin the cross trigger histogram to match the binning of the single trigger histogram.
    """
    edges = np.concatenate((np.array([first_xedge,]), h.axes[0].edges[1:]))
    newh = hist.Hist.new.Var(edges, name="x").Var(h.axes[1].edges, name="y").Weight()

    # weight the first x bins of the new histogram proportionally to the bin width
    dist_old = edges[1] - h.axes[0].edges[0]
    dist_new = edges[1] - edges[0]

    newh.view().value = h.values()
    newh.view().value[0,:] *= dist_new / dist_old

    newh.variance = h.variances()
    return newh

    
def inspectSingleAndCrossTriggers(channel, year, wp, mode, output, ratio, wp_ratio, rebin):
    p_single, p_cross = getRootFilePaths(channel, year)

    if mode == "sf":
        if wp_ratio:
            h_single_data_t, h_single_mc_t, h_cross_data_t, h_cross_mc_t = openHistogramsSF(p_single, p_cross, channel, year, WP("Tight", "Tight"))
            h_single_data_m, h_single_mc_m, h_cross_data_m, h_cross_mc_m = openHistogramsSF(p_single, p_cross, channel, year, WP("Medium", "Medium"))
            h_single_data_t, h_single_data_m = convertToHist(h_single_data_t), convertToHist(h_single_data_m)
            h_single_mc_t, h_single_mc_m = convertToHist(h_single_mc_t), convertToHist(h_single_mc_m)
            if channel == "MuTau":
                h_single_data_t, h_single_data_m = h_single_data_t.T, h_single_data_m.T
                h_single_mc_t, h_single_mc_m = h_single_mc_t.T, h_single_mc_m.T
            h_cross_data_t, h_cross_data_m = convertToHist(h_cross_data_t), convertToHist(h_cross_data_m)
            h_cross_mc_t, h_cross_mc_m = convertToHist(h_cross_mc_t), convertToHist(h_cross_mc_m)

        else:
            h_single_data, h_single_mc, h_cross_data, h_cross_mc = openHistogramsSF(p_single, p_cross, channel, year, wp)
            h_single_data = convertToHist(h_single_data)
            h_single_mc = convertToHist(h_single_mc)
            if channel == "MuTau":
                h_single_data = h_single_data.T
                h_single_mc = h_single_mc.T
            h_cross_data = convertToHist(h_cross_data)
            h_cross_mc = convertToHist(h_cross_mc)
        
    else:
        if wp_ratio:
            h_single_t, h_cross_t = openHistogramsEff(p_single, p_cross, channel, year, WP("Tight", "Tight"), mode)
            h_single_m, h_cross_m = openHistogramsEff(p_single, p_cross, channel, year, WP("Medium", "Medium"), mode)
            h_single_t, h_single_m = convertToHist(h_single_t),convertToHist(h_single_m)
            if channel == "MuTau":
                h_single_t, h_single_m = h_single_t.T, h_single_m.T # invert axis
            h_cross_t, h_cross_m = convertToHist(h_cross_t), convertToHist(h_cross_m)

        else:
            h_single, h_cross = openHistogramsEff(p_single, p_cross, channel, year, wp, mode)
            h_single = convertToHist(h_single)
            if channel == "MuTau":
                h_single = h_single.T # invert axis
            h_cross = convertToHist(h_cross)

    analysis = r"$(\gamma\gamma\rightarrow\tau\tau\:\:analysis)$"
    if wp_ratio:
        title_single = {"ETau": r"Single-e "+analysis,
                        "MuTau": r"Single-$\mu$ (POG: Tight/Medium)"}[channel]
    else:
        title_single = {"ETau": r"Single-e "+analysis,
                        "MuTau": r"Single-$\mu$ (POG: " + wp.idstr + ", " + wp.isostr + ")"}[channel]
    
    title_cross = {"ETau": r"Cross-e$\tau$ " + analysis,
                   "MuTau": r"Cross-$\mu\tau$ " + analysis,}[channel]

    if wp_ratio:
        suffix = '_'.join((channel, year, mode))
    else:
        suffix = '_'.join((channel, wp.id, wp.iso, year, mode))

    outdir = "/eos/home-b/bfontana/www/TriggerScaleFactors/SingleAndCrossMaps"
    if rebin and not wp_ratio:
        outdir += "/Rebin"
    elif rebin and wp_ratio:
        outdir += "/WPRatio_Rebin"
    elif not rebin and wp_ratio:
        outdir += "/WPRatio"
    else:
        outdir += "/Standard"
    if not os.path.exists(outdir):
        os.makedirs(outdir)
    output = os.path.join(outdir, "fig_" + suffix)

    if rebin and not wp_ratio:
        output += "_rebin"
    elif rebin and wp_ratio:
        output += "_wpratio_rebin"
    elif not rebin and wp_ratio:
        output += "_wpratio"

    if ratio:
        output += "_ratio"

    plot = Plotter(channel=channel, year=year, mode=mode, wp_ratio=wp_ratio, output=output, npads=3 if ratio else 2)
    labelpads = {"ETau": (25,20,20), "MuTau": (20,25,20)}

    if wp_ratio:
        if mode == "sf":
            h_single_t = divideHistos(h_single_data_t, h_single_mc_t)
            h_single_m = divideHistos(h_single_data_m, h_single_mc_m)
            h_cross_t  = divideHistos(h_cross_data_t, h_cross_mc_t)
            h_cross_m  = divideHistos(h_cross_data_m, h_cross_mc_m)
            h_single = divideHistos(h_single_t, h_single_m)
            h_cross = divideHistos(h_cross_t, h_cross_m)
        else:
            h_single = divideHistos(h_single_t, h_single_m)
            h_cross = divideHistos(h_cross_t, h_cross_m)
            
    else:
        if mode == "sf":
            h_single = divideHistos(h_single_data, h_single_mc)
            h_cross = divideHistos(h_cross_data, h_cross_mc)

    plot.histo(h_single, title=title_single, loc=0, ratio=mode=="sf" or wp_ratio, labelpad=labelpads[channel][0])
    plot.histo(h_cross,  title=title_cross,  loc=1, ratio=mode=="sf" or wp_ratio, labelpad=labelpads[channel][1])
        
    if ratio:
        if channel == "MuTau": # remove bin edges that do not match between the two histos
            end = len(h_cross.values()[0])-1
            if rebin:
                hsingle_cut = h_single[:,:end]
                single = hsingle_cut
                cross = rebin_cross_low_pt_bin_mutau(h_cross[1:,:end], first_xedge=hsingle_cut.axes[0].edges[0])
            else:
                single = h_single[1:,:end]
                cross = h_cross[2:,:end]
            h_div = divideHistos(single, cross)
        elif channel == "ETau":
            if rebin:
                hsingle_cut = h_single[2:,:]
                cross = h_cross[2:,:]
                single = rebin_cross_low_pt_bin_etau(hsingle_cut, first_xedge=cross.axes[0].edges[0])
                h_div = divideHistos(single, cross)
            else:
                single = h_single[2:,:]
                cross = h_cross[2:,:]
                h_div = divideHistos(single, cross)
        plot.histo(h_div, title="Single / Cross", loc=2, ratio=True, labelpad=labelpads[channel][2])

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Inspect single and cross triggers')
    parser.add_argument('--channel', choices=('ETau', 'MuTau'), help='channel', required=True)
    parser.add_argument('--year', choices=('2016', '2016APV', '2017', '2018'),
                        help='year', required=True)
    parser.add_argument('--idwp', choices=('Loose', 'Medium', 'Tight'), default="Tight", 
                        help='idwp', required=False)
    parser.add_argument('--isowp', choices=('Loose', 'Medium', 'Tight'), default="Tight", 
                        help='isowp', required=False)
    parser.add_argument('--mode', choices=('data', 'mc', 'sf'), default="data", 
                        help='mode')
    parser.add_argument('--rebin', action="store_true")
    parser.add_argument('--output', help='output file', default='plot.png')
    parser.add_argument('--wpratio', action="store_true", help='plot WP ratio')
    parser.add_argument('--show_ratio', action="store_true", help='show ratio')
    args = parser.parse_args()

    inspectSingleAndCrossTriggers(channel=args.channel, year=args.year, wp=WP(args.idwp, args.isowp),
                                  mode=args.mode,
                                  output=args.output, ratio=args.show_ratio, wp_ratio=args.wpratio, rebin=args.rebin)
