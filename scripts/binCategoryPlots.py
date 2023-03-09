from ROOT import *
import re
import os
import sys
import argparse
import fnmatch
import math
from array import array
import modules.ConfigReader as cfgr

def findInFolder (folder, pattern):
        ll = []
        for ff in os.listdir(folder):
            if fnmatch.fnmatch(ff, pattern): ll.append(ff)
        if len (ll) == 0:
            print "*** WARNING: No valid" , pattern , "found in " , folder
            return None
        if len (ll) > 1:
            print "*** WARNING: Too many files found in " , folder , ", using first one : " , ll
        return ll[0]



def flatBinning(rootFile,namelist, var,sel,reg):
    for name in namelist:
        fullName = name + "_" + sel + "_" + reg + "_" + var
        if not rootFile.GetListOfKeys().Contains(fullName):
            print "*** WARNING: histo " , fullName , " not available"
            continue
        
        if 'VBFC2V1' in name:
            h = rootFile.Get(fullName)

            nq = h.GetNbinsX()/2
            xq = array('d', [0.] * nq)  
            yq = array('d', [0.] * nq)  

            for i in xrange(nq):
                        xq[i] = float(i + 1) / nq
                        
            h.GetQuantiles(nq, yq, xq)
            print yq
            return yq

def retrieveHistos (rootFile, namelist, var,catList, reg,flat,binning):
    res = {}
    for name in namelist:
            for sel in catList:
                fullName = name + "_" + sel + "_" + reg + "_" + var
               
                if not rootFile.GetListOfKeys().Contains(fullName):
                    print "*** WARNING: histo " , fullName , " not available"
                    continue
                h = rootFile.Get(fullName)
               
                if not args.flat:
                        res[fullName] = h
                else:
                        hreb = h.Rebin(len(binning)-1,"hreb",binning) 
                        res[fullName] = hreb
    return res
                        
def getHisto (histoName,inputList,catList):

        h = TH1F(histoName, histoName, len(catList), 0 ,len(catList))

        for idx, name in enumerate(inputList):
                for icat, catname in enumerate(catList):                        
                        newname = histoName.replace("XXX", catname)
                        if (name.startswith(newname) and name.endswith(newname)):
                                h.SetBinContent(icat +1, inputList[newname].Integral())
                                #print icat, inputList[newname], str(inputList[newname].Integral()*scaleList[icat])
        return h

### QCD is special and has a strange name, need data to recontruct it
### CORR_DDQCD_SS_DATA_HHKin_mass_SS_defaultBtagMMNoIsoBBTTCut_DsingleMuRunD
#def retrieveQCD (rootFile, var, sel, dataNameList):
#    name = "CORR_DDQCD_SS_DATA_" + var + "_SS_" + sel + "_" + dataNameList[-1]
#    if not rootFile.GetListOfKeys().Contains(name):
#        print "*** WARNING: QCD histo " , name , " not available"
#        return None
#    hQCD = rootFile.Get(name)
#    return hQCD

# makes an histogram by adding together all those in the input list ; inputList: names, histoList: histograms
def makeStack (stackName, histoList):
    s = THStack (stackName, stackName)
    for h in histoList:
        s.Add(h)
    return s

def makeSum (sumName, histoList):
    for i,h in enumerate(histoList):
        if i == 0: hsum = h.Clone(sumName)
        else: hsum.Add(h)
    return hsum



def setPlotStyle ():
    #Styles are: Plain Bold Video Pub Classic Default Modern
    #Modern is the default one
    #ss = gROOT.GetListOfStyles()
    #for s in ss: sys.stdout.write(s.GetName() + " ")
    
    gROOT.SetStyle("Modern")
    #LucaStyle = TStyle ("LucaStyle", "LucaStyle")
    #LucaStyle

# tranform an histo into a TGraphAsymmErrors, with 
def makeTGraphFromHist (histo, newName):
    nPoints  = hData.GetNbinsX()
    fX       = []
    fY       = []
    feYUp    = []
    feYDown  = []
    feXRight = []
    feXLeft  = []

    for ibin in range (1, nPoints+1):
        x = hData.GetBinCenter(ibin);
        y = hData.GetBinContent(ibin);
        dxRight = hData.GetBinLowEdge(ibin+1) - hData.GetBinCenter(ibin);
        dxLeft  = hData.GetBinCenter(ibin) - hData.GetBinLowEdge(ibin);
        dyUp    = hData.GetBinErrorUp(ibin);
        dyLow   = hData.GetBinErrorLow(ibin);

        #if (!drawGrass && (int) y == 0) continue;
        fY.append(y)
        fX.append(x)
        feYUp.append(dyUp)
        feYDown.append(dyLow)
        feXRight.append(dxRight)
        feXLeft.append(dxLeft)

    afX       = array ("d", fX      )
    afY       = array ("d", fY      )
    afeYUp    = array ("d", feYUp   )
    afeYDown  = array ("d", feYDown )
    afeXRight = array ("d", feXRight)
    afeXLeft  = array ("d", feXLeft )

    gData = TGraphAsymmErrors (len(afX), afX, afY, afeXLeft, afeXRight, afeYDown, afeYUp);
    gData.SetMarkerStyle(8);
    gData.SetMarkerSize(1.);
    gData.SetMarkerColor(kBlack);
    gData.SetLineColor(kBlack);
    gData.SetName(newName)
    return gData;

