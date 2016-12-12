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

class Sample
{
    public:
        // hierarchy of objects [selection[variable[systematics]]] 
        typedef ordered_map<std::string, std::shared_ptr<TH1F> >  systColl;  // histograms stored - map is for systematics
        typedef ordered_map<std::string, systColl>                varColl;   // variables plotted
        typedef ordered_map<std::string, varColl>                 selColl;   // all selections

        enum sType {
            kData = 0,
            kSig  = 1,
            kBkg  = 2
        };

        // standard ctor/dtor
        // Sample(std::string name, std::string treename="HTauTauTree");
        Sample(std::string name, std::string filelistname, std::string treename="HTauTauTree", std::string histoname="h_eff");
        ~Sample();
        std::string getName() const {return name_;}

        void setType (sType sampleType) {sampleType_ = sampleType;}

        // file handling and preparation
        void openFileAndTree(std::string treename="HTauTauTree", std::string histoname="h_eff");
        // void getEfficiency(std::string histoname="h_eff");
        long long int getEntries(){return nentries_;}

        // plot handling
        selColl& plots (){return plots_;}

        // specific weights for a certain sample
        void setWeights (std::vector<Weight> weights) {weights_ = weights;}
        void addWeight  (Weight weight) {weights_.push_back(weight);};
        void clearWeights() {weights_.clear();}

        TChain* getTree() {return tree_;}

        const std::vector<Weight>& getWeights() const {return weights_;}
        std::vector<Weight> getWeights() {return weights_;}
        // void addWeight  (std::string wstring); // if no syst, just weight name

    private:
        // TFile* fIn_;
        std::string filelistname_;
        TChain* tree_;
        std::string name_;
        
        double eff_;
        double evt_num_;
        double evt_den_;
        sType  sampleType_; // used in general to label the type of sample

        long long int nentries_;
        selColl plots_;

        std::vector<Weight> weights_;
};

#endif