# Coding: utf-8

_all_ = [ 'makeFinalPlots' ]

import os
import argparse
import multiprocessing
from functools import wraps

from utilsFinalPlots import Histograms, Plotter

class DNNParams:
    def __init__(self, mass, spin):
        self.mass = mass
        self.spin = spin

        self.masses = {"500", "800", "1000"}
        self.spins = {"0", "2"}        
        
    def label(self):
        return r"$pDNN\:(mX={})$".format(self.mass)

    def name(self):
        return "pdnn_m" + self.mass + "_s" + self.spin + "_hh"
    
class Params:
    def __init__(self, channel="TauTau", category="boostedL_pnet", region="SR", year="2018", rebin=1):
        self.years     = {"2016", "2016APV", "2017", "2018"}
        self.cat_res   = {"baseline", "res1b", "res2b", "ttbarCR", "dyCR", "dyCR_res1b", "dyCR_res2b"}
        self.cat_boost = {"baseline_boosted", "boostedL_pnet"}
        self.channels  = {"ETau", "MuTau", "TauTau", "MuMu"}
        self.regions   = {"SR", "SStight", "OSinviso", "SSinviso"}

        self.channel  = channel
        self.category = category
        self.region   = region
        self.year     = year
        self.rebin    = rebin

        self._sanity_checks()
        
        self.histo_name = '_'.join((self.category, self.region))
        self.name = self.histo_name + '_' + '_'.join((self.channel, self.year))
        
    def __repr__(self):
        return "Params(channel={}, category={}, region={}, year={})".format(self.channel, self.category,
                                                                            self.region, self.year)

    def __str__(self):
        return "Channel={}, Category={}, Region={}, Year={}".format(self.channel, self.category,
                                                                    self.region, self.year)

    def _sanity_checks(self) -> None:
        """Sanity checks for the input channel and category."""
        assert self.channel in self.channels, f"Invalid channel: {self.channel}"
        assert self.category in self.cat_res.union(self.cat_boost), f"Invalid category: {self.category}"
        assert self.region in self.regions, f"Invalid region: {self.region}"
        assert self.year in self.years, f"Invalid year: {self.year}"

