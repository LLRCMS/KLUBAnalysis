#!/usr/bin/env bash

export PYTHONPATH=$PWD/physicsModels:$PYTHONPATH

### Defaults
ISRESONANT="1"
TAG=""
NOPREP=0
VAR="DNNoutSM_kl_1"
SIGNAL="ggFRadion"
DRYRUN="0"
PERIOD=""
PERIOD_CHOICES=( "UL16" "UL16APV" "UL17" "UL18" )
EOS_USER="bfontana"
PREPOUT="prepared_outLimits.root"
BASEDIR="${HOME}/CMSSW_11_1_9/src/KLUBAnalysis"

declare -a MASSES;
declare -a CHANNELS;
declare -a SELECTIONS;
declare -a IN_TAGS;
declare -a CFG_FILES;

### Argument parsing
HELP_STR="Prints this help message."
TAG_STR="(String) Defines tag for the output. Defaults to '${TAG}'."
VAR_STR="(String) Variable to use for the likelihood fit."
DRYRUN_STR="(Boolean) Whether to run in dry-run mode. Defaults to '${DRYRUN}'."
SIGNAL_STR="(String) Signal sample type."
DATAPERIOD_STR="(String) Which data period to consider: Legacy18, UL18, ... Defaults to '${PERIOD}'."
NORES_STR="(Boolean) Whether to run on non-resonant analysis. Defaults to resonant analysis."
MASSES_STR="(Array of ints) Resonant masses."
CHANNELS_STR="(Array of strings) Channels."
SELECTIONS_STR="(Array of strings) Selections."
INTAGS_STR="(Array of strings) Tags used to produce the histograms. Go one-on-one with the channels used."
BASEDIR_STR="(String) Base directory."
PREP_STR="(String) Preparing histogram output filename."
CFGFILE_STR="(Array of strings) Main configuration files, one per channel."

function print_usage_submit_skims {
    USAGE="
    Run example: bash $(basename "$0") -t <some_tag>

    -h / --help         [${HELP_STR}]
    -c / --channels     [${CHANNELS_STR}]
    -l / --selections   [${SELECTIONS_STR}]
    -i / --in_tags      [${INTAGS_STR}]
    -s / --signal       [${SIGNAL_STR}]
    -t / --tag          [${TAG_STR}]
    -f / --cfg          [${CFGFILE_STR}]
    -m / --masses       [${MASSES_STR}]
    -b / --base         [${BASEDIR_STR}]
    -v / --var          [${VAR_STR}]
    -d / --data_period  [${DATAPERIOD_STR}]
	-p / --prepout      [${PREP_STR}]
    -r / --nonresonant  [${NORES_STR}]
	--noprep            [${NOPREP}]
    --dryrun            [${DRYRUN_STR}]

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
	-l|--selections)
	    sel_flag=0
	    while [ ${sel_flag} -eq 0 ]; do
		if [[ "${2}" =~ ^[-].*$ ]] || [[ "${2}" =~ ^$ ]]; then
		    sel_flag=1
		else
		    SELECTIONS+=(${2});
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
	-f|--cfg)
            cfg_flag=0
            while [ ${cfg_flag} -eq 0 ]; do
                if [[ "${2}" =~ ^[-].*$ ]] || [[ "${2}" =~ ^$ ]]; then
                    cfg_flag=1
                else
                    CFG_FILES+=(${2});
                    shift;
                fi
            done
            shift;
            ;;
	-t|--tag)
	    TAG=${2}
	    shift; shift;
	    ;;
	-p|--prepout)
	    PREPOUT=${2}
	    shift; shift;
	    ;;
	--noprep)
	    NOPREP=1
	    shift;
	    ;;
	-b|--base)
	    BASEDIR=${2}
	    shift; shift;
	    ;;
	-v|--var)
            VAR=${2}
            shift; shift;
            ;;
	-s|--signal)
            SIGNAL=${2}
            shift; shift;
            ;;
	-m|--masses)
	    mass_flag=0
	    while [ ${mass_flag} -eq 0 ]; do
		if [[ "${2}" =~ ^[-].*$ ]] || [[ "${2}" =~ ^$ ]]; then
		    mass_flag=1
		else
		    MASSES+=(${2});
		    shift;
		fi
	    done
            shift;
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
if [ ${#MASSES[@]} -eq 0 ]; then
    MASSES=("250" "260" "270" "280" "300" "320" "350" "400" "450" "500" "550" "600" "650" "700" "750" "800" "850" "900" "1000" "1250" "1500" "1750" "2000" "2500" "3000")
fi
if [[ ${#CHANNELS[@]} -eq 0 ]]; then
    echo "Select the channels via the '-c / --channel' option."
    exit 1;
fi
if [ ${#SELECTIONS[@]} -eq 0 ]; then
    SELECTIONS=("s1b1jresolvedMcut" "s2b0jresolvedMcut" "sboostedLLMcut")
fi
if [[ ${#IN_TAGS[@]} -eq 0 ]]; then
    echo "Select the input tags via the '-i / --in_tags' option."
    exit 1;
fi
if [[ ${#CFG_FILES[@]} -eq 0 ]]; then
    echo "Select the main configuration files via the '-f / --cfg' option."
    exit 1;
fi

LIMIT_DIR="${BASEDIR}/resonantLimits"
CFG_DIR="${BASEDIR}/config"

# prepare histograms (remove negative bins and/or scale systematics)
if [[ ${NOPREP} -eq 0 ]]; then
	echo "Preparing the histograms..."
	declare -a COMMS_PREP;
	for ichn in "${!CHANNELS[@]}"; do
		HISTDIR="${MAIN_DIR}/${IN_TAGS[${ichn}]}"
		comm_tmp="python prepare_histos.py -f ${HISTDIR}/combined_outLimits.root -o ${PREPOUT} -c ${CHANNELS[${ichn}]} -y ${PERIOD} -q 0 -B 1"
		COMMS_PREP+=("${comm_tmp}")
	done

	# parallelize over the channels
	[[ ${DRYRUN} -eq 1 ]] && parallel echo {} ::: "${COMMS_PREP[@]}" || parallel -j "${#CHANNELS[@]}" {} ::: "${COMMS_PREP[@]}"
fi

echo "Writing the datacards..."
# write datacards
declare -a COMMS_WRITE;
for ichn in "${!CHANNELS[@]}"; do
	for jsel in "${!SELECTIONS[@]}"; do
		HISTDIR="${MAIN_DIR}/${IN_TAGS[${ichn}]}"
		comm_tmp="python ${LIMIT_DIR}/write_res_card.py -f ${HISTDIR}/prepared_outLimits.root --indir ${LIMIT_DIR} -o ${TAG} -c ${CHANNELS[${ichn}]} -y ${PERIOD} -v ${VAR} -i ${CFG_DIR}/${CFG_FILES[${ichn}]} --selections ${SELECTIONS[${jsel}]} --masses ${MASSES[@]} --signal ${SIGNAL}"
		if [ ${ISRESONANT} -eq 1 ]; then
			COMMS_WRITE+=("${comm_tmp} -r")
		else
			COMMS_WRITE+=("${comm_tmp}")
		fi
	done
done

# parallelize over the channels
[[ ${DRYRUN} -eq 1 ]] && parallel echo {} ::: "${COMMS_WRITE[@]}" || parallel -j 300% {} ::: "${COMMS_WRITE[@]}"

if [ ${DRYRUN} -eq 1 ]; then
    echo "Dry-run. The command above were not run."
fi
