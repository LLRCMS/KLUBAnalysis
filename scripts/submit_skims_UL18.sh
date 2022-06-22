#########################
### SKIMS UL 2018 ###
#########################
AMESSAGE="KLUB UL skimming"

### SETUP

SIGDIR="/home/llr/cms/portales/hhbbtautau/KLUB_UL_20220321/CMSSW_11_1_9/src/KLUBAnalysis/inputFiles/UL18_signals/"
BKGDIR="/home/llr/cms/portales/hhbbtautau/KLUB_UL_20220321/CMSSW_11_1_9/src/KLUBAnalysis/inputFiles/UL18_backgrounds"
DATADIR="/home/llr/cms/portales/hhbbtautau/KLUB_UL_20220321/CMSSW_11_1_9/src/KLUBAnalysis/inputFiles/UL18_data/"

SUBMITSCRIPT="python scripts/skimNtuple.py"
PUSF="weights/PUreweight/UL_Run2_PU_SF/2018/PU_UL2018_SF.txt"
THECFG="config/skim_UL18.cfg"

SKIMDIR="/data_CMS/cms/alves/HHresonant_SKIMS"
OUTDIR="SKIMS_UL18_JuneTests"

source scripts/setup.sh
source /opt/exp_soft/cms/t3/t3setup
mkdir -p $SKIMDIR/$OUTDIR/
touch $SKIMDIR/$OUTDIR/README.txt
echo $AMESSAGE > $SKIMDIR/$OUTDIR/README.txt
cp scripts/listAll.sh $SKIMDIR/$OUTDIR/

### INPUT FILE LISTS
declare -A InputList

# Data
InputList["EGamma_Run2018A"]="1_EGamma__Run2018A-UL2018_MiniAODv2-v1.txt"
InputList["EGamma_Run2018B"]="2_EGamma__Run2018B-UL2018_MiniAODv2-v1.txt"
InputList["EGamma_Run2018C"]="3_EGamma__Run2018C-UL2018_MiniAODv2-v1.txt"
InputList["EGamma_Run2018D"]="4_EGamma__Run2018D-UL2018_MiniAODv2-v2.txt"

InputList["Tau_Run2018A"]="1_Tau__Run2018A-UL2018_MiniAODv2-v1.txt"
InputList["Tau_Run2018B"]="2_Tau__Run2018B-UL2018_MiniAODv2-v2.txt"
InputList["Tau_Run2018C"]="3_Tau__Run2018C-UL2018_MiniAODv2-v1.txt"
InputList["Tau_Run2018D"]="4_Tau__Run2018D-UL2018_MiniAODv2-v1.txt"

InputList["SingleMuon_Run2018A"]="5_SingleMuon__Run2018A-UL2018_MiniAODv2-v3.txt"
InputList["SingleMuon_Run2018B"]="6_SingleMuon__Run2018B-UL2018_MiniAODv2-v2.txt"
InputList["SingleMuon_Run2018C"]="7_SingleMuon__Run2018C-UL2018_MiniAODv2-v2.txt"
InputList["SingleMuon_Run2018D"]="8_SingleMuon__Run2018D-UL2018_MiniAODv2-v3.txt"

InputList["MET_Run2018A"]="9_MET__Run2018A-UL2018_MiniAODv2-v2.txt"
InputList["MET_Run2018B"]="10_MET__Run2018B-UL2018_MiniAODv2-v2.txt"
InputList["MET_Run2018C"]="11_MET__Run2018C-UL2018_MiniAODv2-v1.txt"
InputList["MET_Run2018D"]="12_MET__Run2018D-UL2018_MiniAODv2-v1.txt"

#Signal
InputList["ggF_Radion_m250"]="48_GluGluToRadionToHHTo2B2Tau_M-250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m260"]="49_GluGluToRadionToHHTo2B2Tau_M-260_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["ggF_Radion_m270"]="50_GluGluToRadionToHHTo2B2Tau_M-270_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m280"]="51_GluGluToRadionToHHTo2B2Tau_M-280_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m300"]="52_GluGluToRadionToHHTo2B2Tau_M-300_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m320"]="53_GluGluToRadionToHHTo2B2Tau_M-320_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m350"]="54_GluGluToRadionToHHTo2B2Tau_M-350_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m400"]="55_GluGluToRadionToHHTo2B2Tau_M-400_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m450"]="56_GluGluToRadionToHHTo2B2Tau_M-450_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m500"]="57_GluGluToRadionToHHTo2B2Tau_M-500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m550"]="58_GluGluToRadionToHHTo2B2Tau_M-550_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m600"]="59_GluGluToRadionToHHTo2B2Tau_M-600_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m650"]="60_GluGluToRadionToHHTo2B2Tau_M-650_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m700"]="61_GluGluToRadionToHHTo2B2Tau_M-700_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m750"]="62_GluGluToRadionToHHTo2B2Tau_M-750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m800"]="63_GluGluToRadionToHHTo2B2Tau_M-800_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m850"]="64_GluGluToRadionToHHTo2B2Tau_M-850_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m900"]="65_GluGluToRadionToHHTo2B2Tau_M-900_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m1000"]="66_GluGluToRadionToHHTo2B2Tau_M-1000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m1250"]="67_GluGluToRadionToHHTo2B2Tau_M-1250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m1500"]="68_GluGluToRadionToHHTo2B2Tau_M-1500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m1750"]="69_GluGluToRadionToHHTo2B2Tau_M-1750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m2000"]="70_GluGluToRadionToHHTo2B2Tau_M-2000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m2500"]="71_GluGluToRadionToHHTo2B2Tau_M-2500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_Radion_m3000"]="72_GluGluToRadionToHHTo2B2Tau_M-3000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"

