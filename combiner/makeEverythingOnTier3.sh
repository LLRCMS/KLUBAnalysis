#!/bin/bash
# make cards with all vars/selections

#export OUTSTRING="02feb_vismass_L8L8L"
export OUTSTRING="clusterXanda"
#export OUTSTRING="28feb_Resonant_BR"

export BASEETAU="defaultBtagLLNoIsoBBTTCutKine80"
export BASEMUTAU="defaultBtagLLNoIsoBBTTCutKine80" #LL
export BASETAUTAU="defaultBtagLLNoIsoBBTTCut45"
# export BASEETAU="defaultBtagMMNoIsoBBTTCut"
# export BASEMUTAU="defaultBtagMMNoIsoBBTTCut" 
# export BASETAUTAU="defaultBtagLLNoIsoBBTTCut45"

export VARTAUTAU="HH_mass_raw"
export VARMUTAU="HH_mass_raw"
export VARETAU="HH_mass_raw"
# export VARTAUTAU="HHKin_mass_raw"
# export VARMUTAU="HHKin_mass_raw"
# export VARETAU="HHKin_mass_raw"

#export LAMBDAS="Lambdam4 Lambda1 Lambda2dot46 Lambda20"
#export LAMBDAS="HHNode10 HHNode11 HHNode12 HHNode13 HHNode2  HHNode3  HHNode4  HHNode5  HHNode6  HHNode7  HHNode8  HHNode9  HHNodebox HHNodeSM"
export LAMBDAS="HHrewclus1 HHrewclus2 HHrewclus3 HHrewclus4 HHrewclus5 HHrewclus6 HHrewclus7 HHrewclus8 HHrewclus9 HHrewclus10 HHrewclus11 HHrewclus0 HHrewoutlier1clus1 HHrewoutlier2clus1 HHrewoutlier3clus1 HHrewoutlier4clus1 HHrewoutlier5clus1 HHrewoutlier0clus1 HHrewoutlier1clus10 HHrewoutlier2clus10 HHrewoutlier3clus10 HHrewoutlier4clus10 HHrewoutlier5clus10 HHrewoutlier0clus10"
#export LAMBDAS=""
#for il in {0..51}
#do 
#export LAMBDAS="$LAMBDAS HHrew${il}"
#done
#export LAMBDAS="Radion250 Radion260 Radion270 Radion280 Radion300 Radion320 Radion340 Radion350 Radion400 Radion450 Radion500 Radion550 Radion600 Radion650 Radion700 Radion800 Radion900"
#export LAMBDAS="Radion250 Radion260 Radion270 Radion280 Radion300 Radion325 Radion350 Radion375 Radion400 Radion425 Radion450 Radion475 Radion500 Radion525 Radion550 Radion575 Radion600 Radion625 Radion650 Radion675 Radion700 Radion725 Radion750 Radion775 Radion800 Radion825 Radion850 Radion875 Radion900"
export QUANTILES="0.025 0.16 0.5 0.84 0.975 -1.0"

export SOURCE="/home/llr/cms/cadamuro/CleanKLUB/CMSSW_7_4_7/src/KLUBAnalysis" 
# python cardMaker.py -i /home/llr/cms/cadamuro/CleanKLUB/CMSSW_7_4_7/src/KLUBAnalysis/analysis_TauTau_resonant/evalQCD_25feb_pt40/resonantAnalysis_TauTau.cfg -f /home/llr/cms/cadamuro/CleanKLUB/CMSSW_7_4_7/src/KLUBAnalysis/analysis_TauTau_resonant/evalQCD_25feb_pt40/outPlotter.root -o defaultBtagMMNoIsoBBTTCut -v HHKin_mass_raw -c MuTau --dir _test -t 0 -r 1

