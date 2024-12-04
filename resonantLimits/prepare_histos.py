# coding: utf-8

_all_ = [ 'prepare_histos', 'set_shape_syst_list' ]

import os
import re
import argparse
import ROOT

def set_shape_syst_list(period, channel, category):
    """Shape systematic naming mapping: Skimms -> Datacards."""
    assert any(category == x for x in ('res1b', 'res2b', 'boostedL_pnet', 'all'))
    pmap = {'UL16': '2016', 'UL16APV': '2016APV', 'UL17': '2017', 'UL18': '2018'}

    names = {
        # hadronic tau energy scale for decay mode 0
        "tes_DM0"  : "CMS_scale_t_DM0_"    + pmap[period],
        # hadronic tau energy scale for decay mode 1
        "tes_DM1"  : "CMS_scale_t_DM1_"    + pmap[period],
        # hadronic tau energy scale for decay mode 10
        "tes_DM10" : "CMS_scale_t_DM10_"   + pmap[period],
        # hadronic tau energy scale for decay mode 11
        "tes_DM11" : "CMS_scale_t_DM11_"   + pmap[period],
     
        # energy scale of electrons reconstructed as hadronic taus for decay mode 0
        "ees_DM0" : "CMS_scale_t_eFake_"  + pmap[period] + "_DM0",
        # energy scale of electrons reconstructed as hadronic taus for decay mode 1
        "ees_DM1" : "CMS_scale_t_eFake_"  + pmap[period] + "_DM1",
        # energy scale of muons reconstructed as hadronic taus
        "mes" : "CMS_scale_t_muFake_" + pmap[period],
     
        # jet energy scale
        "jes_1"  : "CMS_j_Abs",
        "jes_2"  : "CMS_j_Abs_"       + pmap[period],
        "jes_3"  : "CMS_j_BBEC1",
        "jes_4"  : "CMS_j_BBEC1_"     + pmap[period],
        "jes_5"  : "CMS_j_EC2",
        "jes_6"  : "CMS_j_EC2_"       + pmap[period],
        "jes_7"  : "CMS_j_FlavQCD", 
        "jes_8"  : "CMS_j_HF",
        "jes_9"  : "CMS_j_HF_"        + pmap[period],             
        "jes_10" : "CMS_j_RelBal", 
        "jes_11" : "CMS_j_RelSample_" + pmap[period],
     
        # jet energy resolution
        # "CMS_res_j_" + pmap[period], # MISSING, NEEDS RESKIMMING WITH UP/DOWN VARIATIONS!!!!!!!!!!!!
               
        # pile-up jet id scale factors
        "PUjetIDSF": "CMS_eff_j_PUJET_id_" + pmap[period],

        # jets faking taus # LIKELY REFERRING TO GENUINE TAUS BELOW, BUT NOT 100% SURE
        # "CMS_bbtt_" + pmap[period] + "jetTauFakes_Barrel",
        # "CMS_bbtt_" + pmap[period] + "jetTauFakes_Endcap",

        # deep tau scale factors versus electrons
        "etauFR_barrel": "CMS_bbtt_" + pmap[period] + "_etauFR_barrel",
        "etauFR_endcap": "CMS_bbtt_" + pmap[period] + "_etauFR_endcap",

        # deep tau scale factors versus muons
        "mutauFR_etaLt0p4"    : "CMS_bbtt_" + pmap[period] + "_mutauFR_etaLt0p4",
        "mutauFR_eta0p4to0p8" : "CMS_bbtt_" + pmap[period] + "_mutauFR_eta0p4to0p8",
        "mutauFR_eta0p8to1p2" : "CMS_bbtt_" + pmap[period] + "_mutauFR_eta0p8to1p2",
        # "mutauFR_eta1p2to1p7" : "CMS_bbtt_" + pmap[period] + "_mutauFR_eta1p2to1p7",
        "mutauFR_etaGt1p7"    : "CMS_bbtt_" + pmap[period] + "_mutauFR_etaGt1p7",

        # "tauid_2d_stat0_DM0"             : "CMS_eff_t_id_Stat0_" + pmap[period] + "_DM0",
        # "tauid_2d_stat1_DM0"             : "CMS_eff_t_id_Stat1_" + pmap[period] + "_DM0",
        # "tauid_2d_stat0_DM1"             : "CMS_eff_t_id_Stat0_" + pmap[period] + "_DM1",
        # "tauid_2d_stat1_DM1"             : "CMS_eff_t_id_Stat1_" + pmap[period] + "_DM1",
        # "tauid_2d_stat0_DM10"            : "CMS_eff_t_id_Stat0_" + pmap[period] + "_DM10",
        # "tauid_2d_stat1_DM10"            : "CMS_eff_t_id_Stat1_" + pmap[period] + "_DM10",
        # "tauid_2d_stat0_DM11"            : "CMS_eff_t_id_Stat0_" + pmap[period] + "_DM11",
        # "tauid_2d_stat1_DM11"            : "CMS_eff_t_id_Stat1_" + pmap[period] + "_DM11",
        # "tauid_2d_stat0gt140"            : "CMS_eff_t_id_Stat0Gt140_" + pmap[period],
        # "tauid_2d_stat1gt140"            : "CMS_eff_t_id_Stat1Gt140_" + pmap[period],
        # "tauid_2d_extrapgt140"           : "CMS_eff_t_id_ExtrapGt140",
        # "tauid_2d_systuncorrdmeras_DM0"  : "CMS_eff_t_id_" + pmap[period] + "_DM0",
        # "tauid_2d_systuncorrdmeras_DM1"  : "CMS_eff_t_id_" + pmap[period] + "_DM1",
        # "tauid_2d_systuncorrdmeras_DM10" : "CMS_eff_t_id_" + pmap[period] + "_DM10",
        # "tauid_2d_systuncorrdmeras_DM11" : "CMS_eff_t_id_" + pmap[period] + "_DM11",
        "tauid_2d_systcorrdmeras"        : "CMS_eff_t_id_2016_2017_2018",
        "tauid_2d_systcorrdmuncorreras"  : "CMS_eff_t_id_" + pmap[period],
        "tauid_2d_systcorrerasgt140"     : "CMS_eff_t_id_2016_2017_2018_Gt140",

        # "trigSF_met"  : "CMS_bbtt_" + pmap[period] + "_trigSFMET",
        # "trigSF_stau" : "CMS_bbtt_" + pmap[period] + "_trigSFSingleTau",
    }

    # if channel == "ETau":
    #     names.update({
    #         "trigSFele" : "CMS_bbtt_" + pmap[period] + "_trigSFEle",
    #         })
    # elif channel == "MuTau":
    #     names.update({
    #         "trigSFmu"  : "CMS_bbtt_" + pmap[period] + "_trigSFMu",
    #         })

    # In 2016 we only use the SingleEle trigger in ETau so no uncertainties on the tau triggers should be added
    # if '16' not in pmap[period] and channel != "ETau":
    #     names.update({
    #         "trigSFTauDM0"  : "CMS_bbtt_" + pmap[period] + "_trigSFTauDM0",
    #         "trigSFTauDM1"  : "CMS_bbtt_" + pmap[period] + "_trigSFTauDM1",
    #         "trigSFTauDM10" : "CMS_bbtt_" + pmap[period] + "_trigSFTauDM10",
    #         "trigSFTauDM11" : "CMS_bbtt_" + pmap[period] + "_trigSFTauDM11"
    #         })

    if 'boosted' not in category:
        # b tagging scale factors
        names.update({
            "bTagweightReshapeLF"       : "CMS_btag_LF_2016_2017_2018",
            "bTagweightReshapeHF"       : "CMS_btag_HF_2016_2017_2018",
            "bTagweightReshapeCFERR1"   : "CMS_btag_cferr1_2016_2017_2018",
            "bTagweightReshapeCFERR2"   : "CMS_btag_cferr2_2016_2017_2018",
            "bTagweightReshapeHFSTATS1" : "CMS_btag_hfstats1_" + pmap[period],
            "bTagweightReshapeHFSTATS2" : "CMS_btag_hfstats2_" + pmap[period],
            "bTagweightReshapeLFSTATS1" : "CMS_btag_lfstats1_" + pmap[period],
            "bTagweightReshapeLFSTATS2" : "CMS_btag_lfstats2_" + pmap[period]})
            
    return names

