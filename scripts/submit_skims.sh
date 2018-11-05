AMESSAGE="SF fixed; bitwise trigger passed info"
#OUTDIRR="SKIMS_3Oct2018_Run2017_synch"
#OUTDIRR="SKIMS_3Oct2018_Run2017TEST"
OUTDIRR="SKIMS_29Oct2018_Run2017"
INPUTDIR="inputFiles/Files_June2018_LLR"
PUDIR="weights/PUreweight/outputs"
SKIMDIR="/data_CMS/cms/amendola/HH2017Skims/"

source scripts/setup.sh
source /opt/exp_soft/cms/t3/t3setup
mkdir /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/
mkdir $OUTDIRR
touch /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/README.txt
echo $AMESSAGE > /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/README.txt
cp /home/llr/cms/amendola/CMSSW_7_4_7/src/KLUBAnalysis/scripts/listAll.sh /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/


#SYNCH
#python scripts/skimNtuple.py -T Synch2018 -s True -c  config/skim_2017_sync.cfg  -n 1  -k False -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_Grav450        -i inputFiles/Synch2018/Grav450.list -v True


##################
### SKIMS 2017 ###
##################

#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_SingleMuon2017B        -i inputFiles/Data2017_26Jun2018/1_SingleMuon__Run2017B-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_SingleMuon2017C        -i inputFiles/Data2017_26Jun2018/2_SingleMuon__Run2017C-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_SingleMuon2017D        -i inputFiles/Data2017_26Jun2018/3_SingleMuon__Run2017D-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_SingleMuon2017E        -i inputFiles/Data2017_26Jun2018/4_SingleMuon__Run2017E-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_SingleMuon2017F        -i inputFiles/Data2017_26Jun2018/5_SingleMuon__Run2017F-31Mar2018-v1.txt

#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_SingleElectron2017B        -i inputFiles/Data2017_26Jun2018/1_SingleElectron__Run2017B-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_SingleElectron2017C        -i inputFiles/Data2017_26Jun2018/2_SingleElectron__Run2017C-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_SingleElectron2017D        -i inputFiles/Data2017_26Jun2018/3_SingleElectron__Run2017D-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_SingleElectron2017E        -i inputFiles/Data2017_26Jun2018/4_SingleElectron__Run2017E-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_SingleElectron2017F        -i inputFiles/Data2017_26Jun2018/5_SingleElectron__Run2017F-31Mar2018-v1.txt

 #python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_Tau2017B        -i inputFiles/Data2017_26Jun2018/1_Tau__Run2017B-31Mar2018-v1.txt
 #python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_Tau2017C        -i inputFiles/Data2017_26Jun2018/2_Tau__Run2017C-31Mar2018-v1.txt
 #python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_Tau2017D        -i inputFiles/Data2017_26Jun2018/3_Tau__Run2017D-31Mar2018-v1.txt
 #python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_Tau2017E        -i inputFiles/Data2017_26Jun2018/4_Tau__Run2017E-31Mar2018-v1.txt
 #python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_Tau2017F        -i inputFiles/Data2017_26Jun2018/5_Tau__Run2017F-31Mar2018-v1.txt



# TT
# TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
# hh = 45.7%
# ll = 10.5%
# hl = 21.9% (x2 for permutation t-tbar)


#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 500 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_fullyHad -i $INPUTDIR/15_TTToHadronic_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 377.96 -t True -b 1  --pu $PUDIR/15_TT_fullyHad.txt


#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 700 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_fullyLep -i $INPUTDIR/13_TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 88.29  -t True -b 4 --pu $PUDIR/13_TT_fullyLep.txt


#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 700 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_semiLep -i $INPUTDIR/14_TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 365.34 -t True -b 5 --pu $PUDIR/14_TT_semiLep.txt





### DY

# total XS= 5765.4 pb
# myPUHisto_0j0b : xsFrac: 0.697167 - xs: 4019.4 pb
# myPUHisto_1j0b : xsFrac: 0.196537 - xs: 1133.1 pb
# myPUHisto_1j1b : xsFrac: 0.007292 - xs: 42.04  pb
# myPUHisto_2j0b : xsFrac: 0.063167 - xs: 364.18 pb
# myPUHisto_2j1b : xsFrac: 0.003200 - xs: 18.5   pb
# myPUHisto_2j2b : xsFrac: 0.001070 - xs: 6.17   pb
# myPUHisto_3j0b : xsFrac: 0.018860 - xs: 108.73 pb
# myPUHisto_3j1b : xsFrac: 0.000887 - xs: 5.11   pb
# myPUHisto_3j2b : xsFrac: 0.000704 - xs: 4.06   pb
# myPUHisto_3j3b : xsFrac: 9.78e-06 - xs: 0.056  pb
# myPUHisto_4j0b : xsFrac: 0.010130 - xs: 58.40  pb
# myPUHisto_4j1b : xsFrac: 0.000425 - xs: 2.45   pb
# myPUHisto_4j2b : xsFrac: 0.000535 - xs: 3.08   pb
# myPUHisto_4j3b : xsFrac: 1.12e-05 - xs: 0.064  pb
# myPUHisto_4j4b : xsFrac: 1.98e-06 - xs: 0.011  pb


# total XS=6233.55 pb ----> to update:  6225.42 

