#!/bin/bash
# make cards with all vars/selections

export OUTSTRING="2016_07_26_out3"


export SELECTIONS="resolved_2b0j_Mcut resolved_1b1j_Mcut boosted_Mcut defaultBtagMMNoIsoBBTTCut"
export SELECTIONSTAU="resolved_2b0j_Mcut resolved_1b1j_Mcut boosted_Mcut  defaultBtagLLNoIsoBBTTCut45"
#export SELECTIONS="defaultBtagLLNoIso, defaultBtagLLNoIsoBBTTCut, defaultBtagLLNoIsoBBTTCutKine80"
#export SELECTIONSTAU="defaultBtagLLNoIso45, defaultBtagLLNoIsoBBTTCut45"

export VARTAUTAU="HHKin_mass_raw"
export VARMUTAU="HHKin_mass_raw"
export VARETAU="HHKin_mass_raw"

export LAMBDAS="Radion250 Radion260 Radion270 Radion280 Radion300 Radion320 Radion340 Radion400 Radion450 Radion500 Radion550 Radion600 Radion650 Radion700 Radion750 Radion800 Radion900"
#export LAMBDAS="Radion750"
export QUANTILES="0.025 0.16 0.5 0.84 0.975 -1.0"

export SOURCE="/home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis" 
export FOLDER="evalQCD_Plots_2016_Resonant_25lug_12fb_lessLooseQCD"

 for base in $SELECTIONS
 do
 	echo "python cardMaker.py -i ${SOURCE}/analysis_ETau/${FOLDER}/resonantAnalysis_ETau.cfg     -f ${SOURCE}/analysis_ETau/${FOLDER}/outPlotter3.root   -o $base -v $VARETAU   -c ETau   --dir "_$OUTSTRING" -t 0 -r 1"
 python cardMaker.py -i ${SOURCE}/analysis_ETau/${FOLDER}/resonantAnalysis_ETau.cfg     -f ${SOURCE}/analysis_ETau/${FOLDER}/outPlotter3.root   -o $base -v $VARETAU   -c ETau   --dir "_$OUTSTRING" -t 0 -r 1 #-S /home/llr/cms/cadamuro/FittedHHKinFit/outPlotter_fit_sigs_ETau.root
 python cardMaker.py -i ${SOURCE}/analysis_MuTau/${FOLDER}/resonantAnalysis_MuTau.cfg   -f ${SOURCE}/analysis_MuTau/${FOLDER}/outPlotter3.root  -o $base -v $VARMUTAU  -c MuTau  --dir "_$OUTSTRING" -t 0 -r 1 #-S /home/llr/cms/cadamuro/FittedHHKinFit/outPlotter_fit_sigs_MuTau.root
 done
 for base in $SELECTIONSTAU
 do
 python cardMaker.py -i ${SOURCE}/analysis_TauTau/${FOLDER}/resonantAnalysis_TauTau.cfg -f ${SOURCE}/analysis_TauTau/${FOLDER}/outPlotter.root -o $base -v $VARTAUTAU -c TauTau --dir "_$OUTSTRING" -t 0 -r 1 #-S /home/llr/cms/cadamuro/FittedHHKinFit/outPlotter_fit_sigs_TauTau.root
 done
 
 for i in $LAMBDAS
 	do 
 	#MAKE LIMIT FOR CHANNEL/CATEGORY
 	    for base in $SELECTIONS
 	    do	
 	    ## for c in MuTau ETau TauTau 
 	    ## do
 	    ## 	if [ -a "hh_3_L${i}_13TeV.txt" ]
 	    ## 		then
 	    ## 		combineCards.py -S "hh_3_L${i}_13TeV.txt" >> comb.txt 
 	    ## 	fi
 	    cd cards_MuTau_$OUTSTRING/${i}${base}${VARMUTAU}
 	    pwd
 	    #combineCards.py -S hh_2_*L${i}_13TeV.txt >> comb.txt 
 	    combineCards.py hh_1_C1_L${i}_13TeV.txt hh_1_C2_L${i}_13TeV.txt >> comb.txt
 	    text2workspace.py comb.txt -o comb.root ;
 	    ln -ns ../../prepareHybrid.py .
 	    ln -ns ../../prepareGOF.py .
 	    ln -ns ../../prepareAsymptotic.py .
 	    python prepareAsymptotic.py -n $i
 	    #for q in $QUANTILES
 	    #do
 	    #	python prepareHybrid.py -n $q
 	    #done
 	    cd -
 	    cd cards_ETau_$OUTSTRING/${i}${base}${VARETAU}
 	    pwd
 	    #combineCards.py -S hh_1_*L${i}_13TeV.txt >> comb.txt 
 	    combineCards.py hh_2_C1_L${i}_13TeV.txt hh_2_C2_L${i}_13TeV.txt >> comb.txt
 	    text2workspace.py comb.txt -o comb.root ;
 	    ln -ns ../../prepareHybrid.py .
 	    ln -ns ../../prepareGOF.py .
 	    ln -ns ../../prepareAsymptotic.py .
 	    python prepareAsymptotic.py -n $i
 	    #for q in $QUANTILES
 	    #do
 	    #	python prepareHybrid.py -n $q
 	    #done
 	    cd -
 	    # if [ $i == "Radion450" ]
 	    # 	then
 	    # 	for g in {0..100}
 	    # 		do
 	    # 			python prepareGOF.py -n $g
 	    # 		done
 	    # fi
 	done
 	for base in $SELECTIONSTAU
 	    do	
 	    cd cards_TauTau_$OUTSTRING/${i}${base}${VARTAUTAU}
 	    pwd
 	    #combineCards.py -S hh_3_*L${i}_13TeV.txt >> comb.txt 
 	    combineCards.py hh_1_C1_L${i}_13TeV.txt hh_3_C2_L${i}_13TeV.txt hh_3_C3_L${i}_13TeV.txt >> comb.txt
 	    text2workspace.py comb.txt -o comb.root ;
 	    ln -ns ../../prepareHybrid.py .
 	    ln -ns ../../prepareGOF.py .
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
     done
 
 

