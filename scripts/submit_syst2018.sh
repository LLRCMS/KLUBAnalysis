# LOG
OUTDIRR="SKIMMED_Legacy2018_2022_11_25_EFT"

# WORKDIR
WORKDIR="/data_CMS/cms/motta/HHLegacy_SKIMS/SKIMS2018"

# ENVIRONMENT
source scripts/setup.sh
source /opt/exp_soft/cms/t3/t3setup
#mkdir $OUTDIRR
cp scripts/listSyst.sh $WORKDIR/$OUTDIRR/
cp scripts/listFakeTaus.sh $WORKDIR/$OUTDIRR/

################################################################################################################################
################################################################################################################################

# DY
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_lowMass -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_0b_0JPt         -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_0b_10JPt        -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_0b_30JPt        -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_0b_50JPt        -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_0b_100JPt       -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_0b_200JPt       -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_1b_0JPt         -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_1b_10JPt        -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_1b_30JPt        -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_1b_50JPt        -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_1b_100JPt       -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_1b_200JPt       -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_2b_0JPt         -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_2b_10JPt        -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_2b_30JPt        -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_2b_50JPt        -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_2b_100JPt       -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_2b_200JPt       -c config/skim_Legacy2018.cfg


# TT
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TT_fullyHad -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TT_fullyLep -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TT_semiLep  -c config/skim_Legacy2018.cfg


# EWK
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_EWKWMinus2Jets_WToLNu -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_EWKWPlus2Jets_WToLNu  -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_EWKZ2Jets_ZToLL       -c config/skim_Legacy2018.cfg


# SingleTop
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ST_tW_antitop        -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ST_tW_top            -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ST_tchannel_antitop  -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ST_tchannel_top      -c config/skim_Legacy2018.cfg


# TT+X
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTWJetsToLNu -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTWJetsToQQ  -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTWW         -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTWZ         -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTZToLLNuNu  -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTZZ         -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTZToQQ      -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTWH         -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTZH         -c config/skim_Legacy2018.cfg


# W+Jets
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WJets_HT_0_100     -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WJets_HT_100_200   -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WJets_HT_200_400   -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WJets_HT_400_600   -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WJets_HT_600_800   -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WJets_HT_800_1200  -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WJets_HT_1200_2500 -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WJets_HT_2500_Inf  -c config/skim_Legacy2018.cfg


# WW
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WWTo2L2Nu -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WWTo4Q    -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WWToLNuQQ -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WWW       -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WWZ       -c config/skim_Legacy2018.cfg        


# WZ
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WZTo1L1Nu2Q -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WZTo1L3Nu   -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WZTo2L2Q    -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WZTo3LNu    -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WZZ         -c config/skim_Legacy2018.cfg


# Single Higgs
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WminusHTauTau  -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WplusHTauTau   -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ZH_HBB_ZLL     -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ZH_HBB_ZQQ     -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ZH_HTauTau     -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_VBFHTauTau     -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ggHTauTau      -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ttHJetToBB     -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ttHJetTononBB  -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ttHJetToTauTau -c config/skim_Legacy2018.cfg


# ZZ
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ZZTo2L2Nu -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ZZTo2L2Q  -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ZZTo2Q2Nu -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ZZTo4L    -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ZZZ       -c config/skim_Legacy2018.cfg


# GGF HH NLO
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_NLO_cHHH0_xs    -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_NLO_cHHH1_xs       -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_NLO_cHHH2p45_xs -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_NLO_cHHH5_xs    -c config/skim_Legacy2018.cfg


# VBF HH
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_VBFHH_CV_0_5_C2V_1_C3_1_xs -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_VBFHH_CV_1_5_C2V_1_C3_1_xs -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_VBFHH_CV_1_C2V_0_C3_1_xs   -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_VBFHH_CV_1_C2V_1_C3_0_xs   -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_VBFHH_CV_1_C2V_1_C3_1_xs   -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_VBFHH_CV_1_C2V_1_C3_2_xs   -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_VBFHH_CV_1_C2V_2_C3_1_xs   -c config/skim_Legacy2018.cfg


# Muon
# python scripts/systNtuple.py -d True -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_SingleMuon2018A -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -d True -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_SingleMuon2018B -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -d True -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_SingleMuon2018C -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -d True -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_SingleMuon2018D -c config/skim_Legacy2018.cfg


# Ele
# python scripts/systNtuple.py -d True -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_SingleElectron2018A -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -d True -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_SingleElectron2018B -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -d True -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_SingleElectron2018C -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -d True -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_SingleElectron2018D -c config/skim_Legacy2018.cfg


# Tau
# python scripts/systNtuple.py -d True -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_Tau2018A -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -d True -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_Tau2018B -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -d True -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_Tau2018C -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -d True -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_Tau2018D -c config/skim_Legacy2018.cfg


# Benchmarks JHEP04(2016)126 [1-12] (https://arxiv.org/pdf/1710.08261.pdf) + [8a] (https://link.springer.com/article/10.1007/JHEP09(2018)057)
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM1  -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM2  -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM3  -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM4  -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM5  -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM6  -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM7  -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM8  -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM9  -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM10 -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM11 -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM12 -c config/skim_Legacy2018.cfg
# python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM8a -c config/skim_Legacy2018.cfg


# Benchmarks JHEP03(2020)091 [1b-7b] (https://arxiv.org/pdf/1908.08923.pdf)
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM1b -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM2b -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM3b -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM4b -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM5b -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM6b -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_allNLO_to_BM7b -c config/skim_Legacy2018.cfg


# c2 scan
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_NLO_c2_3p0  -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_NLO_c2_0p35 -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_NLO_c2_1p0  -c config/skim_Legacy2018.cfg

