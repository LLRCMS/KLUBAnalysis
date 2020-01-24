AMESSAGE="First 2018 Legacy ntuples"

OUTDIRR="SKIMS_22Jan2020"

INPUTDIR_BKG="inputFiles/Legacy2018_backgrounds"
INPUTDIR_DATA="inputFiles/Legacy2018_data"
INPUTDIR_SIG="inputFiles/Legacy2018_signals"
#INPUTDIR="inputFiles/MC2017_Jan2019"
PUDIR="weights/PUreweight/outputs_January2019"
SKIMDIR="/data_CMS/cms/amendola/HHLegacy_2018_SKIMS"

source scripts/setup.sh
source /opt/exp_soft/cms/t3/t3setup
mkdir -p $SKIMDIR/$OUTDIRR/
#mkdir $OUTDIRR
touch $SKIMDIR/$OUTDIRR/README.txt
echo $AMESSAGE > $SKIMDIR/$OUTDIRR/README.txt
cp /home/llr/cms/amendola/CMSSW_7_4_7/src/KLUBAnalysis/scripts/listAll.sh $SKIMDIR/$OUTDIRR/



#########################
### SKIMS LEGACY 2018 ###
#########################

#DATA->OK

#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_SingleMuon2017A   -k True     -i $INPUTDIR_DATA/4_SingleMuon__Run2018A-17Sep2018-v2.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_SingleMuon2017B   -k True     -i $INPUTDIR_DATA/5_SingleMuon__Run2018B-17Sep2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_SingleMuon2017C   -k True     -i $INPUTDIR_DATA/6_SingleMuon__Run2018C-17Sep2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_SingleMuon2017D   -k True     -i $INPUTDIR_DATA/2_SingleMuon__Run2018D-PromptReco-v2.txt

#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_SingleElectron2017A   -k True     -i $INPUTDIR_DATA/1_EGamma__Run2018A-17Sep2018-v2.txt 
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_SingleElectron2017B   -k True     -i $INPUTDIR_DATA/2_EGamma__Run2018B-17Sep2018-v1.txt 
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_SingleElectron2017C   -k True     -i $INPUTDIR_DATA/3_EGamma__Run2018C-17Sep2018-v1.txt 
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_SingleElectron2017D   -k True     -i $INPUTDIR_DATA/1_EGamma__Run2018D-PromptReco-v2.txt

#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_Tau2017A   -k True     -i $INPUTDIR_DATA/7_Tau__Run2018A-17Sep2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_Tau2017B   -k True     -i $INPUTDIR_DATA/8_Tau__Run2018B-17Sep2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_Tau2017C   -k True     -i $INPUTDIR_DATA/9_Tau__Run2018C-17Sep2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_Tau2017D   -k True     -i $INPUTDIR_DATA/3_Tau__Run2018D-PromptReco-v2.txt

##### TT->OK
##### TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
##### hh = 45.7%
##### ll = 10.5%
##### hl = 21.9% (x2 for permutation t-tbar)
####

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 700  -Y 2018 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_fullyHad -i $INPUTDIR_BKG/2_TTToHadronic_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 377.96 -t True -b 1  

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 1000  -Y 2018 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_fullyLep -i $INPUTDIR_BKG/1_TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 88.29  -t True -b 4 

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 1000  -Y 2018 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_semiLep -i $INPUTDIR_BKG/1_TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 365.34 -t True -b 5 

### DY 
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_Legacy2018.cfg  -n 200 -k True -n 1000  -Y 2018 -k True -o $SKIMDIR/$OUTDIRR/SKIM_DY -i $INPUTDIR_BKG/DYmerged.txt -x 6225.42 -g True


### # #####################
### Wjets ->OK
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 200  -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_0_100     -i $INPUTDIR_BKG/4_WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt    -y 1.213784 -x 48917.48 -z 100   --tag WJetsToLNu_HT-0To100.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 200  -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_100_200   -i $INPUTDIR_BKG/5_WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 200  -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_200_400   -i $INPUTDIR_BKG/6_WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 200  -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_400_600   -i $INPUTDIR_BKG/7_WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 200  -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_600_800   -i $INPUTDIR_BKG/8_WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 200  -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_800_1200  -i $INPUTDIR_BKG/9_WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 200  -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_1200_2500 -i $INPUTDIR_BKG/10_WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 200  -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_2500_Inf  -i $INPUTDIR_BKG/11_WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt

#  
#
## # #####################
##### ELECTROWEAK -> OK
#
#
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_EWKWPlus2Jets_WToLNu   -x 25.62 -i $INPUTDIR_BKG/27_EWKWPlus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_EWKWMinus2Jets_WToLNu -x 20.25 -i $INPUTDIR_BKG/28_EWKWMinus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_EWKZ2Jets_ZToLL       -x 3.987 -i $INPUTDIR_BKG/25_EWKZ2Jets_ZToLL_M-50_TuneCP5_PSweights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_EWKZ2Jets_ZToNuNu    -x 10.66  -i $INPUTDIR_BKG/26_EWKZ2Jets_ZToNuNu_TuneCP5_PSweights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
#
#
#
#
# # #####################
##### single top : ->OK
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 120  -o $SKIMDIR/$OUTDIRR/SKIM_ST_tW_antitop         -i $INPUTDIR_BKG/21_ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1.txt   -x 34.97 
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 120  -o $SKIMDIR/$OUTDIRR/SKIM_ST_tW_top             -i $INPUTDIR_BKG/22_ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1.txt       -x 35.6  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 120  -o $SKIMDIR/$OUTDIRR/SKIM_ST_tchannel_antitop   -i $INPUTDIR_BKG/23_ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 80.95
# Missing ### python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ST_tchannel_top    -x 136.02   -i 


