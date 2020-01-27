# LOG
OUTDIRR="Skims_Legacy2016_23Jan2020"

# INPUT
INPUTDIR="inputFiles/Legacy2016_backgrounds"
INPUTDIR_DATA="inputFiles/Legacy2016_data"

# OUTPUT
SKIMDIR="/gwteraz/users/brivio/SKIMMED_Legacy2016_23Jan2020"

# PU weights
PUDIR="/gwpool/users/brivio/Hhh_1718/LegacyRun2/CMSSW_10_2_16/src/KLUBAnalysis/weights/PUreweight/Legacy_Run2_PU_SF/2016"

# Environment
source /cvmfs/cms.cern.ch/cmsset_default.sh
source scripts/setup.sh
mkdir $OUTDIRR

<<COMMENT1

###################
# TT
# TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
# hh = 45.7%
# ll = 10.5%
# hl = 21.9% (x2 for permutation t-tbar)
# technical note: use -q long if splitting on ~180-350 jobs

# TT had
echo "Submitting - TThad - "
echo "Submitting - TThad - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 80 -k True -o $SKIMDIR/SKIM_TT_fullyHad -i $INPUTDIR/TT_fullyHad.txt -x 377.96 -t True -b 1 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt


# TT lep
echo "Submitting - TTlep - "
echo "Submitting - TTlep - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 80 -k True -o $SKIMDIR/SKIM_TT_fullyLep -i $INPUTDIR/TT_fullyLep.txt -x 88.29 -t True -b 4 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

COMMENT1

# TT semi
echo "Submitting - TTsemi - "
echo "Submitting - TTsemi - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 80 -k True -o $SKIMDIR/SKIM_TT_semiLep -i $INPUTDIR/TT_semiLep.txt -x 365.34 -t True -b 5 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

<<COMMENT2


# # #####################
# Wjets
echo "Submitting - WJets - "
echo "Submitting - WJets - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

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
echo "Submitting - EWK - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_EWKWPlus2Jets_WToLNu   -i $INPUTDIR/EWKWPlus2Jets_WToLNu.txt   -x 25.62 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_EWKWMinus2Jets_WToLNu  -i $INPUTDIR/EWKWMinus2Jets_WToLNu.txt  -x 20.25 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_EWKZ2Jets_ZToLL        -i $INPUTDIR/EWKZ2Jets_ZToLL.txt        -x 3.987 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_EWKZ2Jets_ZToNuNu      -i $INPUTDIR/EWKZ2Jets_ZToNuNu.txt        -x 10.66 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt



# # #####################
#### Single Top
echo "Submitting - SingleTop - "
echo "Submitting - SingleTop - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_ST_tW_antitop       -i $INPUTDIR/ST_tW_antitop.txt   -x 34.97 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_ST_tW_top           -i $INPUTDIR/ST_tW_top.txt       -x 35.6 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_ST_tchannel_antitop -i $INPUTDIR/ST_t-channel_antitop.txt  -x 80.95 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_ST_tchannel_top       -i $INPUTDIR/ST_t-channel_top.txt    -x 136.02 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt



# # #####################
#### SM Higgs :
### HXSWG: xs(ZH) = 0.8839 pb
### Z->qq : 69.91% , Z->ll : 3,3658% (x3 for all the leptons)
### H->bb : 57.7%  , H->tautau : 6.32%
### ZH (Zll, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
### ZH (Zqq, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
### ZH (Zall, Htautau) : XS teor ZH * BR H->tautau

echo "Submitting - SM Higgs - "
echo "Submitting - SM Higgs - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

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
echo "Submitting - DiBoson - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_WW  -i $INPUTDIR/WW.txt -x 64.28 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_WZ  -i $INPUTDIR/WZ.txt -x 23.48 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_ZZ  -i $INPUTDIR/ZZ.txt -x 10.16 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt



#####################
### TRI-boson:
#XS taken from here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson
#for Z->4Q, I scaled from ZZ->2nu2Q using BR(2Nu2Q) = 2 * 20% * 69.91%
echo "Submitting - TriBoson - "
echo "Submitting - TriBoson - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_WWW   -i $INPUTDIR/WWW.txt -x 0.2086  -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_WZZ   -i $INPUTDIR/WZZ.txt -x 0.05565 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_WWZ   -i $INPUTDIR/WWZ.txt -x 0.1651 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt



# # #####################
#### Others :
echo "Submitting - Others - "
echo "Submitting - Others - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTWJetsToQQ -i $INPUTDIR/TTWJetsToQQ.txt -x 0.4062 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTWJetsToLNu -i $INPUTDIR/TTWJetsToLNu.txt -x 0.2043 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTWW -i $INPUTDIR/TTWW.txt -x 0.007834 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt



# # #####################
# DY
# XS: 6225.42 pb
echo "Submitting - DY LO - "
echo "Submitting - DY LO - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 100 -k True -o $SKIMDIR/SKIM_DY -i $INPUTDIR/DYmerged.txt -x 6225.42 -q shortcms --pu $PUDIR/PU_Legacy2016_SF.txt -g True




######################
### DATA :
######################

echo "Submitting - DATA tau - "
echo "Submitting - DATA tau - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Tau_2016B -i $INPUTDIR_DATA/Tau_2.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Tau_2016C -i $INPUTDIR_DATA/Tau_3.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Tau_2016D -i $INPUTDIR_DATA/Tau_4.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Tau_2016E -i $INPUTDIR_DATA/Tau_5.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Tau_2016F -i $INPUTDIR_DATA/Tau_6.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Tau_2016G -i $INPUTDIR_DATA/Tau_7.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Tau_2016H -i $INPUTDIR_DATA/Tau_8.txt -q longcms


echo "Submitting - DATA Mu - "
echo "Submitting - DATA Mu - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Mu_2016B -i $INPUTDIR_DATA/Muon_2.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Mu_2016C -i $INPUTDIR_DATA/Muon_3.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Mu_2016D -i $INPUTDIR_DATA/Muon_4.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Mu_2016E -i $INPUTDIR_DATA/Muon_5.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Mu_2016F -i $INPUTDIR_DATA/Muon_6.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Mu_2016G -i $INPUTDIR_DATA/Muon_7.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Mu_2016H -i $INPUTDIR_DATA/Muon_8.txt -q longcms


echo "Submitting - DATA Electron - "
echo "Submitting - DATA Electron - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Ele_2016B -i $INPUTDIR_DATA/Electron_2.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Ele_2016C -i $INPUTDIR_DATA/Electron_3.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Ele_2016D -i $INPUTDIR_DATA/Electron_4.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Ele_2016E -i $INPUTDIR_DATA/Electron_5.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Ele_2016F -i $INPUTDIR_DATA/Electron_6.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Ele_2016G -i $INPUTDIR_DATA/Electron_7.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_Ele_2016H -i $INPUTDIR_DATA/Electron_8.txt -q longcms




#################################################################################################################
############################################### OLD SKIM COMMANDS ###############################################
#################################################################################################################


### DATA :
echo "Submitting - DATA tau - "
echo "Submitting - DATA tau - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2018_sync_mib.cfg -n 13 -k True -o $SKIMDIR/SKIM_Tau_2018B -i $INPUTDIR/1_Tau__Run2018B-17Sep2018-v1.txt -q longcms

### DATA :
echo "Submitting - DATA Mu - "
echo "Submitting - DATA Mu - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2018_sync_mib.cfg -n 13 -k True -o $SKIMDIR/SKIM_Mu_2018B -i $INPUTDIR/2_SingleMuon__Run2018B-17Sep2018-v1.txt -q longcms

echo "Submitting - DATA Electron - "
echo "Submitting - DATA Electron - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2018_sync_mib.cfg -n 13 -k True -o $SKIMDIR/SKIM_EGamma_2018B -i $INPUTDIR/1_EGamma__Run2018B-17Sep2018-v1.txt -q longcms




