# compute stitch weights for DY jet binned + nbjets binned
# NOTE: cross-section here is put to 1 pb, so that when multiplying for xsection and lumi in the skimmer
#       one gets the number of events

from __future__ import print_function # print(text,end="")
from ROOT import *
from array import array

########################################

badInclusiveBins = [
[0,0,2],
[0,0,3],
[0,0,4],
[0,0,5],
[0,1,0],
[0,1,1],
[0,1,2],
[0,1,3],
[0,1,4],
[0,1,5],
[0,2,0],
[0,2,1],
[0,2,2],
[0,2,3],
[0,2,4],
[0,2,5],
[0,3,0],
[0,3,1],
[0,3,2],
[0,3,3],
[0,3,4],
[0,3,5],
[0,4,0],
[0,4,1],
[0,4,2],
[0,4,3],
[0,4,4],
[0,4,5],
[1,1,4],
[1,1,5],
[1,2,0],
[1,2,1],
[1,2,2],
[1,2,3],
[1,2,4],
[1,2,5],
[1,3,0],
[1,3,1],
[1,3,2],
[1,3,3],
[1,3,4],
[1,3,5],
[1,4,0],
[1,4,1],
[1,4,2],
[1,4,3],
[1,4,4],
[1,4,5],
[2,1,5],
[2,2,4],
[2,2,5],
[2,3,0],
[2,3,1],
[2,3,2],
[2,3,3],
[2,3,4],
[2,3,5],
[2,4,0],
[2,4,1],
[2,4,2],
[2,4,3],
[2,4,4],
[2,4,5],
[3,1,5],
[3,2,4],
[3,2,5],
[3,3,3],
[3,3,4],
[3,3,5],
[3,4,0],
[3,4,1],
[3,4,2],
[3,4,3],
[3,4,4],
[3,4,5],
[4,0,0],
[4,0,1],
[4,0,2],
[4,0,3],
[4,0,4],
[4,0,5],
[4,1,0],
[4,1,1],
[4,1,2],
[4,1,3],
[4,1,4],
[4,1,5],
[4,2,0],
[4,2,1],
[4,2,2],
[4,2,3],
[4,2,4],
[4,2,5],
[4,3,0],
[4,3,1],
[4,3,2],
[4,3,3],
[4,3,4],
[4,3,5],
[4,4,0],
[4,4,1],
[4,4,2],
[4,4,3],
[4,4,4],
[4,4,5],
]

good1jetBins = [
[1,1,4],
[1,1,5],
[2,2,4],
]

good2jetBins = [
[2,1,5],
[2,2,4],
[2,2,5],
[3,1,5],
[3,2,4],
[3,2,5],
[3,3,3],
[3,3,4],
]

goodPT1Bins = [
]
goodPT2Bins = [
[3,3,3], #pt2- 0.000000
]
goodPT3Bins = [
[1,1,4], #pt3- 0.000027
[2,2,4], #pt3- 0.000070
[3,2,4], #pt3- 0.000095
[3,3,3], #pt3- 0.000050
[3,3,4], #pt3- 0.000001
]
goodPT4Bins = [
[1,1,4], #pt4- 0.002119
[1,1,5], #pt4- 0.000002
[2,1,5], #pt4- 0.000080
[2,2,4], #pt4- 0.004252
[2,2,5], #pt4- 0.000024
[3,1,5], #pt4- 0.000047
[3,2,4], #pt4- 0.005275
[3,2,5], #pt4- 0.000050
[3,3,3], #pt4- 0.000004
[3,3,4], #pt4- 0.000038
]

goodPT5Bins = [
[1,1,4], #pt5- 0.000042
[1,1,5], #pt5- 0.000987
[2,1,5], #pt5- 0.009127
[2,2,4], #pt5- 0.000189
[2,2,5], #pt5- 0.003037
[3,1,5], #pt5- 0.005230
[3,2,4], #pt5- 0.000383
[3,2,5], #pt5- 0.004276
[3,3,4], #pt5- 0.000002
[3,3,5], #pt5- 0.000020
]

########################################

## update these numbers for each production!!
# Prod datasets UL18 Apr2022
Nevents = {
    'inclusive': 195155892,
    '0jet':       85731463,
    '1jet':       91640683,
    '2jet':       44735479,
    'pt1':       106080496, #'PT50To100'
    'pt2':        78318447, #'PT100To250'
    'pt3':        18640531, #'PT250To400'
    'pt4':         1892649, #'PT400To650'
    'pt5':         1906696, #'PT650ToInf'
}