# set all horizontal bar errors to 0
def removeHErrors (graph):
    for ipt in range (0, graph.GetN()):
        graph.SetPointEXlow(ipt, 0)
        graph.SetPointEXhigh(ipt, 0)

# remove all points with content = 0
def removeEmptyPoints (graph):
    zeroes = []
    for ipt in range (0, graph.GetN()):
        x = Double(0.0)
        y = Double(0.0)
        graph.GetPoint(ipt,x,y)
        if y == 0:
            zeroes.append(ipt)
    for i in reversed (zeroes):
        graph.RemovePoint(i)


        
def addOverFlow (histo):
    dummy = TH1F ("tempo",histo.GetTitle (),histo.GetNbinsX () + 1, histo.GetXaxis ().GetXmin (),histo.GetXaxis ().GetXmax () + histo.GetBinWidth (1)) 

    for iBin in range(1,histo.GetNbinsX () + 2):
            dummy.SetBinContent (iBin, histo.GetBinContent (iBin)) 
            dummy.SetBinError (iBin, histo.GetBinError (iBin)) 
  

    if(histo.GetDefaultSumw2()):
           dummy.Sumw2 () 

    name = histo.GetName () 
    histo.SetName ("trash") 

    dummy.GetXaxis().SetTitle("")

    dummy.SetName (name) 
    histo, dummy = dummy, histo
    return histo






def addOverAndUnderFlow ( histo):


  histo.SetBinContent(histo.GetNbinsX(),histo.GetBinContent(histo.GetNbinsX())+histo.GetBinContent(histo.GetNbinsX()+1)); 
  histo.SetBinContent(1,histo.GetBinContent(1)+histo.GetBinContent(0))
  
  if (histo.GetBinErrorOption() != TH1.kPoisson):
    histo.SetBinError(histo.GetNbinsX(),sqrt(pow(histo.GetBinError(histo.GetNbinsX()),2)+pow(histo.GetBinError(histo.GetNbinsX()+1),2)))
    histo.SetBinError(1,sqrt(pow(histo.GetBinError(1),2)+pow(histo.GetBinError(0),2)))  
  

  histo.SetBinContent(0,0)
  histo.SetBinContent(histo.GetNbinsX()+1,0)
  if (histo.GetBinErrorOption() != TH1.kPoisson):
      histo.SetBinError(0,0)
      histo.SetBinError(histo.GetNbinsX()+1,0)
  

        
# NB!! need to be called BEFORE removeHErrors or cannot know bin width
def scaleGraphByBinWidth (graph):
    for ipt in range (0, graph.GetN()):
        bwh = graph.GetErrorXhigh(ipt)
        bwl = graph.GetErrorXlow(ipt)
        bw = bwl + bwh

        eyh = graph.GetErrorYhigh(ipt)
        eyl = graph.GetErrorYlow(ipt)
        x = Double(0.0)
        y = Double(0.0)
        graph.GetPoint (ipt, x, y)
        graph.SetPoint (ipt, x, y/bw)
        graph.SetPointEYlow(ipt, eyl/bw)
        graph.SetPointEYhigh(ipt, eyh/bw)


# Get the uncertainty band from BKG to be plotted in the ratio plot
def makeMCUncertaintyBand (bkgSum):
    nPoints = bkgSum.GetNbinsX()
    fX       = []
    fY       = []
    feYUp    = []
    feYDown  = []
    feXRight = []
    feXLeft  = []

    for ibin in range (1, nPoints+1):
        central = bkgSum.GetBinContent(ibin)
        if central > 0:
            fX.append      (bkgSum.GetBinCenter(ibin))
            fY.append      (1.0)
            feYUp.append   (bkgSum.GetBinErrorUp(ibin)  / central)
            feYDown.append (bkgSum.GetBinErrorLow(ibin) / central)
            feXRight.append(bkgSum.GetBinLowEdge(ibin+1) - bkgSum.GetBinCenter(ibin))
            feXLeft.append (bkgSum.GetBinCenter(ibin) - bkgSum.GetBinLowEdge(ibin))

    afX       = array ("d", fX      )
    afY       = array ("d", fY      )
    afeYUp    = array ("d", feYUp   )
    afeYDown  = array ("d", feYDown )
    afeXRight = array ("d", feXRight)
    afeXLeft  = array ("d", feXLeft )
    gBand = TGraphAsymmErrors (len(afX), afX, afY, afeXLeft, afeXRight, afeYDown, afeYUp);
    return gBand;


