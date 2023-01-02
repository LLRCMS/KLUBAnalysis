# LOG
OUTDIRR="SKIMS_Legacy2016_24Nov2022_EFT"
#OUTDIRR="SKIMS_Legacy2016_24Nov2022_EFT_JERup"
#OUTDIRR="SKIMS_Legacy2016_24Nov2022_EFT_JERdown"

# INPUT
INPUTDIR="inputFiles/Legacy2016_backgrounds"
INPUTDIR_DATA="inputFiles/Legacy2016_data"
INPUTDIR_SIG="inputFiles/Legacy2016_signals"

# OUTPUT
SKIMDIR="/gwteraz/users/brivio/SKIMS_Legacy2016_24Nov2022_EFT"
#SKIMDIR="/gwteraz/users/brivio/SKIMS_Legacy2016_24Nov2022_EFT_JERup"
#SKIMDIR="/gwteraz/users/brivio/SKIMS_Legacy2016_24Nov2022_EFT_JERdown"

# PU weights
PUDIR="/gwpool/users/brivio/Hhh_1718/LegacyRun2/May2020/LIMdev/CMSSW_11_1_0_pre6/src/KLUBAnalysis/weights/PUreweight/Legacy_Run2_PU_SF/2016"

# Environment
source /cvmfs/cms.cern.ch/cmsset_default.sh
source scripts/setup.sh
mkdir $OUTDIRR

<<COMMENT1

# # #####################
#### Single Top
# XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
echo "Submitting - SingleTop - "
echo "Submitting - SingleTop - " >> log_29May2021.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_29May2021.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 30 -k True -o $SKIMDIR/SKIM_ST_tW_antitop        -i $INPUTDIR/ST_tW_antitop_5f.txt        -x 35.85   --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 30 -k True -o $SKIMDIR/SKIM_ST_tW_top            -i $INPUTDIR/ST_tW_top_5f.txt            -x 35.85   --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 30 -k True -o $SKIMDIR/SKIM_ST_t_channel_antitop -i $INPUTDIR/ST_t-channel_antitop_4f.txt -x 80.95   --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 30 -k True -o $SKIMDIR/SKIM_ST_t_channel_top     -i $INPUTDIR/ST_t-channel_top_4f.txt     -x 136.02  --pu $PUDIR/PU_Legacy2016_SF.txt

###################
# TT
# XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
# TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
# hh = 45.7%
# ll = 10.5%
# hl = 21.9% (x2 for permutation t-tbar)

# TT had
echo "Submitting - TThad - "
echo "Submitting - TThad - " >> log_29May2021.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_29May2021.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 50 -k True -o $SKIMDIR/SKIM_TT_fullyHad -i $INPUTDIR/TTToHadronic.txt    -x 377.96 -t True -b 1  --pu $PUDIR/PU_Legacy2016_SF.txt

# TT semi
echo "Submitting - TTsemi - "
echo "Submitting - TTsemi - " >> log_29May2021.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_29May2021.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 150 -k True -o $SKIMDIR/SKIM_TT_semiLep -i $INPUTDIR/TTToSemiLeptonic.txt -x 365.34 -t True -b 5  --pu $PUDIR/PU_Legacy2016_SF.txt

# TT lep
echo "Submitting - TTlep - "
echo "Submitting - TTlep - " >> log_29May2021.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_29May2021.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 150 -k True -o $SKIMDIR/SKIM_TT_fullyLep -i $INPUTDIR/TTTo2L2Nu.txt       -x 88.29 -t True -b 4  --pu $PUDIR/PU_Legacy2016_SF.txt


# # #####################
#### DY Low Mass
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_DY_Low_Mass  -i $INPUTDIR/DYJetsToLL_M-10to50.txt -x 18610    --pu $PUDIR/PU_Legacy2016_SF.txt

# # #####################
#### ELECTROWEAK
# XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
echo "Submitting - EWK - "
echo "Submitting - EWK - " >> log_29May2021.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_29May2021.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 30 -k True -o $SKIMDIR/SKIM_EWKWPlus2Jets_WToLNu  -i $INPUTDIR/EWKWPlus2Jets.txt  -x 25.62  --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 30 -k True -o $SKIMDIR/SKIM_EWKWMinus2Jets_WToLNu -i $INPUTDIR/EWKWMinus2Jets.txt -x 20.25  --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 30 -k True -o $SKIMDIR/SKIM_EWKZ2Jets_ZToLL       -i $INPUTDIR/EWKZ2Jets_ZToL.txt -x 3.987  --pu $PUDIR/PU_Legacy2016_SF.txt