## # #####################
##### SM Higgs : -> OK
#### HXSWG: xs(ZH) = 0.8839 pb
#### Z->qq : 69.91% , Z->ll : 3,3658% (x3 for all the leptons)
#### H->bb : 57.7%  , H->tautau : 6.32%
#### ZH (Zll, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
#### ZH (Zqq, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
#### ZH (Zall, Htautau) : XS teor ZH * BR H->tautau
#
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ZH_HBB_ZLL -i $INPUTDIR_BKG/29_ZH_HToBB_ZToLL_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt -x 0.07924 -y 0.577  
# ggZH?
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ZH_HTauTau -i $INPUTDIR_BKG/32_ZHToTauTau_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt -x 0.8696 -y 0.0632  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ggHTauTau -i $INPUTDIR_BKG/33_GluGluHToTauTau_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt -x 21.46 -y 0.0632  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_WplusHTauTau -i $INPUTDIR_BKG/35_WplusHToTauTau_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt -x 0.867 -y 0.0632  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 50  -o $SKIMDIR/$OUTDIRR/SKIM_VBFHTauTau -i $INPUTDIR_BKG/34_VBFHToTauTau_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1.txt -x 3.861 -y 0.0632 
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -Y 2018 -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_WminusHTauTau -i $INPUTDIR_BKG/36_WminusHToTauTau_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt -x 0.5412 -y 0.0632 
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -Y 2018 -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ttHJetTononBB -i $INPUTDIR_BKG/37_ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 0.507 -y 0.423  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -Y 2018 -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ttHJetToBB -i $INPUTDIR_BKG/38_ttHJetTobb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 0.507 -y 0.577  

### # #####################
###### Others : -> OK
##

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 100 -Y 2018  -o $SKIMDIR/$OUTDIRR/SKIM_TTWJetsToLNu -i $INPUTDIR_BKG/43_TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2.txt -x 0.2043  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 100 -Y 2018  -o $SKIMDIR/$OUTDIRR/SKIM_TTWJetsToQQ -i $INPUTDIR_BKG/42_TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 0.4062  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 100 -Y 2018  -o $SKIMDIR/$OUTDIRR/SKIM_TTWW -i $INPUTDIR_BKG/44_TTWW_TuneCP5_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext2-v1.txt -x 0.007834 
#missing #python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 100 -Y 2018  -o $SKIMDIR/$OUTDIRR/SKIM_TTWZ -i $INPUTDIR_BKG/ -x 0.002441  
#missing #python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 100 -Y 2018  -o $SKIMDIR/$OUTDIRR/SKIM_TTZZ -i $INPUTDIR_BKG/ -x 0.001386  




#
## # ##################### 
## # ### DI-boson: - 7*7 + 6*5 = 49 + 30 = 79 -> OK
## # # note2: all njobs listed here were increased by x10 to make them run on the short queue
## # #XS taken from here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson
## ## for Z->4Q, I scaled from ZZ->2nu2Q using BR(2Nu2Q) = 2 * 20% * 69.91%
#
#missing #python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 100    -o $SKIMDIR/$OUTDIRR/SKIM_ZZZ         -i $INPUTDIR_BKG/ -x 0.01398  

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100 -Y 2018    -o $SKIMDIR/$OUTDIRR/SKIM_WWW                 -i $INPUTDIR_BKG/39_WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2.txt -x 0.2086   
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100 -Y 2018    -o $SKIMDIR/$OUTDIRR/SKIM_WWZ                 -i $INPUTDIR_BKG/41_WWZ_TuneCP5_13TeV-amcatnlo-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2.txt -x 0.1651  
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100 -Y 2018    -o $SKIMDIR/$OUTDIRR/SKIM_WW                  -i $INPUTDIR_BKG/18_WW_TuneCP5_13TeV-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt -x 64.28   
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100 -Y 2018    -o $SKIMDIR/$OUTDIRR/SKIM_WZ                  -i $INPUTDIR_BKG/19_WZ_TuneCP5_13TeV-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v3.txt -x 23.48   
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100 -Y 2018    -o $SKIMDIR/$OUTDIRR/SKIM_WZZ                 -i $INPUTDIR_BKG/40_WZZ_TuneCP5_13TeV-amcatnlo-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2.txt -x 0.05565 
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100 -Y 2018    -o $SKIMDIR/$OUTDIRR/SKIM_ZZ                  -i $INPUTDIR_BKG/20_ZZ_TuneCP5_13TeV-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt -x 10.16  


# signals ->outdated
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_VBFSM -i inputFiles/Files_January2019_signals/46_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.001626 -a True --pu $PUDIR/PUsafe_weights.txt
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_GGHSM -i inputFiles/Files_January2019_signals/39_GluGluToHHTo2B2Tau_node_SM_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.03349 -a True --pu $PUDIR/PUsafe_weights.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_VBF_private -i inputFiles/Files_January2019_signals/PrivateProduction/VBFHHTo2B2Taus_SM.txt -x 0.001626 -a True --pu $PUDIR/PUsafe_weights.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_GGHSM_private -i inputFiles/Files_January2019_signals/PrivateProduction/GluGluToHHTo2B2Taus_12jets_SM.txt -x 0.03349 -a True --pu $PUDIR/PUsafe_weights.txt






