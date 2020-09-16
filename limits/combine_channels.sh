var="DNNoutSM_kl_1"
selections="s1b1jresolvedMcut s2b0jresolvedMcut sboostedLLMcut VBFloose"
channels="TauTau MuTau ETau" 

#tag="2016_10Sept_NS_V2"
#tag="2017_10Sept_NS_V2"
#tag="2018_10Sept_NS_V2"

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
