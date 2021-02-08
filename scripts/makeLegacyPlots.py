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

def retrieveHistos (rootFile, namelist, var, sel, reg):
    res = {}
    for name in namelist:
        fullName = name + "_" + sel + "_" + reg + "_" + var

        if not rootFile.GetListOfKeys().Contains(fullName):
            print "*** WARNING: histo " , fullName , " not available"
            continue
        h = rootFile.Get(fullName)

        res[name] = h
                        
    return res
                        
def getHisto (histoName,inputList,doOverflow):

        for idx, name in enumerate(inputList):

                if (name.startswith(histoName) and name.endswith(histoName)):
                        h = inputList[name].Clone (histoName)
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
    if args.label:
            dummy.GetXaxis().SetTitle(args.label)
    else:
            dummy.GetXaxis().SetTitle(args.var)
    dummy.SetName (name) 
    histo, dummy = dummy, histo
    return histo
    
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

# Get the STATISTICAL ONLY uncertainty band to be plotted in the ratio plot
def makeStatUncertaintyBand (bkgSum):
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
# horErrs : do horizontal errors
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

def Xaxis2binNumber (histo):
    Nbins = histo.GetNbinsX()

    new_histo = TH1F(histo.GetName(), histo.GetName(), Nbins, 0.5, Nbins+0.5)    
    for j in range(1,Nbins+1):
        new_histo.SetBinContent(j,histo.GetBinContent(j))
        new_histo.SetBinError(j,histo.GetBinError(j))

    return new_histo
    
def makeStatSystUncertaintyBand (bkgSum,hBkgs,hBkgNameList,systCfgList,channel,selection,hShapes,hShapesNameList):
    # we read the systematic configs to calculate the lnN systematic uncertainty
    # we read all the shape variated historams to calculate the shape systematic uncertainty
    # as an approximation we jut sum in quadrature all the sources and do not consider any correlation

    nPoints   = bkgSum.GetNbinsX()
    fX        = []
    fY        = []
    feYUp     = []
    feYDown   = []
    feXRight  = []
    feXLeft   = []
    syst_df   = pd.DataFrame(0.0,columns=hBkgNameList, index=['lnNup', 'lnNdown'])

    # this for loop calculates the lnN uncertainty from the syst.cfg files
    # NB store the squared % value of the source!!
    for cfg in systCfgList:
        # read the file
        for line in open('config/'+cfg+'.cfg','r'):
            # split lines and skip not useful ones
            f = line.split()
            if len(f) < 1: continue
            if f[0].startswith('#'): continue # skip comments
            if f[0].startswith('qq'): continue # skipp VBFHH systs
            if f[0].startswith('gg'): continue # skip GGHH systs
            if f[0] == 'type': continue # skip type declarations (they are all lnN)
            # QCD systematics files have a different layout wrt the others, so we parse them separately
            if 'QCD' in cfg:
                if f[0].startswith('['):
                    ch = f[0].split('_')[0].replace('[','')
                    sel = f[0].split('_')[1].replace(']','')
                    gate = True # gate allows to retain the needed values while skipping the not matching ones
                    if ch != channel: gate = False; continue # skip when channel is not matching
                    if sel != selection: gate = False; continue # skip when selection is not matching
                if f[0].startswith('unc') and gate:
                    # append values to the QCD column
                    syst_df['QCD']['lnNup'] += abs(round(float(f[2].split(':')[1])-1,3))**2
                    syst_df['QCD']['lnNdown'] += abs(round(float(f[2].split(':')[1])-1,3))**2
            else:
                if f[0].startswith('['): continue # skip sections headers
                # if there is a / the up and down are different, else they are the same
                if '/' in f[2]: 
                    up = abs(round(float(f[2].split("/")[1])-1,3))
                    down = abs(round(float(f[2].split("/")[0])-1,3))
                else:
                    up = abs(round(float(f[2])-1,3))
                    down = abs(round(float(f[2])-1,3))
                # append correct values to correct place in the dataframe
                for bkg in hBkgNameList:
                    if f[0] == bkg:
                        syst_df[bkg]['lnNup'] += up**2
                        syst_df[bkg]['lnNdown'] += down**2

    # sqrt of all the entries to get the % value
    syst_df = syst_df.transform(lambda x:x**0.5)
    
    # loop on all the bins of the distribution, calculate shape uncertainties and sum all syst and stat
    for ibin in range (1, nPoints+1):
        lnNup = lnNdown = 0
        shapeUp = shapeDown = 0
        central = bkgSum.GetBinContent(ibin)
        if central <= 0: continue
        for bkg, ibkg in zip(hSystBkgNameList, range(len(hSystBkgList))):
        	# calculate the absolute number of events associated to a syst source
            lnNup += max(0,hSystBkgList[ibkg].GetBinContent(ibin)) * syst_df[bkg]['lnNup']
            lnNdown += max(0,hSystBkgList[ibkg].GetBinContent(ibin)) * syst_df[bkg]['lnNdown']

            # loop on all shape syst to get the absolute value of events associated to a shape source
            for shape in hShapesNameList:
                hShape = getHisto(bkg+'_'+shape, hShapes, doOverflow)    
                hBkg   = getHisto(bkg, hBkgs, doOverflow)
                # calculate the difference between the nominal and the variation
                diff = hShape.GetBinContent(ibin) - hBkg.GetBinContent(ibin)
                # categorize the effect as up or down just looking at the sign of the diference
                # sum in quadrature all the effects coming from all the bkgs 
                if diff > 0: shapeUp += diff**2
                if diff <= 0: shapeDown += abs(diff)**2

        shapeUp = shapeUp**0.5
        shapeDown = shapeDown**0.5
        #print(shapeUp, lnNup, '--', shapeDown, lnNdown)

        # we sum in quadrature the lnN syst unc, the shape syst unc, and statistical unc
        fX.append      (bkgSum.GetBinCenter(ibin))
        fY.append      (1.0)
        feYUp.append   ((lnNup**2 + shapeUp**2 + bkgSum.GetBinErrorUp(ibin)**2)**0.5 / central)
        feYDown.append ((lnNdown**2 + shapeDown**2 + bkgSum.GetBinErrorLow(ibin)**2)**0.5 / central)
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

