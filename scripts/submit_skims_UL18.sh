#!/usr/bin/env bash

### Argument parsing
HELP_STR="Prints this help message."
OUT_TAG_STR="Define tag for the output."
KLUB_TAG_STR="Choose tag for the klub input."
FORCE_STR="Whether to override a folder with the same tag. Defaults to false."
RUN_LISTS_STR="Whether to not run the list production script before each submission. Defaults to false."
DATAPERIOD_STR="Which data period to consider: Legacy18, UL18, ..."
function print_usage_submit_skims {
    USAGE=" $(basename "$0") [-H] [-t -f -d -n --klub_tag]

    -h / --help        [ ${HELP_STR} ]
    -t / --tag         [ ${OUT_TAG_STR} ]
    --klub_tag         [ ${KLUB_TAG_STR} ]
    -f / --force       [ ${FORCE_STR} ]
    -n / --no_lists    [ ${NO_LISTS_STR} ]
    -d / --data_period [ ${DATAPERIOD_STR} ]

    Run example: $(basename "$0") -t <some_tag> -f
"
    printf "${USAGE}"
}

FORCE="0"
NO_LISTS="0"
OUT_TAG=""
KLUB_TAG="Jul2022"
DATA_PERIOD="UL18"
while [[ $# -gt 0 ]]; do
    key=${1}
    case $key in
	-h|--help)
	    print_usage_submit_skims
	    exit 1
	    ;;
	-t|--tag)
	    OUT_TAG=${2}
	    shift; shift;
	    ;;
	--klub_tag)
	    KLUB_TAG=${2}
	    shift; shift;
	    ;;
	-f|--force)
	    FORCE="1"
	    shift;
	    ;;
	-n|--no_lists)
	    NO_LISTS="1"
	    shift;
	    ;;
	-d|--data_period)
	    DATA_PERIOD=${2}
	    shift; shift;
	    ;;
	*)  # unknown option
	    echo "Wrong parameter ${1}."
	    exit 1
	    ;;
    esac
done

### Setup variables
THIS_FILE="${BASH_SOURCE[0]}"
THIS_DIR="$( cd "$( dirname ${THIS_FILE} )" && pwd )"
KLUB_DIR="$( cd "$( dirname ${THIS_DIR} )" && pwd )"

SUBMIT_SCRIPT="scripts/skimNtuple.py"
LIST_SCRIPT="scripts/makeListOnStorage.py"
LIST_DIR="/dpm/in2p3.fr/home/cms/trivcat/store/user/lportale/HHNtuples_res/"${DATA_PERIOD}"/"
LIST_DATA_DIR=${LIST_DIR}"Data_"${KLUB_TAG}
LIST_MC_DIR=${LIST_DIR}"MC_"${KLUB_TAG}
declare -a LISTS_DATA=( $(/usr/bin/rfdir ${LIST_DATA_DIR} | awk '{{printf $9" "}}') )
declare -a LISTS_MC=(   $(/usr/bin/rfdir ${LIST_MC_DIR}   | awk '{{printf $9" "}}') )

SKIM_DIR="/data_CMS/cms/${USER}/HHresonant_SKIMS"
mkdir -p ${SKIM_DIR}

IN_DIR=${KLUB_DIR}"/inputFiles/"
SIG_DIR=${IN_DIR}${DATA_PERIOD}"_Signals/"
BKG_DIR=${IN_DIR}${DATA_PERIOD}"_Backgrounds/"
DATA_DIR=${IN_DIR}${DATA_PERIOD}"_Data/"

PU_DIR="weights/PUreweight/UL_Run2_PU_SF/2018/PU_UL2018_SF.txt"
CFG="config/skim_${DATA_PERIOD}.cfg"
PREF="SKIMS_"
OUT_DIR=${PREF}${DATA_PERIOD}"_"${OUT_TAG}
mkdir -p ${OUT_DIR}
ERR_FILE=${OUT_DIR}"/bad_patterns.o"
declare -a ERRORS=()
SEARCH_SPACE=".+\s.+" # trick to capture return values with error messages
OUTSKIM_DIR=${SKIM_DIR}/${OUT_DIR}/

