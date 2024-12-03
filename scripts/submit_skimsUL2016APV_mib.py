import os
import sys
import glob


#--------------------------
date           = '29Nov2024_bkgCor'
execute_bkg    = False
execute_sig    = True
execute_data   = False
#--------------------------


klubDir        = '/gwpool/users/spalluotto/HH_bbtautau/CMSSW_11_1_9/src/KLUBAnalysis'
configFile     = '%s/config/skim_UL16APV.cfg'%klubDir
year           = '2016preVFP'


# LOG ---  
tag            = 'SKIMS_UL2016APV_%s'%date
outDir         = '%s/%s'%(klubDir, tag)
logFile        = '%s/log_%s.txt'%(outDir,date)


# INPUT ---
inputDir_bkg    = '%s/inputFiles/Resonant_UL2016APV_Backgrounds'%klubDir
inputDir_sig    = '%s/inputFiles/Resonant_UL2016APV_Signals'%klubDir
inputDir_data   = '%s/inputFiles/Resonant_UL2016APV_Data'%klubDir

# OUTPUT ---  
skimDir         = '/gwdata/users/spalluotto/ResonantHHbbtautauAnalysis/%s'%tag

# PU WEIGHTS --- 
puDir           = '%s/weights/PUreweight/UL_Run2_PU_SF/2016APV/PU_UL2016APV_SF.txt'%klubDir

# BASE COMMAND ---  
# -T: folder tag name    -s: sleep in submission      -c: skim config file    --period: year     -A: isAPV    -k: run HH kin fitter    -q: batch queue

baseCommand     = 'python %s/scripts/skimNtuple_mib.py    -T %s  -s True  -c %s   --period %s -k True  -q longcms'%(klubDir, tag, configFile, year)