# myPUHisto_0j0b : xsFrac: 0.746053    - xs: 4650.56 pb
# myPUHisto_1j0b : xsFrac: 0.157576    - xs: 982.26 pb	
# myPUHisto_1j1b : xsFrac: 0.00658535  - xs: 41.05 pb	
# myPUHisto_2j0b : xsFrac: 0.0534837   - xs: 333.39 pb	
# myPUHisto_2j1b : xsFrac: 0.00273262  - xs: 17.03 pb 	
# myPUHisto_2j2b : xsFrac: 0.00118358  - xs: 7.38 pb	
# myPUHisto_3j0b : xsFrac: 0.0188891   - xs: 117.75 pb
# myPUHisto_3j1b : xsFrac: 0.000781306 - xs: 4.87 pb
# myPUHisto_3j2b : xsFrac: 0.000881659 - xs: 5.50 pb
# myPUHisto_3j3b : xsFrac: 9.87433E-06 - xs: 0.061 pb
# myPUHisto_4j0b : xsFrac: 0.0106378   - xs: 66.31 pb
# myPUHisto_4j1b : xsFrac: 0.000360878 - xs: 2.25 pb
# myPUHisto_4j2b : xsFrac: 0.000811201 - xs: 5.06 pb 
# myPUHisto_4j3b : xsFrac: 1.0322E-05  - xs: 0.064 pb
# myPUHisto_4j4b : xsFrac: 2.77862E-05 - xs: 0.17 pb

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 700 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DYJets_0j0b -i $INPUTDIR/DYJets.txt -g False -x 4650.56   --pu $PUDIR/DY_special/DY_0j0b.txt --nj 0 --nb 0 

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 700 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DYJets_1j0b -i $INPUTDIR/DYJets.txt -g False -x 982.26  --pu $PUDIR/DY_special/DY_1j0b.txt --nj 1 --nb 0 

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 500 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DYJets_1j1b -i $INPUTDIR/DYJets.txt -g False -x 41.05  --pu $PUDIR/DY_special/DY_1j1b.txt --nj 1 --nb 1 

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 1000 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DYJets_2j0b -i $INPUTDIR/DYJets.txt -g False -x 333.39 --pu $PUDIR/DY_special/DY_2j0b.txt --nj 2 --nb 0 

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 500 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DYJets_2j1b -i $INPUTDIR/DYJets.txt -g False -x 17.03 --pu $PUDIR/DY_special/DY_2j1b.txt --nj 2 --nb 1 

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 1000 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DYJets_2j2b -i $INPUTDIR/DYJets.txt -g False -x 7.38 --pu $PUDIR/DY_special/DY_2j2b.txt --nj 2 --nb 2 

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 1000 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DYJets_3j0b -i $INPUTDIR/DYJets.txt -g False -x 117.75 --pu $PUDIR/DY_special/DY_3j0b.txt --nj 3 --nb 0 

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 500 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DYJets_3j1b -i $INPUTDIR/DYJets.txt -g False -x 4.87  --pu $PUDIR/DY_special/DY_3j1b.txt --nj 3 --nb 1 

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 400 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DYJets_3j2b -i $INPUTDIR/DYJets.txt -g False -x 5.50  --pu $PUDIR/DY_special/DY_3j2b.txt --nj 3 --nb 2 

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 1000 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DYJets_3j3b -i $INPUTDIR/DYJets.txt -g False -x 0.061 --pu $PUDIR/DY_special/DY_3j3b.txt --nj 3 --nb 3 

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 600 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DYJets4j0b -i $INPUTDIR/DYJets.txt -g False -x 66.31 --pu $PUDIR/DY_special/DY_4j0b.txt --nj 4 --nb 0 

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 600 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DYJets_4j1b -i $INPUTDIR/DYJets.txt -g False -x 2.25  --pu $PUDIR/DY_special/DY_4j1b.txt --nj 4 --nb 1 

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 700 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DYJets_4j2b -i $INPUTDIR/DYJets.txt -g False -x 5.06  --pu $PUDIR/DY_special/DY_4j2b.txt --nj 4 --nb 2 

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 700 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DYJets_4j3b -i $INPUTDIR/DYJets.txt -g False -x 0.064  --pu $PUDIR/DY_special/DY_4j3b.txt --nj 4 --nb 3

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 500 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DYJets_4j4b -i $INPUTDIR/DYJets.txt -g False -x 0.17  --pu $PUDIR/DY_special/DY_4j4b.txt --nj 4 --nb 4 


# ########
# # DY NLO
# #

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 700 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DY_NLO -i inputFiles/MC_16Oct2018/1_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x  6225.42  --pu $PUDIR/DYNLO.txt





# # #####################
# Wjets


python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_0_100     -i $INPUTDIR/WJetsToLNu_HT-0To100.txt    -y 1.213784 -x 48917.48 -z 100 --pu $PUDIR/WJets.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_100_200   -i $INPUTDIR/21_WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt  -y 1.213784 -x 1345  --pu $PUDIR/WJets.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_200_400   -i $INPUTDIR/22_WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -y 1.213784 -x 359.7  --pu $PUDIR/WJets.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_400_600   -i $INPUTDIR/23_WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -y 1.213784 -x 48.91 --pu $PUDIR/WJets.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_600_800   -i $INPUTDIR/24_WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -y 1.213784 -x 12.05 --pu $PUDIR/WJets.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_800_1200  -i $INPUTDIR/25_WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt  -y 1.213784 -x 5.501  --pu $PUDIR/WJets.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_1200_2500 -i $INPUTDIR/26_WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -y 1.213784 -x 1.329  --pu $PUDIR/WJets.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_2500_Inf  -i $INPUTDIR/WJetsToLNu_HT-2500ToInf.txt  -y 1.213784 -x 0.03216 -Z 2500 --pu $PUDIR/WJets.txt

 

# # #####################
#### ELECTROWEAK
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 200 -k True -o $SKIMDIR/$OUTDIRR/SKIM_EWKWPlus2Jets_WToLNu   -i $INPUTDIR/9_EWKWPlus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -x 25.62 --pu $PUDIR/9_EWKWPlus2Jets_WToLNu.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 200 -k True -o $SKIMDIR/$OUTDIRR/SKIM_EWKWMinus2Jets_WToLNu  -i $INPUTDIR/10_EWKWMinus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt  -x 20.25 --pu $PUDIR/10_EWKWMinus2Jets_WToLNu.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 200 -k True -o $SKIMDIR/$OUTDIRR/SKIM_EWKZ2Jets_ZToLL        -i $INPUTDIR/11_EWKZ2Jets_ZToLL_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt        -x 3.987 --pu $PUDIR/11_EWKZ2Jets_ZToLL.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 200 -k True -o $SKIMDIR/$OUTDIRR/SKIM_EWKZ2Jets_ZToNuNu      -i $INPUTDIR/12_EWKZ2Jets_ZToNuNu_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt        -x 10.66  --pu $PUDIR/12_EWKZ2Jets_ZToNuNu.txt




# # #####################
#### single top :

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 200 -k True -o $SKIMDIR/$OUTDIRR/SKIM_ST_tW_antitop         -i $INPUTDIR/18_ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -x 34.97  --pu $PUDIR/18_ST_tW_antitop.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 200 -k True -o $SKIMDIR/$OUTDIRR/SKIM_ST_tW_top             -i $INPUTDIR/19_ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt       -x 35.6  --pu $PUDIR/19_ST_tW_top.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 200 -k True -o $SKIMDIR/$OUTDIRR/SKIM_ST_tchannel_antitop   -i $INPUTDIR/16_ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -x 80.95 --pu $PUDIR/16_ST_t-channel_antitop.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 200 -k True -o $SKIMDIR/$OUTDIRR/SKIM_ST_tchannel_top       -i $INPUTDIR/17_ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt       -x 136.02 --pu $PUDIR/17_ST_t-channel_top.txt

