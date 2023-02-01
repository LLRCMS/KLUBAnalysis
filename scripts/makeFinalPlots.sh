if [ ! -d "/eos/home-c/camendol/www" ]; then
    kinit camendol@CERN.CH
    /opt/exp_soft/cms/t3/eos-login -username camendol  
fi

tag=22Jan2020
log=(--log)


#VBF scan
#tag=7Oct2019_DYLO_loosepu_mh_mySF_sboost_noVBFscan


#final
#tag=12Sept2019_Thesis_lambdaScan_11



plotter=makeFinalPlots.py
#plotter=makeFinalPlots_allbkg.py
#plotter=makeFinalPlots_TT.py
#plotter=makeFinalPlots_DY.py


channel=TauTau
#channel=MuTau
#channel=ETau
#channel=MuMu


lumi=41.6
reg=SR  # A:SR , B:SStight , C:OSinviso, D:SSinviso, B': SSrlx
#reg=SStight
#reg=SSrlx
#reg=OSinviso
#reg=SSinviso
#reg=SRVTight


baseline=baseline55



#others=""
others="--quit --ratio --no-sig" # --sigscale 10 10"
#others="--quit --ratio --name DYreg"
#others="--no-binwidth"
#others="--quit --name baseline40"
#others="--quit --name \"baseline~w/o~lep~veto\""

mkdir plotsHHLegacy2018_$channel
mkdir plotsHHLegacy2018_$channel/$tag
mkdir plotsHHLegacy2018_$channel/$tag/$baseline\_$reg


if [[ ${channel} = *"MuTau"* ]]
then
    obj1="#mu"
    obj2="#tau_{h}"
fi

if [[ ${channel} = *"ETau"* ]]
then
    obj1="e"
    obj2="#tau_{h}"
fi

if [[ ${channel} == *"TauTau"* ]]
then
    obj1="#tau_{h1}"
    obj2="#tau_{h2}"
fi

echo $obj1

    #for i in `seq 0 1`; # 1:noLog  - 0:Log
    #for i in `seq 1`;
#do
  #
## Hbb

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var bjet1_pt   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi  --tag $tag --label "b jet p_{T} [GeV]"  $others 
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var bjet2_pt   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi   --tag $tag --label "p_{Tbjet2} [GeV]"  $others 

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var bjet1_eta  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi   --tag $tag --label "b jet #eta"  $others 
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var bjet2_eta  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "b jet #eta"  $others 

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var bjet1_bID_deepCSV  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "b jet DeepCSV score"  $others 
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var bjet2_bID_deepCSV  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "b jet DeepCSV score"  $others 

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var bH_mass        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi   --tag $tag --label "m_{bb} [GeV]"  $others
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var bH_mass_raw        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi   --tag $tag --label "m_{jj} [GeV]"  $others
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var bH_pt        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi  ${log[i]}   --tag $tag --label "PT_{bb} [GeV]"  $others 
###python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var dib_dEtaSign   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi  ${log[i]}   --tag $tag --label "#eta_{bjet1}x#eta_{bjet2}"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var dib_deltaR     --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi  ${log[i]}   --tag $tag --label "#DeltaR_{bb}"  $others --quit

  #
## Htautau
  #
##taus


#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var dau1_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "${obj1} p_{T} [GeV]"  $others 
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var dau2_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "${obj2} p_{T} [GeV]"  $others 

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var dau1_eta       --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "${obj1} #eta"  $others 
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var dau2_eta        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "${obj2} #eta"  $others 


#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var dau1_phi        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "${obj1} #phi"  $others 
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var dau2_phi        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "${obj2} #phi"  $others 


#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var dau1_iso        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "${obj1} Iso"  $others
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var dau2_iso        --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "${obj2} Iso"  $others
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var dau1_MVAisoNew  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi   --tag $tag --label "${obj1} MVA ID"  $others
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var dau2_MVAisoNew  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi   --tag $tag --label "${obj2} MVA ID "  $others 


#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var tauH_SVFIT_mass --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "m_{#tau#tau}^{SVfit} [GeV]"  $others 
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var tauH_mass       --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "m_{#tau#tau}^{vis} [GeV]"  $others

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var tauH_mass       --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "Z mass [GeV]"  $others --log --ymin 1 --ymax 100000000


