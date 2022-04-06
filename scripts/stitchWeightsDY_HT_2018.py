# compute stitch weights for DY jet binned + nbjets binned
# NOTE: cross-section here is put to 1 pb, so that when multiplying for xsection and lumi in the skimmer
#       one gets the number of events

from __future__ import print_function # print(text,end="")
from ROOT import *
from array import array

########################################

badInclusiveBins = [
[0,0,1],
[0,0,2],
[0,0,3],
[0,0,4],
[0,0,5],
[0,0,6],
[0,0,7],
[0,0,8],
[0,1,0],
[0,1,1],
[0,1,2],
[0,1,3],
[0,1,4],
[0,1,5],
[0,1,6],
[0,1,7],
[0,1,8],
[0,2,0],
[0,2,1],
[0,2,2],
[0,2,3],
[0,2,4],
[0,2,5],
[0,2,6],
[0,2,7],
[0,2,8],
[0,3,0],
[0,3,1],
[0,3,2],
[0,3,3],
[0,3,4],
[0,3,5],
[0,3,6],
[0,3,7],
[0,3,8],
[0,4,0],
[0,4,1],
[0,4,2],
[0,4,3],
[0,4,4],
[0,4,5],
[0,4,6],
[0,4,7],
[0,4,8],
[1,0,5],
[1,0,6],
[1,0,7],
[1,0,8],
[1,1,4],
[1,1,5],
[1,1,6],
[1,1,7],
[1,1,8],
[1,2,0],
[1,2,1],
[1,2,2],
[1,2,3],
[1,2,4],
[1,2,5],
[1,2,6],
[1,2,7],
[1,2,8],
[1,3,0],
[1,3,1],
[1,3,2],
[1,3,3],
[1,3,4],
[1,3,5],
[1,3,6],
[1,3,7],
[1,3,8],
[1,4,0],
[1,4,1],
[1,4,2],
[1,4,3],
[1,4,4],
[1,4,5],
[1,4,6],
[1,4,7],
[1,4,8],
[2,0,7],
[2,0,8],
[2,1,5],
[2,1,6],
[2,1,7],
[2,1,8],
[2,2,4],
[2,2,5],
[2,2,6],
[2,2,7],
[2,2,8],
[2,3,0],
[2,3,1],
[2,3,2],
[2,3,3],
[2,3,4],
[2,3,5],
[2,3,6],
[2,3,7],
[2,3,8],
[2,4,0],
[2,4,1],
[2,4,2],
[2,4,3],
[2,4,4],
[2,4,5],
[2,4,6],
[2,4,7],
[2,4,8],
[3,0,8],
[3,1,6],
[3,1,7],
[3,1,8],
[3,2,5],
[3,2,6],
[3,2,7],
[3,2,8],
[3,3,3],
[3,3,4],
[3,3,5],
[3,3,6],
[3,3,7],
[3,3,8],
[3,4,0],
[3,4,1],
[3,4,2],
[3,4,3],
[3,4,4],
[3,4,5],
[3,4,6],
[3,4,7],
[3,4,8],
[4,0,8],
[4,1,0],
[4,1,7],
[4,1,8],
[4,2,7],
[4,2,8],
[4,3,0],
[4,3,1],
[4,3,4],
[4,3,5],
[4,3,6],
[4,3,7],
[4,3,8],
[4,4,0],
[4,4,1],
[4,4,2],
[4,4,3],
[4,4,4],
[4,4,5],
[4,4,6],
[4,4,7],
[4,4,8],
]

good1jetBins = [
]

good2jetBins = [
]

good4jetBins = [
[4,1,0],
[4,3,0],
[4,3,1],
[4,4,0],
[4,4,1],
[4,4,2],
]

goodHT1Bins = [
]
goodHT2Bins = [
]
goodHT3Bins = [
[3,3,3],
[4,4,3],
]
goodHT4Bins = [
[2,2,4],
[3,3,4],
[4,3,4],
[4,4,4],
]