fractionHistosFilesNames = {
    'inclusive': './weights/DYstitchMap/DYmap_UL_NLO_incl_2018_20220602.root',
    '0jet':      './weights/DYstitchMap/DYmap_UL_NLO_0jet_2018_20220602.root',
    '1jet':      './weights/DYstitchMap/DYmap_UL_NLO_1jet_2018_20220602.root',
    '2jet':      './weights/DYstitchMap/DYmap_UL_NLO_2jet_2018_20220602.root',
    'pt1':       './weights/DYstitchMap/DYmap_UL_NLO_PT50To100_2018_20220602.root', #'PT50To100'
    'pt2':       './weights/DYstitchMap/DYmap_UL_NLO_PT100To250_2018_20220602.root', #'PT100To250'
    'pt3':       './weights/DYstitchMap/DYmap_UL_NLO_PT250To400_2018_20220602.root', #'PT250To400'
    'pt4':       './weights/DYstitchMap/DYmap_UL_NLO_PT400To650_2018_20220602.root', #'PT400To650'
    'pt5':       './weights/DYstitchMap/DYmap_UL_NLO_PT650ToInf_2018_20220602.root', #'PT650ToInf'
}

fractionHistosFiles = {}
fractionHistos = {}
fractionHistosINT = {}

xbins  = [0.0, 1.0, 2.0, 3.0, 4.0, 5.0]
ybins  = [0.0, 1.0, 2.0, 3.0, 4.0, 5.0]
#zbins  = [0.0,70.0,100.0,200.0,400.0,600.0,800.0,1200.0,2500.0,10000.0]
zbins  = [0.0,50.,100.,250.,400.,650.,10000.]

lenX = len(xbins)-1
lenY = len(ybins)-1
lenZ = len(zbins)-1

for MCname in fractionHistosFilesNames:
    print ("Opening: " , fractionHistosFilesNames[MCname] , "for" , MCname)
    fractionHistosFiles[MCname] = TFile.Open(fractionHistosFilesNames[MCname])
    fractionHistosINT[MCname]   = fractionHistosFiles[MCname].Get('hINT_nJets_nBs_pt') # histo with integer entries to avoid numerical problems in filling
    fractionHistos[MCname] = TH3D ("h_"+MCname, "h_"+MCname, lenX, array('d',xbins), lenY, array('d',ybins), lenZ, array('d',zbins))
    totInt = fractionHistosINT[MCname].Integral()
    for ix in range (1,6):
        for iy in range (1,6):
            for iz in range (1,7):
                bincont = fractionHistosINT[MCname].GetBinContent(ix,iy,iz)
                ratio = 1.*bincont/totInt
                fractionHistos[MCname].SetBinContent(ix,iy,iz,ratio)
    print ("  Integral:", fractionHistos[MCname].Integral())


