OUTDIRR="Skims_Fall17_MC_DYandWJ"
#OUTDIRR="Skims_Fall17_DATA"

#INPUTDIR="inputFiles/JECproduction_Lug2017"
#INPUTDIR="inputFiles/Fall17_MC/"
#INPUTDIR="/gwpool/users/brivio/Hhh_1718/syncFeb2018/CMSSW_7_4_7/src/KLUBAnalysis/skimNtuples"
#INPUTDIR="inputFiles/Fall17_MC"
#INPUTDIR="inputFiles/Fall17_DATA"
INPUTDIR="inputFiles/Files_June2018"

#SKIMDIR="/gwteraz/users/brivio/SKIMMED_VBF"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_Fall17_MC"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_syncFeb2018"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_20may2018/SKIMMED_Fall17_MC"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_20may2018/SKIMMED_Fall17_DATA"
SKIMDIR="/gwteraz/users/brivio/SKIMMED_11july2018/SKIMMED_Fall17_MC"

source /cvmfs/cms.cern.ch/cmsset_default.sh
source scripts/setup.sh
#mkdir /gwteraz/users/brivio/SKIMMED_Fall17_MC/$OUTDIRR/
mkdir $OUTDIRR
#cp /gwpool/users/brivio/Hhh_1718/CMSSW_7_4_7/src/KLUBAnalysis/scripts/haddAll.sh /gwteraz/users/brivio/SKIMMED_Fall17_MC/$OUTDIRR/



#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync.cfg -n 1  -o $SKIMDIR/SKIM_Graviton450_v3  -i $INPUTDIR/Graviton450_bis.txt -x 1
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync.cfg -n 1  -o $SKIMDIR/SKIM_TauData  -i $INPUTDIR/TauData.txt -x 1 -d True
# <<COMMENT1
# COMMENT1



<<COMMENT1

###################
echo "----- BEGIN -----" >> log.txt

# TT
# TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
# hh = 45.7%
# ll = 10.5%
# hl = 21.9% (x2 for permutation t-tbar)
# technical note: use -q long if splitting on ~180-350 jobs
# TT had
echo "Submitting - TThad - "
echo "Submitting - TThad - " >> log.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 40 -k True -o $SKIMDIR/SKIM_TT_fullyHad -i $INPUTDIR/15_TTToHadronic_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 377.96 -t True -b 1 -q longcms



# TT lep
echo "Submitting - TTlep - "
echo "Submitting - TTlep - " >> log.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 40 -k True -o $SKIMDIR/SKIM_TT_fullyLep -i $INPUTDIR/13_TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 88.29  -t True -b 4 -q longcms




# TT semi
echo "Submitting - TTsemi - "
echo "Submitting - TTsemi - " >> log.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 40 -k True -o $SKIMDIR/SKIM_TT_semiLep -i $INPUTDIR/14_TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 365.34 -t True -b 5 -q longcms


COMMENT1


# # #####################
# DY
echo "Submitting - DY - "
echo "Submitting - DY - " >> log.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 40 -k True -o $SKIMDIR/SKIM_DY -i $INPUTDIR/DYJets.txt -g True -x 5765.4  -q longcms


# # #####################
# Wjets
echo "Submitting - WJets - "
echo "Submitting - WJets - " >> log.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_0_100     -i $INPUTDIR/20_WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2.txt    -y 1.213784 -x 48917.48 -z 100 -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_100_200   -i $INPUTDIR/21_WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt  -y 1.213784 -x 1345 -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_200_400   -i $INPUTDIR/22_WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -y 1.213784 -x 359.7 -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_400_600   -i $INPUTDIR/23_WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -y 1.213784 -x 48.91 -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_600_800   -i $INPUTDIR/24_WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -y 1.213784 -x 12.05 -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_800_1200  -i $INPUTDIR/25_WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt  -y 1.213784 -x 5.501 -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_1200_2500 -i $INPUTDIR/26_WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -y 1.213784 -x 1.329 -q longcms
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 20 -k True -o $SKIMDIR/SKIM_WJets_HT_2500_Inf  -i $INPUTDIR/20_WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v2.txt  -y 1.213784 -x 0.03216 -Z 2500 -q longcms