##################
### SKIMS 2017 ###
##################
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o $SKIMDIR/$OUTDIRR/SKIM_SingleMuon2017B   -k True     -i inputFiles/Data2017_Jan2019/1_SingleMuon__Run2017B-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o $SKIMDIR/$OUTDIRR/SKIM_SingleMuon2017C    -k True     -i inputFiles/Data2017_Jan2019/2_SingleMuon__Run2017C-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o $SKIMDIR/$OUTDIRR/SKIM_SingleMuon2017D     -k True    -i inputFiles/Data2017_Jan2019/3_SingleMuon__Run2017D-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o $SKIMDIR/$OUTDIRR/SKIM_SingleMuon2017E     -k True    -i inputFiles/Data2017_Jan2019/4_SingleMuon__Run2017E-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o $SKIMDIR/$OUTDIRR/SKIM_SingleMuon2017F    -k True     -i inputFiles/Data2017_Jan2019/5_SingleMuon__Run2017F-31Mar2018-v1.txt
##sleep 45m
#
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o $SKIMDIR/$OUTDIRR/SKIM_SingleElectron2017B     -k True    -i inputFiles/Data2017_Jan2019/1_SingleElectron__Run2017B-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o $SKIMDIR/$OUTDIRR/SKIM_SingleElectron2017C    -k True     -i inputFiles/Data2017_Jan2019/2_SingleElectron__Run2017C-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o $SKIMDIR/$OUTDIRR/SKIM_SingleElectron2017D    -k True     -i inputFiles/Data2017_Jan2019/3_SingleElectron__Run2017D-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o $SKIMDIR/$OUTDIRR/SKIM_SingleElectron2017E    -k True     -i inputFiles/Data2017_Jan2019/4_SingleElectron__Run2017E-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o $SKIMDIR/$OUTDIRR/SKIM_SingleElectron2017F     -k True    -i inputFiles/Data2017_Jan2019/5_SingleElectron__Run2017F-31Mar2018-v1.txt
##sleep 30m
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o $SKIMDIR/$OUTDIRR/SKIM_Tau2017B    -k True     -i inputFiles/Data2017_Jan2019/1_Tau__Run2017B-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o $SKIMDIR/$OUTDIRR/SKIM_Tau2017C     -k True    -i inputFiles/Data2017_Jan2019/2_Tau__Run2017C-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o $SKIMDIR/$OUTDIRR/SKIM_Tau2017D     -k True    -i inputFiles/Data2017_Jan2019/3_Tau__Run2017D-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o $SKIMDIR/$OUTDIRR/SKIM_Tau2017E     -k True    -i inputFiles/Data2017_Jan2019/4_Tau__Run2017E-31Mar2018-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg  -n 200   -o $SKIMDIR/$OUTDIRR/SKIM_Tau2017F    -k True     -i inputFiles/Data2017_Jan2019/5_Tau__Run2017F-31Mar2018-v1.txt
#
##sleep 45m
#
####
####
##sleep 45m
##### TT
##### TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
##### hh = 45.7%
##### ll = 10.5%
##### hl = 21.9% (x2 for permutation t-tbar)
####
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 700   -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_fullyHad -i $INPUTDIR/TTToHadronic.txt -x 377.96 -t True -b 1  --pu $PUDIR/PUsafe_weights.txt
##sleep 20m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 1000  -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_fullyLep -i $INPUTDIR/TTTo2L2Nu.txt -x 88.29  -t True -b 4 --pu $PUDIR/PUsafe_weights.txt
##sleep 20m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 1000  -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_semiLep -i $INPUTDIR/TTToSemiLeptonic.txt -x 365.34 -t True -b 5  --pu $PUDIR/TT_semiLep.txt
##sleep 40m
###sleep 1h
####
#####
#####
####
####### DY
####
##### total XS= 6225.42 pb - 5 march 2019
##### myPUHisto_0j0b : xsFrac: 0.746231    - xs: 4645.60 pb
##### myPUHisto_1j0b : xsFrac: 0.157487    - xs: 980.42  pb
##### myPUHisto_1j1b : xsFrac: 0.00659452  - xs: 41.05   pb
##### myPUHisto_2j0b : xsFrac: 0.0532405   - xs: 331.44  pb
##### myPUHisto_2j1b : xsFrac: 0.0027623   - xs: 17.20   pb
##### myPUHisto_2j2b : xsFrac: 0.00114651  - xs: 7.14    pb
##### myPUHisto_3j0b : xsFrac: 0.0190577   - xs: 118.64  pb
##### myPUHisto_3j1b : xsFrac: 0.000788278 - xs: 4.91    pb
##### myPUHisto_3j2b : xsFrac: 0.000889527 - xs: 5.54    pb
##### myPUHisto_3j3b : xsFrac: 9.96244E-06 - xs: 0.062   pb
##### myPUHisto_4j0b : xsFrac: 0.010611    - xs: 66.06   pb
##### myPUHisto_4j1b : xsFrac: 0.000359969 - xs: 2.24    pb
##### myPUHisto_4j2b : xsFrac: 0.000809156 - xs: 5.04    pb
##### myPUHisto_4j3b : xsFrac: 1.0296E-05  - xs: 0.064   pb
##### myPUHisto_4j4b : xsFrac: 2.77162E-06 - xs: 0.017   pb
####
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_DYJets_0j0b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 4645.60 --pu $PUDIR/DY_special/DY_LO_0j0b.txt --nj 0 --nb 0 --DY True --tag $OUTDIRR/SKIM_DYJets_0b0j
##sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_DYJets_1j0b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 980.42  --pu $PUDIR/DY_special/DY_LO_1j0b.txt --nj 1 --nb 0 --DY True --tag $OUTDIRR/SKIM_DYJets_1b0j
##sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_DYJets_1j1b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 41.05   --pu $PUDIR/DY_special/DY_LO_1j1b.txt --nj 1 --nb 1 --DY True --tag $OUTDIRR/SKIM_DYJets_1b1j
#sleep 25m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 800  -o $SKIMDIR/$OUTDIRR/SKIM_DYJets_2j0b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 331.44  --pu $PUDIR/DY_special/DY_LO_2j0b.txt --nj 2 --nb 0 --DY True --tag $OUTDIRR/SKIM_DYJets_2b0j
#sleep 25m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_DYJets_2j1b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 17.20  --pu $PUDIR/DY_special/DY_LO_2j1b.txt --nj 2 --nb 1 --DY True --tag $OUTDIRR/SKIM_DYJets_2b1j
#sleep 25m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_DYJets_2j2b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 7.14    --pu $PUDIR/DY_special/DY_LO_2j2b.txt --nj 2 --nb 2 --DY True --tag $OUTDIRR/SKIM_DYJets_2b2j
#sleep 25m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_DYJets_3j0b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 118.64  --pu $PUDIR/DY_special/DY_LO_3j0b.txt --nj 3 --nb 0 --DY True --tag $OUTDIRR/SKIM_DYJets_3b0j
#sleep 25m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_DYJets_3j1b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 4.91    --pu $PUDIR/DY_special/DY_LO_3j1b.txt --nj 3 --nb 1 --DY True --tag $OUTDIRR/SKIM_DYJets_3b1j
#sleep 25m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_DYJets_3j2b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 5.54    --pu $PUDIR/DY_special/DY_LO_3j2b.txt --nj 3 --nb 2 --DY True --tag $OUTDIRR/SKIM_DYJets_3b2j
#sleep 25m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_DYJets_3j3b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 0.062   --pu $PUDIR/DY_special/DY_LO_3j3b.txt --nj 3 --nb 3 --DY True --tag $OUTDIRR/SKIM_DYJets_3b3j
#
#sleep 25m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_DYJets_4j0b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 66.06   --pu $PUDIR/DY_special/DY_LO_4j0b.txt --nj 4 --nb 0 --DY True --tag $OUTDIRR/SKIM_DYJets_4b0j 
#sleep 25m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_DYJets_4j1b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 2.24   --pu $PUDIR/DY_special/DY_LO_4j1b.txt --nj 4 --nb 1 --DY True --tag $OUTDIRR/SKIM_DYJets_4b1j 
#sleep 25m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_DYJets_4j2b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 5.04    --pu $PUDIR/DY_special/DY_LO_4j2b.txt --nj 4 --nb 2 --DY True --tag $OUTDIRR/SKIM_DYJets_4b2j
#sleep 25m
#
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_DYJets_4j3b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 0.064   --pu $PUDIR/DY_special/DY_LO_4j3b.txt --nj 4 --nb 3 --DY True --tag $OUTDIRR/SKIM_DYJets_4b3j
#sleep 25m
#
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_DYJets_4j4b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 0.017   --pu $PUDIR/DY_special/DY_LO_4j4b.txt --nj 4 --nb 4 --DY True --tag $OUTDIRR/SKIM_DYJets_4b4j
#sleep 10m
##
#
## ########
## # DY NLO
## #
#
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_DY_NLO -i $INPUTDIR/DYJetsToLL_NLO.txt  -x  6225.42  --pu $PUDIR/DYJetsToLL_NLO.txt  --DY True --tag $OUTDIRR/SKIM_DYJets_4b4j
#
###python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 200  -o /data_CMS/cms/amendola/HH2017Skims/$OUTDIRR/SKIM_DY_QQ -i inputFiles/MC2017_Jan2019/1_DYJetsToQQ_HT180_13TeV_TuneCP5-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x  1728  --pu $PUDIR/WJets.txt #--DY True
#
#
#
#
#
### # #####################
### Wjets
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_0_100     -i $INPUTDIR/WJetsToLNu_HT-0To100.txt    -y 1.213784 -x 48917.48 -z 100  --pu $PUDIR/PUsafe_weights.txt  
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_100_200   -i $INPUTDIR/31_WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt  -y 1.213784 -x 1345  --pu $PUDIR/PUsafe_weights.txt 
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_200_400   -i $INPUTDIR/32_WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -y 1.213784 -x 359.7  --pu $PUDIR/PUsafe_weights.txt 
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_400_600   -i $INPUTDIR/33_WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -y 1.213784 -x 48.91  --pu $PUDIR/PUsafe_weights.txt 
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_600_800   -i $INPUTDIR/34_WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -y 1.213784 -x 12.05  --pu $PUDIR/PUsafe_weights.txt 
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_800_1200  -i $INPUTDIR/35_WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt  -y 1.213784 -x 5.501  --pu $PUDIR/PUsafe_weights.txt 
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_1200_2500 -i $INPUTDIR/36_WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt  -y 1.213784 -x 1.329  --pu $PUDIR/PUsafe_weights.txt 
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_2500_Inf  -i $INPUTDIR/37_WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3.txt  -y 1.213784 -x 0.03216 -Z 2500  --pu $PUDIR/PUsafe_weights.txt
#sleep 10m
#  
#
## # #####################
##### ELECTROWEAK
#
#
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_EWKWPlus2Jets_WToLNu   -i $INPUTDIR/13_EWKWPlus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -x 25.62  --pu $PUDIR/EWKWPlus2Jets_WToLNu.txt
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_EWKWMinus2Jets_WToLNu  -i $INPUTDIR/14_EWKWMinus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt  -x 20.25  --pu $PUDIR/EWKWMinus2Jets_WToLNu.txt
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_EWKZ2Jets_ZToLL        -i $INPUTDIR/15_EWKZ2Jets_ZToLL_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt        -x 3.987  --pu $PUDIR/EWKZ2Jets_ZToLL.txt
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_EWKZ2Jets_ZToNuNu      -i $INPUTDIR/16_EWKZ2Jets_ZToNuNu_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt        -x 10.66  --pu $PUDIR/EWKZ2Jets_ZToNuNu.txt
#sleep 10m
#
#
#
#
# # #####################
##### single top :
#sleep 20m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ST_tW_antitop         -i $INPUTDIR/ST_tW_antitop.txt   -x 34.97  --pu $PUDIR/ST_tW_antitop.txt
#sleep 4m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ST_tW_top             -i $INPUTDIR/ST_tW_top.txt       -x 35.6  --pu $PUDIR/ST_tW_top.txt
#sleep 4m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ST_tchannel_antitop   -i $INPUTDIR/23_ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt   -x 80.95  --pu $PUDIR/PUsafe_weights.txt
#sleep 4m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ST_tchannel_top       -i $INPUTDIR/24_ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt       -x 136.02  --pu $PUDIR/PUsafe_weights.txt
#sleep 4m

