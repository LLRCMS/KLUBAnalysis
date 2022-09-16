#!/usr/bin/env bash

### Defaults
DRYRUN="0"
NOSIG="0"
NODATA="0"
TAG=""
CHANNEL=""
SELECTION="baseline"
CHANNEL_CHOICES=( "ETau" "MuTau" "TauTau" )
DATA_PERIOD="UL18"
DATA_PERIOD_CHOICES=( "UL16" "UL17" "UL18" )
REG="SR"  # A:SR , B:SStight , C:OSinviso, D:SSinviso, B': SSrlx
REG_CHOICES=( "SR" "SStight" "OSinviso" "SSinviso" "SSrlx" )
EOS_USER="bfontana"

### Argument parsing
HELP_STR="Prints this help message."
CHANNEL_STR="(String) Which channel to consider: ETau, MuTau, TauTau. Defaults to '${CHANNEL}'."
SELECTION_STR="(String) Which selection to consider. Defaults to '${SELECTION}'."
DRYRUN_STR="(Boolean) Prints all the commands to be launched but does not launch them. Defaults to ${DRYRUN}."
TAG_STR="(String) Defines tag for the output. Defaults to '${TAG}'."
DATAPERIOD_STR="(String) Which data period to consider: Legacy18, UL18, ... Defaults to '${DATA_PERIOD}'."
REG_STR="(String) Which region to consider: A: SR, B: SStight, C: OSinviso, D: SSinviso, B': SSrlx. Defaults to '${REG}'."
NOSIG_STR="(Boolean) Do not include signal samples. Defaults to '${NOSIG}'."
NODATA_STR="(Boolean) Do not include data samples. Defaults to '${NODATA}'."
function print_usage_submit_skims {
    USAGE=" $(basename "$0")
	-h / --help			[ ${HELP_STR} ]
	--dry-run			[ ${DRYRUN_STR} ]
	-c / --channel      [ ${CHANNEL_STR} ]
	-s / --selection    [ ${CHANNEL_STR} ]
	-t / --tag			[ ${TAG_STR} ]
	-r / --region		[ ${REG_STR} ]
    -d / --data_period  [ ${DATAPERIOD_STR} ]
	--nosig             [ ${NOSIG} ]
	--nodata            [ ${NODATA} ]

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
	-s|--selection)
	    SELECTION=${2}
	    shift; shift;
	    ;;
	--dry-run)
	    DRYRUN="1"
	    shift;
	    ;;
	--no-sig)
	    NOSIG="1"
	    shift;
	    ;;
	--no-data)
	    NODATA="1"
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
function array_contains () { 
    local array="$1[@]"
    local seeking=$2
    local in=1
    for element in "${!array}"; do
        if [[ ${element} == "${seeking}" ]]; then
            in=0
            break
        fi
    done
    return $in
}

