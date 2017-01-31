#!/usr/bin/python 
from ROOT import *
from array import *

DIR= '/data_CMS/cms/amendola/SkimmedNtuples/skim_stage2_GluGluToRadionToHHTo2B2Tau_250GeV/'
FILEIN = 'total.root'
FILEOUT = 'Plots250.root'

FILEIN = DIR+FILEIN
FILEOUT = DIR+FILEOUT

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

    def normPlot(self,name,nbins):
        integral =self.plots[name].Integral(0,nbins+1)
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
#Plots.addPlot('EtResTaus',15,-1,1,'(L1jet E_{T}-offline tau E_{T})/offline tau E_{T}','Events')
Plots.addPlot('Unmerged_EtResJets_norm',15,-1,1,'(L1jet E_{T}-offline bjet E_{T})/offline bjet E_{T}','a.u.')
Plots.addPlot('Unmerged_EtResJets_norm_05',15,-1,1,'(L1jet E_{T}-offline bjet E_{T})/offline bjet E_{T}','a.u.')
Plots.addPlot('Unmerged_EtResJets_norm_05to1',15,-1,1,'(L1jet E_{T}-offline bjet E_{T})/offline bjet E_{T}','a.u.')
Plots.addPlot('Merged_EtResJets_norm',5,-1,1,'(L1jet E_{T}-offline bjets sumE_{T})/offline bjets sumE_{T}','a.u.')
#Plots.addPlot('EtResTaus_norm',15,-1,1,'(L1jet E_{T}-offline tau E_{T})/offline tau E_{T}','a.u.')
Plots.addPlot('dib_Dr',20,0,5,'#Delta R(bjet1,bjet2)','Events')
Plots.addPlot('EtaJets05to1',20,-3,3,'#eta L1jet','Events')
Plots.addPlot('EtaJets05',20,-3,3,'#eta L1jet','Events')
Plots.addPlot('DeltaRminJets',30,0,2,'#Delta R(stage2jet,bjet)','Events')
Plots.addPlot('DeltaRminMuons05to1',30,0,2,'#Delta R(stage2muons,bjet)','Events')
Plots.addPlot('DeltaRminMuons05',30,0,2,'#Delta R(stage2muons,bjet)','Events')
Plots.addPlot('DeltaRminTaus',30,0,2,'#Delta R(stage2#tau,#tau)','Events')
Plots.addPlot('PtTau1',30,0,200,'P_{T} #tau1','Events')
Plots.addPlot('PtTau2',30,0,200,'P_{T} #tau2','Events')
Plots.addPlot('PtHbb',30,0,500,'P_{T} Hbb','Events')
Plots.add2Dplot('EtVSdeltaR_muons05',20,0,2,50,0,200,'#Delta R(stage2muons,bjet)','E_{T} L1 muon')
Plots.add2Dplot('EtVSdeltaR_muons05to1',20,0,2,50,0,200,'#Delta R(stage2muons,bjet)','E_{T} L1 muon')
Plots.add2Dplot('UnmergedEt_stage2jetVSbjet',50,0,300,50,0,300,'offline bjet E_{T}','L1jet E_{T}')
Plots.add2Dplot('UnmergedEt_stage2jetVSbjet_cleaned',50,0,300,50,0,300,'offline bjet E_{T}','L1jet E_{T}')
Plots.add2Dplot('MergedEt_stage2jetVSbjet',50,0,300,50,0,300,'offline bjets sumE_{T}','L1jet E_{T}')
#### 
fIn   = TFile.Open(FILEIN)
if not fIn.IsZombie(): print ('File '+FILEIN+' opened')
tIn   = fIn.Get('HTauTauTree')
nEvt  = tIn.GetEntries()

ratioMerged1     = 0
ratioMerged05    = 0 
ratioMerged05to1 = 0
totEvents1       = 0 
totEvents05      = 0 
totEvents05to1   = 0 



