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
parser.add_argument('--moreDY', type=float, dest='moreDY', help='increase DY by factor moreDY', default=None)
parser.add_argument('--moreDY0', type=float, dest='moreDY0', help='increase DY by factor moreDY0', default=None)
parser.add_argument('--moreDY1', type=float, dest='moreDY1', help='increase DY by factor moreDY1', default=None)
parser.add_argument('--moreDY2', type=float, dest='moreDY2', help='increase DY by factor moreDY2', default=None)
parser.add_argument('--moreTT', type=float, dest='moreTT', help='increase TT by factor moreTT', default=None)
parser.add_argument('--moreDYbin0', type=float, dest='moreDYbin0', help='increase DY by factor moreDY0', default=None)
parser.add_argument('--moreDYbin1', type=float, dest='moreDYbin1', help='increase DY by factor moreDY1', default=None)
parser.add_argument('--moreDYbin2', type=float, dest='moreDYbin2', help='increase DY by factor moreDY2', default=None)
parser.add_argument('--SBtoSR', type=float, dest='SBtoSR', help='specity manually the SBtoSR factor', default=None)



parser.add_argument('--extBkg',  dest='extBkg', help='add a bkg from external file', default=None)
parser.add_argument('--extFile', dest='extFile', help='add a bkg from external file', default=None)
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

if args.extBkg:
    if not args.extFile: print "** Error: no external file provided"
    print '... taking histos for bkg: ' , args.extBkg, 'from file : ',args.extFile
    #omngr.addHistos(args.extBkg, args.extFile)
    inFile = ROOT.TFile.Open(args.extFile)
    for sel in omngr.selections:
        for var in omngr.variables:
            htoadd_name = args.extBkg +  "_" + sel + "_" + var
            htoadd = inFile.Get(htoadd_name)

            omngr.histos[htoadd_name] = htoadd.Clone(htoadd_name)

                                                                                                                              
            
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

if args.moreDY:
    omngr.scaleHistos("DY", args.moreDY)
if args.moreTT:
    omngr.scaleHistos("TT", args.moreTT)
if args.moreDY0:
    omngr.scaleHistos("DY0", args.moreDY0)
if args.moreDY1:
    omngr.scaleHistos("DY1", args.moreDY1)
if args.moreDY2:
    omngr.scaleHistos("DY2", args.moreDY2)
if args.moreDYbin0:
    omngr.scaleHistos("DY", args.moreDYbin0, "pt0to50")
if args.moreDYbin1:
    omngr.scaleHistos("DY", args.moreDYbin1, "pt50to150")
if args.moreDYbin2:
    omngr.scaleHistos("DY", args.moreDYbin2, "pt150")

    

# C/D factor:
# if not specified by --SBtoSRforQCD, 
# the number inserted manually in config is used
# if SBtoSRfactor == 1 in config and --SBtoSRforQCD is not specified, C/D computed dynamically
# 
    
if cfg.hasSection('pp_QCD'):
    SBtoSRforQCD =float(cfg.readOption('pp_QCD::SBtoSRfactor'))
    computeSBtoSRdyn = False
    if SBtoSRforQCD == 1 and not args.SBtoSR: 
        computeSBtoSRdyn = True
    if args.SBtoSR: SBtoSRforQCD = args.SBtoSR
    omngr.makeQCD(
        SR           = cfg.readOption('pp_QCD::SR'),
        yieldSB      = cfg.readOption('pp_QCD::yieldSB'),
        shapeSB      = cfg.readOption('pp_QCD::shapeSB'),
        SBtoSRfactor = SBtoSRforQCD,
        regionC      = cfg.readOption('pp_QCD::regionC'),   
        regionD      = cfg.readOption('pp_QCD::regionD'),   
        doFitIf      = cfg.readOption('pp_QCD::doFitIf'),
        fitFunc      = cfg.readOption('pp_QCD::fitFunc'),
        computeSBtoSR = computeSBtoSRdyn
        )

# VBF HH Reweighting
# reads from the config the 6 input samples and the target couplings
if cfg.hasSection('VBF_rew'):
    if eval(cfg.readOption('VBF_rew::doReweighting')):
        print "--- VBF Reweighting ---"
        inputSigList = cfg.readListOption("VBF_rew::inputSignals")
        if len(inputSigList) != 6:
            print "** ERROR: VBF reweighting requires 6 input samples! You only provided:", len(inputSigList), " --> Skipping VBF reweighting!"
        else:
            print "** INFO: VBF reweighting requires the input samples to be in the order: node1, node2, node3, node4, node5, node19!"
            print "** INFO: I will assume they are passed in the correct order!"

            # Check that all input samples are present in sigList
            goodSamples = True
            for sig in inputSigList:
                if sig not in sigList:
                    goodSamples = False
                    print "** ERROR: VBF node ", sig, " not between provided signals  --> Skipping VBF reweighting!"

            # Apply the actual VBF reweighting
            if goodSamples:
                target_kl  = [float(kl)  for kl  in cfg.readListOption("VBF_rew::target_kl") ]
                target_cv  = [float(cv)  for cv  in cfg.readListOption("VBF_rew::target_cv") ]
                target_c2v = [float(c2v) for c2v in cfg.readListOption("VBF_rew::target_c2v")]
                target_xs  = float(cfg.readOption("VBF_rew::target_xs")) if cfg.hasOption("VBF_rew::target_xs") else -1.0

                omngr.makeVBFrew(inputSigList, target_kl, target_cv, target_c2v, target_xs)


fOut = ROOT.TFile(args.dir+"/" + 'analyzedOutPlotter.root', 'recreate')
omngr.saveToFile(fOut)
