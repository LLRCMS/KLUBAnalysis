#!/bin/bash
# make cards with all vars/selections

#Usage: makeCategories.sh [bdtsuffix] [cards/MuTau/ETau/TauTau/combined] [-r] 
export RESONANT=$3
export LEPTONS=$2
export OUTSTRING="2017_03_07_2D_$1"

export STRINGLEPTONS="$1"
export SELECTIONS="s2b0jresolvedMcut${STRINGLEPTONS} s1b1jresolvedMcut${STRINGLEPTONS} sboostedLLMcut"
#export SELECTIONS="s2b0jresolvedMcut s1b1jresolvedMcut sboostedLLMcut"
export NAMESAMPLE="ggHH_bbtt"
#"ggHH_bbtt"
export PART="$RESONANT"
export CF="$CMSSW_BASE/src/KLUBAnalysis/combiner"
if [ "${RESONANT}" != "-r" ]
    then
    export VARIABLE="MT2"
        export LAMBDAS=""
        #export INSELECTIONS="s2b0jresolvedMcutlmr90 s1b1jresolvedMcutlmr90 s2b0jresolvedMcuthmr90 s1b1jresolvedMcuthmr90 sboostedLLMcut s1b1jresolvedMcutlmr70 s2b0jresolvedMcutlmr70 s1b1jresolvedMcutLepTauKine s2b0jresolvedMcutLepTauKine"
        if [ "${RESONANT}" == "0" ]
            then
            for il in {0..499}
            do 
            export LAMBDAS="$LAMBDAS ${il}"
            export RESONANT=""
            done
        fi
        if [ "${RESONANT}" == "1" ]
            then
            for il in {500..999}
            do 
            export LAMBDAS="$LAMBDAS ${il}"            
            export RESONANT=""
            done
        fi
        if [ "${RESONANT}" == "2" ]
            then
            for il in {1000..1499}
            do 
            export LAMBDAS="$LAMBDAS ${il}"
            export RESONANT=""
            done
        fi
        if [ "${RESONANT}" == "3" ]
            then
            for il in {1500..1999}
            do 
            export LAMBDAS="$LAMBDAS ${il}"
            export RESONANT=""
            done
        fi
        if [ "${RESONANT}" == "4" ]
            then
            for il in {2000..2499}
            do 
            export LAMBDAS="$LAMBDAS ${il}"
            export RESONANT=""
            done
        fi
        if [ "${RESONANT}" == "5" ]
            then
            for il in {2500..2672}
            do 
            export LAMBDAS="$LAMBDAS ${il}"
            export RESONANT=""
            done
        fi

    else
        export VARIABLE="HHKin_mass_raw"
        export NAMESAMPLE="Radion"
        export LAMBDAS="250 270 280 300 350 400 450 500 550 600 650 750 900"
fi
#export LAMBDAS="21"
#echo "LAMBDAS ======================"
#echo $LAMBDAS
export QUANTILES="0.025 0.16 0.5 0.84 0.975 -1.0"
export SOURCE="/home/llr/cms/cadamuro/testAnalysisHelper2/CMSSW_7_4_7/src/KLUBAnalysis" 

#create all the cards 3 categories x 3 channels
if [ "${LEPTONS}" == "cards" ]
    then
for ibase in $SELECTIONS
do
    for c in ETau MuTau TauTau
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
    echo "python chcardMaker.py -f analyzedOutPlotter_07_03_2D_${c}_part${PART}.root -o _${OUTSTRING} -c ${c} -i ${SOURCE}/analysis_${c}_7Mar_lims_2dscan_part${PART}/mainCfg_${c}_part${PART}.cfg -y -s ${BASE} ${RESONANT} -u 1 -t"
    python chcardMaker.py -f analyzedOutPlotter_07_03_2D_${c}_part${PART}.root -o "_${OUTSTRING}" -c ${c} -i ${SOURCE}/analysis_${c}_7Mar_lims_2dscan_part${PART}/mainCfg_${c}_part${PART}.cfg -y -s ${BASE} ${RESONANT} -u 1 -t 
    #python chcardMaker.py -f analyzedOutPlotter_07_03_${c}_part${RESONANT}.root -o "_${OUTSTRING}" -c ${c} -i ${SOURCE}//analysis_${c}_7Mar_lims_2dscan_part${RESONANT}/mainCfg_${c}.cfg -y -s ${BASE} -u 1 -t 
