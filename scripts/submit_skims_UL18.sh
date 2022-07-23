SUBMITSCRIPT="python scripts/skimNtuple.py"
SKIMDIR="/data_CMS/cms/${USER}/HHresonant_SKIMS"
mkdir -p "${SKIMDIR}"

######################
### Argument parsing
######################
help_str="prints this help message"
tag_str="select tag"
force_str="whether to override a folder with the same tag"
dataperiod_str="which data period to consider: Legacy18, UL18, ..."
function print_usage_submit_skims {
    usage=" $(basename "$0") [-h] [-t -f -d]
where:
    -h / --help        [ ${help_str} ]
    -f / --force       [ ${force_str} ]
    -t / --tag         [ ${tag_str} ]
    -d / --data_period [ ${dataperiod_str} ]

    Run example: $(basename "$0") -f
"
    printf "${usage}"
}

FORCE="0"
TAG=""
DATA_PERIOD="UL18"
while [[ $# -gt 0 ]]; do
    key="$1"

    case $key in
	-h|--help)
	    print_usage_submit_skims
	    exit 1
	    ;;
	-t|--tag)
	    TAG="$2"
	    shift # past argument
	    shift # past value
	    ;;
	-f|--force)
	    FORCE="1"
	    shift # past argument
	    shift # past value
	    ;;
	-d|--data_period)
	    DATA_PERIOD="$2"
	    shift # past argument
	    shift # past value
	    ;;
	*)  # unknown option
	    echo "Wrong parameter ${1}."
	    exit 1
	    ;;
    esac
done

