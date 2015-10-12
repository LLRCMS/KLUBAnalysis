#!/bin/bash

cd cards ;

OBSERVED=$1
Options="--minosAlgo=stepping --X-rtd TMCSO_AdaptivePseudoAsimov --minimizerStrategy=0 --minimizerTolerance=0.0001 --cminFallback Minuit2:0.01 --cminFallback Minuit:0.001 --expectSignal=1"
if [ $OBSERVED == "observed" ]
then
    Options="${Options} --expectSignal=1"
fi

for i in 0 1 10 20 ;
do
    echo "running combine for lambda${i}";
    cd lambda${i};
    OPTIONS="${Options} -n Lambda${i} "
    #At this point I should do combineCards (to do it once I have more channels)
    combine -M Asymptotic comb.root -m 125.7 ${OPTIONS}
    cd -
done