# # #####################
# Wjets
# tot xs NNLO: 61526.7 pb
# tot xs LO : 50380 pb
# individual XS are taken from : https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#W_jets
# xs LO (HT < 100 ) = 50380 - sum (xs HT > 100) = 50380 pb -  3126.828 pb = 47253.172  pb
# Finally, everything is scaled from the LO to the NNLO xs : (61526.7/50380) = 1.221252481
echo "Submitting - WJets - "
echo "Submitting - WJets - " >> log_29May2021.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_29May2021.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_0_70      -i $INPUTDIR/WJetsToLNu.txt               -y 1.221252481 -x 47253.172 -z 70  --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_70_100    -i $INPUTDIR/WJetsToLNu_HT-70To100.txt    -y 1.221252481 -x 1353             --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_100_200   -i $INPUTDIR/WJetsToLNu_HT-100To200.txt   -y 1.221252481 -x 1346             --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_200_400   -i $INPUTDIR/WJetsToLNu_HT-200To400.txt   -y 1.221252481 -x 360.1           --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_400_600   -i $INPUTDIR/WJetsToLNu_HT-400To600.txt   -y 1.221252481 -x 48.8            --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_600_800   -i $INPUTDIR/WJetsToLNu_HT-600To800.txt   -y 1.221252481 -x 12.07           --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_800_1200  -i $INPUTDIR/WJetsToLNu_HT-800To1200.txt  -y 1.221252481 -x 5.497           --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_1200_2500 -i $INPUTDIR/WJetsToLNu_HT-1200To2500.txt -y 1.221252481 -x 1.329           --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_2500_Inf  -i $INPUTDIR/WJetsToLNu_HT-2500ToInf.txt  -y 1.221252481 -x 0.0321 -Z 2500  --pu $PUDIR/PU_Legacy2016_SF.txt


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
echo "Submitting - SM Higgs - " >> log_29May2021.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_29May2021.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_ZH_HBB_ZLL    -i $INPUTDIR/ZH_HToBB_ZToLL.txt  -x 0.08781 -y 0.577   --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_ZH_HBB_ZQQ    -i $INPUTDIR/ZH_HToBB_ZToQQ.txt  -x 0.6079  -y 0.577   --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_ZH_HTauTau    -i $INPUTDIR/ZHToTauTau.txt      -x 0.880   -y 0.0632  --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WplusHTauTau  -i $INPUTDIR/WplusHToTauTau.txt  -x 0.831   -y 0.0632  --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WminusHTauTau -i $INPUTDIR/WminusHToTauTau.txt -x 0.527   -y 0.0632  --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_ggHTauTau     -i $INPUTDIR/GluGluHToTauTau.txt -x 48.61   -y 0.0632  --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_VBFHTauTau    -i $INPUTDIR/VBFHToTauTau.txt    -x 3.766   -y 0.0632  --pu $PUDIR/PU_Legacy2016_SF.txt


#####################
### Multiboson:
# Some XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
# Some other XS taken from http://cms.cern.ch/iCMS/jsp/db_notes/noteInfo.jsp?cmsnoteid=CMS%20AN-2019/111
echo "Submitting - MultiBoson - "
echo "Submitting - MultiBoson - " >> log_29May2021.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_29May2021.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 40  -k True -o $SKIMDIR/SKIM_ZZTo4L      -i $INPUTDIR/ZZTo4L.txt      -x 1.26   --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25  -k True -o $SKIMDIR/SKIM_ZZTo4Q      -i $INPUTDIR/ZZTo4Q.txt      -x 6.912  --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25  -k True -o $SKIMDIR/SKIM_ZZTo2L2Nu   -i $INPUTDIR/ZZTo2L2Nu.txt   -x 0.564  --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 40  -k True -o $SKIMDIR/SKIM_ZZTo2L2Q    -i $INPUTDIR/ZZTo2L2Q.txt    -x 5.52   --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 15  -k True -o $SKIMDIR/SKIM_ZZTo2Q2Nu   -i $INPUTDIR/ZZTo2Q2Nu.txt   -x 4.07   --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 15  -k True -o $SKIMDIR/SKIM_WZTo3LNu    -i $INPUTDIR/WZTo3LNu.txt    -x 4.43   --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 15  -k True -o $SKIMDIR/SKIM_WZTo1L1Nu2Q -i $INPUTDIR/WZTo1L1Nu2Q.txt -x 10.71  --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_ZZZ         -i $INPUTDIR/ZZZ.txt         -x 0.0147 --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_WZZ         -i $INPUTDIR/WZZ.txt         -x 0.057  --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_WWW         -i $INPUTDIR/WWW.txt         -x 0.209  --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_WWZ         -i $INPUTDIR/WWZ.txt         -x 0.168  --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 15  -k True -o $SKIMDIR/SKIM_WZTo1L3Nu   -i $INPUTDIR/WZTo1L3Nu.txt   -x 3.06   --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 25  -k True -o $SKIMDIR/SKIM_WZTo2L2Q    -i $INPUTDIR/WZTo2L2Q.txt    -x 5.595  --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 15  -k True -o $SKIMDIR/SKIM_WWTo2L2Nu   -i $INPUTDIR/WWTo2L2Nu.txt   -x 12.178 --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 15  -k True -o $SKIMDIR/SKIM_WWToLNuQQ   -i $INPUTDIR/WWToLNuQQ.txt   -x 50.00  --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 15  -k True -o $SKIMDIR/SKIM_WWTo4Q      -i $INPUTDIR/WWTo4Q.txt      -x 51.723 --pu $PUDIR/PU_Legacy2016_SF.txt


