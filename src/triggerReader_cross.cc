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

bool triggerReader_cross::checkORTauTau  (Long64_t triggerbit) // old style tautau
{
    bool OR = false;
    for (unsigned int i = 0; i < _ttTriggers.size(); i++)
    {
        OR = CheckBit (triggerbit, _ttTriggers.at(i));
        if (OR) break;
    }
    if (!OR)
    {
        for (unsigned int i = 0; i < _ttCrossTriggers.size(); i++)
        {
            OR = CheckBit (triggerbit, _ttCrossTriggers.at(i));
            if (OR) break;
        }
    }
    return OR;
}

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

bool triggerReader_cross::checkOR (int pairType, Long64_t triggerbit_1,Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2)
{

    if      (pairType == ((int) OfflineProducerHelper::MuHad) ) return checkORMuTauNew(triggerbit_1, matchFlag1, matchFlag2, trgNoOverlap,goodTriggerType1, goodTriggerType2);
    else if (pairType == ((int) OfflineProducerHelper::EHad)  ) return checkOREleTauNew(triggerbit_1, matchFlag1, matchFlag2, trgNoOverlap,goodTriggerType1, goodTriggerType2);
    else if (pairType == ((int) OfflineProducerHelper::HadHad)) return checkORTauTauNew(triggerbit_1, matchFlag1, matchFlag2, trgNoOverlap,goodTriggerType1, goodTriggerType2);
    //    else if (pairType == ((int) OfflineProducerHelper::HadHad)) return checkORTauTauWithCross(triggerbit_1, triggerbit_2);
    //else if (pairType == ((int) OfflineProducerHelper::HadHad)) return checkORTauTau(triggerbit_1); // old version, working
    else if (pairType == ((int) OfflineProducerHelper::EMu)   ) return checkORMuEleNew (triggerbit_1,matchFlag1,trgNoOverlap,goodTriggerType1);
    else if (pairType == ((int) OfflineProducerHelper::EE)    ) return checkOREleEleNew(triggerbit_1,matchFlag1,trgNoOverlap,goodTriggerType1);
    else if (pairType == ((int) OfflineProducerHelper::MuMu)  ) return checkORMuMuNew  (triggerbit_1,matchFlag1,trgNoOverlap,goodTriggerType1);
    else
    {
        cout << " ** WARNING!! Pair type " << pairType << " not implemented for trigger" << endl;
        return false;
    }
}


bool triggerReader_cross::checkOREleEleNew  (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t trgNoOverlap, Long64_t goodTriggerType1)
{
    bool OR      = false;
    bool _trgNoOverlap = false;
    bool match = false;
    bool goodType = false;

      for (unsigned int i = 0; i < _eeTriggers.size(); i++)
	{
	  OR = CheckBit (triggerbit_1, _eeTriggers.at(i));
	  if (OR){
	    match = CheckBit (matchFlag1, _eeTriggers.at(i));
	    goodType = CheckBit (goodTriggerType1, _eeTriggers.at(i));
	    _trgNoOverlap = CheckBit (trgNoOverlap, _eeTriggers.at(i));
	  }
	  if (!(OR && match && _trgNoOverlap && goodType)) OR = false;
	  if (OR) break;
	}
    return OR;
}

bool triggerReader_cross::checkORMuEleNew  (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t trgNoOverlap, Long64_t goodTriggerType1)
{
    bool OR      = false;
    bool _trgNoOverlap = false;
    bool match = false;
    bool goodType = false;

      for (unsigned int i = 0; i < _emTriggers.size(); i++)
	{
	  OR = CheckBit (triggerbit_1, _emTriggers.at(i));
	  if (OR){
	    match = CheckBit (matchFlag1, _emTriggers.at(i));
	    goodType = CheckBit (goodTriggerType1, _emTriggers.at(i));
	    _trgNoOverlap = CheckBit (trgNoOverlap, _emTriggers.at(i));
	  }
	  if (!(OR && match && _trgNoOverlap && goodType)) OR = false;
	  if (OR) break;
	}
    return OR;
}

bool triggerReader_cross::checkORMuMuNew  (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t trgNoOverlap, Long64_t goodTriggerType1)
{
    bool OR      = false;
    bool _trgNoOverlap = false;
    bool match = false;
    bool goodType = false;

      for (unsigned int i = 0; i < _mmTriggers.size(); i++)
	{
	  OR = CheckBit (triggerbit_1, _mmTriggers.at(i));
	  if (OR){
	    match = CheckBit (matchFlag1, _mmTriggers.at(i));
	    goodType = CheckBit (goodTriggerType1, _mmTriggers.at(i));
	    _trgNoOverlap = CheckBit (trgNoOverlap, _mmTriggers.at(i));
	  }
	  if (!(OR && match && _trgNoOverlap && goodType)) OR = false;
	  if (OR) break;
	}
    return OR;
}

