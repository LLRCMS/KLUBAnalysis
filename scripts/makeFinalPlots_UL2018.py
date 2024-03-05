import os
import os.path as op
import re
import argparse
import fnmatch
import re
import math
from array import array
from ctypes import c_double, c_float

import modules.ConfigReader as cfgr
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
            print(yq)
            return yq

def applyEqualBinWidth(old, roundx=2, roundy=2):
    """
    Replaces the object by changing the labels and
    ensuring all bins have the same visual width.
    Internally the bins are numbered from 0 to nBins.
    """
    darr = lambda x : np.array(x).astype(dtype=np.double)
    def xfunc(l, r):
        lstr = str(round(l, roundx)) if roundx!=0 else str(int(l))
        rstr = str(round(r, roundx)) if roundx!=0 else str(int(r))
        return '[' + lstr + ';' + rstr + '['
    def yfunc(l, r):
        lstr = str(round(l, roundy)) if roundy!=0 else str(int(l))
        rstr = str(round(r, roundy)) if roundy!=0 else str(int(r))
        return '#splitline{[' + lstr + ';}{' + rstr + '[}'
    
    if old.InheritsFrom(ROOT.TGraphAsymmErrors.Class()):
        h = ROOT.TGraphAsymmErrors(old.GetN())
        for ip in range(old.GetN()):
            h.SetPoint(ip, ip, old.GetPointY(ip))
            h.SetPointError(ip, .5, .5,
                            old.GetErrorYlow(ip), old.GetErrorYhigh(ip) )

    elif old.InheritsFrom(ROOT.TH1F.Class()): 
        name = old.GetName() + '_equal_width'
        nx = old.GetNbinsX()
        h = ROOT.TH1F(name, name, nx, 0, nx)
        for bx in range(1, old.GetNbinsX()+1):
            h.SetBinContent(bx, old.GetBinContent(bx))
            #h.SetBinError(bx, old.GetBinError(bx))

        # Erase bin labels (use TGaxis later)
        for bx in range(1, old.GetNbinsX()+1):
            h.GetXaxis().SetBinLabel(bx, '')

    elif old.InheritsFrom(ROOT.TH2D.Class()): 
        name = old.GetName() + '_equal_width'
        nx, ny = old.GetNbinsX(), old.GetNbinsY()
        h = ROOT.TH2D(name, name, nx, 0, nx, ny, 0, ny)
        for by in range(1, old.GetNbinsY()+1):
            for bx in range(1, old.GetNbinsX()+1):
                h.SetBinContent(bx, by, old.GetBinContent(bx, by))
                h.SetBinError(bx, by, old.GetBinError(bx, by))

        # Change bin labels
        for bx in range(1, old.GetNbinsX()+1):
            ledge = old.GetXaxis().GetBinLowEdge(bx)
            redge = old.GetXaxis().GetBinUpEdge(bx)
            h.GetXaxis().SetBinLabel(bx, xfunc(ledge,redge))
        for by in range(1, old.GetNbinsY()+1):
            dedge = old.GetYaxis().GetBinLowEdge(by)
            uedge = old.GetYaxis().GetBinUpEdge(by)
            h.GetYaxis().SetBinLabel(by, yfunc(dedge,uedge))

    else:
        mess = '[apply_equal_bin_width] '
        mess += 'The object should either be a TGraphasymmErrors or a TH2D'
        raise ValueError(mess)
    return h

def retrieveHistos(rootFile, namelist, var, sel, reg, flat, binning):
    res = {}
    for name in namelist:
        fullName = '_'.join((name, sel, reg, var))

        if not rootFile.GetListOfKeys().Contains(fullName):
            print('*** WARNING: histo {} not available.'.format(fullName))
            continue
        h = rootFile.Get(fullName)
    
        if not args.flat:
            res[name] = h
        else:
            hreb = h.Rebin(len(binning)-1, 'hreb', binning) 
            res[name] = hreb
                        
    return res
                        
def getHisto(prefix, inputs, doOverflow):
    assert len(inputs) != 0
    histo = None
    for idx, name in enumerate(inputs):
        if name.startswith(prefix) and name.endswith(prefix):
            histo = inputs[name].Clone(prefix)
            if doOverflow:
                histo = addOverFlow(h)
                break
    if histo is None:
        mes = 'Check the input histograms do have TH1 starting with {}'
        raise ValueError(mes.format(prefix))
    return histo

