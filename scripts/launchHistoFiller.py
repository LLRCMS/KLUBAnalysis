#!/usr/bin/env python

import os
import sys
import argparse
import datetime

# Configurables
isCondor = 'True'    # allowed options: 'True' - 'False'
year     = '2016'    # allowed options: '2016' - '2017' - 2018'
channel  = 'TauTau'  # allowed options: 'ETau' - 'MuTau' - TauTau'
additionalTag = ''
tagDir   = 'analysis_2021_12_20'

# Variations to be run:
# syntax --> [name of dir, name of var suffix, doSyst, njobs, bTagReshapeWeight suffix, JER suffix, doTauFakes]
uncertainties = [
 # Central + shape uncertainties from skims
 ['central'     ,''             , True ,'30', '', '', False],

 # jet->tau_h fake unc. added in skimOutputter
 ['tauFakes'    ,''             , False, '3', '', '', True],

 # Tau shape uncertaintes (objects level, i.e. all variables recomputed)
 ['tauup_DM0'   ,'_tauup_DM0'   , False, '1', '', '', False],
 ['taudown_DM0' ,'_taudown_DM0' , False, '1', '', '', False],
 ['tauup_DM1'   ,'_tauup_DM1'   , False, '1', '', '', False],
 ['taudown_DM1' ,'_taudown_DM1' , False, '1', '', '', False],
 ['tauup_DM10'  ,'_tauup_DM10'  , False, '1', '', '', False],
 ['taudown_DM10','_taudown_DM10', False, '1', '', '', False],
 ['tauup_DM11'  ,'_tauup_DM11'  , False, '1', '', '', False],
 ['taudown_DM11','_taudown_DM11', False, '1', '', '', False],
 ['eleup_DM0'   ,'_eleup_DM0'   , False, '1', '', '', False],
 ['eledown_DM0' ,'_eledown_DM0' , False, '1', '', '', False],
 ['eleup_DM1'   ,'_eleup_DM1'   , False, '1', '', '', False],
 ['eledown_DM1' ,'_eledown_DM1' , False, '1', '', '', False],
 ['muup'        ,'_muup'        , False, '1', '', '', False],
 ['mudown'      ,'_mudown'      , False, '1', '', '', False],

 # JER uncertaintes (read from different skims)
 ['JERup'       ,''             , False, '1', '', '_JERup'  , False],
 ['JERdown'     ,''             , False, '1', '', '_JERdown', False],

 # JES total (objects level, i.e. all variables recomputed)
 ['jetupTot'    ,'_jetupTot'    , False, '1', '_jes_up'  , '', False],
 ['jetdownTot'  ,'_jetdownTot'  , False, '1', '_jes_down', '', False],

 # JES split (objects level, i.e. all variables recomputed)
 ['jetup1'      ,'_jetup1'      , False, '1', '_jetup1' , '', False],
 ['jetup2'      ,'_jetup2'      , False, '1', '_jetup2' , '', False],
 ['jetup3'      ,'_jetup3'      , False, '1', '_jetup3' , '', False],
 ['jetup4'      ,'_jetup4'      , False, '1', '_jetup4' , '', False],
 ['jetup5'      ,'_jetup5'      , False, '1', '_jetup5' , '', False],
 ['jetup6'      ,'_jetup6'      , False, '1', '_jetup6' , '', False],
 ['jetup7'      ,'_jetup7'      , False, '1', '_jetup7' , '', False],
 ['jetup8'      ,'_jetup8'      , False, '1', '_jetup8' , '', False],
 ['jetup9'      ,'_jetup9'      , False, '1', '_jetup9' , '', False],
 ['jetup10'     ,'_jetup10'     , False, '1', '_jetup10', '', False],
 ['jetup11'     ,'_jetup11'     , False, '1', '_jetup11', '', False],

 ['jetdown1'    ,'_jetdown1'    , False, '1', '_jetdown1' , '', False],
 ['jetdown2'    ,'_jetdown2'    , False, '1', '_jetdown2' , '', False],
 ['jetdown3'    ,'_jetdown3'    , False, '1', '_jetdown3' , '', False],
 ['jetdown4'    ,'_jetdown4'    , False, '1', '_jetdown4' , '', False],
 ['jetdown5'    ,'_jetdown5'    , False, '1', '_jetdown5' , '', False],
 ['jetdown6'    ,'_jetdown6'    , False, '1', '_jetdown6' , '', False],
 ['jetdown7'    ,'_jetdown7'    , False, '1', '_jetdown7' , '', False],
 ['jetdown8'    ,'_jetdown8'    , False, '1', '_jetdown8' , '', False],
 ['jetdown9'    ,'_jetdown9'    , False, '1', '_jetdown9' , '', False],
 ['jetdown10'   ,'_jetdown10'   , False, '1', '_jetdown10', '', False],
 ['jetdown11'   ,'_jetdown11'   , False, '1', '_jetdown11', '', False]
]