# # #####################
#### SM Higgs :
### HXSWG: xs(ZH) = 0.8839 pb
### Z->qq : 69.91% , Z->ll : 3,3658% (x3 for all the leptons)
### H->bb : 57.7%  , H->tautau : 6.32%
### ZH (Zll, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
### ZH (Zqq, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
### ZH (Zall, Htautau) : XS teor ZH * BR H->tautau

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_ZH_HBB_ZLL -i $INPUTDIR/49_ZH_HToBB_ZToLL_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.07924 -y 0.577  --pu $PUDIR/49_ZH_HToBB_ZToLL.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_ZH_HBB_ZQQ -i $INPUTDIR/50_ZH_HToBB_ZToQQ_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.554 -y 0.577  --pu $PUDIR/50_ZH_HToBB_ZToQQ.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_ZH_HTauTau -i $INPUTDIR/53_ZHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.8696 -y 0.0632  --pu $PUDIR/53_ZHToTauTau.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_ggHTauTau -i $INPUTDIR/54_GluGluHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 21.46 -y 0.0632  --pu $PUDIR/54_GluGluHToTauTau.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_VBFHTauTau -i $INPUTDIR/55_VBFHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 3.861 -y 0.0632  --pu $PUDIR/55_VBFHToTauTau.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 70 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WplusHTauTau -i $INPUTDIR/56_WplusHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.867 -y 0.0632  --pu $PUDIR/56_WplusHToTauTau.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 70 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WminusHTauTau -i $INPUTDIR/57_WminusHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.5412 -y 0.0632  --pu $PUDIR/57_WminusHToTauTau.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_ttHJetTononBB -i $INPUTDIR/63_ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.507 -y 0.423  --pu $PUDIR/63_ttHJetToNonbb.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_ttHJetToBB -i $INPUTDIR/64_ttHJetTobb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.507 -y 0.577  --pu $PUDIR/64_ttHJetTobb.txt



# # #####################
#### Others :

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 20 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TTWJetsToLNu -i $INPUTDIR/65_TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.2043  --pu $PUDIR/65_TTWJetsToLNu.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 20 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TTWJetsToQQ -i $INPUTDIR/66_TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.4062  --pu $PUDIR/66_TTWJetsToQQ.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 20 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TTWW -i $INPUTDIR/67_TTWW_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1.txt -x 0.007834  --pu $PUDIR/67_TTWW.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 20 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TTWZ -i $INPUTDIR/68_TTWZ_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.002441  --pu $PUDIR/68_TTWZ.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 20 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TTZZ -i $INPUTDIR/72_TTZZ_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.001386  --pu $PUDIR/72_TTZZ.txt




# # #####################
# # ### DI-boson: - 7*7 + 6*5 = 49 + 30 = 79
# # # note2: all njobs listed here were increased by x10 to make them run on the short queue
# # #XS taken from here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson
# ## for Z->4Q, I scaled from ZZ->2nu2Q using BR(2Nu2Q) = 2 * 20% * 69.91%

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 50   -k True -o $SKIMDIR/$OUTDIRR/SKIM_ZZTo4L              -i $INPUTDIR/ZZTo4L.txt -x 1.325  --pu $PUDIR/ZZTo4L.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 50   -k True -o $SKIMDIR/$OUTDIRR/SKIM_ZZTo2L2Q            -i $INPUTDIR/28_ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 3.688  --pu $PUDIR/28_ZZTo2L2Q.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 50   -k True -o $SKIMDIR/$OUTDIRR/SKIM_ZZZ                 -i $INPUTDIR/31_ZZZ_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.01398  --pu $PUDIR/31_ZZZ.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 50   -k True -o $SKIMDIR/$OUTDIRR/SKIM_WWW                 -i $INPUTDIR/32_WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.2086   --pu $PUDIR/32_WWW.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 50   -k True -o $SKIMDIR/$OUTDIRR/SKIM_WWZ                 -i $INPUTDIR/33_WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.1651  --pu $PUDIR/33_WWZ.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 50   -k True -o $SKIMDIR/$OUTDIRR/SKIM_WWTo2L2Nu           -i $INPUTDIR/35_WWTo2L2Nu_NNPDF31_TuneCP5_PSweights_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1.txt -x 11.08  --pu $PUDIR/35_WWTo2L2Nu.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 50   -k True -o $SKIMDIR/$OUTDIRR/SKIM_WWToLNuQQ           -i $INPUTDIR/WWToLNuQQ.txt -x 45.99  --pu $PUDIR/37_WWToLNuQQ.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 50   -k True -o $SKIMDIR/$OUTDIRR/SKIM_WWTo4Q              -i $INPUTDIR/38_WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 47.73   --pu $PUDIR/38_WWTo4Q.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 50   -k True -o $SKIMDIR/$OUTDIRR/SKIM_WZTo3LNu_0Jets -i $INPUTDIR/42_WZTo3LNu_0Jets_MLL-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3.txt -x 0.6368  --pu $PUDIR/42_WZTo3LNu_0Jets.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 50   -k True -o $SKIMDIR/$OUTDIRR/SKIM_WZTo3LNu_1Jets -i $INPUTDIR/43_WZTo3LNu_1Jets_MLL-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.3371  --pu $PUDIR/43_WZTo3LNu_1Jets.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 50   -k True -o $SKIMDIR/$OUTDIRR/SKIM_WZTo3LNu_2Jets -i $INPUTDIR/44_WZTo3LNu_2Jets_MLL-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.06616  --pu $PUDIR/44_WZTo3LNu_2Jets.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 50   -k True -o $SKIMDIR/$OUTDIRR/SKIM_WZTo3LNu_3Jets -i $INPUTDIR/45_WZTo3LNu_3Jets_MLL-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.09558   --pu $PUDIR/45_WZTo3LNu_3Jets.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 50   -k True -o $SKIMDIR/$OUTDIRR/SKIM_WZTo1L1Nu2Q         -i $INPUTDIR/46_WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 11.66  --pu $PUDIR/46_WZTo1L1Nu2Q.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 50   -k True -o $SKIMDIR/$OUTDIRR/SKIM_WZTo2L2Q            -i $INPUTDIR/47_WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 6.331  --pu $PUDIR/47_WZTo2L2Q.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 50   -k True -o $SKIMDIR/$OUTDIRR/SKIM_WZZ                 -i $INPUTDIR/48_WZZ_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.05565  --pu $PUDIR/48_WZZ.txt




##################
### SKIMS 2016 ###
##################



# # #############################
# # ##### ### W+jets in HT bins 
# # ## WJets for HT bins -- chopping away HT > 70 in the inclusive sample is fine, as it is only ~10% of the total number of events
# # ## tot xs NNLO: 61526.7 (+497.1 -264.6) (+/- 2312.7) pb
# # ## individual xs are taken from : https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#W_jets
# # ## and the one for 0-70 and 70-100 computed by difference and using the inclusive sample distribution
# # ## xs LO : 50690 pb
# # ## xs LO (HT < 100 ) = 50690 - sum (xs HT > 100) = 50690 - 1772.52216 = 48917.47784
# # ## then using the inlcusive sample :
# # ## ch->GetEntries("lheHt < 70")                : 186703 (97.09%) --> 47493.98 pb
# # ## ch->GetEntries("lheHt > 70 && lheHt < 100") : 5589   (2.91%)  --> 1423.499 pb
# # ## Finally, everything is scaled from the LO to the NNLO xs : (61526.7/50690.) = 1.213784
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_WJets_HT_0_70       -i inputFiles/Files_7Feb2016/WJets_incl.txt        -y 1.213784 -x 47493.98 -z 70
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_WJets_HT_70_100     -i inputFiles/Files_7Feb2016/WJetsHT-70-100.txt    -y 1.213784 -x 1423.499
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_WJets_HT_100_200    -i inputFiles/Files_7Feb2016/WJetsHT-100-200.txt   -y 1.213784 -x 1345
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_WJets_HT_200_400    -i inputFiles/Files_7Feb2016/WJetsHT-200-400.txt   -y 1.213784 -x 359.7
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_WJets_HT_400_600    -i inputFiles/Files_7Feb2016/WJetsHT-400-600.txt   -y 1.213784 -x 48.91
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_WJets_HT_600_800    -i inputFiles/Files_7Feb2016/WJetsHT-600-800.txt   -y 1.213784 -x 12.05
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_WJets_HT_800_1200   -i inputFiles/Files_7Feb2016/WJetsHT-800-1200.txt  -y 1.213784 -x 5.501
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_WJets_HT_1200_2500  -i inputFiles/Files_7Feb2016/WJetsHT-1200-2500.txt -y 1.213784 -x 1.329
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_WJets_HT_2500_Inf   -i inputFiles/Files_7Feb2016/WJetsHT-2500-Inf.txt  -y 1.213784 -x 0.03216


