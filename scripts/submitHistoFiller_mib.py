#!/usr/bin/env python


###### LAUNCH COMMAND EXAMPLE:
#
# - HISTO FILLER for PLOTTING -
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_MuTau.cfg  --tag analysis_MuTau_6Aug2018_noNLO --n 28
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_ETau.cfg   --tag analysis_ETau_1Aug2018    --n 30
#
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_14Jul2018 --n 44
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_14Jul2018_SStight --n 44
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_14Jul2018_style --n 44
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_25Jul2018    --n 40
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_25Jul2018_reduced_trgSF --n 50
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_28Jul2018_newSF --n 50
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_27Jul2018_pt45 --n 50
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_27Jul2018_pt45_noSF --n 50
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_30Jul2018_newPU_oldSF --n 50
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_1Aug2018_noPU_noSF_splitDY --n 30
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_8Aug2018 --n 28
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_19Oct2018_DY_Zpt_Zmass --n 30
#
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_MuTau.cfg  --tag analysis_MuTau_25Oct2018_TESI  --n 25
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_MuTau.cfg  --tag analysis_MuTau_25Oct2018_TESI_noNLO  --n 30
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_MuTau.cfg  --tag analysis_MuTau_25Oct2018_TESI_NLO  --n 30
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_MuTau.cfg  --tag analysis_MuTau_25Oct2018_TESI_NLO_BDTouts  --n 30
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_ETau.cfg   --tag analysis_ETau_25Oct2018_TESI_test   --n 18
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_25Oct2018_TESI --n 25
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_25Oct2018_TESI_BDTouts --n 30
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_25Oct2018_TESI_RES --n 35
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_25Oct2018_TESI_VBF --n 40
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_25Oct2018_TESI_ggH --n 40
#
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_MuTau.cfg  --tag analysis_MuTau_9Nov2018_testBDT  --n 20
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_MuMu.cfg   --tag analysis_MuMu_12Nov2018 --n 40
# python scripts/submitHistoFiller_mib.py --cfg config/mainCfg_TauTau.cfg --tag analysis_TauTau_13Nov2018_DYNLO --n 40

#
# - LOOK at QCD -
#
# python scripts/submitHistoFiller_mib.py --cfg config/lookAtQCD_VBF_MuTau.cfg  --tag lookatQCD_MuTau_1Aug2018  --n 25
# python scripts/submitHistoFiller_mib.py --cfg config/lookAtQCD_VBF_ETau.cfg   --tag lookatQCD_ETau_1Aug2018   --n 25
# python scripts/submitHistoFiller_mib.py --cfg config/lookAtQCD_VBF_TauTau.cfg --tag lookatQCD_TauTau_1Aug2018 --n 50
#
# python scripts/submitHistoFiller_mib.py --cfg config/lookAtQCD_VBF_TauTau.cfg --tag lookatQCD_TauTau_6Aug2018 --n 28
# python scripts/submitHistoFiller_mib.py --cfg config/lookAtQCD_VBF_TauTau.cfg --tag lookatQCD_TauTau_25Jul2018_newPU --n 40
# python scripts/submitHistoFiller_mib.py --cfg config/lookAtQCD_VBF_TauTau.cfg --tag lookatQCD_TauTau_28Jul2018_newSF --n 40
# python scripts/submitHistoFiller_mib.py --cfg config/lookAtQCD_VBF_TauTau.cfg --tag lookatQCD_TauTau_30Jul2018_newPU_oldSF --n 50
# python scripts/submitHistoFiller_mib.py --cfg config/lookAtQCD_VBF_TauTau.cfg --tag lookatQCD_TauTau_8Sep2018_all --n 45
# python scripts/submitHistoFiller_mib.py --cfg config/lookAtQCD_VBF_TauTau.cfg --tag lookatQCD_TauTau_19Oct2018_DY_Zpt_Zmass --n 30
#
# python scripts/submitHistoFiller_mib.py --cfg config/lookAtQCD_VBF_MuTau.cfg  --tag lookatQCD_MuTau_25Oct2018_TESI  --n 25
# python scripts/submitHistoFiller_mib.py --cfg config/lookAtQCD_VBF_TauTau.cfg --tag lookatQCD_TauTau_25Oct2018_TESI --n 25

# python scripts/submitHistoFiller_mib.py --cfg config/lookAtQCD_VBF_TauTau.cfg --tag lookatQCD_TauTau_13Nov2018 --n 40
# python scripts/submitHistoFiller_mib.py --cfg config/lookAtQCD_VBF_TauTau.cfg --tag lookatQCD_TauTau_13Nov2018_T --n 40


################

import os
import sys
import argparse
import datetime

parser = argparse.ArgumentParser(description='Command line parser of plotting options')
parser.add_argument('--cfg',    dest='cfg',   help='name of the main cfg to run', default=None)
parser.add_argument('--tag',    dest='tag',   help='name of working space (defaults to timestamp)', default=None)
parser.add_argument('--njobs',  dest='njobs', type=int, help='number of jobs for parallelization',  default=17)
args = parser.parse_args()


if not args.cfg:
    print "** Please provide a cfg name: --cfg <relative path from KLUBanalysis/ or absolute path>"
    sys.exit()

# datetime.datetime.now()
outDir = datetime.datetime.now().strftime('%Y.%m.%d_%H.%M.%S')
if args.tag:
    outDir = args.tag
#outDir = "JEC_jobs_pt25/"+outDir

# pathname = os.path.dirname(sys.argv[0])        
# here     = os.path.abspath(pathname)
here = os.getcwd()

program = 'testAnalysisHelper.exe'

proto = 'filler_' ## job .sh fie name
logproto = 'log_' ## job .sh fie name

os.system('mkdir ' + outDir)
for nj in range(0, args.njobs):
    scriptName = proto + str(nj) + '.sh'
    logName    = logproto + str(nj) + '.txt'
    scriptFile = open (outDir + '/' + scriptName, 'w')
    scriptFile.write ('#!/bin/bash\n')
    scriptFile.write ('source /cvmfs/cms.cern.ch/cmsset_default.sh\n')
    scriptFile.write ('eval `scram r -sh`\n')
    scriptFile.write ('cd %s\n' % here)
    scriptFile.write ('eval `scram r -sh`\n')
    scriptFile.write ('source scripts/setup.sh\n')
    command = program + ' ' + args.cfg + ' ' + str(nj) + ' ' + str(args.njobs) + ' 2>&1 | tee ' + outDir + '/' + logName
    scriptFile.write(command)
    scriptFile.close()

    
    os.system ('chmod u+rwx ' + outDir + '/' + scriptName)
    #launchcommand = ('/usr/bin/qsub -q  longcms ' + outDir + '/' + scriptName)
    launchcommand = ('/usr/bin/qsub -q  shortcms ' + outDir + '/' + scriptName)
    print launchcommand
    os.system (launchcommand)
