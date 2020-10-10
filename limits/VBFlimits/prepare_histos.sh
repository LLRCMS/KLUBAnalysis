############
# USAGE:
#
# 1 - prepare files with yield variations --> REMEMBER to modify hardcoded strings (channel, folder...)
# python compute_scales.py -y 2018
#
# 2 - prepare histograms with systematics
# python prepare_histos.py -f ../analysis_TauTau_synch_11June2020_limits_prova/analyzedOutPlotter.root -o prova -c TauTau -y 2018
#
############

python prepare_histos.py -f ../../analysis_TauTau_2016_22Sep2020_limits_VBF/analyzedOutPlotter.root -o 22Sep2020 -c TauTau -y 2016
python prepare_histos.py -f ../../analysis_MuTau_2016_22Sep2020_limits_VBF/analyzedOutPlotter.root  -o 22Sep2020 -c MuTau  -y 2016
python prepare_histos.py -f ../../analysis_ETau_2016_22Sep2020_limits_VBF/analyzedOutPlotter.root   -o 22Sep2020 -c ETau   -y 2016

python prepare_histos.py -f ../../analysis_TauTau_2017_22Sep2020_limits_VBF/analyzedOutPlotter.root -o 22Sep2020 -c TauTau -y 2017
python prepare_histos.py -f ../../analysis_MuTau_2017_22Sep2020_limits_VBF/analyzedOutPlotter.root  -o 22Sep2020 -c MuTau  -y 2017
python prepare_histos.py -f ../../analysis_ETau_2017_22Sep2020_limits_VBF/analyzedOutPlotter.root   -o 22Sep2020 -c ETau   -y 2017
