# compute stitch weights for DY jet binned + nbjets binned
# NOTE: cross-section here is put to 1 pb, so that when multiplying for xsection and lumi in the skimmer
#       one gets the number of events

from __future__ import print_function # print(text,end="")
from ROOT import *
from array import array

########################################

badInclusiveBins = [
[1,0,5],
[1,0,6],
[1,0,7],
[1,0,8],
[1,1,4],
[1,1,5],
[1,1,6],
[1,1,7],
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
[3,0,8],
[3,1,6],
[3,1,7],
[3,1,8],
[3,2,5],
[3,2,6],
[3,2,7],
[3,2,8],
[3,3,0],
[3,3,3],
[3,3,4],
[3,3,5],
[3,3,6],
[3,3,7],
[3,3,8],
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

good3jetBins = [
[3,3,0]
]

good4jetBins = [
[4,1,0],
[4,3,0],
[4,3,1],
[4,4,0],
[4,4,1],
[4,4,2]
]

goodHT3Bins = [
[3,3,3],
[4,4,3]
]

goodHT4Bins = [
[1,1,4],
[2,2,4],
[3,3,4],
[4,3,4],
[4,4,4]
]

goodHT5Bins = [
[1,0,5],
[1,1,5],
[2,1,5],
[2,2,5],
[3,2,5],
[3,3,5],
[4,3,5],
[4,4,5]
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
[4,4,6]
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
[4,4,7]
]

goodHT8Bins = [
[1,0,8],
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
[4,4,8]
]

########################################

## update these numbers for each production!!
#Prod datasets UltraLegacy2016 Mar2022
Nevents = {
    'inclusive'    : 82448537,
    '1jet'         : 31570459,
    '2jet'         : 14573456,
    '3jet'         : 9553830 ,
    '4jet'         : 4450325 ,
    'ht1'          : 5893910 , #'ht70to100'   
    'ht2'          : 8106209 , #'ht100to200'  
    'ht3'          : 5653782 , #'ht200to400'  
    'ht4'          : 2491416 , #'ht400to600'  
    'ht5'          : 2299853 , #'ht600to800'  
    'ht6'          : 2351491 , #'ht800to1200' 
    'ht7'          : 1970857 , #'ht1200to2500'
    'ht8'          : 696811    #'ht2500toInf' 
}

fractionHistosFilesNames = {
    'inclusive' : '../weights/DYstitchMap/DYmap_inclusive_UL16_23Mar2022.root'    ,
    '1jet'      : '../weights/DYstitchMap/DYmap_1Jets_UL16_23Mar2022.root'	  ,
    '2jet'      : '../weights/DYstitchMap/DYmap_2Jets_UL16_23Mar2022.root'	  ,
    '3jet'      : '../weights/DYstitchMap/DYmap_3Jets_UL16_23Mar2022.root'	  ,
    '4jet'      : '../weights/DYstitchMap/DYmap_4Jets_UL16_23Mar2022.root'	  ,
    'ht1'       : '../weights/DYstitchMap/DYmap_HT-70to100_UL16_23Mar2022.root'   ,  # 'ht70to100'   
    'ht2'       : '../weights/DYstitchMap/DYmap_HT-100to200_UL16_23Mar2022.root'  ,  # 'ht100to200'  
    'ht3'       : '../weights/DYstitchMap/DYmap_HT-200to400_UL16_23Mar2022.root'  ,  # 'ht200to400'  
    'ht4'       : '../weights/DYstitchMap/DYmap_HT-400to600_UL16_23Mar2022.root'  ,  # 'ht400to600'  
    'ht5'       : '../weights/DYstitchMap/DYmap_HT-600to800_UL16_23Mar2022.root'  ,  # 'ht600to800'  
    'ht6'       : '../weights/DYstitchMap/DYmap_HT-800to1200_UL16_23Mar2022.root' ,  # 'ht800to1200' 
    'ht7'       : '../weights/DYstitchMap/DYmap_HT-1200to2500_UL16_23Mar2022.root',  # 'ht1200to2500'
    'ht8'       : '../weights/DYstitchMap/DYmap_HT-2500toInf_UL16_23Mar2022.root '   # 'ht2500toInf' 
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
    fractionHistosINT[MCname]   = fractionHistosFiles[MCname].Get('hINT_nJets_nBs_ht') # isto with integer entries to avoid numerical problems in filling
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
            #print ("Bin (nJ, nB, HT):", njet, nb, ht)
            if nb > njet:
                # nB can't be larger than njet by construction
                fractionXS[njet][nb][ht] = 0.0
                #print (" - Empty by default (nB > njet):", fractionXS[njet][nb][ht])
            else:

                #if (fractionHistos['inclusive'].GetBinContent(njet+1, nb+1, ht+1) < 1E-6): print ("bad inclusive bin [%i,%i,%i]," % (njet,nb,ht))

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
#                    else:
#		    	print ("checking bin [%i,%i,%i]," % (njet,nb,ht))
#			for MCname in fractionHistos: 
#				if (MCname != 'inclusive' and fractionHistos[MCname].GetBinContent(njet+1, nb+1, ht+1) > 0):
#					print (MCname + '- %e' % (fractionHistos[MCname].GetBinContent(njet+1, nb+1, ht+1)))
		    
                    # then do good bins from 3jets
                    elif nJnBnHT in good3jetBins:
                        inclTot   = fractionHistos['inclusive'].Integral()                                 # matrix
                        inclStrip = fractionHistos['inclusive'].Integral(njet+1, njet+1, 1, lenY, 1, lenZ) # column
                        subTot    = fractionHistos['3jet'].Integral()                                      # 
                        subStrip  = fractionHistos['3jet'].Integral(njet+1, njet+1, 1, lenY, 1, lenZ)      # sub-column
                        if subTot != subStrip: 
                            print ("WARNING: integral of subTot:", subTot, " do not match", subStrip, "for", nJnBnHT)
                        entries = fractionHistos['3jet'].GetBinContent(njet+1, nb+1, ht+1)                 # bin
                        fractionXS[njet][nb][ht] = 1.*(entries/subStrip)*(inclStrip/inclTot)
                        print (" - Set to 3jet value:", fractionXS[njet][nb][ht])
                    
                    # then do good bins from 4jets
                    elif nJnBnHT in good4jetBins:
                        inclTot   = fractionHistos['inclusive'].Integral()                                 # matrix
                        inclStrip = fractionHistos['inclusive'].Integral(njet+1, njet+1, 1, lenY, 1, lenZ) # column
                        subTot    = fractionHistos['4jet'].Integral()                                      # 
                        subStrip  = fractionHistos['4jet'].Integral(njet+1, njet+1, 1, lenY, 1, lenZ)      # sub-column
                        if subTot != subStrip: 
                            print ("WARNING: integral of subTot:", subTot, " do not match", subStrip, "for", nJnBnHT)
                        entries = fractionHistos['4jet'].GetBinContent(njet+1, nb+1, ht+1)                 # bin
                        fractionXS[njet][nb][ht] = 1.*(entries/subStrip)*(inclStrip/inclTot)
                        print (" - Set to 4jet value:", fractionXS[njet][nb][ht])
                    
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
                        print (" - Set to ht3 value:", fractionXS[njet][nb][ht])

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
                        print (" - Set to ht4 value:", fractionXS[njet][nb][ht])

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
                        print (" - Set to ht5 value:", fractionXS[njet][nb][ht])

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
                        print (" - Set to ht6 value:", fractionXS[njet][nb][ht])

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
                        print (" - Set to ht7 value:", fractionXS[njet][nb][ht])

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
                        print (" - Set to ht8 value:", fractionXS[njet][nb][ht])

                    #  remanining bins
                    else:
                        fractionXS[njet][nb][ht] = fractionHistos['inclusive'].GetBinContent(njet+1, nb+1, ht+1)
                        print (" - Bad bin [%i,%i,%i] set to inclusive value: %e" % (njet,nb,ht,fractionXS[njet][nb][ht]))

#print ("###################################")
#for njet in range (0, lenX):
#    for nb in range (0, lenY):
#        for ht in range (0,lenZ):
#            print (njet, nb, ht, fractionXS[njet][nb][ht])

fOut = TFile ("../weights/DYstitchMap/outXSsplitCombined_UL16_23Mar2022.root", "recreate")
h3DfractionXS = TH3F ("h3DfractionXS", "h3DfractionXS;njet;nb;ht", lenX, array('d',xbins), lenY, array('d',ybins), lenZ, array('d',zbins))
for njet in range (0,lenX):
    for nb in range (0,lenY):
        for ht in range (0,lenZ):
            h3DfractionXS.SetBinContent(njet+1, nb+1, ht+1, fractionXS[njet][nb][ht])
print ("XS integral: " , h3DfractionXS.Integral()) ## should be 1
h3DfractionXS.Write()
for MCname in fractionHistos: fractionHistos[MCname].Write()
fOut.Close()

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
