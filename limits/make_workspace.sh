var="DNNoutSM_kl_1"
selections="s1b1jresolvedMcut s2b0jresolvedMcut sboostedLLMcut VBFloose"
tag="cards_TauTauprova" 

for sel in $selections
do 
cd $tag/$sel$var
rm comb.txt
combineCards.py -S hh_*.txt >> comb.txt
cd -
text2workspace.py $tag/$sel$var/comb.txt -P HHModel:HHdefault -o $tag/$sel$var/comb.root
done
