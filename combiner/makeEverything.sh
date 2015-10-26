#!/bin/bash
# make cards with all vars/selections

export OUTSTRING="23oct"
export BASEMUTAU="defaultBtagML"
export BASETAUTAU="dijethardisoBtagCutM"
export VARTAUTAU="HH_mass"
export VARMUTAU="HH_mass"
export ALLVARS="HH_mass"
export ALLSELS="defaultBtagML dijethardisoBtagCutM"
export LAMBDAS="-4.00 1.00 2.46 20.00"


for i in $LAMBDAS 
	do	
	for j in $ALLSELS ; 
	    do 
	    for v in $ALLVARS
	        #do python cardMaker.py -f /home/llr/cms/cadamuro/HHKlubAnalysis/KLUBAnalysis/analysis_TauTau/evalQCD_Plots_14Oct_FullStat/outPlotter.root -l $i -o $j -v $v -c "TauTau" 
	        do python cardMaker.py -f ../analysis_TauTau/evalQCD_Plots_23Oct_FullStat/outPlotter.root -l $i -o $j -v $v -c "TauTau"
		done
	    done
	done 
mv cards cards_tautau_$OUTSTRING
for i in $LAMBDAS 
do 
	for j in $ALLSELS
	do 
	for v in $ALLVARS
	do 
	#pwd 
        python cardMaker.py -f ../analysis_MuTau/evalQCD_Plots_23Oct_FullStat/outPlotter.root -l $i -o $j -v $v -c "MuTau" 
	done 
	done  
done 
mv cards cards_mutau_$OUTSTRING

#make WS 
for i in $LAMBDAS 
	do	
	for j in $ALLSELS ; 
	    do 
	    for v in $ALLVARS
	        do 
	        cd cards_tautau_$OUTSTRING/lambda${i}${j}${v}
	        pwd
	        combineCards.py -S "hh_3_L${i}_13TeV.txt" >> comb.txt ;
	        text2workspace.py comb.txt -o comb.root ;
	        cd -
	        cd cards_mutau_$OUTSTRING/lambda${i}${j}${v}
	        pwd
	        combineCards.py -S "hh_2_L${i}_13TeV.txt" >> comb.txt ;
	        text2workspace.py comb.txt -o comb.root ;
	        cd -
	    done
	done
	done 

#./makeWS.sh _mutau_$OUTSTRING 2
#./makeWS.sh _tautau_$OUTSTRING 3

#combine cards for everything (without syst)
for i in $LAMBDAS 
	do	
	for j in $ALLSELS ; 
	    do 
	    for v in $ALLVARS
	        do 
	        for c in tautau mutau 
	        do
	        echo "doing limits for cards_${c}_$OUTSTRING/lambda${i}${j}${v}"
	        cd cards_${c}_$OUTSTRING/lambda${i}${j}${v}
	        combine -M Asymptotic comb.root -m 125.0 -t -1 --expectSignal=1 -n Lambda${i} -S 0
	        cd -
	    done
	done
	done
	done 
#./combineResults.sh _mutau_$OUTSTRING
#./combineResults.sh _tautau_$OUTSTRING


#combine with syst for limits
for i in $LAMBDAS 
do
	cd cards_mutau_$OUTSTRING/lambda${i}${BASEMUTAU}${VARMUTAU}
	combine -M Asymptotic comb.root -m 125.0 -t -1 -n Lambda${i}_forLim 
	cd -

	cd cards_tautau_$OUTSTRING/lambda${i}${BASETAUTAU}${VARTAUTAU}
	combine -M Asymptotic comb.root -m 125.0 -t -1 -n Lambda${i}_forLim 
	cd -
done

##plotting: leave it for now

