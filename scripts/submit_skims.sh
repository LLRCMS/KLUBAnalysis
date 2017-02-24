OUTDIRR="Skims2017_20Feb_HHDynRewTest"
AMESSAGE="Testing HH dynamic reweighting. 11 Feb 2017 MC production (most but not all jobs are finished). Data from 25Nov production."

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


#####################
### DATA

# python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 60  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_SingleMuon        -i inputFiles/Files80X_25Nov2016/SingleMu_25Nov2016.txt
# python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 100 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_SingleElectron    -i inputFiles/Files80X_25Nov2016/SingleEle_25Nov2016.txt
# python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 60  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_Tau               -i inputFiles/Files80X_25Nov2016/Tau_25Nov2016.txt

#####################
### DATA PROMPT RECO FOR CONTROL
# python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 60  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_PromptReco_SingleMuon        -i inputFiles/Files80X_25Nov2016/SingleMu_25Nov2016BCDPromptReco.txt
# python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 100 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_PromptReco_SingleElectron    -i inputFiles/Files80X_25Nov2016/SingleEle_25Nov2016BCDPromptReco.txt
# python scripts/skimNtuple.py -T $OUTDIRR -d True  -s True -c  config/skim_2016.cfg  -n 60  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_PromptReco_Tau               -i inputFiles/Files80X_25Nov2016/Tau_25Nov2016BCDPromptReco.txt


####################
## TT
# # TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
# # hh = 45.7%
# # ll = 10.5%
# # hl = 21.9% (x2 for permutation t-tbar)
## technical note: use -q long if splitting on ~180-350 jobs
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 400 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_TT_fullyHad  -i inputFiles/Files_7Feb2016/TT_powheg_fullyHad_7Feb2017.txt    -x 380.1143 -t True -b 1
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 700 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_TT_semiLep   -i inputFiles/Files_7Feb2016/TT_powheg_semiLep_7Feb2017.txt     -x 364.3108 -t True -b 5
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 700 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_TT_fullyLep  -i inputFiles/Files_7Feb2016/TT_powheg_fullyLep_7Feb2017.txt    -x 87.3348  -t True -b 4

# # ####################
# # ## DY
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_DY   -i inputFiles/Files_7Feb2016/DY_mergedList_7Feb2017.txt -g True -x 5765.4 

# # ####################
# # ## ELECTROWEAK
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50   -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_EWKWPlus2Jets_WToLNu   -i inputFiles/Files_7Feb2016/EWKWPlus2Jets_WToLNu_M-50.txt   -x 25.62
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50   -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_EWKWMinus2Jets_WToLNu  -i inputFiles/Files_7Feb2016/EWKWMinus2Jets_WToLNu_M-50.txt  -x 20.25
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50   -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_EWKZ2Jets_ZToLL        -i inputFiles/Files_7Feb2016/EWKZ2Jets_ZToLL_M-50.txt        -x 3.987

# #############################
# ##### ### W+jets : 
# ##### ## note: currently I am chopping away the part with >= 1 jet in the inclusive sample, and writing the corresponding cross section.
# ##### ## But I can increase the stat with some stitching as done in DY if needed
# ##### python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_0jet     -i inputFiles/Files80X_25Nov2016/8_WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt    -x 44149.01333 -e 0
# ##### python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_1jet     -i inputFiles/Files80X_25Nov2016/9_W1JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt   -x 11778.36381
# ##### python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_2jet     -i inputFiles/Files80X_25Nov2016/10_W2JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt  -x 3840.219744
# ##### python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_3jet     -i inputFiles/Files80X_25Nov2016/11_W3JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt  -x 1166.047866
# ##### python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_4jet     -i inputFiles/Files80X_25Nov2016/12_W4JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8__RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1.txt  -x 593.055246

