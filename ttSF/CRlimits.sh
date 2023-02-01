# This script takes as input the outPlotter_*.sh coming from the submitHistoFiller.py script
# and gives as output the comb.root file containing teh fitted ttSF from a CR only fit
# 
# If you have used the ttCR_inclusiveOutPlotter_creator.sh sccript on CR first, then you need 
# to comment out everything up to the call of write_cards_ttCR.py (not included)
#
# To make this work we first have to:
# 	1. crate the following folders: 
#			- ttCRlimits_${Y}inclusive
#			- ttCRlimits_${Y}inclusive/ETau
#			- ttCRlimits_${Y}inclusive/MuTau
#			- ttCRlimits_${Y}inclusive/TauTau
# 	2. launch the submitHistoFiller.py to produce the outplotters inside the folders just created (cfr. paths in the mainCfg)
#	3. launch this bash script
#
# This will create the .root files that then have to be passed to the write_cards script 


export Y="2017"
export CHANNELS="TauTau MuTau ETau"


for ch in $CHANNELS
do 
	cd ../ttCRlimits_${Y}inclusive/${ch}/
	hadd outPlotter.root outPlotter_*.root
	rm outPlotter_*.root
	python ../../scripts/combineFillerOutputs.py --dir ../../ttCRlimits_${Y}inclusive/${ch}/
	cd -
done 

cd ../ttCRlimits_${Y}inclusive/
hadd analyzedOutPlotter.root ETau/analyzedOutPlotter.root MuTau/analyzedOutPlotter.root TauTau/analyzedOutPlotter.root 
cp TauTau/main*.cfg ./mainCfg_ttCR_Legacy${Y}.cfg

cd ../ttSF
python prepare_histos.py -f ../ttCRlimits_${Y}inclusive/analyzedOutPlotter.root -o CR -c 'inclusive' -y ${Y}
mv -v analyzedOutPlotter_${Y}_inclusive_CR.root ../ttCRlimits_${Y}inclusive/

python write_cards_ttCR.py --filename ../ttCRlimits_${Y}inclusive/analyzedOutPlotter_${Y}_inclusive_CR.root --dir '' --channel 'inclusive' --year ${Y} --selection ttCR_invMcut --binbybin 1 --shape 0 --theory --config ../ttCRlimits_${Y}inclusive/main*.cfg --dynamQCD 0

cd cards_inclusive
combineCards.py ttCR_invMcutDNNoutSM_kl_1/hh_*.txt >> comb.txt
text2workspace.py comb.txt -o comb.root
combine -M FitDiagnostics comb.root --rMin 0 --rMax 1 --saveShapes --saveWithUncertainties