goodHT5Bins = [
[1,0,5],
[1,1,5],
[2,1,5],
[2,2,5],
[3,2,5],
[3,3,5],
[4,3,5],
[4,4,5],
]

goodHT6Bins = [
[1,0,6],
[1,1,6],
[2,1,6],
[2,2,6],
[3,1,6],
[3,2,6],
[3,3,6],
[4,3,6],
[4,4,6],
]

goodHT7Bins = [
[1,0,7],
[1,1,7],
[2,0,7],
[2,1,7],
[2,2,7],
[3,1,7],
[3,2,7],
[3,3,7],
[4,1,7],
[4,2,7],
[4,3,7],
[4,4,7],
]

goodHT8Bins = [
[2,0,8],
[2,1,8],
[2,2,8],
[3,0,8],
[3,1,8],
[3,2,8],
[3,3,8],
[4,0,8],
[4,1,8],
[4,2,8],
[4,3,8],
[4,4,8],
]

########################################

## update these numbers for each production!!
# Prod datasets UL18 Apr2022
Nevents = {
    'inclusive'    : 95873931,
    '1jet'         : 60114650,
    '2jet'         : 24003549,
    #'3jet'         :,
    '4jet'         : 8885353,
    #'1b'           : ,
#    '2b'           : 6,
    'ht1'          : 17004433, #'ht70to100'
    'ht2'          : 26202328, #'ht100to200'
    'ht3'          : 18455718, #'ht200to400'
    'ht4'          : 6364488,  #'ht400to600'
    'ht5'          : 7035971,  #'ht600to800'
    'ht6'          : 6678036,  #'ht800to1200'
    'ht7'          : 6139829,  #'ht1200to2500'
    'ht8'          : 1978203   #'ht2500toInf'
}

fractionHistosFilesNames = {
    'inclusive' : './weights/DYstitchMap/DYmap_UL_incl_2018_20220404.root',
    '1jet'      : './weights/DYstitchMap/DYmap_UL_1jet_2018_20220404.root',
    '2jet'      : './weights/DYstitchMap/DYmap_UL_2jet_2018_20220404.root',
    #'3jet'      : './weights/DYstitchMap/DYmap_UL_3jet_2018_20220404.root',
    '4jet'      : './weights/DYstitchMap/DYmap_UL_4jet_2018_20220404.root',
    #'1b'        : './weights/DYstitchMap/DYmap_UL_Bjet_2018_20220404.root',
    #'2b'        : './weights/DYstitchMap/DYmap_UL_BBjet_2018_20220404.root',
    'ht1'       : './weights/DYstitchMap/DYmap_UL_HT70to100_2018_20220404.root',     # 'ht70to100'
    'ht2'       : './weights/DYstitchMap/DYmap_UL_HT100to200_2018_20220404.root',    # 'ht100to200'
    'ht3'       : './weights/DYstitchMap/DYmap_UL_HT200to400_2018_20220404.root',    # 'ht200to400'
    'ht4'       : './weights/DYstitchMap/DYmap_UL_HT400to600_2018_20220404.root',    # 'ht400to600'
    'ht5'       : './weights/DYstitchMap/DYmap_UL_HT600to800_2018_20220404.root',    # 'ht600to800'
    'ht6'       : './weights/DYstitchMap/DYmap_UL_HT800to1200_2018_20220404.root',   # 'ht800to1200'
    'ht7'       : './weights/DYstitchMap/DYmap_UL_HT1200to2500_2018_20220404.root',  # 'ht1200to2500'
    'ht8'       : './weights/DYstitchMap/DYmap_UL_HT2500toInf_2018_20220404.root'    # 'ht2500toInf'
}

fractionHistosFiles = {}
fractionHistos = {}
fractionHistosINT = {}

xbins  = [0.0, 1.0, 2.0, 3.0, 4.0, 5.0]
ybins  = [0.0, 1.0, 2.0, 3.0, 4.0, 5.0]
zbins  = [0.0,70.0,100.0,200.0,400.0,600.0,800.0,1200.0,2500.0,10000.0]

