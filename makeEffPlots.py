from ROOT import *
from array import *
from math import fabs,sqrt

class PlotSet:
    def __init__(self, name):
        self.name = name
        self.plots = {} 
        self.effplots = {} ## contains efficiency plots
        self.toMgPlots= {} ## efficiency plots to put in the multigraph
        self.mgplots= {} ## contains multigraph plots
 
    def addPlot (self, plotName, nbins, xmin, xmax):
        plot_pass = TH1F ('plot_pass_'+plotName+'_'+self.name, 'plot_pass_'+plotName+'_'+self.name, nbins, xmin, xmax)
        plot_tot  = TH1F ('plot_tot_'+plotName+'_'+self.name, 'plot_tot_'+plotName+'_'+self.name, nbins, xmin, xmax)
        self.plots[plotName] = [plot_pass, plot_tot]
        
        ## call sumw2
        for x in self.plots[plotName]:
            x.Sumw2(True)

    def add2DPlot (self, plotName, xbins, xmin, xmax,ybins,ymin,ymax):
        plot_pass = TH2F ('plot_pass_'+plotName+'_'+self.name, 'plot_pass_'+plotName+'_'+self.name, xbins, xmin, xmax,ybins,ymin,ymax)
        plot_tot  = TH2F ('plot_tot_'+plotName+'_'+self.name, 'plot_tot_'+plotName+'_'+self.name, xbins, xmin, xmax,ybins,ymin,ymax)
        self.plots[plotName] = [plot_pass, plot_tot]
        
        ## call sumw2
        for x in self.plots[plotName]:
            x.Sumw2(True)

    def addPlotUserBinning (self, plotName, binning):
        bins =  array('d',binning)
        nbins = len(binning) - 1
      
        plot_pass = TH1F ('plot_pass_'+plotName+'_'+self.name, 'plot_pass_'+plotName+'_'+self.name, nbins, bins)
        plot_tot  = TH1F ('plot_tot_'+plotName+'_'+self.name, 'plot_tot_'+plotName+'_'+self.name, nbins, bins)
        self.plots[plotName] = [plot_pass, plot_tot]
        
        ## call sumw2
        for x in self.plots[plotName]:
            x.Sumw2(True)
            
            
    def makeEffPlot(self, plotName,toMg,xlabel,ylabel):
        # hwPass = self.plots[plotName][0].Clone(self.name+'_eff_'+plotName)
        # hwTot = self.plots[plotName][2].Clone(self.name+'_tot_'+plotName)
        # hwPass.Add(self.plots[plotName][1], -1)
        # hwTot.Add(self.plots[plotName][3], -1)
        # hwPass.Divide(hwTot)
        # self.effplots[plotName] = hwPass
        hwPass = self.plots[plotName][0].Clone(self.name+'_pass_'+plotName)
        hwTot = self.plots[plotName][1].Clone(self.name+'_tot_'+plotName)
        grEff = TGraphAsymmErrors()
        grEff.SetName(self.name+'_'+plotName)
        grEff.BayesDivide(hwPass, hwTot)
        grEff.SetMarkerStyle(8)
        grEff.GetXaxis().SetTitle(xlabel)
        grEff.GetYaxis().SetTitle(ylabel)
        self.effplots[plotName] = grEff
        if toMg: 
            self.toMgPlots[plotName] = grEff

    def make2DEffPlot(self, plotName,xlabel,ylabel):
        # hwPass = self.plots[plotName][0].Clone(self.name+'_eff_'+plotName)
        # hwTot = self.plots[plotName][2].Clone(self.name+'_tot_'+plotName)
        # hwPass.Add(self.plots[plotName][1], -1)
        # hwTot.Add(self.plots[plotName][3], -1)
        # hwPass.Divide(hwTot)
        # self.effplots[plotName] = hwPass
        hEff = self.plots[plotName][0].Clone(self.name+'_pass_'+plotName)
        hTot = self.plots[plotName][1].Clone(self.name+'_tot_'+plotName)
        
        hEff.SetName(self.name+'_'+plotName)
        hEff.Divide(hTot)
        
        hEff.GetXaxis().SetTitle(xlabel)
        hEff.GetYaxis().SetTitle(ylabel)
        self.effplots[plotName] = hEff
 


   
    def makeMultiGraph(self,mgName,xlabel,ylabel):
        mg = TMultiGraph()
        mg.SetName(mgName)
        for x in self.toMgPlots:
            mg.Add(self.toMgPlots[x])
        leg = TLegend(0.65,0.11,0.89,0.28)
        for x in self.toMgPlots:
            leg.AddEntry(Plots.getMgPlot(x),Plots.getMgPlot(x).GetName(),"ep")
        mg.Draw('lap')        
        
        SetOwnership(leg,0)
        leg.Draw()   
        
        mg.GetXaxis().SetTitle(xlabel)
        mg.GetYaxis().SetTitle(ylabel)        
        self.mgplots[mgName] = mg
            
    def saveToFile(self, tFile):
        tFile.cd()
        for x in self.plots:
            for y in self.plots[x]:
                y.Write()
        for x in self.effplots:
            self.effplots[x].Write()
        for x in self.mgplots:
            self.mgplots[x].Write()

    def getPlot (self, name, isPass): # isPass: True: for _pass_, False for _tot_, 
        if isPass:
            return self.plots[name][0]

        if not isPass:
            return self.plots[name][1]

    def getEffPlot (self, name): 
        return self.effplots[name]

    def getMgPlot (self, name): 
        return self.toMgPlots[name]


