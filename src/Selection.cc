#include "Selection.h"

using namespace std;

Selection::Selection(string name, TCut value, vector<Weight> weights)
{
    name_ = name;
    value_ = value;
    weights_ = weights;
}

Selection::Selection(string name, TCut value) :
Selection(name, value, vector<Weight>(0))
{}

Selection::Selection(string name, vector<Selection*> parts)
{
    name_ = name;
    value_ = "";
    weights_.clear();

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