# Verify if a tag was assigned by the user
# If not, print already existing tags (if any) and exits
if [[ -z "${TAG}" ]]; then
    echo "Select the tag via the '--tag' option."
    declare -a tags=( $(/bin/ls -1 "${SKIMDIR}") )
    if [ ${#tags[@]} -ne 0 ]; then
		echo "The following tags are currently available (the prefix is not part of the tag):"
		for tag in "${tags[@]}"; do
			echo "- ${tag}"
		done
    else
		echo "No tags are currently available. Everything looks clean!"
    fi
    exit 1;
fi

# Verify if a data period is defined
if [[ -z ${DATA_PERIOD} ]]; then
	echo "Select the data period via the '--d / --data_period' option."
	exit 1;
fi
	
#####################
### Setup variables
#####################
DATA_INPUT="/home/llr/cms/portales/hhbbtautau/KLUB_UL_20220321/CMSSW_11_1_9/src/KLUBAnalysis/inputFiles/"
SIGDIR=${DATA_INPUT}${DATA_PERIOD}"_signals/"
BKGDIR=${DATA_INPUT}${DATA_PERIOD}"_backgrounds/"
DATADIR=${DATA_INPUT}${DATA_PERIOD}"_data/"

PUSF="weights/PUreweight/UL_Run2_PU_SF/2018/PU_UL2018_SF.txt"
THECFG="config/skim_${DATA_PERIOD}.cfg"
OUTDIR="SKIMS_"${DATA_PERIOD}"_"${TAG}
OUTSKIMDIR="${SKIMDIR}/${OUTDIR}/"
PREF="SKIM_"

# source scripts/setup.sh
# source /opt/exp_soft/cms/t3/t3setup
mkdir -p ${OUTSKIMDIR}
# touch ${OUTSKIMDIR}"README.txt"
# echo "KLUB UL skimming" > $SKIMDIR/$OUTDIR/README.txt
cp scripts/listAll.sh ${OUTSKIMDIR}

######################
### Input files list
######################
declare -A InputList=(
	# Data
	["EGamma_Run2018A"]="1_EGamma__Run2018A-UL2018_MiniAODv2-v1.txt"
	["EGamma_Run2018B"]="2_EGamma__Run2018B-UL2018_MiniAODv2-v1.txt"
	["EGamma_Run2018C"]="3_EGamma__Run2018C-UL2018_MiniAODv2-v1.txt"
	["EGamma_Run2018D"]="4_EGamma__Run2018D-UL2018_MiniAODv2-v2.txt"

	["Tau_Run2018A"]="1_Tau__Run2018A-UL2018_MiniAODv2-v1.txt"
	["Tau_Run2018B"]="2_Tau__Run2018B-UL2018_MiniAODv2-v2.txt"
	["Tau_Run2018C"]="3_Tau__Run2018C-UL2018_MiniAODv2-v1.txt"
	["Tau_Run2018D"]="4_Tau__Run2018D-UL2018_MiniAODv2-v1.txt"

	["SingleMuon_Run2018A"]="5_SingleMuon__Run2018A-UL2018_MiniAODv2-v3.txt"
	["SingleMuon_Run2018B"]="6_SingleMuon__Run2018B-UL2018_MiniAODv2-v2.txt"
	["SingleMuon_Run2018C"]="7_SingleMuon__Run2018C-UL2018_MiniAODv2-v2.txt"
	["SingleMuon_Run2018D"]="8_SingleMuon__Run2018D-UL2018_MiniAODv2-v3.txt"

	["MET_Run2018A"]="9_MET__Run2018A-UL2018_MiniAODv2-v2.txt"
	["MET_Run2018B"]="10_MET__Run2018B-UL2018_MiniAODv2-v2.txt"
	["MET_Run2018C"]="11_MET__Run2018C-UL2018_MiniAODv2-v1.txt"
	["MET_Run2018D"]="12_MET__Run2018D-UL2018_MiniAODv2-v1.txt"

	# Signal
	["ggF_Radion_m250"]="48_GluGluToRadionToHHTo2B2Tau_M-250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m260"]="49_GluGluToRadionToHHTo2B2Tau_M-260_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["ggF_Radion_m270"]="50_GluGluToRadionToHHTo2B2Tau_M-270_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m280"]="51_GluGluToRadionToHHTo2B2Tau_M-280_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m300"]="52_GluGluToRadionToHHTo2B2Tau_M-300_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m320"]="53_GluGluToRadionToHHTo2B2Tau_M-320_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m350"]="54_GluGluToRadionToHHTo2B2Tau_M-350_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m400"]="55_GluGluToRadionToHHTo2B2Tau_M-400_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m450"]="56_GluGluToRadionToHHTo2B2Tau_M-450_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m500"]="57_GluGluToRadionToHHTo2B2Tau_M-500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m550"]="58_GluGluToRadionToHHTo2B2Tau_M-550_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m600"]="59_GluGluToRadionToHHTo2B2Tau_M-600_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m650"]="60_GluGluToRadionToHHTo2B2Tau_M-650_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m700"]="61_GluGluToRadionToHHTo2B2Tau_M-700_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m750"]="62_GluGluToRadionToHHTo2B2Tau_M-750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m800"]="63_GluGluToRadionToHHTo2B2Tau_M-800_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m850"]="64_GluGluToRadionToHHTo2B2Tau_M-850_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m900"]="65_GluGluToRadionToHHTo2B2Tau_M-900_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m1000"]="66_GluGluToRadionToHHTo2B2Tau_M-1000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m1250"]="67_GluGluToRadionToHHTo2B2Tau_M-1250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m1500"]="68_GluGluToRadionToHHTo2B2Tau_M-1500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m1750"]="69_GluGluToRadionToHHTo2B2Tau_M-1750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m2000"]="70_GluGluToRadionToHHTo2B2Tau_M-2000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m2500"]="71_GluGluToRadionToHHTo2B2Tau_M-2500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_Radion_m3000"]="72_GluGluToRadionToHHTo2B2Tau_M-3000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"

	["ggF_BulkGraviton_m250"]="73_GluGluToBulkGravitonToHHTo2B2Tau_M-250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m260"]="74_GluGluToBulkGravitonToHHTo2B2Tau_M-260_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m270"]="75_GluGluToBulkGravitonToHHTo2B2Tau_M-270_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m280"]="76_GluGluToBulkGravitonToHHTo2B2Tau_M-280_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m300"]="77_GluGluToBulkGravitonToHHTo2B2Tau_M-300_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m320"]="78_GluGluToBulkGravitonToHHTo2B2Tau_M-320_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m350"]="79_GluGluToBulkGravitonToHHTo2B2Tau_M-350_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m400"]="80_GluGluToBulkGravitonToHHTo2B2Tau_M-400_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m450"]="81_GluGluToBulkGravitonToHHTo2B2Tau_M-450_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m500"]="82_GluGluToBulkGravitonToHHTo2B2Tau_M-500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m550"]="83_GluGluToBulkGravitonToHHTo2B2Tau_M-550_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m600"]="84_GluGluToBulkGravitonToHHTo2B2Tau_M-600_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m650"]="85_GluGluToBulkGravitonToHHTo2B2Tau_M-650_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m700"]="86_GluGluToBulkGravitonToHHTo2B2Tau_M-700_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m750"]="87_GluGluToBulkGravitonToHHTo2B2Tau_M-750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m800"]="88_GluGluToBulkGravitonToHHTo2B2Tau_M-800_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m850"]="89_GluGluToBulkGravitonToHHTo2B2Tau_M-850_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m900"]="90_GluGluToBulkGravitonToHHTo2B2Tau_M-900_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m1000"]="91_GluGluToBulkGravitonToHHTo2B2Tau_M-1000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m1250"]="92_GluGluToBulkGravitonToHHTo2B2Tau_M-1250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m1500"]="93_GluGluToBulkGravitonToHHTo2B2Tau_M-1500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m1750"]="94_GluGluToBulkGravitonToHHTo2B2Tau_M-1750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m2000"]="95_GluGluToBulkGravitonToHHTo2B2Tau_M-2000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m2500"]="96_GluGluToBulkGravitonToHHTo2B2Tau_M-2500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggF_BulkGraviton_m3000"]="97_GluGluToBulkGravitonToHHTo2B2Tau_M-3000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"

	["VBF_Radion_m250"]="98_VBFToRadionToHHTo2B2Tau_M-250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m260"]="99_VBFToRadionToHHTo2B2Tau_M-260_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m270"]="100_VBFToRadionToHHTo2B2Tau_M-270_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m280"]="101_VBFToRadionToHHTo2B2Tau_M-280_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m300"]="102_VBFToRadionToHHTo2B2Tau_M-300_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m320"]="103_VBFToRadionToHHTo2B2Tau_M-320_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m350"]="104_VBFToRadionToHHTo2B2Tau_M-350_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m400"]="105_VBFToRadionToHHTo2B2Tau_M-400_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m450"]="106_VBFToRadionToHHTo2B2Tau_M-450_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m500"]="107_VBFToRadionToHHTo2B2Tau_M-500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m550"]="108_VBFToRadionToHHTo2B2Tau_M-550_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m600"]="109_VBFToRadionToHHTo2B2Tau_M-600_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m650"]="110_VBFToRadionToHHTo2B2Tau_M-650_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m700"]="111_VBFToRadionToHHTo2B2Tau_M-700_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m750"]="112_VBFToRadionToHHTo2B2Tau_M-750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m800"]="113_VBFToRadionToHHTo2B2Tau_M-800_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m850"]="114_VBFToRadionToHHTo2B2Tau_M-850_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m900"]="115_VBFToRadionToHHTo2B2Tau_M-900_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m1000"]="116_VBFToRadionToHHTo2B2Tau_M-1000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m1250"]="117_VBFToRadionToHHTo2B2Tau_M-1250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m1500"]="118_VBFToRadionToHHTo2B2Tau_M-1500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m1750"]="119_VBFToRadionToHHTo2B2Tau_M-1750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m2000"]="120_VBFToRadionToHHTo2B2Tau_M-2000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m2500"]="121_VBFToRadionToHHTo2B2Tau_M-2500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_Radion_m3000"]="122_VBFToRadionToHHTo2B2Tau_M-3000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"

	["VBF_BulkGraviton_m250"]="123_VBFToBulkGravitonToHHTo2B2Tau_M-250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m260"]="124_VBFToBulkGravitonToHHTo2B2Tau_M-260_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m270"]="125_VBFToBulkGravitonToHHTo2B2Tau_M-270_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m280"]="126_VBFToBulkGravitonToHHTo2B2Tau_M-280_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m300"]="127_VBFToBulkGravitonToHHTo2B2Tau_M-300_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m320"]="128_VBFToBulkGravitonToHHTo2B2Tau_M-320_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m350"]="129_VBFToBulkGravitonToHHTo2B2Tau_M-350_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m400"]="130_VBFToBulkGravitonToHHTo2B2Tau_M-400_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m450"]="131_VBFToBulkGravitonToHHTo2B2Tau_M-450_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m500"]="132_VBFToBulkGravitonToHHTo2B2Tau_M-500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m550"]="133_VBFToBulkGravitonToHHTo2B2Tau_M-550_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m600"]="134_VBFToBulkGravitonToHHTo2B2Tau_M-600_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m650"]="135_VBFToBulkGravitonToHHTo2B2Tau_M-650_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m700"]="136_VBFToBulkGravitonToHHTo2B2Tau_M-700_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m750"]="137_VBFToBulkGravitonToHHTo2B2Tau_M-750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m800"]="138_VBFToBulkGravitonToHHTo2B2Tau_M-800_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m850"]="139_VBFToBulkGravitonToHHTo2B2Tau_M-850_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m900"]="140_VBFToBulkGravitonToHHTo2B2Tau_M-900_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m1000"]="141_VBFToBulkGravitonToHHTo2B2Tau_M-1000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m1250"]="142_VBFToBulkGravitonToHHTo2B2Tau_M-1250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m1500"]="143_VBFToBulkGravitonToHHTo2B2Tau_M-1500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m1750"]="144_VBFToBulkGravitonToHHTo2B2Tau_M-1750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m2000"]="145_VBFToBulkGravitonToHHTo2B2Tau_M-2000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m2500"]="146_VBFToBulkGravitonToHHTo2B2Tau_M-2500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBF_BulkGraviton_m3000"]="147_VBFToBulkGravitonToHHTo2B2Tau_M-3000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"

	# Background
	["TT_fullyLep"]="1_TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["TT_semiLep"]="2_TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["TT_fullyHad"]="3_TTToHadronic_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"

	["WJets_HT_0_70"]="4_WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["WJets_HT_70_100"]="5_WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["WJets_HT_100_200"]="6_WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["WJets_HT_200_400"]="7_WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["WJets_HT_400_600"]="8_WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["WJets_HT_600_800"]="9_WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["WJets_HT_800_1200"]="10_WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["WJets_HT_1200_2500"]="11_WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["WJets_HT_2500_Inf"]="12_WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"

	["DY_merged"]="DYmerged.txt"
	["DY_incl"]="13_DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["DY_1j"]="14_DY1JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["DY_2j"]="15_DY2JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["DY_4j"]="16_DY4JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["DY_HT70To100"]="17_DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["DY_HT100To200"]="18_DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["DY_HT200To400"]="19_DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["DY_HT400To600"]="20_DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["DY_HT600To800"]="21_DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["DY_HT800To1200"]="22_DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["DY_HT1200To2500"]="23_DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["DY_HT2500ToInf"]="24_DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"

	["DY_NLO"]="1_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["DY_NLO_0j"]="2_DYJetsToLL_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["DY_NLO_1j"]="3_DYJetsToLL_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["DY_NLO_2j"]="4_DYJetsToLL_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["DY_NLO_PT50To100"]="1_DYJetsToLL_Pt-50To100_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["DY_NLO_PT100To250"]="2_DYJetsToLL_Pt-100To250_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["DY_NLO_PT250To400"]="3_DYJetsToLL_Pt-250To400_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["DY_NLO_PT400To650"]="4_DYJetsToLL_Pt-400To650_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["DY_NLO_PT650ToInf"]="5_DYJetsToLL_Pt-650ToInf_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"

	["WW"]="25_WW_TuneCP5_13TeV-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["WZ"]="26_WZ_TuneCP5_13TeV-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"

	["ST_tW_antitop"]="27_ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ST_tW_top"]="28_ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ST_tchannel_antitop"]="29_ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["ST_tchannel_top"]="30_ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"

	["EWKWMinus2Jets_WToLNu"]="31_EWKWMinus2Jets_WToLNu_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["EWKWPlus2Jets_WToLNu"]="32_EWKWPlus2Jets_WToLNu_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["EWKZ2Jets_ZToLL"]="33_EWKZ2Jets_ZToLL_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"

	["ZH_HTauTau"]="34_ZHToTauTau_M125_CP5_13TeV-powheg-pythia8_ext1__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["WplusHTauTau"]="35_WplusHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["WminusHTauTau"]="36_WminusHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["VBFHTauTau"]="37_VBFHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ggHTauTau"]="38_GluGluHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v3.txt"

	["ttHToNonBB"]="39_ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ttHToBB"]="40_ttHTobb_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["ttHToTauTau"]="41_ttHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v3.txt"
	["TTWJetsToLNu"]="42_TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["TTWJetsToQQ"]="43_TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["TTZToLLNuNu"]="44_TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
	["TTWW"]="45_TTWW_TuneCP5_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["TTZZ"]="46_TTZZ_TuneCP5_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
	["TTWZ"]="47_TTWZ_TuneCP5_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
)

