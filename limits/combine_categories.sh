var="DNNoutSM_kl_1"
selections="s1b1jresolvedMcut s2b0jresolvedMcut sboostedLLMcut VBFloose"

#tag="cards_TauTau2016_10Sept_NS_V2"
#tag="cards_MuTau2016_10Sept_NS_V2"
#tag="cards_ETau2016_10Sept_NS_V2"

#tag="cards_TauTau2017_10Sept_NS_V2"
#tag="cards_MuTau2017_10Sept_NS_V2"
#tag="cards_ETau2017_10Sept_NS_V2"

#tag="cards_TauTau2018_10Sept_NS_V2"
#tag="cards_MuTau2018_10Sept_NS_V2"
#tag="cards_ETau2018_10Sept_NS_V2"

cd $tag
mkdir comb_cat
rm comb_cat/comb_cat.txt
combineCards.py -S */comb.txt >> comb_cat/comb.txt
text2workspace.py comb_cat/comb.txt -P HHModel:HHdefault -o comb_cat/comb.root
cd -