#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var tauH_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "p_{TH#tau#tau}^{Vis} [GeV]"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var tauH_pt         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "Z p_{T} [GeV]"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var ditau_deltaEta         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "#Delta#eta_{#tau#tau}"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var ditau_deltaPhi         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "#Delta#phi_{#tau#tau}"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var ditau_deltaR         --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "#DeltaR_{#tau#tau}"  $others --quit 
#
## HH
#
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var HH_deltaR --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "#DeltaR_{HH}"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var HH_zV --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "#zeta_{HH}"  $others --quit
#
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var HH_A --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "A_{HH}"  $others --quit
#
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var dau1_z --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "First #tau z"  $others --quit
#
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var dau2_z --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "Second #tau z"  $others --quit
#
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var bjet1_z --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "First b jet z"  $others --quit
#
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var bjet2_z --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "Second b jet z"  $others --quit
#
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var bH_z --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "H_{bb} z"  $others --quit
#
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var tauH_z --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "H_{#tau#tau} z"  $others --quit
#
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var jet5_z --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "First additional jet z"  $others --quit
#
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var HH_z --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "HH system z"  $others --quit
 
#
## VBF
#
##python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var VBFjj_dEtaSign --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "#eta_{VBFj11}#times#eta_{VBFj2}"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var VBFjj_deltaEta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "|#Delta#eta_{jj}|"  $others --quit

python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var VBFjj_mass  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "m_{jj} [GeV]"  $others --quit

python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var VBFjet1_pt  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "p_{TVBFjet1} [GeV]"  $others --quit
python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var VBFjet2_pt  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "p_{TVBFjet2} [GeV]"  $others --quit

python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjj_mass  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "m_{jj} [GeV]"  $others --quit

python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjet1_pt  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "p_{TVBFjet1} [GeV]"  $others --quit
python scripts/$plotter --dir analysis_$channel\_$tag --var VBFjet2_pt  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "p_{TVBFjet2} [GeV]"  $others --quit

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var VBFjet1_eta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "VBF jet #eta"  $others --quit 
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var VBFjet2_eta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "VBF jet #eta"  $others --quit --ymax 14000

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var bH_VBF1_deltaEta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "#Delta#eta(H_{bb},VBFjet1)"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var VBFjet1_btag_deepCSV --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "DeepCSV_{VBFjet1}"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var VBFjet2_PUjetID --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "PUjetID_{VBFjet2}"  $others --quit

#
#met
#
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var met_et    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "Missing transverse momentum [GeV]"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var mT1    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "m_{T1} [GeV]"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var mT2    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "m_{T2} [GeV]"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var met_phi   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "#phi_{MET}"  $others --quit

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var met_er_et      --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "MET no HE [GeV]"  $others --quit


## BDT angela
#

#bH_mass_raw, bjet2_pt, bH_pt,  tauH_SVFIT_pt, BDT_HT20, p_zeta, p_zeta_visible, BDT_ditau_deltaPhi, BDT_tauHsvfitMet_\
#deltaPhi, mT_tauH_MET, mT_total, MT2, BDT_MX, BDT_bH_tauH_MET_InvMass, BDT_bH_tauH_SVFIT_InvMass, BDT_bH_tauH_InvMass, HHKin_mass_raw, HHKin_mass_raw_chi2, BDT_MET_bH_cosTheta


