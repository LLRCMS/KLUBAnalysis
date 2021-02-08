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

# Check that all jobs have finished
for uncDir in uncDirs:
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

print 'All log files look ok!'

# Create a new directory to store the merged file
newDir = tagDir+'/total'
os.system('mkdir '+ newDir)

# Copy the config files
copyCommand = 'cp '+tagDir+'/central/*cfg '+newDir+'/'
print copyCommand
os.system(copyCommand)

# Launch the hadd command
haddCommand = 'hadd '+newDir+'/outPlotter.root '+tagDir+'/*/outPlotter_*root '
print haddCommand
os.system(haddCommand)



