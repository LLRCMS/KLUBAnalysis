// class to derive event weights for b tag
// for more details check https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods#b_tagging_efficiency_in_MC_sampl

#include <string>
#include "BTagCalibrationStandalone.h"

class bTagSF
{
    public:
        enum WP {loose = 0, medium = 1, tight = 2};
        enum SFsyst {central = 0, up = 1, down = 2};
        bTagSF(std::string SFfilename);
        float getSF (WP wpt, SFsyst syst, int jetFlavor, float pt, float eta);
        //float getEff (WP wpt, int jetFlavor, float pt, float eta); // FIXME: to do: retrieve MC efficiency from a data format
    private:
        BTagCalibration m_calib;
        BTagCalibrationReader m_reader[3]; // 0: loose, 1: medium, 2: tight
        BTagCalibrationReader m_reader_up[3]; // 0: loose, 1: medium, 2: tight
        BTagCalibrationReader m_reader_do[3]; // 0: loose, 1: medium, 2: tight
};
