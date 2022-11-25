#tag=UL2016_5Apr2022
tag=UL2016_13Apr2022_newPostVFP

log=(--log)

plotter=makeFinalPlots_UL2016.py

channel=TauTau
#channel=MuTau
#channel=ETau
#channel=MuMu

lumi=16.8

reg=SR  # A:SR , B:SStight , C:OSinviso, D:SSinviso, B': SSrlx

baseline=baseline # baseline, baselineMcut, s1b1jresolvedMcut, s2b0jresolvedMcut, sboostedLLMcut, VBFloose, baseline_noQCD, baseline_MTT, VBFclass

others=""

sigDrawer=--no-sig

mkdir plots_$channel
mkdir plots_$channel/$tag
mkdir plots_$channel/$tag/$baseline\_$reg

#for i in `seq 0 1`; # 1:noLog  - 0:Log
for i in `seq 1`;
do
  #
  ## Hbb
  #
    python scripts/$plotter --dir analysis_$channel\_$tag --var bjet1_pt  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "p_{T}(b_{1}) [GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var bjet2_pt  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "p_{T}(b_{2}) [GeV]" $sigDrawer $others --quit

    python scripts/$plotter --dir analysis_$channel\_$tag --var bjet1_eta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "#eta(b_{1})" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var bjet2_eta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "#eta(b_{2})" $sigDrawer $others --quit

    #python scripts/$plotter --dir analysis_$channel\_$tag --var bjet1_bID_deepFlavor --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "deepFlavourScore(b_{1})" $sigDrawer $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var bjet2_bID_deepFlavor --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "deepFlavourScore(b_{2})" $sigDrawer $others --quit

    python scripts/$plotter --dir analysis_$channel\_$tag --var bH_mass   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "m_{bb} [GeV]"    $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var bH_pt	  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "p_{T,bb} [GeV]" $sigDrawer $others --quit

  #
  ## Htautau
  #
    python scripts/$plotter --dir analysis_$channel\_$tag --var dau1_pt 	--reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "p_{T}(lep_{1}) [GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var dau2_pt 	--reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "p_{T}(lep_{2}) [GeV]" $sigDrawer $others --quit

    python scripts/$plotter --dir analysis_$channel\_$tag --var dau1_eta	--reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "#eta(lep_{1})" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var dau2_eta	--reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "#eta(lep_{1})" $sigDrawer $others --quit

    #python scripts/$plotter --dir analysis_$channel\_$tag --var isTau2real	--reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "isTau2Real" $sigDrawer $others --quit

    #python scripts/$plotter --dir analysis_$channel\_$tag --var dau1MET_deltaphi --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "METDeltaPhi_{lep1}" $sigDrawer $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var dau2MET_deltaphi --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "METDeltaPhi_{lep2}" $sigDrawer $others --quit

    python scripts/$plotter --dir analysis_$channel\_$tag --var tauH_SVFIT_mass --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "m_{#tau#tau}(SVFit) [GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var tauH_SVFIT_pt   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "p_{T,#tau#tau}(SVFit) [GeV]" $sigDrawer $others --quit

    python scripts/$plotter --dir analysis_$channel\_$tag --var tauH_mass	--reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "m_{#tau#tau} (vis) [GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var tauH_pt 	--reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "p_{T,#tau#tau} (vis) [GeV]" $sigDrawer $others --quit

    python scripts/$plotter --dir analysis_$channel\_$tag --var ditau_deltaR	--reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "#DeltaR_{lep1,lep2}" $sigDrawer $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var ditau_deltaEta	--reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "#Delta#eta_{lep1lep2}" $sigDrawer $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var ditau_deltaPhi	--reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "#Delta#phi_{lep1lep2}" $sigDrawer $others --quit

    #python scripts/$plotter --dir analysis_$channel\_$tag --var mT1	        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "m_{T}(lep_{1}) [GeV]" $sigDrawer $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var mT2 	        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "m_{T}(lep_{2}) [GeV]" $sigDrawer $others --quit

    #python scripts/$plotter --dir analysis_$channel\_$tag --var jetFakeSF	--reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "jetFakeSF" $sigDrawer $others --quit
  #
  ## MET
  #
    python scripts/$plotter --dir analysis_$channel\_$tag --var met_et  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "met_{et} [GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var met_phi --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "met_{#phi}" $sigDrawer $others --quit
  #
  ## HH
  #
    #python scripts/$plotter --dir analysis_$channel\_$tag --var HHKin_mass --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "m_{HH}[GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var HH_mass --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "m_{HH}[GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var HH_pt   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "p_{T,HH}[GeV]" $sigDrawer $others --quit

  #
  ## VBF-Jets
  #
    #python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjet1_pt     --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "p_{T}(VBF jet{1}) [GeV]" $sigDrawer $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjet2_pt     --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "p_{T}(VBF jet{2}) [GeV]" $sigDrawer $others --quit

    #python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjet1_eta    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "#eta(VBF jet{1})" $sigDrawer $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjet2_eta    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "#eta(VBF jet{2})" $sigDrawer $others --quit

    #python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjj_mass     --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "m_{jj} [GeV]"    $sigDrawer $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjj_deltaEta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "#Delta#eta_{jj}" $sigDrawer $others --quit

done
