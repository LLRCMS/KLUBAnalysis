OUTDIRR="Skims_Fall17_MC"
#OUTDIRR="Skims_syncFeb2018_v3"

#INPUTDIR="inputFiles/JECproduction_Lug2017"
INPUTDIR="inputFiles/Fall17_MC/"
#INPUTDIR="/gwpool/users/brivio/Hhh_1718/syncFeb2018/CMSSW_7_4_7/src/KLUBAnalysis/skimNtuples"

#SKIMDIR="/gwteraz/users/brivio/SKIMMED_VBF"
SKIMDIR="/gwteraz/users/brivio/SKIMMED_Fall17_MC"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_syncFeb2018"

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

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync.cfg -n 60  -o $SKIMDIR/SKIM_TT_fullyHad  -i $INPUTDIR/3_TTToHadronic_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1.txt -x 380.1143 -t True -b 1

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync.cfg -n 60 -o $SKIMDIR/SKIM_TT_semiLep   -i $INPUTDIR/2_TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1.txt -x 364.3108 -t True -b 5

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync.cfg -n 30 -o $SKIMDIR/SKIM_TT_fullyLep  -i $INPUTDIR/1_TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v2.txt -x 87.3348  -t True -b 4



