//#include "HTT-utilities/LepEffInterface/interface/ScaleFactor.h"
#include "ScaleFactor.h"

void ScaleFactor::init_ScaleFactor(TString inputRootFile){

	TFile * fileIn = new TFile(inputRootFile, "read");
	// if root file not found
	if (fileIn->IsZombie() ) { std::cout << "ERROR in ScaleFactor::init_ScaleFactor(TString inputRootFile) from NTupleMaker/src/ScaleFactor.cc : ‎File " <<inputRootFile << " does not exist. Please check. " <<std::endl; exit(1); };
	
	std::string HistoBaseName = "ZMass";
	etaBinsH = (TH1D*)fileIn->Get("etaBinsH"); 
	std::string etaLabel, GraphName;
	int nEtaBins = etaBinsH->GetNbinsX();
 	for (int iBin=0; iBin<nEtaBins; iBin++){    
		etaLabel = etaBinsH->GetXaxis()->GetBinLabel(iBin+1);
		GraphName = HistoBaseName+etaLabel+"_Data";
		eff_data[etaLabel] = (TGraphAsymmErrors*)fileIn->Get(TString(GraphName)); 
		SetAxisBins(eff_data[etaLabel]);
		GraphName = HistoBaseName+etaLabel+"_MC";
		eff_mc[etaLabel] = (TGraphAsymmErrors*)fileIn->Get(TString(GraphName));
		SetAxisBins(eff_mc[etaLabel]); 
		bool sameBinning = check_SameBinning(eff_data[etaLabel], eff_mc[etaLabel]);
		if (!sameBinning) {std::cout<< "ERROR in ScaleFactor::init_ScaleFactor(TString inputRootFile) from LepEffInterface/src/ScaleFactor.cc . Can not proceed because ScaleFactor::check_SameBinning returned different pT binning for data and MC for eta label " << etaLabel << std::endl; exit(1); }; 
	}
	
	return;
}

