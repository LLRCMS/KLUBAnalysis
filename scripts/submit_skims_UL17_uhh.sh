#!/usr/bin/env bash

THIS_FILE="$( [ ! -z "$ZSH_VERSION" ] && echo "${(%):-%x}" || echo "${BASH_SOURCE[0]}" )"
THIS_DIR="$( cd "$( dirname "$THIS_FILE" )" && pwd )"
KLUB_DIR="$( cd "$( dirname "$THIS_DIR" )" && pwd )"

OUTDIR="SKIMS_uhh_2017_v2_08Apr22"
INDIR="${KLUB_DIR}/inputFiles"
PUDIR="/nfs/dust/cms/user/kramerto/hbt_static_files/KLUBAnalysis/weights/PUreweight/UL_Run2_PU_SF/2017"
SKIMDIR="/nfs/dust/cms/user/$( whoami )/hbt_resonant_run2/HHSkims"

# setup
source "$THIS_DIR/setup.sh"
mkdir -p "$SKIMDIR/$OUTDIR"
touch $SKIMDIR/$OUTDIR/README.txt
cp "$THIS_DIR/listAll.sh" "$SKIMDIR/$OUTDIR"

run_skim() {
    python scripts/skimNtuple_uhh.py -T $OUTDIR -s True -c "${KLUB_DIR}/config/skim_UL17_uhh.cfg" -k "True" --pu "$PUDIR/PU_UL2017_SF.txt" "$@"
}

# test
# run_skim -o "$SKIMDIR/$OUTDIR/SKIM_GluGluTest" -x 88.29 -t False -b 0 -n 1 -i "$INDIR/UL17_signals/GluGluToBulkGravitonToHHTo2B2Tau_M-1000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"


##############################
### SKIMS UL 2017, SIGNALS ###
##############################

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_GGHH_SM"                -a True -n 20 -rt 4 -i "GluGluToHHTo2B2Tau_TuneCP5_PSWeights_node_SM_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m250"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m300"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-300_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m320"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-320_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m260"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-260_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m270"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-270_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m280"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-280_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m350"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-350_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m400"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-400_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m450"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-450_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m500"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m550"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-550_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m600"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-600_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m650"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-650_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m700"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-700_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m750"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m800"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-800_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m850"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-850_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m900"  -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-900_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m1000" -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-1000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m1250" -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-1250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m1500" -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-1500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m1750" -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-1750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m2000" -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-2000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m2500" -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-2500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m3000" -a True -n 20 -rt 4 -i "GluGluToBulkGravitonToHHTo2B2Tau_M-3000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m250"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m260"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-260_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m270"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-270_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m280"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-280_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m300"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-300_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m320"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-320_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m350"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-350_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m400"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-400_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m450"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-450_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m500"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m550"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-550_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m600"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-600_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m650"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-650_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m700"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-700_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m750"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m800"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-800_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m850"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-850_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m900"        -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-900_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m1000"       -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-1000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m1250"       -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-1250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m1500"       -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-1500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m1750"       -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-1750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m2000"       -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-2000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m2500"       -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-2500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m3000"       -a True -n 20 -rt 4 -i "GluGluToRadionToHHTo2B2Tau_M-3000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m250"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m260"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-260_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m270"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-270_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m280"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-280_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m300"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-300_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m320"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-320_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m350"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-350_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m400"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-400_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m450"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-450_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m500"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m550"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-550_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m600"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-600_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m650"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-650_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m700"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-700_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m750"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m800"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-800_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m850"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-850_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m900"  -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-900_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m1000" -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-1000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m1250" -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-1250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m1500" -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-1500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m1750" -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-1750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m2000" -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-2000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m2500" -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-2500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m3000" -a True -n 20 -rt 4 -i "VBFToBulkGravitonToHHTo2B2Tau_M-3000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m250"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m260"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-260_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m270"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-270_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m280"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-280_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m300"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-300_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m320"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-320_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m350"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-350_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m400"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-400_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m450"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-450_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m500"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m550"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-550_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m600"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-600_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m650"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-650_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m700"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-700_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m750"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m800"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-800_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m850"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-850_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m900"        -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-900_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m1000"       -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-1000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m1250"       -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-1250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m1500"       -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-1500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m1750"       -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-1750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m2000"       -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-2000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m2500"       -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-2500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m3000"       -a True -n 20 -rt 4 -i "VBFToRadionToHHTo2B2Tau_M-3000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"


