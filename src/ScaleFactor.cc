// https://github.com/CMS-HTT/LeptonEff-interface.git
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

		if (fileIn->GetListOfKeys()->Contains(TString(GraphName))){
			eff_data[etaLabel] = (TGraphAsymmErrors*)fileIn->Get(TString(GraphName)); 
			SetAxisBins(eff_data[etaLabel]);
		}
		else eff_data[etaLabel] = 0;

		GraphName = HistoBaseName+etaLabel+"_MC";
		if (fileIn->GetListOfKeys()->Contains(TString(GraphName))){
			eff_mc[etaLabel] = (TGraphAsymmErrors*)fileIn->Get(TString(GraphName));
			SetAxisBins(eff_mc[etaLabel]); 
		}
		else eff_mc[etaLabel] =0;

		if (eff_data[etaLabel] != 0 && eff_mc[etaLabel] != 0 ) {
			bool sameBinning = check_SameBinning(eff_data[etaLabel], eff_mc[etaLabel]);
			if (!sameBinning) {std::cout<< "ERROR in ScaleFactor::init_ScaleFactor(TString inputRootFile) from LepEffInterface/src/ScaleFactor.cc . Can not proceed because ScaleFactor::check_SameBinning returned different pT binning for data and MC for eta label " << etaLabel << std::endl; exit(1); }; 
		}
	}
	
	return;
}

///FIX ME: rewrite similarly to init_ScaleFactor(TString inputRootFile) to allow only data or mc in root file
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


std::string ScaleFactor::FindEtaLabel(double Eta, std::string Which){

	Eta = fabs(Eta);
	int binNumber = etaBinsH->GetXaxis()->FindFixBin(Eta);
	std::string EtaLabel = etaBinsH->GetXaxis()->GetBinLabel(binNumber);
	std::map<std::string, TGraphAsymmErrors*>::iterator it;
	if (Which == "data"){
		it =  eff_data.find(EtaLabel);
		if ( it == eff_data.end()) { 
		std::cout << "ERROR in ScaleFactor::get_EfficiencyData(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc : no object corresponding to eta label "<< EtaLabel << " for data " << std::endl; exit(1);
		}
	}

	else if (Which == "mc"){
		it = eff_mc.find(EtaLabel);
		if (it == eff_mc.end()) { 
		std::cout << "ERROR in ScaleFactor::get_EfficiencyMC(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc : no object corresponding to eta label "<< EtaLabel << " for MC " << std::endl; exit(1);
		}		
	}
	
     return EtaLabel;
}


int ScaleFactor::FindPtBin( std::map<std::string, TGraphAsymmErrors *> eff_map, std::string EtaLabel, double Pt){

    int Npoints = eff_map[EtaLabel]->GetN();
	double ptMAX = (eff_map[EtaLabel]->GetX()[Npoints-1])+(eff_map[EtaLabel]->GetErrorXhigh(Npoints-1));
	double ptMIN = (eff_map[EtaLabel]->GetX()[0])-(eff_map[EtaLabel]->GetErrorXlow(0));
	// if pt is overflow, return last pt bin
 	if (Pt >= ptMAX ) return Npoints; 
	// if pt is underflow, return nonsense number and warning
	else if (Pt < ptMIN){ 
 	std::cout<< "WARNING in ScaleFactor::get_EfficiencyData(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc: pT too low (pt = " << Pt << "), min value is " << ptMIN << ". Returned efficiency =1. Weight will be 1. " << std::endl;
	return -99;}
	// if pt is in range
	else {return eff_map[EtaLabel]->GetXaxis()->FindFixBin(Pt);} 
	}


double ScaleFactor::get_EfficiencyData(double pt, double eta){

        double eff;
	std::string label = FindEtaLabel(eta,"data");

	int ptbin = FindPtBin(eff_data, label, pt); 
	if (ptbin == -99){eff =1;} // if pt is underflow 
	else eff = eff_data[label]->GetY()[ptbin-1];

	if (eff > 1.) {std::cout<< "WARNING in ScaleFactor::get_EfficiencyData(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc: Efficiency in data > 1. Set eff = 1." << std::endl; eff=1;} 
	if (eff < 0 ) {std::cout<<"WARNING in ScaleFactor::get_EfficiencyData(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc: Negative efficiency in data. Set eff = 0." <<std::endl; eff=0;}

	return eff;
	
}


