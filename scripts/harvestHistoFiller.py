#!/usr/bin/env python

import os
import sys
import argparse
import datetime
import ROOT

# Configure these depending on what you want to harves
year    = '2016'  # allowed options: 2016 - 2017 - 2018
channel = 'ETau'  # allowed options: ETau - MuTau - TauTau
additionalTag = ''
tagDir  = 'analysis_2021_12_20'

# Create input dir path
if additionalTag == '':
    tagDir = tagDir+'/'+channel+'_'+year
else:
    tagDir = tagDir+'/'+channel+'_'+year+'_'+additionalTag

# Create list of all uncertainty subdirectories
uncDirs = os.listdir(tagDir)

# Input vars to be saved in the merged mainCfg
allVars = ''

# Check that all jobs have finished
for uncDir in uncDirs:

    # Skip 'total' subdirectory if already present
    if uncDir == 'total': continue

    # Read unc subdirectories
    inDir = tagDir + '/' + uncDir
    print 'Checking subdir: ', inDir

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
        if 'JER' in inDir: continue
        if 'tauFakes' in inDir: continue
        if line.startswith('variables'):
            varNames = line.split('=')[1] # Get variables only
            varNames = varNames.strip()   # Remove carriage return
            if allVars == '':
                allVars = varNames
            else:
                allVars = allVars + ', ' + varNames

print '--> All log files look ok!'

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
for line in open(tagDir+'/central/mainCfg_'+channel+'_Legacy'+year+'_limits.cfg'):
    # Edit line with variables
    if line.startswith('variables'):
        line = 'variables = ' + allVars + '\n'
    newMainCfg.write(line)
newMainCfg.close()

# Create "NEWoutput_*.root" file and add "JERup/JERdown" name to JER shifted histos
for uncDir in uncDirs:

    # Skip 'total' subdirectory if already present
    if uncDir == 'total': continue

    # Read unc subdirectories
    inDir = tagDir + '/' + uncDir
    print 'Changing names in: ', inDir

    # Get list of root files
    allFiles  = [f for f in os.listdir(inDir) if os.path.isfile(os.path.join(inDir, f))]
    rootFiles = [f for f in allFiles if 'outPlotter_' in f and 'NEW' not in f]

    for rootFile in rootFiles:

        # Get original name and create a new one
        originalName = inDir+'/'+rootFile
        newName = originalName.replace('outPlotter_','NEWoutPlotter_')

        # For central subdirectory just copy/paste the
        # the root file with the updated name
        if 'central' in uncDir:
            os.system('cp %s %s' % (originalName, newName))

        # Else if running on the shifted subdirectories
        # loop on all the histos and change names if needed
        else:

            # List to store the histos to be copied in the new file
            listHistos = []

            # Read the histos from the original file
            fin = ROOT.TFile(originalName)
            for key in fin.GetListOfKeys():
                kname = key.GetName()
                template = fin.Get(kname)

                # Do not copy the data for shifted subdirectories
                if 'data_obs' in kname: continue

                # Change name to histos in the JER subdirectories
                # by adding '_JERup' or '_JERdown'
                if 'JER' in uncDir:
                    template.SetName(kname+'_'+uncDir)
                    template.SetTitle(kname+'_'+uncDir)

                # Copy only the jetToTauFake up/down uncertainty templates and
                # do not re-add the "central values" templates
                if 'tauFakes' in uncDir:
                    if not 'jetToTauFake' in kname: continue

                # Store histos to be save in the new file
                listHistos.append(template.Clone())

            # Create the new file to store the histos with the correct name
            fout = ROOT.TFile(newName,'RECREATE')
            fout.cd()
            for h in listHistos:
                h.Write()
            fout.Close()

# Finally Launch the hadd command
haddCommand = 'hadd '+newDir+'/outPlotter.root '+tagDir+'/*/NEWoutPlotter_*root '
print haddCommand
os.system(haddCommand)
