/*
    ** class: Sample
    ** author: L. Cadamuro (LLR)
    ** date: 26/05/2016
    ** description: class representing a data sample (either data or MC). Handles file opening and histograms.
    **              input files are listed in a txt file "filename".
*/

#ifndef SAMPLE_H
#define SAMPLE_H

#include <memory>
#include <string>
#include <vector>
#include "Weight.h"
#include "ordered_map.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"

class Sample
{
    public:
        // hierarchy of objects [selection[variable[systematics]]] 
        typedef ordered_map<std::string, std::shared_ptr<TH1F> >  systColl;  // histograms stored - map is for systematics
        typedef ordered_map<std::string, systColl>                varColl;   // variables plotted
        typedef ordered_map<std::string, varColl>                 selColl;   // all selections

        // note: for 2D histos, a 2D plot is still associated to a "single" variable, eg, if I want to plot A vs B, var2D = A:B (as in TTree)
        typedef ordered_map<std::string, std::shared_ptr<TH2F> >  systColl2D;  // histograms stored - map is for systematics
        typedef ordered_map<std::string, systColl2D>              varColl2D;   // variables plotted
        typedef ordered_map<std::string, varColl2D>               selColl2D;   // all selections

        enum sType {
            kData = 0,
            kSig  = 1,
            kBkg  = 2
        };

        // standard ctor/dtor
        // Sample(std::string name, std::string treename="HTauTauTree");
        Sample(std::string name, std::string filelistname, std::string treename="HTauTauTree", std::string histoname="h_eff", int binEffDen = 1);
        
        // build from a list of other samples, histos are added together
        // NOTE: efficiency, ttree, and the other methods used for filling are not updated,
        // so just use this method to make a sum of all the histos of a sample after separate histos have already been filled
        // Sample(std::string name, std::vector<Sample const *> sampleList);

        ~Sample();
        std::string getName() const {return name_;}

        void  setType (sType sampleType) {sampleType_ = sampleType;}
        sType getType () {return sampleType_;}

        // file handling and preparation
        bool openFileAndTree();
        // void getEfficiency(std::string histoname="h_eff");
        long long int getEntries(){return nentries_;}
        void setEffBin(int ibin){
            std::cout << "  ---> INFO: setting eff. bin " << ibin << " for sample " << name_ << std::endl;
            bin_eff_den_ = ibin;
        }
        double getEffDenom(){return evt_den_;}

        // plot handling
        selColl& plots (){return plots_;}
        selColl2D& plots2D (){return plots2D_;}
        void scaleAll(double scale);

        // specific weights for a certain sample
        void setWeights (std::vector<Weight> weights) {weights_ = weights;}
        void addWeight  (Weight weight) {weights_.push_back(weight);}
        void clearWeights() {weights_.clear();}

        TChain* getTree() {return tree_.get();}

        const std::vector<Weight>& getWeights() const {return weights_;}
        std::vector<Weight>& getWeights() {return weights_;}
        // void addWeight  (std::string wstring); // if no syst, just weight name

    private:
        // TFile* fIn_;
        std::string filelistname_;
        std::string treename_;
        std::string histoname_;
        // TChain* tree_;
        std::unique_ptr<TChain> tree_;
        std::string name_;
        
        int    bin_eff_den_;
        double eff_;
        double evt_num_;
        double evt_den_;
        sType  sampleType_; // used in general to label the type of sample

        long long int nentries_;
        selColl plots_;
        selColl2D plots2D_;

        std::vector<Weight> weights_;
};

#endif