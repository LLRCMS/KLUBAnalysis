#!/bin/bash
# make cards with all vars/selections

#export OUTSTRING="2017_02_19_btag_$1"
export OUTSTRING="2019_10_11_VBFsig_1Dcount"
export SELECTIONS="sboostedLLMcut s2b0jresolvedMcut s1b1jresolvedMcut VBFloose VBFtight"
export NAMESAMPLE="VBFHH_bbtt"

export RESONANT=$2

export LEPTONS="ETau MuTau TauTau"


cVvar=(1.0)
c2Vvar=(-4.0 -3.5 -3.0 -2.5 -2.0 -1.5 -1.0 -0.5 0.0 0.2 0.4 0.6 0.8 1.0 1.2 1.4 1.6 1.8 2.0 2.5 3.0 3.5 4.0 4.5 5.0)

export CF="$CMSSW_BASE/src/KLUBAnalysis/combiner2017"
if [ "${RESONANT}" != "-r" ]
    then
    export VARIABLE=""
        export LAMBDAS=""
	export VARIABLES=""
	export C2V=""

        for ik in ${cVvar[@]}
	do
	    for il in ${c2Vvar[@]}
            do 
        export C2V="${C2V} _CV_${ik}_C2V_${il}_C3_1.0"
	echo "<<<<<<<<<<<<<<"$C2V
	    done
	done
	export VARIABLE="BDToutSM_kl_1" 
fi

export QUANTILES="0.025 0.16 0.5 0.84 0.975 -1.0"

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
	python chcardMakerVBF.py -f analyzedOutPlotter_2019_10_11_VBFsig_DNN_${c}_count.root -o "_${OUTSTRING}" -c ${c} -y -s ${BASE} ${RESONANT} -u 1 -t 

    done
done
echo " "
echo "CREATED ALL CARDS"
j=1
echo $C2V

for i in $C2V
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
	    echo ${CF}/cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${BASE}${VARIABLE}
            cd ${CF}/cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${BASE}${VARIABLE}
            pwd
 	    
            combineCards.py -S hh_*.txt >> comb.txt 
            
            text2workspace.py -m ${j} comb.txt -o comb.root ;
            ln -ns ../../prepareHybrid.py .
            ln -ns ../../prepareGOF.py .
            ln -ns ../../prepareAsymptotic.py .
            python prepareAsymptotic.py -m ${j} -n ${NAMESAMPLE}$i
            cd ${CF}
        done
    done
((j++))
done
echo "CATEGORY COMBINATION"
j=1
#CATEGORY COMBINATION
for i in $C2V
do
    cd ${CF}
	mkdir -p cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${VARIABLE}
	pwd
    #MAKE COMBINATION FOR CATEGORY [3 x mass point]
    for ibase in $SELECTIONS
    do
        mkdir -p cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${ibase}${VARIABLE}
        for c in $LEPTONS
        do
             export BASE="$ibase"
	     if [[ "${c}" != "TauTau" && "${ibase}" == *"VBFtight"* ]]
	     then
		 continue
	     fi
            mkdir -p cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${VARIABLE}
            cp cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${BASE}${VARIABLE}/hh_*.* cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${ibase}${VARIABLE}/.
            cp cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${BASE}${VARIABLE}/hh_*.* cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${VARIABLE}/.
            cp cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${BASE}${VARIABLE}/hh_*.* cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${VARIABLE}/.
        done
        cd cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${ibase}${VARIABLE}
        pwd
	# CATEGORIES:
	# C1 = s1b1jresolvedMcut
	# C2 = s2b0jresolvedMcut
	# C3 = sboostedLLMcut
	# C4 = VBFloose
	# C5 = VBFtight
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

         text2workspace.py -m ${j} comb.txt -o comb.root ;
         ln -ns ../../prepareHybrid.py .
         ln -ns ../../prepareGOF.py .
         ln -ns ../../prepareAsymptotic.py .
         python prepareAsymptotic.py -m ${j} -n ${NAMESAMPLE}$i
        cd ${CF}
    done

    #MAKE BIG COMBINATION [1 x mass point]
    cd cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${VARIABLE}
    rm comb.*
    
    combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C4_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C5_L${NAMESAMPLE}${i}_13Te*.txt>> comb.txt 
    text2workspace.py -m ${j} comb.txt -o comb.root ;
    ln -ns ../../prepareHybrid.py .
    ln -ns ../../prepareGOF.py .
    ln -ns ../../prepareAsymptotic.py .
    ln -ns ../../prepareImpacts.py .
    python prepareAsymptotic.py -m ${j} -n ${NAMESAMPLE}$i
    
    cd ${CF}

    #MAKE COMBINATION FOR CHANNEL [3 x mass point]
    for c in $LEPTONS 
    do
	
	cd cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${VARIABLE}
	rm comb.*
	
	if [[ ${c} == "TauTau" ]]
	then
	    combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C4_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C5_L${NAMESAMPLE}${i}_13Te*.txt>> comb.txt
	else
	    combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C4_L${NAMESAMPLE}${i}_13Te*.txt >> comb.txt
	fi
	text2workspace.py -m ${j} comb.txt -o comb.root ;
	ln -ns ../../prepareHybrid.py .
	ln -ns ../../prepareGOF.py .
	ln -ns ../../prepareAsymptotic.py .
	python prepareAsymptotic.py -m ${j} -n ${NAMESAMPLE}$i
	
	cd ${CF}
    done
    
    ((j++))
    
done



