import numpy as np
import math 

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



### this is my generation of points. I need to remap index to kl kt pair
# lmin   = -15.0
# lmax   = 15.0
# lstep  = 1.0

# ktmin  = -0.5
# ktmax  = 2.0
# ktstep = 0.25


# klvals = np.arange(lmin,lmax,lstep)
# ktvals = np.arange(ktmin,ktmax,ktstep)

remap = [] # progressive idx is idx of sample

# fremap = open('../logPointGeneration_finerGrid.txt')
fremap = open('../logPointGeneration_finerGrid_morepts.txt')

# for kt in ktvals:
#     for kl in klvals:
        # remap.append([kl,kt])
for line in fremap:
    line = line.strip()
    tokens = line.split()
    kl = float(tokens[1])
    kt = float(tokens[2])
    remap.append([kl,kt])
# print remap
# for i in range(0, 300):
#     print i, remap[i]
#####################

# print remap

# basePath = '/home/llr/cms/ortona/diHiggs/CMSSW_7_4_7/src/KLUBAnalysis/combiner/cards_Combined_2017_03_01_2D_lmr70/ggHH_bbtt{0}MT2/out_Asym_ggHH_bbtt{0}.log'
# limitResults = '/home/llr/cms/ortona/diHiggs/CMSSW_7_4_7/src/KLUBAnalysis/combiner/points2D.txt'
# limitResults = '/home/llr/cms/ortona/diHiggs/CMSSW_7_4_7/src/KLUBAnalysis/combiner/results2d.txt'
# limitResults = '/home/llr/cms/ortona/diHiggs/CMSSW_7_4_7/src/KLUBAnalysis/combiner/results2673.txt'
limitResults = '/home/llr/cms/cadamuro/HH2016_limits/CMSSW_7_4_7/src/KLUBAnalysis/combiner/extended2D_pointResults.txt'

### prepare result per point
## mass comb_obs comb_exp [-1sigma +1sigma] {-2sigma +2sigma}
limits = {}
lrfile = open(limitResults)
for line in lrfile:
    if 'comb_obs' in line:
        continue
    line = line.strip()
    if not line:
        continue
    tokens = line.split()
    hasSigmaBands = (len(tokens) > 9)
    hasSigmaBands = True
    # print hasSigmaBands

    ipt = int(tokens[0])
    
    if not hasSigmaBands:
        exp = float(tokens[8])
        obs = float(tokens[7])
    else:
        # exp = float(tokens[20])
        # obs = float(tokens[19])

        # exp_p1s = float(tokens[22])
        # exp_p2s = float(tokens[24])

        # exp_m1s = float(tokens[21])
        # exp_m2s = float(tokens[23])
        exp = float(tokens[2])
        obs = float(tokens[1])

        exp_p1s = float(tokens[4])
        exp_p2s = float(tokens[6])

        exp_m1s = float(tokens[3])
        exp_m2s = float(tokens[5])
        
    # if exp < 0:
    #     continue # poitn failes.. --> but it is a region I dont exclude
    if exp < 0:
        exp = 9999999.
        obs = 9999999.

    SF = 1.0
    exp = exp/(0.073*SF) # so it is in fb over SM xs
    obs = obs/(0.073*SF) # so it is in fb over SM xs
    
    if hasSigmaBands: # I need to scale because these limits are the error on the central value --> apply shift
        exp_p1s = exp + exp_p1s/(0.073*SF)
        exp_p2s = exp + exp_p2s/(0.073*SF)
        exp_m1s = exp - exp_m1s/(0.073*SF)
        exp_m2s = exp - exp_m2s/(0.073*SF)

    if not hasSigmaBands:
        limits[ipt] = (exp, obs)
    else:
        limits[ipt] = (exp, obs, exp_p1s, exp_m1s, exp_p2s, exp_m2s)

    if ipt == 0:
        print "obs:       " , 0.073*obs/10.
        print "exp -2sigma" , 0.073*exp_m2s/10.
        print "exp -1sigma" , 0.073*exp_m1s/10.
        print "exp        " , 0.073*exp/10.
        print "exp +1sigma" , 0.073*exp_p1s/10.
        print "exp +2sigma" , 0.073*exp_p2s/10.

if 196 in limits:
    print "SM limit: ", limits[196], "lim/SM : " , getCrossSectionForParameters(1.0, 1.0, 0, 0, 0)[0]
# print getCrossSectionForParameters(1.0, 1.0, 0, 0, 0)

# print limits
# start = 0
start = 2673

fMap = open('../mapFor2DKlKtPlot_bands_2673_to_3563.txt', 'w')
for ipt in range(start, start+len(remap)):
    
    if not ipt in limits:
        print 'skipping pt', ipt
        continue # was failed...
    
    ### check : is this point excluded or not?
    kl = remap[ipt-start][0]
    kt = remap[ipt-start][1]
    thisxs = getCrossSectionForParameters(kl, kt, 0, 0, 0)[0]
    print " .. ", kl, kt, thisxs

    thisexp = limits[ipt][0]
    thisobs = limits[ipt][1]

    expToExclude = thisexp < thisxs
    obsToExclude = thisobs < thisxs

    i_expToExclude = 1 if expToExclude else 0
    i_obsToExclude = 1 if obsToExclude else 0

    if len(limits[ipt]) > 2: ## has sigma vars
        i_exp_p1s = 1 if (limits[ipt][2] < thisxs) else 0
        i_exp_m1s = 1 if (limits[ipt][3] < thisxs) else 0
        i_exp_p2s = 1 if (limits[ipt][4] < thisxs) else 0
        i_exp_m2s = 1 if (limits[ipt][5] < thisxs) else 0
        fMap.write('%i %f %f %i %i %i %i %i %i\n' % (ipt, kl, kt, i_expToExclude, i_obsToExclude, i_exp_p1s, i_exp_m1s, i_exp_p2s, i_exp_m2s))

    else:
        fMap.write('%i %f %f %i %i\n' % (ipt, kl, kt, i_expToExclude, i_obsToExclude))

