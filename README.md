# DoubleHiggs
repo for the h->hh analysis within the LLR framework

 * within a CMSSW version setup the environment (cmsenv)
 *  git clone git@github.com:bvormwald/HHKinFit.git
 * ls
 * cd HHKinFit/
 * ./compile.sh
 * cd ..
 * git clone git@github.com:LLRCMS/DoubleHiggs.git
 * cd DoubleHiggs
 * source scipts/setup.sh
 * make
 * make exe
 
Example of how to skim the big ntuples:

```
python scripts/skimNtuple.py -i /data_CMS/cms/govoni/test_submit_to_tier3/HiggsTauTauOutput_DY_-1Events_0Skipped_1436202480.82 -x 6025.2
```
