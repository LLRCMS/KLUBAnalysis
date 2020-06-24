selections="s1b1jresolvedMcut s2b0jresolvedMcut sboostedLLMcut VBFloose"

#var="DNNoutSM_kl_1_new"
#var="DNNoutSM_kl_5_new"
var="DNNoutSM_kl_1"

#tag="cards_TauTaukl1new"
#tag="cards_TauTaukl5new"
tag="cards_TauTaukl1"

for sel in $selections
do 
cd $tag/$sel$var
rm comb.txt
combineCards.py -S hh_*.txt >> comb.txt
cd -
text2workspace.py $tag/$sel$var/comb.txt -P HHModel:HHdefault -o $tag/$sel$var/comb.root
done