## # #####################
##### SM Higgs :
#### HXSWG: xs(ZH) = 0.8839 pb
#### Z->qq : 69.91% , Z->ll : 3,3658% (x3 for all the leptons)
#### H->bb : 57.7%  , H->tautau : 6.32%
#### ZH (Zll, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
#### ZH (Zqq, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
#### ZH (Zall, Htautau) : XS teor ZH * BR H->tautau
#
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ZH_HBB_ZLL -i $INPUTDIR/59_ZH_HToBB_ZToLL_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.07924 -y 0.577  --pu $PUDIR/ZH_HToBB_ZToLL.txt
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ZH_HBB_ZQQ -i $INPUTDIR/60_ZH_HToBB_ZToQQ_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.554 -y 0.577  --pu $PUDIR/ZH_HToBB_ZToQQ.txt
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ZH_HTauTau -i $INPUTDIR/63_ZHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.8696 -y 0.0632  --pu $PUDIR/ZHToTauTau.txt
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ggHTauTau -i $INPUTDIR/GluGluHToTauTau.txt -x 21.46 -y 0.0632  --pu $PUDIR/GluGluHToTauTau.txt
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_WplusHTauTau -i $INPUTDIR/67_WplusHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.867 -y 0.0632  --pu $PUDIR/WplusHToTauTau.txt
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 50  -o $SKIMDIR/$OUTDIRR/SKIM_VBFHTauTau -i $INPUTDIR/66_VBFHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 3.861 -y 0.0632  --pu $PUDIR/VBFHToTauTau.txt
#sleep 10m
#### 
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_WminusHTauTau -i $INPUTDIR/68_WminusHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.5412 -y 0.0632  --pu $PUDIR/WminusHToTauTau.txt
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ttHJetTononBB -i $INPUTDIR/69_ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt -x 0.507 -y 0.423  --pu $PUDIR/PUsafe_weights.txt
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ttHJetToBB -i $INPUTDIR/70_ttHJetTobb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt -x 0.507 -y 0.577  --pu $PUDIR/PUsafe_weights.txt
#sleep 10m
###sleep30m
### # #####################
###### Others :
##
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_TTWJetsToLNu -i $INPUTDIR/71_TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.2043  --pu $PUDIR/TTWJetsToLNu.txt
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_TTWJetsToQQ -i $INPUTDIR/72_TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.4062  --pu $PUDIR/PUsafe_weights.txt
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_TTWW -i $INPUTDIR/73_TTWW_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14_ext1-v1.txt -x 0.007834  --pu $PUDIR/PUsafe_weights.txt
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_TTWZ -i $INPUTDIR/74_TTWZ_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt -x 0.002441  --pu $PUDIR/PUsafe_weights.txt
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_TTZZ -i $INPUTDIR/78_TTZZ_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt -x 0.001386  --pu $PUDIR/PUsafe_weights.txt
#sleep 10m



