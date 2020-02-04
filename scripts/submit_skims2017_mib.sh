# LOG
OUTDIRR="Skims_Legacy2017_30Jan2020"

# INPUT
INPUTDIR="inputFiles/Legacy2017_backgrounds"
INPUTDIR_DATA="inputFiles/Legacy2017_data"

# OUTPUT
SKIMDIR="/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Legacy2017_02Feb2020"

# PU weights
PUDIR="/gwpool/users/dzuolo/HHbbtautatuAnalysisLegacy/CMSSW_10_2_16/src/KLUBAnalysis/weights/PUreweight/Legacy_Run2_PU_SF/2017"

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

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 80 -k True -o $SKIMDIR/SKIM_TT_fullyHad -i $INPUTDIR/2_TTToHadronic_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2.txt -x 377.96 -t True -b 1 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

# TT lep
echo "Submitting - TTlep - "
echo "Submitting - TTlep - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 80 -k True -o $SKIMDIR/SKIM_TT_fullyLep -i $INPUTDIR/1_TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt -x 88.29 -t True -b 4 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

# TT semi
echo "Submitting - TTsemi - "
echo "Submitting - TTsemi - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 80 -k True -o $SKIMDIR/SKIM_TT_semiLep -i $INPUTDIR/1_TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt -x 365.34 -t True -b 5 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt


# # #####################
# Wjets
echo "Submitting - WJets - "
echo "Submitting - WJets - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets              -i $INPUTDIR/WJetsToLNu_HT-0To100_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14.txt  -y 1.213784 -x 48917.48 -z 100 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_100_200   -i $INPUTDIR/5_WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt  -y 1.213784 -x 1345 -z 100 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_200_400   -i $INPUTDIR/6_WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt  -y 1.213784 -x 359.7 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_400_600   -i $INPUTDIR/7_WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt  -y 1.213784 -x 48.91 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_600_800   -i $INPUTDIR/8_WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -y 1.213784 -x 12.05 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_800_1200  -i $INPUTDIR/9_WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt  -y 1.213784 -x 5.501 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_1200_2500 -i $INPUTDIR/10_WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt  -y 1.213784 -x 1.329 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_WJets_HT_2500_Inf  -i $INPUTDIR/11_WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3.txt  -y 1.213784 -x 0.03216 -Z 2500 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt


# # #####################
# DY split in bins
echo "Submitting - DY LO in bins- "
echo "Submitting - DY LO in bins- " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_DYJets_0j0b -i $INPUTDIR/DY_merged_0j0b.txt -g False -x 4645.6197  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_0j0b.txt --nj 0 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_DYJets_1j0b -i $INPUTDIR/DY_merged_1j0b.txt -g False -x 980.4011  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_1j0b.txt --nj 1 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_DYJets_1j1b -i $INPUTDIR/DY_merged_1j1b.txt -g False -x 41.0524  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_1j1b.txt --nj 1 --nb 1 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_DYJets_2j0b -i $INPUTDIR/DY_merged_2j0b.txt -g False -x 331.4442  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_2j0b.txt --nj 2 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_DYJets_2j1b -i $INPUTDIR/DY_merged_2j1b.txt -g False -x 17.1924  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_2j1b.txt --nj 2 --nb 1 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_DYJets_2j2b -i $INPUTDIR/DY_merged_2j2b.txt -g False -x 7.1342  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_2j2b.txt --nj 2 --nb 2 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_DYJets_3j0b -i $INPUTDIR/DY_merged_3j0b.txt -g False -x 118.6622  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_3j0b.txt --nj 3 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_DYJets_3j1b -i $INPUTDIR/DY_merged_3j1b.txt -g False -x 4.9277  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_3j1b.txt --nj 3 --nb 1 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_DYJets_3j2b -i $INPUTDIR/DY_merged_3j2b.txt -g False -x 5.5015  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_3j2b.txt --nj 3 --nb 2 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_DYJets_3j3b -i $INPUTDIR/DY_merged_3j3b.txt -g False -x 0.0625  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_3j3b.txt --nj 3 --nb 3 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_DYJets_4j0b -i $INPUTDIR/DY_merged_4j0b.txt -g False -x 66.0863  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_4j0b.txt --nj 4 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_DYJets_4j1b -i $INPUTDIR/DY_merged_4j1b.txt -g False -x 2.2380  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_4j1b.txt --nj 4 --nb 1 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_DYJets_4j2b -i $INPUTDIR/DY_merged_4j2b.txt -g False -x 5.0195  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_4j2b.txt --nj 4 --nb 2 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_DYJets_4j3b -i $INPUTDIR/DY_merged_4j3b.txt -g False -x 0.0607  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_4j3b.txt --nj 4 --nb 3 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 30 -k True -o $SKIMDIR/SKIM_DYJets_4j4b -i $INPUTDIR/DY_merged_4j4b.txt -g False -x 0.0177  -q longcms --pu $PUDIR/PU_Legacy2017_SF_DY_LO_4j4b.txt --nj 4 --nb 4 --DY True


