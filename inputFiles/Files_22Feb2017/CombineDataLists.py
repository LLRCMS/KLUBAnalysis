import os


outTag = "7FebreminiAOD_5Mar2017"
Mu = [
    '1_SingleMuon__Run2016B-03Feb2017_ver2-v2.txt',
    '2_SingleMuon__Run2016C-03Feb2017-v1.txt',
    '3_SingleMuon__Run2016D-03Feb2017-v1.txt',
    '4_SingleMuon__Run2016E-03Feb2017-v1.txt',
    '5_SingleMuon__Run2016F-03Feb2017-v1.txt',
    '6_SingleMuon__Run2016G-03Feb2017-v1.txt',
    '1_SingleMuon__Run2016H-03Feb2017_ver2-v1.txt',
    '2_SingleMuon__Run2016H-03Feb2017_ver3-v1.txt',
]

Ele = [
    '1_SingleElectron__Run2016B-03Feb2017_ver2-v2.txt',
    '8_SingleElectron__Run2016C-03Feb2017-v1.txt',
    '9_SingleElectron__Run2016D-03Feb2017-v1.txt',
    '10_SingleElectron__Run2016E-03Feb2017-v1.txt',
    '11_SingleElectron__Run2016F-03Feb2017-v1.txt',
    '12_SingleElectron__Run2016G-03Feb2017-v1.txt',
    '3_SingleElectron__Run2016H-03Feb2017_ver2-v1.txt',
    '4_SingleElectron__Run2016H-03Feb2017_ver3-v1.txt',
]

Tau = [
    '13_Tau__Run2016B-03Feb2017_ver2-v2.txt',
    '14_Tau__Run2016C-03Feb2017-v1.txt',
    '15_Tau__Run2016D-03Feb2017-v1.txt',
    '16_Tau__Run2016E-03Feb2017-v1.txt',
    '17_Tau__Run2016F-03Feb2017-v1.txt',
    '18_Tau__Run2016G-03Feb2017-v1.txt',
    '5_Tau__Run2016H-03Feb2017_ver2-v1.txt',
    '6_Tau__Run2016H-03Feb2017_ver3-v1.txt',
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
