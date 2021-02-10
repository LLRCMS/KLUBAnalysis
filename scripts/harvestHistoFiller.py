#!/usr/bin/env python

import os
import sys
import argparse
import datetime

# Configure these depending on what you want to harves
year    = '2016'  # allowed options: 2016 - 2017 - 2018
channel = 'ETau'  # allowed options: ETau - MuTau - TauTau
tagDir  = 'analysis_2021_02_08'

# Create input dir path
tagDir = tagDir+'/'+channel+'_'+year

# Create list of all uncertainty subdirectories
uncDirs = os.listdir(tagDir)

# Input vars to be saved in the merged mainCfg
allVars = ''

# Check that all jobs have finished
for uncDir in uncDirs:

    # Skip 'total' subdirectory if already present:
    if uncDir == 'total': continue

    # Read unc subdirectories
    inDir = tagDir + '/' + uncDir

    # Get list of log files
    allFiles = [f for f in os.listdir(inDir) if os.path.isfile(os.path.join(inDir, f))]
    logFiles = [f for f in allFiles if 'log_' in f]

    for logFile in logFiles:
        lastLine = os.popen('tail -n1 '+inDir+'/'+logFile).read()
        if 'exiting' in lastLine:
            continue
        else:
            sys.exit('ERROR: '+inDir+'/'+logFile+' did not finish properly, exiting!')

    # Read the main config for each unc and store the variables names
    for line in open(inDir+'/mainCfg_'+channel+'_Legacy'+year+'_limits.cfg'):
        if line.startswith('variables'):
            varNames = line.split('=')[1] # Get variables only
            varNames = varNames.strip()   # Remove carriage return
            if allVars == '':
                allVars = varNames
            else:
                allVars = allVars + ', ' + varNames

print 'All log files look ok!'

# Create a new directory to store the merged file
newDir = tagDir+'/total'
os.system('mkdir '+ newDir)

# Copy the samples and selection config files from the central case
copyCommand = 'cp '+tagDir+'/central/s*cfg '+newDir+'/'
print copyCommand
os.system(copyCommand)

# Create a new main config starting from the central config,
# but containing all the variables (central and shifted ones)
newMainCfg = open(newDir+'/mainCfg_'+channel+'_Legacy'+year+'_limits.cfg','w')
for line in open(inDir+'/mainCfg_'+channel+'_Legacy'+year+'_limits.cfg'):
    # Edit line with variables
    if line.startswith('variables'):
        line = 'variables = ' + allVars + '\n'
    newMainCfg.write(line)
newMainCfg.close()

# Launch the hadd command
haddCommand = 'hadd '+newDir+'/outPlotter.root '+tagDir+'/*/outPlotter_*root '
print haddCommand
os.system(haddCommand)



