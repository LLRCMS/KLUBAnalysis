AMESSAGE="V2 2018 Legacy ntuples"

OUTDIRR="SKIMS_22Jan2020"

INPUTDIR_BKG="inputFiles/Legacy2018_backgrounds"
INPUTDIR_DATA="inputFiles/Legacy2018_data"
INPUTDIR_SIG="inputFiles/Legacy2018_signals"

PUDIR="weights/PUreweight/outputs_January2019" # outdated -> defaul path to be fixed in scripts/skimNtuples.py
SKIMDIR="/data_CMS/cms/davignon/HHLegacy_2018_SKIMS_22_01_20"

source scripts/setup.sh
source /opt/exp_soft/cms/t3/t3setup
mkdir -p $SKIMDIR/$OUTDIRR/
touch $SKIMDIR/$OUTDIRR/README.txt
echo $AMESSAGE > $SKIMDIR/$OUTDIRR/README.txt
cp /data_CMS/cms/davignon/HHbbtautau_01_20/CMSSW_10_2_16/src/KLUBAnalysis/scripts/listAll.sh $SKIMDIR/$OUTDIRR/



#########################
### SKIMS LEGACY 2018 ###
#########################

#DATA - filelists up to date
python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_SingleMuon2018A   -k True     -i $INPUTDIR_DATA/1_SingleMuon__Run2018A-17Sep2018-v2.txt
python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_SingleMuon2018B   -k True     -i $INPUTDIR_DATA/2_SingleMuon__Run2018B-17Sep2018-v1.txt
python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_SingleMuon2018C   -k True     -i $INPUTDIR_DATA/3_SingleMuon__Run2018C-17Sep2018-v1.txt
python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_SingleMuon2018D   -k True     -i $INPUTDIR_DATA/1_SingleMuon__Run2018D-PromptReco-v2.txt

python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_SingleElectron2018A   -k True     -i $INPUTDIR_DATA/1_EGamma__Run2018A-17Sep2018-v2.txt 
python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_SingleElectron2018B   -k True     -i $INPUTDIR_DATA/2_EGamma__Run2018B-17Sep2018-v1.txt 
python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_SingleElectron2018C   -k True     -i $INPUTDIR_DATA/3_EGamma__Run2018C-17Sep2018-v1.txt 
python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_SingleElectron2018D   -k True     -i $INPUTDIR_DATA/1_EGamma__Run2018D-PromptReco-v2.txt

python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_Tau2018A   -k True     -i $INPUTDIR_DATA/1_Tau__Run2018A-17Sep2018-v1.txt
python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_Tau2018B   -k True     -i $INPUTDIR_DATA/2_Tau__Run2018B-17Sep2018-v1.txt
python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_Tau2018C   -k True     -i $INPUTDIR_DATA/3_Tau__Run2018C-17Sep2018-v1.txt
python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_Legacy2018.cfg  -n 200   -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_Tau2018D   -k True     -i $INPUTDIR_DATA/1_Tau__Run2018D-PromptReco-v2.txt

##### TT #FIXME - Obsolete filelists
# XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
# TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
# hh = 45.7%
# ll = 10.5%
# hl = 21.9% (x2 for permutation t-tbar)

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 700   -Y 2018 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_fullyHad  -i $INPUTDIR_BKG/2_TTToHadronic_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt     -x 377.96 -t True -b 1  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 1000  -Y 2018 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_fullyLep  -i $INPUTDIR_BKG/1_TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt        -x 88.29  -t True -b 4 
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 1000  -Y 2018 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_semiLep   -i $INPUTDIR_BKG/1_TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 365.34 -t True -b 5 

### DY # FIXME - obsolete filelists
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_Legacy2018.cfg  -n 200 -k True -n 1000  -Y 2018 -k True -o $SKIMDIR/$OUTDIRR/SKIM_DY -i $INPUTDIR_BKG/DYmerged.txt -x 6225.42 -g True


