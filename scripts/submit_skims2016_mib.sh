# LOG
OUTDIRR="Skims_Legacy2016_1June2020"

# INPUT
INPUTDIR="inputFiles/Legacy2016_backgrounds"
INPUTDIR_DATA="inputFiles/Legacy2016_data"
INPUTDIR_SIG="inputFiles/Legacy2016_signals"

# OUTPUT
SKIMDIR="/gwteraz/users/brivio/SKIMMED_Legacy2016_1June2020"

# PU weights
PUDIR="/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/CMSSW_11_1_0_pre6/src/KLUBAnalysis/weights/PUreweight/Legacy_Run2_PU_SF/2016"

# Environment
source /cvmfs/cms.cern.ch/cmsset_default.sh
source scripts/setup.sh
mkdir $OUTDIRR

<<COMMENT1

###################
# TT
# XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
# TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
# hh = 45.7%
# ll = 10.5%
# hl = 21.9% (x2 for permutation t-tbar)
# TT had
echo "Submitting - TThad - "
echo "Submitting - TThad - " >> log_1June2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_1June2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 50 -k True -o $SKIMDIR/SKIM_TT_fullyHad -i $INPUTDIR/TTToHadronic.txt    -x 377.96 -t True -b 1 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

# TT lep
echo "Submitting - TTlep - "
echo "Submitting - TTlep - " >> log_1June2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_1June2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 50 -k True -o $SKIMDIR/SKIM_TT_fullyLep -i $INPUTDIR/TTTo2L2Nu.txt       -x 88.29 -t True -b 4 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt


# TT semi
echo "Submitting - TTsemi - "
echo "Submitting - TTsemi - " >> log_1June2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_1June2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 200 -k True -o $SKIMDIR/SKIM_TT_semiLep -i $INPUTDIR/TTToSemiLeptonic.txt -x 365.34 -t True -b 5 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt



# # #####################
# Wjets
# tot xs NNLO: 61526.7 pb
# tot xs LO : 50380 pb
# individual XS are taken from : https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#W_jets
# xs LO (HT < 100 ) = 50380 - sum (xs HT > 100) = 50380 pb -  3126.828 pb = 47253.172  pb
# Finally, everything is scaled from the LO to the NNLO xs : (61526.7/50380) = 1.221252481
echo "Submitting - WJets - "
echo "Submitting - WJets - " >> log_1June2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_1June2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_0_70      -i $INPUTDIR/WJetsToLNu.txt               -y 1.221252481 -x 47253.172 -z 70 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_70_100    -i $INPUTDIR/WJetsToLNu_HT-70To100.txt    -y 1.221252481 -x 1353  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_100_200   -i $INPUTDIR/WJetsToLNu_HT-100To200.txt   -y 1.221252481 -x 1346  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_200_400   -i $INPUTDIR/WJetsToLNu_HT-200To400.txt   -y 1.221252481 -x 360.1 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_400_600   -i $INPUTDIR/WJetsToLNu_HT-400To600.txt   -y 1.221252481 -x 48.8  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_600_800   -i $INPUTDIR/WJetsToLNu_HT-600To800.txt   -y 1.221252481 -x 12.07 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_800_1200  -i $INPUTDIR/WJetsToLNu_HT-800To1200.txt  -y 1.221252481 -x 5.497 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_1200_2500 -i $INPUTDIR/WJetsToLNu_HT-1200To2500.txt -y 1.221252481 -x 1.329 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_2500_Inf  -i $INPUTDIR/WJetsToLNu_HT-2500ToInf.txt  -y 1.221252481 -x 0.0321 -Z 2500 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt


# # #####################
#### ELECTROWEAK
# XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
echo "Submitting - EWK - "
echo "Submitting - EWK - " >> log_1June2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_1June2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25 -k True -o $SKIMDIR/SKIM_EWKWPlus2Jets_WToLNu  -i $INPUTDIR/EWKWPlus2Jets.txt  -x 25.62 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25 -k True -o $SKIMDIR/SKIM_EWKWMinus2Jets_WToLNu -i $INPUTDIR/EWKWMinus2Jets.txt -x 20.25 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25 -k True -o $SKIMDIR/SKIM_EWKZ2Jets_ZToLL       -i $INPUTDIR/EWKZ2Jets_ZToL.txt -x 3.987 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt


# # #####################
#### Single Top
# XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
echo "Submitting - SingleTop - "
echo "Submitting - SingleTop - " >> log_1June2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_1June2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25 -k True -o $SKIMDIR/SKIM_ST_tW_antitop        -i $INPUTDIR/ST_tW_antitop_5f.txt        -x 35.85   -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25 -k True -o $SKIMDIR/SKIM_ST_tW_top            -i $INPUTDIR/ST_tW_top_5f.txt            -x 35.85   -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25 -k True -o $SKIMDIR/SKIM_ST_t_channel_antitop -i $INPUTDIR/ST_t-channel_antitop_4f.txt -x 80.95  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25 -k True -o $SKIMDIR/SKIM_ST_t_channel_top     -i $INPUTDIR/ST_t-channel_top_4f.txt     -x 136.02 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt


# # #####################
#### SM Higgs :
### From https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNHLHE2019
### HXSWG: xs(ZH) = 0.880 pb
### HXSWG: xs(W+H) = 0.831 pb
### HXSWG: xs(W-H) = 0.527 pb
### HXSWG: xs(ggH) = 48.61 pb
### HXSWG: xs(VBFH) = 3.766 pb
### Z->qq : 69.91% , Z->ll : 3,3658% (x3 for all the leptons)
### H->bb : 57.7%  , H->tautau : 6.32%
### ZH (Zll, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
### ZH (Zqq, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
### ZH (Zall, Htautau) : XS teor ZH * BR H->tautau
echo "Submitting - SM Higgs - "
echo "Submitting - SM Higgs - " >> log_1June2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_1June2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ZH_HBB_ZLL    -i $INPUTDIR/ZH_HToBB_ZToLL.txt  -x 0.08781 -y 0.577  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ZH_HBB_ZQQ    -i $INPUTDIR/ZH_HToBB_ZToQQ.txt  -x 0.6079  -y 0.577  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ZH_HTauTau    -i $INPUTDIR/ZHToTauTau.txt      -x 0.880   -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_WplusHTauTau  -i $INPUTDIR/WplusHToTauTau.txt  -x 0.831   -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_WminusHTauTau -i $INPUTDIR/WminusHToTauTau.txt -x 0.527   -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ggHTauTau     -i $INPUTDIR/GluGluHToTauTau.txt -x 48.61   -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_VBFHTauTau    -i $INPUTDIR/VBFHToTauTau.txt    -x 3.766   -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt


#####################
### Multiboson:
# Some XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
# Some other XS taken from http://cms.cern.ch/iCMS/jsp/db_notes/noteInfo.jsp?cmsnoteid=CMS%20AN-2019/111
echo "Submitting - MultiBoson - "
echo "Submitting - MultiBoson - " >> log_1June2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_1June2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 35  -k True -o $SKIMDIR/SKIM_ZZTo4L      -i $INPUTDIR/ZZTo4L.txt      -x 1.26   -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25  -k True -o $SKIMDIR/SKIM_ZZTo4Q      -i $INPUTDIR/ZZTo4Q.txt      -x 6.912  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25  -k True -o $SKIMDIR/SKIM_ZZTo2L2Nu   -i $INPUTDIR/ZZTo2L2Nu.txt   -x 0.564  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 15  -k True -o $SKIMDIR/SKIM_ZZTo2L2Q    -i $INPUTDIR/ZZTo2L2Q.txt    -x 5.52   -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 15  -k True -o $SKIMDIR/SKIM_ZZTo2Q2Nu   -i $INPUTDIR/ZZTo2Q2Nu.txt   -x 4.07   -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 15  -k True -o $SKIMDIR/SKIM_WZTo3LNu    -i $INPUTDIR/WZTo3LNu.txt    -x 4.43   -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 15  -k True -o $SKIMDIR/SKIM_WZTo1L1Nu2Q -i $INPUTDIR/WZTo1L1Nu2Q.txt -x 10.71  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 15  -k True -o $SKIMDIR/SKIM_WZTo1L3Nu   -i $INPUTDIR/WZTo1L3Nu.txt   -x 3.06  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 15  -k True -o $SKIMDIR/SKIM_WZTo2L2Q    -i $INPUTDIR/WZTo2L2Q.txt    -x 5.595   -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 15  -k True -o $SKIMDIR/SKIM_WWTo2L2Nu   -i $INPUTDIR/WWTo2L2Nu.txt   -x 12.178  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 15  -k True -o $SKIMDIR/SKIM_WWToLNuQQ   -i $INPUTDIR/WWToLNuQQ.txt   -x 50.00  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 15  -k True -o $SKIMDIR/SKIM_WWTo4Q      -i $INPUTDIR/WWTo4Q.txt      -x 51.723  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_ZZZ         -i $INPUTDIR/ZZZ.txt         -x 0.0147 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_WZZ         -i $INPUTDIR/WZZ.txt         -x 0.057  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_WWW         -i $INPUTDIR/WWW.txt         -x 0.209  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_WWZ         -i $INPUTDIR/WWZ.txt         -x 0.168  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt


# # #####################
#### TTX :
# HXSWG: xs(ttH) = 0.5071 pb
echo "Submitting - TTX - "
echo "Submitting - TTX - " >> log_1June2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_1June2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_ttHJetTononBB -i $INPUTDIR/ttHToNonbb.txt   -x 0.5071 -y 0.423  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_ttHJetToBB    -i $INPUTDIR/ttHTobb.txt      -x 0.5071 -y 0.577  -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_TTWJetsToLNu  -i $INPUTDIR/TTWJetsToLNu.txt -x 0.2043    -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTWJetsToQQ   -i $INPUTDIR/TTWJetsToQQ.txt  -x 0.4062   -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_TTZToLLNuNu   -i $INPUTDIR/TTZToLLNuNu.txt  -x 0.2529    -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTWW          -i $INPUTDIR/TTWW.txt         -x 0.006979 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTWZ          -i $INPUTDIR/TTWZ.txt         -x 0.002441 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTZZ          -i $INPUTDIR/TTZZ.txt         -x 0.001386 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt



#### GGH SM :
# XS:  0.03349 pb
echo "Submitting - GGH SM - "
echo "Submitting - GGH SM - " >> log_1June2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_1June2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_GGHHSM -i $INPUTDIR_SIG/7_GluGluToHHTo2B2Tau_node_SM_13TeV-madgraph__RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2.txt -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt


COMMENT1


######################
#### Reweighting ggF non res - filelists up to date
echo "Submitting - GGF LO Reweight - "
echo "Submitting - GGF LO Reweight - " >> log_1June2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_1June2020.txt
### norm xs = 1 pb
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_HHRew_SM -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_LO_allNodes.txt --pu $PUDIR/PU_Legacy2016_SF.txt  -x 1.0  --kl 1.0  --kt 1.0 -a True

<<COMMENT2


#### VBF SM :
echo "Submitting - VBF SM - "
echo "Submitting - VBF SM - " >> log_1June2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_1June2020.txt

### norm xs = 1 pb
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1   -i $INPUTDIR_SIG/9_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_13TeV-madgraph__RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2.txt     -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_0_5_C2V_1_C3_1 -i $INPUTDIR_SIG/10_VBFHHTo2B2Tau_CV_0_5_C2V_1_C3_1_13TeV-madgraph__RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2.txt  -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_1_5_C2V_1_C3_1 -i $INPUTDIR_SIG/11_VBFHHTo2B2Tau_CV_1_5_C2V_1_C3_1_13TeV-madgraph__RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2.txt  -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_1_C3_0   -i $INPUTDIR_SIG/12_VBFHHTo2B2Tau_CV_1_C2V_1_C3_0_13TeV-madgraph__RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2.txt    -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_1_C3_2   -i $INPUTDIR_SIG/13_VBFHHTo2B2Tau_CV_1_C2V_1_C3_2_13TeV-madgraph__RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2.txt    -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_2_C3_1   -i $INPUTDIR_SIG/14_VBFHHTo2B2Tau_CV_1_C2V_2_C3_1_13TeV-madgraph__RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2.txt    -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt

### norm to theoretical xs 
# xs_theo(SM) = 0.001726
#  CV C2V C3 |  xs_MG5      * xs_theo(SM)/xs_MG5(SM)
#   1   1  1 |  0.001601 pb * 1.078076202             =   0.001726 pb
# 0.5   1  1 |  0.01009  pb * 1.078076202             =   0.010878 pb
# 1.5   1  1 |  0.06153  pb * 1.078076202             =   0.066334 pb
#   1   1  0 |  0.004259 pb * 1.078076202             =   0.004591 pb
#   1   1  2 |  0.001327 pb * 1.078076202             =   0.001431 pb
#   1   2  1 |  0.01335  pb * 1.078076202             =   0.014392 pb

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1   -i $INPUTDIR_SIG/9_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_13TeV-madgraph__RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2.txt     -x 0.001726 -a True -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_0_5_C2V_1_C3_1 -i $INPUTDIR_SIG/10_VBFHHTo2B2Tau_CV_0_5_C2V_1_C3_1_13TeV-madgraph__RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2.txt  -x 0.010878  -a True -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_1_5_C2V_1_C3_1 -i $INPUTDIR_SIG/11_VBFHHTo2B2Tau_CV_1_5_C2V_1_C3_1_13TeV-madgraph__RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2.txt  -x 0.066334  -a True -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_1_C3_0   -i $INPUTDIR_SIG/12_VBFHHTo2B2Tau_CV_1_C2V_1_C3_0_13TeV-madgraph__RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2.txt    -x 0.004591 -a True -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_1_C3_2   -i $INPUTDIR_SIG/13_VBFHHTo2B2Tau_CV_1_C2V_1_C3_2_13TeV-madgraph__RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2.txt    -x 0.001431 -a True -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_2_C3_1   -i $INPUTDIR_SIG/14_VBFHHTo2B2Tau_CV_1_C2V_2_C3_1_13TeV-madgraph__RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2.txt    -x 0.014392  -a True -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt



# # #####################
# DY
echo "Submitting - DY - "
echo "Submitting - DY - " >> log_1June2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_1June2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 100 -k True -o $SKIMDIR/SKIM_DY          -i $INPUTDIR/DYmerged.txt            -x 6077.22 -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt -g True --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_DY_Low_Mass -i $INPUTDIR/DYJetsToLL_M-10to50.txt -x 18610   -q longcms --pu $PUDIR/PU_Legacy2016_SF.txt



######################
### DATA :
######################
echo "Submitting - DATA tau - "
echo "Submitting - DATA tau - " >> log_1June2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_1June2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_2016B -i $INPUTDIR_DATA/Tau_2016B.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_2016C -i $INPUTDIR_DATA/Tau_2016C.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_2016D -i $INPUTDIR_DATA/Tau_2016D.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_2016E -i $INPUTDIR_DATA/Tau_2016E.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_2016F -i $INPUTDIR_DATA/Tau_2016F.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_2016G -i $INPUTDIR_DATA/Tau_2016G.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_2016H -i $INPUTDIR_DATA/Tau_2016H.txt -q longcms



echo "Submitting - DATA Mu - "
echo "Submitting - DATA Mu - " >> log_1June2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_1June2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 35 -k True -o $SKIMDIR/SKIM_Mu_2016B -i $INPUTDIR_DATA/Muon_2016B.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 35 -k True -o $SKIMDIR/SKIM_Mu_2016C -i $INPUTDIR_DATA/Muon_2016C.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 35 -k True -o $SKIMDIR/SKIM_Mu_2016D -i $INPUTDIR_DATA/Muon_2016D.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Mu_2016E -i $INPUTDIR_DATA/Muon_2016E.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Mu_2016F -i $INPUTDIR_DATA/Muon_2016F.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Mu_2016G -i $INPUTDIR_DATA/Muon_2016G.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Mu_2016H -i $INPUTDIR_DATA/Muon_2016H.txt -q longcms



echo "Submitting - DATA Electron - "
echo "Submitting - DATA Electron - " >> log_1June2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_1June2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 35 -k True -o $SKIMDIR/SKIM_Ele_2016B -i $INPUTDIR_DATA/Electron_2016B.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 35 -k True -o $SKIMDIR/SKIM_Ele_2016C -i $INPUTDIR_DATA/Electron_2016C.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 35 -k True -o $SKIMDIR/SKIM_Ele_2016D -i $INPUTDIR_DATA/Electron_2016D.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 35 -k True -o $SKIMDIR/SKIM_Ele_2016E -i $INPUTDIR_DATA/Electron_2016E.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 35 -k True -o $SKIMDIR/SKIM_Ele_2016F -i $INPUTDIR_DATA/Electron_2016F.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 35 -k True -o $SKIMDIR/SKIM_Ele_2016G -i $INPUTDIR_DATA/Electron_2016G.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 35 -k True -o $SKIMDIR/SKIM_Ele_2016H -i $INPUTDIR_DATA/Electron_2016H.txt -q longcms



COMMENT2
