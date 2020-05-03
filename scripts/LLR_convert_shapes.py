#!/usr/bin/env python
# Rearrange LLR bbtautau shape histograms into histograms with the common naming convention.
# This file is part of https://github.com/cms-hh/HHStatAnalysis.

import os
import re
import sys
import argparse
import glob
from ROOT import TFile

parser = argparse.ArgumentParser(description='Apply common naming convention to LLR bbtautau shape histograms.',
                    formatter_class = lambda prog: argparse.HelpFormatter(prog, max_help_position=30, width=90))
parser.add_argument('--input', required=True, dest='input_path', type=str, metavar='INPUT_PATH',
                    help="path were located original histograms")
parser.add_argument('--output', required=True, dest='output', type=str, metavar='OUTPUT_FILE',
                    help="path were to store limits")
parser.add_argument('--analysis', required=True, dest='analysis', type=str, metavar='ANA',
                    help="res_lm, res_hm, nonres or sync")
parser.add_argument('--norm-unc-path', required=False, dest='norm_unc_path', default='', type=str, metavar='NORM_PATH',
                    help="path to the text files with normailzation variations for shape uncertainties")
parser.add_argument('--ignoreShapeUnc', action="store_true", help="Ignore shape uncertainties.")
args = parser.parse_args()

def LoadHistogram(file, hist_name, raise_exception = True):
    hist =  file.Get(hist_name)
    if hist == None:
        if raise_exception:
            raise RuntimeError('Histogram "{}" not found in "{}".'.format(hist_name, file.GetName()))
        return None
    return hist.Clone()

def NumToName(x):
    s = str(x)
    s = re.sub('-', 'm', s)
    return re.sub(r'\.', 'p', s)

def OpenLLRFile(channel):
    #file_name = '{}/{}.root'.format(args.input_path, channel)
    #file_name = args.input_path
    file_name = args.input_path+channel+'.root'
    return TFile(file_name, 'OPEN')

def GetLLRHistName(sample, category, region, variable, uncertainty = None):
    hist_name = '{}_{}_{}_{}'.format(sample, category, region, variable)
    if uncertainty != None:
        hist_name += '_{}'.format(uncertainty)
    return hist_name

signal_region = 'SR'
regions = {
    'SR'        : '',
#    'SStight'   : 'SS_iso',
#    'OSinviso'  : 'OS_antiiso',
#    'SSinviso'  : 'SS_antiiso'
}

era = '13TeV'
scale_variations = [ 'Up', 'Down' ]
shape_variable = None

class CorrelationRange:
    Category, Channel, Analysis, Experiment, LHC = range(5)

class UncDesc:
    def __init__(self, name, correlation_range, LLR_name, scale_upper_case = True):
        self.name = name
        self.correlation_range = correlation_range
        self.LLR_name = LLR_name
        self.shape_variable = None
        self.scale_upper_case = scale_upper_case
        self.norm_correction_dict = None

    def GetHistName(self, sample, channel, category, scale_variation):
        hist_name = '{}'.format(sample)
        if self.correlation_range <= CorrelationRange.Experiment:
            hist_name += '_CMS'
        hist_name += '_{}'.format(self.name)
        if self.correlation_range <= CorrelationRange.Analysis:
            hist_name += '_hh_ttbb'
        if self.correlation_range <= CorrelationRange.Channel:
            hist_name += '_{}'.format(channel)
        if self.correlation_range <= CorrelationRange.Category:
            hist_name += '_{}'.format(category)
        hist_name += '_{}{}'.format(era, scale_variation)
        return hist_name

    def GetLLRHistName(self, sample, channel, category, region, scale_variation):
        variable = None
        LLR_unc = None
        scale_var_name = str(scale_variation)
        if not self.scale_upper_case:
            scale_var_name = scale_variation.lower()
        if self.shape_variable == None:
            variable = shape_variable
            LLR_unc = '{}{}'.format(self.LLR_name, scale_var_name)
        else:
            variable = '{}{}'.format(self.shape_variable, scale_var_name)
        return GetLLRHistName(sample, category, region, variable, LLR_unc)