# -- official names ---
names = {
    # Data
    "SingleElectron_Run2016A" : "EGammaA",
    "SingleElectron_Run2016B" : "EGammaB",
    "SingleElectron_Run2016C" : "EGammaC",
    "SingleElectron_Run2016D" : "EGammaD",
    "SingleElectron_Run2016E" : "EGammaE",
    "SingleElectron_Run2016F" : "EGammaF",
    "Tau_Run2016A" : "TauA",
    "Tau_Run2016B" : "TauB",
    "Tau_Run2016C" : "TauC",
    "Tau_Run2016D" : "TauD",
    "Tau_Run2016E" : "TauE",
    "Tau_Run2016F" : "TauF",
    "SingleMuon_Run2016A" : "MuonA",
    "SingleMuon_Run2016B" : "MuonB",
    "SingleMuon_Run2016C" : "MuonC",
    "SingleMuon_Run2016D" : "MuonD",
    "SingleMuon_Run2016E" : "MuonE",
    "SingleMuon_Run2016F" : "MuonF",
    "MET_Run2016A" : "META",
    "MET_Run2016B" : "METB",
    "MET_Run2016C" : "METC",
    "MET_Run2016D" : "METD",
    "MET_Run2016E" : "METE",
    "MET_Run2016F" : "METF",

    # Signal
    "GluGluToRadionToHHTo2B2Tau_M-250" : "Rad250",
    "GluGluToRadionToHHTo2B2Tau_M-260" : "Rad260",
    "GluGluToRadionToHHTo2B2Tau_M-270" : "Rad270",
    "GluGluToRadionToHHTo2B2Tau_M-280" : "Rad280",
    "GluGluToRadionToHHTo2B2Tau_M-300" : "Rad300",
    "GluGluToRadionToHHTo2B2Tau_M-320" : "Rad320",
    "GluGluToRadionToHHTo2B2Tau_M-350" : "Rad350",
    "GluGluToRadionToHHTo2B2Tau_M-400" : "Rad400",
    "GluGluToRadionToHHTo2B2Tau_M-450" : "Rad450",
    "GluGluToRadionToHHTo2B2Tau_M-500" : "Rad500",
    "GluGluToRadionToHHTo2B2Tau_M-550" : "Rad550",
    "GluGluToRadionToHHTo2B2Tau_M-600" : "Rad600",
    "GluGluToRadionToHHTo2B2Tau_M-650" : "Rad650",
    "GluGluToRadionToHHTo2B2Tau_M-700" : "Rad700",
    "GluGluToRadionToHHTo2B2Tau_M-750" : "Rad750",
    "GluGluToRadionToHHTo2B2Tau_M-800" : "Rad800",
    "GluGluToRadionToHHTo2B2Tau_M-850" : "Rad850",
    "GluGluToRadionToHHTo2B2Tau_M-900" : "Rad900",
    "GluGluToRadionToHHTo2B2Tau_M-1000" : "Rad1000",
    "GluGluToRadionToHHTo2B2Tau_M-1250" : "Rad1250",
    "GluGluToRadionToHHTo2B2Tau_M-1500" : "Rad1500",
    "GluGluToRadionToHHTo2B2Tau_M-1750" : "Rad1750",
    "GluGluToRadionToHHTo2B2Tau_M-2000" : "Rad2000",
    "GluGluToRadionToHHTo2B2Tau_M-2500" : "Rad2500",
    "GluGluToRadionToHHTo2B2Tau_M-3000" : "Rad3000",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-250" : "Grav250",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-260" : "Grav260",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-270" : "Grav270",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-280" : "Grav280",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-300" : "Grav300",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-320" : "Grav320",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-350" : "Grav350",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-400" : "Grav400",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-450" : "Grav450",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-500" : "Grav500",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-550" : "Grav550",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-600" : "Grav600",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-650" : "Grav650",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-700" : "Grav700",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-750" : "Grav750",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-800" : "Grav800",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-850" : "Grav850",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-900" : "Grav900",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-1000" : "Grav1000",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-1250" : "Grav1250",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-1500" : "Grav1500",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-1750" : "Grav1750",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-2000" : "Grav2000",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-2500" : "Grav2500",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-3000" : "Grav3000",

    # Background
    "TTToHadronic"     : "TT_Hadronic",
    "TTTo2L2Nu"        : "TT_FullyLep",
    "TTToSemiLeptonic" : "TT_SemiLep",

    "DYJetsToLL_M-50"                               : "DY_Incl",
    "DYJetsToLL_LHEFilterPtZ-0To50_MatchEWPDG20"    : "DY_PtZ0To50",
    "DYJetsToLL_LHEFilterPtZ-50To100_MatchEWPDG20"  : "DY_PtZ50To100",
    "DYJetsToLL_LHEFilterPtZ-100To250_MatchEWPDG20" : "DY_PtZ100To250",
    "DYJetsToLL_LHEFilterPtZ-250To400_MatchEWPDG20" : "DY_PtZ250To400",
    "DYJetsToLL_LHEFilterPtZ-400To650_MatchEWPDG20" : "DY_PtZ400To650",
    "DYJetsToLL_LHEFilterPtZ-650ToInf_MatchEWPDG20" : "DY_PtZ650ToInf",
    "DYJetsToLL_0J" : "DY_0J",
    "DYJetsToLL_1J" : "DY_1J",
    "DYJetsToLL_2J" : "DY_2J",

    "WJetsToLNu"              : "WJets_HT0To70", # for 0 < HT < 70
    "WJetsToLNu_HT-70To100"   : "WJets_HT70To100",
    "WJetsToLNu_HT-100To200"  : "WJets_HT100To200",
    "WJetsToLNu_HT-200To400"  : "WJets_HT200To400",
    "WJetsToLNu_HT-400To600"  : "WJets_HT400To600",
    "WJetsToLNu_HT-600To800"  : "WJets_HT600To800",
    "WJetsToLNu_HT-800To1200" : "WJets_HT800To1200",
    "WJetsToLNu_HT-1200To2500": "WJets_HT1200To2500",
    "WJetsToLNu_HT-2500ToInf" : "WJets_HT2500ToInf",

    "EWKWPlus2Jets_WToLNu_M-50"   : "EWKWPlus2Jets_WToLNu",
    "EWKWMinus2Jets_WToLNu_M-50"  : "EWKWMinus2Jets_WToLNu",
    "EWKZ2Jets_ZToLL_M-50"        : "EWKZ2Jets_ZToLL",

    "ST_tW_antitop_5f_inclusiveDecays"         : "ST_tW_antitop",
    "ST_tW_top_5f_inclusiveDecays"             : "ST_tW_top",
    "ST_t-channel_antitop_5f_InclusiveDecays"  : "ST_t-channel_antitop",
    "ST_t-channel_top_5f_InclusiveDecays"      : "ST_t-channel_top",

    "GluGluHToTauTau_M125"  : "GluGluHToTauTau",
    "VBFHToTauTau_M125"     : "VBFHToTauTau",
    "WplusHToTauTau_M125"   : "WplusHToTauTau",
    "WminusHToTauTau_M125"  : "WminusHToTauTau",
    "ZHToTauTau_M125"       : "ZHToTauTau",

    "ZH_HToBB_ZToLL_M-125" : "ZH_HToBB_ZToLL",
    "ZH_HToBB_ZToQQ_M-125" : "ZH_HToBB_ZToQQ",

    "ttHToNonbb_M125"  : "ttHToNonbb",
    "ttHTobb_M125"     : "ttHTobb",
    "ttHToTauTau_M125" : "ttHToTauTau",
    
    "WW" : "WW",
    "WZ" : "WZ",
    "ZZ" : "ZZ",

    "WWW_4F" : "WWW",
    "WWZ_4F" : "WWZ",
    "WZZ"    : "WZZ",
    "ZZZ"    : "ZZZ",

    "TTWJetsToLNu"     : "TTWJetsToLNu",
    "TTWJetsToQQ"      : "TTWJetsToQQ",
    "TTZToLLNuNu_M-10"  : "TTZToLLNuNu",
    "TTZToQQ"          : "TTZToQQ",
    "TTWW" : "TTWW",
    "TTZZ" : "TTZZ", 
    "TTWZ" : "TTWZ",

    "TTWH" : "TTWH",
    "TTZH" : "TTZH",

    "GluGluToHHTo2B2Tau_TuneCP5_PSWeights_node_SM" : "GluGluToHHTo2B2Tau"

}