InputList["ggF_BulkGraviton_m250"]="73_GluGluToBulkGravitonToHHTo2B2Tau_M-250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m260"]="74_GluGluToBulkGravitonToHHTo2B2Tau_M-260_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m270"]="75_GluGluToBulkGravitonToHHTo2B2Tau_M-270_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m280"]="76_GluGluToBulkGravitonToHHTo2B2Tau_M-280_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m300"]="77_GluGluToBulkGravitonToHHTo2B2Tau_M-300_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m320"]="78_GluGluToBulkGravitonToHHTo2B2Tau_M-320_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m350"]="79_GluGluToBulkGravitonToHHTo2B2Tau_M-350_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m400"]="80_GluGluToBulkGravitonToHHTo2B2Tau_M-400_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m450"]="81_GluGluToBulkGravitonToHHTo2B2Tau_M-450_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m500"]="82_GluGluToBulkGravitonToHHTo2B2Tau_M-500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m550"]="83_GluGluToBulkGravitonToHHTo2B2Tau_M-550_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m600"]="84_GluGluToBulkGravitonToHHTo2B2Tau_M-600_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m650"]="85_GluGluToBulkGravitonToHHTo2B2Tau_M-650_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m700"]="86_GluGluToBulkGravitonToHHTo2B2Tau_M-700_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m750"]="87_GluGluToBulkGravitonToHHTo2B2Tau_M-750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m800"]="88_GluGluToBulkGravitonToHHTo2B2Tau_M-800_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m850"]="89_GluGluToBulkGravitonToHHTo2B2Tau_M-850_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m900"]="90_GluGluToBulkGravitonToHHTo2B2Tau_M-900_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m1000"]="91_GluGluToBulkGravitonToHHTo2B2Tau_M-1000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m1250"]="92_GluGluToBulkGravitonToHHTo2B2Tau_M-1250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m1500"]="93_GluGluToBulkGravitonToHHTo2B2Tau_M-1500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m1750"]="94_GluGluToBulkGravitonToHHTo2B2Tau_M-1750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m2000"]="95_GluGluToBulkGravitonToHHTo2B2Tau_M-2000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m2500"]="96_GluGluToBulkGravitonToHHTo2B2Tau_M-2500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggF_BulkGraviton_m3000"]="97_GluGluToBulkGravitonToHHTo2B2Tau_M-3000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"

InputList["VBF_Radion_m250"]="98_VBFToRadionToHHTo2B2Tau_M-250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m260"]="99_VBFToRadionToHHTo2B2Tau_M-260_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m270"]="100_VBFToRadionToHHTo2B2Tau_M-270_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m280"]="101_VBFToRadionToHHTo2B2Tau_M-280_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m300"]="102_VBFToRadionToHHTo2B2Tau_M-300_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m320"]="103_VBFToRadionToHHTo2B2Tau_M-320_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m350"]="104_VBFToRadionToHHTo2B2Tau_M-350_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m400"]="105_VBFToRadionToHHTo2B2Tau_M-400_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m450"]="106_VBFToRadionToHHTo2B2Tau_M-450_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m500"]="107_VBFToRadionToHHTo2B2Tau_M-500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m550"]="108_VBFToRadionToHHTo2B2Tau_M-550_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m600"]="109_VBFToRadionToHHTo2B2Tau_M-600_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m650"]="110_VBFToRadionToHHTo2B2Tau_M-650_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m700"]="111_VBFToRadionToHHTo2B2Tau_M-700_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m750"]="112_VBFToRadionToHHTo2B2Tau_M-750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m800"]="113_VBFToRadionToHHTo2B2Tau_M-800_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m850"]="114_VBFToRadionToHHTo2B2Tau_M-850_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m900"]="115_VBFToRadionToHHTo2B2Tau_M-900_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m1000"]="116_VBFToRadionToHHTo2B2Tau_M-1000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m1250"]="117_VBFToRadionToHHTo2B2Tau_M-1250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m1500"]="118_VBFToRadionToHHTo2B2Tau_M-1500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m1750"]="119_VBFToRadionToHHTo2B2Tau_M-1750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m2000"]="120_VBFToRadionToHHTo2B2Tau_M-2000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m2500"]="121_VBFToRadionToHHTo2B2Tau_M-2500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_Radion_m3000"]="122_VBFToRadionToHHTo2B2Tau_M-3000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"

InputList["VBF_BulkGraviton_m250"]="123_VBFToBulkGravitonToHHTo2B2Tau_M-250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m260"]="124_VBFToBulkGravitonToHHTo2B2Tau_M-260_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m270"]="125_VBFToBulkGravitonToHHTo2B2Tau_M-270_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m280"]="126_VBFToBulkGravitonToHHTo2B2Tau_M-280_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m300"]="127_VBFToBulkGravitonToHHTo2B2Tau_M-300_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m320"]="128_VBFToBulkGravitonToHHTo2B2Tau_M-320_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m350"]="129_VBFToBulkGravitonToHHTo2B2Tau_M-350_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m400"]="130_VBFToBulkGravitonToHHTo2B2Tau_M-400_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m450"]="131_VBFToBulkGravitonToHHTo2B2Tau_M-450_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m500"]="132_VBFToBulkGravitonToHHTo2B2Tau_M-500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m550"]="133_VBFToBulkGravitonToHHTo2B2Tau_M-550_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m600"]="134_VBFToBulkGravitonToHHTo2B2Tau_M-600_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m650"]="135_VBFToBulkGravitonToHHTo2B2Tau_M-650_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m700"]="136_VBFToBulkGravitonToHHTo2B2Tau_M-700_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m750"]="137_VBFToBulkGravitonToHHTo2B2Tau_M-750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m800"]="138_VBFToBulkGravitonToHHTo2B2Tau_M-800_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m850"]="139_VBFToBulkGravitonToHHTo2B2Tau_M-850_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m900"]="140_VBFToBulkGravitonToHHTo2B2Tau_M-900_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m1000"]="141_VBFToBulkGravitonToHHTo2B2Tau_M-1000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m1250"]="142_VBFToBulkGravitonToHHTo2B2Tau_M-1250_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m1500"]="143_VBFToBulkGravitonToHHTo2B2Tau_M-1500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m1750"]="144_VBFToBulkGravitonToHHTo2B2Tau_M-1750_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m2000"]="145_VBFToBulkGravitonToHHTo2B2Tau_M-2000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m2500"]="146_VBFToBulkGravitonToHHTo2B2Tau_M-2500_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBF_BulkGraviton_m3000"]="147_VBFToBulkGravitonToHHTo2B2Tau_M-3000_TuneCP5_PSWeights_narrow_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"