#python cardMaker.py -i ${SOURCE}/analysis_ETau/evalQCD_Plots_7Mar_pt40_allHHRew_fix/analysis_ETau.cfg     -f ${SOURCE}/analysis_ETau/evalQCD_Plots_7Mar_pt40_allHHRew_fix/outPlotter.root   -o $BASEETAU   -v $VARETAU   -c ETau   --dir "_$OUTSTRING" -t 1 # -r 1
#python cardMaker.py -i ${SOURCE}/analysis_MuTau/evalQCD_Plots_7Mar_pt40_allHHRew_fix/analysis_MuTau.cfg   -f ${SOURCE}/analysis_MuTau/evalQCD_Plots_7Mar_pt40_allHHRew_fix/outPlotter.root  -o $BASEMUTAU  -v $VARMUTAU  -c MuTau  --dir "_$OUTSTRING" -t 1 # -r 1
python cardMaker.py -i analysis_Xanda.cfg -f ${SOURCE}/analysis_TauTau/evalQCD_Plots_8Apr_XandaRew/outPlotter.root -o $BASETAUTAU -v $VARTAUTAU -c TauTau --dir "_$OUTSTRING" -t 0 # -r 1
# python cardMaker.py -i ${SOURCE}/analysis_ETau_resonant/evalQCD_28feb_pt40/resonantAnalysis_ETau.cfg     -f ${SOURCE}/analysis_ETau_resonant/evalQCD_28feb_pt40/outPlotter.root   -o $BASEETAU   -v $VARETAU   -c ETau   --dir "_$OUTSTRING" -t 0 -r 1 #-S /home/llr/cms/cadamuro/FittedHHKinFit/outPlotter_fit_sigs_ETau.root
# python cardMaker.py -i ${SOURCE}/analysis_MuTau_resonant/evalQCD_28feb_pt40/resonantAnalysis_MuTau.cfg   -f ${SOURCE}/analysis_MuTau_resonant/evalQCD_28feb_pt40/outPlotter.root  -o $BASEMUTAU  -v $VARMUTAU  -c MuTau  --dir "_$OUTSTRING" -t 0 -r 1 #-S /home/llr/cms/cadamuro/FittedHHKinFit/outPlotter_fit_sigs_MuTau.root
# python cardMaker.py -i ${SOURCE}/analysis_TauTau_resonant/evalQCD_28feb_pt40_LoosenQCD_7GeV/resonantAnalysis_TauTau.cfg -f ${SOURCE}/analysis_TauTau_resonant/evalQCD_28feb_pt40_LoosenQCD_7GeV/outPlotter.root -o $BASETAUTAU -v $VARTAUTAU -c TauTau --dir "_$OUTSTRING" -t 0 -r 1 #-S /home/llr/cms/cadamuro/FittedHHKinFit/outPlotter_fit_sigs_TauTau.root

# # python cardMaker.py -i ../config/analysis_MuTau.cfg -f /home/llr/cms/cadamuro/HHKLUBGiacomo/CMSSW_7_4_7/src/KLUBAnalysis/analysis_MuTau/evalQCD_Plots_14Gen_Limits4Giacomo/outPlotter.root -o $BASEMUTAU -v $VARMUTAU -c MuTau --dir "_$OUTSTRING"
# # python cardMaker.py -i ../config/analysis_ETau.cfg -f /home/llr/cms/cadamuro/HHKLUBGiacomo/CMSSW_7_4_7/src/KLUBAnalysis/analysis_ETau/evalQCD_Plots_14Gen_Limits4Giacomo/outPlotter.root -o $BASEETAU -v $VARETAU -c ETau --dir "_$OUTSTRING"
# # python cardMaker.py -i ../config/analysis_TauTau.cfg -f /home/llr/cms/cadamuro/HHKLUBGiacomo/CMSSW_7_4_7/src/KLUBAnalysis/analysis_TauTau/evalQCD_Plots_14Gen_Limits4Giacomo/outPlotter.root -o $BASETAUTAU -v $VARTAUTAU -c TauTau --dir "_$OUTSTRING"


