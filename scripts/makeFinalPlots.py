# Coding: utf-8

_all_ = [ 'makeFinalPlots' ]

import os
import argparse
import multiprocessing

from utilsFinalPlots import Histograms, Plotter

class PlotterFactory:
    def __init__(self, infile, outdir):
        self.outdir = outdir
        self.hists = Histograms(infile)

        self.years     = {"UL16", "UL16APV", "UL17", "UL18"}
        self.cat_res   = {"baseline", "res1b", "res2b"}
        self.cat_boost = {"baseline_boosted", "boostedL", "boostedT"}
        self.channels  = {"ETau", "MuTau", "TauTau"}
        self.regions   = {"SR", "SStight", "OSinviso", "SSinviso"}

        # name, sigscale per channel
        self.variables = {
            "dau1_pt"              : (r"$pT_{1} [GeV]$",              (90, 50, 20, 5000)),
			"dau2_pt"			   : (r"$pT_{2} [GeV]$",			  (90, 50, 9, 5000)),
			"bjet1_pt"			   : (r"$pT_{j1} [GeV]$",			  (90, 90, 9, 10000)),
			"bjet2_pt"			   : (r"$pT_{j2} [GeV]$",			  (90, 90, 9, 10000)),
			"dau1_eta"			   : (r"$\eta_{1}$",					  (40, 40, 2, 4000)),
			"dau2_eta"			   : (r"$\eta_{2}$",					  (40, 40, 2, 4000)),
			"bjet1_eta"			   : (r"$\eta_{j1}$",				  (40, 40, 4, 4000)),
			"bjet2_eta"			   : (r"$\eta_{j2}$",				  (40, 40, 4, 4000)),
			"tauH_mass"			   : (r"$m_{H\tau} [GeV]$",			  (30, 30, 2, 10000)),
			"tauH_pt"			   : (r"$pT_{H\tau} [GeV]$",		  (30, 30, 2, 10000)),
			# "tauH_eta"			   : (r"$eta_{H\tau} [GeV]$",		  (30, 30, 9, 10000)),
			"bH_mass"			   : (r"$m_{Hb} [GeV]$",			  (10, 10, 2, 1000)),
			"bH_pt"				   : (r"$pT_{Hb} [GeV]$",			  (30, 30, 2, 10000)),
			"ditau_deltaR"		   : (r"$\DeltaR(\tau\tau)$",		  (10, 10, 2, 1000)),
			"dib_deltaR"		   : (r"$\DeltaR(bb)$",				  (10, 10, 2, 1000)),
			"HH_deltaR"			   : (r"$\DeltaR(HH)$",				  (30, 30, 2, 7000)),
			"njets"				   : (r"$NJets$",					  (90, 90, 5, 10000)),
			"met_et"			   : (r"$MET [GeV]$",				  (10, 10, 1, 1000)),
			"met_phi"			   : (r"$MET-\phi$",				  (10, 10, 1, 1000)),
			"metnomu_et"		   : (r"$MET-no\mu [GeV]$",			  (20, 20, 2, 2000)),
			"metnomu_phi"		   : (r"$MET-no\mu-\phi$",			  (10, 10, 1, 5000)),
			# "dau1_dxy"			   : (r"$dxy_{1}$",					  (10, 10, 1, 5000)),
			# "dau1_dz"			   : (r"$dz_{1}$",					  (10, 10, 1, 5000)),
			# "dau2_dxy"			   : (r"$dxy_{2}$",					  (10, 10, 1, 4000)),
			# "dau2_dz"			   : (r"$dz_{2}$",					  (10, 10, 1, 4000)),
			"METx"				   : (r"$MET_{x} [GeV]$",			  (10, 10, 1, 4000)),
			"METy"				   : (r"$MET_{y} [GeV]$",			  (10, 10, 1, 4000)),
			# "met_cov00"			   : (r"$Cov(MET)_{00}$",			  (10, 10, 1, 4000)),
			# "met_cov01"			   : (r"$Cov(MET)_{01}$",			  (10, 10, 1, 4000)),
			# "met_cov11"			   : (r"$Cov(MET)_{11}$",			  (10, 10, 1, 4000)),
			"bjet1_bID_deepFlavor" : (r"$DeepFlavour_{j1}$",		  (10, 10, 1, 4000)),
			"bjet2_bID_deepFlavor" : (r"$DeepFlavour_{j2}$",		  (10, 10, 1, 4000)),
			# "bjet1_CvsB"		   : (r"$CvsB_{j1}$",				  (10, 10, 1, 4000)),
			# "bjet1_CvsL"		   : (r"$CvsL_{j1}$",				  (10, 10, 1, 4000)),
			# "bjet2_CvsB"		   : (r"$CvsB_{j2}$",				  (10, 10, 1, 4000)),
			# "bjet2_CvsL"		   : (r"$CvsL_{j2}$",				  (10, 10, 1, 4000)),
			# "bjet1_HHbtag"		   : (r"$HHbTag_{j1}$",				  (10, 10, 1, 4000)),
			# "bjet2_HHbtag"		   : (r"$HHbTag_{j2}$",				  (10, 10, 1, 4000)),
			"tauH_SVFIT_mass"	   : (r"$m_{H\tau}^{SVFit} [GeV]$",	  (30, 30, 2, 10000)),
			"tauH_SVFIT_pt"		   : (r"$pT_{H\tau}^{SVFit} [GeV]$",  (10, 10, 2, 10000)),
			# "tauH_SVFIT_eta"	   : (r"$eta_{H\tau}^{SVFit} [GeV]$", (10, 10, 2, 10000)),
			"HHbregrsvfit_m"	   : (r"$m_{HH}^{PNet} [GeV]$",		  (1, 1, 1, 150)),
			"HHbregrsvfit_pt"	   : (r"$pT_{HH}^{PNet} [GeV]$",	  (30, 30, 10, 10000)),
			"HHbregrsvfit_eta"	   : (r"$eta_{HH}^{PNet} ",			  (40, 40, 2, 4000)),
			# "HH_mass"			   : (r"$m_{HH} [GeV]$",			  (1, 1, 1, 150)),
			# "HHKin_mass"		   : (r"$m_{HHKin} [GeV]$",			  (1, 1, 1, 150)),
        }

        self.logvariables = {"dau1_dxy", "dau1_dz", "dau2_dxy", "dau2_dz", "HH_mass", "HHKin_mass"}
        self.equalwidth = {"HH_mass", "HHKin_mass"}

    def _sanity_checks(self, channel, category, region) -> None:
        """Sanity checks for the input channel and category."""
        assert channel in self.channels, f"Invalid channel: {channel}"
        assert category in self.cat_res.union(self.cat_boost), f"Invalid category: {category}"
        assert region in self.regions, f"Invalid region: {region}"

    def produce_data_mc_with_ratio(self, channel, category, region, year) -> None:
        """Produce the data/mc plots with ratio for the given channel and category."""
        self._sanity_checks(channel, category, region)

        def _produce_parallel(v):
            string = '_{}_{}_{}'.format(category, region, v)

            hdata = self.hists.hists(keys='data_obs'+string, leglabel="Data")['data_obs'+string]
            stackmc = self.hists.stack_mc(keys='.*'+string)

            p = Plotter(self.outdir, channel=channel, cat=category, year=year, npads=2)
            p.data_mc_with_ratio(hdata=hdata, stackmc=stackmc, linewidth=1,
                                 yscale='log' if v in self.logvariables else 'linear',
                                 xlabel=self.variables[v][0], equalwidth=v in self.equalwidth)
            p.save('plot' + string)

        num_workers = int(multiprocessing.cpu_count()/2)
        with multiprocessing.Pool(processes=num_workers) as pool:
            pool.starmap(_produce_parallel, self.variables.keys())