lenX = len(xbins)-1
lenY = len(ybins)-1
lenZ = len(zbins)-1

for MCname in fractionHistosFilesNames:
    print ("Opening: " , fractionHistosFilesNames[MCname] , "for" , MCname)
    fractionHistosFiles[MCname] = TFile.Open(fractionHistosFilesNames[MCname])
    fractionHistosINT[MCname]   = fractionHistosFiles[MCname].Get('hINT_nJets_nBs_ht') # histo with integer entries to avoid numerical problems in filling
    fractionHistos[MCname] = TH3D ("h_"+MCname, "h_"+MCname, lenX, array('d',xbins), lenY, array('d',ybins), lenZ, array('d',zbins))
    totInt = fractionHistosINT[MCname].Integral()
    for ix in range (1,6):
        for iy in range (1,6):
            for iz in range (1,10):
                bincont = fractionHistosINT[MCname].GetBinContent(ix,iy,iz)
                ratio = 1.*bincont/totInt
                fractionHistos[MCname].SetBinContent(ix,iy,iz,ratio)
    print ("  Integral:", fractionHistos[MCname].Integral())


# fraction of XS in each njet-nb bin. Almost identical to inclusive histo, but exploit njet plots to improve statistics in high njet - high nb bins
# fractionXS[njet][nb][ht]
fractionXS =  [[[0 for nz in xrange(lenZ)] for ny in xrange(lenY)] for nx in xrange(lenX)]
for njet in range (0, lenX):
    for nb in range (0, lenY):
        for ht in range (0, lenZ):
            #if (fractionHistos['inclusive'].GetBinContent(njet+1, nb+1, ht+1) < 1E-6): 
            #    print ("bad inclusive bin [%i,%i,%i]," % (njet,nb,ht))
            

                #print ("Bin (nJ, nB, HT):", njet, nb, ht)
            if nb > njet:
                # nB can't be larger than njet by construction
                fractionXS[njet][nb][ht] = 0.0
                #print (" - Empty by default (nB > njet):", fractionXS[njet][nb][ht])
            else:
                if njet == 0:
                    if ht == 0:
                        # bin (0, 0, 0) -> take from inclusive sample
                        fractionXS[njet][nb][ht] = fractionHistos['inclusive'].GetBinContent(njet+1, nb+1, ht+1)
                        #print (" - Set to inclusive value:", fractionXS[njet][nb][ht])
                    else:
                        # bins (0, 0, ht[1-8):
                        # 0 jets --> HT is 0 by construction --> no events here
                        fractionXS[njet][nb][ht] = 0.0
                        #print (" - Empty by default (HT>0 with njet==0):", fractionXS[njet][nb][ht])
                else:
                    # all other bins

                    # create bin (nj,nb,ht)
                    nJnBnHT = [njet, nb, ht]

                    # first do good bins from inclusive sample
                    if nJnBnHT not in badInclusiveBins:
                        fractionXS[njet][nb][ht] = fractionHistos['inclusive'].GetBinContent(njet+1, nb+1, ht+1)
                        #print (" - Set to inclusive value:", fractionXS[njet][nb][ht])
                    #else:
                        #print ("checking bin [%i,%i,%i]," % (njet,nb,ht))

                        #for MCname in fractionHistos:
                        #
                        #    if (MCname != 'inclusive' and fractionHistos[MCname].GetBinContent(njet+1, nb+1, ht+1) > 0):
                        #
                        #        print ("checking bin [%i,%i,%i]," % (njet,nb,ht), MCname + '- %f' % (fractionHistos[MCname].GetBinContent(njet+1, nb+1, ht+1)))
                        

                    # then do good bins from 1jets
                    elif nJnBnHT in good1jetBins:
                        inclTot   = fractionHistos['inclusive'].Integral()                                 # matrix
                        inclStrip = fractionHistos['inclusive'].Integral(njet+1, njet+1, 1, lenY, 1, lenZ) # column
                        subTot    = fractionHistos['1jet'].Integral()                                      #
                        subStrip  = fractionHistos['1jet'].Integral(njet+1, njet+1, 1, lenY, 1, lenZ)      # sub-column
                        if subTot != subStrip:
                            print ("WARNING: integral of subTot:", subTot, " do not match", subStrip, "for", nJnBnHT)
                        entries = fractionHistos['1jet'].GetBinContent(njet+1, nb+1, ht+1)                 # bin
                        fractionXS[njet][nb][ht] = 1.*(entries/subStrip)*(inclStrip/inclTot)
                        
                        #print (" - Set to 1jet value: [%i,%i,%i]" % (njet,nb,ht) , fractionXS[njet][nb][ht])
                    # then do good bins from 1jets
                    elif nJnBnHT in good4jetBins:
                        inclTot   = fractionHistos['inclusive'].Integral()                                 # matrix
                        inclStrip = fractionHistos['inclusive'].Integral(njet+1, njet+1, 1, lenY, 1, lenZ) # column
                        subTot    = fractionHistos['4jet'].Integral()                                      #
                        subStrip  = fractionHistos['4jet'].Integral(njet+1, njet+1, 1, lenY, 1, lenZ)      # sub-column
                        if subTot != subStrip:
                            print ("WARNING: integral of subTot:", subTot, " do not match", subStrip, "for", nJnBnHT)
                        entries = fractionHistos['4jet'].GetBinContent(njet+1, nb+1, ht+1)                 # bin
                        fractionXS[njet][nb][ht] = 1.*(entries/subStrip)*(inclStrip/inclTot)
                        
                        #print (" - Set to 4jet value: [%i,%i,%i]" % (njet,nb,ht) , fractionXS[njet][nb][ht])


                    # then do good bins from 2B
