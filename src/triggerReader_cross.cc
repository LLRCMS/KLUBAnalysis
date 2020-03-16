#include "triggerReader_cross.h"
#include "OfflineProducerHelper.h"

#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>

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
        if (it != _allTriggers.end()){
	  _ttTriggers.push_back (it - _allTriggers.begin());
	  auto itlist = find(_thisSkimTriggers.begin(),_thisSkimTriggers.end(),list.at(i));
	  if (itlist == _thisSkimTriggers.end()) _thisSkimTriggers.push_back(list.at(i));
	}
	  else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

void triggerReader_cross::addMuTauTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()) {
	  _mtTriggers.push_back (it - _allTriggers.begin());
	  auto itlist = find(_thisSkimTriggers.begin(),_thisSkimTriggers.end(),list.at(i));
	  if (itlist == _thisSkimTriggers.end()) _thisSkimTriggers.push_back(list.at(i));
	}
	  else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

void triggerReader_cross::addEleTauTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()){
	  _etTriggers.push_back (it - _allTriggers.begin());
	  auto itlist = find(_thisSkimTriggers.begin(),_thisSkimTriggers.end(),list.at(i));
	  if (itlist == _thisSkimTriggers.end()) _thisSkimTriggers.push_back(list.at(i));
	}
        else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

void triggerReader_cross::addMuEleTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()){
	  _emTriggers.push_back (it - _allTriggers.begin());
	  auto itlist = find(_thisSkimTriggers.begin(),_thisSkimTriggers.end(),list.at(i));
	  if (itlist == _thisSkimTriggers.end()) _thisSkimTriggers.push_back(list.at(i));
	}
	  else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}
void triggerReader_cross::addEleEleTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()){
	  _eeTriggers.push_back (it - _allTriggers.begin());
	  auto itlist = find(_thisSkimTriggers.begin(),_thisSkimTriggers.end(),list.at(i));
	  if (itlist == _thisSkimTriggers.end()) _thisSkimTriggers.push_back(list.at(i));
	}
	  else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}
void triggerReader_cross::addMuMuTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()){
	  _mmTriggers.push_back (it - _allTriggers.begin());
	  auto itlist = find(_thisSkimTriggers.begin(),_thisSkimTriggers.end(),list.at(i));
	  if (itlist == _thisSkimTriggers.end()) _thisSkimTriggers.push_back(list.at(i));
	}
	else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

void triggerReader_cross::addMuTauCrossTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()) {
	  _mtCrossTriggers.push_back (it - _allTriggers.begin());
	  auto itlist = find(_thisSkimTriggers.begin(),_thisSkimTriggers.end(),list.at(i));
	  if (itlist == _thisSkimTriggers.end()) _thisSkimTriggers.push_back(list.at(i));
	}
        else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

void triggerReader_cross::addEleTauCrossTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()){
	  _etCrossTriggers.push_back (it - _allTriggers.begin());
	  auto itlist = find(_thisSkimTriggers.begin(),_thisSkimTriggers.end(),list.at(i));
	  if (itlist == _thisSkimTriggers.end()) _thisSkimTriggers.push_back(list.at(i));
	}
	  else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