# # ####################
# # ## DY incl
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DY   -i inputFiles/Files_7Feb2016/DY_mergedList_7Feb2017.txt -g True -x 5765.4

# # ####################
# # ## DY in HT bins
# # xs from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#DY_Z 
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DY_HT_0_70   -i inputFiles/Files_7Feb2016/DY_mergedList_7Feb2017.txt -g True -x 5765.4 -z 70
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DY_HT_70_100   -i inputFiles/Files_7Feb2016/DY7FebSplit/8_DYJetsToLL_M-50_HT-70to100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 169.9
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DY_HT_100_200   -i inputFiles/Files_7Feb2016/DY7FebSplit/9_DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 147.40
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DY_HT_200_400   -i inputFiles/Files_7Feb2016/DY7FebSplit/11_DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 40.99
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DY_HT_400_600   -i inputFiles/Files_7Feb2016/DY7FebSplit/13_DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 5.678
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DY_HT_600_800   -i inputFiles/Files_7Feb2016/DY7FebSplit/15_DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2.txt -x 1.367
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DY_HT_800_1200   -i inputFiles/Files_7Feb2016/DY7FebSplit/16_DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 0.6304
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DY_HT_1200_2500   -i inputFiles/Files_7Feb2016/DY7FebSplit/17_DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 0.1514
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DY_HT_2500_inf   -i inputFiles/Files_7Feb2016/DY7FebSplit/18_DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 0.003565


# #############################
# ##### ### VV+jets :
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/WplusToLNuWminusTo2JJJ_QCD -i inputFiles/Files_17Nov_VBF/1_WplusToLNuWminusTo2JJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 5.546
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/WplusTo2JWminusToLNuJJ_QCD -i inputFiles/Files_17Nov_VBF/2_WplusTo2JWminusToLNuJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 5.568
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/WplusToLNuWplusTo2JJJ_QCD -i inputFiles/Files_17Nov_VBF/3_WplusToLNuWplusTo2JJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 8.642e-02
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/WminusToLNuWminusTo2JJJ_QCD -i inputFiles/Files_17Nov_VBF/4_WminusToLNuWminusTo2JJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 3.774e-02
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/WplusToLNuZTo2JJJ_QCD -i inputFiles/Files_17Nov_VBF/5_WplusToLNuZTo2JJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 2.159
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/WplusTo2JZTo2LJJ_QCD -i inputFiles/Files_17Nov_VBF/6_WplusTo2JZTo2LJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 6.404e-01
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/WminusToLNuZTo2JJJ_QCD -i inputFiles/Files_17Nov_VBF/7_WminusToLNuZTo2JJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 1.302
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/WminusTo2JZTo2LJJ_QCD -i inputFiles/Files_17Nov_VBF/8_WminusTo2JZTo2LJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 3.866e-01
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/ZTo2LZTo2JJJ_QCD -i inputFiles/Files_17Nov_VBF/9_ZTo2LZTo2JJJ_QCD_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 3.756e-01
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/WplusToLNuWminusTo2JJJ_EWK -i inputFiles/Files_17Nov_VBF/10_WplusToLNuWminusTo2JJJ_EWK_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 9.114e-01
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/WplusTo2JWminusToLNuJJ_EWK -i inputFiles/Files_17Nov_VBF/11_WplusTo2JWminusToLNuJJ_EWK_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 9.107e-01
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/WplusToLNuWplusTo2JJJ_EWK -i inputFiles/Files_17Nov_VBF/12_WplusToLNuWplusTo2JJJ_EWK_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 8.793e-02
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/WminusToLNuWminusTo2JJJ -i inputFiles/Files_17Nov_VBF/13_WminusToLNuWminusTo2JJJ_EWK_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 3.259e-02
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/WplusToLNuZTo2JJJ_EWK -i inputFiles/Files_17Nov_VBF/14_WplusToLNuZTo2JJJ_EWK_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 1.825e-01
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/WplusTo2JZTo2LJJ_EWK -i inputFiles/Files_17Nov_VBF/15_WplusTo2JZTo2LJJ_EWK_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 5.401e-02
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/WminusToLNuZTo2JJJ_EWK -i inputFiles/Files_17Nov_VBF/16_WminusToLNuZTo2JJJ_EWK_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 1.000e-01
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/WminusTo2JZTo2LJJ_EWK -i inputFiles/Files_17Nov_VBF/17_WminusTo2JZTo2LJJ_EWK_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 2.982e-02
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/ZTo2LZTo2JJJ_EWK -i inputFiles/Files_17Nov_VBF/18_ZTo2LZTo2JJJ_EWK_LO_SM_MJJ100PTJ10_TuneCUETP8M1_13TeV-madgraph-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 1.589e-02 





###################
# TT
# TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
# hh = 45.7%
# ll = 10.5%
# hl = 21.9% (x2 for permutation t-tbar)
# technical note: use -q long if splitting on ~180-350 jobs

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 400  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_TT_fullyHad  -i inputFiles/Files_7Feb2016/TT_powheg_fullyHad_7Feb2017.txt    -x 380.1143 -t True -b 1
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 700 -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_TT_semiLep   -i inputFiles/Files_7Feb2016/TT_powheg_semiLep_7Feb2017.txt     -x 364.3108 -t True -b 5

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 700 -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_TT_fullyLep  -i inputFiles/Files_7Feb2016/TT_powheg_fullyLep_7Feb2017.txt    -x 87.3348  -t True -b 4



#####################
### MC VBF HH bbtt non resonant
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 100  -k False -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_VBFHH2b2T_nonres_C2V1        -i inputFiles/VBF_signals_13Nov/1_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 100  -k False -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_VBFHH2b2T_nonres_C2V2        -i inputFiles/VBF_signals_13Nov/2_VBFHHTo2B2Tau_CV_1_C2V_2_C3_1_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt

