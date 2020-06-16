# LOG
OUTDIRR="SKIMS_1June2020"

# WORKDIR
WORKDIR="/data_CMS/cms/amendola/HHLegacy_2018_v2"

# ENVIRONMENT
source scripts/setup.sh
source /opt/exp_soft/cms/t3/t3setup
mkdir $OUTDIRR


################################################################################################################################
################################################################################################################################


# DY
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY         -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_DY_lowMass -c config/skim_Legacy2018.cfg


# EWK
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_EWKWMinus2Jets_WToLNu -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_EWKWPlus2Jets_WToLNu  -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_EWKZ2Jets_ZToLL       -c config/skim_Legacy2018.cfg


# SingleTop
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ST_tW_antitop        -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ST_tW_top            -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ST_tchannel_antitop  -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ST_tchannel_top      -c config/skim_Legacy2018.cfg


# TT+X
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTWJetsToLNu -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTWJetsToQQ  -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTWW         -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTWZ         -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTZToLLNuNu  -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTZZ         -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTZToQQ      -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTWH         -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TTZH         -c config/skim_Legacy2018.cfg


# TT
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TT_fullyHad -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TT_fullyLep -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_TT_semiLep  -c config/skim_Legacy2018.cfg


# W+Jets
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WJets_HT_0_100     -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WJets_HT_100_200   -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WJets_HT_200_400   -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WJets_HT_400_600   -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WJets_HT_600_800   -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WJets_HT_800_1200  -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WJets_HT_1200_2500 -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WJets_HT_2500_Inf  -c config/skim_Legacy2018.cfg


# WW
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WWTo2L2Nu -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WWTo4Q    -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WWToLNuQQ -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WWW       -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WWZ       -c config/skim_Legacy2018.cfg        


# WZ
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WZTo1L1Nu2Q -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WZTo1L3Nu   -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WZTo2L2Q    -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WZTo3LNu    -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WZZ         -c config/skim_Legacy2018.cfg


# Single Higgs
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WminusHTauTau  -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_WplusHTauTau   -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ZH_HBB_ZLL     -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ZH_HBB_ZQQ     -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ZH_HTauTau     -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_VBFHTauTau     -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ggHTauTau      -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ttHJetToBB     -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ttHJetTononBB  -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ttHJetToTauTau -c config/skim_Legacy2018.cfg


# ZZ
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ZZTo2L2Nu -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ZZTo2L2Q  -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ZZTo2Q2Nu -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ZZTo4L    -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_ZZZ       -c config/skim_Legacy2018.cfg


# GGF HH NLO
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_NLO_cHHH0    -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_NLO_SM       -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_NLO_cHHH2p45 -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_NLO_cHHH5    -c config/skim_Legacy2018.cfg


# GGF HH LO
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_SM       -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_node_2   -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_node_3   -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_node_4   -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_node_5   -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_node_6   -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_node_7   -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_node_8   -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_node_9   -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_node_10  -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_node_11  -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_GGHH_node_12  -c config/skim_Legacy2018.cfg


# GGF HH Rew
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_HHRew_SM  -c config/skim_Legacy2018.cfg

# VBF HH
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_VBFHH_CV_1_C2V_1_C3_1   -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_VBFHH_CV_0_5_C2V_1_C3_1 -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_VBFHH_CV_1_5_C2V_1_C3_1 -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_VBFHH_CV_1_C2V_1_C3_0   -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_VBFHH_CV_1_C2V_1_C3_2   -c config/skim_Legacy2018.cfg
python scripts/systNtuple.py -T $OUTDIRR -q long -w $WORKDIR/$OUTDIRR/SKIM_VBFHH_CV_1_C2V_2_C3_1   -c config/skim_Legacy2018.cfg




