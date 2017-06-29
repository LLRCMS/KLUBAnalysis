import os

### use this to combine all DY samples into a single list
### NOTE: compute the number of events per exclusive sample and update 

outTag = "7Feb2017"

allDY = [
'DY7FebSplit/1_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2.txt',
'DY7FebSplit/2_DY1JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt',
'DY7FebSplit/3_DY2JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt',
'DY7FebSplit/4_DY3JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt',
'DY7FebSplit/5_DY4JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt',
'DY7FebSplit/6_DYBJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt',
'DY7FebSplit/7_DYBBJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt',
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