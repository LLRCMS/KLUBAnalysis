/*
** class  : EffCounter
** author : L. Cadamuro (LLR)
** date   : 28/09/2016
** brief  : implements a counter with labeled entries to be used for efficiencies
**          (efficiency at each step of a selection chain)
** note   : using doubles to better take into account big sums of weighted values
*/

#ifndef EFFCOUNTER_H
#define EFFCOUNTER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <ostream>
#include <iostream>
#include <utility>

class EffCounter {

    public:
        EffCounter  ();
        ~EffCounter ();
        bool AddMarker (std::string mname); // true if insertion succeed
        bool Increment (std::string mname, double val); // true if increment succeed
        void PrintSummary (std::ostream& os = std::cout); //  = std::cout
        std::vector <std::pair<std::string, double> > GetSummary();
    private:
        std::vector<std::string> insertedMarkers_; // to keep insertion order
        std::unordered_map<std::string, double> counter_; // to keep insertion order
};

#endif