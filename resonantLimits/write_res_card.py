# coding: utf-8

_all_ = [ ]

import sys
import os
import argparse
import ROOT
from ConfigReader import *
from systReader import *

def parseOptions():
    usage = ('usage: %prog [options] datasetList\n %prog -h for help')
    parser = argparse.ArgumentParser(description=usage)
    
    parser.add_argument('-f', '--filename', default='', help='input plots')
    parser.add_argument('--indir', default='', help='input dir')
    parser.add_argument('-o', '--outdir', default='', help='output dir')
    parser.add_argument('-c', '--channel', default='TauTau', help='final state')
    parser.add_argument('-i', '--config', default='', help='config file')
    parser.add_argument('-s', '--overSel', default='', help='overwrite selection string')
    parser.add_argument('-y', '--period', default='UL18', help='data period')
    parser.add_argument('--signal', default='ggFRadion', help='signal')
    parser.add_argument('-v', '--var', default='DNNoutSM_kl_1',
                        help='variable to fit')
    parser.add_argument('-l', '--selections', required=True, nargs='+',
                        help='categories to write cards for')
    parser.add_argument('-m', '--masses', required=True, nargs='+',
                        help='resonant masses to consider')
    parser.add_argument('-u', '--noShapeUnc', action='store_false', help='disable shape uncertainties')
    parser.add_argument('-r', '--isResonant', action='store_true', help='is Resonant analysis')
    parser.add_argument('-b', '--binbybin', action='store_true', help='add bin by bins systematics')
    parser.add_argument('-t', '--theory', action='store_true', help='add theory systematics')
    parser.add_argument('--ws', dest='makeworkspace', action='store_true', default=False)

    return parser.parse_args()