# #####################
# DY NLO
# total XS= 6225.42 pb - 8 october 2018
echo "Submitting - DY NLO - "
echo "Submitting - DY NLO - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2018_sync_mib.cfg -n 13 -k True -o $SKIMDIR/SKIM_DY_NLO_NewSF_Huge -i $INPUTDIR/1_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1-All.txt -x 6225.42 -q longcms --pu $PUDIR/2018_PuReWeight_SF.txt  --DY True

# # #####################
# DY
echo "Submitting - DY LO - "
echo "Submitting - DY LO - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

#old stayle DY (no PU sample-by-sample, no LOtoNLOreweight, no split in nB and nJets)
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 40 -k True -o $SKIMDIR/SKIM_DY_weights24Sep2018 -i $INPUTDIR/DYJets.txt -g True -x 5765.4  -q shortcms

#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 15 -k True -o $SKIMDIR/SKIM_DY -i $INPUTDIR/DYJets.txt -g True -x 5765.4  -q longcms

#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_DYJetsToLL_XS -i $INPUTDIR/DYJetsToLL.txt -g True -x 5765.4  -q longcms --pu $PUDIR/DYJetsToLL.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_DY1JetsToLL_XS -i $INPUTDIR/3_DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1.txt -g True -x 877.8  -q longcms --pu $PUDIR/3_DY1JetsToLL.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 6 -k True -o $SKIMDIR/SKIM_DY2JetsToLL_XS -i $INPUTDIR/DY2JetsToLL.txt -g True -x 304.4  -q shortcms --pu $PUDIR/DY2JetsToLL.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 6 -k True -o $SKIMDIR/SKIM_DY3JetsToLL_XS -i $INPUTDIR/6_DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1.txt -g True -x 111.5  -q shortcms --pu $PUDIR/6_DY3JetsToLL.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 6 -k True -o $SKIMDIR/SKIM_DY4JetsToLL_XS -i $INPUTDIR/7_DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -g True -x 44.03  -q shortcms --pu $PUDIR/7_DY4JetsToLL.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 6 -k True -o $SKIMDIR/SKIM_DYBBJetsToLL_XS -i $INPUTDIR/8_DYBBJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -g True -x 14.49  -q shortcms --pu $PUDIR/8_DYBBJetsToLL.txt

