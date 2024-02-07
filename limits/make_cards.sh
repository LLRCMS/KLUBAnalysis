export PYTHONPATH=$PWD/physicsModels:$PYTHONPATH

### Most useful options:
# -f : input root file
# -i : input cfg
# -o : output name
# -c : channel
# -y : year
# -b : bin-by-bin unc
# -u : shape unc
# -t : theory unc
# -q : QCD from rateParam (1, default), QCD from file (0)
# -B : rename singleH processes to enable the BR scaling (1 by default)
# example:
#  python write_card.py -f analyzedOutPlotter_TauTau_prova.root -o prova -c TauTau -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_TauTau_synch_11June2020_limits_prova/mainCfg_TauTau_Legacy2018_limits.cfg

# 13 May 2021
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_TauTau_23Apr2021.root -o 2016_13May2021 -c TauTau -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_MuTau_23Apr2021.root  -o 2016_13May2021 -c MuTau  -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_ETau_23Apr2021.root   -o 2016_13May2021 -c ETau   -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#
#python write_card.py -f preparedHistos/analyzedOutPlotter_2017_TauTau_23Apr2021.root -o 2017_13May2021 -c TauTau -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2017_MuTau_23Apr2021.root  -o 2017_13May2021 -c MuTau  -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2017_ETau_23Apr2021.root   -o 2017_13May2021 -c ETau   -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
#
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_TauTau_23Apr2021.root -o 2018_13May2021 -c TauTau -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_MuTau_23Apr2021.root  -o 2018_13May2021 -c MuTau  -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_ETau_23Apr2021.root   -o 2018_13May2021 -c ETau   -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg


# 21 May 2021 noScale
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_TauTau_23Apr2021_noScales.root -o 2016_21May2021_noScales -c TauTau -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_MuTau_23Apr2021_noScales.root  -o 2016_21May2021_noScales -c MuTau  -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_ETau_23Apr2021_noScales.root   -o 2016_21May2021_noScales -c ETau   -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2017_TauTau_23Apr2021_noScales.root -o 2017_21May2021_noScales -c TauTau -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2017_MuTau_23Apr2021_noScales.root  -o 2017_21May2021_noScales -c MuTau  -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2017_ETau_23Apr2021_noScales.root   -o 2017_21May2021_noScales -c ETau   -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_TauTau_23Apr2021_noScales.root -o 2018_21May2021_noScales -c TauTau -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_MuTau_23Apr2021_noScales.root  -o 2018_21May2021_noScales -c MuTau  -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_ETau_23Apr2021_noScales.root   -o 2018_21May2021_noScales -c ETau   -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg

# 25 May 2021 noScale
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_TauTau_25May2021_noScales.root -o 2016_25May2021_noScales -c TauTau -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_05_25/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_MuTau_25May2021_noScales.root  -o 2016_25May2021_noScales -c MuTau  -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_05_25/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_ETau_25May2021_noScales.root   -o 2016_25May2021_noScales -c ETau   -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_05_25/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg

#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_TauTau_25May2021_noScales_1000bins.root -o 2016_25May2021_noScales_1000bins -c TauTau -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_05_25/TauTau_2016_1000bins/total/mainCfg_TauTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_MuTau_25May2021_noScales_1000bins.root  -o 2016_25May2021_noScales_1000bins -c MuTau  -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_05_25/MuTau_2016_1000bins/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_ETau_25May2021_noScales_1000bins.root   -o 2016_25May2021_noScales_1000bins -c ETau   -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_05_25/ETau_2016_1000bins/total/mainCfg_ETau_Legacy2016_limits.cfg

#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_TauTau_3Jun2021_noScales_MT2.root -o 2016_3Jun2021_noScales_MT2 -c TauTau -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_06_03/TauTau_2016__MT2/total/mainCfg_TauTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_MuTau_3Jun2021_noScales_MT2.root  -o 2016_3Jun2021_noScales_MT2 -c MuTau  -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_06_03/MuTau_2016__MT2/total/mainCfg_MuTau_Legacy2016_limits.cfg


