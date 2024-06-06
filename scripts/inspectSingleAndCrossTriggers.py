# Coding: utf-8

_all_ = [ 'inspectSingleAndCrossTriggers' ]

import os
import argparse
import glob
import uproot as up
import hist
import pickle

import matplotlib; import matplotlib.pyplot as plt
import matplotlib.colors as colors
import mplhep as hep
plt.style.use(hep.style.ROOT)

def getRootFilePaths(channel, year):
    """
    Get the paths to the ROOT files containing the single and cross triggers.
    """
    if channel not in ('ETau', 'MuTau'):
        raise ValueError('Invalid channel: {}'.format(channel))
    if year not in ('UL16', 'UL16APV', 'UL17', 'UL18'):
        raise ValueError('Invalid year: {}'.format(year))

    year2 = {
        'UL16'    : '2016',
        'UL16APV' : '2016APV',
        'UL17'    : '2017',
        'UL18'    : '2018'
    }[year]

    base_pog = "weights/MuPogSF_UL/"
    base_cecile = "weights/trigSFs_UL_eleMu/"
    path_single = {
        "ETau": os.path.join(
            base_cecile, "sf_el_" + year2 + "_HLTEle32.root"),
        "MuTau": os.path.join(
            base_pog, year2,
            "Efficiencies_muon_generalTracks_Z_Run2018_UL_SingleMuonTriggers.root")
    }
    path_cross  = { 
        "ETau": os.path.join(
            base_cecile, "sf_el_" + year2 + "_HLTEle24Tau30.root"),
        "MuTau": os.path.join(
            base_cecile, "sf_mu_" + year2 + "_HLTMu20Tau27.root")
    }
    return path_single[channel], path_cross[channel]

def openHistograms(path_single, path_cross, channel):
    """
    Open the histograms contained in the ROOT files.
    """
    single_name = {
        "ETau": "eff_data",
        "MuTau": "NUM_IsoMu24_DEN_CutBasedIdTight_and_PFIsoTight_abseta_pt_efficiencyData"
    }[channel]
    cross_name = {
        "ETau": "eff_data",
        "MuTau": "eff_data"
    }[channel]
    
    with up.open(path_single) as f_single:
        h_single = f_single[single_name]
    with up.open(path_cross) as f_cross:
        h_cross = f_cross[cross_name]
    return h_single, h_cross

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
    def __init__(self, output, npads):
        self.output = output
        figsize = (25, 8) if npads == 3 else (16, 8)
        self.fig, self.axis = plt.subplots(1, npads, figsize=figsize)
        
    def histo(self, h, loc, channel, title="", labelpad=30):
        """ Plot a histogram. """
        ax = self.axis[loc]
        h = self._histo_2d_equalwidth(h, loc)
        _, cbar, _ = hep.hist2dplot(h, ax=ax) # h_single.plot2d(ax=ax1, label='Single triggers')
        cbar.set_label(r'$\varepsilon_{\text{trigger}}$', rotation=0, labelpad=labelpad)

        ax.set_xlabel(r"$p_{T}\:[GeV]$")
        if channel == "ETau":
            ax.set_ylabel(r"$\eta$")
        else:
            ax.set_ylabel(r"$|\eta|$")
        ax.set_title(title, pad=20)

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

def inspectSingleAndCrossTriggers(channel, year, output, ratio):
    p_single, p_cross = getRootFilePaths(channel, year)
    h_single, h_cross = openHistograms(p_single, p_cross, channel)

    h_single = convertToHist(h_single)
    if channel == "MuTau":
        h_single = h_single.T # invert axis
    h_cross = convertToHist(h_cross)

    title_single = {"ETau": r"Single-e",
                    "MuTau": r"Single-$\mu$"}[channel]
    title_cross = {"ETau": r"Cross-e$\tau$",
                   "MuTau": r"Cross-$\mu\tau$"}[channel]

    output = os.path.join("/eos/home-b/bfontana/www/TriggerScaleFactors/",
                          "SingleCrossComparison_" + channel + "_" + year)
    if ratio:
        output += "_ratio"

    plot = Plotter(output=output, npads=3 if ratio else 2)
    labelpads = {"ETau": (45,40), "MuTau": (30,45)}
    plot.histo(h_single, channel=channel, title=title_single, loc=0, labelpad=labelpads[channel][0])
    plot.histo(h_cross,  channel=channel, title=title_cross,  loc=1, labelpad=labelpads[channel][1])

    if ratio:
        if channel == "MuTau": # remove bin edges that do not match between the two histos
            end = len(h_cross.values()[0])-1
            h_div = divideHistos(h_single[1:,:end], h_cross[2:,:end])
        else:
            h_div = divideHistos(h_single, h_cross)
        plot.histo(h_div, title="Single / Cross", loc=2)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Inspect single and cross triggers')
    parser.add_argument('--channel', choices=('ETau', 'MuTau'), help='channel', required=True)
    parser.add_argument('--year', choices=('UL16', 'UL16APV', 'UL17', 'UL18'),
                        help='year', required=True)
    parser.add_argument('--output', help='output file', default='plot.png')
    parser.add_argument('--show_ratio', action="store_true")
    args = parser.parse_args()

    inspectSingleAndCrossTriggers(channel=args.channel, year=args.year, output=args.output,
                                  ratio=args.show_ratio)
