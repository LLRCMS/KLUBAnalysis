###Runs in conda env "MLenv"

from correctionlib import _core

#Download the correct JSON files 
evaluator = _core.CorrectionSet.from_file('2016postVFP_UL/electron.json')

#evaluator["JSONMapName"].evaluate("year[2016preVFP,2016postVFP,2017,2018]",
#                                  "ValueType[sf,syst]",
#                                  "WorkingPoint[RecoBelow20,RecoAbove20,Veto,Loose,Medium,Tight,wp80iso,wp80noiso,wp90iso,wp90noiso]",
#                                  eta[float],
#                                  pt[float])

binedges_eta = [-2.5, -2.0, -1.566, -1.444, -1.0, -0.5, 0.0, 0.5, 1.0, 1.444, 1.566, 2.0, 2.5]
binedges_pt = [10., 20., 45., 75., 100., 500.]

binNames_eta = ["Ltm2p0","m2p0tom1p57","m1p57tom1p44","m1p44tom1p0","m1p0tom0p5","m0p5to0p0","0p0to0p5","0p5to1p0","1p0to1p44","1p44to1p57","1p57to2p0","Gt2p0"]

WorkingPoint = "Tight"
period = "2018"

evaluator = _core.CorrectionSet.from_file('{}_UL/electron.json'.format(period))

sf_per_eta = {}
sys_per_eta = {}

for eta_id, etabin in enumerate(binNames_eta):
    sf_per_eta[etabin] = []
    sys_per_eta[etabin] = []
    
    eval_eta = binedges_eta[eta_id]+0.001

    for pt_id, pt in enumerate(binedges_pt):
        eval_pt = pt+1.
        if eval_pt > 500.:
            continue
        
        valsf= evaluator["UL-Electron-ID-SF"].evaluate(period,"sf",WorkingPoint,eval_eta,eval_pt)
        valsys= evaluator["UL-Electron-ID-SF"].evaluate(period,"syst",WorkingPoint,eval_eta,eval_pt)

        sf_per_eta[etabin].append(valsf)
        sys_per_eta[etabin].append(valsys)
            

sf_per_eta["binedges_eta"] = binedges_eta
sf_per_eta["binedges_pt"] = binedges_pt

import json
with open('{0}_{1}_eleSFs.json'.format(period,WorkingPoint), 'w') as f:
    json.dump(sf_per_eta, f)

sys_per_eta["binedges_eta"] = binedges_eta
sys_per_eta["binedges_pt"] = binedges_pt

with open('{0}_{1}_eleSys.json'.format(period,WorkingPoint), 'w') as f:
    json.dump(sys_per_eta, f)