#------------------------------------------------------------------------------------------
# ############
# BACKGROUNDS MAP 
# -----> -n: nJobs   -x: crossSection   -y: xs stitching scale   -z: HT cut for stitching

bkg_map = {}
bkg_map = {
    # --- DY ---
    # ---------->   -g:  loop on genjets to determine the number of b hadrons   --DY: if it is a DY sample
    "DYJetsToLL_LHEFilterPtZ-0To50_MatchEWPDG20"      : "-n 200 -x 1409.22      -g True  --DY True   --isDYlike",
    "DYJetsToLL_LHEFilterPtZ-50To100_MatchEWPDG20"    : "-n 200 -x 377.12       -g True  --DY True   --isDYlike",
    "DYJetsToLL_LHEFilterPtZ-100To250_MatchEWPDG20"   : "-n 150 -x 92.24        -g True  --DY True   --isDYlike",
    "DYJetsToLL_LHEFilterPtZ-250To400_MatchEWPDG20"   : "-n 50  -x 3.512        -g True  --DY True   --isDYlike",
    "DYJetsToLL_LHEFilterPtZ-400To650_MatchEWPDG20"   : "-n 20  -x 0.4826       -g True  --DY True   --isDYlike",
    "DYJetsToLL_LHEFilterPtZ-650ToInf_MatchEWPDG20"   : "-n 10  -x 0.04487      -g True  --DY True   --isDYlike",

    "DYJetsToLL_0J"                                   : "-n 300 -x 4867.28      -g True  --DY True   --isDYlike",
    "DYJetsToLL_1J"                                   : "-n 300 -x 902.95       -g True  --DY True   --isDYlike",  
    "DYJetsToLL_2J"                                   : "-n 150 -x 342.96       -g True  --DY True   --isDYlike",

    "DYJetsToLL_M-50"                                 : "-n 300 -x 6077.22      -g True  --DY True   --isDYlike",

    # --- TT ---
    # -b: type of TT gen level decay pruning for stitch
    "TTToHadronic"                    : "-n 50  -x 359.44   -t True    -b 1  --isTTlike",
    "TTTo2L2Nu"                       : "-n 100 -x 84.01    -t True    -b 4  --isTTlike",
    "TTToSemiLeptonic"                : "-n 100 -x 347.55   -t True    -b 5  --isTTlike",

    
    # --- single Higgs ---
    # -y: scale to apply on XS for stitching
    "ZHToTauTau_M125"                     : "-n 50    -x 0.880    -y 0.0632 --isDYlike",
    "WplusHToTauTau_M125"                 : "-n 50    -x 0.831    -y 0.0632 --isDYlike",
    "WminusHToTauTau_M125"                : "-n 50    -x 0.527    -y 0.0632 --isDYlike",
    "GluGluHToTauTau_M125"                : "-n 50    -x 48.61    -y 0.0632 --isDYlike",
    "VBFHToTauTau_M125"                   : "-n 50    -x 3.766    -y 0.0632 --isDYlike",

    "ttHToNonbb_M125"                 : "-n 50  -x 0.5071   -y 0.423    --ttHToNonBB True  --isTTlike",
    "ttHTobb_M125"                    : "-n 50  -x 0.5071   -y 0.577   --isTTlike", # but this will be most likely signal like, still fine to put hyp here
    "ttHToTauTau_M125"                : "-n 50  -x 0.5071   -y 0.0632  --isTTlike",

    "ZH_HToBB_ZToLL_M-125"             : "-n 50    -x 0.880    -y 0.058816576 --isDYlike",
    "ZH_HToBB_ZToQQ_M-125"             : "-n 50    -x 0.880    -y 0.407161664 --isDYlike",


    # --- Wjets ---
    # -z: HT cut for stitching on inclusive      -Z: HT low cut for stitching on inclusive
    "WJetsToLNu"                      : "-n 50  -y 1.221252481 -x 47253.172 -z 70   --isDYlike ",  # for 0 < HT < 70
    "WJetsToLNu_HT-70To100"           : "-n 50  -y 1.221252481 -x 1353    --isDYlike" ,
    "WJetsToLNu_HT-100To200"          : "-n 50  -y 1.221252481 -x 1346    --isDYlike" ,
    "WJetsToLNu_HT-200To400"          : "-n 50  -y 1.221252481 -x 360.1   --isDYlike",
    "WJetsToLNu_HT-400To600"          : "-n 50  -y 1.221252481 -x 48.8    --isDYlike",
    "WJetsToLNu_HT-600To800"          : "-n 50  -y 1.221252481 -x 12.07   --isDYlike",
    "WJetsToLNu_HT-800To1200"         : "-n 50  -y 1.221252481 -x 5.497   --isDYlike",
    "WJetsToLNu_HT-1200To2500"        : "-n 50  -y 1.221252481 -x 1.329   --isDYlike",
    "WJetsToLNu_HT-2500ToInf"         : "-n 50  -y 1.221252481 -x 0.0321 -Z 2500   --isDYlike",

    # --- ELECTROWEAK ---
    "EWKWPlus2Jets_WToLNu_M-50"       : "-n 50  -x 25.62   --isDYlike",
    "EWKWMinus2Jets_WToLNu_M-50"      : "-n 50  -x 20.25   --isDYlike",
    "EWKZ2Jets_ZToLL_M-50"            : "-n 50  -x 3.987   --isDYlike",

    # --- Single Top ---
    "ST_tW_antitop_5f_inclusiveDecays"        : "-n 50  -x 35.85  --isTTlike",
    "ST_tW_top_5f_inclusiveDecays"            : "-n 50  -x 35.85  --isTTlike",
    "ST_t-channel_antitop_5f_InclusiveDecays" : "-n 50  -x 80.95  --isTTlike",
    "ST_t-channel_top_5f_InclusiveDecays"     : "-n 50  -x 136.02 --isTTlike",

    # --- Multiboson ---
    "WW"                              : "-n 20  -x 118.7 --isDYlike",
    "WZ"                              : "-n 20  -x 47.13 --isDYlike",
    "ZZ"                              : "-n 20  -x 16.523 --isDYlike",

    "ZZZ"                             : "-n 50  -x 0.0147 --isDYlike",
    "WZZ"                             : "-n 50  -x 0.057 --isDYlike",
    "WWW_4F"                          : "-n 50  -x 0.209 --isDYlike",
    "WWZ_4F"                          : "-n 50  -x 0.168 --isDYlike",

    # --- TTX ---
    "TTWJetsToLNu"                    : "-n 20  -x 0.2043 --isTTlike ",
    "TTWJetsToQQ"                     : "-n 20  -x 0.4062  --isTTlike",
    "TTZToLLNuNu_M-10"                : "-n 20  -x 0.2529  --isTTlike",
    "TTWW"                            : "-n 20  -x 0.006979  --isTTlike",
    "TTZZ"                            : "-n 20  -x 0.001386  --isTTlike",
    "TTWZ"                            : "-n 20  -x 0.002441  --isTTlike",

    "TTZToQQ"                         : "-n 20  -x 0.5104    --isTTlike", 
    "TTWH"                            : "-n 20  -x 0.001143  --isTTlike",
    "TTZH"                            : "-n 20  -x 0.001136  --isTTlike",


    # non resonant signal 
    "GluGluToHHTo2B2Tau_TuneCP5_PSWeights_node_SM" : "-n 10   -x 0.01618 --isDYlike"
}




