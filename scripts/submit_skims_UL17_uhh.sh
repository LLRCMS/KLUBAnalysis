#!/usr/bin/env bash

#
# setup
#

THIS_FILE="$( [ ! -z "$ZSH_VERSION" ] && echo "${(%):-%x}" || echo "${BASH_SOURCE[0]}" )"
THIS_DIR="$( cd "$( dirname "$THIS_FILE" )" && pwd )"
KLUB_DIR="$( cd "$( dirname "$THIS_DIR" )" && pwd )"

OUTDIR="SKIMS_uhh_2017_v4_02Mar23"
INDIR="${KLUB_DIR}/inputFiles"
PUDIR="/nfs/dust/cms/user/kramerto/hbt_static_files/KLUBAnalysis/weights/PUreweight/UL_Run2_PU_SF/2017"
SKIMDIR="/nfs/dust/cms/user/$( whoami )/hbt_resonant_run2/HHSkims"

STITCHING_ON="1"
DRYRUN="0"
RESUBMIT="0"
LIST_SCRIPT="scripts/makeListOnStorage_uhh.py"
IN_TAG="uhh_2017_v4"

# setup
source "$THIS_DIR/setup.sh" ""
mkdir -p "$SKIMDIR/$OUTDIR"
touch $SKIMDIR/$OUTDIR/README.txt
declare -A DATA_MAP


#
# helpers
#

produce_list() {
    local comm="python ${KLUB_DIR}/${LIST_SCRIPT} --tag ${IN_TAG} --write"
    [ "${RESUBMIT}" != "0" ] && comm="${comm} --dry"
    eval "${comm} $@"
}

run_skim() {
    local kind="$1"
    local skimdir="$2"
    local sample="$3"
    local campaign="*"

    # get the campaign for data
    if [ "${kind}" = "data" ]; then
        local data_parts=($( echo "${sample//__/ }" ))
        if [ "${#data_parts[@]}" != "2" ]; then
            2>&1 echo "wrong format for data sample ${sample}"
            return "1"
        fi
        sample="${data_parts[0]}"
        campaign="${data_parts[1]}*"
    fi

    # produce the file list
    local file_list="$( produce_list --sample "${sample}" --kind "${kind}" --campaign "\"${campaign}\"" )"
    if [ ! -f "${file_list}" ]; then
        2>&1 echo "created file list does not exist '${file_list}'"
        return "2"
    fi

    # get additional arguments
    local args=${@:4}

    # build the command and run it
    local comm="python ${KLUB_DIR}/scripts/skimNtuple_uhh.py -i ${file_list} -o ${SKIMDIR}/${OUTDIR}/${skimdir} -c ${KLUB_DIR}/config/skim_UL17_uhh.cfg -T ${OUTDIR} -s True -k True --pu ${PUDIR}/PU_UL2017_SF.txt ${args}"
    [ "${DRYRUN}" != "0" ] && echo "${comm}" || eval "${comm}"
}


#
# run skims
#

##############################
### SKIMS UL 2017, SIGNALS ###
##############################

run_skim "signal" "SKIM_GGHH_SM" "GluGluToHHTo2B2Tau_TuneCP5_PSWeights_node_SM_13TeV-madgraph-pythia8" -a True -n 20 --rt 5

