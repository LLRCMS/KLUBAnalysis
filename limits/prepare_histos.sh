############
# USAGE:
#
# 1 - prepare files with yield variations --> REMEMBER to modify hardcoded strings (channel, folder...)
# python compute_scales.py -y 2018
#
# 2 - prepare histograms with systematics
# python prepare_histos.py -f ../analysis_TauTau_synch_11June2020_limits_prova/analyzedOutPlotter.root -o prova -c TauTau -y 2018 -q 0 -B 1
#
############

# 23 Apr 2021
#python compute_scales.py -y 2016
#python compute_scales.py -y 2017
#python compute_scales.py -y 2018
#
# python prepare_histos.py -f ../analysis_2021_04_23/TauTau_2016/total/analyzedOutPlotter.root -o 23Apr2021 -c TauTau -y 2016 -q 0 -B 1
# python prepare_histos.py -f ../analysis_2021_04_23/MuTau_2016/total/analyzedOutPlotter.root  -o 23Apr2021 -c MuTau  -y 2016 -q 0 -B 1
# python prepare_histos.py -f ../analysis_2021_04_23/ETau_2016/total/analyzedOutPlotter.root   -o 23Apr2021 -c ETau   -y 2016 -q 0 -B 1
#
# python prepare_histos.py -f ../analysis_2021_04_23/TauTau_2017/total/analyzedOutPlotter.root -o 23Apr2021 -c TauTau -y 2017 -q 0 -B 1
# python prepare_histos.py -f ../analysis_2021_04_23/MuTau_2017/total/analyzedOutPlotter.root  -o 23Apr2021 -c MuTau  -y 2017 -q 0 -B 1
# python prepare_histos.py -f ../analysis_2021_04_23/ETau_2017/total/analyzedOutPlotter.root   -o 23Apr2021 -c ETau   -y 2017 -q 0 -B 1
#
# python prepare_histos.py -f ../analysis_2021_04_23/TauTau_2018/total/analyzedOutPlotter.root -o 23Apr2021 -c TauTau -y 2018 -q 0 -B 1
# python prepare_histos.py -f ../analysis_2021_04_23/MuTau_2018/total/analyzedOutPlotter.root  -o 23Apr2021 -c MuTau  -y 2018 -q 0 -B 1
# python prepare_histos.py -f ../analysis_2021_04_23/ETau_2018/total/analyzedOutPlotter.root   -o 23Apr2021 -c ETau   -y 2018 -q 0 -B 1

#python prepare_histos.py -f ../analysis_2021_04_23/TauTau_2016/total/analyzedOutPlotter.root -o 23Apr2021_noScales -c TauTau -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_23/MuTau_2016/total/analyzedOutPlotter.root  -o 23Apr2021_noScales -c MuTau  -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_23/ETau_2016/total/analyzedOutPlotter.root   -o 23Apr2021_noScales -c ETau   -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_23/TauTau_2017/total/analyzedOutPlotter.root -o 23Apr2021_noScales -c TauTau -y 2017 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_23/MuTau_2017/total/analyzedOutPlotter.root  -o 23Apr2021_noScales -c MuTau  -y 2017 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_23/ETau_2017/total/analyzedOutPlotter.root   -o 23Apr2021_noScales -c ETau   -y 2017 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_23/TauTau_2018/total/analyzedOutPlotter.root -o 23Apr2021_noScales -c TauTau -y 2018 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_23/MuTau_2018/total/analyzedOutPlotter.root  -o 23Apr2021_noScales -c MuTau  -y 2018 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_23/ETau_2018/total/analyzedOutPlotter.root   -o 23Apr2021_noScales -c ETau   -y 2018 -q 0 -B 1

# 25 May 2021
#python prepare_histos.py -f ../analysis_2021_05_25/TauTau_2016/total/analyzedOutPlotter.root -o 25May2021_noScales -c TauTau -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_05_25/MuTau_2016/total/analyzedOutPlotter.root  -o 25May2021_noScales -c MuTau  -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_05_25/ETau_2016/total/analyzedOutPlotter.root   -o 25May2021_noScales -c ETau   -y 2016 -q 0 -B 1

#python prepare_histos.py -f ../analysis_2021_05_25/TauTau_2016_1000bins/total/analyzedOutPlotter.root -o 25May2021_noScales_1000bins -c TauTau -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_05_25/MuTau_2016_1000bins/total/analyzedOutPlotter.root  -o 25May2021_noScales_1000bins -c MuTau  -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_05_25/ETau_2016_1000bins/total/analyzedOutPlotter.root   -o 25May2021_noScales_1000bins -c ETau   -y 2016 -q 0 -B 1

#python prepare_histos.py -f ../analysis_2021_06_03/TauTau_2016__MT2/total/analyzedOutPlotter.root -o 3Jun2021_noScales_MT2 -c TauTau -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_06_03/MuTau_2016__MT2/total/analyzedOutPlotter.root  -o 3Jun2021_noScales_MT2 -c MuTau  -y 2016 -q 0 -B 1


