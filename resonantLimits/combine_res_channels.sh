#!/usr/bin/env bash

declare -a MASSES;
declare -a SELECTIONS;

# Defaults
TAG="10Feb_UL18"
VAR="DNNoutSM_kl_1"
SIGNAL="ggFRadion"
LIMIT_DIR="/home/llr/cms/${USER}/CMSSW_11_1_9/src/KLUBAnalysis/resonantLimits"

HELP_STR="Prints this help message."
TAG_STR="(String) Defines tag for the output. Defaults to '${TAG}'."
VAR_STR="(String) Variable to use for the likelihood fit."
SIGNAL_STR="(String) Signal sample type."
DRYRUN_STR="(Boolean) Whether to run in dry-run mode. Defaults to '${DRYRUN}'."
MASSES_STR="(Array of ints) Resonant masses."
SELECTIONS_STR="(Array of strings) Selection categories."
function print_usage_submit_skims {
    USAGE="

    Run example: bash $(basename "$0") -t <some_tag>
    -h / --help       [${HELP_STR}]
    -t / --tag        [${TAG_STR}]
    -v / --var        [${VAR_STR}]
    -s / --signal     [${SIGNAL_STR}]
    -m / --masses     [${MASSES_STR}]
    -l / --selections [${SELECTIONS_STR}]
    --dry-run         [${DRYRUN_STR}] 

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
        *) # unknown option 
            echo "Wrong parameter ${1}."
            exit 1
            ;;
    esac
done

if [ ${#MASSES[@]} -eq 0 ]; then
    MASSES=("250" "260" "270" "280" "300" "320" "350" "400" "450" "500" "550" "600" "650" "700" "750" "800" "850" "900" "1000" "1250" "1500" "1750" "2000" "2500" "3000")
fi

declare -a SELECTIONS_VAR;
for i in "${!SELECTIONS[@]}"; do
    SELECTIONS_VAR+=("${SELECTIONS[$i]}_${VAR}")
done

NEWDIR="${LIMIT_DIR}/cards_${TAG}_CombChn"
mkdir -p ${NEWDIR}
for selection in ${SELECTIONS_VAR[@]}; do
    mkdir -p ${NEWDIR}/${selection}
done

declare -a MASSES_IF;
declare -a MHIGH;
for mass in ${MASSES[@]}; do
	if [ ${mass} -gt "319" ]; then
		MHIGH+=(${mass})
	fi
done

cd ${NEWDIR}
for sel in ${SELECTIONS[@]}; do
	echo "Processing ${sel} ..."
	
    proc="${SIGNAL}{}"
    path_in="${NEWDIR}/${sel}/comb.${proc}"
    path_out="${NEWDIR}/${sel}_${VAR}/comb.${proc}"
    tmp="${NEWDIR}/${sel}_${VAR}/tmp_{}.txt"

	# remove low masses for boosted categories
	if [[ ${sel} =~ .*boosted.* ]]; then
		MASSES_IF=${MHIGH[@]};
	else
		MASSES_IF=${MASSES[@]};
	fi

    # parallelize over the mass
    parallel rm -f -- ${path_in}.txt ::: ${MASSES_IF[@]}
    parallel rm -f -- ${tmp} ::: ${MASSES_IF[@]}
    parallel rm -f -- ${path_out}.txt ::: ${MASSES_IF[@]}
    parallel rm -f -- ${path_out}.root ::: ${MASSES_IF[@]}

    parallel combineCards.py -S ../cards_${TAG}*Tau/${sel}*/hhres*${proc}.txt ">>" ${path_out}.txt ::: ${MASSES_IF[@]}
    parallel echo "SignalScale rateParam \* ${proc} 0.01" ">>" ${tmp} ::: ${MASSES_IF[@]}
    parallel cat ${tmp} ">>" ${path_out}.txt ::: ${MASSES_IF[@]}
    parallel text2workspace.py ${path_out}.txt -o ${path_out}.root ::: ${MASSES_IF[@]}
done
cd -