# makes an histogram by adding together all those in the input list ; inputList: names, histoList: histograms
def makeStack(stackName, histoList):
    s = ROOT.THStack (stackName, stackName)
    for h in histoList:
        s.Add(h)
    return s

def makeSum(sumName, histoList):
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
def removeEmptyPoints(graph):
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
    dummy = ROOT.TH1F('tempo', histo.GetTitle(), histo.GetNbinsX() + 1,
                      histo.GetXaxis().GetXmin(), histo.GetXaxis().GetXmax() + histo.GetBinWidth(1)) 

    for iBin in range(1,histo.GetNbinsX () + 2):
        dummy.SetBinContent (iBin, histo.GetBinContent (iBin)) 
        dummy.SetBinError (iBin, histo.GetBinError (iBin)) 
  
    if(histo.GetDefaultSumw2()):
        dummy.Sumw2() 

    name = histo.GetName () 
    histo.SetName ("trash") 
    if args.label:
        dummy.GetXaxis().SetTitle(args.label)
    else:
        dummy.GetXaxis().SetTitle(args.var)
    dummy.SetName(name)
    histo, dummy = dummy, histo
    return histo

def addOverAndUnderFlow(histo):
    histo.SetBinContent(histo.GetNbinsX(), histo.GetBinContent(histo.GetNbinsX()) + histo.GetBinContent(histo.GetNbinsX()+1)); 
    histo.SetBinContent(1, histo.GetBinContent(1) + histo.GetBinContent(0))
  
    if histo.GetBinErrorOption() != ROOT.TH1.kPoisson:
        histo.SetBinError(histo.GetNbinsX(), sqrt(pow(histo.GetBinError(histo.GetNbinsX()),2) + pow(histo.GetBinError(histo.GetNbinsX()+1),2)))
        histo.SetBinError(1, sqrt(pow(histo.GetBinError(1),2) + pow(histo.GetBinError(0),2)))

    histo.SetBinContent(0,0)
    histo.SetBinContent(histo.GetNbinsX()+1,0)
    if histo.GetBinErrorOption() != ROOT.TH1.kPoisson:
        histo.SetBinError(0,0)
        histo.SetBinError(histo.GetNbinsX()+1,0)

# NB!! need to be called BEFORE removeHErrors or cannot know bin width
def scaleGraphByBinWidth(graph):
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
def makeMCUncertaintyBand(bkgSum):
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
            fX.append(bkgSum.GetBinCenter(ibin))
            fY.append(1.0)
            feYUp.append(bkgSum.GetBinErrorUp(ibin)  / central)
            feYDown.append(bkgSum.GetBinErrorLow(ibin) / central)
            feXRight.append(bkgSum.GetBinLowEdge(ibin+1) - bkgSum.GetBinCenter(ibin))
            feXLeft.append(bkgSum.GetBinCenter(ibin) - bkgSum.GetBinLowEdge(ibin))

    afX       = array('d', fX      )
    afY       = array('d', fY      )
    afeYUp    = array('d', feYUp   )
    afeYDown  = array('d', feYDown )
    afeXRight = array('d', feXRight)
    afeXLeft  = array('d', feXLeft )
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
def makeNonNegativeHistos(hList):
    for h in hList:
        integral = h.Integral()
        for b in range (1, h.GetNbinsX()+1):
            if h.GetBinContent(b) < 0:
                h.SetBinContent (b, 0)
        integralNew = h.Integral()        
        if (integralNew != integral):
            print("** INFO: removed neg bins from histo ", h.GetName())
            
        if integralNew == 0:
            h.Scale(0)
        else:
            h.Scale(integral/integralNew) 