# Retrieve file name
flag_bkg = {}
check_bkg = {}
bkg_files = os.listdir(inputDir_bkg)         # retrieve all the files in the inputDir_bkg

for f in bkg_files:
    check_bkg[f] = []

for label in bkg_map:
    flag_bkg[label] = []
    for f in bkg_files:
        if '%s.txt'%label == f:
            flag_bkg[label].append(f)
            check_bkg[f].append(label)
            bkg_map[label] += ' -i %s/%s  -o  %s/%s '%(inputDir_bkg, f, skimDir, names[label])

# Raise errors if:
if execute_bkg:
 for label in bkg_map:
     if len(flag_bkg[label])>1:
         sys.exit('\nERROR: many files assigned to the same bkg key --- exit()  %s '%flag_bkg[label])
     elif len(flag_bkg[label])<1:
         sys.exit('\nERROR: element in the bkg_map not found  --- exit()    %s'%label)

 for f in bkg_files:
     if len(check_bkg[f])>1:
         sys.exit('\nERROR: many bkg keys assigned to the same file --- exit()   %s'%check_bkg[f] )
     elif len(check_bkg[f])<1:
         print('File not in the bkg_map:   ', f)

 print('\nBackground Files found')












# ############
# SIGNALS MAP 
sig_map = {}
sig_map = {
    # --- GluGlu -> Graviton -> HH -> bb tautau ---
    "GluGluToBulkGravitonToHHTo2B2Tau_M-250"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-260"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-270"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-280"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-300"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-320"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-350"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-400"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-450"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-500"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-550"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-600"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-650"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-700"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-750"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-800"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-850"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-900"     :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-1000"    :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-1250"    :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-1500"    :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-1750"    :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-2000"    :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-2500"    :  "-n 20     -x 1.     -a True",
    "GluGluToBulkGravitonToHHTo2B2Tau_M-3000"    :  "-n 20     -x 1.     -a True",



    # # --- GluGlu -> Radion -> HH -> bb tautau ---
    "GluGluToRadionToHHTo2B2Tau_M-250"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-260"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-270"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-280"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-300"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-320"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-350"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-400"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-450"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-500"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-550"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-600"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-650"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-700"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-750"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-800"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-850"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-900"     :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-1000"    :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-1250"    :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-1500"    :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-1750"    :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-2000"    :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-2500"    :  "-n 20     -x 1.     -a True",
    "GluGluToRadionToHHTo2B2Tau_M-3000"    :  "-n 20     -x 1.     -a True"

    # # --- signals with cross sections ---
    # # --- GluGlu -> Graviton -> HH -> bb tautau ---
    # "GluGluToBulkGravitonToHHTo2B2Tau_M-500"     :  "-n 20     -x 1.941       -a True",
    # "GluGluToBulkGravitonToHHTo2B2Tau_M-600"     :  "-n 20     -x 1.271       -a True",
    # "GluGluToBulkGravitonToHHTo2B2Tau_M-650"     :  "-n 20     -x 1.012       -a True",
    # "GluGluToBulkGravitonToHHTo2B2Tau_M-700"     :  "-n 20     -x 0.8049      -a True",
    # "GluGluToBulkGravitonToHHTo2B2Tau_M-750"     :  "-n 20     -x 0.6405      -a True",
    # "GluGluToBulkGravitonToHHTo2B2Tau_M-800"     :  "-n 20     -x 0.5119      -a True",
    # "GluGluToBulkGravitonToHHTo2B2Tau_M-850"     :  "-n 20     -x 0.4103      -a True",
    # "GluGluToBulkGravitonToHHTo2B2Tau_M-900"     :  "-n 20     -x 0.3301      -a True",
    # "GluGluToBulkGravitonToHHTo2B2Tau_M-1000"    :  "-n 20     -x 0.2169      -a True",
    # "GluGluToBulkGravitonToHHTo2B2Tau_M-1250"    :  "-n 20     -x 0.08201     -a True",
    # "GluGluToBulkGravitonToHHTo2B2Tau_M-1500"    :  "-n 20     -x 0.03394     -a True",
    # "GluGluToBulkGravitonToHHTo2B2Tau_M-2000"    :  "-n 20     -x 0.007042    -a True",
    # "GluGluToBulkGravitonToHHTo2B2Tau_M-2500"    :  "-n 20     -x 0.0017340   -a True",
    # "GluGluToBulkGravitonToHHTo2B2Tau_M-3000"    :  "-n 20     -x 0.0004743   -a True",


    # # --- GluGlu -> Radion -> HH -> bb tautau ---
    # "GluGluToRadionToHHTo2B2Tau_M-500"     :  "-n 20     -x 1402      -a True",
    # "GluGluToRadionToHHTo2B2Tau_M-600"     :  "-n 20     -x 1395      -a True",
    # "GluGluToRadionToHHTo2B2Tau_M-650"     :  "-n 20     -x 1376      -a True",
    # "GluGluToRadionToHHTo2B2Tau_M-700"     :  "-n 20     -x 1352      -a True",
    # "GluGluToRadionToHHTo2B2Tau_M-750"     :  "-n 20     -x 1322      -a True",
    # "GluGluToRadionToHHTo2B2Tau_M-800"     :  "-n 20     -x 1289      -a True",
    # "GluGluToRadionToHHTo2B2Tau_M-850"     :  "-n 20     -x 1252      -a True",
    # "GluGluToRadionToHHTo2B2Tau_M-900"     :  "-n 20     -x 1213      -a True",
    # "GluGluToRadionToHHTo2B2Tau_M-1000"    :  "-n 20     -x 1132      -a True",
    # "GluGluToRadionToHHTo2B2Tau_M-1250"    :  "-n 20     -x 923.4     -a True",
    # "GluGluToRadionToHHTo2B2Tau_M-1500"    :  "-n 20     -x 731.1     -a True",
    # "GluGluToRadionToHHTo2B2Tau_M-2000"    :  "-n 20     -x 432.4     -a True",
    # "GluGluToRadionToHHTo2B2Tau_M-2500"    :  "-n 20     -x 243.2     -a True",
    # "GluGluToRadionToHHTo2B2Tau_M-3000"    :  "-n 20     -x 131.3     -a True"






    # ---- other glu glu -> Radion
    # "GluGluToRadionToHHTo2B2Tau_M-250_"    :  "-n 20     -x 1      ",         #-a True",
    # "GluGluToRadionToHHTo2B2Tau_M-260"     :  "-n 20     -x 1      ",         #-a True",
    # "GluGluToRadionToHHTo2B2Tau_M-270"     :  "-n 20     -x 1      ",         #-a True",
    # "GluGluToRadionToHHTo2B2Tau_M-300_"    :  "-n 20     -x 1      ",         #-a True",
    # "GluGluToRadionToHHTo2B2Tau_M-320"     :  "-n 20     -x 1      ",         #-a True",
    # "GluGluToRadionToHHTo2B2Tau_M-350"     :  "-n 20     -x 1      ",         #-a True",
    # "GluGluToRadionToHHTo2B2Tau_M-400"     :  "-n 20     -x 1      ",         #-a True",
    # "GluGluToRadionToHHTo2B2Tau_M-450"     :  "-n 20     -x 1      ",         #-a True",
    # "GluGluToRadionToHHTo2B2Tau_M-550"     :  "-n 20     -x 1      ",         #-a True",
    # "GluGluToRadionToHHTo2B2Tau_M-600"     :  "-n 20     -x 1      ",         #-a True",
    # "GluGluToRadionToHHTo2B2Tau_M-650"     :  "-n 20     -x 1      ",         #-a True",
    # "GluGluToRadionToHHTo2B2Tau_M-700"     :  "-n 20     -x 1      ",         #-a True",
    # "GluGluToRadionToHHTo2B2Tau_M-800"     :  "-n 20     -x 1      ",         #-a True",
    # "GluGluToRadionToHHTo2B2Tau_M-850"     :  "-n 20     -x 1      ",         #-a True",
    # "GluGluToRadionToHHTo2B2Tau_M-900"     :  "-n 20     -x 1      ",         #-a True",
    # "GluGluToRadionToHHTo2B2Tau_M-1750"    :  "-n 20     -x 1      ",         #-a True",








    # --- NMSSM X -> YH -> bb tautau ---
    # -- Mx = 500
    # "NMSSM_XToYHTo2Tau2B_MX-500_MY-100"    :  "-n 20     -x 1",
    # "NMSSM_XToYHTo2Tau2B_MX-500_MY-125"    :  "-n 20     -x 1",
    # "NMSSM_XToYHTo2Tau2B_MX-500_MY-150"    :  "-n 20     -x 1",
    # "NMSSM_XToYHTo2Tau2B_MX-500_MY-250"    :  "-n 20     -x 1",
    # "NMSSM_XToYHTo2Tau2B_MX-500_MY-300"    :  "-n 20     -x 1",
    # "NMSSM_XToYHTo2Tau2B_MX-500_MY-60"     :  "-n 20     -x 1",
    # "NMSSM_XToYHTo2Tau2B_MX-500_MY-70"     :  "-n 20     -x 1",
    # "NMSSM_XToYHTo2Tau2B_MX-500_MY-80"     :  "-n 20     -x 1",
    # "NMSSM_XToYHTo2Tau2B_MX-500_MY-90"     :  "-n 20     -x 1",

    # # -- Mx = 1000
    # "NMSSM_XToYHTo2Tau2B_MX-1000_MY-100"   :  "-n 20     -x 1",
    # "NMSSM_XToYHTo2Tau2B_MX-1000_MY-125"   :  "-n 20     -x 1",
    # "NMSSM_XToYHTo2Tau2B_MX-1000_MY-150"   :  "-n 20     -x 1",
    # "NMSSM_XToYHTo2Tau2B_MX-1000_MY-250"   :  "-n 20     -x 1",
    # "NMSSM_XToYHTo2Tau2B_MX-1000_MY-300"   :  "-n 20     -x 1",
    # "NMSSM_XToYHTo2Tau2B_MX-1000_MY-60"    :  "-n 20     -x 1",
    # "NMSSM_XToYHTo2Tau2B_MX-1000_MY-70"    :  "-n 20     -x 1",
    # "NMSSM_XToYHTo2Tau2B_MX-1000_MY-80"    :  "-n 20     -x 1",
    # "NMSSM_XToYHTo2Tau2B_MX-1000_MY-90"    :  "-n 20     -x 1",



    # # --- NMSSM - Raffa MC: X -> YH -> bb tautau      where mY [90,150] and mX [300,2500]
    # "NMSSM_XToYHTo2B2Tau_MX_300_600"         : "-n 20      -x 1      ",        #mY 90 to 150
    # "NMSSM_XToYHTo2B2Tau_MX_600_1000"        : "-n 20      -x 1      ",
    # "NMSSM_XToYHTo2B2Tau_MX_1000_1500"       : "-n 20      -x 1      ",
    # "NMSSM_XToYHTo2B2Tau_MX_1500_2000"       : "-n 20      -x 1      ",
    # "NMSSM_XToYHTo2B2Tau_MX_2000_2500"       : "-n 20      -x 1      ",

    # # --- Graviton -> HH -> bbtautau     where mH [90,150] and mG [350,2500]
    # "RSGravitonToHHToTo2B2Tau_mG_350to650"   : "-n 20      -x 1      ",     #mH 90 to 150
    # "RSGravitonToHHToTo2B2Tau_mG_650to1000"  : "-n 20      -x 1      ",
    # "RSGravitonToHHToTo2B2Tau_mG_1000to1500" : "-n 20      -x 1      ",
    # "RSGravitonToHHToTo2B2Tau_mG_1500to2000" : "-n 20      -x 1      ",
    # "RSGravitonToHHToTo2B2Tau_mG_2000to2500" : "-n 20      -x 1      ",


    # # --- NMSSM - new Raffa MC 05/02: X -> YH -> bb tautau      where mY [90,150] and mX [300,2500]
    # "NMSSM_XToHY_HTo2B_YTo2Tau_MX_300_600_MY_90_150"         : "-n 20      -x 1      ",        #mY 90 to 150
    # "NMSSM_XToHY_HTo2B_YTo2Tau_MX_600_1000_MY_90_150"        : "-n 20      -x 1      ",
    # "NMSSM_XToHY_HTo2B_YTo2Tau_MX_1000_1500_MY_90_150"       : "-n 20      -x 1      ",
    # "NMSSM_XToHY_HTo2B_YTo2Tau_MX_1500_2000_MY_90_150"       : "-n 20      -x 1      ",
    # "NMSSM_XToHY_HTo2B_YTo2Tau_MX_2000_2500_MY_90_150"       : "-n 20      -x 1      ",
    # "NMSSM_XToHY_HTo2B_YTo2Tau_MX_300_600_MY_75_175"        :  "-n 20      -x 1      ",
    # "NMSSM_XToHY_HTo2B_YTo2Tau_MX_600_1000_MY_75_175"       :  "-n 20      -x 1      ",
    # "NMSSM_XToHY_HTo2B_YTo2Tau_MX_1500_2000_MY_75_175"      :  "-n 20      -x 1      ",
    # "NMSSM_XToHY_HTo2B_YTo2Tau_MX_1000_1500_MY_75_175"      :  "-n 20      -x 1      ",
    # "NMSSM_XToHY_HTo2B_YTo2Tau_MX_2000_2500_MY_75_175"      :  "-n 20      -x 1      "


}    