declare -a ALL_TAGS=( $(/bin/ls -d ${MAIN_DIR}/*/ | tr '\n' '\0' | xargs -0 -n 1 basename) )
if [[ -z ${TAG} ]]; then
    printf "Select the tag via the '--tag' option. "
    if [ ${#ALL_TAGS[@]} -ne 0 ]; then
		echo "The following tags are currently available:"
		for tag in ${ALL_TAGS[@]}; do
			echo "- ${tag}"
		done
    else
		echo "No tags are currently available. You cannot produce plots without input histograms!"
    fi
    exit 1;
fi
if ! array_contains ALL_TAGS ${TAG}; then
	echo "Tag ${TAG} was not found."
	exit 1
fi

if [[ -z ${DATA_PERIOD} ]]; then
	echo "Select the data period via the '-d / --data_period' option."
	exit 1;
fi
if [[ -z ${CHANNEL} ]]; then
	echo "Select the channel via the '-c / --channel' option."
	exit 1;
fi
if [[ -z ${REG} ]]; then
	echo "Select the region via the '-r / --region' option."
	exit 1;
fi

LUMI="41.6"
MAIN_DIR="${MAIN_DIR}/${TAG}"
EOS_DIR="/eos/user/${EOS_USER:0:1}/${EOS_USER}"
WWW_DIR="${EOS_DIR}/www/${PLOTS_DIR}/${TAG}/${CHANNEL}"
WWW_SUBDIR="${WWW_DIR}/${SELECTION}_${REG}"

[[ ! -d ${EOS_DIR} ]] && /opt/exp_soft/cms/t3/eos-login -username ${EOS_USER} -init

if [ ${DATA_PERIOD} == "UL16" ]; then
	PLOTTER="scripts/makeFinalPlots_UL2016.py"
elif [ ${DATA_PERIOD} == "UL17" ]; then
	PLOTTER="scripts/makeFinalPlots_UL2017.py"
elif [ ${DATA_PERIOD} == "UL18" ]; then
	PLOTTER="scripts/makeFinalPlots_UL2018.py"
fi


### Argument parsing: information for the user
echo "------ Arguments --------------"
printf "DRYRUN\t\t= ${DRYRUN}\n"
printf "TAG\t\t\t= ${TAG}\n"
printf "DATA_PERIOD\t= ${DATA_PERIOD}\n"
printf "CHANNEL\t\t= ${CHANNEL}\n"
printf "REGION\t\t= ${REG}\n"
printf "SELECTION\t= ${SELECTION}\n"
printf "EOS_USER\t= ${EOS_USER}\n"
printf "NOSIG\t\t= ${NOSIG}\n"
printf "NODATA\t\t= ${NODATA}\n"
echo "-------------------------------"

### Ensure connection to /eos/ folder
[[ ! -d ${EOS_DIR} ]] && /opt/exp_soft/cms/t3/eos-login -username ${EOS_USER} -init

OPTIONS="--quit --ratio " # --sigscale 10 10" "--no-binwidth"
if [[ ${NOSIG} -eq 1 ]]; then
	OPTIONS+="--no-sig "
fi
if [[ ${NODATA} -eq 1 ]]; then
	OPTIONS+="--no-data "
fi

OUTDIR="${MAIN_DIR}/${PLOTS_DIR}"
FULL_OUTDIR="${MAIN_DIR}/${PLOTS_DIR}/${CHANNEL}/${SELECTION}_${REG}"
mkdir -p "${FULL_OUTDIR}"

function run() {
	[[ ${DRYRUN} -eq 1 ]] && echo "[DRYRUN] $@" || "$@"
}

function run_plot() {
	comm="python ${PLOTTER} --indir ${MAIN_DIR} --outdir ${OUTDIR} "
	comm+="--reg ${REG} "
	comm+="--sel ${SELECTION} --channel ${CHANNEL} "
	comm+="--lumi ${LUMI} ${OPTIONS} --quit $@"
	[[ ${DRYRUN} -eq 1 ]] && echo "[DRYRUN] ${comm}" || ${comm}
}

declare -A VAR_MAP
VAR_MAP=(
	["dau1_pt"]="p_{T}(1)\s[GeV]"
	["dau2_pt"]="p_{T}(2)\s[GeV]"
	["dau1_eta"]="#eta(1)"
	["dau2_eta"]="#eta(2)"
	["dau1_phi"]="#phi(1)"
	["dau2_phi"]="#phi(2)"

	["ditau_deltaR"]="#Delta(R)(tau,\stau)"

	["bjet1_bID_deepFlavor"]="ID(bjet1)"
	["bjet2_bID_deepFlavor"]="ID(bjet2)"

	["tauH_pt"]="p_{T}(H[#tau\s#tau])\s[GeV]"
	["bH_mass"]="m(H\[bb])\s[GeV]"
	["bH_pt"]="p_{T}(H[bb])\s[GeV]"
	["HH_mass"]="m(HH)\s[GeV]"
	["HH_pt"]="p_{T}(HH)\s[GeV]"
	["HHKin_mass"]="m(HH)_{KinFit}\s[GeV]"

	["bjet1_pt"]="p_{T}(j1)\s[GeV]"
	["bjet2_pt"]="#eta(j2)\s[GeV]"
	["bjet1_eta"]="#eta(j1)"
	["bjet2_eta"]="#eta(j2)"
)

# parallel python ${PLOTTER} --indir ${MAIN_DIR} --outdir ${OUTDIR} --reg ${REG} --sel ${SELECTION} --channel ${CHANNEL} --lumi ${LUMI} ${OPTIONS} --quit --var {1} --lymin 0.7 --label {2} ::: ${!VAR_MAP[@]} ::: ${VAR_MAP[@]}
for avar in ${!VAR_MAP[@]}; do
 	run_plot --var ${avar} --lymin 0.7 --label ${VAR_MAP[$avar]}
done

run mkdir -p ${WWW_DIR}
if [ -d ${WWW_SUBDIR} ]; then
	echo "removing"
	run rm -rf ${WWW_SUBDIR}
fi

run mkdir ${WWW_SUBDIR}

run cp ${FULL_OUTDIR}/*png ${WWW_SUBDIR}
run cp ${FULL_OUTDIR}/*pdf ${WWW_SUBDIR}

echo "Results: https://${EOS_USER}.web.cern.ch/${EOS_USER}/${PLOTS_DIR}/${TAG}/${CHANNEL}/${SELECTION}_${REG}/"