######################
#### Reweighting ggF non res - filelists up to date
echo "Submitting - GGF LO Reweight - "
echo "Submitting - GGF LO Reweight - " >> log_29May2021.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_29May2021.txt
### norm xs = 1 pb
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_HHRew_SM -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_LO_allNodes.txt --pu $PUDIR/PU_Legacy2016_SF.txt  -x 1.0  --kl 1.0  --kt 1.0 -a True

### norm to theoretical xs
# sigma_NNLO+FTapprox for SM: 31.05 fb
# sigma_NNLO+FTapprox / sigma_NLO  = 1.115 for SM # TEMPORARY: need to fix with factor kL dependent (pag. 129: https://arxiv.org/pdf/2003.01700.pdf)
# f(kL) = A + B*kL + C*kL**2
# A = 62.5339
# B = -44.323
# C = 9.6340
# (slide 10: https://indico.cern.ch/event/885273/contributions/3812533/attachments/2016615/3370728/HH_combine_model_7Apr2018.pdf)
# xs (kL = 1)                      = 0.03105 pb --> * BR(hh->bbtautau) = 0.03105 * (0.073056256) = 0.0022683967    pb
# xs (kL = 0)    = f(0)    * 1.115 = 0.06972 pb --> * BR(hh->bbtautau) = 0.06972 * (0.073056256) = 0.005093482168  pb
# xs (kL = 2.45) = f(2.45) * 1.115 = 0.01312 pb --> * BR(hh->bbtautau) = 0.01312 * (0.073056256) = 0.0009584980787 pb
# xs (kL = 5)    = f(5)    * 1.115 = 0.09117 pb --> * BR(hh->bbtautau) = 0.09117 * (0.073056256) = 0.00666053886   pb
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 15 -k True -o $SKIMDIR/SKIM_HHRew_cHHH1    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_LO_allNodes_kl1.txt    --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967    --kl 1.0   --kt 1.0  -a True
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 15 -k True -o $SKIMDIR/SKIM_HHRew_cHHH0    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_LO_allNodes_kl0.txt    --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.005093482168  --kl 0.0   --kt 1.0  -a True
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 15 -k True -o $SKIMDIR/SKIM_HHRew_cHHH2p45 -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_LO_allNodes_kl2p45.txt --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0009584980787 --kl 2.45  --kt 1.0  -a True
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 15 -k True -o $SKIMDIR/SKIM_HHRew_cHHH5    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_LO_allNodes_kl5.txt    --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.00666053886   --kl 5.0   --kt 1.0  -a True

### NLO samples
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_GGHH_NLO_cHHH0_xs    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl0.txt    --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.005093482168  -a True --hhNLO True
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_GGHH_NLO_cHHH1_xs    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl1.txt    --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967    -a True --hhNLO True
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_GGHH_NLO_cHHH2p45_xs -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl2p45.txt --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0009584980787 -a True --hhNLO True
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_GGHH_NLO_cHHH5_xs    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl5.txt    --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.00666053886   -a True --hhNLO True


#### VBF SM :
echo "Submitting - VBF SM - "
echo "Submitting - VBF SM - " >> log_29May2021.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_29May2021.txt

### norm to theoretical xs 
# xs_theo(SM) = 0.001726
#  CV C2V C3 |  xs_MG5      * xs_theo(SM)/xs_MG5(SM)
#   1   1  1 |  0.001601 pb * 1.078076202             =   0.001726 pb --> * BR(hh->bbtautau) = 0.001726 * (0.073056256) = 0.00012609510   pb
# 0.5   1  1 |  0.01009  pb * 1.078076202             =   0.010878 pb --> * BR(hh->bbtautau) = 0.010878 * (0.073056256) = 0.0007947059528 pb
# 1.5   1  1 |  0.06153  pb * 1.078076202             =   0.066334 pb --> * BR(hh->bbtautau) = 0.066334 * (0.073056256) = 0.004846113686  pb
#   1   1  0 |  0.004259 pb * 1.078076202             =   0.004591 pb --> * BR(hh->bbtautau) = 0.004591 * (0.073056256) = 0.0003354012713 pb
#   1   1  2 |  0.001327 pb * 1.078076202             =   0.001431 pb --> * BR(hh->bbtautau) = 0.001431 * (0.073056256) = 0.0001045435023 pb
#   1   2  1 |  0.01335  pb * 1.078076202             =   0.014392 pb --> * BR(hh->bbtautau) = 0.014392 * (0.073056256) = 0.001051425636  pb
#   1   0  1 |  0.02526  pb * 1.078076202             =   0.027232 pb --> * BR(hh->bbtautau) = 0.027232 * (0.073056256) = 0.001989467963  pb
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 40 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1   -i $INPUTDIR_SIG/VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_dipoleRecoilOff.txt    -x 0.00012609510   -a True --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 40 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_0_5_C2V_1_C3_1 -i $INPUTDIR_SIG/VBFHHTo2B2Tau_CV_0_5_C2V_1_C3_1_dipoleRecoilOff.txt  -x 0.0007947059528 -a True --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 40 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_1_5_C2V_1_C3_1 -i $INPUTDIR_SIG/VBFHHTo2B2Tau_CV_1_5_C2V_1_C3_1_dipoleRecoilOff.txt  -x 0.004846113686  -a True --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 40 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_1_C3_0   -i $INPUTDIR_SIG/VBFHHTo2B2Tau_CV_1_C2V_1_C3_0_dipoleRecoilOff.txt    -x 0.0003354012713 -a True --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 40 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_1_C3_2   -i $INPUTDIR_SIG/VBFHHTo2B2Tau_CV_1_C2V_1_C3_2_dipoleRecoilOff.txt    -x 0.0001045435023 -a True --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 40 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_2_C3_1   -i $INPUTDIR_SIG/VBFHHTo2B2Tau_CV_1_C2V_2_C3_1_dipoleRecoilOff.txt    -x 0.001051425636  -a True --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 40 -k True -o $SKIMDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_0_C3_1   -i $INPUTDIR_SIG/VBFHHTo2B2Tau_CV_1_C2V_0_C3_1_dipoleRecoilOff.txt    -x 0.001989467963  -a True --pu $PUDIR/PU_Legacy2016_SF.txt