###########################
### Submission commands ###
###########################
function BASE_COMMAND_SIGNAL() {
	local retval="${SUBMITSCRIPT} -T ${OUTDIR} -d True -s True -c ${THECFG} -q long -Y 2018 -k True -f ${FORCE}"
	echo ${retval}
}
function BASE_COMMAND() {
	local retval="$(BASE_COMMAND_SIGNAL) --pu ${PUSF}"
	echo ${retval}
}

#### DATA - filelists up to date
declare -a datasets=("EGamma" "Tau" "SingleMuon" "MET");
declare -a runs=("Run2018A" "Run2018B" "Run2018C" "Run2018D");
for ds in "${datasets[@]}"; do
	for run in "${runs[@]}"; do
		file="${ds}_${run}";
		$(BASE_COMMAND_SIGNAL) -n 10 -o ${OUTSKIMDIR}${PREF}${file} -i ${DATADIR}${InputList[${file}]}
	done
done
 
# #### HH resonant signal - all masspoints
# # spin-0 ggF
# unset datasets;
# datasets=("ggF_Radion" "ggF_BulkGraviton" "VBF_Radion" "VBF_BulkGraviton");
# declare -a masses=("250" "260" "270" "280" "300" "320" "350" "400" "450" "500" "550" "600" "650" "700" "750" "800" "850" "900" "1000" "1250" "1500" "1750" "2000" "2500" "3000");
# declare -a masses=("650");
# for ds in "${datasets[@]}"; do
# 	for mass in "${masses[@]}"; do
# 		file="${ds}_m${mass}";
# 		$(BASE_COMMAND) -n 20 -o ${OUTSKIMDIR}${PREF}${file} -i ${SIGDIR}${InputList[${file}]} -x 1.
# 	done
# done

