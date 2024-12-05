# Coding: utf-8

_all_ = [ 'create_configs', 'Params' ]

import os
import argparse
import json

class Params:
    """Convenience class for parameter storing and manipulation."""
    def __init__(self, year, onlymet=False, detailed=False):
        self.year = year
        
        self.spin_for_plots = '0'
        self._define_parameters(detailed, onlymet)
        self._reduce_to_year()

    def _add_met_to_data(self):
        """Add MET datasets to all years. Common to every channel per year."""
        channels = ("ETau", "MuTau", "TauTau", "MuMu")
        return {
            "UL16": {k: ["DMETF", "DMETG", "DMETH"] for k in channels},
            "UL16APV": {k: ["DMETB", "DMETC", "DMETD", "DMETE", "DMETF"] for k in channels},
            "UL17": {k: ["MET_Run2017B", "MET_Run2017C", "MET_Run2017D", "MET_Run2017E", "MET_Run2017F"] for k in channels},
            "UL18": {k: ["DMETA", "DMETB", "DMETC", "DMETD"] for k in channels},
        }

    def _add_non_met_data(self):
        """Add non-MET datasets to all years."""
        return {
            "UL16": {
                "ETau": ["DsingleEleF", "DsingleEleG", "DsingleEleH"],
                "MuTau": ["DsingleMuF", "DsingleMuG", "DsingleMuH"],
                "TauTau": ["DTauF", "DTauG", "DTauH"],
                "MuMu": ["DsingleMuF", "DsingleMuG", "DsingleMuH"],
            },
            "UL16APV": {
                "ETau": ["DsingleEleB", "DsingleEleC", "DsingleEleD", "DsingleEleE", "DsingleEleF"],
                "MuTau": ["DsingleMuB", "DsingleMuC", "DsingleMuD", "DsingleMuE", "DsingleMuF"],
                "TauTau": ["DTauB", "DTauC", "DTauD", "DTauE", "DTauF"],
                "MuMu": ["DsingleMuB", "DsingleMuC", "DsingleMuD", "DsingleMuE", "DsingleMuF"],
            },
            "UL17": {
                "ETau": ["EGamma_Run2017B", "EGamma_Run2017C", "EGamma_Run2017D", "EGamma_Run2017E", "EGamma_Run2017F"],
                "MuTau": ["SingleMuon_Run2017B", "SingleMuon_Run2017C", "SingleMuon_Run2017D", "SingleMuon_Run2017E", "SingleMuon_Run2017F"],
                "TauTau": ["Tau_Run2017B", "Tau_Run2017C", "Tau_Run2017D", "Tau_Run2017E", "Tau_Run2017F"],
                "MuMu": ["SingleMuon_Run2017B", "SingleMuon_Run2017C", "SingleMuon_Run2017D", "SingleMuon_Run2017E", "SingleMuon_Run2017F"],
            },
            "UL18": {
                "ETau":   ["DsingleEleA", "DsingleEleB", "DsingleEleC", "DsingleEleD"],
                "MuTau":  ["DsingleMuA", "DsingleMuB", "DsingleMuC", "DsingleMuD"],
                "TauTau": ["DTauA", "DTauB", "DTauC", "DTauD"],
                "MuMu":   ["DsingleMuA", "DsingleMuB", "DsingleMuC", "DsingleMuD"],
            }
        }

    def _define_trigger_sf_systematics(self, channel):
        """Set channel-dependent trigger systematics."""
        if channel == "ETau":
            systs = ["trigSFeleUp:trigSF_ele_up", "trigSFeleDown:trigSF_ele_down"]
        elif channel== "MuTau":
            systs = ["trigSFmuUp:trigSF_mu_up"  , "trigSFmuDown:trigSF_mu_down"]
        elif channel== "TauTau":
            systs = []

        systs += ["trigSFTauDM0Up:trigSF_DM0_up"    , "trigSFTauDM0Down:trigSF_DM0_down",
                  "trigSFTauDM1Up:trigSF_DM1_up"    , "trigSFTauDM1Down:trigSF_DM1_down",
                  "trigSFTauDM10Up:trigSF_DM10_up"  , "trigSFTauDM10Down:trigSF_DM10_down",
                  "trigSFTauDM11Up:trigSF_DM11_up"  , "trigSFTauDM11Down:trigSF_DM11_down",
                  "trigSF_met_up:trigSF_met_up"  , "trigSF_met_down:trigSF_met_down",
                  "trigSF_stau_up:trigSF_stau_up", "trigSF_stau_down:trigSF_stau_down"]
        return ', '.join((systs))
        
    def _define_parameters(self, detailed, onlymet):
        """Defines the values of all parameters."""
        self.lumi = {"UL16": "16800", "UL16APV": "19500", "UL17": "41529", "UL18": "59741"}

        if onlymet:
            self.data = self._add_met_to_data()
        else:
            self.data = self._add_non_met_data()
            met_datasets = self._add_met_to_data()
            for year in self.data.keys():
                for channel in self.data[year].keys():
                    self.data[year][channel].extend(met_datasets[year][channel])

        self.masses = ("250", "260", "270", "280", "300", "320", "350", "400", "450", "500", "550", "600", "650",
                       "700", "750", "800", "850", "900", "1000", "1250", "1500", "1750", "2000", "2500", "3000")
 
        self.signals = {"0": lambda mass : "GGF_Radion" + mass,
                        "2": lambda mass : "GGF_Graviton" + mass}
        self.spins = self.signals.keys()
        
        self.bckg_limits = {
            "DY": ("DY_NLO_incl_stitch", "DY_NLO_0J", "DY_NLO_1J", "DY_NLO_2J",
                   "DY_NLO_Pt0To50", "DY_NLO_Pt50To100", "DY_NLO_Pt100To250", "DY_NLO_Pt250To400", "DY_NLO_Pt400To650", "DY_NLO_Pt650ToInf"),
            "TT": ("TTfullyHad", "TTfullyLep", "TTsemiLep"),
            "W": ("WJets_HT_0_70", "WJets_HT_70_100", "WJets_HT_100_200", "WJets_HT_200_400", "WJets_HT_400_600",
                  "WJets_HT_600_800", "WJets_HT_800_1200", "WJets_HT_1200_2500", "WJets_HT_2500_Inf"),
            "WH": ("WplusHTauTau", "WminusHTauTau"),
            "ZH": ("ZH_HTauTau", "ZH_HBB_ZLL", "ZH_HBB_ZQQ"),
            "ttH": ("ttHJetToTauTau", "ttHJetTononBB", "ttHJetToBB"),
            "HH": ("GGHH_SM",
                   # "VBF_SM"
                   ),
            "EWK": ("EWKWMinus2Jets_WToLNu", "EWKWPlus2Jets_WToLNu", "EWKZ2Jets_ZToLL"),
            "TW": ("TWtop", "TWantitop"),
            "singleT": ("singleTop_top", "singleTop_antitop"),
            "VV": ("WW", "WZ", "ZZ"),
            "TTX": ("TTWW", "TTWZ", "TTZZ", "TTWJetsToQQ", "TTWJetsToLNu", "TTZToQQ", "TTZToLLNuNu", "TTWH", "TTZH"),
            "ggH": ("ggHTauTau",),
            "qqH": ("VBFHTauTau",),
            "VVV": ("WWW", "WWZ", "WZZ", "ZZZ"),
        }
            
        if detailed:
            self.bckg_plots = self.bckg_limits
        else:
            self.bckg_plots = {
                "DY": ("DY_NLO_incl_stitch", "DY_NLO_0J", "DY_NLO_1J", "DY_NLO_2J",
                       "DY_NLO_Pt0To50", "DY_NLO_Pt50To100", "DY_NLO_Pt100To250", "DY_NLO_Pt250To400", "DY_NLO_Pt400To650", "DY_NLO_Pt650ToInf"),
                "TT": ("TTfullyHad", "TTfullyLep", "TTsemiLep"),
                "W": ("WJets_HT_0_70", "WJets_HT_70_100", "WJets_HT_100_200", "WJets_HT_200_400", "WJets_HT_400_600",
                      "WJets_HT_600_800", "WJets_HT_800_1200", "WJets_HT_1200_2500", "WJets_HT_2500_Inf"),
                "H": ("ZH_HTauTau", "ZH_HBB_ZLL", "ZH_HBB_ZQQ", "WplusHTauTau", "WminusHTauTau", "ttHJetToTauTau", "ttHJetTononBB", "ttHJetToBB"),
                "other": ("EWKWMinus2Jets_WToLNu", "EWKWPlus2Jets_WToLNu", "EWKZ2Jets_ZToLL",
                          "TWtop", "TWantitop",
                          "singleTop_top", "singleTop_antitop",
                          "WW", "WZ", "ZZ",
                          "TTWW", "TTWZ", "TTZZ", "TTWJetsToQQ", "TTWJetsToLNu", "TTZToQQ", "TTZToLLNuNu",
                          "TTWH", "TTZH",
                          "WWW", "WWZ", "WZZ", "ZZZ",
                          "VBFHTauTau", "ggHTauTau",
                          "GGHH_SM"),
            }

        self.base_selections = ["baseline", "baseline_boosted"]
        _selections = ["res1b", "res2b", "boostedL_pnet"]
        self.selections = {"ETau": _selections,
                           "MuTau": _selections + ["ttbarCR"],
                           "TauTau": _selections + ["ttbarCR"],
                           "MuMu": _selections + ["dyCR", "dyCR_res1b", "dyCR_res2b"]}

        self.regions = {
            "ETau":
            {"SR": "isOS != 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 5",
             "SStight": "isOS == 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 5",
             "OSinviso": "isOS != 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 1 && dau2_deepTauVsJet < 5",
             "SSinviso": "isOS == 0 && dau1_eleMVAiso == 1 && dau2_deepTauVsJet >= 1 && dau2_deepTauVsJet < 5"
             },
            "MuTau":
            {"SR": "isOS != 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 5",
             "SStight": "isOS == 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 5",
             "OSinviso": "isOS != 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 1 && dau2_deepTauVsJet < 5",
             "SSinviso": "isOS == 0 && dau1_iso < 0.15 && dau2_deepTauVsJet >= 1 && dau2_deepTauVsJet < 5"
             },
            "TauTau":            
            {"SR": "isOS != 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 5",
             "SStight": "isOS == 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 5",
             "OSinviso": "isOS != 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 1 && dau2_deepTauVsJet < 5",
             "SSinviso": "isOS == 0 && dau1_deepTauVsJet >= 5 && dau2_deepTauVsJet >= 1 && dau2_deepTauVsJet < 5"
             },
            "MuMu":
            {"SR": "isOS != 0 && dau1_iso < 0.15 && dau2_iso < 0.15",
             },
        }
        
        self.qcd = '\n'.join((
            "[pp_QCD]",
            "QCDname      = QCD",
            "SR           = SR",
            "yieldSB      = OSinviso",
            "shapeSB      = OSinviso",
            "SBtoSRfactor = 1",
            "doFitIf      = False",
            "fitFunc      = [0] + [1]*x",
            "regionC      = SStight",
            "regionD      = SSinviso",
            "doUnc        = False",
            "#classSBtoSR  = 0.074",
            ))
            
        self.dnn_systematics = (
            # nominal
            "nominal",
            # tau energy scale
            "tes_DM0_up",
            "tes_DM0_down", "tes_DM1_up", "tes_DM1_down",          
            "tes_DM10_up", "tes_DM10_down", "tes_DM11_up", "tes_DM11_down",
            # electron energy scale
            "ees_DM0_up", "ees_DM0_down", "ees_DM1_up", "ees_DM1_down",
            # muon energy scale
            "mes_up", "mes_down",
            # jet energy scale (reduced set)
            "jes_1_up", "jes_1_down", "jes_2_up", "jes_2_down", "jes_3_up", "jes_3_down",
            "jes_4_up", "jes_4_down", "jes_5_up", "jes_5_down", "jes_6_up", "jes_6_down",
            "jes_7_up", "jes_7_down", "jes_8_up", "jes_8_down", "jes_9_up", "jes_9_down",
            "jes_10_up", "jes_10_down", "jes_11_up", "jes_11_down"
        )

        self.event_systematics = lambda channel : (
            '\n'.join((
                "dauSFs = " + ', '.join((
                    #"IdFakeSF_deep_2d = " + ', '.join((                        # ------> it does not exist anymore in the TTree

                    # "tauid_2d_stat0_up:dauSFs_tauid_2d_stat0_up",
                    # "tauid_2d_stat0_down:dauSFs_tauid_2d_stat0_down",
                    # "tauid_2d_stat1_up:dauSFs_tauid_2d_stat1_up",
                    # "tauid_2d_stat1_down:dauSFs_tauid_2d_stat1_down",
                    "tauid_2d_systcorrdmeras_up:dauSFs_tauid_2d_systcorrdmeras_up",
                    "tauid_2d_systcorrdmeras_down:dauSFs_tauid_2d_systcorrdmeras_down",
                    "tauid_2d_systcorrdmuncorreras_up:dauSFs_tauid_2d_systcorrdmuncorreras_up",
                    "tauid_2d_systcorrdmuncorreras_down:dauSFs_tauid_2d_systcorrdmuncorreras_down",
                    # "tauid_2d_systuncorrdmeras_up:dauSFs_tauid_2d_systuncorrdmeras_up",
                    # "tauid_2d_systuncorrdmeras_down:dauSFs_tauid_2d_systuncorrdmeras_down",
                    "tauid_2d_systcorrerasgt140_up:dauSFs_tauid_2d_systcorrerasgt140_up",
                    "tauid_2d_systcorrerasgt140_down:dauSFs_tauid_2d_systcorrerasgt140_down",
                    # "tauid_2d_statgt140_up:dauSFs_tauid_2d_statgt140_up",
                    # "tauid_2d_statgt140_down:dauSFs_tauid_2d_statgt140_down",
                    # "tauid_2d_extrapgt140_up:dauSFs_tauid_2d_extrapgt140_up",
                    # "tauid_2d_extrapgt140_down:dauSFs_tauid_2d_extrapgt140_down",

                    "etauFR_barrelUp:dauSFs_etauFR_barrel_up",
                    "etauFR_barrelDown:dauSFs_etauFR_barrel_down",
                    "etauFR_endcapUp:dauSFs_etauFR_endcap_up",
                    "etauFR_endcapDown:dauSFs_etauFR_endcap_down",
 
                    "mutauFR_etaLt0p4Up:dauSFs_mutauFR_etaLt0p4_up",
                    "mutauFR_etaLt0p4Down:dauSFs_mutauFR_etaLt0p4_down",
                    "mutauFR_eta0p4to0p8Up:dauSFs_mutauFR_eta0p4to0p8_up",
                    "mutauFR_eta0p4to0p8Down:dauSFs_mutauFR_eta0p4to0p8_down",
                    "mutauFR_eta0p8to1p2Up:dauSFs_mutauFR_eta0p8to1p2_up",
                    "mutauFR_eta0p8to1p2Down:dauSFs_mutauFR_eta0p8to1p2_down",
                    "mutauFR_eta1p2to1p7Up:dauSFs_mutauFR_eta1p2to1p7_up",
                    "mutauFR_eta1p2to1p7Down:dauSFs_mutauFR_eta1p2to1p7_down",
                    "mutauFR_etaGt1p7Up:dauSFs_mutauFR_etaGt1p7_up",
                    "mutauFR_etaGt1p7Down:dauSFs_mutauFR_etaGt1p7_down",
                )),
                "trigSF = " + self._define_trigger_sf_systematics(channel),
                "PUjetID_SF = " + ', '.join((
                    "PUjetIDSFUp:PUjetID_SF_up",
                    "PUjetIDSFDown:PUjetID_SF_down")),
                "bTagweightReshape = " + ', '.join((
                    "bTagweightReshapeLFUp:bTagweightReshape_lf_up",
                    "bTagweightReshapeHFUp:bTagweightReshape_hf_up",
                    "bTagweightReshapeHFSTATS1Up:bTagweightReshape_hfstats1_up",
                    "bTagweightReshapeHFSTATS2Up:bTagweightReshape_hfstats2_up",
                    "bTagweightReshapeLFSTATS1Up:bTagweightReshape_lfstats1_up",
                    "bTagweightReshapeLFSTATS2Up:bTagweightReshape_lfstats2_up",
                    "bTagweightReshapeCFERR1Up:bTagweightReshape_cferr1_up",
                    "bTagweightReshapeCFERR2Up:bTagweightReshape_cferr2_up",
                    "bTagweightReshapeLFDown:bTagweightReshape_lf_down",
                    "bTagweightReshapeHFDown:bTagweightReshape_hf_down",
                    "bTagweightReshapeHFSTATS1Down:bTagweightReshape_hfstats1_down",
                    "bTagweightReshapeHFSTATS2Down:bTagweightReshape_hfstats2_down",
                    "bTagweightReshapeLFSTATS1Down:bTagweightReshape_lfstats1_down",
                    "bTagweightReshapeLFSTATS2Down:bTagweightReshape_lfstats2_down",
                    "bTagweightReshapeCFERR1Down:bTagweightReshape_cferr1_down",
                    "bTagweightReshapeCFERR2Down:bTagweightReshape_cferr2_down",
                ))
            ))
        )

        self.dnn_heads = ("hh",) # "tt"
        self.dnn_scheme = "pdnn_m{}_s{}_{}"

    def _reduce_to_year(self):
        self.lumi = self.lumi[self.year]
        self.data = self.data[self.year]

    def define_dnn_binning(self, variables, channel, spin='', mass=''):
        m_chn = {"ETau": "etau", "MuTau": "mutau", "TauTau": "tautau"}
        m_cat = {"res1b": "resolved1b", "res2b": "resolved2b", "boostedL_pnet": "boosted",
                 "ttbarCR": r"\bar{t}t CR",
                 "dyCR": "baseline DY CR", "dyCR_res1b": "res1b DY CR", "dyCR_res2b": "res2b DY CR"}
        m_year = {"UL16": "2016", "UL16APV": "2016APV", "UL17": "2017", "UL18": "2018"}

        bins = "\n[binning]\n\n"

        if self.is_for_limits(spin, mass):
            selections = self.selections[channel]
            if float(mass) < 320:
                selections = [s for s in selections if 'boosted' not in s]
        else:
            selections = self.base_selections.extend(self.selections[channel])

        #with open('scripts/edges_' + m_year[self.year] + '.json') as f:
        with open('scripts/bin_edges_flatsguarded10_2017.json') as f:
            d = json.load(f)
            for var in variables:
                # Dummy binning needed to have make the binning for different categories work properly.
                # These binnings will actually be overwritten by:  category:variable = 0.0, ....
                bins += var + " = 20, 0., 1.\n"

                for reg in self.regions[channel].keys():
                    for sel in selections:
                        if self.is_for_limits(spin, mass):
                            #key = '_'.join((m_chn[channel], m_cat[sel], 'spin', spin, 'mass', mass))
                            key = '_'.join(("2017", m_chn[channel], m_cat[sel], 'os_iso_', 's' + spin, '_m' + mass))
                        else:
                            key = '_'.join((m_chn[channel], m_cat[sel], 'spin', self.spin_for_plots, 'mass', mass))
                        bins += sel + '_' + reg + ':' + var + " = " + str(d[key])[1:-1] + '\n'
                bins += "\n"

        return bins
    
    def define_nondnn_binning(self):
        bin_map = {
	        "nbjetscand"                        : [20, 0, 20],
        	"njets"                             : [10, 0, 10],
        	"njets20"                           : [10, 0, 10],
        	"njets50"                           : [10, 0, 10],
        	"npv"                               : [50, 0, 50],
        	"npu"                               : [25, 0, 50],
        	"dau1_pt"                           : [20, 0, 200],
        	"dau1_eta"                          : [25, -2.5, 2.5],
        	"dau1_phi"                          : [25, -3.14159, 3.14159],
        	"dau1_iso"                          : [24, 0., 1.],
        	"dau1_dxy"                          : [60, -0.02, 0.02],
        	"dau1_dz"                           : [60, -0.1, 0.1],
        	"dau2_pt"                           : [20, 0, 200],
        	"dau2_iso"                          : [24, 0., 1.],
        	"dau2_eta"                          : [25, -2.5, 2.5],
        	"dau2_phi"                          : [25, -3.14159, 3.14159],
        	"dau2_dxy"                          : [100, -0.01, 0.01],
        	"dau2_dz"                           : [100, -0.1, 0.1],
            "dau1_deepTauVsMu"                  : [10, -0.5, 9.5],
            "dau2_deepTauVsMu"                  : [10, -0.5, 9.5],
            "dau1_deepTauVsJet"                 : [10, -0.5, 9.5],
            "dau2_deepTauVsJet"                 : [10, -0.5, 9.5],
            "dau1_deepTauVsEle"                 : [10, -0.5, 9.5],
            "dau2_deepTauVsEle"                 : [10, -0.5, 9.5],
            "met_et"                            : [40, 0, 350],
            "met_phi"                           : [25, -3.2, 3.2],
            "metnomu_et"                        : [40, 120, 500],
            "metnomu_phi"                       : [25, -0.1, 6.3],
            "met_er_et"                         : [30, 0, 600],
            "met_er_phi"                        : [25, -3.14159, 3.14159],
            "HT20Full"                          : [25, 100, 1000],
        	"bjet1_pt"                          : [30, 20, 200],
        	"bjet1_eta"                         : [25, -2.5, 2.5],
        	"bjet1_phi"                         : [25, -3.14159, 3.14159],
        	"bjet1_bID_deepFlavor"              : [40, 0, 1],
        	"bjet2_pt"                          : [30, 20, 200],
        	"bjet2_eta"                         : [25, -2.5, 2.5],
        	"bjet2_phi"                         : [25, -3.14159, 3.14159],
        	"bjet2_bID_deepFlavor"              : [40, 0, 1],
        	"bjet1_JER"                         : [35, 0, 400],
        	"bjet2_JER"                         : [35, 0, 400],
        	"btau_deltaRmin"                    : [25, 0, 5],
        	"btau_deltaRmax"                    : [25, 0, 5],
        	"bH_mass"                           : [30, 0, 300],
        	"bH_pt"                             : [30, 0, 600],
        	"dib_deltaR"                        : [25, 0, 5],
        	"dib_deltaPhi"                      : [15, 0., 3.14159],
        	"dib_deltaEta"                      : [20, 0, 10],
        	"tauH_mass"                         : [30, 0, 300],
        	"tauH_pt"                           : [30, 0, 400],
            "tauH_eta"                          : [25, -2.5, 2.5],
            "tauH_SVFIT_mass"                   : [30, 0, 300],
            "tauH_SVFIT_pt"                     : [30, 0, 600],
            "tauH_SVFIT_eta"                    : [25, -2.5, 2.5],
        	"ditau_deltaR"                      : [25, 0, 5],
        	"ditau_deltaPhi"                    : [15, 0., 3.14159],
        	"ditau_deltaEta"                    : [20, 0, 10],
        	"HH_mass"                           : [50, 0, 3000],
        	"HH_pt"                             : [30, 0, 300],
        	"HHKin_mass"                        : [50, 0, 3000],
        	"HH_deltaPhi"                       : [15, 0., 3.14159],
        	"HHsvfit_pt"                        : [30, 0, 300],
        	"HHsvfit_deltaPhi"                  : [25, 0, 3.14159],
        	"HH_deltaR"                         : [35, 0, 7],
            "fatjet_softdropMass"               : [50, 0, 300],
            "fatjet_pt"                         : [15, 250, 700],
            "fatjet_eta"                        : [7, -2.5, 2.5 ],
            "fatjet_phi"                        : [7, -3.14159, 3.14159],
            "fatjet_particleNetMDJetTags_score" : [15, 0.0, 1.05],
            "fatjet_particleNetMDJetTags_mass"  : [15, 0, 250],
            "HHbregrsvfit_m"                    : [30, 0, 1800],
            "HHbregrsvfit_pt"                   : [30, 0, 600],
            "HHbregrsvfit_eta"                  : [25, -2.5, 2.5],
            "met_cov00"                         : [50, 0., 3000.],
            "met_cov11"                         : [50, 0., 3000.],
            "met_cov01"                         : [80, -1000, 1000],
            "METx"                              : [80, -280., 280.],
            "METy"                              : [80, -280., 280.],
            "bjet1_CvsB"                        : [30, 0., 1.],
            "bjet2_CvsB"                        : [30, 0., 1.],
            "bjet1_CvsL"                        : [30, 0., 1.],
            "bjet2_CvsL"                        : [30, 0., 1.],
            "bjet1_HHbtag"                      : [30, 0., 2.5],
            "bjet2_HHbtag"                      : [30, 0., 2.5],
        }

        if not set(define_nondnn_variables()).issubset(bin_map.keys()):
            diff = set(define_nondnn_variables()) - bin_map.keys()
            raise RuntimeError("The following variables were defined but have not binning assigned: {}.".format(diff))
        
        bins = "[histos]"
        for key, val in bin_map.items():
            bins += '\n' + key + " = " + str(val)[1:-1]            
        bins += "\n"
        return bins

    @staticmethod
    def is_for_limits(spin, mass):
        return False if spin=='' and mass=='' else True


