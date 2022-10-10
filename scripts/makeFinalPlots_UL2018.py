import os
import os.path as op
import re
import argparse
import fnmatch
import math
from array import array
from ctypes import c_double, c_float

import modules.ConfigReader as cfgr
import modules.OutputManager as omng
import ROOT

def findInFolder(folder, pattern):
        ll = []
        for ff in os.listdir(folder):
            if fnmatch.fnmatch(ff, pattern): ll.append(ff)
        if len (ll) == 0:
            print('*** WARNING: No valid {} found in {}.'.format(pattern, folder))
            return None
        if len (ll) > 1:
            print('*** WARNING: Too many files in {}, using the first: {}.'.format(folder, ll))
        return ll[0]

def flatBinning(rootFile,namelist, var,sel,reg):
    for name in namelist:
        fullName = name + '_' + sel + '_' + reg + '_' + var
        if not rootFile.GetListOfKeys().Contains(fullName):
            print('*** WARNING: histo {} not available.'.format(fullName))
            continue
        
        if 'VBFC2V1' in name:
            h = rootFile.Get(fullName)

            nq = h.GetNbinsX()/2
            xq = array('d', [0.] * nq)  
            yq = array('d', [0.] * nq)  

            for i in xrange(nq):
                        xq[i] = float(i + 1) / nq
                        
            h.GetQuantiles(nq, yq, xq)
            return yq

def retrieveHistos(rootFile, namelist, var, sel, reg, flat, binning):
    res = {}
    for name in namelist:
        fullName = name + '_' + sel + '_' + reg + '_' + var

        if not rootFile.GetListOfKeys().Contains(fullName):
            print('*** WARNING: histo {} not available.'.format(fullName))
            continue
        h = rootFile.Get(fullName)
    
        if not args.flat:
                res[name] = h
        else:
                hreb = h.Rebin(len(binning)-1,'hreb',binning) 
                res[name] = hreb
                        
    return res
                        
def getHisto(histoName, inputList, doOverflow):
    assert len(inputList) != 0
    for idx, name in enumerate(inputList):
        if (name.startswith(histoName) and name.endswith(histoName)):
            h = inputList[name].Clone(histoName)
            if doOverflow:
                h = addOverFlow(h)
                break
    return h

# makes an histogram by adding together all those in the input list ; inputList: names, histoList: histograms
def makeStack (stackName, histoList):
    s = ROOT.THStack (stackName, stackName)
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
    
    ROOT.gROOT.SetStyle("Modern")
    #LucaStyle = TStyle ("LucaStyle", "LucaStyle")
    #LucaStyle

# tranform an histo into a TGraphAsymmErrors, with 
def makeTGraphFromHist(histo, newName):
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

    afX       = array ('d', fX      )
    afY       = array ('d', fY      )
    afeYUp    = array ('d', feYUp   )
    afeYDown  = array ('d', feYDown )
    afeXRight = array ('d', feXRight)
    afeXLeft  = array ('d', feXLeft )

    gData = ROOT.TGraphAsymmErrors(len(afX), afX, afY, afeXLeft, afeXRight, afeYDown, afeYUp);
    gData.SetMarkerStyle(8);
    gData.SetMarkerSize(1.);
    gData.SetMarkerColor(ROOT.kBlack);
    gData.SetLineColor(ROOT.kBlack);
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
        x = c_double(0.0)
        y = c_double(0.0)
        graph.GetPoint(ipt,x,y)
        if y == 0:
            zeroes.append(ipt)
    for i in reversed (zeroes):
        graph.RemovePoint(i)


        
def addOverFlow (histo):
    dummy = ROOT.TH1F('tempo',histo.GetTitle (),histo.GetNbinsX () + 1, histo.GetXaxis ().GetXmin (),histo.GetXaxis ().GetXmax () + histo.GetBinWidth (1)) 

    for iBin in range(1,histo.GetNbinsX () + 2):
            dummy.SetBinContent (iBin, histo.GetBinContent (iBin)) 
            dummy.SetBinError (iBin, histo.GetBinError (iBin)) 
  

    if(histo.GetDefaultSumw2()):
           dummy.Sumw2 () 

    name = histo.GetName () 
    histo.SetName ('trash') 
    if args.label:
        dummy.GetXaxis().SetTitle(args.label)
    else:
        dummy.GetXaxis().SetTitle(args.var)
    dummy.SetName (name) 
    histo, dummy = dummy, histo
    return histo