for ev in range(0, nEvt):
    tIn.GetEntry(ev)
    
    #variables
    
    Et_bjet1 = tIn.bjet1_et   
    Et_bjet2 = tIn.bjet2_et
    Et_L1jet1 = tIn.stage2_jet1Et   
    Et_L1jet2 = tIn.stage2_jet2Et
    Eta_L1jet1 = tIn.stage2_jet1Eta
    Eta_L1jet2 = tIn.stage2_jet2Eta
    Pt_bjet1 = tIn.bjet1_pt   
    Pt_bjet2 = tIn.bjet2_pt
    EtJetRes1 = (tIn.stage2_jet1Et - tIn.bjet1_et)/tIn.bjet1_et
    EtJetRes2 = (tIn.stage2_jet2Et - tIn.bjet2_et)/tIn.bjet2_et
    EtJetResM = (tIn.stage2_jet1Et - tIn.bjet2_et- tIn.bjet1_et)/(tIn.bjet2_et+tIn.bjet1_et)
    Et_L1muon1 = tIn.stage2_muon1Et   
    Et_L1muon2 = tIn.stage2_muon2Et    
    bH_pt = tIn.bH_pt    
    PtTau1 = tIn.dau1_pt
    PtTau2 = tIn.dau2_pt
    tauH_pt = tIn.tauH_pt    
    DeltaRmin_stage2jet_bjet1=tIn.DeltaRmin_stage2jet_bjet1
    DeltaRmin_stage2jet_bjet2=tIn.DeltaRmin_stage2jet_bjet2
    DeltaRmin_stage2tau_tau1=tIn.DeltaRmin_stage2tau_tau1
    DeltaRmin_stage2tau_tau2=tIn.DeltaRmin_stage2tau_tau2
    DeltaRmin_stage2muon_bjet1=tIn.DeltaRmin_stage2muon_bjet1
    DeltaRmin_stage2muon_bjet2=tIn.DeltaRmin_stage2muon_bjet2
    dib_deltaR =  tIn.dib_deltaR

    #conditions   
    Etbjet1cut          = Et_bjet1>30
    Etbjet2cut          = Et_bjet2>30
    isTauMerged         = tIn.Lepton2matchesStage2tau1==1
    ditau_deltaR        = tIn.ditau_deltaR
    isJetMerged         = tIn.Bjet2matchesStage2jet1==1
    passes1             = DeltaRmin_stage2jet_bjet1 < 1
    passes2             = DeltaRmin_stage2jet_bjet2 < 1
    passes1Delta05      = DeltaRmin_stage2jet_bjet1 < 0.5
    passes2Delta05      = DeltaRmin_stage2jet_bjet2 < 0.5
    passes1Delta05to1   = DeltaRmin_stage2jet_bjet1 > 0.5 and passes1
    passes2Delta05to1   = DeltaRmin_stage2jet_bjet2 > 0.5 and passes2
    passes1tau          = DeltaRmin_stage2tau_tau1 < 0.2
    passes2tau          = DeltaRmin_stage2tau_tau2 < 0.2
    isTau1              = tIn.pairType == 2
    isTau2              = tIn.pairType <= 2
    ISO1                = tIn.dau1_MVAiso >=3
    ISO2                = tIn.dau2_MVAiso >=3
    PassIsOS            = tIn.isOS==1   
    isMuon1             = DeltaRmin_stage2muon_bjet1 < 0.5
    isMuon2             = DeltaRmin_stage2muon_bjet2 < 0.5

    if not (isJetMerged) and (passes1 and Etbjet1cut): Plots.getPlot('Unmerged_EtResJets').Fill(EtJetRes1)
    if not (isJetMerged) and (passes2 and Etbjet2cut): Plots.getPlot('Unmerged_EtResJets').Fill(EtJetRes2)

    if (isJetMerged) and (passes1 and passes2 and Etbjet1cut and Etbjet2cut): Plots.getPlot('Merged_EtResJets').Fill(EtJetResM)

    #energy resolution jets
    ## DeltaR<1
    if not (isJetMerged) and (passes1): Plots.getPlot('Unmerged_EtResJets_norm').Fill(EtJetRes1)
    if not (isJetMerged) and (passes2): Plots.getPlot('Unmerged_EtResJets_norm').Fill(EtJetRes2)
    if (isJetMerged) and (passes1 or passes2): Plots.getPlot('Merged_EtResJets_norm').Fill(EtJetResM)

    if (isJetMerged) and (passes1 or passes2): ratioMerged1 += 1 
    if (passes1 or passes2): totEvents1 += 1
    if not (isJetMerged) and (passes1): Plots.getPlot('UnmergedEt_stage2jetVSbjet').Fill(Et_bjet1,Et_L1jet1)
    if not (isJetMerged) and (passes2): Plots.getPlot('UnmergedEt_stage2jetVSbjet').Fill(Et_bjet2,Et_L1jet2)
    if not (isJetMerged) and (passes1) and not (isMuon1): Plots.getPlot('UnmergedEt_stage2jetVSbjet_cleaned').Fill(Et_bjet1,Et_L1jet1)
    if not (isJetMerged) and (passes2) and not (isMuon2): Plots.getPlot('UnmergedEt_stage2jetVSbjet_cleaned').Fill(Et_bjet2,Et_L1jet2)



    ## DeltaR<0.5
    if not (isJetMerged) and (passes1Delta05): Plots.getPlot('Unmerged_EtResJets_norm_05').Fill(EtJetRes1)
    if not (isJetMerged) and (passes2Delta05): Plots.getPlot('Unmerged_EtResJets_norm_05').Fill(EtJetRes2)
    if (isJetMerged) and (passes1Delta05 or passes2Delta05): ratioMerged05 += 1 
    if (passes1Delta05 or passes2Delta05): totEvents05 += 1
    if not (isJetMerged) and (passes1Delta05): Plots.getPlot('DeltaRminMuons05').Fill(DeltaRmin_stage2muon_bjet1)
    if not (isJetMerged) and (passes2Delta05): Plots.getPlot('DeltaRminMuons05').Fill(DeltaRmin_stage2muon_bjet2)
    if not (isJetMerged) and (passes1Delta05): Plots.getPlot('EtaJets05').Fill(Eta_L1jet1)
    if not (isJetMerged) and (passes2Delta05): Plots.getPlot('EtaJets05').Fill(Eta_L1jet2)
    if not (isJetMerged) and (passes1Delta05): Plots.getPlot('EtVSdeltaR_muons05').Fill(DeltaRmin_stage2muon_bjet1,Et_L1muon1)
    if not (isJetMerged) and (passes2Delta05): Plots.getPlot('EtVSdeltaR_muons05').Fill(DeltaRmin_stage2muon_bjet2,Et_L1muon2)

    ## 0.5<DeltaR<1
    if not (isJetMerged) and (passes1Delta05to1): Plots.getPlot('Unmerged_EtResJets_norm_05to1').Fill(EtJetRes1)
    if not (isJetMerged) and (passes2Delta05to1): Plots.getPlot('Unmerged_EtResJets_norm_05to1').Fill(EtJetRes2)
    if (isJetMerged) and (passes1Delta05to1 or passes2Delta05to1): ratioMerged05to1 += 1 
    if (passes1Delta05to1 or passes2Delta05to1): totEvents05to1 += 1
    if not (isJetMerged) and (passes1Delta05to1): Plots.getPlot('DeltaRminMuons05to1').Fill(DeltaRmin_stage2muon_bjet1)
    if not (isJetMerged) and (passes2Delta05to1): Plots.getPlot('DeltaRminMuons05to1').Fill(DeltaRmin_stage2muon_bjet2)
    if not (isJetMerged) and (passes1Delta05to1): Plots.getPlot('EtaJets05to1').Fill(Eta_L1jet1)
    if not (isJetMerged) and (passes2Delta05to1): Plots.getPlot('EtaJets05to1').Fill(Eta_L1jet2)
    if not (isJetMerged) and (passes1Delta05to1): Plots.getPlot('EtVSdeltaR_muons05to1').Fill(DeltaRmin_stage2muon_bjet1,Et_L1muon1)
    if not (isJetMerged) and (passes2Delta05to1): Plots.getPlot('EtVSdeltaR_muons05to1').Fill(DeltaRmin_stage2muon_bjet2,Et_L1muon2)
    

    Plots.getPlot('dib_Dr').Fill(dib_deltaR)
    Plots.getPlot('PtHbb').Fill(bH_pt)
    Plots.getPlot('DeltaRminJets').Fill(DeltaRmin_stage2jet_bjet1)
    Plots.getPlot('DeltaRminJets').Fill(DeltaRmin_stage2jet_bjet2)
    Plots.getPlot('DeltaRminTaus').Fill(DeltaRmin_stage2tau_tau1)
    Plots.getPlot('DeltaRminTaus').Fill(DeltaRmin_stage2tau_tau2)
    if(isTau1): Plots.getPlot('PtTau1').Fill(PtTau1)
    if(isTau1): Plots.getPlot('PtTau2').Fill(PtTau2) # pairType == 2
    
