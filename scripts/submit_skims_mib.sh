OUTDIRR="Skims2017_Dec18"

INPUTDIR_VBF="inputFiles/VBF_study"
INPUTDIR="inputFiles/JECproduction_Lug2017"
SKIMDIR="/gwteraz/users/brivio/SKIMMED_VBF"

source /cvmfs/cms.cern.ch/cmsset_default.sh
source scripts/setup.sh
mkdir /gwteraz/users/brivio/SKIMMED_VBF/$OUTDIRR/
mkdir $OUTDIRR
cp /gwpool/users/brivio/Hhh_1718/CMSSW_7_4_7/src/KLUBAnalysis/scripts/haddAll.sh /gwteraz/users/brivio/SKIMMED_VBF/$OUTDIRR/


echo "----- BEGIN -----" >> log.txt

# VBF signals
echo "Submitting - VBF SIGNALS - "
echo "Submitting - VBF SIGNALS - " >> log.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log.txt

python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 1 -k True -o $SKIMDIR/$OUTDIRR/VBFHHTo2B2Tau_111 -i $INPUTDIR_VBF/1_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 1.0 -a True

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 2 -k True -o $SKIMDIR/$OUTDIRR/VBFHHTo2B2Tau_121 -i $INPUTDIR_VBF/2_VBFHHTo2B2Tau_CV_1_C2V_2_C3_1_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 1.0 -a True
