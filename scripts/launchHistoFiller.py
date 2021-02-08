#!/usr/bin/env python

import os
import sys
import argparse
import datetime


# Configurables
isCondor = 'True'    # allowed options: 'True' - 'False'
year     = '2016'    # allowed options: '2016' - '2017' - 2018'
channel  = 'TauTau'  # allowed options: 'ETau' - 'MuTau' - TauTau'
tagDir   = 'analysis_2021_02_07'

# Variations to be run:
# syntax --> [name of dir, name of var suffix, doSyst, njobs, bTagReshapeWeight suffix]
uncertainties = [
 ['central'     ,''             , True ,'10', ''],
 ['tauup_DM0'   ,'_tauup_DM0'   , False, '1', ''],
 ['taudown_DM0' ,'_taudown_DM0' , False, '1', ''],
 ['tauup_DM1'   ,'_tauup_DM1'   , False, '1', ''],
 ['taudown_DM1' ,'_taudown_DM1' , False, '1', ''],
 ['tauup_DM10'  ,'_tauup_DM10'  , False, '1', ''],
 ['taudown_DM10','_taudown_DM10', False, '1', ''],
 ['tauup_DM11'  ,'_tauup_DM11'  , False, '1', ''],
 ['taudown_DM11','_taudown_DM11', False, '1', ''],
 ['eleup_DM0'   ,'_eleup_DM0'   , False, '1', ''],
 ['eledown_DM0' ,'_eledown_DM0' , False, '1', ''],
 ['eleup_DM1'   ,'_eleup_DM1'   , False, '1', ''],
 ['eledown_DM1' ,'_eledown_DM1' , False, '1', ''],
 ['muup'        ,'_muup'        , False, '1', ''],
 ['mudown'      ,'_mudown'      , False, '1', ''],

 ['jetupTot'    ,'_jetupTot'    , False, '1', '_jes_up'  ],
 ['jetdownTot'  ,'_jetdownTot'  , False, '1', '_jes_down'],

 ['jetup1'      ,'_jetup1'      , False, '1', '_jetup1' ],
 ['jetup2'      ,'_jetup2'      , False, '1', '_jetup2' ],
 ['jetup3'      ,'_jetup3'      , False, '1', '_jetup3' ],
 ['jetup4'      ,'_jetup4'      , False, '1', '_jetup4' ],
 ['jetup5'      ,'_jetup5'      , False, '1', '_jetup5' ],
 ['jetup6'      ,'_jetup6'      , False, '1', '_jetup6' ],
 ['jetup7'      ,'_jetup7'      , False, '1', '_jetup7' ],
 ['jetup8'      ,'_jetup8'      , False, '1', '_jetup8' ],
 ['jetup9'      ,'_jetup9'      , False, '1', '_jetup9' ],
 ['jetup10'     ,'_jetup10'     , False, '1', '_jetup10'],
 ['jetup11'     ,'_jetup11'     , False, '1', '_jetup11'],

 ['jetdown1'    ,'_jetdown1'    , False, '1', '_jetdown1' ],
 ['jetdown2'    ,'_jetdown2'    , False, '1', '_jetdown2' ],
 ['jetdown3'    ,'_jetdown3'    , False, '1', '_jetdown3' ],
 ['jetdown4'    ,'_jetdown4'    , False, '1', '_jetdown4' ],
 ['jetdown5'    ,'_jetdown5'    , False, '1', '_jetdown5' ],
 ['jetdown6'    ,'_jetdown6'    , False, '1', '_jetdown6' ],
 ['jetdown7'    ,'_jetdown7'    , False, '1', '_jetdown7' ],
 ['jetdown8'    ,'_jetdown8'    , False, '1', '_jetdown8' ],
 ['jetdown9'    ,'_jetdown9'    , False, '1', '_jetdown9' ],
 ['jetdown10'   ,'_jetdown10'   , False, '1', '_jetdown10'],
 ['jetdown11'   ,'_jetdown11'   , False, '1', '_jetdown11']
]


# Loop on uncertainties to be submitted
for uncertainty in uncertainties:

    # Create output dir
    outDir = tagDir+'/'+channel+'_'+year+'/'+uncertainty[0]
    os.system('mkdir -p ' + outDir)

    # --- Selection Config ---
    # Open original main config
    originalSelectionCfg = open('config/selectionCfg_%s_Legacy%s_template.cfg' % (channel,year))

    # Create new main config for submission
    newSelectionCfg = open(outDir+'/selectionCfg_%s_Legacy%s.cfg' % (channel,year), 'w')

    # Modify selection config for submission
    for line in originalSelectionCfg:

        # Apply or not the systematics
        if 'XXXX' in line:
            if uncertainty[2]:
                line = line.replace('XXXX', '')
            else:
                line = line.replace('XXXX', '#')

        # Edit the btagReshape weight
        if 'YYYY' in line:
            line = line.replace('YYYY', uncertainty[4])

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

        newMainCfg.write(line)
    originalMainCfg.close()
    newMainCfg.close()

    # --- Launch command ---
    command = 'python scripts/submitHistoFiller_parallel.py --cfg '+outDir+'/mainCfg_'+channel+'_Legacy'+year+'_limits.cfg --tag '+outDir+' --n '+uncertainty[3]+' --isCondor '+isCondor
    print command
    os.system(command)


#import pdb; pdb.set_trace()


