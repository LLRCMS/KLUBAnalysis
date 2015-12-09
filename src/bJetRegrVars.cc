#include "bJetRegrVars.h"

void bJetRegrVars::setReader (TMVA::Reader* reader)
{
     reader->AddVariable("Jet_pt", &Jet_pt);
     reader->AddVariable("Jet_corr", &Jet_corr);
     reader->AddVariable("rho", &rho);
     reader->AddVariable("Jet_eta", &Jet_eta);
     reader->AddVariable("Jet_mt", &Jet_mt);
     reader->AddVariable("Jet_leadTrackPt", &Jet_leadTrackPt);
     reader->AddVariable("Jet_leptonPtRel", &Jet_leptonPtRel);
     reader->AddVariable("Jet_leptonPt", &Jet_leptonPt);
     reader->AddVariable("Jet_leptonDeltaR", &Jet_leptonDeltaR);
     reader->AddVariable("Jet_neHEF", &Jet_neHEF);
     reader->AddVariable("Jet_neEmEF", &Jet_neEmEF);
     reader->AddVariable("Jet_chMult", &Jet_chMult);
     reader->AddVariable("Jet_vtxPt", &Jet_vtxPt);
     reader->AddVariable("Jet_vtxMass", &Jet_vtxMass);
     reader->AddVariable("Jet_vtx3dL", &Jet_vtx3dL);
     reader->AddVariable("Jet_vtxNtrk", &Jet_vtxNtrk);
     reader->AddVariable("Jet_vtx3deL", &Jet_vtx3deL);
     return;
}