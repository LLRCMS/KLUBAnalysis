selections="DYclassmdnn__v4__kl1_c2v1_c31_vbf__dy_new TThadclassmdnn__v4__kl1_c2v1_c31_vbf__tt_fh_new VBFclassmdnn__v4__kl1_c2v1_c31_vbf__hh_vbf_new GGFclassmdnn__v4__kl1_c2v1_c31_vbf__hh_ggf_new TTlepclassmdnn__v4__kl1_c2v1_c31_vbf__tt_lep_new ttHclassmdnn__v4__kl1_c2v1_c31_vbf__tth_new"
channels="TauTau MuTau ETau" 

tag="2018_5Oct2020"

mkdir cards_CombChan_$tag

for ch in $channels
do
    for sel in $selections
    do
        cp cards_$ch$tag/$sel/hh_* cards_CombChan_$tag/
    done
done

cd cards_CombChan_$tag
rm comb.txt
combineCards.py -S hh_*.txt >> comb.txt
text2workspace.py comb.txt -P HHModel:HHdefault -o comb.root
cd -