def addOverAndUnderFlow ( histo):
  histo.SetBinContent(histo.GetNbinsX(),histo.GetBinContent(histo.GetNbinsX())+histo.GetBinContent(histo.GetNbinsX()+1)); 
  histo.SetBinContent(1,histo.GetBinContent(1)+histo.GetBinContent(0))
  
  if (histo.GetBinErrorOption() != ROOT.TH1.kPoisson):
    histo.SetBinError(histo.GetNbinsX(),sqrt(pow(histo.GetBinError(histo.GetNbinsX()),2)+pow(histo.GetBinError(histo.GetNbinsX()+1),2)))
    histo.SetBinError(1,sqrt(pow(histo.GetBinError(1),2)+pow(histo.GetBinError(0),2)))

  histo.SetBinContent(0,0)
  histo.SetBinContent(histo.GetNbinsX()+1,0)
  if (histo.GetBinErrorOption() != ROOT.TH1.kPoisson):
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
        x = c_double(0.0)
        y = c_double(0.0)
        graph.GetPoint(ipt, x, y)
        graph.SetPoint(ipt, x.value, y.value/bw)
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

    afX       = array ('d', fX      )
    afY       = array ('d', fY      )
    afeYUp    = array ('d', feYUp   )
    afeYDown  = array ('d', feYDown )
    afeXRight = array ('d', feXRight)
    afeXLeft  = array ('d', feXLeft )
    gBand = ROOT.TGraphAsymmErrors(len(afX), afX, afY, afeXLeft, afeXRight, afeYDown, afeYUp);
    return gBand;

# get median significance (better than s/sqrt(b))
def getZa(s, b, sigma_b):
    x1 = (s+b)*math.log( (s+b) * (b+(sigma_b*sigma_b)) / ((b*b)+((s+b)*(sigma_b*sigma_b))) )
    x2 = ((b*b)/(sigma_b*sigma_b)) * math.log(1+ (sigma_b*sigma_b*s)/(b*(b+(sigma_b*sigma_b))) )
    if (x1-x2) > 0:
        Za = math.sqrt( 2*(x1-x2) )
    else:
        Za =0
    return Za

## do ratio of S/(S+B)
# horErrs : do horizontal errors
def makeSBSPlot (hData, hMC, newName, color, horErrs=False):
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
        den_err = hMC.GetBinError(ibin)
        if den > 0:
            # Y
            Za = getZa(num, den, den_err)
            #fY.append(num/math.sqrt(num+den)) # S/sqrt(S+B)
            fY.append(Za)                      # Za
            feYUp.append(0.0)
            feYDown.append(0.0)

            # X
            fX.append (hData.GetBinCenter(ibin))
            if horErrs:
                feXRight.append(hData.GetBinLowEdge(ibin+1) - hData.GetBinCenter(ibin))
                feXLeft.append(hData.GetBinCenter(ibin) - hData.GetBinLowEdge(ibin))
            else:
                feXLeft.append(0.0)
                feXRight.append(0.0)

    afX       = array ('d', fX      )
    afY       = array ('d', fY      )
    afeYUp    = array ('d', feYUp   )
    afeYDown  = array ('d', feYDown )
    afeXRight = array ('d', feXRight)
    afeXLeft  = array ('d', feXLeft )
    gSBS = ROOT.TGraphAsymmErrors(len(afX), afX, afY, afeXLeft, afeXRight, afeYDown, afeYUp);

    gSBS.SetMarkerStyle(1);
    gSBS.SetMarkerSize(1.);
    gSBS.SetMarkerColor(color);
    gSBS.SetLineColor(color);
    gSBS.SetLineWidth(2);
    gSBS.SetName(newName)

    return gSBS;

## do ratio of Data/MC
# horErrs : do horizontal errors
def makeDataOverMCRatioPlot(hData, hMC, newName, horErrs=False):
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

    afX       = array ('d', fX      )
    afY       = array ('d', fY      )
    afeYUp    = array ('d', feYUp   )
    afeYDown  = array ('d', feYDown )
    afeXRight = array ('d', feXRight)
    afeXLeft  = array ('d', feXLeft )
    gRatio = ROOT.TGraphAsymmErrors(len(afX), afX, afY, afeXLeft, afeXRight, afeYDown, afeYUp);
    
    gRatio.SetMarkerStyle(8);
    gRatio.SetMarkerSize(1.);
    gRatio.SetMarkerColor(ROOT.kBlack);
    gRatio.SetLineColor(ROOT.kBlack);
    gRatio.SetName(newName)

    return gRatio;

## find maximum of tgraph, including error
def findMaxOfGraph (graph):
    uppers = []
    for i in range (0, graph.GetN()):
        x = c_double(0.0)
        y = c_double(0.0)
        graph.GetPoint(i, x, y)
        uppers.append(y.value + graph.GetErrorYhigh(i))
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
            print("** INFO: removed neg bins from histo " , h.GetName())
        
        if integralNew == 0:
            h.Scale(0)
        else:
            h.Scale(integral/integralNew) 


