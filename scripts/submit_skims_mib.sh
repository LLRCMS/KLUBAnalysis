#OUTDIRR="Skims_Fall17_DATA"
#OUTDIRR="Skims_Fall17_MC_TT"
#OUTDIRR="Skims_Fall17_MC_TT2"
#OUTDIRR="Skims_Fall17_MC_DY"
#OUTDIRR="Skims_Fall17_MC_WJets"
#OUTDIRR="Skims_Fall17_MC_WJetsINF"
#OUTDIRR="Skims_Fall17_MC_EWKW"
#OUTDIRR="Skims_Fall17_MC_SingleTop"
#OUTDIRR="Skims_Fall17_MC_Others"
#OUTDIRR="Skims_Fall17_MC_WJetsSpecial"
#OUTDIRR="Skims_Fall17_MC_15July2018"
#OUTDIRR="Skims_Fall17_MC_24July2018"
#OUTDIRR="Skims_Fall17_MC_24July2018_PU"
#OUTDIRR="Skims_Fall17_MC_28Aug2018"
#OUTDIRR="Skims_Fall17_MC_22Oct2018"
#OUTDIRR="Skims_Fall17_MC_6Nov2018"
#OUTDIRR="Skims_Fall17_MC_27Nov2018"
#OUTDIRR="Skims_Fall17_MC_14Dec2018"
#OUTDIRR="Skims_Fall17_MC_7Feb2019"
#OUTDIRR="Skims_Fall17_MC_28Feb2019"
OUTDIRR="Skims_Autumn18_MC_19Jun2019"

#INPUTDIR="inputFiles/JECproduction_Lug2017"
#INPUTDIR="inputFiles/Fall17_MC/"
#INPUTDIR="/gwpool/users/brivio/Hhh_1718/syncFeb2018/CMSSW_7_4_7/src/KLUBAnalysis/skimNtuples"
#INPUTDIR="inputFiles/Fall17_MC"
#INPUTDIR="inputFiles/Fall17_DATA"
#INPUTDIR="inputFiles/Files_June2018"
#INPUTDIR="inputFiles/Files_January2019/finals"
INPUTDIR="inputFiles/Files_June2019/Files_June2019_secondLook"

#SKIMDIR="/gwteraz/users/brivio/SKIMMED_VBF"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_Fall17_MC"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_syncFeb2018"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_20may2018/SKIMMED_Fall17_MC"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_20may2018/SKIMMED_Fall17_DATA"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_15july2018"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_24july2018_PU"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_2aug2018"
#SKIMDIR="/gwpool/users/brivio/Hhh_1718/syncFeb2018/CMSSW_9_0_0/src/KLUBAnalysis/SKIM_DY"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_28aug2018"
#SKIMDIR="/gwpool/users/brivio/Hhh_1718/syncFeb2018/CMSSW_9_0_0/src/KLUBAnalysis/studies/VBFjetChoice/skims_tesi"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_22Oct2018"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_6Nov2018"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_27Nov2018"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_14Dec2018"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_7Feb2019"
#SKIMDIR="/gwteraz/users/brivio/SKIMMED_28Feb2019"
#SKIMDIR="/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_TTBarBKG_ForDeepFlavorEfficiency"
#SKIMDIR="/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_secondLookSkims_deepFlavor"
#SKIMDIR="/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_secondLookSkims_deepCSV"
#SKIMDIR="/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_Prova"
#SKIMDIR="/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_thirdLookSkims_deepFlavor"
SKIMDIR="/gwteraz/users/dzuolo/HHbbtautauAnalysis/SKIMMED_thirdLookSkims_deepCSV"

#PUDIR="/gwpool/users/brivio/Hhh_1718/syncFeb2018/CMSSW_9_0_0/src/KLUBAnalysis/weights/PUreweight/outputs"
PUDIR="/gwpool/users/dzuolo/HbbtautauAnalysis2018/CMSSW_9_0_0/src/KLUBAnalysis/weights/PUreweight"

source /cvmfs/cms.cern.ch/cmsset_default.sh
source scripts/setup.sh
mkdir $OUTDIRR

#### GGH SM :
# XS:  0.03349 pb^-1
echo "Submitting - GGH SM - "
echo "Submitting - GGH SM - " >> log_26Aug2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_26Aug2019.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2018_sync_mib.cfg  -n 13 -k True -o $SKIMDIR/SKIM_GGHSM -i $INPUTDIR/1_GluGluToHHTo2B2Tau_node_SM_TuneCP5_PSWeights_13TeV-madgraph-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 0.03349 -a True -q longcms --pu $PUDIR/2018_PuReWeight_SF.txt

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
echo "Submitting - TThad - " >> log_26Aug2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_26Aug2019.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2018_sync_mib.cfg -n 13 -k True -o $SKIMDIR/SKIM_TT_fullyHad -i $INPUTDIR/4_TTToHadronic_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 377.96 -t True -b 1 -q longcms --pu $PUDIR/2018_PuReWeight_SF.txt

# TT lep
echo "Submitting - TTlep - "
echo "Submitting - TTlep - " >> log_26Aug2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_26Aug2019.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2018_sync_mib.cfg -n 13 -k True -o $SKIMDIR/SKIM_TT_fullyLep -i $INPUTDIR/1_TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 88.29  -t True -b 4 -q longcms --pu $PUDIR/2018_PuReWeight_SF.txt

# TT semi
echo "Submitting - TTsemi - "
echo "Submitting - TTsemi - " >> log_26Aug2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_26Aug2019.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2018_sync_mib.cfg -n 13 -k True -o $SKIMDIR/SKIM_TT_semiLep -i $INPUTDIR/3_TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1.txt -x 365.34 -t True -b 5 -q longcms --pu $PUDIR/2018_PuReWeight_SF.txt

# # #####################
# Wjets
echo "Submitting - WJets - "
echo "Submitting - WJets - " >> log_26Aug2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_26Aug2019.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2018_sync_mib.cfg -n 13 -k True -o $SKIMDIR/SKIM_WJets_Inclusive -i $INPUTDIR/1_WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2.txt -x 52940.0 -q longcms --pu $PUDIR/2018_PuReWeight_SF.txt

