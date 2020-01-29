# Tag
tag=27Jan2020_DMs_tauIDSF

# Options to launch
# --ch    : channels
# --tag   : input directory tag
# --decay : decayMode, for example "bothDM1" means (tau1_DM = 1 && tau2_DM = 1); to use mixed bins use e.g. "DM1_DM0"
# --sel   : selection name, the selection that you defined in mainCfg
# --qcd   : QCD already computed: if True it will use the QCD already stored in analyzedOutplotter.root, thus it will produce only
#           one single datacard_QCD.txt of the signal region; if False it creates 4 datacards A, B, C and D and combines them in combined.txt,
#           in this way it can do the fit while changing simultaneously the QCD

python makeCards_Legacy2016.py --tag $tag --ch TauTau --decay bothDM0 --sel baselineHTauTau_bothDM0_DY
python makeCards_Legacy2016.py --tag $tag --ch TauTau --decay bothDM1 --sel baselineHTauTau_bothDM1_DY
python makeCards_Legacy2016.py --tag $tag --ch TauTau --decay bothDM10 --sel baselineHTauTau_bothDM10_DY
python makeCards_Legacy2016.py --tag $tag --ch TauTau --decay bothDM11 --sel baselineHTauTau_bothDM11_DY
