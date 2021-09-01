from ROOT import *
import re
import os
import sys
import argparse
import fnmatch
import math
from array import array
import modules.ConfigReader as cfgr
import modules.OutputManager as omng
import pandas as pd
from scipy.stats import poisson, chi2

                        
def getHisto (histoName,inputList,doOverflow):
    for idx, name in enumerate(inputList):

            if (name.startswith(histoName) and name.endswith(histoName)):
                    h = inputList[name].Clone(histoName)
                    if doOverflow: h = addOverFlow(h)
                    break
    return h

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
    if args.label:
            dummy.GetXaxis().SetTitle(args.label)
    else:
            dummy.GetXaxis().SetTitle(args.var)
    dummy.SetName (name) 
    histo, dummy = dummy, histo
    return histo
    
# NB!! need to be called BEFORE removeHErrors or cannot know bin width
def scaleGraphByBinWidth (graph, binNames):
    for ipt in range (0, len(binNames)-1):
        bwh = float(binNames[ipt+1])
        bwl = float(binNames[ipt])
        bw = bwh - bwl

        eyh = graph.GetErrorYhigh(ipt)
        eyl = graph.GetErrorYlow(ipt)
        x = Double(0.0)
        y = Double(0.0)
        graph.GetPoint (ipt, x, y)
        graph.SetPoint (ipt, x, y/bw)
        graph.SetPointEYlow(ipt, eyl/bw)
        graph.SetPointEYhigh(ipt, eyh/bw)

def scaleHistoByBinWidth (histo, binNames):
    for ipt in range (1, len(binNames)):
        bwh = float(binNames[ipt])
        bwl = float(binNames[ipt-1])
        bw = bwh - bwl

        #histo.AddBinContent(ipt,bw)
        histo.SetBinContent(ipt, histo.GetBinContent(ipt)/bw)
        histo.SetBinError(ipt, histo.GetBinError(ipt)/bw)

# Get the STATISTICAL ONLY uncertainty band to be plotted in the ratio plot
def makeUncertaintyBand (bkgSum):
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
        else:
            fX.append      (bkgSum.GetBinCenter(ibin))
            fY.append      (0.)
            feYUp.append   (0.)
            feYDown.append (0.)
            feXRight.append(0.)
            feXLeft.append (0.)

    afX       = array ("d", fX      )
    afY       = array ("d", fY      )
    afeYUp    = array ("d", feYUp   )
    afeYDown  = array ("d", feYDown )
    afeXRight = array ("d", feXRight)
    afeXLeft  = array ("d", feXLeft )
    gBand = TGraphAsymmErrors (len(afX), afX, afY, afeXLeft, afeXRight, afeYDown, afeYUp)
    return gBand

## do ratio of Data/MC
# horErrs : do horizontal errors
def makeDataOverMCRatioPlot (gData, hMC, newName, horErrs=False):
    nPoints = hMC.GetNbinsX()
    fX       = []
    fY       = []
    feYUp    = []
    feYDown  = []
    feXRight = []
    feXLeft  = []

    for ibin in range (1, nPoints+1):
        num = gData.Eval(hMC.GetBinCenter(ibin))
        den = hMC.GetBinContent(ibin)
    
        if den > 0:
            # Y
            fY.append(num/den)
            feYUp.append(gData.GetErrorYhigh(ibin - 1) / den)
            feYDown.append(gData.GetErrorYlow(ibin - 1) / den)

        # X
        fX.append (hMC.GetBinCenter(ibin))
        if horErrs:
            feXRight.append(hMC.GetBinLowEdge(ibin+1) - hMC.GetBinCenter(ibin))
            feXLeft.append(hMC.GetBinCenter(ibin) - hMC.GetBinLowEdge(ibin))
        else:
            feXLeft.append(0.0)
            feXRight.append(0.0)

    afX       = array ("d", fX      )
    afY       = array ("d", fY      )
    afeYUp    = array ("d", feYUp   )
    afeYDown  = array ("d", feYDown )
    afeXRight = array ("d", feXRight)
    afeXLeft  = array ("d", feXLeft )
    gRatio = TGraphAsymmErrors (len(afX), afX, afY, afeXLeft, afeXRight, afeYDown, afeYUp)
    
    gRatio.SetMarkerStyle(8)
    gRatio.SetMarkerSize(1.)
    gRatio.SetMarkerColor(kBlack)
    gRatio.SetLineColor(kBlack)
    gRatio.SetName(newName)

    return gRatio

def makeSgnBkgOverBkgRatioPlot (hBkg, hSgn, sgn):
    nBins = hBkg.GetNbinsX()

    hRatio = TH1F("SgnBkgOverBkg", "SgnBkgOverBkg", nBins, 0, nBins)

    for ibin in range (1, nBins+1):
        num = hBkg.GetBinContent(ibin) + hSgn.GetBinContent(ibin)
        den = hBkg.GetBinContent(ibin)

        if den != 0:
            hRatio.SetBinContent(ibin, num / den)

    if sgn == 'ggHH': hRatio.SetLineColor(kBlack)
    else: hRatio.SetLineColor(kCyan)
    hRatio.SetFillColor(0) # the ratio histogram is always "transparent"
    hRatio.SetLineWidth(2)


    return hRatio

