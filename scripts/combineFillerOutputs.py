import modules.ConfigReader as cfgr
import modules.OutputManager as omng
import argparse
import fnmatch
import os
from os import path as op
import subprocess
import sys
import ROOT
import itertools

def findInFolder(folder, pattern):
    ll = []
    for ff in os.listdir(folder):
        if fnmatch.fnmatch(ff, pattern): ll.append(ff)
    if len(ll) == 0:
        mes = '*** WARNING: No valid file {} found in {}.'.format(pattern, folder)
        raise RuntimeError(mes)
    if len(ll) > 1:
        mes = '*** WARNING: Too many files found in {} using first one: {}.'
        raise RuntimeError(mes.format(folder, ll))
    return ll[0]

# prototype: ZZTo2L2Q_defaultBtagLLNoIsoBBTTCut_SSrlx_HHsvfit_deltaPhi
def retrieveHisto (rootFile, name, var, sel):
    fullName = name + '_' + sel + '_' + var
    if not rootFile.GetListOfKeys().Contains(fullName):
        print('*** WARNING: histo {} not available'.format(fullName))
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

def run_combination(outnames, workdir, cfgname, prefix, args):

    outname = findInFolder(workdir, prefix + outnames[0]+'.root')
    cfgname = op.join(workdir, cfgname)
    cfg        = cfgr.ConfigReader(cfgname)
    varList    = cfg.readListOption('general::variables')
    selDefList = cfg.readListOption('general::selections') ## the selection definition
    regDefList = cfg.readListOption('general::regions') ## the regions that are combined with the previous
    bkgList    = cfg.readListOption('general::backgrounds')
    dataList   = cfg.readListOption('general::data')
    sigList    = cfg.readListOption('general::signals')
    selList    = [x[0] + '_' + x[1] for x in list(itertools.product(selDefList, regDefList))]
     
    ## replace what was merged
    cfgsection = outnames[1]
    if cfg.hasSection(cfgsection):
        for groupname in cfg.config[cfgsection]:
            mergelist = cfg.readListOption(cfgsection + '::' + groupname)
            mergelistA = cfg.readOption(cfgsection + '::' + groupname)
            theList = None
            if mergelist[0] in dataList:
                theList = dataList
            elif mergelist[0] in sigList:
                theList = sigList
            elif mergelist[0] in bkgList:
                theList = bkgList

            for x in mergelist:
                try:
                    theList.remove(x)
                except (ValueError,AttributeError):
                    print("'{}' not in list!".format(x))
                    raise
            theList.append(groupname)
    else:
        raise ValueError('Section [{}] missing from {}.'.format(cfgsection, cfgname))
     
    rootfile = ROOT.TFile.Open(op.join(workdir, outname))
    print('... opening {}'.format(op.join(workdir, outname)))
     
    ROOT.gROOT.SetBatch(True)
    omngr = omng.OutputManager(rootfile,
                               selList, selDefList, regDefList, varList, [],
                               dataList, sigList, bkgList)
     
    ## always group together the data and rename them to 'data'
    # omngr.groupTogether('data_obs', dataList)
    if cfg.hasSection('pp_merge'):
        for groupname in cfg.config['pp_merge']:
            omngr.groupTogether(groupname, cfg.readListOption('pp_merge::'+groupname))
     
    if args.extBkg:
        if not args.extFile: print('** Error: no external file provided')
        print('... taking histos for bkg: {} from file : {}'.format(args.extBkg, args.extFile))
        #omngr.addHistos(args.extBkg, args.extFile)
        inFile = ROOT.TFile.Open(args.extFile)
        for sel in omngr.selections:
            for var in omngr.variables:
                htoadd_name = args.extBkg + '_' + sel + '_' + var
                htoadd = inFile.Get(htoadd_name)
                omngr.histos[htoadd_name] = htoadd.Clone(htoadd_name)
                

    if cfg.hasSection('pp_rebin'):
        for ri in cfg.config['pp_rebin']:
            opts = cfg.readListOption('pp_rebin::'+ri)
            if len(opts) < 4:
                print('** Error: Cannot interpret your rebin instructions: {}'.format(opts))
                continue
            var = opts[0]
            seldef = opts[1]
            newbinning = [float(x) for x in opts[2:]]
            if not var in omngr.variables:
                print(' * var {} was not plotted, will not rebin'.format(var))
                continue
            if not seldef in omngr.sel_def:
                print(' * sel {} was not used, will not rebin'.format(sel_def))
                continue            
            for reg in omngr.sel_regions:
                sel = seldef + '_' + reg
                omngr.rebin(var, sel, newbinning)
     
    if args.moreDY:
        omngr.scaleHistos('DY', args.moreDY)
    if args.moreTT:
        omngr.scaleHistos('TT', args.moreTT)
     
    # Apply the bTagReshape normalization factor to preserve the correct
    # yield of the MC processes.
    if cfg.hasSection('bTagRfactor'):
     
        # Read central correction
        central = eval(cfg.readOption('bTagRfactor::central')) if cfg.hasOption('bTagRfactor::central') else 1.
     
        print('--- Applying bTagReshape normalization factor ---')
        print('    >> Values for bTagReshape normalization factor:')
        print('    >> central : {}'.format(central))
     
        # Scale the histos
        for histoname in omngr.histos:
     
            # Don't apply scaling to data
            if 'data_obs' in histoname: continue
     
            # Apply central correction to ALL other histos since we
            # apply the bTagReshape at baseline level to all MC contributions
            h = omngr.histos[histoname]
            h.Scale(central)
     
     
    # C/D factor:
    # if not specified by --SBtoSR, 
    # the number inserted manually in config is used
    # if SBtoSRfactor == 1 in config and --SBtoSR is not specified, C/D computed dynamically
    if cfg.hasSection('pp_QCD'):
        SBtoSRforQCD = float(cfg.readOption('pp_QCD::SBtoSRfactor'))
        SBtoSRforQCDboost = float(cfg.readOption('pp_QCD::boostSBtoSR')) if cfg.hasOption('pp_QCD::boostSBtoSR') else 1.
        SBtoSRforQCDclass = float(cfg.readOption('pp_QCD::classSBtoSR')) if cfg.hasOption('pp_QCD::classSBtoSR') else 1.
        doUpDownQCD = eval(cfg.readOption('pp_QCD::doUnc')) if cfg.hasOption('pp_QCD::doUnc') else False
        computeSBtoSRdyn = False
        if SBtoSRforQCD == 1 and not args.SBtoSR: 
            computeSBtoSRdyn = True
        if args.SBtoSR: SBtoSRforQCD = args.SBtoSR
        omngr.makeQCD(
            SR            = cfg.readOption('pp_QCD::SR'),
            yieldSB       = cfg.readOption('pp_QCD::yieldSB'),
            shapeSB       = cfg.readOption('pp_QCD::shapeSB'),
            SBtoSRfactor  = SBtoSRforQCD,
            regionC       = cfg.readOption('pp_QCD::regionC'),
            regionD       = cfg.readOption('pp_QCD::regionD'),
            doFitIf       = cfg.readOption('pp_QCD::doFitIf'),
            fitFunc       = cfg.readOption('pp_QCD::fitFunc'),
            computeSBtoSR = computeSBtoSRdyn,
            boostSBtoSR   = SBtoSRforQCDboost,
            classSBtoSR   = SBtoSRforQCDclass
            )
        if doUpDownQCD:
            # Compute up/down variation of QCD by taking the shape
            # from SStight and the correction factor from OSinviso/SSinviso
            print('--- Computing Up/Down variation of QCD ---')
            omngr.makeQCD(
                SR            = cfg.readOption('pp_QCD::SR'),
                yieldSB       = cfg.readOption('pp_QCD::regionC'), # SStight
                shapeSB       = cfg.readOption('pp_QCD::regionC'), # SStight
                SBtoSRfactor  = SBtoSRforQCD,
                regionC       = cfg.readOption('pp_QCD::yieldSB'), # OSinviso
                regionD       = cfg.readOption('pp_QCD::regionD'), # SSinviso
                doFitIf       = cfg.readOption('pp_QCD::doFitIf'),
                fitFunc       = cfg.readOption('pp_QCD::fitFunc'),
                computeSBtoSR = computeSBtoSRdyn,
                doUpDown      = doUpDownQCD
                )
            if args.doSymmetricQCD:
                print('--- Symmetrizing templates of QCD ---')
                omngr.symmetrizeQCD()
            
    # VBF HH Reweighting
    # reads from the config the 6 input samples and the target couplings
    if cfg.hasSection('VBF_rew'):
        if eval(cfg.readOption('VBF_rew::doReweighting')):
            print('--- VBF Reweighting ---')
            inputSigList = cfg.readListOption("VBF_rew::inputSignals")
            if len(inputSigList) != 6:
                print('** ERROR: VBF reweighting requires 6 input samples! You only provided:', len(inputSigList), ' --> Skipping VBF reweighting!')
            else:
                print('** INFO: VBF reweighting requires the input samples to be in the order: node1, node2, node3, node4, node5, node19!')
                print('** INFO: I will assume they are passed in the correct order!')
     
                # Check that all input samples are present in sigList
                goodSamples = True
                for sig in inputSigList:
                    if sig not in sigList:
                        goodSamples = False
                        print('** ERROR: VBF node {} not between provided signals --> Skipping VBF reweighting!'.format(sig))
     
                # Apply the actual VBF reweighting
                if goodSamples:
                    target_kl  = [float(kl)  for kl  in cfg.readListOption('VBF_rew::target_kl') ]
                    target_cv  = [float(cv)  for cv  in cfg.readListOption('VBF_rew::target_cv') ]
                    target_c2v = [float(c2v) for c2v in cfg.readListOption('VBF_rew::target_c2v')]
                    target_xs  = float(cfg.readOption('VBF_rew::target_xs')) if cfg.hasOption('VBF_rew::target_xs') else -1.0
     
                    omngr.makeVBFrew(inputSigList, target_kl, target_cv, target_c2v, target_xs)
     
    fName = ('combined_' +
             ('moreTT_' + str(args.moreTT).replace('.', 'p') + '_' if args.moreTT else '') + outname)
    fOut = ROOT.TFile(op.join(workdir, fName), 'recreate')
    omngr.saveToFile(fOut)

