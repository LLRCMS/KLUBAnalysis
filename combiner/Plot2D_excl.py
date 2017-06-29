import ROOT
import collections
from array import array

def formatIsoLabel(isolab):
    # isolab.SetNDC()
    isolab.SetTextAlign(31)
    isolab.SetTextSize(0.02)
    isolab.SetTextFont(42)
    isolab.SetTextColor(ROOT.kGray)


def makeIsoLinesLabels():
    """ unfortunately, make this by hand """
    # isolines = [0.1, 2., 10., 20., 50., 100., 200., 500.]

    isolabels = {}


    # lab = ROOT.TLatex  (0.49, 0.48, "#it{0.1 fb}")
    # lab = ROOT.TLatex  (-2.7, -0.12, "#it{0.1 fb}")
    # formatIsoLabel(lab)
    # lab.SetTextAngle(-20)
    # isolabels[0.1] = lab

    # lab = ROOT.TLatex  (-5, -0.32, "#it{2 fb}")
    # formatIsoLabel(lab)
    # lab.SetTextAngle(-27)
    # isolabels[2.0] = lab

    # lab = ROOT.TLatex  (-7, -0.55, "#it{10 fb}")
    # formatIsoLabel(lab)
    # lab.SetTextAngle(-33)
    # isolabels[10.0] = lab

    # lab = ROOT.TLatex  (-9, -0.65, "#it{20 fb}")
    # formatIsoLabel(lab)
    # lab.SetTextAngle(-35)
    # isolabels[20.0] = lab

    # lab = ROOT.TLatex  (-10, -1.0, "#it{50 fb}")
    # formatIsoLabel(lab)
    # lab.SetTextAngle(-20)
    # isolabels[50.0] = lab

    # lab = ROOT.TLatex  (-11, -1.2, "#it{100 fb}")
    # formatIsoLabel(lab)
    # lab.SetTextAngle(-20)
    # isolabels[100.0] = lab

    # lab = ROOT.TLatex  (-15, -1.5, "#it{200 fb}")
    # formatIsoLabel(lab)
    # lab.SetTextAngle(-20)
    # isolabels[200.0] = lab

    # lab = ROOT.TLatex  (-17, -1.7, "#it{500 fb}")
    # formatIsoLabel(lab)
    # lab.SetTextAngle(-20)
    # isolabels[500.0] = lab


    lab = ROOT.TLatex  (-1.7,-0.20, "#it{0.1 fb}")
    formatIsoLabel(lab)
    lab.SetTextAngle(-30)
    isolabels[0.1] = lab

    lab = ROOT.TLatex  (-4.844247,-0.401035, "#it{2 fb}")
    formatIsoLabel(lab)
    lab.SetTextAngle(-40)
    isolabels[2.0] = lab

    lab = ROOT.TLatex  (-6.1,-0.5, "#it{5 fb}")
    formatIsoLabel(lab)
    lab.SetTextAngle(-40)
    isolabels[5.0] = lab


    lab = ROOT.TLatex  (-7.098491,-0.6205522, "#it{10 fb}")
    formatIsoLabel(lab)
    lab.SetTextAngle(-40)
    isolabels[10.0] = lab

    lab = ROOT.TLatex  (-8.425614,-0.7453987, "#it{20 fb}")
    formatIsoLabel(lab)
    lab.SetTextAngle(-43)
    isolabels[20.0] = lab

    lab = ROOT.TLatex  (-10.33121,-0.9678474, "#it{50 fb}")
    formatIsoLabel(lab)
    lab.SetTextAngle(-45)
    isolabels[50.0] = lab

    lab = ROOT.TLatex  (-12.32597,-1.178177, "#it{100 fb}")
    formatIsoLabel(lab)
    lab.SetTextAngle(-50)
    isolabels[100.0] = lab

    lab = ROOT.TLatex  (-14.74242,-1.458705, "#it{200 fb}")
    formatIsoLabel(lab)
    lab.SetTextAngle(-20)
    # isolabels[200.0] = lab

    lab = ROOT.TLatex  (-18.13192,-1.834199, "#it{500 fb}")
    formatIsoLabel(lab)
    lab.SetTextAngle(-20)
    # isolabels[500.0] = lab

    newlabs = {}
    for lab in isolabels:
        tl = isolabels[lab]
        x = tl.GetX()
        y = tl.GetY()
        labnew = ROOT.TLatex(isolabels[lab])
        labnew.SetX(-1.*x)
        labnew.SetY(-1.*y)
        labnew.SetTextAlign(13)
        newlabs[1000.+lab] = labnew
    for lab in newlabs:
        isolabels[lab] = newlabs[lab]

