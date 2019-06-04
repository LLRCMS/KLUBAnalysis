#include "Selection.h"

using namespace std;

Selection::Selection(string name, TCut value, vector<Weight> weights, vector<Weight_ext> weights_ext)
{
    name_ = name;
    value_ = value;
    weights_ = weights;
    weights_ext_ = weights_ext;
}

Selection::Selection(string name, TCut value) :
  Selection(name, value, vector<Weight>(0), vector<Weight_ext>(0))
{}

Selection::Selection(string name, vector<Selection*> parts)
{
    name_ = name;
    value_ = "";
    weights_.clear();
    weights_ext_.clear();
    for (Selection* prt : parts)
    {
        extend(*prt);
    }

    return;
}

void Selection::extend(const Selection& otherSel)
{
    value_ += otherSel.getValue();
    for (Weight w : otherSel.getWeights())
    {
        if (find(weights_.begin(), weights_.end(), w) == weights_.end())
            weights_.push_back(w);
    }
    for (Weight_ext w : otherSel.getWeightsExt())
    {
        if (find(weights_ext_.begin(), weights_ext_.end(), w) == weights_ext_.end())
            weights_ext_.push_back(w);
    }
    return;
}

void Selection::extend (TCut otherSel)
{
    value_ += otherSel;
    return;
}

void Selection::addWeight (Weight weight)
{
    if (find(weights_.begin(), weights_.end(), weight) == weights_.end())
        weights_.push_back(weight);
    return;
}

void Selection::addWeight (string wstring)
{
    addWeight(Weight(wstring));
}



void Selection::addWeightExt (Weight_ext weight)
{
  //if (find(weights_ext_.begin(), weights_ext_.end(), weight) == weights_ext_.end())
        weights_ext_.push_back(weight);
    return;
}

void Selection::addWeightExt (string wstring, float wfloat)
{
  addWeightExt(Weight_ext(wstring,wfloat));
}