def makeFinalPlots(tag, year):
    basepath_in = "/data_CMS/cms/alves/HHresonant_hist/"
    basepath_out = "/eos/home-b/bfontana/www/HH_Plots/"
    # h = Histograms(infile)
    
    # stack_mc = h.stack_mc(keys='.*_res1b_SR_pdnn_m1000_s0_hh', xlabel=r"pDNN (mX=1000) [GeV]", equalwidth=True)
    # h_data = h.hists(keys='data_obs_res1b_SR_pdnn_m1000_s0_hh', leglabel="Data", equalwidth=True)['data_obs_res1b_SR_pdnn_m1000_s0_hh']

    # plot2 = Plotter(outdir, npads=2)
    # plot2.data_mc_with_ratio(h_data, stack_mc, linewidth=1, yscale='linear',
    #                          xlabel=r"pDNN (mX=1000) [GeV]", equalwidth=True)
    
    # plot2.legend(ncols=4)
    # plot2.save('test2')
    chn = "ETau"
    infile = os.path.join(basepath_in, tag, chn, 'combined_outPlots.root')
    outdir = os.path.join(basepath_out, tag, chn)

    factory = PlotterFactory(infile, outdir)
    factory.produce_data_mc_with_ratio(chn, "res1b", "SR", year)

    # stack_mc_up = h.stack_mc(keys='.*_res1b_SR_pdnn_m1000_s0_hh_tes_DM0_up')
    # stack_mc_down = h.stack_mc(keys='.*_res1b_SR_pdnn_m1000_s0_hh_tes_DM0_down')
    # plot3 = Plotter(outdir, npads=2)
    # plot3.syst_shape_with_ratio(stack_mc, stack_mc_up, stack_mc_down,
    #                             label="TES DM0", linewidth=3, yscale='log',
    #                             xlabel=r"pDNN (mX=1000) [GeV]", equalwidth=True)    
    # plot3.save('test3', ncols_leg=1)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Make final plots for the analysis.')
    parser.add_argument('--tag', type=str, required=True, help='Tag')
    parser.add_argument('--year', type=str, required=True,
                        choices=['2016', '2016APV', '2017', '2018'], help='Year')
    args = parser.parse_args()

    makeFinalPlots(args.tag, args.year)
