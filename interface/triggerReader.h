// class to read the trigger histogram in the files produced in the LLR framework
// and retrieve useful information depending on the pair

// using string everywhere!

#include "TH1F.h"
#include <string>
#include <vector>

class triggerReader
{
    public:
        triggerReader (TH1F* inputHisto);
        ~triggerReader ();
        void addTauTauTrigs (std::vector<std::string> list);
        void addMuTauTrigs  (std::vector<std::string> list);
        void addEleTauTrigs (std::vector<std::string> list);
        void addMuEleTrigs  (std::vector<std::string> list);
        
        bool checkORTauTau  (int triggerbit);
        bool checkORMuTau   (int triggerbit);
        bool checkOREleTau  (int triggerbit);
        bool checkORMuEle   (int triggerbit);

        bool CheckBit (int number, int bitpos);

        bool checkOR (int pairType, int triggerbit);

    private:
        std::vector<std::string> _allTriggers; // all trigger names
        // --- these index the corresponding bits in the std::vector _allTriggers
        std::vector<int> _ttTriggers; // tau tau
        std::vector<int> _mtTriggers; // mu tau
        std::vector<int> _etTriggers; // e tau
        std::vector<int> _emTriggers; // e mu

};