#done
#for base in $SELECTIONSTAU
#do
    #python cardMaker.py -i ${SOURCE}/analysis_TauTau_26Gen/mainCfg_TauTau.cfg   -f ${SOURCE}/analysis_TauTau_26Gen/analyzedOutPlotter.root -o $BASE -c TauTau --dir "_$OUTSTRING" -t 0 # -r #-S /home/llr/cms/cadamuro/FittedHHKinFit/outPlotter_fit_sigs_TauTau.root
    done
done
echo " "
echo "CREATED ALL CARDS"
fi

if [ "${LEPTONS}" == "combined" ]
then

#CATEGORY COMBINATION
for i in $LAMBDAS
do
    cd ${CF}
    mkdir -p cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${VARIABLE}
####     #MAKE COMBINATION FOR CATEGORY [3 x mass point]
    for ibase in $SELECTIONS
    do
####         mkdir -p cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${ibase}${VARIABLE}
        for c in MuTau ETau TauTau
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
            #mkdir -p cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${VARIABLE}
            #cp cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${BASE}${VARIABLE}/hh_*.* cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${ibase}${VARIABLE}/.
            cp cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${BASE}${VARIABLE}/hh_*.* cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${VARIABLE}/.
            #cp cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${BASE}${VARIABLE}/hh_*.* cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${VARIABLE}/.
        done
####         cd cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${ibase}${VARIABLE}
####         pwd
####         if [ -a "hh_1_C1_L${NAMESAMPLE}${i}_13TeV.txt" ] #category 1
####             then
####             combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt  >> comb.txt
####         fi
####         if [ -a "hh_1_C2_L${NAMESAMPLE}${i}_13TeV.txt" ]
####             then
####             combineCards.py -S hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt >> comb.txt
####         fi
####         if [ -a "hh_1_C3_L${NAMESAMPLE}${i}_13TeV.txt" ]
####             then
####             combineCards.py -S hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt >> comb.txt
####         fi
#### #        if [ -a "hh_1_C999_L${i}_13TeV.txt" ]
#### #           then
#### #           combineCards.py -S hh_1_C999_L${i}_13TeV.txt hh_2_C999_L${i}_13TeV.txt hh_3_C999_L${i}_13TeV.txt >> comb.txt
#### #        fi
####          text2workspace.py -m ${i} comb.txt -o comb.root ;
####          ln -ns ../../prepareHybrid.py .
####          ln -ns ../../prepareGOF.py .
####          ln -ns ../../prepareAsymptotic.py .
####          python prepareAsymptotic.py -m ${i} -n ${NAMESAMPLE}$i
#### #         if [ $i == "21" ]
#### #            then
#### #            for g in {0..20}
#### #            do
#### #               python prepareGOF.py -n $g 
#### #           done
#### #       fi
####          #combine -M HybridNew --frequentist -m 125.0 --testStat LHC comb.root  -H ProfileLikelihood -n ${i}_forLim --expectedFromGrid=0.5
####          #for q in $QUANTILES
####          #do
####         #python prepareHybrid.py -n $q
####         #done
####         cd ${CF}
    done

    #MAKE BIG COMBINATION [1 x mass point]
    cd ${CF}
    cd cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${VARIABLE}
    echo "cards_Combined_$OUTSTRING/${NAMESAMPLE}${i}${VARIABLE}"
    pwd
    rm comb.*
    combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt >> comb.txt #ah ma allora le wildcard funzionano?
    #combineCards.py -S hh_*_C2_L${i}_13TeV.txt hh_*_C3_L${i}_13TeV.txt >> comb.txt
    text2workspace.py -m ${i} comb.txt -o comb.root ;
    ln -ns ../../prepareHybrid.py .
    ln -ns ../../prepareGOF.py .
    ln -ns ../../prepareAsymptotic.py .
    ln -ns ../../prepareImpacts.py .
    python prepareAsymptotic.py -m ${i} -n ${NAMESAMPLE}$i