for i in $LAMBDAS 
	do	
	## for c in MuTau ETau TauTau 
	## do
	## 	if [ -a "hh_3_L${i}_13TeV.txt" ]
	## 		then
	## 		combineCards.py -S "hh_3_L${i}_13TeV.txt" >> comb.txt 
	## 	fi
	#cd cards_MuTau_$OUTSTRING/${i}${BASEMUTAU}${VARMUTAU}
	#pwd
	#combineCards.py -S "hh_2_L${i}_13TeV.txt" >> comb.txt 
	#text2workspace.py comb.txt -o comb.root ;
	##combine -M Asymptotic comb.root -m 125.0 -n ${i}_forLim
	#ln -ns ../../prepareHybrid.py .
	#ln -ns ../../prepareGOF.py .
	#ln -ns ../../prepareAsymptotic.py .
	#python prepareAsymptotic.py -n $i
	#for q in $QUANTILES
	#do
	#	python prepareHybrid.py -n $q
	#done
	#cd -
	#cd cards_ETau_$OUTSTRING/${i}${BASEETAU}${VARETAU}
	#pwd
	#combineCards.py -S "hh_1_L${i}_13TeV.txt" >> comb.txt 
	#text2workspace.py comb.txt -o comb.root ;
	#ln -ns ../../prepareHybrid.py .
	#ln -ns ../../prepareGOF.py .
	#ln -ns ../../prepareAsymptotic.py .
	#python prepareAsymptotic.py -n $i
	#for q in $QUANTILES
	#do
	#	python prepareHybrid.py -n $q
	#done
	## if [ $i == "Radion450" ]
	## 	then
	## 	for g in {0..100}
	## 		do
	## 			python prepareGOF.py -n $g
	## 		done
	## fi
	#cd -
	cd cards_TauTau_$OUTSTRING/${i}${BASETAUTAU}${VARTAUTAU}
	pwd
	combineCards.py -S "hh_3_L${i}_13TeV.txt" >> comb.txt 
	text2workspace.py comb.txt -o comb.root ;
	#ln -ns ../../prepareHybrid.py .
	#ln -ns ../../prepareGOF.py .
	ln -ns ../../prepareAsymptotic.py .
	python prepareAsymptotic.py -n $i
	#for q in $QUANTILES
	#do
	#	python prepareHybrid.py -n $q
	#done
	## if [ $i == "Radion450" ]
	## 	then
	## 	for g in {0..100}
	## 		do
	## 			python prepareGOF.py -n $g
	## 		done
	## fi

	cd -

	done


#
#MIXED CUT COMBINATION
#for i in $LAMBDAS
#do
#        mkdir -p cards_Combined_$OUTSTRING/${i}COMBINED
#        cp cards_MuTau_$OUTSTRING/${i}${BASEMUTAU}${VARMUTAU}/hh_*.* cards_Combined_$OUTSTRING/${i}COMBINED/.
#        cp cards_TauTau_$OUTSTRING/${i}${BASETAUTAU}${VARTAUTAU}/hh_*.* cards_Combined_$OUTSTRING/${i}COMBINED/.
#        cp cards_ETau_$OUTSTRING/${i}${BASEETAU}${VARETAU}/hh_*.* cards_Combined_$OUTSTRING/${i}COMBINED/.
#        cd cards_Combined_$OUTSTRING/${i}COMBINED
#        pwd
#        combineCards.py -S "hh_3_L${i}_13TeV.txt" "hh_2_L${i}_13TeV.txt" "hh_1_L${i}_13TeV.txt" >> comb.txt
#        text2workspace.py comb.txt -o comb.root ;
#        #combine -M Asymptotic comb.root -m 125.0 -t -1 --expectSignal=1 -n ${i} -S 0
#        #combine -M Asymptotic comb.root -m 125.0 -n ${i}_forLim
#        ln -ns ../../prepareHybrid.py .
#        ln -ns ../../prepareGOF.py .
#        ln -ns ../../prepareAsymptotic.py .
#        python prepareAsymptotic.py -n $i
#        #combine -M HybridNew --frequentist -m 125.0 --testStat LHC comb.root  -H ProfileLikelihood -n ${i}_forLim --expectedFromGrid=0.5
#        for q in $QUANTILES
#        do
#		python prepareHybrid.py -n $q
#	    done
#	# if [ $i == "Radion450" ]
#	# 	then
#	# 	for g in {0..100}
#	# 		do
#	# 			python prepareGOF.py -n $g
#	# 		done
#	# fi
#        cd -
#        done





#
#	#./makeWS.sh _mutau_$OUTSTRING 2
##./makeWS.sh _tautau_$OUTSTRING 3
#
#
##combine with syst for limits
##for i in $LAMBDAS 
##do
##	#cd cards_mutau_$OUTSTRING/lambda${i}${BASEMUTAU}${VARMUTAU}
##	#combine -M Asymptotic comb.root -m 125.0 -n Lambda${i}_forLim 
##	#cd -
##
##	cd cards_tautau_$OUTSTRING/lambda${i}${BASETAUTAU}${VARTAUTAU}
##	combine -M Asymptotic comb.root -m 125.0 -n Lambda${i}_forLim 
##	cd -
##done
#
###plotting: leave it for now