def make2dList(rows, cols):
    a=[]
    for row in xrange(rows): a += [[0]*cols]
    return a


Plots = PlotSet("Plots")



Plots.addPlot('Et_bjet1',15,0,300)
Plots.addPlot('Et_bjet2',15,0,300)
Plots.addPlot('Et_bjet',15,0,300)
Plots.addPlot('Et_bjet_Dr05',15,0,300)



Plots.addPlot('Et_bjet_cut1',15,0,300)
Plots.addPlot('Et_bjet_cut2',15,0,300)
Plots.addPlot('Et_bjet_cut3',15,0,300)
Plots.addPlot('Et_bjet_cut4',15,0,300)


Plots.addPlot('Pt_bjet1',15,0,300)
Plots.addPlot('Pt_bjet2',15,0,300)
Plots.addPlot('Pt_bjet',15,0,300)


Plots.addPlot('PtTau1',15,0,200)
Plots.addPlot('PtTau2',15,0,200)
Plots.addPlot('PtTau',15,0,200)

Plots.add2DPlot('L1eff_DoubleIsoTau28er_PtTau',15,0,200,15,0,200)
Plots.add2DPlot('L1eff_DoubleIsoTau36er_PtTau',15,0,200,15,0,200)
Plots.add2DPlot('L1eff_DoubleTau50er_PtTau',15,0,200,15,0,200)
Plots.add2DPlot('L1eff_OR_PtTau',15,0,200,15,0,200)







Plots.addPlot('MergedJets_dR',15,0,5)
Plots.addPlot('MergedJets_Hpt',15,0,500)




#### 
fIn   = TFile.Open('skim_stage2_TauTauPt20_250GeV/output.root')
tIn   = fIn.Get('HTauTauTree')
nEvt  = tIn.GetEntries()
optXXplots = Plots

seeds = 10

L1pass = []
for seed in xrange(seeds): 
    L1pass += [0]
L1tot = 0                     


