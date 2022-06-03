#!bin/bash

BKGDIR="/home/llr/cms/portales/hhbbtautau/KLUB_UL_20220321/CMSSW_11_1_9/src/KLUBAnalysis/inputFiles/UL18_backgrounds"
CURRDIR=`pwd`

declare -A InputList
InputList["DY_NLO"]="1_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"
InputList["DY_NLO_0j"]="2_DYJetsToLL_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_NLO_1j"]="3_DYJetsToLL_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_NLO_2j"]="4_DYJetsToLL_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_NLO_PT50To100"]="1_DYJetsToLL_Pt-50To100_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_NLO_PT100To250"]="2_DYJetsToLL_Pt-100To250_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_NLO_PT250To400"]="3_DYJetsToLL_Pt-250To400_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_NLO_PT400To650"]="4_DYJetsToLL_Pt-400To650_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v1.txt"
InputList["DY_NLO_PT650ToInf"]="5_DYJetsToLL_Pt-650ToInf_MatchEWPDG20_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIISummer20UL18MiniAODv2-106X_upgrade2018_realistic_v16_L1v1-v2.txt"

echo "DY_NLO"
$CURRDIR/bin/computDYsplitting_PT.exe $BKGDIR/${InputList["DY_NLO"]} "weights/DYstitchMap/DYmap_UL_NLO_incl_2018_20220602.root"


echo "DY_NLO_0j"
$CURRDIR/bin/computDYsplitting_PT.exe $BKGDIR/${InputList["DY_NLO_0j"]} "weights/DYstitchMap/DYmap_UL_NLO_0jet_2018_20220602.root"
echo "DY_NLO_1j"
$CURRDIR/bin/computDYsplitting_PT.exe $BKGDIR/${InputList["DY_NLO_1j"]} "weights/DYstitchMap/DYmap_UL_NLO_1jet_2018_20220602.root"
echo "DY_NLO_2j"
$CURRDIR/bin/computDYsplitting_PT.exe $BKGDIR/${InputList["DY_NLO_2j"]} "weights/DYstitchMap/DYmap_UL_NLO_2jet_2018_20220602.root"


echo "DY_NLO_PT50To100"
$CURRDIR/bin/computDYsplitting_PT.exe $BKGDIR/${InputList["DY_NLO_PT50To100"]} "weights/DYstitchMap/DYmap_UL_NLO_PT50To100_2018_20220602.root"
echo "DY_NLO_PT100To250"
$CURRDIR/bin/computDYsplitting_PT.exe $BKGDIR/${InputList["DY_NLO_PT100To250"]} "weights/DYstitchMap/DYmap_UL_NLO_PT100To250_2018_20220602.root"
echo "DY_NLO_PT250To400"
$CURRDIR/bin/computDYsplitting_PT.exe $BKGDIR/${InputList["DY_NLO_PT250To400"]} "weights/DYstitchMap/DYmap_UL_NLO_PT250To400_2018_20220602.root"
echo "DY_NLO_PT400To650"
$CURRDIR/bin/computDYsplitting_PT.exe $BKGDIR/${InputList["DY_NLO_PT400To650"]} "weights/DYstitchMap/DYmap_UL_NLO_PT400To650_2018_20220602.root"
echo "DY_NLO_PT650ToInf"
$CURRDIR/bin/computDYsplitting_PT.exe $BKGDIR/${InputList["DY_NLO_PT650ToInf"]} "weights/DYstitchMap/DYmap_UL_NLO_PT650ToInf_2018_20220602.root"
