#tag=7Feb2019_decaymodes_noSF_tightDY
#tag=28Jan2019_decaymodes_bins_DYenriched
#tag=8Mar2019_decaymodes_DYbins_DYenriched_noIDSF
#tag=11Mar2019_decaymodes_DYbins_oldDYWeight_m55_noIDSF
#tag=11Mar2019_decaymodes_DYbins_oldDYWeight_FakeJetQCD_noIDSF
#tag=21Mar2019_decaymodes_DYbins_oldDYWeight_DYenriched_invABCD_QCDFT_noSF
#tag=30Apr2019_decaymodes_nopu_mtt_DYenr_notauID
#tag=30Apr2019_decaymodes_DYenr_loosepu_mh_noID
#tag=30Apr2019_decaymodes_loosepu_mtt_DYenr_notauID
#tag=4May2019_decaymodes_DYLO_loosepu_mtt_notauID
tag=22Jan2020_DMs_tauIDSF_inter
####TauTau channel
### same decay mode
#taking the QCD already computed, without making the fit
#python makeCards.py --tag $tag --ch TauTau --decay bothDM0 --sel baselineHTauTau_bothDM0_DY --qcd True
#python makeCards.py --tag $tag --ch TauTau --decay bothDM1 --sel baselineHTauTau_bothDM1_DY --qcd True
#python makeCards.py --tag $tag --ch TauTau --decay bothDM10 --sel baselineHTauTau_bothDM10_DY --qcd True
#### output is dataset_QCD.txt
#
#computing also the QCD
python makeCards.py --tag $tag --ch TauTau --decay bothDM0 --sel baselineHTauTau_bothDM0_DY
python makeCards.py --tag $tag --ch TauTau --decay bothDM1 --sel baselineHTauTau_bothDM1_DY
python makeCards.py --tag $tag --ch TauTau --decay bothDM10 --sel baselineHTauTau_bothDM10_DY
python makeCards.py --tag $tag --ch TauTau --decay bothDM11 --sel baselineHTauTau_bothDM11_DY
#### output is combined.txt
#
#
#
#### different decay modes
##taking the QCD already computed, without making the fit
#python makeCards.py --tag $tag --ch TauTau --decay cross_DM0_DM1 --sel baselineHTauTau_tau1_DM0_tau2_DM1_DY --qcd True
#python makeCards.py --tag $tag --ch TauTau --decay cross_DM1_DM0 --sel baselineHTauTau_tau1_DM1_tau2_DM0_DY --qcd True
##
#python makeCards.py --tag $tag --ch TauTau --decay cross_DM0_DM10 --sel baselineHTauTau_tau1_DM0_tau2_DM10_DY --qcd True
#python makeCards.py --tag $tag --ch TauTau --decay cross_DM10_DM0 --sel baselineHTauTau_tau1_DM10_tau2_DM0_DY --qcd True
##
#python makeCards.py --tag $tag --ch TauTau --decay cross_DM1_DM10 --sel baselineHTauTau_tau1_DM1_tau2_DM10_DY --qcd True
#python makeCards.py --tag $tag --ch TauTau --decay cross_DM10_DM1 --sel baselineHTauTau_tau1_DM10_tau2_DM1_DY --qcd True
##### output is dataset_QCD.txt
##

#tag=27Mar2019_decaymodes_DYbins_oldDYWeight_DYenriched_noIDSF
#tag=4May2019_decaymodes_DYLO_DYenr_notauIDSF
###MuTau channel
### all triggers
##taking the QCD already computed, without making the fit
#python makeCards.py --tag $tag --ch MuTau --decay bothDM0 --sel baselineHTauTau_DM0_DY --qcd True
#python makeCards.py --tag $tag --ch MuTau --decay bothDM1 --sel baselineHTauTau_DM1_DY --qcd True
#python makeCards.py --tag $tag --ch MuTau --decay bothDM10 --sel baselineHTauTau_DM10_DY --qcd True
##### output is dataset_QCD.txt
##
###computing also the QCD
#python makeCards.py --tag $tag --ch MuTau --decay bothDM0 --sel baselineHTauTau_DM0_DY
#python makeCards.py --tag $tag --ch MuTau --decay bothDM1 --sel baselineHTauTau_DM1_DY
#python makeCards.py --tag $tag --ch MuTau --decay bothDM10 --sel baselineHTauTau_DM10_DY
##### output is combined.txt
#
##taking the QCD already computed, without making the fit
#python makeCards.py --tag $tag --ch MuTau --decay bothDM0 --sel baselineHTauTau_DM0_single_DY --qcd True
#python makeCards.py --tag $tag --ch MuTau --decay bothDM1 --sel baselineHTauTau_DM1_single_DY --qcd True
#python makeCards.py --tag $tag --ch MuTau --decay bothDM10 --sel baselineHTauTau_DM10_single_DY --qcd True
#### output is dataset_QCD.txt
#
##computing also the QCD
#python makeCards.py --tag $tag --ch MuTau --decay bothDM0 --sel baselineHTauTau_DM0_single_DY
#python makeCards.py --tag $tag --ch MuTau --decay bothDM1 --sel baselineHTauTau_DM1_single_DY
#python makeCards.py --tag $tag --ch MuTau --decay bothDM10 --sel baselineHTauTau_DM10_single_DY
#### output is combined.txt
#
##taking the QCD already computed, without making the fit
#python makeCards.py --tag $tag --ch MuTau --decay bothDM0 --sel baselineHTauTau_DM0_cross_DY --qcd True
#python makeCards.py --tag $tag --ch MuTau --decay bothDM1 --sel baselineHTauTau_DM1_cross_DY --qcd True
#python makeCards.py --tag $tag --ch MuTau --decay bothDM10 --sel baselineHTauTau_DM10_cross_DY --qcd True
#### output is dataset_QCD.txt
#
##computing also the QCD
#python makeCards.py --tag $tag --ch MuTau --decay bothDM0 --sel baselineHTauTau_DM0_cross_DY
#python makeCards.py --tag $tag --ch MuTau --decay bothDM1 --sel baselineHTauTau_DM1_cross_DY
#python makeCards.py --tag $tag --ch MuTau --decay bothDM10 --sel baselineHTauTau_DM10_cross_DY
#### output is combined.txt
