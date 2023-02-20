var="DNNoutSM_kl_1"
#selections="s1b1jresolvedMcut s2b0jresolvedMcut sboostedLLMcut GGFclass VBFclass ttHclass TTlepclass TThadclass DYclass"
selections="s1b1jresolvedMcut s2b0jresolvedMcut sboostedLLMcut GGFclass VBFclass ttHclass TTclass DYclass"
channels="TauTau MuTau ETau" 

#tag="2016_24Mar2021"
#tag="2017_24Mar2021"
tag="2018_24Mar2021"

mkdir cards_CombChan_$tag

for ch in $channels
do
    for sel in $selections
    do
        cp cards_$ch$tag/$sel$var/hh_* cards_CombChan_$tag/
    done
done

cd cards_CombChan_$tag
rm comb.txt
combineCards.py -S hh_*.txt >> comb.txt
text2workspace.py comb.txt -P HHModelPinv:model_bbtautau -o comb.root -m 125
cd -