# -2.7,-0.12,"#it{0.1 fb}");
# -5.344247,-0.3501035,"#it{2 fb}");
# -7.398491,-0.5825522,"#it{10 fb}");
# -8.425614,-0.7553987,"#it{20 fb}");
# -10.53121,-0.9878474,"#it{50 fb}");
# -12.22597,-1.238177,"#it{100 fb}");
# -14.74242,-1.458705,"#it{200 fb}");
# -18.13192,-1.834199,"#it{500 fb}");

    return isolabels
    ##

def redrawBorder():
   # this little macro redraws the axis tick marks and the pad border lines.
   ROOT.gPad.Update();
   ROOT.gPad.RedrawAxis();
   l = ROOT.TLine()
   l.SetLineWidth(3)
   l.DrawLine(ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymax(), ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymax());
   l.DrawLine(ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymin(), ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymax());
   l.DrawLine(ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymin(), ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymax());
   l.DrawLine(ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymin(), ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymin());
def getCrossSectionForParameters(l, yt, c2, cg, c2g, BR=1.0):
    """
    Get the theoretical cross-section for a given set of parameters in fb
    """

    params = (l, yt, c2, cg, c2g)

    A = [2.09078, 10.1517, 0.282307, 0.101205, 1.33191, -8.51168, -1.37309, 2.82636, 1.45767, -4.91761, -0.675197, 1.86189, 0.321422, -0.836276, -0.568156]
    
    # From https://github.com/cms-hh/Plotting/blob/nonResonant/nonResonant/5Dfunction.py#L38
    def f(kl, kt, c2, cg, c2g):
        return A[0]*kt**4 + A[1]*c2**2 + (A[2]*kt**2 + A[3]*cg**2)*kl**2 + A[4]*c2g**2 + ( A[5]*c2 + A[6]*kt*kl )*kt**2 + (A[7]*kt*kl + A[8]*cg*kl )*c2 + A[9]*c2*c2g + (A[10]*cg*kl + A[11]*c2g)*kt**2+ (A[12]*kl*cg + A[13]*c2g )*kt*kl + A[14]*cg*c2g*kl

    # From https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHXSWGHH#Current_recommendations_for_di_H
    hh_sm_xs = 33.49

    # Uncertainty
    # hh_sm_xs_up = hh_sm_xs * (1 + math.sqrt(4.3**2 + 5**2 + 2.3**2 + 2.1**2) / 100)
    # hh_sm_xs_down = hh_sm_xs * (1 - math.sqrt(6**2 + 5**2 + 2.3**2 + 2.1**2) / 100)

    res = f(*params)
    # return res * hh_sm_xs, res * hh_sm_xs_down, res * hh_sm_xs_up
    return res * hh_sm_xs * BR

def functionGF_klkt(x):
    return getCrossSectionForParameters(x[0], x[1], 0., 0., 0., 0.073)


def getExtraTexts():
    """ return a list of extra texts to be printed on the plot """
    extratexts = {}

    cmstextfont   = 61  # font of the "CMS" label
    cmstextsize   = 0.05  # font size of the "CMS" label
    extratextfont = 52     # for the "preliminary"
    extratextsize = 0.76 * cmstextsize # for the "preliminary"

    textfont      = 43
    lumitextfont  = 42

    xpos  = 0.227
    ypos  = 0.949


    ## luminosity
    lumibox = ROOT.TLatex  (0.9, ypos, "%.1f fb^{-1} (13 TeV)" % 35.9)       
    lumibox.SetNDC()
    lumibox.SetTextAlign(31)
    lumibox.SetTextSize(extratextsize)
    lumibox.SetTextFont(lumitextfont)
    lumibox.SetTextColor(ROOT.kBlack)
    extratexts['lumi'] = lumibox

    ## CMS
    CMSbox = ROOT.TLatex  (xpos, ypos , "CMS")       
    CMSbox.SetNDC()
    CMSbox.SetTextSize(cmstextsize)
    CMSbox.SetTextFont(cmstextfont)
    CMSbox.SetTextColor(ROOT.kBlack)
    CMSbox.SetTextAlign(31)
    extratexts['cms'] = CMSbox
    
    ## preliminary
    # prelimBox = ROOT.TLatex  (xpos, ypos - 0.05 , "preliminary")
    prelimBox = ROOT.TLatex  (xpos + 0.18, ypos, "preliminary")
    prelimBox.SetNDC()
    prelimBox.SetTextSize(extratextsize)
    prelimBox.SetTextFont(extratextfont)
    prelimBox.SetTextColor(ROOT.kBlack)
    prelimBox.SetTextAlign(31)
    extratexts['preliminary'] = prelimBox
    
    # COUPLINGS
    # coupl = ROOT.TLatex  (0.81, 0.015, "c_{2} = c_{g} = c_{2g} = 0")
    coupl = ROOT.TLatex  (0.845, 0.015, "c_{2} = c_{g} = c_{2g} = 0")
    coupl.SetNDC()
    # coupl.SetTextSize(0.7*extratextsize)
    coupl.SetTextSize(0.65*extratextsize)
    coupl.SetTextFont(42)
    coupl.SetTextColor(ROOT.kBlack)
    coupl.SetTextAlign(31)
    extratexts['couplings'] = coupl

    ### xs
    xs = ROOT.TLatex  (0.915, 0.13, "#sigma(gg#rightarrowHH) #times #bf{#it{#Beta}}(HH#rightarrowbb#tau#tau)")
    formatIsoLabel(xs)
    xs.SetNDC()
    xs.SetTextAngle(-90)
    extratexts['xs'] = xs

    return extratexts

