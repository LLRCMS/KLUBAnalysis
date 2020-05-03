# compute stitch weights for DY jet binned + nbjets binned
# NOTE: cross-section here is put to 1 pb, so that when multiplying for xsection and lumi in the skimmer
#       one gets the number of events

from __future__ import print_function # print(text,end="")
from ROOT import *
from array import array
## update these numbers for each production!!
# Nevents = {
#     'inclusive' : 49877138,
#     '1jet'      : 65431748,
#     '2jet'      : 19695514,
#     '3jet'      : 5753813,
#     '4jet'      : 4101383,
#     '1b'        : 1464655,
#     '2b'        : 2348505
# }

# Nevents = {
#     'inclusive' : 49717683,
#     '1jet'      : 65024881,
#     '2jet'      : 19675588,
#     '3jet'      : 5753813,
#     '4jet'      : 4101383,
#     '1b'        : 1464655,
#     '2b'        : 2384424
# }

#Nevents = {
#        'inclusive' : 48644406,
#        '1jet'      : 62168246,
#        '2jet'      : 19910647,
#        '3jet'      : 5856110,
#        '4jet'      : 4178216,
#        '1b'        : 1187653,
#        '2b'        : 2514302
#    }


#Nevents = {
#    'inclusive' : 93980097,
#    '1jet'      : 34859434,
#    '2jet'      : 9701595,
#    '3jet'      : 1149467,
#    '4jet'      : 0,
#    '1b'        : 0,
#    '2b'        : 2626211
#}


#Prod datasets 2017
#Nevents = {
#    'inclusive' : 96711688,
#    '1jet'      : 34227876,
#    '2jet'      : 9497451,
#    '3jet'      : 1149467,
#    '4jet'      : 1208415,
#    '1b'        : 0,
#    '2b'        : 2695417
#}

#Prod datasets 2017 January2019production
#Nevents = {
#    'inclusive' : 97116256,
#    '1jet'      : 75626833,
#    '2jet'      : 9790490,
#    '3jet'      : 1149467,
#    '4jet'      : 0,
#    '1b'        : 0,
#    '2b'        : 5085248
#}


#Prod datasets Legacy2018, 23/01/2020
#Nevents = {
#    'inclusive' : 15076452,
#    '1jet'      : 19333314,
#    '2jet'      : 20456037,
#    '3jet'      : 5652357,
#    '4jet'      : 2817812,
#    '1b'        : 0,
#    '2b'        : 5039926
#}
#Prod datasets Legacy2016 January2020production
#Nevents = {
#    'inclusive' : 31711,
#    '1jet'      : 4183771,
#    '2jet'      : 19257179,
#    '3jet'      : 5743335,
#    '4jet'      : 3527386,
#    '1b'        : 0,
#    '2b'        : 2554303
#}
#Prod datasets Legacy2017 January2020production
Nevents = {
    'inclusive' : 97744837,
    '1jet'      : 42303958,
    '3jet'      : 6897933,
    '2b'        : 5101202
}

########################################

#fractionHistosFilesNames = {
#    'inclusive' : '../weights/DYstitchMap/DYmap_inclusive_2017_January2019.root',
#    '1jet'      : '../weights/DYstitchMap/DYmap_1jet_2017_January2019.root',
#    '2jet'      : '../weights/DYstitchMap/DYmap_2jet_2017_January2019.root',
#    '3jet'      : '../weights/DYstitchMap/DYmap_3jet_2017_January2019.root',
#    '4jet'      : '../weights/DYstitchMap/DYmap_4jet_2017.root',
#    '1b'        : '../weights/DYstitchMap/DYmap_Bjet.root',
#    '2b'        : '../weights/DYstitchMap/DYmap_BBjet_2017_January2019.root'
#}

#fractionHistosFilesNames = {
#    'inclusive' : '../weights/DYstitchMap/DYmap_inclusive_Legacy2016_27Jan2020.root',
#    '1jet'      : '../weights/DYstitchMap/DYmap_1jet_Legacy2016_27Jan2020.root',
#    '2jet'      : '../weights/DYstitchMap/DYmap_2jet_Legacy2016_27Jan2020.root',
#    '3jet'      : '../weights/DYstitchMap/DYmap_3jet_Legacy2016_27Jan2020.root',
#    '4jet'      : '../weights/DYstitchMap/DYmap_4jet_Legacy2016_27Jan2020.root',
#    '1b'        : '../weights/DYstitchMap/DYmap_Bjet.root',                       # <- dummy
#    '2b'        : '../weights/DYstitchMap/DYmap_BBjet_Legacy2016_27Jan2020.root'
#}

fractionHistosFilesNames = {
    'inclusive' : '../weights/DYstitchMap/DYincl_2017_prova.root',
    '1jet'      : '../weights/DYstitchMap/DY1jet_2017_prova.root',
    '3jet'      : '../weights/DYstitchMap/DY3jet_2017_prova.root',
    '2b'        : '../weights/DYstitchMap/DYBBjet_2017_prova.root'
}

fractionHistosFiles = {}
fractionHistos = {}
fractionHistosINT = {}

xbins  = [0.0, 1.0, 2.0, 3.0, 4.0, 5.0];
ybins  = [0.0, 1.0, 2.0, 3.0, 4.0, 5.0];
zbins  = [0.0,70.0,100.0,200.0,400.0,600.0,800.0,1200.0,2500.0,10000.0];

