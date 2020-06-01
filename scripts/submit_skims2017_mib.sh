# LOG
OUTDIRR="Skims_Legacy2017_24Apr2020"

# INPUT
INPUTDIR="inputFiles/Legacy2017_backgrounds"
INPUTDIR_DATA="inputFiles/Legacy2017_data"
INPUTDIR_SIG="inputFiles/Legacy2017_signals"

# OUTPUT
SKIMDIR="/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_06May2020"

# PU weights
PUDIR="/gwpool/users/dzuolo/HHbbtautatuAnalysisLegacy/CMSSW_10_2_16/src/KLUBAnalysis/weights/PUreweight/Legacy_Run2_PU_SF/2017"

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
echo "Submitting - TThad - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 80 -k True -o $SKIMDIR/SKIM_TT_fullyHad -i $INPUTDIR/ttbar_had.txt    -x 377.96 -t True -b 1 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

# TT lep
echo "Submitting - TTlep - "
echo "Submitting - TTlep - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 80 -k True -o $SKIMDIR/SKIM_TT_fullyLep -i $INPUTDIR/ttbar_lep.txt    -x 88.29  -t True -b 4 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

# TT semi
echo "Submitting - TTsemi - "
echo "Submitting - TTsemi - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 80 -k True -o $SKIMDIR/SKIM_TT_semiLep -i $INPUTDIR/ttbar_semilep.txt -x 365.34 -t True -b 5 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt


# # #####################
# Wjets
# tot xs NNLO: 61526.7 pb
# tot xs LO : 52940 pb
# individual XS are taken from : https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#W_jets
# xs LO (HT < 100 ) = 52940 - sum (xs HT > 100) = 52940 pb - 3171.698 pb = 49768.302 pb
# Finally, everything is scaled from the LO to the NNLO xs : (61526.7/52940.) = 1.162197
echo "Submitting - WJets - "
echo "Submitting - WJets - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_0_70      -i $INPUTDIR/WJetsToLNu.txt  -y 1.162197  -x 49768.302 -z 70 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_70_100    -i $INPUTDIR/WJetsToLNu_HT-70To100.txt    -y 1.162197 -x 1292 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_100_200   -i $INPUTDIR/WJetsToLNu_HT-100To200.txt   -y 1.162197 -x 1395 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_200_400   -i $INPUTDIR/WJetsToLNu_HT-200To400.txt   -y 1.162197 -x 407.9 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_400_600   -i $INPUTDIR/WJetsToLNu_HT-400To600.txt   -y 1.162197 -x 57.48 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_600_800   -i $INPUTDIR/WJetsToLNu_HT-600To800.txt   -y 1.162197 -x 12.87 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_800_1200  -i $INPUTDIR/WJetsToLNu_HT-800To1200.txt  -y 1.162197 -x 5.366 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_1200_2500 -i $INPUTDIR/WJetsToLNu_HT-1200To2500.txt -y 1.162197 -x 1.074 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_2500_Inf  -i $INPUTDIR/WJetsToLNu_HT-2500ToInf.txt  -y 1.162197 -x 0.008 -Z 2500 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt


# # #####################
# DY split in bins
# XS of sub-samples: 6077.22 (XS DY NLO) * getBinContent(njet,nb)(outXSsplitCombined_Legacy2017_30Apr2020_No_HT.root)
echo "Submitting - DY LO in bins- "
echo "Submitting - DY LO in bins- " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 80 -k True -o $SKIMDIR/SKIM_DYJets_0j0b -i $INPUTDIR/DY_merged_0j0b.txt -g False -x 4535.019132 -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_0j0b.txt --nj 0 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 80 -k True -o $SKIMDIR/SKIM_DYJets_1j0b -i $INPUTDIR/DY_merged_1j0b.txt -g False -x 957.069231  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_1j0b.txt --nj 1 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 80 -k True -o $SKIMDIR/SKIM_DYJets_1j1b -i $INPUTDIR/DY_merged_1j1b.txt -g False -x 40.074909   -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_1j1b.txt --nj 1 --nb 1 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 80 -k True -o $SKIMDIR/SKIM_DYJets_2j0b -i $INPUTDIR/DY_merged_2j0b.txt -g False -x 323.556226  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_2j0b.txt --nj 2 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 80 -k True -o $SKIMDIR/SKIM_DYJets_2j1b -i $INPUTDIR/DY_merged_2j1b.txt -g False -x 16.780998   -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_2j1b.txt --nj 2 --nb 1 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 80 -k True -o $SKIMDIR/SKIM_DYJets_2j2b -i $INPUTDIR/DY_merged_2j2b.txt -g False -x 6.966098    -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_2j2b.txt --nj 2 --nb 2 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 80 -k True -o $SKIMDIR/SKIM_DYJets_3j0b -i $INPUTDIR/DY_merged_3j0b.txt -g False -x 115.836968  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_3j0b.txt --nj 3 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 80 -k True -o $SKIMDIR/SKIM_DYJets_3j1b -i $INPUTDIR/DY_merged_3j1b.txt -g False -x 4.810418    -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_3j1b.txt --nj 3 --nb 1 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 80 -k True -o $SKIMDIR/SKIM_DYJets_3j2b -i $INPUTDIR/DY_merged_3j2b.txt -g False -x 5.370485    -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_3j2b.txt --nj 3 --nb 2 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 80 -k True -o $SKIMDIR/SKIM_DYJets_3j3b -i $INPUTDIR/DY_merged_3j3b.txt -g False -x 0.060993    -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_3j3b.txt --nj 3 --nb 3 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 80 -k True -o $SKIMDIR/SKIM_DYJets_4j0b -i $INPUTDIR/DY_merged_4j0b.txt -g False -x 64.511923   -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_4j0b.txt --nj 4 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 80 -k True -o $SKIMDIR/SKIM_DYJets_4j1b -i $INPUTDIR/DY_merged_4j1b.txt -g False -x 2.185699    -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_4j1b.txt --nj 4 --nb 1 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 80 -k True -o $SKIMDIR/SKIM_DYJets_4j2b -i $INPUTDIR/DY_merged_4j2b.txt -g False -x 4.899669    -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_4j2b.txt --nj 4 --nb 2 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 80 -k True -o $SKIMDIR/SKIM_DYJets_4j3b -i $INPUTDIR/DY_merged_4j3b.txt -g False -x 0.059609    -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_4j3b.txt --nj 4 --nb 3 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 80 -k True -o $SKIMDIR/SKIM_DYJets_4j4b -i $INPUTDIR/DY_merged_4j4b.txt -g False -x 0.017445    -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_4j4b.txt --nj 4 --nb 4 --DY True


# # #####################
# DY Low Mass
echo "Submitting - DY Low Mass - "
echo "Submitting - DY Low Mass - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 80 -k True -o $SKIMDIR/SKIM_DYJets_M_10_50_Not_PU_Safe -i $INPUTDIR/DYJetsToLL_M-10to50_Not_PU_Safe.txt -x 18610 -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_M_10_50.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 80 -k True -o $SKIMDIR/SKIM_DYJets_M_10_50_PU_Safe     -i $INPUTDIR/DYJetsToLL_M-10to50_PU_Safe.txt     -x 18610 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

COMMENT1

# # #####################
#### ELECTROWEAK
# XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
echo "Submitting - EWK - "
echo "Submitting - EWK - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 80 -k True -o $SKIMDIR/SKIM_EWKWPlus2Jets_WToLNu   -i $INPUTDIR/EWKWPlus2Jets_WToLNu.txt   -x 29.59 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 80 -k True -o $SKIMDIR/SKIM_EWKWMinus2Jets_WToLNu  -i $INPUTDIR/EWKWMinus2Jets_WToLNu.txt  -x 23.24 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 80 -k True -o $SKIMDIR/SKIM_EWKZ2Jets_ZToLL        -i $INPUTDIR/EWKZ2Jets_ZToLL.txt        -x 3.987 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

<<COMMENT2