class BinDesc:
    def __init__(self, sample_name, LLR_sample_names, points_desc = [], unc_list = [], save_all_regions = False):
        self.sample_name = sample_name
        self.LLR_sample_names = LLR_sample_names
        self.unc_list = unc_list
        self.save_all_regions = save_all_regions

        self.hist_names = {}
        if len(points_desc) == 0:
            self.hist_names[sample_name] = LLR_sample_names
        else:
            for point in points_desc[2]:
                hist_name = points_desc[0](sample_name, point)
                self.hist_names[hist_name] = []
                for LLR_sample_name in LLR_sample_names:
                    LLR_hist_name = points_desc[1](LLR_sample_name, point)
                    self.hist_names[hist_name].append(LLR_hist_name)

class Hist:
    def __init__(self, name):
        self.name = name
        self.root_hist = None

    def Add(self, root_hist):
        if self.root_hist == None:
            self.root_hist = root_hist
        else:
            self.root_hist.Add(root_hist)

    def Write(self, dir):
        if self.root_hist == None:
            raise RuntimeError('Can not create histogram {} for {}.'.format(self.name, dir.GetName()))
        dir.WriteTObject(self.root_hist, self.name, 'Overwrite')

class NormCorrectionDictionary:
    def __init__(self, path, channel, category, unc_suffix):
        self.scale_factors = {}
        file_name = '{}/{}_{}_{}.txt'.format(path, channel, category, unc_suffix)
        file = open(file_name, 'r')
        lines = [ s.strip() for s in file.readlines() ]
        lines = filter(lambda s: len(s) != 0, lines)
        for line in lines:
            columns = filter(lambda s: len(s) != 0, re.split(" |\t", line))
            if len(columns) != 3:
                raise RuntimeError('Invalid format in input file {}'.format(file_name))
            sample_name = columns[0]
            up = float(columns[1])
            down = float(columns[2])
            if sample_name in self.scale_factors:
                raise RuntimeError('Duplicated sample name "{}" in input file {}'.format(sample_name, file_name))
            self.scale_factors[sample_name] = { 'Up' : up, 'Down' : down }

    def GetScaleFactor(self, sample_name, scale_variation):
        if sample_name not in self.scale_factors:
            raise RuntimeError('Scale factor not found for sample "".'.format(sample_name))
        if scale_variation not in self.scale_factors[sample_name]:
            raise RuntimeError('Scale variation "" not found for sample "".'.format(scale_variation, sample_name))
        return self.scale_factors[sample_name][scale_variation]


tauES_unc = UncDesc('scale_t', CorrelationRange.Experiment, 'tau', False)
jetES_unc = UncDesc('scale_j', CorrelationRange.Experiment, 'jet', False)
topPt_unc = UncDesc('topPt', CorrelationRange.Experiment, 'top', True)

#bins = [
#    BinDesc('data_obs', [ 'data_obs' ], save_all_regions = True),
#    BinDesc('TT', [ 'TT' ], unc_list = [ tauES_unc, jetES_unc, topPt_unc ], save_all_regions = True),
#    BinDesc('DY_0b', [ 'DY0b' ], unc_list = [ tauES_unc, jetES_unc ], save_all_regions = True),
#    BinDesc('DY_1b', [ 'DY1b' ], unc_list = [ tauES_unc, jetES_unc ], save_all_regions = True),
#    BinDesc('DY_2b', [ 'DY2b' ], unc_list = [ tauES_unc, jetES_unc ], save_all_regions = True),
#    BinDesc('QCD', [ 'QCD' ], unc_list = [ ], save_all_regions = False),
#    BinDesc('W', [ 'WJets' ], unc_list = [ tauES_unc, jetES_unc ], save_all_regions = True),
#    BinDesc('tW', [ 'TW' ], unc_list = [ tauES_unc, jetES_unc ], save_all_regions = True),
#    BinDesc('WW', [ 'WW' ], unc_list = [ tauES_unc, jetES_unc ], save_all_regions = True),
#    BinDesc('WZ', [ 'WZ' ], unc_list = [ tauES_unc, jetES_unc ], save_all_regions = True),
#    BinDesc('ZZ', [ 'ZZ' ], unc_list = [ tauES_unc, jetES_unc ], save_all_regions = True),
#    BinDesc('ZH', [ 'ZH' ], unc_list = [ tauES_unc, jetES_unc ], save_all_regions = True),
#    BinDesc('EWK', [ 'EWKW', 'EWKZ2Jets_ZToLL' ], unc_list = [ tauES_unc, jetES_unc ], save_all_regions = True)
#]