### # #####################
### Wjets # FIXME - obsolete filelistss
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 200  -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_0_100     -i $INPUTDIR_BKG/4_WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt    -y 1.213784 -x 48917.48 -z 100   --tag WJetsToLNu_HT-0To100
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 200  -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_100_200   -i $INPUTDIR_BKG/5_WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -y 1.213784 -x 1345
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 200  -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_200_400   -i $INPUTDIR_BKG/6_WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -y 1.213784 -x 359.7
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 200  -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_400_600   -i $INPUTDIR_BKG/7_WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt  -y 1.213784 -x 48.91 
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 200  -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_600_800   -i $INPUTDIR_BKG/8_WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -y 1.213784 -x 12.05
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 200  -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_800_1200  -i $INPUTDIR_BKG/9_WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -y 1.213784 -x 5.501
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 200  -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_1200_2500 -i $INPUTDIR_BKG/10_WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -y 1.213784 -x 1.329 
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -n 200  -Y 2018 -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_2500_Inf  -i $INPUTDIR_BKG/11_WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -y 1.213784 -x 0.03216

#  
#
## # #####################
##### ELECTROWEAK # FIXME - obsolete filelists
# XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
#
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_EWKWPlus2Jets_WToLNu   -x 29.59 -i $INPUTDIR_BKG/27_EWKWPlus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_EWKWMinus2Jets_WToLNu  -x 23.24 -i $INPUTDIR_BKG/28_EWKWMinus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 200  -o $SKIMDIR/$OUTDIRR/SKIM_EWKZ2Jets_ZToLL        -x 3.987 -i $INPUTDIR_BKG/25_EWKZ2Jets_ZToLL_M-50_TuneCP5_PSweights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt
#
#
#
#
# # #####################
##### single top : # FIXME - obsolete filelists
# XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
#python scripts/skimNtuple.py  -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 120  -o $SKIMDIR/$OUTDIRR/SKIM_ST_tW_antitop        -x 35.85  -i $INPUTDIR_BKG/21_ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1.txt           
#python scripts/skimNtuple.py  -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 120  -o $SKIMDIR/$OUTDIRR/SKIM_ST_tW_top            -x 35.85  -i $INPUTDIR_BKG/22_ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1.txt              
#python scripts/skimNtuple.py  -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 120  -o $SKIMDIR/$OUTDIRR/SKIM_ST_tchannel_antitop  -x 80.95  -i $INPUTDIR_BKG/23_ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt  
#python scripts/skimNtuple.py  -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ST_tchannel_top      -x 136.02 -i $INPUTDIR_BKG/t-channel_antitop.txt

## # #####################
##### SM Higgs :  # FIXME - obsolete filelists
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

#python scripts/skimNtuple.py  -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -Y 2018 -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ZH_HBB_ZLL    -x 0.08781 -y 0.577  -i $INPUTDIR_BKG/29_ZH_HToBB_ZToLL_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt 
#python scripts/skimNtuple.py  -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -Y 2018 -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ZH_HBB_ZQQ    -x 0.6079  -y 0.577  -i $INPUTDIR_BKG/ZH_HToBB_ZToQQ_M125.txt 
#python scripts/skimNtuple.py  -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -Y 2018 -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ZH_HTauTau    -x 0.880   -y 0.0632 -i $INPUTDIR_BKG/32_ZHToTauTau_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt 
#python scripts/skimNtuple.py  -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -Y 2018 -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ggHTauTau     -x 48.61   -y 0.0632 -i $INPUTDIR_BKG/33_GluGluHToTauTau_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt 
#python scripts/skimNtuple.py  -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -Y 2018 -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_WplusHTauTau  -x 0.831   -y 0.0632 -i $INPUTDIR_BKG/35_WplusHToTauTau_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt   
#python scripts/skimNtuple.py  -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -Y 2018 -n 50   -o $SKIMDIR/$OUTDIRR/SKIM_VBFHTauTau    -x 3.766   -y 0.0632 -i $INPUTDIR_BKG/34_VBFHToTauTau_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1.txt   
#python scripts/skimNtuple.py  -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -Y 2018 -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_WminusHTauTau -x 0.527   -y 0.0632 -i $INPUTDIR_BKG/36_WminusHToTauTau_M125_13TeV_powheg_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt 
#python scripts/skimNtuple.py  -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -Y 2018 -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ttHJetTononBB -x 0.5071 -y 0.577   -i $INPUTDIR_BKG/37_ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt 
#python scripts/skimNtuple.py  -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -Y 2018 -n 100  -o $SKIMDIR/$OUTDIRR/SKIM_ttHJetToBB    -x 0.5071 -y 0.423   -i $INPUTDIR_BKG/38_ttHJetTobb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   