# ##### TT - XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
# ### TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
# ### hh = 45.7%, ll = 10.5%, hl = 21.9% (x2 for permutation t-tbar)
# declare -A datasetsAssoc=(	["TT_fullyHad"]="377.96"
# 							["TT_fullyLep"]="88.29"
# 							["TT_semiLep"]="365.34"
# 						 )
# for ds in "${!datasetsAssoc[@]}"; do
# 	$(BASE_COMMAND) -n 100 -o ${OUTSKIMDIR}${PREF}${ds} -i $SIGDIR${InputList[${ds}]} -x ${datasetsAssoc[${ds}]};
# done

# #### DY - xsec from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#DY_Z
# $(BASE_COMMAND) -n 500 -o ${OUTSKIMDIR}${PREF}DYincl -i ${BKGDIR}${InputList["DY_incl"]} -x 6077.22
# $(BASE_COMMAND) -n 700 -o ${OUTSKIMDIR}${PREF}DYmerged -i ${BKGDIR}${InputList["DY_merged"]} -g True --DY True -x 6077.22
# unset datasets;
# datasets=("DY_1j" "DY_2j" "DY_4j" "DY_HT70To100" "DY_HT100To200" "DY_HT200To400" "DY_HT400To600" "DY_HT600To800" "DY_HT800To1200" "DY_HT1200To2500" "DY_HT2500ToInf");
# for ds in "${datasets[@]}"; do
# 	$(BASE_COMMAND) -n 300 -o ${OUTSKIMDIR}${PREF}"DYmerged" -i ${BKGDIR}${InputList[${ds}]} -g True --DY True -x 1.;
# done