bins = [
    BinDesc('data_obs', ['data_obs'], save_all_regions = False),
    BinDesc('DY'      , ['DY']      , unc_list = [ ], save_all_regions = False),
    BinDesc('TT'      , ['TT']      , unc_list = [ ], save_all_regions = False),
    BinDesc('W'       , ['W']       , unc_list = [ ], save_all_regions = False),
    BinDesc('QCD'     , ['QCD']     , unc_list = [ ], save_all_regions = False),
    BinDesc('EWK'     , ['EWK']     , unc_list = [ ], save_all_regions = False),
    BinDesc('singleT' , ['singleT'] , unc_list = [ ], save_all_regions = False),
    BinDesc('H'       , ['H']       , unc_list = [ ], save_all_regions = False),
    BinDesc('VVV'     , ['VVV']     , unc_list = [ ], save_all_regions = False),
    BinDesc('TTV'     , ['TTV']     , unc_list = [ ], save_all_regions = False),
    BinDesc('TTVV'    , ['TTVV']    , unc_list = [ ], save_all_regions = False),
    BinDesc('VV'      , ['VV']      , unc_list = [ ], save_all_regions = False),
]

if args.analysis == 'res_lm' or args.analysis == 'res_hm':
    shape_variable = 'HHKin_mass_raw'
    masses = [ 250, 270, 280, 300, 350, 400, 450, 500, 550, 600, 650, 750, 900 ]
    out_name_source = lambda bin_name, point: '{}_M{}'.format(bin_name, point)
    LLR_name_source = lambda sample, point: '{}{}'.format(sample, point)
    bins.append(BinDesc('ggRadion_hh_ttbb', [ 'Radion' ], points_desc = [ out_name_source, LLR_name_source, masses ],
                        unc_list = [ tauES_unc, jetES_unc ]))
elif args.analysis == 'nonres':
    shape_variable = 'MT2'
    k_lambda = range(-20, 32)
    out_name_source = lambda bin_name, point: '{}_kl{}'.format(bin_name, NumToName(point))
    LLR_name_source = lambda sample, point: '{}{}'.format(sample, point + 20)
    bins.append(BinDesc('ggh_hh_ttbb', [ 'lambdarew' ], points_desc = [ out_name_source, LLR_name_source, k_lambda ],
                        unc_list = [ tauES_unc, jetES_unc ]))
elif args.analysis == 'sync':
    print 'Doing sync!!'
    shape_variable = 'tauH_pt' #ditau_deltaR
else:
    raise RuntimeError("Unsupported analysis '{}'".format(args.analysis))

eMu_Tau_categories = None
tauTau_categories = { '2j': 'baselineMassCut', 'res1b': 's1b1jresolvedMcut', 'res2b': 's2b0jresolvedMcut',
                      'boosted': 'sboostedLLMcut' }

if args.analysis == 'res_lm':
    eMu_Tau_categories = { '2j': 'baselineMassCut', 'res1b': 's1b1jresolvedMcutlmr90', 'res2b': 's2b0jresolvedMcutlmr90',
                           'boosted': 'sboostedLLMcut' }
