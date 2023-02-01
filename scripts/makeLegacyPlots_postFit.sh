basedir=/home/llr/cms/motta/HHLegacy/CMSSW_11_1_0_pre6/src/KLUBAnalysis
tag=fd__hesse__n200__unblinded__v6

year=2016

reg=SR

ch=TauTau
#ch=MuTau
#ch=ETau

sel1=s1b1jresolvedMcut
sel2=s2b0jresolvedMcut
sel3=sboostedLLMcut
sel4=GGFclass
sel5=VBFclass
sel6=DYclass
sel7=ttHclass
sel8=TTclass

var=DNN

mkdir -p ./LegacyPlots/Legacy$year/$ch\_$tag\_prefit/$sel1
mkdir -p ./LegacyPlots/Legacy$year/$ch\_$tag\_prefit/$sel2
mkdir -p ./LegacyPlots/Legacy$year/$ch\_$tag\_prefit/$sel3
mkdir -p ./LegacyPlots/Legacy$year/$ch\_$tag\_prefit/$sel4
mkdir -p ./LegacyPlots/Legacy$year/$ch\_$tag\_prefit/$sel5
mkdir -p ./LegacyPlots/Legacy$year/$ch\_$tag\_prefit/$sel6
mkdir -p ./LegacyPlots/Legacy$year/$ch\_$tag\_prefit/$sel7
mkdir -p ./LegacyPlots/Legacy$year/$ch\_$tag\_prefit/$sel8

mkdir -p ./LegacyPlots/Legacy$year/$ch\_$tag\_postfit/$sel1
mkdir -p ./LegacyPlots/Legacy$year/$ch\_$tag\_postfit/$sel2
mkdir -p ./LegacyPlots/Legacy$year/$ch\_$tag\_postfit/$sel3
mkdir -p ./LegacyPlots/Legacy$year/$ch\_$tag\_postfit/$sel4
mkdir -p ./LegacyPlots/Legacy$year/$ch\_$tag\_postfit/$sel5
mkdir -p ./LegacyPlots/Legacy$year/$ch\_$tag\_postfit/$sel6
mkdir -p ./LegacyPlots/Legacy$year/$ch\_$tag\_postfit/$sel7
mkdir -p ./LegacyPlots/Legacy$year/$ch\_$tag\_postfit/$sel8

cp $basedir/index.php ./LegacyPlots/Legacy$year/$ch\_$tag\_prefit/$sel1
cp $basedir/index.php ./LegacyPlots/Legacy$year/$ch\_$tag\_prefit/$sel2
cp $basedir/index.php ./LegacyPlots/Legacy$year/$ch\_$tag\_prefit/$sel3
cp $basedir/index.php ./LegacyPlots/Legacy$year/$ch\_$tag\_prefit/$sel4
cp $basedir/index.php ./LegacyPlots/Legacy$year/$ch\_$tag\_prefit/$sel5
cp $basedir/index.php ./LegacyPlots/Legacy$year/$ch\_$tag\_prefit/$sel6
cp $basedir/index.php ./LegacyPlots/Legacy$year/$ch\_$tag\_prefit/$sel7
cp $basedir/index.php ./LegacyPlots/Legacy$year/$ch\_$tag\_prefit/$sel8

cp $basedir/index.php ./LegacyPlots/Legacy$year/$ch\_$tag\_postfit/$sel1
cp $basedir/index.php ./LegacyPlots/Legacy$year/$ch\_$tag\_postfit/$sel2
cp $basedir/index.php ./LegacyPlots/Legacy$year/$ch\_$tag\_postfit/$sel3
cp $basedir/index.php ./LegacyPlots/Legacy$year/$ch\_$tag\_postfit/$sel4
cp $basedir/index.php ./LegacyPlots/Legacy$year/$ch\_$tag\_postfit/$sel5
cp $basedir/index.php ./LegacyPlots/Legacy$year/$ch\_$tag\_postfit/$sel6
cp $basedir/index.php ./LegacyPlots/Legacy$year/$ch\_$tag\_postfit/$sel7
cp $basedir/index.php ./LegacyPlots/Legacy$year/$ch\_$tag\_postfit/$sel8


#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel1 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --ratiosig --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel2 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --ratiosig --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel3 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --ratiosig --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel4 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --ratiosig --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel5 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --ratiosig --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel6 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --ratiosig --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel7 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --ratiosig --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel8 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --ratiosig --binwidth

#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel1 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --ratiosig --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel2 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --ratiosig --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel3 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --ratiosig --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel4 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --ratiosig --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel5 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --ratiosig --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel6 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --ratiosig --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel7 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --ratiosig --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel8 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --ratiosig --binwidth

#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel1 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel2 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel3 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel4 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel5 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel6 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel7 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel8 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --binwidth

#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel1 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel2 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel3 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel4 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel5 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel6 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel7 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --binwidth
#python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel8 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --binwidth

python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel1 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --ratiosig
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel2 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --ratiosig
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel3 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --ratiosig
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel4 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --ratiosig
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel5 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --ratiosig
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel6 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --ratiosig
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel7 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --ratiosig
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel8 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log --ratiosig

python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel1 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --ratiosig
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel2 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --ratiosig
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel3 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --ratiosig
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel4 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --ratiosig
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel5 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --ratiosig
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel6 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --ratiosig
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel7 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --ratiosig
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel8 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log --ratiosig

python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel1 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel2 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel3 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel4 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel5 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel6 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel7 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel8 --var $var --tag $tag --reg $reg --prepost postfit --quit --ratio --log

python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel1 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel2 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel3 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel4 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel5 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel6 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel7 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log
python scripts/makeLegacyPlots_postFit.py --postFitFile fd__hesse__n200__unblinded__v6.root --dir $basedir --year $year --channel $ch --sel $sel8 --var $var --tag $tag --reg $reg --prepost prefit --quit --ratio --log