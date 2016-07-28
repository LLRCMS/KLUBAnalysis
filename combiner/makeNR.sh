#!/bin/bash
# make cards with all vars/selections

export OUTSTRING="2016_07_26"


#export SELECTIONS="resolved_2b0j_Mcut resolved_1b1j_Mcut boosted_Mcut defaultBtagMMNoIsoBBTTCut"
#export SELECTIONSTAU="resolved_2b0j_Mcut resolved_1b1j_Mcut boosted_Mcut  defaultBtagLLNoIsoBBTTCut45"
export SELECTIONS="defaultBtagLLNoIsoBBTTCutKine80"
export SELECTIONSTAU="defaultBtagLLNoIsoBBTTCut45"

export VARTAUTAU="HH_mass_raw"
export VARMUTAU="HH_mass_raw"
export VARETAU="HH_mass_raw"

#export LAMBDAS="Lambdam4 Lambda1 Lambda2dot46 Lambda20"
#export LAMBDAS="HHNode10 HHNode11 HHNode12 HHNode13 HHNode2  HHNode3  HHNode4  HHNode5  HHNode6  HHNode7  HHNode8  HHNode9  HHNodebox HHNodeSM"
#export LAMBDAS="HHrewclus1 HHrewclus2 HHrewclus3 HHrewclus4 HHrewclus5 HHrewclus6 HHrewclus7 HHrewclus8 HHrewclus9 HHrewclus10 HHrewclus11 HHrewclus0 HHrewoutlier1clus1 HHrewoutlier2clus1 HHrewoutlier3clus1 HHrewoutlier4clus1 HHrewoutlier5clus1 HHrewoutlier0clus1 HHrewoutlier1clus10 HHrewoutlier2clus10 HHrewoutlier3clus10 HHrewoutlier4clus10 HHrewoutlier5clus10 HHrewoutlier0clus10"
#export LAMBDAS="HHlambdarew21"
export LAMBDAS="HH5Dplane0 HH5Dplane1 HH5Dplane2 HH5Dplane3 HH5Dplane4 HH5Dplane5 HH5Dplane6 HH5Dplane7 HH5Dplane8 HH5Dplane13 HH5Dplane21 HH5Dplane30 HH5Dplane39 HH5Dplane48 HH5Dplane57 HH5Dplane65 HH5Dplane74 HH5Dplane83 HH5Dplane99 HH5Dplane107 HH5Dplane116 HH5Dplane125 HH5Dplane134 HH5Dplane143 HH5Dplane148 HH5Dplane149 HH5Dplane153 HH5Dplane158 HH5Dplane163 HH5Dplane172 HH5Dplane177 HH5Dplane182 HH5Dplane188 HH5Dplane193 HH5Dplane194 HH5Dplane196 HH5Dplane197 HH5Dplane199 HH5Dplane205 HH5Dplane210 HH5Dplane212 HH5Dplane214 HH5Dplane215 HH5Dplane216 HH5Dplane260 HH5Dplane268 HH5Dplane277 HH5Dplane286 HH5Dplane295 HH5Dplane304 HH5Dplane309 HH5Dplane310 HH5Dplane314 HH5Dplane319 HH5Dplane324 HH5Dplane333 HH5Dplane338 HH5Dplane343 HH5Dplane352 HH5Dplane360 HH5Dplane369 HH5Dplane378 HH5Dplane387 HH5Dplane392 HH5Dplane393 HH5Dplane394 HH5Dplane395 HH5Dplane400 HH5Dplane408 HH5Dplane417 HH5Dplane426 HH5Dplane435 HH5Dplane444 HH5Dplane452 HH5Dplane461 HH5Dplane470 HH5Dplane479 HH5Dplane812 HH5Dplane817 HH5Dplane818 HH5Dplane822 HH5Dplane827 HH5Dplane832 HH5Dplane841 HH5Dplane846 HH5Dplane850 HH5Dplane856 HH5Dplane859 HH5Dplane862 HH5Dplane866 HH5Dplane868 HH5Dplane872 HH5Dplane876 HH5Dplane881 HH5Dplane885 HH5Dplane887 HH5Dplane888 HH5Dplane893 HH5Dplane898 HH5Dplane899 HH5Dplane904 HH5Dplane905 HH5Dplane906 HH5Dplane911 HH5Dplane955 HH5Dplane960 HH5Dplane961 HH5Dplane965 HH5Dplane970 HH5Dplane975 HH5Dplane984 HH5Dplane989 HH5Dplane994 HH5Dplane1086 HH5Dplane1088 HH5Dplane1089 HH5Dplane1090 HH5Dplane1101 HH5Dplane1114 HH5Dplane1125 HH5Dplane1169 HH5Dplane1432"
for il in {0..51}
do 
export LAMBDAS="$LAMBDAS HHlambdarew${il}"
done
for il in {0..11}
do 
export LAMBDAS="$LAMBDAS HHbenchrew${il}"
done
export QUANTILES="0.025 0.16 0.5 0.84 0.975 -1.0"

