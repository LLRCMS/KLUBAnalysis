// https://github.com/CMS-HTT/LeptonEff-interface.git
#include "ScaleFactor.h"

void ScaleFactor::init_EG_ScaleFactor(TString inputRootFile, bool isTriggerSF) {

  TFile * fileIn = new TFile(inputRootFile, "read");
  // if root file not found
  if (fileIn->IsZombie() ) {
    std::cout
      << "ERROR in ScaleFactor::init_EG_ScaleFactor(TString inputRootFile, bool isTriggerSF) from src/ScaleFactor.cc : File "
      << inputRootFile
      << " does not exist. Please check. "
      <<std::endl;
    exit(1);
  }

  TString hsfname = isTriggerSF ? "SF2D":"EGamma_SF2D";
  TH2F *hSF = (TH2F*)fileIn->Get(hsfname);
  TH2F *heffdat = (TH2F*)fileIn->Get(isTriggerSF?"eff_data":"EGamma_SF2D");
  TH2F *heffmc = (TH2F*)fileIn->Get(isTriggerSF?"eff_mc":"EGamma_SF2D");

  // retrieve eta binning (ugly, but should work fine)
  // Trigger SF/eff axes are swapped wrt ID/ISO 
  const int nbin_eta = isTriggerSF? hSF->GetNbinsY():hSF->GetNbinsX();
  TString eta_bins[nbin_eta] = {""};
  // For edges, get upper edge of the first bin / lower edge of the last bin
  TString firstbinlabel = Form("EtaLt%.1f",isTriggerSF?heffdat->GetYaxis()->GetBinLowEdge(2):hSF->GetXaxis()->GetBinLowEdge(2));
  TString lastbinlabel  = Form("EtaGt%.1f",isTriggerSF?heffdat->GetYaxis()->GetBinLowEdge(nbin_eta):hSF->GetXaxis()->GetBinLowEdge(nbin_eta));
  firstbinlabel.ReplaceAll(".","p");
  lastbinlabel.ReplaceAll(".","p");

  //create etabinning Histo
  etaBinsH = new TH1D("etaBinsH","",nbin_eta, isTriggerSF?heffdat->GetYaxis()->GetXbins()->GetArray():hSF->GetXaxis()->GetXbins()->GetArray());

  TString TetaLabel;
  TString GraphName;
  for(int iBin=1; iBin<=nbin_eta; iBin++){
    if (iBin==1) {
      TetaLabel = firstbinlabel;
    }
    else if(iBin==nbin_eta) {
      TetaLabel = lastbinlabel;
    }
    else {
      TetaLabel = Form("Eta%.1fto%.1f",
		       isTriggerSF?heffdat->GetYaxis()->GetBinLowEdge(iBin):hSF->GetXaxis()->GetBinLowEdge(iBin),
		       isTriggerSF?heffdat->GetYaxis()->GetBinLowEdge(iBin+1):hSF->GetXaxis()->GetBinLowEdge(iBin+1));
      TetaLabel.ReplaceAll(".","p");
    }
    etaBinsH->GetXaxis()->SetBinLabel(iBin,TetaLabel);


    std::string etaLabel = (std::string)TetaLabel;
    //GraphName = TString(HistoBaseName)+"_"+etaLabel+"_Data";

    TH1F *hslice_data;
    TH1F *hslice_mc;
    if (!isTriggerSF) {
      hslice_data = (TH1F*)hSF->ProjectionY("slicedata",iBin,iBin);
      hslice_mc = (TH1F*)hSF->ProjectionY("slicedata",iBin,iBin);
    } else {
      hslice_data = (TH1F*)heffdat->ProjectionX("slicedata",iBin,iBin);
      hslice_mc   = (TH1F*)heffmc->ProjectionX("slicedata",iBin,iBin);

    }
    const int nbin_pt = hslice_data->GetNbinsX();

    double data_pt_nom[nbin_pt] = {0.};
    double data_eff_nom[nbin_pt] = {0.};
    double data_pt_errlow[nbin_pt] = {0.};
    double data_pt_errhigh[nbin_pt] = {0.};
    double data_eff_err[nbin_pt] = {0.};
    double mc_pt_nom[nbin_pt] = {0.};
    double mc_eff_nom[nbin_pt] = {0.};
    double mc_pt_errlow[nbin_pt] = {0.};
    double mc_pt_errhigh[nbin_pt] = {0.};
    double mc_eff_err[nbin_pt] = {0.};

    for(int iptbin=0; iptbin<nbin_pt; iptbin++){
      data_pt_nom[iptbin]      = hslice_data->GetXaxis()->GetBinCenter(iptbin+1);
      data_eff_nom[iptbin]     = hslice_data->GetBinContent(iptbin+1);
      data_pt_errlow[iptbin]   = hslice_data->GetXaxis()->GetBinCenter(iptbin+1) - hslice_data->GetXaxis()->GetBinLowEdge(iptbin+1);
      data_pt_errhigh[iptbin]  = hslice_data->GetXaxis()->GetBinUpEdge(iptbin+1) - hslice_data->GetXaxis()->GetBinCenter(iptbin+1);
      data_eff_err[iptbin]     = hslice_data->GetBinError(iptbin+1);
      if(isTriggerSF) {
	mc_pt_nom[iptbin]      = hslice_mc->GetXaxis()->GetBinCenter(iptbin+1);
	mc_eff_nom[iptbin]     = hslice_mc->GetBinContent(iptbin+1);
	mc_pt_errlow[iptbin]   = hslice_mc->GetXaxis()->GetBinCenter(iptbin+1) - hslice_mc->GetXaxis()->GetBinLowEdge(iptbin+1);
	mc_pt_errhigh[iptbin]  = hslice_mc->GetXaxis()->GetBinUpEdge(iptbin+1) - hslice_mc->GetXaxis()->GetBinCenter(iptbin+1);
	mc_eff_err[iptbin]     = hslice_data->GetBinError(iptbin+1);
      }

    }

    eff_data[etaLabel] = new TGraphAsymmErrors(nbin_pt,data_pt_nom, data_eff_nom, data_pt_errlow, data_pt_errhigh, data_eff_err, data_eff_err);
    if(isTriggerSF){
      eff_mc[etaLabel] = new TGraphAsymmErrors(nbin_pt,mc_pt_nom, mc_eff_nom, mc_pt_errlow, mc_pt_errhigh, mc_eff_err, mc_eff_err);
    }
  }
}

