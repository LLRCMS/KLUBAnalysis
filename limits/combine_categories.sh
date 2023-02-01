#tag="cards_TauTau2016_24Mar2021"
#tag="cards_MuTau2016_24Mar2021"
#tag="cards_ETau2016_24Mar2021"

#tag="cards_TauTau2017_24Mar2021"
#tag="cards_MuTau2017_24Mar2021"
#tag="cards_ETau2017_24Mar2021"

#tag="cards_TauTau2018_24Mar2021"
#tag="cards_MuTau2018_24Mar2021"
tag="cards_ETau2018_24Mar2021"

cd $tag
pwd
mkdir comb_cat
rm comb_cat/comb_cat.txt
combineCards.py -S */hh_*.txt >> comb_cat/comb.txt
text2workspace.py comb_cat/comb.txt -P HHModelPinv:model_bbtautau -o comb_cat/comb.root -m 125
cd -