# fraction of XS in each njet-nb bin. Almost identical to inclusive histo, but exploit njet plots to improve statistics in high njet - high nb bins
# fractionXS[njet][nb][pt]
fractionXS =  [[[0 for nz in xrange(lenZ)] for ny in xrange(lenY)] for nx in xrange(lenX)]
for njet in range (0, lenX):
    for nb in range (0, lenY):
        for pt in range (0, lenZ):
            if (fractionHistos['inclusive'].GetBinContent(njet+1, nb+1, pt+1) < 1E-6): 
                print ("bad inclusive bin [%i,%i,%i]," % (njet,nb,pt))
            

                #print ("Bin (nJ, nB, PT):", njet, nb, pt)
            if nb > njet:
                # nB can't be larger than njet by construction
                fractionXS[njet][nb][pt] = 0.0
                #print (" - Empty by default (nB > njet):", fractionXS[njet][nb][pt])
            else:
                if njet == 0:
                    if pt == 0:
                        # bin (0, 0, 0) -> take from inclusive sample
                        fractionXS[njet][nb][pt] = fractionHistos['inclusive'].GetBinContent(njet+1, nb+1, pt+1)
                        #print (" - Set to inclusive value:", fractionXS[njet][nb][pt])
                    else:
                        # bins (0, 0, pt[1-8):
                        # 0 jets --> PT is 0 by construction --> no events here
                        fractionXS[njet][nb][pt] = 0.0
                        #print (" - Empty by default (PT>0 with njet==0):", fractionXS[njet][nb][pt])
                else:
                    # all other bins

                    # create bin (nj,nb,pt)
                    nJnBnPT = [njet, nb, pt]

                    # first do good bins from inclusive sample
                    if nJnBnPT not in badInclusiveBins:
                        fractionXS[njet][nb][pt] = fractionHistos['inclusive'].GetBinContent(njet+1, nb+1, pt+1)
                        #print (" - Set to inclusive value:", fractionXS[njet][nb][pt])
                    #else:
                    #    print ("checking bin [%i,%i,%i]," % (njet,nb,pt))
                    #
                    #    for MCname in fractionHistos:
                    #    
                    #        if (MCname != 'inclusive' and fractionHistos[MCname].GetBinContent(njet+1, nb+1, pt+1) > 0):
                    #    
                    #            print ("checking bin [%i,%i,%i]," % (njet,nb,pt), MCname + '- %f' % (fractionHistos[MCname].GetBinContent(njet+1, nb+1, pt+1)))
                        

                    # then do good bins from 1jets
                    elif nJnBnPT in good1jetBins:
                        inclTot   = fractionHistos['inclusive'].Integral()                                 # matrix
                        inclStrip = fractionHistos['inclusive'].Integral(njet+1, njet+1, 1, lenY, 1, lenZ) # column
                        subTot    = fractionHistos['1jet'].Integral()                                      #
                        subStrip  = fractionHistos['1jet'].Integral(njet+1, njet+1, 1, lenY, 1, lenZ)      # sub-column
                        if subTot != subStrip:
                            print ("WARNING: integral of subTot:", subTot, " do not match", subStrip, "for", nJnBnPT)
                        entries = fractionHistos['1jet'].GetBinContent(njet+1, nb+1, pt+1)                 # bin
                        fractionXS[njet][nb][pt] = 1.*(entries/subStrip)*(inclStrip/inclTot)
                        
                        #print (" - Set to 1jet value: [%i,%i,%i]" % (njet,nb,pt) , fractionXS[njet][nb][pt])
                    # then do good bins from 1jets
                    elif nJnBnPT in good2jetBins:
                        inclTot   = fractionHistos['inclusive'].Integral()                                 # matrix
                        inclStrip = fractionHistos['inclusive'].Integral(njet+1, njet+1, 1, lenY, 1, lenZ) # column
                        subTot    = fractionHistos['2jet'].Integral()                                      #
                        subStrip  = fractionHistos['2jet'].Integral(njet+1, njet+1, 1, lenY, 1, lenZ)      # sub-column
                        if subTot != subStrip:
                            print ("WARNING: integral of subTot:", subTot, " do not match", subStrip, "for", nJnBnPT)
                        entries = fractionHistos['2jet'].GetBinContent(njet+1, nb+1, pt+1)                 # bin
                        fractionXS[njet][nb][pt] = 1.*(entries/subStrip)*(inclStrip/inclTot)
                        
                        #print (" - Set to 4jet value: [%i,%i,%i]" % (njet,nb,pt) , fractionXS[njet][nb][pt])


                    # good bins from PT3 (200to400)
                    elif nJnBnPT in goodPT3Bins:
                        inclTot   = fractionHistos['inclusive'].Integral()                             # matrix
                        inclStrip = fractionHistos['inclusive'].Integral(1, lenX, 1, lenY, pt+1, pt+1) # column
                        subTot    = fractionHistos['pt3'].Integral()                                   #
                        subStrip  = fractionHistos['pt3'].Integral(1, lenX, 1, lenY, pt+1, pt+1)       # sub-column
                        if abs(subTot - subStrip) > 0.0001:
                            print ("WARNING: integral of subTot:", subTot, " do not match", subStrip, "for", nJnBnPT)
                        entries = fractionHistos['pt3'].GetBinContent(njet+1, nb+1, pt+1)              # bin
                        fractionXS[njet][nb][pt] = 1.*(entries/subStrip)*(inclStrip/inclTot)
                        #print (" - Set to pt4 value:", fractionXS[njet][nb][pt])

                    # good bins from PT4 (400to600)
                    elif nJnBnPT in goodPT4Bins:
                        inclTot   = fractionHistos['inclusive'].Integral()                             # matrix
                        inclStrip = fractionHistos['inclusive'].Integral(1, lenX, 1, lenY, pt+1, pt+1) # column
                        subTot    = fractionHistos['pt4'].Integral()                                   #
                        subStrip  = fractionHistos['pt4'].Integral(1, lenX, 1, lenY, pt+1, pt+1)       # sub-column
                        if abs(subTot - subStrip) > 0.0001:
                            print ("WARNING: integral of subTot:", subTot, " do not match", subStrip, "for", nJnBnPT)
                        entries = fractionHistos['pt4'].GetBinContent(njet+1, nb+1, pt+1)              # bin
                        fractionXS[njet][nb][pt] = 1.*(entries/subStrip)*(inclStrip/inclTot)
                        #print (" - Set to pt4 value:", fractionXS[njet][nb][pt])

                    # good bins from PT5 (600to800)
                    elif nJnBnPT in goodPT5Bins:
                        inclTot   = fractionHistos['inclusive'].Integral()                             # matrix
                        inclStrip = fractionHistos['inclusive'].Integral(1, lenX, 1, lenY, pt+1, pt+1) # column
                        subTot    = fractionHistos['pt5'].Integral()                                   #
                        subStrip  = fractionHistos['pt5'].Integral(1, lenX, 1, lenY, pt+1, pt+1)       # sub-column
                        if abs(subTot - subStrip) > 0.0001:
                            print ("WARNING: integral of subTot:", subTot, " do not match", subStrip, "for", nJnBnPT)
                        entries = fractionHistos['pt5'].GetBinContent(njet+1, nb+1, pt+1)              # bin
                        fractionXS[njet][nb][pt] = 1.*(entries/subStrip)*(inclStrip/inclTot)
                        #print (" - Set to pt5 value:", fractionXS[njet][nb][pt])

                    #  remanining bins
                    else:
                        fractionXS[njet][nb][pt] = fractionHistos['inclusive'].GetBinContent(njet+1, nb+1, pt+1)
                        #print (" - Bad bin, set to inclusive value:", fractionXS[njet][nb][pt])