bool triggerReader_cross::checkORTauTauNew  (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2)
{
    bool OR      = false;
    bool match1 = false;
    bool match2 = false;
    bool goodType1 = false;
    bool goodType2 = false;
    bool _trgNoOverlap = false;
    bool match = false;
    bool goodType = false;
    for (unsigned int i = 0; i < _ttCrossTriggers.size(); i++)
      {
	
	OR = CheckBit (triggerbit_1, _ttCrossTriggers.at(i));
	if (OR){
	  
	  match1 = CheckBit (matchFlag1, _ttCrossTriggers.at(i));
	  match2 = CheckBit (matchFlag2, _ttCrossTriggers.at(i));
	  match = match1 && match2;
	  goodType1 = CheckBit (goodTriggerType1, _ttCrossTriggers.at(i));
	  goodType2 = CheckBit (goodTriggerType2, _ttCrossTriggers.at(i));
	  goodType = goodType1 && goodType2;
	  
	  _trgNoOverlap = CheckBit (trgNoOverlap, _ttCrossTriggers.at(i));
	  
	}
	if (!(OR && match && _trgNoOverlap && goodType)) OR = false;
	if (OR) break;
      }
    if (!OR){
      for (unsigned int i = 0; i < _ttTriggers.size(); i++)
	{
	  OR = CheckBit (triggerbit_1, _ttTriggers.at(i));
	  if (OR){
	  match1 = CheckBit (matchFlag1, _ttTriggers.at(i));
	  match2 = true;
	  match = match1 && match2;
	  goodType1 = CheckBit (goodTriggerType1, _ttTriggers.at(i));
	  goodType2 = true;
	  goodType = goodType1 && goodType2;
	  _trgNoOverlap = CheckBit (trgNoOverlap, _ttTriggers.at(i));
	  }
	  if (!(OR && match && _trgNoOverlap && goodType)) OR = false;
	  if (OR) break;
	}
      
      
    }
    return OR;
    
}

bool triggerReader_cross::checkORMuTauNew  (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2)
{
    bool OR      = false;
    bool match1 = false;
    bool match2 = false;
    bool goodType1 = false;
    bool goodType2 = false;
    bool _trgNoOverlap = false;
    bool match = false;
    bool goodType = false;
    for (unsigned int i = 0; i < _mtCrossTriggers.size(); i++)
      {
	
	OR = CheckBit (triggerbit_1, _mtCrossTriggers.at(i));
	if (OR){
	  
	  match1 = CheckBit (matchFlag1, _mtCrossTriggers.at(i));
	  match2 = CheckBit (matchFlag2, _mtCrossTriggers.at(i));
	  match = match1 && match2;
	  goodType1 = CheckBit (goodTriggerType1, _mtCrossTriggers.at(i));
	  goodType2 = CheckBit (goodTriggerType2, _mtCrossTriggers.at(i));
	  goodType = goodType1 && goodType2;
	  
	  _trgNoOverlap = CheckBit (trgNoOverlap, _mtCrossTriggers.at(i));
	  
	}
	if (!(OR && match && _trgNoOverlap && goodType)) OR = false;
	if (OR) break;
      }
    if (!OR){
      for (unsigned int i = 0; i < _mtTriggers.size(); i++)
	{
	  OR = CheckBit (triggerbit_1, _mtTriggers.at(i));
	  if (OR){
	  match1 = CheckBit (matchFlag1, _mtTriggers.at(i));
	  match2 = true;
	  match = match1 && match2;
	  goodType1 = CheckBit (goodTriggerType1, _mtTriggers.at(i));
	  goodType2 = true;
	  goodType = goodType1 && goodType2;
	  _trgNoOverlap = CheckBit (trgNoOverlap, _mtTriggers.at(i));
	  }
	  if (!(OR && match && _trgNoOverlap && goodType)) OR = false;
	  if (OR) break;
	}
      
      
    }
    return OR;
    
}


