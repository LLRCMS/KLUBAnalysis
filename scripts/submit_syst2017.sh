# LOG
#OUTDIRR="TAUFAKE_Legacy2017_5Jan2022"
OUTDIRR="SYST_Legacy2017_24Nov2022_EFT"
#OUTDIRR="TAUFAKE_Legacy2017_24Nov2022_EFT"

# WORKDIR
#WORKDIR="/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_19Feb2021"
WORKDIR="/gwteraz/users/brivio/SKIMS_Legacy2017_24Nov2022_EFT"

# ENVIRONMENT
source /cvmfs/cms.cern.ch/cmsset_default.sh
source scripts/setup.sh
mkdir $OUTDIRR

################################################################################################################################
################################################################################################################################

<<COMMENT1

# Ele - Instant
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Ele_2017B -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Ele_2017C -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Ele_2017D -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Ele_2017E -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Ele_2017F -c config/skim_Legacy2017_mib.cfg

# Tau - Instant
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Tau_2017B -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Tau_2017C -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Tau_2017D -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Tau_2017E -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Tau_2017F -c config/skim_Legacy2017_mib.cfg

# Muon - Instant
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Mu_2017B -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Mu_2017C -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Mu_2017D -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Mu_2017E -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -d True -T $OUTDIRR -w $WORKDIR/SKIM_Mu_2017F -c config/skim_Legacy2017_mib.cfg

# TT -- fullyLep and semiLep 48h < t < 72h - semiLep longer than fullyLep
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TT_fullyHad -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TT_fullyLep -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TT_semiLep  -c config/skim_Legacy2017_mib.cfg

# DY Low mass - Short
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_M_10_50_Not_PU_Safe -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DYJets_M_10_50_PU_Safe     -c config/skim_Legacy2017_mib.cfg

# EWK - Short
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_EWKWMinus2Jets_WToLNu -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_EWKWPlus2Jets_WToLNu  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_EWKZ2Jets_ZToLL       -c config/skim_Legacy2017_mib.cfg

# SingleTop - Short
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ST_tW_antitop        -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ST_tW_top 	   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ST_tchannel_antitop  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ST_tchannel_top	   -c config/skim_Legacy2017_mib.cfg

# WZ - Short
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WZTo1L1Nu2Q -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WZTo1L3Nu   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WZTo2L2Q    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WZTo3LNu    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WZZ         -c config/skim_Legacy2017_mib.cfg

# ZZ - Short
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZZTo2L2Nu -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZZTo2L2Q  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZZTo2Q2Nu -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZZTo4L    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_ZZZ       -c config/skim_Legacy2017_mib.cfg

# WW - Short
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WWTo2L2Nu -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WWTo4Q    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WWToLNuQQ -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WWW       -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WWZ       -c config/skim_Legacy2017_mib.cfg

# TT+X - Long
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTWJetsToLNu -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTWJetsToQQ  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTWW         -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTWZ         -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTZToLLNuNu  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTZZ         -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTZToQQ      -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTWH         -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_TTZH         -c config/skim_Legacy2017_mib.cfg

# GGF HH NLO - Long
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_NLO_cHHH0_xs    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_NLO_cHHH1_xs    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_NLO_cHHH2p45_xs -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_NLO_cHHH5_xs    -c config/skim_Legacy2017_mib.cfg

# VBF HH - Long
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHH_CV_0p5_C2V_1_C3_1_xs -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHH_CV_1_C2V_0_C3_1_xs   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHH_CV_1_C2V_1_C3_0_xs   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHH_CV_1_C2V_1_C3_1_xs   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHH_CV_1_C2V_1_C3_2_xs   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHH_CV_1_C2V_2_C3_1_xs   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_VBFHH_CV_1p5_C2V_1_C3_1_xs -c config/skim_Legacy2017_mib.cfg

# Single Higgs - Long - ttHToTauTau extremely long
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

# W+Jets - Long
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_70_100    -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_0_70      -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_100_200   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_200_400   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_400_600   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_600_800   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_800_1200  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_1200_2500 -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_WJets_HT_2500_Inf  -c config/skim_Legacy2017_mib.cfg

# DY - Very short: some jobs have much more events than others
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_0b_0JPt   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_0b_100JPt -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_0b_10JPt  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_0b_200JPt -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_0b_30JPt  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_0b_50JPt  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_1b_0JPt   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_1b_100JPt -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_1b_10JPt  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_1b_200JPt -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_1b_30JPt  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_1b_50JPt  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_2b_0JPt   -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_2b_100JPt -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_2b_10JPt  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_2b_200JPt -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_2b_30JPt  -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_DY_2b_50JPt  -c config/skim_Legacy2017_mib.cfg

COMMENT1

# EFT BENCHMARKS JHEP04
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM1  -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM2  -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM3  -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM4  -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM5  -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM6  -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM7  -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM8  -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM9  -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM10 -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM11 -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM12 -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM8a -c config/skim_Legacy2017_mib.cfg
#
## EFT BENCHMARKS JHEP03
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM1b -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM2b -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM3b -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM4b -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM5b -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM6b -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_allNLO_to_BM7b -c config/skim_Legacy2017_mib.cfg
#
## c2 SCAN
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_NLO_c2_3p0  -c config/skim_Legacy2017_mib.cfg
#python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_NLO_c2_0p35 -c config/skim_Legacy2017_mib.cfg
python scripts/systNtuple_mib.py -T $OUTDIRR -w $WORKDIR/SKIM_GGHH_NLO_c2_1p0  -c config/skim_Legacy2017_mib.cfg

<<COMMENT2

COMMENT2