# Retrieve file name
flag_sig = {}
check_sig = {}
sig_files = os.listdir(inputDir_sig)         # retrieve all the files in the inputDir_sig

for f in sig_files:
    check_sig[f] = []

for label in sig_map:
    flag_sig[label] = []
    for f in sig_files:
        if '%s.txt'%label == f:
            flag_sig[label].append(f)
            check_sig[f].append(label)
            sig_map[label] += ' -i %s/%s  -o  %s/%s '%(inputDir_sig, f, skimDir, names[label])


# Raise errors if:
if execute_sig:
 for label in sig_map:
     if len(flag_sig[label])>1:
         sys.exit('\nERROR: many files assigned to the same sig key --- exit()  %s '%flag_sig[label])
     elif len(flag_sig[label])<1:
         sys.exit('\nERROR: element in the sig_map not found  --- exit()    %s'%label)

 for f in sig_files:
     if len(check_sig[f])>1:
         sys.exit('\nERROR: many sig keys assigned to the same file --- exit()   %s'%check_sig[f] )
     elif len(check_sig[f])<1:
         print('File not in the sig_map:   ', f)

 print('\nSignal Files found' )














# ############
# DATA MAP 
data_map = {}
data_map = {
    # --- Tau ---
    "Tau_Run2016B" :  "-n 200    -d True  --datasetType 2",
    "Tau_Run2016C" :  "-n 200    -d True  --datasetType 2",
    "Tau_Run2016D" :  "-n 200    -d True  --datasetType 2",
    "Tau_Run2016E" :  "-n 200    -d True  --datasetType 2",
    "Tau_Run2016F" :  "-n 200    -d True  --datasetType 2",
    
    # --- Mu ---
    "SingleMuon_Run2016B" :  "-n 200    -d True",
    "SingleMuon_Run2016C" :  "-n 200    -d True",
    "SingleMuon_Run2016D" :  "-n 200    -d True",
    "SingleMuon_Run2016E" :  "-n 200    -d True",
    "SingleMuon_Run2016F" :  "-n 200    -d True",

    # --- Electron ---
    "SingleElectron_Run2016B" :  "-n 200    -d True",
    "SingleElectron_Run2016C" :  "-n 200    -d True",
    "SingleElectron_Run2016D" :  "-n 200    -d True",
    "SingleElectron_Run2016E" :  "-n 200    -d True",
    "SingleElectron_Run2016F" :  "-n 200    -d True",

    # --- MET ---
    "MET_Run2016B" :  "-n 200    -d True   --datasetType 1",
    "MET_Run2016C" :  "-n 200    -d True   --datasetType 1",
    "MET_Run2016D" :  "-n 200    -d True   --datasetType 1",
    "MET_Run2016E" :  "-n 200    -d True   --datasetType 1",
    "MET_Run2016F" :  "-n 200    -d True   --datasetType 1"
}    