void triggerReader_cross::addTauTauCrossTrigs (vector<string> list)
{
    for (unsigned int i = 0; i < list.size(); i++)
    {
        auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
        if (it != _allTriggers.end()) {
	  _ttCrossTriggers.push_back (it - _allTriggers.begin());
	  auto itlist = find(_thisSkimTriggers.begin(),_thisSkimTriggers.end(),list.at(i));
	  if (itlist == _thisSkimTriggers.end()) _thisSkimTriggers.push_back(list.at(i));
	}
	else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

void triggerReader_cross::addVBFTrigs (vector<string> list)
{
  for (unsigned int i = 0; i < list.size(); i++)
    {
      auto it = find (_allTriggers.begin(), _allTriggers.end(), list.at(i));
      if (it != _allTriggers.end()){
	_vbfTriggers.push_back (it - _allTriggers.begin());
	auto itlist = find(_thisSkimTriggers.begin(),_thisSkimTriggers.end(),list.at(i));
	if (itlist == _thisSkimTriggers.end()) _thisSkimTriggers.push_back(list.at(i));
      }
      else cout << " ** WARNING triggerReader_cross : trigger name " << list.at(i) << " not in input histogram" << endl;
    }
    return;
}

void triggerReader_cross::printTriggerList()
{
  cout<<endl;
  cout<<"@ bit position - path"<<endl;
  for (unsigned int i = 0; i < _thisSkimTriggers.size(); i++) 
    {
      cout<< i <<" - "<<_thisSkimTriggers.at(i)<<endl;
    }
  cout<<endl;
  return;  
}


// ------------------------------
// Check trigger filters matching
// ------------------------------

bool triggerReader_cross::checkOREleEleNew  (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t trgNoOverlap, Long64_t goodTriggerType1, double pt_tau1, double eta_tau1, double eta_tau2, int *pass_triggerbit)
{
    bool OR      = false;
    bool thisPath      = false;
    bool _trgNoOverlap = false;
    bool match = false;
    bool goodType = false;
    std::string firedPath;
    bool ptCut = false;
    bool etaCut1 = false;
    bool etaCut2 = false;
    
    for (unsigned int i = 0; i < _eeTriggers.size(); i++)
    {
      thisPath = false;
      thisPath = CheckBit (triggerbit_1, _eeTriggers.at(i));
      if (thisPath)
      {
        match = CheckBit (matchFlag1, _eeTriggers.at(i));
        goodType = CheckBit (goodTriggerType1, _eeTriggers.at(i));
        _trgNoOverlap = CheckBit (trgNoOverlap, _eeTriggers.at(i));
        if (match && _trgNoOverlap && goodType)
        {
          firedPath = _allTriggers.at(_eeTriggers.at(i));
          boost::regex re_tau1{"Ele(\\d+)"};
          ptCut = checkPtCutSingle(thisPath, firedPath, re_tau1, pt_tau1, 1.0); // 1.0 GeV from HTT twiki Run2 Legacy analisys
          etaCut1 = (fabs(eta_tau1) < 2.1); //electron threshold
          etaCut2 = (fabs(eta_tau2) < 2.1); //electron threshold
        }
        else
        {
          ptCut = false;
          etaCut1 = false;
          etaCut2 = false;
        }
      }
      if (!(thisPath && match && _trgNoOverlap && goodType && ptCut && etaCut1 && etaCut2)) thisPath = false;
      if (thisPath)
      {
        std::vector<string>::iterator it = std::find(_thisSkimTriggers.begin(), _thisSkimTriggers.end(), _allTriggers.at(_eeTriggers.at(i)));
        int thisPathIdx = std::distance(_thisSkimTriggers.begin(), it);
        *pass_triggerbit |=  1 << thisPathIdx;
        OR = true;
      }
      //if (OR) break; // I want to store the information about all triggers that have fired, without breaking the loop when I find one
    }
    return OR;
}

bool triggerReader_cross::checkORMuEleNew  (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t trgNoOverlap, Long64_t goodTriggerType1, double pt_tau1, double eta_tau1, double eta_tau2, int *pass_triggerbit)
{
    bool OR      = false;
    bool thisPath      = false;
    bool _trgNoOverlap = false;
    bool match = false;
    bool goodType = false;
    std::string firedPath;
    bool ptCut = false;
    bool etaCut1 = false;
    bool etaCut2 = false;
    
    for (unsigned int i = 0; i < _emTriggers.size(); i++)
    {
      thisPath = false;
      thisPath = CheckBit (triggerbit_1, _emTriggers.at(i));
      if (thisPath)
      {
        match = CheckBit (matchFlag1, _emTriggers.at(i));
        goodType = CheckBit (goodTriggerType1, _emTriggers.at(i));
        _trgNoOverlap = CheckBit (trgNoOverlap, _emTriggers.at(i));
        if (match && _trgNoOverlap && goodType)
        {
          firedPath = _allTriggers.at(_mmTriggers.at(i));
          boost::regex re_tau1{"Mu(\\d+)"};
          ptCut = checkPtCutSingle(thisPath, firedPath, re_tau1, pt_tau1, 1.0); // 1.0 GeV from HTT twiki Run2 Legacy analisys
          etaCut1 = (fabs(eta_tau1) < 2.1); //muon threshold
          etaCut2 = (fabs(eta_tau2) < 2.1); //electron threshold
        }
        else
        {
          ptCut = false;
          etaCut1 = false;
          etaCut2 = false;
        }
      }
      if (!(thisPath && match && _trgNoOverlap && goodType && ptCut && etaCut1 && etaCut2)) thisPath = false;
      if (thisPath)
      {
        std::vector<string>::iterator it = std::find(_thisSkimTriggers.begin(), _thisSkimTriggers.end(), _allTriggers.at(_emTriggers.at(i)));
        int thisPathIdx = std::distance(_thisSkimTriggers.begin(), it);
        *pass_triggerbit |=  1 << thisPathIdx;
        OR = true;
      }
      //if (OR) break; // I want to store the information about all triggers that have fired, without breaking the loop when I find one
    }
    return OR;
}


bool triggerReader_cross::checkORMuMuNew  (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t trgNoOverlap, Long64_t goodTriggerType1, double pt_tau1, double eta_tau1, double eta_tau2, int *pass_triggerbit)
{
    bool OR      = false;
    bool thisPath      = false;
    bool _trgNoOverlap = false;
    bool match = false;
    bool goodType = false;
    std::string firedPath;
    bool ptCut = false;
    bool etaCut1 = false;
    bool etaCut2 = false;
  
    for (unsigned int i = 0; i < _mmTriggers.size(); i++)
    {
      thisPath = false;
      thisPath = CheckBit (triggerbit_1, _mmTriggers.at(i));
      if (thisPath)
      {
        match = CheckBit (matchFlag1, _mmTriggers.at(i));
        goodType = CheckBit (goodTriggerType1, _mmTriggers.at(i));
        _trgNoOverlap = CheckBit (trgNoOverlap, _mmTriggers.at(i));
        if (match && _trgNoOverlap && goodType)
        {
          firedPath = _allTriggers.at(_mmTriggers.at(i));
          boost::regex re_tau1{"Mu(\\d+)"};
          ptCut = checkPtCutSingle(thisPath, firedPath, re_tau1, pt_tau1, 1.0); // 1.0 GeV from HTT twiki Run2 Legacy analisys
          etaCut1 = (fabs(eta_tau1) < 2.1); //muon threshold
          etaCut2 = (fabs(eta_tau2) < 2.1); //muon threshold
        }
        else
        {
          ptCut = false;
          etaCut1 = false;
          etaCut2 = false;
        }
      }
      if (!(thisPath && match && _trgNoOverlap && goodType && ptCut && etaCut1 && etaCut2)) thisPath = false;
      if (thisPath)
      {
        std::vector<string>::iterator it = std::find(_thisSkimTriggers.begin(), _thisSkimTriggers.end(), _allTriggers.at(_mmTriggers.at(i)));
        int thisPathIdx = std::distance(_thisSkimTriggers.begin(), it);
        *pass_triggerbit |=  1 << thisPathIdx;
        OR = true;
      }
      //if (OR) break; // I want to store the information about all triggers that have fired, without breaking the loop when I find one
    }
    return OR;
}

bool triggerReader_cross::checkORTauTauNew  (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2, double pt_tau1, double eta_tau1, double pt_tau2, double eta_tau2, int *pass_triggerbit)
{
    bool OR     = false;
    bool thisPath      = false;
    bool match1 = false;
    bool match2 = false;
    bool goodType1 = false;
    bool goodType2 = false;
    bool _trgNoOverlap = false;
    bool match = false;
    bool goodType = false;
    std::string firedPath;
    bool ptCut = false;
    bool etaCut1 = false;
    bool etaCut2 = false;
    for (unsigned int i = 0; i < _ttCrossTriggers.size(); i++)
    {
      thisPath = false;
      thisPath = CheckBit (triggerbit_1, _ttCrossTriggers.at(i));
      if (thisPath)
      {
        match1 = CheckBit (matchFlag1, _ttCrossTriggers.at(i));
        match2 = CheckBit (matchFlag2, _ttCrossTriggers.at(i));
        match = match1 && match2;
        goodType1 = CheckBit (goodTriggerType1, _ttCrossTriggers.at(i));
        goodType2 = CheckBit (goodTriggerType2, _ttCrossTriggers.at(i));
        goodType = goodType1 && goodType2;
        _trgNoOverlap = CheckBit (trgNoOverlap, _ttCrossTriggers.at(i));
        
        if (match && _trgNoOverlap && goodType)
        {
          firedPath = _allTriggers.at(_ttCrossTriggers.at(i));
          boost::regex re_tau1{"Tau(\\d+)|TauHPS(\\d+)"};
          boost::regex re_tau2{"Tau(\\d+)|TauHPS(\\d+)"};
          //ptCut = checkPtCutCross(thisPath, firedPath, re_tau1, re_tau2, pt_tau1, pt_tau2, 5.0, 5.0);
          ptCut = (pt_tau1 > 40. && pt_tau2 > 40.); // suggested by tauTrigger Group
          etaCut1 = (fabs(eta_tau1) < 2.1); //tau threshold
          etaCut2 = (fabs(eta_tau2) < 2.1); //tau threshold
        }
        else
        {
          ptCut = false;
          etaCut1 = false;
          etaCut2 = false;
        }
      }
      if (!(thisPath && match && _trgNoOverlap && goodType && ptCut && etaCut1 && etaCut2)) thisPath = false;
      if (thisPath)
      {
        std::vector<string>::iterator it = std::find(_thisSkimTriggers.begin(), _thisSkimTriggers.end(), _allTriggers.at(_ttCrossTriggers.at(i)));
        int thisPathIdx = std::distance(_thisSkimTriggers.begin(), it);
        *pass_triggerbit |=  1 << thisPathIdx;
        OR = true;
      }
      //if (OR) break; // I want to store the information about all triggers that have fired, without breaking the loop when I find one
    }

    //if (!OR)
    //{
    for (unsigned int i = 0; i < _ttTriggers.size(); i++)
    {
        thisPath = false;
        thisPath = CheckBit (triggerbit_1, _ttTriggers.at(i));
        if (thisPath)
        {
          match1 = CheckBit (matchFlag1, _ttTriggers.at(i));
          match2 = true;
          match = match1 && match2;
          goodType1 = CheckBit (goodTriggerType1, _ttTriggers.at(i));
          goodType2 = true;
          goodType = goodType1 && goodType2;
          _trgNoOverlap = CheckBit (trgNoOverlap, _ttTriggers.at(i));
	    
          if (match && _trgNoOverlap && goodType)
          {
            firedPath = _allTriggers.at(_ttTriggers.at(i));
            boost::regex re_tau1{"Tau(\\d+)|TauHPS(\\d+)"};
            ptCut = checkPtCutSingle(thisPath, firedPath, re_tau1, pt_tau1, 5.0);
            etaCut1 = (fabs(eta_tau1) < 2.1); //tau threshold
            etaCut2 = (fabs(eta_tau2) < 2.1); //tau threshold
          }
          else
          {
            ptCut = false;
            etaCut1 = false;
            etaCut2 = false;
          }
        }
        if (!(thisPath && match && _trgNoOverlap && goodType && ptCut && etaCut1 && etaCut2)) thisPath = false;
        if (thisPath)
        {
          std::vector<string>::iterator it = std::find(_thisSkimTriggers.begin(), _thisSkimTriggers.end(), _allTriggers.at(_ttTriggers.at(i)));
          int thisPathIdx = std::distance(_thisSkimTriggers.begin(), it);
          *pass_triggerbit |=  1 << thisPathIdx;
          OR = true;
        }
        //if (OR) break; // I want to store the information about all triggers that have fired, without breaking the loop when I find one
    }
    //}

    return OR;
}

bool triggerReader_cross::checkORMuTauNew  (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2, double pt_tau1, double eta_tau1, double pt_tau2, double eta_tau2, int *pass_triggerbit)
{
    bool OR     = false;
    bool thisPath = false;
    bool match1 = false;
    bool match2 = false;
    bool goodType1 = false;
    bool goodType2 = false;
    bool _trgNoOverlap = false;
    bool match = false;
    bool goodType = false;
    std::string firedPath;
    bool ptCut = false;
    bool etaCut1 = false;
    bool etaCut2 = false;
    for (unsigned int i = 0; i < _mtCrossTriggers.size(); i++)
    {
      thisPath = false; 
      thisPath = CheckBit (triggerbit_1, _mtCrossTriggers.at(i));
      if (thisPath)
      {
        match1 = CheckBit (matchFlag1, _mtCrossTriggers.at(i));
        match2 = CheckBit (matchFlag2, _mtCrossTriggers.at(i));
        match = match1 && match2;
        goodType1 = CheckBit (goodTriggerType1, _mtCrossTriggers.at(i));
        goodType2 = CheckBit (goodTriggerType2, _mtCrossTriggers.at(i));
        goodType = goodType1 && goodType2;
        _trgNoOverlap = CheckBit (trgNoOverlap, _mtCrossTriggers.at(i));

        if (match && _trgNoOverlap && goodType)
        {
          firedPath = _allTriggers.at(_mtCrossTriggers.at(i));
          boost::regex re_tau1{"Mu(\\d+)"};
          boost::regex re_tau2{"Tau(\\d+)|TauHPS(\\d+)"};
          ptCut = checkPtCutCross(thisPath, firedPath, re_tau1, re_tau2, pt_tau1, pt_tau2, 1.0, 5.0);  // 1.0 GeV from HTT twiki Run2 Legacy analisys
          etaCut1 = (fabs(eta_tau1) < 2.1); //muon threshold
          etaCut2 = (fabs(eta_tau2) < 2.1); //cross trigger tau threshold
        }
        else
        {
          ptCut = false;
          etaCut1 = false;
          etaCut2 = false;
        }
      }
      if (!(thisPath && match && _trgNoOverlap && goodType && ptCut && etaCut1 && etaCut2)) thisPath = false;
      if (thisPath)
      {
        std::vector<string>::iterator it = std::find(_thisSkimTriggers.begin(), _thisSkimTriggers.end(), _allTriggers.at(_mtCrossTriggers.at(i)));
        int thisPathIdx = std::distance(_thisSkimTriggers.begin(), it);
        *pass_triggerbit |=  1 << thisPathIdx;
        OR = true;
      }
      //      if (OR) break;
    }
    
    // if (!OR)
    // {
    for (unsigned int i = 0; i < _mtTriggers.size(); i++)
    {
        thisPath = false;
        thisPath = CheckBit (triggerbit_1, _mtTriggers.at(i));
        if (thisPath)
        {
          match1 = CheckBit (matchFlag1, _mtTriggers.at(i));
          match2 = true;
          match = match1 && match2;
          goodType1 = CheckBit (goodTriggerType1, _mtTriggers.at(i));
          goodType2 = true;
          goodType = goodType1 && goodType2;
          _trgNoOverlap = CheckBit (trgNoOverlap, _mtTriggers.at(i));

          if (match && _trgNoOverlap && goodType)
          {
            firedPath = _allTriggers.at(_mtTriggers.at(i));
            boost::regex re_tau1{"Mu(\\d+)"};
            ptCut = checkPtCutSingle(thisPath, firedPath, re_tau1, pt_tau1, 1.0); // 1.0 GeV from HTT twiki Run2 Legacy analisys
            etaCut1 = (fabs(eta_tau1) < 2.1); //muon threshold
            etaCut2 = (fabs(eta_tau2) < 2.3); //single trigger tau threshold
          }
          else
          {
            ptCut = false;
            etaCut1 = false;
            etaCut2 = false;
          }
        }
        if (!(thisPath && match && _trgNoOverlap && goodType && ptCut && etaCut1 && etaCut2)) thisPath = false;
        if (thisPath)
        {
          std::vector<string>::iterator it = std::find(_thisSkimTriggers.begin(), _thisSkimTriggers.end(), _allTriggers.at(_mtTriggers.at(i)));
          int thisPathIdx = std::distance(_thisSkimTriggers.begin(), it);
          *pass_triggerbit |=  1 << thisPathIdx;
          OR = true;
        }
        //        if (OR) break;
    }
    //}

    return OR;
}


bool triggerReader_cross::checkOREleTauNew  (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2, double pt_tau1, double eta_tau1, double pt_tau2, double eta_tau2, int *pass_triggerbit)
{
    bool OR     = false;
    bool thisPath =false;
    bool match1 = false;
    bool match2 = false;
    bool goodType1 = false;
    bool goodType2 = false;
    bool _trgNoOverlap = false;
    bool match = false;
    bool goodType = false;
    std::string firedPath;
    bool ptCut = false;
    bool etaCut1 = false;
    bool etaCut2 = false;
    for (unsigned int i = 0; i < _etCrossTriggers.size(); i++)
    {
      thisPath = false;
      thisPath = CheckBit (triggerbit_1, _etCrossTriggers.at(i));
      if (thisPath)
      {
        match1 = CheckBit (matchFlag1, _etCrossTriggers.at(i));
        match2 = CheckBit (matchFlag2, _etCrossTriggers.at(i));
        match = match1 && match2;
        goodType1 = CheckBit (goodTriggerType1, _etCrossTriggers.at(i));
        goodType2 = CheckBit (goodTriggerType2, _etCrossTriggers.at(i));
        goodType = goodType1 && goodType2;
        _trgNoOverlap = CheckBit (trgNoOverlap, _etCrossTriggers.at(i));

        if (match && _trgNoOverlap && goodType)
        {
          firedPath = _allTriggers.at(_etCrossTriggers.at(i));
          boost::regex re_tau1{"Ele(\\d+)"};
          boost::regex re_tau2{"Tau(\\d+)|TauHPS(\\d+)"};

          ptCut = checkPtCutCross(thisPath, firedPath, re_tau1, re_tau2, pt_tau1, pt_tau2, 1.0, 5.0); // 1.0 GeV from HTT twiki Run2 Legacy analisys
          etaCut1 = (fabs(eta_tau1) < 2.1); //electron threshold
          etaCut2 = (fabs(eta_tau2) < 2.1); //cross trigger tau threshold
        }
        else
        {
          ptCut = false;
          etaCut1 = false;
          etaCut2 = false;
        }
      }
      if (!(thisPath && match && _trgNoOverlap && goodType && ptCut && etaCut1 && etaCut2)) thisPath = false;
      if (thisPath)
      {
        std::vector<string>::iterator it = std::find(_thisSkimTriggers.begin(), _thisSkimTriggers.end(), _allTriggers.at(_etCrossTriggers.at(i)));
        int thisPathIdx = std::distance(_thisSkimTriggers.begin(), it);
        *pass_triggerbit |=  1 << thisPathIdx;
        OR = true;
      }
      //      if (OR) break;
    }
    //if (!OR)
    // {
    for (unsigned int i = 0; i < _etTriggers.size(); i++)
    {
        thisPath = false;
        thisPath = CheckBit (triggerbit_1, _etTriggers.at(i));
        if (thisPath)
        {
          match1 = CheckBit (matchFlag1, _etTriggers.at(i));
          match2 = true;
          match = match1 && match2;
          goodType1 = CheckBit (goodTriggerType1, _etTriggers.at(i));
          goodType2 = true;
          goodType = goodType1 && goodType2;
          _trgNoOverlap = CheckBit (trgNoOverlap, _etTriggers.at(i));

          if (match && _trgNoOverlap && goodType)
          {
            firedPath = _allTriggers.at(_etTriggers.at(i));
            boost::regex re_tau1{"Ele(\\d+)"};
            ptCut = checkPtCutSingle(thisPath, firedPath, re_tau1, pt_tau1, 1.0); // 1.0 GeV from HTT twiki Run2 Legacy analisys
            etaCut1 = (fabs(eta_tau1) < 2.1); //electron threshold
            etaCut2 = (fabs(eta_tau2) < 2.3); //single trigger tau threshold
          }
          else
          {
            ptCut = false;
            etaCut1 = false;
            etaCut2 = false;
          }
        }
        if (!(thisPath && match && _trgNoOverlap && goodType && ptCut && etaCut1 && etaCut2)) thisPath = false;
        if (thisPath)
        {
          std::vector<string>::iterator it = std::find(_thisSkimTriggers.begin(), _thisSkimTriggers.end(), _allTriggers.at(_etTriggers.at(i)));
          int thisPathIdx = std::distance(_thisSkimTriggers.begin(), it);
          *pass_triggerbit |=  1 << thisPathIdx;
          OR = true;
        }
        //        if (OR) break;
    }
    //}

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

bool triggerReader_cross::checkOR (int pairType, Long64_t triggerbit_1, int *pass_triggerbit, Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2, double pt_tau1, double eta_tau1, double pt_tau2, double eta_tau2)
{

  if(pairType == ((int) OfflineProducerHelper::MuHad) ) return checkORMuTauNew(triggerbit_1, matchFlag1, matchFlag2, trgNoOverlap,goodTriggerType1, goodTriggerType2, pt_tau1, eta_tau1, pt_tau2, eta_tau2, pass_triggerbit);
  else if(pairType == ((int) OfflineProducerHelper::EHad)  ) return checkOREleTauNew(triggerbit_1, matchFlag1, matchFlag2, trgNoOverlap,goodTriggerType1, goodTriggerType2, pt_tau1, eta_tau1, pt_tau2, eta_tau2, pass_triggerbit);
  else if (pairType == ((int) OfflineProducerHelper::HadHad)) return checkORTauTauNew(triggerbit_1, matchFlag1, matchFlag2, trgNoOverlap,goodTriggerType1, goodTriggerType2, pt_tau1, eta_tau1, pt_tau2, eta_tau2, pass_triggerbit);
  else if (pairType == ((int) OfflineProducerHelper::EMu)   ) return checkORMuEleNew (triggerbit_1,matchFlag1,trgNoOverlap,goodTriggerType1,pt_tau1,eta_tau1,eta_tau2, pass_triggerbit);
  else if (pairType == ((int) OfflineProducerHelper::EE)    ) return checkOREleEleNew(triggerbit_1,matchFlag1,trgNoOverlap,goodTriggerType1,pt_tau1,eta_tau1,eta_tau2, pass_triggerbit);
  else if (pairType == ((int) OfflineProducerHelper::MuMu)  ) return checkORMuMuNew  (triggerbit_1,matchFlag1,trgNoOverlap,goodTriggerType1,pt_tau1,eta_tau1,eta_tau2, pass_triggerbit);
  else
    {
      cout << " ** WARNING!! Pair type " << pairType << " not implemented for trigger" << endl;
      return false;
    }
}


// ----------------------------
//     trigger pT cut check
// ----------------------------

bool triggerReader_cross::checkPtCutCross (bool OR, std::string firedPath, boost::regex re_tau1, boost::regex re_tau2, double pt_tau1, double pt_tau2, double thr1, double thr2)
{
    bool result = false;
    boost::smatch what1;
    boost::smatch what2;

    if ( boost::regex_search(firedPath, what1, re_tau1) && boost::regex_search(firedPath, what2, re_tau2) )
    {
        double ptcut_tau1; 
        if(what1.length(1) != 0) ptcut_tau1 = boost::lexical_cast<double>(what1[1]) + thr1; // this check is necessary to deal with TauHPSNN triggers
        else                     ptcut_tau1 = boost::lexical_cast<double>(what1[2]) + thr1;
	 
        double ptcut_tau2; 
        if(what2.length(1) != 0) ptcut_tau2 = boost::lexical_cast<double>(what2[1]) + thr2; // this check is necessary to deal with TauHPSNN triggers
        else                     ptcut_tau2 = boost::lexical_cast<double>(what2[2]) + thr2;

        //cout << "************** Double REGEX " << endl;
        //cout << "************** REGEX firedPath : " << firedPath  << endl;
        //cout << "************** REGEX pt taus1/2: " << pt_tau1    << "/" << pt_tau2    << endl;
        //cout << "************** REGEX pt cuts1/2: " << ptcut_tau1 << "/" << ptcut_tau2 << endl;

        if ( pt_tau1> ptcut_tau1 && pt_tau2> ptcut_tau2 ) result = true;
        else                                              result = false;
    }
    else if (OR) // path is fired correctly but can not extract the right pt cut from the name
    {
        cout << " *** ERROR: could not extract the trigger pt cut from:" << firedPath << " - returning FALSE" << endl;
        result = false;
    }
    else
        result = false;

    //cout << "************** REGEX accepted: " << (OR && result) << endl;
    return (OR && result);
}

bool triggerReader_cross::checkPtCutSingle (bool OR, std::string firedPath, boost::regex re_tau1, double pt_tau1, double thr1)
{
    bool result = false;
    boost::smatch what1;

    if ( boost::regex_search(firedPath, what1, re_tau1) )
    {
        double ptcut_tau1;	 
	if(what1.length(1) != 0) ptcut_tau1 = boost::lexical_cast<double>(what1[1]) + thr1; // this check is necessary to deal with TauHPSNN triggers
	else                     ptcut_tau1 = boost::lexical_cast<double>(what1[2]) + thr1;

        //cout << "************** Single REGEX " << endl;
        //cout << "************** REGEX firedPath: " << firedPath  << endl;
        //cout << "************** REGEX pt tau1  : " << pt_tau1    << endl;
        //cout << "************** REGEX pt cut1  : " << ptcut_tau1 << endl;

        if ( pt_tau1> ptcut_tau1 ) result = true;
        else                       result = false;
    }
    else if (OR) // path is fired correctly but can not extract the right pt cut from the name
    {
        cout << " *** ERROR: could not extract the trigger pt cut from:" << firedPath << " - returning FALSE" << endl;
        result = false;
    }
    else
        result = false;

    //cout << "************** REGEX accepted: " << (OR && result) << endl;
    return (OR && result);
}



// ------------------------------------------
//     List HLT path for debuggin purposes
// ------------------------------------------


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


// --------------------------------------------
//  Check if the fired path is a VBF path
//  Same function as checkORTauTauNew but with
//  only the VBF triggers list
// --------------------------------------------

bool triggerReader_cross::isVBFfired  (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2, double pt_tau1, double eta_tau1, double pt_tau2, double eta_tau2)
{
  bool OR = false;
  bool match1 = false;
  bool match2 = false;
  bool goodType1 = false;
  bool goodType2 = false;
  bool _trgNoOverlap = false;
  bool match = false;
  bool goodType = false;
  std::string firedPath;
  bool ptCut = false;
  bool etaCut1 = false;
  bool etaCut2 = false;
  for (unsigned int i = 0; i < _vbfTriggers.size(); i++)
  {
    OR = CheckBit (triggerbit_1, _vbfTriggers.at(i));
    if (OR)
    {
      match1 = CheckBit (matchFlag1, _vbfTriggers.at(i));
      match2 = CheckBit (matchFlag2, _vbfTriggers.at(i));
      match = match1 && match2;
      goodType1 = CheckBit (goodTriggerType1, _vbfTriggers.at(i));
      goodType2 = CheckBit (goodTriggerType2, _vbfTriggers.at(i));
      goodType = goodType1 && goodType2;
      _trgNoOverlap = CheckBit (trgNoOverlap, _vbfTriggers.at(i));

      if (match && _trgNoOverlap && goodType)
      {
        firedPath = _allTriggers.at(_vbfTriggers.at(i));
        boost::regex re_tau1{"Tau(\\d+)|TauHPS(\\d+)"};
        boost::regex re_tau2{"Tau(\\d+)|TauHPS(\\d+)"};
        ptCut = checkPtCutCross(OR, firedPath, re_tau1, re_tau2, pt_tau1, pt_tau2, 5.0, 5.0);
        etaCut1 = (fabs(eta_tau1) < 2.1); //tau threshold
        etaCut2 = (fabs(eta_tau2) < 2.1); //tau threshold
      }
      else
      {
        ptCut = false;
        etaCut1 = false;
        etaCut2 = false;
      }
    }
    if (!(OR && match && _trgNoOverlap && goodType && ptCut && etaCut1 && etaCut2)) OR = false;
    if (OR) break;
  }
  return OR;
}

void triggerReader_cross::listVBF (Long64_t triggerbit_1, Long64_t matchFlag1, Long64_t matchFlag2, Long64_t trgNoOverlap, Long64_t goodTriggerType1, Long64_t goodTriggerType2, double pt_tau1, double eta_tau1, double pt_tau2, double eta_tau2)
{
  bool OR = false;
  bool match1 = false;
  bool match2 = false;
  bool goodType1 = false;
  bool goodType2 = false;
  bool _trgNoOverlap = false;
  bool match = false;
  bool goodType = false;
  std::string firedPath;
  bool ptCut = false;
  bool etaCut1 = false;
  bool etaCut2 = false;
  for (unsigned int i = 0; i < _vbfTriggers.size(); i++)
  {
    OR = CheckBit (triggerbit_1, _vbfTriggers.at(i));
    if (OR)
      cout <<"** trg: VBF fired: "<<_allTriggers.at(_vbfTriggers.at(i))<<endl;
    else
      cout <<"** trg: VBF NOT fired: "<<_allTriggers.at(_vbfTriggers.at(i))<<endl;

    if (OR)
    {
      match1 = CheckBit (matchFlag1, _vbfTriggers.at(i));
      match2 = CheckBit (matchFlag2, _vbfTriggers.at(i));
      match = match1 && match2;
      goodType1 = CheckBit (goodTriggerType1, _vbfTriggers.at(i));
      goodType2 = CheckBit (goodTriggerType2, _vbfTriggers.at(i));
      goodType = goodType1 && goodType2;
      _trgNoOverlap = CheckBit (trgNoOverlap, _vbfTriggers.at(i));

      if (match && _trgNoOverlap && goodType)
      {
        firedPath = _allTriggers.at(_vbfTriggers.at(i));
        boost::regex re_tau1{"Tau(\\d+)|TauHPS(\\d+)"};
        boost::regex re_tau2{"Tau(\\d+)|TauHPS(\\d+)"};
        ptCut = checkPtCutCross(OR, firedPath, re_tau1, re_tau2, pt_tau1, pt_tau2, 5.0, 5.0);
        etaCut1 = (fabs(eta_tau1) < 2.1); //tau threshold
        etaCut2 = (fabs(eta_tau2) < 2.1); //tau threshold
      }
      else
      {
        ptCut = false;
        etaCut1 = false;
        etaCut2 = false;
      }
    }
    cout <<"   matchFlag    "<< match<<endl;
    cout <<"   trgNoOverlap "<< _trgNoOverlap<<endl;
    cout <<"   goodType     "<< goodType<<endl;
    cout <<"   ptCut        "<< ptCut<<endl;
    cout <<"   etaCut1      "<< (etaCut1 && etaCut2)<<endl;
  }
}