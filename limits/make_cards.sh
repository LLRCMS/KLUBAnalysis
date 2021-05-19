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
python write_card.py -f preparedHistos/analyzedOutPlotter_2016_TauTau_23Apr2021.root -o 2016_13May2021 -c TauTau -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/TauTau_2016/total/mainCfg_TauTau_Legacy2016_limits.cfg
python write_card.py -f preparedHistos/analyzedOutPlotter_2016_MuTau_23Apr2021.root  -o 2016_13May2021 -c MuTau  -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/MuTau_2016/total/mainCfg_MuTau_Legacy2016_limits.cfg
python write_card.py -f preparedHistos/analyzedOutPlotter_2016_ETau_23Apr2021.root   -o 2016_13May2021 -c ETau   -y 2016 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/ETau_2016/total/mainCfg_ETau_Legacy2016_limits.cfg

python write_card.py -f preparedHistos/analyzedOutPlotter_2017_TauTau_23Apr2021.root -o 2017_13May2021 -c TauTau -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/TauTau_2017/total/mainCfg_TauTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos/analyzedOutPlotter_2017_MuTau_23Apr2021.root  -o 2017_13May2021 -c MuTau  -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/MuTau_2017/total/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f preparedHistos/analyzedOutPlotter_2017_ETau_23Apr2021.root   -o 2017_13May2021 -c ETau   -y 2017 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/ETau_2017/total/mainCfg_ETau_Legacy2017_limits.cfg

python write_card.py -f preparedHistos/analyzedOutPlotter_2018_TauTau_23Apr2021.root -o 2018_13May2021 -c TauTau -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/TauTau_2018/total/mainCfg_TauTau_Legacy2018_limits.cfg
python write_card.py -f preparedHistos/analyzedOutPlotter_2018_MuTau_23Apr2021.root  -o 2018_13May2021 -c MuTau  -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/MuTau_2018/total/mainCfg_MuTau_Legacy2018_limits.cfg
python write_card.py -f preparedHistos/analyzedOutPlotter_2018_ETau_23Apr2021.root   -o 2018_13May2021 -c ETau   -y 2018 -b 1 -u 1 -t -q 0 -B 1 -i ../analysis_2021_04_23/ETau_2018/total/mainCfg_ETau_Legacy2018_limits.cfg