declare -A IN_LIST DATA_MAP
declare -a DATA_LIST RUNS MASSES

### Argument parsing sanity checks
if [[ -z ${OUT_TAG} ]]; then
    printf "Select the tag via the '--tag' option. "
    declare -a tags=( $(/bin/ls -1 ${SKIM_DIR}) )
    if [ ${#tags[@]} -ne 0 ]; then
		echo "The following tags are currently available:"
		for tag in ${tags[@]}; do
			echo "- ${tag/${PREF}${DATA_PERIOD}_/}" # bash string substitution
		done
    else
		echo "No tags are currently available. Everything looks clean!"
    fi
    exit 1;
fi
if [[ -z ${DATA_PERIOD} ]]; then
	echo "Select the data period via the '--d / --data_period' option."
	exit 1;
fi

### Argument parsing: information for the user
echo "------ Arguments --------------"
echo "FORCE       = ${FORCE}"
echo "NO_LISTS    = ${NO_LISTS}"
echo "OUT_TAG     = ${OUT_TAG}"
echo "KLUB_TAG    = ${KLUB_TAG}"
echo "DATA_PERIOD = ${DATA_PERIOD}"
echo "-------------------------------"

#### Source additional setup
source scripts/setup.sh
source /opt/exp_soft/cms/t3/t3setup
mkdir -p ${OUTSKIM_DIR}
cp scripts/listAll.sh ${OUTSKIM_DIR}
echo "--------Run: $(date) ---------------" >> ${ERR_FILE}

### Submission command
function run_skim() {
	python ${KLUB_DIR}/${SUBMIT_SCRIPT} -T ${OUT_DIR} -s True -c ${KLUB_DIR}/${CFG} -q long -Y 2018 -k True --pu ${PU_DIR} -f ${FORCE} "$@"
}

### Input file list production command
function produce_list() {
	python ${KLUB_DIR}/${LIST_SCRIPT} -t ${KLUB_TAG} --data_period ${DATA_PERIOD} "$@"
}

### Extract sample full name
function find_sample() {
	nargs=$(( ${3}+3 ))
	if [ $# -ne ${nargs} ]; then
		echo "Wrong number of arguments - ${nargs} expected, $# provided"
        exit 1
    fi
	pattern=${1}
	list_dir=${2}
	lists=${@:4}

	sample=""
	nmatches=0
	for ldata in ${lists[@]}; do
		[[ ${ldata} =~ ${pattern} ]] && { sample=${BASH_REMATCH[0]}; nmatches=$(( ${nmatches} + 1 )); }
	done
	if [ ${nmatches} -eq 0 ]; then
		mes="The ${pattern} pattern was not found in ${list_dir} ."
		echo ${mes} >> ${ERR_FILE}
		echo ${mes}
		return 1
	elif [ ${nmatches} -gt 1 ]; then
		mes="The ${pattern} pattern had ${nmatches} matches in ${list_dir} ."
		echo ${mes} >> ${ERR_FILE}
		echo ${mes}
		return 1
	fi
	echo ${sample}
}

### Run on data samples
DATA_LIST=("EGamma" "Tau" "SingleMuon" "MET")
RUNS=("Run2018A" "Run2018B" "Run2018C" "Run2018D")
for ds in ${DATA_LIST[@]}; do
	for run in ${RUNS[@]}; do
		pattern="${ds}__${run}"
		sample=$(find_sample ${pattern} ${LIST_DATA_DIR} ${#LISTS_DATA[@]} ${LISTS_DATA[@]})
		if [[ ${sample} =~ ${SEARCH_SPACE} ]]; then
			ERRORS+=( ${sample} )
		else
			[[ ${NO_LISTS} -eq 0 ]] && produce_list --kind Data --sample ${sample}
		 	run_skim -n 10 --isdata True -o ${OUTSKIM_DIR}${PREF}${pattern} -i ${DATA_DIR} --sample ${sample}			
		fi
	done
done

### Run on HH resonant signal samples
DATA_LIST=( "GluGluToRad" "GluGluToBulkGrav" "VBFToRad" "VBFToBulkGrav" )
MASSES=("250" "260" "270" "280" "300" "320" "350" "400" "450" "500" "550" "600" "650" "700" "750" "800" "850" "900" "1000" "1250" "1500" "1750" "2000" "2500" "3000")
for ds in ${DATA_LIST[@]}; do
	for mass in ${MASSES[@]}; do
		pattern="${ds}.+_M-${mass}_";
		sample=$(find_sample ${pattern} ${LIST_MC_DIR} ${#LISTS_MC[@]} ${LISTS_MC[@]})
		if [[ ${sample} =~ ${SEARCH_SPACE} ]]; then
			ERRORS+=( ${sample} )
		else
			[[ ${NO_LISTS} -eq 0 ]] && produce_list --kind Signals --sample ${sample}
			run_skim -n 20 -o ${OUTSKIM_DIR}${PREF}${pattern} -i ${SIG_DIR} --sample ${sample} -x 1.
		fi
	done
done

### Run on backgrounds samples
DATA_MAP=(
	["TTToHadronic"]="-n 100 -x 377.96"
	["TTTo2L2Nu"]="-n 100 -x 88.29"
	["TTToSemiLeptonic"]="-n 100 -x 365.34"

	["DYJets.+_M-50_T.+amc"]="-n 1000 -x 6077.22 -g True --DY True" # inclusive NLO
	## ["DY_merged"]="-n 300 -x 6077.22 -g True --DY True"
	# ["DY1J"]="-n 333 -x 1. -g True --DY True"
	# ["DY2J"]="-n 333 -x 1. -g True --DY True"		   
	# ["DY3J"]="-n 333 -x 1. -g True --DY True"
	# ["DY4J"]="-n 333 -x 1. -g True --DY True"
	# ["DYJetsToLL_M-50_HT-70to100"]="-n 300 -x 1. -g True --DY True"
	# ["DYJetsToLL_M-50_HT-100to200"]="-n 300 -x 1. -g True --DY True"
	# ["DYJetsToLL_M-50_HT-200to400"]="-n 300 -x 1. -g True --DY True"
	# ["DYJetsToLL_M-50_HT-400to600"]="-n 300 -x 1. -g True --DY True"
	# ["DYJetsToLL_M-50_HT-600to800"]="-n 300 -x 1. -g True --DY True"
	# ["DYJetsToLL_M-50_HT-800to1200"]="-n 300 -x 1. -g True --DY True"
	# ["DYJetsToLL_M-50_HT-1200to2500"]="-n 300 -x 1. -g True --DY True"
	# ["DYJetsToLL_M-50_HT-2500toInf"]="-n 300 -x 1. -g True --DY True"
	# ["DYJetsToLL_Pt-50To100"]="-n 200 -x 1. -g True --DY True"
	# ["DYJetsToLL_Pt-100To250"]="-n 200 -x 1. -g True --DY True"
	# ["DYJetsToLL_Pt-250To400"]="-n 200 -x 1. -g True --DY True"
	# ["DYJetsToLL_Pt-400To650"]="-n 200 -x 1. -g True --DY True"
	# ["DYJetsToLL_Pt-650ToInf"]="-n 200 -x 1. -g True --DY True"

	["WJetsToLNu_T.+madgraph"]="-n 20 -x 48917.48 -y 1.213784 -z 70" # for 0 < HT < 70
	["WJetsToLNu_HT-70To100"]="-n 20 -x 1362 -y 1.213784"
	["WJetsToLNu_HT-100To200"]="-n 20 -x 1345 -y 1.213784"
	["WJetsToLNu_HT-200To400"]="-n 20 -x 359.7 -y 1.213784"
	["WJetsToLNu_HT-400To600"]="-n 20 -x 48.91 -y 1.213784"
	["WJetsToLNu_HT-600To800"]="-n 20 -x 12.05 -y 1.213784"
	["WJetsToLNu_HT-800To1200"]="-n 20 -x 5.501 -y 1.213784"
	["WJetsToLNu_HT-1200To2500"]="-n 20 -x 1.329 -y 1.213784"
	["WJetsToLNu_HT-2500ToInf"]="-n 20 -x 0.03216 -y 1.213784"

	["EWKWPlus2Jets_WToLNu"]="-n 50 -x 25.62"
	["EWKWMinus2Jets_WToLNu"]="-n 50 -x 20.25"
	["EWKZ2Jets_ZToLL"]="-n 50 -x 3.987"

	["ST_tW_antitop"]="-n 50 -x 35.85"
	["ST_tW_top"]="-n 50 -x 35.85"
	["ST_t-channel_antitop"]="-n 50 -x 80.95"
	["ST_t-channel_top"]="-n 50 -x 136.02"

	["GluGluHToTauTau"]="-n 30 -x 48.61 -y 0.0632"
	["VBFHToTauTau"]="-n 30 -x 3.766 -y 0.0632"
	["ZHToTauTau"]="-n 30 -x 0.880 -y 0.0632"
	["WplusHToTauTau"]="-n 30 -x 0.831 -y 0.0632"
	["WminusHToTauTau"]="-n 30 -x 0.527 -y 0.0632"

	["ttHToNonbb"]="-n 30 -x 0.5071 -y 0.3598"
	["ttHTobb"]="-n 30 -x 0.5071 -y 0.577"
	["ttHToTauTau"]="-n 30 -x 0.5071 -y 0.0632"
	
	["_WW"]="-n 20 -x 118.7"
	["_WZ"]="-n 20 -x 47.13"
	# ["_ZZ"]="-n 20 -x 16.523"

	["TTWJetsToLNu"]="-n 20 -x 0.2043"
	["TTWJetsToQQ"]="-n 20 -x 0.4062"
	["TTZToLLNuNu"]="-n 20 -x 0.2529"
	["TTWW"]="-n 20 -x 0.006979"
	["TTZZ"]="-n 20 -x 0.001386"
	["TTWZ"]="-n 20 -x 0.00158"
)
for ds in ${!DATA_MAP[@]}; do
	sample=$(find_sample ${ds} ${LIST_MC_DIR} ${#LISTS_MC[@]} ${LISTS_MC[@]})
	if [[ ${sample} =~ ${SEARCH_SPACE} ]]; then
		ERRORS+=( ${sample} )
	else
		[[ ${NO_LISTS} -eq 0 ]] && produce_list --kind Backgrounds --sample ${sample}
		run_skim -o ${OUTSKIM_DIR}${PREF}${ds} -i ${BKG_DIR} --sample ${sample} ${DATA_MAP[${ds}]}
	fi
done

### Print pattern matching issues
nerr=${#ERRORS[@]}
if [ ${nerr} -ne 0 ]; then
	echo "ATTENTION: The following pattern matching errors were observed:"
fi
for ((i = 0; i < ${nerr}; i++)); do
    echo "  - ${ERRORS[$i]}"
done

###### Cross-section information ######

### TT
# xsec from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
# TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
# hh = 45.7%, ll = 10.5%, hl = 21.9% (x2 for permutation t-tbar)

### DY
# xsec from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#DY_Z

### Electroweak
# xsec from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109

### Single Top
# xsec from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109

### SM Higgs
# from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNHLHE2019

### HXSWG: xs(ZH) = 0.880 pb, xs(W+H) = 0.831 pb, xs(W-H) = 0.527 pb, xs(ggH) = 48.61 pb, xs(VBFH) = 3.766 pb, xs(ttH) = 0.5071 pb
# Z->qq : 69.91% , Z->ll : 3,3658% (x3 for all the leptons), H->bb : 57.7%  , H->tautau : 6.32%
# ZH (Zll, Hbb) : XSBD (xs ZH * BR Z) * H->bb, ZH (Zqq, Hbb) : XSBD (xs ZH * BR Z) * H->bb
# ZH (Zall, Htautau) : XS teor ZH * BR H->tautau

### Multiboson
# xsec from https://arxiv.org/abs/1408.5243 (WW), https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson (WZ,ZZ)
# Some XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
# Some other XS taken from http://cms.cern.ch/iCMS/jsp/db_notes/noteInfo.jsp?cmsnoteid=CMS%20AN-2019/111