void ScaleFactor::init_ScaleFactor(TString inputRootFile, std::string HistoBaseName, bool isHistoFile){

  TFile * fileIn = new TFile(inputRootFile, "read");
  if (fileIn->IsZombie() ) {
	std::cout << "ERROR in ScaleFactor::init_ScaleFactor(TString inputRootFile) from src/ScaleFactor.cc : File " << inputRootFile << " does not exist. Please check. " << std::endl;
	exit(1);
  }

  if(!isHistoFile) { // efficiency file contains TGraphAsymmErrors + eta map (HTT group format for legacy)

    etaBinsH = (TH1D*)fileIn->Get("etaBinsH");
    std::string etaLabel, GraphName;
    int nEtaBins = etaBinsH->GetNbinsX();

    for (int iBin=0; iBin<nEtaBins; iBin++) {
      etaLabel = etaBinsH->GetXaxis()->GetBinLabel(iBin+1);
      GraphName = HistoBaseName+etaLabel+"_Data";
      eff_data[etaLabel] = (TGraphAsymmErrors*)fileIn->Get(TString(GraphName));
      SetAxisBins(eff_data[etaLabel]);
      GraphName = HistoBaseName+etaLabel+"_MC";
      eff_mc[etaLabel] = (TGraphAsymmErrors*)fileIn->Get(TString(GraphName));
      SetAxisBins(eff_mc[etaLabel]);
      bool sameBinning = check_SameBinning(eff_data[etaLabel], eff_mc[etaLabel]);
      if (!sameBinning) {
		std::cout<< "ERROR in ScaleFactor::init_ScaleFactor(TString inputRootFile) from src/ScaleFactor.cc . Can not proceed because ScaleFactor::check_SameBinning returned different pT binning for data and MC for eta label " << etaLabel << std::endl;
		exit(1);
	  }
    }
  }
  else { //efficiency maps in TH2F -> create eta map & TGraphAsymmErrors so that rest of the pipeline isn't disturbed

    // somewhat ugly as applying only to muon SFs
    TString effname_data = "_efficiencyData";
    TString effname_MC = "_efficiencyMC";

    TH2F *heff_data = (TH2F*)fileIn->Get((TString)HistoBaseName+effname_data);
    TH2F *heff_mc   = (TH2F*)fileIn->Get((TString)HistoBaseName+effname_MC);

    // retrieve eta binning (ugly hack, but should work fine)
    const int nbin_eta = heff_data->GetNbinsX();
    TString eta_bins[nbin_eta] = {""};
    TString firstbinlabel = Form("EtaLt%.1f",heff_data->GetXaxis()->GetBinLowEdge(2));
    TString lastbinlabel  = Form("EtaGt%.1f",heff_data->GetXaxis()->GetBinLowEdge(nbin_eta));
    firstbinlabel.ReplaceAll(".","p");
    lastbinlabel.ReplaceAll(".","p");

    //create etabinning Histo
    etaBinsH = new TH1D("etaBinsH", "", nbin_eta,
						heff_data->GetXaxis()->GetXbins()->GetArray());

    TString TetaLabel;
    TString GraphName;
    for(int iBin=1; iBin<=nbin_eta; iBin++) {
      if (iBin==1) {
		TetaLabel = firstbinlabel;
      }
      else if(iBin==nbin_eta) {
		TetaLabel = lastbinlabel;
      }
      else {
		TetaLabel = Form("Eta%.1fto%.1f",
						 heff_data->GetXaxis()->GetBinLowEdge(iBin),
						 heff_data->GetXaxis()->GetBinLowEdge(iBin+1));
		TetaLabel.ReplaceAll(".","p");
      }
      etaBinsH->GetXaxis()->SetBinLabel(iBin,TetaLabel);

      std::string etaLabel = (std::string)TetaLabel;
      GraphName = TString(HistoBaseName)+"_"+etaLabel+"_Data";
      TH1F *hslice_data = (TH1F*)heff_data->ProjectionY("slicedata",iBin,iBin);
      TH1F *hslice_mc   = (TH1F*)heff_mc->ProjectionY("slicemc",iBin,iBin);

      const int nbin_pt = hslice_data->GetNbinsX();

      double data_pt_nom[nbin_pt]      = {0.};
      double data_eff_nom[nbin_pt]     = {0.};
      double data_pt_errlow[nbin_pt]   = {0.};
      double data_pt_errhigh[nbin_pt]  = {0.};
      double data_eff_err[nbin_pt]  = {0.};

      double mc_pt_nom[nbin_pt]      = {0.};
      double mc_eff_nom[nbin_pt]     = {0.};
      double mc_pt_errlow[nbin_pt]   = {0.};
      double mc_pt_errhigh[nbin_pt]  = {0.};
      double mc_eff_err[nbin_pt]  = {0.};

      for(int iptbin=0; iptbin<nbin_pt; iptbin++) {
		data_pt_nom[iptbin]      = hslice_data->GetXaxis()->GetBinCenter(iptbin+1);
		data_eff_nom[iptbin]     = hslice_data->GetBinContent(iptbin+1);
		data_pt_errlow[iptbin]   = hslice_data->GetXaxis()->GetBinCenter(iptbin+1) - hslice_data->GetXaxis()->GetBinLowEdge(iptbin+1);
		data_pt_errhigh[iptbin]  = hslice_data->GetXaxis()->GetBinUpEdge(iptbin+1) - hslice_data->GetXaxis()->GetBinCenter(iptbin+1);
		data_eff_err[iptbin]     = hslice_data->GetBinError(iptbin+1);

		mc_pt_nom[iptbin]      = hslice_mc->GetXaxis()->GetBinCenter(iptbin+1);
		mc_eff_nom[iptbin]     = hslice_mc->GetBinContent(iptbin+1);
		mc_pt_errlow[iptbin]   = hslice_mc->GetXaxis()->GetBinCenter(iptbin+1) - hslice_mc->GetXaxis()->GetBinLowEdge(iptbin+1);
		mc_pt_errhigh[iptbin]  = hslice_mc->GetXaxis()->GetBinUpEdge(iptbin+1) - hslice_mc->GetXaxis()->GetBinCenter(iptbin+1);
		mc_eff_err[iptbin]     = hslice_mc->GetBinError(iptbin+1);
      }
	  
      eff_data[etaLabel] = new TGraphAsymmErrors(nbin_pt,data_pt_nom, data_eff_nom, data_pt_errlow, data_pt_errhigh, data_eff_err, data_eff_err);
      eff_mc[etaLabel]   = new TGraphAsymmErrors(nbin_pt,mc_pt_nom,   mc_eff_nom,   mc_pt_errlow,   mc_pt_errhigh,   mc_eff_err,   mc_eff_err  );
    }
  }

  return;
}


