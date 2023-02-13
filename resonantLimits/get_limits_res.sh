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

HELP_STR="Prints this help message."
TAG_STR="(String) Defines tag for the output. Defaults to '${TAG}'."
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

if [ ${#MASSES[@]} -eq 0 ]; then
    MASSES=("250" "260" "270" "280" "300" "320" "350" "400" "450" "500" "550" "600" "650" "700" "750" "800" "850" "900" "1000" "1250" "1500" "1750" "2000" "2500" "3000")
fi
if [ ${#CHANNELS[@]} -eq 0 ]; then
    CHANNELS=("ETau" "MuTau" "TauTau")
fi
if [ ${#SELECTIONS[@]} -eq 0 ]; then
    SELECTIONS=("s1b1jresolvedInvMcut" "s2b0jresolvedInvMcut" "sboostedLLInvMcut")
fi

for i in "${!CHANNELS[@]}"; do
	card_dir="${LIMIT_DIR}/cards_${TAG}_${CHANNELS[$i]}"
	cd ${card_dir}

	for sel in ${SELECTIONS[@]}; do
		out_dir="${sel}${VAR}/combined_out"
		mkdir -p ${out_dir}

		# parallellize across the mass
		txt="${card_dir}/${sel}${VAR}/comb.${SIGNAL}{}.txt"
		log="${card_dir}/${out_dir}/comb.${SIGNAL}{}.log"
		parallel rm -f -- ${log} ::: ${MASSES[@]}		
		parallel combine -M AsymptoticLimits ${txt} \
				 -n ${IDENTIFIER}${sel} \
				 --run blind \
				 --noFitAsimov \
				 --freezeParameters SignalScale \
				 -m {} ">" ${log} ::: ${MASSES[@]}
	done

	cd -
done
