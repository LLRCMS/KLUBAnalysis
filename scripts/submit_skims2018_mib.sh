# LOG
OUTDIRR="Skims_Legacy2017_24Apr2020"

# INPUT
INPUTDIR_BKG="inputFiles/Legacy2018_backgroundsMIB"
INPUTDIR_DATA="inputFiles/Legacy2018_dataMIB"
INPUTDIR_SIG="inputFiles/Legacy2018_signalsMIB"

# OUTPUT
SKIMDIR="/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2018_15July2020"

# PU weights
PUDIR="/gwpool/users/dzuolo/HHbbtautatuAnalysisLegacy/CMSSW_11_1_0_pre6/src/KLUBAnalysis/weights/PUreweight/Legacy_Run2_PU_SF/2018"

# Environment
source /cvmfs/cms.cern.ch/cmsset_default.sh
source scripts/setup.sh
mkdir $OUTDIRR

<<COMMENT1

#########################
### SKIMS LEGACY 2018 ###
#########################

######################
#### DATA - filelists up to date

python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018_mib.cfg  -n 100 -q longcms -o $SKIMDIR/SKIM_SingleMuon2018A	-k True -i $INPUTDIR_DATA/1_SingleMuon__Run2018A-17Sep2018-v2.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018_mib.cfg  -n 100 -q longcms -o $SKIMDIR/SKIM_SingleMuon2018B	-k True -i $INPUTDIR_DATA/2_SingleMuon__Run2018B-17Sep2018-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018_mib.cfg  -n 100 -q longcms -o $SKIMDIR/SKIM_SingleMuon2018C	-k True -i $INPUTDIR_DATA/3_SingleMuon__Run2018C-17Sep2018-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018_mib.cfg  -n 100 -q longcms -o $SKIMDIR/SKIM_SingleMuon2018D	-k True -i $INPUTDIR_DATA/1_SingleMuon__Run2018D-22Jan2019-v2.txt


python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018_mib.cfg  -n 100 -q longcms -o $SKIMDIR/SKIM_SingleElectron2018A   -k True	-i $INPUTDIR_DATA/1_EGamma__Run2018A-17Sep2018-v2.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018_mib.cfg  -n 100 -q longcms -o $SKIMDIR/SKIM_SingleElectron2018B   -k True	-i $INPUTDIR_DATA/2_EGamma__Run2018B-17Sep2018-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018_mib.cfg  -n 100 -q longcms -o $SKIMDIR/SKIM_SingleElectron2018C   -k True	-i $INPUTDIR_DATA/3_EGamma__Run2018C-17Sep2018-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018_mib.cfg  -n 100 -q longcms -o $SKIMDIR/SKIM_SingleElectron2018D   -k True	-i $INPUTDIR_DATA/1_EGamma__Run2018D-22Jan2019-v2.txt

python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018_mib.cfg  -n 100 -q longcms -o $SKIMDIR/SKIM_Tau2018A              -k True -i $INPUTDIR_DATA/1_Tau__Run2018A-17Sep2018-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018_mib.cfg  -n 100 -q longcms -o $SKIMDIR/SKIM_Tau2018B              -k True -i $INPUTDIR_DATA/2_Tau__Run2018B-17Sep2018-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018_mib.cfg  -n 100 -q longcms -o $SKIMDIR/SKIM_Tau2018C              -k True -i $INPUTDIR_DATA/3_Tau__Run2018C-17Sep2018-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018_mib.cfg  -n 100 -q longcms -o $SKIMDIR/SKIM_Tau2018D              -k True -i $INPUTDIR_DATA/1_Tau__Run2018D-PromptReco-v2.txt

COMMENT1

#######################
##### TT - filelists up to date
### XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
### TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
### hh = 45.7%
### ll = 10.5%
### hl = 21.9% (x2 for permutation t-tbar)
#sleep 1h
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 200 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_TT_fullyHad -x 377.96 -t True -b 1 -i $INPUTDIR_BKG/3_TTToHadronic_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
#python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 200 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_TT_fullyLep -x 88.29  -t True -b 4 -i $INPUTDIR_BKG/1_TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
#python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 200 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_TT_semiLep  -x 365.34 -t True -b 5 -i $INPUTDIR_BKG/2_TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt

<<COMMENT2

######################
#### DY - filelists up to date
### xsec from:
### https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#DY_Z
#sleep 1h
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c  config/skim_Legacy2018_mib.cfg  -n 200 -q longcms -k True  --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_DY         -i $INPUTDIR_BKG/DYmerged.txt -x 6077.22 -g True --DY True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c  config/skim_Legacy2018_mib.cfg  -n 200 -q longcms -k True  --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_DY_lowMass -i $INPUTDIR_BKG/DYJetsToLL_M-10to50_merged.txt -x 18610 


#######################
#### Wjets - filelists up to date
#sleep 1h
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 50 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WJets_HT_0_100     -i $INPUTDIR_BKG/1_WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt		-y 1.213784 -x 48917.48 -z 100   
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 50 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WJets_HT_100_200   -i $INPUTDIR_BKG/2_WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt	-y 1.213784 -x 1345
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 50 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WJets_HT_200_400   -i $INPUTDIR_BKG/3_WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt	-y 1.213784 -x 359.7
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 50 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WJets_HT_400_600   -i $INPUTDIR_BKG/4_WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt	-y 1.213784 -x 48.91
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 50 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WJets_HT_600_800   -i $INPUTDIR_BKG/1_WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt	-y 1.213784 -x 12.05
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 50 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WJets_HT_800_1200  -i $INPUTDIR_BKG/6_WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt  -y 1.213784 -x 5.501
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 50 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WJets_HT_1200_2500 -i $INPUTDIR_BKG/7_WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -y 1.213784 -x 1.329
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 50 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WJets_HT_2500_Inf  -i $INPUTDIR_BKG/8_WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt  -y 1.213784 -x 0.03216


######################
##### ELECTROWEAK - filelists up to date
### XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
#sleep 1h
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 100 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_EWKWPlus2Jets_WToLNu   -x 29.59 -i $INPUTDIR_BKG/1_EWKWPlus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 100 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_EWKWMinus2Jets_WToLNu  -x 23.24 -i $INPUTDIR_BKG/2_EWKWMinus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 100 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_EWKZ2Jets_ZToLL	-x 3.987 -i $INPUTDIR_BKG/3_EWKZ2Jets_ZToLL_M-50_TuneCP5_PSweights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt

######################
##### single top : - filelists up to date
### XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
#sleep 1h
python scripts/skimNtuple_mib_2018.py  -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 80 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_ST_tW_antitop	     -x 35.85  -i $INPUTDIR_BKG/1_ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1.txt
python scripts/skimNtuple_mib_2018.py  -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 80 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_ST_tW_top            -x 35.85  -i $INPUTDIR_BKG/2_ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1.txt
python scripts/skimNtuple_mib_2018.py  -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 80 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_ST_tchannel_antitop  -x 80.95  -i $INPUTDIR_BKG/3_ST_t-channel_antitop_5f_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
python scripts/skimNtuple_mib_2018.py  -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 80 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_ST_tchannel_top	     -x 136.02 -i $INPUTDIR_BKG/4_ST_t-channel_top_5f_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt


######################
##### SM Higgs : - filelists up to date
### From https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNHLHE2019
### HXSWG: xs(ZH) = 0.880 pb
### HXSWG: xs(W+H) = 0.831 pb
### HXSWG: xs(W-H) = 0.527 pb
### HXSWG: xs(ggH) = 48.61 pb
### HXSWG: xs(VBFH) = 3.766 pb
### HXSWG: xs(ttH) = 0.5071 pb
### Z->qq : 69.91% , Z->ll : 3,3658% (x3 for all the leptons)
### H->bb : 57.7%  , H->tautau : 6.32%
### ZH (Zll, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
### ZH (Zqq, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
### ZH (Zall, Htautau) : XS teor ZH * BR H->tautau
#sleep 1h
python scripts/skimNtuple_mib_2018.py  -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 30 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_ZH_HBB_ZLL     -x 0.08781 -y 0.577  -i $INPUTDIR_BKG/ZH_HToBB_ZToLL.txt
python scripts/skimNtuple_mib_2018.py  -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 30 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_ZH_HBB_ZQQ     -x 0.6079  -y 0.577  -i $INPUTDIR_BKG/3_ZH_HToBB_ZToQQ_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
python scripts/skimNtuple_mib_2018.py  -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 30 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_ZH_HTauTau     -x 0.880   -y 0.0632 -i $INPUTDIR_BKG/4_ZHToTauTau_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt
python scripts/skimNtuple_mib_2018.py  -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 30 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_ggHTauTau      -x 48.61   -y 0.0632 -i $INPUTDIR_BKG/8_GluGluHToTauTau_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt
python scripts/skimNtuple_mib_2018.py  -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 30 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WplusHTauTau   -x 0.831   -y 0.0632 -i $INPUTDIR_BKG/5_WplusHToTauTau_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt
python scripts/skimNtuple_mib_2018.py  -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 30 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_VBFHTauTau     -x 3.766   -y 0.0632 -i $INPUTDIR_BKG/7_VBFHToTauTau_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1.txt
python scripts/skimNtuple_mib_2018.py  -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 30 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WminusHTauTau  -x 0.527   -y 0.0632 -i $INPUTDIR_BKG/6_WminusHToTauTau_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt
python scripts/skimNtuple_mib_2018.py  -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 30 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_ttHJetTononBB  -x 0.5071  -y 0.3598 -i $INPUTDIR_BKG/2_ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt --ttHToNonBB True
python scripts/skimNtuple_mib_2018.py  -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 30 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_ttHJetToBB     -x 0.5071  -y 0.577  -i $INPUTDIR_BKG/1_ttHTobb_M125_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v3.txt
python scripts/skimNtuple_mib_2018.py  -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 30 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_ttHJetToTauTau -x 0.5071  -y 0.0632 -i $INPUTDIR_BKG/2_ttHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt


######################
#### Multiboson: - filelists up to date
### Some XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
### Some other XS taken from http://cms.cern.ch/iCMS/jsp/db_notes/noteInfo.jsp?cmsnoteid=CMS%20AN-2019/111
#sleep 1h
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_ZZTo4L         -x 1.26   -i $INPUTDIR_BKG/1_ZZTo4L_13TeV_powheg_pythia8_TuneCP5__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_ZZTo2L2Nu      -x 0.564  -i $INPUTDIR_BKG/2_ZZTo2L2Nu_TuneCP5_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext2-v2.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 200 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_ZZTo2L2Q      -x 5.52   -i $INPUTDIR_BKG/3_ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 200 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_ZZTo2Q2Nu     -x 4.07   -i $INPUTDIR_BKG/4_ZZTo2Q2Nu_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WZTo3LNu       -x 4.43   -i $INPUTDIR_BKG/5_WZTo3LNu_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WZTo1L1Nu2Q	-x 10.71  -i $INPUTDIR_BKG/6_WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 40 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WZTo1L3Nu      -x 3.06   -i $INPUTDIR_BKG/7_WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 200 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WZTo2L2Q      -x 5.595  -i $INPUTDIR_BKG/8_WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WWTo2L2Nu      -x 12.18  -i $INPUTDIR_BKG/9_WWTo2L2Nu_NNPDF31_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WWToLNuQQ      -x 50.00  -i $INPUTDIR_BKG/10_WWToLNuQQ_NNPDF31_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WWTo4Q         -x 51.72  -i $INPUTDIR_BKG/11_WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_ZZZ	       -x 0.0147 -i $INPUTDIR_BKG/4_ZZZ_TuneCP5_13TeV-amcatnlo-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WZZ	       -x 0.057  -i $INPUTDIR_BKG/2_WZZ_TuneCP5_13TeV-amcatnlo-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WWW	       -x 0.209  -i $INPUTDIR_BKG/1_WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_WWZ	       -x 0.168  -i $INPUTDIR_BKG/3_WWZ_TuneCP5_13TeV-amcatnlo-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2.txt


