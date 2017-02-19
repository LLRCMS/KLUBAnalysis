#!/bin/bash
# make cards with all vars/selections

export OUTSTRING="2017_02_19_btag_$1"

export STRINGLEPTONS="$1"
export SELECTIONS="s2b0jresolvedMcut${STRINGLEPTONS} s1b1jresolvedMcut${STRINGLEPTONS} sboostedLLMcut s2b0jresolvedLcut${STRINGLEPTONS} s1b1jresolvedLcut${STRINGLEPTONS}"
export SELECTIONSTAU="s1b1jresolvedMcut s2b0jresolvedMcut sboostedLLMcut"

export RESONANT=$2
export LEPTONS="MuTau ETau TauTau"

export CF="/home/llr/cms/ortona/diHiggs/CMSSW_7_4_7/src/KLUBAnalysis/combiner"
if [ "${RESONANT}" != "-r" ]
    then
    export VARIABLE="MT2"
        export LAMBDAS=""
        #export INSELECTIONS="s2b0jresolvedMcutlmr90 s1b1jresolvedMcutlmr90 s2b0jresolvedMcuthmr90 s1b1jresolvedMcuthmr90 sboostedLLMcut s1b1jresolvedMcutlmr70 s2b0jresolvedMcutlmr70 s1b1jresolvedMcutLepTauKine s2b0jresolvedMcutLepTauKine"
        for il in {0..51}
        do 
        export LAMBDAS="$LAMBDAS lambdarew${il}"
    done
    else
        export VARIABLE="HHKin_mass_raw"
        #export LAMBDAS="Radion250 Radion260 Radion270 Radion280 Radion300 Radion320 Radion340 Radion400 Radion450 Radion500 Radion550 Radion600 Radion650 Radion700 Radion750 Radion800 Radion900"
        #export LAMBDAS="Radion250"
        export LAMBDAS="Radion250 Radion270 Radion280 Radion300 Radion350 Radion400 Radion450 Radion500 Radion550 Radion600 Radion650 Radion750 Radion900"
fi
#echo "LAMBDAS ======================"
#echo $LAMBDAS
export QUANTILES="0.025 0.16 0.5 0.84 0.975 -1.0"
export SOURCE="/home/llr/cms/cadamuro/testAnalysisHelper2/CMSSW_7_4_7/src/KLUBAnalysis" 

#create all the cards 3 categories x 3 channels
for ibase in $SELECTIONS
do
    for c in $LEPTONS
    do
        if [ "${c}" == "MuTau" ]
        then
            export BASE="$ibase"
        fi
        if [ "${c}" == "ETau" ]
            then
            export BASE="$ibase"
        fi
        if [ "${c}" == "TauTau" ]
            then
            export BASE=${ibase/${STRINGLEPTONS}/}
            echo "$BASE"
        fi
    #python cardMaker.py -i ${SOURCE}/analysis_${c}_1Feb_lims/mainCfg_${c}.cfg -f ${SOURCE}/analysis_${c}_1Feb_lims/analyzedOutPlotter.root   -o $BASE -c ${c}   --dir "_$OUTSTRING" -t 0 ${RESONANT} 
    #python chcardMaker.py -f ${SOURCE}/analysis_${c}_11Feb_lims/analyzedOutPlotter.root -o ${OUTSTRING} -c ${c} -i ${SOURCE}/analysis_${c}_11Feb_lims/mainCfg_TauTau.cfg -y -s ${BASE} ${RESONANT}
    python chcardMaker.py -f analyzedOutPlotter_26feb_${c}.root -o "_${OUTSTRING}" -c ${c} -i ${SOURCE}/analysis_${c}_11Feb_lims/mainCfg_${c}.cfg -y -s ${BASE} ${RESONANT} -u 1
#done
#for base in $SELECTIONSTAU
#do
    #python cardMaker.py -i ${SOURCE}/analysis_TauTau_26Gen/mainCfg_TauTau.cfg   -f ${SOURCE}/analysis_TauTau_26Gen/analyzedOutPlotter.root -o $BASE -c TauTau --dir "_$OUTSTRING" -t 0 # -r #-S /home/llr/cms/cadamuro/FittedHHKinFit/outPlotter_fit_sigs_TauTau.root
    done
done
echo " "
echo "CREATED ALL CARDS"
for i in $LAMBDAS
do 
#MAKE LIMIT FOR individual CHANNEL/categories [9 x mass points]
    for ibase in $SELECTIONS
    do	
        for c in $LEPTONS
        do
 	    	#if [ -a "hh_3_L${i}_13TeV.txt" ]
 	    	#	then
 	    	#	combineCards.py -S "hh_3_L${i}_13TeV.txt" >> comb.txt 
 	    	#fi
            if [ "${c}" == "MuTau" ]
            then
                export chanNum="2"
                echo "${c} ${chanNum}"
                export BASE="$ibase"
            fi
            if [ "${c}" == "ETau" ]
            then
                export chanNum="1"
                echo "${c} ${chanNum}" 
                export BASE="$ibase"
            fi
            if [ "${c}" == "TauTau" ]
            then
                export chanNum="3"
                echo "${c} ${chanNum}" 
                export BASE=${ibase/${STRINGLEPTONS}/}
            fi
            cd ${CF}/cards_${c}_$OUTSTRING/${i}${BASE}${VARIABLE}
            pwd
 	        #combineCards.py -S hh_${chanNum}_*L${i}_13TeV.txt >> comb.txt 
            combineCards.py -S hh_*.txt >> comb.txt 
            #combineCards.py hh_${chanNum}_C1_L${i}_13TeV.txt hh_${chanNum}_C2_L${i}_13TeV.txt hh_${chanNum}_C3_L${i}_13TeV.txt >> comb.txt
            text2workspace.py -m 125 comb.txt -o comb.root ;
            ln -ns ../../prepareHybrid.py .
            ln -ns ../../prepareGOF.py .
            ln -ns ../../prepareAsymptotic.py .
            python prepareAsymptotic.py -n $i
            cd ${CF}
        done
    done
