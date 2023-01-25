### Most useful options:
# -f : input root file
# -i : input cfg
# -o : output name
# -c : channel
# -y : year
# -b : bin-by-bin unc
# -u : shape unc
# -t : theory unc
# -r : 1 for resonant analysis setup
#python write_card.py -f analyzedOutPlotter_TauTau_prova.root -o prova -c TauTau -y 2018 -b 1 -u 1 -t -i ../analysis_TauTau_synch_11June2020_limits_prova/mainCfg_TauTau_Legacy2018_limits.cfg


HISTDIR_TAUTAU="/data_CMS/cms/liugeliang/HHbbtautau_histos/Legacy2018_11Oct2021/TauTau/"
HISTDIR_ETAU="/data_CMS/cms/liugeliang/HHbbtautau_histos/Legacy2018_11Oct2021/ETau/"
HISTDIR_MUTAU="/data_CMS/cms/liugeliang/HHbbtautau_histos/Legacy2018_11Oct2021/MuTau/"

CFG_DIR="/home/llr/cms/liugeliang/HH_bbtautau/CMSSW_11_1_0_pre6/src/KLUBAnalysis/config/"
LIMIT_DIR="/data_CMS/cms/liugeliang/HHbbtautau_limits/"

python ${LIMIT_DIR}write_res_card.py -f ${HISTDIR_TAUTAU}analyzedOutPlotter.root -o Legacy2018_11Oct2021 -c TauTau -y 2018 -b 0 -u 0 -i ${CFG_DIR}mainCfg_TauTau_Legacy2018.cfg -r 1
python ${LIMIT_DIR}write_res_card.py -f ${HISTDIR_MUTAU}analyzedOutPlotter.root  -o Legacy2018_11Oct2021 -c MuTau  -y 2018 -b 0 -u 0 -i ${CFG_DIR}mainCfg_MuTau_Legacy2018.cfg -r 1
python ${LIMIT_DIR}write_res_card.py -f ${HISTDIR_ETAU}analyzedOutPlotter.root   -o Legacy2018_11Oct2021 -c ETau   -y 2018 -b 0 -u 0 -i ${CFG_DIR}mainCfg_ETau_Legacy2018.cfg -r 1
