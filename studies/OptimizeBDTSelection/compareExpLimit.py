import ROOT

def parseFile(filename):
    f = open(filename)
    matches = []
    for line in f:
        if not 'Expected 50.0%: r <' in line:
            continue
        val = line.replace('Expected 50.0%: r <', '')
        val = float(val)
        matches.append(val)
    
    if len(matches) == 0:
        print "did not find any expected in file: " , filename
        return 0.0
    else:
        return matches[-1]

def makeXvalList(sigList):
    xs = []

    if len(sigList) == 0:
        return xs

    if 'Radion' in sigList[0]:
        print 'This is a radion sig list'
        for signame in sigList:
            x = signame.replace('Radion', '')
            x = float(x)
            xs.append(x)

    elif 'lambdarew' in sigList[0]:
        print 'This is a lambda sig list'
        for signame in sigList:
            num = int(signame.replace('lambdarew', ''))
            num = num - 20 # the offset in nmbering histos
            xs.append(num)
    return xs

## Radion500s2b0jresolvedMcutnr70HHKin_mass_raw
## out_Asym_Radion500.log
def makeLimitPlot(sigList, xvals, sel, var, folderBase):
    if folderBase[-1] != '/':
        folderBase += '/'
    
    limits = []
    for sig in sigList:
        f = folderBase + sig + sel + var + '/' + 'out_Asym_' + sig + '.log'
        limit = parseFile(f)
        limits.append(limit)
    # print limits
    gr = ROOT.TGraph()
    gr.SetName('gr_' + sel)
    for idx in range(0, len(limits)):
        gr.SetPoint(idx, xvals[idx], limits[idx])
    gr.SetMarkerStyle(20)
    gr.SetMarkerSize(1.)
    return gr
#################################################



# folder = '/home/llr/cms/ortona/diHiggs/CMSSW_7_4_7/src/KLUBAnalysis/combiner/cards_MuTau_2017_01_26_provatagli/'

# base = ['s2b0jresolvedMcut']
# baseMT = ['s2b0jresolvedMcutMTCut']
# basenr = ['s2b0jresolvedMcutnr50', 's2b0jresolvedMcutnr60', 's2b0jresolvedMcutnr70', 's2b0jresolvedMcutnr80', 's2b0jresolvedMcutnr85', 's2b0jresolvedMcutnr90']
# baserlm = ['s2b0jresolvedMcutlmr50', 's2b0jresolvedMcutlmr60', 's2b0jresolvedMcutlmr70', 's2b0jresolvedMcutlmr80', 's2b0jresolvedMcutlmr85', 's2b0jresolvedMcutlmr90']
# baserhm = ['s2b0jresolvedMcuthmr50', 's2b0jresolvedMcuthmr60', 's2b0jresolvedMcuthmr70', 's2b0jresolvedMcuthmr80', 's2b0jresolvedMcuthmr85', 's2b0jresolvedMcuthmr90']

# allSels = base + baseMT + basenr + baserlm + baserhm

# sigList = ['lambdarew0', 'lambdarew5', 'lambdarew10', 'lambdarew15', 'lambdarew18', 'lambdarew19', 'lambdarew20', 'lambdarew21', 'lambdarew22', 'lambdarew23', 'lambdarew24', 'lambdarew25', 'lambdarew30', 'lambdarew40', 'lambdarew50']
# var = 'MT2'
# outname = "limit_plots_lambda.root"

######################

folder = '/home/llr/cms/ortona/diHiggs/CMSSW_7_4_7/src/KLUBAnalysis/combiner/cards_MuTau_BDTtest/'

NRold = ['s2b0jresolvedMcutnr50', 's2b0jresolvedMcutnr60', 's2b0jresolvedMcutnr70', 's2b0jresolvedMcutnr80', 's2b0jresolvedMcutnr85', 's2b0jresolvedMcutnr90']
NRnew = ['s2b0jresolvedMcutnr201750', 's2b0jresolvedMcutnr201760', 's2b0jresolvedMcutnr201770', 's2b0jresolvedMcutnr201780', 's2b0jresolvedMcutnr201785', 's2b0jresolvedMcutnr201790']
resHM = ['s2b0jresolvedMcuthmr50', 's2b0jresolvedMcuthmr60', 's2b0jresolvedMcuthmr70', 's2b0jresolvedMcuthmr80', 's2b0jresolvedMcuthmr85', 's2b0jresolvedMcuthmr90']
resLM = ['s2b0jresolvedMcutlmr50', 's2b0jresolvedMcutlmr60', 's2b0jresolvedMcutlmr70', 's2b0jresolvedMcutlmr80', 's2b0jresolvedMcutlmr85', 's2b0jresolvedMcutlmr90']

allSels = NRold + NRnew + resHM + resLM
#sigList = ['lambdarew0', 'lambdarew5', 'lambdarew10', 'lambdarew15', 'lambdarew18', 'lambdarew19', 'lambdarew20', 'lambdarew21', 'lambdarew22', 'lambdarew23', 'lambdarew24', 'lambdarew25', 'lambdarew26', 'lambdarew27', 'lambdarew28', 'lambdarew30', 'lambdarew35', 'lambdarew40', 'lambdarew50']
#var = 'MT2'
#outname = "limit_plots_lambda_2017train.root"

######################

# sigList = ['Radion250', 'Radion260', 'Radion270', 'Radion280', 'Radion300', 'Radion320', 'Radion340', 'Radion350', 'Radion400', 'Radion500', 'Radion550', 'Radion650', 'Radion700', 'Radion750', 'Radion800', 'Radion900']
sigList = ['Radion250', 'Radion260', 'Radion270', 'Radion300', 'Radion320', 'Radion350', 'Radion400', 'Radion500', 'Radion550', 'Radion650',         'Radion700', 'Radion750', 'Radion800', 'Radion900'] 
var = 'HHKin_mass_raw'
#outname = "limit_plots.root"
outname = "limit_plots_new.root"

xvals = makeXvalList(sigList)
print xvals

## make a limit plot for every selection
limplots = {}
print '... doing limit plots'
for sel in allSels:
    limplots[sel] = makeLimitPlot (sigList, xvals, sel, var, folder)
print '... done'

fOut = ROOT.TFile(outname, "recreate")
for key in limplots:
    limplots[key].Write()