# Loop on uncertainties to be submitted
for uncertainty in uncertainties:

    # Create output dir
    if additionalTag == '':
        outDir = tagDir+'/'+channel+'_'+year+'/'+uncertainty[0]
    else:
        outDir = tagDir+'/'+channel+'_'+year+'_'+additionalTag+'/'+uncertainty[0]
    os.system('mkdir -p ' + outDir)

    # --- Selection Config ---
    # Open original main config
    originalSelectionCfg = open('config/selectionCfg_%s_Legacy%s_template.cfg' % (channel,year))

    # Create new main config for submission
    newSelectionCfg = open(outDir+'/selectionCfg_%s_Legacy%s.cfg' % (channel,year), 'w')

    # Modify selection config for submission
    for line in originalSelectionCfg:

        # Apply or not the systematics (shape uncertainties from skims)
        if 'XXXX' in line:
            if uncertainty[2]:
                line = line.replace('XXXX', '')
            else:
                line = line.replace('XXXX', '#')

        # Edit the btagReshape weight
        if 'YYYY' in line:
            line = line.replace('YYYY', uncertainty[4])

        # Apply the jet->tau_h fake unc. added in skimOutputter
        if 'ZZZZ' in line:
            if uncertainty[6]:
                line = line.replace('ZZZZ', '')
            else:
                line = line.replace('ZZZZ', '#')

        newSelectionCfg.write(line)
    originalSelectionCfg.close()
    newSelectionCfg.close()

    # --- Main Config ---
    # Open original main config
    originalMainCfg = open('config/mainCfg_%s_Legacy%s_template.cfg' % (channel,year))

    # Create new main config for submission
    newMainCfg = open(outDir+'/mainCfg_%s_Legacy%s_limits.cfg' % (channel,year), 'w')

    # Modify main config for submission
    for line in originalMainCfg:

        # Set output directory
        if 'XXXX' in line:
            line = line.replace('XXXX', outDir)
        
        # Set name of variable
        if 'YYYY' in line:
            line = line.replace('YYYY', uncertainty[1])

        # Set the correct selection config
        if 'ZZZZ' in line:
            line = line.replace('ZZZZ', outDir+'/selectionCfg_%s_Legacy%s.cfg' % (channel,year))

        # Set the correct filelist (used for JERup/down)
        if 'WWWW' in line:
            line = line.replace('WWWW', uncertainty[5])

        # Set correct input files pattern
        if 'PPPP' in line:
            if 'JER' in uncertainty[0]:
                line = line.replace('PPPP', 'goodfiles')
            elif 'tauFakes' in uncertainty[0]:
                line = line.replace('PPPP', 'goodtaufakesfiles')
            else:
                line = line.replace('PPPP', 'goodsystfiles')

        newMainCfg.write(line)
    originalMainCfg.close()
    newMainCfg.close()

    # --- Launch command ---
    command = 'python scripts/submitHistoFiller_parallel.py --cfg '+outDir+'/mainCfg_'+channel+'_Legacy'+year+'_limits.cfg --tag '+outDir+' --n '+uncertainty[3]+' --isCondor '+isCondor
    print command
    os.system(command)


#import pdb; pdb.set_trace()


