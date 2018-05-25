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
#echo "----- BEGIN -----" >> log_skim.txt

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

#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 10 -o $SKIMDIR/SKIM_TT_fullyHad_2017F -i $INPUTDIR/2_TTToHadronic_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 380.1143 -t True -b 1 -q longcms

#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 10 -o $SKIMDIR/SKIM_TT_semiLep_2017F -i $INPUTDIR/3_TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 364.3108 -t True -b 5 -q longcms

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 20 -o $SKIMDIR/SKIM_TT_fullyLep_2017B  -i $INPUTDIR/1_TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 87.3348  -t True -b 4 -q shortcms

<<COMMENT1

# # ####################
### DATA :
echo "Submitting - DATA tau - "
echo "Submitting - DATA tau - " >> log.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 10 -k False -o $SKIMDIR/SKIM_Tau_2017B -i $INPUTDIR/5_Tau__Run2017B-31Mar2018-v1.txt -q longcms

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 10 -k False -o $SKIMDIR/SKIM_Tau_2017F -i $INPUTDIR/6_Tau__Run2017F-31Mar2018-v1.txt -q longcms


echo "Submitting - DATA Muon - "
echo "Submitting - DATA Muon - " >> log.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 10 -k False -o $SKIMDIR/SKIM_SingleMuon_2017B -i $INPUTDIR/1_SingleMuon__Run2017B-31Mar2018-v1.txt -q longcms

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 10 -k False -o $SKIMDIR/SKIM_SingleMuon_2017F -i $INPUTDIR/2_SingleMuon__Run2017F-31Mar2018-v1.txt -q longcms


echo "Submitting - DATA Electron - "
echo "Submitting - DATA Electron - " >> log.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 10 -k False -o $SKIMDIR/SKIM_SingleElectron_2017B -i $INPUTDIR/3_SingleElectron__Run2017B-31Mar2018-v1.txt -q longcms

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 20 -k False -o $SKIMDIR/SKIM_SingleElectron_2017F -i $INPUTDIR/4_SingleElectron__Run2017F-31Mar2018-v1.txt -q longcms


COMMENT1



