# LOG
OUTDIRR="Skims_Legacy2016_1June2020"

# WORKDIR
WORKDIR="/gwteraz/users/brivio/SKIMMED_Legacy2016_1June2020"

# ENVIRONMENT
source /cvmfs/cms.cern.ch/cmsset_default.sh
source scripts/setup.sh
mkdir $OUTDIRR

################################################################################################################################
################################################################################################################################


<<COMMENT1

# DY
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_0j0b -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_1j0b -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_1j1b -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_2j0b -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_2j1b -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_2j2b -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_3j0b -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_3j1b -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_3j2b -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_3j3b -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_4j0b -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_4j1b -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_4j2b -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_4j3b -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_4j4b -c config/skim_Legacy2017_mib.cfg

python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_M_10_50_Not_PU_Safe -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_M_10_50_PU_Safe     -c config/skim_Legacy2017_mib.cfg


# EWK
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_EWKWMinus2Jets_WToLNu -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_EWKWPlus2Jets_WToLNu  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_EWKZ2Jets_ZToLL       -c config/skim_Legacy2017_mib.cfg


# SingleTop
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ST_tW_antitop        -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ST_tW_top            -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ST_tchannel_antitop  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ST_tchannel_top      -c config/skim_Legacy2017_mib.cfg


# TT+X
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTWJetsToLNu -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTWJetsToQQ  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTWW         -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTWZ         -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTZToLLNuNu  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTZZ         -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTZToQQ         -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTWH         -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTZH         -c config/skim_Legacy2017_mib.cfg


# TT
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TT_fullyHad -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TT_fullyLep -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TT_semiLep  -c config/skim_Legacy2017_mib.cfg


# W+Jets
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_0_70      -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_70_100    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_100_200   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_200_400   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_400_600   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_600_800   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_800_1200  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_1200_2500 -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_2500_Inf  -c config/skim_Legacy2017_mib.cfg


# WW
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WWTo2L2Nu -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WWTo4Q    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WWToLNuQQ -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WWW       -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WWZ       -c config/skim_Legacy2017_mib.cfg


# WZ
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WZTo1L1Nu2Q -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WZTo1L3Nu   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WZTo2L2Q    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WZTo3LNu    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WZZ         -c config/skim_Legacy2017_mib.cfg


# Single Higgs
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WminusHTauTau -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WplusHTauTau  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZH_HBB_ZLL    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZH_HBB_ZQQ    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZH_HTauTau    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHTauTau    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ggHTauTau     -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ttHJetToBB    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ttHJetTononBB -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ttHToTauTau   -c config/skim_Legacy2017_mib.cfg


# ZZ
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZZTo2L2Nu -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZZTo2L2Q  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZZTo2Q2Nu -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZZTo4L    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZZZ       -c config/skim_Legacy2017_mib.cfg


# GGF HH NLO
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_node_cHHH0    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_node_cHHH1    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_node_cHHH2p45 -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_node_cHHH5    -c config/skim_Legacy2017_mib.cfg


# GGF HH LO
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHHSM        -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_node_2   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_node_3   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_node_4   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_node_7   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_node_9   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_node_12  -c config/skim_Legacy2017_mib.cfg


# VBF HH
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHHSM                 -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHH_CV_1_5_C2V_1_C3_1 -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHH_CV_1_C2V_0_C3_2   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHH_CV_1_C2V_1_C3_0   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHH_CV_1_C2V_1_C3_2   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHH_CV_1_C2V_2_C3_1   -c config/skim_Legacy2017_mib.cfg


COMMENT1