#CATEGORY COMBINATION
for i in $LAMBDAS
do
	 	mkdir -p cards_Combined_$OUTSTRING/${i}
    #MAKE COMBINATION FOR CHANNEL
 	for base in $SELECTIONS
 	do
         mkdir -p cards_Combined_$OUTSTRING/${i}${base}${VARMUTAU}
         cp cards_MuTau_$OUTSTRING/${i}${base}${VARMUTAU}/hh_*.* cards_Combined_$OUTSTRING/${i}${base}${VARMUTAU}/.
         cp cards_MuTau_$OUTSTRING/${i}${base}${VARMUTAU}/hh_*.* cards_Combined_$OUTSTRING/${i}/.
         if [ $base  == "defaultBtagMMNoIsoBBTTCut" ]
         	then 
         	cp cards_TauTau_$OUTSTRING/${i}"defaultBtagLLNoIsoBBTTCut45"${VARTAUTAU}/hh_*.* cards_Combined_$OUTSTRING/${i}${base}${VARMUTAU}/.
         	cp cards_TauTau_$OUTSTRING/${i}"defaultBtagLLNoIsoBBTTCut45"${VARTAUTAU}/hh_*.* cards_Combined_$OUTSTRING/${i}/.
         	cd cards_TauTau_$OUTSTRING
         	ln -ns ${i}"defaultBtagLLNoIsoBBTTCut45"${VARTAUTAU} ${i}"defaultBtagMMNoIsoBBTTCut"${VARTAUTAU}
         	cd -
         else 
         	cp cards_TauTau_$OUTSTRING/${i}${base}${VARTAUTAU}/hh_*.* cards_Combined_$OUTSTRING/${i}${base}${VARMUTAU}/.
         	cp cards_TauTau_$OUTSTRING/${i}${base}${VARTAUTAU}/hh_*.* cards_Combined_$OUTSTRING/${i}/.
         fi
         cp cards_ETau_$OUTSTRING/${i}${base}${VARETAU}/hh_*.* cards_Combined_$OUTSTRING/${i}${base}${VARMUTAU}/.
         cp cards_ETau_$OUTSTRING/${i}${base}${VARETAU}/hh_*.* cards_Combined_$OUTSTRING/${i}/.
         cd cards_Combined_$OUTSTRING/${i}${base}${VARMUTAU}
         pwd
         if [ -a "hh_1_C1_L${i}_13TeV.txt" ]
         	then
         	combineCards.py -S hh_1_C1_L${i}_13TeV.txt hh_2_C1_L${i}_13TeV.txt hh_3_C1_L${i}_13TeV.txt >> comb.txt
         fi
         if [ -a "hh_1_C2_L${i}_13TeV.txt" ]
         	then
         	combineCards.py -S hh_1_C2_L${i}_13TeV.txt hh_2_C2_L${i}_13TeV.txt hh_3_C2_L${i}_13TeV.txt >> comb.txt
         fi
         if [ -a "hh_1_C3_L${i}_13TeV.txt" ]
         	then
         	combineCards.py -S hh_1_C3_L${i}_13TeV.txt hh_2_C3_L${i}_13TeV.txt hh_3_C3_L${i}_13TeV.txt >> comb.txt
         fi
         if [ -a "hh_1_C999_L${i}_13TeV.txt" ]
         	then
         	combineCards.py -S hh_1_C999_L${i}_13TeV.txt hh_2_C999_L${i}_13TeV.txt hh_3_C999_L${i}_13TeV.txt >> comb.txt
         fi
         text2workspace.py comb.txt -o comb.root ;
         #combine -M Asymptotic comb.root -m 125.0 -t -1 --expectSignal=1 -n ${i} -S 0
         #combine -M Asymptotic comb.root -m 125.0 -n ${i}_forLim
         ln -ns ../../prepareHybrid.py .
         ln -ns ../../prepareGOF.py .
         ln -ns ../../prepareAsymptotic.py .
         python prepareAsymptotic.py -n $i
         #combine -M HybridNew --frequentist -m 125.0 --testStat LHC comb.root  -H ProfileLikelihood -n ${i}_forLim --expectedFromGrid=0.5
         #for q in $QUANTILES
         #do
 		#python prepareHybrid.py -n $q
 	    #done
        cd -
    done

	#MAKE BIG COMBINATION
    cd cards_Combined_$OUTSTRING/${i}
    rm comb.*
    combineCards.py -S hh_*_C1_L${i}_13TeV.txt hh_*_C2_L${i}_13TeV.txt hh_*_C3_L${i}_13TeV.txt >> comb.txt
    #combineCards.py -S hh_*_C2_L${i}_13TeV.txt hh_*_C3_L${i}_13TeV.txt >> comb.txt
    text2workspace.py comb.txt -o comb.root ;
    ln -ns ../../prepareHybrid.py .
    ln -ns ../../prepareGOF.py .
    ln -ns ../../prepareAsymptotic.py .
    python prepareAsymptotic.py -n $i
    if [ $i == "Radion750" ]
    	then
    	ln -ns ../../prepareImpacts.py .
    	python prepareImpacts.py -n $i
    fi
    cd -
	for c in MuTau ETau TauTau 
	do
		mkdir -p cards_${c}_$OUTSTRING/${i}
		for base in $SELECTIONS
		do
			cp cards_${c}_$OUTSTRING/${i}${base}${VARMUTAU}/hh_*C*.* cards_${c}_$OUTSTRING/${i}/.
		done
		cd cards_${c}_$OUTSTRING/${i}
		rm comb.*
		combineCards.py -S hh_*_C1_L${i}_13TeV.txt hh_*_C2_L${i}_13TeV.txt hh_*_C3_L${i}_13TeV.txt >> comb.txt
		#combineCards.py -S  hh_*_C2_L${i}_13TeV.txt hh_*_C3_L${i}_13TeV.txt >> comb.txt
		text2workspace.py comb.txt -o comb.root ;
		ln -ns ../../prepareHybrid.py .
		ln -ns ../../prepareGOF.py .
		ln -ns ../../prepareAsymptotic.py .
		python prepareAsymptotic.py -n $i
		cd -
    done


done