#                    elif nJnBnHT in good2BBins:
#                        inclTot   = fractionHistos['inclusive'].Integral()                                 # matrix
#                        inclStrip = fractionHistos['inclusive'].Integral(1, lenX, nb+1, nb+1, 1, lenZ) # column
#                        subTot    = fractionHistos['2b'].Integral()                                        #
#                        subStrip  = fractionHistos['2b'].Integral(1, lenX, nb+1, nb+1, 1, lenZ)        # sub-column
#                        if subTot != subStrip:
#                            print ("WARNING: integral of subTot:", subTot, " do not match", subStrip, "for", nJnBnHT)
#                        entries = fractionHistos['2b'].GetBinContent(njet+1, nb+1, ht+1)                   # bin
#                        fractionXS[njet][nb][ht] = 1.*(entries/subStrip)*(inclStrip/inclTot)
#                        #print (" - Set to 2b value:", fractionXS[njet][nb][ht])

                    # good bins from HT3 (200to400)
                    elif nJnBnHT in goodHT3Bins:
                        inclTot   = fractionHistos['inclusive'].Integral()                             # matrix
                        inclStrip = fractionHistos['inclusive'].Integral(1, lenX, 1, lenY, ht+1, ht+1) # column
                        subTot    = fractionHistos['ht3'].Integral()                                   #
                        subStrip  = fractionHistos['ht3'].Integral(1, lenX, 1, lenY, ht+1, ht+1)       # sub-column
                        if abs(subTot - subStrip) > 0.0001:
                            print ("WARNING: integral of subTot:", subTot, " do not match", subStrip, "for", nJnBnHT)
                        entries = fractionHistos['ht3'].GetBinContent(njet+1, nb+1, ht+1)              # bin
                        fractionXS[njet][nb][ht] = 1.*(entries/subStrip)*(inclStrip/inclTot)
                        #print (" - Set to ht4 value:", fractionXS[njet][nb][ht])

                    # good bins from HT4 (400to600)
                    elif nJnBnHT in goodHT4Bins:
                        inclTot   = fractionHistos['inclusive'].Integral()                             # matrix
                        inclStrip = fractionHistos['inclusive'].Integral(1, lenX, 1, lenY, ht+1, ht+1) # column
                        subTot    = fractionHistos['ht4'].Integral()                                   #
                        subStrip  = fractionHistos['ht4'].Integral(1, lenX, 1, lenY, ht+1, ht+1)       # sub-column
                        if abs(subTot - subStrip) > 0.0001:
                            print ("WARNING: integral of subTot:", subTot, " do not match", subStrip, "for", nJnBnHT)
                        entries = fractionHistos['ht4'].GetBinContent(njet+1, nb+1, ht+1)              # bin
                        fractionXS[njet][nb][ht] = 1.*(entries/subStrip)*(inclStrip/inclTot)
                        #print (" - Set to ht4 value:", fractionXS[njet][nb][ht])

                    # good bins from HT5 (600to800)
                    elif nJnBnHT in goodHT5Bins:
                        inclTot   = fractionHistos['inclusive'].Integral()                             # matrix
                        inclStrip = fractionHistos['inclusive'].Integral(1, lenX, 1, lenY, ht+1, ht+1) # column
                        subTot    = fractionHistos['ht5'].Integral()                                   #
                        subStrip  = fractionHistos['ht5'].Integral(1, lenX, 1, lenY, ht+1, ht+1)       # sub-column
                        if abs(subTot - subStrip) > 0.0001:
                            print ("WARNING: integral of subTot:", subTot, " do not match", subStrip, "for", nJnBnHT)
                        entries = fractionHistos['ht5'].GetBinContent(njet+1, nb+1, ht+1)              # bin
                        fractionXS[njet][nb][ht] = 1.*(entries/subStrip)*(inclStrip/inclTot)
                        #print (" - Set to ht5 value:", fractionXS[njet][nb][ht])

                    # good bins from HT6 (800to1200)
                    elif nJnBnHT in goodHT6Bins:
                        inclTot   = fractionHistos['inclusive'].Integral()                             # matrix
                        inclStrip = fractionHistos['inclusive'].Integral(1, lenX, 1, lenY, ht+1, ht+1) # column
                        subTot    = fractionHistos['ht6'].Integral()                                   #
                        subStrip  = fractionHistos['ht6'].Integral(1, lenX, 1, lenY, ht+1, ht+1)       # sub-column
                        if abs(subTot - subStrip) > 0.0001:
                            print ("WARNING: integral of subTot:", subTot, " do not match", subStrip, "for", nJnBnHT)
                        entries = fractionHistos['ht6'].GetBinContent(njet+1, nb+1, ht+1)              # bin
                        fractionXS[njet][nb][ht] = 1.*(entries/subStrip)*(inclStrip/inclTot)
                        #print (" - Set to ht6 value:", fractionXS[njet][nb][ht])

                    # good bins from HT7 (1200to2500)
                    elif nJnBnHT in goodHT7Bins:
                        inclTot   = fractionHistos['inclusive'].Integral()                             # matrix
                        inclStrip = fractionHistos['inclusive'].Integral(1, lenX, 1, lenY, ht+1, ht+1) # column
                        subTot    = fractionHistos['ht7'].Integral()                                   #
                        subStrip  = fractionHistos['ht7'].Integral(1, lenX, 1, lenY, ht+1, ht+1)       # sub-column
                        if abs(subTot - subStrip) > 0.0001:
                            print ("WARNING: integral of subTot:", subTot, " do not match", subStrip, "for", nJnBnHT)
                        entries = fractionHistos['ht7'].GetBinContent(njet+1, nb+1, ht+1)              # bin
                        fractionXS[njet][nb][ht] = 1.*(entries/subStrip)*(inclStrip/inclTot)
                        #print (" - Set to ht7 value:", fractionXS[njet][nb][ht])

                    # good bins from HT8 (2500toInf)
                    elif nJnBnHT in goodHT8Bins:
                        inclTot   = fractionHistos['inclusive'].Integral()                             # matrix
                        inclStrip = fractionHistos['inclusive'].Integral(1, lenX, 1, lenY, ht+1, ht+1) # column
                        subTot    = fractionHistos['ht8'].Integral()                                   #
                        subStrip  = fractionHistos['ht8'].Integral(1, lenX, 1, lenY, ht+1, ht+1)       # sub-column
                        if abs(subTot - subStrip) > 0.0001:
                            print ("WARNING: integral of subTot:", subTot, " do not match", subStrip, "for", nJnBnHT)
                        entries = fractionHistos['ht8'].GetBinContent(njet+1, nb+1, ht+1)              # bin
                        fractionXS[njet][nb][ht] = 1.*(entries/subStrip)*(inclStrip/inclTot)
                        #print (" - Set to ht8 value:", fractionXS[njet][nb][ht])

                    #  remanining bins
                    else:
                        fractionXS[njet][nb][ht] = fractionHistos['inclusive'].GetBinContent(njet+1, nb+1, ht+1)
                        #print (" - Bad bin, set to inclusive value:", fractionXS[njet][nb][ht])