def readTranslation(fIn):
    f = open(fIn)
    transl = collections.OrderedDict()
    for line in f:
        line = line.strip()
        if not line: continue
        tokens = line.split()
        if len(tokens) != 3:
            print "** cannot interpret transl line" , line
            continue

        idx = int(tokens[0])
        kl = float(tokens[1])
        kt = float(tokens[2])
        transl[idx] = (kl, kt)
    return transl

def readExclusionFile(fIn):
    f = open(fIn)
    exclMap = collections.OrderedDict()
    for line in f:
        line = line.strip()
        if not line: continue
        tokens = line.split()
        ipt = int(tokens[0])
        kl = float(tokens[1])
        kt = float(tokens[2])
        exp = int(tokens[3]) # these, and the following, are 
        obs = int(tokens[4])
        exp_p1s = int(tokens[5])
        exp_m1s = int(tokens[6])
        exp_p2s = int(tokens[7])
        exp_m2s = int(tokens[8])
        # exclMap[(kl, kt)] = (obs, exp, exp_p1s, exp_m1s, exp_p2s, exp_m2s)
        exclMap[ipt] = (obs, exp, exp_p1s, exp_m1s, exp_p2s, exp_m2s)
    return exclMap

def prepareGridHisto(transl, exclMap, name, idx, xmin=-20, xmax=20, ymin=0.4, ymax=2.0, stepx=0.5, stepy=0.05, hcut=None, below=True):
    nxbins = (xmax-xmin)/stepx+1 # +1 because upper boundary is included in generation
    nybins = (ymax-ymin)/stepy+1 # +1 because upper boundary is included in generation
    print 'nxbins:', nxbins, 'nybins:', nybins, 'totbins:', nxbins*nybins
    hgrid = ROOT.TH2F('h'+name, 'h'+name, int(nxbins), xmin-stepx/2., xmax+stepx/2., int(nybins), ymin-stepy/2., ymax+stepy/2.)
    
    for ipt in transl:
        if exclMap[ipt][idx] == 1:
            kl = transl[ipt][0]
            kt = transl[ipt][1]
            if hcut:
                if below and kl > hcut: continue
                elif not below and kl < hcut: continue

            ibinx = hgrid.GetXaxis().FindBin(kl)
            ibiny = hgrid.GetYaxis().FindBin(kt)
            # print kl, hgrid.GetXaxis().GetBinCenter(ibinx), '---', kt, hgrid.GetYaxis().GetBinCenter(ibiny)
            hgrid.SetBinContent(ibinx, ibiny, 1)

    return hgrid

def makeStandardGraph(transl, exclMap, name, idx):
    gr = ROOT.TGraph()
    gr.SetName('stdgr'+name)
    for ipt in transl:
        if exclMap[ipt][idx] == 1:
            kl = transl[ipt][0]
            kt = transl[ipt][1]
            gr.SetPoint(gr.GetN(), kl, kt)
    gr.SetMarkerStyle(8)
    gr.SetMarkerSize(0.6)
    return gr

def getGr(hist):
    gr = ROOT.TGraph2D(hist)
    gr.SetName("gr"+hist.GetName())
    gr.SetNpx(500)
    gr.SetNpy(500)
    # gr.SetNContours(13)
    return gr

def prepareAllGridHisto(transl, exclMap, name, idx, xmin=-20, xmax=20, ymin=0.4, ymax=2.0, stepx=0.5, stepy=0.05):
    h_L = prepareGridHisto(transl, exclMap, name+'_L', idx, xmin, xmax, ymin, ymax, stepx, stepy, hcut=8.0, below=True)
    h_R = prepareGridHisto(transl, exclMap, name+'_R', idx, xmin, xmax, ymin, ymax, stepx, stepy, hcut=8.0, below=False)
    return [h_L, h_R]

