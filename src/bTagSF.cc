#include "bTagSF.h"

bTagSF::bTagSF(std::string SFfilename) : 
    
    m_calib(SFfilename.c_str() , "CSVv2") ,
    m_reader {
    BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "comb", "central"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "comb", "central"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "comb", "central")} ,
    m_reader_up {
    BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "comb", "up"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "comb", "up"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "comb", "up")} ,
    m_reader_do {
    BTagCalibrationReader(&m_calib , BTagEntry::OP_LOOSE,  "comb", "down"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_MEDIUM, "comb", "down"),
    BTagCalibrationReader(&m_calib , BTagEntry::OP_TIGHT,  "comb", "down")}
{}

float bTagSF::getSF (WP wpt, SFsyst syst, int jetFlavor, float pt, float eta)
{
    float SF = 1.0;
    
    BTagEntry::JetFlavor flav;
    if (abs(jetFlavor) == 5) flav = BTagEntry::FLAV_B;
    else if (abs(jetFlavor) == 4) flav = BTagEntry::FLAV_C;
    else flav = BTagEntry::FLAV_UDSG;

    if (syst == central)
        SF = m_reader[(int)wpt].eval(flav, eta, pt);

    else if (syst == up)
        SF = m_reader_up[(int)wpt].eval(flav, eta, pt);
    
    else if (syst == down)
        SF = m_reader_do[(int)wpt].eval(flav, eta, pt);
    
    // double uncertainty up/down if out of some boundaries
    if (syst != central)
    {
        if (flav == BTagEntry::FLAV_B || flav == BTagEntry::FLAV_C)
        {
            if (pt < 30.0 || pt > 670.0 ) SF *= 2.0;
        }
        if (flav == BTagEntry::FLAV_UDSG)
        {
            if (pt < 20.0 || pt > 1000.0 ) SF *= 2.0;
        }
    }

    return SF;
}

/*
float bTagSF::getEff (WP wpt, int jetFlavor, float pt, float eta)
{}
*/

