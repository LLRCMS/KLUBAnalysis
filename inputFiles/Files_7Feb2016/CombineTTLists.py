import os

### use this to combine TT semilep, fullyLep, fullyHad, inclusive into final states from skims

outTag = "7Feb2017"

inclusive = [
'TT7FebSplit/22_TT_TuneCUETP8M2T4_13TeV-powheg-pythia8__RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt',
'TT7FebSplit/23_TT_TuneCUETP8M2T4_13TeV-powheg-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt',
]

semilep = [
'TT7FebSplit/21_TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt',
]

fullylep = [
'TT7FebSplit/20_TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt',
]


print "\n\n\n"

#### fullyHad
command = "cat "
for i in inclusive:
    command += (i + " ")
command += " > TT_powheg_fullyHad_"+outTag+".txt"
print command
os.system(command)

print "\n\n\n"

#### semilep
command = "cat "
for i in inclusive:
    command += (i + " ")
for i in semilep:
    command += (i + " ")
command += " > TT_powheg_semiLep_"+outTag+".txt"
print command
os.system(command)

print "\n\n\n"

#### fullylep
#### semilep
command = "cat "
for i in inclusive:
    command += (i + " ")
for i in fullylep:
    command += (i + " ")
command += " > TT_powheg_fullyLep_"+outTag+".txt"
print command
os.system(command)

print "\n\n\n"