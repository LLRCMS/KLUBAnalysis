#!/usr/bin/env bash                                                                                                                        
declare -a CHANNELS;
declare -a SELECTIONS;
declare -a MASSES;

# Defaults
TAG=""
VAR="pdnn_m{1}_s0_hh"
SIGNAL="GGF_Radion"
DRYRUN="0"
MODE=""
MODE_CHOICES=( "separate" "sel_group" "chn_group" "all_group" "sel_years" "chn_years" "all_years" )
BASEDIR="${HOME}/CMSSW_11_1_9/src/KLUBAnalysis"

HELP_STR="Prints this help message."
TAG_STR="(String) Defines tag for the output. Defaults to '${TAG}'."
MODE_STR="(String) Defines the mode. Defaults to '${MODE}'."
VAR_STR="(String) Variable to use for the likelihood fit."
SIGNAL_STR="(String) Signal sample type."
DRYRUN_STR="(Boolean) Whether to run in dryrun mode. Defaults to '${DRYRUN}'."
MASSES_STR="(Array of ints) Resonant masses."
CHANNELS_STR="(Array of strings) Channels."
SELECTIONS_STR="(Array of strings) Selection categories."
BASEDIR_STR="(String) Base directory."
function print_usage_submit_skims {
    USAGE="

    Run example: bash $(basename "$0") -t <some_tag>
                                      
    -h / --help       [${HELP_STR}]
    -m / --mode       [${MODE_STR}]
    -t / --tag        [${TAG_STR}]
    -b / --base       [${BASEDIR_STR}]
    -v / --var        [${VAR_STR}]
    -s / --signal     [${SIGNAL_STR}]
    -c / --channels   [${CHANNELS_STR}] 
    -m / --masses     [${MASSES_STR}]
    -l / --selections [${SELECTIONS_STR}] 
    --dryrun          [${DRYRUN_STR}]      

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
		--dryrun)
			DRYRUN="1"
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
    SELECTIONS=("res1b" "res2b" "boostedL_pnet")
fi

declare -a MHIGH;
for mass in ${MASSES[@]}; do
    if [ ${mass} -gt "319" ]; then
	MHIGH+=(${mass})
    fi
done

declare -a CATEGORIES_BOOST;
declare -a CATEGORIES_NOBOOST;
for sel in ${SELECTIONS[@]}; do
    if [[ ${sel} =~ .*boosted.* ]]; then
		CATEGORIES_BOOST+=(${sel})
    else
		CATEGORIES_NOBOOST+=(${sel})
    fi
done

LIMIT_DIR="${BASEDIR}/resonantLimits"
IDENTIFIER=".test_${SIGNAL}_${VAR}"

fit_opt=(
	"--verbose 1 "
	"--seed 0 "
	"--toys -1 "
	"--run expected "
	"--noFitAsimov "
	"--cminDefaultMinimizerType Minuit2 "
	"--cminDefaultMinimizerStrategy 0 "
	"--cminDefaultMinimizerTolerance 0.1 "
	"--cminFallbackAlgo Minuit2,0:0.2 "
	"--cminFallbackAlgo Minuit2,0:0.4 "
	"--X-rtd REMOVE_CONSTANT_ZERO_POINT=1 "
	"--freezeParameters SignalScale "
	# "--freezeParameters allConstrainedNuisances,SignalScale "
	# "--saveWorkspace "
)

