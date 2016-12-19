from ROOT import *
from array import *

class PlotSet:
    def __init__(self, name):
        self.name = name
        self.plots = {}
        

    def addPlot (self, plotName, nbins, xmin, xmax,xlabel,ylabel):
        plot = TH1F ('plot_'+plotName+'_'+self.name, 'plot_'+plotName+'_'+self.name, nbins, xmin, xmax)
        plot.Sumw2(True)       
        plot.SetTitle('')
        plot.GetXaxis().SetTitle(xlabel)
        plot.GetYaxis().SetTitle(ylabel)
       
        self.plots[plotName] = plot
        
        
            

    def addPlotUserBinning (self, plotName, binning):
        bins =  array('d',binning)
        nbins = len(binning) - 1
      
        plot = TH1F ('plot_'+plotName+'_'+self.name, 'plot_'+plotName+'_'+self.name, nbins, bins)
        plot.Sumw2(True)        
        self.plots[plotName] = plot
                    
            
    
   
    def saveToFile(self, tFile):
        tFile.cd()
        for x in self.plots:
            self.plots[x].Write()
        
    def getPlot (self, name): 
        return self.plots[name]

    def normPlot(self,name):
        integral =self.plots[name].GetEntries()
        self.plots[name].Scale(1./integral)
        return self.plots[name]

    def set_palette(self,name):
        ncontours=999
        if name == "gray" or name == "grayscale":
            stops = [0.00, 0.34, 0.61, 0.84, 1.00]
            red   = [1.00, 0.84, 0.61, 0.34, 0.00]
            green = [1.00, 0.84, 0.61, 0.34, 0.00]
            blue  = [1.00, 0.84, 0.61, 0.34, 0.00]
            
        else:
            stops = [0.00, 0.34, 0.61, 0.84, 1.00]
            red   = [0.00, 0.00, 0.87, 1.00, 0.51]
            green = [0.00, 0.81, 1.00, 0.20, 0.00]
            blue  = [0.51, 1.00, 0.12, 0.00, 0.00]
            
        s = array('d', stops)
        r = array('d', red)
        g = array('d', green)
        b = array('d', blue)

        npoints = len(s)
        TColor.CreateGradientColorTable(npoints, s, r, g, b, ncontours)
        gStyle.SetNumberContours(ncontours)

    def add2Dplot(self,name,xbins,xmin,xmax, ybins,ymin,ymax,xlabel,ylabel):
        
  
        plot2d = TH2F ('plot_'+name+'_'+self.name, 'plot_'+name+'_'+self.name, xbins, xmin, xmax, ybins, ymin,ymax)
        plot2d.GetXaxis().SetTitle(xlabel)
        plot2d.GetYaxis().SetTitle(ylabel)
        plot2d.Draw("colz")
        self.plots[name] = plot2d

Plots = PlotSet("Plots")


Plots.addPlot('Unmerged_EtResJets',15,-1,1,'(L1jet E_{T}-offline bjet E_{T})/offline bjet E_{T}','Events')
Plots.addPlot('Merged_EtResJets',5,-1,1,'(L1jet E_{T}-offline bjets sumE_{T})/offline bjets sumE_{T}','Events')
Plots.addPlot('Unmerged_EtResJets_norm',15,-1,1,'(L1jet E_{T}-offline bjet E_{T})/offline bjet E_{T}','a.u.')
Plots.addPlot('Merged_EtResJets_norm',5,-1,1,'(L1jet E_{T}-offline bjets sumE_{T})/offline bjets sumE_{T}','a.u.')
Plots.addPlot('dib_Dr',20,0,5,'#Delta R(bjet1,bjet2)','Events')
Plots.addPlot('DeltaRminJets',30,0,2,'#Delta R(stage2jet,bjet)','Events')
Plots.addPlot('DeltaRminTaus',30,0,2,'#Delta R(stage2#tau,#tau)','Events')
Plots.addPlot('PtTau1',30,0,200,'P_{T} #tau1','Events')
Plots.addPlot('PtTau2',30,0,200,'P_{T} #tau2','Events')
Plots.add2Dplot('UnmergedEt_stage2jetVSbjet',50,0,300,50,0,300,'offline bjet E_{T}','L1jet E_{T}')
Plots.add2Dplot('MergedEt_stage2jetVSbjet',50,0,300,50,0,300,'offline bjets sumE_{T}','L1jet E_{T}')
#### 
fIn   = TFile.Open('skim_stage2_TauTauPt20/output.root')
tIn   = fIn.Get('HTauTauTree')
nEvt  = tIn.GetEntries()

