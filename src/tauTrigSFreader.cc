#include "tauTrigSFreader.h"
#include <iostream>
using namespace std;

tauTrigSFreader::tauTrigSFreader(std::string rfilename)
{
    fIn_ = new TFile(rfilename.c_str());

    fMC_[0] = (TF1*) fIn_->Get("mediumMVAIso_eff_MC_DM0");
    fMC_[1] = (TF1*) fIn_->Get("mediumMVAIso_eff_MC_DM1");
    fMC_[2] = (TF1*) fIn_->Get("mediumMVAIso_eff_MC_DM10");

    fDataBG_[0] = (TF1*) fIn_->Get("mediumMVAIso_eff_DataBCDEFG_DM0");
    fDataBG_[1] = (TF1*) fIn_->Get("mediumMVAIso_eff_DataBCDEFG_DM1");
    fDataBG_[2] = (TF1*) fIn_->Get("mediumMVAIso_eff_DataBCDEFG_DM10");

    fDataH_[0] = (TF1*) fIn_->Get("mediumMVAIso_eff_DataH_DM0");
    fDataH_[1] = (TF1*) fIn_->Get("mediumMVAIso_eff_DataH_DM1");
    fDataH_[2] = (TF1*) fIn_->Get("mediumMVAIso_eff_DataH_DM10");
 
    fracBG_ = 27.915/36.772;
    fracH_ = 8.857/36.772;
    
    // this is true as long as all the functions have the same range - but is is the case for now
    xmin_ = fMC_[0]->GetXmin();
    xmax_ = fMC_[0]->GetXmax();

    // # B 5.892
    // # C 2.646
    // # D 4.353
    // # E 4.117
    // # F 3.186
    // # G 7.721
    // # H 8.857
    // # TOT: 36.772

    // # B-G : 27.915 (frac: 0.7591)
    // # H   : 8.857  (frac: 0.24086)

    // # fBG = 27.915/36.772
    // # fH = 8.857/36.772
}

tauTrigSFreader::~tauTrigSFreader()
{}

double tauTrigSFreader::getSF(double pt, int decayMode)
{
    int idx = 0;
    if      (decayMode == 0)  idx = 0;
    else if (decayMode == 1)  idx = 1;
    else if (decayMode == 10) idx = 2;
    else
        cerr << "tauTrigSFreader::getSF ** ERROR ** cannot understand decay mode " << decayMode << " will consider 0" << endl;

    // because outside of its range, Eval return 0
    if (pt < xmin_) pt = xmin_ + 0.1;
    if (pt > xmax_) pt = xmax_ - 0.1;

    double effMC     = fMC_[idx]->Eval(pt);
    double effDataBG = fDataBG_[idx]->Eval(pt);
    double effDataH  = fDataH_[idx]->Eval(pt);

    double SF = (effMC > 0 ? ( (fracBG_*effDataBG + fracH_*effDataH) / effMC ) : 0 );
    return SF;
}