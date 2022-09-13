#!/usr/bin/env bash

### Defaults
DRYRUN="0"
TAG=""
CHANNEL=""
CHANNEL_CHOICES=( "ETau" "MuTau" "TauTau" )
DATA_PERIOD="UL18"
DATA_PERIOD_CHOICES=( "UL16" "UL17" "UL18" )
REG="SR"  # A:SR , B:SStight , C:OSinviso, D:SSinviso, B': SSrlx
REG_CHOICES=( "SR" "SStight" "OSinviso" "SSinviso" "SSrlx" )
EOS_USER="bfontana"

### Argument parsing
HELP_STR="Prints this help message."
CHANNEL_STR="(String) Which channel to consider: ETau, MuTau, TauTau. Defaults to '${CHANNEL}'."
DRYRUN_STR="(Boolean) Prints all the commands to be launched but does not launch them. Defaults to ${DRYRUN}."
TAG_STR="(String) Defines tag for the output. Defaults to '${TAG}'."
DATAPERIOD_STR="(String) Which data period to consider: Legacy18, UL18, ... Defaults to '${DATA_PERIOD}'."
REG_STR="(String) Which region to consider: A: SR, B: SStight, C: OSinviso, D: SSinviso, B': SSrlx. Defaults to '${REG}'."
function print_usage_submit_skims {
    USAGE=" $(basename "$0") [-H] [--dry-run -t -r -c -d -n --klub_tag --stitching_on]

	-h / --help			[ ${HELP_STR} ]
	-c / --channel      [ ${CHANNEL_STR} ]
	--dry-run			[ ${DRYRUN_STR} ]
	-t / --tag			[ ${TAG_STR} ]
	-r / --region		[ ${REG_STR} ]
    -d / --data_period  [ ${DATAPERIOD_STR} ]

    Run example: bash $(basename "$0") -t <some_tag>
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
	-c|--channel)
		CHANNEL=${2}
		if [[ ! " ${CHANNEL_CHOICES[*]} " =~ " ${CHANNEL} " ]]; then
			echo "Currently the following channels are supported:"
			for ch in ${CHANNEL_CHOICES[@]}; do
				echo "- ${ch}" # bash string substitution
			done
			exit 1;
		fi
		shift; shift;
		;;
	--dry-run)
	    DRYRUN="1"
	    shift;
	    ;;
	-t|--tag)
	    TAG=${2}
	    shift; shift;
	    ;;
	-d|--data_period)
	    DATA_PERIOD=${2}
		if [[ ! " ${DATA_PERIOD_CHOICES[*]} " =~ " ${DATA_PERIOD} " ]]; then
			echo "Currently the following data periods are supported:"
			for dp in ${DATA_PERIOD_CHOICES[@]}; do
				echo "- ${dp}" # bash string substitution
			done
			exit 1;
		fi
	    shift; shift;
	    ;;
	--eos)
	    EOS_USER="$2"
	    shift # past argument
	    shift # past value
	    ;;
	-r|--region)
	    REG=${2}
		if [[ ! " ${REG_CHOICES[*]} " =~ " ${REG} " ]]; then
			echo "Currently the following regions are supported:"
			for rg in ${REG_CHOICES[@]}; do
				echo "- ${rg}" # bash string substitution
			done
			exit 1;
		fi
	    shift; shift;
	    ;;
	*)  # unknown option
	    echo "Wrong parameter ${1}."
	    exit 1
	    ;;
    esac
done

### Setup variables
PLOTS_DIR="HH_Plots"
MAIN_DIR="/data_CMS/cms/${USER}/HHresonant_hist"

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
if [[ -z ${DATA_PERIOD} ]]; then
	echo "Select the data period via the '-d / --data_period' option."
	exit 1;
fi
if [[ -z ${CHANNEL} ]]; then
	echo "Select the channel via the '-c / --channeld' option."
	exit 1;
fi
if [[ -z ${REG} ]]; then
	echo "Select the region via the '-r / --region' option."
	exit 1;
