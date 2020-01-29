#!/bin/bash
# make cards with all vars/selections

#export OUTSTRING="2017_02_19_btag_$1"
export OUTSTRING="2019_10_11_VBFsig_1Dcount"


#export SELECTIONS="s2b0jresolvedMcut s1b1jresolvedMcut sboostedLLMcut VBFtight"
export SELECTIONS="sboostedLLMcut s2b0jresolvedMcut s1b1jresolvedMcut VBFloose VBFtight"
#export SELECTIONS="s2b0jresolvedMcut s1b1jresolvedMcut sboostedLLMcut VBFloose VBFtight"
#export SELECTIONS="s2b0jresolvedMcut s1b1jresolvedMcut sboostedLLMcut"
export NAMESAMPLE="VBFHH_bbtt"
#"ggHH_bbtt"
export RESONANT=$2
#export LEPTONS=$1
export LEPTONS="ETau MuTau TauTau"

#c2Vvar=(-20.0 -15.0 -10.0 -4.0 -3.0 -2.0 -1.5 -1.0 0.5 0.0 0.5 1.0 1.5 2.0 3.0 4.0 5.0 10.0 15.0 20.0)

#c2Vvar=(1.0)
#c2Vvar=(1.0)
#cVvar=(1.0)

#cVvar=(-8.0 -6.0 -4.0 -2.0 -1.0 0.0 1.0 2.0 3.0 4.0 6.0 8.0 10.0)
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
    else
        export VARIABLE="HHKin_mass_raw"
        export NAMESAMPLE="Radion"

        export LAMBDAS="250 270 280 300 350 400 450 500 550 600 650 750 900"
fi

export QUANTILES="0.025 0.16 0.5 0.84 0.975 -1.0"
export SOURCE="/home/llr/cms/cadamuro/testAnalysisHelper2/CMSSW_7_4_7/src/KLUBAnalysis" 

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
 	        #combineCards.py -S hh_${chanNum}_*L${i}_13TeV.txt >> comb.txt 
            combineCards.py -S hh_*.txt >> comb.txt 
            #combineCards.py hh_${chanNum}_C1_L${i}_13TeV.txt hh_${chanNum}_C2_L${i}_13TeV.txt hh_${chanNum}_C3_L${i}_13TeV.txt >> comb.txt
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
         text2workspace.py -m ${j} comb.txt -o comb.root ;
         ln -ns ../../prepareHybrid.py .
         ln -ns ../../prepareGOF.py .
         ln -ns ../../prepareAsymptotic.py .
         python prepareAsymptotic.py -m ${j} -n ${NAMESAMPLE}$i
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