def r_factor(y, chan):
    r_map = {
        "UL16": {
            "ETau"  : 1.0068,
            "MuTau" : 1.0063,
            "TauTau": 0.9880,
            "MuMu"  : 1.0,
        },
        "UL16APV": {
            "ETau"  : 0.9611,
            "MuTau" : 0.9661,
            "TauTau": 0.9611,
            "MuMu"  : 1.0,
        },
        "UL17": {
            "ETau"  : 0.9822,
            "MuTau" : 0.9852,
            "TauTau": 0.9786,
            "MuMu"  : 1.0,
        },
        "UL18": {
            "ETau"  : 0.9870,
            "MuTau" : 0.9898,
            "TauTau": 0.9774,
            "MuMu"  : 1.0,
        }
    }
    string = "\n".join(("[bTagRfactor]",
                        f"central = {str(r_map[y][chan])}",
                    ))
    return string
    
def write_limit_main_config(outfile, channel, year, pars, vars_mode, spin='', mass=''):
    """Write single limit main configuration file."""
    for_limits = pars.is_for_limits(spin, mass)
    
    bckg_limits = []
    for k,v in pars.bckg_limits.items():
        bckg_limits.append(k + " = " + ', '.join(v))
    if not for_limits:
        bckg_plots = []
        for k,v in pars.bckg_plots.items():
            bckg_plots.append(k + " = " + ', '.join(v))

    variables = []
    if for_limits:
        variables.extend(define_dnn_variables(year, spin, mass, pars, with_systs=True))
    else:
        if vars_mode != "dnn_only":
            variables.extend(define_nondnn_variables())
        if vars_mode != "no_dnn":
            for m in pars.masses:
                variables.extend(define_dnn_variables(year, pars.spin_for_plots, m, pars, with_systs=False))

    if for_limits:
        signals = "signals = " + pars.signals[spin](mass)
    else:
        signals = "signals = " + ', '.join(((pars.signals[pars.spin_for_plots](m) for m in pars.masses[::5])))

    if pars.is_for_limits(spin, mass):
        selections = pars.selections[channel]
        if float(mass) < 320:
            selections = [s for s in selections if 'boosted' not in s]
    else:
        selections = pars.base_selections + pars.selections[channel]

    if channel == "MuMu":
        qcd_str = ""
    else:
        qcd_str = pars.qcd
        
    content = '\n'.join((
        "[general]",
        "lumi = " + pars.lumi,
        "",
        "data = " + ', '.join(pars.data[channel]),
        "",
        signals,
        "",
        "backgrounds = " + ', '.join((k for x in pars.bckg_limits.values() for k in x)),
        "",
        "variables = " + ', '.join(variables),
        "",
        "selections = " + ', '.join(selections),
        "",
        "regions = " + ', '.join(pars.regions[channel].keys()),
        "",
        "[configs]",
        "sampleCfg = config/sampleCfg_" + year + ".cfg",
        "cutCfg = " + outfile('selection'),
        "",
        "[merge_limits]",
        "data_obs = " + ', '.join(pars.data[channel]),
        '\n'.join(bckg_limits),
        '\n',
        qcd_str,
        '\n',
        r_factor(year, channel),
    ))

    content += '\n'
    
    if not for_limits:
        content += '\n' + '\n'.join((
            "[merge_plots]",
            "data_obs = " + ', '.join(pars.data[channel]),
            '\n'.join(bckg_plots)
        ))

    with open(outfile('main'), 'w') as afile:
        afile.write(content)

