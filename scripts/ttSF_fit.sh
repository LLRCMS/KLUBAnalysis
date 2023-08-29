#!/usr/bin/env bash

### Defaults
DRYRUN="0"
VALIDATION="0"
PER_CHANNEL="0"
TAG=""
VAR="DNNoutSM_kl_1"
PERIOD=""
PERIOD_CHOICES=( "UL16" "UL16APV" "UL17" "UL18" )

### Argument parsing
HELP_STR="Prints this help message."
DRYRUN_STR="(Boolean) Whether to run in dry-run mode. Defaults to '${DRYRUN}'."
VALIDATION_STR="(Boolean) Whether to perform the validation, which uses a control region \n\t\t\t\ttogether with a low DNN score signal region (defined in the configuration files). Required."
PER_CHANNEL_STR="(Boolean) Whether to perform an independent fit per channel. Defaults to '${PER_CHANNEL}'."
PERIOD_STR="(String) Which data period to consider: ${PERIOD_CHOICES[@]}. Defaults to '${PERIOD}'."
TAG_STR="(String) Defines tag for the output. Defaults to '${TAG}'."
VAR_STR="(String) Defines variable for the fit. Defaults to '${VAR}'."

function print_usage_CRlimits {
    USAGE="
    Run example: bash $(basename "$0") -t <some_tag>

    -h / --help         [${HELP_STR}]
    -d / --data_period  [${PERIOD_STR}]
    -t / --tag          [${TAG_STR}]
    --var               [${VAR_STR}]
    --valid             [${VALIDATION_STR}]
    -p / --per_channel  [${PER_CHANNEL_STR}]
    -n / --dryrun       [${DRYRUN_STR}]

"
    printf "${USAGE}"
}

while [[ $# -gt 0 ]]; do
    key=${1}
    case $key in
		-h|--help)
			print_usage_CRlimits
			exit 1
			;;
		-d|--data_period)
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
		-t|--tag)
			TAG=${2}
			shift; shift;
			;;
		--var)
			VAR=${2}
			shift; shift;
			;;
		-n|--dryrun)
			DRYRUN="1"
			shift;
			;;
		--valid)
			VALIDATION="1"
			shift;
			;;
		-p|--per_channel)
			PER_CHANNEL="1"
			shift;
			;;
		*)  # unknown option
			echo "Wrong parameter ${1}."
			exit 1
			;;
    esac
done

if [[ -z ${TAG} ]]; then
    echo "Select the tag via the '-t / --tag' option."
    exit 1;
fi
if [[ -z ${PERIOD} ]]; then
    echo "Select the data period via the '-d / --data_period' option."
    exit 1;
fi

declare -a CHANNELS=("ETau" "MuTau" "TauTau")
STOREDIR="/data_CMS/cms/${USER}/HHresonant_hist"
STOREDIR_NEW="${STOREDIR}/ttSF_${PERIOD}_${TAG}"
if [[ "${PER_CHANNEL}" -eq 1 ]]; then
	STOREDIR_NEW+="_chn"
fi
if [[ "${VALIDATION}" -eq 1 ]]; then
	STOREDIR_NEW+="_valid"
fi

WORKDIR="${HOME}/CMSSW_11_1_9/src/KLUBAnalysis/resonantLimits"

if [[ "${DRYRUN}" == "1" ]]; then
    CMD=echo
else
    CMD=''
fi

function yes_or_no {
    while true; do
        read -p "$* [y/n]: " yn
        case $yn in
            [Yy]*) return 0  ;;  
            [Nn]*) echo "Exit."; exit 1 ;;
        esac
    done
}

if [ "${DRYRUN}" == "0" ] && [ -d ${STOREDIR_NEW} ]; then
	echo "Directory ${STOREDIR_NEW} already exists."
	DELCMD="rm -r ${STOREDIR_NEW}"
	yes_or_no "Do you wish to remove it with: '${DELCMD}'?" && ${DELCMD}
fi

${CMD} mkdir ${STOREDIR_NEW}

if [[ "${PER_CHANNEL}" -eq 0 ]]; then
	# merge all channels within a data period
	for atype in "Plots" "Limits"; do
		${CMD} hadd ${STOREDIR_NEW}/combined_out"${atype}".root \
			   ${STOREDIR}/${TAG}_${CHANNELS[0]}/combined_out"${atype}".root \
			   ${STOREDIR}/${TAG}_${CHANNELS[1]}/combined_out"${atype}".root \
			   ${STOREDIR}/${TAG}_${CHANNELS[2]}/combined_out"${atype}".root
	done
