// class to read the trigger histogram in the files produced in the LLR framework
// and retrieve useful information depending on the pair

// using string everywhere!

#include "TH1F.h"
#include <string>
#include <vector>

class triggerReader_cross
{
    public:
        triggerReader_cross (TH1F* inputHisto);
        ~triggerReader_cross ();
        void addTauTauTrigs      (std::vector<std::string> list);
        void addMuTauTrigs       (std::vector<std::string> list);
        void addEleTauTrigs      (std::vector<std::string> list);
        void addMuEleTrigs       (std::vector<std::string> list);
        void addMuMuTrigs        (std::vector<std::string> list);
        void addEleEleTrigs      (std::vector<std::string> list);
        void addMuTauCrossTrigs  (std::vector<std::string> list);
        void addEleTauCrossTrigs (std::vector<std::string> list);
        void addTauTauCrossTrigs (std::vector<std::string> list);

        bool checkORMuTauWithCross   (Long64_t triggerbit_1, Long64_t triggerbit_2);
        bool checkOREleTauWithCross  (Long64_t triggerbit_1, Long64_t triggerbit_2);
        bool checkORTauTauWithCross  (Long64_t triggerbit_1, Long64_t triggerbit_2);
        bool checkORTauTau           (Long64_t triggerbit); // old style tautau
        bool checkORMuEle            (Long64_t triggerbit);
        bool checkORMuMu             (Long64_t triggerbit);
        bool checkOREleEle           (Long64_t triggerbit);

        bool CheckBit (Long64_t number, int bitpos);

        bool checkOR (int pairType, Long64_t triggerbit_1, Long64_t triggerbit_2=0);
    
    private:
        std::vector<std::string> _allTriggers; // all trigger names
    
        // --- these index the corresponding bits in the std::vector _allTriggers
        std::vector<Long64_t> _ttTriggers; // tau tau
        std::vector<Long64_t> _mtTriggers; // mu tau
        std::vector<Long64_t> _etTriggers; // e tau
        std::vector<Long64_t> _emTriggers; // e mu
        std::vector<Long64_t> _mmTriggers; // mu mu
        std::vector<Long64_t> _eeTriggers; // e e
        std::vector<Long64_t> _mtCrossTriggers; // mu tau cross triggers
        std::vector<Long64_t> _etCrossTriggers; // e tau  cross triggers
        std::vector<Long64_t> _ttCrossTriggers; // tau tau  cross triggers

};