for MCname in fractionHistosFilesNames:
    print ("opening: " , fractionHistosFilesNames[MCname] , "for" , MCname)
    fractionHistosFiles[MCname] = TFile.Open(fractionHistosFilesNames[MCname]) 
    fractionHistosINT[MCname]   = fractionHistosFiles[MCname].Get('hINT_nJets_nBs') # isto with integer entries to avoid numerical problems in filling
    fractionHistos[MCname] = TH3D ("h_"+MCname, "h_"+MCname, 5, array('d',xbins), 5, array('d',ybins), 9, array('d',zbins))
    totInt = fractionHistosINT[MCname].Integral()
    for ix in range (1,6):
        for iy in range (1,6):
	    for iz in range (1,10):
                bincont = fractionHistosINT[MCname].GetBinContent(ix,iy,iz)
                ratio = 1.*bincont/totInt
                fractionHistos[MCname].SetBinContent(ix,iy,iz,ratio)
    print (fractionHistos[MCname].Integral())

# fraction of XS in each njet-nb bin. Almost identical to inclusive histo, but exploit njet plots to improve statistics in high njet - high nb bins
# fractionXS[njet][nb]
fractionXS =  [[[0 for nx in xrange (9)] for ny in xrange (5)] for nz in xrange (5)] 
for njet in range (0, 5):
    if (njet == 2 or njet == 4): continue
    for nb in range (0, 5):
        for ht in range (0,9):
            if nb > njet:
                fractionXS[njet][nb][ht] = 0 # by default no elems there
                # print (njet, nb, ht, "messo a 0", fractionXS[njet][nb][ht])
                # print ("SON QUI: " , njet, nb,ht)
            else:
                if njet == 0:
                    fractionXS[njet][nb][ht] = fractionHistos['inclusive'].GetBinContent(njet+1, nb+1, ht+1)
                    # print (njet, nb, ht, "messo a incl da solo a", fractionXS[njet][nb][ht])
                else:
                    inclTotInt   = fractionHistos['inclusive'].Integral()
                    inclStripInt = fractionHistos['inclusive'].Integral(njet+1, njet+1, 1, 5, 1, 9)
                    njetTotInt   = fractionHistos[str(njet)+'jet'].Integral()
                    njetStripInt = fractionHistos[str(njet)+'jet'].Integral(njet+1, njet+1, 1, 5, 1, 9)
                    if njetTotInt != njetStripInt: print ("ERROR: integral of njet: " , njet , "strip do not match", njetTotInt , njetStripInt)
                    entries = fractionHistos[str(njet)+'jet'].GetBinContent(njet+1, nb+1, ht+1)
                    fractionXS[njet][nb][ht] = 1.*(entries/njetStripInt) * (inclStripInt/inclTotInt)

#print ("###################################")
#for njet in range (0, 5):
#    for nb in range (0, 5):
#        for ht in range (0,9):
#            print (njet, nb, ht, fractionXS[njet][nb][ht])



#fOut = TFile ("../weights/DYstitchMap/outXSsplitCombined_Legacy2018_22Jan2020.root", "recreate")
#fOut = TFile ("../weights/DYstitchMap/outXSsplitCombined_2017_January2019.root", "recreate")
#fOut = TFile ("../weights/DYstitchMap/outXSsplitCombined_Legacy2016_27Jan2020.root", "recreate")
fOut = TFile ("../weights/DYstitchMap/outXSsplitCombined_Legacy2017_prova.root", "recreate")
h2DfractionXS = TH3F ("h2DfractionXS", "h2DfractionXS;njet;nb;ht", 5, array('d',xbins), 5, array('d',ybins), 9, array('d',zbins))
for njet in range (0, 5):
    for nb in range (0, 5):
        for ht in range (0,9):
            h2DfractionXS.SetBinContent(njet+1, nb+1, ht+1, fractionXS[njet][nb][ht])
print ("XS integral: " , h2DfractionXS.Integral()) ## should be 1
h2DfractionXS.Write()
for MCname in fractionHistos: fractionHistos[MCname].Write()
fOut.Close()

## in histograms, x axis = njet, y axis = nb, zaxis = ht
weights = [[[0 for nx in xrange (9)] for ny in xrange (5)] for nz in xrange (5)]
for njet in range (0, 5):
    for nb in range (0, 5):
	for ht in range (0,9):
            w = 0
            for MCname in fractionHistos:
                w += 1.*fractionHistos[MCname].GetBinContent(njet+1, nb+1, ht+1)*Nevents[MCname]
            print ("stitchWeights [%i][%i][%i] = %f ;" % (njet, nb, ht, w))
            weight = fractionXS[njet][nb][ht]/w if w > 0 else 0.
            weights[njet][nb][ht] = weight
	    
totEvts = 0
for MCname in Nevents: totEvts += Nevents[MCname]
print ("TOT EVENTS: " , totEvts)

## scale eveything to the event sum, so that it will erase the event sum in th skimmer at denominator
for njet in range (0, 5):
    print("njet: ", njet, "\n")
    for nb in range (0, 5):
       print("nb: ", nb, "\n")
       for ht in range (0,9):
            weights[njet][nb][ht] *= totEvts
	    print (weights[njet][nb][ht])
	    
####  just to test the output formatting
# weights = [
#     [10 , 20, 33, 40, 50],
#     [11 , 21, 31, 41, 51],
#     [12 , 22, 32, 42, 52],
#     [13 , 23, 33, 43, 53],
#     [14 , 24, 34, 44, 54],
#     [15 , 25, 35, 45, 55]
# ]


#print ("const float stitchWeights [][5] = {", end="\n")
#for row in range (0, len(weights)):
#    print ("    {" , end="")
#    for col in range(0, len(weights[row])):
#        text = str(weights[row][col])
#        if col != ( len(weights[row]) - 1) : text += " , "
#        print (text, end="")
#    endtext = "}"
#    if row != (len (weights)-1) : endtext += ","
#    print (endtext , end="\n")
#print ("};" , end="\n")