# Retrieve file name
flag_data = {}
check_data = {}
data_files = os.listdir(inputDir_data)         # retrieve all the files in the inputDir_data

for f in data_files:
    check_data[f] = []

for label in data_map:
    flag_data[label] = []
    for f in data_files:
        if ('%s'%label) in f:
            flag_data[label].append(f)
            check_data[f].append(label)
            data_map[label] += ' -i %s/%s  -o  %s/%s '%(inputDir_data, f, skimDir, names[label])

# Raise errors if:
if execute_data:
 for label in data_map:
     if len(flag_data[label])>1:
         sys.exit('\nERROR: many files assigned to the same data key --- exit()  %s '%flag_data[label])
     elif len(flag_data[label])<1:
         sys.exit('\nERROR: element in the data_map not found  --- exit()    %s'%label)

 for f in data_files:
     if len(check_data[f])>1:
         sys.exit('\nERROR: many data keys assigned to the same file --- exit()   %s'%check_data[f] )
     elif len(check_data[f])<1:
         print('File not in the data_map:   ', f)

 print('\nData Files found' )



#------------------------------------------------------------------------------------------











# write commands to a shell script that we will run in singularity        
shell_script_file = os.path.join(f"{klubDir}/scripts", "submit_mib_tmp.sh")

if not os.path.exists(outDir):
    os.makedirs(outDir)