fi

function run_hist_preparation() {
	comm="python ${WORKDIR}/prepare_histos.py "
	comm+=" -y ${PERIOD} "
	#comm+="--dynamQCD "
	comm+="$@"
	${CMD} ${comm}
}

echo ""
echo "#### Prepare Histograms ####"
echo ""

# remove empty bins, scale by acceptance, ...
declare -A PREPNAMES;
if [[ "${PER_CHANNEL}" -eq 0 ]]; then
	PREPNAMES["inclusive"]="prep_outLimits_${PERIOD}_inclusive_CR.root"
	run_hist_preparation -f ${STOREDIR_NEW}/combined_outLimits.root \
						 -o ${PREPNAMES["inclusive"]} \
						 -c "inclusive"
else
	for chn in ${CHANNELS[@]}; do
		PREPNAMES[${chn}]="prep_outLimits_${PERIOD}_${chn}_CR.root"
		run_hist_preparation -f ${STOREDIR}/${TAG}_${chn}/combined_outLimits.root \
							 -o ${PREPNAMES[${chn}]} \
							 --outdir ${STOREDIR_NEW} \
							 -c ${chn}
	done
fi

# any mainCfg can be used
declare -a SELECTIONS=("ttCR_invMcut")
if [[ "${VALIDATION}" -eq 1 ]]; then
	SELECTIONS+=("s1b1jresolvedMcutDNNcut" "s2b0jresolvedMcutDNNcut")
fi

CARDTAG="NOTAG"

function run_card_writing() {
	comm="python ${WORKDIR}/write_res_card.py "
	comm+="--indir ${STOREDIR_NEW} "
	comm+="--tag ${CARDTAG} "
	comm+="--period ${PERIOD} "
	comm+="--var ${VAR} "
	comm+="--selections ${SELECTIONS[@]} "
	comm+="--binbybin "
	comm+="--noShapeUnc "
	comm+="--theory "
	comm+="--isResonant "
	comm+="--masses 1000 " #dummy value (set to zero)
	comm+="$@"
	${CMD} ${comm}
}

echo ""
echo "#### Write Datacards ####"
echo ""

if [[ "${PER_CHANNEL}" -eq 0 ]]; then
	run_card_writing --filename ${STOREDIR_NEW}/${PREPNAMES["inclusive"]} \
					 --channel "inclusive" \
					 --config ${STOREDIR}/${TAG}_${CHANNELS[0]}/mainCfg_${CHANNELS[0]}_ttCR_UL18.cfg # any channel would work
else
	for chn in ${CHANNELS[@]}; do
		run_card_writing --filename ${STOREDIR_NEW}/${PREPNAMES[${chn}]} \
						 --channel ${chn} \
						 --config ${STOREDIR}/${TAG}_${chn}/mainCfg_${chn}_ttCR_UL18.cfg
	done
fi


function run_combine() {
	comm="combine -M FitDiagnostics comb.root --saveShapes --saveWithUncertainties "
	comm+="--setParameters r=0.00 --freezeParameters r --rMin 0 --rMax 0 "
	comm+="--redefineSignalPOI rate_TT "
	comm+="$@"
	${CMD} ${comm}
}

declare -a DATACARDS;
for sel in ${SELECTIONS[@]}; do
	DATACARDS+=(${sel}_${VAR}/hhres_*.NOSIGNAL.txt)
done

echo ""
echo "#### Fit ####"
echo ""

if [[ "${PER_CHANNEL}" -eq 0 ]]; then
	printf "\n++++++ Run combine for the inclusive channel ++++++"
	${CMD} cd ${STOREDIR_NEW}/cards_${CARDTAG}_inclusive/
	${CMD} combineCards.py ${DATACARDS[@]} > comb.txt
	${CMD} text2workspace.py comb.txt -o comb.root
	run_combine
else
	for chn in ${CHANNELS[@]}; do
		printf "\n++++++ Run combine for the ${chn} channel ++++++"
		${CMD} cd ${STOREDIR_NEW}/cards_${CARDTAG}_${chn}/
		${CMD} combineCards.py ${DATACARDS[@]} > comb.txt
		${CMD} text2workspace.py comb.txt -o comb.root
		run_combine
	done
fi