double ScaleFactor::get_EfficiencyMC(double pt, double eta) {

	double eff;		
	std::string label = FindEtaLabel(eta,"mc");
	int ptbin = FindPtBin(eff_mc, label, pt); 
	if (ptbin == -99){eff =1;} // if pt is underflow 
	else eff= eff_mc[label]->GetY()[ptbin-1];

	if (eff > 1. ) {std::cout << "WARNING in ScaleFactor::get_EfficiencyMC(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc : Efficiency in MC > 1. Set eff = 1." << std::endl; eff =1;} 		
	if (eff < 0 ) {std::cout<<"WARNING in ScaleFactor::get_EfficiencyMC(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc : Negative efficiency in MC. Set eff = 0." <<std::endl; eff =0;}

	return eff;

}



double ScaleFactor::get_ScaleFactor(double pt, double eta){
	
	double efficiency_data = get_EfficiencyData(pt, eta);
	double efficiency_mc = get_EfficiencyMC(pt, eta);
	double SF;

	if ( efficiency_mc != 0) {SF = efficiency_data/efficiency_mc;}
	else {
	SF=1.; std::cout << "WARNING in ScaleFactor::get_ScaleFactor(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc : MC efficiency = 0. Scale Factor set to 1. ";
	}

	return SF;	
	
}


double ScaleFactor::get_EfficiencyDataError(double pt, double eta){

	double eff_error;
	std::string label = FindEtaLabel(eta,"data");
	int ptbin = FindPtBin(eff_data, label, pt); 
	if (ptbin == -99){eff_error =0.;} // if pt is underflow 
	else eff_error= eff_data[label]->GetErrorYhigh(ptbin-1); 
        // errors are supposed to be symmetric, can use GetErrorYhigh or GetErrorYlow

	double effData = get_EfficiencyData(pt,eta);
	if (eff_error > effData) eff_error = 0.5*effData;
	return eff_error;
}
	
	

double ScaleFactor::get_EfficiencyMCError(double pt, double eta){

	double eff_error;
	std::string label = FindEtaLabel(eta,"mc");
	int ptbin = FindPtBin(eff_mc, label, pt); 
	if (ptbin == -99){eff_error =0.;} // if pt is underflow 
	else eff_error= eff_mc[label]->GetErrorYhigh(ptbin-1); 
	// errors are supposed to be symmetric, can use GetErrorYhigh or GetErrorYlow

	double effMC = get_EfficiencyMC(pt,eta);
	if (eff_error > effMC ) eff_error = 0.5*effMC;
	return eff_error;
}

double ScaleFactor::get_ScaleFactorError(double pt, double eta){

	double SF_error = 0.;
	
	double effData = get_EfficiencyData(pt, eta);
	double effMC = get_EfficiencyMC(pt, eta);
	double errData = get_EfficiencyDataError(pt, eta);
	double errMC =  get_EfficiencyMCError(pt, eta);

	if (errData == 0) {std::cout<<"WARNING in ScaleFactor::get_ScaleFactorError(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc: uncertainty on data point = 0, can not calculate uncertainty on scale factor. Uncertainty set to 0." << std::endl;}
	if (errMC ==0) {std::cout<<"WARNING in ScaleFactor::get_ScaleFactorError(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc: uncertainty on MC = 0, can not calculate uncerttainty on scale factor. Uncertainty set to 0." << std::endl;}
	if (effData ==0) {std::cout<<"WARNING in ScaleFactor::get_ScaleFactorError(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc: efficiency in data = 0, can not calculate uncertainty on scale factor. Uncertainty set to 0." << std::endl;}
	if (effMC ==0) {std::cout<<"WARNING in ScaleFactor::get_ScaleFactorError(double pt, double eta) from LepEffInterface/src/ScaleFactor.cc: efficiency in MC = 0, can not calculate uncertainty on scale factor. Uncertainty set to 0." << std::endl;}
	else {	
	SF_error = pow((errData/effData),2) + pow((errMC/effMC),2);
	SF_error = pow(SF_error, 0.5)*(effData/effMC);
	}
	return SF_error;
}
	

