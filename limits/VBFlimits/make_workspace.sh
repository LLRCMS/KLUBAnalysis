# VBF limits are computed only on the combination of the "multicategories"
# - first the the ABCD regions are combined in each multicategory
# - second the multicategories are combined in a single directory "comb_cat"
# - finally the workspace is created only for the "comb_cat" directory

#tag="cards_TauTau2016_22Sep2020"
#tag="cards_MuTau2016_22Sep2020"
#tag="cards_ETau2016_22Sep2020"

#tag="cards_TauTau2017_22Sep2020"
#tag="cards_MuTau2017_22Sep2020"
#tag="cards_ETau2017_22Sep2020"


#selections="DYclassmdnn__v2__kl1_c2v1_c31__dy_new TThadclassmdnn__v2__kl1_c2v1_c31__tt_fh_new VBFclassmdnn__v2__kl1_c2v1_c31__hh_vbf_new GGFclassmdnn__v2__kl1_c2v1_c31__hh_ggf_new TTlepclassmdnn__v2__kl1_c2v1_c31__tt_lep_new ttHclassmdnn__v2__kl1_c2v1_c31__tth_new"
selections="DYclassmdnn__v3b__kl1_c2v1_c31_vbf__dy_new TThadclassmdnn__v3b__kl1_c2v1_c31_vbf__tt_fh_new VBFclassmdnn__v3b__kl1_c2v1_c31_vbf__hh_vbf_new GGFclassmdnn__v3b__kl1_c2v1_c31_vbf__hh_ggf_new TTlepclassmdnn__v3b__kl1_c2v1_c31_vbf__tt_lep_new ttHclassmdnn__v3b__kl1_c2v1_c31_vbf__tth_new"

for sel in $selections
do 
cd $tag/$sel
rm comb.txt
combineCards.py -S hh_*.txt >> comb.txt
cd -
done

cd $tag
mkdir comb_cat
rm comb_cat/comb.txt
combineCards.py -S */comb.txt >> comb_cat/comb.txt
text2workspace.py comb_cat/comb.txt -P HHModel:HHdefault -o comb_cat/comb.root
cd -
