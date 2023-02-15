#!/usr/bin/env bash

export PYTHONPATH=$PWD/physicsModels:$PYTHONPATH

### Defaults
ISRESONANT="1"
TAG=""
DRYRUN="0"
PERIOD=""
PERIOD_CHOICES=( "UL16" "UL17" "UL18" )
EOS_USER="bfontana"

declare -a CHANNELS;
declare -a IN_TAGS;

### Argument parsing
HELP_STR="Prints this help message."
TAG_STR="(String) Defines tag for the output. Defaults to '${TAG}'."
DRYRUN_STR="(Boolean) Whether to run in dry-run mode. Defaults to '${DRYRUN}'."
DATAPERIOD_STR="(String) Which data period to consider: Legacy18, UL18, ... Defaults to '${PERIOD}'."
NORES_STR="(Boolean) Whether to run on non-resonant analysis. Defaults to resonant analysis."
CHANNELS_STR="(Array of strings) Channels."
INTAGS_STR="(Array of strings) Tags used to produce the histograms. Go one-on-one with the channels used."
function print_usage_submit_skims {
    USAGE="
    Run example: bash $(basename "$0") -t <some_tag>

    -h / --help          [${HELP_STR}]
    -c / --channels      [${CHANNELS_STR}]
    -i / --in_tags       [${INTAGS_STR}]
    -t / --tag           [${TAG_STR}]
    -d / --data_period   [${DATAPERIOD_STR}]
    -r / --nonresonant   [${NORES_STR}]
    --dry_run            [${DRYRUN_STR}]

"
    printf "${USAGE}"
}

while [[ $# -gt 0 ]]; do
    key=${1}
    case $key in
	-h|--help)
	    print_usage_submit_skims
	    exit 1
	    ;;
	-c|--channels)
            chn_flag=0
            while [ ${chn_flag} -eq 0 ]; do
                if [[ "${2}" =~ ^[-].*$ ]] || [[ "${2}" =~ ^$ ]]; then
                    chn_flag=1
                else
                    CHANNELS+=(${2});
                    shift;
                fi
            done
            shift;
            ;;
	-i|--in_tags)
            t_flag=0
            while [ ${t_flag} -eq 0 ]; do
                if [[ "${2}" =~ ^[-].*$ ]] || [[ "${2}" =~ ^$ ]]; then
                    t_flag=1
                else
                    IN_TAGS+=(${2});
                    shift;
                fi
            done
            shift;
            ;;
	-t|--tag)
	    TAG=${2}
	    shift; shift;
	    ;;
	-d|--data_period)
	    PERIOD=${2}
	    if [[ ! " ${PERIOD_CHOICES[*]} " =~ " ${PERIOD} " ]]; then
		echo "Currently the following data periods are supported:"
		for dp in ${PERIOD_CHOICES[@]}; do
		    echo "- ${dp}" # bash string substitution
		done
		exit 1;
	    fi
	    shift; shift;
	    ;;
        -r|--nonresonant)
            ISRESONANT="0"
            shift;
            ;;
	--dryrun)
	    DRYRUN="1"
	    shift;
	    ;;
	*)  # unknown option
	    echo "Wrong parameter ${1}."
	    exit 1
	    ;;
    esac
done

HIST_DIR="HHresonant_hist"
MAIN_DIR="/data_CMS/cms/${USER}/${HIST_DIR}/"

### Argument parsing sanity checks
if [[ -z ${TAG} ]]; then
    printf "Select the tag via the '--tag' option. "
    declare -a tags=( $(/bin/ls -d ${MAIN_DIR}/*/ | tr '\n' '\0' | xargs -0 -n 1 basename) )
    if [ ${#tags[@]} -ne 0 ]; then
	echo "The following tags are currently available:"
	for tag in ${tags[@]}; do
	    echo "- ${tag}"
	    done
    else
	echo "No tags are currently available. Everything looks clean!"
    fi
    exit 1;
fi
if [[ -z ${PERIOD} ]]; then
    echo "Select the data period via the '-d / --data_period' option."
    exit 1;
fi
if [[ ${#CHANNELS[@]} -eq 0 ]]; then
    echo "Select the channels via the '-c / --channel' option."
    exit 1;
fi
if [[ ${#IN_TAGS[@]} -eq 0 ]]; then
    echo "Select the input tags via the '-i / --in_tags' option."
    exit 1;
fi

if [ ${PERIOD} == "UL18" ]; then
    YEAR="2018"
elif [ ${PERIOD} == "UL17" ]; then
    YEAR="2017"
elif [ ${PERIOD} == "UL16" ]; then
    YEAR="2016"
else
  echo "ERROR: Data period ${PERIOD} is not supported! Pick one of the following: ${PERIOD_CHOICES[@]}"
  exit
fi

CFG_DIR="/home/llr/cms/${USER}/CMSSW_11_1_9/src/KLUBAnalysis"
LIMIT_DIR="${CFG_DIR}/resonantLimits"
CFG_DIR="${CFG_DIR}/config"

declare -a COMMS;
for i in "${!CHANNELS[@]}"; do
    HISTDIR="${MAIN_DIR}/${IN_TAGS[$i]}"
    comm_tmp="python ${LIMIT_DIR}/write_res_card.py -f ${HISTDIR}/analyzedOutPlotter.root -o ${TAG} -c ${CHANNELS[$i]} -y ${YEAR} -u 0 -i ${CFG_DIR}/mainCfg_${CHANNELS[$i]}_${PERIOD}.cfg"
    if [ ${ISRESONANT} -eq 1 ]; then
		COMMS+=("${comm_tmp} -r")
	else
		COMMS+=("${comm_tmp}")
    fi
done

# parallelize over the channels
[[ ${DRYRUN} -eq 1 ]] && parallel echo {} ::: "${COMMS[@]}" || parallel {} ::: "${COMMS[@]}"

if [ ${DRYRUN} -eq 1 ]; then
    echo "Dry-run. The command above were not run."
fi

### Most useful options:
# -f : input root file
# -i : input cfg
# -o : output name
# -c : channel
# -y : year
# -b : bin-by-bin unc
# -u : shape unc
# -t : theory unc
# -r : 1 for resonant analysis setup
