/*
** class  : HHReweight5D
** author : L. Cadamuro (LLR)
** date   : 17 Feb 2017
** brief  : Compute a reweight coefficient to transform an input distribution into a general point in kl, kt plane. 31/05/2017: Updated to 5D reweight
*/ 


#ifndef HHREWEIGHT_H
#define HHREWEIGHT_H

#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <utility>

#include "TH1.h"
#include "TH2.h"
#include "TAxis.h"
#include "TFile.h"

#define NCOEFFSA 15

class HHReweight5D{
    
    public:
        HHReweight5D(std::string coeffFile, const TH2* hInput, bool useAbsEta=true);
        ~HHReweight5D();
        double getWeight(double kl, double kt, double c2, double cg, double c2g, double mhh, double cth);
        double getWeight(double kl, double kt, double mhh, double cth); // kl, kt only
        
    private:
        void readInputFile(std::string coeffFile);
        // double functionGF(double kl, double kt, double c2, double cg, double c2g, double A1, double A3, double A7);
        double functionGF(double kl, double kt, double c2, double cg, double c2g, std::array<double, NCOEFFSA> const &A);
        std::vector<std::string> tokenize(std::string input);
        std::pair<int,int> find2DBin(TH2* h, double x, double y);
        
        // adapted from ROOT to check histogram consistency
        bool CheckConsistency(const TH1* h1, const TH1* h2);
        bool CheckAxisLimits(const TAxis *a1, const TAxis *a2 );
        bool CheckBinLimits(const TAxis* a1, const TAxis * a2);
        
        // the coefficients of the reweight - read from the input file
        std::shared_ptr<TH2> h_SM_;
        std::shared_ptr<TH2> h_sumv1_;

        std::array<std::shared_ptr<TH2>, NCOEFFSA> h_A_vec_;

        // double A1_13TeV_;
        // double A3_13TeV_;
        // double A7_13TeV_;

        std::array<double, NCOEFFSA> A_13TeV_;

        // the distribution of the input events
        // must the all the generated events before preselections
        std::shared_ptr<TH2> h_input_;
        bool useAbsEta_;
};

#endif