#print ("###################################")
#for njet in range (0, lenX):
#    for nb in range (0, lenY):
#        for pt in range (0,lenZ):
#            print (njet, nb, pt, fractionXS[njet][nb][pt])

#fOut = TFile ("../weights/DYstitchMap/outXSsplitCombined_Legacy2018_07May2020.root", "recreate")
fOut = TFile ("./weights/DYstitchMap/outXSsplitCombined_NLO_UL2018_02Jun2022.root", "recreate")
h3DfractionXS = TH3F ("h3DfractionXS", "h3DfractionXS;njet;nb;pt", lenX, array('d',xbins), lenY, array('d',ybins), lenZ, array('d',zbins))
for njet in range (0,lenX):
    for nb in range (0,lenY):
        for pt in range (0,lenZ):
            h3DfractionXS.SetBinContent(njet+1, nb+1, pt+1, fractionXS[njet][nb][pt])
print ("XS integral: " , h3DfractionXS.Integral()) ## should be 1
h3DfractionXS.Write()
for MCname in fractionHistos: fractionHistos[MCname].Write()
fOut.Close()

#import pdb; pdb.set_trace()
#exit()

## in histograms, x axis = njet, y axis = nb, zaxis = pt
weights = [[[0 for nz in xrange(lenZ)] for ny in xrange(lenY)] for nx in xrange(lenX)]
for njet in range (0, lenX):
    for nb in range (0, lenY):
        for pt in range (0,lenZ):
            w = 0
            for MCname in fractionHistos:
                w += 1.*fractionHistos[MCname].GetBinContent(njet+1, nb+1, pt+1)*Nevents[MCname]
            #print ("stitchWeights [%i][%i][%i] = %f ;" % (njet, nb, pt, w))
            weight = fractionXS[njet][nb][pt]/w if w > 0 else 0.
            weights[njet][nb][pt] = weight

totEvts = 0
for MCname in Nevents: totEvts += Nevents[MCname]
print ("TOT EVENTS: " , totEvts)

## scale eveything to the event sum, so that it will erase the event sum in th skimmer at denominator
for njet in range (0,lenX):
    #print("njet:", njet)
    for nb in range (0,lenY):
        #print(" nb:", nb)
        for pt in range (0,lenZ):
            weights[njet][nb][pt] *= totEvts
            #print ("  pt:", pt, "weight:", weights[njet][nb][pt])


####  printout to be copied in skimNtuple*.cpp
print("Copy the following table to skimNtuple*.cpp: \n")
print ("const float stitchWeights [5][5][6] = {", end="\n")
for nj in range (0, len(weights)):
    print (" {" , end="\n")
    for nb in range(0, len(weights[nj])):
        print ("  {" , end="")
        for pt in range(0, len(weights[nj][nb])):
            text = str(weights[nj][nb][pt])
            if pt != ( len(weights[nj][nb])-1 ) : text += " , "
            print (text, end="")
        endlinetext = "}"
        if nb != ( len(weights[nj])-1 ) : endlinetext += ","
        print (endlinetext, end="\n")
    endtext = " }"
    if nj != ( len(weights)-1 ) : endtext += ","
    print (endtext, end="\n")
print("};" , end="\n")
