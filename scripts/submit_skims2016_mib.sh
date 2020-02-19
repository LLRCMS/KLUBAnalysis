# LOG
OUTDIRR="Skims_Legacy2016_18Feb2020"

# INPUT
INPUTDIR="inputFiles/Legacy2016_backgrounds"
INPUTDIR_DATA="inputFiles/Legacy2016_data"
INPUTDIR_SIG="inputFiles/Legacy2016_signals"

# OUTPUT
SKIMDIR="/gwteraz/users/brivio/SKIMMED_Legacy2016_18Feb2020"

# PU weights
PUDIR="/gwpool/users/brivio/Hhh_1718/LegacyRun2/CMSSW_10_2_16/src/KLUBAnalysis/weights/PUreweight/Legacy_Run2_PU_SF/2016"

# Environment
source /cvmfs/cms.cern.ch/cmsset_default.sh
source scripts/setup.sh
mkdir $OUTDIRR


<<COMMENT1

#### GGH SM :
# XS:  0.03349 pb^-1
echo "Submitting - GGH SM - "
echo "Submitting - GGH SM - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 10 -k True -o $SKIMDIR/SKIM_GGHHSM -i $INPUTDIR_SIG/7_GluGluToHHTo2B2Tau_node_SM_13TeV-madgraph__RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2.txt -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt


#### VBF SM :
# XS: 0.001626 pb^-1 from XSBD
echo "Submitting - VBF SM - "
echo "Submitting - VBF SM - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 10   -k True -o $SKIMDIR/SKIM_VBFHHSM -i $INPUTDIR_SIG/9_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_13TeV-madgraph__RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2.txt -x 1. -a True -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt



###################
# TT
# TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
# hh = 45.7%
# ll = 10.5%
# hl = 21.9% (x2 for permutation t-tbar)
# technical note: use -q long if splitting on ~180-350 jobs

# TT had
echo "Submitting - TThad - "
echo "Submitting - TThad - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 100 -k True -o $SKIMDIR/SKIM_TT_fullyHad -i $INPUTDIR/TT_fullyHad.txt -x 377.96 -t True -b 1 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt



# TT lep
echo "Submitting - TTlep - "
echo "Submitting - TTlep - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 80 -k True -o $SKIMDIR/SKIM_TT_fullyLep -i $INPUTDIR/TT_fullyLep.txt -x 88.29 -t True -b 4 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

COMMENT1

# TT semi
echo "Submitting - TTsemi - "
echo "Submitting - TTsemi - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 80 -k True -o $SKIMDIR/SKIM_TT_semiLep -i $INPUTDIR/TT_semiLep.txt -x 365.34 -t True -b 5 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

<<COMMENT2


# # #####################
# Wjets
echo "Submitting - WJets - "
echo "Submitting - WJets - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_0_100     -i $INPUTDIR/WJetsToLNu_HT-0To100.txt    -y 1.213784 -x 48917.48 -z 100 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_100_200   -i $INPUTDIR/WJetsToLNu_HT-100To200.txt  -y 1.213784 -x 1345 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_200_400   -i $INPUTDIR/WJetsToLNu_HT-200To400.txt   -y 1.213784 -x 359.7 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_400_600   -i $INPUTDIR/WJetsToLNu_HT-400To600.txt   -y 1.213784 -x 48.91 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_600_800   -i $INPUTDIR/WJetsToLNu_HT-600To800.txt   -y 1.213784 -x 12.05 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_800_1200  -i $INPUTDIR/WJetsToLNu_HT-800To1200.txt  -y 1.213784 -x 5.501 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_1200_2500 -i $INPUTDIR/WJetsToLNu_HT-1200To2500.txt  -y 1.213784 -x 1.329 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_2500_Inf  -i $INPUTDIR/WJetsToLNu_HT-2500ToInf.txt  -y 1.213784 -x 0.03216 -Z 2500 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt



# # #####################
#### ELECTROWEAK
echo "Submitting - EWK - "
echo "Submitting - EWK - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25 -k True -o $SKIMDIR/SKIM_EWKWPlus2Jets_WToLNu   -i $INPUTDIR/EWKWPlus2Jets_WToLNu.txt   -x 25.62 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25 -k True -o $SKIMDIR/SKIM_EWKWMinus2Jets_WToLNu  -i $INPUTDIR/EWKWMinus2Jets_WToLNu.txt  -x 20.25 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25 -k True -o $SKIMDIR/SKIM_EWKZ2Jets_ZToLL        -i $INPUTDIR/EWKZ2Jets_ZToLL.txt        -x 3.987 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25 -k True -o $SKIMDIR/SKIM_EWKZ2Jets_ZToNuNu      -i $INPUTDIR/EWKZ2Jets_ZToNuNu.txt        -x 10.66 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt



# # #####################
#### Single Top
echo "Submitting - SingleTop - "
echo "Submitting - SingleTop - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25 -k True -o $SKIMDIR/SKIM_ST_tW_antitop       -i $INPUTDIR/ST_tW_antitop.txt         -x 34.97 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25 -k True -o $SKIMDIR/SKIM_ST_tW_top           -i $INPUTDIR/ST_tW_top.txt             -x 35.6 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25 -k True -o $SKIMDIR/SKIM_ST_tchannel_antitop -i $INPUTDIR/ST_t-channel_antitop.txt  -x 80.95 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25 -k True -o $SKIMDIR/SKIM_ST_tchannel_top     -i $INPUTDIR/ST_t-channel_top.txt      -x 136.02 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt



# # #####################
#### SM Higgs :
### HXSWG: xs(ZH) = 0.8839 pb
### Z->qq : 69.91% , Z->ll : 3,3658% (x3 for all the leptons)
### H->bb : 57.7%  , H->tautau : 6.32%
### ZH (Zll, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
### ZH (Zqq, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
### ZH (Zall, Htautau) : XS teor ZH * BR H->tautau

echo "Submitting - SM Higgs - "
echo "Submitting - SM Higgs - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ZH_HBB_ZLL -i $INPUTDIR/ZH_HToBB_ZToLL.txt -x 0.07924 -y 0.577 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

# Fixme ggZH --> XS=? ; -y=?
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ggZH_HToBB_ZToLL -i $INPUTDIR/ggZH_HToBB_ZToLL.txt -x 0.?? -y 0.?? -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

# Fixme ggZH --> XS=? ; -y=?
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ggZH_HToBB_ZToNuNu -i $INPUTDIR/ggZH_HToBB_ZToNuNu.txt -x 0.?? -y 0.?? -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ZH_HTauTau -i $INPUTDIR/ZHToTauTau.txt -x 0.8696 -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ggHTauTau -i $INPUTDIR/GluGluHToTauTau.txt -x 21.46 -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_VBFHTauTau -i $INPUTDIR/VBFHToTauTau.txt -x 3.861 -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_WplusHTauTau -i $INPUTDIR/WplusHToTauTau.txt -x 0.867 -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_WminusHTauTau -i $INPUTDIR/WminusHToTauTau.txt -x 0.5412 -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ttHJetToBB -i $INPUTDIR/ttHTobb.txt -x 0.507 -y 0.577 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ttHJetTononBB -i $INPUTDIR/ttHToNonbb.txt -x 0.507 -y 0.423 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt



#####################
### DI-boson:
#XS taken from here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson
#for Z->4Q, I scaled from ZZ->2nu2Q using BR(2Nu2Q) = 2 * 20% * 69.91%
echo "Submitting - DiBoson - "
echo "Submitting - DiBoson - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 30  -k True -o $SKIMDIR/SKIM_WW  -i $INPUTDIR/WW.txt -x 64.28 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 30  -k True -o $SKIMDIR/SKIM_WZ  -i $INPUTDIR/WZ.txt -x 23.48 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 30  -k True -o $SKIMDIR/SKIM_ZZ  -i $INPUTDIR/ZZ.txt -x 10.16 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt



#####################
### TRI-boson:
#XS taken from here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson
#for Z->4Q, I scaled from ZZ->2nu2Q using BR(2Nu2Q) = 2 * 20% * 69.91%
echo "Submitting - TriBoson - "
echo "Submitting - TriBoson - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_WWW   -i $INPUTDIR/WWW.txt -x 0.2086  -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_WZZ   -i $INPUTDIR/WZZ.txt -x 0.05565 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_WWZ   -i $INPUTDIR/WWZ.txt -x 0.1651 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt



# # #####################
#### Others :
echo "Submitting - Others - "
echo "Submitting - Others - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_TTWJetsToQQ -i $INPUTDIR/TTWJetsToQQ.txt -x 0.4062 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_TTWJetsToLNu -i $INPUTDIR/TTWJetsToLNu.txt -x 0.2043 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_TTWW -i $INPUTDIR/TTWW.txt -x 0.007834 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt



# # #####################
# DY
# XS: 6225.42 pb
echo "Submitting - DY LO - "
echo "Submitting - DY LO - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 230 -k True -o $SKIMDIR/SKIM_DY -i $INPUTDIR/DYmerged.txt -x 6225.42 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt -g True --DY True



######################
### DATA :
######################

echo "Submitting - DATA tau - "
echo "Submitting - DATA tau - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Tau_2016B -i $INPUTDIR_DATA/Tau_2.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Tau_2016C -i $INPUTDIR_DATA/Tau_3.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Tau_2016D -i $INPUTDIR_DATA/Tau_4.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Tau_2016E -i $INPUTDIR_DATA/Tau_5.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Tau_2016F -i $INPUTDIR_DATA/Tau_6.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Tau_2016G -i $INPUTDIR_DATA/Tau_7.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Tau_2016H -i $INPUTDIR_DATA/Tau_8.txt -q longcms


echo "Submitting - DATA Mu - "
echo "Submitting - DATA Mu - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 25 -k True -o $SKIMDIR/SKIM_Mu_2016B -i $INPUTDIR_DATA/Muon_2.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 25 -k True -o $SKIMDIR/SKIM_Mu_2016C -i $INPUTDIR_DATA/Muon_3.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 25 -k True -o $SKIMDIR/SKIM_Mu_2016D -i $INPUTDIR_DATA/Muon_4.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 25 -k True -o $SKIMDIR/SKIM_Mu_2016E -i $INPUTDIR_DATA/Muon_5.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 25 -k True -o $SKIMDIR/SKIM_Mu_2016F -i $INPUTDIR_DATA/Muon_6.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 25 -k True -o $SKIMDIR/SKIM_Mu_2016G -i $INPUTDIR_DATA/Muon_7.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 25 -k True -o $SKIMDIR/SKIM_Mu_2016H -i $INPUTDIR_DATA/Muon_8.txt -q longcms



echo "Submitting - DATA Electron - "
echo "Submitting - DATA Electron - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Ele_2016B -i $INPUTDIR_DATA/Electron_2.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Ele_2016C -i $INPUTDIR_DATA/Electron_3.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Ele_2016D -i $INPUTDIR_DATA/Electron_4.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Ele_2016E -i $INPUTDIR_DATA/Electron_5.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Ele_2016F -i $INPUTDIR_DATA/Electron_6.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Ele_2016G -i $INPUTDIR_DATA/Electron_7.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Ele_2016H -i $INPUTDIR_DATA/Electron_8.txt -q longcms





COMMENT2