void ScaleFactor::init_ScaleFactor(TString inputRootFile, std::string HistoBaseName){

  TFile * fileIn = new TFile(inputRootFile, "read");
  // if root file not found                                                                                                                                                                          
  if (fileIn->IsZombie() ) { std::cout << "ERROR in ScaleFactor::init_ScaleFactor(TString inputRootFile) from LepEffInterface/src/ScaleFactor.cc : File " <<inputRootFile << " does not exist. Please check. " <<std::endl; exit(1); };

  etaBinsH = (TH1D*)fileIn->Get("etaBinsH");
  std::string etaLabel, GraphName;
  int nEtaBins = etaBinsH->GetNbinsX();
  for (int iBin=0; iBin<nEtaBins; iBin++){
    etaLabel = etaBinsH->GetXaxis()->GetBinLabel(iBin+1);
    GraphName = HistoBaseName+etaLabel+"_Data";
    eff_data[etaLabel] = (TGraphAsymmErrors*)fileIn->Get(TString(GraphName));
    SetAxisBins(eff_data[etaLabel]);
    GraphName = HistoBaseName+etaLabel+"_MC";
    eff_mc[etaLabel] = (TGraphAsymmErrors*)fileIn->Get(TString(GraphName));
    SetAxisBins(eff_mc[etaLabel]);
    bool sameBinning = check_SameBinning(eff_data[etaLabel], eff_mc[etaLabel]);
    if (!sameBinning) {std::cout<< "ERROR in ScaleFactor::init_ScaleFactor(TString inputRootFile) from LepEffInterface/src/ScaleFactor.cc . Can not proceed because ScaleFactor::check_SameBinning returned different pT binning for data and MC for eta label " << etaLabel << std::endl; exit(1); };
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

double ScaleFactor::get_EfficiencyData(double pt, double eta){
	eta = fabs(eta);
	int binNumber = etaBinsH->GetXaxis()->FindFixBin(eta);
	std::string label = etaBinsH->GetXaxis()->GetBinLabel(binNumber);
	std::map<std::string, TGraphAsymmErrors*>::iterator it;
	it =  eff_data.find(label);
	if ( it == eff_data.end()) { 
	std::cout << "ERROR in ScaleFactor::get_EfficiencyData(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc : no object corresponding to eta label "<< label << " for data " << std::endl; exit(1);
	}
	
	// check if pt is overflow or underflow wrt to ptMIN and ptMAX  
	int Npoints = eff_data[label]->GetN();
	double ptMAX = (eff_data[label]->GetX()[Npoints-1])+(eff_data[label]->GetErrorXhigh(Npoints-1));
	double ptMIN = (eff_data[label]->GetX()[0])-(eff_data[label]->GetErrorXlow(0));
	double eff;		

	// if pt is overflow, take eff of highest pt bin 
	if (pt >= ptMAX ) { eff= eff_data[label]->GetY()[Npoints-1];} 
	
	// if pt is underflow, eff=1 and WARNING message
	else if (pt < ptMIN ) {
	  eff=1; 
	  std::cout<< "WARNING in ScaleFactor::get_EfficiencyData(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc: pT too low (pt = " << pt << "), min value is " << ptMIN << ". Returned efficiency =1. Weight will be 1. " << std::endl;
	}
	
	// if pt is in range take corresponding efficiency
	else {
		int ptbin = eff_data[label]->GetXaxis()->FindFixBin(pt);
		eff = eff_data[label]->GetY()[ptbin-1];		
		if (eff > 1.) {std::cout<< "WARNING in ScaleFactor::get_EfficiencyData(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc: Returned efficiency in data > 1. " << std::endl;} 
		if (eff < 0 ) {std::cout<<"WARNING in ScaleFactor::get_EfficiencyData(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc: Returned negative efficiency in data" <<std::endl;}
	}
	return eff;
}


double ScaleFactor::get_EfficiencyMC(double pt, double eta) {
	eta = fabs(eta);
	int binNumber = etaBinsH->GetXaxis()->FindFixBin(eta);
	std::string label = etaBinsH->GetXaxis()->GetBinLabel(binNumber);
	std::map<std::string, TGraphAsymmErrors*>::iterator it;
	it =  eff_mc.find(label);
	if ( it == eff_mc.end()) { 
	std::cout << "ERROR in ScaleFactor::get_EfficiencyMC(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc: no object corresponding to eta label "<< label << " for MC " << std::endl; exit(1);
	}
	
	// check if pt is overflow or underflow wrt to ptMIN and ptMAX  
	int Npoints = eff_mc[label]->GetN();
	double ptMAX = (eff_mc[label]->GetX()[Npoints-1])+(eff_mc[label]->GetErrorXhigh(Npoints-1));
	double ptMIN = (eff_mc[label]->GetX()[0])-(eff_mc[label]->GetErrorXlow(0));
	double eff;		

	// if pt is overflow, take eff of highest pt bin 
	if (pt >= ptMAX ) { eff= eff_mc[label]->GetY()[Npoints-1];} 

	// if pt is underflow, eff=1 and WARNING message
	else if (pt < ptMIN ) {
	  eff=1; 
	  std::cout<< "WARNING in ScaleFactor::get_EfficiencyMC(double pt, double eta) from LepEffInterfce/src/ScaleFactor.cc: pT too low (pt = " << pt << "), min value is " << ptMIN << ". Returned efficiency =1. Weight will be 1. " << std::endl;
	}
	
	// if pt is in range take corresponding efficiency
	else {
		int ptbin = eff_mc[label]->GetXaxis()->FindFixBin(pt);
		eff = eff_mc[label]->GetY()[ptbin-1];
		if (eff > 1. ) {std::cout << "WARNING in ScaleFactor::get_EfficiencyMC(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc : Returned efficiency in MC > 1. " << std::endl;} 		
		if (eff < 0 ) {std::cout<<"WARNING in ScaleFactor::get_EfficiencyMC(double pt, double eta) from LepEffIntrface/src/ScaleFactor.cc : Returned negative efficiency in MC. " <<std::endl;}
	}

	return eff;

}



double ScaleFactor::get_ScaleFactor(double pt, double eta){
	
	double efficiency_data = get_EfficiencyData(pt, eta);
	double efficiency_mc = get_EfficiencyMC(pt, eta);
	double SF;
	if ( efficiency_mc == 0){std::cout << "WARNING in ScaleFactor::get_ScaleFactor(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc : MC efficiency = 0. Scale Factor set to 0. "; SF = 0. ;}
	SF = efficiency_data/efficiency_mc;
	return SF;
	
}


	
	


