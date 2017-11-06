from ROOT import *

bkgList = ["TT", "DY0b", "DY1b", "DY2b", "WJets", "TW", "EWKW", "EWKZ2Jets_ZToLL", "WW", "WZ", "ZZ", "ZH", 'Radion250', 'Radion270', 'Radion280', 'Radion300', 'Radion350', 'Radion400', 'Radion450', 'Radion500', 'Radion550', 'Radion600', 'Radion650', 'Radion750', 'Radion900', 'lambdarew0', 'lambdarew1', 'lambdarew2', 'lambdarew3', 'lambdarew4', 'lambdarew5', 'lambdarew6', 'lambdarew7', 'lambdarew8', 'lambdarew9', 'lambdarew10', 'lambdarew11', 'lambdarew12', 'lambdarew13', 'lambdarew14', 'lambdarew15', 'lambdarew16', 'lambdarew17', 'lambdarew18', 'lambdarew19', 'lambdarew20', 'lambdarew21', 'lambdarew22', 'lambdarew23', 'lambdarew24', 'lambdarew25', 'lambdarew26', 'lambdarew27', 'lambdarew28', 'lambdarew29', 'lambdarew30', 'lambdarew31', 'lambdarew32', 'lambdarew33', 'lambdarew34', 'lambdarew35', 'lambdarew36', 'lambdarew37', 'lambdarew38', 'lambdarew39', 'lambdarew40', 'lambdarew41', 'lambdarew42', 'lambdarew43', 'lambdarew44', 'lambdarew45', 'lambdarew46', 'lambdarew47', 'lambdarew48', 'lambdarew49', 'lambdarew50', 'lambdarew51']


channel = 'MuTau'
var = 'MT2' ## oops! forgot mt2 here

# channel = 'ETau'
# var = 'ditau_deltaR' ## oops! forgot mt2 here
# # list without the nominal

selections = ['s1b1jresolvedMcutlmr70', 's2b0jresolvedMcutlmr70', 's1b1jresolvedMcutlmr90', 's2b0jresolvedMcutlmr90', 's1b1jresolvedMcuthmr90', 's2b0jresolvedMcuthmr90', 'sboostedLLMcut']

# channel = 'TauTau'
# var = 'MT2'

# selections = ['s1b1jresolvedMcut', 's2b0jresolvedMcut', 'sboostedLLMcut']

# s1b1jresolvedMcutlmr70_tesUp
# s2b0jresolvedMcutlmr70_tesUp
# s1b1jresolvedMcutlmr90_tesUp
# s2b0jresolvedMcutlmr90_tesUp
# s1b1jresolvedMcuthmr90_tesUp
# s2b0jresolvedMcuthmr90_tesUp
# sboostedLLMcut_tesUp
# s1b1jresolvedMcutlmr70_tesDown
# s2b0jresolvedMcutlmr70_tesDown
# s1b1jresolvedMcutlmr90_tesDown
# s2b0jresolvedMcutlmr90_tesDown
# s1b1jresolvedMcuthmr90_tesDown
# s2b0jresolvedMcuthmr90_tesDown
# sboostedLLMcut_tesDown
# s1b1jresolvedMcutlmr70_jesUp
# s2b0jresolvedMcutlmr70_jesUp
# s1b1jresolvedMcutlmr90_jesUp
# s2b0jresolvedMcutlmr90_jesUp
# s1b1jresolvedMcuthmr90_jesUp
# s2b0jresolvedMcuthmr90_jesUp
# sboostedLLMcut_jesUp
# s1b1jresolvedMcutlmr70_jesDown
# s2b0jresolvedMcutlmr70_jesDown
# s1b1jresolvedMcutlmr90_jesDown
# s2b0jresolvedMcutlmr90_jesDown
# s1b1jresolvedMcuthmr90_jesDown
# s2b0jresolvedMcuthmr90_jesDown
# sboostedLLMcut_jesDown
print "doing channel: ", channel

#################

inputFile = '/home/llr/cms/cadamuro/testAnalysisHelper2/CMSSW_7_4_7/src/KLUBAnalysis/analysis_%s_27Feb_scaleSyst/outPlotter.root' % channel
toscan = ['tes', 'jes'] ## will append "Up/Down"

print inputFile

fIn = TFile.Open(inputFile)

# DY0b_s2b0jresolvedMcutlmr70_tesDown_SR_MT2
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
            # print sel, var, bkg
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