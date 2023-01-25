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
    parser.add_option('-r', '--resonant' , dest='isResonant', action="store_true"            , help='is Resonant analysis')
    parser.add_option('-b', '--binbybin' , dest='binbybin'  , action="store_true"            , help='add bin by bins systematics')
    parser.add_option('-t', '--theory'   , dest='theory'    , action="store_true"            , help='add theory systematics')
    parser.add_option('--ws', dest='makeworkspace', action="store_true", default=False)

    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()


def  writeCard(backgrounds,signals,select,region=-1) :

    if   "0b0j"       in select : theCat = "0"
    if   "2b0j"       in select : theCat = "2"
    elif "1b1j"       in select : theCat = "1"
    elif "boosted"    in select : theCat = "3"
    elif "VBFloose"   in select : theCat = "4"

    elif "GGFclass"   in select : theCat = "5"
    elif "VBFclass"   in select : theCat = "6"
    elif "ttHclass"   in select : theCat = "7"
    elif "TTlepclass" in select : theCat = "8"
    elif "TThadclass" in select : theCat = "9"
    elif "DYclass"    in select : theCat = "10"

    variable = {
        "0"  : "HHKin_mass_raw",
        "1"  : "HHKin_mass_raw",
        "2"  : "HHKin_mass_raw",
        "3"  : "HHKin_mass_raw",
        "4"  : "HHKin_mass_raw", # "mdnn__v2__kl1_c2v1_c31__hh_vbf",
#        "0"  : "HHKin_mass_raw_HHKin_mass_raw",
#        "1"  : "HHKin_mass_raw_HHKin_mass_raw",
#        "2"  : "HHKin_mass_raw_HHKin_mass_raw",
#        "3"  : "HHKin_mass_raw_HHKin_mass_raw",
#        "4"  : "HHKin_mass_raw_HHKin_mass_raw", # "mdnn__v2__kl1_c2v1_c31__hh_vbf",

        "5"  : "mdnn__v2__kl1_c2v1_c31__hh_ggf",
        "6"  : "mdnn__v2__kl1_c2v1_c31__hh_vbf",
        "7"  : "mdnn__v2__kl1_c2v1_c31__tth",
        "8"  : "mdnn__v2__kl1_c2v1_c31__tt_lep",
        "9"  : "mdnn__v2__kl1_c2v1_c31__tt_fh",
        "10" : "mdnn__v2__kl1_c2v1_c31__dy",
    }

    theOutputDir = "{0}{1}".format(select,variable[theCat])
    dname = "_"+opt.channel+opt.outDir
    out_dir = "cards{1}/{0}/".format(theOutputDir,dname)

    cmd = "mkdir -p {0}".format(out_dir)
        
    regionName = ["","regB","regC","regD"]
    regionSuffix = ["SR","SStight","OSinviso","SSinviso"]
    status, output = commands.getstatusoutput(cmd)
    thechannel = "2"
    if opt.channel == "ETau" : thechannel="1"
    elif opt.channel == "MuTau" : thechannel = "0"

    #read config
    categories = []
    categories.append((0,select))
    MCbackgrounds=[]
    processes=[]
    inRoot = TFile.Open(opt.filename)
    print("filename = ",opt.filename)
    for bkg in backgrounds:
        #Add protection against empty processes => If I remove this I could build all bins at once instead of looping on the selections
        templateName = "{0}_{1}_SR_{2}".format(bkg,select,variable[theCat])
        print('templateName = ',templateName)
        template = inRoot.Get(templateName)
        template.Print()
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
    
    for proc in range(len(signals)):
	templateName = "{0}_{1}_{3}_{2}".format(signals[proc],select,variable[theCat],regionSuffix[0])
	template = inRoot.Get(templateName)
	if template.Integral() <= 0:
	    print "No Signal here!!!"
	    return -1
    if region == 0 :
        for proc in range(len(signals)):
            print("signals[proc] = ",signals[proc])
            templateName = "{0}_{1}_{3}_{2}".format(signals[proc],select,variable[theCat],regionSuffix[region])
            template = inRoot.Get(templateName)
            srate = template.Integral()
	    #if srate <= 0:
#		print "No signal here!"
#		return -1
            rates.append(srate)

        syst = systReader("../config/systematics_"+opt.year+".cfg",signals,backgrounds,None)
        syst.writeOutput(False)
        syst.verbose(True)
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
        #systsShape = ["CMS_scale_t_13TeV_DM0","CMS_scale_t_13TeV_DM1","CMS_scale_t_13TeV_DM10","CMS_scale_t_13TeV_DM11", "CMS_scale_es_13TeV_DM0", "CMS_scale_es_13TeV_DM1", "CMS_scale_mes_13TeV", "CMS_scale_j_13TeV"] #["CMS_scale_t_13TeV_DM0"] # <-- ADD HERE THE OTHER TES/JES SYST SHAPES (TOP SYST SHAPE IS ADDED BY HAND LATER)
        #systsShape = []
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

        if opt.shapeUnc > 0:
            for name in systsShape:
                for proc in backgrounds:
                    proc_syst[proc][name] = ["shape", 1.]   #applying jes or tes to all MC backgrounds
                    shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Up".format(proc, select,  regionSuffix[region], variable[theCat], name))
                    shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Down".format(proc, select, regionSuffix[region],variable[theCat], name))
                    shiftShapes_newName.append(proc+"_"+name+"Up")
                    shiftShapes_newName.append(proc+"_"+name+"Down")

                for proc in signals:
                    proc_syst[proc][name] = ["shape", 1.]   #applying jes or tes to all signals
                    shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Up".format(proc, select,   regionSuffix[region],variable[theCat],  name))
                    shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Down".format(proc, select, regionSuffix[region],variable[theCat],  name))
                    shiftShapes_newName.append(proc+"_"+name+"Up")
                    shiftShapes_newName.append(proc+"_"+name+"Down")

            # Add top Pt uncertainty
            #proc_syst["TT"]["top"] = ["shape", 1]
            #systsShape.append("top")
            #shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Up".format("TT", select,  regionSuffix[region], variable[theCat], "top"))
            #shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Down".format("TT", select, regionSuffix[region],variable[theCat], "top"))
            #shiftShapes_newName.append("TT_topUp")
            #shiftShapes_newName.append("TT_topDown")

            # Add PUjetID SF uncertainty
            PUjetIDname = "PUjetIDSF"
            systsShape.append(PUjetIDname)
            for proc in backgrounds:
                if "QCD" in proc: continue
                proc_syst[proc][PUjetIDname] = ["shape", 1.]   #applying trigger to all MC backgrounds
                shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Up"  .format(proc, select, regionSuffix[region], variable[theCat], PUjetIDname))
                shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Down".format(proc, select, regionSuffix[region], variable[theCat], PUjetIDname))
                shiftShapes_newName.append(proc+"_"+PUjetIDname+"Up")
                shiftShapes_newName.append(proc+"_"+PUjetIDname+"Down")

            for proc in signals:
                proc_syst[proc][PUjetIDname] = ["shape", 1.]   #applying trigger to all signals
                shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Up"  .format(proc, select, regionSuffix[region], variable[theCat], PUjetIDname))
                shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Down".format(proc, select, regionSuffix[region], variable[theCat], PUjetIDname))
                shiftShapes_newName.append(proc+"_"+PUjetIDname+"Up")
                shiftShapes_newName.append(proc+"_"+PUjetIDname+"Down")

            # Add bTag SF uncertainty
            if "boosted" in select:
                WPs = ["L"]
            else:
                WPs = ["M"]
            for WPname in WPs:
                bTagWPname = "bTagSF" + WPname
                systsShape.append(bTagWPname)
                for proc in backgrounds:
                    if "QCD" in proc: continue
                    proc_syst[proc][bTagWPname] = ["shape", 1.]   #applying trigger to all MC backgrounds
                    shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Up"  .format(proc, select, regionSuffix[region], variable[theCat], bTagWPname))
                    shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Down".format(proc, select, regionSuffix[region], variable[theCat], bTagWPname))
                    shiftShapes_newName.append(proc+"_"+bTagWPname+"Up")
                    shiftShapes_newName.append(proc+"_"+bTagWPname+"Down")

                for proc in signals:
                    proc_syst[proc][bTagWPname] = ["shape", 1.]   #applying trigger to all signals
                    shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Up"  .format(proc, select, regionSuffix[region], variable[theCat], bTagWPname))
                    shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Down".format(proc, select, regionSuffix[region], variable[theCat], bTagWPname))
                    shiftShapes_newName.append(proc+"_"+bTagWPname+"Up")
                    shiftShapes_newName.append(proc+"_"+bTagWPname+"Down")

            # Add tau trigger uncertainties (4 different uncertainties depending on DM)
            #DMs = ["0","1","10","11"]
            #for DMname in DMs:
            #    trigDMname = "trigSFDM" + DMname
            #    systsShape.append(trigDMname)
            #    for proc in backgrounds:
            #        if "QCD" in proc: continue
            #        proc_syst[proc][trigDMname] = ["shape", 1.]   #applying trigger to all MC backgrounds
            #        shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Up"  .format(proc, select, regionSuffix[region], variable[theCat], trigDMname))
            #        shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Down".format(proc, select, regionSuffix[region], variable[theCat], trigDMname))
            #        shiftShapes_newName.append(proc+"_"+trigDMname+"Up")
            #        shiftShapes_newName.append(proc+"_"+trigDMname+"Down")

            #    for proc in signals:
            #        proc_syst[proc][trigDMname] = ["shape", 1.]   #applying trigger to all signals
            #        shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Up"  .format(proc, select, regionSuffix[region], variable[theCat], trigDMname))
            #        shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Down".format(proc, select, regionSuffix[region], variable[theCat], trigDMname))
            #        shiftShapes_newName.append(proc+"_"+trigDMname+"Up")
            #        shiftShapes_newName.append(proc+"_"+trigDMname+"Down")

            # Add deepTauVSjet uncertainties (5 different uncertainties binned in pT)
            if "2" in thechannel:
                PTs = ["40toInf"]
            else:
                PTs = ["20to25", "25to30", "30to35", "35to40", "40toInf"]
            for PTname in PTs:
                tauPTname = "tauid_pt" + PTname
                systsShape.append(tauPTname)
                for proc in backgrounds:
                    if "QCD" in proc: continue
                    proc_syst[proc][tauPTname] = ["shape", 1.]   #applying trigger to all MC backgrounds
                    shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Up"  .format(proc, select, regionSuffix[region], variable[theCat], tauPTname))
                    shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Down".format(proc, select, regionSuffix[region], variable[theCat], tauPTname))
                    shiftShapes_newName.append(proc+"_"+tauPTname+"Up")
                    shiftShapes_newName.append(proc+"_"+tauPTname+"Down")

                for proc in signals:
                    proc_syst[proc][tauPTname] = ["shape", 1.]   #applying trigger to all signals
                    shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Up"  .format(proc, select, regionSuffix[region], variable[theCat], tauPTname))
                    shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Down".format(proc, select, regionSuffix[region], variable[theCat], tauPTname))
                    shiftShapes_newName.append(proc+"_"+tauPTname+"Up")
                    shiftShapes_newName.append(proc+"_"+tauPTname+"Down")

            # Add deepTauVSmu uncertainties (5 different uncertainties binned in eta)
            #ETAs = ["Lt0p4", "0p4to0p8", "0p8to1p2", "1p2to1p7", "Gt1p7"]
            #for ETAname in ETAs:
            #    tauETAname = "mutauFR_eta" + ETAname
            #    systsShape.append(tauETAname)
            #    for proc in backgrounds:
            #        if "QCD" in proc: continue
            #        proc_syst[proc][tauETAname] = ["shape", 1.]   #applying trigger to all MC backgrounds
            #        shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Up"  .format(proc, select, regionSuffix[region], variable[theCat], tauETAname))
            #        shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Down".format(proc, select, regionSuffix[region], variable[theCat], tauETAname))
            #        shiftShapes_newName.append(proc+"_"+tauETAname+"Up")
            #        shiftShapes_newName.append(proc+"_"+tauETAname+"Down")

            #    for proc in signals:
            #        proc_syst[proc][tauETAname] = ["shape", 1.]   #applying trigger to all signals
            #        shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Up"  .format(proc, select, regionSuffix[region], variable[theCat], tauETAname))
            #        shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Down".format(proc, select, regionSuffix[region], variable[theCat], tauETAname))
            #        shiftShapes_newName.append(proc+"_"+tauETAname+"Up")
            #        shiftShapes_newName.append(proc+"_"+tauETAname+"Down")

            # Add deepTauVSele uncertainties (2 different uncertainties for barrel and endcap)
            ELEs = ["barrel", "endcap"]
            for ELEname in ELEs:
                tauELEname = "etauFR_" + ELEname
                systsShape.append(tauELEname)
                for proc in backgrounds:
                    if "QCD" in proc: continue
                    proc_syst[proc][tauELEname] = ["shape", 1.]   #applying trigger to all MC backgrounds
                    shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Up"  .format(proc, select, regionSuffix[region], variable[theCat], tauELEname))
                    shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Down".format(proc, select, regionSuffix[region], variable[theCat], tauELEname))
                    shiftShapes_newName.append(proc+"_"+tauELEname+"Up")
                    shiftShapes_newName.append(proc+"_"+tauELEname+"Down")

                for proc in signals:
                    proc_syst[proc][tauELEname] = ["shape", 1.]   #applying trigger to all signals
                    shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Up"  .format(proc, select, regionSuffix[region], variable[theCat], tauELEname))
                    shiftShapes_toSave.append("{0}_{1}_{2}_{3}_{4}Down".format(proc, select, regionSuffix[region], variable[theCat], tauELEname))
                    shiftShapes_newName.append(proc+"_"+tauELEname+"Up")
                    shiftShapes_newName.append(proc+"_"+tauELEname+"Down")

        col1       = '{: <40}'
        colsysN    = '{: <30}'
        colsysType = '{: <10}'
        cols       = '{: >40}'
        ratecols   = '{0: > 30.4f}'

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

        ########################
        if not opt.isResonant:
            outFile = "hh_{0}_{1}_C{2}_13TeV.txt".format(opt.year,thechannel,theCat)
        else:
            outFile = "hhres_{0}_{1}_C{2}_13TeV.{3}.txt".format(opt.year,thechannel,theCat,signals[0])

            
        file = open(out_dir+outFile, "wb")

        file.write    ('imax *  number of channels\n')
        file.write    ('jmax *  number of processes minus 1\n')
        file.write    ('kmax *  number of nuisance parameters\n')
        file.write    ('----------------------------------------------------------------------------------------------------------------------------------\n')
        ## shapes
        if not opt.isResonant:
            file.write    ('shapes * %s %s $PROCESS $PROCESS_$SYSTEMATIC\n'%(select, "hh_{0}_{1}_C{2}_13TeV.input.root".format(opt.year,thechannel,theCat)))
        else:
            file.write    ('shapes * %s %s $PROCESS $PROCESS_$SYSTEMATIC\n'%(select, "hhres_{0}_{1}_C{2}_13TeV.{3}.input.root".format(opt.year,thechannel,theCat,signals[0])))

        file.write    ('----------------------------------------------------------------------------------------------------------------------------------\n')
        file.write    ((col1+cols+'\n').format('bin', select)) ### blind for now
        ## observation
        file.write    ((col1+cols+'\n').format('observation', '-1')) ### blind for now
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
        
        if not opt.isResonant: ### L.P.: This might require some toughts (?)
            file.write    ('theory group = HH_BR_Hbb HH_BR_Htt QCDscale_ggHH pdf_ggHH mtop_ggHH QCDscale_qqHH pdf_qqHH\n')
        file.write    ("alpha rateParam {0} QCD (@0*@1/@2) QCD_regB,QCD_regC,QCD_regD\n".format(select))

        if (opt.binbybin): file.write('\n* autoMCStats 10')

        file.close()
        if not opt.isResonant:
            outroot = TFile.Open(out_dir+"hh_{0}_{1}_C{2}_13TeV.input.root".format(opt.year,thechannel,theCat),"RECREATE")
        else:
            outroot = TFile.Open(out_dir+"hhres_{0}_{1}_C{2}_13TeV.{3}.input.root".format(opt.year,thechannel,theCat,signals[0]),"RECREATE")

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
        if not opt.isResonant:
            outFile = "hh_{0}_{1}_C{2}_13TeV_{3}.txt".format(opt.year,thechannel,theCat,regionName[region])
        else:
            outFile = "hhres_{0}_{1}_C{2}_13TeV_{3}.{4}.txt".format(opt.year,thechannel,theCat,regionName[region],signals[0])

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
    if opt.channel == "TauTau":
	allSel = [
"s1b1jresolvedMcutMD",
"s1b1jresolvedMcutInvMD",
"s2b0jresolvedMcutMD",
"s2b0jresolvedMcutInvMD",
"sboostedLLMcutMD",
"sboostedLLMcutInvMD",
"VBFlooseMD",
"VBFlooseInvMD"]
	#allSel = ["s1b1jresolvedMcutmHH250_350","s1b1jresolvedMcutmHH350_500","s1b1jresolvedMcutmHH500_700","s1b1jresolvedMcutmHH700_1000","s1b1jresolvedMcutmHH1000_3500","s2b0jresolvedMcutmHH250_350","s2b0jresolvedMcutmHH350_500","s2b0jresolvedMcutmHH500_700","s2b0jresolvedMcutmHH700_1000","s2b0jresolvedMcutmHH1000_3500","sboostedLLMcutmHH250_350","sboostedLLMcutmHH350_500","sboostedLLMcutmHH500_700","sboostedLLMcutmHH700_1000","sboostedLLMcutmHH1000_3500","VBFloosemHH250_350","VBFloosemHH350_500","VBFloosemHH500_700","VBFloosemHH700_1000","VBFloosemHH1000_3500"]
        #allSel = ["s1b1jresolvedMcutmHH250_335","s1b1jresolvedMcutmHH335_475","s1b1jresolvedMcutmHH475_725","s1b1jresolvedMcutmHH725_1100","s1b1jresolvedMcutmHH1100_3500","s2b0jresolvedMcutmHH250_335","s2b0jresolvedMcutmHH335_475","s2b0jresolvedMcutmHH475_725","s2b0jresolvedMcutmHH725_1100","s2b0jresolvedMcutmHH1100_3500","sboostedLLMcutmHH250_625","sboostedLLMcutmHH625_775","sboostedLLMcutmHH775_1100","sboostedLLMcutmHH1100_3500","VBFloosemHH250_335","VBFloosemHH335_475","VBFloosemHH475_725","VBFloosemHH725_1100","VBFloosemHH1100_3500"]
	#allSel = ["s1b1jresolvedMcutmHH250_330","s1b1jresolvedMcutmHH330_510","s1b1jresolvedMcutmHH510_660","s1b1jresolvedMcutmHH660_970","s1b1jresolvedMcutmHH970_3500","s2b0jresolvedMcutmHH250_330","s2b0jresolvedMcutmHH330_420","s2b0jresolvedMcutmHH420_540","s2b0jresolvedMcutmHH540_860","s2b0jresolvedMcutmHH860_3500","sboostedLLMcutmHH250_720","sboostedLLMcutmHH720_900","sboostedLLMcutmHH900_3500","VBFloosemHH250_400","VBFloosemHH400_540","VBFloosemHH540_760","VBFloosemHH760_1000","VBFloosemHH1000_3500"]
    else:
	allSel = [
"s1b1jresolvedMcutDRMD",
"s1b1jresolvedMcutDRInvMD",
"s1b1jresolvedMcutInvDRMD",
"s1b1jresolvedMcutInvDRInvMD",
"s2b0jresolvedMcutDRMD",
"s2b0jresolvedMcutDRInvMD",
"s2b0jresolvedMcutInvDRMD",
"s2b0jresolvedMcutInvDRInvMD",
"sboostedLLMcutDRMD",
"sboostedLLMcutDRInvMD",
"sboostedLLMcutInvDRMD",
"sboostedLLMcutInvDRInvMD",
"VBFlooseDRMD",
"VBFlooseDRInvMD",
"VBFlooseInvDRMD",
"VBFlooseInvDRInvMD"]
    #elif opt.channel == 'ETau':
	#allSel = ["s1b1jresolvedMcutmHH250_370","s1b1jresolvedMcutmHH370_540","s1b1jresolvedMcutmHH540_750","s1b1jresolvedMcutmHH750_1050","s1b1jresolvedMcutmHH1050_3500","s1b1jresolvedMcutInvDR","s2b0jresolvedMcutmHH250_330","s2b0jresolvedMcutmHH330_510","s2b0jresolvedMcutmHH510_750","s2b0jresolvedMcutmHH750_990","s2b0jresolvedMcutmHH990_3500","s2b0jresolvedMcutInvDR","sboostedLLMcutmHH250_370","sboostedLLMcutmHH370_610","sboostedLLMcutmHH610_820","sboostedLLMcutmHH820_1000","sboostedLLMcutmHH1000_3500","sboostedLLMcutInvDR","VBFloosemHH250_370","VBFloosemHH370_500","VBFloosemHH500_770","VBFloosemHH770_970","VBFloosemHH970_3500","VBFlooseInvDR"]
    #elif opt.channel == 'MuTau':
	#allSel = ["s1b1jresolvedMcutmHH250_330","s1b1jresolvedMcutmHH330_520","s1b1jresolvedMcutmHH520_710","s1b1jresolvedMcutmHH710_1000","s1b1jresolvedMcutmHH1000_3500","s1b1jresolvedMcutInvDR","s2b0jresolvedMcutmHH250_330","s2b0jresolvedMcutmHH330_470","s2b0jresolvedMcutmHH470_680","s2b0jresolvedMcutmHH680_1100","s2b0jresolvedMcutmHH1100_3500","s2b0jresolvedMcutInvDR","sboostedLLMcutmHH250_400","sboostedLLMcutmHH400_570","sboostedLLMcutmHH570_810","sboostedLLMcutmHH810_1050","sboostedLLMcutmHH1050_3500","sboostedLLMcutInvDR","VBFloosemHH250_360","VBFloosemHH360_530","VBFloosemHH530_790","VBFloosemHH790_1050","VBFloosemHH1050_3500","VBFlooseInvDR"]
        #allSel = ["s1b1jresolvedMcutmHH250_350","s1b1jresolvedMcutmHH350_500","s1b1jresolvedMcutmHH500_700","s1b1jresolvedMcutmHH700_1000","s1b1jresolvedMcutmHH1000_3500","s1b1jresolvedMcutInvDR","s2b0jresolvedMcutmHH250_350","s2b0jresolvedMcutmHH350_500","s2b0jresolvedMcutmHH500_700","s2b0jresolvedMcutmHH700_1000","s2b0jresolvedMcutmHH1000_3500","s2b0jresolvedMcutInvDR","sboostedLLMcutmHH250_350","sboostedLLMcutmHH350_500","sboostedLLMcutmHH500_700","sboostedLLMcutmHH700_1000","sboostedLLMcutmHH1000_3500","sboostedLLMcutInvDR","VBFloosemHH250_350","VBFloosemHH350_500","VBFloosemHH500_700","VBFloosemHH700_1000","VBFloosemHH1000_3500","VBFlooseInvDR"]
        #allSel = ["s1b1jresolvedMcutmHH250_335","s1b1jresolvedMcutmHH335_475","s1b1jresolvedMcutmHH475_725","s1b1jresolvedMcutmHH725_1100","s1b1jresolvedMcutmHH1100_3500","s1b1jresolvedMcutInvDR","s2b0jresolvedMcutmHH250_335","s2b0jresolvedMcutmHH335_475","s2b0jresolvedMcutmHH475_725","s2b0jresolvedMcutmHH725_1100","s2b0jresolvedMcutmHH1100_3500","s2b0jresolvedMcutInvDR","sboostedLLMcutmHH250_625","sboostedLLMcutmHH625_775","sboostedLLMcutmHH775_1100","sboostedLLMcutmHH1100_3500","sboostedLLMcutInvDR","VBFloosemHH250_335","VBFloosemHH335_475","VBFloosemHH475_725","VBFloosemHH725_1100","VBFloosemHH1100_3500","VBFlooseInvDR"]