#### MC ggHH SM non resonant
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_SM_nonresonant   -i inputFiles/Files_7Feb2016/38_GluGluToHHTo2B2Tau_node_SM_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True



#####################
### TEST DATA W/ MORE JOBS FOR 0JETS SKIMS
# python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 600  -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_SingleMuon        -i inputFiles/Files_22Feb2017/SingleMu_7FebreminiAOD_5Mar2017.txt
# python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 1000 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_SingleElectron    -i inputFiles/Files_22Feb2017/SingleEle_7FebreminiAOD_5Mar2017.txt
# python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 600  -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_Tau               -i inputFiles/Files_22Feb2017/Tau_7FebreminiAOD_5Mar2017.txt



#####################
### DATA
## 15 Giu : job increased by a factor 10, because some few files went corrupted... like this I minimize the fraction of failed jobs to <1%

# python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 600  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_SingleMuon        -i inputFiles/Files_22Feb2017/SingleMu_7FebreminiAOD_5Mar2017.txt
# python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 1000 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_SingleElectron    -i inputFiles/Files_22Feb2017/SingleEle_7FebreminiAOD_5Mar2017.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 600  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_Tau               -i inputFiles/Files_22Feb2017/Tau_7FebreminiAOD_5Mar2017.txt

# # #####################
# # ### DATA PROMPT RECO FOR CONTROL
# # # python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 60  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_PromptReco_SingleMuon        -i inputFiles/Files80X_25Nov2016/SingleMu_25Nov2016BCDPromptReco.txt
# # # python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 100 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_PromptReco_SingleElectron    -i inputFiles/Files80X_25Nov2016/SingleEle_25Nov2016BCDPromptReco.txt
# # # python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 60  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_PromptReco_Tau               -i inputFiles/Files80X_25Nov2016/Tau_25Nov2016BCDPromptReco.txt


###################
# TT
# TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
# hh = 45.7%
# ll = 10.5%
# hl = 21.9% (x2 for permutation t-tbar)
# technical note: use -q long if splitting on ~180-350 jobs
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 400 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_TT_fullyHad  -i inputFiles/Files_7Feb2016/TT_powheg_fullyHad_7Feb2017.txt    -x 380.1143 -t True -b 1
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 700 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_TT_semiLep   -i inputFiles/Files_7Feb2016/TT_powheg_semiLep_7Feb2017.txt     -x 364.3108 -t True -b 5
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 700 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_TT_fullyLep  -i inputFiles/Files_7Feb2016/TT_powheg_fullyLep_7Feb2017.txt    -x 87.3348  -t True -b 4

# # ####################
# # ## DY
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_DY   -i inputFiles/Files_7Feb2016/DY_mergedList_7Feb2017.txt -g True -x 5765.4 

# # ####################
# # ## ELECTROWEAK
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50   -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_EWKWPlus2Jets_WToLNu   -i inputFiles/Files_7Feb2016/EWKWPlus2Jets_WToLNu_M-50.txt   -x 25.62
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50   -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_EWKWMinus2Jets_WToLNu  -i inputFiles/Files_7Feb2016/EWKWMinus2Jets_WToLNu_M-50.txt  -x 20.25
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50   -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_EWKZ2Jets_ZToLL        -i inputFiles/Files_7Feb2016/EWKZ2Jets_ZToLL_M-50.txt        -x 3.987

# #############################
# ##### ### W+jets : 
# ##### ## note: currently I am chopping away the part with >= 1 jet in the inclusive sample, and writing the corresponding cross section.
# ##### ## But I can increase the stat with some stitching as done in DY if needed
# ##### python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_0jet     -i inputFiles/Files80X_25Nov2016/8_WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt    -x 44149.01333 -e 0
# ##### python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_1jet     -i inputFiles/Files80X_25Nov2016/9_W1JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt   -x 11778.36381
# ##### python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_2jet     -i inputFiles/Files80X_25Nov2016/10_W2JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt  -x 3840.219744
# ##### python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_3jet     -i inputFiles/Files80X_25Nov2016/11_W3JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt  -x 1166.047866
# ##### python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_4jet     -i inputFiles/Files80X_25Nov2016/12_W4JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt  -x 593.055246

# # #############################
# # ##### ### W+jets in HT bins 
# # ## WJets for HT bins -- chopping away HT > 70 in the inclusive sample is fine, as it is only ~10% of the total number of events
# # ## tot xs NNLO: 61526.7 (+497.1 -264.6) (+/- 2312.7) pb
# # ## individual xs are taken from : https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#W_jets
# # ## and the one for 0-70 and 70-100 computed by difference and using the inclusive sample distribution
# # ## xs LO : 50690 pb
# # ## xs LO (HT < 100 ) = 50690 - sum (xs HT > 100) = 50690 - 1772.52216 = 48917.47784
# # ## then using the inlcusive sample :
# # ## ch->GetEntries("lheHt < 70")                : 186703 (97.09%) --> 47493.98 pb
# # ## ch->GetEntries("lheHt > 70 && lheHt < 100") : 5589   (2.91%)  --> 1423.499 pb
# # ## Finally, everything is scaled from the LO to the NNLO xs : (61526.7/50690.) = 1.213784
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_0_70       -i inputFiles/Files_7Feb2016/WJets_incl.txt        -y 1.213784 -x 47493.98 -z 70
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_70_100     -i inputFiles/Files_7Feb2016/WJetsHT-70-100.txt    -y 1.213784 -x 1423.499
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_100_200    -i inputFiles/Files_7Feb2016/WJetsHT-100-200.txt   -y 1.213784 -x 1345
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_200_400    -i inputFiles/Files_7Feb2016/WJetsHT-200-400.txt   -y 1.213784 -x 359.7
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_400_600    -i inputFiles/Files_7Feb2016/WJetsHT-400-600.txt   -y 1.213784 -x 48.91
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_600_800    -i inputFiles/Files_7Feb2016/WJetsHT-600-800.txt   -y 1.213784 -x 12.05
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_800_1200   -i inputFiles/Files_7Feb2016/WJetsHT-800-1200.txt  -y 1.213784 -x 5.501
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_1200_2500  -i inputFiles/Files_7Feb2016/WJetsHT-1200-2500.txt -y 1.213784 -x 1.329
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_2500_Inf   -i inputFiles/Files_7Feb2016/WJetsHT-2500-Inf.txt  -y 1.213784 -x 0.03216

