from ROOT import *
from array import *

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

    def addPlotUserBinning (self, plotName, binning):
        bins =  array('d',binning)
        nbins = len(binning) - 1
      
        plot_pass = TH1F ('plot_pass_'+plotName+'_'+self.name, 'plot_pass_'+plotName+'_'+self.name, nbins, bins)
        plot_tot  = TH1F ('plot_tot_'+plotName+'_'+self.name, 'plot_tot_'+plotName+'_'+self.name, nbins, bins)
        self.plots[plotName] = [plot_pass, plot_tot]
        
        ## call sumw2
        for x in self.plots[plotName]:
            x.Sumw2(True)
            
            
    def makeEffPlot(self, plotName,toMg):
        # hwPass = self.plots[plotName][0].Clone(self.name+'_eff_'+plotName)
        # hwTot = self.plots[plotName][2].Clone(self.name+'_tot_'+plotName)
        # hwPass.Add(self.plots[plotName][1], -1)
        # hwTot.Add(self.plots[plotName][3], -1)
        # hwPass.Divide(hwTot)
        # self.effplots[plotName] = hwPass
        hwPass = self.plots[plotName][0].Clone(self.name+'_pass_'+plotName)
        hwTot = self.plots[plotName][1].Clone(self.name+'_tot_'+plotName)
        grEff = TGraphAsymmErrors()
        grEff.SetName(self.name+'_eff_'+plotName)
        grEff.BayesDivide(hwPass, hwTot)
        grEff.SetMarkerStyle(8)
        self.effplots[plotName] = grEff
        if toMg: 
            self.toMgPlots[plotName] = grEff

    def makeMultiGraph(self,mgName):
        mg = TMultiGraph(mgName,mgName)
        for x in self.toMgPlots:
            mg.Add(self.toMgPlots[x])
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

    
Plots = PlotSet("Plots")



Plots.addPlot('Et_bjet1',15,0,300)
Plots.addPlot('Et_bjet2',15,0,300)
Plots.addPlot('Et_bjet',15,0,300)

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

Plots.addPlot('MergedJets_dR',15,0,5)
Plots.addPlot('MergedJets_Hpt',15,0,500)




#### 
fIn   = TFile.Open('skim_stage2_jetstau/output.root')
tIn   = fIn.Get('HTauTauTree')
nEvt  = tIn.GetEntries()
optXXplots = Plots
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
    passes1      = tIn.DeltaRmin_stage2jet_bjet1 < 0.5
    passes2      = tIn.DeltaRmin_stage2jet_bjet2 < 0.5
    isJetMerged =  tIn.Bjet2matchesStage2jet1==1

    cut1_jet1 = tIn.stage2_jet1Et>36
    cut2_jet1 = tIn.stage2_jet1Et>68
    cut3_jet1 = tIn.stage2_jet1Et>128
    cut4_jet1 = tIn.stage2_jet1Et>200

    cut1_jet2 = tIn.stage2_jet2Et>36
    cut2_jet2 = tIn.stage2_jet2Et>68
    cut3_jet2 = tIn.stage2_jet2Et>128
    cut4_jet2 = tIn.stage2_jet2Et>200

    passes1tau      = tIn.DeltaRmin_stage2tau_tau1 < 0.1
    passes2tau      = tIn.DeltaRmin_stage2tau_tau2 < 0.1
    
    isTau1 = tIn.pairType == 2
    isTau2 = tIn.pairType <= 2
    ISO1 =tIn.dau1_MVAiso >=3
    ISO2 =tIn.dau2_MVAiso >=3
    PassIsOS =tIn.isOS==1   
    isTauMerged =  tIn.Lepton2matchesStage2tau1==1


    #fill plots
    optXXplots.getPlot('Et_bjet1',False).Fill(Et_bjet1)
    if (passes1): optXXplots.getPlot('Et_bjet1',True).Fill(Et_bjet1)
    optXXplots.getPlot('Et_bjet2',False).Fill(Et_bjet2)
    if (passes2): optXXplots.getPlot('Et_bjet2',True).Fill(Et_bjet2)
    
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
   




fIn.Close()



Plots.makeEffPlot('Et_bjet1',0)
Plots.makeEffPlot('Et_bjet2',0)
Plots.makeEffPlot('Et_bjet',1)
Plots.makeEffPlot('Et_bjet_cut1',1)
Plots.makeEffPlot('Et_bjet_cut2',1)
Plots.makeEffPlot('Et_bjet_cut3',1)
Plots.makeEffPlot('Et_bjet_cut4',1)

Plots.makeEffPlot('Pt_bjet1',0)
Plots.makeEffPlot('Pt_bjet2',0)
Plots.makeEffPlot('Pt_bjet',0)


Plots.makeEffPlot('MergedJets_dR',0)
Plots.makeEffPlot('MergedJets_Hpt',0)
Plots.makeEffPlot('PtTau1',0)
Plots.makeEffPlot('PtTau2',0)
Plots.makeEffPlot('PtTau',0)




Plots.getMgPlot('Et_bjet_cut1').SetMarkerColor(4)
Plots.getMgPlot('Et_bjet_cut2').SetMarkerColor(3)
Plots.getMgPlot('Et_bjet_cut3').SetMarkerColor(5)
Plots.getMgPlot('Et_bjet_cut4').SetMarkerColor(2)
Plots.makeMultiGraph('Et_bjet_cuts')



fOut= TFile ("skim_stage2_jetstau/effPlots.root", "recreate")
Plots.saveToFile(fOut)