run_skim "signal" "SKIM_ggF_BulkGraviton_m250"  "GluGluToBulkGravitonToHHTo2B2Tau_M-250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m260"  "GluGluToBulkGravitonToHHTo2B2Tau_M-260_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m270"  "GluGluToBulkGravitonToHHTo2B2Tau_M-270_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m280"  "GluGluToBulkGravitonToHHTo2B2Tau_M-280_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m300"  "GluGluToBulkGravitonToHHTo2B2Tau_M-300_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m320"  "GluGluToBulkGravitonToHHTo2B2Tau_M-320_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m350"  "GluGluToBulkGravitonToHHTo2B2Tau_M-350_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m400"  "GluGluToBulkGravitonToHHTo2B2Tau_M-400_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m450"  "GluGluToBulkGravitonToHHTo2B2Tau_M-450_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m500"  "GluGluToBulkGravitonToHHTo2B2Tau_M-500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m550"  "GluGluToBulkGravitonToHHTo2B2Tau_M-550_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m600"  "GluGluToBulkGravitonToHHTo2B2Tau_M-600_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m650"  "GluGluToBulkGravitonToHHTo2B2Tau_M-650_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m700"  "GluGluToBulkGravitonToHHTo2B2Tau_M-700_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m750"  "GluGluToBulkGravitonToHHTo2B2Tau_M-750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m800"  "GluGluToBulkGravitonToHHTo2B2Tau_M-800_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m850"  "GluGluToBulkGravitonToHHTo2B2Tau_M-850_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m900"  "GluGluToBulkGravitonToHHTo2B2Tau_M-900_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m1000" "GluGluToBulkGravitonToHHTo2B2Tau_M-1000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m1250" "GluGluToBulkGravitonToHHTo2B2Tau_M-1250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m1500" "GluGluToBulkGravitonToHHTo2B2Tau_M-1500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m1750" "GluGluToBulkGravitonToHHTo2B2Tau_M-1750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m2000" "GluGluToBulkGravitonToHHTo2B2Tau_M-2000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m2500" "GluGluToBulkGravitonToHHTo2B2Tau_M-2500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_BulkGraviton_m3000" "GluGluToBulkGravitonToHHTo2B2Tau_M-3000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5

run_skim "signal" "SKIM_ggF_Radion_m250"  "GluGluToRadionToHHTo2B2Tau_M-250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m260"  "GluGluToRadionToHHTo2B2Tau_M-260_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m270"  "GluGluToRadionToHHTo2B2Tau_M-270_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m280"  "GluGluToRadionToHHTo2B2Tau_M-280_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m300"  "GluGluToRadionToHHTo2B2Tau_M-300_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m320"  "GluGluToRadionToHHTo2B2Tau_M-320_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m350"  "GluGluToRadionToHHTo2B2Tau_M-350_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m400"  "GluGluToRadionToHHTo2B2Tau_M-400_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m450"  "GluGluToRadionToHHTo2B2Tau_M-450_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m500"  "GluGluToRadionToHHTo2B2Tau_M-500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m550"  "GluGluToRadionToHHTo2B2Tau_M-550_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m600"  "GluGluToRadionToHHTo2B2Tau_M-600_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m650"  "GluGluToRadionToHHTo2B2Tau_M-650_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m700"  "GluGluToRadionToHHTo2B2Tau_M-700_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m750"  "GluGluToRadionToHHTo2B2Tau_M-750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m800"  "GluGluToRadionToHHTo2B2Tau_M-800_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m850"  "GluGluToRadionToHHTo2B2Tau_M-850_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m900"  "GluGluToRadionToHHTo2B2Tau_M-900_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m1000" "GluGluToRadionToHHTo2B2Tau_M-1000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m1250" "GluGluToRadionToHHTo2B2Tau_M-1250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m1500" "GluGluToRadionToHHTo2B2Tau_M-1500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m1750" "GluGluToRadionToHHTo2B2Tau_M-1750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m2000" "GluGluToRadionToHHTo2B2Tau_M-2000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m2500" "GluGluToRadionToHHTo2B2Tau_M-2500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_ggF_Radion_m3000" "GluGluToRadionToHHTo2B2Tau_M-3000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5