export SOURCE="/home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis" 
export FOLDER="evalQCD_Plots_2016_nonResonant_20Lug2016_lessLooseQCD_6fb"
 for base in $SELECTIONS
 do
 echo "python cardMaker.py -i ${SOURCE}/analysis_ETau/${FOLDER}/analysis_ETau.cfg     -f ${SOURCE}/analysis_ETau/${FOLDER}/outPlotter2.root   -o $base -v $VARETAU   -c ETau   --dir "_$OUTSTRING" -t 0 -r 0 "
 python cardMaker.py -i ${SOURCE}/analysis_ETau/${FOLDER}/analysis_ETau.cfg     -f ${SOURCE}/analysis_ETau/${FOLDER}/outPlotter2.root   -o $base -v $VARETAU   -c ETau   --dir "_$OUTSTRING" -t 0 -r 0 #-S /home/llr/cms/cadamuro/FittedHHKinFit/outPlotter_fit_sigs_ETau.root
 python cardMaker.py -i ${SOURCE}/analysis_MuTau/${FOLDER}/analysis_MuTau.cfg   -f ${SOURCE}/analysis_MuTau/${FOLDER}/outPlotter2.root  -o $base -v $VARMUTAU  -c MuTau  --dir "_$OUTSTRING" -t 0 -r 0 #-S /home/llr/cms/cadamuro/FittedHHKinFit/outPlotter_fit_sigs_MuTau.root
 done
 for base in $SELECTIONSTAU
 do
 python cardMaker.py -i ${SOURCE}/analysis_TauTau/${FOLDER}/analysis_TauTau.cfg -f ${SOURCE}/analysis_TauTau/${FOLDER}/outPlotter.root -o $base -v $VARTAUTAU -c TauTau --dir "_$OUTSTRING" -t 0 -r 0 #-S /home/llr/cms/cadamuro/FittedHHKinFit/outPlotter_fit_sigs_TauTau.root
 done
 for i in $LAMBDAS
 	do 
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
 	    combineCards.py -S hh_2_*L${i}_13TeV.txt >> comb.txt 
 	    text2workspace.py comb.txt -o comb.root ;
 	    #combine -M Asymptotic comb.root -m 125.0 -n ${i}_forLim
 	    ln -ns ../../prepareHybrid.py .
 	    ln -ns ../../prepareGOF.py .
 	    ln -ns ../../prepareAsymptotic.py .
 	    python prepareAsymptotic.py -n $i
#         for g in {0..60}
#         do
#           python prepareGOF.py -n $g
#       done

 	    #for q in $QUANTILES
 	    #do
 	    #	python prepareHybrid.py -n $q
 	    #done
 	    cd -
 	    cd cards_ETau_$OUTSTRING/${i}${base}${VARETAU}
 	    pwd
 	    combineCards.py -S hh_1_*L${i}_13TeV.txt >> comb.txt 
 	    text2workspace.py comb.txt -o comb.root ;
 	    ln -ns ../../prepareHybrid.py .
 	    ln -ns ../../prepareGOF.py .
 	    ln -ns ../../prepareAsymptotic.py .
 	    python prepareAsymptotic.py -n $i
#       for g in {0..60}
#       do
#           python prepareGOF.py -n $g
#       done
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
 	    combineCards.py -S hh_3_*L${i}_13TeV.txt >> comb.txt 
 	    text2workspace.py comb.txt -o comb.root ;
 	    ln -ns ../../prepareHybrid.py .
 	    ln -ns ../../prepareGOF.py .
 	    ln -ns ../../prepareAsymptotic.py .
     	python prepareAsymptotic.py -n $i
