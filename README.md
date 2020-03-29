# KLUBAnalysis

Repo for the hh->bbtautau analysis within the LLR framework

## Instructions for Run2 Legacy Analysis
```
cmsrel CMSSW_10_2_16
cd CMSSW_10_2_16/src
cmsenv

# DNN packages
git clone git@github.com:GilesStrong/cms_hh_proc_interface.git
git clone git@github.com:GilesStrong/cms_hh_tf_inference.git
git clone git@github.com:GilesStrong/cms_runII_dnn_models.git

# KinFit and Combine packages
git clone git@github.com:llrcms/hhkinfit2.git -b bbtautau_LegacyRun2
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
git checkout v8.0.1
cd -

scram b -j8

cd HHKinFit2/
ln -ns interface include
source setup.sh
./compile.sh
cd ..

git clone git@github.com:LLRCMS/KLUBAnalysis.git
cd KLUBAnalysis
git checkout VBF_legacy

cd interface/exceptions
ln -ns ../../../HHKinFit2/interface/exceptions/HHInvMConstraintException.h
ln -ns ../../../HHKinFit2/interface/exceptions/HHEnergyRangeException.h
ln -ns ../../../HHKinFit2/interface/exceptions/HHEnergyConstraintException.h
cd -

source scripts/setup.sh
make
make exe
```


### Instructions for older releases/2016/2017 analysis:
<details>


## Instructions for 2017 Analysis 
```
cmsrel CMSSW_9_0_0
cd CMSSW_9_0_0/src
cmsenv

git clone https://github.com/bvormwald/HHKinFit2
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
git checkout 94x
cd -

scram b -j8

cd HHKinFit2/
git checkout tags/v1.1.0
ln -ns interface include
source setup.sh
./compile.sh
cd ..

git clone https://github.com/camendola/KLUBAnalysis.git
cd KLUBAnalysis
git checkout VBF2017

mkdir interface/exceptions
cd interface/exceptions
ln -ns ../../../HHKinFit2/interface/exceptions/HHInvMConstraintException.h
ln -ns ../../../HHKinFit2/interface/exceptions/HHEnergyRangeException.h
ln -ns ../../../HHKinFit2/interface/exceptions/HHEnergyConstraintException.h
cd -


source scripts/setup.sh
make
make exe
```


## New Installing CMSSW_7_4_7
```
cmsrel CMSSW_7_4_7
cd CMSSW_7_4_7/src
cmsenv

git clone https://github.com/bvormwald/HHKinFit2
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
git checkout 74x-root6
cd -
scram b -j8

cd HHKinFit2/
git checkout tags/v1.1.0
ln -ns interface include
source setup.sh
./compile.sh
cd ..

git clone https://github.com/camendola/KLUBAnalysis.git
cd KLUBAnalysis
mkdir interface/exceptions
cd interface/exceptions
ln -ns ../../../HHKinFit2/interface/exceptions/HHInvMConstraintException.h 
ln -ns ../../../HHKinFit2/interface/exceptions/HHEnergyRangeException.h
ln -ns ../../../HHKinFit2/interface/exceptions/HHEnergyConstraintException.h
cd -
cd weights
git clone https://github.com/CMS-HTT/LeptonEfficiencies HTT_SF_2016
git clone -b moriond17 https://github.com/rmanzoni/triggerSF.git tau_trigger_SF_2016
cd -
source scripts/setup.sh
make
make exe
```

## Installing
``` 
cd /data_CMS/cms/govoni/CMSSW_7_4_3/src
cmsenv
git clone https://github.com/bvormwald/HHKinFit2
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
git checkout 74x-root6
cd -
scram b -j8
ls
cd HHKinFit2/
git checkout tags/v1.1.0
ln -ns interface include
source setup.sh
./compile.sh
cd ..
git clone https://github.com/LLRCMS/KLUBAnalysis
cd KLUBAnalysis
mkdir interface/exceptions
cd interface/exceptions
ln -ns ../../../HHKinFit2/interface/exceptions/HHInvMConstraintException.h 
ln -ns ../../../HHKinFit2/interface/exceptions/HHEnergyRangeException.h
ln -ns ../../../HHKinFit2/interface/exceptions/HHEnergyConstraintException.h
cd -
cd weights
# 2015 only
# git clone https://github.com/CMS-HTT/LeptonEfficiencies.git data
# git clone https://github.com/raspereza/LepEff2016 data
git clone https://github.com/CMS-HTT/LeptonEfficiencies HTT_SF_2016
git clone -b moriond17 https://github.com/rmanzoni/triggerSF.git tau_trigger_SF_2016
cd -
source scripts/setup.sh
make
make exe
```
</details>

## How To
Please refer to:
```
https://codimd.web.cern.ch/s/HJM-lQ2AX#
```

### (Old) How To:
<details>