## find maximum of tgraph, including error
def findMaxOfGraph (graph):
    uppers = []
    for i in range (0, graph.GetN()):
        x = Double(0.0)
        y = Double(0.0)
        graph.GetPoint(i, x, y)
        uppers.append (y + graph.GetErrorYhigh(i))
    return max(uppers)

## remove negative bins and reset yield accordingly (WORKS ON A LIST)
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

## remove negative bins and reset yield accordingly (WORKS ON A SINGLE HISTO)
## NB: must be done BEFORE bin width division
def makeNonNegativeHisto (h): 
    integral = h.Integral()
    for b in range (1, h.GetNbinsX()+1):
        if (h.GetBinContent(b) < 0):
           h.SetBinContent (b, 0)
    integralNew = h.Integral()        
    #if (integralNew != integral):
    #    print "** INFO: removed neg bins from histo " , h.GetName() 
    
    # print h.GetName() , integral , integralNew
    if integralNew == 0:
        h.Scale(0)
    else:
        h.Scale(integral/integralNew)

def scaleStatSystUncertaintyBandForStack (grUncert,bkgSum):
    nPoints   = bkgSum.GetNbinsX()
    fX        = []
    fY        = []
    feYUp     = []
    feYDown   = []
    feXRight  = []
    feXLeft   = []

    # in this loop remember that indexing of hists and graphs are shifted by 1
    for ibin in range (1, nPoints+1):
        central = bkgSum.GetBinContent(ibin)
        fX.append      (bkgSum.GetBinCenter(ibin))
        fY.append      (central)
        feYUp.append   (grUncert.GetErrorYhigh(ibin - 1) * central)
        feYDown.append (grUncert.GetErrorYlow(ibin - 1) * central)
        feXRight.append(grUncert.GetErrorXhigh(ibin - 1))
        feXLeft.append (grUncert.GetErrorXlow(ibin - 1))

    afX       = array ("d", fX      )
    afY       = array ("d", fY      )
    afeYUp    = array ("d", feYUp   )
    afeYDown  = array ("d", feYDown )
    afeXRight = array ("d", feXRight)
    afeXLeft  = array ("d", feXLeft )

    gBand = TGraphAsymmErrors (len(afX), afX, afY, afeXLeft, afeXRight, afeYDown, afeYUp)
    return gBand

def pois_conf_int(n, alpha=1-0.683):
    l = chi2.ppf(alpha / 2, 2 * n) / 2 if n > 0 else 0
    u = chi2.ppf(1 - alpha / 2, 2 * (n + 1)) / 2
    return l, u

def setPoissonDataIntervals (graph, binNames, p=0.683):
    for ipt in range (0, len(binNames)-1):
        x = Double(0.0)
        N = Double(0.0)
        graph.GetPoint (ipt, x, N)

        # calculate the absolute value of the edges of the intervals
        eyl, eyh = pois_conf_int(N, alpha=1-p)

        # set the error as the value of the edges of the intervals relative to the observed value N
        graph.SetPointEYlow(ipt, N-eyl)
        graph.SetPointEYhigh(ipt, eyh-N) 

#######################################################################
######################### SCRIPT BODY #################################
#######################################################################