#
## # #####################
## # ### DI-boson: - 7*7 + 6*5 = 49 + 30 = 79
## # # note2: all njobs listed here were increased by x10 to make them run on the short queue
## # #XS taken from here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson
## ## for Z->4Q, I scaled from ZZ->2nu2Q using BR(2Nu2Q) = 2 * 20% * 69.91%
#
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100    -o $SKIMDIR/$OUTDIRR/SKIM_ZZZ                 -i $INPUTDIR/41_ZZZ_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.01398  --pu $PUDIR/PUsafe_weights.txt
#sleep 4m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100    -o $SKIMDIR/$OUTDIRR/SKIM_WWW                 -i $INPUTDIR/42_WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.2086   --pu $PUDIR/PUsafe_weights.txt
#sleep 4m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100    -o $SKIMDIR/$OUTDIRR/SKIM_WWZ                 -i $INPUTDIR/43_WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.1651  --pu $PUDIR/PUsafe_weights.txt
#sleep 4m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100    -o $SKIMDIR/$OUTDIRR/SKIM_WWTo2L2Nu           -i $INPUTDIR/45_WWTo2L2Nu_NNPDF31_TuneCP5_PSweights_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1.txt -x 11.08  --pu $PUDIR/PUsafe_weights.txt 
#sleep 4m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100    -o $SKIMDIR/$OUTDIRR/SKIM_WWToLNuQQ           -i $INPUTDIR/WWToLNuQQ.txt -x 45.99  --pu $PUDIR/WWToLNuQQ.txt
#sleep 4m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100    -o $SKIMDIR/$OUTDIRR/SKIM_WWTo4Q              -i $INPUTDIR/48_WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt -x 47.73   --pu $PUDIR/PUsafe_weights.txt
#sleep 4m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100    -o $SKIMDIR/$OUTDIRR/SKIM_WZTo3LNu -i $INPUTDIR/51_WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt  -x 5.052  --pu $PUDIR/PUsafe_weights.txt
#sleep 4m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100    -o $SKIMDIR/$OUTDIRR/SKIM_WZTo1L1Nu2Q         -i $INPUTDIR/56_WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 11.66  --pu $PUDIR/PUsafe_weights.txt
#sleep 4m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100    -o $SKIMDIR/$OUTDIRR/SKIM_WZTo2L2Q            -i $INPUTDIR/57_WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 6.331  --pu $PUDIR/PUsafe_weights.txt
#sleep 4m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100    -o $SKIMDIR/$OUTDIRR/SKIM_WZZ                 -i $INPUTDIR/58_WZZ_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.05565  --pu $PUDIR/WZZ.txt
#sleep 4m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100   -o $SKIMDIR/$OUTDIRR/SKIM_ZZTo4L              -i $INPUTDIR/40_ZZTo4L_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 1.325  --pu $PUDIR/PUsafe_weights.txt
#sleep 4m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100    -o $SKIMDIR/$OUTDIRR/SKIM_ZZTo2L2Q            -i $INPUTDIR/39_ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 3.688  --pu $PUDIR/PUsafe_weights.txt
#sleep 4m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100    -o $SKIMDIR/$OUTDIRR/SKIM_ZZTo2L2Nu            -i $INPUTDIR/38_ZZTo2L2Nu_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.5644  --pu $PUDIR/PUsafe_weights.txt


