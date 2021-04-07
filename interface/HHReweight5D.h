/*
** class  : HHReweight5D
** author : L. Cadamuro (LLR)
** date   : 17 Feb 2017
** brief  : Compute a reweight coefficient to transform an input distribution into a general point in kl, kt plane. 31/05/2017: Updated to 5D reweight
** Updated by D. Zuolo (MIB) and J. Motta (LLR) for Run 2 analisys - Apr 2021
*/ 

#ifndef HHREWEIGHT_H
#define HHREWEIGHT_H

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "TAxis.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2D.h"
#include "TMath.h"

class HHReweight5D{
    
    public:
        HHReweight5D(std::string coeffFile, const TH2* hInput, std::string EFTBMname, std::string year, bool cms_fake = false, bool useAbsEta=true);
        HHReweight5D(std::string coeffFile, const TH2* hInput, bool useAbsEta=true);
        ~HHReweight5D();
        double getWeight(double mhh, double cth);
        double getWeight(double kl, double kt, double c2, double cg, double c2g, double mhh, double cth);        

    private:
        void readInputFile(std::string coeffFile);
        double functionGF(double kl, double kt, double c2, double cg, double c2g, std::vector<double> A);

        //Compute Total and Differential XS
        double getTotXS(double kl, double kt, double c2, double cg, double c2g);
        double getDiffXS(double kl, double kt, double c2, double cg, double c2g, double mhh, double cth, std::map< std::pair<double, double>, std::vector<double> > A_map);
        
        std::vector<std::string> tokenize(std::string input);
        
        // the distribution of the input events
        // must the all the generated events before preselections
        std::shared_ptr<TH2> h_input_;
        bool useAbsEta_;
    
        // coefficients XS SM
        std::vector<double> A_13TeV_;

        // maps containing: binMhh, binCoshh, A_coefs(_UP/_DN)
        std::map< std::pair<double, double>, std::vector<double> >A_map_;
        std::map< std::pair<double, double>, std::vector<double> >A_map_UP_;
        std::map< std::pair<double, double>, std::vector<double> >A_map_DN_;

        // EFT benchmark couplings values
        std::vector<double> EFTBMcouplings_;
};

#endif

