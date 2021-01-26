basedir=/home/llr/cms/motta/HHLegacy/CMSSW_11_1_0_pre6/src/KLUBAnalysis
tag=/MY_impacts2018
plotter=makeDNNplots.py

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
mkdir -p ./plots/$channel/$sel1
mkdir -p ./plots/$channel/$sel2
mkdir -p ./plots/$channel/$sel3
mkdir -p ./plots/$channel/$sel4
mkdir -p ./plots/$channel/$sel5
mkdir -p ./plots/$channel/$sel6
mkdir -p ./plots/$channel/$sel7
mkdir -p ./plots/$channel/$sel8
# COPY THE index.php FILE
cp $basedir/index.php ./plots/$channel/$sel1
cp $basedir/index.php ./plots/$channel/$sel2
cp $basedir/index.php ./plots/$channel/$sel3
cp $basedir/index.php ./plots/$channel/$sel4
cp $basedir/index.php ./plots/$channel/$sel5
cp $basedir/index.php ./plots/$channel/$sel6
cp $basedir/index.php ./plots/$channel/$sel7
cp $basedir/index.php ./plots/$channel/$sel8


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

# PLOTS WITH BIN-WIDTH SCALING
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel1 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel2 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel3 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel4 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel6 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel7 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel8 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --doStatSystBand


# PLOTS WITHOUT BIN-WIDTH SCALING
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel1 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --no-binwidth --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel2 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --no-binwidth --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel3 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --no-binwidth --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel4 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --no-binwidth --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --no-binwidth --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel6 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --no-binwidth --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel7 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --no-binwidth --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel8 --channel $channel --tag $tag --label "DNN score" --quit --ratio --log --no-binwidth --doStatSystBand


# PLOTS WITH BIN-NUMBER AS X-AXIS
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel1 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --no-binwidth --binNXaxis --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel2 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --no-binwidth --binNXaxis --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel3 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --no-binwidth --binNXaxis --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel4 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --no-binwidth --binNXaxis --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --no-binwidth --binNXaxis --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel6 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --no-binwidth --binNXaxis --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel7 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --no-binwidth --binNXaxis --doStatSystBand
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel8 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --no-binwidth --binNXaxis --doStatSystBand


# PLOTS WITH BIN-NUMBER AS X-AXIS AND NO SYST BAND
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel1 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --no-binwidth --binNXaxis
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel2 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --no-binwidth --binNXaxis
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel3 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --no-binwidth --binNXaxis
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel4 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --no-binwidth --binNXaxis
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel5 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --no-binwidth --binNXaxis
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel6 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --no-binwidth --binNXaxis
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel7 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --no-binwidth --binNXaxis
python scripts/$plotter --dir $basedir/$tag/$channel --year 2018 --var DNNoutSM_kl_1 --reg $reg --sel $sel8 --channel $channel --tag $tag --label "DNN bin number" --quit --ratio --log --no-binwidth --binNXaxis

























