basedir=/gwpool/users/dzuolo/HHbbtautatuAnalysisLegacy/CMSSW_11_1_0_pre6/src/KLUBAnalysis
#tag=analysis_2021_04_23
tag=2021_04_23
plotter=makeLegacyPlots.py

year=2018

channel=TauTau
#channel=MuTau
#channel=ETau

reg=SR

sel1=s1b1jresolvedMcut
sel2=s2b0jresolvedMcut
sel3=sboostedLLMcut
sel4=GGFclass
sel5=VBFclass
sel6=DYclass
sel7=ttHclass
sel8=TTclass
#sel9=VBFloose
#sel10=baseline

# CREATE DIRECTORIES FOR THE SINGLE SELECTIONS
#mkdir -p ./LegacyPlots/Legacy$year/$channel\_$tag/$sel1
#mkdir -p ./LegacyPlots/Legacy$year/$channel\_$tag/$sel2
#mkdir -p ./LegacyPlots/Legacy$year/$channel\_$tag/$sel3
#mkdir -p ./LegacyPlots/Legacy$year/$channel\_$tag/$sel4
#mkdir -p ./LegacyPlots/Legacy$year/$channel\_$tag/$sel5
#mkdir -p ./LegacyPlots/Legacy$year/$channel\_$tag/$sel6
#mkdir -p ./LegacyPlots/Legacy$year/$channel\_$tag/$sel7
#mkdir -p ./LegacyPlots/Legacy$year/$channel\_$tag/$sel8
#mkdir -p ./LegacyPlots/Legacy$year/$channel/$sel9
#mkdir -p ./LegacyPlots/Legacy$year/$channel/$sel10 
# COPY THE index.php FILE
#cp $basedir/index.php ./LegacyPlots/Legacy$year/$channel/$sel1
#cp $basedir/index.php ./LegacyPlots/Legacy$year/$channel/$sel2
#cp $basedir/index.php ./LegacyPlots/Legacy$year/$channel/$sel3
#cp $basedir/index.php ./LegacyPlots/Legacy$year/$channel/$sel4
#cp $basedir/index.php ./LegacyPlots/Legacy$year/$channel/$sel5
#cp $basedir/index.php ./LegacyPlots/Legacy$year/$channel/$sel6
#cp $basedir/index.php ./LegacyPlots/Legacy$year/$channel/$sel7
#cp $basedir/index.php ./LegacyPlots/Legacy$year/$channel/$sel8


if [[ ${channel} = *"MuTau"* ]]
then
        obj1="mu"
        obj2="tau"
fi

if [[ ${channel} = *"ETau"* ]]
then
        obj1="e"
        obj2="tau"
fi

if [[ ${channel} == *"TauTau"* ]]
then
        obj1="tau"
        obj2="tau"
fi

echo $obj1,$obj2


#################################################################
#################################################################
# The plotting script has some bool options that can be called to make the plot as we like it to be:
#     1. --doStatSystBand   -> do the stat+syst uncertainty band in the ratio plot (as of now the uncertainty band on the stack is only the statistical)
#     2. --removeESsystBand -> remove the energy scales (i.e. TES, MES, EES, JES) from the syst uncertainty band
#     3. --no-binwidth      -> turn off the scaling of the histogram by the bin-width
#     4. --binNXaxis        -> make the plot with the bin number on the x-axis instead of the values of the variable being plotted
#     5. --ratio            -> plot the Data/MC ratio plot
#     6. --no-sig           -> do not plot the signal superimposed to the stack plot
#     7. --no-data          -> do not plot the data superimposed to the stack plot
#     8. --dynamicRatioY    -> fix the range of the ratio plot depending on the values of the ratio itself instead of fixed values
# and an option to unblind only part of the DNN score spectrum:
#     1. --blind-range -> do not plot the data in teh specified range 
#                      -> takes as input two numbers: MIN and MAX of range to be blinded
# TEST OPTIONS
#python scripts/$plotter --dir $basedir/analysis\_$tag/$channel\_$year/total --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel2 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.6 1 --no-binwidth --binNXaxis --doStatSystBand --addJERunc
#python scripts/$plotter --dir $basedir/analysis\_$tag/$channel\_$year/total --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel2 --channel $channel --tag $tag --label "DNN score"      --quit --ratio --log --blind-range 0.6 1 --binNXaxis --doStatSystBand --addJERunc
#python scripts/$plotter --dir $basedir/analysis\_$tag/$channel\_$year/total --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel2 --channel $channel --tag $tag --label "DNN score"      --quit --ratio --log --blind-range 0.6 1 --doStatSystBand --addJERunc
#python scripts/$plotter --dir $basedir/analysis\_$tag/$channel\_$year/total --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.6 1 --no-binwidth --binNXaxis --doStatSystBand --addJERunc
#python scripts/$plotter --dir $basedir/analysis\_$tag/$channel\_$year/total --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.6 1 --binNXaxis 
#python scripts/$plotter --dir $basedir/analysis\_$tag/$channel\_$year/total --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.6 1 
python scripts/$plotter --dir $basedir/analysis\_$tag/$channel\_$year/total --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.6 1 --binNXaxis --doStatSystBand --addJERunc
python scripts/$plotter --dir $basedir/analysis\_$tag/$channel\_$year/total --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.6 1 --doStatSystBand --addJERunc
#python scripts/$plotter --dir $basedir/analysis\_$tag/$channel\_$year/total --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN score"      --quit --ratio --log --blind-range 0.6 1 --doStatSystBand --addJERunc
#
#
# PLOTS WITH BIN-WIDTH SCALING
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel1 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel2 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel3 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel4 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel6 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.0 0.5 --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel7 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.0 0.5 --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel8 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.0 0.5 --doStatSystBand
#
#
## PLOTS WITHOUT BIN-WIDTH SCALING
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel1 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel2 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel3 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel4 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel6 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel7 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel8 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --doStatSystBand
#
#
## PLOTS WITH BIN-NUMBER AS X-AXIS
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel1 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel2 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel3 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel4 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel6 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --binNXaxis --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel7 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --binNXaxis --doStatSystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel8 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --binNXaxis --doStatSystBand
#
#
## PLOTS WITH BIN-NUMBER AS X-AXIS AND NO ES IN SYST BAND
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel1 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis --doStatSystBand --removeESsystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel2 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis --doStatSystBand --removeESsystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel3 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis --doStatSystBand --removeESsystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel4 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis --doStatSystBand --removeESsystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis --doStatSystBand --removeESsystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel6 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --binNXaxis --doStatSystBand --removeESsystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel7 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --binNXaxis --doStatSystBand --removeESsystBand
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel8 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --binNXaxis --doStatSystBand --removeESsystBand
#
#
## PLOTS WITH BIN-NUMBER AS X-AXIS AND NO SYST BAND
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel1 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel2 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel3 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel4 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel6 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --binNXaxis
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel7 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --binNXaxis
#python scripts/$plotter --dir $basedir/$tag/$channel --year $year --var DNNoutSM_kl_1 --reg $reg --sel $sel8 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --binNXaxis
