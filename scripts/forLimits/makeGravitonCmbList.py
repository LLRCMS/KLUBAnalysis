import parseLimitFile as plf
import os

inFolder = {
    'lmr90' : '/home/llr/cms/ortona/diHiggs/CMSSW_7_4_7/src/KLUBAnalysis/combiner/cards_Combined_2017_05_02_Graviton_lmr90',
    'hmr90' : '/home/llr/cms/ortona/diHiggs/CMSSW_7_4_7/src/KLUBAnalysis/combiner/cards_Combined_2017_05_02_Graviton_hmr90'
}

dirs = [o for o in os.listdir(inFolder['lmr90']) if os.path.isdir(os.path.join(inFolder['lmr90'],o))]
print '... found', len(dirs), 'mass points, values are:'
masses = [int(x.replace('Graviton', '').replace('HHKin_mass_raw', '')) for x in dirs]
print masses

### make list of points of tgraph
pts = {
    'lmr90' : [],
    'hmr90' : [],
}

for bdt in ['lmr90', 'hmr90']:
    for idx, ddir in enumerate(dirs):
        mass = masses[idx]
        fName = inFolder[bdt] + '/' + ddir + '/' + ('out_Asym_Graviton%i_noTH.log' % mass)
        
        ## all limits in fb -- were produced for sigma X BR = 1 pb, multipluy by 1000
        obs   = 1000.*plf.parseFile(fName, exp=False)
        exp   = 1000.*plf.parseFile(fName)
        m1s_t = 1000.*plf.parseFile(fName, CL='16.0')
        p1s_t = 1000.*plf.parseFile(fName, CL='84.0')
        m2s_t = 1000.*plf.parseFile(fName, CL=' 2.5')
        p2s_t = 1000.*plf.parseFile(fName, CL='97.5')
        
        # convert in errors w.r.t. the exo value
        p2s = p2s_t - exp
        p1s = p1s_t - exp
        m2s = exp - m2s_t
        m1s = exp - m1s_t

        # format: mass, obs, exp, +1sigma, -1sigma, +2sigma, -2sigma
        out = (mass, obs, exp, p1s, m1s, p2s, m2s)
        pts[bdt].append(out)


pts['all'] = []
for pt in pts['lmr90']:
    if pt[0] <= 350:
        print '... lmr90, mass:', pt[0]
        pts['all'].append(pt)    
for pt in pts['hmr90']:
    if pt[0] > 350:
        print '~~~ hmr90, mass:', pt[0]
        pts['all'].append(pt)    

for bdt in pts:
    fout = open('grav_'+bdt+'.txt', 'w')
    fout.write('#mass, obs, exp, +1sigma, -1sigma, +2sigma, -2sigma\n')
    for out in pts[bdt]:
        for val in out: fout.write(str(val) + ' ')
        fout.write('\n')
    fout.close()