bool triggerReader_cross::checkOREleTauNew  (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2)
{
    bool OR      = false;
    bool match1 = false;
    bool match2 = false;
    bool goodType1 = false;
    bool goodType2 = false;
    bool _trgNoOverlap = false;
    bool match = false;
    bool goodType = false;
    for (unsigned int i = 0; i < _etCrossTriggers.size(); i++)
      {
	
	OR = CheckBit (triggerbit_1, _etCrossTriggers.at(i));
	if (OR){
	  
	  match1 = CheckBit (matchFlag1, _etCrossTriggers.at(i));
	  match2 = CheckBit (matchFlag2, _etCrossTriggers.at(i));
	  match = match1 && match2;
	  goodType1 = CheckBit (goodTriggerType1, _etCrossTriggers.at(i));
	  goodType2 = CheckBit (goodTriggerType2, _etCrossTriggers.at(i));
	  goodType = goodType1 && goodType2;
	  
	  _trgNoOverlap = CheckBit (trgNoOverlap, _etCrossTriggers.at(i));
	  
	}
	if (!(OR && match && _trgNoOverlap && goodType)) OR = false;
	if (OR) break;
      }
    if (!OR){
      for (unsigned int i = 0; i < _etTriggers.size(); i++)
	{
	  OR = CheckBit (triggerbit_1, _etTriggers.at(i));
	  if (OR){
	  match1 = CheckBit (matchFlag1, _etTriggers.at(i));
	  match2 = true;
	  match = match1 && match2;
	  goodType1 = CheckBit (goodTriggerType1, _etTriggers.at(i));
	  goodType2 = true;
	  goodType = goodType1 && goodType2;
	  _trgNoOverlap = CheckBit (trgNoOverlap, _etTriggers.at(i));
	  }
	  if (!(OR && match && _trgNoOverlap && goodType)) OR = false;
	  if (OR) break;
	}
      
      
    }
    return OR;
    
}


void triggerReader_cross::listTauTau  (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2)
{
    bool OR      = false;
    bool match1 = false;
    bool match2 = false;
    bool goodType1 = false;
    bool goodType2 = false;
    bool _trgNoOverlap = false;
    bool match = false;
    bool goodType = false;
    for (unsigned int i = 0; i < _ttCrossTriggers.size(); i++)
      {
	
	OR = CheckBit (triggerbit_1, _ttCrossTriggers.at(i));
	if (OR){
	  cout <<"** trg: cross TauTau fired: "<<_allTriggers.at(_ttCrossTriggers.at(i))<<endl;
	}else{
	   cout <<"** trg: cross TauTau NOT fired: "<<_allTriggers.at(_ttCrossTriggers.at(i))<<endl;
	}
	  match1 = CheckBit (matchFlag1, _ttCrossTriggers.at(i));
	    match2 = CheckBit (matchFlag2, _ttCrossTriggers.at(i));
	    match = match1 && match2;
	    goodType1 = CheckBit (goodTriggerType1, _ttCrossTriggers.at(i));
	    goodType2 = CheckBit (goodTriggerType2, _ttCrossTriggers.at(i));
	    goodType = goodType1 && goodType2;
	    
	    _trgNoOverlap = CheckBit (trgNoOverlap, _ttCrossTriggers.at(i));
	    cout <<"   matchFlag   "<< match<<endl;
	    cout <<"   trgNoOverlap "<<_trgNoOverlap<<endl;
	    cout <<"   goodType "<<goodType <<endl;

    
      }

    for (unsigned int i = 0; i < _ttTriggers.size(); i++)
      {
	OR = CheckBit (triggerbit_1, _ttTriggers.at(i));

	if (OR) {
	  cout <<"** trg: single TauTau fired: "<<_allTriggers.at(_ttTriggers.at(i))<<endl;
	}else{
	  cout <<"** trg: single TauTau NOT fired: "<<_allTriggers.at(_ttTriggers.at(i))<<endl;
	}
	match1 = CheckBit (matchFlag1, _ttTriggers.at(i));
	match2 = true;
	match = match1 && match2;
	goodType1 = CheckBit (goodTriggerType1, _ttTriggers.at(i));
	goodType2 = true;
	  goodType = goodType1 && goodType2;
	  _trgNoOverlap = CheckBit (trgNoOverlap, _ttTriggers.at(i));
	  cout <<"   matchFlag   "<< match<<endl;
	  cout <<"   trgNoOverlap "<<_trgNoOverlap<<endl;
	  cout <<"   goodType "<<goodType<<endl;
      }

	  
}



