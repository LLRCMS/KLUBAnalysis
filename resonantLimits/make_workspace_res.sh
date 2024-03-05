#!/usr/bin/env bash

declare -a CHANNELS;
declare -a SELECTIONS;
declare -a MASSES;

# Defaults
TAG="10Feb_UL18"
VAR="DNNoutSM_kl_1"
SIGNAL="ggFRadion"
BASEDIR="${HOME}/CMSSW_11_1_9/src/KLUBAnalysis"

HELP_STR="Prints this help message."
TAG_STR="(String) Defines tag for the output. Defaults to '${TAG}'."
VAR_STR="(String) Variable to use for the likelihood fit."
SIGNAL_STR="(String) Signal sample type."
DRYRUN_STR="(Boolean) Whether to run in dry-run mode. Defaults to '${DRYRUN}'."
MASSES_STR="(Array of ints) Resonant masses."
SELECTIONS_STR="(Array of strings) Selections."
CHANNELS_STR="(Array of strings) Channels."
BASEDIR_STR="(String) Base directory."
function print_usage_submit_skims {
    USAGE="
                                                              
    Run example: bash $(basename "$0") -t <some_tag>
    -h / --help       [${HELP_STR}]
    -t / --tag        [${TAG_STR}]
    -b / --base       [${BASEDIR_STR}]
    -v / --var        [${VAR_STR}]
    -s / --signal     [${SIGNAL_STR}]
    -m / --masses     [${MASSES_STR}]
    -l / --selections [${SELECTIONS_STR}]
    -c / --channels   [${CHANNELS_STR}]
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
	-s|--signal)
	        SIGNAL=${2}
            shift; shift;
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
if [ ${#SELECTIONS[@]} -eq 0 ]; then
    SELECTIONS=("res1b" "res2b" "boostedL_pnet")
fi

declare -a MASSES_IF;
declare -a MHIGH;
for mass in ${MASSES[@]}; do
    if [ ${mass} -gt "319" ]; then
	MHIGH+=(${mass})
	fi
done

LIMIT_DIR="${BASEDIR}/resonantLimits"

for ichn in "${!CHANNELS[@]}"; do
    card_dir="${LIMIT_DIR}/cards_${TAG}_${CHANNELS[${ichn}]}"
    proc="${SIGNAL}_${VAR}_{}"
    comb_="comb.${proc}"
    
    for sel in ${SELECTIONS[@]}; do
	echo "Processing ${sel} for channel ${CHANNELS[${ichn}]} ..."
	cat_dir="${card_dir}/${sel}_${VAR}"
	cd ${cat_dir}

	comb_txt="${cat_dir}/${comb_}.txt"
	comb_root="${cat_dir}/${comb_}.root"

	# remove low masses for boosted categories
	if [[ ${sel} =~ .*boosted.* ]]; then
	    MASSES_IF=${MHIGH[@]};
	    else
	    MASSES_IF=${MASSES[@]};
	    fi

	# parallelize over the mass
	parallel rm -f -- ${comb_txt} ::: ${MASSES_IF[@]}
	parallel combineCards.py -S ${cat_dir}/hhres_*.${SIGNAL}{}.txt ">" ${comb_txt} ::: ${MASSES_IF[@]}
	parallel echo "SignalScale rateParam \* ${SIGNAL}{} 0.01" ">>" ${comb_txt} ::: ${MASSES_IF[@]}

	cd -
	parallel text2workspace.py ${comb_txt} -o ${comb_root} ::: ${MASSES_IF[@]}

    done
    cd ${LIMIT_DIR}
done
