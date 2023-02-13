#!/usr/bin/env bash

declare -a MASSES;

### Defaults
TAG=""
VAR="DNNoutSM_kl_1"
SIGNAL="ggFRadion"

### Argument parsing
HELP_STR="Prints this help message."
TAG_STR="(String) Defines tag for the output. Defaults to '${TAG}'."
VAR_STR="(String) Defines the variable for the likelihood fit. Defaults to '${TAG}'."

function print_usage_submit_skims {
    USAGE="

    Run example: bash $(basename "$0") -t <some_tag>
    -h / --help         [${HELP_STR}]
    -t / --tag          [${TAG_STR}]
    -v / --var          [${VAR_STR}]
    -s / --signal       [${SIGNAL_STR}]
    -m / --masses       [${MASSES_STR}]

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
	-t|--tag)
	    TAG=${2}
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

comb_dir="cards_${TAG}_All"
mkdir -p ${comb_dir}
rm -f -- ${comb_dir}/comb.*.txt

proc="${SIGNAL}{}"
comb_="${comb_dir}/comb.${proc}"
comb_txt="${comb_}.txt"
comb_root="${comb_}.root"
tmp_txt="${comb_dir}/tmp_{}.txt"

#parallelize over the mass
parallel combineCards.py -S cards_${TAG}_*Tau/*${VAR}/hhres*.${proc}.txt ">>" ${comb_txt} ::: ${MASSES[@]}
parallel echo "SignalScale rateParam \* ${proc} 0.01" ">" ${tmp_txt} ::: ${MASSES[@]}
parallel cat ${tmp_txt} ">>" ${comb_txt} ::: ${MASSES[@]}
parallel text2workspace.py ${comb_txt} -o ${comb_root} ::: ${MASSES[@]}

