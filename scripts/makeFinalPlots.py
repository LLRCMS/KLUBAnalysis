# Coding: utf-8

_all_ = [ 'makeFinalPlots' ]

import os
import argparse

from utilsFinalPlots import Histograms, Plotter

class PlotterFactory:
    pass

def makeFinalPlots(infile, outdir):
    h = Histograms(infile)
    # hists = h.hists(keys='WH_boostedL_pnet_SStight_pdnn_m1000_s0_hh')

    # plot1 = Plotter(outdir)
    # plot1.histo(hists['WH_boostedL_pnet_SStight_pdnn_m1000_s0_hh'],
    #             xlabel=r"pDNN (mX=1000) [GeV]", equalwidth=True, linewidth=3)
    # plot1.save('test1')

    
    stack_mc = h.stack_mc(keys='.*_res1b_SR_pdnn_m1000_s0_hh', xlabel=r"pDNN (mX=1000) [GeV]", equalwidth=True)
    h_data = h.hists(keys='data_obs_res1b_SR_pdnn_m1000_s0_hh', leglabel="Data", equalwidth=True)['data_obs_res1b_SR_pdnn_m1000_s0_hh']

    # plot2 = Plotter(outdir, npads=2)
    # plot2.data_mc_with_ratio(h_data, stack_mc, linewidth=1, yscale='linear',
    #                          xlabel=r"pDNN (mX=1000) [GeV]", equalwidth=True)
    
    # plot2.legend(ncols=4)
    # plot2.save('test2')


    stack_mc_up = h.stack_mc(keys='.*_res1b_SR_pdnn_m1000_s0_hh_tes_DM0_up')
    stack_mc_down = h.stack_mc(keys='.*_res1b_SR_pdnn_m1000_s0_hh_tes_DM0_down')
    plot3 = Plotter(outdir, npads=2)
    plot3.syst_shape_with_ratio(stack_mc, stack_mc_up, stack_mc_down,
                                label="TES DM0", linewidth=3, yscale='log',
                                xlabel=r"pDNN (mX=1000) [GeV]", equalwidth=True)    
    plot3.save('test3', ncols_leg=1)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Make final plots for the analysis.')
    parser.add_argument('-i', '--infile', type=str, required=True, help='Input file')
    parser.add_argument('-o', '--outdir', type=str, required=True, help='Output directory')
    args = parser.parse_args()

    # Load the histograms
    infile = os.path.join(args.infile, 'combined_outLimits.root')
    makeFinalPlots(infile, args.outdir)    
