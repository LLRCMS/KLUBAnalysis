#!/bin/bash
# make cards with all vars/selections

export OUTSTRING="CutTable"
export BASEMUTAU="defaultBtagLLNoIsoBBTTCutKine90"
export BASETAUTAU="defaultBtagLLNoIsoBBTTCut"
export BASEETAU="defaultBtagMMNoIsoBBTTCutKine90"
export VARTAUTAU="HHKin_mass"
export VARMUTAU="HHKin_mass"
export VARETAU="HHKin_mass"
#export ALLVARS="HHKin_mass"
#export ALLSELS="dijethardSV defaultOneBtagLNoIso defaultOneBtagMNoIso defaultBtagMMNoIso defaultBtagLLNoIso defaultBtagMMMbbMttNoIso defaultBtagLLMbbMttNoIso defaultBtagMMNoIsoBBTTCut defaultBtagLLNoIsoBBTTCut defaultBtagMMMbbMttNoIsoKine defaultBtagLLMbbMttNoIsoKine defaultBtagMMNoIsoBBTTCutKine defaultBtagLLNoIsoBBTTCutKine"
#export ALLSELS="defaultBtagLLNoIsoBBTTCutKine80 defaultBtagMMNoIsoBBTTCutKine80 defaultBtagLLNoIsoBBTTCut"
#export ALLSELS="defaultBtagLLMbbMttNoIso defaultBtagMMMbbMttNoIso defaultBtagMMKineMbbMttNoIso"
#"defaultBtagML defaultBtagLL dijethardisoBtagCutMMbbMtt dijetOneBL dijetOneBM dijethardisoMbbMtt dijetOneBLKine dijetOneBMKine"
#dijetOneBLKineTrig
#export ALLSELS="defaultBtagMMNoIsoBBTTCut"
export LAMBDAS="Lambdam4 Lambda1 Lambda2dot46 Lambda20"
#export LAMBDAS="Radion250  Radion260  Radion270  Radion280  Radion300  Radion320  Radion340  Radion350  Radion400  Radion450  Radion500"



for i in $LAMBDAS 
	do 
	python cardMaker.py -i ../config/analysis_MuTau.cfg -f /home/llr/cms/cadamuro/HHKLUBGiacomo/CMSSW_7_4_7/src/KLUBAnalysis/analysis_MuTau/evalQCD_Plots_14Gen_Limits4Giacomo/outPlotter.root -o $BASEMUTAU -v $VARMUTAU -c MuTau --dir "_$OUTSTRING" -t 0
	python cardMaker.py -i ../config/analysis_ETau.cfg -f /home/llr/cms/cadamuro/HHKLUBGiacomo/CMSSW_7_4_7/src/KLUBAnalysis/analysis_ETau/evalQCD_Plots_14Gen_Limits4Giacomo/outPlotter.root -o $BASEETAU -v $VARETAU -c ETau --dir "_$OUTSTRING" -t 0
	python cardMaker.py -i ../config/analysis_TauTau.cfg -f /home/llr/cms/cadamuro/HHKLUBGiacomo/CMSSW_7_4_7/src/KLUBAnalysis/analysis_TauTau/evalQCD_Plots_14Gen_Limits4Giacomo/outPlotter.root -o $BASETAUTAU -v $VARTAUTAU -c TauTau --dir "_$OUTSTRING" -t 0
	done


#for c in ETau MuTau TauTau
#	do
#		for i in $LAMBDAS 
#		do	
#		for j in $ALLSELS ; 
#		do 
#
#		for v in $ALLVARS
#            #do python cardMaker.py -i ../config/analysis_${c}.cfg -f /home/llr/cms/cadamuro/HHKLUBGiacomo/CMSSW_7_4_7/src/KLUBAnalysis/analysis_${c}/evalQCD_Plots_14Gen_Limits4Giacomo/outPlotter.root -o $j -v $v -c $c 
#            do python cardMaker.py -i ../config/analysis_${c}.cfg -f /home/llr/cms/cadamuro/HHKLUBGiacomo/CMSSW_7_4_7/src/KLUBAnalysis/analysis_${c}_resonant/evalQCD_15Gen2015_Limits4Giacomo_30fb/outPlotter.root -o $j -v $v -c $c 
#	    done
#	done
#	done 
#	mv cards cards_${c}_$OUTSTRING
#done

#make WS 
###for c in ETau MuTau TauTau
###	do
###		for i in $LAMBDAS 
###		do	
###		cd cards_${c}_$OUTSTRING/${i}${j}${v}
###		pwd
###		if [ -a "hh_3_L${i}_13TeV.txt" ]
###			then
###			combineCards.py -S "hh_3_L${i}_13TeV.txt" >> comb.txt 
###		fi
###		if [ -a "hh_2_L${i}_13TeV.txt" ]
###			then
###			combineCards.py -S "hh_2_L${i}_13TeV.txt" >> comb.txt 
###		fi
###		if [ -a "hh_1_L${i}_13TeV.txt" ]
###			then
###			combineCards.py -S "hh_1_L${i}_13TeV.txt" >> comb.txt 
###		fi
###		text2workspace.py comb.txt -o comb.root ;
###		#combine -M Asymptotic comb.root -m 125.0 -t -1 --expectSignal=1 -n ${i} -S 0
###		combine -M Asymptotic comb.root -m 125.0 -n ${i}_forLim
###		combine -M HybridNew --frequentist --testStat LHC comb.root  -H ProfileLikelihood -n ${i}_forLim --expectedFromGrid=0.5
###		cd -
###	done
###	done