void ScaleFactor::SetAxisBins(TGraphAsymmErrors* graph) {

  int NPOINTS = graph->GetN();
  double AXISBINS [NPOINTS+1] = {};
  for (int i=0; i<NPOINTS; i++) { AXISBINS[i] = (graph->GetX()[i] - graph->GetErrorXlow(i)); }
  AXISBINS[NPOINTS] = (graph->GetX()[NPOINTS-1] + graph->GetErrorXhigh(NPOINTS-1));
  graph->GetXaxis()->Set(NPOINTS, AXISBINS);
  return;
}

bool ScaleFactor::check_SameBinning(TGraphAsymmErrors* graph1, TGraphAsymmErrors* graph2){
  bool haveSameBins = false;
  int n1 = graph1->GetXaxis()->GetNbins();
  int n2 = graph2->GetXaxis()->GetNbins();
  if (n1 != n2 ) {return false;}
  else {
    haveSameBins = true;
    const int nbins = n1;
    double x1, x2;
    for (int i=0; i<nbins; i++){
      x1 = (graph1->GetXaxis()->GetXbins())->GetArray()[i];
      x2 = (graph2->GetXaxis()->GetXbins())->GetArray()[i];
      haveSameBins = haveSameBins and (x1== x2) ;
    }
  }

  return haveSameBins;
}


