selections="s1b1jresolvedMcut s2b0jresolvedMcut sboostedLLMcut VBFloose"

var="DNNoutSM_kl_1"


#tag="cards_TauTau2016_22Sep2020"
#tag="cards_MuTau2016_22Sep2020"
#tag="cards_ETau2016_22Sep2020"

#tag="cards_TauTau2017_22Sep2020"
#tag="cards_MuTau2017_22Sep2020"
tag="cards_ETau2017_22Sep2020"

##tag="cards_TauTau2018_22Sep2020"
##tag="cards_MuTau2018_22Sep2020"
##tag="cards_ETau2018_22Sep2020"

for sel in $selections
do 
cd $tag/$sel$var
rm comb.txt
combineCards.py -S hh_*.txt >> comb.txt
cd -
text2workspace.py $tag/$sel$var/comb.txt -P HHModel:HHdefault -o $tag/$sel$var/comb.root
done