def writeCard(backgrounds, signals, select, varfit, regions=()):
    if any(x in select for x in ('0b0j', '1b1j', '2b0j', 'boosted', 'VBFloose')):
        variable = varfit
    elif 'GGFclass'   in select : variable = 'mdnn__v2__kl1_c2v1_c31__hh_ggf',
    elif 'VBFclass'   in select : variable = 'mdnn__v2__kl1_c2v1_c31__hh_vbf',
    elif 'ttHclass'   in select : variable = 'mdnn__v2__kl1_c2v1_c31__tth',
    elif 'TTlepclass' in select : variable = 'mdnn__v2__kl1_c2v1_c31__tt_lep',
    elif 'TThadclass' in select : variable = 'mdnn__v2__kl1_c2v1_c31__tt_fh',
    elif 'DYclass'    in select : variable = 'mdnn__v2__kl1_c2v1_c31__dy',
    else: raise ValueError('Selection not supported: {}'.format(select))

    svdir = select + '_' + variable
    dname = opt.outdir + '_' + opt.channel
    out_dir = os.path.join(opt.indir, 'cards_{}/{}/'.format(dname, svdir))
    cmd = 'mkdir -p {}'.format(out_dir)
    os.system(cmd)

    #read config
    categories = []
    categories.append((0,select))
    MCbackgrounds=[]
    processes=[]
    inRoot = ROOT.TFile.Open(opt.filename)

    for bkg in backgrounds:
        templateName = '{}_{}_SR_{}'.format(bkg, select, variable)
        try:
            template = inRoot.Get(templateName)
        except (ReferenceError, AttributeError):
            print('File: {}'.format(opt.filename))
            print('Template: {}'.format(templateName))
            raise

        if template.Integral()>0.000001:
            processes.append(bkg)
            if bkg != 'QCD':
                MCbackgrounds.append(bkg)

    rates = []
    iQCD = -1
    totRate = 0
    suffix_str = '{}_{}_{}'.format(select, regions[0], variable)
    templateName = 'data_obs_' + suffix_str
    template = inRoot.Get(templateName)
    obs = template.GetEntries()

    for proc in range(len(backgrounds)):
        if 'QCD' in backgrounds[proc] and regions[0] != 'SR':
            rates.append(-1)
            iQCD = proc
        else:
            templateName = '{}_'.format(backgrounds[proc]) + suffix_str
            template = inRoot.Get(templateName)
            brate = template.Integral()
            rates.append(brate)
            totRate = totRate + brate
    if iQCD >= 0 and regions[0] != 'SR':
        rates[iQCD] = ROOT.TMath.Max(0.0000001,obs-totRate)
    
    # for proc in range(len(signals)):
    #     templateName = '{}_'.format(signals[proc]) + suffix_str
    #     template = inRoot.Get(templateName)
    #     if template.Integral() <= 0:
    #         print('[WARNING] Zero integral for signal {} in category {}.'.format(signals[proc], select))
    #         return -1

    outstr = '_{}_{}_{}_13TeV'.format(opt.period, opt.channel, select)
    hh_prefix = 'hhres' if opt.isResonant else 'hh'
    hh_ext = lambda ext : ('.{}.'.format(signals[0]) if opt.isResonant else '.') + ext

    if regions[0] == 'SR':
        for proc in range(len(signals)):
            templateName = "{}_".format(signals[proc]) + suffix_str
            template = inRoot.Get(templateName)
            srate = template.Integral()
            rates.append(srate)

        syst = systReader('../config/systematics_'+opt.period+'.cfg', signals, backgrounds, None)
        syst.writeOutput(False)
        syst.verbose(True)
        syst.addSystFile('../config/systematics_DY_'+opt.period+'.cfg')
        if opt.theory:
            syst.addSystFile('../config/syst_th.cfg')

        if(opt.channel == 'ETau'):
            syst.addSystFile('../config/systematics_etau_' + opt.period + '.cfg')
        elif(opt.channel == 'MuTau'):
            syst.addSystFile('../config/systematics_mutau_' + opt.period + '.cfg')
        elif(opt.channel == 'TauTau'):
            syst.addSystFile('../config/systematics_tautau_' + opt.period + '.cfg')
        syst.writeSystematics()
        proc_syst = {} # key = proc name; value = {systName: [systType, systVal]] } # too nested? \_(``)_/
        for proc in backgrounds: proc_syst[proc] = {}
        for proc in signals:     proc_syst[proc] = {}

        systsShape = ['CMS_scale_t_DM0_'    + opt.period,
                      'CMS_scale_t_DM1_'    + opt.period,
                      'CMS_scale_t_DM10_'   + opt.period,
                      'CMS_scale_t_DM11_'   + opt.period,
                      'CMS_scale_t_eFake_'  + opt.period + '_DM0',
                      'CMS_scale_t_eFake_'  + opt.period + '_DM1',
                      'CMS_scale_t_muFake_' + opt.period,

                      'CMS_scale_j_FlavQCD', 
                      'CMS_scale_j_RelBal', 
                      'CMS_scale_j_HF', 
                      'CMS_scale_j_BBEC1', 
                      'CMS_scale_j_EC2', 
                      'CMS_scale_j_Abs', 
                      'CMS_scale_j_BBEC1_'     + opt.period,
                      'CMS_scale_j_EC2_'       + opt.period, 
                      'CMS_scale_j_Abs_'       + opt.period, 
                      'CMS_scale_j_HF_'        + opt.period, 
                      'CMS_scale_j_RelSample_' + opt.period,

                      'CMS_eff_j_PUJET_id_' + opt.period,
                      'CMS_bbtt_' + opt.period + '_bTagSF' + ('L' if 'boosted' in select else 'M'),
                      'CMS_bbtt_' + opt.period + 'etauFR_barrel',
                      'CMS_bbtt_' + opt.period + 'etauFR_endcap',]

        if opt.channel == 'TauTau':
            PTnames = ('40toInf',)
        else:
            PTnames = ('20to25', '25to30', '30to35', '35to40', '40toInf')
        for n in PTnames:
            systsShape.append('CMS_eff_t_id_pt' + n + '_' + opt.period)

        # Add tau trigger uncertainties (4 unc. depending on DM for tau legs + 2 unc. for ele and mu legs)
        # For TauTau channel in 2017 and 2018 add also the the VBF trigger of jet legs
        # In 2016 we only use the SingleEle trigger in ETau so no uncertainties on the tau triggers should be added
        DMs = ['DM0', 'DM1', 'DM10', 'DM11']
        if opt.channel == 'MuTau':
            DMs.append('mu')
        if opt.channel == 'ETau':
            if '16' in opt.period:
                DMs = ['ele']
            else:
                DMs.append('ele')
        if opt.channel == 'TauTau' and any(x in opt.period for x in ('17', '18')):
            DMs.append('Jet')
        for n in DMs:
            systsShape.append('CMS_bbtt_' + opt.period + '_trigSF' + n)

        systsNorm  = []  # <-- THIS WILL BE FILLED FROM CONFIGS
        for isy in range(len(syst.SystNames)) :
            if any(x in syst.SystNames[isy] for x in ('CMS_scale_t', 'CMS_scale_j')):
                continue
            for iproc in range(len(syst.SystProcesses[isy])):
                systVal = syst.SystValues[isy][iproc]
                proc_syst[syst.SystProcesses[isy][iproc]][syst.SystNames[isy]] = [syst.SystTypes[isy], systVal]
                systsNorm.append(syst.SystNames[isy])

        if len(systsNorm) > 0:
            systsNorm = list(dict.fromkeys(systsNorm))

        nominalShapes_toSave, nominalShapes_newName = ([] for _ in range(2))
        for proc in backgrounds:
            nominalShapes_toSave.append('{}_'.format(proc) + suffix_str)
            nominalShapes_newName.append(proc)

        for proc in signals:
            nominalShapes_toSave.append('{}_'.format(proc) + suffix_str)
            nominalShapes_newName.append(proc)

        nominalShapes_toSave.append('data_obs_' + suffix_str)
        nominalShapes_newName.append('data_obs')

        shiftShapes_toSave, shiftShapes_newName = ([] for _ in range(2))
        if not opt.noShapeUnc:
            for name in systsShape:
                for proc in backgrounds:
                    if 'QCD' in proc and 'CMS_scale' not in name: 
                        continue
                    proc_syst[proc][name] = ['shape', 1.] # applying jes or tes to all MC backgrounds
                    for t in ('Up', 'Down'):
                        shiftShapes_toSave.append(proc + '_' + suffix_str + '_' + name + t)
                        shiftShapes_newName.append(proc + '_' + name + t)

                for proc in signals:
                    proc_syst[proc][name] = ['shape', 1.] # applying jes or tes to all signals
                    for t in ('Up', 'Down'):
                        shiftShapes_toSave.append(proc + '_' + suffix_str + '_' + name + t)
                        shiftShapes_newName.append(proc + '_' + name + t)

        col1       = '{: <40}'
        colsysN    = '{: <30}'
        colsysType = '{: <10}'
        cols       = '{: >40}'
        ratecols   = '{0: > 30.4f}'

        shapes_lines, lnN_lines = ([] for _ in range(2))
        for name in systsShape:
            shapes_lines.append(colsysN.format(name))
            shapes_lines.append(colsysType.format('shape'))
            for lineproc in backgrounds:
                shapes_lines.append(cols.format('1' if name in proc_syst[lineproc].keys() else '-'))
            for lineproc in signals:
                shapes_lines.append(cols.format('1' if name in proc_syst[lineproc].keys() else '-'))
            shapes_lines.append('\n')

        for name in systsNorm:
            lnN_lines.append(colsysN.format(name))
            lnN_lines.append(colsysType.format('lnN'))
            for lineproc in backgrounds:
                lnN_lines.append(cols.format(proc_syst[lineproc][name][1] if name in proc_syst[lineproc].keys() else '-'))
            for lineproc in signals:
                lnN_lines.append(cols.format(proc_syst[lineproc][name][1] if name in proc_syst[lineproc].keys() else '-'))
            lnN_lines.append('\n')

        ########################
        infile = os.path.join(out_dir, hh_prefix + outstr + hh_ext('input.root'))
        outfile = hh_prefix + outstr + hh_ext('txt')
        with open(out_dir+outfile, 'wb') as file:
            file.write('imax *  number of channels\n')
            file.write('jmax *  number of processes minus 1\n')
            file.write('kmax *  number of nuisance parameters\n')
            file.write('--------------------------------------\n')
            ## shapes
            file.write('shapes * {} {} $PROCESS $PROCESS_$SYSTEMATIC\n'.format(select, infile))
            file.write('--------------------------------------\n')
            file.write((col1+cols+'\n').format('bin', select)) ### blind for now
            ## observation
            file.write((col1+cols+'\n').format('observation', '-1')) ### blind for now
            file.write('--------------------------------------\n')            ## processes
            file.write('# list the expected events for signal and all backgrounds in that bin\n')
            file.write('# the second process line must have a positive number for backgrounds, and 0 or neg for signal\n')
            file.write(col1.format('bin'))
            for i in range(0,len(backgrounds)+len(signals)):
                file.write(cols.format(select))
            file.write('\n')
            file.write(col1.format('process'))
            for proc in backgrounds:
                file.write(cols.format(proc))
            for proc in signals:
                file.write(cols.format(proc))
            file.write('\n')
            file.write(col1.format('process'))
            for i in range(1,len(backgrounds)+1):
                file.write(cols.format(i))
            for i in range(0,len(signals)):
                file.write(cols.format(str(-int(i))))
            file.write('\n')
            file.write(col1.format('rate'))
            for proc in range(len(backgrounds)+len(signals)):
                file.write(ratecols.format(rates[proc]))
            file.write('\n')
            file.write('--------------------------------------\n')
            for line in shapes_lines:
                file.write(line)
            for line in lnN_lines:
                file.write(line)
            file.write('--------------------------------------\n')
            if not opt.isResonant: ### L.P.: This might require some toughts (?)
                file.write('theory group = HH_BR_Hbb HH_BR_Htt QCDscale_ggHH pdf_ggHH mtop_ggHH QCDscale_qqHH pdf_qqHH\n')
            file.write('alpha rateParam {} QCD (@0*@1/@2) QCD_regB,QCD_regC,QCD_regD\n'.format(select))
            if (opt.binbybin):
                file.write('\n* autoMCStats 10')

        outf = ROOT.TFile.Open(infile, 'RECREATE')
        for i, name in enumerate(nominalShapes_toSave):
            h = inRoot.Get(name)
            h.SetTitle(nominalShapes_newName[i])
            h.SetName(nominalShapes_newName[i])
            outf.cd()
            h.Write()
        for i, name in enumerate(shiftShapes_toSave):
            h = inRoot.Get(name)
            h.SetTitle(shiftShapes_newName[i])
            h.SetName(shiftShapes_newName[i])
            outf.cd()
            h.Write()
        outf.Close()

    else: # if region == 0:
        outfile = hh_prefix + outstr + '_' + regions[1] + hh_ext('txt')
        with open(out_dir+outfile, 'wb') as afile:
            afile.write('imax 1\n')
            afile.write('jmax {}\n'.format(len(backgrounds)-1))
            afile.write('kmax *\n')

            afile.write('------------\n')
            afile.write('shapes * * FAKE\n'.format(opt.channel, regions[1]))
            afile.write('------------\n')

            templateName = 'data_obs_' + suffix_str
            template = inRoot.Get(templateName)
            afile.write('bin {} \n'.format(select))
            obs = template.GetEntries()
            afile.write('observation {} \n'.format(obs))

            afile.write('------------\n')

            afile.write('bin ')
            for chan in backgrounds:
                afile.write('{}\t '.format(select))
            afile.write('\n')

            afile.write('process ')
            for chan in backgrounds:
                afile.write('{}\t '.format(chan))

            afile.write('\n')

            afile.write('process ')
            for chan in range(len(backgrounds)): #+1 for the QCD
                afile.write('{}\t '.format(chan+1))
            afile.write('\n')

            afile.write('rate ')

            rates = []
            iQCD = -1
            totRate = 0

            for ichan in range(len(backgrounds)):
                if 'QCD' in backgrounds[ichan]:
                    rates.append(-1)
                    iQCD = ichan
                else:
                    templateName = '{}_'.format(backgrounds[ichan]) + suffix_str
                    template = inRoot.Get(templateName)
                    brate = template.Integral()
                    rates.append(brate)
                    totRate = totRate + brate
            if iQCD >= 0 : rates[iQCD] = ROOT.TMath.Max(0.0000001,obs-totRate)
            for ichan in range(len(backgrounds)):
                afile.write('{0:.4f}\t'.format(rates[ichan]))
            afile.write('\n')
            afile.write('------------\n')
            afile.write('QCD_{} rateParam  {} QCD 1 \n'.format(regions[1], select))

    return out_dir+outfile