<<COMMENT2


# # #####################
#### ELECTROWEAK
echo "Submitting - EWK - "
echo "Submitting - EWK - " >> log.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 10 -k True -o $SKIMDIR/SKIM_EWKWPlus2Jets_WToLNu   -i $INPUTDIR/9_EWKWPlus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -x 25.62 -q longcms

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 10 -k True -o $SKIMDIR/SKIM_EWKWMinus2Jets_WToLNu  -i $INPUTDIR/10_EWKWMinus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt  -x 20.25 -q longcms

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 10 -k True -o $SKIMDIR/SKIM_EWKZ2Jets_ZToLL        -i $INPUTDIR/11_EWKZ2Jets_ZToLL_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt        -x 3.987 -q longcms

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 10 -k True -o $SKIMDIR/SKIM_EWKZ2Jets_ZToNuNu      -i $INPUTDIR/12_EWKZ2Jets_ZToNuNu_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt        -x 10.66 -q longcms

# # #####################
#### tW top :
echo "Submitting - SingleTop - "
echo "Submitting - SingleTop - " >> log.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 50 -k True -o $SKIMDIR/SKIM_ST_tW_antitop         -i $INPUTDIR/18_ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -x 34.97 -q longcms

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 50 -k True -o $SKIMDIR/SKIM_ST_tW_top             -i $INPUTDIR/19_ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt       -x 35.6 -q longcms

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 50 -k True -o $SKIMDIR/SKIM_ST_tchannel_antitop   -i $INPUTDIR/16_ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -x 80.95 -q longcms

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 50 -k True -o $SKIMDIR/SKIM_ST_tchannel_top       -i $INPUTDIR/17_ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt       -x 136.02 -q longcms


# # #####################
#### SM Higgs :
### HXSWG: xs(ZH) = 0.8839 pb
### Z->qq : 69.91% , Z->ll : 3,3658% (x3 for all the leptons)
### H->bb : 57.7%  , H->tautau : 6.32%
### ZH (Zll, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
### ZH (Zqq, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
### ZH (Zall, Htautau) : XS teor ZH * BR H->tautau

echo "Submitting - SM Higgs - "
echo "Submitting - SM Higgs - " >> log.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 5 -k True -o $SKIMDIR/SKIM_ZH_HBB_ZLL -i $INPUTDIR/49_ZH_HToBB_ZToLL_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.07924 -y 0.577

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 5 -k True -o $SKIMDIR/SKIM_ZH_HBB_ZQQ -i $INPUTDIR/50_ZH_HToBB_ZToQQ_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.554 -y 0.577

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 5 -k True -o $SKIMDIR/SKIM_ZH_HTauTau -i $INPUTDIR/53_ZHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.8696 -y 0.0632

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 5 -k True -o $SKIMDIR/SKIM_ggHTauTau -i $INPUTDIR/54_GluGluHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 21.46 -y 0.0632

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 5 -k True -o $SKIMDIR/SKIM_VBFHTauTau -i $INPUTDIR/
55_VBFHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 3.861 -y 0.0632

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 5 -k True -o $SKIMDIR/SKIM_WplusHTauTau -i $INPUTDIR/56_WplusHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.867 -y 0.0632

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 5 -k True -o $SKIMDIR/SKIM_WminusHTauTau -i $INPUTDIR/57_WminusHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.5412 -y 0.0632

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 5 -k True -o $SKIMDIR/SKIM_ttHJetTononBB -i $INPUTDIR/63_ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.507 -y 0.423

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 5 -k True -o $SKIMDIR/SKIM_ttHJetToBB -i $INPUTDIR/64_ttHJetTobb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.507 -y 0.577


