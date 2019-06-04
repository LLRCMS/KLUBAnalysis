/*
    ** class: Sample
    ** author: L. Cadamuro (LLR)
    ** date: 18/09/2016
    ** description: class representing a weight associated to the "Fill" operation.
    **              Central value is always provided and systematic are optional.
    **              Systematics are defined using a key name and string ID (e.g. bTagUP, btag_M_up) [syst name -- name in TTree]
*/

#ifndef WEIGHTEXT_H
#define WEIGHTEXT_H

#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>
#include "ordered_map.h"

class Weight_ext
{
    public:
        // typedef std::string wtype;
        Weight_ext(){weightFormula_ = ""; weightValue_ = -1.;}
        Weight_ext(std::string formula, float value){weightFormula_ = formula; weightValue_ = value;}
        Weight_ext(const Weight_ext& otherW){
	  weightFormula_ = otherW.weightFormula_;
	  weightValue_ = otherW.weightValue_;
	  //systs_ = otherW.systs_;
        }
        ~Weight_ext(){}
        bool operator == (const Weight_ext& w) const { return weightFormula_ == w.weightFormula_;} // will compare name only, not systs
        //Weight_ext& operator = (Weight_ext w) {
	//      std::swap(weightFormula_, w.weightFormula_);
        //    std::swap(systs_, w.systs_);
        //    return *this;
        //}
        //void  addSyst(std::string systName, std::string weight){systs_.append(systName, weight);}
        //void  addSysts(std::vector<std::pair<std::string, std::string> > vSysts);
        //bool  hasSysts() const {return (systs_.size() > 0);}
        //int   getNSysts()   const {return systs_.size();}
        std::string getFormula() const {return weightFormula_;} // nominal weight as appears in TTree
        //std::string getSyst (int idx) const {return systs_.at(idx);} // return syst weight as appears in TTree
        //std::string getSyst (std::string name) const {return systs_.at(name);} // return syst weight as appears in TTree
        //std::string getSystName (int idx) const {return systs_.key(idx);} // return syst weight name as it is appended to the histograms
        // float& getRefToWeightValue() {return weightValue_;}
        float getValue() const {return weightValue_;}
        // std::vector<float>& getSystsValues() {return systsValues_;}

    private:
        std::string weightFormula_; // the name of the weight as in the TTree
        float weightValue_;  // to be used for setbranchaddress
        //ordered_map<std::string, std::string> systs_; // pairs of [name for histograms - value in TTree]
        // std::vector<float> systsValues_; // to be used for setbranchaddress

};

#endif
