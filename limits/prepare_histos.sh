input='../analysis_TauTau_synch_11June2020_limits_prova'

# prepare files with yield variations
python compute_scales.py
# prepare histograms with systematics 
python prepare_histos.py -f ../analysis_TauTau_synch_11June2020_limits_prova/analyzedOutPlotter.root -o prova -c TauTau