def combineIntoBand(m1s, p1s):
    hSum = m1s.Clone(m1s.GetName()+'band')
    hSum.Add(p1s, 1.0)

    for ibinx in range(1, hSum.GetNbinsX()+1):
        for ibiny in range(1, hSum.GetNbinsY()+1):
            if abs(hSum.GetBinContent(ibinx, ibiny)-0.0) < 0.1 or abs(hSum.GetBinContent(ibinx, ibiny)-2.0)<0.1:
                hSum.SetBinContent(ibinx, ibiny, 0)
            elif abs(hSum.GetBinContent(ibinx, ibiny)-1.0)<0.1:
                hSum.SetBinContent(ibinx, ibiny, 1)
                # print ibinx, ibiny, '-->', hSum.GetXaxis().GetBinCenter(ibinx), hSum.GetYaxis().GetBinCenter(ibiny) , 'this is', hSum.GetBinContent(ibinx, ibiny)
            else:
                print '.. making band, this should not happen: ', hSum.GetBinContent(ibinx, ibiny), ibinx, ibiny

    return hSum

def getContour(c1, gr):
    c1.cd()
    gr.Draw('cont z list')
    hCont = gr.GetHistogram()
    hCont.SetName(gr.GetName()+'conttemp')
    hCont.Draw('cont z list')
    ROOT.gPad.Update()
    conts = ROOT.gROOT.GetListOfSpecials().FindObject("contours")
    cont0 = conts.At(0).First()
    cont0.SetName(gr.GetName()+'cont')
    # print type(cont0)
    return cont0

def saturateGr(gr, corner, xmin=-20., xmax=20., ymin=-2.0, ymax=2.0, close=False):
    
    if gr.GetN() <= 1: # fixme: 0?
        return
    
    gr_x = gr.GetX()
    gr_y = gr.GetY()
    
    gr_x_new = [x for x in gr_x]
    gr_y_new = [y for y in gr_y]

    if corner == 'tl':
        if gr_y_new[0] != ymax:
            gr_x_new.insert(0, gr_x[0])
            gr_y_new.insert(0, ymax)
        if gr_x_new[-1] != xmin:
            gr_x_new.append(xmin)
            gr_y_new.append(gr_y_new[-1])
        if close:
            gr_x_new.append(xmin)
            gr_y_new.append(ymax)
            gr_x_new.append(gr_x_new[0])
            gr_y_new.append(gr_y_new[0])
    
    elif corner == 'tr':
        if gr_x_new[0] != xmax:
            gr_x_new.insert(0, xmax)
            gr_y_new.insert(0, gr_y_new[0])
        if gr_y_new[-1] != ymax:
            gr_x_new.append(gr_x_new[-1])
            gr_y_new.append(ymax)
        if close:
            gr_x_new.append(xmax)
            gr_y_new.append(ymax)
            gr_x_new.append(gr_x_new[0])
            gr_y_new.append(gr_y_new[0])
    else:
        print 'saturate graph: todo this corner', corner

    gr.Set(0) # reset the graph
    for idx in range(0, len(gr_x_new)):
        gr.SetPoint(idx, gr_x_new[idx], gr_y_new[idx])

    # I see that these points are in decreasing x
    # for idx in range(0, len(gr_x)):
        # print corner, idx, gr_x[idx], gr_y[idx]

def reflectGr(gr):
    grNew = ROOT.TGraph()
    grNew.SetName('refl_'+gr.GetName())
    gr_x = gr.GetX()
    gr_y = gr.GetY()
    for idx in range(0, len(gr_x)):
        x = -1.*gr_x[idx]
        y = -1.*gr_y[idx]
        grNew.SetPoint(idx, x, y)
    return grNew

def mergeBands(b1, b2):
    gMerged = ROOT.TGraph()
    gMerged.SetName('merged_'+b1.GetName())
    x1 = b1.GetX()
    x2 = b2.GetX()
    y1 = b1.GetY()
    y2 = b2.GetY()
    for idx in range(0, len(x1)):
        gMerged.SetPoint(gMerged.GetN(), x1[idx], y1[idx])
    for idx in reversed(range(0, len(x2))):
        gMerged.SetPoint(gMerged.GetN(), x2[idx], y2[idx])
    return gMerged

