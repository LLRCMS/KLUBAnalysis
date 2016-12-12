/*
    ** class: Selection
    ** author: L. Cadamuro (LLR)
    ** date: 29/08/2016
    ** description: string-based selection (i.e. TCut) that can be composed and has specific weights associated
*/

#ifndef SELECTION_H
#define SELECTION_H

#include <vector>
#include <string>
#include "TCut.h"
#include "Weight.h"

class Selection
{
    public:
        Selection(std::string name, TCut value, std::vector<Weight> weights);
        Selection(std::string name, TCut value);
        Selection(std::string name, std::vector<Selection*> parts); // concatenate multiple sels
        ~Selection(){};

        void extend     (const Selection& otherSel); // extends the current selection with otherSel
        void extend     (TCut otherSel); // extends the current selection with TCut
        void setName    (std::string name) {name_ = name;}
        void setValue   (TCut value) {value_ = value;}
        void setWeights (std::vector<Weight> weights) {weights_ = weights;}
        void addWeight  (Weight weight);
        void addWeight  (std::string wstring);

        std::string getName() const {return name_;}
        TCut getValue() const {return value_;}
        const std::vector<Weight>& getWeights() const {return weights_;}

    private:
        std::string name_;
        TCut value_;
        std::vector<Weight> weights_;
};

#endif