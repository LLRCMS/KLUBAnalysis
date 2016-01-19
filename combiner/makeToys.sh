#!/bin/bash
# make cards with all vars/selections

export OUTSTRING="14Jan_alpha"
export BASEMUTAU="defaultBtagMMNoIsoBBTTCutKine90" #should be LL
export BASETAUTAU="defaultBtagLLNoIsoBBTTCut"
export BASEETAU="defaultBtagMMNoIsoBBTTCutKine90"
#export VARTAUTAU="HHKin_mass"
#export VARMUTAU="HHKin_mass"
#export VARETAU="HHKin_mass"
export VAR="HHKin_mass"
#export LAMBDAS="Lambdam4 Lambda1 Lambda2dot46 Lambda20"
export LAMBDAS="Lambda20"

#assume the ws was already done in a previous step
#text2workspace.py CardFiles_Mul10/realistic-counting-experiment_ST4500_BHMI.txt -o CardFiles_Mul10/ws_ST4500_BHMI.root
#for c in TauTau MuTau ETau
#	do
for i in $LAMBDAS 
	do	
	cd cards_MuTau_$OUTSTRING/${i}${BASEMUTAU}${VAR}
	pwd
	combine comb.root -M HybridNew -m 125.0 --rMin=1 --rMax=200 --freq --fullBToys -s -1 -T 1000 -i 5 --saveToys --saveHybridResult --clsAcc 0 -v9 --singlePoint=1 > r0005.log
	combine comb.root -M HybridNew -m 125.0 --rMin=1 --rMax=200 --freq --fullBToys -s -1 -T 1000 -i 5 --saveToys --saveHybridResult --clsAcc 0 -v9 --singlePoint=5 > r001.log
	combine comb.root -M HybridNew -m 125.0 --rMin=1 --rMax=200 --freq --fullBToys -s -1 -T 1000 -i 5 --saveToys --saveHybridResult --clsAcc 0 -v9 --singlePoint=10 > r003.log
	combine comb.root -M HybridNew -m 125.0 --rMin=1 --rMax=200 --freq --fullBToys -s -1 -T 1000 -i 5 --saveToys --saveHybridResult --clsAcc 0 -v9 --singlePoint=15 > r005.log
	combine comb.root -M HybridNew -m 125.0 --rMin=1 --rMax=200 --freq --fullBToys -s -1 -T 1000 -i 5 --saveToys --saveHybridResult --clsAcc 0 -v9 --singlePoint=20 > r1.log
	combine comb.root -M HybridNew -m 125.0 --rMin=1 --rMax=200 --freq --fullBToys -s -1 -T 1000 -i 5 --saveToys --saveHybridResult --clsAcc 0 -v9 --singlePoint=25 > r15.log
	combine comb.root -M HybridNew -m 125.0 --rMin=1 --rMax=200 --freq --fullBToys -s -1 -T 1000 -i 5 --saveToys --saveHybridResult --clsAcc 0 -v9 --singlePoint=30 > r2.log
	combine comb.root -M HybridNew -m 125.0 --rMin=1 --rMax=200 --freq --fullBToys -s -1 -T 1000 -i 5 --saveToys --saveHybridResult --clsAcc 0 -v9 --singlePoint=35 > r3.log
	combine comb.root -M HybridNew -m 125.0 --rMin=1 --rMax=200 --freq --fullBToys -s -1 -T 1000 -i 5 --saveToys --saveHybridResult --clsAcc 0 -v9 --singlePoint=40 > r4.log
	combine comb.root -M HybridNew -m 125.0 --rMin=1 --rMax=200 --freq --fullBToys -s -1 -T 1000 -i 5 --saveToys --saveHybridResult --clsAcc 0 -v9 --singlePoint=45 > r5.log
	combine comb.root -M HybridNew -m 125.0 --rMin=1 --rMax=200 --freq --fullBToys -s -1 -T 1000 -i 5 --saveToys --saveHybridResult --clsAcc 0 -v9 --singlePoint=50 > r7.log
	combine comb.root -M HybridNew -m 125.0 --rMin=1 --rMax=200 --freq --fullBToys -s -1 -T 1000 -i 5 --saveToys --saveHybridResult --clsAcc 0 -v9 --singlePoint=55 > r9.log
	combine comb.root -M HybridNew -m 125.0 --rMin=1 --rMax=200 --freq --fullBToys -s -1 -T 1000 -i 5 --saveToys --saveHybridResult --clsAcc 0 -v9 --singlePoint=60 > r11.log
	combine comb.root -M HybridNew -m 125.0 --rMin=1 --rMax=200 --freq --fullBToys -s -1 -T 1000 -i 5 --saveToys --saveHybridResult --clsAcc 0 -v9 --singlePoint=65 > r13.log
	combine comb.root -M HybridNew -m 125.0 --rMin=1 --rMax=200 --freq --fullBToys -s -1 -T 1000 -i 5 --saveToys --saveHybridResult --clsAcc 0 -v9 --singlePoint=70 > r15.log
	#mv higgsCombineTest.HybridNew.mH125*root CardFiles_Mul10/
	hadd grid.root higgsCombineTest.HybridNew.mH125*root
	#rm higgsCombineTest.HybridNew.mH125*root

	combine comb.root -M HybridNew --freq --grid=grid.root -v9 -m 125.0 --expectedFromGrid=0.025 > p0025.log
	combine comb.root -M HybridNew --freq --grid=grid.root -v9 -m 125.0 --expectedFromGrid=0.160 > p0016.log
	combine comb.root -M HybridNew --freq --grid=grid.root -v9 -m 125.0 --expectedFromGrid=0.500 > p0500.log
	combine comb.root -M HybridNew --freq --grid=grid.root -v9 -m 125.0 --expectedFromGrid=0.840 > p0840.log
	combine comb.root -M HybridNew --freq --grid=grid.root -v9 -m 125.0 --expectedFromGrid=0.975 > p0975.log

	#grep -h "@ 95% CL" p0025.log >> CardFiles_Mul10/realistic-counting-experiment_expected_4500_CL95.log
	#grep -h "@ 95% CL" p0016.log >> CardFiles_Mul10/realistic-counting-experiment_expected_4500_CL95.log
	#grep -h "@ 95% CL" p0500.log >> CardFiles_Mul10/realistic-counting-experiment_expected_4500_CL95.log
	#grep -h "@ 95% CL" p0840.log >> CardFiles_Mul10/realistic-counting-experiment_expected_4500_CL95.log
	#grep -h "@ 95% CL" p0975.log >> CardFiles_Mul10/realistic-counting-experiment_expected_4500_CL95.log
	#combine -M HybridNew --frequentist --testStat LHC CardFiles_Mul10/realistic-counting-experiment_ST4500_BHMI.txt -H ProfileLikelihood > CardFiles_Mul10/realistic-counting-experiment_observed_4500.log
	#grep "@ 95% CL" CardFiles_Mul10/realistic-counting-experiment_observed_4500.log > CardFiles_Mul10/realistic-counting-experiment_observed_4500_CL95.log
	#rm roostats-*
	cd -
done





