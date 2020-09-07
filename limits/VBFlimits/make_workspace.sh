# VBF limits are computed only on the combination of the "multicategories"
# - first the the ABCD regions are combined in each multicategory
# - second the multicategories are combined in a single directory "comb_cat"
# - finally the workspace is created only for the "comb_cat" directory

tag="cards_prova"

selections="DYclassmdnn__v2__kl1_c2v1_c31__dy TThadclassmdnn__v2__kl1_c2v1_c31__tt_fh VBFclassmdnn__v2__kl1_c2v1_c31__hh_vbf GGFclassmdnn__v2__kl1_c2v1_c31__hh_ggf TTlepclassmdnn__v2__kl1_c2v1_c31__tt_lep ttHclassmdnn__v2__kl1_c2v1_c31__tth"

for sel in $selections
do 
cd $tag/$sel
rm comb.txt
combineCards.py -S hh_*.txt >> comb.txt
cd -
done

cd $tag
mkdir comb_cat
rm comb_cat/comb_cat.txt
combineCards.py -S */comb.txt >> comb_cat/comb_cat.txt
text2workspace.py comb_cat/comb_cat.txt -P HHModel:HHdefault -o comb_cat/comb_cat.root
cd -