### # #####################
###### Others :  # FIXME - obsolete filelists
##

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 20 -k True -o $SKIMDIR/SKIM_TTWJetsToLNu  -i $INPUTDIR_BKG/TTWJetsToLNu.txt -x 0.196  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 20 -k True -o $SKIMDIR/SKIM_TTWJetsToQQ   -i $INPUTDIR_BKG/TTWJetsToQQ.txt  -x 0.4062 
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 20 -k True -o $SKIMDIR/SKIM_TTWW          -i $INPUTDIR_BKG/TTWW.txt         -x 0.006979 
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 20 -k True -o $SKIMDIR/SKIM_TTWZ          -i $INPUTDIR_BKG/TTWZ.txt         -x 0.002441 
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 20 -k True -o $SKIMDIR/SKIM_TTWH          -i $INPUTDIR_BKG/TTWH.txt         -x 0.00158
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 20 -k True -o $SKIMDIR/SKIM_TTZH          -i $INPUTDIR_BKG/TTWH.txt         -x 0.00154
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 20 -k True -o $SKIMDIR/TTZToLLNuNu        -i $INPUTDIR_BKG/TTZToLLNuNu.txt  -x 0.207  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 20 -k True -o $SKIMDIR/TTZToQQ            -i $INPUTDIR_BKG/TTZToQQ.txt      -x 0.5297
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 20 -k True -o $SKIMDIR/TTZZ               -i $INPUTDIR_BKG/TTZZ.txt         -x 0.001386 


## # ##################### 
## # ### Multiboson: # FIXME - obsolete filelists
# Some XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
# Some other XS taken from http://cms.cern.ch/iCMS/jsp/db_notes/noteInfo.jsp?cmsnoteid=CMS%20AN-2019/111

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg   -Y 2018 -n 50  -k True -o $SKIMDIR/SKIM_ZZTo4L      -i $INPUTDIR_BKG/ZZTo4L.txt      -x 1.26   
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg   -Y 2018 -n 50  -k True -o $SKIMDIR/SKIM_ZZTo2L2Nu   -i $INPUTDIR_BKG/ZZTo2L2Nu.txt   -x 0.564 
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg   -Y 2018 -n 50  -k True -o $SKIMDIR/SKIM_ZZTo2L2Q    -i $INPUTDIR_BKG/ZZTo2L2Q.txt    -x 3.38   
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg   -Y 2018 -n 50  -k True -o $SKIMDIR/SKIM_ZZTo2Q2Nu   -i $INPUTDIR_BKG/ZZTo2Q2Nu.txt   -x 4.04   
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg   -Y 2018 -n 50  -k True -o $SKIMDIR/SKIM_WZTo3LNu    -i $INPUTDIR_BKG/WZTo3LNu.txt    -x 4.43   
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg   -Y 2018 -n 50  -k True -o $SKIMDIR/SKIM_WZTo1L1Nu2Q -i $INPUTDIR_BKG/WZTo1L1Nu2Q.txt -x 10.71  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg   -Y 2018 -n 50  -k True -o $SKIMDIR/SKIM_WZTo1L3Nu   -i $INPUTDIR_BKG/WZTo1L3Nu.txt   -x 3.033  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg   -Y 2018 -n 50  -k True -o $SKIMDIR/SKIM_WZTo2L2Q    -i $INPUTDIR_BKG/WZTo2L2Q.txt    -x 5.52  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg   -Y 2018 -n 50  -k True -o $SKIMDIR/SKIM_WWTo2L2Nu   -i $INPUTDIR_BKG/WWTo2L2Nu.txt   -x 12.18  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg   -Y 2018 -n 50  -k True -o $SKIMDIR/SKIM_WWToLNuQQ   -i $INPUTDIR_BKG/WWToLNuQQ.txt   -x 50.00  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg   -Y 2018 -n 50  -k True -o $SKIMDIR/SKIM_WWTo4Q      -i $INPUTDIR_BKG/WWTo4Q.txt      -x 51.72  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg   -Y 2018 -n 50  -k True -o $SKIMDIR/SKIM_ZZZ         -i $INPUTDIR_BKG/ZZZ.txt         -x 0.0147 
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg   -Y 2018 -n 50  -k True -o $SKIMDIR/SKIM_WZZ         -i $INPUTDIR_BKG/WZZ.txt         -x 0.057  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg   -Y 2018 -n 50  -k True -o $SKIMDIR/SKIM_WWW         -i $INPUTDIR_BKG/WWW.txt         -x 0.209  
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg   -Y 2018 -n 50  -k True -o $SKIMDIR/SKIM_WWZ         -i $INPUTDIR_BKG/WWZ.txt         -x 0.168  


