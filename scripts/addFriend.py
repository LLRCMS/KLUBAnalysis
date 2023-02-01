import ROOT
import os
import sys
#import numpy as n
from array import array
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_5Gen/SKIM_DY'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_9Gen/SKIM_DY'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_10Gen/SKIM_DY/'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_27Gen/SKIM_DY/'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_1Feb/SKIM_DY/'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_9Feb/SKIM_DY/'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_11Feb/SKIM_DY/'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_26Feb/SKIM_DY/'
# inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_28Feb/SKIM_DY/'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_13Giu_All/SKIM_DY/'
#inputDir = '/data_CMS/cms/cadamuro/test_submit_to_tier3/Skims2017_11Lug_textdxyz/SKIM_DY/'

#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_0j0b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_1j0b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_1j1b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_2j0b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_2j1b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_2j2b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_3j0b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_3j1b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_3j2b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_3j3b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_4j0b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_4j1b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_4j2b'
#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_DYJets_newXS/SKIM_DYJets_4j3b'
#inputDirs = ['/data_CMS/cms/amendola/HH2017Skims/SKIMS_3Oct2018_Run2017/SKIM_DY_NLO']
#inputDirs = ['/data_CMS/cms/amendola/HH2017Skims/SKIMS_3Oct2018_Run2017/SKIM_DY_NLO']
#inputDirs = ['/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_TT_fullyHad',
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_TT_fullyLep',
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_TT_semiLep']
#inputDirs = ['/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WJets_HT_0_100',	 
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WJets_HT_100_200',	 
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WJets_HT_200_400',	 
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WJets_HT_400_600',	 
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WJets_HT_600_800',	 
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WJets_HT_800_1200',	 
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WJets_HT_1200_2500', 
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WJets_HT_2500_Inf']  
#inputDirs = ['/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_ST_tW_top',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_ST_tW_antitop',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_ST_tchannel_top',	
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_ST_tchannel_antitop',	
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_EWKWMinus2Jets_WToLNu',	
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_EWKWPlus2Jets_WToLNu',	
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_EWKZ2Jets_ZToLL',	
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_EWKZ2Jets_ZToNuNu',	
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WWTo2L2Nu',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WWTo4Q',			
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WWToLNuQQ',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WZTo1L1Nu2Q',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WZTo2L2Q',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WZTo3LNu_0Jets',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WZTo3LNu_1Jets',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WZTo3LNu_2Jets',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WZTo3LNu_3Jets']		
#inputDirs = ['/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_ZZTo2L2Q',	
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_ZZTo4L',			
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_ZH_HBB_ZLL',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_ZH_HBB_ZQQ',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_ZH_HTauTau',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_ggHTauTau',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_VBFHTauTau',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WplusHTauTau',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WminusHTauTau',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_ttHJetTononBB',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_ttHJetToBB',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_ZZZ',			
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WWW',			
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WWZ',			
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_WZZ',			
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_TTWJetsToLNu',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_TTWJetsToQQ',		
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_TTWW',			
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_TTWZ',			
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_TTZZ']                  



#inputDirs = ['/data_CMS/cms/amendola/HH2017Skims/SKIMS_31Jul2018_Run2017/SKIM_Tau2017B',
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_31Jul2018_Run2017/SKIM_Tau2017C',
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_31Jul2018_Run2017/SKIM_Tau2017D',
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_31Jul2018_Run2017/SKIM_Tau2017E',
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_31Jul2018_Run2017/SKIM_Tau2017F',
#             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_VBFSM']

inputDirs = ['/data_CMS/cms/amendola/HH2017Skims/SKIMS_31Jul2018_Run2017/SKIM_SingleElectron2017B',
             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_31Jul2018_Run2017/SKIM_SingleElectron2017C',
             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_31Jul2018_Run2017/SKIM_SingleElectron2017D',
             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_31Jul2018_Run2017/SKIM_SingleElectron2017E',
             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_31Jul2018_Run2017/SKIM_SingleElectron2017F',
             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_31Jul2018_Run2017/SKIM_SingleMuon2017B',
             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_31Jul2018_Run2017/SKIM_SingleMuon2017C',
             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_31Jul2018_Run2017/SKIM_SingleMuon2017D',
             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_31Jul2018_Run2017/SKIM_SingleMuon2017E',
             '/data_CMS/cms/amendola/HH2017Skims/SKIMS_31Jul2018_Run2017/SKIM_SingleMuon2017F']

#inputDir = '/data_CMS/cms/amendola/HH2017Skims/SKIMS_28Aug2018_Run2017/SKIM_TT_semiLep'


############################################################################################################
## NOTE: this script takes about 11 mins for a full DY sample (inclusive + NJet + NBJet)
##
for inputDir in inputDirs:
	print "... processing directory", inputDir
	
	if inputDir[-1] == '/':
	    inputDir = inputDir[:-1] 
	
	inputList = inputDir + '/' + 'goodfiles.txt'
	if not os.path.isfile(inputList):
	    print "Could not find file:", inputList
	    sys.exit()
	
	suffix = '_trigSF95'
	    
	outDir = inputDir+suffix
	
	
	print "... saving outputs in: "
	print outDir
	
	
	
	os.system('mkdir '+outDir)
	
	inputListFile = open(inputList)
	outGoodFiles=open(outDir+'/goodfiles.txt', 'w')


	for idx, line in enumerate(inputListFile):
	    line = line.strip()

	    fileName = line.replace(inputDir+'/', '')

	    print 'file num' , idx
	    # if reading a goodfile, copy it to destination directory
	    outGoodFiles.write(line.replace(inputDir, outDir)+'\n')
	
	    fIn = ROOT.TFile(line)
	    tIn = fIn.Get('HTauTauTree')
	    h_eff = fIn.Get('h_eff')
	
	
	
	
	
	
	    fOut = ROOT.TFile(outDir+"/"+fileName,"recreate");
	    tOut = tIn.CloneTree(0);
	    #IdAndIsoSF_new = n.zeros( 1, dtype=float )
	    new = array("f",[0])
	    tOut.Branch('IdAndIsoSF_new', new, 'IdAndIsoSF_new/F')
	    #tOut.SetBranchStatus("*",1); 
	    for evt in tIn:
                   
	        new[0] = evt.IdAndIsoSF

                if evt.pairType == 0: #MuTau
	            if evt.isTau2real:
	                new[0] = evt.IdAndIsoSF*0.95/0.89
	        if evt.pairType == 1: #ETau
	            if evt.isTau2real:
	                new[0] = evt.IdAndIsoSF*0.95/0.89
	        if evt.pairType == 2: #TauTau
	            if evt.nRealTaus == 1:
	                new[0] = 0.95
	            if evt.nRealTaus==2:
	                new[0] = 0.95*0.95                
                tOut.Fill()

	    tOut.AutoSave()
	    tOut.Write()
	    h_eff.Write()
	    fOut.Close()
	    
	    fIn.Close()
	    
	outGoodFiles.close()
	print '... finished '+ inputDir

        