# #####################
# DY NLO
# total XS= 6225.42 pb - 8 october 2018
echo "Submitting - DY NLO - "
echo "Submitting - DY NLO - " >> log_26Aug2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_26Aug2019.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2018_sync_mib.cfg -n 13 -k True -o $SKIMDIR/SKIM_DY_NLO_NewSF_Huge -i $INPUTDIR/1_DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1-All.txt -x 6225.42 -q longcms --pu $PUDIR/2018_PuReWeight_SF.txt  --DY True

### DATA :
echo "Submitting - DATA tau - "
echo "Submitting - DATA tau - " >> log_26Aug2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_26Aug2019.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2018_sync_mib.cfg -n 13 -k True -o $SKIMDIR/SKIM_Tau_2018B -i $INPUTDIR/1_Tau__Run2018B-17Sep2018-v1.txt -q longcms

### DATA :
echo "Submitting - DATA Mu - "
echo "Submitting - DATA Mu - " >> log_26Aug2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_26Aug2019.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2018_sync_mib.cfg -n 13 -k True -o $SKIMDIR/SKIM_Mu_2018B -i $INPUTDIR/2_SingleMuon__Run2018B-17Sep2018-v1.txt -q longcms

echo "Submitting - DATA Electron - "
echo "Submitting - DATA Electron - " >> log_26Aug2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_26Aug2019.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2018_sync_mib.cfg -n 13 -k True -o $SKIMDIR/SKIM_EGamma_2018B -i $INPUTDIR/1_EGamma__Run2018B-17Sep2018-v1.txt -q longcms

COMMENT1

<<COMMENT2

# # #####################
# DY
echo "Submitting - DY LO - "
echo "Submitting - DY LO - " >> log_28feb2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

#old stayle DY (no PU sample-by-sample, no LOtoNLOreweight, no split in nB and nJets)
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 40 -k True -o $SKIMDIR/SKIM_DY_weights24Sep2018 -i $INPUTDIR/DYJets.txt -g True -x 5765.4  -q shortcms

#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 15 -k True -o $SKIMDIR/SKIM_DY -i $INPUTDIR/DYJets.txt -g True -x 5765.4  -q longcms

#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_DYJetsToLL_XS -i $INPUTDIR/DYJetsToLL.txt -g True -x 5765.4  -q longcms --pu $PUDIR/DYJetsToLL.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 10 -k True -o $SKIMDIR/SKIM_DY1JetsToLL_XS -i $INPUTDIR/3_DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1.txt -g True -x 877.8  -q longcms --pu $PUDIR/3_DY1JetsToLL.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 6 -k True -o $SKIMDIR/SKIM_DY2JetsToLL_XS -i $INPUTDIR/DY2JetsToLL.txt -g True -x 304.4  -q shortcms --pu $PUDIR/DY2JetsToLL.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 6 -k True -o $SKIMDIR/SKIM_DY3JetsToLL_XS -i $INPUTDIR/6_DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1.txt -g True -x 111.5  -q shortcms --pu $PUDIR/6_DY3JetsToLL.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 6 -k True -o $SKIMDIR/SKIM_DY4JetsToLL_XS -i $INPUTDIR/7_DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -g True -x 44.03  -q shortcms --pu $PUDIR/7_DY4JetsToLL.txt
#python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 6 -k True -o $SKIMDIR/SKIM_DYBBJetsToLL_XS -i $INPUTDIR/8_DYBBJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -g True -x 14.49  -q shortcms --pu $PUDIR/8_DYBBJetsToLL.txt

