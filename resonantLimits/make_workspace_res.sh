#!/bin/bash
selections="s1b1jresolvedMcut s2b0jresolvedMcut sboostedLLMcut VBFloose"
masses="250 260 270 280 300 320 350 400 450 500 550 600 650 700 750 800 850 900 1000 1250 1500 1750 2000 2500 3000"
var="DNNoutSM_kl_1"


#tag="cards_TauTau2016_22Sep2020"
#tag="cards_MuTau2016_22Sep2020"
#tag="cards_ETau2016_22Sep2020"

#tag="cards_TauTau2017_22Sep2020"
#tag="cards_MuTau2017_22Sep2020"
#tag="cards_ETau2017_22Sep2020"

##tag="cards_TauTau2018_22Sep2020"
##tag="cards_MuTau2018_22Sep2020"
##tag="cards_ETau2018_22Sep2020"

#tag="cards_TauTau2018_res_17Feb2021"
#tag="cards_MuTau2018_res_17Feb2021"
tags="cards_ETau2018_res_nolowmassboosted_23Feb2021 cards_MuTau2018_res_nolowmassboosted_23Feb2021 cards_TauTau2018_res_nolowmassboosted_23Feb2021"

for tag in $tags
do
    for sel in $selections
    do 
	for mass in $masses
	do
	    cd $tag/$sel$var
	    rm comb.txt
	    combineCards.py -S hhres_*.Radion$mass.txt >> comb.Radion$mass.txt
	    cd -
	    text2workspace.py $tag/$sel$var/comb.Radion$mass.txt -o $tag/$sel$var/comb.Radion$mass.root
	done
    done
    cd /home/llr/cms/portales/hhbbtautau/CMSSW_11_1_0_pre6/src/KLUBAnalysis/lp_limits
done
