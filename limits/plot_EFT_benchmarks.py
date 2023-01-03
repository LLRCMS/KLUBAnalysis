import re, optparse
import os.path
import sys
from math import *
import ROOT

ROOT.gROOT.SetBatch(ROOT.kTRUE)

#####

def parseOptions():
    usage = ('usage: %prog [options]\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)

    parser.add_option('-y', '--year'      , dest='year'      , type='string', default="2016"     , help='year')
    parser.add_option('-t', '--tag'       , dest='tag'       , type='string', default="26Nov2022", help='tag')
    parser.add_option('-b', '--BM_set'    , dest='BM_set'    , type='string', default="JHEP04"   , help='JHEP04 or JHEP03')
    parser.add_option('-l', '--limit_type', dest='limit_type', type='string', default="xsBR"     , help='xsBR or xs')
    parser.add_option(      '--blind',      dest='blind',      action="store_true",                help='plot blind limits')

    # store options and arguments as global variables
    global opt, args
    (opt, args) = parser.parse_args()


def redrawBorder():
   # this little macro redraws the axis tick marks and the pad border lines.
   ROOT.gPad.Update()
   ROOT.gPad.RedrawAxis()
   l = ROOT.TLine()
   l.SetLineWidth(3)
   l.DrawLine(ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymax(), ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymax())
   l.DrawLine(ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymin(), ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymax())
   l.DrawLine(ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymin(), ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymax())
   l.DrawLine(ROOT.gPad.GetUxmin(), ROOT.gPad.GetUymin(), ROOT.gPad.GetUxmax(), ROOT.gPad.GetUymin())

def parseFile(filename, CL='50.0', exp=True):
    f = open(filename)
    matches = []
    for line in f:
       search = ('Expected %s%%: r <'%CL)
       if not exp: search = 'Observed Limit: r <'
       if not search in line:
           continue
       val = line.replace(search, '')
       val = float(val)
       matches.append(val)

    if len(matches) == 0:
        print "did not find any expected in file: " , filename, 'CL=', CL, 'exp?=', exp
        return -1.0
    else:
        return matches[-1]


#####

parseOptions()

year       = opt.year
tag        = opt.tag
BM_set     = opt.BM_set
limit_type = opt.limit_type

if BM_set == 'JHEP04':
    benchmarks = ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '8a']
elif BM_set == 'JHEP03':
    benchmarks = ['1b', '2b', '3b', '4b', '5b', '6b', '7b']
else:
    print 'you did not set a correct BM_set'
    sys.exit()

ptsList = [] # (x, obs, exp, p2s, p1s, m1s, m2s)

for i, bm in enumerate(benchmarks):

    if opt.blind: fName = 'EFT_cards_stackedAndCleaned/cards_'+bm+'/cards_'+tag+'_'+year+'/out_Asym_bm'+bm+'_blind.log'
    else:         fName = 'EFT_cards_stackedAndCleaned/cards_'+bm+'/cards_'+tag+'_'+year+'/out_Asym_bm'+bm+'.log'

    # Can get different results on r_gghh:
    #exp = parseFile(fName)                  # <- How many times the SM I'm excluding
    #exp = parseFile(fName) * 31.05          # <- Excluded HH cross section
    #exp = parseFile(fName) * 31.05 * 0.073  # <- Excluded HH cross section times BR(bbtautau)

    if limit_type == 'xsBR':
        exp   = parseFile(fName)            * 1.0 * 0.073  # <- Excluded HH cross section (1 fb) times BR(bbtautau)
        obs   = parseFile(fName, exp=False) * 1.0 * 0.073
        m1s_t = parseFile(fName, CL='16.0') * 1.0 * 0.073
        p1s_t = parseFile(fName, CL='84.0') * 1.0 * 0.073
        m2s_t = parseFile(fName, CL=' 2.5') * 1.0 * 0.073
        p2s_t = parseFile(fName, CL='97.5') * 1.0 * 0.073
    elif limit_type == 'xs':
        exp   = parseFile(fName)
        obs   = parseFile(fName, exp=False)
        m1s_t = parseFile(fName, CL='16.0')
        p1s_t = parseFile(fName, CL='84.0')
        m2s_t = parseFile(fName, CL=' 2.5')
        p2s_t = parseFile(fName, CL='97.5')

    ## because the other code wants +/ sigma vars as deviations, without sign, from the centeal exp value...
    p2s = p2s_t - exp
    p1s = p1s_t - exp
    m2s = exp - m2s_t
    m1s = exp - m1s_t
    xval = i

    ptsList.append((xval, obs, exp, p2s, p1s, m1s, m2s))
    ptsList.append((xval+1, obs, exp, p2s, p1s, m1s, m2s))