#print ("###################################")
#for njet in range (0, lenX):
#    for nb in range (0, lenY):
#        for ht in range (0,lenZ):
#            print (njet, nb, ht, fractionXS[njet][nb][ht])

#fOut = TFile ("../weights/DYstitchMap/outXSsplitCombined_Legacy2018_07May2020.root", "recreate")
fOut = TFile ("./weights/DYstitchMap/outXSsplitCombined_UL2018_04Apr2022.root", "recreate")
h3DfractionXS = TH3F ("h3DfractionXS", "h3DfractionXS;njet;nb;ht", lenX, array('d',xbins), lenY, array('d',ybins), lenZ, array('d',zbins))
for njet in range (0,lenX):
    for nb in range (0,lenY):
        for ht in range (0,lenZ):
            h3DfractionXS.SetBinContent(njet+1, nb+1, ht+1, fractionXS[njet][nb][ht])
print ("XS integral: " , h3DfractionXS.Integral()) ## should be 1
h3DfractionXS.Write()
for MCname in fractionHistos: fractionHistos[MCname].Write()
fOut.Close()

#import pdb; pdb.set_trace()
#exit()

## in histograms, x axis = njet, y axis = nb, zaxis = ht
weights = [[[0 for nz in xrange(lenZ)] for ny in xrange(lenY)] for nx in xrange(lenX)]
for njet in range (0, lenX):
    for nb in range (0, lenY):
        for ht in range (0,lenZ):
            w = 0
            for MCname in fractionHistos:
                w += 1.*fractionHistos[MCname].GetBinContent(njet+1, nb+1, ht+1)*Nevents[MCname]
            #print ("stitchWeights [%i][%i][%i] = %f ;" % (njet, nb, ht, w))
            weight = fractionXS[njet][nb][ht]/w if w > 0 else 0.
            weights[njet][nb][ht] = weight

