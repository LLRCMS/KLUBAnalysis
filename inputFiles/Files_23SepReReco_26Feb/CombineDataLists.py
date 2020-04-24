import os


outTag = "23SepReReco_26Feb2017"
Mu = [
    '1_SingleMuon__Run2016B-23Sep2016-v3.txt',
    '2_SingleMuon__Run2016C-23Sep2016-v1.txt',
    '3_SingleMuon__Run2016D-23Sep2016-v1.txt',
    '4_SingleMuon__Run2016E-23Sep2016-v1.txt',
    '5_SingleMuon__Run2016F-23Sep2016-v1.txt',
    '1_SingleMuon__Run2016G-23Sep2016-v1.txt',
    '7_SingleMuon__Run2016H-PromptReco-v2.txt',
    '8_SingleMuon__Run2016H-PromptReco-v3.txt',
]

Ele = [
    '1_SingleElectron__Run2016B-23Sep2016-v3.txt',
    '10_SingleElectron__Run2016C-23Sep2016-v1.txt',
    '11_SingleElectron__Run2016D-23Sep2016-v1.txt',
    '12_SingleElectron__Run2016E-23Sep2016-v1.txt',
    '13_SingleElectron__Run2016F-23Sep2016-v1.txt',
    '14_SingleElectron__Run2016G-23Sep2016-v1.txt',
    '15_SingleElectron__Run2016H-PromptReco-v2.txt',
    '16_SingleElectron__Run2016H-PromptReco-v3.txt',
]

Tau = [
    '2_Tau__Run2016B-23Sep2016-v3.txt',
    '18_Tau__Run2016C-23Sep2016-v1.txt',
    '19_Tau__Run2016D-23Sep2016-v1.txt',
    '20_Tau__Run2016E-23Sep2016-v1.txt',
    '21_Tau__Run2016F-23Sep2016-v1.txt',
    '22_Tau__Run2016G-23Sep2016-v1.txt',
    '23_Tau__Run2016H-PromptReco-v2.txt',
    '24_Tau__Run2016H-PromptReco-v3.txt',
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
