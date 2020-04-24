import os
import glob
import re

### step 1 : list all the masses available

radionTag    = "*_GluGluToRadionToHHTo2B2Tau_M-*_narrow*.txt"
regexpSearch = "GluGluToRadionToHHTo2B2Tau\_M-(\d+)"
folder = "./"

#############################################

if folder[-1] != "/" : folder += "/"

# print folder + radionTag
allFiles = glob.glob(folder + radionTag)
allFiles = [x.lstrip(folder) for x in allFiles]

# print allFiles

masses = {}
for f in allFiles:
    m = re.search(regexpSearch, f)
    if m:
        # print "FOUND: " , m.groups()[0]
        mass = m.groups()[0]
        if not mass in masses :
            masses[mass] = []
        masses[mass].append(f)

for i in masses:
    command = "cat "
    for m in masses[i] : command += (m + " ")
    command += " > radion_" + i + ".txt"
    # print command
    os.system(command)