#print ptsList

c1 = ROOT.TCanvas("c1", "c1", 500, 500)
c1.SetFrameLineWidth(2)
c1.SetBottomMargin (0.15)
c1.SetRightMargin (0.05)
c1.SetLeftMargin (0.15)
c1.SetGridx()
c1.SetGridy()

mg = ROOT.TMultiGraph()

gr2sigma = ROOT.TGraphAsymmErrors()
gr1sigma = ROOT.TGraphAsymmErrors()
grexp = ROOT.TH1F('exp', 'exp', len(ptsList)/2, 0, len(ptsList)/2)
grobs = ROOT.TH1F('obs', 'obs', len(ptsList)/2, 0, len(ptsList)/2)

for ipt, pt in enumerate(ptsList):
    xval = pt[0]
    obs  = pt[1]
    exp  = pt[2]
    p2s  = pt[3]
    p1s  = pt[4]
    m1s  = pt[5]
    m2s  = pt[6]
    print xval, exp

    grexp.SetBinContent(pt[0]+1, exp)
    grexp.SetBinError(pt[0]+1, 0.0001)
    grobs.SetBinContent(pt[0]+1, obs)
    grobs.SetBinError(pt[0]+1, 0.0001)
    gr1sigma.SetPoint(ipt, xval, exp)
    gr2sigma.SetPoint(ipt, xval, exp)
    gr1sigma.SetPointError(ipt, 0, 0, m1s, p1s)
    gr2sigma.SetPointError(ipt, 0, 0, m2s, p2s)


######## set styles
grexp.SetLineColor(1)
grexp.SetLineWidth(2)
grexp.SetLineStyle(2)
grexp.SetFillColor(0)

grobs.SetLineColor(1)
grobs.SetLineWidth(2)
grobs.SetMarkerColor(1)
grobs.SetMarkerStyle(20)
grobs.SetMarkerSize(0.65)
grobs.SetFillStyle(0)

gr1sigma.SetMarkerStyle(0)
gr1sigma.SetMarkerColor(3)
gr1sigma.SetFillColor(ROOT.kGreen+1)
gr1sigma.SetLineColor(1)
gr1sigma.SetLineWidth(2)
gr1sigma.SetLineStyle(2)
gr1sigma.SetFillStyle(1001)

gr2sigma.SetMarkerStyle(0)
gr2sigma.SetMarkerColor(5)
gr2sigma.SetFillColor(ROOT.kOrange)
gr2sigma.SetLineColor(1)
gr2sigma.SetLineWidth(2)
gr2sigma.SetLineStyle(2)
gr2sigma.SetFillStyle(1001)

mg.Add(gr2sigma, "3")
mg.Add(gr1sigma, "3")

###########

legend = ROOT.TLegend(0,0,0,0)
legend.SetNColumns(2)
legend.SetX1(0.184297)
legend.SetY1(0.75526)
legend.SetX2(0.785703)
legend.SetY2(0.88)
legend.SetFillColor(ROOT.kWhite)
legend.SetBorderSize(0)
legend.SetTextSize(0.025)
# legend.SetHeader('95% CL upper limits', 'c')
if   BM_set == 'JHEP04': legend.SetHeader("95% CL upper limits on JHEP04(2016)126 shape benchmarks")
elif BM_set == 'JHEP03': legend.SetHeader("95% CL upper limits on JHEP03(2020)091 shape benchmarks")
if opt.blind:
    legend.AddEntry(grexp, "Median expected", "l")
    legend.AddEntry(gr1sigma, "68% expected", "lf")
    dummy = ROOT.TH1F("","",1,0,1)
    legend.AddEntry(dummy, "", "")
    legend.AddEntry(gr2sigma, "95% expected", "lf")
