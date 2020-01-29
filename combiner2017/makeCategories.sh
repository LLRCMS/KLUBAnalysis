#!/bin/bash
# make cards with all vars/selections

#export OUTSTRING="2017_02_19_btag_$1"
export OUTSTRING="2019_10_11_vtight"


export SELECTIONS="s2b0jresolvedMcut s1b1jresolvedMcut sboostedLLMcut VBFtight VBFloose"
#export SELECTIONS="s2b0jresolvedMcut s1b1jresolvedMcut sboostedLLMcut"
export NAMESAMPLE="ggHH_bbtt"
#"ggHH_bbtt"
export RESONANT=$2
#export LEPTONS=$1
export LEPTONS="ETau MuTau TauTau"

klvar=(-20 -15 -10 -8 -6 -4 -3 -2 -1 0.001 1 2 3 2.45 4 5 6 7 8 9 10 11 12 14 16 18 20 25 30)


export CF="$CMSSW_BASE/src/KLUBAnalysis/combiner2017"
if [ "${RESONANT}" != "-r" ]
    then
    export VARIABLE=""
        export LAMBDAS=""
	export VARIABLES=""
        #export INSELECTIONS="s2b0jresolvedMcutlmr90 s1b1jresolvedMcutlmr90 s2b0jresolvedMcuthmr90 s1b1jresolvedMcuthmr90 sboostedLLMcut s1b1jresolvedMcutlmr70 s2b0jresolvedMcutlmr70 s1b1jresolvedMcutLepTauKine s2b0jresolvedMcutLepTauKine"
        for il in {1..29}
        do 
        export LAMBDAS="$LAMBDAS ${il}"
	export VARIABLES="${VARIABLES} BDToutSM_kl_${klvar[$((il-1))]}" 
    done
    else
        export VARIABLE="HHKin_mass_raw"
        export NAMESAMPLE="Radion"
        #export LAMBDAS="Radion250 Radion260 Radion270 Radion280 Radion300 Radion320 Radion340 Radion400 Radion450 Radion500 Radion550 Radion600 Radion650 Radion700 Radion750 Radion800 Radion900"
        #export LAMBDAS="Radion250"
        export LAMBDAS="250 270 280 300 350 400 450 500 550 600 650 750 900"
fi
#export LAMBDAS="21"
#echo "LAMBDAS ======================"
#echo $LAMBDAS
export QUANTILES="0.025 0.16 0.5 0.84 0.975 -1.0"
export SOURCE="/home/llr/cms/cadamuro/testAnalysisHelper2/CMSSW_7_4_7/src/KLUBAnalysis" 

echo $VARIABLES
arrVARIABLES=($VARIABLES)

