#tag=11Jul2018
#tag=14Jul2018
#tag=14Jul2018_TEST
#tag=14Jul2018_SStight
#tag=14Jul2018_VBF
#tag=14Jul2018_style
#tag=19Jul2018
#tag=20Jul2018
#tag=25Jul2018_reduced_PU
#tag=25Jul2018_newPU
#tag=25Jul2018_newPU_noSF
#tag=27Jul2018_pt45
#tag=27Jul2018_pt45_noSF
#tag=28Jul2018_newSF
#tag=30Jul2018_newPU_oldSF
#tag=8Aug2018
#tag=19Oct2018_DY_Zpt_Zmass
#tag=25Oct2018_TESI
#tag=25Oct2018_TESI_noNLO
#tag=25Oct2018_TESI_NLO
#tag=25Oct2018_TESI_NLO_BDTouts
#tag=25Oct2018_TESI_BDTouts
#tag=25Oct2018_TESI_VBF
#tag=25Oct2018_TESI_ggH
#tag=13Nov2018
#tag=13Nov2018_T
#tag=13Nov2018_DYNLO
#tag=29Nov2018
#tag=29Nov2018_tauID92
#tag=3Dec2018_tauID92
#tag=3Dec2018
#tag=3Dec2018_tauID1
#tag=3Dec2018_tauID1_DYNLO
#tag=3Dec2018_noTrg
#tag=3Dec2018_noSF
#tag=3Dec2018_noSF_DYNLO
#tag=3Dec2018_noTrg_DYNLO
#tag=3Dec2018_DYNLO
#tag=4Dec2018
#tag=4Dec2018_inclusive
#tag=3Dec2018_splitting
#tag=11Dec2018_nosplit   #baseline, baseline_dRs2, s1b1jresolved, s1b1jresolved_dRs2, antiB, antiB_dRs2
#tag=11Dec2018_splitting #baseline, baseline_dRs2, s1b1jresolved, s1b1jresolved_dRs2, antiB, antiB_dRs2
#tag=11Dec2018_DYNLO_WJetsTest
#tag=13Dec2018_DYNLO_newSF
tag=13Dec2018_newSF_NoMass

log=(--log)

#plotter=makeFinalPlots.py
plotter=makeFinalPlots_split.py

channel=TauTau
#channel=MuTau
#channel=ETau
#channel=MuMu

#lumi=13.4
lumi=41.6
reg=SR  # A:SR , B:SStight , C:OSinviso, D:SSinviso, B': SSrlx

#baseline=baseline
#baseline=baselineMassCut
#baseline=antiB_noQCD
#baseline6060_noQCD, baseline6060, antiB_noQCD, antiB6060_noQCD
#baseline=baseline_dRg2
#baseline=baseline_dRs2
#baseline=baselineNoMass
#baseline=baselineNoMass_dRs2


#baseline=s1b1jresolved
#baseline=s1b1jresolvedMcut
#baseline=s1b1jresolved_dRs2
#baseline=s1b1jresolvedNoMass
#baseline=s1b1jresolvedNoMass_dRs2

#baseline=s2b0jresolved
#baseline=s2b0jresolvedMcut

#baseline=sboostedLL
#baseline=sboostedLLMcut

#baseline=antiB
#baseline=antiB_dRg2
#baseline=antiB_dRs2
#baseline=antiB_dRg2_4045
#baseline=antiB_dRg2_4540
#baseline=antiB_dRg2_4545
#baseline=antiBNoMass
baseline=antiBNoMass_dRs2

#baseline=baselineVBF
#baseline=baselineNoVBF
#baseline=baselineVBFMcut

#baseline=baselineVBF_btagMfirst
#baseline=baselineVBF_btagM
#baseline=baselineVBF_btagM_bVetoM
#baseline=baselineVBF_L_btagMfirst
#baseline=baselineVBF_T_btagMfirst


others=""

#sigDrawer=--no-sig
sigDrawer=""

mkdir plots_$channel
mkdir plots_$channel/$tag
mkdir plots_$channel/$tag/$baseline\_$reg

