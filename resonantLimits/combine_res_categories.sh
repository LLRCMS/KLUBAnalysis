#!/usr/bin/env bash

TAG=""
VAR="DNNoutSM_kl_1"
SIGNAL="ggFRadion"
LIMIT_DIR="/home/llr/cms/${USER}/CMSSW_11_1_9/src/KLUBAnalysis/resonantLimits"

declare -a CHANNELS;
declare -a MASSES;

HELP_STR="Prints this help message."
TAG_STR="(String) Defines tag for the output. Defaults to '${TAG}'."
VAR_STR="(String) Variable to use for the likelihood fit."
SIGNAL_STR="(String) Signal sample type."
DRYRUN_STR="(Boolean) Whether to run in dry-run mode. Defaults to '${DRYRUN}'."
MASSES_STR="(Array of ints) Resonant masses."
CHANNELS_STR="(Array of strings) Channels."
function print_usage_submit_skims {
    USAGE="

    Run example: bash $(basename "$0") -t <some_tag>                                                                                       
    -h / --help       [${HELP_STR}]
    -t / --tag        [${TAG_STR}]
    -v / --var        [${VAR_STR}]
    -c / --channels   [${CHANNELS_STR}] 
    -m / --masses     [${MASSES_STR}] 
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
        *)  # unknown option                                                                                                               
            echo "Wrong parameter ${1}."
            exit 1
            ;;
    esac
done

if [ ${#MASSES[@]} -eq 0 ]; then
    MASSES=("250" "260" "270" "280" "300" "320" "350" "400" "450" "500" "550" "600" "650" "700" "750" "800" "850" "900" "1000" "1250" "1500" "1750" "2000" "2500" "3000")
fi
if [ ${#CHANNELS[@]} -eq 0 ]; then
    CHANNELS=("ETau" "MuTau" "TauTau")
fi

for i in "${!CHANNELS[@]}"; do
    card_dir="${LIMIT_DIR}/cards_${TAG}_${CHANNELS[$i]}"
    cd ${card_dir}

    comb_dir="comb_cat"
    mkdir -p ${comb_dir}
    rm -f -- ${comb_dir}/comb*.txt
    rm -f -- ${comb_dir}/add_${CHANNELS[$i]}_*.txt

    addp="add_${CHANNELS[$i]}"
    #for mass in ${MASSES[@]}; do
    parallel combineCards.py -S *kl_1/hhres*.${SIGNAL}{}.txt ">" ${comb_dir}/comb.${SIGNAL}{}.txt ::: ${MASSES[@]}
    parallel echo "SignalScale rateParam * ${SIGNAL}{} 0.01" ">" ${comb_dir}/${addp}_{}.txt ::: ${MASSES[@]}
    parallel cat ${comb_dir}/${addp}_{}.txt ">" ${comb_dir}/comb.${SIGNAL}{}.txt ::: ${MASSES[@]}
    parallel text2workspace.py ${comb_dir}/comb.${SIGNAL}{}.txt -o ${comb_dir}/comb.${SIGNAL}{}.root ::: ${MASSES[@]}
    #done
    cd -
done
