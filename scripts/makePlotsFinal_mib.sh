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
tag=13Nov2018_DYNLO

log=(--log)

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
baseline=baseline_dRs2

#baseline=s1b1jresolved
#baseline=s1b1jresolvedMcut

#baseline=s2b0jresolved
#baseline=s2b0jresolvedMcut

#baseline=sboostedLL
#baseline=sboostedLLMcut

#baseline=antiB
#baseline=antiB_dRg2
baseline=antiB_dRs2

#baseline=baselineVBF
#baseline=baselineNoVBF
#baseline=baselineVBFMcut

#baseline=baselineVBF_btagMfirst
#baseline=baselineVBF_btagM
#baseline=baselineVBF_btagM_bVetoM
#baseline=baselineVBF_L_btagMfirst
#baseline=baselineVBF_T_btagMfirst

others=""

mkdir plots_$channel
mkdir plots_$channel/$tag
mkdir plots_$channel/$tag/$baseline\_$reg


#BDToutSM_kl_1, BDToutLM_spin_0_mass_280, BDToutMM_spin_0_mass_400, BDToutHM_spin_0_mass_650
#VARS: dau1_pt, dau2_pt, met_et, met_phi, bjet1_pt, bjet1_eta, tauH_SVFIT_mass, bH_mass, tauH_mass, tauH_pt, bjet2_pt, bH_pt, p_zeta, BDT_MET_bH_cosTheta
#for i in `seq 0 1`; # 1:noLog  - 0:Log
for i in `seq 0 1`;
do
  #
  ## Hbb
  #
    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var bjet1_pt   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(b_{1}) [GeV]" --no-sig $others  --quit
    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var bjet2_pt   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(b_{2}) [GeV]" --no-sig $others --quit

    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var bjet1_eta  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta(bjet_{1})" --no-sig $others --quit
    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var bjet2_eta  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{bjet2}" --no-sig $others --quit

    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var bH_mass        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi  ${log[i]} --ratio  --tag $tag --label "m_{bb} [GeV]" --no-sig $others --quit
    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dib_dEtaSign   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi  ${log[i]} --ratio  --tag $tag --label "#eta_{bjet1}x#eta_{bjet2}" --no-sig $others --quit
    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dib_deltaR     --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi  ${log[i]} --ratio  --tag $tag --label "#DeltaR_{bb}" --no-sig $others --quit

  #
  ## Htautau
  #
  ##taus
    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau1_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(#tau_{1}) [GeV]" --no-sig $others --quit
    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau2_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(#tau_{2}) [GeV]" --no-sig $others --quit

    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau1_eta        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{#tau1}" --no-sig $others --quit
    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau2_eta        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{#tau2}" --no-sig $others --quit

    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau1_phi        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#phi_{#tau1}" --no-sig $others --quit
    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau2_phi        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#phi_{#tau2}" --no-sig $others --quit


    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau1_iso        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "Iso_{#tau1} [GeV]" --no-sig $others --quit
    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau2_iso        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "Iso_{#tau2} [GeV]" --no-sig $others --quit
    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau1_MVAisoNew  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "MVAisoNew_{#tau1} [GeV]" --no-sig $others --quit
    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau2_MVAisoNew  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "MVAisoNew_{#tau2} [GeV]" --no-sig $others --quit


    ####python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var tauH_SVFIT_mass --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{#tau#tau}^{SVfit} [GeV]" --no-sig $others --quit
    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var tauH_mass       --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{#tau#tau}^{vis} [GeV]" --no-sig $others --quit
    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var tauH_SVFIT_pt   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}^{SVfit}(H#tau#tau) [GeV]" --no-sig $others --quit

    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var tauH_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}^{Vis}(#tau#tau) [GeV]" --no-sig $others --quit
    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var ditau_deltaEta         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} -#-ratio  --tag $tag --label "#Delta#eta_{#tau#tau}" --no-sig $others --quit
    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var ditau_deltaPhi         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#Delta#phi_{#tau#tau}" --no-sig $others --quit
    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var ditau_deltaR         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#DeltaR_{#tau#tau}" --no-sig $others --quit

  #
  ## HH
  #
    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var HH_deltaR --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "#DeltaR_{HH}" --no-sig $others --quit
    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var HH_zV --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "Boson Centrality (HH)" --no-sig $others --quit
    ####python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var HH_mass_raw --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "m_{bb#tau#tau} [GeV]" --no-sig $others --quit
    ####python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var HH_pt --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "p_{T}(bb#tau#tau) [GeV]" --no-sig $others --quit
    ####python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var HHsvfit_mass --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "m_{bb#tau#tau}^{SVfit} [GeV]" --no-sig $others --quit
    ####python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var HHsvfit_pt --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "p_{T}^{SVfit}(bb#tau#tau) [GeV]" --no-sig $others --quit

  #
  ## VBF
  #
    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var VBFjj_dEtaSign --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "#eta_{VBFj11}#times#eta_{VBFj2}" --no-sig $others --quit
    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var VBFjj_deltaEta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "#Delta#eta_{jj}" --no-sig $others --quit

    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var VBFjj_mass  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "m_{jj}^{VBF} [GeV]" --no-sig $others --quit

    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var VBFjet1_pt  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "p_{TVBFjet1} [GeV]" --no-sig $others --quit
    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var VBFjet2_pt  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "p_{TVBFjet2} [GeV]" --no-sig $others --quit

    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var VBFjet1_eta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "#eta_{VBFjet1}" --no-sig $others --quit
    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var VBFjet2_eta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "#eta_{VBFjet2}" --no-sig $others --quit

    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var bH_VBF1_deltaEta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "#Delta#eta(H_{bb},VBFjet1)" --no-sig $others --quit
    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var VBFjet1_btag_deepCSV --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "DeepCSV_{VBFjet1}" --no-sig $others --quit
    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var VBFjet2_PUjetID --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "PUjetID_{VBFjet2}" --no-sig $others --quit

  #
  ##met
  #
  python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var met_et    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "MET [GeV]" --no-sig $others --quit
  python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var met_phi   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#phi_{MET}" --no-sig $others --quit

  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var met_er_et      --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "MET no HE [GeV]" --no-sig $others --quit

  #
  ## BDT angela
  #
  ####python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDT_MET_bH_cosTheta    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "cos#theta(bb, MET)" --no-sig $others --quit
  ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDT_HT20    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "HT [GeV]" --no-sig $others --quit
  ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var HHKin_mass_raw    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{HH}^{KinFit} [GeV]" --no-sig $others --blind-range 250 1000 --quit
  ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var MT2    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "MT2 [GeV]" --no-sig $others --blind-range 200 500 --quit
  python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var bH_pt    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(bb) [GeV]" --no-sig $others --quit
  ####python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var p_zeta    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "P_{#zeta}" --no-sig $others --quit

  #
  ## njets
  #
  python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var njets    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "njets" --no-sig $others --quit
  python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var npv    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "npv" --no-sig $others --quit

  #
  ## BDT OUTS - BDToutSM_kl_1, BDToutLM_spin_0_mass_280, BDToutMM_spin_0_mass_400, BDToutHM_spin_0_mass_650
  #
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDToutSM_kl_1    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "BDT score (#tau#tau, k_{#lambda}=1)" --no-sig $others --quit
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDToutLM_spin_0_mass_280    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "BDT score (#tau#tau, spin=0, m_{X}=280 GeV)" --no-sig $others --quit
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDToutMM_spin_0_mass_400    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "BDT score (#tau#tau, spin=0, m_{X}=400 GeV)" --no-sig $others --quit
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDToutHM_spin_0_mass_650    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "BDT score (#tau#tau, spin=0, m_{X}=650 GeV)" --no-sig $others --quit

  #
  ## RESULTS - MT2, HHKin_mass_raw, BDToutSM_kl_1, BDToutLM_spin_0_mass_280, BDToutMM_spin_0_mass_400, BDToutHM_spin_0_mass_650, , BDToutSM_kl_5
  #
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var MT2    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "MT2" --quit --no-data --sbs
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var HHKin_mass_raw    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "m_{HH}^{KinFit} [GeV]" --quit --no-data --sbs
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDToutSM_kl_1    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "BDT score (#tau#tau, k_{#lambda}=1)" --quit --no-data --sbs
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDToutSM_kl_5    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "BDT score (#tau#tau, k_{#lambda}=5)" --quit --no-data --sbs
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDToutSM_kl_2.45    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "BDT score (#tau#tau, k_{#lambda}=2.45)" --quit --no-data --sbs
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDToutSM_kl_0    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "BDT score (#tau#tau, k_{#lambda}=0)" --quit --no-data --sbs
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDToutSM_kl_30    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "BDT score (#tau#tau, k_{#lambda}=0)" --quit --no-data --sbs
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDToutLM_spin_0_mass_280    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "BDT score (#tau#tau, spin=0, m_{X}=280 GeV)" --quit --no-data --sbs
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDToutMM_spin_0_mass_400    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "BDT score (#tau#tau, spin=0, m_{X}=400 GeV)" --quit --no-data --sbs
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDToutHM_spin_0_mass_650    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "BDT score (#tau#tau, spin=0, m_{X}=650 GeV)" --quit --no-data --sbs
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var VBFjj_mass  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "m_{jj}^{VBF} [GeV]" --quit --no-data --sbs
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var VBFjj_deltaEta  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "#Delta#eta_{jj}" --quit --no-data --sbs

  #
  ## VBF
  #
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDToutVBF    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --tag $tag --label "VBF BDT score" --quit --no-data --sbs
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDToutVBF    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "VBF BDT score" --no-sig $others --quit


done


#cd plotsHH2017_$channel
#scp -r $tag/$baseline\_$reg camendol@lxplus.cern.ch://afs/cern.ch/user/c/camendol/www/HH2017/plots$channel/$tag
#cd ..