def write_limit_selection_config(outfile, channel, year, pars, vars_mode, metsf, spin='', mass=''):
    """Write single limit selection configuration file."""
    for_limits = pars.is_for_limits(spin, mass)

    if for_limits:
        dnn_variables = define_dnn_variables(year, spin, mass, pars, with_systs=True)
    else:
        if vars_mode != "dnn_only":
            nondnn_variables = define_nondnn_variables()
        if vars_mode != "no_dnn":
            for m in pars.masses:
                dnn_variables = define_dnn_variables(year, pars.spin_for_plots, m, pars, with_systs=False)
    
    reg = '\n'.join((kreg + " = " + vreg for kreg,vreg in pars.regions[channel].items()))
    
    binning = Params(year).define_nondnn_binning()
    if for_limits:
        binning += Params(year).define_dnn_binning(dnn_variables, channel, spin, mass)
    else:
        if vars_mode != "no_dnn":
            for v, m in zip(dnn_variables, pars.masses):
                binning += Params(year).define_dnn_binning([v], channel, pars.spin_for_plots, m)

    chn_idx = {"ETau": "1", "MuTau": "0", "TauTau": "2", "MuMu": "3"}[channel]
    deepjet = {"UL18":    {"loose": "0.0490", "medium": "0.2783"},
               "UL17":    {"loose": "0.0532", "medium": "0.3040"},
               "UL16APV": {"loose": "0.0508", "medium": "0.2598"},
               "UL16":    {"loose": "0.0480", "medium": "0.2489"},
               }[year]
    pfnet = {"UL18":    {"L": "0.9172", "M": "0.9734", "H": "0.994"},
             "UL17":    {"L": "0.9105", "M": "0.9714", "H": "0.987"},
             "UL16APV": {"L": "0.9088", "M": "0.9737", "H": "0.9883"},
             "UL16":    {"L": "0.9137", "M": "0.9735", "H": "0.9883"},
             }[year]

    category_definitions = '\n'.join((
        "res1b = baseline, btagM , isBoosted != 1, massCut",
        "res2b = baseline, btagMM, massCut",
        "boostedL_pnet = baseline_boosted, pNetBTagL, massCutTau",
        ))
    if channel == "MuTau" or channel == "TauTau":
        category_definitions += '\n' + "ttbarCR = baseline, btagMM, isBoosted != 1, massCutTT"
    elif channel == "MuMu":
        category_definitions += '\n' + "dyCR = baseline, isBoosted != 1, massCutDY"
        category_definitions += '\n' + "dyCR_res1b = baseline, btagM, isBoosted != 1, massCutDY"
        category_definitions += '\n' + "dyCR_res2b = baseline, btagMM, isBoosted != 1, massCutDY"

    if metsf:
        baseline = "!isLeptrigger && isMETtrigger && !isSingleTautrigger && pairType == {} && nleps == 0".format(chn_idx)
    else:
        baseline = "(isLeptrigger || isMETtrigger || isSingleTautrigger) && pairType == {} && nleps == 0".format(chn_idx)
    content = '\n'.join((
        "[selections]",
        "baseline = " + baseline + " && nbjetscand > 1",
        "baseline_boosted = !(( ("+ baseline + ") && nbjetscand > 1 ) && (bjet1_bID_deepFlavor > {wpm} && bjet2_bID_deepFlavor > {wpm}) && (tauH_mass > 20 && bH_mass > 40) ) && (".format(wpm=deepjet["medium"]) + baseline + ") && isBoosted == 1 ",
        "",
        ("btagM  = (bjet1_bID_deepFlavor > {wpm} && bjet2_bID_deepFlavor < {wpm}) || ".format(wpm=deepjet["medium"]) +
         "(bjet1_bID_deepFlavor < {wpm} && bjet2_bID_deepFlavor > {wpm})".format(wpm=deepjet["medium"])),
        "btagLL = bjet1_bID_deepFlavor > {wpl} && bjet2_bID_deepFlavor > {wpl}".format(wpl=deepjet["loose"]),
        "btagMM = bjet1_bID_deepFlavor > {wpm} && bjet2_bID_deepFlavor > {wpm}".format(wpm=deepjet["medium"]),
        "",
        "pNetBTagH = fatjet_particleNetMDJetTags_score >= " + pfnet["H"],
        "pNetBTagM = fatjet_particleNetMDJetTags_score >= " + pfnet["M"],
        "pNetBTagL = fatjet_particleNetMDJetTags_score >= " + pfnet["L"],
        "",
        "massCutTau = tauH_mass > 20 && tauH_mass < 130",
        "massCut    = tauH_mass > 20 && bH_mass > 40",
        "massCutInv = tauH_mass <= 20 || bH_mass <= 40",
        "massCutTT  = tauH_mass > 130",
        "massCutDY  = tauH_mass > 80 && tauH_mass < 100",
        "",
        reg,
        "",
        "baselineMcut    = baseline, massCut",
        "baselineInvMcut = baseline, massCutInv",
        "",
        category_definitions,
        "",
        "[selectionWeights]",
        ""))

    if metsf == "NoSF":
        content += '\n'.join(("baseline = MC_weight, PUReweight, L1pref_weight, trigSFnoMET, dauSFs, PUjetID_SF, bTagweightReshape",
                              "baseline_boosted = MC_weight, PUReweight, L1pref_weight, trigSFnoMET, dauSFs, PUjetID_SF",
                              ""))
    else:
        content += '\n'.join(("baseline = MC_weight, PUReweight, L1pref_weight, trigSF, dauSFs, PUjetID_SF, bTagweightReshape",
                              "baseline_boosted = MC_weight, PUReweight, L1pref_weight, trigSF, dauSFs, PUjetID_SF",
                              ""))

    content += '\n'.join((
        "",
        "[systematics]",
        pars.event_systematics(channel) if for_limits else '',
        "",
        binning,
        "HH_mass = 245. , 255. , 265. , 275. , 285. , 315. , 325. , 375. , 425. , 475. , 525. , 575. , 625. , 675. , 725. , 775. , 825. , 875. , 925. , 1075. , 1325. , 1675. , 1825. , 2175. , 2725. , 3275.",
        "HHKin_mass = 245. , 255. , 265. , 275. , 285. , 315. , 325. , 375. , 425. , 475. , 525. , 575. , 625. , 675. , 725. , 775. , 825. , 875. , 925. , 1075. , 1325. , 1675. , 1825. , 2175. , 2725. , 3275.",
    ))

    with open(outfile('selection'), 'w') as afile:
        afile.write(content)