##################################
### SKIMS UL 2017, BACKGROUNDS ###
##################################

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_DY_incl"               -x 6077.22 --DY True        -g True  -n 300 -rt 4 -i "DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_DY_HT70to100"                     --DY True        -g True  -n 300 -rt 4 -i "DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_DY_HT100to200"                    --DY True        -g True  -n 300 -rt 4 -i "DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_DY_HT200to400"                    --DY True        -g True  -n 300 -rt 4 -i "DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_DY_HT400to600"                    --DY True        -g True  -n 300 -rt 4 -i "DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_DY_HT600to800"                    --DY True        -g True  -n 300 -rt 4 -i "DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_DY_HT800to1200"                   --DY True        -g True  -n 300 -rt 4 -i "DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_DY_HT1200to2500"                  --DY True        -g True  -n 300 -rt 4 -i "DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_DY_HT2500toInf"                   --DY True        -g True  -n 300 -rt 4 -i "DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_DY_1j"                            --DY True --nj 1 -g True  -n 300 -rt 4 -i "DY1JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_DY_2j"                            --DY True --nj 2 -g True  -n 300 -rt 4 -i "DY2JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_DY_3j"                            --DY True --nj 3 -g True  -n 300 -rt 4 -i "DY3JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_DY_4j"                            --DY True --nj 4 -g True  -n 300 -rt 4 -i "DY4JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_WJets_HT0To70"         -x 48917.48 -z 70 -y 1.213784        -n 20  -rt 4 -i "WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_WJets_HT70To100"       -x 1362           -y 1.213784        -n 20  -rt 4 -i "WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_WJets_HT100To200"      -x 1345           -y 1.213784        -n 20  -rt 4 -i "WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_WJets_HT200To400"      -x 359.7          -y 1.213784        -n 20  -rt 4 -i "WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_WJets_HT400To600"      -x 48.91          -y 1.213784        -n 20  -rt 4 -i "WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_WJets_HT600To800"      -x 12.05          -y 1.213784        -n 20  -rt 4 -i "WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_WJets_HT800To1200"     -x 5.501          -y 1.213784        -n 20  -rt 4 -i "WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_WJets_HT1200To2500"    -x 1.329          -y 1.213784        -n 20  -rt 4 -i "WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_WJets_HT2500ToInf"     -x 0.03216        -y 1.213784        -n 20  -rt 4 -i "WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_EWKWPlus2Jets_WToLNu"  -x 25.62                             -n 100 -rt 4 -i "EWKWPlus2Jets_WToLNu_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_EWKWMinus2Jets_WToLNu" -x 20.25                             -n 100 -rt 4 -i "EWKWMinus2Jets_WToLNu_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_EWKZ2Jets_ZToLL"       -x 3.987                             -n 100 -rt 4 -i "EWKZ2Jets_ZToLL_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ST_tW_antitop"         -x 35.85                             -n 80  -rt 4 -i "ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ST_tW_top"             -x 35.85                             -n 80  -rt 4 -i "ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ST_tchannel_antitop"   -x 80.95                             -n 80  -rt 4 -i "ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ST_tchannel_top"       -x 136.02                            -n 80  -rt 4 -i "ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_TT_fullyHad"           -x 377.96 -t True -b 1               -n 100 -rt 4 -i "TTToHadronic_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_TT_fullyLep"           -x 88.29  -t True -b 1               -n 100 -rt 4 -i "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_TT_semiLep"            -x 365.34 -t True -b 1               -n 100 -rt 4 -i "TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_TTWJetsToLNu"          -x 0.2043                            -n 20  -rt 4 -i "TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_TTWJetsToQQ"           -x 0.4062                            -n 20  -rt 4 -i "TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_TTZToLLNuNu"           -x 0.2529                            -n 20  -rt 4 -i "TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_TTWW"                  -x 0.006979                          -n 20  -rt 4 -i "TTWW_TuneCP5_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_TTZZ"                  -x 0.001386                          -n 20  -rt 4 -i "TTZZ_TuneCP5_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_TTWZ"                  -x 0.00158                           -n 20  -rt 4 -i "TTWZ_TuneCP5_13TeV-madgraph-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_WW"                    -x 118.7                             -n 20  -rt 4 -i "WW_TuneCP5_13TeV-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_WZ"                    -x 47.13                             -n 20  -rt 4 -i "WZ_TuneCP5_13TeV-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ZZ"                    -x 16.523                            -n 20  -rt 4 -i "ZZ_TuneCP5_13TeV-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_WWW"                   -x 0.2086                            -n 20  -rt 4 -i "WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9_ext1-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_WWZ"                   -x 0.1651                            -n 20  -rt 4 -i "WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_WZZ"                   -x 0.05565                           -n 20  -rt 4 -i "WZZ_TuneCP5_13TeV-amcatnlo-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9_ext1-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ZZZ"                   -x 0.01398                           -n 20  -rt 4 -i "ZZZ_TuneCP5_13TeV-amcatnlo-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9_ext1-v2.txt"

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_GluGluHToTauTau"       -x 48.58  -y 0.0627                  -n 30  -rt 4 -i "GluGluHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v3.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_VBFHToTauTau"          -x 3.782  -y 0.0627                  -n 30  -rt 4 -i "VBFHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ZHToTauTau"            -x 0.884  -y 0.0627                  -n 30  -rt 4 -i "ZHToTauTau_M125_CP5_13TeV-powheg-pythia8_ext1__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_WplusHToTauTau"        -x 0.831  -y 0.0627                  -n 30  -rt 4 -i "WplusHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_WminusHToTauTau"       -x 0.527  -y 0.0627                  -n 30  -rt 4 -i "WminusHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ttHToNonbb"            -x 0.5071 -y 0.4176 --ttHToNonBB     -n 30  -rt 4 -i "ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ttHTobb"               -x 0.5071 -y 0.5824                  -n 30  -rt 4 -i "ttHTobb_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_ttHToTauTau"           -x 0.5071 -y 0.0627                  -n 30  -rt 4 -i "ttHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v3.txt"


