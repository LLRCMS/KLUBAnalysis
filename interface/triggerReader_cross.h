// class to read the trigger histogram in the files produced in the LLR framework
// and retrieve useful information depending on the pair

// using string everywhere!

#include "TH1F.h"
#include <string>
#include <vector>
#include <boost/regex.hpp>

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

        bool checkORMuTauNew         (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2, double pt_tau1, double pt_tau2);
        bool checkOREleTauNew        (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2, double pt_tau1, double pt_tau2);
        bool checkORTauTauNew        (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2, double pt_tau1, double pt_tau2);
        bool checkORMuEleNew         (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t trgNoOverlap, Long64_t goodTriggerType1, double pt_tau1);
        bool checkORMuMuNew          (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t trgNoOverlap, Long64_t goodTriggerType1, double pt_tau1);
        bool checkOREleEleNew        (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t trgNoOverlap, Long64_t goodTriggerType1, double pt_tau1);

        bool CheckBit (Long64_t number, int bitpos);
        bool checkOR (int pairType, Long64_t triggerbit_1, Long64_t matchFlag1=1, Long64_t matchFlag2=1, Long64_t trgNoOverlap=1, Long64_t goodTriggerType1=1, Long64_t goodTriggerType2=1, double pt_tau1=999, double pt_tau2=999);
        bool checkPtCutCross (bool OR, std::string firedPath, boost::regex re_tau1, boost::regex re_tau2, double pt_tau1, double pt_tau2, double thr1, double thr2);
        bool checkPtCutSingle (bool OR, std::string firedPath, boost::regex re_tau1, double pt_tau1, double thr1);

        void listTauTau (Long64_t triggerbit_1,Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2);
        void listETau   (Long64_t triggerbit_1,Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2);
        void listMuTau  (Long64_t triggerbit_1,Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2);
	
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
