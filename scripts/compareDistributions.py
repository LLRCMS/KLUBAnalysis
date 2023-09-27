import os
import os.path as op
import ROOT

def compareDistributions():
    base = "/data_CMS/cms/alves/HHresonant_hist/{}/combined_outPlots.root"
    channel = "TauTau"
    var = "HHKin_mass"

    #key1 = "{}_Sep20_AllBacks".format(channel)
    key1 = "{}_Sep23".format(channel)
    key2 = "{}_Sep22_Data_Tau".format(channel)
    file1 = ROOT.TFile.Open(base.format(key1))
    file2 = ROOT.TFile.Open(base.format(key2))

    for key in ('data_obs', 'W', 'TT', 'DYmerged'):
        hname = "{}_baseline_SR_{}".format(key, var)
        g1 = file1.Get(hname)
        g2 = file2.Get(hname)

        c1 = ROOT.TCanvas('c1_'+key, 'c1_'+key, 600, 600)
        c1.cd()

        g1.SetTitle('')
        g1.SetLineColor(ROOT.kBlue);
        g1.SetMarkerColor(ROOT.kBlue);
        g1.Draw("PL")
        g2.SetLineColor(ROOT.kRed);
        g2.SetMarkerColor(ROOT.kRed);
        g2.Draw("same")

        legend = ROOT.TLegend(0.52,0.80,0.89,0.91);
        legend.AddEntry(g1, key1, "l");
        legend.AddEntry(g2, key2, "l");
        legend.Draw("same");

        c1.SaveAs("compareDistributions_{}_{}.png".format(key, var))
        
if __name__ == '__main__' :
    ROOT.gStyle.SetOptStat(0)
    ROOT.gROOT.SetBatch(True)
    ROOT.TH1.AddDirectory(0)
    compareDistributions()
