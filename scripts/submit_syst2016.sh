# LOG
OUTDIRR="TAUFAKES_Legacy2016_16Feb2021_varSyst"
#OUTDIRR="TAUFAKES_Legacy2016_16Feb2021_varSyst_ETau"

# WORKDIR
WORKDIR="/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021"
#WORKDIR="/gwteraz/users/brivio/SKIMS_Legacy2016_16Feb2021_ETau"

# ENVIRONMENT
source /cvmfs/cms.cern.ch/cmsset_default.sh
source scripts/setup.sh
mkdir $OUTDIRR

################################################################################################################################
################################################################################################################################

<<COMMENT1

# W+Jets
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_0_70      -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_70_100    -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_100_200   -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_200_400   -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_400_600   -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_600_800   -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_800_1200  -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_1200_2500 -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_2500_Inf  -c config/skim_Legacy2016_mib.cfg

# DY Low mass
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_Low_Mass -c config/skim_Legacy2016_mib.cfg

# EWK
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_EWKWMinus2Jets_WToLNu -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_EWKWPlus2Jets_WToLNu  -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_EWKZ2Jets_ZToLL       -c config/skim_Legacy2016_mib.cfg

# SingleTop
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ST_tW_antitop        -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ST_tW_top            -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ST_t_channel_antitop -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ST_t_channel_top     -c config/skim_Legacy2016_mib.cfg

# TT+X
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTWJetsToQQ  -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTWJetsToLNu -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTWW         -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTWZ         -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTZToLLNuNu  -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTZZ         -c config/skim_Legacy2016_mib.cfg

# WW
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WWTo2L2Nu -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WWTo4Q    -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WWToLNuQQ -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WWW       -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WWZ       -c config/skim_Legacy2016_mib.cfg

# WZ
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WZTo1L1Nu2Q -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WZTo1L3Nu   -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WZTo2L2Q    -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WZTo3LNu    -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WZZ         -c config/skim_Legacy2016_mib.cfg

# Single Higgs
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WminusHTauTau -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WplusHTauTau  -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZH_HBB_ZLL    -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZH_HBB_ZQQ    -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZH_HTauTau    -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHTauTau    -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ggHTauTau     -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ttHJetToBB    -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ttHJetTononBB -c config/skim_Legacy2016_mib.cfg

# ZZ
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZZTo2L2Nu -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZZTo2L2Q  -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZZTo2Q2Nu -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZZTo4L    -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZZTo4Q    -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZZZ       -c config/skim_Legacy2016_mib.cfg

# GGF HH
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_NLO_cHHH0_xs    -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_NLO_cHHH1_xs    -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_NLO_cHHH2p45_xs -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_NLO_cHHH5_xs    -c config/skim_Legacy2016_mib.cfg

# VBF HH
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1   -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHHTo2B2Tau_CV_0_5_C2V_1_C3_1 -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHHTo2B2Tau_CV_1_5_C2V_1_C3_1 -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_1_C3_0   -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_1_C3_2   -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_2_C3_1   -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHHTo2B2Tau_CV_1_C2V_0_C3_1   -c config/skim_Legacy2016_mib.cfg

# TT
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TT_fullyHad -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TT_fullyLep -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TT_semiLep  -c config/skim_Legacy2016_mib.cfg

# Ele
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Ele_2016B -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Ele_2016C -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Ele_2016D -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Ele_2016E -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Ele_2016F -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Ele_2016G -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Ele_2016H -c config/skim_Legacy2016_mib.cfg

# Muon
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Mu_2016C -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Mu_2016D -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Mu_2016E -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Mu_2016F -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Mu_2016G -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Mu_2016B -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Mu_2016H -c config/skim_Legacy2016_mib.cfg

# Tau
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Tau_2016B -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Tau_2016C -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Tau_2016D -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Tau_2016E -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Tau_2016F -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Tau_2016G -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Tau_2016H -c config/skim_Legacy2016_mib.cfg


# DY
##python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY  -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_0b_0JPt   -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_0b_10JPt  -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_0b_110JPt -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_0b_190JPt -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_0b_50JPt  -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_0b_80JPt  -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_1b_0JPt   -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_1b_10JPt  -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_1b_110JPt -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_1b_190JPt -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_1b_50JPt  -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_1b_80JPt  -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_2b_0JPt   -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_2b_10JPt  -c config/skim_Legacy2016_mib.cfg
COMMENT1
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_2b_110JPt -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_2b_190JPt -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_2b_50JPt  -c config/skim_Legacy2016_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_2b_80JPt  -c config/skim_Legacy2016_mib.cfg
<<COMMENT2


COMMENT2