# myPUHisto_0j0b :  72169268.0  - frac:  0.495938761847     - XS:  2859.28533755
# myPUHisto_1j0b :  48082351.0  - frac:  0.330416287742     - XS:  1904.98206535
# myPUHisto_1j1b :  2013682.0   - frac:  0.0138377869904    - XS:  79.7803771142
# myPUHisto_2j0b :  13996883.0  - frac:  0.0961849415563    - XS:  554.544662049
# myPUHisto_2j1b :  726473.0    - frac:  0.00499223741795   - XS:  28.7822456094
# myPUHisto_2j2b :  1440197.0   - frac:  0.00989686520024   - XS:  57.0593866255
# myPUHisto_3j0b :  2899365.0   - frac:  0.0199240968918    - XS:  114.87038822
# myPUHisto_3j1b :  119898.0    - frac:  0.000823925021215  - XS:  4.75025731731
# myPUHisto_3j2b :  1035377.0   - frac:  0.00711498954687   - XS:  41.0207607335
# myPUHisto_3j3b :  11733.0     - frac:  8.06278025815e-05  - XS:  0.464851533003
# myPUHisto_4j0b :  2129640.0   - frac:  0.0146346367928    - XS:  84.3745349651
# myPUHisto_4j1b :  70735.0     - frac:  0.000486082640041  - XS:  2.80246085289
# myPUHisto_4j2b :  809829.0    - frac:  0.00556505009263   - XS:  32.084739804
# myPUHisto_4j3b :  11952.0     - frac:  8.21327449462e-05  - XS:  0.473528127713
# myPUHisto_4j4b :  3140.0      - frac:  2.1577712444e-05   - XS:  0.124404143325
# total XS= 5765.4 pb
# myPUHisto_0j0b : xsFrac: 0.697167 - xs: 4019.4 pb
# myPUHisto_1j0b : xsFrac: 0.196537 - xs: 1133.1 pb
# myPUHisto_1j1b : xsFrac: 0.007292 - xs: 42.04  pb
# myPUHisto_2j0b : xsFrac: 0.063167 - xs: 364.18 pb
# myPUHisto_2j1b : xsFrac: 0.003200 - xs: 18.5   pb
# myPUHisto_2j2b : xsFrac: 0.001070 - xs: 6.17   pb
# myPUHisto_3j0b : xsFrac: 0.018860 - xs: 108.73 pb
# myPUHisto_3j1b : xsFrac: 0.000887 - xs: 5.11   pb
# myPUHisto_3j2b : xsFrac: 0.000704 - xs: 4.06   pb
# myPUHisto_3j3b : xsFrac: 9.78e-06 - xs: 0.056  pb
# myPUHisto_4j0b : xsFrac: 0.010130 - xs: 58.40  pb
# myPUHisto_4j1b : xsFrac: 0.000425 - xs: 2.45   pb
# myPUHisto_4j2b : xsFrac: 0.000535 - xs: 3.08   pb
# myPUHisto_4j3b : xsFrac: 1.12e-05 - xs: 0.064  pb
# myPUHisto_4j4b : xsFrac: 1.98e-06 - xs: 0.011  pb
# total XS= 6233.55 pb - 8 october 2018
# myPUHisto_0j0b : xsFrac: 0.746053    - xs: 4650.56 pb
# myPUHisto_1j0b : xsFrac: 0.157576    - xs: 982.26 pb
# myPUHisto_1j1b : xsFrac: 0.00658535  - xs: 41.05 pb
# myPUHisto_2j0b : xsFrac: 0.0534837   - xs: 333.39 pb
# myPUHisto_2j1b : xsFrac: 0.00273262  - xs: 17.03 pb
# myPUHisto_2j2b : xsFrac: 0.00118358  - xs: 7.38 pb
# myPUHisto_3j0b : xsFrac: 0.0188891   - xs: 117.75 pb
# myPUHisto_3j1b : xsFrac: 0.000781306 - xs: 4.87 pb
# myPUHisto_3j2b : xsFrac: 0.000881659 - xs: 5.50 pb
# myPUHisto_3j3b : xsFrac: 9.87433E-06 - xs: 0.061 pb
# myPUHisto_4j0b : xsFrac: 0.0106378   - xs: 66.31 pb
# myPUHisto_4j1b : xsFrac: 0.000360878 - xs: 2.25 pb
# myPUHisto_4j2b : xsFrac: 0.000811201 - xs: 5.06 pb
# myPUHisto_4j3b : xsFrac: 1.0322E-05  - xs: 0.064 pb
# myPUHisto_4j4b : xsFrac: 2.77862E-05 - xs: 0.17 pb
# total XS= 6225.42 pb - 8 october 2018
# myPUHisto_0j0b : xsFrac: 0.746053    - xs: 4644.49 pb
# myPUHisto_1j0b : xsFrac: 0.157576    - xs: 980.98  pb
# myPUHisto_1j1b : xsFrac: 0.00658535  - xs: 41.00   pb
# myPUHisto_2j0b : xsFrac: 0.0534837   - xs: 332.96  pb
# myPUHisto_2j1b : xsFrac: 0.00273262  - xs: 17.01   pb
# myPUHisto_2j2b : xsFrac: 0.00118358  - xs: 7.37    pb
# myPUHisto_3j0b : xsFrac: 0.0188891   - xs: 117.59  pb
# myPUHisto_3j1b : xsFrac: 0.000781306 - xs: 4.86    pb
# myPUHisto_3j2b : xsFrac: 0.000881659 - xs: 5.49    pb
# myPUHisto_3j3b : xsFrac: 9.87433E-06 - xs: 0.06    pb
# myPUHisto_4j0b : xsFrac: 0.0106378   - xs: 66.22   pb
# myPUHisto_4j1b : xsFrac: 0.000360878 - xs: 2.25    pb
# myPUHisto_4j2b : xsFrac: 0.000811201 - xs: 5.05    pb
# myPUHisto_4j3b : xsFrac: 1.0322E-05  - xs: 0.06    pb
# myPUHisto_4j4b : xsFrac: 2.77862E-05 - xs: 0.17    pb
# total XS= 6225.42 pb - 5 march 2019
# myPUHisto_0j0b : xsFrac: 0.746231    - xs: 4645.60 pb
# myPUHisto_1j0b : xsFrac: 0.157487    - xs: 980.42  pb
# myPUHisto_1j1b : xsFrac: 0.00659452  - xs: 41.05   pb
# myPUHisto_2j0b : xsFrac: 0.0532405   - xs: 331.44  pb
# myPUHisto_2j1b : xsFrac: 0.0027623   - xs: 17.20   pb
# myPUHisto_2j2b : xsFrac: 0.00114651  - xs: 7.14    pb
# myPUHisto_3j0b : xsFrac: 0.0190577   - xs: 118.64  pb
# myPUHisto_3j1b : xsFrac: 0.000788278 - xs: 4.91    pb
# myPUHisto_3j2b : xsFrac: 0.000889527 - xs: 5.54    pb
# myPUHisto_3j3b : xsFrac: 9.96244E-06 - xs: 0.062   pb
# myPUHisto_4j0b : xsFrac: 0.010611    - xs: 66.06   pb
# myPUHisto_4j1b : xsFrac: 0.000359969 - xs: 2.24    pb
# myPUHisto_4j2b : xsFrac: 0.000809156 - xs: 5.04    pb
# myPUHisto_4j3b : xsFrac: 1.0296E-05  - xs: 0.064   pb
# myPUHisto_4j4b : xsFrac: 2.77162E-06 - xs: 0.017   pb

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 11 -k False -o $SKIMDIR/SKIM_DYJets_0j0b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 4645.60  -q longcms --pu $PUDIR/DY_special/DY_0j0b.txt --nj 0 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 11 -k False -o $SKIMDIR/SKIM_DYJets_1j0b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 980.42  -q longcms --pu $PUDIR/DY_special/DY_1j0b.txt --nj 1 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 3 -k False -o $SKIMDIR/SKIM_DYJets_1j1b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 41.05  -q longcms --pu $PUDIR/DY_special/DY_1j1b.txt --nj 1 --nb 1 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 20 -k False -o $SKIMDIR/SKIM_DYJets_2j0b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 331.44  -q shortcms --pu $PUDIR/DY_special/DY_2j0b.txt --nj 2 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 3 -k False -o $SKIMDIR/SKIM_DYJets_2j1b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 17.20  -q shortcms --pu $PUDIR/DY_special/DY_2j1b.txt --nj 2 --nb 1 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 3 -k False -o $SKIMDIR/SKIM_DYJets_2j2b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 7.14  -q shortcms --pu $PUDIR/DY_special/DY_2j2b.txt --nj 2 --nb 2 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_3j0b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 118.64  -q longcms --pu $PUDIR/DY_special/DY_3j0b.txt --nj 3 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_3j1b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 4.91  -q longcms --pu $PUDIR/DY_special/DY_3j1b.txt --nj 3 --nb 1 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_3j2b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 5.54  -q longcms --pu $PUDIR/DY_special/DY_3j2b.txt --nj 3 --nb 2 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_3j3b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 0.062  -q longcms --pu $PUDIR/DY_special/DY_3j3b.txt --nj 3 --nb 3 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_4j0b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 66.06  -q longcms --pu $PUDIR/DY_special/DY_4j0b.txt --nj 4 --nb 0 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_4j1b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 2.24  -q longcms --pu $PUDIR/DY_special/DY_4j1b.txt --nj 4 --nb 1 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_4j2b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 5.04  -q longcms --pu $PUDIR/DY_special/DY_4j2b.txt --nj 4 --nb 2 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_4j3b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 0.064  -q longcms --pu $PUDIR/DY_special/DY_4j3b.txt --nj 4 --nb 3 --DY True

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_DYJets_4j4b -i $INPUTDIR/DYJetsToLL_LO.txt -g False -x 0.017  -q longcms --pu $PUDIR/DY_special/DY_4j4b.txt --nj 4 --nb 4 --DY True

