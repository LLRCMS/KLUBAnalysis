from ROOT import *

bkgList = [ 'TT', 'WJets', 'TW','EWK', 'WW', 'WW', 'WZ', 'ZH','other',  'ZZ','DY0b','DY1b', 'DY2b', 'lambdarew11']


channel = 'TauTau'
var = 'BDToutSM_kl_1' 

selections = ['s1b1jresolvedMcut', 's2b0jresolvedMcut', 'sboostedLLMcut','VBFloose', 'VBFtight']


print "doing channel: ", channel

#################

inputFile = '/home/llr/cms/amendola/HH2017/CMSSW_9_0_0/src/KLUBAnalysis/analysis_%s_12Sept2019_Thesis_selSyst/outPlotter.root' % channel
toscan = ['tes', 'jes'] ## will append "Up/Down"

print inputFile

fIn = TFile.Open(inputFile)


## retrieve histograms
for sel in selections:
    for scale in toscan:
        histos_nominal = {}
        histos_up      = {}
        histos_down    = {}
        
        for bkg in bkgList:
            hname_nominal = bkg + '_' + sel + '_nominal_SR_' + var 
            hname_up      = bkg + '_' + sel + '_' + scale + 'Up_SR_'   + var 
            hname_down    = bkg + '_' + sel + '_' + scale + 'Down_SR_' + var  
            
            histos_nominal[bkg] = fIn.Get(hname_nominal)
            histos_up[bkg]      = fIn.Get(hname_up)
            histos_down[bkg]    = fIn.Get(hname_down)
            
        ## print this to a file
        fout = open (channel + '_' + sel + '_' + scale + '.txt', 'w')
        for bkg in bkgList:
            print sel, var, bkg,scale
            ynom  = histos_nominal[bkg].Integral()
            yup   = histos_up[bkg].Integral()
            ydown = histos_down[bkg].Integral()

            shiftUp   = 1.0
            shiftDown = 1.0
            if ynom > 0 and histos_nominal[bkg].GetEntries() > 10:
                shiftUp = 1.*yup/ynom
                shiftDown = 1.*ydown/ynom

            fout.write('%20s     ' % bkg)
            fout.write('%.3f     ' % shiftUp)
            fout.write('%.3f'      % shiftDown)
            fout.write('\n')
        fout.close()
