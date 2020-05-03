from ROOT import *

weightFile = TFile.Open("outXSsplitCombined_Legacy2017_30Apr2020_No_HT.root")

weightHisto = weightFile.Get("h2DfractionXS")

for njet in range (0,5):

	for nb in range (0,5):
	
		if weightHisto.GetBinContent(njet+1,nb+1) == 0 : continue
	
		else : print ("njet: %i - nb: %i - XS: %f" % (njet, nb, 6077.22 * weightHisto.GetBinContent(njet+1,nb+1)))
