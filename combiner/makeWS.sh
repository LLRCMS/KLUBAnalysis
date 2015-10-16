#!/bin/bash

cd "cards${1}" ;

for i in -4.00 1.00 2.46 20.00 ; #0 1 10 20;
do
for j in dijethardiso baselineisoBtagCutM baselineiso baselineisoBtagCutMMbbMtt dijethardisoBtagCutMMbbMtt
do
for v in HH_pt HH_mass tauH_mass  met_et mT bH_mass
do
    echo "preparing cards and WS for lambda${i}";
    cd lambda${i}${j}${v};
    #At this point I should do combineCards (to do it once I have more channels)
    CARDS=""
    for c in "$2" ;
    do
	CARDS="$CARDS hh_${c}_L${i}_13TeV.txt " ;
    done
    combineCards.py -S $CARDS >> comb.txt ;
    text2workspace.py comb.txt -o comb.root ;
    cd -
done
done
done