# # #####################
#### TTX :
# HXSWG: xs(ttH) = 0.5071 pb
echo "Submitting - TTX - "
echo "Submitting - TTX - " >> log_29May2021.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_29May2021.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_ttHJetTononBB -i $INPUTDIR/ttHToNonbb.txt   -x 0.5071 -y 0.423  --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_ttHJetToBB    -i $INPUTDIR/ttHTobb.txt      -x 0.5071 -y 0.577  --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_TTWJetsToLNu  -i $INPUTDIR/TTWJetsToLNu.txt -x 0.2043           --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTWJetsToQQ   -i $INPUTDIR/TTWJetsToQQ.txt  -x 0.4062           --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_TTZToLLNuNu   -i $INPUTDIR/TTZToLLNuNu.txt  -x 0.2529           --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTWW          -i $INPUTDIR/TTWW.txt         -x 0.006979         --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTWZ          -i $INPUTDIR/TTWZ.txt         -x 0.002441         --pu $PUDIR/PU_Legacy2016_SF.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTZZ          -i $INPUTDIR/TTZZ.txt         -x 0.001386         --pu $PUDIR/PU_Legacy2016_SF.txt

# # #####################
# DY
echo "Submitting - DY - "
echo "Submitting - DY - " >> log_29May2021.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_29May2021.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 220 -k True -o $SKIMDIR/SKIM_DY           -i $INPUTDIR/DYmerged.txt            -x 6077.22  --pu $PUDIR/PU_Legacy2016_SF.txt -g True --DY True


###################
# EFT Benchmarks
echo "Submitting - EFT - "
echo "Submitting - EFT - " >> log_18May2021.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18May2021.txt

#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 5 -k True --pu $PUDIR/PU_Legacy2016_SF.txt -o $SKIMDIR/SKIM_GGHH_c2_10p0_NLOrew  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_LO_allNodes.txt -x 1.0 -a True --EFTbm c2scan --order nlo --c2 10.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg  -n 50 -k True -o $SKIMDIR/SKIM_ST_t_channel_antitop -i $INPUTDIR/ST_t-channel_antitop_4f.txt -x 80.95   --pu $PUDIR/PU_Legacy2016_SF.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 5 -k True --pu $PUDIR/PU_Legacy2016_SF.txt -o $SKIMDIR/SKIM_GGHH_manual_NLOrew   -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_LO_allNodes.txt -x 1.0 -a True --kl 29.0 --kt 3.0 --c2 8.0 --cg 27.0 --c2g 7.0 --EFTbm manual --order nlo --BSMname testingBSM
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 5 -k True --pu $PUDIR/PU_Legacy2016_SF.txt -o $SKIMDIR/SKIMMED_GGHH_cHHH1_NLOrew  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_LO_allNodes.txt -x 1.0 -a True --BSMname cHHH1  --EFTbm cHHH1  --order nlo



######################
### DATA :
######################
echo "Submitting - DATA tau - "
echo "Submitting - DATA tau - " >> log_29May2021.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_29May2021.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_2016B -i $INPUTDIR_DATA/Tau_2016B.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_2016C -i $INPUTDIR_DATA/Tau_2016C.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_2016D -i $INPUTDIR_DATA/Tau_2016D.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_2016E -i $INPUTDIR_DATA/Tau_2016E.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_2016F -i $INPUTDIR_DATA/Tau_2016F.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_2016G -i $INPUTDIR_DATA/Tau_2016G.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Tau_2016H -i $INPUTDIR_DATA/Tau_2016H.txt

