#! /usr/bin/env python
import sys, pwd, commands, optparse
import os
import re
import math
import string
from scipy.special import erf
from ROOT import *
import ROOT
from ConfigReader import *
from systReader import *


def parseOptions():

    usage = ('usage: %prog [options] datasetList\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)
    
    parser.add_option('-f', '--filename' , dest='filename'  , type='string', default=''      , help='input plots')
    parser.add_option('-o', '--dir'      , dest='outDir'    , type='string', default=''      , help='outdput dir')
    parser.add_option('-c', '--channel'  , dest='channel'   , type='string', default='TauTau', help='final state')
    parser.add_option('-i', '--config'   , dest='config'    , type='string', default=''      , help='config file')
    parser.add_option('-s', '--selection', dest='overSel'   , type='string', default=''      , help='overwrite selection string')
    parser.add_option('-y', '--year'     , dest='year'      , type='string', default='2016'  , help='year')
    parser.add_option('-u', '--shape'    , dest='shapeUnc'  , type='int'   , default=1       , help='1:add 0:disable shape uncertainties')
    parser.add_option('-q', '--dynamQCD' , dest='dynamQCD'  , type='int'   , default=1       , help='1:do QCD as rateParam / 0:read QCD from file')
    parser.add_option('-r', '--resonant' , dest='isResonant', action="store_true"            , help='is Resonant analysis')
    parser.add_option('-b', '--binbybin' , dest='binbybin'  , action="store_true"            , help='add bin by bins systematics')
    parser.add_option('-t', '--theory'   , dest='theory'    , action="store_true"            , help='add theory systematics')
    parser.add_option('--ws', dest='makeworkspace', action="store_true", default=False)

    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()


def  writeCard(backgrounds,signals,select,region=-1) :

    if   "0b0j"         in select : theCat = "0"
    elif "1b1j"         in select : theCat = "1"
    elif "2b0j"         in select : theCat = "2"
    elif "boosted"      in select : theCat = "3"
    elif "ttCR_invMcut" in select : theCat = "4"

    variable = {
        "0"  : "DNNoutSM_kl_1",
        "1"  : "DNNoutSM_kl_1",
        "2"  : "DNNoutSM_kl_1",
        "3"  : "DNNoutSM_kl_1",
        "4"  : "DNNoutSM_kl_1",
    }

    theOutputDir = "{0}{1}".format(select,variable[theCat])
    dname = "_"+opt.channel+opt.outDir
    out_dir = "cards{1}/{0}/".format(theOutputDir,dname)

    cmd = "mkdir -p {0}".format(out_dir)
        
    regionName = ["","regB","regC","regD"]
    if theCat == '4': regionName = [""]
    regionSuffix = ["SR","SStight","OSinviso","SSinviso"]
    if theCat == '4': regionSuffix = ["SR"]
    status, output = commands.getstatusoutput(cmd)
    thechannel = "2"
    if opt.channel == "ETau" : thechannel="1"
    elif opt.channel == "MuTau" : thechannel = "0"
    elif opt.channel == "inclusive": thechannel = "3"

    #read config
    categories = []
    categories.append((0,select))
    MCbackgrounds=[]
    processes=[]
    inRoot = TFile.Open(opt.filename)
    for bkg in backgrounds:
        #Add protection against empty processes => If I remove this I could build all bins at once instead of looping on the selections
        templateName = "{0}_{1}_SR_{2}".format(bkg,select,variable[theCat])
        template = inRoot.Get(templateName)
        if template.Integral()>0.000001 :
            processes.append(bkg)
            if bkg is not "QCD" :
                MCbackgrounds.append(bkg)

    rates = []
    iQCD = -1
    totRate = 0
    templateName = "data_obs_{0}_{2}_{1}".format(select,variable[theCat],regionSuffix[region])
    template = inRoot.Get(templateName)
    obs = template.GetEntries()

    for proc in range(len(backgrounds)):
        if "QCD" in backgrounds[proc] and region > 0:
            rates.append(-1)
            iQCD = proc
        else :
            templateName = "{0}_{1}_{3}_{2}".format(backgrounds[proc],select,variable[theCat],regionSuffix[region])
            template = inRoot.Get(templateName)
            brate = template.Integral()
            rates.append(brate)
            totRate = totRate + brate
    if iQCD >= 0 and region > 0: rates[iQCD] = TMath.Max(0.0000001,obs-totRate)
    print str(region), rates

    if region == 0 :
        for proc in range(len(signals)):
            templateName = "{0}_{1}_{3}_{2}".format(signals[proc],select,variable[theCat],regionSuffix[region])
            template = inRoot.Get(templateName)
            srate = template.Integral()
            rates.append(srate)

        syst = systReader("../config/MY_systematics_ttCR.cfg",signals,backgrounds,None)
        syst.writeOutput(False)
        syst.verbose(True)
        if theCat != '4':
            syst.addSystFile("../config/systematics_"+opt.year+".cfg")
            syst.addSystFile("../config/systematics_DY"+opt.year+".cfg")
            if opt.theory:
                syst.addSystFile("../config/syst_th.cfg")
            if(opt.channel == "TauTau"):
                syst.addSystFile("../config/systematics_tautau.cfg")
            elif(opt.channel == "MuTau"):
                syst.addSystFile("../config/systematics_mutau.cfg")
            elif(opt.channel == "ETau"):
                syst.addSystFile("../config/systematics_etau.cfg")
        syst.writeSystematics()
        proc_syst = {} # key = proc name; value = {systName: [systType, systVal]] } # too nested? \_(``)_/
        for proc in backgrounds: proc_syst[proc] = {}
        for proc in signals:     proc_syst[proc] = {}

        systsShape =["CMS_scale_t_13TeV_"+opt.year+"_DM0","CMS_scale_t_13TeV_"+opt.year+"_DM1","CMS_scale_t_13TeV_"+opt.year+"_DM10","CMS_scale_t_13TeV_"+opt.year+"_DM11", "CMS_scale_es_13TeV_"+opt.year+"_DM0", "CMS_scale_es_13TeV_"+opt.year+"_DM1", "CMS_scale_mes_13TeV_"+opt.year+"", "CMS_scale_j_13TeV_"+opt.year+""]
        # If running without the TES/EES/JES... uncomment the following line:
        systsShape = []

        systsNorm  = []  # <-- THIS WILL BE FILLED FROM CONFIGS

        for isy in range(len(syst.SystNames)) :
            if "CMS_scale_t" in syst.SystNames[isy] or "CMS_scale_j" in syst.SystNames[isy]: continue
            for iproc in range(len(syst.SystProcesses[isy])) :
                #if "/" in syst.SystValues[isy][iproc] :
                #    f = syst.SystValues[isy][iproc].split("/")
                #    systVal = (float(f[0]),float(f[1]))
                #    if syst.SystNames[isy] == "HH_BR_Hbb":
                #        import pdb; pdb.set_trace()
                #else :
                #systVal = float(syst.SystValues[isy][iproc])
                systVal = syst.SystValues[isy][iproc]
                print "adding Syst",systVal,syst.SystNames[isy],syst.SystTypes[isy],"to",syst.SystProcesses[isy][iproc]
                proc_syst[syst.SystProcesses[isy][iproc]][syst.SystNames[isy]] = [syst.SystTypes[isy], systVal]
                systsNorm.append(syst.SystNames[isy])

        if len(systsNorm) > 0:  systsNorm = list(dict.fromkeys(systsNorm))

        nominalShapes_toSave  = []
        nominalShapes_newName = []

        shiftShapes_toSave  = []
        shiftShapes_newName = []

        for proc in backgrounds:
            nominalShapes_toSave.append("{0}_{1}_{2}_{3}".format(proc, select, regionSuffix[region], variable[theCat]))
            nominalShapes_newName.append(proc)

        for proc in signals:
            nominalShapes_toSave.append("{0}_{1}_{2}_{3}".format(proc, select, regionSuffix[region], variable[theCat]))
            nominalShapes_newName.append(proc)

        nominalShapes_toSave.append("data_obs_{0}_{1}_{2}".format(select, regionSuffix[region], variable[theCat]))
        nominalShapes_newName.append("data_obs")

        col1       = '{: <40}'
        colsysN    = '{: <30}'
        colsysType = '{: <10}'
        cols       = '{: >35}'
        ratecols   = '{0: > 35.4f}'

        shapes_lines_toWrite = []
        lnN_lines_toWrite     = []

        for name in systsShape:
            shapes_lines_toWrite.append(colsysN.format(name))
            shapes_lines_toWrite.append(colsysType.format("shape"))
            for lineproc in backgrounds: shapes_lines_toWrite.append(cols.format("1" if name in proc_syst[lineproc].keys() else "-"))
            for lineproc in signals:     shapes_lines_toWrite.append(cols.format("1" if name in proc_syst[lineproc].keys() else "-"))
            shapes_lines_toWrite.append("\n")

        for name in systsNorm:
            lnN_lines_toWrite.append(colsysN.format(name))
            lnN_lines_toWrite.append(colsysType.format("lnN"))
            for lineproc in backgrounds: lnN_lines_toWrite.append(cols.format(proc_syst[lineproc][name][1] if name in proc_syst[lineproc].keys() else "-"))
            for lineproc in signals:     lnN_lines_toWrite.append(cols.format(proc_syst[lineproc][name][1] if name in proc_syst[lineproc].keys() else "-"))
            lnN_lines_toWrite.append("\n")
        
        templateName = "data_obs_{0}_{1}_{2}".format(select,regionSuffix[region],variable[theCat])
        template = inRoot.Get(templateName)
        obs = template.Integral()

        ########################

        outFile = "hh_{0}_{1}_C{2}_13TeV.txt".format(opt.year,thechannel,theCat)

        file = open(out_dir+outFile, "wb")

        file.write    ('imax *  number of channels\n')
        file.write    ('jmax *  number of processes minus 1\n')
        file.write    ('kmax *  number of nuisance parameters\n')
        file.write    ('----------------------------------------------------------------------------------------------------------------------------------\n')
        ## shapes
        file.write    ('shapes * %s %s $PROCESS $PROCESS_$SYSTEMATIC\n'%(select, "hh_{0}_{1}_C{2}_13TeV.input.root".format(opt.year,thechannel,theCat)))
        file.write    ('----------------------------------------------------------------------------------------------------------------------------------\n')
        file.write    ((col1+cols+'\n').format('bin', select)) ### blind for now
        ## observation
        file.write    ((col1+cols+'\n').format('observation', obs)) ### blind for now
        file.write    ('----------------------------------------------------------------------------------------------------------------------------------\n')
        ## processes
        file.write    ('# list the expected events for signal and all backgrounds in that bin\n')
        file.write    ('# the second process line must have a positive number for backgrounds, and 0 or neg for signal\n')
        file.write    (col1.format('bin'))
        for i in range(0,len(backgrounds)+len(signals)):
            file.write(cols.format(select))
        file.write    ("\n")
        file.write    (col1.format('process'))
        for proc in backgrounds:
            file.write(cols.format(proc))
        for proc in signals:
            file.write(cols.format(proc))
        file.write    ("\n")
        file.write    (col1.format("process"))
        for i in range(1,len(backgrounds)+1):
            file.write(cols.format(i))
        for i in range(0,len(signals)):
            file.write(cols.format(str(-int(i))))
        file.write    ("\n")
        file.write    (col1.format("rate"))
        for proc in range(len(backgrounds)+len(signals)):
            file.write(ratecols.format(rates[proc]))
        file.write    ("\n")
        file.write    ('----------------------------------------------------------------------------------------------------------------------------------\n')
        for line in shapes_lines_toWrite:
            file.write(line)
        for line in lnN_lines_toWrite:
            file.write(line)
        file.write    ('----------------------------------------------------------------------------------------------------------------------------------\n')

        #file.write    ('theory group = HH_BR_Hbb HH_BR_Htt QCDscale_ggHH pdf_ggHH mtop_ggHH QCDscale_qqHH pdf_qqHH\n')
        if opt.dynamQCD:
            file.write("alpha rateParam {0} QCD (@0*@1/@2) QCD_regB,QCD_regC,QCD_regD\n".format(select))

        if (opt.binbybin): file.write('\n* autoMCStats 10')

        file.write('\nrate_TT rateParam {0} TT 1.0 [0,2]'.format(select))

        file.close()
        outroot = TFile.Open(out_dir+"hh_{0}_{1}_C{2}_13TeV.input.root".format(opt.year,thechannel,theCat),"RECREATE")

        for i, name in enumerate(nominalShapes_toSave):
            print name
            h = inRoot.Get(name)
            h.SetTitle(nominalShapes_newName[i])
            h.SetName(nominalShapes_newName[i])
            outroot.cd()
            h.Write()

        for i, name in enumerate(shiftShapes_toSave):
            h = inRoot.Get(name)
            h.SetTitle(shiftShapes_newName[i])
            h.SetName(shiftShapes_newName[i])
            outroot.cd()
            h.Write()

        outroot.Close()

    else :
        outFile = "hh_{0}_{1}_C{2}_13TeV_{3}.txt".format(opt.year,thechannel,theCat,regionName[region])

        file = open( out_dir+outFile, "wb")

        file.write("imax 1\n")
        file.write("jmax {0}\n".format(len(backgrounds)-1))
        file.write("kmax *\n")

        file.write("------------\n")
        file.write("shapes * * FAKE\n".format(opt.channel,regionName[region]))
        file.write("------------\n")

        templateName = "data_obs_{1}_{3}_{2}".format(bkg,select,variable[theCat],regionSuffix[region])
        template = inRoot.Get(templateName)
        file.write("bin {0} \n".format(select))
        obs = template.GetEntries()
        file.write("observation {0} \n".format(obs))

        file.write("------------\n")

        file.write("bin ")
        for chan in backgrounds:
            file.write("{0}\t ".format(select))
        file.write("\n")

        file.write("process ")
        for chan in backgrounds:
            file.write("{0}\t ".format(chan))

        file.write("\n")

        file.write("process ")
        for chan in range(len(backgrounds)): #+1 for the QCD
            file.write("{0}\t ".format(chan+1))
        file.write("\n")

        file.write("rate ")
        rates = []
        iQCD = -1
        totRate = 0

        for ichan in range(len(backgrounds)):
            if "QCD" in backgrounds[ichan]:
                rates.append(-1)
                iQCD = ichan
            else:
                templateName = "{0}_{1}_{3}_{2}".format(backgrounds[ichan],select,variable[theCat],regionSuffix[region])
                template = inRoot.Get(templateName)
                brate = template.Integral()
                rates.append(brate)
                totRate = totRate + brate
        if iQCD >= 0 : rates[iQCD] = TMath.Max(0.0000001,obs-totRate)
        for ichan in range(len(backgrounds)):
            file.write("{0:.4f}\t".format(rates[ichan]))
        file.write("\n")
        file.write("------------\n")
        file.write("QCD_{0} rateParam  {1} QCD 1 \n".format(regionName[region],select))

    return (out_dir+outFile)



ROOT.gSystem.AddIncludePath("-I$ROOFITSYS/include/")
ROOT.gSystem.AddIncludePath("-Iinclude/")
ROOT.gSystem.Load("libRooFit")
ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit.so")

parseOptions()
datacards = []
configname = opt.config
print configname
input = ConfigReader(configname)

if opt.overSel == "" :
    #allSel = ["s1b1jresolvedMcut", "s2b0jresolvedMcut", "sboostedLLMcut", "VBFloose"]
    allSel = ["s1b1jresolvedMcut", "s2b0jresolvedMcut", "sboostedLLMcut", "GGFclass", "VBFclass", "ttHclass", "TTlepclass", "TThadclass", "DYclass"]
else : allSel = [opt.overSel]

data        = input.readListOption("general::data")
signals     = input.readListOption("general::signals")
backgrounds = input.readListOption("general::backgrounds")

## replace what was merged
if input.hasSection("merge"):
    for groupname in input.config['merge']:
        mergelist = input.readListOption('merge::'+groupname)
        mergelistA = input.readOption('merge::'+groupname)
        theList = None
        if   mergelist[0] in data: theList = data
        elif mergelist[0] in signals:  theList = signals
        elif mergelist[0] in backgrounds:  theList = backgrounds
        for x in mergelist: theList.remove(x)
        theList.append(groupname)

backgrounds.append("QCD")

# rename signals following model convention
for i,sig in enumerate(signals):
    if "GGHH_NLO" in sig: signals[i] = sig.replace("GGHH_NLO","ggHH").replace("_xs","_kt_1_hbbhtautau").replace("cHHH", "kl_")
    if "VBFHH"in sig:     signals[i] = sig.replace("VBFHH","qqHH").replace("C3","kl").replace("_xs","_hbbhtautau") #write 1_5 as 1p5 from the beginning

datacards = []


for sel in allSel : 

    # Use rateParam for QCD
    if opt.dynamQCD:
        print 'QCD from rateParam'
        for ireg in range(0,4) :
            card = writeCard(backgrounds,signals,sel,ireg)
            if ireg == 0: datacards.append(card)

    # Take QCD from file
    else:
        print 'QCD from file'
        card = writeCard(backgrounds,signals,sel,0)
        datacards.append(card)
