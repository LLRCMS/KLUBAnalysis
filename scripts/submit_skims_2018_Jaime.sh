AMESSAGE="V2 2018 Legacy ntuples"

OUTDIRR="SKIMS_1June2020"

INPUTDIR_BKG="inputFiles/Legacy2018_backgrounds"
INPUTDIR_DATA="inputFiles/Legacy2018_data"
INPUTDIR_SIG="/eos/home-c/camendol/HHLegacy2018/SKIMS_1June2020/"

PUDIR="weights/PUreweight/Legacy_Run2_PU_SF/2018"
SKIMDIR="/eos/home-j/jleonhol/HHbbtautau/"

source scripts/setup.sh
mkdir -p $SKIMDIR/$OUTDIRR/
touch $SKIMDIR/$OUTDIRR/README.txt
echo $AMESSAGE > $SKIMDIR/$OUTDIRR/README.txt
cp scripts/listAll.sh $SKIMDIR/$OUTDIRR/

#########################
### SKIMS LEGACY 2018 ###
#########################

######################
#### Signals VBF non res - filelists up to date
### norm xs = 1 pb
python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_Legacy2018_Jaime.cfg  -n 1 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIRR/SKIM_VBFHH_CV_1_C2V_1_C3_1 -i $INPUTDIR_SIG/1_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
