#include "triggerReader.h"
#include "OfflineProducerHelper.h"

using namespace std;

triggerReader::triggerReader (TH1F* inputHisto)
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

triggerReader::~triggerReader()
{}

void triggerReader::addTauTauTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()) _ttTriggers.push_back (it - _allTriggers.begin());
        else cout << " ** WARNING triggerReader : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

void triggerReader::addMuTauTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()) _mtTriggers.push_back (it - _allTriggers.begin());
        else cout << " ** WARNING triggerReader : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

void triggerReader::addEleTauTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()) _etTriggers.push_back (it - _allTriggers.begin());
        else cout << " ** WARNING triggerReader : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

void triggerReader::addMuEleTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()) _emTriggers.push_back (it - _allTriggers.begin());
        else cout << " ** WARNING triggerReader : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

bool triggerReader::checkORTauTau  (int triggerbit)
{
    bool OR = false;
    for (unsigned int i = 0; i < _ttTriggers.size(); i++)
    {
        OR = CheckBit (triggerbit, _ttTriggers.at(i));
        if (OR) break;
    }
    return OR;
}

bool triggerReader::checkORMuTau  (int triggerbit)
{
    bool OR = false;
    for (unsigned int i = 0; i < _mtTriggers.size(); i++)
    {
        OR = CheckBit (triggerbit, _mtTriggers.at(i));
        if (OR) break;
    }
    return OR;
}

bool triggerReader::checkOREleTau  (int triggerbit)
{
    bool OR = false;
    for (unsigned int i = 0; i < _etTriggers.size(); i++)
    {
        OR = CheckBit (triggerbit, _etTriggers.at(i));
        if (OR) break;
    }
    return OR;
}

bool triggerReader::checkORMuEle  (int triggerbit)
{
    bool OR = false;
    for (unsigned int i = 0; i < _emTriggers.size(); i++)
    {
        OR = CheckBit (triggerbit, _emTriggers.at(i));
        if (OR) break;
    }
    return OR;
}

bool triggerReader::CheckBit (int number, int bitpos)
{
    bool res = number & (1 << bitpos);
    return res;
}

bool triggerReader::checkOR (int pairType, int triggerbit)
{
    if (pairType == ((int) OfflineProducerHelper::MuHad) )      return checkORMuTau(triggerbit);
    else if (pairType == ((int) OfflineProducerHelper::EHad) )   return checkOREleTau(triggerbit);
    else if (pairType == ((int) OfflineProducerHelper::HadHad) ) return checkORTauTau(triggerbit);
    else if (pairType == ((int) OfflineProducerHelper::EMu) )    return checkORMuEle(triggerbit);
    else
    {
        cout << " ** WARNING!! Pair type " << pairType << " not implemented for trigger" << endl;
        return false;
    }
}