# 18 June 2021
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_TauTau_8June2021.root -o 2016_19June2021 -c TauTau -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_MuTau_8June2021.root  -o 2016_19June2021 -c MuTau  -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_ETau_8June2021.root   -o 2016_19June2021 -c ETau   -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2017_TauTau_8June2021.root -o 2017_19June2021 -c TauTau -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2017_MuTau_8June2021.root  -o 2017_19June2021 -c MuTau  -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2017_ETau_8June2021.root   -o 2017_19June2021 -c ETau   -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_TauTau_8June2021.root -o 2018_19June2021 -c TauTau -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_MuTau_8June2021.root  -o 2018_19June2021 -c MuTau  -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_ETau_8June2021.root   -o 2018_19June2021 -c ETau   -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg

# 05 July 2021 - 1000bins
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2016_TauTau_5July2021_1000bins.root -o 2016_5July2021_1000bins -c TauTau -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/TauTau_2016_1000bins/total/mainCfg_TauTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2016_MuTau_5July2021_1000bins.root  -o 2016_5July2021_1000bins -c MuTau  -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/MuTau_2016_1000bins/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2016_ETau_5July2021_1000bins.root   -o 2016_5July2021_1000bins -c ETau   -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/ETau_2016_1000bins/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2017_TauTau_5July2021_1000bins.root -o 2017_5July2021_1000bins -c TauTau -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/TauTau_2017_1000bins/total/mainCfg_TauTau_Legacy2017_limits.cfg
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2017_MuTau_5July2021_1000bins.root  -o 2017_5July2021_1000bins -c MuTau  -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/MuTau_2017_1000bins/total/mainCfg_MuTau_Legacy2017_limits.cfg
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2017_ETau_5July2021_1000bins.root   -o 2017_5July2021_1000bins -c ETau   -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/ETau_2017_1000bins/total/mainCfg_ETau_Legacy2017_limits.cfg
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2018_TauTau_5July2021_1000bins.root -o 2018_5July2021_1000bins -c TauTau -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/TauTau_2018_1000bins/total/mainCfg_TauTau_Legacy2018_limits.cfg
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2018_MuTau_5July2021_1000bins.root  -o 2018_5July2021_1000bins -c MuTau  -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/MuTau_2018_1000bins/total/mainCfg_MuTau_Legacy2018_limits.cfg
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2018_ETau_5July2021_1000bins.root   -o 2018_5July2021_1000bins -c ETau   -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/ETau_2018_1000bins/total/mainCfg_ETau_Legacy2018_limits.cfg

# 27 July 2021 - 1000bins
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2016_TauTau_27July2021_1000bins.root -o 2016_13Aug2021_1000bins -c TauTau -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/TauTau_2016_1000bins/total/mainCfg_TauTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2016_MuTau_27July2021_1000bins.root  -o 2016_13Aug2021_1000bins -c MuTau  -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/MuTau_2016_1000bins/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2016_ETau_27July2021_1000bins.root   -o 2016_13Aug2021_1000bins -c ETau   -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/ETau_2016_1000bins/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2017_TauTau_27July2021_1000bins.root -o 2017_13Aug2021_1000bins -c TauTau -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/TauTau_2017_1000bins/total/mainCfg_TauTau_Legacy2017_limits.cfg
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2017_MuTau_27July2021_1000bins.root  -o 2017_13Aug2021_1000bins -c MuTau  -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/MuTau_2017_1000bins/total/mainCfg_MuTau_Legacy2017_limits.cfg
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2017_ETau_27July2021_1000bins.root   -o 2017_13Aug2021_1000bins -c ETau   -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/ETau_2017_1000bins/total/mainCfg_ETau_Legacy2017_limits.cfg
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2018_TauTau_27July2021_1000bins.root -o 2018_13Aug2021_1000bins -c TauTau -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/TauTau_2018_1000bins/total/mainCfg_TauTau_Legacy2018_limits.cfg
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2018_MuTau_27July2021_1000bins.root  -o 2018_13Aug2021_1000bins -c MuTau  -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/MuTau_2018_1000bins/total/mainCfg_MuTau_Legacy2018_limits.cfg
#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2018_ETau_27July2021_1000bins.root   -o 2018_13Aug2021_1000bins -c ETau   -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/ETau_2018_1000bins/total/mainCfg_ETau_Legacy2018_limits.cfg

