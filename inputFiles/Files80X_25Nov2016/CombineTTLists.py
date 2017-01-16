import os

### use this to combine TT semilep, fullyLep, fullyHad, inclusive into final states from skims

outTag = "25Nov2016"

inclusive = [
'TT25NovSplit/3_TT_TuneCUETP8M1_13TeV-powheg-pythia8__RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext3-v1.txt',
'TT25NovSplit/4_TT_TuneCUETP8M1_13TeV-powheg-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext4-v1.txt',
]

semilep = [
'TT25NovSplit/2_TTToSemiLeptonic_13TeV-powheg__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v2.txt',
]

fullylep = [
'TT25NovSplit/1_TTTo2L2Nu_13TeV-powheg__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1.txt',
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