######################
#### Others : - filelists up to date
#sleep 1h
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 120 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_TTWJetsToLNu -x 0.2043   -i $INPUTDIR_BKG/4_TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_TTWJetsToQQ  -x 0.4062	-i $INPUTDIR_BKG/5_TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_TTWW	   -x 0.006979 -i $INPUTDIR_BKG/6_TTWW_TuneCP5_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext2-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_TTWZ	   -x 0.002441 -i $INPUTDIR_BKG/7_TTWZ_TuneCP5_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_TTWH	   -x 0.00158  -i $INPUTDIR_BKG/8_TTWH_TuneCP5_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_TTZH	   -x 0.00154  -i $INPUTDIR_BKG/9_TTZH_TuneCP5_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 200 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_TTZToLLNuNu  -x 0.2529   -i $INPUTDIR_BKG/10_TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_TTZToQQ     -x 0.5297   -i $INPUTDIR_BKG/11_TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1.txt
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_TTZZ	   -x 0.001386 -i $INPUTDIR_BKG/12_TTZZ_TuneCP5_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2.txt


######################
# Signals ggF non res - filelists up to date
# norm xs = 1 p
#sleep 1h
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_SM	  -i $INPUTDIR_SIG/12_GluGluToHHTo2B2Tau_node_SM_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_node_2  -i $INPUTDIR_SIG/4_GluGluToHHTo2B2Tau_node_2_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_node_3  -i $INPUTDIR_SIG/5_GluGluToHHTo2B2Tau_node_3_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_node_4  -i $INPUTDIR_SIG/2_GluGluToHHTo2B2Tau_node_4_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_node_5  -i $INPUTDIR_SIG/3_GluGluToHHTo2B2Tau_node_5_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_node_6  -i $INPUTDIR_SIG/4_GluGluToHHTo2B2Tau_node_6_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_node_7  -i $INPUTDIR_SIG/9_GluGluToHHTo2B2Tau_node_7_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_node_8  -i $INPUTDIR_SIG/10_GluGluToHHTo2B2Tau_node_8_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt  -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_node_9  -i $INPUTDIR_SIG/1_GluGluToHHTo2B2Tau_node_9_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_node_10 -i $INPUTDIR_SIG/1_GluGluToHHTo2B2Tau_node_10_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt  -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_node_11 -i $INPUTDIR_SIG/1_GluGluToHHTo2B2Tau_node_11_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt  -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 20 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_node_12 -i $INPUTDIR_SIG/3_GluGluToHHTo2B2Tau_node_12_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt  -x 1. -a True


######################
#### Signals ggF non res  - filelists up to date
### norm xs = 1 p
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_NLO_SM       -i $INPUTDIR_SIG/2_GluGluToHHTo2B2Tau_node_cHHH1_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt    -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10   -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_NLO_cHHH0	-i $INPUTDIR_SIG/1_GluGluToHHTo2B2Tau_node_cHHH0_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt    -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10   -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_NLO_cHHH2p45 -i $INPUTDIR_SIG/3_GluGluToHHTo2B2Tau_node_cHHH2p45_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10   -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_NLO_cHHH5	-i $INPUTDIR_SIG/4_GluGluToHHTo2B2Tau_node_cHHH5_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt    -x 1. -a True

### norm to theoretical xs 
# sigma_NNLO+FTapprox for SM: 31.05 fb
# sigma_NNLO+FTapprox / sigma_NLO  = 1.115 for SM # TEMPORARY: need to fix with factor kL dependent (pag. 129: https://arxiv.org/pdf/2003.01700.pdf)
# f(kL) = A + B*kL + C*kL**2 
# A = 62.5339 
# B = -44.323
# C = 9.6340
# (slide 10: https://indico.cern.ch/event/885273/contributions/3812533/attachments/2016615/3370728/HH_combine_model_7Apr2018.pdf)
# xs (kL = 1)                      = 0.03105 pb
# xs (kL = 0)    = f(0)    * 1.115 = 0.06972 pb
# xs (kL = 2.45) = f(2.45) * 1.115 = 0.01312 pb
# xs (kL = 5)    = f(5)    * 1.115 = 0.09117 pb

