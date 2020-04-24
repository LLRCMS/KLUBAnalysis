import os

### use this to combine all DY samples into a single list
### NOTE: compute the number of events per exclusive sample and update 

outTag = "25Nov2016"

allDY = [
'DY25NovSplit/1_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0_ext1-v1.txt',
'DY25NovSplit/2_DY1JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt',
'DY25NovSplit/3_DY2JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt',
'DY25NovSplit/4_DY3JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt',
'DY25NovSplit/5_DY4JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt',
'DY25NovSplit/6_DYBJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt',
'DY25NovSplit/7_DYBBJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v2.txt',
]


print "\n\n\n"

#### cat lists
command = "cat "
for i in allDY:
    command += (i + " ")
command += " > DY_mergedList_"+outTag+".txt"
print command
os.system(command)

print "\n\n\n"