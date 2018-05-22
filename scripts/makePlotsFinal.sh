tag=15May2018_2017B

log=(--log)

#channel=TauTau
channel=MuTau
#channel=ETau

#lumi=13.4
lumi=4.7
reg=SR

baseline=s1b1jresolved
#baseline=baselineVBF_btagMfirst

#baseline=baselineVBF_btagM
#baseline=baselineVBF_btagM_bVetoM

#baseline=baselineVBF_L_btagMfirst
#baseline=baselineVBF_T_btagMfirst

others="--quit"


mkdir plots_$channel/$tag
mkdir plots_$channel/$tag/$baseline\_$reg


for i in `seq 0 1`;
do
  #
  ##bjets 
    #  python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var bjet1_pt        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi  ${log[i]} --ratio  --tag $tag --label "p_{Tbjet1}" --no-sig $others
     python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var bH_mass        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi  ${log[i]} --ratio  --tag $tag --label "H_{bb} mass [GeV]" --no-sig $others
#  python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var bjet2_pt       --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{Tbjet2}" --no-sig $others
  
  #		 
  ##HH
  #
  ##taus
  python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau1_pt        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "p_{Tdau1} [GeV]" --no-sig $others
  python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var dau2_pt       --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag  --label "p_{Tdau2} [GeV]" --no-sig $others
  #
  ##met
  python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var met_et           --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "MET [GeV]" --no-sig $others
  python scripts/makeFinalPlots.py --dir analysis_$channel\_$tag --var met_er_et      --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi ${log[i]} --ratio  --tag $tag --label "MET no HE [GeV]" --no-sig $others
  #

done


cd plots_$channel
scp -r $tag/$baseline\_$reg camendol@lxplus.cern.ch://afs/cern.ch/user/c/camendol/www/HH2017/plots$channel/$tag
cd ..