# signals
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_VBFSM -i inputFiles/Files_January2019_signals/46_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.001626 -a True --pu $PUDIR/PUsafe_weights.txt
#sleep 10m
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_GGHSM -i inputFiles/Files_January2019_signals/39_GluGluToHHTo2B2Tau_node_SM_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.03349 -a True --pu $PUDIR/PUsafe_weights.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_VBF_private -i inputFiles/Files_January2019_signals/PrivateProduction/VBFHHTo2B2Taus_SM.txt -x 0.001626 -a True --pu $PUDIR/PUsafe_weights.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_GGHSM_private -i inputFiles/Files_January2019_signals/PrivateProduction/GluGluToHHTo2B2Taus_12jets_SM.txt -x 0.03349 -a True --pu $PUDIR/PUsafe_weights.txt



# # #####################
# # ### Radion signals
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-250   -i inputFiles/Files_7Feb2016/allRadion/58_GluGluToRadionToHHTo2B2Tau_M-250_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-270   -i inputFiles/Files_7Feb2016/allRadion/59_GluGluToRadionToHHTo2B2Tau_M-270_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-280   -i inputFiles/Files_7Feb2016/allRadion/60_GluGluToRadionToHHTo2B2Tau_M-280_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 50 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-300   -i inputFiles/Files_7Feb2016/allRadion/61_GluGluToRadionToHHTo2B2Tau_M-300_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt  -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-350   -i inputFiles/Files_7Feb2016/allRadion/62_GluGluToRadionToHHTo2B2Tau_M-350_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 50 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-400   -i inputFiles/Files_7Feb2016/allRadion/63_GluGluToRadionToHHTo2B2Tau_M-400_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3.txt  -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-450   -i inputFiles/Files_7Feb2016/allRadion/64_GluGluToRadionToHHTo2B2Tau_M-450_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-500   -i inputFiles/Files_7Feb2016/allRadion/65_GluGluToRadionToHHTo2B2Tau_M-500_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-550   -i inputFiles/Files_7Feb2016/allRadion/66_GluGluToRadionToHHTo2B2Tau_M-550_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-600   -i inputFiles/Files_7Feb2016/allRadion/67_GluGluToRadionToHHTo2B2Tau_M-600_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-650   -i inputFiles/Files_7Feb2016/allRadion/68_GluGluToRadionToHHTo2B2Tau_M-650_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-750   -i inputFiles/Files_7Feb2016/allRadion/69_GluGluToRadionToHHTo2B2Tau_M-750_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-900   -i inputFiles/Files_7Feb2016/allRadion/70_GluGluToRadionToHHTo2B2Tau_M-900_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True

# #####################
# ### Graviton signals
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-250   -i inputFiles/Files_29Apr2016_gravitons/1_GluGluToBulkGravitonToHHTo2B2Tau_M-250_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-260   -i inputFiles/Files_29Apr2016_gravitons/2_GluGluToBulkGravitonToHHTo2B2Tau_M-260_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-270   -i inputFiles/Files_29Apr2016_gravitons/3_GluGluToBulkGravitonToHHTo2B2Tau_M-270_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-280   -i inputFiles/Files_29Apr2016_gravitons/4_GluGluToBulkGravitonToHHTo2B2Tau_M-280_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-300   -i inputFiles/Files_29Apr2016_gravitons/5_GluGluToBulkGravitonToHHTo2B2Tau_M-300_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-320   -i inputFiles/Files_29Apr2016_gravitons/6_GluGluToBulkGravitonToHHTo2B2Tau_M-320_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-340   -i inputFiles/Files_29Apr2016_gravitons/7_GluGluToBulkGravitonToHHTo2B2Tau_M-340_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-350   -i inputFiles/Files_29Apr2016_gravitons/8_GluGluToBulkGravitonToHHTo2B2Tau_M-350_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-400   -i inputFiles/Files_29Apr2016_gravitons/9_GluGluToBulkGravitonToHHTo2B2Tau_M-400_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-450   -i inputFiles/Files_29Apr2016_gravitons/10_GluGluToBulkGravitonToHHTo2B2Tau_M-450_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-500   -i inputFiles/Files_29Apr2016_gravitons/11_GluGluToBulkGravitonToHHTo2B2Tau_M-500_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-550   -i inputFiles/Files_29Apr2016_gravitons/12_GluGluToBulkGravitonToHHTo2B2Tau_M-550_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-600   -i inputFiles/Files_29Apr2016_gravitons/13_GluGluToBulkGravitonToHHTo2B2Tau_M-600_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-650   -i inputFiles/Files_29Apr2016_gravitons/14_GluGluToBulkGravitonToHHTo2B2Tau_M-650_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-750   -i inputFiles/Files_29Apr2016_gravitons/15_GluGluToBulkGravitonToHHTo2B2Tau_M-750_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-800   -i inputFiles/Files_29Apr2016_gravitons/16_GluGluToBulkGravitonToHHTo2B2Tau_M-800_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRSGravitonToHHTo2B2Tau_M-300   -i inputFiles/Files_29Apr2016_gravitons/1_GluGluToRSGravitonToHHTo2B2Tau_M-300_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRSGravitonToHHTo2B2Tau_M-650   -i inputFiles/Files_29Apr2016_gravitons/2_GluGluToRSGravitonToHHTo2B2Tau_M-650_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRSGravitonToHHTo2B2Tau_M-900   -i inputFiles/Files_29Apr2016_gravitons/3_GluGluToRSGravitonToHHTo2B2Tau_M-900_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True




#-20, -15, -10, -8, -6, -4, -3, -2, -1, 0.001, 1, 2, 2.45,  3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20, 25, 30

