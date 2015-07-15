#include "../interface/HistoManager.h"


using namespace std;

HistoManager::HistoManager (const char* tag) : _tag (tag) {}

HistoManager::~HistoManager()
{
    // delete all allocated objs
    // for this, always store pointers to studd allocated on the heap!!
    for (it_type it = _map.begin(); it != _map.end(); ++it)
    {
        delete (it->second);
    }
}

int HistoManager::AddElement (TObject* ptr, const char* objTag)
{
    string name (MakeStoredName(objTag));
    
    // not optimal, but I hope to have not so many histos!
    if (_map.find (name) != _map.end())
    {
        cout << "TObject with name " << objTag << " already exists, no object added" << endl;
        return -1;
    }
    
    else
    {
        _map[name] = ptr;
        return 0;
    }
}

TObject* HistoManager::GetElement (const char * objTag)
{
    it_type it = _map.find (MakeStoredName(objTag));
    if (it != _map.end())
        return it->second;
    else
    {
        cout << "No TObject found with name " << objTag << endl;
        return NULL;
    }
}

TH1F * 
HistoManager::AddNewHisto (const char* name, const char* title, 
                           int nbinsx, double xlow, double xup,
                           int color, bool isSignal,
                           TString titleX, TString titleY)
{
    string fullName = MakeStoredName (name);
    TH1F* h = new TH1F (fullName.c_str(), title, nbinsx, xlow, xup);
    h->Sumw2 () ;    
    if (!isSignal)
      {//background
        h->SetFillStyle (1001) ;
        h->SetFillColor (color) ;
        h->SetLineColor (color) ;
      }else{
        h->SetFillStyle (0) ;
        h->SetMarkerColor (color) ;
        h->SetLineColor (color) ;
        h->SetLineWidth (2) ;
        h->SetMarkerStyle (20) ;
      }
    h->GetXaxis ()->SetTitle (titleX) ;
    h->GetYaxis ()->SetTitle (titleY) ;

    int i = AddElement (h, name);
    if (i == -1) delete h; // if not added
    return h ;
}

TH1F* HistoManager::GetHisto(const char* name)
{
    return ((TH1F*) GetElement(name));
}

string HistoManager::MakeStoredName(const char * objTag)
{
    string name (_tag);
    name += "_";
    name += objTag;
    return name;

}

void HistoManager::SaveAllToFile (TFile* fOut)
{
    fOut->cd();
    for (it_type it = _map.begin(); it != _map.end(); ++it)
    {
        it->second->Write();
    }
}