# # #####################
#### Others :
echo "Submitting - Others - "
echo "Submitting - Others - " >> log.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTWJetsToLNu -i $INPUTDIR/65_TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.2043

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTWJetsToQQ -i $INPUTDIR/66_TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.4062

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTWW -i $INPUTDIR/67_TTWW_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1.txt -x 0.007834

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTWZ -i $INPUTDIR/68_TTWZ_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.002441

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_TTZZ -i $INPUTDIR/72_TTZZ_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.001386




# # #####################
# # ### DI-boson: - 7*7 + 6*5 = 49 + 30 = 79
# # # note2: all njobs listed here were increased by x10 to make them run on the short queue
# # #XS taken from here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson
# ## for Z->4Q, I scaled from ZZ->2nu2Q using BR(2Nu2Q) = 2 * 20% * 69.91%
echo "Submitting - DiBoson - "
echo "Submitting - DiBoson - " >> log.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 6   -k True -o $SKIMDIR/SKIM_ZZTo2L2Q            -i $INPUTDIR/28_ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 3.688

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 6   -k True -o $SKIMDIR/SKIM_ZZTo4L              -i $INPUTDIR/ZZTo4L.txt -x 1.325

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 6   -k True -o $SKIMDIR/SKIM_ZZZ                 -i $INPUTDIR/31_ZZZ_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.01398

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 6   -k True -o $SKIMDIR/SKIM_WWW                 -i $INPUTDIR/32_WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.2086

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 6   -k True -o $SKIMDIR/SKIM_WWZ                 -i $INPUTDIR/33_WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.1651

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 6   -k True -o $SKIMDIR/SKIM_WWTo2L2Nu           -i $INPUTDIR/35_WWTo2L2Nu_NNPDF31_TuneCP5_PSweights_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1.txt -x 11.08

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 6   -k True -o $SKIMDIR/SKIM_WWToLNuQQ           -i $INPUTDIR/WWToLNuQQ.txt -x 45.99

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 6   -k True -o $SKIMDIR/SKIM_WWTo4Q              -i $INPUTDIR/38_WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 47.73

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 6   -k True -o $SKIMDIR/SKIM_SKIM_WZTo3LNu_0Jets -i $INPUTDIR/42_WZTo3LNu_0Jets_MLL-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3.txt -x 0.6368

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 6   -k True -o $SKIMDIR/SKIM_SKIM_WZTo3LNu_1Jets -i $INPUTDIR/43_WZTo3LNu_1Jets_MLL-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.3371

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 6   -k True -o $SKIMDIR/SKIM_SKIM_WZTo3LNu_2Jets -i $INPUTDIR/44_WZTo3LNu_2Jets_MLL-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.06616

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 6   -k True -o $SKIMDIR/SKIM_SKIM_WZTo3LNu_3Jets -i $INPUTDIR/45_WZTo3LNu_3Jets_MLL-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.09558

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 6   -k True -o $SKIMDIR/SKIM_WZTo1L1Nu2Q         -i $INPUTDIR/46_WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 11.66

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 6   -k True -o $SKIMDIR/SKIM_WZTo2L2Q            -i $INPUTDIR/47_WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 6.331

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 6   -k True -o $SKIMDIR/SKIM_WZZ                 -i $INPUTDIR/48_WZZ_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.05565



COMMENT2
<<COMMENTFINAL



################################################################################################################################################################
################################################################################################################################################################
################################################################################################################################################################

# # ####################
### DATA :
#echo "Submitting - DATA tau - "
#echo "Submitting - DATA tau - " >> log.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log.txt

#python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 10 -k False -o $SKIMDIR/SKIM_Tau_2017B -i $INPUTDIR/5_Tau__Run2017B-31Mar2018-v1.txt -q longcms

#python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 10 -k False -o $SKIMDIR/SKIM_Tau_2017F -i $INPUTDIR/6_Tau__Run2017F-31Mar2018-v1.txt -q longcms


#echo "Submitting - DATA Muon - "
#echo "Submitting - DATA Muon - " >> log.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log.txt