totEvts = 0
for MCname in Nevents: totEvts += Nevents[MCname]
print ("TOT EVENTS: " , totEvts)

## scale eveything to the event sum, so that it will erase the event sum in th skimmer at denominator
for njet in range (0,lenX):
    #print("njet:", njet)
    for nb in range (0,lenY):
        #print(" nb:", nb)
        for ht in range (0,lenZ):
            weights[njet][nb][ht] *= totEvts
            #print ("  ht:", ht, "weight:", weights[njet][nb][ht])


####  printout to be copied in skimNtuple*.cpp
print("Copy the following table to skimNtuple*.cpp: \n")
print ("const float stitchWeights [5][5][9] = {", end="\n")
for nj in range (0, len(weights)):
    print (" {" , end="\n")
    for nb in range(0, len(weights[nj])):
        print ("  {" , end="")
        for ht in range(0, len(weights[nj][nb])):
            text = str(weights[nj][nb][ht])
            if ht != ( len(weights[nj][nb])-1 ) : text += " , "
            print (text, end="")
        endlinetext = "}"
        if nb != ( len(weights[nj])-1 ) : endlinetext += ","
        print (endlinetext, end="\n")
    endtext = " }"
    if nj != ( len(weights)-1 ) : endtext += ","
    print (endtext, end="\n")
print("};" , end="\n")