if [ ${MODE} == "separate" ]; then
    
    for i in "${!CHANNELS[@]}"; do
		card_dir="${LIMIT_DIR}/cards_${TAG}/${CHANNELS[$i]}"
		cd ${card_dir}
	
		printf "Fitting categories in parallel for channel ${CHANNELS[$i]} "
		printf "(mode ${MODE}, var ${VAR})...\n"
		
		out_="combined_out"
		for sel in ${SELECTIONS[@]}; do
			cat_dir="${card_dir}/${sel}_${VAR}"
			out_dir="${cat_dir}/${out_}"
			parallel mkdir -p "${out_dir}" ::: ${MASSES[@]}
		done
		
		cat_dir_parallel="${card_dir}/{2}_${VAR}"
		out_dir_parallel="${cat_dir_parallel}/${out_}"
		
		# parallellize across the mass and category
		proc="${SIGNAL}_${VAR}"
		in_txt="${cat_dir_parallel}/comb.${proc}.txt"
		out_log="${out_dir_parallel}/comb.${proc}.log"

		# parallel rm -f -- ${out_log} ::: ${MASSES[@]} ::: ${SELECTIONS[@]}

		declare -a COMMS;
		for acat in "${CATEGORIES_BOOST[@]}"; do
			for amass in "${MHIGH[@]}"; do
				in_file=${in_txt//\{1\}/${amass}}
				in_file=${in_file//\{2\}/${acat}}
				out_file=${out_log//\{1\}/${amass}}
				out_file=${out_file//\{2\}/${acat}}

				comm_tmp=("combine "
						  "--method AsymptoticLimits ${in_file} "
						  "-n ${IDENTIFIER//\{1\}/${amass}}_${acat} "
						  "${fit_opt[@]}"
						  "-m ${amass} "
						  "> ${out_file}")
				COMMS+=("${comm_tmp[*]}")
			done
		done
		for acat in "${CATEGORIES_NOBOOST[@]}"; do
			for amass in "${MASSES[@]}"; do
				in_file=${in_txt//\{1\}/${amass}}
				in_file=${in_file//\{2\}/${acat}}
				out_file=${out_log//\{1\}/${amass}}
				out_file=${out_file//\{2\}/${acat}}

				comm_tmp=("combine "
						  "--method AsymptoticLimits ${in_file} "
						  "-n ${IDENTIFIER//\{1\}/${amass}}_${acat} "
						  "${fit_opt[@]}"
						  "-m ${amass} "
						  "> ${out_file}")
				COMMS+=("${comm_tmp[*]}")
			done
		done
		[[ ${DRYRUN} -eq 1 ]] && parallel echo {} ::: "${COMMS[@]}" || parallel -j $((`nproc` - 1)) {} ::: "${COMMS[@]}"
		
		cd -
    done

elif [ ${MODE} == "sel_group" ]; then
    
    for chn in "${CHANNELS[@]}"; do
		echo "Fitting channel ${chn} with mode ${MODE} (var ${VAR})..."
		card_dir="${LIMIT_DIR}/cards_${TAG}/${chn}"
		
		comb_dir="${card_dir}/comb_cat/AllCategories/"
		cd ${comb_dir}
		
		out_dir="${comb_dir}/combined_out"
		mkdir -p "${out_dir}"
		
		# parallellize across the mass
		proc="${SIGNAL}_${VAR}"
		in_txt="${comb_dir}/comb.${proc}.txt"
		out_log="${out_dir}/comb.${proc}.log"
		
		parallel -j 0 rm -f -- ${out_log} ::: ${MASSES[@]}
		
		for amass in "${MASSES[@]}"; do
			declare -a COMMS;
			comm_tmp=("combine "
					  "--method AsymptoticLimits ${in_txt//\{1\}/${amass}} "
					  "-n ${IDENTIFIER//\{1\}/${amass}}_${chn} "
					  "-m ${amass} "
					  "> ${out_log//\{1\}/${amass}}")
			COMMS+=("${comm_tmp[*]}")
		done
		[[ ${DRYRUN} -eq 1 ]] && parallel echo {} ::: "${COMMS[@]}" || parallel -j $((`nproc` - 1)) {} ::: "${COMMS[@]}"
		
		cd -
		
		# group categories according to selections passed by the user
		# for selp in ${SELECTIONS[@]}; do
		#     comb_dir="${card_dir}/comb_cat/"
		#     cd ${comb_dir}
		
		# 	comb_dir="${comb_dir}/${selp}_${VAR}/"
		
		#     out_dir="${comb_dir}/combined_out"
		#     parallel mkdir -p "${out_dir}" ::: ${MASSES[@]}
	    
		#     # parallellize across the mass
		#     proc="${SIGNAL}_${VAR}"
		#     in_txt="${comb_dir}/comb.${proc}.txt"
		#     out_log="${out_dir}/comb.${proc}.log"
	    
		#     parallel -j 0 rm -f -- ${out_log} ::: ${MASSES[@]}
		#     if [[ ${selp} =~ .*res.* ]]; then
		# 		parallel -j $((`nproc` - 1)) \
		# 					 combine -M AsymptoticLimits ${in_txt} \
		# 					 -n ${IDENTIFIER}_${chn} \
		# 					 --run blind \
		# 					 --noFitAsimov \
		# 					 --freezeParameters SignalScale \
		# 					 -m {1} ">" ${out_log} ::: ${MASSES[@]}
		#     else
		# 		parallel -j $((`nproc` - 1)) \
		# 					 combine -M AsymptoticLimits ${in_txt} \
		# 					 -n ${IDENTIFIER}_${chn} \
		# 					 --run blind \
		# 					 --noFitAsimov \
		# 					 --freezeParameters SignalScale \
		# 					 -m {1} ">" ${out_log} ::: ${MHIGH[@]}
		#     fi
	    
		#     cd -
	    
		# done
	done
	
elif [ ${MODE} == "chn_group" ]; then
    
    echo "Fitting category ${sel} with mode ${MODE} (var ${VAR})..."
    card_dir="${LIMIT_DIR}/cards_${TAG}_CombChn"
    cd ${card_dir}
    out_="combined_out"

    for sel in ${SELECTIONS[@]}; do
		comb_dir="${card_dir}/${sel}_${VAR}"
		out_dir="${comb_dir}/${out_}"
		parallel mkdir -p "${out_dir}" ::: ${MASSES[@]}
    done

    comb_dir_parallel="${card_dir}/{2}_${VAR}"
    out_dir_parallel="${comb_dir_parallel}/${out_}"
    
    # parallellize across the mass and categories
    proc="${SIGNAL}_${VAR}"
    in_txt="${comb_dir_parallel}/comb.${proc}.txt"
    out_log="${out_dir_parallel}/comb.${proc}.log"
    parallel -j 0 rm -f -- ${out_log} ::: ${MASSES[@]} ::: ${SELECTIONS[@]}

	declare -a COMMS;
	for acat in "${CATEGORIES_BOOST[@]}"; do
		for amass in "${MHIGH[@]}"; do
			comm_tmp=("combine "
					  "--method AsymptoticLimits ${in_txt//\{1\}/${amass}} "
					  "-n ${IDENTIFIER//\{1\}/${amass}}_${acat} "
					  "${fit_opt[@]} "
					  "-m ${amass} "
					  "> ${out_log//\{1\}/${amass}} ")
			COMMS+=("${comm_tmp[*]}")
		done
	done			  
	for acat in "${CATEGORIES_NOBOOST[@]}"; do
		for amass in "${MASSES[@]}"; do
			comm_tmp=("combine "
					  "--method AsymptoticLimits ${in_txt//\{1\}/${amass}} "
					  "-n ${IDENTIFIER//\{1\}/${amass}}_${acat} "
					  "${fit_opt[@]}"
					  "-m {amass} "
					  "> ${out_log//\{1\}/${amass}}")
			COMMS+=("${comm_tmp[*]}")
		done
	done
	[[ ${DRYRUN} -eq 1 ]] && parallel echo {} ::: "${COMMS[@]}" || parallel -j $((`nproc` - 1)) {} ::: "${COMMS[@]}"
	
    cd -

elif [ ${MODE} == "all_group" ]; then

    echo "Fitting all categories and channels (mode ${MODE}, var ${VAR})..."
    card_dir="${LIMIT_DIR}/cards_${TAG}_All"
    cd ${card_dir}

    out_dir="${card_dir}/combined_out"
    mkdir -p "${out_dir}"

    # parallellize across the mass
    proc="${SIGNAL}_${VAR}"
    in_txt="${card_dir}/comb.${proc}.txt"
    out_log="${out_dir}/comb.${proc}.log"

    parallel -j 0 rm -f -- ${out_log} ::: ${MASSES[@]}

	declare -a COMMS;
	for amass in "${MASSES[@]}"; do		
		comm_tmp=("combine "
				  "--method AsymptoticLimits ${in_txt//\{1\}/${amass}} "
				  "-n ${IDENTIFIER//\{1\}/${amass}}_all "
				  "${fit_opt[@]}"
				  "-m ${amass} "
				  "> ${out_log//\{1\}/${amass}}")
		COMMS+=("${comm_tmp[*]}")
	done
	[[ ${DRYRUN} -eq 1 ]] && parallel echo {} ::: "${COMMS[@]}" || parallel -j $((`nproc` - 1)) {} ::: "${COMMS[@]}"
	
elif [ ${MODE} == "sel_years" ]; then
    card_dir="${LIMIT_DIR}/cards_Run2_${VAR}_CombCat"
    cd ${card_dir}

    for chn in "${CHANNELS[@]}"; do
		echo "Channel ${chn}: Fitting all years and categories (mode ${MODE}, var ${VAR})..."
	
		comb_dir="${card_dir}/${chn}"
		out_dir="${comb_dir}/combined_out"
		mkdir -p "${out_dir}"
		
		proc="${SIGNAL}_${VAR}"
		in_txt="${comb_dir}/comb.${proc}.txt"
		out_log="${out_dir}/comb.${proc}.log"
		
		# parallellize across the mass
		parallel -j 0 rm -f -- ${out_log} ::: ${MASSES[@]}

		declare -a COMMS;
		for amass in "${MASSES[@]}"; do
			comm_tmp=("combine "
					  "--method AsymptoticLimits ${in_txt//\{1\}/${amass}}  "
					  "-n ${IDENTIFIER//\{1\}/${amass}}_years "
					  "${fit_opt[@]}"
					  "-m ${amass} "
					  "> ${out_log//\{1\}/${amass}} ")
			COMMS+=("${comm_tmp[*]}")
		done
		[[ ${DRYRUN} -eq 1 ]] && parallel echo {} ::: "${COMMS[@]}" || parallel -j $((`nproc` - 1)) {} ::: "${COMMS[@]}"
		
    done
    cd -

elif [ ${MODE} == "chn_years" ]; then
    echo "Selections: Fitting all years and channels (mode ${MODE}, var ${VAR})..."

    cd ${LIMIT_DIR}
	
    card_dir="${LIMIT_DIR}/cards_Run2_${VAR}_CombChn"
    for sel in "${SELECTIONS[@]}"; do
		comb_dir="${card_dir}/${sel}_${VAR}"
		out_dir="${comb_dir}/combined_out"
		parallel mkdir -p "${out_dir}" ::: ${MASSES[@]}
    done

    # parallellize across the mass
    proc="${SIGNAL}_${VAR}"

    comb_dir_parallel="${card_dir}/{2}_${VAR}"
    out_dir_parallel="${comb_dir_parallel}/combined_out"

    in_txt="${comb_dir_parallel}/comb.${proc}.txt"
    out_log="${out_dir_parallel}/comb.${proc}.log"

	declare -a COMMS;
	for acat in "${CATEGORIES_BOOST[@]}"; do
		for amass in "${MHIGH[@]}"; do
			comm_tmp=("combine "
					  "--method AsymptoticLimits ${in_txt//\{1\}/${amass}} "
					  "-n ${IDENTIFIER//\{1\}/${amass}}_${acat} "
					  "${fit_opt[@]}"
					  "-m ${amass} "
					  "> ${out_log//\{1\}/${amass}} ")
			COMMS+=("${comm_tmp[*]}")
		done
	done
	for acat in "${CATEGORIES_NOBOOST[@]}"; do
		for amass in "${MASSES[@]}"; do
			comm_tmp=("combine "
					  "--method AsymptoticLimits ${in_txt//\{1\}/${amass}} "
					  "-n ${IDENTIFIER//\{1\}/${amass}}_${acat} "
					  "${fit_opt[@]}"
					  "-m ${amass} "
					  "> ${out_log//\{1\}/${amass}}")
			COMMS+=("${comm_tmp}")
		done
	done
	[[ ${DRYRUN} -eq 1 ]] && parallel echo {} ::: "${COMMS[@]}" || parallel -j $((`nproc` - 1)) {} ::: "${COMMS[@]}"
	
    cd -

elif [ ${MODE} == "all_years" ]; then
    echo "Fitting all years, categories and channels (mode ${MODE}, var ${VAR})..."

	cd ${LIMIT_DIR}
	
    card_dir="${LIMIT_DIR}/cards_Final_CorrectBinning_Run2_All"
    out_dir="${card_dir}/combined_out"
    parallel mkdir -p "${out_dir}" ::: ${MASSES[@]}

    # parallellize across the mass
    proc="${SIGNAL}_${VAR}"
    in_txt="${card_dir}/comb.${proc}.txt"
    out_log="${out_dir}/comb.${proc}.log"

	# parallel -j 0 rm -f -- ${out_log} ::: ${MASSES[@]}

	declare -a COMMS;
	for amass in "${MASSES[@]}"; do
		comm_tmp=("combine "
				  "--method AsymptoticLimits ${in_txt//\{1\}/${amass}} "
				  "-n ${IDENTIFIER//\{1\}/${amass}}_years "
				  "${fit_opt[@]}"
				  "-m ${amass} "
				  "> ${out_log//\{1\}/${amass}}")
		COMMS+=("${comm_tmp[*]}")
	done
	[[ ${DRYRUN} -eq 1 ]] && parallel echo {} ::: "${COMMS[@]}" || parallel -j $((`nproc` - 1)) {} ::: "${COMMS[@]}"
fi

if [ ${DRYRUN} -eq 1 ]; then
    echo "Dry-run. The command above were not run."
fi