def define_dnn_variables(year, spin, mass, pars, with_systs):
    """Define DNN variables to be added to the configuration files."""
    for_limits = pars.is_for_limits(spin, mass)
    variables = []
    for syst in pars.dnn_systematics:
        for head in pars.dnn_heads:
            if syst == "nominal":
                variables.append(pars.dnn_scheme.format(mass, spin, head))
            elif with_systs:
                variables.append(pars.dnn_scheme.format(mass, spin, head) + "_" + syst)
    return variables

def define_nondnn_variables():
    """Define non-DNN variables to be added to the configuration files."""
    return ["nbjetscand", "njets",
            "dau1_pt", "dau1_eta", "dau1_phi", "dau1_dxy", "dau1_dz",
            "dau2_pt", "dau2_iso", "dau2_eta", "dau2_phi", "dau2_dxy", "dau2_dz",
            "met_et", "met_phi", "metnomu_et", "metnomu_phi",
            "METx", "METy", "met_cov00", "met_cov01", "met_cov11",
            "bjet1_pt", "bjet1_eta", "bjet1_phi", "bjet1_JER",
            "bjet2_pt", "bjet2_eta", "bjet2_phi", "bjet2_JER",
            "bjet1_bID_deepFlavor", "bjet2_bID_deepFlavor",
            "bjet1_CvsB", "bjet1_CvsL", "bjet2_CvsB", "bjet2_CvsL",
            "bjet1_HHbtag", "bjet2_HHbtag",
            "bH_mass", "bH_pt", "dib_deltaR", "dib_deltaPhi", "dib_deltaEta",
            "tauH_mass", "tauH_pt", "tauH_eta", "tauH_SVFIT_mass", "tauH_SVFIT_pt", "tauH_SVFIT_eta",
            "ditau_deltaR", "ditau_deltaPhi", "ditau_deltaEta",
            "HH_mass", "HH_pt", "HHKin_mass", "HH_deltaPhi", "HH_deltaR",
            "fatjet_softdropMass", "fatjet_pt", "fatjet_eta", "fatjet_phi",
            "HHbregrsvfit_m", "HHbregrsvfit_pt", "HHbregrsvfit_eta"]