fi

MAIN_DIR="${MAIN_DIR}/${TAG}"
EOS_DIR="/eos/user/${EOS_USER:0:1}/${EOS_USER}"
WWW_DIR="${EOS_DIR}/www/${PLOTS_DIR}/${TAG}/${CHANNEL}/"
WWW_SUBDIR="${WWW_DIR}/${SELECTION}_${REG}"

if [ ${DATA_PERIOD} == "UL16" ]; then
	PLOTTER="scripts/makeFinalPlots_UL2016.py"
elif [ ${DATA_PERIOD} == "UL17" ]; then
	PLOTTER="scripts/makeFinalPlots_UL2017.py"
elif [ ${DATA_PERIOD} == "UL18" ]; then
	PLOTTER="scripts/makeFinalPlots_UL2018.py"
fi

SELECTION="baseline55"
LUMI="41.6"

### Argument parsing: information for the user
echo "------ Arguments --------------"
echo " Passed by the user:"
printf "DRYRUN\t\t\t= ${DRYRUN}\n"
printf "TAG\t\t\t= ${TAG}\n"
printf "DATA_PERIOD\t\t= ${DATA_PERIOD}\n"
printf "CHANNEL\t\t= ${CHANNEL}\n"
printf "REGION\t\t= ${REG}\n"
echo "-------------------------------"

### Ensure connection to /eos/ folder
[[ ! -d ${EOS_DIR} ]] && /opt/exp_soft/cms/t3/eos-login -username ${EOS_USER} -init

OPTIONS="--quit --ratio --no-sig" # --sigscale 10 10"
#others="--no-binwidth"

mkdir ${MAIN_DIR}
mkdir "${MAIN_DIR}/${SELECTION}_${REG}"

if [[ ${CHANNEL} = "ETau" ]]; then
    OBJ1="e"
    OBJ2="#tau_{h}"
elif [[ ${CHANNEL} == "MuTau" ]]; then
    OBJ1="#mu"
    OBJ2="#tau_{h}"
elif [[ ${CHANNEL} == "TauTau" ]]; then
    OBJ1="#tau_{h1}"
    OBJ2="#tau_{h2}"
fi
echo ${OBJ1}
echo ${OBJ2}

function run_plot() {
	comm="python ${PLOTTER} --dir ${MAIN_DIR} --reg ${REG} "
	comm+="--sel ${SELECTION} --channel ${CHANNEL} "
	comm+="--lumi ${LUMI} --tag ${TAG} ${OPTIONS} --quit $@"
	[[ ${DRYRUN} -eq 1 ]] && echo ${comm} || ${comm}
}

declare -A VAR_MAP
VAR_MAP=(
	["VBFjj_mass"]="m_{jj} [GeV]"
	["VBFjet1_pt"]="p_{TVBFjet1} [GeV]"
	["VBFjet2_pt"]="p_{TVBFjet2} [GeV]"
	["VBFjj_mass"]="m_{jj} [GeV]"
	["VBFjet1_pt"]="p_{TVBFjet1} [GeV]"
	["VBFjet2_pt"]="p_{TVBFjet2} [GeV]"
	)
for avar in ${!VAR_MAP[@]}; do
	run_plot --var ${avar} --lymin 0.7 --label ${VAR_MAP[$avar]}
done

if [[ ${DRYRUN} -eq 0 ]]; then
	cd ${MAIN_DIR}

	mkdir ${WWW_DIR}
	if [ -d ${WWW_SUBDIR} ]; then
		echo "removing"
		rm -rf ${WWW_SUBDIR}
	fi

	mkdir ${WWW_SUBDIR}

	cp "${MAIN_DIR}/${SELECTION}_${REG}/*" ${WWW_SUBDIR}

	printf '\e]8;;https://${EOS_USER}.web.cern.ch/${EOS_USER}/${PLOTS_DIR}/\e\\Results\e]8;;\e\\\n'
fi