for ev in range(0, nEvt):
    tIn.GetEntry(ev)

    #variables
    Et_bjet1 = tIn.bjet1_et   
    Et_bjet2 = tIn.bjet2_et
    Pt_bjet1 = tIn.bjet1_pt   
    Pt_bjet2 = tIn.bjet2_pt
    dib_deltaR =  tIn.dib_deltaR
    bH_pt = tIn.bH_pt

    PtTau1 = tIn.dau1_pt
    PtTau2 = tIn.dau2_pt
    ditau_deltaR =  tIn.ditau_deltaR
    tauH_pt = tIn.tauH_pt
    
    #conditions
    passes1      = tIn.DeltaRmin_stage2jet_bjet1 < 1
    passes2      = tIn.DeltaRmin_stage2jet_bjet2 < 1
    isJetMerged =  tIn.Bjet2matchesStage2jet1==1

    cut1_jet1 = tIn.stage2_jet1Et>36
    cut2_jet1 = tIn.stage2_jet1Et>68
    cut3_jet1 = tIn.stage2_jet1Et>128
    cut4_jet1 = tIn.stage2_jet1Et>200

    cut1_jet2 = tIn.stage2_jet2Et>36
    cut2_jet2 = tIn.stage2_jet2Et>68
    cut3_jet2 = tIn.stage2_jet2Et>128
    cut4_jet2 = tIn.stage2_jet2Et>200

    passes1tau      = tIn.DeltaRmin_stage2tau_tau1 < 0.5
    passes2tau      = tIn.DeltaRmin_stage2tau_tau2 < 0.5
    
    isTau1 = tIn.pairType == 2
    isTau2 = tIn.pairType <= 2
    ISO1 =tIn.dau1_MVAiso >=3
    ISO2 =tIn.dau2_MVAiso >=3
    PassIsOS =tIn.isOS==1   
    
    isTauMerged =  tIn.Lepton2matchesStage2tau1==1
    TauTau40 = PtTau1>0 and PtTau2>0
    isL1tau1ISO = tIn.stage2_tau1Iso ==1
    L1seedEtaTau1 = fabs(tIn.stage2_tau1Eta) <2.1
    L1seedEtaTau2 = fabs(tIn.stage2_tau2Eta) <2.1
    isL1tau2ISO = tIn.stage2_tau2Iso ==1 
    areL1tausISO = isL1tau1ISO and isL1tau2ISO  
