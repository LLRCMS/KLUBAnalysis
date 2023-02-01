AMESSAGE="V2 2018 Legacy ntuples"

OUTDIRR="SKIMS_1June2020"

INPUTDIR_BKG="inputFiles/Legacy2018_backgrounds"
INPUTDIR_DATA="inputFiles/Legacy2018_data"
INPUTDIR_SIG="inputFiles/Legacy2018_signals"

PUDIR="weights/PUreweight/Legacy_Run2_PU_SF/2018"
SKIMDIR="/sps/cms/portales/HHLegacy_2018_v2"

mkdir -p $SKIMDIR/$OUTDIRR/
touch $SKIMDIR/$OUTDIRR/README.txt
echo $AMESSAGE > $SKIMDIR/$OUTDIRR/README.txt
cp scripts/listAll.sh $SKIMDIR/$OUTDIRR/

#########################
### SKIMS LEGACY 2018 ###
#########################

python scripts/skimNtuple_cc.py -T $OUTDIRR -s True -c config/skim_Legacy2018.cfg -Y 2018 -n 100 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIRR/SKIM_TTZToQQ      -x 0.5297   -i $INPUTDIR_BKG/11_TTZToQQ_TuneCP5_13TeV-amcatnlo-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1.txt
