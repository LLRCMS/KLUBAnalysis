"""
Calculates systematic acceptance scale factors. UL update required.
"""
from ROOT import *
import argparse

def parseOptions():
    usage = ('usage: %prog [args.ons] datasetList\n %prog -h for help')
    parser = argparse.ArgumentParser(description=usage)
    parser.add_argument('-y', '--year', dest='year', default="2016", help='year')
    return parser.parse_args()

args = parseOptions()

channels = ['TauTau', 'MuTau', 'ETau']

MClist = ['TT', 'W', 'EWK', 'singleT', 'TW', 'ZH', 'WH', 'VV', 'ttH', 'TTX', 'ggH', 'qqH', 'VVV', 'DY_LM',
		  'DY_0b_1Pt', 'DY_0b_2Pt', 'DY_0b_3Pt', 'DY_0b_4Pt', 'DY_0b_5Pt', 'DY_0b_6Pt',
		  'DY_1b_1Pt', 'DY_1b_2Pt', 'DY_1b_3Pt', 'DY_1b_4Pt', 'DY_1b_5Pt', 'DY_1b_6Pt',
		  'DY_2b_1Pt', 'DY_2b_2Pt', 'DY_2b_3Pt', 'DY_2b_4Pt', 'DY_2b_5Pt', 'DY_2b_6Pt',
		  ]

selections = ['res1b', 'res2b', 'boostedL_pnet']

spin = '0'
var = {
	'res1b'        : 'pdnn_m{}_s' + spin,
	'res2b'		   : 'pdnn_m{}_s' + spin,
	'boostedL_pnet': 'pdnn_m{}_s' + spin,
}

toscan = ['tesXXX_DM0', 'tesXXX_DM1', 'tesXXX_DM10', 'tesXXX_DM11', 'eesXXX_DM0', 'eesXXX_DM1', 'mesXXX',
          'jesXXX_Tot', 'jesXXX_1', 'jesXXX_2', 'jesXXX_3', 'jesXXX_4', 'jesXXX_5', 'jesXXX_6', 'jesXXX_7',
          'jesXXX_8', 'jesXXX_9', 'jesXXX_10', 'jesXXX_11']

for channel in channels:
	inputFile = '../analysis_28Dec2022_syst_EFT/%s_%s/outPlotter.root' % (channel, args.year)
    print("Processing channel {} and input file {}.".format(channel, inputFile))
	fIn = TFile.Open(inputFile)
	
	## retrieve histograms
	for sel in selections:
	    print("Selection {}...".format(sel))
	    for scale in toscan:
	        histos_nominal, histos_up, histos_down = {}, {}, {}
	        fout = open("scales" + args.year + "/" + channel + '_' + sel + '_' + scale.replace('XXX','') + '.txt', 'w')
	        for proc in MClist:
	            hname_nominal = proc + '_' + sel + '_nominal_SR_' + var[sel]
	            hname_up      = proc + '_' + sel + '_' + scale.replace('XXX','Up') + '_SR_'   + var[sel]
	            hname_down    = proc + '_' + sel + '_' + scale.replace('XXX','Down') + '_SR_' + var[sel]
	            
	            histos_nominal[proc] = fIn.Get(hname_nominal)
	            histos_up[proc]      = fIn.Get(hname_up)
	            histos_down[proc]    = fIn.Get(hname_down)
	            
	            #print sel, var[sel], proc,scale
	            ynom  = histos_nominal[proc].Integral()
	            yup   = histos_up[proc].Integral()
	            ydown = histos_down[proc].Integral()
	
	            shiftUp   = 1.0
	            shiftDown = 1.0
	            if ynom > 0. and yup > 0. and ydown > 0. and histos_nominal[proc].GetEntries() > 10:
	                shiftUp = 1.*yup/ynom
	                shiftDown = 1.*ydown/ynom

	            fout.write('%30s     ' % proc)
	            fout.write('%.3f     ' % shiftUp)
	            fout.write('%.3f'      % shiftDown)
	            fout.write('\n')

	        fout.close()