run_skim "signal" "SKIM_VBF_BulkGraviton_m250"  "VBFToBulkGravitonToHHTo2B2Tau_M-250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m260"  "VBFToBulkGravitonToHHTo2B2Tau_M-260_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m270"  "VBFToBulkGravitonToHHTo2B2Tau_M-270_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m280"  "VBFToBulkGravitonToHHTo2B2Tau_M-280_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m300"  "VBFToBulkGravitonToHHTo2B2Tau_M-300_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m320"  "VBFToBulkGravitonToHHTo2B2Tau_M-320_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m350"  "VBFToBulkGravitonToHHTo2B2Tau_M-350_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m400"  "VBFToBulkGravitonToHHTo2B2Tau_M-400_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m450"  "VBFToBulkGravitonToHHTo2B2Tau_M-450_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m500"  "VBFToBulkGravitonToHHTo2B2Tau_M-500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m550"  "VBFToBulkGravitonToHHTo2B2Tau_M-550_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m600"  "VBFToBulkGravitonToHHTo2B2Tau_M-600_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m650"  "VBFToBulkGravitonToHHTo2B2Tau_M-650_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m700"  "VBFToBulkGravitonToHHTo2B2Tau_M-700_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m750"  "VBFToBulkGravitonToHHTo2B2Tau_M-750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m800"  "VBFToBulkGravitonToHHTo2B2Tau_M-800_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m850"  "VBFToBulkGravitonToHHTo2B2Tau_M-850_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m900"  "VBFToBulkGravitonToHHTo2B2Tau_M-900_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m1000" "VBFToBulkGravitonToHHTo2B2Tau_M-1000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m1250" "VBFToBulkGravitonToHHTo2B2Tau_M-1250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m1500" "VBFToBulkGravitonToHHTo2B2Tau_M-1500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m1750" "VBFToBulkGravitonToHHTo2B2Tau_M-1750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m2000" "VBFToBulkGravitonToHHTo2B2Tau_M-2000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m2500" "VBFToBulkGravitonToHHTo2B2Tau_M-2500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_BulkGraviton_m3000" "VBFToBulkGravitonToHHTo2B2Tau_M-3000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5

run_skim "signal" "SKIM_VBF_Radion_m250"  "VBFToRadionToHHTo2B2Tau_M-250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m260"  "VBFToRadionToHHTo2B2Tau_M-260_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m270"  "VBFToRadionToHHTo2B2Tau_M-270_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m280"  "VBFToRadionToHHTo2B2Tau_M-280_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m300"  "VBFToRadionToHHTo2B2Tau_M-300_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m320"  "VBFToRadionToHHTo2B2Tau_M-320_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m350"  "VBFToRadionToHHTo2B2Tau_M-350_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m400"  "VBFToRadionToHHTo2B2Tau_M-400_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m450"  "VBFToRadionToHHTo2B2Tau_M-450_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m500"  "VBFToRadionToHHTo2B2Tau_M-500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m550"  "VBFToRadionToHHTo2B2Tau_M-550_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m600"  "VBFToRadionToHHTo2B2Tau_M-600_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m650"  "VBFToRadionToHHTo2B2Tau_M-650_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m700"  "VBFToRadionToHHTo2B2Tau_M-700_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m750"  "VBFToRadionToHHTo2B2Tau_M-750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m800"  "VBFToRadionToHHTo2B2Tau_M-800_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m850"  "VBFToRadionToHHTo2B2Tau_M-850_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m900"  "VBFToRadionToHHTo2B2Tau_M-900_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8"  -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m1000" "VBFToRadionToHHTo2B2Tau_M-1000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m1250" "VBFToRadionToHHTo2B2Tau_M-1250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m1500" "VBFToRadionToHHTo2B2Tau_M-1500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m1750" "VBFToRadionToHHTo2B2Tau_M-1750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m2000" "VBFToRadionToHHTo2B2Tau_M-2000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m2500" "VBFToRadionToHHTo2B2Tau_M-2500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5
run_skim "signal" "SKIM_VBF_Radion_m3000" "VBFToRadionToHHTo2B2Tau_M-3000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8" -a True -n 20 --rt 5


##################################
### SKIMS UL 2017, BACKGROUNDS ###
##################################