python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_NLO_SM_xs	  -i $INPUTDIR_SIG/2_GluGluToHHTo2B2Tau_node_cHHH1_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt	 -x 0.03105 -a True --hhNLO
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_NLO_cHHH0_xs    -i $INPUTDIR_SIG/1_GluGluToHHTo2B2Tau_node_cHHH0_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt	 -x 0.06972 -a True --hhNLO
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_NLO_cHHH2p45_xs -i $INPUTDIR_SIG/3_GluGluToHHTo2B2Tau_node_cHHH2p45_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 0.01312 -a True --hhNLO
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10 -q longcms  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_GGHH_NLO_cHHH5_xs    -i $INPUTDIR_SIG/4_GluGluToHHTo2B2Tau_node_cHHH5_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt	 -x 0.09117 -a True --hhNLO

######################
#### Signals VBF non res - filelists up to date
### norm xs = 1 pb
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_VBFHH_CV_1_C2V_1_C3_1 -i $INPUTDIR_SIG/1_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_VBFHH_CV_0_5_C2V_1_C3_1 -i $INPUTDIR_SIG/2_VBFHHTo2B2Tau_CV_0_5_C2V_1_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_VBFHH_CV_1_5_C2V_1_C3_1 -i $INPUTDIR_SIG/3_VBFHHTo2B2Tau_CV_1_5_C2V_1_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_VBFHH_CV_1_C2V_1_C3_0 -i $INPUTDIR_SIG/4_VBFHHTo2B2Tau_CV_1_C2V_1_C3_0_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_VBFHH_CV_1_C2V_1_C3_2 -i $INPUTDIR_SIG/5_VBFHHTo2B2Tau_CV_1_C2V_1_C3_2_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_VBFHH_CV_1_C2V_2_C3_1 -i $INPUTDIR_SIG/6_VBFHHTo2B2Tau_CV_1_C2V_2_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True

### norm to theoretical xs 
# xs_theo(SM) = 0.001726
#  CV C2V C3 |  xs_MG5      * xs_theo(SM)/xs_MG5(SM)
#   1   1  1 |  0.001668 pb * 1.034772182             =   0.001726 pb
# 0.5   1  1 |  0.01046  pb * 1.034772182             =   0.010824 pb
# 1.5   1  1 |  0.0638   pb * 1.034772182             =   0.066018 pb
#   1   1  0 |  0.004454 pb * 1.034772182             =   0.004609 pb
#   1   1  2 |  0.001375 pb * 1.034772182             =   0.001423 pb
#   1   2  1 |  0.01374  pb * 1.034772182             =   0.014218 pb

python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_VBFHH_CV_1_C2V_1_C3_1_xs -i $INPUTDIR_SIG/1_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt     -x 0.001726  -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_VBFHH_CV_0_5_C2V_1_C3_1_xs -i $INPUTDIR_SIG/2_VBFHHTo2B2Tau_CV_0_5_C2V_1_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 0.010824  -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_VBFHH_CV_1_5_C2V_1_C3_1_xs -i $INPUTDIR_SIG/3_VBFHHTo2B2Tau_CV_1_5_C2V_1_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 0.066018  -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_VBFHH_CV_1_C2V_1_C3_0_xs -i $INPUTDIR_SIG/4_VBFHHTo2B2Tau_CV_1_C2V_1_C3_0_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt     -x 0.004609  -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_VBFHH_CV_1_C2V_1_C3_2_xs -i $INPUTDIR_SIG/5_VBFHHTo2B2Tau_CV_1_C2V_1_C3_2_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt     -x 0.001423  -a True
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 10 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_VBFHH_CV_1_C2V_2_C3_1_xs -i $INPUTDIR_SIG/6_VBFHHTo2B2Tau_CV_1_C2V_2_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt     -x 0.014218  -a True


######################
#### Reweighting ggF non res - filelists up to date
### norm xs = 1 pb
python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c  config/skim_Legacy2018_mib.cfg  -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/SKIM_HHRew_SM  -x 1.0 --kl 1.0  --kt 1.0 -x 1. -a True -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_LO_allNodes.txt

#this is just for cross check with LO node SM, which wrongly has c2g set to 1
#python scripts/skimNtuple_mib_2018.py -T $OUTDIRR -s True -c  config/skim_Legacy2018_mib.cfg  -n 20 -q longcms -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIRR/SKIM_HHRew_SM_wrong  -x 1.0 --kl 1.0  --kt 1.0 --c2 0.0 --cg 0.0 --c2g 1. -a True -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_LO_allNodes.txt

COMMENT2
