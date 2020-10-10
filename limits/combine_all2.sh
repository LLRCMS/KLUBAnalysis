var="DNNoutSM_kl_1"
selections="s1b1jresolvedMcut s2b0jresolvedMcut sboostedLLMcut VBFloose"
channels="TauTau MuTau ETau" 
years="2016 2017 2018"

tag="_4Oct2020"

mkdir cards_CombAll$tag

for ch in $channels
do
    for sel in $selections
    do
        for year in $years
        do
            cp cards_$ch$year$tag/$sel$var/hh_* cards_CombAll$tag/
        done
    done
done

cd cards_CombAll$tag
rm comb.txt
combineCards.py -S hh_*.txt >> comb.txt
text2workspace.py comb.txt -P HHModel:HHdefault -o comb.root
cd -