#run_skim "background" "SKIM_DY_incl"               "DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"                         -x 6077.22 --DY True -g ${STITCHING_ON} -n 300 --rt 4
#run_skim "background" "SKIM_DY_HT70to100"          "DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8"               --DY True -g ${STITCHING_ON} -n 300 --rt 4
#run_skim "background" "SKIM_DY_HT100to200"         "DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8"              --DY True -g ${STITCHING_ON} -n 300 --rt 4
#run_skim "background" "SKIM_DY_HT200to400"         "DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8"              --DY True -g ${STITCHING_ON} -n 300 --rt 4
#run_skim "background" "SKIM_DY_HT400to600"         "DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8"              --DY True -g ${STITCHING_ON} -n 300 --rt 4
#run_skim "background" "SKIM_DY_HT600to800"         "DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8"              --DY True -g ${STITCHING_ON} -n 300 --rt 4
#run_skim "background" "SKIM_DY_HT800to1200"        "DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8"             --DY True -g ${STITCHING_ON} -n 300 --rt 4
#run_skim "background" "SKIM_DY_HT1200to2500"       "DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8"            --DY True -g ${STITCHING_ON} -n 300 --rt 4
#run_skim "background" "SKIM_DY_HT2500toInf"        "DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8"             --DY True -g ${STITCHING_ON} -n 300 --rt 4
#run_skim "background" "SKIM_DY_1j"                 "DY1JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8"               --nj 1 --DY True -g ${STITCHING_ON} -n 300 --rt 4
#run_skim "background" "SKIM_DY_2j"                 "DY2JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8"               --nj 2 --DY True -g ${STITCHING_ON} -n 300 --rt 4
#run_skim "background" "SKIM_DY_3j"                 "DY3JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8"               --nj 3 --DY True -g ${STITCHING_ON} -n 300 --rt 4
#run_skim "background" "SKIM_DY_4j"                 "DY4JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8"               --nj 4 --DY True -g ${STITCHING_ON} -n 300 --rt 4


# NLO DY x-secs taken from XSDB and multiplied by k-factor from NLO to NNLO: 6077.22 [1] / 6404.0 [2]
# [1] NNLO x-sec for inclusive DYJetsToLL_M-50 sample taken from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat13TeV
# [2] https://cms-gen-dev.cern.ch/xsdb/?searchQuery=DAS%3DDYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8
run_skim "background" "SKIM_DY_amc_incl"           "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8"                               -x 6077.22  --DY 0 -g ${STITCHING_ON} -n 300 --rt 4
run_skim "background" "SKIM_DY_amc_PtZ_0To50"      "DYJetsToLL_LHEFilterPtZ-0To50_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8"    -x 1409.22  --DY 0 -g ${STITCHING_ON} -n 300 --rt 4
run_skim "background" "SKIM_DY_amc_PtZ_50To100"    "DYJetsToLL_LHEFilterPtZ-50To100_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8"  -x 377.12   --DY 0 -g ${STITCHING_ON} -n 300 --rt 4
run_skim "background" "SKIM_DY_amc_PtZ_100To250"   "DYJetsToLL_LHEFilterPtZ-100To250_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8" -x 92.24    --DY 0 -g ${STITCHING_ON} -n 300 --rt 4
run_skim "background" "SKIM_DY_amc_PtZ_250to400"   "DYJetsToLL_LHEFilterPtZ-250To400_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8" -x 3.512    --DY 0 -g ${STITCHING_ON} -n 300 --rt 4
run_skim "background" "SKIM_DY_amc_PtZ_400to650"   "DYJetsToLL_LHEFilterPtZ-400To650_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8" -x 0.4826   --DY 0 -g ${STITCHING_ON} -n 300 --rt 4
run_skim "background" "SKIM_DY_amc_PtZ_650toInf"   "DYJetsToLL_LHEFilterPtZ-650ToInf_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8" -x 0.04487  --DY 0 -g ${STITCHING_ON} -n 300 --rt 4
run_skim "background" "SKIM_DY_amc_0j"             "DYJetsToLL_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8"                                 -x 4867.28  --DY 0 -g ${STITCHING_ON} -n 300 --rt 4
run_skim "background" "SKIM_DY_amc_1j"             "DYJetsToLL_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8"                                 -x 902.95   --DY 0 -g ${STITCHING_ON} -n 300 --rt 4
run_skim "background" "SKIM_DY_amc_2j"             "DYJetsToLL_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8"                                 -x 342.96   --DY 0 -g ${STITCHING_ON} -n 300 --rt 4

