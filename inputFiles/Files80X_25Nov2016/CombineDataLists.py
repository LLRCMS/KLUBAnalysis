import os


# outTag = "25Nov2016"
# Mu = [
#     'Data25nov2016/1_SingleMuon__Run2016B-23Sep2016-v3.txt',
#     'Data25nov2016/2_SingleMuon__Run2016C-23Sep2016-v1.txt',
#     'Data25nov2016/3_SingleMuon__Run2016D-23Sep2016-v1.txt',
#     'Data25nov2016/4_SingleMuon__Run2016E-23Sep2016-v1.txt',
#     'Data25nov2016/5_SingleMuon__Run2016F-23Sep2016-v1.txt',
#     'Data25nov2016/6_SingleMuon__Run2016G-23Sep2016-v1.txt',
#     'Data25nov2016/7_SingleMuon__Run2016H-PromptReco-v2.txt',
#     'Data25nov2016/8_SingleMuon__Run2016H-PromptReco-v3.txt',
# ]

# Ele = [
#     'Data25nov2016/9_SingleElectron__Run2016B-23Sep2016-v3.txt',
#     'Data25nov2016/10_SingleElectron__Run2016C-23Sep2016-v1.txt',
#     'Data25nov2016/11_SingleElectron__Run2016D-23Sep2016-v1.txt',
#     'Data25nov2016/12_SingleElectron__Run2016E-23Sep2016-v1.txt',
#     'Data25nov2016/13_SingleElectron__Run2016F-23Sep2016-v1.txt',
#     'Data25nov2016/14_SingleElectron__Run2016G-23Sep2016-v1.txt',
#     'Data25nov2016/15_SingleElectron__Run2016H-PromptReco-v2.txt',
#     'Data25nov2016/16_SingleElectron__Run2016H-PromptReco-v3.txt',
# ]

# Tau = [
#     'Data25nov2016/17_Tau__Run2016B-23Sep2016-v3.txt',
#     'Data25nov2016/18_Tau__Run2016C-23Sep2016-v1.txt',
#     'Data25nov2016/19_Tau__Run2016D-23Sep2016-v1.txt',
#     'Data25nov2016/20_Tau__Run2016E-23Sep2016-v1.txt',
#     'Data25nov2016/21_Tau__Run2016F-23Sep2016-v1.txt',
#     'Data25nov2016/22_Tau__Run2016G-23Sep2016-v1.txt',
#     'Data25nov2016/23_Tau__Run2016H-PromptReco-v2.txt',
#     'Data25nov2016/24_Tau__Run2016H-PromptReco-v3.txt',
# ]


outTag = "25Nov2016BCDPromptReco"
Mu = [
    'DataPromptRecoBCD25nov2016/10_SingleMuon__Run2016D-PromptReco-v2.txt',
    'DataPromptRecoBCD25nov2016/2_SingleMuon__Run2016B-PromptReco-v2.txt',
    'DataPromptRecoBCD25nov2016/7_SingleMuon__Run2016C-PromptReco-v2.txt',
]

Ele = [
    'DataPromptRecoBCD25nov2016/11_SingleElectron__Run2016D-PromptReco-v2.txt',
    'DataPromptRecoBCD25nov2016/4_SingleElectron__Run2016B-PromptReco-v2.txt',
    'DataPromptRecoBCD25nov2016/8_SingleElectron__Run2016C-PromptReco-v2.txt',
]

Tau = [
    'DataPromptRecoBCD25nov2016/29Nov2016ReList/12_Tau__Run2016D-PromptReco-v2.txt',
    'DataPromptRecoBCD25nov2016/29Nov2016ReList/6_Tau__Run2016B-PromptReco-v2.txt',
    'DataPromptRecoBCD25nov2016/29Nov2016ReList/9_Tau__Run2016C-PromptReco-v2.txt',
]

print "\n\n\n"

command = "cat "
for i in Mu:
    command += (i + " ")
command += " > SingleMu_"+outTag+".txt"
print command
os.system(command)

print "\n\n\n"

command = "cat "
for i in Ele:
    command += (i + " ")
command += " > SingleEle_"+outTag+".txt"
print command
os.system(command)

print "\n\n\n"

command = "cat "
for i in Tau:
    command += (i + " ")
command += " > Tau_"+outTag+".txt"
print command
os.system(command)

print "\n\n\n"