#       for g in {0..60}
#       do
#           python prepareGOF.py -n $g
#       done

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

 #
 #MIXED CUT COMBINATION
 for i in $LAMBDAS
 do
         mkdir -p cards_Combined_$OUTSTRING/${i}COMBINED
         cp cards_MuTau_$OUTSTRING/${i}${SELECTIONS}${VARMUTAU}/hh_*.* cards_Combined_$OUTSTRING/${i}COMBINED/.
         cp cards_TauTau_$OUTSTRING/${i}${SELECTIONSTAU}${VARTAUTAU}/hh_*.* cards_Combined_$OUTSTRING/${i}COMBINED/.
         cp cards_ETau_$OUTSTRING/${i}${SELECTIONS}${VARETAU}/hh_*.* cards_Combined_$OUTSTRING/${i}COMBINED/.
         cd cards_Combined_$OUTSTRING/${i}COMBINED
         pwd
         combineCards.py -S hh_3*_L${i}_13TeV.txt hh_2*_L${i}_13TeV.txt hh_1*_L${i}_13TeV.txt >> comb.txt
         text2workspace.py comb.txt -o comb.root ;
         #combine -M Asymptotic comb.root -m 125.0 -t -1 --expectSignal=1 -n ${i} -S 0
         #combine -M Asymptotic comb.root -m 125.0 -n ${i}_forLim
         ln -ns ../../prepareHybrid.py .
         ln -ns ../../prepareGOF.py .
         ln -ns ../../prepareAsymptotic.py .
         python prepareAsymptotic.py -n $i
         if [ $i == "HHlambdarew21" ]
            then
            ln -ns ../../prepareImpacts.py .
            python prepareImpacts.py -n $i
        fi

        for il in {0..11}
        do 
            if [ $i == "HHbenchrew${il}" ]
                then
                for q in $QUANTILES
                do
                   python prepareHybrid.py -n $q
               done
           fi    
       done

#         for g in {0..60}
#         do
#             python prepareGOF.py -n $g
#         done

         #combine -M HybridNew --frequentist -m 125.0 --testStat LHC comb.root  -H ProfileLikelihood -n ${i}_forLim --expectedFromGrid=0.5
 	# if [ $i == "Radion450" ]
 	# 	then
 	# 	for g in {0..100}
 	# 		do
 	# 			python prepareGOF.py -n $g
 	# 		done
 	# fi
         cd -
         done