def retrieveShapes (rootFile, namelist, var, sel, reg, shapeNameList):
    res = {}
    for name in namelist:
        for shapeName in shapeNameList:
            fullName = name + "_" + sel + "_" + reg + "_" + var + '_' + shapeName

            if not rootFile.GetListOfKeys().Contains(fullName):
                print "*** WARNING: histo " , fullName , " not available"
                continue
            h = rootFile.Get(fullName)

            res[name + '_' + shapeName] = h
                        
    return res

def scaleStatSystUncertaintyBandForStack (grUncert,bkgSum):
    nPoints   = bkgSum.GetNbinsX()
    fX        = []
    fY        = []
    feYUp     = []
    feYDown   = []
    feXRight  = []
    feXLeft   = []

    # in this loop remember that indexing of hists and graphs are hifted by 1
    for ibin in range (1, nPoints+1):
        central = bkgSum.GetBinContent(ibin)
        fX.append      (bkgSum.GetBinCenter(ibin))
        fY.append      (central)
        feYUp.append   (grUncert.GetErrorYhigh(ibin-1) * central)
        feYDown.append (grUncert.GetErrorYlow(ibin-1) * central)
        feXRight.append(grUncert.GetErrorXhigh(ibin-1))
        feXLeft.append (grUncert.GetErrorXlow(ibin-1))

    afX       = array ("d", fX      )
    afY       = array ("d", fY      )
    afeYUp    = array ("d", feYUp   )
    afeYDown  = array ("d", feYDown )
    afeXRight = array ("d", feXRight)
    afeXLeft  = array ("d", feXLeft )

    gBand = TGraphAsymmErrors (len(afX), afX, afY, afeXLeft, afeXRight, afeYDown, afeYUp);
    return gBand;

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
    parser.add_argument('--tag', dest='tag', help='plots output folder name', default="./")
    parser.add_argument('--reg', dest='reg', help='region name', default=None)
    parser.add_argument('--title', dest='title', help='plot title', default=None)
    parser.add_argument('--label', dest='label', help='x label', default=None)
    parser.add_argument('--channel', dest='channel', help='channel = (MuTau, ETau, TauTau)', default=None)
    parser.add_argument('--year', dest='year', help='year', default="2018")
    # bool options
    parser.add_argument('--log', dest='log', help='use log scale',  action='store_true', default=False)
    parser.add_argument('--no-data', dest='dodata', help='disable plotting data', action='store_false', default=True)
    parser.add_argument('--no-sig', dest='dosig',  help='disable plotting signal', action='store_false', default=True)
    parser.add_argument('--no-legend', dest='legend',   help='disable drawing legend', action='store_false', default=True)
    parser.add_argument('--no-binwidth', dest='binwidth', help='disable scaling by bin width', action='store_false', default=True)
    parser.add_argument('--ratio', dest='ratio', help='do ratio plot at the botton', action='store_true', default=False)
    parser.add_argument('--no-print', dest='printplot', help='no pdf output', action='store_false', default=True)
    parser.add_argument('--quit', dest='quit', help='quit at the end of the script, no interactive window', action='store_true', default=False)
    parser.add_argument('--overflow', dest='overflow', help='add overflow bin', action='store_true', default=False)
    parser.add_argument('--binNXaxis', dest='binNXaxis', help='plot using the bin number on the x-axis', action='store_true', default=False)
    parser.add_argument('--dynamicRatioY', dest='dynamicRatioY', help='ratio plot with ad hoc y-range?', default=False)
    parser.add_argument('--doStatSystBand', dest='doStatSystBand', help='create stat+syst uncertainty band?', action='store_true', default=False)
    parser.add_argument('--removeESsystBand', dest='removeESsystBand', help='remove energy scales from stat+syst band?', action='store_true', default=False)
    # list options
    parser.add_argument('--blind-range',   dest='blindrange', nargs=2, help='start and end of blinding range', default=None)
    parser.add_argument('--sigscale', dest='sigscale', nargs=2, help='scale to apply to the signals (GGHH VBFHH)', default=None)
    # float options
    parser.add_argument('--lymin', dest='lymin', type=float, help='legend min y position in pad fraction', default=None)
    # store parsed options
    args = parser.parse_args()


    ######################### INITIZLIZATIONS ####################
    if args.quit : gROOT.SetBatch(True)

    c1 = TCanvas ("c1", "c1", 600, 600)
    # c1.SetLeftMargin(0.15);
    # c1.SetBottomMargin(0.12);
    # c1.SetTopMargin(0.055);

    pad1 = None
    pad2 = None

    if args.ratio:
        pad1 = TPad ("pad1", "pad1", 0, 0.25, 1, 1.0)
        pad1.SetFrameLineWidth(3)
        pad1.SetLeftMargin(0.12);
        pad1.SetBottomMargin(0.02);
        pad1.SetTopMargin(0.055);
        pad1.Draw()
    else:
        pad1 = TPad ("pad1", "pad1", 0, 0.0, 1.0, 1.0)
        pad1.SetFrameLineWidth(3)
        pad1.SetLeftMargin(0.12);
        pad1.SetBottomMargin(0.12);
        pad1.SetTopMargin(0.055);
        pad1.Draw()

    pad1.cd()
    if args.log: pad1.SetLogy()


    ######################### USER CONFIGURATION ####################
    cfgName  =  args.dir + '/mainCfg_' + args.channel + "_Legacy" + args.year + "_limits.cfg"
    cfg        = cfgr.ConfigReader (cfgName)
    bkgList    = cfg.readListOption("general::backgrounds")

    doQCD = True
    if not "SR" in args.reg: 
        print("** WARNING: Impossible to plot QCD in this region, it is not an SR region!")
        doQCD = False
    if not "Tau" in args.channel: 
        print("** WARNING: Impossible to plot QCD in this channel, it does not contain at least one hadronic Tau!")
        doQCD = False
    if doQCD:
        bkgList.append('QCD')
    
    sigList = cfg.readListOption("general::signals")
    sigList = ["GGHH_NLO_cHHH1_xs", "VBFHH_CV_1_C2V_1_C3_1_xs"]

    if args.sigscale:
         for i in range(0,len(sigScale)): sigScale[i] = args.sigscale
    else:
        sigScale = [20,100]
        sigScaleValue = 1000

    sigNameList = []
    sigNameList = ["ggHH SM x {0}".format(str(sigScale[0])), "qqHH SM x {0}".format(str(sigScale[1]))]

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


    ######################### START DOING ACTUAL  PLOTTING STUFF ####################
    outplotterName = findInFolder  (args.dir+"/", 'analyzedOutPlotter.root')      
    rootFile = TFile.Open (args.dir+"/"+outplotterName)

    hSigs  = retrieveHistos(rootFile, sigList, args.var, args.sel, args.reg)
    hBkgs  = retrieveHistos(rootFile, bkgList, args.var, args.sel, args.reg)
    hDatas = retrieveHistos(rootFile, dataList, args.var, args.sel, args.reg)

    doOverflow = args.overflow
    
    hDY_0b_1Pt = getHisto("DY_0b_1Pt", hBkgs, doOverflow)
    hDY_0b_2Pt = getHisto("DY_0b_2Pt", hBkgs, doOverflow)
    hDY_0b_3Pt = getHisto("DY_0b_3Pt", hBkgs, doOverflow)
    hDY_0b_4Pt = getHisto("DY_0b_4Pt", hBkgs, doOverflow)
    hDY_0b_5Pt = getHisto("DY_0b_5Pt", hBkgs, doOverflow)
    hDY_0b_6Pt = getHisto("DY_0b_6Pt", hBkgs, doOverflow)
    hDY_1b_1Pt = getHisto("DY_1b_1Pt", hBkgs, doOverflow)
    hDY_1b_2Pt = getHisto("DY_1b_2Pt", hBkgs, doOverflow)
    hDY_1b_3Pt = getHisto("DY_1b_3Pt", hBkgs, doOverflow)
    hDY_1b_4Pt = getHisto("DY_1b_4Pt", hBkgs, doOverflow)
    hDY_1b_5Pt = getHisto("DY_1b_5Pt", hBkgs, doOverflow)
    hDY_1b_6Pt = getHisto("DY_1b_6Pt", hBkgs, doOverflow)
    hDY_2b_1Pt = getHisto("DY_2b_1Pt", hBkgs, doOverflow)
    hDY_2b_2Pt = getHisto("DY_2b_2Pt", hBkgs, doOverflow)
    hDY_2b_3Pt = getHisto("DY_2b_3Pt", hBkgs, doOverflow)
    hDY_2b_4Pt = getHisto("DY_2b_4Pt", hBkgs, doOverflow)
    hDY_2b_5Pt = getHisto("DY_2b_5Pt", hBkgs, doOverflow)
    hDY_2b_6Pt = getHisto("DY_2b_6Pt", hBkgs, doOverflow)
    hDY_LM     = getHisto("DY_LM", hBkgs, doOverflow)
    hTT        = getHisto("TT", hBkgs, doOverflow)
    hWJets     = getHisto("W", hBkgs, doOverflow)
    hEWK       = getHisto("EWK", hBkgs, doOverflow)
    hsingleT   = getHisto("singleT", hBkgs, doOverflow)
    hTW        = getHisto("TW", hBkgs, doOverflow)
    hZH        = getHisto("ZH", hBkgs, doOverflow)
    hWH        = getHisto("WH", hBkgs, doOverflow)
    hVV        = getHisto("VV", hBkgs, doOverflow)
    httH       = getHisto("ttH", hBkgs, doOverflow)
    hTTX       = getHisto("TTX", hBkgs, doOverflow)
    hggH       = getHisto("ggH", hBkgs, doOverflow)
    hVBFH      = getHisto("VBFH", hBkgs, doOverflow)
    hVVV       = getHisto("VVV", hBkgs, doOverflow)
    
    hDYlist = [hDY_LM, hDY_0b_1Pt, hDY_0b_2Pt, hDY_0b_3Pt, hDY_0b_4Pt, hDY_0b_5Pt, hDY_0b_6Pt, hDY_1b_1Pt, hDY_1b_2Pt, hDY_1b_3Pt, hDY_1b_4Pt, hDY_1b_5Pt, hDY_1b_6Pt, hDY_2b_1Pt, hDY_2b_2Pt, hDY_2b_3Pt, hDY_2b_4Pt, hDY_2b_5Pt, hDY_2b_6Pt]
    hothersList = [hEWK, hsingleT, hTW, hVV, hTTX, hVVV, hWJets]
    hSingleHlist = [hZH, hWH, httH, hggH, hVBFH]
    hDY = makeSum("DY",hDYlist)
    hSingleH = makeSum("singleH",hSingleHlist)
    hothers = makeSum("other",hothersList)

    hBkgList = [hothers, hSingleH, hTT, hDY] # list for stack
    hBkgNameList = ["Others", "single H", "t#bar{t}", "DY + jets"] # list for legend

    if doQCD:
        hQCD = getHisto ("QCD", hBkgs,doOverflow)
        hBkgList.append(hQCD)
        hBkgNameList.append("QCD")

    hData = getHisto("data_obs", hDatas , doOverflow)

    # remove all data from blinding region before doing anything else
    if args.blindrange:
        blow = float (args.blindrange[0]) 
        bup = float (args.blindrange[1]) 
        for ibin in range (1, hData.GetNbinsX()+1):
            center = hData.GetBinCenter(ibin)
            if center > blow and center < bup:
                hData.SetBinContent(ibin, 0)

    if args.doStatSystBand:
        # these two lists are used for the calculation of the syst error band. They must:
        #       1. be in the same order
        #       2. use the same names as those in the mainCfg (for hSystBkgNameList)
        hSystBkgList = [hDY_LM, hDY_0b_1Pt, hDY_0b_2Pt, hDY_0b_3Pt, hDY_0b_4Pt, hDY_0b_5Pt, hDY_0b_6Pt, hDY_1b_1Pt, hDY_1b_2Pt, hDY_1b_3Pt, hDY_1b_4Pt, hDY_1b_5Pt, hDY_1b_6Pt, hDY_2b_1Pt, hDY_2b_2Pt, hDY_2b_3Pt, hDY_2b_4Pt, hDY_2b_5Pt, hDY_2b_6Pt, hTT, hWJets, hEWK, hsingleT, hTW, hZH, hWH, hVV, httH, hTTX, hggH, hVBFH, hVVV, hQCD]
        hSystBkgNameList = ['DY_LM', 'DY_0b_1Pt', 'DY_0b_2Pt', 'DY_0b_3Pt', 'DY_0b_4Pt', 'DY_0b_5Pt', 'DY_0b_6Pt', 'DY_1b_1Pt', 'DY_1b_2Pt', 'DY_1b_3Pt', 'DY_1b_4Pt', 'DY_1b_5Pt', 'DY_1b_6Pt', 'DY_2b_1Pt', 'DY_2b_2Pt', 'DY_2b_3Pt', 'DY_2b_4Pt', 'DY_2b_5Pt', 'DY_2b_6Pt', 'TT', 'W', 'EWK', 'singleT', 'TW', 'ZH', 'WH', 'VV', 'ttH', 'TTX', 'ggH', 'VBFH', 'VVV', 'QCD']
        hShapesNameList = ['etauFR_barrelUp', 'etauFR_endcapUp', 'PUjetIDSFUp', 'etauFR_barrelDown', 'etauFR_endcapDown', 'PUjetIDSFDown']
        # the ETau, MuTau, and TauTau channels have some different shapes -> we add them here separately
        if args.channel == 'ETau':
            if args.year == 2016: addShapes = ['trigSFeleUp', 'trigSFeleDown', 'tauid_pt20to25Up', 'tauid_pt25to30Up', 'tauid_pt30to35Up', 'tauid_pt35to40Up' 'tauid_pt40toInfUp', 'tauid_pt20to25Down', 'tauid_pt25to30Down', 'tauid_pt30to35Down', 'tauid_pt35to40Down' 'tauid_pt40toInfDown']
            else: addShapes = ['trigSFDM0Up', 'trigSFDM1Up', 'trigSFDM10Up', 'trigSFDM11Up', 'trigSFDM0Down', 'trigSFDM1Down', 'trigSFDM10Down', 'trigSFDM11Down', 'trigSFeleUp', 'trigSFeleDown', 'tauid_pt20to25Up', 'tauid_pt25to30Up', 'tauid_pt30to35Up', 'tauid_pt35to40Up', 'tauid_pt40toInfUp', 'tauid_pt20to25Down', 'tauid_pt25to30Down', 'tauid_pt30to35Down', 'tauid_pt35to40Down', 'tauid_pt40toInfDown']
            for sh in addShapes: hShapesNameList.append(sh)
        elif args.channel == 'MuTau':
            addShapes = ['trigSFDM0Up', 'trigSFDM1Up', 'trigSFDM10Up', 'trigSFDM11Up', 'trigSFDM0Down', 'trigSFDM1Down', 'trigSFDM10Down', 'trigSFDM11Down', 'trigSFmuUp', 'trigSFmuDown', 'tauid_pt20to25Up', 'tauid_pt25to30Up', 'tauid_pt30to35Up', 'tauid_pt35to40Up', 'tauid_pt40toInfUp', 'tauid_pt20to25Down', 'tauid_pt25to30Down', 'tauid_pt30to35Down', 'tauid_pt35to40Down', 'tauid_pt40toInfDown']
            for sh in addShapes: hShapesNameList.append(sh)
        else:
            addShapes = ['trigSFDM0Up', 'trigSFDM1Up', 'trigSFDM10Up', 'trigSFDM11Up', 'trigSFDM0Down', 'trigSFDM1Down', 'trigSFDM10Down', 'trigSFDM11Down', 'trigSFJetUp', 'trigSFJetDown', 'tauid_pt40toInfUp', 'tauid_pt40toInfDown']
            for sh in addShapes: hShapesNameList.append(sh)
        # we keep the JES, TES, MES, and EES separate from the others so we can also plot all the syst but them
        if not args.removeESsystBand:
            #addES = ['tauup_DM0', 'taudown_DM0', 'tauup_DM1', 'taudown_DM1', 'tauup_DM10', 'taudown_DM10', 'tauup_DM11', 'taudown_DM11', 'eleup_DM0', 'eledown_DM0', 'eleup_DM1', 'eledown_DM1', 'muup', 'mudown', 'jetup1', 'jetup2', 'jetup3', 'jetup4', 'jetup5', 'jetup6', 'jetup7', 'jetup8', 'jetup9', 'jetup10', 'jetup11', 'jetdown1', 'jetdown2', 'jetdown3', 'jetdown4', 'jetdown5', 'jetdown6', 'jetdown7', 'jetdown8', 'jetdown9', 'jetdown10', 'jetdown11']
            addES = ['tauup_DM0', 'taudown_DM0', 'tauup_DM1', 'taudown_DM1', 'tauup_DM10', 'taudown_DM10', 'tauup_DM11', 'taudown_DM11', 'eleup_DM0', 'eledown_DM0', 'eleup_DM1', 'eledown_DM1', 'muup', 'mudown', 'jetupTot', 'jetdownTot']
            for sh in addES: hShapesNameList.append(sh)

        hShapes = retrieveShapes(rootFile, bkgList, args.var, args.sel, args.reg, hShapesNameList)
        hShapesList = []
        for name in hShapes:
            hShapesList.append(getHisto(name, hShapes, doOverflow))

    #################### PERFORM BIN-NUMBER X-AXIS TRANSFORMATION #######################
    if args.binNXaxis:
        for i in range(len(hBkgList)):
            hBkgList[i] = Xaxis2binNumber(hBkgList[i])
        for key in hSigs: 
            hSigs[key] = Xaxis2binNumber(hSigs[key])
        hData = Xaxis2binNumber(hData)

    hDataNonScaled = hData.Clone("hDataNonScaled")
    gData = makeTGraphFromHist(hData, "grData")

    ######################### SET COLORS ####################
    sigColors = {}
    sigColors["GGHH_NLO_cHHH1_xs"] = kBlack
    sigColors["VBFHH_CV_1_C2V_1_C3_1_xs"] = kCyan

    col = TColor()
    bkgColors = {}
    bkgColors["DY"]    = col.GetColor("#44BA68")
    bkgColors["TT"]    = col.GetColor("#F4B642")
    bkgColors["singleH"]     = col.GetColor("#41B4DB")
    bkgColors["other"] = col.GetColor("#ED635E")

    bkgLineColors = {}
    bkgLineColors["DY"]    = col.GetColor("#389956")
    bkgLineColors["TT"]    = col.GetColor("#dea63c")
    bkgLineColors["singleH"]     = col.GetColor("#3ca4c8")
    bkgLineColors["other"] = col.GetColor("#d85a56")
    
    if doQCD:
        bkgColors["QCD"]     = col.GetColor("#F29563")
        bkgLineColors["QCD"] = col.GetColor("#DC885A")

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
    print "** INFO: removing all negative bins from bkg histos"
    makeNonNegativeHistos (hBkgList)
    if args.doStatSystBand: 
        makeNonNegativeHistos (hSystBkgList)
        makeNonNegativeHistos (hShapesList)

    for h in hBkgList: print "Integral ", h.GetName(), " : ", h.Integral(), " - ", h.Integral(-1,-1)
    for n in hDatas: print "Integral ", hDatas[n].GetName(), " : ", hDatas[n].Integral(), " - ", hDatas[n].Integral(-1,-1)
    for i, name in enumerate (sigNameList): print "Integral ", hSigs[sigList[i]].GetName(), " : ", hSigs[sigList[i]].Integral(), " - ", hSigs[sigList[i]].Integral(-1,-1)
    

    #################### PERFORM DIVISION BY BIN WIDTH AND DO NECESSARY STACKS #######################
    bkgStackNS = makeStack ("bkgStackNS", hBkgList)
    hBkgEnvelopeNS = bkgStackNS.GetStack().Last().Clone("hBkgEnvelopeNS")

    if args.binwidth:
        scaleGraphByBinWidth(gData)
        for h in hBkgList:
            h.Scale(1., "width")
        for i, name in enumerate (sigNameList):
            histo = hSigs[sigList[i]]
            histo.Scale(1., "width")

    bkgStack = makeStack ("bkgStack", hBkgList)
    bkgSum = makeSum ("bkgSum", hBkgList)


    ################## TITLE AND AESTETICS ############################
    bkgStack.Draw("HIST")

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

    # width = ((bkgStack.GetXaxis().GetXmax() - bkgStack.GetXaxis().GetXmin())/bkgStack.GetStack().Last().GetNbinsX()) --> WE WRITE ONLY 'BINWIDTH' AT THE DENOMINATOR
    ylabel = "Events"    
    if args.binwidth:
        ylabel = "Events/Bin Width"
        if args.label and "GeV" in args.label: ylabel +=" GeV"
    bkgStack.GetYaxis().SetTitle(ylabel)
    
    intBkg = bkgStack.GetStack().Last().Integral()
    bkgStack.SetTitle(plotTitle)

    # scale the signal by the specified value
    for i, scale in enumerate(sigScale):
        histo = hSigs[sigList[i]]
        histo.Scale(scale)


    ################## LEGEND ######################################
    legminx = 0.5
    legmin = 0.7
    if args.lymin: legmin = args.lymin
    if (len(hBkgNameList) +len(hSigs)>6): legminx = 0.325
    leg = TLegend (legminx, legmin, 0.85, 0.93)
    if (len(hBkgNameList) +len(hSigs)> 6): leg.SetNColumns(2)
    leg.SetFillStyle(0)
    leg.SetBorderSize(0)
    leg.SetTextFont(43)
    leg.SetTextSize(18)

    if args.dosig:
        #for i, name in enumerate (sigNameList):
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
    if args.log: ymin = 0.1

    maxs = []
    
    maxs.append(bkgStack.GetStack().Last().GetMaximum())

    if args.dodata:
        maxs.append(findMaxOfGraph(gData))
        #if not args.binwidth:
        #    maxs.append(hData.GetMaximum() + math.sqrt(hData.GetMaximum()))
        
    if args.dosig :
        for key in hSigs: maxs.append(hSigs[key].GetMaximum())

    ymax = max(maxs)

    # scale max to leave some space (~10%)
    extraspace = 0.3

    if not args.log:
        ymax += extraspace* (ymax-ymin)
    
    else:
        new = extraspace * (math.log(ymax, 10) - math.log(ymin, 10)) + math.log(ymax, 10)
        ymax = math.pow(10, new)

    bkgStack.SetMinimum(ymin)
    bkgStack.SetMaximum(ymax)

    # now we calculate the stat+syst uncertainty band
    if args.doStatSystBand:    
        # create list of files containing the systematic errors of the analysis 
        systCfgs = ['systematics_'+args.year, 'systematics_DY'+args.year, 'systematics_QCD'+args.year]
        if args.channel == 'MuTau': systCfgs.append('systematics_mutau')
        if args.channel == 'ETau': systCfgs.append('systematics_etau')
    
        # initialize the selection names as contained in the systQCD files
        if '1b1j' in args.sel:  
                sel_qcd = 'res1b'
        if '2b0j' in args.sel:  
                sel_qcd = 'res2b'
        if 'boosted' in args.sel:
                sel_qcd = 'boosted'
        if 'GGFclass' in args.sel:  
                sel_qcd = 'classGGF'
        if 'VBFclass' in args.sel:  
                sel_qcd = 'classVBF'
        if 'DYclass' in args.sel:  
                sel_qcd = 'classDY'
        if 'ttHclass' in args.sel:
                sel_qcd = 'classttH'
        if 'TTclass' in args.sel:
                sel_qcd = 'classTT'
        if "VBFloose" in args.sel:
                sel_qcd = "VBFloose" # this does not exist in the systematics_QCD.cfg file --> it is just a dummy to avoid the breaking of makeStatSystUncertaintyBand()

        grUncert = makeStatSystUncertaintyBand(bkgSum, hBkgs, hSystBkgNameList, systCfgs, args.channel, sel_qcd, hShapes, hShapesNameList) 
    else: 
        grUncert = makeStatUncertaintyBand(bkgSum)

    # interactive display
    bkgStack.Draw("HIST")
    grUncertStack = scaleStatSystUncertaintyBandForStack(grUncert, bkgSum)
    grUncertStack.SetFillColor(kGray+2)
    grUncertStack.SetFillStyle(3002)
    grUncertStack.Draw("e2")
    if args.doStatSystBand: leg.AddEntry(grUncertStack, "Stat+Syst uncertainty", 'f')
    else: leg.AddEntry(grUncertStack, "Statistical uncertainty", 'f')
    if args.dosig:
        for key in hSigs: hSigs[key].Draw("hist same")
    if args.dodata:
        removeHErrors(gData)
        removeEmptyPoints(gData)
        gData.Draw("P Z same") # Z: no small line at the end of error bar


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

    if args.year == 2016: lumi = "35.9 fb^{-1} (13 TeV)"
    elif args.year == 2017: lumi = "41.5 fb^{-1} (13 TeV)"
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
            if "baseline" in args.sel:  
                    selName = "baseline"
            if "1b1j" in args.sel:  
                    selName = "res1b"
            if "2b0j" in args.sel:  
                    selName = "res2b"
            if "boosted" in args.sel:
                    selName = "boosted"
            if "GGFclass" in args.sel:  
                    selName = "classGGF"
            if "VBFclass" in args.sel:  
                    selName = "classVBF"
            if "DYclass" in args.sel:  
                    selName = "classDY"
            if "ttHclass" in args.sel:
                    selName = "classttH"
            if "TTclass" in args.sel:
                    selName = "classTT"
            if "VBFloose" in args.sel:
                    selName = "VBF incl."
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
        bkgStack.GetXaxis().SetTitleSize(0.00);
        bkgStack.GetXaxis().SetLabelSize(0.00);

        c1.cd()
        pad2 = TPad ("pad2", "pad2", 0, 0.0, 1, 0.2496)
        pad2.SetLeftMargin(0.12);
        pad2.SetTopMargin(0.045);
        pad2.SetBottomMargin(0.4);
        pad2.SetGridy(True);
        pad2.SetFrameLineWidth(3)
        #pad2.SetGridx(True);
        pad2.Draw()
        pad2.cd()

        grRatio = makeDataOverMCRatioPlot(hDataNonScaled, hBkgEnvelopeNS, "grRatio")
        hRatio = hDataNonScaled.Clone("hRatioAxis") # for ranges only

        hRatio.GetXaxis().SetTitleFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetTitleFont(43) # so that size is in pixels
        hRatio.GetXaxis().SetLabelFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetLabelFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetNdivisions(505)

        #hRatio.GetXaxis().SetTitle(bkgStack.GetXaxis().GetName())
        hRatio.SetTitle(plotTitle)
        hRatio.GetYaxis().SetTitle ("Data/Bkg.") #("Data/MC")
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

        removeEmptyPoints (grRatio)
        
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
            hRatio.SetMinimum(lim_yLow-0.05)
            hRatio.SetMaximum(lim_yUp+0.05)
        else:
            hRatio.SetMinimum(0)
            hRatio.SetMaximum(2)

        hRatio.Draw("axis")
        
        grRatio.Draw("0P Z same") # Z : no small limes at the end of points
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

    ###################### DISPLAY ###################################
    if not args.quit:
        c1.Update()
        pad1.Update()
        if pad2: pad2.Update()
        raw_input() # to prevent script from closing


    if args.printplot:
        saveName = './LegacyPlots/Legacy' + args.year + '/' + args.channel + '/' + args.sel + "/plot_" + args.var + "_" + args.sel +"_" + args.reg
        if args.log: saveName = saveName+"_log"
        if args.binNXaxis: saveName = saveName+"_binNXaxis"
        if args.binwidth: saveName = saveName+"_binWidth"
        if args.doStatSystBand: saveName = saveName+'_StatSystBand'
        if args.removeESsystBand: saveName = saveName+'_noES'

        c1.SaveAs (saveName+".pdf")