#
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var BDT_MET_bH_cosTheta    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "cos#theta(H_{bb}, p_{T}^{miss})"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var tauH_SVFIT_pt    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "H_{#tau#tau}^{SVfit} p_{T} [GeV]"  $others --quit
#
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var mT_tauH_MET    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "m_{T}(H_{#tau#tau}, E_{T}^{miss}) [GeV]"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var BDT_tauHsvfitMet_deltaPhi    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "#Delta #phi(H_{#tau#tau}^{SVfit}, p_{T}^{miss})"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var BDT_bH_tauH_SVFIT_InvMass    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "H_{#tau#tau}^{SVfit} mass [GeV]"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var BDT_bH_tauH_MET_InvMass    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "H_{#tau#tau} mass (with p_{T}^{miss})[GeV]"  $others --quit
##
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var mT_total    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "m_{T}^{TOT} [GeV]"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var mT_tauH_MET    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "m_{T}(H_{#tau#tau}, p_{T}^{miss}) [GeV]"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var BDT_MX   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "m_{X} [GeV]"  $others --quit
##
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var BDT_HT20    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "HT (jets p_{T} > 20 GeV) [GeV]"  $others 
##
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var HHKin_mass_raw    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "m_{HH}^{KinFit} [GeV]"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var HHKin_mass_raw_chi2    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "\chi^{2} m_{HH}^{KinFit} [GeV]"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var BDT_bH_tauH_InvMass    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "m_{HH}^{vis} [GeV]"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var BDT_bH_tauH_SVFIT_InvMass    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "m_{HH}^{SVfit} [GeV]"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var MT2    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "M_{T2} [GeV]"  $others  --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var bH_pt    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "H_{bb} p_{T} [GeV]"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var bH_mass    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "m_{bb} [GeV]"  $others --quit
##
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var bjet2_pt   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "Second b jet p_{T} [GeV]"  $others 
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var p_zeta    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "p_{#zeta}"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var p_zeta_visible    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "p_{#zeta}^{vis}"  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var BDT_ditau_deltaPhi    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "#Delta #phi(#mu,#tau_{h})"  $others --quit --ymax 15000




   #  if [[ ${baseline} != *"Mcut"* && ${baseline} != *"VBF"* ]];
   #  then

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var shifted_BDToutSM_kl_1    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi --log   --tag $tag --label "BDT output SM, k_{#lambda} = 1"  $others --quit
#
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var BDToutSM_kl_1    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi --log   --tag $tag --label "BDT output SM, k_{#lambda} = 1"  $others --quit --ymax 1000000 --ymin 10
# 
#  python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var BDToutSM_kl_0.001    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi --log   --tag $tag --label "BDT output SM, k_{#lambda} = 0"  $others --quit
# python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var BDToutSM_kl_2.45    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi --log   --tag $tag --label "BDT output SM, k_{#lambda} = 2.45"  $others --quit --ymax 1000000 --ymin 10
#  python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var BDToutSM_kl_5    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi --log   --tag $tag --label "BDT output SM, k_{#lambda} = 5"  $others --quit
# python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var BDToutSM_kl_30    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi --log   --tag $tag --label "BDT output SM, k_{#lambda} = 30"  $others --quit --ymax 1000000 --ymin 10

#  python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var DNN_VBFvsGGF_TauTauLoose    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi --log   --tag $tag --label "DNN output - #tau_{h}#tau_{h} loose"  $others --quit --ymax 10000000

#  python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var DNN_VBFvsGGF_MuTau    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi --log   --tag $tag --label "DNN output - #tau_{#mu}#tau_{h}"  $others --quit --ymax 10000000
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var DNN_VBFvsGGF_ETau    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi --log   --tag $tag --label "DNN output - #tau_{e}#tau_{h}"  $others --quit --ymax 100000000
#  python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var DNN_VBFvsGGF_TauTauTight    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi --log   --tag $tag --label "DNN output - #tau_{h}#tau_{h} tight"  $others --quit --ymax 100000


## njets
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var njets    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "Jet multiplicity"  $others


#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var njets20    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi   --log  --tag $tag --label "Jet multiplicity (p_{T} > 20 GeV)"  $others --xmin 0 --xmax 10
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var njets50    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "njets (p_{T} > 50 GeV)"  $others

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var BDT_HT20    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "H_{T} (p_{T} > 20 GeV)"  $others
       
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var nbjetscand    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "njets (p_{T}> 20 GeV; |#eta| < 2.4)"  $others

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var npv    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "npv"  $others

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var rho    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "#rho"  $others 

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var jet3_pt  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "p_{Tjet1} [GeV]"  $others 
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var jet4_pt  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "p_{Tjet2} [GeV]"  $others 
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var jet3_eta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "#eta_{jet1}"  $others
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var jet4_eta --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  --label "#eta_{jet2}"  $others



#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var MT2    --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag --label "M_{T2} [GeV]"  $others --quit


