// class to derive event weights for b tag
// for more details check https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods#b_tagging_efficiency_in_MC_sampl

#include <string>
#include <vector>
#include <utility>
#include "BTagCalibrationStandalone.h"
#include "TFile.h"
#include "TH1F.h"

class bTagSF
{
    public:
        enum WP {loose = 0, medium = 1, tight = 2};
        enum SFsyst {central = 0, up = 1, down = 2};
        bTagSF(std::string SFfilename);
        bTagSF(std::string SFfilename, std::string effFileName, std::string effHistoTag);
        ~bTagSF();
        float getSF (WP wpt, SFsyst syst, int jetFlavor, float pt, float eta);
        float getEff (WP wpt, int jetFlavor, int channel, float pt, float eta); // FIXME: to do: retrieve MC efficiency from a data format
   
        std::vector<float> getEvtWeight (std::vector <std::pair <int, float> >& jets_and_btag, std::vector<float> *jets_px, std::vector<float> *jets_py, std::vector<float> *jets_pz, std::vector<float> *jets_e, std::vector<int> *jets_HadronFlavour, int channel);
    private:
        // related to scale factors
        BTagCalibration m_calib;
        BTagCalibrationReader m_reader[3]; // 0: loose, 1: medium, 2: tight
        BTagCalibrationReader m_reader_up[3]; // 0: loose, 1: medium, 2: tight
        BTagCalibrationReader m_reader_do[3]; // 0: loose, 1: medium, 2: tight

        // related to b tag efficiency
        TFile* m_fileEff;
        TH1F* m_hEff [3][3][3]; // [0: loose, 1: medium, 2: tight] [0: b, 1: c 2: udsg] [0: MuTau, 1: EleTau, :2: TauTau]
};