else : allSel = [opt.overSel]
#allSel = ["s1b1jresolvedMcutDR","s2b0jresolvedMcutDR","sboostedLLMcutDR","VBFlooseDR"]
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
print("Backgrounds: ", backgrounds)
# rename signals following model convention
for i,sig in enumerate(signals):
    if "GGHH_NLO" in sig: signals[i] = sig.replace("GGHH_NLO","ggHH").replace("_xs","_kt_1_hbbhtautau").replace("cHHH", "kl_")
    if "VBFHH"in sig:     signals[i] = sig.replace("VBFHH","qqHH").replace("C3","kl").replace("_xs","_hbbhtautau") #write 1_5 as 1p5 from the beginning

datacards = []

if not opt.isResonant:
    for sel in allSel : 
        for ireg in range(0,4) :
            card = writeCard(backgrounds,signals,sel,ireg)
            if ireg == 0: datacards.append(card)
else:
    for sel in allSel :
        for ireg in range(0,4) :
            for sig in signals :
                sigmass = int(sig.replace("Radion",""))
                if "boosted" in sel and sigmass<301:
                    print("Not generating card for ", sig, " in boosted category")
                else:
                    card = writeCard(backgrounds, [sig], sel, ireg)
                    if ireg ==0: datacards.append(card)
