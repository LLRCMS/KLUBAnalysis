// list of all b jet regression variables to be used for MVA evaluation

#include "TMVA/MsgLogger.h"
#include "TMVA/Config.h"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"

struct bJetRegrVars {
    float Jet_pt;
    float Jet_corr;
    float rho;
    float Jet_eta;
    float Jet_mt;
    float Jet_leadTrackPt;
    float Jet_leptonPtRel;
    float Jet_leptonPt ;
    float Jet_leptonDeltaR;
    float Jet_neHEF;
    float Jet_neEmEF;
    float Jet_chMult;
    float Jet_vtxPt;
    float Jet_vtxMass ;
    float Jet_vtx3dL;
    //int Jet_vtxNtrk ;
    float Jet_vtxNtrk ;
    float Jet_vtx3deL ;

    // ---- functions
    void setReader (TMVA::Reader* reader);
};