done

#CATEGORY COMBINATION
for i in $LAMBDAS
do
	mkdir -p cards_Combined_$OUTSTRING/${i}${VARIABLE}
    #MAKE COMBINATION FOR CATEGORY [3 x mass point]
    for ibase in $SELECTIONS
    do
        mkdir -p cards_Combined_$OUTSTRING/${i}${ibase}${VARIABLE}
        for c in $LEPTONS
        do
            if [ "${c}" == "MuTau" ]
            then
                export BASE="$ibase"
            fi
            if [ "${c}" == "ETau" ]
            then
                export BASE="$ibase"
            fi
            if [ "${c}" == "TauTau" ]
            then
                export BASE=${ibase/${STRINGLEPTONS}/}
            fi
            mkdir -p cards_${c}_$OUTSTRING/${i}${VARIABLE}
            cp cards_${c}_$OUTSTRING/${i}${BASE}${VARIABLE}/hh_*.* cards_Combined_$OUTSTRING/${i}${ibase}${VARIABLE}/.
            cp cards_${c}_$OUTSTRING/${i}${BASE}${VARIABLE}/hh_*.* cards_Combined_$OUTSTRING/${i}${VARIABLE}/.
            cp cards_${c}_$OUTSTRING/${i}${BASE}${VARIABLE}/hh_*.* cards_${c}_$OUTSTRING/${i}${VARIABLE}/.
        done
        cd cards_Combined_$OUTSTRING/${i}${ibase}${VARIABLE}
        pwd
        if [ -a "hh_1_C1_L${i}_13TeV.txt" ] #category 1
            then
            combineCards.py -S hh_*_C1_L${i}_13Te*.txt  >> comb.txt
        fi
        if [ -a "hh_1_C2_L${i}_13TeV.txt" ]
            then
            combineCards.py -S hh_*_C2_L${i}_13Te*.txt >> comb.txt
        fi
        if [ -a "hh_1_C3_L${i}_13TeV.txt" ]
         	then
         	combineCards.py -S hh_*_C3_L${i}_13Te*.txt >> comb.txt
        fi
#        if [ -a "hh_1_C999_L${i}_13TeV.txt" ]
#           then
#           combineCards.py -S hh_1_C999_L${i}_13TeV.txt hh_2_C999_L${i}_13TeV.txt hh_3_C999_L${i}_13TeV.txt >> comb.txt
#        fi
         text2workspace.py -m 125 comb.txt -o comb.root ;
         ln -ns ../../prepareHybrid.py .
         ln -ns ../../prepareGOF.py .
         ln -ns ../../prepareAsymptotic.py .
         python prepareAsymptotic.py -n $i
         #combine -M HybridNew --frequentist -m 125.0 --testStat LHC comb.root  -H ProfileLikelihood -n ${i}_forLim --expectedFromGrid=0.5
         #for q in $QUANTILES
         #do
 		#python prepareHybrid.py -n $q
 	    #done
        cd ${CF}
    done

	#MAKE BIG COMBINATION [1 x mass point]
    cd cards_Combined_$OUTSTRING/${i}${VARIABLE}
    rm comb.*
    combineCards.py -S hh_*_C1_L${i}_13Te*.txt hh_*_C2_L${i}_13Te*.txt hh_*_C3_L${i}_13Te*.txt >> comb.txt #ah ma allora le wildcard funzionano?
    #combineCards.py -S hh_*_C2_L${i}_13TeV.txt hh_*_C3_L${i}_13TeV.txt >> comb.txt
    text2workspace.py -m 125 comb.txt -o comb.root ;
    ln -ns ../../prepareHybrid.py .
    ln -ns ../../prepareGOF.py .
    ln -ns ../../prepareAsymptotic.py .
    ln -ns ../../prepareImpacts.py .
    python prepareAsymptotic.py -n $i
#    if [ $i == "Radion750" ]
#    	then
#    	ln -ns ../../prepareImpacts.py .
#    	python prepareImpacts.py -n $i
#    fi
    cd ${CF}

    #MAKE COMBINATION FOR CHANNEL [3 x mass point]
	for c in $LEPTONS 
	do
#		for base in $SELECTIONS
#		do
#			cp cards_${c}_$OUTSTRING/${i}${base}${VARIABLE}/hh_*C*.* cards_${c}_$OUTSTRING/${i}${VARIABLE}/.
#		done
		cd cards_${c}_$OUTSTRING/${i}${VARIABLE}
		rm comb.*
		combineCards.py -S hh_*_C1_L${i}_13Te*.txt hh_*_C2_L${i}_13Te*.txt hh_*_C3_L${i}_13Te*.txt >> comb.txt
		#combineCards.py -S  hh_*_C2_L${i}_13TeV.txt hh_*_C3_L${i}_13TeV.txt >> comb.txt
		text2workspace.py -m 125 comb.txt -o comb.root ;
		ln -ns ../../prepareHybrid.py .
		ln -ns ../../prepareGOF.py .
		ln -ns ../../prepareAsymptotic.py .
		python prepareAsymptotic.py -n $i
		cd ${CF}
    done


done



