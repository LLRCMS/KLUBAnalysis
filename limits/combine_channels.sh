var="DNNoutSM_kl_1"
selections="s1b1jresolvedMcut s2b0jresolvedMcut sboostedLLMcut GGFclass VBFclass ttHclass TTlepclass TThadclass DYclass"
channels="TauTau MuTau ETau" 

tag="2016_17Nov2020_noQCD"
#tag="2017_17Nov2020_noQCD"
#tag="2018_17Nov2020_noQCD"

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
text2workspace.py comb.txt -P HHModel:HHdefault -o comb.root
cd -