# # # #####################
# # # ### DI-boson:
# # # # note2: all njobs listed here were increased by x10 to make them run on the short queue
# # # #XS taken from here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson
# # ## for Z->4Q, I scaled from ZZ->2nu2Q using BR(2Nu2Q) = 2 * 20% * 69.91%
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50   -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WWTo2L2Nu  -i inputFiles/Files_7Feb2016/40_WWTo2L2Nu_13TeV-powheg__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt   -x 12.178
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50   -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WWToLNuQQ  -i inputFiles/Files_7Feb2016/WWToLNuQQ.txt                                                                                                  -x 49.997
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50   -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WWTo4Q     -i inputFiles/Files_7Feb2016/43_WWTo4Q_13TeV-powheg__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt      -x 51.723

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50   -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ZZTo2L2Nu  -i inputFiles/Files_7Feb2016/32_ZZTo2L2Nu_13TeV_powheg_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt               -x 0.564
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 100  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ZZTo2L2Q   -i inputFiles/Files_7Feb2016/33_ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt  -x 3.22
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 100  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ZZTo4L     -i inputFiles/Files_7Feb2016/34_ZZTo4L_13TeV-amcatnloFXFX-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt       -x 1.212
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50   -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ZZTo4Q     -i inputFiles/Files_7Feb2016/37_ZZTo4Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt    -x 7.06091

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WZTo3LNu       -i inputFiles/Files_7Feb2016/31_WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt      -x 4.42965 
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WZTo1L3Nu      -i inputFiles/Files_7Feb2016/47_WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt    -x 3.033
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WZTo1L1Nu2Q    -i inputFiles/Files_7Feb2016/48_WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3.txt  -x 10.71
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 400  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WZTo2L2Q       -i inputFiles/Files_7Feb2016/49_WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt     -x 5.595

# # # #####################
# # # ### SM Higgs
# # # ### HXSWG: xs(ZH) = 0.8839 pb
# # # ### Z->qq : 69.91% , Z->ll : 3,3658% (x3 for all the leptons)
# # # ### H->bb : 57.7%  , H->tautau : 6.32%
# # # ##### ZH (Zall, Htautau) : 6.32%
# # # ##### ZH (Zll, Hbb)      : 5.83%
# # # ##### ZH (Zqq, Hbb)      : 40.34%
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 100  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ZH_HTauTau      -i inputFiles/Files_7Feb2016/50_ZHToTauTau_M125_13TeV_powheg_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt      -x 0.8839 -y 0.0632
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 100  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ZH_HBB_ZLL      -i inputFiles/Files_7Feb2016/ZH_HToBB_ZToLL.txt                                                                                                              -x 0.8839 -y 0.0583
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 100  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ZH_HBB_ZQQ      -i inputFiles/Files_7Feb2016/53_ZH_HToBB_ZToQQ_M125_13TeV_powheg_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt  -x 0.8839 -y 0.4034


# # #####################
# # ### Radion signals
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-250   -i inputFiles/Files_7Feb2016/allRadion/58_GluGluToRadionToHHTo2B2Tau_M-250_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-270   -i inputFiles/Files_7Feb2016/allRadion/59_GluGluToRadionToHHTo2B2Tau_M-270_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-280   -i inputFiles/Files_7Feb2016/allRadion/60_GluGluToRadionToHHTo2B2Tau_M-280_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-300   -i inputFiles/Files_7Feb2016/allRadion/61_GluGluToRadionToHHTo2B2Tau_M-300_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt  -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-350   -i inputFiles/Files_7Feb2016/allRadion/62_GluGluToRadionToHHTo2B2Tau_M-350_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-400   -i inputFiles/Files_7Feb2016/allRadion/63_GluGluToRadionToHHTo2B2Tau_M-400_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3.txt  -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-450   -i inputFiles/Files_7Feb2016/allRadion/64_GluGluToRadionToHHTo2B2Tau_M-450_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-500   -i inputFiles/Files_7Feb2016/allRadion/65_GluGluToRadionToHHTo2B2Tau_M-500_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-550   -i inputFiles/Files_7Feb2016/allRadion/66_GluGluToRadionToHHTo2B2Tau_M-550_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-600   -i inputFiles/Files_7Feb2016/allRadion/67_GluGluToRadionToHHTo2B2Tau_M-600_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-650   -i inputFiles/Files_7Feb2016/allRadion/68_GluGluToRadionToHHTo2B2Tau_M-650_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-750   -i inputFiles/Files_7Feb2016/allRadion/69_GluGluToRadionToHHTo2B2Tau_M-750_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-900   -i inputFiles/Files_7Feb2016/allRadion/70_GluGluToRadionToHHTo2B2Tau_M-900_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True

# #####################
# ### Graviton signals
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-250   -i inputFiles/Files_29Apr2016_gravitons/1_GluGluToBulkGravitonToHHTo2B2Tau_M-250_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-260   -i inputFiles/Files_29Apr2016_gravitons/2_GluGluToBulkGravitonToHHTo2B2Tau_M-260_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-270   -i inputFiles/Files_29Apr2016_gravitons/3_GluGluToBulkGravitonToHHTo2B2Tau_M-270_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-280   -i inputFiles/Files_29Apr2016_gravitons/4_GluGluToBulkGravitonToHHTo2B2Tau_M-280_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-300   -i inputFiles/Files_29Apr2016_gravitons/5_GluGluToBulkGravitonToHHTo2B2Tau_M-300_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-320   -i inputFiles/Files_29Apr2016_gravitons/6_GluGluToBulkGravitonToHHTo2B2Tau_M-320_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-340   -i inputFiles/Files_29Apr2016_gravitons/7_GluGluToBulkGravitonToHHTo2B2Tau_M-340_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-350   -i inputFiles/Files_29Apr2016_gravitons/8_GluGluToBulkGravitonToHHTo2B2Tau_M-350_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-400   -i inputFiles/Files_29Apr2016_gravitons/9_GluGluToBulkGravitonToHHTo2B2Tau_M-400_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-450   -i inputFiles/Files_29Apr2016_gravitons/10_GluGluToBulkGravitonToHHTo2B2Tau_M-450_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-500   -i inputFiles/Files_29Apr2016_gravitons/11_GluGluToBulkGravitonToHHTo2B2Tau_M-500_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-550   -i inputFiles/Files_29Apr2016_gravitons/12_GluGluToBulkGravitonToHHTo2B2Tau_M-550_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-600   -i inputFiles/Files_29Apr2016_gravitons/13_GluGluToBulkGravitonToHHTo2B2Tau_M-600_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-650   -i inputFiles/Files_29Apr2016_gravitons/14_GluGluToBulkGravitonToHHTo2B2Tau_M-650_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-750   -i inputFiles/Files_29Apr2016_gravitons/15_GluGluToBulkGravitonToHHTo2B2Tau_M-750_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-800   -i inputFiles/Files_29Apr2016_gravitons/16_GluGluToBulkGravitonToHHTo2B2Tau_M-800_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRSGravitonToHHTo2B2Tau_M-300   -i inputFiles/Files_29Apr2016_gravitons/1_GluGluToRSGravitonToHHTo2B2Tau_M-300_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRSGravitonToHHTo2B2Tau_M-650   -i inputFiles/Files_29Apr2016_gravitons/2_GluGluToRSGravitonToHHTo2B2Tau_M-650_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRSGravitonToHHTo2B2Tau_M-900   -i inputFiles/Files_29Apr2016_gravitons/3_GluGluToRSGravitonToHHTo2B2Tau_M-900_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True