#### #CATEGORY COMBINATION
#### for i in $LAMBDAS
#### do
####  	mkdir -p cards_COMBINED_$OUTSTRING/${i}
####  	for base in $SELECTIONS
####  	do
####          mkdir -p cards_COMBINED_$OUTSTRING/${i}${base}${VARMUTAU}
####          cp cards_MuTau_$OUTSTRING/${i}${base}${VARMUTAU}/hh_*.* cards_COMBINED_$OUTSTRING/${i}${base}${VARMUTAU}/.
####          cp cards_MuTau_$OUTSTRING/${i}${base}${VARMUTAU}/hh_*.* cards_COMBINED_$OUTSTRING/${i}/.
####          if [ $base  == "defaultBtagMMNoIsoBBTTCut" ]
####          	then 
####          	cp cards_TauTau_$OUTSTRING/${i}"defaultBtagLLNoIsoBBTTCut45"${VARTAUTAU}/hh_*.* cards_COMBINED_$OUTSTRING/${i}${base}${VARMUTAU}/.
####          	cp cards_TauTau_$OUTSTRING/${i}"defaultBtagLLNoIsoBBTTCut45"${VARTAUTAU}/hh_*.* cards_COMBINED_$OUTSTRING/${i}/.
####          	cd cards_TauTau_$OUTSTRING
####          	ln -ns ${i}"defaultBtagLLNoIsoBBTTCut45"${VARTAUTAU} ${i}"defaultBtagMMNoIsoBBTTCut"${VARTAUTAU}
####          	cd -
####          else 
####          	cp cards_TauTau_$OUTSTRING/${i}${base}${VARTAUTAU}/hh_*.* cards_COMBINED_$OUTSTRING/${i}${base}${VARMUTAU}/.
####          	cp cards_TauTau_$OUTSTRING/${i}${base}${VARTAUTAU}/hh_*.* cards_COMBINED_$OUTSTRING/${i}/.
####          fi
####          cp cards_ETau_$OUTSTRING/${i}${base}${VARETAU}/hh_*.* cards_COMBINED_$OUTSTRING/${i}${base}${VARMUTAU}/.
####          cp cards_ETau_$OUTSTRING/${i}${base}${VARETAU}/hh_*.* cards_COMBINED_$OUTSTRING/${i}/.
####          cd cards_COMBINED_$OUTSTRING/${i}${base}${VARMUTAU}
####          pwd
####          if [ -a "hh_1_C1_L${i}_13TeV.txt" ]
####          	then
####          	combineCards.py -S hh_1_C1_L${i}_13TeV.txt hh_2_C1_L${i}_13TeV.txt hh_3_C1_L${i}_13TeV.txt >> comb.txt
####          fi
####          if [ -a "hh_1_C2_L${i}_13TeV.txt" ]
####          	then
####          	combineCards.py -S hh_1_C2_L${i}_13TeV.txt hh_2_C2_L${i}_13TeV.txt hh_3_C2_L${i}_13TeV.txt >> comb.txt
####          fi
####          if [ -a "hh_1_C3_L${i}_13TeV.txt" ]
####          	then
####          	combineCards.py -S hh_1_C3_L${i}_13TeV.txt hh_2_C3_L${i}_13TeV.txt hh_3_C3_L${i}_13TeV.txt >> comb.txt
####          fi
####          if [ -a "hh_1_C999_L${i}_13TeV.txt" ]
####          	then
####          	combineCards.py -S hh_1_C999_L${i}_13TeV.txt hh_2_C999_L${i}_13TeV.txt hh_3_C999_L${i}_13TeV.txt >> comb.txt
####          fi
####          text2workspace.py comb.txt -o comb.root ;
####          #combine -M Asymptotic comb.root -m 125.0 -t -1 --expectSignal=1 -n ${i} -S 0
####          #combine -M Asymptotic comb.root -m 125.0 -n ${i}_forLim
####          ln -ns ../../prepareHybrid.py .
####          ln -ns ../../prepareGOF.py .
####          ln -ns ../../prepareAsymptotic.py .
####          python prepareAsymptotic.py -n $i
####          #combine -M HybridNew --frequentist -m 125.0 --testStat LHC comb.root  -H ProfileLikelihood -n ${i}_forLim --expectedFromGrid=0.5
####          #for q in $QUANTILES
####          #do
####  		#python prepareHybrid.py -n $q
####  	    #done
####  	# if [ $i == "Radion450" ]
####  	# 	then
####  	# 	for g in {0..100}
####  	# 		do
####  	# 			python prepareGOF.py -n $g
####  	# 		done
####  	# fi
####          cd -
####          done
####     cd cards_COMBINED_$OUTSTRING/${i}
####     rm comb.*
####     combineCards.py -S hh_*_C1_L${i}_13TeV.txt hh_*_C2_L${i}_13TeV.txt hh_*_C3_L${i}_13TeV.txt >> comb.txt
####     #combineCards.py -S hh_*_C2_L${i}_13TeV.txt hh_*_C3_L${i}_13TeV.txt >> comb.txt
####     text2workspace.py comb.txt -o comb.root ;
####     ln -ns ../../prepareHybrid.py .
####     ln -ns ../../prepareGOF.py .
####     ln -ns ../../prepareAsymptotic.py .
####     python prepareAsymptotic.py -n $i
####     cd -
#### 	for c in MuTau ETau TauTau 
#### 	do
#### 		mkdir -p cards_${c}_$OUTSTRING/${i}
#### 		for base in $SELECTIONS
#### 		do
#### 			cp cards_${c}_$OUTSTRING/${i}${base}${VARMUTAU}/hh_*C*.* cards_${c}_$OUTSTRING/${i}/.
#### 		done
#### 		cd cards_${c}_$OUTSTRING/${i}
#### 		rm comb.*
#### 		combineCards.py -S hh_*_C1_L${i}_13TeV.txt hh_*_C2_L${i}_13TeV.txt hh_*_C3_L${i}_13TeV.txt >> comb.txt
#### 		#combineCards.py -S  hh_*_C2_L${i}_13TeV.txt hh_*_C3_L${i}_13TeV.txt >> comb.txt
#### 		text2workspace.py comb.txt -o comb.root ;
#### 		ln -ns ../../prepareHybrid.py .
#### 		ln -ns ../../prepareGOF.py .
#### 		ln -ns ../../prepareAsymptotic.py .
#### 		python prepareAsymptotic.py -n $i
#### 		cd -
####     done
#### done
#### 



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

