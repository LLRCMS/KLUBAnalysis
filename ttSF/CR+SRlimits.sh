# This script takes as input the outPlotter_*.sh coming from the submitHistoFiller.py script
# and gives as output the comb.root file containing teh fitted ttSF from a CR+SR fit
# 
# If you have used the ttCR_inclusiveOutPlotter_creator.sh sccript on SR first, then you need 
# to comment out everything up to the call of write_cards_ttCR.py (not included)
#
# To make this work we first have to:
# 	1. crate the following folders: 
#			- ttCRlimits_${Y}inclusive
#			- ttCRlimits_${Y}inclusive/SR/ETau
#			- ttCRlimits_${Y}inclusive/SR/MuTau
#			- ttCRlimits_${Y}inclusive/SR/TauTau
# 	2. launch the submitHistoFiller.py to produce the outplotters inside the folders just created (cfr. paths in the mainCfg)
#	3. set the correct path to the previously created CR datacard
#   4. launch this bash script
#
# This will create the .root files that then have to be passed to the write_cards script 


export Y="2017"
export CHANNELS="TauTau MuTau ETau"

for CH in $CHANNELS
do 
	cd ../ttCRlimits_${Y}inclusive/SR/${CH}
	hadd outPlotter.root outPlotter_*.root
	rm outPlotter_*.root
	cd - # return to ttSF
	cd .. # move to KLUB
	python scripts/combineFillerOutputs.py --dir ttCRlimits_${Y}inclusive/SR/${CH}
	cd ttSF
	python prepare_histos.py -f ../ttCRlimits_${Y}inclusive/SR/${CH}/analyzedOutPlotter.root -o SR -c ${CH} -y ${Y}
	mv -v analyzedOutPlotter_${Y}_${CH}_SR.root ../ttCRlimits_${Y}inclusive/SR/${CH}
	python write_cards_ttCR.py --filename ../ttCRlimits_${Y}inclusive/SR/${CH}/analyzedOutPlotter_${Y}_${CH}_SR.root --dir '' --channel ${CH} --year ${Y} --selection s2b0jresolvedMcut --binbybin 1 --shape 0 --theory --config ../ttCRlimits_${Y}inclusive/SR/${CH}/mainCfg_${CH}_Legacy${Y}.cfg
	python write_cards_ttCR.py --filename ../ttCRlimits_${Y}inclusive/SR/${CH}/analyzedOutPlotter_${Y}_${CH}_SR.root --dir '' --channel ${CH} --year ${Y} --selection s1b1jresolvedMcut --binbybin 1 --shape 0 --theory --config ../ttCRlimits_${Y}inclusive/SR/${CH}/mainCfg_${CH}_Legacy${Y}.cfg
done

mkdir cards_${Y}CR+SR
mkdir cards_${Y}CR+SR/s2b0jresolvedMcutDNNoutSM_kl_1
mkdir cards_${Y}CR+SR/s1b1jresolvedMcutDNNoutSM_kl_1

for CH in $CHANNELS
do
	mv -v cards_${CH}/s2b0jresolvedMcutDNNoutSM_kl_1/* cards_${Y}CR+SR/s2b0jresolvedMcutDNNoutSM_kl_1
	mv -v cards_${CH}/s1b1jresolvedMcutDNNoutSM_kl_1/* cards_${Y}CR+SR/s1b1jresolvedMcutDNNoutSM_kl_1
	rm -r cards_${CH}
done

cd cards_${Y}CR+SR/s2b0jresolvedMcutDNNoutSM_kl_1
combineCards.py hh_*.txt >> SR2b0j.txt
cd ../s1b1jresolvedMcutDNNoutSM_kl_1
combineCards.py hh_*.txt >> SR1b1j.txt
cd .. # return to cards_${Y}CR+SR
cp -vR ../../ttCRlimits_${Y}inclusive/cards_CR20bin/ttCR_invMcutDNNoutSM_kl_1 .  # HERE PUT THE CORRECT PATH TO THE CR DATACARDS PREVIOUSLY CREATED WITH CRlimits.sh WHEN DOING THE CR ONLY FIT
cd ttCR_invMcutDNNoutSM_kl_1
combineCards.py hh_*.txt >> CR.txt
cd .. # return to cards_${Y}CR+SR
combineCards.py CRttbar=ttCR_invMcutDNNoutSM_kl_1/CR.txt SR2b0j=s2b0jresolvedMcutDNNoutSM_kl_1/SR2b0j.txt SR1b1j=s1b1jresolvedMcutDNNoutSM_kl_1/SR1b1j.txt >> comb.txt

text2workspace.py comb.txt -o comb.root

combine -M FitDiagnostics comb.root --rMin 0 --rMax 1 --saveShapes --saveWithUncertainties


