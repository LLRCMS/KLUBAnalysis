var="DNNoutSM_kl_1"

tag="24June"

mkdir cards_$tag/comb

selections="s1b1jresolvedMcutDNNoutSM_kl_1 s2b0jresolvedMcutDNNoutSM_kl_1 sboostedLLMcutDNNoutSM_kl_1 VBFlooseDNNoutSM_kl_1"
channels="ETau MuTau TauTau"

rm cards_$tag/comb.txt

for sel in $selections
do
mkdir cards_$tag/$sel
for ch in $channels
do

cp cards_$ch$tag/$sel/hh*.* cards_$tag/$sel
cp cards_$ch$tag/$sel/hh*.* cards_$tag/comb
cd cards_$tag/$sel
rm comb.txt
combineCards.py -S hh*.txt>> comb.txt
text2workspace.py comb.txt -P HHModel:HHdefault -o comb.root
cd - 
done
done


cd cards_$tag/comb
rm comb.txt

combineCards.py -S hh*.txt>> comb.txt
text2workspace.py comb.txt -P HHModel:HHdefault -o comb.root

cd -