## SyncNtupleProducer
Specify the options (channels, samples, etc.), in:
```
config/sync/syncNtuples.cfg
```
Specify the selections for each channel in:
```
config/sync/syncNtuples_selections_XX.cut
```
Run using:
```
./bin/syncNtupleProducer.exe config/sync/syncNtuples.cfg
```
Output is located in:
```
syncNtuples
```
(output folder can be specified in config file)
 
## Big ntuples skimming

Example on how to submit a skimming.
 * the "-i" option indicates the folder where the LLR ntuples are stored
 * the "-x" option indicates the cross-section of the sample, which will be multiplied to the MC weight variable
 * jobs to be executed are saved in the folder SKIM_XXX in the current directory, where XXX is the name of the folder containing the LLR ntuples
   * this means that for a re-skimming the same folder is used, which means that the resubmit can be done only on the last skim performed

```
python scripts/skimNtuple.py  \
 -s True -c  config/skim.cfg  -o /data_CMS/cms/govoni/test_submit_to_tier3/Skims5_SS/SKIM_WJet  \
 -i /data_CMS/cms/govoni/test_submit_to_tier3/HiggsTauTauOutput_WJets_-1Events_0Skipped_1437551416.48 \
 -x 61526.7 
``` 

Example on how to check if all the jobs run successfully 
(based on the presence of the "done" file for any failures at cluster level,
and on the presence of "Error" in the log file for root problems,
which sometimes arise.
 * skim.cfg contains the main parameters to be set
 * The flag "-i" is the same as the submission job
 * The "-r" option triggers the checking, always overcomes the initial submission (but if set to "none") and by default lists the problematic jobs.
 * The "-r run" option triggers the resubmission of the jobs identified as failed
 * -s True: introduce a sleep of 0.1 s between each submission, to avoid output saving problems
 * -I True: run the inclusive skim, i.e. does not cut on number of jets
 * -v True: is verbose in the resubmit step
 * -H doit: merge all the root files with a hadd

To skim one sample:
```
python scripts/skimNtuple.py -d True -s True -o /data_CMS/cms/govoni/test_submit_to_tier3/Skims3/SKIM_Data_SingleMuon     -i /data_CMS/cms/cadamuro/test_submit_to_tier3/HiggsTauTauOutput_Data_SingleMuon_-1Events_0Skipped_1437412858.02
```
Once jobs are run to check whether there's files to be resubmitted, 
```
python scripts/skimNtuple.py -r list -d True -s True -o /data_CMS/cms/govoni/test_submit_to_tier3/Skims3/SKIM_Data_SingleMuon     -i /data_CMS/cms/cadamuro/test_submit_to_tier3/HiggsTauTauOutput_Data_SingleMuon_-1Events_0Skipped_1437412858.02
```
If there's any, actually resubmit, 
```
python scripts/skimNtuple.py -r run -d True -s True -o /data_CMS/cms/govoni/test_submit_to_tier3/Skims3/SKIM_Data_SingleMuon     -i /data_CMS/cms/cadamuro/test_submit_to_tier3/HiggsTauTauOutput_Data_SingleMuon_-1Events_0Skipped_1437412858.02
```
Once all jobs finished successfully, hadd the results (this is necessary for the analysis to run)
```
python scripts/skimNtuple.py -H doit -d True -s True -o /data_CMS/cms/govoni/test_submit_to_tier3/Skims3/SKIM_Data_SingleMuon     -i /data_CMS/cms/cadamuro/test_submit_to_tier3/HiggsTauTauOutput_Data_SingleMuon_-1Events_0Skipped_1437412858.02
```

## Plotting

Example on how to draw plots of variables already existing in the skimmed ntuples.
Plots are produced in the "plotter" folder, divided into a set with the proper normalisation
according to the XS inserted in the processing, and one with shapes, where the backgrounds
are combined according to the XS and then the total result is normalised, to compare to 
the signal in shapes.
 * the config file ```plotter_muTau.cfg``` contains all the relevant information for the run
 * an additional ```plotter_muTau.cut``` contains the selections to be applied in the plots generation

```
./bin/plotNEW.exe config/plotter_muTau.cfg 
```

The 2D plots are done with:
```
./bin/plot2D.exe config/plotter_muTau.cfg 
```
The config files for plotting are called config/plotter*cfg,
and the corresponding cut sequences are config/plotter*cut.
The request of having opposite sign candidates is in the selection sequence itself 
for the plotting from the skims.
This is not the case for the analysis sequence, though.

## MVA training

Example on how to run a MVA training on the reduced flat trees.
The TMVATraining.cpp executable uses the TMVATrainingClass to interface to the TMVA package.
 * preselections are applied according to the cfg file
 * BDT training methods available
 * more training methods could be added copyting them from the TMVATrainingClass, which unfortunately does not work properly, since it would need a artisanal root patch which we don't want to do
 
 ```
./bin/TMVATrainingZero.exe ./config/TMVATraining.cfg
```
The MVA info is then added to the SKIM tree with:
```
./bin/addTMVA.exe config/addTMVA.cfg 
```
</details>
