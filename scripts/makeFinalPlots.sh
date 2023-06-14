#!/usr/bin/env bash

### Defaults
DRYRUN="0"
NOSIG="0"
NODATA="0"
CHANNEL="ETau"
CHANNEL_CHOICES=("ETau" "MuTau" "TauTau")
SELECTION="baseline"
SELECTION_CHOICES=( "baseline" "baselineInvMcut" "s1b1jresolvedMcut" "s1b1jresolvedInvMcut"
					"s2b0jresolvedMcut" "s2b0jresolvedInvMcut" "sboostedLLMcut" "sboostedLLInvMcut" )
DATA_PERIOD="UL18"
DATA_PERIOD_CHOICES=( "UL16" "UL17" "UL18" )
REG="SR"  # A:SR , B:SStight , C:OSinviso, D:SSinviso, B': SSrlx
REG_CHOICES=( "SR" "SStight" "OSinviso" "SSinviso" )
EOS_USER="bfontana"
CFGFILE="mainCfg_${CHANNEL}_${DATA_PERIOD}.cfg"
COMPARE="0"

declare -a TAGS;

### Argument parsing
HELP_STR="Prints this help message."
CHANNEL_STR="(String) Which channel to consider: ${CHANNEL_CHOICES[@]}. Defaults to '${CHANNEL}'."
SELECTION_STR="(String) Which selection to consider: ${SELECTION_CHOICES[@]}. Defaults to '${SELECTION}'."
DRYRUN_STR="(Boolean) Prints all the commands to be launched but does not launch them. Defaults to ${DRYRUN}."
TAG_STR="(String) Defines tags for the output. Defaults to '${TAGS[0]}'."
DATAPERIOD_STR="(String) Which data period to consider: Legacy18, UL18, ... Defaults to '${DATA_PERIOD}'."
REG_STR="(String) Which region to consider: A: SR, B: SStight, C: OSinviso, D: SSinviso, B': SSrlx. Defaults to '${REG}'."
NOSIG_STR="(Boolean) Do not include signal samples. Defaults to '${NOSIG}'."
NODATA_STR="(Boolean) Do not include data samples. Defaults to '${NODATA}'."
CFGFILE_STR="(String) Configuration file used to retrieve some information. Defaults to '${CFG_FILE}'."
COMPARE_STR="(Boolean) Skips the single baseline plots, doing only the comparison. Defaults to '${COMPARE}'."
function print_usage_submit_skims {
    USAGE=" 
    Run example: bash $(basename "$0") -t <some_tag>

    -h / --help		    [${HELP_STR}]
    --dryrun		    [${DRYRUN_STR}]
    -c / --channel      [${CHANNEL_STR}]
    -s / --selection    [${SELECTION_STR}]
    -t / --tags		    [${TAG_STR}]
    -r / --region    	[${REG_STR}]
    -d / --data_period  [${DATAPERIOD_STR}]
    --nosig             [${NOSIG_STR}]
    --nodata            [${NODATA_STR}]
    --cfg               [${CFGFILE_STR}]
	--compare           [${COMPARE_STR}]

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
		if [[ ! " ${SELECTION_CHOICES[*]} " =~ " ${SELECTION} " ]]; then
			echo "Currently the following selections are supported:"
			for sl in ${SELECTION_CHOICES[@]}; do
				echo "- ${sl}" # bash string substitution
			done
			exit 1;
		fi
		shift; shift;
		;;
	--dryrun)
	    DRYRUN="1"
	    shift;
	    ;;
	--nosig)
	    NOSIG="1"
	    shift;
	    ;;
	--nodata)
	    NODATA="1"
	    shift;
	    ;;
	--compare)
	    COMPARE="1"
	    shift;
	    ;;
	-t|--tags)
	    TAGS+=("${2}")
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
	--cfg)
	    CFGFILE="$2"
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
if [ ${#TAGS[@]} -eq 2 ]; then
	COMPARE_INDIR="${MAIN_DIR}"
	COMPARE_OUTDIR="${MAIN_DIR}/${TAGS[0]}_OVER_${TAGS[1]}"
fi

### Argument parsing sanity checks
if [[ ${#TAGS[@]} -eq 0 ]]; then
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
	echo "Select the channel via the '-c / --channel' option."
	exit 1;
fi
if [[ -z ${REG} ]]; then
	echo "Select the region via the '-r / --region' option."
	exit 1;
fi

LUMI="59.9"
MAIN_DIR="${MAIN_DIR}/${TAGS[0]}"
EOS_DIR="/eos/user/${EOS_USER:0:1}/${EOS_USER}"
WWW_DIR="${EOS_DIR}/www/${PLOTS_DIR}/${TAGS[0]}/${CHANNEL}"
WWW_SUBDIR="${WWW_DIR}/${SELECTION}_${REG}"
if [ ${#TAGS[@]} -eq 2 ]; then
	COMPARE_WWW_SUBDIR="${EOS_DIR}/www/${PLOTS_DIR}/${TAGS[0]}_OVER_${TAGS[1]}"
fi


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
printf "TAGS\t\t= ${TAGS[*]}\n"
printf "DATA_PERIOD\t= ${DATA_PERIOD}\n"
printf "CHANNEL\t\t= ${CHANNEL}\n"
printf "SELECTION\t= ${SELECTION}\n"
printf "REGION\t\t= ${REG}\n"
printf "EOS_USER\t= ${EOS_USER}\n"
printf "NOSIG\t\t= ${NOSIG}\n"
printf "NODATA\t\t= ${NODATA}\n"
printf "CFGFILE\t\t= ${CFGFILE}\n"
printf "COMPARE\t\t= ${COMPARE}\n"
echo "-------------------------------"

### Ensure connection to /eos/ folder
[[ ! -d ${EOS_DIR} ]] && /opt/exp_soft/cms/t3/eos-login -username ${EOS_USER} -init

OPTIONS="--quit --ratio --saveratio " #"--binwidth"
if [[ ${NOSIG} -eq 0 ]]; then
    OPTIONS+=" --signals ggFRadion280 ggFRadion400 ggFRadion550 ggFRadion800 ggFRadion1500 "
else
    OPTIONS+=" --nosig "
fi

if [[ ${NODATA} -eq 1 ]]; then
    OPTIONS+=" --nodata "
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
	comm+="--cfg ${CFGFILE} "
	comm+="--lumi ${LUMI} ${OPTIONS} $@"
	[[ ${DRYRUN} -eq 1 ]] && echo "[DRYRUN] ${comm}" || ${comm}
}

function compare_ratios() {
	comm="python scripts/compareRatios.py --indir ${COMPARE_INDIR} "
	comm+="--reg ${REG} "
	comm+="--channel ${CHANNEL} --lumi ${LUMI} "
	comm+="$@"
	[[ ${DRYRUN} -eq 1 ]] && echo "[DRYRUN] ${comm}" || ${comm}
}
	
declare -A VAR_MAP
VAR_MAP=(
	["dau1_pt"]="pT_{1}[GeV] "
	["bjet1_pt"]="pT_{j1}[GeV] "
	["bjet2_pt"]="pT_{j2}[GeV] "
	["dau1_eta"]="eta_{1}[GeV] "
	["dau2_eta"]="eta_{2}[GeV] "
	["bjet1_eta"]="eta_{j1}[GeV] "
	["bjet2_eta"]="eta_{j2}[GeV] "
	["tauH_mass"]="m_{H#tau}[Gev] "
	["tauH_pt"]="pT_{H#tau}[Gev] "
	["tauH_SVFIT_mass"]="m_{H#tau_{SVFit}}[Gev] "
	["bH_mass"]="m_{Hb}[Gev] "
	["bH_pt"]="pT_{Hb}[Gev]	"
	["ditau_deltaR"]="#DeltaR(#tau#tau)	"
	["dib_deltaR"]="#DeltaR(bb)	"
	["HH_deltaR"]="#DeltaR(HH) "
	["HH_mass"]="m_{HH}[GeV] "
 	["HHKin_mass"]="m_{HHKin}[GeV] "
 	["HH_mass"]="m_{HH}[GeV] "
 	["HHKin_mass"]="m_{HHKin}[GeV] "
 	["DNNoutSM_kl_1"]="DNN "
)

declare -A EXTRAS
EXTRAS=(
	["dau1_pt"]=" "
	["bjet1_pt"]=" "
	["bjet2_pt"]=" "
	["dau1_eta"]=" "
	["dau2_eta"]=" "
	["bjet1_eta"]=" "
	["bjet2_eta"]=" "
	["tauH_mass"]=" "
	["tauH_pt"]=" "
	["tauH_SVFIT_mass"]=" "
	["bH_mass"]=" "
	["bH_pt"]="	"
	["ditau_deltaR"]=" "
	["dib_deltaR"]=" "
	["HH_deltaR"]=" "
	["HH_mass"]=" --logy "
 	["HHKin_mass"]=" --logy "
 	["HH_mass"]=" --logy --logx "
 	["HHKin_mass"]=" --logy --logx "
 	["DNNoutSM_kl_1"]=" --logy "
)

declare -A SIGSCALE_CHANNEL_MAP
SIGSCALE_CHANNEL_MAP=(
	["dau1_pt"]="100 50 20"
	["bjet1_pt"]="100 100 20 "
	["bjet2_pt"]="100 100 20 "
	["dau1_eta"]="40 40 2 "
	["dau2_eta"]="40 40 2 "
	["bjet1_eta"]="40 40 4 "
	["bjet2_eta"]="40 40 4 "
	["tauH_mass"]="30 30 2 "
	["tauH_pt"]="30 30 10 "
	["tauH_SVFIT_mass"]="10 10 2 "
	["bH_mass"]="30 30 2 "
	["bH_pt"]="100 100 5"
	["ditau_deltaR"]="10 10 1 "
	["dib_deltaR"]="10 10 1 "
	["HH_deltaR"]="20 20 2 "
	["HH_mass"]="5 5 1 "
	["HHKin_mass"]="5 5 1 "
	["HH_mass"]="5 5 1 "
	["HHKin_mass"]="5 5 1 "
	["DNNoutSM_kl_1"]="5 5 1 "
)

if [[ ${COMPARE} -eq 0 ]]; then
	if [ ${#TAGS[@]} -ne 1 ]; then
		echo "Please provide a single tag when no comparison is made!"
		exit 1
	fi
	
	for avar in ${!VAR_MAP[@]}; do
		ssarr=(${SIGSCALE_CHANNEL_MAP[${avar}]})
		if [ ${CHANNEL} == "ETau" ]; then
			ss=${ssarr[0]}
		elif [ ${CHANNEL} == "MuTau" ]; then
			ss=${ssarr[1]}
	elif [ ${CHANNEL} == "TauTau" ]; then
			ss=${ssarr[2]}
		else
			echo "Channel ${CHANNEL} is not supported." 
		fi
		run_plot --var ${avar} --lymin 0.65 --sigscale ${ss} --label ${VAR_MAP[$avar]} ${EXTRAS[$avar]}
	done

	run mkdir -p ${WWW_DIR}
	if [ -d ${WWW_SUBDIR} ]; then
		run rm -rf ${WWW_SUBDIR}
	fi
	
	run mkdir ${WWW_SUBDIR}

	run cp ${FULL_OUTDIR}/*png ${WWW_SUBDIR}
	run cp ${FULL_OUTDIR}/*pdf ${WWW_SUBDIR}

	echo "Results: https://${EOS_USER}.web.cern.ch/${EOS_USER}/${PLOTS_DIR}/${TAGS[0]}/${CHANNEL}/${SELECTION}_${REG}/"

else
	if [ ${#TAGS[@]} -ne 2 ]; then
		echo "Please provide two tags when performing a comparison!"
		exit 1
	fi
	run mkdir -p ${COMPARE_OUTDIR}

	for avar in ${!VAR_MAP[@]}; do
		compare_ratios --var ${avar} --sel "${SELECTION}" \
					   --tags "${TAGS[0]} ${TAGS[1]}" \
					   --label ${VAR_MAP[$avar]} \
					   --outdir ${COMPARE_OUTDIR}
	done

	run mkdir -p ${COMPARE_WWW_SUBDIR}
	run mv ${COMPARE_OUTDIR}/*png ${COMPARE_WWW_SUBDIR}
	run mv ${COMPARE_OUTDIR}/*pdf ${COMPARE_WWW_SUBDIR}
fi
