# coding: utf-8

_all_ = [ ]

import sys
import os
import argparse
import ROOT
from ConfigReader import ConfigReader
from systReader import systReader

from prepare_histos import set_shape_syst_list

def parseOptions():
    usage = ('usage: %prog [options] datasetList\n %prog -h for help')
    parser = argparse.ArgumentParser(description=usage)
    
    parser.add_argument('-f', '--filename', default='prepared_outLimits.root',
                        help='input filename')
    parser.add_argument('--indir', default='', help='input dir')
    parser.add_argument('--outdir', default='', help='output dir')
    parser.add_argument('--tag', default='TAG', help='tag')
    parser.add_argument('-c', '--channel', default='TauTau', help='final state')
    parser.add_argument('--cfg_dir', default='', help='directory with config files')
    parser.add_argument('-y', '--period', default='UL18',
                        choices=('UL16', 'UL16APV', 'UL17', 'UL18'), help='data period')
    parser.add_argument('-v', '--var', help='variable to fit')
    parser.add_argument('-l', '--selections', required=True, nargs='+',
                        help='categories to write cards for')
    parser.add_argument('-m', '--masses', required=True, nargs='+',
                        help='resonant masses to consider')
    parser.add_argument('--spin', default="0", choices=("0", "2"), help='Spin hypothesis.')
    parser.add_argument('-q', '--dynamQCD' , action='store_true',
                        help='1:do QCD as rateParam / 0:read QCD from file')
    parser.add_argument('-u', '--noShapeUnc', action='store_true', help='disable shape uncertainties')
    parser.add_argument('-b', '--binbybin', action='store_true', help='add bin by bins systematics')
    parser.add_argument('-t', '--theory', action='store_true', help='add theory systematics')
    parser.add_argument('--dy_syst', action='store_true', help='add DY systematics')
    parser.add_argument('--ws', dest='makeworkspace', action='store_true', default=False)

    return parser.parse_args()

def create_dir(d):
    if not os.path.exists(d):
        os.makedirs(d)

def setHistoToZero(histo):
    for ibin in range(1, histo.GetNbinsX()+1):
        histo.SetBinContent(ibin, 1E-10)

