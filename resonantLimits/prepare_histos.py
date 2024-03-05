# coding: utf-8

_all_ = [ ]

import os
import argparse
import ROOT

def parseOptions():
    usage = 'usage: %prog [args.ons] datasetList (-h for help)'
    parser = argparse.ArgumentParser(description=usage)

    parser.add_argument('-f', '--filename', default='', help='input plots')
    parser.add_argument('--outdir', default=None, help='output directory')
    parser.add_argument('-o', '--outname', default='prepared_outLimits.root',
                        help='output plots')
    parser.add_argument('-c', '--channel', default='TauTau', help='channel')
    parser.add_argument('-y', '--period', default='UL18', choices=('UL16', 'UL16APV', 'UL17', 'UL18'),
                        help='data period')
    parser.add_argument('-q', '--dynamQCD', default=0, type=int,
                        help='1:do QCD as rateParam / 0:read QCD from file')
    parser.add_argument('-B', '--renameH', type=int, default=1,
                        help='1:rename singleH bkgs / 0:do not rename singleH bkgs')
    parser.add_argument('--binOptimization', action='store_true',
                        help='disable removal of negative bins before doing binning args.mization')
    parser.add_argument('--acceptanceScale', action='store_true',
                        help='do acceptance scaling of uncertainties')

    return parser.parse_args()

args = parseOptions()

systNamesOUT = [
    # hadronic tau energy scale for decay mode 0
    'CMS_scale_t_DM0_'    + args.period,
    # hadronic tau energy scale for decay mode 1
    'CMS_scale_t_DM1_'    + args.period,
    # hadronic tau energy scale for decay mode 10
    'CMS_scale_t_DM10_'   + args.period,
    # hadronic tau energy scale for decay mode 11
    'CMS_scale_t_DM11_'   + args.period,

    # energy scale of electrons reconstructed as hadronic taus for decay mode 0
    'CMS_scale_t_eFake_'  + args.period + '_DM0',
    # energy scale of electrons reconstructed as hadronic taus for decay mode 1
    'CMS_scale_t_eFake_'  + args.period + '_DM1',
    # energy scale of muons reconstructed as hadronic taus
    'CMS_scale_t_muFake_' + args.period,

    # jet energy scale
    'CMS_JES_Abs',
    'CMS_JES_Abs_'       + args.period,
    'CMS_JES_BBEC1',
    'CMS_JES_BBEC1_'     + args.period,
    'CMS_JES_EC2',
    'CMS_JES_EC2_'       + args.period,
    'CMS_JES_FlavQCD', 
    'CMS_JES_HF',
    'CMS_JES_HF_'        + args.period,             
    'CMS_JES_RelBal', 
    'CMS_JES_RelSample_' + args.period,

    # # jets faking taus
    # 'CMS_bbtt_' + args.period + 'jetTauFakes_Barrel',
    # 'CMS_bbtt_' + args.period + 'jetTauFakes_Endcap',

    # # jet energy resolution
    # 'CMS_res_j_' + args.period,

    # # b tagging scale factors
    # 'CMS_btag_LF_2016_2017_2018',
    # 'CMS_btag_HF_2016_2017_2018',
    # 'CMS_btag_cferr1_2016_2017_2018',
    # 'CMS_btag_cferr2_2016_2017_2018',
    # 'CMS_btag_hfstats1_2016_2017_2018',
    # 'CMS_btag_hfstats2_2016_2017_2018',
    # 'CMS_btag_lfstats1_2016_2017_2018',
    # 'CMS_btag_lfstats2_2016_2017_2018',

    # # pile-up jet id scale factors
    # 'CMS_eff_j_PUJET_id_' + args.period,

    # # deep tau scale factors versus electrons
    # 'CMS_bbtt_' + args.period + '_etauFR_barrel',
    # 'CMS_bbtt_' + args.period + '_etauFR_endcap',
    ]

systNames = ['tesXXX_DM0',
             'tesXXX_DM1',
             'tesXXX_DM10',
             'tesXXX_DM11',

             'eesXXX_DM0',
             'eesXXX_DM1',
             'mesXXX',

             'jesXXX1', 'jesXXX2', 'jesXXX3', 'jesXXX4', 'jesXXX5',
             'jesXXX6', 'jesXXX7', 'jesXXX8', 'jesXXX9', 'jesXXX10', 'jesXXX11',

             'jesXXXTot']

## if running WITHOUT the shape syst, uncomment these 2 lines:
# systNamesOUT = []
# systNames = []

ROOT.TH1.AddDirectory(ROOT.kFALSE)
inFile = ROOT.TFile.Open(args.filename, 'READ')

