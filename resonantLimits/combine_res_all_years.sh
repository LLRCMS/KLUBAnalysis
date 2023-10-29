#!/usr/bin/env bash

declare -a MASSES;

### Defaults
VAR="DNNoutSM_kl_1"
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
	["2016"]="25Oct2023_newBigNtuples_forLimits_UL2016_legacyCategories"
	#["2016"]="25Oct2023_newBigNtuples_forLimits_HHMass_UL2016_legacyCategories"
	["2016APV"]="24Oct2023_newBigNtuples_bTagReshapeSF_forLimits_UL2016APV_legacyCategories"
	#["2016APV"]="18Oct2023_newBigNtuples_forLimits_HHMass_UL2016APV_legacyCategories"
	# ["2017"]="Upstream_UL17"
	# ["2018"]="Upstream_UL18"
)

comb_dir="${LIMIT_DIR}/cards_Years_${VAR}_All"
mkdir -p ${comb_dir}
rm -f -- ${comb_dir}/comb.*.txt

proc="${SIGNAL}_${VAR}_{}"
comb_="${comb_dir}/comb.${proc}"
comb_txt="${comb_}.txt"
comb_root="${comb_}.root"

#parallelize over the mass
parallel combineCards.py -S \
		 ${LIMIT_DIR}/cards_${TAGS["2016"]}_*Tau/*${VAR}/hhres*.${SIGNAL}{}.txt \
		 ${LIMIT_DIR}/cards_${TAGS["2016APV"]}_*Tau/*${VAR}/hhres*.${SIGNAL}{}.txt \
		 ">" ${comb_txt} ::: ${MASSES[@]}
parallel echo "SignalScale rateParam \* ${SIGNAL}{} 0.01" ">>" ${comb_txt} ::: ${MASSES[@]}

# echo ${comb_txt}
# echo ${comb_root}
parallel text2workspace.py ${comb_txt} -o ${comb_root} ::: ${MASSES[@]}