def prepare_histos(args):
    if args.pDNN:
        subdirs = ["Spin{}_Mass{}".format(args.spin, m) for m in args.masses]
    else:
        subdirs = ["."]

    if not args.noShapeUnc:
        systNames = set_shape_syst_list(args.period, args.channel, "all")

    if args.acceptanceScale:
        yieldFolder = 'scales' + args.period + '/'

    for subd in subdirs:
        inFile = ROOT.TFile.Open(os.path.join(args.indir, subd, args.filename), 'READ')
        histolist = inFile.GetListOfKeys()
        toth = histolist.GetSize()
        outputDir = os.path.join(args.indir, subd)
        outFile = ROOT.TFile.Open(os.path.join(outputDir, args.outname), 'RECREATE')
                
        itkeys = ROOT.TIter(histolist)
        for ih in range(toth):
            inFile.cd()
            key = itkeys.Next()
            kname = key.GetName()            
            template = inFile.Get(kname)
            if ih%3000==0:
                print('{}/{}: {} ({})'.format(ih, toth, kname, template.ClassName()))

            kname2 = kname
            # If QCD is read from file and not computed as rate parameter in the datacards
            # we can skip all the SStight/OSinviso/SSinviso shifted template and save
            # a lot of time and space
            if args.dynamQCD == 0:
                if (any(x in kname for x in ('SStight', 'OSinviso', 'SSinviso')) and
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
                        template.SetBinContent(ibin, 1E-9)
                        template.SetBinError(ibin, 1E-9)
                        
                if integral>0:
                    if changedInt and template.Integral()>0:
                        template.Scale(integral/template.Integral())
                else:
                    # set to negative in order to identify the problematic templates to be excluded in write_card.py
                    template.Scale(-100.) 

            if 'TH1' in template.ClassName() and not args.noShapeUnc:

                for nameIn, nameOut in systNames.items():
                    found = -1
                    if any(kname.endswith(nameIn + x) for x in
                           ('Up', 'Down', 'up', 'down', '_Up', '_Down', '_up', '_down')):
                        if kname.endswith('Down') or kname.endswith('down'):
                            found = 1
                            direction = 'Down'
                        elif kname.endswith('Up') or kname.endswith('up'):
                            found = 0
                            direction = 'Up'

                    if found == -1:
                        continue

                    if args.acceptanceScale:
                        names = kname.split('_')
                        for i in range(len(names)):
                            if any(x in names[i] for x in ('res1b', 'res2b', 'boosted')):
                                for j in range(1, i):
                                    names[0] += '_' + str(names[j])
                                names[1] = names[i]
                                break

                        proc = names[0]
                        yieldName = yieldFolder + args.channel + '_' + names[1]
                        yieldName = yieldName + '_' + nameIn.replace('XXX','').replace('jes','jes_') + '.txt'
                        with open(yieldName) as inp:
                            scale = 1.0
                            for line in inp:
                                words = line.split()
                                if words[0] == proc:
                                    scale = float(words[1+found])
                                    break
                        template.Scale(scale)

                    kname = re.sub(nameIn + '.*$', '', kname)
                    kname += nameOut + direction

                    break # if found != -1 a name match was already found, which should be unique
                
            outFile.cd()
            template.SetName(kname)
            template.SetTitle(kname)
            template.Write()
            del template
           
        inFile.Close()
        outFile.Close()

if __name__ == '__main__':
    ROOT.TH1.AddDirectory(ROOT.kFALSE)
    
    usage = 'usage: %prog [args.ons] datasetList (-h for help)'
    parser = argparse.ArgumentParser(description=usage)

    parser.add_argument('--filename', default='combined_outLimits.root',
                        help='Input file with combined histograms.')
    parser.add_argument('--indir', default='', help='Input directory.')
    parser.add_argument('--outname', default='prepared_outLimits.root',
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
    parser.add_argument('--pDNN', action='store_true', help='Using the parameterised DNN.')
    parser.add_argument('--masses', nargs='+',
                        default=("250", "260", "270", "280", "300", "320", "350", "400", "450", "500", "550", "600", "650", "700", "750", "800", "850", "900", "1000", "1250", "1500", "1750", "2000", "2500", "3000"),
                        help='Resonant mass points.')
    parser.add_argument('--spin', default="0", choices=("0", "2"), help='Spin hypothesis.')
    parser.add_argument('-u', '--noShapeUnc', action='store_true', help='disable shape uncertainties')
    FLAGS = parser.parse_args()

    prepare_histos(FLAGS)
