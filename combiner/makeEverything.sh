#!/bin/bash
# make cards with all vars/selections

export OUTSTRING="09Dec_ter"
export BASEMUTAU="defaultBtagLLNoIsoBBTTCutKine"
export BASETAUTAU="defaultBtagLLNoIsoBBTTCut"
export BASEETAU="defaultBtagMMNoIsoBBTTCutKine"
export VARTAUTAU="HHKin_mass"
export VARMUTAU="HHKin_mass"
export ALLVARS="HHKin_mass"
export ALLSELS="dijethardSV defaultOneBtagLNoIso defaultOneBtagMNoIso defaultBtagMMNoIso defaultBtagLLNoIso defaultBtagMMMbbMttNoIso defaultBtagLLMbbMttNoIso defaultBtagMMNoIsoBBTTCut defaultBtagLLNoIsoBBTTCut defaultBtagMMMbbMttNoIsoKine defaultBtagLLMbbMttNoIsoKine defaultBtagMMNoIsoBBTTCutKine defaultBtagLLNoIsoBBTTCutKine"
#export ALLSELS="defaultBtagLLMbbMttNoIso defaultBtagMMMbbMttNoIso defaultBtagMMKineMbbMttNoIso"
#"defaultBtagML defaultBtagLL dijethardisoBtagCutMMbbMtt dijetOneBL dijetOneBM dijethardisoMbbMtt dijetOneBLKine dijetOneBMKine"
#dijetOneBLKineTrig

export LAMBDAS="Lambdam4 Lambda1 Lambda2dot46 Lambda20"
#export LAMBDAS="Radion250 Radion300 Radion320 Radion400 Radion500"
#baselineiso, dijethardiso, defaultBtagML, defaultBtagLL, dijethardisoBtagCutMMbbMtt, dijetisoMbbMtt, dijetOneBL, dijetOneBM, dijethardisoMbbMtt, dijetOneBLKine, dijetOneBLKineTrig

for c in MuTau TauTau ETau
	do
		for i in $LAMBDAS 
		do	
		for j in $ALLSELS ; 
		do 

		for v in $ALLVARS
	        #do python cardMaker.py -f /home/llr/cms/cadamuro/HHKlubAnalysis/KLUBAnalysis/analysis_TauTau/evalQCD_Plots_14Oct_FullStat/outPlotter.root -l $i -o $j -v $v -c "TauTau" 
	        #do python cardMaker.py -f ../analysis_${c}/evalQCD_Plots_18Nov_FullStat/outPlotter.root -l $i -o $j -v $v -c $c
	        do python cardMaker.py -i ../config/analysis_${c}.cfg -f ../analysis_${c}/evalQCD_Plots_08Dec_FullStat/outPlotter.root -o $j -v $v -c $c
	    done
	done
	done 
	mv cards cards_${c}_$OUTSTRING
done

#make WS 
for c in TauTau MuTau ETau
	do
		for i in $LAMBDAS 
		do	
		for j in $ALLSELS ; 
		do 

		for v in $ALLVARS
		do 
		cd cards_${c}_$OUTSTRING/${i}${j}${v}
		pwd
		if [ -a "hh_3_L${i}_13TeV.txt" ]
			then
			combineCards.py -S "hh_3_L${i}_13TeV.txt" >> comb.txt 
		fi
		if [ -a "hh_2_L${i}_13TeV.txt" ]
			then
			combineCards.py -S "hh_2_L${i}_13TeV.txt" >> comb.txt 
		fi
		if [ -a "hh_1_L${i}_13TeV.txt" ]
			then
			combineCards.py -S "hh_1_L${i}_13TeV.txt" >> comb.txt 
		fi
		text2workspace.py comb.txt -o comb.root ;
		#combine -M Asymptotic comb.root -m 125.0 -t -1 --expectSignal=1 -n ${i} -S 0
		combine -M Asymptotic comb.root -m 125.0 -n ${i}_forLim
		cd -
	done
	done
	done 
	done

#do the combination
for i in $LAMBDAS 
do	
	for j in $ALLSELS ; 
	do 
	for v in $ALLVARS
	do 
	mkdir -p cards_Combined_$OUTSTRING/${i}${j}${v}
	cp cards_MuTau_$OUTSTRING/${i}${j}${v}/hh_*.* cards_Combined_$OUTSTRING/${i}${j}${v}/.
	cp cards_TauTau_$OUTSTRING/${i}${j}${v}/hh_*.* cards_Combined_$OUTSTRING/${i}${j}${v}/.
	cp cards_ETau_$OUTSTRING/${i}${j}${v}/hh_*.* cards_Combined_$OUTSTRING/${i}${j}${v}/.
	cd cards_Combined_$OUTSTRING/${i}${j}${v}
	combineCards.py -S "hh_3_L${i}_13TeV.txt" "hh_2_L${i}_13TeV.txt" "hh_1_L${i}_13TeV.txt" >> comb.txt
	text2workspace.py comb.txt -o comb.root ;
		#combine -M Asymptotic comb.root -m 125.0 -t -1 --expectSignal=1 -n ${i} -S 0
	combine -M Asymptotic comb.root -m 125.0 -n ${i}_forLim
	cd -
	done
	done
done 


#MIXED CUT COMBINATION
for i in $LAMBDAS
do
        mkdir -p cards_Combined_$OUTSTRING/${i}COMBINED
        cp cards_MuTau_$OUTSTRING/${i}${BASEMUTAU}${VARMUTAU}/hh_*.* cards_Combined_$OUTSTRING/${i}COMBINED/.
        cp cards_TauTau_$OUTSTRING/${i}${BASETAUTAU}${VARTAUTAU}/hh_*.* cards_Combined_$OUTSTRING/${i}COMBINED/.
        cp cards_ETau_$OUTSTRING/${i}${BASETAUTAU}${VARTAUTAU}/hh_*.* cards_Combined_$OUTSTRING/${i}COMBINED/.
        cd cards_Combined_$OUTSTRING/${i}COMBINED
        combineCards.py -S "hh_3_L${i}_13TeV.txt" "hh_2_L${i}_13TeV.txt" "hh_1_L${i}_13TeV.txt" >> comb.txt
        text2workspace.py comb.txt -o comb.root ;
                #combine -M Asymptotic comb.root -m 125.0 -t -1 --expectSignal=1 -n ${i} -S 0
        combine -M Asymptotic comb.root -m 125.0 -n ${i}_forLim
        cd -
        done

	#./makeWS.sh _mutau_$OUTSTRING 2
#./makeWS.sh _tautau_$OUTSTRING 3


#combine with syst for limits
#for i in $LAMBDAS 
#do
#	#cd cards_mutau_$OUTSTRING/lambda${i}${BASEMUTAU}${VARMUTAU}
#	#combine -M Asymptotic comb.root -m 125.0 -n Lambda${i}_forLim 
#	#cd -
#
#	cd cards_tautau_$OUTSTRING/lambda${i}${BASETAUTAU}${VARTAUTAU}
#	combine -M Asymptotic comb.root -m 125.0 -n Lambda${i}_forLim 
#	cd -
#done

##plotting: leave it for now