# # #####################
# DY NLO
# total XS= 6225.42 pb - 8 october 2018
echo "Submitting - DY NLO - "
echo "Submitting - DY NLO - " >> log_28feb2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 45 -k False -o $SKIMDIR/SKIM_DY_NLO -i $INPUTDIR/DYJetsToLL_NLO.txt -g False -x 6225.42 -q longcms --pu $PUDIR/DYJetsToLL_NLO.txt  --DY True

# # #####################
# Wjets
echo "Submitting - WJets - "
echo "Submitting - WJets - " >> log_28feb2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_WJets_HT_0_100     -i $INPUTDIR/WJetsToLNu_HT-0To100.txt    -y 1.213784 -x 48917.48 -z 100 -q longcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_WJets_HT_100_200   -i $INPUTDIR/31_WJetsToLNu_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt  -y 1.213784 -x 1345 -q longcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_WJets_HT_200_400   -i $INPUTDIR/32_WJetsToLNu_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -y 1.213784 -x 359.7 -q longcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_WJets_HT_400_600   -i $INPUTDIR/33_WJetsToLNu_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -y 1.213784 -x 48.91 -q longcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_WJets_HT_600_800   -i $INPUTDIR/34_WJetsToLNu_HT-600To800_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -y 1.213784 -x 12.05 -q longcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_WJets_HT_800_1200  -i $INPUTDIR/35_WJetsToLNu_HT-800To1200_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt  -y 1.213784 -x 5.501 -q longcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_WJets_HT_1200_2500 -i $INPUTDIR/36_WJetsToLNu_HT-1200To2500_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt  -y 1.213784 -x 1.329 -q longcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_WJets_HT_2500_Inf  -i $INPUTDIR/37_WJetsToLNu_HT-2500ToInf_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3.txt  -y 1.213784 -x 0.03216 -Z 2500 -q longcms --pu $PUDIR/PUsafe_weights.txt

# # #####################
#### ELECTROWEAK
echo "Submitting - EWK - "
echo "Submitting - EWK - " >> log_28feb2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 10 -k False -o $SKIMDIR/SKIM_EWKWPlus2Jets_WToLNu   -i $INPUTDIR/13_EWKWPlus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt   -x 25.62 -q longcms --pu $PUDIR/EWKWPlus2Jets_WToLNu.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 10 -k False -o $SKIMDIR/SKIM_EWKWMinus2Jets_WToLNu  -i $INPUTDIR/14_EWKWMinus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt  -x 20.25 -q longcms --pu $PUDIR/EWKWMinus2Jets_WToLNu.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 10 -k False -o $SKIMDIR/SKIM_EWKZ2Jets_ZToLL        -i $INPUTDIR/15_EWKZ2Jets_ZToLL_M-50_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt        -x 3.987 -q shortcms --pu $PUDIR/EWKZ2Jets_ZToLL.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 10 -k False -o $SKIMDIR/SKIM_EWKZ2Jets_ZToNuNu      -i $INPUTDIR/16_EWKZ2Jets_ZToNuNu_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt        -x 10.66 -q longcms --pu $PUDIR/EWKZ2Jets_ZToNuNu.txt

# # #####################
#### tW top :
echo "Submitting - SingleTop - "
echo "Submitting - SingleTop - " >> log_28feb2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 10 -k False -o $SKIMDIR/SKIM_ST_tW_antitop         -i $INPUTDIR/ST_tW_antitop.txt   -x 34.97 -q longcms --pu $PUDIR/ST_tW_antitop.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 10 -k False -o $SKIMDIR/SKIM_ST_tW_top             -i $INPUTDIR/ST_tW_top.txt       -x 35.6 -q longcms --pu $PUDIR/ST_tW_top.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 10 -k False -o $SKIMDIR/SKIM_ST_tchannel_antitop   -i $INPUTDIR/23_ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt   -x 80.95 -q longcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 10 -k False -o $SKIMDIR/SKIM_ST_tchannel_top       -i $INPUTDIR/24_ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt       -x 136.02 -q longcms --pu $PUDIR/PUsafe_weights.txt

# # #####################
#### SM Higgs :
### HXSWG: xs(ZH) = 0.8839 pb
### Z->qq : 69.91% , Z->ll : 3,3658% (x3 for all the leptons)
### H->bb : 57.7%  , H->tautau : 6.32%
### ZH (Zll, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
### ZH (Zqq, Hbb)      : XSBD (xs ZH * BR Z) * H->bb
### ZH (Zall, Htautau) : XS teor ZH * BR H->tautau

