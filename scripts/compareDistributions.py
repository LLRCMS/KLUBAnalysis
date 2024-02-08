import os
import os.path as op
import ROOT

def compareDistributions():
    base = "/data_CMS/cms/alves/HHresonant_hist/{}/combined_outPlots.root"
    channel = "TauTau"
    avars = ["tauH_mass", "bH_mass", "bjet1_JER", "bjet2_JER", "njets"]
    log = False

    #key1 = "{}_Sep20_AllBacks".format(channel)
    key1 = "{}_Oct6".format(channel)
    key2 = "31Jul_{}".format(channel)
    file1 = base.format(key1)
    file2 = base.format(key2)
    print(file1)
    print(file2)
    file1 = ROOT.TFile.Open(file1)
    file2 = ROOT.TFile.Open(file2)

    for key in ('data_obs', 'TT', 'DYmerged'):
        for avar in avars:
            c1 = ROOT.TCanvas('c1_'+key+avar, 'c1_'+key+avar, 600, 600)
            c1.cd()

            hname = "{}_baseline_SR_{}".format(key, avar)
            g1 = file1.Get(hname)
            g2 = file2.Get(hname)

            try:
                g1norm = g1.Clone()
                g1norm.Scale(1./g1.Integral())
                g2norm = g2.Clone()
                g2norm.Scale(1./g2.Integral())
            except ReferenceError:
                print("Key={}, Var={}".format(key, avar))
                raise

            pad1 = ROOT.TPad('pad1'+key+avar, 'pad1'+key+avar, 0, 0.25, 1, 1.0)
            pad1.SetBottomMargin(0.0)
            pad1.Draw()
            pad1.cd()
            if log:
                pad1.SetLogx()
            amax = max(g1norm.GetMaximum(), g2norm.GetMaximum())
            amin = max(g1norm.GetMinimum(), g2norm.GetMinimum())
            g1norm.SetMaximum(amax + 0.2*(amax-amin)) #default value
            g1norm.SetTitle('')
            g1norm.SetLineColor(ROOT.kBlue);
            g1norm.SetLineWidth(2);
            g1norm.SetMarkerColor(ROOT.kBlue);
            g1norm.GetYaxis().SetTitle("a.u.")
            g1norm.GetYaxis().SetTitleSize(20);
            g1norm.GetYaxis().SetTitleFont(43);
            g1norm.GetYaxis().SetTitleOffset(1.45);
            g1norm.Draw("PL")
            
            g2norm.SetLineColor(ROOT.kRed);
            g2norm.SetLineWidth(2);
            g2norm.SetMarkerColor(ROOT.kRed);
            g2norm.Draw("same")
            
            legend = ROOT.TLegend(0.52,0.80,0.89,0.91);
            legend.AddEntry(g1norm, key1 + " (" + key + ")", "lp");
            legend.AddEntry(g2norm, key2 + " (" + key + ")", "lp");
            legend.Draw("same");

            c1.cd()
            pad2 = ROOT.TPad('pad2'+key+avar, 'pad2'+key+avar, 0, 0., 1, 0.25)
            pad2.SetTopMargin(0)
            pad2.SetBottomMargin(0.3)
            pad2.Draw()
            pad2.cd()
            gRatio = g1norm.Clone("ratio"+key+avar)
            gRatio.Divide(g2norm)
            gRatio.SetTitle("")
            gRatio.SetLineColor(ROOT.kBlue)
            gRatio.SetLineWidth(2)
            gRatio.SetMarkerColor(ROOT.kBlue)
            gRatio.GetYaxis().SetTitle("Blue / Red")
            gRatio.GetYaxis().SetTitleSize(20)
            gRatio.GetYaxis().SetTitleFont(43)
            gRatio.GetYaxis().SetTitleOffset(1.45)
            gRatio.GetYaxis().SetLabelFont(43)
            gRatio.GetYaxis().SetLabelSize(15)
            gRatio.GetXaxis().SetTitleSize(20)
            gRatio.GetXaxis().SetTitleFont(43)
            gRatio.GetXaxis().SetTitleOffset(1.6)
            gRatio.GetXaxis().SetLabelFont(43)
            gRatio.GetXaxis().SetLabelSize(15)
            gRatio.SetMinimum(0.71)
            gRatio.SetMaximum(1.29)
            gRatio.Draw("PL")

            c1.Update()
            c1.Draw()
            
            l = ROOT.TLine()
            l.SetLineWidth(2)
            l.SetLineStyle(7)
            l.SetLineColor(ROOT.kBlack)
            dist = (gRatio.GetBinCenter(gRatio.GetNbinsX()+1) - gRatio.GetBinCenter(gRatio.GetNbinsX()))/3
            l.DrawLine(gRatio.GetBinCenter(1), 1.0,
                       gRatio.GetBinCenter(gRatio.GetNbinsX()+1)+dist, 1.0)
            l.Draw("same")

            l1 = ROOT.TLine()
            l1.SetLineWidth(1)
            l1.SetLineStyle(7)
            l1.SetLineColor(ROOT.kBlack)
            dist = (gRatio.GetBinCenter(gRatio.GetNbinsX()+1) - gRatio.GetBinCenter(gRatio.GetNbinsX()))/3
            l1.DrawLine(gRatio.GetBinCenter(1), 1.1,
                        gRatio.GetBinCenter(gRatio.GetNbinsX()+1)+dist, 1.1)
            l1.Draw("same")

            l2 = ROOT.TLine()
            l2.SetLineWidth(1)
            l2.SetLineStyle(7)
            l2.SetLineColor(ROOT.kBlack)
            dist = (gRatio.GetBinCenter(gRatio.GetNbinsX()+1) - gRatio.GetBinCenter(gRatio.GetNbinsX()))/3
            l2.DrawLine(gRatio.GetBinCenter(1), 0.9,
                        gRatio.GetBinCenter(gRatio.GetNbinsX()+1)+dist, 0.9)
            l2.Draw("same")

            c1.SaveAs("compareDistributions_{}_{}.png".format(key, avar))
        
if __name__ == '__main__' :
    ROOT.gStyle.SetOptStat(0)
    ROOT.gROOT.SetBatch(True)
    ROOT.TH1.AddDirectory(0)
    compareDistributions()
