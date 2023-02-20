#!/usr/bin/env bash                                                                                                                        
declare -a CHANNELS;
declare -a SELECTIONS;
declare -a MASSES;

# Defaults
IDENTIFIER=".test"
TAG=""
VAR="DNNoutSM_kl_1"
SIGNAL="ggFRadion"
LIMIT_DIR="/home/llr/cms/${USER}/CMSSW_11_1_9/src/KLUBAnalysis/resonantLimits"

MODE=""
MODE_CHOICES=( "separate" "sel_group" "chn_group" "all_group" )

HELP_STR="Prints this help message."
TAG_STR="(String) Defines tag for the output. Defaults to '${TAG}'."
MODE_STR="(String) Defines the mode. Defaults to '${MODE}'."
VAR_STR="(String) Variable to use for the likelihood fit."
SIGNAL_STR="(String) Signal sample type."
DRYRUN_STR="(Boolean) Whether to run in dry-run mode. Defaults to '${DRYRUN}'."
MASSES_STR="(Array of ints) Resonant masses."
CHANNELS_STR="(Array of strings) Channels."
SELECTIONS_STR="(Array of strings) Selection categories."
function print_usage_submit_skims {
    USAGE="

    Run example: bash $(basename "$0") -t <some_tag>
                                      
    -h / --help       [${HELP_STR}]
    -m / --mode       [${TAG_STR}]
    -t / --tag        [${TAG_STR}]
    -v / --var        [${VAR_STR}]
    -s / --signal     [${SIGNAL_STR}]
    -c / --channels   [${CHANNELS_STR}] 
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
		--mode)
			MODE=${2}
			if [[ ! " ${MODE_CHOICES[*]} " =~ " ${MODE} " ]]; then
				echo "You provided mode=${MODE}."
				echo "Currently the following modes are supported:"
				for md in ${MODE_CHOICES[@]}; do
					echo "- ${md}" # bash string substitution
				done
				exit 1;
			fi
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
        *) # unknown option
	    echo "Wrong parameter ${1}."
            exit 1
            ;;
    esac
done

if [[ -z ${MODE} ]]; then
	echo "Select the data period via the '--mode' option."
	exit 1;
fi
if [ ${#MASSES[@]} -eq 0 ]; then
    MASSES=("250" "260" "270" "280" "300" "320" "350" "400" "450" "500" "550" "600" "650" "700" "750" "800" "850" "900" "1000" "1250" "1500" "1750" "2000" "2500" "3000")
fi
if [ ${#CHANNELS[@]} -eq 0 ]; then
    CHANNELS=("ETau" "MuTau" "TauTau")
fi
if [ ${#SELECTIONS[@]} -eq 0 ]; then
    SELECTIONS=("s1b1jresolvedMcut" "s2b0jresolvedMcut" "sboostedLLMcut")
fi

declare -a MASSES_IF;
declare -a MHIGH;
for mass in ${MASSES[@]}; do
	if [ ${mass} -gt "319" ]; then
		MHIGH+=(${mass})
	fi
done

if [ ${MODE} == "separate" ]; then
	
	for i in "${!CHANNELS[@]}"; do
		card_dir="${LIMIT_DIR}/cards_${TAG}_${CHANNELS[$i]}"
		cd ${card_dir}

		for sel in ${SELECTIONS[@]}; do
			echo "Processing category ${sel} for channel ${CHANNELS[$i]} (mode ${MODE})..."
		
			cat_dir="${card_dir}/${sel}_${VAR}"
			out_dir="${cat_dir}/combined_out"
			mkdir -p "${out_dir}"
			
			# remove low masses for boosted categories
			if [[ ${sel} =~ .*boosted.* ]]; then
				MASSES_IF=${MHIGH[@]}
			else
				MASSES_IF=${MASSES[@]}
			fi
		
			# parallellize across the mass
			in_txt="${cat_dir}/comb.${SIGNAL}{}.txt"
			out_log="${out_dir}/comb.${SIGNAL}{}.log"
			parallel rm -f -- ${out_log} ::: ${MASSES_IF[@]}
			parallel combine -M AsymptoticLimits ${in_txt} \
					 -n ${IDENTIFIER}_${sel} \
					 --run blind \
					 --noFitAsimov \
					 --freezeParameters SignalScale \
					 -m {} ">" ${out_log} ::: ${MASSES_IF[@]}
		done		
		cd -
	done

elif [ ${MODE} == "sel_group" ]; then
		 
	for chn in "${CHANNELS[@]}"; do
		echo "Processing channel ${chn} with mode ${MODE}..."
		card_dir="${LIMIT_DIR}/cards_${TAG}_${chn}"
		cd ${card_dir}

		comb_dir="${card_dir}/comb_cat"
		out_dir="${comb_dir}/combined_out"
		mkdir -p "${out_dir}"

		# parallellize across the mass
		in_txt="${comb_dir}/comb.${SIGNAL}{}.txt"
		out_log="${out_dir}/comb.${SIGNAL}{}.log"

		parallel rm -f -- ${out_log} ::: ${MASSES[@]}
		parallel combine -M AsymptoticLimits ${in_txt} \
				 -n ${IDENTIFIER}_${chn} \
				 --run blind \
				 --noFitAsimov \
				 --freezeParameters SignalScale \
				 -m {} ">" ${out_log} ::: ${MASSES[@]}
		cd -
	done

elif [ ${MODE} == "chn_group" ]; then
		 
	for sel in "${SELECTIONS[@]}"; do
		echo "Processing category ${sel} with mode ${MODE}..."
		card_dir="${LIMIT_DIR}/cards_${TAG}_CombChn"
		cd ${card_dir}

		comb_dir="${card_dir}/${sel}_${VAR}"
		out_dir="${comb_dir}/combined_out"
		mkdir -p "${out_dir}"
		
		# remove low masses for boosted categories
		if [[ ${sel} =~ .*boosted.* ]]; then
			MASSES_IF=${MHIGH[@]}
		else
			MASSES_IF=${MASSES[@]}
		fi

		# parallellize across the mass
		in_txt="${comb_dir}/comb.${SIGNAL}{}.txt"
		out_log="${out_dir}/comb.${SIGNAL}{}.log"
		parallel rm -f -- ${out_log} ::: ${MASSES_IF[@]}
		parallel combine -M AsymptoticLimits ${in_txt} \
				 -n ${IDENTIFIER}_${sel} \
				 --run blind \
				 --noFitAsimov \
				 --freezeParameters SignalScale \
				 -m {} ">" ${out_log} ::: ${MASSES_IF[@]}
		cd -
	done

elif [ ${MODE} == "all_group" ]; then

	echo "Processing all categories and channels (mode ${MODE})..."
	card_dir="${LIMIT_DIR}/cards_${TAG}_All"
	out_dir="${card_dir}/combined_out"
	mkdir -p "${out_dir}"
					
	# parallellize across the mass
	in_txt="${card_dir}/comb.${SIGNAL}{}.txt"
	out_log="${out_dir}/comb.${SIGNAL}{}.log"
	parallel rm -f -- ${out_log} ::: ${MASSES[@]}
	parallel combine -M AsymptoticLimits ${in_txt} \
			 -n ${IDENTIFIER}_all \
			 --run blind \
			 --noFitAsimov \
			 --freezeParameters SignalScale \
			 -m {} ">" ${out_log} ::: ${MASSES[@]}

fi