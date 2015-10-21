#!/bin/bash

cd "cards$1" ;

#OBSERVED=$1
#Options="--minosAlgo=stepping --X-rtd TMCSO_AdaptivePseudoAsimov --minimizerStrategy=0 --minimizerTolerance=0.0001 --cminFallback Minuit2:0.01 --cminFallback Minuit:0.001 -t -1 --expectSignal=1"
Options=" -t -1 --expectSignal=1"
#Options=" -t -1"
#if [ $OBSERVED == "observed" ]
#then
#    Options="${Options} --expectSignal=1"
#fi

for i in -4.00 1.00 2.46 20.00 ; #0 1 10 20 ;
do
for j in dijethardiso baselineisoBtagCutM baselineiso baselineisoBtagCutMMbbMtt dijethardisoBtagCutMMbbMtt
do
for v in HH_pt HH_mass tauH_mass  met_et mT bH_mass
do
    echo "running combine for lambda${i}";
    cd lambda${i}${j}${v};
    #OPTIONS="${Options} -n Lambda${i} "
    OPTIONSNOSYST="${Options} -n Lambda${i} -S 0 "
    #At this point I should do combineCards (to do it once I have more channels)
    #combine -M Asymptotic comb.root -m 125.0 ${OPTIONS} ##ASYMPTOTIC DOESN'T WORK WHEN YIELDS ARE TOO LOW
    combine -M Asymptotic comb.root -m 125.0 ${OPTIONSNOSYST}
    cd -
done
done
done