# # # ## HH din reweight
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_1  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_1   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt    -x 1.0 --kl -20     --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt 
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_2  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_2   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt    -x 1.0 --kl -15     --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_3  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_3   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl -10     --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_4  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_4   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl -8      --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_5  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_5   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl -6      --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_6  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_6   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl -4      --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_7  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_7   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl -3      --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_8  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_8   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl -2      --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_9  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_9   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl -1      --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_10  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_10   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 0.001  --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_11  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_11   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 1       --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_12  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_12   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 2       --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_13  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_13   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 3       --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_14  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_14   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 2.45       --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_15  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_15   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 4       --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
#sleep 30m
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_16  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_16   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 5       --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_17  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_17   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 6       --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_18  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_18   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 7       --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_19  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_19   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 8       --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_20  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_20   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 9       --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_21  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_21   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 10      --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_22  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_22   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 11      --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_23  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_23   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 12      --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_24  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_24   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 14      --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_25  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_25   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 16      --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_26  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_26   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 18      --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_27  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_27   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 20      --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg --tag HH_lambdarew_28  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_28   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 25      --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  --tag HH_lambdarew_29 -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_29   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0 --kl 30      --kt 1.0 -a True --pu $PUDIR/PUsafe_weights.txt



#### HH dyn rew 12 benchmarks
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_1   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt -x 1.0    --kl  7.5  --kt 1.0 --c2 -1.0 --cg  0.0 --c2g  0.0 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_2   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt -x 1.0    --kl  1.0  --kt 1.0 --c2  0.5 --cg -0.8 --c2g  0.6 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_3   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt -x 1.0    --kl  1.0  --kt 1.0 --c2 -1.5 --cg  0.0 --c2g -0.8 -a True 
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_4   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt -x 1.0    --kl -3.5  --kt 1.5 --c2 -3.0 --cg  0.0 --c2g  0.0 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_5   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt -x 1.0    --kl  1.0  --kt 1.0 --c2  0.0 --cg  0.8 --c2g -1.0 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_6   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt -x 1.0    --kl  2.4  --kt 1.0 --c2  0.0 --cg  0.2 --c2g -0.2 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_7   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt -x 1.0    --kl  5.0  --kt 1.0 --c2  0.0 --cg  0.2 --c2g -0.2 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_8   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt -x 1.0    --kl  15.0 --kt 1.0 --c2  0.0 --cg -1.0 --c2g  1.0 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_9   -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt -x 1.0    --kl  1.0  --kt 1.0 --c2  1.0 --cg -0.6 --c2g  0.6 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_10  -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0   --kl 10.0  --kt 1.5 --c2 -1.0 --cg  0.0 --c2g  0.0 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_11  -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0   --kl  2.4  --kt 1.0 --c2  0.0 --cg  1.0 --c2g -1.0 -a True
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_12  -i inputFiles/Files_January2019_signals/GluGluHH_nonRes.txt  -x 1.0   --kl 15.0  --kt 1.0 --c2  1.0 --cg  0.0 --c2g  0.0 -a True 
#sleep30m


#VBF signals

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_VBF_CV_1_C2V_1_C3_0 -i inputFiles/Files_January2019_signals/47_VBFHHTo2B2Tau_CV_1_C2V_1_C3_0_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.003947   -a True --pu $PUDIR/PUsafe_weights.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_VBF_CV_1_C2V_1_C3_2 -i inputFiles/Files_January2019_signals/48_VBFHHTo2B2Tau_CV_1_C2V_1_C3_2_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.001243   -a True --pu $PUDIR/PUsafe_weights.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_VBF_CV_1_C2V_2_C3_1 -i inputFiles/Files_January2019_signals/49_VBFHHTo2B2Tau_CV_1_C2V_2_C3_1_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.012719   -a True --pu $PUDIR/PUsafe_weights.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_VBF_CV_1_5_C2V_1_C3_1 -i inputFiles/Files_January2019_signals/50_VBFHHTo2B2Tau_CV_1_5_C2V_1_C3_1_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.057943   -a True --pu $PUDIR/PUsafe_weights.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_VBF_CV_1_C2V_0_C3_2 -i inputFiles/Files_January2019_signals/PrivateProduction/VBFHHTo2B2Tau_CV_1_C2V_0_C3_2_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14.txt -x 0.01705   -a True --pu $PUDIR/PUsafe_weights.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_VBF_CV_1_C2V_1_C3_1 -i inputFiles/Files_January2019_signals/46_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.001626  -a True --pu $PUDIR/PUsafe_weights.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_GGHSM_xs -i inputFiles/Files_January2019_signals/39_GluGluToHHTo2B2Tau_node_SM_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.03131 -y 0.073 -a True --pu $PUDIR/PUsafe_weights.txt


