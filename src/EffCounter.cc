#include "EffCounter.h"
#include <algorithm>

using namespace std;

EffCounter::EffCounter()
{}

EffCounter::~EffCounter()
{}

bool EffCounter::AddMarker(string mname)
{
    if (find(insertedMarkers_.begin(), insertedMarkers_.end(), mname) != insertedMarkers_.end())
        return false;

    insertedMarkers_.push_back(mname);
    counter_[mname] = 0.;
    return true;
}

bool EffCounter::Increment(string mname, double val)
{
    if (find(insertedMarkers_.begin(), insertedMarkers_.end(), mname) == insertedMarkers_.end())
        return false;
    counter_[mname] += val;
    return true;
}

void EffCounter::PrintSummary (ostream& os)
{
    os << "*** Eff counter summary *** " << endl;
    for (string mk : insertedMarkers_)
    {
        os << mk << " " << counter_[mk] << endl; 
    }
    return;
}

vector <pair<string, double> > EffCounter::GetSummary()
{
    vector <pair<string, double> > vSum;
    for (string mk : insertedMarkers_)
    {
        vSum.push_back(make_pair(mk, counter_[mk]));
    }
    return vSum;

}