# #############################
# ##### ### W+jets in HT bins 
# ## WJets for HT bins -- chopping away HT > 70 in the inclusive sample is fine, as it is only ~10% of the total number of events
# ## tot xs NNLO: 61526.7 (+497.1 -264.6) (+/- 2312.7) pb
# ## individual xs are taken from : https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#W_jets
# ## and the one for 0-70 and 70-100 computed by difference and using the inclusive sample distribution
# ## xs LO : 50690 pb
# ## xs LO (HT < 100 ) = 50690 - sum (xs HT > 100) = 50690 - 1772.52216 = 48917.47784
# ## then using the inlcusive sample :
# ## ch->GetEntries("lheHt < 70")                : 186703 (97.09%) --> 47493.98 pb
# ## ch->GetEntries("lheHt > 70 && lheHt < 100") : 5589   (2.91%)  --> 1423.499 pb
# ## Finally, everything is scaled from the LO to the NNLO xs : (61526.7/50690.) = 1.213784
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_0_70       -i inputFiles/Files_7Feb2016/WJets_incl.txt        -y 1.213784 -x 47493.98 -z 70
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_70_100     -i inputFiles/Files_7Feb2016/WJetsHT-70-100.txt    -y 1.213784 -x 1423.499
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_100_200    -i inputFiles/Files_7Feb2016/WJetsHT-100-200.txt   -y 1.213784 -x 1345
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_200_400    -i inputFiles/Files_7Feb2016/WJetsHT-200-400.txt   -y 1.213784 -x 359.7
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_400_600    -i inputFiles/Files_7Feb2016/WJetsHT-400-600.txt   -y 1.213784 -x 48.91
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_600_800    -i inputFiles/Files_7Feb2016/WJetsHT-600-800.txt   -y 1.213784 -x 12.05
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_800_1200   -i inputFiles/Files_7Feb2016/WJetsHT-800-1200.txt  -y 1.213784 -x 5.501
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_1200_2500  -i inputFiles/Files_7Feb2016/WJetsHT-1200-2500.txt -y 1.213784 -x 1.329
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WJets_HT_2500_Inf   -i inputFiles/Files_7Feb2016/WJetsHT-2500-Inf.txt  -y 1.213784 -x 0.03216

# #####################
# ### tW top : 
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 30 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ST_tW_antitop_5f_inclusiveDecays   -i inputFiles/Files_7Feb2016/10_ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt   -x 35.6
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 30 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ST_tW_top_5f_inclusiveDecays       -i inputFiles/Files_7Feb2016/11_ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt       -x 35.6

# # #####################
# # ### DI-boson:
# # # note2: all njobs listed here were increased by x10 to make them run on the short queue
# # #XS taken from here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson
# ## for Z->4Q, I scaled from ZZ->2nu2Q using BR(2Nu2Q) = 2 * 20% * 69.91%
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50   -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WWTo2L2Nu  -i inputFiles/Files_7Feb2016/40_WWTo2L2Nu_13TeV-powheg__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt   -x 12.178
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50   -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WWToLNuQQ  -i inputFiles/Files_7Feb2016/WWToLNuQQ.txt                                                                                                  -x 49.997
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50   -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WWTo4Q     -i inputFiles/Files_7Feb2016/43_WWTo4Q_13TeV-powheg__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt      -x 51.723

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50   -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ZZTo2L2Nu  -i inputFiles/Files_7Feb2016/32_ZZTo2L2Nu_13TeV_powheg_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt               -x 0.564
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 100  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ZZTo2L2Q   -i inputFiles/Files_7Feb2016/33_ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt  -x 3.22
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 100  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ZZTo4L     -i inputFiles/Files_7Feb2016/34_ZZTo4L_13TeV-amcatnloFXFX-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt       -x 1.212
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50   -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ZZTo4Q     -i inputFiles/Files_7Feb2016/37_ZZTo4Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt    -x 7.06091

# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WZTo3LNu       -i inputFiles/Files_7Feb2016/31_WZTo3LNu_TuneCUETP8M1_13TeV-powheg-pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt      -x 4.42965 
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WZTo1L3Nu      -i inputFiles/Files_7Feb2016/47_WZTo1L3Nu_13TeV_amcatnloFXFX_madspin_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt    -x 3.033
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 200  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WZTo1L1Nu2Q    -i inputFiles/Files_7Feb2016/48_WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3.txt  -x 10.71
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 400  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_WZTo2L2Q       -i inputFiles/Files_7Feb2016/49_WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt     -x 5.595

# #####################
# ### SM Higgs
# ### HXSWG: xs(ZH) = 0.8839 pb
# ### Z->qq : 69.91% , Z->ll : 3,3658% (x3 for all the leptons)
# ### H->bb : 57.7%  , H->tautau : 6.32%
# ##### ZH (Zall, Htautau) : 6.32%
# ##### ZH (Zll, Hbb)      : 5.83%
# ##### ZH (Zqq, Hbb)      : 40.34%
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 100  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ZH_HTauTau      -i inputFiles/Files_7Feb2016/50_ZHToTauTau_M125_13TeV_powheg_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt      -x 0.8839 -y 0.0632
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 100  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ZH_HBB_ZLL      -i inputFiles/Files_7Feb2016/ZH_HToBB_ZToLL.txt                                                                                                              -x 0.8839 -y 0.0583
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 100  -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_ZH_HBB_ZQQ      -i inputFiles/Files_7Feb2016/53_ZH_HToBB_ZToQQ_M125_13TeV_powheg_pythia8__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt  -x 0.8839 -y 0.4034