def run_hadd(name, workdir, args):
    """Adds histograms of split files into a single one."""
    fullname = op.join(workdir, name)
    comm = 'hadd -f ' + fullname + '.root' + ' ' + fullname + '_*.root'
    
    p = subprocess.Popen(comm, shell=True, bufsize=2048, stdin=subprocess.PIPE)
    p.wait()
    if p.returncode != 0:
        m = "The hadd'ing step did not work!"
        raise RuntimeError()

def run_harvesting(outn, workdir, cfgname, prefix, args):
    """
    Gets the histograms from the nominal, up and down skims and places them in the same file.
    Usually run only for obtaining the limits, not for plotting the distributions.
    """
    
    # Create list of all uncertainty subdirectories
    uncDirs = os.listdir(workdir)
    
    # Input vars to be saved in the merged mainCfg
    allVars = None
     
    # Check that all jobs have finished
    for uncDir in uncDirs:
     
        # Skip 'total' subdirectory if already present
        if uncDir == 'total':
            continue
     
        # Read unc subdirectories
        inDir = os.path.join(workdir, uncDir)
        print('Checking subdir: {}'.format(inDir))
     
        # Get list of log files
        allFiles = [f for f in os.listdir(inDir) if os.path.isfile(os.path.join(inDir, f))]
        logFiles = [f for f in allFiles if 'log_' in f]
     
        for logFile in logFiles:
            lastLine = os.popen('tail -n1 ' + inDir + '/' + logFile).read()
            if 'exiting' in lastLine:
                continue
            else:
                mes = inDir + '/' + logFile + ' did not finish properly, exiting!'
                sys.exit('ERROR: ' + mes)
     
        # Read the main config for each unc and store the variables names
        with open(cfgname) as cfgfile:
            for line in cfggile:
                if 'JER' in inDir: continue
                if line.startswith('variables'):
                    varNames = line.split('=')[1] # Get variables only
                    varNames = varNames.strip()   # Remove carriage return
                    if allVars is None:
                        allVars = varNames
                    else:
                        allVars = allVars + ', ' + varNames
     
    print('--> All log files look ok!')
     
    # Create a new directory to store the merged file
    newDir = os.path.join(workdir, '/total')
    if not os.path.exists(newDir):
        os.makedirs(newDir)
     
    # Copy the samples and selection config files from the central case
    copyCommand = 'cp ' + workdir + '/central/s*cfg ' + newDir + '/'
    print(copyCommand)
    os.system(copyCommand)
    
    # Create a new main config starting from the central config,
    # but containing all the variables (central and shifted ones)
    newMainCfg = open(os.path.join(newDir, cfgname), 'w')
    centralCfg = open(os.path.join(workdir, 'central', cfgname))
    for line in centralCfg:
        # Edit line with variables
        if line.startswith('variables'):
            line = 'variables = ' + allVars + '\n'
        newMainCfg.write(line)
    newMainCfg.close()
    centralCfg.close()
            
    # Create "NEWoutput_*.root" file and add "JERup/JERdown" name to JER shifted histos
    for uncDir in uncDirs:
     
        # Skip 'total' subdirectory if already present
        if uncDir == 'total':
            continue
     
        # Read unc subdirectories
        inDir = os.path.join(workdir, uncDir)
        print('Changing names in: {}'.format(inDir))
     
        # Get list of root files
        allFiles  = [f for f in os.listdir(inDir) if os.path.isfile(os.path.join(inDir, f))]
        rootFiles = [f for f in allFiles if outn[0] + '_' in f and prefix not in f]
     
        for rootFile in rootFiles:
     
            # Get original name and create a new one
            originalName = inDir+'/'+rootFile
            newName = originalName.replace(outn[0] + '_', prefix + outn[0]+'_')
     
            # For central subdirectory just copy/paste the
            # the root file with the updated name
            if 'central' in uncDir:
                os.system('cp %s %s' % (originalName, newName))
     
            # Else if running on the shifted subdirectories
            # loop on all the histos and change names if needed
            else:
     
                # List to store the histos to be copied in the new file
                listHistos = []
     
                # Read the histos from the original file
                fin = ROOT.TFile(originalName)
                for key in fin.GetListOfKeys():
                    kname = key.GetName()
                    template = fin.Get(kname)
     
                    # Do not copy the data for shifted subdirectories
                    if 'data_obs' in kname: continue
     
                    # Change name to histos in the JER subdirectories
                    # by adding '_JERup' or '_JERdown'
                    if 'JER' in uncDir:
                        template.SetName(kname+'_'+uncDir)
                        template.SetTitle(kname+'_'+uncDir)
     
                    # Store histos to be save in the new file
                    listHistos.append(template.Clone())
     
                # Create the new file to store the histos with the correct name
                fout = ROOT.TFile(newName,'RECREATE')
                fout.cd()
                for h in listHistos:
                    h.Write()
                fout.Close()

    
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Command line parser of plotting options')
    parser.add_argument('--dir', dest='dir', help='analysis output folder name',
                        default='/data_CMS/cms/' + os.environ['USER'] + '/HHresonant_hist/')
    parser.add_argument('--tag', help='tag name used after skimming', required=True)
    parser.add_argument('--cfg', default='mainCfg_*.cfg', help='configuration file', required=True)
    parser.add_argument('--moreDY', type=float, dest='moreDY', help='increase DY by factor moreDY', default=None)
    parser.add_argument('--moreTT', type=float, dest='moreTT', help='increase TT by factor moreTT', default=None)
    parser.add_argument('--SBtoSR', type=float, dest='SBtoSR', help='specity manually the SBtoSR factor', default=None)
    parser.add_argument('--extBkg',  dest='extBkg', help='add a bkg from external file', default=None)
    parser.add_argument('--extFile', dest='extFile', help='add a bkg from external file', default=None)
    parser.add_argument('--doSymmetricQCD', type=bool, dest='doSymmetricQCD', help='symmetrize QCD templates', default=True)
    args = parser.parse_args()

    workdir = op.join(args.dir, args.tag)
    cfgname = findInFolder(workdir, args.cfg)
    outNames = {'outPlots': 'merge_plots', 'outLimits': 'merge_limits'}

    for outn in outNames.items():
        prefix = ''
        # if outn[0] == 'outLimits': #harvesting systematics is only required for the limits
        #     prefix = 'NEW_'
        #     run_harvesting(outn, workdir, cfgname, prefix, args)
        run_hadd(prefix + outn[0], workdir, args)
        run_combination(outn, workdir, cfgname, prefix, args)