if __name__ == "__main__" :
    TH1.AddDirectory(0)

    titleSize = 22
    labelSize = 20

    # parse user's options
    parser = argparse.ArgumentParser(description='Command line parser of plotting options')
    # string options
    parser.add_argument('--var', dest='var', help='variable name', default=None)
    parser.add_argument('--sel', dest='sel', help='selection name', default=None)
    parser.add_argument('--name', dest='name', help='selection name for plot', default=None)
    parser.add_argument('--dir', dest='dir', help='analysis output folder name', default="./")
    parser.add_argument('--reg', dest='reg', help='region name', default=None)
    parser.add_argument('--title', dest='title', help='plot title', default=None)
    parser.add_argument('--label', dest='label', help='x label', default=None)
    parser.add_argument('--channel', dest='channel', help='channel = (MuTau, ETau, TauTau)', default=None)
    parser.add_argument('--year', dest='year', help='year', default="2018")
    parser.add_argument('--tag', dest='tag', help='string appended to output folder name', default='')
    parser.add_argument('--postFitFile', dest='postFitFile', help='file containing postfit shapes and nuisances', default='')
    parser.add_argument('--prepost', dest='prepost', help='do postfit plots or check prefit ones?', default='postfit')
    # bool options
    parser.add_argument('--log', dest='log', help='use log scale',  action='store_true', default=False)
    parser.add_argument('--no-data', dest='dodata', help='disable plotting data', action='store_false', default=True)
    parser.add_argument('--no-sig', dest='dosig',  help='disable plotting signal', action='store_false', default=True)
    parser.add_argument('--no-legend', dest='legend',   help='disable drawing legend', action='store_false', default=True)
    parser.add_argument('--binwidth', dest='binwidth', help='activate scaling by bin width', action='store_true', default=False)
    parser.add_argument('--ratio', dest='ratio', help='do ratio plot at the botton', action='store_true', default=False)
    parser.add_argument('--no-print', dest='printplot', help='no pdf output', action='store_false', default=True)
    parser.add_argument('--quit', dest='quit', help='quit at the end of the script, no interactive window', action='store_true', default=False)
    parser.add_argument('--overflow', dest='overflow', help='add overflow bin', action='store_true', default=False)
    parser.add_argument('--dynamicRatioY', dest='dynamicRatioY', help='ratio plot with ad hoc y-range?', default=False)
    parser.add_argument('--ratiosig', dest='ratiosig', help='draw signals in the ratio plot', action='store_true', default=False)
    parser.add_argument('--manualDataUnc', dest='manualDataUnc', help='calculate manually the 68 interval on data points instead of reading it from file?', action='store_true', default=False)
    # list options
    parser.add_argument('--blind-range',   dest='blindrange', nargs=2, help='start and end of blinding range', default=[1.0,2.0])
    parser.add_argument('--sigscale', dest='sigscale', nargs=2, help='scale to apply to the signals (GGHH VBFHH)', default=None)
    # float options
    parser.add_argument('--lymin', dest='lymin', type=float, help='legend min y position in pad fraction', default=None)
    # store parsed options
    args = parser.parse_args()


    ######################### INITIZLIZATIONS ####################
    if args.quit : gROOT.SetBatch(True)

    c1 = TCanvas ("c1", "c1", 600, 600)

    pad1 = None
    pad2 = None

    if args.ratio:
        #pad1 = TPad ("pad1", "pad1", 0, 0.25, 1, 1.0)
        pad1 = TPad ("pad1", "pad1", 0, 0.31, 1, 1.0)
        pad1.SetFrameLineWidth(3)
        pad1.SetLeftMargin(0.12)
        pad1.SetBottomMargin(0.02)
        pad1.SetTopMargin(0.055)
        pad1.Draw()
    else:
        pad1 = TPad ("pad1", "pad1", 0, 0.0, 1.0, 1.0)
        pad1.SetFrameLineWidth(3)
        pad1.SetLeftMargin(0.12)
        pad1.SetBottomMargin(0.12)
        pad1.SetTopMargin(0.055)
        pad1.Draw()

    pad1.cd()
    if args.log: pad1.SetLogy()


    ######################### USER CONFIGURATION ####################
    if args.postFitFile == '': 
        print('** ERROR: no file containing postfit shapes and nuisances specified')
        print('** EXITING')
        exit()

    # the channel-bin assignement is done with ROOT's alphabetical order (alphabetical order with precedence to capital letters)
    if args.channel == 'ETau':
        if args.year == '2016':
            if "1b1j" in args.sel:        combine_ch = 'ch55'
            elif "2b0j" in args.sel:      combine_ch = 'ch64'
            elif "boosted" in args.sel:   combine_ch = 'ch1'
            elif "GGFclass" in args.sel:  combine_ch = 'ch19'
            elif "VBFclass" in args.sel:  combine_ch = 'ch37'
            elif "DYclass" in args.sel:   combine_ch = 'ch10'
            elif "ttHclass" in args.sel:  combine_ch = 'ch46'
            elif "TTclass" in args.sel:   combine_ch = 'ch28'
            else: print('** ERROR: incorrect region name'); print('** EXITING'); exit()
        elif args.year == '2017':
            if "1b1j" in args.sel:        combine_ch = 'ch56'
            elif "2b0j" in args.sel:      combine_ch = 'ch65'
            elif "boosted" in args.sel:   combine_ch = 'ch2'
            elif "GGFclass" in args.sel:  combine_ch = 'ch20'
            elif "VBFclass" in args.sel:  combine_ch = 'ch38'
            elif "DYclass" in args.sel:   combine_ch = 'ch11'
            elif "ttHclass" in args.sel:  combine_ch = 'ch47'
            elif "TTclass" in args.sel:   combine_ch = 'ch29'
            else: print('** ERROR: incorrect region name'); print('** EXITING'); exit()
        elif args.year == '2018':
            if "1b1j" in args.sel:        combine_ch = 'ch57'
            elif "2b0j" in args.sel:      combine_ch = 'ch66'
            elif "boosted" in args.sel:   combine_ch = 'ch3'
            elif "GGFclass" in args.sel:  combine_ch = 'ch21'
            elif "VBFclass" in args.sel:  combine_ch = 'ch39'
            elif "DYclass" in args.sel:   combine_ch = 'ch12'
            elif "ttHclass" in args.sel:  combine_ch = 'ch48'
            elif "TTclass" in args.sel:   combine_ch = 'ch30'
            else: print('** ERROR: incorrect region name'); print('** EXITING'); exit()
        else: print('** ERROR: incorrect year'); print('** EXITING'); exit()
    elif args.channel == 'MuTau':
        if args.year == '2016':
            if "1b1j" in args.sel:        combine_ch = 'ch58'
            elif "2b0j" in args.sel:      combine_ch = 'ch67'
            elif "boosted" in args.sel:   combine_ch = 'ch4'
            elif "GGFclass" in args.sel:  combine_ch = 'ch22'
            elif "VBFclass" in args.sel:  combine_ch = 'ch40'
            elif "DYclass" in args.sel:   combine_ch = 'ch13'
            elif "ttHclass" in args.sel:  combine_ch = 'ch49'
            elif "TTclass" in args.sel:   combine_ch = 'ch31'
            else: print('** ERROR: incorrect region name'); print('** EXITING'); exit()
        elif args.year == '2017':
            if "1b1j" in args.sel:        combine_ch = 'ch59'
            elif "2b0j" in args.sel:      combine_ch = 'ch68'
            elif "boosted" in args.sel:   combine_ch = 'ch5'
            elif "GGFclass" in args.sel:  combine_ch = 'ch23'
            elif "VBFclass" in args.sel:  combine_ch = 'ch41'
            elif "DYclass" in args.sel:   combine_ch = 'ch14'
            elif "ttHclass" in args.sel:  combine_ch = 'ch50'
            elif "TTclass" in args.sel:   combine_ch = 'ch32'
            else: print('** ERROR: incorrect region name'); print('** EXITING'); exit()
        elif args.year == '2018':
            if "1b1j" in args.sel:        combine_ch = 'ch60'
            elif "2b0j" in args.sel:      combine_ch = 'ch69'
            elif "boosted" in args.sel:   combine_ch = 'ch6'
            elif "GGFclass" in args.sel:  combine_ch = 'ch24'
            elif "VBFclass" in args.sel:  combine_ch = 'ch42'
            elif "DYclass" in args.sel:   combine_ch = 'ch15'
            elif "ttHclass" in args.sel:  combine_ch = 'ch51'
            elif "TTclass" in args.sel:   combine_ch = 'ch33'
            else: print('** ERROR: incorrect region name'); print('** EXITING'); exit()
        else: print('** ERROR: incorrect year'); print('** EXITING'); exit()
    elif args.channel == 'TauTau':
        if args.year == '2016':
            if "1b1j" in args.sel:        combine_ch = 'ch61'
            elif "2b0j" in args.sel:      combine_ch = 'ch70'
            elif "boosted" in args.sel:   combine_ch = 'ch7'
            elif "GGFclass" in args.sel:  combine_ch = 'ch25'
            elif "VBFclass" in args.sel:  combine_ch = 'ch43'
            elif "DYclass" in args.sel:   combine_ch = 'ch16'
            elif "ttHclass" in args.sel:  combine_ch = 'ch52'
            elif "TTclass" in args.sel:   combine_ch = 'ch34'
            else: print('** ERROR: incorrect region name'); print('** EXITING'); exit()
        elif args.year == '2017':
            if "1b1j" in args.sel:        combine_ch = 'ch62'
            elif "2b0j" in args.sel:      combine_ch = 'ch71'
            elif "boosted" in args.sel:   combine_ch = 'ch8'
            elif "GGFclass" in args.sel:  combine_ch = 'ch26'
            elif "VBFclass" in args.sel:  combine_ch = 'ch44'
            elif "DYclass" in args.sel:   combine_ch = 'ch17'
            elif "ttHclass" in args.sel:  combine_ch = 'ch53'
            elif "TTclass" in args.sel:   combine_ch = 'ch35'
            else: print('** ERROR: incorrect region name'); print('** EXITING'); exit()
        elif args.year == '2018':
            if "1b1j" in args.sel:        combine_ch = 'ch63'
            elif "2b0j" in args.sel:      combine_ch = 'ch72'
            elif "boosted" in args.sel:   combine_ch = 'ch9'
            elif "GGFclass" in args.sel:  combine_ch = 'ch27'
            elif "VBFclass" in args.sel:  combine_ch = 'ch45'
            elif "DYclass" in args.sel:   combine_ch = 'ch18'
            elif "ttHclass" in args.sel:  combine_ch = 'ch54'
            elif "TTclass" in args.sel:   combine_ch = 'ch36'
            else: print('** ERROR: incorrect region name'); print('** EXITING'); exit()
        else: print('** ERROR: incorrect year'); print('** EXITING'); exit()
    else: print('** ERROR: incorrect channel name'); print('** EXITING'); exit()

    if args.prepost == 'prefit': fit_folder = 'shapes_prefit'
    else: fit_folder = 'shapes_fit_s'

    sigScale = [1,1]
    if args.sigscale:
         for i in range(0,len(sigScale)): sigScale[i] = float(args.sigscale[i])

    if sigScale[0] == 1 and sigScale[1] == 1: sig_fit_folder = fit_folder
    else: sig_fit_folder = 'shapes_prefit'

    rootFile = TFile.Open(args.dir+'/'+args.postFitFile)
    list_of_shapes = rootFile.Get(fit_folder+'/'+combine_ch).GetListOfKeys()
    hBkgs = {}
    hSigs = {}
    hDatas = {}
    hTots = {}
    hSigs = { 'ggHH_kl_1_kt_1_hbbhtt'       : TH1F("dummy", "dummy", 1, 0, 1),
              'qqHH_CV_1_C2V_1_kl_1_hbbhtt' : TH1F("dummy", "dummy", 1, 0, 1)
            }
    for hist in list_of_shapes:
        name = hist.GetName()
        if ('background' in name) or ('signal' in name): hTots[name] = rootFile.Get(fit_folder+'/'+combine_ch+'/'+name)
        elif ('ggHH_kl_1_kt_1_hbbhtt' == name) or ('qqHH_CV_1_C2V_1_kl_1_hbbhtt' in name): hSigs[name] = rootFile.Get(sig_fit_folder+'/'+combine_ch+'/'+name)
        elif 'data' in name: hDatas[name] = rootFile.Get(fit_folder+'/'+combine_ch+'/'+name) # NB - data is stored as a TGRaphAsymErrors inside a ROOFit object
        elif (not 'ggHH' in name) and (not 'qqHH' in name): hBkgs[name] = rootFile.Get(fit_folder+'/'+combine_ch+'/'+name)

    sigList = ["ggHH_kl_1_kt_1_hbbhtt", "qqHH_CV_1_C2V_1_kl_1_hbbhtt"]
    if sigScale[0] != 1 and sigScale[1] != 1: sigNameList = ["Prefit ggHH SM x {0}".format(str(sigScale[0])), "Prefit qqHH SM x {0}".format(str(sigScale[1]))]
    elif args.prepost == 'prefit': sigNameList = ["Prefit ggHH", "Prefit qqHH"]
    else: sigNameList = ["Postfit ggHH", "Postfit qqHH"]

    plotTitle = ""

    if args.title:
        plotTitle = args.title

    # read the bins edges that are used from the cfg file
    cfg = cfgr.ConfigReader('config/makeLegacyPlots_binning.cfg')
    binNames = cfg.readListOption('{0}{1}::{2}'.format(args.year, args.channel, args.sel))
    binNumbs = len(binNames) - 1

    ######################### START DOING ACTUAL  PLOTTING STUFF ####################
    doOverflow = args.overflow

    gData = getHisto("data", hDatas , doOverflow) # NB - data is stored as a TGRaphAsymErrors inside a ROOFit object
    gDataNonScaled = gData.Clone("gDataNonScaled")

    # dummy empty TH1F to avoid problems when histos are not present in the fitdiagnostic.root file (we argued that if they are not there it is because they are empty)
    Npts = gData.GetN()
    hEWK = hggH = hTTH = hTTX = hVVV = hTW = hTT = hQCD = hWH = hsingleT = hWJets = hqqH = hVV = hDY = hZH = TH1F("dummy", "dummy", Npts, 0, Npts)

    if 'EWK'     in hBkgs: hEWK     = getHisto("EWK", hBkgs, doOverflow)
    if 'ggH_htt' in hBkgs: hggH     = getHisto("ggH_htt", hBkgs, doOverflow)
    if 'ttH_hbb' in hBkgs: hTTH     = getHisto("ttH_hbb", hBkgs, doOverflow)
    if 'TTX'     in hBkgs: hTTX     = getHisto("TTX", hBkgs, doOverflow)
    if 'VVV'     in hBkgs: hVVV     = getHisto("VVV", hBkgs, doOverflow)
    if 'TW'      in hBkgs: hTW      = getHisto("TW", hBkgs, doOverflow)
    if 'TT'      in hBkgs: hTT      = getHisto("TT", hBkgs, doOverflow)
    if 'QCD'     in hBkgs: hQCD     = getHisto("QCD", hBkgs,doOverflow)
    if 'WH_htt'  in hBkgs: hWH      = getHisto("WH_htt", hBkgs, doOverflow)
    if 'singleT' in hBkgs: hsingleT = getHisto("singleT", hBkgs, doOverflow)
    if 'W'       in hBkgs: hWJets   = getHisto("W", hBkgs, doOverflow)
    if 'qqH_htt' in hBkgs: hqqH     = getHisto("qqH_htt", hBkgs, doOverflow)
    if 'VV'      in hBkgs: hVV      = getHisto("VV", hBkgs, doOverflow)
    if 'DY'      in hBkgs: hDY      = getHisto("DY", hBkgs, doOverflow)
    if 'ZH_hbb'  in hBkgs: hZH      = getHisto("ZH_hbb", hBkgs, doOverflow)

    hothersList = [hEWK, hsingleT, hTW, hVV, hTTX, hVVV, hWJets]
    hSingleHlist = [hZH, hWH, hTTH, hggH, hqqH]
    hSingleH = makeSum("singleH",hSingleHlist)
    hothers = makeSum("other",hothersList)

    # Protection in case QCD is not computed in this channel/category
    if hQCD.GetName() == 'dummy':
        hBkgList     = [hothers , hSingleH, hTT, hDY]                  # list for stack
        hBkgNameList = ["Others", "Single H", "t#bar{t}", "Drell-Yan"] # list for legend
    else:
        hBkgList     = [hothers , hSingleH, hQCD, hTT, hDY]                   # list for stack
        hBkgNameList = ["Others", "Single H", "QCD", "t#bar{t}", "Drell-Yan"] # list for legend

    ######################### SET COLORS ####################
    sigColors = {}
    sigColors["ggHH_kl_1_kt_1_hbbhtt"] = kBlack
    sigColors["qqHH_CV_1_C2V_1_kl_1_hbbhtt"] = kCyan

    col = TColor()
    bkgColors = {}
    bkgColors["DY"]      = col.GetColor("#44BA68")
    bkgColors["TT"]      = col.GetColor("#F4B642")
    bkgColors["singleH"] = col.GetColor("#41B4DB")
    bkgColors["other"]   = col.GetColor("#ED635E")
    bkgColors["QCD"]     = col.GetColor("#F29563")

    bkgLineColors = {}
    bkgLineColors["DY"]      = col.GetColor("#389956")
    bkgLineColors["TT"]      = col.GetColor("#dea63c")
    bkgLineColors["singleH"] = col.GetColor("#3ca4c8")
    bkgLineColors["other"]   = col.GetColor("#d85a56")
    bkgLineColors["QCD"]     = col.GetColor("#DC885A")

    # apply sig color if available
    for key in hSigs:
        hSigs[key].SetLineWidth(2)
        if doOverflow: hSigs[key] = addOverFlow(hSigs[key])
        if key in sigColors:
            thecolor = int(sigColors[key])
            hSigs[key].SetLineColor(thecolor)


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
    for h in hBkgList: print "Integral ", h.GetName(), " : ", h.Integral(), " - ", h.Integral(-1,-1)
    for n in hDatas: print "Integral ", hDatas[n].GetName(), " : ", hDatas[n].Integral(), " - ", hDatas[n].Integral(-1,-1)
    for i, name in enumerate (sigNameList): print "Integral ", hSigs[sigList[i]].GetName(), " : ", hSigs[sigList[i]].Integral(), " - ", hSigs[sigList[i]].Integral(-1,-1)
    
    # Store yields in a txt file for reference
    with open('./LegacyPlots/Legacy' + args.year + '/' + args.channel + '_' + args.tag + '_' + args.prepost + '/' + args.sel+'/yields_'+args.var+'_'+args.sel+'_'+args.reg+'.txt','w') as yields_file:
        yields_file.write('=== Legacy' + args.year + '/' + args.channel + '_' + args.prepost + '/' + args.sel+'/'+args.reg+'/'+args.var+' ===\n')
        for h in hBkgList: yields_file.write("Integral: "+h.GetName()+" : "+str(h.Integral())+" - "+str(h.Integral(-1,-1))+"\n")
        for n in hDatas  : yields_file.write("Integral: "+hDatas[n].GetName()+" : "+str(hDatas[n].Integral())+" - "+str(hDatas[n].Integral(-1,-1))+"\n")
        for i, name in enumerate (sigNameList): yields_file.write("Integral: "+hSigs[sigList[i]].GetName()+" : "+str(hSigs[sigList[i]].Integral())+" - "+str(hSigs[sigList[i]].Integral(-1,-1))+"\n")
    yields_file.close()


    #################### PERFORM DIVISION BY BIN WIDTH AND DO NECESSARY STACKS #######################
    bkgStackNS = makeStack ("bkgStackNS", hBkgList)
    hBkgEnvelopeNS = bkgStackNS.GetStack().Last().Clone("hBkgEnvelopeNS")
    bkgSumNS = hTots['total_background']

    if args.binwidth:
        scaleGraphByBinWidth(gData, binNames)
        for h in hBkgList:
            scaleHistoByBinWidth(h, binNames)
        for i, name in enumerate (sigNameList):
            scaleHistoByBinWidth(hSigs[sigList[i]], binNames)
        for name in hTots:
            scaleHistoByBinWidth(hTots[name], binNames)

    bkgStack = makeStack ("bkgStack", hBkgList)
    bkgSum = hTots['total_background']

    ################## TITLE AND AESTETICS ############################
    bkgStack.Draw("HIST")
    bkgStack.GetXaxis().SetLimits(0,binNumbs)
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
    else: bkgStack.GetXaxis().SetTitle(args.var) 

    for ibin in range (1, bkgStack.GetHistogram().GetNbinsX()+1):
        bkgStack.GetXaxis().SetBinLabel(ibin,"")

    ylabel = "Events"    
    if args.binwidth:
        ylabel = "Events/Bin Width"
        #if args.label and "GeV" in args.label: ylabel +=" GeV"
    bkgStack.GetYaxis().SetTitle(ylabel)
    
    intBkg = bkgStack.GetStack().Last().Integral()
    bkgStack.SetTitle(plotTitle)

    # scale the signal by the specified value
    for i, scale in enumerate(sigScale):
        histo = hSigs[sigList[i]]
        histo.Scale(scale)

    if args.ratiosig == True:
        hggHHratio = makeSgnBkgOverBkgRatioPlot(hBkgEnvelopeNS, hSigs['ggHH_kl_1_kt_1_hbbhtt'], sgn='ggHH')
        hqqHHratio = makeSgnBkgOverBkgRatioPlot(hBkgEnvelopeNS, hSigs['qqHH_CV_1_C2V_1_kl_1_hbbhtt'], sgn='qqHH')


    ################## LEGEND ######################################
    legminx = 0.5
    legmin = 0.7
    if args.lymin: legmin = args.lymin
    if (len(hBkgNameList) +len(hSigs)>5): legminx = 0.325
    leg = TLegend (legminx, legmin, 0.85, 0.93)
    if (len(hBkgNameList) +len(hSigs)>5): leg.SetNColumns(2)
    leg.SetFillStyle(0)
    leg.SetBorderSize(0)
    leg.SetTextFont(43)
    #leg.SetTextSize(18)

    if args.dosig:
        for i, name in reversed(list(enumerate (sigNameList))):
            histo = hSigs[sigList[i]]
            leg.AddEntry (histo, name, "l")

    # add element in same order as stack --> top-bottom
    for i, name in reversed(list(enumerate(hBkgNameList))):
        leg.AddEntry(hBkgList[i], name, "f")

    if args.dodata:
        leg.AddEntry(gData, "Data", "pe")

    ################## Y RANGE SETTINGS ############################
    ymin = 0
    if args.log: ymin = 0.01

    maxs = []
    
    maxs.append(bkgStack.GetStack().Last().GetMaximum())

    if args.dodata:
        maxs.append(findMaxOfGraph(gData))
        #if not args.binwidth:
        #    maxs.append(hData.GetMaximum() + math.sqrt(hData.GetMaximum()))
        
    if args.manualDataUnc:
        setPoissonDataIntervals(gData, binNames, p=0.683)
        setPoissonDataIntervals(gDataNonScaled, binNames, p=0.683)

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

    bkgStack.SetMinimum(ymin)
    bkgStack.SetMaximum(ymax)

    grUncertStack = makeUncertaintyBand(bkgSum)
    grUncertRatio = makeUncertaintyBand(bkgSumNS)

    # interactive display
    grUncertStack = scaleStatSystUncertaintyBandForStack(grUncertStack, bkgSum)
    grUncertStack.SetFillColor(kGray+2)
    grUncertStack.SetFillStyle(3002)
    grUncertStack.Draw("e2")

    if args.prepost == 'prefit': leg.AddEntry(grUncertStack, "Stat+Syst pre-fit unc.", 'f')
    else: leg.AddEntry(grUncertStack, "Stat+Syst post-fit unc.", 'f')
    if args.dosig:
        for key in hSigs: hSigs[key].Draw("hist same")
    if args.dodata:
        removeHErrors(gData)
        #removeEmptyPoints(gData) # commented as was decided to keep also the 0.0 points
        gData.SetMarkerStyle(8)
        gData.SetMarkerSize(1.)
        gData.SetMarkerColor(kBlack)
        gData.SetLineColor(kBlack)
        gData.Draw("0P Z same") # Z: no small line at the end of error bar


    ###################### OTHER TEXT ON PLOT #########################
    cmsTextFont     = 61  # font of the "CMS" label
    cmsTextSize     = 0.05  # font size of the "CMS" label
    extraTextFont   = 52     # for the "preliminary"
    extraTextSize   = 0.76 * cmsTextSize # for the "preliminary"

    t = gPad.GetTopMargin()
    b = gPad.GetBottomMargin()
    l = gPad.GetLeftMargin()
    r = gPad.GetRightMargin()       

    CMSbox = TLatex(l , 1 - t + 0.02, "CMS")       
    extraTextBox = TLatex(l + 0.1 , 1 - t + 0.02,"Preliminary")
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

    if args.year == '2016': lumi = "35.9 fb^{-1} (13 TeV)"
    elif args.year == '2017': lumi = "41.5 fb^{-1} (13 TeV)"
    else: lumi = "59.7 fb^{-1} (13 TeV)"
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
        elif args.channel == "TauTau":
            chName = "bb #tau_{h}#tau_{h}"
        elif args.channel == "ttCR":
            chName = "bb e#tau_{h}/#mu#tau_{h}/#tau_{h}#tau_{h}"
        else:
            print "*** Warning: channel name must be ETau, MuTau, TauTau, or ttCR. You wrote: " , args.channel

        if chName:
            chBox = TLatex  (l + 0.04 , 1 - t - 0.02, chName)
            chBox.SetNDC()
            chBox.SetTextSize(cmsTextSize+20)
            chBox.SetTextFont(43)
            chBox.SetTextColor(kBlack)
            chBox.SetTextAlign(13)
            
    CMSbox.Draw()
    extraTextBox.Draw()
    lumibox.Draw()
    
    if args.legend: leg.Draw()
    if chBox: chBox.Draw()

    if not args.name:
            if "1b1j" in args.sel:      selName = "res1b"
            if "2b0j" in args.sel:      selName = "res2b"
            if "boosted" in args.sel:   selName = "boosted"
            if "GGFclass" in args.sel:  selName = "classGGF"
            if "VBFclass" in args.sel:  selName = "classVBF"
            if "DYclass" in args.sel:   selName = "classDY"
            if "ttHclass" in args.sel:  selName = "classttH"
            if "TTclass" in args.sel:   selName = "classTT"
    else: selName = args.name

    selBox = TLatex  (l + 0.04 , 1 - t - 0.02 - 0.06, selName)
    selBox.SetNDC()
    selBox.SetTextSize(cmsTextSize+20)
    selBox.SetTextFont(43)
    selBox.SetTextColor(kBlack)
    selBox.SetTextAlign(13)
    selBox.Draw()

    ###################### RATIO PLOT #################################
    if args.ratio:
        bkgStack.GetXaxis().SetTitleSize(0.00)
        bkgStack.GetXaxis().SetLabelSize(0.00)
        bkgSumNS.GetXaxis().SetLimits(0,binNumbs)

        c1.cd()
        #pad2 = TPad ("pad2", "pad2", 0, 0.0, 1, 0.2496)
        pad2 = TPad ("pad2", "pad2", 0, 0.0, 1, 0.3096)
        pad2.SetLeftMargin(0.12)
        pad2.SetTopMargin(0.045)
        pad2.SetBottomMargin(0.4)
        pad2.SetGridy(True)
        pad2.SetFrameLineWidth(3)
        pad2.Draw()
        pad2.cd()

        grRatio = makeDataOverMCRatioPlot(gDataNonScaled, hBkgEnvelopeNS, "grRatio")

        bkgSumNS.GetXaxis().SetTitleFont(43) # so that size is in pixels
        bkgSumNS.GetYaxis().SetTitleFont(43) # so that size is in pixels
        bkgSumNS.GetXaxis().SetLabelFont(43) # so that size is in pixels
        bkgSumNS.GetYaxis().SetLabelFont(43) # so that size is in pixels
        bkgSumNS.GetYaxis().SetNdivisions(505)

        #bkgSumNS.GetXaxis().SetTitle(bkgStack.GetXaxis().GetName())
        bkgSumNS.SetTitle(plotTitle)
        bkgSumNS.GetYaxis().SetTitle ("Data/Bkg.") #("Data/MC")
        if args.label: bkgSumNS.GetXaxis().SetTitle (args.label)
        else: bkgSumNS.GetXaxis().SetTitle (args.var)
        bkgSumNS.GetXaxis().SetTitleOffset(3.9)
        bkgSumNS.GetYaxis().SetTitleOffset(1.2)

        bkgSumNS.GetXaxis().SetTitleSize(titleSize)
        bkgSumNS.GetXaxis().SetLabelSize(labelSize)
        bkgSumNS.GetYaxis().SetTitleSize(titleSize)
        bkgSumNS.GetYaxis().SetLabelSize(labelSize)

        bkgSumNS.GetXaxis().SetTickSize(0.10)
        bkgSumNS.GetYaxis().SetTickSize(0.05)

        bkgSumNS.SetStats(0)

        #removeEmptyPoints(grRatio) # commented as was decided to keep also the 0.0 points
        
        # SET THE Y-AXIS OF THE RATIO PLOT BASED ON THE VALUES OF THE RATIO ITSELF -> IN THIS WAY THE PLOTS WILL ALWAYS BE MEANINGFUL
        if args.dynamicRatioY:
            lim_yLow = TMath.MinElement(hDataNonScaled.GetNbinsX(),grRatio.GetY())
            lim_yUp = TMath.MaxElement(hDataNonScaled.GetNbinsX(),grRatio.GetY())
            # WHEN THE VALUES OF THE RATIO ARE NOT NICE TO BE USED FOR THE PLOT WE SET THE LIMITS MANUALLY
            if lim_yLow > 1: lim_yLow = 0.75
            elif lim_yLow == 0: lim_yLow = 0.75
            if lim_yUp < 1.02: lim_yUp = 1.15
            elif lim_yUp > 2: lim_yUp = 1.35
            if "boosted" in args.sel:
                lim_yLow = 0
                lim_yUp = 3
            bkgSumNS.SetMinimum(lim_yLow-0.05)
            bkgSumNS.SetMaximum(lim_yUp+0.05)
        else:
            bkgSumNS.SetMinimum(0)
            bkgSumNS.SetMaximum(2)

        # SET THE LABELS ACCORDING TO THE BINS EDGES      
        axis = bkgSumNS.GetXaxis()

        # disable default axis labels
        axis.SetBinLabel(1, "")

        # get margins and ranges
        l = pad2.GetLeftMargin()
        r = pad2.GetRightMargin()
        b = pad2.GetBottomMargin()
        x_min = axis.GetXmin()
        x_max = axis.GetXmax()

        # create edge labels, they don't even have to be aligned to ticks
        labels = []
        for i, val in enumerate(binNames):
            x = l + (1 - r - l) * i / binNumbs
            y = b - 0.03

            label = TLatex(x, y, "{:.3f}".format(float(val)))
            label.SetNDC(True)
            label.SetTextFont(43)
            label.SetTextSize(15)

            if i == 0: label.SetTextAlign(33)
            elif i == binNumbs: label.SetTextAlign(31)
            else: label.SetTextAlign(32)
       
            label.SetTextAngle(90)
            labels.append(label)
          
        bkgSumNS.GetXaxis().SetTitleOffset(5.4)

        bkgSumNS.Draw("axis")
        bkgSumNS.GetXaxis().SetLimits(0,binNumbs)
        for label in labels:label.Draw()

        grUncertRatio.SetFillColor(kGray+2)
        grUncertRatio.SetFillStyle(3002)
        grUncertRatio.Draw("e2")
        grUncertRatio.GetXaxis().SetLimits(0,binNumbs)

        # draw the postfit signal in the ratio plot
        if args.ratiosig == True:
            hqqHHratio.Draw("HIST same")
            hggHHratio.Draw("HIST same")

        # draw the ratio
        grRatio.Draw("0P Z same") # Z : no small limes at the end of points
        xmin =bkgSumNS.GetXaxis().GetXmin()
        xmax = bkgSumNS.GetXaxis().GetXmax()
        l1 = TLine(xmin, 1, xmax, 1)
        l1.SetLineColor(kRed)
        l1.SetLineStyle(4)
        l1.SetLineWidth(1)
        l1.Draw("same")

        pad2.RedrawAxis()
        pad2.RedrawAxis("g") #otherwise no grid..

    ###################### DISPLAY ###################################
    if not args.quit:
        c1.Update()
        pad1.Update()
        if pad2: pad2.Update()
        raw_input() # to prevent script from closing


    if args.printplot:
        saveName = './LegacyPlots/Legacy' + args.year + '/' + args.channel + '_' + args.tag + '_' + args.prepost + '/' + args.sel + "/plot_" + args.var + "_" + args.sel +"_" + args.reg
        if args.log: saveName = saveName+"_log"
        if args.ratiosig: saveName = saveName+"_ratiosig"
        if args.binwidth: saveName = saveName+"_binWidth"

        c1.SaveAs (saveName+".pdf")
        c1.SaveAs (saveName+".png")

