==== How to create doubleHiggs datacards ===
0. Have histograms prepared from evalQCD and the config
0. System requirements: CMSSW_7_4_7 + KLUBAnalysis + CombineHarvester + HiggsAnalysis/Combine (see twikis for versions)
1. If you need shape uncertainties, some processes have to be renamed. 
   python wrapperHisto.py -i input -o output. (Bonus: introduce empty bin protection)
2. Set the variables you need to control in makeCategories.sh:
   OUTSTRING: (output tag)
   SELECTIONS: The name of the selections as per mainCfg.cfg. DO NOT include BDT related strings as lmr70 etc. These are already taken care of
   LEPTONS: Remove the channels you don't want to use
   Adjust the paths in the line starting with "python chcardMaker.py". Remove "-y" to not do bin-by-bin systematics, use -u 0 to remove shapes uncertainties
3. edit the BDT strings in launchWrapper.sh 
4. ./launchWrapper.sh
5. Results can be plotted using 
   python plotSimple.py [-x benchmark -c -o -b]


==== Note on systematics ===
Log-normal Systematics are defined in config/systematics*cfg and read via systReader.py
Name of the processes in the config file should match those in mainCfg.cfg. If not, add manually the relevant names in ALL the syst*cfg files

Shape systematics are added in chcardMaker.py Look for the line with "AddSyst"
Names should match those in the wrapperHisto.py macro

Bin-by-bin is performed autmatically if chcardMaker is ran with the "-y" option