#python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 10 -k False -o $SKIMDIR/SKIM_SingleMuon_2017B -i $INPUTDIR/1_SingleMuon__Run2017B-31Mar2018-v1.txt -q longcms

#python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 10 -k False -o $SKIMDIR/SKIM_SingleMuon_2017F -i $INPUTDIR/2_SingleMuon__Run2017F-31Mar2018-v1.txt -q longcms


#echo "Submitting - DATA Electron - "
#echo "Submitting - DATA Electron - " >> log.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log.txt

#python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 10 -k False -o $SKIMDIR/SKIM_SingleElectron_2017B -i $INPUTDIR/3_SingleElectron__Run2017B-31Mar2018-v1.txt -q longcms

#python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 20 -k False -o $SKIMDIR/SKIM_SingleElectron_2017F -i $INPUTDIR/4_SingleElectron__Run2017F-31Mar2018-v1.txt -q longcms

###################################
# VBF signals
#echo "Submitting - VBF SIGNALS - "
#echo "Submitting - VBF SIGNALS - " >> log.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 7 -k True -o $SKIMDIR/$OUTDIRR/VBFHHTo2B2Tau_111 -i $INPUTDIR_VBF/1_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 1.0 -a True -q longcms

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 7 -k True -o $SKIMDIR/$OUTDIRR/VBFHHTo2B2Tau_121 -i $INPUTDIR_VBF/2_VBFHHTo2B2Tau_CV_1_C2V_2_C3_1_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 1.0 -a True  -q longcms

# ggH signals
#echo "Submitting - ggH SIGNALS - "
#echo "Submitting - ggH SIGNALS - " >> log.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 7 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-300 -i $INPUTDIR/84_GluGluToRadionToHHTo2B2Tau_M-300_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt -x 1.0 -a True  -q longcms
#python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg -n 7 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-900 -i $INPUTDIR/93_GluGluToRadionToHHTo2B2Tau_M-900_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 1.0 -a True  -q longcms

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 7 -k False -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_newSM -i $INPUTDIR/allNonResClonesBench/filelist_bench_newSM.txt -x 1.0  --kl 1.0 --kt 1.0 --c2 0.0 --cg 0.0 --c2g  0.0  -q longcms


################################################################################################################
############################################### OLD USED FOR JEC ###############################################
################################################################################################################

# # ### SM Higgs  (ZH) - 3*15 = 45
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 8  -k True -o $SKIMDIR/$OUTDIRR/SKIM_ZH_HTauTau      -i $INPUTDIR/73_ZHToTauTau_M125_13TeV_powheg_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt      -x 0.8839 -y 0.0632
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 8  -k True -o $SKIMDIR/$OUTDIRR/SKIM_ZH_HBB_ZLL      -i $INPUTDIR/ZH_HToBB_ZToLL.txt                                                                                                              -x 0.8839 -y 0.0583
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 8  -k True -o $SKIMDIR/$OUTDIRR/SKIM_ZH_HBB_ZQQ      -i $INPUTDIR/76_ZH_HToBB_ZToQQ_M125_13TeV_powheg_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt  -x 0.8839 -y 0.4034

# # ### Radion signals - 5*13 = 65
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-250   -i $INPUTDIR/81_GluGluToRadionToHHTo2B2Tau_M-250_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-270   -i $INPUTDIR/82_GluGluToRadionToHHTo2B2Tau_M-270_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-280   -i $INPUTDIR/83_GluGluToRadionToHHTo2B2Tau_M-280_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-300   -i $INPUTDIR/84_GluGluToRadionToHHTo2B2Tau_M-300_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt  -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-350   -i $INPUTDIR/85_GluGluToRadionToHHTo2B2Tau_M-350_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-400   -i $INPUTDIR/86_GluGluToRadionToHHTo2B2Tau_M-400_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3.txt  -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-450   -i $INPUTDIR/87_GluGluToRadionToHHTo2B2Tau_M-450_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-500   -i $INPUTDIR/88_GluGluToRadionToHHTo2B2Tau_M-500_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-550   -i $INPUTDIR/89_GluGluToRadionToHHTo2B2Tau_M-550_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-600   -i $INPUTDIR/90_GluGluToRadionToHHTo2B2Tau_M-600_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-650   -i $INPUTDIR/91_GluGluToRadionToHHTo2B2Tau_M-650_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-750   -i $INPUTDIR/92_GluGluToRadionToHHTo2B2Tau_M-750_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-900   -i $INPUTDIR/93_GluGluToRadionToHHTo2B2Tau_M-900_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True