## extra pts for a finer 1D plot
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99900   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99900.txt  -x 1.0 --kl 0.0      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99901   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99901.txt  -x 1.0 --kl 0.1      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99902   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99902.txt  -x 1.0 --kl 0.2      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99903   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99903.txt  -x 1.0 --kl 0.3      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99904   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99904.txt  -x 1.0 --kl 0.4      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99905   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99905.txt  -x 1.0 --kl 0.5      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99906   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99906.txt  -x 1.0 --kl 0.6      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99907   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99907.txt  -x 1.0 --kl 0.7      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99908   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99908.txt  -x 1.0 --kl 0.8      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99909   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99909.txt  -x 1.0 --kl 0.9      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99910   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99910.txt  -x 1.0 --kl 1.0      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99911   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99911.txt  -x 1.0 --kl 1.1      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99912   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99912.txt  -x 1.0 --kl 1.2      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99913   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99913.txt  -x 1.0 --kl 1.3      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99914   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99914.txt  -x 1.0 --kl 1.4      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99915   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99915.txt  -x 1.0 --kl 1.5      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99916   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99916.txt  -x 1.0 --kl 1.6      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99917   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99917.txt  -x 1.0 --kl 1.7      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99918   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99918.txt  -x 1.0 --kl 1.8      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99919   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99919.txt  -x 1.0 --kl 1.9      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99920   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99920.txt  -x 1.0 --kl 2.0      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99921   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99921.txt  -x 1.0 --kl 2.1      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99922   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99922.txt  -x 1.0 --kl 2.2      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99923   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99923.txt  -x 1.0 --kl 2.3      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99924   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99924.txt  -x 1.0 --kl 2.4      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99925   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99925.txt  -x 1.0 --kl 2.5      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99926   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99926.txt  -x 1.0 --kl 2.6      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99927   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99927.txt  -x 1.0 --kl 2.7      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99928   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99928.txt  -x 1.0 --kl 2.8      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99929   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99929.txt  -x 1.0 --kl 2.9      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99930   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99930.txt  -x 1.0 --kl 3.0      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99931   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99931.txt  -x 1.0 --kl 3.1      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99932   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99932.txt  -x 1.0 --kl 3.2      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99933   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99933.txt  -x 1.0 --kl 3.3      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99934   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99934.txt  -x 1.0 --kl 3.4      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99935   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99935.txt  -x 1.0 --kl 3.5      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99936   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99936.txt  -x 1.0 --kl 3.6      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99937   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99937.txt  -x 1.0 --kl 3.7      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99938   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99938.txt  -x 1.0 --kl 3.8      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99939   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99939.txt  -x 1.0 --kl 3.9      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99940   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99940.txt  -x 1.0 --kl 4.0      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99941   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99941.txt  -x 1.0 --kl 4.1      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99942   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99942.txt  -x 1.0 --kl 4.2      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99943   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99943.txt  -x 1.0 --kl 4.3      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99944   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99944.txt  -x 1.0 --kl 4.4      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99945   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99945.txt  -x 1.0 --kl 4.5      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99946   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99946.txt  -x 1.0 --kl 4.6      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99947   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99947.txt  -x 1.0 --kl 4.7      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99948   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99948.txt  -x 1.0 --kl 4.8      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99949   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99949.txt  -x 1.0 --kl 4.9      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99950   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99950.txt  -x 1.0 --kl 5.0      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99951   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99951.txt  -x 1.0 --kl 5.1      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99952   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99952.txt  -x 1.0 --kl 5.2      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99953   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99953.txt  -x 1.0 --kl 5.3      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99954   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99954.txt  -x 1.0 --kl 5.4      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99955   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99955.txt  -x 1.0 --kl 5.5      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99956   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99956.txt  -x 1.0 --kl 5.6      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99957   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99957.txt  -x 1.0 --kl 5.7      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99958   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99958.txt  -x 1.0 --kl 5.8      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99959   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99959.txt  -x 1.0 --kl 5.9      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99960   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99960.txt  -x 1.0 --kl 6.0      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99961   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99961.txt  -x 1.0 --kl 6.1      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99962   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99962.txt  -x 1.0 --kl 6.2      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99963   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99963.txt  -x 1.0 --kl 6.3      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99964   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99964.txt  -x 1.0 --kl 6.4      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99965   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99965.txt  -x 1.0 --kl 6.5      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99966   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99966.txt  -x 1.0 --kl 6.6      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99967   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99967.txt  -x 1.0 --kl 6.7      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99968   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99968.txt  -x 1.0 --kl 6.8      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99969   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99969.txt  -x 1.0 --kl 6.9      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99970   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99970.txt  -x 1.0 --kl 7.0      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99971   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99971.txt  -x 1.0 --kl 7.1      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99972   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99972.txt  -x 1.0 --kl 7.2      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99973   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99973.txt  -x 1.0 --kl 7.3      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99974   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99974.txt  -x 1.0 --kl 7.4      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99975   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99975.txt  -x 1.0 --kl 7.5      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99976   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99976.txt  -x 1.0 --kl 7.6      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99977   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99977.txt  -x 1.0 --kl 7.7      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99978   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99978.txt  -x 1.0 --kl 7.8      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99979   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99979.txt  -x 1.0 --kl 7.9      --kt 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_99980   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_99980.txt  -x 1.0 --kl 8.0      --kt 1.0 -a True



# # ## HH with reweighting --  a lot of files!
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_0   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_0.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_0.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_1   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_1.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_1.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_2   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_2.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_2.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_3   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_3.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_3.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_4   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_4.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_4.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_5   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_5.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_5.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_6   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_6.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_6.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_7   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_7.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_7.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_8   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_8.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_8.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_9   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_9.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_9.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_10   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_10.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_10.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_11   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_11.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_11.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_12   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_12.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_12.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_13   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_13.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_13.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_14   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_14.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_14.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_15   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_15.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_15.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_16   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_16.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_16.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_17   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_17.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_17.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_18   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_18.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_18.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_19   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_19.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_19.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_20   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_20.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_20.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_21   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_21.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_21.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_22   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_22.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_22.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_23   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_23.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_23.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_24   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_24.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_24.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_25   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_25.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_25.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_26   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_26.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_26.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_27   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_27.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_27.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_28   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_28.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_28.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_29   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_29.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_29.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_30   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_30.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_30.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_31   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_31.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_31.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_32   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_32.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_32.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_33   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_33.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_33.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_34   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_34.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_34.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_35   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_35.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_35.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_36   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_36.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_36.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_37   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_37.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_37.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_38   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_38.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_38.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_39   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_39.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_39.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_40   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_40.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_40.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_41   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_41.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_41.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_42   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_42.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_42.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_43   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_43.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_43.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_44   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_44.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_44.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_45   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_45.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_45.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_46   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_46.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_46.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_47   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_47.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_47.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_48   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_48.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_48.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_49   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_49.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_49.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_50   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_50.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_50.root -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_51   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_51.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_51.root -a True


# ########
# ## SOME TESTS OF HH DYNAMIC REWEIGHT - IT WORKS!

# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 10 -k True -o $SKIMDIR/$OUTDIRR/SKIM_HH_node_SM   -i inputFiles/Files_7Feb2016/38_GluGluToHHTo2B2Tau_node_SM_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt  -x 1.0 -a True

# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_dyn_21   -i inputFiles/Files_7Feb2016/allNonResClones_dynRew/filelist_allNonRes_dr_21.txt  -x 1.0 --kl 1.0  --kt 1.0 -a True
# # python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 20  -o $SKIMDIR/$OUTDIRR/SKIM_HH_lambdarew_dyn_41   -i inputFiles/Files_7Feb2016/allNonResClones_dynRew/filelist_allNonRes_dr_41.txt  -x 1.0 --kl 20.0 --kt 1.0 -a True


