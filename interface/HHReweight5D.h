/*
** class  : HHReweight5D
** author : L. Cadamuro (LLR)
** date   : 17 Feb 2017
** brief  : Compute a reweight coefficient to transform an input distribution into a general point in kl, kt plane. 31/05/2017: Updated to 5D reweight
** Updated by D. Zuolo (MIB) and J. Motta (LLR) for Run 2 analisys - Apr 2021
**      - the update is based on the approach explained in https://gitlab.cern.ch/hh/eft-benchmarks
**      - the current version of this class allows to get the HHweight for specific EFT benchmarks, c2scans, or manually set couplings
**      - the current version of this class allows to do the reweight both at LO and NLO
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
        HHReweight5D(std::string coeffFile, const TH2* hInput, std::string EFTBMname, std::string year, std::string order, std::string uncertantie = "", bool cms_fake = false, bool useAbsEta=true);
        ~HHReweight5D();
        double getWeight(double mhh, double cth);
        double getWeight(double mhh, double cth, double c2);
        double getWeight(double mhh, double cth, double kl, double kt, double c2, double cg, double c2g);        

    private:
        void readInputFile(std::string coeffFile);
        double functionGF(double kl, double kt, double c2, double cg, double c2g, std::vector<double> A);

        //Compute Total and Differential XS
        double getTotXS(double kl, double kt, double c2, double cg, double c2g);
        double getDiffXS(double kl, double kt, double c2, double cg, double c2g, double mhh, double cth, std::vector< std::vector<double> > A_map);
        
        std::vector<std::string> tokenize(std::string input);
        
        // the distribution of the input events
        // must the all the generated events before preselections
        std::shared_ptr<TH2> h_input_;
        
        // take costheta absolute value
        bool useAbsEta_;
        
        // order of the reweighting LO or NLO
        std::string order_;

        // uncertaintie that can be used:
        // LO
        //      "muR_UP"
        //      "muR_DN"
        //      "muF_UP"
        //      "muF_DN"
        //      "muRF_UP"
        //      "muRF_DN"
        //      "PDF4LHC15_nlo_30_pdfas_UP"
        //      "PDF4LHC15_nlo_30_pdfas_DN"
        //      "PDF4LHC15_nlo_30_pdfas_as_UP"
        //      "PDF4LHC15_nlo_30_pdfas_as_DN"
        // NLO
        //      "fit_UP"
        //      "fit_DN"
        std:: string unc_;
    
        // coefficients XS SM
        std::vector<double> A_13TeV_nlo_;
        std::vector<double> A_13TeV_lo_;

        // map containing: binMhh, binCoshh, A_coefs(_UP/_DN)
        std::vector< std::vector<double> > A_map_;

        // EFT benchmark couplings values
        //      sm, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12
        //      1b, 2b, 3b, 4b, 5b, 6b, 7b
        //      8a
        //      box, cHHH0, cHHH1, cHHH2, cHHH5
        std::vector<double> EFTBMcouplings_;
};

#endif