## do ratio of Data/MC
# horErrs : do horizonta errors
def makeDataOverMCRatioPlot (hData, hMC, newName, horErrs=False):
    nPoints = hData.GetNbinsX()
    fX       = []
    fY       = []
    feYUp    = []
    feYDown  = []
    feXRight = []
    feXLeft  = []

    for ibin in range (1, nPoints+1):
        num = hData.GetBinContent(ibin)
        den = hMC.GetBinContent(ibin)
        if den > 0:
            # Y
            fY.append(num/den)
            feYUp.append(hData.GetBinErrorUp(ibin) / den)
            feYDown.append(hData.GetBinErrorLow(ibin) / den)

            # X
            fX.append (hData.GetBinCenter(ibin))
            if horErrs:
                feXRight.append(hData.GetBinLowEdge(ibin+1) - hData.GetBinCenter(ibin))
                feXLeft.append(hData.GetBinCenter(ibin) - hData.GetBinLowEdge(ibin))
            else:
                feXLeft.append(0.0)
                feXRight.append(0.0)

    afX       = array ("d", fX      )
    afY       = array ("d", fY      )
    afeYUp    = array ("d", feYUp   )
    afeYDown  = array ("d", feYDown )
    afeXRight = array ("d", feXRight)
    afeXLeft  = array ("d", feXLeft )
    gRatio = TGraphAsymmErrors (len(afX), afX, afY, afeXLeft, afeXRight, afeYDown, afeYUp);
    
    gRatio.SetMarkerStyle(8);
    gRatio.SetMarkerSize(1.);
    gRatio.SetMarkerColor(kBlack);
    gRatio.SetLineColor(kBlack);
    gRatio.SetName(newName)

    return gRatio;

## find maximum of tgraph, including error
def findMaxOfGraph (graph):
    uppers = []
    for i in range (0, graph.GetN()):
        x = Double(0.0)
        y = Double(0.0)
        graph.GetPoint(i, x, y)
        uppers.append (y + graph.GetErrorYhigh(i))
    return max(uppers)

# def makeSystUpDownStack (bkgList, systList, newNamePart):
#     for i, name in bkgList:
#         scale = systList[i] # error on nominal histo
#         hUp = None   # the histograms with up/down syst
#         hDown = None #
#         if i == 0:
#             hUp = bkgList[i].Clone (newNamePart + "_up")
#             hDown = bkgList[i].Clone (newNamePart + "_up")
#             hUp.Scale (1.0 + scale)
#             hDown.Scale (1.0 - scale)
#         else:
#             hTempUp   = bkgList[i].Clone (newNamePart + "_tmp_up" + i)
#             hTempDown = bkgList[i].Clone (newNamePart + "_tmp_up" + i)
#             hTempUp.Scale (1.0 + scale)
#             hTempDown.Scale (1.0 - scale)

## remove negative bins and reset yield accordingly
## NB: must be done BEFORE bin width division
def makeNonNegativeHistos (hList):
    for h in hList:
        integral = h.Integral()
        for b in range (1, h.GetNbinsX()+1):
            if (h.GetBinContent(b) < 0):
               h.SetBinContent (b, 0)
        integralNew = h.Integral()        
        if (integralNew != integral):
            print "** INFO: removed neg bins from histo " , h.GetName() 
        
        # print h.GetName() , integral , integralNew
        if integralNew == 0:
            h.Scale(0)
        else:
            h.Scale(integral/integralNew) 


### script body ###