class PlotterFactory:
    def __init__(self, infile, outdir, multithreading=True):
        self.outdir = outdir
        self.multithreading = multithreading
        self.hists = Histograms(infile)

        self.dnn_scale = {
            "320":  {"ETau":   {"res1b": 180, "res2b": 130., "ttbarCR": 130., "dyCR": 130., "dyCR_res1b": 130., "dyCR_res2b": 130., "boostedL_pnet": 50.},
					 "MuTau":  {"res1b": 180, "res2b": 130., "ttbarCR": 130., "dyCR": 130., "dyCR_res1b": 130., "dyCR_res2b": 130., "boostedL_pnet": 50.},
					 "MuMu":   {"res1b": 180, "res2b": 130., "ttbarCR": 130., "dyCR": 130., "dyCR_res1b": 130., "dyCR_res2b": 130., "boostedL_pnet": 50.},
					 "TauTau": {"res1b": 20,  "res2b": 3.,	 "ttbarCR": 3.,   "dyCR": 3.,	 "dyCR_res1b": 3.,	 "dyCR_res2b": 3.,	 "boostedL_pnet": 35.}},
			"400":	{"ETau":   {"res1b": 180, "res2b": 130., "ttbarCR": 130., "dyCR": 130., "dyCR_res1b": 130., "dyCR_res2b": 130., "boostedL_pnet": 50.},
					 "MuTau":  {"res1b": 180, "res2b": 130., "ttbarCR": 130., "dyCR": 130., "dyCR_res1b": 130., "dyCR_res2b": 130., "boostedL_pnet": 50.},
					 "MuMu":   {"res1b": 180, "res2b": 130., "ttbarCR": 130., "dyCR": 130., "dyCR_res1b": 130., "dyCR_res2b": 130., "boostedL_pnet": 50.},
					 "TauTau": {"res1b": 20,  "res2b": 3.,	 "ttbarCR": 3.,   "dyCR": 3.,	 "dyCR_res1b": 3.,	 "dyCR_res2b": 3.,	 "boostedL_pnet": 35.}},
			"2E3":	{"ETau":   {"res1b": 180, "res2b": 130., "ttbarCR": 130., "dyCR": 130., "dyCR_res1b": 130., "dyCR_res2b": 130., "boostedL_pnet": 10.},
					 "MuTau":  {"res1b": 180, "res2b": 130., "ttbarCR": 130., "dyCR": 130., "dyCR_res1b": 130., "dyCR_res2b": 130., "boostedL_pnet": 10.},
					 "MuMu":   {"res1b": 180, "res2b": 130., "ttbarCR": 130., "dyCR": 130., "dyCR_res1b": 130., "dyCR_res2b": 130., "boostedL_pnet": 10.},
					 "TauTau": {"res1b": 10,  "res2b": 3.,	 "ttbarCR": 3.,   "dyCR": 3.,	 "dyCR_res1b": 3.,	 "dyCR_res2b": 3.,	 "boostedL_pnet": 5.}},
			"550":	{"ETau":   {"res1b": 180, "res2b": 130., "ttbarCR": 2E3,  "dyCR": 2E3,	 "dyCR_res1b": 2E3,	 "dyCR_res2b": 2E3,	 "boostedL_pnet": 10.},
					 "MuTau":  {"res1b": 180, "res2b": 130., "ttbarCR": 2E3,  "dyCR": 2E3,	 "dyCR_res1b": 2E3,	 "dyCR_res2b": 2E3,	 "boostedL_pnet": 10.},
					 "MuMu":   {"res1b": 180, "res2b": 130., "ttbarCR": 2E3,  "dyCR": 2E3,	 "dyCR_res1b": 2E3,	 "dyCR_res2b": 2E3,	 "boostedL_pnet": 10.},
					 "TauTau": {"res1b": 10,  "res2b": 3.,	 "ttbarCR": 3.,   "dyCR": 3.,	 "dyCR_res1b": 3.,	 "dyCR_res2b": 3.,	 "boostedL_pnet": 5.}},
			"700":	{"ETau":   {"res1b": 180, "res2b": 130., "ttbarCR": 130., "dyCR": 130., "dyCR_res1b": 130., "dyCR_res2b": 130., "boostedL_pnet": 0.6},
					 "MuTau":  {"res1b": 180, "res2b": 130., "ttbarCR": 130., "dyCR": 130., "dyCR_res1b": 130., "dyCR_res2b": 130., "boostedL_pnet": 0.6},
					 "MuMu":   {"res1b": 180, "res2b": 130., "ttbarCR": 130., "dyCR": 130., "dyCR_res1b": 130., "dyCR_res2b": 130., "boostedL_pnet": 0.6},					  
					 "TauTau": {"res1b": 10,  "res2b": 2.5,	 "ttbarCR": 2.5,  "dyCR": 2.5,	 "dyCR_res1b": 2.5,	 "dyCR_res2b": 2.5,	 "boostedL_pnet": 0.3}},
			"800":	{"ETau":   {"res1b": 180, "res2b": 130., "ttbarCR": 2E3,  "dyCR": 2E3,	 "dyCR_res1b": 2E3,	 "dyCR_res2b": 2E3,	 "boostedL_pnet": 0.2},
					 "MuTau":  {"res1b": 180, "res2b": 130., "ttbarCR": 2E3,  "dyCR": 2E3,	 "dyCR_res1b": 2E3,	 "dyCR_res2b": 2E3,	 "boostedL_pnet": 0.2},
					 "MuMu":   {"res1b": 180, "res2b": 130., "ttbarCR": 2E3,  "dyCR": 2E3,	 "dyCR_res1b": 2E3,	 "dyCR_res2b": 2E3,	 "boostedL_pnet": 0.2},					  
					 "TauTau": {"res1b": 10,  "res2b": 3.,	 "ttbarCR": 3.,   "dyCR": 3.,	 "dyCR_res1b": 3.,	 "dyCR_res2b": 3.,	 "boostedL_pnet": 0.1}},
			"1000": {"ETau":   {"res1b": 180, "res2b": 130., "ttbarCR": 180., "dyCR": 180., "dyCR_res1b": 180., "dyCR_res2b": 180., "boostedL_pnet": 0.2},
					 "MuTau":  {"res1b": 180, "res2b": 130., "ttbarCR": 180., "dyCR": 180., "dyCR_res1b": 180., "dyCR_res2b": 180., "boostedL_pnet": 0.2},
					 "MuMu":   {"res1b": 180, "res2b": 130., "ttbarCR": 180., "dyCR": 180., "dyCR_res1b": 180., "dyCR_res2b": 180., "boostedL_pnet": 0.2},					  
					 "TauTau": {"res1b": 10,  "res2b": 3.,	 "ttbarCR": 3.,   "dyCR": 3.,	 "dyCR_res1b": 3.,	 "dyCR_res2b": 3.,	 "boostedL_pnet": 0.1}},
			"1500": {"ETau":   {"res1b": 180, "res2b": 130., "ttbarCR": 2E3,  "dyCR": 2E3,	 "dyCR_res1b": 2E3,	 "dyCR_res2b": 2E3,	 "boostedL_pnet": 0.1},
					 "MuTau":  {"res1b": 180, "res2b": 130., "ttbarCR": 2E3,  "dyCR": 2E3,	 "dyCR_res1b": 2E3,	 "dyCR_res2b": 2E3,	 "boostedL_pnet": 0.1},
					 "MuMu":   {"res1b": 180, "res2b": 130., "ttbarCR": 2E3,  "dyCR": 2E3,	 "dyCR_res1b": 2E3,	 "dyCR_res2b": 2E3,	 "boostedL_pnet": 0.2},					  
					 "TauTau": {"res1b": 20,  "res2b": 3.,	 "ttbarCR": 3.,   "dyCR": 3.,	 "dyCR_res1b": 3.,	 "dyCR_res2b": 3.,	 "boostedL_pnet": 0.05}},
			"2000": {"ETau":   {"res1b": 180, "res2b": 130., "ttbarCR": 130., "dyCR": 130., "dyCR_res1b": 130., "dyCR_res2b": 130., "boostedL_pnet": 0.2},
					 "MuTau":  {"res1b": 180, "res2b": 130., "ttbarCR": 130., "dyCR": 130., "dyCR_res1b": 130., "dyCR_res2b": 130., "boostedL_pnet": 0.2},
					 "MuMu":   {"res1b": 180, "res2b": 130., "ttbarCR": 130., "dyCR": 130., "dyCR_res1b": 130., "dyCR_res2b": 130., "boostedL_pnet": 0.2},					  
					 "TauTau": {"res1b": 130, "res2b": 3.,	 "ttbarCR": 3.,   "dyCR": 3.,	 "dyCR_res1b": 3.,	 "dyCR_res2b": 3.,	 "boostedL_pnet": 0.1}}
		}																 

        # name, sigscale per channel
        self.variables = {
            "dau1_pt"              : (r"$pT^{\,\mathrm{lepton \#1}}\;[GeV]$",              (90, 50, 20, 5000)),
			"dau2_pt"			   : (r"$pT^{\,\mathrm{lepton \#2}}\;[GeV]$",			  (90, 50, 9, 5000)),
			"bjet1_pt"			   : (r"$pT^{\,\mathrm{jet \#1}}\;[GeV]$",			  (90, 90, 9, 10000)),
			"bjet2_pt"			   : (r"$pT^{\,\mathrm{jet \#2}}\;[GeV]$",			  (90, 90, 9, 10000)),
			"dau1_eta"			   : (r"$\eta^{\,\mathrm{lepton \#1}}$",					  (40, 40, 2, 4000)),
			"dau2_eta"			   : (r"$\eta^{\,\mathrm{lepton \#2}}$",					  (40, 40, 2, 4000)),
			"bjet1_eta"			   : (r"$\eta^{\,\mathrm{jet \#1}}$",				  (40, 40, 4, 4000)),
			"bjet2_eta"			   : (r"$\eta^{\,\mathrm{jet \#2}}$",				  (40, 40, 4, 4000)),
			"tauH_mass"			   : (r"$m_{H\rightarrow\tau\tau}\;[GeV]$",			  (30, 30, 2, 10000)),
			"tauH_pt"			   : (r"$pT_{H\rightarrow\tau\tau}\;[GeV]$",		  (30, 30, 2, 10000)),
			"tauH_eta"			   : (r"$\eta_{H\rightarrow\tau\tau} [GeV]$",		  (30, 30, 9, 10000)),
			"bH_mass"			   : (r"$m_{H\rightarrow bb}\;[GeV]$",			  (10, 10, 2, 1000)),
			"bH_pt"				   : (r"$pT_{H\rightarrow bb}\;[GeV]$",			  (30, 30, 2, 10000)),
			"ditau_deltaR"		   : (r"$\Delta R(\tau\tau)$",		  (10, 10, 2, 1000)),
			"dib_deltaR"		   : (r"$\Delta R(bb)$",				  (10, 10, 2, 1000)),
			"HH_deltaR"			   : (r"$\Delta R(HH)$",				  (30, 30, 2, 7000)),
			"njets"				   : ("#Jets",					  (90, 90, 5, 10000)),
			"met_et"			   : (r"$MET [GeV]$",				  (10, 10, 1, 1000)),
			"met_phi"			   : (r"$MET-\phi$",				  (10, 10, 1, 1000)),
			"metnomu_et"		   : (r"$MET_{no\mu}\;[GeV]$",			  (20, 20, 2, 2000)),
			"metnomu_phi"		   : (r"$MET_{no\mu}-\phi$",			  (10, 10, 1, 5000)),
			"dau1_dxy"			   : (r"$d^{\,\mathrm{lepton \#1}}_{xy}$",					  (10, 10, 1, 5000)),
			"dau1_dz"			   : (r"$d^{\,\mathrm{lepton \#1}}_{z}$",					  (10, 10, 1, 5000)),
			"dau2_dxy"			   : (r"$d^{\,\mathrm{lepton \#2}}_{xy}$",					  (10, 10, 1, 4000)),
			"dau2_dz"			   : (r"$d^{\,\mathrm{lepton \#2}}_{z}$",					  (10, 10, 1, 4000)),
			"METx"				   : (r"$MET_{x} [GeV]$",			  (10, 10, 1, 4000)),
			"METy"				   : (r"$MET_{y} [GeV]$",			  (10, 10, 1, 4000)),
			# "met_cov00"			   : (r"$Cov(MET)_{00}$",			  (10, 10, 1, 4000)),
			# "met_cov01"			   : (r"$Cov(MET)_{01}$",			  (10, 10, 1, 4000)),
			# "met_cov11"			   : (r"$Cov(MET)_{11}$",			  (10, 10, 1, 4000)),
			"bjet1_bID_deepFlavor" : (r"$Deep\:Flavour^{\,\mathrm{jet \#1}}$",		  (10, 10, 1, 4000)),
			"bjet2_bID_deepFlavor" : (r"$Deep\:Flavour^{\,\mathrm{jet \#2}}$",		  (10, 10, 1, 4000)),
			"bjet1_CvsB"		   : (r"$CvsB^{\,\mathrm{jet \#1}}$",				  (10, 10, 1, 4000)),
			"bjet1_CvsL"		   : (r"$CvsL^{\,\mathrm{jet \#1}}$",				  (10, 10, 1, 4000)),
			"bjet2_CvsB"		   : (r"$CvsB^{\,\mathrm{jet \#2}}$",				  (10, 10, 1, 4000)),
			"bjet2_CvsL"		   : (r"$CvsL^{\,\mathrm{jet \#2}}$",				  (10, 10, 1, 4000)),
			"bjet1_HHbtag"		   : (r"$HH-BTag^{\,\mathrm{jet \#1}}$",				  (10, 10, 1, 4000)),
			"bjet2_HHbtag"		   : (r"$HH-BTag^{\,\mathrm{jet \#2}}$",				  (10, 10, 1, 4000)),
			"tauH_SVFIT_mass"	   : (r"$m_{H\tau}^{SVFit}\;[GeV]$",	  (30, 30, 2, 10000)),
			"tauH_SVFIT_pt"		   : (r"$pT_{H\tau}^{SVFit}\;[GeV]$",  (10, 10, 2, 10000)),
			# "tauH_SVFIT_eta"	   : (r"$eta_{H\tau}^{SVFit} [GeV]$", (10, 10, 2, 10000)),
			# "HHbregrsvfit_m"	   : (r"$m_{HH}^{PNet}\;[GeV]$",		  (1, 1, 1, 150)),
			# "HHbregrsvfit_pt"	   : (r"$pT_{HH}^{PNet}\;[GeV]$",	  (30, 30, 10, 10000)),
			# "HHbregrsvfit_eta"	   : (r"$\eta_{HH}^{PNet}$",			  (40, 40, 2, 4000)),
			"HH_mass"			   : (r"$m_{HH}\:\:[GeV]$",			  (1, 1, 1, 150)),
			"HHKin_mass"		   : (r"$m^{Kin}_{HH}\:\:[GeV]$",			  (1, 1, 1, 150)),
        }

        self.systematics = {"tes_DM0", "tes_DM1", "tes_DM10", "tes_DM11"}
        
        self.logvariables = {"dau1_dxy", "dau1_dz", "dau2_dxy", "dau2_dz", "HH_mass", "HHKin_mass"}
        self.equalwidth = {"HH_mass", "HHKin_mass"}
    
    def data_mc_with_ratio_worker(self, variable, pars, pdnn=None) -> None:
        """
        Worker function for producing the data/mc plots with ratio for the given channel and category.
        """
        suffix = '_' + pars.histo_name + '_' + variable
        hdata = self.hists.hists(keys='data_obs'+suffix, rebin=pars.rebin)['data_obs'+suffix]
        
        backgrounds = ['DY'+suffix, 'TT'+suffix, 'other'+suffix, 'W'+suffix, 'H'+suffix]
        if pars.channel != "MuMu" and pars.region == "SR":
            backgrounds += ['QCD'+suffix]
        stackmc = self.hists.stack_mc(keys=backgrounds, order=backgrounds, rebin=pars.rebin)

        hsignals = []
        for m in {"550", "800", "1500"}:
            hsignals.append(self.hists.hists(keys='GGF_Radion'+m+suffix,
                                             scale=self.dnn_scale[m][pars.channel][pars.category],
                                             label="Sig "+m)['GGF_Radion'+m+suffix])

        p = Plotter(self.outdir, channel=pars.channel, cat=pars.category, region=pars.region, year=pars.year, npads=2)
        p.data_mc_with_ratio(stackmc=stackmc, hdata=hdata, 
                                    linewidth=5,
					                yscale='log' if variable in self.logvariables or 'pdnn' in variable else 'linear',
					                xlabel=self.variables[variable][0],
                                    equalwidth=variable in self.equalwidth)
        
        p.save('plot_' + pars.name + '_' + variable, ncols_leg=2)

    def data_mc_signal_worker(self, variable, pars, pdnn) -> None:
        """
        Worker function for producing the data/mc plots with ratio for the given channel and category.
        """
        suffix = '_' + pars.histo_name + '_' + variable
        log_check = variable in self.logvariables or 'pdnn' in variable

        processes_other = ('VVV', 'TTX', 'VV', 'ggH', 'EWK', 'TW', 'HH', 'qqH', 'WH', 'singleT', 'ttH', 'ZH')
        mc_groups = {'others': tuple(proc + suffix for proc in processes_other)}
        stackmc = self.hists.stack_mc(keys='.*'+suffix, groups=mc_groups,
                                      order=['TT'+suffix, 'DY'+suffix, 'W'+suffix, 'QCD'+suffix, 'others'])
        
        hsignal = self.hists.hists(keys='GGF_Radion'+pdnn.mass+suffix,
                                   scale=self.dnn_scale[pdnn.mass][pars.channel][pars.category],
                                   label="Sig "+pdnn.mass)['GGF_Radion'+pdnn.mass+suffix]

        p = Plotter(self.outdir, channel=pars.channel, cat=pars.category, region=pars.region, year=pars.year, npads=1)
        p.mc_signal(stackmc=stackmc, hsignals=hsignal, #hdata=hdata,
                    linewidth=3,
                    edgecolor="black",
					yscale='log' if log_check else 'linear',
					xlabel=self.variables[variable][0] if pdnn is None else pdnn.label(),
                    equalwidth=True)

        savename = 'signal_' + pars.name + '_' + variable + '_m' + pdnn.mass + '_s' + pdnn.spin
        if log_check:
            p.save(savename + '_log', ncols_leg=3)
        else:
            p.save(savename, ncols_leg=3)

    def data_mc_signal_with_ratio_worker(self, variable, pars, pdnn) -> None:
        """
        Worker function for producing the data/mc plots with ratio for the given channel and category.
        """
        suffix = '_' + pars.histo_name + '_' + variable

        hdata = self.hists.hists(keys='data_obs'+suffix, leglabel="Data")['data_obs'+suffix]

        processes_other = ('VVV', 'TTX', 'VV', 'ggH', 'EWK', 'TW', 'HH', 'qqH', 'WH', 'singleT', 'ttH', 'ZH')
        mc_groups = {'others': tuple(proc + suffix for proc in processes_other)}
        stackmc = self.hists.stack_mc(keys='.*'+suffix, groups=mc_groups,
                                      order=['TT'+suffix, 'DY'+suffix, 'W'+suffix, 'H'+suffix, 'others'])

        hsignal = self.hists.hists(keys='GGF_Radion'+pdnn.mass+suffix,
                                   scale=self.dnn_scale[pdnn.mass][pars.channel][pars.category],
                                   label="Sig "+pdnn.mass)['GGF_Radion'+pdnn.mass+suffix]

        p = Plotter(self.outdir, channel=pars.channel, cat=pars.category, region=region, year=pars.year, npads=1)
        p.mc_signal(stackmc=stackmc, hsignals=hsignal, hdata=hdata,
                    linewidth=3,
                    edgecolor="black",
					yscale='log' if variable in self.logvariables or 'pdnn' in variable else 'linear',
					xlabel=self.variables[variable][0] if pdnn is None else pdnn.label(),
                    equalwidth=True)
        p.save('signal_' + pars.name + '_' + variable + '_m' + pdnn.mass + '_s' + pdnn.spin, ncols_leg=3)

    def produce(self, func, pars, pdnn=None):
        avars = self.variables if pdnn is None else [pdnn.name(),]

        if self.multithreading:
            num_workers = multiprocessing.cpu_count()-1
            with multiprocessing.Pool(processes=num_workers) as pool:
                func_args = [(v, pars, pdnn) for v in avars]
                pool.starmap(func, func_args)
        else:
            for v in avars:
                func(variable=v, pars=pars, pdnn=pdnn)

    def _produce_syst_shape_with_ratio_worker(self, syst, mass, spin, pars) -> None:
        suffix = '_' + pars.histo_name + '_' + "pdnn_m" + mass + "_s" + spin + "_hh"
        stack_mc_nom  = self.hists.stack_mc(keys='.*' + suffix)
        stack_mc_up   = self.hists.stack_mc(keys='.*' + suffix + "_" + syst + '_up')
        stack_mc_down = self.hists.stack_mc(keys='.*' + suffix + "_" + syst + '_down')
        p = Plotter(self.outdir, npads=2)
        p.syst_shape_with_ratio(stack_mc_nom, stack_mc_up, stack_mc_down,
                                label="TES DM0",
                                linewidth=3,
                                yscale='linear',
                                xlabel="pDNN (mX={}) [GeV]".format(mass), equalwidth=True)
        p.save('plot_' + pars.name + '_pdnn_m' + mass + "_s" + spin + "_hh_" + syst)

    def produce_syst_shape_with_ratio(self, syst, mass, spin, pars) -> None:
        if self.multithreading:
            num_workers = multiprocessing.cpu_count()-1
            with multiprocessing.Pool(processes=num_workers) as pool:
                func_args = [(syst, mass, spin, pars) for syst in self.systematics]
                pool.starmap(self._produce_syst_shape_with_ratio_worker, func_args)
        else:
            for syst in self.systematics:
                self._produce_syst_shape_with_ratio_worker(syst=syst, mass=mass, spin=spin, pars=pars)
         

