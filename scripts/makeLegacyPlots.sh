basedir=/home/llr/cms/motta/HHLegacy/CMSSW_11_1_0_pre6/src/KLUBAnalysis
tag=/MY_impacts2018
plotter=makeLegacyPlots.py

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

# CREATE DIRECTORIES FOR THE SINGLE SELECTIONS
mkdir -p ./LegacyPlots/$channel/$sel1
mkdir -p ./LegacyPlots/$channel/$sel2
mkdir -p ./LegacyPlots/$channel/$sel3
mkdir -p ./LegacyPlots/$channel/$sel4
mkdir -p ./LegacyPlots/$channel/$sel5
mkdir -p ./LegacyPlots/$channel/$sel6
mkdir -p ./LegacyPlots/$channel/$sel7
mkdir -p ./LegacyPlots/$channel/$sel8
# COPY THE index.php FILE
cp $basedir/index.php ./LegacyPlots/$channel/$sel1
cp $basedir/index.php ./LegacyPlots/$channel/$sel2
cp $basedir/index.php ./LegacyPlots/$channel/$sel3
cp $basedir/index.php ./LegacyPlots/$channel/$sel4
cp $basedir/index.php ./LegacyPlots/$channel/$sel5
cp $basedir/index.php ./LegacyPlots/$channel/$sel6
cp $basedir/index.php ./LegacyPlots/$channel/$sel7
cp $basedir/index.php ./LegacyPlots/$channel/$sel8


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
# 	  1. --doStatSystBand -> do the stat+syst uncertainty band in the ratio plot (as of now the uncertainty band on the stack is only the statistical)
# 	  2. --no-binwidth    -> turn off the scaling of the histogram by the bin-width
# 	  3. --binNXaxis      -> make the plot with the bin number on the x-axis instead of the values of the variable being plotted
# 	  4. --ratio          -> plot the Data/MC ratio plot
# 	  5. --no-sig         -> do not plot the signal superimposed to the stack plot
# 	  6. --no-data        -> do not plot the data superimposed to the stack plot
#     7. --dynamicRatioY  -> fix the range of the ratio plot depending on the values of the ratio itself instead of fixed values
# and an option to unblind only part of the DNN score spectrum:
# 	  1. --blind-range    -> do not plot the data in teh specified range 
#						  -> takes as input two numbers: MIN and MAX of range to be blinded


# PLOTS WITH BIN-WIDTH SCALING
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel1 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel2 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel3 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel4 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel6 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.0 0.5 --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel7 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.0 0.5 --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel8 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.0 0.5 --doStatSystBand


# PLOTS WITHOUT BIN-WIDTH SCALING
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel1 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel2 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel3 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel4 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel6 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel7 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel8 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --doStatSystBand


# PLOTS WITH BIN-NUMBER AS X-AXIS
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel1 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel2 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel3 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel4 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel6 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --binNXaxis --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel7 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --binNXaxis --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel8 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --binNXaxis --doStatSystBand


# PLOTS WITH BIN-NUMBER AS X-AXIS AND NO SYST BAND
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel1 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel2 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel3 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel4 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.5 1 --no-binwidth --binNXaxis
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel6 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --binNXaxis
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel7 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --binNXaxis
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel8 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --blind-range 0.0 0.5 --no-binwidth --binNXaxis

























