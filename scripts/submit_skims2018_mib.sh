# LOG
OUTDIRR="SKIMMED_Legacy2018_14July2020"

# INPUT
INPUTDIR="inputFiles/Legacy2018_backgroundsMIB"
INPUTDIR_DATA="inputFiles/Legacy2018_dataMIB"
INPUTDIR_SIG="inputFiles/Legacy2018_signalsMIB"

# OUTPUT
SKIMDIR="/gwteraz/users/brivio/SKIMMED_Legacy2018_14July2020"

# PU weights
PUDIR="/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/CMSSW_11_1_0_pre6/src/KLUBAnalysis/weights/PUreweight/Legacy_Run2_PU_SF/2018"

# Environment
source /cvmfs/cms.cern.ch/cmsset_default.sh
source scripts/setup.sh
mkdir $OUTDIRR

<<COMMENT1


COMMENT1

# testing 2018 skims
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2018_mib.cfg  -n 1 -k False -o $SKIMDIR/SKIM_TT_fullyHad_TEST -i $INPUTDIR/test1.txt    -x 377.96 -t True -b 1 -q longcms --pu $PUDIR/PU_Legacy2018_SF.txt