for i in $LAMBDAS 
	do	
	cd cards_MuTau_$OUTSTRING/${i}${BASEMUTAU}${VARMUTAU}
	pwd
	combineCards.py -S "hh_2_L${i}_13TeV.txt" >> comb.txt 
	text2workspace.py comb.txt -o comb.root ;
	combine -M Asymptotic comb.root -m 125.0 -n ${i}_forLim
	combine -M HybridNew --frequentist -m 125.0 --testStat LHC comb.root  -H ProfileLikelihood -n ${i}_forLim --expectedFromGrid=0.5
	cd -
	cd cards_ETau_$OUTSTRING/${i}${BASEETAU}${VARETAU}
	pwd
	combineCards.py -S "hh_1_L${i}_13TeV.txt" >> comb.txt 
	text2workspace.py comb.txt -o comb.root ;
	combine -M Asymptotic comb.root -m 125.0 -n ${i}_forLim
	combine -M HybridNew --frequentist -m 125.0 --testStat LHC comb.root  -H ProfileLikelihood -n ${i}_forLim --expectedFromGrid=0.5
	cd -
	cd cards_TauTau_$OUTSTRING/${i}${BASETAUTAU}${VARTAUTAU}
	pwd
	combineCards.py -S "hh_3_L${i}_13TeV.txt" >> comb.txt 
	text2workspace.py comb.txt -o comb.root ;
	combine -M Asymptotic comb.root -m 125.0 -n ${i}_forLim
	combine -M HybridNew --frequentist -m 125.0 --testStat LHC comb.root  -H ProfileLikelihood -n ${i}_forLim --expectedFromGrid=0.5
	cd -

	done


#
###do the combination
##for i in $LAMBDAS 
##do	
##	for j in $ALLSELS ; 
##	do 
##	for v in $ALLVARS
##	do 
##	mkdir -p cards_Combined_$OUTSTRING/${i}${j}${v}
##	cp cards_MuTau_$OUTSTRING/${i}${j}${v}/hh_*.* cards_Combined_$OUTSTRING/${i}${j}${v}/.
##	cp cards_TauTau_$OUTSTRING/${i}${j}${v}/hh_*.* cards_Combined_$OUTSTRING/${i}${j}${v}/.
##	cp cards_ETau_$OUTSTRING/${i}${j}${v}/hh_*.* cards_Combined_$OUTSTRING/${i}${j}${v}/.
##	cd cards_Combined_$OUTSTRING/${i}${j}${v}
##	combineCards.py -S "hh_3_L${i}_13TeV.txt" "hh_2_L${i}_13TeV.txt" "hh_1_L${i}_13TeV.txt" >> comb.txt
##	text2workspace.py comb.txt -o comb.root ;
##		#combine -M Asymptotic comb.root -m 125.0 -t -1 --expectSignal=1 -n ${i} -S 0
##	combine -M Asymptotic comb.root -m 125.0 -n ${i}_forLim
##	cd -
##	done
##	done
##done 
#
#
#MIXED CUT COMBINATION
for i in $LAMBDAS
do
        mkdir -p cards_Combined_$OUTSTRING/${i}COMBINED
        cp cards_MuTau_$OUTSTRING/${i}${BASEMUTAU}${VARMUTAU}/hh_*.* cards_Combined_$OUTSTRING/${i}COMBINED/.
        cp cards_TauTau_$OUTSTRING/${i}${BASETAUTAU}${VARTAUTAU}/hh_*.* cards_Combined_$OUTSTRING/${i}COMBINED/.
        cp cards_ETau_$OUTSTRING/${i}${BASEETAU}${VARETAU}/hh_*.* cards_Combined_$OUTSTRING/${i}COMBINED/.
        cd cards_Combined_$OUTSTRING/${i}COMBINED
        combineCards.py -S "hh_3_L${i}_13TeV.txt" "hh_2_L${i}_13TeV.txt" "hh_1_L${i}_13TeV.txt" >> comb.txt
        text2workspace.py comb.txt -o comb.root ;
                #combine -M Asymptotic comb.root -m 125.0 -t -1 --expectSignal=1 -n ${i} -S 0
        combine -M Asymptotic comb.root -m 125.0 -n ${i}_forLim
        combine -M HybridNew --frequentist -m 125.0 --testStat LHC comb.root  -H ProfileLikelihood -n ${i}_forLim --expectedFromGrid=0.5
        cd -
        done
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

