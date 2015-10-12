#!/bin/bash

cd cards ;

for i in 0 1 10 20;
do
    echo "preparing cards and WS for lambda${i}";
    cd lambda${i};
    #At this point I should do combineCards (to do it once I have more channels)
    CARDS=""
    for c in "2" ;
    do
	CARDS="$CARDS hh_${c}_L${i}_13TeV.txt " ;
    done
    combineCards.py -S $CARDS >> comb.txt ;
    text2workspace.py comb.txt -o comb.root ;
    cd -
done