#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var fatjet_softdropMass   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  $others --quit --label "m_{AK8}"

#other
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var top_Wc_bclose_mass   --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  $others --quit

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var top_Wc_bcentral_mass  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  $others --quit

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var top_Wc_bforward_mass  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  $others --quit



#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var top_Wf_bclose_mass  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  $others --quit

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var top_Wf_bcentral_mass  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  $others --quit

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var top_Wf_bforward_mass  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var top_Wmass_bclose_mass  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  $others --quit

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var top_Wjj_b_mass  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  $others --quit

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var top_Wjj_bclose_mass  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  $others --quit

#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var isTau2real  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var nRealTaus  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  $others --quit
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var dau1_decayMode  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  $others --quit --label "#tau_{1} decay mode"
#python scripts/$plotter --dir /data_CMS/cms/amendola/analysisLegacy2018/analysis_$channel\_$tag --var dau2_decayMode  --reg $reg --sel $baseline --channel $channel --lymin 0.7 --lumi $lumi    --tag $tag  $others --quit --label "#tau_{2} decay mode"



	



cd plotsHHLegacy2018_$channel

mkdir /eos/home-c/camendol/www/HHLegacy2018/plotsHHLegacy2018$channel
mkdir /eos/home-c/camendol/www/HHLegacy2018/plotsHHLegacy2018$channel/$tag
if [ -d  /eos/home-c/camendol/www/HHLegacy2018/plotsHHLegacy2018$channel/$tag/$baseline\_$reg ]
then
    echo "removing"
    rm -rf  /eos/home-c/camendol/www/HHLegacy2018/plotsHHLegacy2018$channel/$tag/$baseline\_$reg
fi
if [ -d  /eos/home-c/camendol/www/HHLegacy2018/plotsHHLegacy2018$channel/$tag/$baseline\_$reg ]
then
    echo "pippa"
fi
mkdir /eos/home-c/camendol/www/HHLegacy2018/plotsHHLegacy2018$channel/$tag/$baseline\_$reg
cp $tag/$baseline\_$reg/* /eos/home-c/camendol/www/HHLegacy2018/plotsHHLegacy2018$channel\_Jan2019/$tag/$baseline\_$reg
cd ..
cp /eos/home-c/camendol/www/index.php /eos/home-c/camendol/www/HHLegacy2018/plotsHHLegacy2018$channel/$tag/$baseline\_$reg


cd plotsHHLegacy2018_$channel



#ssh camendol@lxplus.cern.ch "mkdir -p /eos/home-c/camendol/www/HHLegacy2018/plotsHHLegacy2018$channel\_Jan2019/$tag" && scp -r $tag/$baseline\_$reg camendol@lxplus.cern.ch://eos/home-c/camendol/www/HHLegacy2018/plotsHHLegacy2018$channel\_Jan2019/$tag && scp ../index.php camendol@lxplus.cern.ch://eos/home-c/camendol/www/HHLegacy2018/plotsHHLegacy2018$channel\_Jan2019/$tag && scp ../index.php camendol@lxplus.cern.ch://eos/home-c/camendol/www/HHLegacy2018/plotsHHLegacy2018$channel\_Jan2019/$tag/$baseline\_$reg

#ssh camendol@lxplus.cern.ch "mkdir -p //eos/home-c/camendol/www/HHLegacy2018/plotsHHLegacy2018$channel\_Jan2019/$tag"
#scp -r $tag/$baseline\_$reg camendol@lxplus.cern.ch://eos/home-c/camendol/www/HHLegacy2018/plotsHHLegacy2018$channel\_Jan2019/$tag
#ssh camendol@lxplus.cern.ch "cp /eos/home-c/camendol/www/index.php /eos/home-c/camendol/www/HHLegacy2018/plotsHHLegacy2018$channel\_Jan2019/$tag ; /eos/home-c/camendol/www/index.php /eos/home-c/camendol/www/HHLegacy2018/plotsHHLegacy2018$channel\_Jan2019/$tag/$baseline\_$reg"
#cd ..

echo [Alt + CMD + 2click]: http://camendol.web.cern.ch/camendol/HHLegacy2018/plotsHHLegacy2018$channel/$tag/$baseline\_$reg
