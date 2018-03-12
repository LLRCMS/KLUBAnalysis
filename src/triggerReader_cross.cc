#include "triggerReader_cross.h"
#include "OfflineProducerHelper.h"

using namespace std;

// ------------------------------
//     Constructor
// ------------------------------

triggerReader_cross::triggerReader_cross (TH1F* inputHisto)
{
    _allTriggers.clear();
    int nbins = inputHisto->GetNbinsX();
    int binstoskip = 3; // skip fist bintoskip bins as they don;t contain trigger info but other stuff
    for (int ibin = binstoskip+1; ibin <= nbins; ++ibin)
    {
        string thisName = string(inputHisto->GetXaxis()->GetBinLabel(ibin));
        _allTriggers.push_back(thisName);
    }
    return;
}

// ------------------------------
//     Destructor
// ------------------------------

triggerReader_cross::~triggerReader_cross()
{}


// ------------------------------
//     Add trigger lists
// ------------------------------

void triggerReader_cross::addTauTauTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()) _ttTriggers.push_back (it - _allTriggers.begin());
        else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

void triggerReader_cross::addMuTauTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()) _mtTriggers.push_back (it - _allTriggers.begin());
        else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

void triggerReader_cross::addEleTauTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()) _etTriggers.push_back (it - _allTriggers.begin());
        else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

void triggerReader_cross::addMuEleTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()) _emTriggers.push_back (it - _allTriggers.begin());
        else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}
void triggerReader_cross::addEleEleTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()) _eeTriggers.push_back (it - _allTriggers.begin());
        else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}
void triggerReader_cross::addMuMuTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()) _mmTriggers.push_back (it - _allTriggers.begin());
        else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

void triggerReader_cross::addMuTauCrossTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()) _mtCrossTriggers.push_back (it - _allTriggers.begin());
        else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

void triggerReader_cross::addEleTauCrossTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()) _etCrossTriggers.push_back (it - _allTriggers.begin());
        else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

void triggerReader_cross::addTauTauCrossTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()) _ttCrossTriggers.push_back (it - _allTriggers.begin());
        else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

// ------------------------------
// Check trigger filters matching
// ------------------------------

bool triggerReader_cross::checkORTauTauWithCross  (Long64_t triggerbit_1, Long64_t triggerbit_2)
{
    bool OR      = false;
    bool OR_leg1 = false;
    bool OR_leg2 = false;
    
    // First test the cross triggers
    for (unsigned int i = 0; i < _ttCrossTriggers.size(); i++)
    {
        OR_leg1 = CheckBit (triggerbit_1, _ttCrossTriggers.at(i));
        OR_leg2 = CheckBit (triggerbit_2, _ttCrossTriggers.at(i));
        OR = OR_leg1 && OR_leg2;
        if (OR) break;
    }
    
    // if it doesn't pass the cross triggers, test the single triggers
    if (!OR)
    {
        for (unsigned int i = 0; i < _ttTriggers.size(); i++)
        {
            OR_leg1 = CheckBit (triggerbit_1, _ttTriggers.at(i));
            OR_leg2 = true;
            OR = OR_leg1 && OR_leg2;
            if (OR) break;
        }
    }
    return OR;
}

bool triggerReader_cross::checkORMuTauWithCross  (Long64_t triggerbit_1, Long64_t triggerbit_2)
{
    bool OR      = false;
    bool OR_leg1 = false;
    bool OR_leg2 = false;
    
    // First test the cross triggers
    for (unsigned int i = 0; i < _mtCrossTriggers.size(); i++)
    {
        OR_leg1 = CheckBit (triggerbit_1, _mtCrossTriggers.at(i));
        OR_leg2 = CheckBit (triggerbit_2, _mtCrossTriggers.at(i));
        OR = OR_leg1 && OR_leg2;
        if (OR) break;
    }
    
    // if it doesn't pass the cross triggers, test the single triggers
    if (!OR)
    {
        for (unsigned int i = 0; i < _mtTriggers.size(); i++)
        {
            OR_leg1 = CheckBit (triggerbit_1, _mtTriggers.at(i));
            OR_leg2 = true;
            OR = OR_leg1 && OR_leg2;
            if (OR) break;
        }
    }
    return OR;
}

bool triggerReader_cross::checkOREleTauWithCross  (Long64_t triggerbit_1, Long64_t triggerbit_2)
{
    bool OR      = false;
    bool OR_leg1 = false;
    bool OR_leg2 = false;
    
    // First test the cross triggers
    for (unsigned int i = 0; i < _etCrossTriggers.size(); i++)
    {
        OR_leg1 = CheckBit (triggerbit_1, _etCrossTriggers.at(i));
        OR_leg2 = CheckBit (triggerbit_2, _etCrossTriggers.at(i));
        OR = OR_leg1 && OR_leg2;
        if (OR) break;
    }
    
    // if it doesn't pass the cross triggers, test the single triggers
    if (!OR)
    {
        for (unsigned int i = 0; i < _etTriggers.size(); i++)
        {
            OR_leg1 = CheckBit (triggerbit_1, _etTriggers.at(i));
            OR_leg2 = true;
            OR = OR_leg1 && OR_leg2;
            if (OR) break;
        }
    }
    return OR;
}

bool triggerReader_cross::checkORMuEle  (Long64_t triggerbit)
{
    bool OR = false;
    for (unsigned int i = 0; i < _emTriggers.size(); i++)
    {
        OR = CheckBit (triggerbit, _emTriggers.at(i));
        if (OR) break;
    }
    return OR;
}

bool triggerReader_cross::checkOREleEle  (Long64_t triggerbit)
{
    bool OR = false;
    for (unsigned int i = 0; i < _eeTriggers.size(); i++)
    {
        OR = CheckBit (triggerbit, _eeTriggers.at(i));
        if (OR) break;
    }
    return OR;
}

bool triggerReader_cross::checkORMuMu  (Long64_t triggerbit)
{
    bool OR = false;
    for (unsigned int i = 0; i < _mmTriggers.size(); i++)
    {
        OR = CheckBit (triggerbit, _mmTriggers.at(i));
        if (OR) break;
    }
    return OR;
}

// ------------------------------
//     Check Bit
// ------------------------------

bool triggerReader_cross::CheckBit (Long64_t number, int bitpos)
{
    Long64_t bitdigit = 1;
    bool res = number & (bitdigit << bitpos);
    return res;
}

// ------------------------------
//     General Check OR
// ------------------------------

bool triggerReader_cross::checkOR (int pairType, Long64_t triggerbit_1, Long64_t triggerbit_2)
{

    if      (pairType == ((int) OfflineProducerHelper::MuHad) ) return checkORMuTauWithCross (triggerbit_1, triggerbit_2);
    else if (pairType == ((int) OfflineProducerHelper::EHad)  ) return checkOREleTauWithCross(triggerbit_1, triggerbit_2);
    else if (pairType == ((int) OfflineProducerHelper::HadHad)) return checkORTauTauWithCross(triggerbit_1, triggerbit_2);
    else if (pairType == ((int) OfflineProducerHelper::EMu)   ) return checkORMuEle (triggerbit_1);
    else if (pairType == ((int) OfflineProducerHelper::EE)    ) return checkOREleEle(triggerbit_1);
    else if (pairType == ((int) OfflineProducerHelper::MuMu)  ) return checkORMuMu  (triggerbit_1);
    else
    {
        cout << " ** WARNING!! Pair type " << pairType << " not implemented for trigger" << endl;
        return false;
    }
}





