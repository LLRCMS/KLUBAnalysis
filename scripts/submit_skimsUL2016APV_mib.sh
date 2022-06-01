# LOG
OUTDIRR="SKIMS_UL2016APV_5Apr2022"

# INPUT
INPUTDIR="inputFiles/Resonant2016APV_backgrounds"
INPUTDIR_DATA="inputFiles/Resonant2016APV_data"
INPUTDIR_SIG="inputFiles/Resonant2016APV_signals"

# OUTPUT
SKIMDIR="/gwdata/z/users/dzuolo/ResonantHHbbtautauAnalysis/SKIMS_Res_UL2016APV_5Apr2022"

# PU weights
PUDIR="/gwpool/users/dzuolo/HHbbtautauAnalysisUL/CMSSW_11_1_0_pre6/src/KLUBAnalysis/weights/PUreweight/UL_Run2_PU_SF/2016APV/"

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
echo "Submitting - TT - "
echo "Submitting - TT - " >> log_5Apr2022.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_5Apr2022.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50  -k True -o $SKIMDIR/SKIM_TT_fullyHad -i $INPUTDIR/TTToHadronic.txt     -x 377.96 -t True -b 1 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 200 -k True -o $SKIMDIR/SKIM_TT_fullyLep -i $INPUTDIR/TTTo2L2Nu.txt        -x 88.29  -t True -b 4 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 200 -k True -o $SKIMDIR/SKIM_TT_semiLep  -i $INPUTDIR/TTToSemiLeptonic.txt -x 365.34 -t True -b 5 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt


# # #####################
# Wjets
# tot xs NNLO: 61526.7 pb
# tot xs LO : 50380 pb
# individual XS are taken from : https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#W_jets
# xs LO (HT < 100 ) = 50380 - sum (xs HT > 100) = 50380 pb -  3126.828 pb = 47253.172  pb
# Finally, everything is scaled from the LO to the NNLO xs : (61526.7/50380) = 1.221252481
echo "Submitting - WJets - "
echo "Submitting - WJets - " >> log_5Apr2022.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_5Apr2022.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_WJets_HT_0_70      -i $INPUTDIR/WJetsToLNu.txt               -y 1.221252481 -x 47253.172 -z 70 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_WJets_HT_70_100    -i $INPUTDIR/WJetsToLNu_HT-70To100.txt    -y 1.221252481 -x 1353  -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_WJets_HT_100_200   -i $INPUTDIR/WJetsToLNu_HT-100To200.txt   -y 1.221252481 -x 1346  -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_WJets_HT_200_400   -i $INPUTDIR/WJetsToLNu_HT-200To400.txt   -y 1.221252481 -x 360.1 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_WJets_HT_400_600   -i $INPUTDIR/WJetsToLNu_HT-400To600.txt   -y 1.221252481 -x 48.8  -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_WJets_HT_600_800   -i $INPUTDIR/WJetsToLNu_HT-600To800.txt   -y 1.221252481 -x 12.07 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_WJets_HT_800_1200  -i $INPUTDIR/WJetsToLNu_HT-800To1200.txt  -y 1.221252481 -x 5.497 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_WJets_HT_1200_2500 -i $INPUTDIR/WJetsToLNu_HT-1200To2500.txt -y 1.221252481 -x 1.329 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_WJets_HT_2500_Inf  -i $INPUTDIR/WJetsToLNu_HT-2500ToInf.txt  -y 1.221252481 -x 0.0321 -Z 2500 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt


# # #####################
#### ELECTROWEAK
# XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
echo "Submitting - EWK - "
echo "Submitting - EWK - " >> log_5Apr2022.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_5Apr2022.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True  -n 50 -k True -o $SKIMDIR/SKIM_EWKWPlus2Jets_WToLNu  -i $INPUTDIR/EWKWPlus2Jets_WToLNu_M-50.txt   -x 25.62 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True  -n 50 -k True -o $SKIMDIR/SKIM_EWKWMinus2Jets_WToLNu -i $INPUTDIR/EWKWMinus2Jets_WToLNu_M-50.txt  -x 20.25 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True  -n 50 -k True -o $SKIMDIR/SKIM_EWKZ2Jets_ZToLL       -i $INPUTDIR/EWKZ2Jets_ZToLL_M-50.txt -x 3.987 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt


# # #####################
#### Single Top
# XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
echo "Submitting - SingleTop - "
echo "Submitting - SingleTop - " >> log_5Apr2022.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_5Apr2022.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True  -n 50 -k True -o $SKIMDIR/SKIM_ST_tW_antitop        -i $INPUTDIR/ST_tW_antitop_5f_inclusiveDecays.txt        -x 35.85  -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True  -n 50 -k True -o $SKIMDIR/SKIM_ST_tW_top            -i $INPUTDIR/ST_tW_top_5f_inclusiveDecays.txt            -x 35.85  -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True  -n 50 -k True -o $SKIMDIR/SKIM_ST_t_channel_antitop -i $INPUTDIR/ST_t-channel_antitop_5f_InclusiveDecays.txt -x 80.95  -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True  -n 50 -k True -o $SKIMDIR/SKIM_ST_t_channel_top     -i $INPUTDIR/ST_t-channel_top_5f_InclusiveDecays.txt     -x 136.02 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt


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
echo "Submitting - SM Higgs - " >> log_5Apr2022.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_5Apr2022.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_ZH_HTauTau    -i $INPUTDIR/ZHToTauTau_M125.txt      -x 0.880   -y 0.0632 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_WplusHTauTau  -i $INPUTDIR/WplusHToTauTau_M125.txt  -x 0.831   -y 0.0632 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_WminusHTauTau -i $INPUTDIR/WminusHToTauTau_M125.txt -x 0.527   -y 0.0632 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_ggHTauTau     -i $INPUTDIR/GluGluHToTauTau_M125.txt -x 48.61   -y 0.0632 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_VBFHTauTau    -i $INPUTDIR/VBFHToTauTau_M125.txt    -x 3.766   -y 0.0632 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt


#####################
### Multiboson:
# Triboson taken from http://cms.cern.ch/iCMS/jsp/db_notes/noteInfo.jsp?cmsnoteid=CMS%20AN-2019/111
# Diboson taken from: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson
echo "Submitting - MultiBoson - "
echo "Submitting - MultiBoson - " >> log_5Apr2022.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_5Apr2022.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True  -n 50  -k True -o $SKIMDIR/SKIM_ZZ   -i $INPUTDIR/ZZ.txt  -x 16.523 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True  -n 50  -k True -o $SKIMDIR/SKIM_WZ   -i $INPUTDIR/WZ.txt  -x 47.13  -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True  -n 50  -k True -o $SKIMDIR/SKIM_WW   -i $INPUTDIR/WW.txt  -x 118.7  -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True  -n 50  -k True -o $SKIMDIR/SKIM_ZZZ  -i $INPUTDIR/ZZZ.txt -x 0.0147 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True  -n 50  -k True -o $SKIMDIR/SKIM_WZZ  -i $INPUTDIR/WZZ.txt -x 0.057  -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True  -n 50  -k True -o $SKIMDIR/SKIM_WWW  -i $INPUTDIR/WWW_4F.txt -x 0.209  -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True  -n 50  -k True -o $SKIMDIR/SKIM_WWZ  -i $INPUTDIR/WWZ_4F.txt -x 0.168  -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt


# # #####################
#### TTX :
# HXSWG: xs(ttH) = 0.5071 pb
echo "Submitting - TTX - "
echo "Submitting - TTX - " >> log_5Apr2022.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_5Apr2022.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_ttHJetTononBB -i $INPUTDIR/ttHToNonbb_M125.txt    -x 0.5071 -y 0.423  -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_ttHJetToBB    -i $INPUTDIR/ttHTobb_M125.txt       -x 0.5071 -y 0.577  -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_TTWJetsToLNu  -i $INPUTDIR/TTWJetsToLNu.txt     -x 0.2043   -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_TTWJetsToQQ   -i $INPUTDIR/TTWJetsToQQ.txt      -x 0.4062   -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_TTZToLLNuNu   -i $INPUTDIR/TTZToLLNuNu_M-10.txt -x 0.2529   -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_TTWW          -i $INPUTDIR/TTWW.txt             -x 0.006979 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_TTWZ          -i $INPUTDIR/TTWZ.txt             -x 0.002441 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_TTZZ          -i $INPUTDIR/TTZZ.txt             -x 0.001386 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt


# # #####################
# DY
echo "Submitting - DY - "
echo "Submitting - DY - " >> log_5Apr2022.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_5Apr2022.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 300 -k True -o $SKIMDIR/SKIM_DY -i $INPUTDIR/DY_M-50_Merged.txt -x 6077.22 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt -g True --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 100 -k True -o $SKIMDIR/SKIM_DYInclusive -i $INPUTDIR/DYJetsToLL_M-50.txt -x 6077.22 -q longcms --pu $PUDIR/PU_UL2016APV_SF.txt --DY True

COMMENT1

######################
### DATA :
######################
echo "Submitting - DATA tau - "
echo "Submitting - DATA tau - " >> log_5Apr2022.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_5Apr2022.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_Tau_2016B -i $INPUTDIR_DATA/Tau_Run2016B.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_Tau_2016C -i $INPUTDIR_DATA/Tau_Run2016C.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_Tau_2016D -i $INPUTDIR_DATA/Tau_Run2016D.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_Tau_2016E -i $INPUTDIR_DATA/Tau_Run2016E.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_Tau_2016F -i $INPUTDIR_DATA/Tau_Run2016F.txt -q longcms


echo "Submitting - DATA Mu - "
echo "Submitting - DATA Mu - " >> log_5Apr2022.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_5Apr2022.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_Mu_2016B -i $INPUTDIR_DATA/SingleMuon_Run2016B.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_Mu_2016C -i $INPUTDIR_DATA/SingleMuon_Run2016C.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_Mu_2016D -i $INPUTDIR_DATA/SingleMuon_Run2016D.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_Mu_2016E -i $INPUTDIR_DATA/SingleMuon_Run2016E.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_Mu_2016F -i $INPUTDIR_DATA/SingleMuon_Run2016F.txt -q longcms


echo "Submitting - DATA Electron - "
echo "Submitting - DATA Electron - " >> log_5Apr2022.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_5Apr2022.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_Ele_2016B -i $INPUTDIR_DATA/SingleElectron_Run2016B.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_Ele_2016C -i $INPUTDIR_DATA/SingleElectron_Run2016C.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_Ele_2016D -i $INPUTDIR_DATA/SingleElectron_Run2016D.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_Ele_2016E -i $INPUTDIR_DATA/SingleElectron_Run2016E.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_Ele_2016F -i $INPUTDIR_DATA/SingleElectron_Run2016F.txt -q longcms

echo "Submitting - DATA MET - "
echo "Submitting - DATA MET - " >> log_5Apr2022.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_5Apr2022.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_MET_2016B -i $INPUTDIR_DATA/MET_Run2016B.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_MET_2016C -i $INPUTDIR_DATA/MET_Run2016C.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_MET_2016D -i $INPUTDIR_DATA/MET_Run2016D.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_MET_2016E -i $INPUTDIR_DATA/MET_Run2016E.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_UL2016_mib.cfg  -Y 2016 -A True -n 50 -k True -o $SKIMDIR/SKIM_MET_2016F -i $INPUTDIR_DATA/MET_Run2016F.txt -q longcms