# 8 June
#python prepare_histos.py -f ../analysis_2021_04_23/TauTau_2016/total/analyzedOutPlotter.root -o 8June2021 -c TauTau -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_23/MuTau_2016/total/analyzedOutPlotter.root  -o 8June2021 -c MuTau  -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_23/ETau_2016/total/analyzedOutPlotter.root   -o 8June2021 -c ETau   -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_23/TauTau_2017/total/analyzedOutPlotter.root -o 8June2021 -c TauTau -y 2017 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_23/MuTau_2017/total/analyzedOutPlotter.root  -o 8June2021 -c MuTau  -y 2017 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_23/ETau_2017/total/analyzedOutPlotter.root   -o 8June2021 -c ETau   -y 2017 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_23/TauTau_2018/total/analyzedOutPlotter.root -o 8June2021 -c TauTau -y 2018 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_23/MuTau_2018/total/analyzedOutPlotter.root  -o 8June2021 -c MuTau  -y 2018 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_23/ETau_2018/total/analyzedOutPlotter.root   -o 8June2021 -c ETau   -y 2018 -q 0 -B 1

# 05 July 2021 - 1000bins
#python prepare_histos.py -f ../analysis_2021_04_10/TauTau_2016_1000bins/total/analyzedOutPlotter.root -o 5July2021_1000bins -c TauTau -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_10/MuTau_2016_1000bins/total/analyzedOutPlotter.root  -o 5July2021_1000bins -c MuTau  -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_10/ETau_2016_1000bins/total/analyzedOutPlotter.root   -o 5July2021_1000bins -c ETau   -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_10/TauTau_2017_1000bins/total/analyzedOutPlotter.root -o 5July2021_1000bins -c TauTau -y 2017 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_10/MuTau_2017_1000bins/total/analyzedOutPlotter.root  -o 5July2021_1000bins -c MuTau  -y 2017 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_10/ETau_2017_1000bins/total/analyzedOutPlotter.root   -o 5July2021_1000bins -c ETau   -y 2017 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_10/TauTau_2018_1000bins/total/analyzedOutPlotter.root -o 5July2021_1000bins -c TauTau -y 2018 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_10/MuTau_2018_1000bins/total/analyzedOutPlotter.root  -o 5July2021_1000bins -c MuTau  -y 2018 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_10/ETau_2018_1000bins/total/analyzedOutPlotter.root   -o 5July2021_1000bins -c ETau   -y 2018 -q 0 -B 1

# 27 July test
#python prepare_histos.py -f ../analysis_2021_07_27/TauTau_2016_test/total/analyzedOutPlotter.root -o 27July2021_test7 -c TauTau -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_07_05/TauTau_2016/total/analyzedOutPlotter.root -o 05July2021_test -c TauTau -y 2016 -q 0 -B 1

# 27 July 2021 - 1000bins
#python prepare_histos.py -f ../analysis_2021_04_10/TauTau_2016_1000bins/total/analyzedOutPlotter.root -o 27July2021_1000bins -c TauTau -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_10/MuTau_2016_1000bins/total/analyzedOutPlotter.root  -o 27July2021_1000bins -c MuTau  -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_10/ETau_2016_1000bins/total/analyzedOutPlotter.root   -o 27July2021_1000bins -c ETau   -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_10/TauTau_2017_1000bins/total/analyzedOutPlotter.root -o 27July2021_1000bins -c TauTau -y 2017 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_10/MuTau_2017_1000bins/total/analyzedOutPlotter.root  -o 27July2021_1000bins -c MuTau  -y 2017 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_10/ETau_2017_1000bins/total/analyzedOutPlotter.root   -o 27July2021_1000bins -c ETau   -y 2017 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_10/TauTau_2018_1000bins/total/analyzedOutPlotter.root -o 27July2021_1000bins -c TauTau -y 2018 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_10/MuTau_2018_1000bins/total/analyzedOutPlotter.root  -o 27July2021_1000bins -c MuTau  -y 2018 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_04_10/ETau_2018_1000bins/total/analyzedOutPlotter.root   -o 27July2021_1000bins -c ETau   -y 2018 -q 0 -B 1

#python prepare_histos.py -f ../analysis_2021_04_10/TauTau_2018_1000bins_boostOnly/total/analyzedOutPlotter.root -o 27July2021_1000bins_boostOnly -c TauTau -y 2018 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_08_13/TauTau_2018_test/analyzedOutPlotter.root -o test_symmQCD -c TauTau -y 2018 -q 0 -B 1

# 21 Dec 2021 - tauFakes CR
#python prepare_histos.py -f ../analysis_2021_12_21/MuTau_2016_fakesCR_mT1/total/analyzedOutPlotter.root     -o 21Dec2021_fakesCR_mT1     -c MuTau -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_12_21/MuTau_2016_fakesCR_MET_DNN/total/analyzedOutPlotter.root -o 21Dec2021_fakesCR_MET_DNN -c MuTau -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_12_21/MuTau_2017_fakesCR_MET_DNN/total/analyzedOutPlotter.root -o 21Dec2021_fakesCR_MET_DNN -c MuTau -y 2017 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2021_12_21/MuTau_2018_fakesCR_MET_DNN/total/analyzedOutPlotter.root -o 21Dec2021_fakesCR_MET_DNN -c MuTau -y 2018 -q 0 -B 1