std::string ScaleFactor::FindEtaLabel(double Eta, std::string Which){

  // Eta = fabs(Eta);
  int binNumber = etaBinsH->GetXaxis()->FindFixBin(Eta);
  std::string EtaLabel = etaBinsH->GetXaxis()->GetBinLabel(binNumber);

  std::map<std::string, TGraphAsymmErrors*>::iterator it;
  if (Which == "data"){
    it =  eff_data.find(EtaLabel);
    if ( it == eff_data.end()) {
      std::cout << "ERROR in ScaleFactor::get_EfficiencyData(double pt, double eta, int pType) from src/ScaleFactor.cc : no object corresponding to eta label "<< EtaLabel << " for data " << std::endl; exit(1);
    }
  }

  else if (Which == "mc"){
    it = eff_mc.find(EtaLabel);
    if (it == eff_mc.end()) {
      std::cout << "ERROR in ScaleFactor::get_EfficiencyMC(double pt, double eta) from src/ScaleFactor.cc : no object corresponding to eta label "<< EtaLabel << " for MC " << std::endl; exit(1);
    }
  }

  return EtaLabel;
}


int ScaleFactor::FindPtBin(std::map<std::string, TGraphAsymmErrors*> eff_map,
						   std::string EtaLabel, double Pt, double Eta, int pType)
{
  int Npoints = eff_map[EtaLabel]->GetN();
  double ptMAX = eff_map[EtaLabel]->GetX()[Npoints-1] + eff_map[EtaLabel]->GetErrorXhigh(Npoints-1);
  double ptMIN = eff_map[EtaLabel]->GetX()[0] - eff_map[EtaLabel]->GetErrorXlow(0);

  if (Pt >= ptMAX) { // if pt is overflow, return last pt bin
	return Npoints;
  }
  else if (Pt < ptMIN) { // if pt is underflow, return nonsense number and warning
    std::cout << "WARNING in ScaleFactor::get_EfficiencyData(double pt, double eta, int pType) from src/ScaleFactor.cc: "
			  << "pT too low (pt=" << Pt << ", "
			  << "eta=" << Eta << "), min value is " << ptMIN
			  << " (pType = " << pType << "). "
			  << "Returned efficiency = 1. Weight will be 1. " << std::endl;
    return -99;
  }
  else { // if pt is in range
    for (int graphBin=0; graphBin < Npoints; graphBin++) {
      if (Pt >= eff_map[EtaLabel]->GetPointX(graphBin) - eff_map[EtaLabel]->GetErrorXlow(graphBin)
		  && Pt < eff_map[EtaLabel]->GetPointX(graphBin) + eff_map[EtaLabel]->GetErrorXhigh(graphBin))
		return graphBin+1;
    }
  }
  
  return -99; //should never arrive to this point
}


double ScaleFactor::get_EfficiencyData(double pt, double eta, int pType){

  double eff;
  std::string label = FindEtaLabel(eta, "data");

  int ptbin = FindPtBin(eff_data, label, pt, eta, pType);
  if (ptbin == -99){eff =1;} // if pt is underflow
  else eff = eff_data[label]->GetY()[ptbin-1];

  if (eff > 1.) {std::cout<< "WARNING in ScaleFactor::get_EfficiencyData(double pt, double eta, int pType) from src/ScaleFactor.cc: Efficiency in data > 1. Set eff = 1." << std::endl; eff=1;}
  if (eff < 0 ) {std::cout<<"WARNING in ScaleFactor::get_EfficiencyData(double pt, double eta, int pType) from src/ScaleFactor.cc: Negative efficiency in data. Set eff = 0." <<std::endl; eff=0;}

  return eff;

}