#### HH dyn rew 12 benchmarks
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_benchmark_1   -i inputFiles/Files_7Feb2016/allNonResClonesBench/filelist_bench_1.txt -x 1.0    --kl  7.5  --kt 1.0 --c2 -1.0 --cg  0.0 --c2g  0.0 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_benchmark_2   -i inputFiles/Files_7Feb2016/allNonResClonesBench/filelist_bench_2.txt -x 1.0    --kl  1.0  --kt 1.0 --c2  0.5 --cg -0.8 --c2g  0.6 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_benchmark_3   -i inputFiles/Files_7Feb2016/allNonResClonesBench/filelist_bench_3.txt -x 1.0    --kl  1.0  --kt 1.0 --c2 -1.5 --cg  0.0 --c2g -0.8 -a True 
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_benchmark_4   -i inputFiles/Files_7Feb2016/allNonResClonesBench/filelist_bench_4.txt -x 1.0    --kl -3.5  --kt 1.5 --c2 -3.0 --cg  0.0 --c2g  0.0 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_benchmark_5   -i inputFiles/Files_7Feb2016/allNonResClonesBench/filelist_bench_5.txt -x 1.0    --kl  1.0  --kt 1.0 --c2  0.0 --cg  0.8 --c2g -1.0 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_benchmark_6   -i inputFiles/Files_7Feb2016/allNonResClonesBench/filelist_bench_6.txt -x 1.0    --kl  2.4  --kt 1.0 --c2  0.0 --cg  0.2 --c2g -0.2 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_benchmark_7   -i inputFiles/Files_7Feb2016/allNonResClonesBench/filelist_bench_7.txt -x 1.0    --kl  5.0  --kt 1.0 --c2  0.0 --cg  0.2 --c2g -0.2 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_benchmark_8   -i inputFiles/Files_7Feb2016/allNonResClonesBench/filelist_bench_8.txt -x 1.0    --kl  15.0 --kt 1.0 --c2  0.0 --cg -1.0 --c2g  1.0 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_benchmark_9   -i inputFiles/Files_7Feb2016/allNonResClonesBench/filelist_bench_9.txt -x 1.0    --kl  1.0  --kt 1.0 --c2  1.0 --cg -0.6 --c2g  0.6 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_benchmark_10  -i inputFiles/Files_7Feb2016/allNonResClonesBench/filelist_bench_10.txt -x 1.0   --kl 10.0  --kt 1.5 --c2 -1.0 --cg  0.0 --c2g  0.0 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_benchmark_11  -i inputFiles/Files_7Feb2016/allNonResClonesBench/filelist_bench_11.txt -x 1.0   --kl  2.4  --kt 1.0 --c2  0.0 --cg  1.0 --c2g -1.0 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_benchmark_12  -i inputFiles/Files_7Feb2016/allNonResClonesBench/filelist_bench_12.txt -x 1.0   --kl 15.0  --kt 1.0 --c2  1.0 --cg  0.0 --c2g  0.0 -a True 



# # # ## HH din reweight
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_0   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_0.txt    -x 1.0 --kl -20     --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_1   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_1.txt    -x 1.0 --kl -19     --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_2   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_2.txt    -x 1.0 --kl -18     --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_3   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_3.txt    -x 1.0 --kl -17     --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_4   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_4.txt    -x 1.0 --kl -16     --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_5   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_5.txt    -x 1.0 --kl -15     --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_6   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_6.txt    -x 1.0 --kl -14     --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_7   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_7.txt    -x 1.0 --kl -13     --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_8   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_8.txt    -x 1.0 --kl -12     --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_9   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_9.txt    -x 1.0 --kl -11     --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_10   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_10.txt  -x 1.0 --kl -10     --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_11   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_11.txt  -x 1.0 --kl -9      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_12   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_12.txt  -x 1.0 --kl -8      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_13   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_13.txt  -x 1.0 --kl -7      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_14   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_14.txt  -x 1.0 --kl -6      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_15   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_15.txt  -x 1.0 --kl -5      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_16   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_16.txt  -x 1.0 --kl -4      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_17   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_17.txt  -x 1.0 --kl -3      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_18   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_18.txt  -x 1.0 --kl -2      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_19   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_19.txt  -x 1.0 --kl -1      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_20   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_20.txt  -x 1.0 --kl 0.0001  --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_21   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_21.txt  -x 1.0 --kl 1       --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_22   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_22.txt  -x 1.0 --kl 2       --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_23   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_23.txt  -x 1.0 --kl 3       --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_24   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_24.txt  -x 1.0 --kl 4       --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_25   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_25.txt  -x 1.0 --kl 5       --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_26   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_26.txt  -x 1.0 --kl 6       --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_27   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_27.txt  -x 1.0 --kl 7       --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_28   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_28.txt  -x 1.0 --kl 8       --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_29   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_29.txt  -x 1.0 --kl 9       --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_30   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_30.txt  -x 1.0 --kl 10      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_31   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_31.txt  -x 1.0 --kl 11      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_32   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_32.txt  -x 1.0 --kl 12      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_33   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_33.txt  -x 1.0 --kl 13      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_34   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_34.txt  -x 1.0 --kl 14      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_35   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_35.txt  -x 1.0 --kl 15      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_36   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_36.txt  -x 1.0 --kl 16      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_37   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_37.txt  -x 1.0 --kl 17      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_38   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_38.txt  -x 1.0 --kl 18      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_39   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_39.txt  -x 1.0 --kl 19      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_40   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_40.txt  -x 1.0 --kl 20      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_41   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_41.txt  -x 1.0 --kl 21      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_42   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_42.txt  -x 1.0 --kl 22      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_43   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_43.txt  -x 1.0 --kl 23      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_44   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_44.txt  -x 1.0 --kl 24      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_45   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_45.txt  -x 1.0 --kl 25      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_46   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_46.txt  -x 1.0 --kl 26      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_47   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_47.txt  -x 1.0 --kl 27      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_48   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_48.txt  -x 1.0 --kl 28      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_49   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_49.txt  -x 1.0 --kl 29      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_50   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_50.txt  -x 1.0 --kl 30      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_51   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_51.txt  -x 1.0 --kl 31      --kt 1.0 -a True

