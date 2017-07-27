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
        //bTagSF(std::string SFfilename);
        bTagSF(std::string SFfilename, std::string effFileName, std::string effHistoTag, std::string WPset="80X_ICHEP_2016");
        ~bTagSF();
        float getSF (WP wpt, SFsyst syst, int jetFlavor, float pt, float eta);
        float getEff (WP wpt, int jetFlavor, int channel, float pt, float eta); // FIXME: to do: retrieve MC efficiency from a data format
        void SetWPset(std::string WPset);
        void SetWPset(double loose, double medium, double tight);
   
        std::vector<float> getEvtWeight (std::vector <std::pair <int, float> >& jets_and_btag, std::vector<float> *jets_px, std::vector<float> *jets_py, std::vector<float> *jets_pz, std::vector<float> *jets_e, std::vector<int> *jets_HadronFlavour, int channel);
    private:
        // related to scale factors
        BTagCalibration m_calib;

        /*
        // for b jets
        BTagCalibrationReader m_reader[3]; // 0: loose, 1: medium, 2: tight
        BTagCalibrationReader m_reader_up[3]; // 0: loose, 1: medium, 2: tight
        BTagCalibrationReader m_reader_do[3]; // 0: loose, 1: medium, 2: tight

        // for c jets
        BTagCalibrationReader m_reader_c[3]; // 0: loose, 1: medium, 2: tight
        BTagCalibrationReader m_reader_c_up[3]; // 0: loose, 1: medium, 2: tight
        BTagCalibrationReader m_reader_c_do[3]; // 0: loose, 1: medium, 2: tight

        // for udsg jets
        BTagCalibrationReader m_reader_udsg[3]; // 0: loose, 1: medium, 2: tight
        BTagCalibrationReader m_reader_udsg_up[3]; // 0: loose, 1: medium, 2: tight
        BTagCalibrationReader m_reader_udsg_do[3]; // 0: loose, 1: medium, 2: tight

        BTagCalibrationReader* m_readers [3][3][3]; // [b, c, udsg] [central, up, down] [loose, medium, tight] 
        */
        BTagCalibrationReader m_readers [3]; // new 2016: [loose, medium, tight] 

        // related to b tag efficiency
        TFile* m_fileEff;
        TH1F* m_hEff [3][3][4]; // [0: loose, 1: medium, 2: tight] [0: b, 1: c 2: udsg] [0: MuTau, 1: EleTau, :2: TauTau, 3: ALL]

        double _WPtag[3]; // loose, medium tight WP
};