echo "Submitting - DATA Mu - "
echo "Submitting - DATA Mu - " >> log_29May2021.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_29May2021.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Mu_2016B -i $INPUTDIR_DATA/Muon_2016B.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Mu_2016C -i $INPUTDIR_DATA/Muon_2016C.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Mu_2016D -i $INPUTDIR_DATA/Muon_2016D.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Mu_2016E -i $INPUTDIR_DATA/Muon_2016E.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Mu_2016F -i $INPUTDIR_DATA/Muon_2016F.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Mu_2016G -i $INPUTDIR_DATA/Muon_2016G.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Mu_2016H -i $INPUTDIR_DATA/Muon_2016H.txt


echo "Submitting - DATA Electron - "
echo "Submitting - DATA Electron - " >> log_29May2021.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_29May2021.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Ele_2016B -i $INPUTDIR_DATA/Electron_2016B.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Ele_2016C -i $INPUTDIR_DATA/Electron_2016C.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Ele_2016D -i $INPUTDIR_DATA/Electron_2016D.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Ele_2016E -i $INPUTDIR_DATA/Electron_2016E.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Ele_2016F -i $INPUTDIR_DATA/Electron_2016F.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Ele_2016G -i $INPUTDIR_DATA/Electron_2016G.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_Ele_2016H -i $INPUTDIR_DATA/Electron_2016H.txt


###################################
##### Systematics studies

#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True  -o $SKIMDIR/SKIM_HHRew_cHHH1_2      -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_LO_allNodes_kl1.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  --kl 1.0  --kt 1.0  -a True
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 80 -k True  -o $SKIMDIR/SKIM_WJets_inclusive_2  -i $INPUTDIR/WJetsToLNu.txt                              -y 1.221252481 -x 61526.7  --pu $PUDIR/PU_Legacy2016_SF.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 80 -k True  -o $SKIMDIR/SKIM_TT_semiLep_2       -i $INPUTDIR/TTToSemiLeptonic.txt                        -x 365.34 -t True -b 5  --pu $PUDIR/PU_Legacy2016_SF.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 80 -k False -o $SKIMDIR/SKIM_DY_fixDenom        -i $INPUTDIR/DYmerged.txt                                -x 6077.22  --pu $PUDIR/PU_Legacy2016_SF.txt -g True --DY True


######################################
##### HHcombination overlap checks

# bbbb GGF samples
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_GGHH4b_2016_NLO_cHHH0    -i $INPUTDIR_SIG/GluGluToHHTo4B_NLO_kl0.txt    --pu $PUDIR/PU_Legacy2016_SF.txt  -x 1.0 --hhNLO True
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_GGHH4b_2016_NLO_cHHH1    -i $INPUTDIR_SIG/GluGluToHHTo4B_NLO_kl1.txt    --pu $PUDIR/PU_Legacy2016_SF.txt  -x 1.0 --hhNLO True
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_GGHH4b_2016_NLO_cHHH2p45 -i $INPUTDIR_SIG/GluGluToHHTo4B_NLO_kl2p45.txt --pu $PUDIR/PU_Legacy2016_SF.txt  -x 1.0 --hhNLO True
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_GGHH4b_2016_NLO_cHHH5    -i $INPUTDIR_SIG/GluGluToHHTo4B_NLO_kl5.txt    --pu $PUDIR/PU_Legacy2016_SF.txt  -x 1.0 --hhNLO True

COMMENT1

######################################
##### EFT studies

# No rew - just "gen-skim" (skim with no reco-cuts) for comparison plots
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_plain_kl1  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl1.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname plain_kl1
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_plain_kl0  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl0.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname plain_kl0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_plain_kl2p45  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl2p45.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname plain_kl2p45
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_plain_kl5  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl5.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname plain_kl5

# SM NLO to SM NLO only
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_nloSM_to_nloSM  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl1.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname nloSM_to_nloSM --EFTbm manual --order_input nlo --order_rew nlo --kl 1.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0

# Rew allNLO to kl 1 / 0 / 2.45 / 5
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl1    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname allNLO_to_kl1 --EFTbm manual --order_input nlo --order_rew nlo --kl 1.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl0    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname allNLO_to_kl0 --EFTbm manual --order_input nlo --order_rew nlo --kl 0.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl2p45 -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname allNLO_to_kl2p45 --EFTbm manual --order_input nlo --order_rew nlo --kl 2.45 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl5    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname allNLO_to_kl5 --EFTbm manual --order_input nlo --order_rew nlo --kl 5.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0

# SM NLO to SM NLO only - weighted
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_nloSM_to_nloSM_weightedSimple  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl1.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname nloSM_to_nloSM_weightedSimple --EFTbm manual --order_input nlo --order_rew nlo --kl 1.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_nloSM_to_nloSM_weightedFull  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl1.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname nloSM_to_nloSM_weightedFull --EFTbm manual --order_input nlo --order_rew nlo --kl 1.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0

