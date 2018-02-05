tag=24Gen2018

log=(--log)

#channel=TauTau
#channel=MuTau
channel=ETau


reg=SR

baseline=baselineVBF
#baseline=baselineVBF_btagMfirst

#baseline=baselineVBF_btagM
#baseline=baselineVBF_btagM_bVetoM

#baseline=baselineVBF_L_btagMfirst
#baseline=baselineVBF_T_btagMfirst

mkdir plots_$channel\VBF/$tag
mkdir plots_$channel\VBF/$tag/$baseline\_$reg



for i in `seq 0 1`;
do
   #VBF variables: jets with highest mjj are chosen
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjj_mass     --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "m^{VBF}_{jj} [GeV]"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjj_mass_log     --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjj_deltaEta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "|#Delta#eta^{VBF}_{jj}|"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjet1_eta    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "#eta^{VBF}_{jet1}"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjet2_eta    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "#eta^{VBF}_{jet2}"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjet1_pt     --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "#p^{VBF}_{T jet1} [GeV]"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjet2_pt     --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "p^{VBF}_{T jet2} [GeV]"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjet1_btag   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "btag ID jet^{VBF}_{T1}"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjet2_btag   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "btag ID jet^{VBF}_{T2}"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjj_HT       --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "H^{VBF}_{Tjj} [GeV]"
  #
  #
  ##leading and subleading jet after bb selection
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jet3_pt        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "#p_{T jet3} [GeV]"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jet4_pt        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "#p_{T jet4} [GeV]"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jet3_eta        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "#eta_{jet3}"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jet4_eta        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "#eta_{jet4}"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jj_mass        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9  --quit ${log[i]} --ratio  --tag $tag --label "m_{jj} [GeV]"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jj_mass_log    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9  --quit ${log[i]} --ratio  --tag $tag 
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jj_deltaEta    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "|#Delta#eta_{jj}|"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jj_HT          --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "H_{Tjj} [GeV]"
  #
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var njets          --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "n jets"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var addjets          --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "n additional jets"
  #
  #
  ##bjets ww
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var bjet1_bID      --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "b-jet1 CSV score"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var bjet2_bID      --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "b-jet2 CSV score"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var bjet1_z        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "z_{bjet1}"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var bjet2_z        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "z_{bjet2}"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var bjets_bID        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "b-jets CSV score"
  #		 
  ##HH
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var HH_z            --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "z_{HH}"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var HH_zV           --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "\zeta_{HH}"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var HH_A            --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "A_{HH}"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var HH_mass         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "m_{HH} [GeV]"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var HH_pt           --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "p^{HH}_{T}"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var HH_deltaR       --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "#Delta R(H, H)"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var tauH_mass       --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "m_{H#tau#tau} [GeV]"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var bH_mass         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "m_{Hbb} [GeV]"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var bH_pt           --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "p_{T}^{Hbb} [GeV]"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var tauH_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "p_{T}^{H#tau#tau} [GeV]"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var ditau_deltaPhi  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "#Delta#phi(#tau,#tau)"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var dib_deltaPhi    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "#Delta#phi(b,b)"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var ditau_deltaEta  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "|#Delta#eta(#tau,#tau)|"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var dib_deltaEta    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "|#Delta#eta(b,b)|"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var dib_deltaR      --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "#Delta R(b,b)"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var mT2            --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --blind-range 100 10000 --label "m_{T2} [GeV]"


  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jet5_VBF_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "first additional jet p_{T} [GeV]"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jet5_VBF_eta        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "first additional jet #eta"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jet5_VBF_z          --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "first additional jet z"
  
  ##5th jet (excluding bb and first 2 energetic jets)
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jet5_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "first additional jet p_{T} [GeV]"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jet5_eta        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "first additional jet #eta"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jet5_z          --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "first additional jet z"
  #
  ##taus
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var dau1_z        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "z_{dau1}"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var dau2_z        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag  --label "z_{dau2}"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var dau1_iso      --reg OS --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "MVAiso_{dau1}"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var dau2_iso      --reg OS --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "MVAiso_{dau2}"
  #
  ##met
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var met_et           --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "MET [GeV]"
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var met_et_corr      --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag --label "MET corr [GeV]"
  #
  ##top
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var top_Wc_bclose_mass        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag 
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var top_Wc_bcentral_mass      --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var top_Wc_bforward_mass      --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var top_Wf_bclose_mass        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var top_Wf_bcentral_mass      --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var top_Wmass_bclose_mass      --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var top_Wjj_b_mass      --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag
  python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var top_Wjj_bclose_mass      --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi 35.9 --quit ${log[i]} --ratio  --tag $tag
  

done


cd plots_$channel\VBF
scp -r $tag/$baseline\_$reg camendol@lxplus.cern.ch://afs/cern.ch/user/c/camendol/www/VBFHH/plots$channel\VBF/$tag
cd ..