def dnn_parallel(mass, spin, tag, chn, cat, pars):
    basepath_in = "/data_CMS/cms/alves/HHresonant_hist/"
    basepath_out = "/eos/home-b/bfontana/www/HH_Plots/"

    pdnn_params = DNNParams(mass, spin)
    infile = os.path.join(basepath_in, tag, chn, "Spin"+spin+"_Mass"+mass, 'combined_outLimits.root')
    outdir = os.path.join(basepath_out, tag, chn, cat)
    
    factory = PlotterFactory(infile, outdir, multithreading=False)
    # factory.produce(factory.data_mc_signal_with_ratoi_worker, pars=pars, pdnn=pdnn_params)
    factory.produce(factory.data_mc_signal_worker, pars=pars, pdnn=pdnn_params)

def makeFinalPlots(tag, year, channels, categories, region, rebin, pdnn, singlethreaded=False):
    basepath_in = "/data_CMS/cms/alves/HHresonant_hist/" # "/data_CMS/cms/alves/HHresonant_hist/"
    basepath_out = "./Temp/" # "/eos/home-b/bfontana/www/HH_Plots/"

    for chn in channels:
        for cat in categories:
            outdir = os.path.join(basepath_out, tag, chn, cat)
            if not os.path.exists(outdir):
                os.makedirs(outdir)
                
            print("Running for channel: {}, category: {}".format(chn, cat))
            pars = Params(channel=chn, category=cat, region=region, year=year, rebin=rebin)
            
            if pdnn:
                masses = {"320",}#"400", "500", "700", "800", "1000", "1500", "2000"}
                spins = {"0",}
                if singlethreaded:
                    for mass in masses:
                        for spin in spins:
                            dnn_parallel(mass, spin, tag, chn, cat, pars)
                else:
                    num_workers = multiprocessing.cpu_count()-1
                    with multiprocessing.Pool(processes=num_workers) as pool:
                        func_args = [(mass, spin, tag, chn, cat, pars) for mass in masses for spin in spins]
                        pool.starmap(dnn_parallel, func_args)                
                        
            else:
                infile = os.path.join(basepath_in, tag, chn, 'combined_outPlots.root')

                factory = PlotterFactory(infile, outdir, multithreading=not args.singlethreaded)
                factory.produce(factory.data_mc_with_ratio_worker, pars=pars)
                
    # mass, spin = "500", "0"
    # sm_str = "Spin" + spin + "_Mass" + mass
    # for chn in channels:
    #     for cat in categories:
    #         infile = os.path.join(basepath_in, tag, chn, sm_str, 'combined_outLimits.root')
    #         print(infile)
            
    #         outdir = os.path.join(basepath_out, tag, chn, sm_str, cat)
    #         pars = Params(channel=chn, category=cat, region="SR", year=year)
    #         factory = PlotterFactory(infile, outdir, multithreading=not args.singlethreaded)
    #         factory.produce_syst_shape_with_ratio(syst="tes_DM0", mass=mass, spin=spin, pars=pars)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Make final plots for the analysis.')
    parser.add_argument('--tag', type=str, required=True, help='Tag')
    parser.add_argument('--year', type=str, required=True,
                        choices=['2016', '2016APV', '2017', '2018'], help='Year')
    parser.add_argument('--region', type=str, required=False, default="SR",
                        choices=['SR', 'SSinviso', 'OSinviso', 'SStight'], help='Region')
    parser.add_argument('--channels', type=str, required=True, nargs='+',
                        default=("MuTau", "ETau", "TauTau"),
                        choices=['ETau', 'MuTau', 'TauTau', 'MuMu'], help='Channels')
    parser.add_argument('--categories', type=str, required=True, nargs='+', help='Categories',
                        default=("res1b", "res2b", "boostedL_pnet"),
                        choices=["baseline", "res1b", "res2b", "boostedL_pnet", "dyCR_res1b", "dyCR_res2b", "ttbarCR",])
    parser.add_argument('--rebin', type=int, required=False, help="Rebin factor, leading to less bins.", default=1)
    parser.add_argument('--singlethreaded', action='store_true', help='Disable multithreading; useful for debugging.')
    parser.add_argument('--pdnn', action='store_true', help='Plot DNN variables')
    args = parser.parse_args()

    makeFinalPlots(args.tag, args.year, args.channels, args.categories, args.region, args.rebin, args.pdnn, args.singlethreaded)
