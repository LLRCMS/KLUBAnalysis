import numpy as np
import math 

def parseFile(filename, CL='50.0', exp=True):
    f = open(filename)
    matches = []
    for line in f:
        s = ('Expected %s%: r <'%CL)
        if not exp: s = 'Observed Limit: r <'
        
        if not s in line:
            continue
        val = line.replace(s, '')
        val = float(val)
        matches.append(val)
    
    if len(matches) == 0:
        print "did not find any expected in file: " , filename
        return 0.0
    else:
        return matches[-1]

def getCrossSectionForParameters(l, yt, c2, cg, c2g):
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
    hh_sm_xs_up = hh_sm_xs * (1 + math.sqrt(4.3**2 + 5**2 + 2.3**2 + 2.1**2) / 100)
    hh_sm_xs_down = hh_sm_xs * (1 - math.sqrt(6**2 + 5**2 + 2.3**2 + 2.1**2) / 100)

    res = f(*params)
    return res * hh_sm_xs, res * hh_sm_xs_down, res * hh_sm_xs_up


folder = '/home/llr/cms/ortona/diHiggs/CMSSW_7_4_7/src/KLUBAnalysis/combiner/cards_Combined_2017_03_07_2D_lmr70/ggHH_bbtt{0}MT2/'
pts = [x for x in range(0, 2673)]

fout = open('table_2dscan.txt', 'w')

for pt in pts:
    
    if pt % 100 == 0:
        print pt
    
    fname = folder.format(str(pt)) + 'out_Asym_ggHH_bbtt%i.log' % pt
    # obs = parseFile(fname, '0.0', False)
