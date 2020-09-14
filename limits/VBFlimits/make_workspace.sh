# VBF limits are computed only on the combination of the "multicategories"
# - first the the ABCD regions are combined in each multicategory
# - second the multicategories are combined in a single directory "comb_cat"
# - finally the workspace is created only for the "comb_cat" directory

#tag="cards_TauTau2016_14Sept_NS_V2"
#tag="cards_TauTau2016_14Sept_NS_V3"
tag="cards_TauTau2016_14Sept_NS_V4"

#selections="DYclassmdnn__v2__kl1_c2v1_c31__dy_new TThadclassmdnn__v2__kl1_c2v1_c31__tt_fh_new VBFclassmdnn__v2__kl1_c2v1_c31__hh_vbf_new GGFclassmdnn__v2__kl1_c2v1_c31__hh_ggf_new TTlepclassmdnn__v2__kl1_c2v1_c31__tt_lep_new ttHclassmdnn__v2__kl1_c2v1_c31__tth_new"
#selections="DYclassV3mdnn__v3__kl1_c2v1_c31_vbf__dy_new TThadclassV3mdnn__v3__kl1_c2v1_c31_vbf__tt_fh_new VBFclassV3mdnn__v3__kl1_c2v1_c31_vbf__hh_vbf_new GGFclassV3mdnn__v3__kl1_c2v1_c31_vbf__hh_ggf_new TTlepclassV3mdnn__v3__kl1_c2v1_c31_vbf__tt_lep_new ttHclassV3mdnn__v3__kl1_c2v1_c31_vbf__tth_new"
selections="DYclassV4mdnn__v3__kl1_c2v1_c31_vr__dy_new TThadclassV4mdnn__v3__kl1_c2v1_c31_vr__tt_fh_new VBFclassV4mdnn__v3__kl1_c2v1_c31_vr__hh_vbf_new GGFclassV4mdnn__v3__kl1_c2v1_c31_vr__hh_ggf_new TTlepclassV4mdnn__v3__kl1_c2v1_c31_vr__tt_lep_new ttHclassV4mdnn__v3__kl1_c2v1_c31_vr__tth_new"

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
