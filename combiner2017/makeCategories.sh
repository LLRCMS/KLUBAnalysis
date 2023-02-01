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

#klvar=(-20 -15 -10 -8 -6 -4 -3 -2 -1 0.001 1 2 3 2.45 4 5 6 7 8 9 10 11 12 14 16 18 20 25 30)
klvar=(1)


export CF="$CMSSW_BASE/src/KLUBAnalysis/combiner2017"
if [ "${RESONANT}" != "-r" ]
    then
    export VARIABLE=""
    export LAMBDAS=""
    export VARIABLES=""
    #for il in {1..29}
    #do 
    #    export LAMBDAS="$LAMBDAS ${il}"
    #	 export VARIABLES="${VARIABLES} BDToutSM_kl_${klvar[$((il-1))]}" 
    #done
    export LAMBDAS="11"
fi

export QUANTILES="0.025 0.16 0.5 0.84 0.975 -1.0"

echo $VARIABLES
arrVARIABLES=($VARIABLES)

#create all the cards 3 categories x 3 channels
for ibase in $SELECTIONS
do
    for c in $LEPTONS
    do
        export BASE="$ibase"
        
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
            combineCards.py -S hh_*.txt >> comb.txt 
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
	# CATEGORIES: 
	# C1 = s1b1jresolvedMcut
	# C2 = s2b0jresolvedMcut
	# C3 = sboostedLLMcut
	# C4 = VBFloose
	# C5 = VBFtight

        if [ -a "hh_3_C1_L${NAMESAMPLE}${i}_13TeV.txt" ] 
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

         text2workspace.py -m ${i} comb.txt -o comb.root ;
         ln -ns ../../prepareHybrid.py .
         ln -ns ../../prepareGOF.py .
         ln -ns ../../prepareAsymptotic.py .
         python prepareAsymptotic.py -m ${i} -n ${NAMESAMPLE}$i

        cd ${CF}
    done

	#MAKE BIG COMBINATION [1 x mass point]
    cd cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${arrVARIABLES[$((i-1))]}
    rm comb.*
    combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C4_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C5_L${NAMESAMPLE}${i}_13Te*.txt>> comb.txt 
    text2workspace.py -m ${i} comb.txt -o comb.root ;
    ln -ns ../../prepareHybrid.py .
    ln -ns ../../prepareGOF.py .
    ln -ns ../../prepareAsymptotic.py .
    ln -ns ../../prepareImpacts.py .
    python prepareAsymptotic.py -m ${i} -n ${NAMESAMPLE}$i
    cd ${CF}

    #MAKE COMBINATION FOR CHANNEL [3 x mass point]
    for c in $LEPTONS 
    do
	
	cd cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${arrVARIABLES[$((i-1))]}
	rm comb.*
	if [[ ${c} == "TauTau" ]]
	then
	    combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C4_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C5_L${NAMESAMPLE}${i}_13Te*.txt>> comb.txt
	else
	    combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C4_L${NAMESAMPLE}${i}_13Te*.txt>> comb.txt
	fi
	
	text2workspace.py -m ${i} comb.txt -o comb.root ;
	ln -ns ../../prepareHybrid.py .
	ln -ns ../../prepareGOF.py .
	ln -ns ../../prepareAsymptotic.py .
	python prepareAsymptotic.py -m ${i} -n ${NAMESAMPLE}$i

		cd ${CF}
    done


done



