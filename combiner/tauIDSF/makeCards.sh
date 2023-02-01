### same decay mode
#taking the QCD already computed, without making the fit
python makeCards.py --tag 7Feb2019_decaymodes_noSF --decay bothDM0 --sel baselineHTauTau_bothDM0_DY --qcd True
python makeCards.py --tag 7Feb2019_decaymodes_noSF --decay bothDM1 --sel baselineHTauTau_bothDM1_DY --qcd True
python makeCards.py --tag 7Feb2019_decaymodes_noSF --decay bothDM10 --sel baselineHTauTau_bothDM10_DY --qcd True
### output is dataset_QCD.txt

#computing also the QCD
python makeCards.py --tag 7Feb2019_decaymodes_noSF --decay bothDM0 --sel baselineHTauTau_bothDM0_DY
python makeCards.py --tag 7Feb2019_decaymodes_noSF --decay bothDM1 --sel baselineHTauTau_bothDM1_DY
python makeCards.py --tag 7Feb2019_decaymodes_noSF --decay bothDM10 --sel baselineHTauTau_bothDM10_DY
### output is combined.txt



### different decay modes
#taking the QCD already computed, without making the fit
python makeCards.py --tag 7Feb2019_decaymodes_noSF --decay cross_DM0_DM1 --sel baselineHTauTau_tau1_DM0_tau2_DM1_DY --qcd True
python makeCards.py --tag 7Feb2019_decaymodes_noSF --decay cross_DM1_DM0 --sel baselineHTauTau_tau1_DM1_tau2_DM0_DY --qcd True
cd 7Feb2019_decaymodes_noSF
combineCards.py cross_DM0_DM1=baselineHTauTau_tau1_DM0_tau2_DM1_DY/datacard_QCD.txt cross_DM1_DM0=baselineHTauTau_tau1_DM1_tau2_DM0_DY/datacard_QCD.txt > dataset_QCD_DM0_DM1.txt
cd -

### output is dataset_QCD.txt


### output is combined.txt