run_skim "background" "SKIM_WJets_HT0To70"         "WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"                                 -x 48917.48 -y 1.213784 -z 70 -n 20 --rt 4
run_skim "background" "SKIM_WJets_HT70To100"       "WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8"                      -x 1362     -y 1.213784       -n 20 --rt 4
run_skim "background" "SKIM_WJets_HT100To200"      "WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8"                     -x 1345     -y 1.213784       -n 20 --rt 4
run_skim "background" "SKIM_WJets_HT200To400"      "WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8"                     -x 359.7    -y 1.213784       -n 20 --rt 4
run_skim "background" "SKIM_WJets_HT400To600"      "WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8"                     -x 48.91    -y 1.213784       -n 20 --rt 4
run_skim "background" "SKIM_WJets_HT600To800"      "WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8"                     -x 12.05    -y 1.213784       -n 20 --rt 4
run_skim "background" "SKIM_WJets_HT800To1200"     "WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8"                    -x 5.501    -y 1.213784       -n 20 --rt 4
run_skim "background" "SKIM_WJets_HT1200To2500"    "WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8"                   -x 1.329    -y 1.213784       -n 20 --rt 4
run_skim "background" "SKIM_WJets_HT2500ToInf"     "WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8"                    -x 0.03216  -y 1.213784       -n 20 --rt 4

run_skim "background" "SKIM_EWKWPlus2Jets_WToLNu"  "EWKWPlus2Jets_WToLNu_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8"    -x 25.62 -n 100 --rt 4
run_skim "background" "SKIM_EWKWMinus2Jets_WToLNu" "EWKWMinus2Jets_WToLNu_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8"   -x 20.25 -n 100 --rt 4
run_skim "background" "SKIM_EWKZ2Jets_ZToLL"       "EWKZ2Jets_ZToLL_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8"         -x 3.987 -n 100 --rt 4

run_skim "background" "SKIM_ST_tW_antitop"         "ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"                -x 35.85  -n 80 --rt 4
run_skim "background" "SKIM_ST_tW_top"             "ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"                    -x 35.85  -n 80 --rt 4
run_skim "background" "SKIM_ST_tchannel_antitop"   "ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8" -x 80.95  -n 80 --rt 4
run_skim "background" "SKIM_ST_tchannel_top"       "ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8"     -x 136.02 -n 80 --rt 4

run_skim "background" "SKIM_TT_fullyHad"           "TTToHadronic_TuneCP5_13TeV-powheg-pythia8"                                    -x 377.96 -t True -b 1 -n 100 --rt 4
run_skim "background" "SKIM_TT_fullyLep"           "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"                                       -x 88.29  -t True -b 4 -n 100 --rt 4
run_skim "background" "SKIM_TT_semiLep"            "TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8"                                -x 365.34 -t True -b 5 -n 100 --rt 4
run_skim "background" "SKIM_TTWJetsToLNu"          "TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8"                      -x 0.2043              -n 20  --rt 4
run_skim "background" "SKIM_TTWJetsToQQ"           "TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8"                       -x 0.4062              -n 20  --rt 4
run_skim "background" "SKIM_TTZToLLNuNu"           "TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8"                              -x 0.2529              -n 20  --rt 4
run_skim "background" "SKIM_TTWW"                  "TTWW_TuneCP5_13TeV-madgraph-pythia8"                                          -x 0.006979            -n 20  --rt 4
run_skim "background" "SKIM_TTZZ"                  "TTZZ_TuneCP5_13TeV-madgraph-pythia8"                                          -x 0.001386            -n 20  --rt 4
run_skim "background" "SKIM_TTWZ"                  "TTWZ_TuneCP5_13TeV-madgraph-pythia8"                                          -x 0.00158             -n 20  --rt 4

run_skim "background" "SKIM_WW"                    "WW_TuneCP5_13TeV-pythia8"                                                     -x 118.7   -n 20 --rt 4
run_skim "background" "SKIM_WZ"                    "WZ_TuneCP5_13TeV-pythia8"                                                     -x 47.13   -n 20 --rt 4
run_skim "background" "SKIM_ZZ"                    "ZZ_TuneCP5_13TeV-pythia8"                                                     -x 16.523  -n 20 --rt 4
run_skim "background" "SKIM_WWW"                   "WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8"                                        -x 0.2086  -n 20 --rt 4
run_skim "background" "SKIM_WWZ"                   "WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8"                                        -x 0.1651  -n 20 --rt 4
run_skim "background" "SKIM_WZZ"                   "WZZ_TuneCP5_13TeV-amcatnlo-pythia8"                                           -x 0.05565 -n 20 --rt 4
run_skim "background" "SKIM_ZZZ"                   "ZZZ_TuneCP5_13TeV-amcatnlo-pythia8"                                           -x 0.01398 -n 20 --rt 4

