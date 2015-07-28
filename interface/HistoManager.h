/*
** author: L.Cadamuro (LLR)
** date:   11/06/2015
** 
** Class to manage multiple related histos.
** All histos are referenced with a std::map and retrieved using a string tag
** An unique class string tag is also prepended to make all histos unique even when more
** HistoManager objects are created
**
** For more flexibility the map stores pointers to TObjects even if in general only TH1
** will be stored.
*/

#ifndef HISTOMANAGER_H
#define HISTOMANAGER_H

#include <iostream>
#include <map>

#include "TObject.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

typedef std::map<std::string, TObject*>::iterator it_type;

class HistoManager
{
    public:
        HistoManager (const char* tag);
        ~HistoManager();
        int AddElement (TObject* ptr, const char* objTag); // 0: added, -1: not added
        TObject* GetElement (const char* objTag);
        TH1F * AddNewHisto (const char* name, const char* title, 
                            int nbinsx, double xlow, double xup,
                            int color = 2, int isSignal = 0,
                            TString titleX = "",
                           TString titleY = "") ; // creates a new histo
        void AddNew2DHisto (const char* name, const char* title, 
                          int nbinsx, double xlow, double xup,
                          int nbinsy, double ylow, double yup,
                          int color = 2, int isSignal = 0,
                          TString titleX = "",
                          TString titleY = "",
                          TString titleZ = "") ; // creates a new histo
        TH1F* GetHisto(const char* name);
        TH2F* Get2DHisto(const char* name);
        void SaveAllToFile (TFile* fOut);
        
    private:
        std::string _tag;
        std::map <std::string, TObject*> _map;
        std::string MakeStoredName (const char * objTag) ; // adds internal tag to name for the search
        
};

#endif
