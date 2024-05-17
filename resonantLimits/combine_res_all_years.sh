#!/usr/bin/env bash

declare -a MASSES;

### Defaults
VAR="pdnn_m{1}_s0_hh"
SIGNAL="GGF_Radion"
BASEDIR="${HOME}/CMSSW_11_1_9/src/KLUBAnalysis"

### Argument parsing
HELP_STR="Prints this help message."
VAR_STR="(String) Defines the variable for the likelihood fit. Defaults to '${VAR}'."
SIGNAL_STR="(String) Signal sample type."
MASSES_STR="(Array of ints) Resonant masses."
BASEDIR_STR="(String) Base directory."
function print_usage_submit_skims {
    USAGE="

    Run example: bash $(basename "$0") -t <some_tag>
    -h / --help   [${HELP_STR}]
    -b / --base	  [${BASEDIR_STR}]
    -v / --var	  [${VAR_STR}]
    -s / --signal [${SIGNAL_STR}]
    -m / --masses [${MASSES_STR}]

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
	*)  # unknown option
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
	["2016"]="Final_CorrectBinning_UL16"
	["2016APV"]="Final_CorrectBinning_UL16APV"
	["2017"]="Final_CorrectBinning_UL17"
	["2018"]="Final_CorrectBinning_UL18"
)

comb_dir="${LIMIT_DIR}/cards_Final_CorrectBinning_Run2_All"
mkdir -p ${comb_dir}
parallel mkdir -p ${comb_dir} ::: ${MASSES[@]}
#parallel rm -f -- ${comb_dir}/comb.*.txt ::: ${MASSES[@]}

proc="${SIGNAL}_${VAR}"
comb_="${comb_dir}/comb.${proc}"
comb_txt="${comb_}.txt"
comb_root="${comb_}.root"

ulimit -s unlimited

#parallelize over the mass
parallel combineCards.py -S \
		 ${LIMIT_DIR}/cards_${TAGS["2016"]}/*Tau/*${VAR}/hhres*.${SIGNAL}{1}.txt \
		 ${LIMIT_DIR}/cards_${TAGS["2016APV"]}/*Tau/*${VAR}/hhres*.${SIGNAL}{1}.txt \
		 ${LIMIT_DIR}/cards_${TAGS["2017"]}/*Tau/*${VAR}/hhres*.${SIGNAL}{1}.txt \
		 ${LIMIT_DIR}/cards_${TAGS["2018"]}/*Tau/*${VAR}/hhres*.${SIGNAL}{1}.txt \
		 ">" ${comb_txt} ::: ${MASSES[@]}
parallel echo "SignalScale rateParam \* ${SIGNAL}{1} 0.01" ">>" ${comb_txt} ::: ${MASSES[@]}

echo ${comb_txt}
echo ${comb_root}
parallel text2workspace.py ${comb_txt} -o ${comb_root} ::: ${MASSES[@]}
