#!/usr/bin/env python

import os,sys
import optparse
import fileinput
import time
import glob
import subprocess
from os.path import basename

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

def parseInputFileList (fileName) :
    filelist = []
    with open (fileName) as fIn:
        for line in fIn:
            line = (line.split("#")[0]).strip()
            if line:
                filelist.append(line)
    return filelist

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


if __name__ == "__main__":

    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage)
    parser.add_option ('-i', '--input'     , dest='input'     , help='input folder'                          , default='none')
    parser.add_option ('-x', '--xs'        , dest='xs'        , help='sample xs'                             , default='1.')
    parser.add_option ('-f', '--force'     , dest='force'     , help='replace existing reduced ntuples'      , default=False)
    parser.add_option ('-o', '--output'    , dest='output'    , help='output folder'                         , default='none')
    parser.add_option ('-q', '--queue'     , dest='queue'     , help='batch queue'                           , default='short')
    parser.add_option ('-r', '--resub'     , dest='resub'     , help='resubmit failed jobs'                  , default='none')
    parser.add_option ('-v', '--verb'      , dest='verb'      , help='verbose'                               , default=False)
    parser.add_option ('-s', '--sleep'     , dest='sleep'     , help='sleep in submission'                   , default=False)
    parser.add_option ('-d', '--isdata'    , dest='isdata'    , help='data flag'                             , default=False)
    parser.add_option ('-T', '--tag'       , dest='tag'       , help='folder tag name'                       , default='')
    parser.add_option ('-H', '--hadd'      , dest='hadd'      , help='hadd the resulting ntuples'            , default='none')
    parser.add_option ('-c', '--config'    , dest='config'    , help='skim config file'                      , default='none')
    parser.add_option ('-n', '--njobs'     , dest='njobs'     , help='number of skim jobs'                   , default=100, type = int)
    parser.add_option ('-k', '--kinfit'    , dest='dokinfit'  , help='run HH kin fitter'                     , default="True")
    parser.add_option ('-m', '--mt2'       , dest='domt2'     , help='run stransverse mass calculation'      , default=True)
    parser.add_option ('-y', '--xsscale'   , dest='xsscale'   , help='scale to apply on XS for stitching'    , default='1.0')
    parser.add_option ('-Z', '--htcutlow'  , dest='htcutlow'  , help='HT low cut for stitching on inclusive' , default='-999.0')
    parser.add_option ('-z', '--htcut'     , dest='htcut'     , help='HT cut for stitching on inclusive'     , default='-999.0')
    parser.add_option ('-e', '--njets'     , dest='njets'     , help='njets required for stitching on inclusive'     , default='-999')
    parser.add_option ('-t', '--toprew'    , dest='toprew'    , help='is TT bar sample to compute reweight?' , default=False)
    parser.add_option ('-b', '--topstitch' , dest='topstitch' , help='type of TT gen level decay pruning for stitch'        , default='0')
    parser.add_option ('-g', '--genjets'   , dest='genjets'   , help='loop on genjets to determine the number of b hadrons' , default=False)
    parser.add_option ('-a', '--ishhsignal', dest='ishhsignal', help='isHHsignal'                            , default=False)
    parser.add_option ('--BSMname',          dest='BSMname'   , help='additional name for EFT benchmarks'    , default='none')
    parser.add_option ('--EFTbm',            dest='EFTrew'    , help='EFT benchmarks [SM, 1..12, 1b..7b, 8a, c2scan, manual]', default='none')
    parser.add_option ('--order',            dest='order'     , help='order of reweight: lo/nlo'             , default='nlo')
    parser.add_option ('--uncert',           dest='uncert'    , help='uncertainty on the reweight coeffs'    , default='0')
    parser.add_option ('--cms_fake',         dest='cms_fake'  , help='invert some couplings for 2017/2018'   , default='0')
    parser.add_option ('--kl',               dest='klreweight', help='kl for dynamic reweight'              , default='-999.0')
    parser.add_option ('--kt',               dest='ktreweight', help='kt for dynamic reweight'              , default='-999.0')
    parser.add_option ('--c2',               dest='c2reweight', help='c2 for dynamic reweight'              , default='-999.0')
    parser.add_option ('--cg',               dest='cgreweight', help='cg for dynamic reweight'              , default='-999.0')
    parser.add_option ('--c2g',              dest='c2greweight', help='c2g for dynamic reweight'            , default='-999.0')
    parser.add_option ('--susy',             dest='susyModel' , help='name of susy model to select'         , default='NOTSUSY')
    parser.add_option ('--pu',               dest='PUweights' , help='external PUweights file'              , default='none')
    parser.add_option ('--nj',               dest='DY_nJets'  , help='number of gen Jets for DY bins'       , default='-1')
    parser.add_option ('--nb',               dest='DY_nBJets' , help='number of gen BJets for DY bins'      , default='-1')
    parser.add_option ('--DY',               dest='DY'        , help='if it is a DY sample'                 , default=False)
    parser.add_option ('--ttHToNonBB',       dest='ttHToNonBB', help='if it is a ttHToNonBB sample'         , default=False)
    parser.add_option ('--hhNLO',            dest='hhNLO'     , help='if it is an HH NLO sample'            , default=False,  action = 'store_true')
    parser.add_option ('--period',           dest='period'    , help='2018 or 2017 or 2016preVFP or 2016postVFP'             , default='2018')
    parser.add_option ('--doSyst',           dest='doSyst'    , help='compute up/down values of outputs'    , default=False,  action = 'store_true')
    parser.add_option ('--datasetType',      dest='datasetType',help='Type of dataset being considered, used for avoiding duplicated events. 0: default, 1: MET dataset 2: SingleTau dataset.', default='0', type=int)
    parser.add_option ('--isDYlike',                            help="Boosted corrections : if the process is of the kind V+jets", default=False, action='store_true') 
    parser.add_option ('--isTTlike',                            help="Boosted corrections : if the process is top-enriched", default=False, action='store_true') 

    (opt, args) = parser.parse_args()

    currFolder = os.getcwd ()


        
    # submit the jobs
    # ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

    skimmer = 'skimNtuple_HHbtag.exe'

    if opt.config == 'none' :
        print('config file missing, exiting')
        sys.exit (1)

    if opt.input[-1] == '/' : opt.input = opt.input[:-1]
    if opt.output == 'none' : opt.output = opt.input + '_SKIM'
   
    if not os.path.exists (opt.input) :
        print('input folder', opt.input, 'not existing, exiting')
        sys.exit (1)

    if not opt.force and os.path.exists (opt.output) :
        print('output folder', opt.output, 'existing, exiting')
        sys.exit (1)
    elif os.path.exists (opt.output) :
        os.system ('rm -rf ' + opt.output + '/*')
    os.system ('mkdir ' + opt.output)
    os.system ('cp ' + opt.config + " " + opt.output)
    
    #inputfiles = glob.glob (opt.input + '/*.root')    
    inputfiles = parseInputFileList (opt.input)
    if opt.njobs > len (inputfiles) : opt.njobs = len (inputfiles)
    nfiles = (len (inputfiles) + len (inputfiles) % opt.njobs) // opt.njobs
    inputlists = [inputfiles[x:x+nfiles] for x in range (0, len (inputfiles), nfiles)]

    tagname = "/" + opt.tag if opt.tag else ''
    jobsDir = currFolder + tagname + '/SKIM_' + basename (opt.input)
    jobsDir = jobsDir.rstrip (".txt")
    if float(opt.klreweight) > -990 and opt.BSMname == 'none':
        print('!WARNING! You requested manual HH reweighting, but did not set a proper BSMname! Exiting!')
        sys.exit (0)
    elif opt.EFTrew != 'none':
        jobsDir = jobsDir + '_' + opt.EFTrew
    elif opt.BSMname != 'none':
        jobsDir = jobsDir + '_' + opt.BSMname

    if os.path.exists (jobsDir) : os.system ('rm -f ' + jobsDir + '/*')
    else                        : os.system ('mkdir ' + jobsDir)

    n = int (0)
    commandFile = open (jobsDir + '/submit.sh', 'w')
    for listname in inputlists : 
        #create a wrapper for standalone cmssw job
        listFileName = "filelist_%i.txt" % n
        thisinputlistFile = open(jobsDir + "/" + listFileName, 'w')
        for line in listname:
            thisinputlistFile.write(line+"\n")
        thisinputlistFile.close()
        scriptFile = open ('%s/skimJob_%d.sh'% (jobsDir,n), 'w')
        scriptFile.write ('#!/bin/bash\n')
        scriptFile.write ('export KRB5CCNAME=/gwpool/users/spalluotto/krb5cc_`id -u spalluotto`\n')
        scriptFile.write ('/usr/bin/eosfusebind -g; cd /eos/cms/;\n')
        scriptFile.write ('cd %s\n'%currFolder)
        scriptFile.write (f'/cvmfs/cms.cern.ch/common/cmssw-cc7 -B /gwdata:/gwdata -B /gwteras:/gwteras \
    --command-to-run {jobsDir}/insingularity_{n}.sh\n')
        # scriptFile.write ('echo $HOSTNAME\n')
        # scriptFile.write ('source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
        # scriptFile.write ('eval `scram r -sh`\n')
        # scriptFile.write ('cd %s\n'%currFolder)
        # scriptFile.write ('eval `scram r -sh`\n')
        # scriptFile.write ('source scripts/setup.sh\n')

        inSing = open('%s/insingularity_%d.sh'% (jobsDir,n), 'w')
        inSing.write('#!/bin/bash\n')
        inSing.write('echo $HOSTNAME\n')
        inSing.write('source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
        inSing.write('eval `scram r -sh`\n')
        inSing.write('cd %s\n'%currFolder)
        inSing.write('cd ..\n')
        inSing.write('cmsenv\n')
        inSing.write('cd %s\n'%currFolder)
        inSing.write('source scripts/setup.sh\n')
        
        
        # arguments for the skimNtuple.cpp   ---> argv[] :
        command = skimmer + ' ' + jobsDir+"/"+listFileName + ' ' + opt.output + '/' + "output_"+str(n)+".root" + ' ' + opt.xs  # [1] : files list     [2] : output file      [3] : XS  
        if opt.isdata :  command += ' 1 '    # [4] : isData
        else          :  command += ' 0 '    
        command += ' ' + opt.config + ' '    # [5] : config file
        if opt.dokinfit=="True" : command += " 1 "   # [6] : doKinFit
        else                    : command += " 0 "
        command += " " + opt.xsscale    # [7] : XS scale
        command += " " + opt.htcut      # [8] : HT cut max
        command += " " + opt.htcutlow   # [9] : HT cut min
        if opt.toprew=="True" : command += " 1 "  # [10] : top reweight
        else                  : command += " 0 " 
        if opt.genjets=="True": command += " 1 "  # [11] : DY_tostitch
        else                  : command += " 0 " 
        command += " " + opt.topstitch            # [12] : TT_stitchType
        if opt.domt2          : command += " 1 "  # [13] : deprecated
        else                  : command += " 0 " 
        if opt.ishhsignal     : command += " 1 "  # [14] : isHHsignal
        else                  : command += " 0 "
        command += (" " + opt.njets)              # [15] : njets
        command += (" " + opt.EFTrew + " " + opt.order + " " + opt.uncert + " " + opt.cms_fake + " " + opt.klreweight + " " + opt.ktreweight + " " + opt.c2reweight + " " + opt.cgreweight + " " + opt.c2greweight)  # argv[16] to argv[24]
        command += (" " + opt.susyModel)          # [25] : susy model
        command += (" " + opt.PUweights)          # [26] : PU weights
        command += (" " + opt.DY_nJets)           # [27] : DY n jets
        command += (" " + opt.DY_nBJets)          # [28] : DY b jets
        if opt.DY             : command += " 1 "  # [29] : isDY  - deprecated
        else                  : command += " 0 "
        if opt.ttHToNonBB     : command += " 1 "  # [30] : ttH to non bb 
        else                  : command += " 0 "
        if opt.hhNLO          : command += " 1 "  # [31] : hhNLO
        else                  : command += " 0 " 
        command += (" " + str(opt.period))        # [32] : period
        command += (" " + str(opt.datasetType))   # [33] : dataset type
        if opt.isDYlike : command += " 1 "        # [34] : is DY like for boosted corrections
        else:             command += " 0 "
        if opt.isTTlike : command += " 1 "        # [35] : is DY like for boosted corrections   
        else:             command += " 0 "
        command += ' >& ' + opt.output + '/' + "output_" + str(n) + '.log\n'
        inSing.write (command)
        inSing.write ('touch ' + jobsDir + '/done_%d\n'%n)

        if opt.doSyst:
            sys_command = "skimOutputter.exe"
            sys_command += (" " + opt.output + "/output_"+str(n)+".root")
            sys_command += (" " + opt.output + "/syst_output_"+str(n)+".root")
            sys_command += (" " + opt.config)
            if opt.isdata : sys_command += ' 1 '
            else          : sys_command += ' 0 '
            sys_command += (" " + ">& " + opt.output + "/syst_output_"+str(n)+".log\n")
            scriptFile.write(sys_command)

        inSing.write ('echo "All done for job %d" \n'%n)
        inSing.close ()
        scriptFile.close ()
        os.system ('chmod u+rwx %s/skimJob_%d.sh'% (jobsDir,n))
        os.system ('chmod u+rwx %s/insingularity_%d.sh'% (jobsDir,n))

        print("file %s/skimJob_%d.sh% (jobsDir,n)")

        
        condorFile = open ('%s/condorLauncher_%d.sh'% (jobsDir,n), 'w')
        condorFile.write ('Universe = vanilla\n')
        condorFile.write ('Executable  = '+jobsDir + '/skimJob_' + str (n) + '.sh\n')
        condorFile.write ('Log         = '+jobsDir + '/condor_job_$(ProcId).log\n')
        condorFile.write ('Output      = '+jobsDir + '/condor_job_$(ProcId).out\n')
        condorFile.write ('Error       = '+jobsDir + '/condor_job_$(ProcId).error\n')
        condorFile.write ('Requirements = ((machine == "pccms11.hcms.it") || (machine == "pccms12.hcms.it") || (machine == "pccms13.hcms.it") || (machine == "pccms14.hcms.it") )\n')
        condorFile.write ('queue 1\n')
        condorFile.close ()

        print(command)
        
        command = 'condor_submit '+ jobsDir + '/condorLauncher_' + str (n) + '.sh'
        if opt.sleep : time.sleep (0.1)
        os.system (command)
        commandFile.write (command + '\n')
        n = n + 1
    commandFile.close ()