if not os.path.exists(skimDir):
    os.makedirs(skimDir)

with open(shell_script_file, 'w') as script_file:
    script_file.write("#!/bin/bash\n\n")
    script_file.write(f"cd {klubDir}\n")
    script_file.write("source /cvmfs/cms.cern.ch/cmsset_default.sh\n")
    script_file.write(f"source {klubDir}/scripts/setup.sh\n\n")


#---------------
# SUBMIT BACKGROUNDS

if execute_bkg:
    print('\nSubmitting - backgrounds - ')
    print('OUTDIR = ', outDir)
    with open('%s'%logFile, 'w') as logF:
        logF.write("Submitting - backgrounds - \nOUTDIR = %s"%outDir)
    
    with open(shell_script_file, 'w') as script_file:
        for label in bkg_map:
            command = f"{baseCommand} {bkg_map[label]} --pu {puDir}\n"
            print('bkg command:  ', command)
            script_file.write(command)
        os.chmod(shell_script_file, 0o755)

#---------------
# SUBMIT SIGNALS

if execute_sig:
    print('\nSubmitting - signals - ')
    print('OUTDIR = ', outDir)
    with open('%s'%logFile, 'w') as logF:
        logF.write("Submitting - signals - \nOUTDIR = %s"%outDir)
    with open(shell_script_file, 'w') as script_file:
        for label in sig_map:
            command = f"{baseCommand} {sig_map[label]} --pu {puDir}\n"
            print('sig command:  ', command)
            script_file.write(command)
        os.chmod(shell_script_file, 0o755)

