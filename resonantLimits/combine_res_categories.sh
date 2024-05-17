#!/usr/bin/env bash

declare -a CHANNELS;
declare -a MASSES;
declare -a SELECTION_PREFIXES;

# Defaults
TAG=""
VAR="pdnn_m{1}_s0_hh"
SIGNAL="GGF_Radion"
BASEDIR="${HOME}/CMSSW_11_1_9/src/KLUBAnalysis"
PERIOD=""
PERIOD_CHOICES=( "UL16" "UL16APV" "UL17" "UL18" )

HELP_STR="Prints this help message."
TAG_STR="(String) Defines tag for the output. Defaults to '${TAG}'."
VAR_STR="(String) Variable to use for the likelihood fit."
SIGNAL_STR="(String) Signal sample type."
DRYRUN_STR="(Boolean) Whether to run in dry-run mode. Defaults to '${DRYRUN}'."
MASSES_STR="(Array of ints) Resonant masses."
SELPREFIXES_STR="(Array of strings) Selection category prefixes."
CHANNELS_STR="(Array of strings) Channels."
BASEDIR_STR="(String) Base directory."
DATAPERIOD_STR="(String) Which data period to consider: Legacy18, UL18, ... Defaults to '${PERIOD}'."
function print_usage_submit_skims {
    USAGE="

    Run example: bash $(basename "$0") -t <some_tag>

    -h / --help        [${HELP_STR}]
    -t / --tag         [${TAG_STR}]
    -b / --base        [${BASEDIR_STR}]
    -v / --var         [${VAR_STR}]
    -c / --channels    [${CHANNELS_STR}] 
    -m / --masses      [${MASSES_STR}]
    -l / --selprefixes [${SELPREFIXES_STR}]
    -d / --period      [${DATAPERIOD_STR}] 
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
        -t|--tag)
            TAG=${2}
            shift; shift;
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
        -d|--period)
            PERIOD=${2}
            if [[ ! " ${PERIOD_CHOICES[*]} " =~ " ${PERIOD} " ]]; then
                echo "Currently the following data periods are supported:"
                for dp in ${PERIOD_CHOICES[@]}; do
                    echo "- ${dp}" # bash string substitution                                                                                                 
                done
                exit 1;
            fi
            shift; shift;
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
if [[ -z ${PERIOD} ]]; then
    echo "Select the data period via the '-d / --data_period' option."
    exit 1;
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

for ichn in "${!CHANNELS[@]}"; do
    card_dir="${LIMIT_DIR}/cards_${TAG}/${CHANNELS[${ichn}]}"
    cd ${card_dir}

	# group all categories indiscriminately
	comb_dir="${card_dir}/comb_cat/AllCategories/"
	mkdir -p ${comb_dir}
	rm -f -- ${comb_dir}/comb*.txt
	
	proc="${SIGNAL}_${VAR}"
	comb_="${comb_dir}/comb.${proc}"
	comb_txt="${comb_}.txt"
	comb_root="${comb_}.root"

	# aggregate all files within a selection folder
	parallel combineCards.py \
			 -S ${card_dir}/*${VAR}/hhres_${PERIOD}_${CHANNELS[${ichn}]}*.${SIGNAL}{1}.txt \
			 ">" ${comb_txt} ::: ${MHIGH[@]}
	
	parallel combineCards.py \
			 -S ${card_dir}/res*${VAR}/hhres_${PERIOD}_${CHANNELS[${ichn}]}*.${SIGNAL}{1}.txt \
			 ">>" ${comb_txt} ::: ${MLOW[@]}

	parallel echo "SignalScale rateParam \* ${SIGNAL}{1} 0.01" ">>" ${comb_txt} ::: ${MASSES[@]}
	parallel text2workspace.py ${comb_txt} -o ${comb_root} ::: ${MASSES[@]}

	# group categories according to prefixes passed by the user
	# if no prefixes are passed, the following 'for' block simply does not run
	for selp in ${SELECTION_PREFIXES[@]}; do
		echo "Grouping categories with the '${selp}' prefix..."
		declare -a allpref=($(cd ${card_dir}/ && ls -d -1 ${selp}*/))
		echo "Processing all selections starting with '${selp}' for channel ${CHANNELS[@]} (${#allpref[@]} in total): "
		for allp in ${allpref[@]}; do
			echo "- ${allp}"
		done

		comb_dir="${card_dir}/comb_cat/${selp}_${VAR}"
		mkdir -p ${comb_dir}
		rm -f -- ${comb_dir}/comb*.txt

		proc="${SIGNAL}_${VAR}"
		comb_="${comb_dir}/comb.${proc}"
		comb_txt="${comb_}.txt"
		comb_root="${comb_}.root"

		# aggregate all files within a selection prefix folder	
		parallel combineCards.py \
				 -S ${card_dir}/${selp}*_${VAR}/hhres_${PERIOD}_${CHANNELS[${ichn}]}*.${SIGNAL}{1}.txt \
				 ">" ${comb_txt} ::: ${MHIGH[@]}

		if [[ ${selp} =~ res.* ]]; then
			parallel combineCards.py \
					 -S ${card_dir}/${selp}*_${VAR}/hhres_${PERIOD}_${CHANNELS[${ichn}]}*.${SIGNAL}{1}.txt \
					 ">>" ${comb_txt} ::: ${MLOW[@]}
		fi

		if [[ ${selp} =~ res.* ]]; then
			parallel echo "SignalScale rateParam \* ${SIGNAL}{1} 0.01" ">>" ${comb_txt} ::: ${MASSES[@]}
			parallel text2workspace.py ${comb_txt} -o ${comb_root} ::: ${MASSES[@]}
		else
			parallel echo "SignalScale rateParam \* ${SIGNAL}{1} 0.01" ">>" ${comb_txt} ::: ${MHIGH[@]}
			parallel text2workspace.py ${comb_txt} -o ${comb_root} ::: ${MHIGH[@]}
		fi
			
	done

	cd -
done
