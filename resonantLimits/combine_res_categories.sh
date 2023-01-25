var="DNNoutSM_kl_1"
selections="s1b1jresolvedMcut s2b0jresolvedMcut sboostedLLMcut VBFloose"

masses="250 260 270 280 300 320 350 400 450 500 550 600 650 700 750 800 850 900 1000 1250 1500 1750 2000 2500 3000"

#tag="cards_ETau2018_res_17Feb2021"
#tag="cards_MuTau2018_res_17Feb2021"
tags="cards_TauTauLegacy2018_29Mar2021_m cards_MuTauLegacy2018_29Mar2021_m cards_ETauLegacy2018_29Mar2021_m"
for tag in $tags
do
    cd $tag
    mkdir comb_cat
    rm comb_cat/comb*.txt
    for mass in $masses
    do
	combineCards.py -S *kl_1/hhres*.Radion$mass.txt >> comb_cat/comb.Radion$mass.txt
	echo 'SignalScale rateParam * Radion'$mass' 0.01' > add.txt
	cat add.txt >> comb_cat/comb.Radion$mass.txt
	text2workspace.py comb_cat/comb.Radion$mass.txt -o comb_cat/comb.Radion$mass.root
    done
cd -
done
