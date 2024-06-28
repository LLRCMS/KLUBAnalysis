#!/usr/bin/env bash

declare -a CHANNELS;
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
CHANNELS_STR="(Array of strings) Channels."
BASEDIR_STR="(String) Base directory."
function print_usage_submit_skims {
    USAGE="

    Run example: bash $(basename "$0") -t <some_tag>

    -h / --help        [${HELP_STR}]
    -b / --base        [${BASEDIR_STR}]
    -v / --var         [${VAR_STR}]
    -c / --channels    [${CHANNELS_STR}] 
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
if [ ${#CHANNELS[@]} -eq 0 ]; then
    CHANNELS=("ETau" "MuTau" "TauTau")
fi

declare -a MHIGH;
declare -a MLOW;
for mass in ${MASSES[@]}; do
	if [ ${mass} -gt "319" ]; then
		MHIGH+=(${mass})
	else
		MLOW+=(${mass})
	fi
done

LIMIT_DIR="${BASEDIR}/resonantLimits"

declare -A TAGS=(
	["2016"]="Final_Shapes_UL16"
	["2016APV"]="Final_Shapes_UL16APV"
	["2017"]="Final_Shapes_UL17"
	["2018"]="Final_Shapes_UL18"
)

card_dir="${LIMIT_DIR}/cards_Run2_${VAR}_CombCat"
mkdir -p ${card_dir}
cd ${card_dir}

# group all categories indiscriminately
for chn in "${CHANNELS[@]}"; do

	comb_dir="${card_dir}/${chn}"
	mkdir -p ${comb_dir}
	rm -f -- ${comb_dir}/comb*.txt


	proc="${SIGNAL}_${VAR}_{}"
	comb_="${comb_dir}/comb.${proc}"
	comb_txt="${comb_}.txt"
	comb_root="${comb_}.root"

	# aggregate all files within a selection folder
	filename="hhres_*_${chn}*.${SIGNAL}{}.txt"
	parallel combineCards.py -S \
			 ${LIMIT_DIR}/cards_${TAGS["2016"]}_${chn}/*${VAR}/${filename} \
			 ${LIMIT_DIR}/cards_${TAGS["2016APV"]}_${chn}/*${VAR}/${filename} \
			 ${LIMIT_DIR}/cards_${TAGS["2017"]}_${chn}/*${VAR}/${filename} \
			 ${LIMIT_DIR}/cards_${TAGS["2018"]}_${chn}/*${VAR}/${filename} \
			 ">" ${comb_txt} ::: ${MHIGH[@]}
	
	parallel combineCards.py -S \
			 ${LIMIT_DIR}/cards_${TAGS["2016"]}_${chn}/*resolved*${VAR}/${filename} \
			 ${LIMIT_DIR}/cards_${TAGS["2016APV"]}_${chn}/*resolved*${VAR}/${filename} \
			 ${LIMIT_DIR}/cards_${TAGS["2017"]}_${chn}/*resolved*${VAR}/${filename} \
			 ${LIMIT_DIR}/cards_${TAGS["2018"]}_${chn}/*resolved*${VAR}/${filename} \
			 ">>" ${comb_txt} ::: ${MLOW[@]}

	parallel echo "SignalScale rateParam \* ${SIGNAL}{} 0.01" ">>" ${comb_txt} ::: ${MASSES[@]}
	parallel text2workspace.py ${comb_txt} -o ${comb_root} ::: ${MASSES[@]}
done
cd -
