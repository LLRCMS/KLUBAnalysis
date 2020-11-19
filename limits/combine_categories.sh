var="DNNoutSM_kl_1"
selections="s1b1jresolvedMcut s2b0jresolvedMcut sboostedLLMcut GGFclass VBFclass ttHclass TTlepclass TThadclass DYclass"

#tag="cards_TauTau2016_17Nov2020_noQCD"
#tag="cards_MuTau2016_17Nov2020_noQCD"
#tag="cards_ETau2016_17Nov2020_noQCD"

cd $tag
pwd
mkdir comb_cat
rm comb_cat/comb_cat.txt
#combineCards.py -S */comb.txt >> comb_cat/comb.txt
combineCards.py -S */hh_*.txt >> comb_cat/comb.txt
text2workspace.py comb_cat/comb.txt -P HHModel:HHdefault -o comb_cat/comb.root
cd -
