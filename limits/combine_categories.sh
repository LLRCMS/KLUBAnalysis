#tag="cards_TauTau2016_27Nov2020"
#tag="cards_MuTau2016_27Nov2020"
#tag="cards_ETau2016_27Nov2020"

#tag="cards_TauTau2017_27Nov2020"
#tag="cards_MuTau2017_27Nov2020"
#tag="cards_ETau2017_27Nov2020"

#tag="cards_TauTau2018_27Nov2020"
#tag="cards_MuTau2018_27Nov2020"
#tag="cards_ETau2018_27Nov2020"

cd $tag
pwd
mkdir comb_cat
rm comb_cat/comb_cat.txt
combineCards.py -S */hh_*.txt >> comb_cat/comb.txt
text2workspace.py comb_cat/comb.txt -P HHModel:HHdefault -o comb_cat/comb.root
cd -