#####
# Signals ggF non res - filelists up to date
# norm xs = 1 p
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_GGHH_SM      -i $INPUTDOR_SIG/12_GluGluToHHTo2B2Tau_node_SM_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_GGHH_node_2  -i $INPUTDOR_SIG/4_GluGluToHHTo2B2Tau_node_2_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_GGHH_node_3  -i $INPUTDOR_SIG/5_GluGluToHHTo2B2Tau_node_3_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_GGHH_node_4  -i $INPUTDOR_SIG/2_GluGluToHHTo2B2Tau_node_4_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_GGHH_node_5  -i $INPUTDOR_SIG/3_GluGluToHHTo2B2Tau_node_5_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_GGHH_node_6  -i $INPUTDOR_SIG/4_GluGluToHHTo2B2Tau_node_6_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_GGHH_node_7  -i $INPUTDOR_SIG/9_GluGluToHHTo2B2Tau_node_7_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_GGHH_node_8  -i $INPUTDOR_SIG/10_GluGluToHHTo2B2Tau_node_8_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt  -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_GGHH_node_9  -i $INPUTDOR_SIG/1_GluGluToHHTo2B2Tau_node_9_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_GGHH_node_10 -i $INPUTDOR_SIG/1_GluGluToHHTo2B2Tau_node_10_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt  -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_GGHH_node_11 -i $INPUTDOR_SIG/1_GluGluToHHTo2B2Tau_node_11_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt  -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_GGHH_node_12 -i $INPUTDOR_SIG/3_GluGluToHHTo2B2Tau_node_12_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt  -x 1. -a True


#####
# Signals ggF non res  - filelists up to date
# norm xs = 1 p
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_GGHH_NLO_SM       -i $INPUTDOR_SIG/2_GluGluToHHTo2B2Tau_node_cHHH1_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt    -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_GGHH_NLO_cHHH0    -i $INPUTDOR_SIG/1_GluGluToHHTo2B2Tau_node_cHHH0_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt    -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_GGHH_NLO_cHHH2p45 -i $INPUTDOR_SIG/3_GluGluToHHTo2B2Tau_node_cHHH2p45_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_GGHH_NLO_cHHH5    -i $INPUTDOR_SIG/4_GluGluToHHTo2B2Tau_node_cHHH5_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt    -x 1. -a True

#####
# Signals VBF non res - filelists up to date
# norm xs = 1 pb
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_VBFHH_CV_1_C2V_1_C3_1 -i $INPUTDOR_SIG/1_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_VBF_CV_0_5_C2V_1_C3_1 -i $INPUTDOR_SIG/2_VBFHHTo2B2Tau_CV_0_5_C2V_1_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_VBF_CV_1_5_C2V_1_C3_1 -i $INPUTDOR_SIG/3_VBFHHTo2B2Tau_CV_1_5_C2V_1_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_VBFHH_CV_1_C2V_1_C3_0 -i $INPUTDOR_SIG/4_VBFHHTo2B2Tau_CV_1_C2V_1_C3_0_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_VBFHH_CV_1_C2V_1_C3_2 -i $INPUTDOR_SIG/5_VBFHHTo2B2Tau_CV_1_C2V_1_C3_2_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg  -n 100   -k True -o $SKIMDIR/SKIM_VBFHH_CV_1_C2V_2_C3_1 -i $INPUTDOR_SIG/6_VBFHHTo2B2Tau_CV_1_C2V_2_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True