if __name__ == "__main__" :
    TH1.AddDirectory(0)

    titleSize = 24
    labelSize = 22
    # gStyle.SetLabelFont(43)
    # gStyle.SetTitleFont(43)

    parser = argparse.ArgumentParser(description='Command line parser of plotting options')
    
    #string opts
    parser.add_argument('--var', dest='var', help='variable name', default=None)
    parser.add_argument('--sel', dest='sel', help='selection name', default=None)
    parser.add_argument('--name', dest='name', help='selection name for plot', default=None)
    parser.add_argument('--dir', dest='dir', help='analysis output folder name', default="./")
    parser.add_argument('--tag', dest='tag', help='plots output folder name', default="./")
    parser.add_argument('--reg', dest='reg', help='region name', default=None)
    parser.add_argument('--title', dest='title', help='plot title', default=None)
    parser.add_argument('--label', dest='label', help='x label', default=None)
    parser.add_argument('--channel', dest='channel', help='channel = (MuTau, ETau, TauTau)', default=None)
    parser.add_argument('--siglegextratext', dest='siglegextratext', help='additional optional text to be plotted in legend after signal block', default=None)
    
    #bool opts
    parser.add_argument('--log',     dest='log', help='use log scale',  action='store_true', default=False)

    parser.add_argument('--no-data', dest='dodata', help='disable plotting data',  action='store_false', default=True)
    parser.add_argument('--no-sig',  dest='dosig',  help='disable plotting signal',  action='store_false', default=True)
    parser.add_argument('--no-legend',   dest='legend',   help = 'disable drawing legend',       action='store_false', default=True)
    parser.add_argument('--no-binwidth', dest='binwidth', help = 'disable scaling by bin width', action='store_false', default=True)
    parser.add_argument('--ratio',    dest='ratio', help = 'do ratio plot at the botton', action='store_true', default=False)
    parser.add_argument('--no-print', dest='printplot', help = 'no pdf output', action='store_false', default=True)
    parser.add_argument('--quit',    dest='quit', help = 'quit at the end of the script, no interactive window', action='store_true', default=False)
    parser.add_argument('--overflow',    dest='overflow', help = 'add overflow bin', action='store_true', default=False)
    parser.add_argument('--flat',    dest='flat', help = 'rebin getting flat signal', action='store_true', default=False)

    # par list opt
    parser.add_argument('--blind-range',   dest='blindrange', nargs=2, help='start and end of blinding range', default=None)

    #float opt
    parser.add_argument('--lymin', dest='lymin', type=float, help='legend min y position in pad fraction', default=None)
    parser.add_argument('--ymin', dest='ymin', type=float, help='min y range of plots', default=None)
    parser.add_argument('--ymax', dest='ymax', type=float, help='max y range of plots', default=None)
    parser.add_argument('--sigscale', dest='sigscale', type=float, help='scale to apply to all signals', default=None)
    parser.add_argument('--lumi', dest='lumi_num', type=float, help='lumi in fb-1', default=None)

    
    args = parser.parse_args()

    if args.quit : gROOT.SetBatch(True)
    
    ######################### CANVASES #################################

    c1 = TCanvas ("c1", "c1", 800, 600)
    # c1.SetLeftMargin(0.15);
    # c1.SetBottomMargin(0.12);
    # c1.SetTopMargin(0.055);

    pad1 = None
    pad2 = None

    if args.ratio:
        pad1 = TPad ("pad1", "pad1", 0, 0.5, 1, 1.0)
        pad1.SetFrameLineWidth(3)
        pad1.SetLeftMargin(0.16);
        pad1.SetRightMargin(0.05);
        pad1.SetBottomMargin(0.02);
        pad1.SetTopMargin(0.1);
        
        pad1.Draw()
    else:
        pad1 = TPad ("pad1", "pad1", 0, 0.0, 1.0, 1.0)
        pad1.SetFrameLineWidth(3)
        pad1.SetLeftMargin(0.12);
        pad1.SetBottomMargin(0.12);
        pad1.SetTopMargin(0.1);
        pad1.Draw()


    pad1.cd()



    ######################### PUT USER CONFIGURATION HERE ####################
    cfgName  =  args.dir + "/mainCfg_"+args.channel+"_Legacy2017_tauIDSF.cfg"
    cfg        = cfgr.ConfigReader (cfgName)
    bkgList    = cfg.readListOption("general::backgrounds")
    catList = cfg.readListOption("pp_bins::categories")
    catLabels = cfg.readListOption("pp_bins::labels")

    for il,label in enumerate(catLabels):

            catLabels[il] =  str(label).replace("tau", "#tau")
            #catLabels[il] =  str(label).replace("\\", "#")


    doQCD = True
    if not "SR" in args.reg: doQCD = False
    if not "Tau" in args.channel: doQCD = False
    
    if doQCD:
        bkgList.append('QCD')
        


    bkgColors = {}
    #bkgColors["singleT"] = kOrange+10
    #bkgColors["EWKW"] = kGreen+3
    #bkgColors["VV"] = kViolet
    #bkgColors["SM"] = kBlue+1
    #bkgColors["DY"] = kGreen+1
    #bkgColors["TT"] =  kOrange+1
    #bkgColors["WJets"] = kAzure-2
    #bkgColors["other"] = kCyan+1

    # RGB/HEX colors
    col = TColor()
    bkgColors["DY"] = col.GetColor("#2dc43e")#44BA68 #(TColor(68 ,186,104)).GetNumber() #gROOT.GetColor("#44BA68")
    #bkgColors["DYQQ"] = kMagenta #(TColor(68 ,186,104)).GetNumber() #gROOT.GetColor("#44BA68")
    bkgColors["TT"]    = col.GetColor("#f4c92c")#F4B642 #(TColor(244,182,66 )).GetNumber() #gROOT.GetColor("#F4B642")
    bkgColors["WJets"] = col.GetColor("#30b0db")#41B4DB #(TColor(65 ,180,219)).GetNumber() #gROOT.GetColor("#41B4DB")
    bkgColors["other"] = col.GetColor("#ed4e49")#ED635E #(TColor(237,99 ,94 )).GetNumber() #gROOT.GetColor("#ED635E")

    bkgLineColors = {}
    bkgLineColors["DY"]    = col.GetColor("#27ad37")
    #bkgLineColors["DYQQ"]    = kMagenta
    bkgLineColors["TT"]    = col.GetColor("#e2ba28")
    bkgLineColors["WJets"] = col.GetColor("#278fb2")
    bkgLineColors["other"] = col.GetColor("#c4413c")



    plotTitle = ""

    if args.title:
        plotTitle = args.title
    dataList = ["data_obs"]


    if cfg.hasSection("merge"): 
        for groupname in cfg.config['merge']:
            if "data" in groupname: continue
            mergelist = cfg.readListOption('merge::'+groupname)
            for x in mergelist:
                bkgList.remove(x)
            bkgList.append(groupname)


   


    ###########################################################################
    #setPlotStyle()


    outplotterName = findInFolder  (args.dir+"/", 'analyzedOutPlotter.root')
    

    #    if not "Tau" in args.channel:
    #           outplotterName = findInFolder  (args.dir+"/", 'outPlotter.root')            
 
    rootFile = TFile.Open (args.dir+"/"+outplotterName)

    binning = None
    hBkgs = retrieveHistos  (rootFile, bkgList, args.var, catList ,args.reg,args.flat,binning)

    hDatas = retrieveHistos  (rootFile, dataList, args.var, catList ,args.reg,args.flat,binning)



            
    doOverflow = args.overflow
    

    suffix = "_XXX_" + args.reg + "_" + args.var
    
    hDY = getHisto("DY"+suffix, hBkgs,catList)    
    hTT = getHisto("TT"+suffix, hBkgs,catList)
    hWJets = getHisto("WJets"+suffix, hBkgs,catList)
    hothers = getHisto("other"+suffix, hBkgs,catList)

    hBkgList = [hothers, hWJets, hTT, hDY] ## full list for stack


    
    #hBkgNameList = ["Others", "W + jets", "t#bar{t}", "DY + jets", "DYQQ"] # list for legend
    hBkgNameList = ["Others", "W + jets", "t#bar{t}", "DY + jets"] # list for legend
    

    #if cfg.hasSection('pp_QCD'):
    if doQCD:
        hQCD    = getHisto ("QCD"+suffix, hBkgs,catList)
        hQCD.SetName("QCD")
        hBkgList.append(hQCD)
        hBkgNameList.append("QCD")
        #bkgColors["QCD"] = kPink+5
        bkgColors["QCD"] = col.GetColor("#ff7b00") #F29563 #(TColor(242,149,99)).GetNumber() #gROOT.GetColor("#F29563")

        bkgLineColors["QCD"] = col.GetColor("#d66700")


    hData = getHisto("data_obs"+suffix, hDatas , catList).Clone("hData")

    # remove all data from blinding region before creating tgraph etc...
    if args.blindrange:
        blow = float (args.blindrange[0]) 
        bup = float (args.blindrange[1]) 
        for ibin in range (1, hData.GetNbinsX()+1):
            center = hData.GetBinCenter(ibin)
            if center > blow and center < bup:
                hData.SetBinContent(ibin, 0)


    hDataNonScaled = hData.Clone("hDataNonScaled")
    gData = makeTGraphFromHist (hData, "grData")






    # apply bkg color if available
    for h in hBkgList:
            histoname =h.GetName()
            for key,value in bkgColors.items():
                if key in histoname:    
                        thecolor = int(bkgColors[key])
                        h.SetFillColor(thecolor)
                        h.SetFillStyle(1001)

    # apply bkg lines colors if available
    for h in hBkgList:
        histoname = h.GetName()
        for key,value in bkgLineColors.items():
            if key in histoname:
                thecolor = int(bkgLineColors[key])
                h.SetLineColor(thecolor)
                h.SetLineWidth(1)

            
    #################### REMOVE NEGARIVE BINS #######################
    print "** INFO: removing all negative bins from bkg histos"
    makeNonNegativeHistos (hBkgList)


    #################### PERFORM DIVISION BY BIN WIDTH #######################
    #clones non scaled (else problems with graph ratio because I pass data evt hist)
    bkgStackNS = makeStack ("bkgStackNS", hBkgList)
    hBkgEnvelopeNS = bkgStackNS.GetStack().Last().Clone("hBkgEnvelopeNS")

    if args.binwidth:
        scaleGraphByBinWidth (gData)
        for h in hBkgList:
            h.Scale(1., "width")
        

    #################### DO STACK AND PLOT #######################

    bkgStack = makeStack ("bkgStack", hBkgList)
    bkgSum = makeSum ("bkgSum", hBkgList)
    

    
    if args.log: pad1.SetLogy()



    hDataOverDY = hDataNonScaled.Clone("hDataOverDY")
    #hDataOverDY.Add(hWJets, -1)
    #hDataOverDY.Add(hTT, -1)
    hDataOverDY.Add(hQCD, -1)
    #hDataOverDY.Add(hothers, -1)
    hDataOverDY.Divide(hDY)
    print "Rough computation"
    for icat, cat  in enumerate(catList):
            print cat, hDataOverDY.GetBinContent(icat+1)
    
    
    
    


    ################## TITLE AND AESTETICS ############################
    bkgStack.Draw("HIST")


    bkgStack.GetYaxis().SetTitleFont(43)

    bkgStack.GetYaxis().SetLabelFont(43)
    bkgStack.GetXaxis().SetLabelFont(43)


    
    bkgStack.GetYaxis().SetTitleOffset(1.8)

    
    bkgStack.GetYaxis().SetTitleSize(titleSize)

    
    bkgStack.GetYaxis().SetLabelSize(labelSize)

    for icat, catlabel in enumerate(catLabels):                        
            bkgStack.GetXaxis().SetBinLabel(icat+1, catlabel)                                
                    
    bkgStack.GetXaxis().LabelsOption("v")
    bkgStack.GetXaxis().SetTitle ('')
    if args.ratio: bkgStack.GetXaxis().SetLabelSize(0)

    
    ylabel = "Events"

    bkgStack.GetYaxis().SetTitle(ylabel)
    
    #intBkg = bkgStack.GetHistogram().Integral()
    intBkg = bkgStack.GetStack().Last().Integral()

    bkgStack.SetTitle(plotTitle)

                
    ################## LEGEND ######################################

    legmin = 0.43
    if args.lymin: legmin = args.lymin
    legminx = 0.50
    if (len(hBkgNameList) >3): legminx = 0.4
    leg = TLegend (legminx, legmin, 0.85, 0.87)
    if (len(hBkgNameList) > 3): leg.SetNColumns(2)
    leg.SetFillStyle(0)
    leg.SetBorderSize(0)
    leg.SetTextFont(43)
    leg.SetTextSize(20)

    # add element in same order as stack --> top-bottom
    for i, name in reversed(list(enumerate(hBkgNameList))):
        leg.AddEntry(hBkgList[i], name, "f")


    if args.dodata:
        leg.AddEntry(gData, "Data", "pe")


    ################## Y RANGE SETTINGS ############################
    ymin = 0
    if args.log: ymin = 0.1

    maxs = []
    
    # bkgmax = 0
    # for h in hBkgList: bkgmax+= h.GetMaximum()
    # maxs.append(bkgmax)
    maxs.append(bkgStack.GetStack().Last().GetMaximum())

    if args.dodata:
        maxs.append(findMaxOfGraph(gData))
        #if not args.binwidth:
        #    maxs.append(hData.GetMaximum() + math.sqrt(hData.GetMaximum()))
        

    ymax = max(maxs)

    # scale max to leave some space (~10%)
    extraspace = 0.3

    if not args.log:
        ymax += extraspace* (ymax-ymin)
    
    else:
        new = extraspace * (math.log(ymax, 10) - math.log(ymin, 10)) + math.log(ymax, 10)
        ymax = math.pow(10, new)


    #print "limits: " , ymin, ymax

    ## override form args
    if args.ymin: ymin = args.ymin
    if args.ymax: ymax = args.ymax

    bkgStack.SetMinimum(ymin)
    bkgStack.SetMaximum(ymax)

    # interactive display
    bkgStack.Draw("HIST")
    bkgSum.SetFillColor(kGray+2);
    bkgSum.SetFillStyle(3002);
    bkgSum.Draw("e2 same")

    if args.dodata:
        removeHErrors(gData)
        removeEmptyPoints(gData)
        gData.Draw("P Z same") # Z: no small line at the end of error bar

    ###################### OTHER TEXT ON PLOT #########################

    # extraText = "preliminary"
    # CMStext = "CMS"

    cmsTextFont     = 61  # font of the "CMS" label
    cmsTextSize   = 0.08  # font size of the "CMS" label
    extraTextFont   = 51     # for the "preliminary"
    extraTextSize   = 0.76 * cmsTextSize # for the "preliminary"


    
    t = gPad.GetTopMargin()
    b = gPad.GetBottomMargin()
    l = gPad.GetLeftMargin()
    r = gPad.GetRightMargin()       
    #yoffset = 0.05 # fractional shift   

    CMSbox       = TLatex  (l , 1 - t + 0.02, "CMS")       
    extraTextBox = TLatex  (l + 0.1 , 1 - t + 0.02,"Preliminary")
    CMSbox.SetNDC()
    extraTextBox.SetNDC()
    CMSbox.SetTextSize(cmsTextSize)
    CMSbox.SetTextFont(cmsTextFont)
    CMSbox.SetTextColor(kBlack)
    CMSbox.SetTextAlign(11)
    extraTextBox.SetTextSize(extraTextSize)
    extraTextBox.SetTextFont(extraTextFont)
    extraTextBox.SetTextColor(kBlack)
    extraTextBox.SetTextAlign(11)

    x = 0
    y = 0
    #    histoWidth = histo.GetXaxis().GetBinWidth(1)*histo.GetXaxis().GetNbins()
    #    histoHeight = histo.GetMaximum()-histo.GetMinimum()
 

    lumi = "%.1f fb^{-1} (13 TeV)" % args.lumi_num
    lumibox = TLatex  (1-r, 1 - t + 0.02 , lumi)       
    lumibox.SetNDC()
    lumibox.SetTextAlign(31)
    lumibox.SetTextSize(extraTextSize)
    lumibox.SetTextFont(42)
    lumibox.SetTextColor(kBlack)

    if args.channel:
        if args.channel == "MuTau":
            chName = "bb #mu#tau_{h}"
        elif args.channel == "ETau":
            chName = "bb e#tau_{h}"
        elif args.channel == "TauTau" and not "HTauTau" in args.sel:
            chName = "#tau_{h}#tau_{h}"
        elif args.channel == "TauTau":
            chName = "#tau_{h}#tau_{h}"
        elif args.channel == "MuMu":
            chName = "bb #mu#mu"
        else:
            print "*** Warning: channel name must be MuTau, ETau, TauTau, you wrote: " , args.channel

        if chName:
            chBox = TLatex  (l + 0.04 , 1 - t - 0.02, chName)
            chBox.SetNDC()
            chBox.SetTextSize(cmsTextSize+20)
            chBox.SetTextFont(43)
            chBox.SetTextColor(kBlack)
            chBox.SetTextAlign(13)
            
    #CMSbox.Draw()
    #extraTextBox.Draw()
    lumibox.Draw()
    
    if args.legend: leg.Draw()
    if chBox: chBox.Draw()



    
    ###################### BLINDING BOX ###############################
    if args.blindrange:
        blow = float(args.blindrange[0])
        bup = float(args.blindrange[1])
        bBox = TBox (blow, ymin, bup, 0.93*ymax)
        bBox.SetFillStyle(3002) # NB: does not appear the same in displayed box and printed pdf!!
        bBox.SetFillColor(kGray+2) # NB: does not appear the same in displayed box and printed pdf!!
        bBox.Draw()


    ###################### RATIO PLOT #################################
    if args.ratio:
        bkgStack.GetXaxis().SetTitleSize(0.00);
        bkgStack.GetXaxis().SetLabelSize(0.00);

        c1.cd()
        pad2 = TPad ("pad2", "pad2", 0, 0.0, 1, 0.4996)
        pad2.SetLeftMargin(0.16);
        pad2.SetRightMargin(0.05);
        pad2.SetTopMargin(0.02);
        pad2.SetBottomMargin(0.55);
        pad2.SetGridy(True);
        pad2.SetFrameLineWidth(3)
        #pad2.SetGridx(True);
        pad2.Draw()
        pad2.cd()

        grRatio = makeDataOverMCRatioPlot (hDataNonScaled, hBkgEnvelopeNS, "grRatio")


        hRatio = hDataNonScaled.Clone("hRatioAxis") # for ranges only
        grUncert = makeMCUncertaintyBand (bkgSum) # uncertainty band from MC, always centered at 1.0

        hRatio.GetXaxis().SetTitleFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetTitleFont(43) # so that size is in pixels
        hRatio.GetXaxis().SetLabelFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetLabelFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetNdivisions(505)

        #hRatio.GetXaxis().SetTitle(bkgStack.GetXaxis().GetName())
        hRatio.SetTitle(plotTitle)
        hRatio.GetYaxis().SetTitle ("Data/Bkg.") #("Data/MC")
        hRatio.GetXaxis().SetTitle ('')
        for icat, catlabel in enumerate(catLabels):                        
            hRatio.GetXaxis().SetBinLabel(icat+1, catlabel)      
        hRatio.GetXaxis().LabelsOption("v")

        hRatio.GetXaxis().SetTitleOffset(1.2)
        hRatio.GetYaxis().SetTitleOffset(1.2)

        hRatio.GetXaxis().SetTitleSize(titleSize);
        hRatio.GetXaxis().SetLabelSize(18);
        hRatio.GetYaxis().SetTitleSize(titleSize);
        hRatio.GetYaxis().SetLabelSize(labelSize);

        hRatio.GetXaxis().SetTickSize(0.10)
        hRatio.GetYaxis().SetTickSize(0.05)

        hRatio.SetStats(0)
        #hRatio.SetMinimum(0.5)
        #hRatio.SetMaximum(1.5)
        #hRatio.SetMinimum(0.1)
        #hRatio.SetMaximum(1.9)
        hRatio.SetMinimum(0.75)
        hRatio.SetMaximum(1.25)

        removeEmptyPoints (grRatio)
        hRatio.Draw("axis")
        
        grRatio.Draw("P Z same") # Z : no small limes at the end of points
        xmin =hRatio.GetXaxis().GetXmin()
        xmax = hRatio.GetXaxis().GetXmax()
        l1 = TLine(xmin, 1, xmax, 1)
        l1.SetLineColor(kRed)
        l1.SetLineStyle(4)
        l1.SetLineWidth(1)
        l1.Draw("same")

        grUncert.SetFillColor(kGray+2)
        grUncert.SetFillStyle(3002)
        grUncert.Draw("e2")

        pad2.RedrawAxis();
        pad2.RedrawAxis("g"); #otherwise no grid..
        pad1.cd()
        pad1.RedrawAxis()
    ###################### DISPLAY ###################################
    if not args.quit:
        # pad1.Update() # necessary to show plot
        # pad2.Update() # necessary to show plot
        c1.Update()
        pad1.Update()
        if pad2: pad2.Update()
        raw_input() # to prevent script from closing

    if args.printplot:
        tagch = ""
        if args.channel:
            tagch = "_" + args.channel
        saveName = "./plotsHHLegacy2017_"+args.channel+"/"+args.tag+"/"+args.sel+"_"+args.reg+"/plot_decaymodes_" + args.sel +"_" + args.reg+ tagch
        
        c1.SaveAs (saveName+".pdf")
        c1.SaveAs (saveName+".png")

    hRatioPlot = hDataNonScaled.Clone("hRatioPlot")
    hRatioPlot.Divide(hBkgEnvelopeNS)

    DMmap = TH2F("DMmap","DMmap", 4, 0, 4, 4, 0, 4)
    DMs = {"DM0":1, "DM1":2, "DM10":3, "DM11":4}
    for p in range(2, hRatioPlot.GetNbinsX()+1): 
        print hRatio.GetXaxis().GetBinLabel(p) 
        print hRatioPlot.GetBinContent(p)
        tau1,tau2 = hRatio.GetXaxis().GetBinLabel(p).split("-")
        tau1 = tau1.replace("#tau_{1} ", "").strip()
        tau2 = tau2.replace("#tau_{2} ", "").strip()
        DMmap.SetBinContent(DMs[tau1],DMs[tau2], hRatioPlot.GetBinContent(p))
    for k,v in DMs.items():
        #print k,v
        DMmap.GetXaxis().SetBinLabel(v,k)  
        DMmap.GetYaxis().SetBinLabel(v,k)
    gStyle.SetOptStat(0)
    DMmap.SetMaximum(1.2)
    DMmap.SetMinimum(0.8)
    gStyle.SetPalette(kTemperatureMap)
    DMmap.SetTitle('')
    DMmap.GetXaxis().SetTitle("#tau_{1} DM")
    DMmap.GetYaxis().SetTitle("#tau_{2} DM")
    DMmap.GetZaxis().SetTitle("Data/Bkg")
    c2 = TCanvas ("c2", "c2", 650, 600)
    c2.SetRightMargin(0.15);
    c2.Draw()
    DMmap.Draw("colz")
    if args.printplot:
        tagch = ""
        if args.channel:
            tagch = "_" + args.channel
        saveName = "./plotsHHLegacy2017_"+args.channel+"/"+args.tag+"/"+args.sel+"_"+args.reg+"/plot_decaymodes_map_" + args.sel +"_" + args.reg+ tagch
        
        c2.SaveAs (saveName+".pdf")
        c2.SaveAs (saveName+".png")