# ### Graviton signals - 19*5=95
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-250   -i $INPUTDIR/94_GluGluToBulkGravitonToHHTo2B2Tau_M-250_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-260   -i $INPUTDIR/95_GluGluToBulkGravitonToHHTo2B2Tau_M-260_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-270   -i $INPUTDIR/96_GluGluToBulkGravitonToHHTo2B2Tau_M-270_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-280   -i $INPUTDIR/97_GluGluToBulkGravitonToHHTo2B2Tau_M-280_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-300   -i $INPUTDIR/98_GluGluToBulkGravitonToHHTo2B2Tau_M-300_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-320   -i $INPUTDIR/99_GluGluToBulkGravitonToHHTo2B2Tau_M-320_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-340   -i $INPUTDIR/100_GluGluToBulkGravitonToHHTo2B2Tau_M-340_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-350   -i $INPUTDIR/101_GluGluToBulkGravitonToHHTo2B2Tau_M-350_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-400   -i $INPUTDIR/102_GluGluToBulkGravitonToHHTo2B2Tau_M-400_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-450   -i $INPUTDIR/103_GluGluToBulkGravitonToHHTo2B2Tau_M-450_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-500   -i $INPUTDIR/104_GluGluToBulkGravitonToHHTo2B2Tau_M-500_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-550   -i $INPUTDIR/105_GluGluToBulkGravitonToHHTo2B2Tau_M-550_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-600   -i $INPUTDIR/106_GluGluToBulkGravitonToHHTo2B2Tau_M-600_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-650   -i $INPUTDIR/107_GluGluToBulkGravitonToHHTo2B2Tau_M-650_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-750   -i $INPUTDIR/108_GluGluToBulkGravitonToHHTo2B2Tau_M-750_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToBulkGravitonToHHTo2B2Tau_M-800   -i $INPUTDIR/109_GluGluToBulkGravitonToHHTo2B2Tau_M-800_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRSGravitonToHHTo2B2Tau_M-300   -i $INPUTDIR/110_GluGluToRSGravitonToHHTo2B2Tau_M-300_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRSGravitonToHHTo2B2Tau_M-650   -i $INPUTDIR/111_GluGluToRSGravitonToHHTo2B2Tau_M-650_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k True -o $SKIMDIR/$OUTDIRR/SKIM_GluGluToRSGravitonToHHTo2B2Tau_M-900   -i $INPUTDIR/112_GluGluToRSGravitonToHHTo2B2Tau_M-900_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True

