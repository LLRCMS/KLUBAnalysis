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
#python write_card.py -f analyzedOutPlotter_TauTau_prova.root -o prova -c TauTau -y 2018 -b 1 -u 1 -t -i ../analysis_TauTau_synch_11June2020_limits_prova/mainCfg_TauTau_Legacy2018_limits.cfg


#python write_card.py -f analyzedOutPlotter_2016_TauTau_14Sept_NS.root -o 2016_14Sept_NS_V2 -c TauTau -y 2016 -u 1 -t -i ../../analysis_TauTau_2016_14Sept2020_limits_VBF/mainCfg_TauTau_Legacy2016_limits.cfg
#python write_card.py -f analyzedOutPlotter_2016_TauTau_14Sept_NS.root -o 2016_14Sept_NS_V3 -c TauTau -y 2016 -u 1 -t -i ../../analysis_TauTau_2016_14Sept2020_limits_VBF/mainCfg_TauTau_Legacy2016_limits.cfg
python write_card.py -f analyzedOutPlotter_2016_TauTau_14Sept_NS.root -o 2016_14Sept_NS_V4 -c TauTau -y 2016 -u 1 -t -i ../../analysis_TauTau_2016_14Sept2020_limits_VBF/mainCfg_TauTau_Legacy2016_limits.cfg
