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

# 10 Sept
#python prepare_histos.py -f ../analysis_TauTau_2016_10Sept2020_limits_V2/analyzedOutPlotter.root -o 10Sept_NS_V2 -c TauTau -y 2016
#python prepare_histos.py -f ../analysis_MuTau_2016_10Sept2020_limits_V2/analyzedOutPlotter.root -o 10Sept_NS_V2 -c MuTau -y 2016
#python prepare_histos.py -f ../analysis_ETau_2016_10Sept2020_limits_V2/analyzedOutPlotter.root -o 10Sept_NS_V2 -c ETau -y 2016

#python prepare_histos.py -f ../analysis_TauTau_2017_10Sept2020_limits_V2/analyzedOutPlotter.root -o 10Sept_NS_V2 -c TauTau -y 2017
#python prepare_histos.py -f ../analysis_MuTau_2017_10Sept2020_limits_V2/analyzedOutPlotter.root -o 10Sept_NS_V2 -c MuTau -y 2017
#python prepare_histos.py -f ../analysis_ETau_2017_10Sept2020_limits_V2/analyzedOutPlotter.root -o 10Sept_NS_V2 -c ETau -y 2017
#
#python prepare_histos.py -f ../analysis_TauTau_2018_10Sept2020_limits_V2/analyzedOutPlotter.root -o 10Sept_NS_V2 -c TauTau -y 2018
#python prepare_histos.py -f ../analysis_MuTau_2018_10Sept2020_limits_V2/analyzedOutPlotter.root -o 10Sept_NS_V2 -c MuTau -y 2018
#python prepare_histos.py -f ../analysis_ETau_2018_10Sept2020_limits_V2/analyzedOutPlotter.root -o 10Sept_NS_V2 -c ETau -y 2018


# 20 Sept
python compute_scales.py -y 2016
python compute_scales.py -y 2017
python compute_scales.py -y 2018

python prepare_histos.py -f ../analysis_TauTau_2016_22Sep2020_limits/analyzedOutPlotter.root -o 22Sep2020 -c TauTau -y 2016
python prepare_histos.py -f ../analysis_MuTau_2016_22Sep2020_limits/analyzedOutPlotter.root  -o 22Sep2020 -c MuTau  -y 2016
python prepare_histos.py -f ../analysis_ETau_2016_22Sep2020_limits/analyzedOutPlotter.root   -o 22Sep2020 -c ETau   -y 2016

python prepare_histos.py -f ../analysis_TauTau_2017_22Sep2020_limits/analyzedOutPlotter.root -o 22Sep2020 -c TauTau -y 2017
python prepare_histos.py -f ../analysis_MuTau_2017_22Sep2020_limits/analyzedOutPlotter.root  -o 22Sep2020 -c MuTau  -y 2017
python prepare_histos.py -f ../analysis_ETau_2017_22Sep2020_limits/analyzedOutPlotter.root   -o 22Sep2020 -c ETau   -y 2017

python prepare_histos.py -f ../analysis_TauTau_2018_22Sep2020_limits/analyzedOutPlotter.root -o 22Sep2020 -c TauTau -y 2018
python prepare_histos.py -f ../analysis_MuTau_2018_22Sep2020_limits/analyzedOutPlotter.root  -o 22Sep2020 -c MuTau  -y 2018
python prepare_histos.py -f ../analysis_ETau_2018_22Sep2020_limits/analyzedOutPlotter.root   -o 22Sep2020 -c ETau   -y 2018