histolist = inFile.GetListOfKeys()
toth = histolist.GetSize()
outputDir = os.path.dirname(args.filename) if args.outdir is None else args.outdir
outFile = ROOT.TFile.Open(os.path.join(outputDir, args.outname), 'RECREATE')

# histos_syst_up, histos_syst_down = ([] for _ in range(2))
systReplace = lambda s1, s2 : s1.replace('XXX', s2).replace('tes','tau').replace('ees','ele').replace('mes','mu').replace('jes','jet')
yieldFolder = 'scales' + args.period + '/'

itkeys = ROOT.TIter(histolist)
for ih in range(toth):
    inFile.cd()
    key = itkeys.Next()
    kname = key.GetName()
    template = inFile.Get(kname)
    if ih%500==0:
        print('{}/{}: {} ({})'.format(ih, toth, kname, template.ClassName()))

    # If QCD is read from file and not computed as rate parameter in the datacards
    # we can skip all the SStight/OSinviso/SSinviso shifted template and save
    # a lot of time and space
    if args.dynamQCD == 0:
        if (any(x in kname for x in ('SStight', 'OSiniso', 'SSinviso')) and
            any(x in kname for x in ('Up', 'Down', 'up', 'down'))):
            continue

    # protection against empty bins
    changedInt = False
    integral = template.Integral()
    if not args.binOptimization:
        for ibin in range(1,template.GetNbinsX()+1):
  
            if 'data' in kname:
                continue
  
            if template.GetBinContent(ibin) <= 0:
                changedInt = True
                template.SetBinContent(ibin,1E-9)
                template.SetBinError(ibin,1E-9)

        if integral>0:
            if changedInt and template.Integral()>0:
                template.Scale(integral/template.Integral())
        else:
            template.Scale(-100.) # set to negative in order to identify the problematic templates to be excluded in write_card.py

    if args.acceptanceScale and 'TH1' in template.ClassName():
        for isyst in range(len(systNames)):
            lineToCheckDown = systReplace(systNames[isyst], 'down')
            lineToCheckUp = systReplace(systNames[isyst], 'up')
            found = -1

            if kname.endswith(lineToCheckDown):
                appString, remString = 'Down', 'down'
                found = 1
            elif kname.endswith(lineToCheckUp):
                appString, remString = 'Up', 'up'
                found = 0

            if found>=0:
                names = kname.split('_')

                for i in range(len(names)):
                    if any(x in names[i] for x in ('res1b', 'res2b', 'boosted')):
                        for j in range(1, i):
                            names[0] += '_'+str(names[j])
        #if not (names[i]== 'singleT'): names[1] = names[i]
                        names[1] = names[i]
                        break

                proc = names[0]
                yieldName = yieldFolder + args.channel + '_' + names[1]
                yieldName = yieldName + '_' + systNames[isyst].replace('XXX','').replace('jes','jes_') + '.txt'
                with open(yieldName) as inp:
                    scale = 1.000
                    for line in inp:
                        words = line.split()
                        if words[0] == proc:
                            scale = float(words[1+found])
                            break
                template.Scale(scale)

                kname = kname.replace('_'+syst_replace(systNames[isyst], remString), '')
                kname = kname + '_' + systNamesOUT[isyst] + appString

            # Fix the signal names after the systs have been read
        #if "GGHH_NLO" in kname: kname = kname.replace("GGHH_NLO","ggHH").replace("_xs","_kt_1_hbbhtt").replace("cHHH", "kl_")
    #if "VBFHH"    in kname: kname = kname.replace("VBFHH","qqHH").replace("C3","kl").replace("_xs","_hbbhtt")

# Rename singleH processes to use the HHModel BR scaling
# ggH --> ggH_htt
# qqH --> qqH_htt
# WH  --> WH_htt
# ZH  --> ZH_hbb
# ttH --> ttH_hbb
        if args.renameH:
            if kname[0:4] == 'ggH_': kname = kname.replace('ggH_', 'ggH_htt_')
            if kname[0:4] == 'qqH_': kname = kname.replace('qqH_', 'qqH_htt_')
            if kname[0:3] == 'WH_' : kname = kname.replace('WH_' , 'WH_htt_')
            if kname[0:3] == 'ZH_' : kname = kname.replace('ZH_' , 'ZH_hbb_')
            if kname[0:4] == 'ttH_': kname = kname.replace('ttH_', 'ttH_hbb_')

    outFile.cd()
    template.SetName(kname)
    template.SetTitle(kname)
    template.Write()
    del template
    
inFile.Close()
outFile.Close()
