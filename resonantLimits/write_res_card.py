# coding: utf-8

_all_ = [ ]

import sys
import os
import argparse
import ROOT
from ConfigReader import ConfigReader
from systReader import systReader

def parseOptions():
    usage = ('usage: %prog [options] datasetList\n %prog -h for help')
    parser = argparse.ArgumentParser(description=usage)
    
    parser.add_argument('-f', '--filename', default='', help='input plots')
    parser.add_argument('--indir', default='', help='input dir')
    parser.add_argument('-o', '--tag', default='TAG', help='tag')
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
    parser.add_argument('-q', '--dynamQCD' , action='store_true',
                        help='1:do QCD as rateParam / 0:read QCD from file')
    parser.add_argument('-u', '--noShapeUnc', action='store_true', help='disable shape uncertainties')
    parser.add_argument('-r', '--isResonant', action='store_true', help='is Resonant analysis')
    parser.add_argument('-b', '--binbybin', action='store_true', help='add bin by bins systematics')
    parser.add_argument('-t', '--theory', action='store_true', help='add theory systematics')
    parser.add_argument('--dy_syst', action='store_true', help='add DY systematics')
    parser.add_argument('--ws', dest='makeworkspace', action='store_true', default=False)

    return parser.parse_args()

def writeCard(backgrounds, signals, select, varfit, regions=()):
    if any(x in select for x in ('0b0j', '1b1j', '2b0j', 'boosted', 'VBFloose', 'ttCR_invMcut')):
        variable = varfit

    elif 'GGFclass'   in select : variable = 'mdnn__v2__kl1_c2v1_c31__hh_ggf',
    elif 'VBFclass'   in select : variable = 'mdnn__v2__kl1_c2v1_c31__hh_vbf',
    elif 'ttHclass'   in select : variable = 'mdnn__v2__kl1_c2v1_c31__tth',
    elif 'TTlepclass' in select : variable = 'mdnn__v2__kl1_c2v1_c31__tt_lep',
    elif 'TThadclass' in select : variable = 'mdnn__v2__kl1_c2v1_c31__tt_fh',
    elif 'DYclass'    in select : variable = 'mdnn__v2__kl1_c2v1_c31__dy',
    else: raise ValueError('Selection not supported: {}'.format(select))

    svdir = select + '_' + variable
    dname = opt.tag + '_' + opt.channel
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
            if template.Integral() > 1E-6:
                processes.append(bkg)
                if bkg != 'QCD':
                    MCbackgrounds.append(bkg)
                    
        except (ReferenceError, AttributeError):
            print('\nFile: {}'.format(opt.filename))
            print('Template: {}'.format(templateName))
            raise

    rates = []
    iQCD = -1
    totRate = 0
    nsig, nbak = len(signals), len(backgrounds)
    suffix_str = '{}_{}_{}'.format(select, regions[0], variable)
    templateName = 'data_obs_' + suffix_str
    template = inRoot.Get(templateName)
    obs = template.GetEntries()
    
    for proc in range(nbak):
        if 'QCD' in backgrounds[proc] and regions[0] != 'SR':
            rates.append(-1)
            iQCD = proc
        else:
            templateName = '{}_'.format(backgrounds[proc]) + suffix_str
            template = inRoot.Get(templateName)
            brate = template.Integral()
            rates.append(brate)
            totRate += brate
    if iQCD >= 0 and regions[0] != 'SR':
        rates[iQCD] = ROOT.TMath.Max(0.0000001,obs-totRate)
    
    # for proc in range(nsig):
    #     templateName = '{}_'.format(signals[proc]) + suffix_str
    #     template = inRoot.Get(templateName)
    #     if template.Integral() <= 0:
    #         print('[WARNING] Zero integral for signal {} in category {}.'.format(signals[proc], select))
    #         return -1

    outstr = '_{}_{}_{}_13TeV'.format(opt.period, opt.channel, select)
    hh_prefix = 'hhres' if opt.isResonant else 'hh'
    if nsig==0:
        hh_ext = lambda ext : ('.NOSIGNAL.' if opt.isResonant else '.') + ext
    else:
        hh_ext = lambda ext : ('.{}.'.format(signals[0]) if opt.isResonant else '.') + ext

    configDir = os.path.join(os.environ['HOME'], 'CMSSW_11_1_9/src/KLUBAnalysis/config/')
    if regions[0] == 'SR':
        for proc in range(nsig):
            templateName = '{}_'.format(signals[proc]) + suffix_str
            template = inRoot.Get(templateName)
            rates.append(template.Integral())

        syst = systReader(configDir + '/systematics_'+opt.period+'.cfg', signals, backgrounds, None)
        syst.writeOutput(False)

        if opt.dy_syst:
            syst.addSystFile(configDir + '/systematics_DY_'+opt.period+'.cfg')
        if opt.theory:
            syst.addSystFile(configDir + '/syst_th.cfg')

        if select == 'ttCR_invMcut':
            syst = systReader(configDir + '/systematics_ttCR.cfg', signals, backgrounds, None)
            syst.writeOutput(False)
            syst.verbose(False)
            
        if opt.channel == 'ETau':
            syst.addSystFile(configDir + '/systematics_etau.cfg')
        elif opt.channel == 'MuTau':
            syst.addSystFile(configDir + '/systematics_mutau.cfg')
        elif opt.channel == 'TauTau':
            syst.addSystFile(configDir + '/systematics_tautau.cfg')
        syst.writeSystematics()

        proc_syst = {} # key = proc name; value = {systName: [systType, systVal]] } # too nested? \_(``)_/
        for proc in backgrounds: proc_syst[proc] = {}
        for proc in signals:     proc_syst[proc] = {}

        systsAll = {'shape': None, 'lnN': []}
        if opt.noShapeUnc:
            systsAll['shape'] = []
        else:
            systsAll['shape'] = [
                # hadronic tau energy scale for decay mode 0
                'CMS_scale_t_DM0_'    + opt.period,
                # hadronic tau energy scale for decay mode 1
                'CMS_scale_t_DM1_'    + opt.period,
                # hadronic tau energy scale for decay mode 10
                'CMS_scale_t_DM10_'   + opt.period,
                # hadronic tau energy scale for decay mode 11
                'CMS_scale_t_DM11_'   + opt.period,
                
                # energy scale of electrons reconstructed as hadronic taus for decay mode 0
                'CMS_scale_t_eFake_'  + opt.period + '_DM0',
                # energy scale of electrons reconstructed as hadronic taus for decay mode 1
                'CMS_scale_t_eFake_'  + opt.period + '_DM1',
                # energy scale of muons reconstructed as hadronic taus
                'CMS_scale_t_muFake_' + opt.period,
                
                # jet energy scale
                'CMS_JES_Abs',
                'CMS_JES_Abs_'       + opt.period,
                'CMS_JES_BBEC1',
                'CMS_JES_BBEC1_'     + opt.period,
                'CMS_JES_EC2',
                'CMS_JES_EC2_'       + opt.period,
                'CMS_JES_FlavQCD', 
                'CMS_JES_HF',
                'CMS_JES_HF_'        + opt.period,             
                'CMS_JES_RelBal', 
                'CMS_JES_RelSample_' + opt.period,

                # jets faking taus
                'CMS_bbtt_' + opt.period + 'jetTauFakes_Barrel',
                'CMS_bbtt_' + opt.period + 'jetTauFakes_Endcap',

                # jet energy resolution
                'CMS_res_j_' + opt.period,
                
                # b tagging scale factors
                'CMS_btag_LF_2016_2017_2018',
                'CMS_btag_HF_2016_2017_2018',
                'CMS_btag_cferr1_2016_2017_2018',
                'CMS_btag_cferr2_2016_2017_2018',
                'CMS_btag_hfstats1_2016_2017_2018',
                'CMS_btag_hfstats2_2016_2017_2018',
                'CMS_btag_lfstats1_2016_2017_2018',
                'CMS_btag_lfstats2_2016_2017_2018',

                # pile-up jet id scale factors
                'CMS_eff_j_PUJET_id_' + opt.period,

                # deep tau scale factors versus electrons
                'CMS_bbtt_' + opt.period + '_etauFR_barrel',
                'CMS_bbtt_' + opt.period + '_etauFR_endcap',
            ]

        # deep tau scale factors versus jets
        if opt.channel == 'TauTau':
            PTnames = ('40toInf',)
        else:
            PTnames = ('20to25', '25to30', '30to35', '35to40', '40toInf')
        for n in PTnames:
            systsAll['shape'].append('CMS_eff_t_id_pt' + n + '_' + opt.period)

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
            systsAll['shape'].append('CMS_bbtt_' + opt.period + '_trigSF' + n)

        # not considered until new POG SFs are available
        # # custom tau ID scale factors
        # if opt.channel=='TauTau' and '17' in opt.year:
        #     for dm in DMs:
        #         systsAll['shape'].append('CMS_bbtt_customSF2017' + dm)

        for isy in range(len(syst.SystNames)) :
            if any(x in syst.SystNames[isy] for x in ('CMS_scale_t', 'CMS_scale_j')):
                continue
            for iproc in range(len(syst.SystProcesses[isy])):
                systVal = syst.SystValues[isy][iproc]
                proc_syst[syst.SystProcesses[isy][iproc]][syst.SystNames[isy]] = [syst.SystTypes[isy], systVal]
                systsAll['lnN'].append(syst.SystNames[isy])

        if len(systsAll['lnN']) > 0:
            systsAll['lnN'] = sorted(list(dict.fromkeys(systsAll['lnN'])))
        
        nominalShapes_toSave, nominalShapes_newName = ([] for _ in range(2))
        for proc in backgrounds:
            nominalShapes_toSave.append('{}_'.format(proc) + suffix_str)
            nominalShapes_newName.append(proc)

        for proc in signals:
            nominalShapes_toSave.append('{}_'.format(proc) + suffix_str)
            nominalShapes_newName.append(proc)

        nominalShapes_toSave.append('data_obs_' + suffix_str)
        nominalShapes_newName.append('data_obs')

        if not opt.noShapeUnc:
            shiftShapes_toSave, shiftShapes_newName = ([] for _ in range(2))
            for name in systsAll['shape']:
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

        width_def, width_type = 48, 10
        colwidths = {'col'   : '{: <' + str(width_def) + '}',
                     'sysName' : '{: <' + str(width_def-width_type) + '}',
                     'sysType' : '{: <' + str(width_type) + '}'}

        lineSysts = {'shape': [], 'lnN': []}
        lkeys = lineSysts.keys()
        if opt.noShapeUnc:
            lkeys.remove('shape')
            
        for lkey in lkeys:
            for name in systsAll[lkey]:
                lineSysts[lkey].append(colwidths['sysName'].format(name))
                lineSysts[lkey].append(colwidths['sysType'].format(lkey))

                for lineproc in backgrounds+signals:
                    if name in proc_syst[lineproc].keys():
                        astr = '1' if lkey=='shape' else proc_syst[lineproc][name][1]
                    else:
                        astr = '-'
                    lineSysts[lkey].append(colwidths['col'].format(astr))
                lineSysts[lkey].append('\n')

        ########################
        infile = os.path.join(out_dir, hh_prefix + outstr + hh_ext('input.root'))
        outfile = hh_prefix + outstr + hh_ext('txt')
        with open(out_dir+outfile, 'wb') as afile:
            wf = lambda s : afile.write(s)
            wf('imax *  number of channels\n')
            wf('jmax *  number of processes minus 1\n')
            wf('kmax *  number of nuisance parameters\n')
            wf('--------------------------------------\n')
            ## shapes
            wf('shapes * {} {} $PROCESS $PROCESS_$SYSTEMATIC\n'.format(select, infile))
            wf('--------------------------------------\n')
            wf((colwidths['col']+colwidths['col']+'\n').format('bin', select)) ### blind for now
            ## observation
            wf((colwidths['col']+colwidths['col']+'\n').format('observation', '-1')) ### blind for now
            wf('--------------------------------------\n')            ## processes
            wf('# list the expected events for signal and all backgrounds in that bin\n')
            wf('# the second process line must have a positive number for backgrounds, and 0 or neg for signal\n')
            wf(colwidths['col'].format('bin'))
            for i in range(0,nbak+nsig):
                wf(colwidths['col'].format(select))
            if nsig==0: # add one more for the empty signal process
                wf(colwidths['col'].format(select))
            wf('\n')

            wf(colwidths['col'].format('process'))
            for proc in backgrounds:
                wf(colwidths['col'].format(proc))
            if nsig==0:
                wf(colwidths['col'].format('NoSignal'))
            else:
                for proc in signals:
                    wf(colwidths['col'].format(proc))
            wf('\n')

            wf(colwidths['col'].format('process'))
            for i in range(1,nbak+1):
                wf(colwidths['col'].format(i))

            wf(colwidths['col'].format('0'))
            if nsig > 0:
                for i in range(1,nsig):
                    wf(colwidths['col'].format(str(-int(i))))
            wf('\n')

            wf(colwidths['col'].format('rate'))
            for proc in range(nbak+nsig):
                wf(colwidths['col'].format(rates[proc]))
            if nsig==0: # add one more for the empty signal process
                wf(colwidths['col'].format('0.0'))

            wf('\n')
            wf('--------------------------------------\n')
            for line in lineSysts['shape']+lineSysts['lnN']:
                wf(line)
            wf('--------------------------------------\n')
            if not opt.isResonant: ### L.P.: This might require some toughts (?)
                wf('theory group = HH_BR_Hbb HH_BR_Htt QCDscale_ggHH pdf_ggHH mtop_ggHH QCDscale_qqHH pdf_qqHH\n')

            if opt.dynamQCD:
                wf('alpha rateParam {} QCD (@0*@1/@2) QCD_regB,QCD_regC,QCD_regD\n'.format(select))

            if select == 'ttCR_invMcut':
                wf('rate_TT rateParam {0} TT 1.0 [0,2]\n'.format(select))
                
            if opt.binbybin:
                wf('\n* autoMCStats 10')

        outf = ROOT.TFile.Open(infile, 'RECREATE')
        for i, name in enumerate(nominalShapes_toSave):
            h = inRoot.Get(name)
            h.SetTitle(nominalShapes_newName[i])
            h.SetName(nominalShapes_newName[i])
            outf.cd()
            h.Write()
        if not opt.noShapeUnc:
            for i, name in enumerate(shiftShapes_toSave):
                h = inRoot.Get(name)
                h.SetTitle(shiftShapes_newName[i])
                h.SetName(shiftShapes_newName[i])
                outf.cd()
                h.Write()
        outf.Close()

    else: # if regions[0] != 'SR'
        outfile = hh_prefix + outstr + '_' + regions[1] + hh_ext('txt')
        colwidth = '{: <20}'
        with open(out_dir+outfile, 'wb') as afile:
            wf = lambda s : afile.write(s)
            wf('imax 1\n')
            wf('jmax {}\n'.format(nbak-1))
            wf('kmax *\n')

            wf('------------\n')
            wf('# convert a counting datacard into a shape one; needed to combine it with other shape datacards\n')
            wf('shapes * * FAKE\n'.format(opt.channel, regions[1]))
            wf('------------\n')

            templateName = 'data_obs_' + suffix_str
            template = inRoot.Get(templateName)
            wf('bin {} \n'.format(select))
            obs = template.GetEntries()
            wf('observation {} \n'.format(obs))

            wf('------------\n')

            wf(colwidth.format('bin'))
            for _ in backgrounds:
                wf(colwidth.format(select))
            wf('\n')

            wf(colwidth.format('process'))
            for bkg in backgrounds:
                wf(colwidth.format(bkg))
            wf('\n')

            wf(colwidth.format('process'))
            for chan in range(nbak): #+1 for the QCD
                wf(colwidth.format(chan+1))
            wf('\n')

            rates = []
            iQCD = -1
            totRate = 0
            wf(colwidth.format('rate'))
            for ichan in range(nbak):
                if 'QCD' in backgrounds[ichan]:
                    rates.append(-1)
                    iQCD = ichan
                else:
                    templateName = '{}_'.format(backgrounds[ichan]) + suffix_str
                    template = inRoot.Get(templateName)
                    brate = template.Integral()
                    rates.append(brate)
                    totRate += brate
            if iQCD >= 0:
                rates[iQCD] = ROOT.TMath.Max(0.0000001,obs-totRate)
            for ichan in range(nbak):
                wf(colwidth.format(rates[ichan]))
            wf('\n')
            wf('------------\n')
            wf('QCD_{} rateParam {} QCD 1 \n'.format(regions[1], select))

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
    if 'VBFHH' in sig:
        signals[i] = sig.replace('VBFHH','qqHH').replace('C3','kl').replace('_xs','_hbbhtautau') #write 1_5 as 1p5 from the beginning
        
if opt.dynamQCD:
    regions = (('SR', ''), ('SStight', 'regB'), ('OSinviso', 'regC'), ('SSinviso', 'regD'))
else:
    regions = (('SR', ''),)
    
if not opt.isResonant:
    for sel in selections:
        for ireg in range(len(regions)):
            writeCard(backgrounds, signals, sel, opt.var, regions[ireg])
else:
    for sel in selections:
        if 'ttCR_invMcut' in selections:
            # the signal is passed for convenience; it should be zero
            for ireg in range(len(regions)):
                writeCard(backgrounds, [], sel, opt.var, regions[ireg])
        else:
            for ireg in range(len(regions)):
                for sig in signals:
                    sigmass = int(sig.replace('ggFRadion', ''))
                    if 'boosted' in sel and sigmass<301:
                        print('Not generating card for {} in boosted category'.format(sig))
                    else:
                        writeCard(backgrounds, [sig], sel, opt.var, regions[ireg])