# 03 Jan 2022
#python prepare_histos.py -f ../analysis_2022_01_03/ETau_2018/total/analyzedOutPlotter.root   -o 14Jan2022 -c ETau   -y 2018 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2022_01_03/MuTau_2018/total/analyzedOutPlotter.root  -o 14Jan2022 -c MuTau  -y 2018 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2022_01_03/TauTau_2018/total/analyzedOutPlotter.root -o 14Jan2022 -c TauTau -y 2018 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2022_01_03/ETau_2016/total/analyzedOutPlotter.root   -o 14Jan2022 -c ETau   -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2022_01_03/MuTau_2016/total/analyzedOutPlotter.root  -o 14Jan2022 -c MuTau  -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2022_01_03/TauTau_2016/total/analyzedOutPlotter.root -o 14Jan2022 -c TauTau -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2022_01_03/ETau_2017/total/analyzedOutPlotter.root   -o 14Jan2022 -c ETau   -y 2017 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2022_01_03/MuTau_2017/total/analyzedOutPlotter.root  -o 14Jan2022 -c MuTau  -y 2017 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2022_01_03/TauTau_2017/total/analyzedOutPlotter.root -o 14Jan2022 -c TauTau -y 2017 -q 0 -B 1

# 3 Feb 2022 - rename TT to ttbar
#python prepare_histos.py -f ../analysis_2022_01_03/ETau_2018/total/analyzedOutPlotter.root   -o 3Feb2022_TTrename -c ETau   -y 2018 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2022_01_03/MuTau_2018/total/analyzedOutPlotter.root  -o 3Feb2022_TTrename -c MuTau  -y 2018 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2022_01_03/TauTau_2018/total/analyzedOutPlotter.root -o 3Feb2022_TTrename -c TauTau -y 2018 -q 0 -B 1

# 24 Nove 2022 - EFT
#python prepare_histos.py -f ../analysis_2022_11_24_EFT/ETau_2016/total/analyzedOutPlotter.root   -o 24Nov2022_EFT -c ETau   -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2022_11_24_EFT/MuTau_2016/total/analyzedOutPlotter.root  -o 24Nov2022_EFT -c MuTau  -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2022_11_24_EFT/TauTau_2016/total/analyzedOutPlotter.root -o 24Nov2022_EFT -c TauTau -y 2016 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2022_11_24_EFT/ETau_2017/total/analyzedOutPlotter.root   -o 24Nov2022_EFT -c ETau   -y 2017 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2022_11_24_EFT/MuTau_2017/total/analyzedOutPlotter.root  -o 24Nov2022_EFT -c MuTau  -y 2017 -q 0 -B 1
#python prepare_histos.py -f ../analysis_2022_11_24_EFT/TauTau_2017/total/analyzedOutPlotter.root -o 24Nov2022_EFT -c TauTau -y 2017 -q 0 -B 1

# 28 Dec 2022 / 25 Nov 2022 (2018) - EFT with scales
python prepare_histos.py -f ../analysis_2022_11_24_EFT/ETau_2016/total/analyzedOutPlotter.root   -o 28Dec2022_EFT -c ETau   -y 2016 -q 0 -B 1
python prepare_histos.py -f ../analysis_2022_11_24_EFT/MuTau_2016/total/analyzedOutPlotter.root  -o 28Dec2022_EFT -c MuTau  -y 2016 -q 0 -B 1
python prepare_histos.py -f ../analysis_2022_11_24_EFT/TauTau_2016/total/analyzedOutPlotter.root -o 28Dec2022_EFT -c TauTau -y 2016 -q 0 -B 1
python prepare_histos.py -f ../analysis_2022_11_24_EFT/ETau_2017/total/analyzedOutPlotter.root   -o 28Dec2022_EFT -c ETau   -y 2017 -q 0 -B 1
python prepare_histos.py -f ../analysis_2022_11_24_EFT/MuTau_2017/total/analyzedOutPlotter.root  -o 28Dec2022_EFT -c MuTau  -y 2017 -q 0 -B 1
python prepare_histos.py -f ../analysis_2022_11_24_EFT/TauTau_2017/total/analyzedOutPlotter.root -o 28Dec2022_EFT -c TauTau -y 2017 -q 0 -B 1
python prepare_histos.py -f ../analysis_2022_11_25_EFT/ETau_2018/total/analyzedOutPlotter.root   -o 25Nov2022_EFT -c ETau   -y 2018 -q 0 -B 1
python prepare_histos.py -f ../analysis_2022_11_25_EFT/MuTau_2018/total/analyzedOutPlotter.root  -o 25Nov2022_EFT -c MuTau  -y 2018 -q 0 -B 1
python prepare_histos.py -f ../analysis_2022_11_25_EFT/TauTau_2018/total/analyzedOutPlotter.root -o 25Nov2022_EFT -c TauTau -y 2018 -q 0 -B 1