def writeCard(indir, backgrounds, signals, select, varfit, regions=()):
    if any(x in select for x in ('1b', '2b', 'boosted', 'ttCR_invMcut')):
        variable = varfit
    else: raise ValueError('Selection not supported: {}'.format(select))

    svdir = select + '_' + variable
    out_dir = os.path.join(opt.outdir, 'cards_'+opt.tag, opt.channel, svdir)
    create_dir(out_dir)

    #read config
    categories, MCbackgrounds, processes = [], [], []
    categories.append((0,select))
    inRoot = ROOT.TFile.Open(os.path.join(indir, opt.filename))

    for bkg in backgrounds:
        templateName = '_'.join((bkg, select, 'SR', variable))
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

    #use as backgrounds the updated list of processes
    backgrounds=processes

    rates = []
    iQCD = -1
    totRate = 0
    nsig, nbak = len(signals), len(backgrounds)
    suffix_str = '_'.join((select, regions[0], variable))
    templateName = 'data_obs_' + suffix_str
    template = inRoot.Get(templateName)
    obs = template.GetEntries()
    
    for proc in range(nbak):
        if 'QCD' in backgrounds[proc] and regions[0] != 'SR':
            rates.append(-1)
            iQCD = proc
        else:
            templateName = backgrounds[proc] + '_' + suffix_str
            template = inRoot.Get(templateName)
            if -1E-4 < template.Integral() < 0:
                brate = 0.
            else:
                brate = template.Integral()
            rates.append(brate)
            totRate += brate
    if iQCD >= 0 and regions[0] != 'SR':
        rates[iQCD] = ROOT.TMath.Max(1E-7, obs-totRate)
    
    # for proc in range(nsig):
    #     templateName = '{}_'.format(signals[proc]) + suffix_str
    #     template = inRoot.Get(templateName)
    #     if template.Integral() <= 0:
    #         print('[WARNING] Zero integral for signal {} in category {}.'.format(signals[proc], select))
    #         return -1

    outstr = '_' + '_'.join((opt.period, opt.channel, select, '13TeV'))
    hh_prefix = 'hhres'
    if nsig==0:
        hh_ext = lambda ext : '.NOSIGNAL.' + ext
    else:
        hh_ext = lambda ext : '.{}.'.format(signals[0]) + ext

    if regions[0] == 'SR':
        for proc in range(nsig):
            templateName = '{}_'.format(signals[proc]) + suffix_str
            template = inRoot.Get(templateName)
            try:
                # adress Integral() rounding issues when the rate is zero
                if -1E-4 < template.Integral() < 0.:
                    rates.append(0.)
                else:
                    rates.append(template.Integral())
            except AttributeError:
                print('[ERROR] There was an issue with sample {}.'.format(templateName))
                raise

        syst = systReader(opt.cfg_dir + '/systematics_' + opt.period + '.cfg',
                          signals, backgrounds, None)
        syst.writeOutput(False)

        if opt.dy_syst:
            syst.addSystFile(opt.cfg_dir + '/systematics_DY_'+opt.period+'.cfg')
        if opt.theory:
            syst.addSystFile(opt.cfg_dir + '/syst_th.cfg')

        if select == 'ttCR_invMcut':
            syst = systReader(opt.cfg_dir + '/systematics_ttCR.cfg', signals, backgrounds, None)
            syst.writeOutput(False)
            syst.verbose(False)
            
        if opt.channel == 'ETau':
            syst.addSystFile(opt.cfg_dir + '/systematics_etau.cfg')
        elif opt.channel == 'MuTau':
            syst.addSystFile(opt.cfg_dir + '/systematics_mutau.cfg')
        elif opt.channel == 'TauTau':
            syst.addSystFile(opt.cfg_dir + '/systematics_tautau.cfg')
        syst.writeSystematics()

        proc_syst = {} # key = proc name; value = {systName: [systType, systVal]] } # too nested? \_(``)_/
        for proc in backgrounds: proc_syst[proc] = {}
        for proc in signals:     proc_syst[proc] = {}

        systsAll = {'shape': None, 'lnN': []}
        systsAll['shape'] = [] if opt.noShapeUnc else set_shape_syst_list(opt.period, opt.channel, select).values()

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
                    if 'QCD' in proc: # and 'CMS_scale' not in name: 
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

        first_width, second_width = 42, 8
        colwidths = {'standard': '{: <' + str(30) + '}',
                     'first'   : '{: <' + str(first_width) + '}',
                     'second'  : '{: <' + str(second_width) + '}',
                     'sumtwo'  : '{: <' + str(first_width+second_width) + '}',
                     }

        lineSysts = {'shape': [], 'lnN': []}
        lkeys = lineSysts.keys()
        if opt.noShapeUnc:
            lkeys.remove('shape')
            
        for lkey in lkeys:
            for name in systsAll[lkey]:
                lineSysts[lkey].append(colwidths['first'].format(name))
                lineSysts[lkey].append(colwidths['second'].format(lkey))

                for lineproc in backgrounds+signals:
                    if name in proc_syst[lineproc].keys():
                        astr = '1' if lkey=='shape' else proc_syst[lineproc][name][1]
                    else:
                        astr = '-'
                    lineSysts[lkey].append(colwidths['standard'].format(astr))
                lineSysts[lkey].append('\n')

        ########################
        infile = os.path.join(out_dir, hh_prefix + outstr + hh_ext('input.root'))
        outfile = hh_prefix + outstr + hh_ext('txt')
        with open(os.path.join(out_dir, outfile), 'wb') as afile:
            wf = lambda s : afile.write(s)
            wf('imax *  number of channels\n')
            wf('jmax *  number of processes minus 1\n')
            wf('kmax *  number of nuisance parameters\n')
            wf('-'*200 + '\n')
            ## shapes
            wf('shapes * {} {} $PROCESS $PROCESS_$SYSTEMATIC\n'.format(select, infile))
            wf('-'*200 + '\n')
            wf((colwidths['standard']+colwidths['standard']+'\n').format('bin', select)) ### blind for now
            ## observation
            wf((colwidths['standard']+colwidths['standard']+'\n').format('observation', '-1')) ### blind for now
            wf('-'*200 + '\n')
            wf('# list the expected events for signal and all backgrounds in that bin\n')
            wf('# the second process line must have a positive number for backgrounds, and 0 or neg for signal\n')
            wf(colwidths['sumtwo'].format('bin'))
            for i in range(0,nbak+nsig):
                wf(colwidths['standard'].format(select))
            if nsig==0: # add one more for the empty signal process
                wf(colwidths['standard'].format(select))
            wf('\n')

            wf(colwidths['sumtwo'].format('process'))
            for proc in backgrounds:
                wf(colwidths['standard'].format(proc))
            if nsig==0:
                wf(colwidths['standard'].format('NoSignal'))
            else:
                for proc in signals:
                    wf(colwidths['standard'].format(proc))
            wf('\n')

            wf(colwidths['sumtwo'].format('process'))
            for i in range(1,nbak+1):
                wf(colwidths['standard'].format(i))

            wf(colwidths['standard'].format('0'))
            if nsig > 0:
                for i in range(1,nsig):
                    wf(colwidths['standard'].format(str(-int(i))))
            wf('\n')

            wf(colwidths['sumtwo'].format('rate'))
            for proc in range(nbak+nsig):
                wf(colwidths['standard'].format(rates[proc]))
            if nsig==0: # add one more for the empty signal process
                wf(colwidths['standard'].format('0.0'))

            wf('\n')
            wf('-'*200 + '\n')
            for line in lineSysts['shape']+lineSysts['lnN']:
                wf(line)
            wf('-'*200 + '\n')

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
            if -1E-4 < h.Integral() < 0.:
                setHistoToZero(h)
            outf.cd()
            h.Write()
        
        if not opt.noShapeUnc:
            for i, name in enumerate(shiftShapes_toSave):
                h = inRoot.Get(name)
                h.SetTitle(shiftShapes_newName[i])
                h.SetName(shiftShapes_newName[i])
                if -1E-4 < h.Integral() < 0.:
                    setHistoToZero(h)
                outf.cd()
                h.Write()
        outf.Close()

    else: # if regions[0] != 'SR'
        outfile = hh_prefix + outstr + '_' + regions[1] + hh_ext('txt')
        colwidth = '{: <20}'
        with open(os.path.join(out_dir, outfile), 'wb') as afile:
            wf = lambda s : afile.write(s)
            wf('imax 1\n')
            wf('jmax {}\n'.format(nbak-1))
            wf('kmax *\n')

            wf('-'*200 + '\n')
            wf('# convert a counting datacard into a shape one; needed to combine it with other shape datacards\n')
            wf('shapes * * FAKE\n'.format(opt.channel, regions[1]))
            wf('-'*200 + '\n')

            templateName = 'data_obs_' + suffix_str
            template = inRoot.Get(templateName)
            if -1E-4 < template.Integral() < 0.:
                setHistoToZero(h)

            wf('bin {} \n'.format(select))
            obs = template.GetEntries()
            wf('observation {} \n'.format(obs))

            wf('-'*200 + '\n')

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
                    if -1E-4 < template.Integral() < 0.:
                        brate = 0.
                    else:
                        brate = template.Integral()
                    rates.append(brate)
                    totRate += brate
            if iQCD >= 0:
                rates[iQCD] = ROOT.TMath.Max(0.0000001, obs-totRate)
            for ichan in range(nbak):
                wf(colwidth.format(rates[ichan]))
            wf('\n')
            wf('-'*200 + '\n')
            wf('QCD_{} rateParam {} QCD 1 \n'.format(regions[1], select))

    return os.path.join(out_dir, outfile)

