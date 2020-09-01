# Two steps for this:

# 1 - prepare files with yield variations --> REMEMBER to modify hardcoded strings (channel, folder...)
python compute_scales.py -y 2018

# 2 - prepare histograms with systematics
python prepare_histos.py -f ../analysis_TauTau_synch_11June2020_limits_prova/analyzedOutPlotter.root -o prova -c TauTau -y 2018
