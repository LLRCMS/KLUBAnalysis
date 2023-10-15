import os

MC = True
if MC:
    tag = "BJetFix_Background"
else:
     tag = "Sep23_Data"
base = "/data_CMS/cms/alves/HHresonant_SKIMS/SKIMS_UL18_" + tag


if MC:
    samples = {
        # "TTToHadronic"                      : 40,
        # "TTTo2L2Nu"                         : 3001,
        # "TTToSemiLeptonic"                  : 400,

        # "DYJetsToLL_M-50_TuneCP5_13TeV-amc" : 600,
        # "DYJetsToLL_LHEFilterPtZ-0To50"     : 30,
        # "DYJetsToLL_LHEFilterPtZ-50To100"   : 2000,
        # "DYJetsToLL_LHEFilterPtZ-100To250"  : 30,
        # "DYJetsToLL_LHEFilterPtZ-250To400"  : 795,
        # "DYJetsToLL_LHEFilterPtZ-400To650"  : 30,
        # "DYJetsToLL_LHEFilterPtZ-650ToInf"  : 212,
        # "DYJetsToLL_0J"                     : 30,
        # "DYJetsToLL_1J"                     : 30,
        # "DYJetsToLL_2J"                     : 30,

        # "WJetsToLNu_TuneCP5_13TeV-madgraph" : 40,
        # "WJetsToLNu_HT-70To100"             : 100,
        # "WJetsToLNu_HT-100To200"            : 50,
        # "WJetsToLNu_HT-200To400"            : 50,
        # "WJetsToLNu_HT-400To600"            : 50,
        # "WJetsToLNu_HT-600To800"            : 50,
        # "WJetsToLNu_HT-800To1200"           : 50,
        # "WJetsToLNu_HT-1200To2500"          : 50,
        # "WJetsToLNu_HT-2500ToInf"           : 50,

        # "EWKWPlus2Jets_WToLNu"              : 20,
        # "EWKWMinus2Jets_WToLNu"             : 20,
        # "EWKZ2Jets_ZToLL"                   : 19,

        # "ST_tW_antitop_5f_inclusive"        : 20,
        # "ST_tW_top_5f_inclusive"            : 20,
        # "ST_t-channel_antitop"              : 20,
        # "ST_t-channel_top"                  : 80,

        # "GluGluHToTauTau"                   : 30,
        # "VBFHToTauTau"                      : 30,
        # "WplusHToTauTau"                    : 30,
        # "WminusHToTauTau"                   : 30,
        # "ZHToTauTau"                        : 30,

        # "ZH_HToBB_ZToLL"                    : 30,
        # "ZH_HToBB_ZToQQ"                    : 30,

        # "ttHToNonbb"                        : 20,
        # "ttHTobb"                           : 20,
        # "ttHToTauTau"                       : 20,
        
        # "_WW_TuneCP5"                       : 5,
        # "_WZ_TuneCP5"                       : 5,
        # "_ZZ_TuneCP5"                       : 5,

        # "49_WWW"                          : 4,
        # "50_WWW"                          : 10,
        # "51_WWZ"                          : 5,
        # "52_WWZ"                          : 10,
        # "53_WZZ"                          : 10,
        # "54_WZZ"                          : 10,
        # "55_ZZZ"                          : 10,
        # "56_ZZZ"                          : 10,

        # "TTWJetsToLNu"                      : 20,
        # "TTWJetsToQQ"                       : 11,
        # "TTZToLLNuNu"                       : 20,
        # "TTZToQQ"                          : 200,
        # "TTWW"                              : 10,
        # "TTZZ"                              : 10,
        # "TTWZ"                              : 7,

        # "TTWH"                              : 6,
        # "TTZH"                              : 10,
    }
else:
    samples = {
        "EGamma__Run2018A"     : 200,
        "EGamma__Run2018B"     : 200,
        "EGamma__Run2018C"     : 200,
        "EGamma__Run2018D"     : 200,
        "SingleMuon__Run2018A" : 200,
        "SingleMuon__Run2018B" : 200,
        "SingleMuon__Run2018C" : 200,
        "SingleMuon__Run2018D" : 200,
        "MET__Run2018A"        : 200,
        "MET__Run2018B"        : 200,
        "MET__Run2018C"        : 200,
        "MET__Run2018D"        : 200,
        "Tau__Run2018A"        : 200,
        "Tau__Run2018B"        : 200,
        "Tau__Run2018C"        : 200,
        "Tau__Run2018D"        : 200,
    }

for smp, nsamples in samples.items():
    fname = os.path.join(base, smp, "goodfiles.txt")    
    with open(fname, "w") as f:
        for ns in range(nsamples):
            f.write(os.path.join(base, smp, "output_"+str(ns)+".root\n"))