ROOT.gSystem.AddIncludePath('-I$ROOFITSYS/include/')
ROOT.gSystem.AddIncludePath('-Iinclude/')
ROOT.gSystem.Load('libRooFit')
ROOT.gSystem.Load('libHiggsAnalysisCombinedLimit.so')

opt = parseOptions()

regions = [('SR', ''),]
if opt.dynamQCD:
    regions.extend([('SStight', 'regB'),
                    ('OSinviso', 'regC'), ('SSinviso', 'regD')])

for mass in opt.masses:
    incfg = ConfigReader(os.path.join(
        opt.cfg_dir, "limits",
        '_'.join(('mainCfg', opt.channel, opt.period, 'm'+mass, 's'+opt.spin)) + '.cfg'))

    selections  = opt.selections
    data        = incfg.readListOption('general::data')
    backgrounds = incfg.readListOption('general::backgrounds')
    signal      = incfg.readListOption('general::signals')
    assert len(signal) == 1
     
    ## replace what was merged
    mergesec = 'merge_limits'
    if incfg.hasSection(mergesec):
        for groupname in incfg.config[mergesec]:
            mergelist = incfg.readListOption(mergesec+'::'+groupname)
            theList = None
            if mergelist[0] in data:
                theList = data
            elif mergelist[0] in signal:
                theList = signal
            elif mergelist[0] in backgrounds:
                theList = backgrounds
            else:
                raise ValueError('Unknown sample {}.'.format(mergelist[0]))
     
            for x in mergelist:
                theList.remove(x)
            theList.append(groupname)
     
    # read QCD from file
    backgrounds.append("QCD")

    indir = os.path.join(opt.indir, "Spin{}_Mass{}".format(opt.spin, mass))
    for sel in selections:
        for reg in regions:
            if 'ttCR_invMcut' in selections:
                # the signal is passed for convenience; it should be zero
                writeCard(indir, backgrounds, [], sel, opt.var.format(mass), reg)
            else:
                if 'boosted' in sel and int(mass)<301:
                    print('Not generating card for {} in boosted category'.format(signal))
                else:
                    writeCard(indir, backgrounds, signal, sel, opt.var.replace('{1}', '{}').format(mass), reg)