# Rew allNLO to kl 1 / 0 / 2.45 / 5 - weighted
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl1_weighted    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname allNLO_to_kl1_weighted    --EFTbm manual --order_input nlo --order_rew nlo --kl 1.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl0_weighted    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname allNLO_to_kl0_weighted    --EFTbm manual --order_input nlo --order_rew nlo --kl 0.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl2p45_weighted -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname allNLO_to_kl2p45_weighted --EFTbm manual --order_input nlo --order_rew nlo --kl 2.45 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl5_weighted    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname allNLO_to_kl5_weighted    --EFTbm manual --order_input nlo --order_rew nlo --kl 5.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0

# 2.45 NLO to 2.45 NLO only - weighted
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_2p45_to_2p45_weightedFull  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl2p45.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname 2p45_to_2p45_weightedFull --EFTbm manual --order_input nlo --order_rew nlo --kl 2.45 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0

# 0 NLO to 0 NLO only - weighted
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_0_to_0_weightedFull  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl0.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname 0_to_0_weightedFull --EFTbm manual --order_input nlo --order_rew nlo --kl 0.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0

# 5 NLO to 5 NLO only - weighted
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_5_to_5_weightedFull  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl5.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname 5_to_5_weightedFull --EFTbm manual --order_input nlo --order_rew nlo --kl 5.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0

# Rew allNLO to kl 1 / 0 / 2.45 / 5 - weighted - bugfix
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl1_weighted_bugfix    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname allNLO_to_kl1_weighted_bugfix    --EFTbm manual --order_input nlo --order_rew nlo --kl 1.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl0_weighted_bugfix    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname allNLO_to_kl0_weighted_bugfix    --EFTbm manual --order_input nlo --order_rew nlo --kl 0.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl2p45_weighted_bugfix -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname allNLO_to_kl2p45_weighted_bugfix --EFTbm manual --order_input nlo --order_rew nlo --kl 2.45 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl5_weighted_bugfix    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname allNLO_to_kl5_weighted_bugfix    --EFTbm manual --order_input nlo --order_rew nlo --kl 5.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0

# Rew allNLO to kl 1 / 0 / 2.45 / 5 - weighted - bugfix - totYield
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl1_weighted_bugfix_totYield    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname allNLO_to_kl1_weighted_bugfix_totYield    --EFTbm manual --order_input nlo --order_rew nlo --kl 1.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl0_weighted_bugfix_totYield    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname allNLO_to_kl0_weighted_bugfix_totYield    --EFTbm manual --order_input nlo --order_rew nlo --kl 0.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl2p45_weighted_bugfix_totYield -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname allNLO_to_kl2p45_weighted_bugfix_totYield --EFTbm manual --order_input nlo --order_rew nlo --kl 2.45 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl5_weighted_bugfix_totYield    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967  -a True --hhNLO True --BSMname allNLO_to_kl5_weighted_bugfix_totYield    --EFTbm manual --order_input nlo --order_rew nlo --kl 5.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0

# Rew allNLO to kl 1 / 0 / 2.45 / 5 - weighted - bugfix - (w/ and w/o totYield) - addWeight1
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl1_weighted_bugfix_totYield_addWeight1    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 1.  -a True --hhNLO True --BSMname allNLO_to_kl1_weighted_bugfix_totYield_addWeight1    --EFTbm manual --order_input nlo --order_rew nlo --kl 1.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_allNLO_to_kl1_weighted_bugfix_addWeight1    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt  -x 1.  -a True --hhNLO True --BSMname allNLO_to_kl1_weighted_bugfix_addWeight1    --EFTbm manual --order_input nlo --order_rew nlo --kl 1.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0

# Rew point -> point ("closure test") - Opt1 (weighted - bugfix - w/o totYield - addWeight) - SUCCESS!
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 15 -k True -o $SKIMDIR/SKIM_0_to_0_Opt1       -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl0.txt    --pu $PUDIR/PU_Legacy2016_SF.txt  -x 1. -a True --hhNLO True --BSMname 0_to_0_Opt1       --EFTbm manual --order_input nlo --order_rew nlo --kl 0.0  --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 15 -k True -o $SKIMDIR/SKIM_1_to_1_Opt1       -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl1.txt    --pu $PUDIR/PU_Legacy2016_SF.txt  -x 1. -a True --hhNLO True --BSMname 1_to_1_Opt1       --EFTbm manual --order_input nlo --order_rew nlo --kl 1.0  --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 15 -k True -o $SKIMDIR/SKIM_2p45_to_2p45_Opt1 -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl2p45.txt --pu $PUDIR/PU_Legacy2016_SF.txt  -x 1. -a True --hhNLO True --BSMname 2p45_to_2p45_Opt1 --EFTbm manual --order_input nlo --order_rew nlo --kl 2.45 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 15 -k True -o $SKIMDIR/SKIM_5_to_5_Opt1       -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl5.txt    --pu $PUDIR/PU_Legacy2016_SF.txt  -x 1. -a True --hhNLO True --BSMname 5_to_5_Opt1       --EFTbm manual --order_input nlo --order_rew nlo --kl 5.0  --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0