# $(BASE_COMMAND) -n 1000 -o ${OUTSKIMDIR}${PREF}DY_NLO -i ${BKGDIR}${InputList["DY_NLO"]} -x 6077.22
# unset datasets;
# datasets=("DY_NLO_1j" "DY_NLO_2j" "DY_NLO_4j");
# for ds in "${datasets[@]}"; do
# 	$(BASE_COMMAND) -n 333 -o ${OUTSKIMDIR}${PREF}${ds} -i ${BKGDIR}${InputList[${ds}]} -x 1.;
# done

# unset datasets;
# datasets=("DY_NLO_PT50To100" "DY_NLO_PT100To250" "DY_NLO_PT250To400" "DY_NLO_PT400To650" "DY_NLO_PT650ToInf");
# for ds in ${datasets[@]}; do
# 	$(BASE_COMMAND) -n 200 -o ${OUTSKIMDIR}${PREF}${ds} -i ${BKGDIR}${InputList[${ds}]} -x 1.;
# done

# #### Wjets - filelists up to date
# unset datasetsAssoc;
# datasetsAssoc=(	["WJets_HT_0_70"]="-x 48917.48 -z 70"
# 				["WJets_HT_70_100"]="-x 1362"
# 				["WJets_HT_100_200"]="-x 1345"
# 				["WJets_HT_200_400"]="-x 359.7"
# 				["WJets_HT_400_600"]="-x 48.91"
# 				["WJets_HT_600_800"]="-x 12.05"
# 				["WJets_HT_800_1200"]="-x 5.501"
# 				["WJets_HT_1200_2500"]="-x 1.329"
# 				["WJets_HT_2500_Inf"]="-x 0.03216"
# 			  )
# for ds in "${!datasetsAssoc[@]}"; do
# 	$(BASE_COMMAND) -n 20 -o ${OUTSKIMDIR}${PREF}${ds} -i $BKGDIR${InputList[${ds}]} -y 1.213784 ${datasetsAssoc[${ds}]};
# done

