#tag=23Jan2020
#tag=23Jan2020_invQCD
#tag=27Jan2020_noTauIDSF
#tag=08May2020_SecondProd_FirstSkim_Second_InvQCD
#tag=08May2020
#tag=08May2020_SecondProd_FirstSkim_Third_InvQCD_pTBinnedTauIDSF
#tag=08May2020_SecondProd_FirstSkim_Fourth_InvQCD_homeMadeTauIDSF
#tag=08May2020_SecondProd_FirstSkim_Fifth_InvQCD_noTauIDSF
#tag=08May2020_SecondProd_FirstSkim_Sixth_InvQCD_noDYMTTSF
#tag=08May2020_SecondProd_FirstSkim_Seventh_InvQCD_specialMerging
tag=Legacy2017_15June2020_pt_Dep_SF

log=(--log)

plotter=makeFinalPlots_Legacy2017.py

channel=TauTau
#channel=MuTau
#channel=ETau
#channel=MuMu

lumi=41.529

reg=SR  # A:SR , B:SStight , C:OSinviso, D:SSinviso, B': SSrlx

baseline=baseline
#baseline=s1b1jresolved
#baseline=s2b0jresolved
#baseline=sboostedLL
#baseline=VBFloose
#baseline=baseline_MTT
#baseline=baseline40to70
#baseline=baseline70
#baseline=baseline_both0
#baseline=baseline_both1
#baseline=baseline_both10
#baseline=baseline_both11

others=""

sigDrawer=--no-sig

mkdir plots_$channel
mkdir plots_$channel/$tag
mkdir plots_$channel/$tag/$baseline\_$reg

#VARS: dau1_pt, dau2_pt, dau1_eta, dau2_eta, bjet1_pt, bjet2_pt, tauH_mass, tauH_SVFIT_mass, ditau_deltaR, tauH_pt, bH_pt, bH_mass

#for i in `seq 0 1`; # 1:noLog  - 0:Log
for i in `seq 1`;
do
  #
  ## Hbb
  #
    python scripts/$plotter --dir analysis_$channel\_$tag --var bjet1_pt               --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(b_{1}) [GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var bjet2_pt               --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(b_{2}) [GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var bjet1_bID_deepFlavor   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "bjet1_deepFlavor"   $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var bjet2_bID_deepFlavor   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "bjet2_deepFlavor"   $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var bH_pt                  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T} [GeV]"        $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var bH_mass                --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{bb} [GeV]"       $sigDrawer $others --quit

  #
  ## Htautau
  #
    python scripts/$plotter --dir analysis_$channel\_$tag --var dau1_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(#tau_{1}) [GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var dau2_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(#tau_{2}) [GeV]" $sigDrawer $others --quit

    python scripts/$plotter --dir analysis_$channel\_$tag --var dau1_eta        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{#tau1}"          $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var dau2_eta        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{#tau2}"          $sigDrawer $others --quit

    python scripts/$plotter --dir analysis_$channel\_$tag --var tauH_SVFIT_mass --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{#tau#tau}^{SVfit} [GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var tauH_mass       --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{#tau#tau}^{vis} [GeV]"  $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var tauH_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T#tau#tau}^{vis} [GeV]" $sigDrawer $others --quit

    python scripts/$plotter --dir analysis_$channel\_$tag --var ditau_deltaR    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#DeltaR_{#tau#tau}"          $sigDrawer $others --quit

  #
  ## HH
  #
    python scripts/$plotter --dir analysis_$channel\_$tag --var HH_mass_raw  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "m_{bb#tau#tau} [GeV]" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var HH_pt        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "p_{T}(bb#tau#tau) [GeV]" $sigDrawer $others --quit

  #
  ## BDT OUTS 
  #
  
    python scripts/$plotter --dir analysis_$channel\_$tag --var BDToutSM_kl_1 --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "BDT score (#tau#tau, k_{#lambda}=1)" $sigDrawer $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var DNNoutSM_kl_1 --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "BDT score (#tau#tau, k_{#lambda}=1)" $sigDrawer $others --quit

done