## extra pts for a finer 1D plot
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99900   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99900.txt  -x 1.0 --kl 0.0      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99901   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99901.txt  -x 1.0 --kl 0.1      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99902   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99902.txt  -x 1.0 --kl 0.2      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99903   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99903.txt  -x 1.0 --kl 0.3      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99904   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99904.txt  -x 1.0 --kl 0.4      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99905   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99905.txt  -x 1.0 --kl 0.5      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99906   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99906.txt  -x 1.0 --kl 0.6      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99907   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99907.txt  -x 1.0 --kl 0.7      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99908   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99908.txt  -x 1.0 --kl 0.8      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99909   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99909.txt  -x 1.0 --kl 0.9      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99910   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99910.txt  -x 1.0 --kl 1.0      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99911   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99911.txt  -x 1.0 --kl 1.1      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99912   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99912.txt  -x 1.0 --kl 1.2      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99913   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99913.txt  -x 1.0 --kl 1.3      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99914   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99914.txt  -x 1.0 --kl 1.4      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99915   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99915.txt  -x 1.0 --kl 1.5      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99916   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99916.txt  -x 1.0 --kl 1.6      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99917   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99917.txt  -x 1.0 --kl 1.7      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99918   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99918.txt  -x 1.0 --kl 1.8      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99919   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99919.txt  -x 1.0 --kl 1.9      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99920   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99920.txt  -x 1.0 --kl 2.0      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99921   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99921.txt  -x 1.0 --kl 2.1      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99922   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99922.txt  -x 1.0 --kl 2.2      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99923   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99923.txt  -x 1.0 --kl 2.3      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99924   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99924.txt  -x 1.0 --kl 2.4      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99925   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99925.txt  -x 1.0 --kl 2.5      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99926   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99926.txt  -x 1.0 --kl 2.6      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99927   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99927.txt  -x 1.0 --kl 2.7      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99928   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99928.txt  -x 1.0 --kl 2.8      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99929   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99929.txt  -x 1.0 --kl 2.9      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99930   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99930.txt  -x 1.0 --kl 3.0      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99931   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99931.txt  -x 1.0 --kl 3.1      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99932   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99932.txt  -x 1.0 --kl 3.2      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99933   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99933.txt  -x 1.0 --kl 3.3      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99934   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99934.txt  -x 1.0 --kl 3.4      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99935   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99935.txt  -x 1.0 --kl 3.5      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99936   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99936.txt  -x 1.0 --kl 3.6      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99937   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99937.txt  -x 1.0 --kl 3.7      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99938   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99938.txt  -x 1.0 --kl 3.8      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99939   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99939.txt  -x 1.0 --kl 3.9      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99940   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99940.txt  -x 1.0 --kl 4.0      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99941   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99941.txt  -x 1.0 --kl 4.1      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99942   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99942.txt  -x 1.0 --kl 4.2      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99943   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99943.txt  -x 1.0 --kl 4.3      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99944   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99944.txt  -x 1.0 --kl 4.4      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99945   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99945.txt  -x 1.0 --kl 4.5      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99946   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99946.txt  -x 1.0 --kl 4.6      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99947   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99947.txt  -x 1.0 --kl 4.7      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99948   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99948.txt  -x 1.0 --kl 4.8      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99949   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99949.txt  -x 1.0 --kl 4.9      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99950   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99950.txt  -x 1.0 --kl 5.0      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99951   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99951.txt  -x 1.0 --kl 5.1      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99952   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99952.txt  -x 1.0 --kl 5.2      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99953   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99953.txt  -x 1.0 --kl 5.3      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99954   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99954.txt  -x 1.0 --kl 5.4      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99955   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99955.txt  -x 1.0 --kl 5.5      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99956   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99956.txt  -x 1.0 --kl 5.6      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99957   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99957.txt  -x 1.0 --kl 5.7      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99958   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99958.txt  -x 1.0 --kl 5.8      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99959   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99959.txt  -x 1.0 --kl 5.9      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99960   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99960.txt  -x 1.0 --kl 6.0      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99961   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99961.txt  -x 1.0 --kl 6.1      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99962   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99962.txt  -x 1.0 --kl 6.2      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99963   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99963.txt  -x 1.0 --kl 6.3      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99964   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99964.txt  -x 1.0 --kl 6.4      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99965   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99965.txt  -x 1.0 --kl 6.5      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99966   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99966.txt  -x 1.0 --kl 6.6      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99967   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99967.txt  -x 1.0 --kl 6.7      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99968   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99968.txt  -x 1.0 --kl 6.8      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99969   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99969.txt  -x 1.0 --kl 6.9      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99970   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99970.txt  -x 1.0 --kl 7.0      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99971   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99971.txt  -x 1.0 --kl 7.1      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99972   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99972.txt  -x 1.0 --kl 7.2      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99973   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99973.txt  -x 1.0 --kl 7.3      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99974   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99974.txt  -x 1.0 --kl 7.4      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99975   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99975.txt  -x 1.0 --kl 7.5      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99976   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99976.txt  -x 1.0 --kl 7.6      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99977   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99977.txt  -x 1.0 --kl 7.7      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99978   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99978.txt  -x 1.0 --kl 7.8      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99979   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99979.txt  -x 1.0 --kl 7.9      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_99980   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99980.txt  -x 1.0 --kl 8.0      --kt 1.0 -a True



# # ## HH with reweighting --  a lot of files!
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_0   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_0.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_0.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_1   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_1.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_1.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_2   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_2.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_2.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_3   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_3.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_3.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_4   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_4.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_4.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_5   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_5.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_5.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_6   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_6.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_6.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_7   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_7.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_7.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_8   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_8.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_8.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_9   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_9.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_9.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_10   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_10.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_10.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_11   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_11.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_11.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_12   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_12.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_12.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_13   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_13.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_13.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_14   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_14.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_14.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_15   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_15.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_15.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_16   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_16.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_16.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_17   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_17.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_17.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_18   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_18.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_18.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_19   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_19.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_19.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_20   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_20.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_20.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_21   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_21.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_21.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_22   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_22.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_22.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_23   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_23.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_23.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_24   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_24.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_24.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_25   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_25.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_25.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_26   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_26.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_26.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_27   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_27.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_27.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_28   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_28.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_28.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_29   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_29.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_29.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_30   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_30.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_30.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_31   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_31.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_31.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_32   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_32.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_32.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_33   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_33.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_33.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_34   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_34.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_34.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_35   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_35.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_35.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_36   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_36.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_36.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_37   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_37.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_37.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_38   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_38.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_38.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_39   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_39.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_39.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_40   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_40.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_40.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_41   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_41.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_41.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_42   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_42.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_42.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_43   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_43.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_43.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_44   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_44.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_44.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_45   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_45.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_45.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_46   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_46.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_46.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_47   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_47.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_47.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_48   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_48.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_48.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_49   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_49.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_49.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_50   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_50.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_50.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_51   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_51.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_51.root -a True


# ########
# ## SOME TESTS OF HH DYNAMIC REWEIGHT - IT WORKS!

# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_node_SM   -i inputFiles/Files_7Feb2016/38_GluGluToHHTo2B2Tau_node_SM_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt  -x 1.0 -a True

# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_dyn_21   -i inputFiles/Files_7Feb2016/allNonResClones_dynRew/filelist_allNonRes_dr_21.txt  -x 1.0 --kl 1.0  --kt 1.0 -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_dyn_41   -i inputFiles/Files_7Feb2016/allNonResClones_dynRew/filelist_allNonRes_dr_41.txt  -x 1.0 --kl 20.0 --kt 1.0 -a True


