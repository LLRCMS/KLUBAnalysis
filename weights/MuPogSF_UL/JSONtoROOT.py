# ----------------------------------------
# Utility code kindly shared by G.Liu
# Formats muon trigger SF files provided by MUO POG into ROOT format
# ----------------------------------------

import json
import numpy as np
import ROOT as rt

class TrgSF:
    def __init__(self,name):
        self.file_name=name
        self.hist_name={}
        self.data={}
        self.nInputs={}
        self.nScheme=0
        self.__ReadJSON()
        
    def __ReadJSON(self):
        with open(self.file_name) as f:
            a=json.load(f)['corrections']
            self.nScheme=len(a)
            for iS in range(self.nScheme):
                self.hist_name[iS]=a[iS]['name'].encode('utf-8')
                self.nInputs[iS]=len(a[iS]['inputs'])-1
                self.data[iS]=a[iS]
            
    def __GetHist2D(self,i):
        data=self.data[i]
        var1=data['inputs'][0]['name'].encode('utf-8')
        var2=data['inputs'][1]['name'].encode('utf-8')
        data=data['data']
        bins1=np.array(data['edges'])
        bins2=np.array(data['content'][0]['edges'])
        nbin1=len(bins1)-1
        nbin2=len(bins2)-1
        hist_name=self.hist_name[i]+"_"+var1+"_"+var2
        hist=rt.TH2F(hist_name,";|#eta|;p_{T} (GeV)",nbin1,bins1,nbin2,bins2)
        hist_stat=rt.TH2F(hist_name+"_stat",";|#eta|;p_{T} (GeV)",nbin1,bins1,nbin2,bins2)
        hist_syst=rt.TH2F(hist_name+"_syst",";|#eta|;p_{T} (GeV)",nbin1,bins1,nbin2,bins2)
        for i1 in range(nbin1):
            for i2 in range(nbin2):
                #          eta bin           pt bin           SF info
                # data ['content'][i1]  ['content'][i2]  ['content'][x]  ['value']
                # SF info:
                #  0 -> "AltSig": syst. uncertainty from signal modeling
                #  1 -> massBin: syst. uncertainty from mass binning size
                #  2 -> massRange: syst. uncertainty from mass window size
                #  3 -> stat: stat. uncertainty
                #  4 -> syst: total syst. uncertainty
                #  5 -> nominal: Nominal SF value
                nom=data['content'][i1]['content'][i2]['content'][5]['value']
                stat=data['content'][i1]['content'][i2]['content'][3]['value']
                syst=data['content'][i1]['content'][i2]['content'][4]['value']
                hist.SetBinContent(i1+1,i2+1,nom)
                hist_stat.SetBinContent(i1+1,i2+1,nom)
                hist_stat.SetBinError(i1+1,i2+1,stat)
                hist_syst.SetBinContent(i1+1,i2+1,nom)
                hist_syst.SetBinError(i1+1,i2+1,syst)
        return hist,hist_stat,hist_syst
        
    def __GetHist3D(self,i):
        data=self.data[i]
        var1=data['inputs'][0]['name'].encode('utf-8')
        var2=data['inputs'][1]['name'].encode('utf-8')
        var3=data['inputs'][2]['name'].encode('utf-8')
        data=data['data']
        bins1=np.array(data['edges'])
        bins2=np.array(data['content'][0]['edges'])
        bins3=np.array(data['content'][0]['content'][0]['edges'])
        nbin1=len(bins1)-1
        nbin2=len(bins2)-1
        nbin3=len(bins3)-1
        hist_name=self.hist_name[i]+"_"+var1+"_"+var2+"_"+var3
        hist=rt.TH3F(hist_name,"charge;|#eta|;p_{T} (GeV)",nbin1,bins1,nbin2,bins2,nbin3,bins3)
        hist_stat=rt.TH3F(hist_name+"_stat",";charge;|#eta|;p_{T} (GeV)",nbin1,bins1,nbin2,bins2,nbin3,bins3)
        hist_syst=rt.TH3F(hist_name+"_syst",";charge;|#eta|;p_{T} (GeV)",nbin1,bins1,nbin2,bins2,nbin3,bins3)
        for i1 in range(nbin1):
            for i2 in range(nbin2):
                for i3 in range(nbin3):
                    nom=data['content'][i1]['content'][i2]['content'][i3]['content'][5]['value']
                    stat=data['content'][i1]['content'][i2]['content'][i3]['content'][3]['value']
                    syst=data['content'][i1]['content'][i2]['content'][i3]['content'][4]['value']
                    hist.SetBinContent(i1+1,i2+1,i3+1,nom)
                    hist_stat.SetBinContent(i1+1,i2+1,i3+1,nom)
                    hist_stat.SetBinError(i1+1,i2+1,i3+1,stat)
                    hist_syst.SetBinContent(i1+1,i2+1,i3+1,nom)
                    hist_syst.SetBinError(i1+1,i2+1,i3+1,syst)
        return hist,hist_stat,hist_syst
        
    def JSONtoROOT(self):
        hist={}
        hist_stat={}
        hist_syst={}
        for iS in range(self.nScheme):
            if self.nInputs[iS]==2:
                hist[iS],hist_stat[iS],hist_syst[iS]=self.__GetHist2D(iS)
            elif self.nInputs[iS]==3:
                hist[iS],hist_stat[iS],hist_syst[iS]=self.__GetHist3D(iS)
            else:
                print("Number of inputs {0} not correct!!!".format(self.nInputs[iS]))
                return
        
        f_out=rt.TFile(self.file_name.replace("json","root"),"recreate")
        for iS in range(self.nScheme):
            hist[iS].Write()
            hist_stat[iS].Write()
            hist_syst[iS].Write()
        f_out.Write()
        f_out.Close()

if __name__ == '__main__':

    inputs = [
            "Efficiencies_muon_generalTracks_Z_Run2016_UL_HIPM_SingleMuonTriggers_schemaV2.json",
            "Efficiencies_muon_generalTracks_Z_Run2016_UL_SingleMuonTriggers_schemaV2.json",
            "Efficiencies_muon_generalTracks_Z_Run2017_UL_SingleMuonTriggers_schemaV2.json",
            "Efficiencies_muon_generalTracks_Z_Run2018_UL_SingleMuonTriggers_schemaV2.json"
    ]
    
    for f in inputs:
        trgSF=TrgSF("./"+f)
        trgSF.JSONtoROOT()