#---------------
# SUBMIT DATA

if execute_data:
    print('\nSubmitting - data - ')
    print('OUTDIR = ', outDir)
    with open('%s'%logFile, 'w') as logF:
        logF.write("Submitting - data - \nOUTDIR = %s"%outDir)
    with open(shell_script_file, 'w') as script_file:
        for label in data_map:
            command = f"{baseCommand} {data_map[label]}"
            print('data command:  ', command)
            script_file.write(command)
        os.chmod(shell_script_file, 0o755)

###################


















###### Cross-section information ######
# TT
# XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
# TT x section: 831.76 for inclusive sample, W->had 67,60% , W->l nu 3*10,8% = 32,4% (sum over all leptons)
# hh = 45.7%      -     ll = 10.5%    -    hl = 21.9% (x2 for permutation t-tbar)

### DY
# xsec from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#DY_Z

### Electroweak
# xsec from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109

### Single Top
# xsec from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109

### SM Higgs
# from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNHLHE2019

### HXSWG: xs(ZH) = 0.880 pb, xs(W+H) = 0.831 pb, xs(W-H) = 0.527 pb, xs(ggH) = 48.61 pb, xs(VBFH) = 3.766 pb, xs(ttH) = 0.5071 pb
# Z->qq : 69.91% , Z->ll : 3,3658% (x3 for all the leptons), H->bb : 57.7%  , H->tautau : 6.32%
# ZH (Zll, Hbb) : XSBD (xs ZH * BR Z) * H->bb, ZH (Zqq, Hbb) : XSBD (xs ZH * BR Z) * H->bb
# ZH (Zall, Htautau) : XS teor ZH * BR H->tautau

### Multiboson
# xsec from https://arxiv.org/abs/1408.5243 (WW), https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson (WZ,ZZ)
# Some XS Taken from HTT http://cms.cern.ch/iCMS/user/noteinfo?cmsnoteid=CMS%20AN-2019/109
# Some other XS taken from http://cms.cern.ch/iCMS/jsp/db_notes/noteInfo.jsp?cmsnoteid=CMS%20AN-2019/111