ROOT.gSystem.AddIncludePath('-I$ROOFITSYS/include/')
ROOT.gSystem.AddIncludePath('-Iinclude/')
ROOT.gSystem.Load('libRooFit')
ROOT.gSystem.Load('libHiggsAnalysisCombinedLimit.so')

opt = parseOptions()
configname = opt.config
incfg = ConfigReader(configname)

data        = incfg.readListOption('general::data')
signals     = [opt.signal + x for x in opt.masses]
backgrounds = incfg.readListOption('general::backgrounds')
selections  = opt.selections

## replace what was merged
mergesec = 'merge_limits'
if incfg.hasSection(mergesec):
    for groupname in incfg.config[mergesec]:
        mergelist = incfg.readListOption(mergesec+'::'+groupname)
        theList = None
        if mergelist[0] in data:
            theList = data
        elif mergelist[0] in signals:
            theList = signals
        elif mergelist[0] in backgrounds:
            theList = backgrounds
        else:
            raise ValueError('Unknown sample {}.'.format(mergelist[0]))

        for x in mergelist:
            theList.remove(x)
        theList.append(groupname)

# rename signals following model convention
for i,sig in enumerate(signals):
    if 'GGHH_NLO' in sig:
        signals[i] = sig.replace('GGHH_NLO','ggHH').replace('_xs','_kt_1_hbbhtautau').replace('cHHH', 'kl_')
    if 'VBFHH'in sig:
        signals[i] = sig.replace('VBFHH','qqHH').replace('C3','kl').replace('_xs','_hbbhtautau') #write 1_5 as 1p5 from the beginning

regions = (('SR', ''), ('SStight', 'regB'), ('OSinviso', 'regC'), ('SSinviso', 'regD'))
if not opt.isResonant:
    for sel in selections:
        for ireg in range(len(regions)):
            card = writeCard(backgrounds, signals, sel, opt.var, regions[ireg])
else:
    for sel in selections:
        for ireg in range(len(regions)):
            for sig in signals:
                sigmass = int(sig.replace('ggFRadion', ''))
                if 'boosted' in sel and sigmass<301:
                    print('Not generating card for {} in boosted category'.format(sig))
                else:
                    card = writeCard(backgrounds, [sig], sel, opt.var, regions[ireg])
