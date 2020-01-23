### For fit with QCD already computed, i.e. to extract only the SF
text2workspace.py datacard_QCD.txt
combine -M MultiDimFit datacard_QCD.root -P SF_both1Pr --floatOtherPOIs 0