# # #####################
#### Single Top
# XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
echo "Submitting - SingleTop - "
echo "Submitting - SingleTop - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 60 -k True -o $SKIMDIR/SKIM_ST_tW_antitop       -i $INPUTDIR/ST_tW_antitop.txt        -x 35.85  -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt 

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 60 -k True -o $SKIMDIR/SKIM_ST_tW_top           -i $INPUTDIR/ST_tW_top.txt            -x 35.85  -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt 

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 60 -k True -o $SKIMDIR/SKIM_ST_tchannel_antitop -i $INPUTDIR/ST_t-channel_antitop.txt -x 80.95  -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 60 -k True -o $SKIMDIR/SKIM_ST_tchannel_top     -i $INPUTDIR/ST_t-channel_top.txt     -x 136.02 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt


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
echo "Submitting - SM Higgs - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_ZH_HBB_ZLL    -i $INPUTDIR/ZH_HToBB_ZToLL.txt   -x 0.08781 -y 0.577  -q longcms --pu $PUDIR/PU_Legacy2017_SF_ZH_HToBB_ZToLL.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_ZH_HBB_ZQQ    -i $INPUTDIR/ZH_HToBB_ZToQQ.txt   -x 0.6079  -y 0.577  -q longcms --pu $PUDIR/PU_Legacy2017_SF_ZH_HToBB_ZToQQ.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_ZH_HTauTau    -i $INPUTDIR/ZHToTauTau.txt       -x 0.880   -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WplusHTauTau  -i $INPUTDIR/WplusHToTauTau.txt   -x 0.831   -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2017_SF_WplusHToTauTau.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WminusHTauTau -i $INPUTDIR/WminusHToTauTau.txt  -x 0.527   -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2017_SF_WminusHToTauTau.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_VBFHTauTau    -i $INPUTDIR/VBFHToTauTau.txt     -x 3.766   -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_ggHTauTau     -i $INPUTDIR/GluGluHToTauTau.txt  -x 48.61   -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt


#####################
### Multiboson:
# Some XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
# Some other XS taken from http://cms.cern.ch/iCMS/jsp/db_notes/noteInfo.jsp?cmsnoteid=CMS%20AN-2019/111
echo "Submitting - Multiboson - "
echo "Submitting - Multiboson - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_ZZTo4L      -i $INPUTDIR/ZZTo4L.txt      -x 1.26   -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_ZZTo2L2Nu   -i $INPUTDIR/ZZTo2L2Nu.txt   -x 0.564  -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_ZZTo2L2Q    -i $INPUTDIR/ZZTo2L2Q.txt    -x 5.52   -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_ZZTo2Q2Nu   -i $INPUTDIR/ZZTo2Q2Nu.txt   -x 4.07   -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_WZTo3LNu    -i $INPUTDIR/WZTo3LNu.txt    -x 4.43   -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_WZTo1L1Nu2Q -i $INPUTDIR/WZTo1L1Nu2Q.txt -x 10.71  -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_WZTo1L3Nu   -i $INPUTDIR/WZTo1L3Nu.txt   -x 3.06   -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_WZTo2L2Q    -i $INPUTDIR/WZTo2L2Q.txt    -x 5.595  -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_WWTo2L2Nu   -i $INPUTDIR/WWTo2L2Nu.txt   -x 12.18  -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_WWToLNuQQ   -i $INPUTDIR/WWToLNuQQ.txt   -x 50.00  -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_WWTo4Q      -i $INPUTDIR/WWTo4Q.txt      -x 51.72  -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_ZZZ         -i $INPUTDIR/ZZZ.txt         -x 0.0147 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_WZZ         -i $INPUTDIR/WZZ.txt         -x 0.057  -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_WWW         -i $INPUTDIR/WWW.txt         -x 0.209  -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20  -k True -o $SKIMDIR/SKIM_WWZ         -i $INPUTDIR/WWZ.txt         -x 0.168  -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt


# # #####################
#### TTX :
# HXSWG: xs(ttH) = 0.5071 pb
echo "Submitting - TTX - "
echo "Submitting - TTX - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_ttHJetToBB    -i $INPUTDIR/ttHTobb.txt      -x 0.5071 -y 0.577  -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_ttHJetTononBB -i $INPUTDIR/ttHToNonbb.txt   -x 0.5071 -y 0.3598  -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt --ttHToNonBB True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_ttHToTauTau   -i $INPUTDIR/ttHToTauTau.txt  -x 0.5071 -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_TTWJetsToLNu  -i $INPUTDIR/TTWJetsToLNu.txt -x 0.2043   -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_TTWJetsToQQ   -i $INPUTDIR/TTWJetsToQQ.txt  -x 0.4062   -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_TTWW          -i $INPUTDIR/TTWW.txt         -x 0.006979 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_TTWZ          -i $INPUTDIR/TTWZ.txt         -x 0.002441 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_TTWH          -i $INPUTDIR/TTWH.txt         -x 0.00158  -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_TTZH          -i $INPUTDIR/TTZH.txt         -x 0.00154  -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_TTZToLLNuNu   -i $INPUTDIR/TTZToLLNuNu.txt  -x 0.2529   -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_TTZToQQ       -i $INPUTDIR/TTZToQQ.txt      -x 0.5297   -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_TTZZ          -i $INPUTDIR/TTZZ.txt         -x 0.001386 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt


######################
### DATA :
######################
echo "Submitting - DATA tau - "
echo "Submitting - DATA tau - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Tau_2017B -i $INPUTDIR_DATA/1_Tau__Run2017B-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Tau_2017C -i $INPUTDIR_DATA/1_Tau__Run2017C-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Tau_2017D -i $INPUTDIR_DATA/1_Tau__Run2017D-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Tau_2017E -i $INPUTDIR_DATA/1_Tau__Run2017E-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Tau_2017F -i $INPUTDIR_DATA/1_Tau__Run2017F-31Mar2018-v1.txt -q longcms

echo "Submitting - DATA Mu - "
echo "Submitting - DATA Mu - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Mu_2017B -i $INPUTDIR_DATA/1_SingleMuon__Run2017B-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Mu_2017C -i $INPUTDIR_DATA/1_SingleMuon__Run2017C-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Mu_2017D -i $INPUTDIR_DATA/1_SingleMuon__Run2017D-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Mu_2017E -i $INPUTDIR_DATA/1_SingleMuon__Run2017E-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Mu_2017F -i $INPUTDIR_DATA/1_SingleMuon__Run2017F-31Mar2018-v1.txt -q longcms

echo "Submitting - DATA Electron - "
echo "Submitting - DATA Electron - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Ele_2017B -i $INPUTDIR_DATA/1_SingleElectron__Run2017B-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Ele_2017C -i $INPUTDIR_DATA/1_SingleElectron__Run2017C-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Ele_2017D -i $INPUTDIR_DATA/1_SingleElectron__Run2017D-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Ele_2017E -i $INPUTDIR_DATA/1_SingleElectron__Run2017E-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Ele_2017F -i $INPUTDIR_DATA/1_SingleElectron__Run2017F-31Mar2018-v1.txt -q longcms


#### GGH SM :
# XS:  0.03349 pb^-1
echo "Submitting - GGH SM - "
echo "Submitting - GGH SM - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_GGHHSM        	  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_node_SM.txt       -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_GGHH_node_2   	  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_node_2.txt        -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_GGHH_node_3   	  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_node_3.txt        -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_GGHH_node_4   	  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_node_4.txt        -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_GGHH_node_7   	  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_node_7.txt        -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_GGHH_node_9   	  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_node_9.txt        -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_GGHH_node_12  	  -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_node_12.txt       -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_GGHH_node_cHHH0    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_node_cHHH0.txt    -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_GGHH_node_cHHH1    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_node_cHHH1.txt    -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_GGHH_node_cHHH2p45 -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_node_cHHH2p45.txt -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_GGHH_node_cHHH5    -i $INPUTDIR_SIG/GluGluToHHTo2B2Tau_node_cHHH5.txt    -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt


#### VBF SM :
# XS: 0.001626 pb^-1 from XSBD
echo "Submitting - VBF SM - "
echo "Submitting - VBF SM - " >> log_18Feb2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_18Feb2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_VBFHHSM                 -i $INPUTDIR_SIG/VBFHHTo2B2Tau_CV_1_C2V_1_C3_1.txt   -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_VBFHH_CV_1_C2V_0_C3_2   -i $INPUTDIR_SIG/VBFHHTo2B2Tau_CV_1_C2V_0_C3_2.txt   -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_VBFHH_CV_1_5_C2V_1_C3_1 -i $INPUTDIR_SIG/VBFHHTo2B2Tau_CV_1_5_C2V_1_C3_1.txt -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_VBFHH_CV_1_C2V_1_C3_0   -i $INPUTDIR_SIG/VBFHHTo2B2Tau_CV_1_C2V_1_C3_0.txt   -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_VBFHH_CV_1_C2V_1_C3_2   -i $INPUTDIR_SIG/VBFHHTo2B2Tau_CV_1_C2V_1_C3_2.txt   -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_VBFHH_CV_1_C2V_2_C3_1   -i $INPUTDIR_SIG/VBFHHTo2B2Tau_CV_1_C2V_2_C3_1.txt   -x 1. -a True -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

COMMENT2