if __name__ == "__main__" :
    ROOT.TH1.AddDirectory(0)

    titleSize, labelSize = 24, 22
    parser = argparse.ArgumentParser(description='Command line parser of plotting options')
    
    #string opts
    parser.add_argument('--var', help='variable name', default=None)
    parser.add_argument('--sel', help='selection name', default=None)
    parser.add_argument('--name', help='selection name for plot', default=None)
    parser.add_argument('--indir', help='analysis input folder name', default='.')
    parser.add_argument('--cfg', help='configuration file')
    parser.add_argument('--outdir', help='analysis output folder name', default='.')
    parser.add_argument('--reg', help='region name', default=None)
    parser.add_argument('--title', help='plot title', default=None)
    parser.add_argument('--label', help='x label', default=None)
    parser.add_argument('--channel', choices=('ETau', 'MuTau', 'TauTau', 'MuMu'),
                        help='channel', required=True)
    parser.add_argument('--logy', help='use Y log scale',  action='store_true', default=False)
    parser.add_argument('--logx', help='use X log scale',  action='store_true', default=False)
    parser.add_argument('--nodata', help='disable plotting data', action='store_true')
    parser.add_argument('--nosig', help='disable plotting signal', action='store_true')
    parser.add_argument('--legend',  help='disable drawing legend', action='store_false')
    parser.add_argument('--binwidth', action='store_true', help='disable bin width scaling')
    parser.add_argument('--ratio', help='do ratio plot at the bottom', action='store_true', default=False)
    parser.add_argument('--saveratio', help='save ratio in ROOT file', action='store_true', default=False)
    parser.add_argument('--sbs', action='store_true', default=False,
                        help='do S/(S+B) plot at the bottom')
    parser.add_argument('--noprint', action='store_false',
                        default=True, help='no pdf output')
    parser.add_argument('--quit', action='store_true', default=False,
                        help='quit at the end of the script, no interactive window')
    parser.add_argument('--overflow', action='store_true', default=False,
                        help='add overflow bin')
    parser.add_argument('--flat', action='store_true', default=False,
                        help='rebin getting flat signal')
    parser.add_argument('--equalwidth', action='store_true', default=False,
                        help='display histograms with same visual bin width')

    # par list opt
    parser.add_argument('--blindrange', nargs=2, default=None,
                        help='start and end of blinding range')

    # float opt
    parser.add_argument('--lymin', type=float, default=None,
                        help='legend min y position in pad fraction')
    parser.add_argument('--ymin', type=float, help='min y range of plots', default=None)
    parser.add_argument('--ymax', type=float, help='max y range of plots', default=None)
    parser.add_argument('--sigscale', type=int, default=1, help='scale to apply to all signals [pb]')
    parser.add_argument('--signals', nargs='+', help='resonant signals to overlay', 
                        default=['ggFRadion280', 'ggFRadion400', 'ggFRadion550', 'ggFRadion800', 'ggFRadion1500'])
    parser.add_argument('--lumi', type=float, help='lumi in fb-1', default=None)

    args = parser.parse_args()
    print(args)
    if args.quit:
        ROOT.gROOT.SetBatch(True)
    
    ######################### CANVASES #################################
    c1 = ROOT.TCanvas('c1', 'c1', 600, 600)
    pad1, pad2 = None, None
    if (args.ratio and not args.nodata) or args.sbs:
        if args.equalwidth:
            pad1 = ROOT.TPad('pad1', 'pad1', 0, 0.3, 1, 1.0)
        else:
            pad1 = ROOT.TPad('pad1', 'pad1', 0, 0.25, 1, 1.0)
        pad1.SetFrameLineWidth(3)
        pad1.SetLeftMargin(0.12);
        pad1.SetBottomMargin(0.005);
    else:
        pad1 = ROOT.TPad('pad1', 'pad1', 0, 0.0, 1.0, 1.0)
        pad1.SetFrameLineWidth(3)
        pad1.SetLeftMargin(0.12)
        pad1.SetBottomMargin(0.12)
    pad1.SetTopMargin(0.08)
    pad1.Draw()

    pad1.cd()

    ######################### PUT USER CONFIGURATION HERE ####################
    if args.cfg is None:
        cfgName = 'mainCfg_' + args.channel + '_UL18.cfg'
    else:
        cfgName = args.cfg
    cfgName = op.join(args.indir, cfgName)
    cfg = cfgr.ConfigReader(cfgName)
    bkgList = cfg.readListOption('general::backgrounds')

    doQCD = True
    if not 'SR' in args.reg: doQCD = False
    if not 'Tau' in args.channel: doQCD = False
    
    if doQCD:
        bkgList.append('QCD')

    # color-blind friendly
    # https://indico.cern.ch/event/1372111/contributions/5768994/subcontributions/463452/attachments/2794503/4874367/20240206_CVDProjectReport_DiversityMeeting_YiChen_v3.pdf
    col = ROOT.TColor() 
    bkgColors = {'DY':    (col.GetColor("#7A21DD"), col.GetColor("#7A21DD")),
                 'TT':    (col.GetColor("#9C9CA1"), col.GetColor("#9C9CA1")),
                 'W':     (col.GetColor("#964A8B"), col.GetColor("#964A8B")),
                 'H':     (col.GetColor("#E42536"), col.GetColor("#E42536")),
                 'other': (col.GetColor("#F89C20"), col.GetColor("#F89C20"))}

    plotTitle = args.title if args.title else ""        
    dataList = ['data_obs']
    sec = 'merge_plots'

    if cfg.hasSection(sec):
        for groupname in cfg.config[sec]:
            if "data" in groupname: continue
            mergelist = cfg.readListOption(sec+'::'+groupname)
            for x in mergelist:
                try:
                    bkgList.remove(x)
                except ValueError:
                    print('The culprit is {}, in group {}, in config {}.'.format(x,groupname,cfgName))
                    print('Backgrounds in the list: {}.'.format(bkgList))
                    print('Merged backgrounds in the list: {}.'.format(mergelist))
                    raise
            bkgList.append(groupname)
    else:
        raise ValueError('Section [{}] missing from {}.'.format(sec, cfgName))


    outplot = findInFolder(args.indir+'/', 'combined_outPlots.root')

    rootFile = ROOT.TFile.Open(op.join(args.indir, outplot))

    binning = None
    if args.flat:
        binning = flatBinning(rootFile, args.signals, args.var, args.sel, args.reg)

    opts = args.var, args.sel, args.reg, args.flat, binning

    eqbin = applyEqualBinWidth if args.equalwidth else lambda x: x
    if not args.nosig:
        hSigs = {k:eqbin(x) for k,x in retrieveHistos(rootFile, args.signals, *opts).items()}
    hBkgs = {k:eqbin(x) for k,x in retrieveHistos(rootFile, bkgList, *opts).items()}
    hDatas = {k:eqbin(x) for k,x in retrieveHistos(rootFile, dataList, *opts).items()}

    hopt = hBkgs, args.overflow
    hDY      = getHisto('DY',    *hopt)
    hTT      = getHisto('TT',    *hopt)
    hWJets   = getHisto('W',     *hopt)
    hHiggs   = getHisto('H',     *hopt)
    hothers  = getHisto('other', *hopt)

    # hZH      = getHisto('ZH', *hopt)
    # hWH      = getHisto('WH', *hopt)
    # httH     = getHisto('ttH', *hopt)
    # hggH     = getHisto('ggH', *hopt)
    # hsingleH = makeSum('singleH', [hZH, hWH, httH, hggH])
    
    hBkgList = [(hothers,  'Others'),
                (hHiggs,   'H+HH'),
                (hWJets,   'W+jets'),
                (hTT,      't#bar{t}'),
                (hDY,      'DY')] 

    if doQCD:
        col2 = ROOT.TColor()
        hQCD = getHisto('QCD', hBkgs, args.overflow)
        hQCD.SetName('QCD')
        hBkgList.append((hQCD, 'QCD'))
        bkgColors['QCD'] = (col2.GetColor('#5790FC'),col2.GetColor('#5790FC'))

    hData = getHisto('data_obs', hDatas , args.overflow).Clone('hData')

    # remove all data from blinding region before creating tgraph etc...
    if args.blindrange:
        blow = float (args.blindrange[0]) 
        bup = float (args.blindrange[1]) 
        for ibin in range (1, hData.GetNbinsX()+1):
            center = hData.GetBinCenter(ibin)
            if center > blow and center < bup:
                hData.SetBinContent(ibin, 0)

    hDataNonScaled = hData.Clone('hDataNonScaled')
    gData = makeTGraphFromHist(hData, 'grData')

    # apply sig color if available
    if not args.nosig:
        col = {0: ROOT.kBlack,
               1: ROOT.kGray+2,
               2: ROOT.kRed+3}
        dash = 1
        
        for isig,hsig in enumerate(sorted(hSigs.values())):
            if args.overflow:
                hSigs[key] = addOverFlow(hSigs[key])
            hsig.SetLineColor(col[isig%3])
            if isig%3==0:
                dash = int(not bool(dash))
            hsig.SetLineStyle(1 if dash==0 else 2)
            hsig.SetLineWidth(4)
            
    # apply bkg color if available
    for h in hBkgList:
        histoname = h[0].GetName()
        for key,value in bkgColors.items():
            if key in histoname:    
                fill_color = int(value[0])
                h[0].SetFillColor(fill_color)
                h[0].SetFillStyle(1001)
                line_color = int(value[1])
                h[0].SetLineColor(line_color)
                h[0].SetLineWidth(1)
            
    #################### REMOVE NEGATIVE BINS #######################
    print("** INFO: removing all negative bins from bkg histos")
    makeNonNegativeHistos([x[0] for x in hBkgList])
    
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
                yields_file.write(print_integral(h[0]))
            for h in hDatas.values:
                yields_file.write(print_integral(h))
            if not args.nosig:
                yields_file.write(print_integral)
    else:
        yields_name = op.join(args.outdir, interm_path(args.sel), 'yields.txt')
        with open(yields_name, 'a+') as yields_file:
            yields_file.write('=== ' + op.join(interm_path(args.sel), args.var) + ' ===\n')
            for h in hBkgList:
                yields_file.write(print_integral(h[0]))
            for h in hDatas.values():
                yields_file.write(print_integral(h))
            if not args.nosig:
                for sig in args.signals:
                    yields_file.write(print_integral(hSigs[sig]))

    for h in hBkgList:
        print_integral(h[0], True)
    for h in hDatas.values():
        print_integral(h, True)
    if not args.nosig:
        for sig in args.signals:
            print_integral(hSigs[sig], True)

            
    #################### PERFORM DIVISION BY BIN WIDTH #######################
    #clones non scaled (else problems with graph ratio because I pass data evt hist)
    bkgStackNS = makeStack('bkgStackNS', [x[0] for x in hBkgList])
    hBkgEnvelopeNS = bkgStackNS.GetStack().Last().Clone('hBkgEnvelopeNS')

    if args.binwidth:
        scaleGraphByBinWidth(gData)
        for h in hBkgList:
            h[0].Scale(1., 'width')
        if not args.nosig:
            for sig in args.signals:
                histo = hSigs[sig]
                histo.Scale(1., 'width')

    #################### DO STACK AND PLOT #######################
    bkgStack = makeStack('bkgStack', [x[0] for x in hBkgList])
    bkgSum = makeSum('bkgSum', [x[0] for x in hBkgList])

    if args.logx:
        pad1.SetLogx()
    if args.logy:
        pad1.SetLogy()

    ################## TITLE AND AESTHETICS ############################
    bkgStack.Draw('HIST')

    bkgStack.GetXaxis().SetTitleFont(43)
    bkgStack.GetYaxis().SetTitleFont(43)
    bkgStack.GetXaxis().SetLabelFont(43)
    bkgStack.GetYaxis().SetLabelFont(43)

    bkgStack.GetXaxis().SetTitleOffset(1.0)
    bkgStack.GetYaxis().SetTitleOffset(1.39)

    bkgStack.GetXaxis().SetTitleSize(titleSize)
    bkgStack.GetYaxis().SetTitleSize(titleSize)

    bkgStack.GetXaxis().SetLabelSize(labelSize)
    bkgStack.GetYaxis().SetLabelSize(labelSize)

    if args.label: bkgStack.GetXaxis().SetTitle(args.label)
    else: bkgStack.GetXaxis().SetTitle(args.var)

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

    if not args.nosig:
        for sig in args.signals:
            histo = hSigs[sig]
            histo.Scale(args.sigscale)

    ################## LEGEND ######################################
    legmin = 0.6 if not args.lymin else args.lymin
    legsize, legfont = 20, 43
    legBkg = ROOT.TLegend(0.73, legmin, 0.93, 0.90)
    legBkg.SetFillStyle(0)
    legBkg.SetBorderSize(0)
    legBkg.SetTextFont(legfont)
    legBkg.SetTextSize(legsize)

    if not args.nosig:
        legSig = ROOT.TLegend(0.55, legmin, 0.73, 0.90)
    elif args.nosig and not args.nodata:
        legSig = ROOT.TLegend(0.55, 0.82, 0.73, 0.90)

    if not args.nosig or not args.nodata:
        legSig.SetFillStyle(0)
        legSig.SetBorderSize(0)
        legSig.SetTextFont(legfont)
        legSig.SetTextSize(legsize)
    
    # add element in same order as stack --> top-bottom
    if not args.nosig:
        for sig in args.signals:
            histo = hSigs[sig]
            mass = re.findall('[0-9]+', sig)[0]
            legSig.AddEntry(histo, mass + 'GeV', 'l')

    for h in reversed(hBkgList):
        legBkg.AddEntry(h[0], h[1], 'f')

    if not args.nodata:
        legSig.AddEntry(gData, 'Data', 'pe')

    ################## Y RANGE SETTINGS ############################
    ymin = 1. if args.logy else 0.
    maxs = []
    maxs.append(bkgStack.GetStack().Last().GetMaximum())

    if not args.nodata:
        maxs.append(findMaxOfGraph(gData))
        
    if not args.nosig:
        for key in hSigs: 
            maxs.append(hSigs[key].GetMaximum())

    ymax = max(maxs)

    # scale max to leave some space (~10%)
    extraspace = 0.5

    if not args.logy:
        ymax += extraspace* (ymax-ymin)
    
    else:
        new = extraspace * (math.log(ymax, 10) - math.log(ymin, 10)) + math.log(ymax, 10)
        ymax = math.pow(10, new)

    ## override form args
    if args.ymin: ymin = args.ymin
    if args.ymax: ymax = args.ymax

    bkgStack.SetMinimum(ymin)
    bkgStack.SetMaximum(ymax)

    # interactive display
    bkgStack.Draw('HIST')
    bkgSum.SetFillColor(ROOT.kGray+2);
    bkgSum.SetFillStyle(3002);
    bkgSum.Draw('e2 same')
    xaxis = bkgSum.GetXaxis()
    for bx in range(1, bkgSum.GetNbinsX()+1):
        xaxis.ChangeLabel(bx,-1,-1,-1,-1,-1,"X");

    if not args.nosig:
        for key in hSigs: 
            hSigs[key].Draw('hist same')
            xaxis = hSigs[key].GetXaxis()
            for bx in range(1, hSigs[key].GetNbinsX()+1):
                xaxis.ChangeLabel(bx,-1,-1,-1,-1,-1,"X");

    if not args.nodata:
        removeHErrors(gData)
        removeEmptyPoints(gData)
        gData.Draw('P Z same') # Z: no small line at the end of error bar

    if args.equalwidth and not args.ratio:
        ahisto = retrieveHistos(rootFile, bkgList, *opts).items()[0][1]
        ndiv = ahisto.GetNbinsX()
        finalaxis = ROOT.TGaxis(0., 0., ndiv, 0., 0, ndiv, ndiv, '+')
        for ip in range(1,ndiv+1):
            size = 0.037
            if ip==0:
                ledge = ahisto.GetXaxis().GetBinLowEdge(ip)
                finalaxis.ChangeLabel(ip,-1,size,-1,-1,-1,str(round(ledge, 4)));
            redge = ahisto.GetXaxis().GetBinUpEdge(ip)
            finalaxis.ChangeLabel(ip+1,-1,size,-1,-1,-1,str(round(redge, 4)));
        finalaxis.SetLabelFont(42);
        finalaxis.SetLabelSize(10);
        finalaxis.Draw('same')

    ###################### OTHER TEXT ON PLOT #########################
    cmsTextFont   = 61                 # font of the "CMS" label
    cmsTextSize   = 0.05               # font size of the "CMS" label
    extraTextFont = 52                 # for the "preliminary"
    extraTextSize = 0.76 * cmsTextSize # for the "preliminary"
    
    t = ROOT.gPad.GetTopMargin()
    b = ROOT.gPad.GetBottomMargin()
    l = ROOT.gPad.GetLeftMargin()
    r = ROOT.gPad.GetRightMargin()       

    CMSbox       = ROOT.TLatex(l, 1 - t + 0.01, 'CMS')       
    extraTextBox = ROOT.TLatex(l + 0.08, 1 - t + 0.01, 'Preliminary')
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
    #    histoWidth = histo.GetXaxis().GetBinWidth(1)*histo.GetXaxis().GetNbins()
    #    histoHeight = histo.GetMaximum()-histo.GetMinimum()
 

    lumi = '%.1f fb^{-1} (13 TeV)' % args.lumi
    lumibox = ROOT.TLatex(1-r, 1 - t + 0.01 , lumi)
    lumibox.SetNDC()
    lumibox.SetTextAlign(31)
    lumibox.SetTextSize(extraTextSize)
    lumibox.SetTextFont(42)
    lumibox.SetTextColor(ROOT.kBlack)

    if args.channel:
        if args.channel == 'MuTau':
            chName = 'bb #mu#tau_{h}'
        elif args.channel == 'ETau':
            chName = 'bb e#tau_{h}'
        elif args.channel == 'TauTau':
            chName = 'bb #tau_{h}#tau_{h}'
        elif args.channel == 'MuMu':
            chName = 'bb #mu#mu'

        if chName:
            chBox = ROOT.TLatex(l + 0.04 , 1 - t - 0.01 - 0.04, chName)
            chBox.SetNDC()
            chBox.SetTextSize(cmsTextSize+20)
            chBox.SetTextFont(43)
            chBox.SetTextColor(ROOT.kBlack)
            chBox.SetTextAlign(13)

    if not args.nosig:
        sigxsecName = '#sigma^{{ggF}}_{{X#rightarrowHH}} #it{{B}}_{{HH#rightarrowbb#tau#tau}} = {}pb'.format(args.sigscale)
        sigxsecBox = ROOT.TLatex(l+0.04 , 1-t-0.02-0.1, sigxsecName)
        sigxsecBox.SetNDC()
        sigxsecBox.SetTextSize(cmsTextSize+20)
        sigxsecBox.SetTextFont(43)
        sigxsecBox.SetTextColor(ROOT.kBlack)
        sigxsecBox.SetTextAlign(13)
            
    CMSbox.Draw()
    extraTextBox.Draw()
    lumibox.Draw()
    if not args.nosig:
        sigxsecBox.Draw()
    
    if args.legend:
        if not args.nosig or not args.nodata:
            legSig.Draw()
        legBkg.Draw()
    if chBox:
        chBox.Draw()

    if not args.name:
        if args.sel == 's1b1jresolvedMcut':
            selName = 'resolved 1b'
        elif args.sel == 's2b0jresolvedMcut':
            selName = 'resolved 2b'
        elif args.sel == 'sboostedMcut':
            selName = 'boosted'
        else:
            selName = args.sel
    else:
        selName = args.name

    selBox = ROOT.TLatex(l+0.04, 1-t-0.01, selName)
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
        for nameSig in args.signals:
            histSig = hSigs[nameSig].Clone()
            hSigsNonScaled.append(histSig)

        grSBSs = []
        for sigNS in hSigsNonScaled:
            #grSBS = makeSBSPlot (sigNS, hBkgEnvelopeNS, "S/(S+B)", sigNS.GetLineColor())
            grSBS = makeSBSPlot (sigNS, hBkgEnvelopeNS, "Z_{A}", sigNS.GetLineColor())
            grSBSs.append(grSBS)

        hRatio = hDataNonScaled.Clone("hRatioAxis") # for ranges only
        hRatio.GetXaxis().SetTitleFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetTitleFont(43) # so that size is in pixels
        hRatio.GetXaxis().SetLabelFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetLabelFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetNdivisions(505)

        #hRatio.GetXaxis().SetTitle(bkgStack.GetXaxis().GetName())
        hRatio.SetTitle(plotTitle)
        hRatio.GetYaxis().SetTitle ("Z_{A}") #("S/#sqrt{S+B}") #("Data/MC")
        if args.label: 
            hRatio.GetXaxis().SetTitle(args.label)
        else: 
            hRatio.GetXaxis().SetTitle(args.var)
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

        removeEmptyPoints(grSBS)

        hRatio.Draw("axis")
        for gr in grSBSs:
            gr.Draw("L same") # Z : no small limes at the end of points

        pad2.RedrawAxis();
        pad2.RedrawAxis("g"); #otherwise no grid..

    ###################### RATIO PLOT #################################
    if args.ratio and not args.nodata:
        bkgStack.GetXaxis().SetTitleSize(0.00);
        bkgStack.GetXaxis().SetLabelSize(0.00);
        
    if args.ratio and not args.nodata:
        c1.cd()
        if args.equalwidth:
            pad2 = ROOT.TPad('pad2', 'pad2', 0, 0.0, 1, 0.2996)
        else:
            pad2 = ROOT.TPad('pad2', 'pad2', 0, 0.0, 1, 0.2496)
        pad2.SetLeftMargin(0.12);
        pad2.SetTopMargin(0.02);
        pad2.SetBottomMargin(0.5);
        pad2.SetGridy(True);
        pad2.SetFrameLineWidth(3)
        if args.logx:
            pad2.SetLogx()
        pad2.Draw()
        pad2.cd()

        grRatio = makeDataOverMCRatioPlot (hDataNonScaled, hBkgEnvelopeNS, 'grRatio')
        hRatio = hDataNonScaled.Clone('hRatioAxis') # for ranges only
        grUncert = makeMCUncertaintyBand(bkgSum) # uncertainty band from MC, always centered at 1.0

        hRatio.GetXaxis().SetTitleFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetTitleFont(43) # so that size is in pixels
        hRatio.GetXaxis().SetLabelFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetLabelFont(43) # so that size is in pixels
        hRatio.GetYaxis().SetNdivisions(505)

        #hRatio.GetXaxis().SetTitle(bkgStack.GetXaxis().GetName())
        hRatio.SetTitle(plotTitle)
        hRatio.GetYaxis().SetTitle ("Data/Bkg.") #("Data/MC")
        if args.label: hRatio.GetXaxis().SetTitle (args.label)
        else: hRatio.GetXaxis().SetTitle(args.var)
        if args.equalwidth:
            hRatio.GetXaxis().SetTitleOffset(5.2)
        else:
            hRatio.GetXaxis().SetTitleOffset(3.9)
        hRatio.GetYaxis().SetTitleOffset(1.2)

        hRatio.GetXaxis().SetTitleSize(titleSize);
        hRatio.GetXaxis().SetLabelSize(labelSize);
        hRatio.GetYaxis().SetTitleSize(titleSize);
        hRatio.GetYaxis().SetLabelSize(labelSize);

        hRatio.GetXaxis().SetTickSize(0.10)
        hRatio.GetYaxis().SetTickSize(0.05)

        hRatio.SetStats(0)
        ratmin = 0.61
        hRatio.SetMinimum(ratmin) #default value
        hRatio.SetMaximum(1.39) #default value

        removeEmptyPoints(grRatio)
        hRatio.Draw("axis")

        grRatio.Draw("P Z same") # Z : no small limes at the end of points

        if args.equalwidth and args.ratio:
            ahisto = retrieveHistos(rootFile, bkgList, *opts).items()[0][1]
            ndiv = ahisto.GetNbinsX()
            finalaxis2 = ROOT.TGaxis(0., ratmin-0.11, ndiv, ratmin-0.11, 0., ndiv, ndiv, 'BS')
            step = 1 if 'DNN' in args.var or args.equalwidth else 6
            for ip in range(0,ndiv+1):
                size = 0.1 if ip%step==0 else 0.
                redge = ahisto.GetXaxis().GetBinUpEdge(ip)
                valstr = str(int(redge)) if 'mass' in args.var else str(round(redge, 4)) 
                finalaxis2.ChangeLabel(ip+1,90,size,22,1,-1,valstr);
            finalaxis2.SetTickSize(0.);
            finalaxis2.SetLabelOffset(.05)
            finalaxis2.Draw("same")

        xmin = hRatio.GetXaxis().GetXmin()
        xmax = hRatio.GetXaxis().GetXmax()
        l1 = ROOT.TLine(xmin, 1, xmax, 1)
        l1.SetLineColor(ROOT.kRed)
        l1.SetLineStyle(4)
        l1.SetLineWidth(1)
        l1.Draw("same")

        grUncert.SetFillColor(ROOT.kGray+2)
        grUncert.SetFillStyle(3002)
        grUncert.Draw("e2")

        pad2.RedrawAxis();
        pad2.RedrawAxis("g"); #otherwise no grid..

        if args.saveratio:
            ratioDir = op.join(args.outdir, interm_path(args.sel))
            ratioName = op.join(ratioDir, 'ratios.root')
            ratioFile = ROOT.TFile.Open(ratioName, 'UPDATE')
            grRatio.SetName('_'.join((args.var, args.sel, args.reg, args.channel)))
            grRatio.Write()
            ratioFile.Close()
            print('Ratio file saved: {}.'.format(ratioName))
        
    ###################### DISPLAY ###################################
    if not args.quit:
        # pad1.Update() # necessary to show plot
        # pad2.Update() # necessary to show plot
        c1.Update()
        pad1.Update()
        if pad2: pad2.Update()
        raw_input() # to prevent script from closing

    if args.noprint:
        tagch = ''
        if args.channel:
            tagch = args.channel
        if args.binwidth:
            tagch += '_binWidth'
        if args.equalwidth:
            tagch += '_equalWidth'
        if 'class' in args.sel:
            saveBase = 'plot_' + '_'.join((args.var, args.sel, args.reg, tagch))
            saveName = op.join(args.outdir, interm_path('scores_'), saveBase)
        else:
            saveBase = 'plot_' + '_'.join((args.var, args.sel, args.reg, tagch))
            saveName = op.join(args.outdir, interm_path(args.sel), saveBase)
        
        if args.logx:
                saveName += '_logx' 
        if args.logy:
                saveName += '_logy'

        for ext in ('pdf', 'png'):
            c1.SaveAs(saveName + '.' + ext)