# Rew allNLO to kl 1 / 0 / 2.45 / 5 - Opt1 (weighted - bugfix - w/o totYield - addWeight) - SUCCESS!
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 15 -k True -o $SKIMDIR/SKIM_allNLO_to_kl0_Opt1    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt --pu $PUDIR/PU_Legacy2016_SF.txt -x 1. -a True --hhNLO True --BSMname allNLO_to_kl0_Opt1    --EFTbm manual --order_input nlo --order_rew nlo --kl 0.0  --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 15 -k True -o $SKIMDIR/SKIM_allNLO_to_kl1_Opt1    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt --pu $PUDIR/PU_Legacy2016_SF.txt -x 1. -a True --hhNLO True --BSMname allNLO_to_kl1_Opt1    --EFTbm manual --order_input nlo --order_rew nlo --kl 1.0  --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 15 -k True -o $SKIMDIR/SKIM_allNLO_to_kl2p45_Opt1 -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt --pu $PUDIR/PU_Legacy2016_SF.txt -x 1. -a True --hhNLO True --BSMname allNLO_to_kl2p45_Opt1 --EFTbm manual --order_input nlo --order_rew nlo --kl 2.45 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 15 -k True -o $SKIMDIR/SKIM_allNLO_to_kl5_Opt1    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt --pu $PUDIR/PU_Legacy2016_SF.txt -x 1. -a True --hhNLO True --BSMname allNLO_to_kl5_Opt1    --EFTbm manual --order_input nlo --order_rew nlo --kl 5.0  --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0

# ----------------------
# Doing Reco part now SKIMS_Legacy2016_22Nov2022_EFT
# Standard points:
# xs (kL = 1)                      = 0.03105 pb --> * BR(hh->bbtautau) = 0.03105 * (0.073056256) = 0.0022683967    pb
# xs (kL = 0)    = f(0)    * 1.115 = 0.06972 pb --> * BR(hh->bbtautau) = 0.06972 * (0.073056256) = 0.005093482168  pb
# xs (kL = 2.45) = f(2.45) * 1.115 = 0.01312 pb --> * BR(hh->bbtautau) = 0.01312 * (0.073056256) = 0.0009584980787 pb
# xs (kL = 5)    = f(5)    * 1.115 = 0.09117 pb --> * BR(hh->bbtautau) = 0.09117 * (0.073056256) = 0.00666053886   pb

# No rew - just "skim" for comparison plots
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k False -o $SKIMDIR/SKIM_plain_kl1    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl1.txt    --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0022683967    -a True --hhNLO True --BSMname plain_kl1
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k False -o $SKIMDIR/SKIM_plain_kl0    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl0.txt    --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.005093482168  -a True --hhNLO True --BSMname plain_kl0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k False -o $SKIMDIR/SKIM_plain_kl2p45 -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl2p45.txt --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.0009584980787 -a True --hhNLO True --BSMname plain_kl2p45
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k False -o $SKIMDIR/SKIM_plain_kl5    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_kl5.txt    --pu $PUDIR/PU_Legacy2016_SF.txt  -x 0.00666053886   -a True --hhNLO True --BSMname plain_kl5

# Rew allNLO to kl 1 / 0 / 2.45 / 5 - Opt1 - reco1
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 30 -k False -o $SKIMDIR/SKIM_allNLO_to_kl1_Opt1_reco1    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.0022683967    -a True --hhNLO True --BSMname allNLO_to_kl1_Opt1_reco1    --EFTbm manual --order_input nlo --order_rew nlo --kl 1.0  --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k False -o $SKIMDIR/SKIM_allNLO_to_kl0_Opt1_reco1    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.005093482168  -a True --hhNLO True --BSMname allNLO_to_kl0_Opt1_reco1    --EFTbm manual --order_input nlo --order_rew nlo --kl 0.0  --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k False -o $SKIMDIR/SKIM_allNLO_to_kl2p45_Opt1_reco1 -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.0009584980787 -a True --hhNLO True --BSMname allNLO_to_kl2p45_Opt1_reco1 --EFTbm manual --order_input nlo --order_rew nlo --kl 2.45 --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k False -o $SKIMDIR/SKIM_allNLO_to_kl5_Opt1_reco1    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00666053886   -a True --hhNLO True --BSMname allNLO_to_kl5_Opt1_reco1    --EFTbm manual --order_input nlo --order_rew nlo --kl 5.0  --kt 1.0 --c2 0.0 --cg 0.0 --c2g 0.0