elif args.analysis == 'nonres':
    eMu_Tau_categories = { '2j': 'baselineMassCut', 'res1b': 's1b1jresolvedMcutlmr70', 'res2b': 's2b0jresolvedMcutlmr70',
                           'boosted': 'sboostedLLMcut' }
else:
    eMu_Tau_categories = { '2j': 'baselineMassCut', 'res1b': 's1b1jresolvedMcuthmr90', 'res2b': 's2b0jresolvedMcuthmr90',
                            'boosted': 'sboostedLLMcut' }

#channels = {
#    'eTau'      : [ 'ETau', eMu_Tau_categories ],
#    'muTau'     : [ 'MuTau', eMu_Tau_categories ],
#    'tauTau'    : [ 'TauTau', tauTau_categories ]
#}

if args.analysis == 'sync':
    tauTau_categories = {'2j': 'baseline', 'res1b': 's1b1jresolved', 'res2b': 's2b0jresolved', 'boosted': 'sboostedLL'}
    channels = {
        'eTau'   : [ 'ETau'  , tauTau_categories],
        'muTau'  : [ 'MuTau' , tauTau_categories],
        'tauTau' : [ 'TauTau', tauTau_categories],
    }

output = TFile(args.output, 'RECREATE')

for channel,channel_desc in channels.iteritems():
    LLR_channel = channel_desc[0]
    categories = channel_desc[1]
    print '{} -> {}'.format(LLR_channel, channel)
    input_file = OpenLLRFile(LLR_channel)
    print '  opening file:', input_file
    for category,LLR_category in categories.iteritems():
        output_dir_name = '{}_{}'.format(channel, category)
        #tauES_unc.norm_correction_dict = NormCorrectionDictionary(args.norm_unc_path, LLR_channel, LLR_category, 'tes')
        #jetES_unc.norm_correction_dict = NormCorrectionDictionary(args.norm_unc_path, LLR_channel, LLR_category, 'jes')
        for LLR_region,region in regions.iteritems():
            output_dir_name = '{}_{}'.format(channel, category)
            if len(region) > 0:
                output_dir_name += '_{}'.format(region)
            print '\tprocessing {}'.format(output_dir_name)
            output_dir = output.mkdir(output_dir_name)
            for bin_desc in bins:
                if not bin_desc.save_all_regions and LLR_region != signal_region: continue
                for hist_name,sample_names in bin_desc.hist_names.iteritems():
                    hist = Hist(hist_name)
                    print '\t\tcreating {}'.format(hist_name)
                    for LLR_sample_name in sample_names:
                        LLR_hist_name = GetLLRHistName(LLR_sample_name, LLR_category, LLR_region, shape_variable)
                        print '\t\t\t{}'.format(LLR_hist_name)
                        new_hist = LoadHistogram(input_file, LLR_hist_name, True)
                        hist.Add(new_hist)
                    hist.Write(output_dir)
                    # if args.ignoreShapeUnc or LLR_region != signal_region: continue
                    if args.ignoreShapeUnc: continue
                    print '\t\t\t--- scale variations ---'
                    for unc in bin_desc.unc_list:
                        for scale_variation in scale_variations:
                            unc_hist_name = unc.GetHistName(hist_name, channel, category, scale_variation)
                            unc_hist = Hist(unc_hist_name)
                            print '\t\t\tcreating {}'.format(unc_hist_name)
                            for LLR_sample_name in sample_names:
                                LLR_unc_hist_name = unc.GetLLRHistName(LLR_sample_name, LLR_channel, LLR_category,
                                                                       LLR_region, scale_variation)
                                print '\t\t\t\t{}'.format(LLR_unc_hist_name)
                                new_unc_hist = LoadHistogram(input_file, LLR_unc_hist_name, True)
                                if unc.norm_correction_dict != None:
                                    sf = unc.norm_correction_dict.GetScaleFactor(LLR_sample_name, scale_variation)
                                    new_unc_hist.Scale(sf)
                                unc_hist.Add(new_unc_hist)
                            unc_hist.Write(output_dir)
    input_file.Close()

output.Close()
