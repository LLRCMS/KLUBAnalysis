#!/usr/bin/env bash

declare -a MASSES;
declare -a SELECTION_PREFIXES;

# Defaults
VAR="pdnn_m{1}_s0_hh"
SIGNAL="GGF_Radion"
BASEDIR="${HOME}/CMSSW_11_1_9/src/KLUBAnalysis"

HELP_STR="Prints this help message."
VAR_STR="(String) Variable to use for the likelihood fit."
SIGNAL_STR="(String) Signal sample type."
DRYRUN_STR="(Boolean) Whether to run in dry-run mode. Defaults to '${DRYRUN}'."
MASSES_STR="(Array of ints) Resonant masses."
SELPREFIXES_STR="(Array of strings) Selection category prefixes."
BASEDIR_STR="(String) Base directory."
function print_usage_submit_skims {
    USAGE="

    Run example: bash $(basename "$0")
    -h / --help        [${HELP_STR}]
    -b / --base        [${BASEDIR_STR}]
    -v / --var         [${VAR_STR}]
    -s / --signal      [${SIGNAL_STR}]
    -m / --masses      [${MASSES_STR}]
    -l / --selprefixes [${SELPREFIXES_STR}]
    --dry-run          [${DRYRUN_STR}] 

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
		-b|--base)
			BASEDIR=${2}
			shift; shift;
			;;
        -s|--signal)
            SIGNAL=${2}
            shift; shift;
            ;;
		-v|--var)
            VAR=${2}
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
        -l|--selprefixes)
            sel_flag=0
            while [ ${sel_flag} -eq 0 ]; do
                if [[ "${2}" =~ ^[-].*$ ]] || [[ "${2}" =~ ^$ ]]; then
                    sel_flag=1
                else
                    SELECTION_PREFIXES+=(${2});
                    shift;
                fi
            done
            shift;
            ;;
        *) # unknown option 
            echo "Wrong parameter ${1}."
            exit 1
            ;;
    esac
done

if [ ${#MASSES[@]} -eq 0 ]; then
    MASSES=("250" "260" "270" "280" "300" "320" "350" "400" "450" "500" "550" "600" "650" "700" "750" "800" "850" "900" "1000" "1250" "1500" "1750" "2000" "2500" "3000")
fi

LIMIT_DIR="${BASEDIR}/resonantLimits"

declare -A TAGS=(
	["2016"]="Final_Shapes_UL16"
	["2016APV"]="Final_Shapes_UL16APV"
	["2017"]="Final_Shapes_UL17"
	["2018"]="Final_Shapes_UL18"
)

NEWDIR="${LIMIT_DIR}/cards_Run2_${VAR}_CombChn"
mkdir -p ${NEWDIR}
for selp in ${SELECTION_PREFIXES[@]}; do
    mkdir -p ${NEWDIR}/"${selp}_${VAR}"
done

declare -a MASSES_IF;
declare -a MHIGH;
for mass in ${MASSES[@]}; do
	if [ ${mass} -gt "319" ]; then
		MHIGH+=(${mass})
	fi
done

cd ${NEWDIR}
for selprefix in ${SELECTION_PREFIXES[@]}; do
    # all directories will have the same categories, hence the wildcard 
    # (impossible to tell beforehand which channel was chosen)
	for tag in ${TAGS[@]}; do
		declare -a allpref=($(cd ${LIMIT_DIR}/cards_${tag}_*Tau/ && ls -d -1 ${selprefix}*_${VAR}/))
		echo "Tag ${tag}: processing all selections starting with '${selprefix}' (${#allpref[@]} in total): "
		for selp in ${allpref[@]}; do
			echo "- ${selp}"
		done
	done

    proc="${SIGNAL}_${VAR}_{}"
    path_out_="${NEWDIR}/${selprefix}_${VAR}/comb.${proc}"
    path_txt="${path_out_}.txt"
    path_root="${path_out_}.root"
	echo "Output folder: ${NEWDIR}/${selprefix}_${VAR}/"
	
    # remove low masses for boosted categories
    if [[ ${selprefix} =~ .*boosted.* ]]; then
		MASSES_IF=${MHIGH[@]};
    else
		MASSES_IF=${MASSES[@]};
    fi

    # parallelize over the mass
    parallel rm -f -- ${path_txt} ::: ${MASSES_IF[@]}
    parallel rm -f -- ${path_root} ::: ${MASSES_IF[@]}

    filename="hhres_*_*Tau_${selprefix}*${SIGNAL}{}.txt"
    parallel combineCards.py -S \
			 ${LIMIT_DIR}/cards_${TAGS["2016"]}_*Tau/${selprefix}*_${VAR}/${filename} \
			 ${LIMIT_DIR}/cards_${TAGS["2016APV"]}_*Tau/${selprefix}*_${VAR}/${filename} \
			 ${LIMIT_DIR}/cards_${TAGS["2017"]}_*Tau/${selprefix}*_${VAR}/${filename} \
			 ${LIMIT_DIR}/cards_${TAGS["2018"]}_*Tau/${selprefix}*_${VAR}/${filename} \
			 ">" ${path_txt} ::: ${MASSES_IF[@]}
    parallel echo "SignalScale rateParam \* ${SIGNAL}{} 0.01" ">>" ${path_txt} ::: ${MASSES_IF[@]}
    parallel text2workspace.py ${path_txt} -o ${path_root} ::: ${MASSES_IF[@]}

done
cd -