# Background
InputList["TT_fullyLep"]="1_TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["TT_semiLep"]="2_TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["TT_fullyHad"]="3_TTToHadronic_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"

InputList["WJets_HT_0_70"]="4_WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["WJets_HT_70_100"]="5_WJetsToLNu_HT-70To100_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["WJets_HT_100_200"]="6_WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["WJets_HT_200_400"]="7_WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["WJets_HT_400_600"]="8_WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["WJets_HT_600_800"]="9_WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["WJets_HT_800_1200"]="10_WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["WJets_HT_1200_2500"]="11_WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["WJets_HT_2500_Inf"]="12_WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"

InputList["DY_merged"]="DYmerged.txt"
InputList["DY_incl"]="13_DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["DY_1j"]="14_DY1JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_2j"]="15_DY2JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_4j"]="16_DY4JetsToLL_M-50_MatchEWPDG20_TuneCP5_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_HT70To100"]="17_DYJetsToLL_M-50_HT-70to100_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_HT100To200"]="18_DYJetsToLL_M-50_HT-100to200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_HT200To400"]="19_DYJetsToLL_M-50_HT-200to400_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_HT400To600"]="20_DYJetsToLL_M-50_HT-400to600_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_HT600To800"]="21_DYJetsToLL_M-50_HT-600to800_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_HT800To1200"]="22_DYJetsToLL_M-50_HT-800to1200_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_HT1200To2500"]="23_DYJetsToLL_M-50_HT-1200to2500_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_HT2500ToInf"]="24_DYJetsToLL_M-50_HT-2500toInf_TuneCP5_PSweights_13TeV-madgraphMLM-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"

InputList["DY_NLO"]="1_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["DY_NLO_0j"]="2_DYJetsToLL_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_NLO_1j"]="3_DYJetsToLL_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_NLO_2j"]="4_DYJetsToLL_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_NLO_PT50To100"]="1_DYJetsToLL_Pt-50To100_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_NLO_PT100To250"]="2_DYJetsToLL_Pt-100To250_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_NLO_PT250To400"]="3_DYJetsToLL_Pt-250To400_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_NLO_PT400To650"]="4_DYJetsToLL_Pt-400To650_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_NLO_PT650ToInf"]="5_DYJetsToLL_Pt-650ToInf_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"

InputList["WW"]="25_WW_TuneCP5_13TeV-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["WZ"]="26_WZ_TuneCP5_13TeV-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"

InputList["ST_tW_antitop"]="27_ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ST_tW_top"]="28_ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ST_tchannel_antitop"]="29_ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["ST_tchannel_top"]="30_ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"

InputList["EWKWMinus2Jets_WToLNu"]="31_EWKWMinus2Jets_WToLNu_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["EWKWPlus2Jets_WToLNu"]="32_EWKWPlus2Jets_WToLNu_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["EWKZ2Jets_ZToLL"]="33_EWKZ2Jets_ZToLL_M-50_TuneCP5_withDipoleRecoil_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"

InputList["ZH_HTauTau"]="34_ZHToTauTau_M125_CP5_13TeV-powheg-pythia8_ext1__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["WplusHTauTau"]="35_WplusHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["WminusHTauTau"]="36_WminusHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["VBFHTauTau"]="37_VBFHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ggHTauTau"]="38_GluGluHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v3.txt"

InputList["ttHToNonBB"]="39_ttHToNonbb_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ttHToBB"]="40_ttHTobb_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["ttHToTauTau"]="41_ttHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v3.txt"
InputList["TTWJetsToLNu"]="42_TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["TTWJetsToQQ"]="43_TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["TTZToLLNuNu"]="44_TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["TTWW"]="45_TTWW_TuneCP5_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["TTZZ"]="46_TTZZ_TuneCP5_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["TTWZ"]="47_TTWZ_TuneCP5_13TeV-madgraph-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"


###########################
### Submission commands ###
###########################