#FIXME fai na lista se no ci vogliono 10 anni
    L1seed1 = L1seedEtaTau1 and L1seedEtaTau2 and  areL1tausISO and (tIn.stage2_tau1Et > 28) and (tIn.stage2_tau2Et > 28)
    L1seed2 = L1seedEtaTau1 and L1seedEtaTau2 and  areL1tausISO and (tIn.stage2_tau1Et > 30) and (tIn.stage2_tau2Et > 30)
    L1seed3 = L1seedEtaTau1 and L1seedEtaTau2 and  areL1tausISO and (tIn.stage2_tau1Et > 32) and (tIn.stage2_tau2Et > 32)
    L1seed4 = L1seedEtaTau1 and L1seedEtaTau2 and  areL1tausISO and (tIn.stage2_tau1Et > 33) and (tIn.stage2_tau2Et > 33)
    L1seed5 = L1seedEtaTau1 and L1seedEtaTau2 and  areL1tausISO and (tIn.stage2_tau1Et > 34) and (tIn.stage2_tau2Et > 34)
    L1seed6 = L1seedEtaTau1 and L1seedEtaTau2 and  areL1tausISO and (tIn.stage2_tau1Et > 35) and (tIn.stage2_tau2Et > 35)
    L1seed7 = L1seedEtaTau1 and L1seedEtaTau2 and  areL1tausISO and (tIn.stage2_tau1Et > 36) and (tIn.stage2_tau2Et > 36)
    L1seed8 = L1seedEtaTau1 and L1seedEtaTau2 and (tIn.stage2_tau1Et > 50) and (tIn.stage2_tau2Et > 50)
    L1seed9 = L1seedEtaTau1 and L1seedEtaTau2 and (tIn.stage2_tau1Et > 70) and (tIn.stage2_tau2Et > 70)
    L1seedOR = L1seed1 or L1seed2 or L1seed3 or L1seed4 or L1seed5 or L1seed6 or L1seed7 or L1seed8 or L1seed9    

    

    #fill plots
    optXXplots.getPlot('Et_bjet1',False).Fill(Et_bjet1)
    if (passes1): optXXplots.getPlot('Et_bjet1',True).Fill(Et_bjet1)
    optXXplots.getPlot('Et_bjet2',False).Fill(Et_bjet2)
    if (passes2): optXXplots.getPlot('Et_bjet2',True).Fill(Et_bjet2)
    
    optXXplots.getPlot('Et_bjet_Dr05',False).Fill(Et_bjet1)
    optXXplots.getPlot('Et_bjet_Dr05',False).Fill(Et_bjet2)
    if (tIn.DeltaRmin_stage2jet_bjet1 < 0.5): optXXplots.getPlot('Et_bjet_Dr05',True).Fill(Et_bjet1)
    if (tIn.DeltaRmin_stage2jet_bjet2 < 0.5): optXXplots.getPlot('Et_bjet_Dr05',True).Fill(Et_bjet2)

    optXXplots.getPlot('Et_bjet',False).Fill(Et_bjet1)
    optXXplots.getPlot('Et_bjet',False).Fill(Et_bjet2)
    if (passes1): optXXplots.getPlot('Et_bjet',True).Fill(Et_bjet1)
    if (passes2): optXXplots.getPlot('Et_bjet',True).Fill(Et_bjet2)

    

    optXXplots.getPlot('Pt_bjet1',False).Fill(Pt_bjet1)
    if (passes1): optXXplots.getPlot('Pt_bjet1',True).Fill(Pt_bjet1)
    optXXplots.getPlot('Pt_bjet2',False).Fill(Pt_bjet2)
    if (passes2): optXXplots.getPlot('Pt_bjet2',True).Fill(Pt_bjet2)
    
    optXXplots.getPlot('Pt_bjet',False).Fill(Pt_bjet1)
    optXXplots.getPlot('Pt_bjet',False).Fill(Pt_bjet2)
    if (passes1): optXXplots.getPlot('Pt_bjet',True).Fill(Pt_bjet1)
    if (passes2): optXXplots.getPlot('Pt_bjet',True).Fill(Pt_bjet2)

    
   
    optXXplots.getPlot('Et_bjet_cut1',False).Fill(Et_bjet1)
    optXXplots.getPlot('Et_bjet_cut1',False).Fill(Et_bjet2)
    if (passes1 and cut1_jet1): optXXplots.getPlot('Et_bjet_cut1',True).Fill(Et_bjet1)
    if (passes2 and isJetMerged and cut1_jet1): optXXplots.getPlot('Et_bjet_cut1',True).Fill(Et_bjet2)
    if (passes2 and cut1_jet2) and not (isJetMerged): optXXplots.getPlot('Et_bjet_cut1',True).Fill(Et_bjet2)
    
    optXXplots.getPlot('Et_bjet_cut2',False).Fill(Et_bjet1)
    optXXplots.getPlot('Et_bjet_cut2',False).Fill(Et_bjet2)
    if (passes1 and cut2_jet1): optXXplots.getPlot('Et_bjet_cut2',True).Fill(Et_bjet1)
    if (passes2 and isJetMerged and cut2_jet1): optXXplots.getPlot('Et_bjet_cut2',True).Fill(Et_bjet2)
    if (passes2 and cut2_jet2) and not (isJetMerged): optXXplots.getPlot('Et_bjet_cut2',True).Fill(Et_bjet2)

    optXXplots.getPlot('Et_bjet_cut3',False).Fill(Et_bjet1)
    optXXplots.getPlot('Et_bjet_cut3',False).Fill(Et_bjet2)
    if (passes1 and cut3_jet1): optXXplots.getPlot('Et_bjet_cut3',True).Fill(Et_bjet1)
    if (passes2 and isJetMerged and cut3_jet1): optXXplots.getPlot('Et_bjet_cut3',True).Fill(Et_bjet2)
    if (passes2 and cut3_jet2) and not (isJetMerged): optXXplots.getPlot('Et_bjet_cut3',True).Fill(Et_bjet2)


    optXXplots.getPlot('Et_bjet_cut4',False).Fill(Et_bjet1)
    optXXplots.getPlot('Et_bjet_cut4',False).Fill(Et_bjet2)
    if (passes1 and cut4_jet1): optXXplots.getPlot('Et_bjet_cut4',True).Fill(Et_bjet1)
    if (passes2 and isJetMerged and cut4_jet1): optXXplots.getPlot('Et_bjet_cut4',True).Fill(Et_bjet2)
    if (passes2 and cut4_jet2) and not (isJetMerged): optXXplots.getPlot('Et_bjet_cut4',True).Fill(Et_bjet2)

    
    


    optXXplots.getPlot('MergedJets_dR',False).Fill(dib_deltaR)
    if (isJetMerged): optXXplots.getPlot('MergedJets_dR',True).Fill(dib_deltaR)

    optXXplots.getPlot('MergedJets_Hpt',False).Fill(bH_pt)
    if (isJetMerged): optXXplots.getPlot('MergedJets_Hpt',True).Fill(bH_pt)


    

    if (isTau1 and PassIsOS): optXXplots.getPlot('PtTau1',False).Fill(PtTau1)
    if (passes1tau and isTau1 and PassIsOS): optXXplots.getPlot('PtTau1',True).Fill(PtTau1)
    if (isTau2 and PassIsOS): optXXplots.getPlot('PtTau2',False).Fill(PtTau2)
    if (passes2tau and isTau2 and PassIsOS): optXXplots.getPlot('PtTau2',True).Fill(PtTau2)
   
    if (isTau1 and PassIsOS): optXXplots.getPlot('PtTau',False).Fill(PtTau1)
    if (isTau2 and PassIsOS): optXXplots.getPlot('PtTau',False).Fill(PtTau2)
    if (passes1tau and isTau1 and PassIsOS): optXXplots.getPlot('PtTau',True).Fill(PtTau1)
    if (passes2tau and isTau2 and PassIsOS): optXXplots.getPlot('PtTau',True).Fill(PtTau2)


    if (isTau1 and PassIsOS and passes1tau and passes2tau): optXXplots.getPlot('L1eff_DoubleIsoTau28er_PtTau',False).Fill(PtTau1,PtTau2)
    if (isTau1 and PassIsOS and passes1tau  and passes2tau and L1seed1): optXXplots.getPlot('L1eff_DoubleIsoTau28er_PtTau',True).Fill(PtTau1,PtTau2)

    if (isTau1 and PassIsOS and passes1tau  and passes2tau): optXXplots.getPlot('L1eff_DoubleIsoTau36er_PtTau',False).Fill(PtTau1,PtTau2)
    if (isTau1 and PassIsOS and passes1tau  and passes2tau and L1seed7): optXXplots.getPlot('L1eff_DoubleIsoTau36er_PtTau',True).Fill(PtTau1,PtTau2)

    if (isTau1 and PassIsOS and passes1tau  and passes2tau): optXXplots.getPlot('L1eff_DoubleTau50er_PtTau',False).Fill(PtTau1,PtTau2)
    if (isTau1 and PassIsOS and passes1tau  and passes2tau and L1seed9): optXXplots.getPlot('L1eff_DoubleTau50er_PtTau',True).Fill(PtTau1,PtTau2)

    if (isTau1 and PassIsOS and passes1tau  and passes2tau): optXXplots.getPlot('L1eff_OR_PtTau',False).Fill(PtTau1,PtTau2)
    if (isTau1 and PassIsOS and passes1tau  and passes2tau and L1seedOR): optXXplots.getPlot('L1eff_OR_PtTau',True).Fill(PtTau1,PtTau2)
    
       
    ### overall L1 efficiency
    if (isTau1 and PassIsOS and TauTau40 and passes1tau  and passes2tau): L1tot+=1    
    if (passes1tau and isTau1 and PassIsOS and TauTau40  and passes2tau and L1seed1):L1pass[0]+=1
    if (passes1tau and isTau1 and PassIsOS and TauTau40  and passes2tau and L1seed2):L1pass[1]+=1
    if (passes1tau and isTau1 and PassIsOS and TauTau40  and passes2tau and L1seed3):L1pass[2]+=1
    if (passes1tau and isTau1 and PassIsOS and TauTau40  and passes2tau and L1seed4):L1pass[3]+=1
    if (passes1tau and isTau1 and PassIsOS and TauTau40  and passes2tau and L1seed5):L1pass[4]+=1
    if (passes1tau and isTau1 and PassIsOS and TauTau40  and passes2tau and L1seed6):L1pass[5]+=1
    if (passes1tau and isTau1 and PassIsOS and TauTau40  and passes2tau and L1seed7):L1pass[6]+=1
    if (passes1tau and isTau1 and PassIsOS and TauTau40  and passes2tau and L1seed8):L1pass[7]+=1
    if (passes1tau and isTau1 and PassIsOS and TauTau40  and passes2tau and L1seed9):L1pass[8]+=1
    if (passes1tau and isTau1 and PassIsOS and TauTau40  and passes2tau and L1seedOR):L1pass[9]+=1


