# DoubleHiggs

repo for the h->hh analysis within the LLR framework

## Installing
``` 
cd /data_CMS/cms/govoni/CMSSW_7_4_3/src
cmsenv
cd -
git clone git@github.com:bvormwald/HHKinFit.git
ls
cd HHKinFit/
./compile.sh
cd ..
git clone git@github.com:LLRCMS/DoubleHiggs.git
cd DoubleHiggs
source scripts/setup.sh
make
make exe
```
 
## Big ntuples skimming

Example on how to submit a skimming.
 * the "-i" option indicates the folder where the LLR ntuples are stored
 * the "-x" option indicates the cross-section of the sample, which will be multiplied to the MC weight variable
 * jobs to be executed are saved in the folder SKIM_XXX in the current directory, where XXX is the name of the folder containing the LLR ntuples
   * this means that for a re-skimming the same folder is used, which means that the resubmit can be done only on the last skim performed

```
python scripts/skimNtuple.py \
-i /data_CMS/cms/govoni/test_submit_to_tier3/HiggsTauTauOutput_DY_-1Events_0Skipped_1436202480.82 \
-x 6025.2
``` 

Example on how to check if all the jobs run successfully 
(based on the presence of the "done" file for any failures at cluster level,
and on the presence of "Error" in the log file for root problems,
which sometimes arise.
 * The flag "-i" is the same as the submission job
 * The "-r" option triggers the checking, always overcomes the initial submission (but if set to "none") and by default lists the problematic jobs.
 * The "-r run" option triggers the resubmission of the jobs identified as failed

```
python scripts/skimNtuple.py -r list -i /data_CMS/cms/govoni/test_submit_to_tier3/HiggsTauTauOutput_DY_-1Events_0Skipped_1436202480.82       
```

## Plotting

Example on how to draw plots of variables already existing in the skimmed ntuples.
 * the config file ```samples_example.cfg``` contains all the relevant information for the run
 * an additional ```samples_example.cut``` contains the selections to be applied in the plots generation

```
./bin/plotter.exe config/samples_example.cfg 
```