run_skim "background" "SKIM_GluGluHToTauTau"       "GluGluHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8"                            -x 48.58  -y 0.0627              -n 30 --rt 4
run_skim "background" "SKIM_VBFHToTauTau"          "VBFHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8"                               -x 3.782  -y 0.0627              -n 30 --rt 4
run_skim "background" "SKIM_ZHToTauTau"            "ZHToTauTau_M125_CP5_13TeV-powheg-pythia8_ext1"                                -x 0.884  -y 0.0627              -n 30 --rt 4
run_skim "background" "SKIM_WplusHToTauTau"        "WplusHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8"                             -x 0.831  -y 0.0627              -n 30 --rt 4
run_skim "background" "SKIM_WminusHToTauTau"       "WminusHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8"                            -x 0.527  -y 0.0627              -n 30 --rt 4
run_skim "background" "SKIM_ttHToNonbb"            "ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8"                                 -x 0.5071 -y 0.3549 --ttHToNonBB -n 30 --rt 4
run_skim "background" "SKIM_ttHTobb"               "ttHTobb_M125_TuneCP5_13TeV-powheg-pythia8"                                    -x 0.5071 -y 0.5824              -n 30 --rt 4
run_skim "background" "SKIM_ttHToTauTau"           "ttHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8"                                -x 0.5071 -y 0.0627              -n 30 --rt 4


# ###########################
# ### SKIMS UL 2017, DATA ###
# ###########################

run_skim "data" "SKIM_SingleMuon_Run2017B" "SingleMuon__Run2017B" -d True -n 50 --rt 24
run_skim "data" "SKIM_SingleMuon_Run2017C" "SingleMuon__Run2017C" -d True -n 50 --rt 24
run_skim "data" "SKIM_SingleMuon_Run2017D" "SingleMuon__Run2017D" -d True -n 50 --rt 24
run_skim "data" "SKIM_SingleMuon_Run2017E" "SingleMuon__Run2017E" -d True -n 50 --rt 24
run_skim "data" "SKIM_SingleMuon_Run2017F" "SingleMuon__Run2017F" -d True -n 50 --rt 24

run_skim "data" "SKIM_EGamma_Run2017B" "SingleElectron__Run2017B" -d True -n 50 --rt 24
run_skim "data" "SKIM_EGamma_Run2017C" "SingleElectron__Run2017C" -d True -n 50 --rt 24
run_skim "data" "SKIM_EGamma_Run2017D" "SingleElectron__Run2017D" -d True -n 50 --rt 24
run_skim "data" "SKIM_EGamma_Run2017E" "SingleElectron__Run2017E" -d True -n 50 --rt 24
run_skim "data" "SKIM_EGamma_Run2017F" "SingleElectron__Run2017F" -d True -n 50 --rt 24

run_skim "data" "SKIM_Tau_Run2017B" "Tau__Run2017B" -d True -n 50 --rt 24
run_skim "data" "SKIM_Tau_Run2017C" "Tau__Run2017C" -d True -n 50 --rt 24
run_skim "data" "SKIM_Tau_Run2017D" "Tau__Run2017D" -d True -n 50 --rt 24
run_skim "data" "SKIM_Tau_Run2017E" "Tau__Run2017E" -d True -n 50 --rt 24
run_skim "data" "SKIM_Tau_Run2017F" "Tau__Run2017F" -d True -n 50 --rt 24

run_skim "data" "SKIM_MET_Run2017B" "MET__Run2017B" -d True -n 50 --rt 24
run_skim "data" "SKIM_MET_Run2017C" "MET__Run2017C" -d True -n 50 --rt 24
run_skim "data" "SKIM_MET_Run2017D" "MET__Run2017D" -d True -n 50 --rt 24
run_skim "data" "SKIM_MET_Run2017E" "MET__Run2017E" -d True -n 50 --rt 24
run_skim "data" "SKIM_MET_Run2017F" "MET__Run2017F" -d True -n 50 --rt 24