# ##### ELECTROWEAK - XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
# ##### AND
# ##### single top - XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
# unset datasetsAssoc;
# datasetsAssoc=( ["EWKWPlus2Jets_WToLNu"]="25.62"
# 				["SKIM_EWKWMinus2Jets_WToLNu"]="20.25"
# 				["EWKZ2Jets_ZToLL"]="3.987"

# 				["ST_tW_antitop"]="35.85"
# 				["ST_tW_top"]="35.85"
# 				["ST_tchannel_antitop"]="80.95"
# 				["ST_tchannel_top"]="136.02"
# 			  )
# for ds in "${!datasetsAssoc[@]}"; do
# 	$(BASE_COMMAND) -n 50 -o ${OUTSKIMDIR}${PREF}${ds} -i ${BKGDIR}${InputList[${ds}]} -x ${datasetsAssoc[${ds}]};
# done

# ### SM Higgs - from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNHLHE2019
# # HXSWG: xs(ZH) = 0.880 pb, xs(W+H) = 0.831 pb, xs(W-H) = 0.527 pb, xs(ggH) = 48.61 pb, xs(VBFH) = 3.766 pb, xs(ttH) = 0.5071 pb
# # Z->qq : 69.91% , Z->ll : 3,3658% (x3 for all the leptons), H->bb : 57.7%  , H->tautau : 6.32%
# # ZH (Zll, Hbb) : XSBD (xs ZH * BR Z) * H->bb, ZH (Zqq, Hbb) : XSBD (xs ZH * BR Z) * H->bb
# # ZH (Zall, Htautau) : XS teor ZH * BR H->tautau
# unset datasetsAssoc;
# datasetsAssoc=( ["ggHTauTau"]="-n 30 61 -y 0.0632"
# 				["VBFHTauTau"]="-n 30 -x 3.766 -y 0.0632"
# 				["ZH_HTauTau"]="-n 30 -x 0.880 -y 0.0632"
# 				["WplusHTauTau"]="-n 30 -x 0.831 -y 0.0632"
# 				["WminusHTauTau"]="-n 30 -x 0.527 -y 0.0632"

# 				["ttHToNonBB"]="-n 30 -x 0.5071 -y 0.3598"
# 				["ttHToBB"]="-n 30 -x 0.5071 -y 0.577"
# 				["ttHToTauTau"]="-n 30 -x 0.5071 -y 0.0632"
				
# 				##### Multiboson: -  https://arxiv.org/abs/1408.5243 (WW), https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson (WZ,ZZ
# 				#### Some XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
# 				#### Some other XS taken from http://cms.cern.ch/iCMS/jsp/db_notes/noteInfo.jsp?cmsnoteid=CMS%20AN-2019/111
# 				["WW"]="-n 20 -x 118.7"
# 				["WZ"]="-n 20 -x 47.13"
# 				["ZZ"]="-n 20 -x 16.523"

# 				##### Others : - filelists up to date
# 				["TTWJetsToLNu"]="-n 20 -x 0.2043"
# 				["TTWJetsToQQ"]="-n 20 -x 0.4062"
# 				["TTZToLLNuNu"]="-n 20 -x 0.2529"
# 				["TTWW"]="-n 20 -x 0.006979"
# 				["TTZZ"]="-n 20 -x 0.001386"
# 				["TTWZ"]="-n 20 -x 0.00158"
# 			  )
# for ds in "${!datasetsAssoc[@]}"; do
# 	$(BASE_COMMAND) -o ${OUTSKIMDIR}${PREF}${ds} -i ${BKGDIR}${InputList[${ds}]} ${datasetsAssoc[${ds}]};
# done