#python write_card.py -f preparedHistos_1000bins/analyzedOutPlotter_2018_TauTau_27July2021_1000bins_boostOnly.root -o 2018_27July2021_1000bins_boostOnly -c TauTau -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_10/TauTau_2018_1000bins_boostOnly/total/mainCfg_TauTau_Legacy2018_limits.cfg
#python write_card.py -f analyzedOutPlotter_2018_TauTau_test_symmQCD.root -o test_symmQCD -c TauTau -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_08_13/TauTau_2018_test/mainCfg_TauTau_Legacy2018_limits.cfg

# 21 Dec 2021 - tauFakes CR
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_MuTau_21Dec2021_fakesCR_mT1.root      -o 2016_21Dec2021_fakesCR_mT1     -c MuTau   -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_12_21/MuTau_2016_fakesCR_mT1/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_MuTau_21Dec2021_fakesCR_MET_DNN.root  -o 2016_21Dec2021_fakesCR_MET_DNN -c MuTau   -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_12_21/MuTau_2016_fakesCR_MET_DNN/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_MuTau_21Dec2021_fakesCR_MET_DNN.root  -o 2016_21Dec2021_fakesCR -c MuTau   -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_12_21/MuTau_2016_fakesCR_MET_DNN/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2017_MuTau_21Dec2021_fakesCR_MET_DNN.root  -o 2017_21Dec2021_fakesCR -c MuTau   -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_12_21/MuTau_2017_fakesCR_MET_DNN/total/mainCfg_MuTau_Legacy2017_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_MuTau_21Dec2021_fakesCR_MET_DNN.root  -o 2018_21Dec2021_fakesCR -c MuTau   -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_12_21/MuTau_2018_fakesCR_MET_DNN/total/mainCfg_MuTau_Legacy2018_limits.cfg

# 14 Jan 2022 - final cards
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_ETau_14Jan2022.root   -o 2018_14Jan2022 -c ETau   -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_01_03/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_MuTau_14Jan2022.root  -o 2018_14Jan2022 -c MuTau  -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_01_03/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_TauTau_14Jan2022.root -o 2018_14Jan2022 -c TauTau -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_01_03/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_ETau_14Jan2022.root   -o 2016_14Jan2022 -c ETau   -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_01_03/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_MuTau_14Jan2022.root  -o 2016_14Jan2022 -c MuTau  -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_01_03/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2016_TauTau_14Jan2022.root -o 2016_14Jan2022 -c TauTau -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_01_03/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2017_ETau_14Jan2022.root   -o 2017_14Jan2022 -c ETau   -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_01_03/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2017_MuTau_14Jan2022.root  -o 2017_14Jan2022 -c MuTau  -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_01_03/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2017_TauTau_14Jan2022.root -o 2017_14Jan2022 -c TauTau -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_01_03/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg

# 3 Feb 2022 - rename TT to ttbar
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_ETau_3Feb2022_TTrename.root    -o 2018_3Feb2022_TTrename -c ETau   -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_01_03/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_MuTau_3Feb2022_TTrename.root   -o 2018_3Feb2022_TTrename -c MuTau  -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_01_03/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_TauTau_3Feb2022_TTrename.root  -o 2018_3Feb2022_TTrename -c TauTau -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_01_03/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg

#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_ETau_14Jan2022.root   -o 2018_3Feb2022_TToriginal -c ETau   -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_01_03/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_MuTau_14Jan2022.root  -o 2018_3Feb2022_TToriginal -c MuTau  -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_01_03/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
#python write_card.py -f preparedHistos/analyzedOutPlotter_2018_TauTau_14Jan2022.root -o 2018_3Feb2022_TToriginal -c TauTau -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_01_03/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg

# 24 Nove 2022 - EFT

# ------- 2016 -------
## c2scan
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark c2scan -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark c2scan -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark c2scan -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg

## benchmark 1b
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 1b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 1b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 1b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 2b
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 2b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 2b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 2b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 3b
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 3b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 3b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 3b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 4b
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 4b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 4b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 4b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 5b
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 5b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 5b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 5b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 6b
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 6b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 6b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 6b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 7b
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 7b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 7b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 7b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg

## benchmark 1
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 1 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 1 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 1 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 2
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 2 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 2 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 2 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 3
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 3 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 3 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 3 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 4
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 4 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 4 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 4 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 5
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 5 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 5 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 5 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 6
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 6 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 6 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 6 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 7
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 7 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 7 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 7 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 8
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 8 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 8 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 8 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 9
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 9 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 9 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 9 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 10
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 10 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 10 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 10 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 11
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 11 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 11 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 11 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 12
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 12 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 12 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 12 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
## benchmark 8a
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_ETau_24Nov2022_EFT.root   -o 2016_26Nov2022 -c ETau   -y 2016 --eftBenchmark 8a -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_MuTau_24Nov2022_EFT.root  -o 2016_26Nov2022 -c MuTau  -y 2016 --eftBenchmark 8a -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
#python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2016_TauTau_24Nov2022_EFT.root -o 2016_26Nov2022 -c TauTau -y 2016 --eftBenchmark 8a -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg

# ------- 2017 -------
# c2scan
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark c2scan -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark c2scan -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark c2scan -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg

# benchmark 1b
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 1b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 1b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 1b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 2b
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 2b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 2b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 2b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 3b
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 3b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 3b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 3b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 4b
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 4b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 4b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 4b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 5b
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 5b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 5b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 5b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 6b
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 6b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 6b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 6b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 7b
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 7b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 7b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 7b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg

# benchmark 1
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 1 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 1 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 1 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 2
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 2 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 2 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 2 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 3
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 3 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 3 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 3 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 4
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 4 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 4 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 4 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 5
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 5 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 5 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 5 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 6
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 6 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 6 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 6 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 7
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 7 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 7 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 7 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 8
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 8 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 8 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 8 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 9
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 9 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 9 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 9 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 10
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 10 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 10 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 10 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 11
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 11 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 11 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 11 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 12
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 12 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 12 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 12 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
# benchmark 8a
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_ETau_24Nov2022_EFT.root   -o 2017_26Nov2022 -c ETau   -y 2017 --eftBenchmark 8a -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_MuTau_24Nov2022_EFT.root  -o 2017_26Nov2022 -c MuTau  -y 2017 --eftBenchmark 8a -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos_EFT/analyzedOutPlotter_2017_TauTau_24Nov2022_EFT.root -o 2017_26Nov2022 -c TauTau -y 2017 --eftBenchmark 8a -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_24_EFT/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg

# ------- 2018 -------
# c2scan
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark c2scan -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark c2scan -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark c2scan -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg

# benchmark 1b
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 1b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 1b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 1b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 2b
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 2b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 2b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 2b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 3b
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 3b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 3b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 3b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 4b
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 4b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 4b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 4b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 5b
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 5b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 5b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 5b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 6b
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 6b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 6b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 6b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 7b
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 7b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 7b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 7b -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg

# benchmark 1
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 1 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 1 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 1 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 2
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 2 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 2 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 2 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 3
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 3 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 3 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 3 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 4
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 4 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 4 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 4 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 5
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 5 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 5 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 5 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 6
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 6 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 6 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 6 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 7
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 7 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 7 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 7 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 8
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 8 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 8 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 8 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 9
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 9 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 9 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 9 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 10
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 10 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 10 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 10 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 11
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 11 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 11 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 11 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 12
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 12 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 12 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 12 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
# benchmark 8a
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_ETau_25Nov2022_EFT.root   -o 2018_25Nov2022 -c ETau   -y 2018 --eftBenchmark 8a -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_MuTau_25Nov2022_EFT.root  -o 2018_25Nov2022 -c MuTau  -y 2018 --eftBenchmark 8a -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f ../analysis_2022_11_25_EFT/preparedHistos_EFT/analyzedOutPlotter_2018_TauTau_25Nov2022_EFT.root -o 2018_25Nov2022 -c TauTau -y 2018 --eftBenchmark 8a -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2022_11_25_EFT/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