def setStyles(hframe, canv):
    textfont      = 43
    lumitextfont  = 42
    legtextsize   = 18
    axistextsize  = 30
    axislabelsize = 22
    ndivisions    = 505
    xtitleoffset  = 1.0
    ytitleoffset  = 1.1
    ratioxtitleoffset = 2.8

    hframe.GetXaxis().SetTitleFont(textfont)
    hframe.GetYaxis().SetTitleFont(textfont)
    hframe.GetXaxis().SetLabelFont(textfont)
    hframe.GetYaxis().SetLabelFont(textfont)
    hframe.GetXaxis().SetTitleOffset(xtitleoffset)
    hframe.GetYaxis().SetTitleOffset(ytitleoffset)
    hframe.GetXaxis().SetTitleSize(axistextsize)
    hframe.GetYaxis().SetTitleSize(axistextsize)
    hframe.GetXaxis().SetLabelSize(axislabelsize)
    hframe.GetYaxis().SetLabelSize(axislabelsize)

    c1.SetLeftMargin(0.125)
    c1.SetBottomMargin(0.13)
    c1.SetTopMargin(0.065)

# read the numbering scheme of points
# transl = readTranslation('logPointGeneration.txt')
transl = readTranslation('logPointGeneration_finerGrid_onlyproc.txt')
# read what I excluded from the file
# exclMap = readExclusionFile('mapFor2DKlKtPlot_bands.txt')
exclMap = readExclusionFile('mapFor2DKlKtPlot_bands_2673.txt')

outFolder = 'exclPlots/'

idxs = {
    'obs'     : 0,
    'exp'     : 1,
    'exp_p1s' : 2,
    'exp_m1s' : 3,
    'exp_p2s' : 4,
    'exp_m2s' : 5,
}

##### prepare graphs 2D

hObs = prepareAllGridHisto(transl, exclMap, 'obs', idxs['obs'])
grObs = [getGr(x) for x in hObs]

hExp = prepareAllGridHisto(transl, exclMap, 'exp', idxs['exp'])
grExp = [getGr(x) for x in hExp]

hExp_p1s = prepareAllGridHisto(transl, exclMap, 'exp_p1s', idxs['exp_p1s'])
grExp_p1s = [getGr(x) for x in hExp_p1s]

hExp_m1s = prepareAllGridHisto(transl, exclMap, 'exp_m1s', idxs['exp_m1s'])
grExp_m1s = [getGr(x) for x in hExp_m1s]

hExp_p2s = prepareAllGridHisto(transl, exclMap, 'exp_p2s', idxs['exp_p2s'])
grExp_p2s = [getGr(x) for x in hExp_p2s]

hExp_m2s = prepareAllGridHisto(transl, exclMap, 'exp_m2s', idxs['exp_m2s'])
grExp_m2s = [getGr(x) for x in hExp_m2s]

### control tgraphs - with dots
stdObs = makeStandardGraph(transl, exclMap, 'obs', idxs['obs'])
stdExp = makeStandardGraph(transl, exclMap, 'exp', idxs['exp'])
stdExp_p1s = makeStandardGraph(transl, exclMap, 'exp_p1s', idxs['exp_p1s'])
stdExp_m1s = makeStandardGraph(transl, exclMap, 'exp_m1s', idxs['exp_m1s'])
stdExp_p2s = makeStandardGraph(transl, exclMap, 'exp_p2s', idxs['exp_p2s'])
stdExp_m2s = makeStandardGraph(transl, exclMap, 'exp_m2s', idxs['exp_m2s'])

##### add the histos so that they form a sigma band

# hExp_1s = [combineIntoBand(hExp_m1s[0], hExp_p1s[0]), combineIntoBand(hExp_m1s[1], hExp_p1s[1])]
# hExp_2s = [combineIntoBand(hExp_m2s[0], hExp_p2s[0]), combineIntoBand(hExp_m2s[1], hExp_p2s[1])]
# grExp_1s = [getGr(x) for x in hExp_1s]
# grExp_2s = [getGr(x) for x in hExp_2s]

c1 = ROOT.TCanvas("c1", "c1", 600, 600)

ROOT.gStyle.SetOptStat(0)

############### make plots to control the interpolation
hframe = ROOT.TH2F("hframe", ";k_{#lambda};k_{t}", 100, -20, 20, 100, -1.99, 1.99)

