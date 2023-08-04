"""
Author: Bruno Alves (bruno.alves@cern.ch)
June 2023

This script calculated the purity of the ttbar control regions used to compute ttbar scale factors,
i.e., how much ttbar we have compared to other background sources.
The ttbar purity should be as high as possible when calculating the SFs.

Run the script with `--help` for information on its arguments.
"""
import modules.ConfigReader as cfgr
import argparse
import os
import ROOT

def retrieveHisto(rootFile, bkg, sel, reg, var):
    hname = '_'.join((bkg, sel, reg, var))
    if not rootFile.GetListOfKeys().Contains(hname):
        print('*** WARNING: histo {} not available'.format(hname))
        return None
    hFound = rootFile.Get(hname)
    return hFound

def run_purity(args):
    cfg = cfgr.ConfigReader(args.cfg)

    var = "bjet1_pt"
    varList    = cfg.readListOption('general::variables')
    assert var in varList
    
    selection = "ttCR_invMcut"
    selDefList = cfg.readListOption('general::selections') ## the selection definition
    assert selection in selDefList

    region = "SR"
    regDefList = cfg.readListOption('general::regions') ## the regions that are combined with the previous
    assert region in regDefList
    
    bkgList = cfg.readListOption('general::backgrounds')
     
    ## replace what was merged
    print("Backgrounds removed:")
    cfgsection = "merge_limits"
    if cfg.hasSection(cfgsection):
        for groupname in cfg.config[cfgsection]:
            mergelist = cfg.readListOption(cfgsection + '::' + groupname)
            theList = None
            if mergelist[0] not in bkgList:
                continue

            theList = bkgList    
            for x in mergelist:
                try:
                    theList.remove(x)
                    if args.debug:
                        print("  - {}".format(mergelist[0]))

                except (ValueError,AttributeError):
                    print("'{}' not in list!".format(x))
                    raise
            theList.append(groupname)
    else:
        raise ValueError('Section [{}] missing from {}.'.format(cfgsection, args.cfg))
     
    rootfile = ROOT.TFile.Open( os.path.join(args.dir, "combined_outLimits.root"))
    print('Opening file {}'.format(args.dir))
     
    ROOT.gROOT.SetBatch(True)

    print("Backgrounds considered: ")
    total = 0.
    for bkg in bkgList:
        if args.debug:
            print("  - {}".format(bkg))
        h = retrieveHisto(rootfile, bkg, selection, region, var)
        total += h.Integral()

    httbar = rootfile.Get('_'.join(('TT', selection, region, var)))
    print("TTbar purity: {}%".format( (httbar.Integral() / total) * 100))

    
if __name__ == '__main__':
    run_example = "python scripts/ttSF_purity.py --dir /data_CMS/cms/alves/HHresonant_hist/ttSF_ETau/ --cfg config/mainCfg_ETau_ttCR_UL18.cfg --debug"
    parser = argparse.ArgumentParser(description='Finds the ttbar purity in a specific region.\nRun example:\n' + run_example,
                                     formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument('--dir', dest='dir', help='analysis output folder name',
                        default='/data_CMS/cms/' + os.environ['USER'] + '/HHresonant_hist/')
    parser.add_argument('--cfg', default='mainCfg_*.cfg', help='configuration file', required=True)
    parser.add_argument('--debug', action="store_true", help='print debug information')
    args = parser.parse_args()

    run_purity(args)