#BDToutSM_kl_1, BDToutLM_spin_0_mass_280, BDToutMM_spin_0_mass_400, BDToutHM_spin_0_mass_650
#VARS: dau1_pt, dau2_pt, met_et, met_phi, bjet1_pt, bjet1_eta, tauH_SVFIT_mass, bH_mass, tauH_mass, tauH_pt, bjet2_pt, bH_pt, p_zeta, BDT_MET_bH_cosTheta
#for i in `seq 0 1`; # 1:noLog  - 0:Log
for i in `seq 1`;
do
  #
  ## Hbb
  #
    python scripts/$plotter --dir analysis_$channel\_$tag --var bjet1_pt   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(b_{1}) [GeV]" $sigDrawer $others  --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var bjet2_pt   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(b_{2}) [GeV]" $sigDrawer $others --quit

    python scripts/$plotter --dir analysis_$channel\_$tag --var bjet1_eta  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta(bjet_{1})" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var bjet2_eta  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{bjet2}" $sigDrawer $others --quit

    python scripts/$plotter --dir analysis_$channel\_$tag --var bH_mass        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi  ${log[i]} --ratio  --tag $tag --label "m_{bb} [GeV]" $sigDrawer $others --quit
    ##python scripts/$plotter --dir analysis_$channel\_$tag --var dib_dEtaSign   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi  ${log[i]} --ratio  --tag $tag --label "#eta_{bjet1}x#eta_{bjet2}" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var dib_deltaR     --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi  ${log[i]} --ratio  --tag $tag --label "#DeltaR_{bb}" $sigDrawer $others --quit

  #
  ## Htautau
  #
  ##taus
    python scripts/$plotter --dir analysis_$channel\_$tag --var dau1_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(#tau_{1}) [GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var dau2_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(#tau_{2}) [GeV]" $sigDrawer $others --quit

    python scripts/$plotter --dir analysis_$channel\_$tag --var dau1_eta        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{#tau1}" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var dau2_eta        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{#tau2}" $sigDrawer $others --quit

    python scripts/$plotter --dir analysis_$channel\_$tag --var dau1_phi        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#phi_{#tau1}" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var dau2_phi        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#phi_{#tau2}" $sigDrawer $others --quit


    #python scripts/$plotter --dir analysis_$channel\_$tag --var dau1_iso        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "Iso_{#tau1} [GeV]" $sigDrawer $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var dau2_iso        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "Iso_{#tau2} [GeV]" $sigDrawer $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var dau1_MVAisoNew  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "MVAisoNew_{#tau1} [GeV]" $sigDrawer $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var dau2_MVAisoNew  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "MVAisoNew_{#tau2} [GeV]" $sigDrawer $others --quit


    ####python scripts/$plotter --dir analysis_$channel\_$tag --var tauH_SVFIT_mass --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{#tau#tau}^{SVfit} [GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var tauH_mass       --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{#tau#tau}^{vis} [GeV]" $sigDrawer $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var tauH_SVFIT_pt   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}^{SVfit}(H#tau#tau) [GeV]" $sigDrawer $others --quit

    python scripts/$plotter --dir analysis_$channel\_$tag --var tauH_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}^{Vis}(#tau#tau) [GeV]" $sigDrawer $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var ditau_deltaEta         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} -#-ratio  --tag $tag --label "#Delta#eta_{#tau#tau}" $sigDrawer $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var ditau_deltaPhi         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#Delta#phi_{#tau#tau}" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var ditau_deltaR         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#DeltaR_{#tau#tau}" $sigDrawer $others --quit

  #
  ## HH
  #
    python scripts/$plotter --dir analysis_$channel\_$tag --var HH_deltaR --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "#DeltaR_{HH}" $sigDrawer $others --quit
    ##python scripts/$plotter --dir analysis_$channel\_$tag --var HH_zV --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "Boson Centrality (HH)" $sigDrawer $others --quit
    ####python scripts/$plotter --dir analysis_$channel\_$tag --var HH_mass_raw --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "m_{bb#tau#tau} [GeV]" $sigDrawer $others --quit
    ####python scripts/$plotter --dir analysis_$channel\_$tag --var HH_pt --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "p_{T}(bb#tau#tau) [GeV]" $sigDrawer $others --quit
    ####python scripts/$plotter --dir analysis_$channel\_$tag --var HHsvfit_mass --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "m_{bb#tau#tau}^{SVfit} [GeV]" $sigDrawer $others --quit
    ####python scripts/$plotter --dir analysis_$channel\_$tag --var HHsvfit_pt --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "p_{T}^{SVfit}(bb#tau#tau) [GeV]" $sigDrawer $others --quit

  #
  ## VBF
  #
    ##python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjj_dEtaSign --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "#eta_{VBFj11}#times#eta_{VBFj2}" $sigDrawer $others --quit
    ##python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjj_deltaEta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "#Delta#eta_{jj}" $sigDrawer $others --quit

    ##python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjj_mass  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "m_{jj}^{VBF} [GeV]" $sigDrawer $others --quit

    ##python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjet1_pt  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "p_{TVBFjet1} [GeV]" $sigDrawer $others --quit
    ##python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjet2_pt  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "p_{TVBFjet2} [GeV]" $sigDrawer $others --quit

    #python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjet1_eta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "#eta_{VBFjet1}" $sigDrawer $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjet2_eta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "#eta_{VBFjet2}" $sigDrawer $others --quit

    #python scripts/$plotter --dir analysis_$channel\_$tag --var bH_VBF1_deltaEta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "#Delta#eta(H_{bb},VBFjet1)" $sigDrawer $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjet1_btag_deepCSV --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "DeepCSV_{VBFjet1}" $sigDrawer $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjet2_PUjetID --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "PUjetID_{VBFjet2}" $sigDrawer $others --quit


  #
  ## additional jets
  #
  python scripts/$plotter --dir analysis_$channel\_$tag --var jet3_eta  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta(jet_{3})" $sigDrawer $others --quit
  python scripts/$plotter --dir analysis_$channel\_$tag --var jet4_eta  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{jet_{4}}" $sigDrawer $others --quit

  #
  ##met
  #
  python scripts/$plotter --dir analysis_$channel\_$tag --var met_et    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "MET [GeV]" $sigDrawer $others --quit
  python scripts/$plotter --dir analysis_$channel\_$tag --var met_phi   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#phi_{MET}" $sigDrawer $others --quit

  #python scripts/$plotter --dir analysis_$channel\_$tag --var met_er_et      --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "MET no HE [GeV]" $sigDrawer $others --quit

  #
  ## BDT angela
  #
  ####python scripts/$plotter --dir analysis_$channel\_$tag --var BDT_MET_bH_cosTheta    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "cos#theta(bb, MET)" $sigDrawer $others --quit
  ##python scripts/$plotter --dir analysis_$channel\_$tag --var BDT_HT20    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "HT [GeV]" $sigDrawer $others --quit
  ##python scripts/$plotter --dir analysis_$channel\_$tag --var HHKin_mass_raw    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{HH}^{KinFit} [GeV]" $sigDrawer $others --blind-range 250 1000 --quit
  ##python scripts/$plotter --dir analysis_$channel\_$tag --var MT2    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "MT2 [GeV]" $sigDrawer $others --blind-range 200 500 --quit
  python scripts/$plotter --dir analysis_$channel\_$tag --var bH_pt    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(bb) [GeV]" $sigDrawer $others --quit
  ####python scripts/$plotter --dir analysis_$channel\_$tag --var p_zeta    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "P_{#zeta}" $sigDrawer $others --quit
  python scripts/$plotter --dir analysis_$channel\_$tag --var BDT_topPairMasses    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{topPair}" $sigDrawer $others --quit

  #
  ## njets
  #
  python scripts/$plotter --dir analysis_$channel\_$tag --var njets    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "njets" $sigDrawer $others --quit
  python scripts/$plotter --dir analysis_$channel\_$tag --var npv    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "npv" $sigDrawer $others --quit

  #
  ## BDT OUTS - BDToutSM_kl_1, BDToutLM_spin_0_mass_280, BDToutMM_spin_0_mass_400, BDToutHM_spin_0_mass_650
  #
  #python scripts/$plotter --dir analysis_$channel\_$tag --var BDToutSM_kl_1    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "BDT score (#tau#tau, k_{#lambda}=1)" $sigDrawer $others --quit
  #python scripts/$plotter --dir analysis_$channel\_$tag --var BDToutLM_spin_0_mass_280    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "BDT score (#tau#tau, spin=0, m_{X}=280 GeV)" $sigDrawer $others --quit
  #python scripts/$plotter --dir analysis_$channel\_$tag --var BDToutMM_spin_0_mass_400    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "BDT score (#tau#tau, spin=0, m_{X}=400 GeV)" $sigDrawer $others --quit
  #python scripts/$plotter --dir analysis_$channel\_$tag --var BDToutHM_spin_0_mass_650    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "BDT score (#tau#tau, spin=0, m_{X}=650 GeV)" $sigDrawer $others --quit

  #
  ## RESULTS - MT2, HHKin_mass_raw, BDToutSM_kl_1, BDToutLM_spin_0_mass_280, BDToutMM_spin_0_mass_400, BDToutHM_spin_0_mass_650, , BDToutSM_kl_5
  #
  #python scripts/$plotter --dir analysis_$channel\_$tag --var MT2    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "MT2" --quit --no-data --sbs
  #python scripts/$plotter --dir analysis_$channel\_$tag --var HHKin_mass_raw    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "m_{HH}^{KinFit} [GeV]" --quit --no-data --sbs
  #python scripts/$plotter --dir analysis_$channel\_$tag --var BDToutSM_kl_1    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "BDT score (#tau#tau, k_{#lambda}=1)" --quit --no-data --sbs
  #python scripts/$plotter --dir analysis_$channel\_$tag --var BDToutSM_kl_5    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "BDT score (#tau#tau, k_{#lambda}=5)" --quit --no-data --sbs
  #python scripts/$plotter --dir analysis_$channel\_$tag --var BDToutSM_kl_2.45    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "BDT score (#tau#tau, k_{#lambda}=2.45)" --quit --no-data --sbs
  #python scripts/$plotter --dir analysis_$channel\_$tag --var BDToutSM_kl_0    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "BDT score (#tau#tau, k_{#lambda}=0)" --quit --no-data --sbs
  #python scripts/$plotter --dir analysis_$channel\_$tag --var BDToutSM_kl_30    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "BDT score (#tau#tau, k_{#lambda}=0)" --quit --no-data --sbs
  #python scripts/$plotter --dir analysis_$channel\_$tag --var BDToutLM_spin_0_mass_280    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "BDT score (#tau#tau, spin=0, m_{X}=280 GeV)" --quit --no-data --sbs
  #python scripts/$plotter --dir analysis_$channel\_$tag --var BDToutMM_spin_0_mass_400    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "BDT score (#tau#tau, spin=0, m_{X}=400 GeV)" --quit --no-data --sbs
  #python scripts/$plotter --dir analysis_$channel\_$tag --var BDToutHM_spin_0_mass_650    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "BDT score (#tau#tau, spin=0, m_{X}=650 GeV)" --quit --no-data --sbs
  #python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjj_mass  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "m_{jj}^{VBF} [GeV]" --quit --no-data --sbs
  #python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjj_deltaEta  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "#Delta#eta_{jj}" --quit --no-data --sbs

  #
  ## VBF
  #
  #python scripts/$plotter --dir analysis_$channel\_$tag --var BDToutVBF    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "VBF BDT score" --quit --no-data --sbs
  #python scripts/$plotter --dir analysis_$channel\_$tag --var BDToutVBF    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "VBF BDT score" $sigDrawer $others --quit


done


#cd plotsHH2017_$channel
#scp -r $tag/$baseline\_$reg camendol@lxplus.cern.ch://afs/cern.ch/user/c/camendol/www/HH2017/plots$channel/$tag
#cd ..