echo "Submitting - SM Higgs - "
echo "Submitting - SM Higgs - " >> log_28feb2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_ZH_HBB_ZLL -i $INPUTDIR/59_ZH_HToBB_ZToLL_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.07924 -y 0.577 -q longcms --pu $PUDIR/ZH_HToBB_ZToLL.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_ZH_HBB_ZQQ -i $INPUTDIR/60_ZH_HToBB_ZToQQ_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.554 -y 0.577 -q longcms --pu $PUDIR/ZH_HToBB_ZToQQ.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_ZH_HTauTau -i $INPUTDIR/63_ZHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.8696 -y 0.0632 -q longcms --pu $PUDIR/ZHToTauTau.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_ggHTauTau -i $INPUTDIR/GluGluHToTauTau.txt -x 21.46 -y 0.0632 -q longcms --pu $PUDIR/GluGluHToTauTau.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_WplusHTauTau -i $INPUTDIR/67_WplusHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.867 -y 0.0632 -q longcms --pu $PUDIR/WplusHToTauTau.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_VBFHTauTau -i $INPUTDIR/66_VBFHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 3.861 -y 0.0632 -q longcms --pu $PUDIR/VBFHToTauTau.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_WminusHTauTau -i $INPUTDIR/68_WminusHToTauTau_M125_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.5412 -y 0.0632 -q longcms --pu $PUDIR/WminusHToTauTau.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_ttHJetTononBB -i $INPUTDIR/69_ttHJetToNonbb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt -x 0.507 -y 0.423 -q longcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 2 -k False -o $SKIMDIR/SKIM_ttHJetToBB -i $INPUTDIR/70_ttHJetTobb_M125_TuneCP5_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt -x 0.507 -y 0.577 -q longcms --pu $PUDIR/PUsafe_weights.txt

# # #####################
#### Others :
echo "Submitting - Others - "
echo "Submitting - Others - " >> log_28feb2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 5 -k False -o $SKIMDIR/SKIM_TTWJetsToLNu -i $INPUTDIR/71_TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.2043 -q shortcms --pu $PUDIR/TTWJetsToLNu.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 5 -k False -o $SKIMDIR/SKIM_TTWJetsToQQ -i $INPUTDIR/72_TTWJetsToQQ_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.4062 -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 5 -k False -o $SKIMDIR/SKIM_TTWW -i $INPUTDIR/73_TTWW_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14_ext1-v1.txt -x 0.007834 -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 5 -k False -o $SKIMDIR/SKIM_TTWZ -i $INPUTDIR/74_TTWZ_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt -x 0.002441 -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg -n 5 -k False -o $SKIMDIR/SKIM_TTZZ -i $INPUTDIR/78_TTZZ_TuneCP5_13TeV-madgraph-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt -x 0.001386 -q shortcms --pu $PUDIR/PUsafe_weights.txt

# # #####################
# # ### DI-boson: - 7*7 + 6*5 = 49 + 30 = 79
# # # note2: all njobs listed here were increased by x10 to make them run on the short queue
# # #XS taken from here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson
# ## for Z->4Q, I scaled from ZZ->2nu2Q using BR(2Nu2Q) = 2 * 20% * 69.91%
echo "Submitting - DiBoson - "
echo "Submitting - DiBoson - " >> log_28feb2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt


python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 5   -k False -o $SKIMDIR/SKIM_ZZZ                 -i $INPUTDIR/41_ZZZ_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.01398 -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 5   -k False -o $SKIMDIR/SKIM_WWW                 -i $INPUTDIR/42_WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.2086  -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 5   -k False -o $SKIMDIR/SKIM_WWZ                 -i $INPUTDIR/43_WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.1651 -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 5   -k False -o $SKIMDIR/SKIM_WWTo2L2Nu           -i $INPUTDIR/45_WWTo2L2Nu_NNPDF31_TuneCP5_PSweights_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1.txt -x 11.08 -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 5   -k False -o $SKIMDIR/SKIM_WWToLNuQQ           -i $INPUTDIR/WWToLNuQQ.txt -x 45.99 -q longcms --pu $PUDIR/WWToLNuQQ.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 5   -k False -o $SKIMDIR/SKIM_WWTo4Q              -i $INPUTDIR/48_WWTo4Q_NNPDF31_TuneCP5_13TeV-powheg-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt -x 47.73  -q longcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 5   -k False -o $SKIMDIR/SKIM_WZTo3LNu -i $INPUTDIR/51_WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1.txt  -x 5.052 -q longcms --pu $PUDIR/PUsafe_weights.txt

##python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 5   -k False -o $SKIMDIR/SKIM_WZTo3LNu_0Jets -i $INPUTDIR/1_WZTo3LNu_0Jets_MLL-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3.txt -x 0.6368 -q shortcms --pu $PUDIR/PUsafe_weights.txt

##python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 8   -k False -o $SKIMDIR/SKIM_WZTo3LNu_1Jets -i $INPUTDIR/2_WZTo3LNu_1Jets_MLL-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.3371 -q longcms --pu $PUDIR/PUsafe_weights.txt

##MISSING: python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 8   -k False -o $SKIMDIR/SKIM_WZTo3LNu_2Jets -i $INPUTDIR/44_WZTo3LNu_2Jets_MLL-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.06616 -q longcms --pu $PUDIR/44_WZTo3LNu_2Jets.txt