for ev in range(0, nEvt):
    tIn.GetEntry(ev)


    
    #variables
    
    Et_bjet1 = tIn.bjet1_et   
    Et_bjet2 = tIn.bjet2_et
    Et_L1jet1 = tIn.stage2_jet1Et   
    Et_L1jet2 = tIn.stage2_jet2Et
    Pt_bjet1 = tIn.bjet1_pt   
    Pt_bjet2 = tIn.bjet2_pt
    EtJetRes1 = (tIn.stage2_jet1Et - tIn.bjet1_et)/tIn.bjet1_et
    EtJetRes2 = (tIn.stage2_jet2Et - tIn.bjet2_et)/tIn.bjet2_et
    EtJetResM = (tIn.stage2_jet1Et - tIn.bjet2_et- tIn.bjet1_et)/(tIn.bjet2_et+tIn.bjet1_et)
    bH_pt = tIn.bH_pt    
    PtTau1 = tIn.dau1_pt
    PtTau2 = tIn.dau2_pt
    tauH_pt = tIn.tauH_pt    
    DeltaRmin_stage2jet_bjet1=tIn.DeltaRmin_stage2jet_bjet1
    DeltaRmin_stage2jet_bjet2=tIn.DeltaRmin_stage2jet_bjet2
    DeltaRmin_stage2tau_tau1=tIn.DeltaRmin_stage2tau_tau1
    DeltaRmin_stage2tau_tau2=tIn.DeltaRmin_stage2tau_tau2
    dib_deltaR =  tIn.dib_deltaR

    #conditions   
    Etbjet1cut = Et_bjet1>30
    Etbjet2cut = Et_bjet2>30
    isTauMerged =  tIn.Lepton2matchesStage2tau1==1
    ditau_deltaR =  tIn.ditau_deltaR
    isJetMerged =  tIn.Bjet2matchesStage2jet1==1
    passes1      = DeltaRmin_stage2jet_bjet1 < 0.5
    passes2      = DeltaRmin_stage2jet_bjet2 < 0.5
    passes1tau      = DeltaRmin_stage2tau_tau1 < 0.2
    passes2tau      = DeltaRmin_stage2tau_tau2 < 0.2
    isTau1 = tIn.pairType == 2
    isTau2 = tIn.pairType <= 2
    ISO1 =tIn.dau1_MVAiso >=3
    ISO2 =tIn.dau2_MVAiso >=3
    PassIsOS =tIn.isOS==1   

    if not (isJetMerged) and (passes1 and Etbjet1cut): Plots.getPlot('Unmerged_EtResJets').Fill(EtJetRes1)
    if not (isJetMerged) and (passes2 and Etbjet2cut): Plots.getPlot('Unmerged_EtResJets').Fill(EtJetRes2)

    if (isJetMerged) and (passes1 and passes2 and Etbjet1cut and Etbjet2cut): Plots.getPlot('Merged_EtResJets').Fill(EtJetResM)

    if not (isJetMerged) and (passes1): Plots.getPlot('Unmerged_EtResJets_norm').Fill(EtJetRes1)
    if not (isJetMerged) and (passes2): Plots.getPlot('Unmerged_EtResJets_norm').Fill(EtJetRes2)

    if (isJetMerged) and (passes1 or passes2): Plots.getPlot('Merged_EtResJets_norm').Fill(EtJetResM)
    
    if not (isJetMerged) and (passes1): Plots.getPlot('UnmergedEt_stage2jetVSbjet').Fill(Et_bjet1,Et_L1jet1)
    if not (isJetMerged) and (passes2): Plots.getPlot('UnmergedEt_stage2jetVSbjet').Fill(Et_bjet2,Et_L1jet2)

    if (isJetMerged) and (passes1 or passes2): Plots.getPlot('MergedEt_stage2jetVSbjet').Fill((Et_bjet2+Et_bjet1),(Et_L1jet2+Et_L1jet1))

    Plots.getPlot('dib_Dr').Fill(dib_deltaR)
    Plots.getPlot('DeltaRminJets').Fill(DeltaRmin_stage2jet_bjet1)
    Plots.getPlot('DeltaRminJets').Fill(DeltaRmin_stage2jet_bjet2)
    Plots.getPlot('DeltaRminTaus').Fill(DeltaRmin_stage2tau_tau1)
    Plots.getPlot('DeltaRminTaus').Fill(DeltaRmin_stage2tau_tau2)
    if(isTau1): Plots.getPlot('PtTau1').Fill(PtTau1)
    if(isTau1):Plots.getPlot('PtTau2').Fill(PtTau2)
    
Plots.normPlot('Unmerged_EtResJets_norm')
Plots.normPlot('Merged_EtResJets_norm')


   


fIn.Close()





fOut = TFile ("skim_stage2_TauTauPt20/Plots.root", "recreate")
Plots.saveToFile(fOut)
Plots.set_palette('normal')