if False:
    hframe.Draw()
    for x in grObs:
        x.Draw('col same')
    stdObs.Draw('p same')
    c1.Update()
    c1.Print(outFolder+'/control_Obs.pdf', 'pdf')

    hframe.Draw()
    for x in grExp:
        x.Draw('col same')
    stdExp.Draw('p same')
    c1.Update()
    c1.Print(outFolder+'/control_Exp.pdf', 'pdf')

    hframe.Draw()
    for x in grExp_p1s:
        x.Draw('col same')
    stdExp_p1s.Draw('p same')
    c1.Update()
    c1.Print(outFolder+'/control_Exp_p1s.pdf', 'pdf')

    hframe.Draw()
    for x in grExp_m1s:
        x.Draw('col same')
    stdExp_m1s.Draw('p same')
    c1.Update()
    c1.Print(outFolder+'/control_Exp_m1s.pdf', 'pdf')

    hframe.Draw()
    for x in grExp_p2s:
        x.Draw('col same')
    stdExp_p2s.Draw('p same')
    c1.Update()
    c1.Print(outFolder+'/control_Exp_p2s.pdf', 'pdf')

    hframe.Draw()
    for x in grExp_m2s:
        x.Draw('col same')
    stdExp_m2s.Draw('p same')
    c1.Update()
    c1.Print(outFolder+'/control_Exp_m2s.pdf', 'pdf')

############
# now: 1sigma - 2 sigma bands as superposition of histos
# (because delaunay intepolation does not work well wil bands)
# note also that I cannot play the combineIntoBand trick on this histos, because the ranges returned are different
hFrom_grExp_p1s = [x.GetHistogram() for x in grExp_p1s]
hFrom_grExp_m1s = [x.GetHistogram() for x in grExp_m1s]
hFrom_grExp_p2s = [x.GetHistogram() for x in grExp_p2s]
hFrom_grExp_m2s = [x.GetHistogram() for x in grExp_m2s]


## put everyting in a scale from 0 to 5
# 0 --> white
# 1 --> gray dark
# 2 --> gray bright
# for x in hFrom_grExp_p1s:
#     x.Scale(2.)
#     x.SetMaximum(5.)

# for x in hFrom_grExp_m1s:
#     x.Scale(3.)
#     x.SetMaximum(5.)

# hFrom_grExp_p2s[0].Draw('colz same')
# hFrom_grExp_p1s[1].Draw('colz same')

# raw_input()

# ROOT.gStyle.SetNumberContours(13)
# hFrom_grExp_m2s[0].Draw('col same')
# hFrom_grExp_p2s[0].Draw('col same')
# for x in h_finer_Exp_2s:
#     x.Draw('col same')
# hObs.Draw('colz same')



# for g in grObs:
#     # g.SetFillColor(ROOT.kCyan)
#     g.Draw('col same')

# for g in grExp_2s:
#     g.Draw('col same')

# #### do the following to extract a tgprah for which I can set a fill style..
# grExp_2s[0].Draw('col')
# hObs_sm = grExp_2s[0].GetHistogram()
# hframe.Draw()
# hObs_sm.SetFillColor(ROOT.kCyan)
# hObs_sm.SetLineColor(ROOT.kCyan)
# hObs_sm.Draw('cont z list')
# ROOT.gPad.Update()
# conts = ROOT.gROOT.GetListOfSpecials().FindObject("contours")
# cont0 = conts.At(0).First()
# hframe.Draw()
# # cont0.SetPoint(cont0.GetN(), -20., 2.0)
# cont0.Draw("L same")


# ### does not work properly!! Some contours are done badly --> I need to "saturate" them
contObs = [ROOT.TGraph(getContour(c1, x)) for x in grObs]
contExp = [ROOT.TGraph(getContour(c1, x)) for x in grExp]
contExp_p1s = [ROOT.TGraph(getContour(c1, x)) for x in grExp_p1s]
contExp_m1s = [ROOT.TGraph(getContour(c1, x)) for x in grExp_m1s]
contExp_p2s = [ROOT.TGraph(getContour(c1, x)) for x in grExp_p2s if x.GetN()>0]
contExp_m2s = [ROOT.TGraph(getContour(c1, x)) for x in grExp_m2s if x.GetN()>0]

for idx, x in enumerate(contObs): saturateGr(x, corner=('tl' if idx==0 else 'tr'), close=True)
for idx, x in enumerate(contExp): saturateGr(x, corner=('tl' if idx==0 else 'tr'))
for idx, x in enumerate(contExp_p1s): saturateGr(x, corner=('tl' if idx==0 else 'tr'))
for idx, x in enumerate(contExp_m1s): saturateGr(x, corner=('tl' if idx==0 else 'tr'))
for idx, x in enumerate(contExp_p2s): saturateGr(x, corner=('tl' if idx==0 else 'tr'))
for idx, x in enumerate(contExp_m2s): saturateGr(x, corner=('tl' if idx==0 else 'tr'))