#create all the cards 3 categories x 3 channels
for ibase in $SELECTIONS
do
    for c in $LEPTONS
    do
        export BASE="$ibase"
        
    #python cardMaker.py -i ${SOURCE}/analysis_${c}_1Feb_lims/mainCfg_${c}.cfg -f ${SOURCE}/analysis_${c}_1Feb_lims/analyzedOutPlotter.root   -o $BASE -c ${c}   --dir "_$OUTSTRING" -t 0 ${RESONANT} 
    #python chcardMaker.py -f ${SOURCE}/analysis_${c}_19Feb/analyzedOutPlotter.root -o ${OUTSTRING} -c ${c} -i ${SOURCE}/analysis_${c}_19Feb/mainCfg_TauTau.cfg -y -s ${BASE} ${RESONANT} -u 0
    #python chcardMaker.py -f analyzedOutPlotter_01Mar_2D_${c}.root -o "_${OUTSTRING}" -c ${c} -i ${SOURCE}/analysis_${c}_1Mar_lims_2dscan/mainCfg_${c}.cfg -y -s ${BASE} ${RESONANT} -u 1 -t 
	if [ "${c}" == "MuTau" ]
        then
	    if [[ "${ibase}" == *"VBFtight"*  ]]
	    then
		continue
	    fi
        fi
        if [ "${c}" == "ETau" ]
        then
	    if [[ "${ibase}" == *"VBFtight"*  ]]
	    then
		continue
	    fi
        fi
	python chcardMaker.py -f analyzedOutPlotter_2019_10_11_vtight_${c}.root -o "_${OUTSTRING}" -c ${c} -y -s ${BASE} ${RESONANT} -u 1 -t 
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
            export BASE="$ibase"
            if [ "${c}" == "MuTau" ]
            then
                export chanNum="2"
                echo "${c} ${chanNum}"
		if [[ "${ibase}" == *"VBFtight"*  ]]
		then
		    continue
		fi
            fi
            if [ "${c}" == "ETau" ]
            then
                export chanNum="1"
                echo "${c} ${chanNum}" 
		if [[ "${ibase}" == *"VBFtight"*  ]]
		then
		    continue
		fi
            fi
            if [ "${c}" == "TauTau" ]
            then
                export chanNum="3"
                echo "${c} ${chanNum}" 

            fi
	    echo ${CF}/cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${BASE}${arrVARIABLES[$((i-1))]}
            cd ${CF}/cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${BASE}${arrVARIABLES[$((i-1))]}
            pwd
 	        #combineCards.py -S hh_${chanNum}_*L${i}_13TeV.txt >> comb.txt 
            combineCards.py -S hh_*.txt >> comb.txt 
            #combineCards.py hh_${chanNum}_C1_L${i}_13TeV.txt hh_${chanNum}_C2_L${i}_13TeV.txt hh_${chanNum}_C3_L${i}_13TeV.txt >> comb.txt
            text2workspace.py -m ${i} comb.txt -o comb.root ;
            ln -ns ../../prepareHybrid.py .
            ln -ns ../../prepareGOF.py .
            ln -ns ../../prepareAsymptotic.py .
            python prepareAsymptotic.py -m ${i} -n ${NAMESAMPLE}$i
            cd ${CF}
        done
    done
done

#CATEGORY COMBINATION
for i in $LAMBDAS
do
    cd ${CF}
	mkdir -p cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${arrVARIABLES[$((i-1))]}
	pwd
    #MAKE COMBINATION FOR CATEGORY [3 x mass point]
    for ibase in $SELECTIONS
    do
        mkdir -p cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${ibase}${arrVARIABLES[$((i-1))]}
        for c in $LEPTONS
        do
             export BASE="$ibase"
	     if [[ "${c}" != "TauTau" && "${ibase}" == *"VBFtight"* ]]
	     then
		 continue
	     fi
            mkdir -p cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${arrVARIABLES[$((i-1))]}
            cp cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${BASE}${arrVARIABLES[$((i-1))]}/hh_*.* cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${ibase}${arrVARIABLES[$((i-1))]}/.
            cp cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${BASE}${arrVARIABLES[$((i-1))]}/hh_*.* cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${arrVARIABLES[$((i-1))]}/.
            cp cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${BASE}${arrVARIABLES[$((i-1))]}/hh_*.* cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${arrVARIABLES[$((i-1))]}/.
        done
        cd cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${ibase}${arrVARIABLES[$((i-1))]}
        pwd
        if [ -a "hh_3_C1_L${NAMESAMPLE}${i}_13TeV.txt" ] #category 1
            then
            combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt  >> comb.txt
        fi
        if [ -a "hh_3_C2_L${NAMESAMPLE}${i}_13TeV.txt" ]
            then
            combineCards.py -S hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt >> comb.txt
        fi
        if [ -a "hh_3_C3_L${NAMESAMPLE}${i}_13TeV.txt" ]
         	then
         	combineCards.py -S hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt >> comb.txt
	fi
        if [ -a "hh_3_C4_L${NAMESAMPLE}${i}_13TeV.txt" ]
         	then
         	combineCards.py -S hh_*_C4_L${NAMESAMPLE}${i}_13Te*.txt >> comb.txt

        fi
        if [ -a "hh_3_C5_L${NAMESAMPLE}${i}_13TeV.txt" ]
         	then
         	combineCards.py -S hh_3_C5_L${NAMESAMPLE}${i}_13Te*.txt >> comb.txt

	 fi