Plots.normPlot('Unmerged_EtResJets_norm',15)
Plots.normPlot('Unmerged_EtResJets_norm_05',15)
Plots.normPlot('Unmerged_EtResJets_norm_05to1',15)
Plots.normPlot('Merged_EtResJets_norm',15)
#Plots.normPlot('EtResTaus_norm')

fIn.Close()

fOut = TFile (FILEOUT, "recreate")

Plots.saveToFile(fOut)
if not fOut.IsZombie(): print ('Plots saved in '+FILEOUT)
Plots.set_palette('normal')

print ('# DeltaR < 1')
print ('Number of entries: {0} Merged jets: {1}'.format(totEvents1, ratioMerged1))
ratioMerged1 = float(ratioMerged1)/totEvents1
print ('Ratio of merged jets: {0:.2f}'.format(ratioMerged1)) 

print ('# DeltaR < 0.5')
print ('Number of entries: {0} Merged jets: {1}'.format(totEvents05, ratioMerged05))
ratioMerged05 = float(ratioMerged05)/totEvents05
print ('Ratio of merged jets: {0:.2f}'.format(ratioMerged05)) 

print ('# 0.5 < DeltaR < 1')
print ('Number of entries: {0} Merged jets: {1}'.format(totEvents05to1, ratioMerged05to1))
ratioMerged05to1 = float(ratioMerged05to1)/totEvents05to1
print ('Ratio of merged jets: {0:.2f}'.format(ratioMerged05to1)) 


