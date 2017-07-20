## small tool to convert HTauTau trigger parametrizations into a TF1 that can be easily read in c++
## because JSON is a very clever idea to share numbers, right?

import ROOT
import json
from pprint import pprint
import math

## the CB function
def _crystalballEfficiency(m, m0, sigma, alpha, n, norm):
  
    sqrtPiOver2 = math.sqrt(ROOT.TMath.PiOver2())
    sqrt2       = math.sqrt(2.)
    sig         = abs(sigma)
    t           = (m - m0)/sig * alpha / abs(alpha)
    absAlpha    = abs(alpha/sig)
    a           = ROOT.TMath.Power(n/absAlpha, n) * ROOT.TMath.Exp(-0.5 * absAlpha * absAlpha)
    b           = absAlpha - n/absAlpha
    arg         = absAlpha / sqrt2;
  
    if   arg >  5.: ApproxErf =  1.
    elif arg < -5.: ApproxErf = -1.
    else          : ApproxErf = ROOT.TMath.Erf(arg)
  
    leftArea    = (1. + ApproxErf) * sqrtPiOver2
    rightArea   = ( a * 1./ROOT.TMath.Power(absAlpha-b, n-1) ) / (n - 1)
    area        = leftArea + rightArea
  
    if t <= absAlpha:
        arg = t / sqrt2
        if   arg >  5.: ApproxErf =  1.
        elif arg < -5.: ApproxErf = -1.
        else          : ApproxErf = ROOT.TMath.Erf(arg)
        return norm * (1 + ApproxErf) * sqrtPiOver2 / area
  
    else:
        return norm * (leftArea + a * (1/ROOT.TMath.Power(t-b,n-1) - 1/ROOT.TMath.Power(absAlpha - b,n-1)) / (1 - n)) / area

def crystalballEfficiency(x, par):
    x     = x[0]
    m0    = par[0]
    sigma = par[1]
    alpha = par[2]
    n     = par[3]
    norm  = par[4]
    return _crystalballEfficiency( x, m0, sigma, alpha, n, norm)

def retrieveParams(jsondata, section):
    alpha = jsondata[section]["alpha"] 
    m0    = jsondata[section]["m_{0}"]
    sigma = jsondata[section]["sigma"] 
    norm  = jsondata[section]["norm"] 
    n     = jsondata[section]["n"]
    return (m0, sigma, alpha, n, norm)

##############################################################################

path = 'tau_trigger_SF_2016/di-tau/' # common to all

## input files
inFiles = [
    'mediumMVAiso/real_taus_Mediumiso.json',
    'tightMVAiso/real_taus_Tightiso.json',
]

#tag for every file opened
desc = [
 'mediumMVAIso',
 'tightMVAIso',
]

fOut = ROOT.TFile('tau_trigger_SF_2016.root', 'recreate')

##############################################################################
##############################################################################
if path[-1] != '/':
    path += '/'

inFiles = [path + x for x in inFiles]
for idx, x in enumerate(inFiles):
    print '...' , desc [idx] , '-->', x

# decModes = [0, 1, 10]

## open jsons
for idx, f in enumerate(inFiles):
    with open(f) as data_file:    
        data = json.load(data_file)

    for decMode in (0, 1, 10):
        
        ################## MC
        tf1name = '%s_eff_%s_DM%i' % (desc[idx], 'MC', decMode)
        
        #### ma come si fa a lasciare spazi cosi!!!!!
        secName =  "MC decay mode =  %i" % decMode
        if decMode == 10:
            secName =  "MC decay mode = %i" % decMode

        params = retrieveParams(data, secName)

        tf1 = ROOT.TF1(tf1name, crystalballEfficiency, 0, 1000, 5)
        tf1.SetParameters(params[0], params[1], params[2], params[3], params[4])

        fOut.cd()
        tf1.Write()

        ################## data
        for period in ["BCDEFG", "H"]:
            tf1name = '%s_eff_%s_DM%i' % (desc[idx], 'Data'+period, decMode)
            
            #### ma come si fa a lasciare spazi cosi!!!!!
            secName =  "Data %s decay mode =  %i" % (period, decMode)
            if decMode == 10:
                secName =  "Data %s decay mode = %i" % (period, decMode)

            params = retrieveParams(data, secName)

            tf1 = ROOT.TF1(tf1name, crystalballEfficiency, 0, 1000, 5)
            tf1.SetParameters(params[0], params[1], params[2], params[3], params[4])

            fOut.cd()
            tf1.Write()

### now make scale factors -- let's not waste too much time after
# to combine SF based on lumi, here is the list of lumi in /fb per run period
# B 5.892
# C 2.646
# D 4.353
# E 4.117
# F 3.186
# G 7.721
# H 8.857
# TOT: 36.772

# B-G : 27.915 (frac: 0.7591)
# H   : 8.857  (frac: 0.24086)

# fBG = 27.915/36.772
# fH = 8.857/36.772

# print fBG, fH, fBG + fH

# for dsc in desc:
#     for DM in (0, 1, 10):
#         tf1MC = fOut.Get('%s_eff_MC_DM%i' % (dsc, DM))
#         tf1dataBG = fOut.Get('%s_eff_DataBCDEFG_DM%i' % (dsc, DM))
#         tf1dataH  = fOut.Get('%s_eff_DataH_DM%i' % (dsc, DM))

#         fSFname = '%s_SF_DM%i' % (dsc, DM)
#         fNewDesc = '(%f*%s + %f*%s)/%s' % (fBG, tf1dataBG.GetName(), fH, tf1dataH.GetName(), tf1MC.GetName())
#         print fNewDesc
#         fnew = ROOT.TF1(fSFname, fNewDesc, 0, 1000)
#         fOut.cd()
#         fnew.Write()


