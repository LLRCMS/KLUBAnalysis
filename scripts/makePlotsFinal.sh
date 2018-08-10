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
tag=8Aug2018

log=(--log)

channel=TauTau
#channel=MuTau
#channel=ETau

#lumi=13.4
lumi=41.6
reg=SR  # A:SR , B:SStight , C:OSinviso, D:SSinviso, B': SSrlx

#baseline=baseline
##baseline=baselineMassCut

#baseline=s1b1jresolved
#baseline=s1b1jresolvedMcut

#baseline=s2b0jresolved
#baseline=s2b0jresolvedMcut

#baseline=sboostedLL
#baseline=sboostedLLMcut

baseline=antiB

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

#for i in `seq 0 1`; # 1:noLog  - 0:Log
for i in `seq 0 1`;
do
  #
  ## Hbb
  #
    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var bjet1_pt   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{Tbjet1} [GeV]" --no-sig $others  --quit
    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var bjet2_pt   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{Tbjet2} [GeV]" --no-sig $others --quit

    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var bjet1_eta  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{bjet1}" --no-sig $others --quit
    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var bjet2_eta  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{bjet2}" --no-sig $others --quit

    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var bH_mass        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi  ${log[i]} --ratio  --tag $tag --label "m_{bb} [GeV]" --no-sig $others --quit
    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dib_dEtaSign   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi  ${log[i]} --ratio  --tag $tag --label "#eta_{bjet1}x#eta_{bjet2}" --no-sig $others --quit
    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dib_deltaR     --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi  ${log[i]} --ratio  --tag $tag --label "#DeltaR_{bb}" --no-sig $others --quit

  #
  ## Htautau
  #
  ##taus
    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau1_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T#tau1} [GeV]" --no-sig $others --quit
    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau2_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T#tau2} [GeV]" --no-sig $others --quit

    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau1_eta        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{#tau1}" --no-sig $others --quit
    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau2_eta        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{#tau2}" --no-sig $others --quit

    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau1_phi        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#phi_{#tau1}" --no-sig $others --quit
    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau2_phi        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#phi_{#tau2}" --no-sig $others --quit


    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau1_iso        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "Iso_{#tau1} [GeV]" --no-sig $others --quit
    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau2_iso        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "Iso_{#tau2} [GeV]" --no-sig $others --quit
    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau1_MVAisoNew  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "MVAisoNew_{#tau1} [GeV]" --no-sig $others --quit
    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau2_MVAisoNew  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "MVAisoNew_{#tau2} [GeV]" --no-sig $others --quit


    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var tauH_SVFIT_mass --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{#tau#tau}^{SVfit} [GeV]" --no-sig $others --quit
    python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var tauH_mass       --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{#tau#tau}^{vis} [GeV]" --no-sig $others --quit

    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var tauH_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{TH#tau#tau}^{Vis} [GeV]" --no-sig $others --quit
    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var ditau_deltaEta         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} -#-ratio  --tag $tag --label "#Delta#eta_{#tau#tau}" --no-sig $others --quit
    #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var ditau_deltaPhi         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#Delta#phi_{#tau#tau}" --no-sig $others --quit
    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var ditau_deltaR         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#DeltaR_{#tau#tau}" --no-sig $others --quit

  #
  ## HH
  #
    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var HH_deltaR --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "#DeltaR_{HH}" --no-sig $others --quit
    ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var HH_zV --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "Boson Centrality (HH)" --no-sig $others --quit

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
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var met_et    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "MET [GeV]" --no-sig $others --quit
  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var met_phi   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#phi_{MET}" --no-sig $others --quit

  #python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var met_er_et      --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "MET no HE [GeV]" --no-sig $others --quit

  #
  ## BDT angela
  #
  ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDT_MET_bH_cosTheta    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "cos#theta(H_{bb}, MET)" --no-sig $others --quit
  ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var BDT_HT20    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "HT [GeV]" --no-sig $others --quit
  ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var HHKin_mass_raw    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{HH}^{KinFit} [GeV]" --no-sig $others --blind-range 250 1000 --quit
  ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var MT2    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "MT2 [GeV]" --no-sig $others --blind-range 200 500 --quit
  ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var bH_pt    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{TH_{bb}} [GeV]" --no-sig $others --quit
  ##python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var p_zeta    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "P_{#zeta}" --no-sig $others --quit


  ## njets
  python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var njets    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "njets" --no-sig $others --quit

done


#cd plotsHH2017_$channel
#scp -r $tag/$baseline\_$reg camendol@lxplus.cern.ch://afs/cern.ch/user/c/camendol/www/HH2017/plots$channel/$tag
#cd ..