else:
    legend.AddEntry(grobs, "Observed", "LP")
    legend.AddEntry(gr1sigma, "68% expected", "lf")
    legend.AddEntry(grexp, "Median expected", "l")
    legend.AddEntry(gr2sigma, "95% expected", "lf")

##### text
pt = ROOT.TPaveText(0.1463218,0.8966667,0.2845977,0.954641,"brNDC")
pt.SetBorderSize(0)
pt.SetTextAlign(12)
pt.SetTextFont(62)
pt.SetTextSize(0.04)
pt.SetFillColor(0)
pt.SetFillStyle(0)
pt.AddText("CMS #font[52]{Preliminary}" )

pt2 = ROOT.TPaveText(0.696111,0.8966667,0.907222,0.954641,"brNDC")
pt2.SetBorderSize(0)
pt2.SetFillColor(0)
pt2.SetTextSize(0.035)
pt2.SetTextFont(42)
pt2.SetFillStyle(0)
if   year == '2016': pt2.AddText("bb#tau#tau 36.3 fb^{-1} (13 TeV)")
elif year == '2017': pt2.AddText("bb#tau#tau 41.5 fb^{-1} (13 TeV)")
elif year == '2018': pt2.AddText("bb#tau#tau 59.7 fb^{-1} (13 TeV)")
else               : pt2.AddText("bb#tau#tau  138 fb^{-1} (13 TeV)")

pt3 = ROOT.TPaveText(0.1963218,0.70526,0.2845977,0.88,"brNDC")
pt3.SetBorderSize(0)
pt3.SetTextAlign(12)
pt3.SetTextFont(42)
pt3.SetTextSize(0.04)
pt3.SetFillColor(0)
pt3.SetFillStyle(0)
# if   BM_set == 'JHEP04': pt3.AddText("JHEP04(2016)126")
# elif BM_set == 'JHEP03': pt3.AddText("JHEP03(2020)091")

hframe = ROOT.TH1F('hframe', '', len(benchmarks), 0, len(benchmarks))
if limit_type == 'xsBR':
    hframe.SetMinimum(1)
    hframe.SetMaximum(10000)
elif limit_type == 'xs':
    hframe.SetMinimum(10)
    hframe.SetMaximum(100000)
c1.SetLogy()

hframe.GetYaxis().SetTitleSize(0.04)
hframe.GetXaxis().SetTitleSize(0.04)
hframe.GetYaxis().SetLabelSize(0.03)
hframe.GetXaxis().SetLabelSize(0.045)
hframe.GetXaxis().SetLabelOffset(0.012)
hframe.GetYaxis().SetTitleOffset(1.2)
hframe.GetXaxis().SetTitleOffset(1.2)

hframe.GetXaxis().SetTitle("Shape Benchmark")
if limit_type == 'xsBR':
    hframe.GetYaxis().SetTitle("95% CL on #sigma(gg#rightarrow HH) #times BR(HH #rightarrow bb#tau#tau)")
elif limit_type == 'xs':
    hframe.GetYaxis().SetTitle("95% CL on #sigma(gg#rightarrow HH)")

hframe.SetStats(0)
ROOT.gPad.SetTicky()
hframe.Draw()
for ibin,bench in enumerate(benchmarks):
    hframe.GetXaxis().SetBinLabel(ibin+1, bench)
    #print "Filling bin:" + str(ibin) + " with bm:" + bench
gr2sigma.Draw("3same")
gr1sigma.Draw("3same")
grexp.Draw("PEsame")
grobs.Draw("PEsame")

pt.Draw()
pt2.Draw()
pt3.Draw()
#redrawBorder()
ROOT.gPad.RedrawAxis()
c1.Update()
c1.RedrawAxis("g")
legend.Draw()
c1.Update()

c1.Draw()
if opt.blind: plotname = "plots/EFT/plot_"+tag+"_"+year+"_"+BM_set+"_"+limit_type+"_blind.pdf"
else:         plotname = "plots/EFT/plot_"+tag+"_"+year+"_"+BM_set+"_"+limit_type+".pdf"
c1.Print(plotname, 'pdf')


