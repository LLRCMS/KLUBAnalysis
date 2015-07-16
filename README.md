# DoubleHiggs
repo for the h->hh analysis within the LLR framework

Within a CMSSW version setup the environment (cmsenv)
``` 
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
 
Example of how to skim the big ntuples:

```
python scripts/skimNtuple.py -i /data_CMS/cms/govoni/test_submit_to_tier3/HiggsTauTauOutput_DY_-1Events_0Skipped_1436202480.82 -x 6025.2
``` 

Example on how to check if all the jobs run successfully 
(based on the presence of the "done" file for any failures at cluster level,
and on the presence of "Error" in the log file for root problems,
which sometimes arise.
 * The folder, provided with the flag "-o", is the one created by the launch command
containing the shell scripts to be submitted.
 * The "-r" option triggers the checking, always overcomes the initial submission (but if set to "none") and by default lists the problematic jobs.
 * The "-r run" option triggers the resubmission of the jobs identified as failed

```
python scripts/skimNtuple.py -r list -o /data_CMS/cms/govoni/test_submit_to_tier3/HiggsTauTauOutput_TTJets_-1Events_0Skipped_1436202407.03       
```
