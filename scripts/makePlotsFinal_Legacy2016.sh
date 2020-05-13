tag=12May2020_Legacy2016_tauIDhomemade

log=(--log)

plotter=makeFinalPlots_Legacy2016.py

channel=TauTau
#channel=MuTau
#channel=ETau
#channel=MuMu

lumi=35.92

reg=SR  # A:SR , B:SStight , C:OSinviso, D:SSinviso, B': SSrlx

baseline=baseline  # baseline, s1b1jresolved, s2b0jresolved, sboostedLL, baseline_noQCD, baseline_MTT

others=""

sigDrawer=--no-sig

mkdir plots_$channel
mkdir plots_$channel/$tag
mkdir plots_$channel/$tag/$baseline\_$reg

#VARS: dau1_pt, dau2_pt, dau1_eta, dau2_eta, bjet1_pt, bjet2_pt, tauH_mass, tauH_SVFIT_mass, ditau_deltaR, tauH_pt, tauH_SVFIT_pt, bH_mass, bH_pt, HH_mass_raw, HH_pt

#for i in `seq 0 1`; # 1:noLog  - 0:Log
for i in `seq 1`;
do
  #
  ## Hbb
  #
    python scripts/$plotter --dir analysis_$channel\_$tag --var bjet1_pt   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(b_{1}) [GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var bjet2_pt   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(b_{2}) [GeV]" $sigDrawer $others --quit

   python scripts/$plotter --dir analysis_$channel\_$tag --var bH_mass  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{bb}^{vis} [GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var bH_pt   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(bb)^{vis} [GeV]" $sigDrawer $others --quit

  #
  ## Htautau
  #
    python scripts/$plotter --dir analysis_$channel\_$tag --var dau1_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(#tau_{1}) [GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var dau2_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(#tau_{2}) [GeV]" $sigDrawer $others --quit

    python scripts/$plotter --dir analysis_$channel\_$tag --var dau1_eta        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{#tau1}" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var dau2_eta        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{#tau2}" $sigDrawer $others --quit

    python scripts/$plotter --dir analysis_$channel\_$tag --var tauH_SVFIT_mass --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{#tau#tau}^{SVfit} [GeV]" $sigDrawer $others --quit

    python scripts/$plotter --dir analysis_$channel\_$tag --var tauH_mass       --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{#tau#tau}^{vis} [GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var tauH_pt       --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(#tau#tau)^{vis} [GeV]" $sigDrawer $others --quit


    python scripts/$plotter --dir analysis_$channel\_$tag --var ditau_deltaR         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#DeltaR_{#tau#tau}" $sigDrawer $others --quit

  #
  ## HH
  #
    python scripts/$plotter --dir analysis_$channel\_$tag --var HH_mass_raw  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{HH}^{vis} [GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var HH_pt    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(HH)^{vis} [GeV]" $sigDrawer $others --quit



done





