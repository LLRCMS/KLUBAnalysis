#tag=2018_17Nov2020_plots

log=(--log)

plotter=makeFinalPlots_Legacy2018.py

#channel=TauTau
#channel=MuTau
channel=ETau
#channel=MuMu

lumi=59.741

reg=SR  # A:SR , B:SStight , C:OSinviso, D:SSinviso, B': SSrlx

baseline=baselineMcut  # baseline, baselineMcut, s1b1jresolvedMcut, s2b0jresolvedMcut, sboostedLLMcut, VBFloose, baseline_noQCD, baseline_MTT, VBFclass
#baseline=baselineHTauTau_bothDM1_DY #, baselineHTauTau_bothDM0_DY, baselineHTauTau_bothDM1_DY, baselineHTauTau_bothDM10_DY, baselineHTauTau_bothDM11_DY

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
    ##python scripts/$plotter --dir analysis_$channel\_$tag --var bjet2_eta              --reg $reg --sel $baseline --channel $channel --name $baseline --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{b_{2}}"       $others --quit
    ##python scripts/$plotter --dir analysis_$channel\_$tag --var bjet2_pt               --reg $reg --sel $baseline --channel $channel --name $baseline --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(b_{2}) [GeV]" $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var bjet1_eta              --reg $reg --sel $baseline --channel $channel --name $baseline --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{b_{1}}"       $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var bjet1_pt               --reg $reg --sel $baseline --channel $channel --name $baseline --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(b_{1}) [GeV]" $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var bjet1_bID_deepFlavor   --reg $reg --sel $baseline --channel $channel --name $baseline --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "b_{1} DeepFlavor"   $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var bjet2_bID_deepFlavor   --reg $reg --sel $baseline --channel $channel --name $baseline --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "b_{2} DeepFlavor"   $others --quit
    ##python scripts/$plotter --dir analysis_$channel\_$tag --var bH_mass_raw            --reg $reg --sel $baseline --channel $channel --name $baseline --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{bb} [GeV]"       $others --quit

  #
  ## Htautau
  #
    #python scripts/$plotter --dir analysis_$channel\_$tag --var dau1_pt         --reg $reg --sel $baseline --channel $channel --name $baseline --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{T}(#tau_{1}) [GeV]"      $others --quit
    #python scripts/$plotter --dir analysis_$channel\_$tag --var dau1_eta        --reg $reg --sel $baseline --channel $channel --name $baseline --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "#eta_{#tau1}"               $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var tauH_SVFIT_mass --reg $reg --sel $baseline --channel $channel --name $baseline --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "m_{#tau#tau}^{SVfit} [GeV]" $others --quit

  #
  ## Others
  #
    python scripts/$plotter --dir analysis_$channel\_$tag --var met_et      --reg $reg --sel $baseline --channel $channel --name $baseline --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "MET [GeV]"               $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var met_phi     --reg $reg --sel $baseline --channel $channel --name $baseline --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "MET #phi"                $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjj_mass  --reg $reg --sel $baseline --channel $channel --name $baseline --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "m_{VBFjj}"               $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjet1_pt  --reg $reg --sel $baseline --channel $channel --name $baseline --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "p_{T}(VBF_{jet1}) [GeV]" $others --quit
    python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjet1_eta --reg $reg --sel $baseline --channel $channel --name $baseline --lymin 0.7 --lumi $lumi ${log[i]} --ratio --tag $tag --label "#eta_(VBF_{jet1})"       $others --quit
  
  #
  ## DNN scores
  #
    #python scripts/$plotter --dir analysis_$channel\_$tag --var DNNoutSM_kl_1 --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[0]} --ratio --tag $tag --label "DNN score" $others --blind-range 0.6 1.0 --quit

done


### scores
##
#mkdir plots_$channel/$tag/scores_$reg
##
##
### VBF score
### GGFclass, VBFclass, ttHclass, TTclass, DYclass
#python scripts/$plotter --dir analysis_$channel\_$tag --var DNNoutSM_kl_1 --reg $reg --sel GGFclass --channel $channel --name GGFclass  --lymin 0.7 --lumi $lumi ${log[0]} --ratio --tag $tag --label "DNN score" $others --blind-range 0.6 1.0 --quit
#python scripts/$plotter --dir analysis_$channel\_$tag --var DNNoutSM_kl_1 --reg $reg --sel VBFclass --channel $channel --name VBFclass  --lymin 0.7 --lumi $lumi ${log[0]} --ratio --tag $tag --label "DNN score" $others --blind-range 0.6 1.0 --quit
#python scripts/$plotter --dir analysis_$channel\_$tag --var DNNoutSM_kl_1 --reg $reg --sel ttHclass --channel $channel --name ttHclass  --lymin 0.7 --lumi $lumi ${log[0]} --ratio --tag $tag --label "DNN score" $others --blind-range 0.6 1.0 --quit
#python scripts/$plotter --dir analysis_$channel\_$tag --var DNNoutSM_kl_1 --reg $reg --sel TTclass  --channel $channel --name TTclass   --lymin 0.7 --lumi $lumi ${log[0]} --ratio --tag $tag --label "DNN score" $others --blind-range 0.6 1.0 --quit
#python scripts/$plotter --dir analysis_$channel\_$tag --var DNNoutSM_kl_1 --reg $reg --sel DYclass  --channel $channel --name DYclass   --lymin 0.7 --lumi $lumi ${log[0]} --ratio --tag $tag --label "DNN score" $others --blind-range 0.6 1.0 --quit