#        if [ -a "hh_1_C999_L${i}_13TeV.txt" ]
#           then
#           combineCards.py -S hh_1_C999_L${i}_13TeV.txt hh_2_C999_L${i}_13TeV.txt hh_3_C999_L${i}_13TeV.txt >> comb.txt
#        fi
         text2workspace.py -m ${i} comb.txt -o comb.root ;
         ln -ns ../../prepareHybrid.py .
         ln -ns ../../prepareGOF.py .
         ln -ns ../../prepareAsymptotic.py .
         python prepareAsymptotic.py -m ${i} -n ${NAMESAMPLE}$i
#         if [ $i == "21" ]
#            then
#            for g in {0..20}
#            do
#               python prepareGOF.py -n $g 
#           done
#       fi
         #combine -M HybridNew --frequentist -m 125.0 --testStat LHC comb.root  -H ProfileLikelihood -n ${i}_forLim --expectedFromGrid=0.5
         #for q in $QUANTILES
         #do
 		#python prepareHybrid.py -n $q
 	    #done
        cd ${CF}
    done

	#MAKE BIG COMBINATION [1 x mass point]
    cd cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${arrVARIABLES[$((i-1))]}
    rm comb.*
    combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C4_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C5_L${NAMESAMPLE}${i}_13Te*.txt>> comb.txt 
    #combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C5_L${NAMESAMPLE}${i}_13Te*.txt>> comb.txt 
    #combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt >> comb.txt 
    #combineCards.py -S hh_*_C2_L${i}_13TeV.txt hh_*_C3_L${i}_13TeV.txt >> comb.txt
    text2workspace.py -m ${i} comb.txt -o comb.root ;
    ln -ns ../../prepareHybrid.py .
    ln -ns ../../prepareGOF.py .
    ln -ns ../../prepareAsymptotic.py .
    ln -ns ../../prepareImpacts.py .
    python prepareAsymptotic.py -m ${i} -n ${NAMESAMPLE}$i
#    if [ $i == "21" ]
#    	then
##    	ln -ns ../../prepareImpacts.py .
##        python prepareImpacts.py -n $i -e 0
##        python prepareImpacts.py -n $i -e 1
##        for g in {0..20}
##        do
##           python prepareGOF.py -n $g 
#    #       done
#    for q in $QUANTILES
#    do
#       python prepareHybrid.py -n $q
#   done
# fi
 cd ${CF}

    #MAKE COMBINATION FOR CHANNEL [3 x mass point]
	for c in $LEPTONS 
	do
#		for base in $SELECTIONS
#		do
#			cp cards_${c}_$OUTSTRING/${i}${base}${VARIABLE}/hh_*C*.* cards_${c}_$OUTSTRING/${i}${VARIABLE}/.
#		done
		cd cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${arrVARIABLES[$((i-1))]}
		rm comb.*
		if [[ ${c} == "TauTau" ]]
		then
		    combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C4_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C5_L${NAMESAMPLE}${i}_13Te*.txt>> comb.txt
		else
		    combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C4_L${NAMESAMPLE}${i}_13Te*.txt>> comb.txt
		fi

		#if [[ ${c} == "TauTau" ]]
		#then
		#    combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C5_L${NAMESAMPLE}${i}_13Te*.txt>> comb.txt
		#else
		#    combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt >> comb.txt
		#fi
		# combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt >> comb.txt
		#combineCards.py -S  hh_*_C2_L${i}_13TeV.txt hh_*_C3_L${i}_13TeV.txt >> comb.txt
		text2workspace.py -m ${i} comb.txt -o comb.root ;
		ln -ns ../../prepareHybrid.py .
		ln -ns ../../prepareGOF.py .
		ln -ns ../../prepareAsymptotic.py .
		python prepareAsymptotic.py -m ${i} -n ${NAMESAMPLE}$i
#         if [ $i == "21" ]
#            then
#            for g in {0..20}
#            do
#               python prepareGOF.py -n $g 
#           done
#       fi
		cd ${CF}
    done


done