### script body ###

if __name__ == "__main__" :
    ROOT.TH1.AddDirectory(0)

    titleSize = 24
    labelSize = 22
    # gStyle.SetLabelFont(43)
    # gStyle.SetTitleFont(43)

    parser = argparse.ArgumentParser(description='Command line parser of plotting options')
    
    #string opts
    parser.add_argument('--var', help='variable name', default=None)
    parser.add_argument('--sel', help='selection name', default=None)
    parser.add_argument('--name', help='selection name for plot', default=None)
    parser.add_argument('--indir', 
                        help='analysis input folder name', default='.')
    parser.add_argument('--outdir', dest='outdir',
                        help='analysis output folder name', default='.')
    parser.add_argument('--reg', help='region name', default=None)
    parser.add_argument('--title', help='plot title', default=None)
    parser.add_argument('--label', help='x label', default=None)
    parser.add_argument('--channel', choices=('ETau', 'MuTau', 'TauTau'),
                        help='channel', required=True)
    parser.add_argument('--main_cfg',
                        help='Main configuration file (used in previous steps as well)',
                        default=None)
    parser.add_argument('--siglegextratext', dest='siglegextratext',
                        help='extra opt text for legend after signal block',
                        default=None)
    #bool opts
    parser.add_argument('--log',     dest='log', help='use log scale',  action='store_true', default=False)
    parser.add_argument('--no-data', dest='dodata', help='disable plotting data', action='store_false', default=True)
    parser.add_argument('--no-sig',  dest='dosig',  help='disable plotting signal',  action='store_false')
    parser.add_argument('--no-legend',   dest='legend',   help = 'disable drawing legend',       action='store_false')
    parser.add_argument('--no-binwidth', dest='binwidth', help = 'disable scaling by bin width', action='store_false')
    parser.add_argument('--ratio',    dest='ratio', help = 'do ratio plot at the botton', action='store_true', default=False)
    parser.add_argument('--sbs',    dest='sbs', help = 'do S/(S+B) plot at the botton', action='store_true', default=False)
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

    if args.quit:
        ROOT.gROOT.SetBatch(True)
    
    ######################### CANVASES #################################

    c1 = ROOT.TCanvas('c1', 'c1', 600, 600)

    pad1 = None
    pad2 = None

    if args.ratio or args.sbs:
        pad1 = ROOT.TPad('pad1', 'pad1', 0, 0.25, 1, 1.0)
        pad1.SetFrameLineWidth(3)
        pad1.SetLeftMargin(0.12);
        pad1.SetBottomMargin(0.02);
        pad1.SetTopMargin(0.055);
        pad1.Draw()
    else:
        pad1 = ROOT.TPad('pad1', 'pad1', 0, 0.0, 1.0, 1.0)
        pad1.SetFrameLineWidth(3)
        pad1.SetLeftMargin(0.12);
        pad1.SetBottomMargin(0.12);
        pad1.SetTopMargin(0.055);
        pad1.Draw()

    pad1.cd()

    ######################### PUT USER CONFIGURATION HERE ####################
    cfgName = op.join(args.indir, args.main_cfg)
    cfg = cfgr.ConfigReader (cfgName)
    bkgList = cfg.readListOption('general::backgrounds')

    doQCD = True
    if not 'SR' in args.reg: doQCD = False
    if not 'Tau' in args.channel: doQCD = False
    
    if doQCD:
        bkgList.append('QCD')
    
    sigList = cfg.readListOption("general::signals")

    sigNameList = []
    listColors = (ROOT.kBlack, ROOT.kCyan,)
    sigColors = {}
    for isl,sl in enumerate(sigList):
        if 'VBF' in sl:
            name_list = 'VBF HH SM (x10)'                
        elif 'ggFRadion':
            name_list = 'ggFRadion'
        else:
            name_list = 'ggHTauTau SM x 20'
        sigNameList.append(name_list)
        sigColors[sl] = listColors[isl]

    # RGB/HEX colors
    col = ROOT.TColor()

    bkgColors = {}
    bkgColors["DY_HM"]      = col.GetColor("#44BA68") #(TColor(68 ,186,104)).GetNumber() #gROOT.GetColor("#44BA68")
    bkgColors["TT"]      = col.GetColor("#F4B642") #(TColor(244,182,66 )).GetNumber() #gROOT.GetColor("#F4B642")
    bkgColors["W"]  = col.GetColor("#41B4DB") #(TColor(65 ,180,219)).GetNumber() #gROOT.GetColor("#41B4DB")
    #bkgColors["singleH"] = col.GetColor("#41B4DB") #(TColor(65 ,180,219)).GetNumber() #gROOT.GetColor("#41B4DB")
    bkgColors["other"]    = col.GetColor("#ED635E") #(TColor(237,99 ,94 )).GetNumber() #gROOT.GetColor("#ED635E")

    bkgLineColors = {}
    bkgLineColors["DY_HM"]      = col.GetColor("#389956")
    bkgLineColors["TT"]      = col.GetColor("#dea63c")
    bkgLineColors["W"]  = col.GetColor("#3ca4c8")
    #bkgLineColors["singleH"] = col.GetColor("#3ca4c8")
    bkgLineColors["other"]   = col.GetColor("#d85a56")


    #if args.sigscale:
    #     for i in range(0,len(sigScale)): sigScale[i] = args.sigscale
    #sigScale = [20,100]
    sigScale = [20]
    sigScaleValue = 1000

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
    outplotterName = findInFolder(args.indir+'/', 'analyzedOutPlotter.root')
    
    if not 'Tau' in args.channel:
        outplotterName = findInFolder(args.indir+'/', 'outPlotter.root')            

    rootFile = ROOT.TFile.Open(op.join(args.indir,outplotterName))

    binning = None
    if (args.flat):
        binning = flatBinning(rootFile, sigList, args.var, args.sel, args.reg)

    opts = args.var, args.sel, args.reg, args.flat, binning
    hSigs  = retrieveHistos(rootFile, sigList, *opts)
    hBkgs  = retrieveHistos(rootFile, bkgList, *opts)
    hDatas = retrieveHistos(rootFile, dataList, *opts)

    xsecRatio = 19.56
    if not args.log:
        xsecRatio = xsecRatio/float(10)
    #sigScale = [1. , xsecRatio*hSigs["ggHH"].GetEntries()/float(hSigs["VBFC2V1"].GetEntries())]

    doOverflow = args.overflow
    
    hDY = getHisto('DY_HM', hBkgs, doOverflow)
    hTT = getHisto('TT', hBkgs, doOverflow)
    hWJets = getHisto('W', hBkgs, doOverflow)
    hothers = getHisto('other', hBkgs, doOverflow)

    hBkgList = [hothers, hWJets, hTT, hDY] ## full list for stack
    hBkgNameList = ['Others', 'W + jets', 't#bar{t}' , 'DY_HM'] # list for legend

    if doQCD:
        col2 = ROOT.TColor()
        hQCD = getHisto('QCD', hBkgs, doOverflow)
        hQCD.SetName('QCD')
        hBkgList.append(hQCD)
        hBkgNameList.append('QCD')
        bkgColors['QCD'] = col2.GetColor('#F29563') #(TColor(242,149,99)).GetNumber() #gROOT.GetColor('#F29563')
        bkgLineColors['QCD'] = col2.GetColor('#DC885A')

    #PisaOrder = [0, 1, 4, 3, 2]
    #hBkgList = [hBkgList[i] for i in PisaOrder]
    #hBkgNameList = [hBkgNameList[i] for i in PisaOrder]

    hData = getHisto('data_obs', hDatas , doOverflow).Clone('hData')

    # remove all data from blinding region before creating tgraph etc...
    if args.blindrange:
        blow = float (args.blindrange[0]) 
        bup = float (args.blindrange[1]) 
        for ibin in range (1, hData.GetNbinsX()+1):
            center = hData.GetBinCenter(ibin)
            if center > blow and center < bup:
                hData.SetBinContent(ibin, 0)

    hDataNonScaled = hData.Clone("hDataNonScaled")
    gData = makeTGraphFromHist(hData, "grData")

    # apply sig color if available
    for key in hSigs:
        hSigs[key].SetLineWidth(2)
        if doOverflow: hSigs[key] = addOverFlow(hSigs[key])
        if key in sigColors:
            thecolor = sigColors[key]
            hSigs[key].SetLineColor(thecolor)
            #hSigs[key].SetLineStyle(9)


    # apply bkg color if available
    for h in hBkgList:
        histoname = h.GetName()
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

            
    #################### REMOVE NEGATIVE BINS #######################
    print("** INFO: removing all negative bins from bkg histos")
    makeNonNegativeHistos (hBkgList)

    def print_integral(h, p=False):
        s = 'Integral: ' + h.GetName() + ' : '
        s += str(h.Integral()) + ' - '
        s += str(h.Integral(-1,-1))
        if p:
            print(s)
        return s

    import os.path as op
    interm_path = lambda s: op.join(args.channel, s + '_' + args.reg)
    if "class" in args.sel:
        yields_name = op.join(args.outdir, interm_path('scores_'), 'yields.txt')
        with open(yields_name, 'a+') as yields_file:
            yields_file.write('=== '+ op.join(interm_path('scores_'), args.var) + ' ===\n')
            for h in hBkgList:
                yields_file.write(print_integral(h))
            for h in hDatas.values:
                yields_file.write(print_integral(h))
            for i, name in enumerate(sigNameList):
                yields_file.write(print_integral)
    else:
        yields_name = op.join(args.outdir, interm_path(args.sel), 'yields.txt')
        with open(yields_name, 'a+') as yields_file:
            yields_file.write('=== ' + op.join(interm_path(args.sel), args.var) + ' ===\n')
            for h in hBkgList:
                yields_file.write(print_integral(h))
            for n in hDatas:
                yields_file.write(print_integral(h))
            for i, name in enumerate(sigNameList):
                yields_file.write(print_integral(hSigs[sigList[i]]))

    for h in hBkgList:
        print_integral(h, True)
    for h in hDatas.values():
        print_integral(h, True)
    for i, name in enumerate (sigNameList):
        print_integral(hSigs[sigList[i]], True)
        
    #################### PERFORM DIVISION BY BIN WIDTH #######################
    #clones non scaled (else problems with graph ratio because I pass data evt hist)
    bkgStackNS = makeStack ('bkgStackNS', hBkgList)
    hBkgEnvelopeNS = bkgStackNS.GetStack().Last().Clone('hBkgEnvelopeNS')

    if args.binwidth:
        scaleGraphByBinWidth(gData)
        for h in hBkgList:
            h.Scale(1., 'width')
        for i, name in enumerate (sigNameList):
            histo = hSigs[sigList[i]]
            histo.Scale(1., 'width')

    #################### DO STACK AND PLOT #######################
    bkgStack = makeStack ('bkgStack', hBkgList)
    bkgSum = makeSum ('bkgSum', hBkgList)
    
    if args.log:
        pad1.SetLogy()

    ################## TITLE AND AESTETICS ############################
    bkgStack.Draw('HIST')

    bkgStack.GetXaxis().SetTitleFont(43)
    bkgStack.GetYaxis().SetTitleFont(43)
    bkgStack.GetXaxis().SetLabelFont(43)
    bkgStack.GetYaxis().SetLabelFont(43)

    bkgStack.GetXaxis().SetTitleOffset(1.0)
    bkgStack.GetYaxis().SetTitleOffset(1.4)

    bkgStack.GetXaxis().SetTitleSize(titleSize)
    bkgStack.GetYaxis().SetTitleSize(titleSize)

    bkgStack.GetXaxis().SetLabelSize(labelSize)
    bkgStack.GetYaxis().SetLabelSize(labelSize)

    if args.label: bkgStack.GetXaxis().SetTitle (args.label)
    else: bkgStack.GetXaxis().SetTitle (args.var)

    width = ( (bkgStack.GetXaxis().GetXmax() - bkgStack.GetXaxis().GetXmin()) /
              bkgStack.GetStack().Last().GetNbinsX() )
    ylabel = "Events/%.1f" % width
    if not args.binwidth:
        ylabel = "Events"
    if args.label:
        if "GeV" in args.label: ylabel +=" GeV"
    bkgStack.GetYaxis().SetTitle(ylabel)
    
    #intBkg = bkgStack.GetHistogram().Integral()
    intBkg = bkgStack.GetStack().Last().Integral()
    bkgStack.SetTitle(plotTitle)

    #for key in hSigs:
    #    intSig = hSigs[key].Integral()
    #    if intSig > 0:
    #            hSigs[key].Scale(intBkg/intSig)

    # apply sig scale
    for i, scale in enumerate (sigScale):
        histo = hSigs[sigList[i]]
        histo.Scale(scale)
    #for i, name in enumerate (sigNameList):
    #    histo = hSigs[sigList[i]]
    #    histo.Scale(sigScaleValue)

    ################## LEGEND ######################################
    legminy = args.lymin if args.lymin else 0.45
    legminx = 0.35 if len(hBkgNameList)+len(hSigs)>6 else 0.50
    leg = ROOT.TLegend(legminx, legminy, 0.90, 0.93)
    if (len(hBkgNameList) +len(hSigs)> 6):
        leg.SetNColumns(2)
    leg.SetFillStyle(0)
    leg.SetBorderSize(0)
    leg.SetTextFont(43)

    # add element in same order as stack --> top-bottom
    if args.dosig:
        #for i, name in enumerate (sigNameList):
        for i, name in reversed(list(enumerate (sigNameList))):
            histo = hSigs[sigList[i]]
            leg.AddEntry (histo, name, 'l')
        # null entry to complete signal Xsection
        if args.siglegextratext:
            leg.AddEntry(None, args.siglegextratext, '')

    for i, name in reversed(list(enumerate(hBkgNameList))):
        leg.AddEntry(hBkgList[i], name, 'f')

    if args.dodata:
        leg.AddEntry(gData, 'Data', 'pe')


    ################## Y RANGE SETTINGS ############################
    ymin = 0.1 if args.log else 0.

    maxs = []
    
    # bkgmax = 0
    # for h in hBkgList: bkgmax+= h.GetMaximum()
    # maxs.append(bkgmax)
    maxs.append(bkgStack.GetStack().Last().GetMaximum())

    if args.dodata:
        maxs.append(findMaxOfGraph(gData))
        #if not args.binwidth:
        #    maxs.append(hData.GetMaximum() + math.sqrt(hData.GetMaximum()))
        
    if args.dosig :
        for key in hSigs: maxs.append(hSigs[key].GetMaximum())

    ymax = max(maxs)

    # scale max to leave some space (~10%)
    extraspace = 0.5

    if not args.log:
        ymax += extraspace* (ymax-ymin)
    else:
        new = extraspace * (math.log(ymax, 10) - math.log(ymin, 10)) + math.log(ymax, 10)
        ymax = math.pow(10, new)

    ## override form args
    if args.ymin:
        ymin = args.ymin
    if args.ymax:
        ymax = args.ymax

    bkgStack.SetMinimum(ymin)
    bkgStack.SetMaximum(ymax)

    # interactive display
    bkgStack.Draw('HIST')
    bkgSum.SetFillColor(ROOT.kGray+2);
    bkgSum.SetFillStyle(3002);
    bkgSum.Draw('e2 same')
    if args.dosig:
        for key in hSigs: hSigs[key].Draw('hist same')
    if args.dodata:
        removeHErrors(gData)
        removeEmptyPoints(gData)
        gData.Draw('P Z same') # Z: no small line at the end of error bar

    ###################### OTHER TEXT ON PLOT #########################

    # extraText = "preliminary"
    # CMStext = "CMS"
    cmsTextFont = 61  # font of the "CMS" label
    cmsTextSize = 0.05  # font size of the "CMS" label
    extraTextFont = 52     # for the "preliminary"
    extraTextSize = 0.76 * cmsTextSize # for the "preliminary"

    t = ROOT.gPad.GetTopMargin()
    b = ROOT.gPad.GetBottomMargin()
    l = ROOT.gPad.GetLeftMargin()
    r = ROOT.gPad.GetRightMargin()       
    #yoffset = 0.05 # fractional shift   

    CMSbox       = ROOT.TLatex(l , 1 - t + 0.02, 'CMS')       
    extraTextBox = ROOT.TLatex(l + 0.1 , 1 - t + 0.02, 'Preliminary')
    CMSbox.SetNDC()
    extraTextBox.SetNDC()
    CMSbox.SetTextSize(cmsTextSize)
    CMSbox.SetTextFont(cmsTextFont)
    CMSbox.SetTextColor(ROOT.kBlack)
    CMSbox.SetTextAlign(11)
    extraTextBox.SetTextSize(extraTextSize)
    extraTextBox.SetTextFont(extraTextFont)
    extraTextBox.SetTextColor(ROOT.kBlack)
    extraTextBox.SetTextAlign(11)

    x = 0
    y = 0 

    lumi = '%.1f fb^{-1} (13 TeV)' % args.lumi_num
    lumibox = ROOT.TLatex(1-r, 1 - t + 0.02 , lumi)       
    lumibox.SetNDC()
    lumibox.SetTextAlign(31)
    lumibox.SetTextSize(extraTextSize)
    lumibox.SetTextFont(42)
    lumibox.SetTextColor(ROOT.kBlack)

    if args.channel == 'MuTau':
        chName = 'bb #mu#tau_{h}'
    elif args.channel == 'ETau':
        chName = 'bb e#tau_{h}'
    elif args.channel == 'TauTau':
        chName = 'bb #tau_{h}#tau_{h}'

    if chName:
        chBox = ROOT.TLatex(l + 0.04 , 1 - t - 0.02, chName)
        chBox.SetNDC()
        chBox.SetTextSize(cmsTextSize+20)
        chBox.SetTextFont(43)
        chBox.SetTextColor(ROOT.kBlack)
        chBox.SetTextAlign(13)
            
    CMSbox.Draw()
    extraTextBox.Draw()
    lumibox.Draw()
    
    if args.legend: leg.Draw()
    if chBox: chBox.Draw()

    if not args.name:
        if "baseline" == args.sel:
            selName = "baseline"
        if "baselineInclMeth" == args.sel:
            selName = "baselineInclMeth"
        if "baselineComparison" == args.sel:
            selName = "baselineComparison"
        if "baselineComparisonSF" == args.sel:
            selName = "baselineComparisonSF"
        if "1b1j" in args.sel:
            selName = "1b1j"
        if "2b0j" in args.sel:
            selName = "2b0j"
        if "boosted" in args.sel:
            selName = "boosted"
        if "antiB" in args.sel:
            selName = "antiB"
        if "DYreg" in args.sel:
            selName = "DYreg"
        if "VBFloose" in args.sel:
            selName = "VBFloose"
        if "GGFclass" in args.sel:
            selName = "GGFclass"
        if "VBFclass" in args.sel:
            selName = "VBFclass"
        if "DYclass" in args.sel:
            selName = "DYclass"
        if "ttHclass" in args.sel:
            selName = "ttHclass"
        if "TTlepclass" in args.sel:
            selName = "TTlepclass"
        if "TThadclass" in args.sel:
            selName = "TThadclass"
    else:
        selName = args.name

    selBox = ROOT.TLatex(l + 0.04 , 1 - t - 0.02 - 0.06, selName)
    selBox.SetNDC()
    selBox.SetTextSize(cmsTextSize+20)
    selBox.SetTextFont(43)
    selBox.SetTextColor(ROOT.kBlack)
    selBox.SetTextAlign(13)
    selBox.Draw()
    
    ###################### BLINDING BOX ###############################
    #if args.blindrange:
    #    blow = float(args.blindrange[0])
    #    bup = float(args.blindrange[1])
    #    bBox = TBox (blow, ymin, bup, 0.7*ymax)
    #    bBox.SetFillStyle(3002) # NB: does not appear the same in displayed box and printed pdf!!
    #    bBox.SetFillColor(ROOT.kGray+2) # NB: does not appear the same in displayed box and printed pdf!!
    #    bBox.Draw()

    ###################### S/(S+B) PLOT #################################
    if args.sbs:
        bkgStack.GetXaxis().SetTitleSize(0.00);
        bkgStack.GetXaxis().SetLabelSize(0.00);

        c1.cd()
        pad2 = ROOT.TPad("pad2", "pad2", 0, 0.0, 1, 0.2496)
        pad2.SetLeftMargin(0.12);
        pad2.SetTopMargin(0.02);
        pad2.SetBottomMargin(0.4);
        pad2.SetGridy(True);
        pad2.SetFrameLineWidth(3)
        pad2.Draw()
        pad2.cd()

        # create list of signal histograms clones
        hSigsNonScaled = []
        for nameSig in sigList:
            histSig = hSigs[nameSig].Clone()
            hSigsNonScaled.append(histSig)

        grSBSs = []
        for sigNS in hSigsNonScaled:
            #grSBS = makeSBSPlot (sigNS, hBkgEnvelopeNS, "S/(S+B)", sigNS.GetLineColor())
            grSBS = makeSBSPlot (sigNS, hBkgEnvelopeNS, "Z_{A}", sigNS.GetLineColor())
            grSBSs.append(grSBS)

        hRatio = hDataNonScaled.Clone('hRatioAxis') # for ranges only
        hRatio.GetXaxis().SetTitleFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetTitleFont(43) # so that size is in pixels
        hRatio.GetXaxis().SetLabelFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetLabelFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetNdivisions(505)

        #hRatio.GetXaxis().SetTitle(bkgStack.GetXaxis().GetName())
        hRatio.SetTitle(plotTitle)
        hRatio.GetYaxis().SetTitle ('Z_{A}') #('S/#sqrt{S+B}') #('Data/MC')
        if args.label: hRatio.GetXaxis().SetTitle (args.label)
        else: hRatio.GetXaxis().SetTitle (args.var)
        hRatio.GetXaxis().SetTitleOffset(3.9)
        hRatio.GetYaxis().SetTitleOffset(1.2)

        hRatio.GetXaxis().SetTitleSize(titleSize);
        hRatio.GetXaxis().SetLabelSize(labelSize);
        hRatio.GetYaxis().SetTitleSize(titleSize);
        hRatio.GetYaxis().SetLabelSize(labelSize);

        hRatio.GetXaxis().SetTickSize(0.10)
        hRatio.GetYaxis().SetTickSize(0.05)
        hRatio.SetStats(0)

        minSBS = 9999999999999.0
        maxSBS = -1.0
        for gr in grSBSs:
            min = TMath.MinElement(gr.GetN(),gr.GetY())
            max = TMath.MaxElement(gr.GetN(),gr.GetY())
            if min < minSBS: minSBS = min
            if max > maxSBS: maxSBS = max

        hRatio.SetMinimum(minSBS - ((maxSBS-minSBS)/10.0))
        hRatio.SetMaximum(maxSBS + ((maxSBS-minSBS)/10.0))

        removeEmptyPoints (grSBS)

        hRatio.Draw("axis")
        for gr in grSBSs:
            gr.Draw("L same") # Z : no small limes at the end of points

        pad2.RedrawAxis();
        pad2.RedrawAxis("g"); #otherwise no grid..


    ###################### RATIO PLOT #################################
    tagch = args.channel
    if not args.binwidth:
        tagch += '_noBinWidth'
    id_name = '_'.join((args.var, args.sel, args.reg, tagch))

    if args.ratio:
        bkgStack.GetXaxis().SetTitleSize(0.00);
        bkgStack.GetXaxis().SetLabelSize(0.00);

        c1.cd()
        pad2 = ROOT.TPad('pad2', 'pad2', 0, 0.0, 1, 0.2496)
        pad2.SetLeftMargin(0.12);
        pad2.SetTopMargin(0.02);
        pad2.SetBottomMargin(0.4);
        pad2.SetGridy(True);
        pad2.SetFrameLineWidth(3)
        #pad2.SetGridx(True);
        pad2.Draw()
        pad2.cd()

        grRatio = makeDataOverMCRatioPlot(hDataNonScaled, hBkgEnvelopeNS, 'grRatio')
        hRatio = hDataNonScaled.Clone('hRatioAxis') # for ranges only
        grUncert = makeMCUncertaintyBand(bkgSum) # uncertainty band from MC, always centered at 1.0

        hRatio.GetXaxis().SetTitleFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetTitleFont(43) # so that size is in pixels
        hRatio.GetXaxis().SetLabelFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetLabelFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetNdivisions(505)

        #hRatio.GetXaxis().SetTitle(bkgStack.GetXaxis().GetName())
        hRatio.SetTitle(plotTitle)
        hRatio.GetYaxis().SetTitle ('Data/Bkg.')
        if args.label:
            hRatio.GetXaxis().SetTitle(args.label)
        else: hRatio.GetXaxis().SetTitle (args.var)
        hRatio.GetXaxis().SetTitleOffset(3.9)
        hRatio.GetYaxis().SetTitleOffset(1.2)

        hRatio.GetXaxis().SetTitleSize(titleSize);
        hRatio.GetXaxis().SetLabelSize(labelSize);
        hRatio.GetYaxis().SetTitleSize(titleSize);
        hRatio.GetYaxis().SetLabelSize(labelSize);

        hRatio.GetXaxis().SetTickSize(0.10)
        hRatio.GetYaxis().SetTickSize(0.05)

        hRatio.SetStats(0)
        hRatio.SetMinimum(0.6) #default value
        hRatio.SetMaximum(1.4) #default value

        #removeEmptyPoints(grRatio)
        hRatio.Draw('axis')
        
        grRatio.Draw('P Z same') # Z : no small limes at the end of points
        xmin = hRatio.GetXaxis().GetXmin()
        xmax = hRatio.GetXaxis().GetXmax()
        l1 = ROOT.TLine(xmin, 1, xmax, 1)
        l1.SetLineColor(ROOT.kRed)
        l1.SetLineStyle(4)
        l1.SetLineWidth(1)
        l1.Draw('same')

        grUncert.SetFillColor(ROOT.kGray+2)
        grUncert.SetFillStyle(3002)
        grUncert.Draw('e2')

        pad2.RedrawAxis();
        pad2.RedrawAxis('g'); #otherwise no grid..

        # save the ratio for later comparisons
        ratioDir = op.join(args.outdir, interm_path(args.sel))
        ratioName = op.join(ratioDir, 'ratios.root')
        ratioFile = ROOT.TFile.Open(ratioName, 'UPDATE')
        grRatio.SetName(id_name)
        grRatio.Write()
        ratioFile.Close()
        print('Ratio file saved: {}.'.format(ratioName))
        
    ###################### DISPLAY ###################################
    if not args.quit:
        c1.cd()
        c1.Update()
        pad1.Update()
        if pad2:
            pad2.Update()
        raw_input() # to prevent script from closing

    if args.printplot:
        if 'class' in args.sel:
            saveName = op.join(args.outdir, interm_path('scores_'), id_name)
        else:
            saveName = op.join(args.outdir, interm_path(args.sel), id_name)
        saveName = saveName + ('_log' if args.log else '_flat')
        for ext in ('png',):
            c1.SaveAs(saveName + '.' + ext)
