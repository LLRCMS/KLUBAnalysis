export PYTHONPATH=$PWD/../physicsModels:$PYTHONPATH

### Most useful options:
# -f : input root file
# -i : input cfg
# -o : output name
# -c : channel
# -y : year
# -b : bin-by-bin unc
# -u : shape unc
# -t : theory unc
#python write_card.py -f analyzedOutPlotter_TauTau_prova.root -o prova -c TauTau -y 2018 -b 1 -u 1 -t -i ../analysis_TauTau_synch_11June2020_limits_prova/mainCfg_TauTau_Legacy2018_limits.cfg


python write_card.py -f analyzedOutPlotter_2016_TauTau_22Sep2020.root -o 2016_22Sep2020 -c TauTau -y 2016 -u 1 -t -i ../../analysis_TauTau_2016_22Sep2020_limits_VBF/mainCfg_TauTau_Legacy2016_limits.cfg
python write_card.py -f analyzedOutPlotter_2016_MuTau_22Sep2020.root  -o 2016_22Sep2020 -c MuTau  -y 2016 -u 1 -t -i ../../analysis_MuTau_2016_22Sep2020_limits_VBF/mainCfg_MuTau_Legacy2016_limits.cfg
python write_card.py -f analyzedOutPlotter_2016_ETau_22Sep2020.root   -o 2016_22Sep2020 -c ETau   -y 2016 -u 1 -t -i ../../analysis_ETau_2016_22Sep2020_limits_VBF/mainCfg_ETau_Legacy2016_limits.cfg

python write_card.py -f analyzedOutPlotter_2017_TauTau_22Sep2020.root -o 2017_22Sep2020 -c TauTau -y 2017 -u 1 -t -i ../../analysis_TauTau_2017_22Sep2020_limits_VBF/mainCfg_TauTau_Legacy2017_limits.cfg
python write_card.py -f analyzedOutPlotter_2017_MuTau_22Sep2020.root  -o 2017_22Sep2020 -c MuTau  -y 2017 -u 1 -t -i ../../analysis_MuTau_2017_22Sep2020_limits_VBF/mainCfg_MuTau_Legacy2017_limits.cfg
python write_card.py -f analyzedOutPlotter_2017_ETau_22Sep2020.root   -o 2017_22Sep2020 -c ETau   -y 2017 -u 1 -t -i ../../analysis_ETau_2017_22Sep2020_limits_VBF/mainCfg_ETau_Legacy2017_limits.cfg