fIn.Close()

print("L1eff")
L1eff = 0
DeltaL1eff = 0
for seed in range(0,10):
    L1eff =float(L1pass[seed])/L1tot 
    DeltaL1eff = sqrt(L1eff*(1-L1eff)*float(L1tot))    
    print("{first}+\-{second}".format(first=L1eff,second=DeltaL1eff))

Plots.makeEffPlot('Et_bjet1',0,'E_{T} bjet1','Efficiency')
Plots.makeEffPlot('Et_bjet2',0,'E_{T} bjet2','Efficiency')
Plots.makeEffPlot('Et_bjet',1,'E_{T} bjet','Efficiency')
Plots.makeEffPlot('Et_bjet_Dr05',0,'E_{T} bjet','Efficiency')
Plots.makeEffPlot('Et_bjet_cut1',1,'E_{T} bjet','Efficiency')
Plots.makeEffPlot('Et_bjet_cut2',1,'E_{T} bjet','Efficiency')
Plots.makeEffPlot('Et_bjet_cut3',1,'E_{T} bjet','Efficiency')
Plots.makeEffPlot('Et_bjet_cut4',1,'E_{T} bjet','Efficiency')

Plots.makeEffPlot('Pt_bjet1',0,'P_{T} bjet1','Efficiency')
Plots.makeEffPlot('Pt_bjet2',0,'P_{T} bjet2','Efficiency')
Plots.makeEffPlot('Pt_bjet',0,'P_{T} bjet','Efficiency')