# # #####################
# # ### Radion signals
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-250   -i inputFiles/Files_7Feb2016/allRadion/58_GluGluToRadionToHHTo2B2Tau_M-250_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-270   -i inputFiles/Files_7Feb2016/allRadion/59_GluGluToRadionToHHTo2B2Tau_M-270_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-280   -i inputFiles/Files_7Feb2016/allRadion/60_GluGluToRadionToHHTo2B2Tau_M-280_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-300   -i inputFiles/Files_7Feb2016/allRadion/61_GluGluToRadionToHHTo2B2Tau_M-300_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1.txt  -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-350   -i inputFiles/Files_7Feb2016/allRadion/62_GluGluToRadionToHHTo2B2Tau_M-350_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 50 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-400   -i inputFiles/Files_7Feb2016/allRadion/63_GluGluToRadionToHHTo2B2Tau_M-400_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3.txt  -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-450   -i inputFiles/Files_7Feb2016/allRadion/64_GluGluToRadionToHHTo2B2Tau_M-450_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-500   -i inputFiles/Files_7Feb2016/allRadion/65_GluGluToRadionToHHTo2B2Tau_M-500_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-550   -i inputFiles/Files_7Feb2016/allRadion/66_GluGluToRadionToHHTo2B2Tau_M-550_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-600   -i inputFiles/Files_7Feb2016/allRadion/67_GluGluToRadionToHHTo2B2Tau_M-600_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-650   -i inputFiles/Files_7Feb2016/allRadion/68_GluGluToRadionToHHTo2B2Tau_M-650_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-750   -i inputFiles/Files_7Feb2016/allRadion/69_GluGluToRadionToHHTo2B2Tau_M-750_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_GluGluToRadionToHHTo2B2Tau_M-900   -i inputFiles/Files_7Feb2016/allRadion/70_GluGluToRadionToHHTo2B2Tau_M-900_narrow_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt       -x 1.0 -a True




python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 10 -k True -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_node_SM   -i inputFiles/Files_7Feb2016/38_GluGluToHHTo2B2Tau_node_SM_13TeV-madgraph__RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1.txt  -x 1.0 -a True

python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_dyn_21   -i inputFiles/Files_7Feb2016/allNonResClones_dynRew/filelist_allNonRes_dr_21.txt  -x 1.0 --kl 1.0  --kt 1.0 -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_dyn_41   -i inputFiles/Files_7Feb2016/allNonResClones_dynRew/filelist_allNonRes_dr_41.txt  -x 1.0 --kl 20.0 --kt 1.0 -a True



# ## HH with reweighting --  a lot of files!
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_0   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_0.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_0.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_1   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_1.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_1.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_2   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_2.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_2.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_3   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_3.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_3.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_4   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_4.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_4.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_5   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_5.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_5.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_6   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_6.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_6.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_7   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_7.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_7.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_8   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_8.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_8.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_9   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_9.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_9.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_10   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_10.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_10.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_11   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_11.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_11.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_12   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_12.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_12.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_13   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_13.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_13.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_14   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_14.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_14.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_15   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_15.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_15.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_16   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_16.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_16.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_17   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_17.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_17.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_18   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_18.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_18.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_19   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_19.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_19.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_20   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_20.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_20.root -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_21   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_21.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_21.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_22   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_22.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_22.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_23   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_23.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_23.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_24   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_24.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_24.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_25   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_25.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_25.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_26   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_26.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_26.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_27   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_27.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_27.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_28   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_28.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_28.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_29   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_29.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_29.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_30   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_30.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_30.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_31   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_31.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_31.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_32   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_32.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_32.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_33   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_33.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_33.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_34   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_34.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_34.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_35   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_35.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_35.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_36   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_36.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_36.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_37   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_37.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_37.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_38   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_38.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_38.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_39   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_39.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_39.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_40   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_40.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_40.root -a True
python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_41   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_41.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_41.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_42   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_42.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_42.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_43   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_43.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_43.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_44   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_44.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_44.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_45   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_45.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_45.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_46   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_46.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_46.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_47   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_47.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_47.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_48   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_48.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_48.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_49   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_49.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_49.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_50   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_50.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_50.root -a True
# python scripts/skimNtuple.py -T $OUTDIRR -s True -c  config/skim_2016.cfg  -n 20 -k False -o /data_CMS/cms/cadamuro/test_submit_to_tier3/$OUTDIRR/SKIM_HH_lambdarew_51   -i inputFiles/Files_7Feb2016/allNonResClones/filelist_allNonRes_51.txt  -x 1.0 -w /home/llr/cms/cadamuro/HH2016/CMSSW_7_4_7/src/KLUBAnalysis/hhReweight/lambdaMap_51.root -a True