double ScaleFactor::get_EfficiencyMC(double pt, double eta, int pType) {

  double eff;
  std::string label = FindEtaLabel(eta, "mc");
  int ptbin = FindPtBin(eff_mc, label, pt, eta, pType);
  if (ptbin == -99){eff =1;} // if pt is underflow
  else eff= eff_mc[label]->GetY()[ptbin-1];

  if (eff > 1. ) {std::cout << "WARNING in ScaleFactor::get_EfficiencyMC(double pt, double eta, int pType) from src/ScaleFactor.cc : Efficiency in MC > 1. Set eff = 1." << std::endl; eff =1.;}
  if (eff < 0. ) {std::cout<<"WARNING in ScaleFactor::get_EfficiencyMC(double pt, double eta, int pType) from src/ScaleFactor.cc : Negative efficiency in MC. Set eff = 0." <<std::endl; eff =0.;}

  return eff;

}

double ScaleFactor::get_ScaleFactor(double efficiency_data, double efficiency_mc) {

  double SF;

  if ( efficiency_mc != 0) {SF = efficiency_data/efficiency_mc;}
  else {
    SF=1.; std::cout << "WARNING in ScaleFactor::get_ScaleFactor(double efficiency_data, double efficiency_mc) from src/ScaleFactor.cc : MC efficiency = 0. Scale Factor set to 1. ";
  }

  return SF;

}

double ScaleFactor::get_ScaleFactor(double pt, double eta, int pType) {

  double efficiency_data = get_EfficiencyData(pt, eta, pType);
  double efficiency_mc = get_EfficiencyMC(pt, eta, pType);

  return get_ScaleFactor(efficiency_data, efficiency_mc);
}
double ScaleFactor::get_direct_ScaleFactor(double pt, double eta, int pType){

  std::string label = FindEtaLabel(eta, "data");
  int ptbin = FindPtBin(eff_data, label, pt, eta, pType); // when available, SF stored in eff_data (lazy implementation that should be improved)
  double SF;

  if (ptbin == -99){SF =1;} // if pt is underflow
  else SF= eff_data[label]->GetY()[ptbin-1];

  return SF;

}


double ScaleFactor::get_EfficiencyDataError(double pt, double eta, int pType) {

  double eff_error;
  std::string label = FindEtaLabel(eta, "data");
  int ptbin = FindPtBin(eff_data, label, pt, eta, pType);
  if (ptbin == -99){eff_error =0.;} // if pt is underflow
  else eff_error= eff_data[label]->GetErrorYhigh(ptbin-1);
  // errors are supposed to be symmetric, can use GetErrorYhigh or GetErrorYlow

  double effData = get_EfficiencyData(pt, eta, pType);
  if (eff_error > effData) eff_error = 0.5*effData;
  return eff_error;
}



double ScaleFactor::get_EfficiencyMCError(double pt, double eta, int pType) {

  double eff_error;
  std::string label = FindEtaLabel(eta,"mc");
  int ptbin = FindPtBin(eff_mc, label, pt, eta, pType);
  if (ptbin == -99){eff_error =0.;} // if pt is underflow
  else eff_error= eff_mc[label]->GetErrorYhigh(ptbin-1);
  // errors are supposed to be symmetric, can use GetErrorYhigh or GetErrorYlow

  double effMC = get_EfficiencyMC(pt, eta, pType);
  if (eff_error > effMC ) eff_error = 0.5*effMC;
  return eff_error;
}

double ScaleFactor::get_ScaleFactorError(double effData, double effMC, double errData, double errMC) {

  double SF_error = 0.;

  if (errData==0. and errMC==0.) {
    std::cout<<"WARNING in ScaleFactor::get_ScaleFactorError(double effData, double effMC, double errData, double errMC) from src/ScaleFactor.cc: uncertainty on data and MC = 0, can not calculate uncertainty on scale factor. Uncertainty set to 0." << std::endl;
  }

  if (effData==0. or effMC==0.) {
    std::cout<<"WARNING in ScaleFactor::get_ScaleFactorError(double effData, double effMC, double errData, double errMC) from src/ScaleFactor.cc: efficiency in data or MC = 0, can not calculate uncertainty on scale factor. Uncertainty set to 0." << std::endl;
    return 0.;
  }
  else {
    SF_error = pow((errData/effData),2) + pow((errMC/effMC),2);
    SF_error = pow(SF_error, 0.5)*(effData/effMC);
  }
  return SF_error;
}

double ScaleFactor::get_ScaleFactorError(double pt, double eta, int pType) {

  double effData = get_EfficiencyData(pt, eta, pType);
  double effMC   = get_EfficiencyMC(pt, eta, pType);
  double errData = get_EfficiencyDataError(pt, eta, pType);
  double errMC   = get_EfficiencyMCError(pt, eta, pType);

  return get_ScaleFactorError(effData, effMC, errData, errMC);
}
