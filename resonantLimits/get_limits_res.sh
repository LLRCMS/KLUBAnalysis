#!/bin/bash
selections="s1b1jresolvedMcut s2b0jresolvedMcut sboostedLLMcut VBFloose"
masses="250 260 270 280 300 320 350 400 450 500 550 600 650 700 750 800 850 900 1000 1250 1500 1750 2000 2500 3000"
#masses="300 320 400 600 850 1250"
var="DNNoutSM_kl_1"

mHHs="mHH250_335 mHH335_475 mHH475_725 mHH725_1100 mHH1100_3500 InvDR"

#tags="cards_TauTauLegacy2018_29Mar2021_m cards_MuTauLegacy2018_29Mar2021_m cards_ETauLegacy2018_29Mar2021_m"

tags="cards_MuTauLegacy2018_24May2021 cards_ETauLegacy2018_24May2021"

identifier=".test"

echo "TEST"


for tag in $tags
do
cd $tag
echo $tag
for sel in $selections
do
mkdir $sel/combined_out
for mass in $masses
do
    echo $mass
    rm $sel/combined_out/comb.Radion$mass.log
    combine -M AsymptoticLimits $sel/comb.Radion$mass.txt -n $identifier$sel --run blind --noFitAsimov --freezeParameters SignalScale -m $mass > $sel/combined_out/comb.Radion$mass.log
done
done
cd -
done

tags="cards_TauTauLegacy2018_24May2021"

mHHs="mHH250_335 mHH335_475 mHH475_725 mHH725_1100 mHH1100_3500"

for tag in $tags
do
cd $tag
echo $tag
for sel in $selections
do
mkdir $sel/combined_out
for mass in $masses
do
    echo $mass
    rm $sel/combined_out/comb.Radion$mass.log
    combine -M AsymptoticLimits $sel/comb.Radion$mass.txt -n $identifier$sel --run blind --noFitAsimov --freezeParameters SignalScale -m $mass > $sel/combined_out/comb.Radion$mass.log
done
done
cd -
done