ngr = len(contObs)
for idx in range(0,ngr): contObs.append(ROOT.TGraph(reflectGr(contObs[idx])))
for idx in range(0,ngr): contExp.append(ROOT.TGraph(reflectGr(contExp[idx])))
for idx in range(0,ngr): contExp_p1s.append(ROOT.TGraph(reflectGr(contExp_p1s[idx])))
for idx in range(0,ngr): contExp_m1s.append(ROOT.TGraph(reflectGr(contExp_m1s[idx])))
for idx in range(0,ngr): contExp_p2s.append(ROOT.TGraph(reflectGr(contExp_p2s[idx])))
for idx in range(0,ngr): contExp_m2s.append(ROOT.TGraph(reflectGr(contExp_m2s[idx])))

## merge together the sigma bands
contExp_1s = []
contExp_2s = []

for idx in range(0, len(contExp_p1s)):
    g1s = ROOT.TGraph(mergeBands(contExp_p1s[idx], contExp_m1s[idx]))
    contExp_1s.append(g1s)

for idx in range(0, len(contExp_p2s)):
    g2s = ROOT.TGraph(mergeBands(contExp_p2s[idx], contExp_m2s[idx]))
    contExp_2s.append(g2s)

# print "+2 sigma: tl"
# contExp_p2s[0].Print()
# print '\n\n'
# print "-2 sigma: tl"
# contExp_m2s[0].Print()

contObs_line = [x.Clone('cl'+x.GetName()) for x in contObs]
for x in contObs_line:
    x.SetLineWidth(1)
    x.SetLineColor(ROOT.kBlue+3)
############
# now make plot

color1sigma = ROOT.TColor.GetColor('#DDDDDD')
color2sigma = ROOT.TColor.GetColor('#999999')
colorisolines = ROOT.TColor.GetColor('#BBBBBB')

for x in contExp_2s:
    x.SetFillColor(color2sigma)
    x.SetLineColor(color2sigma)
    x.SetMarkerColor(color2sigma)
    x.SetLineWidth(2)
    x.SetLineStyle(7)
    x.SetMarkerStyle(8)
    x.SetMarkerSize(1)


# this does not form a band
for idx in range(0, len(contExp_m2s)):
    contExp_m2s[idx].SetFillColor(color2sigma)
    contExp_m2s[idx].SetLineColor(color2sigma)
    contExp_m2s[idx].SetMarkerColor(color2sigma)
    contExp_m2s[idx].SetLineWidth(2)
    contExp_m2s[idx].SetLineStyle(7)
    contExp_m2s[idx].SetMarkerStyle(8)
    contExp_m2s[idx].SetMarkerSize(1)

gr2sBR = ROOT.TGraph(reflectGr(contExp_2s[0]))
gr2sBR.SetFillColor(color2sigma)
gr2sBR.SetLineColor(color2sigma)
gr2sBR.SetMarkerColor(color2sigma)
gr2sBR.SetLineWidth(2)
gr2sBR.SetLineStyle(1)
gr2sBR.SetMarkerStyle(8)
gr2sBR.SetMarkerSize(1)


for x in contExp_1s:
    x.SetFillColor(color1sigma)
    x.SetLineColor(color1sigma)
    x.SetMarkerColor(color1sigma)
    x.SetLineWidth(2)
    x.SetLineStyle(1)
    x.SetMarkerStyle(8)
    x.SetMarkerSize(1)

for x in contExp:
    x.SetLineColor(ROOT.kBlack)
    x.SetLineWidth(2)
    x.SetLineStyle(7)
    x.SetMarkerStyle(8)
    x.SetMarkerSize(1)

for x in contObs:
    # x.SetFillColorAlpha(ROOT.kCyan+1, 0.35);
    x.SetFillColorAlpha(ROOT.kAzure + 6, 0.5)
    x.SetLineColor(ROOT.kBlue+3)
    x.SetLineWidth(1)

setStyles(hframe, c1)
hframe.Draw()
contExp_2s[0].Draw('F same')

contExp_m2s[1].SetPoint(contExp_m2s[1].GetN(), 20., 2.0)
contExp_m2s[1].Draw('F same')

contExp_m2s[3].SetPoint(contExp_m2s[3].GetN(), -20., -2.0)
contExp_m2s[3].Draw('F same')

gr2sBR.Draw('F same')

for x in contExp_1s:
    x.Draw("F same")

for x in contExp:
    x.Draw('L same')

for x in contObs:
    x.Draw('F same')

for x in contObs_line:
    x.Draw("L same")

