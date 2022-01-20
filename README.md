# KLUBAnalysis

Repo for the hh->bbtautau analysis within the LLR framework

## Instructions for Run2 Legacy Analysis
```
cmsrel CMSSW_11_1_0_pre6
cd CMSSW_11_1_0_pre6/src
cmsenv

# DNN packages
git clone git@github.com:GilesStrong/cms_hh_proc_interface.git
cd cms_hh_proc_interface
git checkout tags/V4.0
cd -
git clone git@github.com:GilesStrong/cms_hh_tf_inference.git
git clone git@github.com:GilesStrong/cms_runII_dnn_models.git
cd cms_runII_dnn_models/models/test/
mv test.cc test.cc_x
cd -

# Multiclassifier
git clone https://gitlab.cern.ch/hh/bbtautau/MulticlassInference.git

# HHbtag package
git clone git@github.com:hh-italian-group/HHbtag.git HHTools/HHbtag

# KinFit and Combine packages
mkdir HHKinFit2
cd HHKinFit2
git clone git@github.com:jonamotta/HHKinFit2.git -b bbtautau_Run2DNNtraining
cd ..
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
git checkout v8.2.0
cd -

# SVfit packages
git clone https://github.com/LLRCMS/ClassicSVfit.git TauAnalysis/ClassicSVfit -b bbtautau_LegacyRun2
git clone https://github.com/svfit/SVfitTF TauAnalysis/SVfitTF

scram b -j 12

git clone git@github.com:LLRCMS/KLUBAnalysis.git
cd KLUBAnalysis
git checkout VBF_legacy

cd interface/exceptions
ln -ns ../../../HHKinFit2/HHKinFit2/interface/exceptions/HHInvMConstraintException.h
ln -ns ../../../HHKinFit2/HHKinFit2/interface/exceptions/HHEnergyRangeException.h
ln -ns ../../../HHKinFit2/HHKinFit2/interface/exceptions/HHEnergyConstraintException.h
cd -

source scripts/setup.sh
make -j 10
make exe -j 10
```