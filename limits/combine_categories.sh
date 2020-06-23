var="DNNoutSM_kl_1"
selections="s1b1jresolvedMcut s2b0jresolvedMcut sboostedLLMcut VBFloose"
tag="cards_TauTaukl1"

cd $tag
mkdir comb_cat
rm comb_cat/comb_cat.txt
combineCards.py -S */comb.txt >> comb_cat/comb_cat.txt
text2workspace.py comb_cat/comb_cat.txt -P HHModel:HHdefault -o comb_cat/comb_cat.root
cd -