###########################
### SKIMS UL 2017, DATA ###
###########################

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_SingleMuon_Run2017B" -d True -n 10 -rt 4 -i "SingleMuon__Run2017B-UL2017_MiniAODv2-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_SingleMuon_Run2017C" -d True -n 10 -rt 4 -i "SingleMuon__Run2017C-UL2017_MiniAODv2-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_SingleMuon_Run2017D" -d True -n 10 -rt 4 -i "SingleMuon__Run2017D-UL2017_MiniAODv2-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_SingleMuon_Run2017E" -d True -n 10 -rt 4 -i "SingleMuon__Run2017E-UL2017_MiniAODv2-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_SingleMuon_Run2017F" -d True -n 10 -rt 4 -i "SingleMuon__Run2017F-UL2017_MiniAODv2-v1.txt"

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_EGamma_Run2017B"     -d True -n 10 -rt 4 -i "SingleElectron__Run2017B-UL2017_MiniAODv2-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_EGamma_Run2017C"     -d True -n 10 -rt 4 -i "SingleElectron__Run2017C-UL2017_MiniAODv2-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_EGamma_Run2017D"     -d True -n 10 -rt 4 -i "SingleElectron__Run2017D-UL2017_MiniAODv2-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_EGamma_Run2017E"     -d True -n 10 -rt 4 -i "SingleElectron__Run2017E-UL2017_MiniAODv2-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_EGamma_Run2017F"     -d True -n 10 -rt 4 -i "SingleElectron__Run2017F-UL2017_MiniAODv2-v1.txt"

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_Tau_Run2017B"        -d True -n 10 -rt 4 -i "Tau__Run2017B-UL2017_MiniAODv2-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_Tau_Run2017C"        -d True -n 10 -rt 4 -i "Tau__Run2017C-UL2017_MiniAODv2-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_Tau_Run2017D"        -d True -n 10 -rt 4 -i "Tau__Run2017D-UL2017_MiniAODv2-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_Tau_Run2017E"        -d True -n 10 -rt 4 -i "Tau__Run2017E-UL2017_MiniAODv2-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_Tau_Run2017F"        -d True -n 10 -rt 4 -i "Tau__Run2017F-UL2017_MiniAODv2-v1.txt"

run_skim -o "$SKIMDIR/$OUTDIR/SKIM_MET_Run2017B"        -d True -n 10 -rt 4 -i "MET__Run2017B-UL2017_MiniAODv2-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_MET_Run2017C"        -d True -n 10 -rt 4 -i "MET__Run2017C-UL2017_MiniAODv2-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_MET_Run2017D"        -d True -n 10 -rt 4 -i "MET__Run2017D-UL2017_MiniAODv2-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_MET_Run2017E"        -d True -n 10 -rt 4 -i "MET__Run2017E-UL2017_MiniAODv2-v1.txt"
run_skim -o "$SKIMDIR/$OUTDIR/SKIM_MET_Run2017F"        -d True -n 10 -rt 4 -i "MET__Run2017F-UL2017_MiniAODv2-v1.txt"
