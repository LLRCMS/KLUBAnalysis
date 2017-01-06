import modules.ConfigReader as cfgr
import modules.OutputManager as omng
import argparse
import fnmatch
import os
import ROOT
import itertools

def findInFolder (folder, pattern):
    ll = []
    for ff in os.listdir(folder):
        if fnmatch.fnmatch(ff, pattern): ll.append(ff)
    if len (ll) == 0:
        print "*** WARNING: No valid file " , pattern , "found in " , folder
        return None
    if len (ll) > 1:
        print "*** WARNING: Too many files found in " , folder , ", using first one : " , ll
    return ll[0]

# prototype: ZZTo2L2Q_defaultBtagLLNoIsoBBTTCut_SSrlx_HHsvfit_deltaPhi
def retrieveHisto (rootFile, name, var, sel):
    fullName = name + '_' + sel + '_' + var
    if not rootFile.GetListOfKeys().Contains(fullName):
        print "*** WARNING: histo " , fullName , " not available"
        return None
    hFound = rootFile.Get(fullName)
    return hFound

# tag = "sig", "bkg", "DATA"
def retrieveHistos (rootFile, namelist, var, sel):
    res = {}
    for name in namelist:
        theH = retrieveHisto(rootFile, name, var, sel)
        if not theH:
            continue
        # res[name] = SampleHist.SampleHist(name=name, inputH=theH)
        res[name] = theH
    return res

# def makeHistoName(sample, sel, var, syst=""):
#     name = sample +  "_" + sel + "_" + var
#     if syst:
#         name += "_"
#         name += syst
#     return name;

###############################################################################
###############################################################################

parser = argparse.ArgumentParser(description='Command line parser of plotting options')
parser.add_argument('--dir', dest='dir', help='analysis output folder name', default="./")
args = parser.parse_args()

cfgName        = findInFolder  (args.dir+"/", 'mainCfg_*.cfg')
outplotterName = findInFolder  (args.dir+"/", 'outPlotter.root')

cfg        = cfgr.ConfigReader (args.dir + "/" + cfgName)
varList    = cfg.readListOption("general::variables")
var2DList  = cfg.readListOption("general::variables2D")
selDefList = cfg.readListOption("general::selections") ## the selection definition
regDefList = cfg.readListOption("general::regions") ## the regions that are combined with the previous
bkgList    = cfg.readListOption("general::backgrounds")
dataList   = cfg.readListOption("general::data")
sigList    = cfg.readListOption("general::signals")
selList    = [x[0] + '_' + x[1] for x in list(itertools.product(selDefList, regDefList))]

rootfile = ROOT.TFile.Open(args.dir + "/" + outplotterName)
print '... opening ' , (args.dir + "/" + outplotterName)

ROOT.gROOT.SetBatch(True)
omngr = omng.OutputManager()
omngr.selections  = selList   
omngr.sel_def     = selDefList
omngr.sel_regions = regDefList    
omngr.variables   = varList  
omngr.variables2D = var2DList    
# omngr.samples     = sigList + bkgList + dataList
omngr.data        = dataList
omngr.signals     = sigList
omngr.bkgs        = bkgList
omngr.readAll(rootfile)
omngr.groupTogether('TT', ['TTfullyHad', 'TTfullyLep', 'TTsemiLep'])
omngr.groupTogether('WJets', ['WJets0jet', 'WJets1jet', 'WJets2jet', 'WJets3jet', 'WJets4jet'])
omngr.makeQCD(SR='SR', yieldSB='SStight', shapeSB='SSrlx', SBtoSRfactor=1.5, doFitIf="'defaultBtagLLNoIso' in sel and var=='MT2'", fitFunc='[0] + [1]*x + [2]*x*x')

fOut = ROOT.TFile(args.dir+"/" + 'analyzedOutPlotter.root', 'recreate')
omngr.saveToFile(fOut)
