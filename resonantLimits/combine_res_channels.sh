var="DNNoutSM_kl_1"
selections="s1b1jresolvedMcutDNNoutSM_kl_1 s2b0jresolvedMcutDNNoutSM_kl_1 sboostedLLMcutDNNoutSM_kl_1 VBFlooseDNNoutSM_kl_1"
channels="TauTau MuTau ETau" 
masses="250 260 270 280 300 320 350 400 450 500 550 600 650 700 750 800 850 900 1000 1250 1500 1750 2000 2500 3000"

#tag="2016_4Oct2020"
#tag="2017_4Oct2020"
#tag="2018_4Oct2020"
year="2018"
tag="Legacy2018_29Mar2021_m"

mkdir cards_CombChan${year}_res_$tag
for selection in $selections
do
    mkdir cards_CombChan${year}_res_$tag/$selection
done

#for ch in $channels
#do
#    for sel in $selections
#    do
#for mass in $masses
#do
#            cp cards_$ch${year}_res_$tag/$sel$var/comb.Radion$mass.txt cards_CombChan${year}_res_$tag/${sel}/comb.Radion$mass.txt
#done
#    done
#done
selections="s1b1jresolvedMcut s2b0jresolvedMcut sboostedLLMcut VBFloose"
cd cards_CombChan${year}_res_$tag
for sel in $selections
do
    for mass in $masses
    do
	rm $sel/comb.Radion$mass.txt
	combineCards.py -S ../cards_*Tau$tag/$sel*/hhres*Radion$mass.txt >> $sel$var/comb.Radion$mass.txt
	echo 'SignalScale rateParam * Radion'$mass' 0.01' > add.txt
        cat add.txt >> $sel$var/comb.Radion$mass.txt
	text2workspace.py $sel$var/comb.Radion$mass.txt -o $sel$var/comb.Radion$mass.root
    done
done
cd -