##MISSING: python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 8   -k False -o $SKIMDIR/SKIM_WZTo3LNu_3Jets -i $INPUTDIR/45_WZTo3LNu_3Jets_MLL-50_TuneCP5_13TeV-madgraphMLM-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.09558  -q longcms --pu $PUDIR/45_WZTo3LNu_3Jets.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 5   -k False -o $SKIMDIR/SKIM_WZTo1L1Nu2Q         -i $INPUTDIR/56_WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 11.66 -q longcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 5   -k False -o $SKIMDIR/SKIM_WZTo2L2Q            -i $INPUTDIR/57_WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 6.331 -q longcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 5   -k False -o $SKIMDIR/SKIM_WZZ                 -i $INPUTDIR/58_WZZ_TuneCP5_13TeV-amcatnlo-pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.05565 -q longcms --pu $PUDIR/WZZ.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 20   -k False -o $SKIMDIR/SKIM_ZZTo4L              -i $INPUTDIR/40_ZZTo4L_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 1.325 -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 20   -k False -o $SKIMDIR/SKIM_ZZTo2L2Q            -i $INPUTDIR/39_ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 3.688 -q longcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 15   -k False -o $SKIMDIR/SKIM_ZZTo2L2Nu            -i $INPUTDIR/38_ZZTo2L2Nu_13TeV_powheg_pythia8__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.5644 -q shortcms --pu $PUDIR/PUsafe_weights.txt

#### VBF SM :
# XS: 0.001626 pb^-1 from XSBD
echo "Submitting - VBF SM - "
echo "Submitting - VBF SM - " >> log_28feb2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 10   -k True -o $SKIMDIR/SKIM_VBFSM -i $INPUTDIR/signals/46_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 0.001626 -a True -q shortcms --pu $PUDIR/PUsafe_weights.txt

#### GGH SM :
# XS:  0.03349 pb^-1
echo "Submitting - GGH SM - "
echo "Submitting - GGH SM - " >> log_28feb2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 5   -k True -o $SKIMDIR/SKIM_GGHSM -i $INPUTDIR/signals/39_GluGluToHHTo2B2Tau_node_SM_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 0.03349 -a True -q shortcms --pu $PUDIR/PUsafe_weights.txt

#### SIGNALS TESI :
echo "Submitting - SIGNALS TESI - "
echo "Submitting - SIGNALS TESI - " >> log_28feb2019.txt
echo "OUTDIR = $OUTDIRR"
echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

1_GluGluToBulkGravitonToHHTo2B2Tau_M-250_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
2_GluGluToBulkGravitonToHHTo2B2Tau_M-260_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
3_GluGluToBulkGravitonToHHTo2B2Tau_M-270_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
4_GluGluToBulkGravitonToHHTo2B2Tau_M-280_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
5_GluGluToBulkGravitonToHHTo2B2Tau_M-350_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
6_GluGluToBulkGravitonToHHTo2B2Tau_M-400_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
7_GluGluToBulkGravitonToHHTo2B2Tau_M-450_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
8_GluGluToBulkGravitonToHHTo2B2Tau_M-500_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
9_GluGluToBulkGravitonToHHTo2B2Tau_M-550_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
10_GluGluToBulkGravitonToHHTo2B2Tau_M-600_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
11_GluGluToBulkGravitonToHHTo2B2Tau_M-650_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
12_GluGluToBulkGravitonToHHTo2B2Tau_M-750_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
13_GluGluToBulkGravitonToHHTo2B2Tau_M-800_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
14_GluGluToRadionToHHTo2B2Tau_M-250_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
15_GluGluToRadionToHHTo2B2Tau_M-260_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
16_GluGluToRadionToHHTo2B2Tau_M-270_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
17_GluGluToRadionToHHTo2B2Tau_M-280_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
18_GluGluToRadionToHHTo2B2Tau_M-300_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
19_GluGluToRadionToHHTo2B2Tau_M-320_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
20_GluGluToRadionToHHTo2B2Tau_M-350_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
21_GluGluToRadionToHHTo2B2Tau_M-400_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
22_GluGluToRadionToHHTo2B2Tau_M-450_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
23_GluGluToRadionToHHTo2B2Tau_M-500_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
24_GluGluToRadionToHHTo2B2Tau_M-550_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
25_GluGluToRadionToHHTo2B2Tau_M-600_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
26_GluGluToRadionToHHTo2B2Tau_M-650_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
27_GluGluToRadionToHHTo2B2Tau_M-700_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
28_GluGluToRadionToHHTo2B2Tau_M-750_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
29_GluGluToRadionToHHTo2B2Tau_M-800_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
30_GluGluToRadionToHHTo2B2Tau_M-850_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
31_GluGluToRadionToHHTo2B2Tau_M-900_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
32_GluGluToRadionToHHTo2B2Tau_M-1000_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
33_GluGluToRadionToHHTo2B2Tau_M-1250_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
34_GluGluToRadionToHHTo2B2Tau_M-1500_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
35_GluGluToRadionToHHTo2B2Tau_M-1750_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
36_GluGluToRadionToHHTo2B2Tau_M-2000_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
37_GluGluToRadionToHHTo2B2Tau_M-2500_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
38_GluGluToRadionToHHTo2B2Tau_M-3000_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
51_VBFToRadionToHHTo2B2Tau_M-250_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
52_VBFToRadionToHHTo2B2Tau_M-260_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
53_VBFToRadionToHHTo2B2Tau_M-270_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt
54_VBFToRadionToHHTo2B2Tau_M-280_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
55_VBFToRadionToHHTo2B2Tau_M-300_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt
56_VBFToRadionToHHTo2B2Tau_M-350_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt
57_VBFToRadionToHHTo2B2Tau_M-400_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt
58_VBFToRadionToHHTo2B2Tau_M-450_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
59_VBFToRadionToHHTo2B2Tau_M-500_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
60_VBFToRadionToHHTo2B2Tau_M-550_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
61_VBFToRadionToHHTo2B2Tau_M-600_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
62_VBFToRadionToHHTo2B2Tau_M-650_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
63_VBFToRadionToHHTo2B2Tau_M-700_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
64_VBFToRadionToHHTo2B2Tau_M-750_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt
65_VBFToRadionToHHTo2B2Tau_M-800_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
66_VBFToRadionToHHTo2B2Tau_M-850_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
67_VBFToRadionToHHTo2B2Tau_M-900_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
68_VBFToRadionToHHTo2B2Tau_M-1000_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
69_VBFToRadionToHHTo2B2Tau_M-1250_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
70_VBFToRadionToHHTo2B2Tau_M-1500_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
71_VBFToRadionToHHTo2B2Tau_M-1750_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
72_VBFToRadionToHHTo2B2Tau_M-2000_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
73_VBFToRadionToHHTo2B2Tau_M-3000_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
74_VBFToBulkGravitonToHHTo2B2Tau_M-250_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
75_VBFToBulkGravitonToHHTo2B2Tau_M-260_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
76_VBFToBulkGravitonToHHTo2B2Tau_M-270_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
77_VBFToBulkGravitonToHHTo2B2Tau_M-280_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
78_VBFToBulkGravitonToHHTo2B2Tau_M-300_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
79_VBFToBulkGravitonToHHTo2B2Tau_M-320_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
80_VBFToBulkGravitonToHHTo2B2Tau_M-350_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
81_VBFToBulkGravitonToHHTo2B2Tau_M-400_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
82_VBFToBulkGravitonToHHTo2B2Tau_M-450_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
83_VBFToBulkGravitonToHHTo2B2Tau_M-500_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
84_VBFToBulkGravitonToHHTo2B2Tau_M-600_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
85_VBFToBulkGravitonToHHTo2B2Tau_M-650_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
86_VBFToBulkGravitonToHHTo2B2Tau_M-700_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
87_VBFToBulkGravitonToHHTo2B2Tau_M-750_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
88_VBFToBulkGravitonToHHTo2B2Tau_M-850_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
89_VBFToBulkGravitonToHHTo2B2Tau_M-900_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
90_VBFToBulkGravitonToHHTo2B2Tau_M-1000_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
91_VBFToBulkGravitonToHHTo2B2Tau_M-1750_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt
92_VBFToBulkGravitonToHHTo2B2Tau_M-2000_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt

# GGH non-resonant
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/SKIM_nodeSM -i $INPUTDIR/signals/39_GluGluToHHTo2B2Tau_node_SM_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/SKIM_node2 -i $INPUTDIR/signals/40_GluGluToHHTo2B2Tau_node_2_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/SKIM_node3 -i $INPUTDIR/signals/41_GluGluToHHTo2B2Tau_node_3_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/SKIM_node4 -i $INPUTDIR/signals/42_GluGluToHHTo2B2Tau_node_4_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/SKIM_node7 -i $INPUTDIR/signals/43_GluGluToHHTo2B2Tau_node_7_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/SKIM_node9 -i $INPUTDIR/signals/44_GluGluToHHTo2B2Tau_node_9_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/SKIM_node12 -i $INPUTDIR/signals/45_GluGluToHHTo2B2Tau_node_12_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/PUsafe_weights.txt

# VBF non-resonant
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/SKIM_V_1_C2V_1_C3_1 -i $INPUTDIR/signals/46_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 1.0 -a True -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/SKIM_CV_1_C2V_1_C3_0 -i $INPUTDIR/signals/47_VBFHHTo2B2Tau_CV_1_C2V_1_C3_0_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/SKIM_CV_1_C2V_1_C3_2 -i $INPUTDIR/signals/48_VBFHHTo2B2Tau_CV_1_C2V_1_C3_2_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/SKIM_CV_1_C2V_2_C3_1 -i $INPUTDIR/signals/49_VBFHHTo2B2Tau_CV_1_C2V_2_C3_1_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/PUsafe_weights.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/SKIM_CV_1_5_C2V_1_C3_1 -i $INPUTDIR/signals/50_VBFHHTo2B2Tau_CV_1_5_C2V_1_C3_1_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 1.0 -a True -q shortcms --pu $PUDIR/PUsafe_weights.txt

# GGH Radion
python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_ggRadion250 -i $INPUTDIR/signals_tesi/20_GluGluToRadionToHHTo2B2Tau_M-250_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_ggRadion280 -i $INPUTDIR/signals_tesi/23_GluGluToRadionToHHTo2B2Tau_M-280_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_ggRadion350 -i $INPUTDIR/signals_tesi/24_GluGluToRadionToHHTo2B2Tau_M-350_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_ggRadion400 -i $INPUTDIR/signals_tesi/25_GluGluToRadionToHHTo2B2Tau_M-400_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_ggRadion450 -i $INPUTDIR/signals_tesi/26_GluGluToRadionToHHTo2B2Tau_M-450_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_ggRadion500 -i $INPUTDIR/signals_tesi/27_GluGluToRadionToHHTo2B2Tau_M-500_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_ggRadion600 -i $INPUTDIR/signals_tesi/29_GluGluToRadionToHHTo2B2Tau_M-600_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_ggRadion650 -i $INPUTDIR/signals_tesi/30_GluGluToRadionToHHTo2B2Tau_M-650_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_ggRadion750 -i $INPUTDIR/signals_tesi/31_GluGluToRadionToHHTo2B2Tau_M-750_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_ggRadion1000 -i $INPUTDIR/signals_tesi/34_GluGluToRadionToHHTo2B2Tau_M-1000_narrow_13TeV-madgraph_correctedcfg__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_vbfRadion250 -i $INPUTDIR/signals_tesi/53_VBFToRadionToHHTo2B2Tau_M-250_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_vbfRadion280 -i $INPUTDIR/signals_tesi/56_VBFToRadionToHHTo2B2Tau_M-280_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_vbfRadion350 -i $INPUTDIR/signals_tesi/57_VBFToRadionToHHTo2B2Tau_M-350_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_vbfRadion400 -i $INPUTDIR/signals_tesi/58_VBFToRadionToHHTo2B2Tau_M-400_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_vbfRadion450 -i $INPUTDIR/signals_tesi/59_VBFToRadionToHHTo2B2Tau_M-450_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_vbfRadion500 -i $INPUTDIR/signals_tesi/60_VBFToRadionToHHTo2B2Tau_M-500_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_vbfRadion600 -i $INPUTDIR/signals_tesi/61_VBFToRadionToHHTo2B2Tau_M-600_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_vbfRadion650 -i $INPUTDIR/signals_tesi/62_VBFToRadionToHHTo2B2Tau_M-650_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_vbfRadion750 -i $INPUTDIR/signals_tesi/63_VBFToRadionToHHTo2B2Tau_M-750_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