# #### HH dyn rew 12 benchmarks - 12*5=60
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k False -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_1   -i $INPUTDIR/allNonResClonesBench/filelist_bench_1.txt -x 1.0    --kl  7.5  --kt 1.0 --c2 -1.0 --cg  0.0 --c2g  0.0
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k False -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_2   -i $INPUTDIR/allNonResClonesBench/filelist_bench_2.txt -x 1.0    --kl  1.0  --kt 1.0 --c2  0.5 --cg -0.8 --c2g  0.6
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k False -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_3   -i $INPUTDIR/allNonResClonesBench/filelist_bench_3.txt -x 1.0    --kl  1.0  --kt 1.0 --c2 -1.5 --cg  0.0 --c2g -0.8
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k False -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_4   -i $INPUTDIR/allNonResClonesBench/filelist_bench_4.txt -x 1.0    --kl -3.5  --kt 1.5 --c2 -3.0 --cg  0.0 --c2g  0.0
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k False -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_5   -i $INPUTDIR/allNonResClonesBench/filelist_bench_5.txt -x 1.0    --kl  1.0  --kt 1.0 --c2  0.0 --cg  0.8 --c2g -1.0
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k False -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_6   -i $INPUTDIR/allNonResClonesBench/filelist_bench_6.txt -x 1.0    --kl  2.4  --kt 1.0 --c2  0.0 --cg  0.2 --c2g -0.2
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k False -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_7   -i $INPUTDIR/allNonResClonesBench/filelist_bench_7.txt -x 1.0    --kl  5.0  --kt 1.0 --c2  0.0 --cg  0.2 --c2g -0.2
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k False -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_8   -i $INPUTDIR/allNonResClonesBench/filelist_bench_8.txt -x 1.0    --kl  15.0 --kt 1.0 --c2  0.0 --cg -1.0 --c2g  1.0
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k False -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_9   -i $INPUTDIR/allNonResClonesBench/filelist_bench_9.txt -x 1.0    --kl  1.0  --kt 1.0 --c2  1.0 --cg -0.6 --c2g  0.6
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k False -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_10  -i $INPUTDIR/allNonResClonesBench/filelist_bench_10.txt -x 1.0   --kl 10.0  --kt 1.5 --c2 -1.0 --cg  0.0 --c2g  0.0
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k False -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_11  -i $INPUTDIR/allNonResClonesBench/filelist_bench_11.txt -x 1.0   --kl  2.4  --kt 1.0 --c2  0.0 --cg  1.0 --c2g -1.0
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 2 -k False -o $SKIMDIR/$OUTDIRR/SKIM_HH_benchmark_12  -i $INPUTDIR/allNonResClonesBench/filelist_bench_12.txt -x 1.0   --kl 15.0  --kt 1.0 --c2  1.0 --cg  0.0 --c2g  0.0

#echo "Submitting - diboson - "
#echo "Submitting - diboson - " >> log.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log.txt

# # ### DI-boson: - 7*7 + 6*5 = 49 + 30 = 79
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 80   -k True -o $SKIMDIR/$OUTDIRR/SKIM_WWTo2L2Nu  -i $INPUTDIR/63_WWTo2L2Nu_13TeV-powheg__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt   -x 12.178
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 80   -k True -o $SKIMDIR/$OUTDIRR/SKIM_WWToLNuQQ  -i $INPUTDIR/WWToLNuQQ.txt                                                                                                  -x 49.997
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 80   -k True -o $SKIMDIR/$OUTDIRR/SKIM_WWTo4Q     -i $INPUTDIR/66_WWTo4Q_13TeV-powheg__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt      -x 51.723

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 80   -k True -o $SKIMDIR/$OUTDIRR/SKIM_ZZTo2Q2Nu  -i $INPUTDIR/54_ZZTo2Q2Nu_13TeV_amcatnloFXFX_madspin_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 4.04
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 80   -k True -o $SKIMDIR/$OUTDIRR/SKIM_ZZTo2L2Nu  -i $INPUTDIR/55_ZZTo2L2Nu_13TeV_powheg_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt               -x 0.564
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 80  -k True -o $SKIMDIR/$OUTDIRR/SKIM_ZZTo2L2Q   -i $INPUTDIR/56_ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt  -x 3.22
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 80  -k True -o $SKIMDIR/$OUTDIRR/SKIM_ZZTo4L     -i $INPUTDIR/57_ZZTo4L_13TeV-amcatnloFXFX-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt       -x 1.212
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 80   -k True -o $SKIMDIR/$OUTDIRR/SKIM_ZZTo4Q     -i $INPUTDIR/60_ZZTo4Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt    -x 7.06091

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 80  -k True -o $SKIMDIR/$OUTDIRR/SKIM_WZTo3LNu       -i $INPUTDIR/69_WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt      -x 4.42965
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 80  -k True -o $SKIMDIR/$OUTDIRR/SKIM_WZTo1L3Nu      -i $INPUTDIR/70_WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt    -x 3.033
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 80  -k True -o $SKIMDIR/$OUTDIRR/SKIM_WZTo1L1Nu2Q    -i $INPUTDIR/71_WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3.txt  -x 10.71
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 80  -k True -o $SKIMDIR/$OUTDIRR/SKIM_WZTo2L2Q       -i $INPUTDIR/72_WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt     -x 5.595
#sleep 25m

