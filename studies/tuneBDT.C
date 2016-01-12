void tuneBDT(){
	TString channel = "ETau";
	TString selection = "defaultBtagLLNoIsoBBTTCut";

	TString dir;
	dir.Form("analysis_%s/evalQCD_Plots_08Dec_FullStat/outPlotter.root",channel.Data());
	TFile * f = TFile::Open(dir.Data());

	TString name;
	name.Form("OS_sig_%sKine_OS_%s_Lambda20",channel.Data(),selection.Data());
	TH1F *hSig = (TH1F*)f->Get(name.Data());
	name.Form("OS_bkg_%sKine_OS_%s_TT",channel.Data(),selection.Data());
	TH1F *hTT = (TH1F*)f->Get(name.Data());

	int nBins = hSig->GetNbinsX();
	TGraph *g = new TGraph(nBins);
	g->SetMarkerStyle(20);

	for(int i=0;i<nBins;i++){
		float sigEff = hSig->Integral(i+1,nBins)/hSig->Integral(1,nBins);
		float bkgEff = hTT->Integral(i+1,nBins)/hTT->Integral(1,nBins);

		g->SetPoint(i,bkgEff,sigEff);
	}

	new TCanvas("c","c",800,800);
	g->Draw("AP");
	g->GetYaxis()->SetTitle("signal Efficiency");
	g->GetXaxis()->SetTitle("TT Bkg Efficiency");

	g->Print();
}