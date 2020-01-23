if [ ! -d "/eos/home-c/camendol/www" ]; then
    kinit camendol@CERN.CH
    /opt/exp_soft/cms/t3/eos-login -username camendol  
fi

#tag=28Jan2019_decaymodes_bins_newWeights
#tag=7Feb2019_decaymodes
#tag=11Mar2019_decaymodes_DYbins_oldDYWeight_DYenriched_mySF
#tag=11Mar2019_decaymodes_DYbins_oldDYWeight_mySF
#tag=20Mar2019_decaymodes_DYbins_oldDYWeight_DYembedded_FakeJetQCD_mySFfake
#tag=20Mar2019_decaymodes_DYbins_oldDYWeight_FakeJetQCD_mySFfake
#tag=11Mar2019_decaymodes_DYbins_oldDYWeight_tauPOGSF
#tag=21Mar2019_decaymodes_DYbins_oldDYWeight_invABCD_QCDFT_mySF_tau
#tag=27Mar2019_decaymodes_DYbins_oldDYeights_tauPOGSF
#_VTight
#tag=30Apr2019_decaymodes_loosepu_mtt_mySF
#tag=4May2019_decaymodes_DYLO_loosepu_mtt_tauIDSF
#tag=4May2019_decaymodes_DYLO_DYenr_loosepu_mtt_tauIDSF
#tag=4May2019_decaymodes_DYLO_DYenr_loosepu_mtt_notauID
tag=4May2019_decaymodes_DYLO_loosepu_mtt_tauIDSF
#tag=4May2019_decaymodes_DYLO_DYenr_notauIDSF
log=(--log)


plotter=binCategoryPlots.py


channel=TauTau
#channel=MuTau
#channel=ETau
#channel=MuMu

#lumi=13.4
lumi=41.6
reg=SR  # A:SR , B:SStight , C:OSinviso, D:SSinviso, B': SSrlx
#reg=SStight
#reg=SSrlx
#reg=OSinviso
#reg=SSinviso
#reg=SRVTight

baseline=baselineHTauTau55
others="--ratio"
#others=""
others="--quit --ratio --name DY~enriched"
#others="--no-binwidth"
#others="--quit --name baseline40"
#others="--quit --name \"baseline~w/o~lep~veto\""

mkdir plotsHH2017_$channel
mkdir plotsHH2017_$channel/$tag
mkdir plotsHH2017_$channel/$tag/$baseline\_$reg



python scripts/$plotter --dir analysis_$channel\_$tag --var dau1_eta        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi  --tag $tag --label "#eta_{#tau1}" --no-sig $others 


cd plotsHH2017_$channel
mkdir /eos/home-c/camendol/www/HH2017/plotsHH2017$channel\_Jan2019/$tag
if [ -d  /eos/home-c/camendol/www/HH2017/plotsHH2017$channel\_Jan2019/$tag/$baseline\_$reg ]
then
    echo "removing"
    rm -rf  /eos/home-c/camendol/www/HH2017/plotsHH2017$channel\_Jan2019/$tag/$baseline\_$reg
fi
if [ -d  /eos/home-c/camendol/www/HH2017/plotsHH2017$channel\_Jan2019/$tag/$baseline\_$reg ]
then
    echo "pippa"
fi
mkdir /eos/home-c/camendol/www/HH2017/plotsHH2017$channel\_Jan2019/$tag/$baseline\_$reg
cp $tag/$baseline\_$reg/* /eos/home-c/camendol/www/HH2017/plotsHH2017$channel\_Jan2019/$tag/$baseline\_$reg
cd ..

cp /eos/home-c/camendol/www/index.php /eos/home-c/camendol/www/HH2017/plotsHH2017$channel\_Jan2019/$tag
cp /eos/home-c/camendol/www/index.php /eos/home-c/camendol/www/HH2017/plotsHH2017$channel\_Jan2019/$tag/$baseline\_$reg


cd plotsHH2017_$channel



#ssh camendol@lxplus.cern.ch "mkdir -p /eos/home-c/camendol/www/HH2017/plotsHH2017$channel/$tag" && scp -r $tag/$baseline\_$reg camendol@lxplus.cern.ch://eos/home-c/camendol/www/HH2017/plotsHH2017$channel/$tag && scp ../index.php camendol@lxplus.cern.ch://eos/home-c/camendol/www/HH2017/plotsHH2017$channel/$tag && scp ../index.php camendol@lxplus.cern.ch://eos/home-c/camendol/www/HH2017/plotsHH2017$channel/$tag/$baseline\_$reg

#ssh camendol@lxplus.cern.ch "mkdir -p //eos/home-c/camendol/www/HH2017_Jan2019/plotsHH2017$channel/$tag"
#scp -r $tag/$baseline\_$reg camendol@lxplus.cern.ch://eos/home-c/camendol/www/HH2017_Jan2019/plotsHH2017$channel/$tag
#ssh camendol@lxplus.cern.ch "cp /eos/home-c/camendol/www/index.php /eos/home-c/camendol/www/HH2017_Jan2019/plotsHH2017$channel/$tag ; /eos/home-c/camendol/www/index.php /eos/home-c/camendol/www/HH2017_Jan2019/plotsHH2017$channel/$tag/$baseline\_$reg"
#cd ..

echo [Alt + CMD + 2click]: http://camendol.web.cern.ch/camendol/HH2017/plotsHH2017$channel\_Jan2019/$tag/$baseline\_$reg
