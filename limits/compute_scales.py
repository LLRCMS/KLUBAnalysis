from ROOT import *
import optparse

def parseOptions():

    usage = ('usage: %prog [options] datasetList\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)

    parser.add_option('-y', '--year'    , dest='year'    , type='string', default="2016"  , help='year')

    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()


parseOptions()

channels = ['TauTau', 'MuTau', 'ETau']

MClist = ['TT', 'W', 'EWK', 'singleT', 'TW', 'ZH', 'WH', 'VV', 'ttH', 'TTX', 'ggH', 'VBFH', 'VVV', 'DY_0b_1Pt', 'DY_0b_2Pt', 'DY_0b_3Pt', 'DY_0b_4Pt', 'DY_0b_5Pt', 'DY_0b_6Pt', 'DY_1b_1Pt', 'DY_1b_2Pt', 'DY_1b_3Pt', 'DY_1b_4Pt', 'DY_1b_5Pt', 'DY_1b_6Pt', 'DY_2b_1Pt', 'DY_2b_2Pt', 'DY_2b_3Pt', 'DY_2b_4Pt', 'DY_2b_5Pt', 'DY_2b_6Pt', 'DY_LM', 'GGHH_NLO_cHHH1_xs', 'GGHH_NLO_cHHH2p45_xs', 'GGHH_NLO_cHHH5_xs', 'VBFHH_CV_1_C2V_1_C3_1_xs' , 'VBFHH_CV_0p5_C2V_1_C3_1_xs', 'VBFHH_CV_1p5_C2V_1_C3_1_xs', 'VBFHH_CV_1_C2V_1_C3_0_xs', 'VBFHH_CV_1_C2V_1_C3_2_xs', 'VBFHH_CV_1_C2V_2_C3_1_xs']


#selections = ['s1b1jresolvedMcut', 's2b0jresolvedMcut', 'sboostedLLMcut', 'VBFloose', 'GGFclass', 'VBFclass', 'ttHclass', 'TTlepclass', 'TThadclass', 'DYclass']
selections = ['s1b1jresolvedMcut', 's2b0jresolvedMcut', 'sboostedLLMcut', 'VBFloose']

var = {
	's1b1jresolvedMcut' : 'DNNoutSM_kl_1',
	's2b0jresolvedMcut' : 'DNNoutSM_kl_1',
	'sboostedLLMcut'    : 'DNNoutSM_kl_1',
	'VBFloose'          : 'DNNoutSM_kl_1',
	'GGFclass'          : 'mdnn__v2__kl1_c2v1_c31__hh_ggf',
	'VBFclass'          : 'mdnn__v2__kl1_c2v1_c31__hh_vbf',
	'ttHclass'          : 'mdnn__v2__kl1_c2v1_c31__tth',
	'TTlepclass'        : 'mdnn__v2__kl1_c2v1_c31__tt_lep',
	'TThadclass'        : 'mdnn__v2__kl1_c2v1_c31__tt_fh',
	'DYclass'           : 'mdnn__v2__kl1_c2v1_c31__dy'
}

#toscan = ['tes', 'jes'] ## will append "Up/Down"
toscan = ['tesXXX_DM0','tesXXX_DM1','tesXXX_DM10','tesXXX_DM11','eesXXX_DM0','eesXXX_DM1','mesXXX', 'jesXXX_Tot']


for channel in channels:
	print "doing channel: ", channel 
	inputFile = '../analysis_%s_%s_20Sept2020_syst/outPlotter.root' % (channel, opt.year)
	print inputFile	
	fIn = TFile.Open(inputFile)
	
	## retrieve histograms
	for sel in selections:
	    for scale in toscan:
	        histos_nominal = {}
	        histos_up      = {}
	        histos_down    = {}
	        fout = open ("scales"+opt.year+"/"+channel + '_' + sel + '_' + scale.replace('XXX','') + '.txt', 'w')
	        for proc in MClist:
	            hname_nominal = proc + '_' + sel + '_nominal_SR_' + var[sel]
	            hname_up      = proc + '_' + sel + '_' + scale.replace('XXX','Up') + '_SR_'   + var[sel]
	            hname_down    = proc + '_' + sel + '_' + scale.replace('XXX','Down') + '_SR_' + var[sel]
	            
	            histos_nominal[proc] = fIn.Get(hname_nominal)
	            histos_up[proc]      = fIn.Get(hname_up)
	            histos_down[proc]    = fIn.Get(hname_down)
	            
	            print sel, var[sel], proc,scale
	            ynom  = histos_nominal[proc].Integral()
	            yup   = histos_up[proc].Integral()
	            ydown = histos_down[proc].Integral()
	
	            shiftUp   = 1.0
	            shiftDown = 1.0
	            if ynom > 0 and histos_nominal[proc].GetEntries() > 10:
	                shiftUp = 1.*yup/ynom
	                shiftDown = 1.*ydown/ynom
	
	            fout.write('%20s     ' % proc)
	            fout.write('%.3f     ' % shiftUp)
	            fout.write('%.3f'      % shiftDown)
	            fout.write('\n')
	        fout.close()