#    if [ $i == "21" ]
#       then
##      ln -ns ../../prepareImpacts.py .
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
####     for c in "MuTau ETau TauTau"
####     do
#### #       for base in $SELECTIONS
#### #       do
#### #           cp cards_${c}_$OUTSTRING/${i}${base}${VARIABLE}/hh_*C*.* cards_${c}_$OUTSTRING/${i}${VARIABLE}/.
#### #       done
####         cd cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${VARIABLE}
####         rm comb.*
####         combineCards.py -S hh_*_C1_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C2_L${NAMESAMPLE}${i}_13Te*.txt hh_*_C3_L${NAMESAMPLE}${i}_13Te*.txt >> comb.txt
####         #combineCards.py -S  hh_*_C2_L${i}_13TeV.txt hh_*_C3_L${i}_13TeV.txt >> comb.txt
####         text2workspace.py -m ${i} comb.txt -o comb.root ;
####         ln -ns ../../prepareHybrid.py .
####         ln -ns ../../prepareGOF.py .
####         ln -ns ../../prepareAsymptotic.py .
####         python prepareAsymptotic.py -m ${i} -n ${NAMESAMPLE}$i
#### #         if [ $i == "21" ]
#### #            then
#### #            for g in {0..20}
#### #            do
#### #               python prepareGOF.py -n $g 
#### #           done
#### #       fi
####         cd ${CF}
####     done


done


#### else
#### for i in $LAMBDAS
#### do 
#### #MAKE LIMIT FOR individual CHANNEL/categories [9 x mass points]
####     for ibase in $SELECTIONS
####     do  
####         for c in $LEPTONS
####         do
####             #if [ -a "hh_3_L${i}_13TeV.txt" ]
####             #   then
####             #   combineCards.py -S "hh_3_L${i}_13TeV.txt" >> comb.txt 
####             #fi
####             if [ "${c}" == "MuTau" ]
####             then
####                 export chanNum="2"
####                 echo "${c} ${chanNum}"
####                 export BASE="$ibase"
####             fi
####             if [ "${c}" == "ETau" ]
####             then
####                 export chanNum="1"
####                 echo "${c} ${chanNum}" 
####                 export BASE="$ibase"
####             fi
####             if [ "${c}" == "TauTau" ]
####             then
####                 export chanNum="3"
####                 echo "${c} ${chanNum}" 
####                 export BASE=${ibase/${STRINGLEPTONS}/}
####             fi
####             cd ${CF}/cards_${c}_$OUTSTRING/${NAMESAMPLE}${i}${BASE}${VARIABLE}
####             pwd
####             #combineCards.py -S hh_${chanNum}_*L${i}_13TeV.txt >> comb.txt 
####             combineCards.py -S hh_*.txt >> comb.txt 
####             #combineCards.py hh_${chanNum}_C1_L${i}_13TeV.txt hh_${chanNum}_C2_L${i}_13TeV.txt hh_${chanNum}_C3_L${i}_13TeV.txt >> comb.txt
####             text2workspace.py -m ${i} comb.txt -o comb.root ;
####             ln -ns ../../prepareHybrid.py .
####             ln -ns ../../prepareGOF.py .
####             ln -ns ../../prepareAsymptotic.py .
####             python prepareAsymptotic.py -m ${i} -n ${NAMESAMPLE}$i
####             cd ${CF}
####         done
####     done
#### done
fi



