var="DNNoutSM_kl_1"
selections="s1b1jresolvedMcut s2b0jresolvedMcut sboostedLLMcut GGFclass VBFclass ttHclass TTclass DYclass"
benchmarks="1 2 3 4 5 6 7 8 9 10 11 12 1b 2b 3b 4b 5b 6b 7b 8a"
channels="TauTau MuTau ETau" 
years="2018"

tag="_25Nov2022"

for bm in $benchmarks
do
    mkdir EFT_cards_stackedAndCleaned/cards_$bm/cards_CombAll$tag

    for ch in $channels
    do
        for sel in $selections
        do
            for year in $years
            do
                cp EFT_cards_stackedAndCleaned/cards_$bm/cards_$ch$year$tag/$sel$var/datacard_* EFT_cards_stackedAndCleaned/cards_$bm/cards_CombAll$tag
            done
        done
    done

    cd EFT_cards_stackedAndCleaned/cards_$bm/cards_CombAll$tag
    rm comb.txt
    combineCards.py -S datacard_*.txt >> comb.txt
    ulimit -s unlimited
    text2workspace.py comb.txt -o comb.root -m 125
    cd -
done