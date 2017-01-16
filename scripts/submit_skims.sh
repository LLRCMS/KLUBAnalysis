OUTDIRR="Skims2017_10Gen"
AMESSAGE="Res BDT weight file updated. Fixes an error in BDT LepTauKine, now output is correct. A 0.1 percent of data is missing. NoL1TJson . MC is not complete (many jobs in idle/running). Production as of 25 Nov 2016 status."

source /opt/exp_soft/cms/t3/t3setup
mkdir /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/
mkdir $OUTDIRR
touch /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/README.txt
echo $AMESSAGE > /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/README.txt
cp /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/scripts/haddAll.sh /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/


# source /opt/exp_soft/cms/t3/t3setup
# mkdir /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/
# mkdir $OUTDIRR
# touch /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/README.txt
# echo $AMESSAGE > /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/README.txt
# cp /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/scripts/haddAll.sh /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/


# #####################
# ### DATA

python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 60  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_SingleMuon        -i inputFiles/Files80X_25Nov2016/SingleMu_25Nov2016.txt
python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 100 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_SingleElectron    -i inputFiles/Files80X_25Nov2016/SingleEle_25Nov2016.txt
python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 60  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_Tau               -i inputFiles/Files80X_25Nov2016/Tau_25Nov2016.txt

# #####################
# ### DATA PROMPT RECO FOR CONTROL
# # python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 60  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_PromptReco_SingleMuon        -i inputFiles/Files80X_25Nov2016/SingleMu_25Nov2016BCDPromptReco.txt
# # python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 100 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_PromptReco_SingleElectron    -i inputFiles/Files80X_25Nov2016/SingleEle_25Nov2016BCDPromptReco.txt
# # python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 60  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_PromptReco_Tau               -i inputFiles/Files80X_25Nov2016/Tau_25Nov2016BCDPromptReco.txt


#####################
### TT
## # TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
## # hh = 45.7%
## # ll = 10.5%
## # hl = 21.9% (x2 for permutation t-tbar)
### technical note: use -q long if splitting on ~180-350 jobs
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 400 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_TT_fullyHad  -i inputFiles/Files80X_25Nov2016/TT_powheg_fullyHad_25Nov2016.txt    -x 380.1143 -t True -b 1
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 700 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_TT_semiLep   -i inputFiles/Files80X_25Nov2016/TT_powheg_semiLep_25Nov2016.txt     -x 364.3108 -t True -b 5
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 700 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_TT_fullyLep  -i inputFiles/Files80X_25Nov2016/TT_powheg_fullyLep_25Nov2016.txt    -x 87.3348  -t True -b 4

#####################
### DY
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_DY   -i inputFiles/Files80X_25Nov2016/DY_mergedList_25Nov2016.txt -g True -x 5765.4 

#####################
### W+jets : 
## note: currently I am chopping away the part with >= 1 jet in the inclusive sample, and writing the corresponding cross section.
## But I can increase the stat with some stitching as done in DY if needed
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_0jet     -i inputFiles/Files80X_25Nov2016/8_WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt    -x 44149.01333 -e 0
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_1jet     -i inputFiles/Files80X_25Nov2016/9_W1JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt   -x 11778.36381
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_2jet     -i inputFiles/Files80X_25Nov2016/10_W2JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt  -x 3840.219744
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_3jet     -i inputFiles/Files80X_25Nov2016/11_W3JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt  -x 1166.047866
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_4jet     -i inputFiles/Files80X_25Nov2016/12_W4JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt  -x 593.055246

#####################
### tW top : 
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ST_tW_antitop_5f_inclusiveDecays   -i inputFiles/Files80X_25Nov2016/5_ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt   -x 35.6
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ST_tW_top_5f_inclusiveDecays       -i inputFiles/Files80X_25Nov2016/6_ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v2.txt       -x 35.6

#####################
### DI-boson:
# NOTE!!! There are more samples processed to be used together here 
# note2: all njobs listed here were increased by x10 to make them run on the short queue
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 100  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ZZTo2L2Q       -i inputFiles/Files80X_25Nov2016/7_ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt     -x 3.22
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50   -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WWToLNuQQ      -i inputFiles/Files80X_25Nov2016/14_WWToLNuQQ_13TeV-powheg__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt                         -x 49.997
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 250  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WZTo1L3Nu      -i inputFiles/Files80X_25Nov2016/19_WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt   -x 3.05
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 250  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WZTo1L1Nu2Q    -i inputFiles/Files80X_25Nov2016/17_WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt -x 10.71
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 500  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WZTo2L2Q       -i inputFiles/Files80X_25Nov2016/18_WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt    -x 5.595


#####################
### Radion signals
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-250   -i inputFiles/Files80X_25Nov2016/radion_250.txt  -x 1.0 -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-260   -i inputFiles/Files80X_25Nov2016/radion_260.txt  -x 1.0 -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-270   -i inputFiles/Files80X_25Nov2016/radion_270.txt  -x 1.0 -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-280   -i inputFiles/Files80X_25Nov2016/radion_280.txt  -x 1.0 -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-300   -i inputFiles/Files80X_25Nov2016/radion_300.txt  -x 1.0 -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-320   -i inputFiles/Files80X_25Nov2016/radion_320.txt  -x 1.0 -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-340   -i inputFiles/Files80X_25Nov2016/radion_340.txt  -x 1.0 -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-350   -i inputFiles/Files80X_25Nov2016/radion_350.txt  -x 1.0 -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-400   -i inputFiles/Files80X_25Nov2016/radion_400.txt  -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-450   -i inputFiles/Files80X_25Nov2016/radion_450.txt  -x 1.0 -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-500   -i inputFiles/Files80X_25Nov2016/radion_500.txt  -x 1.0 -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-550   -i inputFiles/Files80X_25Nov2016/radion_550.txt  -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-600   -i inputFiles/Files80X_25Nov2016/radion_600.txt  -x 1.0 -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-650   -i inputFiles/Files80X_25Nov2016/radion_650.txt  -x 1.0 -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-700   -i inputFiles/Files80X_25Nov2016/radion_700.txt  -x 1.0 -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-750   -i inputFiles/Files80X_25Nov2016/radion_750.txt  -x 1.0 -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-800   -i inputFiles/Files80X_25Nov2016/radion_800.txt  -x 1.0 -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-900   -i inputFiles/Files80X_25Nov2016/radion_900.txt  -x 1.0 -a True

python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_node_SM   -i inputFiles/Files80X_25Nov2016/33_GluGluToHHTo2B2Tau_node_SM_13TeV-madgraph__RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v3.txt  -x 1.0 -a True