#echo "Submitting - DYEWK - "
#echo "Submitting - DYEWK - " >> log.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log.txt

# # ## DY
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 120 -k True -o $SKIMDIR/$OUTDIRR/SKIM_DY   -i $INPUTDIR/DY_mergedList.txt -g True -x 5765.4

# # ## ELECTROWEAK
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_EWKWPlus2Jets_WToLNu   -i $INPUTDIR/EWKWPlus2Jets_WToLNu_M-50.txt   -x 25.62
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_EWKWMinus2Jets_WToLNu  -i $INPUTDIR/EWKWMinus2Jets_WToLNu_M-50.txt  -x 20.25
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_EWKZ2Jets_ZToLL        -i $INPUTDIR/EWKZ2Jets_ZToLL_M-50.txt        -x 3.987
#sleep 30m


#echo "Submitting - SingleTop - "
#echo "Submitting - SingleTop - " >> log.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log.txt

# ### tW top :
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_ST_tW_antitop_5f_inclusiveDecays   -i $INPUTDIR/33_ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt   -x 35.6
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_ST_tW_top_5f_inclusiveDecays       -i $INPUTDIR/34_ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt       -x 35.6
#sleep 15m


#echo "Submitting - WJets - "
#echo "Submitting - WJets - " >> log.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log.txt

# ##### ### W+jets in HT bins - 9*7 + 8*2 = 63 + 16 = 79
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 70 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_0_70       -i $INPUTDIR/WJets_incl.txt        -y 1.213784 -x 47493.98 -z 70
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 70 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_70_100     -i $INPUTDIR/WJetsHT-70-100.txt    -y 1.213784 -x 1423.499
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 70 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_100_200    -i $INPUTDIR/WJetsHT-100-200.txt   -y 1.213784 -x 1345
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 70 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_200_400    -i $INPUTDIR/WJetsHT-200-400.txt   -y 1.213784 -x 359.7
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 70 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_400_600    -i $INPUTDIR/WJetsHT-400-600.txt   -y 1.213784 -x 48.91
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 70 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_600_800    -i $INPUTDIR/WJetsHT-600-800.txt   -y 1.213784 -x 12.05
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 70 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_800_1200   -i $INPUTDIR/WJetsHT-800-1200.txt  -y 1.213784 -x 5.501
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 70 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_1200_2500  -i $INPUTDIR/WJetsHT-1200-2500.txt -y 1.213784 -x 1.329
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 70 -k True -o $SKIMDIR/$OUTDIRR/SKIM_WJets_HT_2500_Inf   -i $INPUTDIR/WJetsHT-2500-Inf.txt  -y 1.213784 -x 0.03216
#sleep 45m

#echo "Submitting - TThad - "
#echo "Submitting - TThad - " >> log.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log.txt

# TT had
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 220 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_fullyHad  -i $INPUTDIR/TT_powheg_fullyHad.txt    -x 380.1143 -t True -b 1

#echo "Submitting - TTlep - "
#echo "Submitting - TTlep - " >> log.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log.txt

# TT lep
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 220 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_fullyLep  -i $INPUTDIR/TT_powheg_fullyLep.txt    -x 87.3348  -t True -b 4


#echo "Submitting - TTsemi - "
#echo "Submitting - TTsemi - " >> log.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log.txt

# TT semi
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 220 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_semiLep   -i $INPUTDIR/TT_powheg_semiLep.txt     -x 364.3108 -t True -b 5

#echo "---- FINISHED ---- " >> log.txt
 
COMMENTFINAL



