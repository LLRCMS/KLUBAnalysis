tag=8Dic

log=(--log)

channel=TauTau
#channel=MuTau
#channel=ETau
#baseline=baseline

baseline=baselineVBF
#baseline=baselineVBF_btagM
#baseline=baselineVBF_btagM_bVetoM
#baseline=baselineVBF_L
mkdir plots_$channel\VBF/$tag
mkdir plots_$channel\VBF/$tag/$baseline

for i in `seq 0 1`;
do
    #VBF variables: jets with highest mjj are chosen
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjj_mass     --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjj_deltaEta --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjet1_eta    --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjet2_eta    --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjet1_pt     --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjet2_pt     --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjet1_btag   --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjet2_btag   --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var VBFjj_HT       --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    
    
    #leading and subleading jet after bb selection
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jet3_pt        --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jet4_pt        --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jet3_eta        --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jet4_eta        --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jj_mass        --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867  --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jj_deltaEta    --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jj_HT          --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var njets          --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    
    
    #bjets ww
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var bjet1_bID      --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var bjet2_bID      --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var bjet1_z        --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var bjet2_z        --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
		 
    #HH
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var HH_z            --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag 
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var HH_zV           --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var HH_A            --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var tauH_mass            --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var bH_mass            --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var ditau_deltaPhi          --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var dib_deltaPhi            --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var ditau_deltaEta      --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var dib_deltaEta      --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    
    
    
    #5th jet (excluding bb and VBF jets)
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jet5_pt         --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jet5_eta        --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var jet5_z          --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    
    #taus
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var dau1_z        --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var dau2_z        --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag 
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var dau1_iso      --reg OS --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var dau2_iso      --reg OS --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    
    #met
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var met_et           --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var met_et_corr      --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    
    #top
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var top_Wc_bclose_mass        --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var top_Wc_bcentral_mass      --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var top_Wc_bforward_mass      --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var top_Wf_bclose_mass        --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var top_Wf_bcentral_mass      --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag
    python scripts/makeFinalPlots.py --dir analysis_$channel\_VBF$tag --var top_Wf_bforward_mass      --reg SR --sel $baseline --channel $channel --lymin 0.7 --lumi 35867 --quit ${log[i]} --ratio  --tag $tag

done


cd plots_$channel\VBF
scp -r $tag/$baseline camendol@lxplus.cern.ch://afs/cern.ch/user/c/camendol/www/VBFHH/plots$channel\VBF/$tag
cd ..