void triggerReader_cross::listETau  (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap,Long64_t goodTriggerType1, Long64_t goodTriggerType2)
{
    bool OR      = false;
    bool match1 = false;
    bool match2 = false;
    bool goodType1 = false;
    bool goodType2 = false;
    bool _trgNoOverlap = false;
    bool match = false;
    bool goodType = false;
    for (unsigned int i = 0; i < _etCrossTriggers.size(); i++)
      {
	OR = CheckBit (triggerbit_1, _etCrossTriggers.at(i));
	if (OR){
	  cout <<"** trg: cross ETau fired: "<<_allTriggers.at(_etCrossTriggers.at(i))<<endl;
	}else{
	  cout <<"** trg: cross ETau NOT fired: "<<_allTriggers.at(_etCrossTriggers.at(i))<<endl;
	} 
	match1 = CheckBit (matchFlag1, _etCrossTriggers.at(i));
	match2 = CheckBit (matchFlag2, _etCrossTriggers.at(i));
	match = match1 && match2;
	goodType1 = CheckBit (goodTriggerType1, _etCrossTriggers.at(i));
	goodType2 = CheckBit (goodTriggerType2, _etCrossTriggers.at(i));
	goodType = goodType1 && goodType2;
	_trgNoOverlap = CheckBit (trgNoOverlap, _etCrossTriggers.at(i));
	cout <<"   matchFlag   "<< match<<endl;
	cout <<"   trgNoOverlap "<<_trgNoOverlap<<endl;
	cout <<"   goodType "<<goodType<<endl;
	
      }
    
    for (unsigned int i = 0; i < _etTriggers.size(); i++)
      {
	OR = CheckBit (triggerbit_1, _etTriggers.at(i));

	if (OR) {
	  cout <<"** trg: single ETau fired: "<<_allTriggers.at(_etTriggers.at(i))<<endl;
	}else{
	  cout <<"** trg: single ETau NOT fired: "<<_allTriggers.at(_etTriggers.at(i))<<endl;
	}
	match1 = CheckBit (matchFlag1, _etTriggers.at(i));
	match2 = true;
	match = match1 && match2;
	goodType1 = CheckBit (goodTriggerType1, _etTriggers.at(i));
	goodType2 = true;
	goodType = goodType1 && goodType2;
	_trgNoOverlap = CheckBit (trgNoOverlap, _etTriggers.at(i));
	cout <<"   matchFlag   "<< match<<endl;
	cout <<"   trgNoOverlap "<<_trgNoOverlap<<endl;
	cout <<"   goodType "<<goodType<<endl;
	
      }
    
}


void triggerReader_cross::listMuTau  (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2)
{
    bool OR      = false;
    bool match1 = false;
    bool match2 = false;
    bool goodType1 = false;
    bool goodType2 = false;
    bool _trgNoOverlap = false;
    bool match = false;
    bool goodType = false;
    for (unsigned int i = 0; i < _mtCrossTriggers.size(); i++)
      {
	
	OR = CheckBit (triggerbit_1, _mtCrossTriggers.at(i));
	if (OR)
	  {
	    cout <<"** trg: cross MuTau fired: "<<_allTriggers.at(_mtCrossTriggers.at(i))<<endl;
	  }else{
	    cout <<"** trg: cross MuTau NOT fired: "<<_allTriggers.at(_mtCrossTriggers.at(i))<<endl;
	}
	    match1 = CheckBit (matchFlag1, _mtCrossTriggers.at(i));
	    match2 = CheckBit (matchFlag2, _mtCrossTriggers.at(i));
	    match = match1 && match2;
	    goodType1 = CheckBit (goodTriggerType1, _mtCrossTriggers.at(i));
	    goodType2 = CheckBit (goodTriggerType2, _mtCrossTriggers.at(i));
	    goodType = goodType1 && goodType2;
	    _trgNoOverlap = CheckBit (trgNoOverlap, _mtCrossTriggers.at(i));
	    cout <<"   matchFlag   "<< match<<endl;
	    cout <<"   trgNoOverlap "<<_trgNoOverlap<<endl;
	    cout <<"   goodType "<<goodType<<endl;
	

      }
    
    for (unsigned int i = 0; i < _mtTriggers.size(); i++)
      {
	OR = CheckBit (triggerbit_1, _mtTriggers.at(i));
	
	if (OR) {
	  cout <<"** trg: single MuTau fired: "<<_allTriggers.at(_mtTriggers.at(i))<<endl;
	}else{
	  cout <<"** trg: single MuTau NOT fired: "<<_allTriggers.at(_mtTriggers.at(i))<<endl;
	}
	match1 = CheckBit (matchFlag1, _mtTriggers.at(i));
	match2 = true;
	match = match1 && match2;
	goodType1 = CheckBit (goodTriggerType1, _mtTriggers.at(i));
	goodType2 = true;
	goodType = goodType1 && goodType2;
	_trgNoOverlap = CheckBit (trgNoOverlap, _mtTriggers.at(i));
	cout <<"   matchFlag   "<< match<<endl;
	cout <<"   trgNoOverlap "<<_trgNoOverlap<<endl;
	cout <<"   goodType "<<goodType<<endl;
	  
	
      

      }
}



/*void triggerReader_cross::test()
{
  for (unsigned int i = 0; i < _etCrossTriggers.size(); i++)
    {
      
      cout <<"** trg: cross eletau : "<<_allTriggers.at(_etCrossTriggers.at(i))<<endl;
    }

  
}
*/
