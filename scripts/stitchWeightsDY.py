# compute stitch weights for DY jet binned + nbjets binned
# NOTE: cross-section here is put to 1 pb, so that when multiplying for xsection and lumi in the skimmer
#       one gets the number of events

from __future__ import print_function # print(text,end="")
from ROOT import *
\

## update these numbers for each production!!

#Prod datasets Legacy2017 January2020production
Nevents = {
    'inclusive' : 97800939,
    '1jet'	: 77190729,
    '2jet'	: 10126746,
    '3jet'	: 6897933,
    '4jet'	: 4276951,
    '1b'	: 0,
    '2b'	: 5101202,
}

fractionHistosFilesNames = {
    'inclusive' : '../weights/DYstitchMap/DYmap_inclusive_Legacy2017_30Apr2020_No_HT.root',
    '1jet'      : '../weights/DYstitchMap/DYmap_1jet_Legacy2017_30Apr2020_No_HT.root',
    '2jet'      : '../weights/DYstitchMap/DYmap_2jet_Legacy2017_30Apr2020_No_HT.root',
    '3jet'      : '../weights/DYstitchMap/DYmap_3jet_Legacy2017_30Apr2020_No_HT.root',
    '4jet'      : '../weights/DYstitchMap/DYmap_4jet_Legacy2017_30Apr2020_No_HT.root',
    '1b'        : '../weights/DYstitchMap/DYmap_Bjet.root',                       # <- dummy
    '2b'        : '../weights/DYstitchMap/DYmap_BBjet_Legacy2017_30Apr2020_No_HT.root'
}


fractionHistosFiles = {}
fractionHistos = {}
fractionHistosINT = {}

for MCname in fractionHistosFilesNames:
    print ("opening: " , fractionHistosFilesNames[MCname] , "for" , MCname)
    fractionHistosFiles[MCname] = TFile.Open(fractionHistosFilesNames[MCname]) 
    fractionHistosINT[MCname]   = fractionHistosFiles[MCname].Get('hINT_nJets_nBs') # isto with integer entries to avoid numerical problems in filling
    fractionHistos[MCname] = TH2D ("h_"+MCname, "h_"+MCname, 5, 0, 5, 5, 0, 5)
    totInt = fractionHistosINT[MCname].Integral()
    for ix in range (1,6):
        for iy in range (1,6):
            bincont = fractionHistosINT[MCname].GetBinContent(ix, iy)
            ratio = 1.*bincont/totInt
            fractionHistos[MCname].SetBinContent(ix, iy, ratio)
    print (fractionHistos[MCname].Integral())

# fraction of XS in each njet-nb bin. Almost identical to inclusive histo, but exploit njet plots to improve statistics in high njet - high nb bins
# fractionXS[njet][nb]
fractionXS =  [[0]*5 for i in range (0, 5)]
for njet in range (0, 5):
    for nb in range (0, 5):
        if nb > njet:
            fractionXS[njet][nb] = 0 # by default no elems there
            # print (njet, nb, "messo a 0", fractionXS[njet][nb])
            # print ("SON QUI: " , njet, nb)
        else:
            if njet == 0:
                fractionXS[njet][nb] = fractionHistos['inclusive'].GetBinContent(njet+1, nb+1)
                # print (njet, nb, "messo a incl da solo a", fractionXS[njet][nb])
            else:
                inclTotInt   = fractionHistos['inclusive'].Integral()
                inclStripInt = fractionHistos['inclusive'].Integral(njet+1, njet+1, 1, 5)
                njetTotInt   = fractionHistos[str(njet)+'jet'].Integral()
                njetStripInt = fractionHistos[str(njet)+'jet'].Integral(njet+1, njet+1, 1, 5)
                if njetTotInt != njetStripInt: print ("ERROR: integral of njet: " , njet , "strip do not match", njetTotInt , njetStripInt)
                entries = fractionHistos[str(njet)+'jet'].GetBinContent(njet+1, nb+1)
                fractionXS[njet][nb] = 1.*(entries/njetStripInt) * (inclStripInt/inclTotInt)
                # print (njet, nb, "fatto calcolo")

        # print ("SON QUI: " , njet, nb)
        # print (njet, nb, fractionXS[njet][nb])

# print ("###################################")
# for njet in range (0, 5):
#     for nb in range (0, 5):
#         print (njet, nb, fractionXS[njet][nb])



#fOut = TFile ("../weights/DYstitchMap/outXSsplitCombined_Legacy2018_22Jan2020.root", "recreate")
#fOut = TFile ("../weights/DYstitchMap/outXSsplitCombined_2017_January2019.root", "recreate")
#fOut = TFile ("../weights/DYstitchMap/outXSsplitCombined_Legacy2016_27Jan2020.root", "recreate")
fOut = TFile ("../weights/DYstitchMap/outXSsplitCombined_Legacy2017_30Apr2020_No_HT.root", "recreate")
h2DfractionXS = TH2F ("h2DfractionXS", "h2DfractionXS;njet;nb", 5, 0, 5, 5, 0, 5)
for njet in range (0, 5):
    for nb in range (0, 5):
        h2DfractionXS.SetBinContent(njet+1, nb+1, fractionXS[njet][nb])
print ("XS integral: " , h2DfractionXS.Integral()) ## should be 1
h2DfractionXS.Write()
for MCname in fractionHistos: fractionHistos[MCname].Write()
fOut.Close()

## format to print fo copy-paste into skimmer:
# const float stitchWeights [5][5] = {11.55916, 0.316751, 0.341677, 0.362801, 0.267991} ; // weights DY stitch in njets, to be updated at each production (depend on n evts processed)

## in histograms, x axis = njet, y axis = nb
weights = []
for njet in range (0, 5):
    bweights = []
    for nb in range (0, 5):
        w = 0
        for MCname in fractionHistos:
                w += 1.*fractionHistos[MCname].GetBinContent(njet+1, nb+1)*Nevents[MCname]
        # weight = fractionHistos['inclusive'].GetBinContent(njet+1, nb+1)/w if w > 0 else 0.
        weight = fractionXS[njet][nb]/w if w > 0 else 0.
        bweights.append(weight)
        #print ("stitchWeights [%i][%i] = %f ;" % (njet, nb, weight))
    weights.append(bweights)

totEvts = 0
for MCname in Nevents: totEvts += Nevents[MCname]
print ("TOT EVENTS: " , totEvts)

## scale eveything to the event sum, so that it will erase the event sum in th skimmer at denominator
for njet in range (0, 5):
    for nb in range (0, 5):
        weights[njet][nb] *= totEvts

####  just to test the output formatting
# weights = [
#     [10 , 20, 33, 40, 50],
#     [11 , 21, 31, 41, 51],
#     [12 , 22, 32, 42, 52],
#     [13 , 23, 33, 43, 53],
#     [14 , 24, 34, 44, 54],
#     [15 , 25, 35, 45, 55]
# ]


print ("const float stitchWeights [][5] = {", end="\n")
for row in range (0, len(weights)):
    print ("    {" , end="")
    for col in range(0, len(weights[row])):
        text = str(weights[row][col])
        if col != ( len(weights[row]) - 1) : text += " , "
        print (text, end="")
    endtext = "}"
    if row != (len (weights)-1) : endtext += ","
    print (endtext , end="\n")
print ("};" , end="\n")