####
xsfunc = ROOT.TF2('xsfunc', functionGF_klkt, -20., 20., -2., 2.)
isolines = [0.1, 2., 5., 10., 20., 50., 100., 200., 500.]
# isolines = [200.]
xsfunc.SetContour(len(isolines), array('d', isolines))
xsfunc.SetNpx(250)
xsfunc.SetNpy(250)
xsfunc.SetLineColor(colorisolines)
xsfunc.SetLineStyle(3)
xsfunc.SetLineWidth(1)

xsfunc.Draw('cont3 same')

SMmarker = ROOT.TMarker(1.0, 1.0, 33)
SMmarker.SetMarkerSize(2.0)
SMmarker.SetMarkerColor(ROOT.kRed+1)
SMmarker.Draw()
### make legend
# leg = ROOT.TLegend(0.11, 0.0, 0.73, 0.08)
leg = ROOT.TLegend(0.11, 0.0, 0.76, 0.08)
leg.SetFillStyle(0)
leg.SetBorderSize(0)
leg.SetTextFont(43)
leg.SetTextSize(17)

# leg.AddEntry(gr_obs, "Observed CLs", 'f')
# leg.AddEntry(gr_exp, "Expected CLs", 'l')
# leg.AddEntry(gr_1sigma, "Expected #pm1#sigma", 'f')
# leg.AddEntry(gr_2sigma, "Expected #pm2#sigma", 'f')

leg.SetNColumns(3)
# leg.SetHeader("95% CL excluded")
# leg.AddEntry(contObs[0], "Observed CLs", 'f')
# leg.AddEntry(contExp_1s[0], "Expected #pm1#sigma", 'f')
# leg.AddEntry(SMmarker, "SM", 'p')
# leg.AddEntry(contExp[0], "Expected CLs", 'l')
# leg.AddEntry(gr2sBR, "Expected #pm2#sigma", 'f')
leg.AddEntry(contObs[0], "Observed 95% CL excl.", 'f')
leg.AddEntry(contExp_1s[0], "Expected #pm1#sigma", 'f')
leg.AddEntry(SMmarker, "SM", 'p')
leg.AddEntry(contExp[0], "Expected 95% CL excl.", 'l')
leg.AddEntry(gr2sBR, "Expected #pm2#sigma", 'f')


leg.Draw()

extratexts = getExtraTexts()
for extra in extratexts:
    extratexts[extra].Draw()

isoLinesLabels = makeIsoLinesLabels()
for lab in isoLinesLabels:
    print 'label:' , lab
    isoLinesLabels[lab].Draw()

lfake = ROOT.TLine(21, -0.8, 21, -0.65)
lfake.SetLineColor(ROOT.kGray)
lfake.SetLineStyle(3)
lfake.SetLineWidth(1)
lfake.Draw()


##############

c1.RedrawAxis()
redrawBorder()

# grExp_p2s[1].Draw('col same')
# for x in contExp_2s:
#     x.Draw('f same')



# for x in contObs:
#     x.Draw('F same')
# for x in contObs+ contExp+ contExp_p1s+ contExp_m1s+ contExp_p2s+ contExp_m2s:
#     x.Draw('l same')
# grExp_p1s[0].Draw('col same')
# contExp_p1s[0].Draw('l same')
# grExp_p1s[1].Draw('col same')
# contExp_p1s[1].Draw('l same')

# # contExp_1s = []
# # contExp_2s = []

# # ## put together the exclusion bands
# # for i in range(0, len(contExp_p1s)):
# #     contExp_1s.append(contExp_m1s[i].Clone('band'+contExp_m1s[i].GetName()))
# #     x_p1s = contExp_p1s[i].GetX()
# #     y_p1s = contExp_p1s[i].GetY()
# #     for idx in reversed(range(0, len(x_p1s))):
# #         contExp_1s[-1].SetPoint(contExp_1s[-1].GetN(), x_p1s[idx], y_p1s[idx])

# #     contExp_2s.append(contExp_m2s[i].Clone('band'+contExp_m2s[i].GetName()))
# #     x_p2s = contExp_p2s[i].GetX()
# #     y_p2s = contExp_p2s[i].GetY()
# #     for idx in reversed(range(0, len(x_p2s))):
# #         contExp_2s[-1].SetPoint(contExp_2s[-1].GetN(), x_p2s[idx], y_p2s[idx])

# ## now plot!
# hframe.Draw()
# # print contObs


# for x in grObs+ grExp+ grExp_p1s+ grExp_m1s+ grExp_p2s+ grExp_m2s:
#     x.Draw("colz same")
# # for c in contExp_2s:
# #     c.Draw("L same")

c1.Update()
raw_input()

c1.Print("klktscanHH.pdf", 'pdf')