Plots.makeEffPlot('MergedJets_dR',0,'#DeltaR(bjet1,bjet2)','ratio merged jets')
Plots.makeEffPlot('MergedJets_Hpt',0,'E_{T} H_{bb}','ratio merged jets')
Plots.makeEffPlot('PtTau1',0,'P_{T} #tau1','Efficiency')
Plots.makeEffPlot('PtTau2',0,'P_{T} #tau2','Efficiency')
Plots.makeEffPlot('PtTau',0,'P_{T} #tau','Efficiency')

Plots.make2DEffPlot('L1eff_DoubleIsoTau28er_PtTau','P_{T} #tau 1','P_{T} #tau 2')
Plots.make2DEffPlot('L1eff_DoubleIsoTau36er_PtTau','P_{T} #tau 1','P_{T} #tau 2')
Plots.make2DEffPlot('L1eff_DoubleTau50er_PtTau','P_{T} #tau 1','P_{T} #tau 2')
Plots.make2DEffPlot('L1eff_OR_PtTau','P_{T} #tau 1','P_{T} #tau 2')





Plots.getMgPlot('Et_bjet_cut1').SetMarkerColor(4)
Plots.getMgPlot('Et_bjet_cut2').SetMarkerColor(3)
Plots.getMgPlot('Et_bjet_cut3').SetMarkerColor(5)
Plots.getMgPlot('Et_bjet_cut4').SetMarkerColor(2)




Plots.makeMultiGraph('Et_bjet_cuts','E_{T} bjet','Efficiency')

fOut= TFile ("skim_stage2_TauTauPt20_250GeV/effPlots250.root", "recreate")
Plots.saveToFile(fOut)







