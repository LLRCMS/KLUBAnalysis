OUTDIRR="Skims_Fall17_MC"
#OUTDIRR="Skims_Fall17_DATA"

#INPUTDIR="inputFiles/JECproduction_Lug2017"
#INPUTDIR="inputFiles/Fall17_MC/"
#INPUTDIR="/gwpool/users/brivio/Hhh_1718/syncFeb2018/CMSSW_7_4_7/src/KLUBAnalysis/skimNtuples"
INPUTDIR="inputFiles/Fall17_MC"
#INPUTDIR="inputFiles/Fall17_DATA"

#SKIMDIR="/gwteraz/users/brivio/SKIMMED_VBF"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_Fall17_MC"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_syncFeb2018"
SKIMDIR="/gwteraz/users/brivio/SKIMMED_20may2018/SKIMMED_Fall17_MC"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_20may2018/SKIMMED_Fall17_DATA"

source /cvmfs/cms.cern.ch/cmsset_default.sh
source scripts/setup.sh
#mkdir /gwteraz/users/brivio/SKIMMED_Fall17_MC/$OUTDIRR/
mkdir $OUTDIRR
#cp /gwpool/users/brivio/Hhh_1718/CMSSW_7_4_7/src/KLUBAnalysis/scripts/haddAll.sh /gwteraz/users/brivio/SKIMMED_Fall17_MC/$OUTDIRR/



#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync.cfg -n 1  -o $SKIMDIR/SKIM_Graviton450_v3  -i $INPUTDIR/Graviton450_bis.txt -x 1
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync.cfg -n 1  -o $SKIMDIR/SKIM_TauData  -i $INPUTDIR/TauData.txt -x 1 -d True

###################
echo "----- BEGIN -----" >> log_skim.txt

# TT
# TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
# hh = 45.7%
# ll = 10.5%
# hl = 21.9% (x2 for permutation t-tbar)
# technical note: use -q long if splitting on ~180-350 jobs

echo "Submitting - TT - "
echo "Submitting - TT - ">> log_skim.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_skim.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync.cfg -n 80  -o $SKIMDIR/SKIM_TT_fullyHad  -i $INPUTDIR/2_TTToHadronic_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 380.1143 -t True -b 1

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync.cfg -n 80 -o $SKIMDIR/SKIM_TT_semiLep   -i $INPUTDIR/3_TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 364.3108 -t True -b 5

#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync.cfg -n 30 -o $SKIMDIR/SKIM_TT_fullyLep  -i $INPUTDIR/1_TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 87.3348  -t True -b 4

<<COMMENT1

# # ####################
### DATA :
echo "Submitting - DATA tau - "
echo "Submitting - DATA tau - " >> log.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log.txt

python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_3 -i inputFiles/TauData2016/3_Tau__Run2016C-03Feb2017-v1.txt -q shortcms

python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_4 -i inputFiles/TauData2016/4_Tau__Run2016D-03Feb2017-v1.txt -q longcms

python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_5 -i inputFiles/TauData2016/5_Tau__Run2016E-03Feb2017-v1.txt -q shortcms

python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_6 -i inputFiles/TauData2016/6_Tau__Run2016F-03Feb2017-v1.txt -q longcms

python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_7 -i inputFiles/TauData2016/7_Tau__Run2016G-03Feb2017-v1.txt -q shortcms

python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_8 -i inputFiles/TauData2016/8_Tau__Run2016H-03Feb2017_ver2-v1.txt -q longcms

python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg -n 10 -k True -o $SKIMDIR/SKIM_Tau_9 -i inputFiles/TauData2016/9_Tau__Run2016H-03Feb2017_ver3-v1.txt -q longcms


echo "Submitting - DATA Muon - "
echo "Submitting - DATA Muon - " >> log.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log.txt

#python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg -n 1 -k True -o $SKIMDIR/$OUTDIRR/SKIM_SingleMuon -i inputFiles/MuonData2016/SingleMu_7FebreminiAOD_5Mar2017.txt -q shortcms


python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg -n 1 -k True -o $SKIMDIR/$OUTDIRR/SKIM_SingleMuon -i inputFiles/MuonData2016/SingleMu_test.txt -q shortcms



echo "Submitting - DATA Electron - "
echo "Submitting - DATA Electron - " >> log.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log.txt

python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2017.cfg -n 150 -k True -o $SKIMDIR/$OUTDIRR/SKIM_SingleElectron -i inputFiles/ElectronData2016/SingleEle_7FebreminiAOD_5Mar2017.txt -q longcms


COMMENT1