# # #####################
#### ELECTROWEAK
echo "Submitting - EWK - "
echo "Submitting - EWK - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_EWKWPlus2Jets_WToLNu   -i $INPUTDIR/9_EWKWPlus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2.txt   -x 25.62 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_EWKWMinus2Jets_WToLNu  -i $INPUTDIR/10_EWKWMinus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2.txt  -x 20.25 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_EWKZ2Jets_ZToLL        -i $INPUTDIR/8_EWKZ2Jets_ZToLL_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2.txt        -x 3.987 -q long    --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_EWKZ2Jets_ZToNuNu      -i $INPUTDIR/1_EWKZ2Jets_ZToNuNu_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2.txt          -x 10.66 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt


# # #####################
#### Single Top
echo "Submitting - SingleTop - "
echo "Submitting - SingleTop - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_ST_tW_antitop       -i $INPUTDIR/7_ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt   -x 34.97 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_ST_tW_top           -i $INPUTDIR/6_ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt       -x 35.6 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_ST_tchannel_antitop -i $INPUTDIR/5_ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt  -x 80.95 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 20 -k True -o $SKIMDIR/SKIM_ST_tchannel_top       -i $INPUTDIR/5_ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt    -x 136.02 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt


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

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ZH_HBB_ZLL -i $INPUTDIR/12_ggZH_HToBB_ZToLL_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.07924 -y 0.577 -q longcms --pu $PUDIR/PU_Legacy2017_SF_ZH.txt

# Fixme ggZH --> XS=? ; -y=?
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ggZH_HToBB_ZToLL -i $INPUTDIR/12_ggZH_HToBB_ZToLL_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.?? -y 0.?? -q longcms --pu $PUDIR/PU_Legacy2017_SF_ggZHtoLL.txt

# Fixme ggZH --> XS=? ; -y=?
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ggZH_HToBB_ZToNuNu -i $INPUTDIR/ggZH_HToBB_ZToNuNu.txt -x 0.?? -y 0.?? -q longcms --pu $PUDIR/PU_Legacy2017_SF_ggZHtoNuNu.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ZH_HTauTau -i $INPUTDIR/13_ZHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.8696 -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ggHTauTau -i $INPUTDIR/17_GluGluHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1.txt -x 21.46 -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_VBFHTauTau -i $INPUTDIR/14_VBFHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt -x 3.861 -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_WplusHTauTau -i $INPUTDIR/15_WplusHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.867 -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2017_SF_Wplus.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_WminusHTauTau -i $INPUTDIR/16_WminusHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.5412 -y 0.0632 -q longcms --pu $PUDIR/PU_Legacy2017_SF_Wminus.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ttHJetToBB -i $INPUTDIR/17_ttHJetTobb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt -x 0.507 -y 0.577 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_ttHJetTononBB -i $INPUTDIR/21_ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt -x 0.507 -y 0.423 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt


#####################
### DI-boson:
#XS taken from here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson
#for Z->4Q, I scaled from ZZ->2nu2Q using BR(2Nu2Q) = 2 * 20% * 69.91%
echo "Submitting - DiBoson - "
echo "Submitting - DiBoson - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_WW  -i $INPUTDIR/3_WW_TuneCP5_13TeV-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 64.28 -q shortcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_WZ  -i $INPUTDIR/2_WZ_TuneCP5_13TeV-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 23.48 -q shortcms --pu $PUDIR/PU_Legacy2017_SF_WZ.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_ZZ  -i $INPUTDIR/4_ZZ_TuneCP5_13TeV-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2.txt -x 10.16 -q shortcms --pu $PUDIR/PU_Legacy2017_SF.txt


#####################
### TRI-boson:
#XS taken from here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson
#for Z->4Q, I scaled from ZZ->2nu2Q using BR(2Nu2Q) = 2 * 20% * 69.91%
echo "Submitting - TriBoson - "
echo "Submitting - TriBoson - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_WWW   -i $INPUTDIR/18_WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.2086  -q shortcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_WZZ   -i $INPUTDIR/19_WZZ_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.05565 -q longcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg  -n 10  -k True -o $SKIMDIR/SKIM_WWZ   -i $INPUTDIR/20_WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.1651 -q shortcms --pu $PUDIR/PU_Legacy2017_SF.txt


# # #####################
#### Others :
echo "Submitting - Others - "
echo "Submitting - Others - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTWJetsToQQ -i $INPUTDIR/21_TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.4062 -q shortcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTWJetsToLNu -i $INPUTDIR/22_TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.2043 -q shortcms --pu $PUDIR/PU_Legacy2017_SF.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_Legacy2017_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTWW -i $INPUTDIR/28_TTWW_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14_ext1-v1.txt -x 0.007834 -q shortcms --pu $PUDIR/PU_Legacy2017_SF.txt


COMMENT1


######################
### DATA :
######################

echo "Submitting - DATA tau - "
echo "Submitting - DATA tau - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Tau_2017B -i $INPUTDIR_DATA/1_Tau__Run2017B-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Tau_2017C -i $INPUTDIR_DATA/2_Tau__Run2017C-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Tau_2017D -i $INPUTDIR_DATA/3_Tau__Run2017D-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Tau_2017E -i $INPUTDIR_DATA/4_Tau__Run2017E-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Tau_2017F -i $INPUTDIR_DATA/5_Tau__Run2017F-31Mar2018-v1.txt -q longcms

echo "Submitting - DATA Mu - "
echo "Submitting - DATA Mu - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Mu_2017B -i $INPUTDIR_DATA/1_SingleMuon__Run2017B-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Mu_2017C -i $INPUTDIR_DATA/2_SingleMuon__Run2017C-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Mu_2017D -i $INPUTDIR_DATA/3_SingleMuon__Run2017D-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Mu_2017E -i $INPUTDIR_DATA/4_SingleMuon__Run2017E-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Mu_2017F -i $INPUTDIR_DATA/5_SingleMuon__Run2017F-31Mar2018-v1.txt -q longcms

echo "Submitting - DATA Electron - "
echo "Submitting - DATA Electron - " >> log_23Jan2020.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_23Jan2020.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Ele_2017B -i $INPUTDIR_DATA/1_SingleElectron__Run2017B-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Ele_2017C -i $INPUTDIR_DATA/2_SingleElectron__Run2017C-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Ele_2017D -i $INPUTDIR_DATA/3_SingleElectron__Run2017D-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Ele_2017E -i $INPUTDIR_DATA/4_SingleElectron__Run2017E-31Mar2018-v1.txt -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -d True -s True -c config/skim_Legacy2017_mib.cfg -n 50 -k True -o $SKIMDIR/SKIM_Ele_2017F -i $INPUTDIR_DATA/5_SingleElectron__Run2017F-31Mar2018-v1.txt -q longcms