# ----------------------
# Doing EFT skimming now SKIMS_Legacy2016_24Nov2022_EFT
# Benchmarks JHEP04(2016)126 [1-12] (https://arxiv.org/pdf/1710.08261.pdf) + [8a] (https://link.springer.com/article/10.1007/JHEP09(2018)057)
# Benchmarks (use 1 fb * BR(bbtt)):
# xs = 1 fb = 0.001 pb --> * BR(hh->bbtautau) = 0.001 * (0.073056256) = 0.00007305625 pb
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_allNLO_to_BM1  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM1  --EFTbm 1  --order_input nlo --order_rew nlo
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_allNLO_to_BM2  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM2  --EFTbm 2  --order_input nlo --order_rew nlo
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_allNLO_to_BM3  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM3  --EFTbm 3  --order_input nlo --order_rew nlo
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_allNLO_to_BM4  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM4  --EFTbm 4  --order_input nlo --order_rew nlo
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_allNLO_to_BM5  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM5  --EFTbm 5  --order_input nlo --order_rew nlo
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_allNLO_to_BM6  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM6  --EFTbm 6  --order_input nlo --order_rew nlo
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_allNLO_to_BM7  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM7  --EFTbm 7  --order_input nlo --order_rew nlo
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_allNLO_to_BM8  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM8  --EFTbm 8  --order_input nlo --order_rew nlo
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_allNLO_to_BM9  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM9  --EFTbm 9  --order_input nlo --order_rew nlo
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_allNLO_to_BM10 -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM10 --EFTbm 10 --order_input nlo --order_rew nlo
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_allNLO_to_BM11 -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM11 --EFTbm 11 --order_input nlo --order_rew nlo
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_allNLO_to_BM12 -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM12 --EFTbm 12 --order_input nlo --order_rew nlo
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_allNLO_to_BM8a -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM8a --EFTbm 8a --order_input nlo --order_rew nlo

# Benchmarks JHEP03(2020)091 [1b-7b] (https://arxiv.org/pdf/1908.08923.pdf)
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 60 -k True -o $SKIMDIR/SKIM_allNLO_to_BM1b  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM1b  --EFTbm 1b  --order_input nlo --order_rew nlo
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 60 -k True -o $SKIMDIR/SKIM_allNLO_to_BM2b  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM2b  --EFTbm 2b  --order_input nlo --order_rew nlo
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 60 -k True -o $SKIMDIR/SKIM_allNLO_to_BM3b  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM3b  --EFTbm 3b  --order_input nlo --order_rew nlo
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 60 -k True -o $SKIMDIR/SKIM_allNLO_to_BM4b  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM4b  --EFTbm 4b  --order_input nlo --order_rew nlo
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 60 -k True -o $SKIMDIR/SKIM_allNLO_to_BM5b  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM5b  --EFTbm 5b  --order_input nlo --order_rew nlo
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 60 -k True -o $SKIMDIR/SKIM_allNLO_to_BM6b  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM6b  --EFTbm 6b  --order_input nlo --order_rew nlo
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 60 -k True -o $SKIMDIR/SKIM_allNLO_to_BM7b  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt  --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00007305625 -a True --hhNLO True --BSMname allNLO_to_BM7b  --EFTbm 7b  --order_input nlo --order_rew nlo

# c2 scan - Need 6 samples in input to the hhModel
# Samples needed:
# 1. ggHH_kl_1p00_kt_1p00_c2_0p00   31.054    --> NLO kl=1 already produced
# 2. ggHH_kl_1p00_kt_1p00_c2_3p00   2923.567  --> reweight (c2 = 3.0)         --> 2.923567 pb --> * BR(hh->bbtautau) = 2.923567 * (0.073056256) = 0.21358485918 pb
# 3. ggHH_kl_1p00_kt_1p00_c2_0p35   11.103    --> reweight (c2 = 0.35)        --> 0.011103 pb --> * BR(hh->bbtautau) = 0.011103 * (0.073056256) = 0.00081114361 pb
# 4. ggHH_kl_0p00_kt_1p00_c2_0p00   69.697    --> NLO kl=0 already produced
# 5. ggHH_kl_2p45_kt_1p00_c2_0p00   13.183    --> NLO kl=2.45 already produced
# 6. ggHH_kl_0p00_kt_1p00_c2_1p00   155.508   --> reweight (c2 = 1.0)         --> 0.155508 pb --> * BR(hh->bbtautau) = 0.155508 * (0.073056256) = 0.01136083225 pb
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 60 -k True -o $SKIMDIR/SKIM_GGHH_NLO_c2_3p0  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.21358485918 -a True --hhNLO True --BSMname GGHH_NLO_c2_3p0  --EFTbm manual --order_input nlo --order_rew nlo --kl 1.0  --kt 1.0 --c2 3.0  --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 60 -k True -o $SKIMDIR/SKIM_GGHH_NLO_c2_0p35 -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.00081114361 -a True --hhNLO True --BSMname GGHH_NLO_c2_0p35 --EFTbm manual --order_input nlo --order_rew nlo --kl 1.0  --kt 1.0 --c2 0.35 --cg 0.0 --c2g 0.0
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2016_mib.cfg -n 60 -k True -o $SKIMDIR/SKIM_GGHH_NLO_c2_1p0  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_NLO_allNodes.txt --pu $PUDIR/PU_Legacy2016_SF.txt -x 0.01136083225 -a True --hhNLO True --BSMname GGHH_NLO_c2_1p0  --EFTbm manual --order_input nlo --order_rew nlo --kl 1.0  --kt 1.0 --c2 1.0  --cg 0.0 --c2g 0.0


<<COMMENT2


COMMENT2