def create_dir(d):
    if not os.path.exists(d):
        os.makedirs(d)

def create_configs(args):
    """Create all configuration files for limit extraction."""
    params = Params(args.year, onlymet=args.met_sfs is not None, detailed=args.detailed_plots)

    if args.for_limits:
        outdir = os.path.join("config", "limits")
        create_dir(outdir)
         
        outfile = lambda name : os.path.join(outdir, "_".join((name + "Cfg", channel, args.year, "m" + mass, "s" + spin)) + ".cfg")
         
        for spin in params.spins:
            for mass in params.masses:
                for channel in params.data.keys():
                    write_limit_main_config(outfile, channel, args.year, params, args.vars_mode, spin, mass)
                    write_limit_selection_config(outfile, channel, args.year, params, args.vars_mode, args.met_sfs, spin, mass)

    else:
        if args.met_sfs:
            outfile = lambda name : os.path.join("config", "_".join((name + "Cfg", channel, args.year, args.met_sfs)) + ".cfg")
        else:
            outfile = lambda name : os.path.join("config", "_".join((name + "Cfg", channel, args.year)) + ".cfg")

        for channel in params.data.keys():
            write_limit_main_config(outfile, channel, args.year, params, args.vars_mode)
            write_limit_selection_config(outfile, channel, args.year, params, args.vars_mode, args.met_sfs)

            
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Create all configuration files to run the full histogram submission with systematics.')
    parser.add_argument('--year', default="UL18", choices=("UL16APV", "UL16", "UL17", "UL18"),
                        help='Which data period to consider.')
    parser.add_argument('--for_limits', action="store_true",
                        help='Create main and selection configs for limit extraction.')
    parser.add_argument('--vars_mode', choices=('no_dnn', 'dnn_only', 'all'), default='dnn_only',
                        help="Use DNN variables or not.")
    parser.add_argument('--detailed_plots', action="store_true",
                        help="Show individual background contributions in the plots.")
    parser.add_argument('--met_sfs', default=None, choices=("WithSF", "NoSF", None),
                        help="Produce configuration files to test the MET SFs. New files have extensions.")
    
    create_configs(parser.parse_args())