# myPUHisto_0j0b :  72169268.0  - frac:  0.495938761847     - XS:  2859.28533755
# myPUHisto_1j0b :  48082351.0  - frac:  0.330416287742     - XS:  1904.98206535
# myPUHisto_1j1b :  2013682.0   - frac:  0.0138377869904    - XS:  79.7803771142
# myPUHisto_2j0b :  13996883.0  - frac:  0.0961849415563    - XS:  554.544662049
# myPUHisto_2j1b :  726473.0    - frac:  0.00499223741795   - XS:  28.7822456094
# myPUHisto_2j2b :  1440197.0   - frac:  0.00989686520024   - XS:  57.0593866255
# myPUHisto_3j0b :  2899365.0   - frac:  0.0199240968918    - XS:  114.87038822
# myPUHisto_3j1b :  119898.0    - frac:  0.000823925021215  - XS:  4.75025731731
# myPUHisto_3j2b :  1035377.0   - frac:  0.00711498954687   - XS:  41.0207607335
# myPUHisto_3j3b :  11733.0     - frac:  8.06278025815e-05  - XS:  0.464851533003
# myPUHisto_4j0b :  2129640.0   - frac:  0.0146346367928    - XS:  84.3745349651
# myPUHisto_4j1b :  70735.0     - frac:  0.000486082640041  - XS:  2.80246085289
# myPUHisto_4j2b :  809829.0    - frac:  0.00556505009263   - XS:  32.084739804
# myPUHisto_4j3b :  11952.0     - frac:  8.21327449462e-05  - XS:  0.473528127713
# myPUHisto_4j4b :  3140.0      - frac:  2.1577712444e-05   - XS:  0.124404143325
# total XS= 5765.4 pb
# myPUHisto_0j0b : xsFrac: 0.697167 - xs: 4019.4 pb
# myPUHisto_1j0b : xsFrac: 0.196537 - xs: 1133.1 pb
# myPUHisto_1j1b : xsFrac: 0.007292 - xs: 42.04  pb
# myPUHisto_2j0b : xsFrac: 0.063167 - xs: 364.18 pb
# myPUHisto_2j1b : xsFrac: 0.003200 - xs: 18.5   pb
# myPUHisto_2j2b : xsFrac: 0.001070 - xs: 6.17   pb
# myPUHisto_3j0b : xsFrac: 0.018860 - xs: 108.73 pb
# myPUHisto_3j1b : xsFrac: 0.000887 - xs: 5.11   pb
# myPUHisto_3j2b : xsFrac: 0.000704 - xs: 4.06   pb
# myPUHisto_3j3b : xsFrac: 9.78e-06 - xs: 0.056  pb
# myPUHisto_4j0b : xsFrac: 0.010130 - xs: 58.40  pb
# myPUHisto_4j1b : xsFrac: 0.000425 - xs: 2.45   pb
# myPUHisto_4j2b : xsFrac: 0.000535 - xs: 3.08   pb
# myPUHisto_4j3b : xsFrac: 1.12e-05 - xs: 0.064  pb
# myPUHisto_4j4b : xsFrac: 1.98e-06 - xs: 0.011  pb
# total XS= 6233.55 pb - 8 october 2018
# myPUHisto_0j0b : xsFrac: 0.746053    - xs: 4650.56 pb
# myPUHisto_1j0b : xsFrac: 0.157576    - xs: 982.26 pb
# myPUHisto_1j1b : xsFrac: 0.00658535  - xs: 41.05 pb
# myPUHisto_2j0b : xsFrac: 0.0534837   - xs: 333.39 pb
# myPUHisto_2j1b : xsFrac: 0.00273262  - xs: 17.03 pb
# myPUHisto_2j2b : xsFrac: 0.00118358  - xs: 7.38 pb
# myPUHisto_3j0b : xsFrac: 0.0188891   - xs: 117.75 pb
# myPUHisto_3j1b : xsFrac: 0.000781306 - xs: 4.87 pb
# myPUHisto_3j2b : xsFrac: 0.000881659 - xs: 5.50 pb
# myPUHisto_3j3b : xsFrac: 9.87433E-06 - xs: 0.061 pb
# myPUHisto_4j0b : xsFrac: 0.0106378   - xs: 66.31 pb
# myPUHisto_4j1b : xsFrac: 0.000360878 - xs: 2.25 pb
# myPUHisto_4j2b : xsFrac: 0.000811201 - xs: 5.06 pb
# myPUHisto_4j3b : xsFrac: 1.0322E-05  - xs: 0.064 pb
# myPUHisto_4j4b : xsFrac: 2.77862E-05 - xs: 0.17 pb
# total XS= 6225.42 pb - 8 october 2018
# myPUHisto_0j0b : xsFrac: 0.746053    - xs: 4644.49 pb
# myPUHisto_1j0b : xsFrac: 0.157576    - xs: 980.98  pb
# myPUHisto_1j1b : xsFrac: 0.00658535  - xs: 41.00   pb
# myPUHisto_2j0b : xsFrac: 0.0534837   - xs: 332.96  pb
# myPUHisto_2j1b : xsFrac: 0.00273262  - xs: 17.01   pb
# myPUHisto_2j2b : xsFrac: 0.00118358  - xs: 7.37    pb
# myPUHisto_3j0b : xsFrac: 0.0188891   - xs: 117.59  pb
# myPUHisto_3j1b : xsFrac: 0.000781306 - xs: 4.86    pb
# myPUHisto_3j2b : xsFrac: 0.000881659 - xs: 5.49    pb
# myPUHisto_3j3b : xsFrac: 9.87433E-06 - xs: 0.06    pb
# myPUHisto_4j0b : xsFrac: 0.0106378   - xs: 66.22   pb
# myPUHisto_4j1b : xsFrac: 0.000360878 - xs: 2.25    pb
# myPUHisto_4j2b : xsFrac: 0.000811201 - xs: 5.05    pb
# myPUHisto_4j3b : xsFrac: 1.0322E-05  - xs: 0.06    pb
# myPUHisto_4j4b : xsFrac: 2.77862E-05 - xs: 0.17    pb
# total XS= 6225.42 pb - 5 march 2019
# myPUHisto_0j0b : xsFrac: 0.746231    - xs: 4645.60 pb
# myPUHisto_1j0b : xsFrac: 0.157487    - xs: 980.42  pb
# myPUHisto_1j1b : xsFrac: 0.00659452  - xs: 41.05   pb
# myPUHisto_2j0b : xsFrac: 0.0532405   - xs: 331.44  pb
# myPUHisto_2j1b : xsFrac: 0.0027623   - xs: 17.20   pb
# myPUHisto_2j2b : xsFrac: 0.00114651  - xs: 7.14    pb
# myPUHisto_3j0b : xsFrac: 0.0190577   - xs: 118.64  pb
# myPUHisto_3j1b : xsFrac: 0.000788278 - xs: 4.91    pb
# myPUHisto_3j2b : xsFrac: 0.000889527 - xs: 5.54    pb
# myPUHisto_3j3b : xsFrac: 9.96244E-06 - xs: 0.062   pb
# myPUHisto_4j0b : xsFrac: 0.010611    - xs: 66.06   pb
# myPUHisto_4j1b : xsFrac: 0.000359969 - xs: 2.24    pb
# myPUHisto_4j2b : xsFrac: 0.000809156 - xs: 5.04    pb
# myPUHisto_4j3b : xsFrac: 1.0296E-05  - xs: 0.064   pb
# myPUHisto_4j4b : xsFrac: 2.77162E-06 - xs: 0.017   pb

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 11 -k False -o $SKIMDIR/SKIM_DYJets_0j0b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 4645.60  -q longcms --pu $PUDIR/DY_special/DY_0j0b.txt --nj 0 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 11 -k False -o $SKIMDIR/SKIM_DYJets_1j0b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 980.42  -q longcms --pu $PUDIR/DY_special/DY_1j0b.txt --nj 1 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 3 -k False -o $SKIMDIR/SKIM_DYJets_1j1b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 41.05  -q longcms --pu $PUDIR/DY_special/DY_1j1b.txt --nj 1 --nb 1 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 20 -k False -o $SKIMDIR/SKIM_DYJets_2j0b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 331.44  -q shortcms --pu $PUDIR/DY_special/DY_2j0b.txt --nj 2 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 3 -k False -o $SKIMDIR/SKIM_DYJets_2j1b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 17.20  -q shortcms --pu $PUDIR/DY_special/DY_2j1b.txt --nj 2 --nb 1 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 3 -k False -o $SKIMDIR/SKIM_DYJets_2j2b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 7.14  -q shortcms --pu $PUDIR/DY_special/DY_2j2b.txt --nj 2 --nb 2 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_3j0b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 118.64  -q longcms --pu $PUDIR/DY_special/DY_3j0b.txt --nj 3 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_3j1b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 4.91  -q longcms --pu $PUDIR/DY_special/DY_3j1b.txt --nj 3 --nb 1 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_3j2b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 5.54  -q longcms --pu $PUDIR/DY_special/DY_3j2b.txt --nj 3 --nb 2 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_3j3b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 0.062  -q longcms --pu $PUDIR/DY_special/DY_3j3b.txt --nj 3 --nb 3 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_4j0b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 66.06  -q longcms --pu $PUDIR/DY_special/DY_4j0b.txt --nj 4 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_4j1b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 2.24  -q longcms --pu $PUDIR/DY_special/DY_4j1b.txt --nj 4 --nb 1 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_4j2b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 5.04  -q longcms --pu $PUDIR/DY_special/DY_4j2b.txt --nj 4 --nb 2 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_4j3b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 0.064  -q longcms --pu $PUDIR/DY_special/DY_4j3b.txt --nj 4 --nb 3 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_4j4b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 0.017  -q longcms --pu $PUDIR/DY_special/DY_4j4b.txt --nj 4 --nb 4 --DY True




COMMENT2



