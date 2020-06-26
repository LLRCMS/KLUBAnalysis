from ROOT import *


MClist  = ['TT','WJets',   'EWK',     'singleT', 'ZH',      'WH',      'WW',      'WZ',      'ttH','others', 'DY','GGHH_NLO_cHHH1_xs', 'GGHH_NLO_cHHH2p45_xs','GGHH_NLO_cHHH5_xs','VBFHH_CV_1_C2V_1_C3_1_xs' , 'VBFHH_CV_0p5_C2V_1_C3_1_xs', 'VBFHH_CV_1p5_C2V_1_C3_1_xs', 'VBFHH_CV_1_C2V_1_C3_0_xs' ,'VBFHH_CV_1_C2V_1_C3_2_xs' ,'VBFHH_CV_1_C2V_2_C3_1_xs']

 #channels = ['TauTau', 'ETau', 'MuTau']
channels = ['TauTau']
selections = ['s1b1jresolvedMcut', 's2b0jresolvedMcut', 'sboostedLLMcut','VBFloose']

var = "DNNoutSM_kl_1"
#var = {'s1b1jresolvedMcut':'DNNoutSM_kl_1', 
#       's2b0jresolvedMcut':'DNNoutSM_kl_1', 
#       'sboostedLLMcut'   :'DNNoutSM_kl_1', 
#       'VBFloose'         :'DNNoutSM_kl_1' 
#}

year = 2018

#toscan = ['tes', 'jes'] ## will append "Up/Down"

toscan = ['tesXXX_DM0','tesXXX_DM1','tesXXX_DM10','tesXXX_DM11','eesXXX_DM0','eesXXX_DM1','mesXXX', 'jesXXX_Tot']


for channel in channels:
	print "doing channel: ", channel 
	inputFile = '../analysis_%s_24June2020_syst/outPlotter.root' % channel
	print inputFile	
	fIn = TFile.Open(inputFile)
	
	## retrieve histograms
	for sel in selections:
	    for scale in toscan:
	        histos_nominal = {}
	        histos_up      = {}
	        histos_down    = {}
	        fout = open ("scales2018/"+channel + '_' + sel + '_' + scale.replace('XXX','') + '.txt', 'w')
	        for proc in MClist:
	            hname_nominal = proc + '_' + sel + '_nominal_SR_' + var 
	            hname_up      = proc + '_' + sel + '_' + scale.replace('XXX','Up') + '_SR_'   + var 
	            hname_down    = proc + '_' + sel + '_' + scale.replace('XXX','Down') + '_SR_' + var  
	            
	            histos_nominal[proc] = fIn.Get(hname_nominal)
	            histos_up[proc]      = fIn.Get(hname_up)
	            histos_down[proc]    = fIn.Get(hname_down)
	            
	            print sel, var, proc,scale
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