python scripts/skimNtuple_mib.py -T $OUTDIRR -s True -c config/skim_2017_sync_mib.cfg  -n 1   -k True -o $SKIMDIR/signals_TESI/SKIM_vbfRadion1000 -i $INPUTDIR/signals_tesi/66_VBFToRadionToHHTo2B2Tau_M-1000_narrow_13TeV-madgraph__RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1.txt -x 1.0 -a True -q shortcms --pu $PUDIR/VBFSM.txt

COMMENT2

<<COMMENTFINAL
################################################################################################################################################################
################################################################################################################################################################
################################################################################################################################################################

# # ####################
### DATA :
#echo "Submitting - DATA tau - "
#echo "Submitting - DATA tau - " >> log_28feb2019.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

#python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 10 -k False -o $SKIMDIR/SKIM_Tau_2017B -i $INPUTDIR/5_Tau__Run2017B-31Mar2018-v1.txt -q longcms

#python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 10 -k False -o $SKIMDIR/SKIM_Tau_2017F -i $INPUTDIR/6_Tau__Run2017F-31Mar2018-v1.txt -q longcms


#echo "Submitting - DATA Muon - "
#echo "Submitting - DATA Muon - " >> log_28feb2019.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

#python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 10 -k False -o $SKIMDIR/SKIM_SingleMuon_2017B -i $INPUTDIR/1_SingleMuon__Run2017B-31Mar2018-v1.txt -q longcms

#python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 10 -k False -o $SKIMDIR/SKIM_SingleMuon_2017F -i $INPUTDIR/2_SingleMuon__Run2017F-31Mar2018-v1.txt -q longcms


#echo "Submitting - DATA Electron - "
#echo "Submitting - DATA Electron - " >> log_28feb2019.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

#python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 10 -k False -o $SKIMDIR/SKIM_SingleElectron_2017B -i $INPUTDIR/3_SingleElectron__Run2017B-31Mar2018-v1.txt -q longcms

#python scripts/skimNtuple_mib.py -T $OUTDIRR -d True  -s True -c config/skim_2017_sync_mib.cfg -n 20 -k False -o $SKIMDIR/SKIM_SingleElectron_2017F -i $INPUTDIR/4_SingleElectron__Run2017F-31Mar2018-v1.txt -q longcms

###################################
# VBF signals
#echo "Submitting - VBF SIGNALS - "
#echo "Submitting - VBF SIGNALS - " >> log_28feb2019.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 7 -k True -o $SKIMDIR/$OUTDIRR/VBFHHTo2B2Tau_111 -i $INPUTDIR_VBF/1_VBFHHTo2B2Tau_CV_1_C2V_1_C3_1_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 1.0 -a True -q longcms

#python scripts/skimNtuple.py -T $OUTDIRR -s True -c config/skim_2017.cfg -n 7 -k True -o $SKIMDIR/$OUTDIRR/VBFHHTo2B2Tau_121 -i $INPUTDIR_VBF/2_VBFHHTo2B2Tau_CV_1_C2V_2_C3_1_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt -x 1.0 -a True  -q longcms

# ggH signals
#echo "Submitting - ggH SIGNALS - "
#echo "Submitting - ggH SIGNALS - " >> log_28feb2019.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

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
#echo "Submitting - diboson - " >> log_28feb2019.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

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
#echo "Submitting - DYEWK - " >> log_28feb2019.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

# # ## DY
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 120 -k True -o $SKIMDIR/$OUTDIRR/SKIM_DY   -i $INPUTDIR/DY_mergedList.txt -g True -x 5765.4

# # ## ELECTROWEAK
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_EWKWPlus2Jets_WToLNu   -i $INPUTDIR/EWKWPlus2Jets_WToLNu_M-50.txt   -x 25.62
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_EWKWMinus2Jets_WToLNu  -i $INPUTDIR/EWKWMinus2Jets_WToLNu_M-50.txt  -x 20.25
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 100   -k True -o $SKIMDIR/$OUTDIRR/SKIM_EWKZ2Jets_ZToLL        -i $INPUTDIR/EWKZ2Jets_ZToLL_M-50.txt        -x 3.987
#sleep 30m


#echo "Submitting - SingleTop - "
#echo "Submitting - SingleTop - " >> log_28feb2019.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

# ### tW top :
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_ST_tW_antitop_5f_inclusiveDecays   -i $INPUTDIR/33_ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt   -x 35.6
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 100 -k True -o $SKIMDIR/$OUTDIRR/SKIM_ST_tW_top_5f_inclusiveDecays       -i $INPUTDIR/34_ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt       -x 35.6
#sleep 15m


#echo "Submitting - WJets - "
#echo "Submitting - WJets - " >> log_28feb2019.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

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
#echo "Submitting - TThad - " >> log_28feb2019.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

# TT had
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 220 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_fullyHad  -i $INPUTDIR/TT_powheg_fullyHad.txt    -x 380.1143 -t True -b 1

#echo "Submitting - TTlep - "
#echo "Submitting - TTlep - " >> log_28feb2019.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

# TT lep
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 220 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_fullyLep  -i $INPUTDIR/TT_powheg_fullyLep.txt    -x 87.3348  -t True -b 4


#echo "Submitting - TTsemi - "
#echo "Submitting - TTsemi - " >> log_28feb2019.txt
#echo "OUTDIR = $OUTDIRR"
#echo "OUTDIR = $OUTDIRR" >> log_28feb2019.txt

# TT semi
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2017.cfg  -n 220 -k True -o $SKIMDIR/$OUTDIRR/SKIM_TT_semiLep   -i $INPUTDIR/TT_powheg_semiLep.txt     -x 364.3108 -t True -b 5

#echo "---- FINISHED ---- " >> log_28feb2019.txt
 
COMMENTFINAL