######################
#### DATA - filelists up to date
#$SUBMITSCRIPT -T $OUTDIR -d True -s True -c $THECFG -n 50 -q long -Y 2018 -o $SKIMDIR/$OUTDIR/SKIM_EGamma_Run2018A -k True -i $DATADIR/${InputList["EGamma_Run2018A"]}
#$SUBMITSCRIPT -T $OUTDIR -d True -s True -c $THECFG -n 50 -q long -Y 2018 -o $SKIMDIR/$OUTDIR/SKIM_EGamma_Run2018B -k True -i $DATADIR/${InputList["EGamma_Run2018B"]}
#$SUBMITSCRIPT -T $OUTDIR -d True -s True -c $THECFG -n 50 -q long -Y 2018 -o $SKIMDIR/$OUTDIR/SKIM_EGamma_Run2018C -k True -i $DATADIR/${InputList["EGamma_Run2018C"]}
#$SUBMITSCRIPT -T $OUTDIR -d True -s True -c $THECFG -n 50 -q long -Y 2018 -o $SKIMDIR/$OUTDIR/SKIM_EGamma_Run2018D -k True -i $DATADIR/${InputList["EGamma_Run2018D"]}
#
#$SUBMITSCRIPT -T $OUTDIR -d True -s True -c $THECFG -n 50 -q long -Y 2018 -o $SKIMDIR/$OUTDIR/SKIM_Tau_Run2018A -k True -i $DATADIR/${InputList["Tau_Run2018A"]}
#$SUBMITSCRIPT -T $OUTDIR -d True -s True -c $THECFG -n 50 -q long -Y 2018 -o $SKIMDIR/$OUTDIR/SKIM_Tau_Run2018B -k True -i $DATADIR/${InputList["Tau_Run2018B"]}
#$SUBMITSCRIPT -T $OUTDIR -d True -s True -c $THECFG -n 50 -q long -Y 2018 -o $SKIMDIR/$OUTDIR/SKIM_Tau_Run2018C -k True -i $DATADIR/${InputList["Tau_Run2018C"]}
#$SUBMITSCRIPT -T $OUTDIR -d True -s True -c $THECFG -n 50 -q long -Y 2018 -o $SKIMDIR/$OUTDIR/SKIM_Tau_Run2018D -k True -i $DATADIR/${InputList["Tau_Run2018D"]}
#
#$SUBMITSCRIPT -T $OUTDIR -d True -s True -c $THECFG -n 50 -q long -Y 2018 -o $SKIMDIR/$OUTDIR/SKIM_SingleMuon_Run2018A -k True -i $DATADIR/${InputList["SingleMuon_Run2018A"]}
#$SUBMITSCRIPT -T $OUTDIR -d True -s True -c $THECFG -n 50 -q long -Y 2018 -o $SKIMDIR/$OUTDIR/SKIM_SingleMuon_Run2018B -k True -i $DATADIR/${InputList["SingleMuon_Run2018B"]}
#$SUBMITSCRIPT -T $OUTDIR -d True -s True -c $THECFG -n 50 -q long -Y 2018 -o $SKIMDIR/$OUTDIR/SKIM_SingleMuon_Run2018C -k True -i $DATADIR/${InputList["SingleMuon_Run2018C"]}
#$SUBMITSCRIPT -T $OUTDIR -d True -s True -c $THECFG -n 50 -q long -Y 2018 -o $SKIMDIR/$OUTDIR/SKIM_SingleMuon_Run2018D -k True -i $DATADIR/${InputList["SingleMuon_Run2018D"]}
#
#$SUBMITSCRIPT -T $OUTDIR -d True -s True -c $THECFG -n 50 -q long -Y 2018 -o $SKIMDIR/$OUTDIR/SKIM_MET_Run2018A -k True -i $DATADIR/${InputList["MET_Run2018A"]}
#$SUBMITSCRIPT -T $OUTDIR -d True -s True -c $THECFG -n 50 -q long -Y 2018 -o $SKIMDIR/$OUTDIR/SKIM_MET_Run2018B -k True -i $DATADIR/${InputList["MET_Run2018B"]}
#$SUBMITSCRIPT -T $OUTDIR -d True -s True -c $THECFG -n 50 -q long -Y 2018 -o $SKIMDIR/$OUTDIR/SKIM_MET_Run2018C -k True -i $DATADIR/${InputList["MET_Run2018C"]}
#$SUBMITSCRIPT -T $OUTDIR -d True -s True -c $THECFG -n 50 -q long -Y 2018 -o $SKIMDIR/$OUTDIR/SKIM_MET_Run2018D -k True -i $DATADIR/${InputList["MET_Run2018D"]}

 
 
 ######################
 #### HH resonant signal - all masspoints
 # spin-0 ggF
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m250  -i $SIGDIR/${InputList["ggF_Radion_m250"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m260  -i $SIGDIR/${InputList["ggF_Radion_m260"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m270  -i $SIGDIR/${InputList["ggF_Radion_m270"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m280  -i $SIGDIR/${InputList["ggF_Radion_m280"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m300  -i $SIGDIR/${InputList["ggF_Radion_m300"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m320  -i $SIGDIR/${InputList["ggF_Radion_m320"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m350  -i $SIGDIR/${InputList["ggF_Radion_m350"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m400  -i $SIGDIR/${InputList["ggF_Radion_m400"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m450  -i $SIGDIR/${InputList["ggF_Radion_m450"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m500  -i $SIGDIR/${InputList["ggF_Radion_m500"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m550  -i $SIGDIR/${InputList["ggF_Radion_m550"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m600  -i $SIGDIR/${InputList["ggF_Radion_m600"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m650  -i $SIGDIR/${InputList["ggF_Radion_m650"]}  -x 1.
$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m700  -i $SIGDIR/${InputList["ggF_Radion_m700"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m750  -i $SIGDIR/${InputList["ggF_Radion_m750"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m800  -i $SIGDIR/${InputList["ggF_Radion_m800"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m850  -i $SIGDIR/${InputList["ggF_Radion_m850"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m900  -i $SIGDIR/${InputList["ggF_Radion_m900"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m1000 -i $SIGDIR/${InputList["ggF_Radion_m1000"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m1250 -i $SIGDIR/${InputList["ggF_Radion_m1250"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m1500 -i $SIGDIR/${InputList["ggF_Radion_m1500"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m1750 -i $SIGDIR/${InputList["ggF_Radion_m1750"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m2000 -i $SIGDIR/${InputList["ggF_Radion_m2000"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m2500 -i $SIGDIR/${InputList["ggF_Radion_m2500"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_Radion_m3000 -i $SIGDIR/${InputList["ggF_Radion_m3000"]} -x 1.
## spin-2 ggF
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m250  -i $SIGDIR/${InputList["ggF_BulkGraviton_m250"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m260  -i $SIGDIR/${InputList["ggF_BulkGraviton_m260"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m270  -i $SIGDIR/${InputList["ggF_BulkGraviton_m270"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m280  -i $SIGDIR/${InputList["ggF_BulkGraviton_m280"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m300  -i $SIGDIR/${InputList["ggF_BulkGraviton_m300"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m320  -i $SIGDIR/${InputList["ggF_BulkGraviton_m320"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m350  -i $SIGDIR/${InputList["ggF_BulkGraviton_m350"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m400  -i $SIGDIR/${InputList["ggF_BulkGraviton_m400"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m450  -i $SIGDIR/${InputList["ggF_BulkGraviton_m450"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m500  -i $SIGDIR/${InputList["ggF_BulkGraviton_m500"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m550  -i $SIGDIR/${InputList["ggF_BulkGraviton_m550"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m600  -i $SIGDIR/${InputList["ggF_BulkGraviton_m600"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m650  -i $SIGDIR/${InputList["ggF_BulkGraviton_m650"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m700  -i $SIGDIR/${InputList["ggF_BulkGraviton_m700"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m750  -i $SIGDIR/${InputList["ggF_BulkGraviton_m750"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m800  -i $SIGDIR/${InputList["ggF_BulkGraviton_m800"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m850  -i $SIGDIR/${InputList["ggF_BulkGraviton_m850"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m900  -i $SIGDIR/${InputList["ggF_BulkGraviton_m900"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m1000 -i $SIGDIR/${InputList["ggF_BulkGraviton_m1000"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m1250 -i $SIGDIR/${InputList["ggF_BulkGraviton_m1250"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m1500 -i $SIGDIR/${InputList["ggF_BulkGraviton_m1500"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m1750 -i $SIGDIR/${InputList["ggF_BulkGraviton_m1750"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m2000 -i $SIGDIR/${InputList["ggF_BulkGraviton_m2000"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m2500 -i $SIGDIR/${InputList["ggF_BulkGraviton_m2500"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggF_BulkGraviton_m3000 -i $SIGDIR/${InputList["ggF_BulkGraviton_m3000"]} -x 1.
## spin-0 VBF
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m250  -i $SIGDIR/${InputList["VBF_Radion_m250"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m260  -i $SIGDIR/${InputList["VBF_Radion_m260"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m270  -i $SIGDIR/${InputList["VBF_Radion_m270"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m280  -i $SIGDIR/${InputList["VBF_Radion_m280"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m300  -i $SIGDIR/${InputList["VBF_Radion_m300"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m320  -i $SIGDIR/${InputList["VBF_Radion_m320"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m350  -i $SIGDIR/${InputList["VBF_Radion_m350"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m400  -i $SIGDIR/${InputList["VBF_Radion_m400"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m450  -i $SIGDIR/${InputList["VBF_Radion_m450"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m500  -i $SIGDIR/${InputList["VBF_Radion_m500"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m550  -i $SIGDIR/${InputList["VBF_Radion_m550"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m600  -i $SIGDIR/${InputList["VBF_Radion_m600"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m650  -i $SIGDIR/${InputList["VBF_Radion_m650"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m700  -i $SIGDIR/${InputList["VBF_Radion_m700"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m750  -i $SIGDIR/${InputList["VBF_Radion_m750"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m800  -i $SIGDIR/${InputList["VBF_Radion_m800"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m850  -i $SIGDIR/${InputList["VBF_Radion_m850"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m900  -i $SIGDIR/${InputList["VBF_Radion_m900"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m1000 -i $SIGDIR/${InputList["VBF_Radion_m1000"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m1250 -i $SIGDIR/${InputList["VBF_Radion_m1250"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m1500 -i $SIGDIR/${InputList["VBF_Radion_m1500"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m1750 -i $SIGDIR/${InputList["VBF_Radion_m1750"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m2000 -i $SIGDIR/${InputList["VBF_Radion_m2000"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m2500 -i $SIGDIR/${InputList["VBF_Radion_m2500"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_Radion_m3000 -i $SIGDIR/${InputList["VBF_Radion_m3000"]} -x 1.
## spin-2 VBF
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m250  -i $SIGDIR/${InputList["VBF_BulkGraviton_m250"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m260  -i $SIGDIR/${InputList["VBF_BulkGraviton_m260"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m270  -i $SIGDIR/${InputList["VBF_BulkGraviton_m270"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m280  -i $SIGDIR/${InputList["VBF_BulkGraviton_m280"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m300  -i $SIGDIR/${InputList["VBF_BulkGraviton_m300"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m320  -i $SIGDIR/${InputList["VBF_BulkGraviton_m320"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m350  -i $SIGDIR/${InputList["VBF_BulkGraviton_m350"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m400  -i $SIGDIR/${InputList["VBF_BulkGraviton_m400"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m450  -i $SIGDIR/${InputList["VBF_BulkGraviton_m450"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m500  -i $SIGDIR/${InputList["VBF_BulkGraviton_m500"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m550  -i $SIGDIR/${InputList["VBF_BulkGraviton_m550"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m600  -i $SIGDIR/${InputList["VBF_BulkGraviton_m600"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m650  -i $SIGDIR/${InputList["VBF_BulkGraviton_m650"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m700  -i $SIGDIR/${InputList["VBF_BulkGraviton_m700"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m750  -i $SIGDIR/${InputList["VBF_BulkGraviton_m750"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m800  -i $SIGDIR/${InputList["VBF_BulkGraviton_m800"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m850  -i $SIGDIR/${InputList["VBF_BulkGraviton_m850"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m900  -i $SIGDIR/${InputList["VBF_BulkGraviton_m900"]}  -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m1000 -i $SIGDIR/${InputList["VBF_BulkGraviton_m1000"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m1250 -i $SIGDIR/${InputList["VBF_BulkGraviton_m1250"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m1500 -i $SIGDIR/${InputList["VBF_BulkGraviton_m1500"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m1750 -i $SIGDIR/${InputList["VBF_BulkGraviton_m1750"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m2000 -i $SIGDIR/${InputList["VBF_BulkGraviton_m2000"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m2500 -i $SIGDIR/${InputList["VBF_BulkGraviton_m2500"]} -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 10 -q long -k True -Y 2018 --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBF_BulkGraviton_m3000 -i $SIGDIR/${InputList["VBF_BulkGraviton_m3000"]} -x 1.


#######################
##### TT - XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
### TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
### hh = 45.7%, ll = 10.5%, hl = 21.9% (x2 for permutation t-tbar)
#sleep 1h#
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 300 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_TT_fullyHad -t True -b 1 -i $BKGDIR/${InputList["TT_fullyHad"]} -x 377.96
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 300 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_TT_fullyLep -t True -b 4 -i $BKGDIR/${InputList["TT_fullyLep"]} -x 88.29
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 300 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_TT_semiLep  -t True -b 5 -i $BKGDIR/${InputList["TT_semiLep"]}  -x 365.34

######################
#### DY - xsec from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#DY_Z
#sleep 1h
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 700 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DYmerged -i $BKGDIR/${InputList["DY_merged"]}       -g True --DY True -x 6077.22 
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 500 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DYincl -i $BKGDIR/${InputList["DY_incl"]}          -x 6077.22 
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 300 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DYmerged -i $BKGDIR/${InputList["DY_1j"]}           -g True --DY True -x 1. 
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 300 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DYmerged -i $BKGDIR/${InputList["DY_2j"]}           -g True --DY True -x 1. 
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 300 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DYmerged -i $BKGDIR/${InputList["DY_4j"]}           -g True --DY True -x 1. 
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 300 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DYmerged -i $BKGDIR/${InputList["DY_HT70To100"]}    -g True --DY True -x 1. 
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 300 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DYmerged -i $BKGDIR/${InputList["DY_HT100To200"]}   -g True --DY True -x 1. 
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 300 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DYmerged -i $BKGDIR/${InputList["DY_HT200To400"]}   -g True --DY True -x 1. 
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 300 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DYmerged -i $BKGDIR/${InputList["DY_HT400To600"]}   -g True --DY True -x 1. 
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 300 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DYmerged -i $BKGDIR/${InputList["DY_HT600To800"]}   -g True --DY True -x 1. 
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 300 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DYmerged -i $BKGDIR/${InputList["DY_HT800To1200"]}  -g True --DY True -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 300 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DYmerged -i $BKGDIR/${InputList["DY_HT1200To2500"]} -g True --DY True -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 300 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DYmerged -i $BKGDIR/${InputList["DY_HT2500ToInf"]}  -g True --DY True -x 1.

#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 1000 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DY_NLO -i $BKGDIR/${InputList["DY_NLO"]}          -x 6077.22 
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 333 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DY_NLO_0j -i $BKGDIR/${InputList["DY_NLO_0j"]}          -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 333 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DY_NLO_1j -i $BKGDIR/${InputList["DY_NLO_1j"]}          -x 1.
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 333 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DY_NLO_2j -i $BKGDIR/${InputList["DY_NLO_2j"]}          -x 1.

# $SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 200 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DY_NLO_PT50To100  -i $BKGDIR/${InputList["DY_NLO_PT50To100"]}  -x 1.
# $SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 200 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DY_NLO_PT100To250 -i $BKGDIR/${InputList["DY_NLO_PT100To250"]} -x 1.
# $SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 200 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DY_NLO_PT250To400 -i $BKGDIR/${InputList["DY_NLO_PT250To400"]} -x 1.
# $SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 200 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DY_NLO_PT400To650 -i $BKGDIR/${InputList["DY_NLO_PT400To650"]} -x 1.
# $SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 200 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_DY_NLO_PT650ToInf -i $BKGDIR/${InputList["DY_NLO_PT650ToInf"]} -x 1.

#######################
#### Wjets - filelists up to date
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 20 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_WJets_HT_0_70      -i $BKGDIR/${InputList["WJets_HT_0_70"]}      -y 1.213784 -x 48917.48 -z 70
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 20 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_WJets_HT_70_100    -i $BKGDIR/${InputList["WJets_HT_70_100"]}    -y 1.213784 -x 1362
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 20 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_WJets_HT_100_200   -i $BKGDIR/${InputList["WJets_HT_100_200"]}   -y 1.213784 -x 1345	 
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 20 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_WJets_HT_200_400   -i $BKGDIR/${InputList["WJets_HT_200_400"]}   -y 1.213784 -x 359.7	 
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 20 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_WJets_HT_400_600   -i $BKGDIR/${InputList["WJets_HT_400_600"]}   -y 1.213784 -x 48.91	 
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 20 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_WJets_HT_600_800   -i $BKGDIR/${InputList["WJets_HT_600_800"]}   -y 1.213784 -x 12.05	 
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 20 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_WJets_HT_800_1200  -i $BKGDIR/${InputList["WJets_HT_800_1200"]}  -y 1.213784 -x 5.501	 
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 20 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_WJets_HT_1200_2500 -i $BKGDIR/${InputList["WJets_HT_1200_2500"]} -y 1.213784 -x 1.329	 
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -n 20 -q long -Y 2018 -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_WJets_HT_2500_Inf  -i $BKGDIR/${InputList["WJets_HT_2500_Inf"]}  -y 1.213784 -x 0.03216

######################
##### ELECTROWEAK - XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 50 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_EWKWPlus2Jets_WToLNu -i $BKGDIR/${InputList["EWKWPlus2Jets_WToLNu"]} -x 25.62
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 50 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_EWKWMinus2Jets_WToLNu -i $BKGDIR/${InputList["EWKWMinus2Jets_WToLNu"]} -x 20.25
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 50 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_EWKZ2Jets_ZToLL -i $BKGDIR/${InputList["EWKZ2Jets_ZToLL"]} -x 3.987

######################
##### single top - XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 50 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ST_tW_antitop       -i $BKGDIR/${InputList["ST_tW_antitop"]}       -x 35.85
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 50 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ST_tW_top           -i $BKGDIR/${InputList["ST_tW_top"]}           -x 35.85
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 50 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ST_tchannel_antitop -i $BKGDIR/${InputList["ST_tchannel_antitop"]} -x 80.95
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 50 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ST_tchannel_top     -i $BKGDIR/${InputList["ST_tchannel_top"]}     -x 136.02

####################
### SM Higgs - from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNHLHE2019
# HXSWG: xs(ZH) = 0.880 pb, xs(W+H) = 0.831 pb, xs(W-H) = 0.527 pb, xs(ggH) = 48.61 pb, xs(VBFH) = 3.766 pb, xs(ttH) = 0.5071 pb
# Z->qq : 69.91% , Z->ll : 3,3658% (x3 for all the leptons), H->bb : 57.7%  , H->tautau : 6.32%
# ZH (Zll, Hbb) : XSBD (xs ZH * BR Z) * H->bb, ZH (Zqq, Hbb) : XSBD (xs ZH * BR Z) * H->bb
# ZH (Zall, Htautau) : XS teor ZH * BR H->tautau
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 30 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ggHTauTau     -i $BKGDIR/${InputList["ggHTauTau"]}     -x 48.61  -y 0.0632
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 30 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_VBFHTauTau    -i $BKGDIR/${InputList["VBFHTauTau"]}    -x 3.766  -y 0.0632
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 30 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ZH_HTauTau    -i $BKGDIR/${InputList["ZH_HTauTau"]}    -x 0.880  -y 0.0632
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 30 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_WplusHTauTau  -i $BKGDIR/${InputList["WplusHTauTau"]}  -x 0.831  -y 0.0632
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 30 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_WminusHTauTau -i $BKGDIR/${InputList["WminusHTauTau"]} -x 0.527  -y 0.0632
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 30 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ttHToNonBB    -i $BKGDIR/${InputList["ttHToNonBB"]}    -x 0.5071 -y 0.3598
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 30 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ttHToBB       -i $BKGDIR/${InputList["ttHToBB"]}       -x 0.5071 -y 0.577
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 30 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ttHToTauTau   -i $BKGDIR/${InputList["ttHToTauTau"]}   -x 0.5071 -y 0.0632

#######################
##### Multiboson: -  https://arxiv.org/abs/1408.5243 (WW), https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson (WZ,ZZ
#### Some XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
#### Some other XS taken from http://cms.cern.ch/iCMS/jsp/db_notes/noteInfo.jsp?cmsnoteid=CMS%20AN-2019/111
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 20 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_WW -i $BKGDIR/${InputList["WW"]} -x 118.7
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 20 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_WZ -i $BKGDIR/${InputList["WZ"]} -x 47.13
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 20 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_ZZ -i $BKGDIR/${InputList["ZZ"]} -x 16.523
#
#######################
##### Others : - filelists up to date
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 20 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_TTWJetsToLNu -i $BKGDIR/${InputList["TTWJetsToLNu"]} -x 0.2043
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 20 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_TTWJetsToQQ  -i $BKGDIR/${InputList["TTWJetsToQQ"]}  -x 0.4062
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 20 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_TTZToLLNuNu  -i $BKGDIR/${InputList["TTZToLLNuNu"]} -x 0.2529
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 20 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_TTWW         -i $BKGDIR/${InputList["TTWW"]}         -x 0.006979
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 20 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_TTZZ         -i $BKGDIR/${InputList["TTZZ"]}         -x 0.001386
#$SUBMITSCRIPT -T $OUTDIR -s True -c $THECFG -Y 2018 -n 20 -q long -k True --pu $PUSF -o $SKIMDIR/$OUTDIR/SKIM_TTWZ         -i $BKGDIR/${InputList["TTWZ"]}         -x 0.00158


#######################################################################################################################################################################################
##LEGACY

######################
# Signals ggF non res - filelists up to date
# norm xs = 1 p
#sleep 1h
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 20 -q long  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_SM      -i $INPUTDIR_SIG/12_GluGluToHHTo2B2Tau_node_SM_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 20 -q long  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_node_2  -i $INPUTDIR_SIG/4_GluGluToHHTo2B2Tau_node_2_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 20 -q long  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_node_3  -i $INPUTDIR_SIG/5_GluGluToHHTo2B2Tau_node_3_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 20 -q long  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_node_4  -i $INPUTDIR_SIG/2_GluGluToHHTo2B2Tau_node_4_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 20 -q long  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_node_5  -i $INPUTDIR_SIG/3_GluGluToHHTo2B2Tau_node_5_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 20 -q long  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_node_6  -i $INPUTDIR_SIG/4_GluGluToHHTo2B2Tau_node_6_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 20 -q long  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_node_7  -i $INPUTDIR_SIG/9_GluGluToHHTo2B2Tau_node_7_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 20 -q long  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_node_8  -i $INPUTDIR_SIG/10_GluGluToHHTo2B2Tau_node_8_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt  -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 20 -q long  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_node_9  -i $INPUTDIR_SIG/1_GluGluToHHTo2B2Tau_node_9_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 20 -q long  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_node_10 -i $INPUTDIR_SIG/1_GluGluToHHTo2B2Tau_node_10_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt  -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 20 -q long  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_node_11 -i $INPUTDIR_SIG/1_GluGluToHHTo2B2Tau_node_11_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt  -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 20 -q long  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_node_12 -i $INPUTDIR_SIG/3_GluGluToHHTo2B2Tau_node_12_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt  -x 1. -a True


######################
#### Signals ggF non res  - filelists up to date
### norm xs = 1 p
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg -Y 2018 -n 10 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_NLO_cHHH1    -i $INPUTDIR_SIG/2_GluGluToHHTo2B2Tau_node_cHHH1_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt    -x 0.045 -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg -Y 2018 -n 10 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_NLO_cHHH0    -i $INPUTDIR_SIG/1_GluGluToHHTo2B2Tau_node_cHHH0_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt    -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg -Y 2018 -n 10 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_NLO_cHHH2p45 -i $INPUTDIR_SIG/3_GluGluToHHTo2B2Tau_node_cHHH2p45_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg -Y 2018 -n 10 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_NLO_cHHH5    -i $INPUTDIR_SIG/4_GluGluToHHTo2B2Tau_node_cHHH5_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt    -x 1. -a True

### norm to theoretical xs
# sigma_NNLO+FTapprox for SM: 31.05 fb
# sigma_NNLO+FTapprox / sigma_NLO  = 1.115 for SM # TEMPORARY: need to fix with factor kL dependent (pag. 129: https://arxiv.org/pdf/2003.01700.pdf)
# f(kL) = A + B*kL + C*kL**2
# A = 62.5339
# B = -44.323
# C = 9.6340
# (slide 10: https://indico.cern.ch/event/885273/contributions/3812533/attachments/2016615/3370728/HH_combine_model_7Apr2018.pdf)
# xs (kL = 1)                      = 0.03105 pb
# xs (kL = 0)    = f(0)    * 1.115 = 0.06972 pb
# xs (kL = 2.45) = f(2.45) * 1.115 = 0.01312 pb
# xs (kL = 5)    = f(5)    * 1.115 = 0.09117 pb
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 20 -q short  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_NLO_SM_xs       -i $SIGDIR/2_GluGluToHHTo2B2Tau_node_cHHH1_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt    -x 0.03105 -a True --hhNLO
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 10 -q long  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_NLO_cHHH0_xs    -i $INPUTDIR_SIG/1_GluGluToHHTo2B2Tau_node_cHHH0_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt    -x 0.06972 -a True --hhNLO
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 10 -q long  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_NLO_cHHH2p45_xs -i $INPUTDIR_SIG/3_GluGluToHHTo2B2Tau_node_cHHH2p45_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 0.01312 -a True --hhNLO
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 10 -q long  -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_GGHH_NLO_cHHH5_xs    -i $INPUTDIR_SIG/4_GluGluToHHTo2B2Tau_node_cHHH5_TuneCP5_PSWeights_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt    -x 0.09117 -a True --hhNLO

######################
#### Signals VBF non res - filelists up to date
### norm xs = 1 pb
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 10 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_VBFHH_CV_1_C2V_1_C3_1 -i $INPUTDIR_SIG/1_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 10 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_VBFHH_CV_0_5_C2V_1_C3_1 -i $INPUTDIR_SIG/2_VBFHHTo2B2Tau_CV_0_5_C2V_1_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 10 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_VBFHH_CV_1_5_C2V_1_C3_1 -i $INPUTDIR_SIG/3_VBFHHTo2B2Tau_CV_1_5_C2V_1_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 10 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_VBFHH_CV_1_C2V_1_C3_0 -i $INPUTDIR_SIG/4_VBFHHTo2B2Tau_CV_1_C2V_1_C3_0_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 10 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_VBFHH_CV_1_C2V_1_C3_2 -i $INPUTDIR_SIG/5_VBFHHTo2B2Tau_CV_1_C2V_1_C3_2_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg  -n 10 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_VBFHH_CV_1_C2V_2_C3_1 -i $INPUTDIR_SIG/6_VBFHHTo2B2Tau_CV_1_C2V_2_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 1. -a True

### norm to theoretical xs
# xs_theo(SM) = 0.001726
#  CV C2V C3 |  xs_MG5      * xs_theo(SM)/xs_MG5(SM)
#   1   1  1 |  0.001668 pb * 1.034772182             =   0.001726 pb
# 0.5   1  1 |  0.01046  pb * 1.034772182             =   0.010824 pb
# 1.5   1  1 |  0.0638   pb * 1.034772182             =   0.066018 pb
#   1   1  0 |  0.004454 pb * 1.034772182             =   0.004609 pb
#   1   1  2 |  0.001375 pb * 1.034772182             =   0.001423 pb
#   1   2  1 |  0.01374  pb * 1.034772182             =   0.014218 pb

#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg -Y 2018 -n 20 -q short -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_VBFHH_CV_1_C2V_1_C3_1_xs   -i $SIGDIR/1_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 0.001726  -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg -Y 2018 -n 10 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_VBFHH_CV_0_5_C2V_1_C3_1_xs -i $INPUTDIR_SIG/2_VBFHHTo2B2Tau_CV_0_5_C2V_1_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 0.010824  -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg -Y 2018 -n 10 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_VBFHH_CV_1_5_C2V_1_C3_1_xs -i $INPUTDIR_SIG/3_VBFHHTo2B2Tau_CV_1_5_C2V_1_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 0.066018  -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg -Y 2018 -n 10 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_VBFHH_CV_1_C2V_1_C3_0_xs   -i $INPUTDIR_SIG/4_VBFHHTo2B2Tau_CV_1_C2V_1_C3_0_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 0.004609  -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg -Y 2018 -n 10 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_VBFHH_CV_1_C2V_1_C3_2_xs   -i $INPUTDIR_SIG/5_VBFHHTo2B2Tau_CV_1_C2V_1_C3_2_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 0.001423  -a True
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c config/skim_Legacy2018.lp.cfg -Y 2018 -n 10 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_VBFHH_CV_1_C2V_2_C3_1_xs   -i $INPUTDIR_SIG/6_VBFHHTo2B2Tau_CV_1_C2V_2_C3_1_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt   -x 0.014218  -a True


######################
#### Reweighting ggF non res - filelists up to date
### norm xs = 1 pb
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c  config/skim_Legacy2018.lp.cfg  -n 20 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_HHRew_SM  -x 1.0 --kl 1.0  --kt 1.0 -x 1. -a True -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_LO_allNodes.txt

#this is just for cross check with LO node SM, which wrongly has c2g set to 1
#python scripts/skimNtuple.lp.py -T $OUTDIR -s True -c  config/skim_Legacy2018.lp.cfg  -n 20 -q long -k True --pu $PUDIR/PU_Legacy2018_SF.txt -o $SKIMDIR/$OUTDIR/SKIM_HHRew_SM_wrong  -x 1.0 --kl 1.0  --kt 1.0 --c2 0.0 --cg 0.0 --c2g 1. -a True -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_LO_allNodes.txt
