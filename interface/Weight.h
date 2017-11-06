/*
    ** class: Sample
    ** author: L. Cadamuro (LLR)
    ** date: 18/09/2016
    ** description: class representing a weight associated to the "Fill" operation.
    **              Central value is always provided and systematic are optional.
    **              Systematics are defined using a key name and string ID (e.g. bTagUP, btag_M_up) [syst name -- name in TTree]
*/

#ifndef WEIGHT_H
#define WEIGHT_H

#include <string>
#include <utility>
#include <iostream>
#include <algorithm>
#include "ordered_map.h"

class Weight
{
    public:
        // typedef std::string wtype;
        Weight(){weight_ = "";}
        Weight(std::string weight){weight_ = weight;}
        Weight(const Weight& otherW){
            weight_ = otherW.weight_;
            systs_ = otherW.systs_;
        }
        ~Weight(){}
        bool operator == (const Weight& w) const { return weight_ == w.weight_;} // will compare name only, not systs
        Weight& operator = (Weight w) {
            std::swap(weight_, w.weight_);
            std::swap(systs_, w.systs_);
            return *this;
        }
        void  addSyst(std::string systName, std::string weight){systs_.append(systName, weight);}
        void  addSysts(std::vector<std::pair<std::string, std::string> > vSysts);
        bool  hasSysts() const {return (systs_.size() > 0);}
        int   getNSysts()   const {return systs_.size();}
        std::string getName() const {return weight_;} // nominal weight as appears in TTree
        std::string getSyst (int idx) const {return systs_.at(idx);} // return syst weight as appears in TTree
        std::string getSyst (std::string name) const {return systs_.at(name);} // return syst weight as appears in TTree
        std::string getSystName (int idx) const {return systs_.key(idx);} // return syst weight name as it is appended to the histograms
        // float& getRefToWeightValue() {return weightValue_;}
        // float getWeightValue() const {return weightValue_;}
        // std::vector<float>& getSystsValues() {return systsValues_;}

    private:
        std::string weight_; // the name of the weight as in the TTree
        // float weightValue_;  // to be used for setbranchaddress
        ordered_map<std::string, std::string> systs_; // pairs of [name for histograms - value in TTree]
        // std::vector<float> systsValues_; // to be used for setbranchaddress

};

#endif