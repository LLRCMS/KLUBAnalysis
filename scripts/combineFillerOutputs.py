import modules.ConfigReader as cfgr
import modules.OutputManager as omng
import argparse
import fnmatch
import os
import sys
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

## replace what was merged
if cfg.hasSection("merge"):
    for groupname in cfg.config['merge']: 
        mergelist = cfg.readListOption('merge::'+groupname)
        mergelistA = cfg.readOption('merge::'+groupname)
        theList = None
        if   mergelist[0] in dataList: theList = dataList
        elif mergelist[0] in sigList:  theList = sigList
        elif mergelist[0] in bkgList:  theList = bkgList
        for x in mergelist: theList.remove(x)
        theList.append(groupname)

rootfile = ROOT.TFile.Open(args.dir + "/" + outplotterName)
print '... opening ' , (args.dir + "/" + outplotterName)

ROOT.gROOT.SetBatch(True)
omngr = omng.OutputManager()
omngr.selections  = selList   
omngr.sel_def     = selDefList
omngr.sel_regions = regDefList    
omngr.variables   = varList  
omngr.variables2D = var2DList if var2DList else list([])
# omngr.samples     = sigList + bkgList + dataList
omngr.data        = dataList
omngr.sigs        = sigList
omngr.bkgs        = bkgList
omngr.readAll(rootfile)

## always group together the data and rename them to 'data'
# omngr.groupTogether('data_obs', dataList)

if cfg.hasSection('pp_merge'):
    for groupname in cfg.config['pp_merge']:
        omngr.groupTogether(groupname, cfg.readListOption('pp_merge::'+groupname))

if cfg.hasSection('pp_rebin'):
    for ri in cfg.config['pp_rebin']:
        opts = cfg.readListOption('pp_rebin::'+ri)
        if len(opts) < 4:
            print '** Error: Cannot interpret your rebin instructions:', opts
            continue
        var = opts[0]
        seldef = opts[1]
        newbinning = [float(x) for x in opts[2:]]
        if not var in omngr.variables:
            print ' * var' , var , "was not plotted, won't rebin"
            continue
        if not seldef in omngr.sel_def:
            print ' * sel' , seldef , "was not used, won't rebin"
            continue            
        for reg in omngr.sel_regions:
            sel = seldef + '_' + reg
            omngr.rebin(var, sel, newbinning)

if cfg.hasSection('pp_QCD'):
    omngr.makeQCD(
        SR           = cfg.readOption('pp_QCD::SR'),
        yieldSB      = cfg.readOption('pp_QCD::yieldSB'),
        shapeSB      = cfg.readOption('pp_QCD::shapeSB'),
        SBtoSRfactor = float(cfg.readOption('pp_QCD::SBtoSRfactor')),
        doFitIf      = cfg.readOption('pp_QCD::doFitIf'),
        fitFunc      = cfg.readOption('pp_QCD::fitFunc')
        )

fOut = ROOT.TFile(args.dir+"/" + 'analyzedOutPlotter.root', 'recreate')
omngr.saveToFile(fOut)
